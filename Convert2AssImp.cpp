#ifndef AI_IFCCONVERT2ASSIMPIMP_CPP_INC
#define AI_IFCCONVERT2ASSIMPIMP_CPP_INC

#include <osg/CullFace>
#include <osg/Geode>
#include <osg/Hint>
#include <osg/LineWidth>
#include <osg/Material>
#include <osg/Point>
#include <osg/Geometry>
#include <ifcpp/geometry/Carve/IncludeCarveHeaders.h>
// #include <osgUtil/Tessellator>

// #include <assimp/code/Importer/IFC/IFCUtil.h>
#include <assimp/scene.h>
#include <ifcpp/reader/ReaderSTEP.h>
#include "ifcpp/IFC4/include/IfcRepresentationItem.h"
#include "ifcpp/IFC4/include/IfcSurfaceStyle.h"
#include <ifcpp/geometry/Carve/GeometryConverter.h>
#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/geometry/SceneGraphUtils.h>
#include "Convert2AssImp.h"

using namespace std;
using namespace Assimp;

inline void printNode(osg::Node* node){
	printf("Node name: %s\n",node->getName().c_str());
	printf("Node class name: %s\n",node->className());	
}

inline void convert(osg::Geometry* geom,aiMesh& mesh){
	auto* va = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
	auto* na = dynamic_cast<osg::Vec3Array*>(geom->getNormalArray());
	auto* ca = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());
	// printf("va = %d\n",va->size());
	// if (na) printf("na = %d\n",na->size());
	// if (ca) printf("ca = %d\n",ca->size());
		
	mesh.mNumVertices = va->getNumElements();
	// printf("mesh.mNumVertices = %d\n",mesh.mNumVertices);
	mesh.mVertices = new aiVector3D[mesh.mNumVertices];
	if (na) mesh.mNormals = new aiVector3D[mesh.mNumVertices];
	// mesh.mColors = new (aiColor4D*)[AI_MAX_NUMBER_OF_COLOR_SETS];	
	if (ca) mesh.mColors[0] = new aiColor4D[mesh.mNumVertices];//AI_MAX_NUMBER_OF_COLOR_SETS	
	for (size_t i=0; i<mesh.mNumVertices; ++i )
	{
		auto& v = (*va)[i];				
		mesh.mVertices[i] = {v[0],v[1],v[2]};
		
		if (na){
			auto& n = (*na)[i];
			mesh.mNormals[i] = {n[0],n[1],n[2]};
		}
		
		if (ca){
			auto& c = (*ca)[i];		
			mesh.mColors[0][i] = {c[0],c[1],c[2],c[3]};
		}
		
	}
	
	//Now set up faces
	size_t numPrimtiveSets = geom->getNumPrimitiveSets();
	if (numPrimtiveSets==1) {//Almost all the time
		auto* primSet = geom->getPrimitiveSet(0);
		auto type = primSet->getType();
		// printf("Primitive type = %d   (%d) \n",type,osg::PrimitiveSet::Type::DrawArraysPrimitiveType);
		if (type == osg::PrimitiveSet::Type::DrawArraysPrimitiveType){
			auto mode = primSet->getMode();			
			switch (mode)
			{
			case osg::PrimitiveSet::Mode::POINTS:
				/* code */
				mesh.mPrimitiveTypes =  aiPrimitiveType_POINT; 
				break;
			
			case osg::PrimitiveSet::Mode::LINES:
			case osg::PrimitiveSet::Mode::LINE_LOOP:
			case osg::PrimitiveSet::Mode::LINE_STRIP:
			case osg::PrimitiveSet::Mode::LINE_STRIP_ADJACENCY:
			case osg::PrimitiveSet::Mode::LINES_ADJACENCY:
				mesh.mPrimitiveTypes =  aiPrimitiveType_LINE; 
				break;
			case osg::PrimitiveSet::Mode::TRIANGLES:
			case osg::PrimitiveSet::Mode::TRIANGLE_STRIP:
			case osg::PrimitiveSet::Mode::TRIANGLE_STRIP_ADJACENCY:
			case osg::PrimitiveSet::Mode::TRIANGLE_FAN:
			case osg::PrimitiveSet::Mode::TRIANGLES_ADJACENCY:			
				mesh.mPrimitiveTypes =  aiPrimitiveType_TRIANGLE; 
				break;

			case osg::PrimitiveSet::Mode::POLYGON:
				mesh.mPrimitiveTypes =  aiPrimitiveType_POLYGON; 
				break;

			default:
				mesh.mPrimitiveTypes =  aiPrimitiveType_POLYGON; 
				break;
			}
			printf("Primitive mode = %d  --> %d,   (%d)    (%d)\n",mode,osg::PrimitiveSet::TRIANGLES,mesh.mPrimitiveTypes,aiPrimitiveType_TRIANGLE);
			mesh.mNumFaces = primSet->getNumIndices();
			mesh.mFaces = new aiFace[mesh.mNumFaces];
			size_t pos = 0;
			for (size_t i =0;i<mesh.mNumFaces;++i){
				auto& face = mesh.mFaces[i];
				face.mNumIndices = (mesh.mPrimitiveTypes == aiPrimitiveType_POINT) ? 1 : (mesh.mPrimitiveTypes == aiPrimitiveType_LINE) ? 2 
					: (mesh.mPrimitiveTypes == aiPrimitiveType_TRIANGLE) ? 3 : (mode==osg::PrimitiveSet::Mode::QUADS || mode==osg::PrimitiveSet::Mode::QUAD_STRIP) 
					? 4 : 4 ;
				if (face.mNumIndices>3){
					if (mode==osg::PrimitiveSet::Mode::POLYGON)
						face.mNumIndices = (i==mesh.mNumFaces-1) ? mesh.mNumVertices-pos  : 3;
				}
				face.mIndices = new unsigned int[face.mNumIndices];
				for (size_t j=0;j<face.mNumIndices;++j)
					face.mIndices[j] = primSet->index(pos++);
			}
					
		}
	}
	
	// Create geometry texture coordinates
	size_t numTextCoordsArrays = geom->getNumTexCoordArrays();
	unsigned int unit = 0;
	// auto* texCoord = dynamic_cast<osg::Vec2Array*>(geom->getTexCoordArray(unit));
	// const aiVector3D* aiTexCoords = mesh.mTextureCoords[unit];            
	// mesh.mTextureCoords = new aiVector3D[AI_MAX_NUMBER_OF_TEXTURECOORDS];
	// printf("numTextCoordsArrays = %d\n",numTextCoordsArrays);
	for (size_t unit =0;unit<numTextCoordsArrays;++unit){
		auto* texCoord = geom->getTexCoordArray(unit);
		if (!texCoord) continue;
		size_t numUVComponent = (size_t)texCoord->getElementSize()/sizeof(ai_real);
		printf("numUVComponent = %d\n",numUVComponent);
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
}

