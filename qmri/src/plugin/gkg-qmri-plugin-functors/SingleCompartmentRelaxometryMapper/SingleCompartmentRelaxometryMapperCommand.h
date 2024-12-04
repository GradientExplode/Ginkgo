#ifndef _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_SingleCompartmentRelaxometryMapperCommand_h_
#define _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_SingleCompartmentRelaxometryMapperCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/RCPointer.h>

#include <string>
#include <vector>


namespace gkg
{


class SingleCompartmentRelaxometryMapperCommand : public Command, 
                  public Creator2Arg< SingleCompartmentRelaxometryMapperCommand,
                                      Command,
                                      int32_t,
                                      char** >,
                  public Creator1Arg< SingleCompartmentRelaxometryMapperCommand,
                                      Command,
                                      const Dictionary& >
{

  public: 
  
    SingleCompartmentRelaxometryMapperCommand( int32_t argc, 
                                         char* argv[],
                                         bool loadPlugin = false,
                                         bool removeFirst = true );
    SingleCompartmentRelaxometryMapperCommand( 
                    const std::vector< std::string >& fileNameInputVolumes,
                    const std::string& fileNameMask,
                    const std::string& qMriMappingType,
                    const std::vector< double >& optimizerParameters,
                    const std::vector< double >& scalarParameters,
                    const std::vector< std::string >& stringParameters,
                    const std::string& fileNameOutputProtonDensityVolume,
                    const std::string& fileNameOutputRelaxationTimeVolume,
                    const std::string& fileNameOutputScalingVolume,
                    const std::string& fileNameFittedMeasurementVolume, 
                    bool ascii, 
                    const std::string& format, 
                    bool verbose ); 
    SingleCompartmentRelaxometryMapperCommand( const Dictionary& parameters );
    virtual ~SingleCompartmentRelaxometryMapperCommand();
    
    static std::string getStaticName();
    
  protected:
    
    friend class Creator2Arg< SingleCompartmentRelaxometryMapperCommand, 
                              Command, 
                              int32_t,
                              char** >;
    friend class Creator1Arg< SingleCompartmentRelaxometryMapperCommand,
                              Command,
                              const Dictionary& >;
    
    void parse();
    void execute( const std::vector< std::string >& fileNameInputVolumes,
                  const std::string& fileNameMask,
                  const std::string& qMriMappingType,
                  const std::vector< double >& optimizerParameters,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  const std::string& fileNameOutputProtonDensityVolume,
                  const std::string& fileNameOutputRelaxationTimeVolume,
                  const std::string& fileNameOutputScalingVolume,
                  const std::string& fileNameFittedMeasurementVolume, 
                  bool ascii, 
                  const std::string& format, 
                  bool verbose ); 

};


}


#endif
