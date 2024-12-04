#ifndef _gkg_processing_algobase_NonLocalMeansContextMmap_h_
#define _gkg_processing_algobase_NonLocalMeansContextMmap_h_


#include <gkg-processing-coordinates/Vector3d.h>

#include <vector>


namespace gkg
{


template < class T > class Volume;
class NLMNoiseModel;


template < class IN, class OUT >
class NonLocalMeansContextMmap
{

  public:

    NonLocalMeansContextMmap( const Volume< IN >& in,
                              Volume< OUT >& out,
                              Volume< float >& volOut,
                              Volume< float >& M,
                              Volume< float >& Z,
                              const NLMNoiseModel& noiseModel,
                              const std::vector< Vector3d< int32_t > >& block, 
                              const int32_t& halfNeighborhoodSize, 
                              const float& degreeOfFiltering,
                              const int32_t& t );
    virtual ~NonLocalMeansContextMmap();

    void execute();

  private:

    float expLut( float dif ) const;

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
