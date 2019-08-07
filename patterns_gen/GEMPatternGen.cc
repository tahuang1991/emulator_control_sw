
//  
//
//
//
//



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
#include <limits>       // std::numeric_limits
#define RAMPAGE_SIZE 4*1024 

struct GEMPad{
    unsigned int bx;
    unsigned int pad;
    unsigned int etapartition;
    unsigned int vfat;

};

void writenbytes(std::fstream* output, unsigned int n){

    std::cout <<" write n bytes to oss:   " << n << std::endl;
   for (unsigned int i=1;i <= n; i++)
        (*output) << char(0);

}

int main(int argc, char * argv[]) {
    std::vector<GEMPad> input_pads;
    unsigned int tmb_compile_type = 0xc;
    
    //std::fstream text_file(argv[1], std::ios_base::in);
    std::fstream text_file(argv[1]);
     
    unsigned int bx;
    unsigned int pad;
    unsigned int etapartition;
    unsigned int n=0;
    //ignore first line
    std::string str;
    std::string prefix;
   // text_file.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
    std::getline(text_file, prefix);
    std::getline(text_file, str);
    prefix = prefix.substr(7,prefix.length()-6);
    std::cout <<"prefix "<< prefix  <<" second line " << str << std::endl;
    while(!text_file.eof()){
        text_file>>bx>>pad>>etapartition;
        input_pads.push_back(GEMPad());
        input_pads[n].bx = bx;
        input_pads[n].pad = pad;
        input_pads[n].etapartition = etapartition;
        input_pads[n].vfat = (pad/64)*8+(8-etapartition);
        //std::cout <<"text_file " << text_file << std::endl;
        std::cout << "bx " << bx <<" GEM pad " << pad << "  etapartition " << etapartition <<" vfat "<<input_pads[n].vfat << std::endl;     
        n++; 
    }

    std::fstream* oss[24];
    char c;
    //char tmbtype = tmb_compile_type- 0xa + 'a';
    for (unsigned int i=0; i<24; i++){
        std::stringstream ss;
        ss <<prefix<<"_GEMPads_"<< i <<".pat";
        std::cout <<" file name "<< ss << std::endl;
        oss[i] =  new std::fstream(ss.str().c_str(), std::ios_base::out);
        //writenbytes(oss[i], RAMPAGE_SIZE);
        //(*oss[i]) << std::flush;
    }
    int totbytes[24]={0};
    unsigned int remainbits[24] = {0};
    unsigned int remaininfo[24] = {0};
    unsigned int npads_bx[24] = {0};// 4 or less than 4 
    unsigned int lastbx[24] = {0};

    for (unsigned int i = 0; i < input_pads.size() ; i++){
        unsigned int vfat = input_pads[i].vfat;
        std::cout  <<"i " << i << " bx " << input_pads[i].bx << "  etapartition " << (std::bitset<3>)input_pads[i].etapartition 
            <<" GEM pad " << (std::bitset<8>)input_pads[i].pad <<" vfat "<< vfat<< std::endl;     
        if (lastbx[vfat] < input_pads[i].bx ){
            std::cout <<" lastbx " << lastbx[vfat] <<" input_pads bx " << input_pads[i].bx << " bits " << (std::bitset<8>)(remaininfo[vfat] << (8-remainbits[vfat])) << std::endl;
            (*oss[vfat]) << char(remaininfo[vfat] << (8-remainbits[vfat]));
            std::cout <<" writenbytes "<< 7-npads_bx[vfat]*11/8+8*(input_pads[i].bx-1-lastbx[vfat]) << std::endl;
            writenbytes(oss[vfat], 7-npads_bx[vfat]*11/8+8*(input_pads[i].bx-1-lastbx[vfat]));
            lastbx[vfat] = input_pads[i].bx;
            npads_bx[vfat] = 0;
            remainbits[vfat] = 0;
            remaininfo[vfat] = 0;

        }
       if (lastbx[vfat] == input_pads[i].bx){//npads_bx could be 0,1,2,3,4
            if (npads_bx[vfat] == 4) {
                continue;
            }
            else if (npads_bx[vfat] > 4) continue;
            unsigned int info = ((input_pads[i].etapartition << 8) | input_pads[i].pad);
            (*oss[vfat]) << char((remaininfo[vfat] << (8-remainbits[vfat])) | (info >> (3+remainbits[vfat])));
            remainbits[vfat] = 3+remainbits[vfat];
            unsigned int x = std::pow(2, remainbits[vfat]) -1;
            remaininfo[vfat] = (info & x);
            std::cout <<"info  " <<(std::bitset<11>)info << "  remainbits "<< remainbits[vfat] <<" x " << (std::bitset<8>) x << "  remaininfo " <<(std::bitset<8>)remaininfo[vfat] <<std::endl;
            if (remainbits[vfat] >= 8){
                (*oss[vfat]) << char(remaininfo[vfat] >> (remainbits[vfat]-8));
                remainbits[vfat] = remainbits[vfat] - 8;
                x = std::pow(2,remainbits[vfat])-1;
                remaininfo[vfat] = (remaininfo[vfat] & x);
            //std::cout <<"info  " <<(std::bitset<11>)info << "  remainbits "<< remainbits <<" x " << (std::bitset<8>) x << "  remaininfo " <<(std::bitset<8>)remaininfo <<std::endl;
            }
            npads_bx[vfat] ++;
        }
       
       
    }
    for(unsigned int vfat=0; vfat<24; vfat++){ 
        (*oss[vfat]) << char(remaininfo[vfat] << (8-remainbits[vfat]));
        writenbytes(oss[vfat], 7-npads_bx[vfat]*11/8);
        writenbytes(oss[vfat], RAMPAGE_SIZE-lastbx[vfat]*8-8);
        (*oss[vfat]) << std::flush;
    }
}
