#ifndef _gkg_processing_container_RoiMeasure_i_h_
#define _gkg_processing_container_RoiMeasure_i_h_


#include <gkg-processing-container/RoiMeasure.h>
#include <gkg-processing-container/Roi_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
inline
gkg::RoiMeasure< T >::RoiMeasure( gkg::RCPointer< gkg::Roi< T > > roi )
                     : _roi( roi ),
                       _value( 0.0 ),
                       _newValue( 0.0 )
{
}


template < class T >
inline
gkg::RoiMeasure< T >::RoiMeasure( const gkg::RoiMeasure< T >& other )
                     : _roi( other._roi ),
                       _value( other._value ),
                       _newValue( other._newValue )
{
}


template < class T >
inline
gkg::RoiMeasure< T >::~RoiMeasure()
{
}


template < class T >
inline
const double& gkg::RoiMeasure< T >::getValue() const 
{

  return _value;

}


template < class T >
inline
const double& gkg::RoiMeasure< T >::getNewValue() const
{

  return _newValue;

}

template < class T >
inline
gkg::RCPointer< gkg::Roi< T > > gkg::RoiMeasure< T >::getRoi() const
{

  return _roi;

}


template < class T >
inline
void gkg::RoiMeasure< T >::applyNewValue()
{

  try
  {

    _value = _newValue;

  }
  GKG_CATCH( "template < class T >"
             "inline"
             "void gkg::RoiMeasure< T >::applyNewValue()" );

}


template < class T >
inline
void gkg::RoiMeasure< T >::resetNewValue()
{

  try
  {

    _newValue = _value;

  }
  GKG_CATCH( "template < class T >"
             "inline"
             "void gkg::RoiMeasure< T >::resetNewValue()" );

}


#endif
