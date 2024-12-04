#ifndef _gkg_dmri_odf_qbi_QBallOdfCartesianField_h_
#define _gkg_dmri_odf_qbi_QBallOdfCartesianField_h_


#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/PhiFunction.h>


namespace gkg
{


class QBallOdfCartesianField : public OdfCartesianField
{

  public:

    QBallOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );
    QBallOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< float >& boundingBox,
                     const Vector3d< float >& superResolution,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );
    QBallOdfCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< int32_t >& boundingBox,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    QBallOdfCartesianField( RCPointer< Volume< float > > t2,
                            RCPointer< Volume< float > > dw,
                            RCPointer< Volume< int16_t > > mask,
                            int32_t equatorPointCount,
                            RCPointer< PhiFunction > phiFunction,
                            const OrientationSet* outputOrientationSet = 0,
                            const OrientationSet* basisFunctionCenterSet = 0,
                            bool verbose = false );
    QBallOdfCartesianField( RCPointer< Volume< float > > t2,
                            RCPointer< Volume< float > > dw,
                            RCPointer< Volume< int16_t > > mask,
                            const BoundingBox< float >& boundingBox,
                            const Vector3d< float >& superResolution,
                            int32_t equatorPointCount,
                            RCPointer< PhiFunction > phiFunction,
                            const OrientationSet* outputOrientationSet = 0,
                            const OrientationSet* basisFunctionCenterSet = 0,
                            bool verbose = false );
    QBallOdfCartesianField( RCPointer< Volume< float > > t2,
                            RCPointer< Volume< float > > dw,
                            RCPointer< Volume< int16_t > > mask,
                            const BoundingBox< int32_t >& boundingBox,
                            int32_t equatorPointCount,
                            RCPointer< PhiFunction > phiFunction,
                            const OrientationSet* outputOrientationSet = 0,
                            const OrientationSet* basisFunctionCenterSet = 0,
                            bool verbose = false );
    virtual ~QBallOdfCartesianField();

    int32_t getEquatorPointCount() const;
    const OrientationSet& getBasisFunctionCenterSet() const;

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

    int32_t _equatorPointCount;
    RCPointer< PhiFunction > _phiFunction;
    OrientationSet _basisFunctionCenterSet;
    bool _identicalBFCSetAndOOSet;
    Matrix _reconstructionMatrix;

};


}


#endif

