/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_CONTEXT_H_
#define INCLUDE_CONTEXT_H_

#include <map>
#include <string>
#include "brig.h"
#include "brig_buffer.h"
#include "error_reporter_interface.h"

extern int yycolno;
extern int yylineno;
namespace hsa {
namespace brig {
// context for code generation
class Context {
  public:
    enum context_error_t {
      CONTEXT_OK = 0,
      INVALID_POINTER = 1,
      EMPTY_BUFFER,
      INVALID_OFFSET
    };

    /* Constructors */
    Context();
    explicit Context(ErrorReporterInterface* error_reporter);

    /* Error reporter set/get */
    void set_error_reporter(ErrorReporterInterface* error_reporter);
    ErrorReporterInterface* get_error_reporter(void) const;

    /* string buffer manipulators */
    // add a new symbol to .strings section.
    // return the offset to that symbol
    int add_symbol(const std::string& s);

    // lookup offset to a symbol
    // if symbol does not exist, return -1
    int lookup_symbol(const std::string& s);

    // get string at an index
    std::string get_string(uint32_t index) {
      return sbuf->at(index);
    }

    /* helper function to check alignment requirement of each structure */
    template<class T>
    static BrigAlignment alignment_check(T item) {
      switch (item.kind) {
        // directive
        case BrigEDirectiveBlockNumeric:
        case BrigEDirectiveInit:
        // operand
        case BrigEOperandImmed:
          return BrigEAlignment_8;
        default:
          return BrigEAlignment_4;
      }
    }

    /* code to append Brig structures to buffers */

    // append code
    template <class T>
    void append_code(const T* item) {
      uint32_t code_offset = cbuf->size();
      if ((alignment_check(*item) == BrigEAlignment_8) &&
          (code_offset%8)) {
        // need padding to ensure code_offset is a multiple of 8
        BrigDirectivePad bdp = {
          4,                 // Size
          BrigEDirectivePad  // type
        };
        cbuf->append(&bdp);
      }
      cbuf->append(item);
    }

    // append directive
    template <class T>
    void append_directive(const T* item) {
      uint32_t directive_offset = dbuf->size();
      if ((alignment_check(*item) == BrigEAlignment_8) &&
          (directive_offset%8)) {
        // need padding to ensure code_offset is a multiple of 8
        BrigDirectivePad bdp = {
          4,                 // Size
          BrigEDirectivePad  // type
        };
        dbuf->append(&bdp);
      }
      dbuf->append(item);
    }

    // use to append a BrigDirectiveSymbol structs
    // (does not contain a .kind field ? )
    void append_directive_symbol(const BrigDirectiveSymbol* item) {
      uint32_t directive_offset = dbuf->size();
      dbuf->append(item);
    }

    // append operand
    template <class T>
    void append_operand(const T* item) {
    uint32_t operand_offset = obuf->size();
      if ((alignment_check(*item) == BrigEAlignment_8) &&
          (operand_offset%8)) {
        // need padding to ensure code_offset is a multiple of 8
        BrigDirectivePad bdp = {
          4,                 // Size
          BrigEDirectivePad  // type
        };
        obuf->append(&bdp);
      }
      obuf->append(item);
    }

    // append temporary structs
    template <class T>
    void append_temporary_struct(const T* item) {
      temporary_buf -> append(item);
    }

    /* code to get Brig structures from buffers */
    // get temporary structs from temporary buffer
    template <class T>
    context_error_t get_temporary_struct(uint32_t offset, T* item) {
      // check for valid pointer
      if (item == NULL)
        return INVALID_POINTER;

      Buffer::error_t result = temporary_buf->get(offset, item);

      if (result == Buffer::INVALID_OFFSET)
        return INVALID_OFFSET;
      else if (result == Buffer::EMPTY_BUFFER)
        return EMPTY_BUFFER;
      else if (result == Buffer::SUCCESS)
        return CONTEXT_OK;
    }

    // get directive at a specific offset
    template <class T>
    context_error_t get_directive(uint32_t offset, T* item) {
      // check for valid pointer
      if (item == NULL)
        return INVALID_POINTER;

      Buffer::error_t result = dbuf->get(offset, item);

      if (result == Buffer::INVALID_OFFSET)
        return INVALID_OFFSET;
      else if (result == Buffer::EMPTY_BUFFER)
        return EMPTY_BUFFER;
      else if (result == Buffer::SUCCESS)
        return CONTEXT_OK;
    }

    // get code at a specific offset
    template <class T>
    context_error_t get_code(uint32_t offset, T* item) {
      // check for valid pointer
      if (item == NULL)
        return INVALID_POINTER;

      Buffer::error_t result = cbuf->get(offset, item);

      if (result == Buffer::INVALID_OFFSET)
        return INVALID_OFFSET;
      else if (result == Buffer::EMPTY_BUFFER)
        return EMPTY_BUFFER;
      else if (result == Buffer::SUCCESS)
        return CONTEXT_OK;
    }

    // get operand at a specific offset
    template <class T>
    context_error_t get_operand(uint32_t offset, T* item) {
      // check for valid pointer
      if (item == NULL)
        return INVALID_POINTER;

      Buffer::error_t result = obuf->get(offset, item);

      if (result == Buffer::INVALID_OFFSET)
        return INVALID_OFFSET;
       else if (result == Buffer::EMPTY_BUFFER)
        return EMPTY_BUFFER;
      else if (result == Buffer::SUCCESS)
        return CONTEXT_OK;
    }

