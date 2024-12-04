#include <gkg-graphics-QtGL-plugin-functors/Voi/SplineCache.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/GraphicsScene.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Node.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ValidateState.h>


gkg::SplineCache::SplineCache()
{
}


gkg::SplineCache::~SplineCache()
{
}


void gkg::SplineCache::save( QList< gkg::Node* >& knots )
{

  _knots.clear();

  foreach ( gkg::Node* node, knots )
  {

    _knots.push_back( node->scenePos() );

  }

}


void gkg::SplineCache::initializeScene( gkg::GraphicsScene* scene )
{

  if ( scene )
  {

    std::list< QPointF >::iterator
      k = _knots.begin(),
      ke = _knots.end();

    while ( k != ke )
    {

      scene->addNode( *k );
      ++k;

    }

    scene->setState( new gkg::ValidateState( scene ) );

  }

}
