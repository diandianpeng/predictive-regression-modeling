#!/usr/bin/env python2.7

import os,sys

output = 'test1.eps'

cmd = '''
psxy residual-dynamic_cratons.txt -Sc0.1 -Gblue -JQ25 -B20 -Rg -K > %(output)s
pscoast -J -R -N1 -W1p -K -O >> %(output)s
''' % vars()
print cmd
#os.system(cmd)

output = 'test2.eps'

cmd = '''
awk '{print $4,$3}' residual-dynamic_cratons.txt | psxy -R20/310/-3.4/1.2 -JX15/10 -Sc0.1  -Gblue -B40/1 -K > %(output)s
''' % vars()
print cmd
os.system(cmd)


