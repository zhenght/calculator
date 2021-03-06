//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#ifndef vpfHeaderHtmlInterpretationInterfaceAlreadyDefined
#define vpfHeaderHtmlInterpretationInterfaceAlreadyDefined
#include "vpfMacros.h"
#include <ctime>


static ProjectInformationInstance ProjectInfoHeaderHtmlInterpretationInterface(__FILE__, "Html interpretation interface.");

class HtmlInterpretation
{
public:
  std::string htmlRaw;
  std::string htmlJSbuild;
  List<std::string> currentWords;
  //List<bool> charIsSplitting;
  List<std::string> jsFiles;

  void BuildHtmlJSpage(bool appendBuildHash);
  static std::string GetHtmlTagWithManifest();
  static std::string ModifyProblemReport();
  static std::string ClonePageResult();
  static std::string AddTeachersSections();
  static std::string AddUserEmails(const std::string& hostWebAddressWithPort);
  static std::string SubmitAnswers
  (const std::string& inputRandomSeed, bool* outputIsCorrect, bool timeSafetyBrake);
  static std::string SubmitAnswers();
  static std::string GetProblemSolution();
  static std::string GetEditPageHTML();
  static std::string GetEditPageJSON();
  static std::string GetBrowseProblems();
  static std::string GetJavascriptCaptcha();
  static std::string GetCaptchaDiv();
  static std::string GetExamPage();
  static std::string GetExamPageJSON();
  static std::string GetPageFromTemplate();
  static std::string GetJSONFromTemplate();
  static std::string GetJSONUserInfo();
  static std::string GetTopicTableJSON();
  static std::string GetExamPageInterpreter();
  static std::string GetAnswerOnGiveUp();
  static std::string GetAnswerOnGiveUp
  (const std::string& inputRandomSeed, std::string* outputNakedAnswer = 0, bool* outputDidSucceed = 0);
  static std::string GetSelectCourse();
  static std::string GetSelectCourseJSON();
  static std::string GetScoresPage();
  static std::string GetAboutPage();
  static std::string GetApp(bool appendBuildHash);
  static std::string GetOnePageJS(bool appendBuildHash);
  static std::string GetScoresInCoursePage();
  static std::string GetNavigationPanelWithGenerationTime();
  static std::string submitAnswersPreview();
  static std::string GetSetProblemDatabaseInfoHtml();
  static std::string GetAccountsPage(const std::string& hostWebAddressWithPort);
  static std::string GetAccountsPageJSON(const std::string& hostWebAddressWithPort);
  static std::string GetAccountsPageBody(const std::string& hostWebAddressWithPort);
  static std::string ToStringAssignSection();
  static std::string ToStringUserDetails
  (bool adminsOnly, List<JSData>& theUsers, const std::string& hostWebAddressWithPort);
  static std::string ToStringUserScores();
  static std::string ToStringUserDetailsTable
  (bool adminsOnly, List<JSData>& theUsers, const std::string& hostWebAddressWithPort);
  static std::string ToStringCalculatorArgumentsHumanReadable();
  static std::string GetSanitizedComment
  (const Expression& input, FormatExpressions& theFormat, bool& resultIsPlot);
  static std::string GetCommentsInterpretation
  (Calculator& theInterpreterWithAdvice, int indexShift, FormatExpressions& theFormat);
  static std::string ToStringNavigation();
};


#endif
