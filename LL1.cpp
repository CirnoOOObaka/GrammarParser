#include <LL1.h>

ifstream fin_ll1;
ofstream fout_ll1;
ofstream ftable_ll1;

// 产生式类
class WF_ll1
{
public:
    string left;
    set<string> right;
    WF_ll1(char s[])
    {
        left = s;
    }
    void print_ll1()
    {
        fout_ll1 << left.c_str() << "->";
        set<string>::iterator it = right.begin();
        if(right.begin()!= right.end())
        {
            fout_ll1 << it->c_str();
            it++;
        }
        for(; it != right.end() ; it++)
            fout_ll1 << "|" << it->c_str();
        fout_ll1 << endl;
    }
    void insert(char s[])
    {
        right.insert(s);
    }
};

// 变量定义
map<string,set<char>> first;
map<string,set<char>> follow;
map<string,int> VN_dic;
vector<WF_ll1> VN_set;
bool used_ll1[MAX];
vector<map<char,string>> predict_table;

// 函数定义
void dfs_ll1(int x)
{
    if(used_ll1[x]) return;
    used_ll1[x] = 1;
    string& left = VN_set[x].left;
    set<string>& right = VN_set[x].right;
    set<string>::iterator it = right.begin();
    for(; it!= right.end() ; it++)
        for(int i = 0 ; i < it->length() ; i++)
        {
            if(!isupper(it->at(i)) && it->at(i) != '\'')
            {
                first[left].insert(it->at(i));
                break;
            }
            if(isupper(it->at(i)))
            {
                int y;
                if(i != it->length()-1 && it->at(i+1) == '\'')
                    y = VN_dic[it->substr(i,2)]-1;
                else y = VN_dic[it->substr(i,1)]-1;
                string& tleft = VN_set[y].left;
                dfs_ll1(y);
                set<char>& temp = first[tleft];
                set<char>::iterator it1 = temp.begin();
                bool flag = true;
                for(; it1 != temp.end() ; it1++)
                {
                    if(*it1 == '~') flag = false;
                    first[left].insert(*it1);
                }
                if(flag) break;
            }
            else continue;
        }
}

void make_first_ll1()
{
    memset(used_ll1 , 0 , sizeof(used_ll1));
    for(int i = 0 ; i < VN_set.size() ; i++)
        dfs_ll1(i);
    fout_ll1 << "【FIRST集】" << endl;
    map<string,set<char> >::iterator it = first.begin();
    for(; it != first.end() ; it++)
    {
        fout_ll1 << "FIRST(" << it->first.c_str() << ")={";
        set<char> & temp = it->second;
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for(; it1 != temp.end() ; it1++)
        {
            if(flag)
                fout_ll1 << ",";
            fout_ll1 << *it1;
            flag = true;
        }
        fout_ll1 << "}" << endl;
    }
}

void append_ll1(const string& str1 , const string& str2)
{
    set<char>& from = follow[str1];
    set<char>& to = follow[str2];
    set<char>::iterator it = from.begin();
    for(; it != from.end() ; it++)
        to.insert(*it);
}

