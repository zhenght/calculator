//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#ifndef vpfHeader2_h_already_included
#define vpfHeader2_h_already_included

//the following  include contains all the c++ math routines used in the calculator.
//The calculator uses relatively few functions, nevertheless the include is needed.
#include "vpfHeader1.h"


class Data
{
public:
  CommandList* owner;
  MemorySaving<Rational> theRational;
  MemorySaving<PolynomialRationalCoeff> thePoly;
  MemorySaving<RationalFunction> theRationalFunction;
  MemorySaving<std::string> theError;
  int type;
  enum DataType
  { typeError=1, typeRational, typePoly, typeRationalFunction, typeSSalgebra, typeElementSSalgebra
  };
  Data (const Rational& x, CommandList& inputOwner)
  { this->owner=&inputOwner;
    this->type=this->typeRational;
    this->theRational.GetElement()=x;
  }
  Data(CommandList& theOwner) : owner(&theOwner), type(Data::typeError) {}
  Data()
  { this->owner=0;
    this->type=this->typeError;
  }
  Data(const Data& otherData)
  { this->operator=(otherData);
  }
  void FreeMemory()
  { switch (this->type)
    { case Data::typeSSalgebra:
      case Data::typeRational: this->theRational.FreeMemory(); break;
      case Data::typePoly: this->thePoly.FreeMemory(); break;
      case Data::typeRationalFunction: this->theRationalFunction.FreeMemory(); break;
      default:
        break;
    }
  }
  bool MakeElementSemisimpleLieAlgebra
(CommandList& owner, SemisimpleLieAlgebra& ownerAlgebra, int theDisplayIndex, std::stringstream* comments)
  ;
  bool MakeElementSemisimpleLieAlgebra
(CommandList& owner, SemisimpleLieAlgebra& ownerAlgebra, int index1, int index2, std::stringstream* comments)
;
  bool IsEqualToOne()const;
  bool IsEqualToZero()const;
  bool IsSmallInteger(int & whichInteger)const
  { if (this->type!=this->typeRational)
      return false;
    return this->theRational.GetElementConst().IsSmallInteger(whichInteger);
  }
  bool IsInteger()
  { if (this->type!=this->typeRational)
      return false;
    return this->theRational.GetElement().IsInteger();
  }
  int GetSmallInt()
  { return this->theRational.GetElement().NumShort;
  }
  bool SetError(const std::string& inputError)
  { this->FreeMemory();
    this->type=this->typeError;
    this->theError.GetElement()=inputError;
    return false;
  }
  bool operator+=(const Data& other);
  bool operator*=(const Rational& other)
  { Data Other(other, *this->owner);
    return this->operator*=(Other);
  }
  bool operator/=(const Data& other)
  ;
  bool operator*=(const Data& other)
  ;
  void operator=(const Data& other);
  bool operator==(const Data& other);
  std::string ElementToString(std::stringstream* comments=0)const;
  std::string ElementToStringDataType()const;
  bool operator!=(const Data& other)
  { return ! this->operator==(other);
  }
  int HashFunction()const;
  static inline int HashFunction(const Data& input)
  { return input.HashFunction();
  }
  static bool LieBracket
  (Data& left, Data& right, Data& output, std::stringstream* comments)
  ;
  bool OperatorDereference
  (const Data& argument, Data& output, std::stringstream* comments)
  ;
  bool OperatorDereference
  (const Data& argument1, const Data& argument2, Data& output, std::stringstream* comments)
  ;
  Data operator/(const Data& right);
  Data operator*(const Data& right);
};

class Function
{
  public:
  //the functions may not possess a name.
  //If you want your function to possess a name, use a VariableNonBound which has a member function handler.
  std::string theName;
  std::string theArgumentList;
  std::string theDescription;
  std::string theExampleArguments;
  typedef  bool (*FunctionAddress)(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments);
  FunctionAddress theFunction;
  std::string ElementToString()const
  { return this->theName;
  }
  void operator =(const Function& other)
  { this->theName=other.theName;
    this->theArgumentList=other.theArgumentList;
    this->theDescription=other.theDescription;
    this->theExampleArguments=other.theExampleArguments;
    this->theFunction=other.theFunction;
  }
  bool operator==(const Function& other)const
  { return this->theArgumentList==other.theArgumentList &&
    this->theFunction==other.theFunction && this->theName==other.theName;
  }
  Function(){this->theFunction=0;}
  Function
  ( const Function::FunctionAddress& functionPointer,
    const std::string& functionName,
    const std::string& argumentList,
    const std::string& description, const std::string& exampleArguments
  )
  { this->theFunction=functionPointer;
    this->theName=functionName;
    this->theDescription=description;
    this->theExampleArguments=exampleArguments;
    this->theArgumentList=argumentList;
  }
  inline static int HashFunction(const Function& input)
  { return input.HashFunction();
  }
  int HashFunction()const
  { return ((int) this->theFunction)+SomeRandomPrimes[1]*MathRoutines::hashString(theArgumentList);
  }
};

