#ifndef _gkg_processing_algobase_MinMaxFilterWithMask_h_
#define _gkg_processing_algobase_MinMaxFilterWithMask_h_


#include <gkg-processing-algobase/FilterWithMaskFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-container/Volume.h>
#include <vector>


namespace gkg
{


//
// class MinMaxFilterWithMask< IN,
//                             MASK,
//                             OUT >
//

template < class IN, class MASK, class OUT >
class MinMaxFilterWithMask : public FilterWithMaskFunction< IN,
                                                            MASK,
                                                            MASK,
                                                            std::pair< OUT,
                                                                       OUT > >
{

  public:

    void filter( const IN& in,
                 const MASK& mask,
                 const TestFunction< MASK >& testFunction,
                 std::pair< OUT,
                            OUT >& out ) const;

};


//
// class MinMaxFilterWithMask< std::vector< IN >,
//                             MASK,
//                             OUT >
//

template < class IN, class MASK, class OUT >
class MinMaxFilterWithMask< std::vector< IN >,
                            MASK,
                            OUT > : public FilterWithMaskFunction<
                                                            std::vector< IN >,
                                                            std::vector< MASK >,
                                                            MASK,
                                                            std::pair< OUT,
                                                                       OUT > >
{

  public:

    void filter( const std::vector< IN >& in,
                 const std::vector< MASK >& mask,
                 const TestFunction< MASK >& testFunction,
                 std::pair< OUT,
                            OUT >& out ) const;

};


//
// class MinMaxFilterWithMask< Vector,
//                             MASK,
//                             OUT >
//

template < class MASK, class OUT >
class MinMaxFilterWithMask< Vector,
                            MASK,
                            OUT > : public FilterWithMaskFunction< 
                                                            Vector,
                                                            std::vector< MASK >,
                                                            MASK,
                                                            std::pair< OUT,
                                                                       OUT > >
{

  public:

    void filter( const Vector& in,
                 const std::vector< MASK >& mask,
                 const TestFunction< MASK >& testFunction,
                 std::pair< OUT,
                            OUT >& out ) const;

};


//
// class MinMaxFilterWithMask< Volume< IN >,
//                             MASK,
//                             OUT >
//

template < class IN, class MASK, class OUT >
class MinMaxFilterWithMask< Volume< IN >,
                            MASK,
                            OUT > : public FilterWithMaskFunction<
                                                              Volume< IN >,
                                                              Volume< MASK >,
                                                              MASK,
                                                              std::pair< OUT,
                                                                         OUT > >
{

  public:

    void filter( const Volume< IN >& in,
                 const Volume< MASK >& mask,
                 const TestFunction< MASK >& testFunction,
                 std::pair< OUT,
                            OUT >& out ) const;

};


}


#endif
