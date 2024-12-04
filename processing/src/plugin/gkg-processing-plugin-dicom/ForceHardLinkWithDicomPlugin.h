#ifndef _gkg_processing_plugin_dicom_ForceHardLinkWithDicomPlugin_h
#define _gkg_processing_plugin_dicom_ForceHardLinkWithDicomPlugin_h


namespace gkg
{

bool forceHardLinkWithDicomPlugin();


}


#define FORCE_HARD_LINK_WITH_DICOM_PLUGIN \
bool initialized = gkg::forceHardLinkWithDicomPlugin();


#endif


