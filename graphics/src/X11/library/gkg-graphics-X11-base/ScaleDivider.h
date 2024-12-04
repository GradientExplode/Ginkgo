#ifndef _gkg_graphics_X11_base_ScaleDivider_h_
#define _gkg_graphics_X11_base_ScaleDivider_h_


#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <vector>


namespace gkg
{


template < class T >
class ScaleDivider : public RCGUIObject
{

  public:

    virtual ~ScaleDivider();

    const T& getLowerBound() const;
    const T& getUpperBound() const;
    const T& getMajorStep() const;

    int32_t getMajorCount() const;
    const T& getMajorMark( int32_t index ) const;

    int32_t getMinorCount() const;
    const T& getMinorMark( int32_t index ) const;

    static const T StepEpsilon;
    static const T BorderEpsilon;

  protected:

    ScaleDivider( const T& lowerBound,
                  const T& upperBound, const T& step );
    bool limitRange( T& value,
                     const T& lowerValue,
                     const T& upperValue,
                     const T& epislonRel = ( T )0,
                     const T& epsilonAbs = ( T )0 ) const;

    T _lowerBound;
    T _upperBound;
    T _majorStep;
    std::vector< T > _majorMarks;
    std::vector< T > _minorMarks;

};


}


#endif
