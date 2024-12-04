#ifndef _gkg_processing_container_ContinuousField_h_
#define _gkg_processing_container_ContinuousField_h_


#include <gkg-processing-container/SiteMap.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-processing-container/RankSiteLut.h>
#include <gkg-processing-coordinates/Vector2d.h>
#include <gkg-processing-coordinates/Vector3d.h>


namespace gkg
{


template < class T >
class ContinuousField
{

  public:

    typedef typename TextureMap< T* >::iterator iterator;
    typedef typename TextureMap< T* >::const_iterator const_iterator;
    typedef typename TextureMap< T* >::reverse_iterator reverse_iterator;
    typedef typename TextureMap< T* >::const_reverse_iterator
                                                         const_reverse_iterator;

    virtual ~ContinuousField();

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    reverse_iterator rbegin();
    reverse_iterator rend();

    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    bool areItemsStored() const;

    T* getItem( float x, float y = 0, float z = 0 );
    T* getItem( const Vector2d< float >& site, float z = 0 );
    T* getItem( const Vector3d< float >& site );

    SiteMap< int32_t, float >& getSiteMap();
    TextureMap< T* >& getItems();

  protected:

    ContinuousField( bool storeItems );

    virtual T* newItem( const Vector3d< float >& site ) = 0;

    bool _storeItems;
    SiteMap< int32_t, float > _siteMap;
    TextureMap< T* > _items;

    RankSiteLut< int32_t, float > _rankSiteLut;

};


}


#endif

