#include <gkg-processing-numericalanalysis/RandomPointOverSphereDistribution.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::RandomPointOverSphereDistribution< T >::RandomPointOverSphereDistribution(
                                                          const T& radius,
                                                          double multiplicator )
                                            : _radius( radius ),
                                              _multiplicator( multiplicator ),
                                              _randomGenerator(
                                                    gkg::RandomGenerator::Taus )
{

  try
  {

    if ( _radius <= ( T )0.0 )
    {

      throw std::runtime_error( "radius must be strictly positive" );

    }
    if ( _multiplicator <= ( T )0.0 )
    {

      throw std::runtime_error( "multiplicator must be strictly positive" );

    }
    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

  }
  GKG_CATCH( "template < class T > "
             "gkg::RandomPointOverSphereDistribution< T >::"
             "RandomPointOverSphereDistribution( "
             "const T& radius, "
             "double multiplicator)" );

}


template < class T >
gkg::RandomPointOverSphereDistribution< T >::
                                            ~RandomPointOverSphereDistribution()
{
}


template < class T >
void gkg::RandomPointOverSphereDistribution< T >::getRandomPoint(
                                               gkg::Vector3d< T >& point ) const
{

  try
  {

    point.x = ( T )_factory->getGaussianRandomNumber( _randomGenerator, 
                                                      0.0, _multiplicator ) /
                                                      _multiplicator;
    point.y = ( T )_factory->getGaussianRandomNumber( _randomGenerator, 
                                                      0.0, _multiplicator ) /
                                                      _multiplicator;
    point.z = ( T )_factory->getGaussianRandomNumber( _randomGenerator, 
                                                      0.0, _multiplicator ) /
                                                      _multiplicator;

    point.normalize();

    point *= _radius;

  }
  GKG_CATCH( "template < class T > "
             "void "
             "gkg::RandomPointOverSphereDistribution< T >::getRandomPoint( "
             "const gkg::Vector3d< T >& point ) const" );

}


template class gkg::RandomPointOverSphereDistribution< float >;
template class gkg::RandomPointOverSphereDistribution< double >;

