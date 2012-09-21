/* Copyright 2012 <MulticorewareInc> */

#ifndef GLOBALDECL_TEST_H_
#define GLOBALDECL_TEST_H_

namespace hsa {
namespace brig {

class CodegenTestGlobalSamplerDecl: public ::testing::TestWithParam<int>
{

};

std::string inputarray_globalsamplerdecl[10] = {
  "global_Samp &demo[10][10][10] = {coord = normalized, filter = linear,\
    boundaryU = clamp, boundaryV = wrap, boundaryW = mirror } ;",
  "global_Samp &demo[10][10][10] = {coord = unnormalized, filter = nearest,\
    boundaryU = mirroronce, boundaryV = border, boundaryW = clamp } ;",
  "global_Samp &demo[10][10][10] = {coord = unnormalized, filter = nearest,\
    boundaryU = wrap, boundaryV = clamp, boundaryW = wrap } ;",
  "global_Samp &demo[10][10][10] = {coord = unnormalized, filter = nearest, \
    boundaryU = mirror, boundaryV = mirror, boundaryW = border } ;",
  "global_Samp &demo[10][10][10] = {coord = unnormalized, filter = nearest, \
    boundaryU = border, boundaryV = mirroronce, boundaryW = mirroronce } ;",
  "global_Samp &demo ;",
  "global_Samp &demo[10] = {boundaryU = clamp} ;",
  "global_Samp &demo[10][10] = {boundaryU = clamp, boundaryV = clamp};",
  "global_Samp &demo[10][10][10] = {filter = nearest, boundaryU = border, \
    boundaryV = mirroronce} ;",
  "global_Samp &demo[10][10][10] = {filter = nearest, boundaryU = border, \
    boundaryV = mirroronce, boundaryW = mirroronce } ;"
};

BrigDirectiveSampler outputarray_globalsamplerdecl[10] = {
  /* "global_Samp &demo[10][10][10]={coord = normalized, filter = linear,\
      boundaryU = clamp, boundaryV = wrap, boundaryW = mirror } ;" */
  {
    40,                     //size
    BrigEDirectiveSampler,  //kind
    {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      1000,                 // dim
      0,                    // s_name
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
   },

  /*"global_Samp &demo[10][10][10]={coord = unnormalized, filter = nearest,\
     boundaryU = mirroronce, boundaryV = border, boundaryW = clamp } ;"*/
  {
    40,                      //size
    BrigEDirectiveSampler,   //kind
    {
      0,                     // c_code
      BrigGlobalSpace,       // storag class 
      BrigNone ,             // attribut
      0,                     // reserved
      BrigArray,             // symbolModifier
      1000,                  // dim
      0,                     // s_name
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
  },

  /* "global_Samp &demo[10][10][10]={coord = unnormalized, filter = nearest,\
    boundaryU = wrap, boundaryV = clamp, boundaryW = wrap } ;" */
  {
    40,                      //size
    BrigEDirectiveSampler,   //kind
    {
      0,                     // c_code
      BrigGlobalSpace,       // storag class 
      BrigNone ,             // attribut
      0,                     // reserved
      BrigArray,             // symbolModifier
      1000,                  // dim
      0,                     // s_name
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
  },

  /* "global_Samp &demo[10][10][10]={coord = unnormalized, filter = nearest, \
    boundaryU = mirror, boundaryV = mirror, boundaryW = border } ;"*/
  {
    40,                       //size
    BrigEDirectiveSampler,    //kind
    {
      0,                      // c_code
      BrigGlobalSpace,        // storag class 
      BrigNone ,              // attribut
      0,                      // reserved
      BrigArray,              // symbolModifier
      1000,                   // dim
      0,                      // s_name
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
  },

  /* "global_Samp &demo[10][10][10]={coord = unnormalized, filter = nearest, \
    boundaryU = border, boundaryV = mirroronce, boundaryW = mirroronce } ;" */
  {
    40,                       //size
    BrigEDirectiveSampler,    //kind
    {
      0,                      // c_code
      BrigGlobalSpace,        // storag class 
      BrigNone ,              // attribut
      0,                      // reserved
      BrigArray,              // symbolModifier
      1000,                   // dim
      0,                      // s_name
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
  },

  //"global_Samp &demo ;"
  {
    40,                       //size
    BrigEDirectiveSampler,    //kind
    {
      0,                      // c_code
      BrigGlobalSpace,        // storag class 
      BrigNone ,              // attribut
      0,                      // reserved
      BrigArray,              // symbolModifier
      0,                      // dim
      0,                      // s_name
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
  },

  //"global_Samp &demo[10] = {boundaryU = clamp} ;"
  {
    40,                       //size
    BrigEDirectiveSampler,    //kind
    {
      0,                      // c_code
      BrigGlobalSpace,        // storag class 
      BrigNone ,              // attribut
      0,                      // reserved
      BrigArray,              // symbolModifier
      10,                     // dim
      0,                      // s_name
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
  },

  //"global_Samp &demo[10][10]= {boundaryU = clamp, boundaryV = clamp};"
  {
    40,                       //size
    BrigEDirectiveSampler,    //kind
    {
      0,                      // c_code
      BrigGlobalSpace,        // storag class 
      BrigNone ,              // attribut
      0,                      // reserved
      BrigArray,              // symbolModifier
      100,                    // dim
      0,                      // s_name
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
  },

  /*"global_Samp &demo[10][10][10]={filter = nearest, boundaryU = border, \
    boundaryV = mirroronce} ;" */
  {
    40,                       //size
    BrigEDirectiveSampler,    //kind
    {
      0,                      // c_code
      BrigGlobalSpace,        // storag class 
      BrigNone ,              // attribut
      0,                      // reserved
      BrigArray,              // symbolModifier
      1000,                   // dim
      0,                      // s_name
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
  },

  /*"global_Samp &demo[10][10][10]={filter = nearest, boundaryU = border, \
    boundaryV = mirroronce, boundaryW = mirroronce } ;" */
  {
    40,                       //size
    BrigEDirectiveSampler,    //kind
    {
      0,                      // c_code
      BrigGlobalSpace,        // storag class 
      BrigNone ,              // attribut
      0,                      // reserved
      BrigArray,              // symbolModifier
      1000,                   // dim
      0,                      // s_name
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
};

class CodegenTestGlobalImageDecl: public ::testing::TestWithParam<int>
{

};

std::string inputarray_globalimagedecl[18] = {
  "global_RWImg &demo = {format = snorm_int8 ,order = r,width = 2,\
    height = 3,depth = 4 } ;",
  "global_RWImg &demo = {format = snorm_int16 ,order = rx,width = 2,\
    height = 3,depth = 4 } ;", 
  "global_RWImg &demo = {format = unorm_int8 } ;", 
  "global_RWImg &demo = {format = unorm_int16 ,order = rg} ;", 
  "global_RWImg &demo = {format = unorm_short_565 ,order = rgx, width = 2} ;", 
  "global_RWImg &demo = {format = unorm_short_555 ,order = ra, width = 2, \
    height = 3} ;",  
  "global_RWImg &demo = {format = unorm_short_101010 ,order = rgb} ;", 
  "global_RWImg &demo = {format = signed_int8 ,order = rgbx} ;", 
  "global_RWImg &demo = {format = signed_int16 ,order = rgba} ;", 
  "global_RWImg &demo = {format = signed_int32 ,order = a} ;", 
  "global_RWImg &demo = {format = unsigned_int8 ,order = bgra} ;", 
  "global_RWImg &demo = {format = unsigned_int16 ,order = a} ;", 
  "global_RWImg &demo = {format = unsigned_int32 ,order = a} ;", 
  "global_RWImg &demo = {format = half_float ,order = luminance} ;", 
  "global_RWImg &demo = {format = float ,order = luminance} ;",
  "global_RWImg &demo = {format = unorm_int8,order = argb} ;", 
  "global_RWImg &demo = {format = unorm_int16,order = intensity} ;", 
  "global_RWImg &demo[10] ;"
};

BrigDirectiveImage outputarray_globalimagedecl[18] = {

  /*"global_RWImg &demo={format = snorm_int8 ,order = r,width = 2,
    height = 3,depth = 4 } ;" */
  {
    56,                      //size
    BrigEDirectiveImage,     //kind
          {
      0,                     // c_code
      BrigGlobalSpace,       // storag class 
      BrigNone ,             // attribut
      0,                     // reserved
      BrigArray,             // symbolModifier
      0,                     // dim  
      0,                     // s_name
      BrigRWImg,             // type
      1,                     // align
           },
    2,                       //width
    3,                       //height
    4,                       //depth
    1,                       //array
    BrigImage_R,             //order  
    BrigSNORM_INT8           //format
  },

  /*"global_RWImg &demo={format = snorm_int16 ,order = rx,width = 2,\
    height = 3,depth = 4 } ;" */
  {
    56,                      //size
    BrigEDirectiveImage,     //kind
          {
      0,                     // c_code
      BrigGlobalSpace,       // storag class 
      BrigNone ,             // attribut
      0,                     // reserved
      BrigArray,             // symbolModifier
      0,                     // dim  
      0,                     // s_name
      BrigRWImg,             // type
      1,                     // align 
           },
    2,                       //width
    3,                       //height
    4,                       //depth
    1,                       //array
    BrigImage_RX,            //order
    BrigSNORM_INT16          //format
  },

  //"global_RWImg &demo={format = unorm_int8} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImageOrderUnknown,  //order
    BrigUNORM_INT8          //format
  },

  //"global_RWImg &demo={format = unorm_int16 ,order = rg} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RG,           //order
    BrigUNORM_INT16         //format
  },

  //"global_RWImg &demo={format = unorm_short_565 ,order = rgx, width= 2} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    2,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGX,          //order
    BrigUNORM_SHORT_565     //format
  },

  /*"global_RWImg &demo={format = unorm_short_555 ,order = ra, width =2, \
    height =3} ;" */
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    2,                      //width
    3,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RA,           //order
    BrigUNORM_SHORT_555     //format
  },

  //"global_RWImg &demo={format = unorm_short_101010 ,order = rgb} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGB,          //order
    BrigUNORM_SHORT_101010  //format
  },

  //"global_RWImg &demo={format = signed_int8 ,order = rgbx} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGBX,         //order
    BrigSIGNED_INT8         //format
  },

  //"global_RWImg &demo={format = signed_int16 ,order = rgba} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGBA,         //order
    BrigSIGNED_INT16        //format
  },

  //"global_RWImg &demo={format = signed_int32 ,order = a} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array   
    BrigImage_A,            //order
    BrigSIGNED_INT32        //format
  },

  //"global_RWImg &demo={format = unsigned_int8 ,order = bgra} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_BGRA,         //order
    BrigUNSIGNED_INT8       //format
  },

  //"global_RWImg &demo={format = unsigned_int16 ,order = a} ;", 
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_A,            //order
    BrigUNSIGNED_INT16      //format
  },

  //"global_RWImg &demo={format = unsigned_int32 ,order = a} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_A,            //order
    BrigUNSIGNED_INT32      //format
  },

  //"global_RWImg &demo={format = half_float ,order = luminance} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_LUMINANCE,    //order
    BrigHALF_FLOAT          //format
  },

