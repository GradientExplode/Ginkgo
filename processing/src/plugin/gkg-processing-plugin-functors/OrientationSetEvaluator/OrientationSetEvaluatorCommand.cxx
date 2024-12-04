#include <gkg-processing-plugin-functors/OrientationSetEvaluator/OrientationSetEvaluatorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-processing-orientations/Orientation.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>


int32_t getGlobalOrientationCount( const std::string& fileName )
{

  try
  {

    std::ifstream is( fileName.c_str() );

    int32_t fileCount;
    is >> fileCount;
    is.close();

    return fileCount;

  }
  GKG_CATCH( "int getGlobalOrientationCount( const std::string& fileName )" );


}


std::list< gkg::Orientation* > readSubSet( const std::string& fileName,
                                           int32_t& start, int32_t& count )
{

  try
  {

    std::ifstream is( fileName.c_str() );

    int32_t fileCount;
    is >> fileCount;

    if ( start == -1 )
    {

      start = 0;

    }
    if ( count == -1 )
    {

      count = fileCount;

    }

    if ( start + count > fileCount )
    {

      throw std::runtime_error( "not a coherent orientation file" );

    }

    int32_t index = 0;
    float x, y, z;
    while ( index < start )
    {

      is >> x >> y >> z;
      ++ index;

    }

    std::list< gkg::Orientation* > subSet;
    for ( index = 0; index < count; index++ )
    {

      is >> x >> y >> z;
      subSet.push_back( new gkg::Orientation(
                          gkg::Vector3d< float >( x, y, z ) ) );

    }

    is.close();

    return subSet;

  }
  GKG_CATCH( "std::list< gkg::Orientation* > "
             "readSubSet( const std::string& fileName, "
             "int& start, int& count )" );

}


float getGlobalEnergy( const std::list< gkg::Orientation* >& subSet )
{

  float energy = 0.0;

  std::list< gkg::Orientation* >::const_iterator o = subSet.begin(),
                                                 oe = subSet.end();
  while ( o != oe )
  {

    std::list< gkg::Orientation* >::const_iterator oprime = o;
    ++ oprime;
    while ( oprime != oe )
    {

      energy += ( *o )->getEnergy( **oprime );
      ++ oprime;

    }
    ++ o;

  }

  energy /= ( float )( subSet.size() ) *
            ( float )( subSet.size() - 1 );

  return energy;

}


float getConditionNumber( const std::list< gkg::Orientation* >& subSet )
{

  try
  {

    static gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    int32_t orientationCount = ( int32_t )subSet.size();

    gkg::Matrix M( orientationCount, 6 );
    std::list< gkg::Orientation* >::const_iterator o = subSet.begin(),
                                                   oe = subSet.end();
    int32_t index = 0;
    while ( o != oe )
    {

      M( index, 0 ) = ( *o )->getUnitVector().x * ( *o )->getUnitVector().x;
      M( index, 1 ) = ( *o )->getUnitVector().y * ( *o )->getUnitVector().y;
      M( index, 2 ) = ( *o )->getUnitVector().z * ( *o )->getUnitVector().z;
      M( index, 3 ) = 2.0 * ( *o )->getUnitVector().x *
                      ( *o )->getUnitVector().y;
      M( index, 4 ) = 2.0 * ( *o )->getUnitVector().x *
                      ( *o )->getUnitVector().z;
      M( index, 5 ) = 2.0 * ( *o )->getUnitVector().y *
                      ( *o )->getUnitVector().z;
      ++ o;
      ++ index;

    }

    if ( M.getSize1() < M.getSize2() )
    {

      M.transpose();

    }

    int32_t minimumSize = std::min( 6, orientationCount );
    

    gkg::Vector S( minimumSize );
    gkg::Matrix V( minimumSize, minimumSize );
  
    factory->getSingularValueDecomposition( M, V, S );

    double minimum = S( 0 );
    double maximum = S( 0 );

    int32_t s;
    for ( s = 1; s < S.getSize(); s++ )
    {

      if ( S( s ) != 0.0 )
      {

        if ( S( s ) < minimum )
        {

          minimum = S( s );

        }
        else if ( S( s ) > maximum )
        {

          maximum = S( s );

        }

      }

    }

    return ( float )( maximum / minimum );

  }
  GKG_CATCH( "float getConditionNumber( "
             "const std::list< gkg::Orientation* >& subSet )" );

}


gkg::OrientationSetEvaluatorCommand::OrientationSetEvaluatorCommand(
                                                              int32_t argc,
                                			      char* argv[],
                                			      bool loadPlugin,
                                			      bool removeFirst )
                                   : gkg::Command( argc, argv,
                                                   loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH(
         "gkg::OrientationSetEvaluatorCommand::OrientationSetEvaluatorCommand( "
         "int32_t argc, char* argv[], bool loadPlugin, "
         "bool removeFirst )" );

}


gkg::OrientationSetEvaluatorCommand::OrientationSetEvaluatorCommand(
                                    		  const std::string& fileNameIn,
                                    		  int32_t firstOrientation,
                                    		  int32_t orientationCount,
                                    		  int32_t windowSize,
                                    		  const std::string& backupName,
                                    		  bool verbose )
                                   : gkg::Command()
{

  try
  {

    execute( fileNameIn, firstOrientation, orientationCount, windowSize,
             backupName, verbose );

  }
  GKG_CATCH(   
         "gkg::OrientationSetEvaluatorCommand::OrientationSetEvaluatorCommand( "
         "const std::string& fileNameIn, int32_t firstOrientation, "
         "int32_t orientationCount, int32_t windowSize, "
         "const std::string& backupName, bool verbose )" );

}


