#ifndef OVERVIEW_NDRANGES_WORKGROUPS_AND_WORKITEMS_HPP_
#define OVERVIEW_NDRANGES_WORKGROUPS_AND_WORKITEMS_HPP_

#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#define NDRANGE_SIZE_X	16
#define NDRANGE_SIZE_Y	16
#define NDRANGE_SIZE_Z	16

#define NUM_KERNEL_CALLS	1

static char * HSAIL_Path = "Floating-PointSpecialFunctions.bin";

bool TestFailed = false;

hsa::IRuntimeApi* hsaruntime;
uint32_t numDevices;
hsa::vector<hsa::IDevice*> pDevices;
hsa::IProgram * program;

#endif
