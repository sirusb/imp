#ifndef _DENSITYMAP_H
#define _DENSITYMAP_H


#include "EM_config.h"
#include "DensityHeader.h"
#include "Vector3.h"
#include <iostream>
#include <iomanip>
#include "ErrorHandling.h"
#include "MapReaderWriter.h"

//! Class for handling density maps.
/** CONVENTIONS
    1) IT IS ASSUMED THAT THE LOCATION OF A VOXEL IS AT THE LOWER XYZ
       VALUES THAT IT DEFINES AS A BOX.
       This is important for function calc_all_voxel2loc() 
 */
class EMDLLEXPORT DensityMap
{
public:
  DensityMap();
  DensityMap(const DensityMap &other);
  DensityMap&  operator=(const DensityMap &other );
  ~DensityMap();

  /**
   */
  void CreateVoidMap(const int &nx,const int &ny,const int &nz);

  void Read(const char *filename, MapReaderWriter &reader);
  void Write(const char *filename,MapReaderWriter &writer);


  // data managment
  void reset_data(); 

  //! Calculates RMSD and mean of a map values are stored in the header.
  /** The header stores whether map is normalized.
   */
  emreal calcRMS();


  //! Normailze the density voxles according to standard deviation (stdv).
  /** The mean is subtracted from the map, which is then divided by the stdv.
      The normalization flag is set to avoid repeated computation */
  void std_normalize();

  inline bool is_normalized() const {return normalized_;}

  //! Calculate the location of a given voxel.
  /** \param[in] index The voxel index
      \param[in] dim   The dimesion of intereset ( between x:=0,y:=1,z:=2)
      \return the location (x,y,z) (in angstroms) of a given voxel. If the
              index is not part of the map, the function returns -1. 
   */
  float voxel2loc(const int &index,int dim);

  //! Calculate the voxel of a given location
  /** \param[in] x The position ( in angstroms) of the x coordinate
      \param[in] y The position ( in angstroms) of the y coordinate
      \param[in] z The position ( in angstroms) of the z coordinate
      \exception  EMBED_OutOfRange  The point is not covered by the grid.
      \return the voxel index of a given position. If the position is out of
              the boundaries of the map, the function returns -1.
   */
  int loc2voxel(float x, float y, float z) const;

  //! Checks whether a given point is in the grid the voxel of a given location
  /** \param[in] x The position ( in angstroms) of the x coordinate
      \param[in] y The position ( in angstroms) of the y coordinate
      \param[in] z The position ( in angstroms) of the z coordinate
      \return true if the point is part of the grid, false otherwise.
   */
  bool part_of_volume(float x,float y,float z) const;

  //! Gets the value of the voxel located at (x,y,z)
  /** \param[in] x The position ( in angstroms) of the x coordinate
      \param[in] y The position ( in angstroms) of the y coordinate
      \param[in] z The position ( in angstroms) of the z coordinate
      \return the value of the voxel located at (x,y,z)
      \exception std::out_of_range The point is not covered by the grid.
   */
  emreal get_value(float x,float y,float z) const;

  //! Sets the origin of the header
  /**
    \param x the new x
    \param y the new y
    \param z the new z
  */
  void set_origin(float x,float y,float z);


  // inspection functions
  const DensityHeader *get_header()const {return &header_;}
  //! Returns a pointer to the header of the map in a writable version
  DensityHeader *get_header_writable() {return &header_;}

  //! Returns the x-location of the map
  /**
  \exception EMBED_OutOfRange The locations have not been calculated.
  */
  float* get_x_loc() const {
    if (!loc_calculated_) {
      std::ostringstream msg;
      msg << " DensityMap::get_x_loc >> "
      "x location requested before being calculated \n";
      throw EMBED_LogicError(msg.str().c_str());
    }
    return x_loc_;
  }
  //! Returns the y-location of the map
  /**
  \exception EMBED_OutOfRange The locations have not been calculated.
  */
  float* get_y_loc() const {
    if (!loc_calculated_) {
      std::ostringstream msg;
      msg << " DensityMap::get_y_loc >> "
      "y location requested before being calculated \n";
      throw EMBED_LogicError(msg.str().c_str());
    }
    return y_loc_;
  }
  //! Returns the z-location of the map
  /**
  \exception EMBED_OutOfRange The locations have not been calculated.
  */
  float* get_z_loc() const {
    if (!loc_calculated_) {
      std::ostringstream msg;
      msg << " DensityMap::get_z_loc >> "
      "z location requested before being calculated \n";
      throw EMBED_LogicError(msg.str().c_str());
    }
    return z_loc_;
  }

  emreal* get_data() const {return data_;}

  //! Checks if two maps have the same origin
  /** \param[in] other the map to compare with
      \return true if the two maps have the same origin
   */
  bool same_origin(const DensityMap &other) const;

  //! Checks if  two maps have the same dimensions
  /** \param[in] other the map to compare with
      \return true if the two maps have the same dimensions
   */
  bool same_dimensions(const DensityMap &other) const;

  //! Checks if  two maps have the same voxel size
  /** \param[in] other the map to compare with
      \return true if the two maps have the same voxel size
   */
  bool same_voxel_size(const DensityMap &other) const;
  //! Calculates the centroid of all the voxels with 
  //! density above a given threshold
  /** \param[in] threshold the input threshold
  */
  Vector3 get_centroid(emreal threshold=0.0);
  //! Returns the the value of the voxel with the highest density.
  emreal get_max_value() const;
protected:

  //! Calculates the coordinates that correspond to all voxels.
  /** Can be precomputed to make corr faster.  
      \todo which is a better design - have it public or call it from voxel2loc?
   */
  void calc_all_voxel2loc();


  void allocated_data();
  void float2real(float *f_data, emreal **r_data);
  void real2float(emreal *r_data, float **f_data);

  DensityHeader header_; // holds all the info about the map
  emreal *data_; // the order is ZYX (Z-slowest)
  bool data_allocated_;

  //! Locations for each of the voxels of the map (they are precomputed and
  //! each one is of size nvox, being nvox the size of the map)
  float *x_loc_, *y_loc_, *z_loc_;
  //! true if the locations have already been computed
  bool loc_calculated_;

  bool normalized_;
  bool rms_calculated_;

};



#endif //_DENSITYMAP_H
