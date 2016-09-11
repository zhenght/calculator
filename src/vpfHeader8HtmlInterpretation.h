//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#ifndef vpfHeaderHtmlInterpretationAlreadyDefined
#define vpfHeaderHtmlInterpretationAlreadyDefined
#include "vpfHeader1General0_General.h"
#include "vpfHeader7DatabaseInterface_MySQL.h"
#include <ctime>

static ProjectInformationInstance ProjectInfoHeaderHtmlInterpretation(__FILE__, "Html interpretation.");

class CalculatorHTML;

struct TopicElement{
public:
  bool flagIsChapter;
  bool flagIsSection;
  bool flagIsSubSection;
  bool flagIsError;
  std::string title;
  std::string video;
  std::string slides;
  std::string slidesPrintable;
  std::string problem;
  std::string error;
  std::string displayTitle;
  std::string displayVideoLink;
  std::string displaySlidesLink;
  std::string displaySlidesPrintableLink;
  std::string displayProblemLink;
  std::string displayAceProblemLink;
  std::string displayDeadline;
  std::string displayScore;
  std::string displayModifyWeight;
  std::string displayModifyDeadline;
  void reset()
  { this->flagIsSection=false;
    this->flagIsSubSection=false;
    this->flagIsChapter=false;
    this->flagIsError=false;
    this->title="empty";
    this->video="";
    this->slides="";
    this->slidesPrintable="";
    this->problem="";
    this->error="";
  }
  friend std::ostream& operator << (std::ostream& output, const TopicElement& theElt)
  { output << theElt.ToString();
    return output;
  }
  std::string ToString()const;
  static std::string GetTableStart(bool plainStyle);
  static std::string GetTableFinish(bool plainStyle);
  void ComputeLinks(CalculatorHTML& owner, bool plainStyle);
  TopicElement()
  { this->reset();
  }
  static void GetTopicList(const std::string& inputString, List<TopicElement>& output);
};

struct ProblemResources{
public:
  std::string video;
  std::string slides;
  std::string slidesPrintable;
  std::string deadline;
  Rational pointsScored;
  Rational maxPoints;
};

class CalculatorHTML
{
public:
  int NumAttemptsToInterpret;
  int MaxInterpretationAttempts;
  int NumProblemsFound;
  int NumAnswerIdsMathquilled;
  List<int> randomSeedsIfInterpretationFails;
  bool flagIsExamHome;
  bool flagIsExamProblem;
  bool flagParentInvestigated;
  bool flagIsForReal;
  bool flagLoadedFromDB;
  bool flagTagHtmlPresent;
  bool flagTagHeadPresent;
  bool flagTagBodyPresent;
  bool flagUseNavigationBar;
  bool flagMathQuillWithMatrices;
  double timeToParseHtml;
  List<double> timePerAttempt;
  List<List<double> > timeIntermediatePerAttempt;
  List<List<std::string> > timeIntermediateComments;
#ifdef MACRO_use_MySQL
  UserCalculator currentUseR;
#endif
  static std::string stringScoredQuizzes;
  static std::string stringPracticE;
  static std::string stringProblemLink;
  std::string fileName;
  std::string RelativePhysicalFileNameWithFolder;
  std::string inputHtml;

  std::string outputHtmlBodyNoTag;
  std::string outputHtmlHeadNoTag;
  std::string outputHtmlNavigatioN;
  std::string logCommandsProblemGeneration;
  std::string courseHome;

  static const std::string BugsGenericMessage;
  HashedList<std::string, MathRoutines::hashString> tagKeysNoValue;
  List<std::string> calculatorClasses;
  List<std::string> calculatorClassesAnswerFields;
  List<char> splittingChars;
  List<SyntacticElementHTML> eltsStack;
  List<SyntacticElementHTML> theContent;
//  List<std::string> answerFirstCorrectSubmission;
  Selection studentTagsAnswered;
  ProblemData theProblemData;
  std::string topicListContent;
  std::string topicListFileName;
  List<TopicElement> theTopics;
  HashedList<std::string, MathRoutines::hashString> TopicProblemFileNames;
  List<List<std::string> > hdHomeworkGroups;
  HashedList<std::string, MathRoutines::hashString> hdHomeworkGroupNames;
  HashedList<std::string, MathRoutines::hashString> databaseProblemAndHomeworkGroupList;
//  List<std::string> databaseHomeworkGroupCorrespondingToEachProblem;
  List<List<std::string> > userTablE;
  List<std::string> labelsUserTablE;
  List<std::string> problemListOfParent;
  List<std::string> databaseStudentSections;
  bool flagLoadedSuccessfully;
  bool flagLoadedClassDataSuccessfully;
  /////////////////
  std::stringstream comments;
  int GetAnswerIndex(const std::string& desiredAnswerId);
  bool CheckContent(std::stringstream& comments);
  bool CanBeMerged(const SyntacticElementHTML& left, const SyntacticElementHTML& right);
  bool LoadMe(bool doLoadDatabase, std::stringstream& comments);
  bool LoadAndParseTopicList(std::stringstream& comments);
  bool LoadDatabaseInfo(std::stringstream& comments);
  std::string CleanUpFileName(const std::string& inputLink);
  bool ParseHTMLComputeChildFiles(std::stringstream& comments);
  bool ParseHTML(std::stringstream& comments);
  bool ParseHTMLPrepareCommands(std::stringstream& comments);
  bool IsSplittingChar(const std::string& input);
  void LoadFileNames();
  bool IsStateModifierApplyIfYes(SyntacticElementHTML& inputElt);
  bool ExtractAnswerIds(std::stringstream& comments);
  bool InterpretHtml(std::stringstream& comments);
  bool InterpretHtmlOneAttempt(Calculator& theInterpreter, std::stringstream& comments);
  std::string ToStringInterprettedCommands(Calculator& theInterpreter, List<SyntacticElementHTML>& theElements);
  bool ProcessInterprettedCommands(Calculator& theInterpreter, List<SyntacticElementHTML>& theElements, std::stringstream& comments);
  bool PrepareAnswerElements(std::stringstream &comments);
  bool InterpretAnswerElements(std::stringstream& comments);
  bool InterpretOneAnswerElement(SyntacticElementHTML& inputOutput);
  bool PrepareAndExecuteCommands(Calculator& theInterpreter, std::stringstream& comments);
  std::string PrepareUserInputBoxes();
  bool PrepareCommandsAnswerOnGiveUp(Answer& theAnswer, std::stringstream& comments);
  bool PrepareCommentsBeforeSubmission(Answer& theAnswer, std::stringstream& comments);
  bool PrepareCommandsSolution(Answer& theAnswer, std::stringstream& comments);
  bool PrepareCommandsAnswer(Answer& theAnswer, std::stringstream& comments);
  bool PrepareCommandsGenerateProblem(std::stringstream& comments);
  std::string GetProblemHeaderEnclosure();
  bool SetTagClassFromOpenTag(SyntacticElementHTML& output);
  bool SetTagClassFromCloseTag(SyntacticElementHTML& output);

