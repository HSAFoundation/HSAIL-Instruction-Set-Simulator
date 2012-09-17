/*******************************************************************************
 * Copyright (c) 2012 The HSA Foundation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 ******************************************************************************/
/*
 * Author: Aaron Stone
 *
 * Date: 09/10/2012
 *
 * Purpose: This test covers Section 5.13 of the HSAIL PRM - Floating-Point
 *          Special Function
 *
 ******************************************************************************/
#include "rtcommon.hpp"
#include "Floating-PointSpecialFunctions.hpp"

using namespace std;

//===========================================================================
//
// Name: main
//
// Description: Main execution thread.
//
// Inputs: argc, argv - Any input arguments passed in on the command line
//
// Outputs: 0 - Success
//          1 - Fail
//
//===========================================================================
int main(int argc, char * argv[])
{
	// Get the HSA Runtime Object
	if (!SetupHSA(&hsaruntime))
	{
		exit(1);
	}

	if (!GetHSADevices(hsaruntime, &numDevices, &pDevices))
	{
		exit(1);
	}

	if (!MakeHSAProgram(hsaruntime, &pDevices, HSAIL_Path, &program))
	{
		exit(1);
	}

	// Create Kernel
	std::string kname;
	kname.assign("&__Get_fcos"); // Name of kernel to call.
	printf("Building Program\n");
	hsa::IKernel * kernel = program->build(kname.c_str(), kname.length());
	printf("Done Building Program\n");
	
	// Run test on each HSA device that is found on this system
	for (unsigned int i = 0; i < numDevices; i++)
	{
		printf("Executing on Device %d\n", i);
		// Get pointer to the device in the device list.
		hsa::IDevice * device = pDevices[i];
	
		// Create a command queue on the device
		hsa::IQueue * queue;
		hsaruntime->createDeviceQueue(device, 1, queue); 

		// Create Arguments for kernel call
		hsa::RTKernelArg kernelArg1;  // Input Data
		hsa::RTKernelArg kernelArg2;  // Return Data

		// Setup Some Test Data
		unsigned int arraySize = NDRANGE_SIZE_X * NDRANGE_SIZE_Y * NDRANGE_SIZE_Z;
		void * arrayA, * arrayB;

		// Run this test multiple times
		for (unsigned int j = 0; (j < NUM_KERNEL_CALLS) && (TestFailed == false); j++)
		{
			// Allocate HSA memory
			arrayA = hsaruntime->allocateMemory(sizeof(unsigned int) * arraySize, hsa::SYSTEM_MEMORY, *device);
			arrayB = hsaruntime->allocateMemory(sizeof(unsigned int) * arraySize, hsa::SYSTEM_MEMORY, *device);

			unsigned int * DataA = (unsigned int *) arrayA;
			unsigned int * DataB = (unsigned int *) arrayB;

			for (unsigned int k = 0; (k < arraySize) ; k++)
			{
				// Initialize data
				DataA[k] = k;
			}

			// Point the Kernel Arguments to the test data data
			kernelArg1.addr = arrayA;
			kernelArg2.addr = arrayB;

			// Configure the NDRange and workgroup sizes for launch of HSA Kernel
			hsa::LaunchAttributes la;
			la.gridX = NDRANGE_SIZE_X;
			la.gridY = NDRANGE_SIZE_Y;
			la.gridZ = NDRANGE_SIZE_Z;
			la.groupX = NDRANGE_SIZE_X;
			la.groupY = NDRANGE_SIZE_Y;
			la.groupZ = NDRANGE_SIZE_Z;
			hsa::Event * depEvent = NULL;
		
			// Dispatch the Kernel execution
			printf("Launching %dx%dx%d Kernels\n", NDRANGE_SIZE_X, NDRANGE_SIZE_Y, NDRANGE_SIZE_Z);
			queue->dispatch(kernel, &la, depEvent, 3, kernelArg1, kernelArg2);

			// Look through all of the dispatch IDs returned by this execution to make sure they match the first one found
			for (unsigned int k = 0; k < arraySize; k++)
			{
				printf("Result %d = %d\n", k, DataB[k]);
			}
			// This execution was successful
			if (TestFailed == false)
				printf("Test Failed\n");

			// Free the allocated memory
			hsaruntime->freeMemory(arrayA);
			hsaruntime->freeMemory(arrayB);
		}
	}

	// Close the HSA library
	CloseHSA();

	// Return result based on results of testing
	if (TestFailed)
	{
		exit(1);
	}
	else
	{
		exit(0);
	}
}
