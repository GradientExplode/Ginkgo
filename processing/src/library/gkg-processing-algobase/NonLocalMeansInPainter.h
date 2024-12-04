#ifndef _gkg_processing_algobase_NonLocalMeansInPainter_h_
#define _gkg_processing_algobase_NonLocalMeansInPainter_h_


#include <gkg-processing-algobase/NLMNoiseModel.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


template < class IN, class OUT >
class NonLocalMeansInPainter
{

  public:

    NonLocalMeansInPainter( const RCPointer< NLMNoiseModel >& noiseModel,
                            int32_t halfSearchBlockSize,
                            int32_t halfNeighborhoodSize, 
                            float degreeOfFiltering,
                            bool verbose );
    virtual ~NonLocalMeansInPainter();

    void inPaint( const Volume< IN >& inputVolume,
                  const Volume< int16_t >& maskVolume,
                  const Volume< int16_t >& inPaintingMaskVolume,
                  Volume< OUT >& outputVolume ) const;

  private:

    RCPointer< NLMNoiseModel > _noiseModel;
    int32_t _halfSearchBlockSize;
    int32_t _halfNeighborhoodSize;
    float _degreeOfFiltering;
    bool _verbose;

};


}


#endif
