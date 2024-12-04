#ifndef _gkg_simulation_io_ForceHardLinkWithSimulationLibrary_h
#define _gkg_simulation_io_ForceHardLinkWithSimulationLibrary_h


namespace gkg
{


bool forceHardLinkWithSimulationLibrary();


}


#define FORCE_HARD_LINK_WITH_SIMULATION \
bool initialized = gkg::forceHardLinkWithSimulationLibrary();


#endif


