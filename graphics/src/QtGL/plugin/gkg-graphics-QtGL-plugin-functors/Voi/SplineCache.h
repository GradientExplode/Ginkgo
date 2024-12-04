#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_SplineCache_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_SplineCache_h_


#include <QList>
#include <QPointF>

#include <list>


namespace gkg
{


class Node;
class GraphicsScene;


class SplineCache
{

  public:

    SplineCache();
    virtual ~SplineCache();

    void save( QList< Node* >& knots );
    void initializeScene( GraphicsScene* scene );

  private:

    std::list< QPointF > _knots;

};


}


#endif
