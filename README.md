[![DOI](https://zenodo.org/badge/19239/dylanstorey/SubSampleFastq.svg)](https://zenodo.org/badge/latestdoi/19239/dylanstorey/SubSampleFastq)
#README
A simple and straight forward program that randomly sub samples a fastq
file. Written in C++. 

Uses a Fischer-Yates Shuffle to determine which reads it should sample from prior to reading
through any files. 

Uses the kseq.h header file by hl3 (http://lh3lh3.users.sourceforge.net/parsefastq.shtml).

This program randomly sub samples a fastq file:
    Files need to be in the fastq file and can be gzip compressed
    
    Compile with make. 
    
    Usage ./RandomSubFq [optionsp <file1> .. <filen>
      -t can be set with the total number of fastq records in the file (prefered as it keeps from looping over the file twice)
      -w needs to be set to the total number of records you want from the file (must be less than total records)
      -o sets a specific prefix to output files
      -s sets the seed 
      

If you pass more than one file to this program at a time it will pull the same records from each file. This is intended behavior for paired end reads.

This program assumes all files provided are of the same size and that paired end reads are in the same order.
	

