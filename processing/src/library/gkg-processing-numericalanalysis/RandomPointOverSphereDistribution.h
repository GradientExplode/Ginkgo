#ifndef _gkg_processing_numericalanalysis_RandomPointOverSphereDistribution_h_
#define _gkg_processing_numericalanalysis_RandomPointOverSphereDistribution_h_


#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-coordinates/Vector3d.h>


namespace gkg
{


template < class T >
class RandomPointOverSphereDistribution
{

  public:

    RandomPointOverSphereDistribution( const T& radius,
                                       double multiplicator = 1e3 );
    virtual ~RandomPointOverSphereDistribution();

    void getRandomPoint( Vector3d< T >& point ) const;

  protected:


    T _radius;
    double _multiplicator;
    RandomGenerator _randomGenerator;
    NumericalAnalysisImplementationFactory* _factory;

};



}



#endif

