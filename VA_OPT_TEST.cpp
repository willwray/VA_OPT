#if defined(USE_DOCTEST)

  #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
  #include "doctest.h"

#else

  #define TEST_CASE(...) namespace test
  #define CHECK(...) static_assert(__VA_ARGS__,"")
  int main() {}

#endif

#include "VA_OPT.hpp"

#define DATA
#define EMPTY()
#define OBJECT OBJECT2
#define OBJECT2
#define FUNC(x) FUNC2(x)
#define FUNC2(x)

TEST_CASE("IS_EMPTY ") {

CHECK(IS_EMPTY(EMPTY()));
CHECK(IS_EMPTY(DATA EMPTY()));
CHECK(!IS_EMPTY(x EMPTY()));
CHECK(IS_EMPTY(OBJECT EMPTY()));
CHECK(IS_EMPTY(FUNC(z) EMPTY()));

#define FUNC_GEN() ()
#define FUNC_GEN2(x) ()
#define FUNC_GEN3() anything
#define FUNC_GEN4(x) anything

#define FUNC_GEN5() (&)
#define FUNC_GEN6(x) (y)
#define FUNC_GEN7() (y,z)

CHECK(!IS_EMPTY(FUNC_GEN));
CHECK(!IS_EMPTY(FUNC_GEN2));

CHECK(!IS_EMPTY(FUNC_GEN3));
CHECK(!IS_EMPTY(FUNC_GEN4));

CHECK(!IS_EMPTY(FUNC_GEN5));
CHECK(!IS_EMPTY(FUNC_GEN6));
CHECK(!IS_EMPTY(FUNC_GEN7));

#define FUNC_GEN8(x,y) ()
#define FUNC_GEN9(x,y) anything

// Clang rejects with "too few arguments".
// GCC accepts.
// MSVC?
CHECK(!IS_EMPTY(FUNC_GEN8));
CHECK(!IS_EMPTY(FUNC_GEN9));

}
