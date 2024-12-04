#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelFactory.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelAxialStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelCoronalStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelSagittalStrategy.h>


gkg::VoiLabelFactory::VoiLabelFactory()
{
}


gkg::VoiLabelFactory::~VoiLabelFactory()
{
}


gkg::VoiLabelStrategy* 
gkg::VoiLabelFactory::create( gkg::VoiStrategy::StgyType id )
{

  switch ( id )
  {
  
  case gkg::VoiStrategy::StgyAxial:
    return new gkg::VoiLabelAxialStrategy();
    break;
  case gkg::VoiStrategy::StgyCoronal:
    return new gkg::VoiLabelCoronalStrategy();
    break;
  case gkg::VoiStrategy::StgySagittal:
    return new gkg::VoiLabelSagittalStrategy();
    break;
  default:
    return (gkg::VoiLabelStrategy *)NULL;
    break;
    
  }

}
 
