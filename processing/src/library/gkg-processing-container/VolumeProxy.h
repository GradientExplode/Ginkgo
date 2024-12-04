#ifndef _gkg_processing_container_VolumeProxy_h_
#define _gkg_processing_container_VolumeProxy_h_


#include <gkg-core-object/HeaderedObject.h>


namespace gkg
{


template < class T > class Vector2d;
template < class T > class Vector3d;


template < class T >
class VolumeProxy : public HeaderedObject
{

  public:

    VolumeProxy( int32_t sizeX = 1, int32_t sizeY = 1, int32_t sizeZ = 1, 
                 int32_t sizeT = 1 );
    VolumeProxy( const Vector3d< int32_t >& size,
                 int32_t sizeT = 1 );
    VolumeProxy( const Vector2d< int32_t >& size,
                 int32_t sizeZ = 1, int32_t sizeT = 1 );
    VolumeProxy( const VolumeProxy< T >& other );
    virtual ~VolumeProxy();

    int32_t getSizeX() const;
    int32_t getSizeY() const;
    int32_t getSizeZ() const;
    int32_t getSizeT() const;

    int64_t getSizeXY() const;
    int64_t getSizeXYZ() const;
    int64_t getSizeXYZT() const;

    template < class U > 
      bool hasSameSize( const VolumeProxy< U >& other,
                        bool ignoreSizeT = false );
    template < class U > 
      bool hasSameResolution( const VolumeProxy< U >& other,
                              bool ignoreResolutionT = false,
                              double epsilon = 1e-3 );

    void getSize( Vector3d< int32_t >& size ) const;
    void getSize( Vector3d< int32_t >& size, int32_t& sizeT ) const;
    void getResolution( Vector3d< double >& resolution ) const;
    void getResolution( Vector3d< double >& resolution,
                        double& resolutionT ) const;

    void setResolution( const Vector3d< double >& resolution );
    void setResolution( const Vector3d< double >& resolution,
                        double resolutionT );

    void getFieldOfView( Vector3d< float >& fov ) const;
    void getFieldOfView( Vector3d< double >& fov ) const;
    void getFieldOfViewCenter( Vector3d< float >& fovCenter ) const;
    void getFieldOfViewCenter( Vector3d< double >& fovCenter ) const;

    VolumeProxy< T >& operator=( const VolumeProxy< T >& other );

    virtual void update( const Observable* observable = 0, void* arg = 0 );

  protected:

    void prepareHeader();

    int32_t _sizeX;
    int32_t _sizeY;
    int32_t _sizeZ;
    int32_t _sizeT;
    int64_t _sizeXY;
    int64_t _sizeXYZ;
    int64_t _sizeXYZT;

};


}


#endif
