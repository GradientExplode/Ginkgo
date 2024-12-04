#ifndef _gkg_processing_contaner_Roi_h_
#define _gkg_processing_contaner_Roi_h_

#include <gkg-core-object/HeaderedObject.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/RCPointer.h>
#include <list>
#include <map>
#include <string>


namespace gkg
{


template < class T > class RoiMeasure;


template < class T >
class Roi : public HeaderedObject
{

  public:

    Roi( RCPointer< Volume< T > > labelVolume,
         const T& label,
         int32_t stride = 1 );
    Roi( const Roi< T >& other );
    virtual ~Roi();

    Roi< T >& operator=( const Roi< T >& other);

    void addMeasure( const std::string& name,
                     RCPointer< RoiMeasure< T > > roiMeasure );

    RCPointer< RoiMeasure< T > > getMeasure( const std::string& name ) const;
    std::list< RCPointer< RoiMeasure< T > > > getMeasures() const;

    RCPointer< Volume< T > > getLabelVolume() const;
    const T& getLabel() const;
    const int32_t& getStride() const;

  protected:

    RCPointer< Volume< T > > _labelVolume;
    T _label;
    std::map< std::string, RCPointer< RoiMeasure< T > > > _measures;
    int32_t _stride;

};


}


#endif 
