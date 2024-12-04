#ifndef _gkg_processing_algobase_MagnitudeConverter_h_
#define _gkg_processing_algobase_MagnitudeConverter_h_


#include <complex>
#include <vector>


namespace gkg
{


template < class T > class Volume;


//
// class MagnitudeConverter< IN, OUT >
//

template < class IN, class OUT >
class MagnitudeConverter
{

  public:

    void convert( const IN& in, OUT& out ) const;


};


template < class IN, class OUT >
class MagnitudeConverter< std::complex< IN >, OUT >
{

  public:

    void convert( const std::complex< IN >& in, OUT& out ) const;


};


//
// class MagnitudeConverter< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
class MagnitudeConverter< std::vector< IN >, std::vector< OUT > >
{

  public:

    void convert( const std::vector< IN >& in, std::vector< OUT >& out ) const;

};


//
// class MagnitudeConverter< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
class MagnitudeConverter< Volume< IN >, Volume< OUT > >
{

  public:

    void convert( const Volume< IN >& in, Volume< OUT >& out ) const;

};


}


#endif
