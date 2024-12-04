#ifndef _gkg_processing_container_Roi_i_h_
#define _gkg_processing_container_Roi_i_h_


#include <gkg-processing-container/Roi.h>
#include <gkg-processing-container/RoiMeasure_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>


template < class T >
inline
gkg::Roi< T >::Roi( gkg::RCPointer< Volume< T > > labelVolume, 
                    const T& label,
                    int32_t stride )
              : gkg::HeaderedObject(),
                _labelVolume( labelVolume ),
                _label( label ),
                _stride( stride )
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "Roi" ) );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::Roi< T >::Roi( gkg::RCPointer< Volume< T > > labelVolume, "
             "const T& label, "
             "int32_t stride ) " );

}


template < class T >
inline
gkg::Roi< T >::Roi( const gkg::Roi< T >& other )
              : gkg::HeaderedObject(),
                _labelVolume( other._labelVolume ),
                _label( other._label),
                _measures( other._measures ),
                _stride( other._stride )
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "Roi" ) );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::Roi< T >::Roi( const gkg::Roi< T >& other )" );

}


template < class T >
inline
gkg::Roi< T >::~Roi()
{
}


template < class T >
inline
gkg::Roi< T >& gkg::Roi< T >::operator=( const Roi< T >& other )
{

  try
  {

    this->gkg::HeaderedObject::operator=( other );
    _labelVolume = other._labelVolume;
    _label = other._label;
    _measures = other._measures;
    _stride = other._stride;

    return *this;

  }
  GKG_CATCH( "template < class T >"
             "inline"
             "gkg::Roi< T >& gkg::Roi< T >::operator=( " 
	     "const Roi< T >& other )" );

}



template < class T >
inline
void gkg::Roi< T >::addMeasure( const std::string& name, 
                                gkg::RCPointer< RoiMeasure< T > > roiMeasure )
{

  try
  {

    if ( roiMeasure.getPointer() )
    {

      _measures[ name ] = roiMeasure;

    }
    else
    {

      throw std::runtime_error( "the measure pointee is null" );

    } 

  }
  GKG_CATCH( "void gkg::Roi< T >::addMeasure( "
             "const std::string& name,"
             "gkg::RCPointer< RoiMeasure< T > > roiMeasure )" );
  
}


template < class T >
inline
gkg::RCPointer< gkg::RoiMeasure< T > >
gkg::Roi< T >::getMeasure( const std::string& name ) const
{

  try
  {

    typename std::map< std::string,
                       gkg::RCPointer< RoiMeasure< T > > >::const_iterator
      m = _measures.find( name );

    if ( m == _measures.end() )
    {

      throw std::runtime_error( "measure name not found" );

    }
    return m->second;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::RCPointer< gkg::RoiMeasure< T > > "
             "gkg::Roi< T >::getMeasure( const std::string& name ) const" );

}


template < class T >
inline
std::list< gkg::RCPointer< gkg::RoiMeasure< T > > >
gkg::Roi< T >::getMeasures() const
{

  try
  {

    typename std::map< std::string,
                       gkg::RCPointer< gkg::RoiMeasure< T > > >::const_iterator 
      m = _measures.begin(),
      me = _measures.end();;
  
    std::list< gkg::RCPointer< RoiMeasure< T > > > measures;

    while( m != me )
    {

      measures.push_back( m->second );
      ++ m;

    } 

    return measures;

  }
  GKG_CATCH( "std::list< gkg::RCPointer< gkg::RoiMeasure< T > > >" 
             "gkg::Roi< T >::getMeasures() const" );

}


template < class T >
inline
gkg::RCPointer< gkg::Volume< T > > gkg::Roi< T >::getLabelVolume() const
{

  return _labelVolume;

}

template < class T >
inline
const T& gkg::Roi< T >::getLabel() const
{

  return _label;

}

template < class T >
inline
const int32_t& gkg::Roi< T >::getStride() const
{

  return _stride;

}


#endif
