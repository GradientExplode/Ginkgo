#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-graphics-X11-core/Target.h>


gkg::Target::Target( gkg::Glyph* body,
                     gkg::Target::Sensitivity sensitivity )
            : gkg::MonoGlyph( body ),
              _sensitivity( sensitivity )
{
}


gkg::Target::~Target()
{
}


void gkg::Target::pick( gkg::Canvas* canvas,
                        const gkg::GeometryAllocation& geometryAllocation,
                        int32_t depth,
                        gkg::Hit& hit )
{

  gkg::FloatCoordinate x, left, right;
  switch ( _sensitivity )
  {

    case gkg::Target::BodyHit:

      this->gkg::MonoGlyph::pick( canvas, geometryAllocation, depth, hit );
      break;

    case gkg::Target::NeverHit:

      break;

    case gkg::Target::AlwaysHit:

      hit.setTarget( depth, this, 0 );
      break;

    case gkg::Target::PrimitiveHit:

      if ( ( hit.getRight() >= geometryAllocation.getLeft() ) &&
           ( hit.getLeft() < geometryAllocation.getRight() ) &&
	   ( hit.getTop() >= geometryAllocation.getBottom() ) &&
           ( hit.getBottom() < geometryAllocation.getTop() ) )
      {

        hit.setTarget( depth, this, 0 );

      }
      break;

    case gkg::Target::CharacterHit:

      // call to pick is dubious, but needed for now by doc
      this->gkg::MonoGlyph::pick( canvas, geometryAllocation, depth, hit );
      x = hit.getLeft();
      left = geometryAllocation.getLeft();
      right = geometryAllocation.getRight();
      if ( ( hit.getRight() >= left ) && ( x < right ) )
      {

        hit.setTarget( depth, this, ( x > ( ( left+right ) * 0.5 ) ) ? 1 : 0 );

      }
      break;

  }

}
