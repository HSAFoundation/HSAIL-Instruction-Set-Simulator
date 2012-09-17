
#ifndef VECTOR_COPY_HPP_
#define VECTOR_COPY_HPP_

#include <windows.h>
#include <stdio.h>
#include <stdint.h>

static char * HSAIL_Path = "VectorCopy.bin";

hsa::IRuntimeApi* hsaruntime;
uint32_t numDevices;
hsa::vector<hsa::IDevice*>  pDevices;
hsa::IProgram * program;

#endif
