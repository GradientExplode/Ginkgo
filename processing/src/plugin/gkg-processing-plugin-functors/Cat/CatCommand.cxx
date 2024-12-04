#include <gkg-processing-plugin-functors/Cat/CatCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>


//
// class ConcatenationProcess
//

class ConcatenationProcess : public gkg::Process
{

  public:

    ConcatenationProcess( const std::vector< std::string >& inputNames,
                          const std::string& outputName,
                          const std::string& format,
                          const bool& ascii,
                          const bool& verbose );

    const std::vector< std::string >& getInputNames() const;
    const std::string& getOutputName() const;
    const std::string& getFormat() const;
    const bool& getAscii() const;    
    const bool& getVerbose() const;    

  protected:

    const std::vector< std::string >& _inputNames;
    const std::string& _outputName;
    const std::string& _format;
    const bool& _ascii;
    const bool& _verbose;

};


ConcatenationProcess::ConcatenationProcess(
                                   const std::vector< std::string >& inputNames,
                                   const std::string& outputName,
                                   const std::string& format,
                                   const bool& ascii,
                                   const bool& verbose )
                     : gkg::Process( "Volume" ),
                       _inputNames( inputNames ),
                       _outputName( outputName ),
                       _format( format ),
                       _ascii( ascii ),
                       _verbose( verbose )
{
}


const std::vector< std::string >& ConcatenationProcess::getInputNames() const
{

  return _inputNames;

}


const std::string& ConcatenationProcess::getOutputName() const
{

  return _outputName;

}


const std::string& ConcatenationProcess::getFormat() const
{

  return _format;

}


const bool& ConcatenationProcess::getAscii() const
{

  return _ascii;

}


const bool& ConcatenationProcess::getVerbose() const
{

  return _verbose;

}


//
// class XConcatenationProcess
//

class XConcatenationProcess : public ConcatenationProcess
{

  public:

    XConcatenationProcess( const std::vector< std::string >& inputNames,
                           const std::string& outputName,
                           const std::string& format,
                           const bool& ascii,
                           const bool& verbose );

  private:

    template < class T >
    static void catVolume( gkg::Process& process,
                           const std::string& inputName,
                           const gkg::AnalyzedObject&,
                           const std::string& );

};


XConcatenationProcess::XConcatenationProcess(
                                   const std::vector< std::string >& inputNames,
                                   const std::string& outputName,
                                   const std::string& format,
                                   const bool& ascii,
                                   const bool& verbose )
                     : ConcatenationProcess( inputNames,
                                             outputName,
                                             format,
                                             ascii,
                                             verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &catVolume< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &catVolume< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &catVolume< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &catVolume< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &catVolume< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &catVolume< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &catVolume< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &catVolume< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &catVolume< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &catVolume< double > );

}


