#ifndef _gkg_simulation_virtual_tissue_AtomMethodFactory_h_
#define _gkg_simulation_virtual_tissue_AtomMethodFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-simulation-virtual-tissue/Atom.h>
#include <vector>


namespace gkg
{


class AtomMethodFactory : public Singleton< AtomMethodFactory >
{

  public:


    typedef void ( *GetAtomLutVoxelMethod )( const float*,
                                             bool,
                                             float,
                                             float,
                                             float,
                                             float,
                                             float,
                                             float,
                                             int32_t,
                                             int32_t,
                                             int32_t,
                                             int32_t&,
                                             int32_t&,
                                             int32_t& );
    typedef void ( *GetAtomLutVoxelSpanMethod )( const float*,
                                                 bool,
                                                 float,
                                                 float,
                                                 float,
                                                 float,
                                                 float,
                                                 float,
                                                 int32_t,
                                                 int32_t,
                                                 int32_t,
                                                 int32_t&,
                                                 int32_t&,
                                                 int32_t&,
                                                 int32_t&,
                                                 int32_t&,
                                                 int32_t& );
    typedef bool ( *BelongToMethod )( const float*,
                                      bool,
                                      const float&,
                                      const float&,
                                      const float& );

    typedef float ( *DistanceToMembraneMethod )( const float*,
                                                 bool,
                                                 const float&,
                                                 const float&,
                                                 const float& );

    typedef void ( *OutwardNormalToMembraneMethod )( const float*,
                                                     bool,
                                                     const float&,
                                                     const float&,
                                                     const float&,
                                                     float&,
                                                     float&,
                                                     float& );

    typedef bool ( *IsRayCollidingWithAtomMethod )( const float*,
                                                    bool,
                                                    const float&,
                                                    const float&,
                                                    const float&,
                                                    const float&,
                                                    const float&,
                                                    const float&,
                                                    bool,
                                                    float&,
                                                    float&,
                                                    float&,
                                                    float& );

    ~AtomMethodFactory();

    // get atom lut voxel method ///////////////////////////////////////////////
    bool registerGetAtomLutVoxelMethod(
           uint8_t atomType,
           GetAtomLutVoxelMethod getAtomLutVoxelMethod );

    GetAtomLutVoxelMethod getGetAtomLutVoxelMethod( uint8_t atomType ) const;

    // get atom lut voxel span method //////////////////////////////////////////
    bool registerGetAtomLutVoxelSpanMethod(
           uint8_t atomType,
           GetAtomLutVoxelSpanMethod getAtomLutVoxelSpanMethod );

    GetAtomLutVoxelSpanMethod
      getGetAtomLutVoxelSpanMethod( uint8_t atomType ) const;

    // belong to method ////////////////////////////////////////////////////////
    bool registerBelongToMethod(
           uint8_t atomType,
           BelongToMethod belongToMethod );

    BelongToMethod getBelongToMethod( uint8_t atomType ) const;

    // distance to membrane method /////////////////////////////////////////////
    bool registerDistanceToMembraneMethod(
           uint8_t atomType,
           DistanceToMembraneMethod distanceToMembraneMethod );

    DistanceToMembraneMethod getDistanceToMembraneMethod(
                                                       uint8_t atomType ) const;

    // outward normal to membrane method ///////////////////////////////////////
    bool registerOutwardNormalToMembraneMethod(
           uint8_t atomType,
           OutwardNormalToMembraneMethod outwardNormalToMembraneMethod );

    OutwardNormalToMembraneMethod getOutwardNormalToMembraneMethod(
                                                       uint8_t atomType ) const;

    // is ray colliding with atom method ///////////////////////////////////////
    bool registerIsRayCollidingWithAtomMethod(
           uint8_t atomType,
           IsRayCollidingWithAtomMethod isRayCollidingWithAtomMethod );

    IsRayCollidingWithAtomMethod getIsRayCollidingWithAtomMethod(
                                                       uint8_t atomType ) const;

  protected:

    friend class Singleton< AtomMethodFactory >;

    AtomMethodFactory();

    std::vector< GetAtomLutVoxelMethod > _getAtomLutVoxelMethods; 
    std::vector< GetAtomLutVoxelSpanMethod > _getAtomLutVoxelSpanMethods; 
    std::vector< BelongToMethod > _belongToMethods; 
    std::vector< DistanceToMembraneMethod > _distanceToMembraneMethods; 
    std::vector< OutwardNormalToMembraneMethod > 
                                                _outwardNormalToMembraneMethods;
    std::vector< IsRayCollidingWithAtomMethod > _isRayCollidingWithAtomMethods;


};


}


#endif

