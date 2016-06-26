//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#ifndef vpfHeader1General7FileOperations_Encodings_already_included
#define vpfHeader1General7FileOperations_Encodings_already_included

#include "vpfHeader1General0_General.h"
static ProjectInformationInstance ProjectInfoVpfHeader1General7FileOperations_EncodingsHeader1instance(__FILE__, "Header, file routines, encodings and more. ");

struct FileOperations
{
public:
  static HashedList<std::string, MathRoutines::hashString>& GetAllowedFolderNamesParallelToProjectFolder();
  static HashedList<std::string, MathRoutines::hashString>& GetAllowedFolderNamesInsideProjectFolder();
  static bool LoadFileToStringUnsecure
  (const std::string& theFileName, std::string& output, std::stringstream& commentsOnFailure);
  static bool LoadFileToStringOnTopOfOutputFolder
  (const std::string& fileNameUnsecure, std::string& output, std::stringstream& commentsOnFailure);
  static bool LoadFileToStringOnTopOfProjectBase
  (const std::string& theFileName, std::string& output, std::stringstream& commentsOnFailure);
  static bool IsFileNameWithoutDotsAndSlashes(const std::string& fileName);
  static std::string GetFileNameFromFileNameWithPath(const std::string& fileName);
  static bool IsOKforFileNameOnTopOfOutputFolder(const std::string& fileName);
  static std::string GetPathFromFileNameWithPath(const std::string& fileName);
  static std::string GetFileExtensionWithDot(const std::string& theFileName);
  static bool FileExistsUnsecure(const std::string& theFileName);
  static bool FileExistsOnTopOfOutputFolder(const std::string& theFileName);
  static bool FileExistsOnTopOfProjectBase(const std::string& theFileName);
  static bool IsFolderUnsecure(const std::string& theFolderName);
  static bool IsFolderOnTopOfOutputFolder(const std::string& relativeFolderName);
  static bool IsFolderOnTopOfProjectBase(const std::string& relativeFolderName);
  static bool GetFolderFileNamesUnsecure
  (const std::string& theFolderName, List<std::string>& outputFileNamesNoPath,
   List<std::string>* outputFileTypes=0)
   ;
  static bool GetFolderFileNamesOnTopOfOutputFolder
  (const std::string& theFolderName, List<std::string>& outputFileNamesNoPath,
   List<std::string>* outputFileTypes=0)
   ;
  static bool GetFolderFileNamesOnTopOfProjectBase
  (const std::string& theFolderName, List<std::string>& outputFileNamesNoPath,
   List<std::string>* outputFileTypes=0)
   ;
  static bool OpenFileCreateIfNotPresentUnsecure(std::fstream& theFile, const std::string& theFileName, bool OpenInAppendMode, bool truncate, bool openAsBinary);
  static bool OpenFileCreateIfNotPresentOnTopOfOutputFolder(std::fstream& theFile, const std::string& theFileName, bool OpenInAppendMode, bool truncate, bool openAsBinary);
  static bool OpenFileCreateIfNotPresentOnTopOfProjectBase(std::fstream& theFile, const std::string& theFileName, bool OpenInAppendMode, bool truncate, bool openAsBinary);
  static bool OpenFileUnsecure(std::fstream& theFile, const std::string& theFileName, bool OpenInAppendMode, bool truncate, bool openAsBinary);
  static bool OpenFileOnTopOfOutputFolder(std::fstream& theFile, const std::string& theFileName, bool OpenInAppendMode, bool truncate, bool openAsBinary);
  static bool OpenFileOnTopOfProjectBase(std::fstream& theFile, const std::string& theFileName, bool OpenInAppendMode, bool truncate, bool openAsBinary);
};

