#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-simulation-virtual-tissue/PopulationFactory.h>
#include <gkg-simulation-distribution/DistributionFactory.h>
#include <gkg-simulation-kokkos/AtomLut_i.h>
#include <gkg-simulation-kokkos/AtomOverlapSolver_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-algobase/StdDevFilter_i.h>
#include <gkg-communication-sysinfo/Timer.h>
#include <gkg-core-exception/Exception.h>
#include <utility>
#include <parallel/algorithm>
#include <limits>
#include <fstream>


gkg::VirtualTissue::VirtualTissue()
                   : gkg::HeaderedObject()
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // adding a generic syntax set and attributes to header
    ////////////////////////////////////////////////////////////////////////////

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "field_of_view" ] = 
      gkg::Semantic( gkg::TypeOf< std::vector< float > >::getName(), true );
    _syntaxSet[ "__generic__" ][ "population_count" ] = 
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
    _syntaxSet[ "__generic__" ][ "population_types" ] = 
      gkg::Semantic( gkg::TypeOf< std::vector< std::string > >::getName(), true );
    _syntaxSet[ "__generic__" ][ "population_labels" ] = 
      gkg::Semantic( gkg::TypeOf< std::vector< std::string > >::getName(), true );
    _syntaxSet[ "__generic__" ][ "population_cell_counts" ] = 
      gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName(), true );
    _syntaxSet[ "__generic__" ][ "global_atom_count" ] = 
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
    _syntaxSet[ "__generic__" ][ "global_computation_time_in_milliseconds" ] = 
      gkg::Semantic( gkg::TypeOf< double >::getName(), true );
    _syntaxSet[ "__generic__" ][ "remove_overlaps_time_in_milliseconds" ] = 
      gkg::Semantic( gkg::TypeOf< double >::getName(), true );
    _syntaxSet[ "__generic__" ][ "regularization_time_in_milliseconds" ] = 
      gkg::Semantic( gkg::TypeOf< double >::getName(), true );
    _syntaxSet[ "__generic__" ][ "adding_details_time_in_milliseconds" ] = 
      gkg::Semantic( gkg::TypeOf< double >::getName(), true );
    _syntaxSet[ "__generic__" ][ "maximum_atom_count" ] = 
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
    _syntaxSet[ "__generic__" ][ "maximum_atom_count_exceeded" ] = 
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );

    ////////////////////////////////////////////////////////////////////////////
    // adding attributes to header
    ////////////////////////////////////////////////////////////////////////////

    _header.addAttribute( "object_type", std::string( "MedusaGeometry" ) );
    _header.addAttribute( "field_of_view", std::vector< float >( 6U, 0.0f ) );
    _header.addAttribute( "population_count", ( int32_t )0 );
    _header.addAttribute( "global_atom_count", ( int32_t )0 );
    _header.addAttribute( "global_computation_time_in_milliseconds",
                          ( double )0.0 );
    _header.addAttribute( "remove_overlaps_time_in_milliseconds",
                          ( double )0.0 );
    _header.addAttribute( "regularization_time_in_milliseconds",
                          ( double )0.0 );
    _header.addAttribute( "adding_details_time_in_milliseconds",
                          ( double )0.0 );
    _header.addAttribute( "maximum_atom_count",
                          ( int32_t )std::numeric_limits< int32_t >::max() );
    _header.addAttribute( "maximum_atom_count_exceeded", ( int32_t )0 );


    _fieldOfView.setLowerX( 0.0 );
    _fieldOfView.setUpperX( 0.0 );
    _fieldOfView.setLowerY( 0.0 );
    _fieldOfView.setUpperY( 0.0 );
    _fieldOfView.setLowerZ( 0.0 );
    _fieldOfView.setUpperZ( 0.0 );

    _gridResolution = 1.0f;

  }
  GKG_CATCH( "gkg::VirtualTissue::VirtualTissue()" );

}



