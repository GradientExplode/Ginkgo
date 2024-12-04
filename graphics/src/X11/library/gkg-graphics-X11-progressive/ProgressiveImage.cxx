#include <gkg-graphics-X11-progressive/ProgressiveImage.h>
#include <gkg-graphics-X11-progressive/ProgressiveModel.h>
#include <gkg-graphics-X11-progressive/ProgressiveImageContext.h>
#include <gkg-graphics-X11-progressive/ProgressiveThread.h>
#include <gkg-graphics-X11-core/Raster.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/Image.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <gkg-communication-dispatcher/IOCallback.h>
#include <gkg-communication-dispatcher/Dispatcher.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <iostream>


gkg::ProgressiveImage::ProgressiveImage( 
                                        gkg::ProgressiveModel* progressiveModel,
                                        int32_t resolutionDivider,
                                        bool verbose )
                      : gkg::MonoGlyph(),
                        gkg::Observer(),
                        _progressiveModel( progressiveModel ),
                        _resolutionDivider( resolutionDivider ),
                        _needsRedraw( false ),
                        _verbose( verbose )
{

  _raster = new gkg::Raster( _progressiveModel->getPixelWidth(),
                             _progressiveModel->getPixelHeight() );
  gkg::PixelCoordinate x, y;
  for ( y = 0; y < _progressiveModel->getPixelHeight(); y++ )
  {

    for ( x = 0; x < _progressiveModel->getPixelWidth(); x++ )
    {

      _raster->poke( x, y, 0, 0, 0, 1 );

    }

  }

  _progressiveImageContext = new gkg::ProgressiveImageContext( this );
  _progressiveThread = new gkg::ProgressiveThread( this, _verbose );
  _handler = new gkg::IOCallback< gkg::ProgressiveImage >( this,
                                     &gkg::ProgressiveImage::redraw );

  _progressiveModel->addReference();
  _raster->addReference();

  _imagePatch = new gkg::Patch( new gkg::Image( _raster ) );
  setBody( _imagePatch );

  gkg::Dispatcher::getInstance().startTimer( 0, 100000, _handler );

  update();

}


gkg::ProgressiveImage::~ProgressiveImage()
{

  std::cerr << ">>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<" << std::endl;
  gkg::Dispatcher::getInstance().stopTimer( _handler );
  delete _handler;
  delete _progressiveThread;
  delete _progressiveImageContext;
  _raster->removeReference();
  _progressiveModel->removeReference();

}


gkg::ProgressiveModel* gkg::ProgressiveImage::getProgressiveModel() const
{

  return _progressiveModel;

}


int32_t gkg::ProgressiveImage::getResolutionDivider() const
{

  return _resolutionDivider;

}


bool gkg::ProgressiveImage::hasFullResolution() const
{

  return _progressiveImageContext->getStepCount() ==
          ( _resolutionDivider * _resolutionDivider );

}


gkg::ProgressiveImageContext*
gkg::ProgressiveImage::getProgressiveImageContext() const
{

  return _progressiveImageContext;

}


void gkg::ProgressiveImage::resetProgressiveImageContext()
{

  _progressiveImageContext->resetStepCount();
  _mutex.unlock();

}


void gkg::ProgressiveImage::updatePatch()
{

  _progressiveImageContext->fill( _raster );
  _needsRedraw = true;
  _progressiveImageContext->increaseStepCount();

  if ( _verbose )
  {

    std::cerr << "step " << _progressiveImageContext->getStepCount()
              << std::endl;

  }

}


void gkg::ProgressiveImage::redraw( int64_t, int64_t )
{

  _mutex.lock();
  if ( _imagePatch && _imagePatch->getCanvas() && _needsRedraw )
  {

    _imagePatch->redraw();
    _needsRedraw = false;

  }
  gkg::Dispatcher::getInstance().startTimer( 0, 100000, _handler );
  _mutex.unlock();

}

void gkg::ProgressiveImage::update( const Observable* /*observable*/,
                                    void* /*data*/ )
{

  _progressiveThread->launch();

}
