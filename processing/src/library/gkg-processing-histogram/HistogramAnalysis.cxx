#include <gkg-processing-histogram/HistogramAnalysis_i.h>


//
// struct gkg::HistogramAnalysis::Statistics
//

gkg::HistogramAnalysis::Statistics::Statistics( double theMean,
                                                double theStandardDeviation )
                       : mean( theMean ),
                         standardDeviation( theStandardDeviation )
{
}


gkg::HistogramAnalysis::Statistics::Statistics(
                               const gkg::HistogramAnalysis::Statistics& other )
                       : mean( other.mean ),
                         standardDeviation( other.standardDeviation )
{
}


gkg::HistogramAnalysis::Statistics&
gkg::HistogramAnalysis::Statistics::operator=(
                               const gkg::HistogramAnalysis::Statistics& other )
{

  try
  {

    mean = other.mean;
    standardDeviation = other.standardDeviation;

    return *this;

  }
  GKG_CATCH( "gkg::HistogramAnalysis::Statistics::operator=( "
             "const gkg::HistogramAnalysis::Statistics& other )" );


}




//
// class gkg::HistogramAnalysis
//

gkg::HistogramAnalysis::HistogramAnalysis()
                       : gkg::HeaderedObject()
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
                   gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "HistogramAnalysis" ) );

  }
  GKG_CATCH( "gkg::HistogramAnalysis::HistogramAnalysis()" );
  
}

gkg::HistogramAnalysis::HistogramAnalysis(
        const gkg::HistogramAnalysis::Statistics& whiteMatterStatistics,
        const gkg::HistogramAnalysis::Statistics& grayMatterStatistics,
        const gkg::HistogramAnalysis::Statistics& cerebroSpinalFluidStatistics )
                       : gkg::HeaderedObject(),
		         _whiteMatterStatistics( whiteMatterStatistics ),
			 _grayMatterStatistics( grayMatterStatistics ),
			 _cerebroSpinalFluidStatistics(
                                                  cerebroSpinalFluidStatistics )
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "HistogramAnalysis" ) );

  }
  GKG_CATCH( "gkg::HistogramAnalysis::HistogramAnalysis( "
             "const gkg::HistogramAnalysis::Statistics& whiteMatterStatistics, "
             "const gkg::HistogramAnalysis::Statistics& grayMatterStatistics, "
             "const gkg::HistogramAnalysis::Statistics& "
             "cerebroSpinalFluidStatistics )" );
  
}


gkg::HistogramAnalysis::~HistogramAnalysis()
{
}


gkg::HistogramAnalysis&
gkg::HistogramAnalysis::operator=( const gkg::HistogramAnalysis& other )
{

  try
  {

    this->gkg::HeaderedObject::operator=( other );
    
    _whiteMatterStatistics = other._whiteMatterStatistics;
    _grayMatterStatistics = other._grayMatterStatistics;
    _cerebroSpinalFluidStatistics = other._cerebroSpinalFluidStatistics;

    return *this;

  }
  GKG_CATCH( "gkg::HistogramAnalysis&"
             "gkg::HistogramAnalysis::operator=( "
             "const gkg::HistogramAnalysis& other )" );

}


const gkg::HistogramAnalysis::Statistics& 
gkg::HistogramAnalysis::getWhiteMatterStatistics() const
{

  return _whiteMatterStatistics;

}


const gkg::HistogramAnalysis::Statistics& 
gkg::HistogramAnalysis::getGrayMatterStatistics() const
{

  return _grayMatterStatistics;

}


const gkg::HistogramAnalysis::Statistics& 
gkg::HistogramAnalysis::getCerebroSpinalFluidStatistics() const
{

  return _cerebroSpinalFluidStatistics;

}


void 
gkg::HistogramAnalysis::addToHeaderedObject( gkg::HeaderedObject& other ) const
{

  try
  {
  
    gkg::HistogramAnalysis::addSemantic( other );
    

    // white matter
    gkg::Dictionary whiteMatterStatisticsDictionary;
    whiteMatterStatisticsDictionary[ "mean" ] =
      _whiteMatterStatistics.mean;
    whiteMatterStatisticsDictionary[ "standard_deviation" ] =
      _whiteMatterStatistics.standardDeviation;

    // gray matter
    gkg::Dictionary grayMatterStatisticsDictionary;
    grayMatterStatisticsDictionary[ "mean" ] =
      _grayMatterStatistics.mean;
    grayMatterStatisticsDictionary[ "standard_deviation" ] =
      _grayMatterStatistics.standardDeviation;

    // cerebro spinal fluid
    gkg::Dictionary cerebroSpinalFluidStatisticsDictionary;
    cerebroSpinalFluidStatisticsDictionary[ "mean" ] =
      _cerebroSpinalFluidStatistics.mean;
    cerebroSpinalFluidStatisticsDictionary[ "standard_deviation" ] =
      _cerebroSpinalFluidStatistics.standardDeviation;

    // histogram analysis dictionary
    gkg::Dictionary histogramAnalysisDictionary;
    histogramAnalysisDictionary[ "white_matter" ] =
      whiteMatterStatisticsDictionary;
    histogramAnalysisDictionary[ "gray_matter" ] =
      grayMatterStatisticsDictionary; 
    histogramAnalysisDictionary[ "cerebro_spinal_fluid" ] =
      cerebroSpinalFluidStatisticsDictionary;
    
    other.getHeader().addAttribute( "histogram_analysis",
                                    histogramAnalysisDictionary );
    
  }
  GKG_CATCH( "void" 
             "gkg::HistogramAnalysis::addToHeaderedObject("
             "HeaderedObject& other ) const" );
	      
}


void gkg::HistogramAnalysis::addSemantic( gkg::HeaderedObject& other )
{

  try
  {
   
    other.addSemantic( "__histogram_analysis__",
                       "histogram_analysis",
		       gkg::Semantic(
		         gkg::TypeOf< gkg::Dictionary >::getName() ) );

  }
  GKG_CATCH( "void gkg::HistogramAnalysis::addSemantic(" 
             "gkg::HeaderedObject& other )" );

}

