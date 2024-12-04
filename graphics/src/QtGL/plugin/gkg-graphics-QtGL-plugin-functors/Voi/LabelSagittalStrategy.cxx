#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelSagittalStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-processing-container/Volume_i.h>


gkg::VoiLabelSagittalStrategy::VoiLabelSagittalStrategy() 
                              : gkg::VoiLabelStrategy()
{
}


gkg::VoiLabelStrategy::TypeId gkg::VoiLabelSagittalStrategy::id()
{ 

  return gkg::VoiLabelStrategy::ModeSagittal;
  
}


void gkg::VoiLabelSagittalStrategy::doit(
                                VoiData* d,
                                gkg::Volume< int16_t >& lab, 
                                gkg::Vector3d< int32_t >& pt,
                                int32_t, 
                                int32_t col, 
                                std::list< Vector2d< int32_t > >& brushOffsets )
{

  int32_t dy = lab.getSizeY();
  int32_t dz = lab.getSizeZ();
  std::list< gkg::Vector3d< int32_t > > carre;
  std::list< gkg::Vector2d< int32_t > >::iterator
    b = brushOffsets.begin(),
    be = brushOffsets.end();
  int32_t centerX = pt.y, centerY = pt.z;
  int32_t i, j;

  while ( b != be )
  {
    
    i = centerX + b->x;
    j = centerY + b->y;

    if ( ( i >= 0 ) && ( i < dy ) && ( j >= 0 ) && ( j < dz ) )
    {
    
      lab( pt.x, i, j ) = (int16_t)col;
      carre.push_back( gkg::Vector3d< int32_t >( pt.x, i, j ) );

    }

    ++b;
    
  }

  d->notifyLabel( carre );

}


void gkg::VoiLabelSagittalStrategy::spline( VoiData* d, 
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
    lab( slice, x, y ) = label;
    gkg::Vector3d< int32_t > pt( slice, x, y );
    bck.push_back( pt );
    ++p;

  }

  d->notifyLabel( bck );

}
