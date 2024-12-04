#include <gkg-graphics-X11-progressive/ProgressiveImageContext.h>
#include <gkg-graphics-X11-progressive/ProgressiveImage.h>
#include <gkg-graphics-X11-progressive/ProgressiveModel.h>
#include <gkg-graphics-X11-core/Raster.h>
#include <cstring>
#include <algorithm>


gkg::ProgressiveImageContext::ProgressiveImageContext(
                                       gkg::ProgressiveImage* progressiveImage )
                             : gkg::LoopContext< int32_t >(),
                               _progressiveImage( progressiveImage ),
                               _stepCount( 0 ),
                               _offsetX( 0 ),
                               _offsetY( 0 ),
                               _zoom( 0 )
{

  gkg::PixelCoordinate
    pixelWidth = _progressiveImage->getProgressiveModel()->getPixelWidth();
  gkg::PixelCoordinate
    pixelHeight = _progressiveImage->getProgressiveModel()->getPixelHeight();
  int32_t resolutionDivider = _progressiveImage->getResolutionDivider();

  int32_t composeSize = pixelWidth * pixelHeight;
                   
  int32_t stepSizeX = pixelWidth / resolutionDivider +
                      ( pixelWidth % resolutionDivider ? 1 : 0 );

  int32_t stepSizeY = pixelHeight / resolutionDivider +
                      ( pixelHeight % resolutionDivider ? 1 : 0 );

  int32_t stepSize = stepSizeX * stepSizeY;

  int32_t c;
  for ( c = 0; c < gkg::ProgressiveImageContext::CanalCount; c++ )
  {

    _composeBuffer[ c ] = new gkg::FloatColorIntensity[ composeSize ];
    _stepBuffer[ c ] = new gkg::FloatColorIntensity[ stepSize ];

  }
  _composeAlphaBuffer = new float[ composeSize ];
  _stepAlphaBuffer = new float[ stepSize ];

  updatePaddingInformation();

}


gkg::ProgressiveImageContext::~ProgressiveImageContext()
{

  int32_t c;
  for ( c = 0; c < gkg::ProgressiveImageContext::CanalCount; c++ )
  {

    delete [] _composeBuffer[ c ];
    delete [] _stepBuffer[ c ];

  }
  delete [] _composeAlphaBuffer;
  delete [] _stepAlphaBuffer;

}


int32_t gkg::ProgressiveImageContext::getStepCount() const
{

  return _stepCount;

}


void gkg::ProgressiveImageContext::increaseStepCount()
{

  ++ _stepCount;
  updatePaddingInformation();

}


void gkg::ProgressiveImageContext::resetStepCount()
{

  _stepCount = 0;
  updatePaddingInformation();

}


int32_t gkg::ProgressiveImageContext::getOffsetX() const
{

  return _offsetX;

}


int32_t gkg::ProgressiveImageContext::getOffsetY() const
{

  return _offsetY;

}


int32_t gkg::ProgressiveImageContext::getZoom() const
{

  return _zoom;

}


void gkg::ProgressiveImageContext::doIt( int32_t startIndex, int32_t count )
{

  gkg::PixelCoordinate
    pixelWidth = _progressiveImage->getProgressiveModel()->getPixelWidth();
  gkg::PixelCoordinate
    pixelHeight = _progressiveImage->getProgressiveModel()->getPixelHeight();
  int32_t resolutionDivider = _progressiveImage->getResolutionDivider();

  int32_t stepSizeX = pixelWidth / resolutionDivider +
                      ( pixelWidth % resolutionDivider ? 1 : 0 );

  int32_t p = startIndex * stepSizeX;
  int32_t x, y;
  for ( y = startIndex; y < startIndex + count; y++ )
  {

    for ( x = 0; x < stepSizeX; x++, p++ )
    {

      int32_t xOrig = _offsetX + x * resolutionDivider;
      int32_t yOrig = _offsetY + y * resolutionDivider;

      if ( ( xOrig < pixelWidth ) && ( yOrig < pixelHeight ) )
      {

        _progressiveImage->getProgressiveModel()->getValueAt(
                     xOrig, yOrig,
                     _stepBuffer[ gkg::ProgressiveImageContext::Red ][ p ], 
                     _stepBuffer[ gkg::ProgressiveImageContext::Green ][ p ], 
                     _stepBuffer[ gkg::ProgressiveImageContext::Blue ][ p ], 
                     _stepAlphaBuffer[ p ] );

      }

    } 

  }

}


