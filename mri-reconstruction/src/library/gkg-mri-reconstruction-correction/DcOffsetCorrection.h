#ifndef _gkg_mri_reconstruction_correction_DcOffsetCorrection_h_
#define _gkg_mri_reconstruction_correction_DcOffsetCorrection_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <complex>


namespace gkg
{


template < class T > class Volume;


template < class T >
class DcOffsetCorrection : public Singleton< DcOffsetCorrection< T > >
{

  public:

    ~DcOffsetCorrection();

    void apply( Volume< std::complex< T > >& data,
                int32_t rawSizeX, 
                int32_t rawSizeY,
                int32_t rawSizeZ,
                std::complex< T >* dcOffset = 0,
                bool verbose = false ) const;

  private:

    friend class Singleton< DcOffsetCorrection< T > >;

    DcOffsetCorrection();

};


}


#endif
