#include <gkg-processing-transform/Translation3d.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::Translation3d< T >::Translation3d( const T& tx, const T& ty, const T& tz,
                                        bool direct )
                        : gkg::HomogeneousTransform3d< T >()
{

  try
  {

    if ( direct )
    {

      setDirectTranslation( tx, ty, tz, true, false );

    }
    else
    {

      setInverseTranslation( tx, ty, tz, true, false );

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::Translation3d< T >::Translation3d( const T& tx, "
             "const T& ty, const T& tz, bool direct )" );

}


template < class T >
gkg::Translation3d< T >::Translation3d( const gkg::Vector3d< T >& translation,
                                        bool direct )
                        : gkg::HomogeneousTransform3d< T >()
{

  try
  {

    if ( direct )
    {

      setDirectTranslation( translation, true, false );

    }
    else
    {

      setInverseTranslation( translation, true, false );

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::Translation3d< T >::Translation3d( "
             "const gkg::Vector3d< T >& translation, bool direct )" );

}


template < class T >
gkg::Translation3d< T >::Translation3d( const gkg::Translation3d< T >& other )
                        : gkg::HomogeneousTransform3d< T >( other )
{
}


template < class T >
gkg::Translation3d< T >::~Translation3d()
{
}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > > gkg::Translation3d< T >::clone() const
{

  gkg::RCPointer< Transform3d< T > > transform;
  transform.reset( new gkg::Translation3d< T >( *this ) );
  return transform;

}


template < class T >
void gkg::Translation3d< T >::setDirectTranslation( const T& tx,
                                                    const T& ty,
                                                    const T& tz,
                                                    bool update,
                                                    bool notify )
{

  try
  {

    this->setDirectItems( 1.0, 0.0, 0.0, tx,
                          0.0, 1.0, 0.0, ty,
                          0.0, 0.0, 1.0, tz,
                          0.0, 0.0, 0.0, 1.0,
                          update,
                          notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Translation3d< T >::setDirectTranslation( const T& tx, "
             "const T& ty, "
             "const T& tz, "
             "bool update, "
             "bool notify )" );

}


template < class T >
void gkg::Translation3d< T >::setDirectTranslation(
                                         const gkg::Vector3d< T >& translation,
                                         bool update,
                                         bool notify )
{

  try
  {

    this->setDirectItems( 1.0, 0.0, 0.0, translation.x,
                          0.0, 1.0, 0.0, translation.y,
                          0.0, 0.0, 1.0, translation.z,
                          0.0, 0.0, 0.0, 1.0,
                          update,
                          notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Translation3d< T >::setDirectTranslation( "
             "const gkg::Vector3d< T >& translation, "
             "bool update, "
             "bool notify )" );

}


template < class T >
void gkg::Translation3d< T >::getDirectTranslation( T& tx, T& ty, T& tz ) const
{

  tx = this->_directItems[ 0 ][ 3 ];
  ty = this->_directItems[ 1 ][ 3 ];
  tz = this->_directItems[ 2 ][ 3 ];
                            
}


template < class T >
void gkg::Translation3d< T >::getDirectTranslation(
                                         gkg::Vector3d< T >& translation ) const
{

  getDirectTranslation( translation.x, translation.y, translation.z );
                            
}


template < class T >
void gkg::Translation3d< T >::setInverseTranslation( const T& tx,
                                                     const T& ty,
                                                     const T& tz,
                                                     bool update,
                                                     bool notify )
{

  try
  {

    this->setInverseItems( 1.0, 0.0, 0.0, tx,
                           0.0, 1.0, 0.0, ty,
                           0.0, 0.0, 1.0, tz,
                           0.0, 0.0, 0.0, 1.0,
                           update,
                           notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Translation3d< T >::setInverseTranslation( const T& tx,"
             " const T& ty, "
             "const T& tz, "
             "bool update, "
             "bool notify )" );

}


template < class T >
void gkg::Translation3d< T >::setInverseTranslation(
                                         const gkg::Vector3d< T >& translation,
                                         bool update,
                                         bool notify )
{

  try
  {

    this->setInverseItems( 1.0, 0.0, 0.0, translation.x,
                           0.0, 1.0, 0.0, translation.y,
                           0.0, 0.0, 1.0, translation.z,
                           0.0, 0.0, 0.0, 1.0,
                           update,
                           notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Translation3d< T >::setInverseTranslation( "
             "const gkg::Vector3d< T >& translation, "
             "bool update, "
             "bool notify )" );

}


template < class T >
void gkg::Translation3d< T >::getInverseTranslation( T& tx, T& ty, T& tz ) const
{

  tx = this->_inverseItems[ 0 ][ 3 ];
  ty = this->_inverseItems[ 1 ][ 3 ];
  tz = this->_inverseItems[ 2 ][ 3 ];
                            
}


template < class T >
void gkg::Translation3d< T >::getInverseTranslation(
                                         gkg::Vector3d< T >& translation ) const
{

  getInverseTranslation( translation.x, translation.y, translation.z );
                            
}


template class gkg::Translation3d< float >;
template class gkg::Translation3d< double >;