void make_follow_ll1()
{
    while(true)
    {
        bool goon = false;
        for(int i = 0 ; i < VN_set.size() ; i++)
        {
            string& left = VN_set[i].left;
            set<string>& right = VN_set[i].right;
            set<string>::iterator it = right.begin();
            for(; it!= right.end() ; it++)
            {
                bool flag = true;
                const string& str = *it;
                for(int j = it->length()-1 ; j >= 0 ; j--)
                {
                    if(str[j] == '\'')
                    {
                        int x = VN_dic[it->substr(j-1,2)]-1;
                        if(flag)
                        {
                            int tt = follow[it->substr(j-1,2)].size();
                            append_ll1(left , it->substr(j-1,2));
                            int tt1 = follow[it->substr(j-1,2)].size();
                            if(tt1 > tt) goon = true;
                            if(!VN_set[x].right.count("~"))
                                flag = false;
                        }
                        for(int k = j+1 ; k < it->length() ; k++)
                        {
                            if(isupper(str[k]))
                            {
                                string id;
                                if(k != it->length()-1 && str[k+1] == '\'')
                                    id = it->substr(k,2);
                                else id = it->substr(k,1);
                                set<char>& from = first[id];
                                set<char>& to = follow[it->substr(j-1,2)];
                                int tt = to.size();
                                set<char>::iterator it1 = from.begin();
                                for(; it1 != from.end() ; it1++)
                                    if(*it1 != '~')
                                        to.insert(*it1);
                                int tt1 = follow[it->substr(j-1,2)].size();
                                if(tt1 > tt) goon = true;
                                if(!VN_set[VN_dic[id]-1].right.count("~"))
                                    break;
                            }
                            else if(str[k] != '\'')
                            {
                                int tt = follow[it->substr(j-1,2)].size();
                                follow[it->substr(j-1,2)].insert(str[k]);
                                int tt1 = follow[it->substr(j-1,2)].size();
                                if(tt1 > tt)
                                    goon = true;
                                break;
                            }
                            else continue;
                        }
                        j--;
                    }
                    else if(isupper(str[j]))
                    {
                        int x = VN_dic[it->substr(j,1)]-1;
                        if(flag)
                        {
                            int tt = follow[it->substr(j,1)].size();
                            append_ll1(left , it->substr(j,1));
                            if(!VN_set[x].right.count("~"))
                                flag = false;
                            int tt1 = follow[it->substr(j,1)].size();
                            if(tt1 > tt) goon = true;
                        }
                        for(int k = j+1 ; k < it->length() ; k++)
                        {
                            if(isupper(str[k]))
                            {
                                string id;
                                if(k != it->length()-1 && str[k+1] == '\'')
                                    id = it->substr(k,2);
                                else id = it->substr(k,1);
                                set<char>& from = first[id];
                                set<char>& to = follow[it->substr(j,1)];
                                set<char>::iterator it1 = from.begin();
                                int tt = follow[it->substr(j,1)].size();
                                for(; it1 != from.end() ; it1++)
                                    if(*it1 != '~')
                                        to.insert(*it1);
                                int tt1 = follow[it->substr(j,1)].size();
                                if(tt1 > tt) goon = true;
                                if(!VN_set[VN_dic[id]-1].right.count("~"))
                                    break;
                            }
                            else if(str[k] != '\'')
                            {
                                int tt = follow[it->substr(j,1)].size();
                                follow[it->substr(j,1)].insert(str[k]);
                                int tt1 = follow[it->substr(j,1)].size();
                                if(tt1 > tt) goon = true;
                                break;
                            }
                            else continue;
                        }
                    }
                    else flag = false;
                }
            }
        }
        if(!goon) break;
    }
    fout_ll1 << "【FOLLOW集】" << endl;
    map<string,set<char> >::iterator it = follow.begin();
    for(; it != follow.end() ; it++)
    {
        fout_ll1 << "FOLLOW(" << it->first.c_str() << ")={";
        set<char> & temp = it->second;
        temp.insert('#');
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for(; it1 != temp.end() ; it1++)
        {
            if(flag)
                fout_ll1 << ",";
            fout_ll1 << *it1;
            flag = true;
        }
        fout_ll1 << "}" << endl;
    }
}

bool check_first_ll1(const string& text , char ch)
{
    for(int i = 0 ; i < text.length() ; i++)
    {
        bool hasEmpty = false;
        if(!isupper(text[i]) && text[i] != '\'')
        {
            if(text[i] != ch) return false;
            else return true;
        }
        else if(isupper(text[i]))
        {
            string temp;
            if(i != text.length()-1 && text[i+1] == '\'')
                temp = text.substr(i,2);
            else
                temp = text.substr(i,1);
            set<char>& dic = first[temp];
            set<char>::iterator it = dic.begin();
            for(; it != dic.end() ; it++)
            {
                if(*it == '~') hasEmpty = true;
                if(*it == ch) return true;
            }
            if(!hasEmpty) break;
        }
        else continue;
    }
    return false;
}

bool check_follow_ll1(const string& text , char ch)
{
    set<char>& dic = follow[text];
    set<char>::iterator it = dic.begin();
    for(; it != dic.end() ; it++)
        if(*it == ch) return true;
    return false;
}

