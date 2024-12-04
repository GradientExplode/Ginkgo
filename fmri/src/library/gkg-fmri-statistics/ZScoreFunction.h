#ifndef _gkg_fmri_statistics_ZScoreFunction_h_
#define _gkg_fmri_statistics_ZScoreFunction_h_


#include <gkg-fmri-statistics/GLMStatisticalFunction.h>


namespace gkg
{


class ZScoreFunction : public GLMStatisticalFunction
{

  public:

    ZScoreFunction( int32_t effectiveScanCount,
                    int32_t contrastCount );
    ~ZScoreFunction();

    void process( const Vector& effects,
                  const Matrix& effectVariances,
                  Vector& statistics );

};


}


#endif