class Expression
{ void reset(int inputIndexBoundVars)
  { this->theBoss=0;
    this->children.size=0;
    this->theOperation=-1;
    this->theData=0;
    this->errorString="";
    this->format=this->formatDefault;
    this->theComments="";
    this->IndexBoundVars=inputIndexBoundVars;
  }
  public:
  int theOperation;
  int theData;
  List<Expression> children;
  std::string errorString;
  ///////////////////////////////////////
  //two objects are considered equal even when the the following data is different:
  int format;
  int IndexBoundVars;
  CommandList* theBoss;
  std::string theComments;
  enum format
  { formatDefault, formatFunctionUseUnderscore, formatTimesDenotedByStar,
    formatFunctionUseCdot, formatNoBracketsForFunctionArgument, formatMatrix, formatMatrixRow
  };
  void reset(CommandList& newBoss, int inputIndexBoundVars)
  { this->reset(inputIndexBoundVars);
    this->theBoss=&newBoss;
  }
  void AssignChild(int childIndex)
  { Expression tempExp=this->children[childIndex];
    this->operator=(tempExp);
  }
  void MakeDatA(const Data& inputData, CommandList& newBoss, int inputIndexBoundVars)
  ;
  void MakeDatA(const Rational& inputRat, CommandList& newBoss, int inputIndexBoundVars)
  ;
  void MakeInt(int theInt, CommandList& newBoss, int inputIndexBoundVars)
  ;
void MakeVariableNonBoundNoProperties
  (CommandList& owner, int inputIndexBoundVars, int varIndex)
;
void MakeVariableNonBounD
  (CommandList& owner, int inputIndexBoundVars, int varIndex)
;
  void MakeFunction
  (CommandList& owner, int inputIndexBoundVars, const Expression& argument, const std::string& functionName)
;
  void MakeFunction
  (CommandList& owner, int inputIndexBoundVars, const Expression& argument, int functionIndex)
;
  Function::FunctionAddress GetFunctionFromVarName();
  void MakeProducT
  (CommandList& owner, int inputIndexBoundVars, const Expression& left, const Expression& right)
  ;
  void MakeFunction
  (CommandList& owner, int inputIndexBoundVars, const Expression& theFunction, const Expression& theArgument)
  ;
  int GetNumCols()const
  ;
  void MakeXOX
  (CommandList& owner, int inputIndexBoundVars, int theOp, const Expression& left, const Expression& right)
  ;
  std::string ElementToString
  (int recursionDepth=0, int maxRecursionDepth=1000, bool useLatex=false, bool AddBrackets=false,
   bool AddCurlyBraces=false, std::stringstream* outComments=0)const;
  std::string ElementToStringPolishForm(int recursionDepth=0, int maxRecursionDepth=1000);
  static int HashFunction(const Expression& input)
  { return input.HashFunctionRecursive(0, 1000);
  }
  int HashFunctionRecursive(int RecursionDepth, int MaxRecursionDepth)const
  { if (RecursionDepth>MaxRecursionDepth)
      return 0;
    int result=(this->theOperation+1)*SomeRandomPrimes[0]+(this->theData+SomeRandomPrimes[1])*SomeRandomPrimes[2];
    int numCycles=MathRoutines::Minimum(this->children.size, SomeRandomPrimesSize);
    for (int i=0; i<numCycles; i++)
      result+=this->children[i].HashFunctionRecursive(RecursionDepth+1, MaxRecursionDepth)*SomeRandomPrimes[i];
    return result;
  }
  std::string GetOperation()const;
  Expression()
  { this->reset(-1);
  }
  inline bool SetError (const std::string& theError)
  { this->errorString=theError;
    return true;
  }
  Expression(const Expression& other)
  { this->operator=(other);
  }
  bool HasBoundVariables(int Recursion, int MaxRecursionDepth);
  bool IsRationalNumber();
  bool IsInteger();
  bool IsSmallInteger(int& whichInteger);
  bool IsSmallInteger(){int tempI; return this->IsSmallInteger(tempI);}
  bool AreEqualExcludingChildren(const Expression& other) const
  { return
    this->theBoss==other.theBoss &&
    this->theData==other.theData &&
    this->theOperation==other.theOperation &&
    this->children.size==other.children.size &&
    this->errorString==other.errorString
    ;
  }
  Rational GetConstantTerm() const;
  bool NeedBracketsForMultiplication()const;
  bool NeedBracketsForFunctionArgument()const;
  bool NeedBracketsForAddition()const;
  bool NeedBracketsForFunctionName()const;
  bool NeedBracketsForThePower()const;
  bool operator==(const Expression& other)const;
  void CopyValueFromNoChildrenCopy(const Expression& other)
  { this->theComments=other.theComments;
    this->theBoss=other.theBoss;
    this->theData=other.theData;
    this->theOperation=other.theOperation;
    this->errorString=other.errorString;
    this->children.SetSize(other.children.size);
    this->format=other.format;
    this->IndexBoundVars=other.IndexBoundVars;
  }
  inline void operator=(const Expression& other)
  { //warning: we are assuming that invoking function OperatorAssignRecursively
    // recursively RecursionDepthThatWouldNonPopTheStack times
    //will not pop the stack. If this is not the case, the program might crash unexpectedly.
    const int RecursionDepthThatWouldNonPopTheStack=10000;
    this->OperatorAssignRecursively(other, 0, RecursionDepthThatWouldNonPopTheStack);
  }
  void OperatorAssignRecursively(const Expression& other, int RecursionDepth, int MaxRecursionDepth)
  { if (this==&other)
      return;
    this->CopyValueFromNoChildrenCopy(other);
    if (RecursionDepth>MaxRecursionDepth)
    { std::stringstream out;
      out << "Error while copying expression: expression depth exceeded maximum allowed depth of " << MaxRecursionDepth;
      this->errorString=out.str();
      return;
    }
    this->children.SetSize(other.children.size);
    for (int i=0; i<other.children.size; i++)
      this->children[i].OperatorAssignRecursively(other.children[i], RecursionDepth+1, MaxRecursionDepth);
  }
};

