#ifndef _gkg_processing_container_CartesianField_h_
#define _gkg_processing_container_CartesianField_h_


#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/SiteMap.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-processing-container/RankSiteLut.h>


namespace gkg
{


template < class T >
class CartesianField
{

  public:

    typedef typename TextureMap< T >::iterator iterator;
    typedef typename TextureMap< T >::const_iterator const_iterator;
    typedef typename TextureMap< T >::reverse_iterator reverse_iterator;
    typedef typename TextureMap< T >::const_reverse_iterator
                                                         const_reverse_iterator;

    CartesianField( int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                    const Vector3d< double >& resolution,
                    const SiteMap< int32_t, int32_t >& siteMap,
                    int32_t offsetX = 0,
                    int32_t offsetY = 0,
                    int32_t offsetZ = 0 );
    CartesianField( int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                    const Vector3d< double >& resolution,
                    const SiteMap< int32_t, int32_t >& siteMap,
                    const TextureMap< T >& items,
                    int32_t offsetX = 0,
                    int32_t offsetY = 0,
                    int32_t offsetZ = 0 );
    CartesianField( const SiteMap< int32_t, int32_t >& siteMap,
                    const TextureMap< T >& items );
    CartesianField( int32_t offsetX = 0,
                    int32_t offsetY = 0,
                    int32_t offsetZ = 0 );
    virtual ~CartesianField();

    void allocate( int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                   const Vector3d< double >& resolution,
                   int32_t offsetX = 0,
                   int32_t offsetY = 0,
                   int32_t offsetZ = 0 );
    void setSiteMap( const SiteMap< int32_t, int32_t >& siteMap );
    void setField( const SiteMap< int32_t, int32_t >& siteMap,
                   const TextureMap< T >& items );

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    reverse_iterator rbegin();
    reverse_iterator rend();

    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    int32_t getSizeX() const;
    int32_t getSizeY() const;
    int32_t getSizeZ() const;
    Vector3d< int32_t > getSize() const;

    int32_t getOffsetX() const;
    int32_t getOffsetY() const;
    int32_t getOffsetZ() const;
    Vector3d< int32_t > getOffset() const;

    double getResolutionX() const;
    double getResolutionY() const;
    double getResolutionZ() const;
    Vector3d< double > getResolution() const;

    void setItem( const Vector3d< int32_t >& site, const T& thing );
    void setItemFast( const Vector3d< int32_t >& site, const T& thing );
    const T* getItem( int32_t x, int32_t y = 0, int32_t z = 0 ) const;
    const T* getItem( const Vector2d< int32_t >& site, int32_t z = 0 ) const;
    const T* getItem( const Vector3d< int32_t >& site ) const;
    const T* getItemFast( int32_t x, int32_t y = 0, int32_t z = 0 ) const;
    const T* getItemFast( const Vector3d< int32_t >& site ) const;

    T* getItem( int32_t x, int32_t y = 0, int32_t z = 0 );
    T* getItem( const Vector2d< int32_t >& site, int32_t z = 0 );
    T* getItem( const Vector3d< int32_t >& site );
    T* getItemFast( int32_t x, int32_t y = 0, int32_t z = 0 );
    T* getItemFast( const Vector3d< int32_t >& site );

    SiteMap< int32_t, int32_t >& getSiteMap();
    TextureMap< T >& getItems();

  protected:

    void allocateLut( int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                      const Vector3d< double >& resolution );

    SiteMap< int32_t, int32_t > _siteMap;
    TextureMap< T > _items;
    Volume< T* > _lut;

    RankSiteLut< int32_t, int32_t > _rankSiteLut;

    int32_t _offsetX;
    int32_t _offsetY;
    int32_t _offsetZ;

};


}


#endif