gkg::VirtualTissue::VirtualTissue(
            const gkg::Dictionary& geometryDictionary,
            const gkg::RCPointer< gkg::Dictionary >& 
                                             populationTissuePropertyDictionary,
            bool verbose,
            std::ofstream* osLog )
                   : gkg::HeaderedObject()
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // instaciating a timer to measure the computational efficacy of the algo
    ////////////////////////////////////////////////////////////////////////////

    gkg::Timer timer;
    timer.storeTime();

    ////////////////////////////////////////////////////////////////////////////
    // adding a generic syntax set and attributes to header
    ////////////////////////////////////////////////////////////////////////////

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "field_of_view" ] = 
      gkg::Semantic( gkg::TypeOf< std::vector< float > >::getName(), true );
    _syntaxSet[ "__generic__" ][ "population_count" ] = 
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
    _syntaxSet[ "__generic__" ][ "population_types" ] = 
      gkg::Semantic( gkg::TypeOf< std::vector< std::string > >::getName(), true );
    _syntaxSet[ "__generic__" ][ "population_labels" ] = 
      gkg::Semantic( gkg::TypeOf< std::vector< std::string > >::getName(), true );
    _syntaxSet[ "__generic__" ][ "population_cell_counts" ] = 
      gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName(), true );
    _syntaxSet[ "__generic__" ][ "global_atom_count" ] = 
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
    _syntaxSet[ "__generic__" ][ "global_computation_time_in_milliseconds" ] = 
      gkg::Semantic( gkg::TypeOf< double >::getName(), true );
    _syntaxSet[ "__generic__" ][ "remove_overlaps_time_in_milliseconds" ] = 
      gkg::Semantic( gkg::TypeOf< double >::getName(), true );
    _syntaxSet[ "__generic__" ][ "regularization_time_in_milliseconds" ] = 
      gkg::Semantic( gkg::TypeOf< double >::getName(), true );
    _syntaxSet[ "__generic__" ][ "adding_details_time_in_milliseconds" ] = 
      gkg::Semantic( gkg::TypeOf< double >::getName(), true );
    _syntaxSet[ "__generic__" ][ "maximum_atom_count" ] = 
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
    _syntaxSet[ "__generic__" ][ "maximum_atom_count_exceeded" ] = 
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );


    ////////////////////////////////////////////////////////////////////////////
    // collecting field of view
    ////////////////////////////////////////////////////////////////////////////

    gkg::Dictionary::const_iterator
      i = geometryDictionary.find( "field_of_view" );
    if ( i == geometryDictionary.end() )
    {

      throw std::runtime_error( "a 'field_of_view' item is required" );

    }
    gkg::GenericObjectList 
      golFieldOfView = i->second->getValue< gkg::GenericObjectList >();

    if ( golFieldOfView.size() != 6U )
    {

      throw std::runtime_error( "'field_of_view' field must have 6 items" );


    }
    _fieldOfView.setLowerX( golFieldOfView[ 0 ]->getScalar() );
    _fieldOfView.setLowerY( golFieldOfView[ 1 ]->getScalar() );           
    _fieldOfView.setLowerZ( golFieldOfView[ 2 ]->getScalar() );
    _fieldOfView.setUpperX( golFieldOfView[ 3 ]->getScalar() );
    _fieldOfView.setUpperY( golFieldOfView[ 4 ]->getScalar() );
    _fieldOfView.setUpperZ( golFieldOfView[ 5 ]->getScalar() );
    std::vector< float > fov( 6U, 0.0f );
    fov[ 0 ] = _fieldOfView.getLowerX();
    fov[ 1 ] = _fieldOfView.getLowerY();
    fov[ 2 ] = _fieldOfView.getLowerZ();
    fov[ 3 ] = _fieldOfView.getUpperX();
    fov[ 4 ] = _fieldOfView.getUpperY();
    fov[ 5 ] = _fieldOfView.getUpperZ();


    ////////////////////////////////////////////////////////////////////////////
    // collecting grid resolution
    ////////////////////////////////////////////////////////////////////////////

    i = geometryDictionary.find( "grid_resolution" );
    if ( i == geometryDictionary.end() )
    {

      throw std::runtime_error( "a 'grid_resolution' item is required" );

    }
    _gridResolution = ( float )i->second->getScalar();


    ////////////////////////////////////////////////////////////////////////////
    // collecting sphere atom count threshold
    ////////////////////////////////////////////////////////////////////////////
    
    i = geometryDictionary.find( "maximum_atom_count" );
    if ( i == geometryDictionary.end() )

    {

      throw std::runtime_error( "a 'maximum_atom_count' "
                                 " item is required" );

    }
    int32_t maximumAtomCount = ( int32_t )i->second->getScalar();


    ////////////////////////////////////////////////////////////////////////////
    // collecting applyRemoveOverlaps information
    ////////////////////////////////////////////////////////////////////////////

    i = geometryDictionary.find( "apply_remove_overlaps" );
    if ( i == geometryDictionary.end() )
    {

      throw std::runtime_error( "a 'apply_remove_overlaps' item is required" );

    }
    bool applyRemoveOverlaps = 
         ( ( ( int32_t )( i->second->getScalar() + 0.5 ) > 0 ) ? true : false );


    ////////////////////////////////////////////////////////////////////////////
    // collecting applyAtomRegularization information
    ////////////////////////////////////////////////////////////////////////////

    i = geometryDictionary.find( "apply_atom_regularization" );
    if ( i == geometryDictionary.end() )
    {

      throw std::runtime_error( 
                     "a 'apply_atom_regularization' item is required" );

    }
    bool applyAtomRegularization =
         ( ( ( int32_t )( i->second->getScalar() + 0.5 ) > 0 ) ? true : false );


    ////////////////////////////////////////////////////////////////////////////
    // collecting applyAddDetails information
    ////////////////////////////////////////////////////////////////////////////

    i = geometryDictionary.find( "apply_add_details" );
    if ( i == geometryDictionary.end() )
    {

      throw std::runtime_error( "a 'apply_add_details' item is required" );

    }
    bool applyAddDetails =
         ( ( ( int32_t )( i->second->getScalar() + 0.5 ) > 0 ) ? true : false );


    ////////////////////////////////////////////////////////////////////////////
    // collecting maximum iteration count for atom overlap solver
    ////////////////////////////////////////////////////////////////////////////

    i = geometryDictionary.find( "atom_overlap_solver_maximum_iteration_count" );
    int32_t atomSolverMaximumIterationCount = 20;
    if ( i != geometryDictionary.end() )
    {

      atomSolverMaximumIterationCount =
                                    ( int32_t )( i->second->getScalar() + 0.5 );

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting force application ratio (between 0.0 and 1.0)
    ////////////////////////////////////////////////////////////////////////////

    i = geometryDictionary.find( "maximum_force_norm" );
    float maximumForceNorm = 1.0f;
    if ( i != geometryDictionary.end() )
    {

      maximumForceNorm = ( float )i->second->getScalar();

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting repulsion force moving average window size
    ////////////////////////////////////////////////////////////////////////////

    i = geometryDictionary.find( "repulsion_force_moving_average_window_size" );
    if ( i == geometryDictionary.end() )
    {

      throw std::runtime_error( 
            "a 'repulsion_force_moving_average_window_size' item is required" );

    }
    int32_t repulsionForceMovingAverageWindowSize =
                                    ( int32_t )( i->second->getScalar() + 0.5 );


    ////////////////////////////////////////////////////////////////////////////
    // collecting minimum repulsion force attenuation percentage
    ////////////////////////////////////////////////////////////////////////////

    i = geometryDictionary.find(
                             "minimum_repulsion_force_attenuation_percentage" );
    if ( i == geometryDictionary.end() )
    {

      throw std::runtime_error( 
        "a 'minimum_repulsion_force_attenuation_percentage' item is required" );

    }
    float minimumRepulsionForceAttenuationPercentage =
                                                ( float )i->second->getScalar();


    ////////////////////////////////////////////////////////////////////////////
    // collecting repulstion force standard deviation percentage threshold
    ////////////////////////////////////////////////////////////////////////////

    i = geometryDictionary.find(
                                "repulsion_force_stddev_percentage_threshold" );
    if ( i == geometryDictionary.end() )
    {

      throw std::runtime_error( 
        "a 'repulsion_force_stddev_percentage_threshold' item is required" );

    }
    float repulsionForceStddevPercentageThreshold =
                                                ( float )i->second->getScalar();


    ////////////////////////////////////////////////////////////////////////////
    // collecting population(s) and putting atoms in a specific container
    ////////////////////////////////////////////////////////////////////////////

    i = geometryDictionary.find( "populations" );
    if ( i == geometryDictionary.end() )
    {

      throw std::runtime_error( "a 'populations' item is required" );

    }

    _populationTypes.reserve( 100 );
    _populationLabels.reserve( 100 );
    int32_t populationId = 0;

    bool atomCountExceeded = false;
    gkg::Dictionary 
      populationsDictionary = i->second->getValue< gkg::Dictionary >();
    gkg::Dictionary::const_iterator
      pd = populationsDictionary.begin(),
      pde = populationsDictionary.end();
    while ( pd != pde )
    {

      if ( verbose ) 
      {

        if ( osLog )
        {

          *osLog << "  creating population " <<  pd->first << std::endl;

        }
        else
        {

          std::cout << "  creating population " <<  pd->first << std::endl;

        }

      }
      gkg::Dictionary populationDictionary = 
                            pd->second->getValue< gkg::Dictionary >();

      gkg::RCPointer< gkg::Population > population(
                                gkg::PopulationFactory::getInstance().create(
                                  populationId,
                                  this,
                                  populationDictionary,
                                  verbose,
                                  osLog ) );
      _populations.push_back( population );
      _populationTypes.push_back( _populations[ populationId ]->getTypeName() );
      _populationLabels.push_back( pd->first );
      _populationLabelToIds[ pd->first ] = populationId;

      ++ populationId;
      ++ pd;

      //////////////////////////////////////////////////////////////////////////
      // checking if the total amount of sphere atoms in the simulation is less  
      // than the maximum sphere atoms threshold fixed
      // stay under this treshold guarantee us to not be out of memory 
      //////////////////////////////////////////////////////////////////////////

      if ( this->getAtomCount() >= maximumAtomCount )
      {

        atomCountExceeded = true;

      }

    }
    int32_t populationCount = ( int32_t )_populations.size();


    ////////////////////////////////////////////////////////////////////////////
    // if the number of atoms does not exceed the maximum allowed atom count
    // then proceeding to the tissue property map construction for all 
    // populations and all cells, and also proceeding with the end of the 
    // construction of the virtual tissue
    ////////////////////////////////////////////////////////////////////////////
    if ( !atomCountExceeded )
    {

      //////////////////////////////////////////////////////////////////////////
      // manage tissue properties if a dictionary is provided
      //////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "adding tissue properties : "
                 << std::flush;

        }
        else
        {

          std::cout << "adding tissue properties : "
                    << std::flush;

        }

      }

      this->setPopulationTissueProperties( populationTissuePropertyDictionary );

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "done" << std::endl;

        }
        else
        {

          std::cout << "done" << std::endl;

        }

      }


      //////////////////////////////////////////////////////////////////////////
      // add population interaction(s) after creation of all population(s)
      // (for instance, connecting oligodendrocytes to mylinated fibers)
      //////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "adding interactions with other populations : "
                 << std::flush;

        }
        else
        {

          std::cout << "adding interactions with other populations : "
                    << std::flush;

        }

      }

      std::vector< gkg::RCPointer< gkg::Population > >::const_iterator
        p = _populations.begin(),
        pe = _populations.end();
      while ( p != pe )
      {

        ( *p )->addInteractionWithOtherPopulations();
        ++ p;

      }

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "done" << std::endl;

        }
        else
        {

          std::cout << "done" << std::endl;

        }

      }


      //////////////////////////////////////////////////////////////////////////
      // removing overlap(s)
      //////////////////////////////////////////////////////////////////////////


      if ( applyRemoveOverlaps )
      {

        if ( verbose )
        {

          if ( osLog )
          {

            *osLog << "start of atoms overlap removing" << std::endl;

          }
          else
          {

            std::cout << "start of atoms overlap removing" << std::endl;

          }

        }


        timer.storeTime();
        this->removeOverlaps( maximumForceNorm,
                              atomSolverMaximumIterationCount,
                              repulsionForceMovingAverageWindowSize,
                              minimumRepulsionForceAttenuationPercentage,
                              repulsionForceStddevPercentageThreshold,
                              verbose,
                              osLog );
        timer.storeTime();

        if ( verbose )
        {

          if ( osLog )
          {

            *osLog << "end of atoms overlap removing" << std::endl;

          }
          else
          {

            std::cout << "end of atoms overlap removing" << std::endl;

          }

        }

      }
      else
      {


        if ( verbose )
        {

          if ( osLog )
          {

            *osLog << "  - atom count after crowding field of view : "
                   << this->getAtomCount()
                   << " atoms" << std::endl;

          }
          else
          {

            std::cout << "  - atom count after crowding field of view : "
                      << this->getAtomCount()
                      << " atoms" << std::endl;

          }

        }

      }


      //////////////////////////////////////////////////////////////////////////
      // regularizing with further atom(s)
      //////////////////////////////////////////////////////////////////////////

      if ( applyAtomRegularization )
      {

        if ( verbose )
        {

          if ( osLog )
          {

            *osLog << "regularizing with further atom(s) : "
                   << std::flush;

          }
          else
          {

            std::cout << "regularizing with further atom(s) : "
                      << std::flush;

          }

        }

        timer.storeTime();
        this->regularizeWithFurtherAtoms();
        timer.storeTime();

        if ( verbose )
        {

          if ( osLog )
          {

            *osLog << "done" << std::endl;

          }
          else
          {

            std::cout << "done" << std::endl;

          }

        }

      }


      //////////////////////////////////////////////////////////////////////////
      // adding details to cell(s)
      //////////////////////////////////////////////////////////////////////////

      if ( applyAddDetails )
      {

        if ( verbose )
        {

          if ( osLog )
          {

            *osLog << "adding details : " << std::flush;

          }
          else
          {

            std::cout << "adding details : " << std::flush;

          }

        }

        timer.storeTime();
        this->addDetails();
        timer.storeTime();

        if ( verbose )
        {

          if ( osLog )
          {

            *osLog << "done" << std::endl;

          }
          else
          {

            std::cout << "done" << std::endl;

          }

        }

      }


      //////////////////////////////////////////////////////////////////////////
      // adding attributes to header
      //////////////////////////////////////////////////////////////////////////

      std::vector< int32_t > populationCellCounts( populationCount );

      p = _populations.begin();
      populationId = 0;
      while ( p != pe )
      {

        populationCellCounts[ populationId ] =
                                   _populations[ populationId ]->getCellCount();
        ++ populationId;
        ++ p;

      }
      timer.storeTime();


      //////////////////////////////////////////////////////////////////////////
      // collection a synthesis of computation time(s)
      //////////////////////////////////////////////////////////////////////////

      std::list< double > 
        timeLapsInMilliseconds = timer.getTimeLapsInMilliseconds();
      std::list< double >::const_iterator 
        l = timeLapsInMilliseconds.begin(),
        le = timeLapsInMilliseconds.end();
      int32_t index = 0;
      double globalComputationTimeInMilliseconds = 0.0;
      double removeOverlapsTimeInMilliseconds = 0.0;
      double regularizationTimeInMilliseconds = 0.0;
      double addingDetailsTimeInMilliseconds = 0.0;
      while ( l != le )
      {


        globalComputationTimeInMilliseconds += *l;
        if ( index == 1 )
        {

          removeOverlapsTimeInMilliseconds = *l;

        }
        else if ( index == 3 )
        {

          regularizationTimeInMilliseconds = *l;

        }
        else if ( index == 5 )
        {

          addingDetailsTimeInMilliseconds = *l;

        }
        ++ index;
        ++ l;

      }


      //////////////////////////////////////////////////////////////////////////
      // filling header
      //////////////////////////////////////////////////////////////////////////
  
      _header.addAttribute( "object_type", std::string( "MedusaGeometry" ) );
      _header.addAttribute( "population_count", populationCount );        
      _header.addAttribute( "field_of_view", fov );
      _header.addAttribute( "population_types", _populationTypes );
      _header.addAttribute( "population_labels", _populationLabels );
      _header.addAttribute( "population_cell_counts", populationCellCounts );
      _header.addAttribute( "global_atom_count",
                            this->getAtomCount() );
      _header.addAttribute( "global_computation_time_in_milliseconds",
                            globalComputationTimeInMilliseconds );
      _header.addAttribute( "remove_overlaps_time_in_milliseconds",
                            removeOverlapsTimeInMilliseconds );
      _header.addAttribute( "regularization_time_in_milliseconds",
                            regularizationTimeInMilliseconds );
      _header.addAttribute( "adding_details_time_in_milliseconds",
                            addingDetailsTimeInMilliseconds );
      _header.addAttribute( "maximum_atom_count_exceeded", ( int32_t )0 );

    }
    ////////////////////////////////////////////////////////////////////////////
    // else stopping the construction of the virtual tissue and filling the
    // metainformation header with the failure status
    ////////////////////////////////////////////////////////////////////////////
    else
    {

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "atom count exceeded" << std::endl;

        }
        else
        {

          std::cout << "atom count exceeded" << std::endl;

        }

      }
      
      _header.addAttribute( "object_type", std::string( "MedusaGeometry" ) );
      _header.addAttribute( "field_of_view", 
                            std::vector< float >( 6U, 0.0f ) );
      _header.addAttribute( "population_count", ( int32_t )0 );
      _header.addAttribute( "global_atom_count", ( int32_t )0 );
      _header.addAttribute( "global_computation_time_in_milliseconds",
                            ( double )0.0 );
      _header.addAttribute( "remove_overlaps_time_in_milliseconds",
                            ( double )0.0 );
      _header.addAttribute( "regularization_time_in_milliseconds",
                            ( double )0.0 );
      _header.addAttribute( "adding_details_time_in_milliseconds",
                            ( double )0.0 );
      _header.addAttribute( "maximum_atom_count_exceeded", ( int32_t )1 );

    }

  }
  GKG_CATCH( "gkg::VirtualTissue::VirtualTissue( "
             "const gkg::Dictionary& dictionary )" );

}


