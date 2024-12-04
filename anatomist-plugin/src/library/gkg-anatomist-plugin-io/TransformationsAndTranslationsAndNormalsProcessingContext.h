#ifndef _gkg_anatomist_plugin_io_TransformationsAndTranslationsAndNormalsProcessingContext_h_
#define _gkg_anatomist_plugin_io_TransformationsAndTranslationsAndNormalsProcessingContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/Translation3d.h>
#include <vector>
#include <QtOpenGL>
#include <QVector3D>
#include <QVector4D>


namespace gkg
{


class TransformationsAndTranslationsAndNormalsProcessingContext :
                                                   public LoopContext< int32_t >
{

  public:

    TransformationsAndTranslationsAndNormalsProcessingContext(
      const BoundingBox< float >& boundingBox,
      int32_t outputOrientationCount,
      const MeshMap< int32_t, float, 3U >& sphereMeshMap,
      const Vector3d< double >& resolution,
      const std::vector< std::vector< float > >& transformations,
      const std::vector< Translation3d< float > >& translations,
      std::vector< std::vector< std::vector< GLfloat*  > > >& glTransformations,
      std::vector< std::vector< std::vector< QVector4D  > > >& glTranslations,
      std::vector < std::vector < std::vector < QVector3D* > > >& glNormals );
    virtual ~TransformationsAndTranslationsAndNormalsProcessingContext();

    void doIt( int32_t startIndex, int32_t countIndex  );

  private:

    const BoundingBox< float >& _boundingBox;
    int32_t _outputOrientationCount;
    const MeshMap< int32_t, float, 3U >& _sphereMeshMap;
    Vector3d< double > _resolution;
    const std::vector< std::vector< float > >& _transformations;
    const std::vector< Translation3d< float > >& _translations;
    std::vector< std::vector< std::vector< GLfloat*  > > >& _glTransformations;
    std::vector< std::vector< std::vector< QVector4D  > > >& _glTranslations;
    std::vector < std::vector < std::vector < QVector3D* > > >& _glNormals;

};



}


#endif
