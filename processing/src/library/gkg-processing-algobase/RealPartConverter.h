#ifndef _gkg_processing_algobase_RealPartConverter_h_
#define _gkg_processing_algobase_RealPartConverter_h_


#include <complex>
#include <vector>


namespace gkg
{


template < class T > class Volume;


//
// class RealPartConverter< IN, OUT >
//

template < class IN, class OUT >
class RealPartConverter
{

  public:

    void convert( const IN& in, OUT& out ) const;


};


template < class IN, class OUT >
class RealPartConverter< std::complex< IN >, OUT >
{

  public:

    void convert( const std::complex< IN >& in, OUT& out ) const;


};


//
// class RealPartConverter< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
class RealPartConverter< std::vector< IN >, std::vector< OUT > >
{

  public:

    void convert( const std::vector< IN >& in, std::vector< OUT >& out ) const;

};


//
// class RealPartConverter< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
class RealPartConverter< Volume< IN >, Volume< OUT > >
{

  public:

    void convert( const Volume< IN >& in, Volume< OUT >& out ) const;

};


}


#endif
