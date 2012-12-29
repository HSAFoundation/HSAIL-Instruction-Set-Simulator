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

#define Destructor __attribute__((destructor))

Destructor static void fini(void) {
  delete Context::get_instance();
}

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
    
  initialize_all_fields();
  for(unsigned i = 0; i < 8; ++i) cbuf->append_char(0);
  for(unsigned i = 0; i < 8; ++i) dbuf->append_char(0);
  for(unsigned i = 0; i < 8; ++i) obuf->append_char(0);
  for(unsigned i = 0; i < 8; ++i) sbuf->append_char(0);
}

// default destructor
Context::~Context(void) {
  delete cbuf;
  delete dbuf;
  delete obuf;
  delete sbuf;
  delete err_reporter;
}

void Context::clear_context(void) {

  clear_all_buffers();
  func_map.clear();
  func_o_map.clear();
  operand_map.clear();
  local_label_map.clear();
  arg_label_map.clear();
  label_d_map.clear();
  global_symbol_map.clear();
  local_symbol_map.clear();
  arg_symbol_map.clear();
  arg_map.clear();
  if (valid_string) {
    free(token_value.string_val);
    valid_string = false;
  }
  initialize_all_fields();  
}

void Context::initialize_all_fields(void) {
  /* Lexer Values */
  token_type = UNKNOWN;
  token_to_scan = 0;
  token_value.int_val = 0;
  valid_string = false;
  isFirstVersion = true;
  yycolno = 0;
  yylineno = 1;
  
  machine = BrigELarge;
  profile = BrigEFull;
  last_directive_offset = 0;
  current_bdf_offset = 0;
  major = 0;
  minor = 0;
    
  attribute = BrigNone;
  alignment = 0;
  symModifier = 0;
  aluModifier.floatOrInt = 0;
  aluModifier.rounding = 0;
  aluModifier.valid = 0;
  aluModifier.ftz = 0;
  aluModifier.approx = 0;
  aluModifier.fbar = 0;
  aluModifier.reserved = 0;
  dim = 0;
  type = Brigb32;
  is_array = false;
  storageClass = BrigFlatSpace;
}
  /* Error reporter set/get */
ErrorReporterInterface* Context::get_error_reporter(void) const {
  return this->err_reporter;
}

void Context::set_error_reporter(ErrorReporterInterface* error_reporter) {
  this->err_reporter = error_reporter;  
}

void Context::set_error(error_code_t error) {
  // try to free string if the token contains string
  if (err_reporter)
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
Context::context_error_t Context::get_directive_bytes(char* value,
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

Context::context_error_t Context::get_code_bytes(char* value,
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

Context::context_error_t Context::get_operand_bytes(char* value,
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

Context::context_error_t Context::update_last_directive(unsigned char* value,
                                       uint32_t nBytes) {
  uint32_t offset = this->last_directive_offset;                                     
  Buffer::error_t err = dbuf->modify(value, offset, nBytes);

  if (err == Buffer::SUCCESS)
    return CONTEXT_OK;
  else if (err == Buffer::INVALID_OFFSET)
    return INVALID_OFFSET;
  else
    return UNKNOWN_ERROR;
}


Context::context_error_t Context::get_string_bytes(char* value,
                                  uint32_t offset,
                                  uint32_t nBytes) {
  Buffer::error_t err = sbuf->get_bytes(value, offset, nBytes);

  if (err == Buffer::INVALID_OFFSET)
    return INVALID_OFFSET;
  else if (err == Buffer::EMPTY_BUFFER)
    return EMPTY_BUFFER;
  else
    return CONTEXT_OK;
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
}

void Context::clear_string_buffer(void) {
  sbuf->clear();
}

void Context::clear_all_buffers(void) {
  clear_code_buffer();
  clear_directive_buffer();
  clear_operand_buffer();
  clear_string_buffer();
  for(unsigned i = 0; i < 8; ++i) cbuf->append_char(0);
  for(unsigned i = 0; i < 8; ++i) dbuf->append_char(0);
  for(unsigned i = 0; i < 8; ++i) obuf->append_char(0);
  for(unsigned i = 0; i < 8; ++i) sbuf->append_char(0);
}

void Context::initialize_statement_fields(void){
  attribute = BrigNone;
  alignment = 0;
  symModifier = 0;
  type = Brigb32;
  aluModifier.floatOrInt = 0;
  aluModifier.rounding = 0;
  aluModifier.valid = 0;
  aluModifier.ftz = 0;
  aluModifier.approx = 0;
  aluModifier.fbar = 0;
  aluModifier.reserved = 0;
  dim = 0;
  is_array = false;
  storageClass = BrigFlatSpace;
}

BrigdOffset32_t Context::get_symbol(std::string name) {
  if (arg_symbol_map.count(name)) {
    return arg_symbol_map[name];
  } else if (local_symbol_map.count(name)) {
    return local_symbol_map[name];
  } else if (global_symbol_map.count(name)) {
    return global_symbol_map[name];
  }
  return 0;
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
