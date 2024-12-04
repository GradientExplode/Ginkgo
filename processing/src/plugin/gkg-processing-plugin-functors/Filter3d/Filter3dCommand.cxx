#include <gkg-processing-plugin-functors/Filter3d/Filter3dCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-signal/Filter3d_i.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-processing-algobase/MedianFilter_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class Filter3dProcess : public gkg::Process
{

  public:

    Filter3dProcess( const std::string& fileNameOut,
                     const std::string& outType,
                     const std::string& filterType,
                     const std::string& fileNameMask,
                     const gkg::Neighborhood3d::Type& neighborhood3dType,
                     const bool& ascii,
                     const std::string& format,
                     const bool& verbose );

    const std::string& getFileNameOut() const;
    const std::string& getOutType() const;
    const std::string& getFilterType() const;
    const std::string& getFileNameMask() const;
    const gkg::Neighborhood3d::Type& getNeighborhood3dType() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void filter( gkg::Process& process,
                        const std::string& fileNameIn,
                        const gkg::AnalyzedObject&,
                        const std::string& );

    const std::string& _fileNameOut;
    const std::string& _outType;
    const std::string& _filterType;
    const std::string& _fileNameMask;
    const gkg::Neighborhood3d::Type& _neighborhood3dType;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


Filter3dProcess::Filter3dProcess( const std::string& fileNameOut,
                                  const std::string& outType,
                                  const std::string& filterType,
                                  const std::string& fileNameMask,
                                  const gkg::Neighborhood3d::Type&
                                                             neighborhood3dType,
                                  const bool& ascii,
                                  const std::string& format,
                                  const bool& verbose )
                : gkg::Process(),
                  _fileNameOut( fileNameOut ),
                  _outType( outType ),
                  _filterType( filterType ),
                  _fileNameMask( fileNameMask ),
                  _neighborhood3dType( neighborhood3dType ),
                  _ascii( ascii ),
                  _format( format ),
                  _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &Filter3dProcess::filter< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &Filter3dProcess::filter< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &Filter3dProcess::filter< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &Filter3dProcess::filter< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &Filter3dProcess::filter< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &Filter3dProcess::filter< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &Filter3dProcess::filter< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &Filter3dProcess::filter< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &Filter3dProcess::filter< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &Filter3dProcess::filter< double > );

}


const std::string& Filter3dProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& Filter3dProcess::getOutType() const
{

  return _outType;

}


const std::string& Filter3dProcess::getFilterType() const
{

  return _filterType;

}


const std::string& Filter3dProcess::getFileNameMask() const
{

  return _fileNameMask;

}


const gkg::Neighborhood3d::Type& Filter3dProcess::getNeighborhood3dType() const
{

  return _neighborhood3dType;

}


const bool& Filter3dProcess::getAscii() const
{

  return _ascii;

}


const std::string& Filter3dProcess::getFormat() const
{

  return _format;

}


