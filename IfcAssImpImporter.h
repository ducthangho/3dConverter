
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

/** @file  AssbinLoader.h
 *  @brief .assbin File format loader
 */
#ifndef AI_IFCASSIMPIMPORTER_H_INC
#define AI_IFCASSIMPIMPORTER_H_INC

#include <assimp/BaseImporter.h>
#include <assimp/code/Importer/IFC/IFCUtil.h>
#include <ifcpp/reader/ReaderSTEP.h>
#include "ifcpp/IFC4/include/IfcRepresentationItem.h"
#include "ifcpp/IFC4/include/IfcSurfaceStyle.h"
#include <ifcpp/geometry/Carve/GeometryConverter.h>
#include <ifcpp/geometry/GeometrySettings.h>

struct aiMesh;
struct aiNode;
struct aiBone;
struct aiMaterial;
struct aiMaterialProperty;
struct aiNodeAnim;
struct aiAnimation;
struct aiTexture;
struct aiLight;
struct aiCamera;

#ifndef ASSIMP_BUILD_NO_IFCASSIMPIMPORTER

using namespace std;
namespace Assimp    {

// ---------------------------------------------------------------------------------
/** Importer class for 3D Studio r3 and r4 3DS files
 */
class IfcAssImpImporter : public BaseImporter
{
private:
    bool shortened;
    bool compressed;

public:

    IfcAssImpImporter():m_step_reader(new ReaderSTEP()), m_ifc_model(new BuildingModel()),geometry_converter(new GeometryConverter( m_ifc_model )) {

    }

    virtual bool CanRead(
        const std::string& pFile,
        IOSystem* pIOHandler,
        bool checkSig
    ) const;
    virtual const aiImporterDesc* GetInfo() const;
    virtual void InternReadFile(
    const std::string& pFile,
        aiScene* pScene,
        IOSystem* pIOHandler
    );
    void ReadHeader();
    void ReadBinaryScene( IOStream * stream, aiScene* pScene );
    void ReadBinaryNode( IOStream * stream, aiNode** mRootNode, aiNode* parent );
    void ReadBinaryMesh( IOStream * stream, aiMesh* mesh );
    void ReadBinaryBone( IOStream * stream, aiBone* bone );
    void ReadBinaryMaterial(IOStream * stream, aiMaterial* mat);
    void ReadBinaryMaterialProperty(IOStream * stream, aiMaterialProperty* prop);
    void ReadBinaryNodeAnim(IOStream * stream, aiNodeAnim* nd);
    void ReadBinaryAnim( IOStream * stream, aiAnimation* anim );
    void ReadBinaryTexture(IOStream * stream, aiTexture* tex);
    void ReadBinaryLight( IOStream * stream, aiLight* l );
    void ReadBinaryCamera( IOStream * stream, aiCamera* cam );
    shared_ptr<GeometryConverter>	getGeometryConverter()	{ return geometry_converter; }

protected:

    // --------------------
    // const aiImporterDesc* GetInfo () const;

    // --------------------
    void SetupProperties(const Importer* pImp);

    // --------------------
    // void InternReadFile( const std::string& pFile,
    //     aiScene* pScene,
    //     IOSystem* pIOHandler
    // );

private:

    shared_ptr<BuildingModel> m_ifc_model;
	shared_ptr<ReaderSTEP> m_step_reader;
    shared_ptr<GeometryConverter> geometry_converter;
public:


    // loader settings, publicly accessible via their corresponding AI_CONFIG constants
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


private:

    Settings settings;

};



} // end of namespace Assimp

#endif // !! ASSIMP_BUILD_NO_IFCASSIMPIMPORTER

#endif // AI_IFCASSIMPIMPORTER_H_INC
