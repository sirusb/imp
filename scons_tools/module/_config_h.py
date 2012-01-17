
import scons_tools.module
import scons_tools.data
from SCons.Script import Glob, Dir, File, Builder, Action, Exit
import os
import sys
import re

def _add_dep_control(name, preproc, h, controllable, default):
    lcname=name
    if controllable:
        print >> h, """#if !defined(SWIG) && !defined(IMP_DOXYGEN)
namespace internal {
extern %sEXPORT bool use_%s;
}
#endif

inline void set_use_%s(bool tf) {
   internal::use_%s=tf;
}
inline bool get_use_%s() {return internal::use_%s;}
"""%(preproc.replace("_",""), lcname, lcname, lcname, lcname, lcname)
    else:
        print >> h, """
inline void set_use_%s(bool ) {
}
inline bool get_use_%s() {return %s;}
"""%(lcname, lcname, str(default).lower())

def _add_use_or_no(env, h, name):
    nd= name.replace("_USE_","_NO_")
    if nd==name:
        nd= name.replace("_NO_", "_USE_")
    print >> h, "\n",
    #print >> h, "#  if defined("+nd+") || defined("+name+")"
    #print >> h, '#    error "Do not define macro '+name+' directly."'
    #print >> h, "#  endif"
    print >> h, "#  define "+name

def _add_use(env, h, nm):
    vars= scons_tools.module._get_module_variables(env)
    curn= vars['PREPROC']
    if curn=="IMP":
        curn="IMP_KERNEL"
    _add_use_or_no(env, h, curn.upper()+"_USE_"+nm.upper())
def _add_no_use(env, h, nm):
    vars= scons_tools.module._get_module_variables(env)
    curn= vars['PREPROC']
    if curn=="IMP":
        curn="IMP_KERNEL"
    _add_use_or_no(env, h, curn.upper()+"_NO_"+nm.upper())
def _add_version(env, h, nm, version, versionheader, versioncpp):
    print >> h, "#ifndef SWIG"
    print >> h, "\n#include <"+versionheader+">\n"
    if type(version)==type([]):
        if not type(versioncpp) == type([]):
            scons_tools.utility.report_error(env, "Version list expected")
        test = " && ".join([x[0]+" != " + x[1] for x in zip(versioncpp, version)])
    else:
        test=versioncpp +" != "+version
    print >> h, "#  if "+test
    print >> h, '#    error "'+nm+ 'versions does not match expected version. Please rerun"\\'\
    '"configuration tests by running scons with --config=force."'
    print >> h, "#  endif"
    print >> h, "#endif"


