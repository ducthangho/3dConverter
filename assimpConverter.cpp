#include "3dConverter.h"

const char* AICMD_MSG_ABOUT = 
"------------------------------------------------------ \n"
"Open Asset Import Library (\"Assimp\", https://github.com/assimp/assimp) \n"
" -- Commandline toolchain --\n"
"------------------------------------------------------ \n\n"

"Version %i.%i %s%s%s%s%s(GIT commit %x)\n\n";

const char* AICMD_MSG_HELP = 
"assimp <verb> <parameters>\n\n"
" verbs:\n"
" \tinfo       - Quick file stats\n"
" \tlistext    - List all known file extensions available for import\n"
" \tknowext    - Check whether a file extension is recognized by Assimp\n"
#ifndef ASSIMP_BUILD_NO_EXPORT
" \texport     - Export a file to one of the supported output formats\n"
" \tlistexport - List all supported export formats\n"
" \texportinfo - Show basic information on a specific export format\n"
#endif
" \textract    - Extract embedded texture images\n"
" \tdump       - Convert models to a binary or textual dump (ASSBIN/ASSXML)\n"
" \tcmpdump    - Compare dumps created using \'assimp dump <file> -s ...\'\n"
" \tversion    - Display Assimp version\n"
"\n Use \'assimp <verb> --help\' for detailed help on a command.\n"
;

/*extern*/ Assimp::Importer* globalImporter = NULL;

#ifndef ASSIMP_BUILD_NO_EXPORT
/*extern*/ Assimp::Exporter* globalExporter = NULL;
#endif

// ------------------------------------------------------------------------------
// Application entry point
// int main (int argc, char* argv[])
// {
// 	if (argc <= 1)	{
// 		printf("assimp: No command specified. Use \'assimp help\' for a detailed command list\n");
// 		return 0;
// 	}

// 	// assimp version
// 	// Display version information
// 	if (! strcmp(argv[1], "version")) {
// 		const unsigned int flags = aiGetCompileFlags();
// 		printf(AICMD_MSG_ABOUT,
// 			aiGetVersionMajor(),
// 			aiGetVersionMinor(),
// 			(flags & ASSIMP_CFLAGS_DEBUG ?			"-debug "   : ""),
// 			(flags & ASSIMP_CFLAGS_NOBOOST ?		"-noboost " : ""),
// 			(flags & ASSIMP_CFLAGS_SHARED ?			"-shared "  : ""),
// 			(flags & ASSIMP_CFLAGS_SINGLETHREADED ? "-st "      : ""),
// 			(flags & ASSIMP_CFLAGS_STLPORT ?		"-stlport " : ""),
// 			aiGetVersionRevision());

// 		return 0;
// 	}

// 	// assimp help
// 	// Display some basic help (--help and -h work as well 
// 	// because people could try them intuitively)
// 	if (!strcmp(argv[1], "help") || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
// 		printf("%s",AICMD_MSG_HELP);
// 		return 0;
// 	}

// 	// assimp cmpdump
// 	// Compare two mini model dumps (regression suite) 
// 	if (! strcmp(argv[1], "cmpdump")) {
// 		return Assimp_CompareDump (&argv[2],argc-2);
// 	}

// 	// construct global importer and exporter instances
// 	Assimp::Importer imp;
// 	imp.SetPropertyBool("GLOB_MEASURE_TIME",true);
// 	globalImporter = &imp;

// #ifndef ASSIMP_BUILD_NO_EXPORT
// 	// 
// 	Assimp::Exporter exp;
// 	globalExporter = &exp;
// #endif

// 	// assimp listext
// 	// List all file extensions supported by Assimp
// 	if (! strcmp(argv[1], "listext")) {
// 		aiString s;
// 		imp.GetExtensionList(s);

// 		printf("%s\n",s.data);
// 		return 0;
// 	}

// #ifndef ASSIMP_BUILD_NO_EXPORT
// 	// assimp listexport
// 	// List all export file formats supported by Assimp (not the file extensions, just the format identifiers!)
// 	if (! strcmp(argv[1], "listexport")) {
// 		aiString s;
		
// 		for(size_t i = 0, end = exp.GetExportFormatCount(); i < end; ++i) {
// 			const aiExportFormatDesc* const e = exp.GetExportFormatDescription(i);
// 			s.Append( e->id );
// 			if (i!=end-1) {
// 				s.Append("\n");
// 			}
// 		}

// 		printf("%s\n",s.data);
// 		return 0;
// 	}


