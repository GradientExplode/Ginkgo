#include <gkg-processing-plugin-functors/DicomAnalyzer/DicomSender.h>
#include <gkg-processing-plugin-functors/DicomAnalyzer/DicomAnalyzer.h>
#include <gkg-communication-tcpip/Host.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-cppext/StdInt.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>


//
// class DicomSender
//


gkg::DicomSender::DicomSender( const gkg::DicomSeries& series )
                : _series( series )
{
}


gkg::DicomSender::~DicomSender()
{
}



void gkg::DicomSender::send( const std::string& outputDirectory ) const
{

  try
  {

    // building destination directory
    char examSeriesSubDirectory[ 1024 ];
    sprintf( examSeriesSubDirectory, "Exam%s_Series%06d",
             _series.getExamString().c_str(),
             _series.getSeriesId() );

    std::string destinationDirectory =
                                     outputDirectory + '/' +
                                     _series.getInstitutionName() + '/' +
                                     _series.getManufacturersModelName() + '/' +
                                     _series.getDate() + '/' +
                                     _series.getExamString() + '/' +
                                     examSeriesSubDirectory + "_" +
                                     _series.getSeriesDescription();
    std::string command = "mkdir -p " + destinationDirectory;
    int32_t status = ::system( command.c_str() );
    
    if ( status < 0 )
    {
    
      throw std::runtime_error( "Error executing mkdir command" );
    
    }

    // copying files
    int32_t index;
    for ( index = 0; index < _series.getCount(); index++ )
    {

      command = "cp -f " + _series.getOriginName( index ) + " " +
                destinationDirectory + "/" +
                _series.getDestinationName( index );
      status = ::system( command.c_str() );
      
      if ( status )
      {
      
        throw std::runtime_error( "Error executing cp command" );
      
      }

    }

  }
  GKG_CATCH( "void gkg::DicomSender::send( "
             "const std::string& outputDirectory ) const" );

}
