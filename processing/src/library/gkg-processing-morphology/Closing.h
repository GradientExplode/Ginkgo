#ifndef _gkg_processing_morphology_Closing_h_
#define _gkg_processing_morphology_Closing_h_



namespace gkg
{


template < class T > class Volume;
template < class T > class TestFunction;


template < class IN, class OUT >
class Closing
{

  public:

    Closing( float radius,
             const TestFunction< IN >& testFunction,
             const OUT& foreground,
             const OUT& background );
    virtual ~Closing();

    void close( const Volume< IN >& inputVolume,
                Volume< OUT >& closedVolume,
                bool verbose = false ) const;

  protected:

    float _radius;
    const TestFunction< IN >& _testFunction;
    OUT _foreground;
    OUT _background;

};


}


#endif