template < class T >
void 
XConcatenationProcess::catVolume( gkg::Process& process,
                                  const std::string& inputName,
                                  const gkg::AnalyzedObject&,
                                  const std::string& )
{

  try
  {

    ConcatenationProcess&
      concatenationProcess = static_cast< ConcatenationProcess& >( process );


    ////////////////////////////////////////////////////////////////////////////
    // reading input volume
    ////////////////////////////////////////////////////////////////////////////

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "reading input volume '"
                << inputName << "' : "
                << std::flush;

    }

    // reading volume
    gkg::Volume< T > volume;
    std::string format = gkg::Reader::getInstance().read( inputName, volume );

    // collecting size
    gkg::Vector3d< int32_t > size;
    int32_t sizeT = 1;
    volume.getSize( size, sizeT );

    // collecting resolution
    gkg::Vector3d< double > resolution;
    double resolutionT = 1.0;
    volume.getResolution( resolution, resolutionT );

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "dimensions of volume '"
                << inputName 
                << "' : ( "
                << size.x << ", "
                << size.y << ", "
                << size.z << ", "
                << sizeT << " ) ( "
                << resolution.x << "mm, "
                << resolution.y << "mm, "
                << resolution.z << "mm, "
                << resolutionT << " )"
                << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading other input volume proxies, making sanity checks and processing
    // output time dimension
    ////////////////////////////////////////////////////////////////////////////
    gkg::Volume< T > proxy;

    const std::vector< std::string >& 
      inputNames = concatenationProcess.getInputNames();
    int32_t volumeCount = ( int32_t )inputNames.size();
    int32_t v = 0;
    int32_t globalSizeX = size.x;
    for ( v = 1; v < volumeCount; v++ )
    {

      // reading other volume proxy
      gkg::Reader::getInstance().readHeader( inputNames[ v ], proxy );

      // collecting size
      gkg::Vector3d< int32_t > otherSize;
      int32_t otherSizeT = 1;
      proxy.getSize( otherSize, otherSizeT );

      // collecting resolution
      gkg::Vector3d< double > otherResolution;
      double otherResolutionT = 1.0;
      volume.getResolution( otherResolution, otherResolutionT );

      if ( concatenationProcess.getVerbose() )
      {

        std::cout << "dimensions of volume '"
                  << inputNames[ v ] 
                  << "' : ( "
                  << otherSize.x << ", "
                  << otherSize.y << ", "
                  << otherSize.z << ", "
                  << otherSizeT << " ) ( "
                  << otherResolution.x << "mm, "
                  << otherResolution.y << "mm, "
                  << otherResolution.z << "mm, "
                  << otherResolutionT << " )"
                  << std::endl;

      }

      // sanity checks
      if ( ( size.y != otherSize.y ) ||
           ( size.z != otherSize.z ) ||
           ( sizeT != otherSizeT ) ||
           ( resolution != otherResolution ) ||
           ( otherResolutionT != resolutionT ) )
      {

        throw std::runtime_error( "incompatible sizes or resolutions" );

      }

      // updating global Y size
      globalSizeX += otherSize.x;

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating output volume
    ////////////////////////////////////////////////////////////////////////////

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "allocating output volume : ( "
                << globalSizeX << ", "
                << size.y << ", "
                << size.z << ", "
                << sizeT << " ) ( "
                << resolution.x << "mm, "
                << resolution.y << "mm, "
                << resolution.z << "mm, "
                << resolutionT << " )"
                << std::endl;

    }
    gkg::Volume< T > concatenation( globalSizeX, size.y, size.z, sizeT );
    concatenation.setResolution( resolution, resolutionT );


    ////////////////////////////////////////////////////////////////////////////
    // concatenating
    ////////////////////////////////////////////////////////////////////////////

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "concatenating : " << std::flush;

    }

    int32_t offsetX = 0;
    for ( v = 0; v < volumeCount; v++ )
    {

      if ( concatenationProcess.getVerbose() )
      {

        if ( v != 0 )
        {

          std::cout << gkg::Eraser( 14 );

        }
        std::cout << " [ " << std::setw( 3 ) << v + 1 
        	  << " / " << std::setw( 3 ) << volumeCount
        	  << " ]" << std::flush;


      }


      // reading other volume proxy
      gkg::Reader::getInstance().read( inputNames[ v ], volume );
      gkg::Vector3d< int32_t > site;
      int32_t t = 0;
      for ( t = 0; t < sizeT; t++ )
      {

        for ( site.z = 0; site.z < size.z; site.z ++ )
        {

          for ( site.y = 0; site.y < size.y; site.y ++ )
          {

            for ( site.x = 0; site.x < size.x; site.x ++ )
            {

              concatenation( offsetX + site.x, site.y, site.z, t ) =
                volume( site, t );

            }

          }

        }

      }
      offsetX += volume.getSizeX();

    }
    if ( concatenationProcess.getVerbose() )
    {

      std::cout << std::endl;

    }
 

    ////////////////////////////////////////////////////////////////////////////
    // saving motion corrected volume
    ////////////////////////////////////////////////////////////////////////////
    
    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "writing concatenated '"
                << concatenationProcess.getOutputName()
                << "' volume : " << std::flush;

    }
    if ( !concatenationProcess.getFormat().empty() )
    {

      format = concatenationProcess.getFormat();

    }
    gkg::Writer::getInstance().write( concatenationProcess.getOutputName(),
                                      concatenation,
                                      concatenationProcess.getAscii(),
                                      format );
    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }
   
  }
  GKG_CATCH( "template < class T > "
             "void XConcatenationProcess::catVolume( "
             "gkg::Process& process, "
             "const std::string& inputName, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class YConcatenationProcess
//

class YConcatenationProcess : public ConcatenationProcess
{

  public:

    YConcatenationProcess( const std::vector< std::string >& inputNames,
                           const std::string& outputName,
                           const std::string& format,
                           const bool& ascii,
                           const bool& verbose );

  private:

    template < class T >
    static void catVolume( gkg::Process& process,
                           const std::string& inputName,
                           const gkg::AnalyzedObject&,
                           const std::string& );

};


YConcatenationProcess::YConcatenationProcess(
                                   const std::vector< std::string >& inputNames,
                                   const std::string& outputName,
                                   const std::string& format,
                                   const bool& ascii,
                                   const bool& verbose )
                     : ConcatenationProcess( inputNames,
                                             outputName,
                                             format,
                                             ascii,
                                             verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &catVolume< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &catVolume< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &catVolume< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &catVolume< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &catVolume< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &catVolume< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &catVolume< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &catVolume< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &catVolume< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &catVolume< double > );

}


