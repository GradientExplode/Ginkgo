#include <CudaThresholding.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-communication-cuda/CudaDevices.h>

#include <cuda_runtime.h>

#include <iostream>


__global__ void CudaThresholdingKernelFloat( float* c_v,
                                             float* c_o,
                                             float threshold,
                                             int32_t sizeX,
                                             int32_t sizeY,
                                             int32_t sizeZT )
{

  int32_t ix = blockIdx.x * blockDim.x + threadIdx.x;
  int32_t iy = blockIdx.y * blockDim.y + threadIdx.y;
  int32_t sliceSize = sizeX * sizeY;
  int32_t z;

  if ( ( ix < sizeX ) && ( iy < sizeY ) )
  {

    int32_t offset = ix + iy * sizeX;

    for ( z = 0; z < sizeZT; z++, offset += sliceSize )
    {

      if ( c_v[ offset ] >= threshold )
      {

        c_o[ offset ] = c_v[ offset ];

      }

    }

  }

}


void gkg::CudaThreshold( gkg::Volume< float >& data, float threshold )
{

  size_t deviceCount = gkg::CudaDevices::getInstance().size();

  if ( deviceCount < 1 )
  {

    std::cerr << "No CUDA compatible device detected" << std::endl;
    return;

  }

  int32_t deviceID = gkg::CudaDevices::getInstance().getBestDeviceId();
  gkg::CudaProperties bestProperties = 
                      gkg::CudaDevices::getInstance().getProperties( deviceID );

  float* v = &data( 0 );
  float* c_v = 0;
  float* c_o = 0;
  int32_t sizeX = data.getSizeX();
  int32_t sizeY = data.getSizeY();
  int32_t sizeZT = data.getSizeZ() * data.getSizeT();
  size_t size = sizeX * sizeY * sizeZT * sizeof( float );
  cudaEvent_t start, stop, mem;

  CHECK_CUDA_SUCCESS( cudaGetDevice( &deviceID ), "Get device ID" );

  cudaDeviceSynchronize();

  CHECK_CUDA_SUCCESS( cudaEventCreate( &start ), "Create start event" );
  CHECK_CUDA_SUCCESS( cudaEventCreate( &stop ), "Create stop event" );
  CHECK_CUDA_SUCCESS( cudaEventCreate( &mem ), "Create mem event" );
  CHECK_CUDA_SUCCESS( cudaEventRecord( start, 0 ), "Get start time" );

  CHECK_CUDA_SUCCESS( cudaMalloc( &c_v, size ), "Alloc c_v" );
  CHECK_CUDA_SUCCESS( cudaMalloc( &c_o, size ), "Alloc c_o" );

  CHECK_CUDA_SUCCESS( cudaHostRegister( v, size, cudaHostRegisterPortable ),
                      "Host register v" );
  CHECK_CUDA_SUCCESS( cudaMemcpy( c_v, v, size, cudaMemcpyHostToDevice ),
                      "Memcpy v -> c_v" );
  CHECK_CUDA_SUCCESS( cudaMemset( c_o, 0, size ), "Memset c_o to 0" );
  CHECK_CUDA_SUCCESS( cudaEventRecord( mem, 0 ), "Get mem time" );

  dim3 Db( 32, 32 );
  dim3 Dg( sizeX / 32 + ( ( sizeX % 32 ) ? 1 : 0 ),
           sizeY / 32 + ( ( sizeY % 32 ) ? 1 : 0 ) );

  CudaThresholdingKernelFloat<<< Dg, Db >>>( c_v,
                                             c_o,
                                             threshold,
                                             sizeX,
                                             sizeY,
                                             sizeZT );

  cudaEventRecord( stop, 0 );
  cudaEventSynchronize( stop );

  CHECK_CUDA_SUCCESS( cudaMemcpy( v, c_o, size, cudaMemcpyDeviceToHost ),
                      "Memcpy c_o -> v" );
  CHECK_CUDA_SUCCESS( cudaHostUnregister( v ), "Host unregister v" );

  float elapsedTime, memTime;

  cudaEventElapsedTime( &elapsedTime, start, stop );
  cudaEventElapsedTime( &memTime, mem, stop );

  elapsedTime *= 1e-3;
  memTime *= 1e-3;

  cudaEventDestroy( mem );
  cudaEventDestroy( stop );
  cudaEventDestroy( start );
  cudaDeviceSynchronize();

  cudaFree( c_o );
  cudaFree( c_v );

  cudaDeviceReset();

  std::cout << "GPU global time = " << elapsedTime << "s" 
            << " ;  computation time = " << memTime << std::endl;

}
