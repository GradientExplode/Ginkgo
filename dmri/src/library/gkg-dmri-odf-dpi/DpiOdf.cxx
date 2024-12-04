#include <gkg-dmri-odf-dpi/DpiOdf.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::DpiOdf::DpiOdf( const gkg::OrientationSet* orientationSet,
		     const gkg::Volume< float >& t2,
		     const gkg::Volume< float >& dw,
		     const gkg::Vector3d< int32_t >& site,
		     int32_t numberOfShells,
		     int32_t numberOfOrientationsPerShell,
		     int32_t maximumSphericalHarmonicOrder,
		     float R0min, float R0max )
  : gkg::DpiEap( orientationSet, t2, dw, site, numberOfShells, 
		 numberOfOrientationsPerShell, maximumSphericalHarmonicOrder )
{

  try
  {

    // Computing ODF
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();
    
    //_values contains SH coefficients of the EAP
    int32_t odfSHCoefficientCount = _values.size();
    
    int32_t s, l, m;
    for ( s = 0; s < odfSHCoefficientCount; s++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( s, l, m );
      
      std::cout << R0min + R0max << " To be done...\n";
      
    }

  }
  GKG_CATCH( "gkg::DpiOdf::DpiOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "int32_t numberOfShells, "
	     "int32_t numberOfOrientationsPerShell, "
	     "int32_t maximumSphericalHarmonicOrder, "
	     "float R0min, float R0max )" );

}


gkg::DpiOdf::DpiOdf( const gkg::OrientationSet* orientationSet,
		     const gkg::Volume< float >& t2,
		     const gkg::Volume< float >& dw,
		     const gkg::Vector3d< float >& site,
		     int32_t numberOfShells,
		     int32_t numberOfOrientationsPerShell,
		     int32_t maximumSphericalHarmonicOrder,
		     float R0min, float R0max )
  : gkg::DpiEap( orientationSet, t2, dw, site, numberOfShells, 
		 numberOfOrientationsPerShell, maximumSphericalHarmonicOrder )
{

  try
  {

    // Computing ODF
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();
    
    //_values contains SH coefficients of the EAP
    int32_t odfSHCoefficientCount = _values.size();
    
    int32_t s, l, m;
    for ( s = 0; s < odfSHCoefficientCount; s++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( s, l, m );
      
      std::cout << R0min + R0max << "To be done...\n";
      
    }

  }
  GKG_CATCH( "gkg::DpiOdf::DpiOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site, "
	     "int32_t numberOfShells, "
	     "int32_t numberOfOrientationsPerShell, "
	     "int32_t maximumSphericalHarmonicOrder, "
	     "float R0min, float R0max )" );

}


gkg::DpiOdf::DpiOdf( const gkg::OrientationSet* orientationSet,
		     const gkg::Volume< float >& t2,
		     const gkg::Volume< float >& dw,
		     const gkg::Vector3d< int32_t >& site,
		     const gkg::Matrix& reconstructionMatrix,
		     float R0min, float R0max )
  : gkg::DpiEap( orientationSet, t2, dw, site, reconstructionMatrix )
{

  try
  {
    
    // Computing ODF
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();
    
    //_values contains SH coefficients of the EAP
    int32_t odfSHCoefficientCount = _values.size();
    
    int32_t s, l, m;
    for ( s = 0; s < odfSHCoefficientCount; s++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( s, l, m );
      
      std::cout << R0min + R0max << "To be done...\n";
      
    }


  }
  GKG_CATCH( "gkg::DpiOdf::DpiOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "const gkg::Matrix& reconstructionMatrix )"
	     "float R0min, float R0max )" );

}


gkg::DpiOdf::DpiOdf( const gkg::OrientationSet* orientationSet,
		     const gkg::Volume< float >& t2,
		     const gkg::Volume< float >& dw,
		     const gkg::Vector3d< float >& site,
		     const gkg::Matrix& reconstructionMatrix,
		     float R0min, float R0max )
  : gkg::DpiEap( orientationSet, t2, dw, site, reconstructionMatrix )
{

  try
  {
    
    // Computing ODF
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();
    
    //_values contains SH coefficients of the EAP
    int32_t odfSHCoefficientCount = _values.size();
    
    int32_t s, l, m;
    for ( s = 0; s < odfSHCoefficientCount; s++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( s, l, m );
      
      std::cout << R0min + R0max << "To be done...\n";
      
    }


  }
  GKG_CATCH( "gkg::DpiOdf::DpiOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site, "
             "const gkg::Matrix& reconstructionMatrix)"
	     "float R0min, float R0max )" );

}


gkg::DpiOdf::~DpiOdf()
{
}


const gkg::Vector& gkg::DpiOdf::getOdfSphericalHarmonicsCoefficients() const
{

  try
  {

    return _odfSphericalHarmonicsCoefficients;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::DpiOdf::getOdfSphericalHarmonicsCoefficients() const" );

}


