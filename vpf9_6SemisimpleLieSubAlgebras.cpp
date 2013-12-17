//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#include "vpfHeader2Math1_3SemisimpleLieSubalgebras.h"
#include "vpfHeader2Math5_SubsetsSelections.h"
#include "vpfImplementationHeader2Math052_PolynomialComputations_Advanced.h"
#include "vpfImplementationHeader2Math1_SemisimpleLieAlgebras.h"
#include "vpfImplementationHeader2Math3_FiniteGroups.h"
ProjectInformationInstance ProjectInfoVpfSemisimpleSubalgebras(__FILE__, "Semisimple subalgebras of the semisimple Lie algebras. ");

template<>
typename List<DynkinSimpleType>::OrderLeftGreaterThanRight
FormatExpressions::GetMonOrder<DynkinSimpleType>()
{ return 0;
}

template<class coefficient>
void SemisimpleLieAlgebra::GenerateLieSubalgebra(List<ElementSemisimpleLieAlgebra<coefficient> >& inputOutputGenerators)
{ ElementSemisimpleLieAlgebra<coefficient> theBracket;
  HashedList<ChevalleyGenerator> seedMons;
  for (int i=0; i<inputOutputGenerators.size; i++)
    for (int j=0; j<inputOutputGenerators.size; j++)
    { this->LieBracket(inputOutputGenerators[i], inputOutputGenerators[j], theBracket);
      inputOutputGenerators.AddOnTop(theBracket);
      if (theBracket.GetRankOfSpanOfElements(inputOutputGenerators, &seedMons)<inputOutputGenerators.size)
        inputOutputGenerators.RemoveLastObject();
    }
}

bool SemisimpleLieAlgebra::CheckConsistency()const
{ if (this->flagDeallocated)
    crash << "This is a programming error: use after free of SemisimpleLieAlgebra. " << crash;
  this->theWeyl.CheckConsistency();
  return true;
}

bool SemisimpleLieAlgebra::AttempTFindingHEF
(ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> >& inputOutputH, ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> >& inputOutputE,
 ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> >& inputOutputF, std::stringstream* logStream, GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("SemisimpleLieAlgebra::AttemptFindingHEF");
  List<Polynomial<AlgebraicNumber> > theSystem;
  GroebnerBasisComputation<AlgebraicNumber> theComputation;
  ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> > mustBeZero, tempE;
  this->LieBracket(inputOutputH, inputOutputE, mustBeZero);
  mustBeZero-=inputOutputE*2;
  for (int i=0; i<mustBeZero.size(); i++)
    theSystem.AddOnTop(mustBeZero.theCoeffs[i]);
  this->LieBracket(inputOutputH, inputOutputF, mustBeZero);
  mustBeZero+=inputOutputF*2;
  for (int i=0; i<mustBeZero.size(); i++)
    theSystem.AddOnTop(mustBeZero.theCoeffs[i]);
  this->LieBracket(inputOutputE, inputOutputF, mustBeZero);
  mustBeZero-=inputOutputH;
  for (int i=0; i<mustBeZero.size(); i++)
    theSystem.AddOnTop(mustBeZero.theCoeffs[i]);
  if(logStream!=0)
  { *logStream << "The system to solve: ";
    for (int i=0; i<theSystem.size; i++)
      *logStream << "<br>" << theSystem[i].ToString() << " = 0 ";
  }
  theComputation.MaxNumSerreSystemComputations=4001;
  theComputation.MaxNumGBComputations=2001;
  theComputation.SolveSerreLikeSystem(theSystem, 0, theGlobalVariables);
  if (!theComputation.flagSystemSolvedOverBaseField)
  { if (logStream!=0)
    { if (theComputation.flagSystemProvenToHaveNoSolution)
        *logStream << "<br><b>System proven to have no solution. </b>";
      if (theComputation.flagSystemProvenToHaveSolution)
        *logStream << "<br><b>System proven to have solution, but I failed to find it.</b> Either the solution lies outside of the base field, or my heuristics are not good enough.";
    }
    return false;
  }
  if (logStream!=0)
    *logStream << "Solved successfully! One solution: " << theComputation.ToStringSerreLikeSolution();
  PolynomialSubstitution<AlgebraicNumber> theSolutionSub;
  theComputation.GetSubFromPartialSolutionSerreLikeSystem(theSolutionSub);
  inputOutputF.SubstitutionCoefficients(theSolutionSub);
  inputOutputH.SubstitutionCoefficients(theSolutionSub);
  inputOutputE.SubstitutionCoefficients(theSolutionSub);
  if (logStream!=0)
    *logStream << "<br>H= " << inputOutputH.ToString() << "<br>E=" << inputOutputE.ToString() << "<br>F=" << inputOutputF.ToString();
  return true;
}

bool SemisimpleLieAlgebra::AttemptExtendingEtoHEFwithHinCartan
(ElementSemisimpleLieAlgebra<AlgebraicNumber>& theE, ElementSemisimpleLieAlgebra<AlgebraicNumber>& outputH, ElementSemisimpleLieAlgebra<AlgebraicNumber>& outputF,
 std::stringstream* logStream, GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("SemisimpleLieAlgebra::AttemptExtendingEtoHEFwithHinCartan");
  Matrix<AlgebraicNumber> theM;
  this->GetAd(theM, theE);
  MatrixTensor<AlgebraicNumber> theMatTensor, theId;
  theMatTensor=theM;
  theId.MakeId(theM.NumRows);
  MathRoutines::RaiseToPower(theMatTensor, this->GetNumPosRoots(), theId);
  if (!theMatTensor.IsEqualToZero())
  { if (logStream!=0)
      *logStream << "The input E element " << theE.ToString() << " is not nilpotent. The matrix tensor is: " << theMatTensor.ToString() ;
    return false;
  }
  ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> > unknownH, unknownF, knownE;
  knownE=theE;
  this->GetGenericElementCartan(unknownH, 0);
  this->GetGenericElementNegativeBorelNilradical(unknownF, this->GetRank());
  bool success= this->AttempTFindingHEF(unknownH, knownE, unknownF, logStream, theGlobalVariables);
  if (!success)
    return false;
  outputH=unknownH;
  outputF=unknownF;
  return true;
}

void WeylGroup::operator+=(const WeylGroup& other)
{ if (this==&other)
  { WeylGroup tempW;
    tempW=*this;
    *this+=tempW;
    return;
  }
  DynkinType theType=this->theDynkinType;
  theType+=other.theDynkinType;
  this->MakeFromDynkinType(theType);
}

int SemisimpleSubalgebras::GetIndexFullSubalgebra()const
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::GetIndexFullSubalgebra");
  for (int i=0; i<this->theSubalgebraCandidates.size; i++)
    if (this->owneR->theWeyl.theDynkinType==this->theSubalgebraCandidates[i].theWeylNonEmbeddeD.theDynkinType)
      return i;
  return -1;
}

int SemisimpleSubalgebras::GetDisplayIndexFromActual(int ActualIndexSubalgebra)const
{ int result=0;
  for (int i=0; i<=ActualIndexSubalgebra; i++)
    if (!this->theSubalgebraCandidates[i].flagSystemProvedToHaveNoSolution)
      result++;
  return result;
}

std::string SemisimpleSubalgebras::GetPhysicalFileNameSubalgebra(int ActualIndexSubalgebra, FormatExpressions* theFormat)const
{ std::stringstream out;
  out << (theFormat==0 ? "./" : theFormat->PathPhysicalOutputFolder);
  out << CGI::CleanUpForFileNameUse(this->owneR->theWeyl.theDynkinType.ToString()) << "_subalgebra_" << this->GetDisplayIndexFromActual(ActualIndexSubalgebra) << ".html";
  return out.str();
}

std::string SemisimpleSubalgebras::GetPhysicalFileNameFKFTNilradicals(int ActualIndexSubalgebra, FormatExpressions* theFormat)const
{ std::stringstream out;
  out << (theFormat==0 ? "./" : theFormat->PathPhysicalOutputFolder);
  out << CGI::CleanUpForFileNameUse(this->owneR->theWeyl.theDynkinType.ToString()) << "_subalgebra_" << this->GetDisplayIndexFromActual(ActualIndexSubalgebra) << "_FKFTnilradicals.html";
  return out.str();
}

std::string SemisimpleSubalgebras::GetDisplayFileNameSubalgebraAbsolute(int ActualIndexSubalgebra, FormatExpressions* theFormat)const
{ std::stringstream out;
  out << (theFormat==0 ? "./" : theFormat->PathDisplayOutputFolder);
  out << CGI::CleanUpForFileNameUse(this->owneR->theWeyl.theDynkinType.ToString()) << "_subalgebra_" << this->GetDisplayIndexFromActual(ActualIndexSubalgebra) << ".html";
  return out.str();
}

std::string SemisimpleSubalgebras::GetDisplayFileNameSubalgebraRelative(int ActualIndexSubalgebra, FormatExpressions* theFormat)const
{ std::stringstream out;
  out << "./";
  out << CGI::CleanUpForFileNameUse(this->owneR->theWeyl.theDynkinType.ToString()) << "_subalgebra_" << this->GetDisplayIndexFromActual(ActualIndexSubalgebra) << ".html";
  return out.str();
}

std::string SemisimpleSubalgebras::GetDisplayFileNameFKFTNilradicals(int ActualIndexSubalgebra, FormatExpressions* theFormat)const
{ std::stringstream out;
  out << (theFormat==0 ? "./" : theFormat->PathDisplayOutputFolder);
  out << CGI::CleanUpForFileNameUse(this->owneR->theWeyl.theDynkinType.ToString()) << "_subalgebra_" << this->GetDisplayIndexFromActual(ActualIndexSubalgebra) << "_FKFTnilradicals.html";
  return out.str();
}

std::string DynkinType::ToString(FormatExpressions* theFormat)const
{ //std::cout << "d.t. tostring called";

  return this->::MonomialCollection<DynkinSimpleType, Rational>::ToString(theFormat);
}

std::string DynkinType::ToStringRelativeToAmbientType(const DynkinSimpleType& ambientType, FormatExpressions* theFormat)const
{ FormatExpressions tempFormat;
  tempFormat.AmbientWeylLetter=ambientType.theLetter;
  tempFormat.AmbientCartanSymmetricInverseScale=ambientType.CartanSymmetricInverseScale;
  return this->ToString(&tempFormat);
}

std::string SemisimpleSubalgebras::ToStringSSsumaryHTML(FormatExpressions* theFormat)const
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::ToStringSSsumaryHTML");
  if (!this->flagComputeNilradicals)
    return "";
  std::stringstream out;
  int numIsotypicallyCompleteNilrads=0;
  int numFailingConeCondition=0;
  int numNoLinfRelFound=0;
  int numNonCentralizerConditionWithConeCondition=0;
  int numBadParabolics=0;

  for (int i=0; i<this->theSubalgebraCandidates.size; i++)
  { numIsotypicallyCompleteNilrads+=this->theSubalgebraCandidates[i].FKNilradicalCandidates.size;
    numFailingConeCondition+=this->theSubalgebraCandidates[i].NumConeIntersections;
    numNoLinfRelFound+=this->theSubalgebraCandidates[i].NumCasesNoLinfiniteRelationFound;
    numNonCentralizerConditionWithConeCondition+=this->theSubalgebraCandidates[i].NumCentralizerConditionFailsConeConditionHolds;
    numBadParabolics+=this->theSubalgebraCandidates[i].NumBadParabolics;
  }
  out << "<br>There are " << numIsotypicallyCompleteNilrads << " possible isotypic nilradical extensions of the primal subalgebras. Of them "
  << numFailingConeCondition << " have intersecting cones. Of the remaining " << numIsotypicallyCompleteNilrads-numFailingConeCondition
  << " nilradical extensions with non-intersecting cones, " << numIsotypicallyCompleteNilrads-numFailingConeCondition- numNonCentralizerConditionWithConeCondition
  << " satisfy(ies) the centralizer condition and " << numNonCentralizerConditionWithConeCondition << " fail(s) the centralizer condition.";
  if (numBadParabolics>0)
    out << "<br><span style=\"color:#FF0000\">Of the subalgebras satisfying the centralizer condition there are  " << numBadParabolics
    << " pabolic subalgebra(s) that do not extend to parabolic subalgebra(s) of the ambient Lie algebra with Levi types A and C. "
    << " For these subalgebras the PSZ construction is not proven to hold. </span>";
  else
    out << "<br><span style=\"color:#0000FF\"> In each of " << numIsotypicallyCompleteNilrads-numFailingConeCondition- numNonCentralizerConditionWithConeCondition
    << " case(s) when the centralizer condition holds, the parabolic subalgebra in the centralizer with Levi types A and C extends "
    << "to parabolic subalgebra of the ambient Lie algebra whose Levi types are A and C only. </span>";
  if (numFailingConeCondition>0)
  { if (numNoLinfRelFound>0)
      out << "<br><span style=\"color:#FF0000\">In " << numNoLinfRelFound << " cases no L-infinite relation was found. </span>";
    else
      out << "<br><span style=\"color:#0000FF\"> In each of " << numFailingConeCondition << " case(s) of intersecting cones, an L-infinite relation was found. </span>";
  }

  return out.str();
}

std::string SemisimpleSubalgebras::ToStringSSsumaryLaTeX(FormatExpressions* theFormat)const
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::ToStringSSsumaryLaTeX");
  if (!this->flagComputeNilradicals)
    return "";
  std::stringstream out;
  int numIsotypicallyCompleteNilrads=0;
  int numFailingConeCondition=0;
  int numNoLinfRelFound=0;
  int numNonCentralizerConditionWithConeCondition=0;
  int numBadParabolics=0;

  for (int i=0; i<this->theSubalgebraCandidates.size; i++)
  { numIsotypicallyCompleteNilrads+=this->theSubalgebraCandidates[i].FKNilradicalCandidates.size;
    numFailingConeCondition+=this->theSubalgebraCandidates[i].NumConeIntersections;
    numNoLinfRelFound+=this->theSubalgebraCandidates[i].NumCasesNoLinfiniteRelationFound;
    numNonCentralizerConditionWithConeCondition+=this->theSubalgebraCandidates[i].NumCentralizerConditionFailsConeConditionHolds;
    numBadParabolics+=this->theSubalgebraCandidates[i].NumBadParabolics;
  }
  if (numBadParabolics>0)
    out << "<br><span style=\"color:#FF0000\">There are " << numBadParabolics << " bad parabolic subalgebras!</span><br>";
  out << "\n<br>\n\\begin{longtable}{ccp{3cm}p{3cm}cc}";
  out << "\\caption{Semisimple subalgebras in type $" << this->owneR->theWeyl.theDynkinType.ToStringRelativeToAmbientType(this->owneR->theWeyl.theDynkinType[0], theFormat)
  << "$ \\label{tableSSSubalgerbas" << this->owneR->theWeyl.theDynkinType.ToStringRelativeToAmbientType(this->owneR->theWeyl.theDynkinType[0], theFormat) << "}. ";
  out << "Number of isotypically complete nilradicals: " << numIsotypicallyCompleteNilrads << ", of them "
  << numFailingConeCondition << " fail the cone condition.";
  if (numNoLinfRelFound==0)
    out << "<br>In all " << numFailingConeCondition << " cases, an $\\mathfrak{l}$-infinite relation was found. ";
  else
    out << "<br>In " << numNoLinfRelFound << " cases, no L-infinite relation was found. <br>";
  out << "}\\\\\n<br>\n";
  out << "Type $\\mathfrak s$ & Centralizer &Decomp. $\\mathfrak g$ over $\\mathfrak s$ &Decomp. $\\mathfrak g$ over $\\mathfrak s\\oplus \\mathfrak h_c$"
  << "&\\#$\\mathfrak n_\\mathfrak l$& \\# cone failures\\\\\\hline\n<br>\n";
  DynkinType typeCentralizer;
  FormatExpressions tempCharFormat;
  for (int i=0; i<this->theSubalgebraCandidates.size; i++)
  { const CandidateSSSubalgebra& currentSA=this->theSubalgebraCandidates[i];
    if (currentSA.flagSystemProvedToHaveNoSolution)
      continue;
    out << "$" << currentSA.theWeylNonEmbeddeD.theDynkinType.ToStringRelativeToAmbientType(this->owneR->theWeyl.theDynkinType[0], theFormat) << "$";
    if (!currentSA.flagCentralizerIsWellChosen)
      continue;
    typeCentralizer.MakeZero();
    if (currentSA.indexMaxSSContainer!=-1)
      typeCentralizer=this->theSubalgebraCandidates[currentSA.indexMaxSSContainer].theWeylNonEmbeddeD.theDynkinType - currentSA.theWeylNonEmbeddeD.theDynkinType;
    out << "& $ ";
    if(!typeCentralizer.IsEqualToZero())
    { out << typeCentralizer.ToString();
      if (currentSA.centralizerRank!=typeCentralizer.GetRank())
        out << "\\oplus";
    }
    if (currentSA.centralizerRank!=typeCentralizer.GetRank())
      out << "\\mathfrak h_{" << (currentSA.centralizerRank-typeCentralizer.GetRank()).ToString() << "}";
    out << "$";
    if (!currentSA.charFormaT.IsZeroPointer())
      tempCharFormat=currentSA.charFormaT.GetElementConst();
    out << "&$" << currentSA.theCharNonPrimalFundCoords.ToString(&tempCharFormat) << "$ ";
    out << "&$" << currentSA.thePrimalChaR.ToString(&tempCharFormat) << "$ ";
    out << "& " << currentSA.FKNilradicalCandidates.size << "&" << currentSA.NumConeIntersections;
    out << "\\\\ \\hline \n<br>\n";
  }
  out << "\\end{longtable}\n<br>\n";
  out << "\\end{document}";
  return out.str();
}

std::string SemisimpleSubalgebras::ToString(FormatExpressions* theFormat)
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::ToString");
  CGI::GlobalMathSpanID=0;
  bool writingToHD=theFormat==0? false: theFormat->flagUseHtmlAndStoreToHD;
  std::stringstream out;
  int candidatesRealized=0;
  int candidatesNotRealizedNotProvenImpossible=0;
  int candidatesProvenImpossible=0;
  for (int i=0; i<this->theSubalgebraCandidates.size; i++)
  { CandidateSSSubalgebra& currentSA=this->theSubalgebraCandidates[i];
    if (currentSA.flagSystemProvedToHaveNoSolution)
      candidatesProvenImpossible++;
    if (currentSA.flagSystemSolved)
      candidatesRealized++;
  }
  out << "<b>Please note that the code generating the tables is experimental. While the code performs a number of self-test routines, "
  << " it is possible that there still exist programming bugs. We will remove this message as soon as we are confident in the accuracy of all tables.  "
  << " If you see any errors in the tables, we would be very grateful if you email us with a simple explanation of the issue!</b><br>";
  candidatesNotRealizedNotProvenImpossible=  this->theSubalgebraCandidates.size-candidatesRealized- candidatesProvenImpossible;
  if (!writingToHD)
  { out << candidatesRealized << " subalgebras realized.";
    out << "<br>Up to linear equivalence, there are " << this->theSubalgebraCandidates.size << " = " << candidatesRealized << " realized + "
    << candidatesProvenImpossible << " proven impossible + " << candidatesNotRealizedNotProvenImpossible << " neither realized nor proven impossible. \n<hr>\n ";
  } else
  { out << "Up to linear equivalence, there are total " << candidatesRealized << " semisimple subalgebras";
    if (candidatesNotRealizedNotProvenImpossible!=0)
      out << " and "  << candidatesNotRealizedNotProvenImpossible << " semisimple subalgebra candidate(s) which were not realized (but not proven impossible)";
    out << ". ";
  }
  out << "The subalgebras are ordered by rank, Dynkin indices of simple constituents and dimensions of simple constituents. "
  << "The upper index indicates the Dynkin index, the lower index indicates the rank of the subalgebra. ";
  bool showTime= theFormat==0 ? true : theFormat->flagIncludeMutableInformation;
  if (showTime)
    if (this->timeComputationStartInSeconds!=-1 && this->timeComputationEndInSeconds!=-1)
      out << "<br>Computation time in seconds: " << this->timeComputationEndInSeconds - this->timeComputationStartInSeconds << ".";
  if (this->numAdditions!=-1)
    out << "<br>" << this->numAdditions+this->numMultiplications << " total arithmetic operations performed = " << this->numAdditions << " additions and "
    << this->numMultiplications << " multiplications. ";
  out << "<hr>";
  out << "The base field over which the subalgebras were realized is: ";
  if (this->ownerField==0)
    out << CGI::GetMathSpanPure("\\mathbb Q");
  else
    out << this->ownerField->ToString();
  out << "<hr> ";
  int numRegularSAs=0;
  int numSl2s=0;
  for (int i=0; i<this->theSubalgebraCandidates.size; i++)
  { if (this->theSubalgebraCandidates[i].AmRegularSA())
      numRegularSAs++;
    if (this->theSubalgebraCandidates[i].theWeylNonEmbeddeD.theDynkinType.IsTypeA_1())
      numSl2s++;
  }
  out << "Number of root subalgebras other than the Cartan and full subalgebra: " << numRegularSAs-1;
  out << "<br>Number of sl(2)'s: " << numSl2s << "<hr>";
  std::string summaryString= this->ToStringSSsumaryHTML(theFormat);
  if (summaryString!="")
  { out << "Summary:" << summaryString << "<hr>";
    if (this->flagProduceLaTeXtables)
      out << "Summary in LaTeX<br><br>" << this->ToStringSSsumaryLaTeX(theFormat) << "<br><br><hr>";
  }
  if (!writingToHD)
  { for (int i=0; i<this->theSubalgebraCandidates.size; i++)
    { if (!this->theSubalgebraCandidates[i].flagSystemProvedToHaveNoSolution)
        out << "Subalgebra number " << this->GetDisplayIndexFromActual(i) << ".<br>";
      out << this->theSubalgebraCandidates[i].ToString(theFormat) << "\n<hr>\n ";
    }
  }
  else
  { if (theFormat!=0)
    { theFormat->flagCandidateSubalgebraShortReportOnly=true;
      theFormat->flagUseHtmlAndStoreToHD=true;
    }
    for (int i=0; i<this->theSubalgebraCandidates.size; i++)
      if (!this->theSubalgebraCandidates[i].flagSystemProvedToHaveNoSolution)
      { if (!this->theSubalgebraCandidates[i].flagSystemProvedToHaveNoSolution)
          out << "Subalgebra number " << this->GetDisplayIndexFromActual(i) << ".<br>";
        out << this->theSubalgebraCandidates[i].ToString(theFormat) << "\n<hr>\n ";
      }
    FormatExpressions theFormatCopy;
    if (theFormat!=0)
      theFormatCopy=*theFormat;
    theFormatCopy.flagUseMathSpanPureVsMouseHover=true;
    theFormatCopy.flagCandidateSubalgebraShortReportOnly=false;
    for (int i=0; i<this->theSubalgebraCandidates.size; i++)
      if (!this->theSubalgebraCandidates[i].flagSystemProvedToHaveNoSolution)
      { std::fstream outputFileSubalgebra;
        if (!XML::OpenFileCreateIfNotPresent(outputFileSubalgebra, this->GetPhysicalFileNameSubalgebra(i, &theFormatCopy), false, true, false))
        { crash << "<br>This may or may not be a programming error. While processing subalgebra of actual index " << i << " and display index "
          << this->GetDisplayIndexFromActual(i) << ", I requested to create file " << this->GetPhysicalFileNameSubalgebra(i, &theFormatCopy)
          << " for output. However, the file failed to create. Possible explanations: 1. Programming error. "
          << "2. The calculator has no write permission to the folder in which the file is located. "
          << "3. The folder does not exist for some reason lying outside of the calculator. " << crash;
        }
        outputFileSubalgebra << "<html>" << "<script src=\"../../jsmath/easy/load.js\"></script>\n<body>Subalgebra number "
        << this->GetDisplayIndexFromActual(i) << ".<br>" << this->theSubalgebraCandidates[i].ToString(&theFormatCopy);
        if (this->flagComputeNilradicals)
        { std::fstream outputFileFKFTnilradicals;
          if (!XML::OpenFileCreateIfNotPresent(outputFileFKFTnilradicals, this->GetPhysicalFileNameFKFTNilradicals(i, &theFormatCopy), false, true, false))
          { crash << "<br>This may or may not be a programming error. While processing subalgebra of actual index " << i
            << " and display index " << this->GetDisplayIndexFromActual(i) << ", I requested to create file "
            << this->GetPhysicalFileNameFKFTNilradicals(i, &theFormatCopy) << " for output. However, the file failed to create. "
            << " Possible explanations: 1. Programming error. 2. The calculator has no write permission to the"
            << " folder in which the file is located. 3. The folder does not exist for some reason lying outside of the calculator. " << crash;
          }
          outputFileFKFTnilradicals << "<html>" << "<script src=\"" << theFormatCopy.PathDisplayServerBaseFolder << "jsmath/easy/load.js\"></script><body>"
          << this->ToStringAlgebraLink(i, &theFormatCopy) << this->theSubalgebraCandidates[i].ToStringNilradicals(&theFormatCopy) << "\n</body></html>";
        }
        outputFileSubalgebra << "\n</body></html>\n ";
      }
  }
  if (this->theSl2s.size!=0)
  { int numComputedOrbits=0;
    for (int i=0; i<this->theOrbitsAreComputed.size; i++)
      if (this->theOrbitsAreComputed[i])
        numComputedOrbits++;
    out << "<hr>Of the " << this->theOrbits.size << " h element conjugacy classes " << numComputedOrbits
    << " had their Weyl group orbits computed. The h elements and their computed orbit sizes follow. ";
    out << "<table><tr><td>h element</td><td>orbit size</td></tr>";
    for (int i=0; i<this->theOrbitGeneratingElts.size; i++)
    { out << "<tr><td>" << this->theSl2s[i].theH.GetCartanPart().ToString() << "</td>";
      if (this->theOrbitsAreComputed[i])
        out << "<td>" << this->theOrbits[i].size << "</td>";
      else
        out << "<td>not computed</td>";
      out << "</tr>";
    }
    out << "</table>";
    out << this->theSl2s.ToString(theFormat);
  }
  return out.str();
}

void SemisimpleSubalgebras::GetCentralizerChains(List<List<int> >& outputChains)
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::GetCentralizerChains");
  outputChains.SetSize(0);
  Selection Explored;
  Explored.init(this->theSubalgebraCandidates.size);
  outputChains.ReservE(this->theSubalgebraCandidates.size);
  for (int i=0; i< this->theSubalgebraCandidates.size; i++)
    if (!Explored.selected[i])
    { outputChains.SetSize(outputChains.size+1);
      outputChains.LastObject()->SetSize(0);
      outputChains.LastObject()->AddOnTop(i);
      int maxSScontainer=this->theSubalgebraCandidates[i].indexMaxSSContainer;
      if (maxSScontainer!=-1)
      { outputChains.LastObject()->AddOnTop(maxSScontainer);
        Explored.AddSelectionAppendNewIndex(maxSScontainer);
      }
    }
}

void SemisimpleSubalgebras::ComputeSl2sInitOrbitsForComputationOnDemand()
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::ComputeSl2sInitOrbitsForComputationOnDemand");
  this->GetSSowner().FindSl2Subalgebras(this->GetSSowner(), this->theSl2s, *this->theGlobalVariables);
  this->theOrbits.SetSize(this->theSl2s.size);
  this->theOrbitGeneratingElts.SetSize(this->theSl2s.size);
  this->theOrbitsAreComputed.initFillInObject(this->theSl2s.size, false);
  this->theOrbitHelementLengths.Clear();
  this->theOrbitHelementLengths.SetExpectedSize(this->theSl2s.size);
  for (int i=0; i<this->theSl2s.size; i++)
    this->theOrbitHelementLengths.AddOnTop(this->theSl2s[i].LengthHsquared);
}

void SemisimpleSubalgebras::FindTheSSSubalgebras(SemisimpleLieAlgebra& newOwner, const DynkinType* targetType)
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::FindTheSSSubalgebras");
  this->owneR=&newOwner;
  this->ComputeSl2sInitOrbitsForComputationOnDemand();
  this->CheckConsistency();
  CandidateSSSubalgebra emptyCandidate;
  emptyCandidate.owner=this;
  this->ExtendCandidatesRecursive(emptyCandidate, targetType);
//  std::cout << "Num candidates so far: " << this->theSubalgebraCandidates.size;
  if (targetType!=0)
    this->flagAttemptToAdjustCentralizers=false;
  this->HookUpCentralizers(false);
}

bool SemisimpleSubalgebras::RanksAndIndicesFit(const DynkinType& input)const
{ if (input.GetRank()>this->owneR->GetRank())
    return false;
  for (int i=0; i<input.size(); i++)
    if (!this->theOrbitHelementLengths.Contains(input[i].CartanSymmetricInverseScale*2))
      return false;
  return true;
}

bool SemisimpleSubalgebras::GrowDynkinType(const DynkinType& input, List<DynkinType>& output, List<List<int> >* outputImagesSimpleRoots)const
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::GrowDynkinType");
  HashedList<Rational> theLengths;
  theLengths.SetExpectedSize(this->theOrbitHelementLengths.size);
  for (int i=0; i<this->theOrbitHelementLengths.size; i++)
    theLengths.AddOnTopNoRepetition(this->theOrbitHelementLengths[i]/2);
  return input.Grow(theLengths, this->owneR->GetRank(), output, outputImagesSimpleRoots);
}

Vector<Rational> SemisimpleSubalgebras::GetHighestWeightFundNewComponentFromImagesOldSimpleRootsAndNewRoot
(const DynkinType& input, const List<int>& imagesOldSimpleRootsAndNewRoot, CandidateSSSubalgebra& theSSSubalgebraToBeModified)
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::GetHighestWeightFundNewComponentFromImagesOldSimpleRootsAndNewRoot");
  Vector<Rational> result;
  if (input.IsEqualToZero())
  { result.MakeZero(0);
    return result;
  }
  Vector<Rational> newSimpleRoot, highestRootInSimpleRootModuleSimpleCoords;
  theSSSubalgebraToBeModified.theWeylNonEmbeddeD.MakeFromDynkinType(input);
  theSSSubalgebraToBeModified.theWeylNonEmbeddeD.ComputeRho(true);
  int newIndex=*imagesOldSimpleRootsAndNewRoot.LastObject();
  int newRank=theSSSubalgebraToBeModified.theWeylNonEmbeddeD.GetDim();
  newSimpleRoot.MakeEi(newRank, newIndex);
  Vectors<Rational> simpleBasisOld;
  simpleBasisOld.SetSize(newRank-1);
  if (imagesOldSimpleRootsAndNewRoot.size!=newRank)
    crash << "This is a programming error: the root images must be " << newRank << " but there are " << imagesOldSimpleRootsAndNewRoot.size << " elements instead. "
    << "The type is " << input.ToString() << ". " << crash;
  for (int i=0; i<newRank-1; i++)
    simpleBasisOld[i].MakeEi(newRank, imagesOldSimpleRootsAndNewRoot[i]);
  if (input.ToString()=="A^{1}_2")
    std::cout << "<hr>type: " << input.ToString() << ", simplebasisold: " << simpleBasisOld.ToString() << "<br> new simple root: " << newSimpleRoot.ToString();
  theSSSubalgebraToBeModified.theWeylNonEmbeddeD.ComputeExtremeRootInTheSameKMod(simpleBasisOld, newSimpleRoot, highestRootInSimpleRootModuleSimpleCoords, true);
  result.SetSize(newRank-1);
  for (int i=0; i<simpleBasisOld.size; i++)
    result[i]=theSSSubalgebraToBeModified.theWeylNonEmbeddeD.RootScalarCartanRoot
    (highestRootInSimpleRootModuleSimpleCoords, simpleBasisOld[i])*2/
    theSSSubalgebraToBeModified.theWeylNonEmbeddeD.RootScalarCartanRoot(simpleBasisOld[i], simpleBasisOld[i]);
  return result;
}

void CandidateSSSubalgebra::SetUpInjectionHs
(const CandidateSSSubalgebra& baseSubalgebra, const DynkinType& theNewType, const List<int>& theRootInjection, Vector<Rational>* newHScaledToActByTwo)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::SetUpInjectionHs");
  this->reset(baseSubalgebra.owner);
  this->theWeylNonEmbeddeD.MakeFromDynkinType(theNewType);
  this->theWeylNonEmbeddeDdefaultScale.MakeFromDynkinTypeDefaultLengthKeepComponentOrder(theNewType);
  this->theWeylNonEmbeddeDdefaultScale.ComputeRho(true);
  this->theHsScaledToActByTwoInOrderOfCreation.ReservE(baseSubalgebra.theHsScaledToActByTwoInOrderOfCreation.size+1);
  this->theHsScaledToActByTwoInOrderOfCreation=baseSubalgebra.theHsScaledToActByTwoInOrderOfCreation;
  if (newHScaledToActByTwo!=0)
    this->theHsScaledToActByTwoInOrderOfCreation.AddOnTop(*newHScaledToActByTwo);
  DynkinSimpleType newComponent=theNewType.GetSmallestSimpleType();
  this->CartanSAsByComponent=baseSubalgebra.CartanSAsByComponent;
  int indexOffset=theNewType.GetRank()-newComponent.theRank;
  int newIndexInNewComponent=0;
  if (!newComponent.IsEqualToZero())
    newIndexInNewComponent=*theRootInjection.LastObject()-indexOffset;

  Vector<Rational> NewH;
  if (newHScaledToActByTwo!=0)
  { NewH= *newHScaledToActByTwo;
    NewH*= newComponent.GetDefaultRootLengthSquared(newIndexInNewComponent)/2;
  }
  if (newComponent.theRank==1)
  { this->CartanSAsByComponent.SetSize(this->CartanSAsByComponent.size+1);
    this->CartanSAsByComponent.LastObject()->SetSize(1);
    if (newHScaledToActByTwo!=0)
      (*this->CartanSAsByComponent.LastObject())[0]=NewH;
    else
      (*this->CartanSAsByComponent.LastObject())[0].SetSize(0);
  } else
  { Vectors<Rational>& oldComponentHs=*baseSubalgebra.CartanSAsByComponent.LastObject();
    Vectors<Rational>& currentHs=*this->CartanSAsByComponent.LastObject();
    currentHs.SetSize(currentHs.size+1);
    for (int i=0; i<newComponent.theRank-1; i++)
      currentHs[theRootInjection[indexOffset+i]-indexOffset]=oldComponentHs[i];
    if (newHScaledToActByTwo!=0)
      currentHs[newIndexInNewComponent]=NewH;
    else
      currentHs[newIndexInNewComponent].SetSize(0);
  }
  this->theHs.AssignListList(this->CartanSAsByComponent);
  this->theHsScaledToActByTwo.SetSize(this->theHs.size);
  int counter=-1;
  List<DynkinSimpleType> theTypes;
  this->theWeylNonEmbeddeD.theDynkinType.GetTypesWithMults(theTypes);
  for (int i=0; i<this->CartanSAsByComponent.size; i++)
    for (int j=0; j< this->CartanSAsByComponent[i].size; j++)
    { counter++;
      this->theHsScaledToActByTwo[counter]=(this->theHs[counter]/theTypes[i].GetDefaultRootLengthSquared(j))*2;
    }

  this->theHs.AssignListList(this->CartanSAsByComponent);
}

bool CandidateSSSubalgebra::CreateAndAddByExtendingBaseSubalgebra
(const CandidateSSSubalgebra& baseSubalgebra, Vector<Rational>& newHrescaledToActByTwo, const DynkinType& theNewType, const List<int>& theRootInjection)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::CreateAndAddByExtendingBaseSubalgebra");
  this->SetUpInjectionHs(baseSubalgebra, theNewType, theRootInjection, &newHrescaledToActByTwo);
  this->owner->RegisterPossibleCandidate(*this);
  if (!baseSubalgebra.theWeylNonEmbeddeD.theDynkinType.IsEqualToZero() && baseSubalgebra.indexInOwner==-1)
    crash << "This is a programming error: attempting to induce a subalgebra from a non-registered base subalgebra. " << crash;
  //set up induction history:
//  std::cout << "<hr><b>Testing actual extension!!! Type base: "
//  << baseSubalgebra.theWeylNonEmbeddeD.theDynkinType.ToString()
//  << ", type target: " << this->theWeylNonEmbeddeD.theDynkinType.ToString() << "</b><br>";
  this->indexIamInducedFrom=baseSubalgebra.indexInOwner;
  this->RootInjectionsFromInducer=theRootInjection;
  //induction history is complete.
  ProgressReport theReport(this->owner->theGlobalVariables);
  if (!this->ComputeChar(false))
  { if (this->owner->theGlobalVariables!=0)
      theReport.Report("Candidate " + this->theWeylNonEmbeddeD.theDynkinType.ToStringRelativeToAmbientType(this->GetAmbientWeyl().theDynkinType[0]) + " doesn't have fitting chars.");
    //std::cout << this->theWeylNonEmbeddeD.theDynkinType.ToString() << " - bad character.";
    return false;
  }
  if (!this->ComputeSystem(false,false))
  { if (this->owner->theGlobalVariables!=0)
      theReport.Report("Candidate " + this->theWeylNonEmbeddeD.theDynkinType.ToStringRelativeToAmbientType(this->GetAmbientWeyl().theDynkinType[0]) + " -> no system solution.");
    //std::cout << this->theWeylNonEmbeddeD.theDynkinType.ToString() << " - couldnt solve system.";
    return false;
  }
  for (int i=0; i<this->owner->theSubalgebraCandidates.size; i++)
    if (this->theWeylNonEmbeddeD.theDynkinType==this->owner->theSubalgebraCandidates[i].theWeylNonEmbeddeD.theDynkinType)
    { if (this->IsDirectSummandOf(this->owner->theSubalgebraCandidates[i], true))
      { //std::cout << this->theWeylNonEmbeddeD.theDynkinType.ToString() << " is equal to " << this->owner->theSubalgebraCandidates[i].theWeylNonEmbeddeD.theDynkinType.ToString();
        return false;
      } //else
        //std::cout << this->theWeylNonEmbeddeD.theDynkinType.ToString() << " is not a direct summand of "
        //<< this->owner->theSubalgebraCandidates[i].theWeylNonEmbeddeD.theDynkinType.ToString();
    } else
      if (this->theWeylNonEmbeddeD.theDynkinType.ToString()==this->owner->theSubalgebraCandidates[i].theWeylNonEmbeddeD.theDynkinType.ToString())
        crash << crash;
  this->indexInOwner=this->owner->theSubalgebraCandidates.size;
  this->owner->theSubalgebraCandidates.AddOnTop(*this);
  if (!this->owner->theSubalgebraCandidates.LastObject()->indexInOwner==this->owner->theSubalgebraCandidates.size-1)
    crash << "<hr>Something is very wrong: internal check failed! " << crash;
  return true;
}

