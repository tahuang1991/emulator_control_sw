
#include "CSCConstants.h"
//#include <random>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <bitset>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <vector>


//root head file
#include "TTree.h"
#include "TROOT.h"
#include "TFile.h"

#define RAMPAGE_SIZE 4*1024 

struct CLCT{
    unsigned int bx;
    unsigned int keystrip;
    unsigned int localhs;
    unsigned int dcfeb;
    unsigned int pattern;
    unsigned int nhits;
    unsigned int clcthex;
    void initCLCT();
    void initCLCT(unsigned int, unsigned int,unsigned int,unsigned int,unsigned int,unsigned int);

};

void CLCT::initCLCT(){
    bx=0;
    keystrip=0;
    localhs=0;
    dcfeb=0;
    pattern=0;
    nhits=0;
    clcthex=0;
    
}



template <size_t XS_per_CFEB> int GetOutputXStrip(int cfeb, int input_xstrip, int tmb_compile_type) {
  const int Bad_HS = -1;
  const int Min_CFEB_in_non_me11 = 0; 
  const int Max_CFEB_in_non_me11 = 4; 
  const int N_CFEB_in_non_me11 = Max_CFEB_in_non_me11 - Min_CFEB_in_non_me11 + 1; 
  const int Min_XS_in_non_me11 = 0; 
  const int Max_XS_in_non_me11 = N_CFEB_in_non_me11*XS_per_CFEB - 1; 
  const int Min_CFEB_in_region_a = 0; 
  const int Max_CFEB_in_region_a = 3; 
  const int Min_XS_in_region_a = Min_CFEB_in_region_a*XS_per_CFEB;
  const int Max_XS_in_region_a = (Max_CFEB_in_region_a + 1)*XS_per_CFEB - 1; 
  const int Min_CFEB_in_region_b = 4; 
  const int Max_CFEB_in_region_b = 6; 
  const int Min_XS_in_region_b = Min_CFEB_in_region_b*XS_per_CFEB;
  const int Max_XS_in_region_b = (Max_CFEB_in_region_b + 1)*XS_per_CFEB - 1; 
  int output_xs = Bad_HS;
  if(tmb_compile_type == 0xa) {
    if(cfeb >= Min_CFEB_in_non_me11 && cfeb <= Max_CFEB_in_non_me11)
      output_xs = Min_XS_in_non_me11 + (XS_per_CFEB*cfeb + input_xstrip);
    if(!(output_xs >= Min_XS_in_non_me11 && output_xs <= Max_XS_in_non_me11))
      output_xs = Bad_HS;
  }
  else if(tmb_compile_type == 0xb) {
    if(cfeb >= Min_CFEB_in_non_me11 && cfeb <= Max_CFEB_in_non_me11)
      output_xs = Max_XS_in_non_me11 - (XS_per_CFEB*cfeb + input_xstrip); 
    if(!(output_xs >= Min_XS_in_non_me11 && output_xs <= Max_XS_in_non_me11))
      output_xs = Bad_HS;
  }
  else if(tmb_compile_type == 0xc) {
    if(cfeb >= Min_CFEB_in_region_a && cfeb <= Max_CFEB_in_region_a)
      output_xs = Min_XS_in_region_a + (XS_per_CFEB*(cfeb - Min_CFEB_in_region_a) + input_xstrip);
    else if(cfeb >= Min_CFEB_in_region_b && cfeb <= Max_CFEB_in_region_b)
      output_xs = Max_XS_in_region_b - (XS_per_CFEB*(cfeb - Min_CFEB_in_region_b) + input_xstrip);
    if(!(output_xs >= Min_XS_in_region_a && output_xs <= Max_XS_in_region_b))
      output_xs = Bad_HS;
  }
  else if(tmb_compile_type == 0xd) {
    if(cfeb >= Min_CFEB_in_region_a && cfeb <= Max_CFEB_in_region_a)
      output_xs = Max_XS_in_region_a - (XS_per_CFEB*(cfeb - Min_CFEB_in_region_a) + input_xstrip);
    else if(cfeb >= Min_CFEB_in_region_b && cfeb <= Max_CFEB_in_region_b)
      output_xs = Min_XS_in_region_b + (XS_per_CFEB*(cfeb - Min_CFEB_in_region_b) + input_xstrip);
    if(!(output_xs >= Min_XS_in_region_a && output_xs <= Max_XS_in_region_b))
      output_xs = Bad_HS;
  }
  
  return output_xs;
}





