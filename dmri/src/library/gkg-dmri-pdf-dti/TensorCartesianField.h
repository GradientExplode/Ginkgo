#ifndef _gkg_dmri_pdf_dti_TensorCartesianField_h_
#define _gkg_dmri_pdf_dti_TensorCartesianField_h_


#include <gkg-dmri-container/DiffusionCartesianField.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{
  

class TensorCartesianField : public DiffusionCartesianField< Tensor >
{

 public:

    TensorCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    TensorCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< float >& boundingBox,
                     const Vector3d< float >& superResolution,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    TensorCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< int32_t >& boundingBox,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    virtual ~TensorCartesianField();

    static void checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters );
    static Tensor::FitMethod 
      getFitMethod( const std::string& specificStringParameter );

  protected:


    void getBMatrix();
    void getOrientationSVDDecomposition();

    Tensor
      newItem( const Vector3d< int32_t >& originSite,
               const Vector3d< int32_t >& lutSite );
    Tensor
      newItem( const Vector3d< float >& originSite,
               const Vector3d< int32_t >& lutSite );


    //void calculatingOnGridTmp( const gkg::BoundingBox< int32_t >& boundingBox,
    //                            bool verbose );

    Tensor::FitMethod _fitMethod;
    Matrix _matrixB;
    Matrix _matrixU;
    Vector _vectorS;
    Matrix _matrixV;

};


}


#endif
