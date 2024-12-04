#ifndef _gkg_processing_algobase_PhaseConverter_h_
#define _gkg_processing_algobase_PhaseConverter_h_


#include <complex>
#include <vector>


namespace gkg
{


template < class T > class Volume;


//
// class PhaseConverter< IN, OUT >
//

template < class IN, class OUT >
class PhaseConverter
{

  public:

    void convert( const IN& in, OUT& out, bool degree = false ) const;


};


template < class IN, class OUT >
class PhaseConverter< std::complex< IN >, OUT >
{

  public:

    void convert( const std::complex< IN >& in, OUT& out,
                  bool degree = false ) const;


};


//
// class PhaseConverter< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
class PhaseConverter< std::vector< IN >, std::vector< OUT > >
{

  public:

    void convert( const std::vector< IN >& in, std::vector< OUT >& out,
                  bool degree = false ) const;

};


//
// class PhaseConverter< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
class PhaseConverter< Volume< IN >, Volume< OUT > >
{

  public:

    void convert( const Volume< IN >& in, Volume< OUT >& out,
                  bool degree = false ) const;

};


}


#endif
