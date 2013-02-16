# Autogenerated file, run build/tools/setup_cmake.py to regenerate

if (${IMP_USE_CUSTOM_CXX_FLAGS})
set(CMAKE_REQUIRED_FLAGS ${IMP_BIN_CXX_FLAGS})
endif()

INCLUDE (CheckCXXSourceCompiles)

CHECK_CXX_SOURCE_COMPILES("/**
 *  ile nothing.cpp
 *  rief nothing
 *
 *  Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */

int main()
{
  auto a=6;
  return 0;
}

"
 IMP_COMPILER_HAS_AUTO)

if(${IMP_COMPILER_HAS_AUTO} MATCHES 1)
set(IMP_COMPILER_HAS_AUTO 1 CACHE INTERNAL "" FORCE )
else()
set(IMP_COMPILER_HAS_AUTO 0 CACHE INTERNAL "" FORCE)
endif()