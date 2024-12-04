#ifndef _gkg_processing_morphology_Erosion_h_
#define _gkg_processing_morphology_Erosion_h_



namespace gkg
{


template < class T > class Volume;
template < class T > class TestFunction;


template < class IN, class OUT >
class Erosion
{

  public:

    Erosion( float radius,
             const TestFunction< IN >& testFunction,
             const OUT& foreground,
             const OUT& background );
    virtual ~Erosion();

    void erode( const Volume< IN >& inputVolume,
                Volume< OUT >& erodedVolume,
                bool verbose = false ) const;

  protected:

    float _radius;
    const TestFunction< IN >& _testFunction;
    OUT _foreground;
    OUT _background;

};


}


#endif