class InfoVisitor : public osg::NodeVisitor
{
public:
    InfoVisitor(std::map<int, aiNode* >& map_entity_id_to_node,std::map<int, aiNode* >& map_representation_id_to_node) 
	: m_map_entity_id_to_node(map_entity_id_to_node),m_map_representation_id_to_node(map_representation_id_to_node), _level(0)
    {
        setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
    }
    std::string spaces()
    {
        return std::string(_level * 2, ' ');
    }
    virtual void apply(osg::Node &node);
    // virtual void apply(osg::Geode &geode);
	virtual void apply(osg::Transform &transform);
	virtual void apply(osg::Geometry &geom);

	inline size_t numMeshes(){return meshes.size();};
	inline std::vector<aiMesh*>& getMeshes(){
		return meshes;
	}

	inline std::vector<aiNode*>& getNodes(){
		return nodes;
	}
protected:
    unsigned int _level;
	std::map<int, aiNode* >&			m_map_entity_id_to_node;
	std::map<int, aiNode* >&			m_map_representation_id_to_node;
	std::vector<aiMesh*> meshes;
	std::vector<aiNode*> nodes;
};

inline void convertMat(osg::Matrix& m, aiMatrix4x4& o){
	// std::copy(m.ptr(),m.ptr()+16,o);	
	auto * arr = m.ptr();
	o.a1 = arr[0];
	o.a2 = arr[1];
	o.a3 = arr[2];
	o.a4 = arr[3];	
	o.b1 = arr[4];
	o.b2 = arr[5];
	o.b3 = arr[6];
	o.b4 = arr[7];
	o.c1 = arr[8];
	o.c2 = arr[9];
	o.c3 = arr[10];
	o.c4 = arr[11];	
	o.d1 = arr[12];
	o.d2 = arr[13];
	o.d3 = arr[14];
	o.d4 = arr[15];	
}

