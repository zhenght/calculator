//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#include <iostream>
#include "vpfHeader1.h"
#include "vpfHeader2.h"

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
ProjectInformationInstance projectInfoInstanceCalculatorCpp(__FILE__, "Calculator web interface.");

void getPath(char* path, std::string& output)
{ if (path==0) return;
  int length=0;
  output= path;
  while (output[length]!=0 && length<150)
    length++;
  for (int i=length-1; i>=0; i--)
  {
#ifdef WIN32
		if (output[i]=='\\' )
    { output.resize(i+1);
      return;
    }
#else
		if (output[i]=='/')
    { output.resize(i+1);
      return;
    }
#endif
  }
}

void ignoreUserAbortSignal()
{
#ifndef WIN32
  signal(SIGABRT,SIG_IGN);
  signal(SIGTERM,SIG_IGN);
#endif
}

void static_html4(std::stringstream& output);
void static_html3(std::stringstream& output);

bool ComputationComplete;

#ifndef WIN32
timeval ComputationStartGlobal, LastMeasureOfCurrentTime;

GlobalVariables theGlobalVariables;
CommandList theParser;

double GetElapsedTimeInSeconds()
{ gettimeofday(&LastMeasureOfCurrentTime, NULL);
  int miliSeconds =(LastMeasureOfCurrentTime.tv_sec- ComputationStartGlobal.tv_sec)*1000+(LastMeasureOfCurrentTime.tv_usec- ComputationStartGlobal.tv_usec)/1000;
  return ((double) miliSeconds)/1000;
}

void* RunTimer(void* ptr)
{ double elapsedtime=-1;
  for (; ;)
  { usleep(100);
    elapsedtime=GetElapsedTimeInSeconds();
    if (ComputationComplete)
      break;
    if (elapsedtime >=theGlobalVariables.MaxComputationTimeSecondsNonPositiveMeansNoLimit && !
        (theGlobalVariables.MaxComputationTimeSecondsNonPositiveMeansNoLimit<=0))
      break;
  }
  if (!ComputationComplete)
  { std::cout << "</div><br><br><br>Your computation has taken "
    << elapsedtime << " seconds so far.";
    std::cout << "<br>The maximum allowed run time for "
    << " the entire system is  "
    << theGlobalVariables.MaxComputationTimeSecondsNonPositiveMeansNoLimit
    << " seconds (twice the amount of time allowed for calculator interpretation). "
    << "<br>This safety limit is hard coded in this particular server. "
    << "<br>However, if you install the calculator on your own machine you may "
    << "<br>allow arbitrarily large execution time by modifying "
    << " the variable theGlobalVariables.MaxComputationTimeSecondsNonPositiveMeansNoLimit"
    << " located in file " << __FILE__ << "<br><b>Signalling ungraceful exit...</b> ";
    std::cout.flush();
    assert(false);
  } else
    pthread_exit(NULL);
}
#endif

std::string IPAdressCaller;

void makeReport(IndicatorWindowVariables& input)
{ static int counter =-1;
  counter++;
//  if (counter%10!=0)
//    return;
  std::fstream theFile;
  CGI::OpenFileCreateIfNotPresent(theFile, theParser.indicatorFileNamE, false, true, false);
  std::stringstream outStream;
  theFile << " Elapsed calculator time: " << GetElapsedTimeInSeconds() << " second(s).";
  for (int i=0; i<input.ProgressReportStringS.size; i++)
    theFile << "\n" << input.ProgressReportStringS[i] << "\n<br>\n";
  theFile.flush();
  theFile.close();
}

void CallSystemWrapper(const std::string& theCommand)
{ system(theCommand.c_str());
}

std::string GetSelectHTMLStringTEmp
(List<std::string>& optionsType, List<std::string>& optionsRank,
  std::string& selectedType, std::string& selectedRank, bool usePreamble)
{ std::stringstream out;
  out << "\n<script language=\"javascript\">\n"
  << "function updatePreamble(){\n"
  << "  if (! document.getElementById(\"checkUsePreambleID\").checked)\n"
  << "  { document.getElementById(\"textPreamble\").style.display = \"none\";\n"
  << "    document.getElementById(\"textTypeID\").disabled= \"disabled\";\n"
  << "    document.getElementById(\"textDimID\").disabled = \"disabled\";\n"
  << "  }"
  << "  else\n"
  << "  { document.getElementById(\"textPreamble\").innerHTML=\"g:= SemisimpleLieAlgebra{}\"+"
  << " document.getElementById(\"textTypeID\").value+\"_\"+ document.getElementById(\"textDimID\").value+\""
  << "; h_{{i}}:=g_{0, i}; \"; \n"
  << "    document.getElementById(\"textPreamble\").style.display = '';\n"
  << "    document.getElementById(\"textTypeID\").disabled = '';\n"
  << "    document.getElementById(\"textDimID\").disabled = '';\n"
  << "  }\n"
  << "}\n"
  << "\n</script>\n";
  out << "Use predefined preamble";
  out << "<input type=\"checkbox\"";
  if (usePreamble)
    out << " checked=\"checked\"";
  out << " name=\"checkUsePreamble\" id= \"checkUsePreambleID\"onchange=\"updatePreamble();\"></input>";
  out << "  <select name=\"textType\" id=\"textTypeID\" onchange=\"updatePreamble();\">\n";
  for (int i=0; i<optionsType.size; i++)
  { out << "   <option";
    if (selectedType==optionsType[i])
      out << " selected=\"selected\"";
    out << ">" << optionsType[i] << "</option>\n";
  }
  out << "</select>";
  out << "  <select name=\"textDim\" id=\"textDimID\" onchange=\"updatePreamble();\">\n";
  for (int i=0; i<optionsRank.size; i++)
  { out << "   <option";
    if (selectedRank==optionsRank[i])
      out << " selected=\"selected\"";
    out << ">" << optionsRank[i] << "</option>\n";
  }
  out << " </select>\n";
  out << "<span name=\"textPreamble\" id=\"textPreamble\"> </span>";
  return out.str();
}

