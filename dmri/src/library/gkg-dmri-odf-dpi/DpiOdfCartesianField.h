#ifndef _gkg_dmri_odf_dpi_DpiOdfCartesianField_h_
#define _gkg_dmri_odf_dpi_DpiOdfCartesianField_h_


#include <gkg-dmri-pdf-dpi/DpiEapCartesianField.h>
#include <gkg-processing-numericalanalysis/Matrix.h>



namespace gkg
{

class DpiOdfCartesianField : public DpiEapCartesianField
{

  public:

    DpiOdfCartesianField(
		     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
		     std::vector< float > meshRadii,
                     bool verbose = false );
    
    DpiOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< float >& boundingBox,
                     const Vector3d< float >& superResolution,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
		     std::vector< float > meshRadii,
                     bool verbose = false );

    DpiOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< int32_t >& boundingBox,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
		     std::vector< float > meshRadii,
                     bool verbose = false );

    virtual ~DpiOdfCartesianField();

    float getMaximumR0Value() const;
    float getMinimumR0Value() const;

    static void checkOrInitializeDefaultParameters(
					     std::vector< float > meshRadii );

  protected:

    OrientationDistributionFunction
      newItem( const Vector3d< int32_t >& originSite,
               const Vector3d< int32_t >& lutSite );

    OrientationDistributionFunction
      newItem( const Vector3d< float >& originSite,
               const Vector3d< int32_t >& lutSite );

    float _R0min;
    float _R0max;
    
};


}


#endif


