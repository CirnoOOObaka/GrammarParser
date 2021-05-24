#include <LR0.h>

ifstream fin_lr0;
ofstream fout_lr0;
ofstream ftable_lr0;

// 产生式类
class WF_lr0
{
public:
    string left,right;
    int back;
    int id;
    WF_lr0(char s1[] , char s2[] , int x , int y)
    {
        left = s1;
        right = s2;
        back = x;
        id = y;
    }
    WF_lr0(const string& s1 , const string& s2 , int x , int y)
    {
        left = s1;
        right = s2;
        back = x;
        id = y;
    }
    bool operator < (const WF_lr0& a) const
    {
        if(left == a.left)
            return right < a.right;
        return left < a.left;
    }
    bool operator == (const WF_lr0& a) const
    {
        return(left == a.left)&&(right == a.right);
    }
    void print_lr0()
    {
        fout_lr0 << left.c_str() << "->" << right.c_str() << endl;
    }
};

// 闭包类
class Closure_lr0
{
public:
    vector<WF_lr0> element;
    void print_lr0(string str)
    {
        fout_lr0 << setw(15) << "" << setw(15) << str.c_str() << endl;
        for(int i = 0 ; i < element.size() ; i++)
            element[i].print_lr0();
    }
    bool operator ==(const Closure_lr0& a) const
    {
        if(a.element.size() != element.size()) return false;
        for(int i = 0 ; i < a.element.size() ; i++)
            if(element[i] == a.element[i]) continue;
            else return false;
        return true;
    }
};

struct Content_lr0
{
    int type;
    int num;
    string out;
    Content_lr0(){ type = -1; }
    Content_lr0(int a , int b)
        :type(a),num(b){}
};

// 变量定义
vector<WF_lr0> wf_lr0;
map<string,vector<int> > dic_lr0;
string start_lr0 = "S";
vector<Closure_lr0> collection_lr0;
vector<WF_lr0> items_lr0;
char CH_lr0 = '$';
int go_lr0[MAX][MAX];
int to_lr0[MAX];
vector<char> V_lr0;
bool used_lr0[MAX];
Content_lr0 action_lr0[MAX][MAX];
int Goto_lr0[MAX][MAX];

// 函数定义
void make_item_lr0()
{
    memset(to_lr0 , -1 , sizeof(-1));
    for(int i = 0 ; i < wf_lr0.size() ; i++)
        for(int j = 0 ; j <= wf_lr0[i].right.length() ; j++)
        {
            string temp = wf_lr0[i].right;
            temp.insert(temp.begin()+j , CH_lr0);
            dic_lr0[wf_lr0[i].left].push_back(items_lr0.size());
            if(j)
                to_lr0[items_lr0.size()-1] = items_lr0.size();
            items_lr0.push_back(WF_lr0(wf_lr0[i].left , temp , i , items_lr0.size()));
        }
    fout_lr0 << "【项目表】" << endl;
    for(int i = 0 ; i < items_lr0.size() ; i++)
        fout_lr0 << items_lr0[i].left.c_str() << "->" << items_lr0[i].right.c_str() << endl;
}

