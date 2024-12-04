#ifndef _gkg_anatomist_plugin_io_APdfFieldReader_h_
#define _gkg_anatomist_plugin_io_APdfFieldReader_h_


#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/SiteMap.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-container/PdfCartesianField.h>

#include <QVector3D>
#include <QVector4D>
#include <QtOpenGL>

#include <vector>


namespace gkg
{


class APdfFieldReader
{

  public:

    APdfFieldReader( const std::string& fileNameSiteMap,
                     const std::string& fileNameTextureMap,
                     float& displacementMagnitude,
                     int32_t outputOrientationCount,
                     int32_t resamplingNeighborCount,
                     float resamplingGaussianSigma );
    virtual ~APdfFieldReader();

    Vector3d< double > getResolution() const;
    const BoundingBox< float >& getBoundingBox() const;
    bool isPolarHarmonics() const;
    float getDisplacementMagnitude() const;
    int32_t getOutputOrientationCount() const;

    void getSphereVertexCoordinatesAndIndices(
                                         QVector< GLfloat >& vertexCoordinates,
                                         QVector<GLuint>& vertexIndices ) const;

    void getTransformationsAndTranslations(
      std::vector< std::vector< std::vector< GLfloat*  > > >& transformations,
      std::vector< std::vector< std::vector< QVector4D  > > >& translations );
    void getTransformationsAndTranslationsAndNormals( 
      std::vector< std::vector< std::vector< GLfloat*  > > >& transformations,
      std::vector< std::vector< std::vector< QVector4D  > > >& translations,
      std::vector < std::vector < std::vector < QVector3D* > > >& normals );
    void getNormals(
      std::vector < std::vector < std::vector < QVector3D * > > >& normals );


  protected:

    void computePdfField();
    void computeBoundingBox();

    BoundingBox< float > _boundingBox;
    float _displacementMagnitude;
    int32_t _outputOrientationCount;
    int32_t _resamplingNeighborCount;
    float _resamplingGaussianSigma;
    bool _isPolarHarmonics;
    float _meshScale;

    SiteMap< int32_t, int32_t > _siteMap;
    TextureMap< ProbabilityDensityFunction > _pdfs;
    std::map< float, OrientationSet >* _originalOutputMultipleShellSampling;
    std::map< float, OrientationSet >* _outputMultipleShellSampling;
    MeshMap< int32_t, float, 3U > _sphereMeshMap;

    PdfCartesianField* _pdfCartesianField;

    std::vector< std::vector< float > > _transformations;
    std::vector< Translation3d< float > > _translations;

};



}


#endif
