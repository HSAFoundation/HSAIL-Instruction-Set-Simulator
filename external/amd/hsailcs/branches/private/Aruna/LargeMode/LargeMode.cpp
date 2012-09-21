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

/******************************************************************************
Test Name: LargeMode
Corresponds to: Chapter1 - Overview
Test Purpose: To verify if Large Mode is supported
******************************************************************************/

/*****************************************************************************
SVN Information (we will have version information here)
*****************************************************************************/

#include "LargeMode.hpp"

using namespace std;

static char *readFile(std::string source_filename, size_t& size)
{
	FILE* fp;
	fopen_s(&fp, source_filename.c_str(), "rb" );
	uint32_t length;
	size_t offset = 0;
	char *ptr;

	if (!fp)
		return NULL;

	// obtain file size.
	::fseek (fp , 0 , SEEK_END);
	length = ::ftell (fp);
	::rewind (fp);

	ptr = reinterpret_cast<char*>(malloc(offset + length + 1));
	if (length != fread(&ptr[offset], 1, length, fp))
	{
		free(ptr);
		return NULL;
	}

	ptr[offset + length] = '\0';
	size = offset + length;
	::fclose(fp);
	return ptr;
}

int SetupHSA()
{
#if 0
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
		FARPROC lpfnGetRuntime = GetProcAddress(hinstDll, "getRuntime"); 

		if ( lpfnGetRuntime != (FARPROC) NULL )
		{
			printf("Found Function Call\n");
			hsaruntime = (hsa::IRuntimeApi*) (*lpfnGetRuntime) () ;
			return 1;
		}
		else
		{
			printf("Could not find Function\n");
			FreeLibrary( hinstDll ) ;
			return 0;
		}
	}
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

void CloseHSA()
{
	FreeLibrary ( hinstDll ) ;
}

int main(int argc, char * argv[])
{
	printf("Calling Setup\n");

	if (!SetupHSA())
	{
		exit(1);
	}

	hsaruntime->getDeviceCount(&numDevices);
	if (numDevices == 0)
	{
		printf("The HSA runtime API reported 0 devices to run tests on.\n");
		exit(1);
	}
	
	printf("There are %d devices reported on the system\n", numDevices);
	printf("Get Devices\n");
	pDevices = hsaruntime->getDevices();
	printf("Devices Retreived\n");

	/******************************************************************************* 
	Binary File should be created with either assembler or HLC
	1. Using Assembler 
	HSAILasm.exe -assemble foo.hsail
	This creates foo.brigg file
	2. Using HLC
	aoc2 -march=hsail -srctoir foo.cl
	This will convert your open cl kernel to a foo.obj llvmir file
	aoc2 -march=hsail -irtocg foo.obj
	This will convert your foo.obj llvmir file to foo.cgt which contains the HSAIL text.
	3. Using Assembler
	objgen.exe foo.hsail foo
	This creates foo brigg file
	*******************************************************************************/

	std::string binaryFilename = "OverviewNDRangesWorkGroupsAndWorkItems.bin";
	size_t	size;
	char * buffer = readFile(binaryFilename, size);

	if (buffer == NULL)
	{
		printf("Could not read HSAIL File\n");
		exit(1);
	}
	printf("Loaded ELF into Buffer\n");

	// Create Program
	hsa::IProgram * program = NULL;
	hsaruntime->createProgram(buffer, size, &pDevices, program);
	printf("Program Created\n");

	// Create Kernel
	std::string kname;
	kname.assign("&__LargeMode");
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
		hsa::RTKernelArg kernelArg1;  // Return Data
		
		// Setup Some Test Data
		unsigned int arraySize = NDRANGE_SIZE_X * NDRANGE_SIZE_Y * NDRANGE_SIZE_Z;
		void * arrayA;

		for (unsigned int j = 0; (j < NUM_KERNEL_CALLS) && (TestFailed == false); j++)
		{
			// we have to use this method of memory allocation
			arrayA = hsaruntime->allocateMemory(sizeof(unsigned int) * arraySize, hsa::SYSTEM_MEMORY,*device);

			unsigned int * DataA = (unsigned int *) arrayA;
			for (unsigned int k = 0; (k < arraySize) ; k++)
			{
				DataA[k] = 0;
			}


DataA[0x0] = 0x0000000000000001;
DataA[0x8] = 0x00000000FFFFFFFF;
DataA[0x10] = 0x00000000FFFFFFFF;
DataA[0x18] = 0xFFFFFFFFFFFFFFFF;
DataA[0x20] = 0xFFFFFFFFFFFFFFFF;

DataA[0x28] = 0x0000000000000001;
DataA[0x30] = 0x0000000000000001;
DataA[0x38] = 0x0000000000000011;
DataA[0x40] = 0x0000000000000001;
DataA[0x48] = 0x0000000000000011;

//RESULT
//DataA[50] = 0x0000000000000002;
//DataA[58] = 0x0000000100000000;
//DataA[60] = 0x0000000100000010;
//DataA[68] = 0x0000000000000000;
//DataA[70] = 0x0000000000000010;


			// Point the Kernel Arguments to the test data
			kernelArg1.addr = arrayA;

			// args are ready - now we dispatch and compare the results.
			hsa::LaunchAttributes la;
			la.gridX = NDRANGE_SIZE_X;
			la.gridY = NDRANGE_SIZE_Y;
			la.gridZ = NDRANGE_SIZE_Z;
			la.groupX = NDRANGE_SIZE_X;
			la.groupY = NDRANGE_SIZE_Y;
			la.groupZ = NDRANGE_SIZE_Z;
			hsa::Event * depEvent = NULL;
		
			printf("Launching %dx%dx%d Kernels\n", NDRANGE_SIZE_X, NDRANGE_SIZE_Y, NDRANGE_SIZE_Z);
			queue->dispatch(kernel, &la, depEvent, 3, kernelArg1); //Kernel Launch

//The following code writes the values of the array to the screen. Visually compare to start with and later change it to be self-checking.
	  	for (unsigned int j = 0; j < 15; j++)
                        printf ("arrayA[%d] = %x ", j, arrayA[j], "\n");
}
			
			
		
			hsaruntime->freeMemory(arrayA);
		}
	}

	CloseHSA();

	if (TestFailed)
	{
		exit(1);
	}
	else
	{
		exit(0);
	}
}
