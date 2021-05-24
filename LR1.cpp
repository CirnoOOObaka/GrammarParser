#include "LR1.h"

ifstream fin_lr1;
ofstream fout_lr1;
ofstream ftable_lr1;

class WF_lr1
{
public:
    string left,right;
    int back;//所属文法序号
    int id;
    char next;
    WF_lr1 (char s1[] , char s2[] , int x , int y)
    {
        left = s1;
        right = s2;
        back = x;
        id = y;
        next=0;
    }
    WF_lr1 (const string& s1 , const string& s2 , int x , int y)
    {
        left = s1;
        right = s2;
        back = x;
        id = y;
        next=0;
    }
    bool operator < (const WF_lr1& a) const
    {
        if (left == a.left){
            if(right==a.right) return next<a.next;
            else return right<a.right;
        }
        return left<a.left;
    }
    bool operator == (const WF_lr1& a) const
    {
        return (left == a.left)&& (right == a.right)&&(next==a.next);
    }
    void print_lr1 ()
    {
        //fout_lr1 << left.c_str() << "->" << right.c_str() << endl;
        //if(next!=0) cout<<left.c_str()<<"->"<<right.c_str()<<", "<<next<<endl;
        //else cout<<left<<"->"<<right<<endl;
        if(next!=0) fout_lr1<<left.c_str()<<"->"<<right.c_str()<<", "<<next<<endl;
        else fout_lr1<<left<<"->"<<right<<endl;
    }
};

class Closure_lr1
{
public:
    vector<WF_lr1> element;
    vector<int> con;
    map<WF_lr1,int>mp;
    int newseq;
    int flag=0;//是否被合并:0为未合并，1为合并
    void print_lr1 (string str)
    {
        fout_lr1 << setw(15) << "" << setw(15) << str.c_str() << endl;
        //cout<< setw(15) << "" << setw(15) << str.c_str() << endl;
        for (int i = 0 ; i < element.size() ; i++)
            element[i].print_lr1();
    }
    bool operator == (const Closure_lr1& a) const
    {
        if (a.element.size() != element.size()) return false;
        for (int i = 0 ; i < a.element.size() ; i++)
            if (element[i] == a.element[i]) continue;
            else return false;
        return true;
    }
    void Sort(){
        sort(element.begin(),element.end());
    }
};

struct Content_lr1
{
    int type;
    int num;
    string out;
    Content_lr1(){ type = -1; }
    Content_lr1 (int a , int b)
        :type(a),num(b){}
};

vector<WF_lr1> wf_lr1;//文法
map<string,vector<int> > dic_lr1;
map<string,vector<int> > VN_set_lr1;//某个非终结符的所有产生式
map<string,bool>  vis_lr1;
set<char>ch;//转移字符
string start_lr1 = "S";
vector<Closure_lr1> collection_lr1;
vector<WF_lr1> items_lr1;//项目
char CH_lr1 = '.';
int go_lr1[MAX][MAX];
int to_lr1[MAX];
map<pair<int,int>,vector<char> >e;
map<pair<int,char>,int> goarr;
vector<char> V_lr1;
bool used_lr1[MAX];
Content_lr1 action_lr1[MAX][MAX];
int Goto_lr1[MAX][MAX];
map<string,set<char> > first_lr1;
map<string,set<char> > follow_lr1;

