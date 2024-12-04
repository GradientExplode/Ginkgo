#ifndef _gkg_dmri_microstructure_noddi_NoddiMCMCWatsonParameterSet_h_
#define _gkg_dmri_microstructure_noddi_NoddiMCMCWatsonParameterSet_h_


#include <gkg-dmri-microstructure-noddi/NoddiMCMCParameterSet.h>


namespace gkg
{


class NoddiMCMCWatsonParameterSet : public NoddiMCMCParameterSet
{

  public:

    NoddiMCMCWatsonParameterSet( const Vector& lowerBounds,
                                 const Vector& upperBounds,
                                 const std::vector< bool >& fixed,
                                 const Vector& steps );
    virtual ~NoddiMCMCWatsonParameterSet();

    // return true if the new parameters are well located within their lower and
    // upper bounds, and return false if not
    bool updateParameters();

};


}


#endif
