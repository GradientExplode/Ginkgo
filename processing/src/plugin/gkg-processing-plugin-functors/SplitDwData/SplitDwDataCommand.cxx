#include <gkg-processing-plugin-functors/SplitDwData/SplitDwDataCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-processing-algobase/Scaler_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <list>
#include <map>


#define EPSILON  1e-10


void readDiffusionWeightingCount( const std::string& fileNameWeight,
                                  int32_t& t2Count,
                                  int32_t& dwCount )
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

    t2Count = 0;
    dwCount = 0;

    int32_t i = 0;
    double x, y, z, bValue;

    for ( i = 0; i < count; i++ )
    {

      is >> x >> y >> z >> bValue;
      if ( bValue < EPSILON )
      {

        ++ t2Count;

      }
      else
      {

        ++ dwCount;

      }

    }

    is.close();

  }
  GKG_CATCH( "void readDiffusionWeightingCount( "
             "const std::string& fileNameWeight, "
             "int& t2Count, "
             "int& dwCount )" );

}

void readDiffusionWeighting(
                         const std::string& fileNameWeight,
                         int32_t& t2Count,
                         int32_t& dwCount,
                         std::vector< int32_t >& t2Indices,
                         std::vector< int32_t >& dwIndices,
                         std::vector< gkg::Vector3d< double > >& orientationSet,
                         std::vector< double >& bValueSet,
                         int32_t offset,
                         int32_t countInStep,
                         bool hasPc )
{

  try
  {

    std::list< int32_t > tmpT2Indices;
    std::list< int32_t > tmpDwIndices;
    std::list< gkg::Vector3d< double > > tmpOrientationSet;
    std::list< double > tmpBValueSet;

    std::ifstream is( fileNameWeight.c_str() );

    if ( !is )
    {

      throw std::runtime_error( std::string( "cannot open '" ) +
                                fileNameWeight + "' file" );

    }

    int32_t count = 0;
    is >> count;

    if ( ( countInStep == 0 ) && ( offset == 0 ) )
    {

      countInStep = count;

    }

    if ( offset + countInStep > count )
    {

      throw std::runtime_error( "offset + countInStep >= count" );

    }

    t2Count = 0;
    dwCount = 0;

    int32_t i = 0;
    double x, y, z, bValue;
    // dummy reads
    for ( i = 0; i < offset; i++ )
    {

      is >> x >> y >> z >> bValue;

    }
    // actual reads
    for ( i = 0; i < countInStep; i++ )
    {

      is >> x >> y >> z >> bValue;
      if ( bValue < EPSILON )
      {

        ++ t2Count;
        tmpT2Indices.push_back( i + ( hasPc ? 1 : 0 ) );

      }
      else
      {

        ++ dwCount;
        tmpDwIndices.push_back( i + ( hasPc ? 1 : 0 ) );
        tmpOrientationSet.push_back( gkg::Vector3d< double >( x, y, z ) );
        tmpBValueSet.push_back( bValue );

      }

    }

    is.close();

    t2Indices = std::vector< int32_t >( tmpT2Indices.begin(),
                                        tmpT2Indices.end() );
    dwIndices = std::vector< int32_t >( tmpDwIndices.begin(),
                                        tmpDwIndices.end() );
    orientationSet = std::vector< gkg::Vector3d< double > >(
                                                      tmpOrientationSet.begin(),
                                                      tmpOrientationSet.end() );
    bValueSet = std::vector< double >( tmpBValueSet.begin(),
                                       tmpBValueSet.end() );

  }
  GKG_CATCH( "void readDiffusionWeighting( "
             "const std::string& fileNameWeight, "
             "int& t2Count, "
             "int& dwCount, "
             "std::vector< int >& t2Indices, "
             "std::vector< int >& dwIndices, "
             "std::vector< gkg::Vector3d< double >& orientationSet, "
             "std::vector< double >& bValueSet, "
             "bool hasPc )" );

}


//
//  SplitDwDataCommand
//


gkg::SplitDwDataCommand::SplitDwDataCommand( int32_t argc,
                                 	     char* argv[],
                                 	     bool loadPlugin,
                                 	     bool removeFirst )
                       : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::SplitDwDataCommand::SplitDwDataCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::SplitDwDataCommand::SplitDwDataCommand(
                               const std::vector< std::string >& fileNameIn,
                               const std::vector< std::string >& fileNameWeight,
                               const std::string& fileNamePC,
                               const std::string& fileNameT2,
                               const std::string& fileNameMultiT2,
                               const std::string& fileNameDW,
                               bool ascii,
                               const std::string& outputFormat,
                               bool verbose )
                       : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameWeight, fileNamePC, fileNameT2,
             fileNameMultiT2, fileNameDW, ascii, outputFormat, verbose );

  }
  GKG_CATCH( "gkg::SplitDwDataCommand::SplitDwDataCommand( "
             "const std::vector< std::string >& fileNameIn, "
             "const std::vector< std::string >& fileNameWeight, "
             "const std::string& fileNamePC, const std::string& fileNameT2, "
             "const std::string& fileNameMultiT2, "
             "const std::string& fileNameDW, bool ascii, "
             "const std::string& outputFormat, bool verbose )" );

}


