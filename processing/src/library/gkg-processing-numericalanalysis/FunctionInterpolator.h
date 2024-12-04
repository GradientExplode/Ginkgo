#ifndef _gkg_processing_numericalanalysis_FunctionInterpolator_h_
#define _gkg_processing_numericalanalysis_FunctionInterpolator_h_


#include <vector>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class FunctionInterpolatorImplementation;


class FunctionInterpolator
{

  public:

    enum Type
    {

      Linear,
      Polynomial,
      CubicSpline,
      CubicSplinePeriodic,
      NonRoundedAkimaSpline,
      NonRoundedAkimaSplinePeriodic

    };

    FunctionInterpolator( Type type,
                          const std::vector< double >& xs,
                          const std::vector< double >& ys );
    FunctionInterpolator( Type type,
                          const Vector& xs,
                          const Vector& ys );
    virtual ~FunctionInterpolator();

    double getValueAt( double x ) const;

    FunctionInterpolatorImplementation* getImplementation() const;

  protected:

    FunctionInterpolatorImplementation* _functionInterpolatorImplementation;

};


}


#endif
