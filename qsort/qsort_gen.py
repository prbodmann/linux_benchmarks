from random import randint
from math import sqrt
import struct
import sys
MAXINT = 2000000000
MININT = 1000000
MAXARRAY = int(sys.argv[1])
list=[]
with open("qsort_input_"+str(MAXARRAY)+".bin","wb") as fd:
    for i in range(MAXARRAY):
        a = randint(MININT, MAXINT)
        b = randint(MININT, MAXINT)
        c = randint(MININT, MAXINT)
        result = sqrt(pow(a,2)+pow(b,2)+pow(c,2))
        list.append(result)
        s = struct.pack('d', result)
        fd.write(s)
list.sort()
with open("qsort_gold_"+str(MAXARRAY)+".bin","wb") as out:
    for i in list:
        s = struct.pack('d', i)
        out.write(s)
