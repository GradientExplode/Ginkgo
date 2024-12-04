#ifndef _gkg_processing_mesh_MeshFactory_h_
#define _gkg_processing_mesh_MeshFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-processing-container/LightCurve3d.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-object/Dictionary.h>
#include <vector>
#include <map>
#include <string>


namespace gkg
{


class MeshFactory : public Singleton< MeshFactory >
{

  public:

    ~MeshFactory();

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getCylinder( const Vector3d< float >& center,
                   const Vector3d< float >& axis,
                   float length,
                   float radius1,
                   float radius2,
                   int32_t facetCount,
                   bool closed,
                   bool randomRotation ) const;

    RCPointer< MeshMap< int32_t, float, 3U > >
      getGeneralizedCylinder( const LightCurve3d< float >& crossSection,
                              float length,
                              bool closed,
                              bool randomRotation ) const;

    RCPointer< MeshMap< int32_t, float, 3U > >
      getFiber( const LightCurve3d< float >& lightCurve3d,
                const std::vector< float >& radii,
                int32_t vertexCountPerPlane,
                bool randomRotation,
                int32_t destinationRank = 0 ) const;

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getEllipsoid( const Vector3d< float >& center,
                    const Vector3d< float >& e1,
                    const Vector3d< float >& e2,
                    const Vector3d< float >& e3,
                    float radius1,
                    float radius2,
                    float radius3,
                    int32_t sphereVertexCount,
                    bool randomRotation ) const;

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getSphere( const Vector3d< float >& center,
                 float radius,
                 int32_t sphereVertexCount ) const;

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getStar( const Vector3d< float >& center,
               float radius,
               int32_t peakCount,
               const std::vector< float >& peakMagnitudes,
               const std::vector< float >& peakApertureAngles,
               int32_t sphereVertexCount,
               bool sharpPeak,
               bool symmetricPeak,
               bool randomRotation ) const;

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getBox( const Vector3d< float >& lowerPoint,
              const Vector3d< float >& upperPoint,
              bool randomRotation ) const;

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getNetworkOfCylinders( const Vector3d< float >& center,
                             const Vector3d< float >& axis,
                             float length,
                             float radius1,
                             float radius2,
                             int32_t facetCount,
                             bool closed,
                             const Vector3d< float >& networkAxis1,
                             const Vector3d< float >& networkAxis2,
                             float networkSpacing1,
                             float networkSpacing2,
                             int32_t networkCount1,
                             int32_t networkCount2,
                             bool randomRotation ) const;

    RCPointer< MeshMap< int32_t, float, 3U > >
      getNetworkOfGeneralizedCylinders(
                                      const LightCurve3d< float >& crossSection,
                                      float length,
                                      bool closed,
                                      const Vector3d< float >& networkAxis1,
                                      const Vector3d< float >& networkAxis2,
                                      float networkSpacing1,
                                      float networkSpacing2,
                                      int32_t networkCount1,
                                      int32_t networkCount2,
                                      bool randomRotation ) const;

    RCPointer< MeshMap< int32_t, float, 3U > >
      getNetworkOfFibers( const LightCurve3d< float >& lightCurve3d,
                          const std::vector< float >& radii,
                          int32_t vertexCountPerPlane,
                          const Vector3d< float >& networkAxis1,
                          const Vector3d< float >& networkAxis2,
                          float networkSpacing1,
                          float networkSpacing2,
                          int32_t networkCount1,
                          int32_t networkCount2,
                          bool randomRotation ) const;

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getNetworkOfEllipsoids( const Vector3d< float >& center,
                              const Vector3d< float >& e1,
                              const Vector3d< float >& e2,
                              const Vector3d< float >& e3,
                              float radius1,
                              float radius2,
                              float radius3,
                              int32_t sphereVertexCount,
                              const Vector3d< float >& networkAxis1,
                              const Vector3d< float >& networkAxis2,
                              const Vector3d< float >& networkAxis3,
                              float networkSpacing1,
                              float networkSpacing2,
                              float networkSpacing3,
                              int32_t networkCount1,
                              int32_t networkCount2,
                              int32_t networkCount3,
                              bool randomRotation ) const;

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getNetworkOfSpheres( const Vector3d< float >& center,
                           float radius,
                           int32_t sphereVertexCount,
                           const Vector3d< float >& networkAxis1,
                           const Vector3d< float >& networkAxis2,
                           const Vector3d< float >& networkAxis3,
                           float networkSpacing1,
                           float networkSpacing2,
                           float networkSpacing3,
                           int32_t networkCount1,
                           int32_t networkCount2,
                           int32_t networkCount3 ) const;