void SemisimpleSubalgebras::ExtendCandidatesRecursive(const CandidateSSSubalgebra& baseCandidate, const DynkinType* targetType)
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::ExtendCandidatesRecursive");
  int baseRank=baseCandidate.theWeylNonEmbeddeD.GetDim();
  if (baseRank>=this->owneR->GetRank())
    return;
//  if (targetType!=0)
//  { std::cout << "<hr><b>I am aiming at extending " << baseCandidate.theWeylNonEmbeddeD.theDynkinType.ToString()
//    << " to " << targetType->ToString() << "</b>";
//  }
  if (targetType!=0)
    if (!baseCandidate.theWeylNonEmbeddeD.theDynkinType.CanBeExtendedParabolicallyTo(*targetType))
      return;
//  if (targetType!=0)
//  { std::cout << "<b>... continuing to actual computations!!!</b>";
//  }
  List<DynkinType> theLargerTypes;
  List<List<int> > theRootInjections;
  this->GrowDynkinType(baseCandidate.theWeylNonEmbeddeD.theDynkinType, theLargerTypes, &theRootInjections);
  std::cout << "<hr>Candidate extensions: " << theLargerTypes.ToString();
  CandidateSSSubalgebra newCandidate;
  newCandidate.owner=this;
  Vector<Rational> weightHElementWeAreLookingFor;
  ProgressReport theReport1(theGlobalVariables), theReport2(theGlobalVariables), theReport3(theGlobalVariables);
  if (theGlobalVariables!=0)
  { std::stringstream reportStream;
    reportStream << "So far " << this->theSubalgebraCandidates.size << " subalgebras found. Inducing from type "
    << baseCandidate.theWeylNonEmbeddeD.theDynkinType.ToString() << ". There are  " << theLargerTypes.size << " possible extensions total. ";
//    std::cout << "<hr>" << reportStream.str();
    theReport1.Report(reportStream.str());
  }
  List<int> indicesModulesNewComponentExtensionMod;
  indicesModulesNewComponentExtensionMod.ReservE(this->owneR->theWeyl.RootSystem.size);
  Vectors<Rational> startingVector;
  Vectors<Rational> theHCandidatesScaledToActByTwo;
  DynkinSimpleType theSmallType;
  Selection oldRoots;
  for (int i=0; i<theLargerTypes.size; i++)
  { if (theLargerTypes[i].GetRootSystemSize()>this->owneR->GetNumPosRoots()*2)
      continue;
    if (targetType!=0)
      if (!theLargerTypes[i].CanBeExtendedParabolicallyOrIsEqualTo(*targetType))
        if (theLargerTypes[i]!=*targetType)
        { //std::cout << "<br>" << theLargerTypes[i].ToString() << " cannot be extended to " << targetType->ToString();
          continue;
        }
    if (theGlobalVariables!=0)
    { std::stringstream reportStream;
      reportStream << " Exploring extension " << i+1 << " out of " << theLargerTypes.size << ". We are trying to extend "
      << baseCandidate.theWeylNonEmbeddeD.theDynkinType.ToString() << " to " << theLargerTypes[i].ToString() << ". ";
      std::cout << "<hr>" << reportStream.str();
      theReport2.Report(reportStream.str());
    }
    if (baseRank!=0)
    { weightHElementWeAreLookingFor=this->GetHighestWeightFundNewComponentFromImagesOldSimpleRootsAndNewRoot(theLargerTypes[i], theRootInjections[i], newCandidate);
      //std::cout << "<hr>Weight h element we are looking for: " << weightHElementWeAreLookingFor.ToString()
      //<< " base candidate is: " << baseCandidate.ToString();

      indicesModulesNewComponentExtensionMod.SetSize(0);
      for (int j=0; j<baseCandidate.HighestWeightsNONPrimal.size; j++)
        if (baseCandidate.HighestWeightsNONPrimal[j]==weightHElementWeAreLookingFor)
          indicesModulesNewComponentExtensionMod.AddOnTop(j);
      if (indicesModulesNewComponentExtensionMod.size==0)
      { if (theGlobalVariables!=0)
        { std::stringstream reportStream;
          reportStream << " Extension " << i+1 << " out of " << theLargerTypes.size << ", type  " << theLargerTypes[i].ToString()
          << " cannot be realized: no appropriate module.";
          std::cout << "<hr>" << reportStream.str();
          theReport2.Report(reportStream.str());
        }
        continue;
      }
    }
    theSmallType=theLargerTypes[i].GetSmallestSimpleType();
    int indexNewRooT=*theRootInjections[i].LastObject();
    int indexNewRootInSmallType=indexNewRooT-theLargerTypes[i].GetRank()+theSmallType.theRank;
    Rational desiredHLengthSquared=theSmallType.CartanSymmetricInverseScale*2*theSmallType.GetDefaultRootLengthSquared(0)/
    theSmallType.GetDefaultRootLengthSquared(indexNewRootInSmallType);
    newCandidate.SetUpInjectionHs(baseCandidate, theLargerTypes[i], theRootInjections[i]);
    for (int j=0; j<this->theSl2s.size; j++)
    { if (theGlobalVariables!=0)
      { std::stringstream reportStreamX;
        reportStreamX << "Trying to realize via orbit number " << j+1 << " the latest root (index "
        << indexNewRootInSmallType << ") of the candidate simple component  " << theSmallType.ToString();
        if (this->theSl2s[j].LengthHsquared!=desiredHLengthSquared)
          reportStreamX << ". The h element "  << this->theSl2s[j].theH.GetCartanPart().ToString()
          << " of length " << this->theOrbitHelementLengths[j].ToString() << " generating orbit number " << j+1 << " out of "
          << this->theSl2s.size << " does not have the required length of " << desiredHLengthSquared.ToString();
        else
          std::cout << " The h element " << this->theSl2s[j].theH.GetCartanPart().ToString()
          << " generating orbit number " << j+1 << " out of " << this->theSl2s.size << " has the required length. ";
        std::cout << "<br>" << reportStreamX.str();
        theReport3.Report(reportStreamX.str());
      }
      if (this->theSl2s[j].LengthHsquared!=desiredHLengthSquared)
        continue;
      theHCandidatesScaledToActByTwo.SetSize(0);
      if (baseRank!=0)
      { const HashedList<Vector<Rational> >& currentOrbit=this->GetOrbitSl2Helement(j);
        theHCandidatesScaledToActByTwo.ReservE(currentOrbit.size);
        std::cout << "The rescaling factor is " << (theSmallType.GetDefaultRootLengthSquared(indexNewRootInSmallType)/2).ToString();
        for (int k=0; k<currentOrbit.size; k++)
        { if (newCandidate.IsGoodHnewActingByTwo(currentOrbit[k], theRootInjections[i]))
          { if (theGlobalVariables!=0)
            { std::stringstream out2;
              out2 << "sl(2) orbit " << j+1 << ", h orbit candidate " << k+1 << " out of " << currentOrbit.size << " has desired scalar products, adding to list of good candidates. ";
              theReport2.Report(out2.str());
              //std::cout << "<hr>" << out2.str();
            }
            theHCandidatesScaledToActByTwo.AddOnTop(currentOrbit[k]);
          } else
            if (theGlobalVariables!=0)
            { std::stringstream out2;
              out2 << "sl(2) orbit " << j+1 << ", h orbit candidate " << k+1 << " out of " << currentOrbit.size << " is not a valid candidate (doesn't have desired scalar products). ";
              theReport2.Report(out2.str());
              //std::cout << "<hr>" << out2.str();
            }
        }
      } else
      { theHCandidatesScaledToActByTwo.AddOnTop(this->theSl2s[j].theH.GetCartanPart());
        if (theGlobalVariables!=0)
        { std::stringstream out;
          out << "Orbit of " << this->theSl2s[j].theH.GetCartanPart().ToString() << " not generated because that is the very first H element selected.";
        }
      }
      if (theHCandidatesScaledToActByTwo.size==0)
      { std::stringstream out2;
        out2 << "Sl(2) orbit " << j+1 << ": extension of " << baseCandidate.theWeylNonEmbeddeD.theDynkinType.ToString()
        << " to " << theLargerTypes[i].ToString() << " not possible because there were no h candidates.";
        theReport2.Report(out2.str());
        std::cout << "<hr>" << out2.str();
      }
      std::cout << "<hr>Testing a total of " << theHCandidatesScaledToActByTwo.size << " candidates. ";
      for (int k=0; k<theHCandidatesScaledToActByTwo.size; k++)
      { if (theGlobalVariables!=0)
        { std::stringstream out2;
          out2 << "Attempting to extend " << baseCandidate.theWeylNonEmbeddeD.theDynkinType.ToString() << " to "
          << theLargerTypes[i].ToString() << " using sl(2) orbit " << j+1 << ", h element " << k+1 << " out of " << theHCandidatesScaledToActByTwo.size << ".";
          theReport2.Report(out2.str());
          //std::cout << "<br>" << out2.str();
        }
        if (newCandidate.CreateAndAddByExtendingBaseSubalgebra(baseCandidate, theHCandidatesScaledToActByTwo[k], theLargerTypes[i], theRootInjections[i]))
        { if (theGlobalVariables!=0)
          { std::stringstream reportStream;
            reportStream << " Successfully extended " << baseCandidate.theWeylNonEmbeddeD.theDynkinType.ToString() << " to "
            << newCandidate.theWeylNonEmbeddeD.theDynkinType.ToString() << " (Type " << i+1 << " out of " << theLargerTypes.size
            << ", h candidate " << k+1 << " out of " << theHCandidatesScaledToActByTwo.size << "). ";
            std::cout << reportStream.str();
            theReport3.Report(reportStream.str());
          }
          this->ExtendCandidatesRecursive(newCandidate, targetType);
        } else
        { std::stringstream out2;
          out2 << "sl(2) orbit " << j+1 << ", h element " << k+1 << " out of " << theHCandidatesScaledToActByTwo.size << ": did not succeed extending. ";
          //std::cout << out2.str();
          theReport2.Report(out2.str());
        }
      }
      //std::cout << "<hr>Done with the type";
    }
  }
}

void SemisimpleSubalgebras::RegisterPossibleCandidate(CandidateSSSubalgebra& input)
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::RegisterPossibleCandidate");
  SemisimpleLieAlgebra tempSA;
  tempSA.theWeyl=input.theWeylNonEmbeddeDdefaultScale;
  bool needToComputeConstants=this->theSubalgebrasNonEmbedded->Contains(tempSA);
  int theIndex=this->theSubalgebrasNonEmbedded->AddNoRepetitionOrReturnIndexFirst(tempSA);
  if (needToComputeConstants)
    this->theSubalgebrasNonEmbedded->GetElement(theIndex).ComputeChevalleyConstants(theGlobalVariables);
  input.indexInOwnersOfNonEmbeddedMe=theIndex;
}

bool DynkinType::IsTypeA_1()const
{ if (this->size()!=1)
    return false;
  if (this->theCoeffs[0]!=1)
    return false;
  if ((*this)[0].theRank!=1)
    return false;
  return (*this)[0].theLetter=='A';
}

void DynkinType::GetDynkinTypeWithDefaultScales(DynkinType& output)const
{ if (&output==this)
  { DynkinType thisCopy=*this;
    thisCopy.GetDynkinTypeWithDefaultScales(output);
    return;
  }
  output.MakeZero();
  DynkinSimpleType tempType;
  for (int i =0; i<this->size(); i++)
  { tempType.MakeArbitrary((*this)[i].theLetter, (*this)[i].theRank, 1);
    output.AddMonomial(tempType, this->theCoeffs[i]);
  }
}

void DynkinSimpleType::GetAutomorphismActingOnVectorColumn(MatrixTensor<Rational>& output, int AutoIndex)const
{ MacroRegisterFunctionWithName("DynkinSimpleType::GetAutomorphismActingOnVectorColumn");
  if (AutoIndex==0 || this->theLetter=='B' || this->theLetter=='C' || this->theLetter=='G' || this->theLetter=='F' || this->theRank==1 || (this->theLetter=='E' && this->theRank!=6))
  { output.MakeId(this->theRank);
    return;
  }
  output.MakeZero();
  if (this->theLetter=='A' && this->theRank!=1)
    for (int i=0; i<this->theRank; i++)
      output.AddMonomial(MonomialMatrix(i, this->theRank-i-1),1);
  if (this->theLetter=='D')
  { if (this->theRank==4)
    { //Here be D_4 triality.
      //The automorphism group of Dynkin Diagram D_4 is S3
      //The triple node is always fixed:
      output.AddMonomial(MonomialMatrix(1,1),1);
      if (AutoIndex==1)
      { //permutation (12), AutoIndex=1
        output.AddMonomial(MonomialMatrix(0,2),1);
        output.AddMonomial(MonomialMatrix(2,0),1);
        output.AddMonomial(MonomialMatrix(3,3),1);
      } else if (AutoIndex==2)
      { //permutation (23), AutoIndex=2
        output.AddMonomial(MonomialMatrix(0,0),1);
        output.AddMonomial(MonomialMatrix(2,3),1);
        output.AddMonomial(MonomialMatrix(3,2),1);
      } else if (AutoIndex==3)
      { //permutation (12)(23)=(123), AutoIndex=3
        output.AddMonomial(MonomialMatrix(0,2),1);
        output.AddMonomial(MonomialMatrix(2,3),1);
        output.AddMonomial(MonomialMatrix(3,0),1);
      } else if (AutoIndex==4)
      { //permutation (23)(12)=(132), AutoIndex=4
        output.AddMonomial(MonomialMatrix(0,3),1);
        output.AddMonomial(MonomialMatrix(2,0),1);
        output.AddMonomial(MonomialMatrix(3,2),1);
      } else if (AutoIndex==5)
      { //permutation (12)(23)(12)=(13), AutoIndex=5
        output.AddMonomial(MonomialMatrix(0,3),1);
        output.AddMonomial(MonomialMatrix(2,2),1);
        output.AddMonomial(MonomialMatrix(3,0),1);
      } else
        crash << "This is a programming error: requesting triality automorphism with index not in the range 0-5. " << crash;
    } else
    { for (int i=0; i<this->theRank-2; i++)
        output.AddMonomial(MonomialMatrix(i,i), 1);
      output.AddMonomial(MonomialMatrix(this->theRank-1, this->theRank-2),1);
      output.AddMonomial(MonomialMatrix(this->theRank-2, this->theRank-1),1);
    }
  }
  if (this->theLetter=='E' && this->theRank==6)
  { //We gotta flip the Dynkin diagram of E_6. Note that the extra 1-length sticking root comes second and
    //and the triple node comes fourth.
    //Therefore, we must swap the 1st root with the 6th and the third root
    //with the 5th. Conventions, conventions... no way around 'em!
     output.AddMonomial(MonomialMatrix(1,1),1);
     output.AddMonomial(MonomialMatrix(3,3),1);
     output.AddMonomial(MonomialMatrix(0,5),1);
     output.AddMonomial(MonomialMatrix(5,0),1);
     output.AddMonomial(MonomialMatrix(2,4),1);
     output.AddMonomial(MonomialMatrix(4,2),1);
  }
  Rational tempRat=output.GetDeterminant();
  if (tempRat!=1 && tempRat!=-1)
  { FormatExpressions theFormat;
    theFormat.flagUseHTML=false;
    theFormat.flagUseLatex=true;
    crash << "This is a programming error: the determinant of the automorphism matrix of the Dynkin graph must be +/-1, it is instead "
    << tempRat.ToString() << ". The auto matrix is: " << CGI::GetMathMouseHover(output.ToStringMatForm(&theFormat)) << " and the dynkin type is: "
    << this->ToString() << "." << crash;
  }
}

DynkinSimpleType DynkinType::GetSmallestSimpleType()const
{ if (this->size()==0)
    crash << "This is a programming error: asking for the smallest simple type of a 0 dynkin type. " << crash;
  DynkinSimpleType result=(*this)[0];
  for (int i=1; i<this->size(); i++)
    if ((*this)[i]<result)
      result=(*this)[i];
  return result;
}

DynkinSimpleType DynkinType::GetGreatestSimpleType()const
{ if (this->size()==0)
    crash << "This is a programming error: asking for the greatest simple type of a 0 dynkin type. " << crash;
  DynkinSimpleType result=(*this)[0];
  for (int i=1; i<this->size(); i++)
    if ((*this)[i]>result)
      result=(*this)[i];
  return result;
}

const HashedList<ElementWeylGroup<WeylGroup> >& SemisimpleSubalgebras::GetOrbitSl2HelementWeylGroupElt(int indexSl2)
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::GetOrbitSl2HelementWeylGroupElt");
  if (this->theOrbitsAreComputed[indexSl2])
    return this->theOrbitGeneratingElts[indexSl2];
  this->GetOrbitSl2Helement(indexSl2);
  return this->theOrbitGeneratingElts[indexSl2];
}

const HashedList<Vector<Rational> >& SemisimpleSubalgebras::GetOrbitSl2Helement(int indexSl2)
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::GetOrbitSl2Helement");
  if (this->theOrbitsAreComputed[indexSl2])
    return this->theOrbits[indexSl2];
  Vectors<Rational> startingVector;
  startingVector.AddOnTop(this->theSl2s[indexSl2].theH.GetCartanPart());
  std::stringstream out;
  ProgressReport theReport(this->theGlobalVariables);
  if (this->theGlobalVariables!=0)
  { out << "Generating orbit of " << startingVector[0].ToString() << "...";
    theReport.Report(out.str());
//      if (baseCandidate.theWeylNonEmbeddeD.theDynkinType.ToString()=="A^{6}_2")
//        std::cout << out.str();
  }
  if (!this->GetSSowner().theWeyl.GenerateOuterOrbit(startingVector, this->theOrbits[indexSl2], &this->theOrbitGeneratingElts[indexSl2], 60000))
    crash << "<hr> Failed to generate weight orbit: orbit has more than hard-coded limit of 10000 elements. "
    << " This is not a programming error, but I am crashing in flames to let you know you hit the computational limits. "
    << "You might want to work on improving the algorithm for generating semisimple subalgebras. Here is a stack trace for you. " << crash;
  if (this->theGlobalVariables!=0)
  { out << " done. The size of the orbit is " << this->theOrbits[indexSl2].size;
    theReport.Report(out.str());
    //if (baseCandidate.theWeylNonEmbeddeD.theDynkinType.ToString()=="A^{6}_2")
      //std::cout << " done. The size of the orbit is " << theOrbit.size;
  }
  this->theOrbitsAreComputed[indexSl2]=true;
  return this->theOrbits[indexSl2];
}

bool CandidateSSSubalgebra::CheckInitialization()const
{ if (this->flagDeallocated)
    crash << "This is a programming error: use after free of CandidateSSSubalgebra. " << crash;
  if (this->owner==0)
    crash << "This is a programming error: use of non-initialized semisimple subalgebra candidate. " << crash;
  return true;
}

WeylGroup& CandidateSSSubalgebra::GetAmbientWeyl()const
{ this->CheckInitialization();
  return this->owner->GetSSowner().theWeyl;
}

SemisimpleLieAlgebra& CandidateSSSubalgebra::GetAmbientSS()const
{ this->CheckInitialization();
  return this->owner->GetSSowner();
}

void CandidateSSSubalgebra::AddHincomplete(const Vector<Rational>& theH, const ElementWeylGroup<WeylGroup>& theWE, int indexOfOrbit)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::AddHincomplete");
  this->CheckInitialization();
  if (this->CartanSAsByComponent.size==1)
    if (this->CartanSAsByComponent.LastObject()->size==0)
      this->PosRootsPerpendicularPrecedingWeights=this->GetAmbientWeyl().RootsOfBorel;
  for (int i=0; i<this->PosRootsPerpendicularPrecedingWeights.size; i++)
    if (this->GetAmbientWeyl().RootScalarCartanRoot(this->PosRootsPerpendicularPrecedingWeights[i], theH)!=0)
    { this->PosRootsPerpendicularPrecedingWeights.RemoveIndexShiftDown(i);
      i--;
    }
  this->CartanSAsByComponent.LastObject()->AddOnTop(theH);
}

bool CandidateSSSubalgebra::IsGoodHnewActingByTwo(const Vector<Rational>& HNewActingByTwo, const List<int>& theRootInjections)const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::isGoodHnew");
  //Check if input weight is maximally dominant:
  std::cout << "<br>Checking whether " << HNewActingByTwo.ToString() << " is a good new vector. ";
  Rational theScalarProd;
  int indexHneW=*theRootInjections.LastObject();
  for  (int i=0; i<this->GetAmbientWeyl().RootsOfBorel.size; i++)
  { Vector<Rational>& currentPosRoot=this->GetAmbientWeyl().RootsOfBorel[i];
    bool canBeRaisingReflection=true;
    for (int l=0; l<this->theHsScaledToActByTwoInOrderOfCreation.size && canBeRaisingReflection; l++)
    { theScalarProd=this->GetAmbientWeyl().RootScalarCartanRoot(currentPosRoot, this->theHsScaledToActByTwoInOrderOfCreation[l]);
      if (theScalarProd>0)
        canBeRaisingReflection=false;
      if (theScalarProd<0)
        crash << "This is a programming error. While trying to realize type " << this->theWeylNonEmbeddeD.theDynkinType.ToString()
        << ", the candidate h elements of the semisimple subalgebra are supposed to be maximally dominant, "
        << "however the scalar product of the positive root " << currentPosRoot.ToString() << " with the subalgebra root "
        << this->theHsScaledToActByTwoInOrderOfCreation[l].ToString() << " is negative, while the very same positive root has had zero scalar products with all "
        << " preceding roots. Hnew equals: " << HNewActingByTwo.ToString() << " Here are all preceding roots: "
        << this->theHsScaledToActByTwoInOrderOfCreation.ToString() << crash;
    }
    if (canBeRaisingReflection)
      if (this->GetAmbientWeyl().RootScalarCartanRoot(currentPosRoot, HNewActingByTwo)<0)
      { std::cout << "<br>" << HNewActingByTwo.ToString() << " is not a good vector because it has negative scalar product with "
        << currentPosRoot.ToString() << ". ";
        return false;
      }
  }
  for (int i=0; i<this->theHsScaledToActByTwo.size; i++)
  { if (i!=indexHneW)
      theScalarProd= this->GetAmbientWeyl().RootScalarCartanRoot(HNewActingByTwo, this->theHsScaledToActByTwo[i]);
    else
      theScalarProd=this->GetAmbientWeyl().RootScalarCartanRoot(HNewActingByTwo, HNewActingByTwo);
    if (theScalarProd!=this->theWeylNonEmbeddeD.CartanSymmetric(indexHneW, i))
    { std::cout << HNewActingByTwo.ToString() << " is not a good vector because it has scalar product " << theScalarProd.ToString() << " with "
      << (i==indexHneW ? HNewActingByTwo.ToString() : this->theHsScaledToActByTwo[i].ToString()) << " instead of the desired "
      << this->theWeylNonEmbeddeD.CartanSymmetric(indexHneW, i).ToString();
      return false;
    }
  }
  return true;
}

template <class coefficient>
int charSSAlgMod<coefficient>::GetIndexExtremeWeightRelativeToWeyl(WeylGroup& theWeyl)const
{ HashedList<Vector<coefficient> > weightsSimpleCoords;
  weightsSimpleCoords.SetExpectedSize(this->size());
  for (int i=0; i<this->size(); i++)
    weightsSimpleCoords.AddOnTop(theWeyl.GetSimpleCoordinatesFromFundamental((*this)[i].weightFundamentalCoordS));
  for (int i=0; i<weightsSimpleCoords.size; i++)
  { bool isGood=true;
    for (int j=0; j<theWeyl.RootsOfBorel.size; j++)
      if(weightsSimpleCoords.Contains((weightsSimpleCoords[i]+theWeyl.RootsOfBorel[j])))
      { isGood=false;
        break;
      }
    if (isGood)
      return i;
  }
  return -1;
}

bool CandidateSSSubalgebra::IsWeightSystemSpaceIndex(int theIndex, const Vector<Rational>& AmbientRootTestedForWeightSpace)
{ for (int k=0; k<this->theHs.size; k++)
  { Rational desiredScalarProd=this->theWeylNonEmbeddeDdefaultScale.CartanSymmetric(theIndex, k);
    Rational actualScalar= this->GetAmbientWeyl().RootScalarCartanRoot(this->theHs[k], AmbientRootTestedForWeightSpace);
    if (desiredScalarProd!=actualScalar)
      return false;
  }
  return true;
}

bool CandidateSSSubalgebra::ComputeSystem(bool AttemptToChooseCentalizer, bool allowNonPolynomialSystemFailure)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputeSystem");
  ChevalleyGenerator currentGen, currentOpGen;
  this->theInvolvedNegGenerators.SetSize(this->theHsScaledToActByTwo.size);
  this->theInvolvedPosGenerators.SetSize(this->theHsScaledToActByTwo.size);
  for (int i=0; i<this->theHsScaledToActByTwo.size; i++)
  { List<ChevalleyGenerator>& currentInvolvedPosGens= this->theInvolvedPosGenerators[i];
    List<ChevalleyGenerator>& currentInvolvedNegGens= this->theInvolvedNegGenerators[i];
    currentInvolvedNegGens.SetExpectedSize(this->GetAmbientWeyl().GetDim()*2);
    currentInvolvedPosGens.SetExpectedSize(this->GetAmbientWeyl().GetDim()*2);
    currentInvolvedNegGens.SetSize(0);
    currentInvolvedPosGens.SetSize(0);
    for (int j=0; j<this->GetAmbientWeyl().RootSystem.size; j++)
//    for (int j=this->GetAmbientWeyl().RootSystem.size-1; j>=0; j--)
    { const Vector<Rational>& currentAmbRoot=this->GetAmbientWeyl().RootSystem[j];
      int indexCurGen=this->GetAmbientSS().GetGeneratorFromRootIndex(j);
      int opIndex= this->GetAmbientSS().GetGeneratorFromRoot(-currentAmbRoot);
      currentGen.MakeGenerator(*this->owner->owneR, indexCurGen);
      currentOpGen.MakeGenerator(*this->owner->owneR, opIndex);
      if (this->IsWeightSystemSpaceIndex(i, this->GetAmbientWeyl().RootSystem[j]))
      { currentInvolvedPosGens.AddOnTop(currentGen);
        currentInvolvedNegGens.AddOnTop(currentOpGen);
        //std::cout << "<br>Generator " << currentGen.ToString() << " is good";
      } //else
        //std::cout << "<br>Generator " << currentGen.ToString() << " ain't no good";
    }
    if (currentInvolvedNegGens.size==0)
    { if (currentInvolvedPosGens.size!=0)
        crash << "The number of involved negative generators is different from the number of involved positive generators. " << crash;
      return false;
    }
  }
  this->flagUsedInducingSubalgebraRealization=true;
  return this->ComputeSystemPart2(AttemptToChooseCentalizer, allowNonPolynomialSystemFailure);
}

bool CandidateSSSubalgebra::CheckGensBracketToHs()
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::CheckGensBracketToHs");
  if (this->theNegGens.size!=this->thePosGens.size || this->theNegGens.size!=this->theWeylNonEmbeddeD.GetDim())
    return false;
  ElementSemisimpleLieAlgebra<AlgebraicNumber> goalH, lieBracket;
  for (int i=0; i<this->theNegGens.size; i++)
  { goalH.MakeHgenerator(this->theHsScaledToActByTwo[i], *this->owner->owneR);
    this->GetAmbientSS().LieBracket(this->thePosGens[i], this->theNegGens[i], lieBracket);
    lieBracket-=goalH;
    if (!lieBracket.IsEqualToZero())
      return false;
  }
  return true;
}

bool CandidateSSSubalgebra::ComputeSystemPart2(bool AttemptToChooseCentalizer, bool allowNonPolynomialSystemFailure)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputeSystemPart2");
  theSystemToSolve.SetSize(0);
  ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> > lieBracketMinusGoalValue, goalValue;
  Vector<Polynomial<Rational> > desiredHpart;
  this->CheckInitialization();
//  if (this->indexInOwnersOfNonEmbeddedMe<0 || this->indexInOwnersOfNonEmbeddedMe >=this->owner->theSubalgebrasNonEmbedded
  const SemisimpleLieAlgebra& nonEmbeddedMe=(*this->owner->theSubalgebrasNonEmbedded)[this->indexInOwnersOfNonEmbeddedMe];
  this->totalNumUnknownsNoCentralizer=0;
  if (this->theHs.size==0)
    crash << "This is a programming error: the number of involved H's cannot be zero. " << crash;
  if (this->theInvolvedNegGenerators.size!=this->theHs.size)
    crash << "This is a programming error: the number of involved negative generators, which is " << this->theInvolvedNegGenerators.size
    << " is not equal to the subalgebra rank, which is " << this->theHs.size << ". " << crash;
  for (int i=0; i<this->theInvolvedNegGenerators.size; i++)
    this->totalNumUnknownsNoCentralizer+=this->theInvolvedNegGenerators[i].size;
  if (this->theWeylNonEmbeddeD.RootSystem.size==0)
    crash << "This is a programming error: the root system of the candidate subalgebra has not been computed " << crash;
  this->totalNumUnknownsNoCentralizer*=2;
  this->totalNumUnknownsWithCentralizer=this->totalNumUnknownsNoCentralizer;
  this->theUnknownNegGens.SetSize(this->theInvolvedNegGenerators.size);
  this->theUnknownPosGens.SetSize(this->theInvolvedPosGenerators.size);
  if (!AttemptToChooseCentalizer)
    this->theUnknownCartanCentralizerBasis.SetSize(0);
  else
  { int rankCentralizer=-1;
    bool tempB= this->centralizerRank.IsSmallInteger(&rankCentralizer);
    if (!tempB || rankCentralizer<0 || rankCentralizer>this->GetAmbientWeyl().GetDim())
    { if (allowNonPolynomialSystemFailure)
        return false;
      if (!tempB)
        crash << "Error: rankCentralizer is not a small integer. ";
      crash << "This is a programming error: rankCentralizer not computed, or not computed correctly, when it should be. "
      << " Currently rankCentalizer is supposed to be " << rankCentralizer << ". Here is a detailed printout of the candidate subalgebra. "
      << this->ToString() << crash;
    }
    this->totalNumUnknownsWithCentralizer+=rankCentralizer*this->GetAmbientWeyl().GetDim()+1;
    this->theUnknownCartanCentralizerBasis.SetSize(rankCentralizer);
  }
  if (this->indexIamInducedFrom==-1)
    this->flagUsedInducingSubalgebraRealization=false;
  int indexNewRoot=-1;
  if (this->flagUsedInducingSubalgebraRealization)
    indexNewRoot=*this->RootInjectionsFromInducer.LastObject();
  for (int i=0; i<this->theInvolvedNegGenerators.size; i++)
  { bool seedsHaveBeenSown=false;
    if (this->flagUsedInducingSubalgebraRealization)
    { CandidateSSSubalgebra& theInducer=this->owner->theSubalgebraCandidates[this->indexIamInducedFrom];
      if (theInducer.flagSystemSolved && i!=indexNewRoot)
      { //std::cout << " <hr>... and the inducer is: " << theInducer.theWeylNonEmbeddeD.theDynkinType.ToString();
        //if (theInducer.theWeylNonEmbeddeD.theDynkinType.ToString()=="A^{1}_1")
        //  std::cout << "<br>Detailed inducer printout<br>" << theInducer.ToString();
        int preimageIndex=DynkinType::GetIndexPreimageFromRootInjection(i, this->RootInjectionsFromInducer);
        this->theUnknownNegGens[i]=theInducer.theNegGens[preimageIndex];//<-implicit type conversion from base field to polynomial here
        this->theUnknownPosGens[i]=theInducer.thePosGens[preimageIndex];//<-implicit type conversion from base field to polynomial here
        //std::cout << "<br>induced neg generator: " << theInducer.theNegGens[preimageIndex];
        //std::cout << "<br>induced pos generator: " << theInducer.thePosGens[preimageIndex];
        //std::cout << "<br>total num neg generators: " << this->theInvolvedNegGenerators.size;
        seedsHaveBeenSown=true;
      }
    }
    if (seedsHaveBeenSown)
      continue;
    this->GetGenericNegGenLinearCombination(i, this->theUnknownNegGens[i]);
    this->GetGenericPosGenLinearCombination(i, this->theUnknownPosGens[i]);
    //std::cout << "<hr>Unknown generator index " << i << ": " << this->theUnknownNegGens[i].ToString();
  }
  //if (this->theWeylNonEmbeddeD.theDynkinType.ToString()=="A^{1}_2")
  //  std::cout << "<br>Finally, the neg gens be: " << this->theUnknownNegGens.ToString()
  //  << "<br>'n the pos gens be: " << this->theUnknownPosGens.ToString();

  if (this->theUnknownCartanCentralizerBasis.size>0)
  { Matrix<Polynomial<AlgebraicNumber> > theCentralizerCartanVars;
    Vectors<Polynomial<AlgebraicNumber> > theCentralizerCartanElts;
    theCentralizerCartanElts.SetSize(this->theUnknownCartanCentralizerBasis.size);
    for (int i=0; i<this->theUnknownCartanCentralizerBasis.size; i++)
    { this->GetGenericCartanCentralizerLinearCombination(i, this->theUnknownCartanCentralizerBasis[i]);
      theCentralizerCartanElts[i]=this->theUnknownCartanCentralizerBasis[i].GetCartanPart();
      //std::cout << "<hr>Unknown element of centralizer cartan " << i << ": "
      //<< theCentralizerCartanElts[i].ToString();
      //std::cout << "<hr>Unknown generator index " << i << ": "
      //    << this->theUnknownCartanCentralizerBasis[i].ToString();
    }
    theCentralizerCartanElts.GetGramMatrix(theCentralizerCartanVars, &this->GetAmbientWeyl().CartanSymmetric);
    Polynomial<AlgebraicNumber> theDeterminant, theDetMultiplier;
    theDeterminant.MakeDeterminantFromSquareMatrix(theCentralizerCartanVars);
    theDetMultiplier.MakeMonomiaL(this->totalNumUnknownsWithCentralizer-1, 1, 1);
    theDeterminant*=theDetMultiplier;
    theDeterminant+=-1;
    this->theSystemToSolve.AddOnTop(theDeterminant);
  }
  for (int i=0; i<this->theInvolvedNegGenerators.size; i++)
  { desiredHpart=this->theHsScaledToActByTwo[i];//<-implicit type conversion here!
    goalValue.MakeHgenerator(desiredHpart, *this->owner->owneR);
    this->GetAmbientSS().LieBracket(this->theUnknownPosGens[i], this->theUnknownNegGens[i], lieBracketMinusGoalValue);
    //std::cout << "<hr>[" << this->theUnknownPosGens[i].ToString() << ", "
    //<< this->theUnknownNegGens[i].ToString() << "] = " << lieBracketMinusGoalValue.ToString();
    lieBracketMinusGoalValue-=goalValue;
    this->AddToSystem(lieBracketMinusGoalValue);
    for (int j=0; j<this->theUnknownCartanCentralizerBasis.size; j++)
    { this->GetAmbientSS().LieBracket(this->theUnknownNegGens[i], this->theUnknownCartanCentralizerBasis[j], lieBracketMinusGoalValue);
      this->AddToSystem(lieBracketMinusGoalValue);
      this->GetAmbientSS().LieBracket(this->theUnknownPosGens[i], this->theUnknownCartanCentralizerBasis[j], lieBracketMinusGoalValue);
      this->AddToSystem(lieBracketMinusGoalValue);
    }
    for (int j=0; j<this->theInvolvedPosGenerators.size; j++)
      if (i!=j)
      { this->GetAmbientSS().LieBracket(this->theUnknownNegGens[i], this->theUnknownPosGens[j], lieBracketMinusGoalValue);
        //std::cout << "<hr>[" << this->theUnknownNegGens[i].ToString() << ", "
        //<< this->theUnknownPosGens[j].ToString() << "] = " << lieBracketMinusGoalValue.ToString();
        this->AddToSystem(lieBracketMinusGoalValue);
        Vector<Rational> posRoot1, posRoot2;
        posRoot1.MakeEi(this->theWeylNonEmbeddeD.GetDim(), i);
        posRoot2.MakeEi(this->theWeylNonEmbeddeD.GetDim(), j);
        int q;
        if (!nonEmbeddedMe.GetMaxQForWhichBetaMinusQAlphaIsARoot(posRoot1, -posRoot2, q))
          crash << "This is a programming error: the alpha-string along " << posRoot1.ToString() << " through " << (-posRoot2).ToString()
          << " does not contain any root, which is impossible. " << crash;
        //if (this->theWeylNonEmbeddeD.theDynkinType.ToString()=="A^{1}_2")
        //{ std::cout << " <br>the q in the q-string is: " << q;
        //}
        lieBracketMinusGoalValue=this->theUnknownPosGens[j];
        for (int k=0; k<q+1; k++)
          this->GetAmbientSS().LieBracket(this->theUnknownPosGens[i], lieBracketMinusGoalValue, lieBracketMinusGoalValue);
        //if (this->theWeylNonEmbeddeD.theDynkinType.ToString()=="A^{1}_2")
        //  std::cout << "<hr>ad of " << this->theUnknownPosGens[i] << " applied to " << this->theUnknownPosGens[j]
        //  << q+1 << " times = " << lieBracketMinusGoalValue;
        this->AddToSystem(lieBracketMinusGoalValue);
      }
  }
  this->flagSystemSolved=false;
  if (!AttemptToChooseCentalizer)
    this->flagSystemSolved= this->CheckGensBracketToHs();
  if (!this->flagSystemSolved)
  { this->flagSystemGroebnerBasisFound=false;
    this->flagSystemProvedToHaveNoSolution=false;
    if (this->owner->flagAttemptToSolveSystems)
    { long long int startNumOps=Rational::TotalArithmeticOperations();
//      std::cout << "<br>startnumAdditions: " << startNumOps;
      this->AttemptToSolveSystem();
//      std::cout << "<br>final num additions: " << Rational::TotalArithmeticOperations();
      this->totalArithmeticOpsToSolveSystem+=Rational::TotalArithmeticOperations()-startNumOps;
    }
  } else
  { this->flagSystemGroebnerBasisFound=false;
    this->flagSystemProvedToHaveNoSolution=false;
  }
  if (!this->flagSystemSolved && this->flagUsedInducingSubalgebraRealization)
  { //bool seedHadNoSolution=this->flagSystemProvedToHaveNoSolution;
    this->flagUsedInducingSubalgebraRealization=false;
    bool result=this->ComputeSystemPart2(AttemptToChooseCentalizer, allowNonPolynomialSystemFailure);
    //if (seedHadNoSolution && this->flagSystemSolved)
    //  std::cout << "<hr>I did not expect that: seed system coming from inducer had NO solution, but system DID HAVE overall a solution. "
    //  << "This may be a programming mistake. If not, then this needs to be investigated. Here is the subalgebra: " << this->ToString();
    return result;
  }
  if (this->flagSystemProvedToHaveNoSolution)
  { //std::cout << "System proven to have no solution, type is " << this->theWeylNonEmbeddeD.theDynkinType.ToString();
    return false;
  }
  if (this->flagSystemSolved)
  { this->theBasis=this->theNegGens;
    this->theBasis.AddListOnTop(this->thePosGens);
    if (this->theBasis.size>0)
    { this->owner->owneR->GenerateLieSubalgebra(this->theBasis);
      if (this->theBasis.size!=this->theWeylNonEmbeddeD.theDynkinType.GetRootSystemPlusRank())
      { if (!allowNonPolynomialSystemFailure)
          crash << "This is a programming error. Lie subalgebra dimension doesn't fit: dimension of generated subalgebra is "
          << this->theBasis.size << ", must be " << this->theWeylNonEmbeddeD.theDynkinType.GetRootSystemPlusRank()
          << ". The subalgebra is " << this->ToString() << "<br>Involved generators: " << this->theInvolvedNegGenerators.ToString()
          << "<br>and<br>" << this->theInvolvedPosGenerators.ToString() << crash;
        return false;
      }
    }
    this->owner->owneR->GetCommonCentralizer(this->thePosGens, this->HighestVectorsNonSorted);
    this->ComputeCartanOfCentralizer(this->owner->theGlobalVariables);
    this->ComputePrimalModuleDecompositionHWsHWVsOnly(this->owner->theGlobalVariables);
  }
  return true;
}

