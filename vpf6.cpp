//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#include "vpf.h"

template < > int HashedListB<VariableNonBound, VariableNonBound::HashFunction>::PreferredHashSize=50;
template < > int HashedListB<Expression, Expression::HashFunction>::PreferredHashSize=50;
template < > int HashedListB<int, MathRoutines::IntIdentity>::PreferredHashSize=50;
template < > int HashedListB<Data, Data::HashFunction>::PreferredHashSize=1000;
template < > int HashedListB<Function, Function::HashFunction>::PreferredHashSize=100;

template < > int List<SyntacticElement>::ListActualSizeIncrement=50;
template < > int List<Function>::ListActualSizeIncrement=50;
template < > int List<Data>::ListActualSizeIncrement=500;
template < > int List<SemisimpleLieAlgebra>::ListActualSizeIncrement=5;


inline int IntIdentity(const int& x)
{ return x;
}

void Expression::MakeProducT
  (CommandList& owner, int inputIndexBoundVars, const Expression& left, const Expression& right)
{ this->MakeXOX(owner, inputIndexBoundVars, owner.opTimes(), left, right);
}

void Expression::MakeFunction
  (CommandList& owner, int inputIndexBoundVars, const Expression& theFunction, const Expression& theArgument)
{ this->MakeXOX(owner, inputIndexBoundVars, owner.opApplyFunction(), theFunction, theArgument);
}

void Expression::MakeXOX
  (CommandList& owner, int inputIndexBoundVars, int theOp, const Expression& left, const Expression& right)
{ this->reset(owner, inputIndexBoundVars);
  this->theOperation=theOp;
  this->children.SetSize(2);
  this->children[0]=left;
  this->children[1]=right;
}

bool CommandList::ReplaceOXEXEXEXByE(int formatOptions)
{ SyntacticElement& opElt=this->syntacticStack[this->syntacticStack.size-8];
  SyntacticElement& leftE = this->syntacticStack[this->syntacticStack.size-6];
  SyntacticElement& middleE= this->syntacticStack[this->syntacticStack.size-4];
  SyntacticElement& rightE = this->syntacticStack[this->syntacticStack.size-2];
  Expression newExpr;
  newExpr.reset(*this, this->theExpressionContext.size-1);
  newExpr.theOperation=this->GetOperationIndexFromControlIndex(opElt.controlIndex);
  newExpr.format=formatOptions;
  newExpr.children.AddOnTop(leftE.theData);
  newExpr.children.AddOnTop(middleE.theData);
  newExpr.children.AddOnTop(rightE.theData);
  opElt.theData=newExpr;
  opElt.controlIndex=this->conExpression();
  this->DecreaseStackSetCharacterRanges(7);
//    std::cout << this->syntacticStack[this->syntacticStack.size()-1].theData.ElementToStringPolishForm();
  return true;
}

bool CommandList::ReplaceOXXEXEXEXByE(int formatOptions)
{ SyntacticElement& opElt=this->syntacticStack[this->syntacticStack.size-9];
  SyntacticElement& leftE = this->syntacticStack[this->syntacticStack.size-6];
  SyntacticElement& middleE= this->syntacticStack[this->syntacticStack.size-4];
  SyntacticElement& rightE = this->syntacticStack[this->syntacticStack.size-2];
  Expression newExpr;
  newExpr.reset(*this, this->theExpressionContext.size-1);
  newExpr.theOperation=this->GetOperationIndexFromControlIndex(opElt.controlIndex);
  newExpr.format= formatOptions;
  newExpr.children.AddOnTop(leftE.theData);
  newExpr.children.AddOnTop(middleE.theData);
  newExpr.children.AddOnTop(rightE.theData);
  opElt.theData=newExpr;
  opElt.controlIndex=this->conExpression();
  this->DecreaseStackSetCharacterRanges(8);
//    std::cout << this->syntacticStack[this->syntacticStack.size()-1].theData.ElementToStringPolishForm();
  return true;
}

bool CommandList::ReplaceOXEByE(int formatOptions)
{ SyntacticElement& left=this->syntacticStack[this->syntacticStack.size-3];
  SyntacticElement& right = this->syntacticStack[this->syntacticStack.size-1];
  Expression newExpr;
  newExpr.reset(*this, this->theExpressionContext.size-1);
  newExpr.theOperation=this->GetOperationIndexFromControlIndex(left.controlIndex);
  newExpr.format= formatOptions;
  newExpr.children.AddOnTop(right.theData);
  left.theData=newExpr;
  left.controlIndex=this->conExpression();
  this->DecreaseStackSetCharacterRanges(2);
//    std::cout << this->syntacticStack[this->syntacticStack.size()-1].theData.ElementToStringPolishForm();
  return true;
}

bool CommandList::ReplaceOEByE(int formatOptions)
{ SyntacticElement& middle=this->syntacticStack[this->syntacticStack.size-2];
  SyntacticElement& right = this->syntacticStack[this->syntacticStack.size-1];
  Expression newExpr;
  newExpr.reset(*this, this->theExpressionContext.size-1);
  newExpr.theOperation=this->GetOperationIndexFromControlIndex(middle.controlIndex);
  newExpr.format= formatOptions;
  newExpr.children.AddOnTop(right.theData);
  middle.theData=newExpr;
  middle.controlIndex=this->conExpression();
  this->DecreaseStackSetCharacterRanges(1);
//    std::cout << this->syntacticStack[this->syntacticStack.size()-1].theData.ElementToStringPolishForm();
  return true;
}

bool CommandList::isRightSeparator(char c)
{ switch(c)
  { case ' ':
    case '\n':
    case ':':
    case ',':
    case ';':
    case '+':
    case '*':
    case '^':
    case '=':
    case '_':
    case '/':
    case '-':
    case '`':
    case '[':
    case '&':
    case '}':
    case '{':
    case '~':
    case '$':
    case ']':
    case '(':
    case ')':
      return true;
    default:
      return false;
  }
}

bool CommandList::isLeftSeparator(char c)
{ switch(c)
  { case ' ':
    case '\n':
    case ':':
    case ',':
    case ';':
    case '+':
    case '*':
    case '^':
    case '&':
    case '=':
    case '_':
    case '/':
    case '-':
    case '`':
    case '[':
    case '}':
    case '{':
    case '~':
    case '$':
    case ']':
    case '(':
    case ')':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return true;
    default:
      return false;
  }
}

SyntacticElement CommandList::GetEmptySyntacticElement()
{ SyntacticElement result;
  result.controlIndex=this->controlSequences.GetIndex(" ");
  result.theData.reset(*this, -1);
  return result;
}

std::string ExpressionPairs::ElementToString()
{ std::stringstream out;
  out << "the pairs: ";
  for (int i=0; i<this->BoundVariableIndices.size; i++)
    out << this->BoundVariableIndices[i] << "->" << this->variableImages[i].ElementToString() << "<br>";
  return out.str();
}

void CommandList::ParseFillDictionary
  (const std::string& input)
{ std::string current;
  this->theCommands.reset(*this, -1);
  this->syntacticSoup.Reserve(1000);
  this->syntacticSoup.SetSize(0);
  char LookAheadChar;
  SyntacticElement currentElement;
  int currentDigit;
  for (unsigned i=0; i<input.size(); i++)
  { current.push_back(input[i]);
    if (i+1<input.size())
      LookAheadChar=input[i+1];
    else
      LookAheadChar=' ';
    if (current==" " || current=="\n")
      current="";
    else
      if (this->isLeftSeparator(current[0]) || this->isRightSeparator(LookAheadChar) )
      { if (this->controlSequences.Contains(current))
        { currentElement.controlIndex=this->controlSequences.GetIndex(current);
          currentElement.theData.reset(*this, -1);
          this->syntacticSoup.AddOnTop(currentElement);
        } else if (this->isADigit(current, currentDigit))
        { Data tempData= Data(currentDigit, *this);
          currentElement.controlIndex=this->conData();
          currentElement.theData.reset(*this, -1);
          currentElement.theData.MakeDatA(tempData, *this, -1);
          this->syntacticSoup.AddOnTop(currentElement);
        } else
        { this->theDictionary.AddNoRepetition(current);
          currentElement.controlIndex=this->controlSequences.GetIndex("Variable");
          currentElement.theData.reset(*this, -1);
          currentElement.theData.theOperation=this->operations.GetIndex("Variable");
          currentElement.theData.theData=this->theDictionary.GetIndex(current);
          this->syntacticSoup.AddOnTop(currentElement);
        }
        current="";
      }
  }
}

int CommandList::GetOperationIndexFromControlIndex(int controlIndex)
{ return this->operations.GetIndex(this->controlSequences[controlIndex]);
}

int CommandList::GetExpressionIndex()
{ return this->controlSequences.GetIndex("Expression");
}

bool CommandList::ReplaceOXbyE(int inputFormat)
{ this->ReplaceOXbyEX(inputFormat);
  this->DecreaseStackSetCharacterRanges(1);
  return true;
}

bool CommandList::ReplaceXOXbyEusingO(int theControlIndex, int inputFormat)
{ this->ReplaceOXbyEXusingO(theControlIndex, inputFormat);
  return this->ReplaceXEXByE(inputFormat);
}

bool CommandList::ReplaceOXbyEX(int inputFormat)
{ SyntacticElement& theElt=this->syntacticStack[this->syntacticStack.size-2];
  return this->ReplaceOXbyEXusingO(theElt.controlIndex, inputFormat);
}

bool CommandList::ReplaceOXbyEXusingO(int theControlIndex, int inputFormat)
{ SyntacticElement& theElt=this->syntacticStack[this->syntacticStack.size-2];
  theElt.theData.theOperation=this->GetOperationIndexFromControlIndex(theControlIndex);
  theElt.theData.IndexBoundVars=this->theExpressionContext.size-1;
  theElt.theData.format=inputFormat;
  theElt.controlIndex=this->conExpression();
  return true;
}

bool CommandList::ReplaceObyE()
{ SyntacticElement& theElt=this->syntacticStack[this->syntacticStack.size-1];
  theElt.theData.theOperation=this->GetOperationIndexFromControlIndex(theElt.controlIndex);
  theElt.theData.IndexBoundVars=this->theExpressionContext.size-1;
  theElt.controlIndex=this->conExpression();
  return true;
}

bool CommandList::ReplaceVbyE()
{ SyntacticElement& theElt=this->syntacticStack[this->syntacticStack.size-1];
  const std::string& theVarString=this->theDictionary[theElt.theData.theData];
  int indexBoundVar=this->GetCurrentContextBoundVars().GetIndex(theVarString);
  if (indexBoundVar!=- 1)
  { theElt.theData.theOperation=this->opVariableBound();
    theElt.theData.theData=indexBoundVar;
  } else
  { theElt.theData.theOperation=this->opVariableNonBound();
    theElt.theData.theData=this->AddNonBoundVarReturnVarIndex(theVarString, 0, "", "", "");
    //note:     theElt.theData.theOperation.theData     should be initialized with the index of the non-bound variable!
  }
  theElt.controlIndex=this->conExpression();
  theElt.theData.IndexBoundVars=this->theExpressionContext.size-1;
//  std::cout << "now i'm here!";
  return true;
}

bool CommandList::ReplaceVXbyEX()
{ SyntacticElement& theElt=this->syntacticStack[this->syntacticStack.size-2];
  const std::string& theVarString=this->theDictionary[theElt.theData.theData];
  int indexBoundVar=this->GetCurrentContextBoundVars().GetIndex(theVarString);
  if (indexBoundVar!=- 1)
  { theElt.theData.theOperation=this->opVariableBound();
    theElt.theData.theData=indexBoundVar;
  } else
  { theElt.theData.theOperation=this->opVariableNonBound();
    theElt.theData.theData=this->AddNonBoundVarReturnVarIndex(theVarString, 0, "", "", "");
  }
  theElt.theData.IndexBoundVars=this->theExpressionContext.size-1;
  theElt.controlIndex=this->conExpression();
  return true;
}

bool CommandList::CreateNewExpressionContext()
{ this->theExpressionContext.SetSize(this->theExpressionContext.size+1);
  this->theExpressionContext.LastObject()->reset();
  return true;
}

bool CommandList::RegisterBoundVariable()
{ SyntacticElement& theElt=this->syntacticStack[this->syntacticStack.size-3];
  const std::string& theVarString=this->theDictionary[theElt.theData.theData];
  if (!this->GetCurrentContextBoundVars().Contains(theVarString))
  { if (!this->theExpressionContext.LastObject()->flagOpDefineEncountered)
      this->GetCurrentContextBoundVars().AddOnTop(theVarString);
    else
    { theElt.controlIndex=this->conError();
      theElt.ErrorString=  "Error: bound variables cannot be declared after the definition operation := .";
    }
  } else
  { this->DecreaseStackSetCharacterRanges(2);
    this->ReplaceXXYByY();
  }
  return true;
}

bool CommandList::LookAheadAllowsApplyFunction(const std::string& lookAhead)
{ return lookAhead!="{" && lookAhead!="_" && lookAhead!="\\circ" && lookAhead!="{}" &&  lookAhead!="$";
}

bool CommandList::ReplaceListXEByList(int theControlIndex, int inputFormat)
{ SyntacticElement& left = this->syntacticStack[this->syntacticStack.size-3];
  SyntacticElement& right = this->syntacticStack[this->syntacticStack.size-1];
  Expression newExpr;
  newExpr.reset(*this, this->theExpressionContext.size-1);
  newExpr.theOperation=this->opList();
  newExpr.children.AddListOnTop(left.theData.children);
  newExpr.children.AddOnTop(right.theData);
  newExpr.format=inputFormat;
  left.theData=newExpr;
  left.controlIndex=theControlIndex;
  this->DecreaseStackSetCharacterRanges(2);
//    std::cout << this->syntacticStack[this->syntacticStack.size()-1].theData.ElementToStringPolishForm();
  return true;
}

bool CommandList::ReplaceYXdotsXByListYXdotsX(int theControlIndex, int inputFormat, int numXs)
{ SyntacticElement& main = this->syntacticStack[this->syntacticStack.size-numXs-1];
  Expression newExpr;
  newExpr.reset(*this, this->theExpressionContext.size-1);
  newExpr.theOperation=this->opList();
  newExpr.children.AddOnTop(main.theData);
  newExpr.format=inputFormat;
  main.theData=newExpr;
  main.controlIndex=theControlIndex;
  return true;
}

bool CommandList::ReplaceEXEByList(int theControlIndex, int inputFormat)
{ SyntacticElement& left = this->syntacticStack[this->syntacticStack.size-3];
  SyntacticElement& right = this->syntacticStack[this->syntacticStack.size-1];
  Expression newExpr;
  newExpr.reset(*this, this->theExpressionContext.size-1);
  newExpr.theOperation=this->opList();
  newExpr.children.AddOnTop(left.theData);
  newExpr.children.AddOnTop(right.theData);
  newExpr.format=inputFormat;
  left.theData=newExpr;
  left.controlIndex=theControlIndex;
  this->DecreaseStackSetCharacterRanges(2);
//    std::cout << this->syntacticStack[this->syntacticStack.size()-1].theData.ElementToStringPolishForm();
  return true;
}

bool CommandList::ReplaceEEByEusingO(int theControlIndex)
{ SyntacticElement& left = this->syntacticStack[this->syntacticStack.size-2];
  SyntacticElement& right = this->syntacticStack[this->syntacticStack.size-1];
  Expression newExpr;
  newExpr.reset(*this, this->theExpressionContext.size-1);
  newExpr.theOperation=this->GetOperationIndexFromControlIndex(theControlIndex);
  //    newExpr.inputIndexBoundVars=
  newExpr.children.AddOnTop(left.theData);
  newExpr.children.AddOnTop(right.theData);
  left.theData=newExpr;
  this->DecreaseStackSetCharacterRanges(1);
  //    std::cout << this->syntacticStack[this->syntacticStack.size()-1].theData.ElementToStringPolishForm();
  return true;
}

bool CommandList::ReplaceEXXEXEByEusingO(int theControlIndex)
{ SyntacticElement& middle = this->syntacticStack[this->syntacticStack.size-3];
  SyntacticElement& left = this->syntacticStack[this->syntacticStack.size-6];
  SyntacticElement& right = this->syntacticStack[this->syntacticStack.size-1];
  Expression newExpr;
  newExpr.reset(*this, this->theExpressionContext.size-1);
  newExpr.theOperation=this->GetOperationIndexFromControlIndex(theControlIndex);
  newExpr.children.AddOnTop(left.theData);
  newExpr.children.AddOnTop(middle.theData);
  newExpr.children.AddOnTop(right.theData);
  left.theData=newExpr;
  this->DecreaseStackSetCharacterRanges(5);
//    std::cout << this->syntacticStack[this->syntacticStack.size()-1].theData.ElementToStringPolishForm();
  return true;
}

bool CommandList::ReplaceEOEXByEX(int formatOptions)
{ SyntacticElement& middle=this->syntacticStack[this->syntacticStack.size-3];
  SyntacticElement& left = this->syntacticStack[this->syntacticStack.size-4];
  SyntacticElement& right = this->syntacticStack[this->syntacticStack.size-2];
  Expression newExpr;
  newExpr.reset(*this, this->theExpressionContext.size-1);
  newExpr.theOperation=this->GetOperationIndexFromControlIndex(middle.controlIndex);
  newExpr.children.AddOnTop(left.theData);
  newExpr.children.AddOnTop(right.theData);
  newExpr.format=formatOptions;
  left.theData=newExpr;
  middle=*this->syntacticStack.LastObject();
  left.IndexLastCharPlusOne=right.IndexLastCharPlusOne;
  this->syntacticStack.SetSize(this->syntacticStack.size-2);
//    std::cout << this->syntacticStack[this->syntacticStack.size()-1].theData.ElementToStringPolishForm();
  return true;
}

