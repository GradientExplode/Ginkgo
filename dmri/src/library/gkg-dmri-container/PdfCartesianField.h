#ifndef _gkg_dmri_container_PdfCartesianField_h_
#define _gkg_dmri_container_PdfCartesianField_h_


#include <gkg-dmri-container/DiffusionCartesianField.h>
#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-communication-thread/LoopContext.h>
#include <map>


namespace gkg
{


class PdfSamplingContext : public LoopContext< int32_t >
{

  public:

    PdfSamplingContext( TextureMap< ProbabilityDensityFunction >& pdfs,
                        int32_t* lut,
                        std::map< float, OrientationSet >& omss,
                        int32_t resamplingNeighborCount,
                        float resamplingGaussianSigma );
			     
    void doIt( int32_t startIndex, int32_t count );

  private:

    TextureMap< ProbabilityDensityFunction >& _pdfs;
    int32_t* _lut;
    std::map< float, OrientationSet >& _omss;
    int32_t _resamplingNeighborCount;
    float _resamplingGaussianSigma;

};


//
// class PdfCartesianField
//


class PdfCartesianField : public DiffusionCartesianField<
                                                    ProbabilityDensityFunction >
{

  public:

    PdfCartesianField( int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                       const Vector3d< double >& resolution,
                       const SiteMap< int32_t, int32_t >& siteMap,
                       int32_t offsetX,
                       int32_t offsetY,
                       int32_t offsetZ,
                       const std::map< float, OrientationSet >&
                                                  outputMultipleShellSampling );
    PdfCartesianField( int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                       const Vector3d< double >& resolution,
                       const SiteMap< int32_t, int32_t >& siteMap,
                       const TextureMap< ProbabilityDensityFunction >& items,
                       int32_t offsetX,
                       int32_t offsetY,
                       int32_t offsetZ,
                       const std::map< float, OrientationSet >&
                                                  outputMultipleShellSampling );
    PdfCartesianField(
                     const SiteMap< int32_t, int32_t >& siteMap,
                     const TextureMap< ProbabilityDensityFunction >& items,
                     const std::map< float, OrientationSet >&
                                                    outputMultipleShellSampling,
                     int32_t resamplingNeighborCount = 3,
                     float resamplingGaussianSigma = 1.0f );

    PdfCartesianField(
                      int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                      const Vector3d< double >& resolution,
                      const SiteMap< int32_t, int32_t >& siteMap,
                      int32_t offsetX,
                      int32_t offsetY,
                      int32_t offsetZ,
                      const Vector3d< int32_t >& outputCartesianGridSize,
                      const Vector3d< double >& outputCartesianGridResolution );
    PdfCartesianField(
                      int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                      const Vector3d< double >& resolution,
                      const SiteMap< int32_t, int32_t >& siteMap,
                      const TextureMap< ProbabilityDensityFunction >& items,
                      int32_t offsetX,
                      int32_t offsetY,
                      int32_t offsetZ,
                      const Vector3d< int32_t >& outputCartesianGridSize,
                      const Vector3d< double >& outputCartesianGridResolution );
    PdfCartesianField(
                     const SiteMap< int32_t, int32_t >& siteMap,
                     const TextureMap< ProbabilityDensityFunction >& items,
                     const Vector3d< int32_t >& outputCartesianGridSize,
                     const Vector3d< double >& outputCartesianGridResolution );
 
    virtual ~PdfCartesianField();

    bool hasMultipleShellOutputSampling() const;

    const std::map< float, OrientationSet >& 
                                         getOutputMultipleShellSampling() const;
    const Vector3d< int32_t >& getOutputCartesianGridSize() const;
    const Vector3d< double >& getOutputCartesianGridResolution() const;

    ProbabilityDensityFunction::BasisType getBasisType() const;
    std::string getBasisTypeName() const;
    void setStandardBasis( bool verbose = false );

  protected:

    PdfCartesianField(
     RCPointer< Volume< float > > t2,
     RCPointer< Volume< float > > dw,
     RCPointer< Volume< int16_t > > mask,
     const std::map< float, OrientationSet >* outputMultipleShellSampling = 0 );
    PdfCartesianField(
     RCPointer< Volume< float > > t2,
     RCPointer< Volume< float > > dw,
     RCPointer< Volume< int16_t > > mask,
     const BoundingBox< float >& boundingBox,
     const Vector3d< float >& superResolution,
     const std::map< float, OrientationSet >* outputMultipleShellSampling = 0 );
    PdfCartesianField(
     RCPointer< Volume< float > > t2,
     RCPointer< Volume< float > > dw,
     RCPointer< Volume< int16_t > > mask,
     const BoundingBox< int32_t >& boundingBox,
     const std::map< float, OrientationSet >* outputMultipleShellSampling = 0 );

    PdfCartesianField(
     RCPointer< Volume< float > > t2,
     RCPointer< Volume< float > > dw,
     RCPointer< Volume< int16_t > > mask,
     const Vector3d< int32_t >* outputCartesianGridSize = 0,
     const Vector3d< double >* outputCartesianGridResolution = 0 );
    PdfCartesianField(
     RCPointer< Volume< float > > t2,
     RCPointer< Volume< float > > dw,
     RCPointer< Volume< int16_t > > mask,
     const BoundingBox< float >& boundingBox,
     const Vector3d< float >& superResolution,
     const Vector3d< int32_t >* outputCartesianGridSize = 0,
     const Vector3d< double >* outputCartesianGridResolution = 0 );
    PdfCartesianField(
     RCPointer< Volume< float > > t2,
     RCPointer< Volume< float > > dw,
     RCPointer< Volume< int16_t > > mask,
     const BoundingBox< int32_t >& boundingBox,
     const Vector3d< int32_t >* outputCartesianGridSize = 0,
     const Vector3d< double >* outputCartesianGridResolution = 0 );

    void setOutputMultipleShellSampling(
         const std::map< float, OrientationSet >* outputMultipleShellSampling );
    void setOutputCartesianGridSampling(
         const Vector3d< int32_t >* outputCartesianGridSize,
         const Vector3d< double >* outputCartesianGridResolution );

    bool _hasMultipleShellOutputSampling;
    std::map< float, OrientationSet > _outputMultipleShellSampling;
    Vector3d< int32_t > _outputCartesianGridSize;
    Vector3d< double > _outputCartesianGridResolution;

};


}


#endif
