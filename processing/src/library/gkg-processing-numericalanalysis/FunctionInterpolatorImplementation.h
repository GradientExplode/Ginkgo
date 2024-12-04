#ifndef _gkg_processing_numericalanalysis_FunctionInterpolatorImplementation_h_
#define _gkg_processing_numericalanalysis_FunctionInterpolatorImplementation_h_


#include <gkg-processing-numericalanalysis/FunctionInterpolator.h>


namespace gkg
{


class FunctionInterpolatorImplementation
{

  public:


    virtual ~FunctionInterpolatorImplementation();

    virtual double getValueAt( double x ) const = 0;

    virtual FunctionInterpolator* getTarget() const;

  protected:

    FunctionInterpolatorImplementation(
                          FunctionInterpolator* functionInterpolator,
                          FunctionInterpolator::Type type,
                          const std::vector< double >& xs,
                          const std::vector< double >& ys );
    FunctionInterpolatorImplementation(
                          FunctionInterpolator* functionInterpolator,
                          FunctionInterpolator::Type type,
                          const Vector& xs,
                          const Vector& ys );

    FunctionInterpolator* _target;

};


}


#endif