def _action_config_h(target, source, env):
    """The IMPModuleConfig Builder generates a configuration header file
       used to mark classes and functions for export and to define namespaces,
       and a corresponding SWIG interface, e.g.
       env.IMPModuleConfig(('config.h', 'foo_config.i'), env.Value('foo'))
       generates a configuration header and interface for the 'foo' module."""
    vars= scons_tools.module._get_module_variables(env)
    #print "opening h at " +target[0].abspath + " for module %(module)s"%vars
    h = file(target[0].abspath, 'w')
    #print "Generating "+str(h)
    print >> h, """/*
 * \\file %(module_include_path)s/%(module)s_config.h
 * \\brief Provide macros to mark functions and classes as exported
 *        from a DLL/.so, and to set up namespaces
 *
 * When building the module, %(EXPORT)s_EXPORTS should be defined, and when
 * using the module externally, it should not be. Classes and functions
 * declared in the module's headers should then be marked with
 * %(EXPORT)sEXPORT if they are intended to be part of the API and
 * they are not defined entirely in a header.
 *
 * The Windows build environment requires applications to mark exports in
 * this way; we use the same markings to set the visibility of ELF symbols
 * if we have compiler support.
 *
 * All code in this module should live in the %(namespace)s namespace.
 * This is simply achieved by wrapping things with the
 * %(EXPORT)s_BEGIN_NAMESPACE and %(EXPORT)s_END_NAMESPACE macros.
 * There are similar macros for module code that is designed to be for
 * internal use only.
 *
 * This header is auto-generated by scons_tools/module/_config_h.py;
 * it should not be edited manually.
 *
 * Copyright 2007-2012 IMP Inventors. All rights reserved.
 *
 */

#ifndef %(EXPORT)s_CONFIG_H
#define %(EXPORT)s_CONFIG_H

#include <IMP/compatibility/compatibility_config.h>
#include <string>


#  ifdef _MSC_VER

#    ifdef %(EXPORT)s_EXPORTS
#      define %(EXPORT)sEXPORT __declspec(dllexport)
#    else // EXPORTS
#      define %(EXPORT)sEXPORT __declspec(dllimport)
#    endif // EXPORTS

#  else // _MSC_VER

#    ifdef GCC_VISIBILITY
#      define %(EXPORT)sEXPORT __attribute__ ((visibility("default")))
#    else // GCC_VISIBILITY
#      define %(EXPORT)sEXPORT
#    endif // GCC_VISIBILITY
#  endif // _MSC_VER

#  if defined(_MSC_VER) && !defined(SWIG)
#    ifdef %(EXPORT)s_EXPORTS

#      define %(EXPORT)s_EXPORT_TEMPLATE(name)       \
          template class __declspec(dllexport) name

#    else //EXPORTS

#      define %(EXPORT)s_EXPORT_TEMPLATE(name)       \
          template class __declspec(dllimport) name

#    endif // EXPORTS

#  else // MSC and SWIG
#    define %(EXPORT)s_EXPORT_TEMPLATE(name) IMP_REQUIRE_SEMICOLON_NAMESPACE

#  endif // MSC and SWIG


""" % vars
    print >> h, "#  define %(EXPORT)s_BEGIN_NAMESPACE \\"%vars
    for comp in vars['namespace'].split("::"):
        print >> h, "namespace %s {\\" %comp
    print >> h
    print >> h, "#  define %(EXPORT)s_END_NAMESPACE \\"%vars
    for comp in vars['namespace'].split("::"):
        print >> h, "} /* namespace %s */ \\" %comp
    print >> h
    print >> h, """#  define %(EXPORT)s_BEGIN_INTERNAL_NAMESPACE \\
%(EXPORT)s_BEGIN_NAMESPACE \\
namespace internal {
""" %vars
    print >> h
    print >> h, """#  define %(EXPORT)s_END_INTERNAL_NAMESPACE \\
} /* namespace internal */ \\
%(EXPORT)s_END_NAMESPACE
""" %vars
    print >> h
    dta=scons_tools.data.get(env)
    for d in scons_tools.module._get_module_direct_dependencies(env):
        #print "processing", d
        nm=scons_tools.dependency.get_dependency_string(d)
        _add_use(env, h, nm)
        if env['versionchecks'] and dta.dependencies[d].version:
            _add_version(env, h, nm,
                         dta.dependencies[d].version,
                         dta.dependencies[d].versionheader,
                         dta.dependencies[d].versioncpp)
        else:
            #print "no version for", d
            pass
    for d in scons_tools.module._get_module_unfound_dependencies(env):
        nm=scons_tools.dependency.get_dependency_string(d)
        _add_no_use(env, h, nm)
    for d in scons_tools.module._get_module_modules(env):
        nm="IMP_"+d.upper()
        _add_use(env, h, nm)
    for d in scons_tools.module._get_module_unfound_modules(env):
        nm="IMP_"+d.upper()
        _add_no_use(env, h, nm)

    for d in env['IMP_MODULE_CONFIG']:
        if type(d) == type([]):
            name=d[0]
            value=d[1]
        else:
            name=d
            value=None
        nd= name.replace("_USE_","_NO_")
        if nd==name:
            nd= name.replace("_NO_", "_USE_")
        #if nd != name:
        #    print >> h, "#  ifdef "+nd
        #    print >> h, "/* Do not define IMP config macros directly */"
        #    print >> h, """#    error "Do not define macro """+nd+""" directly.\""""
        #    print >> h, "#  endif"
        #print >> h, "#  ifdef "+name
        #print >> h, "/* Do not define IMP config macros directly */"
        #print >> h, """#    error "Do not define macro """+name+""" directly.\""""
        #print >> h, "#  endif"
        if value is not None:
            print >> h, "#  define "+name+" "+str(value)
        else:
            print >> h, "#  define "+name
    print >> h


    # This needs to be called get_module_version_info() to make it easy
    # to call from Objects (which have their own get_version_info() method
    if env['MODULE_HAS_DATA']:
        print >> h, """
#  ifndef SWIG
#    include <IMP/base/internal/directories.h>

IMPBASE_BEGIN_INTERNAL_NAMESPACE
IMPBASEEXPORT std::string get_data_path(std::string module_name,
                                    std::string file_name);
IMPBASEEXPORT std::string get_example_path(std::string module_name,
                                       std::string file_name);

IMPBASE_END_INTERNAL_NAMESPACE
#  endif // SWIG
"""
    print >> h, """

//  functions are defined explicitly for swig
#ifndef SWIG

#ifndef IMP_DOXYGEN
namespace std {
template <class T, class A>
class vector;
template <class A, class B>
class pair;
}
namespace IMP {
namespace compatibility {
template <class T>
class vector;
}
}
#include <sstream>
#endif

%(EXPORT)s_BEGIN_NAMESPACE
/** \\name Standard module methods
  All \imp modules have a set of standard methods to help get information
  about the module and about file associated with the modules.
  @{
  */
%(EXPORT)sEXPORT std::string get_module_version();

inline std::string get_module_name() {
   return "%(namespace)s";
}
"""%vars

    for m in scons_tools.module._get_found_modules(env, scons_tools.module._get_module_modules(env)):
        _add_dep_control(m, vars['PREPROC'], h, env['build'] != 'fast', True)
    for m in scons_tools.module._get_module_unfound_modules(env):
        _add_dep_control(m, vars['PREPROC'], h, False, False)

    if env['MODULE_HAS_DATA']:
        print >> h, """
//! Return the full path to installed data
/** Each module has its own data directory, so be sure to use
    the version of this function in the correct module. To read
    the data file "data_library" that was placed in the \c data
    directory of module "mymodule", do something like
    \code
    std::ifstream in(IMP::mymodule::get_data_path("data_library"));
    \endcode
    This will ensure that the code works when \imp is installed or
    used via the \c tools/imppy.sh script.
*/
inline std::string get_data_path(std::string file_name) {
  return IMP::base::internal::get_data_path("%(module)s", file_name);
}

//! Return the path to installed example data for this module
/** Each module has its own example directory, so be sure to use
    the version of this function in the correct module.  For example
    to read the file \c example_protein.pdb located in the
    \c examples directory of the IMP::atom module, do
    \code
    IMP::atom::read_pdb(IMP::atom::get_example_path("example_protein.pdb", model));
    \endcode
    This will ensure that the code works when \imp is installed or
    used via the \c tools/imppy.sh script.
*/
inline std::string get_example_path(std::string file_name)  {
  return IMP::base::internal::get_example_path("%(module)s", file_name);
}
/** @} */

#if !defined(IMP_DOXYGEN) && !defined(SWIG)
struct Showable {
  std::string str_;

  template <class T>
  const T& get_showable(const T &t) const {
    return t;
  }
  template <class T, class TT>
  std::string get_showable(const std::pair<T,TT> &t) const {
    return Showable(t).str_;
  }
  template <class T>
  std::string get_showable(const IMP::compatibility::vector<T> &t) const {
    return Showable(t).str_;
  }

  template <class T>
  void show_vector(const T &v) {
    std::ostringstream out;
    out << "[";
    for (unsigned int i=0; i< v.size(); ++i) {
      if (i >0) out << ", ";
      out << get_showable(v[i]);
    }
    out<< "]";
    str_= out.str();
  }
  Showable(const std::string& str): str_(str){}
  template <class T, class TT>
  Showable(const std::pair<T, TT> &p) {
    std::ostringstream oss;
    oss << "(" << p.first << ", " << p.second << ")";
    str_=oss.str();
  }
  template <class T, class A>
  Showable(const std::vector<T, A> &v) {
    show_vector(v);
  }
  template <class T>
  Showable(const IMP::compatibility::vector<T> &v) {
    show_vector(v);
  }
};

inline std::ostream &operator<<(std::ostream &out, const Showable &s) {
  out << s.str_;
  return out;
}
#endif

"""% vars

    print >> h,"""

%(EXPORT)s_END_NAMESPACE

#endif // SWIG

#endif  /* %(EXPORT)s_CONFIG_H */""" % vars

