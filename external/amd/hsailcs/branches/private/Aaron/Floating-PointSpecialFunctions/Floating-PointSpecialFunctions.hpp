#ifndef OVERVIEW_NDRANGES_WORKGROUPS_AND_WORKITEMS_HPP_
#define OVERVIEW_NDRANGES_WORKGROUPS_AND_WORKITEMS_HPP_

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "hsa.h"

#define NDRANGE_SIZE_X	16
#define NDRANGE_SIZE_Y	16
#define NDRANGE_SIZE_Z	16

#define NUM_KERNEL_CALLS	1

//char * DLL_Path = "hsa64.dll";
char * DLL_Path = "hsa.dll";
char * HSAIL_Path = "Floating-PointSpecialFunctions.bin";

bool TestFailed = false;

hsa::IRuntimeApi* hsaruntime;
HINSTANCE hinstDll;
uint32_t numDevices;
hsa::vector<hsa::IDevice*>  pDevices;

#endif
