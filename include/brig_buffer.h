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
    while(item_sz --) {
      buf_.push_back(*item_charp++);
    }
  }
  const std::vector<unsigned char>& get(void) { return buf_; }
 private:
  std::vector<unsigned char> buf_;
};
} // namespace brig
}//namespace hsa
#endif //INCLUDE_BRIG_BUFFER_H_