void CandidateSSSubalgebra::ExtendToModule(List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& inputOutput, GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ExtendToModule");
  ElementSemisimpleLieAlgebra<AlgebraicNumber> tempElt;
  List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > theVectorSpace;
  HashedList<ChevalleyGenerator> bufferList;
  theVectorSpace=inputOutput;
  ProgressReport theReport(theGlobalVariables);
  for (int i=0; i<inputOutput.size; i++)
    for (int j=0; j<this->theNegGens.size; j++)
    { std::stringstream reportStream;
      reportStream << "ExtendToModule: Lie bracket of element of index " << i+1 << " and negative generator index " << j+1 << ".";
      theReport.Report(reportStream.str());
      this->GetAmbientSS().LieBracket(this->theNegGens[j], inputOutput[i], tempElt);
      theVectorSpace.AddOnTop(tempElt);
      tempElt.GaussianEliminationByRowsDeleteZeroRows(theVectorSpace, 0, &bufferList);
      if (theVectorSpace.size>inputOutput.size)
        inputOutput.AddOnTop(tempElt);
    }
}

template <class TemplateMonomial, class coefficient>
template <class MonomialCollectionTemplate>
void MonomialCollection<TemplateMonomial, coefficient>::IntersectVectorSpaces
(const List<MonomialCollectionTemplate>& vectorSpace1, const List<MonomialCollectionTemplate>& vectorSpace2, List<MonomialCollectionTemplate>& outputIntersection,
 HashedList<TemplateMonomial>* seedMonomials)
{ List<MonomialCollectionTemplate> theVspaces=vectorSpace1;
  List<MonomialCollectionTemplate> vectorSpace2eliminated=vectorSpace2;
  MonomialCollection<TemplateMonomial, coefficient>::GaussianEliminationByRowsDeleteZeroRows(vectorSpace2eliminated, 0, seedMonomials);
  MonomialCollection<TemplateMonomial, coefficient>::GaussianEliminationByRowsDeleteZeroRows(theVspaces, 0, seedMonomials);
  Matrix<coefficient> theLinCombiMat;
  int firstSpaceDim=theVspaces.size;
  theLinCombiMat.MakeIdMatrix(theVspaces.size+vectorSpace2eliminated.size);
  theVspaces.AddListOnTop(vectorSpace2eliminated);
  vectorSpace2eliminated=theVspaces;
  MonomialCollection<TemplateMonomial, coefficient>::GaussianEliminationByRows(theVspaces, 0, seedMonomials, &theLinCombiMat);
  int dimResult=0;
  for (int i=theVspaces.size-1; i>=0; i--)
    if (theVspaces[i].IsEqualToZero())
      dimResult++;
    else
      break;
  outputIntersection.SetSize(dimResult);
  int counter=-1;
  MonomialCollectionTemplate tempMCT;
  for (int i=theVspaces.size-1; i>=0; i--)
  { if (!theVspaces[i].IsEqualToZero())
      break;
    counter++;
    outputIntersection[counter].MakeZero();
    for (int j=0; j<firstSpaceDim; j++)
    { tempMCT=vectorSpace2eliminated[j];
      tempMCT*=theLinCombiMat(i,j);
      outputIntersection[counter]+=tempMCT;
    }
  }
}

bool CandidateSSSubalgebra::CheckModuleDimensions()const
{ int totalDim=0;
  for (int i=0; i<this->Modules.size; i++)
    for (int j=0; j<this->Modules[i].size; j++)
      totalDim+=this->Modules[i][j].size;
  if (totalDim!=this->GetAmbientSS().GetNumGenerators())
  { FormatExpressions theFormat;
    theFormat.flagCandidateSubalgebraShortReportOnly=false;
    crash << "<br><b>Something went very wrong with candidate " << this->theWeylNonEmbeddeD.theDynkinType.ToStringRelativeToAmbientType(this->GetAmbientWeyl().theDynkinType[0]) << ": dimensions DONT FIT!!! More precisely, "
    << "I am getting total module dimension sum  " << totalDim << " instead of " << this->GetAmbientSS().GetNumGenerators()
    << ".</b> Here is a detailed subalgebra printout. " << this->ToString(&theFormat) << crash;
  }
  return true;
}

void CandidateSSSubalgebra::ComputePrimalModuleDecomposition(GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputePrimalModuleDecomposition");
  for (int i=0; i<this->Modules.size; i++)
    for (int j=0; j<this->Modules[i].size; j++)
      this->ExtendToModule(this->Modules[i][j], theGlobalVariables);
  this->CheckModuleDimensions();
  this->ModulesIsotypicallyMerged.SetSize(this->Modules.size);
  for (int i=0; i<this->Modules.size; i++)
  { this->ModulesIsotypicallyMerged[i].SetSize(0);
    for (int j=0; j<this->Modules[i].size; j++)
      this->ModulesIsotypicallyMerged[i].AddListOnTop(this->Modules[i][j]);
  }
  //please note: part of primalSubalgebraModules have already been computed.
  //std::cout << "<hr>Testing sa: " << this->ToStringTypeAndHs() << "<br>";
  for (int i=0; i<this->ModulesIsotypicallyMerged.size; i++)
  //{ std::cout << "<br>Testing " << this->ModulesIsotypicallyMerged[i].ToString();
    if (this->ModulesIsotypicallyMerged[i][0].IsElementCartan())
    { for (int j=0; j<this->ModulesIsotypicallyMerged[i].size; j++)
        if (!this->ModulesIsotypicallyMerged[i][j].IsElementCartan())
          crash << "<br>This is a programming or mathematical error. Module " << this->ModulesIsotypicallyMerged[i].ToString()
          << " has elements of the ambient Cartan and elements outside of the ambient Cartan, which is not allowed. "
          << "<br>Here is a detailed subalgebra printout. " << crash;
      this->primalSubalgebraModules.AddOnTop(i);

    }// else
    //std::cout << "... aint no cartan no centralizer. ";
  //}
  this->fullBasisByModules.SetSize(0);
  this->fullBasisOwnerModules.SetSize(0);
  this->fullBasisByModules.ReservE(this->GetAmbientSS().GetNumGenerators());
  this->fullBasisOwnerModules.ReservE(this->GetAmbientSS().GetNumGenerators());
  for (int i=0; i<this->ModulesIsotypicallyMerged.size; i++)
    for (int j=0; j<this->ModulesIsotypicallyMerged[i].size; j++)
    { this->fullBasisByModules.AddOnTop(this->ModulesIsotypicallyMerged[i][j]);
      this->fullBasisOwnerModules.AddOnTop(i);
    }
  if (this->fullBasisByModules.size!=this->GetAmbientSS().GetNumGenerators())
    crash << "This is a programming error: the full basis by modules does not have same number of elements as the number of generators of the ambient Lie algebra. " << crash;
  this->WeightsModulesNONprimal.SetSize(this->Modules.size);
  this->WeightsModulesPrimal.SetSize(this->Modules.size);
  Vector<Rational> theProjection, thePrimalProjection;
  for (int i=0; i<this->Modules.size; i++)
    for (int k=0; k<this->Modules[i].size; k++)
    { if (k==0)
      { this->WeightsModulesNONprimal[i].SetSize(this->Modules[i][0].size);
        this->WeightsModulesPrimal[i].SetSize(this->Modules[i][0].size);
      }
      for (int j=0; j<this->Modules[i][k].size; j++)
      { ElementSemisimpleLieAlgebra<AlgebraicNumber>& currentElt=this->Modules[i][k][j];
        Vector<Rational> currentRoot=this->GetAmbientSS().GetWeightOfGenerator(currentElt[0].theGeneratorIndex);
        this->GetWeightProjectionFundCoords(currentRoot, theProjection);
        this->GetPrimalWeightProjectionFundCoords(currentRoot, thePrimalProjection);
        if (k==0)
        { this->WeightsModulesNONprimal[i][j]=theProjection;
          this->WeightsModulesPrimal[i][j]=thePrimalProjection;
        } else
          if (this->WeightsModulesNONprimal[i][j]!=theProjection || this->WeightsModulesPrimal[i][j]!=thePrimalProjection)
            crash << "This is a programming or mathematical error. Given two isomorphic modules over "
            << "the semisimple subalgebra (i.e., same highest weights), and the same order of generation of weights, I got different order "
            << " of the lower weights of the modules. Something is very wrong. " << crash;
      }
    }
  this->centralizerSubalgebraModules.SetSize(0);
  this->RootSystemCentralizerPrimalCoords.Clear();
//  std::cout << "<hr>Computing RootSystemCentralizerPrimalCoords type " << this->theWeylNonEmbeddeD.theDynkinType.ToString();
  for (int i=0; i<this->ModulesIsotypicallyMerged.size; i++)
    if (this->ModulesIsotypicallyMerged[i].size==1 || this->WeightsModulesPrimal[i][0].IsEqualToZero())
    { this->centralizerSubalgebraModules.AddOnTop(i);
      if (!this->WeightsModulesPrimal[i][0].IsEqualToZero())
        this->RootSystemCentralizerPrimalCoords.AddOnTop(this->WeightsModulesPrimal[i][0]);
    }
  for (int i=0; i<this->subalgebraModules.size; i++)
    for (int j=0; j<this->WeightsModulesPrimal[this->subalgebraModules[i]].size; j++)
      if (!this->WeightsModulesPrimal[this->subalgebraModules[i]][j].IsEqualToZero())
        this->RootSystemSubalgebraPrimalCoords.AddOnTop(this->WeightsModulesPrimal[this->subalgebraModules[i]][j]);
  Vectors<Rational> simpleSubSystem;
  simpleSubSystem=this->RootSystemCentralizerPrimalCoords;
  this->theCentralizerSubDiagram.ComputeDiagramTypeModifyInputRelative(simpleSubSystem, this->RootSystemCentralizerPrimalCoords, this->BilinearFormFundPrimal);
  this->theCentralizerType.MakeZero();
  for (int i=0; i<this->theCentralizerSubDiagram.SimpleComponentTypes.size; i++)
    this->theCentralizerType+=this->theCentralizerSubDiagram.SimpleComponentTypes[i];
  this->ComputeCharsPrimalModules();
}

void CandidateSSSubalgebra::reset(SemisimpleSubalgebras* inputOwner)
{ this->owner=inputOwner;
  this->indexInOwner=(-1);
  this->indexIamInducedFrom=-1;
  this->indexInOwnersOfNonEmbeddedMe=(-1);
  this->indexHcandidateBeingGrown=(-1);
  this->indexMaxSSContainer=(-1);
  this->indexSSPartCentralizer=-1;
  this->flagSystemSolved=(false);
  this->flagSystemProvedToHaveNoSolution=(false);
  this->flagSystemGroebnerBasisFound=(false);
  this->flagCentralizerIsWellChosen=(false);
  this->flagUsedInducingSubalgebraRealization=true;
  this->totalNumUnknownsNoCentralizer=(0);
  this->totalNumUnknownsWithCentralizer=(0);
  this->totalArithmeticOpsToSolveSystem=0;
  this->NumConeIntersections=(-1);
  this->NumCasesNoLinfiniteRelationFound=(-1);
  this->NumBadParabolics=(0);
  this->NumCentralizerConditionFailsConeConditionHolds=(0);
  this->centralizerRank=-1;
}

CandidateSSSubalgebra::CandidateSSSubalgebra()
{ this->flagDeallocated=false;
  this->reset(0);
}

void CandidateSSSubalgebra::ComputePairKweightElementAndModule(const ElementSemisimpleLieAlgebra<AlgebraicNumber>& leftKweightElt, int rightIndex, List<int>& output, GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputePairKweightElementAndModule");
  List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& rightModule=this->ModulesIsotypicallyMerged[rightIndex];
  ElementSemisimpleLieAlgebra<AlgebraicNumber> theLieBracket;
  ProgressReport theReport(theGlobalVariables);
  Vector<AlgebraicNumber> coordsInFullBasis;
  output.SetSize(0);
  if (this->fullBasisByModules.size!=this->GetAmbientSS().GetNumGenerators())
    crash << "This is a programming error: fullBasisByModules not computed when it should be. " << crash;
  for (int j=0; j<rightModule.size; j++)
  { this->GetAmbientSS().LieBracket(leftKweightElt, rightModule[j], theLieBracket);
    if (theGlobalVariables!=0)
    { std::stringstream reportStream;
      reportStream << "Bracketing index " << j+1 << " with input element. ";
      theReport.Report(reportStream.str());
    }
    bool tempbool=theLieBracket.GetCoordsInBasis(this->fullBasisByModules, coordsInFullBasis, *theGlobalVariables);
    if (!tempbool)
      crash << "This is a programming error: something has gone very wrong: my k-weight basis " << this->fullBasisByModules.ToString()
      << " does not contain " << theLieBracket.ToString() << crash;
    for (int i=0; i<coordsInFullBasis.size; i++)
      if (!coordsInFullBasis[i].IsEqualToZero())
        output.AddOnTopNoRepetition(this->fullBasisOwnerModules[i]);
  }
}

void CandidateSSSubalgebra::ComputeSinglePair(int leftIndex, int rightIndex, List<int>& output, GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputeSinglePair");
  output.SetSize(0);
  List<int> tempList;
  List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& leftModule=this->ModulesIsotypicallyMerged[leftIndex];
  ProgressReport theReport(theGlobalVariables);
  for (int i=0; i<leftModule.size; i++)
  { if (theGlobalVariables!=0)
    { std::stringstream reportStream;
      reportStream << "Bracketing element number " << i+1 << " out of " << leftModule.size << " with other module. ";
      theReport.Report(reportStream.str());
    }
    this->ComputePairKweightElementAndModule(leftModule[i], rightIndex, tempList, theGlobalVariables);
    output.AddOnTopNoRepetition(tempList);
  }
}

void CandidateSSSubalgebra::ComputePairingTable(GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputePairingTable");
  if (!this->flagCentralizerIsWellChosen)
    return;
  ProgressReport theReport(theGlobalVariables);
  this->NilradicalPairingTable.SetSize(this->ModulesIsotypicallyMerged.size);
  for (int i=0; i<this->NilradicalPairingTable.size; i++)
    this->NilradicalPairingTable[i].SetSize(this->ModulesIsotypicallyMerged.size);
  for (int i=0; i<this->NilradicalPairingTable.size; i++)
    for (int j=i; j<this->NilradicalPairingTable[i].size; j++)
    { if (theGlobalVariables!=0)
      { std::stringstream reportStream;
        reportStream << "Pairing indices " << i+1 << " and " << j+1 << " out of " << this->NilradicalPairingTable.size << ".";
        theReport.Report(reportStream.str());
      }
      this->ComputeSinglePair(i, j, this->NilradicalPairingTable[i][j], theGlobalVariables);
      if (j>i)
        this->NilradicalPairingTable[j][i]=this->NilradicalPairingTable[i][j];
    }
  this->modulesWithZeroWeights.Clear();
  for (int i=0; i<this->NilradicalPairingTable.size; i++)
    for (int j=0; j<this->ModulesIsotypicallyMerged[i].size; j++)
      if (this->GetAmbientSS().GetWeightOfGenerator(this->ModulesIsotypicallyMerged[i][j][0].theGeneratorIndex).IsEqualToZero())
      { this->modulesWithZeroWeights.AddOnTop(i);
        break;
      }
//  std::cout << "Nilradical pairing table size: " << this->NilradicalPairingTable.size;
  this->OppositeModulesByStructure.SetSize(this->NilradicalPairingTable.size);
  for (int i=0; i<this->NilradicalPairingTable.size; i++)
    for (int j=0; j<this->NilradicalPairingTable[i].size; j++)
      for (int k=0; k<this->NilradicalPairingTable[i][j].size; k++)
        if (this->modulesWithZeroWeights.Contains(this->NilradicalPairingTable[i][j][k]))
          if (!(this->primalSubalgebraModules.Contains(i) && this->primalSubalgebraModules.Contains(j)))
            this->OppositeModulesByStructure[i].AddOnTopNoRepetition(j);
  this->ComputeKsl2triples(theGlobalVariables);
}

void CandidateSSSubalgebra::ComputeCharsPrimalModules()
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputeCharsPrimalModules");
  this->CharsPrimalModules.SetSize(this->Modules.size);
  this->CharsPrimalModulesMerged.SetSize(this->Modules.size);
  MonomialChar<Rational> currentWeight;
  currentWeight.owner=0;
  for (int i=0; i<this->CharsPrimalModules.size; i++)
  { this->CharsPrimalModules[i].MakeZero();
    this->CharsPrimalModulesMerged[i].MakeZero();
    for (int j=0; j<this->WeightsModulesPrimal[i].size; j++)
    { currentWeight.weightFundamentalCoordS=this->WeightsModulesPrimal[i][j];
      this->CharsPrimalModules[i].AddMonomial(currentWeight, 1);
      this->CharsPrimalModulesMerged[i].AddMonomial(currentWeight, this->Modules[i].size);
    }
  }
  this->OppositeModulesByChar.SetSize(this->Modules.size);
  List<charSSAlgMod<Rational> > theDualMods;
  theDualMods.SetSize(this->Modules.size);
  for (int i=0; i<this->Modules.size; i++)
  { this->CharsPrimalModules[i].GetDual(theDualMods[i]);
    this->OppositeModulesByChar[i].SetSize(0);
  }
  for (int i=0; i<this->Modules.size; i++)
    for (int j=i; j<this->Modules.size; j++)
      if ((this->CharsPrimalModules[i]-theDualMods[j]).IsEqualToZero())
      { this->OppositeModulesByChar[i].AddOnTop(j);
        if (i!=j)
          this->OppositeModulesByChar[j].AddOnTop(i);
      } //else
        //std::cout << "<hr>" << this->CharsPrimalModules[i].ToString() << " - " << theDualMods[j].ToString()
        //<< "= " << (this->CharsPrimalModules[i]-theDualMods[j]).ToString();
}

void CandidateSSSubalgebra::ComputeKsl2triplesGetOppositeEltsInOppositeModule
(const Vector<Rational>& theElementWeight, const List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& inputOppositeModule, List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& outputElts)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputeKsl2triplesGetOppositeEltsInOppositeModule");
  outputElts.SetSize(0);
  for (int i=0; i<inputOppositeModule.size; i++)
    if ((this->GetPrimalWeightFirstGen(inputOppositeModule[i])+theElementWeight).IsEqualToZero())
      outputElts.AddOnTop(inputOppositeModule[i]);
}

Vector<Rational> CandidateSSSubalgebra::GetPrimalWeightFirstGen(const ElementSemisimpleLieAlgebra<AlgebraicNumber>& input)const
{ Vector<Rational> output;
  Vector<Rational> theRoot=this->GetAmbientSS().GetWeightOfGenerator(input[0].theGeneratorIndex);
  this->GetPrimalWeightProjectionFundCoords(theRoot, output);
  return output;
}

Vector<Rational> CandidateSSSubalgebra::GetNonPrimalWeightFirstGen(const ElementSemisimpleLieAlgebra<AlgebraicNumber>& input)const
{ Vector<Rational> output;
  Vector<Rational> theRoot=this->GetAmbientSS().GetWeightOfGenerator(input[0].theGeneratorIndex);
  this->GetWeightProjectionFundCoords(theRoot, output);
  return output;
}

void CandidateSSSubalgebra::ComputeKsl2triplesGetOppositeEltsAll(const Vector<Rational>& theElementWeight, List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& outputElts)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputeKsl2triplesGetOppositeEltsAll");
  Vector<Rational> otherWeight, tempV;
  outputElts.SetSize(0);
  for (int i=0; i<this->Modules.size; i++)
    for (int j=0; j<this->WeightsModulesPrimal[i].size; j++)
      if ((theElementWeight+this->WeightsModulesPrimal[i][j]).IsEqualToZero())
        for (int k=0; k<this->Modules[i].size; k++)
        { outputElts.AddOnTop(this->Modules[i][k][j]);
          if (!(theElementWeight+this->GetPrimalWeightFirstGen(this->Modules[i][k][j])).IsEqualToZero())
            crash << "This is a programming error: element this->Modules[i][k][j] does not have the primal weight it is supposed to have. " << crash;
        }
}

bool CandidateSSSubalgebra::ComputeKsl2tripleSetUpAndSolveSystem
(const ElementSemisimpleLieAlgebra<AlgebraicNumber>& theE, const List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& FisLinearCombiOf,
 ElementSemisimpleLieAlgebra<AlgebraicNumber>& outputF, GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputeKsl2tripleSetUpAndSolveSystem");
  ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> > Ecopy, theH, theF, tempElt;
  Ecopy=theE;
  this->GetAmbientSS().GetGenericElementCartan(theH, 0);
  theF.MakeZero();
  Polynomial<AlgebraicNumber> tempP;
  for (int i=0; i<FisLinearCombiOf.size; i++)
  { tempElt=FisLinearCombiOf[i];
    tempP.MakeMonomiaL(i+this->GetAmbientSS().GetRank(), 1, 1);
    tempElt*=tempP;
    theF+=tempElt;
  }
  if (!this->GetAmbientSS().AttempTFindingHEF(theH, Ecopy, theF, 0, theGlobalVariables))
    return false;
  outputF=theF;
  return true;
}

void CandidateSSSubalgebra::ComputeKsl2triples(GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputeKsl2triples");
  if (!this->owner->flagComputeNilradicals)
    return;
  this->ModulesSl2opposite.SetSize(this->Modules.size);
  List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > FmustBeAlinCombiOf;
  for (int i=0; i<this->Modules.size; i++)
  { this->ModulesSl2opposite[i].SetSize(this->Modules[i].size);
    for (int j=0; j<this->Modules[i].size; j++)
    { this->ModulesSl2opposite[i][j].SetSize(this->Modules[i][j].size);
      for (int k=0; k<this->ModulesSl2opposite[i][j].size; k++)
      { this->ComputeKsl2triplesGetOppositeEltsInOppositeModule(this->WeightsModulesPrimal[i][k], this->ModulesIsotypicallyMerged[this->OppositeModulesByChar[i][0]], FmustBeAlinCombiOf);
        if (this->ComputeKsl2tripleSetUpAndSolveSystem(this->Modules[i][j][k], FmustBeAlinCombiOf, this->ModulesSl2opposite[i][j][k], theGlobalVariables))
          continue;
        this->ComputeKsl2triplesGetOppositeEltsAll(this->WeightsModulesPrimal[i][k], FmustBeAlinCombiOf);
        this->ComputeKsl2tripleSetUpAndSolveSystem(this->Modules[i][j][k], FmustBeAlinCombiOf, this->ModulesSl2opposite[i][j][k], theGlobalVariables);
      }
    }
  }
}

int CandidateSSSubalgebra::GetRank()const
{ return this->theWeylNonEmbeddeD.theDynkinType.GetRank();
}

int CandidateSSSubalgebra::GetPrimalRank()const
{ if (!this->flagCentralizerIsWellChosen)
    return -1;
  int theRank=0;
  if (!this->centralizerRank.IsSmallInteger(&theRank))
    return -1;
  return theRank+this->theWeylNonEmbeddeD.GetDim();
}

void NilradicalCandidate::CheckInitialization()const
{ if (this->owner==0)
    crash << "This is a programming error: NilradicalCandidate with non-initialized owner" << crash;
}

Vector<Rational> NilradicalCandidate::GetConeStrongIntersectionWeight()const
{ Vector<Rational> theWeight;
  theWeight.MakeZero(this->owner->GetPrimalRank());
  for (int i=0; i<this->theNilradicalWeights.size; i++)
    theWeight+=this->theNilradicalWeights[i]*this->ConeStrongIntersection[i];
  return theWeight;
}

bool NilradicalCandidate::IsCommutingSelectionNilradicalElements(Selection& inputNilradSel)
{ if (inputNilradSel.CardinalitySelection==1)
    return true;
  ElementSemisimpleLieAlgebra<AlgebraicNumber> mustBeZero;
  for (int i=0; i<inputNilradSel.CardinalitySelection; i++)
  { ElementSemisimpleLieAlgebra<AlgebraicNumber>& leftElt=this->theNilradical[inputNilradSel.elements[i]];
    for (int j=0; j<inputNilradSel.CardinalitySelection; j++)
    { if (i==j)
        continue;
      ElementSemisimpleLieAlgebra<AlgebraicNumber>& rightEltPositive=this->theNilradical[inputNilradSel.elements[j]];
      ElementSemisimpleLieAlgebra<AlgebraicNumber>& rightEltNegative=this->theNilradicalElementOpposites[inputNilradSel.elements[j]];
      this->owner->GetAmbientSS().LieBracket(leftElt, rightEltPositive, mustBeZero);
      if (!mustBeZero.IsEqualToZero())
        return false;
      if (rightEltNegative.IsEqualToZero())
        return false;
      this->owner->GetAmbientSS().LieBracket(leftElt, rightEltNegative, mustBeZero);
      if (!mustBeZero.IsEqualToZero())
        return false;
    }
  }
  for (int i=0; i<inputNilradSel.CardinalitySelection; i++)
    for (int j=i+1; j<inputNilradSel.CardinalitySelection; j++)
      if (this->owner->GetScalarSA(this->theNilradicalWeights[inputNilradSel.elements[i]], this->theNilradicalWeights[inputNilradSel.elements[j]])<0)
      { /*crash << "<br>This is either a programming error, or I am missing some mathematical phenomenon: k-sl(2)-triples are "
        << "strongly orthogonal, but their k-weights aren't. Crashing to tactfully let you know. "
        << "The bad elements are: "
        << this->theNilradical[inputNilradSel.elements[i]].ToString() << " of weight "
        << this->theNilradicalWeights[inputNilradSel.elements[i]].ToString()
        << " and "
        << this->theNilradical[inputNilradSel.elements[j]].ToString() << " of weight "
        << this->theNilradicalWeights[inputNilradSel.elements[j]].ToString() << ". "
        //<< "The bilinear form is: " << this->owner->BilinearFormFundPrimal.ToString() << ". "
        //<< " and the subalgebra in play is: " << this->owner->ToString() << ". "
        //<< crash;
        */
        return false;
      }
  return true;
}

Vector<Rational> NilradicalCandidate::GetNilradicalLinearCombi()const
{ Vector<Rational> theNilradLinCombi=this->ConeStrongIntersection;
  theNilradLinCombi.SetSize(this->theNilradicalWeights.size);
  return theNilradLinCombi;
}

void NilradicalCandidate::ComputeParabolicACextendsToParabolicAC(GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("NilradicalCandidate::ComputeParabolicACextendsToParabolicAC");
  Vector<Rational> projectionRoot;
  WeylGroup& theWeyl=this->owner->owner->owneR->theWeyl;
  this->leviRootsAmbienT.ReservE(theWeyl.RootSystem.size);
  this->leviRootsSmallPrimalFundCoords.ReservE(theWeyl.RootSystem.size);
//  std::cout << "<hr>this->ConeSeparatingNormal: " << this->ConeSeparatingNormal.ToString();
 // if (this->ConeSeparatingNormal.size==0)
  //{ std::cout << this->owner->theWeylNonEmbeddeD.theDynkinType.ToString()
  //  << "<br>nilrad:  " << this->theNilradicalWeights.ToString() << "<br>" << "non nilrad: "
  //  << this->theNonFKhws.ToString();
  //  std::cout << "<br>separating normal: " << this->ConeSeparatingNormal.ToString();
  //}
//  Vectors<Rational> thePosWeights;
//  thePosWeights.AddListOnTop(this->theNilradicalWeights);
//  thePosWeights.AddListOnTop(this->owner->RootSystemSubalgebraPrimalCoords);
  //{ std::cout << this->owner->theWeylNonEmbeddeD.theDynkinType.ToString()
  //  << "<br>nilrad:  " << this->theNilradicalWeights.ToString() << "<br>" << "non nilrad: "
  //  << this->theNonFKhws.ToString();
  //  std::cout << "<br>separating normal: " << this->ConeSeparatingNormal.ToString();
  //  std::cout << "<br> this->owner->RootSystemSubalgebraPrimalCoords:" << this->owner->RootSystemSubalgebraPrimalCoords.ToString();
  //  std::cout << "<br>The pos weights: " << thePosWeights.ToString();
  //}
  Vectors<Rational> rootSystemProjections;
  Vectors<Rational> conesCombination;
  rootSystemProjections.SetSize(theWeyl.RootSystem.size);
  for (int i=0; i<theWeyl.RootSystem.size; i++)
    this->owner->GetPrimalWeightProjectionFundCoords(theWeyl.RootSystem[i], rootSystemProjections[i]);
  conesCombination=this->theNilradicalWeights;
  for (int i=0; i<this->theNonFKhws.size; i++)
    conesCombination.AddOnTop(-this->theNonFKhws[i]);
  this->ConeSeparatingNormal.PerturbNormalRelativeToVectorsInGeneralPosition(conesCombination, rootSystemProjections);
  for (int i=0; i<theWeyl.RootSystem.size; i++)
  { this->owner->GetPrimalWeightProjectionFundCoords(theWeyl.RootSystem[i], projectionRoot);
    if (projectionRoot.ScalarEuclidean(this->ConeSeparatingNormal)==0)
    { this->leviRootsAmbienT.AddOnTop(theWeyl.RootSystem[i]);
      if (this->owner->RootSystemCentralizerPrimalCoords.Contains(projectionRoot))
        this->leviRootsSmallPrimalFundCoords.AddOnTop(projectionRoot);
    }
  }
  Vectors<Rational> tempVs=this->leviRootsAmbienT;
  this->theLeviDiagramAmbienT.ComputeDiagramTypeModifyInput(tempVs, theWeyl);
//  std::cout << "<br>levi roots small: " << this->leviRootsSmallPrimalFundCoords.ToString();
  this->theLeviDiagramSmalL.ComputeDiagramTypeModifyInputRelative
  (this->leviRootsSmallPrimalFundCoords, this->owner->RootSystemCentralizerPrimalCoords, this->owner->BilinearFormFundPrimal);
  bool ambientLeviHasBDGE=false;
  for (int i=0; i<this->theLeviDiagramAmbienT.SimpleComponentTypes.size; i++)
  { DynkinSimpleType& currentType=this->theLeviDiagramAmbienT.SimpleComponentTypes[i];
    if (currentType.theLetter=='D' || currentType.theLetter=='G' || currentType.theLetter=='E' || currentType.theLetter=='E')
    { ambientLeviHasBDGE=true;
      break;
    }
    if (currentType.theLetter=='B' && currentType.theRank>2)
    { ambientLeviHasBDGE=true;
      break;
    }
  }
  bool smallLeviHasBDGE=false;
  for (int i=0; i<this->theLeviDiagramSmalL.SimpleComponentTypes.size; i++)
  { DynkinSimpleType& currentType=this->theLeviDiagramSmalL.SimpleComponentTypes[i];
    if (currentType.theLetter=='D' || currentType.theLetter=='G' || currentType.theLetter=='E' || currentType.theLetter=='E')
    { smallLeviHasBDGE=true;
      break;
    }
    if (currentType.theLetter=='B' && currentType.theRank>2)
    { smallLeviHasBDGE=true;
      break;
    }
  }
  this->flagRestrictedCentralizerConditionHoldS=!smallLeviHasBDGE;
  if (!smallLeviHasBDGE)
    this->flagParabolicACextendsToParabolicAC=!ambientLeviHasBDGE;
  if (smallLeviHasBDGE)
  { this->flagParabolicACextendsToParabolicAC=true;
    if (!ambientLeviHasBDGE)
    { crash << "This is a mathematical error. Something is very wrong. The ambient parabolic subalgebra has components "
      << " of type A and C, but intesects the centralizer in components of type B and D. This must be impossible according to "
      << "the PSZ paper and the restriction of Fernando's theorem to the centralizer. " << crash;
    }
  }
}

