//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#ifndef vpfHeaderWebServerInterProcessLogisticsAlreadyDefined
#define vpfHeaderWebServerInterProcessLogisticsAlreadyDefined

#include "vpfHeader2Math0_General.h"
#include "vpfHeader4SystemFunctionsGlobalObjects.h"

static ProjectInformationInstance projectInfoInstanceWebServerInterProcessLogisticsHeader
(__FILE__, "Web server: inter-process communication/logistics.");

//this class is similar to the controller class but coordinates across different processes,
//rather than across different threads.
//inter-process communication is achieved via pipes.

class PipePrimitive
{
public:
  List<int> pipeEnds; //pipeEnds[0] is the read end; pipeEnds[1] is the write end.
  List<char> lastRead;
  List<char> buffer;
  bool flagReadEndBlocks;
  bool flagWriteEndBlocks;
  bool flagDeallocated;
  std::string name;
  void Release();
  bool CreateMe
  (const std::string& inputPipeName,
   bool readEndBlocks, bool writeEndBlocks,
   bool restartServerOnFail, bool dontCrashOnFail);
  std::string GetLastRead();
  bool CheckConsistency();
  bool SetPipeFlagsIfFailThenCrash(int inputFlags, int whichEnd, bool restartServerOnFail, bool dontCrashOnFail);

  bool SetPipeReadNonBlockingIfFailThenCrash (bool restartServerOnFail, bool dontCrashOnFail);
  bool SetPipeReadBlockingModeIfFailThenCrash(bool restartServerOnFail, bool dontCrashOnFail);

  bool SetPipeWriteBlockingIfFailThenCrash(bool restartServerOnFail, bool dontCrashOnFail);
  bool SetPipeWriteNonBlockingIfFailThenCrash(bool restartServerOnFail, bool dontCrashOnFail);

  bool SetPipeWriteFlagsIfFailThenCrash(int inputFlags, int whichEnd, bool restartServerOnFail, bool dontCrashOnFail);
  bool WriteIfFailThenCrash(const std::string& input, bool restartServerOnFail, bool dontCrashOnFail);
  bool ReadIfFailThenCrash(bool restartServerOnFail, bool dontCrashOnFail);
  bool WriteAfterEmptying(const std::string& input, bool restartServerOnFail, bool dontCrashOnFail);
  bool ReadWithoutEmptying(bool restartServerOnFail, bool dontCrashOnFail);

  std::string ToString() const;
  PipePrimitive();
  ~PipePrimitive();
};

class PauseProcess
{
public:
  static std::string currentProcessName;
  PipePrimitive thePausePipe;
  PipePrimitive mutexPipe;
  std::string name;
  MemorySaving<MutexRecursiveWrapper> mutexForProcessBlocking; //<- to avoid two threads from the same process blocking the process.
  bool flagDeallocated;
  std::string ToString() const;
  void Release();
  bool CreateMe(const std::string& inputName, bool restartServerOnFail, bool dontCrashOnFail);
  bool ResetNoAllocation();

  bool CheckConsistency();
  bool CheckPauseIsRequested(bool restartServerOnFail, bool dontCrashOnFail, bool dontLockImServer);
  void PauseIfRequested(bool restartServerOnFail, bool dontCrashOnFail);
  bool PauseIfRequestedWithTimeOut(bool restartServerOnFail, bool dontCrashOnFail);

  void RequestPausePauseIfLocked(bool restartServerOnFail, bool dontCrashOnFail);

  void ResumePausedProcessesIfAny(bool restartServerOnFail, bool dontCrashOnFail);
  void LoCkMe();
  void UnloCkMe();
  static void Release(int& theDescriptor);
  PauseProcess();
  ~PauseProcess();
};

class Pipe
{
public:
  PipePrimitive thePipe;
  PauseProcess theMutexPipe;
  bool flagDeallocated;
  std::string name;
//  static int ConnectWithTimeoutViaSelect(int theFD, const std::string& input);
  std::string GetLastRead()
  { return this->thePipe.GetLastRead();
  }
  static bool SetPipeNonBlockingIfFailThenCrash(int inputPipe, bool restartServerOnFail, bool dontCrashOnFail);
  static bool SetPipeBlockingModeIfFailThenCrash(int inputPipe, bool restartServerOnFail, bool dontCrashOnFail);
  bool ReadNoLocksIfFailThenCrash(bool restartServerOnFail, bool dontCrashOnFail);
  static void WriteIfFailThenCrash(int theFD, const List<char>& input, bool restartServerOnFail, bool dontCrashOnFail);
  static int ReadNoInterrupts(int theFD);
  void Read(bool restartServerOnFail, bool dontCrashOnFail);
  void ReadWithoutEmptying(bool restartServerOnFail, bool dontCrashOnFail);


  static int WriteNoInterrupts(int theFD, const std::string& input);
  static int WriteWithTimeoutViaSelect
  (int theFD, const std::string& input, int timeOutInSeconds, int maxNumTries = 10, std::stringstream* commentsOnFailure = 0);
  static int ReadWithTimeOutViaSelect
  (int theFD, List<char>& output, int timeOutInSeconds, int maxNumTries = 10, std::stringstream* commentsOnFailure = 0);
  void WriteAfterEmptying(const std::string& toBeSent, bool restartServerOnFail, bool dontCrashOnFail);

  std::string ToString() const;
  void Release();
  bool CheckConsistency();
  bool CreateMe(const std::string& inputPipeName);
  bool ResetNoAllocation();
  ~Pipe();
  Pipe();
};

#endif

