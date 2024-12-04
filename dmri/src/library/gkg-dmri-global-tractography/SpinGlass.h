#ifndef _gkg_dmri_global_tractography_SpinGlass_h_
#define _gkg_dmri_global_tractography_SpinGlass_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-pattern/RCPointer.h>
#include <list>


namespace gkg
{


class SpinGlassConnection;
template < class T > class LightCurve3d;


class SpinGlass
{

  public:

    enum ExtremityType
    {

      Forward = 0U,
      Backward = 1U

    };

    typedef RCPointer< SpinGlass > ConnectedSpinGlasses[ 2 ];
    typedef RCPointer< SpinGlassConnection > SpinGlassConnections[ 2 ];

    SpinGlass( uint64_t identity,
               const Vector3d< float >& position,
               const Vector3d< float >& orientation,
               float length );
    SpinGlass( const SpinGlass& other );
    virtual ~SpinGlass();

    SpinGlass& operator=( const SpinGlass& other );

    uint64_t getIdentity() const;
    const Vector3d< float >& getPosition() const;
    const Vector3d< float >& getOrientation() const;
    float getLength() const;

    const RCPointer< SpinGlass >& getConnectedSpinGlass(
                                     const ExtremityType& extremityType ) const;
    const RCPointer< SpinGlassConnection >& 
             getSpinGlassConnection( const ExtremityType& extremityType ) const;
    const Vector3d< float >&
                       getExtremity( const ExtremityType& extremityType ) const;
    const Vector3d< float >&
             getExtremityAndOppositeExtremityType(
                                   const ExtremityType& extremityType,
                                   ExtremityType& oppositeExtremityType ) const;

    void getOptimalPositionAndOrientation(
                                         Vector3d< float >& position,
                                         Vector3d< float >& orientation ) const;

    void setPosition( const Vector3d< float >& position );
    void setOrientation( const Vector3d< float >& orientation );
    void setLength( float length );

    void setConnectedSpinGlass(
                             const ExtremityType& extremityType,
                             const RCPointer< SpinGlass >& connectedSpinGlass );
    void setSpinGlassConnection(
                  const ExtremityType& extremityType,
                  const RCPointer< SpinGlassConnection >& spinGlassConnection );

    void resetConnectedSpinGlass( const ExtremityType& extremityType );
    void resetSpinGlassConnection( const ExtremityType& extremityType );

    bool isIsolated() const;
    bool isConnected( const ExtremityType& extremityType ) const;
    bool isAnExtremity( ExtremityType* freeExtremityType = 0 ) const;

    void checkConsistency() const;

    float getInteractionPotentialWithOther(
                               const ExtremityType& spinGlassExtremityType,
                               float connectionLikelihood,
                               const RCPointer< SpinGlass >& otherSpinGlass,
                               const ExtremityType& otherSpinGlassExtremityType,
                               float otherConnectionLikelihood,
                               float spinGlassDistanceToCortex = 1e38,
                               float otherSpinGlassDistanceToCortex = 1e38,
                               float minimumDistance = 0.0f,
                               float maximumDistance = 1.0f ) const;


    //
    //                   ---->o<----
    //                     ^   ^ ^ ^
    //                     |   | | |
    //        this(spin glass) | | |
    //                         | | |
    //                         | | +---- followerOppositeSpinGlassExtremityType
    //                         +-|------ followerConnectedSpinGlassExtremityType
    //                           |
    //           +---------------+
    //           |
    //        follower(spin glass)
    //
    const RCPointer< SpinGlass >& getFollower( 
                  const ExtremityType& spinGlassExtremityType,
                  ExtremityType& followerConnectedSpinGlassExtremityType,
                  ExtremityType& followerOppositeSpinGlassExtremityType ) const;


    void track( LightCurve3d< float >& curve3d,
                int32_t neighborCount = -1 ) const;
    void trackFromMiddle( LightCurve3d< float >& curve3d,
                          int32_t neighborCount = -1 ) const;

    static 
    ExtremityType getOppositeExtremityType( ExtremityType extremityType );

    static
    std::list< RCPointer< SpinGlass > > 
      getNeighborhood( const RCPointer< SpinGlass >& spinGlass,
                       int32_t neighborCount = -1 );

  protected:

    void updateExtremities();

    uint64_t _identity;
    Vector3d< float > _position;
    Vector3d< float > _orientation;
    Vector3d< float > _backwardExtremity;
    Vector3d< float > _forwardExtremity;
    float _length;

    ConnectedSpinGlasses _connectedSpinGlasses;
    SpinGlassConnections _spinGlassConnections;

};


struct SpinGlassCompare /*: public std::binary_function< SpinGlass,
                                                       SpinGlass,
                                                       bool >
                          deprecated in c++17 */
{

  typedef SpinGlass first_argument_type;
  typedef SpinGlass second_argument_type;
  typedef bool result_type;

  bool operator()( const SpinGlass& s1, const SpinGlass& s2 ) const
  {

    return ( s1.getIdentity() < s2.getIdentity() );

  }

};


}


#endif

