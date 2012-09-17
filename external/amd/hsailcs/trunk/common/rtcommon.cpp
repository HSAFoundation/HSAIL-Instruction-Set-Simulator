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
 * Date: 09/13/2012
 *
 * Purpose: This provides common functions to each hsail conformance test
 *
 ******************************************************************************/
#include "rtcommon.hpp"

using namespace std;

HINSTANCE hinstDll;

//char * DLL_Path = "hsa64.dll";
char * DLL_Path = "hsa.dll";

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
char * ReadFile(std::string source_filename, size_t& size)
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
int SetupHSA(hsa::IRuntimeApi ** hsaruntime)
{
	printf("SetupHSA\n");
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
			*hsaruntime = (hsa::IRuntimeApi*) (*lpfnGetRuntime) () ;
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
// Name: GetHSADevices
//
// Description: Gets the count of HSA Devices on the system and a pointer to the
//              Device Vector Array.
//
// Inputs: hsaruntime - pointer to the hsaruntime obeject
//
// Outputs: Returns: 1 - Success
//                   0 - Fail
//          Updates numDevices with the device count
//          Updates pDevices with Device List
//
//===========================================================================
int GetHSADevices(hsa::IRuntimeApi* hsaruntime, uint32_t * numDevices, hsa::vector<hsa::IDevice*> * pDevices)
{
	// Get the Number of Runtime devices on this system
	hsaruntime->getDeviceCount(numDevices);
	if (numDevices == 0)
	{
		// If there are no devices then test fails.
		printf("The HSA runtime API reported 0 devices to run tests on.\n");
		return 0;
	}
	printf("There are %d devices reported on the system\n", *numDevices);
	
	// Now get the actual device objects
	printf("Getting Devices\n");
	*pDevices = hsaruntime->getDevices();
	printf("Devices Retreived\n");
	return 1;
}

//===========================================================================
//
// Name: MakeHSAProgram
//
// Description: Creates an HSA Program from the HSAIL Brigg
//
// Inputs: hsaruntime - pointer to the hsaruntime obeject
//         pDevices - pointer to the HSA Device objects
//         HSAIL_Path - Location of the HSAIL Brigg file
//         program - This will hold the program object when the function returns
//
// Outputs: Returns: 1 - Success
//                   0 - Fail
//          Updates program with the program object
//
//===========================================================================
int MakeHSAProgram(hsa::IRuntimeApi* hsaruntime, hsa::vector<hsa::IDevice*> * pDevices, char * HSAIL_Path, hsa::IProgram ** program)
{
	// Binary File should be created with either assembler or HLC
	// 1. Using Assembler 
	// HSAILasm.exe -assemble foo.bin
	// This creates foo.bin HSAIL Brig file
	// 2. Using HLC
	// aoc2 -march=hsail -srctoir foo.cl
	// This will convert your open cl kernel to a foo.obj llvmir file
	// aoc2 -march=hsail -irtocg foo.obj
	// This will convert your foo.obj llvmir file to foo.cgt which contains the HSAIL text.
	
	// Load the HSAIL Brig file
	std::string binaryFilename = HSAIL_Path;
	size_t	size;
	char * buffer = ReadFile(binaryFilename, size);
	if (buffer == NULL)
	{
		// If the buffer is NULL then brig file read has failed.
		printf("Could not read HSAIL File\n");
		return 0;
	}
	printf("Loaded ELF into Buffer\n");

	// Create an HSA Program
	hsaruntime->createProgram(buffer, size, pDevices, *program);
	printf("Program Created\n");
	return 1;
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
