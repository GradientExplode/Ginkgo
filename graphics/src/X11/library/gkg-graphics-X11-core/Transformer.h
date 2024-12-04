#ifndef _gkg_graphics_X11_core_Transformer_h_
#define _gkg_graphics_X11_core_Transformer_h_


#include <gkg-graphics-X11-base/RCGUIObject.h>


namespace gkg
{


class Transformer : public RCGUIObject
{

  public:

    Transformer();	// set to identity
    Transformer( const Transformer& other );
    Transformer( const Transformer* pOther );
    Transformer( float a00, float a01,
                 float a10, float a11,
                 float a20, float a21 );
    virtual ~Transformer();

    Transformer& operator=( const Transformer& other );
    bool operator==( const Transformer& other ) const;
    bool operator!=( const Transformer& other ) const;

    void setIdentity();
    bool isIdentity() const;
    bool isInvertible() const;

    virtual void preMultiply( const Transformer& other );
    virtual void postMultiply( const Transformer& other );
    virtual void invert();

    virtual void translate( float dx, float dy );
    virtual void scale( float sx, float sy );
    virtual void rotate( float angle );   // given in degrees
    virtual void skew( float sx, float sy );

    virtual void transform( float& x, float& y ) const;
    virtual void transform( float x, float y, float& tx, float& ty ) const;
    virtual void inverseTransform( float& tx, float& ty ) const;
    virtual void inverseTransform( float tx, float ty,
                                   float& x, float& y ) const;

    float getDeterminant() const;
    virtual void getMatrix( float& a00, float& a01,
                            float& a10, float& a11,
                            float& a20, float& a21 ) const;

    virtual bool isStraight( float x0, float y0, float x1, float y1,
                             float x2, float y2, float x3, float y3 ) const;

    virtual int32_t getKey( float x, float y ) const;

  private:

    void update();

    static int key( int32_t i );

    bool _isIdentity;
    float _mat00, _mat01, _mat10, _mat11, _mat20, _mat21;

};


}


#endif
