#ifndef _gkg_communication_getopt_SeriesOption_i_h_
#define _gkg_communication_getopt_SeriesOption_i_h_


#include <gkg-communication-getopt/SeriesOption.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/UnexpectedOptionException.h>
#include <gkg-core-exception/MissingOptionException.h>


template < class T >
inline
gkg::SeriesOption< T >::SeriesOption( const std::string& name,
                                      const std::string& information,
                                      T& series,
                                      size_t minItemCount,
                                      size_t maxItemCount )
                       : gkg::Option( name, information ),
                         _series( series ),
                         _minItemCount( minItemCount ),
                         _maxItemCount( maxItemCount ),
                         _nextItem( _series.begin() ),
                         _storedItemCount( 0U )
{
}



template < class T >
inline
gkg::SeriesOption< T >::~SeriesOption()
{
}


template < class T >
inline
std::string gkg::SeriesOption< T >::getInformation() const
{

  if ( _minItemCount == 0 )
  {

    return std::string( "[" ) +
           getNames() +
           "<" + gkg::TypeOf< T >::getName() + ">]\n" +
           _information;

  }
  return getNames() +
         "<" + gkg::TypeOf< T >::getName() + ">\n" +
         _information;

}


template < class T >
inline
bool gkg::SeriesOption< T >::recognize( const std::string& name ) const
{

  if ( belongToNames( name ) )
  {

    if ( _maxItemCount && _storedItemCount >= _maxItemCount )
    {

      throw gkg::UnexpectedOptionException( name );

    }
    return true;

  }
  return false;

}


template < class T >
inline
bool gkg::SeriesOption< T >::feed( const std::string& value )
{

  if ( ( _maxItemCount == 0 ) ||
       ( _storedItemCount < _maxItemCount ) )
  {

    if ( _nextItem == _series.end() )
    {

      _series.push_back( typename T::value_type() );
      gkg::StringConverter::toScalar( value, _series.back() );
      _nextItem = _series.end();

    }
    else
    {

      gkg::StringConverter::toScalar( value, *_nextItem );
      _nextItem ++;

    }
    _storedItemCount ++;
    return true;

  }
  return false;

}


template < class T >
inline
void gkg::SeriesOption< T >::check() const
{

  if ( _series.size() < _minItemCount )
  {

    throw gkg::MissingOptionException( getName() );

  }

}


#endif
