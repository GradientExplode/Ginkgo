#ifndef _gkg_processing_roi_ProbabilisticAtlasMeasure_i_h_
#define _gkg_processing_roi_ProbabilisticAtlasMeasure_i_h_

#include <gkg-processing-roi/ProbabilisticAtlasMeasure.h>
#include <gkg-processing-container/RoiMeasure_i.h>
#include <gkg-processing-roi/AtlasInformation_i.h>
#include <cmath>


#define ULTRA_SMALL_PROBABILITY      0.001 / ( double )_dataBaseSize
#define VERY_SMALL_PROBABILITY       0.1 / ( double )_dataBaseSize
#define BACKGROUND_LEVEL             0



template < class T, class A >
inline
gkg::ProbabilisticAtlasMeasure< T, A >::ProbabilisticAtlasMeasure( 
           gkg::RCPointer< gkg::Roi< T > > roi, 
           gkg::RCPointer< gkg::Volume< A > > atlas,
           const std::string& structureDictionaryName,
           const std::string& statisticalDictionaryName,
           int32_t dataBaseSize,
           gkg::RCPointer< gkg::Transform3d< float > > labelToAtlasTransform3d )
                                      : gkg::RoiMeasure< T >( roi ),
                                        _atlas( atlas ),
                                        _atlasInformation(
                                                    *atlas,
                                                    structureDictionaryName,
                                                    statisticalDictionaryName ),
                                        _atlasBoundingBox( *atlas ),
                                        _dataBaseSize( dataBaseSize ),
                                        _labelToAtlasTransform3d(
                                                       labelToAtlasTransform3d )
{

  try
  {

    // pre-processing constant values
    _stride = this->_roi->getStride();
    _strideFactor = _stride * _stride * _stride;


    _ultraSmallContribution = ( double )_strideFactor *
                              ( - std::log( ULTRA_SMALL_PROBABILITY ) );
    _verySmallContribution = ( double )_strideFactor *
                             ( - std::log( VERY_SMALL_PROBABILITY ) );
    _dataBaseSizePlusOne = _dataBaseSize + 1;
    _contributions.resize( _dataBaseSizePlusOne );

    _contributions[ 0 ] = 0.0; // never used
    int32_t p = 0;
    for ( p =1; p < _dataBaseSizePlusOne; p++ )
    {

      _contributions[ p ] = ( double )_strideFactor * 
                            ( - std::log( ( double )p /
                                          ( double )_dataBaseSize ) );

    }

    initialize(); 

  }
  GKG_CATCH( "template < class T, class A >"
             "inline"
             "gkg::ProbabilisticAtlasMeasure< T, A >::"
             "ProbabilisticAtlasMeasure(" 
             "gkg::RCPointer< gkg::Roi< T > > roi,"
             "gkg::RCPointer< gkg::Volume< A > > atlas,"
             "const std::string& structureDictionaryName, "
             "const std::string& statisticalDictionaryName, "
             "int32_t dataBaseSize, "
             "gkg::RCPointer< gkg::Transform3d< float > > "
             "labelToAtlasTranform3d )" );

}


template < class T, class A >
inline
gkg::ProbabilisticAtlasMeasure< T, A >::~ProbabilisticAtlasMeasure()
{
}


