#ifndef _gkg_fmri_glm_BoldGeneralLinearModelSolver_i_h_
#define _gkg_fmri_glm_BoldGeneralLinearModelSolver_i_h_


#include <gkg-fmri-glm/BoldGeneralLinearModelSolver.h>
#include <gkg-fmri-glm/BoldGeneralLinearModelContext_i.h>
#include <gkg-fmri-glm/BoldGeneralLinearModelSolveContext_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>


template < class T >
void gkg::BoldGeneralLinearModelSolver::solve(
                const gkg::Volume< T >& measures,
                int32_t dummyScanCount,
                const std::vector< int32_t >& sliceOrder,
                gkg::Volume< gkg::Kalman::Parameters* >& kalmanLut,
                const std::vector< gkg::Vector3d< int32_t > >& sites,
                bool verbose )
{

  try
  {

    // sanity checks
    if ( measures.getSizeZ() != ( int32_t )sliceOrder.size() )
    {

      throw std::runtime_error( "inconsistent slice count" );

    }

    if ( measures.getSizeT() != _scanTimes.getSize() + dummyScanCount )
    {

      throw std::runtime_error( "inconsistent time count" );

    }
    if ( ( measures.getSizeX() != kalmanLut.getSizeX() ) ||
         ( measures.getSizeY() != kalmanLut.getSizeY() ) ||
         ( measures.getSizeZ() != kalmanLut.getSizeZ() ) )
    {

      throw std::runtime_error( "inconsistent X/Y/Z size" );

    }
         

    // processing effective scan count and volume size(s)
    int32_t effectiveScanCount = measures.getSizeT() - dummyScanCount;

    // looping over times
    int32_t t;
    for ( t = 0; t < effectiveScanCount; t++ )
    {

      if ( verbose )
      {

        if ( t )
        {

          std::cout << gkg::Eraser( 22 );

        }
        std::cout << " [ " << std::setw( 7 ) << t + 1 
                  << " / " << std::setw( 7 ) << effectiveScanCount
                  << " ]" << std::flush;

      }

      _kalmanFilter.setSubDesignMatrix( t );

      // processing volume size(s)
      gkg::BoldGeneralLinearModelSolveContext< T > context( _kalmanFilter,
                                                            measures,
                                                            kalmanLut,
                                                            sites,
                                                            t,
                                                            dummyScanCount );
      gkg::ThreadedLoop< int32_t > threadedLoop( &context, 0, sites.size() );

      threadedLoop.launch();

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 22 );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BoldGeneralLinearModelSolver::solve( "
             "const gkg::Volume< T >& measures, "
             "int32_t dummyScanCount, "
             "const std::vector< int32_t >& sliceOrder, "
             "gkg::Volume< gkg::Kalman::Parameters* >&  kalmanLut, "
             "const std::vector< gkg::Vector3d< int32_t > >& sites, "
             "bool verbose ) const" );

}


template < class T >
void gkg::BoldGeneralLinearModelSolver::iterate(
                      const gkg::Volume< T >& measures,
                      gkg::Volume< gkg::Kalman::Parameters* >& kalmanLut,
                      const std::vector< gkg::Vector3d< int32_t > >& sites,
                      int32_t t )
{

  try
  {

    _kalmanFilter.setSubDesignMatrix( t );
  
    gkg::BoldGeneralLinearModelContext< T > context( _kalmanFilter,
                                                     measures,
                                                     kalmanLut,
                                                     sites,
                                                     t );
    gkg::ThreadedLoop< int32_t > threadedLoop( &context, 0, sites.size() );

    threadedLoop.launch();
    
  }
  GKG_CATCH( "template < class T >"
             "void gkg::BoldGeneralLinearModelSolver::iterate( "
             "const gkg::Volume< T >& measures, "
             "gkg::Volume< gkg::Kalman::Parameters* >& kalmanLut, "
             "const std::vector< gkg::Vector3d< int32_t > >& sites, "
             "int32_t t )" );

}


#endif
