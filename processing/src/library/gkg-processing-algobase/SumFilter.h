#ifndef _gkg_processing_algobase_SumFilter_h_
#define _gkg_processing_algobase_SumFilter_h_


#include <gkg-processing-algobase/FilterFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <vector>


namespace gkg
{


//
// class SumFilter< IN,
//                  OUT >
//

template < class IN, class OUT >
class SumFilter : public FilterFunction< IN,
                                         OUT >
{

  public:

    void filter( const IN& in,
                 OUT& out ) const;

};


//
// class SumFilter< std::vector< IN >,
//                  OUT >
//

template < class IN, class OUT >
class SumFilter< std::vector< IN >,
                 OUT > : public FilterFunction< std::vector< IN >,
                                                OUT >
{

  public:

    void filter( const std::vector< IN >& in,
                 OUT& out ) const;

};


//
// class SumFilter< Vector,
//                  OUT >
//

template < class OUT >
class SumFilter< Vector,
                 OUT > : public FilterFunction< Vector,
                                                OUT >
{

  public:

    void filter( const Vector& in,
                 OUT& out ) const;

};


//
// class SumFilter< Volume< IN >,
//                  OUT >
//

template < class IN, class OUT >
class SumFilter< Volume< IN >,
                 OUT > : public FilterFunction< Volume< IN >,
                                                OUT >
{

  public:

    void filter( const Volume< IN >& in,
                 OUT& out ) const;

};


//
// class SumFilter< Matrix,
//                  OUT >
//

template < class OUT >
class SumFilter< Matrix,
                 OUT > : public FilterFunction< Matrix,
                                                OUT >
{

  public:

    void filter( const Matrix& in,
                 OUT& out ) const;

};


//
// class SumFilter< SparseMatrix,
//                  OUT >
//

template < class OUT >
class SumFilter< SparseMatrix,
                 OUT > : public FilterFunction< SparseMatrix,
                                                OUT >
{

  public:

    void filter( const SparseMatrix& in,
                 OUT& out ) const;

};


}


#endif
