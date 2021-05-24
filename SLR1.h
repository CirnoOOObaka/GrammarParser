#ifndef SLR1_H
#define SLR1_H

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cctype>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <iomanip>

#define MAX 507

using namespace std;

class WF_slr1;
class Closure_slr1;
struct Content_slr1;

void make_item_slr1 ();
void print_item_slr1();
void dfs_slr1 (const string& x);
void make_first_slr1 ();
void append_slr1 (const string& str1 , const string& str2);
bool _check (const vector<int>& id, const string str);
void make_follow_slr1 ();
void make_set_slr1 ();
void make_V_slr1 ();
void make_cmp_slr1 (vector<WF_slr1>& cmp1 , int i  , char ch);
void make_go_slr1 ();
void make_table_slr1 ();
void print_slr1 (string s1 , string s2 , string s3 , string s4 , string s5 , string s6 , string s7);
string get_steps_slr1 (int x);
template<class T> string get_stk_slr1 (vector<T> stk);
string get_shift_slr1 (WF_slr1& temp);
void analyse_slr1 (string src);
void clear_slr1 ();
int run_slr1 (string fin_slr1_path, string fout_slr1_path);
int get_first_set_slr1 (string fin_slr1_path, string fout_slr1_path);
int get_follow_set_slr1 (string fin_slr1_path, string fout_slr1_path);

#endif // SLR1_H
