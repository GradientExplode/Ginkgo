#ifndef _gkg_anatomist_plugin_io_ATwoPoolModelFieldReader_h_
#define _gkg_anatomist_plugin_io_ATwoPoolModelFieldReader_h_


#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/SiteMap.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelCartesianField.h>

#include <QVector3D>
#include <QVector4D>
#include <QtOpenGL>

#include <vector>


namespace gkg
{


class ATwoPoolModelFieldReader
{

  public:

    ATwoPoolModelFieldReader( const std::string& fileNameSiteMap,
                              const std::string& fileNameTextureMap,
                              int32_t outputOrientationCount );
    virtual ~ATwoPoolModelFieldReader();

    Vector3d< double > getResolution() const;
    const BoundingBox< float >& getBoundingBox() const;
    int32_t getOutputOrientationCount() const;

    void getSphereVertexCoordinatesAndIndices(
                                         QVector< GLfloat >& vertexCoordinates,
                                         QVector<GLuint>& vertexIndices ) const;

    void getTransformationsAndTranslations(
      std::vector< std::vector< std::vector< GLfloat*  > > >&
                                                            fastTransformations,
      std::vector< std::vector< std::vector< GLfloat*  > > >&
                                                            slowTransformations,
      std::vector< std::vector< std::vector< QVector4D  > > >& translations );
    void getTransformationsAndTranslationsAndNormals( 
      std::vector< std::vector< std::vector< GLfloat*  > > >&
                                                            fastTransformations,
      std::vector< std::vector< std::vector< GLfloat*  > > >&
                                                            slowTransformations,
      std::vector< std::vector< std::vector< QVector4D  > > >& translations,
      std::vector < std::vector < std::vector < QVector3D* > > >& fastNormals,
      std::vector < std::vector < std::vector < QVector3D* > > >& slowNormals );
    void getNormals(
      std::vector < std::vector < std::vector < QVector3D * > > >& 
                                                                  fastNormals,
      std::vector < std::vector < std::vector < QVector3D * > > >& 
                                                                  slowNormals );


  protected:

    void computeTwoPoolModelField();
    void computeBoundingBox();

    BoundingBox< float > _boundingBox;
    int32_t _outputOrientationCount;
    float _meshScale;

    SiteMap< int32_t, int32_t > _siteMap;
    TextureMap< TwoPoolModel > _twoPoolModels;
    OrientationSet* _outputOrientationSet;
    MeshMap< int32_t, float, 3U > _sphereMeshMap;

    gkg::TwoPoolModelCartesianField* _twoPoolModelCartesianField;

    std::vector< std::vector< float > > _fastTransformations;
    std::vector< std::vector< float > > _slowTransformations;
    std::vector< Translation3d< float > > _translations;

};



}


#endif
