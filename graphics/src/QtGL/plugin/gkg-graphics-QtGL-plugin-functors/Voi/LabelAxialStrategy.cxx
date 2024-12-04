#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelAxialStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-processing-container/Volume_i.h>


gkg::VoiLabelAxialStrategy::VoiLabelAxialStrategy() 
                           : gkg::VoiLabelStrategy()
{
}


gkg::VoiLabelStrategy::TypeId gkg::VoiLabelAxialStrategy::id()
{

  return gkg::VoiLabelStrategy::ModeAxial;
  
}


void gkg::VoiLabelAxialStrategy::doit(
                                VoiData* d, gkg::Volume< int16_t >& lab, 
                                gkg::Vector3d< int32_t >& pt, int32_t, 
                                int32_t col, 
                                std::list< Vector2d< int32_t > >& brushOffsets )
{

  int32_t dx = lab.getSizeX();
  int32_t dy = lab.getSizeY();
  std::list< gkg::Vector3d< int32_t > > carre;
  std::list< gkg::Vector2d< int32_t > >::iterator
    b = brushOffsets.begin(),
    be = brushOffsets.end();
  int32_t centerX = pt.x, centerY = pt.y;
  int32_t i, j;

  while ( b != be )
  {
    
    i = centerX + b->x;
    j = centerY + b->y;

    if ( ( i >= 0 ) && ( i < dx ) && ( j >= 0 ) && ( j < dy ) )
    {
    
      lab( i, j, pt.z ) = (int16_t)col;
      carre.push_back( gkg::Vector3d< int32_t >( i, j, pt.z ) );

    }

    ++b;
    
  }

  d->notifyLabel( carre );

}


void gkg::VoiLabelAxialStrategy::spline( VoiData* d, 
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
    lab( x, y, slice ) = label;
    gkg::Vector3d< int32_t > pt( x, y, slice );
    bck.push_back( pt );
    ++p;

  }

  d->notifyLabel( bck );

}
