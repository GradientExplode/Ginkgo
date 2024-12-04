#ifndef _gkg_processing_algobase_Converter_h_
#define _gkg_processing_algobase_Converter_h_


#include <vector>


namespace gkg
{


template < class T > class Volume;


//
// class Converter< IN, OUT >
//

template < class IN, class OUT >
class Converter
{

  public:

    void convert( const IN& in, OUT& out ) const;


};


//
// class Converter< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
class Converter< std::vector< IN >, std::vector< OUT > >
{

  public:

    void convert( const std::vector< IN >& in, std::vector< OUT >& out ) const;

};


// we must take into account the case where type is the same
template < class T >
class Converter< std::vector< T >, std::vector< T > >
{

  public:

    void convert( const std::vector< T >& in, std::vector< T >& out ) const;

};


//
// class Converter< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
class Converter< Volume< IN >, Volume< OUT > >
{

  public:

    void convert( const Volume< IN >& in, Volume< OUT >& out ) const;

};


// we must take into account the case where type is the same
template < class T >
class Converter< Volume< T >, Volume< T > >
{

  public:

    void convert( const Volume< T >& in, Volume< T >& out ) const;

};


}


#endif
