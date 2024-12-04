#ifndef _gkg_processing_registration_MotionCorrection_h_
#define _gkg_processing_registration_MotionCorrection_h_


#include <gkg-processing-registration/RegistrationAlgorithm.h>


namespace gkg
{


template < class T >
class MotionCorrection
{

  public:

    MotionCorrection( RCPointer< RegistrationAlgorithm< T, T > >
                                                          registrationAlgorithm,
                      int32_t resamplingOrder,
                      const T& background,
                      bool verbose );
    virtual ~MotionCorrection();

    void correct( RCPointer< Volume< T > > volume,
                  int32_t referenceRank,
                  const Vector& initialParameters,
                  const T& lowerThreshold,
                  std::vector< RCPointer< NonLinearTransform3d< float > > >&
                    transform3ds,
                  RCPointer< Transform3d< float > > furtherMotion,
                  const Vector3d< int32_t >& furtherSliceCount );
    void correct( RCPointer< Volume< T > > volume,
                  RCPointer< Volume< T > > reference,
                  const Vector& initialParameters,
                  const T& lowerThreshold,
                  std::vector< RCPointer< NonLinearTransform3d< float > > >&
                    transform3ds,
                  RCPointer< Transform3d< float > > furtherMotion,
                  const Vector3d< int32_t >& furtherSliceCount );

  protected:

    RCPointer< RegistrationAlgorithm< T, T > > _registrationAlgorithm;
    int32_t _resamplingOrder;
    T _background;
    bool _verbose;

};


}


#endif
