//===- AMDGPUDebug.h ------------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

/*****************************************************************************//**
** Copyright (c) 2012 Advanced Micro Devices, Inc. All rights reserved.
**
** \author GPU Developer Tools
** \file AMDGPUDebug.h
** \brief  The AMD GPU Debug API definitions to be included
**          by clients to implement the debugging of shader/kernel programs in
**          AMD Southern Islands or later generation GPUs.
**
** \mainpage Introduction
** \section Overview
** This document describes a set of interfaces which can be used to build a
** GPU kernel/shader debugger for AMD Southern Islands or later generation GPUs.
** This API has been designed to hide the multiple driver specific
** implementations and the internal architecture of a particular GPU device.
** This API is intended for two audiences:  for advanced developers
** wishing to incorporate debug capabilities directly into their applications,
** and for 3rd party tools vendors to incorporate GPU debug capabilities into
** their products.  This should be viewed as a living document;
** many of the interfaces and approaches being used are still untested and are
** likely to change once these concepts are further developed.
**
** \section Assumptions
** It is assumed that the API proposed here is an "in-process" debug API.  That is,
** the debugger is running in the same process space as the program being debugged
** and will have direct access to all process resources. No OS provided inter-process
** debug mechanisms are required, but it should be reasonably straightforward for
** tool developers to create a client/server remote debugging model through the
** introduction of a simple communication protocol.
**
** \section Requirements
** The proposed debugger API is a "C" style API. While it may prove expedient
** for the underlying code to be written in C++, we cannot make the assumption
** that all consumers of this API will be written in C++.
**
** There should be no OS or hardware specific terminology used in this API definition.
**
** Extra diagnostics output about the operation of the AMD GPU Debug API may
** be enabled by registering a client callback function through the provided
** API.
*******************************************************************************/
#ifndef _AMDGPUDEBUG_H_
#define _AMDGPUDEBUG_H_

#include <stdlib.h>

// #include "AMDLoggingRegistrationTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/** The AMD GPU Debug API major version */
#define AMDGPUDEBUG_VERSION_MAJOR 0
/** The AMD GPU Debug API minor version */
#define AMDGPUDEBUG_VERSION_MINOR 1
/** The AMD GPU Debug API build number (will be updated automatically) */
#define AMDGPUDEBUG_VERSION_BUILD 0

#if defined(_WIN32) || defined(__CYGWIN__)
    #if defined(HWDBG_EXPORTS)
        /** The specification of the exported functions  **/
        #define HWDBG_API_ENTRY __declspec(dllexport)
    #else
        /** The specification of the exported functions  **/
        #define HWDBG_API_ENTRY __declspec(dllimport)
    #endif
    /** The API calling convention **/
    #define HWDBG_API_CALL __stdcall
#else
    /** The specification of the exported functions  **/
    #define HWDBG_API_ENTRY
    /** The API calling convention **/
    #define HWDBG_API_CALL
#endif

/** The list of possible status values that can be returned by the provided API. */
typedef enum
{
    NA_HWDBG_STATUS = 0,          /**< an invalid status, should never been seen by the caller */
    HWDBG_STATUS_SUCCESS,         /**< the API is executed successfully */
    HWDBG_STATUS_ERROR,           /**< error value: a generic error detected */
    HWDBG_STATUS_NULL_POINTER,    /**< error value: one of the argument value is NULL when non NULL value is expected */
    HWDBG_STATUS_UNSUPPORTED,     /**< error value: the API is not yet supported */
    HWDBG_STATUS_UNDEFINED,       /**< error value: undefined operation typically due to an incorrect calling order */
    HWDBG_STATUS_MAX              /**< the maximum value, should never been seen by the caller */
} HwDbgStatus;

/** The list of debugger commands to be used with the HwDbgContinueEvent API to
    advance to the next state in the GPU debug engine. **/
