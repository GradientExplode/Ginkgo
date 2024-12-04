#include <gkg-dmri-plugin-functors/DwiPhantomSNRMeasure/DwiPhantomSNRMeasureCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <map>
#include <algorithm>
#include <fstream>
#include <iomanip>


//
// struct DiffusionWeight
//

namespace gkg
{


struct DiffusionWeight
{

  DiffusionWeight();
  DiffusionWeight( const Vector3d< float >& theOrientation, float theBValue );
  DiffusionWeight( const DiffusionWeight& other );

  gkg::Vector3d< float > orientation;
  float bValue;

};


struct DiffusionWeightCompare /*: public std::binary_function< DiffusionWeight,
                                                             DiffusionWeight,
                                                             bool >
                          deprecated in c++17 */
{

  typedef DiffusionWeight first_argument_type;
  typedef DiffusionWeight second_argument_type;
  typedef bool result_type;

  bool operator()( const DiffusionWeight& diffusionWeight1,
                   const DiffusionWeight& diffusionWeight2 ) const
  {

    return ( diffusionWeight1.orientation.z <
             diffusionWeight2.orientation.z ) ||
           ( ( diffusionWeight1.orientation.z ==
               diffusionWeight2.orientation.z ) &&
             ( ( diffusionWeight1.orientation.y <
                 diffusionWeight2.orientation.y ) ||
               ( ( diffusionWeight1.orientation.y ==
                   diffusionWeight2.orientation.y ) &&
                 ( ( diffusionWeight1.orientation.x <
                     diffusionWeight2.orientation.x ) ||
                   ( ( diffusionWeight1.orientation.x ==
                       diffusionWeight2.orientation.x ) &&
                     ( diffusionWeight1.bValue <
                       diffusionWeight2.bValue ) ) ) ) ) );

  }

};


}


gkg::DiffusionWeight::DiffusionWeight()
                     : orientation( 0, 0, 0 ),
                       bValue( 0 )
{
}


gkg::DiffusionWeight::DiffusionWeight( const Vector3d< float >& theOrientation,
                                       float theBValue )
                     : orientation( theOrientation ),
                       bValue( theBValue )
{
}


gkg::DiffusionWeight::DiffusionWeight( const gkg::DiffusionWeight& other )
                     : orientation( other.orientation ),
                       bValue( other.bValue )
{
}


namespace std
{

ostream& operator<<( ostream& os,
                     const gkg::DiffusionWeight& thing );


}


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::DiffusionWeight& thing )
{

  os << "( " << thing.orientation << ", "
             << std::setw( 4 ) << thing.bValue << " s/mm2 )";
  return os;

}


//
// readDiffusionWeighting()
//

void readDiffusionWeighting(
                   const std::string& fileNameWeight,
                   std::map< gkg::DiffusionWeight,
                             std::set< int32_t >,
                             gkg::DiffusionWeightCompare >& diffusionWeightMap )
{

  try
  {

    std::ifstream is( fileNameWeight.c_str() );

    if ( !is )
    {

      throw std::runtime_error( std::string( "cannot open '" ) +
                                fileNameWeight + "' file" );

    }

    int32_t count = 0;
    is >> count;

    gkg::DiffusionWeight diffusionWeight;
    int32_t i;
    for ( i = 0; i < count; i++ )
    {

      is >> diffusionWeight.orientation.x
         >> diffusionWeight.orientation.y
         >> diffusionWeight.orientation.z
         >> diffusionWeight.bValue;

      diffusionWeightMap[ diffusionWeight ].insert( i );

    }

    is.close();

  }
  GKG_CATCH( "void readDiffusionWeighting( "
             "const std::string& fileNameWeight, "
             "std::map< gkg::DiffusionWeight, "
             "          std::set< int32_t >, "
             "          gkg::DiffusionWeightCompare >& diffusionWeightMap )" );

}


//
// class DwiPhantomSNRMeasureCommand
//

