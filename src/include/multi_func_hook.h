//
// Created by lihuang on 2025/4/11.
//
#ifndef HIHOOK_HEADER_H
#define HIHOOK_HEADER_H

#include <stdlib.h>


typedef struct {
  const char *func_name;      // base func name（like "cuGraphAddDependencies"）
  int min_ver;    // adjust to low version
  int max_ver;    // adjust to high version
  const char *real_name;      // the real name（ "cuGraphAddDependencies_v2"）
} CudaFuncMapEntry;

// if multi func, we can add here
// like 12030，means cuda 12.3 ，cuda.h header may give start at version
// all new add func put here
static CudaFuncMapEntry g_func_map[] = {
    /* Source-level aliases selected by cuda.h. Exact ELF dlsym names remain
     * bound to their legacy exported wrappers. */
    {"cuGetProcAddress", 11030, 11999, "cuGetProcAddress"},
    {"cuGetProcAddress", 12000, 99999, "cuGetProcAddress_v2"},
    {"cuGraphInstantiate", 10000, 11039, "cuGraphInstantiate"},
    {"cuGraphInstantiate", 11040, 99999, "cuGraphInstantiateWithFlags"},

    {"cuGraphAddKernelNode", 10000, 11999, "cuGraphAddKernelNode"},
    {"cuGraphAddKernelNode", 12000, 99999, "cuGraphAddKernelNode_v2"},

    {"cuGraphKernelNodeGetParams", 10000, 11999, "cuGraphKernelNodeGetParams"},
    {"cuGraphKernelNodeGetParams", 12000, 99999, "cuGraphKernelNodeGetParams_v2"},

    {"cuGraphKernelNodeSetParams", 10000, 11999, "cuGraphKernelNodeSetParams"},
    {"cuGraphKernelNodeSetParams", 12000, 99999, "cuGraphKernelNodeSetParams_v2"}
};


#endif