void clear_lr1 ()
{
    ch.clear();
    wf_lr1.clear();
    dic_lr1.clear();
    VN_set_lr1.clear();
    vis_lr1.clear();
    start_lr1 = "S";
    collection_lr1.clear();
    items_lr1.clear();
    CH_lr1 = '.';
    V_lr1.clear();
    first_lr1.clear();
    follow_lr1.clear();
    e.clear();
    goarr.clear();
    for (int i = 0; i < MAX; i++) {
        memset(go_lr1[i], 0, MAX * sizeof(go_lr1[0][0]));
        memset(Goto_lr1[i], 0, MAX * sizeof(Goto_lr1[0][0]));
        for (int j = 0; j < MAX; j++)   action_lr1[i][j] = Content_lr1();
//        memset(action_slr1, 0, MAX * sizeof(action_slr1[0][0]));
    }
    memset(to_lr1, 0, sizeof(to_lr1[0]));
    memset(used_lr1, 0, sizeof(used_lr1[0]));
}
void make_item_lr1 ()
{
    memset (to_lr1 , -1 , sizeof (-1));
    for (int i = 0 ; i < wf_lr1.size() ; i++){
        //wf_lr1是产生式集
        VN_set_lr1[wf_lr1[i].left].push_back (i);
        ch.insert(wf_lr1[i].left[0]);
        string right=wf_lr1[i].right;
        for(int x=0;x<right.length();++x){
            ch.insert(right[x]);
        }
    }
    for (int i = 0 ; i < wf_lr1.size() ; i++)//某个非终结符的所有产生式
        for (int j = 0 ; j <= wf_lr1[i].right.length() ; j++)
        {
            string temp = wf_lr1[i].right;
            temp.insert (temp.begin()+j , CH_lr1);
            dic_lr1[wf_lr1[i].left].push_back (items_lr1.size());
            if (j)
                to_lr1[items_lr1.size()-1] = items_lr1.size();
            items_lr1.push_back (WF_lr1 (wf_lr1[i].left , temp , i , items_lr1.size()));
        }
}
Closure_lr1 get_closure(Closure_lr1 I){
    for(int i=0;i<I.element.size();++i){
        WF_lr1 a=I.element[i];
        //a.print_lr1();
        string right=a.right;
        int x=0;
        while(right[x]!='.') x++;
        if(x!=right.length()-1&&isupper(right[x+1])){
            x++;
            set<char>fi;
            if(x!=right.length()-1){
                if(isupper(right[x+1])) fi=first_lr1[right.substr(x,1)];
                else fi.insert(right[x+1]);
            }
            else fi.insert(a.next);
            set<char>::iterator it=fi.find('~');
            if(it!=fi.end()) fi.insert(a.next);
            vector<int> &v=dic_lr1[right.substr(x,1)];
            for(int j=0;j<v.size();++j){
                int b=v[j];
                if(items_lr1[b].right[0]=='.'){
                    it=fi.begin();
                    while(it!=fi.end()){
                        char s=*(it);
                        it++;
                        if(s=='~') continue;
                        WF_lr1 g=items_lr1[b];
                        g.next=s;
                        if(I.mp[g]!=1){
                            I.element.push_back(g);
                            I.mp[g]=1;
                        }
                    }
                }
            }
        }
    }
    return I;
}
Closure_lr1 Go(Closure_lr1 I,char X){
    Closure_lr1 J;
    for(int i=0;i<I.element.size();++i){
        WF_lr1 a=I.element[i];
        string right=a.right;
        int x=0;
        while(right[x]!='.') x++;
        if(x!=right.length()&&right[x+1]==X){
            WF_lr1 g=items_lr1[a.id+1];
            g.next=a.next;
            if(J.mp[g]!=1){
                J.element.push_back(g);
                J.mp[g]=1;
            }
        }
    }
    return get_closure(J);
}
void get_items(){
    Closure_lr1 a;
    WF_lr1 g=items_lr1[0];
    g.next='#';
    a.element.push_back(g);
    Closure_lr1 I=get_closure(a);
    I.Sort();
    //I.print_lr1("I0");
    vector<WF_lr1> &v=I.element;
    collection_lr1.push_back(I);
    for(int i=0;i<collection_lr1.size();++i){
        Closure_lr1 I=collection_lr1[i];
        set<char>::iterator it=ch.begin();
        while(it!=ch.end()){
            char X=(*it);
            it++;
            Closure_lr1 J=Go(I,X);
            sort(J.element.begin(),J.element.end());
            if(J.element.size()!=0){
                int u=-1;
                for(int x=0;x<collection_lr1.size();++x) if(J==collection_lr1[x]){
                    u=x;
                    break;
                }
                if(u!=-1) e[make_pair(i,u)].push_back(X);
                else{
                    e[make_pair(i,collection_lr1.size())].push_back(X);
                    collection_lr1.push_back(J);
                }
            }
        }
    }
}
void print_items(){
    fout_lr1<<"【CLOSURE】"<<endl;
    for(int i=0;i<collection_lr1.size();++i){
        Closure_lr1 I=collection_lr1[i];
        //printf("I%d:\n",i);
        fout_lr1<<"closure-I"<<i<<":"<<endl;
        for(int x=0;x<I.element.size();++x){
            fout_lr1<<I.element[x].left<<"->"<<I.element[x].right<<", "<<I.element[x].next<<endl;
        }
        //printf("\n");
        fout_lr1<<endl;
    }
}
void print_edge(){
    //printf("边：\n");
    fout_lr1<<"【EDGE】"<<endl;
    for(int i=0;i<collection_lr1.size();++i){
        for(int j=0;j<collection_lr1.size();++j){
            vector<char> &v=e[make_pair(i,j)];
            for(int x=0;x<v.size();++x){
                goarr.insert(make_pair(make_pair(i, v[x]),j));
                //printf("I%d->%c->I%d\n",i,v[x],j);
                fout_lr1<<"I"<<i<<"->"<<v[x]<<"->I"<<j<<endl;
            }
        }
    }
}