void make_table_ll1()
{
    fout_ll1 << "【LL(1)分析表】" << endl;
    map<char,string> temp;
    vector<char> letter;
    bool vis[500];
    memset(vis , 0 , sizeof(vis));
    for(int i = 0 ; i < VN_set.size() ; i++)
    {
        set<string>& right = VN_set[i].right;
        set<string>::iterator it = right.begin();
        for(; it != right.end() ; it++)
            for(int j = 0 ; j < it->length() ; j++)
                if(!isupper(it->at(j)) && it->at(j) != '\'')
                {
                    if(vis[it->at(j)]) continue;
                    vis[it->at(j)] = true;
                    letter.push_back(it->at(j));
                }
    }
    for(int i = 0 ; i < VN_set.size() ; i++)
    {
        temp.clear();
        string& left = VN_set[i].left;
        set<string>& right = VN_set[i].right;
        set<string>::iterator it = right.begin();
        for(; it != right.end() ; it++)
            for(int j = 0 ; j < letter.size() ; j++)
            {
                //cout << *it << " " <<  letter[j] << endl;
                if(check_first_ll1(*it , letter[j]))
                {
                    //cout << "YES" << endl;
                    temp[letter[j]] = *it;
                }
                if(it->at(0) == '~' && check_follow_ll1(left, letter[j]))
                    temp[letter[j]] = *it;
            }
        predict_table.push_back(temp);
    }
    // save to complete log
    for(int i = 0 ; i <=(letter.size()+1)*10 ; i++)
        fout_ll1 << "-";
    fout_ll1 << endl;
    fout_ll1 << "|" << setw(9) << "|";
    for(int i = 0 ; i < letter.size() ; i++)
        fout_ll1 << setw(5) << letter[i] << setw(5) << "|";
    fout_ll1 << endl;
    for(int i = 0 ; i <=(letter.size()+1)*10 ; i++)
        fout_ll1 << "-";
    fout_ll1 << endl;
    for(int i = 0 ; i < VN_set.size() ; i++)
    {
        fout_ll1 << "|" << setw(5) << VN_set[i].left.c_str() << setw(4) << "|";
        for(int j = 0 ; j < letter.size() ; j ++)
            if(predict_table[i].count(letter[j]))
                fout_ll1 << setw(7) << predict_table[i][letter[j]].c_str() << setw(3) << "|";
            else
                fout_ll1 << setw(10) << "|";
        fout_ll1 << endl;
        for(int i = 0 ; i <=(letter.size()+1)*10 ; i++)
            fout_ll1 << "-";
        fout_ll1 << endl;
    }
    // save to table log
    ftable_ll1 << "符合 LL(1) 文法" << endl << endl;
    ftable_ll1 << "【LL(1)分析表】" << endl;
    for(int i = 0 ; i <=(letter.size()+1)*10 ; i++)
        ftable_ll1 << "-";
    ftable_ll1 << endl;
    ftable_ll1 << "|" << setw(9) << "|";
    for(int i = 0 ; i < letter.size() ; i++)
        ftable_ll1 << setw(5) << letter[i] << setw(5) << "|";
    ftable_ll1 << endl;
    for(int i = 0 ; i <=(letter.size()+1)*10 ; i++)
        ftable_ll1 << "-";
    ftable_ll1 << endl;
    for(int i = 0 ; i < VN_set.size() ; i++)
    {
        ftable_ll1 << "|" << setw(5) << VN_set[i].left.c_str() << setw(4) << "|";
        for(int j = 0 ; j < letter.size() ; j ++)
            if(predict_table[i].count(letter[j]))
                ftable_ll1 << setw(7) << predict_table[i][letter[j]].c_str() << setw(3) << "|";
            else
                ftable_ll1 << setw(10) << "|";
        ftable_ll1 << endl;
        for(int i = 0 ; i <=(letter.size()+1)*10 ; i++)
            ftable_ll1 << "-";
        ftable_ll1 << endl;
    }
}

void print_ll1(int steps , stack<string> stk , string src , string wf , int x)
{
    fout_ll1 << setw(10) << steps;
    string out = "";
    while(!stk.empty())
    {
        out = stk.top()+out;
        stk.pop();
    }
    fout_ll1 << "#" << setw(9) << out.c_str();
    out ="";
    for(int i = x ; i < src.length() ; i++)
        out += src[i];
    fout_ll1 << setw(10) <<(out+"#").c_str();
    fout_ll1 << setw(12) << wf.c_str() << endl;
}

void analyse_ll1(const string& src)
{
    fout_ll1 << "【过程分析】" << endl;
    stack<string> stk;
    stk.push("E");
    int steps = 1;
    int idx = 0;
    fout_ll1 << setw(10) <<  "steps" << setw(10) << "op-stack" << setw(10) << "input" << setw(12) << "production" << endl;
    while(!stk.empty())
    {
        if (steps > 100) {
            throw "too much steps!";
        }
        string u = stk.top();
        string tmp="";
        stk.pop();
        if(!isupper(u[0]))
        {
            if(idx == src.length() && u[0] == '~');
            else if(src[idx] == u[0])
                idx++;
        }
        else
        {
            int x = VN_dic[u]-1;
//            cout << "x, predict_table.size() " << x << " " << predict_table.size() << endl;
//            cout << "idx, src.size() " << idx << " " << src.size() << endl;
//            cout << "src[idx], predict_table[x].count(src[idx]) " << src[idx] << " " << predict_table[x].count(src[idx]) << endl;
            if (x >= predict_table.size() || x < 0)
                throw "analyse failed!";
            tmp = predict_table[x][src[idx]];
            for(int i = tmp.length()-1 ; i >= 0 ; i--)
            {
                if(tmp[i] == '\'')
                {
                    string v = tmp.substr(i-1,2);
                    stk.push(v);
                    i--;
                }
                else
                {
                    string v = tmp.substr(i,1);
                    stk.push(v);
                }
            }
            tmp = u+"->"+tmp;
        }
        print_ll1(steps++ , stk , src , tmp  , idx);
    }
//    cout << "stk size: " << stk.size() << endl;
    if (steps <= 1)
        throw "analyse failed!";
}

