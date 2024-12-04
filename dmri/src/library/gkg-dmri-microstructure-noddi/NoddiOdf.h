#ifndef _gkg_dmri_microstructure_noddi_NoddiOdf_h_
#define _gkg_dmri_microstructure_noddi_NoddiOdf_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-dmri-microstructure-noddi/NoddiMCMCFunction.h>
#include <gkg-dmri-microstructure-noddi/NoddiMCMCParameterSet.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;
class Matrix;


class NoddiOdf : public OrientationDistributionFunction
{

  public:

    // for standard estimation on the acquired grid
    NoddiOdf( const OrientationSet* orientationSet,
              const Volume< float >& t2,
              const Volume< float >& dw,
              const Vector3d< int32_t >& site,
              RCPointer< NoddiMCMCFunction > noddiMCMCFunction,
              RCPointer< NoddiMCMCParameterSet > noddiMCMCParameterSet,
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
    NoddiOdf( const OrientationSet* orientationSet,
              const Volume< float >& t2,
              const Volume< float >& dw,
              const Vector3d< float >& site,
              RCPointer< NoddiMCMCFunction > noddiMCMCFunction,
              RCPointer< NoddiMCMCParameterSet > noddiMCMCParameterSet,
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

    virtual ~NoddiOdf();

    const RCPointer< NoddiMCMCParameterSet >& getNoddiMCMCParameterSet() const;

  protected:

    RCPointer< NoddiMCMCParameterSet > _noddiMCMCParameterSet;


};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for NoddiOdf
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::NoddiOdf, gkg_NoddiOdf );


#endif
