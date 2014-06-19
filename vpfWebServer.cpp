//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#include "vpfHeader6WebServer.h"

ProjectInformationInstance projectInfoInstanceWebServer(__FILE__, "Web server implementation.");

extern void static_html4(std::stringstream& output);
extern void static_html3(std::stringstream& output);


class logger
{
  public:
  int currentColor;
  std::fstream theFile;
  logger()
  { FileOperations::OpenFileCreateIfNotPresent(theFile, "./../LogFile.html", false, true, false);
    currentColor=logger::normalColor;
  }
  enum loggerSpecialSymbols{ endL, red, blue, yellow, green, normalColor};
  std::string closeTagConsole()
  { return "\e[0m";
  }
  std::string closeTagHtml()
  { if (currentColor==logger::normalColor)
      return "";
    return "</span>";
  }
  std::string openTagConsole()
  { switch (this->currentColor)
    { case logger::red:
        return "\e[1;31m";
      case logger::green:
        return "\e[1;32m";
      case logger::blue:
        return "\e[1;34m";
      case logger::yellow:
        return "\e[1;33m";
      default:
        return "";
    }
  }
  std::string openTagHtml()
  { switch (this->currentColor)
    { case logger::red:
        return "<span style=\"color:red\">";
      case logger::green:
        return "<span style=\"color:green\">";
      case logger::blue:
        return "<span style=\"color:blue\">";
      case logger::yellow:
        return "<span style=\"color:yellow\">";
      default:
        return "";
    }
  }
  logger& operator << (const loggerSpecialSymbols& input)
  { switch (input)
    { case logger::endL:
        std::cout << this->closeTagConsole() << std::endl;
        theFile << this->closeTagHtml() << "\n<br>\n";
        theFile.flush();
        return *this;
      case logger::red:
      case logger::blue:
      case logger::yellow:
      case logger::green:
        this->theFile << this->closeTagHtml();
        this->currentColor=input;
        std::cout << this->openTagConsole();
        this->theFile << this->openTagHtml();
        theFile.flush();
        return *this;
      default:
        return *this;
    }
  }

  template <typename theType>
  logger& operator << (const theType& toBePrinted)
  { std::cout << toBePrinted;
    std::stringstream out;
    out << toBePrinted;
    theFile << out.str();
    return *this;
  }
};

logger theLog;
WebServer theWebServer;

const char* PORT ="8080";  // the port users will be connecting to
const int BACKLOG =10;     // how many pending connections queue will hold

void WebServer::Signal_SIGINT_handler(int s)
{ theLog << "Signal interrupt handler called with input: " << s;
//  << ". Waiting for children to exit... " << logger::endL;
  theWebServer.ReleaseActiveWorker();
  theWebServer.ReleaseNonActiveWorkers();
  while(waitpid(-1, NULL, WNOHANG) > 0)
  { }
  theLog << "All children have exited. " << logger::endL;
  exit(0);
}

