//-----------------------------------------------
// Copyright 2015 BIG Lab, HYU, Seoul, Korea
// Written by Jang-il Sohn
// (sohnjangil@gmail.com or physicon@korea.ac.kr)
// Released under the GPL
//-----------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <algorithm>
#include <queue>
//#include <time.h>
//#include <cstdlib>


using namespace std;


int read_fasta(string infile,   map < string , string > & Seq){

  ifstream fin(infile.c_str());
  
  if( !(fin) ){
    cout << "There are no " << infile << endl;
    return 0;
  }

  string id, seq, tmp;
  size_t found;
  
  getline(fin,tmp);
  id = tmp.substr(1);
  
  while( getline(fin, tmp) ){
    if (tmp[0]=='>'){
      Seq[id] = seq ;
      found=tmp.find("\t");
      id = tmp.substr(1,found-1);
      seq.clear();
    }
    else{
      seq += tmp ;
    }
  }
  Seq[id] = seq ;
  fin.close();
  return 1;
}




int read_fasta(string infile,   map < string , string > & Seq, vector < string > & id_vec){

  ifstream fin(infile.c_str());
  
  if( !(fin) ){
    cout << "There are no " << infile << endl;
    return 0;
  }

  string id, seq, tmp;
  size_t found;
  
  getline(fin,tmp);
  id = tmp.substr(1);
  
  while( getline(fin, tmp) ){
    if (tmp[0]=='>'){
      Seq[id] = seq ;
      id_vec.push_back(id);
      found=tmp.find("\t");
      id = tmp.substr(1,found-1);
      seq.clear();
    }
    else{
      seq += tmp ;
    }
  }
  Seq[id] = seq ;
  id_vec.push_back(id);
  fin.close();
  return 1;
}





void hello(){
  cout << endl
       << "Usage: fasta_divider  fasta_file  number_of_file"
       << endl << endl
       << "\tThe fasta file is divided by number_of_file."
       << endl << endl
       << "\tex) fasta_seq_distribute  test.fa[sta]  100"
       << endl
       << "\t==> test.fa will be divided by 100 to 100 files named as test.fa.1, test.fa.2,... , test.fa.100"
       << endl << endl
       << "Bug-report to" << endl
       << "\tJang-il Sohn" << endl
       << "\tBIG Lab., Hanyang University"<< endl
       << "\tSeoul Korea" << endl
       << "\tsohnjangil@gmail.com" << endl
       << endl
       << "Last update at Jun 27, 2017" << endl << endl ;
}



////////////////////////////////////////////////////////////////////
//
// strat MAIN
//
////////////////////////////////////////////////////////////////////

int main(int argc, char ** argv){

  if(argc != 3) {
    hello();
    return 0;
  }



  ////////////////////////////////////////////////////////////////////
  //
  //  read sequence file
  //
  
  string infile=argv[1];
  
  map < string , string > Seq;
  vector < string > id_vec;

  if( ! read_fasta(infile,Seq,id_vec) ){
    return 0;
  }

  ////////////////////////////////////////////////////////////////////
  //
  //  read parameter: length to cut
  //

  unsigned int dv=atoi(argv[2]);
  
  ofstream fout[dv];
  string outfile;
  for ( unsigned int i = 0 ; i < dv ; i ++){
    outfile=infile+"."+to_string(i+1);
    fout[i].open(outfile.c_str());
  }

  unsigned int fn;
  // unsigned int bn;
  // if ( id_vec.size()<dv ){
  //   bn=1;
  // }
  // else if ( id_vec.size()%dv == 0 ){
  //   bn=id_vec.size()/dv;
  // }else{
  //   bn=id_vec.size()/dv + 1;
  // }
  
  //cout << id_vec.size() << endl;
  //cout << bn << endl;

  for ( unsigned int i = 0 ; i < id_vec.size() ; i ++ ){
    // fn=i/bn;
    //cout << fn << endl;

    fn = i % dv ;
    cout << id_vec[i] << "\tto\t" << argv[1] << "." << fn + 1 << endl;
    
    fout[fn] << ">" << id_vec[i] << endl ;
    fout[fn] << Seq[id_vec[i]] << endl ;
  }
  
  for ( unsigned int i = 0 ; i < dv ; i++ ) fout[i].close();
  
  return 0;
}
