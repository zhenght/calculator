//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".

#ifndef vpfHeaderSystemGlobalObjects_already_included
#define vpfHeaderSystemGlobalObjects_already_included
#include <iostream>
#include "vpfHeader1General0_General.h"
#include "vpfHeader3Calculator0_Interface.h"

#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#endif
//#define cgiLimitRAMuseNumPointersInList
#ifndef __DATE__
#define __DATE__ " unknown date "
#endif

static ProjectInformationInstance projectInfoInstanceCalculatorGlobalAndSystemHeader(__FILE__, "Global objects and system calls header");

extern GlobalVariables theGlobalVariables;
extern FormatExpressions consoleFormat;
extern CommandList theParser;
extern timeval ComputationStartGlobal, LastMeasureOfCurrentTime;
extern pthread_t TimerThread;
extern std::string IPAdressCaller;

extern double GetElapsedTimeInSeconds();
extern void CallSystemWrapper(const std::string& theCommand);
extern bool ComputationComplete;
extern void makeReport(IndicatorWindowVariables& input);
extern std::string GetSelectHTMLStringTEmp(List<std::string>& optionsType, List<std::string>& optionsRank, std::string& selectedType, std::string& selectedRank, bool usePreamble);
extern void PredefinedStrings(std::string& inputOutputCivilizedString);
extern void getPath(char* path, std::string& output);
extern void ignoreUserAbortSignal();

extern void InitializeGlobalObjects();

#ifndef WIN32
extern void* RunTimer(void* ptr);
#endif

#endif // vpfHeaderSystemGlobalObjects_already_included
