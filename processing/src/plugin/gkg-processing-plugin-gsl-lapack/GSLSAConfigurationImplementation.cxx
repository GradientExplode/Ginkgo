#include <gkg-processing-plugin-gsl-lapack/GSLSAConfigurationImplementation.h>
#include <gkg-processing-numericalanalysis/SimulatedAnnealingConfiguration.h>
#include <gkg-core-exception/Exception.h>


gkg::GSLSAConfigurationImplementation::GSLSAConfigurationImplementation(
          gkg::SimulatedAnnealingConfiguration* simulatedAnnealingConfiguration,
           bool verbose )
                                      : _simulatedAnnealingConfiguration(
                                               simulatedAnnealingConfiguration )
{

  try
  {

    _simanEFunc = &gkg::GSLSAConfigurationImplementation::simanEFunc;
    _simanStep = &gkg::GSLSAConfigurationImplementation::simanStep;
    _simanMetric = &gkg::GSLSAConfigurationImplementation::simanMetric;
    if ( verbose )
    {

      _simanPrint = &gkg::GSLSAConfigurationImplementation::simanPrint;

    }
    else
    {

      _simanPrint = &gkg::GSLSAConfigurationImplementation::simanNoPrint;

    }
    _simanCopy = &gkg::GSLSAConfigurationImplementation::simanCopy;
    _simanCopyConstruct =
                        &gkg::GSLSAConfigurationImplementation::simanCopyConstruct;
    _simanDestroy = &gkg::GSLSAConfigurationImplementation::simanDestroy;

  }
  GKG_CATCH( "gkg::GSLSAConfigurationImplementation::"
             "GSLSAConfigurationImplementation( "
             " gkg::SimulatedAnnealingConfiguration* "
             "simulatedAnnealingConfiguration )" );

}


gkg::GSLSAConfigurationImplementation::~GSLSAConfigurationImplementation()
{


}


gkg::SimulatedAnnealingConfiguration*
gkg::GSLSAConfigurationImplementation::
                                      getSimulatedAnnealingConfiguration() const
{

  return _simulatedAnnealingConfiguration;

}


gsl_siman_Efunc_t 
gkg::GSLSAConfigurationImplementation::getSimanEFuncPointer()
{

  return _simanEFunc;

}


gsl_siman_step_t
gkg::GSLSAConfigurationImplementation::getSimanStepPointer()
{

  return _simanStep;

}


gsl_siman_metric_t
gkg::GSLSAConfigurationImplementation::getSimanMetricPointer()
{

  return _simanMetric;

}


gsl_siman_print_t
gkg::GSLSAConfigurationImplementation::getSimanPrintPointer()
{

  return _simanPrint;

}


gsl_siman_copy_t
gkg::GSLSAConfigurationImplementation::getSimanCopyPointer()
{

  return _simanCopy;

}


gsl_siman_copy_construct_t
gkg::GSLSAConfigurationImplementation::getSimanCopyConstructPointer()
{

  return _simanCopyConstruct;

}


gsl_siman_destroy_t
gkg::GSLSAConfigurationImplementation::getSimanDestroyPointer()
{

  return _simanDestroy;

}


double gkg::GSLSAConfigurationImplementation::simanEFunc( void* c )
{

  try
  {

    gkg::SimulatedAnnealingConfiguration* simulatedAnnealingConfiguration =
      static_cast< gkg::SimulatedAnnealingConfiguration* >( c );

    return simulatedAnnealingConfiguration->getEnergy();

  }
  GKG_CATCH( "double gkg::GSLSAFunctionImplementation::simanEFunc( "
             "void* c )" );

}


void gkg::GSLSAConfigurationImplementation::simanStep( const gsl_rng* r,
                                                       void* c,
                                                       double stepSize )
{

  try
  {

    gkg::SimulatedAnnealingConfiguration* simulatedAnnealingConfiguration =
      static_cast< gkg::SimulatedAnnealingConfiguration* >( c );

    double u = gsl_rng_uniform( r );

    simulatedAnnealingConfiguration->step( u, stepSize );

  }
  GKG_CATCH( "void gkg::GSLSAFunctionImplementation::simanStep( "
             "const gsl_rng* r, "
             "void* c, "
             "double stepSize )" );

}


double gkg::GSLSAConfigurationImplementation::simanMetric( void* c1, void* c2 )
{

  try
  {

    gkg::SimulatedAnnealingConfiguration* simulatedAnnealingConfiguration1 = 
      static_cast< gkg::SimulatedAnnealingConfiguration* >( c1 );

    gkg::SimulatedAnnealingConfiguration* simulatedAnnealingConfiguration2 = 
      static_cast< gkg::SimulatedAnnealingConfiguration* >( c2 );
    return simulatedAnnealingConfiguration1->getDistance(
                                            *simulatedAnnealingConfiguration2 );

  }
  GKG_CATCH( "double gkg::GSLSAConfigurationImplementation::simanMetric( "
             "void* c2, void* c1 )" );

}


void gkg::GSLSAConfigurationImplementation::simanPrint( void* c )
{

  try
  {

    gkg::SimulatedAnnealingConfiguration* simulatedAnnealingConfiguration = 
      static_cast< gkg::SimulatedAnnealingConfiguration* >( c );

    simulatedAnnealingConfiguration->display();

  }
  GKG_CATCH( "void gkg::GSLSAConfigurationImplementation::simanPrint( "
             "void* c )" );

}


void gkg::GSLSAConfigurationImplementation::simanNoPrint( void* /*c*/ )
{
}


void gkg::GSLSAConfigurationImplementation::simanCopy( void* source,
                                                       void* destination )
{

  try
  {

    gkg::SimulatedAnnealingConfiguration*
      simulatedAnnealingConfigurationSource = 
        static_cast< gkg::SimulatedAnnealingConfiguration* >( source );

    gkg::SimulatedAnnealingConfiguration*
      simulatedAnnealingConfigurationDestination =
        static_cast< gkg::SimulatedAnnealingConfiguration* >( destination );

    simulatedAnnealingConfigurationDestination->copy(
                                       *simulatedAnnealingConfigurationSource );

  }
  GKG_CATCH( "void gkg::GSLSAConfigurationImplementation::simanCopy( "
             "void* source, void* destination )" );

}


void* gkg::GSLSAConfigurationImplementation::simanCopyConstruct( void* c )
{

  try
  {

    gkg::SimulatedAnnealingConfiguration* simulatedAnnealingConfiguration = 
      static_cast< gkg::SimulatedAnnealingConfiguration* >( c );

    return ( void* )simulatedAnnealingConfiguration->copyConstruct();

  }
  GKG_CATCH( "void* gkg::GSLSAConfigurationImplementation::simanCopyConstruct( "
             "void* c )" );

}


void gkg::GSLSAConfigurationImplementation::simanDestroy( void* c )
{

  try
  {

    gkg::SimulatedAnnealingConfiguration* simulatedAnnealingConfiguration = 
      static_cast< gkg::SimulatedAnnealingConfiguration* >( c );

    delete simulatedAnnealingConfiguration;

  }
  GKG_CATCH( "void gkg::GSLSAConfigurationImplementation::simanDestroy( "
             "void* c )" );

}
