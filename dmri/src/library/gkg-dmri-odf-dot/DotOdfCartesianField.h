#ifndef _gkg_dmri_odf_dot_DotOdfCartesianField_h_
#define _gkg_dmri_odf_dot_DotOdfCartesianField_h_

#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector2d.h>
#include <gkg-processing-numericalanalysis/Matrix.h>

namespace gkg
{

class DotOdfCartesianField : public OdfCartesianField
{

  public:

    DotOdfCartesianField(
		     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
		     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    DotOdfCartesianField(
                     RCPointer< Volume< float > > t2,
		     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< float >& boundingBox,
                     const Vector3d< float >& superResolution,
		     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    DotOdfCartesianField(
		     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< int32_t >& boundingBox,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    virtual ~DotOdfCartesianField();

    int32_t getMaximumSphericalHarmonicOrder() const;
    int32_t getSphericalHarmonicCount() const;
    float getEffectiveDiffusionTime() const;
    float getR0() const;
    bool getMarginalODF() const;
    
    static void checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters );

  protected:

    void getReconstructionMatrix( bool verbose );

    OrientationDistributionFunction
      newItem( const Vector3d< int32_t >& originSite,
               const Vector3d< int32_t >& lutSite );

    OrientationDistributionFunction
      newItem( const Vector3d< float >& originSite,
               const Vector3d< int32_t >& lutSite );

    int32_t _maximumSphericalHarmonicOrder;
    float _effectiveDiffusionTime; 
    float _R0;
    bool  _marginalODF; 
    
    Matrix _reconstructionMatrix;
    Matrix _Bplus;

};


}


#endif

