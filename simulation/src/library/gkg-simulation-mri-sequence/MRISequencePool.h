#ifndef _gkg_simulation_mri_sequence_MRISequencePool_h_
#define _gkg_simulation_mri_sequence_MRISequencePool_h_

#include <gkg-simulation-mri-sequence/MRISequence.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-object/Dictionary.h>
#include <vector>
#include <string>
#include <fstream>


namespace gkg
{


class MRISequencePool
{

  public:

    MRISequencePool( const gkg::Dictionary& dictionary,
                     float timeStepInUs,
                     bool verbose,
                     std::ofstream* osLog = 0 );
    virtual ~MRISequencePool();

    int32_t getCount() const;
    const RCPointer< MRISequence >& getMRISequence( int32_t index ) const;
    const std::string& getMRISequenceName( int32_t index ) const;

    void displayInformation( std::ofstream* osLog = 0 ) const;
    void saveNormalizedGradientProfiles(
                                    const std::string& directoryName ) const;

  protected:

    std::vector< RCPointer< MRISequence > > _mriSequences;
    std::vector< std::string > _mriSequenceNames;

};


}


#endif


