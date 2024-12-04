#ifndef _gkg_processing_numericalanalysis_IllMatrixInverser_h_
#define _gkg_processing_numericalanalysis_IllMatrixInverser_h_


namespace gkg
{


class Vector;
class CVector;
class Matrix;
class CMatrix;


//
// solving equation b = A x

class IllMatrixInverser
{

  public:

    virtual ~IllMatrixInverser();

    virtual void inverse( const Matrix& A,
                          const Vector& b,
                          Vector& x ) const;
                     
    virtual void inverse( const CMatrix& A,
                          const CVector& b,
                          CVector& x ) const = 0;

  protected:

    IllMatrixInverser();

};


}


#endif
