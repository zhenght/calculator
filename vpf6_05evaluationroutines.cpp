//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#include "vpf.h"
ProjectInformationInstance ProjectInfoVpf6_05cpp(__FILE__, "Calculator core evaluation engine. ");

std::string Calculator::ToStringFunctionHandlers()
{ MacroRegisterFunctionWithName("Calculator::ToStringFunctionHandlers");
  std::stringstream out;
  int numOpsHandled=0;
  int numHandlers=0;
  int numInnerHandlers=0;
  for (int i=0; i<this->theAtoms.size; i++)
  { if (this->FunctionHandlers[i].size!=0)
      numOpsHandled++;
    numHandlers+=this->FunctionHandlers[i].size;
    for (int j=0; j<this->FunctionHandlers[i].size; j++)
      if (this->FunctionHandlers[i][j].flagIsInner)
        numInnerHandlers++;
  }
  out << "\n <b> " << numOpsHandled << " built-in atoms are handled by a total of " << numHandlers << " handler functions ("
  << numInnerHandlers << " inner and " << numHandlers-numInnerHandlers << " outer).</b><br>\n";
  bool found=false;
  for (int i=0; i<this->theAtoms.size; i++)
  { int indexCompositeHander=this->operationsComposite.GetIndex(this->theAtoms[i]);
    if (this->FunctionHandlers[i].size>0)
      for (int j=0; j<this->FunctionHandlers[i].size; j++)
        if (this->FunctionHandlers[i][j].flagIamVisible)
        { if (found)
            out << "<br>\n";
          found=true;
          out << this->FunctionHandlers[i][j].ToStringFull();
        }
    if (indexCompositeHander!=-1)
      for (int j=0; j<this->operationsCompositeHandlers[indexCompositeHander].size; j++)
        if (this->operationsCompositeHandlers[indexCompositeHander][j].flagIamVisible)
        { if (found)
            out << "<br>\n";
          found=true;
          out << this->operationsCompositeHandlers[indexCompositeHander][j].ToStringFull();
        }
  }
  return out.str();
}

bool Calculator::outerStandardFunction(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("Calculator::outerStandardFunction");
  RecursionDepthCounter theCounter(&theCommands.RecursionDeptH);
  theCommands.CheckInputNotSameAsOutput(input, output);
  if (!input.IsLisT())
    return false;
  const Expression& functionNameNode =input[0];
  if (functionNameNode.StartsWith())
  { const List<Function>* theHandlers=theCommands.GetOperationCompositeHandlers(functionNameNode[0].theData);
    if (theHandlers!=0)
      for (int i=0; i<theHandlers->size; i++)
        if ((*theHandlers)[i].theFunction(theCommands, input, output))
        { if (theCommands.flagLogEvaluatioN)
            theCommands << "<hr>Built-in substitution: " << (*theHandlers)[i].ToStringSummary() << "<br>Rule stack id: "
            << theCommands.RuleStackCacheIndex;
          return true;
        }
  }
  if (!functionNameNode.IsAtom())
    return false;
//  stOutput << "<br>Evaluating: " << input.ToString();
//  stOutput.flush();
  for (int i=0; i<theCommands.FunctionHandlers[functionNameNode.theData].size; i++)
    if (!theCommands.FunctionHandlers[functionNameNode.theData][i].flagIsInner)
    { Function& outerFun=theCommands.FunctionHandlers[functionNameNode.theData][i];
      if (outerFun.theFunction(theCommands, input, output))
        if(output!=input)
        { output.CheckConsistency();
          if (theCommands.flagLogEvaluatioN)
            theCommands << "<hr>Built-in substitution: " << outerFun.ToStringSummary() << "<br>Rule stack id: "
            << theCommands.RuleStackCacheIndex;
          return true;
        }
    } else
    { Function& innerFun=theCommands.FunctionHandlers[functionNameNode.theData][i];
      if (input.children.size>2)
      { //stOutput << "more than 2 children: " << input.Lispify();
        if (innerFun.inputFitsMyInnerType(input))
          if (innerFun.theFunction(theCommands, input, output))
          { output.CheckConsistency();
            if (theCommands.flagLogEvaluatioN)
              theCommands << "<hr>Built-in substitution: " << innerFun.ToStringSummary() << "<br>Rule stack id: "
              << theCommands.RuleStackCacheIndex;
            return true;
          }
      } else
        if (innerFun.inputFitsMyInnerType(input[1]))
          if (innerFun.theFunction(theCommands, input[1], output))
          { output.CheckConsistency();
            if (theCommands.flagLogEvaluatioN)
              theCommands << "<hr>Built-in substitution: " << innerFun.ToStringSummary()
              << "<br>Rule stack id: " << theCommands.RuleStackCacheIndex;
            return true;
          }
    }
  return false;
}

