
//  main file to generate GEM cluster pattern file from txt file 
//  txt file template:
//  ============
//  prefix:Simpletest
//  bx Pad etapartition size
//  1  10  4     4
//  ===============
//     2019, Tao Huang


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
    unsigned int etapartition;//0-7
    unsigned int globalpad; //0-1535
    unsigned int size;//0-7

};

void writenbytes(std::fstream* output, unsigned int n, unsigned int x=255){

    //std::cout <<" write n bytes to oss:   " << n << std::endl;
   for (unsigned int i=1;i <= n; i++)
        (*output) << char(x);

}

void writenbxs(std::fstream* output, unsigned int n){

    
   for (unsigned int i=1;i <= n; i++){
       writenbytes(output, 7);//default 255
       writenbytes(output,1,255);
   }

}

int main(int argc, char * argv[]) {
    std::vector<GEMPad> input_pads;
    unsigned int tmb_compile_type = 0xc;
    
    //std::fstream text_file(argv[1], std::ios_base::in);
    std::fstream text_file(argv[1]);
     
    unsigned int bx;
    unsigned int pad;
    unsigned int etapartition;
    unsigned int size;
    unsigned int n=0;
    //ignore first line
    std::string str;
    std::string prefix;
   // text_file.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
    std::getline(text_file, prefix);
    std::getline(text_file, str);
    prefix = prefix.substr(7,prefix.length()-6);
    std::cout <<"prefix "<< prefix  <<" second line " << str << std::endl;
    //while(!text_file.eof()){
    while(text_file>>bx>>pad>>etapartition>>size){
        //text_file>>bx>>pad>>etapartition>>size;
        input_pads.push_back(GEMPad());
        input_pads[n].bx = bx;
        input_pads[n].pad = pad;
        input_pads[n].etapartition = etapartition;
        input_pads[n].globalpad = pad + etapartition*192;
        input_pads[n].size = size;
        std::cout << "bx " << bx <<" GEM pad " << pad << "  etapartition " << etapartition <<" size "<< size <<" globalpad "<< pad+192*etapartition<< std::endl;     
        //std::cout <<"text_file " << text_file << std::endl;

        n++; 
    }

    std::fstream* oss;
    char c;
    int totbytes;
    //char tmbtype = tmb_compile_type- 0xa + 'a';
    std::stringstream ss;
    ss <<prefix <<"_GEMPads.pat";
    oss =  new std::fstream(ss.str().c_str(), std::ios_base::out);
    
    totbytes = 0;
    unsigned int remainbits = 0;
    unsigned int remaininfo = 0;
    unsigned int icluster = 0;// 4 or less than 4 
    unsigned int lastbx = 0;
    const unsigned int DataBits=14;
    unsigned int x;

    for (unsigned int i = 0; i < input_pads.size() ; i++){
        //std::cout  <<"i " << i << " bx " << input_pads[i].bx<<" size "<< (std::bitset<3>)input_pads[i].size << "  etapartition " << (std::bitset<3>)input_pads[i].etapartition <<" GEM pad " << (std::bitset<8>)input_pads[i].pad << std::endl;     

        std::cout  <<"i " << i << " bx " << input_pads[i].bx<<" clusterbits "<< (std::bitset<3>)input_pads[i].size <<  (std::bitset<11>)input_pads[i].globalpad << std::endl;     
        if (lastbx < input_pads[i].bx ){
            if (icluster>0 and icluster<4){
                x = std::pow(2, 8-remainbits) -1;
                (*oss) << char((remaininfo << (8-remainbits) | x));
            }
            std::cout <<" lastbx " << lastbx <<" input_pads bx " << input_pads[i].bx <<" remain n  "<< remainbits << " bits " << (std::bitset<8>)(remaininfo << (8-remainbits)) << std::endl;
            writenbytes(oss, 7-icluster*14/8);
            //writenbytes(oss, 1, 0);//???? why 0 here
            writenbytes(oss, 1, 255);//???? why 0 here
            writenbxs(oss, input_pads[i].bx-1-lastbx);
            lastbx = input_pads[i].bx;
            icluster = 0;
            remainbits = 0;
            remaininfo = 0;
        }
       if (lastbx == input_pads[i].bx){//icluster could be 0,1,2,3,4
            if (icluster >= 4) {
                continue;
            }
            unsigned int info = input_pads[i].globalpad;
            info = (info | input_pads[i].size << 11);//valid info should be 14 bits
            switch(icluster) {
                case 0: remainbits = 6;
                        x = std::pow(2, remainbits) -1;
                        remaininfo = (info & x);
                        (*oss) << char(info >> 6);
                        break;
                case 1: (*oss) << char((remaininfo << 2) | (info >> 12));
                        (*oss) << char(info >> 4);
                        remainbits = 4;
                        x = std::pow(2, remainbits) -1;
                        remaininfo = (info & x);
                        break;
                case 2: (*oss) << char((remaininfo << 4) | (info >> 10));
                        (*oss) << char(info >> 2);
                        remainbits = 2;
                        x = std::pow(2, remainbits) -1;
                        remaininfo = (info & x);
                        break;
                case 3: (*oss) << char((remaininfo << 6) | (info >> 8));
                        (*oss) << char(info);
                        remainbits=0;
                        remaininfo = 0;
                        break;
                default:std::cout <<" error icluster "<< icluster << std::endl;
                        break;
            
            }
            icluster ++;
            //(*oss) << char(((remaininfo << (8-remainbits)) | (info >> ((DataBits-8)+remainbits))));
        }
       
       
    }
    std::cout <<" icluster "<< icluster << " remain n "<< remainbits <<" lastbx "<< lastbx <<std::endl; 
    if (icluster>0 and icluster<4){ 
        x = std::pow(2, 8-remainbits) -1;
        (*oss) << (char(remaininfo << (8-remainbits) | x));
    }
    writenbytes(oss, 7-icluster*DataBits/8);
    //writenbytes(oss, 1, 0);
    writenbytes(oss, 1, 255);
    writenbxs(oss, RAMPAGE_SIZE/8-1-lastbx);
    
    (*oss) << std::flush;
}
