#include <gkg-graphics-X11-viewer2d/Viewer2d.h>
#include <gkg-graphics-X11-viewer2d/VolumeModel.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/Raster.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/Image.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/InputHandler.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Transformer.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <cmath>
#include <cstdio>


template < class T >
gkg::Viewer2d< T >::Viewer2d( gkg::VolumeModel< T >* volumeModel,
                              gkg::Style* style )
                   : gkg::InputHandler( volumeModel->getPatch(),
                                        style ),
                     _volumeModel( volumeModel )
{

  try
  {

  _font = 
     new gkg::Font( "-*-courier-medium-r-normal--*-120-*-*-*-iso8859-*" );
  if ( _font )
  {

    _font->addReference();

  }
  _annotationColor = new gkg::Color( 1.0, 0.0, 0.0, 1.0f );
  if ( _annotationColor )
  {

    _annotationColor->addReference();

  }

  _state = gkg::Viewer2d< T >::Idle;
  _currentSlice = _volumeModel->getSizeZ() / 2;
  _deltaSlice = 0;
  _currentRank = 0;
  _deltaRank = 0;
  _deltaLowerThreshold = 0;
  _deltaUpperThreshold = 0;

  }
  GKG_CATCH( "template < class T > "
             "gkg::Viewer2d< T >::Viewer2d( "
             "gkg::VolumeModel< T >* volumeModel, "
             "gkg::Style* style )" );

}


template < class T >
gkg::Viewer2d< T >::Viewer2d( gkg::VolumeModel< T >* volumeModel )
                   : gkg::InputHandler( volumeModel->getPatch(),
                                        gkg::Session::getInstance().
                                              getDefaultDisplay()->getStyle() ),
                     _volumeModel( volumeModel )
{

  try
  {

  _font = 
     new gkg::Font( "-*-courier-medium-r-normal--*-120-*-*-*-iso8859-*" );
  if ( _font )
  {

    _font->addReference();

  }
  _annotationColor = new gkg::Color( 1.0, 0.0, 0.0, 1.0f );
  if ( _annotationColor )
  {

    _annotationColor->addReference();

  }

  _state = gkg::Viewer2d< T >::Idle;
  _currentSlice = _volumeModel->getSizeZ() / 2;
  _deltaSlice = 0;
  _currentRank = 0;
  _deltaRank = 0;
  _deltaLowerThreshold = 0;
  _deltaUpperThreshold = 0;

  }
  GKG_CATCH( "template < class T > "
             "gkg::Viewer2d< T >::Viewer2d( "
             "gkg::VolumeModel< T >* volumeModel, "
             "gkg::Style* style )" );

}


template < class T >
gkg::Viewer2d< T >::~Viewer2d()
{

  if ( _font )
  {

    _font->removeReference();

  }
  if ( _annotationColor )
  {

    _annotationColor->removeReference();

  }

}


template < class T >
void gkg::Viewer2d< T >::draw(
                      gkg::Canvas* canvas,
                      const gkg::GeometryAllocation& geometryAllocation ) const
{

  try
  {

    this->gkg::InputHandler::draw( canvas, geometryAllocation );

    //canvas->pushTransform();
    //gkg::Transformer identity;
    //canvas->setTransformer( identity );

    char tmp[ 64 ];

    sprintf( tmp, "window %g/%g",
             _volumeModel->getLowerThreshold() + _deltaLowerThreshold,
             _volumeModel->getUpperThreshold() + _deltaUpperThreshold ) ;
    canvas->string( _font, tmp, _annotationColor, 10, 5 );

    sprintf( tmp, "slice  %03d/%03d",
             _currentSlice + _deltaSlice + 1,
             _volumeModel->getSizeZ() ) ;
    canvas->string( _font, tmp, _annotationColor, 10, 20 );


    sprintf( tmp, "rank   %03d/%03d",
             _currentRank + _deltaRank + 1,
             _volumeModel->getSizeT() ) ;
    canvas->string( _font, tmp, _annotationColor, 10, 35 );

    //  canvas->popTransform();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Viewer2d< T >::draw( "
             "gkg::Canvas* canvas, "
             "const gkg::GeometryAllocation& geometryAllocation ) const" );

}


