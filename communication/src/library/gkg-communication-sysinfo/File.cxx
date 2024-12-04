#include <gkg-communication-sysinfo/File.h>
#include <gkg-communication-sysinfo/FileImplementation.h>
#include <gkg-communication-core/CommunicationImplementationFactory.h>


gkg::File::File( const std::string& name )
{

  _fileImplementation = gkg::getCommunicationImplementationFactory()
                                        .createFileImplementation( this, name );

}


gkg::File::~File()
{

  delete _fileImplementation;

}


std::string gkg::File::getName() const
{

  return _fileImplementation->getName();

}


std::string gkg::File::getBaseName() const
{

  return _fileImplementation->getBaseName();

}


std::string gkg::File::getDirectoryName() const
{

  return _fileImplementation->getDirectoryName();

}


uint64_t gkg::File::getSize() const
{

  return _fileImplementation->getSize();

}


time_t gkg::File::getLastAccessTime() const
{

  return _fileImplementation->getLastAccessTime();

}


time_t gkg::File::getLastModificationTime() const
{

  return _fileImplementation->getLastModificationTime();

}


time_t gkg::File::getLastStateChangeTime() const
{

  return _fileImplementation->getLastStateChangeTime();

}


bool gkg::File::isExisting() const
{

  return _fileImplementation->isExisting();

}


bool gkg::File::isRegular() const
{

  return _fileImplementation->isRegular();

}


bool gkg::File::isDirectory() const
{

  return _fileImplementation->isDirectory();

}


bool gkg::File::isLink() const
{

  return _fileImplementation->isLink();

}


bool gkg::File::isCharacterDevice() const
{

  return _fileImplementation->isCharacterDevice();

}


bool gkg::File::isBlockDevice() const
{

  return _fileImplementation->isBlockDevice();

}


bool gkg::File::isReadable() const
{

  return _fileImplementation->isReadable();

}


bool gkg::File::isWritable() const
{

  return _fileImplementation->isWritable();

}


bool gkg::File::isExecutable() const
{

  return _fileImplementation->isExecutable();

}


bool gkg::File::isUserReadable() const
{

  return _fileImplementation->isUserReadable();

}


bool gkg::File::isUserWritable() const
{

  return _fileImplementation->isUserWritable();

}


bool gkg::File::isUserExecutable() const
{

  return _fileImplementation->isUserExecutable();

}


bool gkg::File::isGroupReadable() const
{

  return _fileImplementation->isGroupReadable();

}


bool gkg::File::isGroupWritable() const
{

  return _fileImplementation->isGroupWritable();

}


bool gkg::File::isGroupExecutable() const
{

  return _fileImplementation->isGroupExecutable();

}


bool gkg::File::isOtherReadable() const
{

  return _fileImplementation->isOtherReadable();

}


bool gkg::File::isOtherWritable() const
{

  return _fileImplementation->isOtherWritable();

}


bool gkg::File::isOtherExecutable() const
{

  return _fileImplementation->isOtherExecutable();

}


void gkg::File::remove()
{

  _fileImplementation->remove();

}


void gkg::File::update()
{

  _fileImplementation->update();

}


gkg::FileImplementation* gkg::File::getImplementation() const
{

  return _fileImplementation;

}