bool CommandList::isSeparatorFromTheLeftGeneral(const std::string& input)
{ return input=="{" || input=="(" || input=="[" || input=="," || input==":" || input==";" || input==" "
  || input=="MatrixSeparator" || input == "MatrixRowSeparator" || input=="&";
}

bool CommandList::isSeparatorFromTheRightGeneral(const std::string& input)
{ return input=="}" || input==")" || input=="]" || input=="," || input==":" || input==";" || input=="MatrixSeparator"
  || input=="MatrixRowSeparator" || input=="\\\\" || input=="\\end" || input=="&";
}

bool CommandList::isSeparatorFromTheLeftForList(const std::string& input)
{ return input=="{" || input=="(" || input=="[" || input==":" || input==";" || input==" "
  || input=="MatrixSeparator" || input == "MatrixRowSeparator" ;
}

bool CommandList::isSeparatorFromTheLeftForListMatrixRow(const std::string& input)
{ return input=="{" || input=="(" || input=="[" || input==":" || input==";" || input==" "
  || input=="MatrixSeparator" || input == "MatrixRowSeparator" ;
}

bool CommandList::isSeparatorFromTheLeftForMatrixRow(const std::string& input)
{ return input=="{" || input=="(" || input=="[" || input==":" || input==";" || input==" "
  || input=="MatrixSeparator" || input == "MatrixRowSeparator" ;
}

bool CommandList::isSeparatorFromTheRightForList(const std::string& input)
{ return input=="}" || input==")" || input=="]" || input==":" || input==";" || input=="MatrixSeparator"
  || input=="MatrixRowSeparator" || input=="\\\\" || input=="\\end";
}

bool CommandList::isSeparatorFromTheRightForListMatrixRow(const std::string& input)
{ return input=="}" || input==")" || input=="]" || input==":" || input==";" || input=="MatrixSeparator"
  || input=="MatrixRowSeparator" || input=="\\\\" || input=="\\end";
}

bool CommandList::isSeparatorFromTheRightForMatrixRow(const std::string& input)
{ return input=="}" || input==")" || input=="]" || input==":" || input==";" || input=="MatrixSeparator" || input=="&"
  || input=="MatrixRowSeparator" || input=="\\\\" || input=="\\end";
}

bool CommandList::isSeparatorFromTheLeftForDefinition(const std::string& input)
{ return input=="{" || input=="(" || input==";" || input==" ";
}

bool CommandList::isSeparatorFromTheRightForDefinition(const std::string& input)
{ return input=="}" || input==")" || input==";";
}

bool CommandList::isSeparatorFromTheLeftForStatement(const std::string& input)
{ return input=="{" || input=="(" || input==";" || input==" ";
}

bool CommandList::isSeparatorFromTheRightForStatement(const std::string& input)
{ return input=="}" || input==")" || input==";";
}

bool CommandList::LookAheadAllowsTimes(const std::string& lookAhead)
{ return
    lookAhead=="+" || lookAhead=="-" ||
    lookAhead=="*" || lookAhead=="/" ||
    lookAhead=="Expression" || lookAhead==")" ||
    lookAhead=="Variable" ||
    lookAhead==";" || lookAhead=="]" ||
    lookAhead=="}" || lookAhead==":"
    ;
}

bool CommandList::LookAheadAllowsPlus(const std::string& lookAhead)
{ return
    lookAhead=="+" || lookAhead=="-" ||
    lookAhead=="," ||
    lookAhead==")" || lookAhead==";" ||
    lookAhead=="]" || lookAhead=="}" ||
    lookAhead==":"
    ;
}

bool CommandList::LookAheadAllowsDivide(const std::string& lookAhead)
{ return this->LookAheadAllowsTimes(lookAhead);
}

bool CommandList::ApplyOneRule(const std::string& lookAhead)
{ //return false;
  const SyntacticElement& lastE=this->syntacticStack[this->syntacticStack.size-1];
  const std::string& lastS=this->controlSequences[lastE.controlIndex];
  if (lastS==" " && signed (this->syntacticStack.size)>this->numEmptyTokensStart)
  { this->syntacticStack.SetSize(this->syntacticStack.size-1);
    return false;
  }
  const SyntacticElement& secondToLastE=this->syntacticStack[this->syntacticStack.size-2];
  const std::string& secondToLastS=this->controlSequences[secondToLastE.controlIndex];
  const SyntacticElement& thirdToLastE=this->syntacticStack[this->syntacticStack.size-3];
  const std::string& thirdToLastS=this->controlSequences[thirdToLastE.controlIndex];
  const SyntacticElement& fourthToLastE=this->syntacticStack[this->syntacticStack.size-4];
  const std::string& fourthToLastS=this->controlSequences[fourthToLastE.controlIndex];
  const SyntacticElement& fifthToLastE=this->syntacticStack[this->syntacticStack.size-5];
  const std::string& fifthToLastS=this->controlSequences[fifthToLastE.controlIndex];
  const SyntacticElement& sixthToLastE=this->syntacticStack[this->syntacticStack.size-6];
  const std::string& sixthToLastS=this->controlSequences[sixthToLastE.controlIndex];
  const SyntacticElement& seventhToLastE=this->syntacticStack[this->syntacticStack.size-7];
  const std::string& seventhToLastS=this->controlSequences[seventhToLastE.controlIndex];
//  const SyntacticElement& eighthToLastE=this->syntacticStack[this->syntacticStack.size-8];
//  const std::string& eighthToLastS=this->theBoss->controlSequences[eighthToLastE.controlIndex];
//  const SyntacticElement& ninthToLastE=this->syntacticStack[this->syntacticStack.size()-9];
//  const std::string& ninthToLastS=this->theBoss->controlSequences[ninthToLastE.controlIndex];
  if (this->flagLogSyntaxRules)
    std::cout <<"<hr>" << this->ElementToStringSyntacticStack();
  if (lastE.theData.IndexBoundVars==-1)
  { std::cout << "<hr>The last expression while reducing " << this->ElementToStringSyntacticStack()
    << " does not have properly initialized context.";
    assert(false);
  }
  if (secondToLastS==":" && lastS=="=")
  { this->theExpressionContext.LastObject()->flagOpDefineEncountered=true;
    return this->ReplaceXXByCon(this->conDefine());
  }
  if (secondToLastS=="{" && lastS=="}")
    return this->ReplaceXXByCon(this->conApplyFunction(), Expression::formatDefault);
  if (lastS=="\\cdot")
    return this->ReplaceXByCon(this->conApplyFunction(), Expression::formatFunctionUseCdot);
//  if ( thirdToLastS=="{" && secondToLastS=="{}" && lastS=="}")
//    return this->ReplaceXXXByCon(this->conBindVariable());
  if (fifthToLastS=="{" && fourthToLastS=="{" && thirdToLastS=="Variable" && secondToLastS=="}" && lastS=="}")
    return this->RegisterBoundVariable();
  if (lastS=="Variable" && (lookAhead!="}" || secondToLastS!="{"|| thirdToLastS!="{"))
    return this->ReplaceVbyE();
  if (lastS=="=" && secondToLastS=="=")
    return this->ReplaceXXByCon(this->conEqualEqual());
  if (lastS=="Data" && secondToLastS=="Data")
    return this->ReplaceIntIntBy10IntPlusInt();
  if (lastS=="Data" && lookAhead!="Data")
    return this->ReplaceObyE();
  //there is an ambiguity on how should function application be associated
  //Which is better: x{}y{}z:= x{} (y{}z), or x{}y{}z:=(x{}y){}z ?
  //In our implementation, we choose x{}y{}z:= x{} (y{}z). Although this is slightly harder to implement,
  //it appears to be the more natural choice.
//  if (fourthToLastS=="Expression" && thirdToLastS=="{}" && secondToLastS=="Expression"
//      && this->LookAheadAllowsApplyFunction(lastS) )
//    return this->ReplaceEOEXByEX(secondToLastE.theData.format);
  if (thirdToLastS=="Expression" && secondToLastS=="{}" && lastS=="Expression" && this->LookAheadAllowsApplyFunction(lookAhead))
    return this->ReplaceEOEByE();
  if (thirdToLastS=="Expression" && secondToLastS=="_" && lastS=="Expression" && lookAhead!="_")
    return this->ReplaceEXEByEusingO(this->conApplyFunction(), Expression::formatFunctionUseUnderscore);
  //end of ambiguity.
  if (fourthToLastS=="{"  && thirdToLastS=="{}" && secondToLastS=="Expression" && lastS=="}")
    return this->ReplaceXYYXByYY();
  if (lastS=="Expression" && secondToLastS=="^" && thirdToLastS=="Expression" && this->LookAheadAllowsThePower(lookAhead) )
    return this->ReplaceEOEByE();
  if (lastS=="Expression" && secondToLastS=="+" && thirdToLastS=="Expression" && this->LookAheadAllowsPlus(lookAhead) )
    return this->ReplaceEOEByE();
  if (lastS=="Expression" && secondToLastS=="-" && thirdToLastS=="Expression" && this->LookAheadAllowsPlus(lookAhead) )
    return this->ReplaceEOEByE();
  if (lastS=="Expression" && secondToLastS=="-" && this->LookAheadAllowsPlus(lookAhead) )
    return this->ReplaceOEByE();
  if (lastS=="Expression" && secondToLastS=="*" && thirdToLastS=="Expression" && this->LookAheadAllowsTimes(lookAhead) )
    return this->ReplaceEOEByE(Expression::formatTimesDenotedByStar);
  if (lastS=="Expression" && secondToLastS=="/" && thirdToLastS=="Expression" && this->LookAheadAllowsDivide(lookAhead) )
    return this->ReplaceEOEByE();
  if (lastS=="Expression" && secondToLastS=="Expression" && this->LookAheadAllowsTimes(lookAhead) )
    return this->ReplaceEEByEusingO(this->conTimes());
  if (thirdToLastS=="List" && secondToLastS=="{}" && lastS=="Expression")
    return this->ReplaceXXYByListY(this->conExpression(), lastE.theData.format);
  if (thirdToLastS=="(" && secondToLastS=="Expression" && lastS==")")
    return this->ReplaceXEXByE(secondToLastE.theData.format);
  if (thirdToLastS=="{" && secondToLastS=="Expression" && lastS=="}")
    return this->ReplaceXEXByE(Expression::formatNoBracketsForFunctionArgument);
  if (lastS=="Expression" && secondToLastS=="~" && thirdToLastS=="Expression" )
    return this->ReplaceEOEByE();
  if (this->isSeparatorFromTheRightGeneral(lookAhead) && lastS=="Expression" && secondToLastS=="==" && thirdToLastS=="Expression")
    return this->ReplaceEOEByE();
  if (this->isSeparatorFromTheLeftForDefinition(fourthToLastS) && thirdToLastS=="Expression" && secondToLastS==":=" && lastS=="Expression" && this->isSeparatorFromTheRightForDefinition(lookAhead))
    return this->ReplaceEOEByE();
  if (thirdToLastS=="Expression" && secondToLastS=="\\cup" && lastS== "Expression" && this->isSeparatorFromTheRightGeneral(lookAhead))
    return this->ReplaceEOEByE();
  if (thirdToLastS=="Expression" && secondToLastS=="\\sqcup" && lastS== "Expression" && this->isSeparatorFromTheRightGeneral(lookAhead))
    return this->ReplaceEOEByE();
  if (thirdToLastS!="[" && secondToLastS=="Expression" && lastS==",")
    return this->ReplaceYXByListYX(this->conList(), secondToLastE.theData.format);
  if (thirdToLastS=="OperationList" && secondToLastS==","  && lastS=="Expression" && this->isSeparatorFromTheRightGeneral(lookAhead))
    return this->ReplaceListXEByList(this->conList(), Expression::formatDefault);
  if (this->isSeparatorFromTheLeftForList(thirdToLastS) && secondToLastS=="OperationList" && this->isSeparatorFromTheRightForList(lastS))
    return this->ReplaceOXbyEXusingO(this->conList(), Expression::formatDefault);
  if (fifthToLastS=="\\begin" && fourthToLastS=="{" && thirdToLastS=="array" && secondToLastS=="}" && lastS=="Expression")
    return this->ReplaceXXXXXByCon(this->conMatrixSeparator(), Expression::formatMatrix);
  if (fourthToLastS=="\\end" && thirdToLastS=="{" && secondToLastS=="array" && lastS=="}" )
    return this->ReplaceXXXXByCon(this->conMatrixSeparator(), Expression::formatMatrix);
  if (lastS=="\\\\")
    return this->ReplaceXByCon(this->conMatrixRowSeparator(), Expression::formatMatrixRow);
 if (secondToLastS=="Expression" && lastS=="&")
    return this->ReplaceYXByListYX(this->conMatrixRow(), Expression::formatMatrixRow);
  if (thirdToLastS=="MatrixRow" && secondToLastS=="&"  && lastS=="Expression" && this->isSeparatorFromTheRightForMatrixRow(lookAhead))
    return this->ReplaceListXEByList(this->conMatrixRow(), Expression::formatMatrixRow);
  if (secondToLastS=="Expression" && (lastS=="MatrixRowSeparator" || lastS=="MatrixSeparator"))
    return this->ReplaceYXByListYX(this->conMatrixRow(), Expression::formatMatrixRow);
  if (secondToLastS=="MatrixRow" &&  (lastS=="MatrixRowSeparator" || lastS=="MatrixSeparator"))
    return this->ReplaceYXByListYX(this->conListMatrixRow(), Expression::formatMatrix);
  if (thirdToLastS=="ListMatrixRows" && secondToLastS=="MatrixRowSeparator" && lastS=="Expression" && this->isSeparatorFromTheRightForListMatrixRow(lookAhead) )
    return this->ReplaceYByListY(this->conMatrixRow(), Expression::formatMatrixRow);
  if (thirdToLastS=="ListMatrixRows" && secondToLastS=="MatrixRowSeparator" && lastS=="MatrixRow" && this->isSeparatorFromTheRightForListMatrixRow(lookAhead) )
    return this->ReplaceListXEByList(this->conListMatrixRow(), Expression::formatMatrix);
  if (thirdToLastS=="MatrixSeparator" && secondToLastS=="ListMatrixRows" && lastS=="MatrixSeparator")
    return this->ReplaceXOXbyEusingO(this->conList(), Expression::formatMatrix);
  if (fifthToLastS=="[" && fourthToLastS=="Expression" && thirdToLastS=="," && secondToLastS=="Expression" && lastS=="]")
    return this->ReplaceXEXEXByEusingO(this->conLieBracket());
  if (this->isSeparatorFromTheLeftForDefinition(seventhToLastS) && lastS=="Expression" && secondToLastS==":=" && thirdToLastS=="Expression" && fourthToLastS=="if" && fifthToLastS==":" && sixthToLastS=="Expression" && this->isSeparatorFromTheRightForDefinition(lookAhead))
    return this->ReplaceEXXEXEByEusingO(this->conDefineConditional());
  if (this->isSeparatorFromTheLeftForStatement(fourthToLastS) && thirdToLastS=="Expression" && secondToLastS==";" && lastS=="Expression" && this->isSeparatorFromTheRightForStatement(lookAhead))
    return this->ReplaceEEndCommandEbyE();
  if (lastS==";" && lookAhead==";")
    return this->DecreaseStackSetCharacterRanges(1);
  return false;
}

template <class theType>
bool Expression::GetVector
(Vector<theType>& output, int targetDimNonMandatory, Function::FunctionAddress conversionFunction, std::stringstream* comments)
{ if (this->theOperation!=this->theBoss->opList())
  { if (targetDimNonMandatory!=-1)
      if (targetDimNonMandatory!=1)
        return false;
    Data* currentData;
    if (this->theOperation!=this->theBoss->opData())
    { Expression tempExpression=*this;
      if (conversionFunction!=0)
        if (!conversionFunction(*this->theBoss, this->IndexBoundVars, tempExpression, comments))
          return false;
      if (tempExpression.theOperation!=this->theBoss->opData())
        return false;
      currentData=&this->theBoss->theData[tempExpression.theData];
    } else
      currentData=&this->theBoss->theData[this->theData];
    if (!currentData->IsOfType<theType>())
      return false;
    output.SetSize(1);
    output.TheObjects[0]=currentData->GetValue<theType>();
    return true;
  }
  if (targetDimNonMandatory!=-1)
    if (targetDimNonMandatory!=this->children.size)
      return false;
  output.SetSize(this->children.size);
  for (int i=0; i<output.size; i++)
  { Expression& currentE=this->children[i];
    Data* currentData;
    if (this->theOperation!=this->theBoss->opData())
    { Expression tempExpression=currentE;
      if (conversionFunction!=0)
        if (!conversionFunction(*this->theBoss, this->IndexBoundVars, tempExpression, 0))
          return false;
      if (tempExpression.theOperation!=this->theBoss->opData())
        return false;
      currentData=&this->theBoss->theData[tempExpression.theData];
    } else
      currentData=&this->theBoss->theData[currentE.theData];
    if (!currentData->IsOfType<theType>())
      return false;
    output.TheObjects[i]=currentData->GetValue<theType>();
  }
  return true;
}

