#include <gkg-processing-io/HistogramAnalysisDiskFormat.h>
#include <gkg-processing-histogram/HistogramAnalysis_i.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>
#include <iostream>

gkg::HistogramAnalysisDiskFormat::HistogramAnalysisDiskFormat()
                              : gkg::TypedDiskFormat< gkg::HistogramAnalysis >()
{

  this->_extensions.insert( ".han" );

}

gkg::HistogramAnalysisDiskFormat::~HistogramAnalysisDiskFormat()
{
}


std::string gkg::HistogramAnalysisDiskFormat::getName() const
{

  return "histogram-analysis";

}

void gkg::HistogramAnalysisDiskFormat::readHeader(
                                             const std::string& name,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // building HAN file name
    std::string hanName = this->getNameWithoutExtension( name ) + ".han";

    // checking that HAN file can be read
    gkg::HistogramAnalysis histogramAnalysis;
    readHANFile( hanName, histogramAnalysis );
      
    // no exception was launched at this point, so file is OK and we know it is
    // a HistogramAnalysis
    if ( !object.getHeader().hasAttribute( "object_type" ) )
    {

      object.getHeader().addAttribute( "object_type",
                                       std::string( "HistogramAnalysis" ) );

    }

  }
  GKG_CATCH( "void gkg::HistogramAnalysisDiskFormat::readHeader("
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}

void gkg::HistogramAnalysisDiskFormat::writeHeader(
                                   const std::string& /*name*/,
                                   const gkg::HeaderedObject& /*object*/ ) const
{

  // does nothing special because HAN file does't have a meta header info file

}

void gkg::HistogramAnalysisDiskFormat::read(
                                          const std::string& name,
                                          gkg::HistogramAnalysis& object ) const
{

  try
  {

    // reading HAN header
    readHeader( name, object );

    // building HAN file name
    std::string hanName = this->getNameWithoutExtension( name ) + ".han";

    // reading HAN file
    readHANFile( hanName, object );

  }
  GKG_CATCH( "void gkg::HistogramAnalysisDiskFormat::read( "
             "const std::string& name, "
             "gkg::HistogramAnalysis& object ) const" );

}

void gkg::HistogramAnalysisDiskFormat::write( const std::string& name,
                                              gkg::HistogramAnalysis& object,
                                              bool ascii ) const
{

  try
  {
    
    
    // sanity check
    if ( !ascii )
    {
    
      throw std::runtime_error( "only ASCII mode is supported" );
    
    }
    
    // building HAN file name
    std::string hanName = this->getNameWithoutExtension( name ) + ".HAN";

    // writing file
    std::ofstream os( hanName.c_str() );

    // bachground    
    os << "background: mean: 1" << std::endl;

    // cerebro spinal fluid
    os << "csf: "
       << "mean: " << object.getCerebroSpinalFluidStatistics().mean << " "
       << "sigma: " <<object.getCerebroSpinalFluidStatistics().standardDeviation
       << std::endl;

    // gray matter
    os << "gray: "
       << "mean: " << object.getGrayMatterStatistics().mean << " "
       << "sigma: " << object.getGrayMatterStatistics().standardDeviation
       << std::endl;

    // white matter
    os << "white: "
       << "mean: " << object.getWhiteMatterStatistics().mean << " "
       << "sigma: " << object.getWhiteMatterStatistics().standardDeviation
       << std::endl;

    // necessar but unused
    os << "candidate 0: mean: -1 sigma: -1" << std::endl;
    os << "candidate 1: mean: -1 sigma: -1" << std::endl;
    os << "candidate 2: mean: -1 sigma: -1" << std::endl;
    os << "candidate 3: mean: -1 sigma: -1" << std::endl;
    os << "candidate 4: mean: -1 sigma: -1" << std::endl;
    os << "candidate 5: mean: -1 sigma: -1" << std::endl;
    os.close();

    
  }
  GKG_CATCH( "void gkg::HistogramAnalysisDiskFormat::write( "
             "const std::string& name, "
             "gkg::HistogramAnalysis& object, "
             "bool ascii ) const" );

}

bool gkg::HistogramAnalysisDiskFormat::hasReader() const
{

  return true;

}

bool gkg::HistogramAnalysisDiskFormat::hasWriter() const
{

  return true;

}

void gkg::HistogramAnalysisDiskFormat::readHANFile(
                                          const std::string& hanName,
                                          gkg::HistogramAnalysis& object ) const
{

  try
  {
  
  
    // opening stream for *.han file
    std::ifstream is( hanName.c_str() );

    // sanity check
    if ( !is )
    {
    
      throw gkg::FileNotFoundException( hanName );

    }
    
    // reading 
    char line[100];
    is.getline( line, 100 );

    std::string tmp;
    is >> tmp;
    while ( tmp != "gray:" )
    {
    
      is.getline( line, 100 );
      is >> tmp;
      
    }

    // gray matter
    if ( tmp != "gray:" )
    {
    
      throw std::runtime_error( "corrupted HAN file" );
    
    }

    is >> tmp;
    if ( tmp != "mean:" )
    {
    
      throw std::runtime_error( "corrupted HAN file" );
    
    }
    is >> object._grayMatterStatistics.mean;
    
    is >> tmp;
    if ( tmp != "sigma:" )
    {
    
      throw std::runtime_error( "corrupted HAN file" );
    
    }
    is >> object._grayMatterStatistics.standardDeviation;
    

    // white matter
    is >> tmp;
     if ( tmp != "white:" )
    {
    
      throw std::runtime_error( "corrupted HAN file" );
    
    }
    
    is >> tmp;
    if ( tmp != "mean:" )
    {
    
      throw std::runtime_error( "corrupted HAN file" );
    
    }
    is >> object._whiteMatterStatistics.mean;
    
    is >> tmp;
    if ( tmp != "sigma:" )
    {
    
      throw std::runtime_error( "corrupted HAN file" );
    
    }
    is >> object._whiteMatterStatistics.standardDeviation;   
        
    is >> tmp;
    if ( !is )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );

    }

    is.close();
    
  }
  GKG_CATCH( "void gkg::HistogramAnalysisDiskFormat::readHANFile("
             "const std::string& hanName,"
             "gkg::HistogramAnalysis& object ) const" );
	     
}

//
// registrating HistogramAnalysis disk format for "HistogramAnalysis"
// object factory
//

static bool initialize()
{


  try
  {

    gkg::DiskFormatFactory< gkg::HistogramAnalysis 
                          >::getInstance().registerDiskFormat(
         gkg::HistogramAnalysisDiskFormat::getInstance().getName(),
         &gkg::HistogramAnalysisDiskFormat::getInstance() );


    return true;

  }
  GKG_CATCH( "HistogramAnalysis disk format registration" );

}


static bool initialized __attribute__((unused)) = initialize();















    
