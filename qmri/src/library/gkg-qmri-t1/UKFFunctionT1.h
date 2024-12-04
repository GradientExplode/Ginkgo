#ifndef _gkg_qmri_t1_UKFFunctionT1_h_
#define _gkg_qmri_t1_UKFFunctionT1_h_


#include <gkg-processing-numericalanalysis/KFFunction.h>


namespace gkg
{


class UKFFunctionT1 : public KFFunction
{

  public :
   
    UKFFunctionT1();
   
    void getValuesAt( const Matrix& X, Vector& Y ) const;

};
   

}
#endif
