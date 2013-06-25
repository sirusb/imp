# Autogenerated file, run tools/build/setup_cmake.py to regenerate

if(NOT DEFINED %(PKGNAME)s_LIBRARIES)

set(CHECK_COMPILES_BODY "%(body)s")

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules("%(PKGNAME)s_PKGCONF" "%(pkg_config_name)s")

# Include dir
find_path("%(PKGNAME)s_INCLUDE_DIR"
  NAMES %(headers)s
  PATHS ${%(PKGNAME)s_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
foreach(lib %(libraries)s)
find_library("${lib}_LIBRARY"
  NAMES ${lib}
  PATHS ${%(PKGNAME)s_PKGCONF_LIBRARY_DIRS}
)
list(APPEND %(PKGNAME)s_LIBRARY "${${lib}_LIBRARY}")
endforeach(lib)


# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(%(PKGNAME)s_PROCESS_INCLUDES %(PKGNAME)s_INCLUDE_DIR)
set(%(PKGNAME)s_PROCESS_LIBS %(PKGNAME)s_LIBRARY)

# clear target variable
set(%(PKGNAME)s_LIBRARIES "" CACHE INTERNAL "")
libfind_process(%(PKGNAME)s)

if ("${%(PKGNAME)s_LIBRARY}" MATCHES ".*NOTFOUND.*"
    OR "${%(PKGNAME)s_INCLUDE_DIR}" MATCHES ".*NOTFOUND.*")
  %(on_failure)s
else()
  check_compiles("_found" "%(pkgname)s" "%(PKGNAME)s" "%(includes)s" "${%(PKGNAME)s_INCLUDE_DIR}" "${%(PKGNAME)s_LIBRARIES}" %(PKGNAME)s_ok_ok)
  if(${%(PKGNAME)s_ok_ok} MATCHES "1")
    message(STATUS "Found %(pkgname)s at \"${%(PKGNAME)s_INCLUDE_DIR}\" and \"${%(PKGNAME)s_LIBRARIES}\"")
  else()
    %(on_failure)s
  endif()
endif()
else()
message(STATUS "%(PKGNAME)s already setup")
%(on_setup)s
endif(NOT DEFINED %(PKGNAME)s_LIBRARIES)
