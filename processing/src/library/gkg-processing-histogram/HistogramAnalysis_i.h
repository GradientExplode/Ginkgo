#ifndef _gkg_processing_histogram_HistogramAnalysis_i_h_
#define _gkg_processing_histogram_HistogramAnalysis_i_h_


#include <gkg-processing-histogram/HistogramAnalysis.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-exception/Exception.h>


template < class T >
inline
gkg::HistogramAnalysis::HistogramAnalysis( const gkg::Volume< T >& volume )
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "HistogramAnalysis" ) );

    // reading histogram analysis dictionary in header  
    gkg::Dictionary histogramAnalysisDictionary;
    if ( volume.getHeader().hasAttribute( "histogram_analysis" ) )
    {

      volume.getHeader().getAttribute( "histogram_analysis",
	  			      histogramAnalysisDictionary );

    }
    else
    {

      throw std::runtime_error(
                               "'histogram_analysis' attribute was not found" );

    }

    gkg::Dictionary grayMatterStatisticsDictionary;
    gkg::Dictionary whiteMatterStatisticsDictionary;
    gkg::Dictionary cerebroSpinalFluidStatisticsDictionary;


    // white matter
    if ( histogramAnalysisDictionary.find( "white_matter" ) !=
         histogramAnalysisDictionary.end() )
    {

      whiteMatterStatisticsDictionary = histogramAnalysisDictionary[ 
                                  "white_matter" ]->getValue<gkg::Dictionary>();

    }
    else
    {

      throw std::runtime_error( "'white_matter' attribute not found" );

    }

    // gray matter
    if ( histogramAnalysisDictionary.find( "gray_matter" ) !=
         histogramAnalysisDictionary.end() )
    {

      grayMatterStatisticsDictionary = histogramAnalysisDictionary[ 
                                 "gray_matter" ]->getValue< gkg::Dictionary >();

    }
    else
    {

      throw std::runtime_error( "'gray_matter' attribute not found" );

    }

    // cerebro spinal fluid
    if ( histogramAnalysisDictionary.find( "cerebro_spinal_fluid" ) !=
         histogramAnalysisDictionary.end() )
    {

      cerebroSpinalFluidStatisticsDictionary = histogramAnalysisDictionary[
                          "cerebro_spinal_fluid" ]->getValue<gkg::Dictionary>();

    }
    else
    {

      throw std::runtime_error( "'cerebro_spinal_fluid' attribute not found" );

    }

    // white matter
    if ( whiteMatterStatisticsDictionary.find( "mean" ) !=
         whiteMatterStatisticsDictionary.end() )
    {

      _whiteMatterStatistics.mean = whiteMatterStatisticsDictionary[
                                                          "mean" ]->getScalar();

    }
    else
    {

      throw std::runtime_error( "'mean' attribute not found" );

    }
    if ( whiteMatterStatisticsDictionary.find( "standard_deviation" ) !=
         whiteMatterStatisticsDictionary.end() )
    {

      _whiteMatterStatistics.standardDeviation =
           whiteMatterStatisticsDictionary[ "standard_deviation" ]->getScalar();

    }
    else
    {

      throw std::runtime_error( "'standard_deviation' attribute not found" );

    }

    // gray matter
    if ( grayMatterStatisticsDictionary.find( "mean" ) !=
         grayMatterStatisticsDictionary.end() )
    {

      _grayMatterStatistics.mean = grayMatterStatisticsDictionary[
                                                          "mean" ]->getScalar();

    }
    else
    {

      throw std::runtime_error( "'mean' attribute not found" );

    }
    if ( grayMatterStatisticsDictionary.find( "standard_deviation" ) !=
         grayMatterStatisticsDictionary.end() )
    {

      _grayMatterStatistics.standardDeviation = grayMatterStatisticsDictionary[
                                            "standard_deviation" ]->getScalar();

    }
    else
    {

      throw std::runtime_error( "'standard_deviation' attribute not found" );

    }

    // cerebro spinal fluid matter
    if ( cerebroSpinalFluidStatisticsDictionary.find( "mean" ) !=
         cerebroSpinalFluidStatisticsDictionary.end() )
    {

      _cerebroSpinalFluidStatistics.mean =
                          cerebroSpinalFluidStatisticsDictionary[
                                                          "mean" ]->getScalar();

    }
    else
    {

      throw std::runtime_error( "'mean' attribute not found" );

    }
    if ( cerebroSpinalFluidStatisticsDictionary.find( "standard_deviation" ) !=
         cerebroSpinalFluidStatisticsDictionary.end() )
    {

      _cerebroSpinalFluidStatistics.standardDeviation =
                          cerebroSpinalFluidStatisticsDictionary[
                                            "standard_deviation" ]->getScalar();

    }
    else
    {

      throw std::runtime_error( "'standard_deviation' attribute not found" );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::HistogramAnalysis::HistogramAnalysis( "
             "const gkg::Volume< T >& volume )" );

}


#endif
