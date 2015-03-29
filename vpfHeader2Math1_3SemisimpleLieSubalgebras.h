//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#ifndef vpfHeaderSemisimpleLieSubalgebrasIncluded
#define vpfHeaderSemisimpleLieSubalgebrasIncluded

#include "vpfHeader2Math0_General.h"
#include "vpfHeader2Math1_2SemisimpleLieAlgebras_RootSubalgebras.h"
static ProjectInformationInstance ProjectInfoVpfHeader1_4(__FILE__, "Header, semisimple Lie subalgebras. ");

class SemisimpleSubalgebras;
class CandidateSSSubalgebra;

class NilradicalCandidate
{
  public:
  CandidateSSSubalgebra* owner;
  std::string FKnilradicalLog;
  bool flagNilradicalConesIntersect;
  bool flagNilradicalConesStronglyIntersect;
  bool flagComputedRelativelyStrongIntersections;
  bool flagRestrictedCentralizerConditionHoldS;
  bool flagParabolicACextendsToParabolicAC;

  bool flagLinfiniteRelFound;
  DynkinDiagramRootSubalgebra theLeviDiagramAmbienT, theLeviDiagramSmalL;
  //0->not selected; 1->selected; 2->undecided.
  List<int> theNilradicalSelection;
  Vector<Rational> ConeIntersection;
  Vector<Rational> ConeStrongIntersection;
  Vector<Rational> ConeRelativelyStrongIntersection;
  //  Vector<Rational> LInfiniteRelation;
  Vector<Rational> ConeSeparatingNormal;
  Vectors<Rational> theNilradicalWeights;
  Vectors<Rational> theNonFKhws;
  Vectors<Rational> theNonFKhwsStronglyTwoSided;

  List<int> ownerModulesNilradicalElements;
  List<int> ownerModulestheNonFKhwVectors;

  Vectors<Rational> leviRootsAmbienT;
  Vectors<Rational> leviRootsSmallPrimalFundCoords;

  List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > theNonFKhwVectors;
  List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > theNonFKHVectorsStronglyTwoSided;
  List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > theNilradical;
  List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > theNilradicalElementOpposites;
  Selection theNilradSubsel;
  List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > theNilradicalSubset;
  List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > theNonFKhwVectorsStrongRelativeToSubset;
  Vectors<Rational> theNilradicalSubsetWeights;
  Vectors<Rational> theNonFKhwVectorsStrongRelativeToSubsetWeights;

  NilradicalCandidate():owner(0), flagRestrictedCentralizerConditionHoldS(false), flagParabolicACextendsToParabolicAC(false), flagLinfiniteRelFound(false){}
  void reset();
  void CheckInitialization()const;
  bool IsStronglySingular(int moduleIndex);
  bool IsStronglySingularRelativeToSubset(int nonFKweightIndex);
  Vector<Rational> GetConeStrongIntersectionWeight()const;
  Vector<Rational> GetNilradicalLinearCombi()const;
  void GetModGeneratedByNonHWVandNilradElt
  (int indexInNilradSubset, List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& outputLeft,
   List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& outputRight, List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& outputBrackets)const
  ;
  bool TryFindingLInfiniteRels(GlobalVariables* theGlobalVariables);
  void ComputeParabolicACextendsToParabolicAC(GlobalVariables* theGlobalVariables);
//  bool IsLInfiniteRel(GlobalVariables* theGlobalVariables);
  bool IsCommutingSelectionNilradicalElements(Selection& inputNilradSel);
  void ProcessMe(GlobalVariables* theGlobalVariables);
  std::string ToString(FormatExpressions* theFormat=0)const;
  std::string ToStringTableElementWithWeights(const List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& theElts, const Vectors<Rational>& theWeights)const;
  void ComputeTheTwoCones(GlobalVariables* theGlobalVariables);
  void ComputeTheTwoConesRelativeToNilradicalSubset();
};

class CandidateSSSubalgebra
{
  friend std::ostream& operator << (std::ostream& output, const CandidateSSSubalgebra& theCandidate)
  { output << theCandidate.ToString();
    return output;
  }

public:
  WeylGroup theWeylNonEmbeddeD;
  WeylGroup theWeylNonEmbeddeDdefaultScale;
  DynkinDiagramRootSubalgebra theCentralizerSubDiagram;
  DynkinType theCentralizerType;

