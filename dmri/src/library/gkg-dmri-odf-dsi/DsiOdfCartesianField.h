#ifndef _gkg_dmri_odf_dsi_DsiOdfCartesianField_h_
#define _gkg_dmri_odf_dsi_DsiOdfCartesianField_h_


#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-processing-container/Volume_i.h>


namespace gkg
{

class DsiOdfCartesianField : public OdfCartesianField
{

  public:

    DsiOdfCartesianField(
		     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
		     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    DsiOdfCartesianField(
                     RCPointer< Volume< float > > t2,
		     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< float >& boundingBox,
                     const Vector3d< float >& superResolution,
		     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    DsiOdfCartesianField(
		     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< int32_t >& boundingBox,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    virtual ~DsiOdfCartesianField();

    bool getFilterOrNot() const;
    bool getMarginalODF() const;
    int32_t getGridRadius() const;
    float getMinimumR0() const;
    float getMaximumR0() const;
    
    static void checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters );

  protected:

    void prepareForDSI( bool verbose );

    OrientationDistributionFunction
      newItem( const Vector3d< int32_t >& originSite,
               const Vector3d< int32_t >& lutSite );

    OrientationDistributionFunction
      newItem( const Vector3d< float >& originSite,
               const Vector3d< int32_t >& lutSite );

    bool             _filterOrNot;
    Volume< float >  _filter;
    int32_t          _gridRadius;
    float            _R0min;
    float            _R0max;
    bool             _marginalODF; 


};


}


#endif