gkg::VirtualTissue::VirtualTissue( const gkg::VirtualTissue& other )
                   : gkg::HeaderedObject( other ),
                      _fieldOfView( other._fieldOfView ),
                     _gridResolution( other._gridResolution ),
                     _populations( other._populations ),
                     _populationTypes( other._populationTypes ),
                     _populationLabels( other._populationLabels ),
                     _populationLabelToIds( other._populationLabelToIds )
{

  try
  {

    gkg::VirtualTissue::iterator p = _populations.begin(),
                                 pe = _populations.end();
    while ( p != pe )
    {

      ( *p )->setParent( this );
      ++ p;

    }

  }
  GKG_CATCH( "gkg::VirtualTissue::VirtualTissue( "
             "const gkg::VirtualTissue& other )" );

}


gkg::VirtualTissue::~VirtualTissue()
{
}


gkg::VirtualTissue& 
gkg::VirtualTissue::operator=( const gkg::VirtualTissue& other )
{

  try
  {

    _fieldOfView = other._fieldOfView;
    _gridResolution = other._gridResolution;
    _populations = other._populations;
    _populationTypes = other._populationTypes;
    _populationLabels = other._populationLabels;
    _populationLabelToIds = other._populationLabelToIds;

    gkg::VirtualTissue::iterator p = _populations.begin(),
                                 pe = _populations.end();
    while ( p != pe )
    {

      ( *p )->setParent( this );
      ++ p;

    }

    return *this;

  }
  GKG_CATCH( "gkg::VirtualTissue& "
             "gkg::VirtualTissue::operator=( "
             "const gkg::VirtualTissue& other )" );

}


gkg::VirtualTissue::const_iterator gkg::VirtualTissue::begin() const
{

  try
  {

    return _populations.begin();

  }
  GKG_CATCH( "gkg::VirtualTissue::const_iterator "
             "gkg::VirtualTissue::begin() const" );

}


gkg::VirtualTissue::const_iterator gkg::VirtualTissue::end() const
{

  try
  {

    return _populations.end();

  }
  GKG_CATCH( "gkg::VirtualTissue::const_iterator "
             "gkg::VirtualTissue::end() const" );

}


gkg::VirtualTissue::iterator gkg::VirtualTissue::begin()
{

  try
  {

    return _populations.begin();

  }
  GKG_CATCH( "gkg::VirtualTissue::iterator gkg::VirtualTissue::begin()" );

}


gkg::VirtualTissue::iterator gkg::VirtualTissue::end()
{

  try
  {

    return _populations.end();

  }
  GKG_CATCH( "gkg::VirtualTissue::iterator gkg::VirtualTissue::end()" );

}


const gkg::BoundingBox< float >& gkg::VirtualTissue::getFieldOfView() const
{

  try
  {

    return _fieldOfView;

  }
  GKG_CATCH( "const gkg::BoundingBox< float >& "
             "gkg::VirtualTissue::getFieldOfView() const" );


}


float gkg::VirtualTissue::getGridResolution() const
{

  try
  {

    return _gridResolution;

  }
  GKG_CATCH( "float gkg::VirtualTissue::getGridResolution() const" );

}


int32_t gkg::VirtualTissue::getPopulationCount() const
{

  try
  {

    return ( int32_t )_populations.size();

  }
  GKG_CATCH( "int32_t gkg::VirtualTissue::getPopulationCount() const" );

}


int32_t gkg::VirtualTissue::getCellCount() const
{

  try
  {

    int32_t cellCount = 0;
    gkg::VirtualTissue::const_iterator
      p = this->begin(),
      pe = this->end();
    while ( p != pe )
    {

      cellCount += ( *p )->getCellCount();
      ++ p;

    }
    return cellCount;

  }
  GKG_CATCH( "int32_t gkg::VirtualTissue::getCellCount() const" );

}


int32_t gkg::VirtualTissue::getAtomCount() const
{

  try
  {

    int32_t atomCount = 0;
    gkg::VirtualTissue::const_iterator
      p = this->begin(),
      pe = this->end();
    while ( p != pe )
    {

      atomCount += ( *p )->getAtomCount();
      ++ p;

    }
    return atomCount;

  }
  GKG_CATCH( "int32_t gkg::VirtualTissue::getAtomCount() const" );

}


