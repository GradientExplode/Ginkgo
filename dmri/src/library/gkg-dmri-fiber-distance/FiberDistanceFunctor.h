#ifndef _gkg_dmri_fiber_distance_FiberDistanceFunctor_h_
#define _gkg_dmri_fiber_distance_FiberDistanceFunctor_h_


#include <gkg-processing-container/LightCurve3d.h>
#include <gkg-processing-numericalanalysis/DistanceFunctor.h>


namespace gkg
{


class FiberDistanceFunctor : public DistanceFunctor
{

  public:

    virtual ~FiberDistanceFunctor();

    virtual float getDistance( const LightCurve3d< float >& fiber1,
                               const LightCurve3d< float >& fiber2,
                               const float* fiberLength1 = 0,
                               const float* fiberLength2 = 0,
                               float* nonCorrectedDistancePointer = 0,
                               float* distanceCorrectionPointer = 0 ) const = 0;

  protected:

    FiberDistanceFunctor();


    virtual double fastComputeDistanceSquare( const double* s1,
                                              const double* s2,
                                              int32_t count ) const = 0;
    virtual float fastComputeDistanceSquare( const float* s1,
                                             const float* s2,
                                             int32_t count ) const = 0;

    virtual float getFiberLength( const float* s1, int32_t count ) const = 0;
    virtual double getFiberLength( const double* s1, int32_t count ) const = 0;

};


}


#endif

