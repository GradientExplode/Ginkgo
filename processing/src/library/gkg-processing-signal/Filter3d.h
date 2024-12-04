#ifndef _gkg_processing_signal_Filter3d_h_
#define _gkg_processing_signal_Filter3d_h_


#include <gkg-processing-morphology/Neighborhood3d.h>
#include <vector>


namespace gkg
{


template < class T > class Volume;
template < class T > class TestFunction;
template < class IN, class OUT > class FilterFunction;


template < class IN, class MASK, class OUT >
class Filter3d
{

  public:

    void filter( const Volume< IN >& in,
                 const Volume< MASK >& mask,
                 const TestFunction< MASK >& testFunction,
                 const FilterFunction< std::vector< IN >, OUT >& filterFunction,
                 Neighborhood3d::Type neighborhood3dType,
                 Volume< OUT >& out,
                 bool verbose = false ) const;

};


}


#endif