struct CGI
{
public:
  static std::string StyleSheetMathQuillWithTags;
  static std::string StyleSheetCalculatorWithTags;
  static std::string JavascriptAutoCompleteWithTags;
  static std::string JavascriptMathjax;
  static std::string JavascriptMathQuill;
  static std::string JavascriptSha1;
  static std::stringstream outputStream;
  static int GlobalMathSpanID;
  static int GlobalCanvasID;
  static int GlobalGeneralPurposeID;
  static int numLinesAll;
  static int numRegularLines;
  static int numDashedLines;
  static int numDottedLines;
  static int shiftX;
  static int shiftY;
  static int scale;
  static void LoadStrings();
  static void outputLineJavaScriptSpecific(const std::string& lineTypeName, int theDimension, std::string& stringColor, int& lineCounter);
  static void PrepareOutputLineJavaScriptSpecific(const std::string& lineTypeName, int numberLines);
  static std::string URLKeyValuePairsToNormalRecursiveHtml(const std::string& input, int recursionDepth=0);
  static void URLStringToNormal(const std::string& input, std::string& output);
  static std::string URLStringToNormal(const std::string& input)
  { std::string output;
    CGI::URLStringToNormal(input, output);
    return output;
  }
  static bool URLStringToNormalOneStep(std::string& readAhead, std::stringstream& out);
  static bool GetPhysicalFileNameFromRelativeInput(const std::string& inputFileName, std::string& output);
  static std::string StringToURLString(const std::string& input, bool usePlusesForSpacebars=true);
  static void ReplaceEqualitiesAndAmpersandsBySpaces(std::string& inputOutput);
  static void MakeSureWeylGroupIsSane(char& theWeylLetter, int& theRank);
  static std::string GetCalculatorLink(const std::string& DisplayNameCalculator, const std::string& input);
  static std::string GetSliderSpanStartsHidden(const std::string& content, const std::string& label="Expand/collapse", const std::string& desiredID="");
  static std::string GetHtmlLinkFromProjectFileName
  (const std::string& fileName, const std::string& fileDesc="", int line=-1);
  static std::string GetHtmlSwitchMenuDoNotEncloseInTags();
  static std::string GetLatexEmbeddableLinkFromCalculatorInput(const std::string& address, const std::string& display);
  static bool StringToHtmlString(const std::string& input, std::string& output);
  static std::string StringToHtmlStrinG(const std::string& theString);
  static std::string DoubleBackslashes(const std::string& input);
  static std::string& GetMathQuillStyleSheetWithTags();
  static std::string& GetCalculatorStyleSheetWithTags();
  static std::string& GetJavascriptAutocompleteWithTags();
  static std::string& GetJavascriptSha1();
  static std::string& GetJavascriptMathjax();
  static std::string& GetJavascriptMathQuill();

  static std::string GetMathSpanPure(const std::string& input, int upperNumChars=10000);
  static std::string GetMathSpanBeginArrayL(const std::string& input, int upperNumChars=10000);
  static std::string GetMathMouseHover(const std::string& input, int upperNumChars=10000);
  static std::string GetMathMouseHoverBeginArrayL(const std::string& input, int upperNumChars=10000);
  static std::string GetStyleButtonLikeHtml();
  static std::string GetHtmlButton(const std::string& buttonID, const std::string& theScript, const std::string& buttonText);
  static std::string GetHtmlSpanHidableStartsHiddeN
  (const std::string& input, const std::string& labelExpandButton= "info expand/collapse");
  static std::string clearNewLines(const std::string& theString);
  static std::string backslashQuotes(const std::string& theString);
  static std::string clearSlashes(const std::string& theString);
  static std::string CleanUpForFileNameUse(const std::string& inputString);
  static std::string CleanUpForLaTeXLabelUse(const std::string& inputString);
  static std::string GetCalculatorLinkUnclosedPostArguments(const std::string& inputArguments);
  static std::string GetCalculatorLinkUnclosedPostArguments
  (const std::string& DisplayNameCalculator, const std::string& inputArguments);
  static bool IsRepresentedByItselfInURLs(char input);
  static void clearDollarSigns(std::string& theString, std::string& output);
  static void subEqualitiesWithSimeq(std::string& theString, std::string& output);
  static bool AccountOneInputCGIString
(const std::string& fieldName, const std::string& fieldValue, MapList<std::string, std::string, MathRoutines::hashString>& outputMap,
 std::stringstream& commentsOnFailure)
  ;
  static bool ChopCGIInputStringToMultipleStrings
(const std::string& input, MapList<std::string, std::string, MathRoutines::hashString>& outputMap,
 std::stringstream& commentsOnFailure)
  ;
  static std::string GetJavascriptInjectCalculatorResponseInNode();
  static void ElementToStringTooltip(const std::string& input, const std::string& inputTooltip, std::string& output, bool useHtml);
  static std::string ElementToStringTooltip(const std::string& input, const std::string& inputTooltip, bool useHtml){ std::string result; CGI::ElementToStringTooltip(input, inputTooltip, result, useHtml); return result; };
  static std::string ElementToStringTooltip(const std::string& input, const std::string& inputTooltip){ return CGI::ElementToStringTooltip(input, inputTooltip, true); };
  static uint32_t RedGreenBlue(unsigned int r, unsigned int g, unsigned int b);
  static void MakeStdCoutReport(const std::string& input);
  static void MakeReportIndicatorFile(const std::string& input);
  static void FormatCPPSourceCode(const std::string& FileName);
};
#endif