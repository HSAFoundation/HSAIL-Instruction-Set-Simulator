/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_CONTEXT_H_
#define INCLUDE_CONTEXT_H_

#include <string>
#include "brig.h"
#include "brig_buffer.h"


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

    Context(void) {
      cbuf = new Buffer();
      dbuf = new Buffer();
      obuf = new Buffer();
      sbuf = new StringBuffer();
    }

    /* code to append Brig structures to buffers */

    // append code
    template <class T>
    void append_code(const T* item) {
      int code_offset = cbuf->size();
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
      int directive_offset = dbuf->size();
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

    template <class T>
    void append_directive_symbol(const T* item) {
      int directive_offset = dbuf->size();
      dbuf->append(item);
    }


    // append operand
    template <class T>
    void append_operand(const T* item) {
    int operand_offset = obuf->size();
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

    // add a new symbol to .strings section.
    // return the offset to that symbol
    int add_symbol(const std::string& s) {
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
    };

    // lookup offset to a symbol
    // if symbol does not exist, return -1
    int lookup_symbol(const std::string& s) {
      return sbuf->lookup(s);
    };

    /* code to get Brig structures from buffers */

    // get directive at a specific offset
    template <class T>
    context_error_t get_directive(int offset, T* item) {
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
    context_error_t get_code(int offset, T* item) {
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
    context_error_t get_operand(int offset, T* item) {
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

    // get string at an index
    std::string get_string(uint32_t index) {
      return sbuf->at(index);
    }
  // get current offset
    BrigcOffset32_t get_code_offset(void) const {
      return cbuf->size();
    }
    BrigdOffset32_t get_directive_offset(void) const {
      return dbuf->size();
    }
    BrigoOffset32_t get_operand_offset(void) const {
      return obuf->size();
    }
    BrigsOffset32_t get_string_offset(void) const {
      return sbuf->size();
    }

    // functions to get a sequence of values of a certain buffer
    // at a specific offset.
    context_error_t get_directive_bytes(unsigned char* value,
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

    context_error_t get_code_bytes(unsigned char* value,
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

    context_error_t get_operand_bytes(unsigned char* value,
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
    context_error_t update_directive_bytes(unsigned char* value,
                                           uint32_t offset,
                                           uint32_t nBytes) {
      Buffer::error_t err = dbuf->modify(value, offset, nBytes);

      if (err == Buffer::SUCCESS)
        return CONTEXT_OK;
      else if (err == Buffer::INVALID_OFFSET)
        return INVALID_OFFSET;
    }

    context_error_t update_code_bytes(unsigned char* value,
                                      uint32_t offset,
                                      uint32_t nBytes) {
      Buffer::error_t err = cbuf->modify(value, offset, nBytes);

      if (err == Buffer::SUCCESS)
        return CONTEXT_OK;
      else if (err == Buffer::INVALID_OFFSET)
        return INVALID_OFFSET;
    }

    context_error_t update_operand_bytes(unsigned char* value,
                                         uint32_t offset,
                                         uint32_t nBytes) {
      Buffer::error_t err = cbuf->modify(value, offset, nBytes);

      if (err == Buffer::SUCCESS)
        return CONTEXT_OK;
      else if (err == Buffer::INVALID_OFFSET)
        return INVALID_OFFSET;
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

    // clear buffer
    void clear_code_buffer(void) {
      cbuf->clear();
    }

    void clear_directive_buffer(void) {
      dbuf->clear();
    }

    void clear_operand_buffer(void) {
      obuf->clear();
    }

    void clear_string_buffer(void) {
      sbuf->clear();
    }

    void clear_all_buffers(void) {
      cbuf->clear();
      dbuf->clear();
      obuf->clear();
      sbuf->clear();
    }

   bool arg_output;

  BrigcOffset32_t current_bdf_offset;

  private:
    Buffer* cbuf;  // code buffer
    Buffer* dbuf;  // directive buffer
    Buffer* obuf;  // operand buffer
    StringBuffer* sbuf;  // string buffer
};

}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_CONTEXT_H_
