/**
 *  \file helper/atom_hierarchy.h
 *  \brief functionality for defining rigid bodies
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 */

#ifndef IMPHELPER_HELPER_ATOM_HIERARCHY_H
#define IMPHELPER_HELPER_ATOM_HIERARCHY_H

#include "config.h"
#include <IMP/core/XYZR.h>
#include <IMP/atom/Hierarchy.h>


IMPHELPER_BEGIN_NAMESPACE

//! Create a coarse grained molecule
/** The coarse grained model is created with a number of spheres
    based on the resolution and the volume. If the volume is not provided
    it is estimated based on the number of residues. The protein is
    created as a molecular hierarchy rooted at p. The leaves are Domain
    particles wtih appropriate residue indexes stored and are XYZR
    particles.

    Volume is, as usual, in cubic anstroms.

    Currently the function creates a set of balls with radii no greater
    than resolution which overlap by 20% and have a volume of their
    union equal to the passed volume. The balls are held together by
    a ConnectivityRestraint with the given spring constant.

    The coordinates of the balls defining the protein are optimized
    by default, and have garbage values.

    \return A restraint which will enforce appropriate connectivity
    of the protein.
    \untested{create_protein}
    \unstable{create_protein}
    \relatesalso atom::Hierarchy
 */
IMPHELPEREXPORT Restraint* create_protein(Particle *p,
                                        double resolution,
                                        int number_of_residues,
                                        int first_residue_index=0,
                                        double volume=-1,
                                        double spring_strength=1);





/** Get the set of particles providing the least detailed representation
    of the structure. That is, a set of particles with x,y,z coordinates
    which are either the root or have parents without coordinates.
    \unstable{get_simplified_representation}
    \untested{get_simplified_representation}
    \relatesalso atom::Hierarchy
*/
IMPHELPEREXPORT Particles
get_simplified_representation(atom::Hierarchy mhd);


//! Clone the atom::Hierarchy
/** This method copies the atom::Bond, atom::Bonded, atom::Atom,
    atom::Residue, and atom::Domain data and the particle name to the
    new copies in addition to the atom::Hierarchy relationships.

    \untested{clone}
    \relatesalso atom::Hierarchy
*/
IMPHELPEREXPORT
atom::Hierarchy clone(atom::Hierarchy d);



//! Get a bounding box for the atom::Hierarchy
/** This bounding box is that of the highest (in the CS sense of a tree
    growning down from the root) cut
    through the tree where each node in the cut has x,y,z, and r.
    That is, if the root has x,y,z,r then it is the bounding box
    if that sphere. If only the leaves have radii, it is the bounding
    box of the leaves. If no such cut exists, the behavior is undefined.
    \relatesalso atom::Hierarchy
    \relatesalso IMP::algebra::BoundingBoxD
 */
IMPHELPEREXPORT
algebra::BoundingBox3D get_bounding_box(const atom::Hierarchy &h,
                                    FloatKey r
                                    = core::XYZR::get_default_radius_key());


/** See get_bounding_box() for more details.
    \relates atom::Hierarchy
 */
IMPHELPEREXPORT
algebra::Sphere3D bounding_sphere(const atom::Hierarchy &h,
                                    FloatKey r
                                    = core::XYZR::get_default_radius_key());

IMPHELPER_END_NAMESPACE

#endif  /* IMPHELPER_HELPER_ATOM_HIERARCHY_H */
