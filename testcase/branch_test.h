/* Copyright 2012 <MulticorewareInc> */

#ifndef BRANCH_TEST_H_
#define BRANCH_TEST_H_
#include "test.h"

namespace hsa {
namespace brig {

class TestBrn: public ::testing::TestWithParam<int>
{

};

struct BranchTest{
  const char* str;
  BrigInstBase ref;
};

struct BranchTest branch_brn_pair[7] = {
//branch_brn_pair[0]
  {
    "brn @label1;",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size, 0, 0, 0}
    }
  },
//branch_brn_pair[1]
  {
    "brn $s0;",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size, 0, 0, 0}
    }
  },
//branch_brn_pair[2]
  {
    "brn $s0, [@label2];",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size,  
      0, 
      0
      }
    }
  },
//branch_brn_pair[3]
  {
    "brn_width(64) $s0;",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size, 0, 0, 0}
    }
  },
//branch_brn_pair[4]
  {
    "brn_width(64) $s0, [@label2];",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
     {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size,  
      0, 
      0
      }
    }
  },
//branch_brn_pair[5]
  {
    "brn_width(all) $s0;",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {operand_offset, operand_offset + immed_size, 0, 0, 0}
    }
  },
//branch_brn_pair[6]
  {
    "brn_width(all) $s0, [@label2];",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size,  
      0, 
      0
      }
    }
  }
};

class TestCbr: public ::testing::TestWithParam<int>
{

};

struct BranchTest branch_cbr_pair[9] = {
//branch_cbr_pair[0]
  {
    "cbr $c0, @label1;",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size,  
      0, 
      0
      }
    }
  },
//branch_cbr_pair[1]
  {
    "cbr_width(64) $c0, @label1;",
      {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size,  
      0, 
      0
      }
    }
  },
//branch_cbr_pair[2]
   {
    "cbr_width(all) $c0, @label1;",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size,  
      0, 
      0
      }
    }
  },
 //branch_cbr_pair[3]
  {
    "cbr $c0, $s0;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size,  
      0, 
      0
      }
    }
  },
//branch_cbr_pair[4]
  {
    "cbr $c0, $s0, [@label3];",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size,  
      operand_offset + immed_size + reg_size * 2, 
      0
      }
    }
  },
//branch_cbr_pair[5]
  {
    "cbr_width(64) $c0, $s0;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size,  
      0, 
      0
      }
    }
  },
 //branch_cbr_pair[6]
  {
    "cbr_width(64) $c0, $s0, [@label3];",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size,  
      operand_offset + immed_size + reg_size * 2, 
      0
      }
    }
  },
//branch_cbr_pair[7]
  {
    "cbr_width(all) $c0, $s0;",
    {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size,  
      0, 
      0
      }
    }
  },
  //branch_cbr_pair[8]
  {
    "cbr_width(all) $c0, $s0, [@label3];",
     {
    brig_inst_base_size,                          
    BrigEInstBase,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size,  
      operand_offset + immed_size + reg_size * 2, 
      0
      }
    }
  }
};

class TestBrnFbr: public ::testing::TestWithParam<int>
{

};

struct BranchFbrTest{
  const char* str;
  BrigInstMod ref;
};

struct BranchFbrTest branch_brn_fbr_pair[7] = {
//branch_brn_fbr_pair[0]
  {
  "brn_fbar @label1;",
      {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      0, 
      0, 
      0
      },
      {1, 0, 0, 0, 0, 1, 0}
    }
  },
//branch_brn_fbr_pair[1]
  {
  "brn_fbar $s0;",
   {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      0, 
      0, 
      0
      },
      {1, 0 ,0, 0, 0, 1, 0}
    }
  },
//branch_brn_fbr_pair[2]
  {
  "brn_fbar $s0, [@label2];",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size, 
      0, 
      0
      },
      {1, 0, 0, 0, 0, 1, 0}
    }
  },
//branch_brn_fbr_pair[3]
  {
  "brn_width(64)_fbar $s0;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      0, 
      0, 
      0
      },
      {1, 0, 0, 0, 0, 1, 0}
    }
  },
//branch_brn_fbr_pair[4]
  {
  "brn_width(64)_fbar $s0, [@label2];",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size, 
      0, 
      0
      },
      {1, 0, 0, 0, 0, 1, 0}
    }
  },
//branch_brn_fbr_pair[5]
  {
  "brn_width(all)_fbar $s0;",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      0, 
      0, 
      0
      },
     {1, 0, 0, 0, 0, 1, 0}
    }
  },
//branch_brn_fbr_pair[6]
  {
  "brn_width(all)_fbar $s0, [@label2];",
    {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigBrn,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size, 
      0, 
      0
      },
      {1, 0, 0, 0, 0, 1, 0}
    }
  }
};

class TestCbrFbr: public ::testing::TestWithParam<int>
{

};


struct BranchFbrTest branch_cbr_fbr_pair[9] = {
//branch_cbr_fbr_pair[0]
  {
  "cbr_fbar $c0, @label1;",
   {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size, 
      0, 
      0
      },
      {1, 0, 0, 0, 0, 1, 0}
    }
  },
//branch_cbr_fbr_pair[1]
  {
  "cbr_fbar $c0, $s0;",
  {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size, 
      0, 
      0
      },
      {1, 0, 0, 0, 0, 1, 0}
    }
  },
//branch_cbr_fbr_pair[2]
  {
  "cbr_fbar $c0, $s0, [@label3];",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size, 
      operand_offset + immed_size + reg_size * 2 , 
      0
      },
      {1, 0, 0, 0, 0, 1, 0}
    }
  },
//branch_cbr_fbr_pair[3]
  {
  "cbr_width(64)_fbar $c0, @label1;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size, 
      0, 
      0
      },
      {1, 0, 0, 0, 0, 1, 0}
    }
  },
//branch_cbr_fbr_pair[4]
  {
  "cbr_width(all)_fbar $c0, @label1;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size, 
      0, 
      0
      },
      {1, 0, 0, 0, 0, 1, 0}
    }
  },
//branch_cbr_fbr_pair[5]
  {
  "cbr_width(64)_fbar $c0, $s0;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size, 
      0, 
      0
      },
      {1, 0, 0, 0, 0, 1, 0}
    }
  },
//branch_cbr_fbr_pair[6]
  {
  "cbr_width(64)_fbar $c0, $s0, [@label3];",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size, 
      operand_offset + immed_size + reg_size * 2 , 
      0
      },
     {1, 0, 0, 0, 0, 1, 0}
    }
  },
//branch_cbr_fbr_pair[7]
  {
  "cbr_width(all)_fbar $c0, $s0, [@label3];",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size, 
      operand_offset + immed_size + reg_size * 2 , 
      0
      },
      {1, 0, 0, 0, 0, 1, 0}
    }
  },  
//branch_cbr_fbr_pair[8]
  {
  "cbr_width(all)_fbar $c0, $s0;",
     {
    brig_inst_mod_size,
    BrigEInstMod,
    BrigCbr,
    Brigb32,
    BrigNoPacking,
    {
      operand_offset, 
      operand_offset + immed_size, 
      operand_offset + immed_size + reg_size, 
      0, 
      0
      },
      {1, 0, 0, 0, 0, 1, 0}
    }
  }
};

}  // namespace brig
}  // namespace hsa
#endif //BRANCH_TEST_H_
