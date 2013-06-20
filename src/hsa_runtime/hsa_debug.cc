//===- hsa_debug.cc -------------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "AMDGPUDebug.h"

HwDbgStatus
HwDbgGetAPIVersion(unsigned int* pVersionMajorOut,
                   unsigned int* pVersionMinorOut,
                   unsigned int* pVersionBuildOut) {
  if (pVersionMajorOut) *pVersionMajorOut = AMDGPUDEBUG_VERSION_MAJOR;
  if (pVersionMinorOut) *pVersionMinorOut = AMDGPUDEBUG_VERSION_MINOR;
  if (pVersionBuildOut) *pVersionBuildOut = AMDGPUDEBUG_VERSION_BUILD;
  if (!pVersionMajorOut ||
     !pVersionMinorOut ||
     !pVersionBuildOut) {
    return HWDBG_STATUS_NULL_POINTER;
  }
  return HWDBG_STATUS_SUCCESS;
}

HwDbgStatus
HwDbgGetAPIType(HwDbgAPIType* pAPITypeOut) {
  if (!pAPITypeOut) return HWDBG_STATUS_NULL_POINTER;
  *pAPITypeOut = HWDBG_API_HSA;
  return HWDBG_STATUS_SUCCESS;
}

HwDbgStatus
HwDbgSetLoggingCallback(unsigned int types,
                        AMDLoggingCallback pCallback,
                        void* pUser) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgBeginDebugging(void* pContext,
                    HwDbgContextHandle* pDebugContextOut) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgEndDebugging(HwDbgContextHandle hDebugContext) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgWaitForEvent(const unsigned int timeout,
                  HwDbgContextHandle* pDebugContextOut) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgGetEventType(const HwDbgContextHandle hDebugContext,
                  HwDbgEventType* pEventTypeOut) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgGetShaderBinarySize(const HwDbgContextHandle hDebugContext,
                         size_t* pBinarySizeOut) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgGetShaderBinary(const HwDbgContextHandle hDebugContext,
                     void* pBinaryOut) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgCreateBreakpoint(const HwDbgContextHandle hDebugContext,
                      HwDbgBreakpointHandle* pBreakpointOut) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgDeleteBreakpoint(HwDbgBreakpointHandle hBreakpoint) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgSetBreakpointProgramCounter(const HwDbgBreakpointHandle hBreakpoint,
                                 const size_t pc) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgGetBreakpointProgramCounter(const HwDbgBreakpointHandle hBreakpoint,
                                 size_t* pPcOut) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgSetBreakpointEnabled(const HwDbgBreakpointHandle hBreakpoint,
                          const bool bEnabled) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgGetBreakpointEnabled(const HwDbgBreakpointHandle hBreakpoint,
                          bool* pEnabledOut) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgGetProgramCounter(const HwDbgContextHandle hDebugContext,
                       size_t threadIdX,
                       size_t threadIdY,
                       size_t threadIdZ,
                       size_t* pPcOut) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgGetMemory(const HwDbgContextHandle hDebugContext,
               const HwDbgMemoryType memType,
               const size_t address,
               const size_t count,
               size_t* pMemOut) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgContinueEvent(const HwDbgContextHandle hDebugContext,
                   HwDbgCommand command) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgBreakAll(const HwDbgContextHandle hDebugContext) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgKillAll(const HwDbgContextHandle hDebugContext) {
  return HWDBG_STATUS_UNSUPPORTED;
}

HwDbgStatus
HwDbgTrapAllTemp(const HwDbgContextHandle hDebugContext) {
  return HWDBG_STATUS_UNSUPPORTED;
}