const bool& Filter3dProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void Filter3dProcess::filter( gkg::Process& process,
                              const std::string& fileNameIn,
                              const gkg::AnalyzedObject&,
                              const std::string& )
{

  try
  {

    Filter3dProcess&
      filter3dProcess = static_cast< Filter3dProcess& >( process );

    // reading data
    if ( filter3dProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( filter3dProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // defining mask
    gkg::Volume< uint8_t > mask;
    if ( filter3dProcess.getFileNameMask().empty() )
    {

      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "using whole brain mask : " << std::flush;

      }

      mask = gkg::Volume< uint8_t >( volumeIn.getSizeX(),
                                     volumeIn.getSizeY(),
                                     volumeIn.getSizeZ() );
      mask.fill( 1U );

      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "reading '" << filter3dProcess.getFileNameMask()
                  << "' mask: " << std::flush;

      }
      gkg::Reader::getInstance().read( filter3dProcess.getFileNameMask(),
                                       mask );
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

    if ( filter3dProcess.getVerbose() )
    {

      std::cout << "filtering : " << std::flush;

    }

    if ( filter3dProcess.getOutType().empty() )
    {

      gkg::Volume< T > volumeOut;
      gkg::Filter3d< T, uint8_t, T > filter3d;
      gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
      gkg::FilterFunction< std::vector< T >, T >* filterFunction = 0;
      if ( filter3dProcess.getFilterType() == "average" )
      {

        filterFunction = new gkg::AverageFilter< std::vector< T >, T >;

      }
      else if ( filter3dProcess.getFilterType() == "median" )
      {

        filterFunction = new gkg::MedianFilter< std::vector< T >, T >;

      }
      filter3d.filter( volumeIn,
                       mask,
                       equalToFunction,
                       *filterFunction,
                       filter3dProcess.getNeighborhood3dType(),
                       volumeOut,
                       filter3dProcess.getVerbose() );
      delete filterFunction;
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << filter3dProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( filter3dProcess.getFileNameOut(),
                                        volumeOut,
                                        filter3dProcess.getAscii(),
                                        filter3dProcess.getFormat() );
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    if ( gkg::TypeOf< int8_t >::isSameType( filter3dProcess.getOutType() ) )
    {

      gkg::Volume< int8_t > volumeOut;
      gkg::Filter3d< T, uint8_t, int8_t > filter3d;
      gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
      gkg::FilterFunction< std::vector< T >, int8_t >* filterFunction = 0;
      if ( filter3dProcess.getFilterType() == "average" )
      {

        filterFunction = new gkg::AverageFilter< std::vector< T >, int8_t >;

      }
      else if ( filter3dProcess.getFilterType() == "median" )
      {

        filterFunction = new gkg::MedianFilter< std::vector< T >, int8_t >;

      }
      filter3d.filter( volumeIn,
                       mask,
                       equalToFunction,
                       *filterFunction,
                       filter3dProcess.getNeighborhood3dType(),
                       volumeOut,
                       filter3dProcess.getVerbose() );
      delete filterFunction;
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << filter3dProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( filter3dProcess.getFileNameOut(),
                                        volumeOut,
                                        filter3dProcess.getAscii(),
                                        filter3dProcess.getFormat() );
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint8_t >::isSameType(
                                                filter3dProcess.getOutType() ) )
    {

      gkg::Volume< uint8_t > volumeOut;
      gkg::Filter3d< T, uint8_t, uint8_t > filter3d;
      gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
      gkg::FilterFunction< std::vector< T >, uint8_t >* filterFunction = 0;
      if ( filter3dProcess.getFilterType() == "average" )
      {

        filterFunction = new gkg::AverageFilter< std::vector< T >, uint8_t >;

      }
      else if ( filter3dProcess.getFilterType() == "median" )
      {

        filterFunction = new gkg::MedianFilter< std::vector< T >, uint8_t >;

      }
      filter3d.filter( volumeIn,
                       mask,
                       equalToFunction,
                       *filterFunction,
                       filter3dProcess.getNeighborhood3dType(),
                       volumeOut,
                       filter3dProcess.getVerbose() );
      delete filterFunction;
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << filter3dProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( filter3dProcess.getFileNameOut(),
                                        volumeOut,
                                        filter3dProcess.getAscii(),
                                        filter3dProcess.getFormat() );
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int16_t >::isSameType( 
                                                filter3dProcess.getOutType() ) )
    {

      gkg::Volume< int16_t > volumeOut;
      gkg::Filter3d< T, uint8_t, int16_t > filter3d;
      gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
      gkg::FilterFunction< std::vector< T >, int16_t >* filterFunction = 0;
      if ( filter3dProcess.getFilterType() == "average" )
      {

        filterFunction = new gkg::AverageFilter< std::vector< T >, int16_t >;

      }
      else if ( filter3dProcess.getFilterType() == "median" )
      {

        filterFunction = new gkg::MedianFilter< std::vector< T >, int16_t >;

      }
      filter3d.filter( volumeIn,
                       mask,
                       equalToFunction,
                       *filterFunction,
                       filter3dProcess.getNeighborhood3dType(),
                       volumeOut,
                       filter3dProcess.getVerbose() );
      delete filterFunction;
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << filter3dProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( filter3dProcess.getFileNameOut(),
                                        volumeOut,
                                        filter3dProcess.getAscii(),
                                        filter3dProcess.getFormat() );
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint16_t >::isSameType( 
                                                filter3dProcess.getOutType() ) )
    {

      gkg::Volume< uint16_t > volumeOut;
      gkg::Filter3d< T, uint8_t, uint16_t > filter3d;
      gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
      gkg::FilterFunction< std::vector< T >, uint16_t >* filterFunction = 0;
      if ( filter3dProcess.getFilterType() == "average" )
      {

        filterFunction = new gkg::AverageFilter< std::vector< T >, uint16_t >;

      }
      else if ( filter3dProcess.getFilterType() == "median" )
      {

        filterFunction = new gkg::MedianFilter< std::vector< T >, uint16_t >;

      }
      filter3d.filter( volumeIn,
                       mask,
                       equalToFunction,
                       *filterFunction,
                       filter3dProcess.getNeighborhood3dType(),
                       volumeOut,
                       filter3dProcess.getVerbose() );
      delete filterFunction;
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << filter3dProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( filter3dProcess.getFileNameOut(),
                                        volumeOut,
                                        filter3dProcess.getAscii(),
                                        filter3dProcess.getFormat() );
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int32_t >::isSameType( 
                                                filter3dProcess.getOutType() ) )
    {

      gkg::Volume< int32_t > volumeOut;
      gkg::Filter3d< T, uint8_t, int32_t > filter3d;
      gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
      gkg::FilterFunction< std::vector< T >, int32_t >* filterFunction = 0;
      if ( filter3dProcess.getFilterType() == "average" )
      {

        filterFunction = new gkg::AverageFilter< std::vector< T >, int32_t >;

      }
      else if ( filter3dProcess.getFilterType() == "median" )
      {

        filterFunction = new gkg::MedianFilter< std::vector< T >, int32_t >;

      }
      filter3d.filter( volumeIn,
                       mask,
                       equalToFunction,
                       *filterFunction,
                       filter3dProcess.getNeighborhood3dType(),
                       volumeOut,
                       filter3dProcess.getVerbose() );
      delete filterFunction;
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << filter3dProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( filter3dProcess.getFileNameOut(),
                                        volumeOut,
                                        filter3dProcess.getAscii(),
                                        filter3dProcess.getFormat() );
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint32_t >::isSameType( 
                                                filter3dProcess.getOutType() ) )
    {

      gkg::Volume< uint32_t > volumeOut;
      gkg::Filter3d< T, uint8_t, uint32_t > filter3d;
      gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
      gkg::FilterFunction< std::vector< T >, uint32_t >* filterFunction = 0;
      if ( filter3dProcess.getFilterType() == "average" )
      {

        filterFunction = new gkg::AverageFilter< std::vector< T >, uint32_t >;

      }
      else if ( filter3dProcess.getFilterType() == "median" )
      {

        filterFunction = new gkg::MedianFilter< std::vector< T >, uint32_t >;

      }
      filter3d.filter( volumeIn,
                       mask,
                       equalToFunction,
                       *filterFunction,
                       filter3dProcess.getNeighborhood3dType(),
                       volumeOut,
                       filter3dProcess.getVerbose() );
      delete filterFunction;
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << filter3dProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( filter3dProcess.getFileNameOut(),
                                        volumeOut,
                                        filter3dProcess.getAscii(),
                                        filter3dProcess.getFormat() );
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int64_t >::isSameType( 
                                                filter3dProcess.getOutType() ) )
    {

      gkg::Volume< int64_t > volumeOut;
      gkg::Filter3d< T, uint8_t, int64_t > filter3d;
      gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
      gkg::FilterFunction< std::vector< T >, int64_t >* filterFunction = 0;
      if ( filter3dProcess.getFilterType() == "average" )
      {

        filterFunction = new gkg::AverageFilter< std::vector< T >, int64_t >;

      }
      else if ( filter3dProcess.getFilterType() == "median" )
      {

        filterFunction = new gkg::MedianFilter< std::vector< T >, int64_t >;

      }
      filter3d.filter( volumeIn,
                       mask,
                       equalToFunction,
                       *filterFunction,
                       filter3dProcess.getNeighborhood3dType(),
                       volumeOut,
                       filter3dProcess.getVerbose() );
      delete filterFunction;
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << filter3dProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( filter3dProcess.getFileNameOut(),
                                        volumeOut,
                                        filter3dProcess.getAscii(),
                                        filter3dProcess.getFormat() );
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint64_t >::isSameType( 
                                                filter3dProcess.getOutType() ) )
    {

      gkg::Volume< uint64_t > volumeOut;
      gkg::Filter3d< T, uint8_t, uint64_t > filter3d;
      gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
      gkg::FilterFunction< std::vector< T >, uint64_t >* filterFunction = 0;
      if ( filter3dProcess.getFilterType() == "average" )
      {

        filterFunction = new gkg::AverageFilter< std::vector< T >, uint64_t >;

      }
      else if ( filter3dProcess.getFilterType() == "median" )
      {

        filterFunction = new gkg::MedianFilter< std::vector< T >, uint64_t >;

      }
      filter3d.filter( volumeIn,
                       mask,
                       equalToFunction,
                       *filterFunction,
                       filter3dProcess.getNeighborhood3dType(),
                       volumeOut,
                       filter3dProcess.getVerbose() );
      delete filterFunction;
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << filter3dProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( filter3dProcess.getFileNameOut(),
                                        volumeOut,
                                        filter3dProcess.getAscii(),
                                        filter3dProcess.getFormat() );
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< float >::isSameType( filter3dProcess.getOutType() ) )
    {

      gkg::Volume< float > volumeOut;
      gkg::Filter3d< T, uint8_t, float > filter3d;
      gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
      gkg::FilterFunction< std::vector< T >, float >* filterFunction = 0;
      if ( filter3dProcess.getFilterType() == "average" )
      {

        filterFunction = new gkg::AverageFilter< std::vector< T >, float >;

      }
      else if ( filter3dProcess.getFilterType() == "median" )
      {

        filterFunction = new gkg::MedianFilter< std::vector< T >, float >;

      }
      filter3d.filter( volumeIn,
                       mask,
                       equalToFunction,
                       *filterFunction,
                       filter3dProcess.getNeighborhood3dType(),
                       volumeOut,
                       filter3dProcess.getVerbose() );
      delete filterFunction;
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << filter3dProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( filter3dProcess.getFileNameOut(),
                                        volumeOut,
                                        filter3dProcess.getAscii(),
                                        filter3dProcess.getFormat() );
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< double >::isSameType( 
                                                filter3dProcess.getOutType() ) )
    {

      gkg::Volume< double > volumeOut;
      gkg::Filter3d< T, uint8_t, double > filter3d;
      gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
      gkg::FilterFunction< std::vector< T >, double >* filterFunction = 0;
      if ( filter3dProcess.getFilterType() == "average" )
      {

        filterFunction = new gkg::AverageFilter< std::vector< T >, double >;

      }
      else if ( filter3dProcess.getFilterType() == "median" )
      {

        filterFunction = new gkg::MedianFilter< std::vector< T >, double >;

      }
      filter3d.filter( volumeIn,
                       mask,
                       equalToFunction,
                       *filterFunction,
                       filter3dProcess.getNeighborhood3dType(),
                       volumeOut,
                       filter3dProcess.getVerbose() );
      delete filterFunction;
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << filter3dProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( filter3dProcess.getFileNameOut(),
                                        volumeOut,
                                        filter3dProcess.getAscii(),
                                        filter3dProcess.getFormat() );
      if ( filter3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void Filter3dProcess::filter( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   Filter3dCommand
//


gkg::Filter3dCommand::Filter3dCommand( int32_t argc,
                                       char* argv[],
                                       bool loadPlugin,
                                       bool removeFirst )
                    : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::Filter3dCommand::Filter3dCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Filter3dCommand::Filter3dCommand( const std::string& fileNameIn,
                     		       const std::string& fileNameOut,
                     		       const std::string& outType,
                     		       const std::string& filterType,
                     		       const std::string& fileNameMask,
                     		       const std::string& neighborhood,
                     		       bool ascii,
                     		       const std::string& format,
                     		       bool verbose )
                    : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, outType, filterType, fileNameMask,
             neighborhood, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::Filter3dCommand::Filter3dCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& outType, const std::string& filterType, "
             "const std::string& fileNameMask, "
             "const std::string& neighborhood, bool ascii, "
             "const std::string& format, bool verbose )" );

}


gkg::Filter3dCommand::Filter3dCommand( const gkg::Dictionary& parameters )
                    : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, outType );
    DECLARE_STRING_PARAMETER( parameters, std::string, filterType );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, neighborhood );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, outType, filterType, fileNameMask,
             neighborhood, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::Filter3dCommand::Filter3dCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Filter3dCommand::~Filter3dCommand()
{
}


std::string gkg::Filter3dCommand::getStaticName()
{

  try
  {

    return "Filter3d";

  }
  GKG_CATCH( "std::string gkg::Filter3dCommand::getStaticName()" );

}


void gkg::Filter3dCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outType = "";
    std::string filterType = "";
    std::string fileNameMask = "";
    std::string neighborhood = "26";
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "3D filtering",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-t",
                                 "Output type (default=input type)",
                                 outType,
                                 true );
    application.addSingleOption( "-f",
                                 "Filter type :\n"
                                 "- average\n"
                                 "- median\n",
                                 filterType,
                                 true );
    application.addSingleOption( "-m",
                                 "Mask (default=whole input image)",
                                 fileNameMask,
                                 true );
    application.addSingleOption( "-n",
                                 "Neighborhood type (default=26) :\n"
                                 "- 4xy\n"
                                 "- 4xz\n"
                                 "- 4yz\n"
                                 "- 8xy\n"
                                 "- 8xz\n"
                                 "- 8yz\n"
                                 "- 6\n"
                                 "- 18\n"
                                 "- 26\n",
                                 neighborhood,
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

    execute( fileNameIn, fileNameOut, outType, filterType, fileNameMask,
             neighborhood, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Filter3dCommand::parse()" );

}


void gkg::Filter3dCommand::execute( const std::string& fileNameIn,
                     		    const std::string& fileNameOut,
                     		    const std::string& outType,
                     		    const std::string& filterType,
                     		    const std::string& fileNameMask,
                     		    const std::string& neighborhood,
                     		    bool ascii,
                     		    const std::string& format,
                     		    bool verbose )
{

  try
  {

    // sanity checks
    if ( !outType.empty() &&
         ( ! gkg::TypeOf< int8_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint8_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int16_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint16_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int32_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint32_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int64_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint64_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< float >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< double >::isSameType( outType ) ) )
    {

      throw std::runtime_error( "invalid output type" );

    }

    if ( ( filterType != "average" ) &&
         ( filterType != "median" ) )
    {

      throw std::runtime_error( "invalid filter type" );

    }

    gkg::Neighborhood3d::Type neighborhood3dType;

    if ( neighborhood == "4xy" )
    {

      neighborhood3dType = gkg::Neighborhood3d::Neighborhood2d_4xy;

    }
    else if ( neighborhood == "4xz" )
    {

      neighborhood3dType = gkg::Neighborhood3d::Neighborhood2d_4xz;

    }
    else if ( neighborhood == "4yz" )
    {

      neighborhood3dType = gkg::Neighborhood3d::Neighborhood2d_4yz;

    }
    else if ( neighborhood == "8xy" )
    {

      neighborhood3dType = gkg::Neighborhood3d::Neighborhood2d_8xy;

    }
    else if ( neighborhood == "8xz" )
    {

      neighborhood3dType = gkg::Neighborhood3d::Neighborhood2d_8xz;

    }
    else if ( neighborhood == "8yz" )
    {

      neighborhood3dType = gkg::Neighborhood3d::Neighborhood2d_8yz;

    }
    else if ( neighborhood == "6" )
    {

      neighborhood3dType = gkg::Neighborhood3d::Neighborhood3d_6;

    }
    else if ( neighborhood == "18" )
    {

      neighborhood3dType = gkg::Neighborhood3d::Neighborhood3d_18;

    }
    else if ( neighborhood == "26" )
    {

      neighborhood3dType = gkg::Neighborhood3d::Neighborhood3d_26;

    }
    else
    {

      throw std::runtime_error( "bad neighborhood type" );

    }

    if ( verbose )
    {

      std::cout << "neighborhood type : " << neighborhood << std::endl;

    }

    //
    // launching 3D filter process
    //
    Filter3dProcess filter3dProcess( fileNameOut,
                                     outType,
                                     filterType,
                                     fileNameMask,
                                     neighborhood3dType,
                                     ascii,
                                     format,
                                     verbose );
    filter3dProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::Filter3dCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& outType, const std::string& filterType, "
             "const std::string& fileNameMask, "
             "const std::string& neighborhood, bool ascii, "
             "const std::string& format, bool verbose )" );

}


RegisterCommandCreator( Filter3dCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( outType ) +
    			DECLARE_STRING_PARAMETER_HELP( outType ) +
    			DECLARE_STRING_PARAMETER_HELP( filterType ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
