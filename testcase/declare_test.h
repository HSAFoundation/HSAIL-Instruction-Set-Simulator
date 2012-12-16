/* Copyright 2012 <MulticorewareInc> */

#ifndef DECLARE_TEST_H_
#define DECLARE_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class TestGlobalSamplerDecl: public ::testing::TestWithParam<int>
{

};

struct GlobalSamplerDeclTest{
  const char * str;
  BrigDirectiveSampler ref;
};

struct GlobalSamplerDeclTest globalsamplerdecl_pair[10]={
 //globalsamplerdecl_pair[0] 
  {
    "global_Samp &demo[10][10][10] = {coord = normalized, filter = linear,\
    boundaryU = clamp, boundaryV = wrap, boundaryW = mirror } ;",
    {
    brig_directive_sampler_size, //size
    BrigEDirectiveSampler,  //kind
    {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      1000,                 // dim
      string_offset,   // s_name
      BrigSamp,             // type
      1,                    // align
    },
    1,                      //valid
    1,                      //normalized
    BrigSamplerFilterLinear,//filter
    BrigSamplerClamp,       //boundaryU
    BrigSamplerWrap,        //boundaryV
    BrigSamplerMirror,      //boundaryW
    0                       //reserved1
   }
  },
//globalsamplerdecl_pair[1]
  {
    "global_Samp &demo[10][10][10] = {coord = unnormalized, filter = nearest,\
    boundaryU = mirroronce, boundaryV = border, boundaryW = clamp } ;",
    {
    brig_directive_sampler_size,  //size
    BrigEDirectiveSampler,   //kind
    {
      code_offset,      // c_code
      BrigGlobalSpace,       // storag class 
      BrigNone ,             // attribut
      0,                     // reserved
      BrigArray,             // symbolModifier
      1000,                  // dim
      string_offset,    // s_name
      BrigSamp,              // type
      1,                     // align
    },
    1,                       //valid
    0,                       //normalized
    BrigSamplerFilterNearest,//filter
    BrigSamplerMirrorOnce,   //boundaryU
    BrigSamplerBorder,       //boundaryV
    BrigSamplerClamp,        //boundaryW
    0                        //reserved1
  }
  },
 //globalsamplerdecl_pair[2]
  {
    "global_Samp &demo[10][10][10] = {coord = unnormalized, filter = nearest,\
    boundaryU = wrap, boundaryV = clamp, boundaryW = wrap } ;",
      {
    brig_directive_sampler_size,  //size
    BrigEDirectiveSampler,   //kind
    {
      code_offset,      // c_code
      BrigGlobalSpace,       // storag class 
      BrigNone ,             // attribut
      0,                     // reserved
      BrigArray,             // symbolModifier
      1000,                  // dim
      string_offset,   // s_name
      BrigSamp,              // type
      1,                     // align
    },
    1,                       //valid
    0,                       //normalized
    BrigSamplerFilterNearest,//filter
    BrigSamplerWrap,         //boundaryU
    BrigSamplerClamp,        //boundaryV
    BrigSamplerWrap,         //boundaryW
    0                        //reserved1
  }
  },
//globalsamplerdecl_pair[3]
{
    "global_Samp &demo[10][10][10] = {coord = unnormalized, filter = nearest, \
    boundaryU = mirror, boundaryV = mirror, boundaryW = border } ;",
    {
    brig_directive_sampler_size,  //size
    BrigEDirectiveSampler,    //kind
    {
      code_offset,       // c_code
      BrigGlobalSpace,        // storag class 
      BrigNone ,              // attribut
      0,                      // reserved
      BrigArray,              // symbolModifier
      1000,                   // dim
      string_offset,     // s_name
      BrigSamp,               // type
      1,                      // align
    },
    1,                        //valid
    0,                        //normalized
    BrigSamplerFilterNearest, //filter
    BrigSamplerMirror,        //boundaryU
    BrigSamplerMirror,        //boundaryV
    BrigSamplerBorder,        //boundaryW
    0                         //reserved1
  }
  },
//globalsamplerdecl_pair[4]
  {
    "global_Samp &demo[10][10][10] = {coord = unnormalized, filter = nearest, \
    boundaryU = border, boundaryV = mirroronce, boundaryW = mirroronce } ;",
     {
    brig_directive_sampler_size,  //size
    BrigEDirectiveSampler,    //kind
    {
      code_offset,        // c_code
      BrigGlobalSpace,        // storag class 
      BrigNone ,              // attribut
      0,                      // reserved
      BrigArray,              // symbolModifier
      1000,                   // dim
      string_offset,     // s_name
      BrigSamp,               // type
      1,                      // align
    },
    1,                        //valid
    0,                        //normalized
    BrigSamplerFilterNearest, //filter
    BrigSamplerBorder,        //boundaryU
    BrigSamplerMirrorOnce,    //boundaryV
    BrigSamplerMirrorOnce,    //boundaryW
    0                         //reserved1
  }
  },
//globalsamplerdecl_pair[5]
  {
    "global_Samp &demo ;",
    {
    brig_directive_sampler_size, //size
    BrigEDirectiveSampler,    //kind
    {
      code_offset,       // c_code
      BrigGlobalSpace,        // storag class 
      BrigNone ,              // attribut
      0,                      // reserved
      0,              // symbolModifier
      0,                      // dim
      string_offset,     // s_name
      BrigSamp,               // type
      1,                      // align
    },
    0,                        //valid
    0,                        //normalized
    BrigSamplerFilterLinear,  //filter
    BrigSamplerClamp,         //boundaryU
    BrigSamplerClamp,         //boundaryV
    BrigSamplerClamp,         //boundaryW
    0                         //reserved1
  }
  },
//globalsamplerdecl_pair[6]
  {
    "global_Samp &demo[10] = {boundaryU = clamp} ;",
    {
    brig_directive_sampler_size, //size
    BrigEDirectiveSampler,    //kind
    {
      code_offset,       // c_code
      BrigGlobalSpace,        // storag class 
      BrigNone ,              // attribut
      0,                      // reserved
      BrigArray,              // symbolModifier
      10,                     // dim
      string_offset,     // s_name
      BrigSamp,               // type
      1,                      // align
    },
    1,                        //valid
    0,                        //normalized
    BrigSamplerFilterLinear,  //filter
    BrigSamplerClamp,         //boundaryU
    BrigSamplerClamp,         //boundaryV
    BrigSamplerClamp,         //boundaryW
    0                         //reserved1
  }
  },
//globalsamplerdecl_pair[7]
  {
    "global_Samp &demo[10][10]= {boundaryU = clamp, boundaryV = clamp};",
     {
    brig_directive_sampler_size, //size
    BrigEDirectiveSampler,    //kind
    {
      code_offset,       // c_code
      BrigGlobalSpace,        // storag class 
      BrigNone ,              // attribut
      0,                      // reserved
      BrigArray,              // symbolModifier
      100,                    // dim
      string_offset,     // s_name
      BrigSamp,               // type
      1,                      // align
    },
    1,                        //valid
    0,                        //normalized
    BrigSamplerFilterLinear,  //filter
    BrigSamplerClamp,         //boundaryU
    BrigSamplerClamp,         //boundaryV
    BrigSamplerClamp,         //boundaryW
    0                         //reserved1
  }
  },
//globalsamplerdecl_pair[8]
  {
    "global_Samp &demo[10][10][10]={filter = nearest, boundaryU = border, \
    boundaryV = mirroronce} ;",
    {
    brig_directive_sampler_size, //size
    BrigEDirectiveSampler,    //kind
    {
      code_offset,       // c_code
      BrigGlobalSpace,        // storag class 
      BrigNone ,              // attribut
      0,                      // reserved
      BrigArray,              // symbolModifier
      1000,                   // dim
      string_offset,     // s_name
      BrigSamp,               // type
      1,                      // align
    },
    1,                        //valid
    0,                        //normalized
    BrigSamplerFilterNearest, //filter
    BrigSamplerBorder,        //boundaryU
    BrigSamplerMirrorOnce,    //boundaryV
    BrigSamplerClamp,         //boundaryW
    0                         //reserved1
  }
  },
//globalsamplerdecl_pair[9]
  {
    "global_Samp &demo[10][10][10]={filter = nearest, boundaryU = border, \
    boundaryV = mirroronce, boundaryW = mirroronce } ;",
     {
    brig_directive_sampler_size, //size
    BrigEDirectiveSampler,    //kind
    {
      code_offset,       // c_code
      BrigGlobalSpace,        // storag class 
      BrigNone ,              // attribut
      0,                      // reserved
      BrigArray,              // symbolModifier
      1000,                   // dim
      string_offset,     // s_name
      BrigSamp,               // type
      1,                      // align
    },
    1,                        //valid
    0,                        //normalized
    BrigSamplerFilterNearest, //filter
    BrigSamplerBorder,        //boundaryU
    BrigSamplerMirrorOnce,    //boundaryV
    BrigSamplerMirrorOnce,    //boundaryW
    0                         //reserved1
  }
  }
};

