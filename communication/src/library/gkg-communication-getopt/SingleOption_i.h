#ifndef _gkg_communication_getopt_SingleOption_i_h_
#define _gkg_communication_getopt_SingleOption_i_h_


#include <gkg-communication-getopt/SingleOption.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/UnexpectedOptionException.h>
#include <gkg-core-exception/MissingOptionException.h>


template < class T >
inline
gkg::SingleOption< T >::SingleOption( const std::string& name,
                                      const std::string& information,
                                      T& value,
                                      bool optional )
                       : gkg::Option( name, information ),
                         _value( value ),
                         _optional( optional ),
                         _filled( false )
{
}



template < class T >
inline
gkg::SingleOption< T >::~SingleOption()
{
}


template < class T >
inline
std::string gkg::SingleOption< T >::getInformation() const
{

  if ( _optional )
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
bool gkg::SingleOption< T >::recognize( const std::string& name ) const
{

  if ( belongToNames( name ) )
  {

    if ( _filled )
    {

      throw gkg::UnexpectedOptionException( name );

    }
    return true;

  }
  return false;

}


template < class T >
inline
bool gkg::SingleOption< T >::feed( const std::string& value )
{

  if ( _filled )
  {

    return false;

  }
  gkg::StringConverter::toScalar( value, _value );
  _filled = true;
  return true;

}


template < class T >
inline
void gkg::SingleOption< T >::check() const
{

  if ( !_optional && !_filled )
  {

    throw gkg::MissingOptionException( getName() );

  }

}


// specific for boolean option

namespace gkg
{


template <>
inline
bool SingleOption< bool >::recognize( const std::string& name ) const
{

  if ( belongToNames( name ) )
  {

    if ( _filled )
    {

      throw UnexpectedOptionException( name );

    }
    _value = true;
    return true;

  }
  return false;

}


template <>
inline
bool SingleOption< bool >::feed( const std::string& value )
{

  if ( _filled )
  {

    return false;

  }
  bool tmp;
  if ( StringConverter::toScalar( value, tmp ) )
  {

    _value = tmp;
    _filled = true;
    return true;


  }
  else
  {

    _filled = true;
    return false;

  }

}


}


#endif
