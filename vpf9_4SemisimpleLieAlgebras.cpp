//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#include "vpfHeader1_4SemisimpleLieAlgebras.h"
ProjectInformationInstance ProjectInfoVpf9_4cpp
(__FILE__, "Implementation of semisimple subalgebra routines. ");

template<class CoefficientType>
void SemisimpleLieAlgebra::GenerateLieSubalgebra
(List<ElementSemisimpleLieAlgebra<CoefficientType> >& inputOutputGenerators)
{ ElementSemisimpleLieAlgebra<CoefficientType> theBracket;
  HashedList<ChevalleyGenerator> seedMons;
  for (int i=0; i<inputOutputGenerators.size; i++)
    for (int j=0; j<inputOutputGenerators.size; j++)
    { this->LieBracket(inputOutputGenerators[i], inputOutputGenerators[j], theBracket);
      inputOutputGenerators.AddOnTop(theBracket);
      if (theBracket.GetRankOfSpanOfElements
          (inputOutputGenerators, &seedMons)<inputOutputGenerators.size)
        inputOutputGenerators.RemoveLastObject();
    }
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

std::string SemisimpleSubalgebras::ToString(FormatExpressions* theFormat)
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::ToString");
  std::stringstream out;
  int candidatesRealized=0;
  int candidatesNotRealizedNotProvenImpossible=0;
  int candidatesProvenImpossible=0;
  for (int i=0; i<this->Hcandidates.size; i++)
  { CandidateSSSubalgebra& currentSA=this->Hcandidates[i];
    if (currentSA.flagSystemProvedToHaveNoSolution)
      candidatesProvenImpossible++;
    if (currentSA.flagSystemSolved)
      candidatesRealized++;
  }
  candidatesNotRealizedNotProvenImpossible=
  this->Hcandidates.size-candidatesRealized- candidatesProvenImpossible;
  out << candidatesRealized << " subalgebras realized.";
  out << "<br>Total, there are " << this->Hcandidates.size << " = " << candidatesRealized
  << " realized + " << candidatesProvenImpossible << " proven impossible + "
  << candidatesNotRealizedNotProvenImpossible << " neither realized nor proven impossible. \n<hr>\n ";
  for (int i=0; i<this->Hcandidates.size; i++)
    out << this->Hcandidates[i].ToString(theFormat) << "\n<hr>\n ";
  if (this->theSl2s.size!=0)
    out << this->theSl2s.ToString(theFormat);
  return out.str();
}

void SemisimpleSubalgebras::FindAllEmbeddings
(DynkinSimpleType& theType, SemisimpleLieAlgebra& theOwner, GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::FindAllEmbeddings");
  this->owneR=&theOwner;
  this->GetSSowner().FindSl2Subalgebras(theOwner, this->theSl2s, *theGlobalVariables);
  CandidateSSSubalgebra theCandidate;
  theCandidate.owner=this;
  this->ExtendOneComponentOneTypeAllLengthsRecursive
  (theCandidate, theType, false, theGlobalVariables);
}

void SemisimpleSubalgebras::FindTheSSSubalgebras
(SemisimpleLieAlgebra& newOwner, GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::FindTheSSSubalgebras");
  this->owneR=&newOwner;
  this->GetSSowner().FindSl2Subalgebras(newOwner, this->theSl2s, *theGlobalVariables);
  this->FindTheSSSubalgebrasPart2(theGlobalVariables);
}

void SemisimpleSubalgebras::FindTheSSSubalgebrasPart2
(GlobalVariables* theGlobalVariables)
{ CandidateSSSubalgebra emptyCandidate;
  emptyCandidate.owner=this;
  this->ExtendCandidatesRecursive(emptyCandidate, true, theGlobalVariables);
}

void SemisimpleSubalgebras::AddCandidatesSubalgebra
(CandidateSSSubalgebra& input, GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::AddCandidatesSubalgebra");
  SemisimpleLieAlgebra tempSA;
  tempSA.theWeyl=input.theWeylNonEmbeddeDdefaultScale;
  bool needToComputeConstants=this->theSubalgebrasNonEmbedded.Contains(tempSA);
  int theIndex=this->theSubalgebrasNonEmbedded.AddNoRepetitionOrReturnIndexFirst(tempSA);
  if (needToComputeConstants)
    this->theSubalgebrasNonEmbedded[theIndex].ComputeChevalleyConstantS(theGlobalVariables);
  input.indexInOwnersOfNonEmbeddedMe=theIndex;
}

void DynkinType::GetDynkinTypeWithDefaultLengths(DynkinType& output)
{ if (&output==this)
  { DynkinType thisCopy=*this;
    thisCopy.GetDynkinTypeWithDefaultLengths(output);
    return;
  }
  output.MakeZero();
  DynkinSimpleType tempType;
  for (int i =0; i<this->size; i++)
  { tempType.MakeArbitrary((*this)[i].theLetter, (*this)[i].theRank);
    tempType.lengthFirstCoRootSquared=tempType.GetDefaultCoRootLengthSquared(0);
    output.AddMonomial(tempType, this->theCoeffs[i]);
  }
}

DynkinSimpleType DynkinType::GetGreatestSimpleType()const
{ if (this->size==0)
  { std::cout << "This is a programming error: asking for the greatest simple type "
    << " of a 0 dynkin type. " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  DynkinSimpleType result=(*this)[0];
  for (int i=1; i<this->size; i++)
    if ((*this)[i]>result)
      result=(*this)[i];
  return result;
}

void SemisimpleSubalgebras::ExtendOneComponentRecursive
(const CandidateSSSubalgebra& baseCandidate, bool propagateRecursion,
GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::ExtendOneComponentRecursive");
  baseCandidate.CheckInitialization();
  RecursionDepthCounter theCounter(&this->theRecursionCounter);
  int numVectorsFound=baseCandidate.CartanSAsByComponent.LastObject()->size;
  CandidateSSSubalgebra newCandidate;
  DynkinSimpleType theNewTypE=baseCandidate.theWeylNonEmbeddeD.theDynkinType.GetGreatestSimpleType();
  ProgressReport theReport(theGlobalVariables);
  ProgressReport theReport0(theGlobalVariables);
  ProgressReport theReport1(theGlobalVariables);
  ProgressReport theReport2(theGlobalVariables);
  if (numVectorsFound==theNewTypE.theRank)
  { newCandidate=baseCandidate;
    this->AddCandidatesSubalgebra(newCandidate, theGlobalVariables);
    if (!newCandidate.ComputeChar(theGlobalVariables))
    { theReport.Report("Candidate " + newCandidate.ToString() + " ain't no good");
      //std::cout << newCandidate.ToString() << " is bad<br>";
      return;
    }
    if (!newCandidate.ComputeSystem(theGlobalVariables))
    { theReport.Report("Candidate " + newCandidate.ToString() + " ain't no good");
      return;
    }
//    std::cout << newCandidate.ToString() << "<b> is good</b><br>";
    this->Hcandidates.AddOnTop(newCandidate);
    if (propagateRecursion)
      this->ExtendCandidatesRecursive(newCandidate, propagateRecursion, theGlobalVariables);
    return;
  }
  int indexFirstWeight=
  baseCandidate.theWeylNonEmbeddeD.CartanSymmetric.NumRows - theNewTypE.theRank;
  int indexCurrentWeight=indexFirstWeight+numVectorsFound;
  Rational desiredLengthSquared=theNewTypE.lengthFirstCoRootSquared;
  desiredLengthSquared*=theNewTypE.GetDefaultRootLengthSquared(0);
  desiredLengthSquared/=theNewTypE.GetDefaultRootLengthSquared(numVectorsFound);
  Vectors<Rational> startingVector;
  HashedList<Vector<Rational> > theOrbit;
  HashedList<ElementWeylGroup> theOrbitGenerators;
  Vector<Rational> Hrescaled;
  startingVector.SetSize(1);
  std::stringstream out0;
  if (theGlobalVariables!=0)
  { out0 << "Attempting to complete component " << theNewTypE.ToString()
    << ", root " << baseCandidate.CartanSAsByComponent.LastObject()->size+1
    << " out of " << theNewTypE.theRank << ".";
    theReport0.Report(out0.str());
  }
  for (int i=0; i<this->theSl2s.size; i++)
  { if (theGlobalVariables!=0)
    { std::stringstream reportStreamX;
      reportStreamX
      << "Extending  " << baseCandidate.theWeylNonEmbeddeD.theDynkinType.ToString() << ", "
      << numVectorsFound << " out of "
      << theNewTypE.theRank << ", desired length squared= " << desiredLengthSquared << " ."
      << " Current length is " << this->theSl2s[i].LengthHsquared;
      if (this->theSl2s[i].LengthHsquared!=desiredLengthSquared)
        reportStreamX << " which is no good.<br> ";
      else
        reportStreamX << " which is all nice and dandy.<br>";
      theReport1.Report(reportStreamX.str());
//      std::cout << "<br>" << reportStreamX.str();
    }
    if (this->theSl2s[i].LengthHsquared!=desiredLengthSquared)
      continue;
    startingVector[0]=this->theSl2s[i].theH.GetCartanPart();
    std::stringstream out;
    if (theGlobalVariables!=0)
    { out << "Generating orbit of " << startingVector[0].ToString() << "...";
      theReport.Report(out.str());
    }
    if (indexCurrentWeight!=0)
    { if (! this->GetSSowner().theWeyl.GenerateOrbit
        (startingVector, false, theOrbit, false, 1152, &theOrbitGenerators, 100000))
      { std::cout
        << "Failed to generate weight orbit: orbit has more than hard-coded limit of 100000 elements. "
        << " This is not a programming error, but I am crashing in flames to let you know "
        << " you hit the computational limits. You might wanna work on improving the algorithm "
        << " for generating semisimple subalgebras. "
        << " Here is a stack trace for you. "
        << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
        assert(false);
      }
    }
    else
    { theOrbit=startingVector;
      ElementWeylGroup theId;
      theOrbitGenerators.Clear();
      theOrbitGenerators.AddOnTop(theId);
    }
    if (theGlobalVariables!=0)
    { out << " done. The size of the orbit is " << theOrbit.size;
      theReport.Report(out.str());
    }
    for (int j=0; j<theOrbit.size; j++)
    { std::stringstream out2;
      if (theGlobalVariables!=0)
      { out2 << "Exploring orbit element of index  " << j+1 << " out of "
        << theOrbit.size << ".";
        theReport2.Report(out2.str());
      }
      Hrescaled=theOrbit[j];
      Hrescaled*=theNewTypE.GetDefaultRootLengthSquared(numVectorsFound);
      Hrescaled/=2;
      if (baseCandidate.isGoodForTheTop(this->GetSSowner().theWeyl, Hrescaled))
      { if (theGlobalVariables!=0)
        { out2 << " the candidate is good. Attempting to extend it by recursion. ";
          theReport2.Report(out2.str());
        }
        //std::cout << "Is good!";
        newCandidate=baseCandidate;
        newCandidate.AddHincomplete(Hrescaled, theOrbitGenerators[j], i);
        this->ExtendOneComponentRecursive(newCandidate, propagateRecursion, theGlobalVariables);
      } else
        if (theGlobalVariables!=0)
        { out2 << " the candidate is no good. ";
          theReport2.Report(out2.str());
//            std::cout << "No good: " << baseCandidate.theTypes.ToString() << ", "
//            << baseCandidate.CartanSAsByComponent.LastObject()->ToString()
//            << " extra weight  " << Hrescaled.ToString()
//            << "<br>";
        }
    }
  }
}

bool CandidateSSSubalgebra::CheckInitialization()const
{ if (this->owner==0)
  { std::cout << "This is a programming error: use of non-initialized semisimple subalgebra "
    << " candidate. " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }

  return true;
}

WeylGroup& CandidateSSSubalgebra::GetAmbientWeyl()
{ this->CheckInitialization();
  return this->owner->GetSSowner().theWeyl;
}

SemisimpleLieAlgebra& CandidateSSSubalgebra::GetAmbientSS()
{ this->CheckInitialization();
  return this->owner->GetSSowner();
}

void CandidateSSSubalgebra::AddHincomplete
  (const Vector<Rational>& theH, const ElementWeylGroup& theWE, int indexOfOrbit)
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
  this->theHorbitIndices.LastObject()->AddOnTop(indexOfOrbit);
//  this->theHWeylGroupElts.LastObject()->AddOnTop(theWE);
  this->theInvolvedPosGenerators.LastObject()->SetSize
  (this->theInvolvedPosGenerators.LastObject()->size+1);
  this->theInvolvedNegGenerators.LastObject()->SetSize
  (this->theInvolvedNegGenerators.LastObject()->size+1);
}

