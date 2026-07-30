#include <string.h>
#include <ctype.h>
#include <dlfcn.h>
#include "include/libnvml_hook.h"
#include "include/nvml-subset.h"
#include "include/utils.h"
#include "multiprocess/multiprocess_memory_limit.h"

entry_t nvml_library_entry[] = {
    {.name = "nvmlInit"},
    {.name = "nvmlShutdown"},
    {.name = "nvmlErrorString"},
    {.name = "nvmlDeviceGetHandleByIndex"},
    {.name = "nvmlDeviceGetComputeRunningProcesses"},
    {.name = "nvmlDeviceGetPciInfo"},
    {.name = "nvmlDeviceGetProcessUtilization"},
    {.name = "nvmlDeviceGetProcessesUtilizationInfo"},
    {.name = "nvmlDeviceGetCount"},
    {.name = "nvmlDeviceClearAccountingPids"},
    {.name = "nvmlDeviceClearCpuAffinity"},
    {.name = "nvmlDeviceClearEccErrorCounts"},
    {.name = "nvmlDeviceDiscoverGpus"},
    {.name = "nvmlDeviceFreezeNvLinkUtilizationCounter"},
    {.name = "nvmlDeviceGetAccountingBufferSize"},
    {.name = "nvmlDeviceGetAccountingMode"},
    {.name = "nvmlDeviceGetAccountingPids"},
    {.name = "nvmlDeviceGetAccountingStats"},
    {.name = "nvmlDeviceGetActiveVgpus"},
    {.name = "nvmlDeviceGetAPIRestriction"},
    {.name = "nvmlDeviceGetApplicationsClock"},
    {.name = "nvmlDeviceGetAutoBoostedClocksEnabled"},
    {.name = "nvmlDeviceGetBAR1MemoryInfo"},
    {.name = "nvmlDeviceGetBoardId"},
    {.name = "nvmlDeviceGetBoardPartNumber"},
    {.name = "nvmlDeviceGetBrand"},
    {.name = "nvmlDeviceGetBridgeChipInfo"},
    {.name = "nvmlDeviceGetClock"},
    {.name = "nvmlDeviceGetClockInfo"},
    {.name = "nvmlDeviceGetComputeMode"},
    {.name = "nvmlDeviceGetCount_v2"},
    {.name = "nvmlDeviceGetCpuAffinity"},
    {.name = "nvmlDeviceGetCreatableVgpus"},
    {.name = "nvmlDeviceGetCudaComputeCapability"},
    {.name = "nvmlDeviceGetCurrentClocksThrottleReasons"},
    {.name = "nvmlDeviceGetCurrPcieLinkGeneration"},
    {.name = "nvmlDeviceGetCurrPcieLinkWidth"},
    {.name = "nvmlDeviceGetDecoderUtilization"},
    {.name = "nvmlDeviceGetDefaultApplicationsClock"},
    {.name = "nvmlDeviceGetDetailedEccErrors"},
    {.name = "nvmlDeviceGetDisplayActive"},
    {.name = "nvmlDeviceGetDisplayMode"},
    {.name = "nvmlDeviceGetDriverModel"},
    {.name = "nvmlDeviceGetEccMode"},
    {.name = "nvmlDeviceGetEncoderCapacity"},
    {.name = "nvmlDeviceGetEncoderSessions"},
    {.name = "nvmlDeviceGetEncoderStats"},
    {.name = "nvmlDeviceGetEncoderUtilization"},
    {.name = "nvmlDeviceGetEnforcedPowerLimit"},
    {.name = "nvmlDeviceGetFanSpeed"},
    {.name = "nvmlDeviceGetFanSpeed_v2"},
    {.name = "nvmlDeviceGetFieldValues"},
    {.name = "nvmlDeviceGetGpuOperationMode"},
    {.name = "nvmlDeviceGetGraphicsRunningProcesses"},
    {.name = "nvmlDeviceGetGridLicensableFeatures"},
    {.name = "nvmlDeviceGetHandleByIndex_v2"},
    {.name = "nvmlDeviceGetHandleByPciBusId"},
    {.name = "nvmlDeviceGetHandleByPciBusId_v2"},
    {.name = "nvmlDeviceGetHandleBySerial"},
    {.name = "nvmlDeviceGetHandleByUUID"},
    {.name = "nvmlDeviceGetHandleByUUIDV"},
    {.name = "nvmlDeviceGetRunningProcessDetailList"},
    {.name = "nvmlDeviceGetAccountingStats_v2"},
    {.name = "nvmlDeviceGetGpuFabricInfoV"},
    {.name = "nvmlDeviceGetClockOffsets"},
    {.name = "nvmlDeviceSetClockOffsets"},
    {.name = "nvmlDeviceGetCapabilities"},
    {.name = "nvmlSystemGetDriverBranch"},
    {.name = "nvmlDeviceGetAddressingMode"},
    {.name = "nvmlDeviceGetRepairStatus"},
    {.name = "nvmlDeviceGetPdi"},
    {.name = "nvmlDeviceGetNvLinkInfo"},
    {.name = "nvmlDeviceReadWritePRM_v1"},
    {.name = "nvmlDeviceGetGpuInstanceProfileInfoByIdV"},
    {.name = "nvmlSystemGetCPER_v1"},
    {.name = "nvmlDeviceGetPowerMizerMode_v1"},
    {.name = "nvmlDeviceSetPowerMizerMode_v1"},
    {.name = "nvmlDeviceSetHostname_v1"},
    {.name = "nvmlDeviceGetHostname_v1"},
    {.name = "nvmlDeviceGetSramUniqueUncorrectedEccErrorCounts"},
    {.name = "nvmlDeviceWorkloadPowerProfileUpdateProfiles_v1"},
    {.name = "nvmlDeviceSetRusdSettings_v1"},
    {.name = "nvmlDeviceGetBBXTimeData_v1"},
    {.name = "nvmlDeviceGetCurrentClocksEventReasons"},
    {.name = "nvmlDeviceGetRemappedRows_v2"},
    {.name = "nvmlDeviceGetSupportedClocksEventReasons"},
    {.name = "nvmlDeviceGetUnrepairableMemoryFlag_v1"},
    {.name = "nvmlDeviceGetVgpuCapabilities"},
    {.name = "nvmlDeviceGetVgpuHeterogeneousMode"},
    {.name = "nvmlDeviceGetVgpuSchedulerLog"},
    {.name = "nvmlDeviceGetVgpuSchedulerLog_v2"},
    {.name = "nvmlDeviceGetVgpuSchedulerState"},
    {.name = "nvmlDeviceGetVgpuSchedulerState_v2"},
    {.name = "nvmlDeviceGetVgpuTypeCreatablePlacements"},
    {.name = "nvmlDeviceReadPRMCounters_v1"},
    {.name = "nvmlDeviceSetPowerManagementLimit_v2"},
    {.name = "nvmlDeviceSetVgpuCapabilities"},
    {.name = "nvmlDeviceSetVgpuHeterogeneousMode"},
    {.name = "nvmlDeviceSetVgpuSchedulerState"},
    {.name = "nvmlDeviceSetVgpuSchedulerState_v2"},
    {.name = "nvmlDeviceVgpuForceGspUnload"},
    {.name = "nvmlDeviceWorkloadPowerProfileClearRequestedProfiles"},
    {.name = "nvmlDeviceWorkloadPowerProfileSetRequestedProfiles"},
    {.name = "nvmlGpuInstanceGetActiveVgpus"},
    {.name = "nvmlGpuInstanceGetCreatableVgpus"},
    {.name = "nvmlGpuInstanceGetVgpuHeterogeneousMode"},
    {.name = "nvmlGpuInstanceGetVgpuSchedulerLog"},
    {.name = "nvmlGpuInstanceGetVgpuSchedulerLog_v2"},
    {.name = "nvmlGpuInstanceGetVgpuSchedulerState"},
    {.name = "nvmlGpuInstanceGetVgpuSchedulerState_v2"},
    {.name = "nvmlGpuInstanceGetVgpuTypeCreatablePlacements"},
    {.name = "nvmlGpuInstanceSetVgpuHeterogeneousMode"},
    {.name = "nvmlGpuInstanceSetVgpuSchedulerState"},
    {.name = "nvmlGpuInstanceSetVgpuSchedulerState_v2"},
    {.name = "nvmlSystemEventSetCreate"},
    {.name = "nvmlSystemEventSetFree"},
    {.name = "nvmlSystemEventSetWait"},
    {.name = "nvmlSystemRegisterEvents"},
    {.name = "nvmlVgpuTypeGetMaxInstancesPerGpuInstance"},

    {.name = "nvmlDeviceGetIndex"},
    {.name = "nvmlDeviceGetInforomConfigurationChecksum"},
    {.name = "nvmlDeviceGetInforomImageVersion"},
    {.name = "nvmlDeviceGetInforomVersion"},
    {.name = "nvmlDeviceGetMaxClockInfo"},
    {.name = "nvmlDeviceGetMaxCustomerBoostClock"},
    {.name = "nvmlDeviceGetMaxPcieLinkGeneration"},
    {.name = "nvmlDeviceGetMaxPcieLinkWidth"},
    {.name = "nvmlDeviceGetMemoryErrorCounter"},
    {.name = "nvmlDeviceGetMemoryInfo"},
    {.name = "nvmlDeviceGetMemoryInfo_v2"},
    {.name = "nvmlDeviceGetMinorNumber"},
    {.name = "nvmlDeviceGetMPSComputeRunningProcesses"},
    {.name = "nvmlDeviceGetMultiGpuBoard"},
    {.name = "nvmlDeviceGetName"},
    {.name = "nvmlDeviceGetNvLinkCapability"},
    {.name = "nvmlDeviceGetNvLinkErrorCounter"},
    {.name = "nvmlDeviceGetNvLinkRemotePciInfo"},
    {.name = "nvmlDeviceGetNvLinkRemotePciInfo_v2"},
    {.name = "nvmlDeviceGetNvLinkState"},
    {.name = "nvmlDeviceGetNvLinkUtilizationControl"},
    {.name = "nvmlDeviceGetNvLinkUtilizationCounter"},
    {.name = "nvmlDeviceGetNvLinkVersion"},
    {.name = "nvmlDeviceGetP2PStatus"},
    {.name = "nvmlDeviceGetPcieReplayCounter"},
    {.name = "nvmlDeviceGetPcieThroughput"},
    {.name = "nvmlDeviceGetPciInfo_v2"},
    {.name = "nvmlDeviceGetPciInfo_v3"},
    {.name = "nvmlDeviceGetPerformanceState"},
    {.name = "nvmlDeviceGetPersistenceMode"},
    {.name = "nvmlDeviceGetPowerManagementDefaultLimit"},
    {.name = "nvmlDeviceGetPowerManagementLimit"},
    {.name = "nvmlDeviceGetPowerManagementLimitConstraints"},
    {.name = "nvmlDeviceGetPowerManagementMode"},
    {.name = "nvmlDeviceGetPowerState"},
    {.name = "nvmlDeviceGetPowerUsage"},
    {.name = "nvmlDeviceGetRetiredPages"},
    {.name = "nvmlDeviceGetRetiredPagesPendingStatus"},
    {.name = "nvmlDeviceGetSamples"},
    {.name = "nvmlDeviceGetSerial"},
    {.name = "nvmlDeviceGetSupportedClocksThrottleReasons"},
    {.name = "nvmlDeviceGetSupportedEventTypes"},
    {.name = "nvmlDeviceGetSupportedGraphicsClocks"},
    {.name = "nvmlDeviceGetSupportedMemoryClocks"},
    {.name = "nvmlDeviceGetSupportedVgpus"},
    {.name = "nvmlDeviceGetTemperature"},
    {.name = "nvmlDeviceGetTemperatureThreshold"},
    {.name = "nvmlDeviceGetTopologyCommonAncestor"},
    {.name = "nvmlDeviceGetTopologyNearestGpus"},
    {.name = "nvmlDeviceGetTotalEccErrors"},
    {.name = "nvmlDeviceGetTotalEnergyConsumption"},
    {.name = "nvmlDeviceGetUtilizationRates"},
    {.name = "nvmlDeviceGetUUID"},
    {.name = "nvmlDeviceGetVbiosVersion"},
    {.name = "nvmlDeviceGetVgpuMetadata"},
    {.name = "nvmlDeviceGetVgpuProcessUtilization"},
    {.name = "nvmlDeviceGetVgpuUtilization"},
    {.name = "nvmlDeviceGetViolationStatus"},
    {.name = "nvmlDeviceGetVirtualizationMode"},
    {.name = "nvmlDeviceModifyDrainState"},
    {.name = "nvmlDeviceOnSameBoard"},
    {.name = "nvmlDeviceQueryDrainState"},
    {.name = "nvmlDeviceRegisterEvents"},
    {.name = "nvmlDeviceRemoveGpu"},
    {.name = "nvmlDeviceRemoveGpu_v2"},
    {.name = "nvmlDeviceResetApplicationsClocks"},
    {.name = "nvmlDeviceResetNvLinkErrorCounters"},
    {.name = "nvmlDeviceResetNvLinkUtilizationCounter"},
    {.name = "nvmlDeviceSetAccountingMode"},
    {.name = "nvmlDeviceSetAPIRestriction"},
    {.name = "nvmlDeviceSetApplicationsClocks"},
    {.name = "nvmlDeviceSetAutoBoostedClocksEnabled"},
    /** We hijack this call*/
    {.name = "nvmlDeviceSetComputeMode"},
    {.name = "nvmlDeviceSetCpuAffinity"},
    {.name = "nvmlDeviceSetDefaultAutoBoostedClocksEnabled"},
    {.name = "nvmlDeviceSetDriverModel"},
    {.name = "nvmlDeviceSetEccMode"},
    {.name = "nvmlDeviceSetGpuOperationMode"},
    {.name = "nvmlDeviceSetNvLinkUtilizationControl"},
    {.name = "nvmlDeviceSetPersistenceMode"},
    {.name = "nvmlDeviceSetPowerManagementLimit"},
    {.name = "nvmlDeviceSetVirtualizationMode"},
    {.name = "nvmlDeviceValidateInforom"},
    {.name = "nvmlEventSetCreate"},
    {.name = "nvmlEventSetFree"},
    {.name = "nvmlEventSetWait"},
    {.name = "nvmlGetVgpuCompatibility"},
    {.name = "nvmlInit_v2"},
    {.name = "nvmlInitWithFlags"},
    {.name = "nvmlInternalGetExportTable"},
    {.name = "nvmlSystemGetCudaDriverVersion"},
    {.name = "nvmlSystemGetCudaDriverVersion_v2"},
    {.name = "nvmlSystemGetDriverVersion"},
    {.name = "nvmlSystemGetHicVersion"},
    {.name = "nvmlSystemGetNVMLVersion"},
    {.name = "nvmlSystemGetProcessName"},
    {.name = "nvmlSystemGetTopologyGpuSet"},
    {.name = "nvmlUnitGetCount"},
    {.name = "nvmlUnitGetDevices"},
    {.name = "nvmlUnitGetFanSpeedInfo"},
    {.name = "nvmlUnitGetHandleByIndex"},
    {.name = "nvmlUnitGetLedState"},
    {.name = "nvmlUnitGetPsuInfo"},
    {.name = "nvmlUnitGetTemperature"},
    {.name = "nvmlUnitGetUnitInfo"},
    {.name = "nvmlUnitSetLedState"},
    {.name = "nvmlVgpuInstanceGetEncoderCapacity"},
    {.name = "nvmlVgpuInstanceGetEncoderSessions"},
    {.name = "nvmlVgpuInstanceGetEncoderStats"},
    {.name = "nvmlVgpuInstanceGetFbUsage"},
    {.name = "nvmlVgpuInstanceGetFrameRateLimit"},
    {.name = "nvmlVgpuInstanceGetLicenseStatus"},
    {.name = "nvmlVgpuInstanceGetMetadata"},
    {.name = "nvmlVgpuInstanceGetType"},
    {.name = "nvmlVgpuInstanceGetUUID"},
    {.name = "nvmlVgpuInstanceGetVmDriverVersion"},
    {.name = "nvmlVgpuInstanceGetVmID"},
    {.name = "nvmlVgpuInstanceSetEncoderCapacity"},
    {.name = "nvmlVgpuTypeGetClass"},
    {.name = "nvmlVgpuTypeGetDeviceID"},
    {.name = "nvmlVgpuTypeGetFramebufferSize"},
    {.name = "nvmlVgpuTypeGetFrameRateLimit"},
    {.name = "nvmlVgpuTypeGetLicense"},
    {.name = "nvmlVgpuTypeGetMaxInstances"},
    {.name = "nvmlVgpuTypeGetName"},
    {.name = "nvmlVgpuTypeGetNumDisplayHeads"},
    {.name = "nvmlVgpuTypeGetResolution"},
    {.name = "nvmlDeviceGetFBCSessions"},
    {.name = "nvmlDeviceGetFBCStats"},
    {.name = "nvmlDeviceGetGridLicensableFeatures_v2"},
    {.name = "nvmlDeviceGetRetiredPages_v2"},
    {.name = "nvmlDeviceResetGpuLockedClocks"},
    {.name = "nvmlDeviceSetGpuLockedClocks"},
    {.name = "nvmlVgpuInstanceGetAccountingMode"},
    {.name = "nvmlVgpuInstanceGetAccountingPids"},
    {.name = "nvmlVgpuInstanceGetAccountingStats"},
    {.name = "nvmlVgpuInstanceGetFBCSessions"},
    {.name = "nvmlVgpuInstanceGetFBCStats"},
    {.name = "nvmlVgpuTypeGetMaxInstancesPerVm"},
    {.name = "nvmlGetVgpuVersion"},
    {.name = "nvmlSetVgpuVersion"},
    {.name = "nvmlDeviceGetGridLicensableFeatures_v3"},
    {.name = "nvmlDeviceGetHostVgpuMode"},
    {.name = "nvmlDeviceGetPgpuMetadataString"},
    {.name = "nvmlVgpuInstanceGetEccMode"},
    {.name = "nvmlComputeInstanceDestroy"},
    {.name = "nvmlComputeInstanceGetInfo"},
    {.name = "nvmlDeviceCreateGpuInstance"},
    {.name = "nvmlDeviceGetArchitecture"},
    {.name = "nvmlDeviceGetAttributes"},
    {.name = "nvmlDeviceGetAttributes_v2"},
    {.name = "nvmlDeviceGetComputeInstanceId"},
    {.name = "nvmlDeviceGetCpuAffinityWithinScope"},
    {.name = "nvmlDeviceGetDeviceHandleFromMigDeviceHandle"},
    {.name = "nvmlDeviceGetGpuInstanceById"},
    {.name = "nvmlDeviceGetGpuInstanceId"},
    {.name = "nvmlDeviceGetGpuInstancePossiblePlacements"},
    {.name = "nvmlDeviceGetGpuInstanceProfileInfo"},
    {.name = "nvmlDeviceGetGpuInstanceRemainingCapacity"},
    {.name = "nvmlDeviceGetGpuInstances"},
    {.name = "nvmlDeviceGetMaxMigDeviceCount"},
    {.name = "nvmlDeviceGetMemoryAffinity"},
    {.name = "nvmlDeviceGetMigDeviceHandleByIndex"},
    {.name = "nvmlDeviceGetMigMode"},
    {.name = "nvmlDeviceGetRemappedRows"},
    {.name = "nvmlDeviceGetRowRemapperHistogram"},
    {.name = "nvmlDeviceIsMigDeviceHandle"},
    {.name = "nvmlDeviceSetMigMode"},
    {.name = "nvmlEventSetWait_v2"},
    {.name = "nvmlGpuInstanceCreateComputeInstance"},
    {.name = "nvmlGpuInstanceDestroy"},
    {.name = "nvmlGpuInstanceGetComputeInstanceById"},
    {.name = "nvmlGpuInstanceGetComputeInstanceProfileInfo"},
    {.name = "nvmlGpuInstanceGetComputeInstanceRemainingCapacity"},
    {.name = "nvmlGpuInstanceGetComputeInstances"},
    {.name = "nvmlGpuInstanceGetInfo"},
    {.name = "nvmlVgpuInstanceClearAccountingPids"},
    {.name = "nvmlVgpuInstanceGetMdevUUID"},
    {.name = "nvmlComputeInstanceGetInfo_v2"},
    {.name = "nvmlDeviceGetComputeRunningProcesses_v2"},
    {.name = "nvmlDeviceGetGraphicsRunningProcesses_v2"},
    {.name = "nvmlDeviceGetComputeRunningProcesses_v3"},
    {.name = "nvmlDeviceGetGraphicsRunningProcesses_v3"},
    {.name = "nvmlDeviceGetMPSComputeRunningProcesses_v3"},
    {.name = "nvmlDeviceGetMPSComputeRunningProcesses_v2"},
    {.name = "nvmlDeviceSetTemperatureThreshold"},
    //{.name = "nvmlRetry_NvRmControl"},
    {.name = "nvmlVgpuInstanceGetGpuInstanceId"},
    {.name = "nvmlVgpuTypeGetGpuInstanceProfileId"},
};

