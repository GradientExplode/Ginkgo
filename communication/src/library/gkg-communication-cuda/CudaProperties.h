#ifndef _gkg_communication_cuda_CudaProperties_h_
#define _gkg_communication_cuda_CudaProperties_h_


#include <gkg-core-cppext/StdInt.h>

#include <string>
#include <iostream>


namespace gkg
{


struct CudaProperties
{

  CudaProperties( int32_t device = 0 );
  CudaProperties( const CudaProperties& other );

  CudaProperties& operator = ( const CudaProperties& other );

  void copy( const CudaProperties& other );

  std::string _name;
  size_t _globalMemory;
  int32_t _multiprocessors;
  int32_t _coresPerMP;
  int32_t _cores;
  int32_t _clockRate;
  int32_t _warpSize;
  int32_t _maxThreadsPerBlock;
  int32_t _maxBlockDim[ 3 ];
  int32_t _maxGridDim[ 3 ];

};


}


namespace std
{


ostream& operator << ( ostream& os, const gkg::CudaProperties& properties );


}


#endif