gkg::DwiPhantomSNRMeasureCommand::DwiPhantomSNRMeasureCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                 : gkg::Command( argc, argv, loadPlugin,
                                                 removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiPhantomSNRMeasureCommand::DwiPhantomSNRMeasureCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiPhantomSNRMeasureCommand::DwiPhantomSNRMeasureCommand(
                                             const std::string& fileNameDwi,
                                             const std::string& fileNameWeight,
                                             const std::vector< int32_t >& sBox,
                                             const std::vector< int32_t >& nBox,
                                             const std::string& fileNameOut,
                                             bool verbose )
                                 : gkg::Command()
{

  try
  {

    execute( fileNameDwi,
             fileNameWeight,
             sBox,
             nBox,
             fileNameOut,
             verbose  );

  }
  GKG_CATCH( "gkg::DwiPhantomSNRMeasureCommand::DwiPhantomSNRMeasureCommand( "
             "const std::string& fileNameDwi, "
             "const std::string& fileNameWeight, "
             "const std::vector< int32_t >& sBox, "
             "const std::vector< int32_t >& nBox, "
             "const std::string& fileNameOut, "
             "bool verbose )" );

}


gkg::DwiPhantomSNRMeasureCommand::DwiPhantomSNRMeasureCommand(
                                             const gkg::Dictionary& parameters )
                                 : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDwi );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameWeight );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters, std::vector< int32_t >,
                                          sBox );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters, std::vector< int32_t >,
                                          nBox );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameDwi,
             fileNameWeight,
             sBox,
             nBox,
             fileNameOut,
             verbose  );

  }
  GKG_CATCH( "gkg::DwiPhantomSNRMeasureCommand::DwiPhantomSNRMeasureCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiPhantomSNRMeasureCommand::~DwiPhantomSNRMeasureCommand()
{
}


std::string gkg::DwiPhantomSNRMeasureCommand::getStaticName()
{

  try
  {

    return "DwiPhantomSNRMeasure";

  }
  GKG_CATCH( "std::string gkg::DwiPhantomSNRMeasureCommand::getStaticName()" );

}


void gkg::DwiPhantomSNRMeasureCommand::parse()
{

  try
  {

    std::string fileNameDwi;
    std::string fileNameWeight;
    std::vector< int32_t > sBox;
    std::vector< int32_t > nBox;
    std::string fileNameOut = "";
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "DWI phantom SNR measurement tool",
                                  _loadPlugin );
    application.addSingleOption( "-dwi",
                                 "Input DWI file name",
                                 fileNameDwi );
    application.addSingleOption( "-w",
                                 "Diffusion-weighting file name",
                                 fileNameWeight );
    application.addSeriesOption( "-sbox",
                                 "Integer bounding box on which to compute"
                                 " signal statistics",
                                 sBox,
                                 6U, 6U );
    application.addSeriesOption( "-nbox",
                                 "Integer bounding box on which to compute"
                                 " noise statistics",
                                 nBox,
                                 6U, 6U );
    application.addSingleOption( "-o",
                                 "Output text file name",
                                 fileNameOut,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameDwi,
             fileNameWeight,
             sBox,
             nBox,
             fileNameOut,
             verbose  );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiPhantomSNRMeasureCommand::parse()" );

}


void gkg::DwiPhantomSNRMeasureCommand::execute(
                                             const std::string& fileNameDwi,
                                             const std::string& fileNameWeight,
                                             const std::vector< int32_t >& sBox,
                                             const std::vector< int32_t >& nBox,
                                             const std::string& fileNameOut,
                                             bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading diffusion weigthing information
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameWeight << "' : " << std::flush;

    }
    std::map< gkg::DiffusionWeight,
              std::set< int32_t >,
              gkg::DiffusionWeightCompare > diffusionWeightMap;
    readDiffusionWeighting( fileNameWeight,
                            diffusionWeightMap );
    std::map< gkg::DiffusionWeight,
              std::set< int32_t >,
              gkg::DiffusionWeightCompare >::const_iterator
      d, de = diffusionWeightMap.end();
    if ( verbose )
    {

      std::cout << "done" << std::endl;

      std::cout << "diffusion weight(s) : " << std::endl;
      d = diffusionWeightMap.begin();
      while ( d != de )
      {

        std::cout << d->first << std::endl;

        std::set< int32_t >::const_iterator
          i = d->second.begin(), ie = d->second.end();
        while (i != ie )
        {

          std::cout << *i << " ";
          ++ i;

        }
        std::cout << std::endl << std::endl;

        ++ d;

      }

    }

    int32_t diffusionWeightCount = ( int32_t )diffusionWeightMap.size();


    ////////////////////////////////////////////////////////////////////////////
    // reading signal DWI proxy
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading signal ROI : " << std::flush;

    }
    gkg::VolumeProxy< int16_t > dwiProxy;
    gkg::Reader::getInstance().preparePartialBinaryRead<
                                        gkg::Volume< int16_t > >( fileNameDwi,
                                                                  dwiProxy );

    ////////////////////////////////////////////////////////////////////////////
    // building bounding boxes
    ////////////////////////////////////////////////////////////////////////////

    // signal bounding box
    gkg::BoundingBox< int32_t > signalBox( sBox[ 0 ],
                                           sBox[ 3 ],
                                           sBox[ 1 ],
                                           sBox[ 4 ],
                                           sBox[ 2 ],
                                           sBox[ 5 ],
                                           1,
                                           dwiProxy.getSizeT() - 1 );
    int32_t signalSizeX = sBox[ 3 ] - sBox[ 0 ] + 1;
    int32_t signalSizeY = sBox[ 4 ] - sBox[ 1 ] + 1;
    int32_t signalSizeZ = sBox[ 5 ] - sBox[ 2 ] + 1;
    int32_t signalSizeT = dwiProxy.getSizeT() - 1;

    // noise bounding box
    gkg::BoundingBox< int32_t > noiseBox( nBox[ 0 ],
                                          nBox[ 3 ],
                                          nBox[ 1 ],
                                          nBox[ 4 ],
                                          nBox[ 2 ],
                                          nBox[ 5 ],
                                          1,
                                          dwiProxy.getSizeT() - 1 );
    int32_t noiseSizeX = nBox[ 3 ] - nBox[ 0 ] + 1;
    int32_t noiseSizeY = nBox[ 4 ] - nBox[ 1 ] + 1;
    int32_t noiseSizeZ = nBox[ 5 ] - nBox[ 2 ] + 1;
    int32_t noiseSizeT = dwiProxy.getSizeT() - 1;

    ////////////////////////////////////////////////////////////////////////////
    // reading signal and noise DWI data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading signal and noise ROI(s) : " << std::flush;

    }
    gkg::Volume< int16_t > dwi;
    gkg::Reader::getInstance().read( fileNameDwi, dwi );

    // reading signal ROI
    gkg::Volume< int16_t > signalRoi( signalSizeX,
                                      signalSizeY,
                                      signalSizeZ,
                                      signalSizeT );
    gkg::Reader::getInstance().partialBinaryRead< gkg::Volume< int16_t > >(
                                                                    dwiProxy,
                                                                    signalRoi,
                                                                    signalBox );
    // reading noise ROI
    gkg::Volume< int16_t > noiseRoi( noiseSizeX,
                                     noiseSizeY,
                                     noiseSizeZ,
                                     noiseSizeT );
    gkg::Reader::getInstance().partialBinaryRead< gkg::Volume< int16_t > >(
                                                                     dwiProxy,
                                                                     noiseRoi,
                                                                     noiseBox );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing noise statistics
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "processing noise statistics : " << std::flush;

    }


    // processing noise mean
    gkg::Volume< int16_t >::const_iterator n = noiseRoi.begin(),
                                           ne = noiseRoi.end();
    float noiseMean = 0.0f;
    while ( n != ne )
    {

      noiseMean += ( float )*n;
      ++ n;

    }
    noiseMean /= noiseRoi.getSizeXYZT();

    // processing noise standard deviation
    float noiseSDev = 0.0f;
    n = noiseRoi.begin();
    while ( n != ne )
    {

      noiseSDev += ( ( double )*n - noiseMean ) *
                   ( ( double )*n - noiseMean );
      ++ n;

    }
    noiseSDev /= ( noiseRoi.getSizeXYZT() - 1 );
    noiseSDev = std::sqrt( noiseSDev );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing signal statistics
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "processing signal statistics : " << std::flush;

    }

    int32_t x, y, z, index;

    // processing signal mean
    std::vector< float > signalMean( diffusionWeightCount, 0.0f );
    index = 0;
    d = diffusionWeightMap.begin();
    while ( d != de )
    {

      std::set< int32_t >::const_iterator
        t = d->second.begin(),
        te = d->second.end();
      while ( t != te )
      {

        for ( z = 0; z < signalSizeZ; z++ )
        {

          for ( y = 0; y < signalSizeY; y++ )
          {

            for ( x = 0; x < signalSizeX; x++ )
            {

              signalMean[ index ] += signalRoi( x, y, z, *t );

            }

          }

        }

        ++ t;

      }

      signalMean[ index ] /= ( float )signalRoi.getSizeXYZ() *
                             ( float )d->second.size();

      ++ d;
      ++ index;

    }

    // processing signal standard deviation
    std::vector< float > signalSDev( diffusionWeightCount, 0.0f );
    index = 0;
    d = diffusionWeightMap.begin();
    float error;
    while ( d != de )
    {

      std::set< int32_t >::const_iterator
        t = d->second.begin(),
        te = d->second.end();
      while ( t != te )
      {

        for ( z = 0; z < signalSizeZ; z++ )
        {

          for ( y = 0; y < signalSizeY; y++ )
          {

            for ( x = 0; x < signalSizeX; x++ )
            {

              error = signalRoi( x, y, z, *t ) - signalMean[ index ];
              signalSDev[ index ] += error * error;

            }

          }

        }
        ++ t;

      }

      signalSDev[ index ] /= ( ( float )signalRoi.getSizeXYZ() *
                               ( float )d->second.size() - 1 );
      signalSDev[ index ] = std::sqrt( signalSDev[ index ] );

      ++ d;
      ++ index;

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // displaying results
    ////////////////////////////////////////////////////////////////////////////

    if ( fileNameOut.empty() )
    {

      std::cout << "noise" << " :"
                << " mean=" << std::setw( 10 ) << noiseMean
                << " sdev=" << std::setw( 10 ) << noiseSDev
                << std::endl;

      index = 0;
      d = diffusionWeightMap.begin();
      while ( d != de )
      {

        std::cout << d->first << " :"
                  << " mean=" << std::setw( 10 ) << signalMean[ index ]
                  << " sdev=" << std::setw( 10 ) << signalSDev[ index ]
                  << " snr=" << std::setw( 10 ) << signalMean[ index ] /
                                                   noiseMean
                  << std::endl;
        ++ d;
        ++ index;

      }

    }
    else
    {

      std::ofstream os( fileNameOut.c_str() );
      os << "noise" << " :"
         << " mean=" << std::setw( 10 ) << noiseMean
         << " sdev=" << std::setw( 10 ) << noiseSDev
         << std::endl;

      index = 0;
      d = diffusionWeightMap.begin();
      while ( d != de )
      {

        os << d->first << " :"
           << " mean=" << std::setw( 10 ) << signalMean[ index ]
           << " sdev=" << std::setw( 10 ) << signalSDev[ index ]
           << " snr=" << std::setw( 10 ) << signalMean[ index ] /
                                            noiseMean
           << std::endl;
        ++ d;
        ++ index;

      }
      os.close();

    }

  }
  GKG_CATCH( "void gkg::DwiPhantomSNRMeasureCommand::execute( "
             "const std::string& fileNameDwi, "
             "const std::string& fileNameWeight, "
             "const std::vector< int32_t >& sBox, "
             "const std::vector< int32_t >& nBox, "
             "const std::string& fileNameOut, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiPhantomSNRMeasureCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameDwi ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameWeight ) +
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( sBox ) +
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( nBox ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
