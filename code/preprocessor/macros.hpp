#ifndef INCLUDED_PREPROCESSOR_MACROS_HPP
#define INCLUDED_PREPROCESSOR_MACROS_HPP

#include"preprocessor/build.hpp"

#if BUILD == BUILD_DEBUG
    #define IF_DEBUG(...) __VA_ARGS__
    #define IF_RELEASE(...)
#endif

#if BUILD == BUILD_RELEASE
    #define IF_DEBUG(...)
    #define IF_RELEASE(...) __VA_ARGS__
#endif

#endif
