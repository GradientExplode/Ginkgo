#ifndef _gkg_qmri_t2_UKFFunctionT2_h_
#define _gkg_qmri_t2_UKFFunctionT2_h_


#include <gkg-processing-numericalanalysis/KFFunction.h>


namespace gkg
{


class UKFFunctionT2 : public KFFunction
{

  public :
   
    UKFFunctionT2();
   
    void getValuesAt( const Matrix& X, Vector& Y ) const;

};
   

}
#endif
