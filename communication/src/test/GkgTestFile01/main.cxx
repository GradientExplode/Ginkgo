#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <iostream>


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string fileNameIn;


    gkg::Application application( argc, argv,
                                  "File test command" );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.initialize();

    gkg::File file( fileNameIn );

    std::cout << "name : " << file.getName() << std::endl;
    std::cout << "base name : " << file.getBaseName() << std::endl;
    std::cout << "directory name : " << file.getDirectoryName() << std::endl;

    std::cout << "existing : " << file.isExisting() << std::endl;
    std::cout << "regular : " << file.isRegular() << std::endl;
    std::cout << "directory : " << file.isDirectory() << std::endl;
    std::cout << "link : " << file.isLink() << std::endl;
    std::cout << "character device : " << file.isCharacterDevice() << std::endl;
    std::cout << "block device : " << file.isBlockDevice() << std::endl;

    std::cout << "user readable : " << file.isUserReadable() << std::endl;
    std::cout << "user writable : " << file.isUserWritable() << std::endl;
    std::cout << "user executabe : " << file.isUserExecutable() << std::endl;

    std::cout << "group readable : " << file.isGroupReadable() << std::endl;
    std::cout << "group writable : " << file.isGroupWritable() << std::endl;
    std::cout << "group executable : " << file.isGroupExecutable() << std::endl;

    std::cout << "other readable : " << file.isOtherReadable() << std::endl;
    std::cout << "other writable : " << file.isOtherWritable() << std::endl;
    std::cout << "other executable : " << file.isOtherExecutable() << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

