#include <gkg-processing-plugin-functors/PhaseUnwrapper/PhaseUnwrapperCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-signal/PhaseUnwrapperFactory.h>
#include <gkg-processing-signal/PhaseUnwrapperFunction.h>
#include <gkg-processing-signal/WFMGPhaseUnwrapper.h>
#include <gkg-processing-signal/PseudoCorrelation.h>
#include <gkg-processing-signal/ZUnwrapping.h>
#include <gkg-processing-signal/ZFloatUnwrapping.h>
#include <gkg-processing-algobase/Binarizer_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-numericalanalysis/Polynomial2dFitter.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>


//
//   PhaseUnwrapperProcess
//


void removeBias( gkg::Volume< float >& unwrappedPhaseDifference,
                 const gkg::Volume< float >& /* qualityMap */,
                 const gkg::Volume< int16_t >& mask,
                 const std::string& fileNameBias,
                 int32_t maximumFitOrder,
                 int32_t maximumBiasOrder,
                 bool verbose )
{

  try
  {

    int32_t x, y, z;

    int32_t sizeX = unwrappedPhaseDifference.getSizeX();
    int32_t sizeY = unwrappedPhaseDifference.getSizeY();
    int32_t sizeZ = unwrappedPhaseDifference.getSizeZ();

    bool saveBias = !fileNameBias.empty();
    gkg::Volume< float > bias;

    if ( saveBias )
    {

      bias = unwrappedPhaseDifference;
      bias.fill( 0.0 );

    }

    for ( z = 0; z < sizeZ; z++ )
    {

      if ( verbose )
      {

        if ( z != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << z + 1 
                  << " / " << std::setw( 4 ) << sizeZ
                  << " ]" << std::flush;

      }
      int32_t siteCount = 0;
      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( mask( x, y, z ) )
          {

            ++ siteCount;

          }

        }

      }
      gkg::Vector X( siteCount );
      gkg::Vector Y( siteCount );
      gkg::Vector W( siteCount );
      gkg::Vector F( siteCount );

      int32_t n = 0;
      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( mask( x, y, z ) )
          {

            X( n ) = ( double )x / ( double )sizeX;
            Y( n ) = ( double )y / ( double )sizeY;
            W( n ) = ( double )1.0;//qualityMap( x, y, z );
            F( n ) = ( double )unwrappedPhaseDifference( x, y, z );
            ++ n;

          }

        }

      }
      gkg::Polynomial2dFitter polynomial2dFitter( X, Y, W, F, maximumFitOrder );
      float biasValue;
      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( mask( x, y, z ) )
          {

            biasValue = 
                 polynomial2dFitter.getValueAt( ( double )x / ( double )sizeX,
                                                ( double )y / ( double )sizeY,
                                                maximumBiasOrder );
            if ( saveBias )
            {

              bias( x, y, z ) = biasValue;

            }
            unwrappedPhaseDifference( x, y, z ) -= biasValue;

          }
          else
          {

            unwrappedPhaseDifference( x, y, z ) = 0.0;

          }

        }

      }

    }

    if ( saveBias )
    {

      gkg::Writer::getInstance().write( fileNameBias, bias );

    }

  }
  GKG_CATCH( "void removeBias( "
             "gkg::Volume< float >& unwrappedPhaseDifference, "
             "const gkg::Volume< float >& qualityMap, "
             "const gkg::Volume< int16_t >& mask, "
             "const std::string& fileNameBias, "
             "int32_t maximumFitOrder, "
             "int32_t maximumBiasOrder, "
             "bool verbose )" );

}


class PhaseUnwrapperProcess : public gkg::Process
{

  public:

    PhaseUnwrapperProcess( const std::string& fileNameOut,
                           const std::string& fileNameMask,
                           const std::string& fileNameQuality,
                           const double& offset,
                           const double& scale,
                           const std::string& planeAxis,
                           const int32_t& cycleCount,
                           const int32_t& coarsestSize,
                           const int32_t& preSmoothingIterationCount,
                           const int32_t& postSmoothingIterationCount,
                           const bool& useCongruence,
                           const float& qualityThreshold,
                           const bool& removeRampFlag,
                           const bool& ascii,
                           const std::string& format,
                           const bool& applyMaskToOutput,
                           const bool& removeBias,
                           const std::string& fileNameBias,
                           const int32_t& maximumFitOrder,
                           const int32_t& maximumBiasOrder,
                           const int32_t& zUnwrappingType,
                           const bool& verbose );

