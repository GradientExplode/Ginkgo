#ifndef _gkg_simulation_virtual_tissue_EllipsoidAtom_h_
#define _gkg_simulation_virtual_tissue_EllipsoidAtom_h_


#include <gkg-simulation-virtual-tissue/Atom.h>
#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{



// type = 2U
//
// general equation of an ellipsoid of center (cx, cy, cz), of radii (r1, r2, r3) and 
// of axis (e1, e2, e3):
//
// translation to center:
//
//     [1 0  0 -cx ]
//     [0 1  0 -cy ]
// T = [0 0  1 -cz ]
//     [0 0  0  1  ]
//
// rotation matrix:
//
//     [e1x e2x e3x 0]
//     [e1y e2y e3y 0]
// R = [e1z e2z e3z 0]
//     [ 0   0   0  1]
//
// scaling matrix (the final -1 is related to the =1 of the equation):
//
//     [ 1/r1^2  0       0     0 ]
//     [ 0      1/r1^2   0     0 ]
// S = [ 0       0     1/r1^2  0 ]
//     [ 0       0       0    -1 ]
//
// resulting in:
//
// T x R x S x R^T x T^T = [ a00 a01 a02 a03]
//                         [  .  a11 a12 a13]
//                         [  .   .  a22 a23]
//                         [  .   .   .  a33]
//
// we store compressed parameters as follows:
//
// parameters[  0 ] -> a00
// parameters[  1 ] -> a11
// parameters[  2 ] -> a22
// parameters[  3 ] -> 2.0 * a01
// parameters[  4 ] -> 2.0 * a12
// parameters[  5 ] -> 2.0 * a02
// parameters[  6 ] -> 2.0 * a03
// parameters[  7 ] -> 2.0 * a13
// parameters[  8 ] -> 2.0 * a23
// parameters[  9 ] -> a33
//

struct EllipsoidAtom : public Atom
{

  public:


    EllipsoidAtom( const Vector3d< float >& center,
                   float radius1,
                   float radius2,
                   float radius3,
                   const Vector3d< float >& axis1,
                   const Vector3d< float >& axis2,
                   const Vector3d< float >& axis3,
                   bool compressedRepresentation = true );
    EllipsoidAtom( const std::vector< float >& vectorRepresentation,
                   bool compressedRepresentation = true );
    EllipsoidAtom( bool compressedRepresentation = true );
    EllipsoidAtom( const EllipsoidAtom& other );
    virtual ~EllipsoidAtom();

    EllipsoidAtom& operator=( const EllipsoidAtom& other );

    Atom* clone() const;

    bool hasCompressedRepresentation() const;
    void switchRepresentation();

    uint8_t getType() const;
    void setCenter( const Vector3d< float >& center );
    Vector3d< float > getCenter() const;
    float getSpanRadius() const;
    float getRadius1() const;
    float getRadius2() const;
    float getRadius3() const;
    Vector3d< float > getRadii() const;
    Vector3d< float > getAxis1() const;
    Vector3d< float > getAxis2() const;
    Vector3d< float > getAxis3() const;
    Vector3d< float > getRotationAngles( bool degree = false ) const;


    // these methods extract the information from the compressed representation
    void getStdParameters( Vector3d< float >& center,
                           float& radius1,
                           float& radius2,
                           float& radius3,
                           Vector3d< float >& axis1,
                           Vector3d< float >& axis2,
                           Vector3d< float >& axis3 ) const;
    void getStdParameters( Vector3d< float >& center,
                           float& radius1,
                           float& radius2,
                           float& radius3,
                           float& rotationAngleX,
                           float& rotationAngleY,
                           float& rotationAngleZ,
                           bool degree = false ) const;
    void getStdParameters( Vector3d< float >& center,
                           Vector3d< float >& radii,
                           Vector3d< float >& rotationAngles,
                           bool degree = false ) const;


    float getVolume() const;

    bool contains( const Vector3d< float >& position ) const;

    void getTranslatedParameters( float* outputParameters,
                                  float translationX,
                                  float translationY,
                                  float translationZ ) const;

    static uint8_t getStaticType();

    static void getStdParametersFromVectorRepresentation(
                               const float* vectorRepresentation,
                               bool compressedRepresentation,
                               Vector3d< float >& center,
                               float& radius1,
                               float& radius2,
                               float& radius3,
                               Vector3d< float >& axis1,
                               Vector3d< float >& axis2,
                               Vector3d< float >& axis3 );
    static void getVectorRepresentationFromStdParameters(
                               const Vector3d< float >& center,
                               const float& radius1,
                               const float& radius2,
                               const float& radius3,
                               const Vector3d< float >& axis1,
                               const Vector3d< float >& axis2,
                               const Vector3d< float >& axis3,
                               float* vectorRepresentation,
                               bool compressedRepresentation );
    static void getMatrixRepresentationFromVectorRepresentation(
                                              const float* vectorRepresentation,
                                              bool compressedRepresentation,
                                              Matrix& matrixRepresentation );
    static void getVectorRepresentationFromMatrixRepresentation(
                                             const Matrix& matrixRepresentation,
                                             float* vectorRepresentation,
                                             bool compressedRepresentation );
    static void getTranslatedVectorRepresentationFromVectorRepresentation(
                                          const float* vectorRepresentation,
                                          bool compressedRepresentation,
                                          float translationX,
                                          float translationY,
                                          float translationZ,
                                          float* outputVectorRepresentation,
                                          bool outputCompressedRepresentation );


    static void getAtomLutVoxel( const float* vectorRepresentation,
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

    static void getBoundingBox( const float* vectorRepresentation,
                                bool compressedRepresentation,
                                float& lowerX,
                                float& lowerY,
                                float& lowerZ,
                                float& upperX,
                                float& upperY,
                                float& upperZ );

    static void getAtomLutVoxelSpan( const float* vectorRepresentation,
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

    static float dotProduct( const float* atomParameters,
                             bool compressedRepresentation,
                             const float& coordinateX,
                             const float& coordinateY,
                             const float& coordinateZ );
    static float upperThreeByThreeMatrixBilinearProduct( 
                              const float* atomParameters,
                              bool compressedRepresentation,
                              const float& firstCoordinateX,
                              const float& firstCoordinateY,
                              const float& firstCoordinateZ,
                              const float& secondCoordinateX,
                              const float& secondCoordinateY,
                              const float& secondCoordinateZ );
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

