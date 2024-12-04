#include <gkg-fmri-hrf/CanonicalHRF.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>


#define CANONICAL_HRF_TINY  1e-300

gkg::CanonicalHRF::CanonicalHRF( double delay,
                                 double delayUndershoot,
                                 double dispersion,
                                 double dispersionUndershoot,
                                 double ratio )
                  : gkg::HemodynamicResponseFunction( 1 ),
                    _delay( delay ),
                    _delayUndershoot( delayUndershoot ),
                    _dispersion( dispersion ),
                    _dispersionUndershoot( dispersionUndershoot ),
                    _ratio( ratio )
{

  try
  {

    // getting numerical analysis implementation factory
    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

  }
  GKG_CATCH( "gkg::CanonicalHRF::CanonicalHRF( double delay, "
             "double delayUndershoot, "
             "double dispersion, "
             "double dispersionUndershoot, "
             "double ratio )" );

}


gkg::CanonicalHRF::~CanonicalHRF()
{
}


double gkg::CanonicalHRF::getValue( double t ) const
{

  try
  {

    double 
      value = _ratio * _factory->getGammaPdf( t,
                                              _delay / _dispersion,
                                              1.0 / _dispersion ) -
              _factory->getGammaPdf( t,
                                     _delayUndershoot / _dispersionUndershoot,
                                     1.0 / _dispersionUndershoot );
    double scale = _ratio - 1.0;
    if ( scale > CANONICAL_HRF_TINY )
    {

      value /= scale;

    }
    return value;

  }
  GKG_CATCH( "double gkg::CanonicalHRF::getValue( double t ) const" );

}


#undef CANONICAL_HRF_TINY
