#ifndef _gkg_processing_plugin_functors_DicomProcessingUnit_DpuCommandId_h_
#define _gkg_processing_plugin_functors_DicomProcessingUnit_DpuCommandId_h_


namespace gkg
{


enum DpuCommandId
{

  DPU_REGISTER_CLIENT,
  DPU_UNREGISTER_CLIENT,
  DPU_TRANSFER_VOLUME_INT16,
  DPU_EXIT,
  DPU_COMMAND_COUNT

};


}


#endif
