#include <gkg-simulation-mri-sequence/MRISequencePool.h>
#include <gkg-simulation-mri-sequence/MRISequenceFactory.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-exception/Exception.h>


gkg::MRISequencePool::MRISequencePool( const gkg::Dictionary& dictionary,
                                       float timeStepInUs,
                                       bool verbose,
                                       std::ofstream* osLog )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting field of view
    ////////////////////////////////////////////////////////////////////////////

    int32_t mriSequenceCount = ( int32_t )dictionary.size();
    _mriSequences.resize( mriSequenceCount );
    _mriSequenceNames.resize( mriSequenceCount );

    int32_t mriSequenceIndex = 0;

    gkg::Dictionary::const_iterator 
      s = dictionary.begin(),
      se = dictionary.end();
    while ( s != se )
    {

      _mriSequenceNames[ mriSequenceIndex ] = s->first;
      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << std::endl
                 << "scanning sequence '"
                 << _mriSequenceNames[ mriSequenceIndex ]
                 << "'"
                 << std::endl;
          *osLog << "---------------------------------------"
                 << std::endl;

        }
        else
        {

          std::cout << std::endl
                    << "scanning sequence '"
                    << _mriSequenceNames[ mriSequenceIndex ]
                    << "'"
                    << std::endl;
          std::cout << "---------------------------------------"
                    << std::endl;

        }

      }
      gkg::Dictionary 
        mriSequenceDictionary = s->second->getValue< gkg::Dictionary >();

      _mriSequences[ mriSequenceIndex ].reset(
        gkg::MRISequenceFactory::getInstance().create( mriSequenceDictionary,
                                                       timeStepInUs,
                                                       verbose,
                                                       osLog ) );
      _mriSequences[ mriSequenceIndex ]->computePhaseShifts();
      if ( verbose )
      {

        _mriSequences[ mriSequenceIndex ]->displayInformation( osLog );

      }
      ++ mriSequenceIndex;

      ++ s;

    } 


  }
  GKG_CATCH( "gkg::MRISequencePool::MRISequencePool( "
             "const gkg::Dictionary& dictionary, "
             "float timeStepInUs, "
             "bool verbose, "
             "std::ofstream* osLog )" );



}


gkg::MRISequencePool::~MRISequencePool()
{
}


int32_t gkg::MRISequencePool::getCount() const
{

  try
  {

    return ( int32_t )_mriSequences.size();

  }
  GKG_CATCH( "int32_t gkg::MRISequencePool::getCount() const" );

}


const gkg::RCPointer< gkg::MRISequence >& 
gkg::MRISequencePool::getMRISequence( int32_t index ) const
{

  try
  {

    return _mriSequences[ index ];

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::MRISequence >& "
             "gkg::MRISequencePool::getMRISequence( int32_t index ) const" );

}


const std::string& 
gkg::MRISequencePool::getMRISequenceName( int32_t index ) const
{

  try
  {

    return _mriSequenceNames[ index ];

  }
  GKG_CATCH( "const std::string& "
             "gkg::MRISequencePool::getMRISequenceName( "
             "int32_t index ) const" );

}


void gkg::MRISequencePool::displayInformation( std::ofstream* osLog ) const
{

  try
  {

    std::vector< gkg::RCPointer< gkg::MRISequence > >::const_iterator
      s = _mriSequences.begin(),
      se = _mriSequences.end();
    while ( s != se )
    {

      ( *s )->displayInformation( osLog );
      ++ s;

    }

  }
  GKG_CATCH( "void gkg::MRISequencePool::displayInformation( "
             "std::ofstream* osLog ) const" );

}


void gkg::MRISequencePool::saveNormalizedGradientProfiles(
                                     const std::string& directoryName ) const
{

  try
  {

    gkg::Directory directory( directoryName );

    if ( !directory.isValid() )
    {

      directory.mkdir();

    }

    std::vector< gkg::RCPointer< gkg::MRISequence > >::const_iterator
      s = _mriSequences.begin(),
      se = _mriSequences.end();
    std::vector< std::string >::const_iterator
      n = _mriSequenceNames.begin();
    while ( s != se )
    {

      ( *s )->saveNormalizedProfile( directory.getPath() +
                                     gkg::getDirectorySeparator() + 
                                     *n + ".py" );
      ( *s )->saveNormalizedIntegralProfile(
                                     directory.getPath() +
                                     gkg::getDirectorySeparator() + 
                                     *n + "-integral.py" );
      ( *s )->saveNormalizedDiffusionGradientProfile( directory.getPath() +
                                     gkg::getDirectorySeparator() + 
                                     *n + "-bruker-gp.txt",
                                     true );
      ( *s )->saveNormalizedDiffusionGradientProfile( directory.getPath() +
                                     gkg::getDirectorySeparator() + 
                                     *n + ".txt",
                                     false );
      ++ s;
      ++ n;

    }

  }
  GKG_CATCH( "void gkg::MRISequencePool::saveNormalizedGradientProfiles( "
             "const std::string& directoryName ) const" );

}
