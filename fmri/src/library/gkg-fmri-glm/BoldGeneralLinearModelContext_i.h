#ifndef _gkg_fmri_glm_BoldGeneralLinearModelContext_i_h_
#define _gkg_fmri_glm_BoldGeneralLinearModelContext_i_h_


#include <gkg-fmri-glm/BoldGeneralLinearModelContext.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
inline
gkg::BoldGeneralLinearModelContext< T >::BoldGeneralLinearModelContext(
                           gkg::KalmanFilter& kalmanFilter,
                           const gkg::Volume< T >& volume,
                           gkg::Volume< gkg::Kalman::Parameters* >& kalmanLut,
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           int32_t t )
                                       : gkg::LoopContext< int32_t >(),
                                         _kalmanFilter( kalmanFilter ),
                                         _volume( volume ),
                                         _kalmanLut( kalmanLut ),
                                         _sites( sites ),
                                         _t( t )
{
}


template < class T >
inline
void gkg::BoldGeneralLinearModelContext< T >::doIt( int32_t startIndex,
                                                    int32_t count )
{

  try
  {

    std::vector< gkg::Vector3d< int32_t > >::const_iterator
      s = _sites.begin() + startIndex,
      se = s + count;

    while ( s != se )
    {

      _kalmanFilter.iterate( *_kalmanLut( *s ), _volume( *s ), _t, false );

      ++s;

    }
  
  }
  GKG_CATCH( "template < class T >"
             "inline"
             "void gkg::BoldGeneralLinearModelContext< T >::doIt( "
	     "int32_t startIndex, int32_t count )" );

}


#endif
