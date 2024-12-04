#ifndef _gkg_graphics_video_avi_AviIndex_h_
#define _gkg_graphics_video_avi_AviIndex_h_


#include <gkg-core-cppext/StdInt.h>


/* Flags for index */
#define AVIIF_LIST     0x00000001L // chunk is a 'LIST'
#define AVIIF_KEYFRAME 0x00000010L // this frame is a key frame.

#define AVIIF_NOTIME  0x00000100L // this frame doesn't take any time
#define AVIIF_COMPUSE 0x0FFF0000L // these bits are for compressor use

#define AVI_INDEX_OF_INDEXES 0x00
#define AVI_INDEX_OF_CHUNKS  0x01
#define AVI_INDEX_IS_DATA    0x80
#define AVI_INDEX_2FIELD     0x01


namespace gkg
{


struct AviIndexEntry
{

  uint32_t ckid;
  uint32_t dwFlags;
  uint32_t dwChunkOffset;
  uint32_t dwChunkLength;

};


struct AviSuperIndexEntry
{

  uint64_t qwOffset;
  uint32_t dwSize; 
  uint32_t dwDuration;

};


struct AviSuperIndex
{

  uint32_t fccType;
  uint32_t cb;
  uint16_t wLongsPerEntry;
  uint8_t  bIndexSubType;
  uint8_t  bIndexType;
  uint32_t nEntriesInUse;
  uint32_t dwChunkId;
  uint32_t dwReserved[ 3 ];

  AviSuperIndexEntry* aIndex;

  uint32_t dwChunkOffset;
  uint32_t dwChunkLength;

};


struct AviStdIndexEntry
{

  int32_t dwOffset;
  int32_t dwSize; 

};


struct AviStdIndex
{

  uint32_t fcc; 	    
  uint32_t cb;  	    
  uint16_t wLongsPerEntry;  
  uint8_t  bIndexSubType;   
  uint8_t  bIndexType;      
  uint32_t nEntriesInUse;   
  uint32_t dwChunkId;	    
  uint64_t qwBaseOffset;    
  uint64_t dwReserved3;     
  AviStdIndexEntry* aIndex;

};


}


#endif
