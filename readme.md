# **`VA_OPT`**

## Emptiness testing for C++ Preprocessor macro arguments

<details><summary>Copyright &copy; 2019 Will Wray. Distributed under the Boost Software License, V1.0</summary>

### **Boost Software License** - Version 1.0 - August 17th, 2003

```txt
Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
```

[![License](https://img.shields.io/badge/license-boost%201.0-blue.svg)](https://www.boost.org/LICENSE_1_0.txt)

Also at [boost.org](http://www.boost.org/LICENSE_1_0.txt) and accompanying file [LICENSE_1_0.txt](LICENSE_1_0.txt)

</details>

| Linux Travis| Windows Appveyor|
| :---: | :---: |
|gcc-9, clang-7|MSVC 19.21.27702.2|
| [![Build Status](https://travis-ci.org/willwray/VA_OPT.svg?branch=master)](https://travis-ci.org/willwray/VA_OPT) | [![Build status](https://ci.appveyor.com/api/projects/status/oy5q387u5c9pt0rx?svg=true)](https://ci.appveyor.com/project/willwray/VA_OPT) |

----

### **`VA_OPT.hpp`**

C++ preprocessor utilities for testing emptiness of macro arguments  
and for expansion conditional on the emptiness of ARGS.

 1. For portability
 2. For handy utils that wrap `__VA_OPT__`

This is mostly a 'polyfill' library providing `__VA_OPT__`alike  functionality  
where support is not detected (so either not present or not selected).  
The utilities are also useful `__VA_OPT__` wrappers in their own right.

<details>

<summary><code>__VA_OPT__</code> details</summary>

----

C++20 introduces a special function-like macro `__VA_OPT__`  
for use only within a variadic macro's replacement list, in which;
>IF `__VA_ARGS__` is empty  
THEN `__VA_OPT__(X...)` 'vanishes' - i.e. expands to nothing -  
ELSE it expands to `X...`

```c++
#define VARIADIC_MACRO(...) __VA_OPT__(NOT EMPTY)
VARIADIC_MACRO()  // Expands to nothing
VARIADIC_MACRO(?) // Expands to NOT EMPTY
```

----

</details>

Prior to C++20's `__VA_OPT__` there is no reliable test for empty ARGS.

Such 'emptiness' testing is so useful that it shouldn't be limited to C++20.  
Unfortunately, some C++20 compliant compilers hide the feature behind  
a `std=c++2a` flag and there is also no `std` feature-test for `__VA_OPT__`.

This `VA_OPT` library auto-detects preprocessor support for `__VA_OPT__`  
and, if detected, uses it as a robust way to implement:

* **`IS_EMPTY(...)`**
  * 1 if the ... ARGS is empty else 0.

* **`IFN(...)`**
  * IF `...` ARGS are not empty  
    THEN a trailing 'paren expression' `(X)` is 'deparenthesized' to `X`  
    ELSE the trailing paren expression `(X)` is consumed.

    `IFN(ARGS)` is like `__VA_OPT__` but with explicit (ARGS)  
    in place of an implicit `__VA_ARGS__` check.

* **`IFE(...)`**
  * IF `...` ARGS is empty  
    THEN expand trailing 'paren expression' `(X)` to `X`  
    ELSE consume the trailing paren expression.

* **`IFNE(...)(N,E...)`**
  * IF `...` ARGS are not empty  
    THEN expands to `N`  
    ELSE expands to `E...`

If `__VA_OPT__` support is **NOT** detected then the above macros  
are provided using a fallback method (with some failing edge cases).

## Platform note:

Current Sept 2019 `__VA_OPT__` support:

 * Clang 6 introduced it, enabled with `-std=c++2a`  
(don't know how best to enable it with earlier `-std`)
 * GCC 8 has partial support (no `#__VA_OPT__`)  
it is enabled by default without `-std=++2a` but,  
if an earlier `-std` flag is supplied along with `-pedantic`  
then it warns "`__VA_OPT__` is not available until C++2a"  
(don't know how to silence that without dropping `-pedantic`).
 * MSVC's experimental preprocessor is required to compile VA_OPT;  
use /experimental:preprocessor flag in vs2017 or vs2019.  
There's no `__VA_OPT__` support yet but conformance is improving.  
(I do not intend to support non-conforming preprocessors.)

## Credits

Props to pre-pro pioneers, particularly Paul Mensonides.

The fallback techniques are taken from BOOST libraries:  
www.boost.org/doc/libs/1_71_0/libs/vmd  
www.boost.org/doc/libs/1_71_0/libs/preprocessor  
The 'emptiness' methods are adapted from BOOST_VMD_IS_EMPTY  
dependent on BOOST Preprocessor's BOOST_PP_IS_BEGIN_PARENS  
(adapted and exposed here as undocumented `IBP` 'Is Begin Parens'):

`__VA_OPT__` feature detection is adapted from  
https://stackoverflow.com/a/48045656/7443483