void WebServer::Signal_SIGCHLD_handler(int s)
{ theLog << "Signal child handler called with input: " << s << "." << logger::endL;
  while(waitpid(-1, NULL, WNOHANG) > 0)
  { }
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{ if (sa->sa_family == AF_INET)
    return &(((struct sockaddr_in*)sa)->sin_addr);
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

std::string WebWorker::ToStringMessageShort(FormatExpressions* theFormat)const
{ std::stringstream out;
  bool useHtml=theFormat==0 ? false: theFormat->flagUseHTML;
  std::string lineBreak= useHtml ? "<br>\n" : "\r\n";
  out << lineBreak;
  if (this->requestType==this->requestTypeGetCalculator)
    out << "GET " << "(from calculator)";
  else if (this->requestType==this->requestTypePostCalculator)
    out << "POST " << "(from calculator)";
  else if (this->requestType==this->requestTypeGetNotCalculator)
    out << "GET " << "(NOT from calculator)";
  else if (this->requestType==this->requestTypeTogglePauseCalculator)
    out << "GET " << "(pause computation)";
  else if (this->requestType==this->requestTypeGetServerStatus)
    out << "GET (server status)";
  else if (this->requestType==this->requestTypeGetComputationIndicator)
    out << "GET (computation indicator)";
  else
    out << "Request type undefined.";
  out << "<hr>Main address raw: " << this->mainAddresSRAW;
  out << lineBreak << "Main address: " << this->mainAddress;
  out << lineBreak << "Main argument: " << this->mainArgumentRAW;
  out << lineBreak << "Physical file address referred to by main address: " << this->PhysicalFileName;
  out << lineBreak << "Display path server base: "
  << onePredefinedCopyOfGlobalVariables.DisplayPathServerBase;
  return out.str();
}

std::string WebWorker::ToStringMessageFull()const
{ std::stringstream out;
  out << this->ToStringMessage();
  if (this->theStrings.size>0)
  { out << "<hr>\nStrings extracted from message: ";
    for (int i =0; i<this->theStrings.size; i++)
      out << "<br>" << this->theStrings[i];
  }
  return out.str();
}

std::string WebWorker::ToStringMessage()const
{ std::stringstream out;
  FormatExpressions tempFormat;
  tempFormat.flagUseHTML=true;
  out << this->ToStringMessageShort(&tempFormat);

  out << "<hr>";
  out << "Main address RAW: " << this->mainAddresSRAW << "<br>";
  if (this->requestType==this->requestTypeGetCalculator ||
      this->requestType==this->requestTypeGetNotCalculator ||
      this->requestType==this->requestTypeGetServerStatus ||
      this->requestType==this->requestTypeGetComputationIndicator)
    out << "GET " << this->mainAddresSRAW;
  if (requestType==this->requestTypePostCalculator)
    out << "POST " << this->mainAddresSRAW;
  out << "\n<br>\nFull client message:\n" << this->theMessage;
  return out.str();
}

void WebWorker::resetMessageComponentsExceptRawMessage()
{ this->mainArgumentRAW="";
  this->mainAddress="";
  this->mainAddresSRAW="";
  this->PhysicalFileName="";
  this->theStrings.SetSize(0);
  this->requestType=this->requestTypeUnknown;
  this->ContentLength=-1;
  this->flagOutputTimedOut=false;
  this->flagTimedOutComputationIsDone=false;
}

void WebWorker::StandardOutputPart1BeforeComputation()
{ MacroRegisterFunctionWithName("WebServer::StandardOutputPart1BeforeComputation");
  onePredefinedCopyOfGlobalVariables.flagComputationComplete=false;

  stOutput << "<html><meta name=\"keywords\" content= \"Root system, Root system Lie algebra, Vector partition function calculator, vector partition functions, Semisimple Lie algebras, "
  << "Root subalgebras, sl(2)-triples\"> <head> <title>calculator version  " << __DATE__ << ", " << __TIME__ << "</title>";
  stOutput << "<script src=\"../jsmath/easy/load.js\"></script>\n</head>\n<body onload=\"checkCookie();\">\n";
  List<std::string> inputStrings, inputStringNames;
  CGI::ChopCGIInputStringToMultipleStrings(theParser.inputStringRawestOfTheRaw, inputStrings, inputStringNames);
  std::string& civilizedInput=theParser.inputString;
  if (inputStringNames.Contains("textInput"))
    civilizedInput= inputStrings[inputStringNames.GetIndex("textInput")];
  CGI::CivilizedStringTranslationFromCGI(civilizedInput, civilizedInput);
  PredefinedStrings(civilizedInput);

//  civilizedInput="\\int( 1/x dx)";
  crash.userInputStringIfAvailable=civilizedInput;

  std::stringstream tempStreamXX;
  static_html4(tempStreamXX);

  stOutput << tempStreamXX.str();
  stOutput << "<table>\n <tr valign=\"top\">\n <td>";
  stOutput << "\n<FORM method=\"POST\" id=\"formCalculator\" name=\"formCalculator\" action=\""
  << theParser.theGlobalVariableS->DisplayNameCalculatorWithPath << "\">\n" ;
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
    stOutput << "<a href=\"" << theParser.theGlobalVariableS->DisplayNameCalculatorWithPath << "?" << theParser.inputStringRawestOfTheRaw << "\">Link to your input.</a>";
  stOutput << "\n</FORM>";
  //  stOutput << "<br>Number of lists created before evaluation: " << NumListsCreated;
}

void WebWorker::StandardOutputPart2ComputationTimeout()
{ MacroRegisterFunctionWithName("WebWorker::StandardOutputPart2ComputationTimeout");
  theWebServer.GetActiveWorker().flagTimedOutComputationIsDone=true;
  std::stringstream out;
  out << "<table><tr><td>" << theParser.outputString << "</td><td><b>Comments</b>"
  << theParser.outputCommentsString << "</td></tr></table>";
  //clearing the pipe which will signal our data has been received:
  theWebServer.GetActiveWorker().pipeServerToWorkerComputationReportReceived.Read(true);
  //making sure we are not paused:
//  theWebServer.GetActiveWorker().pipeServerToWorkerEmptyingPausesWorker.Read(false);
//  theWebServer.GetActiveWorker().pipeServerToWorkerEmptyingPausesWorker.Write("!", false);
  theLog << logger::red << "Writing final output to indicator." << logger::endL;
  theWebServer.GetActiveWorker().pipeWorkerToServerIndicatorData.WriteAfterClearingPipe(out.str(), false);
  theLog << logger::red << "Final output written to indicator, blocking until data is received." << logger::endL;
  theWebServer.GetActiveWorker().pipeServerToWorkerComputationReportReceived.Read(false);
  theLog << logger::red << "We are unblocked (data was received), writing \"finished\"." << logger::endL;
  theWebServer.GetActiveWorker().pipeWorkerToServerIndicatorData.WriteAfterClearingPipe("finished", false);
  theLog << logger::red << "\"finished\" written to indicator, blocking until received." << logger::endL;
  theWebServer.GetActiveWorker().pipeServerToWorkerComputationReportReceived.Read(false);
}

void WebWorker::StandardOutputPart2StandardExit()
{ MacroRegisterFunctionWithName("WebServer::StandardOutputPart2StandardExit");
  if (theParser.inputString!="")
  { if (theParser.flagProduceLatexLink)
      stOutput << "<br>LaTeX link (\\usepackage{hyperref}):<br> "
      << CGI::GetLatexEmbeddableLinkFromCalculatorInput(theParser.inputStringRawestOfTheRaw, theParser.inputString);
    stOutput << theParser.outputString;
    if (theParser.parsingLog!="")
      stOutput << "<b> As requested, here is a calculator parsing log</b><br>" << theParser.parsingLog;
  }
  stOutput << "\n\n</td>\n\n";
//  bool displayClientMessage=theWebServer.flagUsingBuiltInServer && theWebServer.activeWorker!=-1;
  //displayClientMessage=false;
  if (theParser.outputCommentsString!="")
  { stOutput << "<td valign=\"top\">";
//    if (displayClientMessage)
//      stOutput << "<b>Message from client: </b>" << theWebServer.GetActiveWorker().ToStringMessageFull() << "<hr>";
    //if (theParser.outputCommentsString.size()<10000)
    stOutput << theParser.outputCommentsString;
    //else
    //stOutput << "The comments generated by your computation are a bit too long. If you want to see them click the show/hide button below.<br>"
    //<< CGI::GetHtmlSpanHidableStartsHiddeN(theParser.outputCommentsString);
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
}

void WebWorker::ExtractArgumentFromAddress()
{ MacroRegisterFunctionWithName("WebWorker::ExtractArgumentFromAddress");
//  theLog << "\nmain address:" << this->mainAddress << "=?="
//  << onePredefinedCopyOfGlobalVariables.DisplayNameCalculatorWithPath << "\nmainaddress.size: "
//  << this->mainAddress.size() << "\nonePredefinedCopyOfGlobalVariables.DisplayNameCalculatorWithPath.size(): "
//  << onePredefinedCopyOfGlobalVariables.DisplayNameCalculatorWithPath.size();
  CGI::CivilizedStringTranslationFromCGI(this->mainAddresSRAW, this->mainAddress);
  this->mainArgumentRAW="";
  std::string calculatorArgumentRawWithQuestionMark, tempS;
  if (!MathRoutines::StringBeginsWith(this->mainAddresSRAW, onePredefinedCopyOfGlobalVariables.DisplayNameCalculatorWithPath, &calculatorArgumentRawWithQuestionMark))
    return;
  this->requestType=this->requestTypeGetCalculator;
  MathRoutines::SplitStringInTwo(calculatorArgumentRawWithQuestionMark, 1, tempS, this->mainArgumentRAW);
  theLog << logger::yellow << "this->mainArgumentRAW=" << this->mainArgumentRAW << logger::endL;
  if (MathRoutines::StringBeginsWith(this->mainArgumentRAW, "indicator"))
    this->requestType=this->requestTypeGetComputationIndicator;
  if (MathRoutines::StringBeginsWith(this->mainArgumentRAW, "status"))
    this->requestType=this->requestTypeGetServerStatus;
  if (MathRoutines::StringBeginsWith(this->mainArgumentRAW, "pauseIndicator"))
    this->requestType=this->requestTypeTogglePauseCalculator;
}

void WebWorker::ParseMessage()
{ MacroRegisterFunctionWithName("WebWorker::ParseMessage");
  this->resetMessageComponentsExceptRawMessage();
  std::string buffer;
  buffer.reserve(this->theMessage.size());
  this->theStrings.SetExpectedSize(20);
  for (unsigned i =0; i<this->theMessage.size(); i++)
    if (theMessage[i]!=' ' && theMessage[i]!='\n' && theMessage[i]!='\r')
    { buffer.push_back(this->theMessage[i]);
      if (i==this->theMessage.size()-1)
        this->theStrings.AddOnTop(buffer);
    } else
    { if (buffer!="")
      { this->theStrings.AddOnTop(buffer);
        buffer="";
      }
      if (i>0)
        if (theMessage[i]=='\n' && theMessage[i-1]=='\r')
          this->theStrings.AddOnTop("!CRLF!");
    }
  for (int i=0; i<this->theStrings.size; i++)
    if (this->theStrings[i]=="GET")
    { this->requestType=this->requestTypeGetNotCalculator;
      i++;
      if (i<this->theStrings.size)
      { this->mainAddresSRAW=this->theStrings[i];
        this->ExtractArgumentFromAddress();
      }
    } else if (this->theStrings[i]=="POST")
    { this->requestType=this->requestTypePostCalculator;
      i++;
      if (i<this->theStrings.size)
      { this->mainAddresSRAW=this->theStrings[i];
        if (*this->theStrings.LastObject()!="!CRLF!")
          this->mainArgumentRAW=*this->theStrings.LastObject();
        else
          this->mainArgumentRAW="";
      }
    } else if ((this->theStrings[i]=="Content-Length:" || this->theStrings[i]=="Content-length:" ||
                this->theStrings[i]=="content-length:")
               && i+1<this->theStrings.size)
    { if (this->theStrings[i+1].size()<10000)
      { LargeIntUnsigned theLI;
        if (theLI.AssignStringFailureAllowed(this->theStrings[i+1], true))
          if (!theLI.IsSmallEnoughToFitInInt(&this->ContentLength))
            this->ContentLength=-1;
      }
    }
}

void WebWorker::QueueBytesForSending(const List<char>& bytesToSend, bool MustSendAll)
{ MacroRegisterFunctionWithName("WebWorker::QueueBytesForSending");
  this->remainingBytesToSend.AddListOnTop(bytesToSend);
  if (this->remainingBytesToSend.size>=1024*512 || MustSendAll)
    this->SendAllBytes();
}

void WebWorker::QueueStringForSending(const std::string& stringToSend, bool MustSendAll)
{ MacroRegisterFunctionWithName("WebWorker::SendStringToSocket");
  int oldSize=this->remainingBytesToSend.size;
  this->remainingBytesToSend.SetSize(this->remainingBytesToSend.size+stringToSend.size());
  for (unsigned i=0; i<stringToSend.size(); i++)
    this->remainingBytesToSend[i+oldSize]=stringToSend[i];
//  if (stringToSend.size()>0)
//    if (stringToSend[stringToSend.size()-1]=='\0')
//      theLog << "WARNING: string is null terminated!\r\n";
//    theLog << "Last character string: " << *this->remainingBytesToSend.LastObject() << "\r\n";
  if (this->remainingBytesToSend.size>=1024*512 || MustSendAll)
    this->SendAllBytes();
}

void WebWorker::SendAllBytes()
{ if (this->remainingBytesToSend.size==0)
    return;
  MacroRegisterFunctionWithName("WebWorker::SendAllBytes");
  if (this->connectedSocketID==-1)
  { theLog << logger::red << ("Socket::SendAllBytes failed: connectedSocketID=-1.") << logger::endL;
    return;
  }
  theLog << "Sending " << this->remainingBytesToSend.size << " bytes in chunks of: " << logger::endL;
  //  theLog << "\r\nIn response to: " << this->theMessage;
  while (this->remainingBytesToSend.size>0)
  { int numBytesSent=send(this->connectedSocketID, &this->remainingBytesToSend[0], this->remainingBytesToSend.size,0);
    if (numBytesSent<0)
    { theLog << "WebWorker::SendAllBytes failed. Error: " << this->parent->ToStringLastErrorDescription() << logger::endL;
      return;
    }
    theLog << numBytesSent;
    this->remainingBytesToSend.Slice(numBytesSent, this->remainingBytesToSend.size-numBytesSent);
    if (this->remainingBytesToSend.size>0)
      theLog << ", ";
    theLog << logger::endL;
  }
}

bool WebWorker::CheckConsistency()
{ stOutput.theOutputFunction=0;
  if (this->parent==0)
    crash << "Parent of web worker is not initialized." << crash;
  if (this->indexInParent==-1)
    crash << "Index in parent is bad. " << crash;
  if (this->connectionID==-1)
    crash << "Connection id is bad. " << crash;
  stOutput.theOutputFunction=WebServer::SendStringThroughActiveWorker;
  return true;
}

bool WebWorker::ReceiveAll()
{ MacroRegisterFunctionWithName("WebWorker::ReceiveAll");
  unsigned const int bufferSize=60000;
  char buffer[bufferSize];
  if (this->connectedSocketID==-1)
    crash << "Attempting to receive on a socket with ID equal to -1. " << crash;
  int numBytesInBuffer= recv(this->connectedSocketID, &buffer, bufferSize-1, 0);
  if (numBytesInBuffer<0)
  { theLog << "Socket::ReceiveAll on socket " << this->connectedSocketID << " failed. Error: "
    << this->parent->ToStringLastErrorDescription() << logger::endL;
    return false;
  }
  this->theMessage.assign(buffer, numBytesInBuffer);
//  theLog << this->parent->ToStringStatusActive() << ": received " << numBytesInBuffer << " bytes. " << logger::endL;
  this->ParseMessage();
//  theLog << "Content length computed to be: " << this->ContentLength;
  if (this->ContentLength<=0)
    return true;
  if (this->mainArgumentRAW.size()==(unsigned) this->ContentLength)
    return true;
//  theLog << "Content-length parsed to be: " << this->ContentLength
//  << "However the size of mainArgumentRAW is: " << this->mainArgumentRAW.size();
  if (this->ContentLength>10000000)
  { error="Content-length parsed to be more than 10 million bytes, aborting.";
    theLog << this->error << logger::endL;
    return false;
  }
  if (this->mainArgumentRAW!="")
  { error= "Content-length does not coincide with the size of the message-body, yet the message-body is non-empty. Aborting.";
    theLog << this->error << logger::endL;
    return false;
  }
  this->remainingBytesToSend=(std::string) "HTTP/1.1 100 Continue\r\n";
  this->SendAllBytes();
  this->remainingBytesToSend.SetSize(0);
  this->mainArgumentRAW="";
  std::string bufferString;
  while ((signed) this->mainArgumentRAW.size()<this->ContentLength)
  { numBytesInBuffer= recv(this->connectedSocketID, &buffer, bufferSize-1, 0);
    if (numBytesInBuffer==0)
    { this->error= "While trying to fetch message-body, received 0 bytes. " +
      this->parent->ToStringLastErrorDescription();
      return false;
    }
    if (numBytesInBuffer<0)
    { theLog << "Error fetching message body: " << this->parent->ToStringLastErrorDescription() << logger::endL;
      return false;
    }
    bufferString.assign(buffer, numBytesInBuffer);
    this->mainArgumentRAW+=bufferString;
  }
  if ((signed) this->mainArgumentRAW.size()!=this->ContentLength)
  { std::stringstream out;
    out << "The message-body received by me had length " << this->mainArgumentRAW.size()
    << " yet I expected a message of length " << this->ContentLength << ".";
    this->error=out.str();
    theLog << this->error << logger::endL;
    return false;
  }
  return true;
}

void WebWorker::ExtractPhysicalAddressFromMainAddress()
{ MacroRegisterFunctionWithName("WebWorker::ExtractPhysicalAddressFromMainAddress");
  int numBytesToChop=onePredefinedCopyOfGlobalVariables.DisplayPathServerBase.size();
  std::string displayAddressStart= this->mainAddress.substr(0, numBytesToChop);
  if (displayAddressStart!=onePredefinedCopyOfGlobalVariables.DisplayPathServerBase)
    numBytesToChop=0;
  this->PhysicalFileName=onePredefinedCopyOfGlobalVariables.PhysicalPathServerBase+
  this->mainAddress.substr(numBytesToChop, std::string::npos);
}

int WebWorker::ProcessGetRequestServerStatus()
{ MacroRegisterFunctionWithName("WebWorker::ProcessGetRequestServerStatus");
  stOutput << "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  stOutput << "<html><body> " << this->parent->ToStringStatusAll() << "</body></html>";
  return 0;
}

int WebWorker::ProcessPauseWorker()
{ MacroRegisterFunctionWithName("WebWorker::ProcessPauseWorker");
  theLog << "Proceeding to toggle worker pause." << logger::endL;
  stOutput << "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  if (this->mainArgumentRAW.size()<=14)
  { stOutput << "<b>Indicator takes as argument the id of the child process that is running the computation.</b>";
    return 0;
  }
  int inputWebWorkerNumber= atoi(this->mainArgumentRAW.substr(14, std::string::npos).c_str());
  int inputWebWorkerIndex= inputWebWorkerNumber-1;
  if (inputWebWorkerIndex<0 || inputWebWorkerIndex>=this->parent->theWorkers.size)
  { stOutput << "<b>Indicator requested " << this->mainArgumentRAW
    << " (extracted worker number " << inputWebWorkerNumber << " out of "
    << this->parent->theWorkers.size << ") but the id is out of range. </b>";
    return 0;
  }
  if (!this->parent->theWorkers[inputWebWorkerIndex].flagInUse)
  { stOutput << "<b>Requested worker number " << inputWebWorkerNumber << " is not in use. "
    << "Total number of workers: " << this->parent->theWorkers.size << ". </b>";
    return 0;
  }
  WebWorker& otherWorker=this->parent->theWorkers[inputWebWorkerIndex];
  theLog << "About to read pipeServerToWorker..." << logger::endL;
  otherWorker.pipeServerToWorkerEmptyingPausesWorker.Read(true);
  if (otherWorker.pipeServerToWorkerEmptyingPausesWorker.lastRead.size>0)
  { theLog << logger::yellow << "Successfully read pipeServerToWorkerEmptyingPausesWorker, result was NON EMPTY"
    << logger::endL;
    stOutput << "paused";
    return 0;
  }
  theLog << logger::green << "Successfully read pipeServerToWorkerEmptyingPausesWorker, result was EMPTY"
  << logger::endL;
  stOutput << "unpaused";
  otherWorker.pipeServerToWorkerEmptyingPausesWorker.Write("!", false);
  return 0;
}

int WebWorker::ProcessGetRequestComputationIndicator()
{ MacroRegisterFunctionWithName("WebWorker::ProcessGetRequestComputationIndicator");
  theLog << "Processing get request indicator." << logger::endL;
  stOutput << "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  if (this->mainArgumentRAW.size()<=9)
  { stOutput << "<b>Indicator takes as argument the id of the child process that is running the computation.</b>";
    return 0;
  }
  int inputWebWorkerNumber= atoi(this->mainArgumentRAW.substr(9, std::string::npos).c_str());
  int inputWebWorkerIndex= inputWebWorkerNumber-1;
  if (inputWebWorkerIndex<0 || inputWebWorkerIndex>=this->parent->theWorkers.size)
  { stOutput << "<b>Indicator requested " << this->mainArgumentRAW
    << " (extracted worker number " << inputWebWorkerNumber << " out of "
    << this->parent->theWorkers.size << ") but the id is out of range. </b>";
    return 0;
  }
  if (!this->parent->theWorkers[inputWebWorkerIndex].flagInUse)
  { stOutput << "<b>Requested worker number " << inputWebWorkerNumber << " is not in use. "
    << "Total number of workers: " << this->parent->theWorkers.size << ". </b>";
    return 0;
  }
  WebWorker& otherWorker=this->parent->theWorkers[inputWebWorkerIndex];
//  theLog << "Worker " << this->parent->activeWorker
//  << consoleYellow(" piping 'indicator'" ) << logger::endL;
  otherWorker.pipeServerToWorkerRequestIndicator.WriteAfterClearingPipe("!", true);
//  theLog << "'indicator' piped, waiting for return." << logger::endL;
  otherWorker.pipeWorkerToServerIndicatorData.Read(true);
//  theLog << "indicator returned." << logger::endL;
  if (otherWorker.pipeWorkerToServerIndicatorData.lastRead.size>0)
  { std::string outputString;
    outputString.assign(otherWorker.pipeWorkerToServerIndicatorData.lastRead.TheObjects, otherWorker.pipeWorkerToServerIndicatorData.lastRead.size);
    theLog << logger::yellow << "Indicator string read: " << logger::blue << outputString << logger::endL;
    stOutput << outputString;
    otherWorker.pipeServerToWorkerComputationReportReceived.WriteAfterClearingPipe("!", true);
  }
//  stOutput << "<b>Not implemented: request for indicator for worker " << inputWebWorkerNumber
//  << " out of " << this->parent->theWorkers.size << ".</b>";
  return 0;
}

void WebWorker::PipeProgressReportToParentProcess(const std::string& input)
{ MacroRegisterFunctionWithName("WebWorker::PipeProgressReportToParentProcess");
  this->pipeServerToWorkerRequestIndicator.Read(true);
  if (this->pipeServerToWorkerRequestIndicator.lastRead.size==0)
    return;
  if (this->flagTimedOutComputationIsDone)
    return;
  theLog << "about to potentially block " << logger::endL;
  this->pipeServerToWorkerEmptyingPausesWorker.Read(false);     //if pause was requested, here we block
  theLog << "block skipped" << logger::endL;
  this->pipeServerToWorkerEmptyingPausesWorker.Write("!", false);
  theLog << " data written!";
  this->pipeWorkerToServerIndicatorData.Write(input, true);
}

int WebWorker::ProcessGetRequestFolder()
{ MacroRegisterFunctionWithName("WebWorker::ProcessGetRequestFolder");
  std::stringstream out;
  out << "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
  << "<html><body>";
  //out << this->ToString();
  List<std::string> theFileNames, theFileTypes;
  if (!FileOperations::GetFolderFileNames(this->PhysicalFileName, theFileNames, &theFileTypes))
  { out << "<b>Failed to open directory with physical address " << this->PhysicalFileName
    << " </b></body></html>";
    stOutput << out.str();
    return 0;
  }
  out << "Browsing folder: " << this->mainAddress << "<br>Physical address: " << this->PhysicalFileName << "<hr>";
  for (int i=0; i<theFileNames.size; i++)
  { bool isDir= theFileTypes[i]==".d";
    out << "<a href=\"" << this->mainAddress << theFileNames[i];
    if (isDir)
      out << "/";
    out << "\">" << theFileNames[i];
    if (isDir)
      out << "/";
    out << "</a><br>";
  }
  out << "</body></html>";
  stOutput << out.str();
  return 0;
}

void WebWorker::reset()
{ this->connectedSocketID=-1;
  this->connectedSocketIDLastValueBeforeRelease=-1;
  this->connectionID=-1;
  this->indexInParent=-1;
  this->parent=0;
  this->flagInUse=false;
  this->flagOutputTimedOut=false;
  this->flagTimedOutComputationIsDone=false;
  this->requestType=this->requestTypeUnknown;
  this->pipeServerToWorkerEmptyingPausesWorker.Release();
  this->pipeWorkerToServerControls.Release();
  this->pipeServerToWorkerRequestIndicator.Release();
  this->pipeServerToWorkerComputationReportReceived.Release();
  this->pipeWorkerToServerIndicatorData.Release();
}

WebWorker::WebWorker()
{ this->reset();
}

bool WebWorker::IamActive()
{ if (this->parent==0 || this->indexInParent==-1)
    return false;
  return this->parent->activeWorker==this->indexInParent;
}

bool WebWorker::IsFileExtensionOfBinaryFile(const std::string& fileExtension)
{ if (fileExtension==".png")
    return true;

  return false;
}

void WebWorker::SignalIamDoneReleaseEverything()
{ MacroRegisterFunctionWithName("WebWorker::SignalIamDoneReleaseEverything");
  if (!this->IamActive())
  { theLog << logger::red << ("Signal done called on non-active worker") << logger::endL;
    return;
  }
  theLog << logger::blue << ("worker ") << this->indexInParent+1 << logger::blue << (" is done with the work. ") << logger::endL;
  this->pipeWorkerToServerControls.WriteAfterClearingPipe("close", false);
  theLog << logger::blue << ("Notification dispatched.") << logger::endL;
  this->SendAllBytes();
  this->Release();
}

WebWorker::~WebWorker()
{ //Workers are not allowed to release resources in the destructor:
  //a Worker's destructor is called when expanding List<WebWorker>.
}

std::string WebWorker::GetMIMEtypeFromFileExtension(const std::string& fileExtension)
{ MacroRegisterFunctionWithName("WebWorker::GetMIMEtypeFromFileExtension");
  if (fileExtension==".html")
    return "Content-Type: text/html\r\n";
  if (fileExtension==".txt")
    return "Content-Type: text/plain\r\n";
  if (fileExtension==".png")
    return "Content-Type: image/png\r\n";
  if (fileExtension==".js")
    return "Content-Type: text/javascript\r\n";
  if (fileExtension==".ico")
    return "Content-Type: image/x-icon\r\n";
  return "Content-Type: application/octet-stream\r\n";
}

int WebWorker::ProcessGetRequestNonCalculator()
{ MacroRegisterFunctionWithName("WebWorker::ProcessGetRequestNonCalculator");
  this->ExtractPhysicalAddressFromMainAddress();
  //theLog << this->ToStringShort() << "\r\n";
  if (FileOperations::IsFolder(this->PhysicalFileName))
    return this->ProcessGetRequestFolder();
  if (!FileOperations::FileExists(this->PhysicalFileName))
  { stOutput << "HTTP/1.1 404 Object not found\r\n";
    stOutput << "Content-Type: text/html\r\n\r\n";
    stOutput << "<html><body>";
    stOutput << "<b>File does not exist.</b><br><b> File display name:</b> " << this->mainAddress
    << "<br><b>File physical name:</b> " << this->PhysicalFileName;
    stOutput << "<hr><hr><hr>Message details:<br>" <<  this->ToStringMessage();
    stOutput << "</body></html>";
    return 0;
  }
  std::string fileExtension=FileOperations::GetFileExtensionWithDot(this->PhysicalFileName);
  bool isBinary=this->IsFileExtensionOfBinaryFile(fileExtension);
  std::fstream theFile;
  if (!FileOperations::OpenFile(theFile, this->PhysicalFileName, false, false, !isBinary))
  { stOutput << "HTTP/1.1 200 OK\r\n" << "Content-Type: text/html\r\n";
    stOutput << "\r\n"
    << "<html><body><b>Error: file appears to exist but I could not open it.</b> "
    << "<br><b> File display name: </b>"
    << this->mainAddress << "<br><b>File physical name: </b>"
    << this->PhysicalFileName << "</body></html>";
    return 0;
  }
  theFile.seekp(0, std::ifstream::end);
  unsigned int fileSize=theFile.tellp();
  std::stringstream theHeader;
  theHeader << "HTTP/1.1 200 OK\r\n" << this->GetMIMEtypeFromFileExtension(fileExtension)
  << "Content-length: " << fileSize << "\r\n\r\n";
  this->QueueStringForSending(theHeader.str());
  const int bufferSize=64*1024;
  this->bufferFileIO.SetSize(bufferSize);
  theFile.seekg(0);
  theFile.read(&this->bufferFileIO[0], this->bufferFileIO.size);
  int numBytesRead=theFile.gcount();
  ///////////////////
//  theLog << "*****Message summary begin\r\n" << theHeader.str();
//  theLog << "Sending file  " << this->PhysicalFileName; << " with file extension " << fileExtension
//  << ", file size: " << fileSize;
//  theLog << "\r\n*****Message summary end\r\n";
  ///////////////////
  while (numBytesRead!=0)
  { this->bufferFileIO.SetSize(numBytesRead);
    this->QueueBytesForSending(this->bufferFileIO);
    this->bufferFileIO.SetSize(bufferSize);
    theFile.read(&this->bufferFileIO[0], this->bufferFileIO.size);
    numBytesRead=theFile.gcount();
  }
  this->SendAllBytes();
  return 0;
}

int WebWorker::ProcessRequestTypeUnknown()
{ MacroRegisterFunctionWithName("WebWorker::ProcessRequestTypeUnknown");
  stOutput << "HTTP/1.1 501 Method Not Implemented\r\n";
  stOutput << "Content-Type: text/html\r\n\r\n"
  << "<b>Requested method is not implemented. </b> <hr>The original message received from the server follows."
  << "<hr>\n" << this->ToStringMessage();
  return 0;
}

int WebWorker::StandardOutput()
{ MacroRegisterFunctionWithName("WebServer::StandardOutput");
  WebWorker::StandardOutputPart1BeforeComputation();
  theWebServer.CheckExecutableVersionAndRestartIfNeeded();
  stOutput << theParser.javaScriptDisplayingIndicator;
//  theParser.inputString="%LogEvaluation 3 *3^{1/2}";
  if (theParser.inputString!="")
  { theParser.Evaluate(theParser.inputString);
    onePredefinedCopyOfGlobalVariables.flagComputationComplete=true;
  }
  if (theWebServer.flagUsingBuiltInServer)
    if (theWebServer.GetActiveWorker().flagOutputTimedOut)
    { WebWorker::StandardOutputPart2ComputationTimeout();
      return 0;
    }
  WebWorker::StandardOutputPart2StandardExit();
  stOutput.Flush();
  //exit(0);
  return 0;
}

std::string WebWorker::GetJavaScriptIndicatorFromHD()
{ std::stringstream out;
  out << " <!>\n";
  out << " <script type=\"text/javascript\"> \n";
  out << " var timeOutCounter=0;\n";
//  out << " var newReportString=\"\";\n";
  out << " var showProgress=false;";
  out << " function progressReport()\n";
  out << "{ var el = document.getElementById(\"idProgressReport\");	\n";
  out << "  if (!showProgress) \n";
  out << "  { el.style.display = 'none';\n";
  out << "    return;";
  out << "  }\n";
  out << "  el.style.display = '';\n";
//  out << "  el.contentWindow.location.reload();";
  out << "  timeOutCounter++;\n";
  out << "  var oRequest = new XMLHttpRequest();\n";
  out << "  var sURL  = \"" << onePredefinedCopyOfGlobalVariables.DisplayNameIndicatorWithPath << "\";\n";
  out << "  oRequest.open(\"GET\",sURL,false);\n";
//  out << "  oRequest.setRequestHeader(\"Indicator\",navigator.userAgent);\n";
  out << "  oRequest.send(null)\n";
  out << "  if (oRequest.status==200)\n";
  out << "  { newReportString= oRequest.responseText;\n";
  out << "    el.innerHTML= \"<hr>Refreshing each second. Client time: ~\"+ timeOutCounter+\" second(s)<br>\" +newReportString+\"<hr>\";\n";
  out << "  }\n";
  out << "   window.setTimeout(\"progressReport()\",1000);\n";
  out << " }\n";
  out << " </script>\n";
  out << CGI::GetHtmlButton
  ("progressReportButton", "showProgress=!showProgress; progressReport()", "expand/collapse progress report");
  out << "<br><div "
  //<< "src=\"" << this->indicatorFileNameDisplaY << "\" "
  << "id=\"idProgressReport\" style=\"display:none\">\n";
  out << " </div>\n";
  out << " \n";
  out << " \n";
  return out.str();
}

std::string WebWorker::GetJavaScriptIndicatorBuiltInServer()
{ MacroRegisterFunctionWithName("WebWorker::GetJavaScriptIndicatorBuiltInServer");
  std::stringstream out;
  out << " <!>\n";
//  out << "\n<br>\n<button onclick=\"progressReport()\">Manual report</button>";
  out << "\n<br>\n<button id=\"idButtonSendTogglePauseRequest\" onclick=\"SendTogglePauseRequest()\">Pause</button>";
  out << "<span id=\"idPauseToggleServerResponse\"></span>\n";
  out << "<span id=\"idProgressReportTimer\"></span>\n";
  out << "\n<br>\n<div id=\"idProgressReport\"></div>\n";
  out << "\n<script type=\"text/javascript\"> \n";
  out << "var isPaused=false;\n";
  out << "var isFinished=false;\n";
  out << "var timeIncrementInTenthsOfSecond=4;//measured in tenths of a second\n";
  out << "var timeOutCounter=0;//measured in tenths of a second\n";
  out << "progressReport();\n";
  out << "function SendTogglePauseRequest()\n";
  out << "{ if (isFinished)\n";
  out << "    return;\n";
  out << "  var pauseRequest = new XMLHttpRequest();\n";
  out << "  pauseURL  = \"" << onePredefinedCopyOfGlobalVariables.DisplayNameCalculatorWithPath
  << "?pauseIndicator" << this->indexInParent+1 << "\";\n";
  out << "  pauseRequest.open(\"GET\",pauseURL,false);\n";
//  out << "  oRequest.setRequestHeader(\"Indicator\",navigator.userAgent);\n";
  out << "  pauseRequest.send(null)\n";
  out << "  if (pauseRequest.status!=200)\n";
  out << "    return;\n";
  out << "  isPaused=(pauseRequest.responseText==\"paused\");\n";
  out << "  if (isPaused)\n";
  out << "    document.getElementById(\"idButtonSendTogglePauseRequest\").innerHTML=\"Continue\";\n";
  out << "  else\n";
  out << "    document.getElementById(\"idButtonSendTogglePauseRequest\").innerHTML=\"Pause\";\n";
  out << "  document.getElementById(\"idPauseToggleServerResponse\").innerHTML=pauseRequest.responseText;\n";
  out << "  if (!isPaused)\n";
  out << "    progressReport();\n";
  out << "}\n";
//  out << " progressReport();";
//  out << " var newReportString=\"\";\n";
  out << "\nfunction progressReport()\n";
  out << "{ if (isFinished)\n";
  out << "    return;\n";
  out << "  var progReport = document.getElementById(\"idProgressReport\");	\n";
  out << "  var progReportTimer = document.getElementById(\"idProgressReportTimer\");	\n";
  out << "  if(isPaused)\n";
  out << "    return;\n";
  out << "  progReportTimer.innerHTML =\"<hr>Refreshing every \"+timeIncrementInTenthsOfSecond/10+\" second(s). Client time: ~\"+ Math.floor(timeOutCounter/10)+\" second(s)<br>\";\n";
  out << "  progReportTimer.style.display = '';\n";
  out << "  progReport.style.display = '';\n";
//  out << "  el.contentWindow.location.reload();";
  out << "  timeOutCounter+=timeIncrementInTenthsOfSecond;\n";
  out << "  var oRequest = new XMLHttpRequest();\n";
  if (this->indexInParent==-1)
    theLog << logger::red << ("Worker index in parent is -1!!!") << logger::endL;
  else
    theLog << logger::yellow << ("Worker index: ") << this->indexInParent << logger::endL;

  out << "  var sURL  = \"" << onePredefinedCopyOfGlobalVariables.DisplayNameCalculatorWithPath << "?indicator"
  << this->indexInParent+1 << "\";\n";
  out << "  oRequest.open(\"GET\",sURL,false);\n";
//  out << "  oRequest.setRequestHeader(\"Indicator\",navigator.userAgent);\n";
  out << "  oRequest.send(null)\n";
  out << "  if (oRequest.status==200)\n";
  out << "  { newReportString= oRequest.responseText;\n";
  out << "    if (oRequest.responseText==\"finished\")\n";
  out << "    { isFinished=true;\n";
  out << "      document.getElementById(\"idPauseToggleServerResponse\").innerHTML=\"Computation finished.\";\n";
  out << "      return;\n";
  out << "    }\n";
  out << "    if (oRequest.responseText!=\"\")\n";
  out << "      progReport.innerHTML=newReportString+\"<hr>\";\n";
  out << "  }\n";
  out << "   window.setTimeout(\"progressReport()\",timeIncrementInTenthsOfSecond*100);\n";
  out << " }\n";
  out << " </script>\n";
  out << " \n";
  out << " \n";
  return out.str();
}

int WebWorker::ServeClient()
{ MacroRegisterFunctionWithName("WebServer::ServeClient");
  onePredefinedCopyOfGlobalVariables.IndicatorStringOutputFunction=WebServer::PipeProgressReportToParentProcess;
  if (this->requestType!=this->requestTypeGetComputationIndicator &&
      this->requestType!=this->requestTypeGetServerStatus &&
      this->requestType!=this->requestTypeTogglePauseCalculator)
    this->parent->ReleaseNonActiveWorkers();
    //unless the worker is an indicator, it has no access to communication channels of the other workers
  if (this->requestType==this->requestTypeGetCalculator || this->requestType==this->requestTypePostCalculator)
  { stOutput << "HTTP/1.1 200 OK\n";
    stOutput << "Content-Type: text/html\r\n\r\n";
    theParser.inputStringRawestOfTheRaw=this->mainArgumentRAW;
//    theParser.javaScriptDisplayingIndicator=this->GetJavaScriptIndicatorBuiltInServer(true);
    theParser.javaScriptDisplayingIndicator="";
    this->StandardOutput();
  } else if (this->requestType==this->requestTypeGetNotCalculator)
    this->ProcessGetRequestNonCalculator();
  else if (this->requestType==this->requestTypeTogglePauseCalculator)
    this->ProcessPauseWorker();
  else if (this->requestType==this->requestTypeGetServerStatus)
  { this->ProcessGetRequestServerStatus();
    this->parent->ReleaseNonActiveWorkers();
  } else if (this->requestType==this->requestTypeGetComputationIndicator)
  { this->ProcessGetRequestComputationIndicator();
    this->parent->ReleaseNonActiveWorkers();
  } else if (this->requestType==this->requestTypeUnknown)
    this->ProcessRequestTypeUnknown();
  this->SignalIamDoneReleaseEverything();
  return 0;
}

void WebWorker::ReleaseServerSideResources()
{ this->parent->Release(this->parent->listeningSocketID);//worker has no access to socket listener
}

void WebWorker::ReleaseKeepInUseFlag()
{ MacroRegisterFunctionWithName("WebServer::ReleaseMyPipe");
  this->pipeServerToWorkerEmptyingPausesWorker.Release();
  this->pipeWorkerToServerControls.Release();
  this->pipeServerToWorkerRequestIndicator.Release();
  this->pipeServerToWorkerComputationReportReceived.Release();
  this->pipeWorkerToServerIndicatorData.Release();
  WebServer::Release(this->connectedSocketID);
}

void WebWorker::Release()
{ this->ReleaseKeepInUseFlag();
  this->flagInUse=false;
}

void WebWorker::StandardOutputReturnIndicatorWaitForComputation()
{ MacroRegisterFunctionWithName("WebServer::StandardOutputReturnIndicatorWaitForComputation");
  this->flagOutputTimedOut=true;
  this->flagTimedOutComputationIsDone=false;
  stOutput << this->GetJavaScriptIndicatorBuiltInServer()
//  << "<script language=\"javascript\">progressReport();</script>"
  << "</td></tr></table></body></html>";
  theLog << logger::red << ("Indicator: sending all bytes") << logger::endL;
  this->SendAllBytes();
  theLog << logger::blue << ("Indicator: sending all bytes DONE") << logger::endL;
  for (int i=0; i<this->parent->theWorkers.size; i++)
    if (i!=this->indexInParent)
      this->parent->theWorkers[i].Release();
  //this->parent->Release(this->pipeServerToWorkerControls[0]);
  //this->parent->Release(this->pipeServerToWorkerControls[1]);
  //this->parent->Release(this->pipeWorkerToServerControls[0]);
  //this->parent->Release(this->pipeWorkerToServerControls[1]);
//  this->parent->Release(this->pipeServerToWorkerIndicator[0]);
//  this->parent->Release(this->pipeServerToWorkerIndicator[1]);
//  this->parent->Release(this->pipeWorkerToServerIndicator[0]);
//  this->parent->Release(this->pipeWorkerToServerIndicator[1]);
  this->parent->Release(this->connectedSocketID);

//  this->SignalIamDoneReleaseEverything();
//  theLog << consoleGreen("Indicator: released everything and signalled end.") << logger::endL;
}

std::string WebWorker::ToStringStatus()const
{ std::stringstream out;
  out << "worker " << this->indexInParent+1;
  if (this->flagInUse)
    out << ", <b>in use</b>";
  else
    out << ", not in use";
  out << ", connection " << this->connectionID << ", process ID: ";
  if (this->ProcessPID!=0)
    out << this->ProcessPID;
  else
    out << "(not accessible)";
  out << ", socketID: ";
  if (this->connectedSocketID==-1)
    out << "released in current process, value before release: " << this->connectedSocketIDLastValueBeforeRelease;
  else
    out << this->connectedSocketID;
  out << ", pipe indices: worker to server controls: " << this->pipeWorkerToServerControls.ToString()
  << ", server to worker pause signal pipe:  " << this->pipeServerToWorkerEmptyingPausesWorker.ToString()
  << ", server to worker request indicator: " << this->pipeServerToWorkerRequestIndicator.ToString()
  << ", server to worker computation report received: " << this->pipeServerToWorkerComputationReportReceived.ToString()
  << ", worker to server indicator data: " << this->pipeWorkerToServerIndicatorData.ToString()
  ;

  out << ", user address: " << this->userAddress << ".";
  return out.str();
}

WebServer::~WebServer()
{ if (this->activeWorker!=-1)
    this->GetActiveWorker().SendAllBytes();
  for (int i=0; i<this->theWorkers.size; i++)
    this->theWorkers[i].Release();
  this->activeWorker=-1;
  close(this->listeningSocketID);
}

void WebServer::ReturnActiveIndicatorAlthoughComputationIsNotDone()
{ //theLog << logger::red << ("Got THUS far") << logger::endL;
  theWebServer.GetActiveWorker().StandardOutputReturnIndicatorWaitForComputation();
}

void WebServer::FlushActiveWorker()
{ theWebServer.GetActiveWorker().SendAllBytes();
}

void WebServer::SendStringThroughActiveWorker(const std::string& theString)
{ theWebServer.GetActiveWorker().QueueStringForSending(theString, false);
}

void WebServer::PipeProgressReportToParentProcess(const std::string& theString)
{ theWebServer.GetActiveWorker().PipeProgressReportToParentProcess(theString);
}

WebServer::WebServer()
{ this->flagUsingBuiltInServer=false;
  this->flagTryToKillOlderProcesses=true;
  this->activeWorker=-1;
  this->timeLastExecutableModification=-1;
}

WebWorker& WebServer::GetActiveWorker()
{ MacroRegisterFunctionWithName("WebServer::GetActiveWorker");
  stOutput.theOutputFunction=0; //<- We are checking if the web server is in order.
  //Before that we prevent the crashing mechanism from trying to use (the eventually corrput) web server
  //to report the error over the internet.
  if (this->activeWorker<0 || this->activeWorker>=this->theWorkers.size)
    crash << "Active worker index is " << this->activeWorker << " however I have " << this->theWorkers.size
    << " workers. " << crash;
  stOutput.theOutputFunction=WebServer::SendStringThroughActiveWorker;//<-the web server is in order,
  //therefore we restore the ability to report crashes over the internet.
  return this->theWorkers[this->activeWorker];
}

void WebServer::SignalActiveWorkerDoneReleaseEverything()
{ MacroRegisterFunctionWithName("WebServer::ReleaseActiveWorker");
  if (theWebServer.activeWorker==-1)
    return;
  theWebServer.GetActiveWorker().SignalIamDoneReleaseEverything();
  theWebServer.activeWorker=-1;
}

void WebServer::ReleaseActiveWorker()
{ MacroRegisterFunctionWithName("WebServer::ReleaseActiveWorker");
  if (this->activeWorker==-1)
    return;
  this->GetActiveWorker().Release();
  this->activeWorker=-1;
}

void WebServer::ReleaseNonActiveWorkers()
{ MacroRegisterFunctionWithName("WebServer::ReleaseNonActiveWorkers");
  for (int i=0; i<this->theWorkers.size; i++)
    if (i!=this->activeWorker)
      this->theWorkers[i].ReleaseKeepInUseFlag();
}

void WebServer::ReleaseSocketsNonActiveWorkers()
{ MacroRegisterFunctionWithName("WebServer::ReleaseSocketsNonActiveWorkers");
  for (int i=0; i<this->theWorkers.size; i++)
    if (i!=this->activeWorker)
      this->Release(this->theWorkers[i].connectedSocketID);
}

void WebServer::CreateNewActiveWorker()
{ MacroRegisterFunctionWithName("WebServer::CreateNewActiveWorker");
  if (this->activeWorker!=-1)
  { crash << "Calling CreateNewActiveWorker requres the active worker index to be -1." << crash;
    return;
  }
  for (int i=0; i<this->theWorkers.size; i++)
    if (!this->theWorkers[i].flagInUse)
    { this->activeWorker=i;
      break;
    }
  if (this->activeWorker==-1)
  { this->activeWorker=this->theWorkers.size;
    this->theWorkers.SetSize(this->theWorkers.size+1);
  }
  this->GetActiveWorker().Release();
  this->theWorkers[this->activeWorker].flagInUse=true;

  this->GetActiveWorker().pipeServerToWorkerComputationReportReceived.CreateMe();
  this->GetActiveWorker().pipeServerToWorkerEmptyingPausesWorker.CreateMe();
  this->GetActiveWorker().pipeServerToWorkerEmptyingPausesWorker.Write("!", false);
  this->GetActiveWorker().pipeServerToWorkerRequestIndicator.CreateMe();
  this->GetActiveWorker().pipeWorkerToServerControls.CreateMe();
  this->GetActiveWorker().pipeWorkerToServerIndicatorData.CreateMe();
  this->GetActiveWorker().indexInParent=this->activeWorker;
  this->GetActiveWorker().parent=this;
}

void Pipe::WriteAfterClearingPipe(const std::string& toBeSent, bool doNotBlock)
{ MacroRegisterFunctionWithName("Pipe::WriteAfterClearingPipe");
  this->ReadFileDescriptor(this->pipeIndicesOnCreation[0], true);
  this->lastRead.SetSize(0);
  this->Write(toBeSent, doNotBlock);
}

void Pipe::Write(const std::string& toBeSent, bool doNotBlock)
{ MacroRegisterFunctionWithName("WebServer::WriteToPipe");
  if (this->thePipe[1]==-1)
    return;
  char buffer[2];
  read(this->pipeEmptyingBlocksWrite[0], buffer, 2);
  if (theWebServer.activeWorker!=-1)
    theLog << logger::green << ("worker ") << theWebServer.activeWorker;
  else
    theLog << logger::green << ("server ");
  theLog << logger::green << (" is sending ") << toBeSent.size()
  << logger::green << (" bytes through pipe "+this->ToString()) << logger::endL;
/*  if (doNotBlock)
    theLog << ", NON-blocking." << logger::endL;
  else
    theLog << ", BLOCKING." << logger::endL;*/
  if (doNotBlock)
    fcntl(this->thePipe[1], F_SETFL, O_NONBLOCK);
  else
    fcntl(this->thePipe[1], F_SETFL, 0);
  std::string toBeSentCopy=toBeSent;
  for (;;)
  { this->pipeBuffer.SetSize(4096); // <-once the buffer is resized, this operation does no memory allocation and is fast.
    int numBytesRemaining=toBeSentCopy.size();
    int numBytesWritten=write(this->thePipe[1], toBeSentCopy.c_str(), toBeSentCopy.size());
    numBytesRemaining-=numBytesWritten;
    if (numBytesWritten<=0)
    { write(this->pipeEmptyingBlocksWrite[1], "!", 1);
      return;
    }
    if (numBytesRemaining<=0 || numBytesWritten+numBytesRemaining>(signed) toBeSentCopy.size())
    { write(this->pipeEmptyingBlocksWrite[1], "!", 1);
      return;
    }
    toBeSentCopy= toBeSentCopy.substr(numBytesWritten, numBytesRemaining);
  }
  write(this->pipeEmptyingBlocksWrite[1], "!", 1);
}

std::string Pipe::ToString()const
{ if (this->pipeIndicesOnCreation[0]==-1 && this->pipeIndicesOnCreation[1]==-1)
    return "released";
  std::stringstream out;
  out << this->pipeIndicesOnCreation[1];
  if (this->thePipe[1]==-1)
    out << "(outside)";
  out << "->" << this->pipeIndicesOnCreation[0];
  if (this->thePipe[0]==-1)
    out << "(outside)";
  return out.str();
}

void Pipe::CreateMe()
{ this->Release();
  if (pipe(this->thePipe.TheObjects)<0)
    crash << "Failed to open pipe from parent to child. " << crash;
  this->pipeIndicesOnCreation=this->thePipe;
  if (pipe(this->pipeEmptyingBlocksRead.TheObjects)<0)
    crash << "Failed to open pipe from parent to child. " << crash;
  if (pipe(this->pipeEmptyingBlocksWrite.TheObjects)<0)
    crash << "Failed to open pipe from parent to child. " << crash;
  fcntl(this->thePipe[1], F_SETFL, 0);
  write(this->pipeEmptyingBlocksRead[1], "!", 1);
  write(this->pipeEmptyingBlocksWrite[1], "!", 1);
}

Pipe::~Pipe()
{ //Pipes are not allowed to release resources in the destructor:
  //a pipe's destructor is called when expanding List<Pipe>.
}

void Pipe::Release()
{ WebServer::Release(this->thePipe[0]);
  WebServer::Release(this->thePipe[1]);
  WebServer::Release(this->pipeEmptyingBlocksRead[0]);
  WebServer::Release(this->pipeEmptyingBlocksRead[1]);
  WebServer::Release(this->pipeEmptyingBlocksWrite[0]);
  WebServer::Release(this->pipeEmptyingBlocksWrite[1]);
  this->pipeIndicesOnCreation[0]=-1;
  this->pipeIndicesOnCreation[1]=-1;
  this->lastRead.SetSize(0);
}

void Pipe::ReadFileDescriptor(int readEnd, bool doNotBlock)
{ MacroRegisterFunctionWithName("Pipe::ReadFileDescriptor");
  this->lastRead.SetSize(0);
  if (readEnd==-1)
    return;
  if (doNotBlock)
    fcntl(readEnd, F_SETFL, O_NONBLOCK);
  else
    fcntl(readEnd, F_SETFL, 0);
  int counter=0;
  const unsigned int bufferSize=4096;
  for (;;)
  { this->pipeBuffer.SetSize(bufferSize); // <-once the buffer is resized, this operation does no memory allocation and is fast.
    theLog << logger::blue << (theWebServer.ToStringActiveWorker())
    << logger::blue << (" pipe, " + this->ToString()) << " calling read." << logger::endL;
    int numReadBytes =read(readEnd, this->pipeBuffer.TheObjects, bufferSize);
    if (numReadBytes<0)
    { theLog << logger::red << this->ToString() << ": " << theWebServer.ToStringLastErrorDescription()
      << logger::endL;
      return;
    }
    if (numReadBytes==0)
      return;
    counter++;
    if (counter>10000)
      theLog << "This is not supposed to happen: more than 10000 iterations of read from pipe."
      << logger::endL;
    theLog << logger::blue << theWebServer.ToStringActiveWorker() << " pipe, " << this->ToString()
    << " " << numReadBytes << " bytes read. " << logger::endL;
    this->pipeBuffer.SetSize(numReadBytes);
    this->lastRead.AddListOnTop(this->pipeBuffer);
    if (numReadBytes<(signed) bufferSize)
      return;
  }
}

void Pipe::Read(bool doNotBlock)
{ MacroRegisterFunctionWithName("Pipe::Read");
  if (this->thePipe[0]==-1)
  { this->lastRead.SetSize(0);
    return;
  }
  char buffer[2];
  if (!doNotBlock)
    read(this->pipeEmptyingBlocksRead[0], buffer, 2);
  theLog << logger::blue << theWebServer.ToStringActiveWorker()
  << " reading pipe " << this->ToString() << logger::endL;
  this->ReadFileDescriptor(this->thePipe[0], doNotBlock);
  theLog << logger::blue << theWebServer.ToStringActiveWorker() << " read "
  << this->lastRead.size << " bytes." << logger::endL;
  if (!doNotBlock)
    write(this->pipeEmptyingBlocksRead[1], "!", 1);
}

std::string WebServer::ToStringLastErrorDescription()
{ std::stringstream out;
  out << this->ToStringActiveWorker() << ": " << logger::red << (strerror(errno)) << ". ";
  return out.str();
}

std::string WebServer::ToStringActiveWorker()
{ if (theWebServer.activeWorker==-1)
    return "server";
  std::stringstream out;
  out << "worker " << theWebServer.activeWorker;
  return out.str();
}

std::string WebServer::ToStringStatusActive()
{ MacroRegisterFunctionWithName("WebServer::ToStringStatusActive");
  if (this->activeWorker==-1)
    return "server.";
  std::stringstream out;
  if (this->activeWorker!=this->GetActiveWorker().indexInParent)
    crash << "Bad index in parent!" << crash;
  out << this->GetActiveWorker().ToStringStatus();
  return out.str();
}

std::string WebServer::ToStringStatusAll()
{ MacroRegisterFunctionWithName("WebServer::ToStringStatusFull");
  std::stringstream out;
  if (this->activeWorker==-1)
    out << "The process is functioning as a server.";
  else
  { out << "The process is functioning as a worker. The active worker is number " << this->activeWorker+1 << ".";
    out << "<br>" << this->ToStringStatusActive();
  }
  int numInUse=0;
  for (int i=0; i<this->theWorkers.size; i++)
    if (this->theWorkers[i].flagInUse)
      numInUse++;
  out << "<hr>" << numInUse << " workers in use out of total " << this->theWorkers.size << " workers.";
  for (int i=0; i<this->theWorkers.size; i++)
    out << "<br>" << this->theWorkers[i].ToStringStatus();
  return out.str();
}

void WebServer::CheckExecutableVersionAndRestartIfNeeded()
{ struct stat theFileStat;
  if (stat(onePredefinedCopyOfGlobalVariables.PhysicalNameFolderBelowExecutable.c_str(), &theFileStat)!=0)
    return;
  theLog << "current process spawned from file with time stamp: " << this->timeLastExecutableModification
  << "; latest executable time stamp: " << theFileStat.st_ctime << logger::endL;
  if (this->timeLastExecutableModification!=-1)
    if (this->timeLastExecutableModification!=theFileStat.st_ctime)
    { stOutput << "<b>The server executable was updated, but the server has not been restarted yet. "
      << "Restarting in 0.5 seconds...";
      stOutput << "<script language=\"javascript\">setTimeout(resubmit, 500); "
      << " function resubmit() { document.getElementById('formCalculator').submit();}</script>";
      if (this->activeWorker!=-1)
      { this->GetActiveWorker().SendAllBytes();
        this->ReleaseActiveWorker();
      }
      theLog << logger::red << ("Time stamps are different, RESTARTING.") << logger::endL;
      this->Restart();
    }
}

void WebServer::Restart()
{ theLog << "Killing all copies of the calculator and restarting..." << logger::endL;
  this->Release(this->listeningSocketID);
  system("killall calculator \r\n./calculator server nokill"); //kill any other running copies of the calculator.
}

void WebServer::initDates()
{ this->timeLastExecutableModification=-1;
  struct stat theFileStat;
  if (stat(onePredefinedCopyOfGlobalVariables.PhysicalNameFolderBelowExecutable.c_str(), &theFileStat)!=0)
    return;
  this->timeLastExecutableModification=theFileStat.st_ctime;
}

void WebServer::ReleaseWorkerSideResources()
{ MacroRegisterFunctionWithName("WebServer::ReleaseWorkerSideResources");
  this->Release(this->GetActiveWorker().connectedSocketID);
  //<-release socket- communication is handled by the worker.
  this->activeWorker=-1; //<-The active worker is needed only in the child process.
}

int WebServer::Run()
{ MacroRegisterFunctionWithName("WebServer::Run");
  if (this->flagTryToKillOlderProcesses)
    this->Restart();
  usleep(10000);
  this->initDates();
  addrinfo hints, *servinfo, *p;
  sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  int yes=1;
  char userAddressBuffer[INET6_ADDRSTRLEN];
  int rv;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP
  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
  { theLog << "getaddrinfo: " << gai_strerror(rv) << logger::endL;
    return 1;
  }
  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next)
  { this->listeningSocketID= socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (this->listeningSocketID == -1)
    { stOutput << "Error: socket failed\n";
      continue;
    }
    if (setsockopt(this->listeningSocketID, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
      crash << "Error: setsockopt failed.\n" << crash;
    if (bind(this->listeningSocketID, p->ai_addr, p->ai_addrlen) == -1)
    { close(this->listeningSocketID);
      theLog << "Error: bind failed. " << this->ToStringLastErrorDescription() << logger::endL;
      continue;
    }
    break;
  }
  if (p == NULL)
    crash << "Failed to bind to port " << PORT << "\n" << crash;
  freeaddrinfo(servinfo); // all done with this structure
  if (listen(this->listeningSocketID, BACKLOG) == -1)
    crash << "Listen function failed." << crash;
// The following code does not appear to do anything on my system:
/*  struct sigaction sa;
  sa.sa_handler = &WebServer::Signal_SIGCHLD_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1)
    theLog << "sigaction returned -1" << logger::endL;
  sa.sa_handler=&WebServer::Signal_SIGINT_handler;
  if (sigaction(SIGINT, &sa, NULL) == -1)
    theLog << "sigaction returned -1" << logger::endL;
*/
  theLog << "server: waiting for connections...\r\n" << logger::endL;
  unsigned int connectionsSoFar=0;
  while(true)
  { // main accept() loop
    sin_size = sizeof their_addr;
    int newConnectedSocket = accept(this->listeningSocketID, (struct sockaddr *)&their_addr, &sin_size);
    if (newConnectedSocket <0)
    { theLog << "Accept failed. Error: " << this->ToStringLastErrorDescription() << logger::endL;
      continue;
    }
    //Listen for children who have exited properly.
    //This might need to be rewritten: I wasn't able to make this work with any
    //mechanism other than pipes. This probably due to my lack of skill or UNIX's
    //crappy design (and is most likely due to both).
    for (int i=0; i<this->theWorkers.size; i++)
      if (this->theWorkers[i].flagInUse)
      { Pipe& currentPipe=this->theWorkers[i].pipeWorkerToServerControls;
        currentPipe.Read(true);
        if (currentPipe.lastRead.size>0)
        { this->theWorkers[i].flagInUse=false;
          theLog << logger::green << ("worker ") << i+1 << logger::green << (" done, marking for reuse.") << logger::endL;
        } else
          theLog << logger::red << ("worker ") << i+1 << logger::red << (" not done yet.") << logger::endL;
      }
    this->CreateNewActiveWorker();
    this->GetActiveWorker().connectedSocketID=newConnectedSocket;
    this->GetActiveWorker().connectedSocketIDLastValueBeforeRelease=newConnectedSocket;
    connectionsSoFar++;
    this->GetActiveWorker().connectionID=connectionsSoFar;
    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), userAddressBuffer, sizeof userAddressBuffer);
    this->GetActiveWorker().userAddress=userAddressBuffer;
//    theLog << this->ToStringStatus();
    this->GetActiveWorker().ProcessPID=fork(); //creates an almost identical copy of this process.
    //The original process is the parent, the almost identical copy is the child.
    //theLog << "\r\nChildPID: " << this->childPID;
    if (this->GetActiveWorker().ProcessPID!=0)
    { // this is the child (worker) process
      //releasing all resources worker does not need:
      this->GetActiveWorker().ReleaseServerSideResources();
      onePredefinedCopyOfGlobalVariables.WebServerReturnDisplayIndicatorCloseConnection=
      this->ReturnActiveIndicatorAlthoughComputationIsNotDone;
      InitializeTimer();
      CreateTimerThread();
      onePredefinedCopyOfGlobalVariables.MaxComputationTimeSecondsNonPositiveMeansNoLimit=5000;
      crash.CleanUpFunction=WebServer::SignalActiveWorkerDoneReleaseEverything;
      stOutput.theOutputFunction=WebServer::SendStringThroughActiveWorker;
      stOutput.flushOutputFunction=this->FlushActiveWorker;
//      theLog << this->ToStringStatusActive() << logger::endL;
      this->GetActiveWorker().CheckConsistency();
      if (!this->GetActiveWorker().ReceiveAll())
      { stOutput << "HTTP/1.1 400 Bad Request\r\nContent-type: text/html\r\n\r\n" << this->GetActiveWorker().error;
        return 0;
      }
      return 1;
    }
    this->ReleaseWorkerSideResources();
  }
  return 0;
}

void WebServer::Release(int& theDescriptor)
{ close(theDescriptor);
  theDescriptor=-1;
}
