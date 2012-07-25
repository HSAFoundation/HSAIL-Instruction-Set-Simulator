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

    // append code
    template <class T>
    void append_c(const T* item) {
      cbuf->append(item);
      code_offset+=sizeof(item);
      // std::cout << "Code offset: " << code_offset << std::endl;
    }

    // append directive
    template <class T>
    void append_d(const T* item) {
      dbuf->append(item);
      directive_offset+=sizeof(item);
      // std::cout << "Directive offset: " << directive_offset << std::endl;
    }

    // append operand
    template <class T>
    void append_o(const T* item) {
      obuf->append(item);
      operand_offset+=sizeof(item);
      // std::cout << "Operand offset: " << operand_offset << std::endl;
    }

    // append string
    void append_s(const std::string& item) {
      sbuf->append(item);
      string_offset+-item.length();
      // std::cout << "String offset: " << string_offset << std::endl;
    }

    // get directive
    template <class T>
    void get_d(T* item) {
      std::vector<unsigned char> d_buffer = dbuf->get();
      if (d_buffer.size() == 0) {
        std::cout << "Empty directive buffer." << std::endl;
        return;
      }
      int return_size = sizeof(T);
      int begin = d_buffer.size() - return_size;

      std::vector<unsigned char>::iterator it;
      unsigned char* temp_ptr = reinterpret_cast<unsigned char*>(item);

      for (it = d_buffer.begin()+begin; it < d_buffer.end(); it++)
        *temp_ptr++ = *it;
    }

    // get code
    template <class T>
    void get_c(T* item) {
      std::vector<unsigned char> c_buffer = cbuf->get();
      if (c_buffer.size() == 0) {
        std::cout << "Empty code buffer." << std::endl;
        return;
      }

      int return_size = sizeof(T);
      int begin = c_buffer.size() - return_size;

      std::vector<unsigned char>::iterator it;
      unsigned char* temp_ptr = reinterpret_cast<unsigned char*>(item);

      for (it = c_buffer.begin()+begin; it < c_buffer.end(); it++)
        *temp_ptr++ = *it;
    }

    // get operand
    template <class T>
    void get_o(T* item) {
      std::vector<unsigned char> o_buffer = obuf->get();
      // std::cout << "Operand buffer size: " << o_buffer.size() << std::endl;
      if (o_buffer.size() == 0) {
        std::cout << "Empty operand buffer." << std::endl;
        return;
      }
      int return_size = sizeof(T);
      int begin = o_buffer.size() - return_size;

      std::vector<unsigned char>::iterator it;
      unsigned char* temp_ptr = reinterpret_cast<unsigned char*>(item);

      for (it = o_buffer.begin()+begin; it < o_buffer.end(); it++)
        *temp_ptr++ = *it;
    }

    // get string at a specific offset
    std::string get_s(uint32_t offset) {
      return sbuf->at(offset);
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
