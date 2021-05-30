 #include <SLR1.h>

ifstream fin_slr1;
ofstream fout_slr1;
ofstream ftable_slr1;

// 产生式类
class WF_slr1
{
public:
    string left,right;
    int back;
    int id;
    WF_slr1 (char s1[] , char s2[] , int x , int y)
    {
        left = s1;
        right = s2;
        back = x;
        id = y;
    }
    WF_slr1 (const string& s1 , const string& s2 , int x , int y)
    {
        left = s1;
        right = s2;
        back = x;
        id = y;
    }
    bool operator < (const WF_slr1& a) const
    {
        if (left == a.left)
            return right < a.right;
        return left < a.left;
    }
    bool operator == (const WF_slr1& a) const
    {
        return (left == a.left)&& (right == a.right);
    }
    void print_slr1 ()
    {
        fout_slr1 << left.c_str() << "->" << right.c_str() << endl;
    }
};

// 闭包类
class Closure_slr1
{
public:
    vector<WF_slr1> element;
    void print_slr1 (string str)
    {
        fout_slr1 << setw(15) << "" << setw(15) << str.c_str() << endl;
        for (int i = 0 ; i < element.size() ; i++)
            element[i].print_slr1();
    }
    bool operator == (const Closure_slr1& a) const
    {
        if (a.element.size() != element.size()) return false;
        for (int i = 0 ; i < a.element.size() ; i++)
            if (element[i] == a.element[i]) continue;
            else return false;
        return true;
    }
};

struct Content_slr1
{
    int type;
    int num;
    string out;
    Content_slr1(){ type = -1; }
    Content_slr1 (int a , int b)
        :type(a),num(b){}
};

// 变量定义
vector<WF_slr1> wf_slr1;
map<string,vector<int> > dic_slr1;
map<string,vector<int> > VN_set_slr1;
map<string,bool>  vis_slr1;
string start_slr1 = "S";
vector<Closure_slr1> collection_slr1;
vector<WF_slr1> items_slr1;
char CH_slr1 = '$';
int go_slr1[MAX][MAX];
int to_slr1[MAX];
vector<char> V_slr1;
bool used_slr1[MAX];
Content_slr1 action_slr1[MAX][MAX];
int Goto_slr1[MAX][MAX];
map<string,set<char> > first_slr1;
map<string,set<char> > follow_slr1;

// 函数定义
void make_item_slr1 ()
{
    memset (to_slr1 , -1 , sizeof (-1));
    for (int i = 0 ; i < wf_slr1.size() ; i++)
        VN_set_slr1[wf_slr1[i].left].push_back (i);
    for (int i = 0 ; i < wf_slr1.size() ; i++)
        for (int j = 0 ; j <= wf_slr1[i].right.length() ; j++)
        {
            string temp = wf_slr1[i].right;
            temp.insert (temp.begin()+j , CH_slr1);
            dic_slr1[wf_slr1[i].left].push_back (items_slr1.size());
            if (j)
                to_slr1[items_slr1.size()-1] = items_slr1.size();
            items_slr1.push_back (WF_slr1 (wf_slr1[i].left , temp , i , items_slr1.size()));
        }
}

void print_item_slr1()
{
    fout_slr1 << "【项目表】" << endl;
    for (int i = 0 ; i < items_slr1.size() ; i++)
        fout_slr1 << items_slr1[i].left.c_str() << "->" << items_slr1[i].right.c_str() << " back:" << items_slr1[i].back << "id:" << items_slr1[i].id << endl;
}

void dfs_slr1 (const string& x)
{
    if (vis_slr1[x]) return;
    vis_slr1[x] = 1;
    vector<int>& id = VN_set_slr1[x];
    for (int i = 0 ; i < id.size() ; i++)
    {
        string& left = wf_slr1[id[i]].left;
        string& right = wf_slr1[id[i]].right;
        for (int j = 0 ; j < right.length() ; j++)
            if (isupper(right[j]))
            {
                dfs_slr1 (right.substr(j,1));
                set<char>& temp = first_slr1[right.substr(j,1)];
                set<char>::iterator it = temp.begin();
                bool flag = true;
                for (; it != temp.end() ; it++)
                {
                    if (*it == '~') flag = false;
                    first_slr1[left].insert (*it);
                }
                if (flag) break;
            }
            else
            {
                first_slr1[left].insert (right[j]);
                break;
            }
    }
}

