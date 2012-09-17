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
 * Purpose: The is the HSAIL Conformance Suite Test Harness. It is launches
 *          HSAIL Tests and logs the results
 *
 ******************************************************************************/

#ifdef BUILDFORLINUX
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#else // BUILDFORLINUX
#include <io.h>
#endif // BUILDFORLINUX
#include <fcntl.h>
#include <string>
#include <time.h>
#include <stdarg.h>
#include <fstream>
#include <sys/stat.h>
#include "hsailcs.h"

using namespace std;

std::string FormatString(const char* fmt, ...)
{
    int size = MAX_FORMAT_STRING_SIZE;
    char* buffer = 0;
    buffer = new char[size];
    va_list vl;
    va_start(vl,fmt);
#ifdef BUILDFORLINUX
	int nsize = vsnprintf(buffer,size,fmt,vl);
#else
	int nsize = vsnprintf_s(buffer,MAX_FORMAT_STRING_SIZE,size,fmt,vl);
#endif
	if(size<=nsize)
	{//fail delete buffer and try again
        delete buffer; buffer = 0;
        buffer = new char[nsize+1];//+1 for /0
#ifdef BUILDFORLINUX
		nsize = vsnprintf(buffer,size,fmt,vl);
#else
		nsize = vsnprintf_s(buffer,MAX_FORMAT_STRING_SIZE,size,fmt,vl);
#endif
		
    }
    std::string ret(buffer);
    va_end(vl);
    delete buffer;
    return ret;
}

