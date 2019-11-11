#ifndef AI_IFCCONVERT2ASSIMPIMP_H_INC
#define AI_IFCCONVERT2ASSIMPIMP_H_INC

#include <assimp/code/Importer/IFC/IFCUtil.h>
#include <ifcpp/reader/ReaderSTEP.h>
#include "ifcpp/IFC4/include/IfcRepresentationItem.h"
#include "ifcpp/IFC4/include/IfcSurfaceStyle.h"
#include <ifcpp/geometry/Carve/GeometryConverter.h>
#include <ifcpp/geometry/GeometrySettings.h>
#include "IfcAssImpImporter.h"

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

using namespace std;
namespace Assimp    {


// ------------------------------------------------------------------------------------------------
// Intermediate data storage during conversion. Keeps everything and a bit more.
// ------------------------------------------------------------------------------------------------
class ConverterCarve2AssImp : public StatusCallback
{
    public:
    ConverterCarve2AssImp(const shared_ptr<ReaderSTEP>& step_reader, const shared_ptr<BuildingModel>& ifc_model, aiScene* out,const IfcAssImpImporter::Settings& settings, const shared_ptr<GeometrySettings>& geom_settings)
        : len_scale(1.0)
        , angle_scale(-1.0)
        , m_step_reader(step_reader)
        , m_ifc_model(ifc_model)
        , out(out)
        , settings(settings)
        , apply_openings()
        , collect_openings()
        , m_geom_settings(geom_settings)
    {}

    ~ConverterCarve2AssImp() {
        std::for_each(meshes.begin(),meshes.end(),IFC::delete_fun<aiMesh>());
        std::for_each(materials.begin(),materials.end(),IFC::delete_fun<aiMaterial>());
    }
    
    void convert(const std::map<int, shared_ptr<ProductShapeData> >& map_shape_data, std::vector<aiNode*>& nodes){
        progressTextCallback( L"Converting geometry to AssImp format ..." );
		progressValueCallback( 0, "AssImp" );
        // meshes.clear();
        // materials.clear();
        // cached_meshes.clear();
        // cached_materials.clear();

        shared_ptr<ProductShapeData> ifc_project_data;
		std::vector<shared_ptr<ProductShapeData> > vec_products;

        for( auto it = map_shape_data.begin(); it != map_shape_data.end(); ++it )
		{
			shared_ptr<ProductShapeData> shape_data = it->second;
			if( shape_data )
			{
				vec_products.push_back( shape_data );
			}
		}
/*
        const int num_products = (int)vec_products.size();
#ifdef ENABLE_OPENMP
		Mutex writelock_map;
		Mutex writelock_message_callback;
		Mutex writelock_ifc_project;

#pragma omp parallel firstprivate(num_products) shared(map_entity_id, map_representations)
		{
			// time for one product may vary significantly, so schedule not so many
#pragma omp for schedule(dynamic,40)
#endif
			for( int i = 0; i < num_products; ++i )
			{
				shared_ptr<ProductShapeData>& shape_data = vec_products[i];
				
				weak_ptr<IfcObjectDefinition>& ifc_object_def_weak = shape_data->m_ifc_object_definition;
				if( ifc_object_def_weak.expired() )
				{
					continue;
				}
				shared_ptr<IfcObjectDefinition> ifc_object_def( ifc_object_def_weak );
				
				std::stringstream thread_err;
				if( dynamic_pointer_cast<IfcFeatureElementSubtraction>(ifc_object_def) )
				{
					// geometry will be created in method subtractOpenings
					continue;
				}
				else if( dynamic_pointer_cast<IfcProject>(ifc_object_def) )
				{
#ifdef ENABLE_OPENMP
					ScopedLock scoped_lock( writelock_ifc_project );
#endif
					ifc_project_data = shape_data;
				}

				shared_ptr<IfcProduct> ifc_product = dynamic_pointer_cast<IfcProduct>(ifc_object_def);
				if( !ifc_product )
				{
					continue;
				}

				if( !ifc_product->m_Representation )
				{
					continue;
				}
				
				const int product_id = ifc_product->m_entity_id;
				// std::map<int, osg::ref_ptr<osg::Switch> > map_representation_switches;
				try
				{
					// convertProductShapeToOSG( shape_data, map_representation_switches );
				}
				catch( OutOfMemoryException& e )
				{
					throw e;
				}
				catch( BuildingException& e )
				{
					thread_err << e.what();
				}
				catch( carve::exception& e )
				{
					thread_err << e.str();
				}
				catch( std::exception& e )
				{
					thread_err << e.what();
				}
				catch( ... )
				{
					thread_err << "undefined error, product id " << product_id;
				}

				// if( map_representation_switches.size() > 0 )
				{
					// osg::ref_ptr<osg::Switch> product_switch = new osg::Switch();

					osg::ref_ptr<osg::MatrixTransform> product_transform = new osg::MatrixTransform();
					product_transform->setMatrix( convertMatrixToOSG( shape_data->getTransform() ) );
					product_switch->addChild( product_transform );

					std::stringstream strs_product_switch_name;
					strs_product_switch_name << "#" << product_id << "=" << ifc_product->className() << " group";
					product_switch->setName( strs_product_switch_name.str().c_str() );

					for( auto it_map = map_representation_switches.begin(); it_map != map_representation_switches.end(); ++it_map )
					{
						osg::ref_ptr<osg::Switch>& repres_switch = it_map->second;
						product_transform->addChild( repres_switch );
					}

					// apply statesets if there are any
					const std::vector<shared_ptr<AppearanceData> >& vec_product_appearances = shape_data->getAppearances();
					if( vec_product_appearances.size() > 0 )
					{
						applyAppearancesToGroup( vec_product_appearances, product_switch );
					}

#ifdef ENABLE_OPENMP
					ScopedLock scoped_lock( writelock_map );
#endif
					map_entity_id->insert( std::make_pair( product_id, product_switch ) );
					map_representations->insert( map_representation_switches.begin(), map_representation_switches.end() );
				}

				if( thread_err.tellp() > 0 )
				{
#ifdef ENABLE_OPENMP
					ScopedLock scoped_lock( writelock_message_callback );
#endif
					messageCallback( thread_err.str().c_str(), StatusCallback::MESSAGE_TYPE_ERROR, __FUNC__ );
				}

				// progress callback
				double progress = (double)i / (double)num_products;
				if( progress - m_recent_progress > 0.02 )
				{
#ifdef ENABLE_OPENMP
					if( omp_get_thread_num() == 0 )
#endif
					{
						// leave 10% of progress to openscenegraph internals
						progressValueCallback( progress*0.9, "scenegraph" );
						m_recent_progress = progress;
					}
				}
			}
#ifdef ENABLE_OPENMP
		} // implicit barrier
#endif
//*/
    };


private:

