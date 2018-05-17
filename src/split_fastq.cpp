//--------------------------------------------------------------------
// split_fastq.cpp
// Copyright 2018
// Written by Jang-il Sohn (sohnjangil@gmail.com)
// Bioinformatic and Genomics Lab., Hanyang University, Seoul, Korea
// Released under the GPL license
//--------------------------------------------------------------------


#include <string>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

int main ( int argc , char ** argv ) {

  if ( argc != 6 ){
    cout << "split_fastq  file.fastq  read_assembly.map  assembly_chr.map  target_list  prefix" << endl;
    return 0;
  }

  string id;
  string seq;
  string desc;
  string qual;

  string assembly;
  string chr;

  // map < string , string > read_assembly;
  map < string , set < string> > read_assembly;
  map < string , string > assembly_chr;

  set < string > chr_set;

  ifstream fin ( argv [3] ) ;
  while ( fin >> assembly >> chr ) {
    assembly_chr[assembly]=chr;
  }
  fin.close();

  string msg;
  msg =  "Reading: ";
  msg += argv[3];
  perror(msg.c_str());


  fin.open ( argv[2] );
  while ( fin >> id >> assembly ) {
    read_assembly[id].insert(assembly);
  }
  fin.close();

  msg =  "Reading: ";
  msg += argv[2];
  perror(msg.c_str());


  fin.open(argv[4]);
  while ( fin >> chr ){
    chr_set.insert(chr);
  }
  chr_set.insert("unmapped");
  fin.close();

  msg =  "Reading: ";
  msg += argv[4];
  perror(msg.c_str());

  map < string , string > outfile;
  string prefix=argv[5];
  for ( auto & i : chr_set ){
    outfile[i] = prefix + "." + i + ".fastq";
  }

  map < string , ofstream > fout;
  for ( auto & i : chr_set ){
    fout[i].open(outfile[i].c_str());
  }



  msg =  "Making target files with prefix: ";
  msg += argv[5];
  perror(msg.c_str());



  string tmp;
  fin.open(argv[1]);
  while ( fin >> id >> tmp >> seq >> desc >> qual ){
    id = id.substr(1);
    for ( auto & i : read_assembly[id] ){
      chr = assembly_chr [i];
      if ( chr_set.find(chr) == chr_set.end() ) chr = "unmapped";
      fout[chr] << "@" << id << " " << tmp << endl
		<< seq << endl
		<<desc << endl
		<< qual << endl;
    }
  }  
  fin.close();
  

  msg="Writing files" ;
  perror(msg.c_str());

  


  return 0;
}