void gkg::ProgressiveImageContext::fill( gkg::Raster* raster )
{

  pad();

  gkg::PixelCoordinate
    pixelWidth = _progressiveImage->getProgressiveModel()->getPixelWidth();
  gkg::PixelCoordinate
    pixelHeight = _progressiveImage->getProgressiveModel()->getPixelHeight();

  int32_t p = 0;
  gkg::PixelCoordinate x, y;
  for ( y = 0; y < pixelHeight; y++ ) 
  {

    for ( x = 0; x < pixelWidth; x++, p++ )
    {

      raster->poke( x, y,
                    _composeBuffer[ gkg::ProgressiveImageContext::Red ][ p ],
                    _composeBuffer[ gkg::ProgressiveImageContext::Green ][ p ],
                    _composeBuffer[ gkg::ProgressiveImageContext::Blue ][ p ],
                    _composeAlphaBuffer[ p ] );

    }

  }

}


void gkg::ProgressiveImageContext::updatePaddingInformation()
{

  getPaddingInformation( _stepCount,
                         0, 0, _progressiveImage->getResolutionDivider(),
                         _offsetX, _offsetY, _zoom );

}


void gkg::ProgressiveImageContext::pad()
{

  gkg::PixelCoordinate
    pixelWidth = _progressiveImage->getProgressiveModel()->getPixelWidth();
  gkg::PixelCoordinate
    pixelHeight = _progressiveImage->getProgressiveModel()->getPixelHeight();
  int32_t resolutionDivider = _progressiveImage->getResolutionDivider();

  int32_t c;
  for ( c = 0; c < gkg::ProgressiveImageContext::CanalCount; c++ )
  {

    composeZoomedBuffer( _composeBuffer[ c ],
                         _offsetX, _offsetY, _zoom,
                         pixelWidth, pixelHeight, 
                         _stepBuffer[ c ],
                         resolutionDivider );

  }
  composeZoomedBuffer( _composeAlphaBuffer,
                       _offsetX, _offsetY, _zoom,
                       pixelWidth, pixelHeight, 
                       _stepAlphaBuffer,
                       resolutionDivider );

}


void gkg::ProgressiveImageContext::getPaddingInformation(
                          int32_t stepCount, int32_t x, int32_t y, int32_t zoom,
                          int32_t &offsetX, int32_t& offsetY, int32_t& rzoom )
{

  if ( ( zoom == 1 ) || ( stepCount == 0 ) )
  {

    offsetX = x;
    offsetY = y;
    rzoom = zoom;

  }
  else
  {

    switch( stepCount & 3 )
    {

      case 0:

        getPaddingInformation( stepCount / 4,
                               x, y, zoom / 2,
                               offsetX, offsetY, rzoom );
        break;

      case 1:

        getPaddingInformation( stepCount / 4,
                               x + zoom / 2, y + zoom / 2, zoom / 2,
                               offsetX, offsetY, rzoom );
        break;

      case 2:

        getPaddingInformation( stepCount / 4,
                               x + zoom / 2, y, zoom / 2,
                               offsetX, offsetY, rzoom );
        break;

      case 3:

        getPaddingInformation( stepCount / 4,
                               x, y + zoom / 2, zoom / 2,
                               offsetX, offsetY, rzoom );
        break;

    }

  }

}


