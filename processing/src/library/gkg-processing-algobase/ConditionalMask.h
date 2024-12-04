#ifndef _gkg_processing_algobase_ConditionalMask_h_
#define _gkg_processing_algobase_ConditionalMask_h_


#include <vector>


namespace gkg
{


template < class T > class Volume;
template < class T > class TestFunction;
template < class IN, class OUT > class FilterFunction;


//
// class ConditionalMask< IN, MASK >
//

template < class IN, class MASK >
class ConditionalMask
{

  public:

    void mask( IN& in,
               const MASK& mask,
               const TestFunction< MASK >& testFunction,
               const FilterFunction< IN, IN >& filterFunction ) const;


};


//
// class ConditionalMask< std::vector< IN >, MASK >
//

template < class IN, class MASK >
class ConditionalMask< std::vector< IN >, MASK >
{

  public:

    void mask( std::vector< IN >& in,
               const std::vector< MASK >& mask,
               const TestFunction< MASK >& testFunction,
               const FilterFunction< IN, IN >& filterFunction ) const;

};


//
// class ConditionalMask< Volume< IN >, MASK >
//

template < class IN, class MASK >
class ConditionalMask< Volume< IN >, MASK >
{

  public:

    void mask( Volume< IN >& in,
               const Volume< MASK >& mask,
               const TestFunction< MASK >& testFunction,
               const FilterFunction< IN, IN >& filterFunction ) const;

};


}


#endif
