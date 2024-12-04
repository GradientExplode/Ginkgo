#ifndef _gkg_processing_algobase_MaximumFilter_h_
#define _gkg_processing_algobase_MaximumFilter_h_


#include <gkg-processing-algobase/FilterFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <vector>


namespace gkg
{


//
// class MaximumFilter< IN,
//                      OUT >
//

template < class IN, class OUT >
class MaximumFilter : public FilterFunction< IN,
                                             OUT >
{

  public:

    void filter( const IN& in,
                 OUT& out ) const;

};


//
// class MaximumFilter< std::vector< IN >,
//                      OUT >
//

template < class IN, class OUT >
class MaximumFilter< std::vector< IN >,
                     OUT > : public FilterFunction< std::vector< IN >,
                                                    OUT >
{

  public:

    void filter( const std::vector< IN >& in,
                 OUT& out ) const;

};


//
// class MaximumFilter< Vector,
//                      OUT >
//

template < class OUT >
class MaximumFilter< Vector,
                     OUT > : public FilterFunction< Vector,
                                                    OUT >
{

  public:

    void filter( const Vector& in,
                 OUT& out ) const;

};


//
// class MaximumFilter< Volume< IN >,
//                      OUT >
//

template < class IN, class OUT >
class MaximumFilter< Volume< IN >,
                     OUT > : public FilterFunction< Volume< IN >,
                                                    OUT >
{

  public:

    void filter( const Volume< IN >& in,
                 OUT& out ) const;

};


//
// class MaximumFilter< Matrix,
//                      OUT >
//

template < class OUT >
class MaximumFilter< Matrix,
                     OUT > : public FilterFunction< Matrix,
                                                    OUT >
{

  public:

    void filter( const Matrix& in,
                 OUT& out ) const;

};


//
// class MaximumFilter< SparseMatrix,
//                      OUT >
//

template < class OUT >
class MaximumFilter< SparseMatrix,
                     OUT > : public FilterFunction< SparseMatrix,
                                                    OUT >
{

  public:

    void filter( const SparseMatrix& in,
                 OUT& out ) const;

};


}


#endif
