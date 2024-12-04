#include <gkg-processing-plot/CurvePlotter.h>
#include <gkg-processing-plot/CurvePlotterImplementation.h>
#include <gkg-processing-plot/PlotterFactory.h>
#include <gkg-core-io/StringConverter.h>
#include <cmath>
#include <iostream>


gkg::CurvePlotter::CurvePlotter()
{

  _cpi = gkg::PlotterFactory::getInstance().createCurvePlotterImplementation();

  _originX = 8 * _cpi->getCharacterWidth() + _cpi->getTicWidth();
  _originY = 4 * _cpi->getCharacterHeight() + _cpi->getTicHeight();

  _width = _cpi->getWidth() - 8 * _cpi->getTicWidth() - _originX;
  _height = _cpi->getHeight() - 3 * _cpi->getCharacterHeight() - _originY;

  _arrowWidth = _cpi->getTicWidth();
  _arrowHeight = _cpi->getTicHeight();

}


gkg::CurvePlotter::~CurvePlotter()
{

  delete _cpi;

}


bool gkg::CurvePlotter::plot( const std::vector< double >& x,
                              const std::list< std::vector< double > >& yList,
                              const std::string& xTitle,
                              const std::string& yTitle,
                              const std::string& title ) const
{

  // sanity checks
  std::list< std::vector< double > >::const_iterator y = yList.begin(),
                                                     ye = yList.end();
  while ( y != ye )
  {

    if ( y->size() != x.size() )
    {

      std::cerr << "gkg::CurvePlotter::plot: x and y don't have same size(s)"
                << std::endl;
      return false;

    }
    y ++;

  }

  // determining minimum and maximum values through curves
  y = yList.begin();
  ye = yList.end();
  double yMinimum = ( *y )[ 0 ];
  double yMaximum = ( *y )[ 0 ];
  while ( y != ye )
  {

    std::vector< double >::const_iterator v = y->begin(),
                                          ve = y->end();
    while ( v != ve )
    {

      if ( *v < yMinimum )
      {

        yMinimum = *v;

      }
      if ( *v > yMaximum )
      {

        yMaximum = *v;

      }
      v ++;

    }
    y ++;

  }

  if ( std::fabs( yMaximum - yMinimum ) < 1e-8 )
  {

    yMaximum = yMinimum + 1.0;

  }

  // determining minimum and maximum x
  double xMinimum = x[ 0 ];
  double xMaximum = x[ 0 ];
  std::vector< double >::const_iterator i = x.begin(),
                                        ie = x.end();
  while ( i != ie )
  {

    if ( *i < xMinimum )
    {

      xMinimum = *i;

    }
    if ( *i > xMaximum )
    {

      xMaximum = *i;

    }
    i ++;

  }

  if ( std::fabs( xMaximum - xMinimum ) < 1e-8 )
  {

    xMaximum = xMinimum + 1.0;

  }

  // really plotting things
  _cpi->begin( title );
  plotAxis( xMinimum, xMaximum, yMinimum, yMaximum, xTitle, yTitle, title );

  double r = _width / ( xMaximum - xMinimum );
  double s = _height / ( yMaximum - yMinimum );

  y = yList.begin();
  ye = yList.end();
  int32_t count = 0;
  while ( y != ye )
  {

    _cpi->setLineType( ( gkg::CurvePlotterImplementation::LineType )
                       ( gkg::CurvePlotterImplementation::RunLine1 + count ) );
    _cpi->move( _originX + r * ( x[ 0 ] - xMinimum ),
                _originY + s * ( ( *y )[ 0 ] - yMinimum ) );

    std::vector< double >::const_iterator vx = x.begin(),
                                          vxe = x.end();
    std::vector< double >::const_iterator vy = y->begin();

    while ( vx != vxe )
    {

      _cpi->drawVector( _originX + r * ( *vx - xMinimum ),
                        _originY + s * ( *vy - yMinimum ) );
      vx ++;
      vy ++;

    }
    y ++;
    count ++;

  }
  _cpi->end();

  return true;

}


bool gkg::CurvePlotter::plot( const std::vector< double >& x,
                              const std::vector< double >& y,
                              const std::string& xTitle,
                              const std::string& yTitle,
                              const std::string& title ) const
{

  std::list< std::vector< double > > yList;
  yList.push_back( y );
  return plot( x, yList, xTitle, yTitle, title );

}


