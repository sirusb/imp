# Autogenerated file, run build/tools/setup_cmake.py to regenerate

if(NOT DEFINED FFTW3_LIBRARIES)
message(STATUS "Checking for FFTW3")

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(FFTW3_PKGCONF FFTW3)

# Include dir
find_path(FFTW3_INCLUDE_DIR
  NAMES fftw.h
  PATHS ${FFTW3_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
foreach(lib fftw3)
find_library(${lib}_LIBRARY
  NAMES ${lib}
  PATHS ${FFTW3_PKGCONF_LIBRARY_DIRS}
)
set(FFTW3_LIBRARY ${FFTW3_LIBRARY} ${${lib}_LIBRARY})
endforeach(lib)


# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(FFTW3_PROCESS_INCLUDES FFTW3_INCLUDE_DIR)
set(FFTW3_PROCESS_LIBS FFTW3_LIBRARY)
libfind_process(FFTW3)

if ("${FFTW3_LIBRARY}" MATCHES ".*NOTFOUND.*"
    OR "${FFTW3_INCLUDE_DIR}" MATCHES ".*NOTFOUND.*")
  message(STATUS "FFTW3 not found")
  file(WRITE "${PROJECT_BINARY_DIR}/data/build_info/FFTW3" "ok=False")
else()
  include(CheckCXXSourceCompiles)
  set(CMAKE_REQUIRED_LIBRARIES "${FFTW3_LIBRARY}")
  set(body "#include <fftw.h>
int main(int,char*[]) {
  
  return 0;
}")
  check_cxx_source_compiles("${body}"
 FFTW3_COMPILES)
  if ("FFTW3_COMPILES" MATCHES "1")
    message(STATUS "FFTW3 found " ${FFTW3_INCLUDE_DIR} " " ${FFTW3_LIBRARY})
    file(WRITE "${PROJECT_BINARY_DIR}/data/build_info/FFTW3" "ok=True")
    #set(FFTW3_LINK_PATH ${FFTW3_LIBRARY_DIRS} CACHE INTERNAL ""  FORCE)
    set(FFTW3_INCLUDE_PATH ${FFTW3_INCLUDE_DIR} CACHE INTERNAL "" FORCE)
    set(FFTW3_LIBRARIES ${FFTW3_LIBRARY} CACHE INTERNAL "" FORCE)
  else()
    file(WRITE "${PROJECT_BINARY_DIR}/data/build_info/FFTW3" "ok=False")
  endif()
endif()

else()
message(STATUS "FFTW3 already setup")

endif(NOT DEFINED FFTW3_LIBRARIES)