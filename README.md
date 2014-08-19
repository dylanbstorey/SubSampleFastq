#README
A simple and straight forward program that randomly sub samples a fastq
file. Written in C++. 

Uses a Fischer-Yates Shuffle to determine which reads it should sample from prior to reading
through any files. 

Uses the kseq.h header file by hl3 (http://lh3lh3.users.sourceforge.net/parsefastq.shtml).

