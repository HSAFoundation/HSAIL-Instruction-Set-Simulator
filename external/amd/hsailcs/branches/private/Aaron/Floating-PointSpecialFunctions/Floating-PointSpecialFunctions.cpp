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

#include "Floating-PointSpecialFunctions.hpp"

using namespace std;

//===========================================================================
//
// Name: ReadFile
//
// Description: Opens a text file and places the contents into a buffer. The
//              resulting buffer is allocated by this function. A pointer
//              to the resulting buffer is returned along with the size.
//
// Inputs: source_filename - The path to the file to open.
//         size - The address of where the resulting buffer size will be placed
//
// Outputs: A pointer to the resulting buffer.
//          size - This location will be updated with the resulting buffer size
//
//===========================================================================
static char * ReadFile(std::string source_filename, size_t& size)
{
	FILE* fp;
	fopen_s(&fp, source_filename.c_str(), "rb" );
	uint32_t length;
	size_t offset = 0;
	char *ptr;

	// If there was an issue opening the file return a Null buffer
	if (!fp)
		return NULL;

	// Obtain file size.
	::fseek (fp , 0 , SEEK_END);
	length = ::ftell (fp);
	::rewind (fp);

	// Allocate the buffer to place the file into
	ptr = reinterpret_cast<char*>(malloc(offset + length + 1));
	if (length != fread(&ptr[offset], 1, length, fp))
	{
		// If the length read does not match the computed size then deallocate the buffer and return NULL
		free(ptr);
		return NULL;
	}

	// Close the file and return the data pointer
	ptr[offset + length] = '\0';
	size = offset + length;
	::fclose(fp);
	return ptr;
}

//===========================================================================
//
// Name: SetupHSA
//
// Description: Opens the hsa interface and fetches the runtime object.
//
// Inputs: None
//
// Outputs: Returns: 1 - Success
//                   0 - Fail
//          Updates the hsaruntime variable with the runtime object.
//
//===========================================================================
int SetupHSA()
{

#if 0
	// This code checks to verify the existance of hsa.dll or hsa.so"
	FILE *fp;
	errno_t openerr = fopen_s(&fp, DLL_Path,"r");
	if( fp ) 
	{
		printf("Found %s\n", DLL_Path);
		fclose(fp);
	} 
	else 
	{
		printf("Could not find %s Error %d\n", DLL_Path, openerr);
	}
#endif
	// Get handle to dll
	hinstDll = LoadLibrary(DLL_Path); 
	if (hinstDll != NULL)
	{
		// Get the Address of the getRuntime Function
		FARPROC lpfnGetRuntime = GetProcAddress(hinstDll, "getRuntime"); 

		if ( lpfnGetRuntime != (FARPROC) NULL )
		{
			printf("Found Function Call\n");
			// Get the Runtime Object
			hsaruntime = (hsa::IRuntimeApi*) (*lpfnGetRuntime) () ;
			return 1;
		}
		else
		{
			// Failure because function getRuntime could not be found.
			// Most likely library is corrupt.
			printf("Could not find Function\n");
			FreeLibrary( hinstDll ) ;
			return 0;
		}
	}
	
	// Handle a few errors I've encountered while building this code.
	long lasterror = GetLastError();
	switch(lasterror)
	{
	case 126:
		printf("Could not load dll. ERROR_MOD_NOT_FOUND Code=126.");
		break;
	case 193:
		printf("Could not load dll. ERROR_BAD_EXE_FORMAT Code=193.");
		break;
	default:
		printf("Could not load dll. Last Error Code was %d\n", lasterror);
	}
	return 0;
}

//===========================================================================
//
// Name: CloseHSA
//
// Description: Frees Library from memory
//
// Inputs: N/A
//
// Outputs: N/A
//
//===========================================================================
void CloseHSA()
{
	// Free the library
	FreeLibrary ( hinstDll ) ;
}

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
	if (!SetupHSA())
	{
		exit(1);
	}

	// Get the Number of Runtime devices on this system
	hsaruntime->getDeviceCount(&numDevices);
	if (numDevices == 0)
	{
		// If there are no devices then test fails.
		printf("The HSA runtime API reported 0 devices to run tests on.\n");
		exit(1);
	}
	printf("There are %d devices reported on the system\n", numDevices);
	
	// Now get the actual device objects
	printf("Getting Devices\n");
	pDevices = hsaruntime->getDevices();
	printf("Devices Retreived\n");

	// Binary File should be created with either assembler or HLC
	// 1. Using Assembler 
	// HSAILasm.exe -assemble foo.bin
	// This creates foo.bin HSAIL Brig file
	// 2. Using HLC
	// aoc2 -march=hsail -srctoir foo.cl
	// This will convert your open cl kernel to a foo.obj llvmir file
	// aoc2 -march=hsail -irtocg foo.obj
	// This will convert your foo.obj llvmir file to foo.cgt which contains the HSAIL text.
	// 3. Using Objgen
	// objgen foo.hsail foo.bin
	// This will create the foo.bin HSAIL Brig file.
	
	// Load the HSAIL Brig file
	std::string binaryFilename = HSAIL_Path;
	size_t	size;
	char * buffer = ReadFile(binaryFilename, size);
	if (buffer == NULL)
	{
		// If the buffer is NULL then brig file read has failed.
		printf("Could not read HSAIL File\n");
		exit(1);
	}
	printf("Loaded ELF into Buffer\n");

	// Create an HSA Program
	hsa::IProgram * program = NULL;
	hsaruntime->createProgram(buffer, size, &pDevices, program);
	printf("Program Created\n");

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
