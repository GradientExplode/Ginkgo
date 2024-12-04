#include <gkg-processing-transform/TalairachTransform3d_i.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>


template < class T >
gkg::TalairachTransform3d< T >::TalairachTransform3d(
                                   const gkg::TalairachTransform3d< T >& other )
                               : gkg::CompositeTransform3d< T >( other )
{
}

template < class T >
gkg::TalairachTransform3d< T >::~TalairachTransform3d()
{
}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > > gkg::TalairachTransform3d< T >::clone() const
{

  try
  {
  
    gkg::RCPointer< gkg::Transform3d< T > > transform;
    transform.reset( new gkg::TalairachTransform3d< T >( *this ) );
    return transform;

  }
  GKG_CATCH( "template < class T > "
             "gkg::RCPointer< gkg::Transform3d< T > > "
             "gkg::TalairachTransform3d< T >::clone() const" );

}


template < class T >
gkg::TalairachTransform3d< T >& gkg::TalairachTransform3d< T >::operator=(
                                   const gkg::TalairachTransform3d< T >& other )
{

  try
  {
  
    this->gkg::CompositeTransform3d< T >::operator=( other );
    return *this;
  
  }
  GKG_CATCH( "template < class T > "
             "gkg::TalairachTransform3d< T >& "
             "gkg::TalairachTransform3d::operator=( "
             "const gkg::TalairachTransform3d< T >& other )" );

}


template < class T >
gkg::RCPointer< gkg::AffineWoShearingTransform3d< T > >
gkg::TalairachTransform3d< T >::getAffineWoShearingTransform3d() const
{

  try
  {
  
    gkg::Matrix rotationAndScaling( 3, 3 );
    int32_t i = 0;
    int32_t j = 0;
    for ( i = 0; i < 3; i++ )
    {

      for ( j = 0; j < 3; j++ )
      {

        rotationAndScaling( i, j ) = this->getComposition().
                                                   getDirectCoefficient( i, j );

      }

    }
    gkg::Vector3d< T > scalings;
    gkg::Vector3d< T > u;
    gkg::Vector3d< T > v;
    gkg::Vector3d< T > w;
    gkg::getXYZScalingsAndTrieder( rotationAndScaling, scalings, u, v, w );
    gkg::Vector3d< T > angles = gkg::getXYZAnglesFromTrieder( u, v, w );

    gkg::Vector3d< T > translations;
    translations.x = this->getComposition().getDirectCoefficient( 0, 3 );
    translations.y = this->getComposition().getDirectCoefficient( 1, 3 );
    translations.z = this->getComposition().getDirectCoefficient( 2, 3 );

    gkg::RCPointer< gkg::AffineWoShearingTransform3d< T > >
      affineWoShearingTransform3d( new gkg::AffineWoShearingTransform3d< T >(
                                          scalings, angles, translations ) );

    return affineWoShearingTransform3d; 

  }
  GKG_CATCH( "template < class T > "
             "gkg::RCPointer< gkg::AffineWoShearingTransform3d< T > > "
             "gkg::TalairachTransform3d< T >::"
             "getAffineWoShearingTransform3d() const" );

}


template < class T >
gkg::RCPointer< gkg::ParameterizedAffineWoShearingTransform3d< T > >
gkg::TalairachTransform3d< T >::
                             getParameterizedAffineWoShearingTransform3d() const
{

  try
  {
  
    gkg::RCPointer< gkg::ParameterizedAffineWoShearingTransform3d< T > >
      parameterizedAffineWoShearingTransform3d(
        new gkg::ParameterizedAffineWoShearingTransform3d< T >(
                                    *this->getAffineWoShearingTransform3d() ) );

    return parameterizedAffineWoShearingTransform3d;
  }
  GKG_CATCH( "template < class T > "
             "gkg::RCPointer< "
             "gkg::ParameterizedAffineWoShearingTransform3d< T > > "
             "gkg::TalairachTransform3d< T >::"
             "getParameterizedAffineWoShearingTransform3d() const" );

}


template class gkg::TalairachTransform3d< float >;
template class gkg::TalairachTransform3d< double >;
