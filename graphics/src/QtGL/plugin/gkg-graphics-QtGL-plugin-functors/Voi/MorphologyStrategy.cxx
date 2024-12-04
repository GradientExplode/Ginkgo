#include <gkg-graphics-QtGL-plugin-functors/Voi/MorphologyStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-morphology/Closing.h>
#include <gkg-processing-morphology/Opening.h>
#include <gkg-processing-morphology/Dilation.h>
#include <gkg-processing-morphology/Erosion.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>


gkg::VoiMorphologyStrategy::VoiMorphologyStrategy()
                          : gkg::VoiModeStrategy()
{
}


gkg::VoiModeStrategy::ModeId gkg::VoiMorphologyStrategy::id()
{ 

  return gkg::VoiModeStrategy::MorphologyMode;
  
}


void gkg::VoiMorphologyStrategy::pressed( gkg::VoiDrawA& /* da */,
                                          gkg::VoiData& d, 
                                          gkg::Vector3d< int32_t >& p, 
                                          int32_t t, QPoint& /* qp */,
                                          Qt::KeyboardModifiers )
{

  if ( d.dataLoaded() )
  {

    gkg::Volume< int16_t > roiLabel;
    int32_t lab = (int32_t)d.label()( p );
    gkg::EqualToFunction< int16_t > testFunction( lab );
    double radius = d.getMorphologyRadius();
    int operation = d.getMorphologyOperation();

    switch ( operation )
    {

      case 1:
        {

          gkg::Erosion< int16_t, int16_t > erosion( radius, 
                                                    testFunction, 
                                                    lab,
                                                    0 );
          erosion.erode( d.label(), roiLabel );
          break;

        }

      case 2:
        {

          gkg::Dilation< int16_t, int16_t > dilation( radius, 
                                                      testFunction, 
                                                      lab,
                                                      0 );
          dilation.dilate( d.label(), roiLabel );
          break;

        }

      case 3:
        {

          gkg::Opening< int16_t, int16_t > opening( radius, 
                                                    testFunction, 
                                                    lab,
                                                    0 );
          opening.open( d.label(), roiLabel );
          break;

        }

      case 4:
        {

          gkg::Closing< int16_t, int16_t > closing( radius, 
                                                    testFunction, 
                                                    lab,
                                                    0 );
          closing.close( d.label(), roiLabel );
          break;

        }

    }

    d.morphologyLabel( roiLabel, lab );

  }

  d.notifySlice( p, t );

}


void gkg::VoiMorphologyStrategy::setIndicator( QPainter* painter, QPoint& pos )
{

  indicator.draw( painter, pos );

}