typedef enum
{
    NA_HWDBG_COMMAND = 0,         /**< an invalid command, should never be used */
    HWDBG_COMMAND_CONTINUE,       /**< run to the next state (run to the next breakpoint) */
    // HWDBG_COMMAND_STEP,           /**< run to the next ISA line */
    HWDBG_COMMAND_MAX             /**< the maximum value, should never be used */
} HwDbgCommand;

/** The list of supported driver software stack types supported by the library. */
typedef enum
{
    NA_HWDBG_API = 0,             /**< an invalid API type, should never been seen by the caller */
    HWDBG_API_DIRECTX,            /**< the library supports GPU debugging through the DirectX software stack */
    HWDBG_API_HSA,                /**< the library supports GPU debugging through the HSA driver software stack */
    HWDBG_API_MAX                 /**< the maximum value, should never been seen by the caller */
} HwDbgAPIType;

/** The list of event values returned by the HwDbgGetEventType API. */
typedef enum
{
    NA_HWDBG_EVENT = 0,           /**< an invalid event, should never been seen by the caller */
    HWDBG_EVENT_BEGIN_DEBUGGING,  /**< an event received after the HwDbgStartDebugging API is performed */
    HWDBG_EVENT_END_DEBUGGING,    /**< an event received after the HwDbgEndDebugging API is performed */
    HWDBG_EVENT_POST_STEP,        /**< an event received after the state machine executes a step operation */
    HWDBG_EVENT_POST_BREAKPOINT,  /**< an event received after the state machine executes a run to breakpoint operation */
    HWDBG_EVENT_MAX               /**< the maximum value, should never been seen by the caller */
} HwDbgEventType;

/** the list of supported memory types to be used with the HwDbgGetMemoryType API. */
typedef enum
{
    NA_HWDBG_MEM = 0,             /**< an invalid memory type, should never be used */
    HWDBG_MEM_SCRATCH,            /**< access the scratch memory contents */
    HWDBG_MEM_UAV,                /**< access the uav memory contents */
    HWDBG_MEM_MAX                 /**< the maximum value, should never be used */
} HwDbgMemoryType;

/** A unique handle for the shader debug context (returned by HwBeginDebugging and HwDbgWaitForEvent). */
typedef void* HwDbgContextHandle;

/** A unique handle for a breakpoint (returned by HwDbgCreateBreakpoint). */
typedef void* HwDbgBreakpointHandle;

/***************************************************************************//**
** Get the library version (major, minor and build) number.
**
** \param[out] pVersionMajorOut           The API version major number.
** \param[out] pVersionMinorOut           The API version minor number.
** \param[out] pVersionBuildOut           The API build number.
**
** \return HWDBG_STATUS_SUCCESS       On success.
** \return HWDBG_STATUS_NULL_POINTER  If one of the input argument is NULL.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgGetAPIVersion(unsigned int* pVersionMajorOut,
                   unsigned int* pVersionMinorOut,
                   unsigned int* pVersionBuildOut);

/***************************************************************************//**
** Get the API type of the loaded library (DirectX or HSA).
**
** \param[out] pAPITypeOut                The API type of the library.
**
** \return HWDBG_STATUS_SUCCESS       On success.
** \return HWDBG_STATUS_NULL_POINTER  If the input argument is NULL.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgGetAPIType(HwDbgAPIType* pAPITypeOut);

/*****************************************************************************//**
** Set the logging callback function
** This function will block until all outstanding callbacks are completed
**
** \param[in] types                      Types of logging messages to register for
** \param[in] pCallback                  Callback function pointer
**                                        - !NULL to enable logging callbacks and set callback function pointer
**                                        - NULL to turn off logging callbacks and unset callback function pointer
** \param[in] pUser                      A user specified pointer that is passed back on logging callbacks
**
** \return HWDBG_STATUS_SUCCESS          If the callback was set successfully
** \return HWDBG_STATUS_ERROR            If the callback was not set due to an error
*******************************************************************************/
typedef void (*AMDLoggingCallback)(void *);
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgSetLoggingCallback(unsigned int         types,
                        AMDLoggingCallback pCallback,
                        void*                pUser);