void CandidateSSSubalgebra::AddTypeIncomplete(const DynkinSimpleType& theNewType)
{ if (theNewType.theRank<=0)
  { std::cout << "This is a programming error: I am given a simple Dynkin type of non-positive "
    << " rank. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  WeylGroup tempWeyl, tempWeylnonScaled;
  Rational two=2;
  tempWeyl.MakeArbitrarySimple(theNewType.theLetter, theNewType.theRank, &theNewType.lengthFirstCoRootSquared);
  tempWeylnonScaled.MakeArbitrarySimple(theNewType.theLetter, theNewType.theRank);
  this->theWeylNonEmbeddeD+=tempWeyl;
  this->theWeylNonEmbeddeDdefaultScale+=tempWeylnonScaled;

  this->CartanSAsByComponent.SetSize(this->CartanSAsByComponent.size+1);
  this->CartanSAsByComponent.LastObject()->size=0;
  this->theHorbitIndices.SetSize(this->theHorbitIndices.size+1);
  this->theHorbitIndices.LastObject()->size=0;
//  this->theHWeylGroupElts.SetSize(this->theHWeylGroupElts.size+1);
//  this->theHWeylGroupElts.LastObject()->size=0;
  this->theInvolvedNegGenerators.SetSize(this->theInvolvedNegGenerators.size+1);
  this->theInvolvedNegGenerators.LastObject()->size=0;
  this->theInvolvedPosGenerators.SetSize(this->theInvolvedPosGenerators.size+1);
  this->theInvolvedPosGenerators.LastObject()->size=0;
}

bool CandidateSSSubalgebra::isGoodForTheTop
  (WeylGroup& ownerWeyl, const Vector<Rational>& HneW)const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::isGoodForTheTop");
  Rational theScalarProd;
  int counter=-1;
  int indexHnewInSmallType=this->CartanSAsByComponent.LastObject()->size;
  DynkinSimpleType currentType=this->theWeylNonEmbeddeD.theDynkinType.GetGreatestSimpleType();
  int indexHnew=
  this->theWeylNonEmbeddeD.CartanSymmetric.NumRows-currentType.theRank+indexHnewInSmallType;

  for (int k=0; k<this->CartanSAsByComponent.size; k++)
    for (int l=0; l<this->CartanSAsByComponent[k].size; l++)
    { counter++;
      theScalarProd= ownerWeyl.RootScalarCartanRoot
      (HneW, this->CartanSAsByComponent[k][l]);
      if (theScalarProd!=this->theWeylNonEmbeddeD.CartanSymmetric(indexHnew, counter))
        return false;
    }
  for (int i=0; i<this->PosRootsPerpendicularPrecedingWeights.size; i++)
    if (ownerWeyl.RootScalarCartanRoot
        (HneW, this->PosRootsPerpendicularPrecedingWeights[i])<0)
      return false;
  for (int i=0; i<this->CartanSAsByComponent.size; i++)
    if (this->CartanSAsByComponent[i].Contains(HneW))
    { std::cout << "This is a programming error: I am told that "
      << HneW.ToString() << " is an OK weight to extend the weight subsystem, "
      << " but the weight subsystem contains that weight already: "
      << this->CartanSAsByComponent[i].ToString()
      << ". "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
  Vectors<Rational> tempVectors;
  tempVectors.AssignListList(this->CartanSAsByComponent);
  tempVectors.AddOnTop(HneW);
  if (tempVectors.GetRankOfSpanOfElements()<tempVectors.size)
    return false;
  if (indexHnew==0)
    return ownerWeyl.IsDominantWeight(HneW);
  return true;
}

template <class CoefficientType>
int charSSAlgMod<CoefficientType>::GetIndexExtremeWeightRelativeToWeyl
(WeylGroup& theWeyl)const
{ HashedList<Vector<CoefficientType> > weightsSimpleCoords;
  weightsSimpleCoords.SetExpectedSize(this->size);
  for (int i=0; i<this->size; i++)
    weightsSimpleCoords.AddOnTop
    (theWeyl.GetSimpleCoordinatesFromFundamental((*this)[i].weightFundamentalCoords));
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

bool CandidateSSSubalgebra::IsWeightSystemSpaceIndex
(int theIndex, const Vector<Rational>& AmbientRootTestedForWeightSpace)
{ //std::cout << "<br>h's: " << this->theHs << " testing: "
  //<< AmbientRootTestedForWeightSpace << " for being e-possibility for "
  //<< this->theHs[theIndex];
  for (int k=0; k<this->theHs.size; k++)
  { Rational desiredScalarProd=
    this->theWeylNonEmbeddeDdefaultScale.CartanSymmetric(theIndex, k);
//    if (AmbientRootTestedForWeightSpace.ToString()=="(1, 1, 2)")
    /*{ std::cout << "<br>Desired scalar product of "
      << AmbientRootTestedForWeightSpace.ToString() << " and "
      << this->theHs[k].ToString() << ", determined by indices "
      << k << " and " << theIndex << ", is " << desiredScalarProd
      << " = "
      << this->GetAmbientWeyl().RootScalarCartanRoot(this->theHs[theIndex], this->theHs[k])
      << "*2/"
      << this->GetAmbientWeyl().RootScalarCartanRoot(this->theHs[theIndex], this->theHs[theIndex]);
    }*/
    Rational actualScalar=
    this->GetAmbientWeyl().RootScalarCartanRoot(this->theHs[k], AmbientRootTestedForWeightSpace);
    if (desiredScalarProd!=actualScalar)
    { //if (AmbientRootTestedForWeightSpace.ToString()=="(1, 1, 2)")
      //{ std::cout << ", instead got " << actualScalar;
      //}
      return false;
    }
  }
  return true;
}

bool CandidateSSSubalgebra::ComputeSystem
(GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputeSystem");
  ChevalleyGenerator currentGen, currentOpGen;
  this->theHs.AssignListList(this->CartanSAsByComponent);
  this->theCoRoots.SetSize(this->theHs.size);
  int counter=-1;
  DynkinType dynkinTypeDefaultLengths;
  this->theWeylNonEmbeddeD.theDynkinType.GetDynkinTypeWithDefaultLengths(dynkinTypeDefaultLengths);
  this->theWeylNonEmbeddeDdefaultScale.MakeFromDynkinType(dynkinTypeDefaultLengths);
  this->theWeylNonEmbeddeDdefaultScale.ComputeRho(true);
  List<DynkinSimpleType> theTypes;
  this->theWeylNonEmbeddeD.theDynkinType.GetTypesWithMults(theTypes);
  for (int i=0; i<this->CartanSAsByComponent.size; i++)
    for (int j=0; j< this->CartanSAsByComponent[i].size; j++)
    { counter++;
      this->theCoRoots[counter]=
      (this->theHs[counter]/theTypes[i].GetDefaultRootLengthSquared(j))*2;
    }
  this->theInvolvedNegGenerators.SetSize(this->theCoRoots.size);
  this->theInvolvedPosGenerators.SetSize(this->theCoRoots.size);
  for (int i=0; i<this->theCoRoots.size; i++)
  { List<ChevalleyGenerator>& currentInvolvedPosGens= this->theInvolvedPosGenerators[i];
    List<ChevalleyGenerator>& currentInvolvedNegGens= this->theInvolvedNegGenerators[i];
    currentInvolvedNegGens.SetExpectedSize(this->GetAmbientWeyl().GetDim()*2);
    currentInvolvedPosGens.SetExpectedSize(this->GetAmbientWeyl().GetDim()*2);
    currentInvolvedNegGens.SetSize(0);
    currentInvolvedPosGens.SetSize(0);
    for (int j=0; j<this->GetAmbientWeyl().RootSystem.size; j++)
    { Vector<Rational>& currentAmbRoot=this->GetAmbientWeyl().RootSystem[j];
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
      return false;
  }
  return this->ComputeSystemPart2(theGlobalVariables);
}

bool CandidateSSSubalgebra::ComputeSystemPart2
(GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputeSystemPart2");
  theSystemToSolve.SetSize(0);
  ElementSemisimpleLieAlgebra<Polynomial<Rational> >
  lieBracketMinusGoalValue, goalValue;
  Vector<Polynomial<Rational> > desiredHpart;
  this->CheckInitialization();
//  if (this->indexInOwnersOfNonEmbeddedMe<0 || this->indexInOwnersOfNonEmbeddedMe >=this->owner->theSubalgebrasNonEmbedded
  SemisimpleLieAlgebra& nonEmbeddedMe=
  this->owner->theSubalgebrasNonEmbedded[this->indexInOwnersOfNonEmbeddedMe];
  this->totalNumUnknowns=0;
  if (this->theHs.size==0)
  { std::cout << "This is a programming error: the number of involved H's cannot be zero. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  if (this->theInvolvedNegGenerators.size!=this->theHs.size)
  { std::cout << "This is a programming error: the number of involved negative generators, which is "
    << this->theInvolvedNegGenerators.size << " is not equal to the subalgebra rank, which is "
    << this->theHs.size << ". " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  for (int i=0; i<this->theInvolvedNegGenerators.size; i++)
    this->totalNumUnknowns+=this->theInvolvedNegGenerators[i].size;
  if (this->theWeylNonEmbeddeD.RootSystem.size==0)
  { std::cout << "This is a programming error: the root system of the "
    << " candidate subalgebra has not been computed "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  this->totalNumUnknowns*=2;
  this->theUnknownNegGens.SetSize(this->theInvolvedNegGenerators.size);
  this->theUnknownPosGens.SetSize(this->theInvolvedPosGenerators.size);
  for (int i=0; i<this->theInvolvedNegGenerators.size; i++)
  { this->GetGenericNegGenLinearCombination(i, this->theUnknownNegGens[i]);
    this->GetGenericPosGenLinearCombination(i, this->theUnknownPosGens[i]);
    //std::cout << "<hr>Unknown generator index " << i << ": " << this->theUnknownNegGens[i].ToString();
  }
  for (int i=0; i<this->theInvolvedNegGenerators.size; i++)
  { desiredHpart=this->theCoRoots[i];//<-implicit type conversion here!
    goalValue.MakeHgenerator(desiredHpart, *this->owner->owneR);
    this->GetAmbientSS().LieBracket
    (this->theUnknownPosGens[i], this->theUnknownNegGens[i], lieBracketMinusGoalValue);
    //std::cout << "<hr>[" << this->theUnknownPosGens[i].ToString() << ", "
    //<< this->theUnknownNegGens[i].ToString() << "] = " << lieBracketMinusGoalValue.ToString();
    lieBracketMinusGoalValue-=goalValue;
    this->AddToSystem(lieBracketMinusGoalValue);
    for (int j=0; j<this->theInvolvedPosGenerators.size; j++)
      if (i!=j)
      { this->GetAmbientSS().LieBracket
        (this->theUnknownNegGens[i], this->theUnknownPosGens[j], lieBracketMinusGoalValue);
        //std::cout << "<hr>[" << this->theUnknownNegGens[i].ToString() << ", "
        //<< this->theUnknownPosGens[j].ToString() << "] = " << lieBracketMinusGoalValue.ToString();
        this->AddToSystem(lieBracketMinusGoalValue);

        Vector<Rational> posRoot1, posRoot2;
        posRoot1.MakeEi(this->theWeylNonEmbeddeD.GetDim(), i);
        posRoot2.MakeEi(this->theWeylNonEmbeddeD.GetDim(), j);
        int q;
        if (!nonEmbeddedMe.GetMaxQForWhichBetaMinusQAlphaIsARoot(posRoot1, -posRoot2, q))
        { std::cout << "This is a programming error: the alpha-string along "
          << posRoot1.ToString() << " through " << (-posRoot2).ToString()
          << " does not contain any root, which is impossible. "
          << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
          assert(false);
        }
        lieBracketMinusGoalValue=this->theUnknownPosGens[j];
        for (int k=0; k<q+1; k++)
          this->GetAmbientSS().LieBracket
          (this->theUnknownNegGens[i], lieBracketMinusGoalValue, lieBracketMinusGoalValue);
        //std::cout << "<hr>adjoint element, with power " << q+1 << ": " << lieBracketMinusGoalValue;
        this->AddToSystem(lieBracketMinusGoalValue);
      }
  }
  this->SolveSeparableQuadraticSystemRecursively(theGlobalVariables);
  this->theBasis=this->theNegGens;
  this->theBasis.AddListOnTop(this->thePosGens);
  if (this->theBasis.size>0)
    this->owner->owneR->GenerateLieSubalgebra(this->theBasis);
  return !this->flagSystemProvedToHaveNoSolution;
}

void SemisimpleSubalgebras::reset()
{ this->owneR=0;
  this->theRecursionCounter=0;
  this->theSl2s.owner=0;
  this->flagAttemptToSolveSystems=true;
}


bool CandidateSSSubalgebra::SolveSeparableQuadraticSystemRecursively
(GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::AttemptToSolveSystem");
  this->CheckInitialization();
  this->flagSystemSolved=false;
  this->flagSystemProvedToHaveNoSolution=false;
  this->transformedSystem=this->theSystemToSolve;
  if (this->theNegGens.size!=0)
  { this->flagSystemSolved=true;
    return true;
  }
  GroebnerBasisComputation<Rational> theComputation;
  theComputation.MaxNumComputations=10000;
  if (!this->owner->flagAttemptToSolveSystems)
    return !this->flagSystemProvedToHaveNoSolution;
  this->flagSystemGroebnerBasisFound=
  theComputation.TransformToReducedGroebnerBasis
  (this->transformedSystem, theGlobalVariables);
  if (theComputation.theBasiS.size==1)
    if (theComputation.theBasiS[0].IsAConstant())
      if (!theComputation.theBasiS[0].IsEqualToZero())
        this->flagSystemProvedToHaveNoSolution=true;
  return !this->flagSystemProvedToHaveNoSolution;
}

void CandidateSSSubalgebra::GetGenericNegGenLinearCombination
  (int indexNegGens, ElementSemisimpleLieAlgebra<Polynomial<Rational> >& output)
{/* if (indexNegGens==0)
  { Polynomial<Rational> tempP;
    slTwoSubalgebra& curSl2=this->owner->theSl2s[this->theHorbitIndices[0][0]];
    output.MakeZero(*this->owner->owners, this->owner->indexInOwners);
    for (int j=0; j<curSl2.theF.size; j++)
    { tempP.MakeConst(this->totalNumUnknowns, curSl2.theF.theCoeffs[j]);
      output.AddMonomial(curSl2.theF[j], tempP);
    }
    return;
  }*/
  int offsetIndex=0;
  for (int i=0; i<indexNegGens; i++)
    offsetIndex+=this->theInvolvedNegGenerators[i].size;
  this->GetGenericLinearCombination
  (this->totalNumUnknowns, offsetIndex, this->theInvolvedNegGenerators[indexNegGens], output);
}

void CandidateSSSubalgebra::GetGenericPosGenLinearCombination
  (int indexPosGens, ElementSemisimpleLieAlgebra<Polynomial<Rational> >& output)
{ /*if (indexPosGens==0)
  { Polynomial<Rational> tempP;
    slTwoSubalgebra& curSl2=this->owner->theSl2s[this->theHorbitIndices[0][0]];
    output.MakeZero(*this->owner->owners, this->owner->indexInOwners);
    for (int j=0; j<curSl2.theE.size; j++)
    { tempP.MakeConst(this->totalNumUnknowns, curSl2.theE.theCoeffs[j]);
      output.AddMonomial(curSl2.theE[j], tempP);
    }
    return;
  }*/
  int offsetIndex=0;
  for (int i=0; i<indexPosGens; i++)
    offsetIndex+=this->theInvolvedPosGenerators[i].size;
  offsetIndex+=this->totalNumUnknowns/2;
  this->GetGenericLinearCombination
  (this->totalNumUnknowns, offsetIndex, this->theInvolvedPosGenerators[indexPosGens], output);
}

void CandidateSSSubalgebra::AddToSystem
  (const ElementSemisimpleLieAlgebra<Polynomial<Rational> >& elementThatMustVanish)
{ Polynomial<Rational> thePoly;
//  std::cout << "<hr>I must vanish: " << elementThatMustVanish.ToString();
  for (int i=0; i<elementThatMustVanish.size; i++)
  { thePoly=elementThatMustVanish.theCoeffs[i];
    thePoly.ScaleToIntegralMinHeightFirstCoeffPosReturnsWhatIWasMultipliedBy();
    this->theSystemToSolve.AddOnTopNoRepetition(thePoly);
  }
}

void CandidateSSSubalgebra::GetGenericLinearCombination
(int numVars, int varOffset, List<ChevalleyGenerator>& involvedGens,
 ElementSemisimpleLieAlgebra<Polynomial<Rational> >& output)
{ Polynomial<Rational> theCF;
  ElementSemisimpleLieAlgebra<Polynomial<Rational> > tempMon;
  output.MakeZero(*this->owner->owneR);
  for (int i=0; i<involvedGens.size; i++)
  { theCF.MakeDegreeOne(numVars, varOffset+i, 1);
    tempMon.MakeGenerator(involvedGens[i].theGeneratorIndex, *this->owner->owneR)
    ;
    tempMon*=theCF;
    output+=tempMon;
  }
}

bool CandidateSSSubalgebra::ComputeChar
(GlobalVariables* theGlobalVariables)
{ if (this->indexInOwnersOfNonEmbeddedMe==-1)
  { std::cout << "This is a programming error: attempting to compute char "
    << "of candidate subalgebra that has not been initialized properly. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  MacroRegisterFunctionWithName("CandidateSSSubalgebra::ComputeChar");
  this->CheckInitialization();
  this->theWeylNonEmbeddeD.ComputeRho(true);
  MonomialChar<Rational> tempMon;
  tempMon.weightFundamentalCoords.MakeZero(this->theWeylNonEmbeddeD.GetDim());
  this->theCharFundamentalCoordsRelativeToCartan.Reset();
  this->theCharFundamentalCoordsRelativeToCartan.AddMonomial
  (tempMon, this->GetAmbientSS().GetRank());
  List<DynkinSimpleType> theTypes;
  this->theWeylNonEmbeddeD.theDynkinType.GetTypesWithMults(theTypes);
  for (int k=0; k<this->GetAmbientWeyl().RootSystem.size; k++)
  { int counter=-1;
    for (int i=0; i<this->CartanSAsByComponent.size; i++)
      for (int j=0; j<this->CartanSAsByComponent[i].size; j++)
      { counter++;
        tempMon.weightFundamentalCoords[counter]=
        this->GetAmbientWeyl().RootScalarCartanRoot
        (this->GetAmbientWeyl().RootSystem[k], this->CartanSAsByComponent[i][j])
        /theTypes[i].GetDefaultRootLengthSquared(j)*2
        ;
      }
    this->theCharFundamentalCoordsRelativeToCartan.AddMonomial(tempMon, 1);
  }
  charSSAlgMod<Rational> accumChar, freudenthalChar, outputChar;
  accumChar=this->theCharFundamentalCoordsRelativeToCartan;

  this->theCharFundCoords.MakeZero
  (this->owner->theSubalgebrasNonEmbedded[this->indexInOwnersOfNonEmbeddedMe])
  ;
  while (accumChar.size>0)
  { int currentIndex=
    accumChar.GetIndexExtremeWeightRelativeToWeyl(this->theWeylNonEmbeddeD);
    if (currentIndex==-1)
    { std::cout << "This is a programming error: while decomposing ambient Lie algebra "
      << "over the candidate subalgebra, I got that there is no extreme weight. This is "
      << " impossible: something has gone very wrong. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    if (accumChar.theCoeffs[currentIndex]<0)
      return false;
    for (int i=0; i<accumChar[currentIndex].weightFundamentalCoords.size; i++)
      if (accumChar[currentIndex].weightFundamentalCoords[i]<0)
        return false;
    freudenthalChar.MakeZero
    (this->owner->theSubalgebrasNonEmbedded[this->indexInOwnersOfNonEmbeddedMe]);
    freudenthalChar.AddMonomial(accumChar[currentIndex], accumChar.theCoeffs[currentIndex]);
    this->theCharFundCoords.AddMonomial(accumChar[currentIndex], accumChar.theCoeffs[currentIndex]);
    std::string tempS;
    bool tempBool=freudenthalChar.FreudenthalEvalMeFullCharacter
    (outputChar, -1, &tempS, theGlobalVariables);
    if (!tempBool)
    { std::cout << "This is a programming error: failed to evaluate full character via "
      << " the Freudenthal formula on "
      << " a relatively small example, namely " << freudenthalChar.ToString()
      << ". The failure message was: " << tempS
      << ". This shouldn't happen. "
      <<  CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    accumChar-=outputChar;
  }
  return true;
}

void SemisimpleSubalgebras::ExtendOneComponentOneTypeAllLengthsRecursive
(const CandidateSSSubalgebra& baseCandidate, DynkinSimpleType& theType, bool propagateRecursion,
 GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("SemisimpleSubalgebras::ExtendOneComponentOneTypeAllLengthsRecursive");
  baseCandidate.CheckInitialization();
  CandidateSSSubalgebra theCandidate;
  SemisimpleLieAlgebra tempAlgebra;
  ProgressReport theProgressReport1(theGlobalVariables);
  ProgressReport theProgressReport2(theGlobalVariables);
  ProgressReport theProgressReport3(theGlobalVariables);
  int currentRank=baseCandidate.theWeylNonEmbeddeD.CartanSymmetric.NumRows;
  MonomialChar<Rational> tempMon;
  tempMon.weightFundamentalCoords.MakeZero(currentRank);
  Rational dimCentralizerBase=this->GetSSowner().GetNumGenerators();
  if (!baseCandidate.theCharFundCoords.Contains(tempMon))
  { if (currentRank>0)
      return;
  } else
    dimCentralizerBase=
    baseCandidate.theCharFundCoords.theCoeffs[baseCandidate.theCharFundCoords.GetIndex(tempMon)];
  Rational baseLength=-1;
  List<DynkinSimpleType> theTypes;
  baseCandidate.theWeylNonEmbeddeD.theDynkinType.GetTypesWithMults(theTypes);
  if (theTypes.size!=0)
    if (theType.theRank==theTypes.LastObject()->theRank &&
        theType.theLetter==theTypes.LastObject()->theLetter)
      baseLength=theTypes.LastObject()->lengthFirstCoRootSquared;
  //if (this->GetSSowner().GetRank()-(+)
  std::stringstream consideringStream;
  consideringStream << "\nThe centralizer dimension is " << dimCentralizerBase;
  consideringStream << "<br>\nCurrent rank: " << currentRank
  << "<br>\nConsidering: " << theType.ToString();
//  std::cout << "<br>Considering: " << theType.ToString();
  theProgressReport1.Report(consideringStream.str());
  if (currentRank!=0 && dimCentralizerBase < theType.GetSSAlgDim())
  { consideringStream << " turns out to be no good as the desired root system size is "
    << theType.GetRootSystemSize() << " but I have only "
    << 2*baseCandidate.PosRootsPerpendicularPrecedingWeights.size
    << " weights to use. ";
    theProgressReport1.Report(consideringStream.str());
    return;
  }
  if (theType.theRank+ baseCandidate.theWeylNonEmbeddeD.GetDim()==this->owneR->GetRank())
    for (int i=0; i<theTypes.size; i++)
      if (!this->owneR->theWeyl.theDynkinType.IsPossibleCoRootLength
          (theTypes[i].lengthFirstCoRootSquared))
        return;
  for (int k=0; k<this->theSl2s.size; k++)
  { theType.lengthFirstCoRootSquared=this->theSl2s[k].LengthHsquared;
    if (theType.theRank+ baseCandidate.theWeylNonEmbeddeD.GetDim()==this->owneR->GetRank())
      if (!this->owneR->theWeyl.theDynkinType.IsPossibleCoRootLength(theType.lengthFirstCoRootSquared))
        continue;
    if (theType.lengthFirstCoRootSquared<baseLength)
      continue;
    theCandidate=baseCandidate;
    tempAlgebra.theWeyl.MakeArbitrarySimple(theType.theLetter, theType.theRank);
    int indexSubalgebra=this->SimpleComponentsSubalgebras.GetIndex(tempAlgebra);
    bool mustComputeSSalgebra=(indexSubalgebra==-1);
    if (mustComputeSSalgebra)
    { indexSubalgebra=this->SimpleComponentsSubalgebras.size;
      this->SimpleComponentsSubalgebras.AddOnTop(tempAlgebra);
      this->theSl2sOfSubalgebras.SetSize(this->theSl2sOfSubalgebras.size+1);
    }
    SemisimpleLieAlgebra& theSmallAlgebra=this->SimpleComponentsSubalgebras[indexSubalgebra];
    SltwoSubalgebras& theSmallSl2s=this->theSl2sOfSubalgebras[indexSubalgebra];
    if (mustComputeSSalgebra)
    { std::stringstream tempStream;
      tempStream << "\nGenerating simple Lie algebra "
      << theType.ToString()
      << " (total " << this->SimpleComponentsSubalgebras.size << ")...";
      theProgressReport2.Report(tempStream.str());
      theSmallAlgebra.ComputeChevalleyConstantS(theGlobalVariables);
      theSmallAlgebra.FindSl2Subalgebras
      (theSmallAlgebra, theSmallSl2s, *theGlobalVariables);
      tempStream << " done.";
      theProgressReport2.Report(tempStream.str());
    }
    bool isGood=true;
    for (int i=0; i<theSmallSl2s.size; i++)
    { isGood=false;
      for (int j=0; j<this->theSl2s.size; j++)
      { if (!theSmallSl2s[i].ModuleDecompositionFitsInto(this->theSl2s[j]))
          continue;
        isGood=true;
        break;
      }
      if (!isGood)
        break;
    }
    if (theGlobalVariables!=0)
    { std::stringstream out;
      out << " \n" << theType.ToString();
      if (isGood)
      { out << "<br>" << theType.ToString() << " has fitting sl(2) decompositions.";
        //std::cout  << "<br>" << theType.ToString() << " has fitting sl(2) decompositions.";
      } else
      { out << "<br>" << theType.ToString() << " does not have fitting sl(2) decompositions.";
        //std::cout  << "<br>" << theType.ToString() << " does not have fitting sl(2) decompositions.";
      }
      theProgressReport3.Report(out.str());
    }
    if (isGood)
    { theCandidate.AddTypeIncomplete(theType);
      this->ExtendOneComponentRecursive(theCandidate, propagateRecursion, theGlobalVariables);
    }
  }
}

void SemisimpleSubalgebras::ExtendCandidatesRecursive
  (const CandidateSSSubalgebra& baseCandidate, bool propagateRecursion,
   GlobalVariables* theGlobalVariables)
{ RecursionDepthCounter theCounter(&this->theRecursionCounter);
  MacroRegisterFunctionWithName("SemisimpleSubalgebras::ExtendCandidatesRecursive");
  baseCandidate.CheckInitialization();
  DynkinSimpleType theType;
  ProgressReport theProgressReport1(theGlobalVariables);
  DynkinSimpleType myType;
  myType.theLetter='F';
  myType.theRank=this->GetSSowner().GetRank();
  myType.lengthFirstCoRootSquared=this->theSl2s[0].LengthHsquared;
  if(theGlobalVariables!=0)
  { std::stringstream out;
    out << "\nExploring extensions of " << baseCandidate.ToString();
    theProgressReport1.Report(out.str());
    //std::cout << "Exploring extensions of " << baseCandidate.ToString() << " by: ";
  }
  List<DynkinSimpleType> theTypes;
  baseCandidate.theWeylNonEmbeddeD.theDynkinType.GetTypesWithMults(theTypes);
  if (theTypes.size==0)
    theType.MakeAone();
  else
    theType=*theTypes.LastObject();
  for (; theType<myType; theType++, theType.lengthFirstCoRootSquared=-1)
    this->ExtendOneComponentOneTypeAllLengthsRecursive
      (baseCandidate, theType, propagateRecursion, theGlobalVariables);
}

void slTwoSubalgebra::ElementToStringModuleDecompositionMinimalContainingRegularSAs
(bool useLatex, bool useHtml, SltwoSubalgebras& owner, std::string& output)
{ std::stringstream out;
  std::string tempS;
  if (useLatex)
    out << "$";
  if (useHtml)
  { out << "<table><tr><td align=\"center\">Char.</td>";
    for (int i=0; i<this->IndicesMinimalContainingRootSA.size; i++)
    { rootSubalgebra& theSA= owner.theRootSAs[this->IndicesMinimalContainingRootSA[i]];
      out << "<td align=\"center\">Decomp. " << theSA.theDynkinDiagram.ToString() << "</td>";
    }
    out << "</tr>\n";
  }
  out << "<tr><td align=\"center\"> " << this->hCharacteristic.ToString() << "</td>";
  for (int k=0; k<this->IndicesMinimalContainingRootSA.size; k++)
  { rootSubalgebra& theSA= owner.theRootSAs[this->IndicesMinimalContainingRootSA[k]];
    tempS=theSA.theDynkinDiagram.ToString();
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
(const std::string& formulaTex, std::stringstream& output, bool usePNG, bool useHtml,
 SltwoSubalgebras& container, std::string* physicalPath, std::string* htmlPathServer)
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
  container.texFileNamesForPNG.TheObjects[container.texFileNamesForPNG.size-1]=tempStream.str();
  output << "<img src=\"" << (*htmlPathServer) << "fla" << container.texFileNamesForPNG.size
  << ".png\">";
  if (useHtml)
    output << "\n<br>\n";
}

WeylGroup& slTwoSubalgebra::GetOwnerWeyl()
{ return this->GetOwnerSSAlgebra().theWeyl;
}

bool slTwoSubalgebra::operator==(const slTwoSubalgebra& right)const
{// See Dynkin, Semisimple Lie subalgebras of semisimple Lie algebras, chapter 7-10
  if (this->owneR!=right.owneR)
  { std::cout << "This is a programming error: comparing sl(2) subalgebras"
    << "that have different ambient Lie algebras. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  return this->hCharacteristic==(right.hCharacteristic);
}


std::string slTwoSubalgebra::ToString(FormatExpressions* theFormat)
{ if (this->container==0)
    return "sl(2) subalgebra not initialized.";
  std::stringstream out;  std::string tempS;
  out << "<a name=\"sl2index" << indexInContainer << "\">h-characteristic: "
  << this->hCharacteristic.ToString() << "</a>";
  out << "<br>Length of the weight dual to h: "
  << this->LengthHsquared;
  tempS=this->preferredAmbientSimpleBasis.ToString();
  std::string physicalPath="";
  std::string htmlPathServer="";
  bool usePNG=false;
  bool useHtml=true;
  bool useLatex=false;
  if (theFormat!=0)
    if (theFormat->physicalPath!="")
    { physicalPath=theFormat->physicalPath + "sl2s/";
      htmlPathServer=theFormat->htmlPathServer + "sl2s/";
      usePNG=theFormat->flagUsePNG;
    }
  if (physicalPath=="" || htmlPathServer=="")
  { usePNG=false;
    useHtml=false;
  }
  if (useHtml)
    out << "<br>";
  out << "\nSimple basis ambient algebra w.r.t defining h: " << tempS;
  Matrix<Rational> tempMat;
  if (useHtml)
    out << "<br>";
  if (this->IndicesContainingRootSAs.size>1)
  { out << "Number of containing regular semisimple subalgebras: "
    << this->IndicesContainingRootSAs.size;
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
    rootSubalgebra& currentSA= this->container->theRootSAs[this->IndicesContainingRootSAs[i]];
    if (useHtml)
    { out << "<a href=\"" << htmlPathServer << "../rootHtml_rootSA"
      << this->IndicesContainingRootSAs[i] << ".html\">";
      tempS=currentSA.theDynkinDiagram.ToString();
    }
    out << tempS;
    if (useHtml)
      out << "</a>";
  }
  if (useHtml)
    out << "<br>";
  out << "\nsl(2)-module decomposition of the ambient Lie algebra: ";
  this->ElementToStringModuleDecomposition(useLatex, useHtml, tempS);
  this->ElementToHtmlCreateFormulaOutputReference
  ("$"+tempS+"$", out, usePNG, useHtml, *this->container, &physicalPath, &htmlPathServer);
  if (indexInContainer!=-1)
  { this->container->IndicesSl2decompositionFlas.SetSize(this->container->size);
    this->container->IndicesSl2decompositionFlas[indexInContainer]=
    this->container->texFileNamesForPNG.size-1;
  }
  out << "\nThe below list one possible realization of the sl(2) subalgebra.";
  if (useHtml)
    out << "\n<br>\n";
  std::stringstream tempStreamH, tempStreamE, tempStreamF, tempStreamHE, tempStreamHF, tempStreamEF;
  tempS=this->theH.ToString(theFormat);
  tempStreamH << "\n$h=$ $" << tempS << "$";
  tempS= tempStreamH.str();
  this->ElementToHtmlCreateFormulaOutputReference
  (tempS, out, usePNG, useHtml, *this->container, &physicalPath, &htmlPathServer);
  tempStreamE << "\n$e=$ $" << this->theE.ToString(theFormat) << "$";
  tempS= tempStreamE.str();
  this->ElementToHtmlCreateFormulaOutputReference
  (tempS, out, usePNG, useHtml, *this->container, &physicalPath, &htmlPathServer);
  tempStreamF << "\n$f=$ $" << this->theF.ToString(theFormat) << "$";
  tempS= tempStreamF.str();
  this->ElementToHtmlCreateFormulaOutputReference
  (tempS, out, usePNG, useHtml, *this->container, &physicalPath, &htmlPathServer);
  out << "\n\nThe below are the Lie brackets of the above elements. Printed for debugging.";
  if (useHtml)
    out << "\n<br>\n";
  tempStreamEF << "\n$[e, f]=$ $" <<  this->bufferEbracketF.ToString(theFormat) << "$";
  tempS= tempStreamEF.str();
  this->ElementToHtmlCreateFormulaOutputReference
  (tempS, out, usePNG, useHtml, *this->container, &physicalPath, &htmlPathServer);
  tempStreamHE << "\n$[h, e]=$ $" << this->bufferHbracketE.ToString(theFormat) << "$";
  tempS= tempStreamHE.str();
  this->ElementToHtmlCreateFormulaOutputReference
  (tempS, out, usePNG, useHtml, *this->container, &physicalPath, &htmlPathServer);
  tempStreamHF << "\n$[h, f]=$ $" << this->bufferHbracketF.ToString(theFormat) << "$";
  tempS= tempStreamHF.str();
  this->ElementToHtmlCreateFormulaOutputReference
  (tempS, out, usePNG, useHtml, *this->container, &physicalPath, &htmlPathServer);
  //this->theSystemMatrixForm.ToString(tempS);
  //out <<"\nSystem matrix form we try to solve:\n"<< tempS;
  //this->theSystemColumnVector.ToString(tempS);
  //out <<"\nColumn vector of the system:\n"<<tempS;
  std::stringstream tempStreamActual;
  for (int i=0; i<this->theSystemToBeSolved.size; i++)
  { tempS=this->theSystemToBeSolved[i].ToString(theFormat);
    if (tempS=="")
    { if (useLatex || usePNG)
        tempStreamActual << "~\\\\";
      else
        tempStreamActual << "\n\n";
    }
    else
      tempStreamActual << "\\noindent \\begin{eqnarray*}&& " << tempS << "=0\\end{eqnarray*}\n\n";
  }
  out << "\nThe system we need to solve:\n";
  if (useHtml)
    out << "\n<br>\n";
  tempS= tempStreamActual.str();
  this->ElementToHtmlCreateFormulaOutputReference
  (tempS, out, usePNG, useHtml, *this->container, &physicalPath, &htmlPathServer);
  return out.str();
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

bool slTwoSubalgebra::ModuleDecompositionFitsInto(const slTwoSubalgebra& other)
{ return this->ModuleDecompositionFitsInto(this->highestWeights, this->multiplicitiesHighestWeights, other.highestWeights, other.multiplicitiesHighestWeights);
}

bool slTwoSubalgebra::ModuleDecompositionFitsInto(const List<int>& highestWeightsLeft, const List<int>& multiplicitiesHighestWeightsLeft, const List<int>& highestWeightsRight, const List<int>& multiplicitiesHighestWeightsRight)
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
  CGI::OpenFileCreateIfNotPresent(theFile, filePath, false, true, false);
}

void SltwoSubalgebras::init(SemisimpleLieAlgebra& inputOwner)
{ this->owner=& inputOwner;
  this->theRootSAs.owneR=&inputOwner;
}

void SemisimpleLieAlgebra::FindSl2Subalgebras
(SemisimpleLieAlgebra& inputOwner, SltwoSubalgebras& output, GlobalVariables& theGlobalVariables)
{ MacroRegisterFunctionWithName("SemisimpleLieAlgebra::FindSl2Subalgebras");
  output.init(inputOwner);
  output.GetOwner().ComputeChevalleyConstantS(&theGlobalVariables);
  output.theRootSAs.GenerateAllReductiveRootSubalgebrasUpToIsomorphism
  (theGlobalVariables, true, true);
  //output.theRootSAs.ComputeDebugString(false, false, false, 0, 0, theGlobalVariables);
  output.IndicesSl2sContainedInRootSA.SetSize(output.theRootSAs.size);
  output.IndicesSl2sContainedInRootSA.ReservE(output.theRootSAs.size*2);
  for (int i=0; i<output.IndicesSl2sContainedInRootSA.size; i++)
    output.IndicesSl2sContainedInRootSA[i].size=0;
  ProgressReport theReport(&theGlobalVariables);
  for (int i=0; i<output.theRootSAs.size-1; i++)
  { std::stringstream tempStream;
    tempStream << "\nExploring root subalgebra "
    << output.theRootSAs[i].theDynkinDiagram.ToString() << "(" << (i+1)
    << " out of " << output.theRootSAs.size-1 << " non-trivial)\n";
    theReport.Report(tempStream.str());
    output.theRootSAs[i].GetSsl2SubalgebrasAppendListNoRepetition
    (output, i, theGlobalVariables);
  }
  for (int i=0; i<output.size; i++)
  { slTwoSubalgebra& theSl2= output[i];
    theSl2.IndicesMinimalContainingRootSA.ReservE(theSl2.IndicesContainingRootSAs.size);
    theSl2.IndicesMinimalContainingRootSA.size=0;
    for (int j=0; j<theSl2.IndicesContainingRootSAs.size; j++)
    { bool isMinimalContaining=true;
//      rootSubalgebra& currentRootSA = output.theRootSAs.TheObjects[];
      for (int k=0; k<theSl2.IndicesContainingRootSAs.size; k++)
      { rootSubalgebra& theOtherRootSA =
        output.theRootSAs[theSl2.IndicesContainingRootSAs[k]];
        if (theOtherRootSA.indicesSubalgebrasContainingK.Contains(theSl2.IndicesContainingRootSAs[j]))
        { isMinimalContaining=false;
          break;
        }
      }
      if (isMinimalContaining)
        theSl2.IndicesMinimalContainingRootSA.AddOnTopNoRepetition(theSl2.IndicesContainingRootSAs[j]);
    }
    output.ComputeModuleDecompositionsOfAmbientLieAlgebra(theGlobalVariables);
  }
//  tempRootSA.GetSsl2Subalgebras(tempSl2s, theGlobalVariables, *this);
}

std::string rootSubalgebras::ToString()
{ std::stringstream out;
  for (int i=0; i<this->size; i++)
    out << (*this)[i].ToString();
  return out.str();
}

WeylGroup& rootSubalgebras::GetOwnerWeyl()
{ return this->GetOwnerSSalgebra().theWeyl;
}

SemisimpleLieAlgebra& rootSubalgebras::GetOwnerSSalgebra()
{ if (this->owneR==0)
  { std::cout << "This is a programming error. Attempting to access the "
    << " ambient Lie algebra of a non-initialized collection of root subalgebras. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  return *this->owneR;
}

void rootSubalgebras::GenerateAllReductiveRootSubalgebrasUpToIsomorphism
(GlobalVariables& theGlobalVariables, bool sort, bool computeEpsCoords)
{ MacroRegisterFunctionWithName("rootSubalgebras::GenerateAllReductiveRootSubalgebrasUpToIsomorphism");
  this->size=0;
  this->GetOwnerWeyl().ComputeRho(true);
  //this->initDynkinDiagramsNonDecided(this->AmbientWeyl, WeylLetter, WeylRank);
  rootSubalgebras rootSAsGenerateAll;
  rootSAsGenerateAll.SetSize(this->GetOwnerSSalgebra().GetRank()*2+1);
  rootSAsGenerateAll[0].genK.size=0;
  rootSAsGenerateAll[0].owneR=this->owneR;
  rootSAsGenerateAll[0].ComputeAll();
  this->GenerateAllReductiveRootSubalgebrasContainingInputUpToIsomorphism
  (rootSAsGenerateAll, 1, theGlobalVariables)
  ;
//  std::cout << this->ToString();
  if (sort)
    this->SortDescendingOrderBySSRank();
  if(computeEpsCoords)
    for(int i=0; i<this->size; i++)
      (*this)[i].ComputeEpsCoordsWRTk(theGlobalVariables);
}

void rootSubalgebra::GetSsl2SubalgebrasAppendListNoRepetition
(SltwoSubalgebras& output, int indexInContainer, GlobalVariables& theGlobalVariables)
{ MacroRegisterFunctionWithName("rootSubalgebra::GetSsl2SubalgebrasAppendListNoRepetition");
  //reference: Dynkin, semisimple Lie algebras of simple lie algebras, theorems 10.1-10.4
  Selection theRootsWithZeroCharacteristic;
  Selection simpleRootsChar2;
  Vectors<Rational> RootsWithCharacteristic2;
  Vectors<Rational> reflectedSimpleBasisK;
  RootsWithCharacteristic2.ReservE(this->PositiveRootsK.size);
  ElementWeylGroup raisingElt;
  DynkinDiagramRootSubalgebra tempDiagram;
  int theRelativeDimension= this->SimpleBasisK.size;
  theRootsWithZeroCharacteristic.init(theRelativeDimension);
  Matrix<Rational> InvertedRelativeKillingForm;
  InvertedRelativeKillingForm.init(theRelativeDimension, theRelativeDimension);
  for (int k=0; k<theRelativeDimension; k++)
    for (int j=0; j<theRelativeDimension; j++)
      InvertedRelativeKillingForm.elements[k][j]=this->GetAmbientWeyl().RootScalarCartanRoot
      (this->SimpleBasisK[k], this->SimpleBasisK[j]);
  InvertedRelativeKillingForm.Invert();
  int numCycles= MathRoutines::TwoToTheNth(theRootsWithZeroCharacteristic.MaxSize);
  ProgressReport theReport(&theGlobalVariables);
  Vectors<Rational> tempRoots;
  tempRoots.ReservE(theRootsWithZeroCharacteristic.MaxSize);
  Vectors<Rational> relativeRootSystem, bufferVectors;
  Matrix<Rational> tempMat;
//  Selection tempSel;
  this->PositiveRootsK.GetCoordsInBasis
  (this->SimpleBasisK, relativeRootSystem, bufferVectors, tempMat);
  slTwoSubalgebra theSl2;
  theSl2.container=&output;
  theSl2.owneR=this->owneR;
  SemisimpleLieAlgebra& theLieAlgebra= this->GetOwnerSSalg();
  for (int i=0; i<numCycles; i++, theRootsWithZeroCharacteristic.incrementSelection())
  { tempRoots.size=0;
    for (int j=0; j<theRootsWithZeroCharacteristic.CardinalitySelection; j++)
      tempRoots.AddOnTop(this->SimpleBasisK[theRootsWithZeroCharacteristic.elements[j]]);
    tempDiagram.ComputeDiagramTypeModifyInput(tempRoots, this->GetAmbientWeyl());
    int theSlack=0;
    RootsWithCharacteristic2.size=0;
    simpleRootsChar2=theRootsWithZeroCharacteristic;
    simpleRootsChar2.InvertSelection();
    Vector<Rational> simpleRootsChar2Vect;
    simpleRootsChar2Vect=simpleRootsChar2;
    for (int j=0; j<relativeRootSystem.size; j++)
      if (simpleRootsChar2Vect.ScalarEuclidean(relativeRootSystem[j])==1)
      { theSlack++;
        RootsWithCharacteristic2.AddOnTop(this->PositiveRootsK[j]);
      }
    int theDynkinEpsilon =
    tempDiagram.NumRootsGeneratedByDiagram() + theRelativeDimension - theSlack;
    //if Dynkin's epsilon is not zero the subalgebra cannot be an S sl(2) subalgebra.
    //otherwise, as far as I understand, it always is //
    //except for G_2 (go figure!).
    //(but generatorootsWithZeroCharVectorrs still have to be found)
    //this is done in the below code.
//    std::cout << "Simple basis k: " << this->SimpleBasisK.ToString();
    if (theDynkinEpsilon==0)
    { Vector<Rational> tempRoot, tempRoot2;
      tempRoot.MakeZero(theRelativeDimension);
      for (int k=0; k<theRelativeDimension; k++)
        if(!theRootsWithZeroCharacteristic.selected[k])
          tempRoot[k]=2;
      InvertedRelativeKillingForm.ActOnVectorColumn(tempRoot, tempRoot2);
      Vector<Rational> characteristicH;
      characteristicH.MakeZero(theLieAlgebra.GetRank());
      for(int j=0; j<theRelativeDimension; j++)
        characteristicH+=this->SimpleBasisK[j]*tempRoot2[j];
      this->GetAmbientWeyl().RaiseToDominantWeight(characteristicH, 0, 0, &raisingElt);
      ////////////////////
      reflectedSimpleBasisK=this->SimpleBasisK;
      for (int k=0; k<reflectedSimpleBasisK.size; k++)
        this->GetAmbientWeyl().ActOn(raisingElt, reflectedSimpleBasisK[k], false, false);
      ////////////////////
      theSl2.RootsWithScalar2WithH=RootsWithCharacteristic2;
      for (int k=0; k<theSl2.RootsWithScalar2WithH.size; k++)
        this->GetAmbientWeyl().ActOn(raisingElt, theSl2.RootsWithScalar2WithH[k], false, false);

      theSl2.theH.MakeHgenerator(characteristicH, theLieAlgebra);
      theSl2.theE.MakeZero(theLieAlgebra);
      theSl2.theF.MakeZero(theLieAlgebra);
      //theSl2.ComputeDebugString(false, false, theGlobalVariables);
//      std::cout << "<br>accounting " << characteristicH.ToString();
      if(theLieAlgebra.AttemptExtendingHEtoHEFWRTSubalgebra
         (theSl2.RootsWithScalar2WithH, theRootsWithZeroCharacteristic,
          reflectedSimpleBasisK, characteristicH, theSl2.theE, theSl2.theF,
          theSl2.theSystemMatrixForm, theSl2.theSystemToBeSolved,
          theSl2.theSystemColumnVector, theGlobalVariables))
      { int indexIsoSl2;
        theSl2.MakeReportPrecomputations
        (theGlobalVariables, output, output.size, indexInContainer, *this);
        if(output.ContainsSl2WithGivenHCharacteristic(theSl2.hCharacteristic, &indexIsoSl2))
        { output[indexIsoSl2].IndicesContainingRootSAs.AddOnTop(indexInContainer);
          output.IndicesSl2sContainedInRootSA[indexInContainer].AddOnTop(indexIsoSl2);
        } else
        { output.IndicesSl2sContainedInRootSA[indexInContainer].AddOnTop(output.size);
          theSl2.indexInContainer=output.size;
          output.AddOnTop(theSl2);
        }
      }
      else
      { output.BadHCharacteristics.AddOnTop(characteristicH);
/*        std::cout << "<br>obtained bad characteristic "
        << characteristicH.ToString() << ". The zero diagram is "
        << tempDiagram.ElementToStrinG()
        << "; the Dynkin epsilon is " << theDynkinEpsilon
        << "= the num roots generated by diagram " << tempDiagram.NumRootsGeneratedByDiagram()
        << " + the relative dimension " << theRelativeDimension
        << " - the slack " << theSlack
        << "<br>The relative root system is: "
        << relativeRootSystem.ToString();
        std::cout << "<br> I was exploring " << this->ToString();*/
      }
    }
    std::stringstream out;
    out << "Exploring Dynkin characteristics case " << i+1 << " out of " << numCycles;
//    std::cout << "<br>" << out.str();
    theReport.Report(out.str());
  }
//  std::cout << "Bad chracteristics: " << output.BadHCharacteristics.ToString();
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

bool SltwoSubalgebras::ContainsSl2WithGivenHCharacteristic
(Vector<Rational>& theHCharacteristic, int* outputIndex)
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

void slTwoSubalgebra::ElementToStringModuleDecomposition(bool useLatex, bool useHtml, std::string& output)
{ std::stringstream out;
  for (int i=0; i<this->highestWeights.size; i++)
  { if (this->multiplicitiesHighestWeights[i]>1)
      out << this->multiplicitiesHighestWeights[i];
    out << "V_{" << this->highestWeights[i] << "}";
    if (i!=this->highestWeights.size-1)
      out << "+";
  }
  output=out.str();
}

void slTwoSubalgebra::ComputeModuleDecompositionAmbientLieAlgebra(GlobalVariables& theGlobalVariables)
{ this->ComputeModuleDecomposition
  (this->GetOwnerWeyl().RootsOfBorel, this->GetOwnerWeyl().CartanSymmetric.NumRows,
   this->highestWeights, this->multiplicitiesHighestWeights, this->weightSpaceDimensions,
   theGlobalVariables);
}

void slTwoSubalgebra::ComputeModuleDecompositionOfMinimalContainingRegularSAs(SltwoSubalgebras& owner, int IndexInOwner, GlobalVariables& theGlobalVariables)
{ this->MultiplicitiesDecompositionMinimalContainingRootSA.SetSize
  (this->IndicesMinimalContainingRootSA.size);
  this->HighestWeightsDecompositionMinimalContainingRootSA.SetSize
  (this->IndicesMinimalContainingRootSA.size);
  List<int> buffer;
  for (int i=0; i<this->IndicesMinimalContainingRootSA.size; i++)
  { rootSubalgebra& theSA= owner.theRootSAs[this->IndicesMinimalContainingRootSA[i]];
    this->ComputeModuleDecomposition
    (theSA.PositiveRootsK, theSA.SimpleBasisK.size,
     this->HighestWeightsDecompositionMinimalContainingRootSA[i],
     this->MultiplicitiesDecompositionMinimalContainingRootSA[i], buffer, theGlobalVariables);
  }
}

void slTwoSubalgebra::MakeReportPrecomputations
(GlobalVariables& theGlobalVariables, SltwoSubalgebras& container, int indexInContainer,
 int indexMinimalContainingRegularSA, rootSubalgebra& MinimalContainingRegularSubalgebra)
{ MacroRegisterFunctionWithName("slTwoSubalgebra::MakeReportPrecomputations");
  int theDimension=this->GetOwnerSSAlgebra().GetRank();
  this->IndicesContainingRootSAs.size=0;
  Vectors<Rational> tempRoots;
  tempRoots=(MinimalContainingRegularSubalgebra.SimpleBasisK);
  this->GetOwnerSSAlgebra().theWeyl.TransformToSimpleBasisGeneratorsWRTh
  (tempRoots, this->theH.GetCartanPart());
  DynkinDiagramRootSubalgebra theDiagram;
  theDiagram.ComputeDiagramTypeKeepInput(tempRoots, this->GetOwnerSSAlgebra().theWeyl);
  this->IndicesContainingRootSAs.AddOnTop(indexMinimalContainingRegularSA);
  tempRoots.MakeEiBasis(theDimension);
  this->GetOwnerSSAlgebra().theWeyl.TransformToSimpleBasisGeneratorsWRTh
  (tempRoots, this->theH.GetCartanPart());
  DynkinDiagramRootSubalgebra tempDiagram;
  tempDiagram.ComputeDiagramTypeKeepInput(tempRoots, this->GetOwnerSSAlgebra().theWeyl);
  this->preferredAmbientSimpleBasis=tempRoots;
  this->hCharacteristic.SetSize(theDimension);
  for (int i=0; i<theDimension; i++)
    this->hCharacteristic[i]=
    this->GetOwnerSSAlgebra().theWeyl.RootScalarCartanRoot
    (this->theH.GetCartanPart(), this->preferredAmbientSimpleBasis[i]);
  this->LengthHsquared=
  this->GetOwnerSSAlgebra().theWeyl.RootScalarCartanRoot
  (this->theH.GetCartanPart(), this->theH.GetCartanPart());
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
//  this->ComputeModuleDecomposition();
}

//The below code is related to sl(2) subalgebras of simple Lie algebras
void slTwoSubalgebra::ComputeModuleDecomposition
(Vectors<Rational>& positiveRootsContainingRegularSA, int dimensionContainingRegularSA,
 List<int>& outputHighestWeights, List<int>& outputMultiplicitiesHighestWeights,
 List<int>& outputWeightSpaceDimensions, GlobalVariables& theGlobalVariables)
{ int IndexZeroWeight=positiveRootsContainingRegularSA.size*2;
  outputWeightSpaceDimensions.initFillInObject(4*positiveRootsContainingRegularSA.size+1, 0);
  outputWeightSpaceDimensions[IndexZeroWeight]=dimensionContainingRegularSA;
  List<int> BufferHighestWeights;
  Rational tempRat;
  Vectors<Rational> coordsInPreferredSimpleBasis, tempRoots2;
  Matrix<Rational> tempMat;
  positiveRootsContainingRegularSA.GetCoordsInBasis
  (this->preferredAmbientSimpleBasis, coordsInPreferredSimpleBasis, tempRoots2, tempMat);
  for (int k=0; k<positiveRootsContainingRegularSA.size; k++)
  { tempRat=Vector<Rational>::ScalarEuclidean
    (this->hCharacteristic, coordsInPreferredSimpleBasis[k]);
    assert(tempRat.DenShort==1);
    if (tempRat>positiveRootsContainingRegularSA.size*2)
    { std::cout << "This is a programming error. The scalar product of the h-Characteristic "
      << this->hCharacteristic.ToString()
      << " with the simple root " << coordsInPreferredSimpleBasis[k].ToString()
      << " is larger than " << positiveRootsContainingRegularSA.size*2
      << ". The affected sl(2) subalgebra is " << this->ToString()
      << ". " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
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
    { std::cout << "This is a programming error: "
      << " the sl(2)-module decomposition "
      << " shows an sl(2)-module with highest weight "
      << topMult << " which is impossible. Here is the sl(2) subalgebra. "
      << this->ToString() << "."
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    if (topMult>0)
    { outputHighestWeights.AddOnTop(j-IndexZeroWeight);
      outputMultiplicitiesHighestWeights.AddOnTop(topMult);
      if (j!=IndexZeroWeight)
        BufferHighestWeights[IndexZeroWeight*2-j]-=topMult;
      for (int k=j-2; k>=IndexZeroWeight; k-=2)
      { BufferHighestWeights[k]-=topMult;
        if (k!=IndexZeroWeight)
          BufferHighestWeights[IndexZeroWeight*2-k]-=topMult;
        if(BufferHighestWeights[k]<0 ||
           !(BufferHighestWeights[k]==BufferHighestWeights[IndexZeroWeight*2-k]))
        { std::cout << " This is a programming error: an error check has failed. "
          << " While trying to decompose with respect to  h-characteristic <br> "
          << this->hCharacteristic.ToString()
          << ". The positive root system of the containing root subalgebra is <br>"
          << positiveRootsContainingRegularSA.ToString() << ". "
          << "<br>The preferred simple basis is <br>"
          << this->preferredAmbientSimpleBasis.ToString()
          << ". The coordinates relative to the preferred simple basis are<br>"
          << coordsInPreferredSimpleBasis.ToString()
          << " The starting weights list is <br>"
          << outputWeightSpaceDimensions << ". "
          << " I got that the root space of index  "
          <<  k+1 << " has negative dimension. Something is wrong. "
          << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
          assert(false);
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
  std::string tooltipHchar="Let h be in the Cartan s.a. Let \\alpha_1, ..., \\alpha_n be simple Vectors<Rational> w.r.t. h. Then the h-characteristic is the n-tuple (\\alpha_1(h), ..., \\alpha_n(h))";
  std::string tooltipVDecomposition= "The sl(2) submodules of g are parametrized by their highest weight w.r.t. h. V_l is l+1 dimensional";
  std::string tooltipContainingRegular="A regular semisimple subalgebra might contain an sl(2) such that the sl(2) has no centralizer in the regular semisimple subalgebra, but the regular semisimple subalgebra might fail to be minimal containing. This happens when another minimal containing regular semisimple subalgebra of equal rank nests as a Vector<Rational> subalgebra in the containing SA. See Dynkin, Semisimple Lie subalgebras of semisimple Lie algebras, remark before Theorem 10.4.";
  std::string tooltipHvalue="The actual realization of h. The coordinates of h are given with respect to the fixed original simple basis. Note that the characteristic of h is given *with respect to another basis* (namely, with respect to an h-positive simple basis). I will fix this in the future (email me if you want that done sooner).";
  //if (this->owner==0)

  bool usePNG=theFormat==0? false : theFormat->flagUsePNG;
  bool useHtml=theFormat==0 ? true : theFormat->flagUseHTML;
  bool useLatex=theFormat==0 ? true : theFormat->flagUseLatex;
  std::string physicalPath, displayPath;
  physicalPath=theFormat==0 ? "../" : theFormat->physicalPath;
  displayPath=theFormat==0 ? "../" : theFormat->htmlPathServer;
  out << "Number of sl(2) subalgebras " << this->size << ".\n";
  if (this->IndicesSl2decompositionFlas.size < this->size)
    usePNG = false;
  std::stringstream out2;
  out2
  << "<br> Given a root subsystem P, and a root subsubsystem P_0, in (10.2) "
  << " of Semisimple subalgebras of semisimple Lie algebras, E. Dynkin defines "
  << " a numerical constant e(P, P_0) (which we call Dynkin epsilon). "
  << " <br>In Theorem 10.3, Dynkin proves that if an sl(2) is an S-subalgebra in "
  << " the root subalgebra generated by P, such that it has characteristic 2 "
  << " for all simple roots of P lying in P_0, then e(P, P_0)=0. ";
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
        out << "<b>Bad characteristic: " << this->BadHCharacteristics[i]
        << "</b>";
      }
      else
      { out2
        << "<br><b>It turns out that in the current case of Cartan element h = "
        << this->BadHCharacteristics[i] << " we have that, for a certain P, "
        << " e(P, P_0) equals 0, but I failed to realize the corresponding sl(2) "
        << " as a subalgebra of that P. However, it turns out that h "
        << " is indeed an S-subalgebra of a smaller root subalgebra P'.</b>"
        ;
      }
    }
    if (!allbadAreGoodInTheirBadness)
      out << "<b>found bad characteristics!</b>";
  } else
  { out2 << "It turns out by direct computation that, in the current case of "
    << this->GetOwner().GetLieAlgebraName()
    << ",  e(P,P_0)=0 implies that an S-sl(2) subalgebra of the root subalgebra generated by "
    << " P with characteristic with 2's in the simple roots of P_0 "
    << " always exists. Note that Theorem 10.3 is stated in one "
    << " direction only.";
  }
  if(useHtml)
    out << "<br><br><table><tr><td style=\"padding-right:20px\">"
    << CGI::ElementToStringTooltip("Characteristic", tooltipHchar)
    << "</td><td align=\"center\" title=\"" << tooltipHvalue
    << "\"> h</td><td style=\"padding-left:20px\" title=\""
    << tooltipVDecomposition
    << "\"> Decomposition of ambient Lie algebra</td>"
    << "<td>The square of the length of the weight dual to h.</td>"
    << "<td>Dynkin index = (previous column) *(long root length ambient algebra)^2/4</td>"
    << "<td>Minimal containing regular semisimple SAs</td><td title=\""
    << tooltipContainingRegular << "\">Containing regular semisimple SAs in "
    << "which the sl(2) has no centralizer</td> </tr>";

  for (int i=0; i<this->size; i++)
  { slTwoSubalgebra& theSl2= (*this)[i];
    if (useHtml)
      out << "<tr><td style=\"padding-right:20px\"><a href=\"./sl2s.html#sl2index"
      << i << "\"title=\"" << tooltipHchar << "\" >";
    out << theSl2.hCharacteristic.ToString();
    if (useHtml)
      out << "</a></td><td title=\"" << tooltipHvalue << "\">";
    out << theSl2.theH.GetCartanPart().ToString();
    if(!this->GetOwnerWeyl().IsDominantWeight(theSl2.theH.GetCartanPart()))
      out << "<b>This is wrong!!!!!!!!!!!!! The h is not dual to a dominant weight... </b>";
    if (useHtml)
      out << "</td><td style=\"padding-left:20px\" title=\"" << tooltipVDecomposition << "\">";
    if (useHtml && usePNG)
      out << "<img src=\"./fla"
      << this->IndicesSl2decompositionFlas[i]+1 << ".png\"></td><td>";
    else
    { theSl2.ElementToStringModuleDecomposition(useLatex, useHtml, tempS);
      out << tempS;
      if (useHtml)
        out << "</td><td>";
    }
    out << theSl2.LengthHsquared;
    if (useHtml)
      out << "</td><td>";
    out << theSl2.LengthHsquared * this->GetOwnerWeyl().GetLongestRootLengthSquared()/4;
    if (useHtml)
      out << "</td><td>";
    for (int j=0; j<theSl2.IndicesMinimalContainingRootSA.size; j++)
    { rootSubalgebra& currentSA= this->theRootSAs[theSl2.IndicesMinimalContainingRootSA[j]];
      out << "<a href=\""
      << displayPath
      << "rootHtml_rootSA" << theSl2.IndicesMinimalContainingRootSA[j]
      << ".html\">" << currentSA.theDynkinDiagram.ToString() << "</a>" << ";  ";
    }
    if (useHtml)
      out << "</td><td title=\"" << tooltipContainingRegular << "\">";
    for (int j=0; j<theSl2.IndicesContainingRootSAs.size; j++)
    { rootSubalgebra& currentSA= this->theRootSAs[theSl2.IndicesContainingRootSAs[j]];
      out << "<a href=\""
      <<  displayPath
      << "rootHtml_rootSA" << theSl2.IndicesContainingRootSAs[j] << ".html\">"
      << currentSA.theDynkinDiagram.ToString() << "</a>" << ";  ";
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

void SltwoSubalgebras::ElementToHtml
(FormatExpressions* theFormat, GlobalVariables* theGlobalVariables)
{ std::string physicalPathSAs= theFormat==0 ? "": theFormat->physicalPath;
  std::string htmlPathServerSAs= theFormat==0 ? "": theFormat->htmlPathServer;
  std::string physicalPathSl2s= theFormat==0 ? "": theFormat->physicalPath+"sl2s/";
  std::string htmlPathServerSl2s= theFormat==0 ? "": theFormat->htmlPathServer+"sl2s/";

  ProgressReport theReport(theGlobalVariables);
  theReport.Report("Preparing html pages for sl(2) subalgebras. This might take a while.");
  std::string tempS;
  std::string DisplayNameCalculator= theFormat==0 ? "" : theFormat->DisplayNameCalculator;
  this->theRootSAs.ElementToHtml
  (tempS, physicalPathSAs, htmlPathServerSAs, this, DisplayNameCalculator, theGlobalVariables);
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
  << this->GetOwnerWeyl().theDynkinType.ToString() << ")"
 ;
  outNotation
  << "Notation, structure constants and Weyl group info: "
  << CGI::GetCalculatorLink(DisplayNameCalculator, outNotationCommand.str())
  << "<br> <a href=\""<< DisplayNameCalculator
  << "\"> Calculator main page</a><br><a href=\"../rootHtml.html\">Root subsystem table</a><br>";
  std::string notation= outNotation.str();
  out << this->ToString(theFormat);
  if(usePNG)
  { fileName= physicalPathSl2s;
    fileName.append("sl2s.html");
    CGI::OpenFileCreateIfNotPresent(theFile, fileName, false, true, false);
    tempS= out.str();
    theFile << "<HMTL>"
    << "<title>sl(2)-subalgebras of "
    << this->theRootSAs[0].theDynkinDiagram.ToString() << "</title>";
    theFile << "<meta name=\"keywords\" content=\""
    <<  this->theRootSAs[0].theDynkinDiagram.ToString()
    << " sl(2)-triples, sl(2)-subalgebras, nilpotent orbits simple Lie algebras,"
    << " nilpotent orbits of "
    <<  this->theRootSAs[0].theDynkinDiagram.ToString()
    << ", sl(2)-triples of "
    << this->theRootSAs[0].theDynkinDiagram.ToString()
    << " \">";
    theFile << "<BODY>" << notation << "<a href=\"" << htmlPathServerSl2s
    << "sl2s_nopng.html\"> plain html for your copy+paste convenience</a><br>\n"
    << tempS << "</HTML></BODY>";
    theFile.close();
  }
  fileName= physicalPathSl2s;
  fileName.append("sl2s_nopng.html");
  bool tempB=theFormat->flagUsePNG;
  theFormat->flagUsePNG=false;
  tempS = this->ToString(theFormat);
  theFormat->flagUsePNG=tempB;
  CGI::OpenFileCreateIfNotPresent(theFile, fileName, false, true, false);
  theFile << "<HMTL><BODY>" << notation << "<a href=\""
  << htmlPathServerSl2s << "sl2s.html\"> "
  << ".png rich html for your viewing pleasure</a><br>\n" << tempS << "</HTML></BODY>";
  theFile.close();
  if (usePNG)
  { this->listSystemCommandsLatex.SetSize(this->texFileNamesForPNG.size);
    this->listSystemCommandsDVIPNG.SetSize(this->texFileNamesForPNG.size);
    for (int i=0; i<this->texFileNamesForPNG.size; i++)
    { CGI::OpenFileCreateIfNotPresent(fileFlas, this->texFileNamesForPNG[i], false, true, false);
      fileFlas << "\\documentclass{article}\\begin{document}\\pagestyle{empty}\n"
      << this->texStringsEachFile[i] << "\n\\end{document}";
      std::stringstream tempStreamLatex, tempStreamPNG;
      tempStreamLatex << "latex " << " -output-directory="
      << physicalPathSl2s << " " << this->texFileNamesForPNG[i];
      tempS= this->texFileNamesForPNG[i];
      tempS.resize(tempS.size()-4);
      tempStreamPNG << "dvipng " << tempS << ".dvi -o " << tempS << ".png -T tight";
      this->listSystemCommandsLatex[i]= tempStreamLatex.str();
      this->listSystemCommandsDVIPNG[i]=tempStreamPNG.str();
      fileFlas.close();
    }
  }
}

void rootSubalgebra::ToString
(std::string& output, SltwoSubalgebras* sl2s, int indexInOwner, bool useLatex,
 bool useHtml, bool includeKEpsCoords, GlobalVariables* theGlobalVariables)
{ std::stringstream out;
  std::string tempS;
  std::string latexFooter, latexHeader;
  if (this->SimpleBasisgEpsCoords.size!=this->SimpleBasisK.size || this->SimpleBasisKEpsCoords.size!= this->SimpleBasisK.size || this->kModulesgEpsCoords.size!= this->kModules.size || this->kModulesKepsCoords.size!= this->kModules.size)
    includeKEpsCoords=false;
  int LatexLineCounter=0;
  this->ElementToStringHeaderFooter(latexHeader, latexFooter, useLatex, useHtml, includeKEpsCoords);
  tempS=this->theDynkinDiagram.ToString();
  if (useLatex)
    out << "\\noindent$\\mathfrak{k}_{ss}:$ ";
  else
    out << "k_{ss}: ";
  out << tempS;
  if (sl2s!=0)
  { out << " &nbsp&nbsp&nbsp Contained in: ";
    for (int i=0; i<this->indicesSubalgebrasContainingK.size; i++)
    { if (useHtml)
        out << "<a href=\"./rootHtml_rootSA"
        << this->indicesSubalgebrasContainingK[i] << ".html\">";
      out << tempS;
      if (useHtml)
        out << "</a>, ";
    }
    if (useHtml)
      out << "<br> <a href=\"./rootHtml.html\">Back to root subsystem table </a> ";
  }
  tempS=this->SimpleBasisK.ToString();
  if (useHtml)
    out << "\n<br>\n";
  if (useLatex)
    out << "\n\\noindent";
  out << " Simple basis: " << tempS;
  tempS=this->SimpleBasisgEpsCoords.ElementToStringEpsilonForm(useLatex, useHtml, false);
  if (useHtml)
    out << "\n<br>\nSimple basis epsilon form: " << tempS;
  tempS=this->SimpleBasisKEpsCoords.ElementToStringEpsilonForm(useLatex, useHtml, false);
  if (useHtml)
    out << "\n<br>\nSimple basis epsilon form with respect to k: " << tempS;
  tempS= this->theCentralizerDiagram.ToString();
  if(!useLatex)
    CGI::clearDollarSigns(tempS, tempS);
  if (useLatex)
    out << "\n\n\\noindent ";
  if (useHtml)
    out << "<br>\n";
  if (useLatex)
    out << "$C(\\mathfrak{k_{ss}})_{ss}$: ";
  else
    out << "C(k_{ss})_{ss}: ";
  out << tempS;
  //int CartanPieceSize=
    //this->AmbientWeyl.CartanSymmetric.NumRows- this->SimpleBasisCentralizerRoots.size-
    //  this->SimpleBasisK.size;
  //if (CartanPieceSize!=0)
  //{  if (useLatex)
  //    out << "$\\oplus\\mathfrak{h}_" << CartanPieceSize<<"$";
  //  if (useHtml)
  //    out <<"+h_"<<CartanPieceSize;
  //}
  if (useHtml)
    out << "<br>\n simple basis centralizer: ";
  if (useLatex)
    out << "; simple basis centralizer: ";
  tempS=this->SimpleBasisCentralizerRoots.ToString();
  out << tempS;
  if (sl2s!=0)
  { if (useHtml)
      out << "\n<hr>\n";
    List<int> hCharacteristics_S_subalgebras;
    //this->ComputeIndicesSl2s(indexInOwner, *sl2s, hCharacteristics_S_subalgebras);
    hCharacteristics_S_subalgebras.size=0;
    out << "\nCharacteristics of sl(2) subalgebras that have no centralizer in k (total "
    << sl2s->IndicesSl2sContainedInRootSA[indexInOwner].size << "): ";
    for (int i=0; i<sl2s->IndicesSl2sContainedInRootSA[indexInOwner].size; i++)
    { int theSl2index=sl2s->IndicesSl2sContainedInRootSA[indexInOwner][i];
      slTwoSubalgebra& theSl2 = (*sl2s)[theSl2index];
      if (useHtml)
        out << "<a href=\"./sl2s/sl2s.html#sl2index" << theSl2index << "\">";
      out << theSl2.hCharacteristic.ToString() << ", ";
      if (useHtml)
        out << "</a>";
      bool isS_subalgebra=true;
//      theSl2.hCharacteristic.ComputeDebugString();
      for (int j=0; j<theSl2.IndicesContainingRootSAs.size; j++)
      { int indexComparison= theSl2.IndicesContainingRootSAs[j];
        if (indexComparison!=indexInOwner && sl2s->theRootSAs[indexComparison].indicesSubalgebrasContainingK.Contains(indexInOwner))
        { isS_subalgebra=false;
          break;
        }
      }
      if (isS_subalgebra)
        hCharacteristics_S_subalgebras.AddOnTop(sl2s->IndicesSl2sContainedInRootSA[indexInOwner][i]);
    }
    if (useHtml)
      out << "\n<br>\n";
    out << "\nS-sl(2) subalgebras in k (total " << hCharacteristics_S_subalgebras.size << "): ";
    for (int i=0; i<hCharacteristics_S_subalgebras.size; i++)
      out << sl2s->TheObjects[hCharacteristics_S_subalgebras[i]].hCharacteristic.ToString() << ", ";
  }
  if (useHtml)
  { out << "<hr>\n Number of k-submodules of g/k: " << this->HighestWeightsGmodK.size;
    out << "<br>Module decomposition over k follows. "
    << "The decomposition is given in 1) epsilon coordinates w.r.t. g 2)"
    << " simple coordinates w.r.t. g <br> ";
    std::stringstream //tempStream1,
    tempStream2, tempStream3;
    for(int i=0; i<this->HighestWeightsGmodK.size; i++)
    { //tempStream1 << "\\underbrace{V_{";
      tempStream2 << "\\underbrace{V_{";
      tempStream3 << "\\underbrace{V_{";
      //tempStream1
      //<< this->AmbientWeyl.GetFundamentalCoordinatesFromSimple
      //(this->HighestWeightsGmodK[i]).ElementToStringLetterFormat("\\omega", true, false);
      tempStream2
      << this->kModulesgEpsCoords[i][0].ToStringLetterFormat("\\epsilon");
      tempStream3
      << this->HighestWeightsGmodK[i].ToStringLetterFormat("\\alpha");
      //tempStream1 << "}}_{dim= " << this->kModules[i].size << "} ";
      tempStream2 << "}}_{dim= " << this->kModules[i].size << "} ";
      tempStream3 << "}}_{dim= " << this->kModules[i].size << "} ";
      if (i!=this->HighestWeightsGmodK.size-1)
      { //tempStream1 << "\\oplus";
        tempStream2 << "\\oplus";
        tempStream3 << "\\oplus";
      }
    }
//    out << "\n<hr>\n" << CGI::GetHtmlMathSpanFromLatexFormula(tempStream1.str()) << "\n";
    out << "\n<hr>\n" << CGI::GetHtmlMathSpanFromLatexFormula(tempStream2.str()) << "\n";
    out << "\n<hr>\n" << CGI::GetHtmlMathSpanFromLatexFormula(tempStream3.str()) << "\n<hr>\n";
  }
  if (useLatex)
    out << "\n\n\\noindent Number $\\mathfrak{g}/\\mathfrak{k}$ $\\mathfrak{k}$-submodules: ";
  if (!useHtml)
    out << this->LowestWeightsGmodK.size ;
  if (useHtml)
    out << "<br>\n";
  if (useLatex)
    out << "\n\n";
  out << latexHeader;
  this->kModulesgEpsCoords.SetSize(this->kModules.size);
  for (int i=0; i<this->kModules.size; i++)
  { tempS=this->LowestWeightsGmodK[i].ToString();
    if (useHtml)
      out << "\n<tr><td>";
    if (useLatex)
      out << "\\hline ";
    out << i;
    if (useHtml)
      out << "</td><td>";
    if (useLatex)
      out << " & ";
    out << this->kModules[i].size;
    if (useHtml)
      out << "</td><td>";
    if (useLatex)
      out << " & ";
    out << tempS;
    tempS=this->HighestWeightsGmodK[i].ToString();
    if (useHtml)
      out << "</td><td>";
    if (useLatex)
      out << " & ";
    out  << tempS;
    if (useHtml)
      out << "</td><td>";
    if (useLatex)
      out << " & \n";
    out << this->kModules[i].ToString();
    if (useHtml)
      out << "</td><td>";
    if (i>=this->kModulesgEpsCoords.size)
      this->GetAmbientWeyl().GetEpsilonCoords(this->kModules[i], this->kModulesgEpsCoords[i]);
    out << this->kModulesgEpsCoords[i].ElementToStringEpsilonForm(useLatex, useHtml, true);
    if (useLatex)
      out << " & \n";
    if (useHtml)
      out << "</td>";
    if (includeKEpsCoords)
    { if (useHtml)
        out << "<td>";
      if (useLatex)
        out << " & ";
      out << tempS << this->kModulesKepsCoords[i].ElementToStringEpsilonForm(useLatex, useHtml, true);
      if (useHtml)
        out << "</td>";
      if (useLatex)
        out << "\\\\\n";
    }
    if (useHtml)
      out << "</tr>";
    if (LatexLineCounter>this->NumGmodKtableRowsAllowedLatex)
    { LatexLineCounter=0;
      out << latexFooter << latexHeader;
    }
    if (i!=this->kModules.size-1)
    { LatexLineCounter+=this->kModules[i].size;
      if (useLatex)
       if ((LatexLineCounter>this->NumGmodKtableRowsAllowedLatex) && (LatexLineCounter!=this->kModules.TheObjects[i].size))
        { out << latexFooter << latexHeader;
          LatexLineCounter = this->kModules[i].size;
        }
    }
  }
  if (useHtml)
    out << "</table>";
  if (useLatex)
    out << latexFooter;
  if ((useLatex|| useHtml)&& this->theMultTable.size==0 && this->kModules.size!=0)
    this->GenerateKmodMultTable(this->theMultTable, this->theOppositeKmods, theGlobalVariables);
  if (this->theMultTable.size!=0)
  { if (useHtml)
      out << "\n\n Pairing table:\n\n";
    if (useLatex)
      out << "\n\n\\noindent Pairing table:\n\n\\noindent";
    this->theMultTable.ToString(tempS, useLatex, useHtml, *this);
    out << tempS << "\n";
  }
  output=out.str();
}

std::string CandidateSSSubalgebra::ToString(FormatExpressions* theFormat)const
{ MacroRegisterFunctionWithName("CandidateSSSubalgebra::ToString");
  std::stringstream out;
  out << this->theWeylNonEmbeddeD.theDynkinType;
  out << ". ";
//  out << "<br>Predefined or computed simple generators follow. ";
/*  out << "<br>Negative generators: ";
  for (int i=0; i<this->theNegGens.size; i++)
  { out << this->theNegGens[i].ToString(theFormat);
    if (i!=this->theNegGens.size-1)
      out << ", ";
  }
  out << "<br>Positive generators: ";
  for (int i=0; i<this->thePosGens.size; i++)
  { out << this->thePosGens[i].ToString(theFormat);
    if (i!=this->thePosGens.size-1)
      out << ", ";
  }*/
  if (this->flagSystemProvedToHaveNoSolution)
  { out << " <b> Subalgebra candidate proved to be impossible! </b> ";
    return out.str();
  }
  if (this->theBasis.size!=0)
  { out << "<br>Dimension of subalgebra generated by predefined or computed generators:"
    << this->theBasis.size << "<br>The generators: ";
    for (int i=0; i<this->theBasis.size; i++)
    { out << this->theBasis[i].ToString(theFormat);
      if (i!=this->theBasis.size-1)
        out << ", ";
    }
  }
  out << "<br>Symmetric Cartan matrix scaled: "
  << this->theWeylNonEmbeddeD.CartanSymmetric.ToString(theFormat);
  out << "<br>Decomposition ambient Lie algebra: "
  << this->theCharFundCoords.ToString();
  if (this->theBasis.size!=this->theWeylNonEmbeddeD.theDynkinType.GetRootSystemPlusRank())
  { if (!this->flagSystemSolved)
      out << " <b> Subalgebra not realized, but it might have a solution. </b> ";
    out << "<br>" << this->theUnknownNegGens.size << "*2 (unknown) gens:<br>(";
    for (int i=0; i<this->theUnknownNegGens.size; i++)
    { out << "<br>" << this->theUnknownNegGens[i].ToString(theFormat) << ", " ;
      out << this->theUnknownPosGens[i].ToString(theFormat);
      if (i!=this->theUnknownNegGens.size-1)
        out << ", ";
    }
    out << ")<br>";
    List<DynkinSimpleType> theTypes;
    this->theWeylNonEmbeddeD.theDynkinType.GetTypesWithMults(theTypes);
    for (int i=0; i<this->CartanSAsByComponent.size; i++)
    { out << "Component " << theTypes[i];
      for (int j=0; j<this->CartanSAsByComponent[i].size; j++)
        out << "h-> " << this->CartanSAsByComponent[i][j].ToString() << ", ";
    }
    for (int i=0; i<this->theHs.size; i++)
    { out << "h: " << this->theHs[i] << ", "
      << " e = combination of " << this->theInvolvedPosGenerators[i].ToString()
      << ", f= combination of " << this->theInvolvedNegGenerators[i].ToString();
    }
    out << "Positive weight subsystem: " << this->theWeylNonEmbeddeD.RootsOfBorel.ToString();
    if (this->PosRootsPerpendicularPrecedingWeights.size>0)
      out << " Positive roots that commute with the weight subsystem: "
      << this->PosRootsPerpendicularPrecedingWeights.ToString();
    out << "<br>Symmetric Cartan default scale: "
    << this->theWeylNonEmbeddeDdefaultScale.CartanSymmetric.ToString(theFormat);
    out << "Character ambient Lie algebra: "
    << this->theCharFundamentalCoordsRelativeToCartan.ToString();
    out << "<br>A necessary system to realize the candidate subalgebra.  ";
    FormatExpressions tempFormat=this->theCoeffLetters;
    for (int i=0; i<this->theSystemToSolve.size; i++)
      out << "<br>" << this->theSystemToSolve[i].ToString(&tempFormat) << "= 0";
    out << "<br>The above system after transformation.  ";
    for (int i=0; i<this->transformedSystem.size; i++)
      out << "<br>" << this->transformedSystem[i].ToString(&tempFormat) << "= 0";
    if (!this->flagSystemGroebnerBasisFound)
      out << "<br><b>Failed to find Groebner basis of the above system (the computation is too large).</b>";
    if (this->owner!=0 && this->theHorbitIndices.size>0)
      if (this->theHorbitIndices[0].size>0)
        if (this->theUnknownPosGens.size>0)
        { slTwoSubalgebra& theFirstSl2=this->owner->theSl2s[this->theHorbitIndices[0][0]];
          out << "<br>First positive generator seed realization.<br> "
          << this->theUnknownPosGens[0].ToString(theFormat) << " -> " << theFirstSl2.theE.ToString(theFormat);
          out << "<br>First negative generator seed realization.<br> "
          << this->theUnknownNegGens[0].ToString(theFormat) << " -> " << theFirstSl2.theF.ToString(theFormat);
        }
    out << "<br><b>For the calculator: </b><br>GroebnerLexUpperLimit{}(10000, ";
    for (int i=0; i<this->transformedSystem.size; i++)
    { out << this->transformedSystem[i].ToString(&tempFormat);
      if (i!=this->transformedSystem.size-1)
        out << ", ";
    }
    out << " )";
    if (this->flagSystemSolved)
      out << "<br>Solution of above system: " << this->aSolution.ToString();
  } else
  { List<ElementSemisimpleLieAlgebra<Rational> > highestVectors;
    this->owner->owneR->GetCommonCentralizer(this->thePosGens, highestVectors);
    out << "<br>Highest vectors of representations (total " << highestVectors.size << "): ";
    for (int i=0; i<highestVectors.size; i++)
    { out << highestVectors[i].ToString();
      if (i!=highestVectors.size-1)
        out << ", ";
    }
  }
  return out.str();
}

bool PolynomialSystem::IsALinearSystemWithSolution(Vector<Rational>* outputSolution)
{ MacroRegisterFunctionWithName("PolynomialSystem::IsALinearSystemWithSolution");
  if (this->size<=0)
    return false;
  Matrix<Rational> theSystem;
  Matrix<Rational> theColumnVect, theSolution;
  int numVars=0;
  for (int i=0; i<this->size; i++)
    numVars=MathRoutines::Maximum(numVars, (*this)[i].GetMinNumVars());
  theSystem.init(this->size, numVars);
  theSystem.NullifyAll();
  theColumnVect.init(this->size, 1);
  theColumnVect.NullifyAll();
  for (int i=0; i<this->size; i++)
  { Polynomial<Rational>& curP=(*this)[i];
    for (int j=0; j<curP.size; j++)
    { MonomialP& curMon=curP[j];
      int theIndex;
      if (curMon.IsOneLetterFirstDegree(&theIndex))
        theSystem(j,theIndex)=curP.theCoeffs[j];
      else if (curMon.IsAConstant())
        theColumnVect(j, 0)=curP.theCoeffs[j];
      else
        return false;
    }
  }
  if (!theSystem.Solve_Ax_Equals_b_ModifyInputReturnFirstSolutionIfExists(theSystem, theColumnVect, theSolution))
    return false;
  if (outputSolution!=0)
    outputSolution->AssignMatDetectRowOrColumn(theSolution);
  return true;
}

bool DynkinSimpleType::IsPossibleCoRootLength(const Rational& input)const
{ if (this->theLetter=='C')
  { if ((this->lengthFirstCoRootSquared/input)==2 ||
        (this->lengthFirstCoRootSquared/input)==1)
      return true;
    return false;
  }
  if (this->theLetter=='F' || this->theLetter=='B' )
  { if ((input/this->lengthFirstCoRootSquared)==2 ||
        (input/this->lengthFirstCoRootSquared)==1)
      return true;
    return false;
  }
  if (this->theLetter=='G')
  { //std::cout << "<br>Testing input co-root length " << input << " vs current co-root length: "
    //<< this->lengthFirstCoRootSquared;
    if ((this->lengthFirstCoRootSquared/input)==3 ||
        (this->lengthFirstCoRootSquared/input)==1)
      return true;
    return false;
  }
  if (this->theLetter=='A' || this->theLetter=='D' || this->theLetter=='E')
    return input==this->lengthFirstCoRootSquared;
  std::cout << "This is a programmig error: non-initialized or otherwise faulty Dynkin simple type. "
  << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
  assert(false);
  return false;
}
