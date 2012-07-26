/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_BRIG_BUFFER_H_
#define INCLUDE_BRIG_BUFFER_H_
#include <stdint.h>
#include <stdlib.h>
#include <cassert>
#include <iostream>
#include <vector>
#include <string>

namespace hsa {
namespace brig {
class Buffer {
 public:
  Buffer(void) {}
  template <class T>
  void append(const T *item) {
    size_t item_sz = sizeof(T);
    const unsigned char * item_charp =
      reinterpret_cast<const unsigned char*>(item);
    while (item_sz --) {
      buf_.push_back(*item_charp++);
    }
  }
  const std::vector<unsigned char>& get(void) const {
    return buf_;
  }
  size_t size(void) const {
    return buf_.size();
  }
  void append_char(unsigned char c) {
    buf_.push_back(c);
  }
    // Buffer modifier, used for update the buffer contents.
  void modifier(uint16_t offset,
                unsigned char* value,
                uint16_t nuBytes) 
                {
    std::vector<unsigned char>::iterator it;
    if ( buf_.end() < buf_.begin()+offset+nuBytes ) {
      std::cout << "Invalid access to Buffer." << std::endl;
      return ;
    } else {
      for (it = buf_.begin()+offset; it < buf_.begin()+offset+nuBytes; it++) {
        *it = *value;
        value++;
      }
    }
  }

 private:
  std::vector<unsigned char> buf_;
};

class StringBuffer: public Buffer {
 public:
  StringBuffer(void) {}
  void append(const std::string& s) {
    const char *sp = s.c_str();
    for (size_t i = 0; i < s.size(); i++) {
      append_char(*sp++);
    }
    append_char('\0');
  }
  std::string at(uint32_t index) const {
    assert(index < get().size());
    return std::string(reinterpret_cast<const char *>(&(get()[index])));
  }
};

// context for code generation
class Context {
  public:
    Context(void) {
      cbuf = new Buffer();
      dbuf = new Buffer();
      obuf = new Buffer();
      sbuf = new StringBuffer();
      code_offset = 0;
      directive_offset = 0;
      string_offset = 0;
      operand_offset = 0;
    }

    /* code to append Brig structures to buffers */
    // append code
    template <class T>
    void append_c(const T* item) {
      if ((alignment_check(*item) == BrigEAlignment_8) && 
          (code_offset%8)) {
        // need padding to ensure code_offset is a multiple of 8
        BrigDirectivePad bdp = {
          4,  // Size
          BrigEDirectivePad  // type
        };
        cbuf->append(&bdp);
        code_offset+=4;   
      }
      cbuf->append(item);
      code_offset+=sizeof(T);
    }

    // append directive
    template <class T>
    void append_d(const T* item) {
      if ((alignment_check(*item) == BrigEAlignment_8) && 
          (directive_offset%8)) {
        // need padding to ensure code_offset is a multiple of 8
        BrigDirectivePad bdp = {
          4,  // Size
          BrigEDirectivePad  // type
        };
        dbuf->append(&bdp);
        directive_offset+=4;   
      }
      dbuf->append(item);
      directive_offset+=sizeof(T);
    }

    // append operand
    template <class T>
    void append_o(const T* item) {
      if ((alignment_check(*item) == BrigEAlignment_8) && 
          (operand_offset%8)) {
        // need padding to ensure code_offset is a multiple of 8
        BrigDirectivePad bdp = {
          4,  // Size
          BrigEDirectivePad  // type
        };
        obuf->append(&bdp);
        operand_offset+=4;   
      }
      obuf->append(item);
      operand_offset+=sizeof(T);
    }

    // append string
    void append_s(const std::string& item) {
      sbuf->append(item);
      string_offset+-item.length();
    }

    /* code to get Brig structures from buffers */
    // get directive at a specific offset
    template <class T>
    void get_d(T* item, int offset) {
      if (item == NULL) {
        std::cout << "Caller must allocate the memory for item first." << std::endl;
        return;
      }
      
      std::vector<unsigned char> d_buffer = dbuf->get();
      int return_size = sizeof(T);      
      
      if (d_buffer.size() == 0) {
        std::cout << "Empty directive buffer." << std::endl;
        return;
      }
      
      if (d_buffer.end() < d_buffer.begin()+ offset + return_size) {
        std::cout << "Invalid offset value" << std::endl;
        return;
      }

      std::vector<unsigned char>::iterator it;
      unsigned char* temp_ptr = reinterpret_cast<unsigned char*>(item);

      for (it = d_buffer.begin()+offset; it < d_buffer.begin()+offset+return_size; it++)
        *temp_ptr++ = *it;
    }

