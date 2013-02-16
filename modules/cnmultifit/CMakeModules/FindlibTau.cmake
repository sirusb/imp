# Autogenerated file, run build/tools/setup_cmake.py to regenerate

if(NOT DEFINED libTau_LIBRARIES)
message(STATUS "Checking for libTau")

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(libTau_PKGCONF libTau)

# Include dir
find_path(libTau_INCLUDE_DIR
  NAMES libTAU/PairwiseDockingEngine.h
  PATHS ${libTau_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(libTau_LIBRARY
  NAMES TAU
  PATHS ${libTau_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(libTau_PROCESS_INCLUDES libTau_INCLUDE_DIR)
set(libTau_PROCESS_LIBS libTau_LIBRARY)
libfind_process(libTau)

if (${libTau_LIBRARY} MATCHES "libTau_LIBRARY-NOTFOUND"
    OR ${libTau_INCLUDE_DIR} MATCHES "libTau_INCLUDE_DIR-NOTFOUND")
  message(STATUS "libTau not found")
  file(WRITE "${PROJECT_BINARY_DIR}/data/build_info/libTau" "ok=False")
else()
  message(STATUS "libTau found " ${libTau_INCLUDE_DIR} " " ${libTau_LIBRARY})
  file(WRITE "${PROJECT_BINARY_DIR}/data/build_info/libTau" "ok=True")
  #set(LIBTAU_LINK_PATH ${libTau_LIBRARY_DIRS} CACHE INTERNAL ""  FORCE)
  set(LIBTAU_INCLUDE_PATH ${libTau_INCLUDE_DIR} CACHE INTERNAL "" FORCE)
  set(LIBTAU_LIBRARIES ${libTau_LIBRARY} CACHE INTERNAL "" FORCE)
endif()

else()
message(STATUS "libTau already setup")

endif(NOT DEFINED libTau_LIBRARIES)