    const std::string& getFileNameOut() const;
    const std::string& getFileNameMask() const;
    const std::string& getFileNameQuality() const;
    const double& getOffset() const;
    const double& getScale() const;
    const std::string& getPlaneAxis() const;
    const int32_t& getCycleCount() const;
    const int32_t& getCoarsestSize() const;
    const int32_t& getPreSmoothingIterationCount() const;
    const int32_t& getPostSmoothingIterationCount() const;
    const bool& getUseCongruence() const;
    const float& getQualityThreshold() const;
    const bool& getRemoveRampFlag() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getApplyMaskToOutput() const;
    const bool& getRemoveBias() const;
    const std::string& getFileNameBias() const;
    const int32_t& getMaximumFitOrder() const;
    const int32_t& getMaximumBiasOrder() const;
    const int32_t& getZUnwrappingType() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void unwrap( gkg::Process& process,
                        const std::string& fileNameIn,
                        const gkg::AnalyzedObject&,
                        const std::string& );

    const std::string& _fileNameOut;
    const std::string& _fileNameMask;
    const std::string& _fileNameQuality;
    const double& _offset;
    const double& _scale;
    const std::string& _planeAxis;
    const int32_t& _cycleCount;
    const int32_t& _coarsestSize;
    const int32_t& _preSmoothingIterationCount;
    const int32_t& _postSmoothingIterationCount;
    const bool& _useCongruence;
    const float& _qualityThreshold;
    const bool& _removeRampFlag;
    const bool& _ascii;
    const std::string& _format;
    const bool& _applyMaskToOutput;
    const bool& _removeBias;
    const std::string& _fileNameBias;
    const int32_t& _maximumFitOrder;
    const int32_t& _maximumBiasOrder;
    const int32_t& _zUnwrappingType;
    const bool& _verbose;

};


PhaseUnwrapperProcess::PhaseUnwrapperProcess(
                                     const std::string& fileNameOut,
                                     const std::string& fileNameMask,
                                     const std::string& fileNameQuality,
                                     const double& offset,
                                     const double& scale,
                                     const std::string& planeAxis,
                                     const int32_t& cycleCount,
                                     const int32_t& coarsestSize,
                                     const int32_t& preSmoothingIterationCount,
                                     const int32_t& postSmoothingIterationCount,
                                     const bool& useCongruence,
                                     const float& qualityThreshold,
                                     const bool& removeRampFlag,
                                     const bool& ascii,
                                     const std::string& format,
                                     const bool& applyMaskToOutput,
                                     const bool& removeBias,
                                     const std::string& fileNameBias,
                                     const int32_t& maximumFitOrder,
                                     const int32_t& maximumBiasOrder,
                                     const int32_t& zUnwrappingType,
                                     const bool& verbose)
                      : gkg::Process( "Volume" ),
                        _fileNameOut( fileNameOut ),
                        _fileNameMask( fileNameMask ),
                        _fileNameQuality( fileNameQuality ),
                        _offset( offset ),
                        _scale( scale ),
                        _planeAxis( planeAxis ),
                        _cycleCount( cycleCount ),
                        _coarsestSize( coarsestSize ),
                        _preSmoothingIterationCount(
                                                   preSmoothingIterationCount ),
                        _postSmoothingIterationCount(
                                                  postSmoothingIterationCount ),
                        _useCongruence( useCongruence ),
                        _qualityThreshold( qualityThreshold ),
                        _removeRampFlag( removeRampFlag ),
                        _ascii( ascii ),
                        _format( format ),
                        _applyMaskToOutput( applyMaskToOutput ),
                        _removeBias( removeBias ),
                        _fileNameBias( fileNameBias ),
                        _maximumFitOrder( maximumFitOrder ),
                        _maximumBiasOrder( maximumBiasOrder ),
                        _zUnwrappingType( zUnwrappingType ),
                        _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &PhaseUnwrapperProcess::unwrap< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &PhaseUnwrapperProcess::unwrap< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &PhaseUnwrapperProcess::unwrap< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &PhaseUnwrapperProcess::unwrap< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &PhaseUnwrapperProcess::unwrap< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &PhaseUnwrapperProcess::unwrap< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &PhaseUnwrapperProcess::unwrap< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &PhaseUnwrapperProcess::unwrap< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &PhaseUnwrapperProcess::unwrap< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &PhaseUnwrapperProcess::unwrap< double > );

}