bool NilradicalCandidate::TryFindingLInfiniteRels(GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("NilradicalCandidate::TryFindingLInfiniteRels");
  //Vector<Rational> theNilradLinCombi=this->GetNilradicalLinearCombi();
  //if(theNilradLinCombi.GetNumNonZeroCoords()==1)
  //  return true;
//  Vectors<Rational> curentNilradicalCone;
  Vector<Rational> betterIntersection;
  this->theNilradSubsel.init(this->theNilradicalWeights.size);
  this->flagComputedRelativelyStrongIntersections=false;
  for (int i=1; i<this->theNilradicalWeights.size && i<5; i++)
  { int numcycles=MathRoutines::NChooseK(this->theNilradicalWeights.size, i);
    this->theNilradSubsel.initSelectionFixedCardinality(i);
    for (int j=0; j<numcycles; j++, this->theNilradSubsel.incrementSelectionFixedCardinality(i))
      if (this->IsCommutingSelectionNilradicalElements(this->theNilradSubsel))
      { this->theNilradicalWeights.SubSelection(this->theNilradSubsel, this->theNilradicalSubsetWeights);
        if (this->theNilradicalSubsetWeights.ConesIntersect(this->theNilradicalSubsetWeights, this->theNonFKhwsStronglyTwoSided, &betterIntersection, 0, theGlobalVariables))
        { betterIntersection.ScaleToIntegralMinHeightFirstNonZeroCoordinatePositive();
          this->ConeStrongIntersection.MakeZero(this->theNilradicalWeights.size+this->theNonFKhwsStronglyTwoSided.size);
          this->ConeRelativelyStrongIntersection.SetSize(0);
          for (int k=0; k<this->theNilradSubsel.CardinalitySelection; k++)
          { this->ConeStrongIntersection[this->theNilradSubsel.elements[k]]=betterIntersection[k];
            this->ConeRelativelyStrongIntersection.AddOnTop(betterIntersection[k]);
          }
          this->theNilradical.SubSelection(this->theNilradSubsel, this->theNilradicalSubset);
          this->theNilradicalWeights.SubSelection(this->theNilradSubsel, this->theNilradicalSubsetWeights);
          this->theNonFKhwVectorsStrongRelativeToSubset.SetSize(0);
          this->theNonFKhwVectorsStrongRelativeToSubsetWeights.SetSize(0);
          for (int k=0; k<this->theNonFKhwsStronglyTwoSided.size; k++)
          { this->ConeStrongIntersection[k+this->theNilradicalWeights.size]=betterIntersection[k+i];
            if (betterIntersection[k+i]!=0)
            { this->theNonFKhwVectorsStrongRelativeToSubset.AddOnTop(this->theNonFKHVectorsStronglyTwoSided[k]);
              this->theNonFKhwVectorsStrongRelativeToSubsetWeights.AddOnTop(this->theNonFKhwsStronglyTwoSided[k]);
              this->ConeRelativelyStrongIntersection.AddOnTop(betterIntersection[k+i]);
            }
          }
//          this->LInfiniteRelation.MakeZero(this->owner->GetPrimalRank());
//          for (int k=0; k<this->theNilradicalWeights.size; k++)
//            this->LInfiniteRelation+=this->theNilradicalWeights[k]*this->ConeStrongIntersection[k];
          return true;
        }
      }
  }
//  this->ConeStrongIntersection.MakeZero(this->ConeStrongIntersection.size);
  ProgressReport theReport(theGlobalVariables);
  this->flagComputedRelativelyStrongIntersections=true;
  for (int i=1; i<this->theNilradicalWeights.size+1 && i<5; i++)
  { int numcycles=MathRoutines::NChooseK(this->theNilradicalWeights.size, i);
    this->theNilradSubsel.initSelectionFixedCardinality(i);
//    this->FKnilradicalLog+=out.str();
    for (int j=0; j<numcycles; j++, this->theNilradSubsel.incrementSelectionFixedCardinality(i))
    { std::stringstream out;
      out << "<br>Trying " << i+1 << "-tuples (up to 5-tuples): " << j+1 << " out of " << numcycles << " cycles to process. ";
      theReport.Report(out.str());
      if (this->IsCommutingSelectionNilradicalElements(this->theNilradSubsel))
      { this->ComputeTheTwoConesRelativeToNilradicalSubset();
        if (this->theNilradicalSubsetWeights.ConesIntersect(this->theNilradicalSubsetWeights, this->theNonFKhwVectorsStrongRelativeToSubsetWeights, &this->ConeRelativelyStrongIntersection, 0, theGlobalVariables))
        { this->ConeRelativelyStrongIntersection.ScaleToIntegralMinHeightFirstNonZeroCoordinatePositive();
          return true;
        } else
          this->FKnilradicalLog+="... but the cones dont intersect. ";
      }
    }
  }
  return false;
}

bool NilradicalCandidate::IsStronglySingular(int moduleIndex)
{ this->CheckInitialization();
  for (int i=0; i<this->theNilradicalSelection.size; i++)
    if (this->theNilradicalSelection[i]==1 && !this->owner->primalSubalgebraModules.Contains(i))
      for (int j=0; j<this->owner->NilradicalPairingTable[moduleIndex][i].size; j++)
      { if (this->theNilradicalSelection[this->owner->NilradicalPairingTable[moduleIndex][i][j]]!=1)
          return false;
        if (this->owner->primalSubalgebraModules.Contains(this->owner->NilradicalPairingTable[moduleIndex][i][j]))
          return false;
      }
  return true;
}

bool NilradicalCandidate::IsStronglySingularRelativeToSubset(int nonFKweightIndex)
{ MacroRegisterFunctionWithName("NilradicalCandidate::IsStronglySingularRelativeToSubset");
  this->CheckInitialization();
//  std::stringstream out;
//  out << "<br>Checking out module index " << moduleIndex;
  //this->FKnilradicalLog+=out.str();
  ElementSemisimpleLieAlgebra<AlgebraicNumber> mustBeZero;
  for (int j=0; j<this->theNilradSubsel.CardinalitySelection; j++)
  { this->owner->owner->owneR->LieBracket(this->theNilradical[this->theNilradSubsel.elements[j]], this->theNonFKhwVectors[nonFKweightIndex], mustBeZero);
    if (!mustBeZero.IsEqualToZero())
      return false;
  }
//  this->FKnilradicalLog+= "->IS GOOD";
  return true;
}

void NilradicalCandidate::reset()
{ this->theNilradicalWeights.SetSize(0);
  this->theNonFKhws.SetSize(0);
  this->theNonFKhwsStronglyTwoSided.SetSize(0);
  this->theNonFKHVectorsStronglyTwoSided.SetSize(0);
  this->theNilradical.SetSize(0);
  this->theNilradicalElementOpposites.SetSize(0);
  this->ownerModulesNilradicalElements.SetSize(0);
  this->ownerModulestheNonFKhwVectors.SetSize(0);
  this->theNilradSubsel.init(0);
  this->theNilradicalSubset.SetSize(0);
  this->theNonFKhwVectorsStrongRelativeToSubset.SetSize(0);
  this->theNilradicalSubsetWeights.SetSize(0);
  this->theNonFKhwVectorsStrongRelativeToSubsetWeights.SetSize(0);
}

void NilradicalCandidate::ComputeTheTwoConesRelativeToNilradicalSubset()
{ MacroRegisterFunctionWithName("NilradicalCandidate::ComputeTheTwoConesRelativeToNilradicalSubset");
  this->theNilradical.SubSelection(this->theNilradSubsel, this->theNilradicalSubset);
  this->theNilradicalWeights.SubSelection(this->theNilradSubsel, this->theNilradicalSubsetWeights);
  this->theNonFKhwVectorsStrongRelativeToSubset.SetSize(0);
  this->theNonFKhwVectorsStrongRelativeToSubsetWeights.SetSize(0);
  for (int i=0; i<this->theNonFKhws.size; i++)
    if (this->IsStronglySingularRelativeToSubset(i))
    { this->theNonFKhwVectorsStrongRelativeToSubsetWeights.AddOnTop(this->theNonFKhws[i]);
      this->theNonFKhwVectorsStrongRelativeToSubset.AddOnTop(this->theNonFKhwVectors[i]);
    }
//  std::stringstream out;
//  out << "<hr>Computed the two cones relative to the subset " << this->theNilradSubsel.ToString();
//  out << " to get the theNonFKhwVectorsStrongRelativeToSubset: "
//  << this->ToStringTableElementWithWeights(this->theNonFKhwVectorsStrongRelativeToSubset, this->theNonFKhwVectorsStrongRelativeToSubsetWeights);
//  this->FKnilradicalLog+=out.str();
}

void NilradicalCandidate::ComputeTheTwoCones(GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("NilradicalCandidate::ComputeTheTwoCones");
  this->CheckInitialization();
  this->reset();
//  std::stringstream out;
  for (int i=0; i<this->theNilradicalSelection.size; i++)
    if (!this->owner->primalSubalgebraModules.Contains(i))
    { if (this->theNilradicalSelection[i]==0)
        for (int k=0; k<this->owner->Modules[i].size; k++)
        { this->theNonFKhws.AddOnTop(this->owner->HighestWeightsPrimal[i]);
          this->theNonFKhwVectors.AddOnTop(this->owner->HighestVectors[i][k]);
          this->ownerModulestheNonFKhwVectors.AddOnTop(i);
          if (this->IsStronglySingular(i))
          { this->theNonFKhwsStronglyTwoSided.AddOnTop(this->owner->HighestWeightsPrimal[i]);
            this->theNonFKHVectorsStronglyTwoSided.AddOnTop(this->owner->HighestVectors[i][k]);
          }
        }
      else if (this->theNilradicalSelection[i]==1)
        for (int k=0; k<this->owner->Modules[i].size; k++)
        { this->theNilradicalWeights.AddListOnTop(this->owner->WeightsModulesPrimal[i]);
          this->theNilradical.AddListOnTop(this->owner->Modules[i][k]);
          this->theNilradicalElementOpposites.AddListOnTop(this->owner->ModulesSl2opposite[i][k]);
          for (int l=0; l<this->owner->Modules[i][k].size; l++)
            this->ownerModulesNilradicalElements.AddOnTop(i);
        }
    }
  if (this->ownerModulesNilradicalElements.size!=this->theNilradical.size || this->theNonFKhws.size!=this->ownerModulestheNonFKhwVectors.size)
    crash << "This is a programming error: sizes of indexing arrasy in Fernando Kac nilradical candidate don't match. " << crash;
//  out << "<br>";
//  this->FKnilradicalLogs[inputFKIndex]=out.str();
}

void CandidateSSSubalgebra::EnumerateAllNilradicals(GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::EnumerateAllNilradicals");
  ProgressReport theReport(theGlobalVariables);
  ProgressReport theReport2(theGlobalVariables);

  std::stringstream reportStream;
  reportStream << "Enumerating recursively nilradicals of type " << this->ToStringTypeAndHs() << "...";
  theReport.Report(reportStream.str());
  //std::cout << reportStream.str();
  this->FKNilradicalCandidates.SetSize(0);
  List<int> theSel;
  this->RecursionDepthCounterForNilradicalGeneration=0;
  //0 stands for not selected, 1 for selected from nilradical, 3 for selected from semisimple part, 2 stands for unknown.
  theSel.initFillInObject(this->NilradicalPairingTable.size, 2);
  for(int i=0; i<this->primalSubalgebraModules.size; i++)
    theSel[this->primalSubalgebraModules[i]]=1;
  std::stringstream out;
  if (theSel.size!=this->NilradicalPairingTable.size || theSel.size!=this->ModulesIsotypicallyMerged.size)
    crash << "This is a programming error: selection has " << theSel.size << ", nilraidcal pairing table has " << this->NilradicalPairingTable.size
    << " elements and modules isotypically merged has " << this->ModulesIsotypicallyMerged.size << " elements." << crash;
  this->EnumerateNilradicalsRecursively(theSel, theGlobalVariables, &out);
  if (this->FKNilradicalCandidates.size<1)
    crash << "This is a programming error:" << " while enumerating nilradicals of "
    << this->theWeylNonEmbeddeD.theDynkinType.ToStringRelativeToAmbientType(this->GetAmbientWeyl().theDynkinType[0])
    << " got 0 nilradical candidates which is impossible (the zero nilradical is always possible). " << crash;
  for (int i=0; i<this->FKNilradicalCandidates.size; i++)
  { std::stringstream reportStream2;
    reportStream2 << "Processing nilradical: " << i+1 << " out of " << this->FKNilradicalCandidates.size;
    theReport2.Report(reportStream2.str());
    this->FKNilradicalCandidates[i].ProcessMe(theGlobalVariables);
  }
  this->NumConeIntersections=0;
  this->NumCasesNoLinfiniteRelationFound=0;
  this->NumBadParabolics=0;
  this->NumCentralizerConditionFailsConeConditionHolds=0;
  for (int i=0; i<this->FKNilradicalCandidates.size; i++)
    if (this->FKNilradicalCandidates[i].flagNilradicalConesIntersect)
    { this->NumConeIntersections++;
      if (!this->FKNilradicalCandidates[i].flagLinfiniteRelFound)
        this->NumCasesNoLinfiniteRelationFound++;
    } else
    { if (!this->FKNilradicalCandidates[i].flagParabolicACextendsToParabolicAC)
        this->NumBadParabolics++;
      if (!this->FKNilradicalCandidates[i].flagRestrictedCentralizerConditionHoldS)
        this->NumCentralizerConditionFailsConeConditionHolds++;
    }
//  this->nilradicalGenerationLog=out.str();
  //std::cout << out.str();
}

void Cone::GetLinesContainedInCone(Vectors<Rational>& output)
{ output.SetSize(0);
  for (int i=0; i<this->Vertices.size; i++)
    for (int j=i+1; j<this->Vertices.size; j++)
      if (this->Vertices[i]==-this->Vertices[j])
        output.AddOnTop(this->Vertices[i]);
}

template<class coefficient>
void Vector<coefficient>::PerturbNormalRelativeToVectorsInGeneralPosition(const Vectors<Rational>& NonStrictConeNonPositiveScalar, const List<Vector<Rational> >& VectorsToBeInGeneralPosition)
{ MacroRegisterFunctionWithName("Vectors::PerturbSplittingNormal");
  for (int i=0; i<NonStrictConeNonPositiveScalar.size; i++)
    if (this->ScalarEuclidean(NonStrictConeNonPositiveScalar[i])<0)
      crash << "This is a programming error: the splitting normal " << this->ToString() << " is supposed to have non-negative scalar product with the vector "
      << NonStrictConeNonPositiveScalar[i].ToString() << ", but it doesn't." << crash;
  Vector<Rational> oldThis=*this;
  Cone theCone, orthognalCone;
  theCone.CreateFromVertices(NonStrictConeNonPositiveScalar);
//  std::cout << "<br>Cone normals: " << theCone.Normals.ToString();
  coefficient scalarThis;
  coefficient scalarOther;
  coefficient theScale;
  Vector<Rational> currentModifier;
  Vectors<Rational> allVectors=theCone.Vertices;
  allVectors.AddListOnTop(VectorsToBeInGeneralPosition);
  for (int i=0; i<VectorsToBeInGeneralPosition.size; i++)
    if (this->ScalarEuclidean(VectorsToBeInGeneralPosition[i])==0)
    { bool foundModifier=false;
      for (int j=0; j<theCone.Normals.size; j++)
        if (theCone.Normals[j].ScalarEuclidean(VectorsToBeInGeneralPosition[i])!=0)
        { foundModifier=true;
          currentModifier=theCone.Normals[j];
          break;
        }
      if (!foundModifier)
        continue;
      theScale=1;
      for (int j=0; j<VectorsToBeInGeneralPosition.size; j++)
      { scalarThis= this->ScalarEuclidean(VectorsToBeInGeneralPosition[j]);
        scalarOther= currentModifier.ScalarEuclidean(VectorsToBeInGeneralPosition[j]);
        if (scalarOther*scalarThis<0)
          theScale=MathRoutines::Minimum(theScale, -(scalarThis/scalarOther)/2);
      }
//    std::cout << ( mustGoNegative ? "<br>must go negative" : "<br> must go positive");
//    std::cout << "<br>currentModifier: " << currentModifier.ToString() << " * " << theScale.ToString() << "; *this="
//    << this->ToString();
    *this+=currentModifier*theScale;
//    std::cout << "<br> *this=" << this->ToString();
    for (int i=0; i<NonStrictConeNonPositiveScalar.size; i++)
      if (this->ScalarEuclidean(NonStrictConeNonPositiveScalar[i])<0)
        crash << "<br>This is a programming error: during perturbation, the splitting normal " << this->ToString()
        << " is supposed to have non-negative scalar product with the vector " << NonStrictConeNonPositiveScalar[i].ToString()
        << ", but it doesn't." << crash;
      else
      if (this->ScalarEuclidean(NonStrictConeNonPositiveScalar[i])==0 && oldThis.ScalarEuclidean(NonStrictConeNonPositiveScalar[i])>0)
        crash << "<br>This is a programming error: during perturbation, the splitting normal " << this->ToString()
        << " lost  positive scalar product with " << NonStrictConeNonPositiveScalar[i].ToString() << "." << crash;
    }
//  return true;
}

void NilradicalCandidate::ProcessMe(GlobalVariables* theGlobalVariables)
{ this->CheckInitialization();
  this->flagComputedRelativelyStrongIntersections=false;
  this->flagRestrictedCentralizerConditionHoldS=false;
  this->flagParabolicACextendsToParabolicAC=true;
  this->ComputeTheTwoCones(theGlobalVariables);
  this->flagNilradicalConesIntersect=this->theNilradicalWeights.ConesIntersect
  (this->theNilradicalWeights, this->theNonFKhws, &this->ConeIntersection, &this->ConeSeparatingNormal, theGlobalVariables);
  this->flagLinfiniteRelFound=false;
  if (!this->flagNilradicalConesIntersect)
  { if (this->theNonFKhws.size==0 && this->theNilradicalWeights.size==0)
    { this->flagRestrictedCentralizerConditionHoldS=true;
      this->flagParabolicACextendsToParabolicAC=true;
      return;
    }
    this->ComputeParabolicACextendsToParabolicAC(theGlobalVariables);
    return;
  }
  this->flagNilradicalConesStronglyIntersect=this->theNilradicalWeights.ConesIntersect
  (this->theNilradicalWeights, this->theNonFKhwsStronglyTwoSided, &this->ConeStrongIntersection, 0, theGlobalVariables);
  if (this->flagNilradicalConesStronglyIntersect)
    this->flagLinfiniteRelFound= this->TryFindingLInfiniteRels(theGlobalVariables);
}

std::string CandidateSSSubalgebra::ToStringNilradicalSelection(const List<int>& theSelection)
{ Vector<Rational> undecided, included, excluded, tempV;
  included.MakeZero(this->NilradicalPairingTable.size);
  undecided.MakeZero(this->NilradicalPairingTable.size);
  excluded.MakeZero(this->NilradicalPairingTable.size);
  for (int i=0; i<theSelection.size; i++)
  { tempV.MakeEi(this->NilradicalPairingTable.size, i);
    if (theSelection[i]==0)
      excluded+=tempV;
    else if(theSelection[i]==1)
      included+=tempV;
    else
      undecided+=tempV;
  }
  std::stringstream out;
  out << "included modules: " << included.ToStringLetterFormat("V") << ", excluded modules: " << excluded.ToStringLetterFormat("V")
  << ", undecided modulues: " << undecided.ToStringLetterFormat("V");
  return out.str();
}

bool CandidateSSSubalgebra::IsPossibleNilradicalCarryOutSelectionImplications(List<int>& theSelection, GlobalVariables* theGlobalVariables, std::stringstream* logStream)
{ if (this->FKNilradicalCandidates.size>100)
  { if (logStream!=0)
    { std::string tempS=logStream->str();
      if (tempS.size()>2)
        if (tempS[tempS.size()-1]!='.' || tempS[tempS.size()-2]!='.')
          *logStream << "<br>... number of nilradicals found exceeds 100, I will no longer log the computations ...";
    }
    logStream=0;
  }
  if (logStream!=0)
    *logStream << "<br>********Considering selection " << this->ToStringNilradicalSelection(theSelection) << "********";
  HashedList<int, MathRoutines::IntUnsignIdentity> selectedIndices;
  for (int i=0; i<theSelection.size; i++)
    if (theSelection[i]==1)
      selectedIndices.AddOnTop(i);
  MemorySaving<List<int> > oldSelection;
  if (logStream!=0)
    oldSelection.GetElement()=theSelection;
  this->ExtendNilradicalSelectionToMultFreeOverSSpartSubalgebra(selectedIndices, theGlobalVariables, logStream);
  for (int i=0; i<theSelection.size; i++)
    if (theSelection[i]==0 && selectedIndices.Contains(i))
    { if (logStream!=0)
      { *logStream << " <br>The selection " << this->ToStringNilradicalSelection(theSelection)
        << " is contradictory, as the only way to extend it to a subalgebra (i.e., closed under Lie bracket) is by requesting that module V_{"
        << i+1 << "} be included, but at the same time we have already decided to exclude that module in one of our preceding choices. ";
      }
      return false;
    }
  for (int i=0; i<selectedIndices.size; i++)
    theSelection[selectedIndices[i]]=1;
  if (logStream!=0)
    if (oldSelection.GetElement()!=theSelection)
    { *logStream << "<br>In order to be closed w.r.t. the Lie bracket, I extend the nilradical selection "
      << this->ToStringNilradicalSelection(oldSelection.GetElement()) << " to the following. "
      << this->ToStringNilradicalSelection(theSelection);
    }
  for (int i=0; i<selectedIndices.size; i++)
    for (int j=0; j<this->OppositeModulesByStructure[selectedIndices[i]].size; j++)
    { if (theSelection[this->OppositeModulesByStructure[selectedIndices[i]][j]]==1)
      { if (logStream!=0)
        { *logStream << "<br>The subalgebra selection " << this->ToStringNilradicalSelection(theSelection) << " contains opposite modules and is therefore not allowed. ";
        }
        return false;
      }
      theSelection[this->OppositeModulesByStructure[selectedIndices[i]][j]]=0;
    }
  return true;
}

void CandidateSSSubalgebra::ExtendNilradicalSelectionToMultFreeOverSSpartSubalgebra
(HashedList<int, MathRoutines::IntUnsignIdentity>& inputOutput, GlobalVariables* theGlobalVariables, std::stringstream* logStream)
{ for (int i=0; i<inputOutput.size; i++)
    for (int j=0; j<inputOutput.size; j++)
      for (int k=0; k<this->NilradicalPairingTable[inputOutput[i] ][inputOutput[j]].size; k++)
        inputOutput.AddOnTopNoRepetition(this->NilradicalPairingTable[inputOutput[i] ][inputOutput[j]][k]);
}

void CandidateSSSubalgebra::EnumerateNilradicalsRecursively(List<int>& theSelection, GlobalVariables* theGlobalVariables, std::stringstream* logStream)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::EnumerateNilradicalsRecursively");
  RecursionDepthCounter theCounter(&this->RecursionDepthCounterForNilradicalGeneration);
  if (this->RecursionDepthCounterForNilradicalGeneration>this->NilradicalPairingTable.size+1)
    crash << "<br>oh no... something went very wrong! the nilradical generation recursion depth cannot exceed the number of nilradicals! "
    << crash;
  ProgressReport theReport(theGlobalVariables);
  if (theGlobalVariables!=0)
  { std::stringstream out;
    out << "Enumerating nilradicals: " << this->FKNilradicalCandidates.size << " found so far. ";
    theReport.Report(out.str());
  }
  if (!this->IsPossibleNilradicalCarryOutSelectionImplications(theSelection, theGlobalVariables, logStream))
    return;
  List<int> newSelection;
  bool found=false;
  for (int i=0; i<theSelection.size; i++)
    if (theSelection[i]==2)
    { found=true;
      newSelection=theSelection;
      newSelection[i]=0;
      this->EnumerateNilradicalsRecursively(newSelection, theGlobalVariables, logStream);
      newSelection=theSelection;
      newSelection[i]=1;
      this->EnumerateNilradicalsRecursively(newSelection, theGlobalVariables, logStream);
      break;
      //newSelection
    }
  if (found)
    return;
  NilradicalCandidate tempCandidate;
  tempCandidate.owner=this;
  tempCandidate.theNilradicalSelection=theSelection;
  this->FKNilradicalCandidates.AddOnTop(tempCandidate);
}

void CandidateSSSubalgebra::ComputePrimalModuleDecompositionHighestWeightsOnly(GlobalVariables* theGlobalVariables, HashedList<Vector<Rational> >& outputHWsDualCoords)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputePrimalModuleDecompositionHighestWeightsOnly");
  outputHWsDualCoords.Clear();
  Vector<Rational> currentWeight, currentRootSpace;
  for (int i=0; i<this->HighestVectorsNonSorted.size; i++)
  { ElementSemisimpleLieAlgebra<AlgebraicNumber>& currentVector=this->HighestVectorsNonSorted[i];
    for (int j=0; j<currentVector.size(); j++)
    { currentRootSpace= this->GetAmbientSS().GetWeightOfGenerator(currentVector[j].theGeneratorIndex);
      currentWeight.SetSize(this->theHs.size+ this->CartanOfCentralizer.size);
      for (int k=0; k<this->theHs.size; k++)
        currentWeight[k]=this->GetAmbientWeyl().RootScalarCartanRoot(currentRootSpace, this->theHs[k]);
      for (int k=0; k<this->CartanOfCentralizer.size; k++)
        currentWeight[k+this->theHs.size]=this->GetAmbientWeyl().RootScalarCartanRoot(currentRootSpace, this->CartanOfCentralizer[k]);
      outputHWsDualCoords.AddOnTopNoRepetition(currentWeight);
    }
  }
  outputHWsDualCoords.QuickSortAscending();
}

void CandidateSSSubalgebra::ComputePrimalModuleDecompositionHWsHWVsOnly(GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputePrimalModuleDecompositionHWsHWVsOnly");
  HashedList<Vector<Rational> > theWeightsCartanRestrictedDualCoords;
  this->ComputePrimalModuleDecompositionHighestWeightsOnly(theGlobalVariables, theWeightsCartanRestrictedDualCoords);
  this->ComputePrimalModuleDecompositionHWVsOnly(theGlobalVariables, theWeightsCartanRestrictedDualCoords);
  this->ComputePrimalModuleDecompositionHWsHWVsOnlyLastPart(theGlobalVariables);
}

bool CandidateSSSubalgebra::CompareLeftGreaterThanRight(const Vector<Rational>& left, const Vector<Rational>& right)
{ Vector<Rational> leftSSpart=left;
  Vector<Rational> rightSSpart=right;
  leftSSpart.SetSize(this->theHs.size);
  rightSSpart.SetSize(this->theHs.size);
  if (leftSSpart>rightSSpart)
    return true;
  if (rightSSpart>leftSSpart)
    return false;
  Vector<Rational> leftCpart=left;
  Vector<Rational> rightCpart=right;
  leftCpart.ShiftToTheLeft(this->theHs.size);
  rightCpart.ShiftToTheLeft(this->theHs.size);
  return leftCpart>rightCpart;
}

void CandidateSSSubalgebra::GetWeightProjectionFundCoords(const Vector<Rational>& inputAmbientweight, Vector<Rational>& output)const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::GetWeightProjectionFundCoords");
  output.SetSize(this->theHs.size);
  for (int j=0; j<this->theHs.size; j++)
    output[j]= this->GetAmbientWeyl().RootScalarCartanRoot(inputAmbientweight, this->theHs[j])*2/this->theWeylNonEmbeddeDdefaultScale.CartanSymmetric(j,j);
}

void CandidateSSSubalgebra::GetPrimalWeightProjectionFundCoords(const Vector<Rational>& inputAmbientweight, Vector<Rational>& output)const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::GetPrimalWeightProjectionFundCoords");
  output.SetSize(this->theHs.size+this->CartanOfCentralizer.size);
  for (int j=0; j<this->theHs.size; j++)
    output[j]= this->GetAmbientWeyl().RootScalarCartanRoot(inputAmbientweight, this->theHs[j])*2/this->theWeylNonEmbeddeDdefaultScale.CartanSymmetric(j,j);
  for (int j=0; j<this->CartanOfCentralizer.size; j++)
    output[j+this->theHs.size]=this->GetAmbientWeyl().RootScalarCartanRoot(inputAmbientweight, this->CartanOfCentralizer[j])*2;
}

void CandidateSSSubalgebra::ComputePrimalModuleDecompositionHWsHWVsOnlyLastPart(GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputePrimalModuleDecompositionHWsHWVsOnlyLastPart");
  this->HighestWeightsPrimalNonSorted.SetSize(this->HighestVectorsNonSorted.size);
  this->HighestWeightsNONprimalNonSorted.SetSize(this->HighestVectorsNonSorted.size);
  this->thePrimalChaR.MakeZero();
  MonomialChar<Rational> theWeight;
  theWeight.owner=0;
  Vector<Rational> currentRoot;
  //std::cout << this->theWeylNonEmbeddeD.theDynkinType.ToString();
  //if (this->theWeylNonEmbeddeD.theDynkinType.ToString()=="A^{56/3}_1")
  //{ std::cout << "here be probs!";
  //}
  List<Vector<Rational> > sortingWeights;
  for (int i=0; i<this->HighestVectorsNonSorted.size; i++)
  { currentRoot=this->GetAmbientSS().GetWeightOfGenerator(this->HighestVectorsNonSorted[i][0].theGeneratorIndex);
    Vector<Rational>& currentHWPrimal=this->HighestWeightsPrimalNonSorted[i];
    this->GetPrimalWeightProjectionFundCoords(currentRoot, currentHWPrimal);
    sortingWeights.AddOnTop(currentHWPrimal);
  }
  sortingWeights.QuickSortAscendingCustom(*this, &this->HighestVectorsNonSorted);
  List<List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > > tempModules;
  HashedList<Vector<Rational> > tempHWs;
  tempModules.SetExpectedSize(this->HighestVectorsNonSorted.size);
  tempHWs.SetExpectedSize(this->HighestVectorsNonSorted.size);
  for (int i=0; i<this->HighestVectorsNonSorted.size; i++)
  { currentRoot=this->GetAmbientSS().GetWeightOfGenerator(this->HighestVectorsNonSorted[i][0].theGeneratorIndex);
    Vector<Rational>& currentHWrelative=this->HighestWeightsNONprimalNonSorted[i];
    Vector<Rational>& currentHWPrimal=this->HighestWeightsPrimalNonSorted[i];
    this->GetWeightProjectionFundCoords(currentRoot, currentHWrelative);
    this->GetPrimalWeightProjectionFundCoords(currentRoot, currentHWPrimal);
    theWeight.weightFundamentalCoordS=currentHWPrimal;
    this->thePrimalChaR.AddMonomial(theWeight, 1);
    int theIndex=tempHWs.GetIndex(currentHWPrimal);
    if (theIndex==-1)
    { tempModules.SetSize(tempModules.size+1);
      tempModules.LastObject()->SetSize(0);
      theIndex=tempHWs.size;
      tempHWs.AddOnTop(currentHWPrimal);
    }
    tempModules[theIndex].AddOnTop(this->HighestVectorsNonSorted[i]);
  }
//  std::cout << "<br>" << this->thePrimalChaR.ToString();
  this->HighestVectors.SetExpectedSize(this->thePrimalChaR.size()+this->owner->owneR->GetRank());
  this->HighestWeightsPrimal.SetExpectedSize(this->thePrimalChaR.size()+this->owner->owneR->GetRank());
  this->Modules.SetExpectedSize(this->thePrimalChaR.size()+this->owner->owneR->GetRank());
  this->HighestVectors.SetSize(0);
  this->HighestWeightsPrimal.Clear();
  this->Modules.SetSize(0);
  this->primalSubalgebraModules.SetSize(0);
//  std::cout << "<hr><hr>Type: " << this->theWeylNonEmbeddeD.theDynkinType.ToString() << " The basis: " << this->theBasis.ToString();
  for (int i=0; i<tempModules.size; i++)
  { this->Modules.SetSize(this->Modules.size+1);
    this->HighestVectors.SetSize(this->Modules.size);
    this->HighestVectors.LastObject()->SetSize(0);
    this->HighestWeightsPrimal.AddOnTop(tempHWs[i]);
    if (MonomialCollection<ChevalleyGenerator, Rational>::VectorSpacesIntersectionIsNonTrivial(tempModules[i], this->theBasis))
    { MonomialCollection<ChevalleyGenerator, AlgebraicNumber>::IntersectVectorSpaces(tempModules[i], this->theBasis, *this->HighestVectors.LastObject());
      if (this->HighestVectors.LastObject()->size!=1)
        crash << "This is a programming error: simple component computed to have more than one highest weight vector. " << crash;
      this->primalSubalgebraModules.AddOnTop(this->Modules.size-1);
      this->Modules.LastObject()->SetSize(1);
      *this->Modules.LastObject()->LastObject()=*this->HighestVectors.LastObject();
      if (tempModules[i].size>1)
      { this->Modules.SetSize(this->Modules.size+1);
        this->Modules.LastObject()->SetSize(tempModules[i].size-1);
        this->HighestVectors.SetSize(this->Modules.size);
        this->HighestVectors.LastObject()->SetSize(0);
        this->HighestWeightsPrimal.AddOnTop(tempHWs[i]);
        this->HighestVectors.LastObject()->AddListOnTop(this->HighestVectors[this->HighestVectors.size-2]);
        for (int j=0; j<tempModules[i].size; j++)
        { this->HighestVectors.LastObject()->AddOnTop(tempModules[i][j]);
          if (MonomialCollection<ChevalleyGenerator, Rational>::GetRankOfSpanOfElements(*this->HighestVectors.LastObject())<this->HighestVectors.LastObject()->size)
            this->HighestVectors.LastObject()->RemoveLastObject();
        }
        this->HighestVectors.LastObject()->RemoveIndexSwapWithLast(0);
        if (this->HighestVectors.LastObject()->size!=tempModules[i].size-1)
          crash << "This is a programming error: wrong number of hwv's: got  " << this->HighestVectors.LastObject()->size << ", must have "
          << tempModules[i].size-1 << ". " << crash;
        for (int j=0; j<this->HighestVectors.LastObject()->size; j++)
        { (*this->Modules.LastObject())[j].SetSize(1);
          (*this->Modules.LastObject())[j][0]=(*this->HighestVectors.LastObject())[j];
        }
      }
    } else
    { *this->HighestVectors.LastObject()=tempModules[i];
      this->Modules.LastObject()->SetSize(tempModules[i].size);
      for (int j=0; j<this->HighestVectors.LastObject()->size; j++)
      { (*this->Modules.LastObject())[j].SetSize(1);
        (*this->Modules.LastObject())[j][0]=(*this->HighestVectors.LastObject())[j];
      }
    }
  }
  this->HighestWeightsNONPrimal.SetSize(this->Modules.size);
  for (int i=0; i<this->Modules.size; i++)
    this->HighestWeightsNONPrimal[i]=this->GetNonPrimalWeightFirstGen(this->Modules[i][0][0]);
  this->subalgebraModules=this->primalSubalgebraModules;
//  std::cout << "<br>Subalgebra modules: " << this->subalgebraModules;
  this->charFormaT.GetElement().CustomPlusSign="\\oplus ";
  int theRank=this->theWeylNonEmbeddeD.GetDim();
  this->charFormaT.GetElement().vectorSpaceEiBasisNames.SetSize(theRank+this->CartanOfCentralizer.size);
  for (int i=0; i<this->charFormaT.GetElement().vectorSpaceEiBasisNames.size; i++)
  { std::stringstream tempStream;
    if (i<theRank)
      tempStream << "\\omega_{" << i+1 << "}";
    else
    { tempStream << "\\psi";
      if (this->CartanOfCentralizer.size>1)
        tempStream << "_{" << i-theRank+1 << "}";
    }
    this->charFormaT.GetElement().vectorSpaceEiBasisNames[i]=tempStream.str();
  }
  int numMods=0;
  for (int i=0; i<this->Modules.size; i++)
    numMods+=this->Modules[i].size;
  for (int i=0; i<this->Modules.size; i++)
    for (int j=0; j<this->Modules[i].size; j++)
      if (this->Modules[i][j].size!=1)
        crash << "This is a programming error: empty module! " << crash;
  if (this->thePrimalChaR.GetCoefficientsSum()!=numMods)
    crash << "This is a programming error: the sum of the coeffs of the primal char is " << this->thePrimalChaR.GetCoefficientsSum().ToString()
    << " but there are  " << numMods << " modules. Tempmodules variable: " << tempModules.ToString() << "<br>Candidate details: " << this->ToString() << crash;
}

void CandidateSSSubalgebra::ComputePrimalModuleDecompositionHWVsOnly(GlobalVariables* theGlobalVariables, HashedList<Vector<Rational> >& inputHws)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputePrimalModuleDecompositionHWVsOnly");
  this->CheckConsistency();
  List<Matrix<AlgebraicNumber> > theAdsOfHs;
  Matrix<AlgebraicNumber> tempAd, temp, commonAd, adIncludingCartanActions;
  //std::cout << "<hr>Type "  << this->theWeylNonEmbeddeD.theDynkinType.ToString()
  //<< ", ads of: " << this->thePosGens.ToString();
  for (int i=0; i<this->thePosGens.size; i++)
  { this->GetAmbientSS().GetAd(tempAd, this->thePosGens[i]);
    commonAd.AppendMatrixToTheBottom(tempAd);
//    std::cout << "Getting ad of: " << this->thePosGens[i].ToString();
  }
  ElementSemisimpleLieAlgebra<AlgebraicNumber> tempElt;
  Vectors<Rational> allHs;
  allHs.AddListOnTop(this->theHs);
  allHs.AddListOnTop(this->CartanOfCentralizer);
  theAdsOfHs.SetSize(allHs.size);
  for (int j=0; j<allHs.size; j++)
  { tempElt.MakeHgenerator(allHs[j], this->GetAmbientSS());
    this->GetAmbientSS().GetAd(theAdsOfHs[j], tempElt);
  }
  Vectors<AlgebraicNumber> outputV;
  this->HighestVectorsNonSorted.SetSize(0);
  for (int i=0; i<inputHws.size; i++)
  { adIncludingCartanActions=commonAd;
    for (int j=0; j<allHs.size; j++)
    { tempAd=theAdsOfHs[j];
      temp.MakeIdMatrix(this->GetAmbientSS().GetNumGenerators());
      temp*=inputHws[i][j];
      tempAd-=temp;
      adIncludingCartanActions.AppendMatrixToTheBottom(tempAd);
    }
    adIncludingCartanActions.GetZeroEigenSpace(outputV);
    //  std::cout << "<br>Common ad: " << commonAd.ToString();
    //  std::cout << "<br>Eigenvectors: " << outputV.ToString();
    for (int j=0; j<outputV.size; j++)
    { outputV[j].ScaleToIntegralMinHeightFirstNonZeroCoordinatePositive();
      tempElt.AssignVectorNegRootSpacesCartanPosRootSpaces(outputV[j], this->GetAmbientSS());
      this->HighestVectorsNonSorted.AddOnTop(tempElt);
    }
  }
}

bool SemisimpleSubalgebras::CheckConsistency()const
{ if (this->flagDeallocated)
    crash << "This is a programming error: use after free of semisimple subalgebras. " << crash;
  return true;
}

void SemisimpleSubalgebras:: initHookUpPointers
(SemisimpleLieAlgebra& inputOwner, AlgebraicClosureRationals* theField, HashedListReferences<SemisimpleLieAlgebra>* inputSubalgebrasNonEmbedded,
 ListReferences<SltwoSubalgebras>* inputSl2sOfSubalgebras, GlobalVariables* inputGlobalVariables)
{ this->owneR=&inputOwner;
  this->theSl2s.owner=&inputOwner;
  this->ownerField=theField;
  this->theSubalgebrasNonEmbedded=inputSubalgebrasNonEmbedded;
  this->theSl2sOfSubalgebras=inputSl2sOfSubalgebras;
  this->theGlobalVariables=inputGlobalVariables;
}

