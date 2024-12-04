#ifndef _gkg_processing_roi_VolumeMeasure_i_h_
#define _gkg_processing_roi_VolumeMeasure_i_h_


#include <gkg-processing-roi/VolumeMeasure.h>
#include <gkg-processing-container/RoiMeasure_i.h>


template < class T >
inline
gkg::VolumeMeasure< T >::VolumeMeasure( gkg::RCPointer< gkg::Roi< T > > roi )
                        : gkg::RoiMeasure< T >( roi )
{

  try
  {

    initialize();

  }
  GKG_CATCH( "template < class T >"
             "inline"
             "gkg::VolumeMeasure< T >::VolumeMeasure("
             "gkg::RCPointer< gkg::Roi< T > > roi )" );

}


template < class T >
inline
gkg::VolumeMeasure< T >::~VolumeMeasure()
{
}


template < class T >
inline
void gkg::VolumeMeasure< T >::initialize()
{

  try
  {

    this->_value = 0.0;

    gkg::Vector3d< double > resolution;
    this->_roi->getLabelVolume()->getResolution( resolution );

    int32_t stride = this->_roi->getStride();
    _voxelVolume = resolution.x * resolution.y * resolution.z *
                   stride * stride * stride;

    int32_t sizeX = this->_roi->getLabelVolume()->getSizeX();
    int32_t sizeY = this->_roi->getLabelVolume()->getSizeY();
    int32_t sizeZ = this->_roi->getLabelVolume()->getSizeZ();

    T label = this->_roi->getLabel();
    const gkg::Volume< T >& labelVolume = *this->_roi->getLabelVolume();

    int32_t x, y, z;
    for( z = 0; z < sizeZ; z += stride ) 
    {

      for( y = 0; y < sizeY; y += stride ) 
      {

        for( x = 0; x < sizeX; x += stride ) 
        {

          if ( labelVolume( x, y, z ) == label )
          {

            this->_value += _voxelVolume;

          }

        }

      }

    }

    this->_newValue = this->_value;

  }
  GKG_CATCH( "template < class T >"
             "inline"
             "void gkg::VolumeMeasure< T >::initialize()" );

}


template < class T >
inline
void gkg::VolumeMeasure< T >::processNewValue( 
                                       const gkg::Vector3d< int32_t >& /*site*/,
                                       bool addOrRemove )
{

  try
  {

    if ( addOrRemove )
    {

      // adding a point
      this->_newValue = this->_value + _voxelVolume;

    }
    else
    {

      // removing a point
      this->_newValue = this->_value - _voxelVolume;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::VolumeMeasure< T >::processNewValue( "
             "const gkg::Vector3d< int32_t >& /*site*/, "
             "bool addOrRemove )" );

}


#endif
