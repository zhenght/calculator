//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#ifndef vpfHeaderHtmlSnippetsAlreadyDefined
#define vpfHeaderHtmlSnippetsAlreadyDefined
#include "vpfMacros.h"

static ProjectInformationInstance ProjectInfoHeaderHtmlSnippets(__FILE__, "Html snippets header.");

class HtmlSnippets
{
public:
  static std::string GetJavascriptStandardCookies();
  static std::string GetJavascriptHideHtml();
  static std::string GetJavascriptSubmitEmails();
  static std::string GetJavascriptCookieFunctions();
  static std::string GetJavascriptSubmitMainInputIncludeCurrentFile();
  static std::string GetJavascriptCanvasGraphicsWithTags();
  static std::string GetJavascriptTopicListWithTags();
  static std::string GetDatePickerJavascriptInit();
  static std::string GetJavascriptVariable(const std::string& theVar);
};
#endif
