#include <gkg-graphics-X11-progressive/ProgressiveModel.h>


gkg::ProgressiveModel::ProgressiveModel( gkg::PixelCoordinate pixelWidth,
                                         gkg::PixelCoordinate pixelHeight )
                      : gkg::RCGUIObject(),
                        _pixelWidth( pixelWidth ),
                        _pixelHeight( pixelHeight )
{
}


gkg::ProgressiveModel::~ProgressiveModel()
{
}


gkg::PixelCoordinate gkg::ProgressiveModel::getPixelWidth() const
{

  return _pixelWidth;

}


gkg::PixelCoordinate gkg::ProgressiveModel::getPixelHeight() const
{

  return _pixelHeight;

}

