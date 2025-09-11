#ifdef USE_NVIDIA_ADAPTOR

#include "adaptor.h"
#include "alloc.h"
#include "comm.h"
#include "flagcx.h"
#include <stdio.h>

/*
#ifdef USE_AMD_ADAPTOR
#include "rccl.h"
#include "cuda_to_hip.h"
#else
#include "nccl.h"
#include <cuda.h>
#include <cuda_runtime.h>
#endif
*/

#ifdef USE_AMD_ADAPTOR
#include "rccl.h"
#else
#include "nccl.h"
#endif

#ifdef USE_AMD_ADAPTOR
#include "cuda_to_hip.h"
#else
#include <cuda.h>
#include <cuda_runtime.h>
#endif


#include <map>
struct flagcxInnerComm {
  ncclComm_t base;
};

struct flagcxStream {
  cudaStream_t base;
};

struct flagcxEvent {
  cudaEvent_t base;
};

#define DEVCHECK(func)                                                         \
  {                                                                            \
    int ret = func;                                                            \
    if (ret != cudaSuccess)                                                    \
      return flagcxUnhandledDeviceError;                                       \
  }

#endif // USE_NVIDIA_ADAPTOR