// 	// assimp exportinfo
// 	// stat an export format
// 	if (! strcmp(argv[1], "exportinfo")) {
// 		aiString s;

// 		if (argc<3) {
// 			printf("Expected file format id\n");
// 			return -11;
// 		}

// 		for(size_t i = 0, end = exp.GetExportFormatCount(); i < end; ++i) {
// 			const aiExportFormatDesc* const e = exp.GetExportFormatDescription(i);
// 			if (!strcmp(e->id,argv[2])) {
// 				printf("%s\n%s\n%s\n",e->id,e->fileExtension,e->description);
// 				return 0;
// 			}
// 		}
		
// 		printf("Unknown file format id: \'%s\'\n",argv[2]);
// 		return -12;
// 	}

// 	// assimp export
// 	// Export a model to a file
// 	if (! strcmp(argv[1], "export")) {
// 		return Assimp_Export (&argv[2],argc-2);
// 	}

// #endif

// 	// assimp knowext
// 	// Check whether a particular file extension is known by us, return 0 on success
// 	if (! strcmp(argv[1], "knowext")) {
// 		if (argc<3) {
// 			printf("Expected file extension");
// 			return -10;
// 		}
// 		const bool b = imp.IsExtensionSupported(argv[2]);
// 		printf("File extension \'%s\'  is %sknown\n",argv[2],(b?"":"not "));
// 		return b?0:-1;
// 	}

// 	// assimp info
// 	// Print basic model statistics
// 	if (! strcmp(argv[1], "info")) {
// 		return Assimp_Info ((const char**)&argv[2],argc-2);
// 	}

// 	// assimp dump 
// 	// Dump a model to a file 
// 	if (! strcmp(argv[1], "dump")) {
// 		return Assimp_Dump (&argv[2],argc-2);
// 	}

// 	// assimp extract 
// 	// Extract an embedded texture from a file
// 	if (! strcmp(argv[1], "extract")) {
// 		return Assimp_Extract (&argv[2],argc-2);
// 	}

// 	// assimp testbatchload
// 	// Used by /test/other/streamload.py to load a list of files
// 	// using the same importer instance to check for incompatible
// 	// importers.
// 	if (! strcmp(argv[1], "testbatchload")) {
// 		return Assimp_TestBatchLoad (&argv[2],argc-2);
// 	}

// 	printf("Unrecognized command. Use \'assimp help\' for a detailed command list\n");
// 	return 1;
// }


// ------------------------------------------------------------------------------
void SetLogStreams(const ImportData& imp)
{
	printf("\nAttaching log stream   ...           OK\n");
		
	unsigned int flags = 0;
	if (imp.logFile.length()) {
		flags |= aiDefaultLogStream_FILE;
	}
	if (imp.showLog) {
		flags |= aiDefaultLogStream_STDERR;
	}
	DefaultLogger::create(imp.logFile.c_str(),imp.verbose ? Logger::VERBOSE : Logger::NORMAL,flags);
}


// ------------------------------------------------------------------------------
void FreeLogStreams()
{
	DefaultLogger::kill();
}


// ------------------------------------------------------------------------------
void PrintHorBar()
{
	printf("-----------------------------------------------------------------\n");
}

// ------------------------------------------------------------------------------
// Import a specific file
const aiScene* ImportModel(
	const ImportData& imp, 
	const std::string& path)
{
	// Attach log streams
	if (imp.log) {
		SetLogStreams(imp);
	}
	printf("Launching asset import ...           OK\n");

	// Now validate this flag combination
	if(!globalImporter->ValidateFlags(imp.ppFlags)) {
		printf("ERROR: Unsupported post-processing flags \n");
		return NULL;
	}
	printf("Validating postprocessing flags ...  OK\n");
	if (imp.showLog) {
		PrintHorBar();
	}
		

	// do the actual import, measure time
	const clock_t first = clock();
	const aiScene* scene = globalImporter->ReadFile(path,imp.ppFlags);

	if (imp.showLog) {
		PrintHorBar();
	}
	if (!scene) {
		printf("ERROR: Failed to load file: %s\n", globalImporter->GetErrorString());
		return NULL;
	}

	const clock_t second = ::clock();
	const double seconds = static_cast<double>(second-first) / CLOCKS_PER_SEC;

	printf("Importing file ...                   OK \n   import took approx. %.5f seconds\n"
		"\n",seconds);

	if (imp.log) { 
		FreeLogStreams();
	}
	return scene;
}