  List<Vectors<Rational> > CartanSAsByComponentScaledToActByTwo;
  List<AlgebraicNumber> RatioKillingsByComponent;
  Vectors<Rational> theHsScaledToActByTwo;
  Vectors<Rational> theHs;
  Vectors<Rational> theHsScaledToActByTwoInOrderOfCreation;
  Matrix<Rational> BilinearFormSimplePrimal;
  Matrix<Rational> BilinearFormFundPrimal;
  Matrix<Rational> InducedEmbeddingPrimalFundCoordsIntoSimpleAmbientCoords;
  Matrix<Rational> InducedEmbeddingPrimalFundCoordsIntoFundAmbientCoords;
  Matrix<Rational> MatMultiplyFundCoordsToGetSimple;

  List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > thePosGens;
  List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > theNegGens;
  List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > theBasis;
  List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > fullBasisByModules;
  List<int> fullBasisOwnerModules;
  List<ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> > > theUnknownPosGens;
  List<ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> > > theUnknownNegGens;
  List<ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> > > theUnknownCartanCentralizerBasis;

//  Vector<Rational> aSolution;
  List<List<ChevalleyGenerator> > theInvolvedPosGenerators;
  List<List<ChevalleyGenerator> > theInvolvedNegGenerators;
  charSSAlgMod<Rational> theCharFundamentalCoordsRelativeToCartan;
  charSSAlgMod<Rational> theCharNonPrimalFundCoords;
  charSSAlgMod<Rational> thePrimalChaR;
  Vectors<Rational> PosRootsPerpendicularPrecedingWeights;
  Vectors<Rational> CartanOfCentralizer;
  List<Polynomial<AlgebraicNumber> > theSystemToSolve;
  List<Polynomial<AlgebraicNumber> > transformedSystem;
  SemisimpleSubalgebras* owner;
  int indexInOwner;
  int indexIamInducedFrom;
  List<int> RootInjectionsFromInducer;
  int indexInOwnersOfNonEmbeddedMe;
  int indexHcandidateBeingGrown;
  int indexMaxSSContainer;
  int indexSSPartCentralizer;
  List<int> indicesDirectSummandSuperAlgebra;
  MemorySaving<FormatExpressions> charFormaT;
  bool flagSubalgebraPreloadedButNotVerified;
  bool flagSystemSolved;
  bool flagSystemProvedToHaveNoSolution;
  bool flagSystemGroebnerBasisFound;
  bool flagCentralizerIsWellChosen;
  bool flagUsedInducingSubalgebraRealization;
  int RecursionDepthCounterForNilradicalGeneration;
  int totalNumUnknownsNoCentralizer;
  int totalNumUnknownsWithCentralizer;
  long long int totalArithmeticOpsToSolveSystem;

  List<NilradicalCandidate> FKNilradicalCandidates;
  int NumConeIntersections;
  int NumCasesNoLinfiniteRelationFound;
  int NumBadParabolics;
  int NumCentralizerConditionFailsConeConditionHolds;
  //The highest weight vectors are by definition cartan-centralizer-split
  List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > HighestVectorsNonSorted;
  List<Vector<Rational> > HighestWeightsPrimalNonSorted;
  List<Vector<Rational> > HighestWeightsNONprimalNonSorted;

  List<List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > > HighestVectors;
  HashedList<Vector<Rational> > HighestWeightsPrimal;
  List<Vector<Rational> > HighestWeightsNONPrimal;

  List<List<List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > > > Modules;
  List<List<List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > > > ModulesSl2opposite;
  List<List<ElementSemisimpleLieAlgebra<AlgebraicNumber> > > ModulesIsotypicallyMerged;
//  List<List<ElementSemisimpleLieAlgebra<Rational> > > ModulesSemisimpleSubalgebra;

  List<Vectors<Rational> > WeightsModulesNONprimal;
  List<Vectors<Rational> > WeightsModulesPrimal;
  List<charSSAlgMod<Rational> > CharsPrimalModules;
  List<charSSAlgMod<Rational> > CharsPrimalModulesMerged;
  HashedList<Vector<Rational> > RootSystemCentralizerPrimalCoords;
  HashedList<Vector<Rational> > RootSystemSubalgebraPrimalCoords;

  List<List<List<int> > > NilradicalPairingTable;
  List<int> subalgebraModules;
  List<int> centralizerSubalgebraModules;
  List<int> primalSubalgebraModules;
  List<List<int> > OppositeModulesByStructure;
  List<List<int> > OppositeModulesByChar;

