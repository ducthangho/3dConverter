/*
Open Asset Import Library (assimp)
----------------------------------------------------------------------

Copyright (c) 2006-2019, assimp team


All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------
*/

/** @file  IFCLoad.cpp
 *  @brief Implementation of the Industry Foundation Classes loader.
 */


#ifndef ASSIMP_BUILD_NO_IFCASSIMPIMPORTER
#define ASSIMP_BUILD_NO_IFCASSIMPIMPORTER
#include <iterator>
#include <limits>
#include <tuple>

#ifndef ASSIMP_BUILD_NO_COMPRESSED_IFC
#  ifdef ASSIMP_USE_HUNTER
#    include <minizip/unzip.h>
#  else
#    include <minizip/unzip.h>
#  endif
#endif
#include <sys/types.h>
#include <sys/stat.h>

#include "IfcAssImpImporter.h"
// #include "../STEPParser/STEPFileReader.h"

// #include "IFCUtil.h"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/scene.h>
#include <assimp/importerdesc.h>
// #include <assimp/MemoryIOWrapper.h>

#include <osgUtil/Tessellator>
#include <osgUtil/Optimizer>
#include <osgUtil/SmoothingVisitor>
#include <osgViewer/Viewer>
// for developing only:
#ifdef _DEBUG
#define GEOMETRY_DEBUG_CHECK
#endif


#ifdef GEOMETRY_DEBUG_CHECK
	#include <ifcpp/geometry/Carve/GeomDebugDump.h>
#endif
#include <ifcpp/geometry/Carve/GeometryConverter.h>
#include <ifcpp/geometry/Carve/ConverterOSG.h>
#include <ifcpp/geometry/Carve/GeomUtils.h>

#include <ifcpp/geometry/SceneGraphUtils.h>
#include <ifcpp/geometry/GeometrySettings.h>


#include <ifcpp/IFC4/include/IfcBuildingStorey.h>
#include <ifcpp/IFC4/include/IfcGloballyUniqueId.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/reader/ReaderSTEP.h>
#include <flatbuffers/flatbuffers.h>
// #include <ifcpp/geometry/OCC/GeometryConverterOCC.h>

#ifndef SIZE_MAX
#	define SIZE_MAX (std::numeric_limits<size_t>::max())
#endif


// namespace Assimp {
//     template<> const char* LogFunctions<IfcAssImpImporter>::Prefix()
//     {
//         static auto prefix = "IFC: ";
//         return prefix;
//     }
// }

using namespace Assimp;
// using namespace Assimp::Formatter;
// using namespace Assimp::IFC;

/* DO NOT REMOVE this comment block. The genentitylist.sh script
 * just looks for names adhering to the IfcSomething naming scheme
 * and includes all matches in the whitelist for code-generation. Thus,
 * all entity classes that are only indirectly referenced need to be
 * mentioned explicitly.

  IfcRepresentationMap
  IfcProductRepresentation
  IfcUnitAssignment
  IfcClosedShell
  IfcDoor

 */

// namespace {


// forward declarations
// void SetUnits(ConversionData& conv);
// void SetCoordinateSpace(ConversionData& conv);
// void ProcessSpatialStructures(ConversionData& conv);
// void MakeTreeRelative(ConversionData& conv);
// void ConvertUnit(const shared_ptr<ReaderSTEP>& readerStep,ConversionData& conv);

// } // anon

// Prototype for conversion functions

std::wstring StringToWString(const std::string& s);
std::string WStringToString(const std::wstring& s);

inline std::wstring StringToWString(const std::string& s)
{
    std::wstring temp(s.length(),L' ');
    std::copy(s.begin(), s.end(), temp.begin());
    return temp; 
}


inline std::string WStringToString(const std::wstring& s)
{
    std::string temp(s.length(), ' ');
    std::copy(s.begin(), s.end(), temp.begin());
    return temp; 
}

static const aiImporterDesc desc = {
    "Industry Foundation Classes (IFC) Importer",
    "",
    "",
    "",
    aiImporterFlags_SupportBinaryFlavour,
    0,
    0,
    0,
    0,
    "ifc ifczip stp"
};


// ------------------------------------------------------------------------------------------------
// Constructor to be privately used by Importer
/*IfcAssImpImporter::IfcAssImpImporter()
{
   	m_ifc_model = shared_ptr<BuildingModel>( new BuildingModel() );
	m_geometry_converter = shared_ptr<GeometryConverter>( new GeometryConverter( m_ifc_model ) );
	m_step_reader = shared_ptr<ReaderSTEP>( new ReaderSTEP() );
	// m_step_writer = shared_ptr<WriterSTEP>( new WriterSTEP() );

   	m_rootnode = new osg::Group();
	m_rootnode->setName( "m_rootnode" );
	
	m_sw_model = new osg::Switch();
	m_sw_model->setName( "m_sw_model" );
	m_rootnode->addChild( m_sw_model.get() );

	m_sw_coord_axes = new osg::Switch();
	m_sw_coord_axes->setName( "m_sw_coord_axes" );
	m_rootnode->addChild( m_sw_coord_axes.get() );


}//*/

