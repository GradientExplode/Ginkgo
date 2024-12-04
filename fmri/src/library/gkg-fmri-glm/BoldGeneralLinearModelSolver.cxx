#include <gkg-fmri-glm/BoldGeneralLinearModelSolver_i.h>
#include <gkg-core-exception/Exception.h>


// attention, scanTimes doit être du type:
//   scanTime[ i ] = startingTime + ( i + dummyScanCount ) * TR
//   avec i : 0 -> effectiveScanCount - 1
// ou alors
//   scanTime[ i ] = inputScanTime[ i + dummyScanCount ]
//   avec i : 0 -> effectiveScanCount - 1
// fait l'hypothese que scanTimes sont les temps des acquisitions
//  des volumes, en faisant l'hyothese que les dummy scans n'y sont pas.

gkg::BoldGeneralLinearModelSolver::BoldGeneralLinearModelSolver(
                                    const gkg::Matrix& designMatrix,
                                    const gkg::Vector& scanTimes )
                                  : _scanTimes( scanTimes ),
                                    _kalmanFilter( designMatrix )
{
}


gkg::BoldGeneralLinearModelSolver::~BoldGeneralLinearModelSolver()
{
}


const gkg::Matrix& gkg::BoldGeneralLinearModelSolver::getDesignMatrix() const
{

  try
  {

    return _kalmanFilter.getDesignMatrix();

  }
  GKG_CATCH( "const gkg::Matrix& "
             "gkg::BoldGeneralLinearModelSolver::getDesignMatrix() const" );

}


//
// template instantiation
//

template void gkg::BoldGeneralLinearModelSolver::solve< int8_t >( 
                           const gkg::Volume< int8_t >& measures,
                           int32_t dummyScanCount,
                           const std::vector< int32_t >& sliceOrder,
                           gkg::Volume< Kalman::Parameters* >& kalmanLut,
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           bool verbose = false );
template void gkg::BoldGeneralLinearModelSolver::solve< uint8_t >( 
                           const gkg::Volume< uint8_t >& measures,
                           int32_t dummyScanCount,
                           const std::vector< int32_t >& sliceOrder,
                           gkg::Volume< Kalman::Parameters* >& kalmanLut,
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           bool verbose = false );
template void gkg::BoldGeneralLinearModelSolver::solve< int16_t >( 
                           const gkg::Volume< int16_t >& measures,
                           int32_t dummyScanCount,
                           const std::vector< int32_t >& sliceOrder,
                           gkg::Volume< Kalman::Parameters* >& kalmanLut,
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           bool verbose = false );
template void gkg::BoldGeneralLinearModelSolver::solve< uint16_t >( 
                           const gkg::Volume< uint16_t >& measures,
                           int32_t dummyScanCount,
                           const std::vector< int32_t >& sliceOrder,
                           gkg::Volume< Kalman::Parameters* >& kalmanLut,
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           bool verbose = false );
template void gkg::BoldGeneralLinearModelSolver::solve< int32_t >( 
                           const gkg::Volume< int32_t >& measures,
                           int32_t dummyScanCount,
                           const std::vector< int32_t >& sliceOrder,
                           gkg::Volume< Kalman::Parameters* >& kalmanLut,
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           bool verbose = false );
template void gkg::BoldGeneralLinearModelSolver::solve< uint32_t >( 
                           const gkg::Volume< uint32_t >& measures,
                           int32_t dummyScanCount,
                           const std::vector< int32_t >& sliceOrder,
                           gkg::Volume< Kalman::Parameters* >& kalmanLut,
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           bool verbose = false );
template void gkg::BoldGeneralLinearModelSolver::solve< int64_t >( 
                           const gkg::Volume< int64_t >& measures,
                           int32_t dummyScanCount,
                           const std::vector< int32_t >& sliceOrder,
                           gkg::Volume< Kalman::Parameters* >& kalmanLut,
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           bool verbose = false );
template void gkg::BoldGeneralLinearModelSolver::solve< uint64_t >( 
                           const gkg::Volume< uint64_t >& measures,
                           int32_t dummyScanCount,
                           const std::vector< int32_t >& sliceOrder,
                           gkg::Volume< Kalman::Parameters* >& kalmanLut,
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           bool verbose = false );
template void gkg::BoldGeneralLinearModelSolver::solve< float >( 
                           const gkg::Volume< float >& measures,
                           int32_t dummyScanCount,
                           const std::vector< int32_t >& sliceOrder,
                           gkg::Volume< Kalman::Parameters* >& kalmanLut,
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           bool verbose = false );
template void gkg::BoldGeneralLinearModelSolver::solve< double >( 
                           const gkg::Volume< double >& measures,
                           int32_t dummyScanCount,
                           const std::vector< int32_t >& sliceOrder,
                           gkg::Volume< Kalman::Parameters* >& kalmanLut,
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           bool verbose = false );
