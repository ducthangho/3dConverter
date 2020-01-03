#ifndef AI_IFCCONVERT2ASSIMPIMP_H_INC
#define AI_IFCCONVERT2ASSIMPIMP_H_INC

#pragma once

#include <osg/CullFace>
#include <osg/Geode>
#include <osg/Hint>
#include <osg/LineWidth>
#include <osg/Material>
#include <osg/Point>
#include <osg/Geometry>
// #include <ifcpp/geometry/Carve/IncludeCarveHeaders.h>
// #include <osgUtil/Tessellator>

#include <assimp/code/Importer/IFC/IFCUtil.h>
#include <assimp/scene.h>
#include <ifcpp/reader/ReaderSTEP.h>
#include "ifcpp/IFC4/include/IfcRepresentationItem.h"
#include "ifcpp/IFC4/include/IfcSurfaceStyle.h"
#include <ifcpp/geometry/Carve/GeometryConverter.h>
#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/geometry/SceneGraphUtils.h>
// #include "IfcAssImpImporter.h"



using namespace std;
using namespace Assimp;
typedef std::map<std::string, osg::ref_ptr<osg::Texture> > TextureMap;

struct Settings
    {
        Settings()
            : skipSpaceRepresentations()
            , useCustomTriangulation()
            , skipAnnotations()
            , conicSamplingAngle(10.f)
			, cylindricalTessellation(32)
        {}


        bool skipSpaceRepresentations;
        bool useCustomTriangulation;
        bool skipAnnotations;
        float conicSamplingAngle;
		int cylindricalTessellation;
    };



static void convertOsgMtl(osg::ref_ptr<osg::Material> ta, aiMaterial& mat, float shininessStrength = 1.0f){
		aiString name(ta->getName());
		mat.AddProperty( &name,AI_MATKEY_NAME  );
		osg::Vec4f ambientColor;
		osg::Vec4f diffuseColor;
		osg::Vec4f specularColor;	

		ambientColor = ta->getAmbient(osg::Material::FRONT_AND_BACK);
		diffuseColor = ta->getDiffuse( osg::Material::FRONT_AND_BACK );
		specularColor = ta->getSpecular( osg::Material::FRONT_AND_BACK );
		float shininess = ta->getShininess( osg::Material::FRONT_AND_BACK );
		auto colMd = ta->getColorMode();
		float transparency = 1 - ambientColor[3];
		osg::Vec4f emissiveCol = ta->getEmission(osg::Material::FRONT_AND_BACK);
				
		mat.AddProperty(&ambientColor,1,AI_MATKEY_COLOR_AMBIENT);
		mat.AddProperty(&diffuseColor,1,AI_MATKEY_COLOR_DIFFUSE);
		mat.AddProperty(&specularColor,1,AI_MATKEY_COLOR_SPECULAR);
		mat.AddProperty(&shininess,1,AI_MATKEY_SHININESS);
		mat.AddProperty(&emissiveCol,1,AI_MATKEY_COLOR_EMISSIVE);
		mat.AddProperty(&transparency,1,AI_MATKEY_COLOR_TRANSPARENT);
		mat.AddProperty(&shininessStrength,1,AI_MATKEY_SHININESS_STRENGTH);
		// mat.AddProperty(&colMd,1,AI_MATKEY_)
		// mat.AddProperty(1,AI_MATKEY_COLOR)
	};

	static void convertOsgMtl(const osg::Texture& ta, aiMaterial& mat){
		size_t unit = ta.getMember();
		auto min_filter_mode = ta.getFilter(osg::Texture::MIN_FILTER);
		auto mag_filter_mode = ta.getFilter(osg::Texture::MAG_FILTER);
		auto wrap_mode_s = ta.getWrap(osg::Texture::WRAP_S);
		auto wrap_mode_t = ta.getWrap(osg::Texture::WRAP_T);
		auto wrap_mode_r = ta.getWrap(osg::Texture::WRAP_R);					 
		aiString name(ta.getName());
		aiColor4D col_base,col;
		// AppearanceData
		mat.AddProperty( &name,AI_MATKEY_NAME  );
		// mat->AddProperty(&col_base,1, AI_MATKEY_COLOR_DIFFUSE);
		// mat.AddProperty(AI_MATKEY_)

	}