// ------------------------------------------------------------------------------------------------
// Destructor, private as well
/*IfcAssImpImporter::~IfcAssImpImporter()
{
}//*/

// ------------------------------------------------------------------------------------------------
// Returns whether the class can handle the format of the given file.
bool IfcAssImpImporter::CanRead( const std::string& pFile, IOSystem* pIOHandler, bool checkSig) const
{
    ASSIMP_LOG_INFO("My own CanRead");
    const std::string& extension = GetExtension(pFile);
    if (extension == "ifc" || extension == "ifczip" ) {
        return true;
    } else if ((!extension.length() || checkSig) && pIOHandler)   {
        // note: this is the common identification for STEP-encoded files, so
        // it is only unambiguous as long as we don't support any further
        // file formats with STEP as their encoding.
        const char* tokens[] = {"ISO-10303-21"};
        const bool found( SearchFileHeaderForToken( pIOHandler, pFile, tokens, 1 ) );
        return found;
    }
    return false;
}

// ------------------------------------------------------------------------------------------------
// List all extensions handled by this loader
const aiImporterDesc* IfcAssImpImporter::GetInfo () const
{
    return &desc;
}

// ------------------------------------------------------------------------------------------------
// Setup configuration properties for the loader
void IfcAssImpImporter::SetupProperties(const Importer* pImp)
{
    settings.skipSpaceRepresentations = pImp->GetPropertyBool(AI_CONFIG_IMPORT_IFC_SKIP_SPACE_REPRESENTATIONS,true);
    settings.useCustomTriangulation = pImp->GetPropertyBool(AI_CONFIG_IMPORT_IFC_CUSTOM_TRIANGULATION,true);
    settings.conicSamplingAngle = std::min(std::max((float) pImp->GetPropertyFloat(AI_CONFIG_IMPORT_IFC_SMOOTHING_ANGLE, AI_IMPORT_IFC_DEFAULT_SMOOTHING_ANGLE), 5.0f), 120.0f);
	settings.cylindricalTessellation = std::min(std::max(pImp->GetPropertyInteger(AI_CONFIG_IMPORT_IFC_CYLINDRICAL_TESSELLATION, AI_IMPORT_IFC_DEFAULT_CYLINDRICAL_TESSELLATION), 3), 180);
	settings.skipAnnotations = true;
}

size_t countEntities(osg::Node* node){    
    auto* geode = node->asGeode();
    if (geode){
        size_t count = 1+geode->getNumChildren();
        size_t num = 0;
        for (int i=0;i<geode->getNumChildren();++i){
            num += countEntities(geode->getChild(i));
        }
        return count+num;
    } 
    auto* tf = node->asTransform();
    if (tf){
        auto* mt = tf->asMatrixTransform();
        if (mt) {
            size_t num = 0;
            for (int i=0;i<mt->getNumChildren();++i){
                num += countEntities(mt->getChild(i));
            }
            return num;
        }
    }

    auto* geom = node->asGeometry();
    if (geom){
        size_t num =0;
        num +=  (geom->getNormalArray()) ? geom->getNormalArray()->getNumElements() : 0;
        num += (geom->getVertexArray()) ? geom->getVertexArray()->getNumElements() : 0; 
        num += (geom->getColorArray()) ? geom->getColorArray()->getNumElements() : 0;
        // auto* drawable = geom->asDrawable();
        // if (drawable) {            
        //     return 1;
        // }
        return num;
    }

    auto* group = node->asGroup();
    if (group){
         size_t num = 0;
        for (int i=0;i<group->getNumChildren();++i){
            num += countEntities(group->getChild(i));
        }        
        return num;
    }

    auto* sw = node->asSwitch();
    if (sw){
        size_t num = 0;
        for (int i=0;i<sw->getNumChildren();++i){
            num += countEntities(sw->getChild(i));
        }        
        return num;
    }
   
    return 1;
}


size_t countEntities(osg::ref_ptr<osg::Switch>& node){    
    size_t count = 1+node->getNumChildren();
    size_t num = 0;
    for (int i=0;i<node->getNumChildren();++i){
        num += countEntities(node->getChild(i));
    }
    return count+num;
}


