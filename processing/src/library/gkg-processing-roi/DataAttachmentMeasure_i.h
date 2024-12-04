#ifndef _gkg_processing_roi_DataAttachmentMeasure_i_h_
#define _gkg_processing_roi_DataAttachmentMeasure_i_h_

#include <gkg-processing-roi/DataAttachmentMeasure.h>
#include <gkg-processing-container/RoiMeasure_i.h>
#include <gkg-processing-histogram/HistogramAnalysis.h>


template < class T, class S >
inline
gkg::DataAttachmentMeasure< T, S >::DataAttachmentMeasure( 
                                gkg::RCPointer< gkg::Roi< T > > roi,
                                gkg::RCPointer< gkg::Volume< S > > mriVolume,
                                gkg::RCPointer< gkg::PhiFunction > phiFunction )
                                   : gkg::RoiMeasure< T >( roi ),
                                     _mriVolume( mriVolume ),
                                     _phiFunction( phiFunction )
{

  try
  {

    initialize();

  }
  GKG_CATCH( "template < class T, class S > "
             "inline "
             "gkg::DataAttachmentMeasure< T, S >::DataAttachmentMeasure( "
             "gkg::RCPointer< gkg::Roi< T > > roi, "
             "gkg::RCPointer< gkg::Volume< S > > mriVolume, "
             "gkg::RCPointer< gkg::PhiFunction > phiFunction )" );

}


template < class T, class S >
inline
gkg::DataAttachmentMeasure< T, S >::~DataAttachmentMeasure()
{
}


template < class T, class S >
inline
void gkg::DataAttachmentMeasure< T, S >::initialize()
{

  try
  {

    int32_t stride = this->_roi->getStride();
    _strideFactor = stride * stride * stride;

    gkg::Vector3d< double > resolution;
    this->_roi->getLabelVolume()->getResolution( resolution );

    int32_t sizeX = this->_roi->getLabelVolume()->getSizeX();
    int32_t sizeY = this->_roi->getLabelVolume()->getSizeY();
    int32_t sizeZ = this->_roi->getLabelVolume()->getSizeZ();

    T label = this->_roi->getLabel();
    const gkg::Volume< T >& labelVolume = *this->_roi->getLabelVolume();
    const gkg::Volume< S >& mriVolume = *this->_mriVolume;

    int32_t x, y, z;
    this->_value = 0.0;
    for( z = 0; z < sizeZ; z += stride ) 
    {

      for( y = 0; y < sizeY; y += stride ) 
      {

        for( x = 0; x < sizeX; x += stride )
        {

          if ( labelVolume( x, y, z ) == label )
          {

            this->_value += _phiFunction->getValue(
                                            ( double )mriVolume( x, y, z ) );

          }

        }

      }
	
    }

    this->_value *= _strideFactor;
    this->_newValue = this->_value;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::DataAttachmentMeasure< T >::initialize()" );

}


template < class T, class S >
inline
void gkg::DataAttachmentMeasure< T, S >::processNewValue( 
                                           const gkg::Vector3d< int32_t >& site,
                                           bool addOrRemove )
{


  try
  {

    if ( addOrRemove )
    {

      // adding a point
      this->_newValue = this->_value + _strideFactor *
                        _phiFunction->getValue(
                                           ( double )( *_mriVolume )( site ) );

    }
    else
    {

      // removing a point
      this->_newValue = this->_value - _strideFactor *
                        _phiFunction->getValue(
                                           ( double )( *_mriVolume )( site ) );

    }

  }
  GKG_CATCH( "template < class T, class S > "
             "inline "
             "void gkg::DataAttachmentMeasure< T, S >::processNewValue( "
             "const gkg::Vector3d< int32_t >& site, "
             "bool addOrRemove )" );

}


#endif

