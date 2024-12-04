#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::Transform3d< T >::Transform3d()
                      : gkg::Observable()
{
}


template < class T >
gkg::Transform3d< T >::~Transform3d()
{

  notifyUnregisterObservers();

}


template < class T >
void gkg::Transform3d< T >::getIntegerDirect(
                                            const gkg::Vector3d< T >& from, 
                                            gkg::Vector3d< int32_t >& to ) const
{

  try
  {

    gkg::Vector3d< T > tmp;
    this->getDirect( from, tmp );
    to.x = ( int32_t )gkg::round( tmp.x );
    to.y = ( int32_t )gkg::round( tmp.y );
    to.z = ( int32_t )gkg::round( tmp.z );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Transform3d< T >::getIntegerDirect( "
             "const gkg::Vector3d< T >& from, "
             "gkg::Vector3d< int32_t >& to ) const" );

}


template < class T >
void gkg::Transform3d< T >::getIntegerInverse(
                                          const gkg::Vector3d< T >& to, 
                                          gkg::Vector3d< int32_t >& from ) const
{

  try
  {

    gkg::Vector3d< T > tmp;
    this->getInverse( to, tmp );
    from.x = ( int32_t )gkg::round( tmp.x );
    from.y = ( int32_t )gkg::round( tmp.y );
    from.z = ( int32_t )gkg::round( tmp.z );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Transform3d< T >::getIntegerInverse( "
             "const gkg::Vector3d< T >& to, "
             "gkg::Vector3d< int32_t >& from ) const" );

}


template < class T >
void gkg::Transform3d< T >::getFullIntegerDirect(
                                           const gkg::Vector3d< int32_t >& from, 
                                           gkg::Vector3d< int32_t >& to ) const
{

  try
  {

    gkg::Vector3d< T > tmp;
    this->getDirect( gkg::Vector3d< T >( ( T )from.x,
                                         ( T )from.y,
                                         ( T )from.z ), tmp );
    to.x = ( int32_t )gkg::round( tmp.x );
    to.y = ( int32_t )gkg::round( tmp.y );
    to.z = ( int32_t )gkg::round( tmp.z );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Transform3d< T >::getFullIntegerDirect( "
             "const gkg::Vector3d< int32_t >& from, "
             "gkg::Vector3d< int32_t >& to ) const" );

}


template < class T >
void gkg::Transform3d< T >::getFullIntegerInverse(
                                          const gkg::Vector3d< int32_t >& to, 
                                          gkg::Vector3d< int32_t >& from ) const
{

  try
  {

    gkg::Vector3d< T > tmp;
    this->getInverse( gkg::Vector3d< T >( ( T )to.x,
                                          ( T )to.y,
                                          ( T )to.z ), tmp );
    from.x = ( int32_t )gkg::round( tmp.x );
    from.y = ( int32_t )gkg::round( tmp.y );
    from.z = ( int32_t )gkg::round( tmp.z );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Transform3d< T >::getFullIntegerInverse( "
             "const gkg::Vector3d< int32_t >& to, "
             "gkg::Vector3d< int32_t >& from ) const" );

}


template < class T >
T gkg::Transform3d< T >::getDerivativeAlongX(
                                    const gkg::Vector3d< T >& /* from */ ) const
{

  try
  {

    throw std::runtime_error( "not implemented" );
    return 0;

  }
  GKG_CATCH( "template < class T > "
             "T gkg::Transform3d< T >::getDerivativeAlongX( "
             "const gkg::Vector3d< T >& from ) const" );

}


template < class T >
T gkg::Transform3d< T >::getDerivativeAlongY(
                                    const gkg::Vector3d< T >& /* from */ ) const
{

  try
  {

    throw std::runtime_error( "not implemented" );
    return 0;

  }
  GKG_CATCH( "template < class T > "
             "T gkg::Transform3d< T >::getDerivativeAlongY( "
             "const gkg::Vector3d< T >& from ) const" );

}


template < class T >
T gkg::Transform3d< T >::getDerivativeAlongZ(
                                    const gkg::Vector3d< T >& /* from */ ) const
{

  try
  {

    throw std::runtime_error( "not implemented" );
    return 0;

  }
  GKG_CATCH( "template < class T > "
             "T gkg::Transform3d< T >::getDerivativeAlongZ( "
             "const gkg::Vector3d< T >& from ) const" );

}


template < class T >
void gkg::Transform3d< T >::getDerivativeAlongXYZ(
                            const gkg::Vector3d< T >& /* from */,
                            gkg::Vector3d< T >& /* derivativeAlongXYZ */ ) const
{

  try
  {

    throw std::runtime_error( "not implemented" );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Transform3d< T >::getDerivativeAlongXYZ( "
             "const gkg::Vector3d< T >& from, "
             "gkg::Vector3d< T >& derivativeAlongXYZ ) const" );

}


template < class T >
bool gkg::Transform3d< T >::isComposable() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "template < class T > "
             "bool gkg::Transform3d< T >::isComposable() const" );

}


template < class T >
bool gkg::Transform3d< T >::hasDerivativeAlongX() const
{


  try
  {

    return false;

  }
  GKG_CATCH( "template < class T > "
             "bool gkg::Transform3d< T >::hasDerivativeAlongX() const" );


}


template < class T >
bool gkg::Transform3d< T >::hasDerivativeAlongY() const
{


  try
  {

    return false;

  }
  GKG_CATCH( "template < class T > "
             "bool gkg::Transform3d< T >::hasDerivativeAlongY() const" );


}


template < class T >
bool gkg::Transform3d< T >::hasDerivativeAlongZ() const
{


  try
  {

    return false;

  }
  GKG_CATCH( "template < class T > "
             "bool gkg::Transform3d< T >::hasDerivativeAlongZ() const" );


}


template < class T >
bool gkg::Transform3d< T >::hasDerivativeAlongXYZ() const
{


  try
  {

    return false;

  }
  GKG_CATCH( "template < class T > "
             "bool gkg::Transform3d< T >::hasDerivativeAlongXYZ() const" );


}


template < class T >
bool gkg::Transform3d< T >::isIndirect() const
{


  try
  {

    return false;

  }
  GKG_CATCH( "template < class T > "
             "bool gkg::Transform3d< T >::isIndirect() const" );


}


template class gkg::Transform3d< float >;
template class gkg::Transform3d< double >;
