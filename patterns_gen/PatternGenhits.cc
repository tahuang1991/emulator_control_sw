
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

#define RAMPAGE_SIZE 4*1024 

struct CLCT{
    unsigned int bx;
    unsigned int keystrip;
    unsigned int localhs;
    unsigned int dcfeb;
    unsigned int pattern;
    unsigned int nhits;

};

struct Hit{
    unsigned int bx;
    unsigned int strip;
    unsigned int localhs;
    unsigned int dcfeb;
    unsigned int layer;
};



int layerorder_all[6][6] = {{3,1,5,6,4,2},//layerorder1
                      {4,2,5,6,3,1},
                      {1,3,6,5,2,4},
                      {2,4,6,5,1,3},
                      {4,6,2,1,3,5},
                      {1,2,3,4,5,6}
                      };

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
    
bool hits_Generation(int strip, int pattern, int Nhits, unsigned int hits[CSCConstants::NUM_DCFEBS]){

    //
    if (pattern<2 || pattern >= CSCConstants::NUM_PATTERNS) return false;
    if (Nhits < 1 || Nhits > CSCConstants::NUM_LAYERS) return false;
    std::srand(std::time(0)); // use current time as seed for random generator
    //int random_variable = std::rand();
    for (int i = 0; i < Nhits; i++){
        int layer = std::abs(std::rand())%6;
        while ( hits[layer] != 999)   layer = std::abs(std::rand())%6;
        int hs  = strip+CSCConstants::CLCTPatterns[pattern][layer];
        if (hs < 0 || hs > CSCConstants::NUM_HS) return false;
        hits[layer] = hs;
        //std::cout << "i " << i << " layer " << layer <<"  hs " << hs << std::endl;
    }
    return true;
}

void  triad_Generation( unsigned int (&triads)[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS],unsigned int strip, unsigned int layer ,int tmb_compile_type){

   bool reverselayers = (tmb_compile_type == 0xb);
//unsigned int triads[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS] = {0};
   for (unsigned int j=0; j < CSCConstants::NUM_LAYERS; j++){
        if (j !=layer-1 ) continue;// layer is counted from 1, while j is from 0
        int dcfeb = GetInputCFEBByX<32>(strip, tmb_compile_type);
        int localhs = GetInputXStrip<32>(strip, tmb_compile_type);
        int distrip = localhs/4;
        unsigned int n = 1 << (distrip);
        bool leftstrip = (localhs%4 < 2);//? triad rules for different dcfeb
        bool lefths = ((localhs%4)%2 == 0);
        int l = j;
        if (reverselayers) l = 6-j;
        triads[dcfeb][0][l] = n;
        triads[dcfeb][1][l] = leftstrip? 0 : n;
        triads[dcfeb][2][l] = lefths? 0 : n;
        std::cout <<"init layer " << j <<" real layer "<< l <<"  dcfeb " << dcfeb << " hs "<< strip <<"  localhs " << localhs << " distrip " << distrip 
            << " 1st " << (std::bitset<8>)n << " 2nd " <<(std::bitset<8>)triads[dcfeb][1][j] << " 3rd " << (std::bitset<8>)triads[dcfeb][2][j] <<std::endl;     
  }
   std::cout << std::endl; 
}



/*
void  triad_Generation( unsigned int (&triads)[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS], int strip, int pattern, int Nhits , int tmb_compile_type){

   unsigned int hits[6] = {999, 999, 999, 999, 999, 999};
//unsigned int triads[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS] = {0};
   hits_Generation(strip, pattern, Nhits, hits);
   for (int j=0; j < CSCConstants::NUM_LAYERS; j++){
        if (hits[j] == 999) continue;//default is 0
        int dcfeb = GetInputCFEBByX<32>(hits[j], tmb_compile_type);
        int localhs = GetInputXStrip<32>(hits[j], tmb_compile_type);
        int distrip = localhs/4;
        unsigned int n = 1 << (distrip);
        bool leftstrip = (localhs%4 < 2);//? triad rules for different dcfeb
        bool lefths = ((localhs%4)%2 == 0);
        triads[dcfeb][0][j] = n;
        triads[dcfeb][1][j] = leftstrip? 0 : n;
        triads[dcfeb][2][j] = lefths? 0 : n;
        std::cout <<" layer " << j <<"  dcfeb " << dcfeb << " hs "<< hits[j] <<"  localhs " << localhs << " distrip " << distrip 
            << " 1st " << (std::bitset<8>)n << " 2nd " <<(std::bitset<8>)triads[dcfeb][1][j] << " 3rd " << (std::bitset<8>)triads[dcfeb][2][j] <<std::endl;     
  }
   std::cout << std::endl; 
}

bool triad_Generation( unsigned int (&triads)[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS], CLCT clct, int tmb_compile_type){

    return triad_Generation(triads, clct.keystrip, clct.pattern, clct.nhits, tmb_compile_type);
}
*/  

