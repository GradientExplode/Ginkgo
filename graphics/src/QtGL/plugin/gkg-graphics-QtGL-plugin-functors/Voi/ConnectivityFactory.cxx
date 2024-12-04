#include <gkg-graphics-QtGL-plugin-functors/Voi/ConnectivityFactory.h>


gkg::VoiConnectivityFactory::~VoiConnectivityFactory()
{
}


std::list< gkg::Vector3d< int32_t > >
gkg::VoiConnectivityFactory::create( gkg::VoiEvolutionInfo::EvolConnex ec,
				     gkg::VoiLabelStrategy::TypeId id )
{

  switch ( ec )
    {
    
    case gkg::VoiEvolutionInfo::Connex4:
      {

	switch ( id )
	  {
	  
	  case gkg::VoiLabelStrategy::ModeAxial:
	    return gkg::Neighborhood3d::getInstance().getNeighborOffsets(
				      gkg::Neighborhood3d::Neighborhood2d_4xy );
	  case gkg::VoiLabelStrategy::ModeCoronal:
	    return gkg::Neighborhood3d::getInstance().getNeighborOffsets(
				      gkg::Neighborhood3d::Neighborhood2d_4xz );
	  case gkg::VoiLabelStrategy::ModeSagittal:
	    return gkg::Neighborhood3d::getInstance().getNeighborOffsets(
				      gkg::Neighborhood3d::Neighborhood2d_4yz );
	    break;
	  default:
	    return std::list< Vector3d< int32_t > >();
	    break;
	    
	  }

	break;
	
      }
    case gkg::VoiEvolutionInfo::Connex8:
      {
      
	switch ( id )
	  {
	  
	  case gkg::VoiLabelStrategy::ModeAxial:
	    return gkg::Neighborhood3d::getInstance().getNeighborOffsets(
				      gkg::Neighborhood3d::Neighborhood2d_8xy );
	  case gkg::VoiLabelStrategy::ModeCoronal:
	    return gkg::Neighborhood3d::getInstance().getNeighborOffsets(
				      gkg::Neighborhood3d::Neighborhood2d_8xz );
	  case gkg::VoiLabelStrategy::ModeSagittal:
	    return gkg::Neighborhood3d::getInstance().getNeighborOffsets(
				      gkg::Neighborhood3d::Neighborhood2d_8yz );
	    break;
	  default:
	    return std::list< Vector3d< int32_t > >();
	    break;
	    
	  }

	break;
	
      }
    case gkg::VoiEvolutionInfo::Connex6:
      return gkg::Neighborhood3d::getInstance().getNeighborOffsets(
				        gkg::Neighborhood3d::Neighborhood3d_6 );
      break;
    case gkg::VoiEvolutionInfo::Connex18:
      return gkg::Neighborhood3d::getInstance().getNeighborOffsets(
				       gkg::Neighborhood3d::Neighborhood3d_18 );
      break;
    case gkg::VoiEvolutionInfo::Connex26:
      return gkg::Neighborhood3d::getInstance().getNeighborOffsets(
				       gkg::Neighborhood3d::Neighborhood3d_26 );
      break;
    default:
      return std::list< Vector3d< int32_t > >();
      break;
      
    }
    
}
