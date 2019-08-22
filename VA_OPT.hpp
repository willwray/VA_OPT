/*
  VA_OPT.hpp
  ===============

  Preprocessor macros for __VA_OPT__ feature detection plus related
  emptiness-testing utilities with back-compatibility versions when
  C++20 __VA_OPT__ support is not auto detected (these versions are
  not perfect but will work fine in most use cases, details below).

  VA_OPT_SUPPORT(?)
    1 if __VA_OPT__ support is detected else 0.

  Without VA_OPT_SUPPORT the following macros can fail if the ARG
  is a function-like macro name. If the function macro takes args
  then a compile error may result 'too few args'.

  VA_OPT
    Compatibility macro expanding to __VA_OPT__ if supported,
    else implemented as IFN(__VA_ARGS__)

  IFN(...)
    Expands trailing (X) to X only when the ARGS... are not empty.
    I.e. it is like VA_OPT but with explicit ARGS and when ARGS is
    empty it 'eats' any trailing clause and so expands to nothing.
    IFN has the same false positive as IS_EMPTY below since it is
    implemented the same way

  IS_EMPTY(...)
    1 if the ARG is empty else 0.
    Adapted from BOOST_VMD_IS_EMPTY
    https://www.boost.org/doc/libs/1_71_0/libs/vmd/doc/html/BOOST_VMD_IS_EMPTY.html
    https://github.com/boostorg/vmd/include/boost/vmd/is_empty.hpp

  IBP(...)
    IS_BEGIN_PARENS. Adapted from BOOST_PP_IS_BEGIN_PARENS.
    https://www.boost.org/doc/libs/1_71_0/libs/preprocessor/doc/index.html
    https://github.com/boostorg/preprocessor
    include/boost/preprocessor/punctuation/is_begin_parens.hpp

  Props to Paul Mensonides and Edward Diener, pre-pro pioneers.
*/

// VA_OPT_SUPPORT helper macros for __VA_OPT__ feature detection
// Adapted from https://stackoverflow.com/a/48045656/7443483
// VA_EMPTY works only if __VA_OPT__ is supported, else always -> 1
#define VA_ARG1(A0,A1,...) A1
#define VA_EMPTY(...) VA_ARG1(__VA_OPT__(,)0,1,)

#define VA_OPT_SUPPORT ! VA_EMPTY

#if VA_OPT_SUPPORT(?)

#  define VA_OPT VA_CAT(__VA_,OPT__)
#  define IFN(...) VA_EAT __VA_OPT__(()VA_IDENT)
#  define IS_EMPTY(...) VA_EMPTY(__VA_ARGS__)

#else

#  define VA_OPT IFN(VA_CAT(__VA_,ARGS__))
#  define IFN(...) IFP(IBP(__VA_ARGS__))(IFN_GEN_ID,IFN_EAT_OR_ID)(__VA_ARGS__)
#  define IS_EMPTY(...) IFP(IBP(__VA_ARGS__))(IE_GEN_0,IE_IBP)(__VA_ARGS__)

#endif

#define VA_CAT(A,...) A##__VA_ARGS__
#define VA_EAT(...)
#define VA_IDENT(...) __VA_ARGS__

// IFP helper macros to test IBP for IFN and IS_EMPTY
#define IFP_0(T,...) __VA_ARGS__
#define IFP_1(T,...) T

#define IFP_CAT(A,B) A##B
#define IFP(BP) IFP_CAT(IFP_,BP)

// IS_BEGIN_PAREN helper macros adapted from BOOST VMD
#define IBP_CAT_(A,...) A##__VA_ARGS__
#define IBP_CAT(A,...) IBP_CAT_(A,__VA_ARGS__)

#define IBP_ARG0_(A,...) A
#define IBP_ARG0(...) IBP_ARG0_(__VA_ARGS__)

#define IBP_IS_ARGS(...) 1

#define IBP_IS_ARGS_1 1,
#define IBP_IS_ARGS_IBP_IS_ARGS 0,

// IBP IS_BEGIN_PAREN returns 1 or 0 if ... ARGS is parenthesised
#define IBP(...) IBP_ARG0(IBP_CAT(IBP_IS_ARGS_, IBP_IS_ARGS __VA_ARGS__))

// IFN and IF_EMPTY helpers without __VA_OPT__ support
#if ! VA_OPT_SUPPORT(?)
   // IS_EMPTY helper macros, depend on IBP
#  define IE_REDUCE_IBP(...) ()
#  define IE_GEN_0(...) 0
#  define IE_IBP(...) IBP(IE_REDUCE_IBP __VA_ARGS__ ())

   // IFN helper macros
#  define IFN_IS_ARGS_1 VA_EAT,
#  define IFN_IS_ARGS_IBP_IS_ARGS VA_IDENT,

#  define IFN_(...) IBP_ARG0(IBP_CAT(IFN_IS_ARGS_, IBP_IS_ARGS __VA_ARGS__))

#  define IFN_EAT_OR_ID(...) IFN_(IE_REDUCE_IBP __VA_ARGS__ ())
#  define IFN_GEN_ID(...) VA_IDENT

#endif // IFN and IF_EMPTY defs