  HashedList<int, MathRoutines::IntUnsignIdentity> modulesWithZeroWeights;
  std::string comments;
  std::string nilradicalGenerationLog;
  Rational centralizerRank;
  Rational centralizerDimension;

  bool flagDeallocated;
  CandidateSSSubalgebra();
  ~CandidateSSSubalgebra()
  { this->flagDeallocated=true;
  }
  void reset(SemisimpleSubalgebras* inputOwner=0);
  bool CheckConsistency()const;
  bool CheckMaximalDominance()const;
  bool ComputeAndVerifyFromGeneratorsAndHs();
  int GetPrimalRank()const;
  int GetRank()const;
  int GetSSpartCentralizerOfSSPartCentralizer()const;
  void GetHsScaledToActByTwoByType(List<List<Vectors<Rational> > >& outputHsByType, List<DynkinSimpleType>& outputTypeList)const;
  bool HasHsScaledByTwoConjugateTo(List<Vector<Rational> >& other)const;
  bool IsDirectSummandOf(const CandidateSSSubalgebra& other);
  void GetGenericCartanCentralizerLinearCombination(int indexCartanCentralizerGen, ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> >& output);
  void GetGenericPosGenLinearCombination(int indexPosGens, ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> >& output);
  bool IsExtremeWeight(int moduleIndex, int indexInIsoComponent)const;
  void GetGenericNegGenLinearCombination(int indexNegGens, ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> >& output);
  bool AmRegularSA()const;
  bool CompareLeftGreaterThanRight(const Vector<Rational>& left, const Vector<Rational>& right);
  void GetGenericLinearCombination(int numVars, int varOffset, List<ChevalleyGenerator>& involvedGens, ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> >& output);
  void GetGenericLinearCombinationInvolvedPosGens(int theIndex, ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> >& output);
  void ComputeCentralizerIsWellChosen();
  void AdjustCentralizerAndRecompute(bool allowNonPolynomialSystemFailure);
  void ComputeRatioKillingsByComponent();
  void AddToSystem(const ElementSemisimpleLieAlgebra<Polynomial<AlgebraicNumber> >& elementThatMustVanish);
  bool CreateAndAddExtendBaseSubalgebra
  (const CandidateSSSubalgebra& baseSubalgebra, Vector<Rational>& newHrescaledToActByTwo, const DynkinType& theNewType, const List<int>& theRootInjection)
  ;
  bool ComputeSltwoDynkinIndices(const DynkinType& input);
  bool GetCentralizerTypeIfComputableAndKnown(const DynkinType& input, DynkinType& output);
  void ComputeHsAndHsScaledToActByTwoFromComponents();
  void SetUpInjectionHs
  (const CandidateSSSubalgebra& baseSubalgebra, const DynkinType& theNewType, const List<int>& theRootInjection,
   Vector<Rational>* newHScaledToActByTwo=0);
  void EnumerateAllNilradicals(GlobalVariables* theGlobalVariables);
  std::string ToStringNilradicalSelection(const List<int>& theSelection);
  void EnumerateNilradicalsRecursively(List<int>& theSelection, GlobalVariables* theGlobalVariables, std::stringstream* logStream=0);
  void ExtendNilradicalSelectionToMultFreeOverSSpartSubalgebra
  (HashedList<int, MathRoutines::IntUnsignIdentity>& inputOutput, GlobalVariables* theGlobalVariables, std::stringstream* logStream);
  bool IsPossibleNilradicalCarryOutSelectionImplications(List<int>& theSelection, GlobalVariables* theGlobalVariables, std::stringstream* logStream=0);
  void ExtendToModule(List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& inputOutput, GlobalVariables* theGlobalVariables);
  Vector<Rational> GetPrimalWeightFirstGen(const ElementSemisimpleLieAlgebra<AlgebraicNumber>& input)const;
  Vector<Rational> GetNonPrimalWeightFirstGen(const ElementSemisimpleLieAlgebra<AlgebraicNumber>& input)const;
  void ComputeKsl2triples(GlobalVariables* theGlobalVariables);
  void ComputeKsl2triplesGetOppositeEltsInOppositeModule
  (const Vector<Rational>& theElementWeight, const List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& inputOppositeModule,
   List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& outputElts);
  void ComputeKsl2triplesGetOppositeEltsAll(const Vector<Rational>& theElementWeight, List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& outputElts);
  bool ComputeKsl2tripleSetUpAndSolveSystem
  (const ElementSemisimpleLieAlgebra<AlgebraicNumber>& theE, const List<ElementSemisimpleLieAlgebra<AlgebraicNumber> >& FisLinearCombiOf,
   ElementSemisimpleLieAlgebra<AlgebraicNumber>& outputF, GlobalVariables* theGlobalVariables);
  void ComputeCharsPrimalModules();
  void ComputePairingTable(GlobalVariables* theGlobalVariables);
  void ComputeSinglePair(int leftIndex, int rightIndex, List<int>& output, GlobalVariables* theGlobalVariables);
  int GetNumModules()const;
  void ComputePairKweightElementAndModule(const ElementSemisimpleLieAlgebra<AlgebraicNumber>& leftKweightElt, int rightIndex, List<int>& output, GlobalVariables* theGlobalVariables);
  bool IsWeightSystemSpaceIndex(int theIndex, const Vector<Rational>& AmbientRootTestedForWeightSpace);
  void AddHincomplete(const Vector<Rational>& theH, const ElementWeylGroup<WeylGroup>& theWE, int indexOfOrbit);
  bool CheckInitialization()const;
  bool CheckModuleDimensions()const;
  SemisimpleLieAlgebra& GetAmbientSS()const;
  WeylGroup& GetAmbientWeyl()const;
  void ComputeCartanOfCentralizer(GlobalVariables* theGlobalVariables);
  void ComputePrimalModuleDecomposition(GlobalVariables* theGlobalVariables);
  void ComputePrimalModuleDecompositionHWsHWVsOnly(GlobalVariables* theGlobalVariables);
  void ComputePrimalModuleDecompositionHWVsOnly(GlobalVariables* theGlobalVariables, HashedList<Vector<Rational> >& inputHws);
  void ComputePrimalModuleDecompositionHighestWeightsOnly(GlobalVariables* theGlobalVariables, HashedList<Vector<Rational> >& outputHWsCoords);
  void ComputePrimalModuleDecompositionHWsHWVsOnlyLastPart(GlobalVariables* theGlobalVariables);
  void GetPrimalWeightProjectionFundCoords(const Vector<Rational>& inputAmbientWeight, Vector<Rational>& output)const;
  bool CheckGensBracketToHs();
  void GetWeightProjectionFundCoords(const Vector<Rational>& inputAmbientweight, Vector<Rational>& output)const;
  bool ComputeSystem(bool AttemptToChooseCentalizer, bool allowNonPolynomialSystemFailure);
  bool ComputeSystemPart2(bool AttemptToChooseCentalizer, bool allowNonPolynomialSystemFailure);
  bool ComputeChar(bool allowBadCharacter);
  bool AttemptToSolveSystem();
  bool IsGoodHnewActingByTwo(const Vector<Rational>& HNewActingByTwo, const List<int>& theRootInjections)const;
  Rational GetScalarSA(const Vector<Rational>& primalWeightLeft, const Vector<Rational>& primalWeightRight)const;
  std::string ToStringTypeAndHs(FormatExpressions* theFormat=0)const;
  std::string ToStringGenerators(FormatExpressions* theFormat=0)const;
  std::string ToString(FormatExpressions* theFormat=0)const;
  std::string ToStringSystem(FormatExpressions* theFormat=0)const;
  std::string ToStringSystemPart2(FormatExpressions* theFormat=0)const;
  std::string ToStringLoadUnknown(FormatExpressions* theFormat=0)const;
  std::string ToStringCentralizer(FormatExpressions* theFormat=0)const;
  std::string ToStringCartanSA(FormatExpressions* theFormat=0)const;
  std::string ToStringPairingTable(FormatExpressions* theFormat=0)const;
  std::string ToStringPairingTableLaTeX(FormatExpressions* theFormat=0)const;
  std::string ToStringNilradicals(FormatExpressions* theFormat=0)const;
  std::string ToStringNilradicalsSummary(FormatExpressions* theFormat=0)const;
  std::string ToStringModuleDecompo(FormatExpressions* theFormat=0)const;
  std::string ToStringModuleDecompoLaTeX(FormatExpressions* theFormat=0)const;
  std::string ToStringDrawWeights(FormatExpressions* theFormat=0)const;
  std::string ToStringDrawWeightsHelper(int indexModule, const Vector<Rational>& theWeight)const;
//  std::string ToStringDrawWeightsVersion2(FormatExpressions* theFormat=0)const;
  bool operator>(const CandidateSSSubalgebra& other)const ;

};

