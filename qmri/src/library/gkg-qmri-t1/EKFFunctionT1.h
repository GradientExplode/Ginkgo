#ifndef _gkg_qmri_t1_EKFFunctionT1_h_
#define _gkg_qmri_t1_EKFFunctionT1_h_


#include <gkg-processing-numericalanalysis/KFFunction.h>


namespace gkg
{


class EKFFunctionT1 : public KFFunction
{

  public :
   
    EKFFunctionT1();
   
    void getValueAt( const Vector& X, double& Y ) const;
    void getJacobian( const Vector& X, Vector& J ) const;

};
   

}
#endif
