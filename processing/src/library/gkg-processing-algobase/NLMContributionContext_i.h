#ifndef _gkg_processing_algobase_NLMContributionContext_i_h_
#define _gkg_processing_algobase_NLMContributionContext_i_h_


#include <gkg-processing-algobase/NLMContributionContext.h>
#include <gkg-processing-algobase/NLMNoiseModel.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


template < class IN, class OUT >
inline
gkg::NLMContributionContext< IN, OUT >::NLMContributionContext( 
                                           const gkg::Volume< IN >& in,
                                           gkg::Volume< OUT >& out,
                                           float* volTmp,
                                           float* M,
                                           float* Z,
                                           const gkg::NLMNoiseModel& noiseModel,
                                           const int32_t& t )
                                      : gkg::LoopContext< int32_t >(),
                                        _in( in ),
                                        _out( out ),
                                        _tmp( volTmp ),
                                        _M( M ),
                                        _Z( Z ),
                                        _noiseModel( noiseModel ),
                                        _t( t )
{
}


template < class IN, class OUT >
inline
void gkg::NLMContributionContext< IN, OUT >::doIt( int32_t startIndex,
                                                   int32_t count )
{

  try
  {

    const IN* pI = &_in( 0, 0, 0, _t ) + startIndex;
    OUT* pO = &_out( 0, 0, 0, _t ) + startIndex;
    float* fpO = _tmp + startIndex;
    float* pM = _M + startIndex;
    float* pZ = _Z + startIndex;
    float N, valueOut, f;

    while ( count-- )
    {

      N = *pZ++ + *pM;
      valueOut = ( *fpO++ + _noiseModel.getValue( *pM++, float( *pI ) ) ) / N;
      f = _noiseModel.getFiltered( valueOut );
      *pO++ = OUT( ( f >= 0.0f ) ? f : *pI );
      pI++;

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "void gkg::NLMContributionContext< IN, OUT >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


#endif
