#ifndef _gkg_dartel_core_DartelExponentiation_h_
#define _gkg_dartel_core_DartelExponentiation_h_


#include <gkg-processing-container/Volume.h>


namespace gkg
{


template < class T >
class DartelExponentiation
{

  public:
  
    DartelExponentiation( int32_t k );
    virtual ~DartelExponentiation();

    void getDirectVectorField( const Volume< T >& flowfield,
                               Volume< T >& expU );
    void getInverseVectorField( const Volume< T >& flowfield,
                                Volume< T >& expU );
    
  private:

    void smalldef( double sc, const T *v0, T* t0, float* m );
    void expdef( int32_t k, double sc, const T* v, 
                 T* t0, T* t1, float* m );
    void unwrap( T* f );
    void dartel3_exp( const Volume< T >& U, int32_t k, double sc,
                      Volume< T >& expU );
    void dartel_integrate( const Volume< T >& U, double t0, double t1, 
                           Volume< T >& expU );

    int32_t _k;
    int32_t _sizeX;
    int32_t _sizeY;
    int32_t _sizeZ;
    int32_t _sizeT;
    int32_t _sizeXYZ;
    

};


}


#endif
