#ifndef LR0_H
#define LR0_H

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cctype>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <QString>
#define MAX 507

using namespace std;

class WF_lr0;
class Closure_lr0;
struct Content_lr0;

void make_item_lr0();
void make_set_lr0();
void make_V_lr0();
void make_cmp_lr0(vector<WF_lr0>& cmp1 , int i  , char ch);
void make_go_lr0();
void make_table_lr0();
void print_lr0(string s1 , string s2 , string s3 , string s4 , string s5 , string s6 , string s7);
string get_steps_lr0(int x);
template<class T> string get_stk_lr0(vector<T> stk);
string get_shift_lr0(WF_lr0& temp);
void analyse_lr0(string src);
void clear_lr0();
int run_lr0(string fin_lr0_path, string fout_lr0_path,string input);

#endif // LR0_H
