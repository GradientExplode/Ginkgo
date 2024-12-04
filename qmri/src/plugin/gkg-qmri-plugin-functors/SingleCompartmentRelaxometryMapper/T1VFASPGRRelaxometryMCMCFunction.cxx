#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/T1VFASPGRRelaxometryMCMCFunction.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCFunctionFactory.h>
#include <gkg-core-exception/Exception.h> 
#include <cmath>




gkg::T1VFASPGRRelaxometryMCMCFunction::T1VFASPGRRelaxometryMCMCFunction( 
 const std::vector< int32_t >& measurementCounts,
 const std::vector< double >& noiseStandardDeviations,
 const gkg::RCPointer< gkg::AcquisitionParameterSet >& acquisitionParameterSet )
    : gkg::SingleCompartmentRelaxometryMCMCFunction( measurementCounts,
                                                     noiseStandardDeviations ),
      _acquisitionParameterSet(
          static_cast< const gkg::T1VFASPGRAcquisitionParameterSet* >( 
                                        acquisitionParameterSet.getPointer() ) )
{
}


gkg::T1VFASPGRRelaxometryMCMCFunction::~T1VFASPGRRelaxometryMCMCFunction()
{
}


void gkg::T1VFASPGRRelaxometryMCMCFunction::getValuesAt(
                      const gkg::Vector& parameters, gkg::Vector& values ) const
{

  try
  {

    int32_t inputVolumeCount = ( int32_t )_measurementCounts.size();
    const std::vector< gkg::Vector >&
      repetitionTimes = _acquisitionParameterSet->getRepetitionTimes();
    const std::vector< gkg::Vector >&
      flipAngles = _acquisitionParameterSet->getFlipAngles();
    bool hasB1Mapping = _acquisitionParameterSet->hasB1Mapping();
    const gkg::RCPointer< gkg::Volume< float > >&
      b1MappingVolumePointer = 
                          _acquisitionParameterSet->getB1MappingVolumePointer();

    double b1Scaling = M_PI / 180.0;
    if ( hasB1Mapping )
    {

      b1Scaling = ( ( *b1MappingVolumePointer )( _site ) / 900.0 ) *
                  M_PI / 180.0;

    }

    double protonDensity = parameters( 0 );
    double quantitativeT1 = parameters( 1 );

    int32_t m = 0;
    int32_t inputVolumeIndex = 0;
    int32_t measurementIndex = 0;
    double scaling = 1.0;
    double expMinusTROverT1 = 0.0;
    double correctedFlipAngle = 0.0;
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

        expMinusTROverT1 = 
          std::exp( -repetitionTimes[ inputVolumeIndex ]( measurementIndex ) /
                    quantitativeT1 );
        correctedFlipAngle = flipAngles[ inputVolumeIndex ]( measurementIndex )
                             * b1Scaling;
 
        values( m ) = scaling * protonDensity * 
                      ( ( 1.0 - expMinusTROverT1 ) * 
                        std::sin( correctedFlipAngle ) ) /
                      ( 1.0 - expMinusTROverT1 *
                              std::cos( correctedFlipAngle ) );
        ++ m;

      }

    }

  }
  GKG_CATCH( "void gkg::T1VFASPGRRelaxometryMCMCFunction::getValuesAt( "
             "const gkg::Vector& parameters, gkg::Vector& values ) const" );

}



std::string gkg::T1VFASPGRRelaxometryMCMCFunction::getStaticName()
{

  try
  {

    return "t1-mapping-vfa-spgr";

  }
  GKG_CATCH( "std::string gkg::T1VFASPGRRelaxometryMCMCFunction::"
             "getStaticName()" );

}


RegisterSingleCompartmentRelaxometryMCMCFunction(
  gkg,
  T1VFASPGRRelaxometryMCMCFunction );