class ExpressionPairs
{
public:
  std::string ElementToString();
  HashedListB<int, MathRoutines::IntIdentity> BoundVariableIndices;
  HashedList<Expression> variableImages;
  void reset()
  { this->BoundVariableIndices.Clear();
    this->variableImages.Clear();
  }
};

class SyntacticElement
{
  public:
  int controlIndex;
  int IndexFirstChar;
  int IndexLastCharPlusOne;
  std::string ErrorString;
  Expression theData;
  void operator=(const SyntacticElement& other)
  { this->controlIndex=other.controlIndex;
    this->theData=other.theData;
    this->ErrorString=other.ErrorString;
    this->IndexFirstChar=other.IndexFirstChar;
    this->IndexLastCharPlusOne=other.IndexLastCharPlusOne;
  }
  std::string ElementToString(CommandList& theBoss);
  SyntacticElement()
  { this->controlIndex=0;//controlIndex=0 *MUST* point to the empty control sequence.
    this->ErrorString="";
    this->IndexFirstChar=-1;
    this->IndexLastCharPlusOne=-1;
  }
  SyntacticElement(const SyntacticElement& other)
  { this->operator=(other);
  }
};

class ExpressionContext
{
public:
  HashedListB<std::string, MathRoutines::hashString> BoundVariables;
  HashedList<Expression> cachedExpressions;
  List<Expression> imagesCachedExpressions;
  bool flagOpDefineEncountered;
  void operator=(const ExpressionContext& other)
  { this->BoundVariables=other.BoundVariables;
    this->cachedExpressions=other.cachedExpressions;
    this->imagesCachedExpressions=other.imagesCachedExpressions;
    this->flagOpDefineEncountered=other.flagOpDefineEncountered;
  }
  void reset()
  { this->BoundVariables.Clear();
    this->cachedExpressions.Clear();
    this->imagesCachedExpressions.SetSize(0);
    this->flagOpDefineEncountered=false;
  }
};

