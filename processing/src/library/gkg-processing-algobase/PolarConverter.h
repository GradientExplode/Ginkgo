#ifndef _gkg_processing_algobase_PolarConverter_h_
#define _gkg_processing_algobase_PolarConverter_h_


#include <vector>
#include <complex>


namespace gkg
{


template < class T > class Volume;


//
// class PolarConverter< IN, OUT >
//

template < class IN, class OUT >
class PolarConverter
{

  public:

    void convert( const IN& in,
                  OUT& magnitude, OUT& phase ) const;


};


template < class IN, class OUT >
class PolarConverter< std::complex< IN >, OUT >
{

  public:

    void convert( const std::complex< IN >& in,
                  OUT& magnitude, OUT& phase ) const;


};


//
// class PolarConverter< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
class PolarConverter< std::vector< IN >, std::vector< OUT > >
{

  public:

    void convert( const std::vector< IN >& in,
                  std::vector< OUT >& magnitude,
                  std::vector< OUT >& phase ) const;

};


//
// class PolarConverter< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
class PolarConverter< Volume< IN >, Volume< OUT > >
{

  public:

    void convert( const Volume< IN >& in,
                  Volume< OUT >& magnitude, Volume< OUT >& phase ) const;

};


}


#endif
