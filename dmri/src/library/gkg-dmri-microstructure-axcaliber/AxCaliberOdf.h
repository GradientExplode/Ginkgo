#ifndef _gkg_dmri_microstructure_axcaliber_AxCaliberOdf_h_
#define _gkg_dmri_microstructure_axcaliber_AxCaliberOdf_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberMCMCFunction.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberMCMCParameterSet.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;
class Matrix;


class AxCaliberOdf : public OrientationDistributionFunction
{

  public:


    //
    // 0 -> HinderedDiffusivity (?m2/s)
    // 1 -> IntracellularFraction
    // 2 -> IntracellularPerpendicularDiffusivity (1.7e-9 m2/s)
    // 3 -> IntracellularParallelDiffusivity (?m2/s)
    // 4 -> DiameterMean
    // 5 -> DiameterStdDev
    //


    // for standard estimation on the acquired grid
    AxCaliberOdf( const OrientationSet* orientationSet,
                  const Volume< float >& t2,
                  const Volume< float >& dw,
                  const Vector3d< int32_t >& site,
                  RCPointer< AxCaliberMCMCFunction > axcaliberMCMCFunction,
                  RCPointer< AxCaliberMCMCParameterSet >
                                                      axcaliberMCMCParameterSet,
                  const gkg::Vector& initialParameters,
                  bool applyQuickSearchOptimizer,
                  int32_t quicksearchStepCount,
                  bool applyNlpOptimizer,
                  int32_t nlpMaximumIterationCount,
                  double nlpMaximumSizeOfTheSimplex,
                  bool applyMcmcOptimizer,
                  int32_t mcmcBurninCount,
                  int32_t mcmcSampleCount,
                  int32_t mcmcIntervalCount,
                  int32_t mcmcMaximumIterationCount );

    // for partial voluming resampled data
    AxCaliberOdf( const OrientationSet* orientationSet,
                  const Volume< float >& t2,
                  const Volume< float >& dw,
                  const Vector3d< float >& site,
                  RCPointer< AxCaliberMCMCFunction > axcaliberMCMCFunction,
                  RCPointer< AxCaliberMCMCParameterSet >
                                                      axcaliberMCMCParameterSet,
                  const gkg::Vector& initialParameters,
                  bool applyQuickSearchOptimizer,
                  int32_t quicksearchStepCount,
                  bool applyNlpOptimizer,
                  int32_t nlpMaximumIterationCount,
                  double nlpMaximumSizeOfTheSimplex,
                  bool applyMcmcOptimizer,
                  int32_t mcmcBurninCount,
                  int32_t mcmcSampleCount,
                  int32_t mcmcIntervalCount,
                  int32_t mcmcMaximumIterationCount );

    virtual ~AxCaliberOdf();

    const RCPointer< AxCaliberMCMCParameterSet >& 
      getAxCaliberMCMCParameterSet() const;

  protected:

    RCPointer< AxCaliberMCMCParameterSet > _axCaliberMCMCParameterSet;


};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for AxCaliberOdf
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::AxCaliberOdf, gkg_AxCaliberOdf );


#endif