static void traverseOSGNodes( osg::Node* node, aiScene* scene ) //, TextureMap& textures, const osgDB::Options* options
    {   		     				
        osg::Geode* geode = node->asGeode();
		std::cout<<"geode  "<<geode<<std::endl;        
        int nDrawnables = geode->getNumDrawables();		
		std::vector<aiNode> nodes;
		std::vector<aiMesh> meshes;	
		std::vector<aiMaterial> materials;			
			
        for ( unsigned int n=0; n<nDrawnables; ++n )
        {
            osg::Geometry* geom = geode->getDrawable(n)->asGeometry();
            // Create geometry basic properties
            // const struct aiMesh* mesh = aiScene->mMeshes[ aiNode->mMeshes[n] ];
            // osg::Geometry* geom = new osg::Geometry;
            // geode->addDrawable( geom );
			aiNode node;
			std::vector<unsigned int> mMeshes;			
			mMeshes.push_back(meshes.size());
			nodes.push_back(node);			
            aiMesh m;
			meshes.push_back(m);
			aiMesh& mesh = meshes.back();
            auto* va = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
            auto* na = dynamic_cast<osg::Vec3Array*>(geom->getNormalArray());
            auto* ca = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());
            // osg::Vec3Array* va = new osg::Vec3Array(mesh.mNumVertices);
            // osg::Vec3Array* na = (mesh.mNormals ? new osg::Vec3Array(mesh.mNumVertices) : NULL);
            // osg::Vec4Array* ca = (mesh.mColors[0] ? new osg::Vec4Array(mesh.mNumVertices) : NULL);            
			
            mesh.mPrimitiveTypes = aiPrimitiveType_POLYGON;
            mesh.mNumVertices = va->getNumElements();
            mesh.mVertices = new aiVector3D[mesh.mNumVertices];
            mesh.mNormals = new aiVector3D[mesh.mNumVertices];
            // mesh.mColors = new (aiColor4D*)[AI_MAX_NUMBER_OF_COLOR_SETS];
            mesh.mColors[0] = new aiColor4D[mesh.mNumVertices];//AI_MAX_NUMBER_OF_COLOR_SETS
            for ( size_t i=0; i<va->getNumElements(); ++i )
            {
                auto& v = (*va)[i];
                auto& n = (*na)[i];
                auto& c = (*na)[i];
                mesh.mVertices[i] = {v[0],v[1],v[2]};
                mesh.mNormals[i] = {n[0],n[1],n[2]};
                mesh.mColors[0][i] = {c[0],c[1],c[2],c[3]};                
            }
            
            
            // Create geometry texture coordinates
            auto numTextCoordsArrays = geom->getNumTexCoordArrays();
            unsigned int unit = 0;
            // auto* texCoord = dynamic_cast<osg::Vec2Array*>(geom->getTexCoordArray(unit));
            // const aiVector3D* aiTexCoords = mesh.mTextureCoords[unit];            
            // mesh.mTextureCoords = new[unit];
            for (size_t unit =0;unit<numTextCoordsArrays;++unit){
                auto* texCoord = geom->getTexCoordArray(unit);
				size_t numUVComponent = (size_t)texCoord->getElementSize()/sizeof(ai_real);
                if (!texCoord) continue;
                aiVector3D* aiTexCoords = new aiVector3D[mesh.mNumVertices];
                mesh.mTextureCoords[unit] = aiTexCoords;
				mesh.mNumUVComponents[unit] = numUVComponent;
                switch (numUVComponent){
                    case 1:
                    {
                        // osg::FloatArray* ta = new osg::FloatArray(mesh.mNumVertices);
                        osg::FloatArray* ta = dynamic_cast<osg::FloatArray*>(texCoord);
                        for ( unsigned int i=0; i<mesh.mNumVertices; ++i ){							
							aiTexCoords[i].x = (*ta)[i];
						}
                            
                        // geom->setTexCoordArray( unit, ta );
                    }
                    break;
                    case 2:
					{          
						osg::Vec2Array* ta = dynamic_cast<osg::Vec2Array*>(texCoord);
						for ( unsigned int i=0; i<mesh.mNumVertices; ++i ){						
							aiVector3D& t = aiTexCoords[i];
							t.x = (*ta)[i].x();
							t.y = (*ta)[i].y();
						}
						//     osg::Vec2Array* ta = new osg::Vec2Array(mesh.mNumVertices);
						// for ( unsigned int i=0; i<mesh.mNumVertices; ++i )
						// {
						//     const aiVector3D& t = aiTexCoords[i];
						//     (*ta)[i].set( t.x, t.y );
						// }
						// geom->setTexCoordArray( unit, ta );
					}
					break;
                    case 3:
					{
						osg::Vec3Array* ta = dynamic_cast<osg::Vec3Array*>(texCoord);
						for ( unsigned int i=0; i<mesh.mNumVertices; ++i ){
							aiVector3D& t = aiTexCoords[i];
							t.x = (*ta)[i].x();
							t.y = (*ta)[i].y();
							t.z = (*ta)[i].z();
						}
						// osg::Vec3Array* ta = new osg::Vec3Array(mesh.mNumVertices);
						// for ( unsigned int i=0; i<mesh.mNumVertices; ++i )
						// {
						//     const aiVector3D& t = aiTexCoords[i];
						//     (*ta)[i].set( t.x, t.y, t.z );
						// }
						// geom->setTexCoordArray( unit, ta );
					}
					break;
				}    
            }
			std::vector<aiFace> mfaces;
			// Create geometry primitives
			auto numPriSets = geom->getNumPrimitiveSets();
			for (size_t i=0;i<numPriSets;++i){
				auto* de = geom->getPrimitiveSet(i);				
				int mNumIndices = 0;
				aiFace face;
				int num;
				switch (de->getMode())
				{
				case osg::PrimitiveSet::POLYGON:/* constant-expression */
				case osg::PrimitiveSet::QUADS:
				case osg::PrimitiveSet::QUAD_STRIP:
				case osg::PrimitiveSet::TRIANGLES:
				case osg::PrimitiveSet::TRIANGLE_STRIP:
				case osg::PrimitiveSet::TRIANGLE_FAN:					
					num = de->getNumIndices();
					face.mNumIndices = num;
					face.mIndices = new unsigned int[num];
					for (int j=0;j<num;++j)
						face.mIndices[j] =  de->index(j);
					
					mfaces.push_back(face);
					break;
				case osg::PrimitiveSet::LINE_LOOP:
					mNumIndices = 2;
					/* code */
					break;
				case osg::PrimitiveSet::LINES:
					mNumIndices = 2;
					break;
				case osg::PrimitiveSet::POINTS:
					mNumIndices = 1;
					break;
				case osg::PrimitiveSet::LINE_STRIP:
					break;
				default:
					break;
				}
			}

			mesh.mNumFaces = mfaces.size();
			mesh.mFaces = new aiFace[mesh.mNumFaces];
			for (size_t i = 0;i<mesh.mNumFaces;++i){
				mesh.mFaces[i] = mfaces[i];
			}
			// osg::ref_ptr<osg::DrawElementsUInt>* d = geom->
			// osg::ref_ptr<osg::DrawElementsUInt> de[5];
			// de[1] = new osg::DrawElementsUInt(GL_POINTS);
			// de[2] = new osg::DrawElementsUInt(GL_LINES);
			// de[3] = new osg::DrawElementsUInt(GL_TRIANGLES);
			// de[4] = new osg::DrawElementsUInt(GL_QUADS);
			// de[0] = new osg::DrawElementsUInt(GL_POLYGON);
			
			// osg::DrawElementsUInt* current = NULL;
			// for ( unsigned int f=0; f<mesh.mNumFaces; ++f )
			// {
			//     const struct aiFace& face = mesh.mFaces[f];
			//     if ( face.mNumIndices>4 ) current = de[0].get();
			//     else current = de[face.mNumIndices].get();
				
			//     for ( unsigned i=0; i<face.mNumIndices; ++i )
			//         current->push_back( face.mIndices[i] );
			// }
			
			// for ( unsigned int i=0; i<5; ++i )
			// {
			//     if ( de[i]->size()>0 )
			//         geom->addPrimitiveSet( de[i].get() );
			// }
			// Create textures
		}
		osg::StateSet* ss = geode->getStateSet();
		auto& textureAttrLst = ss->getTextureAttributeList();
		auto& textureModeLst = ss->getTextureModeList();		

		osg::ref_ptr<osg::Material> mat_existing = (osg::Material*)ss->getAttribute(osg::StateAttribute::MATERIAL);
		aiMaterial mtl;					
		convertOsgMtl(mat_existing,mtl);
		materials.push_back(mtl);
		// for (size_t i =0;i<textureAttrLst.size();++i){
		// 	osg::StateSet::AttributeList& attrLst = textureAttrLst[i];
		// 	// std::map<GLenum, unsigned int>& mode = textureModeLst[i];
		// 	for (auto& attr:attrLst){
		// 		osg::StateAttribute::TypeMemberPair type = attr.first;
		// 		auto pair = attr.second;
		// 		osg::ref_ptr<osg::StateAttribute> sa = pair.first;
		// 		osg::StateAttribute::OverrideValue val = pair.second;
		// 		if (sa->isTextureAttribute()){
		// 			osg::Texture* ta = dynamic_cast<osg::TextureAttribute*>(sa.get())->asTexture();
		// 			size_t unit = ta->getMember();
		// 			auto min_filter_mode = ta->getFilter(osg::Texture::MIN_FILTER);
		// 			auto mag_filter_mode = ta->getFilter(osg::Texture::MAG_FILTER);
		// 			auto wrap_mode_s = ta->getWrap(osg::Texture::WRAP_S);
		// 			auto wrap_mode_t = ta->getWrap(osg::Texture::WRAP_T);
		// 			auto wrap_mode_r = ta->getWrap(osg::Texture::WRAP_R);					 

		// 			aiMaterial mtl;					
		// 			materials.push_back(mtl);
		// 		}
		// 	}
		// 	// if (attr.is)
			
		// }


		// const aiMaterial* aiMtl = scene->mMaterials[mesh.mMaterialIndex];
		// aiReturn texFound = AI_SUCCESS;
		// aiTextureOp envOp = aiTextureOp_Multiply;
		// aiTextureMapMode wrapMode[3] = {aiTextureMapMode_Clamp};
		// unsigned int texIndex = 0;
		// aiString path;
		
	/*	while ( texFound==AI_SUCCESS )
		{
			texFound = aiMtl->GetTexture(
				aiTextureType_DIFFUSE, texIndex++, &path, NULL, &unit, NULL, &envOp, &(wrapMode[0]) );
			if ( unit>0 ) unit--;  // The output UV seems to start at 1?
			if ( texFound!=AI_SUCCESS ) break;
			
			std::string texFile(path.data);
			if ( !osgDB::isAbsolutePath(texFile) ) texFile = getFullPath( filename, texFile );
			
			TextureMap::iterator itr = textures.find(texFile);
			if ( itr==textures.end() )
			{
				osg::ref_ptr<osg::Texture2D> tex2D = new osg::Texture2D;
				tex2D->setWrap( osg::Texture::WRAP_S, getWrapMode(wrapMode[0]) );
				tex2D->setWrap( osg::Texture::WRAP_T, getWrapMode(wrapMode[1]) );
				tex2D->setWrap( osg::Texture::WRAP_R, getWrapMode(wrapMode[2]) );
				tex2D->setFilter( osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR );
				tex2D->setFilter( osg::Texture::MAG_FILTER, osg::Texture::LINEAR );
				tex2D->setImage( osgDB::readImageFile(texFile, options) );
				textures[texFile] = tex2D;
			}
			
			ss->setTextureAttributeAndModes( unit, textures[texFile].get() );
			if ( unit>0 ) ss->setTextureAttributeAndModes( unit, new osg::TexEnv(getEnvMode(envOp)) );
		}
		
		// Create materials
		createMaterialData( ss, aiMtl );
        
        
        aiMatrix4x4 m = aiNode->mTransformation;
        m.Transpose();
        
        // Create the node and continue looking for children
        osg::ref_ptr<osg::MatrixTransform> mt;
        mt = new osg::MatrixTransform;
        mt->setMatrix( osg::Matrixf((float*)&m) );
        for ( unsigned int n=0; n<aiNode->mNumChildren; ++n )
        {
            osg::Node* child = traverseAIScene( filename, aiScene, aiNode->mChildren[n], textures, options );
            if ( child ) mt->addChild( child );
        }
        mt->addChild( geode );
        return mt.release();

		}        //*/
        		        
				
		
    }    




