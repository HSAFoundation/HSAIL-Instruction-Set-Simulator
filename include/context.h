/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_CONTEXT_H_
#define INCLUDE_CONTEXT_H_

#include <map>
#include <string>
#include "brig.h"
#include "brig_buffer.h"
#include "error_reporter_interface.h"
#include "tokens.h"

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
      INVALID_OFFSET,
      UNKNOWN_ERROR
    };

    static Context* get_instance(void);
    /* Error reporter set/get */
    void set_error_reporter(ErrorReporterInterface* error_reporter);
    ErrorReporterInterface* get_error_reporter(void) const;

    void set_error(error_code_t error);
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
    static BrigAlignment alignment_check(T &item) {
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

    // append operand
    template <class T>
    void append_operand(const T* item) {
    uint32_t operand_offset = obuf->size();
      if ((alignment_check(*item) == BrigEAlignment_8) &&
          (operand_offset%8)) {
        // need padding to ensure code_offset is a multiple of 8
        BrigOperandPad bdp = {
          4,                 // Size
          BrigEOperandPad  // type
        };
        obuf->append(&bdp);
      }
      obuf->append(item);
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

      assert(false && "Unreachable");
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

      assert(false && "Unreachable");
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

      assert(false && "Unreachable");
    }

    /* Byte-level manipulators */
    // functions to get a sequence of values of a certain buffer
    // at a specific offset.
    context_error_t get_directive_bytes(char* value,
                                        uint32_t offset,
                                        uint32_t nBytes);

    context_error_t get_code_bytes(char* value,
                                   uint32_t offset,
                                   uint32_t nBytes);

    context_error_t get_operand_bytes(char* value,
                                      uint32_t offset,
                                      uint32_t nBytes);

    context_error_t get_string_bytes(char* value,
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

    //modify the BrigDirectiveFunction filed
    void update_bdf_operation_count() ;

    // clear buffers
    void clear_code_buffer(void);
    void clear_directive_buffer(void);
    void clear_operand_buffer(void);
    void clear_string_buffer(void);
    void clear_all_buffers(void);
    void clear_context(void);
    void set_default_values(void);

    /* Temporary Context Manipulators */
    // check context
    uint16_t get_alignment() const;
    BrigAttribute16_t get_attribute() const;
    BrigAluModifier get_alu_modifier() const;
    uint32_t get_symbol_modifier() const;
    BrigMachine16_t get_machine() const;
    BrigProfile16_t get_profile() const;
    BrigSftz16_t get_ftz() const;
    int get_fbar() const;
    BrigDataType16_t get_type() const;
    char get_operand_loc() const;
    uint32_t get_dim() const;
    bool get_isArray() const;
    bool get_isBlockNumeric() const;

    // set context
    void set_alu_modifier(BrigAluModifier modifier);
    void set_symbol_modifier(BrigSymbolModifier modifier);
	void init_symbol_modifier();
    void set_attribute(BrigAttribute16_t attrib);
    void set_alignment(uint16_t align);
    void set_machine(BrigMachine16_t machine);
    void set_profile(BrigProfile16_t profile);
    void set_ftz(BrigSftz16_t ftz);
    void set_fbar(int fbar);
    void set_type(BrigDataType16_t type);
    // let context know the location of current operand
    void set_operand_loc(char loc);
    void set_dim(uint32_t dim);
    void set_isArray(bool is_array);
    void set_isBlockNumeric(bool is_blockNumeric);

    // get current offset
    BrigcOffset32_t get_code_offset(void) const {return cbuf->size();}
    BrigdOffset32_t get_directive_offset(void) const {return dbuf->size();}
    BrigoOffset32_t get_operand_offset(void) const {return obuf->size();}
    BrigsOffset32_t get_string_offset(void) const {return sbuf->size();}

    //get buffer
    Buffer *get_directive(void) const {return dbuf;}
    Buffer *get_code(void) const {return cbuf;}
    Buffer *get_operands(void) const {return obuf;}
    StringBuffer *get_strings(void) const {return sbuf;}

    bool is_arg_output(void) const {return arg_output;}
    void set_arg_output(bool output) { this->arg_output = output; }

    BrigdOffset32_t current_bdf_offset;
    BrigoOffset32_t current_argList_offset;
    BrigdOffset32_t current_img_offset ;
    BrigdOffset32_t current_samp_offset ;
    BrigdOffset32_t current_argdecl_offset;
    // label_o_map contains the info for OperandLabelRef,
    // label_d_map contains the label that needed in an instruction
    std::map<std::string, BrigoOffset32_t> arg_map;
    std::map<std::string, BrigdOffset32_t> func_map;
    std::map<std::string, BrigoOffset32_t> func_o_map;
    std::map<std::string, BrigoOffset32_t> operand_map;
    std::map<std::string, BrigoOffset32_t> label_o_map;
    std::multimap<std::string, BrigcOffset32_t> label_c_map;
    std::map<std::string, BrigdOffset32_t> symbol_map;

    // contains the type info of functionSignature
    std::vector<BrigDirectiveSignature::BrigProtoType> types;

    unsigned int token_to_scan;
    int               yycolno;
    TerminalType      token_type;
    bool  valid_string;

    union token_val {
      int               int_val;
      float             float_val;
      double            double_val;
      char*             string_val;
      BrigDataType16_t  data_type;
      BrigOpcode32_t    opcode;
      BrigPacking16_t   packing;
      BrigStorageClass32_t storage_class;
      BrigImageOrder32_t order;
      BrigImageFormat32_t format;
      BrigBoundaryMode8_t boundary_mode;
      BrigAddrFilter8_t  filter ;
      bool normalized ;
    } token_value;

    ~Context();
  private:
    /* Buffers */
    Context();
    static Context* ctx;
    Buffer* cbuf;  // code buffer
    Buffer* dbuf;  // directive buffer
    Buffer* obuf;  // operand buffer
    StringBuffer* sbuf;  // string buffer

    /* Error reporter */
    ErrorReporterInterface* err_reporter;  // error reporter

    // context variables
    uint16_t alignment;
    uint32_t symModifier;
    BrigMachine16_t machine;
    BrigProfile16_t profile;
    BrigSftz16_t ftz;
    int fbar;
    BrigAttribute16_t attribute;
    BrigDataType16_t type;
    BrigAluModifier aluModifier;
    char operand_loc;   // 1 -> 5
    bool error_reporter_set;
    bool arg_output;
    uint32_t dim;
    bool is_array;
    bool is_blockNumeric;
};

}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_CONTEXT_H_
