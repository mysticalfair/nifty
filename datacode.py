import sys
import string
import random
import re

#name=sys.argv[1]

def refine(name):
    with open(name, "r") as f:
        lines=f.readlines()
    a=''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(5))
    filename="/path/to/file" %(a)
    shorter="codettes/%s.txt" %(a)
    filea=open(filename, "w")

    for line in lines:
       
        if "HandleValueNotification" in line:
            filea.write(line.split("HandleValueNotification on handle 000D with value " ,1) [1])
    filea.close()
    return shorter
    

sys.stdout.write(refine(sys.argv[1]))