void make_first_slr1 ()
{
    vis_slr1.clear();
    map<string,vector<int> >::iterator it2 = dic_slr1.begin();
    for (; it2 != dic_slr1.end() ; it2++)
        if (vis_slr1[it2->first]) continue;
        else dfs_slr1 (it2->first);
    fout_slr1 << "【FIRST集】" << endl;
    map<string,set<char> >::iterator it = first_slr1.begin();
    for (; it != first_slr1.end() ; it++)
    {
        fout_slr1 << "FIRST(" << it->first.c_str() << ")={";
        set<char> & temp = it->second;
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for (; it1 != temp.end() ; it1++)
        {
            if (flag)
                fout_slr1 << ",";
            fout_slr1 << *it1;
            flag = true;
        }
        fout_slr1 << "}" << endl;
    }
}

void append_slr1 (const string& str1 , const string& str2)
{
    set<char>& from = follow_slr1[str1];
    set<char>& to_slr1 = follow_slr1[str2];
    set<char>::iterator it = from.begin();
    for (; it != from.end() ; it++)
        to_slr1.insert (*it);
}

bool _check (const vector<int>& id, const string str)
{
    for (int i = 0 ; i < id.size() ; i++)
    {
        int x = id[i];
        if (wf_slr1[x].right == str) return true;
    }
    return false;
}

void make_follow_slr1 ()
{
    while (true)
    {
        bool goon = false;
        map<string,vector<int> >::iterator it2 = VN_set_slr1.begin();
        for (; it2 != VN_set_slr1.end() ; it2++)
        {
            vector<int>& id = it2->second;
            for (int i = 0 ; i < id.size() ; i++)
            {
                bool flag = true;
                WF_slr1& tt = wf_slr1[id[i]];
                string& left = tt.left;
                const string& right = tt.right;
                for (int j = right.length()-1 ; j >= 0 ; j--)
                    if (isupper(right[j]))
                    {
                        if (flag)
                        {
                            int tx = follow_slr1[right.substr(j,1)].size();
                            append_slr1(left , right.substr(j,1));
                            int tx1 = follow_slr1[right.substr(j,1)].size();
                            if (tx1 > tx) goon = true;
                            if (_check (id , "~"))
                                flag = false;
                        }
                        for (int k = j+1 ; k < right.length() ; k++)
                            if (isupper(right[k]))
                            {
                                string idd = right.substr(k,1);
                                set<char>& from = first_slr1[idd];
                                set<char>& to_slr1 = follow_slr1[right.substr(j,1)];
                                set<char>::iterator it1 = from.begin();
                                int tx = follow_slr1[right.substr(j,1)].size();
                                for (; it1 != from.end() ; it1++)
                                    if (*it1 != '~')
                                        to_slr1.insert (*it1);
                                int tx1 = follow_slr1[right.substr(j,1)].size();
                                if (tx1 > tx) goon = true;
                                if (_check (id , "~"))
                                    break;
                            }
                            else
                            {
                                int tx = follow_slr1[right.substr(j,1)].size();
                                follow_slr1[right.substr(j,1)].insert (right[k]);
                                int tx1 = follow_slr1[right.substr(j,1)].size();
                                if (tx1 > tx) goon = true;
                                break;
                            }
                    }
                    else flag = false;
            }
        }
        if (!goon) break;
    }
    fout_slr1 << "【FOLLOW集】" << endl;
    map<string,set<char> >::iterator it = follow_slr1.begin();
    for (; it != follow_slr1.end() ; it++)
    {
        fout_slr1 << "FOLLOW(" << it->first.c_str() << ")={";
        set<char> & temp = it->second;
        //if (it->first[0] == 'S')
        temp.insert ('#');
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for (; it1 != temp.end() ; it1++)
        {
            if (flag)
                fout_slr1 << ",";
            fout_slr1 << *it1;
            flag = true;
        }
        fout_slr1 << "}" << endl;
    }
}