void make_V_lr1 ()
{
    memset (used_lr1 , 0 , sizeof (used_lr1));
    for (int i = 0 ; i < wf_lr1.size() ; i++)
    {
        string& str = wf_lr1[i].left;
        for (int j = 0 ; j < str.length() ; j++)
        {
            if (used_lr1[str[j]]) continue;
            used_lr1[str[j]] = 1;
            V_lr1.push_back (str[j]);
        }
        string& str1 = wf_lr1[i].right;
        for (int j = 0 ; j < str1.length() ; j++)
        {
            if (used_lr1[str1[j]]) continue;
            used_lr1[str1[j]] = 1;
            V_lr1.push_back (str1[j]);
        }
    }
    sort (V_lr1.begin() , V_lr1.end());
    V_lr1.push_back ('#');
}

void make_table_lr1 (int &result_indicator)
{
    /*cout<<"V_lr1.size()"<<V_lr1.size()<<endl;
    for(int i=0;i<V_lr1.size();i++){
        cout<<V_lr1[i]<<endl;
    }*/
    memset (Goto_lr1 , -1 , sizeof (Goto_lr1));
    //write s to_lr1 the table
    /*对于每个项目集Ii中形如［A→α·Xβ，b］的项目，若GO(Ii,X)＝Ij，
    且X＝a，(a∈VT)时，置action［Ii,a］＝Sj。若X∈VN时，则置:GOTO［Ii,X］＝j
    */
    for(int i = 0 ; i < collection_lr1.size() ; i++){
        for (int j = 0 ; j < V_lr1.size() ; j++)
        {
            char ch = V_lr1[j];
            int x = goarr[make_pair(i, ch)];
            //int x = go_lr1[i][ch];	//获取go(i,a)=j;x就是j
            if (x == -1) continue;
            if (!isupper(ch)){	//判断大写字母；这里就是判断是否为终结符。如果是终结符，   填入Sj
                //action_lr1[i][ch] = Content_lr1(0, x);
                if(x!=0){
                    action_lr1[i][ch] = Content_lr1(0, x); //这里type=0，x=j  ;s部分已经都对了
                }
            }
            else{
                if(x!=0){
                    Goto_lr1[i][ch] = x;	//如果是非终结符，就填入goto部分
                }
            }
        }
    }
    //write r and acc to_lr1 the table
    for (int i = 0 ; i < collection_lr1.size() ; i++){
        for (int j = 0 ; j < collection_lr1[i].element.size() ; j++)	//对一个项目集的遍历
        {
            WF_lr1& tt = collection_lr1[i].element[j];
            if (tt.right[tt.right.length()-1] == CH_lr1)	//规约项目A->a.型
            {
                /*if (tt.left[0] == 'S')
                    action_lr1[i]['#'] = Content_lr1 (2 , -1);	//acc对应的type为2
                else*/
                    for (int k = 0 ; k < V_lr1.size() ; k++)	//遍历每一个非终结符
                    {
                        int y = tt.next;
                        if(tt.back==0){
                            action_lr1[i]['#'] = Content_lr1 (2 , -1);
                        }
                        else{
                            if(action_lr1[i][y].type==0){
                                    result_indicator = 1;
                            }
                            action_lr1[i][y] = Content_lr1 (1, tt.back+1);
                        }

                        /*if(!isupper(y)){

                        }
                        else{
                            continue;
                        }*/
                        /*if (!follow_lr1[tt.left].count(V_lr1[k])) continue;
                        action_lr1[i][y] = Content_lr1 (1, tt.back);*/
                    }
            }
        }
    }
    // save to complete log
        fout_lr1 << "【LR(1)分析表】" << endl;
        fout_lr1 << setw(10) << "|" << setw(5) << V_lr1[0] << setw(5) << "|";
        for (int i = 1 ; i < V_lr1.size() ; i++)
            fout_lr1 << setw(5) << V_lr1[i] << setw(5) << "|";
        fout_lr1 << endl;
        for (int i = 0 ; i < (V_lr1.size()+1)*10 ; i++)
            fout_lr1 << "-";
        fout_lr1 << endl;
        stringstream sin;
        for (int i = 0 ; i < collection_lr1.size() ; i++)
        {
            /*if(collection_lr1[i].flag==1){
                continue;
            }*/
            fout_lr1 << setw(5) << i << setw(5) << "|";
            for (int j = 0 ; j < V_lr1.size() ; j++)
            {
                char ch = V_lr1[j];
                if (isupper(ch))
                {
                    if (Goto_lr1[i][ch] == -1)
                        fout_lr1 << setw(10) << "|";
                    else
                        fout_lr1 << setw(5) << Goto_lr1[i][ch] << setw(5) << "|";
                }
                else
                {
                    sin.clear();
                    if (action_lr1[i][ch].type == -1)
                        fout_lr1 << setw(10) << "|";
                    else
                    {
                        Content_lr1& temp = action_lr1[i][ch];
                        if (temp.type == 0)
                            sin << "S";
                        if (temp.type == 1)
                            sin << "R";
                        if (temp.type == 2)
                            sin << "acc";
                        if (temp.num != -1)
                            sin << temp.num;
                        sin >> temp.out;
                        fout_lr1 << setw(7) << temp.out.c_str() << setw(3) << "|";
                    }
                }
            }
            fout_lr1 << endl;
        }
        for (int i = 0 ; i < (V_lr1.size()+1)*10 ; i++)
            fout_lr1 << "-";
        fout_lr1 << endl;

        // save to table log
        ftable_lr1 << "判定为 LR(1) 文法" << endl << endl;
        ftable_lr1 << "【LR(1)分析表】" << endl;
        ftable_lr1 << setw(10) << "|" << setw(5) << V_lr1[0] << setw(5) << "|";
        for (int i = 1 ; i < V_lr1.size() ; i++)
            ftable_lr1 << setw(5) << V_lr1[i] << setw(5) << "|";
        ftable_lr1 << endl;
        for (int i = 0 ; i < (V_lr1.size()+1)*10 ; i++)
            ftable_lr1 << "-";
        ftable_lr1 << endl;
        stringstream sin2;
        for (int i = 0 ; i < collection_lr1.size() ; i++)
        {
            //
            ftable_lr1 << setw(5) << i << setw(5) << "|";
            for (int j = 0 ; j < V_lr1.size() ; j++)
            {
                char ch = V_lr1[j];
                if (isupper(ch))
                {
                    if (Goto_lr1[i][ch] == -1)
                        ftable_lr1 << setw(10) << "|";
                    else
                        ftable_lr1 << setw(5) << Goto_lr1[i][ch] << setw(5) << "|";
                }
                else
                {
                    sin2.clear();
                    if (action_lr1[i][ch].type == -1)
                        ftable_lr1 << setw(10) << "|";
                    else
                    {
                        Content_lr1& temp = action_lr1[i][ch];
                        if (temp.type == 0)
                            sin2 << "S";
                        if (temp.type == 1)
                            sin2 << "R";
                        if (temp.type == 2)
                            sin2 << "acc";
                        if (temp.num != -1)
                            sin2 << temp.num;
                        sin2 >> temp.out;
                        ftable_lr1 << setw(7) << temp.out.c_str() << setw(3) << "|";
                    }
                }
            }
            ftable_lr1 << endl;
        }
        for (int i = 0 ; i < (V_lr1.size()+1)*10 ; i++)
            ftable_lr1 << "-";
        ftable_lr1 << endl;
}
void dfs_lr1 (const string& x)
{
    if (vis_lr1[x]) return;
    vis_lr1[x] = 1;
    vector<int>& id = VN_set_lr1[x];
    for (int i = 0 ; i < id.size() ; i++)
    {
        string& left = wf_lr1[id[i]].left;
        string& right = wf_lr1[id[i]].right;
        for (int j = 0 ; j < right.length() ; j++)
            if (isupper(right[j]))
            {
                dfs_lr1 (right.substr(j,1));
                set<char>& temp = first_lr1[right.substr(j,1)];
                set<char>::iterator it = temp.begin();
                bool flag = true;
                for (; it != temp.end() ; it++)
                {
                    if (*it == '~') flag = false;
                    first_lr1[left].insert (*it);
                }
                if (flag) break;
            }
            else
            {
                first_lr1[left].insert (right[j]);
                break;
            }
    }
}
void append_lr1 (const string& str1 , const string& str2)
{
    set<char>& from = follow_lr1[str1];
    set<char>& to_lr1 = follow_lr1[str2];
    set<char>::iterator it = from.begin();
    for (; it != from.end() ; it++)
        to_lr1.insert (*it);
}
bool _checkLR1 (const vector<int>& id, const string str)
{

    for (int i = 0 ; i < id.size() ; i++)
    {
        int x = id[i];
        if (wf_lr1[x].right == str) return true;
    }
    return false;
}
void make_first_lr1 ()
{
    vis_lr1.clear();
    map<string,vector<int> >::iterator it2 = dic_lr1.begin();
    for (; it2 != dic_lr1.end() ; it2++)
        if (vis_lr1[it2->first]) continue;
        else dfs_lr1 (it2->first);
    fout_lr1 << "【FIRST集】" << endl;
    map<string,set<char> >::iterator it = first_lr1.begin();
    for (; it != first_lr1.end() ; it++)
    {
        fout_lr1 << "FIRST(" << it->first.c_str() << ")={";
        set<char> & temp = it->second;
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for (; it1 != temp.end() ; it1++)
        {
            if (flag)
                fout_lr1 << ",";
            fout_lr1 << *it1;
            flag = true;
        }
        fout_lr1 << "}" << endl;
    }
}
void make_follow_lr1 ()
{
    while (true)
    {
        bool goon = false;
        map<string,vector<int> >::iterator it2 = VN_set_lr1.begin();
        for (; it2 != VN_set_lr1.end() ; it2++)
        {
            vector<int>& id = it2->second;
            for (int i = 0 ; i < id.size() ; i++)
            {
                bool flag = true;
                WF_lr1& tt = wf_lr1[id[i]];
                string& left = tt.left;
                const string& right = tt.right;
                for (int j = right.length()-1 ; j >= 0 ; j--)
                    if (isupper(right[j]))
                    {
                        if (flag)
                        {
                            int tx = follow_lr1[right.substr(j,1)].size();
                            append_lr1(left , right.substr(j,1));
                            int tx1 = follow_lr1[right.substr(j,1)].size();
                            if (tx1 > tx) goon = true;
                            if (_checkLR1 (id , "~"))
                                flag = false;
                        }
                        for (int k = j+1 ; k < right.length() ; k++)
                            if (isupper(right[k]))
                            {
                                string idd = right.substr(k,1);
                                set<char>& from = first_lr1[idd];
                                set<char>& to_lr1 = follow_lr1[right.substr(j,1)];
                                set<char>::iterator it1 = from.begin();
                                int tx = follow_lr1[right.substr(j,1)].size();
                                for (; it1 != from.end() ; it1++)
                                    if (*it1 != '~')
                                        to_lr1.insert (*it1);
                                int tx1 = follow_lr1[right.substr(j,1)].size();
                                if (tx1 > tx) goon = true;
                                if (_checkLR1 (id , "~"))
                                    break;
                            }
                            else
                            {
                                int tx = follow_lr1[right.substr(j,1)].size();
                                follow_lr1[right.substr(j,1)].insert (right[k]);
                                int tx1 = follow_lr1[right.substr(j,1)].size();
                                if (tx1 > tx) goon = true;
                                break;
                            }
                    }
                    else flag = false;
            }
        }
        if (!goon) break;
    }
    fout_lr1 << "【FOLLOW集】" << endl;
    map<string,set<char> >::iterator it = follow_lr1.begin();
    for (; it != follow_lr1.end() ; it++)
    {
        fout_lr1 << "FOLLOW(" << it->first.c_str() << ")={";
        set<char> & temp = it->second;
        //if (it->first[0] == 'S')
        temp.insert ('#');
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for (; it1 != temp.end() ; it1++)
        {
            if (flag)
                fout_lr1 << ",";
            fout_lr1 << *it1;
            flag = true;
        }
        fout_lr1 << "}" << endl;
    }
}

