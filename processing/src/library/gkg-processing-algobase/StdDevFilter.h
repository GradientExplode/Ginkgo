#ifndef _gkg_processing_algobase_StdDevFilter_h_
#define _gkg_processing_algobase_StdDevFilter_h_


#include <gkg-processing-algobase/FilterFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <vector>


namespace gkg
{


//
// class StdDevFilter< IN,
//                     OUT >
//

template < class IN, class OUT >
class StdDevFilter : public FilterFunction< IN,
                                            OUT >
{

  public:

    void filter( const IN& in,
                 OUT& out ) const;

};


//
// class StdDevFilter< std::vector< IN >,
//                     OUT >
//

template < class IN, class OUT >
class StdDevFilter< std::vector< IN >,
                    OUT > : public FilterFunction< std::vector< IN >,
                                                   OUT >
{

  public:

    void filter( const std::vector< IN >& in,
                 OUT& out ) const;

};


//
// class StdDevFilter< Vector,
//                     OUT >
//

template < class OUT >
class StdDevFilter< Vector,
                    OUT > : public FilterFunction< Vector,
                                                   OUT >
{

  public:

    void filter( const Vector& in,
                 OUT& out ) const;

};


//
// class StdDevFilter< Volume< IN >,
//                     OUT >
//

template < class IN, class OUT >
class StdDevFilter< Volume< IN >,
                    OUT > : public FilterFunction< Volume< IN >,
                                                   OUT >
{

  public:

    void filter( const Volume< IN >& in,
                 OUT& out ) const;

};


//
// class StdDevFilter< Matrix,
//                     OUT >
//

template < class OUT >
class StdDevFilter< Matrix,
                    OUT > : public FilterFunction< Matrix,
                                                   OUT >
{

  public:

    void filter( const Matrix& in,
                 OUT& out ) const;

};


//
// class StdDevFilter< SparseMatrix,
//                     OUT >
//

template < class OUT >
class StdDevFilter< SparseMatrix,
                    OUT > : public FilterFunction< SparseMatrix,
                                                   OUT >
{

  public:

    void filter( const SparseMatrix& in,
                 OUT& out ) const;

};


}


#endif
