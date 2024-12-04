#ifndef _gkg_fmri_stimulus_Stimulus_h_
#define _gkg_fmri_stimulus_Stimulus_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-fmri-hrf/HemodynamicResponseFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <string>


namespace gkg
{


class Stimulus
{

  public:

    virtual ~Stimulus();

    virtual const std::string& getName() const;

    virtual void sample( const Vector& times,
                         Vector& result ) const = 0;

    virtual void convolve( const HemodynamicResponseFunction& hrf,
                           const Vector& times,
                           Vector& result ) const = 0;

  protected:

    Stimulus( const std::string& name );

    std::string _name;

};


}


#endif
