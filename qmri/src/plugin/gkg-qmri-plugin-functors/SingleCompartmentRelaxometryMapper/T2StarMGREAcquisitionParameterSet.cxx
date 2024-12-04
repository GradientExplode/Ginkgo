#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/T2StarMGREAcquisitionParameterSet.h>
#include <gkg-core-exception/Exception.h> 


gkg::T2StarMGREAcquisitionParameterSet::T2StarMGREAcquisitionParameterSet(
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
  GKG_CATCH( "gkg::T2StarMGREAcquisitionParameterSet::"
             "T2StarMGREAcquisitionParameterSet( "
             "const std::vector< int32_t >& measurementCounts, "
             "const std::vector< std::string >& stringParameters )" );

}


gkg::T2StarMGREAcquisitionParameterSet::~T2StarMGREAcquisitionParameterSet()
{
}


const std::vector< gkg::Vector >& 
gkg::T2StarMGREAcquisitionParameterSet::getEchoTimes() const
{

  try
  {

    return _echoTimes;

  }
  GKG_CATCH( "const std::vector< gkg::Vector >& "
             "gkg::T2StarMGREAcquisitionParameterSet::getEchoTimes() const" );


}


std::string gkg::T2StarMGREAcquisitionParameterSet::getStaticName()
{

  try
  {

    return "t2-star-mapping-mgre";

  }
  GKG_CATCH( "std::string gkg::T2StarMGREAcquisitionParameterSet::"
             "getStaticName()" );

}


RegisterAcquisitionParameterSet(
  gkg,
  T2StarMGREAcquisitionParameterSet );