class SemisimpleSubalgebras
{
public:
  GlobalVariables* theGlobalVariables;
  FormatExpressions currentFormat;
  SemisimpleLieAlgebra* owneR;
  AlgebraicClosureRationals* ownerField;
  DynkinType targetDynkinType;
  SltwoSubalgebras theSl2s;
  HashedListReferences<SemisimpleLieAlgebra>* theSubalgebrasNonEmbedded;
  List<List<Rational> > CachedDynkinIndicesSl2subalgebrasSimpleTypes;
  HashedList<DynkinSimpleType> CachedDynkinSimpleTypesWithComputedSl2Subalgebras;
  List<Vectors<Rational> > theOrbits;
  HashedList<Rational> theOrbitHelementLengths;
  HashedList<DynkinSimpleType> theOrbitDynkinIndices;
//  List<HashedList<ElementWeylGroup<WeylGroup> > > theOrbitGeneratingElts;
  List<bool> theOrbitsAreComputed;
  std::string comments;
  std::string fileNameToLogComments;

  //current computation state:
  // state that gets stored to hd:
  List<CandidateSSSubalgebra> currentSubalgebraChain;
  List<int> currentNumLargerTypesExplored;
  List<int> currentNumHcandidatesExplored;
  // state that is recomputed each load:
  List<List<DynkinType> > currentPossibleLargerDynkinTypes;
  List<List<List<int> > > currentRootInjections;
  List<Vectors<Rational> > currentHCandidatesScaledToActByTwo;

