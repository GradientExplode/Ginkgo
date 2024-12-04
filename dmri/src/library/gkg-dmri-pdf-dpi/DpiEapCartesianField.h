#ifndef _gkg_dmri_pdf_dpi_DpiEapCartesianField_h_
#define _gkg_dmri_pdf_dpi_DpiEapCartesianField_h_


#include <gkg-dmri-container/EapCartesianField.h>
#include <gkg-processing-numericalanalysis/Matrix.h>

namespace gkg
{


class DpiEapCartesianField : public EapCartesianField
{

  public:

    DpiEapCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    DpiEapCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< float >& boundingBox,
                     const Vector3d< float >& superResolution,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    DpiEapCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< int32_t >& boundingBox,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    virtual ~DpiEapCartesianField();

    int32_t getMaximumSphericalHarmonicOrder() const;
    int32_t getDataSphericalHarmonicCount() const;
    int32_t getEapSphericalHarmonicCount() const;
    int32_t getNumberOfShells() const;
    int32_t getTotalNumberOfShells() const;
    int32_t getNumberOfOrientationsPerShell() const;
    int32_t getTotalNumberOfOrientations() const;


    const Matrix& getReconstructionMatrix() const;

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

    int32_t               _numberOfShells;
    int32_t               _numberOfOrientationsPerShell;    
    int32_t               _maximumSphericalHarmonicOrder;
    std::vector< float >  _qValues;
    Matrix                _reconstructionMatrix;

};


}


#endif
