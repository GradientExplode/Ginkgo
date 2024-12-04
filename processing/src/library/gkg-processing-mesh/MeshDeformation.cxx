#include <gkg-processing-mesh/MeshDeformation.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::MeshDeformation< T >::MeshDeformation( int32_t deformationCount,
                                            int32_t normalCount )
{

  resize( deformationCount, normalCount );

}


template < class T >
gkg::MeshDeformation< T >::MeshDeformation( 
                                        const gkg::MeshDeformation< T >& other )
                         : _deformations( other._deformations ),
                           _normals( other._normals )
{
}


template < class T >
gkg::MeshDeformation< T >::~MeshDeformation()
{
}


template < class T >
void gkg::MeshDeformation< T >::resize( int32_t deformationCount,
                                        int32_t normalCount )
{

  try
  {

    if ( deformationCount > 0 )
    {

      _deformations.resize( deformationCount );
      
    }
    
    if ( normalCount > 0 )
    {
    
      _normals.resize( normalCount );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::MeshDeformation< T >::resize( "
             "int32_t deformationCount, int32_t normalCount )" );

}


template < class T >
void gkg::MeshDeformation< T >::addNormals( 
                                const std::list< gkg::Vector3d< T > >& normals )
{

  try
  {

    _normals = normals;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::MeshDeformation< T >::addNormals( "
             "const std::list< gkg::Vector3d< T > >& normals )" );

}


template < class T >
std::vector< T >& gkg::MeshDeformation< T >::getDeformations()
{

  return _deformations;

}


template < class T >
std::list< gkg::Vector3d< T > >& gkg::MeshDeformation< T >::getNormals()
{

  return _normals;

}


template < class T >
const std::vector< T >& gkg::MeshDeformation< T >::getDeformations() const
{

  return _deformations;

}


template < class T >
const std::list< gkg::Vector3d< T > >& 
gkg::MeshDeformation< T >::getNormals() const
{

  return _normals;

}


// 
// forcing instanciations
//

template class gkg::MeshDeformation< float >;
template class gkg::MeshDeformation< double >;