int run_lr1andslr1 (string fin_lr1_path, string fout_lr1_path)
{
    //freopen("1.in","r",stdin);
    fin_lr1.open(fin_lr1_path);
    fout_lr1.open(fout_lr1_path);
    ftable_lr1.open("log/temp_table_lr1");
    int n;
    int result_indicator = 0;
    char s[MAX];
    try {
        fin_lr1 >> n;
        fout_lr1 << "【启动 LR(1)分析】" << endl;
        fout_lr1 << "【输入文法】" << endl;
        for (int i = 0 ; i < n ; i++)
        {
            fin_lr1>>s;
            int len = strlen(s),j;
            for (j = 0 ; j < len ; j++)
                if (s[j] == '-') break;
            s[j] = 0;
            wf_lr1.push_back (WF_lr1 (s , s+j+2 ,-1 , -1));
        }
        //cout<<"输入文法"<<endl;
        for(int i=0;i<n;++i){
            wf_lr1[i].print_lr1();
        }
        printf("\n");
        make_item_lr1();
        make_first_lr1();
        make_follow_lr1();
        get_items();
        print_items();
        print_edge();
        make_V_lr1();
        make_table_lr1(result_indicator);
    } catch (exception) {
        result_indicator = 1;
    } catch (const char* msg) {
        cout << msg << endl;
        result_indicator = 1;
    }
    //fout_lr1<<result_indicator<<endl;
    fin_lr1.close();
    fout_lr1.close();
    ftable_lr1.close();
    clear_lr1();
    return result_indicator;
}
