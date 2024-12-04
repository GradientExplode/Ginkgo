################################################################################
# HEADERS
################################################################################

set( HEADERS
gkg-communication-algorithm/AlgorithmConnector.h
gkg-communication-algorithm/AlgorithmConnector_i.h
gkg-communication-algorithm/Algorithm.h
gkg-communication-algorithm/AlgorithmParameter.h
gkg-communication-algorithm/AlgorithmParameter_i.h
gkg-communication-algorithm/Binarizer.h
gkg-communication-algorithm/Binarizer_i.h
gkg-communication-algorithm/TypedAlgorithmParameter.h
gkg-communication-algorithm/TypedAlgorithmParameter_i.h
gkg-communication-algorithm/VectorReader.h
gkg-communication-algorithm/VectorReader_i.h
gkg-communication-algorithm/VectorWriter.h
gkg-communication-algorithm/VectorWriter_i.h
gkg-communication-command/Command.h
gkg-communication-command/CommandFactory.h
gkg-communication-core/CommunicationImplementationFactory.h
gkg-communication-cuda/CudaDevices.h
gkg-communication-cuda/CudaProperties.h
gkg-communication-dispatcher/Dispatcher.h
gkg-communication-dispatcher/IOCallback.h
gkg-communication-dispatcher/IOHandler.h
gkg-communication-getopt/Application.h
gkg-communication-getopt/Option.h
gkg-communication-getopt/OptionParser.h
gkg-communication-getopt/OptionParser_i.h
gkg-communication-getopt/SeriesOption.h
gkg-communication-getopt/SeriesOption_i.h
gkg-communication-getopt/SingleOption.h
gkg-communication-getopt/SingleOption_i.h
gkg-communication-log/HistoryWriter.h
gkg-communication-log/HistoryWriter_i.h
gkg-communication-log/InfoWriter.h
gkg-communication-log/LogWriter.h
gkg-communication-plugin/Plugin.h
gkg-communication-plugin/PluginLoader.h
gkg-communication-plugin/PluginLoaderImplementation.h
gkg-communication-plugin/PluginManager.h
gkg-communication-rpc/RpcCallback.h
gkg-communication-rpc/RpcCommand.h
gkg-communication-rpc/RpcCommandReader.h
gkg-communication-rpc/RpcCommandWriter.h
gkg-communication-rpc/RpcEpilog.h
gkg-communication-rpc/RpcProlog.h
gkg-communication-rpc/RpcReader.h
gkg-communication-rpc/RpcWriter.h
gkg-communication-socket/IOStreamBase.h
gkg-communication-socket/IOStream.h
gkg-communication-socket/IStream.h
gkg-communication-socket/OStream.h
gkg-communication-socket/SocketBuffer.h
gkg-communication-socket/SocketFrame.h
gkg-communication-socket/SocketFrameWriter.h
gkg-communication-socket/SocketHeader.h
gkg-communication-socket/SocketPeer.h
gkg-communication-socket/SocketReader.h
gkg-communication-socket/SocketRegistry.h
gkg-communication-socket/SocketService.h
gkg-communication-socket/SocketStream.h
gkg-communication-socket/SocketTransferBufferFrame.h
gkg-communication-socket/SocketTransferBufferFrame_i.h
gkg-communication-socket/SocketWriter.h
gkg-communication-sysinfo/ConfigurationInfo.h
gkg-communication-sysinfo/ConfigurationInfoImplementation.h
gkg-communication-sysinfo/Directory.h
gkg-communication-sysinfo/DirectoryImplementation.h
gkg-communication-sysinfo/File.h
gkg-communication-sysinfo/FileFinder.h
gkg-communication-sysinfo/FileImplementation.h
gkg-communication-sysinfo/KeyboardHit.h
gkg-communication-sysinfo/SystemInfo.h
gkg-communication-sysinfo/SystemInfoImplementation.h
gkg-communication-sysinfo/Timer.h
gkg-communication-tcpip/Host.h
gkg-communication-tcpip/HostPort.h
gkg-communication-thread/ExclusiveContext.h
gkg-communication-thread/LoopContext.h
gkg-communication-thread/LoopHelper.h
gkg-communication-thread/LoopTask.h
gkg-communication-thread/LoopThread.h
gkg-communication-thread/Mutex.h
gkg-communication-thread/MutexImplementation.h
gkg-communication-thread/PoolThread.h
gkg-communication-thread/Semaphore.h
gkg-communication-thread/SemaphoreImplementation.h
gkg-communication-thread/Task.h
gkg-communication-thread/ThreadCancel.h
gkg-communication-thread/ThreadCondition.h
gkg-communication-thread/ThreadConditionImplementation.h
gkg-communication-thread/ThreadedLoop.h
gkg-communication-thread/ThreadedLoopHelper.h
gkg-communication-thread/ThreadGauge.h
gkg-communication-thread/Thread.h
gkg-communication-thread/ThreadImplementation.h
gkg-communication-thread/ThreadPool.h
gkg-communication-thread/ThreadPoolHelper.h
gkg-communication-X11/X11CommunicationImplementationFactory.h
gkg-communication-X11/X11ConfigurationInfoImplementation.h
gkg-communication-X11/X11DirectoryImplementation.h
gkg-communication-X11/X11FileImplementation.h
gkg-communication-X11/X11MutexImplementation.h
gkg-communication-X11/X11PluginLoaderImplementation.h
gkg-communication-X11/X11SemaphoreImplementation.h
gkg-communication-X11/X11SystemInfoImplementation.h
gkg-communication-X11/X11ThreadConditionImplementation.h
gkg-communication-X11/X11ThreadImplementation.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
gkg-communication-algorithm/AlgorithmConnector.cxx
gkg-communication-algorithm/Algorithm.cxx
gkg-communication-algorithm/AlgorithmParameter.cxx
gkg-communication-command/Command.cxx
gkg-communication-command/CommandFactory.cxx
gkg-communication-core/CommunicationImplementationFactory.cxx
gkg-communication-cuda/CudaDevices.cxx
gkg-communication-cuda/CudaProperties.cxx
gkg-communication-dispatcher/Dispatcher.cxx
gkg-communication-dispatcher/IOHandler.cxx
gkg-communication-getopt/Application.cxx
gkg-communication-getopt/Option.cxx
gkg-communication-getopt/OptionParser.cxx
gkg-communication-log/HistoryWriter.cxx
gkg-communication-log/InfoWriter.cxx
gkg-communication-log/LogWriter.cxx
gkg-communication-plugin/Plugin.cxx
gkg-communication-plugin/PluginLoader.cxx
gkg-communication-plugin/PluginLoaderImplementation.cxx
gkg-communication-plugin/PluginManager.cxx
gkg-communication-rpc/RpcCallback.cxx
gkg-communication-rpc/RpcCommand.cxx
gkg-communication-rpc/RpcCommandReader.cxx
gkg-communication-rpc/RpcCommandWriter.cxx
gkg-communication-rpc/RpcEpilog.cxx
gkg-communication-rpc/RpcProlog.cxx
gkg-communication-rpc/RpcReader.cxx
gkg-communication-rpc/RpcWriter.cxx
gkg-communication-socket/IOStreamBase.cxx
gkg-communication-socket/IOStream.cxx
gkg-communication-socket/IStream.cxx
gkg-communication-socket/OStream.cxx
gkg-communication-socket/SocketBuffer.cxx
gkg-communication-socket/SocketFrame.cxx
gkg-communication-socket/SocketFrameWriter.cxx
gkg-communication-socket/SocketHeader.cxx
gkg-communication-socket/SocketPeer.cxx
gkg-communication-socket/SocketReader.cxx
gkg-communication-socket/SocketRegistry.cxx
gkg-communication-socket/SocketService.cxx
gkg-communication-socket/SocketStream.cxx
gkg-communication-socket/SocketTransferBufferFrame.cxx
gkg-communication-socket/SocketWriter.cxx
gkg-communication-sysinfo/ConfigurationInfo.cxx
gkg-communication-sysinfo/ConfigurationInfoImplementation.cxx
gkg-communication-sysinfo/Directory.cxx
gkg-communication-sysinfo/DirectoryImplementation.cxx
gkg-communication-sysinfo/File.cxx
gkg-communication-sysinfo/FileFinder.cxx
gkg-communication-sysinfo/FileImplementation.cxx
gkg-communication-sysinfo/KeyboardHit.cxx
gkg-communication-sysinfo/SystemInfo.cxx
gkg-communication-sysinfo/SystemInfoImplementation.cxx
gkg-communication-sysinfo/Timer.cxx
gkg-communication-tcpip/Host.cxx
gkg-communication-tcpip/HostPort.cxx
gkg-communication-thread/ExclusiveContext.cxx
gkg-communication-thread/LoopContext.cxx
gkg-communication-thread/LoopHelper.cxx
gkg-communication-thread/LoopTask.cxx
gkg-communication-thread/LoopThread.cxx
gkg-communication-thread/Mutex.cxx
gkg-communication-thread/MutexImplementation.cxx
gkg-communication-thread/PoolThread.cxx
gkg-communication-thread/Semaphore.cxx
gkg-communication-thread/SemaphoreImplementation.cxx
gkg-communication-thread/Task.cxx
gkg-communication-thread/ThreadCancel.cxx
gkg-communication-thread/ThreadCondition.cxx
gkg-communication-thread/ThreadConditionImplementation.cxx
gkg-communication-thread/Thread.cxx
gkg-communication-thread/ThreadedLoop.cxx
gkg-communication-thread/ThreadedLoopHelper.cxx
gkg-communication-thread/ThreadGauge.cxx
gkg-communication-thread/ThreadImplementation.cxx
gkg-communication-thread/ThreadPool.cxx
gkg-communication-thread/ThreadPoolHelper.cxx
gkg-communication-X11/X11CommunicationImplementationFactory.cxx
gkg-communication-X11/X11ConfigurationInfoImplementation.cxx
gkg-communication-X11/X11DirectoryImplementation.cxx
gkg-communication-X11/X11FileImplementation.cxx
gkg-communication-X11/X11MutexImplementation.cxx
gkg-communication-X11/X11PluginLoaderImplementation.cxx
gkg-communication-X11/X11SemaphoreImplementation.cxx
gkg-communication-X11/X11SystemInfoImplementation.cxx
gkg-communication-X11/X11ThreadConditionImplementation.cxx
gkg-communication-X11/X11ThreadImplementation.cxx
)


################################################################################
# Compilation and link
################################################################################

add_library( gkg-communication SHARED ${SOURCES} )
set_property( TARGET gkg-communication PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )

if ( CUDA_FOUND )

  target_link_libraries( gkg-communication gkg-core ${CUDA_LIBRARIES} )

else ( CUDA_FOUND )

  target_link_libraries( gkg-communication gkg-core pthread dl )

endif ( CUDA_FOUND )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS EXCLUDE gkg-communication-license )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-communication DESTINATION lib COMPONENT runtime )
