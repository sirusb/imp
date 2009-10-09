/**
 *  \file GridClosePairsFinder.cpp
 *  \brief Test all pairs.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 *
 */

#include "IMP/core/GridClosePairsFinder.h"
#include "IMP/core/QuadraticClosePairsFinder.h"
#include "IMP/core/XYZR.h"

#include "IMP/core/internal/ParticleGrid.h"
#include <IMP/internal/Vector.h>
#include <IMP/core/utility.h>

IMPCORE_BEGIN_NAMESPACE

namespace {
  typedef IMP::VectorOfRefCounted<internal::ParticleGrid*> GVector;

  static const unsigned int min_grid_size=100;

  struct AddToList {
    ParticlePairsTemp &out_;
    Particle *p_;
    FloatKey rk_;
    double d_;
    AddToList(ParticlePairsTemp &o,
              Particle *p, FloatKey rk, double d): out_(o), p_(p),
                                                   rk_(rk), d_(d){}
    void operator()(Particle *p) {
      if (distance(XYZR(p_, rk_),
                   XYZR(p, rk_)) < d_) {
        out_.push_back(ParticlePair(p_, p));
      }
    }
  };

  struct AddToList2 {
    ParticlePairsTemp &out_;
    FloatKey rk_;
    double d_;
    AddToList2(ParticlePairsTemp &o,
               FloatKey rk, double d): out_(o),
                                       rk_(rk), d_(d){}
    void operator()(Particle *a, Particle *b) {
      if (distance(XYZR(a, rk_),
                   XYZR(b, rk_)) < d_) {
        out_.push_back(ParticlePair(a, b));
      }
    }
  };



  double grid_side_from_r(double r)
  {
    if (r==0) return 1;
    else return r*1.6;
  }

  void grid_partition_points(SingletonContainer *c,
                             double distance,
                             FloatKey rk,
                             GVector &bins)
  {
    if (c->get_number_of_particles() ==0) return;

    double minr=std::numeric_limits<double>::max(), maxr=0;
    for (unsigned int i=0; i< c->get_number_of_particles(); ++i) {
      double r= c->get_particle(i)->get_value(rk);
      if ( r > maxr) maxr=r;
      if ( r > 0 && r < minr) minr=r;
    }
    minr= std::min(maxr, minr);
    double curr=minr*2;
    Floats cuts;
    cuts.push_back(0);
    do {
      cuts.push_back(curr);
      curr *= 2;
    } while (curr < maxr);
    cuts.push_back(2*maxr);

    std::vector<internal::ParticleGrid::Storage> ops(cuts.size());
    for (unsigned int i=0; i< c->get_number_of_particles(); ++i) {
      double r= c->get_particle(i)->get_value(rk);
      bool found=false;
      for (unsigned int j=0; ; ++j) {
      IMP_assert(j< cuts.size(), "Internal error in ASNBLSS");
      if (cuts[j] >= r) {
        ops[j].push_back(c->get_particle(i));
        found=true;
        break;
      }
    }
    IMP_assert(found, "Didn't put particle anywhere");
  }
  // consolidate
  for (unsigned int i=1; i< ops.size(); ++i) {
    if (ops[i-1].size() + ops[i].size() < min_grid_size) {
      ops[i].insert(ops[i].end(), ops[i-1].begin(), ops[i-1].end());
      ops[i-1].clear();
    }
  }
  for (unsigned int i=0; i< cuts.size(); ++i) {
    if (ops[i].empty()) continue;
    Float rmax=0;
    for (unsigned int j=0; j< ops[i].size(); ++j) {
      rmax= std::max(rmax, ops[i][j]->get_value(rk));
    }
    bins.push_back(new internal::ParticleGrid(grid_side_from_r(rmax
                                                               +distance),
                                              ops[i]));
  }
  IMP_LOG(VERBOSE, "Created " << bins.size() << " grids" << std::endl);
  for (unsigned int i=0; i< bins.size(); ++i) {
    IMP_LOG(VERBOSE, *bins[i] << std::endl);
  }
}


void grid_generate_nbl(const internal::ParticleGrid *particle_bin,
                       const internal::ParticleGrid *grid_bin,
                       Float distance,
                       FloatKey rk,
                       ParticlePairsTemp &out)
{
  IMP_LOG(VERBOSE, "Generate nbl for pair " << std::endl);
  for (internal::ParticleGrid::ParticleVoxelIterator
         it= particle_bin->particle_voxels_begin();
       it != particle_bin->particle_voxels_end(); ++it) {
    Particle *p= it->first;
    AddToList f(out, p, rk, distance);
    XYZ d(p);
    internal::ParticleGrid::VirtualIndex index
      = grid_bin->get_virtual_index(d.get_coordinates());
    IMP_LOG(VERBOSE, "Searching for " << p->get_name()
            << " from " << index << std::endl);
    grid_bin->apply_to_nearby(f, index,
                              distance,
                              false);
  }

}
}