void make_set_slr1 ()
{
    bool has[MAX];
    for (int i = 0 ; i < items_slr1.size() ; i++)
        if (items_slr1[i].left[0] == 'S' && items_slr1[i].right[0] == CH_slr1)
        {
            Closure_slr1 temp;
            string& str = items_slr1[i].right;
            vector<WF_slr1>& element = temp.element;
            element.push_back (items_slr1[i]);
            int x = 0;
            for (x = 0 ; x < str.length() ; x++)
                if (str[x] == CH_slr1)
                    break;
            /*if (x != str.length()-1)
            {
                string tt = str.substr(x+1,1);
                vector<int>& id = dic_slr1[tt];
                for (int j = 0 ; j < id.size() ; j++)
                {
                    int tx = id[j];
                    //items_slr1[tx].print_slr1();
                    if (items_slr1[tx].right[0] == CH_slr1)
                        element.push_back (items_slr1[tx]);
                }
            }*/
            memset (has , 0 , sizeof (has));
            has[i] = 1;
            if (x != str.length()-1)
            {
                queue<string> q;
                q.push(str.substr(x+1,1));
                while (!q.empty())
                {
                    string u = q.front();
                    q.pop();
                    vector<int>& id = dic_slr1[u];
                    for(int j = 0 ; j < id.size() ; j++)
                    {
                        int tx = id[j];
                        if (items_slr1[tx].right[0] == CH_slr1)
                        {
                            if (has[tx]) continue;
                            has[tx] = 1;
                            if (isupper(items_slr1[tx].right[1]))
                                q.push (items_slr1[tx].right.substr(1,1));
                            element.push_back (items_slr1[tx]);
                        }
                    }
                }
            }
            collection_slr1.push_back (temp);
        }
    for (int i = 0 ; i < collection_slr1.size() ; i++)
    {
        map<int,Closure_slr1> temp;
        for (int j = 0 ; j < collection_slr1[i].element.size() ; j++)
        {
            string str = collection_slr1[i].element[j].right;
            int x = 0;
            for (; x < str.length() ; x++)
                if (str[x] == CH_slr1) break;
            if (x == str.length()-1)
                continue;
            int y = str[x+1];
            int ii;
            str.erase (str.begin()+x);
            str.insert (str.begin()+x+1 , CH_slr1);
            WF_slr1 cmp = WF_slr1 (collection_slr1[i].element[j].left , str , -1 , -1);
            for (int k = 0 ; k< items_slr1.size() ; k++)
                if (items_slr1[k] == cmp)
                {
                    ii = k;
                    break;
                }
            //string& str1 = items_slr1[ii].right;
            memset (has , 0 , sizeof (has));
            vector<WF_slr1>& element = temp[y].element;
            element.push_back (items_slr1[ii]);
            has[ii] = 1;
            x++;
            /*if (x != str.length()-1)
             {
                 string tt = str.substr(x+1,1);
                 vector<int>& id = dic_slr1[tt];
                 for (int j = 0 ; j < id.size() ; j++)
                 {
                    int tx = id[j];
                    //items_slr1[tx].print_slr1();
                    if (items_slr1[tx].right[0] == CH_slr1)
                        element.push_back (items_slr1[tx]);
                 }
             }*/
            if (x != str.length()-1)
            {
                queue<string> q;
                q.push(str.substr(x+1,1));
                while (!q.empty())
                {
                    string u = q.front();
                    q.pop();
                    vector<int>& id = dic_slr1[u];
                    for(int j = 0 ; j < id.size() ; j++)
                    {
                        int tx = id[j];
                        if (items_slr1[tx].right[0] == CH_slr1)
                        {
                            if (has[tx]) continue;
                            has[tx] = 1;
                            if (isupper(items_slr1[tx].right[1]))
                                q.push (items_slr1[tx].right.substr(1,1));
                            element.push_back (items_slr1[tx]);
                        }
                    }
                }
            }
        }
        map<int,Closure_slr1>::iterator it = temp.begin();
        for (; it != temp.end() ; it++)
            collection_slr1.push_back (it->second);
        for (int i = 0 ; i < collection_slr1.size() ; i++)
            sort (collection_slr1[i].element.begin() , collection_slr1[i].element.end());
        for (int i = 0 ; i < collection_slr1.size() ; i++)
            for (int j = i+1 ; j < collection_slr1.size() ; j++)
                if (collection_slr1[i] == collection_slr1[j])
                    collection_slr1.erase (collection_slr1.begin()+j);
    }
    fout_slr1 << "【CLOSURE】" << endl;
    stringstream sin;
    for (int i = 0 ; i < collection_slr1.size() ; i++)
    {
        sin.clear();
        string out;
        sin <<"closure-I" << i;
        sin >> out;
        collection_slr1[i].print_slr1 (out);
    }
    fout_slr1 << endl;
}

