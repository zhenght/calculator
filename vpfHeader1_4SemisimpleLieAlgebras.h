//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#ifndef vpfHeader1_4_h_already_included
#define vpfHeader1_4_h_already_included

#include "vpfHeader1_2.h"
static ProjectInformationInstance ProjectInfoVpfHeader1_4
(__FILE__, "Header, math routines concerning semisimple Lie algebras. ");

class slTwoSubalgebra
{
public:
  List<int> highestWeights;
  List<int> multiplicitiesHighestWeights;
  List<int> weightSpaceDimensions;
  ElementSemisimpleLieAlgebra<Rational> theH, theE, theF;
  ElementSemisimpleLieAlgebra<Rational> bufferHbracketE, bufferHbracketF, bufferEbracketF;
  SemisimpleLieAlgebra* owneR;
  SltwoSubalgebras* container;
  Rational LengthHsquared;
  int indexInContainer;
  List<int> IndicesContainingRootSAs;
  List<int> IndicesMinimalContainingRootSA;
  List<List<int> > HighestWeightsDecompositionMinimalContainingRootSA;
  List<List<int> > MultiplicitiesDecompositionMinimalContainingRootSA;
  Vectors<Rational> preferredAmbientSimpleBasis;
  Vector<Rational> hCharacteristic;
  Vector<Rational> hElementCorrespondingToCharacteristic;
  Vectors<Rational> hCommutingRootSpaces;
  Vectors<Rational> RootsWithScalar2WithH;
  DynkinDiagramRootSubalgebra DiagramM;
  DynkinDiagramRootSubalgebra CentralizerDiagram;
  PolynomialSubstitution<Rational> theSystemToBeSolved;
  Matrix<Rational> theSystemMatrixForm;
  Matrix<Rational> theSystemColumnVector;
  bool DifferenceTwoHsimpleRootsIsARoot;
  int DynkinsEpsilon;
  slTwoSubalgebra(): owneR(0), container(0), indexInContainer(-1){}
  SltwoSubalgebras& GetContainerSl2s()
  { if (this->container==0)
    { std::cout << "This is a programming error: attempting to access the container "
      << " list of a non-initialized sl(2)-subalgebra. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    return *this->container;
  }
  WeylGroup& GetOwnerWeyl();
  SemisimpleLieAlgebra& GetOwnerSSAlgebra()
  { if (this->owneR==0)
    { std::cout << "This is a programming error: attempting to access the ambient "
      << " Lie algebra of a non-initialized sl(2)-subalgebra. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    return *this->owneR;
  }
  std::string ToString(FormatExpressions* theFormat=0);
  void GetInvolvedPosGenerators(List<ChevalleyGenerator>& output);
  void GetInvolvedNegGenerators(List<ChevalleyGenerator>& output);
  void ElementToStringModuleDecomposition(bool useLatex, bool useHtml, std::string& output);
  void ElementToStringModuleDecompositionMinimalContainingRegularSAs(bool useLatex, bool useHtml, SltwoSubalgebras& owner, std::string& output);
  void ComputeModuleDecomposition(Vectors<Rational>& positiveRootsContainingRegularSA, int dimensionContainingRegularSA, List<int>& outputHighestWeights, List<int>& outputMultiplicitiesHighestWeights, List<int>& outputWeightSpaceDimensions, GlobalVariables& theGlobalVariables);
  void ComputeModuleDecompositionAmbientLieAlgebra(GlobalVariables& theGlobalVariables);
  void ComputeModuleDecompositionOfMinimalContainingRegularSAs(SltwoSubalgebras& owner, int IndexInOwner, GlobalVariables& theGlobalVariables);
  bool ModuleDecompositionFitsInto(const slTwoSubalgebra& other);
  static bool ModuleDecompositionFitsInto(const List<int>& highestWeightsLeft, const List<int>& multiplicitiesHighestWeightsLeft, const List<int>& highestWeightsRight, const List<int>& multiplicitiesHighestWeightsRight);
  void MakeReportPrecomputations(GlobalVariables& theGlobalVariables, SltwoSubalgebras& container, int indexInContainer, int indexMinimalContainingRegularSA, rootSubalgebra& MinimalContainingRegularSubalgebra);
  //the below is outdated, must be deleted as soon as equivalent code is written.
  void ComputeDynkinsEpsilon(WeylGroup& theWeyl);
  void ElementToHtml(std::string& filePath);
  void ElementToHtmlCreateFormulaOutputReference(const std::string& formulaTex, std::stringstream& output, bool usePNG, bool useHtml, SltwoSubalgebras& container, std::string* physicalPath, std::string* htmlPathServer);
  void operator=(const slTwoSubalgebra& right)
  { this->highestWeights=right.highestWeights;
    this->multiplicitiesHighestWeights=right.multiplicitiesHighestWeights;
    this->weightSpaceDimensions=right.weightSpaceDimensions;
    this->HighestWeightsDecompositionMinimalContainingRootSA=
    right.HighestWeightsDecompositionMinimalContainingRootSA;
    this->MultiplicitiesDecompositionMinimalContainingRootSA=
    right.MultiplicitiesDecompositionMinimalContainingRootSA;
    this->hCommutingRootSpaces=right.hCommutingRootSpaces;
    this->CentralizerDiagram=right.CentralizerDiagram;
    this->DiagramM=right.DiagramM;
    this->hCommutingRootSpaces=right.hCommutingRootSpaces;
    this->DifferenceTwoHsimpleRootsIsARoot=right.DifferenceTwoHsimpleRootsIsARoot;
    this->RootsWithScalar2WithH=right.RootsWithScalar2WithH;
    this->DynkinsEpsilon=right.DynkinsEpsilon;
    this->hCharacteristic=right.hCharacteristic;
    this->hElementCorrespondingToCharacteristic=right.hElementCorrespondingToCharacteristic;
    this->owneR=right.owneR;
    this->theE=right.theE;
    this->theH=right.theH;
    this->theF=right.theF;
    this->bufferEbracketF=right.bufferEbracketF;
    this->bufferHbracketE=right.bufferHbracketE;
    this->bufferHbracketF=right.bufferHbracketF;
    this->theSystemToBeSolved=right.theSystemToBeSolved;
    this->theSystemMatrixForm=right.theSystemMatrixForm;
    this->theSystemColumnVector=right.theSystemColumnVector;
    this->IndicesContainingRootSAs=right.IndicesContainingRootSAs;
    this->preferredAmbientSimpleBasis=right.preferredAmbientSimpleBasis;
    this->container=right.container;
    this->indexInContainer=right.indexInContainer;
    this->LengthHsquared=right.LengthHsquared;
  }
  bool operator==(const slTwoSubalgebra& right)const;
  unsigned int HashFunction() const
  { int tempI=MathRoutines::Minimum(SomeRandomPrimesSize, this->hCharacteristic.size);
    int result=0;
    for (int i=0; i<tempI; i++)
      result+= this->hCharacteristic[i].NumShort*SomeRandomPrimes[i];
    return result;
  }
  static inline unsigned int HashFunction(const slTwoSubalgebra& input)
  { return input.HashFunction();
  }
};

class SltwoSubalgebras : public HashedList<slTwoSubalgebra>
{
  friend class SemisimpleSubalgebras;
  SemisimpleLieAlgebra* owner;
public:
  List<int> MultiplicitiesFixedHweight;
  List<List<int> > IndicesSl2sContainedInRootSA;
  List<int> IndicesSl2decompositionFlas;
  Vectors<Rational> BadHCharacteristics;
  int IndexZeroWeight;
  rootSubalgebras theRootSAs;
  List<std::string> texFileNamesForPNG;
  List<std::string> texStringsEachFile;
  List<std::string> listSystemCommandsLatex;
  List<std::string> listSystemCommandsDVIPNG;
  void operator=(const SltwoSubalgebras& other)
  { this->MultiplicitiesFixedHweight=other.MultiplicitiesFixedHweight;
    this->IndicesSl2sContainedInRootSA=other.IndicesSl2sContainedInRootSA;
    this->IndicesSl2decompositionFlas=other.IndicesSl2decompositionFlas;
    this->BadHCharacteristics =other.BadHCharacteristics;
    this->IndexZeroWeight =other.IndexZeroWeight;
    this->owner =other.owner;
    this->theRootSAs =other.theRootSAs;
    this->texFileNamesForPNG =other.texFileNamesForPNG;
    this->texFileNamesForPNG =other.texFileNamesForPNG;
    this->listSystemCommandsLatex =other.listSystemCommandsLatex;
    this->listSystemCommandsDVIPNG =other.listSystemCommandsDVIPNG;
  }
  bool operator==(const SltwoSubalgebras& other)const
  { if (this->owner==0)
      return other.owner==0;
    if (other.owner==0)
      return false;
    return this->GetOwner()==other.GetOwner();
  }
  void CheckForCorrectInitializationCrashIfNot()const
  { if (this->owner==0)
    { std::cout << "<br>This is a programming error. "
      << " Object SltwoSubalgebras is not initialized, although it is supposed to be. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
  }
  WeylGroup& GetOwnerWeyl()const
  { return this->GetOwner().theWeyl;
  }
  SemisimpleLieAlgebra& GetOwner()const
  { this->CheckForCorrectInitializationCrashIfNot();
    return *this->owner;
  }
  void ComputeModuleDecompositionsOfAmbientLieAlgebra(GlobalVariables& theGlobalVariables)
  { for(int i=0; i<this->size; i++)
      this->TheObjects[i].ComputeModuleDecompositionAmbientLieAlgebra(theGlobalVariables);
  }
  void init(SemisimpleLieAlgebra& inputOwners);
  SltwoSubalgebras(): owner(0){}
  SltwoSubalgebras(SemisimpleLieAlgebra& inputOwner)
  : owner(&inputOwner)
  {}
  void ComputeModuleDecompositionsOfMinimalContainingRegularSAs(GlobalVariables& theGlobalVariables)
  { std::cout << "This is a programming error. This function used to work in an older"
    << " version of the program, but, as the requirements have changed, now needs a rewrite. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
    /*for(int i=0; i<this->size; i++)
      (*this)[i].ComputeModuleDecompositionOfMinimalContainingRegularSAs(*this, i, theGlobalVariables);
      */
  }
  void ComputeDebugStringCurrent();
  bool ContainsSl2WithGivenH(Vector<Rational>& theH, int* outputIndex);
  bool ContainsSl2WithGivenHCharacteristic(Vector<Rational>& theHCharacteristic, int* outputIndex);
  void ElementToHtml
  (FormatExpressions* theFormat=0, GlobalVariables* theGlobalVariables=0)
   ;
  std::string ElementToStringNoGenerators(FormatExpressions* theFormat=0);
  void ElementToStringModuleDecompositionMinimalContainingRegularSAs(std::string& output, bool useLatex, bool useHtml);
  std::string ToString(FormatExpressions* theFormat=0);
};

class PolynomialSystem : public List<Polynomial<Rational> >
{
  public:
  void Substitution(const List<Polynomial<Rational> >& theSub)
  { for (int i=0; i<this->size; i++)
      (*this)[i].SubstitutioN(theSub);
  }
  void operator=(List<Polynomial<Rational> >& other)
  { this->::List<Polynomial<Rational> >::operator=(other);
  }
  bool IsALinearSystemWithSolution(Vector<Rational>* outputSolution=0);
};

class SemisimpleSubalgebras;

class FernandoKacNilradicalCandidate
{
  public:
  int indexOwnerCandidate;
  SemisimpleSubalgebras* owner;
  Selection NilradSel;

  FernandoKacNilradicalCandidate(): indexOwnerCandidate(-1), owner(0){}
  void operator=(const FernandoKacNilradicalCandidate& other)
  { this->indexOwnerCandidate=other.indexOwnerCandidate;
    this->owner=other.owner;
    this->NilradSel=other.NilradSel;
  }
};

class CandidateSSSubalgebra
{
public:
  WeylGroup theWeylNonEmbeddeD;
  WeylGroup theWeylNonEmbeddeDdefaultScale;
  List<Vectors<Rational> > CartanSAsByComponent;
  Vectors<Rational> theCoRoots;
  Vectors<Rational> theHs;
  List<ElementSemisimpleLieAlgebra<Rational> > thePosGens;
  List<ElementSemisimpleLieAlgebra<Rational> > theNegGens;
  List<ElementSemisimpleLieAlgebra<Rational> > theBasis;
  List<ElementSemisimpleLieAlgebra<Polynomial<Rational> > > theUnknownPosGens;
  List<ElementSemisimpleLieAlgebra<Polynomial<Rational> > > theUnknownNegGens;

  List<List<int> > theHorbitIndices;
//  List<List<ElementWeylGroup> > theHWeylGroupElts;
  Vector<Rational> aSolution;
  List<List<ChevalleyGenerator> > theInvolvedPosGenerators;
  List<List<ChevalleyGenerator> > theInvolvedNegGenerators;
  charSSAlgMod<Rational> theCharFundamentalCoordsRelativeToCartan;
  charSSAlgMod<Rational> theCharFundCoords;
  //The highest weights are by definition cartan-centralizer-split
  List<ElementSemisimpleLieAlgebra<Rational> > highestVectorsModules;
  List<Vector<Rational> > highestWeightsCartanCentralizerSplitModules;
  List<Vector<Rational> > highestWeightsModules;
  Vectors<Rational> PosRootsPerpendicularPrecedingWeights;
  Vectors<Rational> CartanOfCentralizer;
  List<Polynomial<Rational> > theSystemToSolve;
  List<Polynomial<Rational> > transformedSystem;
  SemisimpleSubalgebras* owner;
  int indexInOwnersOfNonEmbeddedMe;
  int indexMaxSSContainer;
  List<int> indicesDirectSummandSuperAlgebra;
  FormatExpressions theCoeffLetters;
  bool flagSystemSolved;
  bool flagSystemProvedToHaveNoSolution;
  bool flagSystemGroebnerBasisFound;
  int totalNumUnknowns;
  MemorySaving<List<FernandoKacNilradicalCandidate> > FKNilradicalCandidates;
  List<List<ElementSemisimpleLieAlgebra<Rational> > > highestVectorsGrouppedByType;
  List<List<List<int> > > NilradicalPairingTable;
  charSSAlgMod<Rational> theCharOverCartanPlusCartanCentralizer;

  CandidateSSSubalgebra(): owner(0), indexInOwnersOfNonEmbeddedMe(-1), indexMaxSSContainer(-1),
  flagSystemSolved(false), flagSystemProvedToHaveNoSolution(false),
  flagSystemGroebnerBasisFound(false), totalNumUnknowns(0)
  {}
  void GetHsByType
  (List<List<Vectors<Rational> > >& outputHsByType, List<DynkinSimpleType>& outputTypeList)
  ;
  bool HasConjugateHsTo(List<Vector<Rational> >& other);
  bool IsDirectSummandOf(CandidateSSSubalgebra& other, bool computeImmediateDirectSummandOnly);

  void GetGenericPosGenLinearCombination
  (int indexPosGens, ElementSemisimpleLieAlgebra<Polynomial<Rational> >& output)
;

  void GetGenericNegGenLinearCombination
  (int indexNegGens, ElementSemisimpleLieAlgebra<Polynomial<Rational> >& output)
  ;
  void GetGenericLinearCombination
  (int numVars, int varOffset, List<ChevalleyGenerator>& involvedGens,
   ElementSemisimpleLieAlgebra<Polynomial<Rational> >& output)
  ;
  void GetGenericLinearCombinationInvolvedPosGens
  (int theIndex, ElementSemisimpleLieAlgebra<Polynomial<Rational> >& output)
;
  void AddToSystem
  (const ElementSemisimpleLieAlgebra<Polynomial<Rational> >& elementThatMustVanish)
  ;
  void operator=(const CandidateSSSubalgebra& other)
  { this->CartanSAsByComponent=other.CartanSAsByComponent;
    this->theCharFundamentalCoordsRelativeToCartan=other.theCharFundamentalCoordsRelativeToCartan;
    this->theCharFundCoords=other.theCharFundCoords;
    this->theWeylNonEmbeddeD=other.theWeylNonEmbeddeD;
    this->theWeylNonEmbeddeDdefaultScale=other.theWeylNonEmbeddeDdefaultScale;
    this->PosRootsPerpendicularPrecedingWeights=other.PosRootsPerpendicularPrecedingWeights;
    this->indexInOwnersOfNonEmbeddedMe=other.indexInOwnersOfNonEmbeddedMe;
//    this->theTypeTotal=other.theTypeTotal;
    this->thePosGens=other.thePosGens;
    this->theNegGens=other.theNegGens;
    this->theHorbitIndices=other.theHorbitIndices;
//    this->theHWeylGroupElts=other.theHWeylGroupElts;
    this->theInvolvedPosGenerators=other.theInvolvedPosGenerators;
    this->theInvolvedNegGenerators=other.theInvolvedNegGenerators;
    this->owner=other.owner;
    this->theSystemToSolve=other.theSystemToSolve;
    this->theCoRoots=other.theCoRoots;
    this->theHs=other.theHs;
    this->theCoeffLetters=other.theCoeffLetters;
    this->flagSystemSolved=other.flagSystemSolved;
    this->flagSystemGroebnerBasisFound=other.flagSystemGroebnerBasisFound;
    this->flagSystemProvedToHaveNoSolution=other.flagSystemProvedToHaveNoSolution;
    this->transformedSystem=other.transformedSystem;
    this->aSolution=other.aSolution;
    this->totalNumUnknowns=other.totalNumUnknowns;
    this->theUnknownNegGens=other.theUnknownNegGens;
    this->theUnknownPosGens=other.theUnknownPosGens;
    this->theBasis=other.theBasis;
    this->indexMaxSSContainer=other.indexMaxSSContainer;
    this->indicesDirectSummandSuperAlgebra=other.indicesDirectSummandSuperAlgebra;
    this->highestVectorsModules=other.highestVectorsModules;
    this->CartanOfCentralizer=other.CartanOfCentralizer;
    this->highestWeightsModules=other.highestWeightsModules;
    this->highestWeightsCartanCentralizerSplitModules=other.highestWeightsCartanCentralizerSplitModules;
    this->FKNilradicalCandidates=other.FKNilradicalCandidates;
    this->highestVectorsGrouppedByType=other.highestVectorsGrouppedByType;
    this->NilradicalPairingTable=other.NilradicalPairingTable;
    this->theCharOverCartanPlusCartanCentralizer=other.theCharOverCartanPlusCartanCentralizer;

  }
  bool IsWeightSystemSpaceIndex
(int theIndex, const Vector<Rational>& AmbientRootTestedForWeightSpace);
  void AddTypeIncomplete(const DynkinSimpleType& theNewType);
  void AddHincomplete
  (const Vector<Rational>& theH, const ElementWeylGroup& theWE, int indexOfOrbit)
  ;
  bool CheckInitialization()const;
  SemisimpleLieAlgebra& GetAmbientSS();
  WeylGroup& GetAmbientWeyl();
  void ComputeCartanOfCentralizer(GlobalVariables* theGlobalVariables);
  void ComputeCentralizinglySplitModuleDecomposition(GlobalVariables* theGlobalVariables);
  void ComputeCentralizinglySplitModuleDecompositionLastPart(GlobalVariables* theGlobalVariables);
  void ComputeCentralizinglySplitModuleDecompositionHWVsOnly
  (GlobalVariables* theGlobalVariables,
   HashedList<Vector<Rational> >& inputHws)
   ;
  void ComputeCentralizinglySplitModuleDecompositionWeightsOnly
  (GlobalVariables* theGlobalVariables, HashedList<Vector<Rational> >& outputHWs)
  ;
  bool ComputeSystem
(GlobalVariables* theGlobalVariables)
 ;
  bool ComputeSystemPart2
(GlobalVariables* theGlobalVariables)
 ;
  bool ComputeChar
(GlobalVariables* theGlobalVariables)
  ;
  bool SolveSeparableQuadraticSystemRecursively
  (GlobalVariables* theGlobalVariables)
  ;
  bool isGoodForTheTop
  (WeylGroup& ownerWeyl, const Vector<Rational>& HneW)const
  ;
  std::string ToString(FormatExpressions* theFormat=0)const;
};

class SemisimpleSubalgebras
{
public:
  SemisimpleLieAlgebra* owneR;
  SltwoSubalgebras theSl2s;
  List<SemisimpleLieAlgebra> SimpleComponentsSubalgebras;
  HashedListReferences<SemisimpleLieAlgebra> theSubalgebrasNonEmbedded;
  List<SltwoSubalgebras> theSl2sOfSubalgebras;

  List<CandidateSSSubalgebra> Hcandidates;
  int theRecursionCounter;
  bool flagAttemptToSolveSystems;
  void operator=(const SemisimpleSubalgebras& other)
  { this->owneR=other.owneR;
    this->theSl2s=other.theSl2s;
    this->SimpleComponentsSubalgebras=other.SimpleComponentsSubalgebras;
    this->theSubalgebrasNonEmbedded=other.theSubalgebrasNonEmbedded;
    this->theSl2sOfSubalgebras=other.theSl2sOfSubalgebras;
    this->Hcandidates=other.Hcandidates;
    this->theRecursionCounter=other.theRecursionCounter;
    this->flagAttemptToSolveSystems=other.flagAttemptToSolveSystems;
  }
  bool operator==(const SemisimpleSubalgebras& other)
  { if (this->owneR==other.owneR)
      return true;
    if (this->owneR==0 || other.owneR==0)
    { std::cout << "This is a programming error: comparing non-initialized Semisimple Lie Subalgebras. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    return *this->owneR==*other.owneR;
  }

  SemisimpleLieAlgebra& GetSSowner()
  { if (this->owneR==0)
    { std::cout << "This is a programming error: attempted to access non-initialized "
      << " semisimple Lie subalgerbas. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    return *this->owneR;
  }
  void initHookUpPointers(SemisimpleLieAlgebra& inputOwner)
  { this->owneR=&inputOwner;
    this->theSl2s.owner=&inputOwner;
  }
  void reset();
  SemisimpleSubalgebras()
  { this->reset();
  }
  SemisimpleSubalgebras(SemisimpleLieAlgebra& inputOwner)
  { this->reset();
    this->initHookUpPointers(inputOwner);
  }
  std::string ToString(FormatExpressions* theFormat=0);
  void AddCandidatesSubalgebra
  (CandidateSSSubalgebra& theCandidate, GlobalVariables* theGlobalVariables)
  ;
  void HookUpCentralizers
  ( )
  ;
  void FindAllEmbeddings
  (DynkinSimpleType& theType, SemisimpleLieAlgebra& theOwner, GlobalVariables* theGlobalVariables)
  ;
  void FindTheSSSubalgebras
  (SemisimpleLieAlgebra& newOwner, GlobalVariables* theGlobalVariables)
  ;
  void FindTheSSSubalgebrasPart2
  (GlobalVariables* theGlobalVariables)
  ;
  void ExtendCandidatesRecursive
  (const CandidateSSSubalgebra& baseCandidate, bool propagateRecursion,
   GlobalVariables* theGlobalVariables)
  ;
  void ExtendOneComponentOneTypeAllLengthsRecursive
  (const CandidateSSSubalgebra& baseCandidate, DynkinSimpleType& theType,
   bool propagateRecursion, GlobalVariables* theGlobalVariables)
;
  void ExtendOneComponentRecursive
  (const CandidateSSSubalgebra& baseCandidate, bool propagateRecursion,
   GlobalVariables* theGlobalVariables)
  ;
};


#endif