void SemisimpleSubalgebras::reset()
{ this->theGlobalVariables=0;
  this->owneR=0;
  this->ownerField=0;
  this->theRecursionCounter=0;
  this->theSl2s.owner=0;
  this->flagAttemptToSolveSystems=true;
  this->flagComputeModuleDecomposition=true;
  this->flagComputePairingTable=false;
  this->flagComputeNilradicals=false;
  this->flagProduceLaTeXtables=false;
  this->flagAttemptToAdjustCentralizers=true;
  this->timeComputationStartInSeconds=-1;
  this->timeComputationEndInSeconds=-1;
  this->numAdditions=-1;
  this->numMultiplications=-1;
  this->theSubalgebrasNonEmbedded=0;
  this->theSl2sOfSubalgebras=0;
}

bool CandidateSSSubalgebra::AttemptToSolveSystem()
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::AttemptToSolveSystem");
  this->CheckInitialization();
  this->transformedSystem=this->theSystemToSolve;
//  std::cout << "<br>Ere i am j h.";
//  return true;
  GroebnerBasisComputation<AlgebraicNumber> theComputation;
//  std::cout << "<hr>"
//  << "System before transformation: " << this->transformedSystem.ToString()
//  ;
  //std::cout << "<br>additions so far: " << Rational::total
  if (this->owner->theGlobalVariables==0)
    crash << crash;
  for (int i=401; i<5405; i+=5000)
  { theComputation.MaxNumGBComputations=i;
    theComputation.MaxNumSerreSystemComputations=i;
    theComputation.SolveSerreLikeSystem(this->transformedSystem, this->owner->ownerField, this->owner->theGlobalVariables);
    if (theComputation.flagSystemProvenToHaveNoSolution || theComputation.flagSystemProvenToHaveSolution)
      break;
  }
//  std::cout << " <br>And after: " << this->transformedSystem.ToString();
  this->flagSystemSolved=theComputation.flagSystemSolvedOverBaseField;
  this->flagSystemProvedToHaveNoSolution=theComputation.flagSystemProvenToHaveNoSolution;
  this->flagSystemGroebnerBasisFound=this->flagSystemSolved;
  if (this->flagSystemSolved)
  { //std::cout << "The system was solved!!!!";
    this->theNegGens.SetSize(this->theUnknownNegGens.size);
    this->thePosGens.SetSize(this->theUnknownPosGens.size);
    PolynomialSubstitution<AlgebraicNumber> theSub;
    theSub.SetSize(theComputation.systemSolution.GetElement().size);
    for (int i=0; i<theSub.size; i++)
      theSub[i].MakeConst(theComputation.systemSolution.GetElement()[i]);
    ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> > currentNegElt;
    ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> > currentPosElt;
    for (int i=0; i<this->theUnknownNegGens.size; i++)
    { currentNegElt=this->theUnknownNegGens[i];
      currentPosElt=this->theUnknownPosGens[i];
      currentNegElt.SubstitutionCoefficients(theSub);
      currentPosElt.SubstitutionCoefficients(theSub);
      this->theNegGens[i]=currentNegElt;//<-implicit type conversion here, will crash if currentNegElt has non-const coefficients
      this->thePosGens[i]=currentPosElt;//<-implicit type conversion here, will crash if currentNegElt has non-const coefficients
    }
    if (!this->CheckGensBracketToHs())
      crash << "This is a programming error: I just solved the Serre-Like system governing the subalgebra embedding, but the Lie brackets of the "
      << "resulting positive and negative generators are not what they should be. Something has gone very wrong. " << crash;
  } else
  { //if (this->flagSystemProvedToHaveNoSolution)
    //  std::cout << "System " << this->transformedSystem.ToString() << " <b> proven contradictory, good. </b>";
    //else
    //  std::cout << "System " << this->transformedSystem.ToString() << " <b> not solved! </b>";
  }
//  std::cout << "<hr>";
  return !this->flagSystemProvedToHaveNoSolution;
}

void CandidateSSSubalgebra::GetGenericNegGenLinearCombination(int indexNegGens, ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> >& output)
{ int offsetIndex=0;
  for (int i=0; i<indexNegGens; i++)
    offsetIndex+=this->theInvolvedNegGenerators[i].size;
  this->GetGenericLinearCombination(this->totalNumUnknownsWithCentralizer, offsetIndex, this->theInvolvedNegGenerators[indexNegGens], output);
}

void CandidateSSSubalgebra::GetGenericCartanCentralizerLinearCombination(int indexCartanCentralizerGen, ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> >& output)
{ int offsetIndex=this->totalNumUnknownsNoCentralizer+indexCartanCentralizerGen*this->GetAmbientWeyl().GetDim() ;
  List<ChevalleyGenerator> eltsCartan;
  eltsCartan.SetSize(this->GetAmbientWeyl().GetDim());
  for (int i=0; i<eltsCartan.size; i++)
  { eltsCartan[i].theGeneratorIndex=this->GetAmbientSS().GetNumPosRoots()+i;
    eltsCartan[i].owneR=&this->GetAmbientSS();
  }
  this->GetGenericLinearCombination(this->totalNumUnknownsWithCentralizer, offsetIndex, eltsCartan, output);
}

void CandidateSSSubalgebra::GetGenericPosGenLinearCombination(int indexPosGens, ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> >& output)
{ int offsetIndex=0;
  for (int i=0; i<indexPosGens; i++)
    offsetIndex+=this->theInvolvedPosGenerators[i].size;
  offsetIndex+=this->totalNumUnknownsNoCentralizer/2;
  this->GetGenericLinearCombination(this->totalNumUnknownsWithCentralizer, offsetIndex, this->theInvolvedPosGenerators[indexPosGens], output);
}

void CandidateSSSubalgebra::AddToSystem(const ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> >& elementThatMustVanish)
{ Polynomial<AlgebraicNumber> thePoly;
//  std::cout << "<hr>I must vanish: " << elementThatMustVanish.ToString();
  for (int i=0; i<elementThatMustVanish.size(); i++)
  { thePoly=elementThatMustVanish.theCoeffs[i];
    thePoly.ScaleToIntegralMinHeightFirstCoeffPosReturnsWhatIWasMultipliedBy();
    this->theSystemToSolve.AddOnTopNoRepetition(thePoly);
  }
}

void CandidateSSSubalgebra::GetGenericLinearCombination(int numVars, int varOffset, List<ChevalleyGenerator>& involvedGens, ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> >& output)
{ Polynomial<AlgebraicNumber> theCF;
  ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> > tempMon;
  output.MakeZero();
  for (int i=0; i<involvedGens.size; i++)
  { theCF.MakeDegreeOne(numVars, varOffset+i, 1);
    tempMon.MakeGenerator(involvedGens[i].theGeneratorIndex, *this->owner->owneR);
    tempMon*=theCF;
    output+=tempMon;
  }
}

bool CandidateSSSubalgebra::ComputeChar(bool allowBadCharacter)
{ if (this->indexInOwnersOfNonEmbeddedMe==-1)
    crash << "This is a programming error: attempting to compute char of candidate subalgebra that has not been initialized properly. " << crash;
  MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputeChar");
  this->CheckInitialization();
  this->theWeylNonEmbeddeD.ComputeRho(true);
  MonomialChar<Rational> tempMon;
  tempMon.weightFundamentalCoordS.MakeZero(this->theWeylNonEmbeddeD.GetDim());
  tempMon.owner=0;
  this->theCharFundamentalCoordsRelativeToCartan.MakeZero();
  this->theCharFundamentalCoordsRelativeToCartan.AddMonomial(tempMon, this->GetAmbientSS().GetRank());
  List<DynkinSimpleType> theTypes;
  this->theWeylNonEmbeddeD.theDynkinType.GetTypesWithMults(theTypes);
  //std::cout << "<br>Cartan symmetric, type  "
  //<< this->theWeylNonEmbeddeD.theDynkinType.ToString() << " <br>"
  //<< this->theWeylNonEmbeddeD.CartanSymmetric.ToString();
  for (int k=0; k<this->GetAmbientWeyl().RootSystem.size; k++)
  { int counter=-1;
    for (int i=0; i<this->CartanSAsByComponent.size; i++)
      for (int j=0; j<this->CartanSAsByComponent[i].size; j++)
      { counter++;
        tempMon.weightFundamentalCoordS[counter]=
        (this->GetAmbientWeyl().RootScalarCartanRoot(this->GetAmbientWeyl().RootSystem[k], this->CartanSAsByComponent[i][j])/theTypes[i].GetDefaultRootLengthSquared(j))*2;
//        std::cout << "<br>Scalar of " << this->GetAmbientWeyl().RootSystem[k].ToString()
//        << " and " << this->CartanSAsByComponent[i][j].ToString() << "="
//        << this->GetAmbientWeyl().RootScalarCartanRoot
//        (this->GetAmbientWeyl().RootSystem[k], this->CartanSAsByComponent[i][j]).ToString()
//        << " -> "
//        << (this->GetAmbientWeyl().RootScalarCartanRoot
//        (this->GetAmbientWeyl().RootSystem[k], this->CartanSAsByComponent[i][j])
//        /theTypes[i].GetDefaultRootLengthSquared(j))*2;
        if(!tempMon.weightFundamentalCoordS[counter].IsInteger())
        { if (!allowBadCharacter)
            crash << "This is a programming error: function ComputeChar called with Cartan that suggests non-integral characters. At "
            << "the same time, an option banning this possibility has been explicitly selected. " << crash;
          return false;
        }
      }
    this->theCharFundamentalCoordsRelativeToCartan.AddMonomial(tempMon, 1);
  }
  charSSAlgMod<Rational> accumChar, freudenthalChar, outputChar;
  accumChar=this->theCharFundamentalCoordsRelativeToCartan;
  SemisimpleLieAlgebra* nonEmbeddedMe= &this->owner->theSubalgebrasNonEmbedded->GetElement(this->indexInOwnersOfNonEmbeddedMe);
  this->theCharNonPrimalFundCoords.MakeZero();

//  std::cout << "<hr>Current candidate: " << this->ToStringCartanSA();
//  std::cout << "<br>reducing: " << accumChar.ToString();
  while (accumChar.size()>0)
  { int currentIndex=accumChar.GetIndexExtremeWeightRelativeToWeyl(this->theWeylNonEmbeddeD);
    if (currentIndex==-1)
      crash << "This is a programming error: while decomposing ambient Lie algebra over the candidate subalgebra, I got "
      << "that there is no extreme weight. This is impossible: something has gone very wrong. " << crash;
//    std::cout << "<br>Extreme weight: " << //this->theWeylNonEmbeddeD.GetSimpleCoordinatesFromFundamental
//    (accumChar[currentIndex].weightFundamentalCoords).ToString();

    if (accumChar.theCoeffs[currentIndex]<0)
    { //std::cout << "<br>accumChar has negative coeff!";
      return false;
    }
    for (int i=0; i<accumChar[currentIndex].weightFundamentalCoordS.size; i++)
      if (accumChar[currentIndex].weightFundamentalCoordS[i]<0)
      { //std::cout << "<br>accumChar[currentIndex].weightFundamentalCoords[i] is less than 0.";
        return false;
      }
    freudenthalChar.MakeZero();
    tempMon=accumChar[currentIndex];
    tempMon.owner=nonEmbeddedMe;
    freudenthalChar.AddMonomial(tempMon, accumChar.theCoeffs[currentIndex]);
    this->theCharNonPrimalFundCoords.AddMonomial(accumChar[currentIndex], accumChar.theCoeffs[currentIndex]);
    std::string tempS;
    bool tempBool=freudenthalChar.FreudenthalEvalMeFullCharacter(outputChar, -1, &tempS, this->owner->theGlobalVariables);
    if (!tempBool && !allowBadCharacter)
    { crash << "This is a programming error: failed to evaluate full character via the Freudenthal formula on a relatively small example, namely "
      << freudenthalChar.ToString() << ". The failure message was: " << tempS << ". This shouldn't happen. " << crash;
      return false;
    }
    accumChar-=outputChar;
    //std::cout << "<br>remaining char:" << accumChar.ToString();
  }
  return true;
}

void slTwoSubalgebra::ElementToStringModuleDecompositionMinimalContainingRegularSAs(bool useLatex, bool useHtml, SltwoSubalgebras& owner, std::string& output)const
{ std::stringstream out;
  std::string tempS;
  if (useLatex)
    out << "$";
  if (useHtml)
  { out << "<table><tr><td align=\"center\">Char.</td>";
    for (int i=0; i<this->IndicesMinimalContainingRootSA.size; i++)
    { rootSubalgebra& theSA= owner.theRootSAs.theSubalgebras[this->IndicesMinimalContainingRootSA[i]];
      out << "<td align=\"center\">Decomp. "
      << theSA.theDynkinDiagram.ToStringRelativeToAmbientType(this->owneR->theWeyl.theDynkinType[0]) << "</td>";
    }
    out << "</tr>\n";
  }
  out << "<tr><td align=\"center\"> " << this->hCharacteristic.ToString() << "</td>";
  for (int k=0; k<this->IndicesMinimalContainingRootSA.size; k++)
  { rootSubalgebra& theSA= owner.theRootSAs.theSubalgebras[this->IndicesMinimalContainingRootSA[k]];
    tempS=theSA.theDynkinDiagram.ToStringRelativeToAmbientType(this->owneR->theWeyl.theDynkinType[0]);
    if (useHtml)
      out << "<td align=\"center\">";
    for (int i=0; i<this->HighestWeightsDecompositionMinimalContainingRootSA[k].size; i++)
    { if (this->MultiplicitiesDecompositionMinimalContainingRootSA[k][i]>1)
        out << this->MultiplicitiesDecompositionMinimalContainingRootSA[k][i];
      out << "V_{"<<this->HighestWeightsDecompositionMinimalContainingRootSA[k][i] << "}";
      if (i!=this->HighestWeightsDecompositionMinimalContainingRootSA[k].size-1)
        out << "+";
    }
    if (useHtml)
      out << "</td>";
    out << "\n";
  }
  if (useHtml)
    out << "</tr></table>";
  if (useLatex)
    out << "$";
  output=out.str();
}

void slTwoSubalgebra::ElementToHtmlCreateFormulaOutputReference
(const std::string& formulaTex, std::stringstream& output, bool usePNG, bool useHtml, SltwoSubalgebras& container, std::string* physicalPath, std::string* htmlPathServer)const
{ if (!usePNG)
  { output << formulaTex;
    //if (useHtml)
      //output<<"\n<br>\n";
    return;
  }
  std::stringstream tempStream;
  container.texFileNamesForPNG.SetSize(container.texFileNamesForPNG.size+1);
  container.texStringsEachFile.SetSize(container.texFileNamesForPNG.size);
  (*container.texStringsEachFile.LastObject())=formulaTex;
  tempStream << (*physicalPath) << "fla";
  tempStream << container.texFileNamesForPNG.size << ".tex";
  container.texFileNamesForPNG[container.texFileNamesForPNG.size-1]=tempStream.str();
  output << "<img src=\"" << (*htmlPathServer) << "fla" << container.texFileNamesForPNG.size << ".png\">";
  if (useHtml)
    output << "\n<br>\n";
}

WeylGroup& slTwoSubalgebra::GetOwnerWeyl()
{ return this->GetOwnerSSAlgebra().theWeyl;
}

bool slTwoSubalgebra::operator==(const slTwoSubalgebra& right)const
{// See Dynkin, Semisimple Lie subalgebras of semisimple Lie algebras, chapter 7-10
  if (this->owneR!=right.owneR)
    crash << "This is a programming error: comparing sl(2) subalgebras that have different ambient Lie algebras. " << crash;
  return this->hCharacteristic==(right.hCharacteristic);
}

std::string slTwoSubalgebra::ToString(FormatExpressions* theFormat)const
{ MacroRegisterFunctionWithName("slTwoSubalgebra::ToString");
  if (this->container==0)
    return "sl(2) subalgebra not initialized.";
  std::stringstream out;  std::string tempS;
  out << "<a name=\"sl2index" << indexInContainer << "\">h-characteristic: " << this->hCharacteristic.ToString() << "</a>";
  out << "<br>Length of the weight dual to h: " << this->LengthHsquared;
  tempS=this->preferredAmbientSimpleBasis.ToString();
  std::string physicalPath="";
  std::string htmlPathServer="";
  //bool usePNG=false;
  bool useHtml=true;
  bool useLatex=false;
  if (theFormat!=0)
    if (theFormat->PathPhysicalOutputFolder!="")
    { physicalPath=theFormat->PathPhysicalOutputFolder+ "sl2s/";
      htmlPathServer=theFormat->PathDisplayNameCalculator + "sl2s/";
      //usePNG=theFormat->flagUsePNG;
    }
  if (physicalPath=="" || htmlPathServer=="")
  { //usePNG=false;
    useHtml=false;
  }
  if (useHtml)
    out << "<br>";
  out << "\nSimple basis ambient algebra w.r.t defining h: " << tempS;
  Matrix<Rational> tempMat;
  if (useHtml)
    out << "<br>";
  if (this->IndicesContainingRootSAs.size>1)
  { out << "Number of containing regular semisimple subalgebras: " << this->IndicesContainingRootSAs.size;
    if (useHtml)
      out << "<br>";
  }
  FormatExpressions localFormat, latexFormat;
  localFormat.flagUseHTML=useHtml;
  localFormat.flagUseLatex=useLatex;
  latexFormat.flagUseHTML=false;
  latexFormat.flagUseLatex=true;
  for (int i=0; i<this->IndicesContainingRootSAs.size; i++)
  { out << "\nContaining regular semisimple subalgebra number " << i+1 << ": ";
    rootSubalgebra& currentSA= this->container->theRootSAs.theSubalgebras[this->IndicesContainingRootSAs[i]];
    if (useHtml)
    { out << "<a href=\"" << htmlPathServer << "../rootHtml_rootSA" << this->IndicesContainingRootSAs[i] << ".html\">";
      tempS=currentSA.theDynkinDiagram.ToStringRelativeToAmbientType(this->owneR->theWeyl.theDynkinType[0]);
    }
    out << tempS;
    if (useHtml)
      out << "</a>";
  }
  if (useHtml)
    out << "<br>";
  out << "\nsl(2)-module decomposition of the ambient Lie algebra: ";
  this->ElementToStringModuleDecomposition(useLatex, useHtml, tempS);
  out << CGI::GetMathMouseHover(tempS) << "\n<br>\n";
  if (indexInContainer!=-1)
  { this->container->IndicesSl2decompositionFlas.SetSize(this->container->size);
    this->container->IndicesSl2decompositionFlas[indexInContainer]=
    this->container->texFileNamesForPNG.size-1;
  }
  out << "\nBelow is one possible realization of the sl(2) subalgebra.";
  if (useHtml)
    out << "\n<br>\n";
  std::stringstream tempStreamH, tempStreamE, tempStreamF, tempStreamHE, tempStreamHF, tempStreamEF;
  tempS=this->theH.ToString(theFormat);
  tempStreamH << "\nh= " << tempS << "";
  out << CGI::GetMathMouseHover(tempStreamH.str()) << "\n<br>\n";
  tempStreamE << "\ne= " << this->theE.ToString(theFormat) << "\n<br>\n";
  out << CGI::GetMathMouseHover(tempStreamE.str()) << "\n<br>\n";
  tempStreamF << "\nf= " << this->theF.ToString(theFormat) << "\n<br>\n";
/*  out << CGI::GetMathMouseHover(tempStreamF.str()) << "\n<br>\n";
  out << "\nLie brackets of the above elements. Printed for debugging.";
  if (useHtml)
    out << "\n<br>\n";
  tempStreamEF << "\n[e, f]=" <<  this->bufferEbracketF.ToString(theFormat) << "";
  out << CGI::GetMathMouseHover(tempStreamEF.str()) << "\n<br>\n";
  tempStreamHE << "\n[h, e]=" << this->bufferHbracketE.ToString(theFormat) << "";
  out << CGI::GetMathMouseHover(tempStreamHE.str()) << "\n<br>\n";
  tempStreamHF << "\n[h, f]= " << this->bufferHbracketF.ToString(theFormat) << "";
  out << CGI::GetMathMouseHover(tempStreamHF.str()) << "\n<br>\n";*/
  //this->theSystemMatrixForm.ToString(tempS);
  //out <<"\nSystem matrix form we try to solve:\n"<< tempS;
  //this->theSystemColumnVector.ToString(tempS);
  //out <<"\nColumn vector of the system:\n"<<tempS;
  std::stringstream tempStreamActual;
  tempStreamActual << "\\begin{array}{l}";
  for (int i=0; i<this->theSystemToBeSolved.size; i++)
    tempStreamActual << this->theSystemToBeSolved[i].ToString(theFormat) << "~\\\\";
  tempStreamActual << "\\end{array}";
  out << "\nThe polynomial system that corresponds to finding the h,e,f triple:\n";
  if (useHtml)
    out << "\n<br>\n";
  out << CGI::GetMathMouseHover(tempStreamActual.str()) << "\n<br>\n";
  return out.str();
}

bool slTwoSubalgebra::CheckConsistency()const
{ if (this->flagDeallocated)
    crash << "This is a programming error: use after free of slTwoSubalgebra. " << crash;
  if (this->owneR!=0)
    this->owneR->CheckConsistency();
  return true;
}

void slTwoSubalgebra::ComputeDynkinsEpsilon(WeylGroup& theWeyl)
{//outdates, must be erased as soon as I implement an equivalent
  this->DynkinsEpsilon = this->DiagramM.NumRootsGeneratedByDiagram()+this->DiagramM.RankTotal();
  int r=0;
  for (int i=0; i<this->hCharacteristic.size; i++)
    if (!this->hCharacteristic[i].IsEqualToZero())
      r++;
  this->DynkinsEpsilon+= r;
  for (int i=0; i<theWeyl.RootSystem.size; i++)
  { int NumNonZeroFound=0;
    for (int j=0; j<this->hCharacteristic.size; j++)
    { if (theWeyl.RootSystem[i][j]==1)
        NumNonZeroFound++;
      if(NumNonZeroFound>1)
        break;
    }
    if (NumNonZeroFound==1)
      this->DynkinsEpsilon--;
  }
  this->DynkinsEpsilon=0;
  for (int i=0; i<this->hCharacteristic.size; i++)
    if (this->hCharacteristic[i]==1)
      this->DynkinsEpsilon=-1;
  this->DynkinsEpsilon=0;
}

bool slTwoSubalgebra::ModuleDecompositionFitsInto(const slTwoSubalgebra& other)const
{ return this->ModuleDecompositionFitsInto(this->highestWeights, this->multiplicitiesHighestWeights, other.highestWeights, other.multiplicitiesHighestWeights);
}

bool slTwoSubalgebra::ModuleDecompositionFitsInto
(const List<int>& highestWeightsLeft, const List<int>& multiplicitiesHighestWeightsLeft, const List<int>& highestWeightsRight, const List<int>& multiplicitiesHighestWeightsRight)
{ for (int i=0; i<highestWeightsLeft.size; i++)
  { int theIndex= highestWeightsRight.GetIndex(highestWeightsLeft[i]);
    if (theIndex==-1)
      return false;
    else
      if (multiplicitiesHighestWeightsLeft[i]>multiplicitiesHighestWeightsRight[theIndex])
        return false;
  }
  return true;
}

void slTwoSubalgebra::ElementToHtml(std::string& filePath)
{ std::fstream theFile;
  std::string theFileName=filePath;
  theFileName.append("theSlTwo.txt");
  XML::OpenFileCreateIfNotPresent(theFile, filePath, false, true, false);
}

void SltwoSubalgebras::reset(SemisimpleLieAlgebra& inputOwner)
{ MacroRegisterFunctionWithName("SltwoSubalgebras::reset");
  MultiplicitiesFixedHweight.SetSize(0);
  this->IndicesSl2sContainedInRootSA.SetSize(0);
  this->IndicesSl2decompositionFlas.SetSize(0);
  this->BadHCharacteristics.SetSize(0);
  this->IndexZeroWeight=-1;
  this->texFileNamesForPNG.SetSize(0);
  this->texStringsEachFile.SetSize(0);
  this->listSystemCommandsLatex.SetSize(0);
  this->listSystemCommandsDVIPNG.SetSize(0);

  this->owner=& inputOwner;
  this->theRootSAs.owneR=&inputOwner;
}

void SemisimpleLieAlgebra::FindSl2Subalgebras(SemisimpleLieAlgebra& inputOwner, SltwoSubalgebras& output, GlobalVariables& theGlobalVariables)
{ MacroRegisterFunctionWithName("SemisimpleLieAlgebra::FindSl2Subalgebras");
  inputOwner.CheckConsistency();
  output.reset(inputOwner);
  output.CheckConsistency();
  output.GetOwner().ComputeChevalleyConstants(&theGlobalVariables);
  output.theRootSAs.theGlobalVariables=&theGlobalVariables;
  output.theRootSAs.owneR=&inputOwner;
  output.theRootSAs.ComputeAllReductiveRootSubalgebrasUpToIsomorphism();
  //output.theRootSAs.ComputeDebugString(false, false, false, 0, 0, theGlobalVariables);
  output.IndicesSl2sContainedInRootSA.SetSize(output.theRootSAs.theSubalgebras.size);
  output.IndicesSl2sContainedInRootSA.ReservE(output.theRootSAs.theSubalgebras.size*2);
  for (int i=0; i<output.IndicesSl2sContainedInRootSA.size; i++)
    output.IndicesSl2sContainedInRootSA[i].size=0;
  ProgressReport theReport(&theGlobalVariables);
  for (int i=0; i<output.theRootSAs.theSubalgebras.size; i++)
  { std::stringstream tempStream;
    tempStream << "\nExploring root subalgebra " << output.theRootSAs.theSubalgebras[i].theDynkinDiagram.ToStringRelativeToAmbientType(inputOwner.theWeyl.theDynkinType[0])
    << "(" << (i+1) << " out of " << output.theRootSAs.theSubalgebras.size-1 << " non-trivial)\n";
    theReport.Report(tempStream.str());
    output.theRootSAs.theSubalgebras[i].GetSsl2SubalgebrasAppendListNoRepetition(output, i, theGlobalVariables);
  }
  inputOwner.CheckConsistency();
  for (int i=0; i<output.size; i++)
  { //slTwoSubalgebra& theSl2= output.GetElement(i);
    output.GetElement(i).IndicesMinimalContainingRootSA.ReservE(output.GetElement(i).IndicesContainingRootSAs.size);
    output.GetElement(i).IndicesMinimalContainingRootSA.size=0;
    for (int j=0; j<output.GetElement(i).IndicesContainingRootSAs.size; j++)
    { bool isMinimalContaining=true;
//      rootSubalgebra& currentRootSA = output.theRootSAs.TheObjects[];
      for (int k=0; k<output.GetElement(i).IndicesContainingRootSAs.size; k++)
      { rootSubalgebra& theOtherRootSA = output.theRootSAs.theSubalgebras[output.GetElement(i).IndicesContainingRootSAs[k]];
        if (theOtherRootSA.indicesSubalgebrasContainingK.Contains(output.GetElement(i).IndicesContainingRootSAs[j]))
        { isMinimalContaining=false;
          break;
        }
      }
      if (isMinimalContaining)
        output.GetElement(i).IndicesMinimalContainingRootSA.AddOnTopNoRepetition(output.GetElement(i).IndicesContainingRootSAs[j]);
    }
    output.CheckConsistency();
    output.ComputeModuleDecompositionsOfAmbientLieAlgebra(theGlobalVariables);
  }
//  tempRootSA.GetSsl2Subalgebras(tempSl2s, theGlobalVariables, *this);
}

bool CandidateSSSubalgebra::CheckConsistency()const
{ if (this->flagDeallocated)
    crash << "This is a programming error: use after free of CandidateSSSubalgebra. " << crash;
  return true;
}

bool CandidateSSSubalgebra::CheckMaximalDominance()const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::CheckMaximalDominance");
  this->CheckInitialization();
  Rational theScalarProd;
  for (int i=0; i<this->GetAmbientWeyl().RootsOfBorel.size; i++)
  { Vector<Rational>& currentPosRoot=this->GetAmbientWeyl().RootsOfBorel[i];
    bool canBeRaisingReflection=true;
    for (int k=0; k<this->CartanSAsByComponent.size && canBeRaisingReflection; k++)
      for (int l=0; l<this->CartanSAsByComponent[k].size && canBeRaisingReflection; l++)
      { theScalarProd=this->GetAmbientWeyl().RootScalarCartanRoot(currentPosRoot, this->CartanSAsByComponent[k][l]);
        if (theScalarProd>0)
          canBeRaisingReflection=false;
        if (theScalarProd<0)
        { crash << "This is a programming error. The candidate h elements of the semisimple subalgebra are supposed to be maximally dominant, "
          << "however the scalar product of the positive root " << currentPosRoot.ToString() << " with the subalgebra root "
          << this->CartanSAsByComponent[k][l].ToString() << " is negative, while the very same positive root has had zero scalar products with all "
          << " preceding roots. Here are all preceding roots: " << this->CartanSAsByComponent.ToString() << crash;
          return false;
        }
      }
  }
  return true;
}

bool SltwoSubalgebras::CheckConsistency()const
{ if (this->flagDeallocated)
    crash << "This is a programming error: use after free of SemisimpleLieAlgebra. " << crash;
  if (this->owner!=0)
    this->owner->CheckConsistency();
  for(int i=0; i<this->size; i++)
    (*this)[i].CheckConsistency();
  return true;
}

void SltwoSubalgebras::ComputeModuleDecompositionsOfAmbientLieAlgebra(GlobalVariables& theGlobalVariables)
{ this->GrandMasterConsistencyCheck();
  this->CheckConsistency();
  for(int i=0; i<this->size; i++)
    (*this).GetElement(i).ComputeModuleDecompositionAmbientLieAlgebra(theGlobalVariables);
  this->GrandMasterConsistencyCheck();
}


bool SltwoSubalgebras::ContainsSl2WithGivenH(Vector<Rational>& theH, int* outputIndex)
{ if (outputIndex!=0)
    *outputIndex=-1;
  ElementSemisimpleLieAlgebra<Rational> tempH;
  this->CheckForCorrectInitializationCrashIfNot();
  tempH.MakeHgenerator(theH, *this->owner);
  for (int i=0; i<this->size; i++)
    if (this->TheObjects[i].theH==tempH)
    { if (outputIndex!=0)
        *outputIndex=i;
      return true;
    }
  return false;
}

bool SltwoSubalgebras::ContainsSl2WithGivenHCharacteristic(Vector<Rational>& theHCharacteristic, int* outputIndex)
{ if (outputIndex!=0)
    *outputIndex=-1;
  for (int i=0; i<this->size; i++)
    if ((*this)[i].hCharacteristic==theHCharacteristic)
    { if (outputIndex!=0)
        *outputIndex=i;
      return true;
    }
  return false;
}

void slTwoSubalgebra::ElementToStringModuleDecomposition(bool useLatex, bool useHtml, std::string& output)const
{ std::stringstream out;
  for (int i=0; i<this->highestWeights.size; i++)
  { if (this->multiplicitiesHighestWeights[i]>1)
      out << this->multiplicitiesHighestWeights[i];
    out << "V_{" << this->highestWeights[i] << "}";
    if (i!=this->highestWeights.size-1)
      out << "+";
  }
  output=CGI::GetMathMouseHover(out.str());
}

void slTwoSubalgebra::ComputeModuleDecompositionAmbientLieAlgebra(GlobalVariables& theGlobalVariables)
{ this->CheckConsistency();
  this->ComputePrimalModuleDecomposition(this->GetOwnerWeyl().RootsOfBorel, this->GetOwnerWeyl().CartanSymmetric.NumRows, this->highestWeights, this->multiplicitiesHighestWeights, this->weightSpaceDimensions, theGlobalVariables);
}

void slTwoSubalgebra::ComputeModuleDecompositionOfMinimalContainingRegularSAs(SltwoSubalgebras& owner, int IndexInOwner, GlobalVariables& theGlobalVariables)
{ this->MultiplicitiesDecompositionMinimalContainingRootSA.SetSize(this->IndicesMinimalContainingRootSA.size);
  this->HighestWeightsDecompositionMinimalContainingRootSA.SetSize(this->IndicesMinimalContainingRootSA.size);
  List<int> buffer;
  for (int i=0; i<this->IndicesMinimalContainingRootSA.size; i++)
  { rootSubalgebra& theSA= owner.theRootSAs.theSubalgebras[this->IndicesMinimalContainingRootSA[i]];
    this->ComputePrimalModuleDecomposition(theSA.PositiveRootsK, theSA.SimpleBasisK.size, this->HighestWeightsDecompositionMinimalContainingRootSA[i], this->MultiplicitiesDecompositionMinimalContainingRootSA[i], buffer, theGlobalVariables);
  }
}

void slTwoSubalgebra::MakeReportPrecomputations
(GlobalVariables& theGlobalVariables, SltwoSubalgebras& container, int indexInContainer, int indexMinimalContainingRegularSA, rootSubalgebra& MinimalContainingRegularSubalgebra)
{ MacroRegisterFunctionWithName("slTwoSubalgebra::MakeReportPrecomputations");
  int theDimension=this->GetOwnerSSAlgebra().GetRank();
  this->IndicesContainingRootSAs.size=0;
  Vectors<Rational> tempRoots;
  tempRoots=(MinimalContainingRegularSubalgebra.SimpleBasisK);
  this->GetOwnerSSAlgebra().theWeyl.TransformToSimpleBasisGeneratorsWRTh(tempRoots, this->theH.GetCartanPart());
  DynkinDiagramRootSubalgebra theDiagram;
  theDiagram.ComputeDiagramTypeKeepInput(tempRoots, this->GetOwnerSSAlgebra().theWeyl.CartanSymmetric);
  this->IndicesContainingRootSAs.AddOnTop(indexMinimalContainingRegularSA);
  tempRoots.MakeEiBasis(theDimension);
  this->GetOwnerSSAlgebra().theWeyl.TransformToSimpleBasisGeneratorsWRTh(tempRoots, this->theH.GetCartanPart());
  DynkinDiagramRootSubalgebra tempDiagram;
  tempDiagram.ComputeDiagramTypeKeepInput(tempRoots, this->GetOwnerSSAlgebra().theWeyl.CartanSymmetric);
  this->preferredAmbientSimpleBasis=tempRoots;
  this->hCharacteristic.SetSize(theDimension);
  for (int i=0; i<theDimension; i++)
    this->hCharacteristic[i]= this->GetOwnerSSAlgebra().theWeyl.RootScalarCartanRoot(this->theH.GetCartanPart(), this->preferredAmbientSimpleBasis[i]);
  this->LengthHsquared=this->GetOwnerSSAlgebra().theWeyl.RootScalarCartanRoot(this->theH.GetCartanPart(), this->theH.GetCartanPart());
  //this->hCharacteristic.ComputeDebugString();
//  if (this->theE.NonZeroElements.MaxSize==this->owner->theWeyl.RootSystem.size
//      && this->theF.NonZeroElements.MaxSize==this->owner->theWeyl.RootSystem.size
//      && this->theH.NonZeroElements.MaxSize==this->owner->theWeyl.RootSystem.size)
  this->GetOwnerSSAlgebra().LieBracket(this->theE, this->theF, this->bufferEbracketF);
//  std:: cout << "[" << this->theE.ToString() << ", " << this->theF.ToString() << "]="
//  << this->bufferEbracketF.ToString();
  this->GetOwnerSSAlgebra().LieBracket(this->theH, this->theE, this->bufferHbracketE);
  this->GetOwnerSSAlgebra().LieBracket(this->theH, this->theF, this->bufferHbracketF);

  //theSl2.hCharacteristic.ComputeDebugString();
//  this->ComputePrimalModuleDecomposition();
}

