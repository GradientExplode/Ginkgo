#ifndef _gkg_processing_plugin_cgal_ForceHardLinkWithCGALPlugin_h
#define _gkg_processing_plugin_cgal_ForceHardLinkWithCGALPlugin_h


namespace gkg
{

bool forceHardLinkWithCGALPlugin();


}


#define FORCE_HARD_LINK_WITH_CGAL_PLUGIN \
bool initialized = gkg::forceHardLinkWithCGALPlugin();


#endif


