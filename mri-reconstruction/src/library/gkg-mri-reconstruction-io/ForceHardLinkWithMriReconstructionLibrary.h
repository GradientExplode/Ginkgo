#ifndef _gkg_mri_reconstruction_io_ForceHardLinkWithMriReconstructionLibrary_h
#define _gkg_mri_reconstruction_io_ForceHardLinkWithMriReconstructionLibrary_h


namespace gkg
{

bool forceHardLinkWithMriReconstructionLibrary();


}


#define FORCE_HARD_LINK_WITH_MRI_RECONSTRUCTION \
bool initialized = gkg::forceHardLinkWithMriReconstructionLibrary();


#endif


