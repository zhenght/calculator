//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#ifndef vpfHeader1General8DatabaseSystemIndependent_already_included
#define vpfHeader1General8DatabaseSystemIndependent_already_included

#include "vpfHeader1General0_General.h"
static ProjectInformationInstance ProjectInfoVpfHeader1General8DatabaseSystemIndependentinstance(__FILE__, "Header, system independent database data structures. ");

class MySQLdata
{
//This class is needed to attempt to deal with mySQL's
//numerous design errors, to the extent possible.
//Documenting those errors for the interested reader.
//1. Mysql identifiers have max length of 64 characters.
//   Workaround this MySQL bug: when used as identifiers, strings are
//   trimmed. We use the first 30 characters
//   + we append SHA-1 of the entire string.
//   Motivation: we don't lose human-readability for small strings.
//2. Mysql identifiers cannot have ` characters in them.
//   Workaround this MySQL bug: we url-encode any data stored in
//   the database.
//   Motivation: we retain limited human-readability.
//-------------------------
//The value entry of the class stores an arbitrary sequence of characters,
//the sequence we care about and want stored/loaded from DB.
public:
  std::string value;
  MySQLdata(const std::string& other)
  { this->value=other;
  }
  MySQLdata(){}
  bool operator==(const std::string& other)
  { return this->value==other;
  }
  bool operator!=(const std::string& other)
  { return !(*this==other);
  }
  void operator=(const std::string& other)
  { this->value=other;
  }
  std::string GetDatA()const;
  std::string GetDataNoQuotes()const;
  std::string GetIdentifierNoQuotes()const;
  std::string GetIdentifieR()const;
};

struct CourseAndUserInfo
{
public:
  std::string problemWeightSchemaIDComputed;
  std::string deadlineSchemaIDComputed;
  std::string currentInstructorComputed;
  std::string currentInstructorInDB;
  std::string currentSemesterComputed;
  std::string currentSemesterInDB;
  std::string currentSectionInDB;
  std::string currentSectionComputed;
  std::string currentCourseComputed;
  std::string currentCourseInDB;
  std::string rawStringStoredInDB;
  std::string sectionsTaughtByUserString;
  std::string deadlinesString;
  std::string problemWeightString;
  std::string ToStringForDBStorage();
  std::string ToStringHumanReadable();
};

class UserCalculatorData
{
  public:
  double approximateHoursSinceLastTokenWasIssued;
  std::string usernamePlusPassWord;
  MySQLdata userId;
  MySQLdata username;
  MySQLdata email;
  MySQLdata currentTable;
  MySQLdata timeOfActivationTokenCreation;
  MySQLdata actualActivationToken;
  MySQLdata enteredActivationToken;
  MySQLdata enteredAuthenticationToken;
  MySQLdata actualAuthenticationToken;
  MySQLdata problemDataString;
  CourseAndUserInfo courseInfo;

  std::string enteredPassword;
  std::string actualShaonedSaltedPassword;
  std::string enteredShaonedSaltedPassword;
  std::string enteredGoogleToken;
  std::string userRole;
  std::string authenticationTokenCreationTime;
  std::string activationTokenCreationTime;
  std::string activationEmail;
  std::string activationEmailSubject;
  //List<std::string> sectionsViewableByUser;
  List<std::string> selectedColumnsUnsafe;
  List<std::string> selectedColumnValuesUnsafe;
  List<std::string> selectedColumnsRetrievalFailureRemarks;

  List<std::string> selectedRowFieldsUnsafe;
  List<std::string> selectedRowFieldNamesUnsafe;
  bool flagEnteredAuthenticationToken;
  bool flagEnteredPassword;
  bool flagEnteredActivationToken;
  bool flagMustLogin;
  bool flagStopIfNoLogin;
  bool flagUserHasActivationToken;
  bool flagUserHasNoPassword;
  UserCalculatorData();
  bool AssignCourseInfoString(std::stringstream* errorStream);
  void reset();
  void clearPasswordFromMemory();
  void clearAuthenticationTokenAndPassword();
  std::string ToStringUnsecure();
};

struct DatabaseStrings{
public:
  static std::string userId;

  static std::string databaseUser;
  static std::string theDatabaseName;
  static std::string userColumnLabel;
  static std::string usersTableName;
  static std::string userGroupLabel;
  static std::string userCurrentCoursesColumnLabel;
  static std::string courseInfoColumnLabel;

  static std::string deadlinesTableName;
  static std::string infoColumnInDeadlinesTable;

  static std::string problemWeightsTableName;
  static std::string problemWeightsIdColumnName;
  static std::string infoColumnInProblemWeightsTable;

  static std::string sectionsTaughtByUser;

};
#endif
