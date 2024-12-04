#ifndef _gkg_communication_cuda_CudaDevices_h_
#define _gkg_communication_cuda_CudeDevices_h_


#include <gkg-communication-cuda/CudaProperties.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-cppext/StdInt.h>

#ifdef GKG_CUDA
#include <cuda.h>
#include <cuda_runtime.h>
#endif

#include <vector>


#ifndef GKG_CUDA
struct cudaDeviceProp
{

  char* dummy;

};
#endif


namespace gkg
{


class CudaDevices : public Singleton< CudaDevices >
{

  public:

    size_t size();

    int32_t getBestDeviceId( const struct cudaDeviceProp* properties = 0 );

    CudaProperties getProperties( int32_t deviceId );
    void print();

  protected:

    friend class Singleton< CudaDevices >;

    CudaDevices();

  private:

    std::vector< CudaProperties > _devices;

};


}


#endif