int main(int argc, char **argv)
{
#ifndef WIN32
  gettimeofday(&ComputationStartGlobal, NULL);
#endif
  theGlobalVariables.SetFeedDataToIndicatorWindowDefault(&makeReport);
  theGlobalVariables.SetTimerFunction(&GetElapsedTimeInSeconds);
  //Change the below line to modify the computation time of the calculator.
  theGlobalVariables.MaxComputationTimeSecondsNonPositiveMeansNoLimit=500;
  theGlobalVariables.SetCallSystem(&CallSystemWrapper);
  theParser.init(theGlobalVariables);
  MacroRegisterFunctionWithName("main");

  theParser.DisplayNameCalculator="/vpf/cgi-bin/calculator";
  ParallelComputing::cgiLimitRAMuseNumPointersInList=60000000;
  std::string inputPatH;
  std::string tempS;
	std::cin >> theParser.inputStringRawestOfTheRaw;
  ComputationComplete=false;
	if (theParser.inputStringRawestOfTheRaw=="")
	{
#ifdef WIN32
    char buffer[2000];
		size_t tempI=1500;
		::getenv_s(&tempI, buffer, 1500, "QUERY_STRING");
		theParser.inputStringRawestOfTheRaw=buffer;
#else
		theParser.inputStringRawestOfTheRaw=getenv("QUERY_STRING");
//		inputString=::getenv("QUERY_STRING");
#endif
    IPAdressCaller=getenv("REMOTE_ADDR");
    for (int i=0; i<MathRoutines::Minimum((int)IPAdressCaller.size(), SomeRandomPrimesSize); i++)
      IPAdressCaller[i]='A'+(IPAdressCaller[i]*SomeRandomPrimes[i])%26;
	}
	if (argc>=1)
    getPath(argv[0], inputPatH);
  theParser.initDefaultFolderAndFileNames(inputPatH, "/vpf/", IPAdressCaller);
  theParser.InitJavaScriptDisplayIndicator();

//	inputString="textInput=+asf&buttonGo=Go";
//  inputString="weylLetterInput=B&weyRankInput=3&textInput=%2B&buttonGo=Go";
	std::cout << "Content-Type: text/html\n\n";
  std::cout << "<html><meta name=\"keywords\" content= \"Root system, Root system Lie algebra, "
  << "Vector partition function calculator, vector partition functions, Semisimple Lie algebras, "
  << "Root subalgebras, sl(2)-triples\"> <head> <title>vpf version  "
  << __DATE__ << ", " << __TIME__ << "</title>";
  //below follows a script for collapsing and expanding menus
//  std::cout << "<script type=\"text/javascript\" src=\"http://ajax.googleapis.com/ajax/libs/dojo/1.6.1/dojo/dojo.xd.js\""
//  << "></script>";
//  << " djConfig = \"parseOnLoad: true\"></script>";

  std::cout << "<script src=\"" << theParser.DisplayPathServerBase
  << "jsmath/easy/load.js\"></script> ";
  std::cout << "\n"
//  << CGI::GetAnimateShowHideJavascriptMustBEPutInHTMLHead()
  << "</head>\n<body onload=\"checkCookie(); updatePreamble();\">\n";
  //std::cout << IPAdressCaller;
//  std::stringstream tempStreamX;
//  static_html3(tempStreamX);
//  static_html5(tempStreamX);
//  std::cout << tempStreamX.str() << std::endl;
//  std::cout << inputString;
#ifndef WIN32
  pthread_t TimerThread;
  pthread_create(&TimerThread, NULL,*RunTimer, 0);
#endif
//  std::cout << "Raw input: " << inputString << "<hr>";
  CGI::functionCGIServerIgnoreUserAbort=&ignoreUserAbortSignal;
  List<std::string> inputStrings, inputStringNames;
  CGI::ChopCGIInputStringToMultipleStrings
  (theParser.inputStringRawestOfTheRaw, inputStrings, inputStringNames);
//  std::cout << "Chopped strings: ";
//  for (int i=0; i<inputStrings.size; i++)
//    std::cout << inputStringNames[i] << " = " << inputStrings[i] << "<br>";
//  std::cout << "<hr>";
  std::string civilizedInput, inputRankString, inputWeylString;
  if (inputStringNames.Contains("textInput"))
    civilizedInput= inputStrings[inputStringNames.IndexOfObject("textInput")];
  if (inputStringNames.Contains("textDim"))
    inputRankString= inputStrings[inputStringNames.IndexOfObject("textDim")];
  if (inputStringNames.Contains("textType"))
    inputWeylString= inputStrings[inputStringNames.IndexOfObject("textType")];
  CGI::CivilizedStringTranslationFromCGI(civilizedInput, civilizedInput);
  List<std::string> optionsType, optionsRank;
  optionsType.AddOnTop("A");
  optionsType.AddOnTop("B");
  optionsType.AddOnTop("C");
  optionsType.AddOnTop("D");
  optionsType.AddOnTop("E");
  optionsType.AddOnTop("F");
  optionsType.AddOnTop("G");
//  optionsType.AddOnTop("NoPreamble");
  optionsRank.AddOnTop("1");
  optionsRank.AddOnTop("2");
  optionsRank.AddOnTop("3");
  optionsRank.AddOnTop("4");
  optionsRank.AddOnTop("5");
  optionsRank.AddOnTop("6");
  optionsRank.AddOnTop("7");
  optionsRank.AddOnTop("8");
//  civilizedInput="a{}b";
//  civilizedInput="Polynomial{}(a+a_1)";
//  civilizedInput="[a,b,c]";
//  civilizedInput="h_{{a}}:={\\bar h}_a; \nh_1h_2";
//  civilizedInput="g:=SemisimpleLieAlgebra{}A_1";//\ng_0";
//  civilizedInput="g:=SemisimpleLieAlgebra{}A_1;\nh_{{a}}:=g_(0,a);\ng_1+h_1";
  //civilizedInput="SemisimpleLieAlgebra{} A_1";
//  civilizedInput="f{}0:=1;\nf{}1:=f{}2;\nf{}{{x}}:=f{}(x-1)+f{}(x-2);\nf{}1";
//  civilizedInput="1+1";
//  civilizedInput="{{a}}*a:=a^2;\nx*x";
  //civilizedInput="Matrix{}(A,5,6)";
//  civilizedInput="\\begin{array}{cc} a &a \\end{array}";
//  civilizedInput="\\begin{array}{cc}x&((2,3),a,a,((b,a),a,a,b,(a,b)))\\\\ 1\\end{array}";
//  civilizedInput="fib{}1:=1;\nfib{}0:=1;\nfib{}{{n}}:=fib{}(n-1)+fib{}(n-2);\nfib{}10";
//  civilizedInput="hwv{}(B_3,(x_1,0,1),(1,0,0))";
//  civilizedInput="Polynomial{} (1)";
//  civilizedInput="g:= SemisimpleLieAlgebra{}B_3:=g_1 hwv{}(B_3, (x_1,0,1),(1,0,0));\n hwv{}(B_3, (x_1,0,1),(1,0,0)):=v_\\lambda;\n result \n ";
//  inputWeylString="Calculator";
//  civilizedInput="g:=SemisimpleLieAlgebra{} B_3;\ng_{-1} hwv{}(B_3, (x_1,0,1),(1,0,0))";
//  civilizedInput="g:=SemisimpleLieAlgebra{} B_3;\nv:=hwv{}(B_3, (x_1,0,1),(1,0,0));\ng_{0,1}g_{-1} {}v";
//civilizedInput="g:=SemisimpleLieAlgebra{} B_3";
//  civilizedInput="hwv{}(B_3, (x_1,0,1),(1,0,0))";
//  inputWeylString="B";
//  civilizedInput="g:=SemisimpleLieAlgebra{}G_2;\nUE{}(g_1+2g_2)";
  /*civilizedInput="A_1:=1/12x_1^{2}+1/2x_1+5/12;\n\
A_2:=1/12x_1^{2}+7/12x_1+1/2;\n\
A_3:=1/12x_1^{2}+7/12x_1+1;\n\
A_4:=1/12x_1^{2}+2/3x_1+1;\n\
A_5:=1/12x_1^{2}+2/3x_1+1 ;\n\
A_6:=1/12x_1^{2}+3/4x_1+7/6;\n\
A_7:=1/12x_1^{2}+3/4x_1+5/3;\n\
A_8:=1/12x_1^{2}+5/6x_1+7/4;\n\
Solve{}({{a}}x_1+{{b}}):=-b/a;\n\
Solve{}({{a}}x_1):=0;\n\
B{}({{a}},{{b}}):=Solve{}(A_a-A_b);\n\
UnionNoRepetition{}\n\
FunctionToMatrix{}(B,8,8)";*/
//  civilizedInput="g:=SemisimpleLieAlgebra{} G_2;\nv:=hwvbf{}(g_{-1} g_{-2}, g_{-1}g_{-2}, (2,2))";
//  civilizedInput="SemisimpleLieAlgebra{} G_2;\nUE{}(g_1 )";
//  civilizedInput="g:=SemisimpleLieAlgebra{} G_2;\nUE{}(g_2 g_1+g_1)";
//  civilizedInput="g:=SemisimpleLieAlgebra{} G_2;\nUE{}(2x_1 g_2 g_1+3g_1)";
//  civilizedInput="g:=SemisimpleLieAlgebra{} G_2;\nhwTAAbf{}(g_-1, g_-1, (1,0))";
//  civilizedInput="g:=SemisimpleLieAlgebra{} B_3;\nv:=hwv{}(B_3, (x_1,0,1),(1,0,0));\ng_{0,1}g_{-1} v";
//civilizedInput="g:=SemisimpleLieAlgebra{} B_3;\nv:=hwv{}(B_3, (x_1,0,1),(1,0,0));\ng_{0,1}g_{-1} v";
//  civilizedInput="g:= SemisimpleLieAlgebra{}B_3;UE{}(g_{-1}^{2}g_{-2}g_{-3})";
//inputWeylString="A";
//inputRankString="2";
//civilizedInput=" irreducibleRep(1,1)";
//  civilizedInput="a";
//  civilizedInput="g:=SemisimpleLieAlgebra{} B_3;\nv:=hwv{}(B_3, (x_1,0,1),(1,0,0));\ng_{0,1}g_{-1} v";
//  civilizedInput="g:=SemisimpleLieAlgebra{}G_2; 2 h_1";
//  civilizedInput="g:=SemisimpleLieAlgebra{}G_2; 2 g_{0,1}";
//civilizedInput="g:=SemisimpleLieAlgebra{}G_2; (g_{1}^2)^x";
//civilizedInput="g:=SemisimpleLieAlgebra{} B_3;\n[g_2, g_{-9}^{n_5}g_{-8}^{n_4}g_{-6}^{n_3}g_{-4}^{n_2}g_{-1}^{n_1}]";
//  civilizedInput="g:=SemisimpleLieAlgebra{}G_2; g_1; g_2; g_{0,1}; [[g_1,g_2], [g_{-1}, g_{-2}]]";
//civilizedInput="g:=SemisimpleLieAlgebra{} G_2;UE{}(g_3^2)";
//civilizedInput="g:=SemisimpleLieAlgebra{} B_3;v:=hwv{}(B_3, (x_1,0,1),(1,0,0))";
//  civilizedInput="g:= SemisimpleLieAlgebra{}B_3; h_{{i}}:=g_{0, i};g_{-1}^{n_1} hwv{}(B_3, (x_1,0,0),(1,0,0))";
//  civilizedInput="g:= SemisimpleLieAlgebra{}A_1; h_{{i}}:=g_{0, i}; (h_1)hwv{}(A_1, (x_1),(1))";
//if (civilizedInput=="")
//civilizedInput="printSlTwoSubalgebrasAndRootSubalgebras{}(G_2)";
//  civilizedInput="WeylDimFormula{}(G_2, (1,x_1));";
//  civilizedInput="WeylDimFormula{}(G_2, (1,0));WeylDimFormula{}(E_6, (2,0,0,0,0,0));";
//  civilizedInput="DecomposeInducingRepGenVermaModule{}(B_2,(x_1, 1),(1,0), (1,1))";
//civilizedInput="DecomposeInducingRepGenVermaModule{}(B_3,(x_1, 1,0),(1,0,0), (1,0,0))";
//civilizedInput="DecomposeInducingRepGenVermaModule{}(B_3,(a,0,1),(1,0,0), (1,0,1))";
//  civilizedInput=" g:=SemisimpleLieAlgebra{}G_2; hmmG2inB3{}(g_1);hmmG2inB3{}(g_2)";
/*  civilizedInput="g:= SemisimpleLieAlgebra{}B_3; h_{{i}}:=g_{0, i}; i{}(c):= hmmG2inB3{}( Casimir{}G_2); \
v_\\lambda:=hwv{}(B_3, (x_1, 2,0), (1,0,0));\
x_1:= Polynomial{} x_1;\
(i{}c- (1/12x_{1}^{2}+11/12x_{1}+5/2))(\
g_{-2} v_\\lambda\
)";*/
//civilizedInput="g:= SemisimpleLieAlgebra{}B_3; v_\\lambda:=hwv{}(B_3, (x_1, 2,0), (1,0,0));g_2(g_{-2} v_\\lambda)";
//civilizedInput="g:=SemisimpleLieAlgebra{} B_3;c:=hmmG2inB3{}(Casimir{}G_2);hwTAAbf{}(c, 1, (x_1,2,0))";
//civilizedInput="g:= SemisimpleLieAlgebra{}B_3; h_{{i}}:=g_{0, i}; v_\\lambda:=hwv{}(B_3, (x_1, 2,0), (1,0,0));(g_{-5}g_{-1}g_{5}+2g_{-1}h_{3}+2g_{-1}h_{2}+2g_{-1}) v_\\lambda";
 // civilizedInput="g:= SemisimpleLieAlgebra{}B_3; h_{{i}}:=g_{0, i}; c:= hmmG2inB3{}( Casimir{}G_2); x_1:= Polynomial{} x_1;
 // v_\\lambda:=hwv{}(B_3, (x_1, 2,0), (1,0,0));(g_5g_{-5}g_{-1}) v_\\lambda;";
//  civilizedInput="g:= SemisimpleLieAlgebra{}B_3; h_{{i}}:=g_{0, i}; c:= hmmG2inB3{}( Casimir{}G_2);\n x_1:= Polynomial{} x_1;\nv_\\lambda:=hwv{}(B_3, (x_1, 2,0), (1,0,0));\nc v_\\lambda;";
//  civilizedInput="g:= SemisimpleLieAlgebra{}B_2; \nv_\\lambda:=hwv{}(B_2, (1,1), (1,1)); \n-1/3g_{-4}v_\\lambda";
//civilizedInput="animateLittelmannPaths{}(A_2, (1,0));";
//civilizedInput="1";
//civilizedInput="g:=SemisimpleLieAlgebra{}A_1;";
//civilizedInput="Polynomial{}(x(x+y)^2)";
//civilizedInput="g:=SemisimpleLieAlgebra{}G_2; hmmG2inB3{}(c);";
//civilizedInput="g:=SemisimpleLieAlgebra{}A_1; [g_1,g_{-1}] ";
//civilizedInput="fSplitFDpartB3overG2{}(x_1,2,0) ";
//civilizedInput="fSplitFDpartB3overG2{}(x_1,1,0) ";
//civilizedInput="g:=SemisimpleLieAlgebra{}A_2;\nv_\\lambda:=hwv{}(A_2, (a, b),(1,1));\nv_\\lambda";
//civilizedInput="fSplitFDpartB3overG2{}(x_1,1,0)";
//  civilizedInput="fSplitFDpartB3overG2CharsOnly{}(0,1,0)";
//civilizedInput="fSplitFDpartB3overG2{}(x_1,1,0)";
  //civilizedInput="{{a}}:if (a-1)==x:=6;\nx+1-1";
//  civilizedInput="fSplitFDpartB3overG2{}(0,0,1)";
//civilizedInput="fSplitFDpartB3overG2{}(x_1,1,0)";
//civilizedInput="fPrintB3G2branchingTable{}(1)";
//  civilizedInput="fPrintB3G2branchingTableCharsOnly{}(2)";
//civilizedInput= "fSplitFDpartB3overG2{}(0,1,0)";
//civilizedInput="fPrintB3G2branchingTable{}(1)";
//civilizedInput="fPrintB3G2branchingTable{}(1, (0,0,0)); fPrintB3G2branchingTable{}(1, (x_1,0,0))";
//civilizedInput="fPrintB3G2branchingTable{}(2, (0,x,0)); ";
//civilizedInput="fPrintB3G2branchingTable{}(2, (x_1,x_2,0))";
//  civilizedInput="1/0";
//  civilizedInput="x_1:=Polynomial{}(x_{{1}});x_2:=Polynomial{}(x_{{2}});x_1/x_2+x_1/(x_1+x_2)";
//civilizedInput="SplitGenericGenVermaTensorFD{}(G_2,(x_1,x_2), (1,0)); ";
//civilizedInput="SplitFDTensorGenericGeneralizedVerma{}(G_2, (1, 0), (x_1, x_2)); ";
//civilizedInput="SplitFDTensorGenericGeneralizedVerma{}(G_2, (1, 0), (0, x_1)); ";
//  civilizedInput=" \\\\";
  //theParser.flagLogSyntaxRules=true;
//  civilizedInput="WriteGenVermaAsDiffOperators{}(B_3, 1, (0, 0, y)); ";
//civilizedInput="WeylOrbitSimpleCoords{}(B_2, (y, y));";
//  civilizedInput="WriteGenVermaAsDiffOperators{}(B_3, 0, (x_1, x_2, 0)); ";
//  civilizedInput="WeylOrbitFundRho{}(B_3, (1, 0,0) )";
//civilizedInput="1+a";
//  civilizedInput="KLcoeffs{}(B_3, (1,0,0))";
//  civilizedInput="KLcoeffs{}(A_3)";
//civilizedInput="WriteGenVermaAsDiffOperators{}(B_3, 0, (x, 0, 0)); ";
//civilizedInput= "printSlTwoSubalgebrasAndRootSubalgebras{}(E_8)";
//civilizedInput="WriteGenVermaAsDiffOperators{}(B_3, 0, (x_1, 0,0)); ";
//  civilizedInput="fSplitFDpartB3overG2CharsOnly{}(x_1,2,0)";
//civilizedInput="printAllPartitions{}(A_2, (2,3))";
//civilizedInput="g:= SemisimpleLieAlgebra{}A_1; h_{{i}}:=g_{0, i}; g_1;";
//civilizedInput="(Polynomial{}(x_1+x_2+x_3))^30";
//civilizedInput="DecomposeInducingRepGenVermaModule{}(B_3,(0, 1,1),(1,0,0), (1,0,1))";
//civilizedInput="hwv{}(A_1,1, 0)";
//  civilizedInput="hwv{}(A_2,(0,1), (0,0))";
//civilizedInput="hwv{}(A_1, 1,0)";
//  civilizedInput="hwv{}(A_2, (0,0),(0,0))";
//  civilizedInput="g:= SemisimpleLieAlgebra{}A_2; h_{{i}}:=g_{0, i}; [g_{-1},g_{1}] ";
//  civilizedInput="fTestMonomialBasisConjecture{}(2, 50)";
//civilizedInput="hwv{} (G_2, (2,1),(0,0)) ;";
//civilizedInput="DecomposeCharGenVermaToIrreps{}(G_2, (x_1, 0))";
//  civilizedInput="SplitFDpartB3overG2{}(x_1,1,0)";
//  civilizedInput=
//  "g:=SemisimpleLieAlgebra{} B_3;v_\\lambda:=:hwv{}(B_3, (x_1,0,1),(1,0,0));w:=g_{-1} v_\\lambda;g_1 w";
//  civilizedInput="DecomposeCharGenVermaToIrreps{}(G_2, (x_1, 0))";
//  civilizedInput="SplitFDpartB3overG2{}(x_1,1,0)";
//  civilizedInput="LSpath{}(G_2, (0,0), (2,1) )";
//civilizedInput="e_{{i}}:=LRO_i; e_{-1} e_{-1} LSpath{}(G_2, (0,0), (2,1))";
//  civilizedInput="x:=Polynomial{}x;\n(1+x)/x";
//  civilizedInput="x:=Polynomial{}x;x+(x-1)/(x^2+1) +1/(x^2+1)^2 ";
//civilizedInput= "x:=1;x:=Polynomial{}x;(4x^{5}-4 x^{4}+12 x^{3}-12 x^{2}+9 x-9)/(x-1)";
//civilizedInput="1/(1-1)";
//  civilizedInput="EmbedSSalgebraInWeylAlgebra{}(B_3, (0,0,0), (0, 0, 1)); ";
//  civilizedInput="\\sqrt{}(2)+\\sqrt{}2";
//  civilizedInput="\\sqrt{}(2)+\\sqrt{}2";
//  civilizedInput="FactorOneVarPolyOverRationals{}{x^2-4}";
//  civilizedInput="hwv{} (A_3, (1,0,1),(0,0,0)) ; ";
//  civilizedInput="EmbedSSalgebraInWeylAlgebra{}(A_2, (0,0), (0, 1)); ";
//  civilizedInput="EmbedSSalgebraInWeylAlgebra{}(A_2, (1,0), ( 0, 1)); ";
//  civilizedInput="PrintSemisimpleSubalgebras{}(A_2)";
//  civilizedInput="DecomposeInducingRepGenVermaModule{}(B_3,(0, 1,1),(1,0,0), (1,0,1))";
//  civilizedInput="GroebnerBuchberger{}(x^2+1, y^2, x+y )";
//  civilizedInput="GroebnerBuchberger{}(1-s^2-c^2, -x+s^3,-y+c^3)";
//  civilizedInput="GroebnerBuchbergerLex{}(s^2+c^2+1, a-s^4, b-c^4 )";
//  civilizedInput= "PrintB3G2branchingTableCharsOnly{}(2, (x_1,0,0))";
//  civilizedInput="PrintSemisimpleSubalgebras{}(F_4)";
//  civilizedInput="printSlTwoSubalgebrasAndRootSubalgebras{}(F_4)";
//  civilizedInput="printSlTwoSubalgebras{}(E_9)";
//  civilizedInput="experimentalPrintSemisimpleSubalgebras{}(A_2)";
//  civilizedInput="experimentalPrintSemisimpleSubalgebras{}(a_3)";
//  civilizedInput="experimentalPrintSemisimpleSubalgebras{}(b_3)";
//  civilizedInput="experimentalPrintSemisimpleSubalgebras{}(A_2)";
//  civilizedInput="experimentalPrintSemisimpleSubalgebras{}(g_2)";
//  civilizedInput= "experimentalPrintSemisimpleSubalgebras{}(b_3)";
//  civilizedInput="experimentalEmbedSemisimpleInSemisimple{}(G_2, B_3)";
//  civilizedInput="experimentalEmbedSemisimpleInSemisimple{}(a_2, f_4)";
//  civilizedInput="SolveNseparableQuasiLinearSystem{}(   x_{12}x_{24}-x_{10}x_{22}-2x_{8}x_{20}-x_{7}x_{19}+1, x_{11}x_{24}-x_{10}x_{23}-x_{8}x_{21},x_{9}x_{24}-x_{8}x_{23}+x_{7}x_{21},x_{6}x_{24}+2x_{5}x_{23}-x_{4}x_{22}+2x_{3}x_{21}-2x_{2}x_{20}-x_{1}x_{19},x_{12}x_{23}-x_{11}x_{22}-x_{9}x_{20},x_{11}x_{23}+x_{10}x_{22}+x_{8}x_{20}-1,x_{9}x_{23}+x_{8}x_{22}-x_{7}x_{20},x_{12}x_{21}-x_{11}x_{20}+x_{9}x_{19},x_{11}x_{21}+x_{10}x_{20}-x_{8}x_{19},x_{9}x_{21}+x_{8}x_{20}+x_{7}x_{19}-1,x_{12}x_{18}+2x_{11}x_{17}-x_{10}x_{16}+2x_{9}x_{15}-2x_{8}x_{14}-x_{7}x_{13},x_{6}x_{18}-x_{4}x_{16}-2x_{2}x_{14}-x_{1}x_{13}+1,x_{5}x_{18}+x_{4}x_{17}+x_{2}x_{15},x_{3}x_{18}+x_{2}x_{17}-x_{1}x_{15},x_{6}x_{17}+x_{5}x_{16}+x_{3}x_{14},x_{5}x_{17}+x_{4}x_{16}+x_{2}x_{14}-1,x_{3}x_{17}+x_{2}x_{16}-x_{1}x_{14},x_{6}x_{15}+x_{5}x_{14}-x_{3}x_{13},x_{5}x_{15}+x_{4}x_{14}-x_{2}x_{13},x_{3}x_{15}+x_{2}x_{14}+x_{1}x_{13}-1)";
  //civilizedInput="SolveNseparableQuasiLinearSystem{}(x*y-1)";
 // civilizedInput="SolveNseparableQuasiLinearSystem{}(x*y, x*z)";
 //civilizedInput="gcd{}(-x_{13}^{2}x_{15}^{3}x_{21}^{2}x_{22}-2x_{13}^{2}x_{14}x_{15}x_{17}x_{21}^{3}+   2x_{13}^{2}x_{15}^{2}x_{16}x_{21}^{3}+2x_{13}^{2}x_{15}^{2}x_{17}x_{20}x_{21}^{2}+   x_{13}^{2}x_{14}x_{17}x_{18}x_{19}x_{21}^{2}-x_{13}^{2}x_{15}x_{16}x_{18}x_{19}x_{21}^{2}+   x_{13}^{2}x_{15}x_{17}^{2}x_{19}x_{21}^{2},   x_{13}^{3}x_{15}x_{18}x_{21}^{2}x_{22}-2x_{13}^{3}x_{17}^{2}x_{21}^{3}-   4x_{13}^{2}x_{14}x_{15}x_{17}x_{21}^{3}+2x_{13}^{2}x_{15}^{2}x_{16}x_{21}^{3}-   2x_{13}^{3}x_{17}x_{18}x_{20}x_{21}^{2}+2x_{13}^{2}x_{14}x_{17}x_{18}x_{19}x_{21}^{2}-   x_{13}^{2}x_{15}x_{16}x_{18}x_{19}x_{21}^{2}  )";
//  civilizedInput="gcd{}(  y^2  , 1+ y^2 )";
  //civilizedInput="gcd{}(x+ x*y^3,  x y z+1)";
//  civilizedInput="gcd{}(-x_{10}x_{14}x_{16}x_{24}-2x_{12}^{2}x_{15}x_{23}+x_{12}x_{13}x_{14}x_{23}-x_{10}x_{12}x_{15}x_{22}+x_{12}x_{15} , x_{14}x_{16} )";
//  civilizedInput="GroebnerBuchbergerLex{}(x_{6}x_{18}+2x_{5}x_{17}+x_{4}x_{16}+2x_{3}x_{15}+2x_{2}x_{14}+x_{1}x_{13}-3, 2x_{6}x_{18}+3x_{5}x_{17}+x_{4}x_{16}+3x_{3}x_{15}+2x_{2}x_{14}+x_{1}x_{13}-4, x_{5}x_{15}+x_{4}x_{14}-x_{2}x_{13}, x_{6}x_{15}+x_{5}x_{14}-x_{3}x_{13}, x_{3}x_{17}+x_{2}x_{16}-x_{1}x_{14}, x_{6}x_{18}+2x_{5}x_{17}+x_{4}x_{16}+x_{3}x_{15}+x_{2}x_{14}-2, x_{6}x_{17}+x_{5}x_{16}+x_{3}x_{14}, x_{3}x_{18}+x_{2}x_{17}-x_{1}x_{15}, x_{5}x_{18}+x_{4}x_{17}+x_{2}x_{15}, x_{6}x_{24}+2x_{5}x_{23}-x_{4}x_{22}+2x_{3}x_{21}-2x_{2}x_{20}-x_{1}x_{19}, x_{12}x_{24}+2x_{11}x_{23}+x_{10}x_{22}+2x_{9}x_{21}+2x_{8}x_{20}+x_{7}x_{19}-3, x_{11}x_{23}+x_{10}x_{22}+x_{9}x_{21}+2x_{8}x_{20}+x_{7}x_{19}-2, x_{9}x_{21}+x_{8}x_{20}+x_{7}x_{19}-1, x_{11}x_{21}+x_{10}x_{20}-x_{8}x_{19}, x_{12}x_{21}-x_{11}x_{20}+x_{9}x_{19}, x_{9}x_{23}+x_{8}x_{22}-x_{7}x_{20}, x_{12}x_{23}-x_{11}x_{22}-x_{9}x_{20}, x_{9}x_{24}-x_{8}x_{23}+x_{7}x_{21}, x_{11}x_{24}-x_{10}x_{23}-x_{8}x_{21}, x_{12}x_{18}+2x_{11}x_{17}-x_{10}x_{16}+2x_{9}x_{15}-2x_{8}x_{14}-x_{7}x_{13})";
//  civilizedInput="GroebnerBuchbergerLex{}(s^2+c^2+1, a-s^4, b-c^4 )";
//  civilizedInput="a:=Polynomial{}(x+y);";
//  civilizedInput="GroebnerBuchbergerLex{}(s^2+c^2+1, a-s^4, b-c^4 )";
//  civilizedInput="a:=(c:=d; d:=f);\na";
//civilizedInput="({{a}}:={{b}}):=b;";
//civilizedInput="(f{}{{a}}:={{b}}):=(g{}a:=b);f{}a:=a+5;f{}a;";
//  civilizedInput="b:=({{b}}:=b)";
  std::stringstream tempStreamXX;
  static_html4(tempStreamXX);
  std::cout << "<table>\n <tr valign=\"top\">\n <td>";
  std::cout << "\n<FORM method=\"POST\" name=\"formCalculator\" action=\""
  << theParser.DisplayNameCalculator << "\">\n" ;
  std::cout << GetSelectHTMLStringTEmp
  (optionsType, optionsRank, inputWeylString, inputRankString,
   inputStringNames.Contains("checkUsePreamble"))
  << tempStreamXX.str();
  std::cout << "\n<br>\n";
  std::string civilizedInputSafish;
  if (CGI::GetHtmlStringSafeishReturnFalseIfIdentical(civilizedInput, civilizedInputSafish))
    std::cout
    << "Your input has been treated normally, however the return string"
    << " of your input has been modified. More precisely, &lt; and &gt;  are "
    << " modified due to a javascript hijack issue. ";
  std::cout << "<textarea rows=\"3\" cols=\"30\" name=\"textInput\" id=\"textInputID\""
  << " onkeypress=\"if (event.keyCode == 13 && event.shiftKey) {storeSettings(); "
  << " this.form.submit(); return false;}\" >";
  std::cout << civilizedInputSafish;
  std::cout << "</textarea>\n<br>\n";
  std::cout << "<input type=\"submit\" title=\"Shift+Enter=shortcut from input text box. \" name=\"buttonGo\" "
  << "value=\"Go\" onmousedown=\"storeSettings();\" > ";
  theParser.DisplayNameCalculator="/vpf/cgi-bin/calculator";
  if (civilizedInput!="")
    std::cout << "<a href=\"" << theParser.DisplayNameCalculator << "?"
    << theParser.inputStringRawestOfTheRaw
    << "\">Link to your input.</a>";
//  std::cout << "<br>" << CGI::GetLatexEmbeddableLinkFromCalculatorInput
//  (theParser.inputStringRawestOfTheRaw, civilizedInput);
  std::cout << "\n</FORM>";
  std::cout << theParser.javaScriptDisplayingIndicator;
  std::cout.flush();
  if (civilizedInput!="")
  { if (inputStringNames.Contains("checkUsePreamble"))
    { std::stringstream tempStream;
      tempStream << "g:=SemisimpleLieAlgebra{}" << inputWeylString
      << "_" << inputRankString << "; h_{{i}}:=g_{0, i};";
      civilizedInput=tempStream.str()+civilizedInput;
    }
    theParser.Evaluate(civilizedInput);
    ComputationComplete=true;
//    std::cout << "<hr>";
    std::cout << theParser.outputString;
    if (theParser.parsingLog!="")
      std::cout << "<b> As requested, here is a calculator parsing log</b><br>"
      << theParser.parsingLog;
  }
//  theComputation.flagLogSyntaxRules=inputRankString=="2";
  std::cout << "</td>";
  if (theParser.outputCommentsString!="")
  { std::cout << "<td valign=\"top\">";
    if (theParser.outputCommentsString.size()<10000)
      std::cout << theParser.outputCommentsString;
    else
      std::cout << "The comments generated by your computation are a bit too long."
      << " If you want to see them click the show/hide button below.<br>"
      << CGI::GetHtmlSpanHidableStartsHiddeN(theParser.outputCommentsString);
    std::cout << "</td>";
  }
  std::cout << "<td valign=\"top\">";
  std::cout << ProjectInformation::GetMainProjectInfo().ToString();
  std::cout << "<hr><b>CommandList status. </b><br>";
  std::cout << theParser.ToString();

  std::cout << "</td></tr></table>";
  std::stringstream tempStream3;
//  static_html5(tempStream3);
//  static_html6(tempStream3);
//  std::cout << tempStream3.str();
//  std::cout << "</div>";
  std::cout << "\n\n<script language=\"javascript\">\n// List of words to show in drop down\n var functionNameArray =new Array(";
  bool isFirst=true;
  for (int i=0; i<theParser.theObjectContainer.theNonBoundVars.size; i++)
  { VariableNonBound& currentVar=theParser.theObjectContainer.theNonBoundVars[i];
    if (true)
    { if (!isFirst)
        std::cout << ",";
      isFirst=false;
      std::cout << "\"" << currentVar.theName << "\"";
    }
  }
  std::cout << ");\n  //var obj = actb(document.getElementById('textInputID'), functionNameArray);\n</script>\n";
//  for(int i=0; i<theParser.SystemCommands.size; i++)
//  { std::cout << "<br>About to execute: " << theParser.SystemCommands.TheObjects[i].c_str() << "\n" ;
 //   system(theParser.SystemCommands[i].c_str());
 // }

  std::cout << "</body></html>";
  std::cout << "<!--";
  std::cout.flush();
  ProgressReport theReport(theParser.theGlobalVariableS);
  for(int i=0; i<theParser.SystemCommands.size; i++)
  { std::stringstream out;
    out << "\n\ncommand: " << theParser.SystemCommands[i] << "\n" ;
    theReport.Report(out.str());
    system(theParser.SystemCommands[i].c_str());
  }
  std::cout << "-->";
  return 0;
}

