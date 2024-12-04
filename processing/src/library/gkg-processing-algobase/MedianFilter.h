#ifndef _gkg_processing_algobase_MedianFilter_h_
#define _gkg_processing_algobase_MedianFilter_h_


#include <gkg-processing-algobase/FilterFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <vector>
#include <complex>


namespace gkg
{


//
// class MedianFilter< IN,
//                     OUT >
//

template < class IN, class OUT >
class MedianFilter : public FilterFunction< IN,
                                            OUT >
{

  public:

    void filter( const IN& in,
                 OUT& out ) const;

};


//
// class MedianFilter< std::vector< IN >,
//                     OUT >
//

template < class IN, class OUT >
class MedianFilter< std::vector< IN >,
                    OUT > : public FilterFunction< std::vector< IN >,
                                                   OUT >
{

  public:

    void filter( const std::vector< IN >& in,
                 OUT& out ) const;

};


//
// class MedianFilter< std::vector< std::complex< IN > >,
//                     std::complex< OUT > >
//

template < class IN, class OUT >
class MedianFilter< std::vector< std::complex< IN > >,
                    std::complex< OUT > > :
                       public FilterFunction< std::vector< std::complex< IN > >,
                                              std::complex< OUT > >
{

  public:

    void filter( const std::vector< std::complex< IN > >& in,
                 std::complex< OUT >& out ) const;

};


//
// class MedianFilter< Vector,
//                     OUT >
//

template < class OUT >
class MedianFilter< Vector,
                    OUT > : public FilterFunction< Vector,
                                                   OUT >
{

  public:

    void filter( const Vector& in,
                 OUT& out ) const;

};


//
// class MedianFilter< Volume< IN >,
//                     OUT >
//

template < class IN, class OUT >
class MedianFilter< Volume< IN >,
                    OUT > : public FilterFunction< Volume< IN >,
                                                   OUT >
{

  public:

    void filter( const Volume< IN >& in,
                 OUT& out ) const;

};


//
// class MedianFilter< Matrix,
//                     OUT >
//

template < class OUT >
class MedianFilter< Matrix,
                    OUT > : public FilterFunction< Matrix,
                                                   OUT >
{

  public:

    void filter( const Matrix& in,
                 OUT& out ) const;

};


//
// class MedianFilter< SparseMatrix,
//                     OUT >
//

template < class OUT >
class MedianFilter< SparseMatrix,
                    OUT > : public FilterFunction< SparseMatrix,
                                                   OUT >
{

  public:

    void filter( const SparseMatrix& in,
                 OUT& out ) const;

};


}


#endif