const std::string& PhaseUnwrapperProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& PhaseUnwrapperProcess::getFileNameMask() const
{

  return _fileNameMask;

}


const std::string& PhaseUnwrapperProcess::getFileNameQuality() const
{

  return _fileNameQuality;

}


const double& PhaseUnwrapperProcess::getOffset() const
{

  return _offset;

}


const double& PhaseUnwrapperProcess::getScale() const
{

  return _scale;

}


const std::string& PhaseUnwrapperProcess::getPlaneAxis() const
{

  return _planeAxis;

}


const int32_t& PhaseUnwrapperProcess::getCycleCount() const
{

  return _cycleCount;

}


const int32_t& PhaseUnwrapperProcess::getCoarsestSize() const
{

  return _coarsestSize;

}


const int32_t& PhaseUnwrapperProcess::getPreSmoothingIterationCount() const
{

  return _preSmoothingIterationCount;

}


const int32_t& PhaseUnwrapperProcess::getPostSmoothingIterationCount() const
{

  return _postSmoothingIterationCount;

}


const bool& PhaseUnwrapperProcess::getUseCongruence() const
{

  return _useCongruence;

}


const float& PhaseUnwrapperProcess::getQualityThreshold() const
{

  return _qualityThreshold;

}


const bool& PhaseUnwrapperProcess::getRemoveRampFlag() const
{

  return _removeRampFlag;

}


const bool& PhaseUnwrapperProcess::getAscii() const
{

  return _ascii;

}


const std::string& PhaseUnwrapperProcess::getFormat() const
{

  return _format;

}


const bool& PhaseUnwrapperProcess::getApplyMaskToOutput() const
{

  return _applyMaskToOutput;

}


const bool& PhaseUnwrapperProcess::getRemoveBias() const
{

  return _removeBias;

}


const std::string& PhaseUnwrapperProcess::getFileNameBias() const
{

  return _fileNameBias;

}


const int32_t& PhaseUnwrapperProcess::getMaximumFitOrder() const
{

  return _maximumFitOrder;

}


const int32_t& PhaseUnwrapperProcess::getMaximumBiasOrder() const
{

  return _maximumBiasOrder;

}


const int32_t& PhaseUnwrapperProcess::getZUnwrappingType() const
{

  return _zUnwrappingType;

}