void make_set_lr0()
{
    bool has[MAX];
    for(int i = 0 ; i < items_lr0.size() ; i++)
        if(items_lr0[i].left[0] == 'S' && items_lr0[i].right[0] == CH_lr0)
        {
            Closure_lr0 temp;
            string& str = items_lr0[i].right;
            vector<WF_lr0>& element = temp.element;
            element.push_back(items_lr0[i]);
            int x = 0;
            for(x = 0 ; x < str.length() ; x++)
                if(str[x] == CH_lr0)
                    break;
            memset(has , 0 , sizeof(has));
            has[i] = 1;
            if(x != str.length()-1)
            {
                queue<string> q;
                q.push(str.substr(x+1,1));
                while(!q.empty())
                {
                    string u = q.front();
                    q.pop();
                    vector<int>& id = dic_lr0[u];
                    for(int j = 0 ; j < id.size() ; j++)
                    {
                        int tx = id[j];
                        if(items_lr0[tx].right[0] == CH_lr0)
                        {
                            if(has[tx]) continue;
                            has[tx] = 1;
                            if(isupper(items_lr0[tx].right[1]))
                                q.push(items_lr0[tx].right.substr(1,1));
                            element.push_back(items_lr0[tx]);
                        }
                    }
                }
            }
            collection_lr0.push_back(temp);
        }
    for(int i = 0 ; i < collection_lr0.size() ; i++)
    {
        map<int,Closure_lr0> temp;
        for(int j = 0 ; j < collection_lr0[i].element.size() ; j++)
        {
            string str = collection_lr0[i].element[j].right;
            int x = 0;
            for(; x < str.length() ; x++)
                if(str[x] == CH_lr0) break;
            if(x == str.length()-1)
                continue;
            int y = str[x+1];
            int ii;
            str.erase(str.begin()+x);
            str.insert(str.begin()+x+1 , CH_lr0);
            WF_lr0 cmp = WF_lr0(collection_lr0[i].element[j].left , str , -1 , -1);
            for(int k = 0 ; k< items_lr0.size() ; k++)
                if(items_lr0[k] == cmp)
                {
                    ii = k;
                    break;
                }
            memset(has , 0 , sizeof(has));
            vector<WF_lr0>& element = temp[y].element;
            element.push_back(items_lr0[ii]);
            has[ii] = 1;
            x++;
            if(x != str.length()-1)
            {
                queue<string> q;
                q.push(str.substr(x+1,1));
                while(!q.empty())
                {
                    string u = q.front();
                    q.pop();
                    vector<int>& id = dic_lr0[u];
                    for(int j = 0 ; j < id.size() ; j++)
                    {
                        int tx = id[j];
                        if(items_lr0[tx].right[0] == CH_lr0)
                        {
                            if(has[tx]) continue;
                            has[tx] = 1;
                            if(isupper(items_lr0[tx].right[1]))
                                q.push(items_lr0[tx].right.substr(1,1));
                            element.push_back(items_lr0[tx]);
                        }
                    }
                }
            }
        }
        map<int,Closure_lr0>::iterator it = temp.begin();
        for(; it != temp.end() ; it++)
            collection_lr0.push_back(it->second);
        for(int i = 0 ; i < collection_lr0.size() ; i++)
            sort(collection_lr0[i].element.begin() , collection_lr0[i].element.end());
        for(int i = 0 ; i < collection_lr0.size() ; i++)
            for(int j = i+1 ; j < collection_lr0.size() ; j++)
                if(collection_lr0[i] == collection_lr0[j])
                    collection_lr0.erase(collection_lr0.begin()+j);
    }
    fout_lr0 << "【CLOSURE】" << endl;
    stringstream sin;
    for(int i = 0 ; i < collection_lr0.size() ; i++)
    {
        sin.clear();
        string out;
        sin <<"closure-I" << i;
        sin >> out;
        collection_lr0[i].print_lr0(out);
    }
    fout_lr0 << endl;
}

void make_V_lr0()
{
    memset(used_lr0 , 0 , sizeof(used_lr0));
    for(int i = 0 ; i < wf_lr0.size() ; i++)
    {
        string& str = wf_lr0[i].left;
        for(int j = 0 ; j < str.length() ; j++)
        {
            if(used_lr0[str[j]]) continue;
            used_lr0[str[j]] = 1;
            V_lr0.push_back(str[j]);
        }
        string& str1 = wf_lr0[i].right;
        for(int j = 0 ; j < str1.length() ; j++)
        {
            if(used_lr0[str1[j]]) continue;
            used_lr0[str1[j]] = 1;
            V_lr0.push_back(str1[j]);
        }
    }
    sort(V_lr0.begin() , V_lr0.end());
    V_lr0.push_back('#');
}

void make_cmp_lr0(vector<WF_lr0>& cmp1 , int i  , char ch)
{
    for(int j = 0 ; j < collection_lr0[i].element.size() ; j++)
    {
        string str = collection_lr0[i].element[j].right;
        int k;
        for(k = 0 ; k < str.length() ; k++)
            if(str[k] == CH_lr0)
                break;
        if(k != str.length() - 1 && str[k+1] == ch )
        {
            str.erase(str.begin()+k);
            str.insert(str.begin()+k+1 , CH_lr0);
            cmp1.push_back(WF_lr0(collection_lr0[i].element[j].left , str , -1 , -1));
        }
    }
    sort(cmp1.begin() , cmp1.end());
}