void make_V_slr1 ()
{
    memset (used_slr1 , 0 , sizeof (used_slr1));
    for (int i = 0 ; i < wf_slr1.size() ; i++)
    {
        string& str = wf_slr1[i].left;
        for (int j = 0 ; j < str.length() ; j++)
        {
            if (used_slr1[str[j]]) continue;
            used_slr1[str[j]] = 1;
            V_slr1.push_back (str[j]);
        }
        string& str1 = wf_slr1[i].right;
        for (int j = 0 ; j < str1.length() ; j++)
        {
            if (used_slr1[str1[j]]) continue;
            used_slr1[str1[j]] = 1;
            V_slr1.push_back (str1[j]);
        }
    }
    sort (V_slr1.begin() , V_slr1.end());
    V_slr1.push_back ('#');
}

void make_cmp_slr1 (vector<WF_slr1>& cmp1 , int i  , char ch)
{
    for (int j = 0 ; j < collection_slr1[i].element.size() ; j++)
    {
        string str = collection_slr1[i].element[j].right;
        int k;
        for (k = 0 ; k < str.length() ; k++)
            if (str[k] == CH_slr1)
                break;
        if (k != str.length() - 1 && str[k+1] == ch )
        {
            str.erase (str.begin()+k);
            str.insert (str.begin()+k+1 , CH_slr1);
            cmp1.push_back (WF_slr1 (collection_slr1[i].element[j].left , str , -1 , -1));
        }
    }
    sort (cmp1.begin() , cmp1.end());
}

void make_go_slr1 ()
{
    memset (go_slr1 , -1 , sizeof (go_slr1));
    int m = collection_slr1.size();
    for (int t = 0 ; t < V_slr1.size() ; t++)
    {
        char ch = V_slr1[t];
        for (int i = 0 ; i < m ; i++)
        {
            vector<WF_slr1> cmp1;
            make_cmp_slr1 (cmp1 , i , ch);
            if (cmp1.size() == 0) continue;
            for (int j = 0 ; j < m ; j++)
            {
                vector<WF_slr1> cmp2;
                for (int k = 0 ; k < collection_slr1[j].element.size() ; k++)
                {
                    string& str = collection_slr1[j].element[k].right;
                    int x;
                    for (x = 0 ; x < str.length() ; x++)
                        if (str[x] == CH_slr1)
                            break;
                    if (x && str[x-1] == ch)
                        cmp2.push_back (WF_slr1(collection_slr1[j].element[k].left , str , -1 , -1));
                }
                sort (cmp2.begin() , cmp2.end());
                bool flag = true;
                if (cmp2.size() != cmp1.size()) continue;
                for (int k = 0 ; k < cmp1.size() ; k++)
                    if (cmp1[k] == cmp2[k]) continue;
                    else flag = false;
                if (flag)
                    go_slr1[i][ch] = j;
            }
        }

    }
    fout_slr1 << "【EDGE】" << endl;
    stringstream sin;
    string out;
    for (int i = 0 ; i < m ; i++)
        for (int j = 0 ; j < m ; j++)
            for (int k = 0 ; k < MAX ; k++)
                if (go_slr1[i][k] == j)
                {
                    sin.clear();
                    sin << "I" << i << "--" <<(char)(k)<<"--I"<<j;
                    sin >> out;
                    fout_slr1 << out.c_str() << endl;
                }
}

