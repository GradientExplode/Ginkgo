#ifndef _gkg_processing_algobase_MaximumIndexFilter_h_
#define _gkg_processing_algobase_MaximumIndexFilter_h_


#include <gkg-processing-algobase/FilterFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-container/Volume.h>
#include <vector>


namespace gkg
{


template < class T > class Volume;


//
// class MaximumIndexFilter< IN,
//                           OUT >
//

template < class IN, class OUT >
class MaximumIndexFilter : public FilterFunction< IN,
                                                  OUT >
{

  public:

    void filter( const IN& in,
                 OUT& out ) const;


};


//
// class MaximumIndexFilter< std::vector< IN >,
//                           OUT >
//

template < class IN, class OUT >
class MaximumIndexFilter< std::vector< IN >,
                          OUT > : public FilterFunction< std::vector< IN >,
                                                         OUT >
{

  public:

    void filter( const std::vector< IN >& in,
                 OUT& out ) const;

};


//
// class MaximumIndexFilter< Vector,
//                           OUT >
//

template < class OUT >
class MaximumIndexFilter< Vector,
                          OUT > : public FilterFunction< Vector,
                                                         OUT >
{

  public:

    void filter( const Vector& in,
                 OUT& out ) const;

};


//
// class MaximumIndexFilter< Volume< IN >,
//                           OUT >
//

template < class IN, class OUT >
class MaximumIndexFilter< Volume< IN >,
                          OUT > : public FilterFunction< Volume< IN >,
                                                         OUT >
{

  public:

    void filter( const Volume< IN >& in, OUT& out ) const;

};


}


#endif
