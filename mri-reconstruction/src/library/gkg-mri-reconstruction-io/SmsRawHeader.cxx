#include <gkg-mri-reconstruction-io/SmsRawHeader.h>
#include <gkg-core-exception/Exception.h>


std::ostream& std::operator<<( std::ostream& os,
                               const gkg::Mdh& thing )
{

  try
  {

    os << "(   ulFlagsAndDMALength : " << thing.ulFlagsAndDMALength << ", "
       << std::endl
       << "    lMeasUID : " << thing.lMeasUID << ", "
       << std::endl
       << "    ulScanCounter : " << thing.ulScanCounter << ", "
       << std::endl
       << "    ulTimeStamp : " << thing.ulTimeStamp << ", "
       << std::endl
       << "    ulPMUTimeStamp : " << thing.ulPMUTimeStamp << ", "
       << std::endl
       << "    aulEvalInfoMask : ( " << std::flush;

    int32_t i = 0;
    for ( i = 0; i < MDH_NUMBEROFEVALINFOMASK - 1; i++ )
    {

      os << thing.aulEvalInfoMask[ i ] << ", ";

    }
    os << thing.aulEvalInfoMask[ i ] << " )"
       << ", "
       << std::endl
       << "    ushSamplesInScan : " << thing.ushSamplesInScan << ", "
       << std::endl
       << "    ushUsedChannels : " << thing.ushUsedChannels << ", "
       << std::endl
       << "    sLoopCounter : ( ushLine : "
       << thing.sLoopCounter.ushLine << ", "
       << std::endl
       << "                     ushAcquisition : "
       << thing.sLoopCounter.ushAcquisition << ", "
       << std::endl
       << "                     ushSlice : "
       << thing.sLoopCounter.ushSlice << ", "
       << std::endl
       << "                     ushPartition : "
       << thing.sLoopCounter.ushPartition << ", "
       << std::endl
       << "                     ushEcho : "
       << thing.sLoopCounter.ushEcho << ", "
       << std::endl
       << "                     ushPhase : "
       << thing.sLoopCounter.ushPhase << ", "
       << std::endl
       << "                     ushRepetition : "
       << thing.sLoopCounter.ushRepetition << ", "
       << std::endl
       << "                     ushSet : "
       << thing.sLoopCounter.ushSet << ", "
       << std::endl
       << "                     ushSeg : "
       << thing.sLoopCounter.ushSeg << ", "
       << std::endl
       << "                     ushIda : "
       << thing.sLoopCounter.ushIda << ", "
       << std::endl
       << "                     ushIdb : "
       << thing.sLoopCounter.ushIdb << ", "
       << std::endl
       << "                     ushIdc : "
       << thing.sLoopCounter.ushIdc << ", "
       << std::endl
       << "                     ushIdd : "
       << thing.sLoopCounter.ushIdd << ", "
       << std::endl
       << "                     ushIde : "
       << thing.sLoopCounter.ushIde << " ) , "
       << std::endl
       << "    sCutOff : ( ushPre : "
       << thing.sCutOff.ushPre << ", "
       << std::endl
       << "                ushPost : "
       << thing.sCutOff.ushPre << ") , "
       << std::endl
       << "    ushKSpaceCentreColumn : " << thing.ushKSpaceCentreColumn << ", "
       << std::endl
       << "    ushCoilSelect : " << thing.ushCoilSelect << ", "
       << std::endl
       << "    fReadOutOffcentre : " << thing.fReadOutOffcentre << ", "
       << std::endl
       << "    ulTimeSinceLastRF : " << thing.ulTimeSinceLastRF << ", "
       << std::endl
       << "    ushKSpaceCentreLineNo : " << thing.ushKSpaceCentreLineNo << ", "
       << std::endl
       << "    ushKSpaceCentrePartitionNo : "
       << thing.ushKSpaceCentrePartitionNo << ", "
       << std::endl
       << "    aushIceProgramPara : ( " << std::flush;

    for ( i = 0; i < MDH_NUMBEROFICEPROGRAMPARA - 1; i++ )
    {

      os << thing.aushIceProgramPara[ i ] << ", ";

    }
    os << thing.aushIceProgramPara[ i ] << " )"
       << ", "
       << std::endl
       << "    aushFreePara : ( " << std::flush;

    for ( i = 0; i < MDH_FREEHDRPARA - 1; i++ )
    {

      os << thing.aushFreePara[ i ] << ", ";

    }
    os << thing.aushFreePara[ i ] << " )"
       << ", "
       << std::endl;

    os << "    sSliceData : ( sSlicePosVec : ( flSag : "
       << thing.sSliceData.sSlicePosVec.flSag << ", "
       << std::endl
       << "                                    flCor : "
       << thing.sSliceData.sSlicePosVec.flCor << ", "
       << std::endl
       << "                                    flTra : "
       << thing.sSliceData.sSlicePosVec.flTra << " ) , "
       << std::endl
       << "                   aflQuaternion : ( ";
    for ( i = 0; i < 3; i++ )
    {

      os << thing.sSliceData.aflQuaternion[ i ] << ", ";

    }
    os << thing.sSliceData.aflQuaternion[ 3 ] << " ), "
       << std::endl
       << "    ushChannelId : " << thing.ushChannelId << ", "
       << std::endl
       << "    ushPTABPosNeg : " << thing.ushPTABPosNeg << " ) "
       << std::flush;

    return os;

  }
  GKG_CATCH( "std::ostream& std::operator<<( "
             "std::ostream& os, "
             "const gkg::Mdh& thing )" );

}
