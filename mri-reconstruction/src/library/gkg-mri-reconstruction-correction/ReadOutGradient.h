#ifndef _gkg_mri_reconstruction_correction_ReadOutGradient_h_
#define _gkg_mri_reconstruction_correction_ReadOutGradient_h_


#include <gkg-core-cppext/StdInt.h>
#include <vector>


namespace gkg
{


template < class T >
class ReadOutGradient
{

  public:

    virtual ~ReadOutGradient();

    std::vector< T > getTrajectory() const;

  protected:

    ReadOutGradient( int32_t sampleCount );

    std::vector< T > _trajectory;

};


}


#endif
