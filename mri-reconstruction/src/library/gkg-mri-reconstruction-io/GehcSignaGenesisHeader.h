#ifndef _gkg_mri_reconstruction_io_GehcSignaGenesisHeader_h_
#define _gkg_mri_reconstruction_io_GehcSignaGenesisHeader_h_


#include <gkg-mri-reconstruction-io/GehcSignaPFileHeader.h>



namespace gkg
{


struct SUITEDATATYPE
{

  BLOCK    su_id [ 4 ];          // Suite ID
  int16_t  su_uniq;              // Make Unique Flag
  char     su_diskid;            // Disk ID
  char     prodid [ 13 ];        // Product ID
  BLOCK    su_verscre [ 2 ];     // Genesis Version of Record
  BLOCK    su_verscur [ 2 ];     // Genesis Version of Record
  uint32_t su_checksum;          // Suite Record Checksum

  BLOCK    su_padding [ 85 ];    // Spare Space

};


struct PixHdr
{

  int32_t  img_magic;            // magic number
  int32_t  img_hdr_length;       // length of total header in bytes and 
                                 // a byte displacement to the 'pixel data area'
  int32_t  img_width;            // width (pixels) of image
  int32_t  img_height;           // height (pixels) of image
  int32_t  img_depth;            // depth (1, 8, 16, or 24 bits) of pixel
  int32_t  img_compress;         // type of compression; see IC_* below
  int32_t  img_dwindow;          // default window setting
  int32_t  img_dlevel;           // default level setting
  int32_t  img_bgshade;          // background shade to use for non-image
  int32_t  img_ovrflow;          // overflow value
  int32_t  img_undflow;          // underflow value
  int32_t  img_top_offset;       // number of blank lines at image top
  int32_t  img_bot_offset;       // number of blank lines at image bottom
  int16_t  img_version;          // version of the header structure 
                                 // and a word to maintain 32 bit alignment
  uint16_t img_checksum;         // 16 bit end_around_carry sum of pixels
  int32_t  img_p_id;             // a byte disp to unique image identifier
  int32_t  img_l_id;             // byte length of unique image identifier
  int32_t  img_p_unpack;         // a byte disp to 'unpack control'
  int32_t  img_l_unpack;         // byte length of 'unpack control'
  int32_t  img_p_compress;       // a byte disp to 'compression control'
  int32_t  img_l_compress;       // byte length of 'compression control'
  int32_t  img_p_histo;          // a byte disp to 'histogram control'
  int32_t  img_l_histo;          // byte length of 'histogram control'
  int32_t  img_p_text;           // a byte disp to 'text plane data'
  int32_t  img_l_text;           // byte length of 'text plane data'
  int32_t  img_p_graphics;       // a byte disp to 'graphics plane data'
  int32_t  img_l_graphics;       // byte length of 'graphics plane data'
  int32_t  img_p_dbHdr;          // a byte disp to 'data base header data'
  int32_t  img_l_dbHdr;          // byte length of 'data base header data'
  int32_t  img_levelOffset;      // value to add to stored Pixel Data values
                                 // to get the correct presentation value
  int32_t  img_p_user;           // byte displacement to user defined data
  int32_t  img_l_user;           // byte length of user defined data

};


struct GENESIS_HEADER
{

  SUITEDATATYPE rdb_hdr_suite;              //   116 bytes
  EXAMDATATYPE rdb_hdr_exam;                //  1040 bytes
  SERIESDATATYPE rdb_hdr_series;            //  1028 bytes
  MRIMAGEDATATYPE rdb_hdr_image;            //  1044 bytes
  PixHdr rdb_hdr_pixhdr;                    //   124 bytes

};


}


//
//  POOL_HEADER size(s) in byte(s)
//                      +--------------+--------------+
//                      | revision 8.x | revision 9.x |
//  +-------------------+--------------+--------------+
//  |                   |              |              |
//  | SUITEDATATYPE     |       116    |       116    |
//  | EXAMDATATYPE      |      1040    |      1040    |
//  | SERIESDATATYPE    |      1028    |      1028    |
//  | MRIMAGEDATATYPE   |      1044    |      1044    |
//  | PixHdr            |       124    |       124    |
//  |                   |              |              |
//  +-------------------+--------------+--------------+
//                      |      3352    |      3352    |
//                      +--------------+--------------+
//


//
// useful size define macros
//

// revision 8.x

#define GEHC_SIGNA_GENESIS_SUITEDATATYPE_SIZE_REV_8_x         116U
#define GEHC_SIGNA_GENESIS_EXAMDATATYPE_SIZE_REV_8_x         1040U
#define GEHC_SIGNA_GENESIS_SERIESDATATYPE_SIZE_REV_8_x       1028U
#define GEHC_SIGNA_GENESIS_MRIMAGEDATATYPE_SIZE_REV_8_x      1044U
#define GEHC_SIGNA_GENESIS_PIXHDR_SIZE_REV_8_x               124U

#define GEHC_SIGNA_GENESIS_HEADER_SIZE_REV_8_x               3352U


// revision 9.x

#define GEHC_SIGNA_GENESIS_SUITEDATATYPE_SIZE_REV_9_x         116U
#define GEHC_SIGNA_GENESIS_EXAMDATATYPE_SIZE_REV_9_x         1040U
#define GEHC_SIGNA_GENESIS_SERIESDATATYPE_SIZE_REV_9_x       1028U
#define GEHC_SIGNA_GENESIS_MRIMAGEDATATYPE_SIZE_REV_9_x      1044U
#define GEHC_SIGNA_GENESIS_PIXHDR_SIZE_REV_9_x                124U

#define GEHC_SIGNA_GENESIS_HEADER_SIZE_REV_9_x               3352U


#endif
