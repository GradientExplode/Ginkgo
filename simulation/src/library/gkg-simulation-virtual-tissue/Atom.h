#ifndef _gkg_simulation_virtual_tissue_Atom_h_
#define _gkg_simulation_virtual_tissue_Atom_h_


#include <gkg-processing-coordinates/Vector3d.h>


namespace gkg
{


struct Atom
{

  public:

    virtual ~Atom();

    Atom& operator=( const Atom& other );

    virtual Atom* clone() const = 0;

    void setParameters( const float* inputParameters );
    void copyParameters( float* outputParameters );

    virtual bool hasCompressedRepresentation() const;
    virtual void switchRepresentation();

    virtual uint8_t getType() const;
    virtual int32_t getParameterCount() const;
    virtual void setCenter( const Vector3d< float >& center ) = 0;
    virtual Vector3d< float > getCenter() const = 0;
    virtual float getSpanRadius() const = 0;
    virtual float getVolume() const = 0;
    virtual bool contains( const Vector3d< float >& position ) const = 0; 

    virtual void getTranslatedParameters( float* outputParameters,
                                          float translationX,
                                          float translationY,
                                          float translationZ ) const = 0;



    std::vector< float > parameters;

  protected:

    Atom();
    Atom( const Atom& other );

};


typedef Atom* PAtom;


}


#endif

