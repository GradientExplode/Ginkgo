#include <gkg-graphics-X11-core/ArrayCompositor.h>
#include <algorithm>


gkg::ArrayCompositor::ArrayCompositor( gkg::Compositor::Index count )
                     : gkg::Compositor(),
                       _count( count )
{
}


gkg::ArrayCompositor::~ArrayCompositor()
{
}


gkg::Compositor::Index 
gkg::ArrayCompositor::compose( gkg::FloatCoordinate*,
                               gkg::FloatCoordinate*,
                               gkg::FloatCoordinate*,
                               int32_t*,
                               gkg::Compositor::Index componentCount,
                               gkg::FloatCoordinate*,
                               gkg::Compositor::Index,
                               gkg::Compositor::Index* breaks,
                               gkg::Compositor::Index breakCount )
{

  gkg::Compositor::Index 
    count = std::min( ( componentCount - 1 ) / _count + 1, breakCount );

  for ( gkg::Compositor::Index i = 0; i < count; i++ )
  {

    breaks[ i ] = std::min( ( i + 1 ) * _count - 1, componentCount - 1 );

  }

  return count;

}