pthread_once_t init_virtual_map_pre_flag = PTHREAD_ONCE_INIT;
pthread_once_t init_virtual_map_post_flag = PTHREAD_ONCE_INIT;

typedef void* (*fp_dlsym)(void*, const char*);
extern fp_dlsym real_dlsym;
extern int virtual_nvml_devices;
extern int cuda_to_nvml_map_array[CUDA_DEVICE_MAX_COUNT];

nvmlReturn_t nvmlDeviceGetIndex(nvmlDevice_t device, unsigned int *index) {
    return NVML_OVERRIDE_CALL(nvml_library_entry, nvmlDeviceGetIndex, device, index);
}


void load_nvml_libraries() {
    void *table = NULL;
    char driver_filename[FILENAME_MAX];

    if (real_dlsym == NULL) {
        real_dlsym = dlvsym(RTLD_NEXT,"dlsym","GLIBC_2.2.5");
        if (real_dlsym == NULL) {
            void *libc_handle = dlopen("libc.so.6", RTLD_LAZY);
            if (libc_handle) {
                real_dlsym = dlsym(libc_handle, "dlsym");
            }
            if (real_dlsym == NULL)
                LOG_ERROR("real dlsym not found");
        }
    }
    snprintf(driver_filename, FILENAME_MAX - 1, "%s", "libnvidia-ml.so.1");
    driver_filename[FILENAME_MAX - 1] = '\0';

    table = dlopen(driver_filename, RTLD_NOW | RTLD_NODELETE);
    if (!table) {
        LOG_WARN("can't find library %s", driver_filename);  
    }
    int i;
    for (i = 0; i < NVML_ENTRY_END; i++) {
        LOG_DEBUG("loading %s:%d",nvml_library_entry[i].name,i);
        nvml_library_entry[i].fn_ptr = real_dlsym(table, nvml_library_entry[i].name);
        if (!nvml_library_entry[i].fn_ptr) {
            LOG_INFO("can't find function %s in %s", nvml_library_entry[i].name,
                driver_filename);
        }
    }
    LOG_INFO("loaded nvml libraries");
    dlclose(table);
}

