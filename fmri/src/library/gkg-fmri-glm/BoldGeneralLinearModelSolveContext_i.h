#ifndef _gkg_fmri_glm_BoldGeneralLinearModelSolveContext_i_h_
#define _gkg_fmri_glm_BoldGeneralLinearModelSolveContext_i_h_


#include <gkg-fmri-glm/BoldGeneralLinearModelSolveContext.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
inline
gkg::BoldGeneralLinearModelSolveContext< T >::
                                             BoldGeneralLinearModelSolveContext(
                           gkg::KalmanFilter& kalmanFilter,
                           const gkg::Volume< T >& volume,
                           gkg::Volume< gkg::Kalman::Parameters* >& kalmanLut,
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           int32_t t,
                           int32_t dummyScanCount )
                                            : gkg::LoopContext< int32_t >(),
                                              _kalmanFilter( kalmanFilter ),
                                              _volume( volume ),
                                              _kalmanLut( kalmanLut ),
                                              _sites( sites ),
                                              _t( t ),
                                              _dummyScanCount( dummyScanCount )
{
}


template < class T >
inline
void gkg::BoldGeneralLinearModelSolveContext< T >::doIt( int32_t startIndex,
                                                         int32_t count )
{

  try
  {

    std::vector< gkg::Vector3d< int32_t > >::const_iterator
      s = _sites.begin() + startIndex,
      se = s + count;

    while ( s != se )
    {

       _kalmanFilter.iterate( *_kalmanLut( *s ), 
                              _volume( *s, _t + _dummyScanCount ),
                              _t,
                              false );

      ++s;

    }
  
  }
  GKG_CATCH( "template < class T >"
             "inline"
             "void gkg::BoldGeneralLinearModelSolveContext< T >::doIt( "
	     "int32_t startIndex, int32_t count )" );

}


#endif
