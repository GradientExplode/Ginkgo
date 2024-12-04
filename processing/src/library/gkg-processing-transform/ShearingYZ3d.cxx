#include <gkg-processing-transform/ShearingYZ3d.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::ShearingYZ3d< T >::ShearingYZ3d( const T& syz, bool direct )
                       : gkg::HomogeneousTransform3d< T >()
{

  try
  {

    if ( direct )
    {

      setDirectShearingYZ( syz, true, false );

    }
    else
    {

      setInverseShearingYZ( syz, true, false );

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::ShearingYZ3d< T >::ShearingYZ3d( const T& syz, "
             " bool direct )" );

}


template < class T >
gkg::ShearingYZ3d< T >::ShearingYZ3d( const gkg::ShearingYZ3d< T >& other )
                       : gkg::HomogeneousTransform3d< T >( other )
{
}


template < class T >
gkg::ShearingYZ3d< T >::~ShearingYZ3d()
{
}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > > gkg::ShearingYZ3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset( new gkg::ShearingYZ3d< T >( *this ) );
  return transform;

}


template < class T >
void gkg::ShearingYZ3d< T >::setDirectShearingYZ( const T& syz,
                                                  bool update,
                                                  bool notify )
{

  try
  {

    this->setDirectItems( 1.0, 0.0, 0.0, 0.0,
                          0.0, 1.0, 0.0, 0.0,
                          0.0, syz, 1.0, 0.0,
                          0.0, 0.0, 0.0, 1.0,
                          update, notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::ShearingYZ3d< T >::setDirectShearingYZ( const T& syz, "
             "bool update, bool notify )" );

}


template < class T >
void gkg::ShearingYZ3d< T >::getDirectShearingYZ( T& syz ) const
{

  syz = this->_directItems[ 2 ][ 1 ];
                            
}


template < class T >
void gkg::ShearingYZ3d< T >::setInverseShearingYZ( const T& syz,
                                                   bool update,
                                                   bool notify )
{

  try
  {

    this->setInverseItems( 1.0, 0.0, 0.0, 0.0,
                           0.0, 1.0, 0.0, 0.0,
                           0.0, syz, 1.0, 0.0,
                           0.0, 0.0, 0.0, 1.0,
                           update, notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::ShearingYZ3d< T >::setInverseShearingYZ( const T& syz, "
             "bool update, bool notify )" );

}


template < class T >
void gkg::ShearingYZ3d< T >::getInverseShearingYZ( T& syz ) const
{

  syz = this->_inverseItems[ 2 ][ 1 ];
                            
}


template class gkg::ShearingYZ3d< float >;
template class gkg::ShearingYZ3d< double >;
