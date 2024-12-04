#ifndef _gkg_processing_signal_Scrolling3d_h_
#define _gkg_processing_signal_Scrolling3d_h_


#include <gkg-core-pattern/Singleton.h>
#include <complex>


namespace gkg
{


template < class T > class Volume;


template < class T >
class Scrolling3d : public Singleton< Scrolling3d< T > >
{

  public:

    enum Axis
    {

      X_AXIS,
      Y_AXIS,
      Z_AXIS

    };

    ~Scrolling3d();

    void apply( Volume< std::complex< T > >& data,
                const T& scrollingRatio,
                Axis axis,
                bool verbose = false ) const;

  private:

    friend class Singleton< Scrolling3d< T > >;

    Scrolling3d();

};


}


#endif
