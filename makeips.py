#!/usr/bin/python3
# 
# makeips.py
#
# (c)2025 bferguson3
#    @retrodevdiscord
#

import os,sys

if(len(sys.argv) < 3):
    print("Usage:\n $ python3 makeips.py <base-file> <compared-file>")
    sys.exit()

f = open(sys.argv[1], "rb")
base = f.read()
f.close()

f = open(sys.argv[2], "rb")
cmp = f.read()
f.close()

changes=[]
i = 0
if(len(base) < len(cmp)): # file 1 is bigger 
    while i < len(base):
        if(base[i] != cmp[i]):
            ofs = i
            o = []
            while(base[i] != cmp[i]):
                o.append(cmp[i])
                i += 1
            changes.append((ofs, len(o), o))
        i += 1
    add=[]
    while i < len(cmp):
        add.append(cmp[i])
        i += 1
    changes.append((ofs, len(add), add))
else: # file 2 is bigger 
    while i < len(cmp):
        if(base[i] != cmp[i]):
            ofs = i
            o = []
            while(base[i] != cmp[i]):
                o.append(base[i])
                i += 1
            changes.append((ofs, len(o), o))
        i += 1
    add=[]
    while i < len(base):
        add.append(base[i])
        i += 1
    changes.append((ofs, len(add), add))

i = 0
tot = 0
while i < len(changes):
    tot += changes[i][1]
    i += 1

print(len(changes),"changes found, totaling", tot,"bytes")

ips = []
ips.append(ord('P'))
ips.append(ord('A'))
ips.append(ord('T'))
ips.append(ord('C'))
ips.append(ord('H'))
for c in changes:
    # c[0] offset 3
    # c[1] length 2
    # c[2] bytes  n 
    ips.append((c[0] & 0xff0000) >> 16)
    ips.append((c[0] & 0xff00) >> 8)
    ips.append((c[0] & 0xff))
    ips.append((c[1] & 0xff00) >> 8)
    ips.append((c[1] & 0xff))
    for b in c[2]:
        ips.append(b)
ips.append(ord('E'))
ips.append(ord('O'))
ips.append(ord('F'))

f = open("out.ips", "wb")
f.write(bytes(ips))
f.close()

print("out.ips IPS file written.")