void nvml_preInit() {
    ensure_initialized();
    load_env_from_file(ENV_OVERRIDE_FILE);
    load_nvml_libraries();
    for (int i = 0; i < CUDA_DEVICE_MAX_COUNT; i++) {
        cuda_to_nvml_map_array[i] = i;
    }   
}

void nvml_postInit() {
    init_device_info();
}

nvmlReturn_t _nvmlDeviceGetMemoryInfo(nvmlDevice_t device,void* memory,int version) {
    LOG_DEBUG("into nvmlDeviceGetMemoryInfo");
    if (memory == NULL) {
        return NVML_SUCCESS;
    }
    unsigned int dev_id;

    switch (version) {
        case 1:
            CHECK_NVML_API(NVML_OVERRIDE_CALL(nvml_library_entry,nvmlDeviceGetMemoryInfo, device, memory));
            LOG_DEBUG("origin_free=%lld total=%lld\n", ((nvmlMemory_t*)memory)->free, ((nvmlMemory_t*)memory)->total);
            break;
        case 2:
            CHECK_NVML_API(NVML_OVERRIDE_CALL(nvml_library_entry,nvmlDeviceGetMemoryInfo_v2, device, (nvmlMemory_v2_t *)memory));
            LOG_DEBUG("origin_free=%lld total=%lld\n", ((nvmlMemory_v2_t*)memory)->free, ((nvmlMemory_v2_t*)memory)->total);
            break;
        default:
            return NVML_ERROR_INVALID_ARGUMENT;
    }
    CHECK_NVML_API(nvmlDeviceGetIndex(device, &dev_id));
    int cudadev = nvml_to_cuda_map(dev_id);
    if (cudadev < 0) {
        return NVML_SUCCESS;
    }
    size_t usage = get_current_device_memory_usage(cudadev);
    size_t monitor = get_current_device_memory_monitor(cudadev);
    size_t limit = get_current_device_memory_limit(cudadev);
    LOG_DEBUG("usage=%ld limit=%ld monitor=%ld", usage, limit, monitor);
    if (limit == 0) {
        switch (version) {
        case 1:
             ((nvmlMemory_t*)memory)->used = usage;
            return NVML_SUCCESS;
        case 2:
            ((nvmlMemory_v2_t *)memory)->used = usage;
            return NVML_SUCCESS;
        }
    } else {
        switch (version) {
        case 1:
             ((nvmlMemory_t*)memory)->free = (limit-usage);
             ((nvmlMemory_t*)memory)->total = limit;
             ((nvmlMemory_t*)memory)->used = usage;
            return NVML_SUCCESS;
        case 2:
            ((nvmlMemory_v2_t *)memory)->free = (limit-usage);
            ((nvmlMemory_v2_t *)memory)->total = limit;
            ((nvmlMemory_v2_t *)memory)->used = usage;
            return NVML_SUCCESS;
        } 
    }
    return NVML_SUCCESS;
}

