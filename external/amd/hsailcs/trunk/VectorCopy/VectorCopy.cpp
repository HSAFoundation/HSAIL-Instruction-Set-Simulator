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
 * Author: Ross Hytnen
 * Modified by: Aaron Stone
 *
 * Date: 09/10/2012
 *
 * Purpose: This test covers Section 5.13 of the HSAIL PRM - Floating-Point
 *          Special Function
 *
 ******************************************************************************/
#include "rtcommon.hpp"
#include "VectorCopy.hpp"

using namespace std;

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
	kname.assign("&__OpenCL_vec_copy_kernel");
	printf("Building Program\n");
	hsa::IKernel * kernel = program->build(kname.c_str(), kname.length());
	printf("Done Building Program\n");

	// Finished loading the Elf .bin
	for (unsigned int i = 0; i < numDevices; i++)
	{
		printf("Executing on Device %d\n", i);
		// Get this device
		hsa::IDevice *device = pDevices[i];
	
		// Create device queue
		hsa::IQueue * queue;
		hsaruntime->createDeviceQueue(device, 1, queue); 

		// Setup Arguments
		hsa::RTKernelArg kernelArg1;  // vector a
  		hsa::RTKernelArg kernelArg2;  // vector b
  		hsa::RTKernelArg kernelArg3;  // vector length
		
		// Setup Some Test Data
		int arraySize = 16;
		void * arrayA;
		void * arrayB;

		// we have to use this method of memory allocation
		arrayA = hsaruntime->allocateMemory(sizeof(float) * arraySize, hsa::SYSTEM_MEMORY,*device);
		arrayB = hsaruntime->allocateMemory(sizeof(float) * arraySize, hsa::SYSTEM_MEMORY,*device);

		float * floatA = (float *) arrayA;
		float * floatB = (float *) arrayB;
		for (int i = 0; i < arraySize; i++)
		{
			floatA[i] = (float) i;
			floatB[i] = 0;
		}

		// Point the Kernel Arguments to the test data data
		kernelArg1.addr = arrayA;
		kernelArg2.addr = arrayB;
		kernelArg3.s32value = arraySize;

		// args are ready - now we dispatch and compare the results.
		hsa::LaunchAttributes la;
		la.gridX = 16;
		la.gridY = 16;
		la.gridZ = 16;
		la.groupX = 16;
		la.groupY = 16;
		la.groupZ = 16;
		hsa::Event * depEvent = NULL;
		queue->dispatch(kernel, &la, depEvent, 3, kernelArg1, kernelArg2, kernelArg3);
	  
		for (int i = 0; i < arraySize; i++)
		{
			if (floatA[i] != floatB[i])
			{
				printf("Dipatch test: FAILED.\n");
				break;
			}
		}

		hsaruntime->freeMemory(arrayA);
		hsaruntime->freeMemory(arrayB);

		printf("Dispatch test:  Complete.\n");
	}

	CloseHSA();

	exit(0);
}