void static_html3(std::stringstream& output){
output << " <!>\n";
output << " <script type=\"text/javascript\"> \n";
output << " var timeOutCounter=0;\n";
output << " window.setTimeout(\"progressReport()\",1000);\n";
output << " var newReportString=\"\";\n";
output << " function progressReport()\n";
output << " { timeOutCounter++;\n";
output << "   var oRequest = new XMLHttpRequest();\n";
output << "   var sURL  = \"/tmp/report.txt\";\n";
output << "   oRequest.open(\"GET\",sURL,false);\n";
output << "   oRequest.setRequestHeader(\"User-Agent\",navigator.userAgent);\n";
output << "   oRequest.send(null)\n";
output << "   if (oRequest.status==200)\n";
output << "   { newReportString= oRequest.responseText;\n";
output << "     if (newReportString!=\"\")\n";
output << "       el = document.getElementById(\"idProgressReport\").innerHTML= \"Attempting indicator refresh each second. Number of seconds: \"+ timeOutCounter+\"<br>Status file content:<br>\" +newReportString;\n";
output << "   }\n";
output << "   window.setTimeout(\"progressReport()\",1000);\n";
output << " }\n";
output << " </script>\n";
output << " <div id=\"idProgressReport\">\n";
output << " </div>\n";
output << " \n";
output << " \n";
}
void static_html4(std::stringstream& output){
output << " <!>\n";
output << " <script src=\"http://ajax.googleapis.com/ajax/libs/dojo/1.6.1/dojo/dojo.xd.js\" type=\"text/javascript\"></script>\n";
output << " \n";
output << " <script type=\"text/javascript\"> \n";
output << " function switchMenu(obj)\n";
output << " { var el = document.getElementById(obj);	\n";
output << "   if ( el.style.display != \"none\" ) \n";
output << "     el.style.display = 'none';\n";
output << "   else \n";
output << "     el.style.display = '';\n";
output << " }\n";
output << " function hideItem(obj)\n";
output << " { document.getElementById(obj).style.display=\"none\";\n";
output << " }\n";
output << " function showItem(obj)\n";
output << " { document.getElementById(obj).style.display=\"\";\n";
output << " }\n";
output << " </script>\n";
output << " <script type=\"text/javascript\"> \n";
output << " function getCookie(c_name)\n";
output << " { VPFcookie=document.cookie.split(\";\");\n";
output << "   for (i=0;i<VPFcookie.length;i++)\n";
output << "   { x=VPFcookie[i].substr(0,VPFcookie[i].indexOf(\"=\"));\n";
output << "   	y=VPFcookie[i].substr(VPFcookie[i].indexOf(\"=\")+1);\n";
output << "     x=x.replace(/^\\s+|\\s+$/g,\"\");\n";
output << "     if (x==c_name)\n";
output << "       return unescape(y);\n";
output << "   }\n";
output << " }\n";
output << " function storeSettings()\n";
output << " { theCalculatorForm=document.getElementById(\"textInputID\");  \n";
output << "   //alert(theCalculatorForm.style.width);\n";
output << "   addCookie(\"widthCalculatorText\", theCalculatorForm.style.width, 100);  \n";
output << "   addCookie(\"heightCalculatorText\", theCalculatorForm.style.height, 100);\n";
output << "   //alert(document.cookie);\n";
output << " }\n";
output << " \n";
output << " function addCookie(theName, theValue, exdays)\n";
output << " { exdate=new Date();\n";
output << "   exdate.setDate(exdate.getDate() + exdays);\n";
output << "   c_value=escape(theValue) + ((exdays==null) ? \"\" : \"; expires=\"+exdate.toUTCString());\n";
output << "   document.cookie=theName + \"=\" + c_value;\n";
output << " }\n";
output << " \n";
output << " function checkCookie()\n";
output << " { theCalculatorForm=document.getElementById(\"textInputID\");  \n";
output << "   theOldWidth=getCookie(\"widthCalculatorText\");\n";
output << "   theOldHeight=getCookie(\"heightCalculatorText\");\n";
output << "   //alert(\"height: \" + theOldHeight +\" width: \" + theOldWidth);\n";
output << "   //theCalculatorForm.setStyle(\"width:\"+ theOldWidth);\n";
output << " //  theCalculatorForm.style.height=theOldHeight;\n";
output << "   theCalculatorForm.style.width  = theOldWidth;\n";
output << "   theCalculatorForm.style.height = theOldHeight;\n";
output << " }\n";
output << " </script>\n";
}