uint64_t gkg::VirtualTissue::getAtomParameterCount() const
{

  try
  {

    uint64_t atomParameterCount = 0U;
    gkg::VirtualTissue::const_iterator
      p = this->begin(),
      pe = this->end();
    while ( p != pe )
    {

      atomParameterCount += ( *p )->getAtomParameterCount();
      ++ p;

    }
    return atomParameterCount;

  }
  GKG_CATCH( "uint64_t gkg::VirtualTissue::getAtomParameterCount() const" );

}


const std::vector< gkg::RCPointer< gkg::Population > >& 
gkg::VirtualTissue::getPopulations() const
{

  try
  {

    return _populations;

  }
  GKG_CATCH( "const std::vector< gkg::RCPointer< gkg::Population > >& "
             "gkg::VirtualTissue::getPopulations() const" );

}


const gkg::RCPointer< gkg::Population >& 
gkg::VirtualTissue::getPopulation( int32_t populationId ) const
{

  try
  {

    return _populations[ populationId ];

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::Population >& "
             "gkg::VirtualTissue::getPopulation( int32_t populationId ) const" );

}


const std::string& 
gkg::VirtualTissue::getPopulationLabelFromId( int32_t populationId ) const
{

  try
  {

    std::string populationName = "";

    if ( ( populationId < 0 ) ||
         ( populationId >= ( int32_t )_populations.size() ) )
    {

      throw std::runtime_error( "populationId out of range" );

    }

    return _populationLabels[ populationId ];

  }
  GKG_CATCH( "const std::string& "
             "gkg::VirtualTissue::getPopulationLabelFromId( "
             "int32_t populationId ) const" );

}


int32_t gkg::VirtualTissue::getPopulationIdFromLabel( 
                                const std::string& populationLabel ) const
{

  try
  {

    int32_t populationId = -1;

    std::map< std::string, int32_t >::const_iterator
      i = _populationLabelToIds.find( populationLabel );

    if ( i != _populationLabelToIds.end() )
    {

      populationId = i->second;

    }
    return populationId;

  }
  GKG_CATCH( "int32_t gkg::VirtualTissue::getPopulationIdFromLabel( "
             "const std::string& populationLabel ) const" );

}


void gkg::VirtualTissue::setPopulationTissueProperties(
   const gkg::RCPointer< gkg::Dictionary >& populationTissuePropertyDictionary )
{

  try
  {

    // clear the protected member in case it was previously filled
    _populationAndCellBasedTissueProperties.clear();


    if ( !populationTissuePropertyDictionary.isNull() )
    {

      gkg::Dictionary::const_iterator
        t = populationTissuePropertyDictionary->find( "populations" );

      gkg::Dictionary 
        populationBasedTissuePropertyDictionary =
                                     t->second->getValue< gkg::Dictionary >();

      // sanity checks for population size(s)
      if ( populationBasedTissuePropertyDictionary.size() !=
           _populationLabelToIds.size() )
      {

        throw std::runtime_error(
                     "inconsistent population count beetween population-based "
                     "tissue property and geometry dictionaries" );

      }

      // further sanity check: checking that population labels match
      std::map< std::string, int32_t >::const_iterator
        l = _populationLabelToIds.begin(),
        le = _populationLabelToIds.end();
      while ( l != le )
      {

        if ( populationBasedTissuePropertyDictionary.find( l->first ) ==
             populationBasedTissuePropertyDictionary.end() )
        {

          throw std::runtime_error(
                  "inconsistent population labels beetween population-based "
                  "tissue property and geometry dictionaries" );

        }
        ++ l;

      }

      // looping over populations to store their specific tissue property
      // distributions
      int32_t populationCount = this->getPopulationCount();
      std::vector< std::map< std::string,
                             gkg::RCPointer< gkg::ParameterDistribution > > >
        populationBasedTissuePropertyDistributions( populationCount );

      int32_t populationId = 0;
      for ( populationId = 0; populationId < populationCount; populationId++ )
      {

        gkg::Dictionary tissuePropertyDictionaryForCurrentPopulation =
          gkg::getDictionaryValue( populationBasedTissuePropertyDictionary,
                                   _populationLabels[ populationId ] );
 
        gkg::Dictionary::const_iterator
          tp = tissuePropertyDictionaryForCurrentPopulation.begin(),
          tpe = tissuePropertyDictionaryForCurrentPopulation.end();
        while ( tp != tpe )
        {

          const std::string& tissuePropertyLabel = tp->first;
          gkg::Dictionary tissuePropertyDistributionDictionary =
                                    tp->second->getValue< gkg::Dictionary >();

          gkg::RCPointer< gkg::ParameterDistribution >
            tissuePropertyDistribution(
              gkg::DistributionFactory::getInstance().create(
                                     tissuePropertyDistributionDictionary ) );

          populationBasedTissuePropertyDistributions[ populationId ]
                                                    [ tissuePropertyLabel ] =
            tissuePropertyDistribution;

          ++ tp;

        }

      }


      // now, using the stored tissue property distributions to sample
      // properties for each cell

      _populationAndCellBasedTissueProperties.resize( populationCount );
      for ( populationId = 0; populationId < populationCount; populationId++ )
      {

        int32_t cellCountForPopulationId =
                                 _populations[ populationId ]->getCellCount();
        _populationAndCellBasedTissueProperties[ populationId ].resize(
                                                   cellCountForPopulationId );


        std::map< std::string,
                    gkg::RCPointer< gkg::ParameterDistribution > >::
                                                                const_iterator
          db = populationBasedTissuePropertyDistributions[
                                                       populationId ].begin(),
          de = populationBasedTissuePropertyDistributions[
                                                       populationId ].end();

        int32_t cellId = 0;
        for ( cellId = 0; cellId < cellCountForPopulationId; cellId++ )
        {

          std::map< std::string,
                    gkg::RCPointer< gkg::ParameterDistribution > >::
                                                                const_iterator
            d = db;
          while ( d != de )
          {

            const std::string& tissuePropertyLabel = d->first;
            const gkg::RCPointer< gkg::ParameterDistribution >&
              parameterDistribution = d->second;
            _populationAndCellBasedTissueProperties[ populationId ]
                                                   [ cellId ]
                                                   [ tissuePropertyLabel ] =
              parameterDistribution->getRandomValue();
 
            ++ d;

          }

        }

      }

    }

  }
  GKG_CATCH( "gkg::VirtualTissue::setPopulationAndTissueBasedProperties( "
             "const gkg::RCPointer< gkg::Dictionary >& "
             "populationTissuePropertyDictionary )" );
}


const std::vector< std::vector< std::map< std::string, float > > >&
gkg::VirtualTissue::getTissueProperties() const
{

  try
  {

    return _populationAndCellBasedTissueProperties;

  }
  GKG_CATCH( "const std::vector< std::vector< "
             "std::map< std::string, float > > >& "
             "gkg::VirtualTissue::getTissueProperties() const" );

}



float gkg::VirtualTissue::getTissueProperty(
                                  int32_t populationId,
                                  int32_t cellId,
                                  const std::string& tissuePropertyLabel,
                                  float defaultValue ) const
{

  try
  {

    // in case no tissue property is provided, return a default value
    if ( _populationAndCellBasedTissueProperties.empty() )
    {

      return defaultValue;

    }

    // sanity checks
    if ( ( populationId < 0 ) ||
         ( populationId >= this->getPopulationCount() ) )
    {

      throw std::runtime_error( "populationId out of range" );

    }


    if ( ( cellId < 0 ) ||
         ( cellId >= _populations[ populationId ]->getCellCount() ) )
    {

      throw std::runtime_error( "cellId out of range" );

    }

    const std::map< std::string, float >&
      tissueProperties = _populationAndCellBasedTissueProperties[ populationId ]
                                                                [ cellId ];

    std::map< std::string, float >::const_iterator
      tp = tissueProperties.find( tissuePropertyLabel );
    if ( tp == tissueProperties.end() )
    {

      throw std::runtime_error( std::string( "tissue property '" ) +
                                tissuePropertyLabel + "' not found" );


    }

    return tp->second;

  }
  GKG_CATCH( "float gkg::VirtualTissue::getTissueProperty( "
             "int32_t populationId, "
             "int32_t cellId, "
             "const std::string& tissuePropertyLabel, "
             "float defaultValue ) const" );

}


void gkg::VirtualTissue::getPopulationTissuePropertyLabels(
              std::list< std::string >& populationTissuePropertyLabels ) const
{

  try
  {

    if ( !populationTissuePropertyLabels.empty() )
    {

      populationTissuePropertyLabels.clear();

    }

    if ( !_populationAndCellBasedTissueProperties.empty() )
    {

      std::map< std::string, float >::const_iterator
        tp = _populationAndCellBasedTissueProperties[ 0 ][ 0 ].begin(),
        tpe = _populationAndCellBasedTissueProperties[ 0 ][ 0 ].end();
      while ( tp != tpe )
      {

        populationTissuePropertyLabels.push_back( tp->first );
        ++ tp;

      }

    }

  }
  GKG_CATCH( "void gkg::VirtualTissue::getPopulationTissuePropertyLabels( "
             "std::list< std::string >& "
             "populationTissuePropertyLabels ) const" );

}