template < class T >
void gkg::ProgressiveImageContext::composeZoomedBuffer(
                                     T* to,
                                     int32_t offsetX, int32_t offsetY, 
                                     int32_t zoom,
                                     gkg::PixelCoordinate toPixelWidth,
                                     gkg::PixelCoordinate toPixelHeight,
                                     T* from,
                                     int32_t resolutionDivider )
{

  int32_t fromPixelWidth = toPixelWidth / resolutionDivider +
                           ( toPixelWidth % resolutionDivider ? 1 : 0 );
  int32_t fromPixelHeight = toPixelHeight / resolutionDivider +
                           ( toPixelHeight % resolutionDivider ? 1 : 0 );

  if ( zoom == resolutionDivider )
  {

    memcpy( to, from, fromPixelWidth * fromPixelHeight * sizeof( T ) );

    while ( ( fromPixelWidth < toPixelWidth ) ||
              ( fromPixelHeight < toPixelHeight ) )
    {

      zoom2( to, fromPixelWidth, fromPixelHeight, toPixelWidth, toPixelHeight );
      fromPixelWidth *= 2;
      fromPixelHeight *= 2;

    }

// old code without smoothing (big rectangles)
//    int destCol, destLine;
//    for ( destLine = 0; destLine < toPixelHeight; destLine++ )
//      for ( destCol = 0; destCol < toPixelWidth; destCol++ )
//        to[ destCol + destLine * toPixelWidth ] =
//          from[ destCol / resolutionDivider + 
//                ( destLine / resolutionDivider ) * fromPixelWidth ];
    return;

  }
  switch ( zoom )
  {

    case 1:

      {

        int32_t sourceCol, sourceLine, destCol, destLine;

        for ( sourceLine = 0, destLine = offsetY; destLine < toPixelHeight;
              destLine += resolutionDivider, sourceLine++ )
        {

          for ( sourceCol = 0, destCol = offsetX; destCol < toPixelWidth;
                sourceCol++, destCol += resolutionDivider )
          {

            to[ destCol + destLine * toPixelWidth ] =
              from[ sourceCol + sourceLine * fromPixelWidth ];

          }

        }

      }
      break;

    case 2:

      {

        int32_t sourceCol, sourceLine, destCol, destLine;

        for ( sourceLine = 0, destLine = offsetY; destLine < toPixelHeight;
              destLine += resolutionDivider, sourceLine++ )
        {

          for ( sourceCol = 0, destCol = offsetX; destCol < toPixelWidth;
                sourceCol++, destCol += resolutionDivider )
          {

             to[ destCol + destLine * toPixelWidth ] =
               from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( destCol + 1 < toPixelWidth )
               to[ destCol + 1 + destLine * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( destLine + 1 < toPixelHeight )
               to[ destCol + ( destLine + 1 ) * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( ( destCol + 1 < toPixelWidth ) &&
                  ( destLine + 1 < toPixelHeight ) )
               to[ destCol + 1 + ( destLine + 1 )* toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];

          }

        }

      }
      break;

    case 4:

      {

        int32_t sourceCol, sourceLine, destCol, destLine;

        for ( sourceLine = 0, destLine = offsetY; destLine < toPixelHeight;
              destLine += resolutionDivider, sourceLine++ )
        {

          for ( sourceCol = 0, destCol = offsetX; destCol < toPixelWidth;
                sourceCol++, destCol += resolutionDivider )
          {

             to[ destCol + destLine * toPixelWidth ] =
               from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( destLine + 1 < toPixelHeight )
               to[ destCol + ( destLine + 1 ) * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( destLine + 2 < toPixelHeight )
               to[ destCol + ( destLine + 2 ) * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( destLine + 3 < toPixelHeight )
               to[ destCol + ( destLine + 3 ) * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( destCol + 1 < toPixelWidth )
               to[ destCol + 1 + destLine * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( ( destCol + 1 < toPixelWidth ) &&
                  ( destLine + 1 < toPixelHeight ) )
               to[ destCol + 1 + ( destLine + 1 ) * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( ( destCol + 1 < toPixelWidth ) &&
                  ( destLine + 2 < toPixelHeight ) )
               to[ destCol + 1 + ( destLine + 2 ) * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( ( destCol + 1 < toPixelWidth ) &&
                  ( destLine + 3 < toPixelHeight ) )
               to[ destCol + 1 + ( destLine + 3 ) * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( destCol + 2 < toPixelWidth )
               to[ destCol + 2 + destLine * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( ( destCol + 2 < toPixelWidth ) &&
                  ( destLine + 1 < toPixelHeight ) )
               to[ destCol + 2 + ( destLine + 1 ) * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( ( destCol + 2 < toPixelWidth ) &&
                  ( destLine + 2 < toPixelHeight ) )
               to[ destCol + 2 + ( destLine + 2 ) * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( ( destCol + 2 < toPixelWidth ) &&
                  ( destLine + 3 < toPixelHeight ) )
               to[ destCol + 2 + ( destLine + 3 ) * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( destCol + 3 < toPixelWidth )
               to[ destCol + 3 + destLine * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( ( destCol + 3 < toPixelWidth ) &&
                  ( destLine + 1 < toPixelHeight ) )
               to[ destCol + 3 + ( destLine + 1 ) * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( ( destCol + 3 < toPixelWidth ) &&
                  ( destLine + 2 < toPixelHeight ) )
               to[ destCol + 3 + ( destLine + 2 ) * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];
             if ( ( destCol + 3 < toPixelWidth ) &&
                  ( destLine + 3 < toPixelHeight ) )
               to[ destCol + 3 + ( destLine + 3 ) * toPixelWidth ] =
                 from[ sourceCol + sourceLine * fromPixelWidth ];

          }

        }

      }
      break;

    default:

      composeZoomedBuffer( to, offsetX, offsetY, zoom / 2,
                           toPixelWidth, toPixelHeight,
                           from, resolutionDivider );
      composeZoomedBuffer( to, offsetX + zoom / 2, offsetY, zoom / 2,
                           toPixelWidth, toPixelHeight,
                           from, resolutionDivider );
      composeZoomedBuffer( to, offsetX, offsetY + zoom / 2, zoom / 2,
                           toPixelWidth, toPixelHeight,
                           from, resolutionDivider );
      composeZoomedBuffer( to, offsetX + zoom / 2, offsetY + zoom / 2, zoom / 2,
                           toPixelWidth, toPixelHeight,
                           from, resolutionDivider );


  }

}


template < class T >
void gkg::ProgressiveImageContext::zoom2( T* to,
                                          gkg::PixelCoordinate fromPixelWidth,
                                          gkg::PixelCoordinate fromPixelHeight,
                                          gkg::PixelCoordinate toPixelWidth,
                                          gkg::PixelCoordinate toPixelHeight )
{

  int32_t doubleFromPixelWidth = 2 * fromPixelWidth;
  int32_t doubleFromPixelHeight = 2 * fromPixelHeight;

  int32_t destPixelWidth = std::min( doubleFromPixelWidth, toPixelWidth );
  int32_t destPixelHeight = std::min( doubleFromPixelHeight, toPixelHeight );

  int32_t sourceCol, sourceLine, destCol, destLine;
  T v1, v2;

  // dilate lines
  for ( sourceLine = fromPixelHeight - 1, destLine = doubleFromPixelHeight - 2;
        destLine >= 0; sourceLine --, destLine -= 2 )
  {

    if ( destLine < toPixelHeight )
    {

      v2 = to[ fromPixelWidth - 1 + sourceLine * fromPixelWidth ];
      for ( sourceCol = fromPixelWidth - 1, destCol = doubleFromPixelWidth - 2;
            destCol >= 0; sourceCol--, destCol -= 2, v2 = v1 )
      {

        v1 = to[ sourceCol + sourceLine * fromPixelWidth ];
        to[ destCol + destLine * destPixelWidth ] = v1;
        if ( destCol < toPixelWidth - 1 )
        {

          to[ destCol + 1 + destLine * destPixelWidth ] = ( v1 + v2 ) / 2;

        }

      }

    }

  }

  // interpolate between lines
  for ( destLine = 1; destLine < destPixelHeight - 1; destLine += 2 )
  {

    for ( destCol = 0; destCol < destPixelWidth; destCol ++ )
    {

      to[ destCol + destLine * destPixelWidth ] =
        ( to[ destCol + ( destLine - 1 ) * destPixelWidth ] +
          to[ destCol + ( destLine + 1 ) * destPixelWidth ] ) / 2;

    }

  }          

}