class VariableNonBound
{
  public:
  std::string theName;
  int HandlerFunctionIndex;
  inline static int HashFunction(const VariableNonBound& input)
  { return MathRoutines::hashString(input.theName);
  }
  VariableNonBound(const char x[]):theName(x), HandlerFunctionIndex(-1){}
  VariableNonBound(const std::string& input):theName(input), HandlerFunctionIndex(-1) {}
  VariableNonBound():HandlerFunctionIndex(-1) {}
  std::string GetHandlerFunctionName(CommandList& owner);
  //C++ does have horrible syntax:
//the following returns a pointer to a function that return bool and takes theCommands, commandIndex and Expression
// as input
//[Edit:] masked the horror in a typedef. Now the horror lies in a single isolated line. Nevertheless, a horror.
  Function::FunctionAddress GetFunction
  (CommandList& owner)
  ;
  int HashFunction()const
  { return VariableNonBound::HashFunction(*this);
  }
  void operator=(const VariableNonBound& other)
  { this->theName=other.theName;
    this->HandlerFunctionIndex=other.HandlerFunctionIndex;
  }
  bool operator==(const VariableNonBound& other)const
  { return this->theName==other.theName;
  }
};


class CommandList
{
public:
//control sequences parametrize the syntactical elements
  HashedListB<std::string, MathRoutines::hashString> controlSequences;
//operations parametrize the expression elements
//operations are the labels of the nodes of the expression tree
//As operations can be thought of as functions, and functions are named by the class VariableNonBound,
//operations are in fact realized as elements of type VariableNonBound.
  HashedList<VariableNonBound> operations;
//Non bound variables can only be data entries of an expression x for which x.theOperation
//equals x.theBoss->opVariableNonBound()
  HashedList<VariableNonBound> theNonBoundVars;

  HashedList<Function> theFunctions;
  List<int> targetProperties;
  HashedListB<std::string, MathRoutines::hashString> theDictionary;
  List<Expression> buffer1, buffer2;
  int MaxRecursionDepthDefault;
  int MaxAlgTransformationsPerExpression;
  int MaxLatexChars;
  double MaxAllowedTimeInSeconds;
  ///////////////////////////////////////////////////////////////////////////
  bool flagTimeLimitErrorDetected;
  bool flagFirstErrorEncountered;
  bool flagMaxRecursionErrorEncountered;
  bool flagMaxTransformationsErrorEncountered;
  bool flagNewContextNeeded;

  bool flagLogSyntaxRules;
  ///////////////////////////////////////////////////////////////////////////
  int TotalNumPatternMatchedPerformed;
  int NumPredefinedVars;
  int numEmptyTokensStart;
  Expression theCommands;
//  std::vector<std::stringstream> theLogs;
  List<SyntacticElement> syntacticSoup;
  List<SyntacticElement> syntacticStack;

  List<ExpressionContext> theExpressionContext;
  ////
  HashedList<Expression> ExpressionStack;

  std::string syntaxErrors;
  List<std::string> evaluationErrors;
  std::string input;
  std::string output;
  std::string theLog;
//following are the variout hardcoded data structures.
  GlobalVariables* theGlobalVariableS;
  List<SemisimpleLieAlgebra> theLieAlgebras;
  HashedList<ElementSimpleLieAlgebra> theLieAlgebraElements;
  HashedListB<Data, Data::HashFunction> theData;
//end of hardcoded data structures
  std::string ElementToString();
  double StartTimeInSeconds;
  SyntacticElement GetSyntacticElementEnd()
  { SyntacticElement result;
    result.controlIndex=this->controlSequences.GetIndex(";");
    return result;
  }
  bool DecreaseStackSetCharacterRanges(int decrease)
  { if (decrease<=0)
      return true;
    assert( this->syntacticStack.size-decrease>0);
    this->syntacticStack[this->syntacticStack.size-decrease-1].IndexLastCharPlusOne=
    this->syntacticStack[this->syntacticStack.size-1].IndexLastCharPlusOne;
    this->syntacticStack.SetSize(this->syntacticStack.size-decrease);
    return true;
  }
  std::string ElementToStringSyntacticStack();
  std::string ElementToStringSyntactic(bool usePolishForm=false)
  ;
  bool isSeparatorFromTheLeftGeneral(const std::string& input);
  bool isSeparatorFromTheLeftForDefinition(const std::string& input);
  bool isSeparatorFromTheLeftForStatement(const std::string& input);
  bool isSeparatorFromTheLeftForList(const std::string& input);
  bool isSeparatorFromTheLeftForListMatrixRow(const std::string& input);
  bool isSeparatorFromTheLeftForMatrixRow(const std::string& input);

  bool isSeparatorFromTheRightGeneral(const std::string& input);
  bool isSeparatorFromTheRightForDefinition(const std::string& input);
  bool isSeparatorFromTheRightForStatement(const std::string& input);
  bool isSeparatorFromTheRightForList(const std::string& input);
  bool isSeparatorFromTheRightForListMatrixRow(const std::string& input);
  bool isSeparatorFromTheRightForMatrixRow(const std::string& input);