bool CommandList::fHWV
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ if (theExpression.children.size!=3)
  { if (comments!=0)
      *comments << "Function HWV is expected to have three arguments: SS algebra type, List{}, List{}. ";
    return false;
  }
  Expression& leftE=theExpression.children[0];
  Expression& middleE=theExpression.children[1];
  Expression& rightE=theExpression.children[2];
  Expression resultSSalgebraE;
  resultSSalgebraE=leftE;
  if (!CommandList::fSSAlgebra(theCommands, inputIndexBoundVars, resultSSalgebraE, comments))
  { if(comments!=0)
      *comments << "Failed to create a semisimple Lie algebra from the first argument, which is " << leftE.ElementToString();
    return false;
  }
  if  (resultSSalgebraE.errorString!="")
  { if(comments!=0)
      *comments << "Failed to create a semisimple Lie algebra from the first argument, which is " << leftE.ElementToString();
    return false;
  }
  if (resultSSalgebraE.theOperation!=theCommands.opData())
  { if(comments!=0)
      *comments << "Failed to create a semisimple Lie algebra from the first argument, which is " << leftE.ElementToString();
    return false;
  }
  Data& theSSdata=theCommands.theData[resultSSalgebraE.theData];
  if (theSSdata.type!=Data::typeSSalgebra)
  { if(comments!=0)
      *comments << "Failed to create a semisimple Lie algebra from the first argument, which is " << leftE.ElementToString();
    return false;
  }
  if (theSSdata.GetSmallInt()>= theCommands.theLieAlgebras.size)
  { std::cout << "This is a programming error: semisimple Lie algebra referenced but not allocated. "
    << "Please debug file " << __FILE__ << " line  " << __LINE__ << ".";
    assert(false);
  }
  SemisimpleLieAlgebra& theSSalgebra=theCommands.theLieAlgebras[theSSdata.GetSmallInt()];
  int theRank=theSSalgebra.GetRank();
  Vector<PolynomialRationalCoeff> highestWeight;
  Vector<Rational> parabolicSel;
  if (!middleE.GetVector<PolynomialRationalCoeff>(highestWeight, theRank, &CommandList::fPolynomial, comments))
  { if(comments!=0)
      *comments << "Failed to convert the second argument of HWV to a list of " << theRank
      << " polynomials. The second argument you gave is " << middleE.ElementToString() << ".";
    return false;
  }
  if (!rightE.GetVector<Rational>(parabolicSel, theRank, 0, comments))
  { if(comments!=0)
      *comments << "Failed to convert the third argument of HWV to a list of " << theRank
      << " rationals. The third argument you gave is " << rightE.ElementToString() << ".";
    return false;
  }
    Vector<RationalFunction> theWeight;
/*  theWeight=weight;
  RationalFunction RFOne, RFZero;
  RFOne.MakeNVarConst(theNode.impliedNumVars, 1, & theGlobalVariables);
  RFZero.Nullify(theNode.impliedNumVars, & theGlobalVariables);
  std::string report;
  ElementTensorsGeneralizedVermas<RationalFunction>& theElt=theNode.theGenVermaElt.GetElement();
  List<ModuleSSalgebraNew<RationalFunction> >& theMods=theNode.owner->theModulePolys;
  int indexOfModule=-1;
  Selection selectionParSel;
  selectionParSel=parSel;
  for (int i=0; i<theMods.size; i++)
  { ModuleSSalgebraNew<RationalFunction>& currentMod=theMods[i];
    if (theWeight==currentMod.theHWFundamentalCoordsBaseField &&
          selectionParSel==currentMod.parabolicSelectionNonSelectedAreElementsLevi )
    { indexOfModule=i;
      break;
    }
  }
  if (indexOfModule==-1)
  { indexOfModule=theMods.size;
    theMods.SetSize(theMods.size+1);
  }
  ModuleSSalgebraNew<RationalFunction>& theMod=theMods[indexOfModule];
  if (!theMod.flagIsInitialized)
  { assert(theWeight[0].NumVars==RFOne.NumVars);
    bool isGood=theMod.MakeFromHW
    (theNode.owner->theHmm.theRange(), theWeight, parSel, theGlobalVariables, RFOne, RFZero, &report);

    out << report;
    if (!isGood)
      return theNode.SetError(theNode.errorImplicitRequirementNotSatisfied);
  }
  theElt.MakeHWV(theMods, indexOfModule, RFOne);
  theNode.outputString=out.str();
  theNode.ExpressionType=theNode.typeGenVermaElt;
  return theNode.errorNoError;*/
  if (comments!=0)
    *comments << "Yeeeepeeeeeee!!!!!  "
    << "<br>The highest weight: " << highestWeight.ElementToString() << "<br> The par sel: " << parabolicSel.ElementToString();
  return false;
}

bool CommandList::fMatrix
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ if (theExpression.children.size!=3)
    return false;
  Expression& leftE=theExpression.children[0];
  Expression& middleE=theExpression.children[1];
  Expression& rightE=theExpression.children[2];
  int numRows, numCols;
  if (!middleE.IsSmallInteger(numRows) || !rightE.IsSmallInteger(numCols))
    return false;
  if (numRows<=0 || numCols<=0)
    return false;
  if (numRows>1000 || numCols>1000)
  { if (comments!=0)
      *comments << "Max number of rows/columns is 1000. You requested " << numRows << " rows and "
      << numCols << " columns.<br>";
    return false;
  }
  Expression Result;
  Result.reset(theCommands, inputIndexBoundVars);
  Result.theOperation=theCommands.opList();
  Result.format=Result.formatMatrix;
  Result.children.SetSize(numRows);
  Expression theIndices;
  theIndices.reset(theCommands, inputIndexBoundVars);
  theIndices.theOperation=theCommands.opList();
  theIndices.children.SetSize(2);
  for (int i=0; i<numRows; i++)
  { Expression& currentRow=Result.children[i];
    currentRow.reset(theCommands, inputIndexBoundVars);
    currentRow.theOperation=theCommands.opList();
    currentRow.children.SetSize(numCols);
    currentRow.format=currentRow.formatMatrixRow;
    for (int j=0; j<numCols; j++)
    { Expression& currentE=currentRow.children[j];
      theIndices.children[0].MakeInt(i+1, theCommands, inputIndexBoundVars);
      theIndices.children[1].MakeInt(j+1, theCommands, inputIndexBoundVars);
      currentE.MakeFunction(theCommands, inputIndexBoundVars, leftE, theIndices);
    }
  }
  theExpression=Result;
  return true;
}

bool CommandList::fPolynomial
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ Data outputData(theCommands);
  std::stringstream errorLog;
  outputData.type=outputData.typePoly;
  HashedList<Expression> VariableImages;
  if (!theCommands.ExtractPolyRational(outputData.thePoly.GetElement(), theExpression, VariableImages, 0, 10000, &errorLog))
  { outputData.type=outputData.typeError;
    outputData.theError.GetElement()=errorLog.str();
  }
  theExpression.theOperation=theCommands.opData();
  theExpression.theData=theCommands.theData.AddNoRepetitionOrReturnIndexFirst(outputData);
  if (comments!=0)
    for (int i=0; i<VariableImages.size; i++)
    { *comments  << "x_{" << i+1 << "}:=" << VariableImages[i].ElementToString(0, 10);
      if (i!=VariableImages.size-1)
        *comments << ";<br>";
    }
  return true;
}

bool CommandList::fSSAlgebra
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ std::stringstream errorStream;
  errorStream << "Error: the simple Lie algebra takes as argument of the form VariableNonBound_Data "
    <<" (in mathematical language Type_Rank). Instead I recieved " << theExpression.ElementToString() << " at file "
    << __FILE__ << " line " <<  __LINE__ << ".";
  if (theExpression.children.size!=2)
  { errorStream << " The input of the function does not have two arguments";
    return theExpression.SetError(errorStream.str());
  }
  Expression& typeE=theExpression.children[0];
  Expression& rankE=theExpression.children[1];
  if (rankE.theOperation!=theCommands.opData() || typeE.theOperation!=theCommands.opVariableNonBound())
  { errorStream << " The first node is " << typeE.ElementToString() << ". The second node is "
    << rankE.ElementToString();
    return theExpression.SetError(errorStream.str());
  }
  int theRank=1;
  if (!theCommands.theData[rankE.theData].IsSmallInteger(theRank))
  { errorStream << "The rank of a Lie algebra must be a small integer.";
    return theExpression.SetError(errorStream.str());
  }
  if (theRank<1 || theRank>8)
  { errorStream << "The rank of a simple Lie algebra must be between 1 and 8; you entered " << theRank << " instead.";
    return theExpression.SetError(errorStream.str());
  }
  std::string& theTypeName=theCommands.theNonBoundVars[typeE.theData].theName;
  if (theTypeName.size()!=1)
  { errorStream << "The type of a simple Lie algebra must be the letter A, B, C, D, E, F or G. Instead, it is "
    << theTypeName << ".";
    return theExpression.SetError(errorStream.str());
  }
  char theWeylLetter=theTypeName[0];
  if (theWeylLetter=='a') theWeylLetter='A';
  if (theWeylLetter=='b') theWeylLetter='B';
  if (theWeylLetter=='c') theWeylLetter='C';
  if (theWeylLetter=='d') theWeylLetter='D';
  if (theWeylLetter=='e') theWeylLetter='E';
  if (theWeylLetter=='f') theWeylLetter='F';
  if (theWeylLetter=='g') theWeylLetter='G';
  if (!(theWeylLetter=='A' || theWeylLetter=='B' || theWeylLetter=='C' || theWeylLetter=='D' || theWeylLetter=='E' || theWeylLetter=='F' || theWeylLetter=='G'))
  { errorStream << "The type of a simple Lie algebra must be the letter A, B, C, D, E, F or G.";
    return theExpression.SetError(errorStream.str());
  }
  int oldSize=theCommands.theLieAlgebras.size;
  SemisimpleLieAlgebra theSSalgebra;
  theSSalgebra.ComputeChevalleyConstantS(theWeylLetter, theRank, theCommands.theLieAlgebras, *theCommands.theGlobalVariableS);
  Data tempData(theCommands);
  tempData.theRational.GetElement()=theCommands.theLieAlgebras.AddNoRepetitionOrReturnIndexFirst(theSSalgebra);
  if (oldSize<theCommands.theLieAlgebras.size)
    if (comments!=0)
      *comments << "Lie algebra of type " << theSSalgebra.GetLieAlgebraName()
      << " generated. The resulting multiplication table is " << theSSalgebra.ElementToString(*theCommands.theGlobalVariableS);
  tempData.type=tempData.typeSSalgebra;
  theExpression.theData=theCommands.theData.AddNoRepetitionOrReturnIndexFirst(tempData);
  theExpression.theOperation=theCommands.opData();
  theExpression.children.SetSize(0);
  return true;
}

bool Expression::HasBoundVariables(int RecursionDepth, int MaxRecursionDepth)
{ if (RecursionDepth>MaxRecursionDepth)
  { std::cout << "This is a programming error: function HasBoundVariables has exceeded recursion depth limit. "
    << "Please debug file " << __FILE__ << " line " << __LINE__ << ". ";
    assert(false);
  }
  if (this->theOperation==this->theBoss->opVariableBound())
    return true;
  for (int i=0; i<this->children.size; i++)
    if (this->children[i].HasBoundVariables(RecursionDepth, MaxRecursionDepth))
      return true;
  return false;
}

bool CommandList::fIsInteger
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ if (theExpression.HasBoundVariables(0, theCommands.MaxRecursionDepthDefault))
    return false;
  if (theExpression.IsInteger())
    theExpression.MakeDatA(1, theCommands, inputIndexBoundVars);
  else
    theExpression.MakeDatA(0, theCommands, inputIndexBoundVars);
  return true;
}

bool CommandList::AppendOpandsReturnTrueIfOrderNonCanonical
  (Expression& theExpression, List<Expression>& output, int theOp, int RecursionDepth, int MaxRecursionDepth)
{ if (RecursionDepth>MaxRecursionDepthDefault)
    return false;
  bool result=false;
  if (theExpression.theOperation!=theOp)
    output.AddOnTop(theExpression);
  else
    for (int i=0; i<theExpression.children.size; i++)
    { if (this->AppendOpandsReturnTrueIfOrderNonCanonical(theExpression.children[i], output, theOp, RecursionDepth+1, MaxRecursionDepth))
        result=true;
      if (i<theExpression.children.size-1 &&
          theExpression.children[i].theOperation==theOp &&
          theExpression.children[i].children.size>1)
        result=true;
    }
  return result;
}

void CommandList::initPredefinedVars()
{ this->AddNonBoundVarMustBeNew
("IsInteger", &this->fIsInteger, "",
 " If the argument has no bound variables, returns 1 if the argument is an integer, 0 otherwise. ", "IsInteger{}a;\nIsInteger{}1;\nf{}{{a}}:=IsInteger{}a;\nf{}1;\nf{}b");
  this->AddNonBoundVarMustBeNew
  ("SemisimpleLieAlgebra", & this->fSSAlgebra, "",
   "Creates a simple Lie algebra. Will be changed to creating a semisimple Lie algebra in the foreseeable future. \
   Creates a function that returns the elements of a simple Lie algebra with Weyl type and rank \
   given in the format WeylLetter_Rank.  \
   Elements of the cartan are addressed as arguments of the form (0,s), root system generators are addressed \
   with one index only. ", "g:=SemisimpleLieAlgebra{}G_2; g_1; g_2; g_{0,1}; [[g_1,g_2], [g_{-1}, g_{-2}]]");
  this->AddNonBoundVarMustBeNew
  ("Polynomial", & this->fPolynomial, "",
   "Creates a c++ representation of a polynomial expression. The data structure is implemented without \
   use of expression trees, and obeys substitution rules as a single object (expression with a single node).",
   "Polynomial{}(x(x+y)^2+(x{}x)^2)");
  this->AddNonBoundVarMustBeNew
  ("FunctionToMatrix", & this->fMatrix, "",
   "Creates a matrix from a function. The first argument gives the function, the second argument the number of rows, \
   the third- the number of columns.\
   ", "X:=FunctionToMatrix{}(A,5,6); A{}({{a}},{{b}}):=a+b; X;");
  this->AddNonBoundVarMustBeNew
  ("Union", & this->EvaluateStandardUnion, "",
   "Makes a union of the elements of its arguments. Same action as \\cup but different syntax; useful for matrices. ",
   "X:=FunctionToMatrix{}(A,3,4); Union{}X; A{}({{i}},{{j}}):=i*i-j*j; Union{}X ");
  this->AddNonBoundVarMustBeNew
  ("UnionNoRepetition", & this->EvaluateStandardUnionNoRepetition, "",
   "Same action as \\sqcup (union no repetition) but different syntax; useful for matrices. ",
   "X:=FunctionToMatrix{}(A,3,4); UnionNoRepetition{}X; A{}({{i}},{{j}}):=i*i-j*j; UnionNoRepetition{}X");
  this->AddNonBoundVarMustBeNew
  ("hwv", & this->fHWV, "",
   "Highest weight vector . ", "hwv{}(B_3, (x_1,0,1),(1,0,0))");

  this->NumPredefinedVars=this->theNonBoundVars.size;
}