//The below code is related to sl(2) subalgebras of simple Lie algebras
void slTwoSubalgebra::ComputePrimalModuleDecomposition
(Vectors<Rational>& positiveRootsContainingRegularSA, int dimensionContainingRegularSA, List<int>& outputHighestWeights,
 List<int>& outputMultiplicitiesHighestWeights, List<int>& outputWeightSpaceDimensions, GlobalVariables& theGlobalVariables)
{ MacroRegisterFunctionWithName("slTwoSubalgebra::ComputePrimalModuleDecomposition");
  this->CheckConsistency();
  positiveRootsContainingRegularSA.CheckConsistency();
  if (positiveRootsContainingRegularSA.size<=0)
    crash << "This is a programming error: positiveRootsContainingRegularSA has less than one element. " << crash;
  int IndexZeroWeight=positiveRootsContainingRegularSA.size*2;
  outputWeightSpaceDimensions.initFillInObject(4*positiveRootsContainingRegularSA.size+1, 0);
  outputWeightSpaceDimensions[IndexZeroWeight]=dimensionContainingRegularSA;
  List<int> BufferHighestWeights;
  Rational tempRat;
  Vectors<Rational> coordsInPreferredSimpleBasis, tempRoots2;
  Matrix<Rational> tempMat;
  positiveRootsContainingRegularSA.GetCoordsInBasis(this->preferredAmbientSimpleBasis, coordsInPreferredSimpleBasis, tempRoots2, tempMat);
  for (int k=0; k<positiveRootsContainingRegularSA.size; k++)
  { tempRat=this->hCharacteristic.ScalarEuclidean(coordsInPreferredSimpleBasis[k]);
    if(tempRat.DenShort!=1)
      crash << crash;
    if (tempRat>positiveRootsContainingRegularSA.size*2)
    { crash << "This is a programming error. The scalar product of the h-Characteristic " << this->hCharacteristic.ToString()
      << " with the simple root " << coordsInPreferredSimpleBasis[k].ToString() << " is larger than " << positiveRootsContainingRegularSA.size*2
      << ". The affected sl(2) subalgebra is " << this->ToString() << ". " << crash;
      break;
    }
    outputWeightSpaceDimensions[IndexZeroWeight+tempRat.NumShort]++;
    outputWeightSpaceDimensions[IndexZeroWeight-tempRat.NumShort]++;
  }
  BufferHighestWeights=(outputWeightSpaceDimensions);
  outputHighestWeights.ReservE(positiveRootsContainingRegularSA.size*2);
  outputMultiplicitiesHighestWeights.ReservE(positiveRootsContainingRegularSA.size*2);
  outputHighestWeights.size=0;
  outputMultiplicitiesHighestWeights.size=0;
//  this->hCharacteristic.ComputeDebugString();
//  std::cout << "Starting weights:  " << BufferHighestWeights;
  for (int j=BufferHighestWeights.size-1; j>=IndexZeroWeight; j--)
  { int topMult = BufferHighestWeights[j];
    if (topMult<0)
      crash << "This is a programming error: the sl(2)-module decomposition shows an sl(2)-module with highest weight "
      << topMult << " which is impossible. Here is the sl(2) subalgebra. " << this->ToString() << "." << crash;
    if (topMult>0)
    { outputHighestWeights.AddOnTop(j-IndexZeroWeight);
      outputMultiplicitiesHighestWeights.AddOnTop(topMult);
      if (j!=IndexZeroWeight)
        BufferHighestWeights[IndexZeroWeight*2-j]-=topMult;
      for (int k=j-2; k>=IndexZeroWeight; k-=2)
      { BufferHighestWeights[k]-=topMult;
        if (k!=IndexZeroWeight)
          BufferHighestWeights[IndexZeroWeight*2-k]-=topMult;
        if(BufferHighestWeights[k]<0 || !(BufferHighestWeights[k]==BufferHighestWeights[IndexZeroWeight*2-k]))
        { crash.theCrashReport << " This is a programming error: an error check has failed. While trying to decompose with respect to  h-characteristic <br> "
          << this->hCharacteristic.ToString() << ". The positive root system of the containing root subalgebra is <br>" << positiveRootsContainingRegularSA.ToString()
          << ". <br>The preferred simple basis is <br>" << this->preferredAmbientSimpleBasis.ToString() << ". The coordinates relative to the preferred simple basis are<br>"
          << coordsInPreferredSimpleBasis.ToString() << " The starting weights list is <br>" << outputWeightSpaceDimensions << ". "
          << " I got that the root space of index  " <<  k+1 << " has negative dimension. Something is wrong. ";
          crash << crash;
        }
      }
    }
  }
}

void SltwoSubalgebras::ElementToStringModuleDecompositionMinimalContainingRegularSAs(std::string& output, bool useLatex, bool useHtml)
{ std::string tempS;
  std::stringstream out;
  for (int i=0; i<this->size; i++)
  { (*this)[i].ElementToStringModuleDecompositionMinimalContainingRegularSAs(useLatex, useHtml, *this, tempS);
    out << tempS;
    if (useHtml)
      out << "\n<br>";
    out << "\n";
  }
  output=out.str();
}

std::string SltwoSubalgebras::ElementToStringNoGenerators(FormatExpressions* theFormat)
{ MacroRegisterFunctionWithName("SltwoSubalgebras::ElementToStringNoGenerators");
  std::string tempS; std::stringstream out;
  std::string tooltipHchar="Let h be in the Cartan s.a. Let \\alpha_1, ..., \\alpha_n be simple root w.r.t. h. Then the h-characteristic is the n-tuple (\\alpha_1(h), ..., \\alpha_n(h))";
  std::string tooltipVDecomposition="The sl(2) submodules of g are parametrized by their highest weight w.r.t. h. V_l is l+1 dimensional";
  std::string tooltipContainingRegular=
  "A regular semisimple subalgebra might contain an sl(2) such that the sl(2) has no centralizer in the regular semisimple subalgebra, but the regular semisimple subalgebra might fail to be \
  minimal containing. This happens when another minimal containing regular semisimple subalgebra of equal rank nests as a root subalgebra in the containing SA. \
  See Dynkin, Semisimple Lie subalgebras of semisimple Lie algebras, remark before Theorem 10.4.";
  std::string tooltipHvalue=
  "The actual realization of h. The coordinates of h are given with respect to the fixed original simple basis. Note that the characteristic of h is given \
  *with respect to another basis* (namely, with respect to an h-positive simple basis). I will fix this in the future (email me if you want that done sooner).";
  bool useHtml=theFormat==0 ? true : theFormat->flagUseHTML;
  bool useLatex=theFormat==0 ? true : theFormat->flagUseLatex;
  std::string physicalPath, displayPath;
  physicalPath=theFormat==0 ? "../" : theFormat->PathPhysicalOutputFolder;
  displayPath=theFormat==0 ? "../" : theFormat->PathDisplayOutputFolder;
  out << "Number of sl(2) subalgebras " << this->size << ".\n";
  std::stringstream out2;
  out2 << "<br>Length longest root ambient algebra squared/4= " << this->GetOwnerWeyl().GetLongestRootLengthSquared()/4 << "<br>";
  out2 << "<br> Given a root subsystem P, and a root subsubsystem P_0, in (10.2) of Semisimple subalgebras of semisimple Lie algebras, E. Dynkin defines "
  << " a numerical constant e(P, P_0) (which we call Dynkin epsilon). <br>In Theorem 10.3, Dynkin proves that if an sl(2) is an S-subalgebra in "
  << " the root subalgebra generated by P, such that it has characteristic 2 for all simple roots of P lying in P_0, then e(P, P_0)=0. ";

  if (this->BadHCharacteristics.size>0)
  { bool allbadAreGoodInTheirBadness=true;
    for (int i=0; i<this->BadHCharacteristics.size; i++)
    { bool isGoodInItsbaddness=false;
      for (int j=0; j<this->size; j++)
        if (this->BadHCharacteristics[i]==(*this)[j].theH.GetCartanPart())
        { isGoodInItsbaddness=true;
          break;
        }
      if (!isGoodInItsbaddness)
      { allbadAreGoodInTheirBadness=false;
        out << "<b>Bad characteristic: " << this->BadHCharacteristics[i] << "</b>";
      }
      else
      { out2 << "<br><b>It turns out that in the current case of Cartan element h = " << this->BadHCharacteristics[i] << " we have that, for a certain P, "
        << " e(P, P_0) equals 0, but I failed to realize the corresponding sl(2) as a subalgebra of that P. However, it turns out that h "
        << " is indeed an S-subalgebra of a smaller root subalgebra P'.</b>";
      }
    }
    if (!allbadAreGoodInTheirBadness)
      out << "<b>found bad characteristics!</b>";
  } else
  { out2 << "It turns out by direct computation that, in the current case of " << this->GetOwner().GetLieAlgebraName()
    << ",  e(P,P_0)=0 implies that an S-sl(2) subalgebra of the root subalgebra generated by P with characteristic with 2's in the simple roots of P_0 "
    << " always exists. Note that Theorem 10.3 is stated in one direction only.";
  }
  if(useHtml)
    out << "<br><br><table><tr><td style=\"padding-right:20px\">" << CGI::ElementToStringTooltip("Characteristic", tooltipHchar)
    << "</td><td align=\"center\" title=\"" << tooltipHvalue << "\"> h</td><td style=\"padding-left:20px\" title=\""
    << tooltipVDecomposition << "\"> Decomposition of ambient Lie algebra</td><td>The square of the length of the weight dual to h.</td>"
    << "<td>Dynkin index </td><td>Minimal containing regular semisimple SAs</td><td title=\""
    << tooltipContainingRegular << "\">Containing regular semisimple SAs in which the sl(2) has no centralizer</td> </tr>";
  for (int i=0; i<this->size; i++)
  { const slTwoSubalgebra& theSl2= (*this)[i];
    if (useHtml)
      out << "<tr><td style=\"padding-right:20px\"><a href=\"./sl2s.html#sl2index" << i << "\"title=\"" << tooltipHchar << "\" >";
    out << theSl2.hCharacteristic.ToString();
    if (useHtml)
      out << "</a></td><td title=\"" << tooltipHvalue << "\">";
    out << theSl2.theH.GetCartanPart().ToString();
    if(!this->GetOwnerWeyl().IsDominantWeight(theSl2.theH.GetCartanPart()))
      out << "<b>This is wrong!!!!!!!!!!!!! The h is not dual to a dominant weight... </b>";
    if (useHtml)
      out << "</td><td style=\"padding-left:20px\" title=\"" << tooltipVDecomposition << "\">";
    theSl2.ElementToStringModuleDecomposition(useLatex, useHtml, tempS);
    out << tempS;
    if (useHtml)
      out << "</td><td>";
    out << theSl2.LengthHsquared;
    if (useHtml)
      out << "</td><td>";
    out << theSl2.LengthHsquared * this->GetOwnerWeyl().GetLongestRootLengthSquared()/4;
    if (useHtml)
      out << "</td><td>";
    for (int j=0; j<theSl2.IndicesMinimalContainingRootSA.size; j++)
    { rootSubalgebra& currentSA= this->theRootSAs.theSubalgebras[theSl2.IndicesMinimalContainingRootSA[j]];
      out << "<a href=\"" << displayPath << "rootHtml_rootSA" << theSl2.IndicesMinimalContainingRootSA[j] << ".html\">"
      << currentSA.theDynkinDiagram.ToStringRelativeToAmbientType(this->owner->theWeyl.theDynkinType[0]) << "</a>" << ";  ";
    }
    if (useHtml)
      out << "</td><td title=\"" << tooltipContainingRegular << "\">";
    for (int j=0; j<theSl2.IndicesContainingRootSAs.size; j++)
    { rootSubalgebra& currentSA= this->theRootSAs.theSubalgebras[theSl2.IndicesContainingRootSAs[j]];
      out << "<a href=\"" <<  displayPath << "rootHtml_rootSA" << theSl2.IndicesContainingRootSAs[j] << ".html\">"
      << currentSA.theDynkinDiagram.ToStringRelativeToAmbientType(this->owner->theWeyl.theDynkinType[0]) << "</a>" << ";  ";
    }
    if (useHtml)
      out << "</td></tr>\n";
  }
  if (useHtml)
    out << "</table><HR width=\"100%\">";
  out << out2.str() << "<hr>";
  return out.str();
}

std::string SltwoSubalgebras::ToString(FormatExpressions* theFormat)
{ std::string tempS; std::stringstream out; std::stringstream body;
  bool useHtml= theFormat==0 ? true : theFormat->flagUseHTML;
  for (int i=0; i<this->size; i++)
  { tempS=(*this)[i].ToString(theFormat);
  //  body<< "Index "<< i<<": ";
    if(useHtml)
      body << "<br>";
    body << tempS;
    if(useHtml)
      body << "<HR width=\"100%\">";
  }
  if(useHtml)
    out << "<br>";
  out << this->ElementToStringNoGenerators(theFormat);
  out << body.str();
  return out.str();
}

void SltwoSubalgebras::ElementToHtml(FormatExpressions* theFormat, GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("SltwoSubalgebras::ElementToHtml");
  std::string physicalPathSAs= theFormat==0 ? "": theFormat->PathPhysicalOutputFolder;
  std::string htmlPathServerSAs= theFormat==0 ? "": theFormat->PathDisplayOutputFolder;
  std::string physicalPathSl2s= theFormat==0 ? "": theFormat->PathPhysicalOutputFolder+"sl2s/";
  std::string htmlPathServerSl2s= theFormat==0 ? "": theFormat->PathDisplayOutputFolder+"sl2s/";
  std::string PathDisplayServerBaseFolder= theFormat==0 ? "../../": theFormat->PathDisplayServerBaseFolder;
  std::string DisplayNameCalculator= theFormat==0 ? "" : theFormat->PathDisplayNameCalculator;
  ProgressReport theReport(theGlobalVariables);
  theReport.Report("Preparing html pages for sl(2) subalgebras. This might take a while.");
  this->theRootSAs.ToHTML(theFormat, this);
  bool usePNG=true;
  if (physicalPathSAs=="")
    usePNG=false;
  if(usePNG)
  { int numExpectedFiles= this->size*8;
    this->texFileNamesForPNG.ReservE(numExpectedFiles);
    this->texStringsEachFile.ReservE(numExpectedFiles);
    this->listSystemCommandsLatex.ReservE(numExpectedFiles);
    this->listSystemCommandsDVIPNG.ReservE(numExpectedFiles);
  }
  this->texFileNamesForPNG.size=0;
  this->texStringsEachFile.size=0;
  this->listSystemCommandsLatex.size=0;
  this->listSystemCommandsDVIPNG.size=0;
  std::stringstream out, outNotation, outNotationCommand;
  std::string fileName;
  std::fstream theFile, fileFlas;
  outNotationCommand << "printSemisimpleLieAlgebra{}("
  << this->GetOwnerWeyl().theDynkinType.ToStringRelativeToAmbientType(this->GetOwnerWeyl().theDynkinType[0]) << ")" ;
  outNotation << "Notation, structure constants and Weyl group info: " << CGI::GetCalculatorLink(DisplayNameCalculator, outNotationCommand.str())
  << "<br> <a href=\"" << DisplayNameCalculator << "\"> Calculator main page</a><br><a href=\"../rootSubalgebras.html\">Root subsystem table</a><br>";
  std::string notation= outNotation.str();
  out << this->ToString(theFormat);
  std::string tempS;
  if(usePNG)
  { fileName= physicalPathSl2s;
    fileName.append("sl2s.html");
    XML::OpenFileCreateIfNotPresent(theFile, fileName, false, true, false);
    tempS= out.str();
    theFile << "<HMTL><title>sl(2)-subalgebras of "
    << this->theRootSAs.theSubalgebras[0].theDynkinDiagram.ToStringRelativeToAmbientType(this->owner->theWeyl.theDynkinType[0]) << "</title>";
    theFile << "<script src=\"../../../jsmath/easy/load.js\"></script> ";
    theFile << "<meta name=\"keywords\" content=\""
    <<  this->theRootSAs.theSubalgebras[0].theDynkinDiagram.ToStringRelativeToAmbientType(this->owner->theWeyl.theDynkinType[0])
    << " sl(2)-triples, sl(2)-subalgebras, nilpotent orbits simple Lie algebras, nilpotent orbits of "
    <<  this->theRootSAs.theSubalgebras[0].theDynkinDiagram.ToStringRelativeToAmbientType(this->owner->theWeyl.theDynkinType[0])
    << ", sl(2)-triples of "
    << this->theRootSAs.theSubalgebras[0].theDynkinDiagram.ToStringRelativeToAmbientType(this->owner->theWeyl.theDynkinType[0])
    << " \">";
    theFile << "<BODY>" << notation << "<a href=\"" << htmlPathServerSl2s << "sl2s_nopng.html\"> plain html for your copy+paste convenience</a><br>\n"
    << tempS << "</HTML></BODY>";
    theFile.close();
  }
  fileName= physicalPathSl2s;
  fileName.append("sl2s_nopng.html");
  bool tempB=theFormat->flagUsePNG;
  theFormat->flagUsePNG=false;
  tempS = this->ToString(theFormat);
  theFormat->flagUsePNG=tempB;
  XML::OpenFileCreateIfNotPresent(theFile, fileName, false, true, false);
  theFile << "<HMTL><BODY>" << notation << "<a href=\"" << htmlPathServerSl2s << "sl2s.html\"> .png rich html for your viewing pleasure</a><br>\n"
  << tempS << "</HTML></BODY>";
  theFile.close();
  if (usePNG)
  { this->listSystemCommandsLatex.SetSize(this->texFileNamesForPNG.size);
    this->listSystemCommandsDVIPNG.SetSize(this->texFileNamesForPNG.size);
    for (int i=0; i<this->texFileNamesForPNG.size; i++)
    { XML::OpenFileCreateIfNotPresent(fileFlas, this->texFileNamesForPNG[i], false, true, false);
      fileFlas << "\\documentclass{article}\\begin{document}\\pagestyle{empty}\n" << this->texStringsEachFile[i] << "\n\\end{document}";
      std::stringstream tempStreamLatex, tempStreamPNG;
      tempStreamLatex << "latex " << " -output-directory=" << physicalPathSl2s << " " << this->texFileNamesForPNG[i];
      tempS= this->texFileNamesForPNG[i];
      tempS.resize(tempS.size()-4);
      tempStreamPNG << "dvipng " << tempS << ".dvi -o " << tempS << ".png -T tight";
      this->listSystemCommandsLatex[i]= tempStreamLatex.str();
      this->listSystemCommandsDVIPNG[i]=tempStreamPNG.str();
      fileFlas.close();
    }
  }
}

bool CandidateSSSubalgebra::IsExtremeWeight(int moduleIndex, int indexInIsoComponent)const
{ Vector<Rational>& currentWeight=this->WeightsModulesPrimal[moduleIndex][indexInIsoComponent];
  Vectors<Rational>& currentWeights=this->WeightsModulesPrimal[moduleIndex];
  for (int j=0; j<this->primalSubalgebraModules.size; j++)
    for (int k=0; k<this->WeightsModulesPrimal[this->primalSubalgebraModules[j]].size; k++)
      if (!this->WeightsModulesPrimal[this->primalSubalgebraModules[j]][k].IsEqualToZero())
      { Vector<Rational>& currentRoot=this->WeightsModulesPrimal[this->primalSubalgebraModules[j]][k];
        if (currentWeights.Contains(currentWeight-currentRoot) && currentWeights.Contains(currentWeight+currentRoot))
          return false;
      }
  return true;
}

Rational CandidateSSSubalgebra::GetScalarSA(const Vector<Rational>& primalWeightLeft, const Vector<Rational>& primalWeightRight)const
{ return primalWeightLeft.ScalarProduct(primalWeightRight, this->BilinearFormFundPrimal);
}

std::string CandidateSSSubalgebra::ToStringDrawWeightsHelper(int indexModule, const Vector<Rational>& theWeight)const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ToStringDrawWeightsHelper");
  std::stringstream out;
  List<List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > >& currentMod=this->Modules[indexModule];
  FormatExpressions charFormat;
  charFormat.vectorSpaceEiBasisNames.SetSize(this->GetPrimalRank());
  for (int i=0; i<this->GetRank(); i++)
  { std::stringstream tempStream;
    tempStream << "\\\\omega_{" << i+1 << "}";
    charFormat.vectorSpaceEiBasisNames[i]=tempStream.str();
  }
  for (int i=this->GetRank(); i<this->GetPrimalRank(); i++)
  { std::stringstream tempStream;
    tempStream << "\\\\psi_{" << i+1 << "}";
    charFormat.vectorSpaceEiBasisNames[i]=tempStream.str();
  }
  for (int i=0; i<currentMod.size; i++)
  { out << "<table style=\\\"border:1px solid #000;\\\">";
    out << "<tr><td colspan=\\\"3\\\">"
    << "<span class=\\\"math\\\">"
    << "V_{" << this->HighestWeightsPrimal[indexModule].ToStringLetterFormat("\\\\omega", &charFormat) << "}"
    << " </span></td></tr>"
    << "<tr>"
    << "<td style=\\\"text-align: center;\\\">basis</td>"
    << "<td style=\\\"text-align: center; border-left:1px solid #000; white-space: nowrap;\\\">weights simple coords.</td>"
    << "<td style=\\\"text-align: center; border-left:1px solid #000; white-space: nowrap;\\\">weights fund.coords.</td>"
    << "<td style=\\\"text-align: center; border-left:1px solid #000; white-space: nowrap;\\\">weights primal</td>"
    << "</tr>";
    for (int j=0; j<currentMod[i].size; j++)
    { std::string openTag="", closeTag="";
      if (this->WeightsModulesPrimal[indexModule][j]==theWeight)
      { openTag="<span style=\\\"color:#FF0000\\\">";
        closeTag="</span>";
      }
      out << "<tr>";

      out << "<td style=\\\"text-align: center;\\\">" << openTag;
      out << "<span class=\\\"math\\\">" << CGI::DoubleBackslashes(currentMod[i][j].ToString()) << "</span>";
      out << closeTag << "</td>";

      Vector<Rational> tempV;
      this->MatMultiplyFundCoordsToGetSimple.ActOnVectorColumn(this->WeightsModulesPrimal[indexModule][j],tempV);
      out << "<td style=\\\"text-align: center; border-left:1px solid #000;\\\">" << openTag;
      out << tempV.ToString();
      out << closeTag << "</td>";

      out << "<td style=\\\"text-align: center; border-left:1px solid #000;\\\">" << openTag;
      out << "<span class=\\\"math\\\">"
      << this->WeightsModulesNONprimal[indexModule][j].ToStringLetterFormat("\\\\omega", &charFormat) << "</span>";
      out << closeTag << "</td>";

      out << "<td style=\\\"text-align: center; border-left:1px solid #000;\\\">" << openTag;
      out << "<span class=\\\"math\\\">"
      << this->WeightsModulesPrimal[indexModule][j].ToStringLetterFormat("\\\\omega", &charFormat) << "</span>";
      out << closeTag << "</td>";
      out << "</tr>";
    }
    out << "</table>";
  }
  return out.str();
}

std::string CandidateSSSubalgebra::ToStringDrawWeights(FormatExpressions* theFormat)const
{ if (!this->flagCentralizerIsWellChosen)
    return "";
  MacroRegisterFunctionWithName("CandidateSSSubalgebra::ToStringDrawWeights");
  if (theFormat!=0)
    if (!theFormat->flagIncludeMutableInformation)
      return "<br>Weight diagram not drawn to avoid javascript problems (use command PrintSemisimpleSubalgebras if you want to see the weight diagram). ";
  int thePrimalRank=-1;
  (this->centralizerRank+this->theHs.size).IsSmallInteger(&thePrimalRank);
  if (thePrimalRank<2)
    return "";
  std::stringstream out;
  if (thePrimalRank!=this->BilinearFormFundPrimal.NumCols)
  { out << "<br>The primal rank was computed to be " << thePrimalRank << " but the bilinear form in fundamental coordinates relative to "
    << " the subalgebra was not computed: this->BilinearFormFundPrimal has " << this->BilinearFormFundPrimal.NumRows << " rows. ";
    return out.str();
  }
  out << "<br>Weight diagram. The coordinates corresponding to the simple roots of the subalgerba are fundamental.  "
  << "<br>The bilinear form is therefore given relative to the fundamental coordinates.<br> ";

//  out << "Embeddings fund coords into fund coords ambient.";
//  Matrix<Rational> theMat=this->MatMultiplyFundCoordsToGetSimple;
//  theMat.


  Vectors<Rational> BasisToDrawCirclesAt;
  DrawingVariables theDV;
  theDV.theBuffer.theBilinearForm.init(thePrimalRank, thePrimalRank);
  for (int i=0; i<thePrimalRank; i++)
    for (int j=0; j<thePrimalRank; j++)
      theDV.theBuffer.theBilinearForm(i,j)=this->BilinearFormFundPrimal(i,j).DoubleValue();
  Vector<Rational> zeroVector;
  zeroVector.MakeZero(thePrimalRank);
  BasisToDrawCirclesAt.MakeEiBasis(thePrimalRank);
  Vectors<Rational> cornerWeights;
  int maxModDim=0;
  for (int i=0; i<this->Modules.size; i++)
    maxModDim=MathRoutines::Maximum(this->Modules[i][0].size, maxModDim);
  theDV.DefaultHtmlHeight= MathRoutines::Maximum(600, maxModDim*35);
  for (int i=0; i<this->Modules.size; i++)
  { cornerWeights.SetSize(0);
    for (int j=0; j<this->Modules[i].size; j++)
      for (int k=0; k<this->Modules[i][j].size; k++)
      { if (j==0)
        { int color= CGI::RedGreenBlue(150,150,0);
          if (this->primalSubalgebraModules.Contains(i))
          { color=CGI::RedGreenBlue(0,250,0);
            theDV.drawLineBetweenTwoVectorsBuffer(zeroVector, this->WeightsModulesPrimal[i][k], theDV.PenStyleNormal, color);
          }
          theDV.drawCircleAtVectorBuffer(this->WeightsModulesPrimal[i][k], 2, theDV.PenStyleNormal, color);
          if (this->IsExtremeWeight(i, k))
            cornerWeights.AddOnTop(this->WeightsModulesPrimal[i][k]);
        }
        if (k==this->Modules[i][j].size-1 && BasisToDrawCirclesAt.size<thePrimalRank)
        { BasisToDrawCirclesAt.AddOnTop(this->WeightsModulesPrimal[i][k]);
          if (BasisToDrawCirclesAt.GetRankOfSpanOfElements()!=BasisToDrawCirclesAt.size)
            BasisToDrawCirclesAt.RemoveLastObject();
        }
      }
    int color=CGI::RedGreenBlue(250, 250,0);
    if (this->primalSubalgebraModules.Contains(i))
      color=CGI::RedGreenBlue(0, 0, 250);
    for (int j=0; j<cornerWeights.size; j++)
    { Rational minDist=0;
      for (int k=0; k<cornerWeights.size; k++)
      { Rational tempRat=Vector<Rational>::ScalarProduct((cornerWeights[k]-cornerWeights[j]), (cornerWeights[k]-cornerWeights[j]), this->BilinearFormFundPrimal);
        if (minDist==0)
          minDist=tempRat;
        else if (tempRat!=0)
          minDist=MathRoutines::Minimum(tempRat, minDist);
      }
      for (int k=j+1; k<cornerWeights.size; k++)
        if (minDist==Vector<Rational>::ScalarProduct((cornerWeights[k]-cornerWeights[j]), (cornerWeights[k]-cornerWeights[j]), this->BilinearFormFundPrimal))
          theDV.drawLineBetweenTwoVectorsBuffer(cornerWeights[k], cornerWeights[j], theDV.PenStyleNormal, color);
    }
  }
  theDV.theBuffer.labeledVectors.SetSize(0);
  theDV.theBuffer.labelsOfLabeledVectors.SetSize(0);
  theDV.theBuffer.toBeHighlightedWhenLabeledVectorHovered.SetSize(0);
  HashedList<Vector<Rational> > currentWeights;
  for (int i=0; i<this->Modules.size; i++)
  { currentWeights.Clear();
    currentWeights.AddOnTopNoRepetition(this->WeightsModulesPrimal[i]);
    Vectors<double> currentWeightsDouble;
    currentWeightsDouble.SetSize(currentWeights.size);
    for (int j=0; j<currentWeightsDouble.size; j++)
      currentWeightsDouble[j]=currentWeights[j].GetVectorDouble();
    for (int j=0; j<currentWeights.size; j++)
    { theDV.theBuffer.labeledVectors.AddOnTop(currentWeights[j].GetVectorDouble());
      theDV.theBuffer.toBeHighlightedWhenLabeledVectorHovered.AddOnTop(currentWeightsDouble);
      theDV.theBuffer.labelsOfLabeledVectors.AddOnTop(this->ToStringDrawWeightsHelper(i, currentWeights[j]));
    }
  }
  theDV.theBuffer.BasisToDrawCirclesAt.SetSize(BasisToDrawCirclesAt.size);
  for(int i=0; i<theDV.theBuffer.BasisToDrawCirclesAt.size; i++)
  { theDV.theBuffer.BasisToDrawCirclesAt[i].SetSize(thePrimalRank);
    for (int j=0; j<thePrimalRank; j++)
      theDV.theBuffer.BasisToDrawCirclesAt[i][j]=BasisToDrawCirclesAt[i][j].DoubleValue();
    theDV.drawCircleAtVectorBuffer(BasisToDrawCirclesAt[i], 4, theDV.PenStyleNormal, CGI::RedGreenBlue(250, 0,0));
    //theDV.drawTextAtVectorBuffer
    //(BasisToDrawCirclesAt[i], BasisToDrawCirclesAt[i].ToString(), CGI::RedGreenBlue(0, 0,0), theDV.TextStyleNormal, 0);
  }
//  theDV.theBuffer.GraphicsUnit[0]/=sqrt(this->theWeylNonEmbeddeD.CartanSymmetric(0,0).DoubleValue());
  out << theDV.GetHtmlFromDrawOperationsCreateDivWithUniqueName(thePrimalRank);
  return out.str();
}

std::string CandidateSSSubalgebra::ToStringModuleDecompoLaTeX(FormatExpressions* theFormat)const
{ if (this->Modules.size<=0)
    return "";
  MacroRegisterFunctionWithName("CandidateSSSubalgebra::ToStringModuleDecompoLaTeX");
  if (!this->owner->flagComputeModuleDecomposition)
    return "";
  if (!this->flagCentralizerIsWellChosen)
    return "";
  std::stringstream out;
  //out << "Isotypic module decomposition over primal subalgebra (total " << this->Modules.size << " isotypic components). ";
  FormatExpressions tempCharFormat;
  if (!this->charFormaT.IsZeroPointer())
    tempCharFormat= this->charFormaT.GetElementConst();
  //bool useModuleIndex=false;
  out << "\\documentclass{article}\\usepackage{amssymb}\\usepackage{longtable}\\usepackage{multirow}\\begin{document}" ;
  out << "<br> \\begin{longtable}{c|c|c|c|c|c}\n<br>\n\\caption{Module decomposition of $"
  << this->owner->owneR->theWeyl.theDynkinType.ToString() << "$ over $"
  << this->theWeylNonEmbeddeD.theDynkinType.ToStringRelativeToAmbientType(this->GetAmbientWeyl().theDynkinType[0])
  << " \\oplus \\mathfrak h_c$\\label{tableModuleDecompo} }\\\\"
  << "Component &Module & elements & elt. weights& $h$-element $\\mathfrak k-sl(2)$-triple& $f$-element $\\mathfrak k-sl(2)$-triple\\\\";
  ElementSemisimpleLieAlgebra<AlgebraicNumber> tempLieBracket;
  for (int i=0; i<this->Modules.size; i++)
  { int numRowsIsoCompo=this->Modules[i].size*this->Modules[i][0].size;
    if (numRowsIsoCompo>1)
      out << "\\multirow{" << numRowsIsoCompo << "}{*}";
    out << "{$W_{" << i+1 << "} $}";
    out << "&";
    for (int j=0; j<this->Modules[i].size; j++)
    { if (j>0)
        out << " & ";
      if (this->Modules[i][j].size>1)
        out << "\\multirow{" << this->Modules[i][j].size << "}{*}";
      out << "{$V_{" << this->HighestWeightsPrimal[i].ToStringLetterFormat("\\omega", &tempCharFormat) << "}$} ";
      out << "&";
      for (int k=0; k<this->Modules[i][j].size; k++)
      { if (k>0)
          out << "&&";
        out << "$" << this->Modules[i][j][k].ToString(theFormat) << "$";
        out << "&";
        out << "$" << this->WeightsModulesPrimal[i][k].ToStringLetterFormat("\\omega", &tempCharFormat) << "$" << "& ";
        bool OpsAreGood=false;
        if (i<this->ModulesSl2opposite.size)
          if (j<this->ModulesSl2opposite[i].size)
            if (k<this->ModulesSl2opposite[i][j].size)
              OpsAreGood=true;
        if (OpsAreGood)
        { List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& currentOpModule=this->ModulesSl2opposite[i][j];
          if (!currentOpModule[k].IsEqualToZero())
          { this->GetAmbientSS().LieBracket(this->Modules[i][j][k], currentOpModule[k], tempLieBracket);
            out << "$" << tempLieBracket.ToString() << "$&";
            out << "$" << currentOpModule[k].ToString() << "$";
          } else
            out << "$N/A$&N/A";
        }
        out << "\\\\" << " <br>";
      }
      out << "\\cline{2-6}";
    }
    out << "\\hline";
  }
  out << "\\end{longtable}<br>\n\\end{document}";
  return out.str();
}


std::string CandidateSSSubalgebra::ToStringModuleDecompo(FormatExpressions* theFormat)const
{ if (this->Modules.size<=0)
    return "";
  MacroRegisterFunctionWithName("CandidateSSSubalgebra::ToStringModuleDecompo");
  bool useMouseHover=theFormat==0 ? true : !theFormat->flagUseMathSpanPureVsMouseHover;
  std::stringstream out;
  out << "Isotypic module decomposition over primal subalgebra (total " << this->Modules.size << " isotypic components). ";
  out << "<table border=\"1\">";
  FormatExpressions tempCharFormat;
  if (!this->charFormaT.IsZeroPointer())
    tempCharFormat= this->charFormaT.GetElementConst();
  out << "<tr><td>Isotypical components + highest weight</td>";
  for (int i=0; i<this->HighestWeightsPrimal.size; i++)
  { bool visible=true;
    bool isDouble=false;
    if (i>0)
      if (this->HighestWeightsPrimal[i]==this->HighestWeightsPrimal[i-1])
        visible=false;
    if (i<this->HighestWeightsPrimal.size-1)
      if (this->HighestWeightsPrimal[i]==this->HighestWeightsPrimal[i+1])
        isDouble=true;
    if (visible)
    { if (isDouble)
        out << "<td colspan=\"2\">";
      else
        out << "<td>";
      std::stringstream tempStream;
      tempStream << "V_{" << this->HighestWeightsPrimal[i].ToStringLetterFormat("\\omega", &tempCharFormat) << "} ";
      if (useMouseHover)
        out << CGI::GetMathMouseHover(tempStream.str()) << "-> " << this->HighestWeightsPrimal[i].ToString();
      else
        out << CGI::GetMathSpanPure(tempStream.str()) << "-> " << this->HighestWeightsPrimal[i].ToString();
      out << "</td>";
    }
  }
  out << "</tr>";
  out << "<tr><td>Module label </td>";
  for (int i=0; i<this->HighestWeightsPrimal.size; i++)
    out << "<td>" << "W_{" << i+1 << "}" << "</td>";
  out << "</tr>";
  out <<"<tr><td>Module elements (weight vectors). <span style=\"color:#0000FF\">In blue - corresp. F element</span>. "
  << "<span style=\"color:#FF0000\">In red -corresp. H element</span>. </td>";
  ElementSemisimpleLieAlgebra<AlgebraicNumber> tempLieBracket;
  for (int i=0; i<this->Modules.size; i++)
  { out << "<td>";
    if (this->primalSubalgebraModules.Contains(i))
    { if (this->subalgebraModules.Contains(i))
        out << "Semisimple subalgebra component. ";
      else
        out << "Cartan of centralizer component. ";
    }
    out << "<table border=\"1\"><tr>";
    for (int j=0; j<this->Modules[i].size; j++)
    { List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& currentModule=this->Modules[i][j];
      out << "<td>";
      out << "<table>";
      for (int k=0; k<currentModule.size; k++)
      { out << "<tr><td>" << currentModule[k].ToString() << "</td>";
        bool OpsAreGood=false;
        if (i<this->ModulesSl2opposite.size)
          if (j<this->ModulesSl2opposite[i].size)
            if (k<this->ModulesSl2opposite[i][j].size)
              OpsAreGood=true;
        if (OpsAreGood)
        { List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& currentOpModule=this->ModulesSl2opposite[i][j];
          if (!currentOpModule[k].IsEqualToZero())
          { out << "<td><span style=\"color:#0000FF\">" << currentOpModule[k].ToString() << "</span></td>";
            this->GetAmbientSS().LieBracket(currentModule[k], currentOpModule[k], tempLieBracket);
            out << "<td><span style=\"color:#FF0000\">" << tempLieBracket.ToString() << "</span></td>";
          }
          else
            out << "<td><span style=\"color:#0000FF\"> N/A </span></td>";
        }
        out << "</tr>";
      }
      out << "</table>";
      out << "</td>";
    }
//  out << "<td>|||</td><td>Union of the isotypic components:<br>";
//  for (int j=0; j<this->ModulesIsotypicallyMerged[i].size; j++)
//  { out << this->ModulesIsotypicallyMerged[i][j].ToString() << "<br>";
//  }
//  out << "</td>";
    out << "</tr></table>";
    out << "</td>";
  }
  out << "</tr>";
  out << "<tr>";
  out << "<td>weights of elements in fundamental coords w.r.t. Cartan of subalgebra in same order as above</td>";
  for (int i=0; i<this->WeightsModulesNONprimal.size; i++)
  { out << "<td>";
    for (int j=0; j<this->WeightsModulesNONprimal[i].size; j++)
    { out << this->WeightsModulesNONprimal[i][j].ToStringLetterFormat("\\omega", &tempCharFormat);
      if (j!=this->WeightsModulesNONprimal[i].size-1)
        out << "<br>";
    }
    out << "</td>";
  }
  out << "</tr>";
  out << "<tr>";
  out << "<td>weights of elements in (fundamental coords w.r.t. Cartan of subalgebra)+ Cartan centralizer</td>";
  for (int i=0; i<this->WeightsModulesPrimal.size; i++)
  { out << "<td>";
    for (int j=0; j<this->WeightsModulesPrimal[i].size; j++)
    { out << this->WeightsModulesPrimal[i][j].ToStringLetterFormat("\\omega", &tempCharFormat);
      if (j!=this->WeightsModulesPrimal[i].size-1)
        out << "<br>";
    }
    out << "</td>";
  }
  out << "</tr>";
  out << "<tr>";
  tempCharFormat.FDrepLetter="M";
  out << "<td>Single module character over Cartan of s.a.+ Cartan of centralizer of s.a.</td>";
  if (this->CharsPrimalModules.size>0)
    for (int i=0; i<this->CharsPrimalModules.size; i++)
    { out << "<td>";
      if (useMouseHover)
        out << CGI::GetMathMouseHover(this->CharsPrimalModules[i].ToString(&tempCharFormat));
      else
        out << CGI::GetMathSpanPure(this->CharsPrimalModules[i].ToString(&tempCharFormat));
      out << "</td>";
    }
  else
    for (int i=0; i<this->Modules.size; i++)
      out << "<td>Not computed.</td>";
  out << "</tr>";
  out << "<tr>";
  out << "<td>Isotypic character</td>";
  if (this->CharsPrimalModulesMerged.size>0)
    for (int i=0; i<this->CharsPrimalModulesMerged.size; i++)
    { out << "<td>";
      if (useMouseHover)
        out << CGI::GetMathMouseHover(this->CharsPrimalModulesMerged[i].ToString(&tempCharFormat));
      else
        out << CGI::GetMathSpanPure(this->CharsPrimalModulesMerged[i].ToString(&tempCharFormat));
      out << "</td>";
    }
  else
    for (int i=0; i<this->Modules.size; i++)
      out << "<td>Not computed.</td>";
  out << "</tr>";
  out << "</table>";
  Vector<Rational> semisimpleSAv, centralizerV;
  semisimpleSAv.MakeZero(this->Modules.size);
  centralizerV.MakeZero(this->Modules.size);
  for (int i=0; i<this->subalgebraModules.size; i++)
    semisimpleSAv[this->subalgebraModules[i]]+=1;
  for (int i=0; i<this->centralizerSubalgebraModules.size; i++)
    centralizerV[this->centralizerSubalgebraModules[i]]+=1;
  out << "<br>Semisimple subalgebra: " << semisimpleSAv.ToStringLetterFormat("W");
  out << "<br>Centralizer extension: " << centralizerV.ToStringLetterFormat("W");
  return out.str();
}