nvmlReturn_t nvmlDeviceGetMemoryInfo(nvmlDevice_t device, nvmlMemory_t* memory) {
    return _nvmlDeviceGetMemoryInfo(device,memory,1); 
}

nvmlReturn_t nvmlDeviceGetMemoryInfo_v2(nvmlDevice_t device, nvmlMemory_v2_t* memory) {
    return _nvmlDeviceGetMemoryInfo(device,memory,2);
}


nvmlReturn_t nvmlDeviceGetNvLinkRemotePciInfo_v2 ( nvmlDevice_t device, unsigned int  link, nvmlPciInfo_t* pci ) {
    nvmlReturn_t res = NVML_OVERRIDE_CALL(nvml_library_entry,nvmlDeviceGetNvLinkRemotePciInfo_v2,device,link,pci);
    return res;
}

nvmlReturn_t nvmlDeviceGetNvLinkRemotePciInfo ( nvmlDevice_t device, unsigned int  link, nvmlPciInfo_t* pci ) {
    nvmlReturn_t res = NVML_OVERRIDE_CALL(nvml_library_entry,nvmlDeviceGetNvLinkRemotePciInfo,device,link,pci);
    return res;
}

nvmlReturn_t nvmlDeviceGetHandleByIndex ( unsigned int  index, nvmlDevice_t* device ){
    nvmlReturn_t res;
    LOG_DEBUG("nvmlDeviceGetHandleByIndex index=%u",index); 
    res = NVML_OVERRIDE_CALL_NO_LOG(nvml_library_entry,nvmlDeviceGetHandleByIndex,index,device);
    return res;
}

