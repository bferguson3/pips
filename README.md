# pIPS v0.1

I got tired of not having an open source, multi-platform IPS tool, so I made one. Enjoy.

## patcher.c / patcher.py

Utilize an IPS patch file (RLE supported) to patch a file.<br>
Produces "out.bin"

## makeips.c / makeips.py

Create an IPS file from two input files. (Currently does not RLE compress)<br>
Produces "out.ips"

## checksum.c / checksum.py

Calculates 16-bit checksum by adding every byte (8 bits) in a file. (Super NES/Super Famicom checksum)