//===========================================================================
//
// Name: ProcessHSAILCSConfigurationFile
//
// Description: Executes a HSAIL CS Configuration file and saves the results
//
// Inputs: filename - Path and Filename of configuration file to run.
//
// Outputs: true - Success
//          false - Fail
//
//===========================================================================
bool ProcessHSAILCSConfigurationFile(const char * filename)
{
	ofstream hslgfp;  // File handle to Main Log
	ifstream hscfgfp; // File handle to the HSAILCS Configuration File
	struct stat st;
	std::string ErrorString, CommandString, TestLogDest, InputBuffer;
	//char FormatBuffer[MAX_FORMAT_STRING_SIZE];
	int TestCounter, spacesused, insertspaces;
	struct tm * timeinfo;
	time_t rawtime;

	rawtime = time(NULL);  // Get the Current Time
#ifdef BUILDFORLINUX	
	timeinfo = localtime( &rawtime );  // Convert Current Time to a tm structure
#else // BUILDFORLINUX
	struct tm timeinf;
	localtime_s(&timeinf, &rawtime); // Convert Current Time to a tm structure
	timeinfo = &timeinf;
#endif // BUILDFORLINUX

	// Verify that logs folder exists and if it doesn't then create it
	if (stat(LOGS_DIR, &st) != 0)
	{
		printf("Creating folder: %s\n", LOGS_DIR);
		CommandString = "mkdir " LOGS_DIR;
		if (system(CommandString.c_str()) != 0)
		{
			ErrorString = "ERROR: Could not create folder for hsailcs logs\n";
			perror(ErrorString.c_str());
			return false;
		}
	}

    // Create the Date String to be used for creating folder for this test execution
	TestLogDest = FormatString("%s%s%04d%02d%02d_%02d%02d%02d", LOGS_DIR, SLASH, timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	// Create the folder to be used for this test execution
	printf("Creating folder: %s\n", TestLogDest.c_str());
	if (stat(TestLogDest.c_str(), &st) != 0)
	{
		CommandString = FormatString("mkdir %s", TestLogDest.c_str());
		if (system(CommandString.c_str()) != 0)
		{
			ErrorString = "ERROR: Could not create log folder for this run\n";
			perror(ErrorString.c_str());
			return false;
		}
	}

	// Open Main Log for HSAIL Conformance Suite
	printf("Creating %s\n", MAIN_LOG);
	hslgfp.open(FormatString("%s%s%s", TestLogDest.c_str(), SLASH, MAIN_LOG).c_str());
	if(!hslgfp.is_open())
	{
		ErrorString = "ERROR: Failed to create HSAILCS log file ";
		ErrorString += MAIN_LOG;
		ErrorString += "\n";
		perror(ErrorString.c_str());
		return false;
	}

	// Open the HSAIL CS Configuration file
	printf("Opening HSAILCS Configuration file: %s\n", filename);

	hscfgfp.open(filename);
	if(!hscfgfp.is_open())
	{
		ErrorString = "ERROR: Failed to HSAIL Conformance Suite Configuration File ";
		ErrorString += filename;
		ErrorString += "\n";
		perror(ErrorString.c_str());
		hslgfp << ErrorString.c_str();
		hslgfp.close();
		return false;
	}

	// Verify the HSAIL Tests directory exists
	if (stat(TESTS_DIR, &st) != 0)
	{
		ErrorString = "ERROR: HSAIL Test Folder ";
		ErrorString += TESTS_DIR;
		ErrorString += " does not exist\n";
		perror(ErrorString.c_str());
		return false;
	}
	
	TestCounter = 0;
	while(hscfgfp.good())
	{
		// Read next line from HSAILCS Configuration File
		getline(hscfgfp, InputBuffer);

		// Make sure that the command read from the configuration file begins with a valid character.
		// This helps to prevent bad configuration input from crashing hsailcs.
		if ( (InputBuffer[0] >= '0' &&  InputBuffer[0] <= '9') ||
			 (InputBuffer[0] >= 'A' &&  InputBuffer[0] <= 'Z') ||
			 (InputBuffer[0] >= 'a' &&  InputBuffer[0] <= 'z'))
		{
			int errorcode = 0;
			int err, out, save_out, save_err;
			std::string TestName;

			// Get just the name of the Binary.  This is used for creating the log name for this test
			TestName = InputBuffer.substr(0, InputBuffer.find(' '));
			
			printf("Executing: %s", InputBuffer.c_str());
			fflush(stdout);
			hslgfp << "Executing: " + InputBuffer;
			
			// Increment number of spaces.  This is used for string formating 
			spacesused = InputBuffer.length() + strlen("Executing: ");
			
			// Increment the index of how many tests have been executed so far.  Also used for file name.
			TestCounter++; 
			
			// Create the File to be used for this Test's Output Log.			
			CommandString = FormatString("%s%s%06d_%s.log", TestLogDest.c_str(), SLASH, TestCounter, TestName.c_str());

#ifdef BUILDFORLINUX
			out = open(CommandString.c_str(), O_RDWR|O_CREAT|O_APPEND, 0600);
#else // BUILDFORLINUX
			_sopen_s(&out, CommandString.c_str(), O_RDWR|O_CREAT|O_APPEND, _SH_DENYNO, 0600);
#endif // BUILDFORLINUX
			if (-1 == out)
			{
				ErrorString = "ERROR: Can't Open " + CommandString;
				perror(ErrorString.c_str());
				errorcode = 1;
			}

			// Create the File to be used for this Test's Error output
			CommandString = FormatString("%s%s%06d_%s.err.log", TestLogDest.c_str(), SLASH, TestCounter, TestName.c_str());
#ifdef BUILDFORLINUX
			err = open(CommandString.c_str(), O_RDWR|O_CREAT|O_APPEND, 0600);
#else // BUILDFORLINUX
			_sopen_s(&err, CommandString.c_str(), O_RDWR|O_CREAT|O_APPEND, _SH_DENYNO, 0600);
#endif // BUILDFORLINUX
			if (-1 == err)
			{
				ErrorString = "ERROR: Can't Open %s\n" + CommandString;
				perror(ErrorString.c_str());
				errorcode = 1;
			}

			// Redirect stdout to this tests output log file
#ifdef BUILDFORLINUX
			save_out = dup(fileno(stdout));
			save_err = dup(fileno(stderr));
			if (-1 == dup2(out, fileno(stdout)))
#else // BUILDFORLINUX
			save_out = _dup(_fileno(stdout));
			save_err = _dup(_fileno(stderr));
			if (-1 == _dup2(out, _fileno(stdout)))
#endif // BUILDFORLINUX
			{
				perror("ERROR: Cannot redirect stdout\n");
				errorcode = 1;
			}

			// Redirect stderr to this tests error log file
#ifdef BUILDFORLINUX
			if (-1 == dup2(err, fileno(stderr)))
#else // BUILDFORLINUX
			if (-1 == _dup2(err, _fileno(stderr)))
#endif // BUILDFORLINUX
			{
				perror("ERROR: Cannot redirect stderr\n");
				errorcode = 1;
			}

			// Execute the Actual test now
			if (!errorcode)
			{
				CommandString = TESTS_DIR SLASH + InputBuffer;
				if (system(CommandString.c_str()) != 0)
				{
					errorcode = 2;
				}
			}

			// Redirect stdout and stderr back to the console
			fflush(stdout);
			fflush(stderr);
#ifdef BUILDFORLINUX
			close(out);
			close(err);
			dup2(save_out, fileno(stdout));
			dup2(save_err, fileno(stderr));
			close(save_out);
			close(save_err);
#else // BUILDFORLINUX
			_close(out);
			_close(err);
			_dup2(save_out, _fileno(stdout));
			_dup2(save_err, _fileno(stderr));
			_close(save_out);
			_close(save_err);
#endif // BUILDFORLINUX

			// Format output for console and main hsailcs log
			if (spacesused > 80)
				insertspaces = 0;
			else
				insertspaces = spacesused - 80;

			// Handle Error Codes
			switch (errorcode)
			{
			case 0:
				ErrorString = FormatString("%*s FINISHED\n", insertspaces, "");
				printf("%s", ErrorString.c_str());
				hslgfp << ErrorString;
				break;
			case 1:
				ErrorString = FormatString("%*s ERROR: Could not open log file. Skipping Test\n", insertspaces, "");
				printf("%s", ErrorString.c_str());
				hslgfp << ErrorString;
				break;
			case 2:
				ErrorString = FormatString("%*s FAILED\n", insertspaces, "");
				printf("%s", ErrorString.c_str());
				hslgfp << ErrorString;
				break;
			default:
				ErrorString = FormatString("%*s Unknown Error Occurred\n", insertspaces, "");
				printf("%s", ErrorString.c_str());
				hslgfp << ErrorString;
				break;
			}
		}
	}

	printf("HSAIL Conformance Suite Testing is Complete\n");
	hslgfp << "HSAIL Conformance Suite Testing is Complete\n";

	// Close Configuration File
	hscfgfp.close();

	// Close HSAILCS Main Log
	hslgfp.close();
	return true;
}

//===========================================================================
//
// Name: PrintUsage
//
// Description: Prints the usage of the HSAILCS to standard output
//
// Inputs: None
//
// Outputs: None
//
//===========================================================================
void PrintUsage(void)
{
	printf("Usage: hsailcs <Configuration File>\n");
	printf("Example: hsailcs hsailcsscripts\\full.hsailcs\n");
	printf("\n");
}

//===========================================================================
//
// Name: main
//
// Description: Entry Point for Hsail Conformance Suite
//
// Inputs: argc - Contains the number of input arguments to the HSAILCS
//         argv[0] - Should always be "hsailcs"
//         argv[1] - This is a string which points at the configuration file to run
// Outputs: 0 = Success
//          1 = Failure
//
//===========================================================================
int main(int argc, char * argv[])
{
	if(argc != 2)
	{
		printf("ERROR: Not Enough Arguments\n");
		PrintUsage();
		exit(1);
	}

	if (!ProcessHSAILCSConfigurationFile(argv[1]))
		exit(1);

	exit(0);
}