void gkg::VirtualTissue::getTissuePropertyOutputPopulationAndCellBasedCSVFile(
const std::string& 
               fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile ) const
{

  try
  {

    //
    // opening output stream
    //

    std::ofstream
      os( fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile.c_str() );

    os << "populationId cellId ";

    //
    // extracting labels
    //

    std::list< std::string > populationTissuePropertyLabels;
    this->getPopulationTissuePropertyLabels( populationTissuePropertyLabels );

    std::list< std::string >::const_iterator
      l = populationTissuePropertyLabels.begin(),
      le = populationTissuePropertyLabels.end();
    while ( l != le )
    {

      os << *l;
      ++ l;
      if ( l != le )
      {

        os << " ";

      }
      else
      {

        os << std::endl;

      }

    }

    //
    // saving tissue properties for each population/cell
    //

    if ( !_populationAndCellBasedTissueProperties.empty() )
    {

      int32_t populationCount = 
                      ( int32_t )_populationAndCellBasedTissueProperties.size();
      int32_t populationId = 0;

      for ( populationId = 0; populationId < populationCount; populationId++ )
      {

        int32_t cellCount = ( int32_t )_populationAndCellBasedTissueProperties[
                                                          populationId ].size();
        int32_t cellId = 0;

        for ( cellId = 0; cellId < cellCount; cellId++ )
        {


          os << populationId << " " << cellId << " ";

          const std::map< std::string, float >&
            tissueProperties =
              _populationAndCellBasedTissueProperties[ populationId ][ cellId ];

          std::map< std::string, float >::const_iterator
            tp = tissueProperties.begin(),
            tpe = tissueProperties.end();
          while ( tp != tpe )
          {

            os << tp->second;
            ++ tp;
            if ( tp != tpe )
            {

              os << " ";

            }
            else
            {

              os << std::endl;

            }

          }

        }

      }

    }

    //
    // closing output stream
    //

    os.close();

  }
  GKG_CATCH( "void gkg::VirtualTissue::"
             "getTissuePropertyOutputPopulationAndCellBasedCSVFile( "
             "const std::string& "
             "fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile ) "
             "const" );

}



gkg::RCPointer< gkg::Volume< gkg::RCPointer< 
                                       std::list< gkg::Vector3d< float > > > > >
gkg::VirtualTissue::getPopulationAtomCenterLut(
                 const std::set< std::string >& populationTypes ) const
{

  try
  {

    // allocating the lookup table
    int32_t populationAtomCenterLutSizeX = 
                   ( int32_t )( _fieldOfView.getSpanX() / _gridResolution ) + 1;
    int32_t populationAtomCenterLutSizeY = 
                   ( int32_t )( _fieldOfView.getSpanY() / _gridResolution ) + 1;
    int32_t populationAtomCenterLutSizeZ = 
                   ( int32_t )( _fieldOfView.getSpanZ() / _gridResolution ) + 1;
    gkg::RCPointer< gkg::Volume<
                       gkg::RCPointer< std::list< gkg::Vector3d< float > > > > >
      populationAtomCenterLut(
         new gkg::Volume< gkg::RCPointer<
                            std::list< gkg::Vector3d< float > > > >(
                                               populationAtomCenterLutSizeX,
                                               populationAtomCenterLutSizeY,
                                               populationAtomCenterLutSizeZ ) );
    // computing the translation to remove FOV offset
    gkg::Vector3d< float > translation( -_fieldOfView.getLowerX(),
                                        -_fieldOfView.getLowerY(),
                                        -_fieldOfView.getLowerZ() );

    // looping over population(s)
    gkg::Vector3d< int32_t > lutVoxel;
    std::vector< gkg::RCPointer< gkg::Population > >::const_iterator
      p = _populations.begin(),
      pe = _populations.end();
    while ( p != pe )
    {

      // if the population is part of the ones concerned
      if ( populationTypes.find( ( *p )->getTypeName() ) !=
           populationTypes.end() )
      {


        std::cout << "population of type " << ( *p )->getTypeName()
                  << std::endl;
        // looping over cell(s)
        std::vector< gkg::RCPointer< gkg::Cell > >::const_iterator
          c = ( *p )->begin(),
          ce = ( *p )->end();
        while ( c != ce )
        {

          // looping over atom(s)
          std::vector< gkg::Atom* >::const_iterator
            a = ( *c )->begin(),
            ae = ( *c )->end();
          while ( a != ae )
          {

            // storing the center of the atom in the LUT
            lutVoxel.x = ( int32_t )( ( ( *a )->getCenter().x + translation.x ) /
                                       _gridResolution );
            lutVoxel.y = ( int32_t )( ( ( *a )->getCenter().y + translation.y ) /
                                       _gridResolution );
            lutVoxel.z = ( int32_t )( ( ( *a )->getCenter().z + translation.z ) /
                                       _gridResolution );
            lutVoxel.x = std::min( std::max( lutVoxel.x, 0 ),
                                   populationAtomCenterLutSizeX - 1 );
            lutVoxel.y = std::min( std::max( lutVoxel.y, 0 ),
                                   populationAtomCenterLutSizeY - 1 );
            lutVoxel.z = std::min( std::max( lutVoxel.z, 0 ),
                                   populationAtomCenterLutSizeZ - 1 );
            gkg::RCPointer< std::list< gkg::Vector3d< float > > >&
              targetList = ( *populationAtomCenterLut )( lutVoxel );
            if ( targetList.isNull() )
            {

              targetList.reset( new  std::list< gkg::Vector3d< float > > );

            }
            targetList->push_back( ( *a )->getCenter() );

            ++ a;

          }
          ++ c;

        }

      }
      ++ p;

    }

    return populationAtomCenterLut;

  }
  GKG_CATCH( "gkg::RCPointer< "
             "gkg::Volume< gkg::RCPointer< "
             "std::list< gkg::Vector3d< float > > > > > "
             "gkg::VirtualTissue::getPopulationAtomCenterLut( "
             "const std::set< std::string >& populationTypes ) const" );

}



gkg::RCPointer< gkg::Volume< gkg::RCPointer< std::list< int32_t > > > >
gkg::VirtualTissue::getPopulationAtomIndexLut(
                                         int32_t& maximumAtomPerLutVoxel ) const
{

  try
  {

    // allocating the lookup table
    int32_t populationAtomCenterLutSizeX = 
                   ( int32_t )( _fieldOfView.getSpanX() / _gridResolution ) + 1;
    int32_t populationAtomCenterLutSizeY = 
                   ( int32_t )( _fieldOfView.getSpanY() / _gridResolution ) + 1;
    int32_t populationAtomCenterLutSizeZ = 
                   ( int32_t )( _fieldOfView.getSpanZ() / _gridResolution ) + 1;
    gkg::RCPointer< gkg::Volume< gkg::RCPointer< std::list< int32_t > > > >
      populationAtomIndexLut(
         new gkg::Volume< gkg::RCPointer< std::list< int32_t > > >(
                                               populationAtomCenterLutSizeX,
                                               populationAtomCenterLutSizeY,
                                               populationAtomCenterLutSizeZ ) );

    // computing the translation to remove FOV offset
    gkg::Vector3d< float > translation( -_fieldOfView.getLowerX(),
                                        -_fieldOfView.getLowerY(),
                                        -_fieldOfView.getLowerZ() );


    // looping over population(s)
    maximumAtomPerLutVoxel = 0;
    int32_t atomIndex = 0;
    gkg::Vector3d< int32_t > lutVoxel;
    std::vector< gkg::RCPointer< gkg::Population > >::const_iterator
      p = _populations.begin(),
      pe = _populations.end();
    while ( p != pe )
    {

      // looping over cell(s)
      std::vector< gkg::RCPointer< gkg::Cell > >::const_iterator
        c = ( *p )->begin(),
        ce = ( *p )->end();
      while ( c != ce )
      {

        // looping over atom(s)
        std::vector< gkg::Atom* >::const_iterator
          a = ( *c )->begin(),
          ae = ( *c )->end();
        while ( a != ae )
        {

          // storing the center of the atom in the LUT
          lutVoxel.x = ( int32_t )( ( ( *a )->getCenter().x + translation.x ) /
                                     _gridResolution );
          lutVoxel.y = ( int32_t )( ( ( *a )->getCenter().y + translation.y ) /
                                     _gridResolution );
          lutVoxel.z = ( int32_t )( ( ( *a )->getCenter().z + translation.z ) /
                                     _gridResolution );
          lutVoxel.x = std::min( std::max( lutVoxel.x, 0 ),
                                 populationAtomCenterLutSizeX - 1 );
          lutVoxel.y = std::min( std::max( lutVoxel.y, 0 ),
                                 populationAtomCenterLutSizeY - 1 );
          lutVoxel.z = std::min( std::max( lutVoxel.z, 0 ),
                                 populationAtomCenterLutSizeZ - 1 );
          gkg::RCPointer< std::list< int32_t > >&
            targetList = ( *populationAtomIndexLut )( lutVoxel );
          if ( targetList.isNull() )
          {

            targetList.reset( new std::list< int32_t > );

          }
          targetList->push_back( atomIndex );
          maximumAtomPerLutVoxel = std::max( ( int32_t )targetList->size(),
                                             maximumAtomPerLutVoxel );

          ++ atomIndex;
          ++ a;

        }
        ++ c;

      }
      ++ p;

    }

    return populationAtomIndexLut;

  }
  GKG_CATCH( "gkg::RCPointer< "
             "gkg::Volume< gkg::RCPointer< "
             "std::list< int32_t > > > > "
             "gkg::VirtualTissue::getPopulationAtomIndexLut( "
             "int32_t& maximumAtomPerLutVoxel ) const" );

}