void dumptriads(unsigned int triads[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS]){


    for (int j = 0; j < 3; j++){
        for (int k = 0; k < CSCConstants::NUM_LAYERS; k++){
            for (int i = 0; i < CSCConstants::NUM_DCFEBS; i++)
                std::cout <<"  " << (std::bitset<8>)triads[i][j][k];     
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;

}

bool checktwotriads(unsigned int (&triads)[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS], unsigned int (&triads_final)[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS]){

    unsigned int test=0;
    for (int j = 0; j < 3; j++){
        for (int k = 0; k < CSCConstants::NUM_LAYERS; k++){
            for (int i = 0; i < CSCConstants::NUM_DCFEBS; i++){
                test = (triads[i][j][k] & triads_final[i][j][k]);
                if (test != 0) {
                    std::cout <<" error test is not 0 , test "<<test <<  std::endl;
                    dumptriads(triads);
                    dumptriads(triads_final);
                    return false;
                }
            }
        }
    }
  return true;
}


void finalizetriads(unsigned int (&triads)[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS], unsigned int (&triads_final)[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS]){


    for (int j = 0; j < 3; j++){
        for (int k = 0; k < CSCConstants::NUM_LAYERS; k++){
            for (int i = 0; i < CSCConstants::NUM_DCFEBS; i++){
                triads_final[i][j][k] = triads[i][j][k] | triads_final[i][j][k];
                triads[i][j][k] =0;
            }
        }
    }
   
}

void writetriads(unsigned int (&triads)[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS],std::vector<std::fstream*> oss, int thisfeb=-1){
   
    //std::cout <<" triads written into pattern file, not synchronized, dcfeb "<< thisfeb << std::endl;
    //dumptriads(triads);
    if (thisfeb==-1){
    for (int i = 0; i < CSCConstants::NUM_DCFEBS; i++){
        for (int j = 0; j < 3; j++){
            for (int k = 0; k < CSCConstants::NUM_LAYERS; k++){
                (*(oss[i])) << char(triads[i][j][k]);
                triads[i][j][k] = 0;//clear final triads
            }
            (*(oss[i])) << char(0) << char(0);
        }            
    }
    }else if (thisfeb>=0 and thisfeb< CSCConstants::NUM_DCFEBS){
        for (int j = 0; j < 3; j++){
            for (int k = 0; k < CSCConstants::NUM_LAYERS; k++){
                (*(oss[thisfeb])) << char(triads[thisfeb][j][k]);
                triads[thisfeb][j][k] = 0;//clear final triads
            }
            (*(oss[thisfeb])) << char(0) << char(0);
        }            
    }else std::cout<<"dcfeb error! thisfeb "<< thisfeb << std::endl;
   
}

void fillnbytes(std::vector<std::fstream*> oss,unsigned int n, unsigned int thisfeb){

    for (unsigned int j=0; j<n; j++)
        (*(oss[thisfeb])) << char(0);

}

void fillnbytes_allfebs(std::vector<std::fstream*> oss,unsigned int n){

    for (unsigned int i=0; i<oss.size(); i++)
        for (unsigned int j=0; j<n; j++)
            (*(oss[i])) << char(0);

}



void shufflelayers(unsigned int (&triads)[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS], int thisfeb=-1, int layerset = 1){

    if (thisfeb ==-1){ 
    for (int i=0; i < CSCConstants::NUM_DCFEBS ; i++){
        for( int j = 0; j < 3; j++){
		unsigned int layers[6] ={0};
		for (int k=0; k < CSCConstants::NUM_LAYERS ; k++){
                        //old layer layerorder[k] to
			layers[k] = triads[i][j][(layerorder_all[layerset-1][k]-1)];	
		}
		for (int k=0; k < CSCConstants::NUM_LAYERS ; k++){
                        //new layer k 
			triads[i][j][k] = layers[k];	
		}
	    }
    }
    }else if (thisfeb>=0 and thisfeb<CSCConstants::NUM_DCFEBS){
    
        for( int j = 0; j < 3; j++){
		    unsigned int layers[6] ={0};
		    for (int k=0; k < CSCConstants::NUM_LAYERS ; k++){
                        //old layer layerorder[k] to
			    layers[k] = triads[thisfeb][j][(layerorder_all[layerset-1][k]-1)];	
		    }
		    for (int k=0; k < CSCConstants::NUM_LAYERS ; k++){
                        //new layer k 
			    triads[thisfeb][j][k] = layers[k];	
		    }
       } 
    
    }else std::cout<<"dcfeb error! thisfeb "<< thisfeb << std::endl;

}

int main(int argc, char * argv[]) {
    unsigned int input_hs = 0;
    unsigned int input_layer=0;//layer count from 1 to 6
    unsigned int input_bx =0;
    unsigned int tmb_compile_type = 0xc;
    bool stagger = (tmb_compile_type == 0xa || tmb_compile_type == 0xb);
    std::vector<Hit> hits;
    if(argc <= 1) std::cout <<"error! false to get input file" << std::endl;
    //std::cout <<" argv1 "<< argv[1] << std::endl;
    std::fstream text_file(argv[1], std::ios_base::in);
    if (not text_file.good()){
        std::cout <<"error! can not find or open input file" << std::endl;
        return 0;
    }
    unsigned int triads[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS] = { { {0} } };
    unsigned int triads_final[CSCConstants::NUM_DCFEBS][3][CSCConstants::NUM_LAYERS] = { { {0} } };

    unsigned int n=0;
    std::string str;
    std::string prefix;
   // text_file.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
    std::getline(text_file, prefix);
    std::getline(text_file, str);
    
    prefix = prefix.substr(7,prefix.length()-7);
    std::cout <<"prefix "<<prefix <<" second line  " << str << std::endl;
    //while(!text_file.eof()){
      while (text_file>>input_bx>>input_hs >> input_layer){
        hits.push_back(Hit());
        hits[n].bx = input_bx;
        hits[n].strip = input_hs;
        hits[n].layer = input_layer;
        hits[n].dcfeb = GetInputCFEBByX<32>(input_hs, tmb_compile_type);//get dcfeb
        hits[n].localhs = GetInputXStrip<32>(input_hs, tmb_compile_type);//get localhs
        
        //std::cout <<"text_file " << text_file << std::endl;
        //std::cout <<"Hits bx " << input_bx <<" hs " << input_hs << " layer "<< input_layer<< std::endl;     
        n++; 
    }

    std::vector<std::fstream*> oss;
    unsigned int size = hits.size();
    char tmbtype = tmb_compile_type- 0xa + 'a';
    for (int i = 0; i < CSCConstants::NUM_DCFEBS; i++){
        std::stringstream ss;
        ss << prefix <<"_ClctPattern_CFEB" << i << "_tmb"<< tmbtype << ".pat";
    	oss.push_back( new std::fstream(ss.str().c_str(), std::ios_base::out));
    }

    int lockbx[CSCConstants::NUM_DCFEBS]={0};
    //unsigned int lastbx= 0;
    unsigned int lastbxs[CSCConstants::NUM_DCFEBS] = {0};
    for (unsigned int i=0; i<hits.size(); i++){
        //std::cout <<"Hits bx " << hits[i].bx <<" hs " << hits[i].strip << " layer "<< hits[i].layer<< std::endl;     
        unsigned int thisfeb = hits[i].dcfeb;
        if (hits[i].bx <= lastbxs[thisfeb]+lockbx[thisfeb] and hits[i].bx > lastbxs[thisfeb]) continue;
    
        if (lastbxs[thisfeb] < hits[i].bx and lockbx[thisfeb]>0){
            //std::cout <<"final triads for bx "<< lastbx << std::endl;
            //dumptriads(triads_final);
            shufflelayers(triads_final, thisfeb);
            //std::cout <<" shuffle layers  pattern file " << std::endl;
            //dumptriads(triads_final);
            //write lastbx into oss
            writetriads(triads_final, oss, thisfeb); 
            //????????????????
            //how to consider last bx and current bx for each DCFEB
            lockbx[thisfeb] =0;
            lastbxs[thisfeb] = lastbxs[thisfeb]+3;
            for (;lastbxs[thisfeb] < hits[i].bx; lastbxs[thisfeb]++)
                     fillnbytes(oss, 8, thisfeb);//fill each dcfeb 8bytes
            }else if (lastbxs[thisfeb] < hits[i].bx and lockbx[thisfeb]==0){
            for (;lastbxs[thisfeb] < hits[i].bx; lastbxs[thisfeb]++)
                     fillnbytes(oss, 8, thisfeb);//fill each dcfeb 8bytes
            }
     if (lastbxs[thisfeb] == hits[i].bx){
          
          lockbx[thisfeb] = 2;//lock next two bx, when to unlock???
          //get final triads
          triad_Generation(triads, hits[i].strip,hits[i].layer, tmb_compile_type);
          //std::cout <<"i "<< i <<" triad for hs"<< hits[i].strip << std::endl;
          //dumptriads(triads);
          checktwotriads(triads, triads_final);
          finalizetriads(triads, triads_final);
          //std::cout << "final triads at this moment " << std::endl;
          //  dumptriads(triads_final);
        }
    }
        
    shufflelayers(triads_final);
    writetriads(triads_final, oss);
    for (unsigned int i=0; i<CSCConstants::NUM_DCFEBS; i++){
        
        //std::cout <<"dcfeb "<< i <<" last bx " << lastbxs[i] << std::endl; 
        fillnbytes(oss,RAMPAGE_SIZE - (lastbxs[i]+3)*8, i); //bx starts from 0
    }

    for (unsigned int i=0; i< oss.size() ; i++)
        (*(oss[i])) << std::flush;
}
