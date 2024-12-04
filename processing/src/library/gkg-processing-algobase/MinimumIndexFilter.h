#ifndef _gkg_processing_algobase_MinimumIndexFilter_h_
#define _gkg_processing_algobase_MinimumIndexFilter_h_


#include <gkg-processing-algobase/FilterFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-container/Volume.h>
#include <vector>


namespace gkg
{


template < class T > class Volume;


//
// class MinimumIndexFilter< IN,
//                           OUT >
//

template < class IN, class OUT >
class MinimumIndexFilter : public FilterFunction< IN,
                                                  OUT >
{

  public:

    void filter( const IN& in,
                 OUT& out ) const;


};


//
// class MinimumIndexFilter< std::vector< IN >,
//                           OUT >
//

template < class IN, class OUT >
class MinimumIndexFilter< std::vector< IN >,
                          OUT > : public FilterFunction< std::vector< IN >,
                                                         OUT >
{

  public:

    void filter( const std::vector< IN >& in,
                 OUT& out ) const;

};


//
// class MinimumIndexFilter< Vector,
//                           OUT >
//

template < class OUT >
class MinimumIndexFilter< Vector,
                          OUT > : public FilterFunction< Vector,
                                                         OUT >
{

  public:

    void filter( const Vector& in,
                 OUT& out ) const;

};


//
// class MinimumIndexFilter< Volume< IN >,
//                           OUT >
//

template < class IN, class OUT >
class MinimumIndexFilter< Volume< IN >,
                          OUT > : public FilterFunction< Volume< IN >,
                                                         OUT >
{

  public:

    void filter( const Volume< IN >& in, OUT& out ) const;

};


}


#endif