void gkg::VirtualTissue::getMeshMaps(
             std::vector< gkg::MeshMap< int32_t, float, 3U > >& meshMaps ) const
{

  try
  {

    
    int32_t meshMapCount = ( int32_t )meshMaps.size();
    std::vector< gkg::MeshAccumulator< int32_t, float, 3U > >
      meshAccumulators( meshMapCount );

    gkg::VirtualTissue::const_iterator
      p = _populations.begin(),
      pe = _populations.end();
    while ( p != pe )
    {

      ( *p )->fillMeshAccumulators( meshAccumulators );
      ++ p;

    }

    int32_t meshMapIndex = 0;
    for ( meshMapIndex = 0; meshMapIndex < meshMapCount; meshMapIndex++ )
    {

      meshMaps[ meshMapIndex ].add( meshAccumulators[ meshMapIndex ] );

    }

  }
  GKG_CATCH( "void gkg::VirtualTissue::getMeshMap( "
             "std::vector< gkg::MeshMap< int32_t, float, 3U > >& meshMaps ) "
             "const" );

}


const std::list< float >& 
gkg::VirtualTissue::getTotalRepulsionForceNorms() const
{

  try
  {
  
    return _totalRepulsionForceNorms;
  
  }
  GKG_CATCH( "const std::list< float >& "
             "gkg::VirtualTissue::getTotalRepulsionForceNorms() const" );
  
}


