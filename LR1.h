#ifndef LR1_H
#define LR1_H

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

class WF_lr1;
class Closure_lr1;
struct Content_lr1;

void clear_lr1();
void make_item_lr1 ();
void get_items();
void print_edge();
void make_V_lr1 ();
void make_table_lr1 ();
void dfs_lr1 (const string& x);
void append_lr1 (const string& str1 , const string& str2);
bool _checkLR1 (const vector<int>& id, const string str);
void make_first_lr1 ();
void make_follow_lr1 ();
void analyse_lr1 (string src);
int run_lr1andslr1(string fin_lr1_path, string fout_lr1_path);

#endif // LR1_H
