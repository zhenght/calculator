//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#include <chrono>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
//#define cgiLimitRAMuseNumPointersInList

#include "vpfHeader4SystemFunctionsGlobalObjects.h"
#include "vpfHeader6WebServer.h"
ProjectInformationInstance projectInfoInstanceCalculatorSystem(__FILE__, "System functions, platform dependent code.");


timeval ComputationStartGlobal, LastMeasureOfCurrentTime;
pthread_t TimerThread;

double GetElapsedTimeInSeconds()
{ gettimeofday(&LastMeasureOfCurrentTime, NULL);
  int miliSeconds =(LastMeasureOfCurrentTime.tv_sec- ComputationStartGlobal.tv_sec)*1000+(LastMeasureOfCurrentTime.tv_usec- ComputationStartGlobal.tv_usec)/1000;
  return ((double) miliSeconds)/1000;
}

void InitializeTimer()
{
  gettimeofday(&ComputationStartGlobal, NULL);
}

void CreateTimerThread()
{
  pthread_create(&TimerThread, NULL,*RunTimerVoidPtr, 0);
}

void SleepFunction(int microseconds)
{ usleep(microseconds);
}

struct TimerThreadData{
  double elapsedtime;
  double elapsedComputationTime;
  double computationStartTime;
  int counter=0;
  int microsecondsleep=100000;
  ProgressReportWebServer theReport1, theReport2;
//  ThreadWrapper theThread;
  void Run();
  bool HandleComputationTimer();
  bool HandleComputationCompleteStandard();
  bool HandleTimerSignalToServer();
  bool HandleMaxComputationTime();
  bool HandlePingServerIamAlive();
  bool HandleComputationTimeout();
};

bool TimerThreadData::HandleComputationTimer()
{ if (theGlobalVariables.flagComputationStarted)
    if (this->computationStartTime<0)
      this->computationStartTime=GetElapsedTimeInSeconds();
  this->elapsedtime=GetElapsedTimeInSeconds();
  if (this->computationStartTime>0)
    this->elapsedComputationTime=this->elapsedtime-this->computationStartTime;
  return false;
}

bool TimerThreadData::HandleComputationCompleteStandard()
{ if (theGlobalVariables.flagComputationCompletE)
    theReport1.SetStatus("Starting timer cycle break: computation is complete.");
  return false;
}

bool TimerThreadData::HandleTimerSignalToServer()
{ if (this->counter%20!=0)
    return false;
  std::stringstream reportStream;
  reportStream << "Timer: " << this->elapsedtime << " seconds elapsed since the timer was launched.";
  theReport1.SetStatus(reportStream.str());
  return false;
}

bool TimerThreadData::HandleMaxComputationTime()
{ if (theGlobalVariables.MaxComputationTimeSecondsNonPositiveMeansNoLimit<=0)
    return false;
  if (elapsedComputationTime<=0)
    return false;
  if (elapsedComputationTime<=theGlobalVariables.MaxComputationTimeSecondsNonPositiveMeansNoLimit)
    return false;
  if (theGlobalVariables.flagComputationCompletE)
  { theReport2.SetStatus((std::string)"The allowed clock time has ran out, but it seems the computation is already done. "+
                         (std::string)"Continuing to run (in order to wrap it up)...");
    return false;
  }
  theReport2.SetStatus("Starting timer cycle break: computation time too long.");
  if (!theGlobalVariables.flagComputationStarted)
    crash << "Something has gone wrong. Computation has not started, yet " << elapsedtime
    << " seconds have already passed."
    << " This may be an error in the web-server routines of the calculator!" << crash;
  std::stringstream out;
  out << "<b>This is a safety time-out crash. You may have requested a computation that takes too long."
  << "</b> Your computation ran for ";
  if (elapsedComputationTime>0)
    out << elapsedComputationTime << " seconds";
  else
    out << " (unknown amount of time)";
  out << ". The allowed run time is "
  << theGlobalVariables.MaxComputationTimeSecondsNonPositiveMeansNoLimit
  << " seconds (twice the amount of time allowed for calculator interpretation). "
  << "<br>The present timeout limit may be extended by modifying theGlobalVariables.MaxComputationTimeSecondsNonPositiveMeansNoLimit;"
  << " the restriction can be altogether lifted by modifying source file "
  << __FILE__ << "<br>";
  crash << out.str() << crash;
  return true;
}

bool TimerThreadData::HandleComputationTimeout()
{ if (!theGlobalVariables.flagUsingBuiltInWebServer)
    return false;
  if (theGlobalVariables.MaxComputationTimeBeforeWeTakeAction<=0)
    return false;
  if (elapsedComputationTime<=0)
    return false;
  if (elapsedComputationTime<=theGlobalVariables.MaxComputationTimeBeforeWeTakeAction)
    return false;
//  std::cout << "GOT TO HERE\n";
  if (theGlobalVariables.WebServerReturnDisplayIndicatorCloseConnection==0)
    return false;
//  std::cout << "GOT TO HERE pt 2\n";
  if (theGlobalVariables.flagOutputTimedOut )
    return false;
//  std::cout << "GOT TO HERE pt 3\n";
  theGlobalVariables.flagTimeOutExplanationAlreadyDisplayed=true;
  theReport2.SetStatus("Starting timer cycle displaying time out explanation.");
  theGlobalVariables.WebServerReturnDisplayIndicatorCloseConnection();
  theReport2.SetStatus("Starting timer cycle displaying time out indicator done, continuing timer cycle.");
  return false;
}

bool TimerThreadData::HandlePingServerIamAlive()
{ if (theGlobalVariables.flagComputationFinishedAllOutputSentClosing)
    return true;
  if (theGlobalVariables.WebServerTimerPing==0)
    return false;
  theGlobalVariables.WebServerTimerPing(this->elapsedtime);
  return false;
}

void TimerThreadData::Run()
{ this->elapsedtime=-1;
  this->elapsedComputationTime=-1;
  this->computationStartTime=-1;
  this->counter=0;
  this->microsecondsleep=100000;
//  std::cout << "Got thus far RunTimerVoidPtr - 2" << std::endl;
  for (; ;)
  { this->counter++;
    this->HandleComputationTimer();
    SleepFunction(microsecondsleep);
    this->HandleComputationCompleteStandard();
    this->HandleTimerSignalToServer();
    this->HandleMaxComputationTime();
    this->HandleComputationTimeout();
    if (this->HandlePingServerIamAlive())
      break;
  }
  RegisterFunctionCall(__FILE__, __LINE__, "exiting timer");
  theReport2.SetStatus("Timer thread: finished.");
}


void* RunTimerVoidPtr(void* ptr)
{ MacroRegisterFunctionWithName("RunTimerVoidPtr");
//  std::cout << "Got thus far RunTimerVoidPtr" << std::endl;
  TimerThreadData theThread;
  theThread.Run();


  pthread_exit(NULL);
  return 0;
}

void CallSystemWrapper(const std::string& theCommand)
{ system(theCommand.c_str());
}

void CallChDirWrapper(const std::string& theDir)
{ chdir(theDir.c_str());
}
