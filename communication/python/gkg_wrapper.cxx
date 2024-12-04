#include <Python.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/SystemInfo.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <string>
#include <list>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>


static PyObject* executeCommand_wrapper( PyObject* self,
                                         PyObject* args )
{

  try
  {

    PyObject *obj = 0;

    // parse arguments
    if ( !PyArg_ParseTuple( args, "O", &obj ) )
    {

      return 0;

    }

    if ( PyDict_Check( obj ) )
    {

      PyObject* key = 0;
      PyObject* value = 0;
      Py_ssize_t pos = 0;

      std::string algorithmName;
      bool verbose = false;
      PyObject* dictionaryValue = 0;

      gkg::Dictionary parameters;

      while ( PyDict_Next( obj, &pos, &key, &value ) )
      {

        std::string theKey;

#if PY_MAJOR_VERSION >= 3
        if ( PyUnicode_Check( key ) )
        {

          theKey = PyUnicode_AsUTF8( key );

        }
#else
        if ( PyBytes_Check( key ) )
        {

          theKey = PyBytes_AsString( key );

        }
#endif
        if ( ( theKey != "algorithm" ) &&
             ( theKey != "parameters" ) &&
             ( theKey != "verbose" ) )
        {

          std::cout << "unknown command argument" << std::endl;

        }
        else
        {

          if ( theKey == "algorithm" )
          {

#if PY_MAJOR_VERSION >= 3
            if ( PyUnicode_Check( value ) )
            {

              algorithmName = std::string( PyUnicode_AsUTF8( value ) );

            }
#else            
            if ( PyBytes_Check( value ) )
            {

              algorithmName = std::string( PyBytes_AsString( value ) );

            }
#endif
            else
            {

              throw std::runtime_error( "algorithm must be a string" );

            }

          }
          else if ( theKey == "verbose" )
          {

            if ( PyBool_Check( value ) )
            {

              verbose = ( value == Py_True ) ? true : false;

            }
            else
            {

              throw std::runtime_error( "verbose must be a boolean" );

            }

          }
          else if ( theKey == "parameters" )
          {

            if ( PyDict_Check( value ) )
            {

              dictionaryValue = value;

            }
            else
            {

              throw std::runtime_error( "parameters must be a dictionary" );

            }

          }

        }

      }

      if ( verbose )
      {

        std::cout << "algorithm : " << algorithmName << std::endl << std::endl;

      }

      PyObject* parameterKey = 0;
      PyObject* parameterValue = 0;
      Py_ssize_t parameterPosition = 0;

      while ( PyDict_Next( dictionaryValue,
                           &parameterPosition,
                           &parameterKey,
                           &parameterValue ) )
      {

#if PY_MAJOR_VERSION >= 3
        if ( !PyUnicode_Check( parameterKey ) )
        {

          throw std::runtime_error( "parameter keys must be strings" );

        }
        std::string parameterName = std::string(
                                             PyUnicode_AsUTF8( parameterKey ) );
#else
        if ( !PyBytes_Check( parameterKey ) )
        {

          throw std::runtime_error( "parameter keys must be strings" );

        }
        std::string parameterName = std::string(
                                             PyBytes_AsString( parameterKey ) );
#endif
#if PY_MAJOR_VERSION >= 3
        if ( PyLong_Check( parameterValue ) )
        {

          int32_t theParameterValue = _PyLong_AsInt( parameterValue );
          parameters[ parameterName ] = theParameterValue;
          if ( verbose )
          {

            std::cout << "  " << parameterName << " -> "
                      << theParameterValue << std::endl;

          }

        }
#else
        if ( PyInt_Check( parameterValue ) )
        {

          int32_t theParameterValue = _PyInt_AsInt( parameterValue );
          parameters[ parameterName ] = theParameterValue;
          if ( verbose )
          {

            std::cout << "  " << parameterName << " -> "
                      << theParameterValue << std::endl;

          }

        }
#endif
        else if ( PyFloat_Check( parameterValue ) )
        {

          double theParameterValue = PyFloat_AsDouble( parameterValue );
          parameters[ parameterName ] = theParameterValue;
          if ( verbose )
          {

            std::cout << "  " << parameterName << " -> "
                      << theParameterValue << std::endl;

          }

        }
        else if ( PyBool_Check( parameterValue ) )
        {

          long theParameterValue = ( parameterValue == Py_True ) ? 1 : 0;
          parameters[ parameterName ] = ( int32_t )theParameterValue;

          if ( verbose )
          {

            std::cout << "  " << parameterName << " -> "
                      << theParameterValue << std::endl;

          }

        }
#if PY_MAJOR_VERSION >= 3
        else if ( PyUnicode_Check( parameterValue ) )
        {

          std::string theParameterValue( PyUnicode_AsUTF8( parameterValue ) );
          parameters[ parameterName ] = theParameterValue;

          if ( verbose )
          {

            std::cout << "  " << parameterName << " -> "
                      << theParameterValue << std::endl;

          }

        }
#else
        else if ( PyBytes_Check( parameterValue ) )
        {

          std::string theParameterValue( PyBytes_AsString( parameterValue ) );
          parameters[ parameterName ] = theParameterValue;

          if ( verbose )
          {

            std::cout << "  " << parameterName << " -> "
                      << theParameterValue << std::endl;

          }

        }
#endif        
        else if ( PyTuple_Check( parameterValue ) )
        {

          if ( verbose )
          {

            std::cout << "  " << parameterName << " -> " << std::flush;

          }
          Py_ssize_t itemCount = PyTuple_Size( parameterValue );

          gkg::GenericObjectList genericObjectList( itemCount );

          Py_ssize_t index = 0U;
          for ( index = 0U; index < itemCount; index++ )
          {

            PyObject* item = PyTuple_GetItem( parameterValue, index );

#if PY_MAJOR_VERSION >= 3
            if ( PyLong_Check( item ) )
            {

              int32_t theItemValue = _PyLong_AsInt( item );
              genericObjectList[ index ] = theItemValue;

              if ( verbose )
              {

                std::cout << theItemValue << " " << std::flush;

              }

            }
#else
            if ( PyInt_Check( item ) )
            {

              int32_t theItemValue = _PyInt_AsInt( item );
              genericObjectList[ index ] = theItemValue;

              if ( verbose )
              {

                std::cout << theItemValue << " " << std::flush;

              }

            }
#endif
            else if ( PyFloat_Check( item ) )
            {

              double theItemValue = PyFloat_AsDouble( item );
              genericObjectList[ index ] = theItemValue;

              if ( verbose )
              {

                std::cout << theItemValue << " " << std::flush;

              }

            }
            else if ( PyBool_Check( item ) )
            {

              long theItemValue = ( item == Py_True ) ? 1 : 0;
              genericObjectList[ index ] = ( int32_t )theItemValue;

              if ( verbose )
              {

                std::cout << theItemValue << " " << std::flush;

              }

            }

#if PY_MAJOR_VERSION >= 3
            else if ( PyUnicode_Check( item ) )
            {

              std::string theItemValue( PyUnicode_AsUTF8( item ) );
              genericObjectList[ index ] = theItemValue;

              if ( verbose )
              {

                std::cout << theItemValue << " " << std::flush;

              }

            }
#else
            else if ( PyBytes_Check( item ) )
            {

              std::string theItemValue( PyBytes_AsString( item ) );
              genericObjectList[ index ] = theItemValue;

              if ( verbose )
              {

                std::cout << theItemValue << " " << std::flush;

              }

            }
#endif
          }
          if ( verbose )
          {

            std::cout << std::endl;

          }

          parameters[ parameterName ] = genericObjectList;

        }

      }

      if ( verbose )
      {

        std::cout << std::endl 
                  << "running " << algorithmName << " ..."
                  << std::endl
                  << std::endl;

      }

      gkg::RCPointer< gkg::Command > command( 
                      gkg::CommandFactory::getInstance().create( algorithmName,
                                                                 parameters ) );

    }

    Py_RETURN_NONE;

  }
  GKG_CATCH( "static PyObject* executeCommand( "
             "PyObject* self, "
             "PyObject* args, "
             "PyObject* keywords )" );

}


static PyMethodDef GkgMethods[] = 
{

  { "executeCommand", executeCommand_wrapper, METH_VARARGS | METH_KEYWORDS,
    "GKG command instanciation" },
  { NULL, NULL, 0, NULL }

};


#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef gkgPyModule = 
{

  PyModuleDef_HEAD_INIT,
  "gkg",
  "Extension module of GKG",
  -1,
  GkgMethods

};


PyMODINIT_FUNC PyInit_gkg()
{

  PyObject* pyObject = PyModule_Create( &gkgPyModule );
  gkg::PluginLoader::getInstance().load( false, true );

  return pyObject;

}

#else

PyMODINIT_FUNC initgkg()
{

  Py_InitModule3( "gkg", GkgMethods, "Extension module of GKG" );
  gkg::PluginLoader::getInstance().load( false, true );

}

#endif
