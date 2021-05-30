#include "LALR1.h"
const int MAX=509;
ifstream fin_lalr1;
ofstream fout_lalr1;
ofstream ftable_lalr1;

class WF_lalr1
{
public:
    string left,right;
    int back;//所属文法序号
    int id;
    char next;
    WF_lalr1 (char s1[] , char s2[] , int x , int y)
    {
        left = s1;
        right = s2;
        back = x;
        id = y;
        next=0;
    }
    WF_lalr1 (const string& s1 , const string& s2 , int x , int y)
    {
        left = s1;
        right = s2;
        back = x;
        id = y;
        next=0;
    }
    bool operator < (const WF_lalr1& a) const
    {
        if (left == a.left){
            if(right==a.right) return next<a.next;
            else return right<a.right;
        }
        return left<a.left;
    }
    bool operator == (const WF_lalr1& a) const
    {
        return (left == a.left)&& (right == a.right)&&(next==a.next);
    }
    void print_lalr1 ()
    {
        //fout_lalr1 << left.c_str() << "->" << right.c_str() << endl;
        //if(next!=0) cout<<left.c_str()<<"->"<<right.c_str()<<", "<<next<<endl;
        //else cout<<left<<"->"<<right<<endl;
        if(next!=0) fout_lalr1<<left.c_str()<<"->"<<right.c_str()<<", "<<next<<endl;
        else fout_lalr1<<left<<"->"<<right<<endl;
    }
};

