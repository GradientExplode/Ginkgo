#ifndef _gkg_processing_numericalanalysis_KFFunction_h_
#define _gkg_processing_numericalanalysis_KFFunction_h_


#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class KFFunction
{

  public:

    KFFunction( int32_t parameterCount );
    virtual ~KFFunction();

    void setParameters( const Vector& parameters );

    int32_t getParameterCount() const;

    virtual void getValueAt( const Vector& X, double& Y ) const;
    virtual void getValuesAt( const Matrix& X, Vector& Y ) const;
    virtual void getJacobian( const Vector& X, Vector& J ) const;

  protected:

    Vector _parameters;
    int32_t _parameterCount;

};


}


#endif