void CommandList::init(GlobalVariables& inputGlobalVariables)
{ this->theGlobalVariableS=& inputGlobalVariables;
  this->MaxAlgTransformationsPerExpression=100000;
  this->MaxRecursionDepthDefault=10000;
  this->MaxAllowedTimeInSeconds=inputGlobalVariables.MaxAllowedComputationTimeInSeconds/2;
  this->flagLogSyntaxRules=false;
  this->flagNewContextNeeded=true;
  this->MaxLatexChars=2000;
  this->numEmptyTokensStart=9;
  this->controlSequences.Clear();
  this->operations.Clear();
  this->syntacticSoup.SetSize(0);
  this->syntacticStack.SetSize(0);
  this->syntacticSoup.Reserve(1000);
  this->syntacticStack.Reserve(1000);
  this->flagTimeLimitErrorDetected=false;
  this->flagFirstErrorEncountered=false;
  this->flagMaxTransformationsErrorEncountered=false;
  this->flagMaxRecursionErrorEncountered=false;
  this->theNonBoundVars.Clear();
  this->theExpressionContext.SetSize(0);
  this->ExpressionStack.Clear();

  this->theData.Clear();
  this->theDictionary.Clear();
  this->theFunctions.Clear();
  this->syntaxErrors="";
  this->evaluationErrors.SetSize(0);
  this->targetProperties.SetSize(0);
  this->AddOperationNoFail
  ("+", this->EvaluateStandardPlus, "",
   "Collects all terms (over the rationals), adding up terms proportional up to a rational number. \
    Zero summands are removed, unless zero is the only term left. ", "1+a-2a_1+1/2+a_1", false);
  this->AddOperationNoFail
  ("-", this->EvaluateStandardMinus, "",
   "Transforms a-b to a+(-1)*b and -b to (-1)*b. Equivalent to a rule \
   -{{b}}:=MinnusOne*b; {{a}}-{{b}}:=a+MinnusOne*b", "-1+(-5)", false);
  this->AddOperationNoFail
  ("/", this->EvaluateStandardDivide, "",
    "If a and b are rational computes a/b. Otherwise does nothing.", "3/5+(a+b)/5", false);
  this->AddOperationNoFail
  ("*", this->EvaluateStandardTimes, "",
   "1) Reorders all multiplicative terms in regular order, e.g. ((a*b)*(c*d))*f:=a*(b*(c*(d*f))). \
   2) Applies the left and right distributive laws ({{a}}+{{b}})*{{c}}:=a*c+b*c; {{c}}*({{a}}+{{b}}):=c*a+c*b. \
   3) 3.1) If a and b are rational, computes a*b. 3.2) If b is rational, substitutes a*b by b*a (i.e. {{a}}{{b}}:if IsRational{} b:=b*a;). \
   3.3) If the expression is of the form a*(b*c) and  a and b are rational, substitutes a*(b*c) by (a*b)*c. \
   3.4) If the expression is of the form a*(b*c) and b is rational but a is not, substitutes the expression by b*(a*c). ",
   "2*c_1*d*3", false);
  this->AddOperationNoFail
  ("[]", this->EvaluateStandardLieBracket, "",
   "Lie bracket. Not documented at the moment, as the calculator implementation of semisimple Lie \
   algebra elements might change.", "g:=SemisimpleLieAlgebra{}A_1; [g_1,g_{-1}] ", false);
  this->AddOperationNoFail(":=", 0, "", "", "", false);
  this->AddOperationNoFail("if:=", 0, "", "", "", false);
  this->AddOperationNoFail("^", 0, "", "", "", false);
  this->AddOperationNoFail
  ("==", this->EvaluateStandardEqualEqual, "",
   "Evaluates to 1 if the left argument equals the right argument, otherwise evaluates to zero.",
   "x==y; x:=1; y:=1; x==y", false);
  //the following operation for function application is chosen on purpose so that it corresponds to LaTeX-undetectable
  //expression
  this->AddOperationNoFail
  ("{}", this->EvaluateStandardFunction, "",
   "1) If the first argument of {} is rational, the operation substitutes the expression with that constant. \
   2) If the first argument is of type predefined data but not rational, invokes the c++ implementation of the \
   dereference operator of the data. \
   3) If the first argument of {} has a hard-coded handler function, invokes that function.", "f{}(x)+1{}(x)-(1/2){}x ", false);
  //the following is the binding variable operation
  this->AddOperationNoFail("VariableNonBound", 0, "", "", "", false);
  this->AddOperationNoFail("VariableBound", 0, "", "", "", false);
  this->AddOperationNoFail("OperationList", 0, "", "", "", false);
//  this->AddOperationNoFail("Matrix", 0, "Matrix", "", "", "");
  this->AddOperationNoFail
  ("\\cup", this->EvaluateStandardUnion, "",
   "If all arguments of \\cup are of type list, substitutes the expression with a list containing \
   the union of all members (with repetition).", "x\\cup List{} x \\cup List{}x \\cup (a,b,x)", false);
  this->AddOperationNoFail
  ("\\sqcup", this->EvaluateStandardUnionNoRepetition, "",
   "If all arguments of \\sqcup are of type list, substitutes the expression with a list containing \
   the union of all members; all repeating members are discarded.", "(x,y,x)\\sqcup(1,x,y,2)", false);
  this->AddOperationNoFail("Error", 0, "", "", "", false);
  this->AddOperationNoFail("Data", 0, "", "", "", false);
  this->AddOperationNoFail(";", 0, "", "", "", false);

  this->controlSequences.AddOnTop(" ");//empty token must always come first!!!!
  this->controlSequences.AddOnTop("{{}}");
  this->controlSequences.AddOnTop("Variable");
  for (int i=0; i<this->operations.size; i++)
    this->controlSequences.AddOnTop(this->operations[i].theName);//all operations are also control sequences
  this->controlSequences.AddOnTop("Expression");
  this->controlSequences.AddOnTop(",");
  this->controlSequences.AddOnTop("if");
  this->controlSequences.AddOnTop("\\cdot");
  this->controlSequences.AddOnTop("_");
  this->controlSequences.AddOnTop("(");
  this->controlSequences.AddOnTop(")");
  this->controlSequences.AddOnTop("[");
  this->controlSequences.AddOnTop("]");
  this->controlSequences.AddOnTop("{");
  this->controlSequences.AddOnTop("}");
  this->controlSequences.AddOnTop(":");
  this->controlSequences.AddOnTop("ListMatrixRows");
  this->controlSequences.AddOnTop("List");
  this->controlSequences.AddOnTop("MatrixRow");
  this->controlSequences.AddOnTop("=");
  this->controlSequences.AddOnTop("$");
  this->controlSequences.AddOnTop("MatrixSeparator");
  this->controlSequences.AddOnTop("MatrixRowSeparator");
  this->controlSequences.AddOnTop("\\begin");
  this->controlSequences.AddOnTop("array");
  this->controlSequences.AddOnTop("\\end");
  this->controlSequences.AddOnTop("\\\\");
  this->controlSequences.AddOnTop("&");
//  this->controlSequences.AddOnTop("c...c");
//    this->thePropertyNames.AddOnTop("IsCommutative");
  this->TotalNumPatternMatchedPerformed=0;
  this->initPredefinedVars();
  this->initTargetProperties();
}

bool CommandList::CollectSummands(int inputIndexBoundVars, Expression& theExpression)
{ List<Expression>& summands= this->buffer1;
  summands.SetSize(0);
  bool needSimplification=this->AppendSummandsReturnTrueIfOrderNonCanonical
  (theExpression, summands, 0, this->MaxRecursionDepthDefault);
  HashedList<Expression> summandsNoCoeff;
  List<Rational> theCoeffs;
  Rational constTerm=0;
  bool foundConstTerm=false;
//  std::cout << "<b>" << theExpression.ElementToString() << "</b>";
//  if (theExpression.ElementToString()=="(4)*(a) b+(a) b")
//    std::cout << "problem!";
  for (int i=0; i<summands.size; i++)
  { Expression* currentSummandNoCoeff;
    currentSummandNoCoeff=&summands[i];
//    this->OrderMultiplicationTreeProperly(inputIndexBoundVars, *currentSummandNoCoeff);
    Rational theCoeff=1;
    if (currentSummandNoCoeff->theOperation==this->opTimes())
    { if(currentSummandNoCoeff->children[0].theOperation==this->opData())
      { Data& curData=this->theData[currentSummandNoCoeff->children[0].theData];
        if (curData.type==curData.typeRational)
        { theCoeff=curData.theRational.GetElement();
          currentSummandNoCoeff=& currentSummandNoCoeff->children[1];
          if (theCoeff.IsEqualToZero())
            needSimplification=true;
        }
      }
    } else if (currentSummandNoCoeff->theOperation==this->opData())
    { Data& curData=this->theData[currentSummandNoCoeff->theData];
      if (curData.type==curData.typeRational)
      { if (!foundConstTerm)
          constTerm=curData.theRational.GetElement();
        else
          constTerm+=curData.theRational.GetElement();
        if (foundConstTerm || curData.IsEqualToZero())
          needSimplification=true;
        foundConstTerm=true;
        continue;
      }
    }
    int currentIndex=summandsNoCoeff.GetIndex(*currentSummandNoCoeff);
    if (currentIndex==-1)
    { summandsNoCoeff.AddOnTop(*currentSummandNoCoeff);
      theCoeffs.AddOnTop(0);
      currentIndex=summandsNoCoeff.size-1;
    } else
      needSimplification=true;
    theCoeffs[currentIndex]+=theCoeff;
  }
/*
  std::cout << "<hr>summands: ";
  for (unsigned i=0; i< summands.size(); i++)
    std::cout << summands[i].ElementToString() << ", ";
  std::cout << " const term: " << constTerm;
    std::cout << "<br>coeff->summand_no_coeff: ";
  for (int i=0; i< summandsNoCoeff.size(); i++)
    std::cout << theCoeffs[i] << "->" << summandsNoCoeff[i].ElementToString() << ", ";
  std::cout << " const term: " << constTerm;
*/
  if (!needSimplification)
    return false;
  List<Expression> summandsWithCoeff;
  summandsWithCoeff.SetSize(0);
  summandsWithCoeff.Reserve(summandsNoCoeff.size);
  for (int i=0; i<summandsNoCoeff.size; i++)
  { if (theCoeffs[i].IsEqualToZero())
      continue;
    summandsWithCoeff.SetSize(summandsWithCoeff.size+1);
    Expression& current=summandsWithCoeff[summandsWithCoeff.size-1];
    if (!theCoeffs[i].IsEqualToOne())
    { current.reset(*this, inputIndexBoundVars);
      current.theOperation=this->opTimes();
      current.children.SetSize(2);
      current.children[0].MakeDatA(theCoeffs[i], *this, inputIndexBoundVars);
      current.children[1]=summandsNoCoeff[i];
    } else
      current=summandsNoCoeff[i];
  }
  if (!constTerm.IsEqualToZero() || summandsWithCoeff.size==0)
  { summandsWithCoeff.SetSize(summandsWithCoeff.size+1);
    Expression& current=summandsWithCoeff[summandsWithCoeff.size-1];
    current.MakeDatA(constTerm, *this, inputIndexBoundVars);
  }
  if (summandsWithCoeff.size==1)
  { theExpression=summandsWithCoeff[0];
    return true;
  }
  Expression* currentExp;
  currentExp=&theExpression;
  for (int i=0; i<summandsWithCoeff.size-1; i++)
  { currentExp->reset(*this, inputIndexBoundVars);
    currentExp->theOperation=this->opPlus();
    currentExp->children.SetSize(2);
    currentExp->children[0]=summandsWithCoeff[i];
    currentExp=& currentExp->children[1];
  }
  *currentExp=summandsWithCoeff[summandsWithCoeff.size-1];
  return true;
}

bool CommandList::EvaluateStandardTimes
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ //std::cout << "<br>At start of evaluate standard times: " << theExpression.ElementToString();
  if (theCommands.EvaluateDoDistribute
      (theCommands, inputIndexBoundVars, theExpression, comments, theCommands.opTimes(), theCommands.opPlus()))
    return true;
  //std::cout << "<br>After distribute: " << theExpression.ElementToString();
  if (theCommands.EvaluateDoAssociatE
      (theCommands, inputIndexBoundVars, theExpression, comments, theCommands.opTimes()))
    return true;
  if (theCommands.EvaluateDoExtractBaseMultiplication(theCommands, inputIndexBoundVars, theExpression, comments))
    return true;
  if (theExpression.children.size!=2)
    return false;
  //std::cout << "<br>After do associate: " << theExpression.ElementToString();
  return false;
}

bool CommandList::EvaluateDoExtractBaseMultiplication
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ if (theExpression.children.size!=2 || theExpression.theOperation!=theCommands.opTimes())
    return false;
  bool result=false;
  Expression& leftE=theExpression.children[0];
  Expression& rightE=theExpression.children[1];
  //handle Rational*Rational:
  if (leftE.theOperation==theCommands.opData() && rightE.theOperation==theCommands.opData())
  { Data& leftD=theCommands.theData[leftE.theData];
    Data& rightD=theCommands.theData[rightE.theData];
    if (leftD.type==leftD.typeRational && rightD.type==rightD.typeRational)
    { theExpression.MakeDatA(leftD*rightD, theCommands, inputIndexBoundVars);
      return true;
    }
  }
  //handle Anything*Rational:=Rational*Anything
  if (rightE.theOperation==theCommands.opData())
    if (theCommands.theData[rightE.theData].type==Data::typeRational)
    { MathRoutines::swap(leftE, rightE);
      result=true;
    }
  if (rightE.theOperation==theCommands.opTimes())
  { assert(rightE.children.size==2);
    Expression& rightLeftE=rightE.children[0];
    //handle Anything1*(Rational*Anything2):=Rational*(Anything1*Anything2)
    if (rightLeftE.theOperation==theCommands.opData())
      if (theCommands.theData[rightLeftE.theData].type==Data::typeRational)
      { MathRoutines::swap(rightLeftE, leftE);
        result=true;
      }
    //handle Rational1*(Rational2*Anything):=(Rational1*Rational2)*Anything
    if (leftE.theOperation==theCommands.opData() && rightLeftE.theOperation==theCommands.opData())
    { Data& leftD=theCommands.theData[leftE.theData];
      Data& righLefttD=theCommands.theData[rightLeftE.theData];
      if (leftD.type==leftD.typeRational && righLefttD.type==righLefttD.typeRational)
      { leftE.MakeDatA(leftD*righLefttD, theCommands, inputIndexBoundVars);
        rightE.AssignChild(1);
        result=true;
      }
    }
  }
  //handle 0*Anything:=0
  if (leftE.theOperation==theCommands.opData() )
  { Data& leftD=theCommands.theData[leftE.theData];
    if (leftD.type==leftD.typeRational && leftD.IsEqualToZero())
    { theExpression.MakeInt(0, theCommands, inputIndexBoundVars);
      result=true;
    }
  }
  return result;
}

bool CommandList::EvaluateDoAssociatE
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments, int theOperation)
{ List<Expression>& opands=theCommands.buffer1;
  opands.SetSize(0);
  //std::cout << "<br>At start of do associate: " << theExpression.ElementToString();
  bool needsModification=theCommands.AppendOpandsReturnTrueIfOrderNonCanonical
  (theExpression, opands, theOperation, 0, theCommands.MaxRecursionDepthDefault);
  if (!needsModification)
    return false;
  Expression* currentExpression;
  currentExpression=&theExpression;
  Expression emptyE;
  for (int i=0; i<opands.size-1; i++)
  { currentExpression->MakeXOX(theCommands, inputIndexBoundVars, theOperation, opands[i], emptyE);
    currentExpression=&currentExpression->children[1];
  }
  *currentExpression=*opands.LastObject();
  //std::cout << "<br>At end do associate: " << theExpression.ElementToString();
  return true;
}

bool CommandList::EvaluateDoDistribute
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments, int theMultiplicativeOP, int theAdditiveOp)
{ Expression& left=theExpression.children[0];
  Expression& right=theExpression.children[1];
  if ((left.theOperation==theCommands.opPlus() || left.theOperation==theCommands.opMinus()) && left.children.size==2)
  { theCommands.EvaluateDoLeftDistributeBracketIsOnTheLeft(theCommands, inputIndexBoundVars, theExpression, comments, theMultiplicativeOP, theAdditiveOp);
    return true;
  }
  if ((right.theOperation==theCommands.opPlus() || right.theOperation==theCommands.opMinus()) && right.children.size==2)
  { theCommands.EvaluateDoRightDistributeBracketIsOnTheRight(theCommands, inputIndexBoundVars, theExpression, comments, theMultiplicativeOP);
    return true;
  }
  return false;
}

bool CommandList::EvaluateDoLeftDistributeBracketIsOnTheLeft
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments, int theMultiplicativeOP, int theAdditiveOp)
{ if (theExpression.theOperation!=theMultiplicativeOP)
    return false;
  if (theExpression.children[0].theOperation!=theAdditiveOp)
    return false;
  if (theExpression.children[0].children.size!=2)
    return false;
  Expression left=theExpression.children[0];
  Expression right=theExpression.children[1];
  int theFormat=theExpression.format;
  theExpression.reset(theCommands, inputIndexBoundVars);
  theExpression.theOperation=theAdditiveOp;
  theExpression.children.SetSize(2);
  theExpression.children[0].MakeXOX(theCommands, inputIndexBoundVars, theMultiplicativeOP, left.children[0], right);
  theExpression.children[1].MakeXOX(theCommands, inputIndexBoundVars, theMultiplicativeOP, left.children[1], right);
  theExpression.children[0].format=theFormat;
  theExpression.children[1].format=theFormat;
  return true;
}

bool CommandList::EvaluateDoRightDistributeBracketIsOnTheRight
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments, int theMultiplicativeOP)
{ if (theExpression.theOperation!=theMultiplicativeOP)
    return false;
  int rightOp=theExpression.children[1].theOperation;
  if (rightOp!=theCommands.opPlus() && rightOp!=theCommands.opMinus())
    return false;
  if (theExpression.children[1].children.size!=2)
    return false;
  Expression left=theExpression.children[0];
  Expression right=theExpression.children[1];
  int theFormat=theExpression.format;
  theExpression.reset(theCommands, inputIndexBoundVars);
  theExpression.theOperation=rightOp;
  theExpression.children.SetSize(2);
  theExpression.children[0].MakeXOX(theCommands, inputIndexBoundVars, theMultiplicativeOP, left, right.children[0]);
  theExpression.children[1].MakeXOX(theCommands, inputIndexBoundVars, theMultiplicativeOP, left, right.children[1]);
  theExpression.children[0].format=theFormat;
  theExpression.children[1].format=theFormat;
  return true;
}

bool CommandList::EvaluateStandardPlus
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ return theCommands.CollectSummands(inputIndexBoundVars, theExpression);
}

bool CommandList::EvaluateIf
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ if (theExpression.children.size!=3)
  { theExpression.errorString="Programming error: operation :if := takes three arguments.";
    return true;
  }
  Expression& left= theExpression.children[0];
  if (left.theOperation!=theCommands.opData())
    return false;
  if (theCommands.theData[left.theData].IsEqualToOne())
  { theExpression.AssignChild(1);
    return true;
  }
  if (theCommands.theData[left.theData].IsEqualToZero())
  { theExpression.AssignChild(2);
    return true;
  }
  return false;
}

bool CommandList::EvaluateStandardFunction
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ if (theExpression.theOperation!=theCommands.opApplyFunction())
    return false;
  if (theExpression.children.size==0)
  { theExpression.errorString=
    "Programming error: function has no name; this should never happen. \
    Please dubug function CommandList::EvaluateStandardFunction";
    return true;
  }
//  if (theCommands.EvaluateDoLeftDistributeBracketIsOnTheLeft
//      (theCommands, inputIndexBoundVars, theExpression, theCommands.opFunction(), theCommands.opPlus()))
//    return true;
//  if (theCommands.EvaluateDoLeftDistributeBracketIsOnTheLeft
//      (theCommands, inputIndexBoundVars, theExpression, theCommands.opFunction(), theCommands.opTimes()))
//    return true;
  Expression& functionNameNode =theExpression.children[0];
  Expression& functionArgumentNode=theExpression.children[1];

  assert(theExpression.children.size==2);
  if (functionNameNode.theOperation==theCommands.opData())
  { Data tempData(theCommands);
    Data& theFunData=theCommands.theData[functionNameNode.theData];
    if (theFunData.type==Data::typeRational)
    { theExpression.AssignChild(0);
      return true;
    }
    if (functionArgumentNode.theOperation!=theCommands.opList())
    { if (functionArgumentNode.theOperation!=theCommands.opData() )
        return false;
      if (!theFunData.OperatorDereference
          (theCommands.theData[functionArgumentNode.theData], tempData, comments))
        return false;
      theExpression.MakeDatA(tempData, theCommands, inputIndexBoundVars);
      return true;
    }
    if (functionArgumentNode.children.size!=2)
      return false;
    Expression& leftE=functionArgumentNode.children[0];
    Expression& rightE=functionArgumentNode.children[1];
    if (leftE.theOperation!=theCommands.opData() || rightE.theOperation!=theCommands.opData())
      return false;
    Data& leftD=theCommands.theData[leftE.theData];
    Data& rightD=theCommands.theData[rightE.theData];
    if (!theFunData.OperatorDereference(leftD, rightD, tempData, comments))
      return false;
    theExpression.MakeDatA(tempData, theCommands, inputIndexBoundVars);
    return true;
  }
  if (functionNameNode.theOperation!=theExpression.theBoss->opVariableNonBound())
    return false;
  Function::FunctionAddress theFun;
  theFun= functionNameNode.GetFunctionFromVarName();
  if (theFun==0)
    return false;
  Expression result;
  result=theExpression.children[1];
  if(theFun(theCommands, inputIndexBoundVars, result, comments))
  { theExpression=result;
    return true;
  }
  return false;
}