#ifndef ASSIMP_BUILD_NO_EXPORT
// ------------------------------------------------------------------------------
bool ExportModel(const aiScene* pOut,  
	const ImportData& imp, 
	const std::string& path,
	const char* pID)
{
	// Attach log streams
	if (imp.log) {
		SetLogStreams(imp);
	}
	printf("Launching asset export ...           OK\n");

	if (imp.showLog) {
		PrintHorBar();
	}

	// do the actual export, measure time
	const clock_t first = clock();
	const aiReturn res = globalExporter->Export(pOut,pID,path);

	if (imp.showLog) {
		PrintHorBar();
	}
	if (res != AI_SUCCESS) {
		printf("Failed to write file\n");
		printf("ERROR: %s\n", globalExporter->GetErrorString());
		return false;
	}

	const clock_t second = ::clock();
	const double seconds = static_cast<double>(second-first) / CLOCKS_PER_SEC;

	printf("Exporting file ...                   OK \n   export took approx. %.5f seconds\n"
		"\n",seconds);

	if (imp.log) { 
		FreeLogStreams();
	}

	return true;
}
#endif

// ------------------------------------------------------------------------------
// Process standard arguments
int ProcessStandardArguments(
	ImportData& fill, 
	const char* const * params,
	unsigned int num)
{
	// -ptv    --pretransform-vertices
	// -gsn    --gen-smooth-normals
	// -gn     --gen-normals
	// -cts    --calc-tangent-space
	// -jiv    --join-identical-vertices
	// -rrm    --remove-redundant-materials
	// -fd     --find-degenerates
	// -slm    --split-large-meshes
	// -lbw    --limit-bone-weights
	// -vds    --validate-data-structure
	// -icl    --improve-cache-locality
	// -sbpt   --sort-by-ptype
	// -lh     --convert-to-lh
	// -fuv    --flip-uv
	// -fwo    --flip-winding-order
	// -tuv    --transform-uv-coords
	// -guv    --gen-uvcoords
	// -fid    --find-invalid-data
	// -fixn   --fix normals
	// -tri    --triangulate
	// -fi     --find-instances
	// -og     --optimize-graph
	// -om     --optimize-meshes
	// -db     --debone
	// -sbc    --split-by-bone-count
	// -gs	   --global-scale
	//
	// -c<file> --config-file=<file>

	for (unsigned int i = 0; i < num;++i) 
	{
        const char *param = params[ i ];
        printf( "param = %s\n", param );
		if (! strcmp( param, "-ptv") || ! strcmp( param, "--pretransform-vertices")) {
			fill.ppFlags |= aiProcess_PreTransformVertices;
		}
		else if (! strcmp( param, "-gsn") || ! strcmp( param, "--gen-smooth-normals")) {
			fill.ppFlags |= aiProcess_GenSmoothNormals;
		}
    else if (! strcmp( param, "-dn") || ! strcmp( param, "--drop-normals")) {
			fill.ppFlags |= aiProcess_DropNormals;
		}
		else if (! strcmp( param, "-gn") || ! strcmp( param, "--gen-normals")) {
			fill.ppFlags |= aiProcess_GenNormals;
		}
		else if (! strcmp( param, "-jiv") || ! strcmp( param, "--join-identical-vertices")) {
			fill.ppFlags |= aiProcess_JoinIdenticalVertices;
		}
		else if (! strcmp( param, "-rrm") || ! strcmp( param, "--remove-redundant-materials")) {
			fill.ppFlags |= aiProcess_RemoveRedundantMaterials;
		}
		else if (! strcmp( param, "-fd") || ! strcmp( param, "--find-degenerates")) {
			fill.ppFlags |= aiProcess_FindDegenerates;
		}
		else if (! strcmp( param, "-slm") || ! strcmp( param, "--split-large-meshes")) {
			fill.ppFlags |= aiProcess_SplitLargeMeshes;
		}
		else if (! strcmp( param, "-lbw") || ! strcmp( param, "--limit-bone-weights")) {
			fill.ppFlags |= aiProcess_LimitBoneWeights;
		}
		else if (! strcmp( param, "-vds") || ! strcmp( param, "--validate-data-structure")) {
			fill.ppFlags |= aiProcess_ValidateDataStructure;
		}
		else if (! strcmp( param, "-icl") || ! strcmp( param, "--improve-cache-locality")) {
			fill.ppFlags |= aiProcess_ImproveCacheLocality;
		}
		else if (! strcmp( param, "-sbpt") || ! strcmp( param, "--sort-by-ptype")) {
			fill.ppFlags |= aiProcess_SortByPType;
		}
		else if (! strcmp( param, "-lh") || ! strcmp( param, "--left-handed")) {
			fill.ppFlags |= aiProcess_ConvertToLeftHanded;
		}
		else if (! strcmp( param, "-fuv") || ! strcmp( param, "--flip-uv")) {
			fill.ppFlags |= aiProcess_FlipUVs;
		}
		else if (! strcmp( param, "-fwo") || ! strcmp( param, "--flip-winding-order")) {
			fill.ppFlags |= aiProcess_FlipWindingOrder;
		}
		else if (! strcmp( param, "-tuv") || ! strcmp( param, "--transform-uv-coords")) {
			fill.ppFlags |= aiProcess_TransformUVCoords;
		}
		else if (! strcmp( param, "-guv") || ! strcmp( param, "--gen-uvcoords")) {
			fill.ppFlags |= aiProcess_GenUVCoords;
		}
		else if (! strcmp( param, "-fid") || ! strcmp( param, "--find-invalid-data")) {
			fill.ppFlags |= aiProcess_FindInvalidData;
		}
		else if (! strcmp( param, "-fixn") || ! strcmp( param, "--fix-normals")) {
			fill.ppFlags |= aiProcess_FixInfacingNormals;
		}
		else if (! strcmp( param, "-tri") || ! strcmp( param, "--triangulate")) {
			fill.ppFlags |= aiProcess_Triangulate;
		}
		else if (! strcmp( param, "-cts") || ! strcmp( param, "--calc-tangent-space")) {
			fill.ppFlags |= aiProcess_CalcTangentSpace;
		}
		else if (! strcmp( param, "-fi") || ! strcmp( param, "--find-instances")) {
			fill.ppFlags |= aiProcess_FindInstances;
		}
		else if (! strcmp( param, "-og") || ! strcmp( param, "--optimize-graph")) {
			fill.ppFlags |= aiProcess_OptimizeGraph;
		}
		else if (! strcmp( param, "-om") || ! strcmp( param, "--optimize-meshes")) {
			fill.ppFlags |= aiProcess_OptimizeMeshes;
		}
		else if (! strcmp( param, "-db") || ! strcmp( param, "--debone")) {
			fill.ppFlags |= aiProcess_Debone;
		}
		else if (! strcmp( param, "-sbc") || ! strcmp( param, "--split-by-bone-count")) {
			fill.ppFlags |= aiProcess_SplitByBoneCount;
		}
		else if (!strcmp(param, "-embtex") || ! strcmp(param, "--embed-textures")) {
			fill.ppFlags |= aiProcess_EmbedTextures;
		}
		else if (!strcmp(param, "-gs") || ! strcmp(param, "--global-scale")) {
			fill.ppFlags |= aiProcess_GlobalScale;
		}
		else if (! strncmp( param, "-c",2) || ! strncmp( param, "--config=",9)) {
			const unsigned int ofs = (params[i][1] == '-' ? 9 : 2);

			// use default configurations
            if (!strncmp( param + ofs, "full", 4 )) {
                fill.ppFlags |= aiProcessPreset_TargetRealtime_MaxQuality;
            } else if (!strncmp( param + ofs, "default", 7 )) {
				fill.ppFlags |= aiProcessPreset_TargetRealtime_Quality;
			} else if (! strncmp( param +ofs,"fast",4)) {
				fill.ppFlags |= aiProcessPreset_TargetRealtime_Fast;
			}
		} else if (! strcmp( param, "-l") || ! strcmp( param, "--show-log")) {
			fill.showLog = true;
		}
		else if (! strcmp( param, "-v") || ! strcmp( param, "--verbose")) {
			fill.verbose = true;
		}
		else if (! strncmp( param, "--log-out=",10) || ! strncmp( param, "-lo",3)) {
			fill.logFile = std::string(params[i]+(params[i][1] == '-' ? 10 : 3));
			if (!fill.logFile.length()) {
				fill.logFile = "assimp-log.txt";
			}
		}
	}

	if (fill.logFile.length() || fill.showLog || fill.verbose) {
		fill.log = true;
	}

	return 0;
}

// ------------------------------------------------------------------------------
int Assimp_TestBatchLoad (
	const char* const* params, 
	unsigned int num)
{
	for(unsigned int i = 0; i < num; ++i) {
		globalImporter->ReadFile(params[i],aiProcessPreset_TargetRealtime_MaxQuality);
		// we're totally silent. scene destructs automatically.
	}
	return 0;
}