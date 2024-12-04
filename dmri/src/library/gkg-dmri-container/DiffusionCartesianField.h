#ifndef _gkg_dmri_container_DiffusionCartesianField_h_
#define _gkg_dmri_container_DiffusionCartesianField_h_


#include <gkg-dmri-container/DiffusionCartesianFieldLoopContext.h>
#include <gkg-processing-container/CartesianField.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <vector>


namespace gkg
{


template < class T > class DiffusionCartesianFieldFunctor;


template < class T >
class DiffusionCartesianField : public CartesianField< T >
{

  public:

    DiffusionCartesianField( int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                             const Vector3d< double >& resolution,
                             const SiteMap< int32_t, int32_t >& siteMap,
                             int32_t offsetX = 0,
                             int32_t offsetY = 0,
                             int32_t offsetZ = 0 );
    DiffusionCartesianField( int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                             const Vector3d< double >& resolution,
                             const SiteMap< int32_t, int32_t >& siteMap,
                             const TextureMap< T >& items,
                             int32_t offsetX = 0,
                             int32_t offsetY = 0,
                             int32_t offsetZ = 0 );
    DiffusionCartesianField( const SiteMap< int32_t, int32_t >& siteMap,
                             const TextureMap< T >& items );
    virtual ~DiffusionCartesianField();

    const Volume< float >& getT2Volume() const;
    const Volume< float >& getDWVolume() const;
    const Volume< int16_t >& getMaskVolume() const;

    const RCPointer< Volume< float > >& getRCPointerOfT2Volume() const;
    const RCPointer< Volume< float > >& getRCPointerOfDWVolume() const;
    const RCPointer< Volume< int16_t > >& getRCPointerOfMaskVolume() const;

    const OrientationSet& getInputOrientationSet() const;
    const std::vector< float >& getBValues() const;

    void setFunctor( DiffusionCartesianFieldFunctor< T >* functor );
    DiffusionCartesianFieldFunctor< T >* getFunctor() const;

  protected:

    friend class DiffusionCartesianFieldLoopContext< T >;

    DiffusionCartesianField( RCPointer< Volume< float > > t2,
                             RCPointer< Volume< float > > dw,
                             RCPointer< Volume< int16_t > > mask );
    DiffusionCartesianField( RCPointer< Volume< float > > t2,
                             RCPointer< Volume< float > > dw,
                             RCPointer< Volume< int16_t > > mask,
                             const BoundingBox< float >& boundingBox,
                             const Vector3d< float >& superResolution );
    DiffusionCartesianField( RCPointer< Volume< float > > t2,
                             RCPointer< Volume< float > > dw,
                             RCPointer< Volume< int16_t > > mask,
                             const BoundingBox< int32_t >& boundingBox );

    void doT2DwMaskSanityChecks();
    void setInputOrientationSet();

    void calculatingOnGrid( const BoundingBox< int32_t >& boundingBox,
                            bool verbose );
    void calculatingOnSubGrid( const BoundingBox< float >& boundingBox,
                               const Vector3d< float >& superResolution,
                               bool verbose );
    float getMinimumAngleOfInputOrientationSet() const;

    virtual T newItem( const Vector3d< int32_t >& originSite,
                       const Vector3d< int32_t >& lutSite );
    virtual T newItem( const Vector3d< float >& originSite,
                       const Vector3d< int32_t >& lutSite );

    RCPointer< Volume< float > > _t2;
    RCPointer< Volume< float > > _dw;
    RCPointer< Volume< int16_t > > _mask;

    DiffusionCartesianFieldFunctor< T >* _functor;

    OrientationSet _inputOrientationSet;
    std::vector< float > _bValues;

};


}



#endif