template < class T >
void gkg::Viewer2d< T >::update( const gkg::Volume< T >& volume )
{

  try
  {
  
    _volumeModel->update( volume, _currentSlice, _currentRank );
    _volumeModel->getPatch()->redraw();
    redraw();
 
  }
  GKG_CATCH( "template < class T > "
             "void gkg::Viewer2d< T >::update( "
             "const gkg::Volume< T >& volume )" );

}


template < class T >
void gkg::Viewer2d< T >::press( const gkg::Event& event )
{

  try
  {

    _x0 = event.getPointerX();
    _y0 = event.getPointerY();

    if ( _x0 > 32 && _x0 < 80 && _y0 > 20 && _y0 < 30 )
    {

      _state = gkg::Viewer2d< T >::SliceModification;
      //std::cout << "entering SliceModification state" << std::endl;

    }
    else if ( _x0 > 32 && _x0 < 80 && _y0 > 35 && _y0 < 45 )
    {

      _state = gkg::Viewer2d< T >::RankModification;
      //std::cout << "entering RankModification state" << std::endl;

    }
    else
    {

      _state = gkg::Viewer2d< T >::Windowing;
      //std::cout << "entering Windowing state" << std::endl;

    }

    this->gkg::InputHandler::press( event );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Viewer2d< T >::press( const gkg::Event& event )" );

}


template < class T >
void gkg::Viewer2d< T >::drag( const gkg::Event& event )
{

  try
  {

    _x2 = event.getPointerX();
    _y2 = event.getPointerY();

    if ( _state == gkg::Viewer2d< T >::SliceModification )
    {

      _deltaSlice = ( int32_t )( ( _x2 - _x0 ) / 20.0 );

      if ( _currentSlice + _deltaSlice > _volumeModel->getSizeZ() - 1 )
      {

        _deltaSlice = _volumeModel->getSizeZ() - 1 - _currentSlice;

      }
      else if ( _currentSlice + _deltaSlice < 0 )
      {

        _deltaSlice = -_currentSlice;

      }
      _volumeModel->fillRaster( _currentSlice + _deltaSlice, _currentRank );

    }
    else if ( _state == gkg::Viewer2d< T >::RankModification )
    {

      _deltaRank = ( int32_t )( ( _x2 - _x0 ) / 20.0 );

      if ( _currentRank + _deltaRank > _volumeModel->getSizeT() - 1 )
      {

        _deltaRank = _volumeModel->getSizeT() - 1 - _currentRank;

      }
      else if ( _currentRank + _deltaRank < 0 )
      {

        _deltaRank = -_currentRank;

      }
      _volumeModel->fillRaster( _currentSlice, _currentRank + _deltaRank );

    }
    else if ( _state == gkg::Viewer2d< T >::Windowing )
    {

      double oldLowerThreshold = _volumeModel->getLowerThreshold();
      double oldUpperThreshold = _volumeModel->getUpperThreshold();

      _deltaLowerThreshold = _x2 - _x0;
      _deltaUpperThreshold = _y2 - _y0;

      double newLowerThreshold = getNewThreshold( oldLowerThreshold,
                                                  _deltaLowerThreshold );
      double newUpperThreshold = getNewThreshold( oldUpperThreshold,
                                                  _deltaUpperThreshold );

      if ( newLowerThreshold < newUpperThreshold )
      {

        _volumeModel->setLowerThreshold( newLowerThreshold );
        _volumeModel->setUpperThreshold( newUpperThreshold );

      }
      _volumeModel->fillRaster( _currentSlice, _currentRank );
      if ( newLowerThreshold < newUpperThreshold )
      {

        _volumeModel->setLowerThreshold( oldLowerThreshold );
        _volumeModel->setUpperThreshold( oldUpperThreshold );

      }

    }
    _volumeModel->getPatch()->redraw();
    redraw();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Viewer2d< T >::drag( const gkg::Event& event )" );

}


template < class T >
void gkg::Viewer2d< T >::release( const gkg::Event& event )
{

  try
  {

    _x2 = event.getPointerX();
    _y2 = event.getPointerY();

    if ( _state == gkg::Viewer2d< T >::SliceModification )
    {

      if ( event.getPointerButton() == gkg::Event::WheelDown )
      {

        _deltaSlice = -1;

        if ( _currentSlice + _deltaSlice > _volumeModel->getSizeZ() - 1 )
        {

          _deltaSlice = _volumeModel->getSizeZ() - 1 - _currentSlice;

        }
        else if ( _currentSlice + _deltaSlice < 0 )
        {

          _deltaSlice = -_currentSlice;

        }
        _currentSlice += _deltaSlice;
        _deltaSlice = 0;
        _volumeModel->fillRaster( _currentSlice, _currentRank );
        //std::cout << "leaving SliceModification state" << std::endl;

      }
      else if ( event.getPointerButton() == gkg::Event::WheelUp )
      {

        _deltaSlice = +1;

        if ( _currentSlice + _deltaSlice > _volumeModel->getSizeZ() - 1 )
        {

          _deltaSlice = _volumeModel->getSizeZ() - 1 - _currentSlice;

        }
        else if ( _currentSlice + _deltaSlice < 0 )
        {

          _deltaSlice = -_currentSlice;

        }
        _currentSlice += _deltaSlice;
        _deltaSlice = 0;
        _volumeModel->fillRaster( _currentSlice, _currentRank );
        //std::cout << "leaving SliceModification state" << std::endl;

      }
      else
      {

        _deltaSlice = ( int32_t )( ( _x2 - _x0 ) / 20.0 );

        if ( _currentSlice + _deltaSlice > _volumeModel->getSizeZ() - 1 )
        {

          _deltaSlice = _volumeModel->getSizeZ() - 1 - _currentSlice;

        }
        else if ( _currentSlice + _deltaSlice < 0 )
        {

          _deltaSlice = -_currentSlice;

        }
        _currentSlice += _deltaSlice;
        _deltaSlice = 0;
       _volumeModel->fillRaster( _currentSlice, _currentRank );
       //std::cout << "leaving SliceModification state" << std::endl;

      }

    }
    else if ( _state == gkg::Viewer2d< T >::RankModification )
    {

      if ( event.getPointerButton() == gkg::Event::WheelDown )
      {

        _deltaRank = -1;

        if ( _currentRank + _deltaRank > _volumeModel->getSizeT() - 1 )
        {

          _deltaRank = _volumeModel->getSizeT() - 1 - _currentRank;

        }
        else if ( _currentRank + _deltaRank < 0 )
        {

          _deltaRank = -_currentRank;

        }
        _currentRank += _deltaRank;
        _deltaRank = 0;
        _volumeModel->fillRaster( _currentSlice, _currentRank );
        //std::cout << "leaving RankModification state" << std::endl;

      }
      else if ( event.getPointerButton() == gkg::Event::WheelUp )
      {

        _deltaRank = +1;

        if ( _currentRank + _deltaRank > _volumeModel->getSizeT() - 1 )
        {

          _deltaRank = _volumeModel->getSizeT() - 1 - _currentRank;

        }
        else if ( _currentRank + _deltaRank < 0 )
        {

          _deltaRank = -_currentRank;

        }
        _currentRank += _deltaRank;
        _deltaRank = 0;
        _volumeModel->fillRaster( _currentSlice, _currentRank );
        //std::cout << "leaving RankModification state" << std::endl;

      }
      else
      {


        _deltaRank = ( int32_t )( ( _x2 - _x0 ) / 20.0 );

        if ( _currentRank + _deltaRank > _volumeModel->getSizeT() - 1 )
        {

          _deltaRank = _volumeModel->getSizeT() - 1 - _currentRank;

        }
        else if ( _currentRank + _deltaRank < 0 )
        {

          _deltaRank = -_currentRank;

        }
        _currentRank += _deltaRank;
        _deltaRank = 0;
        _volumeModel->fillRaster( _currentSlice, _currentRank );
        //std::cout << "leaving RankModification state" << std::endl;

      }

    }
    else if ( _state == gkg::Viewer2d< T >::Windowing )
    {

      _deltaLowerThreshold = 0;
      _deltaUpperThreshold = 0;

      double newLowerThreshold = getNewThreshold(
                                   _volumeModel->getLowerThreshold(),
                                   _x2 - _x0 );
      double newUpperThreshold = getNewThreshold(
                                   _volumeModel->getUpperThreshold(),
                                   _y2 - _y0 );

      if ( newLowerThreshold < newUpperThreshold )
      {

        _volumeModel->setLowerThreshold( newLowerThreshold );
        _volumeModel->setUpperThreshold( newUpperThreshold );

      }
      _volumeModel->fillRaster( _currentSlice, _currentRank );
      //std::cout << "leaving Windowing state" << std::endl;

    }

    _volumeModel->getPatch()->redraw();
    _state = gkg::Viewer2d< T >::Idle;
    redraw();
    this->gkg::InputHandler::release( event );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Viewer2d< T >::release( const gkg::Event& event )" );

}


template < class T >
double gkg::Viewer2d< T >::getNewThreshold( double oldValue,
                                            double deltaValue ) const
{

  try
  {

    double a = std::log( 3.0 ) / 150.0;

    if ( ( deltaValue <= 100.0 && deltaValue >= 0.0 ) ||
         ( deltaValue < 0.0 && deltaValue >= -100.0 ) )
    {

      return oldValue + 0.2 * deltaValue;

    }

    if ( deltaValue < -100.0 )
    {

      if ( oldValue <= 20.0 )
      {

        return oldValue - 20.0 + 10.0 * ( deltaValue + 100.0 );

      }
      return std::exp( a * ( deltaValue + 100.0 ) +
                       std::log( oldValue - 20.0 ) );

    }

    if ( deltaValue > 100.0 )
    {

      if ( oldValue <= -20.0 )
      {

        return oldValue + 20.0 + 10.0 * ( deltaValue - 100.0 );

      }
      return std::exp( a * ( deltaValue - 100.0 ) +
                       std::log( oldValue + 20.0 ) );

    }

    return oldValue;

  }
  GKG_CATCH( "template < class T > "
             "double gkg::Viewer2d< T >::getNewThreshold( double oldValue, "
             "double deltaValue ) const" );

}


// specialization(s)


namespace gkg
{

template <>
double Viewer2d< RGBComponent >::getNewThreshold( double oldValue,
                                                  double deltaValue ) const
{

  try
  {

    double value = oldValue;
    if ( ( deltaValue > 0.0 ) && ( deltaValue <= 100.0 ) )
    {

      value = oldValue + 0.005 * deltaValue;

    }
    else if ( deltaValue > 100.0 )
    {

      value = oldValue + 0.5 + 0.02 * ( deltaValue - 100.0 );

    }
    else if ( ( deltaValue < 0.0 ) && ( deltaValue >= -100.0 ) )
    {

      value = oldValue + 0.005 * deltaValue;

    }
    else if ( deltaValue < -100.0 )
    {

      value = oldValue - 0.5 + 0.02 * ( deltaValue + 100.0 );

    }

    return value;

  }
  GKG_CATCH( "template <> "
             "double Viewer2d< RGBComponent >::getNewThreshold( "
             "double oldValue, "
             "double deltaValue ) const" );

}


}


//
// explicit instanciations
//

template class gkg::Viewer2d< uint8_t >;
template class gkg::Viewer2d< int8_t >;
template class gkg::Viewer2d< uint16_t >;
template class gkg::Viewer2d< int16_t >;
template class gkg::Viewer2d< uint32_t >;
template class gkg::Viewer2d< int32_t >;
template class gkg::Viewer2d< uint64_t >;
template class gkg::Viewer2d< int64_t >;
template class gkg::Viewer2d< float >;
template class gkg::Viewer2d< double >;
template class gkg::Viewer2d< gkg::RGBComponent >;