    IFC::IfcFloat len_scale, angle_scale;
    bool plane_angle_in_radians;

    shared_ptr<GeometrySettings>				m_geom_settings;
    shared_ptr<ReaderSTEP> m_step_reader;
    shared_ptr<BuildingModel> m_ifc_model;
    shared_ptr<GeometryConverter> m_geometry_converter;
    aiScene* out;

    IFC::IfcMatrix4 wcs;
    std::vector<aiMesh*> meshes;
    std::vector<aiMaterial*> materials;

    struct MeshCacheIndex {
        const IfcRepresentationItem* item; unsigned int matindex;
        MeshCacheIndex() : item(NULL), matindex(0) { }
        MeshCacheIndex(const IfcRepresentationItem* i, unsigned int mi) : item(i), matindex(mi) { }
        bool operator == (const MeshCacheIndex& o) const { return item == o.item && matindex == o.matindex; }
        bool operator < (const MeshCacheIndex& o) const { return item < o.item || (item == o.item && matindex < o.matindex); }
    };
    typedef std::map<MeshCacheIndex, std::set<unsigned int> > MeshCache;
    MeshCache cached_meshes;

    typedef std::map<const IfcSurfaceStyle*, unsigned int> MaterialCache;
    MaterialCache cached_materials;

    const IfcAssImpImporter::Settings& settings;

    // Intermediate arrays used to resolve openings in walls: only one of them
    // can be given at a time. apply_openings if present if the current element
    // is a wall and needs its openings to be poured into its geometry while
    // collect_openings is present only if the current element is an
    // IfcOpeningElement, for which all the geometry needs to be preserved
    // for later processing by a parent, which is a wall.
    std::vector<IFC::TempOpening>* apply_openings;
    std::vector<IFC::TempOpening>* collect_openings;

    std::set<uint64_t> already_processed;
};

};
#endif