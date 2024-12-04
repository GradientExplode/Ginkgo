#ifndef _gkg_processing_algobase_ImagPartConverter_h_
#define _gkg_processing_algobase_ImagPartConverter_h_


#include <complex>
#include <vector>


namespace gkg
{


template < class T > class Volume;


//
// class ImagPartConverter< IN, OUT >
//

template < class IN, class OUT >
class ImagPartConverter
{

  public:

    void convert( const IN& in, OUT& out ) const;


};


template < class IN, class OUT >
class ImagPartConverter< std::complex< IN >, OUT >
{

  public:

    void convert( const std::complex< IN >& in, OUT& out ) const;


};


//
// class ImagPartConverter< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
class ImagPartConverter< std::vector< IN >, std::vector< OUT > >
{

  public:

    void convert( const std::vector< IN >& in, std::vector< OUT >& out ) const;

};


//
// class ImagPartConverter< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
class ImagPartConverter< Volume< IN >, Volume< OUT > >
{

  public:

    void convert( const Volume< IN >& in, Volume< OUT >& out ) const;

};


}


#endif
