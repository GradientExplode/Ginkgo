#ifndef _gkg_processing_roi_LabelledRoiDictionary_h_
#define _gkg_processing_roi_LabelledRoiDictionary_h_

#include<gkg-core-object/HeaderedObject.h>
#include<gkg-core-object/Dictionary.h>
#include<string>
#include<set>


namespace gkg
{


class LabelledRoiDictionary : public HeaderedObject
{

  public:

    LabelledRoiDictionary( const std::string& name );
    LabelledRoiDictionary( const LabelledRoiDictionary& other );
    LabelledRoiDictionary( const HeaderedObject& headeredObject,
                           const std::string& name );
    LabelledRoiDictionary( const Dictionary& dictionary,
                           const std::string& name );
    virtual ~LabelledRoiDictionary();

    LabelledRoiDictionary& operator=( const LabelledRoiDictionary& other );

    std::string getName() const;

    Dictionary getDictionary() const;
    int32_t getRoiCount() const;
    Dictionary getRoiDictionary( const std::string& roiKey ) const;

    std::string getRoiName( const std::string& roiKey ) const;
    std::string getRoiAcronym( const std::string& roiKey ) const;
    template < class T >
    void getRoiLabel( const std::string& roiKey, T& label ) const;
    double getConfidenceLevel( const std::string& roiKey ) const;
    template < class T >
    void getAdjacencies( std::set< std::pair< T, T > >& adjacencies ) const;

    void addSemantic( HeaderedObject& headeredObject ) const;
    void addToHeaderedObject( HeaderedObject& headeredObject ) const;

  protected:

    std::string _name;

};

}

#endif
