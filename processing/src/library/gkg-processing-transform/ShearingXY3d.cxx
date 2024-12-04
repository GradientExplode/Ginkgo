#include <gkg-processing-transform/ShearingXY3d.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::ShearingXY3d< T >::ShearingXY3d( const T& sxy, bool direct )
                       : gkg::HomogeneousTransform3d< T >()
{

  try
  {

    if ( direct )
    {

      setDirectShearingXY( sxy, true, false );

    }
    else
    {

      setInverseShearingXY( sxy, true, false );

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::ShearingXY3d< T >::ShearingXY3d( const T& sxy, "
             "bool direct )" );

}


template < class T >
gkg::ShearingXY3d< T >::ShearingXY3d( const gkg::ShearingXY3d< T >& other )
                       : gkg::HomogeneousTransform3d< T >( other )
{
}


template < class T >
gkg::ShearingXY3d< T >::~ShearingXY3d()
{
}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > > gkg::ShearingXY3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset( new gkg::ShearingXY3d< T >( *this ) );
  return transform;

}


template < class T >
void gkg::ShearingXY3d< T >::setDirectShearingXY( const T& sxy,
                                                  bool update,
                                                  bool notify )
{

  try
  {

    this->setDirectItems( 1.0, 0.0, 0.0, 0.0,
                          sxy, 1.0, 0.0, 0.0,
                          0.0, 0.0, 1.0, 0.0,
                          0.0, 0.0, 0.0, 1.0,
                          update, notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::ShearingXY3d< T >::setDirectShearingXY( const T& sxy, "
             "bool update, bool notify )" );

}


template < class T >
void gkg::ShearingXY3d< T >::getDirectShearingXY( T& sxy ) const
{

  sxy = this->_directItems[ 1 ][ 0 ];
                            
}


template < class T >
void gkg::ShearingXY3d< T >::setInverseShearingXY( const T& sxy,
                                                   bool update,
                                                   bool notify )
{

  try
  {

    this->setInverseItems( 1.0, 0.0, 0.0, 0.0,
                           sxy, 1.0, 0.0, 0.0,
                           0.0, 0.0, 1.0, 0.0,
                           0.0, 0.0, 0.0, 1.0,
                           update, notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::ShearingXY3d< T >::setInverseShearingXY( const T& sxy, "
             "bool update, bool notify )" );

}


template < class T >
void gkg::ShearingXY3d< T >::getInverseShearingXY( T& sxy ) const
{

  sxy = this->_inverseItems[ 1 ][ 0 ];
                            
}


template class gkg::ShearingXY3d< float >;
template class gkg::ShearingXY3d< double >;