  bool LookAheadAllowsThePower(const std::string& lookAhead)
  { return lookAhead!="{}";
  }
  HashedListB<std::string, MathRoutines::hashString>& GetCurrentContextBoundVars()
  { return this->theExpressionContext.LastObject()->BoundVariables;
  }
  bool LookAheadAllowsApplyFunction(const std::string& lookAhead)
;
  bool LookAheadAllowsPlus(const std::string& lookAhead)
  ;
  bool LookAheadAllowsTimes(const std::string& lookAhead);
  bool LookAheadAllowsDivide(const std::string& lookAhead);
  bool ReplaceEXXEXEByEusingO(int theOp);
  bool ReplaceEEByEusingO(int theControlIndex);
  bool ReplaceEEndCommandEbyE();
  bool ReplaceOEByE(int formatOptions=Expression::formatDefault);
  bool ReplaceOXEByE(int formatOptions=Expression::formatDefault);
  bool ReplaceOXXEXEXEXByE(int formatOptions=Expression::formatDefault);
  bool ReplaceOXEXEXEXByE(int formatOptions=Expression::formatDefault);
  bool ReplaceEOEXByEX(int formatOptions=Expression::formatDefault);
  bool ReplaceECByC();
  bool ReplaceEXEByList(int theControlIndex, int inputFormat=Expression::formatDefault);
  bool ReplaceYXByListYX(int theControlIndex, int inputFormat=Expression::formatDefault)
  { return this->ReplaceYXdotsXByListYXdotsX(theControlIndex, inputFormat, 1);
  }
  bool ReplaceXOXbyEusingO(int theControlIndex, int inputFormat=Expression::formatDefault)
  ;
  bool ReplaceYByListY(int theControlIndex, int inputFormat=Expression::formatDefault)
  { return this->ReplaceYXdotsXByListYXdotsX(theControlIndex, inputFormat, 0);
  }
  bool ReplaceXXYByListY(int theControlIndex, int inputFormat=Expression::formatDefault)
  { this->ReplaceYByListY(theControlIndex, inputFormat);
    return this->ReplaceXXYByY();
  }
  bool ReplaceYXdotsXByListYXdotsX(int theControlIndex, int inputFormat=Expression::formatDefault, int numXs=0);
  bool ReplaceListXEByList(int theControlIndex, int inputFormat=Expression::formatDefault);
  bool ReplaceCEByC();
  bool ReplaceCCByC();
  bool ReplaceEOEByE(int formatOptions=Expression::formatDefault)
  { return this->ReplaceEXEByEusingO(this->syntacticStack[this->syntacticStack.size-2].controlIndex, formatOptions);
  }
  bool ReplaceXEXEXByEusingO(int theOperation, int formatOptions=Expression::formatDefault)
  ;
  bool ReplaceEXEByEusingO(int theOperation, int formatOptions=Expression::formatDefault)
  ;
  bool ReplaceXXByCon(int theCon, int theFormat=Expression::formatDefault)
  { this->syntacticStack[this->syntacticStack.size-2].controlIndex=theCon;
    this->syntacticStack[this->syntacticStack.size-2].theData.format=theFormat;
    this->DecreaseStackSetCharacterRanges(1);
    return true;
  }
  bool ReplaceXByCon(int theCon, int theFormat=Expression::formatDefault);
  bool ReplaceXByConCon(int con1, int con2, int format1=Expression::formatDefault, int format2=Expression::formatDefault);
  bool ReplaceXXXByCon(int theCon);
  bool ReplaceXXXByConCon(int con1, int con2, int inputFormat1=Expression::formatDefault, int inputFormat2=Expression::formatDefault);
  bool ReplaceXXXXXByCon(int theCon, int inputFormat=Expression::formatDefault);
  bool ReplaceXXXXXByConCon(int con1, int con2, int inputFormat1=Expression::formatDefault, int inputFormat2=Expression::formatDefault);
  bool ReplaceXXXXByConCon(int con1, int con2, int inputFormat1=Expression::formatDefault, int inputFormat2=Expression::formatDefault);
  bool ReplaceXXXXByCon(int con1, int inputFormat1=Expression::formatDefault);
  bool ReplaceXXYByY()
  { this->syntacticStack[this->syntacticStack.size-3]=this->syntacticStack[this->syntacticStack.size-1];
    this->syntacticStack.SetSize(this->syntacticStack.size-2);
    return true;
  }
  bool ReplaceXYYXByYY()
  { this->syntacticStack[this->syntacticStack.size-4]=this->syntacticStack[this->syntacticStack.size-3];
    this->syntacticStack[this->syntacticStack.size-3]=this->syntacticStack[this->syntacticStack.size-2];
    this->syntacticStack.SetSize(this->syntacticStack.size-2);
    return true;
  }
  bool ReplaceXYXByY()
  { this->syntacticStack[this->syntacticStack.size-3]=this->syntacticStack[this->syntacticStack.size-2];
    this->DecreaseStackSetCharacterRanges(2);
    return true;
  }
  bool ReplaceXYByY()
  { this->syntacticStack[this->syntacticStack.size-2]=this->syntacticStack[this->syntacticStack.size-1];
    this->syntacticStack.SetSize(this->syntacticStack.size-1);
    return true;
  }
  bool ReplaceXEXByE(int inputFormat=Expression::formatDefault)
  { this->syntacticStack[this->syntacticStack.size-3]=this->syntacticStack[this->syntacticStack.size-2];
    this->syntacticStack[this->syntacticStack.size-3].theData.format=inputFormat;
    this->DecreaseStackSetCharacterRanges(2);
    return true;
  }
  bool ReplaceObyE();
  bool ReplaceOXbyEX(int inputFormat=Expression::formatDefault);
  bool ReplaceOXbyEXusingO(int theControlIndex, int inputFormat=Expression::formatDefault);
  bool ReplaceOXbyE(int inputFormat=Expression::formatDefault);
  bool ReplaceVXbyEX();
  bool ReplaceVbyE();
  bool ReplaceIntIntBy10IntPlusInt()
  ;
  bool CreateNewExpressionContext();
  bool RegisterBoundVariable();
  int GetOperationIndexFromControlIndex(int controlIndex);
  int GetExpressionIndex();
  SyntacticElement GetEmptySyntacticElement();
  bool ApplyOneRule(const std::string& lookAhead);
  void resetStack()
  { SyntacticElement emptyElement=this->GetEmptySyntacticElement();
    this->syntacticStack.initFillInObject(this->numEmptyTokensStart, emptyElement);
  }
  int conError()
  { return this->controlSequences.GetIndexIMustContainTheObject("Error");
  }
  int conExpression()
  { return this->controlSequences.GetIndexIMustContainTheObject("Expression");
  }
  int conBindVariable()
  { return this->controlSequences.GetIndexIMustContainTheObject("{{}}");
  }
  int conData()
  { return this->controlSequences.GetIndexIMustContainTheObject("Data");
  }
  int conEqualEqual()
  { return this->controlSequences.GetIndexIMustContainTheObject("==");
  }
  int conApplyFunction()
  { return this->controlSequences.GetIndexIMustContainTheObject("{}");
  }
  int conTimes()
  { return this->controlSequences.GetIndexIMustContainTheObject("*");
  }
  int conEndStatement()
  { return this->controlSequences.GetIndexIMustContainTheObject(";");
  }
  int conDefineConditional()
  { return this->controlSequences.GetIndexIMustContainTheObject("if:=");
  }
  int conComma()
  { return this->controlSequences.GetIndexIMustContainTheObject(",");
  }
  int conDefine()
  { return this->controlSequences.GetIndexIMustContainTheObject(":=");
  }
  int conList()
  { return this->controlSequences.GetIndexIMustContainTheObject("OperationList");
  }
  int conListNoRepetition()
  { return this->controlSequences.GetIndexIMustContainTheObject("ListNoRepetition");
  }
  int conListMatrixRow()
  { return this->controlSequences.GetIndexIMustContainTheObject("ListMatrixRows");
  }
  int conMatrixRow()
  { return this->controlSequences.GetIndexIMustContainTheObject("MatrixRow");
  }
  int conMatrixSeparator()
  { return this->controlSequences.GetIndexIMustContainTheObject("MatrixSeparator");
  }
  int conMatrixRowSeparator()
  { return this->controlSequences.GetIndexIMustContainTheObject("MatrixRowSeparator");
  }
  int conLieBracket()
  { return this->controlSequences.GetIndexIMustContainTheObject("[]");
  }
  int conOpenBracket()
  { return this->controlSequences.GetIndexIMustContainTheObject("(");
  }
  int conCloseBracket()
  { return this->controlSequences.GetIndexIMustContainTheObject(")");
  }
  int opApplyFunction()
  { return this->operations.GetIndexIMustContainTheObject("{}");
  }
  int opDefine()
  { return this->operations.GetIndexIMustContainTheObject(":=");
  }
  int opDefineConditional()
  { return this->operations.GetIndexIMustContainTheObject("if:=");
  }
  int opThePower()
  { return this->operations.GetIndexIMustContainTheObject("^");
  }
  int opVariableNonBound()
  { return this->operations.GetIndexIMustContainTheObject("VariableNonBound");
  }
  int opEqualEqual()
  { return this->operations.GetIndexIMustContainTheObject("==");
  }
  int opData()
  { return this->operations.GetIndexIMustContainTheObject("Data");
  }
  int opList()
  { return this->operations.GetIndexIMustContainTheObject("OperationList");
  }
  int opUnion()
  { return this->operations.GetIndexIMustContainTheObject("\\cup");
  }
  int opEndStatement()
  { return this->operations.GetIndexIMustContainTheObject(";");
  }
  int opUnionNoRepetition()
  { return this->operations.GetIndexIMustContainTheObject("\\sqcup");
  }
  int opVariableBound()
  { return this->operations.GetIndexIMustContainTheObject("VariableBound");
  }
  int opPlus()
  { return this->operations.GetIndexIMustContainTheObject("+");
  }
  int opMinus()
  { return this->operations.GetIndexIMustContainTheObject("-");
  }
  int opTimes()
  { return this->operations.GetIndexIMustContainTheObject("*");
  }
  int opLieBracket()
  { return this->operations.GetIndexIMustContainTheObject("[]");
  }
  int opDivide()
  { return this->operations.GetIndexIMustContainTheObject("/");
  }
  bool AppendOpandsReturnTrueIfOrderNonCanonical
  (Expression& theExpression, List<Expression>& output, int theOp, int RecursionDepth, int MaxRecursionDepth)
;
  bool ExtractPolyRational
  (PolynomialRationalCoeff& output, const Expression& theInput, HashedList<Expression>& VariableImages,
   int RecursionDepth=0, int MaxRecursionDepthMustNotPopStack=10000, std::stringstream* errorLog=0)
  ;
  bool AppendMultiplicandsReturnTrueIfOrderNonCanonical
  (Expression& theExpression, List<Expression>& output, int RecursionDepth, int MaxRecursionDepth)
  { return this->AppendOpandsReturnTrueIfOrderNonCanonical(theExpression, output, this->opTimes(), RecursionDepth, MaxRecursionDepth);
  }
  bool AppendSummandsReturnTrueIfOrderNonCanonical
  (Expression& theExpression, List<Expression>& output, int RecursionDepth, int MaxRecursionDepth)
  { return this->AppendOpandsReturnTrueIfOrderNonCanonical(theExpression, output, this->opPlus(), RecursionDepth, MaxRecursionDepth);
  }
  void SpecializeBoundVars
(Expression& toBeSubbed, int targetCommandIndex, ExpressionPairs& matchedPairs, int RecursionDepth, int MaxRecursionDepth)
  ;
  bool ExpressionHasBoundVars(Expression& theExpression, int RecursionDepth, int MaxRecursionDepth)
  { if (RecursionDepth>MaxRecursionDepth)
    { std::stringstream out;
      out << "Max recursion depth of " << MaxRecursionDepth << " exceeded.";
      theExpression.errorString=out.str();
      return false;
    }
    if (theExpression.theOperation==this->opVariableBound())
      return true;
    else
      for (int i=0; i<theExpression.children.size; i++)
        if (this->ExpressionHasBoundVars(theExpression.children[i], RecursionDepth+1, MaxRecursionDepth+1))
          return true;
    return false;
  }
  Expression* PatternMatch
  (int inputIndexBoundVars, Expression& thePattern, Expression& theExpression,
   ExpressionPairs& bufferPairs, int RecursionDepth,
   int MaxRecursionDepth, Expression* condition=0, std::stringstream* theLog=0, bool logAttempts=false)
;
  bool ProcessOneExpressionOnePatternOneSub
  (int inputIndexBoundVars, Expression& thePattern, Expression& theExpression, ExpressionPairs& bufferPairs,
   int RecursionDepth, int maxRecursionDepth, std::stringstream* theLog=0, bool logAttempts=false)
  ;
/*  Expression GetProperty(const std::string& propertyName, int theData=0)
  { ExpressionProperty result;
    result.value=theData;
    result.nameIndex=this->thePropertyNames.GetIndex(propertyName);
    if (result.nameIndex==-1)
    { this->thePropertyNames.AddOnTop(propertyName);
      result.nameIndex=this->thePropertyNames.size()-1;
    }
    return result;
  }*/
  bool isADigit(const std::string& input, int& whichDigit)
  { if (input.size()!=1)
      return false;
    whichDigit=input[0]-'0';
    return whichDigit<10 && whichDigit>=0;
  }
//  bool OrderMultiplicationTreeProperly(int commandIndex, Expression& theExpression);
  bool CollectSummands(int inputIndexBoundVars, Expression& theExpression);
  bool ExpressionMatchesPattern
  (const Expression& thePattern, const Expression& input, ExpressionPairs& matchedExpressions,
   int RecursionDepth=0, int MaxRecursionDepth=500, std::stringstream* theLog=0)
  ;
  bool ExpressionsAreEqual
  (const Expression& left, const Expression& right, int RecursionDepth=0, int MaxRecursionDepth=500)
  ;