void make_table_slr1 ()
{
    memset (Goto_slr1 , -1 , sizeof (Goto_slr1));
    //write s to_slr1 the table
    for(int i = 0 ; i < collection_slr1.size() ; i++)
        for (int j = 0 ; j < V_slr1.size() ; j++)
        {
            char ch = V_slr1[j];
            int x = go_slr1[i][ch];
            if (x == -1) continue;
            if (!isupper(ch))
                action_slr1[i][ch] = Content_slr1 (0 , x);
            else
                Goto_slr1[i][ch] = x;
        }
    //write r and acc to_slr1 the table
    for (int i = 0 ; i < collection_slr1.size() ; i++)
        for (int j = 0 ; j < collection_slr1[i].element.size() ; j++)
        {
            WF_slr1& tt = collection_slr1[i].element[j];
            if (tt.right[tt.right.length()-1] == CH_slr1)
            {
                if (tt.left[0] == 'S')
                    action_slr1[i]['#'] = Content_slr1 (2 , -1);
                else
                    for (int k = 0 ; k < V_slr1.size() ; k++)
                    {
                        int y = V_slr1[k];
                        if (!follow_slr1[tt.left].count(V_slr1[k])) continue;
                        action_slr1[i][y] = Content_slr1 (1, tt.back);
                    }
            }
        }
    // save to complete log
    fout_slr1 << "【SLR(1)分析表】" << endl;
    fout_slr1 << setw(10) << "|" << setw(5) << V_slr1[0] << setw(5) << "|";
    for (int i = 1 ; i < V_slr1.size() ; i++)
        fout_slr1 << setw(5) << V_slr1[i] << setw(5) << "|";
    fout_slr1 << endl;
    for (int i = 0 ; i < (V_slr1.size()+1)*10 ; i++)
        fout_slr1 << "-";
    fout_slr1 << endl;
    stringstream sin;
    for (int i = 0 ; i < collection_slr1.size() ; i++)
    {
        fout_slr1 << setw(5) << i << setw(5) << "|";
        for (int j = 0 ; j < V_slr1.size() ; j++)
        {
            char ch = V_slr1[j];
            if (isupper(ch))
            {
                if (Goto_slr1[i][ch] == -1)
                    fout_slr1 << setw(10) << "|";
                else
                    fout_slr1 << setw(5) << Goto_slr1[i][ch] << setw(5) << "|";
            }
            else
            {
                sin.clear();
                if (action_slr1[i][ch].type == -1)
                    fout_slr1 << setw(10) << "|";
                else
                {
                    Content_slr1& temp = action_slr1[i][ch];
                    if (temp.type == 0)
                        sin << "S";
                    if (temp.type == 1)
                        sin << "R";
                    if (temp.type == 2)
                        sin << "acc";
                    if (temp.num != -1)
                        sin << temp.num;
                    sin >> temp.out;
                    fout_slr1 << setw(7) << temp.out.c_str() << setw(3) << "|";
                }
            }
        }
        fout_slr1 << endl;
    }
    for (int i = 0 ; i < (V_slr1.size()+1)*10 ; i++)
        fout_slr1 << "-";
    fout_slr1 << endl;

    // save to table log
    ftable_slr1 << "判定为 SLR(1) 文法" << endl << endl;
    ftable_slr1 << "【LR(0)分析表】" << endl;
    ftable_slr1 << setw(10) << "|" << setw(5) << V_slr1[0] << setw(5) << "|";
    for (int i = 1 ; i < V_slr1.size() ; i++)
        ftable_slr1 << setw(5) << V_slr1[i] << setw(5) << "|";
    ftable_slr1 << endl;
    for (int i = 0 ; i < (V_slr1.size()+1)*10 ; i++)
        ftable_slr1 << "-";
    ftable_slr1 << endl;
    stringstream sin2;
    for (int i = 0 ; i < collection_slr1.size() ; i++)
    {
        ftable_slr1 << setw(5) << i << setw(5) << "|";
        for (int j = 0 ; j < V_slr1.size() ; j++)
        {
            char ch = V_slr1[j];
            if (isupper(ch))
            {
                if (Goto_slr1[i][ch] == -1)
                    ftable_slr1 << setw(10) << "|";
                else
                    ftable_slr1 << setw(5) << Goto_slr1[i][ch] << setw(5) << "|";
            }
            else
            {
                sin2.clear();
                if (action_slr1[i][ch].type == -1)
                    ftable_slr1 << setw(10) << "|";
                else
                {
                    Content_slr1& temp = action_slr1[i][ch];
                    if (temp.type == 0)
                        sin2 << "S";
                    if (temp.type == 1)
                        sin2 << "R";
                    if (temp.type == 2)
                        sin2 << "acc";
                    if (temp.num != -1)
                        sin2 << temp.num;
                    sin2 >> temp.out;
                    ftable_slr1 << setw(7) << temp.out.c_str() << setw(3) << "|";
                }
            }
        }
        ftable_slr1 << endl;
    }
    for (int i = 0 ; i < (V_slr1.size()+1)*10 ; i++)
        ftable_slr1 << "-";
    ftable_slr1 << endl;
}

