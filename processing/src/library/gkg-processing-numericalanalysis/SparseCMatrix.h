#ifndef _gkg_processing_numericalanalysis_SparseCMatrix_h_
#define _gkg_processing_numericalanalysis_SparseCMatrix_h_


#include <gkg-core-cppext/StdInt.h>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <iostream>
#include <complex>


namespace gkg
{


class SparseMatrix;
class Matrix;
class Vector;
class CMatrix;
class CVector;


class SparseCMatrix
{

  public:

    typedef boost::numeric::ublas::mapped_matrix<
                                 std::complex< double > >::reference reference;
    typedef boost::numeric::ublas::mapped_matrix<
                                 std::complex< double > >::const_reference
                                                               const_reference;

    typedef boost::numeric::ublas::mapped_matrix<
                                 std::complex< double > >::iterator1 iterator1;
    typedef boost::numeric::ublas::mapped_matrix<
                                 std::complex< double > >::iterator2 iterator2;

    typedef boost::numeric::ublas::mapped_matrix<
                                 std::complex< double > >::const_iterator1
                                                               const_iterator1;
    typedef boost::numeric::ublas::mapped_matrix<
                                 std::complex< double > >::const_iterator2
                                                               const_iterator2;

    SparseCMatrix( int32_t size1 = 1, int32_t size2 = 1 );
    SparseCMatrix( const SparseCMatrix& other );
    SparseCMatrix( const SparseMatrix& other );
    SparseCMatrix( const CMatrix& other );
    SparseCMatrix( const Matrix& other );
    virtual ~SparseCMatrix();

    SparseCMatrix& operator=( const SparseCMatrix& other );
    SparseCMatrix& operator=( const SparseMatrix& other );
    SparseCMatrix& operator=( const CMatrix& other );
    SparseCMatrix& operator=( const Matrix& other );

    void reallocate( int32_t size1, int32_t size2 );

    iterator1 begin1();
    iterator1 end1();

    const_iterator1 begin1() const;
    const_iterator1 end1() const;

    int32_t getSize1() const;
    int32_t getSize2() const;
    int32_t getNonZeroElementCount() const;

    bool hasElement( int32_t i, int32_t j ) const;

    const_reference operator()( int32_t i, int32_t j ) const;
    reference operator()( int32_t i, int32_t j );

    void eraseElement( int32_t i, int32_t j );

    bool isSquare() const;

    void setZero();
    void setIdentity();
    void setRow( int32_t s1, const CVector& row );
    void setColumn( int32_t s2, const CVector& column );
    void fill( const std::complex< double >& value );
    void fill( int32_t offset1,
               int32_t offset2,
               int32_t size1,
               int32_t size2,
               const std::complex< double >& value );
    void fill( int32_t offset1,
               int32_t offset2,
               const SparseCMatrix& other,
               int32_t size1 = 0,
               int32_t size2 = 0 );
    void fill( int32_t offset1,
               int32_t offset2,
               const CMatrix& other,
               int32_t size1 = 0,
               int32_t size2 = 0 );
    void setDiagonal( const std::complex< double >& value );
    void transpose();
    void conjugate();

    std::complex< double > getTrace() const;
    SparseCMatrix getTransposition() const;
    SparseCMatrix getConjugate() const;
    SparseCMatrix getComposition( const SparseCMatrix& other ) const;
    SparseCMatrix getComposition( const CMatrix& other ) const;
    CVector getComposition( const CVector& other ) const;
    SparseCMatrix getComposition( const SparseMatrix& other ) const;
    SparseCMatrix getComposition( const Matrix& other ) const;
    CVector getComposition( const Vector& other ) const;
    CVector toCVector() const;
    SparseMatrix getMagnitude() const;
    CMatrix toCMatrix() const;

    CVector getRow( int32_t i ) const;
    CVector getColumn( int32_t j ) const;

    std::map< size_t, std::complex< double > >& getData();
    const std::map< size_t, std::complex< double > >& getData() const;

  protected:

    boost::numeric::ublas::mapped_matrix< std::complex< double > > _matrix;

};


}


//
// unary operators + & -
//

// + SparseCMatrix
gkg::SparseCMatrix 
operator + ( const gkg::SparseCMatrix& thing );

// - SparseCMatrix
gkg::SparseCMatrix 
operator - ( const gkg::SparseCMatrix& thing );


//
// binary operator +
//

// SparseCMatrix + SparseCMatrix
gkg::SparseCMatrix
operator + ( const gkg::SparseCMatrix& thing1,
             const gkg::SparseCMatrix& thing2 );

// SparseCMatrix + SparseMatrix
gkg::SparseCMatrix
operator + ( const gkg::SparseCMatrix& thing1,
             const gkg::SparseMatrix& thing2 );


//
// binary operator -
//

// SparseCMatrix - SparseCMatrix
gkg::SparseCMatrix
operator - ( const gkg::SparseCMatrix& thing1,
             const gkg::SparseCMatrix& thing2 );

// SparseCMatrix - SparseMatrix
gkg::SparseCMatrix
operator - ( const gkg::SparseCMatrix& thing1,
             const gkg::SparseMatrix& thing2 );


//
// binary operator *
//

// SparseCMatrix * SparseCMatrix
gkg::SparseCMatrix
operator * ( const gkg::SparseCMatrix& thing1,
             const gkg::SparseCMatrix& thing2 );

// SparseCMatrix * SparseMatrix
gkg::SparseCMatrix
operator * ( const gkg::SparseCMatrix& thing1,
             const gkg::SparseMatrix& thing2 );

// SparseCMatrix * CVector
gkg::CVector
operator * ( const gkg::SparseCMatrix& thing1,
             const gkg::CVector& thing2 );

// SparseCMatrix * Vector
gkg::CVector
operator * ( const gkg::SparseCMatrix& thing1,
             const gkg::Vector& thing2 );

// SparseCMatrix * complex< double >
gkg::SparseCMatrix
operator * ( const gkg::SparseCMatrix& thing1,
             const std::complex< double >& thing2 );

// SparseCMatrix * double
gkg::SparseCMatrix
operator * ( const gkg::SparseCMatrix& thing1,
             const double& thing2 );

//
// binary operator /
//

// SparseCMatrix / complex< double >
gkg::SparseCMatrix
operator / ( const gkg::SparseCMatrix& thing1,
             const std::complex< double >& thing2 );

// SparseCMatrix / double
gkg::SparseCMatrix
operator / ( const gkg::SparseCMatrix& thing1,
             const double& thing2 );


namespace std
{

std::ostream& operator << ( ostream& os, const gkg::SparseCMatrix& thing );
bool operator == ( const gkg::SparseCMatrix& thing1,
                   const gkg::SparseCMatrix& thing2 );

}


#endif
