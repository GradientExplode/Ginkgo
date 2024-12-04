#ifndef _gkg_processing_mesh_MeshSlicer_h_
#define _gkg_processing_mesh_MeshSlicer_h_


//
// this class is an adaptation of the algorithm proposed in:
//
// An Optimal Algorithm for 3D Triangle Mesh Slicing
// R. Minetto, N. Volpato, J. Stolfi, R.M.M.H. Gregori and M.V.G. da Silva,
// pp. 1-10, 2017 Computer-Aided Design (CAD), Elsevier.
// http://dx.doi.org/10.1016/j.cad.2017.07.001
// 
// for which C++ and Python codes are available at the following URL:
//   https://github.com/rminetto/slicing
//


#include <gkg-processing-container/MeshMap.h>
#include <gkg-processing-container/Curve3dMap.h>
#include <map>
#include <list>
#include <vector>
#include <unordered_map>


namespace gkg
{





class MeshSlicer
{

  public:

    //
    // class Segment
    //
    class Segment
    {

      public:


        Segment()
        {

          index = 0;
          a = 0.0;
          b = 0.0;
          isVertical = false;

        }

        Segment( const Vector3d< float >& theExtremity1,
                 const Vector3d< float >& theExtremity2,
                 int32_t theIndex = 0 )
        {

          extremities[ 0 ] = theExtremity1;
          extremities[ 1 ] = theExtremity2;
          index = theIndex;
          if ( ( theExtremity2.x - theExtremity1.x ) != 0.0f )
          {

            a = ( double )( ( theExtremity2.y - theExtremity1.y ) /
                            ( theExtremity2.x - theExtremity1.x ) );
            b = ( double )( ( theExtremity1.y - ( a * theExtremity1.x ) ) );
            isVertical = false;

          }
          else
          {

            isVertical = true;

          }

        }

        Segment( const Segment& other )
        {

          extremities[ 0 ] = other.extremities[ 0 ];
          extremities[ 1 ] = other.extremities[ 1 ];
          index = other.index;
          a = other.a;
          b = other.b;
          isVertical = other.isVertical;

        }
                                          
        ~Segment()
        {
        }

        Segment& operator=( const Segment& other )
        {

          extremities[ 0 ] = other.extremities[ 0 ];
          extremities[ 1 ] = other.extremities[ 1 ];
          index = other.index;
          a = other.a;
          b = other.b;
          isVertical = other.isVertical;
          return *this;

        }

        bool operator==( const Segment& other ) const
        {

          return ( ( extremities[ 0 ].x == other.extremities[ 0 ].x ) &&
                   ( extremities[ 0 ].y == other.extremities[ 0 ].y ) &&
                   ( extremities[ 0 ].z == other.extremities[ 0 ].z ) &&
                   ( extremities[ 1 ].x == other.extremities[ 1 ].x ) &&
                   ( extremities[ 1 ].y == other.extremities[ 1 ].y ) &&
                   ( extremities[ 1 ].z == other.extremities[ 1 ].z ) ); 

        }

        Vector3d< float > extremities[ 2 ];
        int32_t index;
        double a;
        double b;
        bool isVertical;

    };


    //
    // struct HashVector3d
    //

    struct HashVector3d
    {
  
      size_t operator() ( const Vector3d< float >& v ) const
      {

        size_t h = std::hash< float>()( v.x );

        std::hash< float > hasher;
        h ^= hasher( v.y ) + 0x9e3779b9 + ( h << 6) + ( h >> 2 );
        h ^= hasher( v.z ) + 0x9e3779b9 + ( h << 6) + ( h >> 2 );

        return h;

      }

    };

    typedef std::unordered_map< Vector3d< float >,
                                std::vector< Vector3d< float > >,
                                HashVector3d > UnorderedMap;

    struct Contour
    {

      bool external;
      bool clockwise;
      std::vector< Vector3d< float > > P;

    };

    //
    // class MeshSlicer
    //

    MeshSlicer( float epsilon,
                float minimumThickness,
                float maximumThickness,
                bool adaptive = false,
                bool clockwise = true,
                bool verbose = false );
    virtual ~MeshSlicer();

    void slicing( const MeshMap< int32_t, float, 3U >& meshes,
                  std::map< int32_t, Curve3dMap< float > >& curve3dMaps ) const;
                  

  protected:

    float computePlanes( float lowerZ, float upperZ,
                         std::vector< float >& planes ) const;
    void incrementalSlicing( 
                          const std::vector< Vector3d< float > >& vertices,
                          const std::vector< Polygon< 3U > >& polygons,
                          const std::vector< float >& minimumZ,
                          const std::vector< float >& maximumZ,
                          const std::vector< float >& planes,
                          float delta,
                          std::list< LightCurve3d< float > >& curve3ds ) const;

    void getTriangleToPlaneIntersection( 
                               const std::vector< Vector3d< float > >& vertices,
                               const Polygon< 3U >& triangle,
                               float currentPlane,
                               Segment& segment ) const;
    void getExtremity( const Vector3d< float >& vertex1,
                       const Vector3d< float >& vertex2,
                       float currentPlane,
                       Vector3d< float >& extremity ) const;

    void constructContour( const std::list< Segment >& segments,
                           std::list< Contour >& contours ) const;

    std::vector< gkg::Vector3d< float > >
      startLoop( std::vector< UnorderedMap >& H ) const;
    void extendLoop( std::vector< Vector3d< float > >& P,
                     std::vector< UnorderedMap >& H ) const;

    void rayCasting( std::list< Contour >& contours ) const;

    void addPoint( const Vector3d< float >& p1,
                   const Vector3d< float >& p2,
                   std::vector< Segment>& segments,
                   BoundingBox< float >& boundingBox,
                   bool first,
                   int32_t index ) const;
    bool contains( const Vector3d< float >& point,
                   const BoundingBox< float >& boundingBox,
                   const std::vector< Segment >& segments,
                   int32_t index ) const;
    bool hasSegmentIntersection( const Segment& segment1,
                                 const Segment& segment2 ) const;
                                 
    bool isInside( const Segment& segment,
                   const Vector3d< float >& point ) const;

    float _epsilon;
    float _minimumThickness;
    float _maximumThickness;
    bool _adaptive;
    bool _clockwise;
    bool _verbose;

};


}


#endif
