/* Copyright 2012 <MulticorewareInc> */

#include <stdio.h>
#include <string>
#include "brig.h"
#include "brig_buffer.h"
#include "error_reporter_interface.h"
#include "context.h"

extern int yylineno;
namespace hsa {
namespace brig {
Context* Context::ctx = NULL;

Context* Context::get_instance(void) {
     if (ctx == NULL) {
         ctx = new Context();
     }
     return ctx;
}

// default constructor
Context::Context(void) {
  cbuf = new Buffer();
  dbuf = new Buffer();
  obuf = new Buffer();
  sbuf = new StringBuffer();
  err_reporter = NULL;
  yycolno = 0;
  yylineno = 1;
  error_reporter_set = false;
  set_default_values();
}

// default destructor
Context::~Context(void) {
  delete cbuf;
  delete dbuf;
  delete sbuf;
  delete err_reporter;
  delete &func_map;
  delete &func_o_map;
  delete &operand_map;
  delete &label_c_map;
  delete &label_o_map;
  delete ctx;
}

void Context::clear_context(void) {
  clear_all_buffers();
  func_map.clear();
  func_o_map.clear();
  operand_map.clear();
  label_o_map.clear();
  label_c_map.clear();
  symbol_map.clear();
  if (valid_string) {
    free(token_value.string_val);
    valid_string = false;
  }
  set_default_values();
}

void Context::set_default_values(void) {
  machine = BrigELarge;
  profile = BrigEFull;
  ftz = BrigENosftz;
  attribute = BrigNone;
  alignment = 1;
  fbar = 0;
  token_type = UNKNOWN;
  token_to_scan = 0;
  token_value.int_val = 0;
  valid_string = false;
  yycolno = 0;
  yylineno = 1;
  arg_output = false ;
  token_value.format = BrigImageFormatUnknown;
  token_value.order = BrigImageOrderUnknown;
  token_value.storage_class = BrigFlatSpace;
}
  /* Error reporter set/get */
ErrorReporterInterface* Context::get_error_reporter(void) const {
  return this->err_reporter;
}

void Context::set_error_reporter(ErrorReporterInterface* error_reporter) {
  this->err_reporter = error_reporter;
  this->error_reporter_set = true;
}

void Context::set_error(error_code_t error) {
  // try to free string if the token contains string
  if (error_reporter_set)
    err_reporter->report_error(error, yylineno, yycolno);
  else
    printf("Error reporter has not been set up\n");
}

// add a new symbol to .strings section.
// return the offset to that symbol
int Context::add_symbol(const std::string& s) {
  // check if symbol exists
  int offset = sbuf->lookup(s);
  if (offset < 0) {
    // symbol does not exist. Add new
    offset = sbuf->size();  // current offset
    sbuf->append(s);
    return offset;
  } else {
  // symbol already exists in buffer
    return offset;
  }
}

// lookup offset to a symbol
// if symbol does not exist, return -1
int Context::lookup_symbol(const std::string& s) {
  return sbuf->lookup(s);
}

// functions to get a sequence of values of a certain buffer
// at a specific offset.
Context::context_error_t Context::get_directive_bytes(unsigned char* value,
                                    uint32_t offset,
                                    uint32_t nBytes) {
  Buffer::error_t err = dbuf->get_bytes(value, offset, nBytes);

  if (err == Buffer::INVALID_OFFSET)
    return INVALID_OFFSET;
  else if (err == Buffer::EMPTY_BUFFER)
    return EMPTY_BUFFER;
  else
    return CONTEXT_OK;
}

Context::context_error_t Context::get_code_bytes(unsigned char* value,
                               uint32_t offset,
                               uint32_t nBytes) {
  Buffer::error_t err = cbuf->get_bytes(value, offset, nBytes);

  if (err == Buffer::INVALID_OFFSET)
    return INVALID_OFFSET;
  else if (err == Buffer::EMPTY_BUFFER)
    return EMPTY_BUFFER;
  else
    return CONTEXT_OK;
}

Context::context_error_t Context::get_operand_bytes(unsigned char* value,
                                  uint32_t offset,
                                  uint32_t nBytes) {
  Buffer::error_t err = obuf->get_bytes(value, offset, nBytes);

  if (err == Buffer::INVALID_OFFSET)
    return INVALID_OFFSET;
  else if (err == Buffer::EMPTY_BUFFER)
    return EMPTY_BUFFER;
  else
    return CONTEXT_OK;
}


// Modify a number of bytes of a buffer at a specific offset
Context::context_error_t Context::update_directive_bytes(unsigned char* value,
                                       uint32_t offset,
                                       uint32_t nBytes) {
  Buffer::error_t err = dbuf->modify(value, offset, nBytes);

  if (err == Buffer::SUCCESS)
    return CONTEXT_OK;
  else if (err == Buffer::INVALID_OFFSET)
    return INVALID_OFFSET;
  else
    return UNKNOWN_ERROR;
}

Context::context_error_t Context::update_code_bytes(unsigned char* value,
                                  uint32_t offset,
                                  uint32_t nBytes) {
  Buffer::error_t err = cbuf->modify(value, offset, nBytes);

  if (err == Buffer::SUCCESS)
    return CONTEXT_OK;
  else if (err == Buffer::INVALID_OFFSET)
    return INVALID_OFFSET;
  else
    return UNKNOWN_ERROR;
}

Context::context_error_t Context::update_operand_bytes(unsigned char* value,
                                     uint32_t offset,
                                     uint32_t nBytes) {
  Buffer::error_t err = obuf->modify(value, offset, nBytes);

  if (err == Buffer::SUCCESS)
    return CONTEXT_OK;
  else if (err == Buffer::INVALID_OFFSET)
    return INVALID_OFFSET;
  else
    return UNKNOWN_ERROR;
}

// clear buffers
void Context::clear_code_buffer(void) {
  cbuf->clear();
}

void Context::clear_directive_buffer(void) {
  dbuf->clear();
}

void Context::clear_operand_buffer(void) {
  obuf->clear();
  // pad the first 8 bytes with 0
  for (unsigned i = 0; i < sizeof(uint64_t); ++i)
    obuf->append_char(0);
}

void Context::clear_string_buffer(void) {
  sbuf->clear();
}

void Context::clear_all_buffers(void) {
  clear_code_buffer();
  clear_directive_buffer();
  clear_operand_buffer();
  clear_string_buffer();
}

// check context
uint16_t Context::get_alignment() const {
  return alignment;
}

BrigAttribute16_t Context::get_attribute() const {
  return attribute;
}

BrigAluModifier Context::get_alu_modifier() const {
  return aluModifier;
}

BrigSymbolModifier Context::get_symbol_modifier() const {
  return symModifier;
}

BrigMachine16_t Context::get_machine() const {
  return machine;
}

BrigProfile16_t Context::get_profile() const {
  return profile;
}

BrigSftz16_t Context::get_ftz() const {
  return ftz;
}

int Context::get_fbar() const {
  return fbar;
}

BrigDataType16_t Context::get_type() const {
  return type;
}

BrigOpcode32_t Context::get_opcode() const {
  return opcode;
}

char Context::get_operand_loc() const {
  return operand_loc;
}

uint32_t Context::get_dim() const {
  return dim;
}

bool Context::get_isArray() const {
  return is_array;
}

bool Context::get_isBlockNumeric() const {
  return is_blockNumeric;
}

// set context
void Context::set_alu_modifier(BrigAluModifier modifier) {
  this->aluModifier = modifier;
}

void Context::set_symbol_modifier(BrigSymbolModifier modifier) {
  this->symModifier = modifier;
}

void Context::set_attribute(BrigAttribute16_t attrib) {
  this->attribute = attrib;
}

void Context::set_alignment(uint16_t align) {
  this->alignment = align;
}


void Context::set_machine(BrigMachine16_t machine) {
  this->machine = machine;
}

void Context::set_profile(BrigProfile16_t profile) {
  this->profile = profile;
}

void Context::set_ftz(BrigSftz16_t ftz) {
  this->ftz = ftz;
}

void Context::set_fbar(int fbar) {
  this->fbar = fbar;
}

void Context::set_type(BrigDataType16_t type) {
  this->type = type;
}

void Context::set_opcode(BrigOpcode32_t opcode) {
  this->opcode = opcode;
}

// let context know the location of current operand
void Context::set_operand_loc(char loc) {
  this->operand_loc = loc;
}

void Context::set_dim(uint32_t dim) {
  this->dim = dim;
}

void Context::set_isArray(bool is_array) {
  this->is_array = is_array;
}

void Context::set_isBlockNumeric(bool is_blockNumeric) {
  this->is_blockNumeric = is_blockNumeric;
}

// the operationCount of BrigDirectiveFunction add by 1
void Context::update_bdf_operation_count(){
  BrigDirectiveFunction bdf;
  this->get_directive(this->current_bdf_offset, &bdf);
  bdf.operationCount++;

  unsigned char * bdf_charp =
      reinterpret_cast<unsigned char*>(&bdf);
  this->update_directive_bytes(bdf_charp,
                                 this->current_bdf_offset,
                                 sizeof(bdf));
}

}  // namespace brig
}  // namespace hsa