template < class T, class A >
inline
void gkg::ProbabilisticAtlasMeasure< T, A >::initialize()
{

  try
  {

    T label = this->_roi->getLabel();
    gkg::Volume< T >& labelVolume = *this->_roi->getLabelVolume();
    int32_t r = _atlasInformation.getRank( label );

    int32_t labelSizeX = labelVolume.getSizeX();
    int32_t labelSizeY = labelVolume.getSizeY();
    int32_t labelSizeZ = labelVolume.getSizeZ();


    // initializing probabilistic atlas measurement(s)
    gkg::Vector3d< int32_t > from;
    gkg::Vector3d< int32_t > to;  

    this->_value = 0.0;
    for( from.z = 0; from.z < labelSizeZ; from.z += _stride ) 
    {

      for( from.y = 0; from.y < labelSizeY; from.y += _stride ) 
      {

        for( from.x = 0; from.x < labelSizeX; from.x += _stride ) 
        {

          if ( labelVolume( from ) == label )
          {

            _labelToAtlasTransform3d->getFullIntegerDirect( from, to );
            if ( _atlasBoundingBox.contains( to ) )
            {


              A atlasValue = ( *_atlas )( to, r );

              if ( ( atlasValue > BACKGROUND_LEVEL ) &&
                   ( atlasValue != _dataBaseSizePlusOne ) )
              {

                this->_value += _contributions[ atlasValue ];

              }
              else if ( atlasValue == BACKGROUND_LEVEL )
              {

                // in case we are outside the dilated ROI
                this->_value += _ultraSmallContribution;

              }
              else if ( atlasValue == _dataBaseSizePlusOne )
              {

                // in case we are in the dilated part of the ROI
                this->_value += _verySmallContribution;

              }

            }

          }

        }

      }

    }
    this->_newValue = this->_value;

  }
  GKG_CATCH( "template < class T, class A > "
             "inline "
             "void gkg::ProbabilisticAtlasMeasure< T, A >::initialize()" );

}


template < class T, class A >
inline
void gkg::ProbabilisticAtlasMeasure< T, A >::processNewValue( 
                                           const gkg::Vector3d< int32_t >& site,
                                           bool addOrRemove )
{

  try
  {

    T currentLabel = this->_roi->getLabel();

    gkg::Vector3d< int32_t > to;
    _labelToAtlasTransform3d->getFullIntegerDirect( site, to );

    if ( _atlasBoundingBox.contains( to ) )
    {

      int32_t r = _atlasInformation.getRank( currentLabel );

      if( addOrRemove )
      {

        if ( ( ( *_atlas )( to, r ) > BACKGROUND_LEVEL ) &&
             ( ( *_atlas )( to, r ) != ( _dataBaseSize + 1 ) ) )
        {

          this->_newValue = this->_value - _strideFactor *
                                     std::log( ( double )( *_atlas )( to, r ) /
                                     ( double )_dataBaseSize );

        }
        else if ( ( *_atlas )( to, r ) == BACKGROUND_LEVEL )
        {

          // in case we are outside the dilated ROI
          this->_newValue = this->_value - _strideFactor *
                            std::log( ULTRA_SMALL_PROBABILITY );

        }
        else if ( ( *_atlas )( to, r ) == _dataBaseSize + 1 )
        {

          // in case we are in the dilated part of the ROI
          this->_newValue = this->_value - _strideFactor *
                            std::log( VERY_SMALL_PROBABILITY );

        }

      }
      else
      {

        if ( ( ( *_atlas )( to, r ) > BACKGROUND_LEVEL ) &&
             ( ( *_atlas )( to, r ) != ( _dataBaseSize + 1 ) ) )
        {

          this->_newValue = this->_value + _strideFactor *
                            std::log( ( double )( *_atlas )( to, r ) /
                                      ( double )_dataBaseSize );

        }
        else if ( ( *_atlas )( to, r ) == BACKGROUND_LEVEL )
        {

          // in case we are outside the dilated ROI
          this->_newValue = this->_value + _strideFactor *
                            std::log( ULTRA_SMALL_PROBABILITY );

        }
        else if ( ( *_atlas )( to, r ) == _dataBaseSize + 1 )
        {

          // in case we are in the dilated part of the ROI
          this->_newValue = this->_value + _strideFactor *
                            std::log( VERY_SMALL_PROBABILITY );

        }

      }

    }


  }
  GKG_CATCH( "template < class T, class A > "
             "inline "
             "void gkg::ProbabilisticAtlasMeasure< T, A >::processNewValue( "
             "const gkg::Vector3d< int32_t >& site, "
             "bool addOrRemove )" );

}


#undef ULTRA_SMALL_PROBABILITY
#undef VERY_SMALL_PROBABILITY
#undef BACKGROUND_LEVEL

#endif