/***************************************************************************//**
** Mark the start debugging for a shader dispatch.
** This function should be called right before the shader dispatch to be debugged.
** Only one shader dispatch should be between HwDbgBeginDebugging and
** HwDbgEndDebugging.
**
** \param[in]  pContext               The debug context. Varies per API being
**                                     debugged:
**                                    DX - ID3D11DeviceContext*
**                                    CL - cl_command_queue*
**                                    GL - ????????
**
** \param[out] pDebugContextOut       The handle to identify the particular
**                                    shader debug context that started.
**
** \return HWDBG_STATUS_SUCCESS       On success.
** \return HWDBG_STATUS_NULL_POINTER  If the input argument is NULL.
** \return HWDBG_STATUS_UNSUPPORTED   If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgBeginDebugging(void* pContext,
                    HwDbgContextHandle* pDebugContextOut);

/***************************************************************************//**
** Mark the end debugging for a shader dispatch.
** This function should be called right after the shader dispatch to be debugged.
** Only one shader dispatch should be between HwDbgBeginDebugging and
** HwDbgEndDebugging.
**
** \param[in]  hDebugContext          The debug context handle returned from
**                                     HwDbgBeginDebugging.
**
** \return HWDBG_STATUS_SUCCESS       On success.
** \return HWDBG_STATUS_NULL_POINTER  If the input argument is NULL.
** \return HWDBG_STATUS_UNSUPPORTED   If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgEndDebugging(HwDbgContextHandle hDebugContext);

/***************************************************************************//**
** A synchronous function (blocking) to wait on an event from the GPU.
** This function should be called on a separate CPU thread.
**
** \param[in]  timeout                    Wait for x milliseconds for the event
**                                         before timing out.
** \param[out] pDebugContextOut           The handle to identify the particular
**                                         shader debug context for this event.
**
** \return HWDBG_STATUS_SUCCESS       On success.
** \return HWDBG_STATUS_NULL_POINTER  If the input argument is NULL.
** \return HWDBG_STATUS_UNSUPPORTED   If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgWaitForEvent(const unsigned int        timeout,
                        HwDbgContextHandle* pDebugContextOut);

/***************************************************************************//**
** Get the last event type associated with dispatch.
**
** \param[in]  hDebugContext              The context handle received
**                                     from HwDbgWaitForEvent API.
** \param[out] pEventTypeOut          The event type.
**
** \return HWDBG_STATUS_SUCCESS       On success.
** \return HWDBG_STATUS_NULL_POINTER  If the input argument is NULL.
** \return HWDBG_STATUS_UNSUPPORTED   If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgGetEventType(const HwDbgContextHandle hDebugContext,
                        HwDbgEventType*    pEventTypeOut);

/***************************************************************************//**
** Get the size of the shader binary.
**
** \param[in]  hDebugContext          The context handle received
**                                     from HwDbgWaitForEvent API.
** \param[out] pBinarySizeOut         The size in bytes of the binary.
**
** \return HWDBG_STATUS_SUCCESS       On success.
** \return HWDBG_STATUS_NULL_POINTER  If the input argument is NULL.
** \return HWDBG_STATUS_UNSUPPORTED   If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgGetShaderBinarySize(const HwDbgContextHandle hDebugContext,
                               size_t*            pBinarySizeOut);

/***************************************************************************//**
** Get the shader binary.
**
** \param[in]  hDebugContext          The context handle received
**                                     from HwDbgWaitForEvent API.
** \param[out] pBinaryOut             Pointer to a chunk of memory greater
**                                     or equal in size the that returned
**                                     by HwDbgGetShaderBinarySize.
**
** \return HWDBG_STATUS_SUCCESS       On success.
** \return HWDBG_STATUS_NULL_POINTER  If the input argument is NULL.
** \return HWDBG_STATUS_UNSUPPORTED   if the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgGetShaderBinary(const HwDbgContextHandle hDebugContext,
                           void*              pBinaryOut);

/***************************************************************************//**
** Create a breakpoint. It is initially disabled.
**
** \param[in]  hDebugContext          The context handle received
**                                     from HwDbgWaitForEvent API.
** \param[out] pBreakpointOut         The handle of newly the created
**                                     breakpoint. It is valid for use
**                                     anywhere after creation. However, it is
**                                     undefined to change the breakpoint state
**                                     outside the HwDbgWaitForEvent/
**                                     HwDbgContinueEvent pair associated with
**                                     the shader dispatch that the breakpoint
**                                     was created for.
**
** \return HWDBG_STATUS_SUCCESS       On success.
** \return HWDBG_STATUS_NULL_POINTER  If the input argument is NULL.
** \return HWDBG_STATUS_UNSUPPORTED   If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgCreateBreakpoint(const HwDbgContextHandle     hDebugContext,
                            HwDbgBreakpointHandle* pBreakpointOut);

/***************************************************************************//**
** Delete a breakpoint.
**
** \param[in] hBreakpoint            The breakpoint handle. The handle is
**                                    invalid after this call and may be
**                                    returned in future calls to
**                                    HwDbgCreateBreakpoint.
**
** \return HWDBG_STATUS_SUCCESS      On success.
** \return HWDBG_STATUS_UNSUPPORTED  If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgDeleteBreakpoint(HwDbgBreakpointHandle hBreakpoint);

/***************************************************************************//**
** Set the program counter for a breakpoint.
**
** \param[in] hBreakpoint            The breakpoint handle.
** \param[in] pc                     The program counter to set.
**
** \return HWDBG_STATUS_SUCCESS      On success.
** \return HWDBG_STATUS_UNSUPPORTED  If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgSetBreakpointProgramCounter(const HwDbgBreakpointHandle hBreakpoint,
                                 const size_t                pc);

/***************************************************************************//**
** Get the program counter for a breakpoint.
**
** \param[in]  hBreakpoint            The breakpoint handle.
** \param[out] pPcOut                 The program counter.
**
** \return HWDBG_STATUS_SUCCESS       On success.
** \return HWDBG_STATUS_NULL_POINTER  If the input argument is NULL.
** \return HWDBG_STATUS_UNSUPPORTED   If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgGetBreakpointProgramCounter(const HwDbgBreakpointHandle hBreakpoint,
                                       size_t*               pPcOut);

/***************************************************************************//**
** Set whether a breakpoint is enabled or disabled.
**
** \param[in] hBreakpoint            The breakpoint handle.
** \param[in] bEnabled               True to enable the breakpoint,
**                                    false to disable.
**
** \return HWDBG_STATUS_SUCCESS      On success.
** \return HWDBG_STATUS_UNSUPPORTED  If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgSetBreakpointEnabled(const HwDbgBreakpointHandle hBreakpoint,
                          const bool                  bEnabled);

/***************************************************************************//**
** Get whether a breakpoint is enabled or disabled.
**
** \param[in]  hBreakpoint            The breakpoint handle.
** \param[out] pEnabledOut            True if breakpoint is enabled,
**                                     false if disabled.
**
** \return HWDBG_STATUS_SUCCESS       On success.
** \return HWDBG_STATUS_NULL_POINTER  If the input argument is NULL.
** \return HWDBG_STATUS_UNSUPPORTED   If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgGetBreakpointEnabled(const HwDbgBreakpointHandle hBreakpoint,
                                bool*                 pEnabledOut);

/***************************************************************************//**
** Get the program counter for the active thread.
** May only be called after a HWDBG_EVENT_POST_STEP or
** HWDBG_EVENT_POST_BREAKPOINT event.
**
** \param[in]  hDebugContext          The context handle received
**                                     from HwDbgWaitForEvent API.
** \param[in]  threadIdX              The particular thread id (work item id) to
**                                     request the program counter.  This is
**                                     the x-dimension of the thread id.
** \param[in]  threadIdY              The particular thread id (work item id) to
**                                     request the program counter.  This is
**                                     the y-dimension of the thread id.
**                                     For 1D dispatch, use 0 for this.
** \param[in]  threadIdZ              The particular thread id (work item id) to
**                                     request the program counter.  This is
**                                     the z-dimension of the thread id.
**                                     For 1D or 2D dispatch, use 0 for this.
** \param[out] pPcOut                 The current program counter.
**
** \return HWDBG_STATUS_SUCCESS       On success.
** \return HWDBG_STATUS_NULL_POINTER  If the input argument is NULL.
** \return HWDBG_STATUS_UNSUPPORTED   If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgGetProgramCounter(const HwDbgContextHandle hDebugContext,
                       size_t                   threadIdX,
                       size_t                   threadIdY,
                       size_t                   threadIdZ,
                       size_t*                  pPcOut);

/***************************************************************************//**
** Get the memory contents of the specified region.
** May only be called after a HWDBG_EVENT_POST_STEP or
** HWDBG_EVENT_POST_BREAKPOINT event.
**
** \param[in]  hDebugContext          The context handle received
**                                     from HwDbgWaitForEvent API.
** \param[in]  memType                The type of memory to retrieve values
**                                     from.
** \param[in]  address                The the starting memory address.
** \param[in]  count                  The number of bytes to retrieve.
** \param[out] pMemOut                Pointer to a memory chunk at least
**                                     count bytes long.
**
** \return HWDBG_STATUS_SUCCESS       On success.
** \return HWDBG_STATUS_NULL_POINTER  If the input argument is NULL.
** \return HWDBG_STATUS_UNSUPPORTED   If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgGetMemory(const HwDbgContextHandle hDebugContext,
               const HwDbgMemoryType    memType,
               const size_t             address,
               const size_t             count,
                     size_t*            pMemOut);

/***************************************************************************//**
** After receiving an event with HwDbgWaitForEvent, call this function to
** instruct the GPU to perform the next operation (step, continue, etc).
** This is an asynchronous call, subsequent calls are undefined until
** HwDbgWaitEvent returns the same dispatch handle.
**
** \param[in] hDebugContext             The context handle received
**                                       from HwDbgWaitForEvent API.
** \param[in] command                   The debugger command to execute next.
**
** \return HWDBG_STATUS_SUCCESS      On success.
** \return HWDBG_STATUS_UNDEFINED    The call is undefined due to
**                                    a pending operation.
** \return HWDBG_STATUS_UNSUPPORTED  The API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgContinueEvent(const HwDbgContextHandle hDebugContext,
                         HwDbgCommand       command);

/***************************************************************************//**
** Break execution of all threads. May be called at any time.
**
** \param[in]  hDebugContext          The context handle received
**                                     from HwDbgWaitForEvent API.
**
** \return HWDBG_STATUS_SUCCESS       On success.
**         HWDBG_STATUS_UNSUPPORTED   If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgBreakAll(const HwDbgContextHandle hDebugContext);

/***************************************************************************//**
** Kill execution of all threads to terminate execution of shader.
** May be called at any time.
**
** \param[in]  hDebugContext          The context handle received
**                                     from HwDbgWaitForEvent API.
**
** \return HWDBG_STATUS_SUCCESS       On success.
** \return HWDBG_STATUS_UNSUPPORTED   If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgKillAll(const HwDbgContextHandle hDebugContext);

/***************************************************************************//**
** Temporary entrypoint to allow simulating a breakpoint by initiating a trap.
** \warning This is only a temporary entrypoint for testing purposes.
**
** \param[in]  hDebugContext          The context handle received
**                                     from HwDbgWaitForEvent API.
**
** \return HWDBG_STATUS_SUCCESS       On success.
** \return HWDBG_STATUS_UNSUPPORTED   If the API is not yet implemented.
*******************************************************************************/
extern HWDBG_API_ENTRY HwDbgStatus HWDBG_API_CALL
HwDbgTrapAllTemp(const HwDbgContextHandle hDebugContext);

#ifdef __cplusplus
}
#endif

#endif
