#ifndef _gkg_fmri_statistics_TScoreFunction_h_
#define _gkg_fmri_statistics_TScoreFunction_h_


#include <gkg-fmri-statistics/GLMStatisticalFunction.h>


namespace gkg
{


class TScoreFunction : public GLMStatisticalFunction
{

  public:

    TScoreFunction( int32_t effectiveScanCount,
                    int32_t contrastCount );
    ~TScoreFunction();

    void process( const Vector& effects,
                  const Matrix& effectVariances,
                  Vector& statistics );

};


}


#endif
