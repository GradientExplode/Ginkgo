#ifndef _gkg_dmri_microstructure_axon_mapping_AxonMappingMCMCFunction_h_
#define _gkg_dmri_microstructure_axon_mapping_AxonMappingMCMCFunction_h_


#include <gkg-dmri-microstructure-model/MicrostructureMCMCFunction.h>


namespace gkg
{


class AxonMappingMCMCFunction : public MicrostructureMCMCFunction
{

  public:

    AxonMappingMCMCFunction( RCPointer< Volume< float > > dw,
                             const OrientationSet& outputOrientationSet,
                             double noiseStandardDeviation );
    virtual ~AxonMappingMCMCFunction();

};


}


#endif
