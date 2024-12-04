#include <gkg-processing-io/MaskReaderProcess.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::MaskReaderProcess::MaskReaderProcess( gkg::Volume< int8_t >& theMask,
                                           const std::string& theMode,
                                           const double& theThreshold1,
                                           const double& theThreshold2,
                                           bool theVerbose )
                       : gkg::Process( "Volume" ),
                         mask( theMask ),
                         mode( theMode ),
                         threshold1( theThreshold1 ),
                         threshold2( theThreshold2 ),
                         verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &MaskReaderProcess::read< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &MaskReaderProcess::read< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &MaskReaderProcess::read< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &MaskReaderProcess::read< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &MaskReaderProcess::read< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &MaskReaderProcess::read< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &MaskReaderProcess::read< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &MaskReaderProcess::read< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &MaskReaderProcess::read< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &MaskReaderProcess::read< double > );

  }
  GKG_CATCH( "gkg::MaskReaderProcess::MaskReaderProcess( "
             "gkg::Volume< int8_t >& theMask, "
             "const std::string& theMode, "
             "const double& theThreshold1, "
             "const double& theThreshold2, "
             "bool theVerbose )" );

}


template < class M >
void gkg::MaskReaderProcess::read( gkg::Process& process,
                                   const std::string& fileNameMask,
                                   const gkg::AnalyzedObject&,
                                   const std::string& )
{

  try
  {

    gkg::MaskReaderProcess&
      maskReaderProcess = static_cast< gkg::MaskReaderProcess& >( process );

    gkg::Volume< int8_t >& mask = maskReaderProcess.mask;
    const std::string& mode = maskReaderProcess.mode;
    const double& threshold1 = maskReaderProcess.threshold1;
    const double& threshold2 = maskReaderProcess.threshold2;
    const bool& verbose = maskReaderProcess.verbose;

    // reading mask
    gkg::Volume< M > maskVolume;

    if ( verbose )
    {

      std::cout << "reading '" << fileNameMask << "' : " << std::flush;

    }
    gkg::Reader::getInstance().read( fileNameMask, maskVolume );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "building int8_t mask : " << std::flush;

    }
    gkg::TestFunction< M >* testFunction = 
        gkg::TestFunctionFactory< M >::getInstance().createTestFunction( 
                                                              mode,
                                                              ( M )threshold1,
                                                              ( M )threshold2 );
    int32_t sizeX = maskVolume.getSizeX();
    int32_t sizeY = maskVolume.getSizeY();
    int32_t sizeZ = maskVolume.getSizeZ();
    int32_t sizeT = maskVolume.getSizeT();
    double resolutionX = 1.0;
    double resolutionY = 1.0;
    double resolutionZ = 1.0;
    double resolutionT = 1.0;
    if ( maskVolume.getHeader().hasAttribute( "resolutionX" ) )
    {

      maskVolume.getHeader().getAttribute( "resolutionX", resolutionX );

    }
    if ( maskVolume.getHeader().hasAttribute( "resolutionY" ) )
    {

      maskVolume.getHeader().getAttribute( "resolutionY", resolutionY );

    }
    if ( maskVolume.getHeader().hasAttribute( "resolutionZ" ) )
    {

      maskVolume.getHeader().getAttribute( "resolutionZ", resolutionZ );

    }
    if ( maskVolume.getHeader().hasAttribute( "resolutionT" ) )
    {

      maskVolume.getHeader().getAttribute( "resolutionT", resolutionT );

    }
    mask = gkg::Volume< int8_t >( sizeX, sizeY, sizeZ, sizeT );
    mask.getHeader().addAttribute( "resolutionX", resolutionX);
    mask.getHeader().addAttribute( "resolutionY", resolutionY);
    mask.getHeader().addAttribute( "resolutionZ", resolutionZ);
    mask.getHeader().addAttribute( "resolutionT", resolutionT);

    typename gkg::Volume< M >::const_iterator i = maskVolume.begin(),
                                              ie = maskVolume.end();
    gkg::Volume< int8_t >::iterator o = mask.begin();
    while ( i != ie )
    {

      *o = ( testFunction->getState( *i ) ? 1 : 0 );
      ++ i;
      ++ o;

    }


    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class M > "
             "void gkg::MaskReaderProcess::read( gkg::Process& process, "
             "const std::string& fileNameMask, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


