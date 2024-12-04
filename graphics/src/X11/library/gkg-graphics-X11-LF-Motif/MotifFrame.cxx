#include <gkg-graphics-X11-LF-Motif/MotifFrame.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-graphics-X11-core/Bevel.h>


gkg::MotifFrame::MotifFrame( gkg::Glyph* glyph,
                             gkg::TellTaleState* tellTaleState,
                             gkg::MotifInfo* motifInfo,
                             gkg::FloatCoordinate thickness,
                             bool target,
                             bool choosable )
                : gkg::BevelFrame( glyph, thickness ),
                  _tellTaleState( tellTaleState ),
                  _motifInfo( motifInfo ),
                  _target( target ),
                  _choosable( choosable )
{

  if ( _tellTaleState )
  {

    _tellTaleState->addReference();

  }
  if ( _motifInfo )
  {

    _motifInfo->addReference();

  }

}


gkg::MotifFrame::~MotifFrame()
{

  if ( _tellTaleState )
  {

    _tellTaleState->removeReference();

  }
  if ( _motifInfo )
  {

    _motifInfo->removeReference();

  }

}


const gkg::MotifInfo& gkg::MotifFrame::getMotifInfo() const
{

  return *_motifInfo;

}


void 
gkg::MotifFrame::draw( gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  this->gkg::BevelFrame::draw( canvas, geometryAllocation );
  if ( !_tellTaleState->test( gkg::TellTaleState::Enabled ) )
  {

    canvas->fillRectangle( geometryAllocation.getLeft(),
                           geometryAllocation.getBottom(),
                           geometryAllocation.getRight(),
                           geometryAllocation.getTop(),
                           _motifInfo->getGrayOut() );

  }

}


void gkg::MotifFrame::pick( gkg::Canvas* canvas,
                            const gkg::GeometryAllocation& geometryAllocation,
                            int32_t depth,
                            gkg::Hit& hit )
{

  if ( _target )
  {

    gkg::FloatCoordinate x = hit.getLeft();
    gkg::FloatCoordinate y = hit.getBottom();
    if ( ( x >= geometryAllocation.getLeft() ) &&
         ( x < geometryAllocation.getRight() ) &&
         ( y >= geometryAllocation.getBottom() ) &&
         ( y < geometryAllocation.getTop() ) )
    {

      hit.setTarget( depth, this, 0 );

    }

  }
  else
  {

    this->gkg::BevelFrame::pick( canvas, geometryAllocation, depth, hit );

  }

}


void 
gkg::MotifFrame::drawFrame( gkg::Canvas* canvas,
                            const gkg::GeometryAllocation& geometryAllocation,
                            gkg::FloatCoordinate thickness ) const
{

  const gkg::Color* c1;
  const gkg::Color* c2;
  const gkg::Color* c3;
  c2 = _motifInfo->getFlat();
  if ( _tellTaleState->test( gkg::TellTaleState::EnabledActive ) )
  {

    c1 = _motifInfo->getLight();
    c3 = _motifInfo->getDark();

  }
  else if ( _choosable &&
            _tellTaleState->test( gkg::TellTaleState::EnabledChosen ) )
  {

    c1 = _motifInfo->getDark();
    c3 = _motifInfo->getLight();

  }
  else
  {

    c1 = c2;
    c3 = c2;

  }
  gkg::Bevel::drawRectangle( canvas,
                             c1, c2, c3,
                             thickness,
                             geometryAllocation.getLeft(),
                             geometryAllocation.getBottom(),
                             geometryAllocation.getRight(),
                             geometryAllocation.getTop() );

}
