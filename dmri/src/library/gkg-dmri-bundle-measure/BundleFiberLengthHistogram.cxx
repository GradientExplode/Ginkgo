#include <gkg-dmri-bundle-measure/BundleFiberLengthHistogram.h>
#include <gkg-dmri-bundle-measure/BundleMeasure_i.h>
#include <gkg-dmri-fiber-measure/FiberMeasure_i.h>
#include <gkg-dmri-bundle-measure/BundleMeasureChecker.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


gkg::BundleFiberLengthHistogram::BundleFiberLengthHistogram()
                                : gkg::BundleMeasure< std::list< float > >()

{
}


gkg::BundleFiberLengthHistogram::~BundleFiberLengthHistogram()
{
}


std::list< float > 
gkg::BundleFiberLengthHistogram::get(
                   const gkg::BundleFiberLengthHistogram::Bundle& bundle ) const
{

  try
  {

    // initializing statistics

    // processing fiber lengths
    int32_t fiberCount = bundle.getCurve3dCount();
    std::list< float > fiberLengths;

    int32_t f = 0;
    for ( f = 0; f < fiberCount; f++ )
    {

      fiberLengths.push_back( _fiberLength.get( bundle.getCurve3d( f ) ) );

    }

    return fiberLengths;

  }
  GKG_CATCH( "std::list< float > "
             "gkg::BundleFiberLengthHistogram::get("
             "const gkg::BundleFiberLengthHistogram::Bundle& bundle ) const" );

}


template < class L, class Compare >
void gkg::BundleFiberLengthHistogram::spreadSheetFunctor(
            const std::vector< std::string >& stringParameters,
            const std::vector< double >& scalarParameters,
            gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap,
            std::map< L,
                      std::map< std::string, gkg::Matrix >,
                      Compare >& measures )
{

  try
  {

    // collecting verbosity
    bool verbose = ( ( ( int32_t )( scalarParameters[ 0 ] + 0.5 ) ) ?
                     true : false );


    // collecting output filename for python histograms and numbe rof bins
    std::string fileNamePython = stringParameters[ 0 ];
    int32_t binCount = ( int32_t )( scalarParameters[ 1 ] + 0.5 );

    if ( verbose )
    {

      std::cout << "looping over bundle(s) : "
                << std::flush;

    }
    gkg::BundleFiberLengthHistogram bundleFiberLengthHistogram;

    typename gkg::BundleMap< L, Compare >::const_iterator
      b = bundleMap->begin(),
      be = bundleMap->end();
    while ( b != be )
    {

      const L& label = b->first;
      const typename gkg::BundleMap< L, Compare >::Bundle& bundle = b->second;

      if ( verbose )
      {

        std::cout << gkg::StringConverter::toString( label ) << " "
                  << std::flush;

      }

      std::list< float >
        fiberLengths = bundleFiberLengthHistogram.get( bundle );
      float minimumLength = +1e38;
      float maximumLength = 0.0;
      int32_t fiberCount = ( int32_t )fiberLengths.size();

      gkg::Matrix&
          measure = measures[ label ][ "bundle_fiber_length_histogram" ];
      measure.reallocate( 1, fiberCount );
      int32_t f = 0;
      std::list< float >::const_iterator
        l = fiberLengths.begin(),
        le = fiberLengths.end();
      while ( l != le )
      {

        measure( 0, f ) = *l;
        if ( *l < minimumLength )
        {

          minimumLength = *l;

        }
        else if ( *l > maximumLength )
        {

          maximumLength = *l;

        }

        ++ l;
        ++ f;

      }

      // then storing into a python file ready to display the histogram
      std::string fileNamePythonForBundle = 
                                      fileNamePython + "_" +
                                      gkg::StringConverter::toString( label ) +
                                     ".py";
      std::ofstream os( fileNamePythonForBundle.c_str() );


      os << "#!/usr/bin/env python" << std::endl;
      os << "import numpy as np" << std::endl;
      os << "import matplotlib.mlab as mlab" << std::endl;
      os << "import matplotlib.pyplot as plt" << std::endl;
      os << std::endl;
      os << "x = [ " << std::flush;
      l = fiberLengths.begin();
      while ( l != le )
      {

        os << *l;

        ++ l;

        if ( l != le )
        {

          os << ", ";

        }
        ++ f;

      }
      os << " ]" << std::endl;

      os << "# the histogram of the data" << std::endl;
      os << "n, bins, patches = plt.hist( x, " << binCount
         << ", density=1, facecolor='green', " \
            "alpha=0.75, cumulative = True )" << std::endl;
      os << std::endl;
      os << "plt.xlabel('Length')" << std::endl;
      os << "plt.ylabel('Probability')" << std::endl;
      os << "plt.title( 'Histogram of lengths' )" << std::endl;
      os << "#plt.axis([" << minimumLength << ", " 
         << maximumLength << ", 0, 1.0])" << std::endl;
      os << "plt.grid(True)" << std::endl;
      os << std::endl;
      os << "plt.show()" << std::endl;
      os << std::endl;
      os.close();

      ++ b;

    }


    if ( verbose )
    {

      std::cout << std::endl;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "void gkg::BundleFiberLengthHistogram::spreadSheetFunctor( "
             "const std::vector< std::string >& /*stringParameters*/, "
             "const std::vector< double >& /*scalarParameters*/, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap, "
             "std::map< L, "
             "std::map< std::string, gkg::Matrix >, "
             "Compare >& measures )" );

}


template 
void gkg::BundleFiberLengthHistogram::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template 
void gkg::BundleFiberLengthHistogram::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

//
// adding check information to BundleFiberLengthHistogram
//

static bool initializeBundleFiberLengthHistogramChecker()
{

  try
  {

    gkg::BundleMeasureChecker::getInstance().addChecker(
      "bundle_fiber_length_histogram",
      ".  <P1>: output python hitogram filename",
      ".  <P1>: verbosity (1->true, 0-> false)\n"
      ".  <P2>: histogram bin count\n",
      1,
      2,
      0 );

    return true;

  }
  GKG_CATCH( "static bool initializeBundleFiberLengthHistogramChecker()" );

}


static bool initialized __attribute__((unused)) =
                                 initializeBundleFiberLengthHistogramChecker();