bool CommandList::EvaluateStandardEqualEqual
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ assert(theExpression.children.size==2);
  Expression& left=theExpression.children[0];
  Expression& right=theExpression.children[1];
  if (left==right)
    theExpression.MakeInt(1, theCommands, inputIndexBoundVars);
  else
    theExpression.MakeInt(0, theCommands, inputIndexBoundVars);
  return true;
}

bool CommandList::EvaluateStandardLieBracket
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ if (theExpression.children.size!=2)
    return false;
  assert(theExpression.theOperation=theCommands.opLieBracket());
  Expression& leftE=theExpression.children[0];
  Expression& rightE=theExpression.children[1];
  if (leftE.theOperation!=theCommands.opData() || rightE.theOperation!=theCommands.opData())
    return false;
  Data& leftD=theCommands.theData[leftE.theData];
  Data& rightD=theCommands.theData[rightE.theData];
  Data newData(theCommands);
  if (!Data::LieBracket(leftD, rightD, newData, comments))
    return false;
  theExpression.MakeDatA(newData, theCommands, inputIndexBoundVars);
  return true;
}

bool Expression::IsRationalNumber()
{ if (this->theOperation!=this->theBoss->opData())
    return false;
  if (this->theBoss->theData[this->theData].type==Data::typeRational)
    return true;
  return false;
}

bool Expression::IsInteger()
{ if (this->theOperation!=this->theBoss->opData())
    return false;
  return this->theBoss->theData[this->theData].IsInteger();
}

bool Expression::IsSmallInteger(int& whichInteger)
{ if (this->theOperation!=this->theBoss->opData())
    return false;
  return this->theBoss->theData[this->theData].IsSmallInteger(whichInteger);
}

bool CommandList::EvaluateStandardUnion
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ if (theExpression.children.size==1)
  { theExpression.AssignChild(0);
    return true;
  }
  if (theExpression.children.size==0)
    return false;
  Expression resultExpression;
  resultExpression.reset(theCommands, inputIndexBoundVars);
  resultExpression.theOperation=theCommands.opList();
  for (int i=0; i<theExpression.children.size; i++)
    if (theExpression.children[i].theOperation!=theCommands.opList())
      return false;
  for (int i=0; i<theExpression.children.size; i++)
    resultExpression.children.AddListOnTop(theExpression.children[i].children);
  theExpression=resultExpression;
  theExpression.format=theExpression.formatDefault;
  return true;
}

bool CommandList::EvaluateStandardUnionNoRepetition
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ if (theExpression.children.size==1)
  { theExpression.AssignChild(0);
    return true;
  }
  if (theExpression.children.size==0)
    return false;
  for (int i=0; i<theExpression.children.size; i++)
    if (theExpression.children[i].theOperation!=theCommands.opList())
      return false;
  HashedList<Expression> tempList;
  for (int i=0; i<theExpression.children.size; i++)
    tempList.AddNoRepetition(theExpression.children[i].children);
  theExpression.theOperation=theCommands.opList();
  theExpression.theData=0;
  theExpression.children=tempList;
  theExpression.format=theExpression.formatDefault;
  return true;
}

bool CommandList::EvaluateStandardDivide
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ if (theExpression.children.size!=2)
    return false;
  Expression& leftE= theExpression.children[0];
  Expression& rightE= theExpression.children[1];
  if (rightE.IsRationalNumber() && !leftE.IsRationalNumber())
  { Data tempData(1, theCommands);
    tempData/=theCommands.theData[rightE.theData];
    rightE=leftE;
    leftE.MakeDatA(tempData, theCommands, inputIndexBoundVars);
    return true;
  }
  if (leftE.theOperation!= theCommands.opData() || rightE.theOperation!=theCommands.opData())
    return false;
  Data& leftData=theCommands.theData[leftE.theData];
  Data& rightData=theCommands.theData[rightE.theData];
  if (leftData.type!=leftData.typeRational || rightData.type!=rightData.typeRational)
    return false;
  Data resultData=leftData/rightData;
  theExpression.MakeDatA(resultData, theCommands, inputIndexBoundVars);
  return true;
}

bool CommandList::EvaluateStandardMinus
(CommandList& theCommands, int inputIndexBoundVars, Expression& theExpression, std::stringstream* comments)
{ if (theExpression.children.size!=1&& theExpression.children.size!=2)
  { theExpression.errorString="Programming error: operation - takes one or two arguments.";
    return false;
  }
  Expression* toBeTransformed=0;
  if (theExpression.children.size==1)
  { theExpression.AssignChild(0);
    toBeTransformed=&theExpression;
  }
  else
  { toBeTransformed=& theExpression.children[1];
    theExpression.theOperation=theCommands.opPlus();
  }
  Expression result, minusOne;
  minusOne.MakeInt(-1, theCommands, inputIndexBoundVars);
  result.MakeProducT(theCommands, inputIndexBoundVars, minusOne, *toBeTransformed);
  *toBeTransformed=result;
  //std::cout << toBeTransformed->ElementToString();
  return true;
}

bool Expression::operator==
(const Expression& other)const
{ if (this->theBoss==0)
    assert(false);
  return this->theBoss->ExpressionsAreEqual(*this, other);
}

bool CommandList::ExtractPolyRational
  (PolynomialRationalCoeff& output, const Expression& theInput, HashedList<Expression>& VariableImages,
   int RecursionDepth, int MaxRecursionDepthMustNotPopStack, std::stringstream* errorLog)
{ if (RecursionDepth>MaxRecursionDepthMustNotPopStack)
  { if (errorLog!=0)
      *errorLog << "Max recursion depth of " << MaxRecursionDepthMustNotPopStack
      << " exceeded while trying to evaluate polynomial expression (i.e. your polynomial expression is too large).";
    return false;
  }
  if (RecursionDepth==0)
  { VariableImages.Clear();
    output.Nullify(0);
  }
  if (theInput.theOperation==this->opTimes() || theInput.theOperation==this->opPlus() || theInput.theOperation==this->opMinus())
  { PolynomialRationalCoeff bufferPoly;
    for (int i=0; i<theInput.children.size; i++)
    { if (!this->ExtractPolyRational(bufferPoly, theInput.children[i], VariableImages, RecursionDepth+1, MaxRecursionDepthMustNotPopStack, errorLog))
        return false;
//      assert(bufferPoly.NumVars<VariableImages.size);
      bufferPoly.SetNumVariablesSubDeletedVarsByOne(VariableImages.size);
      output.SetNumVariablesSubDeletedVarsByOne(VariableImages.size);
      if (theInput.theOperation==this->opTimes())
      { if (i==0)
          output=bufferPoly;
        else
          output*=bufferPoly;
      }
      else if (theInput.theOperation==this->opMinus())
      { if (theInput.children.size==1)
          output.Subtract(bufferPoly);
        else if (i==0)
          output+=bufferPoly;
        else
          output.Subtract(bufferPoly);
      } else if (theInput.theOperation==this->opPlus())
      { if (i==0)
          output=bufferPoly;
        else
          output+=bufferPoly;
      }
    }
    return true;
  }
  if (theInput.theOperation==this->opThePower())
  { assert(theInput.children.size==2);
    Expression& right= theInput.children[1];
    if (right.theOperation==this->opData())
    { int  thePower=0;
      if (this->theData[right.theData].IsSmallInteger(thePower))
      { if(!this->ExtractPolyRational(output, theInput.children[0], VariableImages, RecursionDepth+1, MaxRecursionDepthMustNotPopStack, errorLog))
          return false;
        output.RaiseToPower(thePower, 1);
        return true;
      }
    }
  }
  if (theInput.theOperation==this->opData())
  { Data& theData=this->theData[theInput.theData];
    if (theData.type==theData.typeRational)
    { output.MakeNVarConst(0, theData.theRational.GetElement());
      return true;
    }
  }
  std::string debugString=theInput.ElementToString();
  int theIndex=VariableImages.AddNoRepetitionOrReturnIndexFirst(theInput);
  output.MakeMonomialOneLetter(VariableImages.size, theIndex, 1, 1);
  return true;
}

bool CommandList::ExpressionsAreEqual
  (const Expression& left, const Expression& right, int RecursionDepth, int MaxRecursionDepth)
{ if (RecursionDepth>MaxRecursionDepth)
  { std::stringstream out;
    out << "Error: maximium recursion depth of " << MaxRecursionDepth << " exceeded while comparing expressions: " ;
    out << left.ElementToString() << " and " << right.ElementToString();
    std::cout << out.str();
    this->evaluationErrors.AddOnTop(out.str());
    return false;
  }
  if (!left.AreEqualExcludingChildren(right))
    return false;
  for (int i=0; i<left.children.size; i++)
    if (!this->ExpressionsAreEqual (left.children[i], right.children[i], RecursionDepth+1, MaxRecursionDepth))
      return false;
  return true;
}

