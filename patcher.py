#!/usr/bin/python3
# 
#  patch.py 
#
#  (c)2025 bferguson3
#     @retrodevdiscord
#

import os,sys 


if(len(sys.argv) < 3):
    print("Usage:\n $ python3 patch.py <base-file> <patch-file>.ips")
    sys.exit()

f = open(sys.argv[1], "rb")
base = f.read()
f.close()

f = open(sys.argv[2], "rb")
ips = f.read()
f.close()

_hdr = ips[:5]
if _hdr != b'PATCH':
    print("Not a valid IPS file!")
    sys.exit()

bc = 5

class patch:
    def __init__(self):
        self.offset = 0
        self.length = 0
        self.bytes = []
    ###
###
patches=[]
while ips[bc:bc+3] != b'EOF':
    p = patch()
    p.offset = (ips[bc] << 16)|(ips[bc+1] << 8)|(ips[bc+2]) 
    bc += 3
    p.length = (ips[bc] << 8)|ips[bc+1]
    bc += 2
    if(p.length == 0):
        # this is RLE 
        rlesz = (ips[bc]<<8)|(ips[bc+1])
        n = 0
        while n < rlesz:
            p.bytes.append(ips[bc+2])
            n += 1
        bc += 3
    else:
        p.bytes = ips[bc:bc+p.length]
        bc += p.length 
    patches.append(p)

for p in patches:
    base = base[:p.offset] + p.bytes + base[p.offset+p.length:]

f = open("out.bin", "wb")
f.write(base)
f.close()
print("out.bin written.")