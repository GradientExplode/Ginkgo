#include <gkg-processing-plugin-functors/Brownian/BrownianCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector2d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <list>
#include <cmath>


#define MOTION_RATIO 0.02
#define RADIUS 6
#define SIGMA 10
#define PARTICULE_COUNT 100
#define ITERATION_COUNT 300
#define MATRIX_SIZE 256


class Particule
{

  public:

    Particule( gkg::NumericalAnalysisImplementationFactory* factory,
               gkg::RandomGenerator* randomGenerator,
               const gkg::Vector2d< float >& fov );
    virtual ~Particule();

    void move();
    void draw( gkg::Volume< uint8_t >& motion ) const;

  protected:

    gkg::NumericalAnalysisImplementationFactory* _factory;
    gkg::RandomGenerator* _randomGenerator;
    gkg::Vector2d< float > _fov;
    std::list< gkg::Vector2d< float > > _positions;

};


Particule::Particule( gkg::NumericalAnalysisImplementationFactory* factory,
                      gkg::RandomGenerator* randomGenerator,
                      const gkg::Vector2d< float >& fov )
          : _factory( factory ),
            _randomGenerator( randomGenerator ),
            _fov( fov )
{

  _positions.push_back( gkg::Vector2d< float >(
    _factory->getUniformRandomNumber( *_randomGenerator, 0, _fov.x ),
    _factory->getUniformRandomNumber( *_randomGenerator, 0, _fov.y ) ) );

}
         

Particule::~Particule()
{
}


void Particule::move()
{

  gkg::Vector2d< float > lastPosition;

  if ( _positions.empty() )
  {

    lastPosition.x = 0;
    lastPosition.y = 0;

  }
  else
  {

    lastPosition = _positions.back();

  }

  gkg::Vector2d< float > deltaPosition( 
      _factory->getUniformRandomNumber( *_randomGenerator,
                                         -_fov.x * MOTION_RATIO,
                                         +_fov.x * MOTION_RATIO ),
      _factory->getUniformRandomNumber( *_randomGenerator,
                                         -_fov.y * MOTION_RATIO,
                                         +_fov.y * MOTION_RATIO ) );


  gkg::Vector2d< float > newPosition( lastPosition );
  newPosition += deltaPosition;

  _positions.push_back( newPosition );

}                 


void Particule::draw( gkg::Volume< uint8_t >& motion ) const
{

  std::list< gkg::Vector2d< float > >::const_iterator
    p = _positions.begin(), pe = _positions.end();

  int32_t count = 0;
  double distance = 0.0;
  int32_t dx, dy;
  while ( p != pe )
  {

    int32_t positionX = ( int32_t )p->x;
    int32_t positionY = ( int32_t )p->y;

    for ( dx = -RADIUS; dx <= RADIUS; dx++ )
    {

      for ( dy = -RADIUS; dy <= RADIUS; dy ++ )
      {

        distance = std::sqrt( ( double )dx * ( double )dx + 
                              ( double )dy * ( double )dy );
        if ( distance <= RADIUS )
        {

          if ( ( positionX + dx <= ( int32_t )motion.getSizeX() - 1 ) &&
               ( positionY + dy <= ( int32_t )motion.getSizeY() - 1 ) &&
               ( positionX + dx >= 0 ) &&
               ( positionY + dy >= 0 ) )
          {

            motion( positionX + dx, positionY + dy, count ) =
              ( uint8_t )( 100.0 * std::exp( - ( distance * distance ) /
                    ( SIGMA * SIGMA ) ) );

          }

        }

      }

    }
    ++ count;
    ++ p;

  }

}


//
// class BrownianCommand
//


gkg::BrownianCommand::BrownianCommand( int32_t argc,
                                       char* argv[],
                                       bool loadPlugin,
                                       bool removeFirst )
                    : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::BrownianCommand::BrownianCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::BrownianCommand::BrownianCommand( bool verbose )
{

  try
  {

    execute( verbose );

  }
  GKG_CATCH( "gkg::BrownianCommand::BrownianCommand( bool verbose )" );

}


gkg::BrownianCommand::BrownianCommand( const gkg::Dictionary& parameters )
                    : gkg::Command( parameters )
{

  try
  {

    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( verbose );

  }
  GKG_CATCH( "gkg::BrownianCommand::BrownianCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::BrownianCommand::~BrownianCommand()
{
}


std::string gkg::BrownianCommand::getStaticName()
{

  try
  {

    return "Brownian";

  }
  GKG_CATCH( "std::string gkg::BrownianCommand::getStaticName()" );

}


void gkg::BrownianCommand::parse()
{

  try
  {

    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Brownian motion simulator",
                                  _loadPlugin );

    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::BrownianCommand::parse()" );

}


void gkg::BrownianCommand::execute( bool /* verbose */ )
{

  try
  {

    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );
    std::vector< Particule* > particules( PARTICULE_COUNT );
    gkg::Vector2d< float > fov( MATRIX_SIZE - 1, MATRIX_SIZE - 1 );
    int32_t p;

    for ( p = 0; p < PARTICULE_COUNT; p++ )
    {

      particules[ p ] = new Particule( factory,
                                       &randomGenerator,
                                       fov );

    }

    int32_t s;
    for ( s = 0; s < ITERATION_COUNT - 1; s++ )
    {

      for ( p = 0; p < PARTICULE_COUNT; p++ )
      {

        particules[ p ]->move();

      }

    }

    gkg::Volume< uint8_t > motion( MATRIX_SIZE,
                                   MATRIX_SIZE,
                                   ITERATION_COUNT );
    motion.fill( 0 );

    for ( p = 0; p < PARTICULE_COUNT; p++ )
    {

      particules[ p ]->draw( motion );

    }

    gkg::Writer::getInstance().write( "motion.ima", motion, false );

    for ( p = 0; p < PARTICULE_COUNT; p++ )
    {

      delete particules[ p ];

    }

  }
  GKG_CATCH( "void gkg::BrownianCommand::execute( bool verbose )" );

}


RegisterCommandCreator( BrownianCommand,
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
