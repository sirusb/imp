/**
 *  \file Transformation3D.h   \brief Simple 3D transformation class.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 *
 */

#ifndef IMPMISC_TRANSFORMATION_3D_H
#define IMPMISC_TRANSFORMATION_3D_H

#include "misc_exports.h"

#include "IMP/Vector3D.h"
#include "IMP/misc/Rotation3D.h"

IMPMISC_BEGIN_NAMESPACE

//! Simple 3D transformation class
/** \ingroup helper
*/
class IMPMISCEXPORT Transformation3D
{
public:
  // public for swig
  typedef Transformation3D This;
  Transformation3D():rot_(),trans_(){
  }
  Transformation3D(const Rotation3D& r, const Vector3D& t)
    : rot_(r), trans_(t){}
  //! transform
  Vector3D transform(const Vector3D &o) const {
    return rot_.mult(o) + trans_;
  }
  Vector3D get_trans()const{return trans_;}
  //  Matrix3D get_mat()const{return rot_;}
  void show(std::ostream& out = std::cout) const {
    rot_.show(out);
    out<<" || "<<trans_<<"\n";
  }
private:
  Vector3D trans_; //tranlation
  Rotation3D rot_;  //rotation
};

IMPMISC_END_NAMESPACE

#endif  /* IMPMISC_TRANSFORMATION_3D_H */
