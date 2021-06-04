# -*- coding: utf-8 -*-
"""
Created on Thu Jun  3 19:02:57 2021

@author: Dell
"""

from graphviz import Digraph
import sys
with open(r"log/temp_out.txt" ,"r",encoding='utf-8') as f:
    str = f.read()
    f.close()
s1=str[str.find("【EDGE】")+7:str.find("【L")-1]
#print(s1)
lists1= s1.split('\n')  #转换函数

'''s2=str[str.find("【CLOSURE】")+10:str.find("【EDGE】")-2]
print(s2)
lists2= s2.split('closure-')'''

dot = Digraph(comment='The Test', format="png")
'''
for i in range(1,len(lists2)):
    m=lists2[i]
    m1=m[:m.find("\n")]
    dot.node(lists2[i],lists2[i],shape='circle')
'''
for i in range(len(lists1)):
    dot.edge(lists1[i].split('->')[0], lists1[i].split('->')[2], lists1[i].split('->')[1])
#dot.view()
dot.render('fa_py/output-picture.gv',format='png', view=False)
sys.exit()