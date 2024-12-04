#ifndef _gkg_processing_algobase_NonLocalMeansFilter_h_
#define _gkg_processing_algobase_NonLocalMeansFilter_h_


namespace gkg
{


template< class T > class Volume;
class NLMNoiseModel;


template < class IN, class OUT >
class NonLocalMeansFilter
{

  public:

    NonLocalMeansFilter( const NLMNoiseModel& noiseModel,
                         int32_t halfSearchBlockSize,
                         int32_t halfNeighborhoodSize, 
                         float degreeOfFiltering );
    virtual ~NonLocalMeansFilter();

    void filter( const Volume< IN >& in, Volume< OUT >& out ) const;

  private:

    const NLMNoiseModel& _noiseModel;
    int32_t _halfSearchBlockSize;
    int32_t _halfNeighborhoodSize;
    float _degreeOfFiltering;

};


}


#endif
