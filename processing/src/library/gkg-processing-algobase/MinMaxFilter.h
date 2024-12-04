#ifndef _gkg_processing_algobase_MinMaxFilter_h_
#define _gkg_processing_algobase_MinMaxFilter_h_


#include <gkg-processing-algobase/FilterFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <vector>


namespace gkg
{


//
// class MinMaxFilter< IN,
//                     OUT >
//

template < class IN, class OUT >
class MinMaxFilter : public FilterFunction< IN,
                                            std::pair< OUT,
                                                       OUT > >
{

  public:

    void filter( const IN& in,
                 std::pair< OUT,
                            OUT >& out ) const;

};


//
// class MinMaxFilter< std::vector< IN >,
//                     OUT >
//

template < class IN, class OUT >
class MinMaxFilter< std::vector< IN >,
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
// class MinMaxFilter< Vector,
//                     OUT >
//

template < class OUT >
class MinMaxFilter< Vector,
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
// class MinMaxFilter< Volume< IN >,
//                     OUT >
//

template < class IN, class OUT >
class MinMaxFilter< Volume< IN >,
                    OUT > : public FilterFunction< Volume< IN >,
                                                   std::pair< OUT,
                                                              OUT > >
{

  public:

    void filter( const Volume< IN >& in,
                 std::pair< OUT,
                            OUT >& out ) const;

};


//
// class MinMaxFilter< Matrix,
//                      OUT >
//

template < class OUT >
class MinMaxFilter< Matrix,
                    OUT > : public FilterFunction< Matrix,
                                                   std::pair< OUT,
                                                              OUT > >
{

  public:

    void filter( const Matrix& in,
                 std::pair< OUT,
                            OUT >& out ) const;

};


//
// class MinMaxFilter< SparseMatrix,
//                      OUT >
//

template < class OUT >
class MinMaxFilter< SparseMatrix,
                    OUT > : public FilterFunction< SparseMatrix,
                                                   std::pair< OUT,
                                                              OUT > >
{

  public:

    void filter( const SparseMatrix& in,
                 std::pair< OUT,
                            OUT >& out ) const;

};


}


#endif
