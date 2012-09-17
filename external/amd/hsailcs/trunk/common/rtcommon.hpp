#ifndef RT_COMMON_HPP_
#define RT_COMMON_HPP_

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include "hsa.h"

char * ReadFile(std::string source_filename, size_t& size);
int SetupHSA(hsa::IRuntimeApi ** hsaruntime);
int GetHSADevices(hsa::IRuntimeApi* hsaruntime, uint32_t * numDevices, hsa::vector<hsa::IDevice*> * pDevices);
int MakeHSAProgram(hsa::IRuntimeApi* hsaruntime, hsa::vector<hsa::IDevice*> * pDevices, char * HSAIL_Path, hsa::IProgram ** program);
void CloseHSA();

#endif