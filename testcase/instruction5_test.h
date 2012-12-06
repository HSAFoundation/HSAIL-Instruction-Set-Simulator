/* Copyright 2012 <MulticorewareInc> */

#ifndef INSTRUCTION5_TEST_H_
#define INSTRUCTION5_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class Instruction05Opf2u4 : public ::testing::TestWithParam<int>
{

};

struct Instruction05Opf2u4Test{
  const char* str;
  BrigInstBase ref;
};

struct Instruction05Opf2u4Test instruction5opf2u4_pair[14] = {
//instruction5opf2u4_pair[0]
  {
    "f2u4_u32 $s1, $s2, $s2, $s2, $s2;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size, 
      operand_offset + reg_size,
      operand_offset + reg_size
      }
    }
  },
//instruction5opf2u4_pair[1]
  {
    "f2u4_u32 $s1, $s2, $s3, $s4, $s9;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2,
      operand_offset + reg_size * 3,
      operand_offset + reg_size * 4
      }
    }
  },
//instruction5opf2u4_pair[2]
  {
    "f2u4_u32 $s1, $s2, $s3, $s9, $s3;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2,
      operand_offset + reg_size * 3,
      operand_offset + reg_size * 2
      }
    }
  },
//instruction5opf2u4_pair[3]
  {
    "f2u4_u32 $s1, $s2, $s3, $s9, 1.0f;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2,
      operand_offset + reg_size * 3,
      RoundUp8(operand_offset + reg_size * 4)
      }
    }
  },
//instruction5opf2u4_pair[4]
  {
    "f2u4_u32 $s1, $s2, $s9, 1.0f, 1.0f;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2,
      RoundUp8(operand_offset + reg_size * 3), 
      RoundUp8(RoundUp8(operand_offset + reg_size * 3) + immed_size)
      }
    }
  },
//instruction5opf2u4_pair[5]
  {
    "f2u4_u32 $s1, $s2, 1.0f, 1.0f, 1.0f;",
    {
    brig_inst_base_size,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2), 
      RoundUp8(RoundUp8(operand_offset + reg_size * 2) + immed_size),
      RoundUp8(RoundUp8(RoundUp8(operand_offset + reg_size * 2) + immed_size) + 
        immed_size)
      }
    }
  },
//instruction5opf2u4_pair[6]
  {
    "f2u4_u32 $s1, 1.0f, 1.0f, 1.0f, 1.0f;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      RoundUp8(operand_offset + reg_size), 
      RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size),
      RoundUp8(RoundUp8(RoundUp8(operand_offset + reg_size) + immed_size) + 
        immed_size),
      RoundUp8(RoundUp8(RoundUp8(RoundUp8(operand_offset + reg_size) + 
        immed_size) + immed_size) + immed_size)
      }
    }
  },
//instruction5opf2u4_pair[7]
  {
    "f2u4_u32 $s1, $s2, $s3, $s9, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      operand_offset + reg_size * 3, 
      operand_offset + reg_size * 4
      }
    }
  },
//instruction5opf2u4_pair[8]
  {
    "f2u4_u32 $s1, $s2, $s3, WAVESIZE, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      operand_offset + reg_size * 3, 
      operand_offset + reg_size * 3
      }
    }
  },
//instruction5opf2u4_pair[9]
  {
    "f2u4_u32 $s1, $s2, WAVESIZE, WAVESIZE, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2, 
      operand_offset + reg_size * 2, 
      operand_offset + reg_size * 2
      }
    }
  },
//instruction5opf2u4_pair[10]
  {
    "f2u4_u32 $s1, WAVESIZE, WAVESIZE, WAVESIZE, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size, 
      operand_offset + reg_size, 
      operand_offset + reg_size
      }
    }
  },
//instruction5opf2u4_pair[11]
  {
    "f2u4_u32 $s1, $s2, $s3, 1.0f, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      operand_offset + reg_size * 2,  
      RoundUp8(operand_offset + reg_size * 3),
      RoundUp8(operand_offset + reg_size * 3) + immed_size
      }
    }
  },
//instruction5opf2u4_pair[12]
  {
    "f2u4_u32 $s1, $s2, 1.0f, 1.0f, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2),
      RoundUp8(RoundUp8(operand_offset + reg_size * 2) + immed_size),
      RoundUp8(RoundUp8(operand_offset + reg_size * 2) + immed_size) + 
        immed_size
      }
    }
  },
//instruction5opf2u4_pair[13]
  {
    "f2u4_u32 $s1, $s2, 1.0f, WAVESIZE, WAVESIZE;",
     {
    brig_inst_base_size,
    BrigEInstBase,
    BrigF2u4,
    Brigu32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + reg_size, 
      RoundUp8(operand_offset + reg_size * 2),
      RoundUp8(operand_offset + reg_size * 2) + immed_size,
      RoundUp8(operand_offset + reg_size * 2) + immed_size
      }
    }
  }
};

}  // namespace brig
}  // namespace hsa
#endif //INSTRUCTION5_TEST_H_
