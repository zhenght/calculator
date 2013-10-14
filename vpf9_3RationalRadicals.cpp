//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#include "vpfImplementationHeader2Math051_PolynomialComputations_Basic.h"
#include "vpfHeader2Math2_AlgebraicNumbers.h"
#include "vpfHeader2Math5_SubsetsSelections.h"
ProjectInformationInstance ProjectInfoVpf9_3cpp(__FILE__, "Algebraic numbers. ");

std::string MonomialVector::ToString(FormatExpressions* theFormat)const
{ if (theFormat!=0)
    if (this->theIndex< theFormat->vectorSpaceEiBasisNames.size && this->theIndex>=0)
      return theFormat->vectorSpaceEiBasisNames[this->theIndex];
  std::stringstream out;
  out << "e_{" << this->theIndex+1 << "}";
  return out.str();
}

bool AlgebraicClosureRationals::CheckConsistency()const
{ return true;
}

void AlgebraicClosureRationals::GetMultiplicativeOperatorFromRadicalSelection(const Selection& theSel, MatrixTensor<Rational>& outputOp)
{ MacroRegisterFunctionWithName("AlgebraicClosureRationals::GetMultiplicativeOperatorFromRadicalSelection");
  outputOp.MakeZero();
  Selection vectorActedOnSel, resultVectorSel;
  vectorActedOnSel.init(this->theQuadraticRadicals.size);
  resultVectorSel.init(this->theQuadraticRadicals.size);
  Rational theCoeff;
  MonomialMatrix tempM;
  do
  { theCoeff=1;
    for (int i=0; i<this->theQuadraticRadicals.size; i++)
    { if (vectorActedOnSel.selected[i] && theSel.selected[i])
      { resultVectorSel.selected[i]=false;
        theCoeff*=this->theQuadraticRadicals[i];
      } else if (!vectorActedOnSel.selected[i] && !theSel.selected[i])
        resultVectorSel.selected[i]=false;
      else
        resultVectorSel.selected[i]=true;
    }
    resultVectorSel.ComputeIndicesFromSelection();
    tempM.MakeEij(this->GetIndexFromRadicalSelection(resultVectorSel), this->GetIndexFromRadicalSelection(vectorActedOnSel));
    outputOp.AddMonomial(tempM, theCoeff);
  } while (vectorActedOnSel.IncrementReturnFalseIfBackToBeginning());
}

void AlgebraicClosureRationals::ComputeDisplayStringsFromRadicals()
{ MacroRegisterFunctionWithName("AlgebraicClosureRationals::ComputeDisplayStringsFromRadicals");
  if (!this->flagIsQuadraticRadicalExtensionRationals)
    return;
  this->DisplayNamesBasisElements.SetSize(this->theBasisMultiplicative.size);
  Selection theSel;
  theSel.init(this->theQuadraticRadicals.size);
  do
  { std::stringstream out;
    for (int i=0; i<theSel.CardinalitySelection; i++)
    { const LargeInt& theRad=this->theQuadraticRadicals[theSel.elements[i]];
      out << "\\sqrt{" << theRad.ToString() << "}";
    }
    this->DisplayNamesBasisElements[this->GetIndexFromRadicalSelection(theSel)]=out.str();
  } while (theSel.IncrementReturnFalseIfBackToBeginning());
}

int AlgebraicClosureRationals::GetIndexFromRadicalSelection(const Selection& theSel)
{ if (theSel.MaxSize>30)
  { crash << "This is a programming error: the algebraic extension is too large to be handled by the current data structures. "
    << crash;
  }
  int result=0;
  for (int i=theSel.MaxSize-1; i>=0; i--)
  { result*=2;
    if (theSel.selected[i])
      result+=1;
  }
  return result;
}

bool AlgebraicClosureRationals::MergeRadicals(const List<LargeInt>& theRadicals)
{ MacroRegisterFunctionWithName("AlgebraicClosureRationals::MergeTwoQuadraticRadicalExtensions");
  if (!this->flagIsQuadraticRadicalExtensionRationals )
  { crash << "This is a programming error: AlgebraicClosureRationals::MergeTwoQuadraticRadicalExtensions "
    << "with at least one of two arguments that is not a quadratic radical extension of the rationals. "
    << crash;
  }
  //refine factors:
  HashedList<LargeInt> radicalsNew=this->theQuadraticRadicals;
  radicalsNew.AddOnTopNoRepetition(theRadicals);
  bool found=true;
  LargeIntUnsigned candidateGCD, leftQuotient, rightQuotient;
  while (found)
  { found=false;
    for (int i=0; i<radicalsNew.size; i++)
      for (int j=i+1; j<radicalsNew.size; j++)
      { if (radicalsNew[i]==-1 || radicalsNew[j]==-1)
          continue;
        LargeIntUnsigned::gcd(radicalsNew[i].value, radicalsNew[j].value, candidateGCD);
        if (candidateGCD>1)
        { leftQuotient=radicalsNew[i].value/candidateGCD;
          rightQuotient=radicalsNew[j].value/candidateGCD;
          radicalsNew.RemoveIndexSwapWithLast(j);
          radicalsNew.RemoveIndexSwapWithLast(i);
          if (leftQuotient>1)
            radicalsNew.AddOnTopNoRepetition((LargeInt) leftQuotient);
          if (rightQuotient>1)
            radicalsNew.AddOnTopNoRepetition((LargeInt) rightQuotient);
          radicalsNew.AddOnTopNoRepetition((LargeInt) candidateGCD);
          found=true;
        }
      }
  }
  radicalsNew.QuickSortAscending();
  if (radicalsNew.size>16)
  { std::cout << "Computing with fields whose dimension over the rationals is greater than 2^16 is not allowed. "
    << "Such computations are too large for the current implementation of algberaic extensions of the rationals. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    return (false);
  }
  this->theBasisMultiplicative.SetSize(MathRoutines::TwoToTheNth(radicalsNew.size));
  MatrixTensor<Rational> currentInjection;
  currentInjection.MakeZero();
  Selection largerFieldSel, smallerFieldSel;
  largerFieldSel.init(radicalsNew.size);
  smallerFieldSel.init(this->theQuadraticRadicals.size);
  do
  { largerFieldSel.initNoMemoryAllocation();
    for (int j=0; j<this->theQuadraticRadicals.size; j++)
      if (smallerFieldSel.selected[j])
      { if (this->theQuadraticRadicals[j]==-1)
        { largerFieldSel.AddSelectionAppendNewIndex(radicalsNew.GetIndex(-1));
          continue;
        }
        for (int k=0; k<radicalsNew.size; k++)
          if (this->theQuadraticRadicals[j]%radicalsNew[k]==0 && radicalsNew[k]!=-1)
            largerFieldSel.AddSelectionAppendNewIndex(k);
      }
//        std::cout << "<hr>smaller field sel: " << smallerFieldSel.ToString() << " larger field sel: " << largerFieldSel.ToString();
    currentInjection.AddMonomial(MonomialMatrix(this->GetIndexFromRadicalSelection(largerFieldSel), this->GetIndexFromRadicalSelection(smallerFieldSel)), 1);
  }while (smallerFieldSel.IncrementReturnFalseIfBackToBeginning());
  this->theQuadraticRadicals=radicalsNew;
  largerFieldSel.init(radicalsNew.size);
  do
    this->GetMultiplicativeOperatorFromRadicalSelection(largerFieldSel, this->theBasisMultiplicative[this->GetIndexFromRadicalSelection(largerFieldSel)]);
  while(largerFieldSel.IncrementReturnFalseIfBackToBeginning());
//  std::cout << "<hr>Computing display strings";
  this->RegisterNewBasis(currentInjection);
  this->ComputeDisplayStringsFromRadicals();
//  std::cout << this->ToString();
  return true;
}