const bool& PhaseUnwrapperProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void PhaseUnwrapperProcess::unwrap( gkg::Process& process,
                                    const std::string& fileNameIn,
                                    const gkg::AnalyzedObject&,
                                    const std::string& )
{

  try
  {

    PhaseUnwrapperProcess&
      phaseUnwrapperProcess = static_cast< PhaseUnwrapperProcess& >( process );

    ////////////////////////////////////////////////////////////////////////////
    // reading data
    ////////////////////////////////////////////////////////////////////////////
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading mask
    ////////////////////////////////////////////////////////////////////////////
    gkg::Volume< int16_t > mask;
    if ( !phaseUnwrapperProcess.getFileNameMask().empty() )
    {

      if ( phaseUnwrapperProcess.getVerbose() )
      {

        std::cout << "reading '" << phaseUnwrapperProcess.getFileNameMask()
                  << "' : " << std::flush;

      }
      gkg::Reader::getInstance().read( phaseUnwrapperProcess.getFileNameMask(),
                                       mask );
      if ( phaseUnwrapperProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      mask.reallocate( volumeIn.getSizeX(),
                       volumeIn.getSizeY(),
                       volumeIn.getSizeZ() );
      mask.fill( 1 );

    }

    ////////////////////////////////////////////////////////////////////////////
    // converting data to float
    ////////////////////////////////////////////////////////////////////////////
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "converting to floating type : " << std::flush;

    }
    gkg::Converter< gkg::Volume< T >, gkg::Volume< float > > converter;
    gkg::Volume< float > volumeOut;
    converter.convert( volumeIn, volumeOut );
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // scaling and adding offset
    ////////////////////////////////////////////////////////////////////////////
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "scaling and adding offset : " << std::flush;

    }
    float offset = phaseUnwrapperProcess.getOffset();
    float scale = phaseUnwrapperProcess.getScale();
    gkg::Volume< float >::iterator i = volumeOut.begin(),
                                   ie = volumeOut.end();
    while ( i != ie )
    {

      *i = offset + scale * ( *i );
      ++ i;

    }
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // processing quality map
    ////////////////////////////////////////////////////////////////////////////
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "processing quality map : " << std::flush;

    }
    gkg::Volume< float > qualityMap( volumeOut.getSizeX(),
                                     volumeOut.getSizeY(),
                                     volumeOut.getSizeZ() );
    qualityMap.getHeader() = volumeOut.getHeader();
    qualityMap.getHeader()[ "sizeT" ] = 1;
    qualityMap.getHeader()[ "item_type" ] = gkg::TypeOf<float>::getName();                             
    gkg::PseudoCorrelation( volumeOut, qualityMap, 3 );
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing quality map
    ////////////////////////////////////////////////////////////////////////////
    if ( !phaseUnwrapperProcess.getFileNameQuality().empty() )
    {

      if ( phaseUnwrapperProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << phaseUnwrapperProcess.getFileNameQuality() << "' : "
                  << std::flush;

      }
      gkg::Writer::getInstance().write( 
                                     phaseUnwrapperProcess.getFileNameQuality(),
                                     qualityMap,
                                     phaseUnwrapperProcess.getAscii(),
                                     phaseUnwrapperProcess.getFormat() );
      if ( phaseUnwrapperProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // unwrapping in-plane data
    ////////////////////////////////////////////////////////////////////////////
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "unwrapping in-plane phase : " << std::flush;

    }
    gkg::PhaseUnwrapper< float >*
      phaseUnwrapper = gkg::PhaseUnwrapperFactory< float >::getInstance().
        createPhaseUnwrapper( "weighted-full-multigrid-phase-unwrapper" );

    gkg::WFMGPhaseUnwrapper< float >*
      theWFMGPhaseUnwrapper = static_cast< gkg::WFMGPhaseUnwrapper< float >*>(
                                                               phaseUnwrapper );
    if ( phaseUnwrapperProcess.getPlaneAxis() == "z" )
    {

      theWFMGPhaseUnwrapper->setPlaneAxis(
                                          gkg::WFMGPhaseUnwrapper< float >::Z );

    }
    else if ( phaseUnwrapperProcess.getPlaneAxis() == "y" )
    {

      theWFMGPhaseUnwrapper->setPlaneAxis(
                                          gkg::WFMGPhaseUnwrapper< float >::Y );

    }
    else if ( phaseUnwrapperProcess.getPlaneAxis() == "x" )
    {

      theWFMGPhaseUnwrapper->setPlaneAxis(
                                          gkg::WFMGPhaseUnwrapper< float >::X );

    }
    else
    {

      throw std::runtime_error( "bad plane axis" );

    }
    theWFMGPhaseUnwrapper->setCycleCount(
                       phaseUnwrapperProcess.getCycleCount() );
    theWFMGPhaseUnwrapper->setCoarsestSize(
                       phaseUnwrapperProcess.getCoarsestSize() );
    theWFMGPhaseUnwrapper->setPreSmoothingIterationCount(
                       phaseUnwrapperProcess.getPreSmoothingIterationCount() );
    theWFMGPhaseUnwrapper->setPostSmoothingIterationCount(
                       phaseUnwrapperProcess.getPostSmoothingIterationCount() );
    theWFMGPhaseUnwrapper->useCongruence(
                       phaseUnwrapperProcess.getUseCongruence() );

    phaseUnwrapper->unwrap( volumeOut, qualityMap,
                            phaseUnwrapperProcess.getVerbose() );
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // unwrapping phase along z axis
    ////////////////////////////////////////////////////////////////////////////
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "unwrapping inter-slice phase : " << std::flush;

    }
    int32_t referenceSlice = volumeOut.getSizeZ() / 2;

    if ( phaseUnwrapperProcess.getZUnwrappingType() == 1 )
    {

      gkg::ZUnwrapping( volumeOut,
                        qualityMap,
                        mask,
                        referenceSlice );

    }
    else if ( phaseUnwrapperProcess.getZUnwrappingType() == 2 )
    {

      gkg::ZFloatUnwrapping( volumeOut,
                             qualityMap,
                             mask,
                             referenceSlice );
    }
    else
    {

      throw std::runtime_error( "invalid z-unwrapping type" );

    }

    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // removing ramp(s)
    ////////////////////////////////////////////////////////////////////////////
    if ( phaseUnwrapperProcess.getRemoveRampFlag() )
    {

      if ( phaseUnwrapperProcess.getVerbose() )
      {

        std::cout << "removing ramp : " << std::flush;

      }
      gkg::removeRamp( volumeOut, qualityMap );
      if ( phaseUnwrapperProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // processing quality mask
    ////////////////////////////////////////////////////////////////////////////
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "processing quality mask : " << std::flush;

    }
    gkg::GreaterOrEqualToFunction< float > 
     greaterOrEqualToFunction( phaseUnwrapperProcess.getQualityThreshold() );
    gkg::Binarizer< gkg::Volume< float >, gkg::Volume< uint8_t > >
      binarizer( greaterOrEqualToFunction, 1, 0 );
    gkg::Volume< uint8_t > qualityMask( qualityMap.getSizeX(),
                                        qualityMap.getSizeY(),
                                        qualityMap.getSizeZ() );
    qualityMask.getHeader() = qualityMap.getHeader();
    qualityMask.getHeader()[ "item_type" ] = gkg::TypeOf< uint8_t >::getName();
    binarizer.binarize( qualityMap, qualityMask );
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // removing constant offset
    ////////////////////////////////////////////////////////////////////////////
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "removing constant phase offset : " << std::flush;

    }
    gkg::removeOffset( volumeOut, qualityMask );
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // removing bias
    ////////////////////////////////////////////////////////////////////////////
    if ( phaseUnwrapperProcess.getRemoveBias() )
    {

      if ( phaseUnwrapperProcess.getVerbose() )
      {

        std::cout << "removing bias : " << std::flush;

      }
      removeBias( volumeOut, qualityMap, mask,
                  phaseUnwrapperProcess.getFileNameBias(),
                  phaseUnwrapperProcess.getMaximumFitOrder(),
                  phaseUnwrapperProcess.getMaximumBiasOrder(),
                  phaseUnwrapperProcess.getVerbose() );
      if ( phaseUnwrapperProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // masking output volume
    ////////////////////////////////////////////////////////////////////////////
    if ( phaseUnwrapperProcess.getApplyMaskToOutput() )
    {

      if ( phaseUnwrapperProcess.getVerbose() )
      {

        std::cout << "masking output volume : " << std::flush;

      }
      gkg::Volume< int16_t >::const_iterator m = mask.begin();
      gkg::Volume< float >::iterator o = volumeOut.begin(),
                                     oe = volumeOut.end();
      while ( o != oe )
      {

        *o = ( *m ? *o : 0.0 );
        ++ o;
        ++ m;

      }
      if ( phaseUnwrapperProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // writing unwrapped data
    ////////////////////////////////////////////////////////////////////////////
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "writing '"
                << phaseUnwrapperProcess.getFileNameOut() << "' : "
                << std::flush;

    }
    gkg::Writer::getInstance().write( phaseUnwrapperProcess.getFileNameOut(),
                                      volumeOut,
                                      phaseUnwrapperProcess.getAscii(),
                                      phaseUnwrapperProcess.getFormat() );
    if ( phaseUnwrapperProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void PhaseUnwrapperProcess::unwrap( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   PhaseUnwrapperCommand
//


gkg::PhaseUnwrapperCommand::PhaseUnwrapperCommand( int32_t argc,
                                		   char* argv[],
                                		   bool loadPlugin,
                                		   bool removeFirst )
                          : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::PhaseUnwrapperCommand::PhaseUnwrapperCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::PhaseUnwrapperCommand::PhaseUnwrapperCommand(
                           		    const std::string& fileNameIn,
    			   		    const std::string& fileNameOut,
    			   		    const std::string& fileNameMask,
    			   		    const std::string& fileNameQuality,
    			   		    double offset,
    			   		    double scale,
    			   		    const std::string& planeAxis,
    			   		    int32_t cycleCount,
    			   		    int32_t coarsestSize,
    			   		    int32_t preSmoothingIterationCount,
    			   		    int32_t postSmoothingIterationCount,
    			   		    bool useCongruence,
    			   		    float qualityThreshold,
    			   		    bool removeRampFlag,
    			   		    bool ascii,
    			   		    const std::string& format,
    			   		    bool applyMaskToOutput,
    			   		    bool removeBias,
    			   		    const std::string& fileNameBias,
    			   		    int32_t maximumFitOrder,
    			   		    int32_t maximumBiasOrder,
    			   		    int32_t zUnwrappingType,
    			   		    bool verbose )
                          : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, fileNameMask, fileNameQuality, offset,
             scale, planeAxis, cycleCount, coarsestSize, 
             preSmoothingIterationCount, postSmoothingIterationCount, 
             useCongruence, qualityThreshold, removeRampFlag, ascii,
             format, applyMaskToOutput, removeBias, fileNameBias,
             maximumFitOrder, maximumBiasOrder, zUnwrappingType, verbose );

  }
  GKG_CATCH( "gkg::PhaseUnwrapperCommand::PhaseUnwrapperCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
    	     "const std::string& fileNameMask, "
    	     "const std::string& fileNameQuality, "
    	     "double offset, double scale, const std::string& planeAxis, "
    	     "int32_t cycleCount, int32_t coarsestSize, "
    	     "int32_t preSmoothingIterationCount,  "
             "int32_t postSmoothingIterationCount, bool useCongruence, "
    	     "float qualityThreshold, bool removeRampFlag, bool ascii, "
    	     "const std::string& format, bool applyMaskToOutput, "
    	     "bool removeBias, const std::string& fileNameBias, "
    	     "int32_t maximumFitOrder, int32_t maximumBiasOrder, "
    	     "int32_t zUnwrappingType, bool verbose )" );

}


gkg::PhaseUnwrapperCommand::PhaseUnwrapperCommand(
                                             const gkg::Dictionary& parameters )
                          : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameQuality );
    DECLARE_FLOATING_PARAMETER( parameters, double, offset );
    DECLARE_FLOATING_PARAMETER( parameters, double, scale );
    DECLARE_STRING_PARAMETER( parameters, std::string, planeAxis );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, cycleCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, coarsestSize );
    DECLARE_INTEGER_PARAMETER( parameters,
                               int32_t,
                               preSmoothingIterationCount );
    DECLARE_INTEGER_PARAMETER( parameters,
                               int32_t,
                               postSmoothingIterationCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, useCongruence );
    DECLARE_FLOATING_PARAMETER( parameters, float, qualityThreshold );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, removeRampFlag );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, applyMaskToOutput );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, removeBias );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameBias );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, maximumFitOrder );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, maximumBiasOrder );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, zUnwrappingType );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, fileNameMask, fileNameQuality, offset,
             scale, planeAxis, cycleCount, coarsestSize, 
             preSmoothingIterationCount, postSmoothingIterationCount, 
             useCongruence, qualityThreshold, removeRampFlag, ascii,
             format, applyMaskToOutput, removeBias, fileNameBias,
             maximumFitOrder, maximumBiasOrder, zUnwrappingType, verbose );

  }
  GKG_CATCH( "gkg::PhaseUnwrapperCommand::PhaseUnwrapperCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::PhaseUnwrapperCommand::~PhaseUnwrapperCommand()
{
}


std::string gkg::PhaseUnwrapperCommand::getStaticName()
{

  try
  {

    return "PhaseUnwrapper";

  }
  GKG_CATCH( "std::string gkg::PhaseUnwrapperCommand::getStaticName()" );

}


void gkg::PhaseUnwrapperCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string fileNameMask = "";
    std::string fileNameQuality = "";
    double offset = 0.0;
    double scale = 1.0;
    std::string planeAxis = "z";
    int32_t cycleCount = 2;
    int32_t coarsestSize = 3;
    int32_t preSmoothingIterationCount = 2;
    int32_t postSmoothingIterationCount = 2;
    bool useCongruence = false;
    float qualityThreshold = 0.9;
    bool removeRampFlag = false;
    bool ascii = false;
    std::string format = "";
    bool applyMaskToOutput = false;
    bool removeBias = false;
    std::string fileNameBias = "";
    int32_t maximumFitOrder = 8;
    int32_t maximumBiasOrder = 2;
    int32_t zUnwrappingType = 1;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Phase unwrapper",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input phase map",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output floating unwrapped phase map name",
                                 fileNameOut );
    application.addSingleOption( "-m",
                                 "int16_t mask file name",
                                 fileNameMask,
                                 true );
    application.addSingleOption( "-q",
                                 "Output floating quality map name",
                                 fileNameQuality,
                                 true );
    application.addSingleOption( "-offset",
                                 "Offset value\n"
                                 "Float convertion from type T to f is"
                                 " f = offset + T * scale\n"
                                 "(default=0.0)",
                                 offset,
                                 true );
    application.addSingleOption( "-scale",
                                 "Scale value \n"
                                 "Float convertion from type T to f is"
                                 " f = offset + T * scale\n"
                                 "(default=1.0)",
                                 scale,
                                 true );
    application.addSingleOption( "-plane",
                                 "Main plane axis x / y / z (default=z)",
                                 planeAxis,
                                 true );
    application.addSingleOption( "-cycleCount",
                                 "WFMG cycle count (default=2)",
                                 cycleCount,
                                 true );
    application.addSingleOption( "-coarsestSize",
                                 "WFMG coarsest size (default=3)",
                                 coarsestSize,
                                 true );
    application.addSingleOption( "-preSmoothingIterationCount",
                                 "WFMG pre-smoothing iteration count"
                                 " (default=2)",
                                 preSmoothingIterationCount,
                                 true );
    application.addSingleOption( "-postSmoothingIterationCount",
                                 "WFMG post-smoothing iteration count"
                                 " (default=2)",
                                 postSmoothingIterationCount,
                                 true );
    application.addSingleOption( "-useCongruence",
                                 "Use congruence correction (default=false)",
                                 useCongruence,
                                 true );
    application.addSingleOption( "-qualityThreshold",
                                 "Quality threshold [default=0.9]",
                                 qualityThreshold,
                                 true );
    application.addSingleOption( "-removeRamp",
                                 "Remove ramp(s) according to x, y, and z axis",
                                 removeRampFlag,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-applyMaskToOutput",
                                 "Apply mask to output (default=false)",
                                 applyMaskToOutput, 
                                 true );
    application.addSingleOption( "-removeBias",
                                 "Remove bias with 2D polynomial correction"
                                 "(default=false)",
                                 removeBias, 
                                 true );
    application.addSingleOption( "-b",
                                 "Bias file name",
                                 fileNameBias,
                                 true );
     application.addSingleOption( "-maximumFitOrder",
                                 "Maximum 2D polynomial fit order "
                                 "(default=8)",
                                 maximumFitOrder, 
                                 true );
     application.addSingleOption( "-maximumBiasOrder",
                                 "Maximum 2D polynomial bias order"
                                 "(default=2)",
                                 maximumBiasOrder, 
                                 true );
     application.addSingleOption( "-zUnwrappingType",
                                 "Unwrapping along z type:\n"
                                 "1->standard using least square\n"
                                 "2->advanced using optimizer\n"
                                 "(default=1)",
                                 zUnwrappingType, 
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn, fileNameOut, fileNameMask, fileNameQuality, offset,
             scale, planeAxis, cycleCount, coarsestSize, 
             preSmoothingIterationCount, postSmoothingIterationCount, 
             useCongruence, qualityThreshold, removeRampFlag, ascii,
             format, applyMaskToOutput, removeBias, fileNameBias,
             maximumFitOrder, maximumBiasOrder, zUnwrappingType, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::PhaseUnwrapperCommand::parse()" );

}