template <size_t XS_per_CFEB> int GetInputXStrip(int output_xstrip, int tmb_compile_type) {
  const int Min_CFEB_in_non_me11 = 0;
  const int Max_CFEB_in_non_me11 = 4;
  const int Min_CFEB_in_region_a = 0;
  const int Max_CFEB_in_region_a = 3;
  const int Min_CFEB_in_region_b = 4;
  const int Max_CFEB_in_region_b = 6;
  const int Max_XS_in_CFEB = XS_per_CFEB - 1;
  
  int region = output_xstrip / XS_per_CFEB;
  int phase = output_xstrip % XS_per_CFEB;
  int input_xs = -1;
  if(tmb_compile_type == 0xa) {
    if(region >= Min_CFEB_in_non_me11 && region <= Max_CFEB_in_non_me11)
      input_xs = phase;
  }
  else if(tmb_compile_type == 0xb) {
    if(region >= Min_CFEB_in_non_me11 && region <= Max_CFEB_in_non_me11)
      input_xs = Max_XS_in_CFEB - phase;
  }
  else if(tmb_compile_type == 0xc) {
    if(region >= Min_CFEB_in_region_a && region <= Max_CFEB_in_region_a) {
      input_xs = phase;
    }
    else if(region >= Min_CFEB_in_region_b && region <= Max_CFEB_in_region_b) {
      input_xs = Max_XS_in_CFEB - phase;
    }
  }
  else if(tmb_compile_type == 0xd) {
    if(region >= Min_CFEB_in_region_a && region <= Max_CFEB_in_region_a) {
      input_xs = Max_XS_in_CFEB - phase;
    }
    else if(region >= Min_CFEB_in_region_b && region <= Max_CFEB_in_region_b) {
      input_xs = phase;
    }
  }
  return input_xs;
}
    //
template <size_t XS_per_CFEB>  int GetInputCFEBByX(int output_xstrip, int tmb_compile_type) {
  const int Min_CFEB_in_non_me11 = 0;
  const int Max_CFEB_in_non_me11 = 4;
  const int Min_CFEB_in_region_a = 0;
  const int Max_CFEB_in_region_a = 3;
  const int Min_CFEB_in_region_b = 4;
  const int Max_CFEB_in_region_b = 6;
  
  int region = output_xstrip / XS_per_CFEB;
  int cfeb = -1;
  if(tmb_compile_type == 0xa) {
    if(region >= Min_CFEB_in_non_me11 && region <= Max_CFEB_in_non_me11)
      cfeb = region;
  }
  else if(tmb_compile_type == 0xb) {
    if(region >= Min_CFEB_in_non_me11 && region <= Max_CFEB_in_non_me11)
      cfeb = Max_CFEB_in_non_me11 - region;
  }
  else if(tmb_compile_type == 0xc) {
    if(region >= Min_CFEB_in_region_a && region <= Max_CFEB_in_region_a) {
      cfeb = region;
    }
    else if(region >= Min_CFEB_in_region_b && region <= Max_CFEB_in_region_b) {
      cfeb = Min_CFEB_in_region_b + (Max_CFEB_in_region_b - region);
    }
  }
  else if(tmb_compile_type == 0xd) {
    if(region >= Min_CFEB_in_region_a && region <= Max_CFEB_in_region_a) {
      cfeb = Min_CFEB_in_region_a + (Max_CFEB_in_region_a - region);
    }
    else if(region >= Min_CFEB_in_region_b && region <= Max_CFEB_in_region_b) {
      cfeb = region;
    }
  }
  return cfeb;
}
    

