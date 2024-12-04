#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/T2MSMEAcquisitionParameterSet.h>
#include <gkg-core-exception/Exception.h> 


gkg::T2MSMEAcquisitionParameterSet::T2MSMEAcquisitionParameterSet(
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
                "echo times filename count does not match input volume count" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading the echo time(s)
    //   stringParameters[ 0 ] -> echo times filename of 1st input volume
    //   ....
    //   stringParameters[ i - 1 ] -> echo times filename of ith input volume
    ////////////////////////////////////////////////////////////////////////////
    _echoTimes.resize( inputVolumeCount );

    int32_t inputVolumeIndex = 0;
    for ( inputVolumeIndex = 0;
          inputVolumeIndex < inputVolumeCount;
          inputVolumeIndex++ )
    {

      gkg::AcquisitionParameterSet::readTextFile(
                                        stringParameters[ inputVolumeIndex ],
                                        measurementCounts[ inputVolumeIndex ],
                                        _echoTimes[ inputVolumeIndex ] );

    }

  }
  GKG_CATCH( "gkg::T2MSMEAcquisitionParameterSet::"
             "T2MSMEAcquisitionParameterSet( "
             "const std::vector< int32_t >& measurementCounts, "
             "const std::vector< std::string >& stringParameters )" );

}


gkg::T2MSMEAcquisitionParameterSet::~T2MSMEAcquisitionParameterSet()
{
}


const std::vector< gkg::Vector >& 
gkg::T2MSMEAcquisitionParameterSet::getEchoTimes() const
{

  try
  {

    return _echoTimes;

  }
  GKG_CATCH( "const std::vector< gkg::Vector >& "
             "gkg::T2MSMEAcquisitionParameterSet::getEchoTimes() const" );


}


std::string gkg::T2MSMEAcquisitionParameterSet::getStaticName()
{

  try
  {

    return "t2-mapping-msme";

  }
  GKG_CATCH( "std::string gkg::T2MSMEAcquisitionParameterSet::"
             "getStaticName()" );

}


RegisterAcquisitionParameterSet(
  gkg,
  T2MSMEAcquisitionParameterSet );
