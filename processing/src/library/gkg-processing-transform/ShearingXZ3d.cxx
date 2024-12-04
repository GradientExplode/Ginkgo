#include <gkg-processing-transform/ShearingXZ3d.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::ShearingXZ3d< T >::ShearingXZ3d( const T& sxz, bool direct )
                       : gkg::HomogeneousTransform3d< T >()
{

  try
  {

    if ( direct )
    {

      setDirectShearingXZ( sxz, true, false );

    }
    else
    {

      setInverseShearingXZ( sxz, true, false );

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::ShearingXZ3d< T >::ShearingXZ3d( const T& shxz, "
             "bool direct )" );

}


template < class T >
gkg::ShearingXZ3d< T >::ShearingXZ3d( const gkg::ShearingXZ3d< T >& other )
                       : gkg::HomogeneousTransform3d< T >( other )
{
}


template < class T >
gkg::ShearingXZ3d< T >::~ShearingXZ3d()
{
}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > > gkg::ShearingXZ3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset( new gkg::ShearingXZ3d< T >( *this ) );
  return transform;

}


template < class T >
void gkg::ShearingXZ3d< T >::setDirectShearingXZ( const T& sxz,
                                                  bool update,
                                                  bool notify )
{

  try
  {

    this->setDirectItems( 1.0, 0.0, 0.0, 0.0,
                          0.0, 1.0, 0.0, 0.0,
                          sxz, 0.0, 1.0, 0.0,
                          0.0, 0.0, 0.0, 1.0,
                          update, notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::ShearingXZ3d< T >::setDirectShearingXZ( const T& sxz, "
             "bool update, bool notify )" );

}


template < class T >
void gkg::ShearingXZ3d< T >::getDirectShearingXZ( T& sxz ) const
{

  sxz = this->_directItems[ 2 ][ 0 ];
                            
}


template < class T >
void gkg::ShearingXZ3d< T >::setInverseShearingXZ( const T& sxz,
                                                   bool update,
                                                   bool notify )
{

  try
  {

    this->setInverseItems( 1.0, 0.0, 0.0, 0.0,
                           0.0, 1.0, 0.0, 0.0,
                           sxz, 0.0, 1.0, 0.0,
                           0.0, 0.0, 0.0, 1.0,
                           update, notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::ShearingXZ3d< T >::setInverseShearingXZ( const T& sxz, "
             "bool update, bool notify )" );

}


template < class T >
void gkg::ShearingXZ3d< T >::getInverseShearingXZ( T& sxz ) const
{

  sxz = this->_inverseItems[ 2 ][ 0 ];
                            
}


template class gkg::ShearingXZ3d< float >;
template class gkg::ShearingXZ3d< double >;