  //"global_RWImg &demo={format = float ,order = luminance} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_LUMINANCE,    //order
    BrigFLOAT               //format
  },

  //"global_RWImg &demo={format = unorm_int8,order = argb} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_ARGB,         //order
    BrigUNORM_INT8          //format
  },

  //"global_RWImg &demo={format = unorm_int16,order = intensity} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigRWImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_INTENSITY,    //order
    BrigUNORM_INT16         //format
  },

  //"global_RWImg &demo[10] ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      10,                   // dim  
      0,                    // s_name
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
};

class CodegenTestGlobalReadOnlyImageDecl: public ::testing::TestWithParam<int>
{

};

std::string inputarray_globalreadonlyimagedecl[18] = {
  "global_ROImg &demo = {format = snorm_int8 ,order = r,width = 2,height = 3,\
    depth = 4 } ;",
  "global_ROImg &demo = {format = snorm_int16 ,order = rx,width = 2,height = 3,\
    depth = 4 } ;", 
  "global_ROImg &demo[10] = {format = unorm_int8 } ;", 
  "global_ROImg &demo = {format = unorm_int16 ,order = rg} ;", 
  "global_ROImg &demo = {format = unorm_short_565 ,order = rgx, width= 2} ;", 
  "global_ROImg &demo = {format = unorm_short_555 ,order = ra, width = 2, \
    height = 3} ;",  
  "global_ROImg &demo = {format = unorm_short_101010 ,order = rgb} ;", 
  "global_ROImg &demo = {format = signed_int8 ,order = rgbx} ;", 
  "global_ROImg &demo = {format = signed_int16 ,order = rgba} ;", 
  "global_ROImg &demo = {format = signed_int32 ,order = a} ;", 
  "global_ROImg &demo = {format = unsigned_int8 ,order = bgra} ;", 
  "global_ROImg &demo = {format = unsigned_int16 ,order = a} ;", 
  "global_ROImg &demo = {format = unsigned_int32 ,order = a} ;", 
  "global_ROImg &demo = {format = half_float ,order = luminance} ;", 
  "global_ROImg &demo = {format = float ,order = luminance} ;",
  "global_ROImg &demo = {format = unorm_int8,order = argb} ;", 
  "global_ROImg &demo = {format = unorm_int16,order = intensity} ;",
  "global_ROImg &demo[10] ;"
};

