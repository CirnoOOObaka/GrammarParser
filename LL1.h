#ifndef LL1_H
#define LL1_H

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
#include <fstream>
#include <iomanip>
#define MAX 507

using namespace std;

class WF_ll1;

void dfs_ll1(int x);
void make_first_ll1();
void append_ll1(const string& str1 , const string& str2);
void make_follow_ll1();
bool check_first_ll1(const string& text , char ch);
bool check_follow_ll1(const string& text , char ch);
void make_table_ll1();
void print_ll1(int steps , stack<string> stk , string src , string wf , int x);
void analyse_ll1(const string& src);
void clear_ll1();
int run_ll1(string fin_ll1_path, string fout_ll1_path);
int get_first_set_ll1(string fin_ll1_path, string fout_ll1_path);
int get_follow_set_ll1(string fin_ll1_path, string fout_ll1_path);

#endif // LL1_H