nvmlReturn_t nvmlDeviceGetHandleByIndex_v2 ( unsigned int  index, nvmlDevice_t* device ){
    nvmlReturn_t res;
    LOG_DEBUG("nvmlDeviceGetHandleByIndex_v2 index=%u",index); 
    res = NVML_OVERRIDE_CALL_NO_LOG(nvml_library_entry,nvmlDeviceGetHandleByIndex_v2,index,device);
    return res;
}

nvmlReturn_t nvmlDeviceGetHandleByPciBusId_v2 ( const char* pciBusId, nvmlDevice_t* device ) {
    LOG_INFO("NVML DeviceGetHandleByPciBusID_v2 %s",pciBusId);
    return NVML_OVERRIDE_CALL(nvml_library_entry,nvmlDeviceGetHandleByPciBusId_v2,pciBusId,device);
}


nvmlReturn_t nvmlDeviceGetHandleByPciBusId(const char *pciBusId,
                                           nvmlDevice_t *device) {
    LOG_DEBUG("NVML DeviceGetHandleByPciBusId %s",pciBusId);
    return NVML_OVERRIDE_CALL(nvml_library_entry, nvmlDeviceGetHandleByPciBusId,
                         pciBusId, device);
}

nvmlReturn_t nvmlDeviceGetHandleBySerial ( const char* serial, nvmlDevice_t* device ) {
    LOG_INFO("NVML DeviceGetHandleBySerial Not supported %s",serial);
    return NVML_OVERRIDE_CALL(nvml_library_entry,nvmlDeviceGetHandleBySerial,serial,device);
}

