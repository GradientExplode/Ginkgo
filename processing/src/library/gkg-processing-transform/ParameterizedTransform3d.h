#ifndef _gkg_processing_transform_ParameterizedTransform3d_h_
#define _gkg_processing_transform_ParameterizedTransform3d_h_


#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <vector>
#include <iostream>


namespace gkg
{


template < class T >
class ParameterizedTransform3d : public Transform3d< T >
{

  public:

    virtual ~ParameterizedTransform3d();

    void getDirect( const Vector3d< T >& from, Vector3d< T >& to ) const;
    void getInverse( const Vector3d< T >& to, Vector3d< T >& from ) const;

    bool isComposable() const;

    virtual int32_t getParameterCount() const = 0;

    void setDirectParameters( const std::vector< T >& parameters,
                              bool update = true );
    void setDirectParameters( const Vector& parameters,
                              bool update = true );
    virtual void getDirectParameters( std::vector< T >& parameters ) const = 0;
    void getDirectParameters( Vector& parameters ) const;

    void setInverseParameters( const std::vector< T >& parameters,
                               bool update = true );
    void setInverseParameters( const Vector& parameters,
                               bool update = true );
    virtual void getInverseParameters( std::vector< T >& parameters ) const = 0;
    void getInverseParameters( Vector& parameters ) const;

    virtual
       void getDerivativeAlongParameters( const Vector3d< T >& from,
                                          std::vector< T >& derivatives ) const;
    virtual void getDerivativeAlongParameters( const Vector3d< T >& from,
                                               Vector& derivatives ) const;
    virtual T getDerivativeAlongParameter( const Vector3d< T >& from,
                                           int32_t parameterIndex ) const;
    virtual bool hasDerivativeAlongParameters() const;

    virtual void readTrm( std::istream& is ) = 0;
    virtual void writeTrm( std::ostream& os ) const = 0;


  protected:

    ParameterizedTransform3d( RCPointer< Transform3d< T > > transform3d );
    ParameterizedTransform3d( const Transform3d< T >& transform3d );

    virtual void setDirectParametersImpl(
                                       const std::vector< T >& parameters ) = 0;
    virtual void setInverseParametersImpl(
                                       const std::vector< T >& parameters ) = 0;

    RCPointer< Transform3d< T > > _transform3d;

};


}


#endif
