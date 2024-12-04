#ifndef _gkg_dmri_odf_charmed_CharmedOdf_h_
#define _gkg_dmri_odf_charmed_CharmedOdf_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-dmri-odf-charmed/CharmedMCMCFunction.h>
#include <gkg-dmri-odf-charmed/CharmedMCMCParameterSet.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;
class Matrix;


class CharmedOdf : public OrientationDistributionFunction
{

  public:

    // for standard estimation on the acquired grid
    CharmedOdf( const OrientationSet* orientationSet,
                const Volume< float >& t2,
                const Volume< float >& dw,
                const Vector3d< int32_t >& site,
                RCPointer< CharmedMCMCFunction > charmedMCMCFunction,
                RCPointer< CharmedMCMCParameterSet > charmedMCMCParameterSet,
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
    CharmedOdf( const OrientationSet* orientationSet,
                const Volume< float >& t2,
                const Volume< float >& dw,
                const Vector3d< float >& site,
                RCPointer< CharmedMCMCFunction > charmedMCMCFunction,
                RCPointer< CharmedMCMCParameterSet > charmedMCMCParameterSet,
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

    virtual ~CharmedOdf();

    const RCPointer< CharmedMCMCParameterSet >& getCharmedMCMCParameterSet() const;

  protected:

    RCPointer< CharmedMCMCParameterSet > _charmedMCMCParameterSet;


};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for CharmedOdf
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::CharmedOdf, gkg_CharmedOdf );


#endif
