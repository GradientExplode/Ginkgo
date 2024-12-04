#include <gkg-graphics-QtGL-plugin-functors/Voi/SplineItem.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Node.h>
#include <gkg-processing-numericalanalysis/AkimaSpline3d.h>

#include <QPen>

#include <cmath>


gkg::SplineItem::SplineItem( QGraphicsItem* parent )
               : QGraphicsPathItem( parent )
{
}


void gkg::SplineItem::setColor( gkg::RGBComponent color )
{

  _color = color;
  update();

}


void gkg::SplineItem::addKnot( gkg::Node* node )
{

  _knots.push_back( node );
  update();

}


void gkg::SplineItem::insertKnot( gkg::Node* node )
{

  double dist = 1e6;
  int32_t i = 0, n = (int32_t)_knots.size() - 1, pos = n;

  for ( i = 0; i < n; i++ )
  {

    gkg::Vector3d< double > A( _knots[ i ]->scenePos().x(),
  			       _knots[ i ]->scenePos().y(),
  			       0.0 );
    gkg::Vector3d< double > B( _knots[ i + 1 ]->scenePos().x(), 
  			       _knots[ i + 1 ]->scenePos().y(),
  			       0.0 );
    gkg::Vector3d< double > P( node->scenePos().x(),
  			       node->scenePos().y(),
  			       0.0 );
    gkg::Vector3d< double > AB = B - A;
    gkg::Vector3d< double > AP = P - A;
    double scalar = AB.dot( AP );
    double normAB = AB.getNorm2();
    double normAP = AP.getNorm2();

    if ( ( scalar >= 0.0 ) && ( scalar < normAB ) )
    {

      double d = std::sqrt( normAP - scalar * scalar / normAB );

      if ( d < dist )
      {

        dist = d;
        pos = i + 1;

      }

    }

  }

  _knots.insert( pos, node );
  update();

}


void gkg::SplineItem::removeKnot( gkg::Node* node )
{

  if ( _knots.count( node ) == 2 )
  {

    _knots.removeAll( node );
    _knots.push_back( _knots.front() );

  }
  else
  {

    _knots.removeOne( node );

  }

  update();

}


void gkg::SplineItem::closeSpline()
{

  _knots.push_back( _knots.front() );
  update();

}


QList< gkg::Node* >& gkg::SplineItem::getKnots()
{

  return _knots;

}


QPolygonF& gkg::SplineItem::getPolygon()
{

  return _polygon;

}


void gkg::SplineItem::clear()
{

  _knots.clear();
  update();

}


int32_t gkg::SplineItem::size()
{

  return int32_t( _knots.size() );

}


void gkg::SplineItem::update()
{

  QPainterPath path;

  if ( _knots.size() )
  {

    std::vector< gkg::Vector3d< double > > curve;

    foreach( gkg::Node* node, _knots )
    {

      curve.push_back( gkg::Vector3d< double >( node->scenePos().x(),
                                                node->scenePos().y(),
                                                0.0 ) );

    }

    gkg::LightCurve3d< double > curve3d( curve );
    gkg::AkimaSpline3d< double > spline( curve3d );
    gkg::LightCurve3d< double > interp;

    spline.getInterpolatedCurve3d( interp, 10 );
    _polygon.clear();

    gkg::LightCurve3d< double >::const_iterator
      p = interp.begin(),
      pe = interp.end();

    while ( p != pe )
    {

      _polygon << QPointF( p->x, p->y );
      ++p;

    }

    path.setFillRule( Qt::WindingFill );
    path.addPolygon( _polygon );
    path.closeSubpath();

    setPen( QPen( QColor( _color.r, _color.g, _color.b ), 2 ) );
    setBrush( QBrush( QColor( _color.r, _color.g, _color.b, 63 ) ) );

  }

  setPath( path );

}
