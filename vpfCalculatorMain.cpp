//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".

#include "vpfHeader4SystemFunctionsGlobalObjects.h"

ProjectInformationInstance projectInfoInstanceCalculatorCpp(__FILE__, "Calculator web interface.");

extern void static_html4(std::stringstream& output);
extern void static_html3(std::stringstream& output);
extern int serverMain();

int main_command_input(int argc, char **argv)
{ MacroRegisterFunctionWithName("main_command_input");
  std::stringstream theInputStream;
  theParser.theGlobalVariableS->SetStandardStringOutput(&CGI::MakeStdCoutReport);
  for (int i=1; i<argc; i++)
  { theInputStream << argv[i];
    if (i<argc-1)
      theInputStream << " ";
  }
  //  stOutput << "\n\n\n" << theParser.DisplayPathServerBase << "\n\n";
  //  return 0;
  theParser.inputStringRawestOfTheRaw =theInputStream.str();
  PredefinedStrings(theParser.inputStringRawestOfTheRaw);
  theParser.Evaluate(theParser.inputStringRawestOfTheRaw);
  std::fstream outputFile;
  XML::OpenFileCreateIfNotPresent(outputFile, "./outputFileCommandLine.html", false, true, false);
  stOutput << theParser.outputString;
  outputFile << theParser.outputString;
  stOutput << "\nTotal running time: " << GetElapsedTimeInSeconds() << " seconds. \nOutput written in file ./outputFileCommandLine.html\n";
  return 0;
}

