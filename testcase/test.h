/* Copyright 2012 <MulticorewareInc> */

#ifndef TEST_H_
#define TEST_H_

#include <iostream>
#include "gtest/gtest.h"
#include "tokens.h"
#include "lexer.h"
#include "parser.h"
#include "brig.h"
#include "error_reporter.h"
#include "context.h"
#include "parser_wrapper.h"

namespace hsa {
namespace brig {

const int directive_offset=8;
const int code_offset = 8;
const int operand_offset = 8;
const int string_offset = 8;
const int reg_size = 12;
const int regv2_size = 16;
const int regv4_size = 24;
const int immed_size = 24;
const int wavesz_size = 4;
const int address_size = 12;
const int indirect_size = 16;
const int compound_size = 20;
const int labelref_size = 8;
const int functionref_size = 8;
const int opaque_size = 16;
const int brig_directive_version_size=20;
const int brig_directive_sampler_size = 40;
const int brig_directive_image_size = 56;
const int brig_directive_symbol_size = 40;
const int brig_directive_label_size = 12;
const int brig_directive_control_size = 24;
const int brig_directive_extension_size = 12;
const int brig_directive_pragma_size = 12;
const int brig_directive_loc_size = 20;
const int brig_directive_file_size = 16;
const int brig_inst_base_size = 32;
const int brig_inst_bar_size = 36;
const int brig_inst_cvt_size = 40;
const int brig_inst_cmp_size = 44;
const int brig_inst_mod_size = 36;
const int brig_inst_read_size = 40;
const int brig_inst_image_size = 40;
const int brig_inst_mem_size = 36;
const int brig_inst_ldst_size = 44;
const int brig_directive_label_list_size = 20;
const int brig_inst_atomic_image_size = 48;
const int brig_inst_atomic_size = 44;
const int brig_directive_signature_size = 28;
static const int kAlign = 8;
extern int RoundUp8(int bytes);
}  // namespace brig
}  // namespace hsa
#endif //TEST_H_