// ------------------------------------------------------------------------------------------------
// Intermediate data storage during conversion. Keeps everything and a bit more.
// ------------------------------------------------------------------------------------------------
class ConverterCarve2AssImp : public StatusCallback
{
    public:
		
    ConverterCarve2AssImp(const shared_ptr<ReaderSTEP>& step_reader, const shared_ptr<BuildingModel>& ifc_model, aiScene* out,const Settings& settings, 
		const shared_ptr<GeometrySettings>& geom_settings,std::map<int, osg::ref_ptr<osg::Switch> > & entity_id_to_node,std::map<int, osg::ref_ptr<osg::Switch> > & rep_id_to_node)
        : len_scale(1.0)
        , angle_scale(-1.0)
        , m_step_reader(step_reader)
        , m_ifc_model(ifc_model)
        , out(out)
        , settings(settings)
        // , apply_openings()
        // , collect_openings()
        , m_geom_settings(geom_settings)
		, entity_id_to_node(entity_id_to_node)
		, representation_id_to_node(rep_id_to_node)
    {}

    ~ConverterCarve2AssImp() {
        std::for_each(meshes.begin(),meshes.end(),IFC::delete_fun<aiMesh>());
        std::for_each(materials.begin(),materials.end(),IFC::delete_fun<aiMaterial>());
    }    