BrigDirectiveImage outputarray_globalreadonlyimagedecl[18] = {

 /*"global_ROImg &demo={format = snorm_int8 ,order = r,width = 2,height = 3,\
    depth = 4 } ;"*/
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    2,                      //width
    3,                      //height
    4,                      //depth
    1,                      //array
    BrigImage_R,            //order
    BrigSNORM_INT8          //format
  },

 /*"global_ROImg &demo={format = snorm_int16 ,order = rx,width = 2,height = 3,\
    depth = 4 } ;" */
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    2,                      //width
    3,                      //height
    4,                      //depth
    1,                      //array
    BrigImage_RX,           //order
    BrigSNORM_INT16         //format
  },

 //"global_ROImg &demo[10]={format = unorm_int8} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      10,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImageOrderUnknown,  //order
    BrigUNORM_INT8          //format
  },

 //"global_ROImg &demo={format = unorm_int16 ,order = rg} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RG,           //order
    BrigUNORM_INT16         //format
  },

 //"global_ROImg &demo={format = unorm_short_565 ,order = rgx, width= 2} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    2,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGX,          //order
    BrigUNORM_SHORT_565     //format
  },

 /*"global_ROImg &demo={format = unorm_short_555 ,order = ra, width =2, \
    height =3} ;" */
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    2,                      //width
    3,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RA,           //order
    BrigUNORM_SHORT_555     //format
  },

 //"global_ROImg &demo={format = unorm_short_101010 ,order = rgb} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name 
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGB,          //order
    BrigUNORM_SHORT_101010  //format
  },

 //"global_ROImg &demo={format = signed_int8 ,order = rgbx} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGBX,         //order
    BrigSIGNED_INT8         //format
  },

 //"global_ROImg &demo={format = signed_int16 ,order = rgba} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_RGBA,         //order
    BrigSIGNED_INT16        //format
  },

 //"global_ROImg &demo={format = signed_int32 ,order = a} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_A,            //order
    BrigSIGNED_INT32        //format
  },

 //"global_ROImg &demo={format = unsigned_int8 ,order = bgra} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_BGRA,         //order
    BrigUNSIGNED_INT8       //format
  },

  //"global_ROImg &demo={format = unsigned_int16 ,order = a} ;", 
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_A,            //order
    BrigUNSIGNED_INT16      //format
  },

 //"global_ROImg &demo={format = unsigned_int32 ,order = a} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_A,            //order
    BrigUNSIGNED_INT32      //format
  },

 //"global_ROImg &demo={format = half_float ,order = luminance} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_LUMINANCE,    //order
    BrigHALF_FLOAT          //format
  },

  //"global_ROImg &demo={format = float ,order = luminance} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_LUMINANCE,    //order
    BrigFLOAT               //format
  },

  //"global_ROImg &demo={format = unorm_int8,order = argb} ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_ARGB,         //order
    BrigUNORM_INT8          //format
  },

 //"global_ROImg &demo={format = unorm_int16,order = intensity} ;"
   {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      0,                    // dim  
      0,                    // s_name
      BrigROImg,            // type
      1,                    // align
           },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_INTENSITY,    //order
    BrigUNORM_INT16         //format
  },

  //"global_ROImg &demo[10] ;"
  {
    56,                     //size
    BrigEDirectiveImage,    //kind
          {
      0,                    // c_code
      BrigGlobalSpace,      // storag class 
      BrigNone ,            // attribut
      0,                    // reserved
      BrigArray,            // symbolModifier
      10,                    // dim  
      0,                    // s_name
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
};

class CodegenTestGlobalPrivateDecl: public ::testing::TestWithParam<int>
{

};

std::string inputarray_globalprivatedecl[3] = {
  "private_u32 &tmp[2][2];\n",
  "private_s32 &tmp;\n",
  "private_b32 &tmp[2];\n"
};

BrigDirectiveSymbol outputarray_globalprivatedecl[3] = {

  // "private_u32 &tmp[2][2];\n"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    4,                      // dim
    0,                      // s_name
    Brigu32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },

  // "private_s32 &tmp;\n"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigs32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },

  // "private_b32 &tmp[2];\n"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigb32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
};

