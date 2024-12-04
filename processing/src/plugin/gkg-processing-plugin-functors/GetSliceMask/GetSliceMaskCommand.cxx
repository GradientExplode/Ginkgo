#include <gkg-processing-plugin-functors/GetSliceMask/GetSliceMaskCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/Binarizer_i.h>
#include <gkg-processing-algobase/LowerThanFunction_i.h>
#include <gkg-processing-algobase/LowerOrEqualToFunction_i.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-algobase/DifferentFromFunction_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/GreaterThanFunction_i.h>
#include <gkg-processing-algobase/BetweenOrEqualToFunction_i.h>
#include <gkg-processing-algobase/BetweenFunction_i.h>
#include <gkg-processing-algobase/OutsideOrEqualToFunction_i.h>
#include <gkg-processing-algobase/OutsideFunction_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


//
//   SliceMaskProcess
//


class SliceMaskProcess : public gkg::Process
{

  public:

    SliceMaskProcess( const std::string& fileNameOut,
                      const int32_t& xSliceIndex,
                      const int32_t& ySliceIndex,
                      const int32_t& zSliceIndex,
                      const std::string& outType,
                      const std::string& mode,
                      const double& threshold1,
                      const double& threshold2,
                      const double& foreground,
                      const double& background,
                      const bool& ascii,
                      const std::string& format,
                      const bool& verbose );

    const std::string& getFileNameOut() const;
    const int32_t& getXSliceIndex() const;
    const int32_t& getYSliceIndex() const;
    const int32_t& getZSliceIndex() const;
    const std::string& getOutType() const;
    const std::string& getMode() const;
    const double& getThreshold1() const;
    const double& getThreshold2() const;
    const double& getForeground() const;
    const double& getBackground() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static bool check( int32_t xSliceIndex,
                       int32_t ySliceIndex,
                       int32_t zSliceIndex,
                       int32_t x,
                       int32_t y,
                       int32_t z );
    template < class T >
    static void mask( gkg::Process& process,
                      const std::string& fileNameIn,
                      const gkg::AnalyzedObject&,
                      const std::string& );

    const std::string& _fileNameOut;
    const int32_t& _xSliceIndex;
    const int32_t& _ySliceIndex;
    const int32_t& _zSliceIndex;
    const std::string& _outType;
    const std::string& _mode;
    const double& _threshold1;
    const double& _threshold2;
    const double& _foreground;
    const double& _background;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


SliceMaskProcess::SliceMaskProcess( const std::string& fileNameOut,
                                    const int32_t& xSliceIndex,
                                    const int32_t& ySliceIndex,
                                    const int32_t& zSliceIndex,
                                    const std::string& outType,
                                    const std::string& mode,
                                    const double& threshold1,
                                    const double& threshold2,
                                    const double& foreground,
                                    const double& background,
                                    const bool& ascii,
                                    const std::string& format,
                                    const bool& verbose)
                 : gkg::Process(),
                   _fileNameOut( fileNameOut ),
                   _xSliceIndex( xSliceIndex ),
                   _ySliceIndex( ySliceIndex ),
                   _zSliceIndex( zSliceIndex ),
                   _outType( outType ),
                   _mode( mode ),
                   _threshold1( threshold1 ),
                   _threshold2( threshold2 ),
                   _foreground( foreground ),
                   _background( background ),
                   _ascii( ascii ),
                   _format( format ),
                   _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &SliceMaskProcess::mask< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &SliceMaskProcess::mask< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &SliceMaskProcess::mask< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &SliceMaskProcess::mask< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &SliceMaskProcess::mask< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &SliceMaskProcess::mask< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &SliceMaskProcess::mask< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &SliceMaskProcess::mask< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &SliceMaskProcess::mask< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &SliceMaskProcess::mask< double > );

}


const std::string& SliceMaskProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const int32_t& SliceMaskProcess::getXSliceIndex() const
{

  return _xSliceIndex;

}


const int32_t& SliceMaskProcess::getYSliceIndex() const
{

  return _ySliceIndex;

}


const int32_t& SliceMaskProcess::getZSliceIndex() const
{

  return _zSliceIndex;

}


