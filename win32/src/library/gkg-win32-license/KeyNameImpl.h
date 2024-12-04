#ifndef _gkg_win32_license_KeyNameImpl_h_
#define _gkg_win32_license_KeyNameImpl_h_


void gkg::KeyName::initialize()
{

  _keys.insert( std::make_pair( std::string( "t1Seq" ), RT_T1_SEQUENCE ) );
  _keys.insert( std::make_pair( std::string( "fMRISeq" ), RT_FMRI_SEQUENCE ) );
  _keys.insert( std::make_pair( std::string( "dMRISeq" ), RT_DMRI_SEQUENCE ) );

}


#endif