bool CommandList::ExpressionMatchesPattern
  (const Expression& thePattern, const Expression& input, ExpressionPairs& matchedExpressions,
   int RecursionDepth, int MaxRecursionDepth, std::stringstream* theLog)
{ ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  assert(thePattern.theBoss==this && input.theBoss==this);
  static int ExpressionMatchesPatternDebugCounter=-1;
  static bool printLocalDebugInfo=false;
  ExpressionMatchesPatternDebugCounter++;
//  std::cout << " ExpressionMatchesPatternDebugCounter: " << ExpressionMatchesPatternDebugCounter;
//  printLocalDebugInfo=(ExpressionMatchesPatternDebugCounter>-1);
  if (printLocalDebugInfo)
  { std::cout << " <hr> current input: " << input.ElementToString() << "<br>current pattern: " << thePattern.ElementToString();
    std::cout << "<br> current matched expressions: " << matchedExpressions.ElementToString();
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (RecursionDepth>MaxRecursionDepth)
  { std::stringstream out;
    out << "Max recursion depth of " << MaxRecursionDepth << " exceeded whlie trying to match expression pattern "
    << thePattern.ElementToString() << " onto expression " << input.ElementToString();
    this->evaluationErrors.AddOnTop(out.str());
    return false;
  }
  if (this->opDefine()==input.theOperation)
    return false;
  int opVarB=this->opVariableBound();
  if (thePattern.theOperation== opVarB)
  { int indexLeft= matchedExpressions.BoundVariableIndices.GetIndex(thePattern.theData);
    if (indexLeft==-1)
    { matchedExpressions.BoundVariableIndices.AddOnTop(thePattern.theData);
      matchedExpressions.variableImages.AddOnTop(input);
      indexLeft=matchedExpressions.BoundVariableIndices.size-1;
    }
    if (!(matchedExpressions.variableImages[indexLeft]==input))
      return false;
    if (printLocalDebugInfo)
      std::cout << "<br><b>Match!</b>";
    return true;
  }
  if (thePattern.theOperation!=input.theOperation || thePattern.theData!=input.theData || thePattern.children.size!=input.children.size )
    return false;
  for (int i=0; i<thePattern.children.size; i++)
    if (!(this->ExpressionMatchesPattern
    (thePattern.children[i], input.children[i], matchedExpressions, RecursionDepth+1, MaxRecursionDepth, theLog)))
      return false;
  if (printLocalDebugInfo)
    std::cout << "<br><b>Match!</b>";
  return true;
}

bool CommandList::EvaluateExpressionReturnFalseIfExpressionIsBound
(Expression& theExpression, int RecursionDepth, int maxRecursionDepth,
 ExpressionPairs& bufferPairs, std::stringstream* theLog)
{ if (RecursionDepth>=maxRecursionDepth)
  { std::stringstream out;
    out << "Recursion depth limit of " << this->MaxRecursionDepthDefault << " exceeded while evaluating expressions.";
    theExpression.errorString=out.str();
    if (this->flagMaxRecursionErrorEncountered)
      this->evaluationErrors.AddOnTop(out.str());
    this->flagMaxRecursionErrorEncountered=true;
    return true;
  }
  if (theExpression.errorString!="")
    return true;
  std::stringstream comments;
  if (theExpression.theOperation<0 || theExpression.theBoss!=this)
    return true;
  if (this->ExpressionStack.Contains(theExpression))
  { std::stringstream errorStream;
    errorStream << "I think I have detected a cycle: " << theExpression.ElementToString()
    << " is transformed to an expression that contains the starting expression. ";
    theExpression.SetError(errorStream.str());
    return true;
  }
  static int errorCounter=0;
  errorCounter++;
  std::string debugString="non-initialized";
  std::string debugStringIntermediate="non-initialized";

  debugString=theExpression.ElementToString();
  //std::cout << "<hr> At error counter " << errorCounter << " expression is: " << debugString;
  this->ExpressionStack.AddOnTop(theExpression);
  HashedList<Expression> currentExpressionTransformations;
//  currentExpressionTransformations.AddOnTop(theExpression);

  bool NonReduced=true;
  int counter=-1;
  int indexInCache=-1;
  if (theExpression.IndexBoundVars==3)
  { debugString=theExpression.ElementToString();
  }
  if (theExpression.IndexBoundVars!=-1)
  { indexInCache=this->theExpressionContext[theExpression.IndexBoundVars].cachedExpressions.GetIndex(theExpression);
    if (indexInCache!=-1)
    { theExpression=
      this->theExpressionContext[theExpression.IndexBoundVars].imagesCachedExpressions[indexInCache];
    } else
    { this->theExpressionContext[theExpression.IndexBoundVars].cachedExpressions.AddOnTop(theExpression);
      this->theExpressionContext[theExpression.IndexBoundVars].imagesCachedExpressions.AddOnTop(theExpression);
      indexInCache=this->theExpressionContext[theExpression.IndexBoundVars].cachedExpressions.size-1;
    }
  }
  //reduction phase:
  bool resultExpressionIsFree=true;
  while (NonReduced)
  { if (this->theGlobalVariableS->GetElapsedSeconds()!=0)
      if (this->theGlobalVariableS->GetElapsedSeconds()-this->StartTimeInSeconds >this->MaxAllowedTimeInSeconds)
      { if (!this->flagTimeLimitErrorDetected)
          std::cout << "<br><b>Max allowed computational time is " << this->MaxAllowedTimeInSeconds << ";  so far, "
          << this->theGlobalVariableS->GetElapsedSeconds()-this->StartTimeInSeconds  << " have elapsed -> aborting computation ungracefully.</b>";
        this->flagTimeLimitErrorDetected=true;
        this->ExpressionStack.PopIndexSwapWithLast(this->ExpressionStack.size-1);
        return true;
      }
    counter++;
    NonReduced=false;
    for (int i=0; i<theExpression.children.size; i++)
    { if(!this->EvaluateExpressionReturnFalseIfExpressionIsBound
          (theExpression.children[i], RecursionDepth+1, maxRecursionDepth, bufferPairs, theLog))
        resultExpressionIsFree=false;
      if (theExpression.children[i].errorString!="")
      { this->ExpressionStack.PopIndexSwapWithLast(this->ExpressionStack.size-1);
        std::stringstream errorStream;
        errorStream << "Error: child " << i+1 << " contains an error.";
        theExpression.SetError(errorStream.str());
        return true;
      }
    }
    if (counter>this->MaxAlgTransformationsPerExpression)
    { if (!this->flagMaxTransformationsErrorEncountered)
      { std::stringstream out;
        out << "<br>Maximum number of algebraic transformations of " << this->MaxAlgTransformationsPerExpression << " exceeded."
        << " while simplifying " << theExpression.ElementToString();
        theExpression.errorString=out.str();
      }
      this->ExpressionStack.PopIndexSwapWithLast(this->ExpressionStack.size-1);
      return true;
    }
    Function::FunctionAddress theFun=this->operations[theExpression.theOperation].GetFunction(*this);
    if (theFun!=0)
      if (theFun(*this, theExpression.IndexBoundVars, theExpression, &comments))
        NonReduced=true;
//    std::cout << "<br>after standard eval: " << theExpression.ElementToString();
//    for (unsigned i=0; i<this->targetProperties.size(); i++)
//      if (theExpression.GetPropertyValue(this->targetProperties[i])!=1)
//        if (this->theNonBoundVars[this->targetProperties[i]].theHandler!=0)
//          if (this->theNonBoundVars[this->targetProperties[i]].theHandler(*this, inputIndexBoundVars, theExpression))
//            NonReduced=true;
    for (int i=0; i<theExpression.IndexBoundVars; i++)
      if (this->theCommands.children[i].errorString=="")
      { Expression& currentPattern=this->theCommands.children[i];
        if (currentPattern.errorString=="")
          if (currentPattern.theOperation==this->opDefine() || currentPattern.theOperation==this->opDefineConditional())
          { this->TotalNumPatternMatchedPerformed++;
            bufferPairs.reset();
            if(this->ProcessOneExpressionOnePatternOneSub
            (theExpression.IndexBoundVars, currentPattern, theExpression, bufferPairs,
             RecursionDepth+1, maxRecursionDepth, theLog))
            { NonReduced=true;
              break;
            }
          }
      }
    if (theExpression.IndexBoundVars!=-1)
    { this->theExpressionContext[theExpression.IndexBoundVars].imagesCachedExpressions[indexInCache]=theExpression;
      if (NonReduced)
      { if (currentExpressionTransformations.Contains(theExpression))
        { std::stringstream errorStream;
          errorStream << "I think I detected a substitution cycle, " << theExpression.ElementToString()
          << " appears twice in the reduction cycle";
          theExpression.SetError(errorStream.str());
          break;
        } else
          currentExpressionTransformations.AddOnTop(theExpression);
      }
    }
  }
  this->ExpressionStack.PopIndexSwapWithLast(this->ExpressionStack.size-1);

  theExpression.theComments=comments.str();
  if (theExpression.theOperation==this->opVariableBound())
    return false;
  return resultExpressionIsFree;
}

Expression* CommandList::PatternMatch
  (int inputIndexBoundVars, Expression& thePattern, Expression& theExpression, ExpressionPairs& bufferPairs,
   int RecursionDepth, int MaxRecursionDepth, Expression* condition, std::stringstream* theLog, bool logAttempts)
{ if (RecursionDepth>=MaxRecursionDepthDefault)
  { std::stringstream out;
    out << "Error: while trying to evaluate expression, the maximum recursion depth of " << MaxRecursionDepth
    << " was exceeded";
    theExpression.errorString=out.str();
    return 0;
  }
  if (!this->ExpressionMatchesPattern(thePattern, theExpression, bufferPairs, RecursionDepth+1, MaxRecursionDepth, theLog))
    return 0;
  if (theLog!=0 && logAttempts)
    (*theLog) << "<hr>found pattern: " << theExpression.ElementToString() << " -> " << thePattern.ElementToString();
  if (condition==0)
    return &theExpression;
  Expression tempExp=*condition;
  if (theLog!=0 && logAttempts)
    (*theLog) << "<hr>Specializing condition pattern: " << tempExp.ElementToString();
  this->SpecializeBoundVars(tempExp, inputIndexBoundVars, bufferPairs, RecursionDepth+1, MaxRecursionDepth);
  if (theLog!=0 && logAttempts)
    (*theLog) << "<hr>Specialized condition: " << tempExp.ElementToString() << "; evaluating...";
  ExpressionPairs tempPairs;
  this->EvaluateExpressionReturnFalseIfExpressionIsBound
  (tempExp, RecursionDepth+1, MaxRecursionDepth, tempPairs, theLog);
  if (theLog!=0 && logAttempts)
    (*theLog) << "<hr>The evaluated specialized condition: " << tempExp.ElementToString() << "; evaluating...";
  if (tempExp.theOperation==this->opData())
    if (this->theData[tempExp.theData].IsEqualToOne())
      return & theExpression;
  return 0;
}

void CommandList::SpecializeBoundVars
(Expression& toBeSubbed, int targetinputIndexBoundVars, ExpressionPairs& matchedPairs, int RecursionDepth, int MaxRecursionDepth)
{ toBeSubbed.IndexBoundVars=targetinputIndexBoundVars;
  if (toBeSubbed.theOperation==this->opVariableBound())
  { int indexMatching= matchedPairs.BoundVariableIndices.GetIndex(toBeSubbed.theData);
    toBeSubbed=matchedPairs.variableImages[indexMatching];
//    this->ExpressionHasBoundVars(toBeSubbed, RecursionDepth+1, MaxRecursionDepth);
    return;
  }
  for (int i=0; i<toBeSubbed.children.size; i++)
    this->SpecializeBoundVars(toBeSubbed.children[i], targetinputIndexBoundVars, matchedPairs, RecursionDepth+1, MaxRecursionDepth);
//  this->ExpressionHasBoundVars(toBeSubbed, RecursionDepth+1, MaxRecursionDepth);
}

bool CommandList::ProcessOneExpressionOnePatternOneSub
  (int inputIndexBoundVars, Expression& thePattern, Expression& theExpression, ExpressionPairs& bufferPairs,
   int RecursionDepth, int maxRecursionDepth, std::stringstream* theLog, bool logAttempts)
{ assert(thePattern.theOperation==this->opDefine() ||
  thePattern.theOperation==this->opDefineConditional());
  assert(thePattern.children.size==2 || thePattern.children.size==3);
  if (theLog!=0 && logAttempts)
  { (*theLog) << "<hr>attempting to reduce expression " << theExpression.ElementToString();
    (*theLog) << " by pattern " << thePattern.ElementToString();
  }
  Expression& currentPattern=thePattern.children[0];
  Expression* theCondition=0;
  bool isConditionalDefine=thePattern.theOperation==this->opDefineConditional();
  if (isConditionalDefine)
    theCondition=&thePattern.children[1];
  Expression* toBeSubbed=0;
  if ((thePattern.theOperation==this->opDefine()) || isConditionalDefine)
    toBeSubbed=this->PatternMatch
    (inputIndexBoundVars, currentPattern, theExpression, bufferPairs, RecursionDepth+1, maxRecursionDepth, theCondition, theLog);
  if (toBeSubbed==0)
    return false;
  if (theLog!=0 && logAttempts)
    *theLog << "<br><b>found a match!</b>";
  if (isConditionalDefine)
    *toBeSubbed=thePattern.children[2];
  else
    *toBeSubbed=thePattern.children[1];
  toBeSubbed->IndexBoundVars= inputIndexBoundVars;
  this->SpecializeBoundVars(*toBeSubbed, inputIndexBoundVars, bufferPairs, RecursionDepth+1, maxRecursionDepth);
  return true;
}

void CommandList::ExtractExpressions()
{ std::string lookAheadToken;
  std::stringstream errorLog;
  this->theExpressionContext.SetSize(0);
  this->syntacticStack.Reserve(this->syntacticSoup.size+this->numEmptyTokensStart);
  this->syntacticStack.SetSize(this->numEmptyTokensStart);
  for (int i=0; i<this->numEmptyTokensStart; i++)
    this->syntacticStack[i]=this->GetEmptySyntacticElement();
  this->CreateNewExpressionContext();
  for (int i=0; i<this->syntacticSoup.size; i++)
  { if (i+1<this->syntacticSoup.size)
      lookAheadToken=this->controlSequences[this->syntacticSoup[i+1].controlIndex];
    else
      lookAheadToken=";";
    this->syntacticStack.AddOnTop(this->syntacticSoup[i]);
    if (this->syntacticStack.LastObject()->controlIndex==this->conEndStatement())
      this->CreateNewExpressionContext();
    Expression& currentExpression=this->syntacticStack.LastObject()->theData;
    currentExpression.IndexBoundVars=this->theExpressionContext.size-1;
    while(this->ApplyOneRule(lookAheadToken))
    {}
  }
  if (this->syntacticStack.size==this->numEmptyTokensStart+1)
  { SyntacticElement& result=this->syntacticStack[this->numEmptyTokensStart];
    if (result.ErrorString=="" && result.controlIndex==this->conExpression())
      this->theCommands=result.theData;
    else if (result.ErrorString!="")
      errorLog << "Syntax error with message: " << result.ErrorString;
    else
    { errorLog << "Syntax error: your command simplifies to a single syntactic element but it is not an expression. <br>";
      errorLog << "It simplifies to:<br> " << this->ElementToStringSyntacticStack();
    }
  } else if (this->syntacticStack.size!=this->numEmptyTokensStart)
  { errorLog << "Syntax error: your command does not simplify to a syntactic element. <br>";
    errorLog << "Instead it simplifies to:<br> " << this->ElementToStringSyntacticStack();
  }
  this->syntaxErrors = errorLog.str();
}


void CommandList::EvaluateCommands()
{ std::stringstream out;
  ExpressionPairs thePairs;

//  this->theLogs.resize(this->theCommands.size());
//this->ElementToString();
  std::stringstream loggingStream;
  if (this->syntaxErrors!="")
  { out << "<hr><b>Syntax errors encountered</b><br>";
    out << this->syntaxErrors;
    out << "<hr>";
  }
  std::string startingExpressionString=this->theCommands.ElementToString(0,10000);
  this->EvaluateExpressionReturnFalseIfExpressionIsBound
  (this->theCommands, 0, this->MaxRecursionDepthDefault, thePairs, &loggingStream);
  std::stringstream comments;
  out << "<table border=\"1\" ><tr><th>Your input</th><th>Result</th><th>Result in LaTeX</th></tr><tr><td>"<< startingExpressionString << "</td><td>"  << this->theCommands.ElementToString(0, 10000)
  << "</td><td>" << this->theCommands.ElementToString(0, 10000, true, false, false, &comments) << "</td></tr>";
  if (comments.str()!="")
    out << "<tr><td colspan=\"3\"> "<< comments.str() << "</td></tr>";
  out << "</table>";
  this->theLog= loggingStream.str();
  this->output=out.str();
}

std::string SyntacticElement::ElementToString(CommandList& theBoss)
{ std::stringstream out;
  bool makeTable=this->controlIndex==theBoss.conExpression() || this->controlIndex==theBoss.conError()
  || this->controlIndex==theBoss.conList() ;
  if (makeTable)
    out << "<table border=\"1\"><tr><td>";
  if (this->controlIndex<0)
    out << "Error: control index is not initialized! This is likely a programming error.";
  else
    out << theBoss.controlSequences[this->controlIndex];
  if (makeTable)
  { out << "</td></tr><tr><td>";
    out << this->theData.ElementToString(0, 10);
    if (this->ErrorString!="")
      out << "</td></tr><tr><td>" << this->ErrorString;
    out << "</td></tr></table>";
  }
  return out.str();
}

Function::FunctionAddress VariableNonBound::GetFunction(CommandList& owner)
{ if (this->HandlerFunctionIndex==-1)
    return 0;
  assert(this->HandlerFunctionIndex>=0 && this->HandlerFunctionIndex <owner.theFunctions.size);
  return owner.theFunctions[this->HandlerFunctionIndex].theFunction;
}

Function::FunctionAddress Expression::GetFunctionFromVarName()
{ assert(this->theBoss!=0);
  assert(this->theOperation==this->theBoss->opVariableNonBound());
  return this->theBoss->theNonBoundVars[this->theData].GetFunction(*this->theBoss);
}

int Expression::GetNumCols()const
{ if (this->theOperation!=this->theBoss->opList() || this->format!=this->formatMatrix)
    return -1;
  int theMax=1;
  for (int i=0; i<this->children.size; i++)
    if (this->children[i].format==this->formatMatrixRow && this->children[i].theOperation==this->theBoss->opList())
      theMax=MathRoutines::Maximum(this->children[i].children.size, theMax);
  return theMax;
}

std::string Expression::ElementToString
(int recursionDepth, int maxRecursionDepth, bool useLatex, bool AddBrackets, bool AddCurlyBraces,
 std::stringstream* outComments)const
{ if (maxRecursionDepth>0)
    if(recursionDepth>maxRecursionDepth)
      return "(...)";
  if (this->theBoss==0)
    return "(ProgrammingErrorNoBoss)";
  assert((int)(this->theBoss)!=-1);
  std::stringstream out;
//  if (this->theBoss->flagLogSyntaxRules && recursionDepth<=1)
//  { out << "(ContextIndex=" << this->IndexBoundVars << ")";
//  }
  std::string additionalDataComments;
  if (this->errorString!="")
  { if (outComments!=0)
      *outComments << this->errorString << " ";
    return "(Error:SeeComments)";
  }
  if (this->theOperation<0)
  { out << "(ErrorNoOp)";
    return out.str();
  }
  if (AddBrackets)
    out << "(";
  if (AddCurlyBraces)
    out << "{";
  if (this->theOperation==this->theBoss->opDefine())
    out << this->children[0].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments)
    << ":=" << this->children[1].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments);
  else if (this->theOperation==this->theBoss->opDefineConditional())
    out <<  this->children[0].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments) << " :if "
    << this->children[1].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, true, false, outComments)
    << ":=" << this->children[2].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments);
  else if (this->theOperation==this->theBoss->opDivide() )
    out << this->children[0].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, this->children[0].NeedBracketsForMultiplication(), false, outComments)
    << "/" << this->children[1].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, this->children[1].NeedBracketsForMultiplication(), false, outComments);
  else if (this->theOperation==this->theBoss->opTimes() )
  { std::string tempS=this->children[0].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, this->children[0].NeedBracketsForMultiplication(), false, outComments);
    //if (false)
   // {
    if (tempS=="-1")
      tempS="-";
    if (tempS=="1")
      tempS="";
    //} else
      //tempS="("+tempS+")";
    out << tempS;
    if (this->format==this->formatTimesDenotedByStar && tempS!="-" && tempS!="")
      out << "*"; else out << " ";
    out << this->children[1].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, this->children[1].NeedBracketsForMultiplication(), false, outComments);
  }
  else if (this->theOperation==this->theBoss->opThePower())
    out << this->children[0].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, this->children[0].NeedBracketsForThePower(), false, outComments)
    << "^{" << this->children[1].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments) << "}";
  else if (this->theOperation==this->theBoss->opPlus() )
  { assert(this->children.size>=2);
    out << this->children[0].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, this->children[0].NeedBracketsForAddition(), false, outComments);
    std::string tempS=this->children[1].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, this->children[1].NeedBracketsForAddition(), false, outComments);
    if (tempS.size()>0)
      if (tempS[0]!='-')
        out << "+";
    out << tempS;
  }
  else if (this->theOperation==this->theBoss->opMinus())
  { if ( this->children.size==1)
      out << "-" << this->children[0].ElementToString
      (recursionDepth+1, maxRecursionDepth, useLatex, this->children[0].NeedBracketsForMultiplication(), false, outComments);
    else
    { assert(children.size==2);
      out << this->children[0].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, this->children[0].NeedBracketsForAddition(), false, outComments)
      << "-" << this->children[1].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, this->children[1].NeedBracketsForAddition(), false, outComments);
    }
  }
  else if (this->theOperation==this->theBoss->opVariableBound())
  { if (this->IndexBoundVars<0 || this->IndexBoundVars>= this->theBoss->theExpressionContext.size)
      out << "(BadContext=" << this->IndexBoundVars << ")";
    assert(this->theBoss->theExpressionContext[this->IndexBoundVars].BoundVariables.size>0);
    assert(this->theBoss->theExpressionContext[this->IndexBoundVars].BoundVariables.size>this->theData);
    out << "{{" << this->theBoss->theExpressionContext[this->IndexBoundVars].BoundVariables[this->theData] << "}}";
  }
  else if (this->theOperation==this->theBoss->opVariableNonBound())
    out << this->theBoss->theNonBoundVars[this->theData].theName;
  else if (this->theOperation==this->theBoss->opData())
  { std::stringstream dataComments;
    out << this->theBoss->theData[this->theData].ElementToString(&dataComments);
    additionalDataComments=dataComments.str();
  }
  else if (this->theOperation==this->theBoss->opApplyFunction())
  { assert(this->children.size>=2);
    switch(this->format)
    { case Expression::formatFunctionUseUnderscore:
        out <<  this->children[0].ElementToString
        (recursionDepth+1, maxRecursionDepth, useLatex, false, this->children[0].NeedBracketsForFunctionName(), outComments)
        << "_" << this->children[1].ElementToString
        (recursionDepth+1, maxRecursionDepth, useLatex, false, this->children[1].NeedBracketsForFunctionName(), outComments) << "";
        break;
      case Expression::formatFunctionUseCdot:
        out <<  this->children[0].ElementToString
        (recursionDepth+1, maxRecursionDepth, useLatex, this->children[0].NeedBracketsForFunctionName(), false, outComments)
        << "\\cdot(" << this->children[1].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments) << ")";
        break;
      default:
        out << this->children[0].ElementToString
        (recursionDepth+1, maxRecursionDepth, useLatex, this->children[0].NeedBracketsForFunctionName(), false, outComments)
        << "{{}" << this->children[1].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, this->children[1].NeedBracketsForFunctionArgument(), false, outComments) << "}";
        break;
    }
  }
  else if (this->theOperation==this->theBoss->opEqualEqual())
    out << this->children[0].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments)
    << "==" << this->children[1].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments);
  else if (this->theOperation==this->theBoss->opList())
  { switch (this->format)
    { case Expression::formatMatrixRow:
        for (int i=0; i<this->children.size; i++)
        { out << this->children[i].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments);
          if (i!=this->children.size-1)
            out << "& ";
        }
        break;
      case Expression::formatMatrix:
        if (useLatex)
          out << "\\left(";
        out << "\\begin{array}{";
        for (int i=0; i<this->GetNumCols(); i++)
          out << "c";
        out << "} ";
        for (int i=0; i<this->children.size; i++)
        { out << this->children[i].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments);
          if (i!=this->children.size-1)
            out << "\\\\ \n";
        }
        out << " \\end{array}";
        if (useLatex)
          out << "\\right)";
        break;
      default:
        out << "(";
        for (int i=0; i<this->children.size; i++)
        { out << this->children[i].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments);
          if (i!=this->children.size-1)
            out << ", ";
        }
        out << ")";
        break;
    }
  } else if (this->theOperation==this->theBoss->opLieBracket())
    out << "[" << this->children[0].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments)
    << "," << this->children[1].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments)
    << "]";
  else if (this->theOperation==this->theBoss->opUnion())
    out << this->children[0].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments)
    << "\\cup " << this->children[1].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments)
    ;
  else if (this->theOperation==this->theBoss->opUnionNoRepetition())
    out << this->children[0].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments)
    << "\\sqcup " << this->children[1].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments)
    ;
  else if (this->theOperation==this->theBoss->opEndStatement())
  { out << "<table>";
    for (int i=0; i<this->children.size; i++)
    { out << "<tr><td>";
      if (useLatex && recursionDepth==0)
        out << CGI::GetHtmlMathSpanFromLatexFormula
        (this->children[i].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments)
         +((i!=this->children.size-1)?";": ""));
      else
      { out << this->children[i].ElementToString(recursionDepth+1, maxRecursionDepth, useLatex, false, false, outComments);
        if (i!=this->children.size-1)
          out << ";";
      }
//      out << "&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp Context index: " << this->IndexBoundVars;
      out << "</td></tr>";
    }
    out << "</table>";
  }
  else
    out << "(ProgrammingError:NotDocumented)" ;
  if (AddBrackets)
    out << ")";
  if (AddCurlyBraces)
    out << "}";
  if (this->errorString!="")
    out << ")";
  if (outComments!=0)
  { if (this->theComments!="" || additionalDataComments!="")
      *outComments << "Comments to expression " << out.str() << ":<br>";
    if (this->theComments!="")
      *outComments << this->theComments << "<br>";
    if (additionalDataComments!="")
      *outComments << additionalDataComments;
  }
  if (useLatex && recursionDepth==0 && this->theOperation!=theBoss->opEndStatement())
    return CGI::GetHtmlMathSpanFromLatexFormula(out.str());
  return out.str();
}

