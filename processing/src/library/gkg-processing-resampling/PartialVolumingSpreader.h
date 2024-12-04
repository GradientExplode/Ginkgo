#ifndef _gkg_processing_resampling_PartialVolumingSpreader_h_
#define _gkg_processing_resampling_PartialVolumingSpreader_h_


#include <gkg-processing-resampling/Spreader.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T > class Volume;
template < class T > class Vector3d;


template < class T >
class PartialVolumingSpreader : public Spreader< T >,
                                public Singleton< PartialVolumingSpreader< T > >
{

  public:

    ~PartialVolumingSpreader();

    std::string getName() const;

    void spread( const T& value,
                 const Vector3d< float >& outLocation,
                 Volume< T >& outVolume,
                 const Vector3d< double >* resolution = 0,
                 int32_t rank = 0 ) const;

    void spread( const T& value,
                 const Vector3d< double >& outLocation,
                 Volume< T >& outVolume,
                 const Vector3d< double >* resolution = 0,
                 int32_t rank = 0 ) const;

  private:

    friend class Singleton< PartialVolumingSpreader< T > >;

    PartialVolumingSpreader();

};


}


#endif
