/* Copyright 2012 <MulticorewareInc> */

#ifndef CVT_TEST_H_
#define CVT_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class TestCvt: public ::testing::TestWithParam<int>
{

};

struct CvtTest{
  const char* str;
  BrigInstCvt ref;
};

struct CvtTest cvt_pair[62] = {
//cvt_pair[0]
  {
    "cvt_b1_b1 $c1, $c2;",
    {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigb1,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigb1,            //sType
      0,                 //reserved
    }
  },
//cvt_pair[1]
  {
    "cvt_u8_b1 $s1, $c2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu8,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigb1,            //sType
      0,                 //reserved
    }
  },
//cvt_pair[2]
  {
    "cvt_s8_b1 $s1, $c2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs8,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigb1,            //sType
      0,                 //reserved
    }
  },
//cvt_pair[3]
  {
   "cvt_b1_u8 $c1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigb1,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigu8,            //sType
      0,                 //reserved
    }
  },                  

//cvt_pair[4]
  {
   "cvt_u16_u8 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu16,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigu8,            //sType
      0,                 //reserved
    }
  },                   
 //cvt_pair[5]
  {
   "cvt_s16_u8 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs16,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigu8,            //sType
      0,                 //reserved
    }
  },                      
 //cvt_pair[6]
  {
   "cvt_f16_u8 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf16,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigu8,            //sType
      0,                 //reserved
    }
  },                         
//cvt_pair[7]
  {
   "cvt_b1_s8 $c1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigb1,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigs8,            //sType
      0,                 //reserved
    }
  },                 
//cvt_pair[8]
  {
    "cvt_u32_s8 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigs8,            //sType
      0,                 //reserved
    }
  },                         
//cvt_pair[9]
  {
    "cvt_s32_s8 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigs8,            //sType
      0,                 //reserved
    }
  },                      
 //cvt_pair[10]
  {
    "cvt_f32_s8 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigs8,            //sType
      0,                 //reserved
    }
  },                    
//cvt_pair[11]
  {
    "cvt_f32_s8 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigs8,            //sType
      0,                 //reserved
    }
  },          
//cvt_pair[12]
  {
    "cvt_u64_u16 $d1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu64,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigu16,            //sType
      0,                 //reserved
    }
  },                
  //cvt_pair[13]
  {
    "cvt_s64_u16 $d1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs64,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigu16,            //sType
      0,                 //reserved
    }
  },                  
    //cvt_pair[14]
  {
    "cvt_f64_u16 $d1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf64,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigu16,            //sType
      0,                 //reserved
    }
  },                 
    //cvt_pair[15]
  {
    "cvt_u8_s16 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu8,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigs16,            //sType
      0,                 //reserved
    }
  },                 
    //cvt_pair[16]
  {
    "cvt_s8_s16 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs8,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigs16,            //sType
      0,                 //reserved
    }
  },                 
   //cvt_pair[17]
  {
   "cvt_u8_f16 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu8,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf16,            //sType
      0,                 //reserved
    }
  },                
   //cvt_pair[18]
  {
   "cvt_s8_f16 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs8,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf16,            //sType
      0,                 //reserved
    }
  },                 
    //cvt_pair[19]
  {
   "cvt_u16_f16 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu16,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf16,            //sType
      0,                 //reserved
    }
  },                 
  //cvt_pair[20]
  {
   "cvt_u16_f16 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu16,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf16,            //sType
      0,                 //reserved
    }
  },                    
 //cvt_pair[21]
  {
   "cvt_s16_f16 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs16,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf16,            //sType
      0,                 //reserved
    }
  },                       
  //cvt_pair[22]
  {
   "cvt_f16_f16 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf16,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf16,            //sType
      0,                 //reserved
    }
  },                    
//cvt_pair[23]
  {
   "cvt_u8_u32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu8,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigu32,            //sType
      0,                 //reserved
    }
  },                    
  //cvt_pair[24]
  {
   "cvt_s8_u32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs8,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigu32,            //sType
      0,                 //reserved
    }
  },         
 //cvt_pair[25]
  {
   "cvt_u16_s32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu16,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigs32,            //sType
      0,                 //reserved
    }
  },            
 //cvt_pair[26]
  {
   "cvt_s16_s32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs16,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigs32,            //sType
      0,                 //reserved
    }
  },               
 //cvt_pair[27]
  {
   "cvt_f16_s32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf16,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigs32,            //sType
      0,                 //reserved
    }
  },                 
 //cvt_pair[28]
  {
   "cvt_u32_f32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf32,            //sType
      0,                 //reserved
    }
  },                    
 //cvt_pair[29]
  {
   "cvt_s32_f32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf32,            //sType
      0,                 //reserved
    }
  },                       
//cvt_pair[30]
  {
   "cvt_f32_f32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf32,            //sType
      0,                 //reserved
    }
  },                     
  //cvt_pair[31]
  {
   "cvt_u64_u64 $d1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu64,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigu64,            //sType
      0,                 //reserved
    }
  },                  
  //cvt_pair[32]
  {
   "cvt_s64_u64 $d1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs64,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigu64,            //sType
      0,                 //reserved
    }
  },                 
