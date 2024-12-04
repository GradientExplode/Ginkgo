#ifndef _gkg_fmri_contrast_ContrastEffectsContext_h_
#define _gkg_fmri_contrast_ContrastEffectsContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/Kalman.h>
#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{


class ContrastEffectsContext : public LoopContext< int32_t >
{

  public:

    ContrastEffectsContext( const Volume< Kalman::Parameters* >& kalmanLut,
                            const std::vector< Vector3d< int32_t > >& sites,
                            const Matrix& C,
                            Volume< float >& volumeOfEffects,
                            Volume< float >& volumeOfEffectVariances );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const Volume< Kalman::Parameters* >& _kalmanLut;
    const std::vector< Vector3d< int32_t > >& _sites;
    const Matrix& _C;
    Volume< float >& _volumeOfEffects;
    Volume< float >& _volumeOfEffectVariances;
    Matrix _Ct;
    int32_t _contrastCount;

};


}


#endif
