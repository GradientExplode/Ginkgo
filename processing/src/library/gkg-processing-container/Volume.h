#ifndef _gkg_processing_container_Volume_h_
#define _gkg_processing_container_Volume_h_


#include <gkg-processing-container/VolumeProxy.h>
#include <gkg-processing-container/AutoMemoryMappedVector.h>
#include <gkg-processing-coordinates/Vector2d.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <vector>


namespace gkg
{


template < class T >
class Volume : public VolumeProxy< T >
{

  public:

    typedef typename AutoMemoryMappedVector< T >::iterator iterator;
    typedef typename AutoMemoryMappedVector< T >::const_iterator const_iterator;

    Volume( int32_t sizeX = 1, int32_t sizeY = 1, 
            int32_t sizeZ = 1, int32_t sizeT = 1 );
    Volume( const Vector3d< int32_t >& size,
            int32_t sizeT = 1 );
    Volume( const Vector2d< int32_t >& size,
            int32_t sizeZ = 1, int32_t sizeT = 1 );
    Volume( const Volume< T >& other );
    virtual ~Volume();

    Volume< T >& operator=( const Volume< T >& other );

    void fill( const T& value );

    void allocate();
    void reallocate( int32_t sizeX = 1, int32_t sizeY = 1,
                     int32_t sizeZ = 1, int32_t sizeT = 1,
                     bool copyData = true );
    void reallocate( const Vector3d< int32_t >& size,
                     int32_t sizeT = 1,
                     bool copyData = true );
    void reallocate( const Vector2d< int32_t >& size,
                     int32_t sizeZ = 1, int32_t sizeT = 1,
                     bool copyData = true );

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    const T& operator()( int32_t x, int32_t y = 0, 
                         int32_t z = 0, int32_t t = 0 ) const;
    T& operator() ( int32_t x, int32_t y = 0, int32_t z = 0, int32_t t = 0 );

    const T& operator()( const Vector2d< int32_t >& site,
                         int32_t z = 0, int32_t t = 0 ) const;
    T& operator() ( const Vector2d< int32_t >& site,
                    int32_t z = 0, int32_t t = 0 );

    const T& operator()( const Vector3d< int32_t >& site,
                         int32_t t = 0 ) const;
    T& operator() ( const Vector3d< int32_t >& site, int32_t t = 0 );

    bool isMemoryMapped() const;

  protected:

    void updateLineAccessors();

    AutoMemoryMappedVector< T > _items;
    std::vector< std::vector< std::vector< T* > > >  _lineAccessors;

};


}


#endif
