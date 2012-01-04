/**
 *  \file core/quad_predicates.h    \brief Define some predicates.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#ifndef IMPCORE_QUAD_PREDICATES_H
#define IMPCORE_QUAD_PREDICATES_H

#include "core_config.h"
#include <IMP/QuadPredicate.h>

IMPCORE_BEGIN_NAMESPACE

/** Always return a constant value.
 */
class IMPCOREEXPORT ConstantQuadPredicate: public QuadPredicate {
  int v_;
public:
  ConstantQuadPredicate(int v,
                             std::string name="ConstQuadPredicate%1%");
  IMP_INDEX_QUAD_PREDICATE(ConstantQuadPredicate, {
      IMP_UNUSED(m); IMP_UNUSED(pi);
      return v_;
    });
};

IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_QUAD_PREDICATES_H */
