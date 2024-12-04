#ifndef _gkg_processing_algobase_MinMaxIndexFilter_h_
#define _gkg_processing_algobase_MinMaxIndexFilter_h_


#include <gkg-processing-algobase/FilterFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-container/Volume.h>
#include <vector>


namespace gkg
{


template < class T > class Volume;


//
// class MinMaxIndexFilter< IN,
//                          OUT >
//

template < class IN, class OUT >
class MinMaxIndexFilter : public FilterFunction< IN,
                                                 std::pair< OUT, OUT > >
{

  public:

    void filter( const IN& in,
                 std::pair< OUT, OUT >& out ) const;

};


//
// class MinMaxIndexFilter< std::vector< IN >,
//                          OUT >
//

template < class IN, class OUT >
class MinMaxIndexFilter< std::vector< IN >,
                         OUT > : public FilterFunction< std::vector< IN >, 
                                                        std::pair< OUT,
                                                                   OUT > >
{

  public:

    void filter( const std::vector< IN >& in, 
                 std::pair< OUT,
                            OUT >& out ) const;

};


//
// class MinMaxIndexFilter< Vector,
//                          OUT >
//

template < class OUT >
class MinMaxIndexFilter< Vector,
                         OUT > : public FilterFunction< Vector, 
                                                        std::pair< OUT,
                                                                   OUT > >
{

  public:

    void filter( const Vector& in,
                 std::pair< OUT,
                            OUT >& out ) const;

};


//
// class MinMaxIndexFilter< Volume< IN >,
//                          OUT >
//

template < class IN, class OUT >
class MinMaxIndexFilter< Volume< IN >,
                         OUT > : public FilterFunction< Volume< IN >, 
                                                        std::pair< OUT,
                                                                   OUT > >
{

  public:

    void filter( const Volume< IN >& in,
                 std::pair< OUT,
                            OUT >& out ) const;

};


}


#endif