void gkg::VirtualTissue::removeOverlaps(
                            float maximumForceNorm,
                            int32_t atomSolverMaximumIterationCount,
                            int32_t repulsionForceMovingAverageWindowSize,
                            float minimumRepulsionForceAttenuationPercentage,
                            float repulsionForceStddevPercentageThreshold,
                            bool verbose,
                            std::ofstream* osLog )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // computing the actual total number of atoms
    ////////////////////////////////////////////////////////////////////////////

    int32_t atomCount = this->getAtomCount();


    ////////////////////////////////////////////////////////////////////////////
    // filling atom branches of cells and switching to uncompressed 
    // representation for computational efficacy
    ////////////////////////////////////////////////////////////////////////////

    bool hasCompressedRepresentation = false;    
    gkg::VirtualTissue::const_iterator
      p = this->begin(),
      pe = this->end();
    while ( p != pe )
    {

      gkg::Population::const_iterator
        c = ( *p )->begin(),
        ce = ( *p )->end();
      while ( c != ce )
      {

        gkg::Cell::const_iterator
          a = ( *c )->begin(),
          ae = ( *c )->end();
        while ( a != ae )
        {

          hasCompressedRepresentation = ( *a )->hasCompressedRepresentation();
          if ( hasCompressedRepresentation )
          {
          
            ( *a )->switchRepresentation();
          
          }
          ++ a;


        }

        ++ c;

      }

      ++ p;

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing the total atom parameter count
    ////////////////////////////////////////////////////////////////////////////

    uint64_t atomParameterCount = this->getAtomParameterCount();


    ////////////////////////////////////////////////////////////////////////////
    // allocating structure(s) for Kokkos
    ////////////////////////////////////////////////////////////////////////////


    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << "  - allocating structure(s) for Kokkos" << std::endl;

      }
      else
      {

        std::cout << "  - allocating structure(s) for Kokkos" << std::endl;

      }

    }


    // creating views for Kokkos
    UInt8VectorView atomTypes( "atomTypes", atomCount );
    BoolVectorView atomCompressedRepresentations(
                                   "atomCompressedRepresentations", atomCount );
    UInt32VectorView atomBranchLabels( "atomBranchLabels", atomCount );
    FloatVectorView atomParameters( "atomParameters", atomParameterCount );
    UInt64VectorView atomParameterOffsets( "atomParameterOffsets", atomCount );
    Int32VectorView populationIds( "populationIds", atomCount );
    Int32VectorView cellIds( "cellIds", atomCount );
    Float3VectorView forces( "forces", atomCount );

    ////////////////////////////////////////////////////////////////////////////
    // filling Kokkos view(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << "  - filling Kokkos view(s)" << std::endl;

      }
      else
      {

        std::cout << "  - filling Kokkos view(s)" << std::endl;

      }

    }


    int32_t atomIndex = 0;
    uint64_t atomParameterIndex = 0U;
    int32_t populationId = 0;
    int32_t cellId = 0;
    float atomSpanRadius = 0.0f;
    float maximumAtomSpanRadius = 0.0f;
    uint32_t lastBranchLabel = 1U;
    
    p = this->begin();
    while ( p != pe )
    {

      populationId = ( *p )->getId();
      gkg::Population::const_iterator
        c = ( *p )->begin(),
        ce = ( *p )->end();
      while ( c != ce )
      {

        cellId = ( *c )->getId();

        ( *c )->fillAtomBranchLabels( &atomBranchLabels( atomIndex ),
                                      lastBranchLabel );

        gkg::Cell::const_iterator
          a = ( *c )->begin(),
          ae = ( *c )->end();
        while ( a != ae )
        {

          atomTypes( atomIndex ) = ( *a )->getType();
          atomCompressedRepresentations( atomIndex ) =
                                          ( *a )->hasCompressedRepresentation();

          ( *a )->copyParameters( &atomParameters( atomParameterIndex ) );
          atomParameterOffsets( atomIndex ) = atomParameterIndex;
          atomSpanRadius = ( *a )->getSpanRadius();
          populationIds( atomIndex ) = populationId;
          cellIds( atomIndex ) = cellId;

          if ( atomSpanRadius > maximumAtomSpanRadius )
          {

            maximumAtomSpanRadius = atomSpanRadius;

          }

          atomParameterIndex += ( uint64_t )( *a )->getParameterCount();

          ++ atomIndex;
          ++ a;

        }

        ++ c;

      }

      ++ p;

    }

    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << "  - maximumAtomSpanRadius : "
               << maximumAtomSpanRadius << "um"
               << std::endl;
        *osLog << "  - total branch count : "
               << lastBranchLabel - 1U
               << std::endl;

      }
      else
      {

        std::cout << "  - maximumAtomSpanRadius : "
                  << maximumAtomSpanRadius << "um"
                  << std::endl;
        std::cout << "  - total branch count : "
                  << lastBranchLabel - 1U
                  << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // defining the size for the atom index LUT
    ////////////////////////////////////////////////////////////////////////////


    int32_t lutSizeX = ( int32_t )( std::ceil( _fieldOfView.getSpanX() /
                                               _gridResolution + 0.5 ) );
    int32_t lutSizeY = ( int32_t )( std::ceil( _fieldOfView.getSpanY() /
                                                _gridResolution + 0.5 ) );
    int32_t lutSizeZ = ( int32_t )( std::ceil( _fieldOfView.getSpanZ() /
                                               _gridResolution + 0.5 ) );
    int32_t lutSizeXY = lutSizeX * lutSizeY;
    int32_t lutSizeXYZ = lutSizeXY * lutSizeZ;

    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << "  - defining the LUT size : "
               << lutSizeX << " x " << lutSizeY << " x " << lutSizeZ 
               << std::endl;

      }
      else
      {

        std::cout << "  - defining the LUT size : "
                  << lutSizeX << " x " << lutSizeY << " x " << lutSizeZ 
                  << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing the search half neighborhood size for the LUT
    ////////////////////////////////////////////////////////////////////////////


    int32_t neighborhoodHalfSize = 
                  std::max( 1,
                            ( int32_t )( std::ceil( maximumAtomSpanRadius / 
                                                    _gridResolution ) + 0.5 ) );
    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << "  - defining the search half neighborhood size : "
               << neighborhoodHalfSize 
               << std::endl;

      }
      else
      {

        std::cout << "  - defining the search half neighborhood size : "
                  << neighborhoodHalfSize 
                  << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating instances of Kokkos kernel(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << "  - creating instances of Kokkos kernel(s)" << std::endl;

      }
      else
      {

        std::cout << "  - creating instances of Kokkos kernel(s)" << std::endl;

      }

    }

    // allocating a view to store the number of atoms for each voxel
    Int32VectorView atomCountPerGridVoxel( "atomCountPerGridVoxel",
                                          ( int64_t )lutSizeXYZ );

    // allocating a view to store the offset for each voxel
    Int32VectorView atomOffsetPerGridVoxel( "atomOffsetPerGridVoxel",
                                           ( int64_t )lutSizeXYZ );

    // allocating a Kokkos kernel that initializes the number of atoms 
    // for each voxel to zero
    auto atomCountPerGridVoxelInitializer = 
           gkg::AtomCountPerGridVoxelInitializer( atomCountPerGridVoxel );

    // allocating a Kokkos kernel that computes the number of atoms 
    // for all voxels
    auto atomCountPerGridVoxelComputer =
           gkg::AtomCountPerGridVoxelComputer( atomTypes,
                                               atomCompressedRepresentations,
                                               atomParameters,
                                               atomParameterOffsets,
                                               _fieldOfView.getLowerX(),
                                               _fieldOfView.getSpanX(),
                                               _fieldOfView.getLowerY(),
                                               _fieldOfView.getSpanY(),
                                               _fieldOfView.getLowerZ(),
                                               _fieldOfView.getSpanZ(),
                                               lutSizeX,
                                               lutSizeY,
                                               lutSizeZ,
                                               lutSizeXY,
                                               atomCountPerGridVoxel );




    // allocating a Kokkos kernel that computes the total atom count present
    // in  atomCountPerGridVoxel
    auto totalAtomCountComputer =
           gkg::TotalAtomCountComputer( atomCountPerGridVoxel );
    // allocating a Kokkos kernel that computes the maximum number of  
    // atoms for all voxels
    auto maximumAtomCountPerGridVoxelComputer =
           gkg::MaximumAtomCountPerGridVoxelComputer( atomCountPerGridVoxel );
    // allocating a Kokkos kernel to initialize repulsion forces       
    auto repulsionForceInitializer = gkg::RepulsionForceInitializer( forces );

                                                      
    // allocating a Kokkos kernel to compute the norm of all repulsion forces
    auto totalRepulsionForceNormComputer = gkg::TotalRepulsionForceNormComputer(
                                                                       forces );
                                                                       
    // allocating a Kokkos kernel to apply repulsion forces                
    auto repulsionForceApplier = gkg::RepulsionForceApplier(
                                                  atomTypes,
                                                  atomCompressedRepresentations,
                                                  atomParameters,
                                                  atomParameterOffsets,
                                                  forces,
                                                  maximumForceNorm );

    // allocating a Kokkos kernel to regularize the applied forces
    auto atomSmoother = gkg::AtomSmoother( atomCount,
                                           atomTypes,
                                           atomCompressedRepresentations,
                                           atomParameters,
                                           atomParameterOffsets,
                                           atomBranchLabels );


    ////////////////////////////////////////////////////////////////////////////
    // removing overlap(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << "  - removing overlaps of " << atomCount
               << " atoms" << std::endl;

      }
      else
      {

        std::cout << "  - removing overlaps of " << atomCount
                  << " atoms" << std::endl;

      }

    }


    float totalRepulsionForceNorm = std::numeric_limits< float >::max();
    int32_t maximumAtomCountPerGridVoxel = 0;
                            
                            
    float startingTotalRepulsionForceNorm = 0.0;                        
    float currentRepulsionForceStddevPercentage = 100.0;

    std::vector<float>
       repulsionForceNormMovingAverages( repulsionForceMovingAverageWindowSize,
                                         0.0 );
    int32_t movingAverageIndex = 0;
    gkg::StdDevFilter< std::vector< float >, float > stdDevFilter;
    float standardDeviation = 0.0;


    Int32VectorView atomIndexLut( "atomIndexLut", ( int64_t )atomCount );

    int32_t iteration = 0;
    bool movingWindowActive = false;
    while ( ( iteration < atomSolverMaximumIterationCount ) &&
            ( currentRepulsionForceStddevPercentage >= 
              repulsionForceStddevPercentageThreshold ) &&
            ( totalRepulsionForceNorm > 0.0 ) )
    {


      /////////////////////////////////////////////////////////////////////////
      // displaying the current iteration index
      /////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << std::endl << "    iteration " << iteration << std::endl;
          *osLog << "    --------------------------------" << std::endl;

        }
        else
        {

          std::cout << std::endl << "    iteration " << iteration << std::endl;
          std::cout << "    --------------------------------" << std::endl;

        }

      }


      /////////////////////////////////////////////////////////////////////////
      // computing the atom index look-up table
      /////////////////////////////////////////////////////////////////////////

      // initializing the view storing the number of atoms for each 
      // voxel of the LUT table
      Kokkos::parallel_for( range_policy( 0, lutSizeXYZ ),
                            atomCountPerGridVoxelInitializer );
      Kokkos::fence();


      // computing the number of atoms for all voxels
      Kokkos::parallel_for( range_policy( 0, atomCount ),
                            atomCountPerGridVoxelComputer );
      Kokkos::fence();


      // computing the maximum number of atoms for all voxels
      maximumAtomCountPerGridVoxel = 0;
      Kokkos::parallel_reduce( lutSizeXYZ, 
                               maximumAtomCountPerGridVoxelComputer,
                               maximumAtomCountPerGridVoxel );
      Kokkos::fence();

      // computing the atoms offest for all voxels
      int32_t cumulatedAtomCount = 0;
      int32_t lutVoxelIndex = 0;
      for ( lutVoxelIndex = 0; lutVoxelIndex  < lutSizeXYZ; lutVoxelIndex++ )
      {

        const int32_t& atomCountForCurrentGridVoxel = 
          atomCountPerGridVoxel( lutVoxelIndex );
        if ( atomCountForCurrentGridVoxel == 0 )
        {

          atomOffsetPerGridVoxel( lutVoxelIndex ) = -1;

        }
        else
        {

          atomOffsetPerGridVoxel( lutVoxelIndex ) = cumulatedAtomCount;
          cumulatedAtomCount += atomCountForCurrentGridVoxel;

        }

      }

      // re-initializing the view storing the number of atoms 
      // for each voxel        
      Kokkos::parallel_for( range_policy( 0, lutSizeXYZ ),
                            atomCountPerGridVoxelInitializer );
      Kokkos::fence();

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "    - maximum atom count per voxel :  "
                 << maximumAtomCountPerGridVoxel
                 << std::endl;
          *osLog << "    - lut size :  "
                 << lutSizeX << " x "
                 << lutSizeY << " x "
                 << lutSizeZ
                 << std::endl;
          *osLog << "    - lut memory consumption :  "
                 << ( int64_t )atomCount *
                    ( int64_t )4 / 
                    ( 1024.0 * 1024.0 )
                 << " MBytes"
                 << std::endl;

        }
        else
        {

          std::cout << "    - maximum atom count per voxel :  "
                    << maximumAtomCountPerGridVoxel
                    << std::endl;
          std::cout << "    - lut size :  "
                    << lutSizeX << " x "
                    << lutSizeY << " x "
                    << lutSizeZ
                    << std::endl;
          std::cout << "    - lut memory consumption :  "
                    << ( int64_t )atomCount *
                       ( int64_t )4 /
                       ( 1024.0 * 1024.0 )
                    << " MBytes"
                    << std::endl;

        }

      }

      
      // filling the look-up table with atom indices
      auto atomIndexLutComputer =
             gkg::AtomIndexLutComputer( atomTypes,
                                        atomCompressedRepresentations,
                                        atomParameters,
                                        atomParameterOffsets,
                                        _fieldOfView.getLowerX(),
                                        _fieldOfView.getSpanX(),
                                        _fieldOfView.getLowerY(),
                                        _fieldOfView.getSpanY(),
                                        _fieldOfView.getLowerZ(),
                                        _fieldOfView.getSpanZ(),
                                        lutSizeX,
                                        lutSizeY,
                                        lutSizeZ,
                                        lutSizeXY,
                                        lutSizeXYZ,
                                        atomCountPerGridVoxel,
                                        atomOffsetPerGridVoxel,
                                        atomIndexLut );
      Kokkos::parallel_for( range_policy( 0, atomCount ),
                            atomIndexLutComputer );
      Kokkos::fence();

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "    - atom index LUT computed " << std::endl;

        }
        else
        {

          std::cout << "    - atom index LUT computed " << std::endl;

        }

      }


      /////////////////////////////////////////////////////////////////////////
      // computing repulsion forces
      /////////////////////////////////////////////////////////////////////////

      // re-initializing forces to zero
      Kokkos::parallel_for( range_policy( 0, atomCount ),
                            repulsionForceInitializer );
      Kokkos::fence();
      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "    - forces initialized " << std::endl;

        }
        else
        {

          std::cout << "    - forces initialized " << std::endl;

        }

      }


      // allocating a Kokkos kernel to compute repulsion forces
      // with the newly computed atom index LUT
      auto repulsionForceComputer = gkg::RepulsionForceComputer(
                                                  atomCount,
                                                  atomTypes,
                                                  atomCompressedRepresentations,
                                                  atomParameters,
                                                  atomParameterOffsets,
                                                  populationIds,
                                                  cellIds,
                                                  _fieldOfView.getLowerX(),
                                                  _fieldOfView.getSpanX(),
                                                  _fieldOfView.getLowerY(),
                                                  _fieldOfView.getSpanY(),
                                                  _fieldOfView.getLowerZ(),
                                                  _fieldOfView.getSpanZ(),
                                                  lutSizeX,
                                                  lutSizeY,
                                                  lutSizeZ,
                                                  lutSizeXY,
                                                  lutSizeXYZ,
                                                  atomCountPerGridVoxel,
                                                  atomOffsetPerGridVoxel,
                                                  atomIndexLut,
                                                  neighborhoodHalfSize,
                                                  forces );

      // computing repulsion forces      
      Kokkos::parallel_for( range_policy( 0, atomCount ),
                            repulsionForceComputer );
      Kokkos::fence();

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "    - forces computed " << std::endl;

        }
        else
        {

          std::cout << "    - forces computed " << std::endl;

        }

      }

      // applying repulsion forces
      Kokkos::parallel_for( range_policy( 0, atomCount ),
                            repulsionForceApplier );
      Kokkos::fence();

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "    - forces applied " << std::endl;

        }
        else
        {

          std::cout << "    - forces applied " << std::endl;

        }

      }

      // computing total repulsion force norm
      totalRepulsionForceNorm = 0.0f;
      Kokkos::parallel_reduce( atomCount, 
                               totalRepulsionForceNormComputer, 
                               totalRepulsionForceNorm );
      Kokkos::fence();

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "    - total repulsion force norm computed : ====> "
                 << totalRepulsionForceNorm << std::endl;
          *osLog << "    - average repulsion force norm : "
                 << totalRepulsionForceNorm / atomCount << std::endl;

        }
        else
        {

          std::cout << "    - total repulsion force norm computed : ====> "
                    << totalRepulsionForceNorm << std::endl;
          std::cout << "    - average repulsion force norm : "
                    << totalRepulsionForceNorm / atomCount << std::endl;

        }

      }

      _totalRepulsionForceNorms.push_back( totalRepulsionForceNorm ); 

      if ( iteration == 0 )
      {
      
        startingTotalRepulsionForceNorm = totalRepulsionForceNorm;
      
      }

      if ( totalRepulsionForceNorm <= 
           ( minimumRepulsionForceAttenuationPercentage *
             startingTotalRepulsionForceNorm / 100.0 ) )
      {
      
        movingWindowActive = true;
      
      }
      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "    - moving average active : "
                 << movingWindowActive << std::endl;

        }
        else
        {

          std::cout << "    - moving average active : "
                    << movingWindowActive << std::endl;

        }

      }


      // storing current norm in moving average window
      if ( movingWindowActive )
      {
      
        repulsionForceNormMovingAverages[ movingAverageIndex %
                                      repulsionForceMovingAverageWindowSize ] =
          totalRepulsionForceNorm;
          
        if ( movingAverageIndex >= 10 )
        {
        
          stdDevFilter.filter( repulsionForceNormMovingAverages,
                               standardDeviation );
          currentRepulsionForceStddevPercentage = 
            standardDeviation * 100.0 /
            startingTotalRepulsionForceNorm;
          if ( verbose )
          {

            if ( osLog )
            {

              *osLog << "    - current repulsion force stddev percentage : "
                     << currentRepulsionForceStddevPercentage << std::endl;

            }
            else
            {

              std::cout << "    - current repulsion force stddev percentage : "
                        << currentRepulsionForceStddevPercentage << std::endl;

            }

          }

        }
      
        ++ movingAverageIndex;
      
      }


      // applying smoothing
      Kokkos::parallel_for( range_policy( 0, atomCount ),
                            atomSmoother );
      Kokkos::fence();

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "    - smoothing applied " << std::endl;

        }
        else
        {

          std::cout << "    - smoothing applied " << std::endl;

        }

      }

      ++ iteration;

    }


    ////////////////////////////////////////////////////////////////////////////
    // filling back the atom(s), population id(s) and cell id(s)
    ////////////////////////////////////////////////////////////////////////////

    atomIndex = 0;
    populationId = 0;
    cellId = 0;
    gkg::VirtualTissue::iterator
      pPrime = this->begin(),
      pPrimeE = this->end(); 
    while ( pPrime != pPrimeE )
    {

      gkg::Population::iterator
        c = ( *pPrime )->begin(),
        ce = ( *pPrime )->end();
      while ( c != ce )
      {

        gkg::Cell::iterator
          a = ( *c )->begin(),
          ae = ( *c )->end();
        while ( a != ae )
        {

          ( *a )->setParameters(
                         &atomParameters( atomParameterOffsets( atomIndex ) ) );
          if ( !( *a )->hasCompressedRepresentation() )
          {
          
            ( *a )->switchRepresentation();
          
          }
          ++ atomIndex;
          ++ a;

        }

        ++ c;

      }

      ++ pPrime;

    }

  }
  GKG_CATCH( "void gkg::VirtualTissue::removeOverlaps( "
             "float maximumForceNorm, "
             "int32_t atomSolverMaximumIterationCount, "
             "int32_t repulsionForceMovingAverageWindowSize, "
             "float minimumRepulsionForceAttenuationPercentage, "
             "float repulsionForceStddevPercentageThreshold )" );

} 


