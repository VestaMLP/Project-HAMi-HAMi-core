#ifndef __LIBCUDA_HOOK_H__
#define __LIBCUDA_HOOK_H__

#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dlfcn.h>
#define NVML_NO_UNVERSIONED_FUNC_DEFS
#include <cuda.h>
#include <pthread.h>
#include "include/log_utils.h"

typedef struct {
  void *fn_ptr;
  char *name;
} cuda_entry_t;

#define FILENAME_MAX 4096

#define CONTEXT_SIZE 104857600

typedef CUresult (*cuda_sym_t)();

#define CUDA_OVERRIDE_ENUM(x) OVERRIDE_##x

#define CUDA_FIND_ENTRY(table, sym) ({ (table)[CUDA_OVERRIDE_ENUM(sym)].fn_ptr; })

#define CUDA_OVERRIDE_CALL(table, sym, ...)                                    \
  ({    \
    LOG_DEBUG("Hijacking %s", #sym);                                           \
    cuda_sym_t _entry = (cuda_sym_t)CUDA_FIND_ENTRY(table, sym);               \
    if (_entry == NULL) {                                                      \
      LOG_ERROR("Hijack failed: %s is NULL", #sym);                            \
    }                                                                          \
    _entry(__VA_ARGS__);                                                       \
  })

typedef enum {
    /* cuInit Part */
    CUDA_OVERRIDE_ENUM(cuInit),
    /* cuDevice Part */
    CUDA_OVERRIDE_ENUM(cuDeviceGetAttribute),
    CUDA_OVERRIDE_ENUM(cuDeviceGet),
    CUDA_OVERRIDE_ENUM(cuDeviceGetCount),
    CUDA_OVERRIDE_ENUM(cuDeviceGetName),
    CUDA_OVERRIDE_ENUM(cuDeviceCanAccessPeer),
    CUDA_OVERRIDE_ENUM(cuDeviceGetP2PAttribute),
    CUDA_OVERRIDE_ENUM(cuDeviceGetByPCIBusId),
    CUDA_OVERRIDE_ENUM(cuDeviceGetPCIBusId),
    CUDA_OVERRIDE_ENUM(cuDeviceGetUuid),
    CUDA_OVERRIDE_ENUM(cuDeviceGetUuid_v2),
    CUDA_OVERRIDE_ENUM(cuDeviceGetDefaultMemPool),
    CUDA_OVERRIDE_ENUM(cuDeviceGetLuid),
    CUDA_OVERRIDE_ENUM(cuDeviceGetMemPool),
    CUDA_OVERRIDE_ENUM(cuDeviceTotalMem_v2),
    CUDA_OVERRIDE_ENUM(cuDriverGetVersion),
    CUDA_OVERRIDE_ENUM(cuDeviceGetTexture1DLinearMaxWidth),
    CUDA_OVERRIDE_ENUM(cuDeviceSetMemPool),
    CUDA_OVERRIDE_ENUM(cuFlushGPUDirectRDMAWrites),

    /* cuContext Part */
    CUDA_OVERRIDE_ENUM(cuDevicePrimaryCtxGetState),
    CUDA_OVERRIDE_ENUM(cuDevicePrimaryCtxRetain),
    CUDA_OVERRIDE_ENUM(cuDevicePrimaryCtxSetFlags_v2),
    CUDA_OVERRIDE_ENUM(cuDevicePrimaryCtxRelease_v2),
    CUDA_OVERRIDE_ENUM(cuCtxGetDevice),
    CUDA_OVERRIDE_ENUM(cuCtxCreate_v2),
    CUDA_OVERRIDE_ENUM(cuCtxCreate_v3),
    CUDA_OVERRIDE_ENUM(cuCtxCreate_v4),
    CUDA_OVERRIDE_ENUM(cuCtxDestroy_v2),
    CUDA_OVERRIDE_ENUM(cuCtxGetApiVersion),
    CUDA_OVERRIDE_ENUM(cuCtxGetCacheConfig),
    CUDA_OVERRIDE_ENUM(cuCtxGetCurrent),
    CUDA_OVERRIDE_ENUM(cuCtxGetFlags),
    CUDA_OVERRIDE_ENUM(cuCtxGetLimit),
    CUDA_OVERRIDE_ENUM(cuCtxGetSharedMemConfig),
    CUDA_OVERRIDE_ENUM(cuCtxGetStreamPriorityRange),
    CUDA_OVERRIDE_ENUM(cuCtxPopCurrent_v2),
    CUDA_OVERRIDE_ENUM(cuCtxPushCurrent_v2),
    CUDA_OVERRIDE_ENUM(cuCtxSetCacheConfig),
    CUDA_OVERRIDE_ENUM(cuCtxSetCurrent),
    CUDA_OVERRIDE_ENUM(cuCtxSetLimit),
    CUDA_OVERRIDE_ENUM(cuCtxSetSharedMemConfig),
    CUDA_OVERRIDE_ENUM(cuCtxSynchronize),
    //CUDA_OVERRIDE_ENUM(cuCtxEnablePeerAccess),
    CUDA_OVERRIDE_ENUM(cuGetExportTable),

    /* cuStream Part */
    CUDA_OVERRIDE_ENUM(cuStreamCreate),
    CUDA_OVERRIDE_ENUM(cuStreamDestroy_v2),
    CUDA_OVERRIDE_ENUM(cuStreamSynchronize),
    /* cuMemory Part */
    CUDA_OVERRIDE_ENUM(cuArray3DCreate_v2),
    CUDA_OVERRIDE_ENUM(cuArrayCreate_v2),
    CUDA_OVERRIDE_ENUM(cuArrayDestroy),
    CUDA_OVERRIDE_ENUM(cuMemAlloc_v2),
    CUDA_OVERRIDE_ENUM(cuMemAllocHost_v2),
    CUDA_OVERRIDE_ENUM(cuMemAllocManaged),
    CUDA_OVERRIDE_ENUM(cuMemAllocPitch_v2),
    CUDA_OVERRIDE_ENUM(cuMemFree_v2),
    CUDA_OVERRIDE_ENUM(cuMemFreeHost),
    CUDA_OVERRIDE_ENUM(cuMemHostAlloc),
    CUDA_OVERRIDE_ENUM(cuMemHostRegister_v2),
    CUDA_OVERRIDE_ENUM(cuMemHostUnregister),
    CUDA_OVERRIDE_ENUM(cuMemcpyDtoH_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpyHtoD_v2),
    CUDA_OVERRIDE_ENUM(cuMipmappedArrayCreate),
    CUDA_OVERRIDE_ENUM(cuMipmappedArrayDestroy),
    CUDA_OVERRIDE_ENUM(cuMemGetInfo_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpy),
    CUDA_OVERRIDE_ENUM(cuPointerGetAttribute),
    CUDA_OVERRIDE_ENUM(cuPointerGetAttributes),
    CUDA_OVERRIDE_ENUM(cuPointerSetAttribute),
    CUDA_OVERRIDE_ENUM(cuIpcCloseMemHandle),
    CUDA_OVERRIDE_ENUM(cuIpcGetMemHandle),
    CUDA_OVERRIDE_ENUM(cuIpcOpenMemHandle_v2),
    CUDA_OVERRIDE_ENUM(cuMemGetAddressRange_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpyAsync),
    CUDA_OVERRIDE_ENUM(cuMemcpyAtoD_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpyDtoA_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpyDtoD_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpyDtoDAsync_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpyDtoHAsync_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpyHtoDAsync_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpyPeer),
    CUDA_OVERRIDE_ENUM(cuMemcpyPeerAsync),
    CUDA_OVERRIDE_ENUM(cuMemsetD16_v2),
    CUDA_OVERRIDE_ENUM(cuMemsetD16Async),
    CUDA_OVERRIDE_ENUM(cuMemsetD2D16_v2),
    CUDA_OVERRIDE_ENUM(cuMemsetD2D16Async),
    CUDA_OVERRIDE_ENUM(cuMemsetD2D32_v2),
    CUDA_OVERRIDE_ENUM(cuMemsetD2D32Async),
    CUDA_OVERRIDE_ENUM(cuMemsetD2D8_v2),
    CUDA_OVERRIDE_ENUM(cuMemsetD2D8Async),
    CUDA_OVERRIDE_ENUM(cuMemsetD32_v2),
    CUDA_OVERRIDE_ENUM(cuMemsetD32Async),
    CUDA_OVERRIDE_ENUM(cuMemsetD8_v2),
    CUDA_OVERRIDE_ENUM(cuMemsetD8Async),
    CUDA_OVERRIDE_ENUM(cuMemAdvise),
    CUDA_OVERRIDE_ENUM(cuMemAdvise_v2),
    CUDA_OVERRIDE_ENUM(cuFuncSetCacheConfig),
    CUDA_OVERRIDE_ENUM(cuFuncSetSharedMemConfig),
    CUDA_OVERRIDE_ENUM(cuFuncGetAttribute),
    CUDA_OVERRIDE_ENUM(cuFuncSetAttribute),
    CUDA_OVERRIDE_ENUM(cuLaunchKernel),
    CUDA_OVERRIDE_ENUM(cuLaunchKernelEx),
    CUDA_OVERRIDE_ENUM(cuLaunchCooperativeKernel),
    /* cuEvent Part */
    CUDA_OVERRIDE_ENUM(cuEventCreate),
    CUDA_OVERRIDE_ENUM(cuEventDestroy_v2),
    CUDA_OVERRIDE_ENUM(cuModuleLoad),
    CUDA_OVERRIDE_ENUM(cuModuleLoadData),
    CUDA_OVERRIDE_ENUM(cuModuleLoadDataEx),
    CUDA_OVERRIDE_ENUM(cuModuleLoadFatBinary),
    CUDA_OVERRIDE_ENUM(cuModuleGetFunction),
    CUDA_OVERRIDE_ENUM(cuModuleUnload),
    CUDA_OVERRIDE_ENUM(cuModuleGetGlobal_v2),
    CUDA_OVERRIDE_ENUM(cuModuleGetTexRef),
    CUDA_OVERRIDE_ENUM(cuModuleGetSurfRef),
    CUDA_OVERRIDE_ENUM(cuLinkAddData_v2),
    CUDA_OVERRIDE_ENUM(cuLinkCreate_v2),
    CUDA_OVERRIDE_ENUM(cuLinkAddFile_v2),
    CUDA_OVERRIDE_ENUM(cuLinkComplete),
    CUDA_OVERRIDE_ENUM(cuLinkDestroy),

    /* Virtual Memory Part */
    CUDA_OVERRIDE_ENUM(cuMemAddressReserve),
    CUDA_OVERRIDE_ENUM(cuMemCreate),
    CUDA_OVERRIDE_ENUM(cuMemRelease),
    CUDA_OVERRIDE_ENUM(cuMemMap),
    CUDA_OVERRIDE_ENUM(cuMemImportFromShareableHandle),
    CUDA_OVERRIDE_ENUM(cuMemAllocAsync),
    CUDA_OVERRIDE_ENUM(cuMemFreeAsync),
    /* cuda11.7 new api memory part */
    CUDA_OVERRIDE_ENUM(cuMemHostGetDevicePointer_v2),
    CUDA_OVERRIDE_ENUM(cuMemHostGetFlags),
    CUDA_OVERRIDE_ENUM(cuMemPoolTrimTo),
    CUDA_OVERRIDE_ENUM(cuMemPoolSetAttribute),
    CUDA_OVERRIDE_ENUM(cuMemPoolGetAttribute),
    CUDA_OVERRIDE_ENUM(cuMemPoolSetAccess),
    CUDA_OVERRIDE_ENUM(cuMemPoolGetAccess),
    CUDA_OVERRIDE_ENUM(cuMemPoolCreate),
    CUDA_OVERRIDE_ENUM(cuMemPoolDestroy),
    CUDA_OVERRIDE_ENUM(cuMemAllocFromPoolAsync),
    CUDA_OVERRIDE_ENUM(cuMemPoolExportToShareableHandle),
    CUDA_OVERRIDE_ENUM(cuMemPoolImportFromShareableHandle),
    CUDA_OVERRIDE_ENUM(cuMemPoolExportPointer),
    CUDA_OVERRIDE_ENUM(cuMemPoolImportPointer),
    CUDA_OVERRIDE_ENUM(cuMemcpy2D_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpy2DUnaligned_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpy2DAsync_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpy3D_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpy3DAsync_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpy3DPeer),
    CUDA_OVERRIDE_ENUM(cuMemcpy3DPeerAsync),
    CUDA_OVERRIDE_ENUM(cuMemPrefetchAsync),
    CUDA_OVERRIDE_ENUM(cuMemPrefetchAsync_v2),
    CUDA_OVERRIDE_ENUM(cuMemRangeGetAttribute),
    CUDA_OVERRIDE_ENUM(cuMemRangeGetAttributes),
    /* cuda 11.7 external resource management */
    CUDA_OVERRIDE_ENUM(cuImportExternalMemory),
    CUDA_OVERRIDE_ENUM(cuExternalMemoryGetMappedBuffer),
    CUDA_OVERRIDE_ENUM(cuExternalMemoryGetMappedMipmappedArray),
    CUDA_OVERRIDE_ENUM(cuDestroyExternalMemory),
    CUDA_OVERRIDE_ENUM(cuImportExternalSemaphore),
    CUDA_OVERRIDE_ENUM(cuSignalExternalSemaphoresAsync),
    CUDA_OVERRIDE_ENUM(cuWaitExternalSemaphoresAsync),
    CUDA_OVERRIDE_ENUM(cuDestroyExternalSemaphore),
    /* cuda graph part */
    CUDA_OVERRIDE_ENUM(cuGraphCreate),
    CUDA_OVERRIDE_ENUM(cuGraphAddKernelNode_v2),
    CUDA_OVERRIDE_ENUM(cuGraphKernelNodeGetParams_v2),
    CUDA_OVERRIDE_ENUM(cuGraphKernelNodeSetParams_v2),
    CUDA_OVERRIDE_ENUM(cuGraphAddMemcpyNode),
    CUDA_OVERRIDE_ENUM(cuGraphMemcpyNodeGetParams),
    CUDA_OVERRIDE_ENUM(cuGraphMemcpyNodeSetParams),
    CUDA_OVERRIDE_ENUM(cuGraphAddMemsetNode),
    CUDA_OVERRIDE_ENUM(cuGraphMemsetNodeGetParams),
    CUDA_OVERRIDE_ENUM(cuGraphMemsetNodeSetParams),
    CUDA_OVERRIDE_ENUM(cuGraphAddHostNode),
    CUDA_OVERRIDE_ENUM(cuGraphHostNodeGetParams),
    CUDA_OVERRIDE_ENUM(cuGraphHostNodeSetParams),
    CUDA_OVERRIDE_ENUM(cuGraphAddChildGraphNode),
    CUDA_OVERRIDE_ENUM(cuGraphChildGraphNodeGetGraph),
    CUDA_OVERRIDE_ENUM(cuGraphAddEmptyNode),
    CUDA_OVERRIDE_ENUM(cuGraphAddEventRecordNode),
    CUDA_OVERRIDE_ENUM(cuGraphEventRecordNodeGetEvent),
    CUDA_OVERRIDE_ENUM(cuGraphEventRecordNodeSetEvent),
    CUDA_OVERRIDE_ENUM(cuGraphAddEventWaitNode),
    CUDA_OVERRIDE_ENUM(cuGraphEventWaitNodeGetEvent),
    CUDA_OVERRIDE_ENUM(cuGraphEventWaitNodeSetEvent),
    CUDA_OVERRIDE_ENUM(cuGraphAddExternalSemaphoresSignalNode),
    CUDA_OVERRIDE_ENUM(cuGraphExternalSemaphoresSignalNodeGetParams),
    CUDA_OVERRIDE_ENUM(cuGraphExternalSemaphoresSignalNodeSetParams),
    CUDA_OVERRIDE_ENUM(cuGraphAddExternalSemaphoresWaitNode),
    CUDA_OVERRIDE_ENUM(cuGraphExternalSemaphoresWaitNodeGetParams),
    CUDA_OVERRIDE_ENUM(cuGraphExternalSemaphoresWaitNodeSetParams),
    CUDA_OVERRIDE_ENUM(cuGraphExecExternalSemaphoresSignalNodeSetParams),
    CUDA_OVERRIDE_ENUM(cuGraphExecExternalSemaphoresWaitNodeSetParams),
    CUDA_OVERRIDE_ENUM(cuGraphClone),
    CUDA_OVERRIDE_ENUM(cuGraphNodeFindInClone),
    CUDA_OVERRIDE_ENUM(cuGraphNodeGetType),
    CUDA_OVERRIDE_ENUM(cuGraphGetNodes),
    CUDA_OVERRIDE_ENUM(cuGraphGetRootNodes),
    CUDA_OVERRIDE_ENUM(cuGraphGetEdges),
    CUDA_OVERRIDE_ENUM(cuGraphGetEdges_v2),
    CUDA_OVERRIDE_ENUM(cuGraphNodeGetDependencies),
    CUDA_OVERRIDE_ENUM(cuGraphNodeGetDependencies_v2),
    CUDA_OVERRIDE_ENUM(cuGraphNodeGetDependentNodes),
    CUDA_OVERRIDE_ENUM(cuGraphNodeGetDependentNodes_v2),
    CUDA_OVERRIDE_ENUM(cuGraphAddDependencies),
    CUDA_OVERRIDE_ENUM(cuGraphAddDependencies_v2),
    CUDA_OVERRIDE_ENUM(cuGraphRemoveDependencies),
    CUDA_OVERRIDE_ENUM(cuGraphRemoveDependencies_v2),
    CUDA_OVERRIDE_ENUM(cuGraphDestroyNode),
    CUDA_OVERRIDE_ENUM(cuGraphInstantiate),
    CUDA_OVERRIDE_ENUM(cuGraphInstantiateWithFlags),
    CUDA_OVERRIDE_ENUM(cuGraphUpload),
    CUDA_OVERRIDE_ENUM(cuGraphLaunch),
    CUDA_OVERRIDE_ENUM(cuGraphExecDestroy),
    CUDA_OVERRIDE_ENUM(cuGraphDestroy),

    CUDA_OVERRIDE_ENUM(cuCtxGetDevice_v2),
    CUDA_OVERRIDE_ENUM(cuCtxSynchronize_v2),
    CUDA_OVERRIDE_ENUM(cuDeviceGetHostAtomicCapabilities),
    CUDA_OVERRIDE_ENUM(cuDeviceGetP2PAtomicCapabilities),
    CUDA_OVERRIDE_ENUM(cuGraphNodeGetContainingGraph),
    CUDA_OVERRIDE_ENUM(cuGraphNodeGetLocalId),
    CUDA_OVERRIDE_ENUM(cuGraphNodeGetToolsId),
    CUDA_OVERRIDE_ENUM(cuGraphGetId),
    CUDA_OVERRIDE_ENUM(cuGraphExecGetId),
    CUDA_OVERRIDE_ENUM(cuFuncGetParamCount),
    CUDA_OVERRIDE_ENUM(cuKernelGetParamCount),
    CUDA_OVERRIDE_ENUM(cuGraphNodeGetParams),
    CUDA_OVERRIDE_ENUM(cuCoredumpRegisterStartCallback),
    CUDA_OVERRIDE_ENUM(cuCoredumpRegisterCompleteCallback),
    CUDA_OVERRIDE_ENUM(cuCoredumpDeregisterStartCallback),
    CUDA_OVERRIDE_ENUM(cuCoredumpDeregisterCompleteCallback),
    CUDA_OVERRIDE_ENUM(cuLaunchHostFunc),
    CUDA_OVERRIDE_ENUM(cuLaunchHostFunc_ptsz),
    CUDA_OVERRIDE_ENUM(cuLaunchHostFunc_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpyBatchAsync),
    CUDA_OVERRIDE_ENUM(cuMemcpy3DBatchAsync),
    CUDA_OVERRIDE_ENUM(cuMemcpyBatchAsync_ptsz),
    CUDA_OVERRIDE_ENUM(cuMemcpy3DBatchAsync_ptsz),
    CUDA_OVERRIDE_ENUM(cuMemcpyWithAttributesAsync_ptsz),
    CUDA_OVERRIDE_ENUM(cuMemcpy3DWithAttributesAsync_ptsz),
    CUDA_OVERRIDE_ENUM(cuMemPrefetchBatchAsync_ptsz),
    CUDA_OVERRIDE_ENUM(cuMemDiscardBatchAsync_ptsz),
    CUDA_OVERRIDE_ENUM(cuMemDiscardAndPrefetchBatchAsync_ptsz),
    CUDA_OVERRIDE_ENUM(cuMemcpyBatchAsync_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpy3DBatchAsync_v2),
    CUDA_OVERRIDE_ENUM(cuMemcpyWithAttributesAsync),
    CUDA_OVERRIDE_ENUM(cuMemcpy3DWithAttributesAsync),
    CUDA_OVERRIDE_ENUM(cuMemPrefetchBatchAsync),
    CUDA_OVERRIDE_ENUM(cuMemDiscardBatchAsync),
    CUDA_OVERRIDE_ENUM(cuMemDiscardAndPrefetchBatchAsync),
    CUDA_OVERRIDE_ENUM(cuMemGetDefaultMemPool),
    CUDA_OVERRIDE_ENUM(cuMemGetMemPool),
    CUDA_OVERRIDE_ENUM(cuMemSetMemPool),
    CUDA_OVERRIDE_ENUM(cuMulticastBindMem),
    CUDA_OVERRIDE_ENUM(cuMulticastBindAddr),
    CUDA_OVERRIDE_ENUM(cuMulticastBindMem_v2),
    CUDA_OVERRIDE_ENUM(cuMulticastBindAddr_v2),
    CUDA_OVERRIDE_ENUM(cuLogicalEndpointIdReserve),
    CUDA_OVERRIDE_ENUM(cuLogicalEndpointIdRelease),
    CUDA_OVERRIDE_ENUM(cuLogicalEndpointCreate),
    CUDA_OVERRIDE_ENUM(cuLogicalEndpointAddDevice),
    CUDA_OVERRIDE_ENUM(cuLogicalEndpointDestroy),
    CUDA_OVERRIDE_ENUM(cuLogicalEndpointBindAddr),
    CUDA_OVERRIDE_ENUM(cuLogicalEndpointBindMem),
    CUDA_OVERRIDE_ENUM(cuLogicalEndpointUnbind),
    CUDA_OVERRIDE_ENUM(cuLogicalEndpointExport),
    CUDA_OVERRIDE_ENUM(cuLogicalEndpointImport),
    CUDA_OVERRIDE_ENUM(cuLogicalEndpointGetLimits),
    CUDA_OVERRIDE_ENUM(cuLogicalEndpointQuery),
    CUDA_OVERRIDE_ENUM(cuStreamGetDevResource),
    CUDA_OVERRIDE_ENUM(cuStreamGetDevResource_ptsz),
    CUDA_OVERRIDE_ENUM(cuDevSmResourceSplit),
    CUDA_OVERRIDE_ENUM(cuStreamBeginCaptureToCig),
    CUDA_OVERRIDE_ENUM(cuStreamBeginCaptureToCig_ptsz),
    CUDA_OVERRIDE_ENUM(cuStreamEndCaptureToCig),
    CUDA_OVERRIDE_ENUM(cuStreamEndCaptureToCig_ptsz),
    CUDA_OVERRIDE_ENUM(cuStreamBeginRecaptureToGraph),
    CUDA_OVERRIDE_ENUM(cuStreamBeginRecaptureToGraph_ptsz),
    CUDA_OVERRIDE_ENUM(cuGetProcAddress),
    CUDA_OVERRIDE_ENUM(cuGetProcAddress_v2),
    CUDA_ENTRY_END
}cuda_override_enum_t;

extern cuda_entry_t cuda_library_entry[];

#endif

#undef cuGetProcAddress
CUresult cuGetProcAddress( const char* symbol, void** pfn, int  cudaVersion, cuuint64_t flags );
#undef cuGraphInstantiate
CUresult cuGraphInstantiate(CUgraphExec *phGraphExec, CUgraph hGraph, CUgraphNode *phErrorNode, char *logBuffer, size_t bufferSize);

#undef cuLaunchHostFunc
CUresult CUDAAPI cuLaunchHostFunc(CUstream hStream, CUhostFn fn, void *userData);
CUresult CUDAAPI cuLaunchHostFunc_ptsz(CUstream hStream, CUhostFn fn, void *userData);
CUresult CUDAAPI cuMemcpyBatchAsync_ptsz(CUdeviceptr *dsts, CUdeviceptr *srcs, size_t *sizes, size_t count, CUmemcpyAttributes *attrs, size_t *attrsIdxs, size_t numAttrs, size_t *failIdx, CUstream hStream);
CUresult CUDAAPI cuMemcpy3DBatchAsync_ptsz(size_t numOps, CUDA_MEMCPY3D_BATCH_OP *opList, size_t *failIdx, unsigned long long flags, CUstream hStream);
CUresult CUDAAPI cuMemcpyWithAttributesAsync_ptsz(CUdeviceptr dst, CUdeviceptr src, size_t size, CUmemcpyAttributes *attr, CUstream hStream);
CUresult CUDAAPI cuMemcpy3DWithAttributesAsync_ptsz(CUDA_MEMCPY3D_BATCH_OP *op, unsigned long long flags, CUstream hStream);
CUresult CUDAAPI cuMemPrefetchBatchAsync_ptsz(CUdeviceptr *dptrs, size_t *sizes, size_t count, CUmemLocation *locs, size_t *idxs, size_t numLocs, unsigned long long flags, CUstream stream);
CUresult CUDAAPI cuMemDiscardBatchAsync_ptsz(CUdeviceptr *dptrs, size_t *sizes, size_t count, unsigned long long flags, CUstream stream);
CUresult CUDAAPI cuMemDiscardAndPrefetchBatchAsync_ptsz(CUdeviceptr *dptrs, size_t *sizes, size_t count, CUmemLocation *locs, size_t *idxs, size_t numLocs, unsigned long long flags, CUstream stream);

CUresult CUDAAPI cuStreamGetDevResource_ptsz(CUstream stream, CUdevResource *result, CUdevResourceType type);
CUresult CUDAAPI cuStreamBeginCaptureToCig_ptsz(CUstream stream, CUstreamCigCaptureParams *params);
CUresult CUDAAPI cuStreamEndCaptureToCig_ptsz(CUstream stream);
CUresult CUDAAPI cuStreamBeginRecaptureToGraph_ptsz(CUstream stream, CUstreamCaptureMode mode, CUgraph graph, CUgraphRecaptureCallback callback, void *userData);

