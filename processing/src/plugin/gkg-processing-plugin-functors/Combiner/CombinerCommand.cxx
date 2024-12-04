#include <gkg-processing-plugin-functors/Combiner/CombinerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/Combiner_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <string>
#include <vector>
#include <iostream>


//
// class MaskProcess
//

class MaskProcess : public gkg::Process
{

  public:

    MaskProcess( gkg::Volume< int8_t >& mask,
                 const std::string& mode,
                 const double& threshold1,
                 const double& threshold2,
                 const double& background,
                 bool verbose );

    gkg::Volume< int8_t >& getMask();
    const std::string& getMode() const;
    const double& getThreshold1() const;
    const double& getThreshold2() const;
    const double& getBackground() const;
    const bool& getVerbose() const;

  private:

    template < class M >
    static void read( gkg::Process& process,
                      const std::string& fileNameMask,
                      const gkg::AnalyzedObject&,
                      const std::string& );


    gkg::Volume< int8_t >& _mask;
    const std::string& _mode;
    const double& _threshold1;
    const double& _threshold2;
    const double& _background;
    const bool& _verbose;

};


MaskProcess::MaskProcess( gkg::Volume< int8_t >& mask,
                          const std::string& mode,
                          const double& threshold1,
                          const double& threshold2,
                          const double& background,
                          bool verbose )
            : gkg::Process( "Volume" ),
              _mask( mask ),
              _mode( mode ),
              _threshold1( threshold1 ),
              _threshold2( threshold2 ),
              _background( background ),
              _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &MaskProcess::read< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &MaskProcess::read< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &MaskProcess::read< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &MaskProcess::read< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &MaskProcess::read< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &MaskProcess::read< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &MaskProcess::read< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &MaskProcess::read< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &MaskProcess::read< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &MaskProcess::read< double > );

}


gkg::Volume< int8_t >& MaskProcess::getMask()
{

  return _mask;

}
 

const std::string& MaskProcess::getMode() const
{

  return _mode;

}


const double& MaskProcess::getThreshold1() const
{

  return _threshold1;

}


const double& MaskProcess::getThreshold2() const
{

  return _threshold2;

}


const double& MaskProcess::getBackground() const
{

  return _background;

}


const bool& MaskProcess::getVerbose() const
{

  return _verbose;

}


