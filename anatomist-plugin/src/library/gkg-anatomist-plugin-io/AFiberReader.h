#ifndef _gkg_anatomist_plugin_io_AFiberReader_h_
#define _gkg_anatomist_plugin_io_AFiberReader_h_


#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-coordinates/Vector3d.h>

#include <QVector3D>
#include <QVector4D>
#include <QtOpenGL>

#include <vector>


namespace gkg
{


class AFiberReader
{

  public:

    AFiberReader( const std::string& fileNameBundleMap );
    virtual ~AFiberReader();

    const BoundingBox< float >& getBoundingBox() const;
    void getFiberCoordinates( 
               int32_t samplingValue,
               std::vector< QVector3D* >& coordinates, 
               std::vector< int32_t >& fiberCount,
               std::vector< std::pair< std::string, int32_t > >& bundleNames );

  protected:

    std::string _fileNameBundleMap;
    BoundingBox< float > _boundingBox;

};


}


#endif
