#ifndef _gkg_dmri_io_ForceHardLinkWithGinkgoLibrary_h
#define _gkg_dmri_io_ForceHardLinkWithGinkgoLibrary_h


namespace gkg
{

bool forceHardLinkWithGinkgoLibrary();


}


#define FORCE_HARD_LINK_WITH_CONNECTOMIST \
bool initialized = gkg::forceHardLinkWithGinkgoLibrary();


#endif


