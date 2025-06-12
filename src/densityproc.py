#!/usr/bin/env python2.7

import os

cmd='''gcc cratonvalue.cpp -lm -o cratonjudge.o
       gcc cloud_cratonvalue.cpp -lm -o cloud.o'''
os.system(cmd)

#exe='cratonjudge.o'
exe='cloud.o'

rdn='rdc_dyn12_slab2_the_figure.xyz'
 
polynn='coor_cratons.xy'

outputn='residual-dynamic_cratons.txt'

labn='LAB_Davies.xyz'

for k in range(1):
    
    cmd='./%s %s %s %s %s' %(exe,rdn,polynn,outputn,labn)
    os.system(cmd)