nvmlReturn_t nvmlDeviceGetHandleByUUID ( const char* uuid, nvmlDevice_t* device ) {
    nvmlReturn_t res = NVML_OVERRIDE_CALL(nvml_library_entry,nvmlDeviceGetHandleByUUID,uuid,device);
    return res;
}

nvmlReturn_t nvmlDeviceGetCount ( unsigned int* deviceCount ) {
    return NVML_OVERRIDE_CALL(nvml_library_entry,nvmlDeviceGetCount_v2,deviceCount);
}

nvmlReturn_t nvmlDeviceGetCount_v2 ( unsigned int* deviceCount ) {
    return NVML_OVERRIDE_CALL(nvml_library_entry,nvmlDeviceGetCount_v2,deviceCount);
}

nvmlReturn_t nvmlInitWithFlags( unsigned int  flags ) {
    LOG_DEBUG("nvmlInitWithFlags")
    pthread_once(&init_virtual_map_pre_flag, (void(*) (void))nvml_preInit);
    nvmlReturn_t res =  NVML_OVERRIDE_CALL(nvml_library_entry, nvmlInitWithFlags,flags);
    pthread_once(&init_virtual_map_post_flag,(void (*)(void))nvml_postInit);
    return res;
}

nvmlReturn_t nvmlInit(void) {
    LOG_DEBUG("nvmlInit")
    pthread_once(&init_virtual_map_pre_flag,(void (*)(void))nvml_preInit);
    nvmlReturn_t res = NVML_OVERRIDE_CALL(nvml_library_entry, nvmlInit_v2);
    pthread_once(&init_virtual_map_post_flag,(void (*)(void))nvml_postInit);
    return res;
}

nvmlReturn_t nvmlInit_v2(void) {
    LOG_DEBUG("nvmlInit_v2");
    pthread_once(&init_virtual_map_pre_flag,(void (*)(void))nvml_preInit);
    nvmlReturn_t res = NVML_OVERRIDE_CALL(nvml_library_entry, nvmlInit_v2);
    pthread_once(&init_virtual_map_post_flag,(void (*)(void))nvml_postInit);
    return res;
}

nvmlReturn_t nvmlDeviceGetPciInfo_v3(nvmlDevice_t device, nvmlPciInfo_t *pci) {
  nvmlReturn_t res = NVML_OVERRIDE_CALL(nvml_library_entry, nvmlDeviceGetPciInfo_v3, device,
                         pci);
  return res;
}

nvmlReturn_t nvmlDeviceGetPciInfo_v2(nvmlDevice_t device, nvmlPciInfo_t *pci) {
  nvmlReturn_t res = NVML_OVERRIDE_CALL(nvml_library_entry, nvmlDeviceGetPciInfo_v2, device,
                         pci);
  return res;
}

nvmlReturn_t nvmlDeviceGetPciInfo(nvmlDevice_t device, nvmlPciInfo_t *pci) {
  nvmlReturn_t res =  NVML_OVERRIDE_CALL(nvml_library_entry, nvmlDeviceGetPciInfo, device, pci);
  return res;
}

nvmlReturn_t nvmlDeviceGetUUID(nvmlDevice_t device, char *uuid,
                               unsigned int length) {
    nvmlReturn_t res = NVML_OVERRIDE_CALL(nvml_library_entry, nvmlDeviceGetUUID, device, uuid,
                         length);
    return res;
}


