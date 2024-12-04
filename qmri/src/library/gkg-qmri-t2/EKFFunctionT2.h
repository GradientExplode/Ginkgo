#ifndef _gkg_qmri_t2_EKFFunctionT2_h_
#define _gkg_qmri_t2_EKFFunctionT2_h_


#include <gkg-processing-numericalanalysis/KFFunction.h>


namespace gkg
{


class EKFFunctionT2 : public KFFunction
{

  public :
   
    EKFFunctionT2();
   
    void getValueAt( const Vector& X, double& Y ) const;
    void getJacobian( const Vector& X, Vector& J ) const;

};
   

}
#endif
