#include <gkg-processing-coordinates/VoxelSampler.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::VoxelSampler< T >::VoxelSampler(
                                     const gkg::Vector3d< double >& resolution,
                                     int32_t pointCount )
                       : _resolution( resolution ),
                         _pointCount( pointCount )
{
}


template < class T >
gkg::VoxelSampler< T >::~VoxelSampler()
{
}


template < class T >
const gkg::Vector3d< double >& gkg::VoxelSampler< T >::getResolution() const
{

  try
  {

    return _resolution;

  }
  GKG_CATCH( "template < class T > "
             "const gkg::Vector3d< double >& "
             "gkg::VoxelSampler< T >::getResolution() const" );

}


template < class T >
int32_t gkg::VoxelSampler< T >::getPointCount() const
{

  try
  {

    return _pointCount;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VoxelSampler< T >::getPointCount() const" );

}


// forcing instanciation(s)

template class gkg::VoxelSampler< float >;
template class gkg::VoxelSampler< double >;