template < class T >
void 
YConcatenationProcess::catVolume( gkg::Process& process,
                                  const std::string& inputName,
                                  const gkg::AnalyzedObject&,
                                  const std::string& )
{

  try
  {

    ConcatenationProcess&
      concatenationProcess = static_cast< ConcatenationProcess& >( process );


    ////////////////////////////////////////////////////////////////////////////
    // reading input volume
    ////////////////////////////////////////////////////////////////////////////

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "reading input volume '"
                << inputName << "' : "
                << std::flush;

    }

    // reading volume
    gkg::Volume< T > volume;
    std::string format = gkg::Reader::getInstance().read( inputName, volume );

    // collecting size
    gkg::Vector3d< int32_t > size;
    int32_t sizeT = 1;
    volume.getSize( size, sizeT );

    // collecting resolution
    gkg::Vector3d< double > resolution;
    double resolutionT = 1.0;
    volume.getResolution( resolution, resolutionT );

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "dimensions of volume '"
                << inputName 
                << "' : ( "
                << size.x << ", "
                << size.y << ", "
                << size.z << ", "
                << sizeT << " ) ( "
                << resolution.x << "mm, "
                << resolution.y << "mm, "
                << resolution.z << "mm, "
                << resolutionT << " )"
                << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading other input volume proxies, making sanity checks and processing
    // output time dimension
    ////////////////////////////////////////////////////////////////////////////
    gkg::Volume< T > proxy;

    const std::vector< std::string >& 
      inputNames = concatenationProcess.getInputNames();
    int32_t volumeCount = ( int32_t )inputNames.size();
    int32_t v = 0;
    int32_t globalSizeY = size.y;
    for ( v = 1; v < volumeCount; v++ )
    {

      // reading other volume proxy
      gkg::Reader::getInstance().readHeader( inputNames[ v ], proxy );

      // collecting size
      gkg::Vector3d< int32_t > otherSize;
      int32_t otherSizeT = 1;
      proxy.getSize( otherSize, otherSizeT );

      // collecting resolution
      gkg::Vector3d< double > otherResolution;
      double otherResolutionT = 1.0;
      volume.getResolution( otherResolution, otherResolutionT );

      if ( concatenationProcess.getVerbose() )
      {

        std::cout << "dimensions of volume '"
                  << inputNames[ v ] 
                  << "' : ( "
                  << otherSize.x << ", "
                  << otherSize.y << ", "
                  << otherSize.z << ", "
                  << otherSizeT << " ) ( "
                  << otherResolution.x << "mm, "
                  << otherResolution.y << "mm, "
                  << otherResolution.z << "mm, "
                  << otherResolutionT << " )"
                  << std::endl;

      }

      // sanity checks
      if ( ( size.x != otherSize.x ) ||
           ( size.z != otherSize.z ) ||
           ( sizeT != otherSizeT ) ||
           ( resolution != otherResolution ) ||
           ( otherResolutionT != resolutionT ) )
      {

        throw std::runtime_error( "incompatible sizes or resolutions" );

      }

      // updating global Y size
      globalSizeY += otherSize.y;

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating output volume
    ////////////////////////////////////////////////////////////////////////////

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "allocating output volume : ( "
                << size.x << ", "
                << globalSizeY << ", "
                << size.z << ", "
                << sizeT << " ) ( "
                << resolution.x << "mm, "
                << resolution.y << "mm, "
                << resolution.z << "mm, "
                << resolutionT << " )"
                << std::endl;

    }
    gkg::Volume< T > concatenation( size.x, globalSizeY, size.z, sizeT );
    concatenation.setResolution( resolution, resolutionT );


    ////////////////////////////////////////////////////////////////////////////
    // concatenating
    ////////////////////////////////////////////////////////////////////////////

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "concatenating : " << std::flush;

    }

    int32_t offsetY = 0;
    for ( v = 0; v < volumeCount; v++ )
    {

      if ( concatenationProcess.getVerbose() )
      {

        if ( v != 0 )
        {

          std::cout << gkg::Eraser( 14 );

        }
        std::cout << " [ " << std::setw( 3 ) << v + 1 
        	  << " / " << std::setw( 3 ) << volumeCount
        	  << " ]" << std::flush;


      }


      // reading other volume proxy
      gkg::Reader::getInstance().read( inputNames[ v ], volume );
      gkg::Vector3d< int32_t > site;
      int32_t t = 0;
      for ( t = 0; t < sizeT; t++ )
      {

        for ( site.z = 0; site.z < size.z; site.z ++ )
        {

          for ( site.y = 0; site.y < size.y; site.y ++ )
          {

            for ( site.x = 0; site.x < size.x; site.x ++ )
            {

              concatenation( site.x, offsetY + site.y, site.z, t ) =
                volume( site, t );

            }

          }

        }

      }
      offsetY += volume.getSizeY();

    }
    if ( concatenationProcess.getVerbose() )
    {

      std::cout << std::endl;

    }
 

    ////////////////////////////////////////////////////////////////////////////
    // saving motion corrected volume
    ////////////////////////////////////////////////////////////////////////////
    
    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "writing concatenated '"
                << concatenationProcess.getOutputName()
                << "' volume : " << std::flush;

    }
    if ( !concatenationProcess.getFormat().empty() )
    {

      format = concatenationProcess.getFormat();

    }
    gkg::Writer::getInstance().write( concatenationProcess.getOutputName(),
                                      concatenation,
                                      concatenationProcess.getAscii(),
                                      format );
    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }
   
  }
  GKG_CATCH( "template < class T > "
             "void YConcatenationProcess::catVolume( "
             "gkg::Process& process, "
             "const std::string& inputName, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class ZConcatenationProcess
//

class ZConcatenationProcess : public ConcatenationProcess
{

  public:

    ZConcatenationProcess( const std::vector< std::string >& inputNames,
                           const std::string& outputName,
                           const std::string& format,
                           const bool& ascii,
                           const bool& verbose );

  private:

    template < class T >
    static void catVolume( gkg::Process& process,
                           const std::string& inputName,
                           const gkg::AnalyzedObject&,
                           const std::string& );

};


ZConcatenationProcess::ZConcatenationProcess(
                                   const std::vector< std::string >& inputNames,
                                   const std::string& outputName,
                                   const std::string& format,
                                   const bool& ascii,
                                   const bool& verbose )
                     : ConcatenationProcess( inputNames,
                                             outputName,
                                             format,
                                             ascii,
                                             verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &catVolume< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &catVolume< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &catVolume< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &catVolume< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &catVolume< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &catVolume< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &catVolume< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &catVolume< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &catVolume< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &catVolume< double > );

}


