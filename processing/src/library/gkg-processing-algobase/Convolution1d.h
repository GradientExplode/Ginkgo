#ifndef _gkg_processing_algobase_Convolution1d_h_
#define _gkg_processing_algobase_Convolution1d_h_


namespace gkg
{


class Vector;


class Convolution1d
{

  public:

    void convolve( const Vector& in1,
                   const Vector& in2,
                   Vector& out ) const;

};


}


#endif