void clear_ll1() {
    first.clear();
    follow.clear();
    VN_dic.clear();
    VN_set.clear();
    memset(used_ll1, false, MAX * sizeof(used_ll1[0]));
    predict_table.clear();
}

int run_ll1(string fin_ll1_path, string fout_ll1_path)
{
    fin_ll1.open(fin_ll1_path);
    fout_ll1.open(fout_ll1_path);
    ftable_ll1.open("log/temp_table_ll1");

    int n;
    int result_indicator = 0;
    char s[MAX];
    try {
        fin_ll1 >> n;
        fout_ll1 << "【启动 LL(1)分析】" << endl;
        fout_ll1 << "【输入文法】" << endl;
        for(int i = 0 ; i < n ; i++)
        {
            fin_ll1 >> s;
            fout_ll1 << s << endl;
            int len = strlen(s),j;
            for(j = 0 ; j < len ; j++)
                if(s[j] == '-') break;
            s[j] = 0;
            if(!VN_dic[s])
            {
                VN_set.push_back(s);
                VN_dic[s] = VN_set.size();
            }
            int x = VN_dic[s]-1;
            VN_set[x].insert(s+j+2);
        }
        make_first_ll1();
        make_follow_ll1();
        make_table_ll1();
        string in = "i*i+i";
        analyse_ll1(in);
    } catch (exception) {
        result_indicator = 1;
    } catch (const char* msg) {
        cout << msg << endl;
        result_indicator = 1;
    }

    fin_ll1.close();
    fout_ll1.close();
    ftable_ll1.close();
    clear_ll1();

    return result_indicator;
}

int get_first_set_ll1(string fin_ll1_path, string fout_ll1_path)
{
    fin_ll1.open(fin_ll1_path);
    fout_ll1.open(fout_ll1_path);
    ftable_ll1.open("log/temp_table_ll1");

    int n;
    int result_indicator = 0;
    char s[MAX];
    try {
        fin_ll1 >> n;
        for(int i = 0 ; i < n ; i++)
        {
            fin_ll1 >> s;
            int len = strlen(s),j;
            for(j = 0 ; j < len ; j++)
                if(s[j] == '-') break;
            s[j] = 0;
            if(!VN_dic[s])
            {
                VN_set.push_back(s);
                VN_dic[s] = VN_set.size();
            }
            int x = VN_dic[s]-1;
            VN_set[x].insert(s+j+2);
        }
        make_first_ll1();
    } catch (exception) {
        result_indicator = 1;
    } catch (const char* msg) {
        cout << msg << endl;
        result_indicator = 1;
    }

    fin_ll1.close();
    fout_ll1.close();
    ftable_ll1.close();
    clear_ll1();

    return result_indicator;

}

int get_follow_set_ll1(string fin_ll1_path, string fout_ll1_path)
{
    fin_ll1.open(fin_ll1_path);
    fout_ll1.open(fout_ll1_path);
    ftable_ll1.open("log/temp_table_ll1");

    int n;
    int result_indicator = 0;
    char s[MAX];
    try {
        fin_ll1 >> n;
        for(int i = 0 ; i < n ; i++)
        {
            fin_ll1 >> s;
            int len = strlen(s),j;
            for(j = 0 ; j < len ; j++)
                if(s[j] == '-') break;
            s[j] = 0;
            if(!VN_dic[s])
            {
                VN_set.push_back(s);
                VN_dic[s] = VN_set.size();
            }
            int x = VN_dic[s]-1;
            VN_set[x].insert(s+j+2);
        }
        for(int i = 0 ; i < VN_set.size() ; i++)
            dfs_ll1(i);
//        make_first_ll1();
        make_follow_ll1();
    } catch (exception) {
        result_indicator = 1;
    } catch (const char* msg) {
        cout << msg << endl;
        result_indicator = 1;
    }

    fin_ll1.close();
    fout_ll1.close();
    ftable_ll1.close();
    clear_ll1();

    return result_indicator;

}