template < class T >
void 
ZConcatenationProcess::catVolume( gkg::Process& process,
                                  const std::string& inputName,
                                  const gkg::AnalyzedObject&,
                                  const std::string& )
{

  try
  {

    ConcatenationProcess&
      concatenationProcess = static_cast< ConcatenationProcess& >( process );


    ////////////////////////////////////////////////////////////////////////////
    // reading input volume
    ////////////////////////////////////////////////////////////////////////////

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "reading input volume '"
                << inputName << "' : "
                << std::flush;

    }

    // reading volume
    gkg::Volume< T > volume;
    std::string format = gkg::Reader::getInstance().read( inputName, volume );

    // collecting size
    gkg::Vector3d< int32_t > size;
    int32_t sizeT = 1;
    volume.getSize( size, sizeT );

    // collecting resolution
    gkg::Vector3d< double > resolution;
    double resolutionT = 1.0;
    volume.getResolution( resolution, resolutionT );

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "dimensions of volume '"
                << inputName 
                << "' : ( "
                << size.x << ", "
                << size.y << ", "
                << size.z << ", "
                << sizeT << " ) ( "
                << resolution.x << "mm, "
                << resolution.y << "mm, "
                << resolution.z << "mm, "
                << resolutionT << " )"
                << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading other input volume proxies, making sanity checks and processing
    // output time dimension
    ////////////////////////////////////////////////////////////////////////////
    gkg::Volume< T > proxy;

    const std::vector< std::string >& 
      inputNames = concatenationProcess.getInputNames();
    int32_t volumeCount = ( int32_t )inputNames.size();
    int32_t v = 0;
    int32_t globalSizeZ = size.z;
    for ( v = 1; v < volumeCount; v++ )
    {

      // reading other volume proxy
      gkg::Reader::getInstance().readHeader( inputNames[ v ], proxy );

      // collecting size
      gkg::Vector3d< int32_t > otherSize;
      int32_t otherSizeT = 1;
      proxy.getSize( otherSize, otherSizeT );

      // collecting resolution
      gkg::Vector3d< double > otherResolution;
      double otherResolutionT = 1.0;
      volume.getResolution( otherResolution, otherResolutionT );

      if ( concatenationProcess.getVerbose() )
      {

        std::cout << "dimensions of volume '"
                  << inputNames[ v ] 
                  << "' : ( "
                  << otherSize.x << ", "
                  << otherSize.y << ", "
                  << otherSize.z << ", "
                  << otherSizeT << " ) ( "
                  << otherResolution.x << "mm, "
                  << otherResolution.y << "mm, "
                  << otherResolution.z << "mm, "
                  << otherResolutionT << " )"
                  << std::endl;

      }

      // sanity checks
      if ( ( size.x != otherSize.x ) ||
           ( size.y != otherSize.y ) ||
           ( sizeT != otherSizeT ) ||
           ( resolution != otherResolution ) ||
           ( otherResolutionT != resolutionT ) )
      {

        throw std::runtime_error( "incompatible sizes or resolutions" );

      }

      // updating global Z size
      globalSizeZ += otherSize.z;

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating output volume
    ////////////////////////////////////////////////////////////////////////////

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "allocating output volume : ( "
                << size.x << ", "
                << size.y << ", "
                << globalSizeZ << ", "
                << sizeT << " ) ( "
                << resolution.x << "mm, "
                << resolution.y << "mm, "
                << resolution.z << "mm, "
                << resolutionT << " )"
                << std::endl;

    }
    gkg::Volume< T > concatenation( size.x, size.y, globalSizeZ, sizeT );
    concatenation.setResolution( resolution, resolutionT );


    ////////////////////////////////////////////////////////////////////////////
    // concatenating
    ////////////////////////////////////////////////////////////////////////////

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "concatenating : " << std::flush;

    }

    int32_t offsetZ = 0;
    for ( v = 0; v < volumeCount; v++ )
    {

      if ( concatenationProcess.getVerbose() )
      {

        if ( v != 0 )
        {

          std::cout << gkg::Eraser( 14 );

        }
        std::cout << " [ " << std::setw( 3 ) << v + 1 
        	  << " / " << std::setw( 3 ) << volumeCount
        	  << " ]" << std::flush;


      }


      // reading other volume proxy
      gkg::Reader::getInstance().read( inputNames[ v ], volume );
      gkg::Vector3d< int32_t > site;
      int32_t t = 0;
      for ( t = 0; t < sizeT; t++ )
      {

        for ( site.z = 0; site.z < size.z; site.z ++ )
        {

          for ( site.y = 0; site.y < size.y; site.y ++ )
          {

            for ( site.x = 0; site.x < size.x; site.x ++ )
            {

              concatenation( site.x, site.y, offsetZ + site.z, t ) =
                volume( site, t );

            }

          }

        }

      }
      offsetZ += volume.getSizeZ();

    }
    if ( concatenationProcess.getVerbose() )
    {

      std::cout << std::endl;

    }
 

    ////////////////////////////////////////////////////////////////////////////
    // saving motion corrected volume
    ////////////////////////////////////////////////////////////////////////////
    
    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "writing concatenated '"
                << concatenationProcess.getOutputName()
                << "' volume : " << std::flush;

    }
    if ( !concatenationProcess.getFormat().empty() )
    {

      format = concatenationProcess.getFormat();

    }
    gkg::Writer::getInstance().write( concatenationProcess.getOutputName(),
                                      concatenation,
                                      concatenationProcess.getAscii(),
                                      format );
    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }
   
  }
  GKG_CATCH( "template < class T > "
             "void ZConcatenationProcess::catVolume( "
             "gkg::Process& process, "
             "const std::string& inputName, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class TConcatenationProcess
//

class TConcatenationProcess : public ConcatenationProcess
{

  public:

    TConcatenationProcess( const std::vector< std::string >& inputNames,
                           const std::string& outputName,
                           const std::string& format,
                           const bool& ascii,
                           const bool& verbose );

  private:

    template < class T >
    static void catVolume( gkg::Process& process,
                           const std::string& inputName,
                           const gkg::AnalyzedObject&,
                           const std::string& );

};


TConcatenationProcess::TConcatenationProcess(
                                   const std::vector< std::string >& inputNames,
                                   const std::string& outputName,
                                   const std::string& format,
                                   const bool& ascii,
                                   const bool& verbose )
                     : ConcatenationProcess( inputNames,
                                             outputName,
                                             format,
                                             ascii,
                                             verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &catVolume< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &catVolume< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &catVolume< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &catVolume< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &catVolume< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &catVolume< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &catVolume< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &catVolume< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &catVolume< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &catVolume< double > );

}


