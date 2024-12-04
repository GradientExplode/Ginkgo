#ifndef _gkg_processing_transform_NonLinearTransform3d_h_
#define _gkg_processing_transform_NonLinearTransform3d_h_


#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-transform/ParameterizedRigidTransform3d.h>
#include <gkg-processing-transform/ParameterizedAffineWoShearingTransform3d.h>
#include <gkg-processing-transform/ParameterizedAffineTransform3d.h>
#include <gkg-processing-transform/VectorFieldTransform3d.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


template < class T >
class NonLinearTransform3d : public Transform3d< T >
{

  public:

    NonLinearTransform3d( bool applyInverseLinearTransformFirst = false );
    NonLinearTransform3d(
       const RCPointer< ParameterizedRigidTransform3d< T > >&
                                                  parameterizedRigidTransform3d,
       const RCPointer< VectorFieldTransform3d< T > >& vectorFieldTransform3d,
       bool applyInverseLinearTransformFirst = false );
    NonLinearTransform3d(
       const RCPointer< ParameterizedAffineWoShearingTransform3d< T > >&
                                       parameterizedAffineWoShearingTransform3d,
       const RCPointer< VectorFieldTransform3d< T > >& vectorFieldTransform3d,
       bool applyInverseLinearTransformFirst = false );
    NonLinearTransform3d(
       const RCPointer< ParameterizedAffineTransform3d< T > >& 
                                                 parameterizedAffineTransform3d,
       const RCPointer< VectorFieldTransform3d< T > >& vectorFieldTransform3d,
       bool applyInverseLinearTransformFirst = false );
    NonLinearTransform3d(
       const RCPointer< VectorFieldTransform3d< T > >& vectorFieldTransform3d,
       bool applyInverseLinearTransformFirst = false );
    NonLinearTransform3d( const NonLinearTransform3d< T >& other );
    virtual ~NonLinearTransform3d();

    RCPointer< Transform3d< T > > clone() const;

    void getDirect( const Vector3d< T >& from, 
                    Vector3d< T >& to ) const;
    void getInverse( const Vector3d< T >& to, 
                     Vector3d< T >& from ) const;

    void readTrm( const std::string& fileNameLinearTransform,
                  const std::string& fileNameNonLinearDirectTransform,
                  const std::string& fileNameNonLinearInverseTransform );
    void writeTrm( const std::string& fileNameLinearTransform,
                   const std::string& fileNameNonLinearDirectTransform,
                   const std::string& fileNameNonLinearInverseTransform ) const;

    void writeInverseTrm(
                   const std::string& fileNameLinearTransform,
                   const std::string& fileNameNonLinearDirectTransform,
                   const std::string& fileNameNonLinearInverseTransform ) const;
                  
    const RCPointer< ParameterizedRigidTransform3d< T > >&
      getParameterizedRigidTransform3d() const;
    const RCPointer< ParameterizedAffineWoShearingTransform3d< T > >&
      getParameterizedAffineWoShearingTransform3d() const;
    const RCPointer< ParameterizedAffineTransform3d< T > >&
      getParameterizedAffineTransform3d() const;

    const RCPointer< ParameterizedTransform3d< T > >&
      getParameterizedLinearTransform3d() const;
    const RCPointer< VectorFieldTransform3d< T > >&
      getVectorFieldTransform3d() const;

  protected:

    RCPointer< ParameterizedRigidTransform3d< T > >
                                                 _parameterizedRigidTransform3d;
    RCPointer< ParameterizedAffineWoShearingTransform3d< T > >
                                      _parameterizedAffineWoShearingTransform3d;
    RCPointer< ParameterizedAffineTransform3d< T > >
                                                _parameterizedAffineTransform3d;

    RCPointer< ParameterizedTransform3d< T > > _linearTransform3d;
    RCPointer< VectorFieldTransform3d< T > > _vectorFieldTransform3d;

    bool _applyInverseLinearTransformFirst;

};


}


#endif

