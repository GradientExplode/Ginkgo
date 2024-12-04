#ifndef _gkg_processing_extrapolation_RecursiveExtrapolator3d_h_
#define _gkg_processing_extrapolation_RecursiveExtrapolator3d_h_


#include <gkg-processing-extrapolation/Extrapolator.h>
#include <gkg-processing-algobase/FilterFunction.h>
#include <gkg-processing-morphology/Neighborhood3d.h>


namespace gkg
{


template < class IN, class MASK, class OUT >
class RecursiveExtrapolator3d : public Extrapolator< IN, MASK, OUT >
{

  public:

    RecursiveExtrapolator3d(
                 const FilterFunction< std::vector< IN >, OUT >& filterFunction,
                 Neighborhood3d::Type neighborhood3dType );
    ~RecursiveExtrapolator3d();

    void extrapolate( const Volume< IN >& input,
                      const Volume< MASK >& restrictedMask,
                      const Volume< MASK >& enlargedMask,
                      const TestFunction< MASK >& testFunction,
                      Volume< OUT >& ouput,
                      bool verbose = false ) const;

  private:

    const FilterFunction< std::vector< IN >, OUT >& _filterFunction;
    Neighborhood3d::Type _neighborhood3dType;

};


}


#endif

