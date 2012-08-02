#include <string>
#include "brig.h"
#include "brig_buffer.h"
#include "error_reporter_interface.h"
#include "context.h"

extern int yycolno;
extern int yylineno;
namespace hsa {
namespace brig {

// default constructor
Context::Context(void) {
  cbuf = new Buffer();
  dbuf = new Buffer();
  obuf = new Buffer();
  sbuf = new StringBuffer();
  temporary_buf = new Buffer();
  err_reporter = NULL;
  clear_context();
  yycolno = 0;
  yylineno = 1;
}

Context::Context(ErrorReporterInterface* error_reporter) {
  this->err_reporter = error_reporter;
  cbuf = new Buffer();
  dbuf = new Buffer();
  obuf = new Buffer();
  sbuf = new StringBuffer();
  temporary_buf = new Buffer();
  clear_context();

  yycolno = 0;
  yylineno = 1;
}

  /* Error reporter set/get */
ErrorReporterInterface* Context::get_error_reporter(void) const {
  return this->err_reporter;
}

void Context::set_error_reporter(ErrorReporterInterface* error_reporter) {
  this->err_reporter = error_reporter;
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
}

Context::context_error_t Context::update_code_bytes(unsigned char* value,
                                  uint32_t offset,
                                  uint32_t nBytes) {
  Buffer::error_t err = cbuf->modify(value, offset, nBytes);

  if (err == Buffer::SUCCESS)
    return CONTEXT_OK;
  else if (err == Buffer::INVALID_OFFSET)
    return INVALID_OFFSET;
}

Context::context_error_t Context::update_operand_bytes(unsigned char* value,
                                     uint32_t offset,
                                     uint32_t nBytes) {
  Buffer::error_t err = cbuf->modify(value, offset, nBytes);

  if (err == Buffer::SUCCESS)
    return CONTEXT_OK;
  else if (err == Buffer::INVALID_OFFSET)
    return INVALID_OFFSET;
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
  BrigDirectivePad bdp = {0, 0};
  obuf->append(&bdp);
  obuf->append(&bdp);
}

void Context::clear_string_buffer(void) {
  sbuf->clear();
}

void Context::clear_temporary_buffer(void) {
  temporary_buf->clear();
}

void Context::clear_all_buffers(void) {
  clear_code_buffer();
  clear_directive_buffer();
  clear_operand_buffer();
  clear_string_buffer();
  clear_temporary_buffer();
}
void Context::clear_context(void) {
  clear_temporary_context();
  clear_all_buffers();
  func_map.clear();
  operand_map.clear();
  label_o_map.clear();
  label_c_map.clear();
  set_default_values();
}

void Context::clear_temporary_context(void) {
  clear_temporary_buffer();
  set_default_values();
}

void Context::set_default_values(void) {
  machine = BrigESmall;
  profile = BrigEFull;
  ftz = BrigENosftz;
  attribute = BrigNone;
  fbar = 0;
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

BrigPacking16_t Context::get_packing() const {
  return packing;
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

// set context
void Context::set_alu_modifier(BrigAluModifier modifier) {
  this->aluModifier = modifier;
}

void Context::set_symbol_modifier(BrigSymbolModifier modifier) {
  this->symModifier = modifier;
};

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

void Context::set_packing(BrigPacking16_t packing) {
  this->packing = packing;
}
// let context know the location of current operand
void Context::set_operand_loc(char loc) {
  this->operand_loc = loc;
}

BrigoOffset32_t Context::get_current_label_offset(void) const {
  return current_label_offset;
}
// get current instruction offset
BrigcOffset32_t Context::get_current_inst_offset(void) const {
  return current_inst_offset;
}
// get current BrigDirectiveFunction offset
BrigdOffset32_t Context::get_current_bdf_offset(void) const {
  return current_bdf_offset;
}

void Context::set_current_label_offset(BrigoOffset32_t offset) {
  this->current_label_offset = offset;
}

void Context::set_current_inst_offset(BrigcOffset32_t offset) {
  this->current_inst_offset = offset;
}

void Context::set_current_bdf_offset(BrigdOffset32_t offset) {
  this->current_bdf_offset = offset;
}

void Context::insert_to_function_map(std::string key, uint32_t value) {
  if (!func_map.count(key)) {
    func_map[key] = value;
  }
}

int Context::lookup_function_map(std::string key) {
  if (func_map.count(key))
    return func_map[key];
  else
    return -1;
}

void Context::insert_to_operand_map(std::string key, uint32_t value) {
  if (!operand_map.count(key)) {
    operand_map[key] = value;
  }
}

int Context::lookup_operand_map(std::string key) {
  if (operand_map.count(key))
    return operand_map[key];
  else
    return -1;
}

void Context::insert_to_label_o_map(std::string key, uint32_t value) {
  if (!label_o_map.count(key)) {
    label_o_map[key] = value;
  }
}

int Context::lookup_label_o_map(std::string key) {
  if (label_o_map.count(key))
    return label_o_map[key];
  else
    return -1;
}



}  // namespace brig
}  // namespace hsa