template < class M >
void MaskProcess::read( gkg::Process& process,
                        const std::string& fileNameMask,
                        const gkg::AnalyzedObject&,
                        const std::string& )
{

  try
  {

    MaskProcess&
      maskProcess = static_cast< MaskProcess& >( process );

    // reading mask
    gkg::Volume< M > maskVolume;

    if ( maskProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameMask << "' : " << std::flush;

    }
    gkg::Reader::getInstance().read( fileNameMask, maskVolume );
    if ( maskProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( maskProcess.getVerbose() )
    {

      std::cout << "building int8_t mask : " << std::flush;

    }
    gkg::TestFunction< M >* testFunction = 
        gkg::TestFunctionFactory< M >::getInstance().createTestFunction( 
                                             maskProcess.getMode(),
                                             ( M )maskProcess.getThreshold1(),
                                             ( M )maskProcess.getThreshold2() );
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

    maskProcess.getMask() = gkg::Volume< int8_t >( sizeX, sizeY, sizeZ, sizeT );
    maskProcess.getMask().getHeader().addAttribute( "resolutionX", resolutionX);
    maskProcess.getMask().getHeader().addAttribute( "resolutionY", resolutionY);
    maskProcess.getMask().getHeader().addAttribute( "resolutionZ", resolutionZ);
    maskProcess.getMask().getHeader().addAttribute( "resolutionT", resolutionT);

    typename gkg::Volume< M >::const_iterator i = maskVolume.begin(),
                                              ie = maskVolume.end();
    gkg::Volume< int8_t >::iterator o = maskProcess.getMask().begin();
    while ( i != ie )
    {

      *o = ( testFunction->getState( *i ) ? 1 : 0 );
      ++ i;
      ++ o;

    }


    if ( maskProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class M > "
             "void MaskProcess::read( gkg::Process& process, "
             "const std::string& fileNameMask, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class CombinerProcess
//

class CombinerProcess : public gkg::Process
{

  public:

    CombinerProcess( const std::vector< std::string >& fileNameIns,
                     const std::string& fileNameOut,
                     const std::vector< std::string >& functor1s,
                     const std::vector< std::string >& functor2s,
                     const std::vector< double >& numerator1s,
                     const std::vector< double >& denominator1s,
                     const std::vector< double >& numerator2s,
                     const std::vector< double >& denominator2s,
                     const std::vector< std::string >& operators,
                     const std::string& fileNameMask,
                     const std::string& mode,
                     const double& threshold1,
                     const double& threshold2,
                     const double& background,
                     const std::string& outputType,
                     const bool& ascii,
                     const std::string& format,
                     const bool& verbose );

    const std::vector< std::string >& getFileNameIns() const;
    const std::string& getFileNameOut() const;
    const std::vector< std::string >& getFunctor1s() const;
    const std::vector< std::string >& getFunctor2s() const;
    const std::vector< double >& getNumerator1s() const;
    const std::vector< double >& getDenominator1s() const;
    const std::vector< double >& getNumerator2s() const;
    const std::vector< double >& getDenominator2s() const;
    const std::vector< std::string >& getOperators() const;
    const std::string& getFileNameMask() const;
    const std::string& getMode() const;
    const double& getThreshold1() const;
    const double& getThreshold2() const;
    const double& getBackground() const;
    const std::string& getOutputType() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void combine( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    const std::vector< std::string >& _fileNameIns;
    const std::string& _fileNameOut;
    const std::vector< std::string >& _functor1s;
    const std::vector< std::string >& _functor2s;
    const std::vector< double >& _numerator1s;
    const std::vector< double >& _denominator1s;
    const std::vector< double >& _numerator2s;
    const std::vector< double >& _denominator2s;
    const std::vector< std::string >& _operators;
    const std::string& _fileNameMask;
    const std::string& _mode;
    const double& _threshold1;
    const double& _threshold2;
    const double& _background;
    const std::string& _outputType;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


CombinerProcess::CombinerProcess( const std::vector< std::string >& fileNameIns,
                                  const std::string& fileNameOut,
                                  const std::vector< std::string >& functor1s,
                                  const std::vector< std::string >& functor2s,
                                  const std::vector< double >& numerator1s,
                                  const std::vector< double >& denominator1s,
                                  const std::vector< double >& numerator2s,
                                  const std::vector< double >& denominator2s,
                                  const std::vector< std::string >& operators,
                                  const std::string& fileNameMask,
                                  const std::string& mode,
                                  const double& threshold1,
                                  const double& threshold2,
                                  const double& background,
                                  const std::string& outputType,
                                  const bool& ascii,
                                  const std::string& format,
                                  const bool& verbose )
                : gkg::Process( "Volume" ),
                  _fileNameIns( fileNameIns ),
                  _fileNameOut( fileNameOut ),
                  _functor1s( functor1s ),
                  _functor2s( functor2s ),
                  _numerator1s( numerator1s ),
                  _denominator1s( denominator1s ),
                  _numerator2s( numerator2s ),
                  _denominator2s( denominator2s ),
                  _operators( operators ),
                  _fileNameMask( fileNameMask ),
                  _mode( mode ),
                  _threshold1( threshold1 ),
                  _threshold2( threshold2 ),
                  _background( background ),
                  _outputType( outputType ),
                  _ascii( ascii ),
                  _format( format ),
                  _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &CombinerProcess::combine< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &CombinerProcess::combine< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &CombinerProcess::combine< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &CombinerProcess::combine< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &CombinerProcess::combine< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &CombinerProcess::combine< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &CombinerProcess::combine< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &CombinerProcess::combine< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &CombinerProcess::combine< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &CombinerProcess::combine< double > );

}


const std::vector< std::string >& CombinerProcess::getFileNameIns() const
{

  return _fileNameIns;

}


const std::string& CombinerProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::vector< std::string >& CombinerProcess::getFunctor1s() const
{

  return _functor1s;

}


const std::vector< std::string >& CombinerProcess::getFunctor2s() const
{

  return _functor2s;

}


const std::vector< double >& CombinerProcess::getNumerator1s() const
{

  return _numerator1s;

}


const std::vector< double >& CombinerProcess::getDenominator1s() const
{

  return _denominator1s;

}


const std::vector< double >& CombinerProcess::getNumerator2s() const
{

  return _numerator2s;

}


const std::vector< double >& CombinerProcess::getDenominator2s() const
{

  return _denominator2s;

}


const std::vector< std::string >& CombinerProcess::getOperators() const
{

  return _operators;

}


const std::string& CombinerProcess::getFileNameMask() const
{

  return _fileNameMask;

}


const std::string& CombinerProcess::getMode() const
{

  return _mode;

}


const double& CombinerProcess::getThreshold1() const
{

  return _threshold1;

}


const double& CombinerProcess::getThreshold2() const
{

  return _threshold2;

}


const double& CombinerProcess::getBackground() const
{

  return _background;

}


const std::string& CombinerProcess::getOutputType() const
{

  return _outputType;

}


const bool& CombinerProcess::getAscii() const
{

  return _ascii;

}


const std::string& CombinerProcess::getFormat() const
{

  return _format;

}


const bool& CombinerProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void CombinerProcess::combine( gkg::Process& process,
                               const std::string& /*fileNameIn*/,
                               const gkg::AnalyzedObject&,
                               const std::string& )
{

  try
  {

    CombinerProcess&
      combinerProcess = static_cast< CombinerProcess& >( process );

    // collecting input volume count
    int32_t inputVolumeCount = 
                             ( int32_t )combinerProcess.getFileNameIns().size();

    // reading data
    std::vector< gkg::Volume< T > > inputVolumes( inputVolumeCount );
    int32_t i;
    for ( i = 0; i < inputVolumeCount; i++ )
    {

      std::string fileNameIn = combinerProcess.getFileNameIns()[ i ];
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "reading '" << fileNameIn << "' : " << std::flush;

      }
      gkg::Reader::getInstance().read( fileNameIn, inputVolumes[ i ] );
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }
      if ( !inputVolumes[ i ].hasSameSize( inputVolumes[ 0 ], true ) ||
           !inputVolumes[ i ].hasSameResolution( inputVolumes[ 0 ], true ) )
      {

        throw std::runtime_error(
                     "incoherent size and resolution between input volume(s)" );

      }

    }

    // reading mask
    gkg::Volume< int8_t >* maskVolume = 0;
    gkg::TestFunction< int8_t >* testFunction = 0;
    bool hasMask = false;
    std::string fileNameMask = combinerProcess.getFileNameMask();
    if ( !fileNameMask.empty() )
    {

      hasMask = true;
      maskVolume = new gkg::Volume< int8_t >;
      MaskProcess maskProcess( *maskVolume,
                               combinerProcess.getMode(),
                               combinerProcess.getThreshold1(),
                               combinerProcess.getThreshold2(),
                               combinerProcess.getBackground(),
                               combinerProcess.getVerbose() );
      maskProcess.execute( fileNameMask );
      if ( !maskVolume->hasSameSize( inputVolumes[ 0 ], true ) &&
           !maskVolume->hasSameResolution( inputVolumes[ 0 ], true ) )
      {

        throw std::runtime_error(
            "incoherent size and resolution between mask and input volume(s)" );

      }

      // creating test function if mask available
      testFunction = 
        gkg::TestFunctionFactory< int8_t >::getInstance().createTestFunction( 
                                                                         "gt",
                                                                         0 );

    }

    if ( combinerProcess.getOutputType().empty() )
    {

      // combining data
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "combining : " << std::flush;

      }
      gkg::Volume< T > outputVolume;
      if ( hasMask )
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< T > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators(),
                    testFunction,
                    ( T )combinerProcess.getBackground() );

        combiner.combine( inputVolumes,
                          *maskVolume,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      else
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< T > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators() );

        combiner.combine( inputVolumes,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }
      // writing output volume
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "writing '"
                << combinerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( combinerProcess.getFileNameOut(),
                                        outputVolume,
                                        combinerProcess.getAscii(),
                                        combinerProcess.getFormat() );
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int8_t >::isSameType( 
                                             combinerProcess.getOutputType() ) )
    {

      // combining data
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "combining : " << std::flush;

      }
      gkg::Volume< int8_t > outputVolume;
      if ( hasMask )
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< int8_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators(),
                    testFunction,
                    ( int8_t )combinerProcess.getBackground() );

        combiner.combine( inputVolumes,
                          *maskVolume,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      else
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< int8_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators() );

        combiner.combine( inputVolumes,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }
      // writing output volume
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "writing '"
                << combinerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( combinerProcess.getFileNameOut(),
                                        outputVolume,
                                        combinerProcess.getAscii(),
                                        combinerProcess.getFormat() );
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint8_t >::isSameType( 
                                             combinerProcess.getOutputType() ) )
    {

      // combining data
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "combining : " << std::flush;

      }
      gkg::Volume< uint8_t > outputVolume;
      if ( hasMask )
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< uint8_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators(),
                    testFunction,
                    ( uint8_t )combinerProcess.getBackground() );

        combiner.combine( inputVolumes,
                          *maskVolume,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      else
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< uint8_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators() );

        combiner.combine( inputVolumes,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }
      // writing output volume
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "writing '"
                << combinerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( combinerProcess.getFileNameOut(),
                                        outputVolume,
                                        combinerProcess.getAscii(),
                                        combinerProcess.getFormat() );
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int16_t >::isSameType( 
                                             combinerProcess.getOutputType() ) )
    {

      // combining data
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "combining : " << std::flush;

      }
      gkg::Volume< int16_t > outputVolume;
      if ( hasMask )
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< int16_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators(),
                    testFunction,
                    ( int16_t )combinerProcess.getBackground() );

        combiner.combine( inputVolumes,
                          *maskVolume,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      else
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< int16_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators() );

        combiner.combine( inputVolumes,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }
      // writing output volume
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "writing '"
                << combinerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( combinerProcess.getFileNameOut(),
                                        outputVolume,
                                        combinerProcess.getAscii(),
                                        combinerProcess.getFormat() );
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint16_t >::isSameType( 
                                             combinerProcess.getOutputType() ) )
    {

      // combining data
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "combining : " << std::flush;

      }
      gkg::Volume< uint16_t > outputVolume;
      if ( hasMask )
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< uint16_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators(),
                    testFunction,
                    ( uint16_t )combinerProcess.getBackground() );

        combiner.combine( inputVolumes,
                          *maskVolume,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      else
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< uint16_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators() );

        combiner.combine( inputVolumes,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }
      // writing output volume
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "writing '"
                << combinerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( combinerProcess.getFileNameOut(),
                                        outputVolume,
                                        combinerProcess.getAscii(),
                                        combinerProcess.getFormat() );
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int32_t >::isSameType( 
                                             combinerProcess.getOutputType() ) )
    {

      // combining data
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "combining : " << std::flush;

      }
      gkg::Volume< int32_t > outputVolume;
      if ( hasMask )
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< int32_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators(),
                    testFunction,
                    ( int32_t )combinerProcess.getBackground() );

        combiner.combine( inputVolumes,
                          *maskVolume,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      else
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< int32_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators() );

        combiner.combine( inputVolumes,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }
      // writing output volume
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "writing '"
                << combinerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( combinerProcess.getFileNameOut(),
                                        outputVolume,
                                        combinerProcess.getAscii(),
                                        combinerProcess.getFormat() );
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint32_t >::isSameType( 
                                             combinerProcess.getOutputType() ) )
    {

      // combining data
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "combining : " << std::flush;

      }
      gkg::Volume< uint32_t > outputVolume;
      if ( hasMask )
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< uint32_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators(),
                    testFunction,
                    ( uint32_t )combinerProcess.getBackground() );

        combiner.combine( inputVolumes,
                          *maskVolume,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      else
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< uint32_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators() );

        combiner.combine( inputVolumes,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }
      // writing output volume
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "writing '"
                << combinerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( combinerProcess.getFileNameOut(),
                                        outputVolume,
                                        combinerProcess.getAscii(),
                                        combinerProcess.getFormat() );
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int64_t >::isSameType( 
                                             combinerProcess.getOutputType() ) )
    {

      // combining data
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "combining : " << std::flush;

      }
      gkg::Volume< int64_t > outputVolume;
      if ( hasMask )
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< int64_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators(),
                    testFunction,
                    ( int64_t )combinerProcess.getBackground() );

        combiner.combine( inputVolumes,
                          *maskVolume,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      else
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< int64_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators() );

        combiner.combine( inputVolumes,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }
      // writing output volume
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "writing '"
                << combinerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( combinerProcess.getFileNameOut(),
                                        outputVolume,
                                        combinerProcess.getAscii(),
                                        combinerProcess.getFormat() );
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint64_t >::isSameType( 
                                             combinerProcess.getOutputType() ) )
    {

      // combining data
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "combining : " << std::flush;

      }
      gkg::Volume< uint64_t > outputVolume;
      if ( hasMask )
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< uint64_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators(),
                    testFunction,
                    ( uint64_t )combinerProcess.getBackground() );

        combiner.combine( inputVolumes,
                          *maskVolume,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      else
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< uint64_t > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators() );

        combiner.combine( inputVolumes,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }
      // writing output volume
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "writing '"
                << combinerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( combinerProcess.getFileNameOut(),
                                        outputVolume,
                                        combinerProcess.getAscii(),
                                        combinerProcess.getFormat() );
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< float >::isSameType( 
                                             combinerProcess.getOutputType() ) )
    {

      // combining data
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "combining : " << std::flush;

      }
      gkg::Volume< float > outputVolume;
      if ( hasMask )
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< float > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators(),
                    testFunction,
                    ( float )combinerProcess.getBackground() );

        combiner.combine( inputVolumes,
                          *maskVolume,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      else
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< float > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators() );

        combiner.combine( inputVolumes,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }
      // writing output volume
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "writing '"
                << combinerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( combinerProcess.getFileNameOut(),
                                        outputVolume,
                                        combinerProcess.getAscii(),
                                        combinerProcess.getFormat() );
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< double >::isSameType( 
                                             combinerProcess.getOutputType() ) )
    {

      // combining data
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "combining : " << std::flush;

      }
      gkg::Volume< double > outputVolume;
      if ( hasMask )
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< double > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators(),
                    testFunction,
                    ( double )combinerProcess.getBackground() );

        combiner.combine( inputVolumes,
                          *maskVolume,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      else
      {

        gkg::Combiner< gkg::Volume< T >,
                       gkg::Volume< int8_t >,
                       gkg::Volume< double > >
          combiner( combinerProcess.getFunctor1s(),
                    combinerProcess.getFunctor2s(),
                    combinerProcess.getNumerator1s(),
                    combinerProcess.getDenominator1s(),
                    combinerProcess.getNumerator2s(),
                    combinerProcess.getDenominator2s(),
                    combinerProcess.getOperators() );

        combiner.combine( inputVolumes,
                          outputVolume,
                          combinerProcess.getVerbose() );

      }
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }
      // writing output volume
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "writing '"
                << combinerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( combinerProcess.getFileNameOut(),
                                        outputVolume,
                                        combinerProcess.getAscii(),
                                        combinerProcess.getFormat() );
      if ( combinerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

    // removing allocated object(s)
    if ( testFunction )
    {
    
      delete testFunction;

    }
    if ( maskVolume )
    {
    
      delete maskVolume;

    }

  }
  GKG_CATCH( "template < class T, class M > "
             "void CombinerProcess::combine( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class CombinerCommand
//


gkg::CombinerCommand::CombinerCommand( int32_t argc,
                                       char* argv[],
                                       bool loadPlugin,
                                       bool removeFirst )
                    : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::CombinerCommand::CombinerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::CombinerCommand::CombinerCommand(
                     		  const std::vector< std::string >& fileNameIns,
                     		  const std::string& fileNameOut,
    		     		  std::vector< std::string >& functor1s,
    		     		  std::vector< std::string >& functor2s,
    		     		  std::vector< double >& numerator1s,
    		     		  std::vector< double >& denominator1s,
    		     		  std::vector< double >& numerator2s,
    		     		  std::vector< double >& denominator2s,
    		     		  std::vector< std::string >& operators,
    		     		  const std::string& fileNameMask,
    		     		  const std::string& mode,
    		     		  double threshold1,
    		     		  double threshold2,
    		     		  double background,
    		     		  const std::string& outputType,
    		     		  bool ascii,
    		     		  const std::string& format,
    		     		  bool verbose )
{

  try
  {

    execute( fileNameIns, fileNameOut, functor1s, functor2s, numerator1s,
             denominator1s, numerator2s, denominator2s, operators,
             fileNameMask, mode, threshold1, threshold2, background,
             outputType, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::CombinerCommand::CombinerCommand( "
             "const std::vector< std::string >& fileNameIns, "
             "const std::string& fileNameOut, "
    	     "std::vector< std::string >& functor1s, "
    	     "std::vector< std::string >& functor2s, "
    	     "std::vector< double >& numerator1s, "
    	     "std::vector< double >& denominator1s, "
    	     "std::vector< double >& numerator2s, "
    	     "std::vector< double >& denominator2s, "
    	     "std::vector< std::string >& operators, "
    	     "const std::string& fileNameMask, const std::string& mode, "
    	     "double threshold1, double threshold2, "
    	     "double background, const std::string& outputType, "
    	     "bool ascii, const std::string& format, bool verbose )" );

}


gkg::CombinerCommand::CombinerCommand( const gkg::Dictionary& parameters )
                    : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, 
                                         std::vector< std::string >,
                                         fileNameIns );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, 
                                         std::vector< std::string >,
                                         functor1s );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, 
                                         std::vector< std::string >,
                                         functor2s );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, 
                                           std::vector< double >,
                                           numerator1s );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, 
                                           std::vector< double >,
                                           denominator1s );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, 
                                           std::vector< double >,
                                           numerator2s );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, 
                                           std::vector< double >,
                                           denominator2s );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, 
                                         std::vector< std::string >,
                                         operators );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, mode );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold1 );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold2 );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputType );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameIns, fileNameOut, functor1s, functor2s, numerator1s,
             denominator1s, numerator2s, denominator2s, operators,
             fileNameMask, mode, threshold1, threshold2, background,
             outputType, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::CombinerCommand::CombinerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::CombinerCommand::~CombinerCommand()
{
}