GridClosePairsFinder::GridClosePairsFinder(){}

ParticlePairsTemp GridClosePairsFinder
::get_close_pairs(SingletonContainer *ca,
                  SingletonContainer *cb) const {
  IMP_NEW(QuadraticClosePairsFinder, qp, ());
  qp->set_radius_key(get_radius_key());
  qp->set_distance(get_distance());
  return qp->get_close_pairs(ca, cb);
}

ParticlePairsTemp GridClosePairsFinder
::get_close_pairs(SingletonContainer *c) const {
  IMP_LOG(TERSE, "Rebuilding NBL with Grid and cutoff "
          << get_distance() << std::endl );

    IMP::VectorOfRefCounted<internal::ParticleGrid*> bins;

  if (get_radius_key()== FloatKey()) {
    internal::ParticleGrid::Storage ps(c->particles_begin(),
                                       c->particles_end());
    /** \todo need to pick better value here.
     */
    bins.push_back(new internal::ParticleGrid(1.0, ps));

  } else {
    grid_partition_points(c, get_distance(), get_radius_key(), bins);
  }
  ParticlePairsTemp out;
  for (unsigned int i=0; i< bins.size(); ++i) {
    for (unsigned int j=i+1; j< bins.size(); ++j) {
      grid_generate_nbl(bins[i], bins[j], get_distance(),
                        get_radius_key(), out);
    }

    internal::ParticleGrid::Index last_index;
    for (internal::ParticleGrid::ParticleVoxelIterator it
           = bins[i]->particle_voxels_begin();
         it != bins[i]->particle_voxels_end(); ++it) {
      IMP_LOG(VERBOSE, "Searching with particle " << it->first->get_name()
              << std::endl);
      AddToList f(out, it->first, get_radius_key(), get_distance());
      bins[i]->apply_to_nearby(f, it->second,
                               get_distance(),
                               true);

      if (it->second != last_index) {
        AddToList2 fp(out, get_radius_key(), get_distance());
        IMP_LOG(VERBOSE, "Searching in " << it->second
                << std::endl);
        bins[i]->apply_to_cell_pairs(fp, it->second);
      }
      last_index=it->second;
    }
  }
  return out;
}


void GridClosePairsFinder::show(std::ostream &out) const {
  out << "GridClosePairsFinder\n";
}


ParticlesTemp
GridClosePairsFinder::get_used_particles(SingletonContainer *sc) const {
  ParticlesTemp ret= sc->get_particles();
  return ret;
}

ParticlesTemp
GridClosePairsFinder::get_used_particles(SingletonContainer *a,
                                         SingletonContainer *b) const {
  ParticlesTemp ret0= a->get_particles();
  ParticlesTemp ret1= b->get_particles();
  ret0.insert(ret0.end(), ret1.begin(), ret1.end());
  return ret0;
}

IMPCORE_END_NAMESPACE
