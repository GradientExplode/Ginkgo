#include <gkg-dmri-pdf-functor/EapFunctorMeanSquaredDisplacement.h>
#include <gkg-dmri-pdf-functor/EapFunctor_i.h>
#include <gkg-dmri-pdf-functor/EapFunctorFactory_i.h>
#include <gkg-dmri-pdf-dpi/DpiEapCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::EapFunctorMeanSquaredDisplacement::EapFunctorMeanSquaredDisplacement()
                                 : gkg::EapFunctor< gkg::Volume< float >,
                                                    float >()
{
}


gkg::EapFunctorMeanSquaredDisplacement::~EapFunctorMeanSquaredDisplacement()
{
}


std::string gkg::EapFunctorMeanSquaredDisplacement::getName() const
{

  return "mean_squared_displacement";

}


bool gkg::EapFunctorMeanSquaredDisplacement::isStandardBasisRequired() const
{

  return false;

}


void gkg::EapFunctorMeanSquaredDisplacement::process(
                           gkg::Volume< float >& output,
			   EapCartesianField& eapCartesianField,
			   const std::vector< double >& scalarParameters,
			   const std::vector< std::string >& /*stringParameters*/,
			   bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }

    
    gkg::DpiEapCartesianFieldFunctor* dpiEapCartesianFieldFunctor =
      dynamic_cast< gkg::DpiEapCartesianFieldFunctor* >(
						 eapCartesianField.getFunctor() );

    
    if ( dpiEapCartesianFieldFunctor )
    {

      output =
        dpiEapCartesianFieldFunctor->getMeanSquaredDisplacement();      

      float rmax = scalarParameters[ scalarParameters.size() - 1 ];

      // getting site(s)
      const std::list< Vector3d< int32_t > >&
	sites = eapCartesianField.getSiteMap().getSites( 0 );

      // getting offset
      Vector3d< int32_t > 
	offset = eapCartesianField.getOffset();

      std::list< Vector3d< int32_t > >::const_iterator
	s = sites.begin(),
	se = sites.end();
     
      while ( s != se )
      {

	output( *s - offset ) *= rmax*rmax*rmax; 

	++s;

      }

    }


  }
  GKG_CATCH( "void gkg::EapFunctorMeanSquaredDisplacement::process( "
             "gkg::Volume< float >& output, "
             "gkg::EapCartesianField& eapCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// EAP functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_EapFunctorMeanSquaredDisplacement
                                                       __attribute__((unused)) =
gkg::EapFunctorFactory< gkg::Volume< float >, float >::getInstance().
		   registerEapFunctor(
		       gkg::EapFunctorMeanSquaredDisplacement::getInstance() );