void print_slr1 (string s1 , string s2 , string s3 , string s4 , string s5 , string s6 , string s7)
{
    fout_slr1 << setw(15) << s1.c_str() << "|"
              << setw(15) << s2.c_str() << setw(15) << s3.c_str() << setw(20) << s4.c_str() << "|"
              << setw(15) << s5.c_str() << setw(15) << s6.c_str() << setw(15) << s7.c_str() << endl;
}

string get_steps_slr1 (int x)
{
    stringstream sin;
    sin << x;
    string ret;
    sin >> ret;
    return ret;
}

template<class T>
string get_stk_slr1 (vector<T> stk)
{
    stringstream sin;
    for (int i = 0 ; i < stk.size() ; i++)
        sin << stk[i];
    string ret;
    sin >> ret;
    return ret;
}

string get_shift_slr1 (WF_slr1& temp)
{
    stringstream sin;
    sin << "reduce(" << temp.left << "->" << temp.right <<")";
    string out;
    sin >> out;
    return out;
}

void analyse_slr1 (string src)
{
    fout_slr1 << "【过程分析】" << endl;
    print_slr1 ("steps","op-stack" ,"input","operation","state-stack" , "ACTION" , "GOTO");
    vector<char> op_stack;
    vector<int> st_stack;
    src+= "#";
    op_stack.push_back ('#');
    st_stack.push_back (0);
    int steps = 1;
    bool is_accepted = false;
    for (int i = 0 ; i < src.length() ; i++)
    {
        char u = src[i];
        int top = st_stack[st_stack.size()-1];
        Content_slr1& act = action_slr1[top][u];
        if (act.type == 0)
        {
            print_slr1 (get_steps_slr1 (steps++) , get_stk_slr1 (op_stack) , src.substr(i), "shift",  get_stk_slr1(st_stack) , act.out , "");
            op_stack.push_back (u);
            st_stack.push_back (act.num);
        }
        else if (act.type == 1)
        {
            WF_slr1& tt = wf_slr1[act.num];
            int y = st_stack[st_stack.size()-tt.right.length()-1];
            int x = Goto_slr1[y][tt.left[0]];
            print_slr1 (get_steps_slr1 (steps++) , get_stk_slr1 (op_stack) , src.substr(i) , get_shift_slr1(tt) ,get_stk_slr1(st_stack),act.out,get_steps_slr1(x));
            for (int j = 0 ; j < tt.right.length() ; j++)
            {
                st_stack.pop_back();
                op_stack.pop_back();
            }
            op_stack.push_back (tt.left[0]);
            st_stack.push_back (x);
            i--;
        }
        else if (act.type == 2)
        {
            print_slr1 (get_steps_slr1(steps++), get_stk_slr1(op_stack) , src.substr(i) , "Accept" , get_stk_slr1(st_stack) , act.out , "");
            is_accepted = true;
        }
        else continue;
    }
//    cout << "op_stack depth: " << op_stack.size() << endl;
//    cout << "st_stack depth: " << st_stack.size() << endl;
    if (!is_accepted)
        throw "analyse failed!";
    if (steps == 1)
        throw "analyse failed!";
}

