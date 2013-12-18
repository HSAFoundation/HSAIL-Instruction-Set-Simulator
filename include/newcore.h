//===- newcore.h ----------------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef NEWCORE_H
#define NEWCORE_H

#include <cstdlib>

#include <stdint.h>

extern "C" {

#define HSACORE_CPU_SIBLINGS 256

  enum HsaStatus {
    kHsaStatusSuccess = 0,
    kHsaStatusTimedOut = 1,
    kHsaStatusInvalidArgument = -1,
    kHsaStatusOutOfResources = -2,
    kHsaStatusResourceUnavailable = -3,
    kHsaStatusInvalidResource = -4,
    kHsaStatusUnsupportedOperation = -5,
    kHsaStatusInvalidOperation = -6,
    kHsaStatusBuildFailed = -7,
    kHsaStatusError = -15,
    kHsaStatusNotImplemented = -16,
    kHsaStatusNotInitialized = -17,
    kHsaStatusInitFailed = -18,
    kHsaStatusMisaligned = -19,
    kHsaStatusRetry = -22,
    kHsaStatusOutOfMemory = -23
  };

  enum HsaHeapType {
    kHsaHeapTypeSystem = 0,
    kHsaHeapTypePrivate = 2,
    kHsaHeapTypeNumHeapTypes,
    kHsaHeapTypeSize = 0xFFFFFFFF
  };

  enum HsaDeviceType { kHsaDeviceTypeThroughput = 1 };

  enum HsaCacheLevel {
    kHsaCacheLevelL1Cache = 0x1,
    kHsaCacheLevelL2Cache = 0x2,
    kHsaCacheLevelConstantCache = 0x4,
    kHsaCacheLevelTextureCache = 0x8
  };

  enum HsaSignalCondition {
    kHsaSignalConditionDebug,
    kHsaSignalConditionSyscall
  };

  enum HsaQueueType {
    kHsaQueueTypeCompute = 0,
    kHsaQueueTypeXXX = 1
  };

  enum HsaQueuePriority {
    kHsaQueuePriorityMinimum = -3,
    kHsaQueuePriorityLow = -2,
    kHsaQueuePriorityBelowNormal = -1,
    kHsaQueuePriorityNormal = 0,
    kHsaQueuePriorityAboveNormal = 1,
    kHsaQueuePriorityHigh = 2,
    kHsaQueuePriorityMaximum = 3
  };

  enum HsaQueueFraction {
    kHsaQueueFractionOne = 10,
    kHsaQueueFractionTwo = 20,
    kHsaQueueFractionThree = 30,
    kHsaQueueFractionFour = 40,
    kHsaQueueFractionFive = 50,
    kHsaQueueFractionSix = 60,
    kHsaQueueFractionSeven = 70,
    kHsaQueueFractionEight = 80,
    kHsaQueueFractionNine = 90,
    kHsaQueueFractionTen = 100
  };

  typedef void HsaQueueId;
  typedef void HsaSyscallArguments;
  typedef uint32_t HsaBrigDirectiveOffset;
  typedef void *HsaFinalizerContext;
  typedef uint64_t HsaSignal;
  typedef HsaStatus(*HsaFindSymbolInMap)(HsaFinalizerContext context,
                                         HsaBrigDirectiveOffset symbol_directive,
                                         uint64_t *offset);

  struct HsaDim3 {
    uint32_t x;
    uint32_t y;
    uint32_t z;
  };

  union HsaCoreCacheType {
    uint32_t value;
    struct {
      unsigned int data:1;
      unsigned int instruction:1;
      unsigned int cpu:1;
      unsigned int hsa_cu:1;
      unsigned int reserved:28;
    } ui32;
  };


  struct HsaCacheDescriptor {
    uint32_t processor_id_low;
    HsaCacheLevel cache_level;
    uint32_t cache_size;
    uint32_t cache_line_size;
    uint32_t cache_lines_per_tag;
    uint32_t cache_associativity;
    uint32_t cache_latency;
    HsaCoreCacheType cache_type;
    uint32_t sibling_map [HSACORE_CPU_SIBLINGS];
  };

  union HsaMemoryProperty {
    uint32_t memory_property;
    struct {
      unsigned int hot_pluggable:1;
      unsigned int non_volatile:1;
      unsigned int reserved:30;
    } ui32;
  };

  struct HsaMemoryDescriptor {
    HsaHeapType heap_type;
    union {
      uint64_t size_in_bytes;
      struct {
        uint32_t size_in_bytes_low;
        uint32_t size_in_bytes_high;
      } ui32;
    };
    HsaMemoryProperty flags;
    uint32_t width;
    uint32_t memory_clock_max;
  };

  typedef struct {
    uint32_t full_flag;
    // what caused this execution to signal the agent
    HsaSignalCondition *sig_condition;
    uint32_t *work_item_id; // same as workitemid in hsail
    uint32_t *compute_unit_id; // same as cuid_u32 in hsail
    // pointer to the AQL packet, potentially a pointer to the copy of the
    // original AQL packet
    uint64_t *AQL_packet_ptr;
    uint64_t *virtual_address; // virtual address
    uint64_t *current_program_counter; // program counter at which error occured
    HsaQueueId *queue_id; // queue identifier qid_u32 in hsail
    HsaDim3 *work_group_id; // work group id
    HsaSyscallArguments *sycall_args; // syscall arguments
    uint64_t **syscall_output; // output of syscall
    uint32_t num_entries; // number of entries in each element
  } HsaThreadExecutionInfo;

  typedef struct HsaDevice {
    uint32_t device_id;
    HsaDeviceType device_type;
    uint32_t node_id;
    uint32_t number_compute_units;
    uint32_t max_clock_rate_of_f_compute;
    uint32_t max_clock_rate_of_c_compute;
    uint32_t group_memory_size;
    uint32_t wave_front_size;
    uint32_t max_waves_per_cu;
    uint32_t number_memory_descriptors;
    HsaMemoryDescriptor *memory_descriptors;
    uint32_t number_cache_descriptors;
    HsaCacheDescriptor *cache_descriptors;
    uint32_t queue_size;
    uint8_t is_image_support;
    uint8_t is_double_precision;
    HsaThreadExecutionInfo *mailbox;
    uint32_t max_concurrent_wavefronts;
  } HsaDevice;

  struct HsaBrig {
    void *string_section;
    void *directive_section;
    void *code_section;
    void *operand_section;
    void *debug_section;
    uint32_t string_section_byte_size;
    uint32_t directive_section_byte_size;
    uint32_t code_section_byte_size;
    uint32_t operand_section_byte_size;
    uint32_t debug_section_byte_size;
  };

  struct HsaAQLKernel {
    uint64_t kernel_object_address;
    const HsaDevice *device;
    size_t kernel_arg_size_bytes;
    uint32_t group_memory_usage;
    int workitem_private_segment_size_bytes;
    int workitem_arg_segment_size_bytes;
    int workitem_spill_segment_size_bytes;
  };

  struct HsaAqlDispatchPacket {
    uint32_t format:8;
    uint32_t flush_destination_cache:1;
    uint32_t invalidate_source_cache:1;
    uint32_t cache_level_action:1;
    uint32_t interrupt_on_completion:1;
    uint32_t performance_counter_sampling:1;
    uint32_t head_execute_only:1;
    uint32_t sync_function:8;
    uint32_t dimensions:2;
    uint32_t exception_control:10;
    uint64_t kernel_object_address;
    uint64_t completion_object_address;
    uint64_t kernel_args_address;
    HsaDim3 nd_range_size;
    HsaDim3 workgroup_size;
    uint32_t group_memory_usage;
    uint32_t sync_dw0;
    uint32_t sync_dw1;
    uint32_t sync_dw2;
    uint32_t sync_dw3;
    uint32_t sync_dw4;
    uint32_t sync_dw5;
    uint32_t sync_dw6;
    uint32_t sync_dw7;
    uint32_t sync_dw8;
    uint32_t sync_dw9;
    uint64_t dispatch_id;
  };

  struct HsaCompletionObject {
    int32_t status;
    union ExceptionFlagsUnion {
      struct ExceptionFlags {
        uint32_t invalid_operation:1;
        uint32_t divide_by_zero:1;
        uint32_t overflow:1;
        uint32_t underflow:1;
        uint32_t inexact:1;
      } exception_flags;
      uint32_t u32_exception_flags;
    } u;
    uint64_t parse_time_stamp;
    uint64_t dispatch_time_stamp;
    uint64_t completion_time_stamp;
    HsaSignal completion_signal;
  };

  struct HsaQueue {
    uint64_t queue_id;
    const HsaDevice *device;
    uint8_t *base;
    size_t size_bytes;
    volatile uint32_t *cached_write_offset;
    volatile uint32_t *write_offset;
    volatile uint32_t *read_offset;
    volatile uint32_t *doorbell_offset;
    uint64_t dispatch_id;
    void *queue_details;
  };

  // MCW Extensions
  HsaStatus HsaCreateBrig(const char *file, struct HsaBrig *brig);
  HsaStatus HsaDestroyBrig(struct HsaBrig *brig);
  HsaStatus HsaGetKernelOffset(const char *kernelName,
                               const struct HsaBrig *brig,
                               HsaBrigDirectiveOffset *offset);

  // Standard Functions
  HsaStatus HsaGetDevices(unsigned int *count, const HsaDevice **device_list);
  HsaStatus HsaFinalizeHsail(const HsaDevice *device,
                             HsaBrig *brig,
                             HsaBrigDirectiveOffset kernel_directive,
                             const char *options,
                             HsaFinalizerContext context,
                             HsaFindSymbolInMap find_symbol_function,
                             HsaAQLKernel *kernel);
  HsaStatus HsaCreateUserModeQueue(const HsaDevice *device,
                                   void *buffer, size_t buffer_size,
                                   HsaQueueType queue_type,
                                   HsaQueuePriority queue_priority,
                                   HsaQueueFraction queue_fraction,
                                   HsaQueue **queue);
  HsaStatus HsaWriteAQLPacket(HsaQueue *queue,
                              HsaAqlDispatchPacket *aql_packet);
  HsaStatus HsaWaitCompletion(HsaCompletionObject *completion);
  HsaStatus HsaDestroyUserModeQueue(HsaQueue *queue);
}
#endif // NEWCORE_H