	aiNode* convertOSG2AssImpNode(osg::Node* node);
	aiNode* convertTransform(osg::Transform* node);
	aiMesh* convertDrawable(osg::Drawable* drawable);
	aiNode* convertGeode(osg::Geode* node);
	aiNode* visitNode(aiNode* aiParent,  uint64_t id, osg::Node* node,bool is_entity = true);
	aiNode* visitChildNode(aiNode* aiParent,  uint64_t id, osg::Node* node,bool is_entity = true);
	std::map< osg::Node*, aiNode* > m_node_map;
private:

    double len_scale, angle_scale;
    bool plane_angle_in_radians;

    shared_ptr<GeometrySettings>				m_geom_settings;
    shared_ptr<ReaderSTEP> m_step_reader;
    shared_ptr<BuildingModel> m_ifc_model;
    shared_ptr<GeometryConverter> m_geometry_converter;
	std::map<int, osg::ref_ptr<osg::Switch> > & entity_id_to_node;
	std::map<int, osg::ref_ptr<osg::Switch> > & representation_id_to_node;
    std::map<int, aiNode* >			m_map_entity_id_to_node;
	std::map<int, aiNode* >			m_map_representation_id_to_node;
    aiScene* out;

    // IFC::IfcMatrix4 wcs;
    std::vector<aiMesh*> meshes;
    std::vector<aiMaterial*> materials;
	std::vector<aiCamera*> cameras;
	std::vector<aiLight*> lights;
	std::vector<aiAnimation*> animations;
	std::vector<aiTexture*> textures;