nvmlReturn_t nvmlDeviceGetHandleByUUIDV(const nvmlUUID_t *uuid, nvmlDevice_t *device) {
    if (uuid == NULL || device == NULL) return NVML_ERROR_INVALID_ARGUMENT;
    pthread_once(&init_virtual_map_pre_flag, (void (*)(void))nvml_preInit);
    pthread_once(&init_virtual_map_post_flag, (void (*)(void))nvml_postInit);
    if (uuid->version != (unsigned int)(sizeof(nvmlUUID_v1_t) | (1U << 24)) ||
        (uuid->type != 1U && uuid->type != 2U)) return NVML_ERROR_INVALID_ARGUMENT;
    unsigned int count = virtual_nvml_devices > 0 ? (unsigned int)virtual_nvml_devices : 0U;
    for (unsigned int i = 0; i < count; ++i) {
        nvmlDevice_t candidate;
        nvmlUUID_t candidateUuid = {0};
        if (NVML_OVERRIDE_CALL(nvml_library_entry, nvmlDeviceGetHandleByIndex_v2, cuda_to_nvml_map(i), &candidate) == NVML_SUCCESS &&
            NVML_OVERRIDE_CALL(nvml_library_entry, nvmlDeviceGetUUID, candidate, candidateUuid.value.str, sizeof(candidateUuid.value.str)) == NVML_SUCCESS &&
            ((uuid->type == 1U && strncmp(uuid->value.str, candidateUuid.value.str, NVML_DEVICE_UUID_ASCII_LEN) == 0) ||
             (uuid->type == 2U && memcmp(uuid->value.bytes, candidateUuid.value.bytes, NVML_DEVICE_UUID_BINARY_LEN) == 0))) { *device = candidate; return NVML_SUCCESS; }
    }
    return NVML_ERROR_NOT_FOUND;
}
nvmlReturn_t nvmlDeviceGetRunningProcessDetailList(nvmlDevice_t device, nvmlProcessDetailList_t *plist) { /* Process namespace/filtering is not implemented. */ return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetAccountingStats_v2(nvmlDevice_t device, nvmlAccountingStats_v2_t *stats) { /* Accounting virtualization cannot populate v2 safely. */ return NVML_ERROR_NOT_SUPPORTED; }

nvmlReturn_t nvmlDeviceGetProcessesUtilizationInfo(nvmlDevice_t device, nvmlProcessesUtilizationInfo_t *processesUtilInfo) { (void)device; (void)processesUtilInfo; return NVML_ERROR_NOT_SUPPORTED; }

/* NVML 13 additions: conservative virtualization policy. */
nvmlReturn_t nvmlDeviceGetGpuFabricInfoV(nvmlDevice_t device, nvmlGpuFabricInfoV_t *gpuFabricInfo) { (void)device; (void)gpuFabricInfo; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetClockOffsets(nvmlDevice_t device, nvmlClockOffset_t *info) { (void)device; (void)info; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceSetClockOffsets(nvmlDevice_t device, nvmlClockOffset_t *info) { (void)device; (void)info; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetCapabilities(nvmlDevice_t device, nvmlDeviceCapabilities_t *caps) { (void)device; (void)caps; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlSystemGetDriverBranch(nvmlSystemDriverBranchInfo_t *branchInfo, unsigned int length) { return NVML_OVERRIDE_CALL(nvml_library_entry, nvmlSystemGetDriverBranch, branchInfo, length); }
nvmlReturn_t nvmlDeviceGetAddressingMode(nvmlDevice_t device, nvmlDeviceAddressingMode_t *mode) { (void)device; (void)mode; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetRepairStatus(nvmlDevice_t device, nvmlRepairStatus_t *repairStatus) { (void)device; (void)repairStatus; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetPdi(nvmlDevice_t device, nvmlPdi_t *pdi) { (void)device; (void)pdi; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetNvLinkInfo(nvmlDevice_t device, nvmlNvLinkInfo_t *info) { (void)device; (void)info; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceReadWritePRM_v1(nvmlDevice_t device, nvmlPRMTLV_v1_t *buffer) { (void)device; (void)buffer; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetGpuInstanceProfileInfoByIdV(nvmlDevice_t device, unsigned int profileId, nvmlGpuInstanceProfileInfo_v2_t *info) { (void)device; (void)profileId; (void)info; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlSystemGetCPER_v1(nvmlGetCPER_v1_t *cper) { (void)cper; return NVML_ERROR_NOT_SUPPORTED; }

/* CUDA 13.1/13.2 additions: conservative virtualization policy. */
nvmlReturn_t nvmlDeviceGetPowerMizerMode_v1(nvmlDevice_t device, nvmlDevicePowerMizerModes_v1_t *mode) { (void)device; (void)mode; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceSetPowerMizerMode_v1(nvmlDevice_t device, nvmlDevicePowerMizerModes_v1_t *mode) { (void)device; (void)mode; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceSetHostname_v1(nvmlDevice_t device, nvmlHostname_v1_t *hostname) { (void)device; (void)hostname; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetHostname_v1(nvmlDevice_t device, nvmlHostname_v1_t *hostname) { (void)device; (void)hostname; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetSramUniqueUncorrectedEccErrorCounts(nvmlDevice_t device, nvmlEccSramUniqueUncorrectedErrorCounts_t *counts) { (void)device; (void)counts; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceWorkloadPowerProfileUpdateProfiles_v1(nvmlDevice_t device, nvmlWorkloadPowerProfileUpdateProfiles_v1_t *profiles) { (void)device; (void)profiles; return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceSetRusdSettings_v1(nvmlDevice_t device, nvmlRusdSettings_v1_t *settings) { (void)device; (void)settings; return NVML_ERROR_NOT_SUPPORTED; }

/* API 13 vGPU, PRM, power, and system-event APIs expose global or privileged state. */
nvmlReturn_t nvmlDeviceGetBBXTimeData_v1(nvmlDevice_t device, nvmlBBXTimeData_v1_t *timeData) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetCurrentClocksEventReasons(nvmlDevice_t device, unsigned long long *clocksEventReasons) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetRemappedRows_v2(nvmlDevice_t device, nvmlRemappedRowsInfo_v2_t *info) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetSupportedClocksEventReasons(nvmlDevice_t device, unsigned long long *supportedClocksEventReasons) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetUnrepairableMemoryFlag_v1(nvmlDevice_t device, nvmlUnrepairableMemoryStatus_v1_t *unrepairableMemoryStatus) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetVgpuCapabilities(nvmlDevice_t device, nvmlDeviceVgpuCapability_t capability, unsigned int *capResult) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetVgpuHeterogeneousMode(nvmlDevice_t device, nvmlVgpuHeterogeneousMode_t *pHeterogeneousMode) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetVgpuSchedulerLog(nvmlDevice_t device, nvmlVgpuSchedulerLog_t *pSchedulerLog) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetVgpuSchedulerLog_v2(nvmlDevice_t device, nvmlVgpuSchedulerLogInfo_v2_t *pSchedulerLogInfo) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetVgpuSchedulerState(nvmlDevice_t device, nvmlVgpuSchedulerGetState_t *pSchedulerState) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetVgpuSchedulerState_v2(nvmlDevice_t device, nvmlVgpuSchedulerStateInfo_v2_t *pSchedulerStateInfo) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceGetVgpuTypeCreatablePlacements(nvmlDevice_t device, nvmlVgpuTypeId_t vgpuTypeId, nvmlVgpuPlacementList_t *pPlacementList) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceReadPRMCounters_v1(nvmlDevice_t device, nvmlPRMCounterList_v1_t *counterList) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceSetPowerManagementLimit_v2(nvmlDevice_t device, nvmlPowerValue_v2_t *powerValue) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceSetVgpuCapabilities(nvmlDevice_t device, nvmlDeviceVgpuCapability_t capability, nvmlEnableState_t state) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceSetVgpuHeterogeneousMode(nvmlDevice_t device, const nvmlVgpuHeterogeneousMode_t *pHeterogeneousMode) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceSetVgpuSchedulerState(nvmlDevice_t device, nvmlVgpuSchedulerSetState_t *pSchedulerState) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceSetVgpuSchedulerState_v2(nvmlDevice_t device, nvmlVgpuSchedulerState_v2_t *pSchedulerState) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceVgpuForceGspUnload(nvmlDevice_t device) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceWorkloadPowerProfileClearRequestedProfiles(nvmlDevice_t device, nvmlWorkloadPowerProfileRequestedProfiles_t *requestedProfiles) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlDeviceWorkloadPowerProfileSetRequestedProfiles(nvmlDevice_t device, nvmlWorkloadPowerProfileRequestedProfiles_t *requestedProfiles) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlGpuInstanceGetActiveVgpus(nvmlGpuInstance_t gpuInstance, nvmlActiveVgpuInstanceInfo_t *pVgpuInstanceInfo) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlGpuInstanceGetCreatableVgpus(nvmlGpuInstance_t gpuInstance, nvmlVgpuTypeIdInfo_t *pVgpus) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlGpuInstanceGetVgpuHeterogeneousMode(nvmlGpuInstance_t gpuInstance, nvmlVgpuHeterogeneousMode_t *pHeterogeneousMode) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlGpuInstanceGetVgpuSchedulerLog(nvmlGpuInstance_t gpuInstance, nvmlVgpuSchedulerLogInfo_t *pSchedulerLogInfo) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlGpuInstanceGetVgpuSchedulerLog_v2(nvmlGpuInstance_t gpuInstance, nvmlVgpuSchedulerLogInfo_v2_t *pSchedulerLogInfo) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlGpuInstanceGetVgpuSchedulerState(nvmlGpuInstance_t gpuInstance, nvmlVgpuSchedulerStateInfo_t *pSchedulerStateInfo) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlGpuInstanceGetVgpuSchedulerState_v2(nvmlGpuInstance_t gpuInstance, nvmlVgpuSchedulerStateInfo_v2_t *pSchedulerStateInfo) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlGpuInstanceGetVgpuTypeCreatablePlacements(nvmlGpuInstance_t gpuInstance, nvmlVgpuCreatablePlacementInfo_t *pCreatablePlacementInfo) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlGpuInstanceSetVgpuHeterogeneousMode(nvmlGpuInstance_t gpuInstance, const nvmlVgpuHeterogeneousMode_t *pHeterogeneousMode) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlGpuInstanceSetVgpuSchedulerState(nvmlGpuInstance_t gpuInstance, nvmlVgpuSchedulerState_t *pScheduler) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlGpuInstanceSetVgpuSchedulerState_v2(nvmlGpuInstance_t gpuInstance, nvmlVgpuSchedulerState_v2_t *pSchedulerState) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlSystemEventSetCreate(nvmlSystemEventSetCreateRequest_t *request) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlSystemEventSetFree(nvmlSystemEventSetFreeRequest_t *request) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlSystemEventSetWait(nvmlSystemEventSetWaitRequest_t *request) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlSystemRegisterEvents(nvmlSystemRegisterEventRequest_t *request) { return NVML_ERROR_NOT_SUPPORTED; }
nvmlReturn_t nvmlVgpuTypeGetMaxInstancesPerGpuInstance(nvmlVgpuTypeMaxInstance_t *pMaxInstance) { return NVML_ERROR_NOT_SUPPORTED; }
