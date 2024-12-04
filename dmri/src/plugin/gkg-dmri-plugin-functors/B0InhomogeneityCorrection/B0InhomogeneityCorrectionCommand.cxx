#include <gkg-dmri-plugin-functors/B0InhomogeneityCorrection/B0InhomogeneityCorrectionCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>

#include <iomanip>
#include <cmath>


//
// class B0InhomogeneityCorrectionProcess
//

struct B0InhomogeneityCorrectionProcess : public gkg::Process
{

  public:
  
    B0InhomogeneityCorrectionProcess( 
         const std::string& theFileNamePhaseDifferenceVolume,
         const std::string& theFileNamePhaseDifferenceToDistortedTransform3d,
         const std::string& theFileNameUndistortedVolume,
         const std::string& theFileNameShiftVolume,
         const std::string& thePhaseAxis,
         const std::vector< float >& thePhaseToPixelFactorInformation,
         const bool& theAscii,
         const std::string& theFormat,
         const bool& theVerbose );

    const std::string& fileNamePhaseDifferenceVolume;
    const std::string& fileNamePhaseDifferenceToDistortedTransform3d;
    const std::string& fileNameUndistortedVolume;
    const std::string& fileNameShiftVolume;
    const std::string& phaseAxis;
    const std::vector< float >& phaseToPixelFactorInformation;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:
  
    template < class T >
    static void correct( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    template < class T >
    static void resample( const gkg::Volume< T >& distortedVolume,
                          const gkg::Volume< float >& shiftVolume,
                          const std::string& phaseAxis,
                          gkg::Volume< T >& undistortedVolume );

};


B0InhomogeneityCorrectionProcess::B0InhomogeneityCorrectionProcess( 
         const std::string& theFileNamePhaseDifferenceVolume,
         const std::string& theFileNamePhaseDifferenceToDistortedTransform3d,
         const std::string& theFileNameUndistortedVolume,
         const std::string& theFileNameShiftVolume,
         const std::string& thePhaseAxis,
         const std::vector< float >& thePhaseToPixelFactorInformation,
         const bool& theAscii,
         const std::string& theFormat,
         const bool& theVerbose )
                    : gkg::Process( "Volume" ),
                      fileNamePhaseDifferenceVolume(
                                             theFileNamePhaseDifferenceVolume ),
                      fileNamePhaseDifferenceToDistortedTransform3d(
                          theFileNamePhaseDifferenceToDistortedTransform3d ),
                      fileNameUndistortedVolume( theFileNameUndistortedVolume ),
                      fileNameShiftVolume( theFileNameShiftVolume ),
                      phaseAxis( thePhaseAxis ),
                      phaseToPixelFactorInformation(
                                             thePhaseToPixelFactorInformation ),
                      ascii( theAscii ),
                      format( theFormat ),
                      verbose( theVerbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &B0InhomogeneityCorrectionProcess::correct< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &B0InhomogeneityCorrectionProcess::correct< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &B0InhomogeneityCorrectionProcess::correct< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &B0InhomogeneityCorrectionProcess::correct< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &B0InhomogeneityCorrectionProcess::correct< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &B0InhomogeneityCorrectionProcess::correct< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &B0InhomogeneityCorrectionProcess::correct< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &B0InhomogeneityCorrectionProcess::correct< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &B0InhomogeneityCorrectionProcess::correct< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &B0InhomogeneityCorrectionProcess::correct< double > );

}


