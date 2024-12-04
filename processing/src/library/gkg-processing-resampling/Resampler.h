#ifndef _gkg_processing_resampling_Resampler_h_
#define _gkg_processing_resampling_Resampler_h_


// Comment or uncomment following line to allow multithreading for resampling
#define RESAMPLER_USE_THREADED_LOOP

#include <gkg-core-cppext/StdInt.h>
#ifdef RESAMPLER_USE_THREADED_LOOP
#include <gkg-communication-thread/LoopContext.h>
#endif
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <string>
#include <complex>




namespace gkg
{


template < class T > class Volume;
template < class T > class Transform3d;
template < class T > class Vector3d;



template < class T >
class Resampler
{

  public:

    virtual ~Resampler();

    virtual std::string getName() const = 0;
    virtual int32_t getOrder() const = 0;

    virtual void resample( const Volume< T >& inVolume,
                           const Transform3d< float >& transform3d,
                           const T& outBackground,
                           Volume< T >& outVolume,
                           bool verbose = false );

#ifdef RESAMPLER_USE_THREADED_LOOP
    virtual void resample( const Volume< T >& inVolume,
                           const Transform3d< float >& transform3d,
                           const T& outBackground,
                           Volume< T >& outVolume,
                           Vector3d< double >& outResolution,
                           int32_t zStartIndex,
                           int32_t zCount,
                           bool verbose = false );
#endif

    virtual void resample( const Volume< T >& inVolume,
                           const Transform3d< double >& transform3d,
                           const T& outBackground,
                           Volume< T >& outVolume,
                           bool verbose = false );

#ifdef RESAMPLER_USE_THREADED_LOOP
    virtual void resample( const Volume< T >& inVolume,
                           const Transform3d< double >& transform3d,
                           const T& outBackground,
                           Volume< T >& outVolume,
                           Vector3d< double >& outResolution,
                           int32_t zStartIndex,
                           int32_t zCount,
                           bool verbose = false );
#endif

    virtual void resample( const Volume< T >& inVolume,
                           const Transform3d< float >& transform3d,
                           const T& outBackground,
                           const Vector3d< float >& outLocation,
                           T& outValue,
                           const Vector3d< double >* inResolution = 0 );
    virtual void resample( const Volume< T >& inVolume,
                           const Transform3d< double >& transform3d,
                           const T& outBackground,
                           const Vector3d< double >& outLocation,
                           T& outValue,
                           const Vector3d< double >* inResolution = 0 );

  protected:

    Resampler();

    virtual void doResample( const Volume< T >& inVolume,
                             const Transform3d< float >& transform3d,
                             const T& outBackground,
                             const Vector3d< float >& outLocation,
                             T& outValue ) = 0;
    virtual void doResample( const Volume< T >& inVolume,
                             const Transform3d< double >& transform3d,
                             const T& outBackground,
                             const Vector3d< double >& outLocation,
                             T& outValue ) = 0;

};


#ifdef RESAMPLER_USE_THREADED_LOOP

template < class T1, class T2 >
class ResamplerContext : public LoopContext< int32_t >
{

  public:

    ResamplerContext( Resampler< T1 >* resampler,
                      const Volume< T1 >& inVolume,
                      const Transform3d< T2 >& transform3d,
                      const T1& outBackground,
                      Volume< T1 >& outVolume,
                      Vector3d< double >& outResolution,
                      bool verbose );

    void doIt( int32_t startIndex, int32_t count );

  private:

    Resampler< T1 >* _resampler;
    const Volume< T1 >& _inVolume;
    const Transform3d< T2 >& _transform3d;
    const T1& _outBackground;
    Volume< T1 >& _outVolume;
    Vector3d< double >& _outResolution;
    bool _verbose;

};

#endif



}


#endif