void AlgebraicClosureRationals::RegisterNewBasis
  (const MatrixTensor<Rational>& theInjection)
{ //VectorSparse<Rational> eltVectorForm;
  for (int j=0; j<this->theBasesAdditive.size; j++)
    for (int i=0; i<this->theBasesAdditive[j].size; i++)
      theInjection.ActOnVectorColumn(this->theBasesAdditive[j][i]);
  this->theBasesAdditive.SetSize(this->theBasesAdditive.size+1);
  this->theBasesAdditive.LastObject()->SetSize(this->theBasisMultiplicative.size);
  for (int i=0; i<this->theBasisMultiplicative.size; i++)
    (*this->theBasesAdditive.LastObject())[i].MaKeEi(i);
}

void AlgebraicClosureRationals::ChooseGeneratingElement()
{ MacroRegisterFunctionWithName("AlgebraicExtensionRationals::ChooseGeneratingElement");
  SelectionPositiveIntegers theSel;
  int DimOverRationals=this->theBasisMultiplicative.size;
  theSel.init(DimOverRationals);
  this->theGeneratingElementPowersBasis.SetSize(0);
  Vector<Rational> currentVect;
//  std::cout << "Dim over rationals: " << this->DimOverRationals;
//  int counter =0;
  this->GeneratingElemenT.owner=this;
  this->GeneratingElemenT.basisIndex=this->theBasesAdditive.size-1;
  for (theSel.IncrementReturnFalseIfBackToBeginning(); ; theSel.IncrementReturnFalseIfBackToBeginning())
  { this->GeneratingElemenT.theElT.MakeZero();
    for (int i=0; i<theSel.theInts.size; i++)
    { MonomialVector tempV;
      tempV.MakeEi(i);
      this->GeneratingElemenT.theElT.AddMonomial(tempV, theSel.theInts[i]);
    }
//    std::cout << "<br>selection: " << theSel.ToString() << ", generator: " << this->GeneratingElemenT.theElt.ToString();
    this->GetMultiplicationBy(this->GeneratingElemenT, this->GeneratingElementTensorForm);
//    std::cout << ", current generator= " << this->GeneratingElementTensorForm.ToStringMatForm();
    this->GeneratingElementTensorForm.GetMatrix(this->GeneratingElementMatForm, DimOverRationals);
//    std::cout << ", in mat form= " << this->GeneratingElementMatForm.ToString();
    this->theGeneratingElementPowersBasis.SetSize(0);
    currentVect.MakeEi(DimOverRationals, 0);
    this->theGeneratingElementPowersBasis.AddOnTop(currentVect);
    do
    { //counter ++;
      //if (counter>1000)
      //  assert(false);
      this->GeneratingElementMatForm.ActOnVectorColumn(currentVect);
      this->theGeneratingElementPowersBasis.AddOnTop(currentVect);
      //std::cout << "<br>The basis: " << this->theGeneratingElementPowersBasis.ToString()
      //<< " has rank: " << this->theGeneratingElementPowersBasis.GetRankOfSpanOfElements();
      if (this->theGeneratingElementPowersBasis.size>this->theGeneratingElementPowersBasis.GetRankOfSpanOfElements())
      { this->theGeneratingElementPowersBasis.SetSize(this->theGeneratingElementPowersBasis.size-1);
        break;
      }
    } while (true);
    if (this->theGeneratingElementPowersBasis.size==DimOverRationals)
      break;
  }
}

