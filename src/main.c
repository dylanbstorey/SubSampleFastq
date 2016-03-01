/*
 * RandomSubFq
 * 
 * Copyright 2013 Dylan Storey <dylan.storey@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include "kseq.h"
#include <zlib.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>

using namespace std;

KSEQ_INIT(gzFile, gzread) 


void usage();

int main (int argc, char  **argv){

	// declarations //
	gzFile in;  
    kseq_t *record;  
    int l;  
	int total_records = 0;
	int* records;
    int* shuffled;
    int total_needed = 0;
    string prefix = "";
    int seed = time(NULL);
    
    
    char *file_s[2] = {0,0}; //empty array to hold file name(s)
    int file_n = 0 ;// tracking number of files in;
    
    char *file_o[2] ={0,0}; //empty array for output file names

    
    char c;

    while (	(c = getopt (argc, argv, "-t:w:o:")) != -1) {
		switch(c){
			case '\1' : // text 
				file_s[file_n] = optarg;
				file_n++;
				break;
			case 't' :
				total_records = atoi(optarg);
				break;
			case 'w':
				total_needed = atoi(optarg);
				break;
			case 'h':
				usage();
				return -1;
			case 'o':
				prefix = optarg;
				break;
			case 's':
				seed = atoi(optarg);
				break;
			}
		
		}
    
    
    
    if (total_needed == 0){
		cerr << "Surely you need more than zero records from this file !" << endl;
		usage();
		return -1;
		}
		
	if (file_n == 0){
		cerr << "It doesn't look like you provided any files !" << endl;
		usage();
		return -1;
		}
	
    //get the total number of reads if needed
	if (total_records == 0){
		cerr << "The number of records in the file were not provided , counting records may be time consuming !" << endl;
		in = gzopen(file_s[0], "r"); 
		record = kseq_init(in); // initialize kseq object
		while ((l = kseq_read(record)) >= 0) {
			++total_records;
			}
	
		kseq_destroy(record); // destroy the record  
		gzclose(in); // close file  
		cerr << "Total Records in File: " << total_records << endl;
		}
    
    if (total_needed >= total_records){
		cerr << "The number of records requested either matches or exceeds the number of records provided" << endl;
		usage();
		return -1;
		}
      
    
    // initialize array of ints//
    records = new int[total_records];
  	for (l=0;l<total_records ; ++l){
		records[l] = l;
		}


  	srand (seed);
  	// Yates shuffle (random enough for us) //
	  	for (l = total_records-1; l > 0 ; l--){
			int random = rand() % l; // get a random number between 0 and current max
			int temp = records[random]; // get the value of random array index
			records[random] = records[l]; // set random index to current max record
			records[l] = temp; // set current max to random
			}
		
	

	//file extension addition
	for( int n =0; n <file_n ;++n){
		
		// append our output file name //
		string base = file_s[n];
		base = prefix+base+".rand";
		file_o[n] = strdup(base.c_str());		
		// open output file stream //
		ofstream file_out;
		file_out.open(file_o[n]);
	
		//open our first file//
	    in = gzopen(file_s[0], "r");  //open file
	    record = kseq_init(in); // initialize kseq object
		int current_record_number = 0;  
	    while ((l = kseq_read(record)) >= 0) { // iterate over the file
			if (records[current_record_number] < total_needed){
				if (record->comment.l){
					file_out << "@" << record->name.s << " " << record->comment.s << endl;  
					}
				else {
					file_out << "@" << record->name.s << endl;
					}
				file_out << record->seq.s << endl;
	
				if (record->qual.l) {
					file_out << "+" << endl;
					file_out << record->qual.s << endl;  
					}	
				}
		current_record_number++;
		}
	    
	
	    kseq_destroy(record); // destroy the record  
	    gzclose(in); // close incoming file
	    file_out.close(); // close the output file
    
    }
    return 0;
}

void usage(){
	cerr << endl <<"This program randomly sub samples a fastq file" << endl << endl;
	cerr << "Usage ./RandomSubFq [options] <file1> .. <filen>" <<endl;
	cerr << "Files need to be in the fastq file and can be gzip compressed" << endl;
	cerr << "\t-t can be set with the total number of fastq records in the file (prefered as it keeps from looping over the file twice)." << endl;
	cerr << "\t-w needs to be set to the total number of records you want from the file (must be less than total records)." << endl<<endl;
	cerr << "\t-o output with a specific prefix"<<endl;
	cerr << "\t-s sets the seed for RNG";
	cerr << "If you pass more than one file to this program at a time it will pull the same records from each file. This is intended behavior for paired end reads." << endl;
	cerr << "This program assumes all files provided are of the same size and that paired end reads are in the same order." <<endl << endl;
	
	}


