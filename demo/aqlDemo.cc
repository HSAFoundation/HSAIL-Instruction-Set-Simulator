//===- aqlDemo.cc ---------------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <iostream>

#include <cassert>
#include <cstdlib>
#include <cstring>

#include <stdint.h>

// #include "copy256_brig_object.h"
#include "newcore.h"

#define STR(X) #X
#define XSTR(X) STR(X)

struct HsaBrigObject {
  HsaBrig *brig;
  HsaBrigDirectiveOffset kernel_directive;
} object, *__hsa_brig_object = &object;

int main(int argc, char** argv) {

  //
  // Discover available Hsa Devices.
  //
  HsaStatus status;
  unsigned int dev_count;
  const HsaDevice *device = NULL;
  const HsaDevice *device_list = NULL;

  status = HsaGetDevices(&dev_count, &device_list);
  assert(status == kHsaStatusSuccess && dev_count != 0);
  device = &device_list[0];

  HsaBrig brig;
  __hsa_brig_object->brig = &brig;
  status = HsaCreateBrig(XSTR(OBJ_PATH) "/aqlDemo.o", &brig);
  assert(status == kHsaStatusSuccess);

  HsaAQLKernel kernel;
  status = HsaGetKernelOffset("aqlDemo", &brig,
                              &__hsa_brig_object->kernel_directive);
  assert(status == kHsaStatusSuccess);

  //
  // In this example program the BRIG object is compiled into the
  // executable via header file copy256_brig_object.h which contains
  // the BRIG object as a "char" array.
  //
  // The variables __hsa_brig_object
  // represent the kernel and its size in HSAIL.
  // There is only one kernel in this example.
  //
  // Compiling / Finalizing the HSAIL binary (BRIG) to Hsa Device ISA
  // is accomplished by calling the Finalizer Api.
  //
  // @note: The kernel being finalized does a vector copy of 256 words.
  //

  status = HsaFinalizeHsail(device, __hsa_brig_object->brig,
                            __hsa_brig_object->kernel_directive, NULL, NULL,
                            NULL, &kernel);
  assert(status == kHsaStatusSuccess);

  //
  // Initialize and prepare AQL Dispatch Packet for dispatch
  //
  HsaAqlDispatchPacket aql_packet;
  memset(&aql_packet,0, sizeof(HsaAqlDispatchPacket));

  //
  // Set the kernel_object_address, a group memory, arg, spill and private
  // memory segments with corresponding elements from the compiled device
  // kernel.
  aql_packet.kernel_object_address = kernel.kernel_object_address;
  aql_packet.group_memory_usage = kernel.group_memory_usage;

  //
  // Set launch dimensions and problem size
  //
  uint32_t list_size = 256;
  aql_packet.dimensions = 1;
  aql_packet.nd_range_size.y = 1;
  aql_packet.nd_range_size.z = 1;
  aql_packet.nd_range_size.x = list_size;
  aql_packet.workgroup_size.y = 1;
  aql_packet.workgroup_size.z = 1;
  aql_packet.workgroup_size.x = 16;

  //
  // Set up kernel arguments which includes three parameters. These
  // are src list, dest list and size of list. The elements of the
  // list are specified as 32 bit words.
  //
  void *src_list;
  void *dst_listput;
  uint32_t buff_size;
  buff_size = list_size * sizeof(uint32_t);
  src_list = malloc(buff_size);
  memset(src_list, 0x11235813, buff_size);

  dst_listput = malloc(buff_size);
  memset(dst_listput, 0, buff_size);
  //
  // Allocate a buffer to pass in handles to kernel arguments. The
  // offsets in the buffer where input / output parameters are written
  // is specified by the kernel signature.
  //
  uint32_t num_args;
  size_t argument_size;
  uint64_t *kernel_arguments;

  num_args = 2;
  argument_size = num_args * sizeof(uint64_t);
  kernel_arguments = reinterpret_cast<uint64_t *>(malloc(argument_size));
  kernel_arguments[0] = (uint64_t)src_list;
  kernel_arguments[1] = (uint64_t)dst_listput;
  aql_packet.kernel_args_address = (uint64_t)kernel_arguments;

  //
  // Set-up the notification object for kernel completion
  //
  size_t completion_size;
  HsaCompletionObject *completion;

  completion_size = sizeof(HsaCompletionObject);
  completion = reinterpret_cast<HsaCompletionObject *>(malloc(completion_size));
  memset(completion, 0, sizeof(HsaCompletionObject));
  aql_packet.completion_object_address = (uint64_t)completion;

  //
  // Create a user-level queue, in host memory to submit kernel
  // for execution (dispatch)
  //
  HsaQueue *queue;
  status = HsaCreateUserModeQueue(device, NULL, 0,
                                  kHsaQueueTypeCompute,
                                  kHsaQueuePriorityMaximum,
                                  kHsaQueueFractionTen, &queue);
  assert(status == kHsaStatusSuccess);

  //
  // Launch the kernel by copying the populated AQL packet into the queue
  //
  //
  // Simple example that shows how to write an AQL packet to the queue
  // HsaWriteAQLPacket copies the packet to the kernel in a thread safe way
  // and does necessary wraparounds. User can just do a memcpy if they
  // chose to deal with wraparounds
  //
  status = HsaWriteAQLPacket(queue, &aql_packet);
  assert(status == kHsaStatusSuccess);

  //
  // Wait on the sync condition to be satisfied
  //
  status = HsaWaitCompletion(completion);
  assert(status == kHsaStatusSuccess);

  //
  // validate the output list is as expected
  //
  uint32_t ret_code;
  if (memcmp(src_list, dst_listput, list_size) != 0) {
    ret_code = 3;
    std::cout << "Failed" << std::endl;
  } else {
    ret_code = 0;
    std::cout << "Passed!" << std::endl;
  }

  //
  // Release the user level qeueu object used in dispatch
  //
  status = HsaDestroyUserModeQueue(queue);
  assert(status == kHsaStatusSuccess);

  //
  // Clean up by de-allocating the various
  // memory resources used in dispatch
  //
  free(kernel_arguments);

  free(src_list);

  free(dst_listput);

  free(completion);

  HsaDestroyBrig(&brig);

  return ret_code;
}
