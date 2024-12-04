#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/T1IRSERelaxometryMCMCFunction.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCFunctionFactory.h>
#include <gkg-core-exception/Exception.h> 
#include <cmath>




gkg::T1IRSERelaxometryMCMCFunction::T1IRSERelaxometryMCMCFunction( 
 const std::vector< int32_t >& measurementCounts,
 const std::vector< double >& noiseStandardDeviations,
 const gkg::RCPointer< gkg::AcquisitionParameterSet >& acquisitionParameterSet )
    : gkg::SingleCompartmentRelaxometryMCMCFunction( measurementCounts,
                                                     noiseStandardDeviations ),
      _acquisitionParameterSet(
          static_cast< const gkg::T1IRSEAcquisitionParameterSet* >( 
                                        acquisitionParameterSet.getPointer() ) )
{
}


gkg::T1IRSERelaxometryMCMCFunction::~T1IRSERelaxometryMCMCFunction()
{
}


void gkg::T1IRSERelaxometryMCMCFunction::getValuesAt(
                      const gkg::Vector& parameters, gkg::Vector& values ) const
{

  try
  {

    int32_t inputVolumeCount = ( int32_t )_measurementCounts.size();
    const std::vector< gkg::Vector >&
      inversionTimes = _acquisitionParameterSet->getInversionTimes();

    double protonDensity = parameters( 0 );
    double quantitativeT1 = parameters( 1 );

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
          std::abs( 
            scaling * protonDensity *
            ( 1.0 - 2 *
              std::exp( -inversionTimes[ inputVolumeIndex]( measurementIndex ) /
                        quantitativeT1 ) ) );


        ++ m;

      }

    }


  }
  GKG_CATCH( "void gkg::T1IRSERelaxometryMCMCFunction::getValuesAt( "
             "const gkg::Vector& parameters, gkg::Vector& values ) const" );

}



std::string gkg::T1IRSERelaxometryMCMCFunction::getStaticName()
{

  try
  {

    return "t1-mapping-irse";

  }
  GKG_CATCH( "std::string gkg::T1IRSERelaxometryMCMCFunction::"
             "getStaticName()" );

}


RegisterSingleCompartmentRelaxometryMCMCFunction(
  gkg,
  T1IRSERelaxometryMCMCFunction );

