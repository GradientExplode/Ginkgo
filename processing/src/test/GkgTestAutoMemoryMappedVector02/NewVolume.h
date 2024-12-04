#ifndef _gkg_processing_container_NewVolume_h_
#define _gkg_processing_container_NewVolume_h_


#include <gkg-processing-container/VolumeProxy.h>
#include "NewAutoMemoryMappedVector.h"
#include <gkg-processing-coordinates/Vector2d.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <vector>


namespace gkg
{


template < class T >
class NewVolume : public VolumeProxy< T >
{

  public:

    typedef typename NewAutoMemoryMappedVector< T >::iterator iterator;
    typedef typename NewAutoMemoryMappedVector< T >::const_iterator const_iterator;

    NewVolume( int32_t sizeX = 1, int32_t sizeY = 1, 
            int32_t sizeZ = 1, int32_t sizeT = 1 );
    NewVolume( const Vector3d< int32_t >& size,
            int32_t sizeT = 1 );
    NewVolume( const Vector2d< int32_t >& size,
            int32_t sizeZ = 1, int32_t sizeT = 1 );
    NewVolume( const NewVolume< T >& other );
    virtual ~NewVolume();

    NewVolume< T >& operator=( const NewVolume< T >& other );

    void fill( const T& value, bool verbose = false );

    void allocate();
    void reallocate( int32_t sizeX = 1, int32_t sizeY = 1,
                     int32_t sizeZ = 1, int32_t sizeT = 1 );
    void reallocate( const Vector3d< int32_t >& size,
                     int32_t sizeT = 1 );
    void reallocate( const Vector2d< int32_t >& size,
                     int32_t sizeZ = 1, int32_t sizeT = 1 );

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
    void setAutoMemoryVerbose();

  protected:

    void updateLineAccessors();

    NewAutoMemoryMappedVector< T > _items;
    std::vector< std::vector< std::vector< T* > > >  _lineAccessors;

};


}


#endif
