#ifndef _gkg_processing_numericalanalysis_PowellNewUOAFunction_h_
#define _gkg_processing_numericalanalysis_PowellNewUOAFunction_h_


#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/OptimizerConstraint.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>


namespace gkg
{


class PowellNewUOAFunction
{

  public:

    virtual ~PowellNewUOAFunction();

    int32_t getParameterCount() const;

    void addConstraints(
           const std::vector< RCPointer< OptimizerConstraint > >& constraints );
                           
    void getConstrainedParameters( const gkg::Vector& unconstrainedParameters,
                                   gkg::Vector& constrainedParameters ) const;

    virtual double getValueAt( const Vector& parameters ) const;
    virtual double getValueAt( const double* parameters,
                               int32_t count ) const = 0;

  protected:

    PowellNewUOAFunction( int32_t parameterCount );

    void applyConstraints( const double* parameters, int32_t count,
                           double* constrainedParameters ) const;

    int32_t _parameterCount;
    std::vector< gkg::RCPointer< gkg::OptimizerConstraint > > _constraints;
    

};


}


#endif