bool gkg::CurvePlotter::plot( const std::list< std::vector< double > >& yList,
                              const std::string& xTitle,
                              const std::string& yTitle,
                              const std::string& title ) const
{

  std::vector< double > x( yList.front().size() );
  for ( size_t i = 0; i < x.size(); i++ )
  {

    x[ i ] = ( double )i;

  }
  return plot( x, yList, xTitle, yTitle, title );

}


bool gkg::CurvePlotter::plot( const std::vector< double >& y,
                              const std::string& xTitle,
                              const std::string& yTitle,
                              const std::string& title ) const
{

  std::list< std::vector< double > > yList;
  yList.push_back( y );
  return plot( yList, xTitle, yTitle, title );

}


bool gkg::CurvePlotter::plot(
                const std::vector< double >& x,
                const std::list< std::vector< std::complex< double > > >& yList,
                const std::string& xTitle,
                const std::string& yTitle,
                const std::string& title ) const
{

  // sanity checks
  std::list< std::vector< std::complex< double > > >::const_iterator
                                                     y = yList.begin(),
                                                     ye = yList.end();
  while ( y != ye )
  {

    if ( y->size() != x.size() )
    {

      std::cerr << "gkg::CurvePlotter::plot: x and y don't have same size(s)"
                << std::endl;
      return false;

    }
    y ++;

  }

  // determining minimum and maximum values through curves
  y = yList.begin();
  ye = yList.end();
  double yMinimum = std::real( ( *y )[ 0 ] );
  double yMaximum = std::real( ( *y )[ 0 ] );
  while ( y != ye )
  {

    std::vector< std::complex< double > >::const_iterator v = y->begin(),
                                                          ve = y->end();
    while ( v != ve )
    {

      if ( std::real( *v ) < yMinimum )
      {

        yMinimum = std::real( *v );

      }
      if ( std::real( *v ) > yMaximum )
      {

        yMaximum = std::real( *v );

      }
      if ( std::imag( *v ) < yMinimum )
      {

        yMinimum = std::imag( *v );

      }
      if ( std::imag( *v ) > yMaximum )
      {

        yMaximum = std::imag( *v );

      }
      v ++;

    }
    y ++;

  }

  if ( std::fabs( yMaximum - yMinimum ) < 1e-8 )
  {

    yMaximum = yMinimum + 1.0;

  }

  // determining minimum and maximum x
  double xMinimum = x[ 0 ];
  double xMaximum = x[ 0 ];
  std::vector< double >::const_iterator i = x.begin(),
                                        ie = x.end();
  while ( i != ie )
  {

    if ( *i < xMinimum )
    {

      xMinimum = *i;

    }
    if ( *i > xMaximum )
    {

      xMaximum = *i;

    }
    i ++;

  }

  if ( std::fabs( xMaximum - xMinimum ) < 1e-8 )
  {

    xMaximum = xMinimum + 1.0;

  }

  // really plotting things
  _cpi->begin( title );
  plotAxis( xMinimum, xMaximum, yMinimum, yMaximum, xTitle, yTitle, title );

  double r = _width / ( xMaximum - xMinimum );
  double s = _height / ( yMaximum - yMinimum );

  y = yList.begin();
  ye = yList.end();
  int32_t count = 0;
  while ( y != ye )
  {

    // plotting real part
    _cpi->setLineType( ( gkg::CurvePlotterImplementation::LineType )
                       ( gkg::CurvePlotterImplementation::RunLine1 + count ) );
    _cpi->move( _originX + r * ( x[ 0 ] - xMinimum ),
                _originY + s * ( std::real( ( *y )[ 0 ] ) - yMinimum ) );

    std::vector< double >::const_iterator vx = x.begin(),
                                          vxe = x.end();
    std::vector< std::complex< double > >::const_iterator vy = y->begin();
    while ( vx != vxe )
    {

      _cpi->drawVector( _originX + r * ( *vx - xMinimum ),
                        _originY + s * ( std::real( *vy ) - yMinimum ) );
      vx ++;
      vy ++;

    }
    count ++;

    // plotting imaginary part
    _cpi->setLineType( ( gkg::CurvePlotterImplementation::LineType )
                       ( gkg::CurvePlotterImplementation::RunLine1 + count ) );
    _cpi->move( _originX + r * ( x[ 0 ] - xMinimum ),
                _originY + s * ( std::imag( ( *y )[ 0 ] ) - yMinimum ) );

    vx = x.begin();
    vxe = x.end();
    vy = y->begin();
    while ( vx != vxe )
    {

      _cpi->drawVector( _originX + r * ( *vx - xMinimum ),
                        _originY + s * ( std::imag( *vy ) - yMinimum ) );
      vx ++;
      vy ++;

    }
    count ++;

    y ++;

  }
  _cpi->end();

  return true;

}