    // get code
    template <class T>
    void get_c(T* item, int offset) {
      if (item == NULL) {
        std::cout << "Caller must allocate the memory for item first." << std::endl;
        return;
      }
      
      std::vector<unsigned char> c_buffer = cbuf->get();
      int return_size = sizeof(T);
      
      if (c_buffer.size() == 0) {
        std::cout << "Empty code buffer." << std::endl;
        return;
      }
      
      if (c_buffer.end() < c_buffer.begin()+ offset + return_size) {
        std::cout << "Invalid offset value" << std::endl;
        return;
      }

      std::vector<unsigned char>::iterator it;
      unsigned char* temp_ptr = reinterpret_cast<unsigned char*>(item);

      for (it = c_buffer.begin()+offset; it < c_buffer.begin()+offset+return_size; it++)
        *temp_ptr++ = *it;
    }

    // get operand
    template <class T>
    void get_o(T* item, int offset) {
      if (item == NULL) {
        std::cout << "Caller must allocate the memory for item first." << std::endl;
        return;
      }
      
      std::vector<unsigned char> o_buffer = obuf->get();
      int return_size = sizeof(T);
      
      if (o_buffer.size() == 0) {
        std::cout << "Empty operand buffer." << std::endl;
        return;
      }
            
      if (o_buffer.end() < o_buffer.begin()+ offset + return_size) {
        std::cout << "Invalid offset value" << std::endl;
        return;
      }

      std::vector<unsigned char>::iterator it;
      unsigned char* temp_ptr = reinterpret_cast<unsigned char*>(item);

      for (it = o_buffer.begin() + offset; it < o_buffer.begin() + offset + return_size; it++)
        *temp_ptr++ = *it;
    }

    BrigcOffset32_t get_code_offset(void) const { return code_offset; }
    BrigdOffset32_t get_directive_offset(void) const { return directive_offset; }
    BrigoOffset32_t get_operand_offset(void) const { return operand_offset; }
    BrigsOffset32_t get_string_offset(void) const { return string_offset; }
    
    /*---- functions for update a certain buffer by offset. ----*/
        // directive
    void get_d(unsigned char* value, uint16_t offset, uint16_t nuBytes) {
      std::vector<unsigned char> d_buffer = dbuf->get();
      if (d_buffer.size() == 0) {
        std::cout << "Empty directive buffer." << std::endl;
        return;
      }
      std::vector<unsigned char>::iterator it;

      for (it = d_buffer.begin()+offset; 
                it < d_buffer.begin()+offset+nuBytes; it++)
        *value++ = *it;
    }

        //get code
    void get_c(unsigned char* value, uint16_t offset, uint16_t nuBytes) {
      std::vector<unsigned char> c_buffer = cbuf->get();
      if (c_buffer.size() == 0) {
        std::cout << "Empty directive buffer." << std::endl;
        return;
      }
      std::vector<unsigned char>::iterator it;

      for (it = c_buffer.begin()+offset; 
                it < c_buffer.begin()+offset+nuBytes; it++)
        *value++ = *it;
    }
        //get operand
    void get_o(unsigned char* value, uint16_t offset, uint16_t nuBytes) {
      std::vector<unsigned char> o_buffer = obuf->get();
      if (o_buffer.size() == 0) {
        std::cout << "Empty directive buffer." << std::endl;
        return;
      }
      std::vector<unsigned char>::iterator it;

      for (it = o_buffer.begin()+offset; 
                it < o_buffer.begin()+offset+nuBytes; it++)
        *value++ = *it;
    }
    
        // get string at a specific offset
    std::string get_s(uint32_t offset) {
      return sbuf->at(offset);
    }


    /*---- functions for update a certain buffer by offset. ----*/
    void updateBuffer_d (uint16_t offset,
                         unsigned char* value,
                         uint16_t nuBytes) 
                         {
      dbuf->modifier(offset, value, nuBytes);
    }

    void updateBuffer_c (uint16_t offset,
                         unsigned char* value,
                         uint16_t nuBytes) 
                         {
      cbuf->modifier(offset, value, nuBytes);
    }

    void updateBuffer_o (uint16_t offset,
                         unsigned char* value,
                         uint16_t nuBytes) 
                         {
      obuf->modifier(offset, value, nuBytes);
    }
    
    /* helper function to check alignment requirement of each structure */
    template<class T>
    static BrigAlignment alignment_check(T item) {
      switch(item.kind) {
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

  private:
    Buffer* cbuf;  // code buffer
    Buffer* dbuf;  // directive buffer
    Buffer* obuf;  // operand buffer
    StringBuffer* sbuf;  // string buffer
    BrigcOffset32_t code_offset;
    BrigdOffset32_t directive_offset;
    BrigsOffset32_t string_offset;
    BrigoOffset32_t operand_offset;
};
}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_BRIG_BUFFER_H_
