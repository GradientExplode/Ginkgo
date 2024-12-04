#ifndef _gkg_processing_algobase_NonLocalMeansContext_h_
#define _gkg_processing_algobase_NonLocalMeansContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-coordinates/Vector3d.h>

#include <vector>


namespace gkg
{


template < class T > class Volume;
class NLMNoiseModel;


template < class T >
class NonLocalMeansContext : public LoopContext< int32_t >
{

  public:

    NonLocalMeansContext( const Volume< T >& in,
                          float* out,
                          float* M,
                          float* Z,
                          const NLMNoiseModel& noiseModel,
                          const std::vector< Vector3d< int32_t > >& block, 
                          const int32_t& halfNeighborhoodSize, 
                          const float& degreeOfFiltering,
                          const int32_t& t );

    void doIt( int32_t startIndex, int32_t count );

  private:

    float expLut( float dif );

    const Volume< T >& _in;
    float* _out;
    float* _M;
    float* _Z;
    const NLMNoiseModel& _noiseModel;
    const std::vector< Vector3d< int32_t > >& _block;
    const int32_t& _halfNeighborhoodSize;
    const int32_t& _t;
    float _h;
    float _expLut[ 30000 ];

};


}


#endif