  static bool EvaluateStandardUnion
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
  ;
  static bool EvaluateStandardUnionNoRepetition
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
  ;
  static bool EvaluateStandardPlus
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
  ;
  static bool EvaluateStandardTimes
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
  ;
  static bool EvaluateStandardDivide
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
  ;
  static bool EvaluateStandardLieBracket
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
  ;
  static bool EvaluateStandardMinus
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
  ;
  static bool EvaluateStandardFunction
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
  ;
  static bool EvaluateStandardEqualEqual
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
  ;
  static bool EvaluateDoAssociatE
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* coments, int theOperation)
  ;
  static bool EvaluateDoCollect
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
  ;
  static bool EvaluateDoExtractBaseMultiplication
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
  ;
  static bool EvaluateDoDistribute
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments, int theMultiplicativeOP, int theAdditiveOp)
  ;
  static bool EvaluateDoLeftDistributeBracketIsOnTheLeft
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments, int theMultiplicativeOP, int theAdditiveOp)
  ;
  static bool EvaluateDoRightDistributeBracketIsOnTheRight
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments, int theMultiplicativeOP)
  ;
  static bool EvaluateIf
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
  ;
  static bool fMatrix
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
  ;
  static bool fIsInteger
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
  ;
  static bool fPolynomial
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
  ;
  static bool fSSAlgebra
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
;
  static bool fElementSSAlgebra
  (CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
;
  void AddEmptyHeadedCommand();
  CommandList()
  { this->theGlobalVariableS=0;
  }
  void AddOperationNoFail
  (const std::string& theOpName,
   const Function::FunctionAddress& theFunAddress,
   const std::string& opFunctionTechnicalName,
   const std::string& opArgumentList="", const std::string& opDescription="",
   const std::string& opExample=""
   )
   ;
  void AddNonBoundVarMustBeNew
  (const std::string& theName, const Function::FunctionAddress& funHandler,
   const std::string& argList, const std::string& description, const std::string& exampleArgs
   )
  { int theIndex=this->AddNonBoundVarReturnVarIndex(theName, funHandler, argList, description, exampleArgs);
    if (theIndex!=this->theNonBoundVars.size-1)
      assert(false);
  }
  int GetIndexNonBoundVar(const std::string& theName)
  { VariableNonBound tempVar;
    tempVar.theName=theName;
    return this->theNonBoundVars.GetIndex(tempVar);
  }
  int AddNonBoundVarReturnVarIndex
(const std::string& theName, const Function::FunctionAddress& funHandler,
  const std::string& argList, const std::string& description, const std::string& exampleArgs
)
;
  void init(GlobalVariables& inputGlobalVariables);
  void initPredefinedVars()
  ;
  void initTargetProperties()
  {// this->AddTargetProperty("IsDistributed", & this->EvaluateDoDistribute);
  }
  void ExtractExpressions();
  void EvaluateCommands();
  bool EvaluateExpressionReturnFalseIfExpressionIsBound
(Expression& theExpression, int RecursionDepth, int maxRecursionDepth,
 ExpressionPairs& bufferPairs, std::stringstream* theLog=0)
 ;
  void Evaluate(const std::string& theInput)
  { if (this->theGlobalVariableS==0)
    { this->output= "This is a programming error: commandList not initialized properly. Please report this bug. ";
      return;
    }
    this->StartTimeInSeconds=this->theGlobalVariableS->GetElapsedSeconds();
    this->input=theInput;
    this->ParseFillDictionary(this->input);
    this->ExtractExpressions();
    this->EvaluateCommands();
  }
  bool isLeftSeparator(char c);
  bool isRightSeparator(char c);
  void ParseFillDictionary(const std::string& input);
};

#endif