template < class T >
void 
TConcatenationProcess::catVolume( gkg::Process& process,
                                  const std::string& inputName,
                                  const gkg::AnalyzedObject&,
                                  const std::string& )
{

  try
  {

    ConcatenationProcess&
      concatenationProcess = static_cast< ConcatenationProcess& >( process );


    ////////////////////////////////////////////////////////////////////////////
    // reading input volume
    ////////////////////////////////////////////////////////////////////////////

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "reading input volume '"
                << inputName << "' : "
                << std::flush;

    }

    // reading volume
    gkg::Volume< T > volume;
    std::string format = gkg::Reader::getInstance().read( inputName, volume );

    // collecting size
    gkg::Vector3d< int32_t > size;
    int32_t sizeT = 1;
    volume.getSize( size, sizeT );

    // collecting resolution
    gkg::Vector3d< double > resolution;
    double resolutionT = 1.0;
    volume.getResolution( resolution, resolutionT );

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "dimensions of volume '"
                << inputName 
                << "' : ( "
                << size.x << ", "
                << size.y << ", "
                << size.z << ", "
                << sizeT << " ) ( "
                << resolution.x << "mm, "
                << resolution.y << "mm, "
                << resolution.z << "mm, "
                << resolutionT << " )"
                << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading other input volume proxies, making sanity checks and processing
    // output time dimension
    ////////////////////////////////////////////////////////////////////////////
    gkg::Volume< T > proxy;

    const std::vector< std::string >& 
      inputNames = concatenationProcess.getInputNames();
    int32_t volumeCount = ( int32_t )inputNames.size();
    int32_t v = 0;
    int32_t globalSizeT = sizeT;
    for ( v = 1; v < volumeCount; v++ )
    {

      // reading other volume proxy
      gkg::Reader::getInstance().readHeader( inputNames[ v ], proxy );

      // collecting size
      gkg::Vector3d< int32_t > otherSize;
      int32_t otherSizeT = 1;
      proxy.getSize( otherSize, otherSizeT );

      // collecting resolution
      gkg::Vector3d< double > otherResolution;
      double otherResolutionT = 1.0;
      volume.getResolution( otherResolution, otherResolutionT );

      if ( concatenationProcess.getVerbose() )
      {

        std::cout << "dimensions of volume '"
                  << inputNames[ v ] 
                  << "' : ( "
                  << otherSize.x << ", "
                  << otherSize.y << ", "
                  << otherSize.z << ", "
                  << otherSizeT << " ) ( "
                  << otherResolution.x << "mm, "
                  << otherResolution.y << "mm, "
                  << otherResolution.z << "mm, "
                  << otherResolutionT << " )"
                  << std::endl;

      }

      // sanity checks
      if ( ( size != otherSize ) ||
           ( resolution != otherResolution ) ||
           ( otherResolutionT != resolutionT ) )
      {

        throw std::runtime_error( "incompatible sizes or resolutions" );

      }

      // updating global T size
      globalSizeT += otherSizeT;

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating output volume
    ////////////////////////////////////////////////////////////////////////////

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "allocating output volume : ( "
                << size.x << ", "
                << size.y << ", "
                << size.z << ", "
                << globalSizeT << " ) ( "
                << resolution.x << "mm, "
                << resolution.y << "mm, "
                << resolution.z << "mm, "
                << resolutionT << " )"
                << std::endl;

    }
    gkg::Volume< T > concatenation( size, globalSizeT );
    concatenation.setResolution( resolution, resolutionT );


    ////////////////////////////////////////////////////////////////////////////
    // concatenating
    ////////////////////////////////////////////////////////////////////////////

    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "concatenating : " << std::flush;

    }

    int32_t globalT = 0;
    for ( v = 0; v < volumeCount; v++ )
    {

      if ( concatenationProcess.getVerbose() )
      {

        if ( v != 0 )
        {

          std::cout << gkg::Eraser( 14 );

        }
        std::cout << " [ " << std::setw( 3 ) << v + 1 
        	  << " / " << std::setw( 3 ) << volumeCount
        	  << " ]" << std::flush;


      }


      // reading other volume proxy
      gkg::Reader::getInstance().read( inputNames[ v ], volume );
      gkg::Vector3d< int32_t > site;
      int32_t t = 0;
      for ( t = 0; t < volume.getSizeT(); t++ )
      {

        for ( site.z = 0; site.z < size.z; site.z ++ )
        {

          for ( site.y = 0; site.y < size.y; site.y ++ )
          {

            for ( site.x = 0; site.x < size.x; site.x ++ )
            {

              concatenation( site, globalT ) = volume( site, t );

            }

          }

        }
        ++ globalT;

      }

    }
    if ( concatenationProcess.getVerbose() )
    {

      std::cout << std::endl;

    }
 

    ////////////////////////////////////////////////////////////////////////////
    // saving motion corrected volume
    ////////////////////////////////////////////////////////////////////////////
    
    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "writing concatenated '"
                << concatenationProcess.getOutputName()
                << "' volume : " << std::flush;

    }
    if ( !concatenationProcess.getFormat().empty() )
    {

      format = concatenationProcess.getFormat();

    }
    gkg::Writer::getInstance().write( concatenationProcess.getOutputName(),
                                      concatenation,
                                      concatenationProcess.getAscii(),
                                      format );
    if ( concatenationProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }
   
  }
  GKG_CATCH( "template < class T > "
             "void TConcatenationProcess::catVolume( "
             "gkg::Process& process, "
             "const std::string& inputName, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class CatCommand
//


gkg::CatCommand::CatCommand( int32_t argc,
                             char* argv[],
                             bool loadPlugin,
                             bool removeFirst )
               : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::CatCommand::CatCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::CatCommand::CatCommand( const std::vector< std::string >& inputNames,
                	     const std::string& outputName,
                	     const std::string& type,
                	     bool ascii,
                	     const std::string& format,
                	     bool verbose )
{

  try
  {

    execute( inputNames, outputName, type, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::CatCommand::CatCommand( "
             "const std::vector< std::string >& inputNames, "
             "const std::string& outputName, const std::string& type, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::CatCommand::CatCommand( const gkg::Dictionary& parameters )
               : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, 
                                         std::vector< std::string >,
                                         inputNames );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputName );
    DECLARE_STRING_PARAMETER( parameters, std::string, type );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( inputNames, outputName, type, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::CatCommand::CatCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::CatCommand::~CatCommand()
{
}


std::string gkg::CatCommand::getStaticName()
{

  try
  {

    return "Cat";

  }
  GKG_CATCH( "std::string gkg::CatCommand::getStaticName()" );

}


void gkg::CatCommand::parse()
{

  try
  {

    std::vector< std::string > inputNames;
    std::string outputName;
    std::string type;
    std::string format = "";
    bool ascii = false;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // command line options
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "Concatenates files along time or rank "
                                  "(assuming containers homogeneity)",
                                  _loadPlugin );
    application.addSeriesOption( "-i",
                                 "Input container name list",
                                 inputNames );
    application.addSingleOption( "-o",
                                 "Output concatenated file",
                                 outputName );
    application.addSingleOption( "-t",
                                 "Concatenation type :\n"
                                 "- x: along x axis,\n"
                                 "- y: along y axis,\n"
                                 "- z: along z axis,\n"
                                 "- t: along t axis",
                                 type );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=first input)",
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

    execute( inputNames, outputName, type, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::CatCommand::parse()" );

}


void gkg::CatCommand::execute( const std::vector< std::string >& inputNames,
                	       const std::string& outputName,
                	       const std::string& type,
                	       bool ascii,
                	       const std::string& format,
                	       bool verbose )
{

  try
  {

    if ( type == "x" )
    {

      XConcatenationProcess
        concatenationProcess( inputNames, outputName, format, ascii, verbose );
      concatenationProcess.execute( inputNames[ 0 ] );

    }
    else if ( type == "y" )
    {

      YConcatenationProcess
        concatenationProcess( inputNames, outputName, format, ascii, verbose );
      concatenationProcess.execute( inputNames[ 0 ] );

    }
    else if ( type == "z" )
    {

      ZConcatenationProcess
        concatenationProcess( inputNames, outputName, format, ascii, verbose );
      concatenationProcess.execute( inputNames[ 0 ] );

    }
    else if ( type == "t" )
    {

      TConcatenationProcess
        concatenationProcess( inputNames, outputName, format, ascii, verbose );
      concatenationProcess.execute( inputNames[ 0 ] );

    }
    else
    {

      throw std::runtime_error( "bad concatenation type" );

    }

  }
  GKG_CATCH( "void gkg::CatCommand::execute( "
             "const std::vector< std::string >& inputNames, "
             "const std::string& outputName, const std::string& type, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator( CatCommand,
                        DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( inputNames ) +
                        DECLARE_STRING_PARAMETER_HELP( outputName ) +
                        DECLARE_STRING_PARAMETER_HELP( type ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                        DECLARE_STRING_PARAMETER_HELP( format ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
