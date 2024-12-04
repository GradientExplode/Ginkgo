#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/HierarchicalDensityBasedSpatialClustering_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<cstdint>


void loadCsv( int32_t numberOfValues, bool skipHeader,
              std::vector< std::vector< double > >& dataset )
{

  try
  {

    std::string  attribute;

    std::string line = "";

    int32_t currentAttributes;

    std::string fileName = "FourProminentClusterDataset.csv";
    std::ifstream file( fileName, std::ios::in );
    if ( !file )
    {

      throw std::runtime_error( "unable to open CSV file" );

    }
    if ( skipHeader )
    {

      getline( file, line );

    }
    while ( getline( file, line ) )
    {

      // reading through each line
      std::stringstream s( line );

      std::vector< double > row;
      currentAttributes = numberOfValues;

      while ( getline( s, attribute, ',' ) && currentAttributes != 0 )
      {

        row.push_back( std::stod( attribute ) );
        -- currentAttributes;

      }
      dataset.push_back( row );

    }

  }
  GKG_CATCH( "void loadCsv( int32_t numberOfValues, bool skipHeader, "
             "std::vector< std::vector< double > >& dataset )" );

}


namespace gkg
{


template <>
class DistanceOperator< std::vector< double > >
{

  public:

    
    DistanceOperator()
    {
    }

    double get( const std::vector< double >& item1,
                const std::vector< double >& item2 ) const
    {

      return std::sqrt( ( item1[ 0 ] - item2[ 0 ] ) *
                        ( item1[ 0 ] - item2[ 0 ] ) +
                        ( item1[ 1 ] - item2[ 1 ] ) *
                        ( item1[ 1 ] - item2[ 1 ] ) );

    }

};


}


int main( int /*argc*/, char* argv[] )
{


  int result = EXIT_SUCCESS;

  try
  {

    gkg::PluginLoader::getInstance().load();

    // in fact, 'gsl' library is the default; so the following line is not
    // mandatory: it is only for didactic purpose
    gkg::NumericalAnalysisSelector::getInstance().select( "gsl" );


    std::vector< std::vector< double > > dataset;
    loadCsv( 2, false, dataset );


    int32_t itemCount = ( int32_t  )dataset.size();

    std::cout << "item count size: " << itemCount << std::endl;
    

    gkg::DistanceOperator< std::vector< double > > distanceOperator;


    gkg::HierarchicalDensityBasedSpatialClustering< std::vector< double > >
      hdbscan( dataset, 
               distanceOperator,
               5,
               5 );

    hdbscan.displayResults();

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