gkg::SplitDwDataCommand::SplitDwDataCommand( const gkg::Dictionary& parameters )
                       : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >,
                                         fileNameIn );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >,
                                         fileNameWeight );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNamePC );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMultiT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDW );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameWeight, fileNamePC, fileNameT2,
             fileNameMultiT2, fileNameDW, ascii, outputFormat, verbose );

  }
  GKG_CATCH( "gkg::SplitDwDataCommand::SplitDwDataCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::SplitDwDataCommand::~SplitDwDataCommand()
{
}


std::string gkg::SplitDwDataCommand::getStaticName()
{

  try
  {

    return "SplitDwData";

  }
  GKG_CATCH( "std::string gkg::SplitDwDataCommand::getStaticName()" );

}


void gkg::SplitDwDataCommand::parse()
{

  try
  {

    std::vector< std::string > fileNameIn;
    std::vector< std::string > fileNameWeight;
    std::string fileNamePC;
    std::string fileNameT2;
    std::string fileNameMultiT2;
    std::string fileNameDW;
    bool ascii = false;
    std::string outputFormat = "gis";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Split GkgDwSpinEchoEpi data into "
                                  "Phase Correction/T2/Diffusion Weighted "
                                  "files",
                                  _loadPlugin );
    application.addSeriesOption( "-i",
                                 "Input file name(s)",
                                 fileNameIn,
                                 1U );
    application.addSingleOption( "-pc",
                                 "Output phase correction file name",
                                 fileNamePC,
                                 true );
    application.addSingleOption( "-t2",
                                 "Output T2 file name",
                                 fileNameT2 );
    application.addSingleOption( "-mt2",
                                 "Output multiple T2 file name",
                                 fileNameMultiT2,
                                 true );
    application.addSingleOption( "-dw",
                                 "Output DW file name",
                                 fileNameDW );
    application.addSeriesOption( "-w",
                                 "Diffusion-weighting file name(s)",
                                 fileNameWeight );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn, fileNameWeight, fileNamePC, fileNameT2,
             fileNameMultiT2, fileNameDW, ascii, outputFormat, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::SplitDwDataCommand::parse()" );

}


void gkg::SplitDwDataCommand::execute(
                  	       const std::vector< std::string >& fileNameIn,
                               const std::vector< std::string >& fileNameWeight,
                               const std::string& fileNamePC,
                               const std::string& fileNameT2,
                               const std::string& fileNameMultiT2,
                               const std::string& fileNameDW,
                               bool /* ascii */,
                               const std::string& outputFormat,
                               bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting file count and performing sanity checks
    ////////////////////////////////////////////////////////////////////////////
    int32_t pfileCount = ( int32_t )fileNameIn.size();
    int32_t wfileCount = ( int32_t )fileNameWeight.size();

    if ( !pfileCount )
    {

      throw std::runtime_error( "there should be at least one file" );

    }


    if ( !( ( ( pfileCount == 1 ) && ( wfileCount == 0 ) ) ||
            ( ( pfileCount == 1 ) && ( wfileCount == 1 ) ) ||
            ( ( pfileCount > 1 ) && ( wfileCount == 0 ) ) ||
            ( ( pfileCount > 1 ) && ( wfileCount == 1 ) ) ||
            ( ( pfileCount > 1 ) && ( wfileCount == pfileCount ) ) ) )
    {

      throw std::runtime_error( "incompatible p-file and weight-file count" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // checking that data is readable and collecting size(s)
    ////////////////////////////////////////////////////////////////////////////
    std::vector< gkg::VolumeProxy< int16_t > > inputVolumeProxy( pfileCount );

    int32_t f;
    std::vector< int32_t > sizeX( pfileCount );
    std::vector< int32_t > lineCount( pfileCount );
    std::vector< int32_t > sliceCount( pfileCount );
    std::vector< int32_t > timeCount( pfileCount );
    int32_t globalTimeCount = 0;
    for ( f = 0; f < pfileCount; f++ )
    {

      if ( verbose )
      {

        std::cout << "reading GIS file header: "
                  << fileNameIn[ f ] << std::endl;

      }
      std::string format;

      format = gkg::Reader::getInstance().preparePartialBinaryRead<
                              gkg::Volume< int16_t > >( fileNameIn[ f ],
                                                        inputVolumeProxy[ f ] );
      if ( format != "gis" )
      {

        throw std::runtime_error( "wrong format, should be \'gis\'" );

      }

      sizeX[ f ] = inputVolumeProxy[ f ].getSizeX();
      lineCount[ f ] = inputVolumeProxy[ f ].getSizeY();
      sliceCount[ f ] = inputVolumeProxy[ f ].getSizeZ();
      timeCount[ f ] = inputVolumeProxy[ f ].getSizeT();

      globalTimeCount += timeCount[ f ];

      if ( f != 0 )
      {

        if ( ( sizeX[ f ] != sizeX[ 0 ] ) ||
             ( lineCount[ f ] != lineCount[ 0 ] ) ||
             ( sliceCount[ f ] != sliceCount[ 0 ] ) )
        {

          throw std::runtime_error( "inconsistent X, Y, Z size(s)" );

        }

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // getting information about pulse sequence name
    ////////////////////////////////////////////////////////////////////////////
    std::vector< std::string > psd_iname( pfileCount );

    for ( f = 0; f < pfileCount; f++ )
    {

      if ( inputVolumeProxy[ f ].getHeader().hasAttribute( 
                                       "pool_header.rdb_hdr_image.psd_iname" ) )
      {

        inputVolumeProxy[ f ].getHeader().getAttribute(
                        "pool_header.rdb_hdr_image.psd_iname", psd_iname[ f ] );

      }
      else if ( inputVolumeProxy[ f ].getHeader().hasAttribute( "dicom" ) )
      {

        gkg::Dictionary dicomDictionary;
        inputVolumeProxy[ f ].getHeader().getAttribute( "dicom",
                                                        dicomDictionary );
        if ( dicomDictionary.find( "(0019,109e)" ) != dicomDictionary.end() )
        {

          psd_iname[ f ] = dicomDictionary[ "(0019,109e)" ]->getValue<
                                                                std::string >();

        }
        else if ( dicomDictionary.find( "(0018,0024)" ) !=
                  dicomDictionary.end() )
        {

          gkg::GenericObjectList genericObjectListNames =
            dicomDictionary[ "(0018,0024)" ]->getValue<
                                                     gkg::GenericObjectList >();
          if ( genericObjectListNames[ 0 ]->getString().find( "ep_b" ) !=
               std::string::npos )
          {

            psd_iname[ f ] = "GkgSmsVB13ADwDualSpinEchoEpi";

          }

        }

      }

      if ( f != 0 )
      {

        if ( psd_iname[ f ] != psd_iname[ 0 ] )
        {

          throw std::runtime_error( "inconsistent pulse sequence name" );

        }

      }

    }

    if ( verbose )
    {

      std::cout <<  "pulse sequence name :  " << psd_iname[ 0 ] << std::endl;

    }

    if ( ( psd_iname[ 0 ] != "GkgDwSeEpi" ) &&
         ( psd_iname[ 0 ] != "GkgDwSe" ) &&
         ( psd_iname[ 0 ] != "NmrDwSeEpi" ) &&
         ( psd_iname[ 0 ] != "NmrDwSe" ) &&
         ( psd_iname[ 0 ] != "GkgSmsVB13ADwDualSpinEchoEpi" ) )
    {

      throw std::runtime_error( "bad pulse sequence name" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // checking if data has got phase correction volume
    ////////////////////////////////////////////////////////////////////////////
    bool hasPc = false;
    if ( ( psd_iname[ 0 ] == "GkgDwSeEpi" ) ||
         ( psd_iname[ 0 ] == "NmrDwSeEpi" ) )
    {

      hasPc = true;

    }
    else if ( ( psd_iname[ 0 ] != "GkgDwSe" ) ||
              ( psd_iname[ 0 ] != "NmrDwSe" ) ||
              ( psd_iname[ 0 ] != "GkgSmsVB13ADwDualSpinEchoEpi" ) )
    {

      hasPc = false;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading diffusion-weighting information(s)
    ////////////////////////////////////////////////////////////////////////////
    std::vector< int32_t > t2Count( pfileCount );
    std::vector< int32_t > dwCount( pfileCount );

    std::vector< std::vector< int32_t > > t2Indices( pfileCount );
    std::vector< std::vector< int32_t > > dwIndices( pfileCount );
    std::vector< std::vector< gkg::Vector3d< double > > >
      orientationSets( pfileCount );
    std::vector< std::vector< double > > bValueSets( pfileCount );

    int32_t globalT2Count = 0;
    int32_t globalDwCount = 0;
    double globalBValue = 0;

    double signX = +1.0;
    double signY = +1.0;
    double signZ = +1.0;


    if ( ( pfileCount == 1 ) && ( wfileCount == 0 ) )
    {

      if ( ( psd_iname[ 0 ] == "GkgDwSeEpi" ) ||
           ( psd_iname[ 0 ] == "NmrDwSeEpi" ) )
      {

        double opuser5 = 0.0;
        double opuser6 = 0.0;
        double opuser8 = 0.0;
        inputVolumeProxy[ 0 ].getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user5", opuser5 );
        inputVolumeProxy[ 0 ].getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user6", opuser6 );
        inputVolumeProxy[ 0 ].getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user8", opuser8 );

        if ( ( ( int32_t )( opuser8 + 0.5 ) / 100 ) == 101 )
        {

          opuser8 -= 10100;

        }

        if ( opuser8 >= 100 )
        {

          throw std::runtime_error(
            "multiple step acquisition requires multiple p-files" );

        }

        t2Count[ 0 ] = ( int32_t )( opuser8 + 0.5 );
        dwCount[ 0 ] = ( int32_t )( opuser5 + 0.5 );
        globalBValue = ( double )opuser6;

      }
      else if ( ( psd_iname[ 0 ] == "GkgDwSe" ) ||
                ( psd_iname[ 0 ] == "NmrDwSe" ) )
      {

        gkg::Dictionary dicomDictionary;
        inputVolumeProxy[ 0 ].getHeader().getAttribute( "dicom",
                                                        dicomDictionary );

        // tag containing orientation count information
        std::string tag_0019_10aa =
          dicomDictionary[ "(0019,10aa)" ]->getValue< std::string >();
        // tag containing b-value information
        std::string tag_0019_10ab =
          dicomDictionary[ "(0019,10ab)" ]->getValue< std::string >();
        // tag containing T2 count information
        std::string tag_0019_10ad =
          dicomDictionary[ "(0019,10ad)" ]->getValue< std::string >();

        float opuser3 = 0.0;
        float opuser4 = 0.0;
        float opuser6 = 0.0;
        gkg::StringConverter::toScalar( tag_0019_10aa, opuser3 );
        gkg::StringConverter::toScalar( tag_0019_10ab, opuser4 );
        gkg::StringConverter::toScalar( tag_0019_10ad, opuser6 );

        if ( opuser6 >= 100 )
        {

          throw std::runtime_error(
            "multiple step acquisition requires multiple p-files" );

        }

        t2Count[ 0 ] = ( int32_t )( opuser6 + 0.5 );
        dwCount[ 0 ] = ( int32_t )( opuser3 + 0.5 );
        globalBValue = ( double )opuser4;

      }
      else if ( psd_iname[ 0 ] == "GkgSmsVB13ADwDualSpinEchoEpi" )
      {

        gkg::Dictionary dicomDictionary;
        inputVolumeProxy[ 0 ].getHeader().getAttribute( "dicom",
                                                        dicomDictionary );

        gkg::GenericObjectList genericObjectListBValues =
          dicomDictionary[ "(0019,100c)" ]->getValue<gkg::GenericObjectList >();

        gkg::GenericObjectList::iterator
          b = genericObjectListBValues.begin(),
          be = genericObjectListBValues.end();
        std::map< int32_t, int32_t > bvalueMap;
        int32_t bValue = 0;
        int32_t maximumBValue = 0;
        while ( b != be )
        {

          bValue = 0;
          gkg::StringConverter::toScalar( ( *b )->getString(), bValue );
          if ( bValue > maximumBValue )
          {

            maximumBValue = bValue;

          }
          bvalueMap[ bValue ] ++;
          ++ b;

        }

        t2Count[ 0 ] = ( int32_t )bvalueMap[ 0 ];
        dwCount[ 0 ] = ( int32_t )bvalueMap[ maximumBValue ];
        globalBValue = ( double )maximumBValue;

        // managing trieder
        std::string axisTag = 
          dicomDictionary[ "(0051,1013)" ]->getValue< std::string >();

        signX = ( axisTag[ 1 ] == 'L' ? +1.0 : -1.0 );
        signY = ( axisTag[ 2 ] == 'A' ? +1.0 : -1.0 );
        signZ = ( axisTag[ 3 ] == 'H' ? +1.0 : -1.0 );

      }

      t2Indices[ 0 ] = std::vector< int32_t >( t2Count[ 0 ] );
      dwIndices[ 0 ] = std::vector< int32_t >( dwCount[ 0 ] );
      int32_t i;
      for ( i = 0; i < t2Count[ 0 ]; i++ )
      {

        t2Indices[ 0 ][ i ] = ( hasPc ? 1 : 0 ) + i;

      }
      for ( i = 0; i < dwCount[ 0 ]; i++ )
      {

        dwIndices[ 0 ][ i ] = ( hasPc ? 1 : 0 ) + t2Count[ 0 ] + i;

      }

      orientationSets[ 0 ] = std::vector< gkg::Vector3d< double > >(
                                                                 dwCount[ 0 ] );
      for ( i = 0; i < dwCount[ 0 ]; i++ )
      {

        orientationSets[ 0 ][ i ] = gkg::Vector3d< double >(
                  signX * gkg::GkgGetUniformOrientation( dwCount[ 0 ], i, 0 ),
                  signY * gkg::GkgGetUniformOrientation( dwCount[ 0 ], i, 1 ),
                  signZ * gkg::GkgGetUniformOrientation( dwCount[ 0 ], i, 2 ) );

      }
      bValueSets[ 0 ] = std::vector< double >( dwCount[ 0 ],
                                               globalBValue );

      globalT2Count = t2Count[ 0 ];
      globalDwCount = dwCount[ 0 ];

    }
    else if ( ( pfileCount > 1 ) && ( wfileCount == 0 ) )
    {

      int32_t step = 0, stepCount = 0, offset = 0, countInStep = 0;
      for ( f = 0; f < pfileCount; f++ )
      {

        if ( ( psd_iname[ 0 ] == "GkgDwSeEpi" ) ||
             ( psd_iname[ 0 ] == "NmrDwSeEpi" ) )
        {

          double opuser5 = 0.0;
          double opuser6 = 0.0;
          double opuser8 = 0.0;
          inputVolumeProxy[ f ].getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user5", opuser5 );
          inputVolumeProxy[ f ].getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user6", opuser6 );
          inputVolumeProxy[ f ].getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user8", opuser8 );
 
          if ( opuser8 < 100 )
          {

            throw std::runtime_error(
              "multiple step acquisition requires step count information" );

          }

          if ( f == 0 )
          {

            globalT2Count = ( int32_t )( opuser8 + 0.5 ) % 100;
            globalDwCount = ( int32_t )( opuser5 + 0.5 );
            globalBValue = ( double )opuser6;
            stepCount = ( ( int32_t )( opuser8 + 0.5 ) / 100 ) % 100;
            step = ( int32_t )( opuser8 + 0.5 ) / 10000;

          }
          else
          {

            if ( ( ( int32_t )( opuser8 + 0.5 ) % 100 ) != globalT2Count )
            {

              throw std::runtime_error( "incoherent p-files" );

            }
            if ( ( int32_t )( opuser5 + 0.5 ) != globalDwCount )
            {

              throw std::runtime_error( "incoherent p-files" );

            }
            if ( ( double )opuser6 != globalBValue )
            {

              throw std::runtime_error( "incoherent p-files" );

            }
            if ( ( ( ( int32_t )( opuser8 + 0.5 ) / 100 ) % 100 ) !=
                 stepCount )
            {

              throw std::runtime_error( "incoherent p-files" );

            }
            step = ( int32_t )( opuser8 + 0.5 ) / 10000;

          }

        }
        else if ( ( psd_iname[ 0 ] == "GkgDwSe" ) ||
                  ( psd_iname[ 0 ] == "NmrDwSe" ) )
        {

          gkg::Dictionary dicomDictionary;
          inputVolumeProxy[ f ].getHeader().getAttribute( "dicom",
                                                          dicomDictionary );

          // tag containing orientation count information
          std::string tag_0019_10aa =
            dicomDictionary[ "(0019,10aa)" ]->getValue< std::string >();
          // tag containing b-value information
          std::string tag_0019_10ab =
            dicomDictionary[ "(0019,10ab)" ]->getValue< std::string >();
          // tag containing T2 count information
          std::string tag_0019_10ad =
            dicomDictionary[ "(0019,10ad)" ]->getValue< std::string >();

          float opuser3 = 0.0;
          float opuser4 = 0.0;
          float opuser6 = 0.0;
          gkg::StringConverter::toScalar( tag_0019_10aa, opuser3 );
          gkg::StringConverter::toScalar( tag_0019_10ab, opuser4 );
          gkg::StringConverter::toScalar( tag_0019_10ad, opuser6 );

          if ( opuser6 < 100 )
          {

            throw std::runtime_error(
              "multiple step acquisition requires step count information" );

          }

          if ( f == 0 )
          {

            globalT2Count = ( int32_t )( opuser6 + 0.5 ) % 100;
            globalDwCount = ( int32_t )( opuser3 + 0.5 );
            globalBValue = ( double )opuser4;
            stepCount = ( ( int32_t )( opuser6 + 0.5 ) / 100 ) % 100;
            step = ( int32_t )( opuser6 + 0.5 ) / 10000;

          }
          else
          {

            if ( ( ( int32_t )( opuser6 + 0.5 ) % 100 ) != globalT2Count )
            {

              throw std::runtime_error( "incoherent dicom files" );

            }
            if ( ( int32_t )( opuser3 + 0.5 ) != globalDwCount )
            {

              throw std::runtime_error( "incoherent dicom files" );

            }
            if ( ( double )opuser4 != globalBValue )
            {

              throw std::runtime_error( "incoherent dicom files" );

            }
            if ( ( ( ( int32_t )( opuser6 + 0.5 ) / 100 ) % 100 ) !=
                 stepCount )
            {

              throw std::runtime_error( "incoherent dicom files" );

            }
            step = ( int32_t )( opuser6 + 0.5 ) / 10000;

          }

        }

        int32_t baseCount = ( globalT2Count + globalDwCount ) / stepCount;
        int32_t remainingCount = ( globalT2Count + globalDwCount ) % stepCount;
        if ( step <= remainingCount )
        {

          countInStep = baseCount + 1;
          offset = ( step - 1 ) * ( baseCount + 1 );

        }
        else
        {

          countInStep = baseCount;
          offset = ( step - 1 ) * baseCount + remainingCount;

        }

        if ( offset < globalT2Count )
        {

          if ( offset + countInStep < globalT2Count )
          {

            t2Count[ f ] = countInStep;
            dwCount[ f ] = 0;
            t2Indices[ f ] = std::vector< int32_t >( countInStep );
            dwIndices[ f ].clear();
            int32_t i;
            for ( i = 0; i < countInStep; i++ )
            {

              t2Indices[ f ][ i ] = ( hasPc ? 1 : 0 ) + i;

            }
            orientationSets[ f ].clear();
            bValueSets[ f ].clear();

          }
          else
          {

            t2Count[ f ] = globalT2Count - offset;
            dwCount[ f ] = countInStep - t2Count[ f ];
            t2Indices[ f ] = std::vector< int32_t >( t2Count[ f ] );
            dwIndices[ f ] = std::vector< int32_t >( dwCount[ f ] );;
            int32_t i;
            for ( i = 0; i < t2Count[ f ]; i++ )
            {

              t2Indices[ f ][ i ] = ( hasPc ? 1 : 0 ) + i;

            }
            for ( i = 0; i < dwCount[ f ]; i++ )
            {

              dwIndices[ f ][ i ] = ( hasPc ? 1 : 0 ) + t2Count[ f ] + i;

            }
            orientationSets[ f ] = std::vector< gkg::Vector3d< double > >(
                                                                 dwCount[ f ] );
            for ( i = 0; i < dwCount[ f ]; i++ )
            {

              orientationSets[ f ][ i ] =
                gkg::Vector3d< double >(
                  signX * gkg::GkgGetUniformOrientation( globalDwCount, i, 0 ),
                  signY * gkg::GkgGetUniformOrientation( globalDwCount, i, 1 ),
                  signZ * gkg::GkgGetUniformOrientation( globalDwCount, i, 2 ) );

            }
            bValueSets[ f ] = std::vector< double >( dwCount[ f ],
                                                     globalBValue );

          }

        }
        else
        {

          t2Count[ f ] = 0;
          dwCount[ f ] = countInStep;
          t2Indices[ f ].clear();
          dwIndices[ f ] = std::vector< int32_t >( dwCount[ f ] );
          int32_t i;
          for ( i = 0; i < dwCount[ f ]; i++ )
          {

            dwIndices[ f ][ i ] = ( hasPc ? 1 : 0 ) + i;

          }
          orientationSets[ f ] = std::vector< gkg::Vector3d< double > >(
                                                                 dwCount[ f ] );
          for ( i = 0; i < dwCount[ f ]; i++ )
          {

            orientationSets[ f ][ i ] =
              gkg::Vector3d< double >(
                signX * gkg::GkgGetUniformOrientation( globalDwCount,
                                        offset - globalT2Count + i, 0 ),
                signY * gkg::GkgGetUniformOrientation( globalDwCount,
                                        offset - globalT2Count + i, 1 ),
                signZ * gkg::GkgGetUniformOrientation( globalDwCount,
                                        offset - globalT2Count + i, 2 ) );

          }
          bValueSets[ f ] = std::vector< double >( dwCount[ f ],
                                                   globalBValue );

        }
/*
        std::cout << "globalT2Count=" << globalT2Count << std::endl;
        std::cout << "globalDwCount=" << globalDwCount << std::endl;
        std::cout << "baseCount=" << baseCount << std::endl;
        std::cout << "remainingCount=" << remainingCount << std::endl;
        std::cout << "offset=" << offset << std::endl;
        std::cout << "countInStep=" << countInStep << std::endl;
        std::cout << "step=" << step << std::endl;
        std::cout << "stepCount=" << stepCount << std::endl;
        std::cout << "t2Count[" << f << "]=" << t2Count[ f ] << std::endl;
        std::cout << "dwCount[" << f << "]=" << dwCount[ f ] << std::endl;
        std::cout << "---------------------------------------" << std::endl;
*/

      }

    }
    else if ( ( pfileCount > 1 ) && ( wfileCount == 1 ) )
    {

      readDiffusionWeightingCount( fileNameWeight[ 0 ],
                                   globalT2Count,
                                   globalDwCount );

      int32_t step = 0, stepCount = 0, offset = 0, countInStep = 0;
      for ( f = 0; f < pfileCount; f++ )
      {

        if ( ( psd_iname[ 0 ] == "GkgDwSeEpi" ) ||
             ( psd_iname[ 0 ] == "NmrDwSeEpi" ) )
        {

          double opuser5 = 0.0;
          double opuser6 = 0.0;
          double opuser8 = 0.0;
          inputVolumeProxy[ f ].getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user5", opuser5 );
          inputVolumeProxy[ f ].getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user6", opuser6 );
          inputVolumeProxy[ f ].getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user8", opuser8 );

          if ( ( int32_t )( opuser5 + 0.5 ) != 0 )
          {

            throw std::runtime_error( "opuser5 must be null" );

          }
          if ( ( int32_t )( opuser6 + 0.5 ) != 0 )
          {

            throw std::runtime_error( "opuser6 must be null" );

          }
          if ( ( ( int32_t )( opuser8 + 0.5 ) % 100 ) != 0 )
          {

            throw std::runtime_error( "opuser8 must be null" );

          }
          if ( opuser8 < 100 )
          {

            throw std::runtime_error(
              "multiple step acquisition requires step count information" );

          }

          if ( f == 0 )
          {

            stepCount = ( ( int32_t )( opuser8 + 0.5 ) / 100 ) % 100;
            step = ( int32_t )( opuser8 + 0.5 ) / 10000;

          }
          else
          {

            if ( ( ( ( int32_t )( opuser8 + 0.5 ) / 100 ) % 100 ) !=
                 stepCount )
            {

              throw std::runtime_error( "incoherent p-files" );

            }
            step = ( int32_t )( opuser8 + 0.5 ) / 10000;

          }

        }
        else if ( ( psd_iname[ 0 ] == "GkgDwSe" ) ||
                  ( psd_iname[ 0 ] == "NmrDwSe" ) )
        {

          gkg::Dictionary dicomDictionary;
          inputVolumeProxy[ f ].getHeader().getAttribute( "dicom",
                                                          dicomDictionary );

          // tag containing orientation count information
          std::string tag_0019_10aa =
            dicomDictionary[ "(0019,10aa)" ]->getValue< std::string >();
          // tag containing b-value information
          std::string tag_0019_10ab =
            dicomDictionary[ "(0019,10ab)" ]->getValue< std::string >();
          // tag containing T2 count information
          std::string tag_0019_10ad =
            dicomDictionary[ "(0019,10ad)" ]->getValue< std::string >();

          float opuser3 = 0.0;
          float opuser4 = 0.0;
          float opuser6 = 0.0;
          gkg::StringConverter::toScalar( tag_0019_10aa, opuser3 );
          gkg::StringConverter::toScalar( tag_0019_10ab, opuser4 );
          gkg::StringConverter::toScalar( tag_0019_10ad, opuser6 );

          if ( ( int32_t )( opuser3 + 0.5 ) != 0 )
          {

            throw std::runtime_error( "opuser5 must be null" );

          }
          if ( ( int32_t )( opuser4 + 0.5 ) != 0 )
          {

            throw std::runtime_error( "opuser6 must be null" );

          }
          if ( ( ( int32_t )( opuser6 + 0.5 ) % 100 ) != 0 )
          {

            throw std::runtime_error( "opuser8 must be null" );

          }
          if ( opuser6 < 100 )
          {

            throw std::runtime_error(
              "multiple step acquisition requires step count information" );

          }

          if ( f == 0 )
          {

            stepCount = ( ( int32_t )( opuser6 + 0.5 ) / 100 ) % 100;
            step = ( int32_t )( opuser6 + 0.5 ) / 10000;

          }
          else
          {

            if ( ( ( ( int32_t )( opuser6 + 0.5 ) / 100 ) % 100 ) !=
                 stepCount )
            {

              throw std::runtime_error( "incoherent dicom files" );

            }
            step = ( int32_t )( opuser6 + 0.5 ) / 10000;

          }

        }

        int32_t baseCount = ( globalT2Count + globalDwCount ) / stepCount;
        int32_t remainingCount = ( globalT2Count + globalDwCount ) % stepCount;
        if ( step <= remainingCount )
        {

          countInStep = baseCount + 1;
          offset = ( step - 1 ) * ( baseCount + 1 );

        }
        else
        {

          countInStep = baseCount;
          offset = ( step - 1 ) * baseCount + remainingCount;

        }

        readDiffusionWeighting( fileNameWeight[ f ],
                                t2Count[ f ],
                                dwCount[ f ],
                                t2Indices[ f ],
                                dwIndices[ f ],
                                orientationSets[ f ],
                                bValueSets[ f ],
                                offset,
                                countInStep,
                                hasPc );

      }

    }
    else if ( ( pfileCount >= 1 ) && ( wfileCount == pfileCount ) )
    {


      for ( f = 0; f < pfileCount; f++ )
      {

        readDiffusionWeighting( fileNameWeight[ f ],
                                t2Count[ f ],
                                dwCount[ f ],
                                t2Indices[ f ],
                                dwIndices[ f ],
                                orientationSets[ f ],
                                bValueSets[ f ],
                                0,
                                0,
                                hasPc );
        globalT2Count += t2Count[ f ];
        globalDwCount += dwCount[ f ];

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // time count sanity check
    ////////////////////////////////////////////////////////////////////////////
    if ( globalTimeCount != ( pfileCount * ( hasPc ? 1 : 0 ) + globalT2Count +
                                                               globalDwCount ) )
    {

      throw std::runtime_error( "inconsistent T size(s)" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // preparing a bounding box and a sub-volume for reading
    ////////////////////////////////////////////////////////////////////////////
    gkg::BoundingBox< int32_t > inputBoundingBox( 0, sizeX[ 0 ] - 1,
                                                  0, lineCount[ 0 ] - 1,
                                                  0, sliceCount[ 0 ] - 1,
                                                  0, 0 );
    gkg::Volume< int16_t > inputSubVolume;


    ////////////////////////////////////////////////////////////////////////////
    // preparing phase correction partial write proxy
    ////////////////////////////////////////////////////////////////////////////
    gkg::VolumeProxy< int16_t > pcVolumeProxy;
    if ( !fileNamePC.empty() && hasPc )
    {

      if ( verbose )
      {

        std::cout << "preparing phase correction partial write in \'"
                  << outputFormat
                  << "\' format: "
                  << std::flush;

      }
      pcVolumeProxy = gkg::VolumeProxy< int16_t >( sizeX[ 0 ],
                                                   lineCount[ 0 ],
                                                   sliceCount[ 0 ],
                                                   pfileCount );
      pcVolumeProxy.getHeader() = inputVolumeProxy[ 0 ].getHeader();
      pcVolumeProxy.getHeader()[ "sizeT" ] = pfileCount;

      if ( gkg::Writer::getInstance().preparePartialBinaryWrite< 
                                    gkg::Volume< int16_t > >( fileNamePC,
                                                              pcVolumeProxy,
                                                              outputFormat ) !=
           outputFormat )
      {

        throw std::runtime_error(
            std::string( "failed to prepare phase correction partial write " ) +
            "with format \' " + outputFormat + "\'" );

      }
      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // preparing T2 partial write proxy
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "preparing T2 partial write in \'"
                << outputFormat
                << "\' format: "
                << std::flush;

    }
    gkg::VolumeProxy< int16_t > t2VolumeProxy( sizeX[ 0 ],
                                               lineCount[ 0 ],
                                               sliceCount[ 0 ] );
    t2VolumeProxy.getHeader() = inputVolumeProxy[ 0 ].getHeader();
    t2VolumeProxy.getHeader()[ "sizeT" ] = 1;

    if ( gkg::Writer::getInstance().preparePartialBinaryWrite< 
                                    gkg::Volume< int16_t > >( fileNameT2,
                                                              t2VolumeProxy,
                                                              outputFormat ) !=
         outputFormat )
    {

      throw std::runtime_error(
              std::string( "failed to prepare T2 partial write with " ) +
              "format \' " + outputFormat + "\'" );

    }
    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // preparing T2 average float volume
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "preparing T2 averaging volume:"
                << std::flush;

    }
    gkg::Volume< float > t2VolumeFloat( sizeX[ 0 ],
                                        lineCount[ 0 ],
                                        sliceCount[ 0 ] );
    t2VolumeFloat.fill( 0.0 );
    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // preparing multi T2 partial write proxy
    ////////////////////////////////////////////////////////////////////////////
    gkg::VolumeProxy< int16_t > multiT2VolumeProxy;
    if ( !fileNameMultiT2.empty() )
    {

      if ( verbose )
      {

        std::cout << "preparing multi T2 partial write in \'"
                  << outputFormat
                  << "\' format: "
                  << std::flush;

      }
      multiT2VolumeProxy = gkg::VolumeProxy< int16_t >( sizeX[ 0 ],
                                                        lineCount[ 0 ],
                                                        sliceCount[ 0 ],
                                                        globalT2Count );
      multiT2VolumeProxy.getHeader() = inputVolumeProxy[ 0 ].getHeader();
      multiT2VolumeProxy.getHeader()[ "sizeT" ] = globalT2Count;

      if ( gkg::Writer::getInstance().preparePartialBinaryWrite<
                                   gkg::Volume< int16_t > >( fileNameMultiT2,
                                                             multiT2VolumeProxy,
                                                             outputFormat ) !=
           outputFormat )
      {

        throw std::runtime_error(
               std::string( "failed to prepare multi-T2 partial write with " ) +
               "format \' " + outputFormat + "\'" );

      }
      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // preparing raw DW partial write proxy
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "preparing raw DW partial write in \'"
                << outputFormat
                << "\' format: "
                << std::flush;

    }
    gkg::VolumeProxy< int16_t > dwVolumeProxy( sizeX[ 0 ],
                                               lineCount[ 0 ],
                                               sliceCount[ 0 ],
                                               globalDwCount );
    dwVolumeProxy.getHeader() = inputVolumeProxy[ 0 ].getHeader();
    dwVolumeProxy.getHeader()[ "sizeT" ] = globalDwCount;

    std::vector< double > bValues( globalDwCount );
    std::vector< double >::iterator b = bValues.begin();
    for ( f = 0; f < pfileCount; f++ )
    {

      std::vector< double >::const_iterator i = bValueSets[ f ].begin(),
                                            ie = bValueSets[ f ].end();
      while ( i != ie )
      {

        *b = *i;
        ++ i;
        ++ b;

      }

    }
    dwVolumeProxy.getHeader().addAttribute( "bvalues", bValues );

    gkg::GenericObjectList orientations( globalDwCount );
    int32_t o = 0;
    for ( f = 0; f < pfileCount; f++ )
    {

      std::vector< gkg::Vector3d< double > >::const_iterator
                                            i = orientationSets[ f ].begin(),
                                            ie = orientationSets[ f ].end();
      while ( i != ie )
      {

        gkg::GenericObjectList orientation( 3 );
        orientation[ 0 ] = i->x;
        orientation[ 1 ] = i->y; 
        orientation[ 2 ] = i->z;
        orientations[ o ] = orientation;

        ++ i;
        ++ o;

      }

    }
    dwVolumeProxy.getHeader().addAttribute( "diffusion_gradient_orientations",
                                            orientations );

    if ( gkg::Writer::getInstance().preparePartialBinaryWrite< 
                                gkg::Volume< int16_t > >( fileNameDW,
                                                          dwVolumeProxy,
                                                          outputFormat ) !=
         outputFormat )
    {

      throw std::runtime_error(
              std::string( "failed to prepare raw DW partial write with " ) +
              "format \' " + outputFormat + "\'" );

    }
    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }


    gkg::BoundingBox< int32_t > outputBoundingBox( 0, sizeX[ 0 ] - 1,
                                                   0, lineCount[ 0 ] - 1,
                                                   0, sliceCount[ 0 ] - 1,
                                                   0, 0 );

    ////////////////////////////////////////////////////////////////////////////
    // building phase correction volume if required
    ////////////////////////////////////////////////////////////////////////////
    if ( hasPc && !fileNamePC.empty() )
    {

      if ( verbose )
      {

        std::cout << "building PC data: " << std::flush;

      }
      inputBoundingBox.setLowerT( 0 );
      inputBoundingBox.setUpperT( 0 );
      for ( f = 0; f < pfileCount; f++ )
      {

        if ( verbose )
        {

          if ( f != 0 )
          {

            std::cout << gkg::Eraser( 20 );

          }
          std::cout << "file[ " << std::setw( 4 ) << f + 1
                    << " / " << std::setw( 4 ) << pfileCount
                    << " ] " << std::flush;

        }

        std::string format = gkg::Reader::getInstance().partialBinaryRead<
                                                   gkg::Volume< int16_t > >(
                                                          inputVolumeProxy[ f ],
                                                          inputSubVolume,
                                                          inputBoundingBox,
                                                          "gis" );
        if ( format != "gis" )
        {

          throw std::runtime_error( "wrong format, should be \'gis\'");

        }

        outputBoundingBox.setLowerT( f );
        outputBoundingBox.setUpperT( f );
        gkg::Writer::getInstance().partialBinaryWrite< 
	                                         gkg::Volume< int16_t > >(
                                                              pcVolumeProxy,
                                                              inputSubVolume,
                                                              outputBoundingBox,
                                                              outputFormat );

      }

    }
    if ( verbose )
    {

      std::cout << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building T2 data
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building T2 data: " << std::flush;

    }
    int32_t t2Index = 0;
    for ( f = 0; f < pfileCount; f++ )
    {

      int32_t k;
      for ( k = 0; k < t2Count[ f ]; k++ )
      {

        if ( verbose )
        {

          if ( t2Index != 0 )
          {

            std::cout << gkg::Eraser( 20 );

          }
          std::cout << "rank[ " << std::setw( 4 ) << t2Index + 1
                    << " / " << std::setw( 4 ) << globalT2Count
                    << " ] " << std::flush;

        }

        inputBoundingBox.setLowerT( t2Indices[ f ][ k ] );
        inputBoundingBox.setUpperT( t2Indices[ f ][ k ] );

        std::string format = gkg::Reader::getInstance().partialBinaryRead<
                                                   gkg::Volume< int16_t > >(
                                                          inputVolumeProxy[ f ],
                                                          inputSubVolume,
                                                          inputBoundingBox,
                                                          "gis" );
        if ( format != "gis" )
        {

          throw std::runtime_error( "wrong format, should be \'gis\'");

        }

        // accumulating data
        gkg::Volume< int16_t >::const_iterator i = inputSubVolume.begin(),
                                               ie = inputSubVolume.end();      
        gkg::Volume< float >::iterator a =   t2VolumeFloat.begin();
        while ( i != ie )
        {

          *a += ( float )*i;
          ++ i;
          ++ a;

        }

        ++ t2Index;

      }

    }
    gkg::Scaler< gkg::Volume< float > > scaler( 1 / ( float )globalT2Count );
    scaler.scale( t2VolumeFloat );

    gkg::Converter< gkg::Volume< float >,
                    gkg::Volume< int16_t > > converter;
    gkg::Volume< int16_t > averageT2Volume;
    converter.convert( t2VolumeFloat, averageT2Volume );

    outputBoundingBox.setLowerT( 0 );
    outputBoundingBox.setUpperT( 0 );
    gkg::Writer::getInstance().partialBinaryWrite< gkg::Volume< int16_t > >(
                                                              t2VolumeProxy,
                                                              averageT2Volume,
                                                              outputBoundingBox,
                                                              outputFormat );
    if ( verbose )
    {

      std::cout << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building multi-T2 data
    ////////////////////////////////////////////////////////////////////////////
    if ( !fileNameMultiT2.empty() )
    {

      if ( verbose )
      {

        std::cout << "building multi-T2 data: " << std::flush;

      }
      int32_t t2Index = 0;
      for ( f = 0; f < pfileCount; f++ )
      {

        int32_t k;
        for ( k = 0; k < t2Count[ f ]; k++ )
        {

          if ( verbose )
          {

            if ( t2Index != 0 )
            {

              std::cout << gkg::Eraser( 20 );

            }
            std::cout << "rank[ " << std::setw( 4 ) << t2Index + 1
                      << " / " << std::setw( 4 ) << globalT2Count
                      << " ] " << std::flush;

          }

          inputBoundingBox.setLowerT( t2Indices[ f ][ k ] );
          inputBoundingBox.setUpperT( t2Indices[ f ][ k ] );

          std::string format = gkg::Reader::getInstance().partialBinaryRead<
                                                     gkg::Volume< int16_t > >(
                                                          inputVolumeProxy[ f ],
                                                          inputSubVolume,
                                                          inputBoundingBox,
                                                          "gis" );
          if ( format != "gis" )
          {

            throw std::runtime_error( "wrong format, should be \'gis\'");

          }

          outputBoundingBox.setLowerT( t2Index );
          outputBoundingBox.setUpperT( t2Index );
          gkg::Writer::getInstance().partialBinaryWrite<
                                   gkg::Volume< int16_t > >( multiT2VolumeProxy,
                                                             inputSubVolume,
                                                             outputBoundingBox,
                                                             outputFormat );

          ++ t2Index;

        }

      }
      if ( verbose )
      {

        std::cout << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // building DW data
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building DW data: " << std::flush;

    }
    int32_t dwIndex = 0;
    for ( f = 0; f < pfileCount; f++ )
    {

      int32_t k;
      for ( k = 0; k < dwCount[ f ]; k++ )
      {

        if ( verbose )
        {

          if ( dwIndex != 0 )
          {

            std::cout << gkg::Eraser( 20 );

          }
          std::cout << "rank[ " << std::setw( 4 ) << dwIndex + 1
                    << " / " << std::setw( 4 ) << globalDwCount
                    << " ] " << std::flush;

        }

        inputBoundingBox.setLowerT( dwIndices[ f ][ k ] );
        inputBoundingBox.setUpperT( dwIndices[ f ][ k ] );

        std::string format = gkg::Reader::getInstance().partialBinaryRead<
                                                   gkg::Volume< int16_t > >(
                                                          inputVolumeProxy[ f ],
                                                          inputSubVolume,
                                                          inputBoundingBox,
                                                          "gis" );
        if ( format != "gis" )
        {

          throw std::runtime_error( "wrong format, should be \'gis\'");

        }

        outputBoundingBox.setLowerT( dwIndex );
        outputBoundingBox.setUpperT( dwIndex );
        gkg::Writer::getInstance().partialBinaryWrite< 
	                                         gkg::Volume< int16_t > >(
                                                              dwVolumeProxy,
                                                              inputSubVolume,
                                                              outputBoundingBox,
                                                              outputFormat );

        ++ dwIndex;

      }

    }
    if ( verbose )
    {

      std::cout << std::endl;

    }

  }
  GKG_CATCH( "void gkg::SplitDwDataCommand::execute( "
             "const std::vector< std::string >& fileNameIn, "
             "const std::vector< std::string >& fileNameWeight, "
             "const std::string& fileNamePC, const std::string& fileNameT2, "
             "const std::string& fileNameMultiT2, "
             "const std::string& fileNameDW, bool ascii, "
             "const std::string& outputFormat, bool verbose )" );

}


RegisterCommandCreator(
                    SplitDwDataCommand,
    		    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameIn ) +
    		    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameWeight ) +
    		    DECLARE_STRING_PARAMETER_HELP( fileNamePC ) +
    		    DECLARE_STRING_PARAMETER_HELP( fileNameT2 ) +
    		    DECLARE_STRING_PARAMETER_HELP( fileNameMultiT2 ) +
    		    DECLARE_STRING_PARAMETER_HELP( fileNameDW ) +
    		    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    		    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    		    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