std::string NilradicalCandidate::ToStringTableElementWithWeights(const List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& theElts, const Vectors<Rational>& theWeights)const
{ std::stringstream out;
  out << "<table border=\"1\"><tr>";
  for (int i=0; i<theWeights.size; i++)
  { out << "<td><table><tr><td>" << theWeights[i].ToString() << "</td></tr>";
    for (int j=0; j<theWeights.size; j++)
      if (theWeights[i]==theWeights[j])
        out << "<tr><td>" << theElts[j].ToString() << "</td></tr>";
    out << "</table></td>";
  }
  out << "</tr></table>";
  return out.str();
}

std::string NilradicalCandidate::ToString(FormatExpressions* theFormat)const
{ MacroRegisterFunctionWithName("NilradicalCandidate::ToString");
  std::stringstream out;
  out << this->FKnilradicalLog;
  Vector<Rational> currentNilrad;
  currentNilrad=this->theNilradicalSelection;
  out << currentNilrad.ToStringLetterFormat("W");
  if (this->flagNilradicalConesIntersect)
    out << ". Cones intersect. ";
  else
    out << ". Cones don't intersect. ";
  if (this->theNilradicalWeights.size>0)
    out << "<br>Nilradical cone: <br>" << this->ToStringTableElementWithWeights(this->theNilradical, this->theNilradicalWeights);
  out << "<br> Highest weight cone:<br> " << this->ToStringTableElementWithWeights(this->theNonFKhwVectors, this->theNonFKhws);
  if (this->flagNilradicalConesIntersect)
  { out << "<br>Strongly singular weights: " << this->theNonFKhwsStronglyTwoSided.ToString();
    out << "<br>Cone intersection: " << this->ConeIntersection.ToStringLetterFormat("w");
    out << "<br> ";
    FormatExpressions tempFormat;
    tempFormat.vectorSpaceEiBasisNames.SetSize(this->ConeIntersection.size);
    for (int j=0; j<this->theNilradicalWeights.size; j++)
      tempFormat.vectorSpaceEiBasisNames[j]=this->theNilradicalWeights[j].ToString();
    for (int j=0; j<this->theNonFKhws.size; j++)
      tempFormat.vectorSpaceEiBasisNames[j+this->theNilradicalWeights.size]=this->theNonFKhws[j].ToString();
    out << this->ConeIntersection.ToStringLetterFormat("w", &tempFormat);
    if (this->flagNilradicalConesStronglyIntersect)
    { for (int j=0; j<this->theNonFKhwsStronglyTwoSided.size; j++)
        tempFormat.vectorSpaceEiBasisNames[j+this->theNilradicalWeights.size]=this->theNonFKhwsStronglyTwoSided[j].ToString();
      out << "<br>In addition, the nilradical cones intersect strongly " << "at weight " << this->GetConeStrongIntersectionWeight().ToString();
      out << "<br>" << this->ConeStrongIntersection.ToStringLetterFormat("w", &tempFormat);
      out << "<br>The involved nilradical elements: " << "<br><table><tr>";
      for (int i=0; i<this->theNilradicalWeights.size; i++)
        if (this->ConeStrongIntersection[i]!=0)
        { Vector<Rational>& currentNilradWeight= this->theNilradicalWeights[i];
          out << "<td><table border=\"1\"><tr><td>" << currentNilradWeight.ToString() << "</td></tr>";
          for (int j=0; j<this->theNilradicalWeights.size; j++)
            if (currentNilradWeight==this->theNilradicalWeights[j])
            { out << "<tr><td>" << this->theNilradical[j].ToString() << "</td></tr>";
              out << "<tr><td>" << this->theNilradicalElementOpposites[j].ToString() << "</td></tr>";
            }
          out << "<tr><td>Scal. prod. <br> L-inf. rel.: " << this->owner->GetScalarSA(currentNilradWeight, this->GetConeStrongIntersectionWeight())
          << "</td></td>";
          out << "</table>";
          out << "</td>";
        }
      out << "</tr></table>";
    }
    if (this->flagComputedRelativelyStrongIntersections && this->flagLinfiniteRelFound)
    { tempFormat.vectorSpaceEiBasisNames.SetSize(this->ConeRelativelyStrongIntersection.size);
      for (int j=0; j<this->theNilradicalSubset.size; j++)
        tempFormat.vectorSpaceEiBasisNames[j]=this->theNilradicalSubsetWeights[j].ToString();
      for (int j=0; j<this->theNonFKhwVectorsStrongRelativeToSubset.size; j++)
        tempFormat.vectorSpaceEiBasisNames[j+this->theNilradicalSubset.size]=this->theNonFKhwVectorsStrongRelativeToSubsetWeights[j].ToString();
      out << "<br>In addition, strong nilradical cone intersections did not suffice, I needed to compute relatively strong intersections. ";
      out << "<br>The nilradical subset:" << this->ToStringTableElementWithWeights(this->theNilradicalSubset, this->theNilradicalSubsetWeights);
      out << "<br>The highest weight cone consisting of elements commuting with the nilradical subset: "
      << this->ToStringTableElementWithWeights(this->theNonFKhwVectorsStrongRelativeToSubset, this->theNonFKhwVectorsStrongRelativeToSubsetWeights);
      out << "<br>Relation: " << this->ConeRelativelyStrongIntersection.ToStringLetterFormat("w", &tempFormat);
    }
    if (this->flagLinfiniteRelFound)
      out << "<br><b>L-infinite relation exists!</b>";
    else
      out << "<br><span style=\"color:#FF0000\"><b>No L-infinite relation found.</b></span>";
  } else
  { if (!this->flagParabolicACextendsToParabolicAC)
      out << "<br><span style=\"color:#FF0000\"><b>Strong centralizer condition not proven to hold.</b></span>";
    if (this->flagRestrictedCentralizerConditionHoldS)
      out << "<br>The centralizer condition holds. ";
    out << "<br>Levi roots primal fundamental coordinates: " << this->leviRootsSmallPrimalFundCoords.ToString()
    << "<br>Levi components in centralizer: "
    << this->theLeviDiagramSmalL.ToStringRelativeToAmbientType(this->owner->owner->owneR->theWeyl.theDynkinType[0])
    << "<br>Levi roots containing parabolic: " << this->leviRootsAmbienT.ToString()
    << "<br>Levi components containing parabolic: "
    << this->theLeviDiagramAmbienT.ToStringRelativeToAmbientType(this->owner->owner->owneR->theWeyl.theDynkinType[0]);
    out << "<br>Separating hyperplane: " << this->ConeSeparatingNormal.ToStringLetterFormat("u");
  }
  return out.str();
}

void NilradicalCandidate::GetModGeneratedByNonHWVandNilradElt
(int indexInNilradSubset, List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& outputLeft, List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& outputRight, List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& outputBrackets)const
{ MacroRegisterFunctionWithName("NilradicalCandidate::GetModGeneratedByNonHWVandNilradElt");
  outputBrackets.SetSize(0);
  outputBrackets.AddOnTop(this->theNilradicalSubset[indexInNilradSubset]);
  outputRight.SetSize(1);
  outputLeft.SetSize(1);
  ElementSemisimpleLieAlgebra<AlgebraicNumber> theLieBracket;
  for (int i=0; i<outputBrackets.size; i++)
    if (!outputBrackets[i].IsEqualToZero())
      for (int j=0; j<this->theNonFKhwVectorsStrongRelativeToSubset.size; j++)
      { this->owner->owner->owneR->LieBracket(this->theNonFKhwVectorsStrongRelativeToSubset[j], outputBrackets[i], theLieBracket);
        outputBrackets.AddOnTop(theLieBracket);
        outputLeft.AddOnTop(this->theNonFKhwVectorsStrongRelativeToSubset[j]);
        outputRight.AddOnTop(outputBrackets[i]);
      }
  outputLeft.PopIndexShiftDown(0);
  outputRight.PopIndexShiftDown(0);
  outputBrackets.PopIndexShiftDown(0);
}

std::string CandidateSSSubalgebra::ToStringNilradicalsSummary(FormatExpressions* theFormat)const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ToStringNilradicalsSummary");
  if (this->FKNilradicalCandidates.size==0)
    return "";
  std::stringstream out;
  out << "<br>There are " << this->FKNilradicalCandidates.size << " possible isotypic nilradical extensions of the primal subalgebra. Of them "
  << this->NumConeIntersections << " have intersecting cones. Of the remaining "
  << this->FKNilradicalCandidates.size-this->NumConeIntersections << " nilradical extensions with non-intersecting cones, "
  << this->FKNilradicalCandidates.size-this->NumConeIntersections-this->NumCentralizerConditionFailsConeConditionHolds
  << " satisfy the centralizer condition and " << this->NumCentralizerConditionFailsConeConditionHolds << " fail the centralizer condition.";
  if (this->NumBadParabolics>0)
    out << "<br><span style=\"color:#FF0000\">Of the subalgebra(s) satisfying the centralizer condition,  "
    << this->NumBadParabolics << " have (has) centralizer parabolics that do not extend to  "
    << " parabolics of the ambient Lie algebra with Levi types A and C. For these subalgebras the PSZ construction is not proven to hold. </span>";
  else
    out << "<br><span style=\"color:#0000FF\"> In each of "
    << this->FKNilradicalCandidates.size-this->NumConeIntersections-this->NumCentralizerConditionFailsConeConditionHolds
    << " case(s) when the centralizer condition holds, the parabolic subalgebra in the centralizer with Levi types A and C extends "
    << "to parabolic subalgebra of the ambient Lie algebra whose Levi types are A and C only. </span>";
  if (this->NumConeIntersections>0)
  { if (this->NumCasesNoLinfiniteRelationFound>0)
      out << "<br><span style=\"color:#FF0000\">In " << this->NumCasesNoLinfiniteRelationFound << " cases no L-infinite relation was found. </span>";
    else
      out << "<br><span style=\"color:#0000FF\"> In each of " << this->NumConeIntersections << " case(s) of intersecting cones, an L-infinite relation was found. </span>";
  }
  return out.str();
}

std::string CandidateSSSubalgebra::ToStringNilradicals(FormatExpressions* theFormat)const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ToStringNilradicals");
  if (this->FKNilradicalCandidates.size==0)
  { if (this->owner->flagComputeNilradicals)
    { if (this->AmRegularSA())
        return "Nilradicals not computed, but that is OK because this a root subalgebra. ";
      else
        return "<b>Nilradicals not computed AND this is not a root subalgebra.</b>";
    } else
      return "";
  }
  std::stringstream out;
  out << this->ToStringNilradicalsSummary(theFormat);
  Vector<Rational> primalBase;
  primalBase = this->FKNilradicalCandidates[0].theNilradicalSelection;
  out << "<br>The primal extension of the semisimple subalgerba equals: " << primalBase.ToStringLetterFormat("W");
  if (this->owner->flagProduceLaTeXtables)
  { Vector<Rational> currentNilradVector;
    out << "<br><br>A summary in LaTeX <br>\\begin{longtable}{c|c|c|c }"
    << "\\caption{Nilradicals\\label{tableNilrads} }\\\\ $ \\mathfrak n _{\\mathfrak l} $ & Cones intersect & Cone intersection ";
    FormatExpressions tempFormat;
    if (!this->charFormaT.IsZeroPointer())
      tempFormat= this->charFormaT.GetElementConst();
    List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > RelevantBracketsLeft, RelevantBracketsRight, RelevantBrackets;
    for (int i=0; i<this->FKNilradicalCandidates.size; i++)
    { const NilradicalCandidate& currentNilrad=this->FKNilradicalCandidates[i];
      currentNilradVector=currentNilrad.theNilradicalSelection;
      for (int j=0; j<this->primalSubalgebraModules.size; j++)
        currentNilradVector[this->primalSubalgebraModules[j]]-=1;
      out << "\\\\\\hline<br>\n";
      out << "$" << currentNilradVector.ToStringLetterFormat("W") << "$ &";
      if (currentNilrad.flagNilradicalConesIntersect)
        out << "yes";
      else
        out << "no";
      out << "&";
      if (currentNilrad.flagLinfiniteRelFound)
      { out << "\\begin{tabular}{l";
        for (int j=0; j<currentNilrad.theNonFKhwVectorsStrongRelativeToSubsetWeights.size+currentNilrad.theNilradicalSubsetWeights.size; j++)
          out << "cc";
        out << "}Relation&";
        for (int j=0; j<currentNilrad.theNonFKhwVectorsStrongRelativeToSubsetWeights.size; j++)
        { Rational theCF=currentNilrad.ConeRelativelyStrongIntersection[currentNilrad.theNilradicalSubsetWeights.size+j];
          theCF.Minus();
          out << "$" << (currentNilrad.theNonFKhwVectorsStrongRelativeToSubsetWeights[j]*theCF).ToStringLetterFormat("\\omega", &tempFormat)
          << "$";
          out << " & ";
          if (j!=currentNilrad.theNonFKhwVectorsStrongRelativeToSubsetWeights.size-1)
            out << "+&";
          else
            out << "=&";
        }
        for (int j=0; j<currentNilrad.theNilradicalSubsetWeights.size; j++)
        { Rational theCF=currentNilrad.ConeRelativelyStrongIntersection[j];
          out << "$" << (currentNilrad.theNilradicalSubsetWeights[j]*theCF).ToStringLetterFormat("\\omega", &tempFormat) << "$";
          out << " & ";
          if (j!=currentNilrad.theNilradicalSubsetWeights.size-1)
            out << "+&";
        }
        out << "\\\\<br>\n Elts.& ";
        for (int j=0; j<currentNilrad.theNonFKhwVectorsStrongRelativeToSubset.size; j++)
        { out << "$" << currentNilrad.theNonFKhwVectorsStrongRelativeToSubset[j].ToString() << "$";
          out << " & &";
        }
        for (int j=0; j<currentNilrad.theNilradicalSubset.size; j++)
        { out << "$" << currentNilrad.theNilradicalSubset[j].ToString() << "$";
          out << " & ";
          if (j!=currentNilrad.theNilradicalSubset.size-1)
            out << " &";
        }
        out << "\\\\<br>\n Opposite elts. &";
        for (int j=0; j<currentNilrad.theNonFKhwVectorsStrongRelativeToSubset.size; j++)
          out << " & &";
        for (int j=0; j<currentNilrad.theNilradicalSubset.size; j++)
        { out << "$" << currentNilrad.theNilradicalElementOpposites[currentNilrad.theNilradSubsel.elements[j]].ToString() << "$";
          out << " & ";
          if (j!=currentNilrad.theNilradicalSubset.size-1)
            out << " &";
        }
        out << "\\end{tabular}\\\\\n<br>";
        out << "&& Relevant Lie brackets: ";
        ElementSemisimpleLieAlgebra<AlgebraicNumber> tempElt;
        std::stringstream tempStream;
        for (int j=0; j<currentNilrad.theNilradicalSubset.size; j++)
        { currentNilrad.GetModGeneratedByNonHWVandNilradElt(j, RelevantBracketsLeft, RelevantBracketsRight, RelevantBrackets);
          for (int k=0; k<RelevantBrackets.size; k++)
            tempStream << "$[" << RelevantBracketsLeft[k].ToString() << ", " << RelevantBracketsRight[k].ToString() << "] ="
            << RelevantBrackets[k].ToString() << "$, ";
        }
        for (int j=0; j<currentNilrad.theNilradicalSubset.size; j++)
          for (int k=0; k<currentNilrad.theNilradicalSubset.size; k++)
            if (k!=j)
            { this->owner->owneR->LieBracket(currentNilrad.theNilradicalSubset[j], currentNilrad.theNilradicalElementOpposites[currentNilrad.theNilradSubsel.elements[k]], tempElt);
              tempStream << "$[" << currentNilrad.theNilradicalSubset[j].ToString() << ", "
              << currentNilrad.theNilradicalElementOpposites[currentNilrad.theNilradSubsel.elements[k]].ToString()
              << "]=" << tempElt.ToString() << ", ";
            }
        std::string tempS=tempStream.str();
        tempS.resize(tempS.size()-2);
        out << tempS;
      }

  //    for (int j=0; j<currentNilrad.theNonFKhws.size; j++)
  //    { out << " $" << currentNilrad.theNonFKhws[j].ToStringLetterFormat("\\omega", &tempFormat) << "$";
  //      if (j!=currentNilrad.theNonFKhws.size-1)
  //        out << ", ";
  //    }
    }
    out << "\\end{longtable}";
  }
  for (int i=0; i<this->FKNilradicalCandidates.size; i++)
    out << "<hr>Subalgebra " << i+1 << ": " << this->FKNilradicalCandidates[i].ToString(theFormat);
  if (this->nilradicalGenerationLog!="")
    out << "<br>Nilradical generation log:" << this->nilradicalGenerationLog;
  return out.str();
}

std::string CandidateSSSubalgebra::ToStringPairingTableLaTeX(FormatExpressions* theFormat)const
{ if (!this->NilradicalPairingTable.size>0)
    return "";
  std::stringstream out;
  out << "\\documentclass{article}\\usepackage{longtable} \\begin{document}<br>";
  out << "Modules that have a zero weight (" << this->modulesWithZeroWeights.size << " total): ";
  for (int i=0; i<this->modulesWithZeroWeights.size; i++)
  { out << "$W_{" << this->modulesWithZeroWeights[i]+1 << "}$";
    if (i!=this->modulesWithZeroWeights.size-1)
      out << ", ";
  }
  out << "<br>Opposite modules <br>\n" << " \\begin{longtable}{c";
  for (int i=0; i<this->OppositeModulesByStructure.size; i++)
    out << "|c";
  out << "}\\\\ ";
  for (int i=0; i<this->OppositeModulesByStructure.size; i++)
    out << "&$W_{" << i+1 << "}$";
  out << "\\\\\\hline Opp. by structure:";
  for (int i=0; i< this->OppositeModulesByStructure.size; i++)
  { out << "&";
    for (int j=0; j<this->OppositeModulesByStructure[i].size; j++)
    { out << "$W_{" << this->OppositeModulesByStructure[i][j]+1 << "}$";
      if (j!=this->OppositeModulesByStructure[i].size-1)
      out << ", ";
    }
  }
  out << "\\\\\\hline Opposite character";
  for (int i=0; i< this->OppositeModulesByChar.size; i++)
  { out << "&";
    for (int j=0; j<this->OppositeModulesByChar[i].size; j++)
    { out << "$W_{" << this->OppositeModulesByChar[i][j]+1 << "} $";
      if (j!=this->OppositeModulesByChar[i].size-1)
        out << ", ";
    }
  }
  out << "\\end{longtable}";
  out << "<br>\\begin{longtable}{c";
  for (int i=0; i<this->Modules.size; i++)
    out << "|c";
  out << "} \\caption{Pairing table\\label{tablePairingTable}}\\\\<br>\n";
  FormatExpressions tempCharFormat;
  if (!this->charFormaT.IsZeroPointer())
    tempCharFormat= this->charFormaT.GetElementConst();
  out << "<br>\n";
  for (int i=0; i<this->NilradicalPairingTable.size; i++)
    out << "&$W_{" << i+1 << "}$";
  for (int i=0; i<this->NilradicalPairingTable.size; i++)
  { out << "\\\\\\hline<br\n>";
    out << "$W_{" << i+1 << "}$";
    for (int j=0; j<this->NilradicalPairingTable.size; j++)
    { out << "&";
      for (int k=0; k<this->NilradicalPairingTable[i][j].size; k++)
      { out << "$W_{" << this->NilradicalPairingTable[j][i][k]+1 << "}$";
        if (k!=this->NilradicalPairingTable[i][j].size-1)
          out << ", ";
      }
    }
  }
  out << "\\end{longtable}";
  out << "<br>\\end{document}";
  return out.str();
}

std::string CandidateSSSubalgebra::ToStringPairingTable(FormatExpressions* theFormat)const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ToStringPairingTable");
  if (!this->NilradicalPairingTable.size>0)
    return "";
  std::stringstream out;
  out << "<br>Below is the pairing table of the modules, defined as follows. Let g' be the subalgebra obtained by extending the current semisimple subalgebra "
  << " with the Cartan of the centralizer (following Penkov, Serganova, Zuckermann,  we call a subalgebra g' arising in this fashion ``primal''). "
  << " Let A, B, C be  isotypic components in the decomposition of the ambient Lie algebra over g'. We say that A and B pair to C if there exist "
  << " elements a in A and b in B such that  0\\neq [a,b]\\in C. Note that, in general, A and B may pair to multiple modules C', C'', etc. We note that "
  << " if A and B pair to C then clearly C is isomorphic to some component in the decomposition of A\\otimes B over g'. <br> We recall that W_{1}, W_{2}, ... are abbreviated notation "
  << " for the primal subalgebra modules indicated in the table above. ";
  out << "<br>Modules that have a zero weight (" << this->modulesWithZeroWeights.size << " total): ";
  for (int i=0; i<this->modulesWithZeroWeights.size; i++)
  { out << "V_{" << this->modulesWithZeroWeights[i]+1 << "}";
    if (i!=this->modulesWithZeroWeights.size-1)
      out << ", ";
  }
  out << "<br><table border=\"1\"><tr><td></td>";
  for (int i=0; i< this->OppositeModulesByStructure.size; i++)
    out << "<td>W_{" << i+1 << "}" << "</td>";
  out << "</tr>";
  out << "<tr> <td>Non-compatible (a.k.a. ``opposite'') modules:</td>";
  for (int i=0; i< this->OppositeModulesByStructure.size; i++)
  { out << "<td>";
    for (int j=0; j<this->OppositeModulesByStructure[i].size; j++)
    { out << "W_{" << this->OppositeModulesByStructure[i][j]+1 << "}";
      if (j!=this->OppositeModulesByStructure[i].size-1)
      out << ", ";
    }
    out << "</td>";
  }
  out << "</tr>";
  out << "<tr> <td>Opposite modules by character:</td>";
  for (int i=0; i< this->OppositeModulesByChar.size; i++)
  { out << "<td>";
    for (int j=0; j<this->OppositeModulesByChar[i].size; j++)
    { out << "W_{" << this->OppositeModulesByChar[i][j]+1 << "}";
      if (j!=this->OppositeModulesByChar[i].size-1)
        out << ", ";
    }
    out << "</td>";
  }
  out << "</tr>";

  out << "</table>";
  out << "<br>Modules corresponding to the semisimple subalgebra: ";
  Vector<Rational> theSAvector, tempV;
  theSAvector.MakeZero(this->NilradicalPairingTable.size);
  for (int i=0; i<this->subalgebraModules.size; i++)
  { tempV.MakeEi(this->NilradicalPairingTable.size, this->subalgebraModules[i]);
    theSAvector+=tempV;
  }
  out << theSAvector.ToStringLetterFormat("V");
  out << "<br><table><tr><td>Modules</td>";
  FormatExpressions tempCharFormat;
  if (!this->charFormaT.IsZeroPointer())
    tempCharFormat= this->charFormaT.GetElementConst();
  for (int i=0; i<this->NilradicalPairingTable.size; i++)
    out << "<td><b>" << "W_{" << i+1 << "}"
//    << "=" << this->thePrimalChar[i].ToString(&tempCharFormat)
    << "</b></td>";
  out << "</tr>";
  for (int i=0; i<this->NilradicalPairingTable.size; i++)
  { out << "<tr><td> <b>" << "W_{" << i+1 << "}</b></td>";
    for (int j=0; j<this->NilradicalPairingTable.size; j++)
    { out << "<td>";
      for (int k=0; k<this->NilradicalPairingTable[i][j].size; k++)
      { out << "W_{" << this->NilradicalPairingTable[j][i][k]+1 << "}";
        if (k!=this->NilradicalPairingTable[i][j].size-1)
          out << ", ";
      }
      out << "</td>";
    }
    out << "</tr>";
  }
  out << "</table>";
  return out.str();
}

void DynkinType::ScaleFirstCoRootSquaredLength(const Rational& multiplyCoRootSquaredLengthBy)
{ DynkinType result;
  result.MakeZero();
  result.SetExpectedSize(this->size());
  DynkinSimpleType tempType;
  for (int i=0; i<this->size(); i++)
  { tempType=(*this)[i];
    tempType.CartanSymmetricInverseScale*=multiplyCoRootSquaredLengthBy;
    result.AddMonomial(tempType, this->theCoeffs[i]);
  }
  *this=result;
}

std::string SemisimpleSubalgebras::ToStringAlgebraLink(int ActualIndexSubalgebra, FormatExpressions* theFormat)const
{ if (ActualIndexSubalgebra<0)
    return "(non-initialized)";
//  bool shortReportOnly=theFormat==0 ? true : theFormat->flagCandidateSubalgebraShortReportOnly;
  bool useMouseHover=theFormat==0 ? true : !theFormat->flagUseMathSpanPureVsMouseHover;
  std::stringstream out;
  bool makeLink= theFormat==0? false : theFormat->flagUseHtmlAndStoreToHD;
  if (this->theSubalgebraCandidates[ActualIndexSubalgebra].flagSystemProvedToHaveNoSolution)
    makeLink=false;
  std::string theTypeString=this->theSubalgebraCandidates[ActualIndexSubalgebra].theWeylNonEmbeddeD.theDynkinType.ToStringRelativeToAmbientType(this->GetSSowner().theWeyl.theDynkinType[0]);
  if (makeLink)
  { out << "<a href=\"" << this->GetDisplayFileNameSubalgebraRelative(ActualIndexSubalgebra, theFormat) << "\" style=\"text-decoration: none\">";
    if (useMouseHover)
      out << CGI::GetMathMouseHover(theTypeString);
    else
      out << CGI::GetMathSpanPure(theTypeString);
    out << "</a> ";
  } else
    out << theTypeString << "";
  return out.str();
}

std::string CandidateSSSubalgebra::ToStringCartanSA(FormatExpressions* theFormat)const
{ std::stringstream out;
  bool useLaTeX=theFormat==0? true : theFormat->flagUseLatex;
  bool useHtml=theFormat==0? true : theFormat->flagUseHTML;
  bool useMouseHover=theFormat==0 ? true : !theFormat->flagUseMathSpanPureVsMouseHover;

  List<DynkinSimpleType> theSimpleTypes;
  this->theWeylNonEmbeddeD.theDynkinType.GetTypesWithMults(theSimpleTypes);
  FormatExpressions tempFormat;
  tempFormat.AmbientWeylLetter=this->GetAmbientWeyl().theDynkinType[0].theLetter;
  tempFormat.AmbientCartanSymmetricInverseScale=this->GetAmbientWeyl().theDynkinType[0].CartanSymmetricInverseScale;
  out << "<br>Elements Cartan subalgebra by components: ";
  for (int i=0; i<this->CartanSAsByComponent.size; i++)
  { if (useLaTeX && useHtml)
    { if (useMouseHover)
        out << CGI::GetMathMouseHover(theSimpleTypes[i].ToString(&tempFormat), 1000) << ": ";
      else
        out << CGI::GetMathSpanPure(theSimpleTypes[i].ToString(&tempFormat), 1000) << ": ";
    } else
      out << theSimpleTypes[i].ToString(&tempFormat) << ":";
    for (int j=0; j<this->CartanSAsByComponent[i].size; j++)
    { out << this->CartanSAsByComponent[i][j].ToString() << ": "
      << this->GetAmbientWeyl().RootScalarCartanRoot(this->CartanSAsByComponent[i][j],this->CartanSAsByComponent[i][j]);
      if (j!=this->CartanSAsByComponent[i].size-1 || i!=this->CartanSAsByComponent.size-1)
        out << ", ";
    }
  }
  return out.str();
}

int CandidateSSSubalgebra::GetSSpartCentralizerOfSSPartCentralizer()const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::GetSSpartCentralizerOfSSPartCentralizer");
  if (!this->flagCentralizerIsWellChosen)
    return -1;
  if (this->indexSSPartCentralizer==-1)
    return this->owner->GetIndexFullSubalgebra();
  return this->owner->theSubalgebraCandidates[this->indexSSPartCentralizer].indexSSPartCentralizer;
}

std::string CandidateSSSubalgebra::ToStringCentralizer(FormatExpressions* theFormat)const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ToStringCentralizer");
  if (this->flagSystemProvedToHaveNoSolution)
    return "";
  std::stringstream out;
  bool useMouseHover=theFormat==0 ? true : !theFormat->flagUseMathSpanPureVsMouseHover;
  if (this->flagCentralizerIsWellChosen && this->centralizerRank!=0 )
  { out << "<br>Centralizer: ";
    Rational dimToralPartCentralizer=this->centralizerRank;
    if (this->indexSSPartCentralizer!=-1)
    { CandidateSSSubalgebra& centralizerSSpart=this->owner->theSubalgebraCandidates[this->indexSSPartCentralizer];
      out << this->owner->ToStringAlgebraLink(this->indexSSPartCentralizer, theFormat);
      dimToralPartCentralizer-=centralizerSSpart.GetRank();
      if (dimToralPartCentralizer!=0)
        out << " + ";
    } else if (!this->theCentralizerType.IsEqualToZero())
    { if (useMouseHover)
        out << CGI::GetMathMouseHover(this->theCentralizerType.ToStringRelativeToAmbientType(this->GetAmbientWeyl().theDynkinType[0]) );
      else
        out << CGI::GetMathSpanPure(this->theCentralizerType.ToStringRelativeToAmbientType(this->GetAmbientWeyl().theDynkinType[0]));
      out << " (can't determine subalgebra number - subalgebras computed partially?)";
      dimToralPartCentralizer-=this->theCentralizerType.GetRank();
    }
    if (dimToralPartCentralizer!=0)
    { std::stringstream toralPartStream;
      toralPartStream << "T_{" << dimToralPartCentralizer.ToString() << "}";
      if (useMouseHover)
        out << CGI::GetMathMouseHover(toralPartStream.str());
      else
        out << CGI::GetMathSpanPure(toralPartStream.str());
      out << " (toral part, subscript=dimension)";
    }
    out << ". ";
  }
  if (this->flagCentralizerIsWellChosen && this->centralizerRank==0)
    out << "<br>Centralizer: 0";
  if (this->flagCentralizerIsWellChosen && this->GetSSpartCentralizerOfSSPartCentralizer()!=-1)
  { out << "<br>The semisimple part of the centralizer of the semisimple part of my centralizer: "
    << this->owner->ToStringAlgebraLink(this->GetSSpartCentralizerOfSSPartCentralizer(), theFormat);
  }
  if (!this->flagCentralizerIsWellChosen)
  { //if (this->owner->flagAttemptToAdjustCentralizers)
    out << "<br><span style=\"color:#FF0000\"><b>The Cartan of the centralizer does not lie in the ambient Cartan (the computation was too large?).</b></span> "
    << "The intersection of the ambient Cartan with the centralizer is of dimension " << this->CartanOfCentralizer.size << " instead of the desired dimension " << this->centralizerRank.ToString() << ". ";
    //else
    //  out << "<br><span style=\"color:#FF0000\"><b>I was unable to compute the centralizer (not all subalgebras were computed?).</b></span> ";
  }
  if (this->centralizerRank!=0)
  { out << "<br>Basis of Cartan of centralizer: ";
    out << this->CartanOfCentralizer.ToString();
  }
  return out.str();
}

void CandidateSSSubalgebra::ComputeCentralizerIsWellChosen()
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputeCentralizerIsWellChosen");
  if (this->flagSystemProvedToHaveNoSolution)
  { this->flagCentralizerIsWellChosen=false;
    return;
  }
  MonomialChar<Rational> theZeroWeight;
  theZeroWeight.owner=0;
  theZeroWeight.weightFundamentalCoordS.MakeZero(this->theHs.size);
  this->centralizerRank =this->theCharNonPrimalFundCoords.GetMonomialCoefficient(theZeroWeight);
  if (this->centralizerRank==0)
  { this->flagCentralizerIsWellChosen=true;
    return;
  }
  ProgressReport theReport1(this->owner->theGlobalVariables);
  DynkinType centralizerType;
  if (this->indexMaxSSContainer!=-1)
  { centralizerType =this->owner->theSubalgebraCandidates[this->indexMaxSSContainer].theWeylNonEmbeddeD.theDynkinType;
    centralizerType-=this->theWeylNonEmbeddeD.theDynkinType;
    if (this->owner->theGlobalVariables!=0)
    { std::stringstream reportStream;
      reportStream << "<hr>The centralizer of subalgebra of type " << this->theWeylNonEmbeddeD.theDynkinType.ToString() << " is of type "
      << centralizerType.ToString();
      theReport1.Report(reportStream.str());
    }
      std::cout << "<br>centralizerType: " << centralizerType.ToString() ;
    this->centralizerRank-=centralizerType.GetRootSystemSize();
    if (this->RootSystemCentralizerPrimalCoords.size>0)
      if (centralizerType!=this->theCentralizerType)
        crash << "This is a programming error: two different methods for computing the centralizer type yield different results: by sub-diagram I computed the type as "
        << this->theCentralizerType.ToString() << " but looking at subalgerba containing the current one I got centralizer type "
        << centralizerType.ToString() << crash;
  }
    std::cout << "<br>centralizer rank: " << this->centralizerRank << ", cartan centralizer size: " << this->CartanOfCentralizer.size;
  this->flagCentralizerIsWellChosen=(this->centralizerRank==this->CartanOfCentralizer.size);
  if (this->indexMaxSSContainer!=-1 && this->flagCentralizerIsWellChosen)
    for (int i=0; i<this->owner->theSubalgebraCandidates.size; i++)
      if (centralizerType==this->owner->theSubalgebraCandidates[i].theWeylNonEmbeddeD.theDynkinType)
        if(this->owner->theSubalgebraCandidates[i].indicesDirectSummandSuperAlgebra.Contains(this->indexMaxSSContainer))
        { this->indexSSPartCentralizer=i;
          break;
        }
}

std::string CandidateSSSubalgebra::ToStringSystem(FormatExpressions* theFormat)const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ToStringSystem");
  std::stringstream out;
  if (!this->flagSystemSolved)
    out << "<br><b> Subalgebra not realized, but it might have a solution. </b> ";
  else
  { out << "<br>Subalgebra realized. ";
    if (!this->flagCentralizerIsWellChosen)
      out << "<b>However, the centralizer is not well chosen.</b>";
  }
  out << "<br>" << this->theUnknownNegGens.size << "*2 (unknown) gens:<br>(";
  for (int i=0; i<this->theUnknownNegGens.size; i++)
  { out << "<br>" << this->theUnknownNegGens[i].ToString(theFormat) << ", " ;
    out << this->theUnknownPosGens[i].ToString(theFormat);
    if (i!=this->theUnknownNegGens.size-1)
      out << ", ";
  }
  out << ")<br>";
  if (this->theUnknownCartanCentralizerBasis.size>0)
  { out << "<br>Unknown splitting cartan of centralizer.<br>\n";
    for (int i=0; i<this->theUnknownCartanCentralizerBasis.size; i++)
    { out << this->theUnknownCartanCentralizerBasis[i].ToString();
      if (i!=this->theUnknownCartanCentralizerBasis.size-1)
        out << ", ";
    }
    out << "<br>";
  }
  for (int i=0; i<this->theHs.size; i++)
  { out << "h: " << this->theHs[i] << ", " << " e = combination of " << this->theInvolvedPosGenerators[i].ToString()
    << ", f= combination of " << this->theInvolvedNegGenerators[i].ToString();
  }
  out << "Positive weight subsystem: " << this->theWeylNonEmbeddeD.RootsOfBorel.ToString();
  if (this->PosRootsPerpendicularPrecedingWeights.size>0)
    out << " Positive roots that commute with the weight subsystem: " << this->PosRootsPerpendicularPrecedingWeights.ToString();
  out << "<br>Symmetric Cartan default scale: " << this->theWeylNonEmbeddeDdefaultScale.CartanSymmetric.ToString(theFormat);
  out << "Character ambient Lie algebra: " << this->theCharFundamentalCoordsRelativeToCartan.ToString();
  out << "<br>A necessary system to realize the candidate subalgebra.  ";
  FormatExpressions tempFormat;
  for (int i=0; i<this->theSystemToSolve.size; i++)
    out << "<br>" << this->theSystemToSolve[i].ToString(&tempFormat) << "= 0";
  out << "<br>The above system after transformation.  ";
  for (int i=0; i<this->transformedSystem.size; i++)
    out << "<br>" << this->transformedSystem[i].ToString(&tempFormat) << "= 0";
  if (!this->flagSystemGroebnerBasisFound)
    out << "<br><b>Failed to find Groebner basis of the above system (the computation is too large).</b>";
  out << "<br><b>For the calculator part 1: </b>";
  out << "<br>FindOneSolutionSerreLikePolynomialSystem{}( ";
  for (int i=0; i<this->theSystemToSolve.size; i++)
  { out << this->theSystemToSolve[i].ToString(&tempFormat);
    if (i!=this->theSystemToSolve.size-1)
      out << ", ";
  }
  out << " )";
  out << "<br><b>For the calculator part 2: </b>";
  out << "<br>(";
  for (int i=0; i<this->theUnknownNegGens.size; i++)
  { out << this->theUnknownNegGens[i].ToString(theFormat) << ", " ;
    out << this->theUnknownPosGens[i].ToString(theFormat);
    if (i!=this->theUnknownNegGens.size-1)
      out << ", ";
  }
  out << ");";
  out << "<br>GroebnerLexUpperLimit{}(10000, ";
  for (int i=0; i<this->theSystemToSolve.size; i++)
  { out << this->theSystemToSolve[i].ToString(&tempFormat);
    if (i!=this->theSystemToSolve.size-1)
      out << ", ";
  }
  out << " )";
  return out.str();
}

