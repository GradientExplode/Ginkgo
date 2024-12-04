#ifndef _gkg_processing_numericalanalysis_OptimizerConstraint_h_
#define _gkg_processing_numericalanalysis_OptimizerConstraint_h_


#include <string>


namespace gkg
{

// base class
class OptimizerConstraint
{

  public:

    virtual ~OptimizerConstraint();

    virtual double transform( double value ) const = 0;
    virtual double inverseTransform( double value ) const = 0;
    virtual double inverseDerivative( double value ) const = 0;

    virtual double constrainedDeltaParameter( double deltaParameter ) const;

    virtual std::string displayType() const = 0;
    virtual std::string displayTransformed( double value ) const;

  protected:

    OptimizerConstraint();

};


// no constraint
class NoneOptimizerConstraint : public OptimizerConstraint
{

  public:

    NoneOptimizerConstraint();
    virtual ~NoneOptimizerConstraint();

    double transform( double value ) const;
    double inverseTransform( double value ) const;
    double inverseDerivative( double value ) const;

    std::string displayType() const;

};


// [ a; +inf )
class SemiClosedRightHalfLineOptimizerConstraint : public OptimizerConstraint
{

  public:

    SemiClosedRightHalfLineOptimizerConstraint( double minimum );
    virtual ~SemiClosedRightHalfLineOptimizerConstraint();

    double transform( double value ) const;
    double inverseTransform( double value ) const;
    double inverseDerivative( double value ) const;

    std::string displayType() const;

  protected:

    double _minimum;

};


// ( a; +inf )
class OpenRightHalfLineOptimizerConstraint : public OptimizerConstraint
{

  public:

    OpenRightHalfLineOptimizerConstraint( double minimum );
    virtual ~OpenRightHalfLineOptimizerConstraint();

    double transform( double value ) const;
    double inverseTransform( double value ) const;
    double inverseDerivative( double value ) const;

    std::string displayType() const;

  protected:

    double _minimum;

};


// ( a; +inf )
class OpenRightHalfLineAlternativeOptimizerConstraint :
                                                      public OptimizerConstraint
{

  public:

    OpenRightHalfLineAlternativeOptimizerConstraint( double minimum );
    virtual ~OpenRightHalfLineAlternativeOptimizerConstraint();

    double transform( double value ) const;
    double inverseTransform( double value ) const;
    double inverseDerivative( double value ) const;

    std::string displayType() const;

  protected:

    double _minimum;

};


// ( -inf; a ]
class SemiClosedLeftHalfLineOptimizerConstraint : public OptimizerConstraint
{

  public:

    SemiClosedLeftHalfLineOptimizerConstraint( double maximum );
    virtual ~SemiClosedLeftHalfLineOptimizerConstraint();

    double transform( double value ) const;
    double inverseTransform( double value ) const;
    double inverseDerivative( double value ) const;

    std::string displayType() const;

  protected:

    double _maximum;

};


// ( -inf; a )
class OpenLeftHalfLineOptimizerConstraint : public OptimizerConstraint
{

  public:

    OpenLeftHalfLineOptimizerConstraint( double maximum );
    virtual ~OpenLeftHalfLineOptimizerConstraint();

    double transform( double value ) const;
    double inverseTransform( double value ) const;
    double inverseDerivative( double value ) const;

    std::string displayType() const;

  protected:

    double _maximum;

};


// ( -inf; a )
class OpenLeftHalfLineAlternativeOptimizerConstraint :
                                                      public OptimizerConstraint
{

  public:

    OpenLeftHalfLineAlternativeOptimizerConstraint( double maximum );
    virtual ~OpenLeftHalfLineAlternativeOptimizerConstraint();

    double transform( double value ) const;
    double inverseTransform( double value ) const;
    double inverseDerivative( double value ) const;

    std::string displayType() const;

  protected:

    double _maximum;

};


// [ a; b ]
class ClosedIntervalOptimizerConstraint : public OptimizerConstraint
{

  public:

    ClosedIntervalOptimizerConstraint( double minimum, double maximum );
    virtual ~ClosedIntervalOptimizerConstraint();

    double transform( double value ) const;
    double inverseTransform( double value ) const;
    double inverseDerivative( double value ) const;

    double constrainedDeltaParameter( double deltaParameter ) const;

    std::string displayType() const;

  protected:

    double _minimum;
    double _maximum;

};


// ( a; b )
class OpenIntervalOptimizerConstraint : public OptimizerConstraint
{

  public:

    OpenIntervalOptimizerConstraint( double minimum, double maximum );
    virtual ~OpenIntervalOptimizerConstraint();

    double transform( double value ) const;
    double inverseTransform( double value ) const;
    double inverseDerivative( double value ) const;

    std::string displayType() const;

  protected:

    double _minimum;
    double _maximum;

};


// ( a; b )
class OpenIntervalAlternativeOptimizerConstraint : public OptimizerConstraint
{

  public:

    OpenIntervalAlternativeOptimizerConstraint( double minimum, double maximum );
    virtual ~OpenIntervalAlternativeOptimizerConstraint();

    double transform( double value ) const;
    double inverseTransform( double value ) const;
    double inverseDerivative( double value ) const;

    std::string displayType() const;

  protected:

    double _minimum;
    double _maximum;

};


}


#endif