class CodegenTestGlobalGroupDecl: public ::testing::TestWithParam<int>
{

};

std::string inputarray_globalgroupdecl[3] = {
   "group_u32 &tmp[2][2];",
   "group_s32 &tmp;",
   "group_b32 &tmp[2];"
};

BrigDirectiveSymbol outputarray_globalgroupdecl[3] = {
  // "group_u32 &tmp[2][2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,         // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    4,                      // dim
    0,                      // s_name
    Brigu32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },

  // "group_s32 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,         // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigs32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },

  // "group_b32 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,         // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigb32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
};

class CodegenTestFileDecl: public ::testing::TestWithParam<int>
{

};

std::string inputarray_filedecl[] = {
  "file 1 \"\" ;",
  "file 2 \"this is a file\";"
};

BrigDirectiveFile outputarray_filedecl[] = {
  {
    16,                   //size
    BrigEDirectiveFile,   //kind
    0,                    //c_code
    1,                    //fileid
    0                     //s_filename
  },
  {
    16,                   //size
    BrigEDirectiveFile,   //kind
    0,                    //c_code
    2,                    //fileid
    0                     //s_filename
  }
};

class TestFileDeclFalseInput: public ::testing::TestWithParam<int>
{

};
std::string inputarray_filedecl_false[] = {
  "file 1 \";",
  "file 2 this is a file\";",
  "file  \"this is a file\";",
  "fil 3 \"this is a file\";",
  "file 3 \"this is a file\" ",
  "file 1.0f \"this is a file\";",
  "file -1 \"this is a file\";"
};

