#ifndef _gkg_processing_numericalanalysis_SparseMatrix_h_
#define _gkg_processing_numericalanalysis_SparseMatrix_h_


#include <gkg-core-cppext/StdInt.h>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <iostream>
#include <complex>


namespace gkg
{


class SparseCMatrix;
class Matrix;
class Vector;
class CMatrix;
class CVector;


class SparseMatrix
{

  public:

    typedef boost::numeric::ublas::mapped_matrix< double >::reference
                                                               reference;
    typedef boost::numeric::ublas::mapped_matrix< double >::const_reference
                                                               const_reference;

    typedef boost::numeric::ublas::mapped_matrix< double >::iterator1 iterator1;
    typedef boost::numeric::ublas::mapped_matrix< double >::iterator2 iterator2;

    typedef boost::numeric::ublas::mapped_matrix< double >::const_iterator1
                                                                const_iterator1;
    typedef boost::numeric::ublas::mapped_matrix< double >::const_iterator2
                                                                const_iterator2;

    SparseMatrix( int32_t size1 = 1, int32_t size2 = 1 );
    SparseMatrix( const SparseMatrix& other );
    SparseMatrix( const Matrix& other );
    virtual ~SparseMatrix();

    SparseMatrix& operator=( const SparseMatrix& other );
    SparseMatrix& operator=( const Matrix& other );

    void reallocate( int32_t size1, int32_t size2 );

    iterator1 begin1();
    iterator1 end1();

    const_iterator1 begin1() const;
    const_iterator1 end1() const;

    int32_t getSize1() const;
    int32_t getSize2() const;
    int32_t getNonZeroElementCount() const;
    int32_t getNonZeroElementCountForLine( int32_t i ) const;
    int32_t getNonZeroElementCountForColumn( int32_t j ) const;

    bool hasElement( int32_t i, int32_t j ) const;

    const_reference operator()( int32_t i, int32_t j ) const; 
    reference operator()( int32_t i, int32_t j ); 

    void eraseElement( int32_t i, int32_t j );

    bool isSquare() const;

    void setZero();
    void setIdentity();
    void setRow( int32_t s1, const Vector& row );
    void setColumn( int32_t s2, const Vector& column );
    void fill( const double& value );
    void fill( int32_t offset1,
               int32_t offset2,
               int32_t size1,
               int32_t size2,
               const double& value );
    void fill( int32_t offset1,
               int32_t offset2,
               const SparseMatrix& other,
               int32_t size1 = 0,
               int32_t size2 = 0 );
    void fill( int32_t offset1,
               int32_t offset2,
               const Matrix& other,
               int32_t size1 = 0,
               int32_t size2 = 0 );
    void setDiagonal( const double& value );
    void transpose();

    double getTrace() const;
    SparseMatrix getTransposition() const;
    SparseMatrix getComposition( const SparseMatrix& other ) const;
    Vector getComposition( const Vector& other ) const;
    SparseCMatrix getComposition( const SparseCMatrix& other ) const;
    CVector getComposition( const CVector& other ) const;
    Vector toVector() const;
    CVector toCVector() const;
    Matrix toMatrix() const;

    Vector getRow( int32_t i ) const;
    Vector getColumn( int32_t j ) const;

    std::map< size_t, double >& getData();
    const std::map< size_t, double >& getData() const;

  protected:

    boost::numeric::ublas::mapped_matrix< double > _matrix;

};


}


//
// unary operators + & -
//

// + SparseMatrix
gkg::SparseMatrix 
operator + ( const gkg::SparseMatrix& thing );

// - SparseMatrix
gkg::SparseMatrix 
operator - ( const gkg::SparseMatrix& thing );


//
// binary operator +
//

// SparseMatrix + SparseMatrix
gkg::SparseMatrix
operator + ( const gkg::SparseMatrix& thing1,
             const gkg::SparseMatrix& thing2 );

// SparseMatrix + SparseCMatrix
gkg::SparseCMatrix
operator + ( const gkg::SparseMatrix& thing1,
             const gkg::SparseCMatrix& thing2 );


//
// binary operator -
//

// SparseMatrix - SparseMatrix
gkg::SparseMatrix
operator - ( const gkg::SparseMatrix& thing1,
             const gkg::SparseMatrix& thing2 );

// SparseMatrix - SparseCMatrix
gkg::SparseCMatrix
operator - ( const gkg::SparseMatrix& thing1,
             const gkg::SparseCMatrix& thing2 );


//
// binary operator *
//

// SparseMatrix * SparseMatrix
gkg::SparseMatrix
operator * ( const gkg::SparseMatrix& thing1,
             const gkg::SparseMatrix& thing2 );

// SparseMatrix * SparseCMatrix
gkg::SparseCMatrix
operator * ( const gkg::SparseMatrix& thing1,
             const gkg::SparseCMatrix& thing2 );

// SparseMatrix * Vector
gkg::Vector
operator * ( const gkg::SparseMatrix& thing1,
             const gkg::Vector& thing2 );

// SparseMatrix * CVector
gkg::CVector
operator * ( const gkg::SparseMatrix& thing1,
             const gkg::CVector& thing2 );

// SparseMatrix * double
gkg::SparseMatrix
operator * ( const gkg::SparseMatrix& thing1,
             const double& thing2 );

// SparseMatrix * complex< double >
gkg::SparseCMatrix
operator * ( const gkg::SparseMatrix& thing1,
             const std::complex< double >& thing2 );

//
// binary operator /
//

// SparseMatrix / double
gkg::SparseMatrix
operator / ( const gkg::SparseMatrix& thing1,
             const double& thing2 );

// SparseMatrix / complex< double >
gkg::SparseCMatrix
operator / ( const gkg::SparseMatrix& thing1,
             const std::complex< double >& thing2 );


namespace std
{

std::ostream& operator << ( ostream& os, const gkg::SparseMatrix& thing );
bool operator == ( const gkg::SparseMatrix& thing1,
                   const gkg::SparseMatrix& thing2 );

}


#endif
