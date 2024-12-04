#ifndef _gkg_processing_mask_MaskAlgorithm_h_
#define _gkg_processing_mask_MaskAlgorithm_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


template < class T > class Volume;


template < class IN, class OUT >
class MaskAlgorithm
{

  public:

    virtual ~MaskAlgorithm();

    virtual std::string getName() const = 0;

    virtual void getMask( const Volume< IN >& volume,
                          Volume< OUT >& mask,
                          bool verbose ) const = 0;

  protected:

    MaskAlgorithm();

};


}


#endif