void gkg::VirtualTissue::regularizeWithFurtherAtoms()
{

  try
  {

    gkg::VirtualTissue::iterator
      p = _populations.begin(),
      pe = _populations.end();
    while ( p != pe )
    {

      ( *p )->regularizeWithFurtherAtoms();
      ++ p;

    }

  }
  GKG_CATCH( "void gkg::VirtualTissue::regularizeWithFurtherAtoms()" );

}


void gkg::VirtualTissue::addDetails()
{

  try
  {

    gkg::VirtualTissue::iterator
      p = _populations.begin(),
      pe = _populations.end();
    while ( p != pe )
    {

      ( *p )->addDetails();
      ++ p;

    }

  }
  GKG_CATCH( "void gkg::VirtualTissue::addDetails()" );

}


void gkg::VirtualTissue::addPopulations(
            const gkg::BoundingBox< float >& fieldOfView,
            const std::vector< gkg::RCPointer< gkg::Population > >& populations,
            const std::vector< std::string >& populationTypes,
            const std::vector< std::string >& populationLabels )
{

  try
  {

    int32_t populationCount = ( int32_t )populations.size();
    if ( populationTypes.size() != populations.size() )
    {

      throw std::runtime_error(
                 "population types and populations have inconsistent size(s)" );

    }
    if ( populationLabels.size() != populations.size() )
    {

      throw std::runtime_error(
                "population labels and populations have inconsistent size(s)" );

    }

    _fieldOfView = fieldOfView;
    _populations = populations;
    _populationTypes = populationTypes;
    _populationLabels = populationLabels;

    std::vector< gkg::RCPointer< gkg::Population > >::iterator
      pp = _populations.begin(),
      ppe = _populations.end();
    while ( pp != ppe )
    {

      ( *pp )->setParent( this );
      ++ pp;

    }

    int32_t populationId = 0;
    for ( populationId = 0; populationId < populationCount; populationId++ )
    {

      _populationLabelToIds[ _populationLabels[ populationId ] ] =
        populationId;

    }

    std::vector< float > fov( 6U, 0.0f );
    fov[ 0 ] = _fieldOfView.getLowerX();
    fov[ 1 ] = _fieldOfView.getLowerY();
    fov[ 2 ] = _fieldOfView.getLowerZ();
    fov[ 3 ] = _fieldOfView.getUpperX();
    fov[ 4 ] = _fieldOfView.getUpperY();
    fov[ 5 ] = _fieldOfView.getUpperZ();

    populationId = 0;
    std::vector< std::string > populationCellCounts( populationCount );
    std::vector< gkg::RCPointer< gkg::Population > >::const_iterator
      p = _populations.begin(),
      pe = _populations.end();
    while ( p != pe )
    {

      populationCellCounts[ populationId ] =
                                   _populations[ populationId ]->getCellCount();
      ++ populationId;
      ++ p;

    }

    _header.addAttribute( "population_count", populationCount );        
    _header.addAttribute( "field_of_view", fov );
    _header.addAttribute( "population_types", _populationTypes );
    _header.addAttribute( "population_labels", _populationLabels );
    _header.addAttribute( "population_cell_counts", populationCellCounts );
    _header.addAttribute( "global_atom_count",
                          this->getAtomCount() );

  }
  GKG_CATCH( "void gkg::VirtualTissue::addPopulations( "
             "const gkg::BoundingBox< float >& fieldOfView, "
             "const std::vector< gkg::RCPointer< gkg::Population > >& populations, "
             "const std::vector< std::string >& populationTypes, "
             "const std::vector< std::string >& populationLabels )" );

}


