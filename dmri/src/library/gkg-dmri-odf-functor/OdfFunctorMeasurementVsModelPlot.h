#ifndef _gkg_dmri_odf_functor_OdfFunctorMeasurementVsModelPlot_h_
#define _gkg_dmri_odf_functor_OdfFunctorMeasurementVsModelPlot_h_


#include <gkg-dmri-odf-functor/OdfFunctor.h>
#include <gkg-dmri-microstructure-plot/MeasurementVsModelPlotter.h>
#include <gkg-dmri-microstructure-plot/MeasurementVsModelAttenuation.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class OdfFunctorMeasurementVsModelPlot :
                          public OdfFunctor< MeasurementVsModelPlotter,
                                             MeasurementVsModelAttenuation >,
                          public Singleton< OdfFunctorMeasurementVsModelPlot >
{

  public:

    ~OdfFunctorMeasurementVsModelPlot();

    std::string getName() const;
    bool isStandardBasisRequired() const;


    // scalarParameters[ 0 ] : RGB scale
    // scalarParameters[ 1 ] : mesh scale
    // scalarParameters[ 2 ] : lower GFA threshold
    // scalarParameters[ 3 ] : upper GFA threshold
    // scalarParameters[ 4 ] : ascii
    // scalarParameters[ 5 ] : output orientation count
    // scalarParameters[ 6 ] : b-value delta variation in s/mm2
    // scalarParameters[ 7 ] : model orientation count 


    // stringParameters[ 0 ] : volume format
    // stringParameters[ 1 ] : mesh format
    // stringParameters[ 2 ] : texture format
    // stringParameters[ 3 ] : output filename
    // stringParameters[ 4 ] : mask filename
    // stringParameters[ 5 ] : 3D transformation from mask to ODF Cartesian 
    //                         field filename


    void process( MeasurementVsModelPlotter& output,
                  OdfCartesianField& odfCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< OdfFunctorMeasurementVsModelPlot >;

    OdfFunctorMeasurementVsModelPlot();        

};


}


#endif