void gkg::PhaseUnwrapperCommand::execute( const std::string& fileNameIn,
    			   		  const std::string& fileNameOut,
    			   		  const std::string& fileNameMask,
    			   		  const std::string& fileNameQuality,
    			   		  double offset,
    			   		  double scale,
    			   		  const std::string& planeAxis,
    			   		  int32_t cycleCount,
    			   		  int32_t coarsestSize,
    			   		  int32_t preSmoothingIterationCount,
    			   		  int32_t postSmoothingIterationCount,
    			   		  bool useCongruence,
    			   		  float qualityThreshold,
    			   		  bool removeRampFlag,
    			   		  bool ascii,
    			   		  const std::string& format,
    			   		  bool applyMaskToOutput,
    			   		  bool removeBias,
    			   		  const std::string& fileNameBias,
    			   		  int32_t maximumFitOrder,
    			   		  int32_t maximumBiasOrder,
    			   		  int32_t zUnwrappingType,
    			   		  bool verbose )
{

  try
  {

    PhaseUnwrapperProcess phaseUnwrapperProcess( fileNameOut,
                                                 fileNameMask,
                                                 fileNameQuality,
                                                 offset,
                                                 scale,
                                                 planeAxis,
                                                 cycleCount,
                                                 coarsestSize,
                                                 preSmoothingIterationCount,
                                                 postSmoothingIterationCount,
                                                 useCongruence,
                                                 qualityThreshold,
                                                 removeRampFlag,
                                                 ascii,
                                                 format,
                                                 applyMaskToOutput,
                                                 removeBias,
                                                 fileNameBias,
                                                 maximumFitOrder,
                                                 maximumBiasOrder,
                                                 zUnwrappingType,
                                                 verbose );
    phaseUnwrapperProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::PhaseUnwrapperCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
    	     "const std::string& fileNameMask, "
    	     "const std::string& fileNameQuality, "
    	     "double offset, double scale, const std::string& planeAxis, "
    	     "int32_t cycleCount, int32_t coarsestSize, "
    	     "int32_t preSmoothingIterationCount,  "
             "int32_t postSmoothingIterationCount, bool useCongruence, "
    	     "float qualityThreshold, bool removeRampFlag, bool ascii, "
    	     "const std::string& format, bool applyMaskToOutput, "
    	     "bool removeBias, const std::string& fileNameBias, "
    	     "int32_t maximumFitOrder, int32_t maximumBiasOrder, "
    	     "int32_t zUnwrappingType, bool verbose )" );

}