void make_go_lr0()
{
    memset(go_lr0 , -1 , sizeof(go_lr0));
    int m = collection_lr0.size();
    for(int t = 0 ; t < V_lr0.size() ; t++)
    {
        char ch = V_lr0[t];
        for(int i = 0 ; i < m ; i++)
        {
            vector<WF_lr0> cmp1;
            make_cmp_lr0(cmp1 , i , ch);
            if(cmp1.size() == 0) continue;
            for(int j = 0 ; j < m ; j++)
            {
                vector<WF_lr0> cmp2;
                for(int k = 0 ; k < collection_lr0[j].element.size() ; k++)
                {
                    string& str = collection_lr0[j].element[k].right;
                    int x;
                    for(x = 0 ; x < str.length() ; x++)
                        if(str[x] == CH_lr0)
                            break;
                    if(x && str[x-1] == ch)
                        cmp2.push_back(WF_lr0(collection_lr0[j].element[k].left , str , -1 , -1));
                }
                sort(cmp2.begin() , cmp2.end());
                bool flag = true;
                if(cmp2.size() != cmp1.size()) continue;
                for(int k = 0 ; k < cmp1.size() ; k++)
                    if(cmp1[k] == cmp2[k]) continue;
                    else flag = false;
                if(flag)
                    go_lr0[i][ch] = j;
            }
        }

    }
    fout_lr0 << "【EDGE】" << endl;
    stringstream sin;
    string out;
    for(int i = 0 ; i < m ; i++)
        for(int j = 0 ; j < m ; j++)
            for(int k = 0 ; k < MAX ; k++)
                if(go_lr0[i][k] == j)
                {
                    sin.clear();
                    sin << "I" << i << "--" <<(char)(k)<<"--I"<<j;
                    sin >> out;
                    fout_lr0 << out.c_str() << endl;
                }
}

