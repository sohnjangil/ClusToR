//--------------------------------------------------------------------
// split_scaffold.cpp
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

using namespace std;

int main ( int argc , char ** argv ) {
  if ( argc != 5 ){
    cout << "split_scaffold  assembly.fasta  assembly_chr.map  target_list  prefix" << endl;
    return 0;
  }
  
  string chr;
  string id;
  string seq;
  string tmp;
  
  map < string , string > assembly_chr;
  set < string > chr_set;
  
  ifstream fin ( argv [2] ) ;
  while ( fin >> id >> chr ) {
    assembly_chr[id]=chr;
  }
  fin.close();

  fin.open(argv[3]);
  while ( fin >> chr ){
    chr_set.insert(chr);
  }
  chr_set.insert("unmapped");
  fin.close();
  

  map < string , string > outfile;
  string prefix = argv[4];
  for ( auto & i : chr_set ){
    outfile[i]=prefix + "." + i + ".fasta";
  }

  
  map < string , ofstream > fout;
  for ( auto & i : chr_set ){
    cout << i << endl;
    fout[i].open(outfile[i].c_str());
  }


  string output;
  
  fin.open(argv[1]);
  getline(fin,tmp);
  id=tmp.substr(1);
  while ( getline(fin,tmp) ){
    if ( tmp[0]=='>' ){
      output = ">" + id + "\n" + seq + "\n";
      chr = assembly_chr[id];

      if ( chr_set.find(chr) == chr_set.end() ) chr = "unmapped";
      fout[chr] << output << endl;
      
      id = tmp.substr(1);
      seq.clear();
    }
    else{
      seq+=tmp;
    }
  }
  output = ">" + id + "\n" + seq + "\n";
  chr = assembly_chr[id];
  if ( chr_set.find(chr) == chr_set.end() ) chr = "unmapped";
  fout[chr] << output << endl;
      

  
  fin.close();

  for ( auto & i : chr_set ){
    fout[i].close();
  }
  
  return 0;
}
