%module(directors="1") "IMP.algebra"

%{
#include "IMP.h"
#include "IMP/algebra.h"
%}

%include "kernel/pyext/IMP_macros.i"
%include "kernel/pyext/IMP_exceptions.i"

%include "algebra_config.i"
%include "exception.i"
%include "std_except.i"

/* Get definitions of kernel base classes (but do not wrap) */
%import "kernel/pyext/IMP.i"
%import "kernel/pyext/IMP_keys.i"

namespace IMP {
  namespace algebra {
    typedef VectorD<3> Vector3D;
  }
}

/* Wrap our own base classes */
%include "VectorD.i"
%include "IMP/algebra/Rotation3D.h"
%include "IMP/algebra/Transformation3D.h"
%include "IMP/algebra/geometric_alignment.h"
%include "IMP/algebra/eigen_analysis.h"
%include "IMP/algebra/Cylinder3D.h"
%include "IMP/algebra/vector_generators.h"

namespace IMP {
 namespace algebra {
   %template(random_vector_on_sphere) random_vector_on_sphere<3>;
   %template(random_vector_in_sphere) random_vector_in_sphere<3>;
   %template(random_vector_in_box) random_vector_in_box<3>;
   %template(random_vector_on_unit_sphere) random_vector_on_unit_sphere<3>;
   %template(random_vector_in_unit_sphere) random_vector_in_unit_sphere<3>;
   %template(random_vector_in_unit_box) random_vector_in_unit_box<3>;
   %template(random_vector_on_box) random_vector_on_box<3>;
 }
}
