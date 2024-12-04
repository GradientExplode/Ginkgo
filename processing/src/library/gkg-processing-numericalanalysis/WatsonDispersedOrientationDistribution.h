#ifndef _gkg_processing_numericalanalysis_WatsonDispersedOrientationDistribution_h_
#define _gkg_processing_numericalanalysis_WatsonDispersedOrientationDistribution_h_


#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-coordinates/Vector3d.h>


namespace gkg
{


template < class T >
class WatsonDispersedOrientationDistribution
{

  public:

    WatsonDispersedOrientationDistribution(
                                         double kappa,
                                         const Vector3d< T >& meanOrientation );
    virtual ~WatsonDispersedOrientationDistribution();

    void setRandomSeedWithTime();
    void getRandomOrientation( Vector3d< T >& point ) const;

  protected:

    double _kappa;
    Vector3d< T > _meanOrientation;
    RandomGenerator _randomGenerator;
    NumericalAnalysisImplementationFactory* _factory;

};



}



#endif

