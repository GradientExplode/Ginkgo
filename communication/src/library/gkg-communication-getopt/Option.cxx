#include <gkg-communication-getopt/Option.h>
#include <algorithm>


gkg::Option::Option( const std::string& name, const std::string& information )
{

  addName( name );
  _information = information;

}


gkg::Option::~Option()
{
}


const std::string& gkg::Option::getName() const
{

  return _names[ 0 ];

}


std::string gkg::Option::getNames() const
{

  std::string names = _names[ 0 ];
  std::deque< std::string >::const_iterator n = _names.begin() + 1,
                                            ne = _names.end();
  while ( n != ne )
  {

    names += " | " + *n;
    n ++;

  }
  return names;

}


bool gkg::Option::belongToNames( const std::string& name ) const
{

  return std::find( _names.begin(), _names.end(), name ) != _names.end();

}


void gkg::Option::addName( const std::string& name )
{

  _names.push_back( name );

}