    RCPointer< MeshMap< int32_t, float ,3U > >
      getNetworkOfStars( const Vector3d< float >& center,
                         float radius,
                         int32_t peakCount,
                         const std::vector< float >& peakMagnitudes,
                         const std::vector< float >& peakApertureAngles,
                         int32_t sphereVertexCount,
                         bool sharpPeak,
                         bool symmetricPeak,
                         const Vector3d< float >& networkAxis1,
                         const Vector3d< float >& networkAxis2,
                         const Vector3d< float >& networkAxis3,
                         float networkSpacing1,
                         float networkSpacing2,
                         float networkSpacing3,
                         int32_t networkCount1,
                         int32_t networkCount2,
                         int32_t networkCount3,
                         bool randomRotation ) const;

    RCPointer< MeshMap< int32_t, float, 3U > >
      getNetworkOfBoxes( const Vector3d< float >& lowerPoint,
                         const Vector3d< float >& upperPoint,
                         const Vector3d< float >& networkAxis1,
                         const Vector3d< float >& networkAxis2,
                         const Vector3d< float >& networkAxis3,
                         float networkSpacing1,
                         float networkSpacing2,
                         float networkSpacing3,
                         int32_t networkCount1,
                         int32_t networkCount2,
                         int32_t networkCount3,
                         bool randomRotation ) const;

    std::map< std::string, RCPointer< MeshMap< int32_t, float, 3U > > >
      getMeshes( const std::string& fileName ) const;


  protected:
  
    friend class Singleton< MeshFactory >;
    
    MeshFactory();

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getCylinder( Dictionary& parameters ) const;

    RCPointer< MeshMap< int32_t, float, 3U > >
      getGeneralizedCylinder( Dictionary& parameters ) const;

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getFiber( Dictionary& parameters ) const;

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getEllipsoid( Dictionary& parameters ) const;

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getSphere( Dictionary& parameters ) const;

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getStar( Dictionary& parameters ) const;

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getBox( Dictionary& parameters ) const;

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getNetworkOfCylinders( Dictionary& parameters ) const;

    RCPointer< MeshMap< int32_t, float, 3U > >
      getNetworkOfGeneralizedCylinders( Dictionary& parameters ) const;

    RCPointer< MeshMap< int32_t, float, 3U > >
      getNetworkOfFibers( Dictionary& parameters ) const;

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getNetworkOfEllipsoids( Dictionary& parameters ) const;

    RCPointer< MeshMap< int32_t, float, 3U > > 
      getNetworkOfSpheres( Dictionary& parameters ) const;

    RCPointer< MeshMap< int32_t, float,3U > >
      getNetworkOfStars( Dictionary& parameters ) const;

    RCPointer< MeshMap< int32_t, float,3U > >
      getNetworkOfBoxes( Dictionary& parameters ) const;

    std::vector< Vector3d< float > >
      getRotationAngles( int32_t angleCount ) const;

    Vector3d< float > getVector3d( GenericObject& genericObject ) const;
    float getFloat( GenericObject& genericObject ) const;
    int32_t getInteger( GenericObject& genericObject ) const;
    bool getBoolean( GenericObject& genericObject ) const;
    LightCurve3d< float > getLightCurve3d( GenericObject& genericObject ) const;
    std::vector< float > getVectorOfFloat( GenericObject& genericObject ) const;

};


}


#endif
