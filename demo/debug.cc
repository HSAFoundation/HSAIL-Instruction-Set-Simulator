//===- debug.cc -----------------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "AMDGPUDebug.h"

#include <cstdio>

static const char *getAPITypeStr(HwDbgAPIType apiType) {
  if (apiType == HWDBG_API_HSA) {
    return "HSA";
  } else if (apiType == HWDBG_API_DIRECTX) {
    return "DirectX";
  } else {
    return "Unknown";
  }
}

int main(int argc, char **argv) {

  unsigned major;
  unsigned minor;
  unsigned build;
  HwDbgStatus status = HwDbgGetAPIVersion(&major, &minor, &build);
  if (status != HWDBG_STATUS_SUCCESS) exit(-1);
  printf("Debug API Verison %u.%u.%u\n", major, minor, build);

  HwDbgAPIType apiType;
  status = HwDbgGetAPIType(&apiType);
  if (status != HWDBG_STATUS_SUCCESS) exit(-1);
  printf("Debug API Type: %s\n", getAPITypeStr(apiType));

  return 0;
}