def _print_config_h(target, source, env):
    vars= scons_tools.module._get_module_variables(env)
    print "Generating %(module)s_config.h"%vars

ConfigH = Builder(action=Action(_action_config_h,
                                _print_config_h))

def _action_config_cpp(target, source, env):
    vars= scons_tools.module._get_module_variables(env)
    vars['version']= source[0].get_contents()
    cpp = file(target[0].abspath, 'w')

    print >> cpp, """/**
 *  \\file %(module_include_path)s/config.cpp
 *  \\brief %(module)s module version information.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 *
 */
""" % vars

    print >> cpp, """#include <%(module_include_path)s/%(module)s_config.h>
"""  % vars


    print >> cpp, "%(EXPORT)s_BEGIN_NAMESPACE\n" % vars


    print >> cpp, """
std::string get_module_version() {
    return std::string("%(version)s");
}
""" %vars
    if vars['module']=="base":
        print >> cpp, """
namespace internal {
 std::string imp_data_path="%s";
 std::string imp_example_path="%s";
}
"""%(source[1].get_contents(), source[2].get_contents())
    elif vars['module']=="RMF":
        print >> cpp, """
namespace internal {
std::string rmf_data_path="%s";
std::string rmf_example_path="%s";
}
"""%(source[1].get_contents(), source[2].get_contents())

    if env['build'] != 'fast':
        print >> cpp, """
  namespace internal {
  """
        for m in scons_tools.module._get_module_modules(env):
            print >> cpp, "bool use_%s;"%m
        print >> cpp, """
  }
  """
    print >> cpp, "\n%(EXPORT)s_END_NAMESPACE" % vars



def _print_config_cpp(target, source, env):
    vars= scons_tools.module._get_module_variables(env)
    print "Generating %(module)s_config.cpp"%vars


ConfigCPP = Builder(action=Action(_action_config_cpp,
                                  _print_config_cpp))
