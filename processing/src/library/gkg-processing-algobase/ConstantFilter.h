#ifndef _gkg_processing_algobase_ConstantFilter_h_
#define _gkg_processing_algobase_ConstantFilter_h_


#include <gkg-processing-algobase/FilterFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <vector>


namespace gkg
{


template < class T > class Volume;


//
// class ConstantFilter< IN,
//                       OUT >
//

template < class IN, class OUT >
class ConstantFilter : public FilterFunction< IN,
                                              OUT >
{

  public:

    ConstantFilter( const OUT& out );

    void filter( const IN& in,
                 OUT& out ) const;

  protected:

    OUT _out;

};


//
// class ConstantFilter< std::vector< IN >,
//                       OUT >
//

template < class IN, class OUT >
class ConstantFilter< std::vector< IN >,
                      OUT > : public FilterFunction< std::vector< IN >,
                                                     OUT >
{

  public:

    ConstantFilter( const OUT& out );

    void filter( const std::vector< IN >& in,
                 OUT& out ) const;

  protected:

    OUT _out;

};


//
// class ConstantFilter< Vector,
//                       OUT >
//

template < class OUT >
class ConstantFilter< Vector,
                      OUT > : public FilterFunction< Vector,
                                                     OUT >
{

  public:

    ConstantFilter( const OUT& out );

    void filter( const Vector& in,
                 OUT& out ) const;

  protected:

    OUT _out;

};


//
// class ConstantFilter< Volume< IN >,
//                       OUT >
//

template < class IN, class OUT >
class ConstantFilter< Volume< IN >,
                      OUT > : public FilterFunction< Volume< IN >,
                                                     OUT >
{

  public:

    ConstantFilter( const OUT& out );

    void filter( const Volume< IN >& in,
                 OUT& out ) const;

  protected:

    OUT _out;

};


//
// class ConstantFilter< Matrix,
//                       OUT >
//

template < class OUT >
class ConstantFilter< Matrix,
                      OUT > : public FilterFunction< Matrix,
                                                     OUT >
{

  public:

    ConstantFilter( const OUT& out );

    void filter( const Matrix& in,
                 OUT& out ) const;

  protected:

    OUT _out;

};


//
// class ConstantFilter< SparseMatrix,
//                       OUT >
//

template < class OUT >
class ConstantFilter< SparseMatrix,
                      OUT > : public FilterFunction< SparseMatrix,
                                                     OUT >
{

  public:

    ConstantFilter( const OUT& out );

    void filter( const SparseMatrix& in,
                 OUT& out ) const;

  protected:

    OUT _out;

};


}


#endif
