#ifndef _CudaThresholding_h_
#define _CudaThresholding_h_


#include <gkg-processing-container/Volume.h>

#include <cuda.h> 


#define CHECK_CUDA_SUCCESS( exp, msg )                                     \
  { if ( ( exp ) != cudaSuccess )                                          \
    {                                                                      \
      std::cerr << "Error on CUDA operation (" << msg << ")" << std::endl; \
      std::cerr << "  -> " << cudaGetErrorString( cudaGetLastError() )     \
                << std::endl;                                              \
    }                                                                      \
  }


namespace gkg
{


void CudaThreshold( Volume< float >& data, float threshold );


}


#endif
