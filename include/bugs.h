// Work around bug in gcc 4.4's implementation of shared_ptr
// Bug Report: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=42019
// Fixed by Jonathon Wakely 11/20/2009 in libstdc++ revision 154377
#if __GNUC__ == 4 && __GNUC_MINOR__ <= 4

#include <cassert>

#define typeid(X) fake::fake()

namespace fake {
inline std::type_info &fake() {
  assert(false && "Tried to call fake method?!");
}
}

#endif // __GNUC__ == 4 && __GNUC_MINOR__ == 4