gkg::OrientationSetEvaluatorCommand::OrientationSetEvaluatorCommand(
                                             const gkg::Dictionary& parameters )
                                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, firstOrientation );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, orientationCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, windowSize );
    DECLARE_STRING_PARAMETER( parameters, std::string, backupName );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, firstOrientation, orientationCount, windowSize,
             backupName, verbose );

  }
  GKG_CATCH( "gkg::OrientationSetEvaluatorCommand::"
             "OrientationSetEvaluatorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::OrientationSetEvaluatorCommand::~OrientationSetEvaluatorCommand()
{
}


std::string gkg::OrientationSetEvaluatorCommand::getStaticName()
{

  try
  {

    return "OrientationSetEvaluator";

  }
  GKG_CATCH( 
           "std::string gkg::OrientationSetEvaluatorCommand::getStaticName()" );

}


void gkg::OrientationSetEvaluatorCommand::parse()
{

  try
  {

    std::string fileNameIn;
    int32_t firstOrientation = -1;
    int32_t orientationCount = -1;
    int32_t windowSize = -1;
    std::string backupName = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Discrete spheric distribution evaluation",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Orientation set text file",
                                 fileNameIn );
    application.addSingleOption( "-o1",
                                 "First orientation index",
                                 firstOrientation,
                                 true );
    application.addSingleOption( "-oc",
                                 "Orientation count",
                                 orientationCount,
                                 true );
    application.addSingleOption( "-ws",
                                 "Window size",
                                 windowSize,
                                 true );
    application.addSingleOption( "-backup",
                                 "Backup filename",
                                 backupName,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn, firstOrientation, orientationCount, windowSize,
             backupName, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::OrientationSetEvaluatorCommand::parse()" );

}


void gkg::OrientationSetEvaluatorCommand::execute(
                  	            		  const std::string& fileNameIn,
                                    		  int32_t firstOrientation,
                                    		  int32_t orientationCount,
                                    		  int32_t windowSize,
                                    		  const std::string& backupName,
                                    		  bool /* verbose */ )
{

  try
  {

    if ( windowSize == -1 )
    {

      // reading subset
      std::list< gkg::Orientation* >
        subSet = readSubSet( fileNameIn, firstOrientation, orientationCount );

      std::list< gkg::Orientation* >::iterator o, oe = subSet.end();

      // backuping sub-set
      if ( !backupName.empty() )
      {

        std::ofstream os( backupName.c_str() );
        os << subSet.size() << std::endl;
        o = subSet.begin();
        while ( o != oe )
        {

          os << **o;
          ++ o;

        }

        os.close();

      }

      // processing energy of sub-set
      std::cout << "global energy : " << getGlobalEnergy( subSet )
                << std::endl;

      // processing condition number of sub-set
      std::cout << "cond.  number : " << getConditionNumber( subSet )
                << std::endl;

      // deleting subset
      o = subSet.begin();
      while ( o != oe )
      {

        delete *o;
        ++ o;

      }

    }
    else
    {

      int32_t globalOrientationCount = getGlobalOrientationCount( fileNameIn );
      int32_t s;

      int32_t subSetCount = globalOrientationCount - windowSize + 1;

      float meanEnergy = 0.0;
      float meanConditionNumber = 0.0;
      std::vector< float > energies( subSetCount );
      std::vector< float > conditionNumbers( subSetCount );

      for ( s = 0; s < subSetCount; s++ )
      {

        // reading subset
        std::list< gkg::Orientation* >
          subSet = readSubSet( fileNameIn, s, windowSize );

        // processing energy of sub-set
        energies[ s ] = getGlobalEnergy( subSet );
        conditionNumbers[ s ] = getConditionNumber( subSet );
        meanEnergy += energies[ s ];
        meanConditionNumber += conditionNumbers[ s ];

      }
      meanEnergy /= subSetCount;
      meanConditionNumber /= subSetCount;

      std::ostream* pOstream = 0;
      if ( !backupName.empty() )
      {

        pOstream = new std::ofstream( backupName.c_str() );


      }
      else
      {

        pOstream = &std::cout;

      }

      *pOstream << "sub-set count     : " << subSetCount << std::endl;
      *pOstream << "mean energy       : " << meanEnergy << std::endl;
      *pOstream << "mean cond. number : " << meanConditionNumber << std::endl;

      for ( s = 0; s < subSetCount; s++ )
      {

        *pOstream << std::setw( 5 ) << s
                  << "    " << energies[ s ]
                  << "    " << conditionNumbers[ s ] << std::endl;

      }

      if ( !backupName.empty() )
      {

        delete pOstream;

      }
      
    }

  }
  GKG_CATCH( "void gkg::OrientationSetEvaluatorCommand::execute( "
             "const std::string& fileNameIn, int32_t firstOrientation, "
             "int32_t orientationCount, int32_t windowSize, "
             "const std::string& backupName, bool verbose )" );

}


RegisterCommandCreator( OrientationSetEvaluatorCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_INTEGER_PARAMETER_HELP( firstOrientation ) +
    			DECLARE_INTEGER_PARAMETER_HELP( orientationCount ) +
    			DECLARE_INTEGER_PARAMETER_HELP( windowSize ) +
    			DECLARE_STRING_PARAMETER_HELP( backupName ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
