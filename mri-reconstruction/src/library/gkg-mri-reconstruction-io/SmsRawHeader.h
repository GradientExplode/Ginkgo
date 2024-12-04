#ifndef _gkg_mri_reconstructionio_SmsRawHeader_h_
#define _gkg_mri_reconstructionio_SmsRawHeader_h_


#include <gkg-core-cppext/StdInt.h>
#include <iostream>


#define MDH_NUMBEROFEVALINFOMASK   2
#define MDH_NUMBEROFICEPROGRAMPARA 4
#define MDH_FREEHDRPARA            4


namespace gkg
{


//
// definition of loop counter structure
//

struct LoopCounter
{

  uint16_t ushLine;        // line index
  uint16_t ushAcquisition; // acquisition index
  uint16_t ushSlice;       // slice index
  uint16_t ushPartition;   // partition index
  uint16_t ushEcho;        // echo index                   
  uint16_t ushPhase;       // phase index                
  uint16_t ushRepetition;  // measurement repeat index
  uint16_t ushSet;         // set index
  uint16_t ushSeg;         // segment index  (for TSE)
  uint16_t ushIda;         // IceDimension a index
  uint16_t ushIdb;         // IceDimension b index
  uint16_t ushIdc;         // IceDimension c index
  uint16_t ushIdd;         // IceDimension d index
  uint16_t ushIde;         // IceDimension e index

};


//
// definition of slice vectors
//

struct SliceVector
{

  float flSag;
  float flCor;
  float flTra;

};


//
// definition or slice data
//

struct SliceData
{

  SliceVector sSlicePosVec; // slice position vector       
  float aflQuaternion[ 4 ]; // rotation matrix as quaternion

};


//
// definition of cut-off data
//

struct CutOffData
{

  uint16_t ushPre;   // write ushPre zeros at line start 
  uint16_t ushPost;  // write ushPost zeros at line end  

};


//
// definition of measurement data header (128 bytes in total)
//

struct Mdh
{

  uint32_t ulFlagsAndDMALength;        // bit  0..24: DMA length [bytes]
                                       // bit     25: pack bit
                                       // bit 26..31: pci_rx enable flags
  int32_t lMeasUID;                    // measurement user ID
  uint32_t ulScanCounter;              // scan counter [1...]
  uint32_t ulTimeStamp;                // time stamp [2.5 ms ticks since 00:00]
  uint32_t ulPMUTimeStamp;             // PMU time stamp [2.5 ms ticks since 
                                       //                 lasttrigger]
  uint32_t aulEvalInfoMask[ MDH_NUMBEROFEVALINFOMASK ];
                                       // evaluation info mask field
  uint16_t ushSamplesInScan;           // number of samples acquired in scan
  uint16_t ushUsedChannels;            // number of channels used in scan
  LoopCounter sLoopCounter;            // loop counters
  CutOffData sCutOff;                  // cut-off values
  uint16_t ushKSpaceCentreColumn;      // centre of echo
  uint16_t ushCoilSelect;              // bit 0..3: CoilSelect
  float fReadOutOffcentre;             // readOut offcenter value                           
  uint32_t ulTimeSinceLastRF;          // sequence time stamp since last RF
                                       // pulse
  uint16_t ushKSpaceCentreLineNo;      // number of K-space centre line
  uint16_t ushKSpaceCentrePartitionNo; // number of K-space centre partition
  uint16_t aushIceProgramPara[ MDH_NUMBEROFICEPROGRAMPARA ]; 
                                       // free parameter for IceProgram   
  uint16_t aushFreePara[ MDH_FREEHDRPARA ]; // free parameter                          

  SliceData sSliceData;                // slice Data
  uint16_t ushChannelId;               // channel Id must be the last parameter
  uint16_t ushPTABPosNeg;              // negative, absolute PTAB position in 
                                       // [0.1 mm]      

};

   
}


namespace std
{

ostream& operator<<( ostream& os,
                     const gkg::Mdh& thing );

}


#endif
