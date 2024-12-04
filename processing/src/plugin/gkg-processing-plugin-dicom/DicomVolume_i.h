#ifndef _gkg_processing_plugin_dicom_DicomVolume_i_h_
#define _gkg_processing_plugin_dicom_DicomVolume_i_h_


#include <gkg-processing-plugin-dicom/DicomVolume.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>


template < class T >
inline
gkg::DicomVolume< T >::DicomVolume( gkg::Volume< T >& volume, 
                                    pdcm::DataInfo* info )
                     : pdcm::DicomProxy(),
                       _volume( volume )
{

  if ( info )
  {

    _info = *info;

  }

}


template < class T >
inline
bool gkg::DicomVolume< T >::allocate( pdcm::DataInfo* info )
{

  if ( info )
  {

    _info = *info;

  }

  _info.initialize();
  _volume.getBinaryHeader().clear();
  _volume.getBinaryHeader().resize( _info._fileCount );
  _volume.getBinaryHeader().setFormat( "dicom" );

  pdcm::BoundingBox< int32_t > bBox;

  if ( _info._noFlip )
  {

    bBox = _info._boundingBox;

  }
  else
  {

    bBox = _info._patientOrientation.getDirectBoundingBox( _info._boundingBox );

  }

  gkg::BoundingBox< int32_t > boundingBox( bBox.getLowerX(), bBox.getUpperX(),
                                           bBox.getLowerY(), bBox.getUpperY(),
                                           bBox.getLowerZ(), bBox.getUpperZ(),
                                           bBox.getLowerT(), bBox.getUpperT() );

  int32_t subSizeX = boundingBox.getUpperX() - boundingBox.getLowerX() + 1;
  int32_t subSizeY = boundingBox.getUpperY() - boundingBox.getLowerY() + 1;
  int32_t subSizeZ = boundingBox.getUpperZ() - boundingBox.getLowerZ() + 1;
  int32_t subSizeT = boundingBox.getUpperT() - boundingBox.getLowerT() + 1;

  gkg::Vector3d< double > resolution( _info._resolution.x,
                                      _info._resolution.y,
                                      _info._resolution.z );
  _volume.reallocate( subSizeX, subSizeY, subSizeZ, subSizeT );
  _volume.setResolution( resolution, _info._repetitionTime );

  return true;

}


template < class T >
inline
void gkg::DicomVolume< T >::destroy()
{

  _info.clear();

}


template < class T >
inline
bool gkg::DicomVolume< T >::isNull() const
{

  if ( _volume.getSizeXYZT() <= 1 )
  {

    return true;

  }

  return false;

}


template < class T >
inline
bool gkg::DicomVolume< T >::isMemoryMapped() const
{

  return _volume.isMemoryMapped();

}


template < class T >
inline
pdcm::BinaryHeader& gkg::DicomVolume< T >::getBinaryHeader()
{

  return reinterpret_cast< pdcm::BinaryHeader& >( _volume.getBinaryHeader() );

}


template < class T >
inline
uint8_t* gkg::DicomVolume< T >::operator()( int32_t x, 
                                            int32_t y, 
                                            int32_t z, 
                                            int32_t t )
{

  return (uint8_t*)&_volume( x, y, z, t );

}


#endif

