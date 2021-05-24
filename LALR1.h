#ifndef LALR1_H
#define LALR1_H
#include <iostream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <string>
#include <cctype>
#include <stack>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <iomanip>
using namespace std;
// 产生式类
//const int MAX=509;

class WF_lalr1;
class Closure_lalr1;
struct Content_lalr1;

void clear_lalr1();
void make_item_lalr1();
void get_lalr1_items();
//void print_edge();
//void print_items();
void print_items2();
void print_edge2();
Closure_lalr1 get_lalr1_closure(Closure_lalr1 I);
Closure_lalr1 Go_lalr1(Closure_lalr1 I, char X);

void make_V_lalr1 ();
int Closure_equal(Closure_lalr1 I,Closure_lalr1 J);
void get_merge_items();

void make_table_lalr1 ();
void dfs_lalr1 (const string& x);
void append_lalr1 (const string& str1 , const string& str2);
bool _checkLALR1 (const vector<int>& id, const string str);
void make_first_lalr1 ();
void make_follow_lalr1 ();
//void analyse_lalr1 (string src);
int run_lalr1(string fin_lalr1_path, string fout_lalr1_path);
#endif // LALR1_H
