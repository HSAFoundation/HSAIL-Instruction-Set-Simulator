/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_BRIG_BUFFER_H_
#define INCLUDE_BRIG_BUFFER_H_
#include <stdint.h>
#include <stdlib.h>
#include <cassert>
#include <vector>
#include <string>

namespace hsa {
namespace brig {
class Buffer {
 public:
  enum error_t {
    SUCCESS = 0,
    INVALID_OFFSET,
    EMPTY_BUFFER
    };

  Buffer(void) {}

  // append an item
  template <class T> void append(const T *item) {
    size_t item_sz = item->size;
    assert(item_sz && "Appending a zero size element?!");
    const unsigned char * item_charp =
      reinterpret_cast<const unsigned char*>(item);
    while (item_sz --) {
      buf_.push_back(*item_charp++);
    }
  }

  // pop an item
  template <class T> void pop(const T *item) {
    size_t item_sz = item->size;
    assert(item_sz && "pop a zero size element?!");
    while (item_sz --) {
      buf_.pop_back();
    }
  }

  // get the whole buffer as a vector
  const std::vector<char> &get(void) const {
    return buf_;
  }

  std::vector<char> &get(void) {
    return buf_;
  }

  // get a specific number of bytes from a specific offset
  error_t get_bytes(char* value, uint32_t offset, uint32_t nBytes) {
    if (buf_.size() == 0)
      return EMPTY_BUFFER;
    if (buf_.end() < buf_.begin()+ offset + nBytes)
      return INVALID_OFFSET;

    std::vector<char>::iterator it;
    for (it = buf_.begin()+offset;
         it < buf_.begin()+offset+nBytes;
         it++)
      *value++ = *it;
    return SUCCESS;
  }

  // get a specific item at specific offset
  // input: offset value
  //        a pointer to allocated memory for returned item
  //
  // output: the specific item
  // return 0 if succeed
  // return -1 if fail
  template <class T>
  error_t get(uint32_t offset, T* item) {
    int return_size = sizeof(T);

    if (buf_.size() == 0)
      return EMPTY_BUFFER;
    if (buf_.end() < buf_.begin()+ offset + return_size)
      return INVALID_OFFSET;


    std::vector<char>::iterator it;
    unsigned char* temp_ptr = reinterpret_cast<unsigned char*>(item);

    for (it = buf_.begin()+offset;
         it < buf_.begin()+offset+return_size;
         it++)
      *temp_ptr++ = *it;

    return SUCCESS;
  }

  // get buffer size
  size_t size(void) const {
    return buf_.size();
  }

  // append char
  void append_char(unsigned char c) {
    buf_.push_back(c);
  }
  // Buffer modify, used for update the buffer contents.
  error_t modify(unsigned char* value,
                uint32_t offset,
                uint32_t nBytes) {
    std::vector<char>::iterator it;
    if ( buf_.end() < buf_.begin()+offset+nBytes ) {
      return INVALID_OFFSET;
    } else {
      for (it = buf_.begin()+offset; it < buf_.begin()+offset+nBytes; it++) {
        *it = *value;
        value++;
      }
      return SUCCESS;
    }
  }

  void clear(void) {
    buf_.clear();
  }


 private:
  std::vector<char> buf_;
};

class StringBuffer: public Buffer {
 public:
  StringBuffer(void) {}

  // append string
  void append(const std::string& s) {
    const char *sp = s.c_str();
    for (size_t i = 0; i < s.size(); i++) {
      append_char(*sp++);
    }
    append_char('\0');
  }

  // get string at an index
  std::string at(uint32_t index) const {
    assert(index < get().size());
    return std::string(reinterpret_cast<const char *>(&(get()[index])));
  }

  // look up offset to a string
  // return the offset if the string exists in buffer
  // return -1 if string does not exist
  int lookup(const std::string& s) {
    std::string temp;
    uint32_t index = 0;
    while (index < size()) {
      temp = at(index);
      if (!temp.compare(s))
        return index;
      else
        index+=temp.length()+1;
    }
    return -1;
  };
};

}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_BRIG_BUFFER_H_
