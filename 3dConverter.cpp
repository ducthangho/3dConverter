// 3dConverter.cpp : Defines the entry point for the application.
//

#include "3dConverter.h"
#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>		// Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <clipp.h>
#include <assimp/DefaultLogger.hpp>
#include <plf_nanotimer.h>

#include <ifcpp/IFC4/include/IfcBuildingStorey.h>
#include <ifcpp/IFC4/include/IfcGloballyUniqueId.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/reader/ReaderSTEP.h>


using namespace std;

const std::string test_ifc_file = "/home/ducthangho/assimp/test/models/IFC/AC14-FZK-Haus.ifc";

// Prototype for conversion functions
std::wstring StringToWString(const std::string& s);
std::string WStringToString(const std::wstring& s);

std::wstring StringToWString(const std::string& s)
{
    std::wstring temp(s.length(),L' ');
    std::copy(s.begin(), s.end(), temp.begin());
    return temp; 
}


std::string WStringToString(const std::wstring& s)
{
    std::string temp(s.length(), ' ');
    std::copy(s.begin(), s.end(), temp.begin());
    return temp; 
}


bool DoTheImportThingIfcPP(const std::string &pFile){
	// create an IFC model and a reader for IFC files in STEP format:
	shared_ptr<BuildingModel> ifc_model(new BuildingModel());
	shared_ptr<ReaderSTEP> step_reader(new ReaderSTEP());

	// load the model:
	step_reader->loadModelFromFile( StringToWString(pFile), ifc_model);

	// get a map of all loaded IFC entities:
	const std::map<int, shared_ptr<BuildingEntity> >& map_entities = ifc_model->getMapIfcEntities();

	for (auto it : map_entities)
	{
		shared_ptr<BuildingEntity> entity = it.second;
		
		// check for certain type of the entity:
		shared_ptr<IfcBuildingStorey> ifc_storey = dynamic_pointer_cast<IfcBuildingStorey>(entity);
		if (ifc_storey)
		{
			// access attributes:
			// if (ifc_storey->m_GlobalId)
			// {
			// 	std::wcout << L"found IfcBuildingStorey entity with GUID: " << ifc_storey->m_GlobalId->m_value << std::endl;
			// }
		}
	}
	std::cout<<"Finish import from ifc++ "<<endl;
}

bool DoTheImportThing(const std::string &pFile)
{		
	plf::nanotimer timer;
	// Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// propably to request more postprocessing than we do in this example.
	cout << "Importing IFC file " << pFile << endl;
	// Create a logger instance 
	Assimp::DefaultLogger::create("",Assimp::Logger::VERBOSE);
	// Now I am ready for logging my stuff
	Assimp::DefaultLogger::get()->info("this is my info-call");

	timer.start();
	const aiScene *scene = importer.ReadFile(pFile,
											 aiProcess_CalcTangentSpace |
												 aiProcess_Triangulate |
												 aiProcess_JoinIdenticalVertices |
												 aiProcess_SortByPType);

	// If the import failed, report it
	if (!scene)
	{		
		//DoTheErrorLogging(importer.GetErrorString());
		cout << importer.GetErrorString() << endl;
		// Kill it after the work is done
		Assimp::DefaultLogger::kill();
		return false;
	}

	cout<<"Number of meshes = "<<scene->mMeshes<<endl;
	// Now we can access the file's contents.
	//DoTheSceneProcessing(scene);
	double results = timer.get_elapsed_ns();
	std::cout << "Timing import: " << results << " nanoseconds." << std::endl;
	cout << "Import successful." << endl;
	// We're done. Everything will be cleaned up by the importer destructor
	// Kill it after the work is done
	Assimp::DefaultLogger::kill();
	std::cout<<"Finish import from assimp "<<endl;
	return true;
}

int main(int argc, char *argv[])
{
	using namespace clipp;
	bool rec = false, utf16 = false;
	std::vector<int> tools;
	string infile = "", fmt = "gltf";

	auto tool = (option("-tool", "--tool") & value("library", tools)).doc("0 = assimp(default) ;  1 = ifc++  ;   2= magnum");

	auto cli = (value("input file", infile),
				tool,
				option("-o") & value("output format", fmt));

	if (!parse(argc, argv, cli))
		cout << make_man_page(cli, argv[0]);
	// ...

	if (tools.size() == 0)
	{
		// DoTheImportThing(infile);
		DoTheImportThingIfcPP(infile);
	}
	else
	{
		for (int i = 0; i < tools.size(); ++i)
		{
			auto tool = tools[i];
			std::cout << "Import by " << tool << endl;
			if (tool == 0)
			{
				// DoTheImportThing(infile);
				DoTheImportThingIfcPP(infile);
			}
		}
	}
	cout << "Hello World BC." << endl;
	return 0;
}