bool AlgebraicClosureRationals::ReduceMe()
{ MacroRegisterFunctionWithName("AlgebraicExtensionRationals::ReduceMe");
//  double timeStart=0;
//  if (this->owner!=0)
//    if (this->owner->theGlobalVariables!=0)
//      timeStart=this->owner->theGlobalVariables->GetElapsedSeconds();
  this->ChooseGeneratingElement();
//  if (this->owner!=0)
//    if (this->owner->theGlobalVariables!=0)
//      std::cout << "<hr> Time needed to chose generating element: " << this->owner->theGlobalVariables->GetElapsedSeconds()-timeStart;
//  std::cout << "Reducing: " << this->ToString();
  if (this->flagIsQuadraticRadicalExtensionRationals)
    return true;
  Polynomial<Rational> theMinPoly, smallestFactor;
  theMinPoly.AssignMinPoly(this->GeneratingElementMatForm);
  int theDim=this->theBasisMultiplicative.size;
//  std::cout << "<hr><br>Factoring: " << theMinPoly.ToString() << "</b></hr>";
  bool mustBeTrue=theMinPoly.FactorMeOutputIsSmallestDivisor(smallestFactor, 0);
  if (!mustBeTrue)
  { std::cout << "This is a programming error: failed to factor polynomial " << theMinPoly.ToString() << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
//  std::cout << "<br>After factoring, min poly=" << theMinPoly.ToString() << " factor= " << smallestFactor.ToString();
  if (smallestFactor.TotalDegreeInt()==theDim)
    return true;
//  std::cout << "<br>Min poly factors.";
  MatrixTensor<Rational> theBasisChangeMat, theBasisChangeMatInverse;
  theBasisChangeMat.AssignVectorsToColumns(this->theGeneratingElementPowersBasis);
  theBasisChangeMatInverse=theBasisChangeMat;
  theBasisChangeMatInverse.Invert();
//  if (injectionFromLeftParent!=0)
//    injectionFromLeftParent->MultiplyOnTheLeft(theBasisChangeMatInverse);
//  if (injectionFromRightParent!=0)
//    injectionFromRightParent->MultiplyOnTheLeft(theBasisChangeMatInverse);
  Polynomial<Rational> zToTheNth, remainderAfterReduction, tempP;
  MatrixTensor<Rational> theProjection;
  int smallestFactorDegree=-1;
  if (!smallestFactor.TotalDegree().IsSmallInteger(&smallestFactorDegree))
  { std::cout << "This is a programming error: " << smallestFactor.ToString() << " has non-integral exponent, which "
    << " should be impossible in the current context. " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  //theProjection.init(smallestFactorDegree, this->DimOverRationals);
  theProjection.MakeZero();
  for (int i=0; i<smallestFactorDegree; i++)
    theProjection.AddMonomial(MonomialMatrix(i,i),1);
  for (int i=smallestFactorDegree; i<theDim; i++)
  { zToTheNth.MakeMonomiaL(0, i, 1, 1);
    zToTheNth.DivideBy(smallestFactor, tempP, remainderAfterReduction);
    for (int j=0; j<remainderAfterReduction.size(); j++)
    { int theIndex=-1;
      remainderAfterReduction[j](0).IsSmallInteger(&theIndex);
      theProjection.AddMonomial(MonomialMatrix(theIndex, i),remainderAfterReduction.theCoeffs[j]);
    }
  }
  this->theBasisMultiplicative.SetSize(smallestFactorDegree);
  MonomialMatrix tempM;
  for (int i=0; i<this->theBasisMultiplicative.size; i++)
  { this->theBasisMultiplicative[i].MakeZero();
    for (int j=0; j<this->theBasisMultiplicative.size; j++)
    { zToTheNth.MakeMonomiaL(0, i+j, 1, 1);
      zToTheNth.DivideBy(smallestFactor, tempP, remainderAfterReduction);
      for (int k=0; k<remainderAfterReduction.size(); k++)
      { int theIndex;
        remainderAfterReduction[k](0).IsSmallInteger(&theIndex);
        tempM.vIndex=theIndex;
        tempM.dualIndex=j;
        this->theBasisMultiplicative[i].AddMonomial(tempM, remainderAfterReduction.theCoeffs[k]);
      }
    }
  }
  this->GeneratingElemenT.owner=this;
  if (this->theBasisMultiplicative.size>1)
    this->GeneratingElemenT.theElT.MaKeEi(1, 1);
  else
    this->GeneratingElemenT.theElT.MaKeEi(0, 1);
  this->GetMultiplicationBy(this->GeneratingElemenT, this->GeneratingElementTensorForm);
  this->GeneratingElementTensorForm.GetMatrix(this->GeneratingElementMatForm, this->theBasisMultiplicative.size);
  this->RegisterNewBasis(theProjection);
  return true;
}

void AlgebraicClosureRationals::GetAdditionTo(const AlgebraicNumber& input, VectorSparse<Rational>& output)
{ MacroRegisterFunctionWithName("AlgebraicClosureRationals::GetAdditionTo");
  if (&output==&input.theElT)
  { AlgebraicNumber anCopy=input;
    this->GetAdditionTo(anCopy, output);
    return;
  }
  if (input.owner==0)
  { if (input.theElT.size()>0)
      output.MaKeEi(0, input.theElT.theCoeffs[0]);
    return;
  }
  if (input.basisIndex<0 || input.basisIndex>=this->theBasesAdditive.size)
  { std::cout << "This is a programming error: element has basis index " << input.basisIndex << ". "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
  }
  if (input.basisIndex==this->theBasesAdditive.size-1)
  { output=input.theElT;
    return;
  }
  output.MakeZero();
  for (int i=0; i<input.theElT.size(); i++)
  { int currentIndex=input.theElT[i].theIndex;
    if (currentIndex<0 || currentIndex>=this->theBasesAdditive[input.basisIndex].size)
    { std::cout << "This is a programming error: I am getting basis index " << input.basisIndex << " with current index " << currentIndex
      << ". A printout of the algebraic closure follows. " << this->ToString() << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    output.AddOtherTimesConst(this->theBasesAdditive[input.basisIndex][currentIndex], input.theElT.theCoeffs[i]);
  }
}

void AlgebraicClosureRationals::GetMultiplicationBy
  (const AlgebraicNumber& input, MatrixTensor<Rational>& output)
{ MacroRegisterFunctionWithName("AlgebraicClosureRationals::GetMultiplicationBy");
  output.MakeZero();
  VectorSparse<Rational> inputAdditiveForm;
  this->GetAdditionTo(input, inputAdditiveForm);
  MatrixTensor<Rational> currentMat;
  for (int i=0; i<inputAdditiveForm.size(); i++)
  { if (inputAdditiveForm[i].theIndex<0 || inputAdditiveForm[i].theIndex>=this->theBasisMultiplicative.size)
    { std::cout << "This is a programming error: element " << input.ToString() << " has bad index, namely, "
      << inputAdditiveForm[i].theIndex << ". The algebraic closure is: " << this->ToString() << ". " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    currentMat= this->theBasisMultiplicative[inputAdditiveForm[i].theIndex];
    currentMat*=inputAdditiveForm.theCoeffs[i];
    output+=currentMat;
  }
}

Rational AlgebraicNumber::GetDenominatorRationalPart()const
{ if (this->owner==0)
  { if (this->theElT.IsEqualToZero())
      return 1;
    return this->theElT.theCoeffs[0].GetDenominator();
  }
  VectorSparse<Rational> theEltAdditive;
  this->owner->GetAdditionTo(*this, theEltAdditive);
  LargeIntUnsigned resultLCM=1;
  for (int i=0; i<theEltAdditive.size(); i++)
    resultLCM=LargeIntUnsigned::lcm(resultLCM, theEltAdditive.theCoeffs[i].GetDenominator());
  return resultLCM;
}

Rational AlgebraicNumber::GetNumeratorRationalPart()const
{ if (this->owner==0)
  { if (this->theElT.IsEqualToZero())
      return 0;
    return this->theElT.theCoeffs[0].GetNumerator();
  }
  VectorSparse<Rational> theEltAdditive;
  this->owner->GetAdditionTo(*this, theEltAdditive);
  LargeInt resultGCD=1;
  LargeIntUnsigned tempR;
  if (theEltAdditive.size()>0)
    resultGCD= theEltAdditive.theCoeffs[0].GetNumerator();
  for (int i=1; i<theEltAdditive.size(); i++)
  { tempR=resultGCD.value;
    LargeIntUnsigned::gcd(tempR, theEltAdditive.theCoeffs[i].GetNumerator().value, resultGCD.value);
  }
  return resultGCD;
}

unsigned int AlgebraicNumber::HashFunction()const
{ //assert(false);
  //WARNING. Algebraic numbers, as they are recorded in memory at the moment,
  //do not have unique presentations, so we return 0 as their hash function.
  //Computing a hash function can be done, for example, by picking the hash function of the minimal polynomial
  //over the rationals. However, such computations appear to be too heavy, and will make sense only if we need to deal
  //with large sets of algebraic numbers without repetition.
  return 0;
}

bool AlgebraicNumber::operator==(const Rational& other)const
{ if (this->theElT.IsEqualToZero())
    return other==0;
  if (this->theElT.size()!=1)
    return false;
  if (this->theElT[0].theIndex!=0)
    return false;
  return this->theElT.theCoeffs[0]==other;
}

bool AlgebraicNumber::NeedsBrackets()const
{ if (this->owner==0)
    return false;
  VectorSparse<Rational> additiveForm;
  this->owner->GetAdditionTo(*this, additiveForm);
  return (additiveForm.size()>1);
}

bool AlgebraicNumber::CheckNonZeroOwner()const
{ if (this->owner==0)
  { std::cout << "This is a programming error: algebraic number with non-initialized owner not permitted in the current context."
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  return true;
}

bool AlgebraicNumber::ConstructFromMinPoly(const Polynomial<AlgebraicNumber>& thePoly, AlgebraicClosureRationals& inputOwner, GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("AlgebraicNumber::ConstructFromMinPoly");
  return inputOwner.AdjoinRootMinPoly(thePoly, *this, theGlobalVariables);
}

void AlgebraicClosureRationals::reset()
{ this->flagIsQuadraticRadicalExtensionRationals=true;
  this->theBasisMultiplicative.SetSize(1);
  this->theBasisMultiplicative[0].MakeId(1);

  this->theGeneratingElementPowersBasis.SetSize(1);
  this->theGeneratingElementPowersBasis[0].MakeEi(1,0);
  this->theBasesAdditive.SetSize(1);
  this->theBasesAdditive[0].SetSize(1);
  this->theBasesAdditive[0][0].MaKeEi(0);
  this->theQuadraticRadicals.Clear();
  this->DisplayNamesBasisElements.SetSize(1);
  this->DisplayNamesBasisElements[0]="";
  this->GeneratingElementTensorForm.MakeId(1);
  this->GeneratingElementMatForm.MakeIdMatrix(1);
  this->GeneratingElemenT.owner=this;
  this->GeneratingElemenT.theElT.MaKeEi(0);
}

bool AlgebraicClosureRationals::AdjoinRootQuadraticPolyToQuadraticRadicalExtension
(const Polynomial<AlgebraicNumber>& thePoly, AlgebraicNumber& outputRoot, GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("AlgebraicClosureRationals::AdjoinRootQuadraticPolyToQuadraticRadicalExtension");
  if (thePoly.TotalDegree()!=2|| !this->flagIsQuadraticRadicalExtensionRationals)
    return false;
  Polynomial<AlgebraicNumber> algNumPoly;
  this->ConvertPolyDependingOneVariableToPolyDependingOnFirstVariableNoFail(thePoly, algNumPoly);
  Polynomial<Rational> minPoly;
  minPoly.MakeZero();
  Rational currentCF, theLinearTermCFdividedByTwo, theConstTermShifted;
  for (int i=0; i<algNumPoly.size(); i++)
    if (!algNumPoly.theCoeffs[i].IsRational(&currentCF))
      return false;
    else
      minPoly.AddMonomial(algNumPoly[i], currentCF);
  minPoly/=minPoly.GetMonomialCoefficient(minPoly.GetMaxMonomial());
  minPoly.GetCoeffInFrontOfLinearTermVariableIndex(0, theLinearTermCFdividedByTwo);
  theLinearTermCFdividedByTwo/=2;
  minPoly.GetConstantTerm(theConstTermShifted);
  theConstTermShifted-=theLinearTermCFdividedByTwo*theLinearTermCFdividedByTwo;
  theConstTermShifted*=-1;
//  std::cout << "<hr>Adjoining radical of: " << theConstTermShifted;
  if (!outputRoot.AssignRationalQuadraticRadical(theConstTermShifted, *this))
    return false;
//  std::cout << " ... to get: " << outputRoot.ToString();
  outputRoot-=theLinearTermCFdividedByTwo;
  //Check our work:
  PolynomialSubstitution<AlgebraicNumber> checkSub;
  checkSub.SetSize(1);
  checkSub[0].MakeConst(outputRoot);
  algNumPoly.Substitution(checkSub);
  if (!algNumPoly.IsEqualToZero())
  { std::cout << "This is a programming error. The number z:=" << outputRoot.ToString() << " was just adjoined to the base field; z"
    << " was given by requesting that it has minimial polynomial " << algNumPoly.ToString() << ", however, substituting z back in to the minimal polynomial "
    << "does not yield zero, rather yields " << algNumPoly.ToString() << ". " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  //check end
  return true;

}

void AlgebraicClosureRationals::ConvertPolyDependingOneVariableToPolyDependingOnFirstVariableNoFail
(const Polynomial<AlgebraicNumber>& input, Polynomial<AlgebraicNumber>& output)
{ MacroRegisterFunctionWithName("AlgebraicClosureRationals::ConvertPolyDependingOneVariableToPolyDependingOnFirstVariableNoFail");
  int indexVar=-1;
  if (!input.IsOneVariableNonConstPoly(&indexVar))
  { std::cout << "This is a programming error: I am being asked convert to a one-variable polynomial a polynomial "
    << "depending on more than one variables. The input poly is: " << input.ToString() << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  PolynomialSubstitution<AlgebraicNumber> theSub;
  theSub.MakeIdSubstitution(indexVar+1);
  theSub[indexVar].MakeMonomiaL(0, 1, 1);
  output=input;
//  std::cout << "<hr>" << output.ToString() << "<br>";
  output.Substitution(theSub);
//  std::cout << "<hr>" << output.ToString() << "<br>";
}

bool AlgebraicClosureRationals::AdjoinRootMinPoly(const Polynomial<AlgebraicNumber>& thePoly, AlgebraicNumber& outputRoot, GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("AlgebraicClosureRationals::AdjoinRootMinPoly");
  if(this->AdjoinRootQuadraticPolyToQuadraticRadicalExtension(thePoly, outputRoot, theGlobalVariables))
    return true;
  Polynomial<AlgebraicNumber> minPoly;
  this->ConvertPolyDependingOneVariableToPolyDependingOnFirstVariableNoFail(thePoly, minPoly);
  int indexMaxMonMinPoly=minPoly.GetIndexMaxMonomial();
  AlgebraicNumber leadingCF=minPoly.theCoeffs[indexMaxMonMinPoly];
  minPoly/=leadingCF;
  AlgebraicClosureRationals backUpCopy;
  backUpCopy=*this;
  MatrixTensor<Rational> theGenMat;
  int degreeMinPoly=minPoly.TotalDegreeInt();
  int startingDim=this->theBasisMultiplicative.size;
  if (degreeMinPoly*startingDim> 10000 || startingDim>10000 || degreeMinPoly>10000)
  { std::cout << "<hr>Adjoining root of minimial polynomial failed: the current field extension dimension over the rationals is "
    << startingDim << ", the degree of the minimial polynomial is " << degreeMinPoly << ", yielding expected final dimension "
    << startingDim << "*" << degreeMinPoly << " = " << startingDim*degreeMinPoly << " over the rationals. The calculator is hard-coded "
    << " to accept dimension over the rationals no larger than 10000 (multiplication in such a field corresponds to a "
    << " 10000x10000 matrix (100 000 000 entries). If you do indeed want to carry out such large computations, you need to "
    << " compile the calculator on your own, modifying file " << __FILE__ << ", line " << __LINE__ << ".";
    return false;
  }
  theGenMat.MakeZero();
  for (int i=0; i<degreeMinPoly-1; i++)
    for (int j=0; j<startingDim; j++)
      theGenMat.AddMonomial(MonomialMatrix((i+1)*startingDim+j, i*startingDim+j), 1);
  Polynomial<AlgebraicNumber> minusMinPolyMinusMaxMon=minPoly;
  int indexMaxMon=minusMinPolyMinusMaxMon.GetIndexMaxMonomial(MonomialP::LeftIsGEQTotalDegThenLexicographicLastVariableStrongest);
  const MonomialP maxMon=minusMinPolyMinusMaxMon[indexMaxMon];
  AlgebraicNumber maxMonCoeff=minusMinPolyMinusMaxMon.theCoeffs[indexMaxMon];
  minusMinPolyMinusMaxMon.SubtractMonomial(maxMon, maxMonCoeff);
  minusMinPolyMinusMaxMon/=maxMonCoeff;
  minusMinPolyMinusMaxMon/=-1;
  MatrixTensor<Rational> currentCoeffMatForm;
  for (int i=0; i<minusMinPolyMinusMaxMon.size(); i++)
  { AlgebraicNumber& currentCoeff=minusMinPolyMinusMaxMon.theCoeffs[i];
    const MonomialP& currentMon=minusMinPolyMinusMaxMon[i];
    this->GetMultiplicationBy(currentCoeff, currentCoeffMatForm);
    for (int j=0; j<currentCoeffMatForm.size(); j++)
    { int relRowIndex=currentCoeffMatForm[j].vIndex;
      int relColIndex=currentCoeffMatForm[j].dualIndex;
      if (relRowIndex==-1 || relColIndex==-1)
      { std::cout << "This is a programming error: non initialized monomial. " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
        assert(false);
      }
      theGenMat.AddMonomial(MonomialMatrix(currentMon.TotalDegreeInt()*startingDim+relRowIndex, startingDim*(degreeMinPoly-1)+relColIndex), currentCoeffMatForm.theCoeffs[j]);
    }
  }
  int finalDim=degreeMinPoly*startingDim;
  List<MatrixTensor<Rational> > finalBasis;
  finalBasis.SetSize(this->theBasisMultiplicative.size);
  MatrixTensor<Rational> theGenMatPower;
  theGenMatPower.MakeId(degreeMinPoly);
  for (int i=0; i<startingDim; i++)
    finalBasis[i].AssignTensorProduct(theGenMatPower, this->theBasisMultiplicative[i]);
  this->theBasisMultiplicative=finalBasis;
  theBasisMultiplicative.SetSize(finalDim);
  theGenMatPower=theGenMat;
  for (int i=1; i<degreeMinPoly; i++)
  { for (int j=0; j<startingDim; j++)
    { this->theBasisMultiplicative[i*startingDim+j]=theGenMatPower;
      this->theBasisMultiplicative[i*startingDim+j]*=finalBasis[j];
    }
    if (i!=degreeMinPoly-1)
      theGenMatPower*=theGenMat;
  }
  MatrixTensor<Rational> theInjection;
  theInjection.MakeId(startingDim);
  this->RegisterNewBasis(theInjection);
  outputRoot.owner=this;
  outputRoot.theElT.MaKeEi(startingDim);
  outputRoot.basisIndex=this->theBasesAdditive.size-1;
  this->flagIsQuadraticRadicalExtensionRationals=false;
  if (!this->ReduceMe())
  { *this=backUpCopy;
    return false;
  }
  //Sanity check code here:
  PolynomialSubstitution<AlgebraicNumber> theSub;
  theSub.SetSize(1);
  theSub[0].MakeConst(outputRoot);
  minPoly.Substitution(theSub);
  if (!minPoly.IsEqualToZero())
  { std::cout << "This is a programming error. The number z:=" << outputRoot.ToString() << " was just adjoined to the base field; z"
    << " was given by requesting that it has minimial polynomial " << minPoly.ToString() << ", however, substituting z back in to the minimal polynomial "
    << "does not yield zero, rather yields " << minPoly.ToString() << ". " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  //
  return true;
}

void AlgebraicNumber::Invert()
{ MacroRegisterFunctionWithName("AlgebraicNumber::Invert");
  if (this->owner==0)
  { if (this->theElT.IsEqualToZero())
    { std::cout << "This is a programming error: division by zero. " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    bool isGood=(this->theElT.size()==1);
    if (isGood)
      isGood=(this->theElT[0].theIndex==0);
    if (!isGood)
    { std::cout << "This is a programming error: Algebraic number has no owner, so it must be rational, but it appears to be not. "
      << " as its theElt vector is: " << this->theElT.ToString() << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    this->theElT.theCoeffs[0].Invert();
    return;
  }
  MatrixTensor<Rational> theInverted;
  Matrix<Rational> tempMat2;
  this->owner->GetMultiplicationBy(*this, theInverted);
  theInverted.GetMatrix(tempMat2, this->owner->theBasisMultiplicative.size);
  tempMat2.Invert();
  theInverted=tempMat2;
  this->theElT.MaKeEi(0);
  theInverted.ActOnVectorColumn(this->theElT);
  this->basisIndex=this->owner->theBasesAdditive.size-1;
}

void AlgebraicNumber::operator/=(const AlgebraicNumber& other)
{ MacroRegisterFunctionWithName("AlgebraicNumber::operator/=");
  AlgebraicNumber otherCopy=other;
  otherCopy.Invert();
//  std::cout << "<hr>other: " << other.theElt.ToString() << " inverted: " << otherCopy.ToString();
  *this*=otherCopy;
}

void AlgebraicNumber::operator*=(const Rational& other)
{ this->theElT*=(other);
}

bool AlgebraicNumber::CheckCommonOwner(const AlgebraicNumber& other)const
{ if (this->owner==0 || other.owner==0)
    return true;
  if (this->owner!=other.owner)
  { std::cout << "This is a programming error. Two algebraic numbers have different algebraic closures when they shouldn't. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
    return false;
  }
  return true;
}

void AlgebraicNumber::operator-=(const AlgebraicNumber& other)
{ MacroRegisterFunctionWithName("AlgebraicNumber::operator=");
  this->CheckCommonOwner(other);
  if (this->basisIndex==other.basisIndex)
  { this->theElT-=other.theElT;
    return;
  }
  AlgebraicClosureRationals* theOwner=this->owner;
  if (theOwner==0)
    theOwner=other.owner;
  if (theOwner==0)
  { std::cout << "This is a programming error: algebraic numbers with zero owners but different basis indices. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  VectorSparse<Rational> AdditiveFormOther;
  theOwner->GetAdditionTo(*this, this->theElT);
  theOwner->GetAdditionTo(other, AdditiveFormOther);
  this->owner=theOwner;
  this->basisIndex=theOwner->theBasesAdditive.size-1;
  this->theElT-=AdditiveFormOther;
}

void AlgebraicNumber::operator+=(const AlgebraicNumber& other)
{ MacroRegisterFunctionWithName("AlgebraicNumber::operator+=");
  //std::cout << "Adding " << this->ToString() << " to " << other.ToString();
  this->CheckCommonOwner(other);
  if (this->basisIndex==other.basisIndex)
  { this->theElT+=other.theElT;
    return;
  }
  AlgebraicClosureRationals* theOwner=this->owner;
  if (theOwner==0)
    theOwner=other.owner;
  if (theOwner==0)
  { std::cout << "This is a programming error: algebraic numbers with zero owners but different basis indices. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  VectorSparse<Rational> AdditiveFormOther;
  theOwner->GetAdditionTo(*this, this->theElT);
  theOwner->GetAdditionTo(other, AdditiveFormOther);
  this->owner=theOwner;
  this->basisIndex=theOwner->theBasesAdditive.size-1;
  this->theElT+=AdditiveFormOther;
//  std::cout << " ... to get: " << this->ToString();
}

bool AlgebraicNumber::CheckConsistency()const
{ if (this->flagDeallocated)
  { std::cout << "This is a programming error: use after free of AlgebraicNumber. " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  return true;
}

void AlgebraicNumber::operator*=(const AlgebraicNumber& other)
{ MacroRegisterFunctionWithName("AlgebraicNumber::operator*=");
  this->CheckConsistency();
  if (other.owner==0)
  { if (other.IsEqualToZero())
    { this->theElT.MakeZero();
      return;
    }
    this->theElT*=other.theElT.theCoeffs[0];
    return;
  }
  if (this->owner==0)
  { if (this->theElT.IsEqualToZero())
      return;
    Rational tempRat=this->theElT.GetMonomialCoefficient(MonomialVector(0));
    *this=other;
    *this*=tempRat;
    return;
  }
  this->CheckCommonOwner(other);
  AlgebraicNumber otherCopy=other;
//  std::cout << "Converting <hr>" << CGI::GetHtmlMathSpanPure(this->ToString()) << " and <br><br>\n\n\n\n<br><br>"
//  << CGI::GetHtmlMathSpanPure(otherCopy.ToString());
//  std::cout << " <br><br>To get: " << CGI::GetHtmlMathSpanPure(this->ToString()) << "<br>\n\n and  <br><br>\n\n\n\n<br><br> \n"
//  << CGI::GetHtmlMathSpanPure(otherCopy.ToString()) ;
  //std::cout << " <hr>multiplying " << this->theElt.ToString() << " by " << other.theElt.ToString() << " ";
  MatrixTensor<Rational> leftMat, rightMat;
  FormatExpressions tempformat;
  tempformat.flagUseLatex=true;
  tempformat.flagUseHTML=false;
  this->owner->GetMultiplicationBy(*this, leftMat);
  this->owner->GetMultiplicationBy(otherCopy,rightMat);
//  std::cout << "<br><br>\n\n\n\n<br><br> in matrix form: " << CGI::GetHtmlMathSpanPure(leftMat.ToStringMatForm(&tempformat)) << " by "
//  << CGI::GetHtmlMathSpanPure(rightMat.ToStringMatForm(&tempformat));
  leftMat*=rightMat;
  this->basisIndex=this->owner->theBasesAdditive.size-1;
  this->theElT.MaKeEi(0);
  //std::cout << "matrix " << CGI::GetHtmlMathSpanPure(leftMat.ToStringMatForm(&tempformat));
  leftMat.ActOnVectorColumn(this->theElT);
  //std::cout << this->theElt.ToString();
}

void AlgebraicNumber::SqrtMeDefault()
{ this->RadicalMeDefault(2);
}

void AlgebraicNumber::AssignRational(const Rational& input, AlgebraicClosureRationals& inputOwner)
{ this->owner=0;
  this->theElT.MaKeEi(0, input);
}

bool AlgebraicNumber::AssignRationalQuadraticRadical(const Rational& inpuT, AlgebraicClosureRationals& inputOwner)
{ MacroRegisterFunctionWithName("AlgebraicNumber::AssignRationalRadical");
  //std::cout << "<hr>Assigning rational radical of  " << inpuT.ToString();
  if (inpuT==0)
    return false;
  if (!inputOwner.flagIsQuadraticRadicalExtensionRationals)
  { Polynomial<AlgebraicNumber> minPoly;
    minPoly.MakeMonomiaL(0,2);
    minPoly-=inpuT;
    return this->ConstructFromMinPoly(minPoly, inputOwner, inputOwner.theGlobalVariables);
  }
  List<LargeInt> theFactors;
  Rational absoluteInput=inpuT;
  if (absoluteInput<0)
  { theFactors.AddOnTop(-1);
    absoluteInput*=-1;
  }
  LargeInt squareFreeInput=absoluteInput.GetNumerator();
  squareFreeInput*=absoluteInput.GetDenominator();
  List<unsigned int> primeFactors;
  List<int> theMults;
  if (!squareFreeInput.value.Factor(primeFactors, theMults))
    return false;
  squareFreeInput.value=1;
  Rational squareRootRationalPart=1;
  squareRootRationalPart/=absoluteInput.GetDenominator();
  for (int i=0; i<primeFactors.size; i++)
  { if (theMults[i]%2==1)
      squareFreeInput*=primeFactors[i];
    Rational tempLI=primeFactors[i];
    tempLI.RaiseToPower(theMults[i]/2);
    squareRootRationalPart*=tempLI;
  }
//  squareRootRationalPart/=input.GetDenominator();

  if (!squareFreeInput.IsEqualToOne())
    theFactors.AddOnTop(squareFreeInput);
  if (theFactors.size==0)
  { this->AssignRational(squareRootRationalPart, inputOwner);
    return true;
  }
  if(!inputOwner.MergeRadicals(theFactors))
    return false;
//  std::cout << "After merging radicals, the field is: " << inputOwner.ToString();
  Selection FactorSel;
  FactorSel.init(inputOwner.theQuadraticRadicals.size);
  for (int i=0; i<theFactors.size; i++)
    if (theFactors[i]==-1)
      FactorSel.AddSelectionAppendNewIndex(inputOwner.theQuadraticRadicals.GetIndex(-1));
    else
      for (int j=0; j<inputOwner.theQuadraticRadicals.size; j++)
        if (inputOwner.theQuadraticRadicals[j]!=-1)
          if (theFactors[i]%inputOwner.theQuadraticRadicals[j]==0)
            FactorSel.AddSelectionAppendNewIndex(j);
  this->owner=&inputOwner;
  this->basisIndex=this->owner->theBasesAdditive.size-1;
  this->theElT.MaKeEi(this->owner->GetIndexFromRadicalSelection(FactorSel));
  this->theElT*=squareRootRationalPart;
//  VectorSparse<Rational> commentMeOut;
//  this->owner->GetAdditionTo(*this, commentMeOut);
//  std::cout << "<hr>radical additive form: " << commentMeOut.ToString() << "<br>radical final value: " << this->ToString() << "<br>";
  return true;
}

void AlgebraicNumber::RadicalMeDefault(int theRad)
{ std::cout << "Not implemented yet!" << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
  assert(false);
/*  MatrixTensor<Rational> theRadicalOp;
  theRadicalOp.MakeZero();
  MonomialTensor tempM;
  for (int i=0; i<this->GetMinPoly().size; i++)
  { tempM=this->GetMinPoly()[i];
    tempM.ExponentMeBy(theRad);
    newMinPoly.AddMonomial(tempM, this->GetMinPoly().theCoeffs[i]);
  }
  this->rootIndex=0;
  this->minPolyIndex= this->theRegistry->theMinPolys.AddNoRepetitionOrReturnIndexFirst(newMinPoly);*/
}

std::string AlgebraicClosureRationals::ToString(FormatExpressions* theFormat)const
{ std::stringstream out;
  FormatExpressions tempFormat;
  tempFormat.flagUseHTML=false;
  tempFormat.flagUseLatex=true;
  out << "Multiplicative basis";
  for (int i=0; i<this->theBasisMultiplicative.size; i++)
  { out << "<br>";
    std::stringstream theFlaStream;
    if (i<this->DisplayNamesBasisElements.size)
    { if (this->DisplayNamesBasisElements[i]=="")
        theFlaStream << "1";
      else
        theFlaStream << this->DisplayNamesBasisElements[i];
    } else
      theFlaStream << " e_{" << i+1 << "}";
    theFlaStream << ":=" << this->theBasisMultiplicative[i].ToStringMatForm(&tempFormat);
    out << CGI::GetHtmlMathSpanPure(theFlaStream.str());
    if (i!=this->theBasisMultiplicative.size-1)
      out << ",  ";
  }
  if (this->flagIsQuadraticRadicalExtensionRationals)
    out << "<br>Generating element not selected. ";
  else
    out << this->GeneratingElementMatForm.ToString(&tempFormat);
  out << "<br>There are " <<  this->theBasesAdditive.size << " registered old bases. ";
  for (int i=0; i<this->theBasesAdditive.size; i++)
  { out << "<hr>Basis " << i+1 << " has " << this->theBasesAdditive[i].size << " elements: ";
    for (int j=0; j<this->theBasesAdditive[i].size; j++)
    { out << CGI::GetHtmlMathSpanPure(this->theBasesAdditive[i][j].ToString(&tempFormat));
      if (j!=this->theBasesAdditive[i].size-1)
        out << ", ";
    }
  }
  return out.str();
}

bool AlgebraicNumber::IsRational(Rational* whichRational)const
{ if (this->IsEqualToZero())
  { if (whichRational!=0)
      *whichRational=0;
    return true;
  }
  for (int i=0; i<this->theElT.size(); i++)
    if (this->theElT[i].theIndex!=0)
      return false;
    else
      if (whichRational!=0)
        *whichRational=this->theElT.theCoeffs[i];
  return true;
}

bool AlgebraicNumber::IsEqualToZero()const
{ return this->theElT.IsEqualToZero();
}

std::string AlgebraicNumber::ToString(FormatExpressions* theFormat)const
{ if (this->owner==0)
  { if (this->theElT.IsEqualToZero())
      return "0";
    return this->theElT.theCoeffs[0].ToString();
  }
  std::stringstream out;
  FormatExpressions tempFormat;
  tempFormat.vectorSpaceEiBasisNames=this->owner->DisplayNamesBasisElements;
  VectorSparse<Rational> theAdditiveVector;
  this->owner->GetAdditionTo(*this, theAdditiveVector);
  out << theAdditiveVector.ToString(&tempFormat); //<< "~ in~ the~ field~ " << this->owner->ToString();
  return out.str();
}

bool AlgebraicNumber::operator==(const AlgebraicNumber& other)const
{ Rational ratValue;
  if (this->IsRational(&ratValue))
    return other==ratValue;
  if (other.IsRational(&ratValue))
    return *this==ratValue;
  if (this->owner!=other.owner)
  { std::cout << "This might or might not be a programming error: comparing two algebraic number that do not have the same owner. "
    << "The numbers have owners of respective addresses " << this->owner << " and " << other.owner << ". "
    << "Crashing to let you know. " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  this->CheckNonZeroOwner();
  if (this->basisIndex==other.basisIndex)
    return this->theElT==other.theElT;
  VectorSparse<Rational> leftAdditive, rightAdditive;
  this->owner->GetAdditionTo(*this, leftAdditive);
  this->owner->GetAdditionTo(other, rightAdditive);
  return leftAdditive==rightAdditive;
}

void AlgebraicNumber::operator=(const Polynomial<AlgebraicNumber>& other)
{ if (!other.IsAConstant(this))
  { std::cout << "This is a programming error: attempting to assign non-constant polynomial to a Rational number is not allowed. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
}

void AlgebraicNumber::operator=(const Rational& other)
{ this->owner=0;
  this->theElT.MaKeEi(0, other);
  this->basisIndex=0;
}

void Rational::operator=(const AlgebraicNumber& other)
{ bool isGood=other.IsRational(this);
  if (!isGood)
  { std::cout << "This is a programming error: attempting to assign the non-rational algebraic number " << other.ToString() << "to a rational number. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
}

std::string ElementZmodP::ToString(FormatExpressions* theFormat)const
{ std::stringstream out;
  out << "(" << this->theValue.ToString() << " ~mod~ " << this->theModulo.ToString() << ")";
  return out.str();
}

bool ElementZmodP::AssignRational(const Rational& other)
{ this->CheckIamInitialized();
  *this= other.GetNumerator();
  ElementZmodP den;
  den.theModulo=this->theModulo;
  den=other.GetDenominator();
  if (den.IsEqualToZero())
    return false;
  *this/=den;
  return true;
}

void ElementZmodP::operator/=(const ElementZmodP& other)
{ this->CheckIamInitialized();
  this->CheckEqualModuli(other);
  LargeInt theInverted, otherValue, theMod;
  theMod=this->theModulo;
  otherValue=other.theValue;
  theInverted=MathRoutines::InvertXModN(otherValue, theMod);
  *this*=theInverted;
}