std::string CandidateSSSubalgebra::ToStringTypeAndHs(FormatExpressions* theFormat)const
{ std::stringstream out;
  out << this->theWeylNonEmbeddeD.theDynkinType.ToString(theFormat) << ", Cartan: " << this->theHs.ToString(theFormat);
  return out.str();
}

std::string CandidateSSSubalgebra::ToStringGenerators(FormatExpressions* theFormat)const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ToStringGenerators");
  if (this->theBasis.size==0)
    return "";
  bool useLaTeX=theFormat==0 ? true : theFormat->flagUseLatex;
  bool useHtml=theFormat==0 ? true : theFormat->flagUseHTML;
  bool shortReportOnly=theFormat==0 ? true : theFormat->flagCandidateSubalgebraShortReportOnly;
  bool useMouseHover=theFormat==0 ? true : ((shortReportOnly && !theFormat->flagUseMathSpanPureVsMouseHover) || !theFormat->flagUseHtmlAndStoreToHD);
  std::stringstream out;
  out << "<br>Dimension of subalgebra generated by predefined or computed generators: " << this->theBasis.size << ". " << "<br>Negative simple generators: ";
  for (int i=0; i<this->theNegGens.size; i++)
  { if (useHtml && useLaTeX)
    { if (useMouseHover)
        out << CGI::GetMathMouseHover(this->theNegGens[i].ToString(theFormat), 2000);
      else
        out << CGI::GetMathSpanPure(this->theNegGens[i].ToString(theFormat), 2000);
    } else
      out << this->theNegGens[i].ToString(theFormat);
    if (i!=this->theNegGens.size-1)
      out << ", ";
  }
  out << "<br>Positive simple generators: ";
  for (int i=0; i<this->thePosGens.size; i++)
  { if (useHtml && useLaTeX)
    { if (useMouseHover)
        out << CGI::GetMathMouseHover(this->thePosGens[i].ToString(theFormat), 2000);
      else
        out << CGI::GetMathSpanPure(this->thePosGens[i].ToString(theFormat), 2000);
    } else
      out << this->thePosGens[i].ToString(theFormat);
    if (i!=this->thePosGens.size-1)
      out << ", ";
  }
  return out.str();
}

bool CandidateSSSubalgebra::AmRegularSA()const
{ for (int i=0; i<this->theNegGens.size; i++)
    if (this->theNegGens[i].size()>1 || this->thePosGens[i].size()>1)
      return false;
  return true;
}

std::string CandidateSSSubalgebra::ToString(FormatExpressions* theFormat)const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ToString");
  std::stringstream out;
  bool useLaTeX=theFormat==0 ? true : theFormat->flagUseLatex;
  bool useHtml=theFormat==0 ? true : theFormat->flagUseHTML;
  bool writingToHD=theFormat==0? false : theFormat->flagUseHtmlAndStoreToHD;
  bool shortReportOnly=theFormat==0 ? true : theFormat->flagCandidateSubalgebraShortReportOnly;
  bool useMouseHover=theFormat==0 ? true : !theFormat->flagUseMathSpanPureVsMouseHover;
  out << "Subalgebra type: " << this->owner->ToStringAlgebraLink(this->indexInOwner, theFormat) << " (click on type for detailed printout).";
  if (this->AmRegularSA())
    out << "<br>The subalgebra is regular (= the semisimple part of a root subalgebra). ";
  if (this->indexIamInducedFrom!=-1)
    out << "<br>Subalgebra is (parabolically) induced from " << this->owner->ToStringAlgebraLink(this->indexIamInducedFrom, theFormat) << ". ";
//  else
//    out << "<br>subalgebra outta nowhere!";
  if (this->flagSystemProvedToHaveNoSolution)
  { out << " <b> Subalgebra candidate proved to be impossible! </b> ";
    return out.str();
  }
  bool weightsAreCoordinated=true;
  if (this->Modules.size!=this->WeightsModulesNONprimal.size)
    weightsAreCoordinated=false;
  else
    for (int i=0; i<this->Modules.size; i++)
      if (this->Modules[i].size!=this->WeightsModulesNONprimal[i].size)
      { weightsAreCoordinated=false;
        break;
      }
  if (this->indicesDirectSummandSuperAlgebra.size>0)
  { out << "<br>Contained up to conjugation as a direct summand of: ";
    for (int i=0; i<this->indicesDirectSummandSuperAlgebra.size; i++)
    { out << this->owner->ToStringAlgebraLink(this->indicesDirectSummandSuperAlgebra[i], theFormat);
      if (i!=this->indicesDirectSummandSuperAlgebra.size-1)
        out << ", ";
    }
    out << ". ";
  }
  out << "<br>" << this->ToStringCartanSA(theFormat);
  out << this->ToStringCentralizer(theFormat);
  bool displayNilradSummary=(this->owner->flagComputeNilradicals && this->flagCentralizerIsWellChosen && this->flagSystemSolved);
  if (displayNilradSummary)
    displayNilradSummary=!shortReportOnly || (this->NumBadParabolics>0);
  if (displayNilradSummary)
  { out << "<hr>" << this->ToStringNilradicalsSummary(theFormat);
    out << "<a href=\"" << this->owner->GetDisplayFileNameFKFTNilradicals(this->indexInOwner, theFormat)
    << "\"> Detailed information on isotypical nilradicals. </a><hr>";
  }
  out << this->ToStringGenerators(theFormat);
  out << "<br>Scalar products basis of Cartan subalgebra (Cartan symmetric co-matrix): ";
  FormatExpressions tempFormat;
  tempFormat.flagUseLatex=true;
  tempFormat.flagUseHTML=false;
  FormatExpressions charFormatNonConst;
  if (!this->charFormaT.IsZeroPointer())
    charFormatNonConst= this->charFormaT.GetElementConst();
  if (useLaTeX && useHtml)
  { if (useMouseHover)
      out << CGI::GetMathMouseHover(this->theWeylNonEmbeddeD.CartanSymmetric.ToString(&tempFormat));
    else
      out << CGI::GetMathSpanPure(this->theWeylNonEmbeddeD.CartanSymmetric.ToString(&tempFormat));
  } else
    out << this->theWeylNonEmbeddeD.CartanSymmetric.ToString(theFormat);
  out << "<br>Decomposition of ambient Lie algebra: ";
  if (useLaTeX)
  { if (useMouseHover)
      out << CGI::GetMathMouseHover(this->theCharNonPrimalFundCoords.ToString(&charFormatNonConst), 20000);
    else
      out << CGI::GetMathSpanPure(this->theCharNonPrimalFundCoords.ToString(&charFormatNonConst), 20000);
  } else
    out << this->theCharNonPrimalFundCoords.ToString(&charFormatNonConst);
  if (this->CartanOfCentralizer.size>0)
  { out << "<br>Primal decomposition of the ambient Lie algebra (refining the above decomposition; the order from the above decomposition is not preserved): ";
    if (useLaTeX)
    { if (useMouseHover)
        out << CGI::GetMathMouseHover(this->thePrimalChaR.ToString(&charFormatNonConst), 20000);
      else
        out << CGI::GetMathSpanPure(this->thePrimalChaR.ToString(&charFormatNonConst), 20000);
    } else
      out << this->thePrimalChaR.ToString(&charFormatNonConst);
  }
  if (this->flagCentralizerIsWellChosen&& weightsAreCoordinated)
  { int numZeroWeights=0;
    out << "<br>The number of zero weights w.r.t. the Cartan subalgebra minus the dimension of the centralizer of the subalgebra equals " ;
    for (int i=0; i<this->Modules.size; i++)
      for (int j=0; j<this->WeightsModulesNONprimal[i].size; j++)
        if(this->WeightsModulesNONprimal[i][j].IsEqualToZero())
          numZeroWeights+=this->Modules[i].size;
    out << numZeroWeights << " - " << this->centralizerRank << "=" << ((this->centralizerRank-numZeroWeights)*(-1)).ToString() << ".";
  }
  if (this->flagSystemSolved && !shortReportOnly)
  { out << "<br>In the table below we indicate the highest weight "
    << "vectors of the decomposition of "
    << " the ambient Lie algebra as a module over the semisimple part. The second row indicates "
    << " weights of the highest weight vectors relative to the Cartan of the semisimple subalgebra. ";
    if (this->flagCentralizerIsWellChosen && this->CartanOfCentralizer.size>0)
    { out << "As the centralizer is well-chosen and the centralizer of our subalgebra is non-trivial, we may in addition split highest weight vectors"
      << " with the same weight over the semisimple part over the centralizer (recall that the centralizer preserves the weights over the subalgebra "
      << " and in particular acts on the highest weight vectors). Therefore we have chosen our highest weight vectors to be, in addition, weight vectors"
      << " over the Cartan of the centralizer of the  starting subalgebra. Their weight over the sum of the Cartans of the semisimple subalgebra and "
      << " its centralizer is indicated in the third row. The weights corresponding to the Cartan of the centralizer are again indicated with the letter"
      << " \\omega. As there is no preferred way of chosing a basis of the Cartan of the centralizer (unlike the starting semisimple Lie algebra:"
      << " there we have a preferred basis induced by the fundamental weights), our centralizer weights are simply given by the constant by which the "
      << " k^th basis element of the Cartan of the centralizer acts on the highest weight vector. Here, we use the choice for basis of the Cartan "
      << "of the centralizer given at the start of the page. ";
    }
    out << "<table border=\"1px solid black\"><tr><td>Highest vectors of representations (total " << this->HighestVectorsNonSorted.size << ") ";
    if (this->flagCentralizerIsWellChosen)
      out << "; the vectors are over the primal subalgebra.";
    out << "</td>";
    for (int i=0; i<this->HighestVectorsNonSorted.size; i++)
      out << "<td>" << this->HighestVectorsNonSorted[i].ToString() << "</td>";
    out << "</tr><tr><td>weight</td>";
    for (int i=0; i<this->HighestWeightsNONprimalNonSorted.size; i++)
      out << "<td>" << this->HighestWeightsNONprimalNonSorted[i].ToStringLetterFormat("\\omega", &charFormatNonConst) << "</td>";
    out << "</tr>";
    if (this->flagCentralizerIsWellChosen && this->CartanOfCentralizer.size>0)
    { out << "<tr><td>weights rel. to Cartan of (centralizer+semisimple s.a.). </td>";
      for (int i=0; i<this->HighestWeightsPrimalNonSorted.size; i++)
        out << "<td>" << this->HighestWeightsPrimalNonSorted[i].ToStringLetterFormat("\\omega", &charFormatNonConst) << "</td>";
      out << "</tr>";
    }
    out << "</table>";
  }
  if (!shortReportOnly)
  { out << this->ToStringModuleDecompo(theFormat);
    if (this->owner->flagProduceLaTeXtables)
      out << "LaTeX version of module decomposition: <br><br>" << this->ToStringModuleDecompoLaTeX(theFormat) << "<br><br>";
    out << this->ToStringPairingTable(theFormat);
    if (this->owner->flagProduceLaTeXtables)
      out << "LaTeX version of pairing table: <br><br>" << this->ToStringPairingTableLaTeX(theFormat) << "<br><br>";
    out << "<br>" << this->ToStringDrawWeights(theFormat) << "<br>";
  }
  bool shouldDisplaySystem=false;
  if (this->totalArithmeticOpsToSolveSystem!=0)
  { out << "<br>Made total " << totalArithmeticOpsToSolveSystem << " arithmetic operations while solving the Serre relations polynomial system. ";
    if (this->totalArithmeticOpsToSolveSystem>1000000 && !shortReportOnly)
    { shouldDisplaySystem=true;
      out << "<br>The total number of arithmetic operations I needed to solve the Serre relations polynomial system was larger than 1 000 000. I am printing out "
      << " the Serre relations system for you: maybe that can help improve the polynomial system algorithms. ";
    }
  }
  shouldDisplaySystem=shouldDisplaySystem || !this->flagSystemSolved || !this->flagCentralizerIsWellChosen;
  if (writingToHD)
    shouldDisplaySystem=shouldDisplaySystem && !shortReportOnly;
  if (shouldDisplaySystem)
    out << this->ToStringSystem(theFormat);
  return out.str();
}

void CandidateSSSubalgebra::GetHsByType(List<List<Vectors<Rational> > >& outputHsByType, List<DynkinSimpleType>& outputTypeList)const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::GetHsByType");
  List<DynkinSimpleType> allTypes;
  this->theWeylNonEmbeddeD.theDynkinType.GetTypesWithMults(allTypes);
  outputHsByType.SetSize(0);
  outputTypeList.SetSize(0);
  if (allTypes.size!=this->CartanSAsByComponent.size)
    crash << "This is a programming error: allTypes.size must equal this->CartanSAsByComponent.size. " << crash;
  for (int i=0; i<allTypes.size; i++)
  { bool shouldOpenNewType=true;
    if (i!=0)
      shouldOpenNewType=!(allTypes[i]==allTypes[i-1]);
    if (shouldOpenNewType)
    { outputHsByType.SetSize(outputHsByType.size+1);
      outputHsByType.LastObject()->SetSize(0);
      outputTypeList.AddOnTop(allTypes[i]);
    }
    outputHsByType.LastObject()->AddOnTop(this->CartanSAsByComponent[i]);
  }
}

template <class coefficient>
void WeylGroup::RaiseToMaximallyDominant(List<Vector<coefficient> >& theWeights, bool useOuterAutos)
{ bool found;
  MemorySaving<Vectors<coefficient> > theWeightsCopy;
  if (useOuterAutos)
    this->ComputeOuterAutos();
  for (int i=0; i<theWeights.size; i++)
    do
    { found=false;
      for (int j=0; j<this->RootsOfBorel.size; j++)
        if (this->RootScalarCartanRoot(this->RootsOfBorel[j], theWeights[i])<0)
        { bool isGood=true;
          for (int k=0; k<i; k++)
            if (this->RootScalarCartanRoot(this->RootsOfBorel[j], theWeights[k])>0)
            { isGood=false;
              break;
            }
          if (!isGood)
            continue;
          for (int k=0; k<theWeights.size; k++)
            this->ReflectBetaWRTAlpha(this->RootsOfBorel[j], theWeights[k], false, theWeights[k]);
          found=true;
        }
      if (useOuterAutos)
      { Vector<Rational> zeroWeight;
        zeroWeight.MakeZero(this->GetDim());
        HashedList<MatrixTensor<Rational> >& outerAutos=this->theOuterAutos.GetElement().theElements;
        for (int j=0; j<outerAutos.size; j++)
        { theWeightsCopy.GetElement()=theWeights;
          outerAutos[j].ActOnVectorsColumn(theWeightsCopy.GetElement());
          bool isGood=true;
          for (int k=0; k<i; k++)
            if (!(theWeightsCopy.GetElement()[k]-theWeights[k]).IsPositiveOrZero())
            { isGood=false;
              break;
            }
          if (!isGood)
            continue;
          if (!(theWeightsCopy.GetElement()[i]-theWeights[i]).IsGreaterThanLexicographic(zeroWeight))
            continue;
          found=true;
          theWeights=theWeightsCopy.GetElement();
        }
      }
    } while (found);
}

bool CandidateSSSubalgebra::HasConjugateHsTo(List<Vector<Rational> >& input)const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::HasConjugateHsTo");
//  bool doDebug=this->theWeylNonEmbeddeD.theDynkinType.ToString()=="A^{2}_1";
//  if (doDebug)
//    std::cout << "<br>Checking whether " << this->theHs.ToString() << " are conjugated to " << input.ToString();
  if (input.size!=this->theHs.size)
    return false;
  List<Vector<Rational> > raisedInput=input;
  List<Vector<Rational> > myVectors=this->theHs;
  //if (doDebug)
  //  std::cout << "<br>Comparing simultaneously: " << raisedInput.ToString() << " with " << myVectors.ToString();
  WeylGroup& ambientWeyl=this->GetAmbientWeyl();
  ambientWeyl.RaiseToMaximallyDominant(raisedInput, true);
  ambientWeyl.RaiseToMaximallyDominant(myVectors, true);
  //if (doDebug)
  //  std::cout << "<br>raised input is: " << raisedInput.ToString() << ", my raised h's are: " << myVectors.ToString();
  return myVectors==raisedInput;
}

bool CandidateSSSubalgebra::IsDirectSummandOf(const CandidateSSSubalgebra& other, bool computeImmediateDirectSummandOnly)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::IsDirectSummandOf");
  if (other.flagSystemProvedToHaveNoSolution)
    return false;
//  bool doDebug=(this->theWeylNonEmbeddeD.theDynkinType.ToString()== "A^{2}_1" && other.theWeylNonEmbeddeD.theDynkinType.ToString()=="A^{2}_1+A^{1}_1");
//  if (doDebug)
//    std::cout << " <hr><hr>Testing whether " << this->ToString() << " is a direct summand of " << other.ToString() << "...<br>";
  DynkinType theDifference;
  theDifference= other.theWeylNonEmbeddeD.theDynkinType;
  theDifference-=this->theWeylNonEmbeddeD.theDynkinType;
  for (int i=0; i<theDifference.size(); i++)
    if (theDifference.theCoeffs[i]<0)
    { //std::cout << " it's not because types don't match.";
      return false;
    }
  Incrementable<SelectionFixedRank> selectedTypes;
  List<DynkinSimpleType> isoTypes;
  SelectionFixedRank currentTypeSelection;
  SelectionWithMaxMultiplicity outerIsoSelector;
  List<List<Vectors<Rational> > > theHsByType;
  other.GetHsByType(theHsByType, isoTypes);
  for (int i=0; i<isoTypes.size; i++)
  { Rational ratMult=this->theWeylNonEmbeddeD.theDynkinType.GetMonomialCoefficient(isoTypes[i]);
    int intMult;
    if (!ratMult.IsSmallInteger(&intMult))
      return false;
    currentTypeSelection.SetNumItemsAndDesiredSubsetSize(intMult, theHsByType[i].size);
    selectedTypes.theElements.AddOnTop(currentTypeSelection);
  }
  FinitelyGeneratedMatrixMonoid<Rational> theOuterAutos;
  this->theWeylNonEmbeddeD.theDynkinType.GetOuterAutosGeneratorsActOnVectorColumn(theOuterAutos.theGenerators);
  for (int i=0; i<theOuterAutos.theGenerators.size; i++)
    theOuterAutos.theGenerators[i].Transpose();
  bool mustBeTrue=theOuterAutos.GenerateElements(100000);
  if (!mustBeTrue)
    crash << "Failed to generate outer automorphisms of Dynkin simple type. The upper limit for such automorphism group size is 100000" << crash;
  Rational numCyclesFromTypes=selectedTypes.GetNumTotalCombinations();
  if (!numCyclesFromTypes.IsSmallInteger())
    crash << "Computation is too large: I am crashing to let you know that the program cannot handle such a large number of outer automorphisms" << crash;
  List<Vector<Rational> > conjugationCandidates;
  Vectors<Rational> currentComponent;
  ProgressReport theReport(this->owner->theGlobalVariables);
  if (this->owner->theGlobalVariables!=0)
  { std::stringstream reportStream;
    reportStream << "Computing whether " << this->theWeylNonEmbeddeD.theDynkinType << " is direct summand of "
    << other.theWeylNonEmbeddeD.theDynkinType.ToString() << ". The outer automorphisms of the smaller type are: ";
    FormatExpressions theFormat;
    theFormat.flagUseHTML=false;
    theFormat.flagUseLatex=true;
    for (int i=0; i<theOuterAutos.theElements.size; i++)
      if (i>=100)
        reportStream << "... and so on, only the first 100 elements printed out of total " << theOuterAutos.theElements.size << ". ";
      else
        reportStream << "<br>" << CGI::GetMathMouseHover(theOuterAutos.theElements[i].ToStringMatForm(&theFormat));
    theReport.Report(reportStream.str());
  }
  do
    for (int k=0; k<theOuterAutos.theElements.size; k++)
    { conjugationCandidates.SetSize(0);
      for (int i=0; i<selectedTypes.theElements.size; i++)
      { Selection& currentSel=selectedTypes.theElements[i].theSelection;
        for (int j=0; j<currentSel.CardinalitySelection; j++)
        { currentComponent= theHsByType[i][currentSel.elements[j]];
          conjugationCandidates.AddListOnTop(currentComponent);
        }
      }
      theOuterAutos.theElements[k].ActOnVectorROWSOnTheLeft(conjugationCandidates, conjugationCandidates);
      if (this->HasConjugateHsTo(conjugationCandidates))
        return true;
    }
  while (selectedTypes.IncrementReturnFalseIfBackToBeginning());
  return false;
}

void CandidateSSSubalgebra::AdjustCentralizerAndRecompute(bool allowNonPolynomialSystemFailure)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::AdjustCentralizerAndRecompute");
  if (this->flagSystemProvedToHaveNoSolution)
    return;
  this->ComputeCentralizerIsWellChosen();
  if (this->owner->flagAttemptToAdjustCentralizers)
    if (!this->flagCentralizerIsWellChosen)
    { //std::cout << "<hr>Adjusting " << this->ToStringTypeAndHs();
      //std::cout << "<br>Starting generators: " << this->ToStringGenerators();
      this->ComputeSystem(true, allowNonPolynomialSystemFailure);
      //std::cout << "<br>... and final generators: " << this->ToStringGenerators();
      this->ComputeCentralizerIsWellChosen();
    }
}

int CandidateSSSubalgebra::GetNumModules()const
{ int result=0;
  for (int i=0; i<this->Modules.size; i++)
    result+=this->Modules[i].size;
  return result;
}

void SemisimpleSubalgebras::ComputePairingTablesAndFKFTtypes()
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::ComputePairingTablesAndFKFTtypes");
  ProgressReport theReport(this->theGlobalVariables);
  for (int i=0; i<this->theSubalgebraCandidates.size; i++)
  { CandidateSSSubalgebra& currentSA=this->theSubalgebraCandidates[i];
    if (!currentSA.flagCentralizerIsWellChosen || !currentSA.flagSystemSolved)
      continue;
    if (!this->flagComputePairingTable)
      continue;
    if (this->theGlobalVariables!=0)
    { std::stringstream reportStream2;
      reportStream2 << "Computing pairing table of subalgebra number " << i+1 << " out of " << this->theSubalgebraCandidates.size
      << ". The subalgebra is of type " << this->theSubalgebraCandidates[i].ToStringTypeAndHs() << "... ";
      theReport.Report(reportStream2.str());
    }
    if (currentSA.AmRegularSA())
      continue;
    currentSA.ComputePairingTable(this->theGlobalVariables);
    if (this->theGlobalVariables!=0)
    { std::stringstream reportStream2;
      reportStream2 << "Computing pairing table of subalgebra number " << i+1 << " out of " << this->theSubalgebraCandidates.size
      << ". The subalgebra is of type " << this->theSubalgebraCandidates[i].ToStringTypeAndHs() << "... DONE. Computing Fernando-Kac subalgebra candidates.";
      theReport.Report(reportStream2.str());
    }
    if (this->flagComputeNilradicals && !this->theSubalgebraCandidates[i].AmRegularSA())
      currentSA.EnumerateAllNilradicals(this->theGlobalVariables);
  }
}

void SemisimpleSubalgebras::HookUpCentralizers(bool allowNonPolynomialSystemFailure)
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::HookUpCentralizers");
  List<int> theCandidatePermutation;
  theCandidatePermutation.SetSize(this->theSubalgebraCandidates.size);
  for (int i=0; i<theCandidatePermutation.size; i++)
    theCandidatePermutation[i]=i;
  this->theSubalgebraCandidates.QuickSortAscending(0, &theCandidatePermutation);
  HashedList<int, MathRoutines::IntUnsignIdentity> theCandidatePermutationHashed;
  theCandidatePermutationHashed=theCandidatePermutation;
  ProgressReport theReport1(this->theGlobalVariables), theReport2(this->theGlobalVariables);
  std::stringstream reportStream;
  theReport1.Report("<hr>\nHooking up centralizers ");
  for (int i=0; i<this->theSubalgebraCandidates.size; i++)
  { if (!this->theSubalgebraCandidates[i].flagSystemSolved)
      continue;
    CandidateSSSubalgebra& currentSA=this->theSubalgebraCandidates[i];
    std::stringstream reportStream2;
    reportStream2 << "Computing centralizer of subalgebra number " << i+1 << " out of " << this->theSubalgebraCandidates.size
    << ". The subalgebra is of type " << currentSA.ToStringTypeAndHs() << ". ";
    theReport2.Report(reportStream2.str());
    currentSA.indexInOwner=i;
    currentSA.indexIamInducedFrom=theCandidatePermutationHashed.GetIndex(currentSA.indexIamInducedFrom);
    currentSA.indicesDirectSummandSuperAlgebra.SetSize(0);
    currentSA.indexMaxSSContainer=-1;
//    std::cout << "<hr>Exploring centralizers of " << currentSA.theWeylNonEmbeddeD.theDynkinType.ToString();
    for (int j=0; j<this->theSubalgebraCandidates.size; j++)
    { if (i==j)
        continue;
      CandidateSSSubalgebra& otherSA=this->theSubalgebraCandidates[j];
      if (currentSA.IsDirectSummandOf(otherSA, true))
      { currentSA.indicesDirectSummandSuperAlgebra.AddOnTop(j);
        if (currentSA.indexMaxSSContainer==-1)
          currentSA.indexMaxSSContainer=j;
        if (this->theSubalgebraCandidates[currentSA.indexMaxSSContainer].theWeylNonEmbeddeD.theDynkinType.GetRootSystemPlusRank()
            <otherSA.theWeylNonEmbeddeD.theDynkinType.GetRootSystemPlusRank())
          currentSA.indexMaxSSContainer=j;
//        std::cout << " direct summand of " << otherSA.theWeylNonEmbeddeD.theDynkinType.ToString() << ", ";
      }// else std::cout << " NOT direct summand of " << otherSA.theWeylNonEmbeddeD.theDynkinType.ToString() << ", ";
    }
  }
  theReport1.Report("<hr>\nCentralizers computed, adjusting centralizers with respect to the Cartan subalgebra.");
  for (int i=0; i<this->theSubalgebraCandidates.size; i++)
  { if (!this->theSubalgebraCandidates[i].flagSystemSolved)
      continue;
    std::stringstream reportStream2;
    reportStream2 << "Adjusting the centralizer of subalgebra number " << i+1 << " out of " << this->theSubalgebraCandidates.size
    << ". The subalgebra is of type " << this->theSubalgebraCandidates[i].ToStringTypeAndHs() << ". ";
    theReport2.Report(reportStream2.str());
    this->theSubalgebraCandidates[i].AdjustCentralizerAndRecompute(allowNonPolynomialSystemFailure);
  }
  theReport1.Report("<hr>\nComputing pairing tables.");
  if (this->flagComputeModuleDecomposition)
    for (int i=0; i<this->theSubalgebraCandidates.size; i++)
      if (this->theSubalgebraCandidates[i].flagCentralizerIsWellChosen && this->theSubalgebraCandidates[i].flagSystemSolved)
      { std::stringstream reportStream2;
        reportStream2 << "Computing primal module decomposition of subalgebra number " << i+1 << " out of " << this->theSubalgebraCandidates.size
        << ". The subalgebra is of type " << this->theSubalgebraCandidates[i].ToStringTypeAndHs() << ". ";
        theReport2.Report(reportStream2.str());
        this->theSubalgebraCandidates[i].ComputePrimalModuleDecomposition(this->theGlobalVariables);
      }
}

bool DynkinType::operator>(const DynkinType& other)const
{ if (this->GetRank()>other.GetRank())
    return true;
  if (other.GetRank()>this->GetRank())
    return false;
  DynkinType difference;
  difference=*this;
  difference-=other;
  if (difference.IsEqualToZero())
    return false;
  DynkinSimpleType highestSimpleTypeDifference=difference[0];
  Rational maxComponentDifferenceMult=difference.theCoeffs[0];
  for (int i=1; i<difference.size(); i++)
    if (difference[i]>highestSimpleTypeDifference)
    { maxComponentDifferenceMult=difference.theCoeffs[i];
      highestSimpleTypeDifference=difference[i];
    }
  return maxComponentDifferenceMult>0;
}

bool CandidateSSSubalgebra::operator>(const CandidateSSSubalgebra& other)const
{ //if (this->owner!=other.owner)
  //{ crash << "This is a programming error: comparing CandidateSSSubalgebra with different owners. "
  //  << crash;
  //}
  return this->theWeylNonEmbeddeD.theDynkinType>other.theWeylNonEmbeddeD.theDynkinType;
}

void CandidateSSSubalgebra::ComputeCartanOfCentralizer(GlobalVariables* theGlobalVariables)
{ Vectors<AlgebraicNumber> theHWsNonSorted;
  Vectors<AlgebraicNumber> theCartan;
  theHWsNonSorted.SetSize(this->HighestVectorsNonSorted.size);
  for (int i=0; i<this->HighestVectorsNonSorted.size; i++)
  { ElementSemisimpleLieAlgebra<AlgebraicNumber>& currentElt=this->HighestVectorsNonSorted[i];
    currentElt.ElementToVectorNegativeRootSpacesFirst(theHWsNonSorted[i]);
  }
  ElementSemisimpleLieAlgebra<AlgebraicNumber> tempElt;
  Vector<Rational> tempV;
  theCartan.SetSize(this->GetAmbientSS().GetRank());
  for (int i=0; i<this->GetAmbientSS().GetRank(); i++)
  { tempV.MakeEi(this->GetAmbientSS().GetRank(), i);
    tempElt.MakeHgenerator(tempV, this->GetAmbientSS());
    tempElt.ElementToVectorNegativeRootSpacesFirst(theCartan[i]);
  }
  Vectors<AlgebraicNumber> outputCartanCentralizer;
  Vector<AlgebraicNumber> theCentralizerH;
  theHWsNonSorted.IntersectTwoLinSpaces(theHWsNonSorted, theCartan, outputCartanCentralizer, theGlobalVariables);
  this->CartanOfCentralizer.SetSize(outputCartanCentralizer.size);
  AlgebraicNumber theFirstNonZeroCoeff;
  for (int i=0; i<this->CartanOfCentralizer.size; i++)
  { tempElt.AssignVectorNegRootSpacesCartanPosRootSpaces(outputCartanCentralizer[i], *this->owner->owneR);
    //std::cout << "<hr>Temp elt: " << tempElt.ToString();
    theCentralizerH=tempElt.GetCartanPart();
    theFirstNonZeroCoeff=theCentralizerH[theCentralizerH.GetIndexFirstNonZeroCoordinate()];
    if (!theFirstNonZeroCoeff.IsRational())
      theCentralizerH/=theFirstNonZeroCoeff;
    this->CartanOfCentralizer[i]=theCentralizerH;
    //std::cout << " ... in vector format: " << this->CartanOfCentralizer[i].ToString();
    this->CartanOfCentralizer[i].ScaleToIntegralMinHeightFirstNonZeroCoordinatePositive();
    //std::cout << " ... scaled: " << this->CartanOfCentralizer[i].ToString();
  }
//  if (this->CartanOfCentralizer.size>0)
//    this->CartanOfCentralizer[0]*=4;
/*  Matrix<Rational> basisChangeMatrix;
  Vectors<Rational> aBasisProjection, aBasisPreimages;
  Vector<Rational> projection, preimage;
  for (int i=0; i<this->owner->owneR->GetRank(); i++)
  { preimage.MakeEi(this->owner->owneR->GetRank(), i);
    this->GetPrimalWeightProjectionFundCoords(preimage, projection);
    aBasisProjection.AddOnTop(projection);
    if (aBasisProjection.size==aBasisProjection.GetRankOfSpanOfElements())
      aBasisPreimages.AddOnTop(preimage);
    else
      aBasisProjection.RemoveLastObject();
  }
  aBasisPreimages.GetGramMatrix(this->BilinearFormFundPrimal, &this->owner->owneR->theWeyl.CartanSymmetric);
  std::cout << "<hr>aBasisPreimages: " << aBasisPreimages.ToString();
  basisChangeMatrix.AssignVectorsToColumns(aBasisProjection);
  basisChangeMatrix.Invert();
  std::cout << "<br>basisChangeMatrix: " << basisChangeMatrix.ToString();
  std::cout << "<br>BilinearFormFundPrimal: " << this->BilinearFormFundPrimal.ToString();
  this->BilinearFormFundPrimal*=basisChangeMatrix;
  basisChangeMatrix.Transpose();
  this->BilinearFormFundPrimal.MultiplyOnTheLeft(basisChangeMatrix);
  std::cout << "<br>final bilinear form fund primal: " << this->BilinearFormFundPrimal.ToString();
*/
  ////////////////
  this->BilinearFormSimplePrimal=this->theWeylNonEmbeddeD.CartanSymmetric;
  Matrix<Rational> centralizerPart, matFundCoordsSimple, diagMat, diagMatrix2, bilinearFormInverted;
//  std::cout << "<hr>Cartan of Centralizer: " << this->CartanOfCentralizer.ToString() << "<br>Cartan symmetric: "
//  << this->owner->owneR->theWeyl.CartanSymmetric.ToString();
  this->CartanOfCentralizer.GetGramMatrix(centralizerPart, &this->owner->owneR->theWeyl.CartanSymmetric);
  this->BilinearFormSimplePrimal.DirectSumWith(centralizerPart);
  bilinearFormInverted=this->BilinearFormSimplePrimal;
  bilinearFormInverted.Invert();
  diagMat.init(this->BilinearFormSimplePrimal.NumRows, this->BilinearFormSimplePrimal.NumCols);
  diagMatrix2.init(this->BilinearFormSimplePrimal.NumRows, this->BilinearFormSimplePrimal.NumCols);
  diagMat.NullifyAll();
  diagMatrix2.NullifyAll();
  for (int i=0; i<this->BilinearFormSimplePrimal.NumRows; i++)
    if (i<this->theHs.size)
    { diagMat(i,i)=this->theWeylNonEmbeddeDdefaultScale.CartanSymmetric(i,i)/2;
      diagMatrix2(i,i)=this->theWeylNonEmbeddeD.CartanSymmetric(i,i)/2;
    } else
    { diagMat(i,i).AssignNumeratorAndDenominator(1,2);
      diagMatrix2(i,i)=1;
    }
  matFundCoordsSimple=bilinearFormInverted;
  matFundCoordsSimple*=diagMat;
  this->MatMultiplyFundCoordsToGetSimple=bilinearFormInverted;
  this->MatMultiplyFundCoordsToGetSimple*=diagMatrix2;
//  std::cout
//  << "<br>diagMat=" << diagMat.ToString()
//  << "<br>this->BilinearFormSimplePrimal= " << this->BilinearFormSimplePrimal.ToString()
//  << "<br>fundCoordsViaSimple: " << fundCoordsViaSimple.ToString();
  this->BilinearFormFundPrimal=matFundCoordsSimple;
  this->BilinearFormFundPrimal.Transpose();
//  std::cout
//  << "<br>fundCoordsViaSimple.Transpose(): " << this->BilinearFormFundPrimal.ToString();
  this->BilinearFormFundPrimal*=this->BilinearFormSimplePrimal;
//  std::cout
//  << "<br>fundCoordsViaSimple.Transpose()*this->BilinearFormSimplePrimal: " << this->BilinearFormFundPrimal.ToString();
  this->BilinearFormFundPrimal*=matFundCoordsSimple;
/*  this->InducedEmbeddingPrimalFundCoordsIntoSimpleAmbientCoords.init(this->GetAmbientWeyl().GetDim(), this->GetPrimalRank());
  for (int i=0; i<this->GetRank(); i++)
    this->InducedEmbeddingPrimalFundCoordsIntoSimpleAmbientCoords.AssignVectorToColumnKeepOtherColsIntactNoInit
    (i, this->theHs[i]);
  for (int i=this->GetRank(); i<this->GetPrimalRank(); i++)
    this->InducedEmbeddingPrimalFundCoordsIntoSimpleAmbientCoords.AssignVectorToColumnKeepOtherColsIntactNoInit
    (i, this->CartanOfCentralizer[i-this->GetRank()]);
*/
//  std::cout
//  << "<br>fundCoordsViaSimple.Transpose()*this->BilinearFormSimplePrimal*fundCoordsViaSimple: " << this->BilinearFormFundPrimal.ToString()
//  << "<hr>";
}