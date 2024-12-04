#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelCoronalStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ConnectivityFactory.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/Volume_i.h>


gkg::VoiLabelCoronalStrategy::VoiLabelCoronalStrategy() 
                             : gkg::VoiLabelStrategy()
{
}


gkg::VoiLabelStrategy::TypeId gkg::VoiLabelCoronalStrategy::id()
{ 

  return VoiLabelStrategy::ModeCoronal;

}


void gkg::VoiLabelCoronalStrategy::doit(
                                VoiData* d,
                                gkg::Volume< int16_t >& lab, 
                                gkg::Vector3d< int32_t >& pt, int32_t, 
                                int32_t col, 
                                std::list< Vector2d< int32_t > >& brushOffsets )
{

  int32_t dx = lab.getSizeX();
  int32_t dz = lab.getSizeZ();
  std::list< gkg::Vector3d< int32_t > > carre;
  std::list< gkg::Vector2d< int32_t > >::iterator
    b = brushOffsets.begin(),
    be = brushOffsets.end();
  int32_t centerX = pt.x, centerY = pt.z;
  int32_t i, j;

  while ( b != be )
  {
    
    i = centerX + b->x;
    j = centerY + b->y;

    if ( ( i >= 0 ) && ( i < dx ) && ( j >= 0 ) && ( j < dz ) )
    {
    
      lab( i, pt.y, j ) = (int16_t)col;
      carre.push_back( gkg::Vector3d< int32_t >( i, pt.y, j ) );

    }

    ++b;
    
  }

  d->notifyLabel( carre );

}


void gkg::VoiLabelCoronalStrategy::spline( VoiData* d, 
                                           gkg::Volume< int16_t >& lab, 
                                           std::list< QPoint >& spline,
                                           int32_t slice, int32_t label )
{

  std::list< QPoint >::iterator
    p  = spline.begin(),
    pe = spline.end();

  bck.clear();

  while ( p != pe )
  {

    int32_t x = p->x();
    int32_t y = p->y();
    lab( x, slice, y ) = label;
    gkg::Vector3d< int32_t > pt( x, slice, y );
    bck.push_back( pt );
    ++p;

  }

  d->notifyLabel( bck );

}