void make_table_lr0()
{
    memset(Goto_lr0 , -1 , sizeof(Goto_lr0));
    //write s to_lr0 the table
    for(int i = 0 ; i < collection_lr0.size() ; i++)
        for(int j = 0 ; j < V_lr0.size() ; j++)
        {
            char ch = V_lr0[j];
            int x = go_lr0[i][ch];
            if(x == -1) continue;
            if(!isupper(ch))
                action_lr0[i][ch] = Content_lr0(0 , x);
            else
                Goto_lr0[i][ch] = x;
        }
    //write r and acc to_lr0 the table
    for(int i = 0 ; i < collection_lr0.size() ; i++)
        for(int j = 0 ; j < collection_lr0[i].element.size() ; j++)
        {
            WF_lr0& tt = collection_lr0[i].element[j];
            if(tt.right[tt.right.length()-1] == CH_lr0)
            {
                if(tt.left[0] == 'S')
                    action_lr0[i]['#'] = Content_lr0(2 , -1);
                else
                    for(int k = 0 ; k < V_lr0.size() ; k++)
                    {
                        int y = V_lr0[k];
                        action_lr0[i][y] = Content_lr0(1, tt.back);
                    }
            }
        }
    // save to complete log
    fout_lr0 << "【LR(0)分析表】" << endl;
    fout_lr0 << setw(10) << "|" << setw(5) << V_lr0[0] << setw(5) << "|";
    for(int i = 1 ; i < V_lr0.size() ; i++)
        fout_lr0 << setw(5) << V_lr0[i] << setw(5) << "|";
    fout_lr0 << endl;
    for(int i = 0 ; i <(V_lr0.size()+1)*10 ; i++)
        fout_lr0 << "-";
    fout_lr0 << endl;
    stringstream sin;
    for(int i = 0 ; i < collection_lr0.size() ; i++)
    {
        fout_lr0 << setw(5) << i << setw(5) << "|";
        for(int j = 0 ; j < V_lr0.size() ; j++)
        {
            char ch = V_lr0[j];
            if(isupper(ch))
            {
                if(Goto_lr0[i][ch] == -1)
                    fout_lr0 << setw(10) << "|";
                else
                    fout_lr0 << setw(5) << Goto_lr0[i][ch] << setw(5) << "|";
            }
            else
            {
                sin.clear();
                if(action_lr0[i][ch].type == -1)
                    fout_lr0 << setw(10) << "|";
                else
                {
                    Content_lr0& temp = action_lr0[i][ch];
                    if(temp.type == 0)
                        sin << "S";
                    if(temp.type == 1)
                        sin << "R";
                    if(temp.type == 2)
                        sin << "acc";
                    if(temp.num != -1)
                        sin << temp.num;
                    sin >> temp.out;
                    fout_lr0 << setw(7) << temp.out.c_str() << setw(3) << "|";
                }
            }
        }
        fout_lr0 << endl;
    }
    for(int i = 0 ; i <(V_lr0.size()+1)*10 ; i++)
        fout_lr0 << "-";
    fout_lr0 << endl;

    // save to table log
    ftable_lr0 << "判定为 LR(0) 文法" << endl << endl;
    ftable_lr0 << "【LR(0)分析表】" << endl;
    ftable_lr0 << setw(10) << "|" << setw(5) << V_lr0[0] << setw(5) << "|";
    for(int i = 1 ; i < V_lr0.size() ; i++)
        ftable_lr0 << setw(5) << V_lr0[i] << setw(5) << "|";
    ftable_lr0 << endl;
    for(int i = 0 ; i <(V_lr0.size()+1)*10 ; i++)
        ftable_lr0 << "-";
    ftable_lr0 << endl;
    stringstream sin2;
    for(int i = 0 ; i < collection_lr0.size() ; i++)
    {
        ftable_lr0 << setw(5) << i << setw(5) << "|";
        for(int j = 0 ; j < V_lr0.size() ; j++)
        {
            char ch = V_lr0[j];
            if(isupper(ch))
            {
                if(Goto_lr0[i][ch] == -1)
                    ftable_lr0 << setw(10) << "|";
                else
                    ftable_lr0 << setw(5) << Goto_lr0[i][ch] << setw(5) << "|";
            }
            else
            {
                sin2.clear();
                if(action_lr0[i][ch].type == -1)
                    ftable_lr0 << setw(10) << "|";
                else
                {
                    Content_lr0& temp = action_lr0[i][ch];
                    if(temp.type == 0)
                        sin2 << "S";
                    if(temp.type == 1)
                        sin2 << "R";
                    if(temp.type == 2)
                        sin2 << "acc";
                    if(temp.num != -1)
                        sin2 << temp.num;
                    sin2 >> temp.out;
                    ftable_lr0 << setw(7) << temp.out.c_str() << setw(3) << "|";
                }
            }
        }
        ftable_lr0 << endl;
    }
    for(int i = 0 ; i <(V_lr0.size()+1)*10 ; i++)
        ftable_lr0 << "-";
    ftable_lr0 << endl;
}

void print_lr0(string s1 , string s2 , string s3 , string s4 , string s5 , string s6 , string s7)
{
    fout_lr0 << setw(15) << s1.c_str() << "|"
             << setw(15) << s2.c_str() << setw(15) << s3.c_str() << setw(20) << s4.c_str() << "|"
             << setw(15) << s5.c_str() << setw(15) << s6.c_str() << setw(15) << s7.c_str() << endl;
}

string get_steps_lr0(int x)
{
    stringstream sin;
    sin << x;
    string ret;
    sin >> ret;
    return ret;
}

template<class T>
string get_stk_lr0(vector<T> stk)
{
    stringstream sin;
    for(int i = 0 ; i < stk.size() ; i++)
        sin << stk[i];
    string ret;
    sin >> ret;
    return ret;
}

string get_shift_lr0(WF_lr0& temp)
{
    stringstream sin;
    sin << "reduce(" << temp.left << "->" << temp.right <<")";
    string out;
    sin >> out;
    return out;
}