class Closure_lalr1
{
public:
    vector<WF_lalr1> element;
    vector<int> con;
    map<WF_lalr1,int>mp;
    int newseq;
    int flag=0;//是否被合并:0为未合并，1为合并
    void print_lalr1 (string str)
    {
        fout_lalr1 << setw(15) << "" << setw(15) << str.c_str() << endl;
        //cout<< setw(15) << "" << setw(15) << str.c_str() << endl;
        for (int i = 0 ; i < element.size() ; i++)
            element[i].print_lalr1();
    }
    bool operator == (const Closure_lalr1& a) const
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

struct Content_lalr1
{
    int type;
    int num;
    string out;
    Content_lalr1(){ type = -1; }
    Content_lalr1 (int a , int b)
        :type(a),num(b){}
};

vector<WF_lalr1> wf_lalr1;//文法
map<string,vector<int> > dic_lalr1;
map<string,vector<int> > VN_set_lalr1;//某个非终结符的所有产生式
map<string,bool>  vis_lalr1;
set<char>ch_lalr1;//转移字符
string start_lalr1 = "S";
vector<Closure_lalr1> collection_lalr1;
vector<WF_lalr1> items_lalr1;//项目
char CH_lalr1 = '.';
int go_lalr1[MAX][MAX];
int to_lalr1[MAX];
map<pair<int,int>,vector<char> >e_lalr1;
map<pair<int,char>,int> goarr_lalr1;
vector<char> V_lalr1;
bool used_lalr1[MAX];
Content_lalr1 action_lalr1[MAX][MAX];
int Goto_lalr1[MAX][MAX];
int action_check2[MAX][MAX];
int Goto_check2[MAX][MAX];
map<string,set<char> > first_lalr1;
map<string,set<char> > follow_lalr1;

void clear_lalr1 ()
{
    ch_lalr1.clear();
    wf_lalr1.clear();
    dic_lalr1.clear();
    VN_set_lalr1.clear();
    vis_lalr1.clear();
    start_lalr1 = "S";
    collection_lalr1.clear();
    items_lalr1.clear();
    CH_lalr1 = '.';
    V_lalr1.clear();
    first_lalr1.clear();
    follow_lalr1.clear();
    e_lalr1.clear();
    goarr_lalr1.clear();
    for (int i = 0; i < MAX; i++) {
        memset(go_lalr1[i], 0, MAX * sizeof(go_lalr1[0][0]));
        memset(Goto_lalr1[i], 0, MAX * sizeof(Goto_lalr1[0][0]));
        for (int j = 0; j < MAX; j++){
            action_check2[i][j]=-1;
            Goto_check2[i][j] = -1;
            action_lalr1[i][j] = Content_lalr1();
        }
//        memset(action_slr1, 0, MAX * sizeof(action_slr1[0][0]));
    }
    memset(to_lalr1, 0, sizeof(to_lalr1[0]));
    memset(used_lalr1, 0, sizeof(used_lalr1[0]));
}
void make_item_lalr1 ()
{
    memset (to_lalr1 , -1 , sizeof (-1));
    for (int i = 0 ; i < wf_lalr1.size() ; i++){
        //wf_lalr1是产生式集
        VN_set_lalr1[wf_lalr1[i].left].push_back (i);
        ch_lalr1.insert(wf_lalr1[i].left[0]);
        string right=wf_lalr1[i].right;
        for(int x=0;x<right.length();++x){
            ch_lalr1.insert(right[x]);
        }
    }
    for (int i = 0 ; i < wf_lalr1.size() ; i++)//某个非终结符的所有产生式
        for (int j = 0 ; j <= wf_lalr1[i].right.length() ; j++)
        {
            string temp = wf_lalr1[i].right;
            temp.insert (temp.begin()+j , CH_lalr1);
            dic_lalr1[wf_lalr1[i].left].push_back (items_lalr1.size());
            if (j)
                to_lalr1[items_lalr1.size()-1] = items_lalr1.size();
            items_lalr1.push_back (WF_lalr1 (wf_lalr1[i].left , temp , i , items_lalr1.size()));
        }
}
Closure_lalr1 get_lalr1_closure(Closure_lalr1 I){
    for(int i=0;i<I.element.size();++i){
        WF_lalr1 a=I.element[i];
        //a.print_lalr1();
        string right=a.right;
        int x=0;
        while(right[x]!='.') x++;
        if(x!=right.length()-1&&isupper(right[x+1])){
            x++;
            set<char>fi;
            if(x!=right.length()-1){
                if(isupper(right[x+1])) fi=first_lalr1[right.substr(x,1)];
                else fi.insert(right[x+1]);
            }
            else fi.insert(a.next);
            set<char>::iterator it=fi.find('~');
            if(it!=fi.end()) fi.insert(a.next);
            vector<int> &v=dic_lalr1[right.substr(x,1)];
            for(int j=0;j<v.size();++j){
                int b=v[j];
                if(items_lalr1[b].right[0]=='.'){
                    it=fi.begin();
                    while(it!=fi.end()){
                        char s=*(it);
                        it++;
                        if(s=='~') continue;
                        WF_lalr1 g=items_lalr1[b];
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
Closure_lalr1 Go_lalr1(Closure_lalr1 I,char X){
    Closure_lalr1 J;
    for(int i=0;i<I.element.size();++i){
        WF_lalr1 a=I.element[i];
        string right=a.right;
        int x=0;
        while(right[x]!='.') x++;
        if(x!=right.length()&&right[x+1]==X){
            WF_lalr1 g=items_lalr1[a.id+1];
            g.next=a.next;
            if(J.mp[g]!=1){
                J.element.push_back(g);
                J.mp[g]=1;
            }
        }
    }
    return get_lalr1_closure(J);
}

void get_lalr1_items(){
    Closure_lalr1 a;
    WF_lalr1 g=items_lalr1[0];
    g.next='#';
    a.element.push_back(g);
    Closure_lalr1 I=get_lalr1_closure(a);
    I.Sort();
    //I.print_lalr1("I0");
    vector<WF_lalr1> &v=I.element;
    collection_lalr1.push_back(I);
    for(int i=0;i<collection_lalr1.size();++i){
        Closure_lalr1 I=collection_lalr1[i];
        set<char>::iterator it=ch_lalr1.begin();
        while(it!=ch_lalr1.end()){
            char X=(*it);
            it++;
            Closure_lalr1 J=Go_lalr1(I,X);
            sort(J.element.begin(),J.element.end());
            if(J.element.size()!=0){
                int u=-1;
                for(int x=0;x<collection_lalr1.size();++x) if(J==collection_lalr1[x]){
                    u=x;
                    break;
                }
                if(u!=-1) e_lalr1[make_pair(i,u)].push_back(X);
                else{
                    e_lalr1[make_pair(i,collection_lalr1.size())].push_back(X);
                    collection_lalr1.push_back(J);
                }
            }
        }
    }
}
/*void print_items(){
    printf("项目集规范族：\n");
    for(int i=0;i<collection_lalr1.size();++i){
        Closure_lalr1 I=collection_lalr1[i];
        printf("I%d:\n",i);
        for(int x=0;x<I.element.size();++x){
            cout<<I.element[x].left<<"->"<<I.element[x].right<<", "<<I.element[x].next<<endl;
            fout_lalr1<<I.element[x].left<<"->"<<I.element[x].right<<", "<<I.element[x].next<<endl;
        }
        printf("\n");
    }
}
void print_edge(){
    printf("边：\n");
    for(int i=0;i<collection_lalr1.size();++i){
        for(int j=0;j<collection_lalr1.size();++j){
            vector<char> &v=e_lalr1[make_pair(i,j)];
            for(int x=0;x<v.size();++x){
                goarr_lalr1.insert(make_pair(make_pair(i, v[x]),j));
                printf("I%d->%c->I%d\n",i,v[x],j);
            }
        }
    }
}*/
int Closure_equal(Closure_lalr1 I,Closure_lalr1 J){
    //相等返回0，不等返回1
    for (int i = 0; i < I.element.size();i++){
        //判断I中项目都在J中
        WF_lalr1 m=I.element[i];
        int sign = 0;
        for (int j = 0; j < J.element.size();j++){
            WF_lalr1 m2=J.element[j];
            if(m.left==m2.left&&m.right==m2.right){
                sign = 1;
                break;
            }
        }
        if(sign==0){
            return 1;
        }
    }
    for (int i = 0; i < J.element.size();i++){
        //判断J中项目都在I中
        WF_lalr1 m=J.element[i];
        int sign = 0;
        for (int j = 0; j < I.element.size();j++){
            WF_lalr1 m2=J.element[j];
            if(m.left==m2.left&&m.right==m2.right){
                sign = 1;
                break;
            }
        }
        if(sign==0){
            return 1;
        }
    }
    return 0;
}
void get_merge_items(){
    for(int i=0;i<collection_lalr1.size();++i){
        if(collection_lalr1[i].flag==1){
            continue;
        }
        Closure_lalr1 I=collection_lalr1[i];
        collection_lalr1[i].newseq=i;
        for(int j=i+1;j<collection_lalr1.size();j++){
            if(collection_lalr1[j].flag==1){
                continue;
            }
            Closure_lalr1 J=collection_lalr1[j];
            collection_lalr1[j].newseq=j;
            if(Closure_equal(I,J)==0){
                //对项目闭包的排序
                //还有更改后续print_items和print_edge
                //以及冲突的判断
                //还有一个方法是另开一个collection来实现，这个也要涉及到后面的排序等等
                //现在思路是在对Closure_lalr1加一个序号：这个序号用来存储合并后的
                //加入con中
                I.con.push_back(j);
                //内容的合并
                for(int z=0;z<J.element.size();z++){
                    collection_lalr1[i].element.push_back(J.element[z]);
                }
                //下标的改变；
                //printf("I:%d J:%d\n",I.newseq,J.newseq);
                collection_lalr1[i].newseq=i*10+j;
                //把J闭包移除
                collection_lalr1[j].flag=1;
            }
        }
        for(int z=0;z<I.con.size();z++){
            collection_lalr1[I.con[z]].newseq=collection_lalr1[i].newseq;
        }
    }
}


void make_V_lalr1 ()
{
    memset (used_lalr1 , 0 , sizeof (used_lalr1));
    for (int i = 0 ; i < wf_lalr1.size() ; i++)
    {
        string& str = wf_lalr1[i].left;
        for (int j = 0 ; j < str.length() ; j++)
        {
            if (used_lalr1[str[j]]) continue;
            used_lalr1[str[j]] = 1;
            V_lalr1.push_back (str[j]);
        }
        string& str1 = wf_lalr1[i].right;
        for (int j = 0 ; j < str1.length() ; j++)
        {
            if (used_lalr1[str1[j]]) continue;
            used_lalr1[str1[j]] = 1;
            V_lalr1.push_back (str1[j]);
        }
    }
    sort (V_lalr1.begin() , V_lalr1.end());
    V_lalr1.push_back ('#');
}
void print_items2(){
    //转移函数的改变:Go：这里要变两边的：原本到J的项目集现在到了I；
    //这里也不用改，只需要在输出边时，变成输出newseq就行了
    //由于相同的心经过转换函数到达的心仍然相同，所以不用：只需要输出更改为flag=1的要输出，flag=0的不用
    //printf("合并后：项目集规范族：\n");
    fout_lalr1 << "【CLOSURE(Merged)】" << endl;
    for(int i=0;i<collection_lalr1.size();++i){
        if(collection_lalr1[i].flag==1){
            continue;
        }
        //printf("i is %d,flag is %d\n", i, collection_lalr1[i].flag);
        Closure_lalr1 I=collection_lalr1[i];
        printf("I%d:\n",I.newseq);
        fout_lalr1<<"closure-I"<<I.newseq<<endl;
        for(int x=0;x<I.element.size();++x){
            //cout<<I.element[x].left<<"->"<<I.element[x].right<<", "<<I.element[x].next<<endl;
            fout_lalr1<<I.element[x].left<<"->"<<I.element[x].right<<", "<<I.element[x].next<<endl;

        }
        printf("\n");
        fout_lalr1<<endl;
    }
}
void print_edge2(){
    //printf("边：\n");
    fout_lalr1<<"【EDGE】"<<endl;
    for(int i=0;i<collection_lalr1.size();++i){
        if(collection_lalr1[i].flag==1){
            continue;
        }
        for(int j=0;j<collection_lalr1.size();++j){
            vector<char> &v=e_lalr1[make_pair(i,j)];
            for(int x=0;x<v.size();++x){
                goarr_lalr1.insert(make_pair(make_pair(i, v[x]),j));
                //printf("I%d->%c->I%d\n",collection_lalr1[i].newseq,v[x],collection_lalr1[j].newseq);
                fout_lalr1<<"I"<<collection_lalr1[i].newseq<<"->"<<v[x]<<"->I"<<j<<endl;
            }
        }
    }
}
void make_table_lalr1 (int &result_indicator2)
{
    /*cout<<"V_lalr1.size()"<<V_lalr1.size()<<endl;
    for(int i=0;i<V_lalr1.size();i++){
        cout<<V_lalr1[i]<<endl;
    }*/
    memset (Goto_lalr1 , -1 , sizeof (Goto_lalr1));
    memset(action_check2,-1,sizeof(action_check2));
    memset(Goto_check2,-1,sizeof(Goto_check2));
    //write s to_lalr1 the table
    /*对于每个项目集Ii中形如［A→α·Xβ，b］的项目，若GO(Ii,X)＝Ij，
    且X＝a，(a∈VT)时，置action［Ii,a］＝Sj。若X∈VN时，则置:GOTO［Ii,X］＝j
    */
    for(int i = 0 ; i < collection_lalr1.size() ; i++){
        if(collection_lalr1[i].flag==1){
            continue;
        }
        for (int j = 0 ; j < V_lalr1.size() ; j++)
        {
            char ch = V_lalr1[j];
            int x = goarr_lalr1[make_pair(i, ch)];
            //int x = go_lalr1[i][ch];	//获取go(i,a)=j;x就是j
            if (x == -1) continue;
            if (!isupper(ch)){	//判断大写字母；这里就是判断是否为终结符。如果是终结符，   填入Sj
                //action_lalr1[i][ch] = Content_lalr1(0, x);
                if(x!=0){
                    if(action_check2[i][ch]!=-1){
                        result_indicator2 = 1;
                    }
                    action_lalr1[i][ch] = Content_lalr1(0, collection_lalr1[x].newseq); //这里type=0，x=j  ;s部分已经都对了
                    action_check2[i][ch] = 1;
                    //printf("action %d %c %d\n", i, ch, result_indicator2);
                }
            }
            else{
                if(x!=0){
                    if(Goto_check2[i][ch]!=-1){
                        result_indicator2 = 1;
                    }
                    Goto_lalr1[i][ch] = collection_lalr1[x].newseq;	//如果是非终结符，就填入goto部分
                    Goto_check2[i][ch] = 1;
                    //printf("goto %d %c %d\n", i, ch, result_indicator2);
                }
            }
        }
    }
    //write r and acc to_lalr1 the table
    for (int i = 0 ; i < collection_lalr1.size() ; i++){
        if(collection_lalr1[i].flag==1){
            continue;
        }
        for (int j = 0 ; j < collection_lalr1[i].element.size() ; j++)	//对一个项目集的遍历
        {
            WF_lalr1& tt = collection_lalr1[i].element[j];
            if (tt.right[tt.right.length()-1] == CH_lalr1)	//规约项目A->a.型
            {
                /*if (tt.left[0] == 'S')
                    action_lalr1[i]['#'] = Content_lalr1 (2 , -1);	//acc对应的type为2
                else*/
                    for (int k = 0 ; k < V_lalr1.size() ; k++)	//遍历每一个非终结符
                    {
                        int y = tt.next;
                        if(tt.back==0){
                            action_lalr1[i]['#'] = Content_lalr1 (2 , -1);
                            action_check2[i]['#'] = 1;
                            printf("action %d # %d\n", i, result_indicator2);
                        }
                        else{
                            if(action_check2[i][y]==1&&action_lalr1[i][y].num!=tt.back+1){
                                result_indicator2 = 1;
                            }
                            action_lalr1[i][y] = Content_lalr1 (1, tt.back+1);
                            action_check2[i][y] = 1;
                            //printf("action next %d %d %d back:%d\n", collection_lalr1[i].newseq, y, result_indicator2,tt.back);
                        }

                        /*if(!isupper(y)){

                        }
                        else{
                            continue;
                        }*/
                        /*if (!follow_lalr1[tt.left].count(V_lalr1[k])) continue;
                        action_lalr1[i][y] = Content_lalr1 (1, tt.back);*/
                    }
            }
        }
    }
    // save to complete log
    fout_lalr1 << "【LALR(1)分析表】" << endl;
    fout_lalr1 << setw(10) << "|" << setw(5) << V_lalr1[0] << setw(5) << "|";
    for (int i = 1 ; i < V_lalr1.size() ; i++)
        fout_lalr1  << setw(5) << V_lalr1[i] << setw(5) << "|";
    fout_lalr1 << endl;
    for (int i = 0 ; i < (V_lalr1.size()+1)*10 ; i++)
        fout_lalr1  << "-";
    fout_lalr1 << endl;
    stringstream sin;
    for (int i = 0 ; i < collection_lalr1.size() ; i++)
    {
        if(collection_lalr1[i].flag==1){
            continue;
        }
        fout_lalr1 << setw(5) << collection_lalr1[i].newseq << setw(5) << "|";
        //spe++;
        for (int j = 0 ; j < V_lalr1.size() ; j++)
        {
            char ch = V_lalr1[j];
            if (isupper(ch))
            {
                if (Goto_lalr1[i][ch] == -1)
                    fout_lalr1 << setw(10) << "|";
                else
                    fout_lalr1 << setw(5) << Goto_lalr1[i][ch] << setw(5) << "|";
            }
            else
            {
                sin.clear();
                if (action_lalr1[i][ch].type == -1)
                    fout_lalr1 << setw(10) << "|";
                else
                {
                    Content_lalr1& temp = action_lalr1[i][ch];
                    if (temp.type == 0)
                        sin << "S";
                    if (temp.type == 1)
                        sin << "R";
                    if (temp.type == 2)
                        sin << "acc";
                    if (temp.num != -1)
                        sin << temp.num;
                    sin >> temp.out;
                    fout_lalr1 << setw(7) << temp.out.c_str() << setw(3) << "|";
                }
            }
        }
        fout_lalr1 << endl;
    }
    for (int i = 0 ; i < (V_lalr1.size()+1)*10 ; i++)
        fout_lalr1 << "-";
    fout_lalr1 << endl;

    // save to table log
    ftable_lalr1 << "判定为 LALR(1) 文法" << endl << endl;
    ftable_lalr1 << "【LALR(1)分析表】" << endl;
    ftable_lalr1 << setw(10) << "|" << setw(5) << V_lalr1[0] << setw(5) << "|";
    for (int i = 1 ; i < V_lalr1.size() ; i++)
        ftable_lalr1 << setw(5) << V_lalr1[i] << setw(5) << "|";
    ftable_lalr1 << endl;
    for (int i = 0 ; i < (V_lalr1.size()+1)*10 ; i++)
        ftable_lalr1 << "-";
    ftable_lalr1 << endl;
    stringstream sin2;
    for (int i = 0 ; i < collection_lalr1.size() ; i++)
    {
        if(collection_lalr1[i].flag==1){
            continue;
        }
        ftable_lalr1 << setw(5) << collection_lalr1[i].newseq << setw(5) << "|";
        for (int j = 0 ; j < V_lalr1.size() ; j++)
        {
            char ch = V_lalr1[j];
            if (isupper(ch))
            {
                if (Goto_lalr1[i][ch] == -1)
                    ftable_lalr1 << setw(10) << "|";
                else
                    ftable_lalr1 << setw(5) << Goto_lalr1[i][ch] << setw(5) << "|";
            }
            else
            {
                sin2.clear();
                if (action_lalr1[i][ch].type == -1)
                    ftable_lalr1 << setw(10) << "|";
                else
                {
                    Content_lalr1& temp = action_lalr1[i][ch];
                    if (temp.type == 0)
                        sin2 << "S";
                    if (temp.type == 1)
                        sin2 << "R";
                    if (temp.type == 2)
                        sin2 << "acc";
                    if (temp.num != -1)
                        sin2 << temp.num;
                    sin2 >> temp.out;
                    ftable_lalr1 << setw(7) << temp.out.c_str() << setw(3) << "|";
                }
            }
        }
        ftable_lalr1 << endl;
    }
    for (int i = 0 ; i < (V_lalr1.size()+1)*10 ; i++)
        ftable_lalr1 << "-";
    ftable_lalr1 << endl;
}
void dfs_lalr1 (const string& x)
{
    if (vis_lalr1[x]) return;
    vis_lalr1[x] = 1;
    vector<int>& id = VN_set_lalr1[x];
    for (int i = 0 ; i < id.size() ; i++)
    {
        string& left = wf_lalr1[id[i]].left;
        string& right = wf_lalr1[id[i]].right;
        for (int j = 0 ; j < right.length() ; j++)
            if (isupper(right[j]))
            {
                dfs_lalr1 (right.substr(j,1));
                set<char>& temp = first_lalr1[right.substr(j,1)];
                set<char>::iterator it = temp.begin();
                bool flag = true;
                for (; it != temp.end() ; it++)
                {
                    if (*it == '~') flag = false;
                    first_lalr1[left].insert (*it);
                }
                if (flag) break;
            }
            else
            {
                first_lalr1[left].insert (right[j]);
                break;
            }
    }
}
void append_lalr1 (const string& str1 , const string& str2)
{
    set<char>& from = follow_lalr1[str1];
    set<char>& to_lalr1 = follow_lalr1[str2];
    set<char>::iterator it = from.begin();
    for (; it != from.end() ; it++)
        to_lalr1.insert (*it);
}

bool _checkLALR1 (const vector<int>& id, const string str)
{

    for (int i = 0 ; i < id.size() ; i++)
    {
        int x = id[i];
        if (wf_lalr1[x].right == str) return true;
    }
    return false;
}
void make_first_lalr1 ()
{
    vis_lalr1.clear();
    map<string,vector<int> >::iterator it2 = dic_lalr1.begin();
    for (; it2 != dic_lalr1.end() ; it2++)
        if (vis_lalr1[it2->first]) continue;
        else dfs_lalr1 (it2->first);
    fout_lalr1 << "【FIRST集】" << endl;
    map<string,set<char> >::iterator it = first_lalr1.begin();
    for (; it != first_lalr1.end() ; it++)
    {
        fout_lalr1 << "FIRST(" << it->first.c_str() << ")={";
        set<char> & temp = it->second;
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for (; it1 != temp.end() ; it1++)
        {
            if (flag)
                fout_lalr1 << ",";
            fout_lalr1 << *it1;
            flag = true;
        }
        fout_lalr1 << "}" << endl;
    }
}
void make_follow_lalr1 ()
{
    while (true)
    {
        bool goon = false;
        map<string,vector<int> >::iterator it2 = VN_set_lalr1.begin();
        for (; it2 != VN_set_lalr1.end() ; it2++)
        {
            vector<int>& id = it2->second;
            for (int i = 0 ; i < id.size() ; i++)
            {
                bool flag = true;
                WF_lalr1& tt = wf_lalr1[id[i]];
                string& left = tt.left;
                const string& right = tt.right;
                for (int j = right.length()-1 ; j >= 0 ; j--)
                    if (isupper(right[j]))
                    {
                        if (flag)
                        {
                            int tx = follow_lalr1[right.substr(j,1)].size();
                            append_lalr1(left , right.substr(j,1));
                            int tx1 = follow_lalr1[right.substr(j,1)].size();
                            if (tx1 > tx) goon = true;
                            if (_checkLALR1 (id , "~"))
                                flag = false;
                        }
                        for (int k = j+1 ; k < right.length() ; k++)
                            if (isupper(right[k]))
                            {
                                string idd = right.substr(k,1);
                                set<char>& from = first_lalr1[idd];
                                set<char>& to_lalr1 = follow_lalr1[right.substr(j,1)];
                                set<char>::iterator it1 = from.begin();
                                int tx = follow_lalr1[right.substr(j,1)].size();
                                for (; it1 != from.end() ; it1++)
                                    if (*it1 != '~')
                                        to_lalr1.insert (*it1);
                                int tx1 = follow_lalr1[right.substr(j,1)].size();
                                if (tx1 > tx) goon = true;
                                if (_checkLALR1 (id , "~"))
                                    break;
                            }
                            else
                            {
                                int tx = follow_lalr1[right.substr(j,1)].size();
                                follow_lalr1[right.substr(j,1)].insert (right[k]);
                                int tx1 = follow_lalr1[right.substr(j,1)].size();
                                if (tx1 > tx) goon = true;
                                break;
                            }
                    }
                    else flag = false;
            }
        }
        if (!goon) break;
    }
    fout_lalr1 << "【FOLLOW集】" << endl;
    map<string,set<char> >::iterator it = follow_lalr1.begin();
    for (; it != follow_lalr1.end() ; it++)
    {
        fout_lalr1 << "FOLLOW(" << it->first.c_str() << ")={";
        set<char> & temp = it->second;
        //if (it->first[0] == 'S')
        temp.insert ('#');
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for (; it1 != temp.end() ; it1++)
        {
            if (flag)
                fout_lalr1 << ",";
            fout_lalr1 << *it1;
            flag = true;
        }
        fout_lalr1 << "}" << endl;
    }
}
//--------------------------------------------------------------------------------------------------
void print_lalr1 (string s1 , string s2 , string s3 , string s4 , string s5 , string s6 , string s7)
{
    fout_lalr1 << setw(15) << s1.c_str() << "|"
              << setw(15) << s2.c_str() << setw(15) << s3.c_str() << setw(20) << s4.c_str() << "|"
              << setw(15) << s5.c_str() << setw(15) << s6.c_str() << setw(15) << s7.c_str() << endl;
}
string get_steps_lalr1 (int x)
{
    stringstream sin;
    sin << x;
    string ret;
    sin >> ret;
    return ret;
}

template<class T>
string get_stk_lalr1 (vector<T> stk)
{
    stringstream sin;
    for (int i = 0 ; i < stk.size() ; i++)
        sin << stk[i];
    string ret;
    sin >> ret;
    return ret;
}

string get_shift_lalr1 (WF_lalr1& temp)
{
    stringstream sin;
    sin << "reduce(" << temp.left << "->" << temp.right <<")";
    string out;
    sin >> out;
    return out;
}
int get_rank(int rank_merged)
{
    for(int rank=0;rank<collection_lalr1.size();rank++)
    {
        if(rank_merged == collection_lalr1[rank].newseq)
        return rank;
    }
    return -1;
}
void analyse_lalr1 (string src)
{
    fout_lalr1 << "【过程分析】" << endl;
    print_lalr1 ("steps","op-stack" ,"input","operation","state-stack" , "ACTION" , "GOTO");
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
        int top = get_rank(st_stack[st_stack.size()-1]);
        //cout<< top<<endl;
        Content_lalr1& act = action_lalr1[top][u];
        if (act.type == 0)
        {
            print_lalr1 (get_steps_lalr1 (steps++) , get_stk_lalr1 (op_stack) , src.substr(i), "shift",  get_stk_lalr1(st_stack) , act.out , "");
            op_stack.push_back (u);
            st_stack.push_back (act.num);
            //cout << act.num<<endl;
        }
        else if (act.type == 1)
        {
            WF_lalr1& tt = wf_lalr1[act.num-1];
            int y = get_rank(st_stack[st_stack.size()-tt.right.length()-1]);
            cout<<y<<"  testing"<<endl;
            int x = Goto_lalr1[y][tt.left[0]];
            print_lalr1 (get_steps_lalr1 (steps++) , get_stk_lalr1 (op_stack) , src.substr(i) , get_shift_lalr1(tt) ,get_stk_lalr1(st_stack),act.out,get_steps_lalr1(x));
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
            print_lalr1 (get_steps_lalr1(steps++), get_stk_lalr1(op_stack) , src.substr(i) , "Accept" , get_stk_lalr1(st_stack) , act.out , "");
            is_accepted = true;
        }
        else continue;
    }
//    cout << "op_stack depth: " << op_stack.size() << endl;
//    cout << "st_stack depth: " << st_stack.size() << endl;
   //if (!is_accepted) throw "analyse failed!";
    //if (steps == 1)throw "analyse failed!";
}


int run_lalr1 (string fin_lalr1_path, string fout_lalr1_path, string input)
{
    freopen("1.in","r",stdin);
    fin_lalr1.open(fin_lalr1_path);
    fout_lalr1.open(fout_lalr1_path);
    ftable_lalr1.open("log/temp_table_lalr1");
    int n;
    int result_indicator2 = 0;
    char s[MAX];
    try {
        fin_lalr1>>n;
        fout_lalr1 << "【启动 LALR(1)分析】" << endl;
        fout_lalr1 << "【输入文法】" << endl;
        for (int i = 0 ; i < n ; i++)
        {
            fin_lalr1>>s;
            int len = strlen(s),j;
            for (j = 0 ; j < len ; j++)
                if (s[j] == '-') break;
            s[j] = 0;
            wf_lalr1.push_back (WF_lalr1 (s , s+j+2 ,-1 , -1));
        }
        //cout<<"输入文法"<<endl;
        for(int i=0;i<n;++i){
            wf_lalr1[i].print_lalr1();
        }
        printf("\n");
        make_item_lalr1();
        make_first_lalr1();
        make_follow_lalr1();
        get_lalr1_items();
        //判断冲突
        get_merge_items();//合并同心项集
        print_items2();
        print_edge2();
        make_V_lalr1();
        make_table_lalr1(result_indicator2);
        if(input == string("default"));
        else analyse_lalr1 (input);
    } catch (exception) {
        result_indicator2 = 1;
    } catch (const char* msg) {
        cout << msg << endl;
        result_indicator2 = 1;
    }
    //fout_lalr1<<result_indicator2<<endl;
    fin_lalr1.close();
    fout_lalr1.close();
    ftable_lalr1.close();
    clear_lalr1();
    //printf("%d", result_indicator2);
    return result_indicator2;
}



