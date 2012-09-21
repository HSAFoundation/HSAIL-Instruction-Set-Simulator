#ifndef OVERVIEW_NDRANGES_WORKGROUPS_AND_WORKITEMS_HPP_
#define OVERVIEW_NDRANGES_WORKGROUPS_AND_WORKITEMS_HPP_

#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#define NDRANGE_SIZE_X	16
#define NDRANGE_SIZE_Y	16
#define NDRANGE_SIZE_Z	16

#define NUM_KERNEL_CALLS	5

char * HSAIL_Path = "OverviewNDRangesWorkGroupsAndWorkItems.bin";

bool TestFailed = false;
unsigned int DispatchIDs[NUM_KERNEL_CALLS]; 

hsa::IRuntimeApi* hsaruntime;
uint32_t numDevices;
hsa::vector<hsa::IDevice*>  pDevices;
hsa::IProgram * program;

#endif
