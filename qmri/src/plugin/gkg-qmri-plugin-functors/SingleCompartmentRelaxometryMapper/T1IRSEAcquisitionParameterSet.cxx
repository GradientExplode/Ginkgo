#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/T1IRSEAcquisitionParameterSet.h>
#include <gkg-core-exception/Exception.h> 


gkg::T1IRSEAcquisitionParameterSet::T1IRSEAcquisitionParameterSet(
                            const std::vector< int32_t >& measurementCounts,
                            const std::vector< std::string >& stringParameters )
                                   : gkg::AcquisitionParameterSet()
{

  try
  {


    int32_t inputVolumeCount = ( int32_t )measurementCounts.size();

    if ( ( int32_t )stringParameters.size() != inputVolumeCount )
    {

      throw std::runtime_error(
           "inversion times filename count does not match input volume count" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading the inversion time(s)
    //   stringParameters[ 0 ] -> inversion times filename of 1st input volume
    //   ....
    //   stringParameters[ i - 1 ] -> inversion times filename of ith input 
    //                                volume
    ////////////////////////////////////////////////////////////////////////////
    _inversionTimes.resize( inputVolumeCount );

    int32_t inputVolumeIndex = 0;
    for ( inputVolumeIndex = 0;
          inputVolumeIndex < inputVolumeCount;
          inputVolumeIndex++ )
    {

      gkg::AcquisitionParameterSet::readTextFile(
                                        stringParameters[ inputVolumeIndex ],
                                        measurementCounts[ inputVolumeIndex ],
                                        _inversionTimes[ inputVolumeIndex ] );

    }

  }
  GKG_CATCH( "gkg::T1IRSEAcquisitionParameterSet::"
             "T1IRSEAcquisitionParameterSet( "
             "const std::vector< int32_t >& measurementCounts, "
             "const std::vector< std::string >& stringParameters )" );

}


gkg::T1IRSEAcquisitionParameterSet::~T1IRSEAcquisitionParameterSet()
{
}


const std::vector< gkg::Vector >& 
gkg::T1IRSEAcquisitionParameterSet::getInversionTimes() const
{

  try
  {

    return _inversionTimes;

  }
  GKG_CATCH( "const std::vector< gkg::Vector >& "
             "gkg::T1IRSEAcquisitionParameterSet::getInversionTimes() const" );


}


std::string gkg::T1IRSEAcquisitionParameterSet::getStaticName()
{

  try
  {

    return "t1-mapping-irse";

  }
  GKG_CATCH( "std::string gkg::T1IRSEAcquisitionParameterSet::"
             "getStaticName()" );

}


RegisterAcquisitionParameterSet(
  gkg,
  T1IRSEAcquisitionParameterSet );