std::string gkg::CombinerCommand::getStaticName()
{

  try
  {

    return "Combiner";

  }
  GKG_CATCH( "std::string gkg::CombinerCommand::getStaticName()" );

}


void gkg::CombinerCommand::parse()
{

  try
  {

    std::vector< std::string > fileNameIns;
    std::string fileNameOut;
    std::vector< std::string > functor1s;
    std::vector< std::string > functor2s;
    std::vector< double > numerator1s;
    std::vector< double > denominator1s;
    std::vector< double > numerator2s;
    std::vector< double > denominator2s;
    std::vector< std::string > operators;
    std::string fileNameMask;
    std::string mode = "gt";
    double threshold1 = 0.0;
    double threshold2 = 0.0;
    double background = 0.0;
    std::string outputType = "";
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application(
        _argc, _argv,
        "Volume combiner\n"
        "O = (n1[1]/d1[1])*f1[1]( (n2[1]/d2[1]) * f2[1](I[1]) ) <op[1]> ... \n"
        "    <op[M-1]> (n1[M]/d1[M])*f1[M]( (n2[M]/d2[M]) * f2[M](I[M]) )\n"
        "    <op[M]> (n1[M+1]/d1[M+1])\n",
        _loadPlugin );
    application.addSeriesOption( "-i",
                                 "Input volume file name list",
                                 fileNameIns,
                                 1 );
    application.addSingleOption( "-o",
                                 "Output combined volume file name",
                                 fileNameOut );
    application.addSeriesOption( "-f",
                                 "f functor list (default=list of 'id')\n"
                                 "- id -> identity functor\n"
                                 "- inv -> inverse functor\n"
                                 "- ln -> natural logarithm functor\n"
                                 "- log -> base 10 logarithm functor\n"
                                 "- exp -> exponential functor\n"
                                 "- sqr -> square power functor\n"
                                 "- cub -> cubic power functor\n"
                                 "- sqrt -> square root functor\n"
                                 "- cbrt -> cubic root functor\n"
                                 "- cos -> cosine functor\n"
                                 "- sin -> sine functor\n"
                                 "- tan -> tangent functor\n"
                                 "- acos -> arc-cosine functor\n"
                                 "- asin -> arc-sine functor\n"
                                 "- atan -> arc-tangent functor",
                                 functor1s,
                                 0 );
    application.addSeriesOption( "-g",
                                 "g functor list (default=list of 'id')\n"
                                 "- id -> identity functor\n"
                                 "- inv -> inverse functor\n"
                                 "- ln -> natural logarithm functor\n"
                                 "- log -> base 10 logarithm functor\n"
                                 "- exp -> exponential functor\n"
                                 "- sqr -> square power functor\n"
                                 "- cub -> cubic power functor\n"
                                 "- sqrt -> square root functor\n"
                                 "- cbrt -> cubic root functor\n"
                                 "- cos -> cosine functor\n"
                                 "- sin -> sine functor\n"
                                 "- tan -> tangent functor\n"
                                 "- acos -> arc-cosine functor\n"
                                 "- asin -> arc-sine functor\n"
                                 "- atan -> arc-tangent functor",
                                 functor2s,
                                 0 );
    application.addSeriesOption( "-num1",
                                 "First numerator list (default=list of 1.0)",
                                 numerator1s,
                                 0 );
    application.addSeriesOption( "-den1",
                                 "First denominator list (default=list of 1.0)",
                                 denominator1s,
                                 0 );
    application.addSeriesOption( "-num2",
                                 "Second numerator list (default=list of 1.0)",
                                 numerator2s,
                                 0 );
    application.addSeriesOption( "-den2",
                                 "Second denominator list "
                                 "(default=list of 1.0)",
                                 denominator2s,
                                 0 );
    application.addSeriesOption( "-op",
                                 "Operator list (default=list of '+')\n"
                                 "- '+' -> addition\n"
                                 "- '-' -> substraction\n"
                                 "- '*' -> multiplication\n"
                                 "- '/' -> division",
                                 operators,
                                 0 );
    application.addSingleOption( "-M",
                                 "Mask volume file name",
                                 fileNameMask,
                                 true );
    application.addSingleOption( "-m",
                                 "Threshold masking mode (default=gt)\n"
                                 " - lt -> lower than\n"
                                 " - le -> lower or equal to\n"
                                 " - eq -> equal to\n"
                                 " - di -> different from\n"
                                 " - ge -> greater or equal to\n"
                                 " - gt -> greater than\n"
                                 " - be -> between or equal\n"
                                 " - bt -> between\n"
                                 " - oe -> outside or equal\n"
                                 " - ou -> outside\n",
                                 mode,
                                 true );
    application.addSingleOption( "-t1",
                                 "First threshold (default=0.0)",
                                 threshold1,
                                 true );
    application.addSingleOption( "-t2",
                                 "Second threshold (required for be/bt/oe/ou) "
                                 "(default=0.0)",
                                 threshold2,
                                 true );
    application.addSingleOption( "-background",
                                 "Background (default=0.0)",
                                 background,
                                 true );
    application.addSingleOption( "-t",
                                 "Output volume item type "
                                 "(default=same as input)",
                                 outputType,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
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

    execute( fileNameIns, fileNameOut, functor1s, functor2s, numerator1s,
             denominator1s, numerator2s, denominator2s, operators,
             fileNameMask, mode, threshold1, threshold2, background,
             outputType, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::CombinerCommand::parse()" );

}


void gkg::CombinerCommand::execute(
                     		  const std::vector< std::string >& fileNameIns,
                     		  const std::string& fileNameOut,
    		     		  std::vector< std::string >& functor1s,
    		     		  std::vector< std::string >& functor2s,
    		     		  std::vector< double >& numerator1s,
    		     		  std::vector< double >& denominator1s,
    		     		  std::vector< double >& numerator2s,
    		     		  std::vector< double >& denominator2s,
    		     		  std::vector< std::string >& operators,
    		     		  const std::string& fileNameMask,
    		     		  const std::string& mode,
    		     		  double threshold1,
    		     		  double threshold2,
    		     		  double background,
    		     		  const std::string& outputType,
    		     		  bool ascii,
    		     		  const std::string& format,
    		     		  bool verbose )
{

  try
  {

    //
    // sanity check(s)
    //
    if ( ( mode != "lt" ) &&
         ( mode != "le" ) &&
         ( mode != "eq" ) &&
         ( mode != "di" ) &&
         ( mode != "ge" ) &&
         ( mode != "gt" ) &&
         ( mode != "be" ) &&
         ( mode != "bt" ) &&
         ( mode != "oe" ) &&
         ( mode != "ou" ) )
    {

      throw std::runtime_error( "invalid masking mode" );

    }

    std::vector< std::string >::const_iterator f1 = functor1s.begin(),
                                               f1e = functor1s.end();
    while ( f1 != f1e )
    {

      if ( ( *f1 != "id" ) &&
           ( *f1 != "inv" ) &&
           ( *f1 != "ln" ) &&
           ( *f1 != "log" ) &&
           ( *f1 != "exp" ) &&
           ( *f1 != "sqr" ) &&
           ( *f1 != "cub" ) &&
           ( *f1 != "sqrt" ) &&
           ( *f1 != "cbrt" ) &&
           ( *f1 != "cos" ) &&
           ( *f1 != "sin" ) &&
           ( *f1 != "tan" ) &&
           ( *f1 != "acos" ) &&
           ( *f1 != "asin" ) &&
           ( *f1 != "atan" ) )
      {

        throw std::runtime_error( "invalid functor 1" );

      }
      ++ f1;

    }

    std::vector< std::string >::const_iterator f2 = functor2s.begin(),
                                               f2e = functor2s.end();
    while ( f2 != f2e )
    {

      if ( ( *f2 != "id" ) &&
           ( *f2 != "inv" ) &&
           ( *f2 != "ln" ) &&
           ( *f2 != "log" ) &&
           ( *f2 != "exp" ) &&
           ( *f2 != "sqr" ) &&
           ( *f2 != "cub" ) &&
           ( *f2 != "sqrt" ) &&
           ( *f2 != "cbrt" ) &&
           ( *f2 != "cos" ) &&
           ( *f2 != "sin" ) &&
           ( *f2 != "tan" ) &&
           ( *f2 != "acos" ) &&
           ( *f2 != "asin" ) &&
           ( *f2 != "atan" ) )
      {

        throw std::runtime_error( "invalid functor 2" );

      }
      ++ f2;

    }

    std::vector< std::string >::const_iterator o = operators.begin(),
                                               oe = operators.end();
    while ( o != oe )
    {

      if ( ( *o != "+" ) &&
           ( *o != "-" ) &&
           ( *o != "*" ) &&
           ( *o != "/" ) )
      {

        throw std::runtime_error( "invalid operator" );

      }
      ++ o;

    }

    int32_t inputCount = ( int32_t )fileNameIns.size();   
    if ( functor1s.empty() )
    {

      functor1s = std::vector< std::string >( inputCount, "id" );

    }
    if ( functor2s.empty() )
    {

      functor2s = std::vector< std::string >( inputCount, "id" );

    }
    if ( numerator1s.empty() )
    {

      numerator1s = std::vector< double >( inputCount + 1, 1.0 );
      numerator1s[ inputCount ] = 0.0;

    }
    if ( denominator1s.empty() )
    {

      denominator1s = std::vector< double >( inputCount + 1, 1.0 );

    }
    if ( numerator2s.empty() )
    {

      numerator2s = std::vector< double >( inputCount, 1.0 );

    }
    if ( denominator2s.empty() )
    {

      denominator2s = std::vector< double >( inputCount, 1.0 );

    }
    if ( operators.empty() )
    {

      operators = std::vector< std::string >( inputCount, "+" );

    }

    if ( ( int32_t )functor1s.size() != inputCount )
    {

      throw std::runtime_error( "bad functor 1 list size" );

    } 
    if ( ( int32_t )functor2s.size() != inputCount )
    {

      throw std::runtime_error( "bad functor 2 list size" );

    } 
    if ( ( int32_t )numerator1s.size() != inputCount + 1 )
    {

      throw std::runtime_error( "bad numerator 1 list size" );

    } 
    if ( ( int32_t )denominator1s.size() != inputCount + 1 )
    {

      throw std::runtime_error( "bad denominator 1 list size" );

    } 
    if ( ( int32_t )numerator2s.size() != inputCount )
    {

      throw std::runtime_error( "bad numerator 2 list size" );

    } 
    if ( ( int32_t )denominator2s.size() != inputCount )
    {

      throw std::runtime_error( "bad denominator 2 list size" );

    } 
    if ( ( int32_t )operators.size() != inputCount )
    {

      throw std::runtime_error( "bad operator list size" );

    } 

    //
    // launching process
    //
    CombinerProcess combinerProcess( fileNameIns,
                                     fileNameOut,
                                     functor1s,
                                     functor2s,
                                     numerator1s,
                                     denominator1s,
                                     numerator2s,
                                     denominator2s,
                                     operators,
                                     fileNameMask,
                                     mode,
                                     threshold1,
                                     threshold2,
                                     background,
                                     outputType,
                                     ascii,
                                     format,
                                     verbose );

    combinerProcess.execute( fileNameIns[ 0 ] );

  }
  GKG_CATCH( "void gkg::CombinerCommand::execute( "
             "const std::vector< std::string >& fileNameIns, "
             "const std::string& fileNameOut, "
    	     "std::vector< std::string >& functor1s, "
    	     "std::vector< std::string >& functor2s, "
    	     "std::vector< double >& numerator1s, "
    	     "std::vector< double >& denominator1s, "
    	     "std::vector< double >& numerator2s, "
    	     "std::vector< double >& denominator2s, "
    	     "std::vector< std::string >& operators, "
    	     "const std::string& fileNameMask, const std::string& mode, "
    	     "double threshold1, double threshold2, "
    	     "double background, const std::string& outputType, "
    	     "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator(
                   CombinerCommand,
                   DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameIns ) +
                   DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                   DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( functor1s ) +
                   DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( functor2s ) +
                   DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( numerator1s ) +
                   DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( denominator1s ) +
                   DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( numerator2s ) +
                   DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( denominator2s ) +
                   DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( operators ) +
                   DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
                   DECLARE_STRING_PARAMETER_HELP( mode ) +
                   DECLARE_FLOATING_PARAMETER_HELP( threshold1 ) +
                   DECLARE_FLOATING_PARAMETER_HELP( threshold2 ) +
                   DECLARE_FLOATING_PARAMETER_HELP( background ) +
                   DECLARE_STRING_PARAMETER_HELP( outputType ) +
                   DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                   DECLARE_STRING_PARAMETER_HELP( format ) +
                   DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