class CodegenTestLocation: public ::testing::TestWithParam<int>
{

};

std::string inputarray_location[] = {
  "loc 12 22 32 ;"
};

BrigDirectiveLoc outputarray_location[] = {
  {
    20,                   //size
    BrigEDirectiveLoc,    //kind
    0,                    //c_code
    12,                   //sourceFile
    22,                   //sourceLine
    32                    //sourceColumn
  },

};

class TestLocationFalseInput: public ::testing::TestWithParam<int>
{

};
std::string inputarray_location_false[] = {
  "locq 12 22 32 ;",
  "loc 12 22 32 ",
  "loc 12 22 ; ",
  "loc -12 22 32 ;",
  "loc 12 22.0f 32 ;"
};

class CodegenTestGlobalSymbolDecl: public ::testing::TestWithParam<int>
{

};

std::string inputarray_globalsymboldecl[] = {
//alignment externOrStatic
  "align 1 static group_s8 &tmp[2];",
  "align 2 static group_s16 &tmp[2];",
  "align 4 static group_s32 &tmp[2];",
  "align 8 static group_s64 &tmp[2];",
  "align 4 extern group_u8 &tmp[2];",
  "align 4 extern group_u16 &tmp[2];",
  "align 8 extern group_u32 &tmp[2];",
  "align 8 extern group_u64 &tmp[2];",
//externOrStatic alignment
  "static align 2 private_f16 &tmp[2];",
  "static align 8 private_f32 &tmp[2];",
  "static align 8 private_f64 &tmp[2];",
  "static align 1 private_b1 &tmp[2];",
  "extern align 1 private_b8 &tmp[2];",
  "extern align 2 private_b16 &tmp[2];",
  "extern align 4 private_b32 &tmp[2];",
  "extern align 8 private_b64 &tmp[2];",
 //externOrStatic
  "extern private_b128 &tmp;",
  "extern private_u8x4 &tmp;",
  "extern private_u8x16 &tmp;",
  "extern group_s8x16 &tmp;",
  "static group_u16x8 &tmp;",
  "static group_s16x8 &tmp;",
  "static group_f16x8 &tmp;",
//alignment
  "align 4 group_s8x4 &tmp;",
  "align 8 private_u8x8 &tmp;",
//const alignment externOrStatic
  "const align 8 extern group_s8x8 &tmp;",
  "const align 8 static private_s16x2 &tmp[2];",
//const externOrStatic
  "const extern group_u16x2 &tmp;",
  "const static private_f16x2 &tmp[2];",
//const alignment
  "const align 8 group_u16x4 &tmp;",
  "const align 8 private_s16x4 &tmp[2];",
//alignment const externOrStatic
  "align 8 const extern group_f16x4 &tmp;",
  "align 8 const static private_f16x4 &tmp[2];",
//externOrStatic const alignment
  "extern const align 8 group_f16x4 &tmp;",
  "static const align 8 private_f16x4 &tmp[2];",
//externOrStatic const
  "static const group_u32x4 &tmp;",
  "static const group_s32x4 &tmp;",
  "static const group_f32x4 &tmp[2];",
  "extern const private_u32x4 &tmp;",
  "extern const private_s32x4 &tmp;",
  "extern const private_f32x4 &tmp[2];",
//alignment const
  "align 8 const group_u32x2 &tmp;",
  "align 8 const group_s32x2 &tmp[2];",
  "align 8 const private_f32x2 &tmp;",
//alignment externOrStatic const
  "align 8 extern const group_f16x4 &tmp;",
  "align 8 static const private_f16x4 &tmp[2];",
//externOrStatic alignment const
  "extern align 8 const group_f16x4 &tmp[2];",
  "static align 8 const private_f16x4 &tmp;",
//no declprefix
  "group_u32 &tmp[2][2];",
  "group_s32 &tmp;",
  "group_b32 &tmp[2];",
  "private_u32 &tmp[2][2];",
  "private_s32 &tmp;",
  "private_b32 &tmp[2];"
};

