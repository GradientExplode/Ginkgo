#ifndef _gkg_fmri_statistics_PValueFunction_h_
#define _gkg_fmri_statistics_PValueFunction_h_


#include <gkg-fmri-statistics/GLMStatisticalFunction.h>
#include <gkg-fmri-statistics/ZScoreFunction.h>


namespace gkg
{


class PValueFunction : public GLMStatisticalFunction
{

  public:

    PValueFunction( int32_t effectiveScanCount,
                    int32_t contrastCount );
    ~PValueFunction();

    void process( const Vector& effects,
                  const Matrix& effectVariances,
                  Vector& statistics );

  private:

    ZScoreFunction _zScoreFunction;

};


}


#endif
