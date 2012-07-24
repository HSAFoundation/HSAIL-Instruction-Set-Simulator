#ifndef INCLUDE_BRIG_BUFFER_H_
#define INCLUDE_BRIG_BUFFER_H_
namespace hsa {
namespace brig {
class buffer {
 public:
  buffer(void) {}
  template <class T>
  void append(const T *item) {
  }
  const std::vector<unsigned char>& get(void) { return buf_; }
 private:
  std::vector<unsigned char> buf_;
};
} // namespace brig
}//namespace hsa
#endif //INCLUDE_BRIG_BUFFER_H_