//cvt_pair[33]
  {
   "cvt_f64_u64 $d1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf64,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigu64,            //sType
      0,                 //reserved
    }
  },                     
  //cvt_pair[34]
  {
   "cvt_u8_s64 $s1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu8,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigs64,            //sType
      0,                 //reserved
    }
  },                
 //cvt_pair[35]
  {
   "cvt_s8_s64 $s1, $d2;",  
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs8,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,0,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigs64,            //sType
      0,                 //reserved
    }
  },                
  //cvt_pair[36]
  {
   "cvt_u16_f64 $s1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu16,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  },                  
   //cvt_pair[37]
  {
   "cvt_s16_f64 $s1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs16,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  },                   
//cvt_pair[38]
  {
   "cvt_f16_f64 $s1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf16,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  },            
 //cvt_pair[39]
  {
   "cvt_upi_f16_f16 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf16,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,2,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf16,            //sType
      0,                 //reserved
    }
  },               
  //cvt_pair[40]
  {
   "cvt_downi_f32_f32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,3,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf32,            //sType
      0,                 //reserved
    }
  },              
 //cvt_pair[41]
  {
   "cvt_zeroi_f64_f64 $d1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf64,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,1,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  },                 
  //cvt_pair[42]
  {
   "cvt_neari_f32_f32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf32,            //sType
      0,                 //reserved
    }
  },                 
  //cvt_pair[43]
  {
   "cvt_upi_u32_f32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,2,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf32,            //sType
      0,                 //reserved
    }
  },              
   //cvt_pair[44]
  {
   "cvt_downi_s32_f32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,3,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf32,            //sType
      0,                 //reserved
    }
  },               
//cvt_pair[45]
  {
   "cvt_up_f32_f64 $s1, $d2;", 
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,2,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  },                   
  //cvt_pair[46]
  {
   "cvt_down_f32_f64 $s1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,3,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  },             
 //cvt_pair[47]
  {
   "cvt_zero_f32_f64 $s1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,1,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  },               
 //cvt_pair[48]
  {
   "cvt_up_u32_f64 $s1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,2,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  },               
 //cvt_pair[49]
  {
   "cvt_down_s32_f64 $s1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
     {1,1,3,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  },               
 //cvt_pair[50]
  {
    "cvt_ftz_f64_f64 $d1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf64,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,0,1,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  },                 
 //cvt_pair[51]
  {
    "cvt_upi_f16_f16 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf16,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,2,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf16,            //sType
      0,                 //reserved
    }
  },              
 //cvt_pair[52]
  {
    "cvt_downi_f32_f32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,3,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf32,            //sType
      0,                 //reserved
    }
  },                
 //cvt_pair[53]
  {
    "cvt_zeroi_f64_f64 $d1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf64,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
       {1,1,1,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  },           
 //cvt_pair[54]
  {
    "cvt_neari_f32_f32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,0,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf32,            //sType
      0,                 //reserved
    }
  },              
  //cvt_pair[55]
  {
    "cvt_upi_u32_f32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
       {1,1,2,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf32,            //sType
      0,                 //reserved
    }
  },             
  //cvt_pair[56]
  {
    "cvt_downi_s32_f32 $s1, $s2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
       {1,1,3,0,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf32,            //sType
      0,                 //reserved
    }
  },               
  //cvt_pair[57]
  {
    "cvt_ftz_up_f32_f64 $s1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
       {1,1,2,1,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  },                 
  //cvt_pair[58]
  {
    "cvt_ftz_down_f32_f64 $s1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
       {1,1,3,1,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  },                 
 //cvt_pair[59]
  {
    "cvt_ftz_zero_f32_f64 $s1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigf32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
       {1,1,1,1,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  },                    
//cvt_pair[60]
  {
    "cvt_ftz_up_u32_f64 $s1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigu32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,2,1,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  },                    
  //cvt_pair[61]
  {
     "cvt_ftz_down_s32_f64 $s1, $d2;",
   {
      brig_inst_cvt_size, //size
      BrigEInstCvt,       //kind
      BrigCvt,            //opcode
      Brigs32,             //type
      BrigNoPacking,      //packing
    {operand_offset, operand_offset + reg_size, 0, 0, 0},
      {1,1,3,1,0,0,0},//valid,float,near,noftz,approx,fbar,reserved
      Brigf64,            //sType
      0,                 //reserved
    }
  }
};

class TestCvtInvalid: public ::testing::TestWithParam<int>
{

};

std::string inputarray_cvt_invalid[4] = {
  //must reject ftz combined with integer rounding modes
  "cvt_ftz_upi_f32_f32 $s1, $s2;",
  "cvt_ftz_downi_f32_f32 $s1, $s2;",
  "cvt_ftz_zeroi_f64_f64 $d1, $d2;",
  "cvt_ftz_neari_f32_f32 $s1, $s2;"
};
}  // namespace brig
}  // namespace hsa
#endif //CVT_TEST_H_
