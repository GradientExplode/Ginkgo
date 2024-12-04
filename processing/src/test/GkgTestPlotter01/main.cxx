#include <gkg-processing-plot/CurvePlotter.h>
#include <gkg-processing-plot/PlotterFactory.h>
#include <gkg-communication-sysinfo/KeyboardHit.h>
#include <cmath>
#include <complex>
#include <iostream>
#include <cstdlib>


int main()
{


  // example of GnuPlot plotter use with multiple real data set(s)

  gkg::CurvePlotter gnuPlotCurvePlotter1;

  std::vector< double > data1( 30 ), data2( 30 );
  for ( size_t i = 0; i < data1.size(); i++ )
  {

    data1[ i ] = ( ( double )i / 2 ) * ( ( double )i / 2 ) *
                 std::log( ( double )i + 1.0 );
    data2[ i ] = 10.0 * ( double )i;

  }

  std::list< std::vector< double > > data;
  data.push_back( data1 );
  data.push_back( data2 );

  gnuPlotCurvePlotter1.plot( data, "abscisses", "ordonnees", "curve 1" );

  // example of GnuPlot plotter use with one real data set
  gkg::CurvePlotter gnuPlotCurvePlotter2;

  std::vector< std::complex< double > > data3( 30 );
  for ( size_t i = 0; i < data3.size(); i++ )
  {

    data3[ i ] = std::complex< double >(
                               ( ( double )i / 2 ) * ( ( double )i / 2 ) *
                               std::log( ( double )i + 1.0 ),
                               10.0 * ( double )i );

  }
  gnuPlotCurvePlotter2.plot( data3, "cplx_abs", "cplx_ord", "curve 2" );


  // now we select Postscript plotter with Plotter factory
  gkg::PlotterFactory::getInstance().select( "PostscriptPlot" );

  // we can customize first coordinate
  std::vector< double > x( 30 );
  for ( size_t i = 0; i < data1.size(); i++ )
  {

    x[ i ] = 2.0 * ( ( double )i / 30 ) * ( ( double )i / 30 );

  }

  // example of Postscript plotter use with multiple real data set(s)
  gkg::CurvePlotter postscriptCurvePlotter1;
  postscriptCurvePlotter1.plot( x, data, "abscisses", "ordonnees", "curve 1" );


  // example of Postscript plotter use with one real data set(s)
  gkg::CurvePlotter postscriptCurvePlotter2;
  postscriptCurvePlotter2.plot( x, data3, "cplx_abs", "cplx_ord", "curve 2" );


  gkg::keyboardHit();

  return EXIT_SUCCESS;

}
