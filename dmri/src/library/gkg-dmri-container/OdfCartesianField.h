#ifndef _gkg_dmri_container_OdfCartesianField_h_
#define _gkg_dmri_container_OdfCartesianField_h_


#include <gkg-dmri-container/DiffusionCartesianField.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>


namespace gkg
{


class OdfCartesianField : public DiffusionCartesianField<
                                               OrientationDistributionFunction >
{

  public:

    OdfCartesianField( int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                       const Vector3d< double >& resolution,
                       const SiteMap< int32_t, int32_t >& siteMap,
                       int32_t offsetX,
                       int32_t offsetY,
                       int32_t offsetZ,
                       const OrientationSet& outputOrientationSet );
    OdfCartesianField( int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                       const Vector3d< double >& resolution,
                       const SiteMap< int32_t, int32_t >& siteMap,
                       const TextureMap< OrientationDistributionFunction >&
                                                                          items,
                       int32_t offsetX,
                       int32_t offsetY,
                       int32_t offsetZ,
                       const OrientationSet& outputOrientationSet );
    OdfCartesianField( const SiteMap< int32_t, int32_t >& siteMap,
                       const TextureMap< OrientationDistributionFunction >&
                                                                          items,
                       const OrientationSet& outputOrientationSet );
     virtual ~OdfCartesianField();

    const OrientationSet& getOutputOrientationSet() const;

    OrientationDistributionFunction::BasisType getBasisType() const;
    void setStandardBasis( bool verbose = false );

    void setOutputOrientationSet( const OrientationSet* outputOrientationSet );

  protected:

    OdfCartesianField( RCPointer< Volume< float > > t2,
                       RCPointer< Volume< float > > dw,
                       RCPointer< Volume< int16_t > > mask,
                       const OrientationSet* outputOrientationSet = 0 );
    OdfCartesianField( RCPointer< Volume< float > > t2,
                       RCPointer< Volume< float > > dw,
                       RCPointer< Volume< int16_t > > mask,
                       const BoundingBox< float >& boundingBox,
                       const Vector3d< float >& superResolution,
                       const OrientationSet* outputOrientationSet = 0 );
    OdfCartesianField( RCPointer< Volume< float > > t2,
                       RCPointer< Volume< float > > dw,
                       RCPointer< Volume< int16_t > > mask,
                       const BoundingBox< int32_t >& boundingBox,
                       const OrientationSet* outputOrientationSet = 0 );

    OrientationSet _outputOrientationSet;

};


}


#endif
