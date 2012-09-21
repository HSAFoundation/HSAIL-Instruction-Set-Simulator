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
 * Date: 08/15/2012
 *
 * Purpose: This test covers Section 2.1 - Overview of NDRanges, Work-Groups, 
 *          and Work-Items.
 *
 ******************************************************************************/
#include "rtcommon.hpp"
#include "OverviewNDRangesWorkGroupsAndWorkItems.hpp"

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
	printf("Calling Setup\n");

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
	kname.assign("&__Get_Dispatch_ID"); // Name of kernel to call.
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
		hsa::RTKernelArg kernelArg1;  // Return Data

		// TODO: Need to use the wavesize to create the NDRange.
		// Published API does not match the header files
		
		// Setup Some Test Data
		unsigned int arraySize = NDRANGE_SIZE_X * NDRANGE_SIZE_Y * NDRANGE_SIZE_Z;
		void * arrayA;

		// Run this test multiple times
		for (unsigned int j = 0; (j < NUM_KERNEL_CALLS) && (TestFailed == false); j++)
		{
			// Allocate HSA memory
			arrayA = hsaruntime->allocateMemory(sizeof(unsigned int) * arraySize, hsa::SYSTEM_MEMORY,*device);
			unsigned int * DataA = (unsigned int *) arrayA;
			for (unsigned int k = 0; (k < arraySize) ; k++)
			{
				// Initialize data to 0x00
				DataA[k] = 0;
			}

			// Point the Kernel Arguments to the test data data
			kernelArg1.addr = arrayA;

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
			queue->dispatch(kernel, &la, depEvent, 3, kernelArg1);
	  
			// Collect first dispatch ID returned
			DispatchIDs[j] = DataA[0];
			printf("Kernel (0,0,0) Dispatch ID = %X\n", DispatchIDs[j]);

			// Look through all of the dispatch IDs returned by this execution to make sure they match the first one found
			for (unsigned int k = 0; k < arraySize; k++)
			{
				if (DataA[k] != DispatchIDs[j])
				{
					// Test has failed. Print dispatch id of failing kernel
					unsigned int a,b,c;

					c = k % NDRANGE_SIZE_Z;
					b = (k / NDRANGE_SIZE_Z) % NDRANGE_SIZE_Y; 
					a = k / (NDRANGE_SIZE_Y * NDRANGE_SIZE_Z);
	 
					printf("Kernel (%d,%d,%d) Returned Dispatch ID %X	FAIL\n", a, b, c);
					TestFailed = true;
					break;
				}
			}
			// This execution was successful
			if (TestFailed == false)
				printf("All Kernels came back with Dispatch ID = %X 	PASS\n", DispatchIDs[j]);

			// Verify that all dispatch ids have incremented with each kernel call
			// Note: This will fail if there are other HSA programs running on the system while Conformance testing is in progress
			for (unsigned int k = 0; k < j; k++)
			{
				if ( (DispatchIDs[k] + 1) != DispatchIDs[k+1])
				{
					printf("Execution call %d did not increment by 1 from previous execution call.      FAIL\n", k+1);
					printf("Dispatch ID for execution %d was %d. Dispatch ID for execution %d was %d\n", k, DispatchIDs[k], k+1, DispatchIDs[k+1]);
					TestFailed = true;
				}
			}

			// Free the allocated memory
			hsaruntime->freeMemory(arrayA);
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
