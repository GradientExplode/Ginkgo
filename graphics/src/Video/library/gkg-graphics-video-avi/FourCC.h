#ifndef _gkg_graphics_video_avi_FourCC_h_
#define _gkg_graphics_video_avi_FourCC_h_


#ifndef mmioFOURCC
#define mmioFOURCC( ch0, ch1, ch2, ch3 ) \
    ( (int)(unsigned char)(ch0) | ( (int)(unsigned char)(ch1) << 8 ) | \
    ( (int)(unsigned char)(ch2) << 16 ) | ( (int)(unsigned char)(ch3) << 24 ) )
#endif

#ifndef aviTWOCC
#define aviTWOCC( ch0, ch1 ) \
    ( (unsigned short)(unsigned char)(ch0) | \
    ( (unsigned short)(unsigned char)(ch1) << 8 ) )
#endif

// form types, list types, and chunk types
#define formtypeAVI             mmioFOURCC('A', 'V', 'I', ' ')

#define listtypeAVIHEADER       mmioFOURCC('h', 'd', 'r', 'l')
#define listtypeSTREAMHEADER    mmioFOURCC('s', 't', 'r', 'l')
#define listtypeAVIMOVIE	mmioFOURCC('m', 'o', 'v', 'i')
#define listtypeAVIRECORD	mmioFOURCC('r', 'e', 'c', ' ')
#define listtypeINFO		mmioFOURCC('I', 'N', 'F', 'O')

#define ckidAVIMAINHDR          mmioFOURCC('a', 'v', 'i', 'h')
#define ckidSTREAMHEADER        mmioFOURCC('s', 't', 'r', 'h')
#define ckidSTREAMFORMAT        mmioFOURCC('s', 't', 'r', 'f')
#define ckidSTREAMHANDLERDATA   mmioFOURCC('s', 't', 'r', 'd')
#define ckidSTREAMNAME		mmioFOURCC('s', 't', 'r', 'n')
#define ckidAVINEWINDEX		mmioFOURCC('i', 'd', 'x', '1')

// Chunk id to use for extra chunks for padding
#define ckidAVIPADDING		mmioFOURCC('J', 'U', 'N', 'K')

// Stream types for the <fccType> field of the stream header
#define streamtypeVIDEO		mmioFOURCC('v', 'i', 'd', 's')
#define streamtypeAUDIO		mmioFOURCC('a', 'u', 'd', 's')
#define streamtypeMIDI		mmioFOURCC('m', 'i', 'd', 's')
#define streamtypeTEXT		mmioFOURCC('t', 'x', 't', 's')

// Basic chunk types
#define cktypeDIBbits		aviTWOCC('d', 'b')
#define cktypeDIBcompressed	aviTWOCC('d', 'c')
#define cktypePALchange		aviTWOCC('p', 'c')
#define cktypeWAVEbytes		aviTWOCC('w', 'b')
#define cktypeINDEX		aviTWOCC('i', 'x')

// Main avi four_cc
#define FOURCC_RIFF	mmioFOURCC('R', 'I', 'F', 'F')
#define FOURCC_LIST	mmioFOURCC('L', 'I', 'S', 'T')

// Macro to make a ckid for a chunk out of a TWOCC and a stream number
// from 0-255.
#define MAKELONG(low,high) \
	 ((int32_t)(((uint16_t)(low)) | (((uint32_t)((uint16_t)(high))) << 16)))
#define ToHex(n) ((uint8_t)(((n) > 9) ? ((n) - 10 + 'A') : ((n) + '0')))
#define MAKEAVICKID(tcc, stream) \
  MAKELONG((ToHex((stream) & 0x0f) << 8) | (ToHex(((stream) & 0xf0) >> 4)), tcc)


#endif
