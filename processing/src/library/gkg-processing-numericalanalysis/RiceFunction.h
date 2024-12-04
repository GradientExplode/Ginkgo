#ifndef _gkg_processing_numericalanalysis_RiceFunction_h_
#define _gkg_processing_numericalanalysis_RiceFunction_h_


#include <gkg-processing-numericalanalysis/ParametricFunction.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>


namespace gkg
{


class RiceFunction : public ParametricFunction
{

  public:
  
    RiceFunction( const Vector& parameters );
    
    void getValueAt( const Vector& x, Vector& y ) const;
    void getValuesAt( Matrix& x, Matrix& y ) const;
    
    double getApproximateMean() const;
    double getApproximateStandardDeviation() const;

  private:
    
    double getValueAt( double x ) const;
    
    gkg::NumericalAnalysisImplementationFactory* _factory;

    double _approximateMean;
    double _approximateStandardDeviation;

};


}


#endif