std::string Expression::ElementToStringPolishForm(int recursionDepth, int maxRecursionDepth)
{ if (maxRecursionDepth>0)
    if(recursionDepth>maxRecursionDepth)
      return "...and so on (maximum depth reached)...";
  if (this->theBoss==0)
    return " non-initialized ";
  std::stringstream out;
  if (this->theOperation<0)
  { out << " operation not initialized ";
    return out.str();
  }
  if (this->children.size>0)
    out << "<table border=\"1\"> <tr><td  align=\"center\" colspan=\"" << this->children.size << "\">";
  out << this->theBoss->operations[this->theOperation].theName << " (operation index " << this->theOperation << ")";
  out << ", " << this->theData;
  if (this->children.size>0)
  { out << "</td></tr><td>";
    for (int i=0; i<this->children.size; i++)
    { out << this->children[i].ElementToStringPolishForm(recursionDepth+1, maxRecursionDepth);
      if (i!=this->children.size-1)
        out << "</td><td> ";
    }
    out << "</td></tr>";
  }
  return out.str();
}

std::string VariableNonBound::GetHandlerFunctionName(CommandList& owner)
{ if (this->HandlerFunctionIndex==-1)
    return "";
  return owner.theFunctions[this->HandlerFunctionIndex].theName;
}

void CommandList::AddOperationNoFail
  (const std::string& theOpName,
   const Function::FunctionAddress& theFunAddress,
   const std::string& opArgumentList, const std::string& opDescription,
   const std::string& opExample, bool inputNameNotUsed
   )
{ VariableNonBound theVar;
  if (theFunAddress!=0)
  { Function currentFunction(theFunAddress, theOpName, opArgumentList, opDescription, opExample, inputNameNotUsed);
    theVar.HandlerFunctionIndex=this->theFunctions.AddNoRepetitionOrReturnIndexFirst(currentFunction);
  } else
    theVar.HandlerFunctionIndex=-1;
  theVar.theName=theOpName;
  if (this->operations.Contains(theOpName))
  { assert(false);
    return;
  }
  this->operations.AddOnTop(theVar);
}

int CommandList:: AddNonBoundVarReturnVarIndex
(const std::string& theName, const Function::FunctionAddress& funHandler,
  const std::string& argList, const std::string& description, const std::string& exampleArgs
)
{ VariableNonBound theVar;
  theVar.theName=theName;
  theVar.HandlerFunctionIndex=-1;
  int theIndex=this->theNonBoundVars.GetIndex(theVar);
  if (theIndex!=-1)
    return theIndex;
  if (funHandler!=0)
  { Function theFun(funHandler, theName, argList, description, exampleArgs, true);
    theVar.HandlerFunctionIndex=this->theFunctions.AddNoRepetitionOrReturnIndexFirst(theFun);
  }
  this->theNonBoundVars.AddOnTop(theVar);
  this->theDictionary.AddNoRepetition(theName);
  return this->theNonBoundVars.size-1;
}

std::string CommandList::ElementToStringNonBoundVars()
{ std::stringstream out;
  std::string openTag1="<span style=\"color:#0000FF\">";
  std::string closeTag1="</span>";
  std::string openTag2="<span style=\"color:#FF0000\">";
  std::string closeTag2="</span>";
  out << "<br>\n" << this->theNonBoundVars.size << " global variables " << " (= "
  << this->NumPredefinedVars  << " predefined + " << this->theNonBoundVars.size-this->NumPredefinedVars
  << " user-defined). <br>Predefined: \n<br>\n";
  for (int i=0; i<this->theNonBoundVars.size; i++)
  { out << openTag1 << this->theNonBoundVars[i].theName << closeTag1;
    std::string handlerName=this->theNonBoundVars[i].GetHandlerFunctionName(*this);
    if (handlerName!="")
      out << " [handled by: " << openTag2 << handlerName << closeTag2 << "]";
    if (i!=this->theNonBoundVars.size-1)
    { out << ", ";
      if (i==this->NumPredefinedVars-1 )
        out << "<br>user-defined:\n<br>\n";
    }
  }
  return out.str();
}

std::string Function::ElementToString(CommandList& theBoss)const
{ std::stringstream out;
  out << "<span style=\"display: inline\" id=\"functionBox" << CGI::clearSlashes(this->theName) << "\" >";
  if (this->flagNameIsUsed)
    out << this->theName << "{}(" << this->theArgumentList << ")";
  else
    out << this->theName;
  out <<  "<button" << CGI::GetStyleButtonLikeHtml() << " onclick=\"switchMenu('fun" << CGI::clearSlashes(this->theName)
  << "');\">More/less info</button><span id=\"fun" << CGI::clearSlashes(this->theName)
  << "\" style=\"display: none\"><br>";
  if (!this->flagNameIsUsed)
    out << "This function is invoked indirectly as an operation handler. ";

  out << this->theDescription;
  if (this->theExample!="")
  out << "<br>Example. <a href=\""
    << theBoss.DisplayNameCalculator  << "?"
    << " textType=Calculator&textDim=1&textInput="
    << CGI::UnCivilizeStringCGI(this->theExample)
    << "\"> " << this->theExample << "</a>" ;
    out << "</span></span>";
  return out.str();
}

std::string CommandList::ElementToStringFunctionHandlers()
{ std::stringstream out;
  out << "\n <b>Handler functions (" << this->theFunctions.size << " total).</b><br>\n";
  for (int i=0; i<this->theFunctions.size; i++)
  { out << "\n" << this->theFunctions[i].ElementToString(*this);
    if (i!=this->theFunctions.size-1)
      out << "<br>";
  }
  return out.str();
}

std::string CommandList::ElementToString()
{ std::stringstream out;
  std::string openTag1="<span style=\"color:#0000FF\">";
  std::string closeTag1="</span>";
  std::string openTag2="<span style=\"color:#FF0000\">";
  std::string closeTag2="</span>";
  std::string openTag3="<span style=\"color:#00FF00\">";
  std::string closeTag3="</span>";
  out << " Total number of pattern matches performed: " << this->TotalNumPatternMatchedPerformed << "";
  double elapsedSecs=this->theGlobalVariableS->GetElapsedSeconds() - this->StartTimeInSeconds;
  out << "<br>Elapsed time since evaluation was started: "
  << elapsedSecs << " seconds (" << elapsedSecs*1000 << " milliseconds).";
  out << "<hr>" << this->ElementToStringFunctionHandlers() << "<hr>";
  out << "<br>Control sequences (" << this->controlSequences.size << " total):\n<br>\n";
  for (int i=0; i<this->controlSequences.size; i++)
  { out << openTag1 << this->controlSequences[i] << closeTag1;
    if (i!=this->controlSequences.size)
      out << ", ";
  }
  out << "<br>\nOperators (" << this->operations.size << " total):<br>\n";
  for (int i=0; i<this->operations.size; i++)
  { out << "\n" << openTag1 << this->operations[i].theName << closeTag1;
    std::string handlerName=this->operations[i].GetHandlerFunctionName(*this);
    if (this->operations[i].GetFunction(*this)!= 0)
      out << " [handled by: " << openTag2 << handlerName << closeTag2 << "]";
    if(i!=this->operations.size-1)
      out << ", ";
  }
  out << "<br>\n Dictionary (" << this->theDictionary.size << " total):\n<br>\n";
  for (int i=0; i<this->theDictionary.size; i++)
  { out << openTag1 << this->theDictionary[i] << closeTag1;
    if (i!=this->theDictionary.size-1)
      out << ", ";
  }
  out << this->ElementToStringNonBoundVars();
  out << "<br>\nData entries (" << this->theData.size << " total):\n<br>\n";
  for (int i=0; i<this->theData.size; i++)
  { out << openTag3 << this->theData[i].ElementToString() << closeTag3;
    if (i!=this->theData.size-1)
      out  << ", ";
  }
  out << "<hr>";
  for (int k=0; k<this->theExpressionContext.size; k++)
  { ExpressionContext& currentContext=this->theExpressionContext[k];
    out <<"<hr>" << "Context "<< k+1;
    out << "<br>\n Cached expressions (" << currentContext.cachedExpressions.size << " total):\n<br>\n";
    for (int i=0; i<currentContext.cachedExpressions.size; i++)
      out << currentContext.cachedExpressions[i].ElementToString() << " -> "
      << currentContext.imagesCachedExpressions[i].ElementToString() << "<br>";
    if (currentContext.BoundVariables.size>0)
    { out << "Bound variables:<br>\n ";
      for (int i=0; i<currentContext.BoundVariables.size; i++)
      { out << currentContext.BoundVariables[i];
        if (i!=currentContext.BoundVariables.size-1)
          out << ", ";
      }
    }
  }
  return out.str();
}

Rational Expression::GetConstantTerm()const
{ if (this->theOperation==this->theBoss->opData())
  { Data& curData=this->theBoss->theData[this->theData];
    if (curData.type==curData.typeRational)
      return curData.theRational.GetElement();
  }
  if (this->theOperation==this->theBoss->opTimes())
    if (this->children[0].theOperation==this->theBoss->opData())
    { Data& curData=this->theBoss->theData[this->children[0].theData];
      if (curData.type==curData.typeRational)
        return curData.theRational.GetElement();
    }
  return 1;
}

bool Expression::NeedBracketsForFunctionName() const
{ return !(
  this->theOperation==this->theBoss->opVariableBound() ||
  this->theOperation==this->theBoss->opVariableNonBound()
  || ( this->theOperation==this->theBoss->opApplyFunction() && this->format==this->formatFunctionUseUnderscore)
  );
}

bool Expression::NeedBracketsForMultiplication()const
{ //return this->children.size()>1;
  return
  this->theOperation==this->theBoss->opPlus() ||
  this->theOperation==this->theBoss->opMinus()
  ;
}

bool Expression::NeedBracketsForAddition()const
{ return
  false
  ;
}

bool Expression::NeedBracketsForFunctionArgument()const
{ return this->format!=formatNoBracketsForFunctionArgument;
}

bool Expression::NeedBracketsForThePower()const
{ return
  this->theOperation==this->theBoss->opPlus() ||
  this->theOperation==this->theBoss->opMinus() ||
  this->theOperation==this->theBoss->opTimes() ||
  this->theOperation==this->theBoss->opDivide()
  ;
}

void Expression::MakeInt(int theValue, CommandList& newBoss, int inputIndexBoundVars)
{ this->reset(newBoss, inputIndexBoundVars);
  this->theData=newBoss.theData.AddNoRepetitionOrReturnIndexFirst(Data(theValue, newBoss));
  this->theOperation=newBoss.opData();
}

void Expression::MakeDatA(const Rational& inputData, CommandList& newBoss, int inputIndexBoundVars)
{ this->MakeDatA(Data(inputData, newBoss), newBoss, inputIndexBoundVars);
}

void Expression::MakeDatA(const Data& inputData, CommandList& newBoss, int inputIndexBoundVars)
{ this->reset(newBoss, inputIndexBoundVars);
  this->theData=newBoss.theData.AddNoRepetitionOrReturnIndexFirst(inputData);
  assert(this->theData<newBoss.theData.size);
  this->theOperation=newBoss.opData();
}

void Expression::MakeVariableNonBoundNoProperties
  (CommandList& owner, int inputIndexBoundVars, int varIndex)
{ this->reset(owner, inputIndexBoundVars);
  this->theData=varIndex;
  this->theOperation=owner.opVariableNonBound();
}

void Expression::MakeFunction
  (CommandList& owner, int inputIndexBoundVars, const Expression& argument, int functionIndex)
{ this->reset(owner, inputIndexBoundVars);
  this->theOperation=owner.opApplyFunction();
  this->children.SetSize(2);
  this->children[0].MakeVariableNonBoundNoProperties(owner, inputIndexBoundVars, functionIndex);
  this->children[1]=argument;
}

void Expression::MakeFunction
  (CommandList& owner, int inputIndexBoundVars, const Expression& argument, const std::string& functionName)
{ int index=owner.GetIndexNonBoundVar(functionName);
  assert(index!=-1);
  this->MakeFunction(owner, inputIndexBoundVars, argument, index);
}

Data Data::operator/(const Data& right)
{ Data result(*this);
  if (this->type!=this->typeRational || right.type!=this->typeRational)
  { std::cout << "I cannot divide expression of type " << this->type << " by expression of type " << right.type;
    assert(false);
    return result;
  }
  if (right.theRational.GetElementConst().IsEqualToZero())
    result.SetError("Error: division by zero");
  else
    result=Data(this->theRational.GetElementConst()/right.theRational.GetElementConst(), *this->owner);
  return result;
}

Data Data::operator*(const Data& right)
{ Data result(*this);
  if (this->type!=this->typeRational || right.type!=this->typeRational)
  { std::cout << "I cannot divide expression of type " << this->type << " by expression of type " << right.type;
    assert(false);
    return result;
  }
  result=Data(this->theRational.GetElementConst()*right.theRational.GetElementConst(), *this->owner);
  return result;
}

bool Data::operator+=(const Data& other)
{ std::stringstream out;
  if (this->type!=other.type)
  { out << "Adding different types, " << this->ElementToStringDataType() << " and "
    << other.ElementToStringDataType() << ", is not allowed.";
    return this->SetError(out.str());
  }
  switch(this->type)
  { case Data::typeRational:
      this->theRational.GetElement()+=other.theRational.GetElementConst();
      return true;
    default:
      out << "Don't know how to add elements of type " << this->ElementToStringDataType() << ". ";
      return this->SetError(out.str());
  }
}

bool Data::operator==(const Data& other)
{ if(this->owner!=other.owner)
    return false;
  if (this->type!=other.type)
    return false;
  switch(this->type)
  { case Data::typeSSalgebra:
    case Data::typeElementSSalgebra:
    case Data::typeRational:
      return this->theRational.GetElement()==other.theRational.GetElementConst();
    case Data::typePoly:
      return this->thePoly.GetElement()==other.thePoly.GetElementConst();
    case Data::typeError:
      return this->theError.GetElement()==other.theError.GetElementConst();
    default:
      std::cout << "This is a programming error: operator== does not cover type "
      << this->ElementToStringDataType()
      << " Please debug file " << __FILE__ << " line " << __LINE__ << ".";
      assert(false);
      return false;
  }
}

int Data::HashFunction()const
{ switch (this->type)
  { case Data::typeSSalgebra:
    case Data::typeElementSSalgebra:
    case Data::typeRational:
      return this->theRational.GetElementConst().HashFunction()*this->type;
    case Data::typePoly:
      return this->thePoly.GetElementConst().HashFunction()*this->type;
    case Data::typeError:
      return MathRoutines::hashString(this->theError.GetElementConst());
    default:
      std::cout << "This is a programming error. Data::HashFunction() does not cover type "
      << this->type << ", please debug line"
      << __LINE__;
      assert(false);
      return 0;
  }
}

bool CommandList::ReplaceIntIntBy10IntPlusInt()
{ SyntacticElement& left=this->syntacticStack[this->syntacticStack.size-2];
  SyntacticElement& right=this->syntacticStack[this->syntacticStack.size-1];
  Data tempData(this->theData[left.theData.theData]);
  tempData*=10;
  tempData+=this->theData[right.theData.theData];
  left.theData.MakeDatA(tempData, *this, this->theExpressionContext.size-1);
  this->DecreaseStackSetCharacterRanges(1);
  return true;
}

void Data::operator=(const Data& other)
{ if (this==&other)
    return;
  this->type=other.type;
  this->owner=other.owner;
  switch(this->type)
  { case Data::typeElementSSalgebra:
    case Data::typeSSalgebra:
    case Data::typeRational: this->theRational=other.theRational; break;
    case Data::typePoly: this->thePoly=other.thePoly; break;
    case Data::typeError: this->theError=other.theError; break;
    default:
      std::cout << "This is a programming error: operator= does not cover type "
      << this->ElementToStringDataType()
      << ". Please debug file " << __FILE__ << " line " << __LINE__ << ".";
      assert(false);
  }
}

std::string Data::ElementToStringDataType() const
{ switch(this->type)
  { case Data::typeElementSSalgebra: return "ElementSSalgebra";
    case Data::typeSSalgebra: return "SemisimpleLieAlgebra";
    case Data::typeRational:  return "Rational";
    case Data::typeError:  return "Error";
    default:
      std::cout << "This is a programming error: Data::ElementToStringDataType does not cover all cases. Please "
      << " debug file " << __FILE__ << " line " << __LINE__ << ".";
      assert(false);
      return "unknown";
  }
}

