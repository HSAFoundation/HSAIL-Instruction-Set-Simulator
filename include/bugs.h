#if !defined(__ICC) && !defined(__clang__) && defined(GCC)
#define __IS_GCC__ 1
#else // !defined(__ICC) && !defined(__clang__) && defined(GCC)
#define __IS_GCC__ 0
#endif // !defined(__ICC) && !defined(__clang__) && defined(GCC)

// Work around bug in gcc 4.4's implementation of shared_ptr
// Bug Report: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=42019
// Fixed by Jonathon Wakely 11/20/2009 in libstdc++ revision 154377
#if __IS_GCC__ && __GNUC__ == 4 && __GNUC_MINOR__ <= 4

#include <cassert>

#define typeid(X) fake::fake()

namespace std {
class type_info;
}

namespace fake {
static std::type_info &fake() {
  assert(false && "Tried to call fake method?!");
}
}

#endif // __IS_GCC__ && __GNUC__ == 4 && __GNUC_MINOR__ <= 4