bool gkg::CurvePlotter::plot( const std::vector< double >& x,
                              const std::vector< std::complex< double > >& y,
                              const std::string& xTitle,
                              const std::string& yTitle,
                              const std::string& title ) const
{

  std::list< std::vector< std::complex< double > > > yList;
  yList.push_back( y );
  return plot( x, yList, xTitle, yTitle, title );

}


bool gkg::CurvePlotter::plot(
                const std::list< std::vector< std::complex< double > > >& yList,
                const std::string& xTitle,
                const std::string& yTitle,
                const std::string& title ) const
{

  std::vector< double > x( yList.front().size() );
  for ( size_t i = 0; i < x.size(); i++ )
  {

    x[ i ] = ( double )i;

  }
  return plot( x, yList, xTitle, yTitle, title );

}


bool gkg::CurvePlotter::plot( const std::vector< std::complex< double > >& y,
                              const std::string& xTitle,
                              const std::string& yTitle,
                              const std::string& title ) const
{

  std::list< std::vector< std::complex< double > > > yList;
  yList.push_back( y );
  return plot( yList, xTitle, yTitle, title );

}


void gkg::CurvePlotter::plotAxis( double xMinimum, double xMaximum,
                                  double yMinimum, double yMaximum,
                                  const std::string& xTitle,
                                  const std::string& yTitle,
                                  const std::string& title ) const
{

  _cpi->setLineType( gkg::CurvePlotterImplementation::BorderLine );

  // drawing axis
  _cpi->move( _originX,
              _originY );
  _cpi->drawArrow( _originX,
                   _originY,
                   _originX + _width + _arrowWidth,
                   _originY,
                   true );
  _cpi->move( _originX,
              _originY );
  _cpi->drawArrow( _originX,
                   _originY,
                   _originX,
                   _originY + _height + _arrowHeight,
                   true );
  _cpi->setRightJustify();
  _cpi->drawText( _originX + _width + _arrowWidth,
                  _originY - 2.5 * _cpi->getCharacterHeight(),
                  xTitle );
  _cpi->setLeftJustify();
  _cpi->drawText( _originX / 2,
                  _originY + _height + _cpi->getCharacterHeight() +
                  _arrowHeight,
                  yTitle );

  // drawing x ticks
  int32_t tickCount = 4;
  double posTick = _originX;
  double posChar = posTick;
  double value = xMinimum;
  double deltaPos = _width / ( double )tickCount;
  double deltaValue = ( xMaximum - xMinimum ) / ( double )tickCount;
  _cpi->setCenterJustify();
  int32_t i;
  for ( i = 0; i <= tickCount; i++ )
  {

    _cpi->move( posTick, _originY );
    _cpi->drawVector( posTick,
                      _originY - _cpi->getTicHeight() );
    _cpi->drawText( posChar,
                    _originY - _cpi->getCharacterHeight() -
                    _cpi->getTicHeight(),
                    gkg::StringConverter::toString( value, 'f', 5, 2 ) );
    value += deltaValue;
    posTick += deltaPos;
    posChar += deltaPos;

  }

  // drawing y ticks
  tickCount = 4;
  posTick = _originY;
  posChar = posTick;
  value = yMinimum;
  deltaPos = _height / ( double )tickCount;
  deltaValue = ( yMaximum - yMinimum ) / ( double )tickCount;
  _cpi->setRightJustify();
  for ( i = 0; i <= tickCount; i++ )
  {

    _cpi->move( _originX, posTick );
    _cpi->drawVector( _originX - _cpi->getTicWidth(),
                      posTick );
    _cpi->drawText( _originX - _cpi->getTicWidth(),
                    posChar,
                    gkg::StringConverter::toString( value, 'f', 5, 2 ) );
    value += deltaValue;
    posTick += deltaPos;
    posChar += deltaPos;

  }

  // drawing title
  if ( !title.empty() )
  {

    _cpi->setCenterJustify();
    _cpi->drawText( _cpi->getWidth()  / 2,
                    _originY + _height + _cpi->getCharacterHeight() +
                    _arrowHeight,
                    title );

  }

}


