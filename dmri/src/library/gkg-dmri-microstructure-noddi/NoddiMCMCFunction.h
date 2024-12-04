#ifndef _gkg_dmri_microstructure_noddi_NoddiMCMCFunction_h_
#define _gkg_dmri_microstructure_noddi_NoddiMCMCFunction_h_


#include <gkg-dmri-microstructure-model/MicrostructureMCMCFunction.h>


namespace gkg
{


class NoddiMCMCFunction : public MicrostructureMCMCFunction
{

  public:

    NoddiMCMCFunction( RCPointer< Volume< float > > dw,
                       const OrientationSet& outputOrientationSet,
                       double noiseStandardDeviation );
    virtual ~NoddiMCMCFunction();

  protected:

    std::vector< float > _minusBiTimesSquareOfDiDotDo;

};


}


#endif