class TestGlobalImageDecl: public ::testing::TestWithParam<int>
{

};

struct GlobalImageDeclTest{
  const char * str;
  BrigDirectiveImage ref;
};

struct GlobalImageDeclTest globalimagedecl_pair[18]={
//globalimagedecl_pair[0]
  {
    "global_RWImg &demo = {format = snorm_int8 ,order = r,width = 2,\
    height = 3,depth = 4 } ;",
     {
    brig_directive_image_size, //size
    BrigEDirectiveImage,     //kind
          {
      code_offset,        // c_code
      BrigGlobalSpace,       // storag class 
      BrigNone ,             // attribut
      0,                     // reserved
      0,             // symbolModifier
      0,                     // dim  
      string_offset,    // s_name
      BrigRWImg,             // type
      1,                     // align
           },
    2,                       //width
    3,                       //height
    4,                       //depth
    1,                       //array
    BrigImage_R,             //order  
    BrigSNORM_INT8           //format
  }
  },
//globalimagedecl_pair[1]
  {
    "global_RWImg &demo={format = snorm_int16 ,order = rx,width = 2,\
    height = 3,depth = 4 } ;",
     {
    brig_directive_image_size, //size
    BrigEDirectiveImage,     //kind
          {
      code_offset,       // c_code
      BrigGlobalSpace,       // storag class 
      BrigNone ,             // attribut
      0,                     // reserved
      0,             // symbolModifier
      0,                     // dim  
      string_offset,    // s_name
      BrigRWImg,             // type
      1,                     // align 
           },
    2,                       //width
    3,                       //height
    4,                       //depth
    1,                       //array
    BrigImage_RX,            //order
    BrigSNORM_INT16          //format
      }
   },
//globalimagedecl_pair[2]
  {
    "global_RWImg &demo={format = unorm_int8} ;",
    {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,   // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImageOrderUnknown,  //order
    BrigUNORM_INT8          //format
  }
  },
//globalimagedecl_pair[3]
  {
    "global_RWImg &demo={format = unorm_int16 ,order = rg} ;",
     {
    brig_directive_image_size,//size
    BrigEDirectiveImage,    //kind
          {
      code_offset,      // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RG,           //order
    BrigUNORM_INT16         //format
  }
  },
//globalimagedecl_pair[4]
  {
    "global_RWImg &demo={format = unorm_short_565 ,order = rgx, width= 2} ;",
    {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,   // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    2,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGX,          //order
    BrigUNORM_SHORT_565     //format
  }
  },
//globalimagedecl_pair[5]
  {
    "global_RWImg &demo={format = unorm_short_555 ,order = ra, width =2, \
    height =3} ;",
    {
    brig_directive_image_size,//size
    BrigEDirectiveImage,    //kind
          {
      code_offset,      // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,  // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    2,                      //width
    3,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RA,           //order
    BrigUNORM_SHORT_555     //format
  }
  },
//globalimagedecl_pair[6]
  {
    "global_RWImg &demo={format = unorm_short_101010 ,order = rgb} ;",
    {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,      // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,  // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGB,          //order
    BrigUNORM_SHORT_101010  //format
  }
  },
//globalimagedecl_pair[7]
  {
    "global_RWImg &demo={format = signed_int8 ,order = rgbx} ;",
     {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGBX,         //order
    BrigSIGNED_INT8         //format
  }
  },
//globalimagedecl_pair[8]
  {
    "global_RWImg &demo={format = signed_int16 ,order = rgba} ;",
     {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset, // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGBA,         //order
    BrigSIGNED_INT16        //format
  }
  },
//globalimagedecl_pair[9]
  {
    "global_RWImg &demo={format = signed_int32 ,order = a} ;",
     {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,   // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array   
    BrigImage_A,            //order
    BrigSIGNED_INT32        //format
  }
  },
//globalimagedecl_pair[10]
  {
    "global_RWImg &demo={format = unsigned_int8 ,order = bgra} ;",
    {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,      // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,   // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_BGRA,         //order
    BrigUNSIGNED_INT8       //format
  }
  },
//globalimagedecl_pair[11]
  {
   "global_RWImg &demo={format = unsigned_int16 ,order = a} ;", 
     {
    brig_directive_image_size,//size
    BrigEDirectiveImage,    //kind
          {
      code_offset,      // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                   // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,  // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_A,            //order
    BrigUNSIGNED_INT16      //format
  }
  },
//globalimagedecl_pair[12]
  {
    "global_RWImg &demo={format = unsigned_int32 ,order = a} ;",
    {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,  // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_A,            //order
    BrigUNSIGNED_INT32      //format
  }
  },
//globalimagedecl_pair[13]
  {
    "global_RWImg &demo={format = half_float ,order = luminance} ;",
    {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,      // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_LUMINANCE,    //order
    BrigHALF_FLOAT          //format
  }
  },
//globalimagedecl_pair[14]
  {
    "global_RWImg &demo={format = float ,order = luminance} ;",
    {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,   // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_LUMINANCE,    //order
    BrigFLOAT               //format
  }
  },
//globalimagedecl_pair[15]
  {
    "global_RWImg &demo={format = unorm_int8,order = argb} ;",
     {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,      // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,   // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_ARGB,         //order
    BrigUNORM_INT8          //format
  }
  },
//globalimagedecl_pair[16]
  {
    "global_RWImg &demo={format = unorm_int16,order = intensity} ;",
     {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,      // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_INTENSITY,    //order
    BrigUNORM_INT16         //format
  }
  },
//globalimagedecl_pair[17]
  {
    "global_RWImg &demo[10] ;",
     {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,      // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      10,                   // dim  
      string_offset,   // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImageOrderUnknown,  //order
    BrigImageFormatUnknown  //format
  }
  }
};

class TestGlobalReadOnlyImageDecl: public ::testing::TestWithParam<int>
{

};

struct GlobalReadOnlyImageDeclTest{
  const char * str;
  BrigDirectiveImage ref;
};

struct GlobalReadOnlyImageDeclTest globalreadonlyimagedecl_pair[18] = {
//globalreadonlyimagedecl_pair[0]
  {
    "global_ROImg &demo={format = snorm_int8 ,order = r,width = 2,height = 3,\
    depth = 4 } ;",
    {
    brig_directive_image_size,  //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset , // s_name
      BrigROImg,            // type
      1,                    // align
           },
    2,                      //width
    3,                      //height
    4,                      //depth
    1,                      //array
    BrigImage_R,            //order
    BrigSNORM_INT8          //format
  }
  },
//globalreadonlyimagedecl_pair[1]
  {
    "global_ROImg &demo={format = snorm_int16 ,order = rx,width = 2,height = 3,\
    depth = 4 } ;",
     {
    brig_directive_image_size,//size
    BrigEDirectiveImage,    //kind
          {
      code_offset,    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,  // s_name
      BrigROImg,            // type
      1,                    // align
           },
    2,                      //width
    3,                      //height
    4,                      //depth
    1,                      //array
    BrigImage_RX,           //order
    BrigSNORM_INT16         //format
  }
  },
//globalreadonlyimagedecl_pair[2]
  {
    "global_ROImg &demo[10]={format = unorm_int8} ;",
    {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      10,                    // dim  
      string_offset,    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImageOrderUnknown,  //order
    BrigUNORM_INT8          //format
  }
  },
//globalreadonlyimagedecl_pair[3]
  {
    "global_ROImg &demo={format = unorm_int16 ,order = rg} ;",
     {
    brig_directive_image_size,  //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RG,           //order
    BrigUNORM_INT16         //format
  }
  },
//globalreadonlyimagedecl_pair[4]
  {
    "global_ROImg &demo={format = unorm_short_565 ,order = rgx, width= 2} ;",
     {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,   // s_name
      BrigROImg,            // type
      1,                    // align
           },
    2,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGX,          //order
    BrigUNORM_SHORT_565     //format
  }
  },
//globalreadonlyimagedecl_pair[5]
  {
    "global_ROImg &demo={format = unorm_short_555 ,order = ra, width =2, \
    height =3} ;",
     {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,   // s_name
      BrigROImg,            // type
      1,                    // align
           },
    2,                      //width
    3,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RA,           //order
    BrigUNORM_SHORT_555     //format
  }
  },
//globalreadonlyimagedecl_pair[6]
  {
    "global_ROImg &demo={format = unorm_short_101010 ,order = rgb} ;",
    {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,   // s_name 
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGB,          //order
    BrigUNORM_SHORT_101010  //format
  }
  },
//globalreadonlyimagedecl_pair[7]
  {
    "global_ROImg &demo={format = signed_int8 ,order = rgbx} ;",
    {
    brig_directive_image_size,  //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,  // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGBX,         //order
    BrigSIGNED_INT8         //format
  }
  },
//globalreadonlyimagedecl_pair[8]
 {
    "global_ROImg &demo={format = signed_int8 ,order = rgbx} ;",
    {
    brig_directive_image_size,  //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,  // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGBX,         //order
    BrigSIGNED_INT8         //format
  }
  },
//globalreadonlyimagedecl_pair[9]
  {
    "global_ROImg &demo={format = signed_int32 ,order = a} ;",
    {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,   // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_A,            //order
    BrigSIGNED_INT32        //format
  }
  },
//globalreadonlyimagedecl_pair[10]
  {
    "global_ROImg &demo={format = unsigned_int8 ,order = bgra} ;",
    {
    brig_directive_image_size,  //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,   // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_BGRA,         //order
    BrigUNSIGNED_INT8       //format
  }
  },
//globalreadonlyimagedecl_pair[11]
  {
    "global_ROImg &demo={format = unsigned_int16 ,order = a} ;", 
    {
    brig_directive_image_size,  //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,  // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_A,            //order
    BrigUNSIGNED_INT16      //format
  }
  },
//globalreadonlyimagedecl_pair[12]
  {
    "global_ROImg &demo={format = unsigned_int32 ,order = a} ;",
     {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,   // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,   // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_A,            //order
    BrigUNSIGNED_INT32      //format
  }
  },
//globalreadonlyimagedecl_pair[13]
  {
    "global_ROImg &demo={format = half_float ,order = luminance} ;",
     {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,   // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_LUMINANCE,    //order
    BrigHALF_FLOAT          //format
  }
  },
//globalreadonlyimagedecl_pair[14]
  {
    "global_ROImg &demo={format = float ,order = luminance} ;",
     {
    brig_directive_image_size,//size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,  // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_LUMINANCE,    //order
    BrigFLOAT               //format
  }
  },
//globalreadonlyimagedecl_pair[15]
  {
    "global_ROImg &demo={format = unorm_int8,order = argb} ;",
     {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,   // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_ARGB,         //order
    BrigUNORM_INT8          //format
  }
  },
//globalreadonlyimagedecl_pair[16]
  {
    "global_ROImg &demo={format = unorm_int16,order = intensity} ;",
     {
    brig_directive_image_size,  //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,     // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      0,            // symbolModifier
      0,                    // dim  
      string_offset,  // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_INTENSITY,    //order
    BrigUNORM_INT16         //format
  }
  },
//globalreadonlyimagedecl_pair[17]
  {
    "global_ROImg &demo[10] ;",
    {
    brig_directive_image_size, //size
    BrigEDirectiveImage,    //kind
          {
      code_offset,      // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      10,                    // dim  
      string_offset,    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImageOrderUnknown,  //order
    BrigImageFormatUnknown  //format
  }
  }
};

class TestGlobalPrivateDecl: public ::testing::TestWithParam<int>
{

};

struct GlobalPrivateDeclTest{
  const char * str;
  BrigDirectiveSymbol ref;
};

struct GlobalPrivateDeclTest globalprivatedecl_pair[3] = {
//globalprivatedecl_pair[0]
  {
     "private_u32 &tmp[2][2];\n",
     {
  brig_directive_symbol_size,  // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    4,                      // dim
    string_offset,     // s_name
    Brigu32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalprivatedecl_pair[1]
  {
    "private_s32 &tmp;\n",
    {
  brig_directive_symbol_size,  // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    0,              // symbolModifier
    0,                      // dim
    string_offset,   // s_name
    Brigs32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalprivatedecl_pair[2]
  {
    "private_b32 &tmp[2];\n",
     {
  brig_directive_symbol_size,// size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,      // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigb32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  }
};

class TestGlobalGroupDecl: public ::testing::TestWithParam<int>
{

};

struct GlobalGroupDeclTest{
  const char * str;
  BrigDirectiveSymbol ref;
};

struct GlobalGroupDeclTest globalgroupdecl_pair[3] = {
//globalgroupdecl_pair[0]
  {
    "group_u32 &tmp[2][2];",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,         // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    4,                      // dim
    string_offset,     // s_name
    Brigu32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalgroupdecl_pair[1]
  {
    "group_s32 &tmp;",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,         // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    0,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigs32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalgroupdecl_pair[2]
  {
    "group_b32 &tmp[2];",
     {
  brig_directive_symbol_size,// size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,         // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigb32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  }
};

class TestGlobalSymbolDecl: public ::testing::TestWithParam<int>
{

};

struct GlobalSymbolDeclTest{
  const char * str;
  BrigDirectiveSymbol ref;
};

struct GlobalSymbolDeclTest globalsymboldecl_pair[54] = {
//globalsymboldecl_pair[0]
  {
    "align 1 static group_s8 &tmp[2];",
     {
  brig_directive_symbol_size,  // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,        // c_code
    BrigGroupSpace,         // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigs8,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[1]
  {
    "align 2 static group_s16 &tmp[2];",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,         // storag class 
    BrigStatic,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigs16,                // type
    2,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[2]
  {
    "align 4 static group_s32 &tmp[2];",
    {
  brig_directive_symbol_size,// size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,         // storag class 
    BrigStatic,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigs32,                // type
    4,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[3]
  {
    "align 8 static group_s64 &tmp[2];",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,         // storag class 
    BrigStatic,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigs64,                // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[4]
  {
    "align 4 extern group_u8 &tmp[2];",
    {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,         // storag class 
    BrigExtern,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigu8,                // type
    4,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[5]
  {
    "align 4 extern group_u16 &tmp[2];",
         {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,         // storag class 
    BrigExtern,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigu16,                // type
    4,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[6]
  {
    "align 8 extern group_u32 &tmp[2];",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,         // storag class 
    BrigExtern,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigu32,                // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[7]
  {
    "align 8 extern group_u64 &tmp[2];",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,        // c_code
    BrigGroupSpace,         // storag class 
    BrigExtern,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigu64,                // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[8]
  {
    "static align 2 private_f16 &tmp[2];",
      {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,         // storag class 
    BrigStatic,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigf16,                // type
    2,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[9]
  {
    "static align 8 private_f32 &tmp[2];",
    {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,         // storag class 
    BrigStatic,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigf32,                // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[10]
  {
    "static align 8 private_f64 &tmp[2];",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,        // c_code
    BrigPrivateSpace,         // storag class 
    BrigStatic,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigf64,                // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[11]
  {
    "static align 1 private_b1 &tmp[2];",
    {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigb1,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[12]
  {
    "extern align 1 private_b8 &tmp[2];",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigb8,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[13]
  {
    "extern align 2 private_b16 &tmp[2];",
     {
  brig_directive_symbol_size,  // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigb16,                // type
    2,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[14]
  {
    "extern align 4 private_b32 &tmp[2];",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,      // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigb32,                // type
    4,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[15]
  {
    "extern align 8 private_b64 &tmp[2];",
      {
  brig_directive_symbol_size,  // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigb64,                // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[16] 
  {
    "extern private_b128 &tmp;",
    {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    0,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigb128,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[17]
  {
    "extern private_u8x4 &tmp;",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    0,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigu8x4,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[18]
  {
    "extern private_u8x16 &tmp;",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,      // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    0,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigu8x16,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[19]
  {
    "extern group_s8x16 &tmp;",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    0,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigs8x16,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[20]
  {
    "static group_u16x8 &tmp;",
    {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,      // c_code
    BrigGroupSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    0,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigu16x8,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[21]
  {
    "static group_s16x8 &tmp;",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    0,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigs16x8,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[22]
  {
    "static group_f16x8 &tmp;",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,      // c_code
    BrigGroupSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    0,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigf16x8,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[23]
  {
    "align 4 group_s8x4 &tmp;",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,      // c_code
    BrigGroupSpace,       // storag class 
    BrigNone,             // attribute
    0,                      // reserved
    0,              // symbolModifier
    0,                      // dim
    string_offset,    // s_name
    Brigs8x4,              // type
    4,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[24]
{
   "align 8 private_u8x8 &tmp;",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone,             // attribute
    0,                      // reserved
    0,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigu8x8,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[25]
{
    "const align 8 extern group_s8x8 &tmp;",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigs8x8,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[26]
  {
    "const align 8 static private_s16x2 &tmp[2];",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,        // c_code
    BrigPrivateSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst | BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigs16x2,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[27]
{
    "const extern group_u16x2 &tmp;",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    string_offset,    // s_name
    Brigu16x2,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[28]
  {
    "const static private_f16x2 &tmp[2];",
      {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst | BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigf16x2,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[29]
  {
    "const align 8 group_u16x4 &tmp;",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,     // c_code
    BrigGroupSpace,       // storag class 
    BrigNone,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigu16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[30]
  {
    "const align 8 private_s16x4 &tmp[2];",
     {
  brig_directive_symbol_size,  // size
  BrigEDirectiveSymbol,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone,             // attribute
    0,                      // reserved
    BrigConst | BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigs16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[31]
  {
    "align 8 const extern group_f16x4 &tmp;",
    {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[32]
  {
    "align 8 const static private_f16x4 &tmp[2];",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst | BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[33]
  {
    "extern const align 8 group_f16x4 &tmp;",
      {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[34]
  {
    "static const align 8 private_f16x4 &tmp[2];",
      {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst | BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[35]
  {
    "static const group_u32x4 &tmp;",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigu32x4,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[36]
  {
    "static const group_s32x4 &tmp;",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigs32x4,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
 },
//globalsymboldecl_pair[37]
  {
    "static const group_f32x4 &tmp[2];",
    {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst | BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigf32x4,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[38]
  {
    "extern const private_u32x4 &tmp;",
    {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigu32x4,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[39]
  {
    "extern const private_s32x4 &tmp;",
      {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    string_offset,    // s_name
    Brigs32x4,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[40]
  {
    "extern const private_f32x4 &tmp[2];",
     {
  brig_directive_symbol_size,  // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst | BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigf32x4,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[41]
  {
    "align 8 const group_u32x2 &tmp;",
    {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,      // c_code
    BrigGroupSpace,       // storag class 
    BrigNone,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigu32x2,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[42]
  {
    "align 8 const group_s32x2 &tmp[2];",
     {
  brig_directive_symbol_size,  // size
  BrigEDirectiveSymbol,    // kind
    {
    code_offset,     // c_code
    BrigGroupSpace,       // storag class 
    BrigNone,             // attribute
    0,                      // reserved
    BrigConst | BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigs32x2,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[43]
  {
    "align 8 const private_f32x2 &tmp;",
      {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigf32x2,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[44]
  {
    "align 8 extern const group_f16x4 &tmp;",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,      // c_code
    BrigGroupSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[45]
  {
    "align 8 static const private_f16x4 &tmp[2];",
    {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,        // c_code
    BrigPrivateSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst | BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[46]
  {
    "extern align 8 const group_f16x4 &tmp[2];",
    {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,      // c_code
    BrigGroupSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst | BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[47]
  {
    "static align 8 const private_f16x4 &tmp;",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[48]
  {
    "group_u32 &tmp[2][2];",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,         // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    4,                      // dim
    string_offset,     // s_name
    Brigu32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[49]
  {
    "group_s32 &tmp;",
    {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol,    // kind
    {
    code_offset,      // c_code
    BrigGroupSpace,         // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    0,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigs32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[50]
  {
    "group_b32 &tmp[2];",
     {
  brig_directive_symbol_size,  // size
  BrigEDirectiveSymbol,    // kind
    {
    code_offset,       // c_code
    BrigGroupSpace,         // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigb32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[51]
  {
    "private_u32 &tmp[2][2];\n",
     {
  brig_directive_symbol_size,// size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    4,                      // dim
    string_offset,     // s_name
    Brigu32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[52]
  {
    "private_s32 &tmp;\n",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    0,              // symbolModifier
    0,                      // dim
    string_offset,     // s_name
    Brigs32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  },
//globalsymboldecl_pair[53]
  {
    "private_b32 &tmp[2];\n",
     {
  brig_directive_symbol_size, // size
  BrigEDirectiveSymbol ,    // kind
    {
    code_offset,       // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    string_offset,     // s_name
    Brigb32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
  }
};

class TestGlobalSymbolDeclInvalid: public ::testing::TestWithParam<int>{

};

std::string inputarray_globalsymboldecl_invalid[6] = {
/*If an alignment is specified, 
it must be equal to or greater than the variable's natural alignment.*/
  "align 4 private_u8x8 &tmp;",
  "align 2 group_s8x4 &tmp;",
  "align 8 static group_f16x8 &tmp;",
  "align 1 private_b32 &tmp[2];\n",
//Valid values of n are 1, 2, 4, 8, and 16
  "align 10 group_s8x4 &tmp;",
  "align 9 group_s8x4 &tmp;"
};

class TestGlobalInitializableInvalid: public ::testing::TestWithParam<int>{

};

std::string inputarray_globalinitializable_invalid[6] = {
/*If an alignment is specified, 
it must be equal to or greater than the variable's natural alignment.
Opaque image and sampler objects always have a size of 32 bytes and a
natural alignment of 16 bytes*/
  "align 8 global_Samp &demo ;",
  "align 8 global_RWImg &demo={format = unorm_int8} ;",
  "align 8 global_ROImg &demo[10]={format = unorm_int8} ;",
  "align 2 readonly_s32 &x[4]= {12, 13,14, -13};",
//Valid values of n are 1, 2, 4, 8, and 16
  "align 5 readonly_s32 &x[4]= {12, 13,14, -13};",
  "align 6 readonly_s32 &x[4]= {12, 13,14, -13};"
};


class TestunInitializableDecl: public ::testing::TestWithParam<int>{

};

struct unInitializableDeclTest{
  const char * str;
  BrigDirectiveSymbol ref;
};

struct unInitializableDeclTest uninitializabledecl_pair[3] = {
//uninitializabledecl_pair[0]
  {
    "private_u32 %x;",
    {
      brig_directive_symbol_size,
      BrigEDirectiveSymbol,
      {
        code_offset,
        BrigPrivateSpace,
        BrigNone,
        0,
        0,
        0,
        string_offset,
        Brigu32,
        1
      },
      0,
      0
    }
  },
//uninitializabledecl_pair[1]
  {
    "group_u32 &x[8];",
    {
      brig_directive_symbol_size,
      BrigEDirectiveSymbol,
      {
        code_offset,
        BrigGroupSpace,
        BrigNone,
        0,
        BrigArray,
        8,
        string_offset,
        Brigu32,
        1
      },
      0,
      0
    }
  },
//uninitializabledecl_pair[2]
  {
    "spill_s32 %x[8];",
    {
      brig_directive_symbol_size,
      BrigEDirectiveSymbol,
      {
        code_offset,
        BrigSpillSpace,
        BrigNone,
        0,
        BrigArray,
        8,
        string_offset,
        Brigs32,
        1
      },
      0,
      0
    }
  }
};

class TestKernelArgumentDecl: public ::testing::TestWithParam<int>{

};

struct KernelArgumentDeclTest{
  const char * str;
  BrigDirectiveSymbol ref;
};

struct KernelArgumentDeclTest kernelargumentdecl_pair[4] = {
//argumentdecl_pair[0]
  {
    "kernarg_u32 %x;",
    {
      brig_directive_symbol_size,
      BrigEDirectiveSymbol,
      {
        code_offset,
        BrigKernargSpace,
        BrigNone,
        0,
        0,
        0,
        string_offset,
        Brigu32,
        1
      },
      0,
      0
    }
  },
//argumentdecl_pair[1]
  {
    "kernarg_ROImg %x[8];",
    {
      brig_directive_symbol_size,
      BrigEDirectiveSymbol,
      {
        code_offset,
        BrigKernargSpace,
        BrigNone,
        0,
        BrigArray,
        8,
        string_offset,
        BrigROImg,
        1
      },
      0,
      0
    }
  },
//argumentdecl_pair[2]
  {
    "const static kernarg_u32 %x[2];",
    {
      brig_directive_symbol_size,
      BrigEDirectiveSymbol,
      {
        code_offset,
        BrigKernargSpace,
        BrigStatic, 
        0,
        BrigConst | BrigArray,
        2,
        string_offset,
        Brigu32,
        1
      },
      0,
      0
    }
  },
//argumentdecl_pair[3]
  {
    "const align 8 extern kernarg_ROImg %x;",
    {
      brig_directive_symbol_size,
      BrigEDirectiveSymbol,
      {
        code_offset,
        BrigKernargSpace,
        BrigExtern,
        0,
        BrigConst,
        0,
        string_offset,
        BrigROImg,
        8
      },
      0,
      0
    }
  }
};

class TestArgUninitializableDecl: public ::testing::TestWithParam<int>{

};

struct ArgUninitializableDeclTest{
  const char * str;
  BrigDirectiveSymbol ref;
};

struct ArgUninitializableDeclTest arguninitializabledecl_pair[2] = {
//arguninitializabledecl_pair[0]
  {
    "arg_u32 %x;",
    {
      brig_directive_symbol_size,
      BrigEDirectiveSymbol,
      {
        code_offset,
        BrigArgSpace,
        BrigNone,
        0,
        0,
        0,
        string_offset,
        Brigu32,
        1
      },
      0,
      0
    }
  },
//arguninitializabledecl_pair[1]
  {
    "arg_u32 %x[8];",
    {
      brig_directive_symbol_size,
      BrigEDirectiveSymbol,
      {
        code_offset,
        BrigArgSpace,
        BrigNone,
        0,
        BrigArray,
        8,
        string_offset,
        Brigu32,
        1
      },
      0,
      0
    }
  }
};

class TestArgumentDecl: public ::testing::TestWithParam<int>{

};

struct ArgumentDeclTest{
  const char * str;
  BrigDirectiveSymbol ref;
};

struct ArgumentDeclTest argumentdecl_pair[4] = {
//argumentdecl_pair[0]
  {
    "arg_u32 %x",
    {
      brig_directive_symbol_size,
      BrigEDirectiveSymbol,
      {
        code_offset,
        BrigArgSpace,
        BrigNone,
        0,
        0,
        0,
        string_offset,
        Brigu32,
        1
      },
      0,
      0
    }
  },
//argumentdecl_pair[1]
  {
    "arg_ROImg %x[8]",
    {
      brig_directive_symbol_size,
      BrigEDirectiveSymbol,
      {
        code_offset,
        BrigArgSpace,
        BrigNone,
        0,
        BrigArray,
        8,
        string_offset,
        BrigROImg,
        1
      },
      0,
      0
    }
  },
//argumentdecl_pair[2]
  {
    "const static arg_u32 %x[2]",
    {
      brig_directive_symbol_size,
      BrigEDirectiveSymbol,
      {
        code_offset,
        BrigArgSpace,
        BrigStatic, 
        0,
        BrigConst | BrigArray,
        2,
        string_offset,
        Brigu32,
        1
      },
      0,
      0
    }
  },
//argumentdecl_pair[3]
  {
    "const align 8 extern arg_ROImg %x",
    {
      brig_directive_symbol_size,
      BrigEDirectiveSymbol,
      {
        code_offset,
        BrigArgSpace,
        BrigExtern,
        0,
        BrigConst,
        0,
        string_offset,
        BrigROImg,
        8
      },
      0,
      0
    }
  }
};

}  // namespace brig
}  // namespace hsa
#endif //DECLARE_TEST_H_

