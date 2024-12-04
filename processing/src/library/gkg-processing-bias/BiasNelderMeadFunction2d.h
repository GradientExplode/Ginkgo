#ifndef _gkg_processing_bias_BiasNelderMeadFunction2d_h_
#define _gkg_processing_bias_BiasNelderMeadFunction2d_h_


#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/NelderMeadSimplexFunction.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


//
// class BiasFunctor< T >
//


template < class T >
struct BiasFunctor
{

  virtual ~BiasFunctor()
  {
  }
  
  virtual double getValueAt( const T& value, double parameter ) const = 0;

};


template < class T >
struct MultiplicativeBiasFunctor : public BiasFunctor< T >
{

  double getValueAt( const T& value, double parameter ) const
  {

    return ( ( double )value * parameter );

  }

};


template < class T >
struct AdditiveBiasFunctor : public BiasFunctor< T >
{

  double getValueAt( const T& value, double parameter ) const
  {

    return ( ( double )value + parameter );

  }

};



//
// class BiasNelderMeadFunction2d< T >
//


template < class T >
class BiasNelderMeadFunction2d : public NelderMeadSimplexFunction
{

  public:

    BiasNelderMeadFunction2d( RCPointer< BiasFunctor< T > > biasFunctor,
                              const Volume< T >& in,
                              const Volume< int16_t >& mask,
                              const Volume< float >& weight,
                              Volume< T >& out,
                              Volume< float >& bias,
                              const std::string& planeAxis );
    ~BiasNelderMeadFunction2d();

    void setReferenceSlice( int32_t slice );
    void setBiasedSlice( int32_t slice );
    void setTime( int32_t time );

    virtual double getValueAt( const Vector& parameters ) const = 0;
    void applyBiasCorrection( double scale );

  protected:

    RCPointer< BiasFunctor< T > > _biasFunctor;
    const Volume< T >& _in;
    const Volume< int16_t >& _mask;
    const Volume< float >& _weight;
    Volume< T >& _out;
    Volume< float >& _bias;
    std::string _planeAxis;
    int32_t _referenceSlice;
    int32_t _biasedSlice;
    int32_t _time;

};


}


#endif

