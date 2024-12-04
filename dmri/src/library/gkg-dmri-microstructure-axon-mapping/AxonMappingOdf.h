#ifndef _gkg_dmri_microstructure_axon_mapping_AxonMappingOdf_h_
#define _gkg_dmri_microstructure_axon_mapping_AxonMappingOdf_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-dmri-microstructure-axon-mapping/AxonMappingMCMCFunction.h>
#include <gkg-dmri-microstructure-axon-mapping/AxonMappingMCMCParameterSet.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;
class Matrix;


class AxonMappingOdf : public OrientationDistributionFunction
{

  public:

    template < class S >
    AxonMappingOdf( const OrientationSet* orientationSet,
                    const Volume< float >& t2,
                    const Volume< float >& dw,
                    const Vector3d< S >& site,
                    RCPointer< AxonMappingMCMCFunction >
                                                        axonMappingMCMCFunction,
                    RCPointer< AxonMappingMCMCParameterSet >
                                                    axonMappingMCMCParameterSet,
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

    virtual ~AxonMappingOdf();

    const RCPointer< AxonMappingMCMCParameterSet >&
                                         getAxonMappingMCMCParameterSet() const;

  protected:

    RCPointer< AxonMappingMCMCParameterSet > _axonMappingMCMCParameterSet;


};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for AxonMappingOdf
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::AxonMappingOdf, gkg_AxonMappingOdf );


#endif
