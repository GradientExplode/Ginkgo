#ifndef _gkg_simulation_virtual_tissue_SphereAtom_h_
#define _gkg_simulation_virtual_tissue_SphereAtom_h_


#include <gkg-simulation-virtual-tissue/Atom.h>


namespace gkg
{

// type = 1U
// parameters[ 0 ] -> center.x
// parameters[ 1 ] -> center.y
// parameters[ 2 ] -> center.z
// parameters[ 3 ] -> radius

struct SphereAtom : public Atom
{

  public:

    SphereAtom( const Vector3d< float >& center, float radius );
    SphereAtom();
    SphereAtom( const SphereAtom& other );
    virtual ~SphereAtom();

    SphereAtom& operator=( const SphereAtom& other );

    Atom* clone() const;

    uint8_t getType() const;
    void setCenter( const Vector3d< float >& center );
    void setCenter( float centerX,
                    float centerY,
                    float centerZ );
    Vector3d< float > getCenter() const;
    float getSpanRadius() const;
    void setRadius( float radius );
    float getRadius() const;
    float getVolume() const;
    bool contains( const Vector3d< float >& position ) const; 
    

    void getTranslatedParameters( float* outputParameters,
                                  float translationX,
                                  float translationY,
                                  float translationZ ) const;

    static uint8_t getStaticType();
    static void getAtomLutVoxel( const float* inputParameters,
                                 bool compressedRepresentation,
                                 float lowerX,
                                 float lowerY,
                                 float lowerZ,
                                 float spanX,
                                 float spanY,
                                 float spanZ,
                                 int32_t lutSizeX,
                                 int32_t lutSizeY,
                                 int32_t lutSizeZ,
                                 int32_t& lutVoxelX,
                                 int32_t& lutVoxelY,
                                 int32_t& lutVoxelZ );
    static void getAtomLutVoxelSpan( const float* inputParameters,
                                     bool compressedRepresentation,
                                     float lowerX,
                                     float lowerY,
                                     float lowerZ,
                                     float gridResolutionInUmX,
                                     float gridResolutionInUmY,
                                     float gridResolutionInUmZ,
                                     int32_t lutSizeX,
                                     int32_t lutSizeY,
                                     int32_t lutSizeZ,
                                     int32_t& infLutVoxelX,
                                     int32_t& supLutVoxelX,
                                     int32_t& infLutVoxelY,
                                     int32_t& supLutVoxelY,
                                     int32_t& infLutVoxelZ,
                                     int32_t& supLutVoxelZ );
    static bool belongTo( const float* atomParameters,
                          bool compressedRepresentation,
                          const float& coordinateX,
                          const float& coordinateY,
                          const float& coordinateZ );
    static float distanceToMembrane( const float* atomParameters,
                                     bool compressedRepresentation,
                                     const float& coordinateX,
                                     const float& coordinateY,
                                     const float& coordinateZ );
    static void getOutwardNormalAt( const float* atomParameters,
                                    bool compressedRepresentation,
                                    const float& coordinateX,
                                    const float& coordinateY,
                                    const float& coordinateZ,
                                    float& normalX,
                                    float& normalY,
                                    float& normalZ );
    static bool isRayCollidingWithAtom( const float* atomParameters,
                                        bool compressedRepresentation,
                                        const float& coordinateX,
                                        const float& coordinateY,
                                        const float& coordinateZ,
                                        const float& stepX,
                                        const float& stepY,
                                        const float& stepZ,
                                        bool insideAtom,
                                        float& intersectionX,
                                        float& intersectionY,
                                        float& intersectionZ,
                                        float& collisionDistance );

};


}


#endif

