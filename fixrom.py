#!/usr/bin/python3
#
#  fixrom.py
#
#  (c)2025 bferguson3
#     @retrodevdiscord
#

import os,sys

if len(sys.argv) < 2: # not enough arguments
    print("Usage:\n $ python3 fixrom.py <rom-file>.sfc")

# load rom bytes #
f = open(sys.argv[1], "rb")
inby = f.read()
f.close()

# try each ROM header location... 
print("Checking header sanity...")
found = False
# lowrom?
romtype = 0
i = 0x7fc0
while i < 0x7fc0+21:
    if(inby[i] < 0x20)or(inby[i]>0x7f):
        print("Not LoROM...")
        break
    i += 1
if i == 0x7fc0+21:
    # LoROM!
    print("Its LoROM.")
    hdr = inby[0x7fc0:0x8000]
    found = True
###

# hirom?
if found == False:
    i = 0xffc0
    while i < 0xffc0 + 21:
        if(inby[i] < 0x20)or(inby[i]>0x7f):
            print("Not HiROM...")
            break 
        i += 1
if(i == 0xffc0+21):
    print("Its HiROM.")
    found = True
    romtype = 1
    hdr = inby[0xffc0:0x10000]
###

# exrom?
if found == False:
    i = 0x40ffc0
    while i < 0x40ffc0 + 21:
        if(inby[i] < 0x20)or(inby[i]>0x7f):
            print("Not ExROM...")
            break
        i += 1
if(i == 0x40ffc0 + 21):
    print("Its ExROM.")
    found = True 
    romtype = 2
    hdr = inby[0x40ffc0:0x410000]
###

# nullify compliment and chksum in header 
hdr = hdr[:29] + b'\xff\xff\x00\x00' + hdr[33:]

if found == False:
    print("No valid header found. Exiting...")
    sys.exit()

# get 16-bit checksum of all 8-bit values
sum = 0x0000
i = 0
while i < len(inby):
    sum += inby[i]
    if sum > 0xffff:
        sum %= 0x10000
    i += 1
print("Checksum:",hex(sum),"("+str(sum)+")")

# replace chksums in header 
hdr = hdr[:29] + bytes([(sum ^ 0xffff) & 0xff, ((sum ^ 0xffff) & 0xff00) >> 8]) + bytes([(sum & 0xff),(sum&0xff00)>>8]) + hdr[33:]

# replace header in ROM bytes 
if(romtype == 0):
    inby = inby[:0x7fc0] + hdr + inby[0x8000:]
elif(romtype == 1):
    inby = inby[:0xffc0] + hdr + inby[0x10000:]
else:
    inby = inby[:0x40ffc0] + hdr + inby[0x410000:]
print("Header patched.")

# write final file 
f = open("out.bin", "wb")
f.write(inby)
f.close()

print("File out.bin written OK.")