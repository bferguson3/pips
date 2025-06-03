# pIPS v0.2

I got tired of not having a tiny, open source, multi-platform IPS command-line tool, so I made one. Enjoy.

## patcher.c / patcher.py

Utilize an IPS patch file (RLE supported) to patch a file.<br>
Produces "out.bin"

## makeips.c / makeips.py

Create an IPS file from two input files. (Currently does not RLE compress)<br>
Produces "out.ips"

## makebips.c (new to v0.2)

Makes a BIPS file from two input files.<br>
Format is identical to IPS, but offset and length for each patch is 4 bytes long to support up to 4GB files.<br>
Warning: Loads entire file into memory!

## chksum.c 

Calculates 16-bit checksum by adding every byte (8 bits) in a file. 

## fixrom.py 

Searches for a valid ROM header, calculates valid SNES/SFC checksum and patches it.
