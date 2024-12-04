#include <gkg-graphics-QtGL-plugin-functors/Voi/ReadProcess.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-processing-algobase/Converter_i.h>


gkg::ReadProcess::ReadProcess( const std::string& outputType, 
                               gkg::Volume< int16_t >* volInt16,
                               gkg::Volume< float >* volFloat,
                               gkg::Volume< gkg::RGBComponent >* volRGB )
                : gkg::Process( "Volume" ),
                  _outputType( outputType ),
                  _volumeFloat( volFloat ),
                  _volumeRGB( volRGB ),
                  _volumeInt16( volInt16 ),
                  _isRGB( false )
{

  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &gkg::ReadProcess::read< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &gkg::ReadProcess::read< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &gkg::ReadProcess::read< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &gkg::ReadProcess::read< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &gkg::ReadProcess::read< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &gkg::ReadProcess::read< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &gkg::ReadProcess::read< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &gkg::ReadProcess::read< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &gkg::ReadProcess::read< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &gkg::ReadProcess::read< double > );
  registerProcess( "Volume", gkg::TypeOf< gkg::RGBComponent >::getName(),
                   &gkg::ReadProcess::readRGB );

}


const std::string& gkg::ReadProcess::getOutputType() const
{

  return _outputType;

}


gkg::Volume< int16_t >* gkg::ReadProcess::getOutputVolumeInt16()
{

  return _volumeInt16;

}


gkg::Volume< float >* gkg::ReadProcess::getOutputVolumeFloat()
{

  return _volumeFloat;

}


gkg::Volume< gkg::RGBComponent >* gkg::ReadProcess::getOutputVolumeRGB()
{

  return _volumeRGB;

}


bool gkg::ReadProcess::isRgb()
{

  return _isRGB;

}


void gkg::ReadProcess::setRgb( bool status )
{

  _isRGB = status;

}


template < class T >
void gkg::ReadProcess::read( gkg::Process& process, 
                             const std::string& fileName,
			     const gkg::AnalyzedObject&,
                             const std::string& )
{
  
  gkg::ReadProcess& readProcess = static_cast< gkg::ReadProcess& >( process );

  gkg::Volume< T > volume;
  gkg::Reader::getInstance().template read< gkg::Volume< T > >( fileName, 
                                                                volume );
 
  readProcess.setRgb( false );
    
  if ( readProcess.getOutputType() == gkg::TypeOf< float >::getName() )
  {
   
    gkg::Converter< gkg::Volume< T >, gkg::Volume< float > > converter;
    converter.convert( volume, *readProcess.getOutputVolumeFloat() );
    
  }
  else if ( readProcess.getOutputType() == gkg::TypeOf< int16_t >::getName() )
  {
   
    gkg::Converter< gkg::Volume< T >, gkg::Volume< int16_t > > converter;
    converter.convert( volume, *readProcess.getOutputVolumeInt16() );
    
  }

}


void gkg::ReadProcess::readRGB( gkg::Process& process, 
                                const std::string& fileName,
                                const gkg::AnalyzedObject&,
                                const std::string& )
{

  gkg::ReadProcess& readProcess = static_cast< gkg::ReadProcess& >( process );
  readProcess.setRgb( true );

  gkg::Reader::getInstance().read( fileName, 
                                   *readProcess.getOutputVolumeRGB() );

}
