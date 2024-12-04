#ifndef _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_AcquisitionParameterSet_h_
#define _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_AcquisitionParameterSet_h_


#include <gkg-processing-numericalanalysis/Vector.h>
#include <string>


namespace gkg
{


class AcquisitionParameterSet
{

  public:

    virtual ~AcquisitionParameterSet();


    static void readTextFile( const std::string& fileName,
                              int32_t count, 
                              Vector& values );

  protected:

    AcquisitionParameterSet();


};


}


#endif
