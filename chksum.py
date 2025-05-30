#!/usr/bin/python3
#
# chksum.py
#
# (c)2025 bferguson3
#    @retrodevdiscord
#

import os,sys

if len(sys.argv) < 2:
	print("Usage:\n $ python3 chksum.py <file>")

f = open(sys.argv[1], "rb")
inby = f.read()
f.close()

chksum = 0x0000

i = 0
while i < len(inby):
	chksum += inby[i]
	if chksum > 0xffff:
		chksum %= 0x10000
	i += 1

print("Checksum:",hex(chksum),"("+str(chksum)+")")
