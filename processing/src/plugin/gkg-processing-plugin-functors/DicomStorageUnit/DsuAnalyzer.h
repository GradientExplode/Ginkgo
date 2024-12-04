#ifndef _gkg_processing_plugin_functors_DicomStorageUnit_DsuAnalyzer_h_
#define _gkg_processing_plugin_functors_DicomStorageUnit_DsuAnalyzer_h_


#include <gkg-core-cppext/StdInt.h>

#include <string>
#include <iostream>
#include <list>
#include <deque>
#include <map>


namespace gkg
{


class DsuSeries
{

  public:

    enum Type
    {

      DICOM,
      UNKNOWN

    };

    DsuSeries( const std::string& institutionName,
               const std::string& manufacturersModelName,
               const std::string& date,
               const std::string& patientId,
               int32_t studyId,
               int32_t seriesId,
               Type type );
    DsuSeries( const DsuSeries& other );
    virtual ~DsuSeries();

    const std::string& getInstitutionName() const;
    const std::string& getManufacturersModelName() const;
    const std::string& getDate() const;
    const std::string& getPatientId() const;
    int32_t getStudyId() const;
    int32_t getDsuSeriesId() const;

    std::string getExamString() const;

    Type getType() const;

    std::string getOriginName( int32_t index ) const;
    std::string getDestinationName( int32_t index ) const;

    int32_t getCount() const;

  private:

    friend class DsuAnalyzer;

    void addImage( int32_t imageNumber, const std::string& originFileName );
    bool check( const std::string& institutionName,
                const std::string& manufacturersModelName,
                const std::string& date,
                const std::string& patientId,
                int32_t studyId,
                int32_t seriesId ) const;
    void updateLut();

    std::string _institutionName;
    std::string _manufacturersModelName;
    std::string _date;
    std::string _patientId;
    int32_t _studyId;
    int32_t _seriesId;
    Type _type;
    std::deque< int32_t > _lutImageNumber; 
    std::deque< std::string > _lutOriginFileName; 
    std::map< int32_t, std::string > _originFileNames;

};


class DsuAnalyzer
{

  public:

    DsuAnalyzer( const std::string& directory,
                 const std::string& defaultInstitutionName );
    virtual ~DsuAnalyzer();

    const std::string& getDirectory() const;

    const std::list< DsuSeries >& getSeries() const;

  private:

    void analyzeSelection();
    std::list< std::string > getFileNames() const;
    std::string getNextName( std::istream& is, char separator ) const;
    void resolveLinkOrDirectory( const std::string& name,
                                 std::list< std::string >& names ) const;
    DsuSeries::Type analyzeFile( const std::string& fileName,
                              	 std::string& institutionName,
                              	 std::string& manufacturersModelName,
                              	 std::string& date,
                              	 std::string& patientId,
                              	 int32_t& studyId,
                              	 int32_t& seriesId,
                              	 int32_t& imageNumber ) const;

    void analyzeDicomFile( const std::string& fileName,
                           std::string& institutionName,
                           std::string& manufacturersModelName,
                           std::string& date,
                           std::string& patientId,
                           int32_t& studyId,
                           int32_t& seriesId,
                           int32_t& imageNumber ) const;

    std::string _directory;
    std::string _defaultInstitutionName;
    std::list< DsuSeries > _series;

};


}


namespace std
{


ostream& operator << ( ostream& os, const gkg::DsuSeries& thing );


}

#endif