// ------------------------------------------------------------------------------------------------
// Imports the given file into the given scene structure.
void IfcAssImpImporter::InternReadFile( const std::string& pFile, aiScene* pScene, IOSystem* pIOHandler)
{
    printf("MY CUSTOM IMPORTER\n");
    // create an IFC model and a reader for IFC files in STEP format:    
	osg::ref_ptr<osg::Switch> model_switch = getModelNode();
    SceneGraphUtils::clearAllChildNodes( model_switch );
    shared_ptr<GeometryConverter> geometry_converter = getGeometryConverter();
    geometry_converter->clearMessagesCallback();
	geometry_converter->resetModel();
    m_ifc_model->resetIfcModel();
	// load the model:
    try{
        m_step_reader->loadModelFromFile( StringToWString(pFile), m_ifc_model);        
        // convert IFC geometric representations into Carve geometry
        geometry_converter->convertGeometry();    
        
        // convert Carve geometry to OSG
		shared_ptr<ConverterOSG> converter_osg( new ConverterOSG( geometry_converter->getGeomSettings() ) );
		converter_osg->setMessageTarget( geometry_converter.get() );
		converter_osg->convertToOSG( geometry_converter->getShapeInputData(), model_switch );

		// in case there are IFC entities that are not in the spatial structure
		const std::map<int, shared_ptr<BuildingObject> >& objects_outside_spatial_structure = geometry_converter->getObjectsOutsideSpatialStructure();
		if( objects_outside_spatial_structure.size() > 0 )
		{
			osg::ref_ptr<osg::Switch> sw_objects_outside_spatial_structure = new osg::Switch();
			sw_objects_outside_spatial_structure->setName( "IfcProduct objects outside spatial structure" );

			converter_osg->addNodes( objects_outside_spatial_structure, sw_objects_outside_spatial_structure );
			if( sw_objects_outside_spatial_structure->getNumChildren() > 0 )
			{
				model_switch->addChild( sw_objects_outside_spatial_structure );
			}
		}

        if( model_switch )
		{
			bool optimize = true;
			if( optimize )
			{
				osgUtil::Optimizer opt;
				opt.optimize(model_switch);
			}

			// if model bounding sphere is far from origin, move to origin
			const osg::BoundingSphere& bsphere = model_switch->getBound();
			if( bsphere.center().length() > 10000 )
			{
				if( bsphere.center().length()/bsphere.radius() > 100 )
				{
					std::unordered_set<osg::Geode*> set_applied;
					SceneGraphUtils::translateGroup( model_switch, -bsphere.center(), set_applied );
				}
			}

            
            ConverterCarve2AssImp conv(m_step_reader, m_ifc_model, pScene,settings, geometry_converter->getGeomSettings()
                ,converter_osg->getMapEntityIdToSwitch(), converter_osg->getMapRepresentationToSwitch() );
            
            int totalEntities = m_ifc_model->getMapIfcEntities().size();
            size_t countNum = countEntities(model_switch);


            std::cout<<"Now traverse OSG node and convert to aiScene ---  "<< totalEntities <<" ***  "<<countNum  <<std::endl;
            conv.convertOSG2AssImpNode(getRootNode());
            // traverseOSGNodes(getRootNode(),pScene);
            // osgViewer::Viewer viewer;
            // viewer.setSceneData(getRootNode());
            // viewer.run();
            geometry_converter->clearInputCache();
		}

        // ConverterCarve2AssImp conv(m_step_reader, m_ifc_model, pScene,settings, geometry_converter->getGeomSettings());
        // conv.setMessageTarget(geometry_converter.get());
        // conv.convert( geometry_converter->getShapeInputData());
        // // get a map of all loaded IFC entities:
        // const std::map<int, shared_ptr<BuildingEntity> >& map_entities = m_ifc_model->getMapIfcEntities();

        // for (auto it : map_entities)
        // {
        // 	shared_ptr<BuildingEntity> entity = it.second;
            
        // 	// check for certain type of the entity:
        // 	shared_ptr<IfcBuildingStorey> ifc_storey = dynamic_pointer_cast<IfcBuildingStorey>(entity);
        // 	if (ifc_storey)		{
        // 		// access attributes:
        // 		if (ifc_storey->m_GlobalId)
        // 		{
        // 			std::wcout << L"found IfcBuildingStorey entity with GUID: " << ifc_storey->m_GlobalId->m_value << std::endl;
        // 		}
        // 	}
        // }
        
    } catch( OutOfMemoryException& e)
	{
		throw e;
	}
	catch( BuildingException& e )
	{
		std::cerr << e.what();
	}
	catch( std::exception& e )
	{
		std::cerr << e.what();
	}
	catch( ... )
	{
		std::cerr << "loadModelFromFile, createGeometryOSG failed" << std::endl;
	}
	    
}


#endif
