#ifndef _gkg_processing_rfpulse_Magnetization_h_
#define _gkg_processing_rfpulse_Magnetization_h_


#include <complex>
#include <vector>


namespace gkg
{


class RFPulse;


class Magnetization
{

  public:

    Magnetization( double Mx, double My, double Mz );
    Magnetization( const std::complex< double >& Mxy, double Mz );
    Magnetization( const Magnetization& other );
    virtual ~Magnetization();

    double getMx() const;
    double getMy() const;
    double getMz() const;
    const std::complex< double >& getMxy() const;

    Magnetization applyBlochEquation( const RFPulse& rfPulse,
                                      const std::vector< double >& gradientX,
                                      const std::vector< double >& gradientY,
                                      double coordX,
                                      double coordY,
                                      bool crushed = false ) const;

  protected:

    std::complex< double > _Mxy;
    double _Mz;

};


}


#endif