template < class T >
void B0InhomogeneityCorrectionProcess::correct( gkg::Process& process,
                                                const std::string& fileNameIn,
                                                const gkg::AnalyzedObject&,
                                                const std::string& )
{

  try
  {

    B0InhomogeneityCorrectionProcess&
      correctionProcess = static_cast< B0InhomogeneityCorrectionProcess& >(
                                                                      process );


    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////
    if ( !( ( correctionProcess.phaseToPixelFactorInformation.size() == 1U ) ||
            ( correctionProcess.phaseToPixelFactorInformation.size() == 6U ) ) )
    {

      throw std::runtime_error(
        "phase to pixel factor or the list of parameters required to build it "
        "is mandatory" );

    }

    if ( !( ( correctionProcess.phaseAxis == "x" ) ||
            ( correctionProcess.phaseAxis == "y" ) ||
            ( correctionProcess.phaseAxis == "z" ) ) )
    {

      throw std::runtime_error( "bad phase axis" );

    }
            

    ////////////////////////////////////////////////////////////////////////////
    // reading distorted volume proxy
    ////////////////////////////////////////////////////////////////////////////

    if ( correctionProcess.verbose )
    {

      std::cout << "reading input distorted volume '"
                << fileNameIn << "' : " << std::flush;

    }
    std::string format;
    gkg::VolumeProxy< T > distortedVolumeProxy;
    format = gkg::Reader::getInstance().template preparePartialBinaryRead<
                                                      gkg::Volume< T > > (
                                             fileNameIn, distortedVolumeProxy );
    if ( correctionProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading phase difference volume
    ////////////////////////////////////////////////////////////////////////////

    if ( correctionProcess.verbose )
    {

      std::cout << "reading phase difference map '"
                << correctionProcess.fileNamePhaseDifferenceVolume
                << "' : " << std::flush;

    }
    gkg::Volume< float > phaseDifferenceVolume;
    gkg::Reader::getInstance().read(
                                correctionProcess.fileNamePhaseDifferenceVolume,
                                phaseDifferenceVolume );
    if ( correctionProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading the phase difference -> distorted volume 3D transform
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::Transform3d< float > > transform3d;

    if ( correctionProcess.fileNamePhaseDifferenceToDistortedTransform3d.
                                                                       empty() )
    {

      if ( correctionProcess.verbose )
      {

        std::cout << "phase difference -> distorted 3D transform set to id"
                  << std::flush;

      }
      transform3d.reset( new gkg::IdentityTransform3d< float > );
      if ( correctionProcess.verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( correctionProcess.verbose )
      {

        std::cout << "reading phase difference -> distorted 3D transform : "
                  << std::flush;

      }
      gkg::RigidTransform3d< float >*
        rigidTransform3d = new gkg::RigidTransform3d< float >;
      std::ifstream 
        is( correctionProcess.
                        fileNamePhaseDifferenceToDistortedTransform3d.c_str() );
      rigidTransform3d->readTrm( is );
      is.close();
      transform3d.reset( rigidTransform3d );
      if ( correctionProcess.verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // phase to pixel factor
    ////////////////////////////////////////////////////////////////////////////

    float phaseToPixelFactor = 1.0;
    if ( correctionProcess.phaseToPixelFactorInformation.size() == 1U )
    {

      phaseToPixelFactor = correctionProcess.phaseToPixelFactorInformation[ 0 ];


    }
    else if ( correctionProcess.phaseToPixelFactorInformation.size() == 6U )
    {

      float epiPhaseLineCount = 
                           correctionProcess.phaseToPixelFactorInformation[ 0 ];
      float partialFourierFactor = 
                           correctionProcess.phaseToPixelFactorInformation[ 1 ];
      float parallelAccelerationFactor = 
                           correctionProcess.phaseToPixelFactorInformation[ 2 ];
      float echoSpacing = 
                           correctionProcess.phaseToPixelFactorInformation[ 3 ];
      float echoTimeDifference = 
                           correctionProcess.phaseToPixelFactorInformation[ 4 ];
      float phaseSign = correctionProcess.phaseToPixelFactorInformation[ 5 ];

      phaseToPixelFactor = ( epiPhaseLineCount * partialFourierFactor /
                             parallelAccelerationFactor ) *
                           ( echoSpacing / ( 2 * M_PI * echoTimeDifference ) ) *
                           phaseSign;
      
      if ( correctionProcess.verbose )
      {

        std::cout << "EPI phase line count : "
                  << epiPhaseLineCount
                  << std::endl;
        std::cout << "partial Fourier factor : "
                  << partialFourierFactor
                  << std::endl;
        std::cout << "parallel acceleration factor : "
                  << parallelAccelerationFactor
                  << std::endl;
        std::cout << "echo spacing : "
                  << echoSpacing
                  << std::endl;
        std::cout << "echo time difference of the fieldmap : "
                  << echoTimeDifference
                  << std::endl;
        std::cout << "sign of the factor (-1 or +1) : "
                  << phaseSign
                  << std::endl;

      }

    }

    if ( correctionProcess.verbose )
    {

      std::cout << "phase to pixel factor : " <<   phaseToPixelFactor
                << " voxel/rad"
                << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building shift volume in voxel unit
    ////////////////////////////////////////////////////////////////////////////

    if ( correctionProcess.verbose )
    {

      std::cout << "building shift volume : " << std::flush;

    }
    gkg::Vector3d< int32_t > distortedSize;
    int32_t distortedSizeT = 1;
    distortedVolumeProxy.getSize( distortedSize, distortedSizeT );

    gkg::Vector3d< double > distortedResolution;
    distortedVolumeProxy.getResolution( distortedResolution );

    gkg::Volume< float > shiftVolume( distortedSize );
    shiftVolume.setResolution( distortedResolution );

    gkg::Resampler< float >* resampler =
                gkg::ResamplerFactory< float >::getInstance().getResampler( 1 );
    resampler->resample( phaseDifferenceVolume,
                         *transform3d,
                         0.0f,
                         shiftVolume,
                         correctionProcess.verbose );

    gkg::Volume< float >::iterator s = shiftVolume.begin(),
                                   se = shiftVolume.end();
    while ( s != se )
    {

      *s *= phaseToPixelFactor;
      ++ s;

    }

    if ( correctionProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // saving shift volume
    ////////////////////////////////////////////////////////////////////////////

    if ( !correctionProcess.fileNameShiftVolume.empty() )
    {


      if ( correctionProcess.verbose )
      {

        std::cout << "writing shift volume : " << std::flush;

      }

      gkg::Writer::getInstance().write( correctionProcess.fileNameShiftVolume,
                                        shiftVolume,
                                        correctionProcess.ascii,
                                        correctionProcess.format );

      if ( correctionProcess.verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // preparing a bounding box and volume(s) for partial read
    ////////////////////////////////////////////////////////////////////////////

    if ( correctionProcess.verbose )
    {

      std::cout << "preparing partial read and write: " << std::flush;

    }
    gkg::BoundingBox< int32_t > boundingBox( 0, distortedSize.x - 1,
                                             0, distortedSize.y - 1,
                                             0, distortedSize.z - 1,
                                             0, 0 );
    gkg::Volume< T > distortedVolume;

    // preparing partial write proxy
    gkg::VolumeProxy< T > undistortedVolumeProxy( distortedSize,
                                                  distortedSizeT );
    undistortedVolumeProxy.getHeader() = distortedVolumeProxy.getHeader();
    gkg::Volume< T > undistortedVolume( distortedSize );


    if ( gkg::Writer::getInstance().template preparePartialBinaryWrite< 
                gkg::Volume< T > >( correctionProcess.fileNameUndistortedVolume,
                                    undistortedVolumeProxy,
                                    format ) != format )
    {

      throw std::runtime_error(
              std::string( "failed to prepare partial write with format \' " ) +
              format + "\'" );

    }

    if ( correctionProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // looping over ranks(s) and undistorting volume(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( correctionProcess.verbose )
    {

      std::cout << "correcting: " << std::flush;

    }

    int32_t t;
    for ( t = 0; t < distortedSizeT; t++ )
    {

      if ( correctionProcess.verbose )
      {

        if ( t != 0 )
        {

          std::cout << gkg::Eraser( 18 );

        }
        std::cout << "rank[ " << std::setw( 3 ) << t + 1 
                  << " / " << std::setw( 3 ) << distortedSizeT
                  << " ] " << std::flush;

      }

      boundingBox.setLowerT( t );
      boundingBox.setUpperT( t );

      // reading input volume at rank t
      if ( correctionProcess.verbose )
      {

        std::cout << "reading from disk" << std::flush;

      }
      if ( gkg::Reader::getInstance().template partialBinaryRead<
                                         gkg::Volume< T > >(
                                                           distortedVolumeProxy,
                                                           distortedVolume,
                                                           boundingBox,
                                                           format ) != format )
      {

        throw std::runtime_error( std::string( "wrong format, should be '" ) +
                                  format + "'" );

      }
      if ( correctionProcess.verbose )
      {

        std::cout << gkg::Eraser( 17 ) << std::flush;

      }

      // resampling 
      if ( correctionProcess.verbose )    
      {

        std::cout << "resampling slices" << std::flush;

      }
      resample( distortedVolume,
                shiftVolume,
                correctionProcess.phaseAxis,
                undistortedVolume );
      if ( correctionProcess.verbose )
      {

        std::cout << gkg::Eraser( 17 ) << std::flush;

      }

      // writing corrected data
      if ( correctionProcess.verbose )
      {

        std::cout << "writing to disk" << std::flush;

      }
      gkg::Writer::getInstance().template partialBinaryWrite< gkg::Volume< T > >(
                                                         undistortedVolumeProxy,
                                                         undistortedVolume,
                                                         boundingBox,
                                                         format );
      if ( correctionProcess.verbose )
      {

        std::cout << gkg::Eraser( 15 ) << std::flush;

      }

    }
    if ( correctionProcess.verbose )
    {

      std::cout << gkg::Eraser( 18 ) << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void B0InhomogeneityCorrectionProcess::correct( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


template < class T >
void B0InhomogeneityCorrectionProcess::resample(
                                        const gkg::Volume< T >& distortedVolume,
                                        const gkg::Volume< float >& shiftVolume,
                                        const std::string& phaseAxis,
                                        gkg::Volume< T >& undistortedVolume )
{

  try
  {

    int32_t sizeX = distortedVolume.getSizeX();
    int32_t sizeY = distortedVolume.getSizeY();
    int32_t sizeZ = distortedVolume.getSizeZ();

    int32_t x, y, z;
    
    if ( phaseAxis == "y" )
    {
    
      float yOrigReal, weight;
      int32_t y1, y2;
      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            yOrigReal = ( float )y - shiftVolume( x, y, z );

            y1 = ( int32_t )std::floor( yOrigReal );
            y2 = y1 + 1;
            weight = yOrigReal - ( float )y1;

            if ( ( y1 < 0 ) || ( y2 > sizeY - 1 ) )
            {

              undistortedVolume( x, y, z ) = ( T )0;

            }
            else
            {

              undistortedVolume( x, y, z ) =
                         ( T )( ( 1.0 - weight ) * distortedVolume( x, y1, z ) +
                                weight * distortedVolume( x, y2, z ) );

            }

          }

        }
        
      }

    }
    else if ( phaseAxis == "x" )
    {
    
      float xOrigReal, weight;
      int32_t x1, x2;
      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            xOrigReal = ( float )x - shiftVolume( x, y, z );

            x1 = ( int32_t )std::floor( xOrigReal );
            x2 = x1 + 1;
            weight = xOrigReal - ( float )x1;

            if ( ( x1 < 0 ) || ( x2 > sizeX - 1 ) )
            {

              undistortedVolume( x, y, z ) = ( T )0;

            }
            else
            {

              undistortedVolume( x, y, z ) =
                         ( T )( ( 1.0 - weight ) * distortedVolume( x1, y, z ) +
                                weight * distortedVolume( x2, y, z ) );

            }

          }

        }
        
      }

    }
    else if ( phaseAxis == "z" )
    {
    
      float zOrigReal, weight;
      int32_t z1, z2;
      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            zOrigReal = ( float )z - shiftVolume( x, y, z );

            z1 = ( int32_t )std::floor( zOrigReal );
            z2 = z1 + 1;
            weight = zOrigReal - ( float )z1;

            if ( ( z1 < 0 ) || ( z2 > sizeZ - 1 ) )
            {

              undistortedVolume( x, y, z ) = ( T )0;

            }
            else
            {

              undistortedVolume( x, y, z ) =
                        ( T )( ( 1.0 - weight ) * distortedVolume( x, y, z1 ) +
                               weight * distortedVolume( x, y, z2 ) );

            }

          }

        }
        
      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void B0InhomogeneityCorrectionProcess::resample( "
             "const gkg::Volume< T >& distortedVolume, "
             "const gkg::Volume< float >& shiftVolume, "
             "const std::string& phaseAxis, "
             "gkg::Volume< T >& undistortedVolume )" );

}



//
// class B0InhomogeneityCorrectionCommand
//

gkg::B0InhomogeneityCorrectionCommand::B0InhomogeneityCorrectionCommand(
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
  GKG_CATCH( "gkg::B0InhomogeneityCorrectionCommand::"
             "B0InhomogeneityCorrectionCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::B0InhomogeneityCorrectionCommand::B0InhomogeneityCorrectionCommand(
               const std::string& fileNameDistortedVolume,
               const std::string& fileNamePhaseDifferenceVolume,
               const std::string& fileNamePhaseDifferenceToDistortedTransform3d,
               const std::string& fileNameUndistortedVolume,
               const std::string& fileNameShiftVolume,
               const std::string& phaseAxis,
               const std::vector< float >& phaseToPixelFactorInformation,
               bool ascii,
               const std::string& format,
               bool verbose )
                                      : gkg::Command()
{

  try
  {

    execute( fileNameDistortedVolume,
             fileNamePhaseDifferenceVolume,
             fileNamePhaseDifferenceToDistortedTransform3d,
             fileNameUndistortedVolume,
             fileNameShiftVolume,
             phaseAxis,
             phaseToPixelFactorInformation,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::B0InhomogeneityCorrectionCommand::"
             "B0InhomogeneityCorrectionCommand( "
             "const std::string& fileNameDistortedVolume, "
             "const std::string& fileNamePhaseDifferenceVolume, "
             "const std::string& "
             "fileNamePhaseDifferenceToDistortedTransform3d, "
             "const std::string& fileNameUndistortedVolume, "
             "const std::string& fileNameShiftVolume, "
             "const std::string& phaseAxis, "
             "const std::vector< float >& phaseToPixelFactorInformation, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


gkg::B0InhomogeneityCorrectionCommand::B0InhomogeneityCorrectionCommand(
                                             const gkg::Dictionary& parameters )
                                      : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameDistortedVolume );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNamePhaseDifferenceVolume );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNamePhaseDifferenceToDistortedTransform3d );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameUndistortedVolume );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameShiftVolume );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              phaseAxis );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           phaseToPixelFactorInformation );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameDistortedVolume,
             fileNamePhaseDifferenceVolume,
             fileNamePhaseDifferenceToDistortedTransform3d,
             fileNameUndistortedVolume,
             fileNameShiftVolume,
             phaseAxis,
             phaseToPixelFactorInformation,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::B0InhomogeneityCorrectionCommand::"
             "B0InhomogeneityCorrectionCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::B0InhomogeneityCorrectionCommand::~B0InhomogeneityCorrectionCommand()
{
}


std::string gkg::B0InhomogeneityCorrectionCommand::getStaticName()
{

  try
  {

    return "B0InhomogeneityCorrection";

  }
  GKG_CATCH( "std::string gkg::B0InhomogeneityCorrectionCommand::getStaticName()" );

}


void gkg::B0InhomogeneityCorrectionCommand::parse()
{

  try
  {

    std::string fileNameDistortedVolume;
    std::string fileNamePhaseDifferenceVolume;
    std::string fileNamePhaseDifferenceToDistortedTransform3d;
    std::string fileNameUndistortedVolume;
    std::string fileNameShiftVolume;
    std::string phaseAxis = "y";
    std::vector< float > phaseToPixelFactorInformation;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Correction of distortions due to "
                                  "B0 inhomogeneities (susceptibility "
                                  "artifacts, bad shim, ...)",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input distorted volume",
                                 fileNameDistortedVolume );
    application.addSingleOption( "-p",
                                 "Input float phase difference volume",
                                 fileNamePhaseDifferenceVolume );
    application.addSingleOption( "-t",
                                 "Phase difference map to distorted data "
                                 "3D transform [default=identity]",
                                 fileNamePhaseDifferenceToDistortedTransform3d,
                                 true );
    application.addSingleOption( "-o",
                                 "Output undistorted volume",
                                 fileNameUndistortedVolume );
    application.addSingleOption( "-s",
                                 "Output shift volume in voxel coordinates",
                                 fileNameShiftVolume,
                                 true );
    application.addSingleOption( "-phaseAxis",
                                 "Phase encoding axis of the acquisition "
                                 "x/y/z [default=y]",
                                 phaseAxis,
                                 true );
    application.addSeriesOption( "-parameters",
                                 "Phase to pixel factor parameters; two "
                                 "possible prescritions:\n"
                                 "1) directly give the phase to pixel factor\n"
                                 "2) give in this order the parameters:\n"
                                 "   - EPI phase line count\n"
                                 "   - partial Fourier factor\n"
                                 "   - parallel acceleration factor\n"
                                 "   - echo spacing\n"
                                 "   - echo time difference of the fieldmap\n"
                                 "   - sign of the factor (-1 or +1)",
                                 phaseToPixelFactorInformation,
                                 1 );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();

    execute( fileNameDistortedVolume,
             fileNamePhaseDifferenceVolume,
             fileNamePhaseDifferenceToDistortedTransform3d,
             fileNameUndistortedVolume,
             fileNameShiftVolume,
             phaseAxis,
             phaseToPixelFactorInformation,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::B0InhomogeneityCorrectionCommand::parse()" );

}


void gkg::B0InhomogeneityCorrectionCommand::execute(
               const std::string& fileNameDistortedVolume,
               const std::string& fileNamePhaseDifferenceVolume,
               const std::string& fileNamePhaseDifferenceToDistortedTransform3d,
               const std::string& fileNameUndistortedVolume,
               const std::string& fileNameShiftVolume,
               const std::string& phaseAxis,
               const std::vector< float >& phaseToPixelFactorInformation,
               bool ascii,
               const std::string& format,
               bool verbose )
 {

  try
  {

    //
    // launching B0 inhomogeneity correction process
    //
    B0InhomogeneityCorrectionProcess correctionProcess(
                               fileNamePhaseDifferenceVolume,
                               fileNamePhaseDifferenceToDistortedTransform3d,
                               fileNameUndistortedVolume,
                               fileNameShiftVolume,
                               phaseAxis,
                               phaseToPixelFactorInformation,
                               ascii,
                               format,
                               verbose );
    correctionProcess.execute( fileNameDistortedVolume );

  }
  GKG_CATCH( "void gkg::B0InhomogeneityCorrectionCommand::execute( "
             "const std::string& fileNameDistortedVolume, "
             "const std::string& fileNamePhaseDifferenceVolume, "
             "const std::string& "
             "fileNamePhaseDifferenceToDistortedTransform3d, "
             "const std::string& fileNameUndistortedVolume, "
             "const std::string& fileNameShiftVolume, "
             "const std::string& phaseAxis, "
             "const std::vector< float >& phaseToPixelFactorInformation, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    B0InhomogeneityCorrectionCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameDistortedVolume ) +
    DECLARE_STRING_PARAMETER_HELP( fileNamePhaseDifferenceVolume ) +
    DECLARE_STRING_PARAMETER_HELP(
                               fileNamePhaseDifferenceToDistortedTransform3d ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameUndistortedVolume ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameShiftVolume ) +
    DECLARE_STRING_PARAMETER_HELP( phaseAxis ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP(
                                               phaseToPixelFactorInformation ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_STRING_PARAMETER_HELP( format ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