void analyse_lr0(string src)
{
    fout_lr0 << "【过程分析】" << endl;
    print_lr0("steps","op-stack" ,"input","operation","state-stack" , "ACTION" , "GOTO");
    vector<char> op_stack;
    vector<int> st_stack;
    src+= "#";
    op_stack.push_back('#');
    st_stack.push_back(0);
    int steps = 1;
    bool is_accepted = false;
    for(int i = 0 ; i < src.length() ; i++)
    {
        char u = src[i];
        int top = st_stack[st_stack.size()-1];
        Content_lr0& act = action_lr0[top][u];
        if(act.type == 0)
        {
            print_lr0(get_steps_lr0(steps++) , get_stk_lr0(op_stack) , src.substr(i), "shift",  get_stk_lr0(st_stack) , act.out , "");
            op_stack.push_back(u);
            st_stack.push_back(act.num);
        }
        else if(act.type == 1)
        {
            WF_lr0& tt = wf_lr0[act.num];
            int y = st_stack[st_stack.size()-tt.right.length()-1];
            int x = Goto_lr0[y][tt.left[0]];
            print_lr0(get_steps_lr0(steps++) , get_stk_lr0(op_stack) , src.substr(i) , get_shift_lr0(tt) ,get_stk_lr0(st_stack),act.out,get_steps_lr0(x));
            for(int j = 0 ; j < tt.right.length() ; j++)
            {
                st_stack.pop_back();
                op_stack.pop_back();
            }
            op_stack.push_back(tt.left[0]);
            st_stack.push_back(x);
            i--;
        }
        else if(act.type == 2)
        {
            print_lr0(get_steps_lr0(steps++), get_stk_lr0(op_stack) , src.substr(i) , "Accept" , get_stk_lr0(st_stack) , act.out , "");
            is_accepted = true;
        }
        else continue;
    }
//    cout << "op_stack depth: " << op_stack.size() << endl;
//    cout << "op_stack[op_stack.size()-1]: " << op_stack[op_stack.size()-1] << endl;
    if (!is_accepted)
        throw "analyse failed!";
    if (steps == 1)
        throw "analyse failed!";
}

void clear_lr0() {
    wf_lr0.clear();
    dic_lr0.clear();
    start_lr0 = "S";
    collection_lr0.clear();
    items_lr0.clear();
    CH_lr0 = '$';
    go_lr0[MAX][MAX];
    for (int i = 0; i < MAX; i++) {
        memset(go_lr0[i], 0, MAX*sizeof(go_lr0[0][0]));
        //        memset(action_lr0[i], 0, MAX*sizeof(action_lr0[0][0]));
        memset(Goto_lr0[i], 0, MAX*sizeof(Goto_lr0[0][0]));
        for (int j = 0; j < MAX; j++) action_lr0[i][j] = Content_lr0();
    }
    memset(to_lr0, 0, MAX*sizeof(to_lr0[0]));
    memset(used_lr0, 0, MAX*sizeof(used_lr0[0]));
    V_lr0.clear();
}

int run_lr0(string fin_lr0_path, string fout_lr0_path)
{
    fin_lr0.open(fin_lr0_path);
    fout_lr0.open(fout_lr0_path);
    ftable_lr0.open("log/temp_table_lr0");

    int n;
    int result_indicator = 0;
    char s[MAX];
    try {
        fin_lr0 >> n;
        fout_lr0 << "【启动 LR(0)分析】" << endl;
        fout_lr0 << "【输入文法】" << endl;
        for(int i = 0 ; i < n ; i++)
        {
            fin_lr0 >> s;
            int len = strlen(s),j;
            for(j = 0 ; j < len ; j++)
                if(s[j] == '-') break;
            s[j] = 0;
            wf_lr0.push_back(WF_lr0(s , s+j+2 ,-1 , -1));
            wf_lr0[wf_lr0.size()-1].print_lr0();
        }
        make_item_lr0();
        make_set_lr0();
        make_V_lr0();
        make_go_lr0();
        make_table_lr0();
        analyse_lr0("abbcde");
    } catch (exception) {
        result_indicator = 1;
    } catch (const char* msg) {
        cout << msg << endl;
        result_indicator = 1;
    }

    fin_lr0.close();
    fout_lr0.close();
    ftable_lr0.close();
    clear_lr0();

    return result_indicator;
}