std::string Data::ElementToString(std::stringstream* comments)const
{ std::stringstream out;
  if (this->owner==0)
    return "(ProgrammingError:NoOwner)";
  switch(this->type)
  { case Data::typeSSalgebra:
      out << "SemisimpleLieAlgebra{}("
      << this->owner->theLieAlgebras[this->theRational.GetElementConst().NumShort].GetLieAlgebraName(false) << ")";
//      if (comments!=0)
//        *comments << "Comments to data " << out.str() << ":<br>"
//        << this->owner->theLieAlgebras[this->theRational.GetElementConst().NumShort].
//        ElementToString(*this->owner->theGlobalVariableS);
      return out.str();
    case Data::typeElementSSalgebra:
      out << this->owner->theLieAlgebraElements[this->theRational.GetElementConst().NumShort].ElementToString();
      return out.str();
    case Data::typeRational:
      return this->theRational.GetElementConst().ElementToString();
    case Data::typePoly:
      out << "Polynomial{}(" << this->thePoly.GetElementConst().ElementToString() << ")";
      return out.str();
    case Data::typeError:
      out << "(Error)";
      if (comments!=0)
        *comments << this->theError.GetElementConst();
      return out.str();
    default:
      std::cout << "This is a programming error: don't know how to convert element of type " << this->type
      << " (type " << this->ElementToStringDataType() << ") to string. "
      << "Please debug file " << __FILE__ << " line " << __LINE__;
      assert(false);
      return out.str();
  }
}

bool CommandList::ReplaceXEXEXByEusingO(int theControlIndex, int formatOptions)
{ SyntacticElement& lefT = this->syntacticStack[this->syntacticStack.size-4];
  SyntacticElement& right = this->syntacticStack[this->syntacticStack.size-2];
  SyntacticElement& result = this->syntacticStack[this->syntacticStack.size-5];
  Expression newExpr;
  newExpr.reset(*this, this->theExpressionContext.size-1);
  newExpr.theOperation=this->GetOperationIndexFromControlIndex(theControlIndex);
  newExpr.children.AddOnTop(lefT.theData);
  newExpr.children.AddOnTop(right.theData);
  newExpr.format=formatOptions;
  result.theData=newExpr;
  result.controlIndex=this->conExpression();
  this->DecreaseStackSetCharacterRanges(4);
  return true;
}

bool CommandList::ReplaceEEndCommandEbyE()
{ SyntacticElement& left = this->syntacticStack[this->syntacticStack.size-3];
  SyntacticElement& right = this->syntacticStack[this->syntacticStack.size-1];
  assert(left.theData.IndexBoundVars!=-1);
  if (left.theData.theOperation==this->opEndStatement())
    left.theData.children.AddOnTop(right.theData);
  else
  { Expression newExpr;
    newExpr.reset(*this, this->theExpressionContext.size-1);
    newExpr.theOperation=this->opEndStatement();
    newExpr.children.AddOnTop(left.theData);
    newExpr.children.AddOnTop(right.theData);
    newExpr.format=Expression::formatDefault;
    left.theData=newExpr;
  }
  this->DecreaseStackSetCharacterRanges(2);
//    std::cout << this->syntacticStack[this->syntacticStack.size()-1].theData.ElementToStringPolishForm();
  return true;
}

bool CommandList::ReplaceEXEByEusingO(int theControlIndex, int formatOptions)
{ SyntacticElement& left = this->syntacticStack[this->syntacticStack.size-3];
  SyntacticElement& right = this->syntacticStack[this->syntacticStack.size-1];
  Expression newExpr;
  newExpr.reset(*this, this->theExpressionContext.size-1);
  newExpr.theOperation=this->GetOperationIndexFromControlIndex(theControlIndex);
  newExpr.children.AddOnTop(left.theData);
  newExpr.children.AddOnTop(right.theData);
  newExpr.format=formatOptions;
  left.theData=newExpr;
  this->DecreaseStackSetCharacterRanges(2);
//    std::cout << this->syntacticStack[this->syntacticStack.size()-1].theData.ElementToStringPolishForm();
  return true;
}

bool Data::MakeElementSemisimpleLieAlgebra
(CommandList& owner, SemisimpleLieAlgebra& ownerAlgebra, int index1, int index2, std::stringstream* comments)
{ bool isGood=(index1==0 && index2 <=ownerAlgebra.GetRank() && index2>0);
  if (!isGood)
  { if (comments!=0)
      *comments
       << "You requested element of the Cartan subalgebra labeled by (" << index1 << ", " << index2
      << " of semisimple Lie algebra " << ownerAlgebra.GetLieAlgebraName()
      << "). For your request to succeed, the first index must be zero and the second must be an integer"
      << " between 1 and the rank of the Algebra which is " << ownerAlgebra.GetRank()
      << ". If you want to request an element that is in a root space outside of the Cartan, you should only one index."
      ;
    return false;
  }
  ElementSimpleLieAlgebra tempElt;
  int actualIndeX=index2-1+ownerAlgebra.GetNumPosRoots();
  tempElt.AssignChevalleyGeneratorCoeffOneIndexNegativeRootspacesFirstThenCartanThenPositivE
  (actualIndeX, ownerAlgebra);
  this->owner=&owner;
  this->type=this->typeElementSSalgebra;
  this->theRational.GetElement()=owner.theLieAlgebraElements.AddNoRepetitionOrReturnIndexFirst(tempElt);
  return true;
}

bool Data::MakeElementSemisimpleLieAlgebra
(CommandList& inputOwner, SemisimpleLieAlgebra& ownerAlgebra, int theDisplayIndex, std::stringstream* comments)
{ bool isGood=(theDisplayIndex>0 && theDisplayIndex<= ownerAlgebra.GetNumPosRoots()) ||
  (theDisplayIndex<0 && theDisplayIndex>=- ownerAlgebra.GetNumPosRoots());
  if (!isGood)
  { if (comments!=0)
      *comments
       << "<b>Error</b>. You requested element of index " << theDisplayIndex
      << " of semisimple Lie algebra " << ownerAlgebra.GetLieAlgebraName()
      << ". The index of the root space must be a non-zero integer "
      << " of absolute value between 1 and the number of positive roots. "
      << "The number of positive roots for the current semisimple Lie algebra is "
      << ownerAlgebra.GetNumPosRoots()
      << ". If you want to request an element of the Cartan, you should use two indices, the first of which is zero. For example,"
      << " ElementSemisimpleAlgebra{}(0,1) gives you the an element of the Cartan corresponding to the first simple root. "
      ;
    return false;
  }
  ElementSimpleLieAlgebra tempElt;
  int actualIndex=theDisplayIndex;
  if (actualIndex<0)
    actualIndex+=ownerAlgebra.GetNumPosRoots();
  else
    actualIndex+=ownerAlgebra.GetNumPosRoots()+ownerAlgebra.GetRank()-1;
  tempElt.AssignChevalleyGeneratorCoeffOneIndexNegativeRootspacesFirstThenCartanThenPositivE
  (actualIndex, ownerAlgebra);
  this->owner=&inputOwner;
  this->type=this->typeElementSSalgebra;
  this->theRational.GetElement()=inputOwner.theLieAlgebraElements.AddNoRepetitionOrReturnIndexFirst(tempElt);
  return true;
}

SemisimpleLieAlgebra& ElementSimpleLieAlgebra::GetOwner()
{ if (this->ownerArray==0 || this->indexOfOwnerAlgebra==-1)
  { std::cout << "This is a programming error: a semisimple Lie algebra element has not been initialized properly. "
    << "Please debug file " << __FILE__ << " line " << __LINE__ << ". ";
    assert(false);
  }
  if ( this->indexOfOwnerAlgebra<0 || this->ownerArray->size<=this->indexOfOwnerAlgebra)
  { std::cout << "This is a programming error: a semisimple Lie algebra container has not been initialized properly. "
    << "Please debug file " << __FILE__ << " line " << __LINE__ << ". ";
    assert(false);
  }
  return this->ownerArray->TheObjects[this->indexOfOwnerAlgebra];
}

bool Data::LieBracket
  (Data& left, Data& right, Data& output, std::stringstream* comments)
{ if (left.type==Data::typeError || right.type==Data::typeError)
    return false;
  if (left.type==Data::typeRational || right.type==Data::typeRational)
  { output=Data(0, *left.owner);
    return true;
  }
  if (left.type==Data::typeElementSSalgebra || right.type==Data::typeElementSSalgebra)
  { ElementSimpleLieAlgebra& leftElt=left.owner->theLieAlgebraElements[left.GetSmallInt()];
    ElementSimpleLieAlgebra& rightElt=right.owner->theLieAlgebraElements[right.GetSmallInt()];
    if (leftElt.ownerArray!=rightElt.ownerArray || leftElt.indexOfOwnerAlgebra!=rightElt.indexOfOwnerAlgebra)
      return false;
    SemisimpleLieAlgebra& theOwnerAlg=leftElt.GetOwner();
    ElementSimpleLieAlgebra outputElt;
    theOwnerAlg.LieBracket(leftElt, rightElt, outputElt);
    output.owner=left.owner;
    output.type=output.typeElementSSalgebra;
    output.theRational.GetElement()=output.owner->theLieAlgebraElements.AddNoRepetitionOrReturnIndexFirst(outputElt);
    return true;
  }
  return false;
}

bool Data::OperatorDereference
  (const Data& argument1, const Data& argument2, Data& output, std::stringstream* comments)
{ int whichInteger1, whichInteger2;
  switch(this->type)
  { case Data::typeSSalgebra:
      if (!argument1.IsSmallInteger(whichInteger1) ||!argument2.IsSmallInteger(whichInteger2) )
      { if (comments!=0)
        *comments << "You requested element of a semisimple Lie algebra labeled by ("
        << argument1.ElementToString() << ", " << argument2.ElementToString()
        << ")  which is not a pair of small integers. ";
        return false;
      }
      return output.MakeElementSemisimpleLieAlgebra
      (*this->owner, this->owner->theLieAlgebras[this->GetSmallInt()], whichInteger1, whichInteger2, comments);
    default:
      return false;
  }
}

bool Data::OperatorDereference
  (const Data& argument, Data& output, std::stringstream* comments)
{ int whichInteger;
  switch(this->type)
  { case Data::typeSSalgebra:
      if (!argument.IsSmallInteger(whichInteger))
      { if (comments!=0)
        *comments << "You requested element of a semisimple Lie algebra labeled by "
        << argument.ElementToString() << " which is not a small integer. ";
        return false;
      }
      return output.MakeElementSemisimpleLieAlgebra
      (*this->owner, this->owner->theLieAlgebras[this->GetSmallInt()], whichInteger, comments);
    default:
      return false;
  }
}

bool Data::IsEqualToOne()const
{ switch (this->type)
  { case Data::typeRational:
      return this->theRational.GetElementConst().IsEqualToOne();
    default:
      return false;
  }
}

bool Data::IsEqualToZero()const
{ switch (this->type)
  { case Data::typeRational:
      return this->theRational.GetElementConst().IsEqualToZero();
    default:
      return false;
   }
}

bool Data::operator*=(const Data& other)
{ std::stringstream out;
  if (this->type!=other.type)
  { out << "Multiplying different types, " << this->type << " and " << other.type << ", is not allowed.";
    return this->SetError(out.str());
  }
  switch(this->type)
  { case Data::typeRational:
      this->theRational.GetElement()*=other.theRational.GetElementConst();
      return true;
    default:
      out << "Don't know how to multiply elements of type " << this->type << ". ";
      return this->SetError(out.str());
  }
}

bool Data::operator/=(const Data& other)
{ std::stringstream out;
  if (this->type!=other.type)
  { out << "Multiplying different types, " << this->type << " and " << other.type << ", is not allowed.";
    return this->SetError(out.str());
  }
  if (other.IsEqualToZero())
    return false;
  switch(this->type)
  { case Data::typeRational:
      this->theRational.GetElement()/=other.theRational.GetElementConst();
      return true;
    default:
      out << "Don't know how to multiply elements of type " << this->type << ". ";
      return this->SetError(out.str());
  }
}

std::string CommandList::ElementToStringSyntacticStack()
{ std::stringstream out;
  if (this->syntacticStack.size<this->numEmptyTokensStart)
    return "Error: this is a programming error: not enough empty tokens in the start of the syntactic stack.";
  bool HasMoreThanOneSignificantEntriesOrIsNotExpression=(this->syntacticStack.size>this->numEmptyTokensStart+1);
  SyntacticElement& lastSyntacticElt=* this->syntacticStack.LastObject();
  if (this->syntacticStack.size==this->numEmptyTokensStart+1)
    if (lastSyntacticElt.controlIndex!=this->conExpression())
      HasMoreThanOneSignificantEntriesOrIsNotExpression=true;
  if (HasMoreThanOneSignificantEntriesOrIsNotExpression)
    out << "<table border=\"1\"><tr><td>";
  for (int i=this->numEmptyTokensStart; i<this->syntacticStack.size; i++)
  { out << this->syntacticStack[i].ElementToString(*this);
    if (HasMoreThanOneSignificantEntriesOrIsNotExpression)
    { out << "</td>";
      if (i!=this->syntacticStack.size-1)
        out << "<td>";
    }
  }
  if (HasMoreThanOneSignificantEntriesOrIsNotExpression)
    out << "</td></tr></table>";
  return out.str();
}

bool CommandList::ReplaceXXXByCon(int theCon)
{ this->syntacticStack[this->syntacticStack.size-3].controlIndex=theCon;
  this->DecreaseStackSetCharacterRanges(2);
  return true;
}

bool CommandList::ReplaceXXXByConCon(int con1, int con2, int inputFormat1, int inputFormat2)
{ this->syntacticStack[this->syntacticStack.size-2].controlIndex=con1;
  this->syntacticStack[this->syntacticStack.size-3].controlIndex=con2;
  this->syntacticStack[this->syntacticStack.size-2].theData.format=inputFormat2;
  this->syntacticStack[this->syntacticStack.size-3].theData.format=inputFormat1;
  this->DecreaseStackSetCharacterRanges(1);
  return true;
}

bool CommandList::ReplaceXXXXXByCon(int theCon, int inputFormat)
{ this->syntacticStack[this->syntacticStack.size-5].controlIndex=theCon;
  this->DecreaseStackSetCharacterRanges(4);
  return true;
}

bool CommandList::ReplaceXXXXXByConCon(int con1, int con2, int inputFormat1, int inputFormat2)
{ this->syntacticStack[this->syntacticStack.size-4].controlIndex=con2;
  this->syntacticStack[this->syntacticStack.size-5].controlIndex=con1;
  this->syntacticStack[this->syntacticStack.size-4].theData.format=inputFormat2;
  this->syntacticStack[this->syntacticStack.size-5].theData.format=inputFormat1;
  this->DecreaseStackSetCharacterRanges(3);
  return true;
}

bool CommandList::ReplaceXXXXByConCon(int con1, int con2, int inputFormat1, int inputFormat2)
{ this->syntacticStack[this->syntacticStack.size-3].controlIndex=con2;
  this->syntacticStack[this->syntacticStack.size-4].controlIndex=con1;
  this->syntacticStack[this->syntacticStack.size-3].theData.format=inputFormat2;
  this->syntacticStack[this->syntacticStack.size-4].theData.format=inputFormat1;
  this->DecreaseStackSetCharacterRanges(2);
  return true;
}

bool CommandList::ReplaceXXXXByCon(int con1, int inputFormat1)
{ this->syntacticStack[this->syntacticStack.size-4].controlIndex=con1;
  this->syntacticStack[this->syntacticStack.size-4].theData.format=inputFormat1;
  this->DecreaseStackSetCharacterRanges(3);
  return true;
}

bool CommandList::ReplaceXByCon(int theCon, int theFormat)
{ this->syntacticStack[this->syntacticStack.size-1].controlIndex=theCon;
  this->syntacticStack[this->syntacticStack.size-1].theData.format=theFormat;
  //    this->DecreaseStackSetCharacterRanges(2);
  return true;
}

bool CommandList::ReplaceXByConCon
(int con1, int con2, int format1, int format2)
{ this->syntacticStack.SetSize(this->syntacticStack.size+1);
  this->syntacticStack.LastObject()->theData.reset(*this, this->theExpressionContext.size-1);
  this->syntacticStack[this->syntacticStack.size-2].controlIndex=con1;
  this->syntacticStack[this->syntacticStack.size-1].controlIndex=con2;
  this->syntacticStack[this->syntacticStack.size-2].theData.format=format1;
  this->syntacticStack[this->syntacticStack.size-1].theData.format=format2;
  //    this->DecreaseStackSetCharacterRanges(2);
  return true;
}

template < >
bool Data::IsOfType<PolynomialRationalCoeff>()const
{ return this->type==this->typePoly;
}

template < >
PolynomialRationalCoeff Data::GetValue<PolynomialRationalCoeff>()const
{ if (this->type!=this->typePoly)
  { std::cout << "This is a programming error. Data of type "
    << this->ElementToStringDataType()
    << " is treated as if it were data of type Polynomial."
    << " Please debug file " << __FILE__ << " line " <<__LINE__;
    assert(false);
  }
  return this->thePoly.GetElementConst();
}

template < >
bool Data::IsOfType<Rational>()const
{ return this->type==this->typeRational;
}

template < >
Rational Data::GetValue<Rational>()const
{ if (this->type!=this->typeRational)
  { std::cout << "This is a programming error. Data of type "
    << this->ElementToStringDataType()
    << " is treated as if it were data of type Rational."
    << " Please debug file " << __FILE__ << " line " <<__LINE__;
    assert(false);
  }
  return this->theRational.GetElementConst();
}