BrigDirectiveSymbol outputarray_globalsymboldecl[] = {
//alignment externOrStatic
//"align 1 static group_s8 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,         // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigs8,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"align 2 static group_s16 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,         // storag class 
    BrigStatic,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigs16,                // type
    2,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"align 4 static group_s32 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,         // storag class 
    BrigStatic,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigs32,                // type
    4,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"align 8 static group_s64 &tmp[2];"
 {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,         // storag class 
    BrigStatic,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigs64,                // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"align 4 extern group_u8 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,         // storag class 
    BrigExtern,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigu8,                // type
    4,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"align 4 extern group_u16 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,         // storag class 
    BrigExtern,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigu16,                // type
    4,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"align 8 extern group_u32 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,         // storag class 
    BrigExtern,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigu32,                // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"align 8 extern group_u64 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,         // storag class 
    BrigExtern,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigu64,                // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//externOrStatic alignment
//"static align 2 private_f16 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,         // storag class 
    BrigStatic,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigf16,                // type
    2,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"static align 8 private_f32 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,         // storag class 
    BrigStatic,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigf32,                // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"static align 8 private_f64 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,         // storag class 
    BrigStatic,            // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigf64,                // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"static align 1 private_b1 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigb1,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"extern align 1 private_b8 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigb8,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"extern align 2 private_b16 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigb16,                // type
    2,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"extern align 4 private_b32 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigb32,                // type
    4,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"extern align 8 private_b64 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigb64,                // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//externOrStatic
//"extern private_b128 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigb128,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"extern private_u8x4 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigu8x4,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"extern private_u8x16 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigu8x16,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"extern group_s8x16 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigs8x16,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"static group_u16x8 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigu16x8,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"static group_s16x8 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigs16x8,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"static group_f16x8 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigf16x8,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//alignment
//"align 4 group_s8x4 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigNone,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigs8x4,              // type
    4,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"align 8 private_u8x8 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone,             // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigu8x8,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//const alignment externOrStatic
//"const align 8 extern group_s8x8 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigs8x8,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"const align 8 static private_s16x2 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigs16x2,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//const externOrStatic
//"const extern group_u16x2 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigu16x2,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"const static private_f16x2 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigf16x2,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//const alignment
//"const align 8 group_u16x4 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigNone,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigu16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"const align 8 private_s16x4 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigs16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//alignment const externOrStatic
//"align 8 const extern group_f16x4 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"align 8 const static private_f16x4 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//externOrStatic const alignment
//"extern const align 8 group_f16x4 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"static const align 8 private_f16x4 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//externOrStatic const
//"static const group_u32x4 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigu32x4,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"static const group_s32x4 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigs32x4,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"static const group_f32x4 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigf32x4,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"extern const private_u32x4 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigu32x4,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"extern const private_s32x4 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigs32x4,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"extern const private_f32x4 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigf32x4,              // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//alignment const
//"align 8 const group_u32x2 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigNone,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigu32x2,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"align 8 const group_s32x2 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigNone,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigs32x2,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"align 8 const private_f32x2 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigf32x2,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//alignment externOrStatic const
//"align 8 extern const group_f16x4 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"align 8 static const private_f16x4 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//externOrStatic alignment const
//"extern align 8 const group_f16x4 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,       // storag class 
    BrigExtern,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//"static align 8 const private_f16x4 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigStatic,             // attribute
    0,                      // reserved
    BrigConst,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigf16x4,              // type
    8,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
//no declprefix
// "group_u32 &tmp[2][2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,         // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    4,                      // dim
    0,                      // s_name
    Brigu32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
// "group_s32 &tmp;"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,         // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigs32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
// "group_b32 &tmp[2];"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigGroupSpace,         // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigb32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
// "private_u32 &tmp[2][2];\n"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    4,                      // dim
    0,                      // s_name
    Brigu32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
// "private_s32 &tmp;\n"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    0,                      // dim
    0,                      // s_name
    Brigs32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  },
// "private_b32 &tmp[2];\n"
  {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
    {
    0,                      // c_code
    BrigPrivateSpace,       // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    2,                      // dim
    0,                      // s_name
    Brigb32,                // type
    1,                      // align
     },
  0,                        // d_init
  0,                        // reserved
  }
};

}  // namespace brig
}  // namespace hsa
#endif //GLOBALDECL_TEST_H_

