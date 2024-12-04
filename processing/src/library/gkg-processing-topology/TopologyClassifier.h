#ifndef _gkg_processing_topology_TopologyClassifier_h_
#define _gkg_processing_topology_TopologyClassifier_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-processing-topology/TopologicalComponent.h>


namespace gkg
{


#define GKG_TOPOLOGY_UNKNOWN                  0
#define GKG_TOPOLOGY_VOLUME_POINT             10
#define GKG_TOPOLOGY_ISOLATED_POINT           20
#define GKG_TOPOLOGY_SIMPLE_POINT             30
#define GKG_TOPOLOGY_CURVE_POINT              40
#define GKG_TOPOLOGY_CURVES_JUNCTION          50
#define GKG_TOPOLOGY_SURFACE_POINT            60
#define GKG_TOPOLOGY_SURFACE_CURVES_JUNCTION  70
#define GKG_TOPOLOGY_SURFACES_JUNCTION        80
#define GKG_TOPOLOGY_SURFACES_CURVES_JUNCTION 90


//
// Topological class matrix
//
// ---------------------------------
// |     \ Cbar|    |    |    |    |
// | Cstar\    |  0 |  1 |  2 |  3 |
// |-----------+----+----+----+----|
// |     0     |  0 | 20 | 20 | 20 |
// |-----------+----+----+----+----|
// |     1     | 10 | 30 | 60 | 80 |
// |-----------+----+----+----+----|
// |     2     | 10 | 40 | 70 | 90 |
// |-----------+----+----+----+----|
// |     3     | 10 | 50 | 70 | 90 |
// ---------------------------------


class TopologyClassifier : public Singleton< TopologyClassifier >
{

  public :

    typedef uint8_t TopologicalClassMatrix[ 4 ][ 4 ];

    ~TopologyClassifier();

    void setStride( int32_t stride );

    template < class T >
    bool isSimplePoint( const Volume< T >& volume,
                        const TestFunction< T >& testFunction,
                        const Vector3d< int32_t >& site,
                        const int32_t& t = 0 );
    template < class T >
    bool isSimplePoint( const Volume< T >& volume,
                        const TestFunction< T >& testFunction,
                        const Vector3d< int32_t >& site,
                        int32_t& CStar,
                        int32_t& CBar,
                        const int32_t& t = 0 );
    template < class T >
    bool isCurvePoint( const Volume< T >& volume,
                       const TestFunction< T >& testFunction,
                       const Vector3d< int32_t >& site,
                       int32_t* CStar = 0,
                       int32_t* CBar = 0,
                       const int32_t& t = 0 );
    template < class T >
    bool isSurfacePoint( const Volume< T >& volume,
                         const TestFunction< T >& testFunction,
                         const Vector3d< int32_t >& site,
                         int32_t* CStar = 0,
                         int32_t* CBar = 0,
                         const int32_t& t = 0 );

    template < class T >
    void getCStarAndCBar( const Volume< T >& volume,
                          const TestFunction< T >& testFunction,
                          const Vector3d< int32_t >& site,
                          int32_t& CStar,
                          int32_t& CBar,
                          const int32_t& t = 0 );

    template < class T >
    Volume< uint8_t > getClassification( 
                                        const Volume< T >& volumeIn,
                                        const TestFunction< T >& testFunction );

  protected:

    friend class Singleton< TopologyClassifier >;

    TopologyClassifier();

    int32_t binaryDecisionDiagram( int32_t* X, bool& inverse );

    TopologicalComponent _component26;
    TopologicalComponent _component18;

    TopologicalClassMatrix _classMatrix;

    std::list< gkg::Vector3d< int32_t > > _neighbors;

};

}


#endif
