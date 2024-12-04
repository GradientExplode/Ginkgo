#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-processing-algobase/RescalerWithSlopeAndIntercept_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>


template < class O >
gkg::TypedVolumeReaderProcess< O >::TypedVolumeReaderProcess(
                                                   gkg::Volume< O >& theVolume )
                                   : gkg::Process( "Volume" ),
            	                     volume( theVolume )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &TypedVolumeReaderProcess< O >::read< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &TypedVolumeReaderProcess< O >::read< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &TypedVolumeReaderProcess< O >::read< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &TypedVolumeReaderProcess< O >::read< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &TypedVolumeReaderProcess< O >::read< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &TypedVolumeReaderProcess< O >::read< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &TypedVolumeReaderProcess< O >::read< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &TypedVolumeReaderProcess< O >::read< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &TypedVolumeReaderProcess< O >::read< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &TypedVolumeReaderProcess< O >::read< double > );

  }
  GKG_CATCH( "template < class O > "
             "gkg::TypedVolumeReaderProcess< O >::TypedVolumeReaderProcess( "
             "gkg::Volume< O >& theVolume )" );

}


template < class O > template < class T >
void gkg::TypedVolumeReaderProcess< O >::read( gkg::Process& process,
                                               const std::string& fileNameIn,
                                               const gkg::AnalyzedObject&,
                                               const std::string& )
{

  try
  {

    gkg::TypedVolumeReaderProcess< O >&
      volumeReaderProcess = static_cast< gkg::TypedVolumeReaderProcess< O >& >(
                                                                      process );

    gkg::Volume< T > inputVolume;
    gkg::Reader::getInstance().read( fileNameIn, inputVolume );

    double slope = 1.0;
    double intercept = 0.0;

    if ( inputVolume.getHeader().hasAttribute( "rescale_slope" ) )
    {

      gkg::GenericObjectList genericObjectList;
      inputVolume.getHeader().getAttribute( "rescale_slope",
                                            genericObjectList );
      slope = genericObjectList[ 0 ]->getScalar();

    }
    if ( inputVolume.getHeader().hasAttribute( "rescale_intercept" ) )
    {

      gkg::GenericObjectList genericObjectList;
      inputVolume.getHeader().getAttribute( "rescale_intercept",
                                            genericObjectList );
      intercept = genericObjectList[ 0 ]->getScalar();

    }

    gkg::RescalerWithSlopeAndIntercept< gkg::Volume< T >, gkg::Volume< O > >
      rescalerWithSlopeAndIntercept( slope, intercept );

    rescalerWithSlopeAndIntercept.rescale( inputVolume, 
                                           volumeReaderProcess.volume );


    // then resetting slope and intercept to 1.0/0.0
    std::vector< double > rescaleSlopeVector( 1 );
    rescaleSlopeVector[ 0 ] = 1.0;
    volumeReaderProcess.volume.getHeader().addAttribute( "rescale_slope",
                                                         rescaleSlopeVector );

    std::vector< double > rescaleInterceptVector( 1 );
    rescaleInterceptVector[ 0 ] = 0.0;

    volumeReaderProcess.volume.getHeader().addAttribute( "rescale_intercept",
                                                         rescaleInterceptVector );


  }
  GKG_CATCH( "template < class O > template < class T > "
             "void gkg::TypedVolumeReaderProcess< O >::read( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}



template class gkg::TypedVolumeReaderProcess< uint8_t >;
template class gkg::TypedVolumeReaderProcess< int8_t >;
template class gkg::TypedVolumeReaderProcess< uint16_t >;
template class gkg::TypedVolumeReaderProcess< int16_t >;
template class gkg::TypedVolumeReaderProcess< uint32_t >;
template class gkg::TypedVolumeReaderProcess< int32_t >;
template class gkg::TypedVolumeReaderProcess< uint64_t >;
template class gkg::TypedVolumeReaderProcess< int64_t >;
template class gkg::TypedVolumeReaderProcess< float >;
template class gkg::TypedVolumeReaderProcess< double >;


