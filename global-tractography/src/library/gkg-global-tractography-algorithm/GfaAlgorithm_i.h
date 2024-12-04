#ifndef _gkg_dmri_tractography_GfaAlgorithm_i_h_
#define _gkg_dmri_tractography_GfaAlgorithm_i_h_


#include <gkg-dmri-plugin-functors/MpiTest/GfaAlgorithm.h>
#include <gkg-dmri-plugin-functors/MpiTest/ParallelizedAlgorithm_i.h>
#include <gkg-core-exception/Exception.h>


gkg::GfaAlgorithm::GfaAlgorithm( const std::string& )
                           : gkg::ParallelizedAlgorithm()
{
}


gkg::GfaAlgorithm::~GfaAlgorithm()
{
}


std::string gkg::GfaAlgorithm::getStaticName()
{

  return "gfa";

}


bool gkg::GfaAlgorithm::compute() const
{

  try
  {

    return true;

  }
  GKG_CATCH( "" );

}


#endif