    /*struct MeshCacheIndex {
        const IfcRepresentationItem* item; unsigned int matindex;
        MeshCacheIndex() : item(NULL), matindex(0) { }
        MeshCacheIndex(const IfcRepresentationItem* i, unsigned int mi) : item(i), matindex(mi) { }
        bool operator == (const MeshCacheIndex& o) const { return item == o.item && matindex == o.matindex; }
        bool operator < (const MeshCacheIndex& o) const { return item < o.item || (item == o.item && matindex < o.matindex); }
    };
    typedef std::map<MeshCacheIndex, std::set<unsigned int> > MeshCache;
    MeshCache cached_meshes;//*/

    // typedef std::map<const IfcSurfaceStyle*, unsigned int> MaterialCache;
    // MaterialCache cached_materials;

    const Settings& settings;

    // Intermediate arrays used to resolve openings in walls: only one of them
    // can be given at a time. apply_openings if present if the current element
    // is a wall and needs its openings to be poured into its geometry while
    // collect_openings is present only if the current element is an
    // IfcOpeningElement, for which all the geometry needs to be preserved
    // for later processing by a parent, which is a wall.
    // std::vector<IFC::TempOpening>* apply_openings;
    // std::vector<IFC::TempOpening>* collect_openings;

    std::set<uint64_t> already_processed;
};

#endif