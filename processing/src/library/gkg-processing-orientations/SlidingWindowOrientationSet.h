#ifndef _gkg_processing_orientations_SlidingWindowOrientationSet_h_
#define _gkg_processing_orientations_SlidingWindowOrientationSet_h_


#include <gkg-core-cppext/StdInt.h>
#include <list>
#include <iostream>


namespace gkg
{


class Orientation;


class SlidingWindowOrientationSet
{

  public:

    SlidingWindowOrientationSet( int32_t windowSize,
                                 int32_t orientationCount,
                                 bool avoidSymmetricOrientations = true );
    virtual ~SlidingWindowOrientationSet();

    int32_t getWindowSize() const;
    int32_t getOrientationCount() const;
    const std::list< Orientation* >& getOrientations() const;

    float getGlobalEnergy( float threshold, float power ) const;

    void setForces( float threshold, float power );

    void move( float step );
    void undo();

    int32_t getSubSetCount() const;
    void moveSubSet( int32_t rank, float step );
    void undoSubSet( int32_t rank );

  protected:

    int32_t _windowSize;
    std::list< Orientation* > _orientations;
    int32_t _probabilityFactor;

};


}


namespace std
{


ostream& operator << ( ostream& os,
                       const gkg::SlidingWindowOrientationSet& thing );


}


#endif