const std::string& SliceMaskProcess::getOutType() const
{

  return _outType;

}


const std::string& SliceMaskProcess::getMode() const
{

  return _mode;

}


const double& SliceMaskProcess::getThreshold1() const
{

  return _threshold1;

}


const double& SliceMaskProcess::getThreshold2() const
{

  return _threshold2;

}


const double& SliceMaskProcess::getForeground() const
{

  return _foreground;

}


const double& SliceMaskProcess::getBackground() const
{

  return _background;

}


const bool& SliceMaskProcess::getAscii() const
{

  return _ascii;

}


const std::string& SliceMaskProcess::getFormat() const
{

  return _format;

}


const bool& SliceMaskProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
bool SliceMaskProcess::check( int32_t xSliceIndex,
                              int32_t ySliceIndex,
                              int32_t zSliceIndex,
                              int32_t x,
                              int32_t y,
                              int32_t z )
{

  return ( xSliceIndex == x ) ||
         ( ySliceIndex == y ) ||
         ( zSliceIndex == z );

}

template < class T >
void SliceMaskProcess::mask( gkg::Process& process,
                             const std::string& fileNameIn,
                             const gkg::AnalyzedObject&,
                             const std::string& )
{

  try
  {

    SliceMaskProcess&
      sliceMaskProcess = static_cast< SliceMaskProcess& >( process );

    // reading data
    if ( sliceMaskProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( sliceMaskProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // sanity checks
    if ( sliceMaskProcess.getXSliceIndex() > 0 )
    {

      if ( sliceMaskProcess.getXSliceIndex() >= volumeIn.getSizeX() )
      {

        throw std::runtime_error( "X index incompatible with input size" );

      }

    }
    else if ( sliceMaskProcess.getYSliceIndex() > 0 )
    {

      if ( sliceMaskProcess.getYSliceIndex() >= volumeIn.getSizeY() )
      {

        throw std::runtime_error( "Y index incompatible with input size" );

      }

    }
    else if ( sliceMaskProcess.getZSliceIndex() > 0 )
    {

      if ( sliceMaskProcess.getZSliceIndex() >= volumeIn.getSizeZ() )
      {

        throw std::runtime_error( "Z index incompatible with input size" );

      }

    }

    if ( sliceMaskProcess.getVerbose() )
    {

      std::cout << "extracting slice mask : " << std::flush;

    }

    gkg::TestFunction< T >* testFunction = 0;
    if ( sliceMaskProcess.getMode() == "lt" )
    {

      testFunction = new gkg::LowerThanFunction< T >(
                                        ( T )sliceMaskProcess.getThreshold1() );

    }
    else if ( sliceMaskProcess.getMode() == "le" )
    {

      testFunction = new gkg::LowerOrEqualToFunction< T >(
                                        ( T )sliceMaskProcess.getThreshold1() );

    }
    else if ( sliceMaskProcess.getMode() == "eq" )
    {

      testFunction = new gkg::EqualToFunction< T >(
                                        ( T )sliceMaskProcess.getThreshold1() );

    }
    else if ( sliceMaskProcess.getMode() == "di" )
    {

      testFunction = new gkg::DifferentFromFunction< T >(
                                        ( T )sliceMaskProcess.getThreshold1() );

    }
    else if ( sliceMaskProcess.getMode() == "ge" )
    {

      testFunction = new gkg::GreaterOrEqualToFunction< T >(
                                        ( T )sliceMaskProcess.getThreshold1() );

    }
    else if ( sliceMaskProcess.getMode() == "gt" )
    {

      testFunction = new gkg::GreaterThanFunction< T >(
                                        ( T )sliceMaskProcess.getThreshold1() );

    }
    else if ( sliceMaskProcess.getMode() == "be" )
    {

      testFunction = new gkg::BetweenOrEqualToFunction< T >(
                                        ( T )sliceMaskProcess.getThreshold1(),
                                        ( T )sliceMaskProcess.getThreshold2() );

    }
    else if ( sliceMaskProcess.getMode() == "bt" )
    {

      testFunction = new gkg::BetweenFunction< T >(
                                        ( T )sliceMaskProcess.getThreshold1(),
                                        ( T )sliceMaskProcess.getThreshold2() );

    }
    else if ( sliceMaskProcess.getMode() == "oe" )
    {

      testFunction = new gkg::OutsideOrEqualToFunction< T >(
                                        ( T )sliceMaskProcess.getThreshold1(),
                                        ( T )sliceMaskProcess.getThreshold2() );

    }
    else if ( sliceMaskProcess.getMode() == "ou" )
    {

      testFunction = new gkg::OutsideFunction< T >(
                                        ( T )sliceMaskProcess.getThreshold1(),
                                        ( T )sliceMaskProcess.getThreshold2() );

    }

    if ( sliceMaskProcess.getOutType().empty() )
    {

      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< T > >
        binarizer( *testFunction,
                   ( T )sliceMaskProcess.getForeground(),
                   ( T )sliceMaskProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeIn );

      int32_t x, y, z, t;
      for ( t = 0; t < volumeIn.getSizeT(); t++ )
      {

        for ( z = 0; z < volumeIn.getSizeZ(); z++ )
        {

          for ( y = 0; y < volumeIn.getSizeY(); y++ )
          {

            for ( x = 0; x < volumeIn.getSizeX(); x++ )
            {

              if ( !SliceMaskProcess::check< T >(
                           sliceMaskProcess.getXSliceIndex(),
                           sliceMaskProcess.getYSliceIndex(),
                           sliceMaskProcess.getZSliceIndex(),
                           x, y, z ) )
              {

                volumeIn( x, y, z, t )
                  = ( T )sliceMaskProcess.getBackground();

              }

            }

          }

        }

      }

      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << sliceMaskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( sliceMaskProcess.getFileNameOut(),
                                        volumeIn,
                                        sliceMaskProcess.getAscii(),
                                        sliceMaskProcess.getFormat() );
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    if ( gkg::TypeOf< int8_t >::isSameType( sliceMaskProcess.getOutType() ) )
    {

      gkg::Volume< int8_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< int8_t > >
        binarizer( *testFunction,
                   ( int8_t )sliceMaskProcess.getForeground(),
                   ( int8_t )sliceMaskProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      int32_t x, y, z, t;
      for ( t = 0; t < volumeOut.getSizeT(); t++ )
      {

        for ( z = 0; z < volumeOut.getSizeZ(); z++ )
        {

          for ( y = 0; y < volumeOut.getSizeY(); y++ )
          {

            for ( x = 0; x < volumeOut.getSizeX(); x++ )
            {

              if ( !SliceMaskProcess::check< int8_t >(
                           sliceMaskProcess.getXSliceIndex(),
                           sliceMaskProcess.getYSliceIndex(),
                           sliceMaskProcess.getZSliceIndex(),
                           x, y, z ) )
              {

                volumeOut( x, y, z, t )
                  = ( int8_t )sliceMaskProcess.getBackground();

              }

            }

          }

        }

      }

      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << sliceMaskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( sliceMaskProcess.getFileNameOut(),
                                        volumeOut,
                                        sliceMaskProcess.getAscii(),
                                        sliceMaskProcess.getFormat() );
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint8_t >::isSameType( 
                                               sliceMaskProcess.getOutType() ) )
    {

      gkg::Volume< uint8_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< uint8_t > >
        binarizer( *testFunction,
                   ( uint8_t )sliceMaskProcess.getForeground(),
                   ( uint8_t )sliceMaskProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      int32_t x, y, z, t;
      for ( t = 0; t < volumeOut.getSizeT(); t++ )
      {

        for ( z = 0; z < volumeOut.getSizeZ(); z++ )
        {

          for ( y = 0; y < volumeOut.getSizeY(); y++ )
          {

            for ( x = 0; x < volumeOut.getSizeX(); x++ )
            {

              if ( !SliceMaskProcess::check< uint8_t >(
                           sliceMaskProcess.getXSliceIndex(),
                           sliceMaskProcess.getYSliceIndex(),
                           sliceMaskProcess.getZSliceIndex(),
                           x, y, z ) )
              {

                volumeOut( x, y, z, t )
                  = ( uint8_t )sliceMaskProcess.getBackground();

              }

            }

          }

        }

      }

      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << sliceMaskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( sliceMaskProcess.getFileNameOut(),
                                        volumeOut,
                                        sliceMaskProcess.getAscii(),
                                        sliceMaskProcess.getFormat() );
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int16_t >::isSameType( 
                                               sliceMaskProcess.getOutType() ) )
    {

      gkg::Volume< int16_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< int16_t > >
        binarizer( *testFunction,
                   ( int16_t )sliceMaskProcess.getForeground(),
                   ( int16_t )sliceMaskProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      int32_t x, y, z, t;
      for ( t = 0; t < volumeOut.getSizeT(); t++ )
      {

        for ( z = 0; z < volumeOut.getSizeZ(); z++ )
        {

          for ( y = 0; y < volumeOut.getSizeY(); y++ )
          {

            for ( x = 0; x < volumeOut.getSizeX(); x++ )
            {

              if ( !SliceMaskProcess::check< int16_t >(
                           sliceMaskProcess.getXSliceIndex(),
                           sliceMaskProcess.getYSliceIndex(),
                           sliceMaskProcess.getZSliceIndex(),
                           x, y, z ) )
              {

                volumeOut( x, y, z, t )
                  = ( int16_t )sliceMaskProcess.getBackground();

              }

            }

          }

        }

      }

      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << sliceMaskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( sliceMaskProcess.getFileNameOut(),
                                        volumeOut,
                                        sliceMaskProcess.getAscii(),
                                        sliceMaskProcess.getFormat() );
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint16_t >::isSameType( 
                                               sliceMaskProcess.getOutType() ) )
    {

      gkg::Volume< uint16_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< uint16_t > >
        binarizer( *testFunction,
                   ( uint16_t )sliceMaskProcess.getForeground(),
                   ( uint16_t )sliceMaskProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      int32_t x, y, z, t;
      for ( t = 0; t < volumeOut.getSizeT(); t++ )
      {

        for ( z = 0; z < volumeOut.getSizeZ(); z++ )
        {

          for ( y = 0; y < volumeOut.getSizeY(); y++ )
          {

            for ( x = 0; x < volumeOut.getSizeX(); x++ )
            {

              if ( !SliceMaskProcess::check< uint16_t >(
                           sliceMaskProcess.getXSliceIndex(),
                           sliceMaskProcess.getYSliceIndex(),
                           sliceMaskProcess.getZSliceIndex(),
                           x, y, z ) )
              {

                volumeOut( x, y, z, t )
                  = ( uint16_t )sliceMaskProcess.getBackground();

              }

            }

          }

        }

      }

      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << sliceMaskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( sliceMaskProcess.getFileNameOut(),
                                        volumeOut,
                                        sliceMaskProcess.getAscii(),
                                        sliceMaskProcess.getFormat() );
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int32_t >::isSameType( 
                                               sliceMaskProcess.getOutType() ) )
    {

      gkg::Volume< int32_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< int32_t > >
        binarizer( *testFunction,
                   ( int32_t )sliceMaskProcess.getForeground(),
                   ( int32_t )sliceMaskProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      int32_t x, y, z, t;
      for ( t = 0; t < volumeOut.getSizeT(); t++ )
      {

        for ( z = 0; z < volumeOut.getSizeZ(); z++ )
        {

          for ( y = 0; y < volumeOut.getSizeY(); y++ )
          {

            for ( x = 0; x < volumeOut.getSizeX(); x++ )
            {

              if ( !SliceMaskProcess::check< int32_t >(
                           sliceMaskProcess.getXSliceIndex(),
                           sliceMaskProcess.getYSliceIndex(),
                           sliceMaskProcess.getZSliceIndex(),
                           x, y, z ) )
              {

                volumeOut( x, y, z, t )
                  = ( int32_t )sliceMaskProcess.getBackground();

              }

            }

          }

        }

      }

      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << sliceMaskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( sliceMaskProcess.getFileNameOut(),
                                        volumeOut,
                                        sliceMaskProcess.getAscii(),
                                        sliceMaskProcess.getFormat() );
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint32_t >::isSameType( 
                                               sliceMaskProcess.getOutType() ) )
    {

      gkg::Volume< uint32_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< uint32_t > >
        binarizer( *testFunction,
                   ( uint32_t )sliceMaskProcess.getForeground(),
                   ( uint32_t )sliceMaskProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      int32_t x, y, z, t;
      for ( t = 0; t < volumeOut.getSizeT(); t++ )
      {

        for ( z = 0; z < volumeOut.getSizeZ(); z++ )
        {

          for ( y = 0; y < volumeOut.getSizeY(); y++ )
          {

            for ( x = 0; x < volumeOut.getSizeX(); x++ )
            {

              if ( !SliceMaskProcess::check< uint32_t >(
                           sliceMaskProcess.getXSliceIndex(),
                           sliceMaskProcess.getYSliceIndex(),
                           sliceMaskProcess.getZSliceIndex(),
                           x, y, z ) )
              {

                volumeOut( x, y, z, t )
                  = ( uint32_t )sliceMaskProcess.getBackground();

              }

            }

          }

        }

      }

      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << sliceMaskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( sliceMaskProcess.getFileNameOut(),
                                        volumeOut,
                                        sliceMaskProcess.getAscii(),
                                        sliceMaskProcess.getFormat() );
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int64_t >::isSameType( 
                                               sliceMaskProcess.getOutType() ) )
    {

      gkg::Volume< int64_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< int64_t > >
        binarizer( *testFunction,
                   ( int64_t )sliceMaskProcess.getForeground(),
                   ( int64_t )sliceMaskProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      int32_t x, y, z, t;
      for ( t = 0; t < volumeOut.getSizeT(); t++ )
      {

        for ( z = 0; z < volumeOut.getSizeZ(); z++ )
        {

          for ( y = 0; y < volumeOut.getSizeY(); y++ )
          {

            for ( x = 0; x < volumeOut.getSizeX(); x++ )
            {

              if ( !SliceMaskProcess::check< int64_t >(
                           sliceMaskProcess.getXSliceIndex(),
                           sliceMaskProcess.getYSliceIndex(),
                           sliceMaskProcess.getZSliceIndex(),
                           x, y, z ) )
              {

                volumeOut( x, y, z, t )
                  = ( int64_t )sliceMaskProcess.getBackground();

              }

            }

          }

        }

      }

      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << sliceMaskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( sliceMaskProcess.getFileNameOut(),
                                        volumeOut,
                                        sliceMaskProcess.getAscii(),
                                        sliceMaskProcess.getFormat() );
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint64_t >::isSameType( 
                                               sliceMaskProcess.getOutType() ) )
    {

      gkg::Volume< uint64_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< uint64_t > >
        binarizer( *testFunction,
                   ( uint64_t )sliceMaskProcess.getForeground(),
                   ( uint64_t )sliceMaskProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      int32_t x, y, z, t;
      for ( t = 0; t < volumeOut.getSizeT(); t++ )
      {

        for ( z = 0; z < volumeOut.getSizeZ(); z++ )
        {

          for ( y = 0; y < volumeOut.getSizeY(); y++ )
          {

            for ( x = 0; x < volumeOut.getSizeX(); x++ )
            {

              if ( !SliceMaskProcess::check< uint64_t >(
                           sliceMaskProcess.getXSliceIndex(),
                           sliceMaskProcess.getYSliceIndex(),
                           sliceMaskProcess.getZSliceIndex(),
                           x, y, z ) )
              {

                volumeOut( x, y, z, t )
                  = ( uint64_t )sliceMaskProcess.getBackground();

              }

            }

          }

        }

      }

      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << sliceMaskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( sliceMaskProcess.getFileNameOut(),
                                        volumeOut,
                                        sliceMaskProcess.getAscii(),
                                        sliceMaskProcess.getFormat() );
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< float >::isSameType( 
                                               sliceMaskProcess.getOutType() ) )
    {

      gkg::Volume< float > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< float > >
        binarizer( *testFunction,
                   ( float )sliceMaskProcess.getForeground(),
                   ( float )sliceMaskProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      int32_t x, y, z, t;
      for ( t = 0; t < volumeOut.getSizeT(); t++ )
      {

        for ( z = 0; z < volumeOut.getSizeZ(); z++ )
        {

          for ( y = 0; y < volumeOut.getSizeY(); y++ )
          {

            for ( x = 0; x < volumeOut.getSizeX(); x++ )
            {

              if ( !SliceMaskProcess::check< float >(
                           sliceMaskProcess.getXSliceIndex(),
                           sliceMaskProcess.getYSliceIndex(),
                           sliceMaskProcess.getZSliceIndex(),
                           x, y, z ) )
              {

                volumeOut( x, y, z, t )
                  = ( float )sliceMaskProcess.getBackground();

              }

            }

          }

        }

      }

      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << sliceMaskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( sliceMaskProcess.getFileNameOut(),
                                        volumeOut,
                                        sliceMaskProcess.getAscii(),
                                        sliceMaskProcess.getFormat() );
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< double >::isSameType( 
                                               sliceMaskProcess.getOutType() ) )
    {

      gkg::Volume< double > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< double > >
        binarizer( *testFunction,
                   ( double )sliceMaskProcess.getForeground(),
                   ( double )sliceMaskProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      int32_t x, y, z, t;
      for ( t = 0; t < volumeOut.getSizeT(); t++ )
      {

        for ( z = 0; z < volumeOut.getSizeZ(); z++ )
        {

          for ( y = 0; y < volumeOut.getSizeY(); y++ )
          {

            for ( x = 0; x < volumeOut.getSizeX(); x++ )
            {

              if ( !SliceMaskProcess::check< double >(
                           sliceMaskProcess.getXSliceIndex(),
                           sliceMaskProcess.getYSliceIndex(),
                           sliceMaskProcess.getZSliceIndex(),
                           x, y, z ) )
              {

                volumeOut( x, y, z, t )
                  = ( double )sliceMaskProcess.getBackground();

              }

            }

          }

        }

      }

      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << sliceMaskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( sliceMaskProcess.getFileNameOut(),
                                        volumeOut,
                                        sliceMaskProcess.getAscii(),
                                        sliceMaskProcess.getFormat() );
      if ( sliceMaskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void SliceMaskProcess::mask( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   GetSliceMaskCommand
//


gkg::GetSliceMaskCommand::GetSliceMaskCommand( int32_t argc,
                                               char* argv[],
                                               bool loadPlugin,
                                               bool removeFirst )
                        : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::GetSliceMaskCommand::GetSliceMaskCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::GetSliceMaskCommand::GetSliceMaskCommand( const std::string& fileNameIn,
    			                       const std::string& fileNameOut,
    			 		       int32_t xSliceIndex,
    			 		       int32_t ySliceIndex,
    			 		       int32_t zSliceIndex,
    			 		       const std::string& outType,
    			 		       const std::string& mode,
    			 		       double threshold1,
    			 		       double threshold2,
    			 		       double foreground,
    			 		       double background,
    			 		       bool ascii,
    			 		       const std::string& format,
    			 		       bool verbose )
                        : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, xSliceIndex, ySliceIndex, zSliceIndex,
    	     outType, mode, threshold1, threshold2, foreground, background,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::GetSliceMaskCommand::GetSliceMaskCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "int32_t xSliceIndex, int32_t ySliceIndex, int32_t zSliceIndex, "
             "const std::string& outType, const std::string& mode, "
             "double threshold1, double threshold2, double foreground, "
    	     "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


gkg::GetSliceMaskCommand::GetSliceMaskCommand(
                                             const gkg::Dictionary& parameters )
                        : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, xSliceIndex );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, ySliceIndex );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, zSliceIndex );
    DECLARE_STRING_PARAMETER( parameters, std::string, outType );
    DECLARE_STRING_PARAMETER( parameters, std::string, mode );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold1 );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold2 );
    DECLARE_FLOATING_PARAMETER( parameters, double, foreground );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, xSliceIndex, ySliceIndex, zSliceIndex,
    	     outType, mode, threshold1, threshold2, foreground, background,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::GetSliceMaskCommand::GetSliceMaskCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GetSliceMaskCommand::~GetSliceMaskCommand()
{
}


std::string gkg::GetSliceMaskCommand::getStaticName()
{

  try
  {

    return "GetSliceMask";

  }
  GKG_CATCH( "std::string gkg::GetSliceMaskCommand::getStaticName()" );

}


void gkg::GetSliceMaskCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    int32_t xSliceIndex = -1;
    int32_t ySliceIndex = -1;
    int32_t zSliceIndex = -1;
    std::string outType = "";
    std::string mode;
    double threshold1;
    double threshold2 = 0.0;
    double foreground = 1.0;
    double background = 0.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Conditional binarization operation "
                                  "of container for building "
                                  "Axial/Sagittal/Coronal masks",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-x",
                                 "X slice index",
                                 xSliceIndex,
                                 true );
    application.addSingleOption( "-y",
                                 "Y slice index",
                                 ySliceIndex,
                                 true );
    application.addSingleOption( "-z",
                                 "Z slice index",
                                 zSliceIndex,
                                 true );
    application.addSingleOption( "-t",
                                 "Output type (default=input type)",
                                 outType,
                                 true );
    application.addSingleOption( "-m",
                                 "Threshold mode\n"
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
                                 mode );
    application.addSingleOption( "-t1",
                                 "First threshold",
                                 threshold1 );
    application.addSingleOption( "-t2",
                                 "Second threshold (required for be/bt/oe/ou)",
                                 threshold2,
                                 true );
    application.addSingleOption( "-f",
                                 "Foreground value (default=1)",
                                 foreground,
                                 true );
    application.addSingleOption( "-b",
                                 "Background value (default=0)",
                                 background,
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

    execute( fileNameIn, fileNameOut, xSliceIndex, ySliceIndex, zSliceIndex,
    	     outType, mode, threshold1, threshold2, foreground, background,
             ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::GetSliceMaskCommand::parse()" );

}


void gkg::GetSliceMaskCommand::execute( const std::string& fileNameIn,
    			 		const std::string& fileNameOut,
    			 		int32_t xSliceIndex,
    			 		int32_t ySliceIndex,
    			 		int32_t zSliceIndex,
    			 		const std::string& outType,
    			 		const std::string& mode,
    			 		double threshold1,
    			 		double threshold2,
    			 		double foreground,
    			 		double background,
    			 		bool ascii,
    			 		const std::string& format,
    			 		bool verbose )
{

  try
  {

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

      throw std::runtime_error( "invalid threshold mode" );

    }

    if ( ( xSliceIndex > 0 &&
           ySliceIndex > 0 ) ||
         ( xSliceIndex > 0 &&
           zSliceIndex > 0 ) ||
         ( ySliceIndex > 0 &&
           zSliceIndex > 0 ) )
    {

      throw std::runtime_error( "only one slice index should be set" );

    }
         

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
         

    //
    // launching slice mask process
    //
    SliceMaskProcess sliceMaskProcess( fileNameOut,
                                       xSliceIndex,
                                       ySliceIndex,
                                       zSliceIndex,
                                       outType,
                                       mode,
                                       threshold1,
                                       threshold2,
                                       foreground,
                                       background,
                                       ascii,
                                       format,
                                       verbose );
    sliceMaskProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::GetSliceMaskCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "int32_t xSliceIndex, int32_t ySliceIndex, int32_t zSliceIndex, "
             "const std::string& outType, const std::string& mode, "
             "double threshold1, double threshold2, double foreground, "
    	     "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( GetSliceMaskCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_INTEGER_PARAMETER_HELP( xSliceIndex ) +
    			DECLARE_INTEGER_PARAMETER_HELP( ySliceIndex ) +
    			DECLARE_INTEGER_PARAMETER_HELP( zSliceIndex ) +
    			DECLARE_STRING_PARAMETER_HELP( outType ) +
    			DECLARE_STRING_PARAMETER_HELP( mode ) +
    			DECLARE_FLOATING_PARAMETER_HELP( threshold1 ) +
    			DECLARE_FLOATING_PARAMETER_HELP( threshold2 ) +
    			DECLARE_FLOATING_PARAMETER_HELP( foreground ) +
    			DECLARE_FLOATING_PARAMETER_HELP( background ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