  //end current computation state variables.

  HashedList<Vectors<Rational> > theHsOfSubalgebras; //used to search for subalgebras quickly
  List<CandidateSSSubalgebra> theSubalgebras;
  bool flagRealizedAllCandidates;
  bool flagAttemptToSolveSystems;
  bool flagComputePairingTable;
  bool flagComputeModuleDecomposition;
  bool flagComputeNilradicals;
  bool flagProduceLaTeXtables;
  bool flagAttemptToAdjustCentralizers;
  double timeComputationStartInSeconds;
  double timeComputationEndInSeconds;
  signed long long int numAdditions;
  signed long long int numMultiplications;
  bool flagDeallocated;
  std::string DisplayNameMainFile1NoPath;
  std::string DisplayNameMainFile2FastLoadNoPath;
  std::string DisplayNameMainFile1WithPath;
  std::string DisplayNameMainFile2FastLoadWithPath;

  std::string PhysicalNameMainFile1;
  std::string PhysicalNameMainFile2FastLoad;

  std::string (*ToStringExpressionString)(SemisimpleSubalgebras& input);
  bool LoadState
  ( List<int>& currentChainInt,
    List<int>& numExploredTypes,
    List<int>& numExploredHs,
    std::stringstream& reportStream
   )
  ;
  int GetNumPossibleSAs()const;
  int GetDisplayIndexFromActual(int ActualIndexSubalgebra)const;
  void ComputeFolderNames(FormatExpressions& outputFormat);
  void CheckFileWritePermissions();
  void WriteReportToFiles();
  std::string ToStringAlgebraLink(int ActualIndexSubalgebra, FormatExpressions* theFormat)const;
  std::string GetPhysicalFileNameSubalgebra(int ActualIndexSubalgebra, FormatExpressions* theFormat)const;
  std::string GetDisplayFileNameSubalgebraRelative(int ActualIndexSubalgebra, FormatExpressions* theFormat)const;
  std::string GetDisplayFileNameSubalgebraAbsolute(int ActualIndexSubalgebra, FormatExpressions* theFormat)const;
  std::string GetPhysicalFileNameFKFTNilradicals(int ActualIndexSubalgebra, FormatExpressions* theFormat)const;
  std::string GetDisplayFileNameFKFTNilradicals(int ActualIndexSubalgebra, FormatExpressions* theFormat)const;
  const CandidateSSSubalgebra& baseSubalgebra();
  bool operator==(const SemisimpleSubalgebras& other)
  { MacroRegisterFunctionWithName("SemisimpleSubalgebras::operator==");
    if (this->owneR==other.owneR)
      return true;
    if (this->owneR==0 || other.owneR==0)
      crash << "This is a programming error: comparing non-initialized Semisimple Lie Subalgebras. " << crash;
    return *this->owneR==*other.owneR;
  }
  SemisimpleLieAlgebra& GetSSowner()const
  { if (this->owneR==0)
      crash << "This is a programming error: attempted to access non-initialized semisimple Lie subalgerbas. " << crash;
    return *this->owneR;
  }
  bool IncrementReturnFalseIfPastLast();
  bool RemoveLastSubalgebra();
  bool GetCentralizerTypeIfComputableAndKnown(const DynkinType& input, DynkinType& output);
  bool CombinatorialCriteriaAllowRealization();
  bool ComputeCurrentHCandidates();
  void initHookUpPointers
  (SemisimpleLieAlgebra& inputOwner, AlgebraicClosureRationals* theField, HashedListReferences<SemisimpleLieAlgebra>* inputSubalgebrasNonEmbedded,
   ListReferences<SltwoSubalgebras>* inputSl2sOfSubalgebras, GlobalVariables* inputGlobalVariables);
  void reset();
  ~SemisimpleSubalgebras()
  { this->flagDeallocated=true;
  }
  SemisimpleSubalgebras(): flagDeallocated(false)
  { this->reset();
  }
  void AddSubalgebraIfNewSetToStackTop(CandidateSSSubalgebra& input);
  void AddSubalgebraToStack
(CandidateSSSubalgebra& input, int inputNumLargerTypesExplored, int inputNumHcandidatesExplored)
;
  SemisimpleSubalgebras
  (SemisimpleLieAlgebra& inputOwner, AlgebraicClosureRationals* theField, HashedListReferences<SemisimpleLieAlgebra>* inputSubalgebrasNonEmbedded,
   ListReferences<SltwoSubalgebras>* inputSl2sOfSubalgebras, GlobalVariables* inputGlobalVariables): flagDeallocated(false)
  { this->reset();
    this->initHookUpPointers(inputOwner, theField, inputSubalgebrasNonEmbedded, inputSl2sOfSubalgebras, inputGlobalVariables);
  }
  bool CheckConsistencyHs()const;
  bool CheckConsistency()const;
  bool CheckInitialization() const;
  std::string ToStringState(FormatExpressions* theFormat=0);
  std::string ToStringCurrentChain(FormatExpressions* theFormat=0);
  std::string ToStringProgressReport(FormatExpressions* theFormat=0);
  std::string ToString(FormatExpressions* theFormat=0);
  std::string ToStringSSsumaryLaTeX(FormatExpressions* theFormat=0)const;
  std::string ToStringSSsumaryHTML(FormatExpressions* theFormat=0)const;
  void ComputePairingTablesAndFKFTtypes();
  void GetCentralizerChains(List<List<int> >& outputChains);
  const Vectors<Rational>& GetOrbitSl2Helement(int indexSl2);
  int GetIndexFullSubalgebra()const;
//  const HashedList<ElementWeylGroup<WeylGroup> >& GetOrbitSl2HelementWeylGroupElt(int indexSl2);
  bool RanksAndIndicesFit(const DynkinType& input)const;
  bool GrowDynkinType(const DynkinType& input, List<DynkinType>& output, List<List<int> >* outputImagesSimpleRoots)const;
  Vector<Rational>
  GetHighestWeightFundNewComponentFromImagesOldSimpleRootsAndNewRoot
  (const DynkinType& input, const List<int>& imagesOldSimpleRootsAndNewRoot, CandidateSSSubalgebra& theSSSubalgebraToBeModified);
  void RegisterPossibleCandidate(CandidateSSSubalgebra& theCandidate);
  void HookUpCentralizers(bool allowNonPolynomialSystemFailure);
  void ComputeSl2sInitOrbitsForComputationOnDemand();
  void FindAllEmbeddings(DynkinSimpleType& theType, SemisimpleLieAlgebra& theOwner);
  bool FindTheSSSubalgebrasFromScratch(SemisimpleLieAlgebra& newOwner, const DynkinType* targetType=0);
  void FindTheSSSubalgebrasInit();
  bool FindTheSSSubalgebrasContinue();

  void GetHCandidates
(Vectors<Rational>& outputHCandidatesScaledToActByTwo, CandidateSSSubalgebra& newCandidate,
 DynkinType& currentType, List<int>& currentRootInjection)
  ;

};


#endif


