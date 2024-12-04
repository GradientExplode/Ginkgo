#ifndef _gkg_processing_algobase_NonLocalMeansInPainterMemoryMappingContext_h_
#define _gkg_processing_algobase_NonLocalMeansInPainterMemoryMappingContext_h_


#include <gkg-processing-container/Volume.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-algobase/NLMNoiseModel.h>
#include <gkg-core-pattern/RCPointer.h>

#include <vector>


namespace gkg
{



template < class IN, class OUT >
class NonLocalMeansInPainterMemoryMappingContext
{

  public:

    NonLocalMeansInPainterMemoryMappingContext(
                              const Volume< IN >& inputVolume,
                              Volume< OUT >& outputVolume,
                              const Volume< int16_t >& maskVolume,
                              const Volume< int16_t >& inPaintingMaskVolume,
                              Volume< float >& outputSubVolume,
                              Volume< float >& M,
                              Volume< float >& Z,
                              const RCPointer< NLMNoiseModel >& noiseModel,
                              const std::vector< Vector3d< int32_t > >& offsets, 
                              const int32_t& halfNeighborhoodSize, 
                              const float& degreeOfFiltering,
                              const int32_t& t );
    virtual ~NonLocalMeansInPainterMemoryMappingContext();

    void execute();

  private:

    float expLut( float dif );

    const Volume< IN >& _in;
    Volume< OUT >& _out;
    Volume< float >& _volOut;
    Volume< float >& _M;
    Volume< float >& _Z;
    const NLMNoiseModel& _noiseModel;
    const std::vector< Vector3d< int32_t > >& _block;
    const int32_t& _halfNeighborhoodSize;
    const int32_t& _t;
    float _h;
    float _expLut[ 30000 ];

};


}


#endif
