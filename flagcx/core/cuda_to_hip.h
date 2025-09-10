/**
 * Copyright 2017-2023 XGBoost contributors
 */
#pragma once

#if defined(USE_AMD_ADAPTOR)

#include <hip/hip_runtime_api.h>

#ifndef hipEventWaitDefault
#define hipEventWaitDefault                                 0
#endif

#define cudaSuccess                                         hipSuccess
#define cudaError                                           hipError_t
#define cudaError_t                                         hipError_t
#define cudaGetLastError                                    hipGetLastError
#define cudaPeekAtLastError                                 hipPeekAtLastError
#define cudaErrorInvalidValue                               hipErrorInvalidValue
#define cudaErrorNotReady                                   hipErrorNotReady

#define cudaStream_t                                        hipStream_t
#define cudaStreamCreate                                    hipStreamCreate
#define cudaStreamCreateWithFlags                           hipStreamCreateWithFlags
#define cudaStreamDestroy                                   hipStreamDestroy
#define cudaStreamWaitEvent                                 hipStreamWaitEvent
#define cudaStreamSynchronize                               hipStreamSynchronize
#define cudaStreamQuery                                     hipStreamQuery

#define cudaStreamLegacy                                    hipStreamLegacy
#define cudaStreamPerThread                                 hipStreamPerThread
#define cudaStreamNonBlocking                               hipStreamNonBlocking

#define cudaEvent_t                                         hipEvent_t
#define cudaEventCreate                                     hipEventCreate
#define cudaEventCreateWithFlags                            hipEventCreateWithFlags
#define cudaEventDestroy                                    hipEventDestroy
#define cudaEventQuery                                      hipEventQuery
#define cudaEventSynchronize                                hipEventSynchronize
#define cudaEventWaitDefault                                hipEventWaitDefault
#define cudaEventDisableTiming                              hipEventDisableTiming
#define cudaEventRecordDefault                              hipEventRecordDefault
#define cudaEventRecordWithFlags                            hipEventRecordWithFlags

#define cudaDeviceProp                                      hipDeviceProp_t
#define cudaGetDevice                                       hipGetDevice
#define cudaSetDevice                                       hipSetDevice
#define cudaGetDeviceCount                                  hipGetDeviceCount
#define cudaDeviceSynchronize                               hipDeviceSynchronize

#define cudaGetDeviceProperties                             hipGetDeviceProperties
#define cudaDeviceGetAttribute                              hipDeviceGetAttribute
#define cudaDeviceGetByPCIBusId                             hipDeviceGetByPCIBusId
#define cudaDeviceGetPCIBusId                               hipDeviceGetPCIBusId

#define cudaMallocHost                                      hipHostMalloc
#define cudaFreeHost                                        hipFreeHost
#define cudaMalloc                                          hipMalloc
#define cudaMallocAsync                                     hipMallocAsync
#define cudaFree                                            hipFree
#define cudaFreeAsync                                       hipFreeAsync
#define cudaMallocManaged                                   hipMallocManaged

#define cudaMemcpy                                          hipMemcpy
#define cudaMemcpyAsync                                     hipMemcpyAsync
#define cudaMemcpyDefault                                   hipMemcpyDefault
#define cudaMemcpyHostToDevice                              hipMemcpyHostToDevice
#define cudaMemcpyHostToHost                                hipMemcpyHostToHost
#define cudaMemcpyDeviceToHost                              hipMemcpyDeviceToHost
#define cudaMemcpyDeviceToDevice                            hipMemcpyDeviceToDevice
#define cudaMemsetAsync                                     hipMemsetAsync
#define cudaMemset                                          hipMemset
#define cudaMemcpyKind                                      hipMemcpyKind
#define cudaMemAttachGlobal                                 hipMemAttachGlobal

#define cudaPointerAttributes                               hipPointerAttribute_t 
#define cudaPointerGetAttributes                            hipPointerGetAttributes
#define cuPointerSetAttribute                               hipPointerSetAttribute

#define CU_POINTER_ATTRIBUTE_SYNC_MEMOPS                    HIP_POINTER_ATTRIBUTE_SYNC_MEMOPS
#define CUdeviceptr                                         hipDeviceptr_t

/* hipMemoryTypeUnregistered not supported */
#define cudaMemoryTypeUnregistered                          hipMemoryTypeUnified
#define cudaMemoryTypeUnified                               hipMemoryTypeUnified
#define cudaMemoryTypeHost                                  hipMemoryTypeHost

#define cudaMemGetInfo                                      hipMemGetInfo
#define cudaFuncSetAttribute                                hipFuncSetAttribute

#define cudaDevAttrMultiProcessorCount                      hipDeviceAttributeMultiprocessorCount
#define cudaOccupancyMaxActiveBlocksPerMultiprocessor       hipOccupancyMaxActiveBlocksPerMultiprocessor

#define cudaLaunchHostFunc                                  hipLaunchHostFunc

namespace thrust {
    namespace hip {
    }

    namespace cuda = thrust::hip;
}

namespace thrust {
#define cuda_category hip_category
}

namespace hipcub {
}

namespace cub = hipcub;

#endif
