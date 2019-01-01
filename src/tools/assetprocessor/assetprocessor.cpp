/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/


// AssetProcessorDLL.cpp : Defines the entry point for the DLL application.
//

#include "pch.hpp"
#include "assetprocessor.hpp"
#include "asset_processor_script.hpp"
#include "cstdmf/processor_affinity.hpp"
#include "cstdmf/bw_util.hpp"
#include "pyscript/script.hpp"
#include <direct.h>


#ifdef _MANAGED
#pragma managed(push, off)
#endif

ASSETPROCESSOR_API void init_AssetProcessor()
{	
	uint32 processor = 0;
	FILE* f;

	//read current affinity
	f = bw_fopen( "processor_affinity.bin", "rb" );
	if (f)
	{
		fread( &processor, sizeof(processor), 1, f );
		fclose(f);
	}

	processor = processor+1;
	ProcessorAffinity::set(processor);
	uint32 actual = ProcessorAffinity::get();

	//write current affinity
	f = bw_fopen( "processor_affinity.bin", "wb" );
	fwrite( &actual, sizeof(actual), 1, f );
	fclose(f);	

	AssetProcessorScript::init();

	PyErr_Clear();

    return;
}


BOOL __stdcall DllMain( HANDLE hModule, DWORD Reason, LPVOID Reserved )
{
	// The lpvReserved parameter to DllMain is NULL if the DLL is being 
	// unloaded because of a call to FreeLibrary, it?s non NULL if the 
	// DLL is being unloaded due to process termination.
	// http://blogs.msdn.com/larryosterman/archive/2004/06/10/152794.aspx
	//
	// Basically, the order in which DLLs get unloaded on process termination
	// (as opposed to calling FreeLibrary explicitly) is not guaranteed. What 
	// was happening here is that D3D will get unloaded first before us causing
	// our D3D pointers to be invalid, which was making us crash on exit.
	//
	// This check is here as a fail-safe, but you should call _AssetProcessor.fini
	// explicitly from script instead of relying on the OS/display driver cleaning
	// everything up for us.
	if (Reason == DLL_PROCESS_DETACH && Reserved == 0)
	{
		AssetProcessorScript::fini();
	}
	return TRUE;
}


#ifdef _MANAGED
#pragma managed(pop)
#endif
