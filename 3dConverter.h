// 3dConverter.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#ifndef CONVETER3D_INCLUDED
#define CONVETER3D_INCLUDED

#include <iostream>

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits>

#include <assimp/postprocess.h>
#include <assimp/version.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>

#ifndef ASSIMP_BUILD_NO_EXPORT
#	include <assimp/Exporter.hpp>
#endif

#ifdef ASSIMP_BUILD_NO_OWN_ZLIB
#include <zlib.h>
#else
#include <zlib.h>
#endif


#ifndef SIZE_MAX
#	define SIZE_MAX (std::numeric_limits<size_t>::max())
#endif


using namespace Assimp;


// Global assimp importer instance
extern Assimp::Importer* globalImporter;

#ifndef ASSIMP_BUILD_NO_EXPORT
// Global assimp exporter instance
extern Assimp::Exporter* globalExporter;
#endif

// ------------------------------------------------------------------------------
/// Defines common import parameters
struct ImportData {
	ImportData()
		:	ppFlags	(0)
		,	showLog (false)
		,	verbose (false)
		,	log	    (false)
	{}

	/// Post-processing flags
	unsigned int ppFlags;

	// Log to std::err?
	bool showLog;

	// Log file
	std::string logFile;

	// Verbose log mode?
	bool verbose;

	// Need to log?
	bool log;
};

// ------------------------------------------------------------------------------
/** Process standard arguments
 *
 *  @param fill Filled by function
 *  @param params Command line parameters to be processed
 *  @param num NUmber of params
 *  @return 0 for success */
int ProcessStandardArguments(ImportData& fill, 
	const char* const* params,
	unsigned int num);

// ------------------------------------------------------------------------------
/** Import a specific model file
 *  @param imp Import configuration to be used
 *  @param path Path to the file to be read */
const aiScene* ImportModel(
	const ImportData& imp, 
	const std::string& path);

#ifndef ASSIMP_BUILD_NO_EXPORT

// ------------------------------------------------------------------------------
/** Export a specific model file
 *  @param imp Import configuration to be used
 *  @param path Path to the file to be written
 *  @param format Format id*/
bool ExportModel(const aiScene* pOut, 
	const ImportData& imp, 
	const std::string& path,
	const char* pID);

#endif

// ------------------------------------------------------------------------------
/** assimp_dump utility
 *  @param params Command line parameters to 'assimp dumb'
 *  @param Number of params
 *  @return 0 for success*/
int Assimp_Dump (
	const char* const* params, 
	unsigned int num);

// ------------------------------------------------------------------------------
/** assimp_export utility
 *  @param params Command line parameters to 'assimp export'
 *  @param Number of params
 *  @return 0 for success*/
int Assimp_Export (
	const char* const* params, 
	unsigned int num);

// ------------------------------------------------------------------------------
/** assimp_extract utility
 *  @param params Command line parameters to 'assimp extract'
 *  @param Number of params
 *  @return 0 for success*/
int Assimp_Extract (
	const char* const* params, 
	unsigned int num);

// ------------------------------------------------------------------------------
/** assimp_cmpdump utility
 *  @param params Command line parameters to 'assimp cmpdump'
 *  @param Number of params
 *  @return 0 for success*/
int Assimp_CompareDump (
	const char* const* params, 
	unsigned int num);

// ------------------------------------------------------------------------------
/** @brief assimp info utility
 *  @param params Command line parameters to 'assimp info'
 *  @param Number of params
 *  @return 0 for success */
int Assimp_Info (
	const char* const* params, 
	unsigned int num);

// ------------------------------------------------------------------------------
/** @brief assimp testbatchload utility
 *  @param params Command line parameters to 'assimp testbatchload'
 *  @param Number of params
 *  @return 0 for success */
int Assimp_TestBatchLoad (
	const char* const* params, 
	unsigned int num);


#endif // !! AICMD_MAIN_INCLUDED

// TODO: Reference additional headers your program requires here.