int main(int argc, char * argv[]) {
    unsigned int input_hs = 0;
    unsigned int input_pattern = 0;
    unsigned int input_nhits =0;
    unsigned int input_bx =0;
    unsigned int tmb_compile_type = 0xc;
    std::vector<CLCT> clcts;
    CLCT clct0,clct0_true, clct1, clct1_true;
    clct0.initCLCT();
    clct1.initCLCT();
    clct0_true.keystrip = 0;
    clct0_true.nhits = 0;
    clct0_true.pattern = 0;
    clct0_true.dcfeb = 0;
    clct0_true.localhs = 0;
    clct1_true.keystrip = 0;
    clct1_true.nhits = 0;
    clct1_true.pattern = 0;
    clct1_true.dcfeb = 0;
    clct1_true.localhs = 0;
    std::fstream text_file(argv[1], std::ios_base::in);
    /*if(argc > 1)
        input_hs = atoi(argv[1]);
    if(argc > 2)
        input_pattern = atoi(argv[2]);
    if(argc > 3)
        input_nhits = atoi(argv[3]);
    */
/*
 * Bx where error occurred : 328
 *
 * ----------------------
 *  CLCT0 data = 0xd77a9
 *  ----------------------
 *  CLCT0.Valid      = 0x1
 *  CLCT0.Nhits      = 0x4
 *  CLCT0.pattern    = 0xa
 *  CLCT0.Key HStrip = 119
 *  CLCT0.BXN        = 0x1
 *  CLCT0.sync err   = 0x1
 *
 *  ----------------------
 *  CLCT1 data = 0xd0000
 *  ----------------------
 *  CLCT1.Valid      = 0x0
 *  CLCT1.Nhits      = 0x0
 *  CLCT1.pattern    = 0x0
 *  CLCT1.Key HStrip = 0
 *  CLCT1.BXN        = 0x1
 *  CLCT1.sync err   = 0x1
 *  */
    unsigned int n=0;
    bool errordetected=false;
    TTree *evtree = new TTree("evtree","event tree");//to fill two clct for each events
    evtree->Branch("globalbx",&n);
    evtree->Branch("clct0_truenhits",&clct0_true.nhits);
    evtree->Branch("clct0_truekestrip",&clct0_true.keystrip);
    evtree->Branch("clct0_truepattern",&clct0_true.pattern);
    evtree->Branch("clct0_truedcfeb",&clct0_true.dcfeb);
    evtree->Branch("clct0_truelocalhs",&clct0_true.localhs);
    evtree->Branch("clct1_truenhits",&clct1_true.nhits);
    evtree->Branch("clct1_truekestrip",&clct1_true.keystrip);
    evtree->Branch("clct1_truepattern",&clct1_true.pattern);
    evtree->Branch("clct1_truedcfeb",&clct1_true.dcfeb);
    evtree->Branch("clct1_truelocalhs",&clct1_true.localhs);
    evtree->Branch("clct0_bx",&clct0.bx);
    evtree->Branch("clct0_nhits",&clct0.nhits);
    evtree->Branch("clct0_kestrip",&clct0.keystrip);
    evtree->Branch("clct0_pattern",&clct0.pattern);
    evtree->Branch("clct0_dcfeb",&clct0.dcfeb);
    evtree->Branch("clct0_localhs",&clct0.localhs);
    evtree->Branch("clct1_bx",&clct1.bx);
    evtree->Branch("clct1_nhits",&clct1.nhits);
    evtree->Branch("clct1_kestrip",&clct1.keystrip);
    evtree->Branch("clct1_pattern",&clct1.pattern);
    evtree->Branch("clct1_dcfeb",&clct1.dcfeb);
    evtree->Branch("clct1_localhs",&clct1.localhs);
    std::vector<std::string> prefixs;
    std::vector<std::string> clctdata;
    prefixs.clear();
    std::string str;
    while (text_file>>str){
        if (prefixs.size()==5 and prefixs[0]=="Bx" and prefixs[1]=="where" and prefixs[2]=="error" and prefixs[3]=="occurred" 
            and prefixs[4]== ":" )  {
            n = atoi(str.c_str());
            std::cout <<" find error bx "<< n << std::endl;
            errordetected = true;//flag to fine CLCT
            prefixs.clear();
        }
        else if (prefixs.size()==5){
            prefixs.erase(prefixs.begin());
            prefixs.push_back(str);
            //std::cout <<" prefixs at begin " << prefixs.at(0) <<" last "<< prefixs.back() <<" size "<< prefixs.size()<< std::endl;
        }
        else prefixs.push_back(str);

        if (!errordetected) continue;
        //cin>>std::hex>>clct1.clcthex
        //here to find CLCT
       evtree->Fill(); 
    }
   TFile *file = new TFile("CLCT_Ana.root","RECREATE");
   evtree->Write();
   file->Close();

   // text_file.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
    //while(!text_file.eof()){
     /* while (text_file>>input_bx>>input_hs>>input_pattern>>input_nhits){
        clcts.push_back(CLCT());
        clcts[n].bx = input_bx;
        clcts[n].keystrip = input_hs;
        clcts[n].pattern = input_pattern;
        clcts[n].nhits = input_nhits;
        clcts[n].dcfeb = GetInputCFEBByX<32>(input_hs, tmb_compile_type);//get dcfeb
        clcts[n].localhs = GetInputXStrip<32>(input_hs, tmb_compile_type);//get localhs
        
        //std::cout <<"text_file " << text_file << std::endl;
        std::cout <<"bx " << input_bx <<" CLCT hs " << input_hs << "  pattern " << input_pattern << " input_nhits " << input_nhits << std::endl;     
        n++; 
    }*/
}
