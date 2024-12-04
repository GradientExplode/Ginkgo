#ifndef _gkg_processing_topology_HomotopicHoleFiller_h_
#define _gkg_processing_topology_HomotopicHoleFiller_h_


namespace gkg
{


template < class T > class Volume;
template < class T > class TestFunction;

template < class IN >
class HomotopicHoleFiller
{

  public:

    HomotopicHoleFiller( const IN& label );
    virtual ~HomotopicHoleFiller();

    void fillHoles( Volume< IN >& inputVolume );

  protected:

    IN _label;

};


}


#endif