void InfoVisitor::apply(osg::Transform &transform)
{
    std::cout << spaces() << transform.libraryName() << "-->"
              << transform.className() <<" (" << transform.getName()<<")"<< std::endl;

	auto* mt = transform.asMatrixTransform();
	if (mt){
		osg::Matrix m_transformMat = mt->getMatrix();
		// m_transformMat.transpose();
		aiNode* nd = new aiNode;
		convertMat(m_transformMat,nd->mTransformation);
		nd->mTransformation.Transpose();
		nodes.push_back(nd);
	}
	
    _level++;
    traverse(transform);
    _level--;
}//*/


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void InfoVisitor::apply(osg::Node &node)
{
    std::cout << spaces() << node.libraryName() << ":::"
              << node.className() <<" (" << node.getName()<<")"<< std::endl;
    _level++;
    traverse(node);
    _level--;
}//*/
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
/*void InfoVisitor::apply(osg::Geode &geode)
{
    std::cout << spaces() << geode.libraryName() << "::"
              << geode.className() << " (" << geode.getName()<<")" << std::endl;
    _level++;
	meshes.reserve(meshes.size()+geode.getNumDrawables());
    for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
    {
        osg::Drawable *drawable = geode.getDrawable(i);
        std::cout << spaces() << drawable->libraryName() << "::"
                  << drawable->className() << " (" << drawable->getName()<<")"<< std::endl;
		osg::Geometry* geom = drawable->asGeometry();		
		aiMesh* m = new aiMesh;
		meshes.push_back(m);		
		convert(geom,*m);
    }
    traverse(geode);
    _level--;
}//*/
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void InfoVisitor::apply(osg::Geometry &geom)
{
    std::cout << spaces() << geom.libraryName() << "::"
              << geom.className() << " (" << geom.getName()<<")" << std::endl;
    _level++;	
	aiMesh* m = new aiMesh;
	meshes.push_back(m);		
	convert(&geom,*m);    
    traverse(geom);
    _level--;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

/*template <typename T>
inline void toaiMatrix4x4(aiMatrix4x4& out_of_range,const T* in){
	out.a1 = in[0];
    out.b1 = in[1];
    out.c1 = in[2];

    out.a2 = in[3];
    out.b2 = in[4];
    out.c2 = in[5];

    out.a3 = in[6];
    out.b3 = in[7];
    out.c3 = in[8];
}

/*inline void toaiMatrix4x4(aiMatrix4x4& m,const double* in){
	std::copy(in,in+sizeof(aiMatrix4x4),&m);
}//*/

aiNode* ConverterCarve2AssImp::visitNode(aiNode* aiParent, uint64_t id, osg::Node* node,bool is_entity){
	// InfoVisitor infoVisitor(m_map_entity_id_to_node, m_map_representation_id_to_node);
	// node->accept(infoVisitor);
	// printf("infoVisitor.numMeshes() = %d\n",infoVisitor.numMeshes());
	// printNode(node);
	if (node->className() == "Switch"){
		osg::Switch* sw = node->asSwitch();
		std::unique_ptr<aiNode> groupNode(new aiNode(node->getName())) ;
		groupNode->mParent = aiParent;				
		auto numChildren = sw->getNumChildren();
		groupNode->mNumChildren = numChildren;		
		groupNode->mChildren = new aiNode*[numChildren];		
		aiMetadata* metaData = new aiMetadata();
		metaData->Alloc(2);		
		metaData->Add("id",id);
		metaData->Add("is_entity",is_entity);
		groupNode->mMetaData = metaData;
		// for (size_t i =0;i<groupNode->mNumChildren;++i){
		// 	aiNode* nd = visitNode(groupNode.get(),sw->getChild(i) );
		// 	groupNode->mChildren[i] = nd;
		// 	aiMetadata* metaData = new aiMetadata();
		// 	metaData->Alloc(2);
		// 	metaData->Add("visibility",sw->getValue(i));
		// 	nd->mMetaData = metaData;
		// 	nd->mParent = groupNode.get();
		// }
		return groupNode.release();
	}
	/*osg::Node* ptr = NULL;
	if (node->className() == "Group"){
		std::unique_ptr<aiNode> groupNode(new aiNode(node->getName())) ;
		groupNode->mParent = aiParent;		
		osg::Group* group = node->asGroup();
		auto numChildren = group->getNumChildren();
		groupNode->mNumChildren = numChildren;
		groupNode->mChildren = new aiNode*[numChildren];
		for (size_t i =0;i<groupNode->mNumChildren;++i){
			aiNode* nd = visitNode(groupNode.get(),group->getChild(i) );
			groupNode->mChildren[i] = nd;
		}
		return groupNode.release();
	} else if ( node->asTransform() != NULL ){
		osg::Transform* osgTrans = node->asTransform();
		if ( ptr=osgTrans->asMatrixTransform() ){
			osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(ptr);
			aiNode* tNode = new aiNode(node->getName());		
			tNode->mParent = aiParent;		
			auto& matrix = mt->getMatrix();
			aiMatrix4x4& m = tNode->mTransformation;
			toaiMatrix4x4 (m,matrix.ptr());
			auto numChildren = mt->getNumChildren();
			tNode->mNumChildren = numChildren;
			tNode->mChildren = new aiNode*[numChildren];
			for (size_t i =0;i<tNode->mNumChildren;++i){
				aiNode* nd = visitNode(tNode,mt->getChild(i) );
				tNode->mChildren[i] = nd;
			}
			return tNode;
		}
		
		
	} else if ( (ptr = node->asTransform()) != NULL ){
		
	}//*/

	return NULL;
}

aiNode* ConverterCarve2AssImp::convertTransform(osg::Transform* node){	
	if ( node && node->asMatrixTransform() ){
		// printf("Convert matrix transform\n");
		osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(node->asMatrixTransform());		
		aiNode* tNode = new aiNode(node->getName());				
		auto matrix = mt->getMatrix();
		if (!mt) return tNode;
		aiMatrix4x4& m = tNode->mTransformation;
		convertMat(matrix,m);
		// printf("FINISH convertMat\n");
		// toaiMatrix4x4 (m,matrix.ptr());
		auto numChildren = mt->getNumChildren();
		tNode->mNumChildren = numChildren;
		tNode->mChildren = new aiNode*[numChildren];
		for (size_t i =0;i<tNode->mNumChildren;++i){
			aiNode* nd = visitChildNode(tNode, -1  ,mt->getChild(i) );
			if (!nd) continue;
			tNode->mChildren[i] = nd;
			nd->mParent = tNode;			
		}
		// printf("Exit MATRIX Transform\n");
		return tNode;
	}
	printf("Not a MATRIX Transform\n");
	return NULL;
}

aiMesh* ConverterCarve2AssImp::convertDrawable(osg::Drawable* drawable){
	
	if (drawable->asGeometry()){		
		osg::Geometry* geom = drawable->asGeometry();		
		aiMesh* mesh = new aiMesh;
		convert(geom,*mesh);		
		return mesh;
	} else printf("Something else\n");
	return NULL;
}

aiNode* ConverterCarve2AssImp::convertGeode(osg::Geode* geode){	
	if ( geode  ){
		// printf("Convert geode transform\n");		
		aiNode* tNode = new aiNode(geode->getName());				
		auto numChildren = geode->getNumDrawables();		
		tNode->mNumMeshes = numChildren;
		tNode->mMeshes = new unsigned int[numChildren];
		tNode->mNumChildren = 0;		
		for (size_t i =0;i<tNode->mNumMeshes;++i){
			auto drawable = geode->getDrawable(i);
			aiMesh* nd = convertDrawable(drawable);
			if (!nd) continue;
			meshes.push_back(nd);
			tNode->mMeshes[i] = meshes.size()-1;			
			// nd->mParent = tNode;			
		}
		// printf("Exit MATRIX Transform\n");
		return tNode;
	}
	printf("Not a MATRIX Transform\n");
	return NULL;
}

aiNode* ConverterCarve2AssImp::visitChildNode(aiNode* aiParent, uint64_t id, osg::Node* node,bool is_entity){
	// printf("Enter visit child node\n");
	// printNode(node);
	if (!node) {
		printf("It's NULL\n");
		return NULL;
	}
	// printf("Unlikely\n");
	aiNode* groupNode = m_node_map[node];	
	// printf("OK Unlikely  %d   %d \n",node,groupNode);	
	if (node && node->asSwitch()){
		// printf("SWITCH\n");
		osg::Switch* sw = node->asSwitch();						
		// printf("Searching %d   --  FOUND %d\n",node,groupNode);
		if (!groupNode || !sw) {
			printf("Not found Switch\n");
			return NULL;
		} else {
			groupNode = new aiNode(node->getName());
			groupNode->mNumChildren = sw->getNumChildren();
			groupNode->mChildren = new aiNode*[groupNode->mNumChildren];
		}
		auto* metaData = groupNode->mMetaData;		
		uint64_t iid = -1;
		if (metaData)  
			metaData->Get("id",iid);
		
		for (size_t i =0;i<groupNode->mNumChildren;++i){	
			osg::Node* m_node = sw->getChild(i);
			if (!m_node) continue;
			aiNode* nd = m_node_map[m_node];
			uint64_t id = -1;
			aiMetadata* metaData = NULL;
			if (!nd) {
				// printf("Explore node %d\n",m_node);				
				nd = visitChildNode(NULL,id,m_node,false);
				if (!nd) continue;
				metaData = new aiMetadata();
				metaData->Alloc(2);				
				nd->mMetaData = metaData;
			} else {
				printf("Found old node");
				metaData = nd->mMetaData;		
				if (metaData) metaData->Get("id",iid);
			}
			// printf("Adding visibility %d\n",metaData);
			if (metaData) metaData->Add("visibility",sw->getValue(i));				
			groupNode->mChildren[i] = nd;
			if (nd) nd->mParent = groupNode;
			// printf("End of loop\n");
		}

	} else if (node->asTransform()) {
		// printf("Fuck\n");
		auto* mt = node->asTransform();
		auto* tf = convertTransform( mt );
		if (tf) {
			tf->mParent = aiParent;
			if (aiParent) {
				printf("Adding 01 children\n");
				aiParent->addChildren(1,&tf);
				printf("Done Adding 01 children\n");
			}
		}
		return tf;
	} else if (node->asGeode()) {
		// printf("Fuck\n");
		auto* geode = node->asGeode();
		return convertGeode( geode );		
	} else {
		// auto* geom = node->asGeometry();
		printf("*************\n");
		printNode(node);
	}

	// printf("Out of visitChildNode\n");
	// if (aiParent && groupNode) {
	// 	printf("Adding 1 children\n");
	// 	aiParent->addChildren(1,&groupNode);
	// 	printf("Done Adding 1 children\n");
	// }
	// printf("Return of visitChildNode\n");
	return groupNode;
	// return NULL;
}


aiNode* ConverterCarve2AssImp::convertOSG2AssImpNode(osg::Node* node){
	// std::map<int, osg::ref_ptr<osg::Switch> > & entity_id_to_node;
	// std::map<int, osg::ref_ptr<osg::Switch> > & representation_id_to_node;	
	m_node_map.clear();
	for (auto it = entity_id_to_node.begin(); it!=entity_id_to_node.end();++it){
		auto id = it->first;
		osg::ref_ptr<osg::Switch>& node = it->second;
		aiNode* dest = visitNode(NULL,id,node,true);
		if (!dest) printf("dest is NULL1\n");
		m_map_entity_id_to_node[id] = dest;
		m_node_map[ node.get() ] = dest;		
	}

	for (auto it = representation_id_to_node.begin(); it!=representation_id_to_node.end();++it){
		auto id = it->first;
		osg::ref_ptr<osg::Switch>& node = it->second;
		aiNode* dest = visitNode(NULL,id,node,false);
		if (!dest) printf("dest is NULL2\n");
		m_map_representation_id_to_node[id] = dest;
		m_node_map[ node.get() ] = dest;		
	}

	printf("Number of element is %d\n",m_node_map.size());

	for (auto it = entity_id_to_node.begin(); it!=entity_id_to_node.end();++it){
		auto id = it->first;
		osg::ref_ptr<osg::Switch>& node = it->second;
		aiNode* dest = visitChildNode(NULL,id,node.get(),true);		
		if (!dest) printf("dest is NULL3\n");
	}

	for (auto it = representation_id_to_node.begin(); it!=representation_id_to_node.end();++it){
		auto id = it->first;
		osg::ref_ptr<osg::Switch>& node = it->second;
		aiNode* dest = visitChildNode(NULL,id,node.get(),false);		
		if (!dest) printf("dest is NULL4\n");
	}
	printf("Found %d meshes\n", meshes.size());
	printf("DONE\n");
	return NULL;
}
#endif
