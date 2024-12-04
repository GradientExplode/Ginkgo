#ifndef _gkg_qmri_plugin_functors_RelaxometryT1CorticalLaminationMapper_RelaxometryT1CorticalLaminationMapperCommand_h_
#define _gkg_qmri_plugin_functors_RelaxometryT1CorticalLaminationMapper_RelaxometryT1CorticalLaminationMapperCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-processing-numericalanalysis/Vector.h>

#include <string>
#include <vector>


namespace gkg
{


class RelaxometryT1CorticalLaminationMapperCommand : public Command,
               public Creator2Arg< RelaxometryT1CorticalLaminationMapperCommand,
                                   Command,
                                   int32_t,
                                   char** >,
               public Creator1Arg< RelaxometryT1CorticalLaminationMapperCommand,
                                   Command,
                                   const Dictionary& >
{

  public:

    RelaxometryT1CorticalLaminationMapperCommand(
                               int32_t argc,
                               char* argv[], 
                               bool loadPlugin = false,
                               bool removeFirst = true );
    RelaxometryT1CorticalLaminationMapperCommand(
                               const std::string& fileNameIn, 
                               const std::string& fileNameTimes,
                               const std::string& fileNameMask,
                               const std::string& fileNameOut,
                               const std::string& fileNameFittedData,
                               const std::string& fileNameHistogram,
                               int32_t componentCount,
                               const std::string& optimizerType,
                               const std::vector< double >& optimizerParameters,
                               const std::vector< double >& lowerBoundaries,
                               const std::vector< double >& upperBoundaries,
                               const std::vector< double >& histogramParameters,
                               bool ascii,
                               const std::string& format,
                               bool verbose );
    RelaxometryT1CorticalLaminationMapperCommand(
                               const Dictionary& parameters );
    virtual ~RelaxometryT1CorticalLaminationMapperCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< RelaxometryT1CorticalLaminationMapperCommand,
                              Command,
                              int32_t, char** >;
    friend class Creator1Arg< RelaxometryT1CorticalLaminationMapperCommand,
                              Command,
                              const Dictionary& >;

    static bool sortingT1( const std::pair< int32_t, double >& a,
                           const std::pair< int32_t, double >& b );

    void parse();
    void execute( const std::string& fileNameIn, 
                  const std::string& fileNameTimes,
                  const std::string& fileNameMask,
                  const std::string& fileNameOut,
                  const std::string& fileNameFittedData,
                  const std::string& fileNameHistogram,
                  int32_t componentCount,
                  const std::string& optimizerType,
                  const std::vector< double >& optimizerParameters,
                  const std::vector< double >& lowerBoundaries,
                  const std::vector< double >& upperBoundaries,
                  const std::vector< double >& histogramParameters,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

    double getBayesianInformationCriterion(
                  const Vector& measuredHistogramOrdinates,
                  const Vector& fittedHistogramOrdinates,
                  int32_t gaussianCount ) const;

    int32_t getClosestT1Class(
                         double t1Value,
                         const  Vector& sortedOptimalGaussianParameters ) const;

};


}


#endif
