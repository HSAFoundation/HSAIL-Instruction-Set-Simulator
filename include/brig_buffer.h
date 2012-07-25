/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_BRIG_BUFFER_H_
#define INCLUDE_BRIG_BUFFER_H_
#include <stdint.h>
#include <stdlib.h>

include <iostream>
#include <vector>

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
  const std::vector<unsigned char>& get(void) {
    return buf_;
  }
 private:
  std::vector<unsigned char> buf_;
};


// context for code generation
class Context {
  public:
    Context(void) {
      cbuf = new Buffer();
      dbuf = new Buffer();
      cbuf = new Buffer();
      sbuf = new Buffer();
    }

    // append code
    template <class T>
    void append_c(const T* item) { cbuf->append(item); }

    // append directive
    template <class T>
    void append_d(const T* item) { dbuf->append(item); }

    // append operand
    template <class T>
    void append_o(const T* item) { obuf->append(item); }

    // append string
    template <class T>
    void append_s(const T* item) { sbuf->append(item); }

    // get directive
    template <class T>
    void get_d(T* item) {
      std::vector<unsigned char> d_buffer = dbuf->get();
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
      int return_size = sizeof(T);
      int begin = o_buffer.size() - return_size;

      std::vector<unsigned char>::iterator it;
      unsigned char* temp_ptr = reinterpret_cast<unsigned char*>(item);

      for (it = o_buffer.begin()+begin; it < o_buffer.end(); it++)
        *temp_ptr++ = *it;
    }

  private:
    Buffer* cbuf;  // code buffer
    Buffer* dbuf;  // directive buffer
    Buffer* obuf;  // operand buffer
    Buffer* sbuf;  // string buffer
};
}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_BRIG_BUFFER_H_
