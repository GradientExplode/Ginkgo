#ifndef _gkg_fmri_contrast_ContrastEffects_h_
#define _gkg_fmri_contrast_ContrastEffects_h_

#include <gkg-core-object/HeaderedObject.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-fmri-paradigm/Paradigm.h>
#include <gkg-processing-numericalanalysis/KalmanFilter.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/Vector.h>

#include <map>
#include <string>
#include <vector>


namespace gkg
{


class ContrastEffects : public HeaderedObject
{

  public:

    struct Information
    {

      Information();
      Information( const std::string& theName,
                   const std::vector< std::string >& theRegressorNames,
                   const Vector& theWeights );
      Information( const Information& other );

      Information& operator = ( const Information& other );

      std::string name;
      std::vector< std::string > regressorNames;
      Vector weights;

    };

    ContrastEffects();
    ContrastEffects( const Paradigm& paradigm );
    virtual ~ContrastEffects();

    void setParadigm( const Paradigm& paradigm );

    int32_t getRegressorCount() const;
    std::string getRegressorName( int32_t index ) const;
    int32_t getRegressorIndex( const std::string& name ) const;

    int32_t getContrastCount() const;
    std::string getContrastName( int32_t index ) const;
    int32_t getContrastIndex( const std::string& name ) const;

    void addContrastEffect( const Information& information );
    const Information& getInformation( int32_t index ) const;

    void process( const Volume< Kalman::Parameters* >& kalmanLut,
                  Volume< float >& volumeOfEffects,
                  Volume< float >& volumeOfEffectVariances ) const;
    void process( const Volume< Kalman::Parameters* >& kalmanLut,
                  const std::vector< Vector3d< int32_t > >& sites,
                  Volume< float >& volumeOfEffects,
                  Volume< float >& volumeOfEffectVariances ) const;

  protected:

    int32_t _regressorCount;         // i.e. stimuli + 1 + driftPolynomOrder
    std::map< std::string, int32_t > _stimuliNameToIndexLut;
    std::map< int32_t, std::string > _stimuliIndexToNameLut;

    std::map< std::string, int32_t > _contrastNameToIndexLut;
    std::map< int32_t, std::string > _contrastIndexToNameLut;
    std::map< int32_t, Information > _informationLut;

    std::map< int32_t, Vector > _contrasts;

};


}


#endif
