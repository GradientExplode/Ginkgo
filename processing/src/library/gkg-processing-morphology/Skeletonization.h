#ifndef _gkg_processing_morphology_Skeletonization_h_
#define _gkg_processing_morphology_Skeletonization_h_


namespace gkg
{


template < class T > class Volume;
template < class T > class TestFunction;

template < class IN, class OUT >
class Skeletonization
{

  public:

    Skeletonization( const TestFunction< IN >& testFunction,
                     const OUT& foreground,
                     const OUT& background );
    virtual ~Skeletonization();

    void skeletonize( const Volume< IN >& inputVolume,
                      Volume< OUT >& skeletonizedVolume,
                      bool verbose = false ) const;

  protected:

    const TestFunction< IN >& _testFunction;
    OUT _foreground;
    OUT _background;

};


}


#endif
