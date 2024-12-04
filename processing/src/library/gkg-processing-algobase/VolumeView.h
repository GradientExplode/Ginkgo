#ifndef _gkg_processing_algobase_VolumeView_h_
#define _gkg_processing_algobase_VolumeView_h_


#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


template < class T >
class VolumeView
{

  public:

    VolumeView( const Volume< T >& volume );
    virtual ~VolumeView();

    void extract( const BoundingBox< int32_t >& boundingBox, Volume< T >& out ) const;

  private:

    const Volume< T >& _volume;

};


}


#endif
