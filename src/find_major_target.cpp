//--------------------------------------------------------------------
// find_major_target.cpp
// Copyright 2018
// Written by Jang-il Sohn (sohnjangil@gmail.com)
// Bioinformatic and Genomics Lab., Hanyang University, Seoul, Korea
// Released under the GPL license
//--------------------------------------------------------------------


#include <string>
#include <map>
#include <fstream>
#include <iostream>

using namespace std;

int main ( int argc , char ** argv ){
  if ( argc != 2 ){
    cout << "find_major_target  map2target > output" << endl;
    return 0;
  }

  string tmp;
  string contig;
  string chromo;
  size_t map_length;

  map < string , map < string , size_t > > contig_chromo_leng;

  ifstream fin ( argv [1] );
  getline ( fin , tmp );
  while ( fin >> tmp >> contig >> chromo >> tmp >> map_length >> tmp >> tmp ){
    contig_chromo_leng[contig][chromo]=map_length;
  }
  fin.close();

  for ( auto & i : contig_chromo_leng ){
    size_t max = 0 ;
    for ( auto & j : i.second ){
      if ( j.second > max ){
	chromo = j.first;
	max = j.second;
      }
    }
    cout << i.first << "\t" << chromo << endl;
  }

  return 0;
}