  bool PrepareCommands(std::stringstream& comments);
  std::string CleanUpCommandString(const std::string& inputCommand);
  void InterpretNotByCalculator(SyntacticElementHTML& inputOutput);
  std::string GetDeadline
  (const std::string& problemName, const std::string& sectionNumber,
   bool inheritFromGroup, bool& outputIsInherited);
  bool MergeOneProblemAdminData
(const std::string& inputProblemName, ProblemData& inputProblemInfo,
 std::stringstream& commentsOnFailure)
  ;
  bool MergeProblemInfoInDatabase
  (std::string& incomingProblemInfo,
   std::stringstream& commentsOnFailure)
  ;
  bool ReadProblemInfoAppend
  (const std::string& inputInfoString,
   MapLisT<std::string, ProblemData, MathRoutines::hashString>&
   outputProblemInfo,
   std::stringstream& commentsOnFailure
   )
   ;
  void StoreDeadlineInfo
  (std::string& outputString,
   MapLisT<std::string, ProblemData, MathRoutines::hashString>&
   inputProblemInfo)
   ;
  void StoreProblemWeightInfo
  (std::string& outputString,
   MapLisT<std::string, ProblemData, MathRoutines::hashString>&
   inputProblemInfo)
   ;
  std::string InterpretGenerateDeadlineLink
(bool isActualProblem, const std::string& cleaneduplink, const std::string& urledProblem, bool problemAlreadySolved)
  ;
  std::string ToStringLinkAndDetailsFromFileName(const std::string& theFileName, const std::string& stringToDisplay="");
  std::string ToStringLinkFromFileName(const std::string& theFileName, const std::string& stringToDisplay="");
  std::string ToStringCalculatorProblemSourceFromFileName(const std::string& theFileName);
  void InterpretGenerateLink(SyntacticElementHTML& inputOutput);
  std::string ToStringProblemWeighT(const std::string& theFileName);
  std::string ToStringProblemScoreFull(const std::string& theFileName);
  std::string ToStringProblemScoreShort(const std::string& theFileName, bool& outputAlreadySolved);
  bool ComputeAnswerRelatedStrings(SyntacticElementHTML& inputOutput);
  void InterpretGenerateStudentAnswerButton(SyntacticElementHTML& inputOutput);
  bool PrepareClassData(std::stringstream& commentsOnFailure);
  void InterpretManageClass(SyntacticElementHTML& inputOutput);
  void InterpretTopicList(SyntacticElementHTML& inputOutput);
  void InterpretTableOfContents(SyntacticElementHTML& inputOutput);
  void InterpretAccountInformationLinks(SyntacticElementHTML& inputOutput);
  void InterpretJavascripts(SyntacticElementHTML& inputOutput);
  std::string GetEditPageButton();
  std::string GetJavascriptSubmitMainInputIncludeCurrentFile();
  std::string GetDatePickerJavascriptInit();
  std::string GetDatePickerStart(const std::string& theId);
  std::string GetJavascriptSubmitAnswers();
  std::string GetJavascriptMathQuillBoxes();
  void LoadCurrentProblemItem();
  void FigureOutCurrentProblemList(std::stringstream& comments);
  std::string LoadAndInterpretCurrentProblemItem();
  bool FindExamItem();
  static unsigned int HashFunction(const CalculatorHTML& input)
  { return input.HashFunction();
  }
  unsigned int HashFunction()const
  { return MathRoutines::hashString(this->fileName);
  }
  bool operator==(const CalculatorHTML& other)const
  { return this->fileName==other.fileName;
  }
  std::string ToStringDeadlinesFormatted
  (const std::string& cleanedUpLink, const List<std::string>& sectionNumbers,
  bool problemAlreadySolved, bool returnEmptyStringIfNoDeadline)
  ;
  std::string ToStringOnEDeadlineFormatted
  (const std::string& cleanedUpLink, const std::string& sectionNumber,
  bool problemAlreadySolved, bool returnEmptyStringIfNoDeadline)
  ;
  std::string ToStringCalculatorArgumentsForProblem
  (const std::string& requestType, const std::string& studentView,
   const std::string& studentSection="", bool includeRandomSeedIfAppropriate=false)const;
  std::string ToStringProblemNavigation()const;
  std::string ToStringExtractedCommands();
  std::string ToStringContent();
  std::string ToStringParsingStack(List<SyntacticElementHTML>& theStack);
  CalculatorHTML();
};


#endif