    /* Byte-level manipulators */
    // functions to get a sequence of values of a certain buffer
    // at a specific offset.
    context_error_t get_directive_bytes(unsigned char* value,
                                        uint32_t offset,
                                        uint32_t nBytes);

    context_error_t get_code_bytes(unsigned char* value,
                                   uint32_t offset,
                                   uint32_t nBytes);

    context_error_t get_operand_bytes(unsigned char* value,
                                      uint32_t offset,
                                      uint32_t nBytes);

    // Modify a number of bytes of a buffer at a specific offset
    context_error_t update_directive_bytes(unsigned char* value,
                                           uint32_t offset,
                                           uint32_t nBytes);

    context_error_t update_code_bytes(unsigned char* value,
                                      uint32_t offset,
                                      uint32_t nBytes);

    context_error_t update_operand_bytes(unsigned char* value,
                                         uint32_t offset,
                                         uint32_t nBytes);


    // clear buffers
    void clear_code_buffer(void);
    void clear_directive_buffer(void);
    void clear_operand_buffer(void);
    void clear_string_buffer(void);
    void clear_temporary_buffer(void);
    void clear_all_buffers(void);
    void clear_context(void);
    void clear_temporary_context(void);
    void set_default_values(void);

    /* Temporary Context Manipulators */
    // check context
    uint16_t get_alignment() const;
    BrigAttribute16_t get_attribute() const;
    BrigAluModifier get_alu_modifier() const;
    BrigSymbolModifier get_symbol_modifier() const;
    BrigPacking16_t get_packing() const;
    BrigMachine16_t get_machine() const;
    BrigProfile16_t get_profile() const;
    BrigSftz16_t get_ftz() const;
    int get_fbar() const;
    BrigDataType16_t get_type() const;
    BrigOpcode32_t get_opcode() const;
    char get_operand_loc() const;

    // set context
    void set_alu_modifier(BrigAluModifier modifier);
    void set_symbol_modifier(BrigSymbolModifier modifier);
    void set_attribute(BrigAttribute16_t attrib);
    void set_alignment(uint16_t align);
    void set_machine(BrigMachine16_t machine);
    void set_profile(BrigProfile16_t profile);
    void set_ftz(BrigSftz16_t ftz);
    void set_fbar(int fbar);
    void set_type(BrigDataType16_t type);
    void set_opcode(BrigOpcode32_t opcode);
    void set_packing(BrigPacking16_t packing);
    // let context know the location of current operand
    void set_operand_loc(char loc);

    // get current offset
    BrigcOffset32_t get_code_offset(void) const {return cbuf->size();}
    BrigdOffset32_t get_directive_offset(void) const {return dbuf->size();}
    BrigoOffset32_t get_operand_offset(void) const {return obuf->size();}
    BrigsOffset32_t get_string_offset(void) const {return sbuf->size();}
    uint32_t get_temporary_offset(void) const {return temporary_buf->size();}

    BrigoOffset32_t get_current_label_offset(void) const;
    void set_current_label_offset(BrigoOffset32_t offset);
    // get current instruction offset
    BrigcOffset32_t get_current_inst_offset(void) const;
    void set_current_inst_offset(BrigcOffset32_t offset);
    // get current BrigDirectiveFunction offset
    BrigdOffset32_t get_current_bdf_offset(void) const;
    void set_current_bdf_offset(BrigdOffset32_t offset);

    bool is_arg_output(void) const {return arg_output;}
    void set_arg_output(bool output) { this->arg_output = output; }

    void insert_to_function_map(std::string key, uint32_t value);
    int lookup_function_map(std::string key);

    void insert_to_operand_map(std::string key, uint32_t value);
    int lookup_operand_map(std::string key);

    void insert_to_label_o_map(std::string key, uint32_t value);
    int lookup_label_o_map(std::string key);


    BrigoOffset32_t current_label_offset;
    BrigcOffset32_t current_inst_offset;
    BrigdOffset32_t current_bdf_offset;
    // label_o_map contains the info for OperandLabelRef,
    // label_d_map contains the label that needed in a instruction
    std::map<std::string, BrigdOffset32_t> func_map;
    std::map<std::string, BrigoOffset32_t> func_o_map;
    std::map<std::string, BrigoOffset32_t> operand_map;
    std::map<std::string, BrigoOffset32_t> label_o_map;
    std::multimap<std::string, BrigcOffset32_t> label_c_map;

  private:
    /* Buffers */
    Buffer* cbuf;  // code buffer
    Buffer* dbuf;  // directive buffer
    Buffer* obuf;  // operand buffer
    Buffer* temporary_buf;  // a buffer to put temporary structs
    StringBuffer* sbuf;  // string buffer

    /* Error reporter */
    ErrorReporterInterface* err_reporter;  // error reporter

    // context variables
    uint16_t alignment;
    BrigSymbolModifier symModifier;
    BrigMachine16_t machine;
    BrigProfile16_t profile;
    BrigSftz16_t ftz;
    int fbar;
    BrigAttribute16_t attribute;
    BrigDataType16_t type;
    BrigOpcode32_t opcode;
    BrigAluModifier aluModifier;
    BrigPacking16_t packing;
    char operand_loc;   // 1 -> 5


    bool arg_output;


};

}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_CONTEXT_H_
