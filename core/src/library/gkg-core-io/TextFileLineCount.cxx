#include <gkg-core-io/TextFileLineCount.h>
#include <gkg-core-exception/Exception.h>


#include <fstream>
#include <sstream>
#include <list>



int32_t gkg::getTextFileLineCount( const std::string& fileName )
{

  try
  {

    std::ifstream is( fileName.c_str() );

    if ( !is )
    {
    
      throw std::runtime_error( std::string("unable to open '" ) + 
                                fileName + "'" );

    }

    std::string line;
    std::string lineWithoutSpaceAndTab;
    int32_t lineCount = 0;
    while ( std::getline( is, line ) )
    {

      lineWithoutSpaceAndTab = "";
      int32_t index = 0;
      for ( index = 0; index < ( int32_t )line.size(); index++ )
      {

        if ( ( line[ index ] != ' ' ) &&
             ( line[ index ] != '\t' ) )
        {

          lineWithoutSpaceAndTab += line[ index ];

        }

      }

      if ( lineWithoutSpaceAndTab.size() )
      {

        ++ lineCount;

      }

    }
    is.close();

    return lineCount;

  }
  GKG_CATCH( "int32_t gkg::getTextFileLineCount( "
             "const std::string& fileName )" );

}


std::vector< int32_t > 
gkg::getScalarItemCountPerLine( const std::string& fileName )
{

  try
  {


    std::list< int32_t > elementCounts;

    std::ifstream is( fileName.c_str() );

    if ( !is )
    {
    
      throw std::runtime_error( std::string("unable to open '" ) + 
                                fileName + "'" );

    }

    std::string line;
    std::string lineWithoutSpaceAndTab;
    while ( std::getline( is, line ) )
    {

      lineWithoutSpaceAndTab = "";
      int32_t index = 0;

      for ( index = 0; index < ( int32_t )line.size(); index++ )
      {

        if ( ( line[ index ] != ' ' ) &&
             ( line[ index ] != '\t' ) )
        {

          lineWithoutSpaceAndTab += line[ index ];

        }

      }

      if ( lineWithoutSpaceAndTab.size() )
      {

        int32_t elementCount = 0;
        double value = 0.0;
        std::istringstream iss( line );
        while ( iss >> value )
        {

          ++ elementCount;

        }

        elementCounts.push_back( elementCount );

      }
      else
      {

        elementCounts.push_back( 0 );

      }

    }
    is.close();

    return std::vector< int32_t >( elementCounts.begin(), elementCounts.end() );

  }
  GKG_CATCH( "std::vector< int32_t > "
             "gkg::getScalarItemCountPerLine( const std::string& fileName )" );

}


std::vector< int32_t > 
gkg::getStringItemCountPerLine( const std::string& fileName )
{

  try
  {


    std::list< int32_t > elementCounts;

    std::ifstream is( fileName.c_str() );

    if ( !is )
    {
    
      throw std::runtime_error( std::string("unable to open '" ) + 
                                fileName + "'" );

    }

    std::string line;
    std::string lineWithoutSpaceAndTab;
    while ( std::getline( is, line ) )
    {

      lineWithoutSpaceAndTab = "";
      int32_t index = 0;

      for ( index = 0; index < ( int32_t )line.size(); index++ )
      {

        if ( ( line[ index ] != ' ' ) &&
             ( line[ index ] != '\t' ) )
        {

          lineWithoutSpaceAndTab += line[ index ];

        }

      }

      if ( lineWithoutSpaceAndTab.size() )
      {

        int32_t elementCount = 0;
        std::string value;
        std::istringstream iss( line );
        while ( iss >> value )
        {

          ++ elementCount;

        }

        elementCounts.push_back( elementCount );

      }
      else
      {

        elementCounts.push_back( 0 );

      }

    }
    is.close();

    return std::vector< int32_t >( elementCounts.begin(), elementCounts.end() );

  }
  GKG_CATCH( "std::vector< int32_t > "
             "gkg::getStringrItemCountPerLine( const std::string& fileName )" );

}
