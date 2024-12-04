#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeFactory.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/SelectionStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/PaintStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/EvolStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/FillStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/PickColorStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/StatisticStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/SplineStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/MorphologyStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DistanceStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/CylinderStrategy.h>


gkg::VoiModeFactory::VoiModeFactory()
{
}


gkg::VoiModeFactory::~VoiModeFactory()
{
}


gkg::VoiModeStrategy*
gkg::VoiModeFactory::create( gkg::VoiModeStrategy::ModeId id )
{

  switch ( id )
    {
    
    case gkg::VoiModeStrategy::SelectionMode:
      return new gkg::VoiSelectionStrategy();
      break;
    case gkg::VoiModeStrategy::PaintMode:
      return new gkg::VoiPaintStrategy();
      break;
    case gkg::VoiModeStrategy::EvolutionMode:
      return new gkg::VoiEvolutionStrategy();
      break;
    case gkg::VoiModeStrategy::FillingMode:
      return new gkg::VoiFillStrategy();
      break;
    case gkg::VoiModeStrategy::PickingColorMode:
      return new gkg::VoiPickColorStrategy();
      break;
    case gkg::VoiModeStrategy::StatisticMode:
      return new gkg::VoiStatisticStrategy();
      break;
    case gkg::VoiModeStrategy::SplineMode:
      return new gkg::VoiSplineStrategy();
      break;
    case gkg::VoiModeStrategy::MorphologyMode:
      return new gkg::VoiMorphologyStrategy();
      break;
    case gkg::VoiModeStrategy::DistanceMode:
      return new gkg::VoiDistanceStrategy();
      break;
    case gkg::VoiModeStrategy::CylinderMode:
      return new gkg::VoiCylinderStrategy();
      break;
    default:
      return (gkg::VoiModeStrategy *)NULL;
      break;

    }

}
