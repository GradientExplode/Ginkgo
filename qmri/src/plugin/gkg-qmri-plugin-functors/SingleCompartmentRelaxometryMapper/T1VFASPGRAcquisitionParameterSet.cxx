#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/T1VFASPGRAcquisitionParameterSet.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-core-exception/Exception.h> 


gkg::T1VFASPGRAcquisitionParameterSet::T1VFASPGRAcquisitionParameterSet(
                            const std::vector< int32_t >& measurementCounts,
                            const std::vector< std::string >& stringParameters )
                                      : gkg::AcquisitionParameterSet()
{

  try
  {

    int32_t inputVolumeCount = ( int32_t )measurementCounts.size();

    if ( ( ( int32_t )stringParameters.size() != ( 2 * inputVolumeCount ) ) &&
         ( ( int32_t )stringParameters.size() != ( 2 * inputVolumeCount + 1 ) )
       )
    {

      throw std::runtime_error(
          "bad filename count; should be twice the number of input volume count"
          " plus eventually 1 in case of B1 scaling" );

    }

    if ( ( int32_t )stringParameters.size() == ( 2 * inputVolumeCount + 1 ) )
    {

      _hasB1Mapping = true;
      _b1MappingVolumePointer.reset( new gkg::Volume< float > );
      gkg::TypedVolumeReaderProcess< float >
        b1MappingVolumeReaderProcess( *_b1MappingVolumePointer );
      b1MappingVolumeReaderProcess.execute( stringParameters[ 2 ] );

    }
    else
    {

      _hasB1Mapping = false;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading the repetition time(s)
    //   stringParameters[ 0 ] -> repetition times filename of 1st input volume
    //   ....
    //   stringParameters[ i - 1 ] -> repetition times filename of ith input 
    //                                volume
    //   stringParameters[ i ] -> flip angles filename of 1st input volume
    //   ....
    //   stringParameters[ j - 1 ] -> flip angles filename of ith input 
    //                                volume
    ////////////////////////////////////////////////////////////////////////////

    // first, reading the repetition times files
    _repetitionTimes.resize( inputVolumeCount );

    int32_t inputVolumeIndex = 0;
    for ( inputVolumeIndex = 0;
          inputVolumeIndex < inputVolumeCount;
          inputVolumeIndex++ )
    {

      gkg::AcquisitionParameterSet::readTextFile(
                                        stringParameters[ inputVolumeIndex ],
                                        measurementCounts[ inputVolumeIndex ],
                                        _repetitionTimes[ inputVolumeIndex ] );

    }

    // second, reading the flip angles files
    _flipAngles.resize( inputVolumeCount );

    for ( inputVolumeIndex = 0;
          inputVolumeIndex < inputVolumeCount;
          inputVolumeIndex++ )
    {

      gkg::AcquisitionParameterSet::readTextFile(
                                        stringParameters[ inputVolumeCount +
                                                          inputVolumeIndex ],
                                        measurementCounts[ inputVolumeIndex ],
                                        _flipAngles[ inputVolumeIndex ] );

    }


  }
  GKG_CATCH( "gkg::T1VFASPGRAcquisitionParameterSet::"
             "T1VFASPGRAcquisitionParameterSet( "
             "const std::vector< int32_t >& measurementCounts, "
             "const std::vector< std::string >& stringParameters )" );

}


gkg::T1VFASPGRAcquisitionParameterSet::~T1VFASPGRAcquisitionParameterSet()
{
}


const std::vector< gkg::Vector >& 
gkg::T1VFASPGRAcquisitionParameterSet::getRepetitionTimes() const
{

  try
  {

    return _repetitionTimes;

  }
  GKG_CATCH( "const std::vector< gkg::Vector >& "
             "gkg::T1VFASPGRAcquisitionParameterSet::"
             "getRepetitionTimes() const" );


}


const std::vector< gkg::Vector >& 
gkg::T1VFASPGRAcquisitionParameterSet::getFlipAngles() const
{

  try
  {

    return _flipAngles;

  }
  GKG_CATCH( "const std::vector< gkg::Vector >& "
             "gkg::T1VFASPGRAcquisitionParameterSet::"
             "getFlipAngles() const" );


}


bool gkg::T1VFASPGRAcquisitionParameterSet::hasB1Mapping() const
{

  try
  {

    return _hasB1Mapping;

  }
  GKG_CATCH( "bool "
             "gkg::T1VFASPGRAcquisitionParameterSet::"
             "hasB1Mapping() const" );


}


const gkg::RCPointer< gkg::Volume< float > >& 
gkg::T1VFASPGRAcquisitionParameterSet::getB1MappingVolumePointer() const
{

  try
  {

    return _b1MappingVolumePointer;

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::Volume< float > >& "
             "gkg::T1VFASPGRAcquisitionParameterSet::"
             "getB1MappingVolumePointer() const" );


}


std::string gkg::T1VFASPGRAcquisitionParameterSet::getStaticName()
{

  try
  {

    return "t1-mapping-vfa-spgr";

  }
  GKG_CATCH( "std::string gkg::T1VFASPGRAcquisitionParameterSet::"
             "getStaticName()" );

}


RegisterAcquisitionParameterSet(
  gkg,
  T1VFASPGRAcquisitionParameterSet );
