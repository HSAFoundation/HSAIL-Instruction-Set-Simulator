/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_BRIG_BUFFER_H_
#define INCLUDE_BRIG_BUFFER_H_

#include <vector>
#include <stdint.h>
#include <stdlib.h>
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
  const std::vector<unsigned char>& get(void) { return buf_; }
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
    T* get_d(void) {
      if (!dbuf)
        return NULL;
      
      std::vector<unsigned char> temp = dbuf->get();
      int return_size = sizeof(T);
      int begin = temp.size() - return_size; 
      T* ret = new T();
      memcpy((void*)ret,(void*)&temp[begin], return_size);
      return ret;
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
