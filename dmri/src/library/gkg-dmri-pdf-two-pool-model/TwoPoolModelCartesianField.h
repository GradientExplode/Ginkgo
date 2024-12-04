#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModelCartesianField_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModelCartesianField_h_


#include <gkg-dmri-container/DiffusionCartesianField.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{
  

class TwoPoolModelCartesianField :
                                  public DiffusionCartesianField< TwoPoolModel >
{

 public:

    TwoPoolModelCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );
    TwoPoolModelCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< float >& boundingBox,
                     const Vector3d< float >& superResolution,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );
    TwoPoolModelCartesianField(
                     RCPointer< Volume< float > > t2,
                     RCPointer< Volume< float > > dw,
                     RCPointer< Volume< int16_t > > mask,
                     const BoundingBox< int32_t >& boundingBox,
                     const OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose = false );

    TwoPoolModelCartesianField( const SiteMap< int32_t, int32_t >& siteMap,
                       const TextureMap< TwoPoolModel >& items,
                       const OrientationSet& outputOrientationSet );

    virtual ~TwoPoolModelCartesianField();

    const OrientationSet& getOutputOrientationSet() const;

    static void checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters );
    static TwoPoolModel::FitMethod 
      getFitMethod( const std::string& specificStringParameter );

  protected:

    void getQSpaceSamples();

    TwoPoolModel
      newItem( const Vector3d< int32_t >& originSite,
               const Vector3d< int32_t >& lutSite );
    TwoPoolModel
      newItem( const Vector3d< float >& originSite,
               const Vector3d< int32_t >& lutSite );

    void setOutputOrientationSet( const OrientationSet* outputOrientationSet );


    TwoPoolModel::FitMethod _fitMethod;
    Matrix _qSpaceSamples;
    OrientationSet _outputOrientationSet;

};


}


#endif
