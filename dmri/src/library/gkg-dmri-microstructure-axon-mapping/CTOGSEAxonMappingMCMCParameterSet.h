#ifndef _gkg_dmri_microstructure_axon_mapping_CTOGSEAxonMappingMCMCParameterSet_h_
#define _gkg_dmri_microstructure_axon_mapping_CTOGSEAxonMappingMCMCParameterSet_h_


#include <gkg-dmri-microstructure-axon-mapping/AxonMappingMCMCParameterSet.h>


namespace gkg
{


class CTOGSEAxonMappingMCMCParameterSet : public AxonMappingMCMCParameterSet
{

  public:

    CTOGSEAxonMappingMCMCParameterSet( const Vector& lowerBounds,
                                     const Vector& upperBounds,
                                     const std::vector< bool >& fixed,
                                     const Vector& steps );
    virtual ~CTOGSEAxonMappingMCMCParameterSet();

    // return true if the new parameters are well located within their lower and
    // upper bounds, and return false if not
    bool updateParameters();

};


}


#endif