void clear_slr1 ()
{
    wf_slr1.clear();
    dic_slr1.clear();
    VN_set_slr1.clear();
    vis_slr1.clear();
    start_slr1 = "S";
    collection_slr1.clear();
    items_slr1.clear();
    CH_slr1 = '$';
    V_slr1.clear();
    first_slr1.clear();
    follow_slr1.clear();
    for (int i = 0; i < MAX; i++) {
        memset(go_slr1[i], 0, MAX * sizeof(go_slr1[0][0]));
        memset(Goto_slr1[i], 0, MAX * sizeof(Goto_slr1[0][0]));
        for (int j = 0; j < MAX; j++)   action_slr1[i][j] = Content_slr1();
//        memset(action_slr1, 0, MAX * sizeof(action_slr1[0][0]));
    }
    memset(to_slr1, 0, sizeof(to_slr1[0]));
    memset(used_slr1, 0, sizeof(used_slr1[0]));
}

int run_slr1 (string fin_slr1_path, string fout_slr1_path, string input)
{
    fin_slr1.open(fin_slr1_path);
    fout_slr1.open(fout_slr1_path);
    ftable_slr1.open("log/temp_table_slr1");

    int n;
    int result_indicator = 0;
    char s[MAX];
    try {
        fin_slr1 >> n;
        fout_slr1 << "【启动 SLR(1)分析】" << endl;
        fout_slr1 << "【输入文法】" << endl;
        for (int i = 0 ; i < n ; i++)
        {
            fin_slr1 >> s;
            int len = strlen(s),j;
            for (j = 0 ; j < len ; j++)
                if (s[j] == '-') break;
            s[j] = 0;
            wf_slr1.push_back (WF_slr1 (s , s+j+2 ,-1 , -1));
            wf_slr1[wf_slr1.size()-1].print_slr1();
        }
        make_item_slr1();
        print_item_slr1();
        make_first_slr1();
        make_follow_slr1();
        make_set_slr1();
        make_V_slr1();
        make_go_slr1();
        make_table_slr1();
        if(input == string("default"))analyse_slr1 ("(i*i)+i");
        else analyse_slr1(input);
    } catch (exception) {
        result_indicator = 1;
    } catch (const char* msg) {
        cout << msg << endl;
        result_indicator = 1;
    }

    fin_slr1.close();
    fout_slr1.close();
    ftable_slr1.close();
    clear_slr1();

    return result_indicator;
}

int get_first_set_slr1 (string fin_slr1_path, string fout_slr1_path)
{
    fin_slr1.open(fin_slr1_path);
    fout_slr1.open(fout_slr1_path);
    ftable_slr1.open("log/temp_table_slr1");

    int n;
    int result_indicator = 0;
    char s[MAX];
    try {
        fin_slr1 >> n;
        for (int i = 0 ; i < n ; i++)
        {
            fin_slr1 >> s;
            int len = strlen(s),j;
            for (j = 0 ; j < len ; j++)
                if (s[j] == '-') break;
            s[j] = 0;
            wf_slr1.push_back (WF_slr1 (s , s+j+2 ,-1 , -1));
        }
        make_item_slr1();
        make_first_slr1();
    } catch (exception) {
        result_indicator = 1;
    } catch (const char* msg) {
        cout << msg << endl;
        result_indicator = 1;
    }

    fin_slr1.close();
    fout_slr1.close();
    ftable_slr1.close();
    clear_slr1();

    return result_indicator;
}

int get_follow_set_slr1 (string fin_slr1_path, string fout_slr1_path)
{
    fin_slr1.open(fin_slr1_path);
    fout_slr1.open(fout_slr1_path);
    ftable_slr1.open("log/temp_table_slr1");

    int n;
    int result_indicator = 0;
    char s[MAX];
    try {
        fin_slr1 >> n;
        for (int i = 0 ; i < n ; i++)
        {
            fin_slr1 >> s;
            int len = strlen(s),j;
            for (j = 0 ; j < len ; j++)
                if (s[j] == '-') break;
            s[j] = 0;
            wf_slr1.push_back (WF_slr1 (s , s+j+2 ,-1 , -1));
        }
        make_item_slr1();
        make_follow_slr1();
    } catch (exception) {
        result_indicator = 1;
    } catch (const char* msg) {
        cout << msg << endl;
        result_indicator = 1;
    }

    fin_slr1.close();
    fout_slr1.close();
    ftable_slr1.close();
    clear_slr1();

    return result_indicator;
}
