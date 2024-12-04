#include <gkg-graphics-X11-core/SimpleCompositor.h>
#include <gkg-graphics-X11-core/Glyph.h>
#include <algorithm>


gkg::SimpleCompositor::SimpleCompositor()
                      : gkg::Compositor()
{
}


gkg::SimpleCompositor::~SimpleCompositor()
{
}


gkg::Compositor::Index 
gkg::SimpleCompositor::compose( gkg::FloatCoordinate* naturals,
                                gkg::FloatCoordinate*,
                                gkg::FloatCoordinate*,
                                int32_t* penalties,
                                gkg::Compositor::Index componentCount,
                                gkg::FloatCoordinate* spans,
                                gkg::Compositor::Index spanCount,
                                gkg::Compositor::Index* breaks,
                                gkg::Compositor::Index breakCount )
{

  gkg::FloatCoordinate span = 0;
  bool breakable = false;
  gkg::Compositor::Index count = 0;
  int32_t penalty;
  for ( gkg::Compositor::Index i = 0; i < componentCount; i++ )
  {

    if ( i == componentCount - 1 )
    {

      penalty = gkg::Compositor::PenaltyGood;
      span = fil;

    }
    else
    {

      penalty = penalties[ i ];
      span += naturals[ i ];

    }
    if ( penalty < gkg::Compositor::PenaltyBad )
    {

      breakable = true;

    }
    if ( span > spans[ std::min( count, spanCount - 1 ) ] )
    {

      while ( breakable && penalty == gkg::Compositor::PenaltyBad )
      {

        -- i;
        penalty = penalties[ i ];

      }
      if ( count < breakCount )
      {

        breaks[ count ] = i;

      }
      span = 0;
      breakable = false;
      ++ count;

    }

  }
  return std::min( count, breakCount );

}
