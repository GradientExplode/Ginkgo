#ifndef _gkg_dmri_tractography_TractographyAlgorithm_i_h_
#define _gkg_dmri_tractography_TractographyAlgorithm_i_h_


#include <gkg-dmri-tractography/TractographyAlgorithm.h>
#include <gkg-core-exception/Exception.h>


template < class L, class Compare >
inline
gkg::TractographyAlgorithm< L, Compare >::TractographyAlgorithm(
                                     const gkg::Vector3d< int32_t >& size,
                                     const gkg::Vector3d< double >& resolution )
             : _size( size ),
               _resolution( resolution ),
               _boundingBox( 0.0f,
                             ( float )( _size.x * _resolution.x ),
                             0.0f,
                             ( float )( _size.y * _resolution.y ),
                             0.0f,
                             ( float )( _size.z * _resolution.z ) )

{
}


template < class L, class Compare >
inline
gkg::TractographyAlgorithm< L, Compare >::~TractographyAlgorithm()
{
}


template < class L, class Compare >
inline
const gkg::Vector3d< int32_t >& 
gkg::TractographyAlgorithm< L, Compare >::getSize() const
{

  return _size;

}


template < class L, class Compare >
inline
const gkg::Vector3d< double >& 
gkg::TractographyAlgorithm< L, Compare >::getResolution() const
{

  return _resolution;

}


template < class L, class Compare >
inline
void 
gkg::TractographyAlgorithm< L, Compare >::prepare(
                  gkg::OdfCartesianField&,
                  const gkg::Volume< int16_t >&,
                  const gkg::Transform3d< float >&,
                  const gkg::Transform3d< float >&,
                  const gkg::Transform3d< float >&,
                  const gkg::Transform3d< float >&,
                  bool )
{

  // nothing to do by default

}


template < class L, class Compare >
inline
gkg::Vector3d< int32_t > 
gkg::TractographyAlgorithm< L, Compare >::getVoxelCoordinates(
                                     const gkg::Vector3d< float >& point ) const
{

  return gkg::Vector3d< int32_t >( ( int32_t )( point.x / _resolution.x ),
                                   ( int32_t )( point.y / _resolution.y ),
                                   ( int32_t )( point.z / _resolution.z ) );

}


template < class L, class Compare >
inline
gkg::Vector3d< float > 
gkg::TractographyAlgorithm< L, Compare >::getRealCoordinates(
                                   const gkg::Vector3d< int32_t >& voxel ) const
{

  return gkg::Vector3d< float >( ( float )( voxel.x * _resolution.x ),
                                 ( float )( voxel.y * _resolution.y ),
                                 ( float )( voxel.z * _resolution.z ) );

}


#endif
