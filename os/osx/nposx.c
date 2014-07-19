/* -----------------------------------------------------------------------------
*
*  nposx.c
*
*  ANTz - realtime 3D data visualization tools for the real-world, based on NPE.
*
*  ANTz is hosted at http://openantz.com and NPE at http://neuralphysics.org
*
*  Written in 2010-2012 by Shane Saxon - contact@saxondigital.net
*
*  Please see main.c for a complete list of additional code contributors.
*
*  To the extent possible under law, the author(s) have dedicated all copyright 
*  and related and neighboring rights to this software to the public domain
*  worldwide. This software is distributed without any warranty.
*
*  Released under the CC0 license, which is GPL compatible.
*
*  You should have received a copy of the CC0 Public Domain Dedication along
*  with this software (license file named COPYING.txt). If not, see
*  http://creativecommons.org/publicdomain/zero/1.0/
*
* --------------------------------------------------------------------------- */

#include "nposx.h"


#ifdef NP_OSX_

#include <Carbon/Carbon.h>
#include <sys/time.h>                // for gettimeofday()

// current working directory is OS specific
//-----------------------------------------------------------------------------
void npOSXGetCWD (char* buffer, int* size)
{
	//using the linux method
}

//-----------------------------------------------------------------------------
void npOSXSetCWD (char* buffer)  //voidptr
{

}

//-----------------------------------------------------------------------------
void npOSXGetAppPath (char* buffer, int* size)
{
	int result = 0;
	static bool firstTimeCalled = true;
	char* chPtr;
	static char appPath[kNPmaxPath];

	if (firstTimeCalled)
	{
		result = _NSGetExecutablePath (appPath, &size);
		if (result)
			printf ("err 3288 - invalid app path\n");
		
		chPtr = strrchr (appPath, '/');		//find the end of the path
		if (chPtr != NULL)					//we cut off the app exec name 
		{
			chPtr++;
			*chPtr = '\0';					//terminate to make a C string
			printf ("\nApp Path : %s", appPath);
			
			//copy the app path to the passed in buffer and set the size
			strcpy (buffer, appPath);
			*size = strlen (buffer);
			
			firstTimeCalled = false;
		}
		else
			printf ("err 3289 - invalid app path\n");
    }
	else 
	{
		//copy the app path to the passed in buffer and set the size
		strcpy (buffer, appPath);
		*size = strlen (buffer);
	}
}

//-----------------------------------------------------------------------------
void npOSXGetOpenFilePath (char* buffer, int* size, void* dataRef)
{

}


//-----------------------------------------------------------------------------
void npOSXSetOpenFilePath (char* buffer, void* dataRef)
{

}

//-----------------------------------------------------------------------------
FILE* npOSXFileDialog (const char* fileName, int dialogType, void* dataRef)
{																			//zz-osx debug
	 FILE* filePtr = NULL;
	
		NavDialogCreationOptions dialogOptions;
		NavDialogRef dialog;
		NavReplyRecord replyRecord;
		CFURLRef fileAsCFURLRef = NULL;
		FSRef fileAsFSRef;
		OSStatus status;
		bool result = false;
	
		unsigned char filePath[1024];
		char msg[4096];
		
		// Get the standard set of defaults
		status = NavGetDefaultDialogCreationOptions (&dialogOptions);
			require_noerr( status, CantGetNavOptions );
		
		// Make the	window app-wide modal
		dialogOptions.modality = kWindowModalityAppModal;
	
	//	dialogOptions.location = fileName;
		
		// Create the dialog
		status = NavCreateGetFileDialog (&dialogOptions, NULL, NULL, NULL, NULL, NULL, &dialog);
			require_noerr( status, CantCreateDialog );
		
		// Show it
		status = NavDialogRun (dialog);
			require_noerr( status, CantRunDialog );
		
		// Get the reply
		status = NavDialogGetReply (dialog, &replyRecord);
		require( ((status == noErr) || (status == userCanceledErr)), CantGetReply );
		
		// If the user clicked "Cancel", just bail
		if ( status == userCanceledErr ) goto UserCanceled;
		
		// Get the file
		status = AEGetNthPtr ( &(replyRecord.selection), 1, typeFSRef, NULL, NULL, &fileAsFSRef, sizeof(FSRef), NULL);
			require_noerr( status, CantExtractFSRef );
		
		// Convert it to a CFURL
		fileAsCFURLRef = CFURLCreateFromFSRef(NULL, &fileAsFSRef);
	
	result = CFURLGetFileSystemRepresentation(fileAsCFURLRef, true, filePath, 1024);
	
	if (!result)
		npPostMsg("err 9824 - cannot convert file dialog path", kNPmsgErr, dataRef);
	else
	{
		// printf ("\nFile Path: %s\n", filePath);
		
		sprintf (msg, "%s", filePath);
		
		filePtr = fopen (msg, "r");
	
		if (filePtr != NULL)
		{
			sprintf (msg, "File Open: %s", filePath);
			npPostMsg (msg, kNPmsgCtrl, dataRef);
		}
		else
			npPostMsg ("err 2995 - File Pointer is NULL", kNPmsgErr, dataRef);
	
		return filePtr;
	}
	
	// Cleanup
	CantExtractFSRef:
	UserCanceled:
		verify_noerr( NavDisposeReply(&replyRecord) );
	CantGetReply:
	CantRunDialog:
		NavDialogDispose(dialog);
	CantCreateDialog:
	CantGetNavOptions:
	// return fileAsCFURLRef;	//part of original sample, does not apply here
	
	return NULL;
}

//-----------------------------------------------------------------------------
void npOSXUpdateTime (void* dataRef)
{
	//using the linux method
}

//-----------------------------------------------------------------------------
void npOSXGetTime()
{

}

//-----------------------------------------------------------------------------
void npOSXSleep (double seconds)
{
	return;																//debug zz
}

//-----------------------------------------------------------------------------
void npOSXTimeStampCSV (char* fileName)
{
	return;																//debug zz
}

//-----------------------------------------------------------------------------
void npOSXSetCursorPos (int x, int y)
{
	return;																//debug zz
}

//-----------------------------------------------------------------------------
void npOSXShowCursor (int hide)
{
	return;																//debug zz
}


#endif