RegisterCommandCreator(
                 PhaseUnwrapperCommand ,
    		 DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    		 DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    		 DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    		 DECLARE_STRING_PARAMETER_HELP( fileNameQuality ) +
    		 DECLARE_FLOATING_PARAMETER_HELP( offset ) +
    		 DECLARE_FLOATING_PARAMETER_HELP( scale ) +
    		 DECLARE_STRING_PARAMETER_HELP( planeAxis ) +
    		 DECLARE_INTEGER_PARAMETER_HELP( cycleCount ) +
    		 DECLARE_INTEGER_PARAMETER_HELP( coarsestSize ) +
    		 DECLARE_INTEGER_PARAMETER_HELP( preSmoothingIterationCount ) +
    		 DECLARE_INTEGER_PARAMETER_HELP( postSmoothingIterationCount ) +
    		 DECLARE_BOOLEAN_PARAMETER_HELP( useCongruence ) +
    		 DECLARE_FLOATING_PARAMETER_HELP( qualityThreshold ) +
    		 DECLARE_BOOLEAN_PARAMETER_HELP( removeRampFlag ) +
    		 DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    		 DECLARE_STRING_PARAMETER_HELP( format ) +
    		 DECLARE_BOOLEAN_PARAMETER_HELP( applyMaskToOutput ) +
    		 DECLARE_BOOLEAN_PARAMETER_HELP( removeBias ) +
    		 DECLARE_STRING_PARAMETER_HELP( fileNameBias ) +
    		 DECLARE_INTEGER_PARAMETER_HELP( maximumFitOrder ) +
    		 DECLARE_INTEGER_PARAMETER_HELP( maximumBiasOrder ) +
    		 DECLARE_INTEGER_PARAMETER_HELP( zUnwrappingType ) +
    		 DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
