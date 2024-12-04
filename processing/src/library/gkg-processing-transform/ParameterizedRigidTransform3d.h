#ifndef _gkg_processing_transform_ParameterizedRigidTransform3d_h_
#define _gkg_processing_transform_ParameterizedRigidTransform3d_h_


#include <gkg-processing-transform/ParameterizedTransform3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>


namespace gkg
{


//
//    parameters[ 0 ] -> X rotation angle in radians
//    parameters[ 1 ] -> Y rotation angle in radians
//    parameters[ 2 ] -> Z rotation angle in radians
//    parameters[ 3 ] -> X translation in millimeters
//    parameters[ 4 ] -> Y translation in millimeters
//    parameters[ 5 ] -> Z translation in millimeters
//

template < class T >
class ParameterizedRigidTransform3d : public ParameterizedTransform3d< T >
{

  public:

    ParameterizedRigidTransform3d();
    ParameterizedRigidTransform3d(
                                const RigidTransform3d< T >& rigidTransform3d );
    ParameterizedRigidTransform3d(
                              const ParameterizedRigidTransform3d< T >& other );
    virtual ~ParameterizedRigidTransform3d();

    int32_t getParameterCount() const;

    RCPointer< Transform3d< T > > clone() const;

    ParameterizedRigidTransform3d< T >& operator=(
                              const ParameterizedRigidTransform3d< T >& other );

    const RigidTransform3d< T >& getRigidTransform3d() const;
    RigidTransform3d< T >& getRigidTransform3d();

    void getDirectParameters( std::vector< T >& parameters ) const;
    void getInverseParameters( std::vector< T >& parameters ) const;

    void readTrm( std::istream& is );
    void writeTrm( std::ostream& os ) const;

  protected:

    void setDirectParametersImpl( const std::vector< T >& parameters );
    void setInverseParametersImpl( const std::vector< T >& parameters );

    RigidTransform3d< T >* _rigidTransform3d;

};


}


#endif
