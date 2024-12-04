#include <gkg-processing-numericalanalysis/WatsonDispersedOrientationDistribution.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::WatsonDispersedOrientationDistribution< T >::WatsonDispersedOrientationDistribution(
                                     double kappa,
                                     const gkg::Vector3d< T >& meanOrientation )
                                                 : _kappa( kappa ),
                                                   _meanOrientation(
                                                              meanOrientation ),
                                                   _randomGenerator(
                                                    gkg::RandomGenerator::Taus )
{

  try
  {

    if ( _kappa <= ( T )0.0 )
    {

      throw std::runtime_error( "kappa must be strictly positive" );

    }
    _meanOrientation.normalize();
    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

  }
  GKG_CATCH( "template < class T > "
             "gkg::WatsonDispersedOrientationDistribution< T >::"
             "WatsonDispersedOrientationDistribution( "
             "double kappa, "
             "const gkg::Vector3d< T >& meanOrientation)" );

}


template < class T >
gkg::WatsonDispersedOrientationDistribution< T >::
                                       ~WatsonDispersedOrientationDistribution()
{
}


template < class T >
void gkg::WatsonDispersedOrientationDistribution< T >::setRandomSeedWithTime()
{

  try
  {

    _randomGenerator.setSeedWithTime();


  }
  GKG_CATCH( "template < class T > "
             "void gkg::WatsonDispersedOrientationDistribution< T >::"
             "setRandomSeedWithTime()" );

}

template < class T >
void gkg::WatsonDispersedOrientationDistribution< T >::getRandomOrientation(
                                         gkg::Vector3d< T >& orientation ) const
{

  try
  {

    _factory->getRandomWatsonOrientation( _kappa,
                                          _meanOrientation,
                                          _randomGenerator,
                                          orientation );

  }
  GKG_CATCH( "template < class T > "
             "void "
             "gkg::WatsonDispersedOrientationDistribution< T >::"
             "getRandomOrientation( "
             "const gkg::Vector3d< T >& orientation ) const" );

}


template class gkg::WatsonDispersedOrientationDistribution< float >;
template class gkg::WatsonDispersedOrientationDistribution< double >;

