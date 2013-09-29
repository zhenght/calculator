//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#ifndef vpfHeader_HeaderModulesSSLielgebras
#define vpfHeader_HeaderModulesSSLielgebras
#include "vpfHeader2Math0_General.h"

static ProjectInformationInstance ProjectInfoVpfHeaderModulesSSLielgebras(__FILE__, "Header, generalized Verma modules. ");

template <class coefficient>
class ModuleSSalgebra
{ List<MatrixTensor<coefficient> > actionsGeneratorsMaT;
  List<List<List<ElementUniversalEnveloping<coefficient> > > > actionsGeneratorS;
  Selection ComputedGeneratorActions;
  Rational hwtaabfSimpleGensOnly
  (const MonomialTensor<int, MathRoutines::IntUnsignIdentity>& leftMon, const MonomialTensor<int, MathRoutines::IntUnsignIdentity>& rightMon,
   ProgressReport* theProgressReport=0);
  Rational hwTrace
  (const Pair<MonomialTensor<int, MathRoutines::IntUnsignIdentity>, MonomialTensor<int, MathRoutines::IntUnsignIdentity> >& thePair, ProgressReport* theProgressReport=0);
  void CheckConsistency(GlobalVariables& theGlobalVariables);
public:
  SemisimpleLieAlgebra* owneR;
  HashedList<MonomialUniversalEnveloping<coefficient> > theGeneratingWordsNonReduced;
  //Note: for some reason, the linker fails to resolve without the explicit template
  //specialization below.
  //[Update:] made a bug report on this in the gcc bug tracker.
  //This issue has officially been recognized as a gcc bug. Hope to get a fix soon.
  HashedListSpecialized<MonomialTensor<int, MathRoutines::IntUnsignIdentity> > theGeneratingWordsNonReducedInt;
  Vectors<Rational> theGeneratingWordsWeightsPlusWeightFDpart;
  List<LittelmannPath> thePaths;
  List<List<MonomialUniversalEnveloping<coefficient> > > theGeneratingWordsGrouppedByWeight;
  List<List<MonomialTensor<int, MathRoutines::IntUnsignIdentity> > > theGeneratingWordsIntGrouppedByWeight;
//  List<ElementUniversalEnveloping<coefficient> > theSimpleGens;
//  List<List<List<ElementUniversalEnveloping<coefficient> > > > actionsSimpleGens;
//  List<Matrix<coefficient> > actionsSimpleGensMatrixForM;
  List<Matrix<coefficient> > theBilinearFormsAtEachWeightLevel;
  List<Matrix<coefficient> > theBilinearFormsInverted;
//  Vectors<Rational> weightsSimpleGens;
  Vector<coefficient> theHWDualCoordsBaseFielD;
  Vector<coefficient> theHWSimpleCoordSBaseField;
  Vector<coefficient> theHWFundamentalCoordsBaseField;
  Vector<Rational> theHWFDpartDualCoords;
  Vector<Rational> theHWFDpartSimpleCoordS;
  Vector<Rational> theHWFDpartFundamentalCoordS;
//  List<List<Matrix<coefficient> > >
  HashedList<Vector<Rational> > theModuleWeightsSimpleCoords;
  charSSAlgMod<coefficient> theCharOverH;
  charSSAlgMod<coefficient> theChaR;
  Selection parabolicSelectionNonSelectedAreElementsLevi;
  Selection parabolicSelectionSelectedAreElementsLevi;
  std::string highestWeightVectorNotation;
  //Note: for some reason, the linker fails to resolve without the
  //explicit template specialization below.
  //[Update:] This is now a recognized gcc bug.
  HashedListSpecialized<Pair<MonomialTensor<int, MathRoutines::IntUnsignIdentity>, MonomialTensor<int, MathRoutines::IntUnsignIdentity> > > cachedPairs;
  List<Rational> cachedTraces;
  bool flagIsInitialized;
  bool flagConjectureBholds;
  bool flagConjectureCholds;
  int NumCachedPairsBeforeSimpleGen;
  int NumRationalMultiplicationsAndAdditionsBeforeSimpleGen;
  int MaxNumCachedPairs;
  void reset();
  bool operator==(const ModuleSSalgebra<coefficient>& other)
  { return
    this->owneR==other.owneR && this->theHWFundamentalCoordsBaseField==other.theHWFundamentalCoordsBaseField
    && this->parabolicSelectionNonSelectedAreElementsLevi==other.parabolicSelectionNonSelectedAreElementsLevi;
  }
  bool HasFreeAction(int generatorIndex)const
  { Vector<Rational> theWeight= this->GetOwner().GetWeightOfGenerator(generatorIndex);
    for (int i=0; i<this->parabolicSelectionNonSelectedAreElementsLevi.CardinalitySelection; i++)
      if (theWeight[this->parabolicSelectionNonSelectedAreElementsLevi.elements[i]].IsNegative())
        return true;
    return false;
  }
  bool HasZeroActionFDpart(int generatorIndex)const
  { Vector<Rational> theWeight= this->GetOwner().GetWeightOfGenerator(generatorIndex);
    for (int i=0; i<this->parabolicSelectionNonSelectedAreElementsLevi.CardinalitySelection; i++)
      if (theWeight[this->parabolicSelectionNonSelectedAreElementsLevi.elements[i]].IsPositive())
        return true;
    return false;
  }
  int GetOffsetFromWeightIndex(int weightIndex)
  { int result=0;
    for (int i=0; i<weightIndex; i++)
      result+=this->theGeneratingWordsGrouppedByWeight[i].size;
    return result;
  }
  void ApplyTAA
  (MonomialTensor<int, MathRoutines::IntUnsignIdentity>& theMon)
  ;
  void GetFDchar(charSSAlgMod<coefficient>& output);
  void Substitution(const PolynomialSubstitution<Rational>& theSub);
//  List<ElementUniversalEnveloping<coefficient> > theGeneratingWordsLittelmannForm;
//  HashedList<MonomialUniversalEnveloping<coefficient> > theGeneratingMonsPBWform;
//  List
//  List<Matrix<coefficient> > ActionsChevalleyGenerators;
  MatrixTensor<coefficient>& GetActionGeneratorIndeX(int generatorIndex, GlobalVariables& theGlobalVariables, const coefficient& theRingUnit=1, const coefficient& theRingZero=0);
  MatrixTensor<coefficient>& GetActionSimpleGeneratorIndex(int generatorIndex, GlobalVariables& theGlobalVariables, const coefficient& theRingUnit=1, const coefficient& theRingZero=0);
  int GetMinNumVars()
  { if (this->theHWFundamentalCoordsBaseField.size<=0)
      return -1;
    int result=0;
    for (int i=0; i<this->theHWFundamentalCoordsBaseField.size; i++)
      result=MathRoutines::Maximum
      (result, this->theHWFundamentalCoordsBaseField[i].GetMinNumVars());
    return result;
  }
  int GetDim()const
  { return this->theGeneratingWordsNonReduced.size;
  }
  void IntermediateStepForMakeFromHW(Vector<coefficient>& HWDualCoordS, GlobalVariables& theGlobalVariables, const coefficient& theRingUnit, const coefficient& theRingZero);
  bool MakeFromHW
  (SemisimpleLieAlgebra& inputAlgebra, Vector<coefficient>& HWFundCoords, const Selection& selNonSelectedAreElementsLevi,
   GlobalVariables& theGlobalVariables, const coefficient& theRingUnit, const coefficient& theRingZero, std::string* outputReport,
  bool computeSimpleGens=true);
  SemisimpleLieAlgebra& GetOwner()const
  { if (this->owneR==0)
    { std::cout << "This is a programming error: calling GetOwner() on a non-initialized "
      << " generalized Verma module. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    return *this->owneR;
  }
  void GetAdActionHomogenousElT
  (ElementUniversalEnveloping<coefficient>& inputHomogeneous, Vector<Rational> & weightUEEltSimpleCoords, List<List<ElementUniversalEnveloping<coefficient> > >& outputSortedByArgumentWeight,
   GlobalVariables& theGlobalVariables, const coefficient& theRingUnit, const coefficient& theRingZero);
  void GetMatrixHomogenousElt
  (ElementUniversalEnveloping<coefficient>& inputHomogeneous, List<List<ElementUniversalEnveloping<coefficient> > >& outputSortedByArgumentWeight,
   Vector<Rational> & weightUEEltSimpleCoords, MatrixTensor<coefficient>& output, GlobalVariables& theGlobalVariables,
   const coefficient& theRingUnit, const coefficient& theRingZero);
  void ExpressAsLinearCombinationHomogenousElement
  (ElementUniversalEnveloping<coefficient>& inputHomogeneous, ElementUniversalEnveloping<coefficient>& outputHomogeneous, int indexInputBasis,
   const Vector<coefficient>& subHiGoesToIthElement, GlobalVariables& theGlobalVariables, const coefficient& theRingUnit, const coefficient& theRingZero);
  std::string ToString(FormatExpressions* theFormat=0)const;
  std::string ElementToStringHWV(FormatExpressions* theFormat=0)const
  { if (this->highestWeightVectorNotation!="")
      return this->highestWeightVectorNotation;
    std::stringstream out;
    out << "v_{" << this->theHWFundamentalCoordsBaseField.ToString(theFormat) << ", " << this->parabolicSelectionNonSelectedAreElementsLevi.ToString() << "}";
    return out.str();
//    return "hwv{}("+ this->GetOwner().GetLieAlgebraName(false)+ "," + this->theHWFundamentalCoordsBaseField.ToString(theFormat) + ","
//    + Vector<Rational> (this->parabolicSelectionNonSelectedAreElementsLevi).ToString(theFormat) + ")";
  }
  void SplitOverLevi
  (std::string* Report, Selection& splittingParSel, GlobalVariables& theGlobalVariables, const coefficient& theRingUnit, const coefficient& theRingZero,
   List<ElementUniversalEnveloping<coefficient> >* outputEigenVectors=0, Vectors<coefficient>* outputWeightsFundCoords=0, Vectors<coefficient>* outputEigenSpace=0,
   charSSAlgMod<coefficient>* outputChar=0);
  void SplitFDpartOverFKLeviRedSubalg
  (HomomorphismSemisimpleLieAlgebra& theHmm, Selection& LeviInSmall, GlobalVariables& theGlobalVariables,
   List<ElementUniversalEnveloping<coefficient> >* outputEigenVectors=0, Vectors<coefficient>* outputWeightsFundCoords=0,
   Vectors<coefficient>* outputEigenSpace=0, std::stringstream* comments=0, const coefficient& theRingUnit=1, const coefficient& theRingZero=0);
  template<class ResultType>
  void GetElementsNilradical(List<ElementUniversalEnveloping<ResultType> >& output, bool useNegativeNilradical, List<int>* listOfGenerators=0);
  bool IsNotInLevi(int theGeneratorIndex);
  bool IsNotInParabolic(int theGeneratorIndex);
  void GetGenericUnMinusElt(bool shiftPowersByNumVarsBaseField, ElementUniversalEnveloping<Polynomial<Rational> >& output, GlobalVariables& theGlobalVariables);
  void GetGenericUnMinusElt(bool shiftPowersByNumVarsBaseField, ElementUniversalEnveloping<RationalFunctionOld>& output, GlobalVariables& theGlobalVariables);
  //The input of the following function is supposed to be an honest element of the
  //Universal enveloping,
  //i.e. inputElt is not allowed to have non-small integer exponents.
  bool GetActionGenVermaModuleAsDiffOperator(ElementSemisimpleLieAlgebra<Rational>& inputElt, quasiDiffOp<Rational>& output, GlobalVariables& theGlobalVariables);
  bool GetActionEulerOperatorPart(const MonomialP& theCoeff, ElementWeylAlgebra<Rational>& outputDO, GlobalVariables& theGlobalVariables);
  ModuleSSalgebra() : owneR(0), flagIsInitialized(false), MaxNumCachedPairs(1000000)
  {
  }
};

template<class coefficient>
class ElementTensorsGeneralizedVermas :
public MonomialCollection<MonomialTensorGeneralizedVermas<coefficient>, coefficient>
{
public:
  bool MultiplyOnTheLeft
  (const ElementUniversalEnveloping<coefficient>& theUE, ElementTensorsGeneralizedVermas<coefficient>& output, SemisimpleLieAlgebra& ownerAlgebra,
   GlobalVariables& theGlobalVariables, const coefficient& theRingUnit, const coefficient& theRingZero)const;
  bool MultiplyOnTheLeft
  (const MonomialUniversalEnveloping<coefficient>& theUE, ElementTensorsGeneralizedVermas<coefficient>& output, SemisimpleLieAlgebra& ownerAlgebra,
   GlobalVariables& theGlobalVariables, const coefficient& theRingUnit, const coefficient& theRingZero)const;
  void TensorOnTheRight
  (const ElementTensorsGeneralizedVermas<coefficient>& right, GlobalVariables& theGlobalVariables, const coefficient& theRingUnit=1, const coefficient& theRingZero=0);
  void MultiplyByElementLieAlg
  (ElementTensorsGeneralizedVermas<coefficient>& output, SemisimpleLieAlgebra& ownerAlgebra, int indexGenerator, GlobalVariables& theGlobalVariables,
   const coefficient& theRingUnit, const coefficient& theRingZero)const;
  void MultiplyBy(const ElementTensorsGeneralizedVermas<coefficient>& standsOnTheRight, GlobalVariables& theGlobalVariables);
  bool IsHWV()const
  { if (this->theCoeffs.size!=1)
      return false;
    if (!this->theCoeffs[0].IsEqualToOne())
      return false;
    return (*this)[0].IsHWV();
  }
  void MakeHWV(ModuleSSalgebra<coefficient>& theOwner, const coefficient& theRingUnit);
  void Substitution(const PolynomialSubstitution<Rational>& theSub, ListReferences<ModuleSSalgebra<coefficient> >& theMods);
  void SetNumVariables(int GoalNumVars)
  { for (int i=0; i<this->size; i++)
      this->TheObjects[i].SetNumVariables(GoalNumVars);
  }
  SemisimpleLieAlgebra& GetOwnerSS()const
  { if (this->size() <=0)
    { std::cout << "This is a programming error: calling GetOwnerModule() on a tensor element which has no monomials."
      << " This is not allowed as the index of the owner modules are stored in the monomials. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    const MonomialTensorGeneralizedVermas<coefficient>& theMon=(*this)[0];
    if (theMon.theMons.size<=0)
    { std::cout << "This is a programming error: calling GetOwnerModule() on a tensor element which has a constant monomial."
      << " This is not allowed: constant monomials do not have owners. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    MonomialGeneralizedVerma<coefficient>& theGmon=theMon.theMons[0];
    return theGmon.owneR->GetOwner();
  }
  int GetNumVars()
  { if (this->size==0)
      return -1;
    int theAnswer=this->TheObjects[0].GetNumVars();
    for (int i=1; i<this->size; i++)
      if (theAnswer!=this->TheObjects[i].GetNumVars())
        return -1;
    return theAnswer;
  }
  unsigned int HashFunction()const
  { return this->:: MonomialCollection<MonomialTensorGeneralizedVermas<coefficient>, coefficient >::HashFunction();
  }
  void operator=(const ElementTensorsGeneralizedVermas<coefficient>& other)
  { this->::MonomialCollection<MonomialTensorGeneralizedVermas<coefficient>, coefficient>::operator=(other);
  }
  void operator=(const ElementSumGeneralizedVermas<coefficient>& other);
  static unsigned int HashFunction(const ElementTensorsGeneralizedVermas<coefficient>& input)
  { return input.HashFunction();
  }
};
#endif
