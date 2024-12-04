#ifndef _gkg_simulation_mri_sequence_PGSESequence_h_
#define _gkg_simulation_mri_sequence_PGSESequence_h_


#include <gkg-simulation-mri-sequence/MRISequence.h>
#include <gkg-core-pattern/Creator.h>
#include <string>


namespace gkg
{


class PGSESequence : public MRISequence,
                     public Creator4Arg< PGSESequence,
                                         MRISequence,
                                         const Dictionary&,
                                         float,
                                         bool,
                                         std::ofstream* >
{

  public:

    virtual ~PGSESequence();

    void getNormalizedValueAt( float timeInUs, 
                               float& normalizedGradientValue ) const;
    void getNormalizedIntegralValueAt(
                                 float timeInUs, 
                                 float& normalizedIntegralGradientValue ) const;

    void displayInformation( std::ofstream* osLog ) const;

    void saveDiffusionInformation( const std::string& fileName ) const;

    static std::string getStaticName();

  protected:

    friend struct Creator4Arg< PGSESequence,
                               MRISequence,
                               const Dictionary&,
                               float,
                               bool,
                               std::ofstream* >;

    PGSESequence( const Dictionary& parameters,
                  float timeStepInUs,
                  bool verbose,
                  std::ofstream* osLog );

    float _bigDeltaInUs;
    float _littleDeltaInUs;
    
    float _t0;
    float _t1;
    float _t2;
    float _t3;
    float _t4;
    float _t5;
    float _t6;
    float _t7;
    

};



}



#endif