int main(int argc, char **argv)
{ MacroRegisterFunctionWithName("main");
  InitializeGlobalObjects();
  onePredefinedCopyOfGlobalVariables.MaxComputationTimeSecondsNonPositiveMeansNoLimit=5000;
  //	stOutput <<  "<br>" << (int) &theGlobalVariables.callSystem ;
  ParallelComputing::cgiLimitRAMuseNumPointersInList=4000000000;
  onePredefinedCopyOfGlobalVariables.inputDisplayPath="trunk/";
  if (argc>=1)
  { getPath(argv[0], onePredefinedCopyOfGlobalVariables.executablePath);
  //    stOutput << "input path: " << inputPatH << "\n\n";
    bool found=false;
    for (int j=onePredefinedCopyOfGlobalVariables.executablePath.size()-2; j>=0; j--)
    { if (onePredefinedCopyOfGlobalVariables.executablePath[j]=='/')
      { if (found)
          break;
        onePredefinedCopyOfGlobalVariables.inputDisplayPath="";
        found=true;
      }
      if (found)
        onePredefinedCopyOfGlobalVariables.inputDisplayPath.push_back(onePredefinedCopyOfGlobalVariables.executablePath[j]);
    }
    if (found)
      for (unsigned j=0; j<onePredefinedCopyOfGlobalVariables.inputDisplayPath.size()/2; j++)
        MathRoutines::swap(onePredefinedCopyOfGlobalVariables.inputDisplayPath[j], onePredefinedCopyOfGlobalVariables.inputDisplayPath[onePredefinedCopyOfGlobalVariables.inputDisplayPath.size()-1-j]);
  }
  if (argc==2)
  { std::string tempArgument=argv[1];
    if (tempArgument=="server")
      if (!serverMain())
      { stOutput << "Server exit normal. ";
        return 0;
      }
  }
  stOutput << "Content-Type: text/html\n\n";
  //  stOutput << "input path: " << inputDisplayPath << "\n\n";
  theParser.init(onePredefinedCopyOfGlobalVariables);
	if (argc>1)
    return main_command_input(argc, argv);
  else
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
  stOutput << "<html><meta name=\"keywords\" content= \"Root system, Root system Lie algebra, Vector partition function calculator, vector partition functions, Semisimple Lie algebras, "
  << "Root subalgebras, sl(2)-triples\"> <head> <title>calculator version  " << __DATE__ << ", " << __TIME__ << "</title>";
  stOutput << "<script src=\"../jsmath/easy/load.js\"></script>\n</head>\n<body onload=\"checkCookie();\">\n";
  CGI::functionCGIServerIgnoreUserAbort=&ignoreUserAbortSignal;
  List<std::string> inputStrings, inputStringNames;
  CGI::ChopCGIInputStringToMultipleStrings(theParser.inputStringRawestOfTheRaw, inputStrings, inputStringNames);
  std::string civilizedInput;
  if (inputStringNames.Contains("textInput"))
    civilizedInput= inputStrings[inputStringNames.GetIndex("textInput")];
  CGI::CivilizedStringTranslationFromCGI(civilizedInput, civilizedInput);
//  civilizedInput="WeylGroupTauSignatures{}(b_3)";
//  civilizedInput="WeylGroupTauSignatures{}(f_4);";
//  civilizedInput="WeylGroupConjugacyClassesRepresentatives{}a_2";
//  civilizedInput="WeylGroupConjugacyClasses b_2";
//  civilizedInput="WeylGroupConjugacyClassesRepresentatives a_2";
//  civilizedInput="WeylGroupTauSignatures{}(b_2);";
//  civilizedInput="WeylGroupTauSignatures{}(g_2);";
//  civilizedInput="WeylGroupTauSignatures{}(e_7);";
//civilizedInput="printRootSubalgebras{}a_1";
// civilizedInput="\\varepsilon_{{a}}:=MakeWeight{}(B_3, a, epsilon); 2\\varepsilon_2;";
//civilizedInput="eta_{{a}}:=MakeWeight{}(A_3, a, epsilon);eta_2";
//civilizedInput="UnivariatePartialFractions(\\frac{1}{x^2-2})";
//  civilizedInput="WeylOrbitFundRho{}(B_2, (1/2, 0) )";
//  civilizedInput="WeylOrbitFundRho{}(B_3, (3/2, 0,0) )";
//  civilizedInput="DFQEuler(x^2 + y^2 - 1, 0, 0, 1000, -2.5, 2.5)";
//  civilizedInput="WeylOrbitFundRho{}(B_2, (y, 0) )";
//  civilizedInput="printSemisimpleSubalgebras{}(e_7 )";
//civilizedInput="A:=\\frac{x}{y}; B:=d/dx A;Denominator:=CoefficientOf(d/dx y, B);";
  PredefinedStrings(civilizedInput);

  crash.userInputStringIfAvailable=civilizedInput;

  std::stringstream tempStreamXX;
  static_html4(tempStreamXX);

  stOutput << tempStreamXX.str();
  stOutput << "<table>\n <tr valign=\"top\">\n <td>";
  stOutput << "\n<FORM method=\"POST\" name=\"formCalculator\" action=\"" << theParser.DisplayNameCalculator << "\">\n" ;
  std::string civilizedInputSafish;
  if (CGI::GetHtmlStringSafeishReturnFalseIfIdentical(civilizedInput, civilizedInputSafish))
    stOutput << "Your input has been treated normally, however the return string of your input has been modified. More precisely, &lt; and &gt;  are "
    << " modified due to a javascript hijack issue. <br>";
  stOutput << "<textarea rows=\"3\" cols=\"30\" name=\"textInput\" id=\"textInputID\" onkeypress=\"if (event.keyCode == 13 && event.shiftKey) {storeSettings(); "
  << " this.form.submit(); return false;}\" >";
  stOutput << civilizedInputSafish;
  stOutput << "</textarea>\n<br>\n";
  stOutput << "<input type=\"submit\" title=\"Shift+Enter=shortcut from input text box. \" name=\"buttonGo\" value=\"Go\" onmousedown=\"storeSettings();\" > ";
  if (civilizedInput!="")
    stOutput << "<a href=\"" << theParser.DisplayNameCalculator << "?" << theParser.inputStringRawestOfTheRaw << "\">Link to your input.</a>";
  stOutput << "\n</FORM>";
  stOutput << theParser.javaScriptDisplayingIndicator;
  //  stOutput << "<br>Number of lists created before evaluation: " << NumListsCreated;
  if (civilizedInput!="")
  { theParser.Evaluate(civilizedInput);
    ComputationComplete=true;
    if (theParser.flagProduceLatexLink)
      stOutput << "<br>LaTeX link (\\usepackage{hyperref}):<br> " << CGI::GetLatexEmbeddableLinkFromCalculatorInput(theParser.inputStringRawestOfTheRaw, civilizedInput);
    stOutput << theParser.outputString;
    if (theParser.parsingLog!="")
      stOutput << "<b> As requested, here is a calculator parsing log</b><br>" << theParser.parsingLog;
  }
  stOutput << "\n\n</td>\n\n";
  if (theParser.outputCommentsString!="")
  { stOutput << "<td valign=\"top\">";
    if (theParser.outputCommentsString.size()<10000)
      stOutput << theParser.outputCommentsString;
    else
      stOutput << "The comments generated by your computation are a bit too long. If you want to see them click the show/hide button below.<br>"
      << CGI::GetHtmlSpanHidableStartsHiddeN(theParser.outputCommentsString);
    stOutput << "</td>";
  }
  stOutput << "<td valign=\"top\">";
  ProjectInformation::GetMainProjectInfo().theFiles.QuickSortAscending();
  stOutput << ProjectInformation::GetMainProjectInfo().ToString();
  stOutput << "<hr><b>Calculator status. </b><br>";
  stOutput << theParser.ToString();

  stOutput << "</td></tr></table>";
  std::stringstream tempStream3;
  stOutput << "\n\n<script language=\"javascript\">\n// List of words to show in drop down\n var functionNameArray = new Array(";
  stOutput << ");\n  //var obj = actb(document.getElementById('textInputID'), functionNameArray);\n</script>\n";
  stOutput << "</body></html>";
  stOutput << "<!--";
  ProgressReport theReport(theParser.theGlobalVariableS);
  for(int i=0; i<theParser.SystemCommands.size; i++)
  { std::stringstream out;
    out << "\n\ncommand: " << theParser.SystemCommands[i] << "\n" ;
    theReport.Report(out.str());
    system(theParser.SystemCommands[i].c_str());
  }
  stOutput << "-->";
  //close()
  return 0;
}
