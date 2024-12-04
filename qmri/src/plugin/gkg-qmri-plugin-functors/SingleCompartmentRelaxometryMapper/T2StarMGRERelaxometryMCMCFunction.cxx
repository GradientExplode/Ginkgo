#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/T2StarMGRERelaxometryMCMCFunction.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCFunctionFactory.h>
#include <gkg-core-exception/Exception.h> 
#include <cmath>




gkg::T2StarMGRERelaxometryMCMCFunction::T2StarMGRERelaxometryMCMCFunction( 
 const std::vector< int32_t >& measurementCounts,
 const std::vector< double >& noiseStandardDeviations,
 const gkg::RCPointer< gkg::AcquisitionParameterSet >& acquisitionParameterSet )
    : gkg::SingleCompartmentRelaxometryMCMCFunction( measurementCounts,
                                                     noiseStandardDeviations ),
      _acquisitionParameterSet(
          static_cast< const gkg::T2StarMGREAcquisitionParameterSet* >( 
                                        acquisitionParameterSet.getPointer() ) )
{
}


gkg::T2StarMGRERelaxometryMCMCFunction::~T2StarMGRERelaxometryMCMCFunction()
{
}


void gkg::T2StarMGRERelaxometryMCMCFunction::getValuesAt(
                      const gkg::Vector& parameters, gkg::Vector& values ) const
{

  try
  {

    int32_t inputVolumeCount = ( int32_t )_measurementCounts.size();
    const std::vector< gkg::Vector >&
      echoTimes = _acquisitionParameterSet->getEchoTimes();

    double protonDensity = parameters( 0 );
    double quantitativeT2Star = parameters( 1 );

    int32_t m = 0;
    int32_t inputVolumeIndex = 0;
    int32_t measurementIndex = 0;
    double scaling = 1.0;
    for ( inputVolumeIndex = 0;
          inputVolumeIndex < inputVolumeCount;
          inputVolumeIndex++ )
    {

      if ( inputVolumeIndex > 0 )
      {

        scaling = parameters( 1 + inputVolumeIndex );

      }

      for ( measurementIndex = 0;
            measurementIndex < _measurementCounts[ inputVolumeIndex ]; 
            measurementIndex++ )
      {
 
        values( m ) =
          scaling * protonDensity *
          std::exp( -echoTimes[ inputVolumeIndex]( measurementIndex ) /
                    quantitativeT2Star );

        ++ m;

      }

    }


  }
  GKG_CATCH( "void gkg::T2StarMGRERelaxometryMCMCFunction::getValuesAt( "
             "const gkg::Vector& parameters, gkg::Vector& values ) const" );

}



std::string gkg::T2StarMGRERelaxometryMCMCFunction::getStaticName()
{

  try
  {

    return "t2-star-mapping-mgre";

  }
  GKG_CATCH( "std::string gkg::T2StarMGRERelaxometryMCMCFunction::"
             "getStaticName()" );

}


RegisterSingleCompartmentRelaxometryMCMCFunction(
  gkg,
  T2StarMGRERelaxometryMCMCFunction );
