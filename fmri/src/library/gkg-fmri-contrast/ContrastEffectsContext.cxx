#include <gkg-fmri-contrast/ContrastEffectsContext.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


gkg::ContrastEffectsContext::ContrastEffectsContext(
                       const gkg::Volume< gkg::Kalman::Parameters* >& kalmanLut,
                       const std::vector< gkg::Vector3d< int32_t > >& sites,
                       const gkg::Matrix& C,
                       gkg::Volume< float >& volumeOfEffects,
                       gkg::Volume< float >& volumeOfEffectVariances )
                           : gkg::LoopContext< int32_t >(),
                             _kalmanLut( kalmanLut ),
                             _sites( sites ),
                             _C( C ),
                             _volumeOfEffects( volumeOfEffects ),
                             _volumeOfEffectVariances(
                                                      volumeOfEffectVariances ),
                             _contrastCount( volumeOfEffects.getSizeT() )
{

  try
  {

    _Ct = C.getTransposition();

  }
  GKG_CATCH( "gkg::ContrastEffectsContext::ContrastEffectsContext( "
             "gkg::Volume< gkg::Kalman::Parameters* >& kalmanLut, "
             "const std::vector< gkg::Vector3d< int32_t > >& sites, "
             "const gkg::Matrix& C, "
             "gkg::Volume< float >& volumeOfEffects, "
             "gkg::Volume< float >& volumeOfEffectVariances )" );

}


void gkg::ContrastEffectsContext::doIt( int32_t startIndex, int32_t count )
{

  try
  {

    int32_t contrast, contrast1, contrast2;
    std::vector< gkg::Vector3d< int32_t > >::const_iterator
      s = _sites.begin() + startIndex,
      se = s + count;

    while ( s != se )
    {

      gkg::Kalman::Parameters* parameters = _kalmanLut( *s );

      // calculating effects
      gkg::Vector effects = _Ct.getComposition( parameters->X );

      // calculating variance of effects
      gkg::Matrix variances = _Ct.getComposition( parameters->varianceOfX ).
        			  getComposition( _C );

      for ( contrast = 0, contrast1 = 0; 
            contrast1 < _contrastCount;
            contrast1++ )
      {

        _volumeOfEffects( *s, contrast1 ) = ( float )effects( contrast1 );

        for ( contrast2 = 0; 
              contrast2 < _contrastCount;
              contrast2++, contrast++ )
        {

          _volumeOfEffectVariances( *s, contrast ) =
        		    ( float )( variances( contrast1, contrast2 ) *
        			       parameters->squareScaleParameter );

        }

      }

      ++s;

    }
  }
  GKG_CATCH( "void gkg::ContrastEffectsContext::doIt( "
             "int32_t startIndex, int32_t count )" );

}
