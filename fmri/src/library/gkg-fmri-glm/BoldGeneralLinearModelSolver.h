#ifndef _gkg_fmri_glm_BoldGeneralLinearModelSolver_h_
#define _gkg_fmri_glm_BoldGeneralLinearModelSolver_h_

#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/KalmanFilter.h>

#include <vector>


namespace gkg
{


class BoldGeneralLinearModelSolver
{

  public:

    BoldGeneralLinearModelSolver( const Matrix& designMatrix,
                                  const Vector& scanTimes );
    virtual ~BoldGeneralLinearModelSolver();

    const Matrix& getDesignMatrix() const;

    template < class T >
    void solve( const Volume< T >& measures,
                int32_t dummyScanCount,
                const std::vector< int32_t >& sliceOrder,
                Volume< Kalman::Parameters* >& kalmanLut,
                const std::vector< Vector3d< int32_t > >& sites,
                bool verbose = false );

    template < class T >
    void iterate( const Volume< T >& measures,
                  Volume< Kalman::Parameters* >& kalmanLut,
                  const std::vector< Vector3d< int32_t > >& sites,
                  int32_t t );

  protected:

    Vector _scanTimes;
    KalmanFilter _kalmanFilter;

};


}


#endif
