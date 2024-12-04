#ifndef _gkg_dmri_odf_dti_TensorOdfCartesianField_h_
#define _gkg_dmri_odf_dti_TensorOdfCartesianField_h_


#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-dmri-pdf-dti/TensorCartesianField.h>


namespace gkg
{


class TensorOdfCartesianField : public OdfCartesianField
{

  public:

    TensorOdfCartesianField( RCPointer< Volume< float > > t2,
                             RCPointer< Volume< float > > dw,
                             RCPointer< Volume< int16_t > > mask,
                             const OrientationSet* outputOrientationSet,
                             const std::vector< double >&
                                                specificScalarParameters,
                             const std::vector< std::string >&
                                                specificStringParameters,
                             bool verbose = false );
    TensorOdfCartesianField( RCPointer< Volume< float > > t2,
                             RCPointer< Volume< float > > dw,
                             RCPointer< Volume< int16_t > > mask,
                             const BoundingBox< float >& boundingBox,
                             const Vector3d< float >& superResolution,
                             const OrientationSet* outputOrientationSet,
                             const std::vector< double >&
                                                specificScalarParameters,
                             const std::vector< std::string >&
                                                specificStringParameters,
                             bool verbose = false );
    TensorOdfCartesianField( RCPointer< Volume< float > > t2,
                             RCPointer< Volume< float > > dw,
                             RCPointer< Volume< int16_t > > mask,
                             const BoundingBox< int32_t >& boundingBox,
                             const OrientationSet* outputOrientationSet,
                             const std::vector< double >&
                                                specificScalarParameters,
                             const std::vector< std::string >&
                                                specificStringParameters,
                             bool verbose = false );

    virtual ~TensorOdfCartesianField();

    TensorCartesianField& getTensorCartesianField();

    static void checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters );
    static Tensor::FitMethod 
      getFitMethod( const std::string& specificStringParameter );

  protected:

    OrientationDistributionFunction
      newItem( const Vector3d< int32_t >& originSite,
               const Vector3d< int32_t >& lutSite );
    OrientationDistributionFunction
      newItem( const Vector3d< float >& originSite,
               const Vector3d< int32_t >& lutSite );

    TensorCartesianField _tensorCartesianField;

};


}


#endif