bool Calculator::ExpressionMatchesPattern(const Expression& thePattern, const Expression& input, BoundVariablesSubstitution& matchedExpressions, std::stringstream* theLog)
{ MacroRegisterFunctionWithName("Calculator::ExpressionMatchesPattern");
  RecursionDepthCounter recursionCounter(&this->RecursionDeptH);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (!(thePattern.theBoss==this && input.theBoss==this))
    crash << "This is a programming error. Either a pattern or an input has a wrongly  initialized owner: the pattern is "
    << thePattern.ToString() << " and the input is " << input.ToString() << ". The error is certainly in the preceding code; here "
    << "is a stack trace, however beware that the error might be in code preceding the stack loading. "
    << crash;
//  static int ExpressionMatchesPatternDebugCounter=-1;
  static const bool printLocalDebugInfo=false;
//  if (input.ToString()=="f{}((a))=a+5")
//    printLocalDebugInfo=true;
  //ExpressionMatchesPatternDebugCounter++;
//  stOutput << " ExpressionMatchesPatternDebugCounter: " << ExpressionMatchesPatternDebugCounter;
//  printLocalDebugInfo=(ExpressionMatchesPatternDebugCounter>-1);
  if (printLocalDebugInfo)
  { stOutput << " <hr> current input: " << input.ToString() << "<br>current pattern: " << thePattern.ToString();
    stOutput << "<br> current matched expressions: " << matchedExpressions.ToString();
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (this->RecursionDeptH>this->MaxRecursionDeptH)
  { std::stringstream out;
    out << "Max recursion depth of " << this->MaxRecursionDeptH << " exceeded whlie trying to match expression pattern "
    << thePattern.ToString() << " onto expression " << input.ToString();
    this->evaluationErrors.AddOnTop(out.str());
    return false;
  }
//  if (this->opDefine()==input.theOperation)
//    return false;
  int opVarB=this->opBind();
  if (thePattern.IsListStartingWithAtom(opVarB))
  { int indexLeft= matchedExpressions.theBoundVariables.GetIndex(thePattern);
    if (indexLeft==-1)
    { matchedExpressions.theBoundVariables.AddOnTop(thePattern);
      matchedExpressions.variableImages.AddOnTop(input);
      indexLeft=matchedExpressions.theBoundVariables.size-1;
    }
    if (matchedExpressions.variableImages[indexLeft]!=input)
      return false;
    if (printLocalDebugInfo)
      stOutput << "<br><b>Match!</b>";
    return true;
  }
  if (thePattern.theData!=input.theData || thePattern.children.size!=input.children.size )
    return false;
  for (int i=0; i<thePattern.children.size; i++)
    if (!(this->ExpressionMatchesPattern(thePattern[i], input[i], matchedExpressions, theLog)))
      return false;
  if (printLocalDebugInfo)
    stOutput << "<br><b>Match!</b>";
  return true;
}

struct StackMaintainerRules
{
public:
  Calculator* theBoss;
  int startingRuleStackIndex;
  int startingRuleStackSize;
  StackMaintainerRules(Calculator* inputBoss);
  void AddRule(const Expression& theRule);
  ~StackMaintainerRules();
};

void StackMaintainerRules::AddRule(const Expression& theRule)
{ if (this->theBoss==0)
    crash << crash;
  this->theBoss->RuleStack.AddChildOnTop(theRule);
  this->theBoss->RuleStackCacheIndex=this->theBoss->cachedRuleStacks.GetIndex(this->theBoss->RuleStack);
  if (this->theBoss->RuleStackCacheIndex==-1)
    if (this->theBoss->cachedRuleStacks.size<this->theBoss->MaxCachedExpressionPerRuleStack)
    { this->theBoss->RuleStackCacheIndex=this->theBoss->cachedRuleStacks.size;
      this->theBoss->cachedRuleStacks.AddOnTop(this->theBoss->RuleStack);
    }
  if (this->theBoss->flagLogRules)
    *this->theBoss << "<hr>Added rule " << theRule.ToString() << " with state identifier "
    << this->theBoss->RuleStackCacheIndex;
}

StackMaintainerRules::StackMaintainerRules(Calculator* inputBoss)
{ this->theBoss=inputBoss;
  if (this->theBoss==0)
    return;
  this->startingRuleStackIndex=inputBoss->RuleStackCacheIndex;
  this->startingRuleStackSize=inputBoss->RuleStack.children.size;
}

StackMaintainerRules::~StackMaintainerRules()
{ if (this->theBoss==0)
    return;
  this->theBoss->RuleStackCacheIndex=this->startingRuleStackIndex;
  this->theBoss->RuleStack.children.SetSize(this->startingRuleStackSize);
  this->theBoss=0;
}

bool Calculator::EvaluateExpression
(Calculator& theCommands, const Expression& input, Expression& output)
{ RecursionDepthCounter recursionCounter(&theCommands.RecursionDeptH);
  MacroRegisterFunctionWithName("Calculator::EvaluateExpression");
  if (theCommands.flagLogFullTreeCrunching && theCommands.RecursionDeptH<3)
  { theCommands << "<br>";
    for (int i=0; i<theCommands.RecursionDeptH; i++)
      theCommands << "&nbsp&nbsp&nbsp&nbsp";
      //stOutput << "&nbsp&nbsp&nbsp&nbsp";
    theCommands << "Evaluating " << input.Lispify() << " with rule stack cache index " << theCommands.RuleStackCacheIndex; // << this->RuleStack.ToString();
//    stOutput << "Evaluating " << input.Lispify() << " with rule stack of size " << this->RuleStack.size; // << this->RuleStack.ToString();
  }
  if (theCommands.RecursionDepthExceededHandleRoughly())
    return theCommands << " Evaluating expression: " << input.ToString() << " aborted. ";
  output=input;
  if (output.IsError())
  { theCommands.flagAbortComputationASAP=true;
    return true;
  }
  //////////////////////////////
  //  stOutput << "temporary check comment me out";
  //  theCommands.ExpressionStack.GrandMasterConsistencyCheck();
  //  input.CheckConsistency();
  //  input.HashFunction();
  //////////////////////////////
  if (theCommands.EvaluatedExpressionsStack.Contains(input))
  { std::stringstream errorStream;
    errorStream << "I think I have detected an infinite cycle: I am asked to reduce " << input.ToString()
    << " but I have already seen that expression in the expression stack. ";
    theCommands.flagAbortComputationASAP=true;
    return output.MakeError(errorStream.str(), theCommands);
  }
  theCommands.EvaluatedExpressionsStack.AddOnTop(input);
  Expression theExpressionWithContext;
  theExpressionWithContext.reset(theCommands, 3);
  theExpressionWithContext.AddChildAtomOnTop(theCommands.opSequence());
  theExpressionWithContext.AddChildValueOnTop(theCommands.RuleStackCacheIndex);
  theExpressionWithContext.AddChildOnTop(input);
  int indexInCache=theCommands.cachedExpressions.GetIndex(theExpressionWithContext);
  if (indexInCache!=-1)
  { if (theCommands.flagLogCache)
    { theCommands << "<hr>Cache hit with state identifier " << theCommands.RuleStackCacheIndex << ": "
      << output.ToString() << " -> " << theCommands.imagesCachedExpressions[indexInCache].ToString();
    }
    output= theCommands.imagesCachedExpressions[indexInCache];
  } else
  { bool shouldCache=
    theCommands.cachedExpressions.size<theCommands.MaxCachedExpressionPerRuleStack &&
    !output.IsBuiltInType() && !output.IsBuiltInAtom();
    if (shouldCache)
    { theCommands.cachedExpressions.AddOnTop(theExpressionWithContext);
      indexInCache=theCommands.cachedExpressions.size-1;
      theCommands.imagesCachedExpressions.SetSize(indexInCache+1);
      theCommands.imagesCachedExpressions.LastObject()->reset(theCommands);
      theCommands.imagesCachedExpressions.LastObject()->MakeError("Error: not computed yet.", theCommands);
    }
  }
  //reduction phase:
  bool ReductionOcurred=true;
  int counterNumTransformations=0;
  Expression transformationE(theCommands);
  Expression beforePatternMatch;
  while (ReductionOcurred && !theCommands.flagAbortComputationASAP)
  { StackMaintainerRules theRuleStackMaintainer(&theCommands);
    ReductionOcurred=false;
    counterNumTransformations++;
    if (indexInCache!=-1)
      theCommands.imagesCachedExpressions[indexInCache]=output;
    //////------Handling naughty expressions------
    if (theCommands.theGlobalVariableS->MaxComputationTimeSecondsNonPositiveMeansNoLimit>0)
      if (theCommands.theGlobalVariableS->GetElapsedSeconds()!=0)
        if (theCommands.theGlobalVariableS->GetElapsedSeconds()>theCommands.theGlobalVariableS->MaxComputationTimeSecondsNonPositiveMeansNoLimit/2)
        { if (!theCommands.flagTimeLimitErrorDetected)
            stOutput << "<br><b>Max allowed computational time is " << theCommands.theGlobalVariableS->MaxComputationTimeSecondsNonPositiveMeansNoLimit/2 << ";  so far, "
            << theCommands.theGlobalVariableS->GetElapsedSeconds()-theCommands.StartTimeEvaluationInSecondS << " have elapsed -> aborting computation ungracefully.</b>";
          theCommands.flagTimeLimitErrorDetected=true;
          theCommands.flagAbortComputationASAP=true;
          break;
        }
    if (counterNumTransformations>theCommands.MaxAlgTransformationsPerExpression)
    { if (!theCommands.flagMaxTransformationsErrorEncountered)
      { std::stringstream out;
        out << " While simplifying " << output.ToString(&theCommands.formatVisibleStrings) << "<br>Maximum number of algebraic transformations of "
        << theCommands.MaxAlgTransformationsPerExpression << " exceeded.";
        output.MakeError(out.str(), theCommands);
        theCommands.flagAbortComputationASAP=true;
        theCommands.flagMaxTransformationsErrorEncountered=true;
      }
      break;
    }
    //////------End of handling naughty expressions------
    /////-------Evaluating children if the expression is not of built-in type-------
    //bool foundError=false;
    ProgressReport theReport(theCommands.theGlobalVariableS);
    if (!output.IsFrozen())
      for (int i=0; i<output.children.size && !theCommands.flagAbortComputationASAP; i++)
      { if (theCommands.theGlobalVariableS!=0)
          if (i>0 && output.StartsWith(theCommands.opEndStatement()))
          { std::stringstream reportStream;
            for (int j=1; j<i; j++)
              if (output[j].StartsWith(theCommands.opDefine()) || output[j].StartsWith(theCommands.opDefineConditional()))
                reportStream << "<br>" << output[j].ToString();
            reportStream << "<br><b>" << output[i].ToString() << "</b>";
            theReport.Report(reportStream.str());
          }
        if (theCommands.EvaluateExpression(theCommands, output[i], transformationE))
          output.SetChilD(i, transformationE);
        if (output[i].IsError())
        { theCommands.flagAbortComputationASAP=true;
          break;
        }
        if (output.StartsWith(theCommands.opEndStatement()))
          if (output[i].StartsWith(theCommands.opDefine()) || output[i].StartsWith(theCommands.opDefineConditional()))
            theRuleStackMaintainer.AddRule(output[i]);
      }
    if (theCommands.flagAbortComputationASAP)
      break;
    /////-------Default operation handling-------
    if (theCommands.outerStandardFunction(theCommands, output, transformationE))
    { ReductionOcurred=true;
      if (theCommands.flagLogEvaluatioN)
      { /* *this << "<br>";
        if (theCommands.flagLogRules)
        { *this << "<br>Rule stack size: " << theCommands.RuleStack.size << ", context identifier: "
          << theCommands.RuleContextIdentifier << "<br>Rules: " << theCommands.RuleStack.ToString() << "<br>";
        }*/
        theCommands << "<br>Rule context identifier: " << theCommands.RuleStackCacheIndex
        << "<br>" << CGI::GetMathMouseHover(output.ToString()) << " -> "
        << CGI::GetMathMouseHover(transformationE.ToString());
      }
      output=transformationE;
      continue;
    }
    /////-------Default operation handling end-------
    /////-------Evaluating children end-------
    /////-------User-defined pattern matching------
    for (int i=0; i<theCommands.RuleStack.children.size && !theCommands.flagAbortComputationASAP; i++)
    { const Expression& currentPattern=theCommands.RuleStack[i];
      theCommands.TotalNumPatternMatchedPerformed++;
      if (theCommands.flagLogEvaluatioN)
        beforePatternMatch=output;
      BoundVariablesSubstitution bufferPairs;
      if(theCommands.ProcessOneExpressionOnePatternOneSub(currentPattern, output, bufferPairs, &theCommands.Comments, theCommands.flagLogPatternMatching))
      { ReductionOcurred=true;
        if (theCommands.flagLogEvaluatioN)
        { /*
          if (theCommands.flagLogRules)
          { *this << "<br>Rule stack size: " << theCommands.RuleStack.size << ", context identifier: "
            << theCommands.RuleContextIdentifier << "<br>Rules: " << theCommands.RuleStack.ToString() << "<br>";
          }
          */
          theCommands << "<hr>Rule cache index: " << theCommands.RuleStackCacheIndex << "<br>Rule: " << currentPattern.ToString()
          << "<br>" << CGI::GetMathSpanPure(beforePatternMatch.ToString()) << " -> "
          << CGI::GetMathSpanPure(output.ToString());
        }
        break;
      }
    }
/////-------User-defined pattern matching end------
  }
  theCommands.EvaluatedExpressionsStack.RemoveLastObject();
  if (theCommands.flagLogFullTreeCrunching && theCommands.RecursionDeptH<3)
  { theCommands << "<br>";
    for (int i=0; i<theCommands.RecursionDeptH; i++)
      theCommands << "&nbsp&nbsp&nbsp&nbsp";
    theCommands << "to get: " << output.Lispify();
  }

  return true;
}

Expression* Calculator::PatternMatch
(const Expression& thePattern, Expression& theExpression, BoundVariablesSubstitution& bufferPairs, const Expression* condition, std::stringstream* theLog, bool logAttempts)
{ MacroRegisterFunctionWithName("Calculator::PatternMatch");
  RecursionDepthCounter recursionCounter(&this->RecursionDeptH);
  if (this->RecursionDeptH>=this->MaxRecursionDeptH)
  { std::stringstream out;
    out << "Error: while trying to evaluate expression, the maximum recursion depth of " << this->MaxRecursionDeptH << " was exceeded";
    theExpression.MakeError(out.str(), *this);
    return 0;
  }
//  if (theExpression.ToString()=="f{}((a))=a+5")
//  { stOutput << "!here";
//  }
  thePattern.CheckInitialization();
  theExpression.CheckInitialization();
  if (!this->ExpressionMatchesPattern(thePattern, theExpression, bufferPairs, theLog))
    return 0;
  if (theLog!=0 && logAttempts)
    (*theLog) << "<hr>found pattern: " << theExpression.ToString() << " -> " << thePattern.ToString() << " with " << bufferPairs.ToString();
  if (condition==0)
    return &theExpression;
  Expression tempExp=*condition;
  tempExp.CheckInitialization();
  if (theLog!=0 && logAttempts)
    (*theLog) << "<hr>Specializing condition pattern: " << tempExp.ToString();
  this->SpecializeBoundVars(tempExp, bufferPairs);
  tempExp.CheckInitialization();
  if (theLog!=0 && logAttempts)
    (*theLog) << "<hr>Specialized condition: " << tempExp.ToString() << "; evaluating...";
  BoundVariablesSubstitution tempPairs;
  Expression conditionResult;
  this->EvaluateExpression(*this, tempExp, conditionResult);
  if (theLog!=0 && logAttempts)
    (*theLog) << "<hr>The evaluated specialized condition: " << conditionResult.ToString() << "; evaluating...";
  if (conditionResult.IsEqualToOne())
    return &theExpression;
  return 0;
}

void Calculator::SpecializeBoundVars(Expression& toBeSubbedIn, BoundVariablesSubstitution& matchedPairs)
{ MacroRegisterFunctionWithName("Calculator::SpecializeBoundVars");
  RecursionDepthCounter recursionCounter(&this->RecursionDeptH);
  if (toBeSubbedIn.IsListOfTwoAtomsStartingWith(this->opBind()))
  { int indexMatching= matchedPairs.theBoundVariables.GetIndex(toBeSubbedIn);
    if (indexMatching!=-1)
    { toBeSubbedIn=matchedPairs.variableImages[indexMatching];
      //this->ExpressionHasBoundVars(toBeSubbed, RecursionDepth+1, MaxRecursionDepth);
      return;
    }
  }
  Expression subbedE;
  for (int i=0; i<toBeSubbedIn.children.size; i++)
  { subbedE=toBeSubbedIn[i];
    this->SpecializeBoundVars(subbedE, matchedPairs);
    toBeSubbedIn.SetChilD(i, subbedE);
  }
//  this->ExpressionHasBoundVars(toBeSubbed, RecursionDepth+1, MaxRecursionDepth);
}

bool Calculator::ProcessOneExpressionOnePatternOneSub
(const Expression& thePattern, Expression& theExpression, BoundVariablesSubstitution& bufferPairs, std::stringstream* theLog, bool logAttempts)
{ MacroRegisterFunctionWithName("Calculator::ProcessOneExpressionOnePatternOneSub");
  RecursionDepthCounter recursionCounter(&this->RecursionDeptH);
  if (!thePattern.StartsWith(this->opDefine(), 3) && !thePattern.StartsWith(this->opDefineConditional(), 4))
    return false;
  if (theLog!=0 && logAttempts)
  { (*theLog) << "<hr>attempting to reduce expression " << theExpression.ToString();
    (*theLog) << " by pattern " << thePattern.ToString();
  }
  theExpression.CheckInitialization();
  const Expression& currentPattern=thePattern[1];
  const Expression* theCondition=0;
  bool isConditionalDefine=
  thePattern.StartsWith(this->opDefineConditional(), 4);
  if (isConditionalDefine)
    theCondition=&thePattern[2];
  Expression* toBeSubbed=this->PatternMatch
  (currentPattern, theExpression, bufferPairs, theCondition, theLog, logAttempts);
  if (toBeSubbed==0)
    return false;
  if (theLog!=0 && logAttempts)
    *theLog << "<br><b>found a match!</b>";
  if (isConditionalDefine)
    *toBeSubbed=thePattern[3];
  else
    *toBeSubbed=thePattern[2];
  this->SpecializeBoundVars(*toBeSubbed, bufferPairs);
  return true;
}

bool Calculator::ParseAndExtractExpressions
(const std::string& theInputString, Expression& outputExp, List<SyntacticElement>& outputSynSoup, List<SyntacticElement>& outputSynStack, std::string* outputSynErrors)
{ MacroRegisterFunctionWithName("Calculator::ParseAndExtractExpressions");
  this->CurrentSyntacticStacK=&outputSynStack;
  this->CurrrentSyntacticSouP=&outputSynSoup;
  this->ParseFillDictionary(theInputString);
  bool result=this->ExtractExpressions(outputExp, outputSynErrors);
  this->CurrentSyntacticStacK=&this->syntacticStacK;
  this->CurrrentSyntacticSouP=&this->syntacticSouP;
  return result;
}

void Calculator::Evaluate(const std::string& theInput)
{ MacroRegisterFunctionWithName("Calculator::Evaluate");
  if (this->theGlobalVariableS==0)
  { this->outputString= "This is a programming error: commandList not initialized properly. Please report this bug. ";
    return;
  }
  this->StartTimeEvaluationInSecondS=this->theGlobalVariableS->GetElapsedSeconds();
  this->inputString=theInput;
  this->ParseAndExtractExpressions(theInput, this->theProgramExpression, this->syntacticSouP, this->syntacticStacK, & this->syntaxErrors);
  this->EvaluateCommands();
}

void Calculator::EvaluateCommands()
{ MacroRegisterFunctionWithName("Calculator::EvaluateCommands");
  std::stringstream out;

//  this->theLogs.resize(this->theCommands.size());
//this->ToString();
  //std::stringstream comments;
  if (this->syntaxErrors!="")
  { out << "<hr><b>Syntax errors encountered</b><br>";
    out << this->syntaxErrors;
    out << "<hr>";
  }
//  stOutput
//  << "Starting expression: " << this->theProgramExpression.ToString()
//  << "<hr>";
  Expression StartingExpression=this->theProgramExpression;
  this->flagAbortComputationASAP=false;
  this->Comments.clear();
  ProgressReport theReport(theGlobalVariableS);
  theReport.Report("Evaluating expressions, current expression stack:");
  this->EvaluateExpression(*this, this->theProgramExpression, this->theProgramExpression);
  if (this->RecursionDeptH!=0)
    crash << "This is a programming error: the starting recursion depth before evaluation was 0, but after evaluation it is "
    << this->RecursionDeptH << "." << crash;
  this->theGlobalVariableS->theDefaultFormat.flagMakingExpressionTableWithLatex=true;
  this->theGlobalVariableS->theDefaultFormat.flagUseLatex=true;
  this->theGlobalVariableS->theDefaultFormat.flagExpressionIsFinal=true;
  this->theGlobalVariableS->theDefaultFormat.flagExpressionNewLineAllowed=true;
  if (!this->flagDisplayFullExpressionTree)
    out << this->theProgramExpression.ToString(&this->theGlobalVariableS->theDefaultFormat, &StartingExpression);
  else
    out << "<hr>Input:<br> " << StartingExpression.ToStringFull() << "<hr>"
    << "Output:<br>" << this->theProgramExpression.ToStringFull();
  this->outputString=out.str();
  std::stringstream commentsStream;
  if (this->theObjectContainer.theAlgebraicClosure.theBasisMultiplicative.size>1)
    commentsStream << "<b>Algebraic number closure status. </b><br>" << this->theObjectContainer.theAlgebraicClosure.ToString() << "<hr>";
  if (this->Comments.str()!="")
    commentsStream << "<b>Comments.</b><br><span>" << this->Comments.str() << "</span>";
  this->outputCommentsString=commentsStream.str();
}
