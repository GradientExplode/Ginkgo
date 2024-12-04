#ifndef _gkg_processing_resampling_SplineResampler_h_
#define _gkg_processing_resampling_SplineResampler_h_


#include <gkg-processing-resampling/Resampler.h>
#include <gkg-processing-container/Volume.h>
#include <vector>


namespace gkg
{


template < class T >
struct SplineResamplerTraits
{

  typedef double coefficient_type;

  static double norm2( const coefficient_type& value );

};


template <>
struct SplineResamplerTraits< std::complex< float > >
{

  typedef std::complex< double > coefficient_type;

  static double norm2( const coefficient_type& value );

};


template <>
struct SplineResamplerTraits< std::complex< double > >
{

  typedef std::complex< double > coefficient_type;

  static double norm2( const coefficient_type& value );

};


template <>
struct SplineResamplerTraits< gkg::RGBComponent >
{

  typedef gkg::RGBComponent coefficient_type;

  static double norm2( const gkg::RGBComponent& value );

};


template < class T >
class SplineResampler : public Resampler< T >
{

  public:

    virtual ~SplineResampler();

    virtual void resample( const Volume< T >& inVolume,
                           const Transform3d< float >& transform3d,
                           const T& outBackground,
                           Volume< T >& outVolume,
                           bool verbose = false );
    virtual void resample( const Volume< T >& inVolume,
                           const Transform3d< double >& transform3d,
                           const T& outBackground,
                           Volume< T >& outVolume,
                           bool verbose = false );

    virtual void resample( const Volume< T >& inVolume,
                           const Transform3d< double >& transform3d,
                           const T& outBackground,
                           const Vector3d< double >& outLocation,
                           T& outValue );
    virtual void resample( const Volume< T >& inVolume,
                           const Transform3d< float >& transform3d,
                           const T& outBackground,
                           const Vector3d< float >& outLocation,
                           T& outValue );

    // be carreful; spline resampler need to pre-process spline coefficients;
    // in order to be more efficient, they are calculated with a recursive
    // scheme from input reference volume; this recursive algorithm is much 
    // faster when you have to resample a whole volume, but it is less faster
    // when you only need to resample one point; so a cach mechanism has been
    // implemented in order to process those coefficients only once,
    // provided the input inVolume has not changed, or a call to reset() 
    // function has not been sent.
    // This reset() function force recalculation of the spline coefficient set.
    void reset();

  protected:

    SplineResampler();

    virtual void doResample( const Volume< T >& /*inVolume*/,
                             const Transform3d< float >& transform3d,
                             const T& outBackground,
                             const Vector3d< float >& outLocation,
                             T& outValue );
    virtual void doResample( const Volume< T >& /*inVolume*/,
                             const Transform3d< double >& transform3d,
                             const T& outBackground,
                             const Vector3d< double >& outLocation,
                             T& outValue );
    void updateSplineCoefficients( const Volume< T >& inVolume,
                                   bool verbose );
    void iirConvolveMirror( std::vector< typename 
                                 SplineResamplerTraits< T >::coefficient_type >&
                                                                   data ) const;
    int32_t getFold( int32_t i, int32_t size ) const;
    virtual double getBSplineWeight( int32_t i, double x ) const = 0;

    T convert( typename SplineResamplerTraits< T >::coefficient_type&
               intensity ) const;

    bool _reset;
    std::vector< double > _poles;
    double _gain;
    Volume< typename SplineResamplerTraits< T >::coefficient_type >
      _splineCoefficients;

};


}


#endif
