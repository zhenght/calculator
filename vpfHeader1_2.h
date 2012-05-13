//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#ifndef vpfHeader1_2_h_already_included
#define vpfHeader1_2_h_already_included

#include "vpfHeader1.h"
#ifndef ProjectInfoVpfHeader1_2AlreadyDefined
#define ProjectInfoVpfHeader1_2AlreadyDefined
static ProjectInformationInstance ProjectInfoVpfHeader1_2(__FILE__, "Header file containing c++ math routines. ");
#endif

class Lattice
{
  void TestGaussianEliminationEuclideanDomainRationals(Matrix<Rational> & output);
public:
  inline static const std::string GetXMLClassName(){ return "Lattice";}
  Matrix<Rational>  basisRationalForm;
  Matrix<LargeInt> basis;
  LargeIntUnsigned Den;
  int GetDim()const{return this->basis.NumCols;}
  int GetRank()const{return this->basis.NumRows;}
  void IntersectWith(const Lattice& other);
  bool FindOnePreimageInLatticeOf
    (const Matrix<Rational> & theLinearMap, const Vectors<Rational>& input, Vectors<Rational>& output, GlobalVariables& theGlobalVariables)
;
  void IntersectWithPreimageOfLattice
  (const Matrix<Rational> & theLinearMap, const Lattice& other, GlobalVariables& theGlobalVariables)
;
  void IntersectWithLineGivenBy(Vector<Rational> & inputLine, Vector<Rational> & outputGenerator);
  static bool GetClosestPointInDirectionOfTheNormalToAffineWallMovingIntegralStepsInDirection
  (Vector<Rational> & startingPoint, Vector<Rational> & theAffineHyperplane, Vector<Rational> & theDirection, Vector<Rational> & outputPoint)
  ;
  void GetDefaultFundamentalDomainInternalPoint(Vector<Rational> & output);
  bool GetInternalPointInConeForSomeFundamentalDomain
(Vector<Rational> & output, Cone& coneContainingOutputPoint, GlobalVariables& theGlobalVariables)
  ;
  void GetRootOnLatticeSmallestPositiveProportionalTo
(Vector<Rational> & input, Vector<Rational> & output, GlobalVariables& theGlobalVariables)
  ;
  void GetRougherLatticeFromAffineHyperplaneDirectionAndLattice
  (const Vector<Rational> & theDirection, Vector<Rational> & outputDirectionMultipleOnLattice, Vector<Rational> & theShift, Vector<Rational> & theAffineHyperplane,
   Vectors<Rational>& outputRepresentatives,
   Vectors<Rational>& movementInDirectionPerRepresentative,
   Lattice& outputRougherLattice,
   GlobalVariables& theGlobalVariables)
     ;
  void ApplyLinearMap
  (Matrix<Rational> & theMap, Lattice& output)
  ;
  void IntersectWithBothOfMaxRank(const Lattice& other);
  void GetDualLattice(Lattice& output)const;
  bool IsInLattice(const Vector<Rational>& theVector)const
  { Vector<Rational> tempVect=theVector;
    if (!this->ReduceVector(tempVect))
      return false;
    return tempVect.IsEqualToZero();
  }
  //returns false if the vector is not in the vector space spanned by the lattice
  bool ReduceVector(Vector<Rational>& theVector) const;
  //In the following two functions, the format of the matrix theSub of the substitution is as follows.
  //Let the ambient dimension be n, and the coordinates be x_1,..., x_n.
  //Let the new vector space be of dimension m, with coordinates y_1,..., y_m.
  //Then theSub is an n by m matrix, where the i^th row of the matrix gives the expression of x_i via the y_j's.
  //In addition, we require that n>=m (otherwise, in general, we do not expect to get a lattice).
  //For example, if we want to carry out the substitution
  //x_1=y_1+y_2, x_2=y_1-y_2, x_3=y_1, then
  //theSub should be initialized as:
  //1  1
  //1 -1
  //1  0
  bool SubstitutionHomogeneous
    (const Matrix<Rational> & theSub, GlobalVariables& theGlobalVariables)
;
  bool SubstitutionHomogeneous
    (const PolynomialSubstitution<Rational>& theSub, GlobalVariables& theGlobalVariables)
;
//the following function follows the same convention as the preceding except that we allow n<m. However,
// in order to assure that the preimage of the lattice is a lattice,
//we provide as input an ambient lattice in the new vector space of dimension m
  bool SubstitutionHomogeneous
    (const Matrix<Rational> & theSub, Lattice& resultIsSubsetOf, GlobalVariables& theGlobalVariables)
;
  void Reduce
  ()
  ;
  void IntersectWithLinearSubspaceSpannedBy(const Vectors<Rational>& theSubspaceBasis);
  void IntersectWithLinearSubspaceGivenByNormals(const Vectors<Rational>& theSubspaceNormals);
  void IntersectWithLinearSubspaceGivenByNormal(const Vector<Rational> & theNormal);
static bool GetHomogeneousSubMatFromSubIgnoreConstantTerms
(const PolynomialSubstitution<Rational>& theSub, Matrix<Rational> & output, GlobalVariables& theGlobalVariables)
;
  //returning false means that the lattice given as rougher is not actually rougher than the current lattice
  //or that there are too many representatives
  bool GetAllRepresentatives
  (const Lattice& rougherLattice, Vectors<Rational>& output)const
  ;
  bool GetAllRepresentativesProjectingDownTo
  (const Lattice& rougherLattice, Vectors<Rational>& startingShifts, Vectors<Rational>& output)const
  ;
  inline std::string ElementToString()const{return this->ElementToString(true, false);}
  std::string ElementToString(bool useHtml, bool useLatex)const;
  bool operator==(const Lattice& other){return this->basisRationalForm==other.basisRationalForm;}
  void operator=(const Lattice& other)
  { this->basis=other.basis;
    this->Den=other.Den;
    this->basisRationalForm=other.basisRationalForm;
  }
  void WriteToFile
  (std::fstream& output, GlobalVariables* theGlobalVariables)
  ;
  bool ReadFromFile
  (std::fstream& input, GlobalVariables* theGlobalVariables)
  ;
  void MakeZn(int theDim);
  void RefineByOtherLattice(const Lattice& other);
  void MakeFromRoots
  (const Vectors<Rational>& input)
  ;
  Lattice(){}
  Lattice(const Lattice& other){this->operator=(other);}
  void MakeFromMat
  (const Matrix<Rational> & input)
  ;
};

class QuasiPolynomial
{
public:
  inline static std::string GetXMLClassName(){ return "Quasipolynomial";}
  int GetNumVars()const{return this->AmbientLatticeReduced.basis.NumRows;}
  GlobalVariables* buffers;
  Lattice AmbientLatticeReduced;
  Vectors<Rational> LatticeShifts;
  std::string DebugString;
  List<Polynomial<Rational> > valueOnEachLatticeShift;
  std::string ElementToString(bool useHtml, bool useLatex){return this->ElementToString(useHtml, useLatex, 0);}
  std::string ElementToString(bool useHtml, bool useLatex, FormatExpressions* thePolyFormat);
  void ComputeDebugString(){this->DebugString=this->ElementToString(false, false);}
  Rational Evaluate(const Vector<Rational> & input);
  void AddLatticeShift(const Polynomial<Rational> & input, const Vector<Rational> & inputShift);
  void AddAssumingLatticeIsSame(const QuasiPolynomial& other);
  void MakeRougherLattice(const Lattice& latticeToRoughenBy);
  void MakeFromPolyShiftAndLattice(const Polynomial<Rational> & inputPoly, const Vector<Rational> & theShift, const Lattice& theLattice, GlobalVariables& theGlobalVariables);
  void MakeZeroLatTiceZn(int theDim);
  void MakeZeroOverLattice(Lattice& theLattice);
//  bool ExtractLinearMapAndTranslationFromSub
//  ()
 // ;
  bool IsEqualToZero()const {return this->valueOnEachLatticeShift.size==0;}
  void Substitution
  (const Matrix<Rational> & mapFromNewSpaceToOldSpace, const Vector<Rational> & inputTranslationSubtractedFromArgument,
   const Lattice& ambientLatticeNewSpace, QuasiPolynomial& output, GlobalVariables& theGlobalVariables)
  ;
  void Substitution
  (const Matrix<Rational> & mapFromNewSpaceToOldSpace,
   const Lattice& ambientLatticeNewSpace, QuasiPolynomial& output, GlobalVariables& theGlobalVariables)
  ;
  void Substitution
  (const Vector<Rational> & inputTranslationSubtractedFromArgument,
   QuasiPolynomial& output, GlobalVariables& theGlobalVariables)
  ;
  bool SubstitutionLessVariables
  (const PolynomialSubstitution<Rational>& theSub, QuasiPolynomial& output, GlobalVariables& theGlobalVariables)const
  ;
  void operator+=(const QuasiPolynomial& other);
  QuasiPolynomial(){this->buffers=0;}
  void WriteToFile(std::fstream& output, GlobalVariables* theGlobalVariables);
  bool ReadFromFile(std::fstream& input, GlobalVariables* theGlobalVariables);
  QuasiPolynomial(const QuasiPolynomial& other){this->operator=(other);}
  void operator*=(const Rational& theConst);
  void operator=(const QuasiPolynomial& other)
  { this->AmbientLatticeReduced=other.AmbientLatticeReduced;
    this->LatticeShifts=other.LatticeShifts;
    this->valueOnEachLatticeShift=other.valueOnEachLatticeShift;
    this->buffers=other.buffers;
  }

};

class partFraction: ListLight<oneFracWithMultiplicitiesAndElongations>
{
private:
  void findPivot();
  void findInitialPivot();
  //void intRootToString(std::stringstream& out, int* TheRoot, bool MinusInExponent);
  bool rootIsInFractionCone (partFractions& owner, Vector<Rational> * theRoot, GlobalVariables& theGlobalVariables);
  friend class partFractions;
  friend class partFractionPolynomialSubstitution;
public:
  std::string DebugString;
  int LastDistinguishedIndex;
  int FileStoragePosition;
  bool PowerSeriesCoefficientIsComputed;
  bool IsIrrelevant;
  bool RelevanceIsComputed;
  List<int> IndicesNonZeroMults;
  bool RemoveRedundantShortRootsClassicalRootSystem
(partFractions& owner, Vector<Rational>* Indicator, Polynomial<LargeInt>& buffer1, int theDimension, GlobalVariables& theGlobalVariables)
  ;
  bool RemoveRedundantShortRoots(partFractions& owner, Vector<Rational> * Indicator, GlobalVariables& theGlobalVariables, int theDimension);
  bool AlreadyAccountedForInGUIDisplay;
  static bool flagAnErrorHasOccurredTimeToPanic;
//  static  bool flagUsingPrecomputedOrlikSolomonBases;
  static std::fstream TheBigDump;
  static bool UseGlobalCollector;
  static bool MakingConsistencyCheck;
  static Rational CheckSum, CheckSum2;
  static Vector<Rational> theVectorToBePartitioned;
//  static ListPointers<partFraction> GlobalCollectorPartFraction;
  void ComputePolyCorrespondingToOneMonomial
  (QuasiPolynomial& outputQP, MonomialP& theMon, Vectors<Rational>& normals,
   Lattice& theLattice, GlobalVariables& theGlobalVariables)
  ;
  static void EvaluateIntPoly
  (const Polynomial<LargeInt>& input, const Vector<Rational> & values, Rational& output)
  ;
  static void MakePolynomialFromOneNormal(Vector<Rational> & normal, Vector<Rational> & shiftRational, int theMult,  Polynomial<Rational> & output);
  void ComputeNormals
(partFractions& owner, Vectors<Rational>& output, int theDimension, Matrix<Rational>& buffer)
  ;
  int ComputeGainingMultiplicityIndexInLinearRelation
(bool flagUsingOrlikSolomon, Matrix<Rational> & theLinearRelation)
  ;
  void GetRootsFromDenominator
  (partFractions& owner, Vectors<Rational>& output)
;
  void GetVectorPartitionFunction
  (partFractions& owner, Polynomial<LargeInt>& theCoeff, QuasiPolynomial& output, GlobalVariables& theGlobalVariables)
  ;
  LargeInt EvaluateIntPolyAtOne(Polynomial<LargeInt>& input);
  //void InsertNewRootIndex(int index);
  //void MultiplyMinusShiftBy (int* theRoot, int Multiplicity);
  void MultiplyCoeffBy(Rational& r);
  void decomposeAMinusNB
(int indexA, int indexB, int n, int indexAminusNB, MonomialCollection<partFraction, Polynomial<LargeInt> >& output,
 GlobalVariables& theGlobalVariables, Vector<Rational>* Indicator, partFractions& owner)
   ;
  bool DecomposeFromLinRelation
(Matrix<Rational> & theLinearRelation, MonomialCollection<partFraction, Polynomial<LargeInt> >& output,
 Vector<Rational>* Indicator, GlobalVariables& theGlobalVariables, bool flagUsingOSbasis, List<Vector<Rational> >& startingVectors)
   ;
  void ComputeOneCheckSuM
(partFractions& owner, Rational& output, int theDimension, GlobalVariables& theGlobalVariables)
  ;
  bool ReduceMeOnce
(const Polynomial<LargeInt>& myCoeff, Polynomial<LargeInt>& outputCoeff, GlobalVariables& theGlobalVariables,
 Vector<Rational>* Indicator, Vectors<Rational>& startingVectors)
  ;
  void ReduceMonomialByMonomial(partFractions& owner, int myIndex, GlobalVariables& theGlobalVariables, Vector<Rational> * Indicator);
  void ApplySzenesVergneFormulA
(List<Vector<Rational> >& startingVectors,  List<int>& theSelectedIndices, List<int>& theElongations, int GainingMultiplicityIndex,
 int ElongationGainingMultiplicityIndex, MonomialCollection<partFraction, Polynomial<LargeInt> >& output,
 GlobalVariables& theGlobalVariables, Vector<Rational> * Indicator)
  ;
  void ApplyGeneralizedSzenesVergneFormulA
(List<int>& theSelectedIndices, List<int>& theGreatestElongations, List<int>& theCoefficients,
 int GainingMultiplicityIndex, int ElongationGainingMultiplicityIndex,
 MonomialCollection<partFraction, Polynomial<LargeInt> >& output,
 GlobalVariables& theGlobalVariables, Vector<Rational>* Indicator,
 List<Vector<Rational> >& startingVectors)
   ;
  bool CheckForOrlikSolomonAdmissibility(List<int>& theSelectedIndices);
  bool reduceOnceTotalOrderMethod
(MonomialCollection<partFraction, Polynomial<LargeInt> >& output, GlobalVariables& theGlobalVariables,
 Vector<Rational>* Indicator, partFractions& owner)
   ;
//  void reduceOnceOrlikSolomonBasis(partFractions&Accum);
  bool reduceOnceGeneralMethodNoOSBasis
(partFractions& owner, MonomialCollection<partFraction, Polynomial<LargeInt> >& output, GlobalVariables& theGlobalVariables,
 Vector<Rational>* Indicator, Vectors<Rational>& bufferVectors, Matrix<Rational>& bufferMat)
   ;
  bool ReduceOnceGeneralMethod
(partFractions& owner, MonomialCollection<partFraction, Polynomial<LargeInt> >& output, GlobalVariables& theGlobalVariables,
 Vector<Rational>* Indicator, Vectors<Rational>& bufferVectors, Matrix<Rational>& bufferMat)
  ;
  bool AreEqual(partFraction& p);
  bool IsReduced();
  int HashFunction() const;
  static inline int HashFunction(const partFraction& input)
  { return input.HashFunction();
  }
  int MultiplyByOneFrac(oneFracWithMultiplicitiesAndElongations& f);
  void init(int numRoots);
  //int Elongate(int indexElongatedFraction, int theElongation);
  void ComputeIndicesNonZeroMults();
  bool DecreasePowerOneFrac(int index, int increment);
  //void GetNumerator(Polynomial<Rational> & output);
  //void SetNumerator(Polynomial<Rational> & input);
  void PrepareFraction
(int indexA, int indexB, int AminusNBindex, bool indexAisNullified, partFraction& output,
 Polynomial<LargeInt>& AminusNbetaPoly, Polynomial<LargeInt>& outputCommonCoeff)
  ;
  void Assign(const partFraction& p);
  void AssignDenominatorOnly(const partFraction& p);
  void AssignNoIndicesNonZeroMults(partFraction& p);
  int getSmallestNonZeroIndexGreaterThanOrEqualTo(partFractions& owner, int minIndex);
  int ControlLineSizeFracs(std::string& output, FormatExpressions& PolyFormatLocal);
  int ControlLineSizeStringPolys(std::string& output, FormatExpressions& PolyFormatLocal);
  //void swap(int indexA, int indexB);
  partFraction();
  ~partFraction();
  void GetPolyReduceMonomialByMonomial(partFractions& owner, GlobalVariables& theGlobalVariables, Vector<Rational>& theExponent, int StartMonomialPower, int DenPowerReduction, int startDenominatorPower, Polynomial<LargeInt>& output);
  void ReduceMonomialByMonomialModifyOneMonomial
(partFractions& Accum, GlobalVariables& theGlobalVariables, SelectionWithDifferentMaxMultiplicities& thePowers,
 List<int>& thePowersSigned, MonomialP& input, LargeInt& inputCoeff)
  ;
  void GetAlphaMinusNBetaPoly
  (partFractions& owner, int indexA, int indexB, int n, Polynomial<LargeInt>& output)
  ;
  void GetNElongationPolyWithMonomialContribution
(List<Vector<Rational> >& startingVectors, List<int>& theSelectedIndices, List<int>& theCoefficients,
 List<int>& theGreatestElongations, int theIndex, Polynomial<LargeInt>& output, int theDimension)
  ;
  void GetNElongationPoly
  (List<Vector<Rational> >& startingVectors, int index, int baseElongation, int LengthOfGeometricSeries,
   Polynomial<LargeInt>& output, int theDimension)
  ;
  static void GetNElongationPoly(Vector<Rational>& exponent, int n, Polynomial<LargeInt>& output, int theDimension);
  int GetNumProportionalVectorsClassicalRootSystems(partFractions& owner);
  bool operator==(const partFraction& right);
  void operator=(const partFraction& right);
  bool initFromRoots(partFractions& owner, Vectors<Rational>& input);
  std::string ElementToString
(partFractions& owner, bool LatexFormat, bool includeVPsummand, bool includeNumerator,
 FormatExpressions& PolyFormatLocal, GlobalVariables& theGlobalVariables, int& NumLinesUsed)
   ;
  void ReadFromFile(partFractions& owner, std::fstream& input, GlobalVariables* theGlobalVariables, int theDimension);
  void WriteToFile(std::fstream& output, GlobalVariables* theGlobalVariables);
  int SizeWithoutDebugString();
};

class Cone
{
  void ComputeVerticesFromNormalsNoFakeVertices(GlobalVariables& theGlobalVariables);
  bool EliminateFakeNormalsUsingVertices(int theDiM, int numAddedFakeWalls, GlobalVariables& theGlobalVariables);
public:
  inline static const std::string GetXMLClassName(){ return "Cone";}
  bool flagIsTheZeroCone;
  Vectors<Rational> Vertices;
  Vectors<Rational> Normals;
//  bool flagHasSufficientlyManyVertices;
  int LowestIndexNotCheckedForChopping;
  int LowestIndexNotCheckedForSlicingInDirection;
  std::string ElementToString(FormatExpressions* theFormat=0){return this->ElementToString(false, false, *theFormat);}
  void TransformToWeylProjective
  (ConeComplex& owner, GlobalVariables& theGlobalVariables)
  ;
  std::string ElementToString(bool useLatex, bool useHtml, FormatExpressions& theFormat){return this->ElementToString(useLatex, useHtml, false, false, theFormat);}
  std::string ElementToString(bool useLatex, bool useHtml, bool PrepareMathReport, bool lastVarIsConstant, FormatExpressions& theFormat);
  std::string DrawMeToHtmlProjective(DrawingVariables& theDrawingVariables, FormatExpressions& theFormat);
  std::string DrawMeToHtmlLastCoordAffine(DrawingVariables& theDrawingVariables, FormatExpressions& theFormat);
  void TranslateMeMyLastCoordinateAffinization(Vector<Rational> & theTranslationVector);
  bool IsAnHonest1DEdgeAffine(Vector<Rational> & vertex1, Vector<Rational> & vertex2)
  { int numCommonWalls=0;
    for (int i=0; i<this->Normals.size; i++)
      if(vertex1.ScalarEuclidean(this->Normals[i]).IsEqualToZero() && vertex2.ScalarEuclidean(this->Normals[i]).IsEqualToZero())
      { numCommonWalls++;
        if (numCommonWalls==this->GetDim()-2)
          return true;
      }
    return false;
  }
  bool IsTheEntireSpace()
  { return this->Normals.size==0 && this->flagIsTheZeroCone;
  }
  bool IsAnHonest1DEdgeAffine(int vertexIndex1, int vertexIndex2)
  { Vector<Rational> & vertex1=this->Vertices[vertexIndex1];
    Vector<Rational> & vertex2=this->Vertices[vertexIndex2];
    return this->IsAnHonest1DEdgeAffine(vertex1, vertex2);
  }
  bool DrawMeLastCoordAffine
  (bool InitDrawVars, DrawingVariables& theDrawingVariables, FormatExpressions& theFormat,
   int ChamberWallColor=0);
  bool DrawMeProjective
(Vector<Rational> * coordCenterTranslation, bool initTheDrawVars, DrawingVariables& theDrawingVariables, FormatExpressions& theFormat)
  ;
  bool IsInCone(const Vector<Rational>& point) const;
  bool IsInCone(const Vectors<Rational>& vertices)const
  { for (int i=0; i<vertices.size; i++)
      if (!this->IsInCone(vertices[i]))
        return false;
    return true;
  }
  bool GetLatticePointsInCone
  (Lattice& theLattice, Vector<Rational> & theShift, int upperBoundPointsInEachDim, bool lastCoordinateIsOne,
   Vectors<Rational>& outputPoints, Vector<Rational> * shiftAllPointsBy)
  ;
  bool MakeConvexHullOfMeAnd(const Cone& other, GlobalVariables& theGlobalVariables);
  void ChangeBasis
  (Matrix<Rational> & theLinearMap, GlobalVariables& theGlobalVariables)
    ;
  std::string DebugString;
  int GetDim()
  { if (this->Normals.size==0)
      return 0;
    return this->Normals.TheObjects[0].size;
  }
  void SliceInDirection
  (Vector<Rational> & theDirection, ConeComplex& output, GlobalVariables& theGlobalVariables )
;
  bool CreateFromNormalS
  (Vectors<Rational>& inputNormals, bool UseWithExtremeMathCautionAssumeConeHasSufficientlyManyProjectiveVertices, GlobalVariables& theGlobalVariables)
  ;
  //returns false if the cone is non-proper, i.e. when either
  //1) the cone is empty or is of smaller dimension than it should be
  //2) the resulting cone is the entire space
  bool CreateFromNormals
  (Vectors<Rational>& inputNormals, GlobalVariables& theGlobalVariables)
  { return this->CreateFromNormalS(inputNormals, false, theGlobalVariables);
  }
  bool CreateFromVertices
  (Vectors<Rational>& inputVertices, GlobalVariables& theGlobalVariables)
  ;
  void GetInternalPoint(Vector<Rational> & output)
  { if (this->Vertices.size<=0)
      return;
    this->Vertices.sum(output, this->Vertices[0].size);
  }
  Vector<Rational>  GetInternalPoint(){Vector<Rational>  result; this->GetInternalPoint(result); return result;}
  int HashFunction()const
  { return this->Vertices.HashFunction();
  }
  static inline int HashFunction(const Cone& input)
  { return input.HashFunction();
  }
  bool ProduceNormalFromTwoNormalsAndSlicingDirection
  (Vector<Rational> & SlicingDirection, Vector<Rational> & normal1, Vector<Rational> & normal2, Vector<Rational> & output)
  ;
  bool ReadFromFile
  (std::fstream& output, GlobalVariables* theGlobalVariables)
;
  void WriteToFile
  (std::fstream& output, GlobalVariables* theGlobalVariables)
;
  bool ReadFromFile
  (std::fstream& input, Vectors<Rational>& buffer, GlobalVariables* theGlobalVariables)
;
  void operator=(const Cone& other)
  { //this->flagHasSufficientlyManyVertices=other.flagHasSufficientlyManyVertices;
    this->flagIsTheZeroCone=other.flagIsTheZeroCone;
    this->Vertices=other.Vertices;
    this->Normals=other.Normals;
    this->LowestIndexNotCheckedForSlicingInDirection=other.LowestIndexNotCheckedForSlicingInDirection;
    this->LowestIndexNotCheckedForChopping=other.LowestIndexNotCheckedForChopping;
  }
  Cone(const Cone& other){ this->operator=(other);}
  Cone()
  { this->LowestIndexNotCheckedForSlicingInDirection=0;
    this->LowestIndexNotCheckedForChopping=0;
    this->flagIsTheZeroCone=true;
    //this->flagHasSufficientlyManyVertices=true;
  }
  void IntersectAHyperplane
  (Vector<Rational> & theNormal, Cone& outputConeLowerDim, GlobalVariables& theGlobalVariables)
  ;
  bool GetRootFromLPolyConstantTermGoesToLastVariable
  (Polynomial<Rational> & inputLPoly, Vector<Rational> & output)
  ;
  bool SolveLPolyEqualsZeroIAmProjective
  ( Polynomial<Rational> & inputLPoly,
   Cone& outputCone, GlobalVariables& theGlobalVariables
   )
  ;
  bool SolveLQuasiPolyEqualsZeroIAmProjective
  (QuasiPolynomial& inputLQP,
   List<Cone>& outputConesOverEachLatticeShift, GlobalVariables& theGlobalVariables
   )
  ;
  bool operator>(const Cone& other)const
  { return this->Normals>other.Normals;
  }
  bool operator==(const Cone& other)const
  { return this->flagIsTheZeroCone==other.flagIsTheZeroCone && this->Normals==other.Normals;
  }
};

class ConeLatticeAndShift
{
  public:
  inline static std::string GetXMLClassName(){ return "ConeLatticeShift";}
  Cone theProjectivizedCone;
  Lattice theLattice;
  Vector<Rational>  theShift;
  void FindExtremaInDirectionOverLatticeOneNonParam
(Vector<Rational> & theLPToMaximizeAffine, Vectors<Rational>& outputAppendLPToMaximizeAffine,
 List<ConeLatticeAndShift>& outputAppend,
 GlobalVariables& theGlobalVariables)
       ;
  void operator=(const ConeLatticeAndShift& other)
  { this->theProjectivizedCone=other.theProjectivizedCone;
    this->theLattice=other.theLattice;
    this->theShift=other.theShift;
  }
  void WriteToFile
  (std::fstream& output, GlobalVariables* theGlobalVariables)
  ;
  void FindExtremaInDirectionOverLatticeOneNonParamDegenerateCase
(Vector<Rational> & theLPToMaximizeAffine, Vectors<Rational>& outputAppendLPToMaximizeAffine,
 List<ConeLatticeAndShift>& outputAppend, Matrix<Rational> & theProjectionLatticeLevel,
 GlobalVariables& theGlobalVariables)
 ;
  bool ReadFromFile
  (std::fstream& input, GlobalVariables* theGlobalVariables)
  ;
  std::string ElementToString(FormatExpressions& theFormat);
  int GetDimProjectivized(){return this->theProjectivizedCone.GetDim();}
  int GetDimAffine(){return this->theProjectivizedCone.GetDim()-1;}
};

class ConeComplex : public HashedList<Cone>
{
public:
  bool flagChambersHaveTooFewVertices;
  bool flagIsRefined;
//  List<int> NonRefinedChambers;
  int indexLowestNonRefinedChamber;
  HashedList<Vector<Rational> > splittingNormals;
  Vectors<Rational> slicingDirections;
  Cone ConvexHull;
  std::string DebugString;
  void RefineOneStep(GlobalVariables& theGlobalVariables);
  void Refine(GlobalVariables& theGlobalVariables);
  void RefineMakeCommonRefinement(const ConeComplex& other, GlobalVariables& theGlobalVariables);
  void Sort(GlobalVariables& theGlobalVariables);
  void RefineAndSort(GlobalVariables& theGlobalVariables);
  void FindMaxmumOverNonDisjointChambers
    (Vectors<Rational>& theMaximaOverEachChamber, Vectors<Rational>& outputMaxima, GlobalVariables& theGlobalVariables)
    ;
  void MakeAffineAndTransformToProjectiveDimPlusOne
  (Vector<Rational> & affinePoint, ConeComplex& output, GlobalVariables& theGlobalVariables)
  ;
  void TransformToWeylProjective
  (GlobalVariables& theGlobalVariables)
  ;
  int GetDim(){if (this->size<=0) return -1; return this->TheObjects[0].GetDim();}
  bool AddNonRefinedChamberOnTopNoRepetition(Cone& newCone, GlobalVariables& theGlobalVariables);
  void PopChamberSwapWithLast(int index);
  void GetAllWallsConesNoOrientationNoRepetitionNoSplittingNormals
    (Vectors<Rational>& output)const
  ;
  bool DrawMeLastCoordAffine(bool InitDrawVars, DrawingVariables& theDrawingVariables, FormatExpressions& theFormat);
  void TranslateMeMyLastCoordinateAffinization(Vector<Rational> & theTranslationVector, GlobalVariables& theGlobalVariables);
  void InitFromDirectionsAndRefine
  (Vectors<Rational>& inputVectors, GlobalVariables& theGlobalVariables)
  ;
  void InitFromAffineDirectionsAndRefine
  (Vectors<Rational>& inputDirections, Vectors<Rational>& inputAffinePoints, GlobalVariables& theGlobalVariables)
  ;
  std::string DrawMeToHtmlLastCoordAffine
(DrawingVariables& theDrawingVariables, FormatExpressions& theFormat)
;
  bool DrawMeProjective
(Vector<Rational> * coordCenterTranslation, bool InitDrawVars, DrawingVariables& theDrawingVariables, FormatExpressions& theFormat)
  ;
  std::string DrawMeToHtmlProjective
(DrawingVariables& theDrawingVariables, FormatExpressions& theFormat)
;
  std::string ElementToString(){return this->ElementToString(false, false);}
  std::string ElementToString(bool useLatex, bool useHtml);
  void ComputeDebugString(){this->DebugString=this->ElementToString();}
  int GetLowestIndexchamberContaining(const Vector<Rational> & theRoot)const
  { for (int i=0; i<this->size; i++)
      if (this->TheObjects[i].IsInCone(theRoot))
        return i;
    return -1;
  }
  bool findMaxLFOverConeProjective
  (Cone& input, List<Polynomial<Rational> >& inputLinPolys, List<int>& outputMaximumOverEeachSubChamber, GlobalVariables& theGlobalVariables)
  ;
  bool findMaxLFOverConeProjective
  (Cone& input, Vectors<Rational>& inputLFsLastCoordConst,
   List<int>& outputMaximumOverEeachSubChamber,
   GlobalVariables& theGlobalVariables)
  ;
  void initFromCones
(List<Vectors<Rational> >& NormalsOfCones, bool UseWithExtremeMathCautionAssumeConeHasSufficientlyManyProjectiveVertices, GlobalVariables& theGlobalVariables)
  ;
  void initFromCones
(List<Cone>& NormalsOfCones, bool UseWithExtremeMathCautionAssumeConeHasSufficientlyManyProjectiveVertices, GlobalVariables& theGlobalVariables)
  ;
  bool SplitChamber
(int indexChamberBeingRefined, bool weAreSlicingInDirection, bool weAreChopping, Vector<Rational> & killerNormal, GlobalVariables& theGlobalVariables)
  ;
  void GetNewVerticesAppend
  (Cone& myDyingCone, Vector<Rational> & killerNormal, HashedList<Vector<Rational> >& outputVertices, GlobalVariables& theGlobalVariables)
  ;
  void init()
  { this->splittingNormals.Clear();
    this->slicingDirections.size=0;
    this->Clear();
    this->indexLowestNonRefinedChamber=0;
    this->ConvexHull.Normals.size=0;
    this->ConvexHull.Vertices.size=0;
    this->ConvexHull.flagIsTheZeroCone=true;
  }
  ConeComplex(const ConeComplex& other)
  { this->operator=(other);
  }
  ConeComplex()
  { this->flagChambersHaveTooFewVertices=false;
    this->flagIsRefined=false;
  }
  void WriteToFile
  (std::fstream& output, GlobalVariables* theGlobalVariables){this->WriteToFile(output, theGlobalVariables, -1);}
  void WriteToFile
  (std::fstream& output, GlobalVariables* theGlobalVariables, int UpperLimit)
  ;
  bool ReadFromFile
  (std::fstream& input, GlobalVariables* theGlobalVariables)
  { return this->ReadFromFile(input, theGlobalVariables, -1);
  }
  bool ReadFromFile
  (std::fstream& input, GlobalVariables* theGlobalVariables, int UpperLimitDebugPurposes)
  ;
  void operator=(const ConeComplex& other)
  { this->:: HashedList<Cone>::operator=(other);
    this->splittingNormals=other.splittingNormals;
    this->slicingDirections=other.slicingDirections;
    this->indexLowestNonRefinedChamber=other.indexLowestNonRefinedChamber;
    this->flagIsRefined=other.flagIsRefined;
    this->flagChambersHaveTooFewVertices=other.flagChambersHaveTooFewVertices;
  }
};

class partFractions: public MonomialCollection<partFraction, Polynomial<LargeInt> >
{ bool splitPartial(GlobalVariables& theGlobalVariables, Vector<Rational> * Indicator);
  void initCommon();
  partFractions(const partFractions& other);
public:
  int AmbientDimension;
  int IndexLowestNonProcessed;
  int IndexCurrentlyProcessed;
  int HighestIndex;
  int NumberIrrelevantFractions;
  int NumberRelevantReducedFractions;
  int NumMonomialsInTheNumerators;
  int NumGeneratorsInTheNumerators;
  int NumRelevantNonReducedFractions;
  //int NumRelevantMonomialsInNonReducedFractions;
  //int NumRelevantGeneratorsInNonReducedFractions;
  static int NumMonomialsInNumeratorsRelevantFractions;
  int NumGeneratorsRelevenatFractions;
  int NumMonomialsInNumeratorsIrrelevantFractions;
  int NumGeneratorsIrrelevantFractions;
  int NumTotalReduced;
  int NumProcessedForVPFfractions;
  int NumRunsReduceMonomialByMonomial;
  //GlobalVariables theGlobalVariables;
  static int NumProcessedForVPFMonomialsTotal;
  static std::fstream ComputedContributionsList;
  static const int MaxReadFileBufferSize= 33554432; //= 32 MB of read buffer size
  std::string DebugString;
  Rational StartCheckSum;
  Rational EndCheckSum;
  static Rational CheckSum;
  bool flagDiscardingFractions;
  bool flagUsingOrlikSolomonBasis;
  bool flagInitialized;
  int LimitSplittingSteps;
  int SplitStepsCounter;
  ConeComplex theChambers;
  static  bool flagSplitTestModeNoNumerators;
  static  bool flagAnErrorHasOccurredTimeToPanic;
  static  bool flagMakingProgressReport;
  static  bool flagUsingCheckSum;
  static int flagMaxNumStringOutputLines;

  HashedList<Vector<Rational> > startingVectors;
  Matrix<int> TableAllowedAminusB;
  Matrix<int> TableAllowedAminus2B;
  Selection IndicesRedundantShortRoots;
  List<int> IndicesDoublesOfRedundantShortRoots;
  int NumNonRedundantShortRoots;
  Vector<Rational> weights;
  void initFromRoots(Vectors<Rational>& theAlgorithmBasis, Vector<Rational>* theWeights);
  int AddRootAndSort(Vector<Rational>& theRoot);
  int AddRootPreserveOrder(Vector<Rational>& theRoot);
  int getIndex(const Vector<Rational>& TheRoot);
  int getIndexDoubleOfARoot(const Vector<Rational>& TheRoot);
  void ComputeTable(int theDimension);
  void PrepareCheckSums(GlobalVariables& theGlobalVariables);
  std::string DoTheFullComputationReturnLatexFileString
  (GlobalVariables& theGlobalVariables, Vectors<Rational>& toBePartitioned, FormatExpressions& theFormat, std::string* outputHtml)
  ;
  bool ArgumentsAllowed
  (Vectors<Rational>& theArguments, std::string& outputWhatWentWrong, GlobalVariables& theGlobalVariables)
  ;
  bool AssureIndicatorRegularity(GlobalVariables& theGlobalVariables, Vector<Rational> & theIndicator);
  void CompareCheckSums(GlobalVariables& theGlobalVariables);
  void ComputeDebugString(GlobalVariables& theGlobalVariables);
  void ComputeDebugStringNoNumerator(GlobalVariables& theGlobalVariables);
  void ComputeDebugStringWithVPfunction(GlobalVariables& theGlobalVariables);
  void ComputePolyCorrespondingToOneMonomial
(QuasiPolynomial& outputQP, int monomialIndex, Vectors<Rational>& normals, Lattice& theLattice, GlobalVariables& theGlobalVariables)
;
  void ComputeDebugStringBasisChange(Matrix<LargeInt>& VarChange, GlobalVariables& theGlobalVariables);
  bool initFromRoots(Vectors<Rational>& input, GlobalVariables& theGlobalVariables);
  void initAndSplit(Vectors<Rational>& input, GlobalVariables& theGlobalVariables);
  void Run(Vectors<Rational>& input, GlobalVariables& theGlobalVariables);
  //row index is the index of the Vector<Rational> ; column(second) index is the coordinate index
  void RemoveRedundantShortRootsClassicalRootSystem(GlobalVariables& theGlobalVariables, Vector<Rational> * Indicator);
  void RemoveRedundantShortRoots(GlobalVariables& theGlobalVariables, Vector<Rational> * Indicator);
  bool RemoveRedundantShortRootsIndex
(MonomialCollection<partFraction, Polynomial<LargeInt> >& output, int theIndex, GlobalVariables& theGlobalVariables,
 Vector<Rational>* Indicator)
  ;
  bool splitClassicalRootSystem(bool ShouldElongate, GlobalVariables& theGlobalVariables, Vector<Rational> * Indicator);
  bool split(GlobalVariables& theGlobalVariables, Vector<Rational> * Indicator);
  void ComputeKostantFunctionFromWeylGroup
(char WeylGroupLetter, int WeylGroupNumber, QuasiPolynomial& output, Vector<Rational>* ChamberIndicator,
 bool UseOldData, bool StoreToFile, GlobalVariables&  theGlobalVariables)
 ;
 bool IsHigherThanWRTWeight
 (const Vector<Rational>& left, const Vector<Rational>& r, const Vector<Rational>& theWeights)
 ;
  void ComputeSupport(List<Vectors<Rational> >& output, std::stringstream& outputString);
  void ComputeOneCheckSum(Rational& output, GlobalVariables& theGlobalVariables);
  void AccountPartFractionInternals(int sign, int index, Vector<Rational> * Indicator, GlobalVariables& theGlobalVariables);
  void PopIndexHashChooseSwapByLowestNonProcessedAndAccount(int index, GlobalVariables& theGlobalVariables, Vector<Rational> * Indicator);
  void PopIndexSwapLastHashAndAccount(int index, GlobalVariables& theGlobalVariables, Vector<Rational> * Indicator);
  void PrepareIndicatorVariables();
  void initFromOtherPartFractions(partFractions& input, GlobalVariables& theGlobalVariables);
  void IncreaseHighestIndex(int increment);
  std::string ElementToString(GlobalVariables& theGlobalVariables, FormatExpressions& theFormat){std::string tempS; this->ElementToString(tempS, theGlobalVariables, theFormat);  return tempS;}
  void ElementToString(std::string& output, GlobalVariables& theGlobalVariables, FormatExpressions& theFormat);
  int ElementToString
  (std::string& output, bool LatexFormat, bool includeVPsummand, bool includeNumerator,
   GlobalVariables& theGlobalVariables, FormatExpressions& theFormat)
   ;
  int ElementToStringBasisChange
  (Matrix<LargeInt>& VarChange, bool UsingVarChange, std::string& output, bool LatexFormat,
 bool includeVPsummand, bool includeNumerator, GlobalVariables& theGlobalVariables, FormatExpressions& PolyFormatLocal)
;
  int ElementToStringOutputToFile(std::fstream& output, bool LatexFormat, bool includeVPsummand, bool includeNumerator, GlobalVariables& theGlobalVariables);
  int ElementToStringBasisChangeOutputToFile(Matrix<LargeInt>& VarChange, bool UsingVarChange, std::fstream& output, bool LatexFormat, bool includeVPsummand, bool includeNumerator, GlobalVariables& theGlobalVariables);
  bool GetVectorPartitionFunction
  (QuasiPolynomial& output, Vector<Rational> & newIndicator, GlobalVariables& theGlobalVariables)
  ;
  bool VerifyFileComputedContributions(GlobalVariables& theGlobalVariables);
  void WriteToFileComputedContributions(std::fstream& output, GlobalVariables&  theGlobalVariables);
  int ReadFromFileComputedContributions(std::fstream& input, GlobalVariables&  theGlobalVariables);
  void WriteToFile(std::fstream& output, GlobalVariables* theGlobalVariables);
  void ReadFromFile(std::fstream& input, GlobalVariables* theGlobalVariables);
  void ResetRelevanceIsComputed(){for (int i=0; i<this->size; i++){this->TheObjects[i].RelevanceIsComputed=false; }; }
  partFractions();
  int SizeWithoutDebugString();
  bool CheckForMinimalityDecompositionWithRespectToRoot(Vector<Rational>  *theRoot, GlobalVariables& theGlobalVariables);
  void MakeProgressReportSplittingMainPart(GlobalVariables& theGlobalVariables);
  void MakeProgressReportRemovingRedundantRoots(GlobalVariables& theGlobalVariables);
  void MakeProgressReportUncoveringBrackets(GlobalVariables& theGlobalVariables);
  void MakeProgressVPFcomputation(GlobalVariables& theGlobalVariables);
  void operator=(const partFractions& other);
};

class WeylGroup: public HashedList<ElementWeylGroup>
{
  Matrix<int> CartanSymmetricIntBuffer;
  void UpdateIntBuffer()
  { this->CartanSymmetricIntBuffer.init(this->CartanSymmetric.NumRows, this->CartanSymmetric.NumCols);
    for (int i=0; i<this->CartanSymmetric.NumRows; i++)
      for (int j=0; j<this->CartanSymmetric.NumCols; j++)
        this->CartanSymmetricIntBuffer.elements[i][j]=this->CartanSymmetric.elements[i][j].NumShort;
  }
  Matrix<Rational>  FundamentalToSimpleCoords;
  Matrix<Rational>  SimpleToFundamentalCoords;
  bool flagFundamentalToSimpleMatricesAreComputed;
  inline void ComputeFundamentalToSimpleMatrices()
  { if (flagFundamentalToSimpleMatricesAreComputed)
      return;
    Vectors<Rational> fundamentalBasis;
    this->GetFundamentalWeightsInSimpleCoordinates(fundamentalBasis);
    this->FundamentalToSimpleCoords.AssignRootsToRowsOfMatrix(fundamentalBasis);
    this->FundamentalToSimpleCoords.Transpose();
    this->SimpleToFundamentalCoords=this->FundamentalToSimpleCoords;
    this->SimpleToFundamentalCoords.Invert();
    this->flagFundamentalToSimpleMatricesAreComputed=true;
  }
public:
  Matrix<Rational> CartanSymmetric;
  Vector<Rational> rho;
  char WeylLetter;
  Rational LongRootLength;
  //Rational LongLongScalarProdPositive;
  //Rational ShortShortScalarProdPositive;
  //Rational ShortLongScalarProdPositive;
  //Rational ShortRootLength;
  HashedList<Vector<Rational> > RootSystem;
  Vectors<Rational> RootsOfBorel;
  static bool flagAnErrorHasOcurredTimeToPanic;
//  void MakeFromParSel(Vector<Rational> & parSel, WeylGroup& input);
  void ComputeRho(bool Recompute);
  std::string ElementToString();
  void MakeArbitrary(char WeylGroupLetter, int n);
  void GenerateAdditivelyClosedSubset(Vectors<Rational>& input, Vectors<Rational>& output);
  Rational GetKillingDivTraceRatio();
  Rational EstimateNumDominantWeightsBelow(Vector<Rational> & inputHWsimpleCoords, GlobalVariables& theGlobalVariables);
  bool ContainsARootNonStronglyPerpendicularTo(Vectors<Rational>& theVectors, Vector<Rational>& input)
 ;
  int NumRootsConnectedTo(Vectors<Rational>& theVectors, Vector<Rational>& input)
  ;
  void MakeAn(int n);
  void MakeEn(int n);
  void MakeBn(int n);
  void MakeCn(int n);
  void MakeDn(int n);
  void MakeF4();
  void MakeG2();
  WeylGroup(){this->flagFundamentalToSimpleMatricesAreComputed=false;}
  Matrix<Rational> * GetMatrixFundamentalToSimpleCoords()
  { this->ComputeFundamentalToSimpleMatrices();
    return &this->FundamentalToSimpleCoords;
  }
  Matrix<Rational>* GetMatrixSimpleToFundamentalCoords()
  { this->ComputeFundamentalToSimpleMatrices();
    return &this->SimpleToFundamentalCoords;
  }
  template<class CoefficientType>
  Vector<CoefficientType> GetSimpleCoordinatesFromFundamental
  (const Vector<CoefficientType>& inputInFundamentalCoords)
  ;
  template<class CoefficientType>
  Vector<CoefficientType> GetFundamentalCoordinatesFromSimple
  (const Vector<CoefficientType>& inputInSimpleCoords)
  ;
  Vector<Rational> GetDualCoordinatesFromSimple
  (const Vector<Rational> & inputInSimpleCoords)
  { return this->GetDualCoordinatesFromFundamental(this->GetFundamentalCoordinatesFromSimple(inputInSimpleCoords));
  }
  template <class CoefficientType>
  Vector<CoefficientType> GetDualCoordinatesFromFundamental
  (const Vector<CoefficientType>& inputInFundamentalCoords)
  { Vector<CoefficientType> result=inputInFundamentalCoords;
    if (result.size!=this->GetDim())
    { std::cout << "This is a programming error. The input fundamental weight has " << result.size
      << " coordinates, while the rank of the Weyl group is " << this->GetDim() << ". Please debug file "
      << CGI::GetHtmlLinkFromFileName(__FILE__) << " line " << __LINE__ << ".";
      assert(false);
    }
    for (int i=0; i<result.size; i++)
      result[i]*=this->CartanSymmetric.elements[i][i]/2;
    return result;
  }
  template <class CoefficientType>
  CoefficientType GetScalarProdSimpleRoot(const Vector<CoefficientType> & input, int indexSimpleRoot)
  { CoefficientType result, buffer;
    result=input[0].GetZero();//<-the value of zero is not known at compile time (example: multivariate polynomials have unknown #variables)
    Rational* currentRow=this->CartanSymmetric.elements[indexSimpleRoot];
    for (int i=0; i<input.size; i++)
    { buffer=input[i];
      buffer*=currentRow[i];
      result+=buffer;
    }
    return result;
  }
  template <class CoefficientType>
  CoefficientType WeylDimFormulaSimpleCoords
  (Vector<CoefficientType>& theWeightInSimpleCoords, const CoefficientType& theRingUnit=1)
  ;
  template <class CoefficientType>
  CoefficientType WeylDimFormulaFundamentalCoords
  (Vector<CoefficientType>& weightFundCoords, const CoefficientType& theRingUnit=1)
  ;
  template <class CoefficientType>
  void RaiseToDominantWeight
  (Vector<CoefficientType>& theWeight, int* sign=0, bool* stabilizerFound=0)
  ;
  void GetCoxeterPlane
  (Vector<double>& outputBasis1, Vector<double>& outputBasis2, GlobalVariables& theGlobalVariables)
;
  void DrawRootSystem
(DrawingVariables& outputDV, bool wipeCanvas, GlobalVariables& theGlobalVariables,
 bool drawWeylChamber, Vector<Rational> * bluePoint=0, bool LabelDynkinDiagramVertices=false,
 Vectors<Rational>* predefinedProjectionPlane=0)
  ;
  bool HasStronglyPerpendicularDecompositionWRT
(Vector<Rational>& input, int UpperBoundNumBetas, Vectors<Rational>& theSet, Vectors<Rational>& output,
 List<Rational>& outputCoeffs, bool IntegralCoefficientsOnly)
 ;
  void MakeFromDynkinType(List<char>& theLetters, List<int>& theRanks, List<int>* theMultiplicities);
  void MakeFromDynkinType(List<char>& theLetters, List<int>& theRanks){ this->MakeFromDynkinType(theLetters, theRanks, 0); }
  //void GetLongRootLength(Rational& output);
  static bool IsAddmisibleDynkinType(char candidateLetter, int n);
  //the below will not do anything if the inputLetter is not a valid Dynkin letter
  static void TransformToAdmissibleDynkinType(char inputLetter, int& outputRank);
  void GetEpsilonCoords(char WeylLetter, int WeylRank, Vectors<Rational>& simpleBasis, Vector<Rational> & input, Vector<Rational> & output, GlobalVariables& theGlobalVariables);
  void GetEpsilonCoords(const Vector<Rational> & input, Vector<Rational> & output, GlobalVariables& theGlobalVariables);
  void GetEpsilonCoords(List<Vector<Rational> >& input, Vectors<Rational>& output, GlobalVariables& theGlobalVariables);
  Vector<Rational> GetEpsilonCoords(const Vector<Rational> & input, GlobalVariables& theGlobalVariables)
  { Vector<Rational> tempRoot;
    this->GetEpsilonCoords(input, tempRoot, theGlobalVariables);
    return tempRoot;
  }
  bool IsStronglyPerpendicularTo(const Vector<Rational>& input, const Vector<Rational>& other);
  bool IsStronglyPerpendicularTo(const Vector<Rational>& input, const Vectors<Rational>& others);
  void GetEpsilonCoordsWRTsubalgebra(Vectors<Rational>& generators, List<Vector<Rational> >& input, Vectors<Rational>& output, GlobalVariables& theGlobalVariables);
  void GetEpsilonMatrix(char WeylLetter, int WeylRank, GlobalVariables& theGlobalVariables, Matrix<Rational> & output);
  void ComputeWeylGroup();
  bool LeftIsHigherInBruhatOrderThanRight(ElementWeylGroup& left, ElementWeylGroup& right);
  void GetMatrixReflection(Vector<Rational> & reflectionRoot, Matrix<Rational> & output);
  template <class CoefficientType>
  bool GetAlLDominantWeightsHWFDIM
(Vector<CoefficientType> & highestWeightSimpleCoords, HashedList<Vector<CoefficientType> >& outputWeightsSimpleCoords,
 int upperBoundDominantWeights, std::string& outputDetails, GlobalVariables& theGlobalVariables)
  ;
template <class CoefficientType>
  bool FreudenthalEval
  (Vector<CoefficientType>& inputHWfundamentalCoords, HashedList<Vector<CoefficientType> >& outputDominantWeightsSimpleCoords,
   List<CoefficientType>& outputMultsSimpleCoords, std::string& outputDetails,
   GlobalVariables& theGlobalVariables, int UpperBoundFreudenthal)
  ;
  void GetWeylChamber
  (Cone& output, GlobalVariables& theGlobalVariables)
  ;
  std::string GenerateWeightSupportMethoD1
  (Vector<Rational> & highestWeightSimpleCoords, Vectors<Rational>& outputWeightsSimpleCoords, int upperBoundWeights, GlobalVariables& theGlobalVariables)
;
  void GetIntegralLatticeInSimpleCoordinates(Lattice& output);
  void GetFundamentalWeightsInSimpleCoordinates(Vectors<Rational>& output);
  inline int GetDim()const{return this->CartanSymmetric.NumRows;}
  void ComputeWeylGroup(int UpperLimitNumElements);
  void ComputeWeylGroupAndRootsOfBorel(Vectors<Rational>& output);
  void ComputeRootsOfBorel(Vectors<Rational>& output);
  static Rational GetSizeWeylByFormula(char weylLetter, int theDim);
  bool IsARoot(const Vector<Rational> & input){ return this->RootSystem.Contains(input); }
  void GenerateRootSubsystem(Vectors<Rational>& theRoots);
  template <class CoefficientType>
  bool GenerateOrbit
  (Vectors<CoefficientType>& theRoots, bool RhoAction, HashedList<Vector<CoefficientType> >& output,
   bool UseMinusRho, int expectedOrbitSize=-1, WeylGroup* outputSubset=0, int UpperLimitNumElements=-1)
   ;
//  int GetNumRootsFromFormula();
  void GenerateRootSystemFromKillingFormMatrix();
  void WriteToFile(std::fstream& output);
  void ReadFromFile(std::fstream& input);
  void ActOnAffineHyperplaneByGroupElement(int index, affineHyperplane& output, bool RhoAction, bool UseMinusRho);
  void ProjectOnTwoPlane(Vector<Rational> & orthonormalBasisVector1, Vector<Rational> & orthonormalBasisVector2, GlobalVariables& theGlobalVariables);
  void GetLowestElementInOrbit
  (Vector<Rational>  & inputOutput, ElementWeylGroup* outputWeylElt,
   Vectors<Rational>& bufferEiBAsis,
   bool RhoAction,
   bool UseMinusRho, int* sign=0, bool* stabilizerFound=0)
  { this->GetExtremeElementInOrbit
    (inputOutput, outputWeylElt, bufferEiBAsis, true, RhoAction, UseMinusRho, sign, stabilizerFound);
  }
  void GetHighestElementInOrbit
  (Vector<Rational>  & inputOutput, ElementWeylGroup* outputWeylElt,
   Vectors<Rational>& bufferEiBAsis,
   bool RhoAction, bool UseMinusRho,
   int* sign, bool* stabilizerFound)
  { this->GetExtremeElementInOrbit
    (inputOutput, outputWeylElt, bufferEiBAsis, false, RhoAction, UseMinusRho, sign, stabilizerFound);
  }
  void GetExtremeElementInOrbit
  (Vector<Rational> & inputOutput, ElementWeylGroup* outputWeylElt,
   Vectors<Rational>& bufferEiBAsis,
   bool findLowest, bool RhoAction, bool UseMinusRho, int* sign,
   bool* stabilizerFound)
     ;
  void GetLongestWeylElt(ElementWeylGroup& outputWeylElt) ;
  bool IsEigenSpaceGeneratorCoxeterElement(Vector<Rational> & input);
  void GetCoxeterElement(ElementWeylGroup& outputWeylElt)
  { outputWeylElt.SetSize(this->GetDim());
    for (int i=0; i<outputWeylElt.size; i++)
      outputWeylElt[i]=i;
  }
  template <class Element>
  void ActOn(ElementWeylGroup& theGroupElement, Vector<Element>& theVector, bool RhoAction, bool UseMinusRho, const Element& theRingZero)
  { for (int i=0; i<theGroupElement.size; i++)
      this->SimpleReflection(theGroupElement[i], theVector, RhoAction, UseMinusRho, theRingZero);
  }
  template <class Element>
  void ActOn(int indexOfWeylElement, Vector<Element>& theVector, bool RhoAction, bool UseMinusRho, const Element& theRingZero=0)
  { this->ActOn(this->TheObjects[indexOfWeylElement], theVector, RhoAction, UseMinusRho, theRingZero);
  }
  template <class Element>
  void ActOnDual(int index,Vector<Element>& theVector, bool RhoAction, const Element& theRingZero);
  //theRoot is a list of the simple coordinates of the Vector<Rational>
  //theRoot serves as both input and output
  void ActOnRootAlgByGroupElement(int index, PolynomialSubstitution<Rational>& theRoot, bool RhoAction);
  void ActOnRootByGroupElement(int index, Vector<Rational> & theRoot, bool RhoAction, bool UseMinusRho);
  void ActOnDualSpaceElementByGroupElement(int index, Vector<Rational> & theDualSpaceElement, bool RhoAction);
  void SimpleReflectionRoot(int index, Vector<Rational> & theRoot, bool RhoAction, bool UseMinusRho);
  template <class Element>
  void SimpleReflection
  (int index, Vector<Element>& theVector, bool RhoAction, bool UseMinusRho, const Element& theRingZero=0)
  ;
  void GetMatrixOfElement(int theIndex, Matrix<Rational> & outputMatrix);
  void GetMatrixOfElement(ElementWeylGroup& input, Matrix<Rational> & outputMatrix);
  void SimpleReflectionDualSpace(int index, Vector<Rational> & DualSpaceElement);
  void SimpleReflectionRootAlg(int index, PolynomialSubstitution<Rational>& theRoot, bool RhoAction);
  bool IsPositiveOrPerpWRTh(const Vector<Rational> & input, const Vector<Rational> & theH){ return this->RootScalarCartanRoot(input, theH).IsPositiveOrZero(); }
  template<class CoefficientType>
  void ReflectBetaWRTAlpha(Vector<Rational>& alpha, Vector<CoefficientType>& Beta, bool RhoAction, Vector<CoefficientType>& Output);
  bool IsRegular(Vector<Rational> & input, int* indexFirstPerpendicularRoot);
  template <class leftType, class rightType>
  void RootScalarCartanRoot
  (const Vector<leftType> & r1, const Vector<rightType> & r2, leftType& output)const;
  double RootScalarCartanRoot(const Vector<double>& r1, const Vector<double>& r2)const
  { assert(r1.size==r2.size && r1.size==this->GetDim());
    double result=0;
    for (int i=0; i<this->GetDim(); i++)
      for (int j=0; j<this->GetDim(); j++)
        result+=this->CartanSymmetric.elements[i][j].DoubleValue()*r1.TheObjects[i]*r2.TheObjects[j];
    return result;
  }
  template <class leftType, class rightType>
  leftType RootScalarCartanRoot(const Vector<leftType> & r1, const Vector<rightType> & r2)const
  { leftType tempRat;
    this->RootScalarCartanRoot(r1, r2, tempRat);
    return tempRat;
  }
  //the below functions perturbs input so that inputH has non-zero scalar product with Vectors<Rational> of the Vector<Rational>  system,
  //without changing the inputH-sign of any Vector<Rational>  that had a non-zero scalar product to begin with
  void PerturbWeightToRegularWRTrootSystem(const Vector<Rational>& inputH, Vector<Rational> & output);
  template <class CoefficientType>
  bool IsDominantWRTgenerator(const Vector<CoefficientType>& theWeight, int generatorIndex)
;
  template <class CoefficientType>
  bool IsDominantWeight(const Vector<CoefficientType> & theWeight)
;
  void TransformToSimpleBasisGenerators(Vectors<Rational>& theGens);
  void TransformToSimpleBasisGeneratorsWRTh(Vectors<Rational>& theGens, const Vector<Rational> & theH);
  int length(int index);
  void operator=(const WeylGroup& other);
  bool operator==(const WeylGroup& other)
  { return this->CartanSymmetric==other.CartanSymmetric;
  }
};

template <class Element>
void WeylGroup::SimpleReflection
  (int index, Vector<Element>& theVector, bool RhoAction, bool UseMinusRho, const Element& theRingZero)
{ Element alphaShift, tempRat;
  alphaShift=theRingZero;
  for (int i=0; i<this->CartanSymmetric.NumCols; i++)
  { tempRat.Assign(theVector.TheObjects[i]);
    tempRat*=(this->CartanSymmetric.elements[index][i]*(-2));
    alphaShift+=(tempRat);
  }
  if (this->flagAnErrorHasOcurredTimeToPanic)
  { std::string tempS;
    tempS=alphaShift.ElementToString();
  }
  alphaShift/=(this->CartanSymmetric.elements[index][index]);
  if (RhoAction)
  { if(UseMinusRho)
      alphaShift+=(1);
    else
      alphaShift+=(-1);
  }
  theVector.TheObjects[index]+=(alphaShift);
}

class ReflectionSubgroupWeylGroup: public HashedList<ElementWeylGroup>
{
public:
  bool truncated;
  WeylGroup AmbientWeyl;
  WeylGroup Elements;
  List<ElementWeylGroup> RepresentativesQuotientAmbientOrder;
  Vectors<Rational> simpleGenerators;
  //format: each element of of the group is a list of generators, reflections with respect to the simple generators, and outer
  //automorphisms.
  //format of the externalAutomorphisms:
  // For a fixed external automorphism (of type Vectors<Rational>) the i^th entry gives the image of the simple Vector<Rational>  with 1 on the i^th coordinate
  List<Vectors<Rational> > ExternalAutomorphisms;
  HashedList<Vector<Rational> > RootSubsystem;
  Vectors<Rational> RootsOfBorel;
  std::string DebugString;
  void ComputeDebugString(){this->ElementToString(this->DebugString, true); }
  void ElementToString(std::string& output, bool displayElements);
  std::string ElementToStringBruhatGraph();
  std::string ElementToStringCosetGraph();
  std::string ElementToStringFromLayersAndArrows
  (List<List<List<int> > >& arrows, List<List<int> >& Layers, int GraphWidth, bool useAmbientIndices)
  ;
  std::string ElementToString(bool displayElements=true){std::string tempS; this->ElementToString(tempS, displayElements); return tempS;}
  Vector<Rational> GetRho();
  template <class CoefficientType>
  void RaiseToDominantWeight
  (Vector<CoefficientType>& theWeight, int* sign=0, bool* stabilizerFound=0)
  ;
  template <class CoefficientType>
  bool FreudenthalEvalIrrepIsWRTLeviPart
(Vector<CoefficientType>& inputHWfundamentalCoords, HashedList<Vector<CoefficientType> >& outputDominantWeightsSimpleCoords,
 List<CoefficientType>& outputMultsSimpleCoords, std::string& outputDetails,
 GlobalVariables& theGlobalVariables, int UpperBoundFreudenthal)
  ;
  void MakeParabolicFromSelectionSimpleRoots
(WeylGroup& inputWeyl, const Selection& ZeroesMeanSimpleRootSpaceIsInParabolic, GlobalVariables& theGlobalVariables, int UpperLimitNumElements)
  ;
  void MakeParabolicFromSelectionSimpleRoots
(WeylGroup& inputWeyl, const Vector<Rational> & ZeroesMeanSimpleRootSpaceIsInParabolic, GlobalVariables& theGlobalVariables, int UpperLimitNumElements)
;
  bool GetAlLDominantWeightsHWFDIMwithRespectToAmbientAlgebra
  (Vector<Rational> & highestWeightSimpleCoords, HashedList<Vector<Rational> >& outputWeightsSimpleCoords,
 int upperBoundDominantWeights, std::string& outputDetails, GlobalVariables& theGlobalVariables)
 ;
  template <class CoefficientType>
  bool GetAlLDominantWeightsHWFDIM
  (Vector<CoefficientType> & highestWeightSimpleCoords, HashedList<Vector<CoefficientType> >& outputWeightsSimpleCoords,
 int upperBoundDominantWeights, std::string& outputDetails, GlobalVariables& theGlobalVariables)
 ;
  bool DrawContour
(const Vector<Rational> & highestWeightSimpleCoord, DrawingVariables& theDV, GlobalVariables& theGlobalVariables, int theColor,
 int UpperBoundVertices)
    ;
//The dirty C++ language forces that the body of this function appear after the definitions of IsDominantWRTgenerator.
//Apparently the algorithm of making an oriented acyclic graph totally ordered is a too difficult task for the designers of c++
// so I have to do it for them.
  template <class CoefficientType>
  bool IsDominantWeight(const Vector<CoefficientType> & theWeight)
  ;
  template <class CoefficientType>
  bool IsDominantWRTgenerator(const Vector<CoefficientType> & theWeight, int generatorIndex)
  ;
  void FindQuotientRepresentatives(int UpperLimit);
  void GetMatrixOfElement(ElementWeylGroup& input, Matrix<Rational> & outputMatrix);
  template <class CoefficientType>
  bool GenerateOrbitReturnFalseIfTruncated(const Vector<CoefficientType>& input, Vectors<CoefficientType>& outputOrbit, int UpperLimitNumElements);
  void ComputeSubGroupFromGeneratingReflections(Vectors<Rational>& inputGenerators, List<Vectors<Rational> > & inputExternalAutos, GlobalVariables& theGlobalVariables, int UpperLimitNumElements, bool recomputeAmbientRho);
  void ComputeRootSubsystem();
  void ActByElement(int index, Vector<Rational> & theRoot);
  void ActByElement(int index, Vector<Rational> & input, Vector<Rational> & output){this->ActByElement(this->TheObjects[index], input, output);}
  void ActByElement(ElementWeylGroup& theElement, Vector<Rational> & input, Vector<Rational> & output);
  void ActByElement(int index, Vectors<Rational>& input, Vectors<Rational>& output) {this->ActByElement(this->TheObjects[index], input, output);}
  void ActByElement(ElementWeylGroup& theElement, Vectors<Rational>& input, Vectors<Rational>& output);
  void WriteToFile(std::fstream& output, GlobalVariables* theGlobalVariables);
  void ReadFromFile(std::fstream& input, GlobalVariables* theGlobalVariables);
  void Assign(const ReflectionSubgroupWeylGroup& other);
  void operator=(const ReflectionSubgroupWeylGroup& other){ this->Assign(other); }
};

template <class CoefficientType>
bool WeylGroup::IsDominantWeight(const Vector<CoefficientType>& theWeight)
{ int theDimension= this->GetDim();
  for (int i=0; i<theDimension; i++)
    if (!this->IsDominantWRTgenerator(theWeight, i))
      return false;
  return true;
}

template <class CoefficientType>
bool ReflectionSubgroupWeylGroup::IsDominantWeight(const Vector<CoefficientType>& theWeight)
{ for (int i=0; i<this->simpleGenerators.size; i++)
    if (! this->IsDominantWRTgenerator(theWeight, i))
      return false;
  return true;
}

class multTableKmods : public List<List<List<int> > >
{
public:
  std::string DebugString;
  void ElementToString(std::string& output, rootSubalgebra& owner);
  void ElementToString(std::string& output, bool useLaTeX, bool useHtml, rootSubalgebra& owner);
  void ComputeDebugString(rootSubalgebra& owner){ this->ElementToString(this->DebugString, owner); }
};

class DynkinDiagramRootSubalgebra
{
public:
  std::string DynkinStrinG;
  void ElementToStrinG
  (std::string& output, bool useDollarSigns, bool IncludeAlgebraNames)
  ;
  std::string ElementToStrinG
  (bool useDollarSigns, bool IncludeAlgebraNames)
  { std::string result;
    this->ElementToStrinG(result, useDollarSigns, IncludeAlgebraNames);
    return result;
  }
  void ComputeDynkinStrinG()
  { this->ElementToStrinG(this->DynkinStrinG, false, false);
  }
  std::string SetComponent
  (const std::string& WeylLetterWithLength, int WeylRank, int componentIndex)
  ;
  static std::string GetNameFrom
  (const std::string& WeylLetterWithLength, int WeylRank, bool IncludeAlgebraNames)
  ;
  static std::string GetDiagramAndAlgebraName
  (const std::string& WeylLetterWithLength, int WeylRank)
  {return DynkinDiagramRootSubalgebra::GetNameFrom(WeylLetterWithLength, WeylRank, true);}
  List<Vectors<Rational> >  SimpleBasesConnectedComponents;
  //to each connected component of the simple bases corresponds
  //its dynkin string with the same index
  List<std::string> DynkinTypeStrings;
  List<int> ComponentRanks;
  List<std::string> ComponentLetters;
  List<int> indicesThreeNodes;
  List<List<int> > indicesEnds;
  List<List<int> > sameTypeComponents;
  List<int> indexUniComponent;
  List<int> indexInUniComponent;
  int RankTotal();
  int NumRootsGeneratedByDiagram();
  void Sort();
  void SwapDynkinStrings(int i, int j);
  bool LetterIsDynkinGreaterThanLetter(char letter1, char letter2);
  //the below function takes as an input a set of Vectors<Rational> and computes the corredponding Dynkin diagram of the
  //Vector<Rational>  subsystem. Note: the simleBasisInput is required to be a set of simple Vectors<Rational>. The procedure calls a
  //transformation to simple basis on the simpleBasisInput, so your input will get changed if it wasn't
  //simple as required!
  inline void ComputeDiagramTypeModifyInput(Vectors<Rational>& simpleBasisInput, WeylGroup& theWeyl)
  { theWeyl.TransformToSimpleBasisGenerators(simpleBasisInput);
    this->ComputeDiagramTypeKeepInput(simpleBasisInput, theWeyl);
  }
  //the below function is just as the above but doesn't modify simpleBasisInput
  void ComputeDiagramTypeKeepInput
  (const Vectors<Rational>& simpleBasisInput, WeylGroup& theWeyl)
  ;
  void ComputeDynkinStrings
  (WeylGroup& theWeyl)
  ;
  void ComputeDynkinString
(int indexComponent, WeylGroup& theWeyl)
  ;
  void GetKillingFormMatrixUseBourbakiOrder(Matrix<Rational> & output, WeylGroup& theWeyl);
  int numberOfThreeValencyNodes(int indexComponent, WeylGroup& theWeyl);
  void Assign(const DynkinDiagramRootSubalgebra& right);
  inline void operator=(const DynkinDiagramRootSubalgebra& right){this->Assign(right); }
  bool operator==(const DynkinDiagramRootSubalgebra& right) const;
  bool IsGreaterThan(DynkinDiagramRootSubalgebra& right);
  Rational GetSizeCorrespondingWeylGroupByFormula();
  void GetSimpleBasisInBourbakiOrder(Vectors<Rational>& output);
  void GetSimpleBasisInBourbakiOrderOneComponentAppend(Vectors<Rational>& outputAppend, int index);
  void GetAutomorphism(List<List<int> >& output, int index);
  void GetAutomorphisms(List<List<List<int> > >& output);
  void GetMapFromPermutation(Vectors<Rational>& domain, Vectors<Rational>& range, List<int>& thePerm, List<List<List<int> > >& theAutos, SelectionWithDifferentMaxMultiplicities& theAutosPerm, DynkinDiagramRootSubalgebra& right);
};

class coneRelation
{
public:
  Vectors<Rational> Alphas;
  Vectors<Rational> Betas;
  List<Rational> AlphaCoeffs;
  List<Rational> BetaCoeffs;
  List<List<int> > AlphaKComponents;
  List<List<int> > BetaKComponents;
  int IndexOwnerRootSubalgebra;
  bool GenerateAutomorphisms(coneRelation& right, rootSubalgebras& owners);
  DynkinDiagramRootSubalgebra theDiagram;
  DynkinDiagramRootSubalgebra theDiagramRelAndK;
  std::string DebugString;
  std::string stringConnectedComponents;
  void ReadFromFile(std::fstream& input, GlobalVariables* theGlobalVariables, rootSubalgebras& owner);
  void WriteToFile(std::fstream& output, GlobalVariables* theGlobalVariables);
  void ComputeTheDiagramAndDiagramRelAndK(rootSubalgebra& owner);
  void ComputeDiagramRelAndK(rootSubalgebra& owner);
  void FixRepeatingRoots(Vectors<Rational>& theRoots, List<Rational>& coeffs);
  void RelationOneSideToString(std::string& output, const std::string& letterType, List<Rational>& coeffs, List<List<int> >& kComponents, Vectors<Rational>& theRoots, bool useLatex, rootSubalgebra& owner);
  void GetEpsilonCoords(Vectors<Rational>& input, Vectors<Rational>& output, WeylGroup& theWeyl, GlobalVariables& theGlobalVariables);
  int ElementToString(std::string& output, rootSubalgebras& owners, bool useLatex, bool includeScalarsProductsEachSide, bool includeMixedScalarProducts);
  int RootsToScalarProductString(Vectors<Rational>& inputLeft, Vectors<Rational>& inputRight, const std::string& letterTypeLeft, const std::string& letterTypeRight, std::string& output, bool useLatex, rootSubalgebra& owner);
  void ComputeConnectedComponents(Vectors<Rational>& input, rootSubalgebra& owner, List<List<int> >& output);
  void ComputeDebugString(rootSubalgebras& owner, bool includeScalarsProducts, bool includeMixedScalarProducts){ this->ElementToString(this->DebugString, owner, true, includeScalarsProducts, includeMixedScalarProducts);  }
  void MakeLookCivilized(rootSubalgebra& owner, Vectors<Rational>& NilradicalRoots);
  bool IsStrictlyWeaklyProhibiting(rootSubalgebra& owner, Vectors<Rational>& NilradicalRoots, GlobalVariables& theGlobalVariables, rootSubalgebras& owners, int indexInOwner);
  void FixRightHandSide(rootSubalgebra& owner, Vectors<Rational>& NilradicalRoots);
  bool leftSortedBiggerThanOrEqualToRight(List<int>& left, List<int>& right);
  void ComputeKComponents(Vectors<Rational>& input, List<List<int> >& output, rootSubalgebra& owner);
  void RelationOneSideToStringCoordForm(std::string& output,  List<Rational>& coeffs, Vectors<Rational>& theRoots, bool EpsilonForm);
  void GetSumAlphas(Vector<Rational> & output, int theDimension);
  bool CheckForBugs(rootSubalgebra& owner, Vectors<Rational>& NilradicalRoots);
  void SortRelation(rootSubalgebra& owner);
  void operator=(const coneRelation& right)
  { this->Alphas.CopyFromBase(right.Alphas);
    this->Betas.CopyFromBase(right.Betas);
    this->AlphaCoeffs.CopyFromBase(right.AlphaCoeffs);
    this->BetaCoeffs.CopyFromBase(right.BetaCoeffs);
    this->AlphaKComponents.CopyFromBase(right.AlphaKComponents);
    this->BetaKComponents.CopyFromBase(right.BetaKComponents);
    this->theDiagram=right.theDiagram;
    this->theDiagramRelAndK=right.theDiagramRelAndK;
    this->IndexOwnerRootSubalgebra=right.IndexOwnerRootSubalgebra;
    this->DebugString= right.DebugString;
  }
  bool operator==(const coneRelation& right){ return this->DebugString==right.DebugString; }
  int HashFunction() const
  { int tempI= ::MathRoutines::Minimum((int)this->DebugString.length(), ::SomeRandomPrimesSize);
    int result=0;
    for (int i=0; i<tempI; i++)
      result+= this->DebugString[i]*::SomeRandomPrimes[i];
    return result;
  }
  static inline int HashFunction(const coneRelation& input)
  { return input.HashFunction();
  }
  coneRelation(){this->IndexOwnerRootSubalgebra=-1; }
};

class coneRelations: public HashedList<coneRelation>
{
public:
  int NumAllowedLatexLines;
  bool flagIncludeSmallerRelations;
  bool flagIncludeCoordinateRepresentation;
  bool flagIncludeSubalgebraDataInDebugString;
  std::string DebugString;
  List<std::string> CoordinateReps;
  void GetLatexHeaderAndFooter(std::string& outputHeader, std::string& outputFooter);
  void ElementToString
(std::string& output, rootSubalgebras& owners, bool useLatex, bool useHtml, std::string* htmlPathPhysical,
 std::string* htmlPathServer, GlobalVariables& theGlobalVariables, const std::string& DisplayNameCalculator)
  ;
  void ComputeDebugString(rootSubalgebras& owners, std::string* htmlPathPhysical, std::string* htmlPathServer, GlobalVariables& theGlobalVariables)
  { this->ElementToString (this->DebugString, owners, true, false, htmlPathPhysical, htmlPathServer, theGlobalVariables, "");
  }
  void ComputeDebugString(rootSubalgebras& owners, GlobalVariables& theGlobalVariables){ this->ComputeDebugString(owners, 0, 0, theGlobalVariables); };
  void WriteToFile(std::fstream& output, GlobalVariables* theGlobalVariables);
  void ReadFromFile(std::fstream& input, GlobalVariables* theGlobalVariables, rootSubalgebras& owner);
  void AddRelationNoRepetition(coneRelation& input, rootSubalgebras& owners, int indexInRootSubalgebras);
  coneRelations()
  { this->NumAllowedLatexLines=40;
    this->flagIncludeSmallerRelations=true;
    this->flagIncludeCoordinateRepresentation=false;
    this->flagIncludeSubalgebraDataInDebugString=false;
  }
};

class rootSubalgebra
{
public:
  int NumNilradicalsAllowed;
  int NumConeConditionFailures;
  int NumRelationsWithStronglyPerpendicularDecomposition;
  //int NumRelationsgreaterLengthThan2;
  int NumGmodKtableRowsAllowedLatex;
  int NumTotalSubalgebras;
  bool flagFirstRoundCounting;
  bool flagComputeConeCondition;
  bool flagMakingProgressReport;
  bool flagAnErrorHasOccuredTimeToPanic;
  static int ProblemCounter;
  static int ProblemCounter2;
  List<int> indicesSubalgebrasContainingK;
  multTableKmods theMultTable;
  List<int> theOppositeKmods;
  DynkinDiagramRootSubalgebra theDynkinDiagram;
  DynkinDiagramRootSubalgebra theCentralizerDiagram;
  List<List<int> > coneRelationsBuffer;
  List<int> coneRelationsNumSameTypeComponentsTaken;
  List<DynkinDiagramRootSubalgebra> relationsDiagrams;
  WeylGroup AmbientWeyl;
  Vectors<Rational> genK;
  Vectors<Rational> SimpleBasisK;
  Vectors<Rational> PositiveRootsK;
  Vectors<Rational> AllRootsK;
  Selection NilradicalKmods;
  Selection CentralizerKmods;
  Vectors<Rational> LowestWeightsGmodK;
  Vectors<Rational> HighestWeightsGmodK;
  Vectors<Rational> HighestRootsK;
  Vectors<Rational> TestedRootsAlpha;
  Vectors<Rational> CentralizerRoots;
  Vectors<Rational> SimpleBasisCentralizerRoots;
  Vectors<Rational> SimpleBasisKEpsCoords;
  Vectors<Rational> SimpleBasisgEpsCoords;
  List<Vectors<Rational> >  kModulesKepsCoords;
  List<Vectors<Rational> >  kModulesgEpsCoords;
  List<Vectors<Rational> > kModules;
  List<Vectors<Rational> > PosRootsKConnectedComponents;
  List<Selection> theKEnumerations;
  List<int> theKComponentRanks;
  std::string DebugString;
  rootSubalgebra();
  //returns -1 if the weight/Vector<Rational>  is not in g/k
  int GetIndexKmoduleContainingRoot(Vector<Rational> & input);
  void GetCoxeterPlane
  (Vector<double>& outputBasis1, Vector<double>& outputBasis2, GlobalVariables& theGlobalVariables)
  ;
  void GetCoxeterElement(Matrix<Rational> & output);
  bool IsGeneratingSingularVectors(int indexKmod, Vectors<Rational>& NilradicalRoots);
  bool rootIsInCentralizer(Vector<Rational> & input);
  bool IsBKhighest(Vector<Rational> & input);
  bool rootIsInNilradicalParabolicCentralizer(Selection& positiveSimpleRootsSel, Vector<Rational> & input);
  void ComputeEpsCoordsWRTk(GlobalVariables& theGlobalVariables);
  bool AttemptTheTripleTrick(coneRelation& theRel, Vectors<Rational>& NilradicalRoots, GlobalVariables& theGlobalVariables);
  bool AttemptTheTripleTrickWRTSubalgebra(coneRelation& theRel, Vectors<Rational>& highestWeightsAllowed, Vectors<Rational>& NilradicalRoots, GlobalVariables& theGlobalVariables);
  void ExtractRelations(Matrix<Rational> & matA, Matrix<Rational> & matX, Vectors<Rational>& NilradicalRoots, rootSubalgebras& owner, int indexInOwner, GlobalVariables& theGlobalVariables, Vectors<Rational>& Ksingular);
  bool GenerateIsomorphismsPreservingBorel(rootSubalgebra& right, GlobalVariables& theGlobalVariables, ReflectionSubgroupWeylGroup* outputAutomorphisms, bool actOnCentralizerOnly);
  void GenerateAutomorphismsPreservingBorel(GlobalVariables& theGlobalVariables, ReflectionSubgroupWeylGroup& outputAutomorphisms);
  void MakeGeneratingSingularVectors(coneRelation& theRelation, Vectors<Rational>& nilradicalRoots);
  bool attemptExtensionToIsomorphismNoCentralizer(Vectors<Rational>& Domain, Vectors<Rational>& Range, GlobalVariables& theGlobalVariables, int RecursionDepth, ReflectionSubgroupWeylGroup* outputAutomorphisms, bool GenerateAllpossibleExtensions, bool* abortKmodule, Vectors<Rational>* additionalDomain, Vectors<Rational>* additionalRange);
  static bool attemptExtensionToIsomorphism(Vectors<Rational>& Domain, Vectors<Rational>& Range, GlobalVariables& theGlobalVariables, ReflectionSubgroupWeylGroup* outputAutomorphisms, bool actOnCentralizerOnly, WeylGroup& theWeyl, bool *DomainAndRangeGenerateNonIsoSAs);
  bool CheckForSmallRelations(coneRelation& theRel, Vectors<Rational>& nilradicalRoots);
  int NumRootsInNilradical();
  void MakeSureAlphasDontSumToRoot(coneRelation& theRel, Vectors<Rational>& NilradicalRoots);
  bool IsARoot(const Vector<Rational> & input);
  bool IsARootOrZero(Vector<Rational> & input);
  void KEnumerationsToLinComb(GlobalVariables& theGlobalVariables);
  void DoKRootsEnumeration(GlobalVariables& theGlobalVariables);
  void ComputeCentralizerFromKModulesAndSortKModules();
  void MatrixToRelation(coneRelation& output, Matrix<Rational> & matA, Matrix<Rational> & matX, int theDimension, Vectors<Rational>& NilradicalRoots);
  void DoKRootsEnumerationRecursively(int indexEnumeration, GlobalVariables& theGlobalVariables);
  void MakeProgressReportPossibleNilradicalComputation(GlobalVariables& theGlobalVariables, rootSubalgebras& owner, int indexInOwner);
  void MakeProgressReportGenAutos(int progress, int outOf, int found, GlobalVariables& theGlobalVariables);
  void ComputeDebugString(GlobalVariables& theGlobalVariables);
  void ComputeDebugString(bool useLatex, bool useHtml, bool includeKEpsCoords, GlobalVariables& theGlobalVariables){ this->ElementToString(this->DebugString, useLatex, useHtml, includeKEpsCoords, theGlobalVariables); };
  bool IndexIsCompatibleWithPrevious(int startIndex, int RecursionDepth,  multTableKmods& multTable, List<Selection>& impliedSelections, List<int>& oppositeKmods, rootSubalgebras& owner, GlobalVariables& theGlobalVariables);
  bool IsAnIsomorphism(Vectors<Rational>& domain, Vectors<Rational>& range, GlobalVariables& theGlobalVariables, ReflectionSubgroupWeylGroup* outputAutomorphisms, Vectors<Rational>* additionalDomain, Vectors<Rational>* additionalRange);
  bool ListHasNonSelectedIndexLowerThanGiven(int index, List<int>& tempList, Selection& tempSel);
  void GeneratePossibleNilradicalsRecursive(Controller& PauseMutex, GlobalVariables& theGlobalVariables, multTableKmods& multTable, List<Selection>& impliedSelections, List<int>& oppositeKmods, rootSubalgebras& owner, int indexInOwner);
  void GeneratePossibleNilradicals(Controller& PauseMutex, List<Selection>& impliedSelections, int& parabolicsCounter, GlobalVariables& theGlobalVariables, bool useParabolicsInNilradical, rootSubalgebras& owner, int indexInOwner);
  void GeneratePossibleNilradicalsInit(List<Selection>& impliedSelections, int& parabolicsCounter);
  void WriteToFileNilradicalGeneration(std::fstream& output, GlobalVariables* theGlobalVariables, rootSubalgebras& owner);
  void ReadFromFileNilradicalGeneration(std::fstream& input, GlobalVariables* theGlobalVariables, rootSubalgebras& owner);
  bool ConeConditionHolds(GlobalVariables& theGlobalVariables, rootSubalgebras& owner, int indexInOwner, bool doExtractRelations);
  bool ConeConditionHolds(GlobalVariables& theGlobalVariables, rootSubalgebras& owner, int indexInOwner, Vectors<Rational>& NilradicalRoots, Vectors<Rational>& Ksingular, bool doExtractRelations);
  void PossibleNilradicalComputation(GlobalVariables& theGlobalVariables, Selection& selKmods, rootSubalgebras& owner, int indexInOwner);
  void ElementToStringHeaderFooter(std::string& outputHeader, std::string& outputFooter, bool useLatex, bool useHtml, bool includeKEpsCoords);
  void ElementToString(std::string& output, GlobalVariables& theGlobalVariables){this->ElementToString(output, false, false, false, theGlobalVariables); };
  void ElementToHtml(int index, std::string& path, SltwoSubalgebras* sl2s, GlobalVariables& theGlobalVariables);
  void ElementToString(std::string& output, bool useLatex, bool useHtml, bool includeKEpsCoords, GlobalVariables& theGlobalVariables){this->ElementToString(output, 0, 0, useLatex, useHtml, includeKEpsCoords, theGlobalVariables); };
  void ElementToString(std::string& output, SltwoSubalgebras* sl2s, int indexInOwner, bool useLatex, bool useHtml, bool includeKEpsCoords, GlobalVariables& theGlobalVariables);
  bool RootsDefineASubalgebra(Vectors<Rational>& theRoots);
  void GenerateKmodMultTable(List<List<List<int> > >& output, List<int>& oppositeKmods, GlobalVariables& theGlobalVariables);
  void MakeProgressReportMultTable(int index, int outOf, GlobalVariables& theGlobalVariables);
  void KmodTimesKmod(int index1, int index2, List<int>& oppositeKmods, List<int>& output);
  void initFromAmbientWeyl();
  void GetSsl2SubalgebrasAppendListNoRepetition(SltwoSubalgebras& output, int indexInContainer, GlobalVariables& theGlobalVariables, SemisimpleLieAlgebra& theLieAlgebra);
  void ComputeAllButAmbientWeyl();
  void ComputeDynkinDiagramKandCentralizer();
  void ComputeAll();
  void ComputeRootsOfK();
  void ComputeKModules();
  void ComputeHighestWeightInTheSameKMod(Vector<Rational> & input, Vector<Rational> & outputHW);
  void ComputeExtremeWeightInTheSameKMod(Vector<Rational> & input, Vector<Rational> & outputW, bool lookingForHighest);
  inline void operator=(const rootSubalgebra& right){this->Assign(right); }
  void Assign(const rootSubalgebra& right);
  void ComputeLowestWeightInTheSameKMod(Vector<Rational> & input, Vector<Rational> & outputLW);
  void GetLinearCombinationFromMaxRankRootsAndExtraRoot(bool DoEnumeration, GlobalVariables& theGlobalVariables);
//  void commonCodeForGetLinearCombinationFromMaxRankRootsAndExtraRoot();
  void initForNilradicalGeneration();
  void GetLinearCombinationFromMaxRankRootsAndExtraRootMethod2(GlobalVariables& theGlobalVariables);
  bool LinCombToString(Vector<Rational> & alphaRoot, int coeff, Vector<Rational> & linComb, std::string& output);
  bool LinCombToStringDistinguishedIndex(int distinguished, Vector<Rational> & alphaRoot, int coeff, Vector<Rational> & linComb, std::string& output);
  void WriteMultTableAndOppositeKmodsToFile(std::fstream& output, List<List<List<int> > >& inMultTable, List<int>& inOpposites);
  void ReadMultTableAndOppositeKmodsFromFile(std::fstream& input, List<List<List<int> > >& outMultTable, List<int>& outOpposites);
};

class rootSubalgebras: public List<rootSubalgebra>
{
public:
  std::string DebugString;
  coneRelations theBadRelations;
  coneRelations theGoodRelations;
  coneRelations theMinRels;
  List<List<int> > ActionsNormalizerCentralizerNilradical;
  List<ReflectionSubgroupWeylGroup> CentralizerOuterIsomorphisms;
  List<ReflectionSubgroupWeylGroup> CentralizerIsomorphisms;
  //Code used in nilradical generation:
  List<Selection> ImpiedSelectionsNilradical;
  List<List<List<int> > > storedNilradicals;
  List<SemisimpleLieAlgebra>* ownerArray;
  int indexInOnwer;
  int parabolicsCounterNilradicalGeneration;
  List<int> numNilradicalsBySA;
  int IndexCurrentSANilradicalsGeneration;
  int NumReductiveRootSAsToBeProcessedNilradicalsGeneration;
  List<int> CountersNilradicalsGeneration;
  List<int> NumConeConditionHoldsBySSpart;
  int RecursionDepthNilradicalsGeneration;
  Controller controllerLProhibitingRelations;
  int NumSubalgebrasProcessed;
  int NumConeConditionFailures;
  int NumSubalgebrasCounted;
  int NumLinesPerTableLatex;
  int NumColsPerTableLatex;
  int UpperLimitNumElementsWeyl;
  static int ProblemCounter;
  WeylGroup AmbientWeyl;
  std::string ReportStringNonNilradicalParabolic;
  bool flagComputingLprohibitingWeights;
  bool flagUseDynkinClassificationForIsomorphismComputation;
  bool flagUsingParabolicsInCentralizers;
  bool flagUsingActionsNormalizerCentralizerNilradical;
  bool flagNilradicalComputationInitialized;
  bool flagCountingNilradicalsOnlyNoComputation;
  bool flagComputeConeCondition;
  bool flagLookingForMinimalRels;
  bool flagStoringNilradicals;
  void ComputeKmodMultTables(GlobalVariables& theGlobalVariables);
  bool ApproveKmoduleSelectionWRTActionsNormalizerCentralizerNilradical(Selection& targetSel, GlobalVariables& theGlobalVariables);
  bool ApproveSelAgainstOneGenerator(List<int>& generator, Selection& targetSel, GlobalVariables& theGlobalVariables);
  void RaiseSelectionUntilApproval(Selection& targetSel, GlobalVariables& theGlobalVariables);
  void ApplyOneGenerator(List<int>& generator, Selection& targetSel, GlobalVariables& theGlobalVariables);
  void GenerateActionKintersectBIsos(rootSubalgebra& theRootSA, GlobalVariables& theGlobalVariables);
  void GenerateKintersectBOuterIsos(rootSubalgebra& theRootSA, GlobalVariables& theGlobalVariables);
  void ComputeActionNormalizerOfCentralizerIntersectNilradical(Selection& SelectedBasisRoots, rootSubalgebra& theRootSA, GlobalVariables& theGlobalVariables);
  void ComputeNormalizerOfCentralizerIntersectNilradical(ReflectionSubgroupWeylGroup& outputSubgroup, Selection& SelectedBasisRoots, rootSubalgebra& theRootSA, GlobalVariables& theGlobalVariables);
  void GenerateAllReductiveRootSubalgebrasUpToIsomorphism(GlobalVariables& theGlobalVariables, char WeylLetter, int WeylRank, bool sort, bool computeEpsCoords);
  void GenerateAllReductiveRootSubalgebrasUpToIsomorphism(GlobalVariables& theGlobalVariables, bool sort, bool computeEpsCoords);
  bool IsANewSubalgebra(rootSubalgebra& input, GlobalVariables& theGlobalVariables);
  int IndexSubalgebra(rootSubalgebra& input, GlobalVariables& theGlobalVariables);
  void GenerateAllReductiveRootSubalgebrasContainingInputUpToIsomorphism(rootSubalgebras& bufferSAs, int RecursionDepth, GlobalVariables& theGlobalVariables);
  void ElementToStringDynkinTable(bool useLatex, bool useHtml, std::string* htmlPathPhysical, std::string* htmlPathServer, std::string& output);
  void GetTableHeaderAndFooter(std::string& outputHeader, std::string& outputFooter, bool useLatex, bool useHtml);
  void SortDescendingOrderBySSRank();
  void pathToHtmlFileNameElements(int index, std::string* htmlPathServer, std::string& output, bool includeDotHtml);
  void pathToHtmlReference(int index, std::string& DisplayString, std::string* htmlPathServer, std::string& output);
  void WriteToDefaultFileNilradicalGeneration(GlobalVariables* theGlobalVariables);
  bool ReadFromDefaultFileNilradicalGeneration(GlobalVariables* theGlobalVariables);
  void WriteToFileNilradicalGeneration(std::fstream& output, GlobalVariables* theGlobalVariables);
  void ReadFromFileNilradicalGeneration(std::fstream& input, GlobalVariables* theGlobalVariables);
  void ElementToStringRootSpaces(std::string& output, bool includeMatrixForm, Vectors<Rational>& input, GlobalVariables& theGlobalVariables);
  void ElementToStringConeConditionNotSatisfying(std::string& output, bool includeMatrixForm, GlobalVariables& theGlobalVariables);
  void ElementToHtml(std::string& header, std::string& pathPhysical, std::string& htmlPathServer, SltwoSubalgebras* Sl2s, GlobalVariables& theGlobalVariables);
  void ElementToStringCentralizerIsomorphisms(std::string& output, bool useLatex, bool useHtml, int fromIndex, int NumToProcess, GlobalVariables& theGlobalVariables);
  void ElementToString
(std::string& output, SltwoSubalgebras* sl2s, bool useLatex, bool useHtml, bool includeKEpsCoords,
 std::string* htmlPathPhysical, std::string* htmlPathServer, GlobalVariables& theGlobalVariables, const std::string& DisplayNameCalculator)
  ;
  void ComputeLProhibitingRelations(GlobalVariables& theGlobalVariables);
  void ComputeAllRootSubalgebrasUpToIso(GlobalVariables& theGlobalVariables, int StartingIndex, int NumToBeProcessed);
  void ComputeDebugString
  (bool useLatex, bool useHtml, bool includeKEpsCoords, std::string* htmlPathPhysical,
   std::string* htmlPathServer, GlobalVariables& theGlobalVariables)
  { this->ElementToString(this->DebugString, 0, useLatex, useHtml, includeKEpsCoords, htmlPathPhysical, htmlPathServer, theGlobalVariables, "");
  }
  void MakeProgressReportGenerationSubalgebras(rootSubalgebras& bufferSAs, int RecursionDepth, GlobalVariables& theGlobalVariables, int currentIndex, int TotalIndex);
  void MakeProgressReportAutomorphisms(ReflectionSubgroupWeylGroup& theSubgroup, rootSubalgebra& theRootSA, GlobalVariables& theGlobalVariables);
  void initForNilradicalGeneration()
  { this->NumSubalgebrasProcessed=0;
    this->NumConeConditionFailures=0;
    this->NumSubalgebrasCounted=0;
    this->IndexCurrentSANilradicalsGeneration=0;
    this->ReportStringNonNilradicalParabolic="";
    this->NumReductiveRootSAsToBeProcessedNilradicalsGeneration=this->size-1;
    if (this->size>0)
    { this->TheObjects[0].GeneratePossibleNilradicalsInit(this->ImpiedSelectionsNilradical, this->parabolicsCounterNilradicalGeneration);
      this->NumConeConditionHoldsBySSpart.initFillInObject(this->size, 0);
    }
  }
  rootSubalgebras()
  { this->flagNilradicalComputationInitialized=false;
    this->flagStoringNilradicals=false;
    this->flagUsingParabolicsInCentralizers=true;
    this->flagUseDynkinClassificationForIsomorphismComputation=true;
    this->flagCountingNilradicalsOnlyNoComputation = false;
    this->flagComputingLprohibitingWeights=false;
    this->flagComputeConeCondition=false;
    this->flagUsingActionsNormalizerCentralizerNilradical=true;
    this->flagLookingForMinimalRels=false;
    this->NumLinesPerTableLatex=20;
    this->NumColsPerTableLatex=4;
    this->UpperLimitNumElementsWeyl=0;
    this->ownerArray=0;
    this->indexInOnwer=-1;
    this->initForNilradicalGeneration();
  }
};

class slTwo
{
public:
  std::string DebugString;
  void ElementToString(std::string& output, GlobalVariables& theGlobalVariables, SltwoSubalgebras& container, int indexInContainer, bool useLatex, bool useHtml, bool usePNG, std::string* physicalPath, std::string* htmlPathServer, FormatExpressions& PolyFormatLocal);
  void ElementToString(std::string& output, GlobalVariables& theGlobalVariables, SltwoSubalgebras& container, bool useLatex, bool useHtml, FormatExpressions& PolyFormatLocal){ this->ElementToString(output, theGlobalVariables, container, 0, useLatex, useHtml, false, 0, 0, PolyFormatLocal);}
  void ElementToStringModuleDecomposition(bool useLatex, bool useHtml, std::string& output);
  void ElementToStringModuleDecompositionMinimalContainingRegularSAs(bool useLatex, bool useHtml, SltwoSubalgebras& owner, std::string& output);
  void ComputeDebugString(bool useHtml, bool useLatex, GlobalVariables& theGlobalVariables, SltwoSubalgebras& container){ FormatExpressions PolyFormatLocal; this->ElementToString(this->DebugString, theGlobalVariables, container, useLatex, useHtml, PolyFormatLocal); }
  List<int> highestWeights;
  List<int> multiplicitiesHighestWeights;
  List<int> weightSpaceDimensions;
  ElementSemisimpleLieAlgebra theH, theE, theF;
  ElementSemisimpleLieAlgebra bufferHbracketE, bufferHbracketF, bufferEbracketF;
  SemisimpleLieAlgebra* owner;
  List<int> IndicesContainingRootSAs;
  List<int> IndicesMinimalContainingRootSA;
  List<List<int> > HighestWeightsDecompositionMinimalContainingRootSA;
  List<List<int> > MultiplicitiesDecompositionMinimalContainingRootSA;
  Vectors<Rational> preferredAmbientSimpleBasis;
  Vector<Rational>  hCharacteristic;
  Vector<Rational>  hElementCorrespondingToCharacteristic;
  Vectors<Rational> hCommutingRootSpaces;
  Vectors<Rational> RootsHavingScalarProduct2WithH;
  DynkinDiagramRootSubalgebra DiagramM;
  DynkinDiagramRootSubalgebra CentralizerDiagram;
  PolynomialSubstitution<Rational> theSystemToBeSolved;
  Matrix<Rational>  theSystemMatrixForm;
  Matrix<Rational>  theSystemColumnVector;
  bool DifferenceTwoHsimpleRootsIsARoot;
  int DynkinsEpsilon;
  void ComputeModuleDecomposition(Vectors<Rational>& positiveRootsContainingRegularSA, int dimensionContainingRegularSA, List<int>& outputHighestWeights, List<int>& outputMultiplicitiesHighestWeights, List<int>& outputWeightSpaceDimensions, GlobalVariables& theGlobalVariables);
  void ComputeModuleDecompositionAmbientLieAlgebra(GlobalVariables& theGlobalVariables);
  void ComputeModuleDecompositionOfMinimalContainingRegularSAs(SltwoSubalgebras& owner, int IndexInOwner, GlobalVariables& theGlobalVariables);
  bool ModuleDecompositionFitsInto(const slTwo& other);
  static bool ModuleDecompositionFitsInto(const List<int>& highestWeightsLeft, const List<int>& multiplicitiesHighestWeightsLeft, const List<int>& highestWeightsRight, const List<int>& multiplicitiesHighestWeightsRight);
  void MakeReportPrecomputations(GlobalVariables& theGlobalVariables, SltwoSubalgebras& container, int indexInContainer, int indexMinimalContainingRegularSA, rootSubalgebra& MinimalContainingRegularSubalgebra);
  //the below is outdated, must be deleted as soon as equivalent code is written.
  void ComputeDynkinsEpsilon(WeylGroup& theWeyl);
  void ElementToHtml(std::string& filePath);
  void ElementToHtmlCreateFormulaOutputReference(const std::string& formulaTex, std::stringstream& output, bool usePNG, bool useHtml, SltwoSubalgebras& container, std::string* physicalPath, std::string* htmlPathServer);
  void operator=(const slTwo& right)
  { this->highestWeights.CopyFromBase(right.highestWeights);
    this->multiplicitiesHighestWeights.CopyFromBase(right.multiplicitiesHighestWeights);
    this->weightSpaceDimensions.CopyFromBase(right.weightSpaceDimensions);
    this->HighestWeightsDecompositionMinimalContainingRootSA.CopyFromBase(right.HighestWeightsDecompositionMinimalContainingRootSA);
    this->MultiplicitiesDecompositionMinimalContainingRootSA.CopyFromBase(right.MultiplicitiesDecompositionMinimalContainingRootSA);
    this->hCommutingRootSpaces.CopyFromBase(right.hCommutingRootSpaces);
    this->CentralizerDiagram.Assign(right.CentralizerDiagram);
    this->DiagramM.Assign(right.DiagramM);
    this->hCommutingRootSpaces.CopyFromBase(right.hCommutingRootSpaces);
    this->DifferenceTwoHsimpleRootsIsARoot=right.DifferenceTwoHsimpleRootsIsARoot;
    this->RootsHavingScalarProduct2WithH=right.RootsHavingScalarProduct2WithH;
    this->DynkinsEpsilon=right.DynkinsEpsilon;
    this->hCharacteristic=(right.hCharacteristic);
    this->hElementCorrespondingToCharacteristic=(right.hElementCorrespondingToCharacteristic);
    this->owner = right.owner;
    this->theE= right.theE;
    this->theH= right.theH;
    this->theF= right.theF;
    this->bufferEbracketF= right.bufferEbracketF;
    this->bufferHbracketE=right.bufferHbracketE;
    this->bufferHbracketF=right.bufferHbracketF;
    this->theSystemToBeSolved=right.theSystemToBeSolved;
    this->theSystemMatrixForm=right.theSystemMatrixForm;
    this->theSystemColumnVector= right.theSystemColumnVector;
    this->IndicesContainingRootSAs.CopyFromBase(right.IndicesContainingRootSAs);
    this->preferredAmbientSimpleBasis= right.preferredAmbientSimpleBasis;
  }
  bool  operator==(const slTwo& right)
  {// See Dynkin, Semisimple Lie subalgebras of semisimple Lie algebras, chapter 7-10
     return this->hCharacteristic==(right.hCharacteristic);
  }
  int HashFunction() const
  { int tempI=MathRoutines::Minimum(SomeRandomPrimesSize, this->hCharacteristic.size);
    int result=0;
    for (int i=0; i<tempI; i++)
      result+= this->hCharacteristic.TheObjects[i].NumShort*SomeRandomPrimes[i];
    return result;
  }
  static inline int HashFunction(const slTwo& input)
  { return input.HashFunction();
  }

};

class VectorPartition
{
public:
  Vectors<Rational> PartitioningRoots;
  Vector<Rational>  theRoot;
  //format: each element of thePartitions gives an array whose entries give
  // the multiplicity of the weights. I.e. if PartitioningRoots has 2 elements, then thePartitions.TheObjects[0]
  // would have 2 elements: the first giving the multiplicity of PartitioningRoots.TheObjects[0] and the second - the multiplicity of
  // PartitioningRoots.TheObjects[1]
  List<List<int> > thePartitions;
  std::string DebugString;
  std::string ElementToString(bool useHtml);
  void ComputeDebugString(bool useHtml){this->DebugString=this->ElementToString(useHtml);}
  int ComputeVectorPartitionFunctionSmall(Vector<Rational> & theRoot, Vectors<Rational>& theRoots);
  void ComputeAllPartitions();
  void ComputeAllPartitionsRecursive(int currentIndex, List<int>& CurrentPartition, int UpperBoundEachIndex, Vector<Rational> & toBePartitioned);
};

class SemisimpleLieAlgebra
{
public:
  List<SemisimpleLieAlgebra>* owner;
  int indexInOwner;
  bool flagAnErrorHasOccurredTimeToPanic;
  WeylGroup theWeyl;
  //format:
  //the Chevalley constants are listed in the same order as the Vector<Rational>  system of the Weyl group
  // i.e. if \alpha is the Vector<Rational>  at the i^th position in this->theWyl.RootSystem and \beta -
  //the Vector<Rational>  at the j^th position, then
  //the chevalley constant N_{\alpha\beta} given by [g^\alpha, g^\beta]=N_{\alpha\beta}g^{\alpha+\beta}
  //will be located at the ij^{th} entry in the below matrix.
  //Let $\alpha$ be a Vector<Rational> . Then our choice of the elements of the Cartan subalgebra is such that
  //1.   [g^{\alpha}, g^{-\alpha}]=h_\alpha * (2/ \langle\alpha,\alpha\rangle)
  //2.   [h_{\alpha},g^\beta] :=\langle\alpha,\beta\rangle g^\beta
  //Reference: Samelson, Notes on Lie algebras, pages 46-51
  Matrix<Rational> ChevalleyConstants;
  Matrix<bool> Computed;
  //The below gives a total ordering to all generators, including the elements of the Cartan
  //the order is:  We put first the generators corresponding to the negative roots in ascending order,
  //we put second the elements of the Cartan
  //we put last the positive roots in ascending order.
//  Matrix<int> theLiebracketPairingIndices;
  Matrix<ElementSemisimpleLieAlgebra> theLiebrackets;
//  List<int> OppositeRootSpaces;
  List<int> UEGeneratorOrderIncludingCartanElts;
  void Assign(const SemisimpleLieAlgebra& other)
  { this->owner=other.owner;
    this->indexInOwner=other.indexInOwner;
    this->theWeyl=(other.theWeyl);
    this->ChevalleyConstants=other.ChevalleyConstants;
//    this->OppositeRootSpaces.CopyFromBase(other.OppositeRootSpaces);
    this->theLiebrackets=(other.theLiebrackets);
//    this->theLiebracketPairingIndices=(other.theLiebracketPairingIndices);
    this->Computed=(other.Computed);
    this->UEGeneratorOrderIncludingCartanElts=other.UEGeneratorOrderIncludingCartanElts;
  }
  SemisimpleLieAlgebra(): owner(0), indexInOwner(-1){ }
  void ComputeMultTable(GlobalVariables& theGlobalVariables);
  void CheckConsistency()
  { if (this->owner==0 || this->indexInOwner==-1)
    { std::cout << "This is a programming error. Use of non-initialized semisimple Lie algebra. Please debug file "
      << __FILE__ << " line " << __LINE__;
      assert(false);
    }
  }
  std::string ElementToString(FormatExpressions* inputFormat);
  std::string GetStringFromChevalleyGenerator
  (int theIndex, FormatExpressions* thePolynomialFormat)const
  ;
  bool CommutatorIsNonZero(int leftIndex, int rightIndex)
  { return !this->theLiebrackets.elements[leftIndex][rightIndex].IsEqualToZero();
  }
  std::string GetLieAlgebraName(bool includeNonTechnicalNames=true)const
  { return this->GetLieAlgebraName(this->theWeyl.WeylLetter, this->GetRank(), includeNonTechnicalNames);
  }
  static std::string GetLieAlgebraName(char WeylLetter, int WeylDim, bool includeNonTechnicalNames=true)
  { std::stringstream out;
    out << WeylLetter << "_" << WeylDim;
    if (includeNonTechnicalNames)
      if (WeylLetter!='E' && WeylLetter!='F' && WeylLetter!='G')
        switch (WeylLetter)
        { case 'A':  out << "(sl(" << WeylDim+1 << "))"; break;
          case 'B':  out << "(so(" << 2*WeylDim+1 << "))"; break;
          case 'C':  out << "(sp(" << 2*WeylDim << "))"; break;
          case 'D':  out << "(so(" << 2*WeylDim << "))"; break;
          default: out << "(" << WeylLetter << "_" << WeylDim << ")"; break;
        }
    return out.str();
  }
  void GetMinusTransposeAuto(const ElementSemisimpleLieAlgebra& input, ElementSemisimpleLieAlgebra& output);
  void GenerateWeightSupportMethod2(Vector<Rational> & theHighestWeight, Vectors<Rational>& output, GlobalVariables& theGlobalVariables);
  inline int GetNumGenerators()const{ return this->theWeyl.CartanSymmetric.NumRows+this->theWeyl.RootSystem.size;}
  inline int GetNumPosRoots()const{ return this->theWeyl.RootsOfBorel.size;}
  inline int GetRank()const{ return this->theWeyl.CartanSymmetric.NumRows;}
  int CartanIndexToChevalleyGeneratorIndex(int theIndex){ return this->theWeyl.RootsOfBorel.size+theIndex;}
  int RootToIndexInUE(const Vector<Rational> & input){ return this->RootIndexOrderAsInRootSystemToGeneratorIndexNegativeRootsThenCartanThenPositive(this->theWeyl.RootSystem.GetIndex(input));}
  int DisplayIndexToRootIndex(int theIndex);
  int DisplayIndexToChevalleyGeneratorIndex(int theIndex)
  { if (theIndex<0)
      return theIndex+this->GetNumPosRoots();
    return theIndex+this->GetNumPosRoots()+this->GetRank()-1;
  }
  int RootIndexOrderAsInRootSystemToGeneratorIndexNegativeRootsThenCartanThenPositive(int theIndex)const;
  int RootIndexToDisplayIndexNegativeSpacesFirstThenCartan(int theIndex)const;
  void OrderSetNilradicalNegativeMost(Selection& parSelZeroMeansLeviPart)
  { for (int i=0; i<this->GetNumPosRoots(); i++)
    { int translationCoeff=0;
      for (int j=0; j<this->GetRank(); j++)
        if (parSelZeroMeansLeviPart.selected[j])
          translationCoeff+=this->theWeyl.RootSystem[i][j].NumShort*this->GetNumPosRoots();
      this->UEGeneratorOrderIncludingCartanElts[i]=i+translationCoeff;
    }
  }
  void OrderSSalgebraForHWbfComputation()
  { int numPosRoots=this->GetNumPosRoots();
    for (int i=0; i<numPosRoots; i++)
      this->UEGeneratorOrderIncludingCartanElts[i]=-1;
  }
  void OrderSSLieAlgebraStandard()
  { int numGens=this->GetNumGenerators();
    for (int i=0; i<numGens; i++)
      this->UEGeneratorOrderIncludingCartanElts[i]=i;
  }
  //the below function returns an negative number if the chevalley generator is an element of the Cartan subalgebra
  int ChevalleyGeneratorIndexToRootIndex(int theIndex)const;
  int ChevalleyGeneratorIndexToElementCartanIndex(int theIndex){ return theIndex-this->theWeyl.RootsOfBorel.size;}
  int ChevalleyGeneratorIndexToDisplayIndex(int theIndex)const
  { //std::cout << "<br>num pos roots: " <<  this->GetNumPosRoots();
   // std::cout << " rank: "<< this->GetRank();
    if (theIndex<this->GetNumPosRoots())
      return theIndex-this->GetNumPosRoots();
    if (theIndex>=this->GetNumPosRoots()+this->GetRank())
      return theIndex+1-this->GetNumPosRoots()-this->GetRank();
    return theIndex-this->GetNumPosRoots();
  }
  bool AreOrderedProperly(int leftIndex, int rightIndex);
  bool IsGeneratorFromCartan(int theIndex)const
  { return theIndex>=this->GetNumPosRoots() && theIndex<this->GetNumPosRoots()+this->GetRank();
  }
  bool AreOppositeRootSpaces(int leftIndex, int rightIndex)
  { if(this->IsGeneratorFromCartan(leftIndex) || this->IsGeneratorFromCartan(rightIndex))
      return false;
    int left  = this->ChevalleyGeneratorIndexToRootIndex(leftIndex);
    int right=this->ChevalleyGeneratorIndexToRootIndex(rightIndex);
    return (this->theWeyl.RootSystem[left]+this->theWeyl.RootSystem[right]).IsEqualToZero();
  }
  void GenerateVermaMonomials(Vector<Rational> & highestWeight, GlobalVariables& theGlobalVariables)
  ;
  void ComputeChevalleyConstantS
(GlobalVariables& theGlobalVariables)
  ;
  //Setup: \gamma+\delta=\epsilon+\zeta=\eta is a Vector<Rational> .
  //then the below function computes n_{-\epsilon, -\zeta}
  void LieBracket(const ElementSemisimpleLieAlgebra& g1, const ElementSemisimpleLieAlgebra& g2, ElementSemisimpleLieAlgebra& output);
  void ComputeOneChevalleyConstant(int indexGamma, int indexDelta, int indexMinusEpsilon, int indexMinusZeta, int indexEta);
  void ExploitSymmetryAndCyclicityChevalleyConstants(int indexI, int indexJ);
  void ExploitSymmetryChevalleyConstants(int indexI, int indexJ);
  void ExploitTheCyclicTrick(int i, int j, int k);
  int GetMaxQForWhichBetaMinusQAlphaIsARoot(Vector<Rational> & alpha, Vector<Rational> & beta);
  Rational GetConstant(const Vector<Rational> & root1, const Vector<Rational> & root2);
  void ComputeCommonAdEigenVectors
(int theDegree, List<ElementUniversalEnveloping<Polynomial<Rational> > >& theGenerators,
 List<ElementUniversalEnveloping<Polynomial<Rational> > >& generatorsBeingActedOn,
 List<ElementUniversalEnveloping<Polynomial<Rational> > >& output, std::stringstream& out,
 GlobalVariables& theGlobalVariables)
  ;
  void ComputeCommonAdEigenVectorsFixedWeight
(Vector<Rational> & theWeight, Vectors<Rational>& theHs, List<ElementUniversalEnveloping<Polynomial<Rational> > >& theGenerators,
 List<ElementUniversalEnveloping<Polynomial<Rational> > >& output, std::stringstream& out,
 GlobalVariables& theGlobalVariables)
;
  bool CheckClosedness(std::string& output, GlobalVariables& theGlobalVariables);
  void ElementToStringVermaMonomials(std::string& output);
  void ElementToStringEmbedding(std::string& output);
  int GetChevalleyGeneratorIndexCorrespondingToNonZeroRootSpace(const Vector<Rational>& input)const
  { int theIndex=this->theWeyl.RootSystem.GetIndex(input);
    if (theIndex>=this->GetNumPosRoots())
      theIndex+=this->GetRank();
    return theIndex;
  }
  Vector<Rational> GetWeightOfGenerator(int index)
  { if (index<this->GetNumPosRoots())
      return this->theWeyl.RootSystem[index];
    if (index>=this->GetRank()+this->GetNumPosRoots())
      return this->theWeyl.RootSystem[index-this->GetRank()];
    Vector<Rational> result;
    result.MakeZero(this->GetRank());
    return result;
  }
  //returns true if returning constant, false if returning element of h
  bool GetConstantOrHElement(const Vector<Rational> & root1, const Vector<Rational> & root2, Rational& outputRat, Vector<Rational> & outputH);
  bool TestForConsistency(GlobalVariables& theGlobalVariables);
  bool FindComplementaryNilpotent(ElementSemisimpleLieAlgebra& e, ElementSemisimpleLieAlgebra& output, GlobalVariables& theGlobalVariables);
  bool AttemptExtendingHEtoHEF(Vector<Rational> & h, ElementSemisimpleLieAlgebra& e, ElementSemisimpleLieAlgebra& output, GlobalVariables& theGlobalVariables);
  bool AttemptExtendingHEtoHEFWRTSubalgebra(Vectors<Rational>& RootsWithCharacteristic2, Vectors<Rational>& relativeRootSystem, Selection& theZeroCharacteristics, Vectors<Rational>& simpleBasisSA, Vector<Rational> & h, ElementSemisimpleLieAlgebra& outputE, ElementSemisimpleLieAlgebra& outputF, Matrix<Rational> & outputMatrixSystemToBeSolved, PolynomialSubstitution<Rational>& outputSystemToBeSolved, Matrix<Rational> & outputSystemColumnVector, GlobalVariables& theGlobalVariables);
  void FindSl2Subalgebras(SltwoSubalgebras& output, char WeylLetter, int WeylRank, GlobalVariables& theGlobalVariables);
  void FindSl2Subalgebras(SltwoSubalgebras& output, GlobalVariables& theGlobalVariables);
  void GetSl2SubalgebraFromRootSA(GlobalVariables& theGlobalVariables);
  void GetAd(Matrix<Rational> & output, ElementSemisimpleLieAlgebra& e);
  void initHEFSystemFromECoeffs(int theRelativeDimension, Selection& theZeroCharacteristics, Vectors<Rational>& rootsInPlay, Vectors<Rational>& simpleBasisSA,  Vectors<Rational>& SelectedExtraPositiveRoots, int numberVariables, int numRootsChar2, int halfNumberVariables, Vector<Rational> & targetH, Matrix<Rational> & inputFCoeffs, Matrix<Rational> & outputMatrixSystemToBeSolved, Matrix<Rational> & outputSystemColumnVector, PolynomialSubstitution<Rational>& outputSystemToBeSolved);
  void MakeChevalleyTestReport(int i, int j, int k, int Total, GlobalVariables& theGlobalVariables);
  void MakeSl2ProgressReport(int progress, int found, int foundGood, int DifferentHs, int outOf, GlobalVariables& theGlobalVariables);
  void MakeSl2ProgressReportNumCycles(int progress, int outOf, GlobalVariables& theGlobalVariables);
  bool IsInTheWeightSupport(Vector<Rational> & theWeight, Vector<Rational> & highestWeight, GlobalVariables& theGlobalVariables);
  void GenerateOneMonomialPerWeightInTheWeightSupport(Vector<Rational> & theHighestWeight, GlobalVariables& theGlobalVariables);
  void CreateEmbeddingFromFDModuleHaving1dimWeightSpaces(Vector<Rational> & theHighestWeight, GlobalVariables& theGlobalVariables);
  int GetLengthStringAlongAlphaThroughBeta(Vector<Rational> & alpha, Vector<Rational> & beta, int& distanceToHighestWeight, Vectors<Rational>& weightSupport);
  void ComputeOneAutomorphism(GlobalVariables& theGlobalVariables, Matrix<Rational> & outputAuto,  bool useNegativeRootsFirst);
  void operator=(const SemisimpleLieAlgebra& other){ this->Assign(other);}
  bool operator==(const SemisimpleLieAlgebra& other)
  { return this->theWeyl==other.theWeyl;
  }
};

class Parser;
class HomomorphismSemisimpleLieAlgebra
{
public:
  List<SemisimpleLieAlgebra>* owners;
  int indexDomain;
  int indexRange;
  //Let rk:=Rank(Domain)
  //format of ImagesSimpleChevalleyGenerators: the first rk elements give the images of the Chevalley generators corresponding to simple positive Vectors<Rational>
  //the second rk elements give the images of the Chevalley generators corresponding to simple negative Vectors<Rational>
  List<ElementSemisimpleLieAlgebra> imagesSimpleChevalleyGenerators;
  //format of ImagesAllChevalleyGenerators: the Generators are given in the same order as the one used in MonomialUniversalEnveloping
  List<ElementSemisimpleLieAlgebra> imagesAllChevalleyGenerators;
  List<ElementSemisimpleLieAlgebra> domainAllChevalleyGenerators;
  List<ElementSemisimpleLieAlgebra> GmodK;
  Vectors<Rational> RestrictedRootSystem;
  SemisimpleLieAlgebra& theDomain()
  { return owners->TheObjects[this->indexDomain];
  }
  SemisimpleLieAlgebra& theRange()
  { return owners->TheObjects[this->indexRange];
  }
  HomomorphismSemisimpleLieAlgebra(): owners(0), indexDomain(0), indexRange(1){}
  std::string DebugString;
  void GetWeightsGmodKInSimpleCoordsK
  (Vectors<Rational>& outputWeights, GlobalVariables& theGlobalVariables)
  { this->GetWeightsWrtKInSimpleCoordsK(outputWeights, this->GmodK, theGlobalVariables);}
  void GetWeightsKInSimpleCoordsK
  (Vectors<Rational>& outputWeights, GlobalVariables& theGlobalVariables)
  { this->GetWeightsWrtKInSimpleCoordsK(outputWeights, this->imagesAllChevalleyGenerators, theGlobalVariables); }
  void GetWeightsWrtKInSimpleCoordsK
  (Vectors<Rational>& outputWeights, List<ElementSemisimpleLieAlgebra>& inputElts, GlobalVariables& theGlobalVariables)
  ;
  void ElementToString(std::string& output, GlobalVariables& theGlobalVariables) {this->ElementToString(output, false, theGlobalVariables);};
  void ElementToString(std::string& output, bool useHtml, GlobalVariables& theGlobalVariables);
  void MakeG2InB3(Parser& owner, GlobalVariables& theGlobalVariables);
  void MakeGinGWithId
  (char theWeylLetter, int theWeylDim, List<SemisimpleLieAlgebra>& ownerOfAlgebras,
   GlobalVariables& theGlobalVariables)
  ;
  void ProjectOntoSmallCartan(Vector<Rational> & input, Vector<Rational> & output, GlobalVariables& theGlobalVariables);
  void ProjectOntoSmallCartan(Vectors<Rational>& input, Vectors<Rational>& output, GlobalVariables& theGlobalVariables);
  void GetMapSmallCartanDualToLargeCartanDual(Matrix<Rational> & output);
  void ComputeDebugString(GlobalVariables& theGlobalVariables){this->ElementToString(this->DebugString, theGlobalVariables);}
  void ComputeDebugString(bool useHtml, GlobalVariables& theGlobalVariables){this->ElementToString(this->DebugString, useHtml, theGlobalVariables);}
  std::string ElementToString(GlobalVariables& theGlobalVariables){ std::string tempS; this->ElementToString(tempS, theGlobalVariables); return tempS; }
  void GetRestrictionAmbientRootSystemToTheSmallerCartanSA(Vectors<Rational>& output, GlobalVariables& theGlobalVariables);
  bool ComputeHomomorphismFromImagesSimpleChevalleyGenerators(GlobalVariables& theGlobalVariables);
  bool CheckClosednessLieBracket(GlobalVariables& theGlobalVariables);
  void ApplyHomomorphism
  (ElementSemisimpleLieAlgebra& input, ElementSemisimpleLieAlgebra& output)
  ;
  bool ApplyHomomorphism
  (ElementUniversalEnveloping<Polynomial<Rational> >& input,
   ElementUniversalEnveloping<Polynomial<Rational> >& output, GlobalVariables& theGlobalVariables)
   ;
  bool ApplyHomomorphism
  (MonomialUniversalEnveloping<Polynomial<Rational> >& input,
   ElementUniversalEnveloping<Polynomial<Rational> >& output, GlobalVariables& theGlobalVariables)
   ;
};

class SemisimpleLieAlgebraOrdered
{
public:
  SemisimpleLieAlgebra theOwner;
  //the format of the order is arbitrary except for the following requirements:
  //-All elements of the order must be either 1) nilpotent or 2) elements of the Cartan
  //-Let the number of positive Vectors<Rational> be N and the rank be K. Then the indices N,..., N+K-1 must
  //   correspond to the elements of the Cartan.
  List<ElementSemisimpleLieAlgebra> theOrder;
  //The order of chevalley generators is as follows. First come negative Vectors<Rational>, then elements of cartan, then positive Vectors<Rational>
  //The weights are in increasing order
  //The i^th column of the matrix gives the coordinates of the i^th Chevalley generator in the current coordinates
  Matrix<Rational>  ChevalleyGeneratorsInCurrentCoords;
  void AssignGeneratorCoeffOne(int theIndex, ElementSemisimpleLieAlgebra& output){output.operator=(this->theOrder.TheObjects[theIndex]);}
  int GetDisplayIndexFromGeneratorIndex(int GeneratorIndex);
  void GetLinearCombinationFrom
  (ElementSemisimpleLieAlgebra& input, Vector<Rational> & theCoeffs)
  ;
  void init
  (List<ElementSemisimpleLieAlgebra>& inputOrder, SemisimpleLieAlgebra& owner, GlobalVariables& theGlobalVariables)
;
 void initDefaultOrder
  (SemisimpleLieAlgebra& owner, GlobalVariables& theGlobalVariables)
;
};

template<class CoefficientType>
class ElementUniversalEnvelopingOrdered;

template<class CoefficientType>
class MonomialUniversalEnvelopingOrdered
{
  void SimplifyAccumulateInOutputNoOutputInit
(ElementUniversalEnvelopingOrdered<CoefficientType>& output, GlobalVariables* theContext,
 const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0
 )
  ;
  MonomialUniversalEnvelopingOrdered(const MonomialUniversalEnvelopingOrdered& other);
public:
  SemisimpleLieAlgebraOrdered* owner;
  std::string DebugString;
  std::string ElementToString
(bool useLatex, bool useCalculatorFormat, FormatExpressions* PolyFormatLocal, GlobalVariables& theGlobalVariables)const
;
  void ComputeDebugString()
  { GlobalVariables theGlobalVariables;
    FormatExpressions PolyFormatLocal;
    this->DebugString=this->ElementToString(false, true, &PolyFormatLocal, theGlobalVariables);
  }
  // SelectedIndices gives the non-zero powers of the generators participating in the monomial
  // Powers gives the powers of the generators in the order specified in the owner
  List<int> generatorsIndices;
  List<CoefficientType> Powers;
  CoefficientType Coefficient;
  static bool flagAnErrorHasOccurredTimeToPanic;
  void MultiplyBy
  (const MonomialUniversalEnveloping<CoefficientType>& other,
   ElementUniversalEnvelopingOrdered<CoefficientType>& output)
  ;
  void MultiplyByGeneratorPowerOnTheRight(int theGeneratorIndex, const CoefficientType& thePower);
  void MultiplyByGeneratorPowerOnTheRight(int theGeneratorIndex, int thePower);
  void MultiplyByNoSimplify(const MonomialUniversalEnvelopingOrdered& other);
  void MakeZero(int numVars, SemisimpleLieAlgebraOrdered& theOwner, GlobalVariables* theContext);
  void MakeZero(const CoefficientType& theRingZero, SemisimpleLieAlgebraOrdered& theOwner);
  bool ModOutFDRelationsExperimental
    (GlobalVariables* theContext, const Vector<Rational> & theHWsimpleCoords,
   const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
;
  void ModOutVermaRelations
  (GlobalVariables* theContext, const List<CoefficientType>* subHiGoesToIthElement=0,
   const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
     ;
  void SetNumVariables(int newNumVars);
  int HashFunction() const;
  static inline int HashFunction(const MonomialUniversalEnvelopingOrdered<CoefficientType>& input)
  { return input.HashFunction();
  }
  void GetDegree(Polynomial<Rational> & output)
  { output.MakeZero(this->Coefficient.NumVars);
    for (int i=0; i<this->generatorsIndices.size; i++)
      output+=(this->Powers.TheObjects[i]);
  }
  bool GetElementUniversalEnveloping
  (ElementUniversalEnveloping<CoefficientType>& output, SemisimpleLieAlgebra& owner)
  ;
  bool IsEqualToZero()const{return this->Coefficient.IsEqualToZero();}
  bool CommutingLeftIndexAroundRightIndexAllowed(CoefficientType& theLeftPower, int leftGeneratorIndex, CoefficientType& theRightPower, int rightGeneratorIndex);
  bool CommutingRightIndexAroundLeftIndexAllowed(CoefficientType& theLeftPower, int leftGeneratorIndex, CoefficientType& theRightPower, int rightGeneratorIndex);
  bool SwitchConsecutiveIndicesIfTheyCommute
(int theLeftIndex, MonomialUniversalEnvelopingOrdered<CoefficientType>& output, GlobalVariables* theContext,
 const CoefficientType& theRingZero=0)
   ;
  void MakeConst(const CoefficientType& theConst, SemisimpleLieAlgebraOrdered& theOwner){this->generatorsIndices.size=0; this->Powers.size=0; this->Coefficient=theConst; this->owner=&theOwner;}
  void Simplify
  (ElementUniversalEnvelopingOrdered<CoefficientType>& output, GlobalVariables* theContext,
   const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
  ;
  void CommuteConsecutiveIndicesLeftIndexAroundRight
  (int theIndeX, ElementUniversalEnvelopingOrdered<CoefficientType>& output, GlobalVariables* theContext,
    const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0
    )
  ;
  void CommuteConsecutiveIndicesRightIndexAroundLeft
(int theIndeX, ElementUniversalEnvelopingOrdered<CoefficientType>& output, GlobalVariables* theContext,
 const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0
 )
  ;
  MonomialUniversalEnvelopingOrdered(){this->owner=0;}
  void SubstitutionCoefficients
(PolynomialSubstitution<Rational>& theSub)
;
  bool operator==(const MonomialUniversalEnvelopingOrdered& other)const
  { assert(this->owner==other.owner);
    return this->Powers==other.Powers && this->generatorsIndices==other.generatorsIndices;
  }
  void operator*=(const MonomialUniversalEnvelopingOrdered& other);
  template<class OtherCoefficientType>
  void AssignChangeCoefficientType(const MonomialUniversalEnvelopingOrdered<OtherCoefficientType>& other)
  { this->Coefficient=other.Coefficient;
    this->generatorsIndices=other.generatorsIndices.size;
    this->Powers.SetSize(other.Powers.size);
    for(int i=0; i<this->Powers.size; i++)
      this->Powers[i]=other.Powers[i];
  }
  inline void operator=(const MonomialUniversalEnvelopingOrdered& other)
  { this->generatorsIndices.CopyFromBase(other.generatorsIndices);
    this->Powers.CopyFromBase(other.Powers);
    this->Coefficient=(other.Coefficient);
    this->owner=other.owner;
  }
};


template <class CoefficientType>
bool MonomialUniversalEnvelopingOrdered<CoefficientType>::flagAnErrorHasOccurredTimeToPanic=false;

template <class CoefficientType>
class ElementUniversalEnvelopingOrdered : public HashedList<MonomialUniversalEnvelopingOrdered<CoefficientType> >
{
private:
  void AddMonomialNoCleanUpZeroCoeff(const MonomialUniversalEnvelopingOrdered<CoefficientType>& input);
  void CleanUpZeroCoeff();
  friend class MonomialUniversalEnvelopingOrdered<CoefficientType>;
public:
  std::string DebugString;
  GlobalVariables* context;
  void ElementToString
(std::string& output, bool useLatex,
 bool useCalculatorFormat,
 FormatExpressions& PolyFormatLocal, GlobalVariables& theGlobalVariables)const
  ;
  std::string ElementToString
  (bool useLatex, bool useCalculatorFormat, FormatExpressions& PolyFormatLocal,
   GlobalVariables& theGlobalVariables)const
  { std::string tempS;
    this->ElementToString(tempS, useLatex, useCalculatorFormat, PolyFormatLocal, theGlobalVariables);
    return tempS;
  }
  std::string ElementToString
  (FormatExpressions& PolyFormatLocal, GlobalVariables& theGlobalVariables)const
  { std::string tempS;
    this->ElementToString(tempS, true, true, PolyFormatLocal, theGlobalVariables);
    return tempS;
  }
  std::string ElementToString(FormatExpressions& PolyFormatLocal)const
  { GlobalVariables theGlobalVariables;
    return this->ElementToString(PolyFormatLocal, theGlobalVariables);
  }
  bool NeedsBrackets()const{return this->size>1;}
  void ComputeDebugString()
  { FormatExpressions tempFormat;
    GlobalVariables theGlobalVariables;
    this->DebugString=this->ElementToString(tempFormat, theGlobalVariables);
  }
  SemisimpleLieAlgebraOrdered* owner;
  void AddMonomial(const MonomialUniversalEnvelopingOrdered<CoefficientType>& input);
  void AssignElementCartan(const Vector<Rational> & input, int numVars, SemisimpleLieAlgebraOrdered& theOwner);
  void AssignElementLieAlgebra
(const ElementSemisimpleLieAlgebra& input, const CoefficientType& theRingUnit,
 const CoefficientType& theRingZero, SemisimpleLieAlgebraOrdered& theOwner)
;
  void MakeOneGenerator
  (int theIndex, const CoefficientType& theCoeff, SemisimpleLieAlgebraOrdered& owner,
   GlobalVariables* theContext);
//  void MakeOneGeneratorCoeffOne(Vector<Rational> & rootSpace, int numVars, SemisimpleLieAlgebraOrdered& theOwner){this->MakeOneGeneratorCoeffOne(theOwner.RootToIndexInUE(rootSpace), numVars, theOwner);};
  void MakeZero(SemisimpleLieAlgebraOrdered& theOwner);
  bool AssignElementUniversalEnveloping
  (ElementUniversalEnveloping<CoefficientType>& input, SemisimpleLieAlgebraOrdered& owner,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero, GlobalVariables* theContext)
  ;
  bool AssignMonomialUniversalEnveloping
  (MonomialUniversalEnveloping<CoefficientType>& input, const CoefficientType& inputCoeff, SemisimpleLieAlgebraOrdered& owner,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero, GlobalVariables* theContext)
  ;
  bool ModOutFDRelationsExperimental
  (GlobalVariables* theContext, const Vector<Rational> & theHWsimpleCoords,
   const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
  ;
  bool IsEqualToZero()const {return this->size==0;}
  bool GetElementUniversalEnveloping
  (ElementUniversalEnveloping<CoefficientType>& output, SemisimpleLieAlgebra& owner);
  bool ConvertToLieAlgebraElementIfPossible(ElementSemisimpleLieAlgebra& output)const;
  void SubstitutionCoefficients
(PolynomialSubstitution<Rational>& theSub, GlobalVariables* theContext)
  ;
  void MakeConst(const CoefficientType& coeff, SemisimpleLieAlgebraOrdered& theOwner)
  { this->MakeZero(theOwner);
    MonomialUniversalEnvelopingOrdered<CoefficientType> tempMon;
    tempMon.MakeConst(coeff, theOwner);
    this->AddMonomial(tempMon);
  }
  void Simplify
  (GlobalVariables* theContext,
    const CoefficientType& theRingUnit=1,  const CoefficientType& theRingZero=0);
  int GetNumVars()const
  { if (this->size==0)
      return 0;
    else
      return this->TheObjects[0].Coefficient.NumVars;
  }
  inline void MultiplyBy(const ElementUniversalEnvelopingOrdered& other){this->operator*=(other);}
  void ModOutVermaRelations
  (GlobalVariables* theContext, const List<CoefficientType>* subHiGoesToIthElement=0,
   const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
  ;

  void ModOutVermaRelationSOld
  (bool SubHighestWeightWithZeroes, const PolynomialSubstitution<Rational>& highestWeightSub, GlobalVariables* theContext, const CoefficientType& theRingUnit)
  ;
  template<class CoefficientTypeQuotientField>
  static void GetBasisFromSpanOfElements
  (List<ElementUniversalEnvelopingOrdered<CoefficientType> >& theElements,
   Vectors<CoefficientTypeQuotientField>& outputCoords,
   List<ElementUniversalEnvelopingOrdered<CoefficientType> >& outputTheBasis,
   const CoefficientTypeQuotientField& theFieldUnit, const CoefficientTypeQuotientField& theFieldZero,
   GlobalVariables& theGlobalVariables)
   ;
  static void GetBasisFromSpanOfElements
  (List<ElementUniversalEnvelopingOrdered>& theElements, Vectors<Polynomial<CoefficientType> >& outputCoordinates,
   List<ElementUniversalEnvelopingOrdered>& outputTheBasis, GlobalVariables& theGlobalVariables)
;
  bool GetCoordsInBasis
  (List<ElementUniversalEnvelopingOrdered<CoefficientType> >& theBasis, Vector<CoefficientType>& output,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero,
   GlobalVariables& theGlobalVariables)const
  ;
  static void GetCoordinateFormOfSpanOfElements
  (int numVars, List<ElementUniversalEnvelopingOrdered>& theElements,
   Vectors<Polynomial<CoefficientType> >& outputCoordinates,
   ElementUniversalEnvelopingOrdered& outputCorrespondingMonomials, GlobalVariables& theGlobalVariables)
;
//  static void GetCoordinateFormOfSpanOfElements
//  (List<ElementUniversalEnvelopingOrdered>& theElements, Vectors<Rational>& outputCoordinates, ElementUniversalEnvelopingOrdered& outputCorrespondingMonomials, GlobalVariables& theGlobalVariables)
//;
  void AssignFromCoordinateFormWRTBasis
  (List<ElementUniversalEnveloping<CoefficientType> >& theBasis, Vector<Polynomial<CoefficientType> >& input,
   SemisimpleLieAlgebraOrdered& owner)
  ;
  void SetNumVariables(int newNumVars);
  void RaiseToPower(int thePower, const CoefficientType& theRingUnit);
  bool IsAPowerOfASingleGenerator()
  { if (this->size!=1)
      return false;
    MonomialUniversalEnvelopingOrdered<CoefficientType>& tempMon=this->TheObjects[0];
    if (!tempMon.Coefficient.IsEqualToOne())
      return false;
    if (tempMon.generatorsIndices.size!=1)
      return false;
    return true;
  }
  void MakeCasimir(SemisimpleLieAlgebraOrdered& theOwner, int numVars, GlobalVariables& theGlobalVariables);
  void ActOnMe(const ElementSemisimpleLieAlgebra& theElt, ElementUniversalEnvelopingOrdered& output);
  void LieBracketOnTheRight(const ElementUniversalEnvelopingOrdered& right, ElementUniversalEnvelopingOrdered& output);
  void LieBracketOnTheRight
  (const ElementSemisimpleLieAlgebra& right, const CoefficientType& ringUnit, const CoefficientType& ringZero)
  ;
  void operator=(const ElementUniversalEnvelopingOrdered& other)
  { this->::HashedList<MonomialUniversalEnvelopingOrdered<CoefficientType> >::operator=(other);
    this->owner=other.owner;
  }
  template<class OtherCoefficientType>
  void AssignChangeCoefficientType (const ElementUniversalEnvelopingOrdered<OtherCoefficientType>& other)
  { this->MakeZero(*other.owner);
    MonomialUniversalEnvelopingOrdered<CoefficientType> tempMon;
    this->Reserve(other.size);
    for (int i=0; i<other.size; i++)
    { tempMon.AssignChangeCoefficientType<OtherCoefficientType>(other[i]);
      this->AddMonomial(tempMon);
    }
  }
  void operator+=(const ElementUniversalEnvelopingOrdered& other);
  void operator+=(int other);
  void operator+=(const Rational& other);
  void operator-=(const ElementUniversalEnvelopingOrdered& other);
  void operator*=(const ElementUniversalEnvelopingOrdered& other);
  template <class SecondType>
  void operator/=(const SecondType& other);
  template<class otherType>
  void operator*=(const otherType& other);
  ElementUniversalEnvelopingOrdered(){this->owner=0;}
  bool IsProportionalTo
 (const ElementUniversalEnvelopingOrdered<CoefficientType>& other,
  CoefficientType& outputTimesMeEqualsOther,
  const CoefficientType& theRingZero)const
  ;
  ElementUniversalEnvelopingOrdered(const ElementUniversalEnvelopingOrdered& other)
  { this->operator=(other);
  }
  void ClearDenominators(CoefficientType& outputWasMultipliedBy, const CoefficientType& theRingUnit)
  { outputWasMultipliedBy=theRingUnit;
    CoefficientType currentCoeff;
    for (int i=0; i<this->size; i++)
    { MonomialUniversalEnvelopingOrdered<CoefficientType>& currentMon=this->TheObjects[i];
      currentMon.Coefficient.ClearDenominators(currentCoeff);
      for (int j=0; j<this->size; j++)
        if (j!=i)
          this->TheObjects[j].Coefficient*=currentCoeff;
      outputWasMultipliedBy*=currentCoeff;
    }
  }
};

template <class CoefficientType>
class MonomialUniversalEnveloping
{
private:
public:
  std::string ElementToString
  (FormatExpressions* theFormat=0)const
  ;
  List<SemisimpleLieAlgebra>* owners;
  int indexInOwners;
  // SelectedIndices gives the non-zero powers of the chevalley generators participating in the monomial
  // Powers gives the powers of the Chevalley generators in the order they appear in generatorsIndices
  List<int> generatorsIndices;
  List<CoefficientType> Powers;
  bool AdjointRepresentationAction
  (const ElementUniversalEnveloping<CoefficientType>& input, ElementUniversalEnveloping<CoefficientType>& output, GlobalVariables& theGlobalVariables)
  ;
  bool IsEqualToZero()const
  { return this->Coefficient.IsEqualToZero();
  }
  template<class otherType>
  void Assign(const MonomialUniversalEnveloping<otherType>& other)
  { this->generatorsIndices=(other.generatorsIndices);
    this->Powers.SetSize(other.Powers.size);
    for (int i=0; i<other.Powers.size; i++)
      this->Powers[i]=other.Powers[i];
    this->owners=other.owners;
    this->indexInOwners=other.indexInOwners;
  }
  SemisimpleLieAlgebra& GetOwner()const{return this->owners->TheObjects[this->indexInOwners];}
//  void MultiplyBy(const MonomialUniversalEnveloping& other, ElementUniversalEnveloping<CoefficientType>& output);
  void MultiplyBy(const MonomialUniversalEnveloping<CoefficientType>& other)
  { if (this==&other)
    { MonomialUniversalEnveloping<CoefficientType> tempMon;
      tempMon=other;
      this->MultiplyBy(tempMon);
      return;
    }
    this->generatorsIndices.AddListOnTop(other.generatorsIndices);
    this->Powers.AddListOnTop(other.Powers);
    this->SimplifyEqualConsecutiveGenerators(0);
  }
  void MultiplyByGeneratorPowerOnTheRight(int theGeneratorIndex, const CoefficientType& thePower);
  void MultiplyByNoSimplify(const MonomialUniversalEnveloping& standsOnTheRight);
  void MakeZero
(int numVars, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners)
  ;
  void MakeZero(const CoefficientType& theRingZero, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners)
  { this->init(inputOwners, inputIndexInOwners);
  }
  bool HWTAAbilinearForm
  (const MonomialUniversalEnveloping<CoefficientType>&right, CoefficientType& output,
   const Vector<CoefficientType>* subHiGoesToIthElement, GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero, std::stringstream* logStream=0)
  ;
  void ModOutVermaRelations
  (CoefficientType& outputCoeff, GlobalVariables* theContext,
   const Vector<CoefficientType>* subHiGoesToIthElement=0, const CoefficientType& theRingUnit=1,
   const CoefficientType& theRingZero=0)
;
  bool SimplifyEqualConsecutiveGenerators(int lowestNonReducedIndex=0);
  void SetNumVariables(int newNumVars);
  void Substitution(const PolynomialSubstitution<Rational>& theSub);
  int HashFunction() const;
  static inline int HashFunction(const MonomialUniversalEnveloping<CoefficientType>& input)
  { return input.HashFunction();
  }
  void GetDegree(Polynomial<Rational> & output)
  { if (this->Powers.size==0)
    { output.MakeZero(0);
      return;
    }
    output=this->Powers[0];
    for (int i=1; i<this->generatorsIndices.size; i++)
      output+=(this->Powers[i]);
  }
  void init(List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners)
  { this->owners=&inputOwners;
    this->indexInOwners=inputIndexInOwners;
    this->Powers.size=0;
    this->generatorsIndices.size=0;
  }
  bool CommutingABntoBnAPlusLowerOrderAllowed
(CoefficientType& theLeftPower, int leftGeneratorIndex, CoefficientType& theRightPower, int rightGeneratorIndex)
   ;
  bool CommutingAnBtoBAnPlusLowerOrderAllowed
(CoefficientType& theLeftPower, int leftGeneratorIndex, CoefficientType& theRightPower, int rightGeneratorIndex)
   ;
  bool SwitchConsecutiveIndicesIfTheyCommute
(int theLeftIndex)
  ;
  void MakeConst(List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwner)
  { this->generatorsIndices.size=0;
    this->Powers.size=0;
    this->owners=&inputOwners;
    this->indexInOwners=inputIndexInOwner;
  }
  //we assume the standard order for being simplified to be Ascending.
  //this way the positive Vectors<Rational> will end up being in the end, which is very convenient for computing with Verma modules
  //format of the order of the generators:
  // first come the negative Vectors<Rational>, in increasing height, then the elements of the Cartan subalgebra, then the positive Vectors<Rational>, in increasing height
  //The order of the positive Vectors<Rational> is the same as the order in which Vectors<Rational> are kept in WeylGroup::RootSystem
  // The order of the negative Vectors<Rational> is reverse to the order in which they are kept in WeylGroup::RootSystem
  // with the "zero level Vectors<Rational>" - i.e. the elements of the Cartan subalgebra - put in between.
  void Simplify
  (ElementUniversalEnveloping<CoefficientType>& output, GlobalVariables& theGlobalVariables,
 const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0 )
  ;
  void CommuteABntoBnAPlusLowerOrder
(int theIndeX, ElementUniversalEnveloping<CoefficientType>& output,
 const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
  ;
  void CommuteAnBtoBAnPlusLowerOrder
(int indexA, ElementUniversalEnveloping<CoefficientType>& output,
 const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
  ;
  MonomialUniversalEnveloping():owners(0), indexInOwners(-1){}
  bool operator>(const MonomialUniversalEnveloping& other)
  { if (other.generatorsIndices.size>this->generatorsIndices.size)
      return false;
    if (other.generatorsIndices.size< this->generatorsIndices.size)
      return true;
    for (int i=0; i<this->generatorsIndices.size; i++)
    { if (other.generatorsIndices[i]>this->generatorsIndices[i])
        return false;
      if (other.generatorsIndices[i]<this->generatorsIndices[i])
        return true;
      if (other.Powers[i]>this->Powers[i])
        return false;
      if (this->Powers[i]>other.Powers[i])
        return true;
    }
    return false;
  }
  bool operator==(const MonomialUniversalEnveloping& other)const
  { return this->owners==other.owners && this->indexInOwners==other.indexInOwners
    && this->Powers==other.Powers && this->generatorsIndices==other.generatorsIndices;
  }
  inline void operator=(const MonomialUniversalEnveloping& other)
  { this->Assign(other);
  }
  inline void operator*=(const MonomialUniversalEnveloping& other)
  { this->MultiplyBy(other);
  }
};

template <class CoefficientType>
class ElementUniversalEnveloping: public MonomialCollection<MonomialUniversalEnveloping<CoefficientType>, CoefficientType>
{
private:
  void CleanUpZeroCoeff();
  friend class MonomialUniversalEnveloping<CoefficientType>;
public:
  List<SemisimpleLieAlgebra>* owners;
  int indexInOwners;
  bool AdjointRepresentationAction
  (const ElementUniversalEnveloping<CoefficientType>& input, ElementUniversalEnveloping<CoefficientType>& output, GlobalVariables& theGlobalVariables)
  ;
  bool ConvertToRationalCoeff(ElementUniversalEnveloping<Rational>& output);
  bool IsEqualToZero()const
  { return this->size==0;
  }
  bool HWMTAbilinearForm
  (const ElementUniversalEnveloping<CoefficientType>&right, CoefficientType& output,
   const Vector<CoefficientType>* subHiGoesToIthElement, GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero, std::stringstream* logStream=0)
  ;
  std::string IsInProperSubmodule
  (const Vector<CoefficientType>* subHiGoesToIthElement, GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
   ;
  bool HWTAAbilinearForm
  (const ElementUniversalEnveloping<CoefficientType>&right, CoefficientType& output,
   const Vector<CoefficientType>* subHiGoesToIthElement, GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero, std::stringstream* logStream=0)const
  ;
  bool HWTAAbilinearForm
  (const MonomialUniversalEnveloping<CoefficientType>&right, CoefficientType& output,
   const Vector<CoefficientType>* subHiGoesToIthElement, GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero, std::stringstream* logStream=0)const
  { ElementUniversalEnveloping<CoefficientType> tempElt;
    tempElt.MakeZero(*this->owners, this->indexInOwners);
    tempElt.AddMonomial(right, theRingUnit);
    return this->HWTAAbilinearForm
    (tempElt, output, subHiGoesToIthElement, theGlobalVariables, theRingUnit, theRingZero, logStream);
  }
  bool NeedsBracketForMultiplication()
  { return this->size>1;
  }
  bool ApplyMinusTransposeAutoOnMe();
  bool ApplyTransposeAntiAutoOnMe();
  void AssignElementCartan
  (const Vector<Rational> & input, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
   ;
  void AssignElementLieAlgebra
(const ElementSemisimpleLieAlgebra& input, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
  ;
  void MakeOneGenerator
(int theIndex, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners, const CoefficientType& theRingUnit)
  ;
  void MakeOneGeneratorCoeffOne
(int theIndex, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
 ;
  void MakeOneGeneratorCoeffOne
  (int theIndex, int numVars, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners)
;
  void MakeOneGeneratorCoeffOne
  (const Vector<Rational> & rootSpace, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners,
  const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
  { this->MakeOneGeneratorCoeffOne
    (inputOwners[inputIndexInOwners].RootToIndexInUE(rootSpace), inputOwners, inputIndexInOwners,
     theRingUnit, theRingZero);
  }
  void MakeZero(List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners);
  bool ConvertToLieAlgebraElementIfPossible(ElementSemisimpleLieAlgebra& output)const;
  void MakeConst(const Rational& coeff, int numVars, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners);
  void MakeConst(const CoefficientType& coeff, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners)
  { this->MakeZero(inputOwners, inputIndexInOwners);
    MonomialUniversalEnveloping<CoefficientType> tempMon;
    tempMon.MakeConst(inputOwners, inputIndexInOwners);
    this->AddMonomial(tempMon, coeff);
  }
  void Simplify
  (GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
   ;
  int GetNumVars()const
  { if (this->size<1)
      return 0;
    return this->theCoeffs[0].GetNumVars();
  }
  inline void MultiplyBy(const ElementUniversalEnveloping<CoefficientType>& standsOnTheRight)
  { this->operator*=(standsOnTheRight);
  }
  inline void MultiplyBy
  (const MonomialUniversalEnveloping<CoefficientType>& standsOnTheRight, const CoefficientType& theCoeff)
  ;
  void ModToMinDegreeFormFDRels
  (const Vector<Rational> & theHWinSimpleCoords,
   GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero);
  void ModOutVermaRelations
  (GlobalVariables* theContext, const Vector<CoefficientType>* subHiGoesToIthElement,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
   ;
  static void GetCoordinateFormOfSpanOfElements
  (int numVars, List<ElementUniversalEnveloping<CoefficientType> >& theElements,
   Vectors<CoefficientType>& outputCoordinates, ElementUniversalEnveloping<CoefficientType>& outputCorrespondingMonomials,
   GlobalVariables& theGlobalVariables)

;
  bool GetCoordsInBasis
  (List<ElementUniversalEnveloping<CoefficientType> >& theBasis, Vector<CoefficientType>& output,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero,
   GlobalVariables& theGlobalVariables)const
;
  static inline int HashFunction (const ElementUniversalEnveloping<CoefficientType>& input)
  { return input.HashFunction();
  }
  int HashFunction()const
  { return this->::MonomialCollection<MonomialUniversalEnveloping<CoefficientType>, CoefficientType>::HashFunction();
  }
template<class CoefficientTypeQuotientField>
static bool GetBasisFromSpanOfElements
  (List<ElementUniversalEnveloping<CoefficientType> >& theElements,
   Vectors<CoefficientTypeQuotientField>& outputCoords,
   List<ElementUniversalEnveloping<CoefficientType> >& outputTheBasis,
   const CoefficientTypeQuotientField& theFieldUnit, const CoefficientTypeQuotientField& theFieldZero,
   GlobalVariables& theGlobalVariables)
   ;
  static void GetCoordinateFormOfSpanOfElements
  (List<ElementUniversalEnveloping<CoefficientType> >& theElements, Vectors<Rational>& outputCoordinates,
   ElementUniversalEnveloping<CoefficientType>& outputCorrespondingMonomials,
   GlobalVariables& theGlobalVariables)
   ;
  void AssignFromCoordinateFormWRTBasis
  (List<ElementUniversalEnveloping<CoefficientType> >& theBasis,
   Vector<CoefficientType>& input, SemisimpleLieAlgebra& owner)
  ;
  void SetNumVariables(int newNumVars);
  void RaiseToPower(int thePower);
  bool IsAPowerOfASingleGenerator()const
  { if (this->size!=1)
      return false;
    if (!this->theCoeffs[0].IsEqualToOne())
      return false;
    MonomialUniversalEnveloping<CoefficientType>& tempMon=this->TheObjects[0];
    return tempMon.generatorsIndices.size==1;
  }
  void SubstitutionCoefficients
(PolynomialSubstitution<Rational>& theSub, GlobalVariables* theContext,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
  ;
  void Substitution
(const PolynomialSubstitution<Rational>& theSub)
  ;
  void MakeCasimir
(SemisimpleLieAlgebra& theOwner, GlobalVariables& theGlobalVariables,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
   ;
  void LieBracketOnTheRight(const ElementUniversalEnveloping<CoefficientType>& right, ElementUniversalEnveloping<CoefficientType>& output)const;
  void LieBracketOnTheLeft(const ElementSemisimpleLieAlgebra& left);
  void AssignInt(int coeff, int numVars, SemisimpleLieAlgebra& theOwner)
  { Rational tempRat=coeff;
    this->MakeConst(tempRat, numVars, *theOwner.owner, theOwner.indexInOwner);
  }
  SemisimpleLieAlgebra& GetOwner()const{return this->owners->TheObjects[this->indexInOwners];}
  template <class otherType>
  void Assign(const ElementUniversalEnveloping<otherType>& other)
  { this->owners=other.owners;
    this->indexInOwners=other.indexInOwners;
    MonomialUniversalEnveloping<CoefficientType> tempMon;
    CoefficientType theCoeff;
    for (int i=0; i<other.size; i++)
    { tempMon.Assign(other[i]);
      theCoeff=other.theCoeffs[i];
      this->AddMonomial(tempMon, theCoeff);
    }
  }
  void operator=(const ElementUniversalEnveloping<CoefficientType>& other)
  { this->::MonomialCollection<MonomialUniversalEnveloping<CoefficientType>, CoefficientType>::operator=(other);
    this->owners=other.owners;
    this->indexInOwners=other.indexInOwners;
  }
  void operator*=(const ElementUniversalEnveloping<CoefficientType>& standsOnTheRight);
  template<class otherType>
  void operator/=(const otherType& other)
  { this->::MonomialCollection<MonomialUniversalEnveloping<CoefficientType>, CoefficientType>
    ::operator/=(other);
  }
  template<class otherType>
  void operator*=(const otherType& other)
  { this->::MonomialCollection<MonomialUniversalEnveloping<CoefficientType>, CoefficientType>
    ::operator*=(other);
  }

  ElementUniversalEnveloping<CoefficientType>():owners(0), indexInOwners(-1){}
  ElementUniversalEnveloping<CoefficientType>(const ElementUniversalEnveloping<CoefficientType>& other){this->operator=(other);}
};

class SltwoSubalgebras : public HashedList<slTwo>
{
public:
  std::string DebugString;
//  List< int > hSingularWeights;
  List<int> MultiplicitiesFixedHweight;
  List<List<int> > IndicesSl2sContainedInRootSA;
  List<int> IndicesSl2decompositionFlas;
  Vectors<Rational> BadHCharacteristics;
  int IndexZeroWeight;
  List<SemisimpleLieAlgebra> owner;
  rootSubalgebras theRootSAs;
  void ComputeDebugString(GlobalVariables& theGlobalVariables, WeylGroup& theWeyl, bool useLatex, bool useHtml){ this->ElementToString(this->DebugString, theGlobalVariables, theWeyl, useLatex, useHtml, false, 0, 0);};
  List<std::string> texFileNamesForPNG;
  List<std::string> texStringsEachFile;
  List<std::string> listSystemCommandsLatex;
  List<std::string> listSystemCommandsDVIPNG;
  void ComputeModuleDecompositionsOfAmbientLieAlgebra(GlobalVariables& theGlobalVariables)
  { for(int i=0; i<this->size; i++)
      this->TheObjects[i].ComputeModuleDecompositionAmbientLieAlgebra(theGlobalVariables);
  }
  SltwoSubalgebras()
  { this->owner.SetSize(1);
    this->owner[0].indexInOwner=0;
    this->owner[0].owner=&this->owner;
  }
  void ComputeModuleDecompositionsOfMinimalContainingRegularSAs(GlobalVariables& theGlobalVariables)
  { for(int i=0; i<this->size; i++)
      this->TheObjects[i].ComputeModuleDecompositionOfMinimalContainingRegularSAs(*this, i, theGlobalVariables);
  }
  void getZuckermansArrayE8(Vectors<Rational>& output);
  void MakeProgressReport(int index, int outOf, GlobalVariables& theGlobalVariables);
  void ComputeDebugStringCurrent();
  bool ContainsSl2WithGivenH(Vector<Rational> & theH, int* outputIndex);
  bool ContainsSl2WithGivenHCharacteristic(Vector<Rational> & theHCharacteristic, int* outputIndex);
  void ElementToHtml
(GlobalVariables& theGlobalVariables, WeylGroup& theWeyl, bool usePNG, std::string& physicalPath, std::string& htmlPathServer,
 std::string& DisplayNameCalculator)
   ;
  void ElementToStringModuleDecompositionMinimalContainingRegularSAs(std::string& output, bool useLatex, bool useHtml);
  void ElementToString(std::string& output, GlobalVariables& theGlobalVariables, WeylGroup& theWeyl, bool useLatex, bool useHtml, bool usePNG, std::string* physicalPath, std::string* htmlPathServer);
  void ElementToStringNoGenerators(std::string& output, GlobalVariables& theGlobalVariables, WeylGroup& theWeyl, bool useLatex, bool useHtml, bool usePNG, std::string* physicalPath, std::string* htmlPathServer);
};

class reductiveSubalgebra
{
  public:
  int indexInOwner;
};

class reductiveSubalgebras
{
public:
  SltwoSubalgebras theSl2s;
  SltwoSubalgebras CandidatesPrincipalSl2ofSubalgebra;
  Selection RemainingCandidates;
//  List<DynkinDiagramRootSubalgebra> thePossibleDynkinDiagrams;
  List<List<int> > theRanks;
  List<List<int> > theMultiplicities;
  List<List<char> > theLetters;
  List<List<int> > thePartitionValues;
  List<List<int> > thePartitionMultiplicities;
  List<SltwoSubalgebras> theCandidateSubAlgebras;
  List<List<int> > IndicesMatchingSl2s;
  List<List<int> > IndicesMatchingPartitionSl2s;
  List<List<int> > IndicesMatchingActualSl2s;
  void MatchRealSl2sToPartitionSl2s();
  void MakeSelectionBasedOnPrincipalSl2s(GlobalVariables& theGlobalVariables);
  void MatchActualSl2sFixedRootSAToPartitionSl2s(GlobalVariables& theGlobalVariables);
  void GenerateModuleDecompositionsPrincipalSl2s(int theRank, GlobalVariables& theGlobalVariables);
  void ElementToStringCandidatePrincipalSl2s(bool useLatex, bool useHtml, std::string& output, GlobalVariables& theGlobalVariables);
  void ElementToStringDynkinType(int theIndex, bool useLatex, bool useHtml, std::string& output);
  void FindTheReductiveSubalgebras(char WeylLetter, int WeylIndex, GlobalVariables& theGlobalVariables);
  void EnumerateAllPossibleDynkinDiagramsOfRankUpTo(int theRank);
  void GenerateAllPartitionsUpTo(int theRank);
  void GenerateAllPartitionsDontInit(int theRank);
  void GenerateAllDiagramsForPartitionRecursive(int indexPartition, int indexInPartition, List<int>& ranksBuffer, List<int>& multiplicitiesBuffer, List<char>& lettersBuffer);
  void GenerateAllPartitionsRecursive(int remainingToBePartitioned, int CurrentValue, List<int>& Multiplicities, List<int>& Values);
};

class ElementWeylAlgebra
{
private:
  //the standard order is as follows. First come the variables, then the differential operators, i.e. for 2 variables the order is x_1 x_2 \partial_{x_1}\partial_{x_2}
  Polynomial<Rational> StandardOrder;
public:
  std::string DebugString;
  void ComputeDebugString(bool useBeginEqnArray, bool useXYs){this->ElementToString(this->DebugString, useXYs, true, useBeginEqnArray); }
  void ElementToString(std::string& output, List<std::string>& alphabet, bool useLatex, bool useBeginEqnArray);
  void ElementToString(std::string& output, bool useXYs, bool useLatex, bool useBeginEqnArray);
  std::string ElementToString(bool useLatex){std::string tempS; this->ElementToString(tempS, false, useLatex, false); return tempS;}
  //NumVariables must equal 2*StandardOrder.NumVars
  int NumVariables;
  void MakeGEpsPlusEpsInTypeD(int i, int j, int NumVars);
  void MakeGEpsMinusEpsInTypeD(int i, int j, int NumVars);
  void MakeGMinusEpsMinusEpsInTypeD(int i, int j, int NumVars);
  void Makexixj(int i, int j, int NumVars);
  void Makexi(int i, int NumVars);
  void Makedi(int i, int NumVars);
  void Makedidj(int i, int j, int NumVars);
  void Makexidj(int i, int j, int NumVars);
  void MakeZero(int NumVars);
  bool ActOnPolynomial(Polynomial<Rational> & thePoly);
  void GetStandardOrder(Polynomial<Rational> & output){output=this->StandardOrder;}
  void SetNumVariables(int newNumVars);
  void TimesConstant(Rational& theConstant){ this->StandardOrder*=(theConstant);}
  void MultiplyOnTheLeft(ElementWeylAlgebra& standsOnTheLeft, GlobalVariables& theGlobalVariables);
  void MultiplyOnTheRight(const ElementWeylAlgebra& standsOnTheRight);
  void LieBracketOnTheLeft(ElementWeylAlgebra& standsOnTheLeft, GlobalVariables& theGlobalVariables);
  void LieBracketOnTheLeftMakeReport(ElementWeylAlgebra& standsOnTheLeft, GlobalVariables& theGlobalVariables, std::string& report);
  void LieBracketOnTheRightMakeReport(ElementWeylAlgebra& standsOnTheRight, GlobalVariables& theGlobalVariables, std::string& report);
  void LieBracketOnTheRight(ElementWeylAlgebra& standsOnTheRight, GlobalVariables& theGlobalVariables);
  void Assign(const ElementWeylAlgebra& other)
  { this->StandardOrder=(other.StandardOrder);
    this->NumVariables=other.NumVariables;
  }
  void AssignPolynomial(const Polynomial<Rational>& thePoly)
  { this->StandardOrder=thePoly;
    this->StandardOrder.IncreaseNumVariables(thePoly.NumVars);
    this->NumVariables=thePoly.NumVars;
  }
  void Subtract(const ElementWeylAlgebra& other)
  { this->StandardOrder-=(other.StandardOrder);
  }
  void MakeConst(int NumVars, const Rational& theConst);
  void SubstitutionTreatPartialsAndVarsAsIndependent
  (PolynomialSubstitution<Rational>& theSub)
  ;
  void RaiseToPower(int thePower);
  void Add(const ElementWeylAlgebra& other){ this->StandardOrder+=other.StandardOrder;}
  void MultiplyTwoMonomials(MonomialP& left, MonomialP& right, Polynomial<Rational> & OrderedOutput);
  ElementWeylAlgebra(){ this->NumVariables=0; }
  void operator=(const std::string& input);
  void operator+=(const ElementWeylAlgebra& other){this->Add(other);}
  inline void operator*=(const ElementWeylAlgebra& other){ this->MultiplyOnTheRight(other);}
  void operator-=(const ElementWeylAlgebra& other){this->Subtract(other);}
  bool IsLetter(char theLetter);
  bool IsIndex(char theIndex);
  bool IsNumber(char theNumber);
};

class LittelmannPath
{
public:
  WeylGroup* owner;
  Vectors<Rational> Waypoints;
  void MakeFromWeightInSimpleCoords
  (Vector<Rational> & weightInSimpleCoords, WeylGroup& theOwner)
  ;
  void MakeFromWaypoints
  (Vectors<Rational>& weightsInSimpleCoords, WeylGroup& theOwner)
  { this->owner=& theOwner;
    this->Waypoints=weightsInSimpleCoords;
    this->Simplify();
  }
  void ActByFalpha(int indexAlpha);
  void ActByEalpha(int indexAlpha);
//   List<Rational> Speeds;
  void operator+=(const LittelmannPath& other)
  { this->Waypoints.Reserve(this->Waypoints.size+other.Waypoints.size);
    Vector<Rational>  endPoint=*this->Waypoints.LastObject();
    for (int i=0; i<other.Waypoints.size; i++)
      this->Waypoints.AddOnTop(other.Waypoints[i]+endPoint);
  }
  std::string ElementToStringIndicesToCalculatorOutput
(LittelmannPath& inputStartingPath, List<int> & input)
  ;
  std::string ElementToStringOperatorSequenceStartingOnMe
(List<int> & input)
  ;
  bool GenerateOrbit
(List<LittelmannPath>& output, List<List<int> >& outputOperators, GlobalVariables& theGlobalVariables, int UpperBoundNumElts,
 Selection* parabolicNonSelectedAreInLeviPart=0)
;
  bool MinimaAreIntegral();
  std::string ElementToString(bool useSimpleCoords=true)
  { if (this->Waypoints.size==0)
      return "0";
    std::stringstream out;
    for (int i=0; i<this->Waypoints.size; i++)
    { if (useSimpleCoords)
        out << this->Waypoints[i].ElementToString();
      else
        out << this->owner->GetFundamentalCoordinatesFromSimple(this->Waypoints[i]).ElementToString();
      if (i!=this->Waypoints.size-1)
        out << "->";
    }
    return out.str();
  }
  void Simplify();
  int HashFunction()const
  { return this->Waypoints.HashFunction();
  }
  static inline int HashFunction(const LittelmannPath& input)
  { return input.HashFunction();
  }
  bool IsEqualToZero()const
  { return this->Waypoints.size==0;
  }
  void operator=(const LittelmannPath& other)
  { this->Waypoints=other.Waypoints;
    this->owner=other.owner;
  }
  bool operator==(const LittelmannPath& other)const
  { return this->Waypoints==other.Waypoints;
  }
};

class ConeLatticeAndShiftMaxComputation
{ public:
  inline static const std::string GetXMLClassName(){ return "ConeLatticeAndShiftMaxComputation";}
  int numNonParaM;
  int numProcessedNonParam;
  List<ConeComplex> complexStartingPerRepresentative;
  List<ConeComplex> complexRefinedPerRepresentative;
  List<List<Vectors<Rational> > > theMaximaCandidates;
  List<Vectors<Rational> > startingLPtoMaximize;
  List<Vectors<Rational> > finalMaxima;
  Lattice theStartingLattice;
  Lattice theFinalRougherLattice;
  Vector<Rational>  theStartingRepresentative;
  Vectors<Rational> theFinalRepresentatives;
  List<ConeLatticeAndShift> theConesLargerDim;
  List<ConeLatticeAndShift> theConesSmallerDim;
  List<List<ConeComplex> > finalMaximaChambers;
  List<List<List<int> > > finalMaximaChambersIndicesMaxFunctions;
  List<bool> IsInfinity;
  Vectors<Rational> LPtoMaximizeLargerDim;
  Vectors<Rational> LPtoMaximizeSmallerDim;

  std::string ElementToString(FormatExpressions* theFormat=0);
  void init
  (Vector<Rational> & theNEq, Cone& startingCone, Lattice& startingLattice, Vector<Rational> & startingShift)
  ;
  void FindExtremaParametricStep1
    (Controller& thePauseController, bool assumeNewConesHaveSufficientlyManyProjectiveVertices, GlobalVariables& theGlobalVariables)
       ;
  void FindExtremaParametricStep2TrimChamberForMultOne
    (Controller& thePauseController, GlobalVariables& theGlobalVariables)
;
  void FindExtremaParametricStep3
    (Controller& thePauseController, GlobalVariables& theGlobalVariables)
;
  void FindExtremaParametricStep4
    (Controller& thePauseController, GlobalVariables& theGlobalVariables)
;
  void FindExtremaParametricStep5
    (Controller& thePauseController, GlobalVariables& theGlobalVariables)
;

  void WriteToFile
  (std::fstream& output, GlobalVariables* theGlobalVariables)
  ;
  bool ReadFromFile
(std::fstream& input, GlobalVariables* theGlobalVariables, int UpperLimitDebugPurposes)
  ;
};

class PiecewiseQuasipolynomial
{
  public:
  //Note: PiecewiseQuasipolynomial assumes that its variable GlobalVariables* theBuffers
  // is always initialized with a non-zero object.
  //This is an engineering decision that yet has to be proven good: please follow the assumption,
  //but keep in mind that a better solution might exist, and be ready to switch.
  //Let the piecewise quasipolynomial be in n variables.
  //Then the theProjectivizedComplex is an n+1-dimensional complex,
  //which is the projectivization of the n-dim affine complex representing the
  //domain of the piecewise quasipoly.
  ConeComplex theProjectivizedComplex;
  List<QuasiPolynomial> theQPs;
  GlobalVariables* theBuffers;
  int NumVariables;
  void operator=(PiecewiseQuasipolynomial& other)
  { this->theBuffers=other.theBuffers;
    this->NumVariables=other.NumVariables;
    this->theQPs=other.theQPs;
    this->theProjectivizedComplex=other.theProjectivizedComplex;
  }
  std::string ElementToString(bool useLatex, bool useHtml);
  PiecewiseQuasipolynomial(){this->theBuffers=0;}
  PiecewiseQuasipolynomial(GlobalVariables& PermanentGlobalVariables){this->theBuffers=& PermanentGlobalVariables;}
  void DrawMe
 (DrawingVariables& theDrawingVars, int numLatticePointsPerDim, Cone* RestrictingChamber=0,
 Vector<Rational> * distinguishedPoint=0)
  ;
  int GetNumVars(){return this->NumVariables;}
  inline void MakeCommonRefinement(const PiecewiseQuasipolynomial& other){ this->MakeCommonRefinement(other.theProjectivizedComplex);  }
  void MakeCommonRefinement(const ConeComplex& other);
  void TranslateArgument(Vector<Rational> & translateToBeAddedToArgument, GlobalVariables& theGlobalVariables);
  bool MakeVPF
  (Vectors<Rational>& theRoots, std::string& outputstring, GlobalVariables& theGlobalVariables)
  ;
  Rational Evaluate(const Vector<Rational> & thePoint);
  Rational EvaluateInputProjectivized(const Vector<Rational> & thePoint);
  void MakeZero(int numVars, GlobalVariables& theGlobalVariables)
  { this->NumVariables=numVars;
    this->theProjectivizedComplex.init();
    this->theBuffers=& theGlobalVariables;
    this->theQPs.size=0;
  }
  void operator+=(const PiecewiseQuasipolynomial& other);
  void operator*=(const Rational& other);
  void operator=(const PiecewiseQuasipolynomial& other);
};

template <class CoefficientType>
class MonomialChar
{
public:
  Vector<CoefficientType> weightFundamentalCoords;
  void AccountSingleWeight
(Vector<Rational>& currentWeightSimpleCoords, Vector<Rational>& otherHighestWeightSimpleCoords, WeylGroup& theWeyl,
 Rational& theMult, CoefficientType& theCoeff, charSSAlgMod<CoefficientType>& outputAccum, Rational& finalCoeff
 )
  ;
  std::string TensorAndDecompose
(MonomialChar<CoefficientType>& other, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners, charSSAlgMod<CoefficientType>& output,
 GlobalVariables& theGlobalVariables)
   ;
  std::string ElementToString
  (FormatExpressions* theFormat=0)
  ;
  inline int HashFunction()const
  { return weightFundamentalCoords.HashFunction();
  }
  static inline int HashFunction(const MonomialChar<CoefficientType>& input)
  { return input.HashFunction();
  }
  inline bool operator==(const MonomialChar<CoefficientType>& other) const
  { return this->weightFundamentalCoords==other.weightFundamentalCoords;
  }
  inline bool operator>(const MonomialChar<CoefficientType>& other) const
  { return this->weightFundamentalCoords>other.weightFundamentalCoords;
  }
};

template <class CoefficientType>
class charSSAlgMod : public MonomialCollection<MonomialChar<CoefficientType>, CoefficientType>
{
  public:
  List<SemisimpleLieAlgebra>* listOwners;
  int indexInOwners;
  void Reset()
  { this->Clear();
    this->listOwners=0;
    this->indexInOwners=-1;
  }
  void MakeZero(List<SemisimpleLieAlgebra>& inputOwners, int inputIndex)
  { this->Clear();
    this->listOwners=&inputOwners;
    this->indexInOwners=inputIndex;
  }
  bool IsEqualToZero()
  { return this->size==0;
  }
  charSSAlgMod(): listOwners(0), indexInOwners(-1){}
  std::string ElementToStringCharacter(List<Vector<Rational> >& theWeights, List<Rational>& theMults);
  void MakeFromWeight
(const Vector<CoefficientType>& inputWeightSimpleCoords, List<SemisimpleLieAlgebra>& inputOwners,
 int inputIndexInOwner)
   ;
  bool GetDominantCharacterWRTsubalgebra
 (charSSAlgMod& outputCharOwnerSetToZero, std::string& outputDetails,
  GlobalVariables& theGlobalVariables, int upperBoundNumDominantWeights)
;
  bool FreudenthalEvalMe
 (charSSAlgMod& outputCharOwnerSetToZero, std::string& outputDetails,
  GlobalVariables& theGlobalVariables, int upperBoundNumDominantWeights)
;
  bool DrawMeNoMults
(std::string& outputDetails, GlobalVariables& theGlobalVariables,
 DrawingVariables& theDrawingVars, int upperBoundWeights)
  { return this->DrawMe(outputDetails, theGlobalVariables, theDrawingVars, upperBoundWeights, false);
  }
  bool DrawMeWithMults
(std::string& outputDetails, GlobalVariables& theGlobalVariables,
 DrawingVariables& theDrawingVars, int upperBoundWeights)
  { return this->DrawMe(outputDetails, theGlobalVariables, theDrawingVars, upperBoundWeights, true);
  }
  void DrawMeAssumeCharIsOverCartan
(WeylGroup& actualAmbientWeyl, GlobalVariables& theGlobalVariables, DrawingVariables& theDrawingVars)
  ;
  SemisimpleLieAlgebra& GetOwner()
  { return this->listOwners->TheObjects[this->indexInOwners];
  }

  bool DrawMe
(std::string& outputDetails, GlobalVariables& theGlobalVariables,
 DrawingVariables& theDrawingVars, int upperBoundWeights, bool useMults)
  ;
  bool SplitCharOverLevi
(std::string* Report, charSSAlgMod& output, Vector<Rational> & splittingParSel, const Vector<Rational> & ParSelFDInducingPart,
 ReflectionSubgroupWeylGroup& outputWeylSub, GlobalVariables& theGlobalVariables)
     ;
  void MakeTrivial(List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners);
  std::string MultiplyBy(const charSSAlgMod& other, GlobalVariables& theGlobalVariables);
  std::string operator*=(const charSSAlgMod& other);
  std::string operator*=(const MonomialChar<Rational>& other);
};

template <class CoefficientType>
class ModuleSSalgebraNew
{ List<Matrix<CoefficientType> > actionsGeneratorsMaT;
  List<List<List<ElementUniversalEnveloping<CoefficientType> > > > actionsGeneratorS;
  Selection ComputedGeneratorActions;
public:
  int indexAlgebra;
  List<SemisimpleLieAlgebra>* theAlgebras;
  HashedList<MonomialUniversalEnveloping<CoefficientType> > theGeneratingWordsNonReduced;
  Vectors<Rational> theGeneratingWordsNonReducedWeights;

  List<List<MonomialUniversalEnveloping<CoefficientType> > > theGeneratingWordsGrouppedByWeight;
  List<ElementUniversalEnveloping<CoefficientType> > theSimpleGens;
  List<List<List<ElementUniversalEnveloping<CoefficientType> > > > actionsSimpleGens;
  List<Matrix<CoefficientType> > actionsSimpleGensMatrixForM;
  List<Matrix<CoefficientType> > theBilinearFormsAtEachWeightLevel;
  List<Matrix<CoefficientType> > theBilinearFormsInverted;
  Vectors<Rational> weightsSimpleGens;

  Vector<CoefficientType> theHWDualCoordsBaseField;
  Vector<CoefficientType> theHWSimpleCoordSBaseField;

  Vector<Rational> theHWDualCoordS;
  Vector<Rational> theHWSimpleCoordS;
  Vector<Rational> theHWFundamentalCoordS;
  Vector<CoefficientType> theHWFundamentalCoordsBaseField;

//  List<List<Matrix<CoefficientType> > >
  HashedList<Vector<Rational> > theModuleWeightsSimpleCoords;
  charSSAlgMod<CoefficientType> theCharOverH;
  charSSAlgMod<CoefficientType> theChaR;
  Selection parabolicSelectionNonSelectedAreElementsLevi;
  Selection parabolicSelectionSelectedAreElementsLevi;
  bool flagIsInitialized;
  bool operator==(const ModuleSSalgebraNew<CoefficientType>& other)
  { return
    this->indexAlgebra==other.indexAlgebra
    && this->theHWFundamentalCoordsBaseField==other.theHWFundamentalCoordsBaseField
    && this->parabolicSelectionNonSelectedAreElementsLevi==other.parabolicSelectionNonSelectedAreElementsLevi;
  }
  void operator=(const ModuleSSalgebraNew<CoefficientType>& other)
  { if (this==&other)
      return;
    this->actionsGeneratorsMaT=other.actionsGeneratorsMaT;
    this->actionsGeneratorS=other.actionsGeneratorS;
    this->ComputedGeneratorActions=other.ComputedGeneratorActions;
    this->theAlgebras=other.theAlgebras;
    this->indexAlgebra=other.indexAlgebra;
    this->theGeneratingWordsNonReduced= other.theGeneratingWordsNonReduced;
    this->theGeneratingWordsNonReducedWeights=other.theGeneratingWordsNonReducedWeights;
    this->theGeneratingWordsGrouppedByWeight= other.theGeneratingWordsGrouppedByWeight;
    this->theSimpleGens=other.theSimpleGens;
    this->actionsSimpleGens=other.actionsSimpleGens;
    this->actionsSimpleGensMatrixForM= other.actionsSimpleGensMatrixForM;
    this->theBilinearFormsAtEachWeightLevel=other.theBilinearFormsAtEachWeightLevel;
    this->theBilinearFormsInverted=other.theBilinearFormsInverted;
    this->weightsSimpleGens=other.weightsSimpleGens;
    this->theHWDualCoordsBaseField=other.theHWDualCoordsBaseField;
    this->theHWDualCoordS = other.theHWDualCoordS;
    this->theHWSimpleCoordS=other.theHWSimpleCoordS;
    this->theHWSimpleCoordSBaseField=other.theHWSimpleCoordSBaseField;
    this->theHWFundamentalCoordS=other.theHWFundamentalCoordS;
    this->theHWFundamentalCoordsBaseField= other.theHWFundamentalCoordsBaseField;
    this->theModuleWeightsSimpleCoords=other.theModuleWeightsSimpleCoords;
    this->theCharOverH=other.theCharOverH;
    this->theChaR=other.theChaR;
    this->parabolicSelectionNonSelectedAreElementsLevi=other.parabolicSelectionNonSelectedAreElementsLevi;
    this->parabolicSelectionSelectedAreElementsLevi=other.parabolicSelectionSelectedAreElementsLevi;
    this->flagIsInitialized=other.flagIsInitialized;
//    std::cout << "<hr><hr><b>Copying from:</b> " << other.ElementToString()
//    << "<b>Copy result:</b>" << this->ElementToString() << "<br><b>End of copy</b><hr><hr>";

  }
  void SetNumVariables(int GoalNumVars);
  void Substitution(const PolynomialSubstitution<Rational>& theSub);
//  List<ElementUniversalEnveloping<CoefficientType> > theGeneratingWordsLittelmannForm;
//  HashedList<MonomialUniversalEnveloping<CoefficientType> > theGeneratingMonsPBWform;
//  List
//  List<Matrix<CoefficientType> > ActionsChevalleyGenerators;
  Matrix<CoefficientType>& GetActionGeneratorIndex
  (int generatorIndex, GlobalVariables& theGlobalVariables,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
 ;
  int GetNumVars()
  { if (this->theHWFundamentalCoordsBaseField.size<=0)
      return -1;
    return this->theHWFundamentalCoordsBaseField[0].GetNumVars();
  }
  int GetDim()
  { return this->theGeneratingWordsNonReduced.size;
  }
  void MakeFundamentalRep
(char WeylLetter, int theRank, GlobalVariables& theGlobalVariables)
  ;
  void IntermediateStepForMakeFromHW
( Vector<CoefficientType>& HWDualCoordS, GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit,
  const CoefficientType& theRingZero)
  ;
  bool MakeFromHW
(List<SemisimpleLieAlgebra>& inputAlgebras, int inputIndexAlgebra, Vector<CoefficientType>& HWFundCoords,
 const Selection& selNonSelectedAreElementsLevi, GlobalVariables& theGlobalVariables,
const CoefficientType& theRingUnit, const CoefficientType& theRingZero,
 std::string* outputReport)
  ;
  SemisimpleLieAlgebra& GetOwner()const{return this->theAlgebras->TheObjects[this->indexAlgebra];}
  void GetAdActionHomogenousElT
  (ElementUniversalEnveloping<CoefficientType>& inputHomogeneous, Vector<Rational> & weightUEEltSimpleCoords,
   List<List<ElementUniversalEnveloping<CoefficientType> > >& outputSortedByArgumentWeight,
   GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
   ;
  void GetMatrixHomogenousElt
  (ElementUniversalEnveloping<CoefficientType>& inputHomogeneous,
   List<List<ElementUniversalEnveloping<CoefficientType> > >& outputSortedByArgumentWeight,
    Vector<Rational> & weightUEEltSimpleCoords, Matrix<CoefficientType>& output,
   GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
   ;
  void ExpressAsLinearCombinationHomogenousElement
  (ElementUniversalEnveloping<CoefficientType>& inputHomogeneous,
   ElementUniversalEnveloping<CoefficientType>& outputHomogeneous,
   int indexInputBasis, const Vector<CoefficientType>& subHiGoesToIthElement,
   GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero
   )
  ;
  std::string ElementToString()const;
  std::string ElementToStringHWV(FormatExpressions* theFormat=0)const
  { return "hwv{}("+ this->GetOwner().GetLieAlgebraName(false)+ "," + this->theHWFundamentalCoordsBaseField.ElementToString(theFormat) + ","
    + Vector<Rational> (this->parabolicSelectionNonSelectedAreElementsLevi).ElementToString(theFormat) + ")";
  }
  void SplitOverLevi
  (std::string* Report, Vector<Rational>& splittingParSel, GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit,
   const CoefficientType& theRingZero)
   ;
   ModuleSSalgebraNew() : indexAlgebra(-1), theAlgebras(0), flagIsInitialized(false)
   {}
};

template<class CoefficientType>
class MonomialGeneralizedVerma
{
  public:
  List<ModuleSSalgebraNew<CoefficientType> >* owneR;
  int indexInOwner;
  MonomialUniversalEnveloping<CoefficientType> theMonCoeffOne;
  int indexFDVector;
  MonomialGeneralizedVerma(): owneR(0), indexInOwner(-1), indexFDVector(-1) { }
  void MultiplyMeByUEEltOnTheLeft
  (const CoefficientType& theCoeff, const ElementUniversalEnveloping<CoefficientType>& theUE, ElementSumGeneralizedVermas<CoefficientType>& outputAccum,
   GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
   ;
  void operator=(const MonomialGeneralizedVerma<CoefficientType>& other)
  { this->owneR=other.owneR;
    this->indexFDVector=other.indexFDVector;
    this->indexInOwner=other.indexInOwner;
    this->theMonCoeffOne=other.theMonCoeffOne;
  }

  std::string ElementToString
  (FormatExpressions* theFormat=0, bool includeV=true)const
  ;
  bool IsEqualToZero()const
  { return this->Coefficient.IsEqualToZero();
  }
  bool operator==(const MonomialGeneralizedVerma<CoefficientType>& other)
  { if (this->indexFDVector==other.indexFDVector && this->indexInOwner==other.indexInOwner)
      return this->theMonCoeffOne==other.theMonCoeffOne;
    return false;
  }
  void SetNumVariables(int GoalNumVars)
  { assert(this->owneR->size>this->indexInOwner);
    this->theMonCoeffOne.SetNumVariables(GoalNumVars);
    this->owneR->TheObjects[this->indexInOwner].SetNumVariables(GoalNumVars);
  }
  void Substitution(const PolynomialSubstitution<Rational>& theSub);
  int HashFunction()const
  { return this->indexFDVector*SomeRandomPrimes[0]+this->indexInOwner*SomeRandomPrimes[1];
  }
  static inline int HashFunction(const MonomialGeneralizedVerma<CoefficientType>& input)
  { return input.HashFunction();
  }
  bool operator>(const MonomialGeneralizedVerma<CoefficientType>& other)
  ;
  void ReduceMe
(const CoefficientType& myCoeff, ElementSumGeneralizedVermas<CoefficientType>& outputAccum, GlobalVariables& theGlobalVariables,
  const CoefficientType& theRingUnit, const CoefficientType& theRingZero)const
   ;
  void MakeConst(List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners)
  { this->theMonCoeffOne.MakeConst(inputOwners, inputIndexInOwners);
  }
  ModuleSSalgebraNew<CoefficientType>& GetOwner()const
  { return this->owneR->TheObjects[this->indexInOwner];
  }
};

template<class CoefficientType>
class ElementSumGeneralizedVermas : public MonomialCollection<MonomialGeneralizedVerma<CoefficientType>, CoefficientType >
{
  public:
  List<ModuleSSalgebraNew<CoefficientType> >* owneR;
  void MultiplyMeByUEEltOnTheLeft
  (const ElementUniversalEnveloping<CoefficientType>& theUE, GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
  ;
  void ReduceMonAndAddToMe
  (const MonomialGeneralizedVerma<CoefficientType>& theMon, const CoefficientType& theMonCoeff, GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
  ;
  int HashFunction()const
  { return this->MonomialCollection<MonomialGeneralizedVerma<CoefficientType>, CoefficientType >::HashFunction();
  }
  static int HashFunction(const ElementSumGeneralizedVermas<CoefficientType>& input)
  { return input.HashFunction();
  }
  ElementSumGeneralizedVermas():owneR(0){}
  void MakeHWV
  (List<ModuleSSalgebraNew<CoefficientType> >& theOwner, int TheIndexInOwner, const CoefficientType& theRingUnit)
  ;
  int GetNumVars()
  { if (this->owneR==0)
      return -1;
    if (this->owneR->size==0)
      return -1;
    int theAnswer=this->owneR->TheObjects[0].GetNumVars();
    for (int i=1; i<this->owneR->size; i++)
      if (theAnswer!=this->owneR->TheObjects[i].GetNumVars())
        return -1;
    return theAnswer;
  }
  void MakeZero
  (List<ModuleSSalgebraNew<CoefficientType> >& theOwner)
  { this->Clear();
    this->owneR=&theOwner;
  }
  std::string ElementToString
  (FormatExpressions* theFormat=0)const
    ;
  void operator=(const ElementSumGeneralizedVermas<CoefficientType>& other)
  { this->owneR=other.owneR;
    this->::MonomialCollection<MonomialGeneralizedVerma<CoefficientType>, CoefficientType>:: operator=(other);
  }
};

template <class CoefficientType>
class MonomialTensorGeneralizedVermas
{
public:
  List<MonomialGeneralizedVerma<CoefficientType> > theMons;
  void SimplifyNormalizeCoeffs
  ()
  ;
  void MakeZero( const CoefficientType& theRingZero)
  { this->Coefficient=theRingZero;
    this->theMons.SetSize(0);
  }
  int GetNumVars()
  { return this->Coefficient.GetNumVars();
  }
  void operator*=(const MonomialTensorGeneralizedVermas<CoefficientType>& other)
  { if (this==&other)
    { MonomialTensorGeneralizedVermas<CoefficientType> tempMon1;
      tempMon1=other;
      this->MultiplyBy(tempMon1);
      return;
    }
    this->AddListOnTop(other.theMons);
  }
  void operator*=(const MonomialGeneralizedVerma<CoefficientType>& other)
  { this->theMons.AddOnTop(other);
  }
  int HashFunction()const
  { int numCycles=MathRoutines::Minimum(SomeRandomPrimesSize, this->theMons.size);
    int result=0;
    for (int i=0; i<numCycles; i++)
      result+=SomeRandomPrimes[i]*this->theMons[i].HashFunction();
    return result;
  }
  static int HashFunction(const MonomialTensorGeneralizedVermas<CoefficientType>& input)
  { return input.HashFunction();
  }
  void SetNumVariables(int GoalNumVars)
  { for (int i=0; i<this->theMons.size; i++)
      this->theMons[i].SetNumVariables(GoalNumVars);
  }
  void Substitution(const PolynomialSubstitution<Rational>& theSub)
  { for (int i=0; i<this->theMons.size; i++)
      this->theMons[i].Substitution(theSub);
  }
  std::string ElementToString(FormatExpressions* theFormat=0, bool includeV=true)const
  ;
  MonomialTensorGeneralizedVermas(){}
  void operator=(const MonomialTensorGeneralizedVermas<CoefficientType>& other)
  { this->theMons=other.theMons;
  }
  void operator=(const MonomialGeneralizedVerma<CoefficientType>& other)
  ;
  bool operator==(const MonomialTensorGeneralizedVermas<CoefficientType>& other)const
  { if (this->theMons.size!=other.theMons.size)
      return false;
    for(int i=0; i<this->theMons.size; i++)
      if(!(this->theMons[i]==other.theMons[i]))
        return false;
    return true;
  }
  bool operator>(const MonomialTensorGeneralizedVermas<CoefficientType>& other)const
  { if (this->theMons.size>other.theMons.size)
      return true;
    if (other.theMons.size>this->theMons.size)
      return false;
    ///This might need a rewrite. As it is, it will cause monomials to be sorted according to the
    ///alphabetical order of their human-readable strings. If I have time, I will make a better scheme for
    ///comparison.
    return this->ElementToString()>other.ElementToString();
  }
};

template <class CoefficientType>
void MonomialTensorGeneralizedVermas <CoefficientType>::SimplifyNormalizeCoeffs
()
{ if (this->Coefficient.IsEqualToZero())
  { this->MakeZero(this->Coefficient.GetZero());
    return;
  }
  for (int i=0; i<this->theMons.size; i++)
  { CoefficientType& currentCF=this->theMons[i].Coefficient;
    if (currentCF.IsEqualToZero())
    { this->MakeZero(this->Coefficient.GetZero());
      return;
    }
    this->Coefficient*=currentCF;
    currentCF=currentCF.GetOne();
  }
}

template<class CoefficientType>
class ElementTensorsGeneralizedVermas :
public MonomialCollection<MonomialTensorGeneralizedVermas<CoefficientType>, CoefficientType >
{
public:
  bool MultiplyMeByUEEltOnTheLeft
  (const ElementUniversalEnveloping<CoefficientType>& theUE, ElementTensorsGeneralizedVermas<CoefficientType>& output,
   List<ModuleSSalgebraNew<CoefficientType> >& theOwner,
   GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)const
  ;
  bool MultiplyMeByUEEltOnTheLeft
  (const MonomialUniversalEnveloping<CoefficientType>& theUE, const CoefficientType& theCoeff,
   ElementTensorsGeneralizedVermas<CoefficientType>& output,
   List<ModuleSSalgebraNew<CoefficientType> >& theOwner,
   GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)const
  ;
  void MultiplyMeByElementLieAlg
  (List<ModuleSSalgebraNew<CoefficientType> >& theOwner, int indexGenerator,
   GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
  ;
  void MultiplyBy
  (const ElementTensorsGeneralizedVermas<CoefficientType>& standsOnTheRight,
   GlobalVariables& theGlobalVariables)
   ;
  void MakeHWV
  (List<ModuleSSalgebraNew<CoefficientType> >& theOwner, int TheIndexInOwner, const CoefficientType& theRingUnit)
  ;
  void Substitution(const PolynomialSubstitution<Rational>& theSub)
  ;
  void SetNumVariables
  (int GoalNumVars)
  { for (int i=0; i<this->size; i++)
      this->TheObjects[i].SetNumVariables(GoalNumVars);
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
  void MakeZero
  ()
  { this->Clear();
  }
  std::string ElementToString
  (FormatExpressions* theFormat=0)const
    ;
  int HashFunction()const
  { return this->:: MonomialCollection<MonomialTensorGeneralizedVermas<CoefficientType>, CoefficientType >::HashFunction();
  }
  void operator=(const ElementTensorsGeneralizedVermas<CoefficientType>& other)
  { this->::MonomialCollection<MonomialTensorGeneralizedVermas<CoefficientType>, CoefficientType>::operator=(other);
  }
  void operator=(const ElementSumGeneralizedVermas<CoefficientType>& other);
  static int HashFunction(const ElementTensorsGeneralizedVermas<CoefficientType>& input)
  { return input.HashFunction();
  }
};

class Parser;
class ParserNode
{
private:
  bool ConvertToNextType
(int GoalType, int GoalNumVariables, bool& ErrorHasOccured, GlobalVariables& theGlobalVariables)
;
public:
  std::string DebugString;
  std::string outputString;
  void ComputeDebugString(GlobalVariables& theGlobalVariables, FormatExpressions& theFormat){ this->ElementToString(this->DebugString, theGlobalVariables, theFormat); }
  void ElementToString(std::string& output, GlobalVariables& theGlobalVariables, FormatExpressions& theFormat);
  Parser* owner;
  int indexParentNode;
  int indexInOwner;
  int Operation; //using tokenTypes from class Parser
  int ExpressionType;
  int ErrorType;
  int IndexContextLieAlgebra;
  int impliedNumVars;
  bool Evaluated;
  MemorySaving<Polynomial<Rational> > polyValue;
  MemorySaving<ElementWeylAlgebra> WeylAlgebraElement;
  MemorySaving<ElementUniversalEnveloping<Polynomial<Rational> > > UEElement;
  MemorySaving<ElementUniversalEnvelopingOrdered<Polynomial<Rational> > > UEElementOrdered;
  MemorySaving<Polynomial<Rational> > polyBeingMappedTo;
  MemorySaving<ElementWeylAlgebra> weylEltBeingMappedTo;
  MemorySaving<RationalFunction> ratFunction;
  MemorySaving<Cone> theCone;
  MemorySaving<Lattice> theLattice;
  MemorySaving<QuasiPolynomial> theQP;
  MemorySaving<charSSAlgMod<Rational> > theChar;
  MemorySaving<partFractions> thePFs;
  MemorySaving<PiecewiseQuasipolynomial> thePiecewiseQP;
  MemorySaving<AnimationBuffer> theAnimation;
  MemorySaving<LittelmannPath> theLittelmann;
  MemorySaving<ElementTensorsGeneralizedVermas<RationalFunction> > theGenVermaElt;
  List<int> children;
  int intValue;
  Rational rationalValue;
  void operator=(const ParserNode& other);
  void Clear();
  int GetMaxImpliedNumVarsChildren();
  int GetMaxNumImpliedVarsFromUnderscore();
  int GetStrongestExpressionChildrenConvertChildrenIfNeeded(GlobalVariables& theGlobalVariables);
  void ConvertChildrenAndMyselfToStrongestExpressionChildren(GlobalVariables& theGlobalVariables);
  void CopyValue(const ParserNode& other);
  bool ConvertToType
(int theType, int GoalNumVars, GlobalVariables& theGlobalVariables)
  ;
  bool ConvertChildrenToType(int theType, int GoalNumVars, GlobalVariables& theGlobalVariables);
  //the order of the types matters, they WILL be compared by numerical value!
  enum typeExpression{typeUndefined=0, typeIntegerOrIndex, typeRational, typeLieAlgebraElement, typePoly, typeRationalFunction, typeUEElementOrdered, //=6
  typeUEelement, typeGenVermaElt, typeWeylAlgebraElement, typeMapPolY, typeMapWeylAlgebra, typeString, typePDF, typeLattice, typeCone, //=14
  typeArray, typeQuasiPolynomial, typePartialFractions, //=17
  typeCharSSFDMod,
  typePiecewiseQP,
  typeAnimation,
  typeLittelman,
  typeFile, typeDots,
  typeError //typeError must ALWAYS have the highest numerical value!!!!!
  };
  enum typesErrors{errorNoError=0, errorDivisionByZero, errorDivisionByNonAllowedType, errorMultiplicationByNonAllowedTypes,
  errorUnknownOperation, errorOperationByUndefinedOrErrorType, errorProgramming, errorBadIndex, errorDunnoHowToDoOperation,
  errorWrongNumberOfArguments,//=9
  errorBadOrNoArgument, errorBadSyntax, errorBadSubstitution, errorConversionError, errorDimensionProblem,//=14
  errorImplicitRequirementNotSatisfied, errorBadFileFormat };
  void InitForAddition(GlobalVariables* theContext);
  void InitForMultiplication(GlobalVariables* theContext);
  std::string ElementToStringValueAndType
  (bool useHtml, GlobalVariables& theGlobalVariables, bool displayOutputString, FormatExpressions& theFormat)
  { return this->ElementToStringValueAndType(useHtml, 0, 3, theGlobalVariables, displayOutputString, theFormat);
  }
  std::string ElementToStringValueAndType
(bool useHtml, int RecursionDepth, int maxRecursionDepth, GlobalVariables& theGlobalVariables, bool displayOutputString, FormatExpressions& theFormat)
;
  SemisimpleLieAlgebra& GetContextLieAlgebra();
  std::string ElementToStringValueOnlY(bool useHtml, GlobalVariables& theGlobalVariables, FormatExpressions& theFormat)
  { return this->ElementToStringValueOnlY(useHtml, 0, 2, theGlobalVariables, theFormat);
  }
  std::string ElementToStringValueOnlY
  (bool useHtml, int RecursionDepth, int maxRecursionDepth, GlobalVariables& theGlobalVariables, FormatExpressions& theFormat)
;
  std::string ElementToStringErrorCode(bool useHtml);
  void TrimSubToMinNumVarsChangeImpliedNumVars(PolynomialSubstitution<Rational>& theSub, int theDimension);
  template <class CoefficientType>
bool GetRootRationalDontUseForFunctionArguments
(Vector<CoefficientType>& output, GlobalVariables& theGlobalVariables)
;
  template <class CoefficientType>
CoefficientType& GetElement();
  template <class CoefficientType>
bool GetListDontUseForFunctionArguments
(List<CoefficientType>& output, GlobalVariables& theGlobalVariables, int ExpressionType, int theImpliedNumVars)
;
bool GetRootRationalFromFunctionArguments
(//List<int>& argumentList,
 Vector<Rational> & output, GlobalVariables& theGlobalVariables)
;
bool GetRootSRationalDontUseForFunctionArguments
(
 Vectors<Rational>& output, int& outputDim, GlobalVariables& theGlobalVariables)
;
  bool GetRootInt(Vector<int>& output, GlobalVariables& theGlobalVariables);
  void CopyError(ParserNode& other) {this->ExpressionType=other.ExpressionType; this->ErrorType=other.ErrorType;}
  int SetError(int theError){this->ExpressionType=this->typeError; this->ErrorType=theError; return theError;}
  int CarryOutSubstitutionInMe(PolynomialSubstitution<Rational>& theSub, GlobalVariables& theGlobalVariables);
  void ReduceRatFunction();
  void EvaluateLieBracket(GlobalVariables& theGlobalVariables);
  void Evaluate(GlobalVariables& theGlobalVariables);
  int EvaluateTimes(GlobalVariables& theGlobalVariables);
  void EvaluateDivide(GlobalVariables& theGlobalVariables);
  void EvaluateOrder(GlobalVariables& theGlobalVariables);
  void CreateDefaultLatexAndPDFfromString
  (const std::string& theLatexFileString)
  ;
  void EvaluateInteger(GlobalVariables& theGlobalVariables);
  bool GetRootsEqualDimNoConversionNoEmptyArgument
(List<int>& theArgumentList, Vectors<Rational>& output, int& outputDim)
;
  bool GetListRootsEqualSizeEqualDimNoConversionNoEmptyArgument
(List<int>& theArgumentList, List<Vectors<Rational> >& output, int& outputRootsSize, int& outputDim,
 GlobalVariables& theGlobalVariables)
;
  static int EvaluateInvariantsExteriorPowerFundamentalRepsPlusTrivialReps
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  ;
  static int EvaluateRepresentationFromHWFundCoords
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  ;
  static int EvaluateLattice
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateMultiplyEltGenVermaOnTheRight
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateDrawWeightSupportWithMults
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateHWMTABilinearForm
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateHWTAABilinearForm
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateSplitGenVermaBthreeOverGtwo
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateFreudenthal
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateHWV
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateIsInProperSubmoduleVermaModule
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateLittelmannPaths
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateSplitIrrepOverLeviParabolic
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateSplitFDPartGenVermaOverLeviParabolic
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateSplitCharOverLeviParabolic
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateMakeWeylFromParSel
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateLittelmannPathFromWayPoints
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateAllLittelmannPaths
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateLittelmannEAlpha
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;

  static int EvaluateGetCoxeterBasis
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateAnimationPause
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateAnimationClearScreen
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateChar
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateDrawRootSystemCoxeterPlaneRootSA
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateAnimationDots
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateAnimateRootSAs
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateMinusTransposeAuto
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateTransposeAntiAuto
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateAnimationClonePreviousFrameDrawExtraLine
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateG2ParabolicSupport
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;

  static int EvaluateGroebner
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateG2InB3MultsParabolic
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateRelations
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateModOutRelsFromGmodKtimesVerma
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateCreateFromDirectionsAndSalamiSlice
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateParabolicWeylGroups
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateActByWeylAlgebraElement
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateParabolicWeylGroupsBruhatGraph
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateVPF
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;

  static int EvaluateLatticeImprecise
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateMakeCasimir
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateDrawWeightSupport
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateAdjointAction
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateWeylAction
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  { return theNode.EvaluateWeylAction(theNode, theArgumentList, theGlobalVariables, false, false, false);}
  static int EvaluateWeylRhoAction
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  { return theNode.EvaluateWeylAction(theNode, theArgumentList, theGlobalVariables, false, true, false);}
  static int EvaluateWeylMinusRhoAction
    (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  { return theNode.EvaluateWeylAction(theNode, theArgumentList,  theGlobalVariables, false, true, true);}
  static int EvaluateSolveLPolyEqualsZeroOverCone
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  ;
  static int EvaluateSliceCone
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  ;
  static int EvaluateAnimateRootSystemBluePoint
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateAnimateRootSystem
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  { return ParserNode::EvaluateAnimateRootSystem(theNode, theArgumentList, theGlobalVariables, 0);
  }
  static int EvaluateAnimateRootSystem
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables, Vector<Rational> * bluePoint)
  ;
  static int EvaluateCone(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables);
  static int EvaluateConeFromVertices(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables);
  static int EvaluateSlTwoInSlN(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables);
  static int EvaluateVectorPFIndicator
(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateDrawRootSystem
  (ParserNode& theNode, char WeylLetter, int WeylRank, GlobalVariables& theGlobalVariables,
   Vector<Rational> * bluePoint, bool wipeCanvas=true, bool LabelDynkin=false, bool DrawWeylChamber=false, Vectors<Rational>* projectionPlane=0)
  ;
  static int EvaluateDrawRootSystemLabelDynkin
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  ;
  static int EvaluateDrawRootSystemOld
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables, Vector<Rational> * bluePoint)
;
  static int EvaluateDrawRootSystem
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables, Vector<Rational> * bluePoint)
;
  static int EvaluateDrawRootSystemFixedProjectionPlane
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateDrawRootSystem
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  { return theNode.EvaluateDrawRootSystem(theNode, theArgumentList, theGlobalVariables, 0);}
  static int EvaluateDrawRootSystemOld
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables){return EvaluateDrawRootSystemOld(theNode, theArgumentList, theGlobalVariables,0);}
  static int EvaluatePrintRootSAsAndSlTwos
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables, bool redirectToSlTwos, bool forceRecompute)
;
  static int EvaluateDrawConeAffine
(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateDrawConeProjective
(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateG2InB3Computation
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateClosestPointToHyperplaneAlongTheNormal
(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateDecomposeOverSubalgebra
(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  int EvaluateWriteToFile(GlobalVariables& theGlobalVariables);
  void EvaluateOuterAutos(GlobalVariables& theGlobalVariables);
  void EvaluateMinus(GlobalVariables& theGlobalVariables);
  void EvaluateDereferenceArray(GlobalVariables& theGlobalVariables);
  void EvaluateMinusUnary(GlobalVariables& theGlobalVariables);
  void EvaluatePrintWeyl(GlobalVariables& theGlobalVariables);
  void EvaluateGCDorLCM(GlobalVariables& theGlobalVariables);
  static int EvaluateWeylAction
  (ParserNode& theNode,
   List<int>& theArgumentList, GlobalVariables& theGlobalVariables,
   bool DualAction, bool useRho, bool useMinusRho)
  ;
  int EvaluatePlus(GlobalVariables& theGlobalVariables);
  static int EvaluatePartialFractions
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  ;
  static int EvaluateSplit
    (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateFindExtremaInDirectionOverLattice
    (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int  EvaluateWeylDimFormula
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  ;
  static int EvaluateIntersectLatticeWithPreimageLattice
    (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  ;
  void EvaluatePrintEmbedding(GlobalVariables& theGlobalVariables);
  static int EvaluatePrintDecomposition
    (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  ;
  static int EvaluatePrintRootSystem
    (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateFactorial
    (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluatePrintAllPrimesEratosthenes
    (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateInvariantsSl2DegreeM
    (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  ;
  static int EvaluateEigen
    (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  ;
  static int EvaluateEigenOrdered
    (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  ;
  void EvaluateSecretSauce(GlobalVariables& theGlobalVariables);
  static int EvaluateSecretSauceOrdered
    (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
  ;
  void EvaluateThePower(GlobalVariables& theGlobalVariables);
  void EvaluateUnderscore(GlobalVariables& theGlobalVariables);
  int EvaluateUnderscoreLeftArgumentIsArray(GlobalVariables& theGlobalVariables);
  void EvaluateEmbedding(GlobalVariables& theGlobalVariables);
  int EvaluateSubstitution(GlobalVariables& theGlobalVariables);
  int EvaluateApplySubstitution(GlobalVariables& theGlobalVariables);
  static int EvaluateModVermaRelations
   (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  static int EvaluateModVermaRelationsOrdered
   (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
;
  void EvaluateFunction(GlobalVariables& theGlobalVariables);
  void ExtractAndEvalWeylSubFromMap(GlobalVariables& theGlobalVariables);
  bool AllChildrenAreOfDefinedNonErrorType();
  bool OneChildrenOrMoreAreOfType(int theType);
  ParserNode();
};


class ParserFunctionArgumentTree
{ public:
  //format: the if the function argument is composite, then it corresponds to the first non-used element of nestedArgumentsOfArguments
  // for example, the function argument (A, (B,C), D, (E,F) )
  // would have functionArguments: typeA, typeComposite, typeD, typeComposite
  // and nestedArgumentsOfArguments would have two elements, the parametrizing respectively (B ,C) and (E,F)
  List<int> functionArguments;
  List<ParserFunctionArgumentTree> nestedArgumentsOfArguments;
  void operator=(const ParserFunctionArgumentTree& other)
  { this->functionArguments=other.functionArguments;
    this->nestedArgumentsOfArguments=other.nestedArgumentsOfArguments;
  }
  std::string ElementToString(bool useHtml, bool useLatex)const;
  void MakeZero(){this->functionArguments.size=0; this->nestedArgumentsOfArguments.size=0;}

bool ConvertOneArgumentIndex
  (ParserNode& theNode, int theIndex, int& lowestNestedIndexNonExplored, GlobalVariables& theGlobalVariables)
  ;
  bool ConvertArguments
  (ParserNode& theNode, List<int>& outputArgumentIndices, GlobalVariables& theGlobalVariables)
    ;
  bool MakeFunctionArgumentFromString(const std::string& theArgumentList);
  bool MakeFromString(unsigned int& currentChar, const std::string& theArgumentList);
  bool AddArgumentNonNestedChangeInput(std::string& theArgument);
  static int GetTokenFromArgumentStringChangeInput(std::string& theArgument)
  { unsigned int finalSize=0;
    for (unsigned int i=0; i< theArgument.size(); i++)
      if (theArgument[i]!=' ')
      { theArgument[finalSize]=theArgument[i];
        finalSize++;
      }
    theArgument.resize(finalSize);
    return ParserFunctionArgumentTree::GetTokenFromArgumentStringNoSpaces(theArgument);
  }
  static int GetTokenFromArgumentStringNoSpaces(const std::string& theArgument);
  static std::string GetArgumentStringFromToken(int theArgument);

};

class ParserFunction
{
public:
  ParserFunctionArgumentTree theArguments;
  int (*theActualFunction)(ParserNode& theNode, List<int>& theArguments, GlobalVariables& theGlobalVariables);
  char exampleAmbientWeylLetter;
  int exampleAmbientWeylRank;
  bool flagVisible;
  std::string functionDescription;
  std::string functionName;
  std::string example;
  void operator=(const ParserFunction& other)
  { this->theArguments=other.theArguments;
    this->theActualFunction=other.theActualFunction;
    this->functionDescription=other.functionDescription;
    this->functionName=other.functionName;
    this->example=other.example;
    this->flagVisible=other.flagVisible;
    this->exampleAmbientWeylRank=other.exampleAmbientWeylRank;
    this->exampleAmbientWeylLetter=other.exampleAmbientWeylLetter;
  }
  std::string ElementToString
(bool useHtml, bool useLatex, Parser& owner)const
  ;
  int CallMe
  (ParserNode& theNode, GlobalVariables& theGlobalVariables)
  ;
  bool MakeMe
  (const std::string& theFunctionName, const std::string& theFunctionArguments, const std::string& theFunctionDescription, const std::string& theExample, int (*inputFunctionAddress)(ParserNode& theNode, List<int>& theArguments, GlobalVariables& theGlobalVariables))
  ;
  int HashFunction()const
  { return this->GetHashFromString(this->functionName);
  }
  static inline int HashFunction(const ParserFunction& input)
  { return input.HashFunction();
  }
  int GetHashFromString(const std::string& input)const
  { int numCycles=MathRoutines::Minimum(SomeRandomPrimesSize, (int) input.size());
    int result=0;
    for(int i=0; i<numCycles; i++)
      result+=SomeRandomPrimes[i]*input[i];
    return result;
  }
  bool operator>(const ParserFunction& other) const
  { return this->functionName>other.functionName;
  }
  bool operator==(const ParserFunction& other)const
  { return this->functionName==other.functionName;
  }
  ParserFunction(){}
  void MakeFunctionNoArguments(){this->theArguments.MakeZero();}
};

//the below class was written and implemented by an idea of helios from the forum of www.cplusplus.com
class Parser: public List<ParserNode>
{
public:
  std::string DebugString;
  std::string StringBeingParsed;
  List<std::string> SystemCommands;

  std::string DisplayNameCalculator;

  std::string DisplayPathServerBase;
  std::string PhysicalPathServerBase;
  char DefaultWeylLetter;
  int DefaultWeylRank;

  std::string DisplayPathOutputFolder;
  std::string PhysicalPathOutputFolder;

  std::string DisplayNameDefaultOutput;
  std::string DisplayNameDefaultOutputNoPath;
  std::string PhysicalNameDefaultOutput;

  std::string indicatorFileName;
  std::string indicatorFileNameDisplay;
  std::string indicatorReportFileName;
  std::string indicatorReportFileNameDisplay;
  std::string userLabel;
  int MaxFoundVars;
  bool flagDisplayIndicator;
  ParserNode theValue;
  HomomorphismSemisimpleLieAlgebra theHmm;
  SemisimpleLieAlgebraOrdered testAlgebra;
  SemisimpleLieAlgebraOrdered testSubAlgebra;
  ModuleSSalgebraNew<Rational> theModulE;
  List<ModuleSSalgebraNew<RationalFunction> > theModulePolys;
  List<SemisimpleLieAlgebra> theAlgebras;

  std::string javaScriptDisplayingIndicator;
  std::string afterSystemCommands;
  int GetNumVarsModulePolys();
  void SetNumVarsModulePolys(int NumVars);
//  SemisimpleLieAlgebra theLieAlgebra;
  void ComputeDebugString(bool includeLastNode, GlobalVariables& theGlobalVariables, FormatExpressions& theFormat)
  { this->ElementToString(includeLastNode, DebugString, true, theGlobalVariables, theFormat);
  }
  void ElementToString(bool includeLastNode, std::string& output, bool useHtml, GlobalVariables& theGlobalVariables, FormatExpressions& theFormat);
  enum tokenTypes
  { tokenExpression, tokenEmpty, tokenEnd, tokenDigit, tokenInteger, tokenPlus, tokenMinus, tokenMinusUnary, tokenUnderscore,  //=8
    tokenTimes, tokenDivide, tokenPower, tokenOpenBracket, tokenCloseBracket,//=13
    tokenOpenLieBracket, tokenCloseLieBracket, tokenOpenCurlyBracket, tokenCloseCurlyBracket, tokenX, tokenF, //=19
    tokenPartialDerivative, tokenComma, tokenLieBracket, tokenG, //=23
    tokenH, tokenC, tokenEmbedding, tokenVariable, //=27
    tokenArraY, tokenMapsTo, tokenColon, tokenDereferenceArray,
    tokenDot,
    tokenEndStatement, tokenFunction, tokenFunctionNoArgument,
  };
  HashedList<ParserFunction> theFunctionList;
  enum functionList
  { functionEigen, functionEigenOrdered, functionLCM, functionGCD, functionSecretSauce, functionSecretSauceOrdered, //=5
    functionWeylDimFormula, functionOuterAutos, functionMod, functionInvariants, functionOrder, functionEmbedding, //=11
    functionPrintDecomposition, functionPrintRootSystem, functionSlTwoInSlN, functionActByWeyl, functionActByAffineWeyl, //=16
    functionPrintWeylGroup, functionChamberParam, functionMaximumLFoverCone, functionCone, functionLattice, //21
    functionGetAllRepresentatives, functionInvertLattice, functionQuasiPolynomial, functionPartialFractions, functionSplit, //=26
    functionGetVPIndicator, functionMaximumQPoverCone, functionWriteToFile, functionReadFromFile, //=30
    functionIntersectWithSubspace, functionIntersectLatticeWithLatticePreimage, functionSubstitutionInQuasipolynomial, //=33
    functionIntersectHyperplaneByACone,
  };
  List<int> TokenBuffer;
  List<int> ValueBuffer;
  List<int> TokenStack;
  List<int> ValueStack;
  List<int> NodeIndexStack;
  int numEmptyTokensAtBeginning;
  bool flagFunctionListInitialized;
//  ElementSemisimpleLieAlgebra LieAlgebraValue;
//  ElementWeylAlgebra WeylAlgebraValue;
  //Transform operations create no new nodes.
  //Replace operations create new nodes.
  void ExpandOnTop(int numNew)
  { this->SetSize(this->size+numNew);
    for (int i=0; i<numNew; i++)
    { this->TheObjects[this->size-1-i].owner=this;
      this->TheObjects[this->size-1-i].Clear();
      this->TheObjects[this->size-1-i].indexInOwner=this->size-1-i;
    }
  }
  bool ReplaceObyE()
  { this->ExpandOnTopUseOperationOffset(0, this->tokenExpression);
    this->TransformRepeatXAtoA(1);
    return true;
  }
  bool ReplaceYOZbyE()
  { return this->ReplaceTwoChildrenOperationOffset(-2, 0, -1, 3, this->tokenExpression);
  }
  bool ReplaceOXAXbyE()
  { return this->ReplaceOneChildOperationOffset(-1, -3, 4, this->tokenExpression);
  }
  bool ReplaceOYbyE()
  { return this->ReplaceOneChildOperationOffset(0, -1, 2, this->tokenExpression);
  }
  bool ReplaceZYbyE(int theOperationToken)
  { return this->ReplaceTwoChildrenOperationToken(-1,0,2, theOperationToken, this->tokenExpression);
  }
  bool ReplaceXYbyE(int theOperationToken)
  { return this->ReplaceOneChildOperationToken(0, 2, theOperationToken, this->tokenExpression);
  }
  bool ReplaceXYCZXbyE(int theOperationToken)
  { return this->ReplaceTwoChildrenOperationToken(-3, -1, 5, theOperationToken, this->tokenExpression);
  }
  bool ReplaceXYXbyA()
  { return this->ReplaceOneChildOperationToken(-1, 3, this->tokenArraY, this->tokenArraY);
  }
  bool ReplaceEXEXbyE(int theOperation)
  { return this->ReplaceTwoChildrenOperationToken(-3, -1, 4, theOperation, this->tokenExpression);
  }
  bool ReplaceDdotsDbyEdoTheArithmetic();
  bool ReplaceXECdotsCEXbyE(int theDimension, int theNewToken, int theOperation);
  bool ReplaceXECdotsCEXEXbyE(int theDimension, int theNewToken, int theOperation);
  void FatherByLastNodeChildrenWithIndexInNodeIndex(int IndexInNodeIndex)
  { int childIndex=this->NodeIndexStack.TheObjects[IndexInNodeIndex];
    this->LastObject()->children.AddOnTop(childIndex);
    this->TheObjects[childIndex].indexParentNode=this->size-1;
  }
  bool ReplaceOneChildOperationToken
  (int child1Offset, int StackDecreaseNotCountingNewExpression, int theOperationToken, int theToken)
  { int lastIndexInNodeIndex=this->TokenStack.size-1;
    this->ExpandOnTopIncreaseStacks(theOperationToken, theToken, 0);
    this->FatherByLastNodeChildrenWithIndexInNodeIndex(lastIndexInNodeIndex+child1Offset);
    this->TransformRepeatXAtoA(StackDecreaseNotCountingNewExpression);
    return true;
  }
  bool ReplaceTwoChildrenOperationToken
  (int child1Offset, int child2Offset, int StackDecreaseNotCountingNewExpression, int theOperationToken, int theToken)
  { int lastIndexInNodeIndex=this->TokenStack.size-1;
    this->ExpandOnTopIncreaseStacks(theOperationToken, theToken, 0);
    this->FatherByLastNodeChildrenWithIndexInNodeIndex(lastIndexInNodeIndex+child1Offset);
    this->FatherByLastNodeChildrenWithIndexInNodeIndex(lastIndexInNodeIndex+child2Offset);
    this->TransformRepeatXAtoA(StackDecreaseNotCountingNewExpression);
    return true;
  }
  bool ExpandOnTopIncreaseStacks(int theOperationToken, int theToken, int theIntValue)
  { this->ExpandOnTop(1);
    this->LastObject()->Operation=theOperationToken;
    this->LastObject()->intValue=theIntValue;
    this->NodeIndexStack.AddOnTop(this->size-1);
    this->ValueStack.AddOnTop(theIntValue);
    this->TokenStack.AddOnTop(theToken);
    return true;
  }
  bool ExpandOnTopUseOperationOffset(int OperationOffset, int theToken)
  { return this->ExpandOnTopIncreaseStacks(this->TokenStack.TheObjects[this->TokenStack.size-1+OperationOffset], theToken, this->ValueStack.TheObjects[this->TokenStack.size-1+OperationOffset]);
  }
  bool ReplaceTwoChildrenOperationOffset
  (int child1Offset, int child2Offset, int OperationOffset, int StackDecreaseNotCountingNewExpression, int theToken)
  { int lastIndexInNodeIndex=this->TokenStack.size-1;
    this->ExpandOnTopUseOperationOffset(OperationOffset, theToken);
    this->FatherByLastNodeChildrenWithIndexInNodeIndex(lastIndexInNodeIndex+child1Offset);
    this->FatherByLastNodeChildrenWithIndexInNodeIndex(lastIndexInNodeIndex+child2Offset);
    this->TransformRepeatXAtoA(StackDecreaseNotCountingNewExpression);
    return true;
  }
  std::string CreateBasicStructureConstantInfoIfItDoesntExist
  (GlobalVariables& theGlobalVariables)
  ;
  bool ReplaceOneChildOperationOffset
  (int child1Offset, int OperationOffset, int StackDecreaseNotCountingNewExpression, int theToken)
  { int lastIndexInNodeIndex=this->TokenStack.size-1;
    this->ExpandOnTopUseOperationOffset(OperationOffset, theToken);
    this->FatherByLastNodeChildrenWithIndexInNodeIndex(lastIndexInNodeIndex+child1Offset);
    this->TransformRepeatXAtoA(StackDecreaseNotCountingNewExpression);
    return true;
  }
  bool TransformXtoE()
  { *this->TokenStack.LastObject()=this->tokenExpression;
    *this->ValueStack.LastObject()=0;
    return true;
  }
  bool TransformRepeatXAtoA(int repeat)
  { int startIndex=this->ValueStack.size-1;
    int targetIndex=startIndex-repeat;
    this->ValueStack.SwapTwoIndices(startIndex, targetIndex);
    this->NodeIndexStack.SwapTwoIndices(startIndex, targetIndex);
    this->TokenStack.SwapTwoIndices(startIndex, targetIndex);
    this->ValueStack.size-=repeat;
    this->NodeIndexStack.size-=repeat;
    this->TokenStack.size-=repeat;
    return true;
  }
  bool TransformXtoNothing()
  { this->ValueStack.PopLastObject();
    this->TokenStack.PopLastObject();
    this->NodeIndexStack.PopLastObject();
    return true;
  }
  bool TransformRepeatXtoNothing(int repeat)
  { for (int i=0; i<repeat; i++)
      this->TransformXtoNothing();
    return true;
  }
  bool TransformXYXtoY()
  { this->TransformXtoNothing();
    this->TransformRepeatXAtoA(1);
    return true;
  }
  void InitAndTokenize(const std::string& input);
  void Evaluate(GlobalVariables& theGlobalVariables);
  std::string ParseEvaluateAndSimplify
  (const std::string& input, bool useHtml, GlobalVariables& theGlobalVariables, FormatExpressions& theFormat)
;
  void ParseEvaluateAndSimplifyPart1(const std::string& input, GlobalVariables& theGlobalVariables);
  std::string ParseEvaluateAndSimplifyPart2
  (const std::string& input, bool useHtml, GlobalVariables& theGlobalVariables, FormatExpressions& theFormat)
;
  ElementUniversalEnveloping<Polynomial<Rational> > ParseAndCompute
  (const std::string& input, GlobalVariables& theGlobalVariables)
  ;
  void Parse(const std::string& input);
  void ParseNoInit(int indexFrom, int indexTo);
  void ParseAndCompute(const std::string& input, std::string& output, GlobalVariables& theGlobalVariables);
  bool ApplyRules(int lookAheadToken);
  bool lookAheadTokenProhibitsPlus(int theToken);
  bool lookAheadTokenProhibitsTimes(int theToken);
  bool lookAheadTokenAllowsMapsTo(int theToken);
  bool TokenProhibitsUnaryMinus(int theToken);
  void initTestAlgebraNeedsToBeRewritteN(GlobalVariables& theGlobalVariables);
  void initTestAlgebraNeedsToBeRewrittenG2InB3(GlobalVariables& theGlobalVariables);
  void initFunctionList(char defaultExampleWeylLetter, int defaultExampleWeylRank);
  void AddOneFunctionToDictionaryNoFail
  (const std::string& theFunctionName, const std::string& theFunctionArguments, const std::string& theFunctionDescription,
   const std::string& theExample, int (*inputFunctionAddress)(ParserNode& theNode, List<int>& theArguments, GlobalVariables& theGlobalVariables)
   )
  { bool tempBool=this->AddOneFunctionToDictionary
      (theFunctionName, theFunctionArguments, theFunctionDescription, theExample, 'B', 3, true, inputFunctionAddress);
    assert(tempBool);
  }
  void AddOneFunctionToDictionaryNoFail
  (const std::string& theFunctionName, const std::string& theFunctionArguments, const std::string& theFunctionDescription, const std::string& theExample, char ExampleWeylLetter, int ExampleWeylRank, bool isVisible, int (*inputFunctionAddress)(ParserNode& theNode, List<int>& theArguments, GlobalVariables& theGlobalVariables)
   )
  { bool tempBool=this->AddOneFunctionToDictionary(theFunctionName, theFunctionArguments, theFunctionDescription, theExample, ExampleWeylLetter, ExampleWeylRank, isVisible, inputFunctionAddress);
    assert(tempBool);
  }
  void AddOneFunctionToDictionaryNoFail
  (const std::string& theFunctionName, const std::string& theFunctionArguments,
   const std::string& theFunctionDescription, const std::string& theExample, char ExampleWeylLetter,
   int ExampleWeylRank, int (*inputFunctionAddress)(ParserNode& theNode, List<int>& theArguments, GlobalVariables& theGlobalVariables)
   )
  { bool tempBool=this->AddOneFunctionToDictionary(theFunctionName, theFunctionArguments, theFunctionDescription, theExample, ExampleWeylLetter, ExampleWeylRank, true, inputFunctionAddress);
    assert(tempBool);
  }
  bool AddOneFunctionToDictionary
  (const std::string& theFunctionName, const std::string& theFunctionArguments, const std::string& theFunctionDescription,
   const std::string& theExample, char ExampleWeylLetter, int ExampleWeylRank, bool isVisible,
   int (*inputFunctionAddress)(ParserNode& theNode, List<int>& theArguments, GlobalVariables& theGlobalVariables)
)
  { ParserFunction newFunction;
    bool result=newFunction.MakeMe(theFunctionName, theFunctionArguments, theFunctionDescription, theExample, inputFunctionAddress);
    if (!this->theFunctionList.AddNoRepetition(newFunction))
      return false;
    this->theFunctionList.LastObject()->exampleAmbientWeylLetter=ExampleWeylLetter;
    this->theFunctionList.LastObject()->exampleAmbientWeylRank=ExampleWeylRank;
    this->theFunctionList.LastObject()->flagVisible=isVisible;
    return result;
  }
  void SubAby(int newToken);
  std::string GetFunctionDescription();
  bool StackTopIsASub(int& outputNumEntries);
  bool StackTopIsAVector(int& outputDimension);
  bool StackTopIsDelimiter1ECdotsCEDelimiter2(int& outputDimension, int LeftDelimiter, int RightDelimiter);
  bool StackTopIsDelimiter1ECdotsCEDelimiter2EDelimiter3(int& outputDimension, int LeftDelimiter, int middleDelimiter, int rightDelimiter);
  bool IsAWordSeparatingCharacter(char c);
  bool LookUpInDictionaryAndAdd(std::string& input);
  void TokenToStringStream(std::stringstream& out, int theToken);
  void Clear();
  Parser(){ this->flagFunctionListInitialized=false; this->flagDisplayIndicator=true;}
};

class PolynomialOverModule;


template <class CoefficientType>
class ElementVermaModuleOrdered
{
public:
  ElementUniversalEnvelopingOrdered<CoefficientType> theElT;
  std::string DebugString;
  PolynomialSubstitution<CoefficientType> theSubNthElementIsImageNthCoordSimpleBasis;
  void ComputeDebugString(){FormatExpressions tempFormat; this->DebugString=this->ElementToString(tempFormat);}
  std::string ElementToString(const FormatExpressions& theFormat)const;
  void ElementToString(std::string& output)const{output=this->ElementToString();}
  bool IsEqualToZero()const {return this->theElT.IsEqualToZero();}
  bool NeedsBrackets()const;
  void AssignElementUniversalEnvelopingOrderedTimesHighestWeightVector
  (ElementUniversalEnvelopingOrdered<CoefficientType>& input, const ElementVermaModuleOrdered<CoefficientType>& theRingZero,
   GlobalVariables* theContext, const CoefficientType& theRingUnit)
  ;
  void ActOnMe
  (const ElementSemisimpleLieAlgebra& actingElt, ElementVermaModuleOrdered<CoefficientType>& output, SemisimpleLieAlgebra& owner, const CoefficientType& theRingUnit, const CoefficientType& theRingZero, GlobalVariables* theContext)const
  ;
  static void GetBasisFromSpanOfElements
  (List<ElementVermaModuleOrdered<CoefficientType> >& theElements, Vectors<RationalFunction>& outputCoordinates, List<ElementVermaModuleOrdered>& outputTheBasis,
    const RationalFunction& RFOne, const RationalFunction& RFZero, GlobalVariables& theGlobalVariables)
    ;
  bool GetCoordsInBasis
  (const List<ElementVermaModuleOrdered<CoefficientType> >& theBasis, Vector<CoefficientType>& output, const CoefficientType& theRingUnit, const CoefficientType& theRingZero, GlobalVariables& theGlobalVariables)const
  ;
  bool IsProportionalTo(const ElementVermaModuleOrdered<CoefficientType>& other, CoefficientType& outputTimesMeEqualsOther, const CoefficientType& theRingZero)const{ return this->theElT.IsProportionalTo(other.theElT, outputTimesMeEqualsOther, theRingZero); }
  void MakeZero(SemisimpleLieAlgebraOrdered& owner, PolynomialSubstitution<Rational>& incomingSub)
  { this->theElT.MakeZero(owner);
    this->theSubNthElementIsImageNthCoordSimpleBasis=incomingSub;
  }
  template <class CoefficientTypeOther>
  void operator*=(const CoefficientTypeOther& theConst)
  { this->theElT.operator*=<CoefficientTypeOther>(theConst);
  }
  void MultiplyOnTheLeft
(const ElementSemisimpleLieAlgebra& other, ElementVermaModuleOrdered<CoefficientType>& output, const CoefficientType& theRingUnit, const CoefficientType& theRingZero, GlobalVariables* theContext)
;
  void ClearDenominators(CoefficientType& outputWasMultipliedBy, const CoefficientType& theRingUnit){ this->theElT.ClearDenominators(outputWasMultipliedBy, theRingUnit); }
  void operator/=(const CoefficientType& theConst){this->theElT.operator/=(theConst);}
  void operator-=(const ElementVermaModuleOrdered& other){ this->theElT-=other.theElT;}
  void operator+=(const ElementVermaModuleOrdered& other){ this->theElT+=other.theElT;}
  void operator=(const ElementVermaModuleOrdered& other)
  { this->theElT=other.theElT;
    this->theSubNthElementIsImageNthCoordSimpleBasis=other.theSubNthElementIsImageNthCoordSimpleBasis;
  }
};

class slTwoInSlN
{
  int GetModuleIndexFromHighestWeightVector(const Matrix<Rational> & input)
  { Rational tempRat;
    for (int i=0; i<this->theHighestWeightVectors.size; i++)
      if (this->theHighestWeightVectors.TheObjects[i].IsProportionalTo(input, tempRat))
        return i;
    return -1;
  }
public:
  int theDimension;
  Matrix<Rational>  theH;
  Matrix<Rational>  theE;
  Matrix<Rational>  theF;

  List<int> thePartition;
  List<Matrix<Rational> > theProjectors;
  List<Matrix<Rational> > theHighestWeightVectors;
  List<List<Matrix<Rational> > > theGmodKModules;
  List<List<List<int> > > PairingTable;
  void GetIsPlusKIndexingFrom(int input, int& s, int& k);
  std::string ElementMatrixToTensorString(const Matrix<Rational> & input, bool useHtml);
  std::string initFromModuleDecomposition(List<int>& decompositionDimensions, bool useHtml, bool computePairingTable);
  std::string initPairingTable(bool useHtml);
  std::string ElementModuleIndexToString(int input, bool useHtml);
  std::string GetNotationString(bool useHtml);
  bool ComputeInvariantsOfDegree
  (List<int>& decompositionDimensions, int theDegree, List<Polynomial<Rational> >& output, std::string& outputError, GlobalVariables& theGlobalVariables)
  ;
  std::string PairTwoIndices
  (List<int>& output, int leftIndex, int rightIndex, bool useHtml)
  ;
  void ExtractHighestWeightVectorsFromVector
  (Matrix<Rational> & input, List<Matrix<Rational> >& outputDecompositionOfInput, List<Matrix<Rational> >& outputTheHWVectors)
  ;
  void ClimbDownFromHighestWeightAlongSl2String
  (Matrix<Rational> & input, Matrix<Rational> & output, Rational& outputCoeff, int generatorPower)
  ;
  void ClimbUpFromVector
  (Matrix<Rational> & input, Matrix<Rational> & outputLastNonZero, int& largestPowerNotKillingInput)
  ;
};

class GeneralizedVermaModuleCharacters
{
public:
  inline static const std::string GetXMLClassName(){ return "GeneralizedVermaCharacters";}
  Controller thePauseControlleR;
  List<Matrix<Rational> > theLinearOperators;
  //the first k variables correspond to the Cartan of the smaller Lie algebra
  //the next l variables correspond to the Cartan of the larger Lie algebra
  //the last variable is the projectivization
  List<Matrix<Rational> > theLinearOperatorsExtended;
  Vector<Rational>  NonIntegralOriginModificationBasisChanged;
  std::fstream theMultiplicitiesMaxOutput;
  std::fstream theMultiplicitiesMaxOutputReport2;
  Vectors<Rational> GmodKnegativeWeightS;
  Vectors<Rational> GmodKNegWeightsBasisChanged;
  Matrix<Rational>  preferredBasisChangE;
  Matrix<Rational>  preferredBasisChangeInversE;
  Vectors<Rational> preferredBasiS;
  Cone PreimageWeylChamberLargerAlgebra;
  Cone WeylChamberSmallerAlgebra;
  Cone PreimageWeylChamberSmallerAlgebra;
  Lattice theExtendedIntegralLatticeMatForM;
  List<QuasiPolynomial> theQPsNonSubstituted;
  List<List<QuasiPolynomial> > theQPsSubstituted;
  List<QuasiPolynomial> theMultiplicities;
//  List<QuasiPolynomial> theMultiplicitiesExtremaCandidates;
  int UpperLimitChambersForDebugPurposes;
  int numNonZeroMults;
  Selection ParabolicLeviPartRootSpacesZeroStandsForSelected;
  Selection ParabolicSelectionSmallerAlgebra;
  List<Rational> theCoeffs;
  Vectors<Rational> theTranslationS;
  Vectors<Rational> theTranslationsProjectedBasisChanged;
  partFractions thePfs;
//  List<Cone> allParamSubChambersRepetitionsAllowedConeForm;
  ConeComplex projectivizedParamComplex;
  ConeLatticeAndShiftMaxComputation theMaxComputation;
  ConeComplex smallerAlgebraChamber;
  ConeComplex projectivizedChambeR;
  std::stringstream log;
  Parser theParser;
  WeylGroup WeylSmaller;
  WeylGroup WeylLarger;
  int computationPhase;
  int NumProcessedConesParam;
  int NumProcessedExtremaEqualOne;
  std::string ComputeMultsLargerAlgebraHighestWeight
  ( Vector<Rational> & highestWeightLargerAlgebraFundamentalCoords, Vector<Rational> & parabolicSel, Parser& theParser, GlobalVariables& theGlobalVariables
   )
  ;
  std::string CheckMultiplicitiesVsOrbits
  (GlobalVariables& theGlobalVariables)
  ;
  std::string ElementToStringMultiplicitiesReport
  (GlobalVariables& theGlobalVariables)
  ;
  void IncrementComputation
  (Vector<Rational> & parabolicSel, GlobalVariables& theGlobalVariables)
  ;
  std::string PrepareReport(GlobalVariables& theGlobalVariables);
  GeneralizedVermaModuleCharacters()
  { this->UpperLimitChambersForDebugPurposes=-1;
    this->computationPhase=0;
    this->NumProcessedConesParam=0;
    this->NumProcessedExtremaEqualOne=0;
    this->numNonZeroMults=0;
  }
  void ReadFromDefaultFile(GlobalVariables* theGlobalVariables);
  void WriteToDefaultFile(GlobalVariables* theGlobalVariables);
  void WriteToFile
  (std::fstream& output, GlobalVariables* theGlobalVariables)
  ;
  bool ReadFromFile
  (std::fstream& input, GlobalVariables* theGlobalVariables)
  { std::string tempS;
    int numReadWords;
    Vector<Rational>  parSel; parSel=this->ParabolicLeviPartRootSpacesZeroStandsForSelected;
    if (theGlobalVariables!=0)
    { this->theParser.theHmm.MakeG2InB3(this->theParser, *theGlobalVariables);
      this->initFromHomomorphism(parSel, this->theParser.theHmm, *theGlobalVariables);
    } else
    { GlobalVariables tempGlobalVars;
      this->theParser.theHmm.MakeG2InB3(this->theParser, tempGlobalVars);
      this->initFromHomomorphism(parSel, this->theParser.theHmm, tempGlobalVars);
    }
    XMLRoutines::ReadThroughFirstOpenTag(input, numReadWords, this->GetXMLClassName());

    input >> tempS >> this->computationPhase;
    assert(tempS=="ComputationPhase:");
    bool result=true;
    if (this->computationPhase!=0)
      result= this->ReadFromFileNoComputationPhase(input, theGlobalVariables);
    XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, numReadWords, this->GetXMLClassName());
    return result;
  }
  bool ReadFromFileNoComputationPhase
  (std::fstream& input, GlobalVariables* theGlobalVariables)
  ;
  std::string PrepareReportOneCone
  (FormatExpressions& theFormat, Cone& theCone, GlobalVariables& theGlobalVariables)
  ;
  void GetProjection(int indexOperator, const Vector<Rational> & input, Vector<Rational> & output);
  void SplitByMultiplicityFreeWall(Cone& theCone, ConeComplex& output);
  void InitTheMaxComputation
  (GlobalVariables& theGlobalVariables)
  ;
  void ComputeQPsFromChamberComplex
  (GlobalVariables& theGlobalVariables)
  ;
  void GetSubFromIndex
  (PolynomialSubstitution<Rational>& outputSub, Matrix<LargeInt>& outputMat, LargeIntUnsigned& ouputDen, int theIndex)
;
  void SortMultiplicities(GlobalVariables& theGlobalVariables);
  void GetSubFromNonParamArray
(Matrix<Rational> & output, Vector<Rational> & outputTranslation, Vectors<Rational>& NonParams, int numParams)
  ;
  void initQPs
  (GlobalVariables& theGlobalVariables)
  ;
  void initFromHomomorphism
  (Vector<Rational> & theParabolicSel, HomomorphismSemisimpleLieAlgebra& input, GlobalVariables& theGlobalVariables)
  ;
  void TransformToWeylProjectiveStep1
  (GlobalVariables& theGlobalVariables)
  ;
  void TransformToWeylProjectiveStep2
  (GlobalVariables& theGlobalVariables)
  ;
  void TransformToWeylProjective
  (int indexOperator, Vector<Rational> & startingNormal, Vector<Rational> & outputNormal)
  ;
};

class SemisimpleSubalgebras
{
public:
  std::string DebugString;
  std::string ElementToString();
  void ComputeDebugString(){this->DebugString=this->ElementToString();}
  SltwoSubalgebras theSl2s;
  SemisimpleLieAlgebra theAlgebra;
  List<Vectors<Rational> >  theHcandidates;
  int indexLowestUnexplored;
  void FindHCandidates
    (char WeylLetter, int WeylDim, GlobalVariables& theGlobalVariables)
  ;
  void FindHCandidatesWithOneExtraHContaining
  (Vectors<Rational>& inpuT, GlobalVariables& theGlobalVariables)
  ;
};

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::MakeOneGenerator
(int theIndex, const CoefficientType& theCoeff, SemisimpleLieAlgebraOrdered& owner, GlobalVariables* theContext)
{ this->MakeZero(owner);
  MonomialUniversalEnvelopingOrdered<CoefficientType> tempMon;
//  tempMon.MakeZero(theCoeff.NumVars, owner, theContext);
  tempMon.Coefficient=theCoeff;
  tempMon.MultiplyByGeneratorPowerOnTheRight(theIndex, tempMon.Coefficient);
  this->AddOnTop(tempMon);
}

template<typename Element>
void Matrix<Element>::RowToRoot(int rowIndex, Vector<Element>& output)const
{ output.SetSize(this->NumCols);
  for (int i=0; i<this->NumCols; i++)
    output.TheObjects[i]=this->elements[rowIndex][i];
}

template<typename Element>
void Matrix<Element>::ColToRoot(int colIndex, Vector<Element>& output)const
{ output.SetSize(this->NumRows);
  for (int i=0; i<this->NumRows; i++)
    output[i]=this->elements[i][colIndex];
}

class affineHyperplane
{
public:
  std::string DebugString;
  Vector<Rational>  affinePoint;
  Vector<Rational>  normal;
  void ElementToString(std::string& output);
  void ComputeDebugString(){this->ElementToString(this->DebugString); }
  //void InduceFromFacet(Facet& input);
  //the below returns false if the projection is not of full dimension
  int HashFunction()const;
  static inline int HashFunction(const affineHyperplane& input)
  { return input.HashFunction();
  }
//  bool ProjectFromFacet(Facet& input);
  bool ProjectFromFacetNormal(Vector<Rational> & input);
  bool ContainsPoint(Vector<Rational> & thePoint);
  void MakeFromNormalAndPoint(Vector<Rational> & inputPoint, Vector<Rational> &inputNormal);
  bool HasACommonPointWithPositiveTwoToTheNth_ant();
  void Assign(const affineHyperplane& right)
  { this->affinePoint=right.affinePoint;
    this->normal=right.normal;
  }
  inline void operator=(const affineHyperplane& right){ this->Assign(right); }
  bool operator==(const affineHyperplane& right);
};

class affineHyperplanes: public List<affineHyperplane>
{
public:
  std::string DebugString;
  void ElementToString(std::string& output);
  void ComputeDebugString(){this->ElementToString(this->DebugString); }
};

class affineCone
{
public:
  affineHyperplanes theWalls;
  int HashFunction() const;
  inline static int HashFunction(const affineCone& input){return input.HashFunction();}
  inline int GetDimension();
  void SuperimposeAffineCones(affineCones& theOtherComplex);
  //void induceFromCombinatorialChamber(CombinatorialChamber& input);
  bool WallIsInternalInCone(affineHyperplane& theKillerCandidate);
  //The below function returns true if the system of homogeneous linear inequalities Ax<=b
  //has a solution, false otherwise, where A is a matrix and x and b are column vectors.
//  bool SystemLinearInequalitiesHasSolution
//    (Matrix<Rational> & matA, Matrix<Rational> & matb, Matrix<Rational> & outputPoint);
  bool SplitByAffineHyperplane(affineHyperplane& theKillerPlane, affineCones& output);
  void Assign(const affineCone& right){this->theWalls.CopyFromBase(right.theWalls); }
  inline void operator=(const affineCone& right){this->Assign(right); }
};

class affineCones: public HashedList<affineCone>
{
public:
  void SuperimposeAffineCones(affineCones& theOtherComplex);
};

template <class Object>
void List<Object>::QuickSortAscending(int BottomIndex, int TopIndex)
{ if (TopIndex<=BottomIndex)
    return;
  int HighIndex = TopIndex;
  for (int LowIndex = BottomIndex+1; LowIndex<=HighIndex; LowIndex++)
    if (this->TheObjects[LowIndex]>(this->TheObjects[BottomIndex]))
    { this->SwapTwoIndices(LowIndex, HighIndex);
      LowIndex--;
      HighIndex--;
    }
  if (this->TheObjects[HighIndex]>this->TheObjects[BottomIndex])
    HighIndex--;
  this->SwapTwoIndices(BottomIndex, HighIndex);
  this->QuickSortAscending(BottomIndex, HighIndex-1);
  this->QuickSortAscending(HighIndex+1, TopIndex);
}

template <class Object>
void List<Object>::AddListOnTop(const List<Object>& theList)
{ int oldsize= this->size;
  int otherSize=theList.size;
  this->SetSize(oldsize+otherSize);
  for (int i=0; i<otherSize; i++)
    this->TheObjects[i+oldsize]= theList.TheObjects[i];
}

template<class Object>
void List<Object>::SwapTwoIndices(int index1, int index2)
{ if (index1==index2)
    return;
  Object tempO;
  tempO= this->TheObjects[index1];
  this->TheObjects[index1]=this->TheObjects[index2];
  this->TheObjects[index2]=tempO;
}

template<class Object>
int List<Object>::IndexOfObject(const Object& o) const
{ for (int i=0; i<this->size; i++)
    if (this->TheObjects[i]==o)
      return i;
  return -1;
}

template <class Object>
void List<Object>::swap(List<Object>& l1, List<Object>& l2)
{ List<Object>* bigL;
  List<Object>* smallL;
  int smallSize;
  if (l1.size<l2.size)
  { smallL=&l1;
    bigL=&l2;
    smallSize=l1.size;
  } else
  { bigL=&l1;
    smallL=&l2;
    smallSize=l2.size;
  }
  smallL->SetSize(bigL->size);
  Object tempO;
  for(int i=0; i<smallSize; i++)
  { tempO=smallL->TheObjects[i];
    smallL->TheObjects[i]=bigL->TheObjects[i];
    bigL->TheObjects[i]=tempO;
  }
  for(int i=smallSize; i<bigL->size; i++)
    smallL->TheObjects[i]=bigL->TheObjects[i];
  bigL->size=smallSize;
}

template <class Object>
int List<Object>::SizeWithoutObjects()
{ return  sizeof(this->ActualSize)+ sizeof(this->IndexOfVirtualZero)+ sizeof(this->size)+ sizeof(this->TheActualObjects)+ sizeof(this->TheObjects);
}

template <class Object>
bool List<Object>::operator>(const List<Object>& other)const
{ if (this->size>other.size)
    return true;
  if (other.size>this->size)
    return false;
  for (int i=0; i<this->size; i++)
  { if (this->TheObjects[i]>other.TheObjects[i])
      return true;
    if (other.TheObjects[i]>this->TheObjects[i])
      return false;
  }
  return false;
}

template <class Object>
void List<Object>::ShiftUpExpandOnTop(int StartingIndex)
{ this->SetSize(this->size+1);
  for (int i=this->size-1; i>StartingIndex; i--)
    this->TheObjects[i]= this->TheObjects[i-1];
}

template <class Object>
void List<Object>::initFillInObject(int theSize, const Object& o)
{ this->SetSize(theSize);
  for (int i=0; i<this->size; i++)
    this->TheObjects[i]=o;
}

template <class Object>
bool List<Object>::AddOnTopNoRepetition(const Object& o)
{ if (this->IndexOfObject(o)!=-1)
    return false;
  this->AddOnTop(o);
  return true;
}

template <class Object>
inline Object* List<Object>::LastObject()
{ return &this->TheObjects[this->size-1];
}

template <class Object>
bool List<Object>::HasACommonElementWith(List<Object>& right)
{ for(int i=0; i<this->size; i++)
    for (int j=0; j<right.size; j++)
      if (this->TheObjects[i]==right.TheObjects[j])
        return true;
  return false;
}

template <class Object>
void List<Object>::AssignLight(const ListLight<Object>& From)
{ this->SetSize(From.size);
  for (int i=0; i<this->size; i++)
    this->TheObjects[i]= From.TheObjects[i];
}

template <class Object>
void List<Object>::CopyFromBase(const List<Object>& From)
{ if (this==&From)
    return;
  this->SetSize(From.size);
  for (int i=0; i<this->size; i++)
    this->TheObjects[i]= From.TheObjects[i];
}

template <class Object>
void List<Object>::Reserve(int theSize)
{ if (!(this->ActualSize>= this->IndexOfVirtualZero+theSize))
  { ParallelComputing::SafePointDontCallMeFromDestructors();
    this->ExpandArrayOnTop(this->IndexOfVirtualZero+theSize- this->ActualSize);
  }
}

template <class Object>
void List<Object>::RemoveFirstOccurenceSwapWithLast(const Object& o)
{ for (int i=0; i<this->size; i++)
    if (o==this->TheObjects[i])
    { this->PopIndexSwapWithLast(i);
      return;
    }
}

template <class Object>
void List<Object>::SetSize(int theSize)
{ if (theSize<0)
    theSize=0;
  this->Reserve(theSize);
  this->size=theSize;
}

template <class Object>
inline void List<Object>::ElementToStringGeneric(std::string& output)const
{ this->ElementToStringGeneric(output, this->size);
}

template <class Object>
inline void List<Object>::ElementToStringGeneric(std::string& output, int NumElementsToPrint)const
{ std::stringstream out;
  int Upper=MathRoutines::Minimum(NumElementsToPrint, this->size);
  for (int i=0; i<Upper; i++)
    out << this->TheObjects[i].ElementToString() << "\n";
  output= out.str();
}

template <class Object>
inline void List<Object>::AddObjectOnTopCreateNew()
{ this->SetSize(this->size+1);
}

template <class Object>
void List<Object>::PopIndexShiftUp(int index)
{ if (size==0)
    return;
  this->size--;
  for (int i=index; i>=1; i--)
    this->TheObjects[i]=this->TheObjects[i-1];
  this->TheObjects++;
  IndexOfVirtualZero++;
}

template <class Object>
void List<Object>::PopIndexSwapWithLast(int index)
{ if (this->size==0)
    return;
  this->size--;
  this->TheObjects[index]=this->TheObjects[this->size];
}

template <class Object>
void List<Object>::PopLastObject()
{ if (this->size==0)
    return;
  this->size--;
}

template <class Object>
List<Object>::List()
{ this->initConstructorCallOnly();
}

template <class Object>
List<Object>::List(int StartingSize)
{ this->initConstructorCallOnly();
  this->SetSize(StartingSize);
}

template <class Object>
List<Object>::List(int StartingSize, const Object& fillInObject)
{ this->initConstructorCallOnly();
  this->initFillInObject(StartingSize, fillInObject);
}

template <class Object>
void List<Object>::ReleaseMemory()
{ delete [] this->TheActualObjects;
#ifdef CGIversionLimitRAMuse
ParallelComputing::GlobalPointerCounter-=this->ActualSize;
  ParallelComputing::CheckPointerCounters();
#endif
  this->ActualSize=0;
  this->IndexOfVirtualZero=0;
  this->size=0;
  this->TheObjects=0;
  this->TheActualObjects=0;
}

template <class Object>
List<Object>::~List()
{ delete [] this->TheActualObjects;
#ifdef CGIversionLimitRAMuse
ParallelComputing::GlobalPointerCounter-=this->ActualSize;
  this->ActualSize=0;
  ParallelComputing::CheckPointerCounters();
#endif
  this->TheActualObjects=0;
  this->TheObjects=0;
}

template <class Object>
void List<Object>::ExpandArrayOnBottom(int increase)
{ if (increase<=0)
    return;
  Object* newArray = new Object[this->ActualSize+increase];
#ifdef CGIversionLimitRAMuse
ParallelComputing::GlobalPointerCounter+=this->ActualSize+increase;
  ParallelComputing::CheckPointerCounters();
#endif
  for (int i=0; i<this->size; i++)
    newArray[i+increase+this->IndexOfVirtualZero]=this->TheObjects[i];
  delete [] this->TheActualObjects;
#ifdef CGIversionLimitRAMuse
ParallelComputing::GlobalPointerCounter-=this->ActualSize;
  ParallelComputing::CheckPointerCounters();
#endif
  this->TheActualObjects= newArray;
  this->ActualSize+=increase;
  this->IndexOfVirtualZero+=increase;
  this->TheObjects = this->TheActualObjects+this->IndexOfVirtualZero;
}

template <class Object>
void List<Object>::ExpandArrayOnTop(int increase)
{ if (increase<=0)
    return;
  Object* newArray = new Object[this->ActualSize+increase];
#ifdef CGIversionLimitRAMuse
ParallelComputing::GlobalPointerCounter+=this->ActualSize+increase;
  ParallelComputing::CheckPointerCounters();
#endif
  for (int i=0; i<this->size; i++)
    newArray[i+this->IndexOfVirtualZero]=this->TheObjects[i];
  delete [] this->TheActualObjects;
#ifdef CGIversionLimitRAMuse
ParallelComputing::GlobalPointerCounter-=this->ActualSize;
  ParallelComputing::CheckPointerCounters();
#endif
  this->TheActualObjects= newArray;
  this->ActualSize+=increase;
  this->TheObjects = this->TheActualObjects+this->IndexOfVirtualZero;
}

template <class Object>
void List<Object>::ReverseOrderElements()
{ int tempI= this->size/2;
  for (int i=0; i<tempI; i++)
    this->SwapTwoIndices(i, this->size-i-1);
}

template <class Object>
void List<Object>::AddObjectOnBottom(const Object& o)
{ if (this->IndexOfVirtualZero==0)
    this->ExpandArrayOnBottom(List<Object>::ListActualSizeIncrement);
  this->IndexOfVirtualZero--;
  this->TheObjects--;
  this->TheObjects[0]=o;
  this->size++;
}

template <class Object>
void List<Object>::AddOnTop(const Object& o)
{ if (this->IndexOfVirtualZero+this->size>=this->ActualSize)
    this->ExpandArrayOnTop(List<Object>::ListActualSizeIncrement);
  this->TheObjects[size]=o;
  this->size++;
}

template<class Object>
void HashedListReferences<Object>::IncreaseSizeWithZeroPointers(int increase)
{ if (increase<=0)
    return;
  if (this->ActualSize<this->size+increase)
  { Object** newArray= new Object*[this->size+increase];
#ifdef CGIversionLimitRAMuse
ParallelComputing::GlobalPointerCounter+=increase;
  ParallelComputing::CheckPointerCounters();
#endif
    for (int i=0; i<this->size; i++)
      newArray[i]=this->TheObjects[i];
    delete [] this->TheObjects;
    this->TheObjects= newArray;
    this->ActualSize+=increase;
  }
  for(int i=this->size; i<this->ActualSize; i++)
    this->TheObjects[i]=0;
  this->size+=increase;
}

template<class Object>
void HashedListReferences<Object>::initAndCreateNewObjects(int d)
{ this->KillAllElements();
  this->SetSize(d);
  for (int i=0; i<d; i++)
    this->TheObjects[i]=new Object;
#ifdef CGIversionLimitRAMuse
ParallelComputing::GlobalPointerCounter+=d;
  ParallelComputing::CheckPointerCounters();
#endif
}

template<class Object>
void HashedListReferences<Object>::resizeToLargerCreateNewObjects(int increase)
{ if (increase<=0)
    return;
  int oldsize= this->size;
  this->SetSize(this->size+increase);
  for (int i=oldsize; i<this->size; i++)
    this->TheObjects[i]=new Object;
#ifdef CGIversionLimitRAMuse
ParallelComputing::GlobalPointerCounter+=this->size-oldsize;
  ParallelComputing::CheckPointerCounters();
#endif
}

template<class Object>
void HashedListReferences<Object>::KillAllElements()
{ for (int i =0; i<this->size; i++)
  { delete this->TheObjects[i];
#ifdef CGIversionLimitRAMuse
    if (this->TheObjects[i]!=0)ParallelComputing::GlobalPointerCounter--;
    ParallelComputing::CheckPointerCounters();
#endif
    this->TheObjects[i]=0;
  }
  this->size=0;
}

template<class Object>
bool HashedListReferences<Object>::AddOnTop(const Object& o)
{ if (this->ContainsObject(o)==-1)
  { this->AddOnTop(o);
    return true;
  }
  return false;
}

class VermaModulesWithMultiplicities: public HashedList<Vector<Rational> >
{
public:
  WeylGroup* TheWeylGroup;
  List<int> TheMultiplicities;
  List<bool> Explored;
  int NextToExplore;
  int LowestNonExplored;
  List<List<int> > BruhatOrder;
  List<List<int> > SimpleReflectionsActionList;
  List<List<int> > InverseBruhatOrder;
  std::string DebugString;
  //important: in both the R- and KL-polynomials, if a polynomial Rxy is non-zero,
  //then x is bigger than y. This is the opposite to the usually accepted convention!
  //The reason for that is the following: if you want to compute
  //once you are done with computing with a given highest weight,
  //you want to be able to release the used memory; that is why the higher weight must
  //be the first, not the second index!
  List<OneVarIntPolynomialSubstitution> KLPolys;
  List<OneVarIntPolynomialSubstitution> RPolys;
  void KLcoeffsToString(List<int>& theKLCoeffs, std::string& output);
  void FindNextToExplore();
  int FindLowestBruhatNonExplored();
  int FindHighestBruhatNonExplored(List<bool>& theExplored);
  void initTheMults();
  void Compute(int x);
  void Check();
  //returns true if reduction succeeded, false otherwise
  bool ComputeRxy(int x, int y, int SimpleReflectionIndex);
  void ComputeKLxy(int w, int x);
  bool IsMaxNonEplored(int index);
  bool IndexGEQIndex(int a, int b);
  bool IndexGreaterThanIndex(int a, int b);
  void ComputeDebugString();
  void ElementToString(std::string& output);
  void MergeBruhatLists(int fromList, int toList);
  void KLPolysToString(std::string& output);
  void ComputeKLcoefficientsFromIndex(int ChamberIndex, List<int>& output);
  void ComputeKLcoefficientsFromChamberIndicator(Vector<Rational> & ChamberIndicator, List<int>& output);
  int ChamberIndicatorToIndex(Vector<Rational> & ChamberIndicator);
  void RPolysToString(std::string& output);
  void ComputeKLPolys(WeylGroup* theWeylGroup, int TopChamberIndex);
  void ComputeRPolys();
  int ComputeProductfromSimpleReflectionsActionList(int x, int y);
  void WriteKLCoeffsToFile(std::fstream& output, List<int>& KLcoeff, int TopIndex);
  //returns the TopIndex of the KL coefficients
  int ReadKLCoeffsFromFile(std::fstream& input, List<int>& output);
  VermaModulesWithMultiplicities(){this->TheWeylGroup=0; }
  void GeneratePartialBruhatOrder();
  void ExtendOrder();
  void ComputeFullBruhatOrder();
  void initFromWeyl(WeylGroup* theWeylGroup);
};


template<class CoefficientType>
void ElementVermaModuleOrdered<CoefficientType>::GetBasisFromSpanOfElements
  (List<ElementVermaModuleOrdered>& theElements, Vectors<RationalFunction>& outputCoordinates, List<ElementVermaModuleOrdered>& outputTheBasis,
    const RationalFunction& RFOne, const RationalFunction& RFZero, GlobalVariables& theGlobalVariables)
{ List<ElementUniversalEnvelopingOrdered<CoefficientType> > theEltsUEform;
  theEltsUEform.SetSize(theElements.size);
  for (int i=0; i<theElements.size; i++)
    theEltsUEform.TheObjects[i]=theElements.TheObjects[i].theElT;
  List<ElementUniversalEnvelopingOrdered<CoefficientType> > theBasisUEform;
  ElementUniversalEnvelopingOrdered<CoefficientType>::GetBasisFromSpanOfElements(theEltsUEform, outputCoordinates, theBasisUEform, RFOne, RFZero, theGlobalVariables);
  outputTheBasis.SetSize(theBasisUEform.size);
  for (int i=0; i<theBasisUEform.size; i++)
  { outputTheBasis.TheObjects[i].theElT=theBasisUEform.TheObjects[i];
    outputTheBasis.TheObjects[i].theSubNthElementIsImageNthCoordSimpleBasis=theElements.TheObjects[0].theSubNthElementIsImageNthCoordSimpleBasis;
  }
}

template<class CoefficientType>
bool ElementVermaModuleOrdered<CoefficientType>::GetCoordsInBasis
  (const List<ElementVermaModuleOrdered<CoefficientType> >& theBasis, Vector<CoefficientType>& output, const CoefficientType& theRingUnit, const CoefficientType& theRingZero, GlobalVariables& theGlobalVariables)const
{ List<ElementUniversalEnvelopingOrdered<CoefficientType> > theEltsUEform;
  theEltsUEform.SetSize(theBasis.size);
  for (int i=0; i<theBasis.size; i++)
  { theEltsUEform.TheObjects[i]=theBasis.TheObjects[i].theElT;
  }
  return this->theElT.GetCoordsInBasis(theEltsUEform, output, theRingUnit, theRingZero, theGlobalVariables);
}

template<class CoefficientType>
bool ElementVermaModuleOrdered<CoefficientType>::NeedsBrackets()const
{ return this->theElT.NeedsBrackets();
}

template <class CoefficientType>
void ElementVermaModuleOrdered<CoefficientType>::AssignElementUniversalEnvelopingOrderedTimesHighestWeightVector
  (ElementUniversalEnvelopingOrdered<CoefficientType>& input, const ElementVermaModuleOrdered<CoefficientType>& theRingZero, GlobalVariables* theContext, const CoefficientType& theRingUnit)
{ this->theElT.operator=(input);
  assert(theRingZero.theSubNthElementIsImageNthCoordSimpleBasis.size==3);
  this->theSubNthElementIsImageNthCoordSimpleBasis=theRingZero.theSubNthElementIsImageNthCoordSimpleBasis;
  this->theElT.ModOutVermaRelationSOld(false, this->theSubNthElementIsImageNthCoordSimpleBasis, theContext, theRingUnit);
}

template <class CoefficientType>
void ElementVermaModuleOrdered<CoefficientType>::MultiplyOnTheLeft
(const ElementSemisimpleLieAlgebra& other, ElementVermaModuleOrdered<CoefficientType>& output, const CoefficientType& theRingUnit, const CoefficientType& theRingZero, GlobalVariables* theContext)
{ ElementUniversalEnvelopingOrdered<CoefficientType> tempElt;
  tempElt.AssignElementLieAlgebra(other, theRingUnit, theRingZero, *this->theElT.owner);
  //std::cout << "<br>multiplying " << tempElt.ElementToString() << " times " << this->ElementToString();
  tempElt*=this->theElT;
  output.theElT=tempElt;
  //std::cout << "<br> ... and the result before simplifying is: " << output.theElT.ElementToString();
  output.theElT.Simplify(theContext, theRingUnit, theRingZero);
  //std::cout << "<br> before modding out we get: " << output.theElT.ElementToString();
  output.theElT.ModOutVermaRelationSOld(false, this->theSubNthElementIsImageNthCoordSimpleBasis, theContext, theRingUnit);
  //std::cout << "<br> finally we get: " << output.theElT.ElementToString();
}

template <class CoefficientType>
std::string ElementVermaModuleOrdered<CoefficientType>::ElementToString(const FormatExpressions& theFormat)const
{ std::stringstream out;
  std::string tempS=MathRoutines::ElementToStringBrackets(this->theElT, theFormat);
  if (tempS.size()>1)
    out << "(";
  if (tempS!="1")
    out << tempS;
  if (tempS.size()>1)
    out << ")";
  if (tempS!="0")
    out << " v";
  return out.str();
}

template<class CoefficientType>
void ElementVermaModuleOrdered<CoefficientType>::ActOnMe
  (const ElementSemisimpleLieAlgebra& actingElt, ElementVermaModuleOrdered<CoefficientType>& output, SemisimpleLieAlgebra& owner,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero, GlobalVariables* theContext)const
{ ElementUniversalEnvelopingOrdered<CoefficientType> tempElt;
  tempElt.AssignElementLieAlgebra(actingElt, theRingUnit, theRingZero, *this->theElT.owner);
//  std::cout << "<br>" << actingElt.ElementToString() << " acts on " << this->ElementToString();
  tempElt.LieBracketOnTheRight(this->theElT, output.theElT);
  output.theElT.Simplify(theContext, theRingUnit, theRingZero);
  output.theSubNthElementIsImageNthCoordSimpleBasis=this->theSubNthElementIsImageNthCoordSimpleBasis;
//  std::cout << "<br>and the result before modding out is: " << output.ElementToString();
//  int numVars=output.theElt.GetNumVars();
  output.theElT.ModOutVermaRelationSOld(false, this->theSubNthElementIsImageNthCoordSimpleBasis, theContext, theRingUnit);
//  std::cout << "<br>and after modding out we get: " << output.ElementToString();
}



template<class CoefficientType>
template<class CoefficientTypeQuotientField>
void ElementUniversalEnvelopingOrdered<CoefficientType>::GetBasisFromSpanOfElements
  (List<ElementUniversalEnvelopingOrdered<CoefficientType> >& theElements,
   Vectors<CoefficientTypeQuotientField>& outputCoords,
   List<ElementUniversalEnvelopingOrdered<CoefficientType> >& outputTheBasis, const CoefficientTypeQuotientField& theFieldUnit, const CoefficientTypeQuotientField& theFieldZero,
   GlobalVariables& theGlobalVariables)
{ if (theElements.size==0)
    return;
  ElementUniversalEnvelopingOrdered<CoefficientType> outputCorrespondingMonomials;
  outputCorrespondingMonomials.MakeZero(*theElements.TheObjects[0].owner);
  Vectors<CoefficientTypeQuotientField> outputCoordsBeforeReduction;
  for (int i=0; i<theElements.size; i++)
    for (int j=0; j<theElements.TheObjects[i].size; j++)
      outputCorrespondingMonomials.AddNoRepetition(theElements.TheObjects[i].TheObjects[j]);
  outputCoordsBeforeReduction.SetSize(theElements.size);
  for (int i=0; i<theElements.size; i++)
  { Vector<CoefficientTypeQuotientField>& currentList=outputCoordsBeforeReduction.TheObjects[i];
    currentList.MakeZero(outputCorrespondingMonomials.size, theFieldZero);
    ElementUniversalEnvelopingOrdered<CoefficientType>& currentElt=theElements.TheObjects[i];
    for (int j=0; j<currentElt.size; j++)
    { MonomialUniversalEnvelopingOrdered<CoefficientType>& currentMon=currentElt.TheObjects[j];
      currentList.TheObjects[outputCorrespondingMonomials.GetIndex(currentMon)]=currentMon.Coefficient;
    }
  }
  outputTheBasis.size=0;
  outputTheBasis.Reserve(theElements.size);
  Vectors<CoefficientTypeQuotientField> basisCoordForm;
  basisCoordForm.Reserve(theElements.size);
  Selection selectedBasis;
  outputCoordsBeforeReduction.ComputeDebugString();
  outputCoordsBeforeReduction.SelectABasis(basisCoordForm, theFieldZero, selectedBasis, theGlobalVariables);
  for (int i=0; i<selectedBasis.CardinalitySelection; i++)
    outputTheBasis.AddOnTop(theElements.TheObjects[selectedBasis.elements[i]]);
  Matrix<CoefficientType> bufferMat;
  Vectors<CoefficientType> bufferVectors;
  outputCoordsBeforeReduction.GetCoordsInBasis(basisCoordForm, outputCoords, bufferVectors, bufferMat, theFieldUnit, theFieldZero);
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::Simplify
(GlobalVariables* theContext,
 const CoefficientType& theRingUnit,  const CoefficientType& theRingZero)
{ ElementUniversalEnvelopingOrdered buffer;
  ElementUniversalEnvelopingOrdered output;
  //this->ComputeDebugString();
  output.MakeZero(*this->owner);
  for (int i=0; i<this->size; i++)
  { this->TheObjects[i].Simplify(buffer, theContext, theRingUnit, theRingZero);
    output+=buffer;
    //output.ComputeDebugString();
  }
  *this=output;
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::MakeZero(SemisimpleLieAlgebraOrdered& theOwner)
{ this->Clear();
  this->owner=&theOwner;
}


template <class CoefficientType>
void MonomialUniversalEnvelopingOrdered<CoefficientType>::Simplify
(ElementUniversalEnvelopingOrdered<CoefficientType>& output, GlobalVariables* theContext,
 const CoefficientType& theRingUnit,  const CoefficientType& theRingZero
 )
{ output.MakeZero(*this->owner);
  output.AddOnTop(*this);
  this->SimplifyAccumulateInOutputNoOutputInit(output, theContext, theRingUnit, theRingZero);
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::CleanUpZeroCoeff()
{ for (int i=0; i<this->size; i++)
    if (this->TheObjects[i].Coefficient.IsEqualToZero())
    { this->PopIndexSwapWithLast(i);
      i--;
    }
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::operator+=(const ElementUniversalEnvelopingOrdered<CoefficientType>& other)
{ this->Reserve(other.size);
  for (int i=0; i<other.size; i++)
    this->AddMonomialNoCleanUpZeroCoeff(other.TheObjects[i]);
  this->CleanUpZeroCoeff();
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::operator-=(const ElementUniversalEnvelopingOrdered<CoefficientType>& other)
{ this->Reserve(other.size);
  MonomialUniversalEnvelopingOrdered<CoefficientType> tempMon;
  for (int i=0; i<other.size; i++)
  { tempMon=other.TheObjects[i];
    tempMon.Coefficient*=-1;
    this->AddMonomialNoCleanUpZeroCoeff(tempMon);
  }
  this->CleanUpZeroCoeff();
}

template <class CoefficientType>
template <class otherType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::operator*=(const otherType& other)
{ if (other.IsEqualToZero())
  { this->MakeZero(*this->owner);
    return;
  }
  for (int i=0; i<this->size; i++)
    this->TheObjects[i].Coefficient.operator*=(other);
}

template <class CoefficientType>
template <class SecondType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::operator/=
(const SecondType& other)
{ for (int i=0; i<this->size; i++)
    this->TheObjects[i].Coefficient.operator/=(other);
}

template <class CoefficientType>
void MonomialUniversalEnvelopingOrdered<CoefficientType>::MultiplyByNoSimplify(const MonomialUniversalEnvelopingOrdered& other)
{ assert(this!=&other);
  this->generatorsIndices.Reserve(other.generatorsIndices.size+this->generatorsIndices.size);
  this->Powers.Reserve(other.generatorsIndices.size+this->generatorsIndices.size);
  this->Coefficient.MultiplyBy(other.Coefficient);
  if (other.generatorsIndices.size==0)
    return;
  int firstIndex=other.generatorsIndices.TheObjects[0];
  int i=0;
//  std::string tempS;
//  tempS=other.ElementToString();
  if (this->generatorsIndices.size>0)
    if (firstIndex==(*this->generatorsIndices.LastObject()))
    { //this->ComputeDebugString();
      *this->Powers.LastObject()+=other.Powers.TheObjects[0];
      i=1;
    }
  for (; i<other.generatorsIndices.size; i++)
  { this->Powers.AddOnTop(other.Powers.TheObjects[i]);
    this->generatorsIndices.AddOnTop(other.generatorsIndices.TheObjects[i]);
  }
}

template <class CoefficientType>
bool ElementUniversalEnvelopingOrdered<CoefficientType>::IsProportionalTo
 (const ElementUniversalEnvelopingOrdered<CoefficientType>& other, CoefficientType& outputTimesMeEqualsOther,
  const CoefficientType& theRingZero)const
{ if (this->IsEqualToZero())
  { if (other.IsEqualToZero())
      return true;
    return false;
  }
  if (other.IsEqualToZero())
  { outputTimesMeEqualsOther=theRingZero;
    return true;
  }
  if (other.size!=this->size)
    return false;
  MonomialUniversalEnvelopingOrdered<CoefficientType>& theMon= this->TheObjects[0];
  int theIndex=other.GetIndex(theMon);
  if (theIndex==-1)
    return false;
  MonomialUniversalEnvelopingOrdered<CoefficientType>& otherMon= other.TheObjects[theIndex];
  theMon.ComputeDebugString();
  otherMon.ComputeDebugString();
  outputTimesMeEqualsOther=otherMon.Coefficient;
  outputTimesMeEqualsOther/=theMon.Coefficient;
  ElementUniversalEnvelopingOrdered<CoefficientType> tempElt;

  tempElt=*this;
  tempElt*=outputTimesMeEqualsOther;
  tempElt-=other;
  return tempElt.IsEqualToZero();
}

template<class CoefficientType>
void MonomialUniversalEnvelopingOrdered<CoefficientType>::operator*=
(const MonomialUniversalEnvelopingOrdered& other)
{ assert(this!=&other);
  this->Coefficient*=other.Coefficient;
  for (int i=0; i<other.generatorsIndices.size; i++)
    this->MultiplyByGeneratorPowerOnTheRight(other.generatorsIndices.TheObjects[i], other.Powers.TheObjects[i]);
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::operator*=(const ElementUniversalEnvelopingOrdered<CoefficientType>& other)
{ ElementUniversalEnvelopingOrdered output;
  output.MakeZero(*this->owner);
  output.Reserve(this->size*other.size);
  MonomialUniversalEnvelopingOrdered<CoefficientType> tempMon;
  for (int i=0; i<this->size; i++)
    for(int j=0; j<other.size; j++)
    { tempMon=this->TheObjects[i];
      tempMon*=other.TheObjects[j];
      output.AddMonomial(tempMon);
    }
  *this=output;
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::AddMonomialNoCleanUpZeroCoeff
(const MonomialUniversalEnvelopingOrdered<CoefficientType>& input)
{ //MonomialUniversalEnvelopingOrdered<CoefficientType> tempMon;
  //tempMon=input;
  //tempMon.ComputeDebugString();
  //this->ComputeDebugString();
  int theIndex= this->GetIndex(input);
  if (theIndex==-1)
    this->AddOnTop(input);
  else
    this->TheObjects[theIndex].Coefficient+=input.Coefficient;
}

template <class CoefficientType>
void MonomialUniversalEnvelopingOrdered<CoefficientType>::SimplifyAccumulateInOutputNoOutputInit
(ElementUniversalEnvelopingOrdered<CoefficientType>& output, GlobalVariables* theContext,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero
 )
{ int IndexlowestNonSimplified=0;
  ElementUniversalEnvelopingOrdered<CoefficientType> buffer2;
  MonomialUniversalEnvelopingOrdered<CoefficientType> tempMon;
  //simplified order is descending order
  while (IndexlowestNonSimplified<output.size)
  { bool reductionOccurred=false;
    if (output.TheObjects[IndexlowestNonSimplified].Coefficient.IsEqualToZero())
      reductionOccurred=true;
    else
      for (int i=0; i<output.TheObjects[IndexlowestNonSimplified].generatorsIndices.size-1; i++)
        if (output.TheObjects[IndexlowestNonSimplified].generatorsIndices.TheObjects[i]>output.TheObjects[IndexlowestNonSimplified].generatorsIndices.TheObjects[i+1])
        { if (output.TheObjects[IndexlowestNonSimplified].SwitchConsecutiveIndicesIfTheyCommute
              (i, tempMon, theContext, theRingZero))
          { output.AddMonomialNoCleanUpZeroCoeff(tempMon);
//            tempMon.ComputeDebugString();
            reductionOccurred=true;
            break;
          }
          if (this->CommutingRightIndexAroundLeftIndexAllowed(output.TheObjects[IndexlowestNonSimplified].Powers.TheObjects[i], output.TheObjects[IndexlowestNonSimplified].generatorsIndices.TheObjects[i], output.TheObjects[IndexlowestNonSimplified].Powers.TheObjects[i+1], output.TheObjects[IndexlowestNonSimplified].generatorsIndices.TheObjects[i+1]))
          { output.TheObjects[IndexlowestNonSimplified].CommuteConsecutiveIndicesRightIndexAroundLeft(i, buffer2, theContext, theRingUnit, theRingZero);
            for (int j=0; j<buffer2.size; j++)
              output.AddMonomialNoCleanUpZeroCoeff(buffer2.TheObjects[j]);
//            output.ComputeDebugString();
            reductionOccurred=true;
            break;
          }
          if (this->CommutingLeftIndexAroundRightIndexAllowed(output.TheObjects[IndexlowestNonSimplified].Powers.TheObjects[i], output.TheObjects[IndexlowestNonSimplified].generatorsIndices.TheObjects[i], output.TheObjects[IndexlowestNonSimplified].Powers.TheObjects[i+1], output.TheObjects[IndexlowestNonSimplified].generatorsIndices.TheObjects[i+1]))
          { output.TheObjects[IndexlowestNonSimplified].CommuteConsecutiveIndicesLeftIndexAroundRight
            (i, buffer2, theContext, theRingUnit, theRingZero);
            for (int j=0; j<buffer2.size; j++)
              output.AddMonomialNoCleanUpZeroCoeff(buffer2.TheObjects[j]);
//            output.ComputeDebugString();
            reductionOccurred=true;
            break;
          }
        }
    if (reductionOccurred)
      output.PopIndexSwapWithLast(IndexlowestNonSimplified);
    else
      IndexlowestNonSimplified++;
//    output.ComputeDebugString();
  }
  output.CleanUpZeroCoeff();
}


template <class CoefficientType>
bool MonomialUniversalEnvelopingOrdered<CoefficientType>::SwitchConsecutiveIndicesIfTheyCommute
(int theLeftIndex, MonomialUniversalEnvelopingOrdered<CoefficientType>& output, GlobalVariables* theContext,
 const CoefficientType& theRingZero)
{ if (theLeftIndex>=this->generatorsIndices.size-1)
    return false;
  int theLeftGeneratorIndex=this->generatorsIndices.TheObjects[theLeftIndex];
  int theRightGeneratorIndex=this->generatorsIndices.TheObjects[theLeftIndex+1];
  ElementSemisimpleLieAlgebra tempElt;
  this->owner->theOwner.LieBracket(this->owner->theOrder.TheObjects[theLeftGeneratorIndex], this->owner->theOrder.TheObjects[theRightGeneratorIndex], tempElt);
  if (tempElt.IsEqualToZero())
  { output.generatorsIndices.Reserve(this->generatorsIndices.size);
    output.Powers.Reserve(this->generatorsIndices.size);
    output.MakeZero(theRingZero, *this->owner);
    output.Coefficient=this->Coefficient;
    //output.ComputeDebugString();
    for (int i=0; i<theLeftIndex; i++)
      output.MultiplyByGeneratorPowerOnTheRight(this->generatorsIndices.TheObjects[i], this->Powers.TheObjects[i]);
    output.MultiplyByGeneratorPowerOnTheRight(this->generatorsIndices.TheObjects[theLeftIndex+1], this->Powers.TheObjects[theLeftIndex+1]);
    output.MultiplyByGeneratorPowerOnTheRight(this->generatorsIndices.TheObjects[theLeftIndex], this->Powers.TheObjects[theLeftIndex]);
    for (int i=theLeftIndex+2; i<this->generatorsIndices.size; i++)
      output.MultiplyByGeneratorPowerOnTheRight(this->generatorsIndices.TheObjects[i], this->Powers.TheObjects[i]);
    return true;
  }
  return false;
}

template <class CoefficientType>
bool MonomialUniversalEnvelopingOrdered<CoefficientType>::CommutingRightIndexAroundLeftIndexAllowed(CoefficientType& theLeftPower, int leftGeneratorIndex, CoefficientType& theRightPower, int rightGeneratorIndex)
{ return this->CommutingLeftIndexAroundRightIndexAllowed(theRightPower, rightGeneratorIndex, theLeftPower, leftGeneratorIndex);
}

template <class CoefficientType>
void MonomialUniversalEnvelopingOrdered<CoefficientType>::CommuteConsecutiveIndicesRightIndexAroundLeft
(int theIndeX, ElementUniversalEnvelopingOrdered<CoefficientType>& output, GlobalVariables* theContext,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero
 )
{ if (theIndeX==this->generatorsIndices.size-1)
    return;
  output.MakeZero(*this->owner);
  MonomialUniversalEnvelopingOrdered tempMon;
  tempMon.MakeZero(theRingZero, *this->owner);
  tempMon.Powers.Reserve(this->generatorsIndices.size+2);
  tempMon.generatorsIndices.Reserve(this->generatorsIndices.size+2);
  tempMon.Powers.size=0;
  tempMon.generatorsIndices.size=0;
  int rightGeneratorIndeX= this->generatorsIndices.TheObjects[theIndeX+1];
  int leftGeneratorIndeX=this->generatorsIndices.TheObjects[theIndeX];
  CoefficientType theRightPoweR, theLeftPoweR;
  theRightPoweR= this->Powers.TheObjects[theIndeX+1];
  theLeftPoweR= this->Powers.TheObjects[theIndeX];
  theRightPoweR-=1;
  int powerDroP=0;
//  if (this->flagAnErrorHasOccurredTimeToPanic)
//  if (this->ElementToString()=="2f_{5}f_{-5}f_{-4}" || this->ElementToString()=="2f_{11}f_{-4}")
//  { std::cout << "here we are!";
//    this->flagAnErrorHasOccurredTimeToPanic=true;
//  }

  CoefficientType acquiredCoefficienT;
  acquiredCoefficienT=this->Coefficient;
  tempMon.Coefficient=this->Coefficient;
  for (int i=0; i<theIndeX; i++)
    tempMon.MultiplyByGeneratorPowerOnTheRight(this->generatorsIndices.TheObjects[i], this->Powers.TheObjects[i]);
  MonomialUniversalEnvelopingOrdered startMon;
  startMon=tempMon;
  ElementSemisimpleLieAlgebra adResulT, tempElT, theLeftElt;
  this->owner->AssignGeneratorCoeffOne(rightGeneratorIndeX, adResulT);
  this->owner->AssignGeneratorCoeffOne(leftGeneratorIndeX, theLeftElt);
  //tempLefttElt.ComputeDebugString(*this->owner, false, false);
  Vector<Rational>  theCoeffs;
  do
  { this->owner->GetLinearCombinationFrom(adResulT, theCoeffs);
    for (int i=0; i<theCoeffs.size; i++)
      if (theCoeffs.TheObjects[i]!=0)
      { int theNewGeneratorIndex=i;
        tempMon=startMon;
        if (this->flagAnErrorHasOccurredTimeToPanic)
        { tempMon.ComputeDebugString();
          this->ComputeDebugString();
        }
        tempMon.Coefficient=acquiredCoefficienT;
        if (this->flagAnErrorHasOccurredTimeToPanic)
        { tempMon.ComputeDebugString();
          this->ComputeDebugString();
        }
        tempMon.Coefficient*=(theCoeffs.TheObjects[i]);
        if (this->flagAnErrorHasOccurredTimeToPanic)
        { tempMon.ComputeDebugString();
          this->ComputeDebugString();
        }
        tempMon.MultiplyByGeneratorPowerOnTheRight(theNewGeneratorIndex, theRingUnit);
        tempMon.MultiplyByGeneratorPowerOnTheRight(leftGeneratorIndeX, theLeftPoweR);
        tempMon.MultiplyByGeneratorPowerOnTheRight(rightGeneratorIndeX, theRightPoweR);
        for (int i=theIndeX+2; i<this->generatorsIndices.size; i++)
          tempMon.MultiplyByGeneratorPowerOnTheRight(this->generatorsIndices.TheObjects[i], this->Powers.TheObjects[i]);
        if (this->flagAnErrorHasOccurredTimeToPanic)
        { tempMon.ComputeDebugString();
          this->ComputeDebugString();
        }
        output.AddOnTop(tempMon);
      }
    acquiredCoefficienT*=(theLeftPoweR);
    theLeftPoweR-=1;
    this->owner->theOwner.LieBracket(theLeftElt, adResulT, tempElT);
    adResulT=tempElT;
    powerDroP++;
    acquiredCoefficienT/=powerDroP;
//    if (this->flagAnErrorHasOccurredTimeToPanic)
//      adResulT.ComputeDebugString(false, false);
  }while(!adResulT.IsEqualToZero() && !acquiredCoefficienT.IsEqualToZero());
}

template <class CoefficientType>
void MonomialUniversalEnvelopingOrdered<CoefficientType>::CommuteConsecutiveIndicesLeftIndexAroundRight
(int theIndeX, ElementUniversalEnvelopingOrdered<CoefficientType>& output, GlobalVariables* theContext,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero
 )
{ if (theIndeX==this->generatorsIndices.size-1)
    return;
  output.MakeZero(*this->owner);
  MonomialUniversalEnvelopingOrdered tempMon;
  tempMon.MakeZero(theRingZero, *this->owner);
  tempMon.Powers.Reserve(this->generatorsIndices.size+2);
  tempMon.generatorsIndices.Reserve(this->generatorsIndices.size+2);
  tempMon.Powers.size=0;
  tempMon.generatorsIndices.size=0;
  int rightGeneratorIndex= this->generatorsIndices.TheObjects[theIndeX+1];
  int leftGeneratorIndex=this->generatorsIndices.TheObjects[theIndeX];
  CoefficientType theRightPower, theLeftPower;
  theRightPower= this->Powers.TheObjects[theIndeX+1];
  theLeftPower= this->Powers.TheObjects[theIndeX];
  theLeftPower-=1;
  int powerDrop=0;
 /* if (this->ElementToString()=="2f_{5}f_{-5}f_{-4}" || this->ElementToString()=="2f_{11}f_{-4}")
  { std::cout << "here we are!";
    this->flagAnErrorHasOccurredTimeToPanic=true;
  }*/

  CoefficientType acquiredCoefficient;
  acquiredCoefficient=(this->Coefficient);
  tempMon.Coefficient=this->Coefficient;
  for (int i=0; i<theIndeX; i++)
    tempMon.MultiplyByGeneratorPowerOnTheRight(this->generatorsIndices.TheObjects[i], this->Powers.TheObjects[i]);
  tempMon.MultiplyByGeneratorPowerOnTheRight(this->generatorsIndices.TheObjects[theIndeX], theLeftPower);
  MonomialUniversalEnvelopingOrdered startMon, tempMon2;
  startMon=tempMon;
  ElementSemisimpleLieAlgebra adResult, tempElt, tempRightElt;
  this->owner->AssignGeneratorCoeffOne(leftGeneratorIndex, adResult);
  this->owner->AssignGeneratorCoeffOne(rightGeneratorIndex, tempRightElt);
//  tempRightElt.ComputeDebugString(*this->owner, false, false);
  Vector<Rational>  theCoeffs;
  do
  { //acquiredCoefficient.ComputeDebugString();
    //theRightPower.ComputeDebugString();
    //adResult.ComputeDebugString(*this->owner, false, false);
    //tempMon.ComputeDebugString();
    //tempMon.ComputeDebugString();
    this->owner->GetLinearCombinationFrom(adResult, theCoeffs);
    for (int i=0; i<theCoeffs.size; i++)
      if(theCoeffs.TheObjects[i]!=0)
      { int theNewGeneratorIndex= i;
        tempMon=startMon;
        tempMon.MultiplyByGeneratorPowerOnTheRight(rightGeneratorIndex, theRightPower);
        tempMon.Coefficient=acquiredCoefficient;
        tempMon.Coefficient*=(theCoeffs.TheObjects[i]);
        tempMon.MultiplyByGeneratorPowerOnTheRight(theNewGeneratorIndex, theRingUnit);
        for (int i=theIndeX+2; i<this->generatorsIndices.size; i++)
          tempMon.MultiplyByGeneratorPowerOnTheRight(this->generatorsIndices.TheObjects[i], this->Powers.TheObjects[i]);
        output.AddOnTop(tempMon);
      }
    acquiredCoefficient*=(theRightPower);
    theRightPower-=1;
    this->owner->theOwner.LieBracket(adResult, tempRightElt, tempElt);
    adResult=tempElt;
    powerDrop++;
    acquiredCoefficient/=powerDrop;
    //adResult.ComputeDebugString(*this->owner, false, false);
  }while(!adResult.IsEqualToZero() && !acquiredCoefficient.IsEqualToZero());
}

template <class CoefficientType>
bool MonomialUniversalEnvelopingOrdered<CoefficientType>::CommutingLeftIndexAroundRightIndexAllowed(CoefficientType& theLeftPower, int leftGeneratorIndex, CoefficientType& theRightPower, int rightGeneratorIndex)
{ int tempInt;
  if (theLeftPower.IsSmallInteger(tempInt))
  { if(theRightPower.IsSmallInteger(tempInt))
      return true;
    int numPosRoots=this->owner->theOwner.theWeyl.RootsOfBorel.size;
    int theDimension= this->owner->theOwner.theWeyl.CartanSymmetric.NumRows;
    if(rightGeneratorIndex>= numPosRoots && rightGeneratorIndex<numPosRoots+theDimension)
    { ElementSemisimpleLieAlgebra tempElt;
      this->owner->theOwner.LieBracket(this->owner->theOrder.TheObjects[leftGeneratorIndex], this->owner->theOrder.TheObjects[rightGeneratorIndex], tempElt);
      if (tempElt.IsEqualToZero())
        return true;
      else
        return false;
    } else
      return true;
  }
  return false;
}

template <class CoefficientType>
void MonomialUniversalEnvelopingOrdered<CoefficientType>::MakeZero(int numVars, SemisimpleLieAlgebraOrdered& theOwner, GlobalVariables* theContext)
{ this->Coefficient.MakeZero((int)numVars, theContext);
  this->owner=&theOwner;
  this->generatorsIndices.size=0;
  this->Powers.size=0;
}

template <class Element>
void Matrix<Element>::ComputeDeterminantOverwriteMatrix(Element &output, const Element& theRingOne, const Element& theRingZero)
{ int tempI;
  output=theRingOne;
  Element tempRat;
  assert(this->NumCols==this->NumRows);
  int dim =this->NumCols;
  for (int i=0; i<dim; i++)
  {  //this->ComputeDebugString();
    tempI = this->FindPivot(i, i, dim-1);
    if (tempI==-1)
    { output=theRingZero;
      return;
    }
    this->SwitchTwoRows(i, tempI);
    if(tempI!=i){output.Minus(); }
    tempRat.Assign(this->elements[i][i]);
    output*=(tempRat);
    tempRat.Invert();
    this->RowTimesScalar(i, tempRat);
    for (int j=i+1; j<dim; j++)
      if (!this->elements[j][i].IsEqualToZero())
      { tempRat.Assign(this->elements[j][i]);
        tempRat.Minus();
        this->AddTwoRows (i, j, i, tempRat);
      }
  }
}

template<class CoefficientType>
void ModuleSSalgebraNew<CoefficientType>::SetNumVariables
(int GoalNumVars)
{ for (int i=0; i<this->actionsGeneratorsMaT.size; i++)
    this->actionsGeneratorsMaT[i].SetNumVariables(GoalNumVars);
  for (int i=0; i<this->actionsGeneratorS.size; i++)
    for (int j=0; j<this->actionsGeneratorS[i].size; j++)
      for (int k=0; k<this->actionsGeneratorS[i][j].size; k++)
        this->actionsGeneratorS[i][j][k].SetNumVariables(GoalNumVars);
  List<MonomialUniversalEnveloping<CoefficientType> > oldGeneratingWordsNonReduced;
  oldGeneratingWordsNonReduced.CopyFromBase(this->theGeneratingWordsNonReduced);
  this->theGeneratingWordsNonReduced.Clear();
  for (int i=0; i<oldGeneratingWordsNonReduced.size; i++)
  { oldGeneratingWordsNonReduced[i].SetNumVariables(GoalNumVars);
    this->theGeneratingWordsNonReduced.AddOnTop(oldGeneratingWordsNonReduced[i]);
  }
  for (int i=0; i<this->theGeneratingWordsGrouppedByWeight.size; i++)
    for (int j=0; j<this->theGeneratingWordsGrouppedByWeight[i].size; j++)
      this->theGeneratingWordsGrouppedByWeight[i][j].SetNumVariables(GoalNumVars);
  for (int i=0; i<this->theSimpleGens.size; i++)
    this->theSimpleGens[i].SetNumVariables(GoalNumVars);
  for (int i=0; i<this->actionsSimpleGens.size; i++)
    for (int j=0; j<this->actionsSimpleGens[i].size; j++)
      for (int k=0; k<this->actionsSimpleGens[i][j].size; k++)
        this->actionsSimpleGens[i][j][k].SetNumVariables(GoalNumVars);
  for (int i=0; i<this->actionsSimpleGensMatrixForM.size; i++)
  { this->actionsSimpleGensMatrixForM[i].SetNumVariables(GoalNumVars);
  }
  for (int i=0; i<this->theBilinearFormsAtEachWeightLevel.size; i++)
  { this->theBilinearFormsAtEachWeightLevel[i].SetNumVariables(GoalNumVars);
    this->theBilinearFormsInverted[i].SetNumVariables(GoalNumVars);
  }
  for (int i=0; i<this->theHWDualCoordsBaseField.size; i++)
  { this->theHWDualCoordsBaseField[i].SetNumVariables(GoalNumVars);
    this->theHWFundamentalCoordsBaseField[i].SetNumVariables(GoalNumVars);
  }
}

template<class Element>
void Matrix<Element>::Substitution(const PolynomialSubstitution<Rational>& theSub)
{ for (int i=0; i<this->NumRows; i++)
    for (int j=0; j<this->NumCols; j++)
      this->elements[i][j].Substitution(theSub);
}

template<class CoefficientType>
void ModuleSSalgebraNew<CoefficientType>::Substitution
(const PolynomialSubstitution<Rational>& theSub)
{ for (int i=0; i<this->actionsGeneratorsMaT.size; i++)
    this->actionsGeneratorsMaT[i].Substitution(theSub);
  for (int i=0; i<this->actionsGeneratorS.size; i++)
    for (int j=0; j<this->actionsGeneratorS[i].size; j++)
      for (int k=0; k<this->actionsGeneratorS[i][j].size; k++)
        this->actionsGeneratorS[i][j][k].Substitution(theSub);
  List<MonomialUniversalEnveloping<CoefficientType> > oldGeneratingWordsNonReduced;
  oldGeneratingWordsNonReduced.CopyFromBase(this->theGeneratingWordsNonReduced);
  this->theGeneratingWordsNonReduced.Clear();
  for (int i=0; i<oldGeneratingWordsNonReduced.size; i++)
  { oldGeneratingWordsNonReduced[i].Substitution(theSub);
    this->theGeneratingWordsNonReduced.AddOnTop(oldGeneratingWordsNonReduced[i]);
  }
  for (int i=0; i<this->theGeneratingWordsGrouppedByWeight.size; i++)
    for (int j=0; j<this->theGeneratingWordsGrouppedByWeight[i].size; j++)
      this->theGeneratingWordsGrouppedByWeight[i][j].Substitution(theSub);;
  for (int i=0; i<this->theSimpleGens.size; i++)
    this->theSimpleGens[i].Substitution(theSub);;
  for (int i=0; i<this->actionsSimpleGens.size; i++)
    for (int j=0; j<this->actionsSimpleGens[i].size; j++)
      for (int k=0; k<this->actionsSimpleGens[i][j].size; k++)
        this->actionsSimpleGens[i][j][k].Substitution(theSub);;
  for (int i=0; i<this->actionsSimpleGensMatrixForM.size; i++)
  { this->actionsSimpleGensMatrixForM[i].Substitution(theSub);;
  }
  for (int i=0; i<this->theBilinearFormsAtEachWeightLevel.size; i++)
  { this->theBilinearFormsAtEachWeightLevel[i].Substitution(theSub);;
    this->theBilinearFormsInverted[i].Substitution(theSub);;
  }
  for (int i=0; i<this->theHWDualCoordsBaseField.size; i++)
  { this->theHWDualCoordsBaseField[i].Substitution(theSub);;
    this->theHWFundamentalCoordsBaseField[i].Substitution(theSub);;
  }
}

template <class CoefficientType>
std::string MonomialTensorGeneralizedVermas<CoefficientType>::ElementToString
  (FormatExpressions* theFormat, bool includeV)const
{ std::stringstream out;
  std::string tempS;
  if (this->theMons.size>1)
    for (int i=0; i<this->theMons.size; i++)
      out << "(" << this->theMons[i].ElementToString(theFormat, includeV) << ")";
  else
    out << this->theMons[0].ElementToString(theFormat, includeV);
//  std::cout << "<br>" << out.str() << " has " << this->theMons.size << " multiplicands with hash functions: ";
//  for (int i=0; i<this->theMons.size; i++)
//    std::cout << this->theMons[i].HashFunction() << ", ";
  return out.str();
}

template <class CoefficientType>
std::string MonomialGeneralizedVerma<CoefficientType>::ElementToString
  (FormatExpressions* theFormat, bool includeV)const
{ ModuleSSalgebraNew<CoefficientType>& theMod=this->owneR->TheObjects[this->indexInOwner];
  std::string tempS;
  if (tempS=="1")
    tempS="";
  if (tempS=="-1")
    tempS="-";
  tempS+=this->theMonCoeffOne.ElementToString(theFormat);
  if (tempS=="1")
    tempS="";
  if (tempS=="-1")
    tempS="-";
  std::stringstream out;
  out << tempS;
  tempS= theMod.theGeneratingWordsNonReduced[this->indexFDVector].ElementToString(theFormat);
  if (tempS!="1")
    out << tempS;
  if (includeV)
    out << theMod.ElementToStringHWV(theFormat);
//  std::cout << "<br>MonomialP " << out.str() << " has indexInOwner " << this->indexInOwner;
  return out.str();
}

template <class CoefficientType>
std::string ElementSumGeneralizedVermas<CoefficientType>::ElementToString
  (FormatExpressions* theFormat)const
{ if (this->size==0)
    return "0";
  Vector<Rational>  parSel;
  std::stringstream out;
  std::string tempS;
  for (int i=0; i<this->size; i++)
  { MonomialGeneralizedVerma<CoefficientType>& currentMon=this->TheObjects[i];
    ModuleSSalgebraNew<CoefficientType>& theMod=currentMon.owneR->TheObjects[currentMon.indexInOwner];
    parSel= theMod.parabolicSelectionNonSelectedAreElementsLevi;
    tempS=currentMon.ElementToString(theFormat);
    if (tempS=="1")
      tempS="";
    if (tempS=="-1")
      tempS="-";
    if (i>0)
    { if (tempS.size()>0)
      { if (tempS[0]!='-')
          out << "+";
      } else
        out << "+";
    }
    out << tempS;
    tempS= theMod.theGeneratingWordsNonReduced[currentMon.indexFDVector].ElementToString(theFormat);
    if (tempS!="1")
      out << tempS;
  }
  return out.str();
}

template <class TemplateMonomial, class Element>
std::string MonomialCollection<TemplateMonomial, Element>::ElementToString
(FormatExpressions* theFormat)const
{ if (this->size==0)
    return "0";
  std::stringstream out;
  std::string tempS1, tempS2;
  List<TemplateMonomial> sortedMons;
  sortedMons=*this;
  sortedMons.QuickSortDescending();
  out << "(hash: " << this->HashFunction() << ")";
  for (int i=0; i<sortedMons.size; i++)
  { TemplateMonomial& currentMon=sortedMons[i];
    Element& currentCoeff=this->theCoeffs[this->GetIndex(currentMon)];
    if (currentCoeff.NeedsBrackets())
      tempS1="("+currentCoeff.ElementToString(theFormat)+ ")";
    else
      tempS1=currentCoeff.ElementToString(theFormat);
    tempS2=currentMon.ElementToString(theFormat);
    if (tempS1=="1" && tempS2!="1")
      tempS1="";
    if (tempS1=="-1"&& tempS2!="1")
      tempS1="-";
    if(tempS2!="1")
      tempS1+=tempS2;
    if (i>0)
    { if (tempS1.size()>0)
      { if (tempS1[0]!='-')
          out << "+";
      } else
        out << "+";
    }
    out << tempS1;
  }
  return out.str();
}

template <class CoefficientType>
std::string ElementTensorsGeneralizedVermas<CoefficientType>::ElementToString
  (FormatExpressions* theFormat)const
{ return this->::MonomialCollection<MonomialTensorGeneralizedVermas<CoefficientType>, CoefficientType>::ElementToString(theFormat);
}

template <class CoefficientType>
void MonomialUniversalEnveloping<CoefficientType>::SetNumVariables(int newNumVars)
{//the below code is wrong messed up with substitutions!
  //Left in comments to remind you of what you shouldnt do.
  // if (this->Coefficient.NumVars==newNumVars)
  //  return;
  for(int i=0; i<this->generatorsIndices.size; i++)
    this->Powers.TheObjects[i].SetNumVariablesSubDeletedVarsByOne(newNumVars);
}

template <class CoefficientType>
std::string MonomialUniversalEnveloping<CoefficientType>::ElementToString(FormatExpressions* theFormat)const
{ std::stringstream out;
  std::string tempS;
  if (this->owners==0 || this->indexInOwners==-1)
    return "(Error:Programming:NonInitializedMonomial)";
  if (this->generatorsIndices.size==0)
    return "1";
  for (int i=0; i<this->generatorsIndices.size; i++)
  { CoefficientType& thePower=this->Powers[i];
    int theIndex=this->generatorsIndices[i];
    tempS=this->GetOwner().GetStringFromChevalleyGenerator(theIndex, theFormat);
    //if (thePower>1)
    //  out << "(";
    out << tempS;
    if (!thePower.IsEqualToOne())
    { out << "^";
     // if (useRootIndices)
      out << "{";
      out << thePower.ElementToString(theFormat);
      //if (useRootIndices)
      out << "}";
    }
    //if (thePower>1)
    //  out << ")";
  }
  return out.str();
}

template <class CoefficientType>
void MonomialUniversalEnveloping<CoefficientType>::MakeZero
(int numVars, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners)
{ this->Coefficient.MakeZero(numVars);
  this->owners=&inputOwners;
  this->indexInOwners=inputIndexInOwners;
  this->generatorsIndices.size=0;
  this->Powers.size=0;
}

template <class CoefficientType>
int MonomialUniversalEnveloping<CoefficientType>::HashFunction() const
{ int top=MathRoutines::Minimum(SomeRandomPrimesSize, this->generatorsIndices.size);
  int result=0;
  for (int i=0; i<top; i++)
    result+=SomeRandomPrimes[i]*this->generatorsIndices.TheObjects[i];
  return result;
}

template <class CoefficientType>
bool ParserNode::GetRootRationalDontUseForFunctionArguments
(Vector<CoefficientType>& output, GlobalVariables& theGlobalVariables)
{ if (this->ExpressionType!=this->typeArray)
  { output.SetSize(1);
    if (!this->ConvertToType(this->typeRational, 0, theGlobalVariables))
      return false;
    output.TheObjects[0]=this->rationalValue;
    return true;
  }
  output.SetSize(this->children.size);
  for (int i=0; i<output.size; i++)
  { ParserNode& currentNode=this->owner->TheObjects[this->children.TheObjects[i]];
    if (!currentNode.ConvertToType(this->typeRational, this->impliedNumVars, theGlobalVariables))
      return false;
    output.TheObjects[i]=currentNode.rationalValue;
  }
  return true;
}

template <class CoefficientType>
bool ParserNode::GetListDontUseForFunctionArguments
(List<CoefficientType>& output, GlobalVariables& theGlobalVariables, int goalExpressionType, int theImpliedNumVars)
{ if (this->ExpressionType!=this->typeArray)
  { output.SetSize(1);
    if (!this->ConvertToType(goalExpressionType, theImpliedNumVars, theGlobalVariables))
      return false;
    output.TheObjects[0]=this->GetElement<CoefficientType>();
    return true;
  }
  output.SetSize(this->children.size);
  for (int i=0; i<output.size; i++)
  { ParserNode& currentNode=this->owner->TheObjects[this->children.TheObjects[i]];
    if (!currentNode.ConvertToType(goalExpressionType, theImpliedNumVars, theGlobalVariables))
      return false;
    output.TheObjects[i]=currentNode.GetElement<CoefficientType>();
  }
  return true;
}



template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::MakeCasimir
(SemisimpleLieAlgebra& theOwner, GlobalVariables& theGlobalVariables,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ //std::stringstream out;
  this->MakeZero(*theOwner.owner, theOwner.indexInOwner);
  WeylGroup& theWeyl= this->GetOwner().theWeyl;
  int theDimension=theWeyl.CartanSymmetric.NumRows;
  Vector<Rational>  tempRoot1, tempRoot2;
  Matrix<Rational>  killingForm;
  killingForm.init(theDimension, theDimension);
  for (int i=0; i<theDimension; i++)
  { tempRoot1.MakeEi(theDimension, i);
    for (int j=0; j<theDimension; j++)
    { killingForm.elements[i][j]=0;
      tempRoot2.MakeEi(theDimension, j);
      for (int k=0; k<theWeyl.RootSystem.size; k++)
        killingForm.elements[i][j]+= theWeyl.RootScalarCartanRoot(tempRoot1, theWeyl.RootSystem.TheObjects[k])* theWeyl.RootScalarCartanRoot(tempRoot2, theWeyl.RootSystem.TheObjects[k]);
    }
  }
//  std::cout << killingForm.ElementToString(true, false);
//  killingForm.Invert(theGlobalVariables);
//  killingForm.ComputeDebugString();
//  out << killingForm.ElementToString(true, false);
//  std::cout << killingForm.ElementToString(true, false);


  ElementUniversalEnveloping<CoefficientType> tempElt1, tempElt2;
//this code is to check a math formula:
//  ElementUniversalEnveloping checkElement;
//  checkElement.MakeZero(theOwner);
  Matrix<Rational>  invertedSymCartan;
  invertedSymCartan=theWeyl.CartanSymmetric;
  invertedSymCartan.Invert();
////////////////////////////////////////////////////////////////////////
  for (int i=0; i<theDimension; i++)
  { tempRoot1.MakeEi(theDimension, i);
  //implementation without the ninja formula:
//    killingForm.ActOnVectorColumn(tempRoot1, tempRoot2);
//    tempElt1.AssignElementCartan(tempRoot1, numVars, theOwner);
//    tempElt2.AssignElementCartan(tempRoot2, numVars, theOwner);
//    tempElt1*=tempElt2;
//    *this+=tempElt1;
// Alternative implementation using a ninja formula I cooked up after looking at the printouts:
    invertedSymCartan.ActOnVectorColumn(tempRoot1, tempRoot2);
    tempElt1.AssignElementCartan(tempRoot1, *this->owners, this->indexInOwners, theRingUnit, theRingZero);
    tempElt2.AssignElementCartan(tempRoot2, *this->owners, this->indexInOwners, theRingUnit, theRingZero);
    tempElt1*=tempElt2;
    *this+=tempElt1;
  }
  Rational tempRat;
  Vector<Rational>  theSum;
  for (int i=0; i<theWeyl.RootSystem.size; i++)
  { //Implementation without the ninja formula:
//    tempRat=0;
//    Vector<Rational> & theRoot=theWeyl.RootSystem.TheObjects[i];
//    int indexOfOpposite=theWeyl.RootSystem.GetIndex(-theRoot);
//    Vector<Rational> & theOpposite= theWeyl.RootSystem.TheObjects[indexOfOpposite];
//    for (int j=0; j<theWeyl.RootSystem.size; j++)
//    { Vector<Rational> & current=theWeyl.RootSystem.TheObjects[j];
//      if (current==theOpposite)
//        tempRat+=2;
//       else
//       { int indexOfSum=theWeyl.RootSystem.GetIndex(current+theRoot);
//         if (indexOfSum!=-1)
//           tempRat+=(theOwner.ChevalleyConstants.elements[i][j]*theOwner.ChevalleyConstants.elements[indexOfOpposite][indexOfSum]);
//       }
//     }
//     tempRat+=2;
//     tempRat= 1/tempRat;
//     tempElt2.MakeOneGeneratorCoeffOne(theOpposite, numVars, theOwner);
//     tempElt1.MakeOneGeneratorCoeffOne(theRoot, numVars, theOwner);
//     tempElt2*=tempElt1;
//
//     tempElt2*=tempRat;
//     *this+=tempElt2;
    //The ninja formula alternative implementation:
    Vector<Rational> & theRoot=theWeyl.RootSystem.TheObjects[i];
    tempElt2.MakeOneGeneratorCoeffOne(-theRoot, *theOwner.owner, theOwner.indexInOwner, theRingUnit, theRingZero);
    tempElt1.MakeOneGeneratorCoeffOne(theRoot, *theOwner.owner, theOwner.indexInOwner, theRingUnit, theRingZero);
    tempElt2*=tempElt1;
    tempElt2*=theWeyl.RootScalarCartanRoot(theWeyl.RootSystem[i], theWeyl.RootSystem[i])/2;
    *this+=tempElt2;
  }
  *this/=theWeyl.GetKillingDivTraceRatio();
// Check that the ninja formula is correct:
//  FormatExpressions tempPolyFormat;
//  tempPolyFormat.MakeAlphabetArbitraryWithIndex("g", "h");
//  std::cout << "Killing divided by trace ratio:" << theWeyl.GetKillingDivTraceRatio().ElementToString();
//  std::cout << "<br>casimir: " << this->ElementToString(false, false, theGlobalVariables, tempPolyFormat);
//  std::cout << "<br>check element: " << checkElement.ElementToString(false, false, theGlobalVariables, tempPolyFormat);

//  std::cout << "<br> check element minus casimir=" << checkElement.ElementToString(false, false, theGlobalVariables, tempPolyFormat);
  //this->DebugString=out.str();
//  Vector<Rational> tempRoot;
//  for (int i=0; i<theDimension; i++)
//  { tempRoot.MakeEi(theDimension, i);
//    if (!length1Explored)
//    { length1= theWeyl.RootScalarCartanRoot(tempRoot, tempRoot);
//      length1Explored=true;
//      coefficient1=0;
//      for (int j=0; j<theWeyl.RootsOfBorel.size; j++)
//      { coefficient1+=theWeyl.RootScalarCartanRoot(tempRoot, theWeyl.RootsOfBorel.TheObjects[j])*theWeyl.RootScalarCartanRoot(tempRoot, theWeyl.RootsOfBorel.TheObjects[j]);
//        coef
//      }
//    }
//  }
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::SubstitutionCoefficients
(PolynomialSubstitution<Rational>& theSub, GlobalVariables* theContext, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ ElementUniversalEnveloping<CoefficientType> endResult;
  MonomialUniversalEnveloping<CoefficientType> currentMon;
  endResult.MakeZero(*this->owners, this->indexInOwners);
  CoefficientType tempCF;
  for (int i=0; i<this->size; i++)
  { currentMon=this->TheObjects[i];
    this->theCoeffs[i].Substitution(theSub, this->theCoeffs[i].GetNumVars(), 1);
    endResult.AddMonomial(currentMon, tempCF);
  }
  if (theContext!=0)
    endResult.Simplify(*theContext, theRingUnit, theRingZero);
  else
  { GlobalVariables theGlobalVariables;
    endResult.Simplify(theGlobalVariables, theRingUnit, theRingZero);
  }
  this->operator=(endResult);
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::LieBracketOnTheRight
(const ElementUniversalEnveloping<CoefficientType>& right, ElementUniversalEnveloping<CoefficientType>& output)const
{ ElementUniversalEnveloping<CoefficientType> tempElt, tempElt2;
  tempElt=*this;
  tempElt*=right;
  tempElt2=right;
  tempElt2*=*this;
  output=tempElt;
  output-=tempElt2;
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::CleanUpZeroCoeff()
{ for (int i=0; i<this->size; i++)
    if (this->TheObjects[i].Coefficient.IsEqualToZero())
    { this->PopIndexSwapWithLast(i);
      i--;
    }
}

template <class CoefficientType>
void MonomialUniversalEnveloping<CoefficientType>::MultiplyByGeneratorPowerOnTheRight
(int theGeneratorIndex, const CoefficientType& thePower)
{ if (thePower.IsEqualToZero())
    return;
  if (this->generatorsIndices.size>0)
    if (*this->generatorsIndices.LastObject()==theGeneratorIndex)
    { this->Powers.LastObject()->operator+=(thePower);
      return;
    }
  this->Powers.AddOnTop(thePower);
  this->generatorsIndices.AddOnTop(theGeneratorIndex);
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::MakeOneGenerator
(int theIndex, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners, const CoefficientType& theRingUnit)
{ this->MakeZero(inputOwners, inputIndexInOwners);
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  tempMon.init(inputOwners, inputIndexInOwners);
  tempMon.MultiplyByGeneratorPowerOnTheRight(theIndex, theRingUnit);
  this->AddMonomial(tempMon, theRingUnit);
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::MakeOneGeneratorCoeffOne
(int theIndex, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ this->MakeZero(inputOwners, inputIndexInOwners);
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  tempMon.MakeZero(theRingZero, inputOwners, inputIndexInOwners);
  tempMon.MultiplyByGeneratorPowerOnTheRight(theIndex, theRingUnit);
  this->AddMonomial(tempMon, theRingUnit);
}

template <class CoefficientType>
bool ElementUniversalEnveloping<CoefficientType>::ConvertToLieAlgebraElementIfPossible
(ElementSemisimpleLieAlgebra& output)const
{ output.MakeZero(*this->owners, this->indexInOwners);
//  SemisimpleLieAlgebra& theOwner=this->owners->TheObjects[this->indexInOwners];
//  int numPosRoots=theOwner.theWeyl.RootsOfBorel.size;
  Rational tempRat=0;
  Polynomial<Rational>  tempP;
  ChevalleyGenerator tempChevalley;
  for (int i=0; i<this->size; i++)
  { MonomialUniversalEnveloping<CoefficientType>& tempMon= this->TheObjects[i];
    tempMon.GetDegree(tempP);
    if (!tempP.IsEqualToOne())
      return false;
    if (this->theCoeffs[i].TotalDegree()>0)
      return false;
    if (this->theCoeffs[i].size>0)
      tempRat=this->theCoeffs[i].theCoeffs[0];
    else
      tempRat=0;
    tempChevalley.MakeGenerator(*this->owners, this->indexInOwners, tempMon.generatorsIndices[0]);
    output.AddMonomial(tempChevalley, tempRat);
  }
  return true;
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::AssignElementLieAlgebra
(const ElementSemisimpleLieAlgebra& input, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ this->MakeZero(inputOwners, inputIndexInOwners);
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  tempMon.MakeZero(theRingZero, inputOwners, inputIndexInOwners);
  tempMon.generatorsIndices.SetSize(1);
  tempMon.Powers.SetSize(1);
  tempMon.Powers[0]=theRingUnit;
  CoefficientType tempCF;
  for (int i=0; i<input.size; i++)
  { int theIndex=input[i].theGeneratorIndex;
    tempCF=theRingUnit; //<- to facilitate implicit type conversion: theRingUnit does not have to be of type Rational
    tempCF*=input.theCoeffs[i];//<- to facilitate implicit type conversion: theRingUnit does not have to be of type Rational
    tempMon.generatorsIndices[0]=theIndex;
    this->AddMonomial(tempMon, tempCF);
  }
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::GetCoordinateFormOfSpanOfElements
  (int numVars, List<ElementUniversalEnveloping<CoefficientType> >& theElements,
   Vectors<CoefficientType>& outputCoordinates, ElementUniversalEnveloping<CoefficientType>& outputCorrespondingMonomials,
   GlobalVariables& theGlobalVariables)
{ if (theElements.size==0)
    return;
  outputCorrespondingMonomials.MakeZero(*theElements[0].owners, theElements[0].indexInOwners);
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  for (int i=0; i<theElements.size; i++)
    for (int j=0; j<theElements[i].size; j++)
      outputCorrespondingMonomials.AddNoRepetition(theElements[i][j]);
  outputCoordinates.SetSize(theElements.size);
  Polynomial<Rational>  ZeroPoly;
  ZeroPoly.MakeZero((int)numVars);
  for (int i=0; i<theElements.size; i++)
  { Vector<CoefficientType>& current=outputCoordinates[i];
    current.initFillInObject(outputCorrespondingMonomials.size, ZeroPoly);
    ElementUniversalEnveloping& currentElt=theElements[i];
    for (int j=0; j<currentElt.size; j++)
    { MonomialUniversalEnveloping<CoefficientType>& currentMon=currentElt.TheObjects[j];
      current.TheObjects[outputCorrespondingMonomials.GetIndex(currentMon)]=currentElt.theCoeffs[j];
    }
  }
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::AssignElementCartan
(const Vector<Rational> & input, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ MonomialUniversalEnveloping<CoefficientType> tempMon;
  this->MakeZero(inputOwners, inputIndexInOwners);
  tempMon.MakeZero(theRingZero, inputOwners, inputIndexInOwners);
  int theDimension= this->GetOwner().theWeyl.CartanSymmetric.NumRows;
  int numPosRoots=this->GetOwner().theWeyl.RootsOfBorel.size;
  tempMon.generatorsIndices.SetSize(1);
  tempMon.Powers.SetSize(1);
  CoefficientType tempCF;
  for (int i=0; i<theDimension; i++)
    if (!input.TheObjects[i].IsEqualToZero())
    { (*tempMon.generatorsIndices.LastObject())=i+numPosRoots;
      *tempMon.Powers.LastObject()=theRingUnit;
      tempCF=theRingUnit;               //<- to facilitate type conversion
      tempCF*=input.TheObjects[i]; //<- to facilitate type conversion we call extra assignment
      this->AddMonomial(tempMon, tempCF);
    }
}

template <class CoefficientType>
void MonomialUniversalEnveloping<CoefficientType>::Simplify
(ElementUniversalEnveloping<CoefficientType>& output, GlobalVariables& theGlobalVariables,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero )
{ output.MakeZero(*this->owners, this->indexInOwners);
  output.AddOnTop(*this);
  this->SimplifyAccumulateInOutputNoOutputInit(output, theGlobalVariables, theRingUnit, theRingZero);
}

template <class CoefficientType>
void MonomialUniversalEnveloping<CoefficientType>::MultiplyByNoSimplify
(const MonomialUniversalEnveloping<CoefficientType>& standsOnTheRight)
{ this->generatorsIndices.Reserve(standsOnTheRight.generatorsIndices.size+this->generatorsIndices.size);
  this->Powers.Reserve(standsOnTheRight.generatorsIndices.size+this->generatorsIndices.size);
  if (standsOnTheRight.generatorsIndices.size==0)
    return;
  int firstIndex=standsOnTheRight.generatorsIndices.TheObjects[0];
  int i=0;
  if (this->generatorsIndices.size>0)
    if (firstIndex==(*this->generatorsIndices.LastObject()))
    { *this->Powers.LastObject()+=standsOnTheRight.Powers.TheObjects[0];
      i=1;
    }
  for (; i<standsOnTheRight.generatorsIndices.size; i++)
  { this->Powers.AddOnTop(standsOnTheRight.Powers.TheObjects[i]);
    this->generatorsIndices.AddOnTop(standsOnTheRight.generatorsIndices.TheObjects[i]);
  }
}

template <class CoefficientType>
bool ElementUniversalEnvelopingOrdered<CoefficientType>::GetElementUniversalEnveloping
  (ElementUniversalEnveloping<CoefficientType>& output, SemisimpleLieAlgebra& owner)
{ ElementUniversalEnveloping<CoefficientType> Accum, tempElt;
  Accum.MakeZero(*owner.owner, owner.indexInOwner);
  for (int i=0; i<this->size; i++)
    if (!this->TheObjects[i].GetElementUniversalEnveloping(tempElt, owner))
      return false;
    else
      Accum+=tempElt;
  output=Accum;
  return true;
}

template <class CoefficientType>
bool MonomialUniversalEnvelopingOrdered<CoefficientType>::GetElementUniversalEnveloping
  (ElementUniversalEnveloping<CoefficientType>& output, SemisimpleLieAlgebra& owner)
{ ElementUniversalEnveloping<CoefficientType> Accum;
  ElementUniversalEnveloping<CoefficientType> tempMon;
  int theIndex;
  int theDegree;
  Accum.MakeConst(this->Coefficient, *owner.owner, owner.indexInOwner);
  for (int i=0; i<this->generatorsIndices.size; i++)
    if (this->Powers[i].IsSmallInteger(theDegree))
    { tempMon.AssignElementLieAlgebra
      (this->owner->theOrder[this->generatorsIndices[i]], *owner.owner, owner.indexInOwner,
        this->Coefficient.GetOne(), this->Coefficient.GetZero());
      tempMon.RaiseToPower(theDegree);
      Accum.MultiplyBy(tempMon);
    } else
      if (this->owner->theOrder.TheObjects[this->generatorsIndices.TheObjects[i]].IsACoeffOneChevalleyGenerator(theIndex, owner))
      { tempMon.MakeOneGeneratorCoeffOne
        (theIndex, *owner.owner, owner.indexInOwner, this->Coefficient.GetOne(), this->Coefficient.GetZero());
        tempMon.TheObjects[0].Powers.TheObjects[0]=this->Powers.TheObjects[i];
        Accum.MultiplyBy(tempMon);
      } else
        return false;
  output.operator=(Accum);
  return true;
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::SetNumVariables(int newNumVars)
{ //this->ComputeDebugString();
  if (this->size==0)
    return;
  int currentNumVars=this->TheObjects[0].Coefficient.NumVars;
  if (currentNumVars==newNumVars)
    return;
  ElementUniversalEnvelopingOrdered Accum;
  Accum.MakeZero(*this->owner);
  MonomialUniversalEnvelopingOrdered<CoefficientType> tempMon;
  for (int i=0; i<this->size; i++)
  { tempMon=this->TheObjects[i];
    tempMon.SetNumVariables(newNumVars);
    Accum.AddMonomial(tempMon);
  }
//  Accum.ComputeDebugString();
  this->operator=(Accum);
 // this->ComputeDebugString();
}

template <class CoefficientType>
void MonomialUniversalEnvelopingOrdered<CoefficientType>::SetNumVariables(int newNumVars)
{ //the below commented out code causes problems in substitution code!
  //if (this->Coefficient.NumVars==newNumVars)
  //  return;
  this->Coefficient.SetNumVariablesSubDeletedVarsByOne((int)newNumVars);
  for(int i=0; i<this->generatorsIndices.size; i++)
    this->Powers.TheObjects[i].SetNumVariablesSubDeletedVarsByOne((int)newNumVars);
}

template <class CoefficientType>
bool ElementUniversalEnvelopingOrdered<CoefficientType>::GetCoordsInBasis
  (List<ElementUniversalEnvelopingOrdered<CoefficientType> >& theBasis, Vector<CoefficientType>& output,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero, GlobalVariables& theGlobalVariables)const
{ List<ElementUniversalEnvelopingOrdered<CoefficientType> > tempBasis, tempElts;
  tempBasis=theBasis;
  tempBasis.AddOnTop(*this);
  Vectors<CoefficientType> tempCoords;
  this->GetBasisFromSpanOfElements(tempBasis, tempCoords, tempElts, theRingUnit, theRingZero, theGlobalVariables);
  Vector<CoefficientType> tempRoot;
  tempRoot=*tempCoords.LastObject();
  tempCoords.SetSize(theBasis.size);
  return tempRoot.GetCoordsInBasiS(tempCoords, output, theRingUnit, theRingZero);
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::GetCoordinateFormOfSpanOfElements
(int numVars, List<ElementUniversalEnvelopingOrdered>& theElements, Vectors<Polynomial<CoefficientType> >& outputCoordinates,
 ElementUniversalEnvelopingOrdered& outputCorrespondingMonomials, GlobalVariables& theGlobalVariables)
{ if (theElements.size==0)
    return;
  outputCorrespondingMonomials.MakeZero(*theElements.TheObjects[0].owner);
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  for (int i=0; i<theElements.size; i++)
    for (int j=0; j<theElements.TheObjects[i].size; j++)
      outputCorrespondingMonomials.AddNoRepetition(theElements.TheObjects[i].TheObjects[j]);
  outputCoordinates.SetSize(theElements.size);
  Polynomial<Rational>  ZeroPoly;
  ZeroPoly.MakeZero((int)numVars);
  for (int i=0; i<theElements.size; i++)
  { Vector<Polynomial<CoefficientType> >& current=outputCoordinates[i];
    current.initFillInObject(outputCorrespondingMonomials.size, ZeroPoly);
    ElementUniversalEnvelopingOrdered& currentElt=theElements[i];
    for (int j=0; j<currentElt.size; j++)
    { MonomialUniversalEnvelopingOrdered<CoefficientType>& currentMon=currentElt.TheObjects[j];
      current.TheObjects[outputCorrespondingMonomials.GetIndex(currentMon)]=currentMon.Coefficient;
    }
  }
}

template <class CoefficientType>
void MonomialUniversalEnvelopingOrdered<CoefficientType>::SubstitutionCoefficients
(PolynomialSubstitution<Rational>& theSub)
{ if (theSub.size<1)
    return;
  this->Coefficient.Substitution(theSub, theSub[0].NumVars);
  this->SetNumVariables(theSub[0].NumVars);
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::SubstitutionCoefficients
(PolynomialSubstitution<Rational>& theSub, GlobalVariables* theContext)
{ ElementUniversalEnvelopingOrdered<CoefficientType> endResult;
  MonomialUniversalEnvelopingOrdered<CoefficientType> currentMon;
  endResult.MakeZero(*this->owner);
  for (int i=0; i<this->size; i++)
  { currentMon=this->TheObjects[i];
    currentMon.SubstitutionCoefficients(theSub);
    endResult.AddMonomial(currentMon);
  }
//  endResult.Simplify(theContext);
  this->operator=(endResult);
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::MakeConst
(const Rational& coeff, int numVars, List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners)
{ MonomialUniversalEnveloping<CoefficientType> tempMon;
  this->MakeZero(inputOwners, inputIndexInOwners);
  if (coeff.IsEqualToZero())
    return;
  Polynomial<Rational>  tempP;
  tempP.MakeConst((int)numVars, coeff);
  tempMon.MakeConst(inputOwners, inputIndexInOwners);
  this->AddMonomial(tempMon, tempP);
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::MakeZero(List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners)
{ this->::MonomialCollection<MonomialUniversalEnveloping<CoefficientType>, CoefficientType >::MakeZero();
  this->owners=&inputOwners;
  this->indexInOwners=inputIndexInOwners;
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::MultiplyBy
  (const MonomialUniversalEnveloping<CoefficientType>& standsOnTheRight, const CoefficientType& theCoeff)
{ if (standsOnTheRight.generatorsIndices.size==0)
    return;
  ElementUniversalEnveloping<CoefficientType> output;
  output.SetExpectedSize(this->size);
  output.MakeZero(*this->owners, this->indexInOwners);
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  CoefficientType newCoeff;
  for (int i=0; i<this->size; i++)
  { tempMon=this->TheObjects[i];
    tempMon*=standsOnTheRight;
    newCoeff=this->theCoeffs[i];
    newCoeff*=theCoeff;
    output.AddMonomial(tempMon, newCoeff);
  }
  (*this)=output;
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::operator*=(const ElementUniversalEnveloping& standsOnTheRight)
{ ElementUniversalEnveloping output;
  output.MakeZero(*this->owners, this->indexInOwners);
  output.SetExpectedSize(standsOnTheRight.size*this->size);
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  int sizeOriginal=0;
  CoefficientType powerOriginal, CoeffOriginal;
  for (int i=0; i<this->size; i++)
  { tempMon=this->TheObjects[i];
    sizeOriginal=tempMon.generatorsIndices.size;
    if (sizeOriginal>0)
      powerOriginal=*tempMon.Powers.LastObject();
    for(int j=0; j<standsOnTheRight.size; j++)
    { tempMon.generatorsIndices.size=sizeOriginal;
      tempMon.Powers.size=sizeOriginal;
      if (sizeOriginal>0)
        *tempMon.Powers.LastObject()=powerOriginal;
      CoeffOriginal=this->theCoeffs[i];
      CoeffOriginal*=standsOnTheRight.theCoeffs[j];
      tempMon.MultiplyByNoSimplify(standsOnTheRight.TheObjects[j]);
      //tempMon.ComputeDebugString();
      output.AddMonomial(tempMon, CoeffOriginal);
    }
  }
  *this=output;
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::SetNumVariables(int newNumVars)
{ //this->ComputeDebugString();
  if (this->size==0)
    return;
  int currentNumVars=this->theCoeffs[0].NumVars;
  if (currentNumVars==newNumVars)
    return;
  ElementUniversalEnveloping<CoefficientType> Accum;
  Accum.MakeZero(*this->owners, this->indexInOwners);
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  CoefficientType tempCoeff;
  for (int i=0; i<this->size; i++)
  { tempMon=this->TheObjects[i];
    tempMon.SetNumVariables(newNumVars);
    tempCoeff=this->theCoeffs[i];
    tempCoeff.SetNumVariables(newNumVars);
    Accum.AddMonomial(tempMon, tempCoeff);
  }
//  Accum.ComputeDebugString();
  this->operator=(Accum);
 // this->ComputeDebugString();
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::RaiseToPower(int thePower)
{ ElementUniversalEnveloping<CoefficientType> buffer;
  buffer=*this;
  if (this->size==0)
    return;
  this->MakeConst(this->theCoeffs[0].GetOne(), *this->owners, this->indexInOwners);
  for (int i=0; i<thePower; i++)
    this->operator*=(buffer);
}

template <class CoefficientType>
void MonomialUniversalEnvelopingOrdered<CoefficientType>::MakeZero(const CoefficientType& theRingZero, SemisimpleLieAlgebraOrdered& theOwner)
{ this->Coefficient=theRingZero;
  this->owner=&theOwner;
  this->generatorsIndices.size=0;
  this->Powers.size=0;
}

template <class CoefficientType>
int MonomialUniversalEnvelopingOrdered<CoefficientType>::HashFunction() const
{ int top=MathRoutines::Minimum(SomeRandomPrimesSize, this->generatorsIndices.size);
  int result=0;
  for (int i=0; i<top; i++)
    result+=SomeRandomPrimes[i]*this->generatorsIndices.TheObjects[i];
  return result;
}

template <class CoefficientType>
void MonomialUniversalEnvelopingOrdered<CoefficientType>::MultiplyByGeneratorPowerOnTheRight(int theGeneratorIndex, int thePower)
{ if (thePower==0)
    return;
  Polynomial<Rational>  tempP;
  tempP.MakeConst(this->Coefficient.NumVars, thePower);
  this->MultiplyByGeneratorPowerOnTheRight(theGeneratorIndex, tempP);
}

template <class CoefficientType>
void MonomialUniversalEnvelopingOrdered<CoefficientType>::MultiplyByGeneratorPowerOnTheRight(int theGeneratorIndex, const CoefficientType& thePower)
{ if (thePower.IsEqualToZero())
    return;
  if (this->generatorsIndices.size>0)
    if (*this->generatorsIndices.LastObject()==theGeneratorIndex)
    { (*this->Powers.LastObject())+=thePower;
      return;
    }
  this->Powers.AddOnTop(thePower);
  this->generatorsIndices.AddOnTop(theGeneratorIndex);
}


template <class CoefficientType>
std::string MonomialUniversalEnvelopingOrdered<CoefficientType>::ElementToString
(bool useLatex, bool useCalculatorFormat,
 FormatExpressions* PolyFormatLocal, GlobalVariables& theGlobalVariables)const
{ if (this->owner==0)
    return "faulty monomial non-initialized owner. Slap the programmer.";
  if (this->IsEqualToZero())
    return "0";
  std::stringstream out;
  std::string tempS;
  if (this->generatorsIndices.size>0)
  { tempS= MathRoutines::ElementToStringBrackets(this->Coefficient, PolyFormatLocal);
    if (tempS=="1")
      tempS="";
    if (tempS=="-1")
      tempS="-";
  } else
    tempS= this->Coefficient.ElementToString(PolyFormatLocal);
  out << tempS;
  for (int i=0; i<this->generatorsIndices.size; i++)
  { CoefficientType& thePower=this->Powers[i];
    int theIndex=this->generatorsIndices[i];
    bool usebrackets=false;
    tempS=this->owner->theOwner.GetStringFromChevalleyGenerator
    (theIndex, PolyFormatLocal)
    ;
    if (usebrackets)
      out << "(";
    out << tempS;
    if (usebrackets)
      out << ")";
    tempS=thePower.ElementToString(PolyFormatLocal);
    if (tempS!="1")
    { out << "^";
     // if (useLatex)
      out << "{";
      out << tempS;
      //if (useLatex)
      out << "}";
    }
    //if (thePower>1)
    //  out << ")";
  }
  return out.str();
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::ElementToString
(std::string& output, bool useLatex,
 bool useCalculatorFormat, FormatExpressions& PolyFormatLocal, GlobalVariables& theGlobalVariables)const
{ std::stringstream out;
  std::string tempS;
  if (this->size==0)
    out << "0";
  int IndexCharAtLastLineBreak=0;
  for (int i=0; i<this->size; i++)
  { MonomialUniversalEnvelopingOrdered<CoefficientType>& current=this->TheObjects[i];
    tempS=current.ElementToString(false, useCalculatorFormat, &PolyFormatLocal, theGlobalVariables);
    if (i!=0)
      if (tempS.size()>0)
        if (tempS[0]!='-')
          out << '+';
    out << tempS;
    if (((int)out.tellp())- IndexCharAtLastLineBreak>150)
    { IndexCharAtLastLineBreak=out.tellp();
      out << "\\\\&&";
    }
  }
  output=out.str();
}

template <class CoefficientType>
bool ElementUniversalEnvelopingOrdered<CoefficientType>::AssignElementUniversalEnveloping
  (ElementUniversalEnveloping<CoefficientType>& input, SemisimpleLieAlgebraOrdered& owner,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero, GlobalVariables* theContext)
{ ElementUniversalEnvelopingOrdered<CoefficientType> tempElt;
  this->MakeZero(owner);
  for (int i=0; i<input.size; i++)
  { if(!tempElt.AssignMonomialUniversalEnveloping
       (input.TheObjects[i], input.theCoeffs[i], owner, theRingUnit, theRingZero, theContext))
      return false;
    this->operator+=(tempElt);
  }
  this->Simplify(theContext, theRingUnit, theRingZero);
  return true;
}

template <class CoefficientType>
bool ElementUniversalEnvelopingOrdered<CoefficientType>::AssignMonomialUniversalEnveloping
  (MonomialUniversalEnveloping<CoefficientType>& input, const CoefficientType& inputCoeff, SemisimpleLieAlgebraOrdered& owner,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero, GlobalVariables* theContext)
{ ElementUniversalEnvelopingOrdered theMon;
  ElementSemisimpleLieAlgebra tempElt;
  CoefficientType theCoeff;
  theCoeff=inputCoeff;
  this->MakeConst(theCoeff, owner);
  //std::cout << "<br>after initialization with constant I am " << this->ElementToString();
  for (int i=0; i<input.generatorsIndices.size; i++)
  { int thePower;
    bool isASmallInt=input.Powers.TheObjects[i].IsSmallInteger(thePower);
    if (isASmallInt)
    { tempElt.AssignChevalleyGeneratorCoeffOneIndexNegativeRootspacesFirstThenCartanThenPositivE
      (input.generatorsIndices.TheObjects[i], *input.owners, input.indexInOwners);
      theMon.AssignElementLieAlgebra(tempElt, theRingUnit, theRingZero, owner);
      //std::cout << "<br>raising " << theMon.ElementToString() << " to power " << thePower;
      theMon.RaiseToPower(thePower, theRingUnit);
      //std::cout << " to obtain " << theMon.ElementToString();
    }
    else
      return false;
    this->MultiplyBy(theMon);

  }
  //this->Simplify(theContext);
  return true;
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::AssignElementLieAlgebra
(const ElementSemisimpleLieAlgebra& input, const CoefficientType& theRingUnit, const CoefficientType& theRingZero, SemisimpleLieAlgebraOrdered& theOwner)
{ this->MakeZero(theOwner);
  Vector<Rational>  ElementRootForm;
  input.ElementToVectorNegativeRootSpacesFirst(ElementRootForm);
  theOwner.ChevalleyGeneratorsInCurrentCoords.ActOnVectorColumn(ElementRootForm);
  MonomialUniversalEnvelopingOrdered<CoefficientType> tempMon;
  tempMon.MakeZero(theRingZero, theOwner);
  tempMon.generatorsIndices.SetSize(1);
  tempMon.Powers.SetSize(1);
  tempMon.Powers.TheObjects[0]=theRingUnit;
  for (int theIndex=0; theIndex<ElementRootForm.size; theIndex++)
    if ( ElementRootForm.TheObjects[theIndex]!=0)
    { tempMon.Coefficient=theRingUnit;
      tempMon.Coefficient*=ElementRootForm.TheObjects[theIndex];
      tempMon.generatorsIndices.TheObjects[0]=theIndex;
      this->AddOnTop(tempMon);
    }
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::RaiseToPower(int thePower, const CoefficientType& theRingUnit)
{ if (this->size==0)
    return;
  ElementUniversalEnvelopingOrdered<CoefficientType> buffer;
  buffer.operator=(*this);
  this->MakeConst(theRingUnit, *this->owner);
  //std::cout << "<br>raising " <<buffer.ElementToString() << " to power " << thePower;
  for (int i=0; i<thePower; i++)
    this->operator*=(buffer);
  //std::cout << "<br> and the result is " << this->ElementToString();
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::LieBracketOnTheRight
(const ElementSemisimpleLieAlgebra& right, const CoefficientType& ringUnit, const CoefficientType& ringZero)
{ ElementUniversalEnvelopingOrdered<CoefficientType> tempElt;
  tempElt.AssignElementLieAlgebra(right, ringUnit, ringZero, *this->owner);
  this->LieBracketOnTheRight(tempElt, *this);
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::LieBracketOnTheRight(const ElementUniversalEnvelopingOrdered& right, ElementUniversalEnvelopingOrdered& output)
{ ElementUniversalEnvelopingOrdered tempElt, tempElt2;
  tempElt=*this;
  tempElt*=right;
  tempElt2=right;
  tempElt2*=*this;
  output=tempElt;
  output-=tempElt2;
}

template<class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::AddMonomial(const MonomialUniversalEnvelopingOrdered<CoefficientType>& input)
{ if (input.IsEqualToZero())
    return;
  int theIndex= this->GetIndex(input);
  if (theIndex==-1)
    this->AddOnTop(input);
  else
  { this->TheObjects[theIndex].Coefficient+=input.Coefficient;
    if (this->TheObjects[theIndex].Coefficient.IsEqualToZero())
      this->PopIndexSwapWithLast(theIndex);
  }
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::ModOutVermaRelationSOld
  (bool SubHighestWeightWithZeroes, const PolynomialSubstitution<Rational>& highestWeightSub,
   GlobalVariables* theContext, const CoefficientType& theRingUnit)
{ MonomialUniversalEnvelopingOrdered<CoefficientType> tempMon;
  ElementUniversalEnvelopingOrdered<CoefficientType> output;
  output.MakeZero(*this->owner);
  for (int i=0; i<this->size; i++)
  { tempMon= this->TheObjects[i];
//    tempMon.ComputeDebugString();
    tempMon.ModOutVermaRelationSOld(SubHighestWeightWithZeroes, highestWeightSub, theContext, theRingUnit);
//    tempMon.ComputeDebugString();
    output.AddMonomial(tempMon);
  }
  this->operator=(output);
}

template <class Element>
void PolynomialSubstitution<Element>::MakeLinearSubConstTermsLastRow(Matrix<Element>& theMat)
{ this->SetSize(theMat.NumCols);
  MonomialP tempM;
  for (int i=0; i<this->size; i++)
  { this->TheObjects[i].MakeZero((int)theMat.NumRows-1);
    for (int j=0; j<theMat.NumRows-1; j++)
    { tempM.MakeZero((int)theMat.NumRows-1);
      tempM[j]=1;
      this->TheObjects[i].AddMonomial(tempM, theMat.elements[j][i]);
    }
    this->TheObjects[i]+=(theMat.elements[theMat.NumRows-1][i]);
  }
}

template <class CoefficientType>
void ElementTensorsGeneralizedVermas<CoefficientType>::Substitution
  (const PolynomialSubstitution<Rational>& theSub)
{ ElementTensorsGeneralizedVermas<CoefficientType> output;
  MonomialTensorGeneralizedVermas<CoefficientType> currentMon;
  output.MakeZero();
  CoefficientType tempCF;
  for (int i=0; i<this->size; i++)
  { currentMon=this->TheObjects[i];
    currentMon.Substitution(theSub);
    tempCF=this->theCoeffs[i];
    tempCF.Substitution(theSub);
    output.AddMonomial(currentMon, tempCF);
  }
  *this=output;
//  std::cout << "<hr>result: " << this->ElementToString(theGlobalVariables);
}

template <class CoefficientType>
void MonomialGeneralizedVerma<CoefficientType>::Substitution
(const PolynomialSubstitution<Rational>& theSub)
{ this->theMonCoeffOne.Substitution(theSub);
  ModuleSSalgebraNew<CoefficientType> newOwner;
  newOwner=this->owneR->TheObjects[this->indexInOwner];
  newOwner.Substitution(theSub);
  this->indexInOwner=this->owneR->AddNoRepetitionOrReturnIndexFirst(newOwner);
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::Substitution
(const PolynomialSubstitution<Rational>& theSub)
{ ElementUniversalEnveloping<CoefficientType> output;
  output.MakeZero(*this->owners, this->indexInOwners);
  MonomialUniversalEnveloping<CoefficientType> theMon;
  CoefficientType tempCF;
  for (int i=0; i<this->size; i++)
  { theMon=this->TheObjects[i];
    theMon.Substitution(theSub);
    tempCF=this->theCoeffs[i];
    tempCF.Substitution(theSub);
    output.AddMonomial(theMon, tempCF);
  }
  *this=output;
}

template <class CoefficientType>
void MonomialUniversalEnveloping<CoefficientType>::Substitution(const PolynomialSubstitution<Rational>& theSub)
{ for (int i=0; i<this->generatorsIndices.size; i++)
    this->Powers[i].Substitution(theSub);
  this->SimplifyEqualConsecutiveGenerators(0);
}

template <class CoefficientType>
void MonomialTensorGeneralizedVermas<CoefficientType>::operator=
(const MonomialGeneralizedVerma<CoefficientType>& other)
{ this->theMons.SetSize(1);
  this->theMons[0]=other;
}

template <class CoefficientType>
void ElementTensorsGeneralizedVermas<CoefficientType>::operator=(const ElementSumGeneralizedVermas<CoefficientType>& other)
{ this->MakeZero();
  MonomialTensorGeneralizedVermas<CoefficientType> theMon;
  for (int i=0; i<other.size; i++)
  { theMon=other[i];
    this->AddMonomial(theMon, other.theCoeffs[i]);
  }
}

template <class CoefficientType>
bool ElementTensorsGeneralizedVermas<CoefficientType>::MultiplyMeByUEEltOnTheLeft
  (const ElementUniversalEnveloping<CoefficientType>& theUE, ElementTensorsGeneralizedVermas<CoefficientType>& output,
   List<ModuleSSalgebraNew<CoefficientType> >& theOwner,
   GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)const
{ ElementTensorsGeneralizedVermas<CoefficientType> tempET;
  output.MakeZero();
  for (int i=0; i<theUE.size; i++)
  { if (!this->MultiplyMeByUEEltOnTheLeft(theUE[i], theUE.theCoeffs[i], tempET, theOwner, theGlobalVariables, theRingUnit, theRingZero))
    { ElementSumGeneralizedVermas<CoefficientType> tempOutput;
      for (int j=0; j<this->size; j++)
      { MonomialTensorGeneralizedVermas<CoefficientType> currentMon=this->TheObjects[j];
        if (currentMon.theMons.size!=1)
          return false;
        MonomialGeneralizedVerma<CoefficientType>& currentSingleMon=currentMon.theMons[0];
        if (j==0)
          tempOutput.MakeZero(*currentSingleMon.owneR);
        tempOutput.AddMonomial(currentSingleMon, this->theCoeffs[j]);
      }
      tempOutput.MultiplyMeByUEEltOnTheLeft(theUE, theGlobalVariables, theRingUnit, theRingZero);
      output=tempOutput;
      return true;
    }
    output+=tempET;
  }
  return true;
  //for (int i=0; i<theOwner.size; i++)
  //{ //std::cout << "<hr><hr>Module" << i+1 << "<br>" << theOwner[i].ElementToString();
  //}
}

template <class CoefficientType>
bool ElementTensorsGeneralizedVermas<CoefficientType>::MultiplyMeByUEEltOnTheLeft
(const MonomialUniversalEnveloping<CoefficientType>& theUE, const CoefficientType& theCoeff,
 ElementTensorsGeneralizedVermas<CoefficientType>& output,
 List<ModuleSSalgebraNew<CoefficientType> >& theOwner,
 GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)const
{ if (theCoeff.IsEqualToZero())
  { output.MakeZero();
    return true;
  }
  output=*this;
  output*=(theCoeff);
  for(int i=theUE.Powers.size-1; i>=0; i--)
  { int thePower;
    if (!theUE.Powers[i].IsSmallInteger(thePower))
      return false;
    int theIndex=theUE.generatorsIndices[i];
    for (int j=0; j<thePower; j++)
    //{ //std::cout <<"<hr>Acting by generator index " << theIndex << " on " << this->ElementToString();
      output.MultiplyMeByElementLieAlg(theOwner, theIndex, theGlobalVariables, theRingUnit, theRingZero);
      //std::cout <<"<br>to get: " << this->ElementToString();
    //}
  }
  return true;
}

template <class CoefficientType>
void ElementTensorsGeneralizedVermas<CoefficientType>::MultiplyMeByElementLieAlg
  (List<ModuleSSalgebraNew<CoefficientType> >& theOwner, int indexGenerator, GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ if (theOwner.size<=0)
    return;
  ElementTensorsGeneralizedVermas<CoefficientType> output;
  output.MakeZero();
  MonomialTensorGeneralizedVermas<CoefficientType> accumMon, monActedOn;
  ElementSumGeneralizedVermas<CoefficientType> tempElt;
  ElementUniversalEnveloping<CoefficientType> theGenerator;
  theGenerator.MakeOneGenerator
  (indexGenerator, *theOwner[0].theAlgebras, theOwner[0].indexAlgebra, theRingUnit);
  ;
//  FormatExpressions tempFormat;
//  tempFormat.MakeAlphabetArbitraryWithIndex("g", "h");
  CoefficientType currentCoeff;
  for (int i=0; i<this->size; i++)
  { MonomialTensorGeneralizedVermas<CoefficientType>& currentMon=this->TheObjects[i];
    accumMon.theMons.SetSize(0);
    for (int j=0; j<currentMon.theMons.size; j++)
    { tempElt.MakeZero(theOwner);
      tempElt.AddMonomial(currentMon.theMons[j], theRingUnit);
 //     std::cout << "<hr> Acting by " << theGenerator.ElementToString()
 //     << " on " << tempElt.ElementToString() << "<br>";
      tempElt.MultiplyMeByUEEltOnTheLeft(theGenerator, theGlobalVariables, theRingUnit, theRingZero);
//      std::cout << "<br> result: " << tempElt.ElementToString();
      for (int k=0; k<tempElt.size; k++)
      { currentCoeff=this->theCoeffs[i];
        currentCoeff*=tempElt.theCoeffs[k];
        monActedOn=accumMon;
        monActedOn*=(tempElt[k]);
        for (int l=j+1; l<currentMon.theMons.size; l++)
          monActedOn*=currentMon.theMons[l];
        output.AddMonomial(monActedOn, currentCoeff);
//        std::cout << "<br>accounted: " << monActedOn.ElementToString();
      }
      accumMon*=currentMon.theMons[j];
    }
  }
  *this=output;
}

template <class CoefficientType>
void MonomialGeneralizedVerma<CoefficientType>::MultiplyMeByUEEltOnTheLeft
(const CoefficientType& theCoeff, const ElementUniversalEnveloping<CoefficientType>& theUE, ElementSumGeneralizedVermas<CoefficientType>& outputAccum,
GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ MonomialGeneralizedVerma<CoefficientType> currentMon;
  CoefficientType currentCoeff;
  for (int j=0; j<theUE.size; j++)
  { currentMon.theMonCoeffOne=theUE[j];
    currentMon.theMonCoeffOne*=this->theMonCoeffOne;
    currentMon.owneR=this->owneR;
    currentMon.indexFDVector=this->indexFDVector;
    currentMon.indexInOwner=this->indexInOwner;
    currentCoeff=theCoeff;
    currentCoeff*=theUE.theCoeffs[j];
//    std::cout << "<hr>Applying " << theUE[j].ElementToString() << " on " << this->ElementToString();
    outputAccum.ReduceMonAndAddToMe(currentMon, currentCoeff, theGlobalVariables, theRingUnit, theRingZero);
  //  std::cout << "<br>and accummulating to obtain " << outputAccum.ElementToString() << "<hr>";
  }
//  std::cout << "<hr>result: " << this->ElementToString(theGlobalVariables);
}

template <class CoefficientType>
void ElementSumGeneralizedVermas<CoefficientType>::MultiplyMeByUEEltOnTheLeft
  (const ElementUniversalEnveloping<CoefficientType>& theUE, GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ ElementSumGeneralizedVermas<CoefficientType> buffer, Accum;
  MonomialGeneralizedVerma<CoefficientType> currentMon;
  Accum.MakeZero(*this->owneR);
  for (int i=0; i<this->size; i++)
  { //std::cout << "<hr>Multiplying " << this->TheObjects[i].ElementToString() << " by " << theUE.ElementToString();
    this->TheObjects[i].MultiplyMeByUEEltOnTheLeft(this->theCoeffs[i], theUE, buffer, theGlobalVariables, theRingUnit, theRingZero);
    //std::cout << "<br>to obtain " << buffer.ElementToString();
    Accum+=buffer;
  }
  *this=Accum;
//  std::cout << "<hr>result: " << this->ElementToString(theGlobalVariables);
}

template <class CoefficientType>
void MonomialGeneralizedVerma<CoefficientType>::ReduceMe
(const CoefficientType& myCoeff, ElementSumGeneralizedVermas<CoefficientType>& outputAccum, GlobalVariables& theGlobalVariables,
  const CoefficientType& theRingUnit, const CoefficientType& theRingZero)const
{ if (myCoeff.IsEqualToZero())
  { std::cout << "Warning: this is likely a programming error: if it is not, this message needs to be removed. "
    << " MonomialGeneralizedVerma<CoefficientType>::ReduceMe is called with a zero coefficient input. "
    << " At the moment I programmed this function, I did not foresee any uses of ReduceMe with zero coefficient input, "
    << " however I was not able to affirm that will never happen. "
    << "Hence I do not crash the calculator, but display this benign warning message.";
    return;
  }
  //std::cout << "<hr><hr>Reducing  " << this->ElementToString();
  ModuleSSalgebraNew<CoefficientType>& theMod=this->owneR->TheObjects[this->indexInOwner];
  theMod.GetOwner().OrderSetNilradicalNegativeMost(theMod.parabolicSelectionNonSelectedAreElementsLevi);
  //std::cout << "<br>";
  //for (int i=0; i<theMod.GetOwner().UEGeneratorOrderIncludingCartanElts.size; i++)
  //{ std::cout << "<br>generator index " << i << " has order " << theMod.GetOwner().UEGeneratorOrderIncludingCartanElts[i];
  //}
  ElementUniversalEnveloping<CoefficientType> theUEelt;
  theUEelt.MakeZero(*this->GetOwner().theAlgebras, this->GetOwner().indexAlgebra);
  theUEelt.AddMonomial(this->theMonCoeffOne, myCoeff);
  //std::cout << " <br>the UE elt before simplifying: " << theUEelt.ElementToString();
  theUEelt.Simplify(theGlobalVariables, theRingUnit, theRingZero);
  //std::cout << " <br>the UE elt after simplifying: " << theUEelt.ElementToString();
  MonomialUniversalEnveloping<CoefficientType> currentMon;
  MonomialGeneralizedVerma<CoefficientType> newMon;
  CoefficientType theCoeff;
  Matrix<CoefficientType> tempMat1, tempMat2;
//  std::cout << theMod.ElementToString();
  //std::cout << "<br>theMod.theModuleWeightsSimpleCoords.size: "
  //<< theMod.theModuleWeightsSimpleCoords.size;
  for (int l=0; l<theUEelt.size; l++)
  { currentMon=theUEelt[l];
    //std::cout << "<br> Processing monomial " << currentMon.ElementToString();
    tempMat1.MakeIdMatrix(theMod.theGeneratingWordsNonReduced.size, theRingUnit, theRingZero);
    for (int k=currentMon.Powers.size-1; k>=0; k--)
    { int thePower;
      if (!currentMon.Powers[k].IsSmallInteger(thePower))
        break;
      int theIndex=currentMon.generatorsIndices[k];
      tempMat2=theMod.GetActionGeneratorIndex(theIndex, theGlobalVariables, theRingUnit, theRingZero);
      //std::cout << "<hr>Action generator " << theIndex << ":<br>"
      //<< tempMat2.ElementToString();
      if (tempMat2.NumRows==0)
      { //if (theIndex>=theMod.GetOwner().GetRank()+theMod.GetOwner().GetNumPosRoots())
        //{ std::cout << "<br>Error! Accum: " << this->ElementToString();
        //return;
        //}
        break;
      }
      for (int s=0; s<thePower; s++)
        tempMat1.MultiplyOnTheLeft(tempMat2, theRingZero);
      currentMon.Powers.size--;
      currentMon.generatorsIndices.size--;
    }
//      std::cout << "<br> Action is the " << currentMon.ElementToString()
//      << " free action plus <br>"
//      << tempMat1.ElementToString();
    newMon.owneR=this->owneR;
    newMon.indexInOwner=this->indexInOwner;
//    CoefficientType newCoeff;
    for (int i=0; i<tempMat1.NumRows; i++)
      if (!tempMat1.elements[i][this->indexFDVector].IsEqualToZero())
      { newMon.theMonCoeffOne=currentMon;
        newMon.indexFDVector=i;
//        std::cout << "<br>adding to " << outputAccum.ElementToString() << " the monomial " << newMon.ElementToString() << " with coefficient "
//        << tempMat1.elements[i][this->indexFDVector].ElementToString() << " to obtain ";
        outputAccum.AddMonomial(newMon, tempMat1.elements[i][this->indexFDVector]);
//        std::cout << outputAccum.ElementToString();
      }
  }
//  std::cout << "<br>Matrix of the action: " << tempMat1.ElementToString();
//  std::cout << "<br> Accum: " << this->ElementToString();
  theMod.GetOwner().OrderSSLieAlgebraStandard();
}

template <class CoefficientType>
void ElementSumGeneralizedVermas<CoefficientType>::ReduceMonAndAddToMe
  (const MonomialGeneralizedVerma<CoefficientType>& theMon, const CoefficientType& theMonCoeff, GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ theMon.ReduceMe(theMonCoeff, *this, theGlobalVariables, theRingUnit, theRingZero);
}

template <class CoefficientType>
Matrix<CoefficientType>& ModuleSSalgebraNew<CoefficientType>::GetActionGeneratorIndex
(int generatorIndex, GlobalVariables& theGlobalVariables,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ int numGenerators=this->GetOwner().GetNumGenerators();
  int theDim=this->GetOwner().GetRank();
  int numPosRoots=this->GetOwner().GetNumPosRoots();
  assert(generatorIndex>=0 && generatorIndex<numGenerators);
  if (this->ComputedGeneratorActions.selected[generatorIndex])
  { //std::cout << "<br>generator index " << generatorIndex << " is precomputed: "
    //<< this->actionsGeneratorsMaT[generatorIndex].ElementToString(true, false);
    return this->actionsGeneratorsMaT[generatorIndex];
  }
  this->ComputedGeneratorActions.AddSelectionAppendNewIndex(generatorIndex);
  ElementUniversalEnveloping<CoefficientType> tempElt;
  Vector<Rational> theWeight;
  tempElt.MakeOneGenerator(generatorIndex,*this->theAlgebras, this->indexAlgebra, theRingUnit);
  theWeight.MakeZero(theDim);
  if (generatorIndex<numPosRoots)
    theWeight=this->GetOwner().theWeyl.RootSystem[generatorIndex];
  if(generatorIndex>=numPosRoots+theDim)
    theWeight=this->GetOwner().theWeyl.RootSystem[generatorIndex-theDim];
  for (int i=0; i<this->parabolicSelectionNonSelectedAreElementsLevi.CardinalitySelection; i++)
    if (theWeight[this->parabolicSelectionNonSelectedAreElementsLevi.elements[i]]!=0)
    { if (theWeight[this->parabolicSelectionNonSelectedAreElementsLevi.elements[i]]<0)
      { this->actionsGeneratorsMaT[generatorIndex].init(0,0);
//        std::cout << "<br>generator index " << generatorIndex << " has free action. ";
        return this->actionsGeneratorsMaT[generatorIndex];
      } else
      { this->actionsGeneratorsMaT[generatorIndex].init
        (this->theGeneratingWordsNonReduced.size,this->theGeneratingWordsNonReduced.size);
        this->actionsGeneratorsMaT[generatorIndex].NullifyAll(theRingZero);
//        std::cout << "<br>generator index " << generatorIndex << " has ZERO action. ";
        return this->actionsGeneratorsMaT[generatorIndex];
      }
    }

  this->GetMatrixHomogenousElt
  (tempElt, this->actionsGeneratorS[generatorIndex], theWeight,
   this->actionsGeneratorsMaT[generatorIndex], theGlobalVariables, theRingUnit, theRingZero);
//  std::cout << "<br>generator index " << generatorIndex << " has been computed to be: "
//  << this->actionsGeneratorsMaT[generatorIndex].ElementToString(true, false);
  return this->actionsGeneratorsMaT[generatorIndex];
}

template  <class CoefficientType>
void ModuleSSalgebraNew<CoefficientType>::
GetMatrixHomogenousElt
(ElementUniversalEnveloping<CoefficientType>& inputHomogeneous,
  List<List<ElementUniversalEnveloping<CoefficientType> > >& outputSortedByArgumentWeight,
  Vector<Rational>& weightUEEltSimpleCoords, Matrix<CoefficientType>& output,
  GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{//  std::cout << "<hr>status of the module @ start GetMatrixHomogenousElt" << this->ElementToString();
  this->GetAdActionHomogenousElT
  (inputHomogeneous, weightUEEltSimpleCoords, outputSortedByArgumentWeight, theGlobalVariables, theRingUnit, theRingZero)
  ;
  output.init(this->theGeneratingWordsNonReduced.size, this->theGeneratingWordsNonReduced.size);
  output.NullifyAll(theRingZero);
  for (int j=0; j<outputSortedByArgumentWeight.size; j++)
    for (int k=0; k<outputSortedByArgumentWeight[j].size; k++)
    { MonomialUniversalEnveloping<CoefficientType>& currentMon=this->theGeneratingWordsGrouppedByWeight[j][k];
      ElementUniversalEnveloping<CoefficientType>& imageCurrentMon=outputSortedByArgumentWeight[j][k];
      int indexColumn=this->theGeneratingWordsNonReduced.GetIndex(currentMon);
      assert(indexColumn!=-1);
      for (int l=0; l< imageCurrentMon.size; l++)
      { int indexRow=this->theGeneratingWordsNonReduced.GetIndex(imageCurrentMon[l]);
        assert(indexRow!=-1);
        output.elements[indexRow][indexColumn]=imageCurrentMon.theCoeffs[l];
//        std::cout <<"<br> Index row: " << indexRow << "; index column: " << indexColumn;
      }
    }
  if (!this->flagIsInitialized)
    return;
//  std::cout << "<hr><hr><hr><hr><hr>input GetMatrixHomogenousElt: " << inputHomogeneous.ElementToString();
//  std::cout << "<hr>status of the module @ GetMatrixHomogenousElt" << this->ElementToString();
//  std::cout << "<hr>output GetMatrixHomogenousElt: " << output.ElementToString(true, false);
}

template<class CoefficientType>
CoefficientType WeylGroup::WeylDimFormulaSimpleCoords(Vector<CoefficientType>& theWeightInSimpleCoords, const CoefficientType& theRingUnit)
{ CoefficientType Result, buffer;
  Vector<CoefficientType> rhoOverNewRing, rootOfBorelNewRing, sumWithRho;//<-to facilitate type conversion!
  rhoOverNewRing=this->rho;//<-type conversion here!
  Result=theRingUnit;
//  std::cout << "<br>doing the weyl dim formula with: " << theWeightInSimpleCoords.ElementToString();
//  std::cout << "<br>rho is:" << rhoOverNewRing.ElementToString();
//  std::cout << "<br>rho before conversion: " << this->rho.ElementToString();
//  std::cout << "<br>we get: ";
  for (int i=0; i<this->RootsOfBorel.size; i++)
  { rootOfBorelNewRing=this->RootsOfBorel[i]; //<-type conversion here!
    sumWithRho=rhoOverNewRing+theWeightInSimpleCoords;
    buffer=(this->RootScalarCartanRoot(sumWithRho, rootOfBorelNewRing));
    buffer/=this->RootScalarCartanRoot(rhoOverNewRing, rootOfBorelNewRing);
//    std::cout << "(" << buffer.ElementToString() << ")";
    Result*=buffer;
  }
  return Result;
}

template<class CoefficientType>
Vector<CoefficientType> WeylGroup::GetFundamentalCoordinatesFromSimple
(const Vector<CoefficientType>& inputInFundamentalCoords)
{ Matrix<Rational> & tempMat=*this->GetMatrixSimpleToFundamentalCoords();
  Vector<CoefficientType> result=inputInFundamentalCoords;
  tempMat.ActOnVectorColumn(result);
  return result;
}

template<class CoefficientType>
Vector<CoefficientType> WeylGroup::GetSimpleCoordinatesFromFundamental
(const Vector<CoefficientType>& inputInFundamentalCoords)
{ Matrix<Rational>& tempMat=*this->GetMatrixFundamentalToSimpleCoords();
  Vector<CoefficientType> result;
  result=inputInFundamentalCoords;
//  std::cout << "<br>transition matrix from fundamental to simple: " << tempMat.ElementToString();
  tempMat.ActOnVectorColumn(result, result[0].GetZero());
  return result;
}

template<class CoefficientType>
CoefficientType WeylGroup::WeylDimFormulaFundamentalCoords(Vector<CoefficientType>& weightFundCoords, const CoefficientType& theRingUnit)
{ Vector<CoefficientType> theWeightInSimpleCoords;
  theWeightInSimpleCoords = this->GetSimpleCoordinatesFromFundamental(weightFundCoords);
  return this->WeylDimFormulaSimpleCoords(theWeightInSimpleCoords);
}

template<class leftType, class rightType>
void WeylGroup::RootScalarCartanRoot(const Vector<leftType>& r1, const Vector<rightType>& r2, leftType& output)const
{ output=r1[0].GetZero();
  leftType buffer;
  for (int i=0; i<this->CartanSymmetric.NumRows; i++)
    for (int j=0; j<this->CartanSymmetric.NumCols; j++)
    { buffer=r1[i];
      buffer*=r2[j];
      buffer*=this->CartanSymmetric.elements[i][j];
      output+=(buffer);
    }
}

template<class CoefficientType>
void Vectors<CoefficientType>::IntersectTwoLinSpaces
  (const Vectors<CoefficientType>& firstSpace, const Vectors<CoefficientType>& secondSpace,
   Vectors<CoefficientType>& output, GlobalVariables& theGlobalVariables, const CoefficientType& theRingZero
   )
{ //std::cout << "<br>*****Debugging Intersection linear spaces: ";
  //std::cout << "<br>input first space: " << firstSpace.ElementToString();
  //std::cout << "<br>input second space: " << secondSpace.ElementToString();
  Vectors<CoefficientType> firstReduced, secondReduced;
  Selection tempSel;
  firstSpace.SelectABasis(firstReduced, theRingZero, tempSel, theGlobalVariables);
  secondSpace.SelectABasis(secondReduced, theRingZero, tempSel, theGlobalVariables);
  //std::cout << "<br>first selected basis: " << firstSpace.ElementToString();
  //std::cout << "<br>second selected basis: " << secondSpace.ElementToString();
  if (firstReduced.size==0 || secondReduced.size==0)
  { output.size=0;
    return;
  }
  int theDim=firstReduced.TheObjects[0].size;
  Matrix<CoefficientType> theMat;
  theMat.init(theDim, firstReduced.size+secondReduced.size);
  for (int i=0; i<theDim; i++)
  { for (int j=0; j<firstReduced.size; j++)
      theMat.elements[i][j]=firstReduced.TheObjects[j].TheObjects[i];
    for (int j=0; j<secondReduced.size; j++)
    { theMat.elements[i][firstReduced.size+j]=theRingZero;
      theMat.elements[i][firstReduced.size+j]-=secondReduced.TheObjects[j].TheObjects[i];
    }
  }
  Matrix<CoefficientType> matEmpty;
  //std::cout << "<br>The matrix before the gaussian elimination:" << theMat.ElementToString(true, false);
  theMat.GaussianEliminationByRows(matEmpty, tempSel);
  //std::cout << "<br>The matrix after the gaussian elimination:" << theMat.ElementToString(true, false);
  output.Reserve(tempSel.CardinalitySelection);
  output.size=0;
  Vector<CoefficientType> nextIntersection;
  for(int i=0; i<tempSel.CardinalitySelection; i++)
  { int currentIndex=tempSel.elements[i];
    //std::cout << "<br>current pivot index : " << currentIndex;
    assert(currentIndex>=firstReduced.size);
    nextIntersection.MakeZero(theDim, theRingZero);
    for (int j=0; j<firstReduced.size; j++)
      if (!tempSel.selected[j])
        nextIntersection+=firstReduced.TheObjects[j]*theMat.elements[j][currentIndex];
    output.AddOnTop(nextIntersection);
  }
  //std::cout << "<br> final output: " << output.ElementToString();
  //std::cout << "<br>******************End of debugging linear space intersections";
}

template <class CoefficientType>
bool ReflectionSubgroupWeylGroup::GetAlLDominantWeightsHWFDIM
(Vector<CoefficientType>& highestWeightSimpleCoords, HashedList<Vector<CoefficientType> >& outputWeightsSimpleCoords,
 int upperBoundDominantWeights, std::string& outputDetails, GlobalVariables& theGlobalVariables)
{ std::stringstream out;
  this->ComputeRootSubsystem();
//  double startTime=theGlobalVariables.GetElapsedSeconds();
//  std::cout << "<br>time elapsed: " << theGlobalVariables.GetElapsedSeconds()-startTime;
  Vector<CoefficientType> highestWeightTrue=highestWeightSimpleCoords;
  Vectors<Rational> basisEi;
  int theDim=this->AmbientWeyl.GetDim();
  basisEi.MakeEiBasis(theDim);
  this->RaiseToDominantWeight(highestWeightTrue);
  Vector<CoefficientType> highestWeightFundCoords=this->AmbientWeyl.GetFundamentalCoordinatesFromSimple(highestWeightTrue);
  int theTopHeightSimpleCoords=(int) highestWeightSimpleCoords.GetVectorRational().SumCoords().DoubleValue()+1;
//  int theTopHeightFundCoords=(int) highestWeightFundCoords.SumCoords().DoubleValue();
  if (theTopHeightSimpleCoords<0)
    theTopHeightSimpleCoords=0;
  List<HashedList<Vector<CoefficientType> > > outputWeightsByHeight;
  int topHeightRootSystem=this->AmbientWeyl.RootsOfBorel.LastObject()->SumCoords().NumShort;
  int topHeightRootSystemPlusOne=topHeightRootSystem+1;
  outputWeightsByHeight.SetSize(topHeightRootSystemPlusOne);
  int finalHashSize=100;
  for (int i=0; i<topHeightRootSystemPlusOne; i++)
    outputWeightsByHeight[i].SetHashSizE(finalHashSize);
  outputWeightsSimpleCoords.Clear();
  outputWeightsByHeight[0].AddOnTop(highestWeightTrue);
  int numTotalWeightsFound=0;
  Vector<CoefficientType> currentWeight, currentWeightRaisedToDominant;
//  std::cout << "<br>time spend before working cycle: " << theGlobalVariables.GetElapsedSeconds()-startTime;
  for (int lowestUnexploredHeightDiff=0; lowestUnexploredHeightDiff<=theTopHeightSimpleCoords;
  lowestUnexploredHeightDiff++)
  { //double startCycleTime=theGlobalVariables.GetElapsedSeconds();
    if (upperBoundDominantWeights>0 && numTotalWeightsFound>upperBoundDominantWeights)
      break;
    int bufferIndexShift=lowestUnexploredHeightDiff%topHeightRootSystemPlusOne;
    HashedList<Vector<CoefficientType> >& currentHashes=outputWeightsByHeight[bufferIndexShift];
    for (int lowest=0; lowest<currentHashes.size; lowest++)
      for (int i=0; i<this->RootsOfBorel.size; i++)
      { currentWeight=currentHashes[lowest];
        currentWeight-=this->RootsOfBorel[i];
        if (this->IsDominantWeight(currentWeight))
        { int currentIndexShift=this->RootsOfBorel[i].SumCoords().NumShort;
          currentIndexShift=(currentIndexShift+bufferIndexShift)%topHeightRootSystemPlusOne;
          if (outputWeightsByHeight[currentIndexShift].AddNoRepetition(currentWeight))
          { numTotalWeightsFound++;
            outputWeightsByHeight[currentIndexShift].AdjustHashes();
          }
        }
      }
//    std::cout << "<br>time spent before accounting at height level " << lowestUnexploredHeightDiff
//    << ": " << theGlobalVariables.GetElapsedSeconds()-startCycleTime;
//    std::cout << " Size of current level: " << currentHashes.size;
    outputWeightsSimpleCoords.AddOnTop(currentHashes);
//    std::cout << ". Time spent after accounting at height level " << lowestUnexploredHeightDiff
//    << ": " << theGlobalVariables.GetElapsedSeconds()-startCycleTime;
//    startCycleTime=theGlobalVariables.GetElapsedSeconds();
    outputWeightsSimpleCoords.AdjustHashes();
    currentHashes.Clear();
//    std::cout << ". Time spent clearing up buffer at height level " << lowestUnexploredHeightDiff
//    << ": " << theGlobalVariables.GetElapsedSeconds()-startCycleTime;
  }
  out << " Total number of dominant weights: " << outputWeightsSimpleCoords.size;
  if (numTotalWeightsFound>=upperBoundDominantWeights)
    out << "<hr>This message is generated either because the number of weights has "
    << "exceeded the hard-coded RAM memory limits, or because "
    << " a priori bound for the number of weights is WRONG. If the latter "
    << " is the case, make sure to send an angry email to the author(s).";
  outputDetails=out.str();
  //std::cout << "<hr><hr>Total time spent generating weights: " << -startTime+theGlobalVariables.GetElapsedSeconds();
  return (numTotalWeightsFound<=upperBoundDominantWeights);
}

template <class CoefficientType>
void ReflectionSubgroupWeylGroup::RaiseToDominantWeight(Vector<CoefficientType>& theWeight, int* sign, bool* stabilizerFound)
{ if (sign!=0)
    *sign=1;
  if (stabilizerFound!=0)
    *stabilizerFound=false;
  Rational theScalarProd;
//  int theDim=this->AmbientWeyl.GetDim();
  for (bool found = true; found; )
  { found=false;
    for (int i=0; i<this->simpleGenerators.size; i++)
    { if (! this->IsDominantWRTgenerator(theWeight, i))
      { found=true;
        this->AmbientWeyl.ReflectBetaWRTAlpha(this->simpleGenerators[i], theWeight, false, theWeight);
        if (sign!=0)
          *sign*=-1;
      }
      if (stabilizerFound!=0)
        if (theScalarProd.IsEqualToZero())
          *stabilizerFound=true;
    }
  }
//  std::cout << "<hr># simple reflections applied total: " << numTimesReflectionWasApplied;
}

template <class CoefficientType>
void WeylGroup::ReflectBetaWRTAlpha(Vector<Rational>& alpha, Vector<CoefficientType>& Beta, bool RhoAction, Vector<CoefficientType>& Output)
{ CoefficientType bufferCoeff, alphaShift, lengthA;
  Vector<CoefficientType> result;
  result=(Beta);
  alphaShift=Beta[0].GetZero();//<-the zero of CoefficientType is not known at compile time (think multivariate polynomials)
  lengthA=alphaShift;
  if (RhoAction)
    result+=(this->rho);//<-implicit type conversion here if CoefficientType is not Rational
  for (int i=0; i<this->CartanSymmetric.NumRows; i++)
    for (int j=0; j<this->CartanSymmetric.NumCols; j++)
    { bufferCoeff=(result[j]);
      bufferCoeff*=(alpha[i]);
      bufferCoeff*=(this->CartanSymmetric.elements[i][j]*(-2));
      alphaShift+=(bufferCoeff);
      bufferCoeff=(alpha[i]);
      bufferCoeff*=(alpha[j]);
      bufferCoeff*=(this->CartanSymmetric.elements[i][j]);
      lengthA+=(bufferCoeff);
    }
  alphaShift/=(lengthA);
  Output.SetSize(this->CartanSymmetric.NumRows);
  for (int i=0; i<this->CartanSymmetric.NumCols; i++)
  { bufferCoeff=(alphaShift);
    bufferCoeff*=(alpha[i]);
    bufferCoeff+=(result[i]);
    Output[i]=(bufferCoeff);
  }
  if (RhoAction)
    Output-=this->rho;
}

template <class CoefficientType>
std::string MonomialChar<CoefficientType>::ElementToString
  (FormatExpressions* theFormat)
{ std::stringstream out;
  bool useBrackets=false;
  if (useBrackets)
    out << "char{}(" << this->weightFundamentalCoords.ElementToStringLetterFormat("\\omega", false) << ")";
  else
    out << "V_{" << weightFundamentalCoords.ElementToStringLetterFormat("\\omega", false) << "}";
  return out.str();
}

template<class CoefficientType>
bool charSSAlgMod<CoefficientType>::DrawMe
(std::string& outputDetails, GlobalVariables& theGlobalVariables,
 DrawingVariables& theDrawingVars, int upperBoundWeights, bool useMults)
{ charSSAlgMod<CoefficientType> CharCartan;
  bool result= this->FreudenthalEvalMe
  (CharCartan, outputDetails, theGlobalVariables, upperBoundWeights);
  std::stringstream out;
  Vectors<Rational> currentOrbit;
  WeylGroup& theWeyl=this->listOwners->TheObjects[this->indexInOwners].theWeyl;
  theWeyl.DrawRootSystem(theDrawingVars, false, theGlobalVariables, true);
  int totalNumWeights=0;
  Vectors<CoefficientType> dominantWeightsNonHashed;
  HashedList<Vector<CoefficientType> > finalWeights;
  Vector<Rational> convertor;
  for (int i=0; i< CharCartan.size; i++)
  { MonomialChar<CoefficientType>& currentMon=CharCartan[i];
    dominantWeightsNonHashed.size=0;
    dominantWeightsNonHashed.AddOnTop
    (theWeyl.GetSimpleCoordinatesFromFundamental(currentMon.weightFundamentalCoords));
    bool isTrimmed=!theWeyl.GenerateOrbit
    (dominantWeightsNonHashed, false, finalWeights, false, 0,  0, upperBoundWeights);
    totalNumWeights+=finalWeights.size;
    if (isTrimmed || totalNumWeights>upperBoundWeights)
    { out << "Did not generate all weights of the module due to RAM limits. ";
      result=false;
      break;
    }
    for (int j=0; j<finalWeights.size; j++)
    { convertor=finalWeights[j].GetVectorRational();
      theDrawingVars.drawCircleAtVectorBuffer(convertor, 5, DrawingVariables::PenStyleNormal, CGI::RedGreenBlue(0,0,0));
      if (useMults)
        theDrawingVars.drawTextAtVectorBuffer
          (convertor, CharCartan.theCoeffs[i].ElementToString(), CGI::RedGreenBlue(0,0,0), theDrawingVars.PenStyleNormal, 0);
    }
  }
  out << "<br>Number of computed weights: " << totalNumWeights << ". ";
  if (result && totalNumWeights<upperBoundWeights)
    out << "<br>All weights were computed and are drawn. <br>";
  else
    out << "<br><b> Not all weights were computed. </b>";
  outputDetails=out.str();
//  outputWeights.CopyFromBase(finalWeights);
  return result;
}

template <class CoefficientType>
void charSSAlgMod<CoefficientType>::DrawMeAssumeCharIsOverCartan
(WeylGroup& actualAmbientWeyl, GlobalVariables& theGlobalVariables, DrawingVariables& theDrawingVars)
{ if (actualAmbientWeyl.GetDim()<2)
    return;
  Vector<CoefficientType> actualWeight;
  Vector<Rational> actualWeightRationalPart;

  actualAmbientWeyl.DrawRootSystem(theDrawingVars, true, theGlobalVariables, false, 0, false);
  for (int j=0; j<this->size; j++)
  { actualWeight=actualAmbientWeyl.GetSimpleCoordinatesFromFundamental(this->TheObjects[j].weightFundamentalCoords);
    actualWeightRationalPart=actualWeight.GetVectorRational(); // <-type conversion here!
    theDrawingVars.drawCircleAtVectorBuffer(actualWeightRationalPart, 5, DrawingVariables::PenStyleNormal, CGI::RedGreenBlue(0,0,0));
    theDrawingVars.drawTextAtVectorBuffer
    (actualWeightRationalPart, this->theCoeffs[j].ElementToString(), CGI::RedGreenBlue(0,0,0), theDrawingVars.PenStyleNormal, 0);
  }
}

template <class CoefficientType>
bool ReflectionSubgroupWeylGroup::GenerateOrbitReturnFalseIfTruncated
(const Vector<CoefficientType>& input, Vectors<CoefficientType>& outputOrbit, int UpperLimitNumElements)
{ HashedList<Vector<CoefficientType> > theOrbit;
  bool result = true;
  theOrbit.Clear();
  Vector<CoefficientType> tempRoot;
  theOrbit.AddOnTop(input);
  MemorySaving<Vectors<CoefficientType> >ExternalAutosOverAmbientField;
  for (int i=0; i<theOrbit.size; i++)
  { for (int j=0; j<this->simpleGenerators.size; j++)
    { this->AmbientWeyl.ReflectBetaWRTAlpha(this->simpleGenerators[j], theOrbit[i], false, tempRoot);
      int oldsize=theOrbit.size;
      std::string debugString=tempRoot.ElementToString();
      theOrbit.AddNoRepetition(tempRoot);
      if (oldsize<theOrbit.size)
        std::cout << "<br>" << debugString << " with hash " << tempRoot.HashFunction() << " added, ";
    }
    for (int j=1; j<this->ExternalAutomorphisms.size; j++)
    { ExternalAutosOverAmbientField.GetElement()=this->ExternalAutomorphisms[j];
      theOrbit[i].GetCoordsInBasiS(ExternalAutosOverAmbientField.GetElement(), tempRoot);
      theOrbit.AddNoRepetition(tempRoot);
    }
    if (UpperLimitNumElements>0)
      if (theOrbit.size>=UpperLimitNumElements)
      { result=false;
        break;
      }
  }
  outputOrbit.CopyFromBase(theOrbit);
  return result;
}

template <class CoefficientType>
bool ReflectionSubgroupWeylGroup::FreudenthalEvalIrrepIsWRTLeviPart
(Vector<CoefficientType>& inputHWfundamentalCoords, HashedList<Vector<CoefficientType> >& outputDominantWeightsSimpleCoords,
 List<CoefficientType>& outputMultsSimpleCoords, std::string& outputDetails,
 GlobalVariables& theGlobalVariables, int UpperBoundFreudenthal)
{ //double startTimer=theGlobalVariables.GetElapsedSeconds();
  this->ComputeRootSubsystem();
  Vectors<Rational> EiBasis;
  EiBasis.MakeEiBasis(this->AmbientWeyl.GetDim());
  List<bool> Explored;
  Vector<CoefficientType> hwSimpleCoords;
  hwSimpleCoords=this->AmbientWeyl.GetSimpleCoordinatesFromFundamental(inputHWfundamentalCoords);
  if (!this->GetAlLDominantWeightsHWFDIM
      (hwSimpleCoords, outputDominantWeightsSimpleCoords, UpperBoundFreudenthal, outputDetails, theGlobalVariables))
  { std::stringstream errorLog;
    errorLog << "Error: the number of dominant weights exceeded hard-coded limit of "
    << UpperBoundFreudenthal << ". Please check out whether LiE's implementation of the Freudenthal "
    << " formula can do your computation.";
    outputDetails=errorLog.str();
    return false;
  }
//  std::cout << "Highest weight: " << hwSimpleCoords.ElementToString() << "<br>Dominant weights wrt. levi part("
//  << outputDominantWeightsSimpleCoords.size << "):<br> ";
//  for (int i=0; i<outputDominantWeightsSimpleCoords.size; i++)
//    std::cout << "<br>" << outputDominantWeightsSimpleCoords[i].ElementToString();
  Explored.initFillInObject(outputDominantWeightsSimpleCoords.size, false);
  outputMultsSimpleCoords.SetSize(outputDominantWeightsSimpleCoords.size);
  Vector<CoefficientType> currentWeight, currentDominantRepresentative;
  Vector<CoefficientType> Rho;
  Rho=this->GetRho();//<-implicit type conversion here!
//  std::cout << "<br> Rho equals: " << Rho.ElementToString();
  //out << "<br> Rho equals: " << Rho.ElementToString();
  CoefficientType hwPlusRhoSquared;
  hwPlusRhoSquared=this->AmbientWeyl.RootScalarCartanRoot(hwSimpleCoords+Rho, hwSimpleCoords+Rho);
  Explored[0]=true;
  outputMultsSimpleCoords[0]=1;
//  std::cout << "<br>time for generating weights and initializations: " << theGlobalVariables.GetElapsedSeconds()-startTimer;
  //static double totalTimeSpentOnHashIndexing=0;
//  static double timeSpentRaisingWeights=0;
  Vector<CoefficientType> convertor;
  CoefficientType bufferCoeff;
  for (int k=1; k< outputDominantWeightsSimpleCoords.size; k++)
  { Explored[k]=true;
    CoefficientType& currentAccum=outputMultsSimpleCoords[k];
    currentAccum=0;
    for (int j=0; j<this->RootsOfBorel.size; j++)
      for (int i=1; ; i++)
      { convertor=this->RootsOfBorel[j];
        convertor*=i;
        currentWeight= outputDominantWeightsSimpleCoords[k]+convertor;
        currentDominantRepresentative=currentWeight;
//        double startLocal=theGlobalVariables.GetElapsedSeconds();
        this->RaiseToDominantWeight(currentDominantRepresentative);
//        std::cout << "<br>currentDominant representative: " << currentDominantRepresentative.ElementToString();
        int theIndex=outputDominantWeightsSimpleCoords.GetIndex(currentDominantRepresentative);
//        std::cout << "<br>index of currentDomain rep: " << theIndex;
        //totalTimeSpentOnHashIndexing+=theGlobalVariables.GetElapsedSeconds()-beforeHash;
        if (theIndex==-1)
          break;
//        std::cout << "<br> summing over weight: " << currentWeight.ElementToString();
        if (!Explored[theIndex])
        { std::stringstream errorLog;
          errorLog << "This is an internal error check. If you see it, that means " << " that the Freudenthal algorithm implementation is "
          << " wrong (because the author of the implementation was dumb enough to try to write less code than what is "
          << " suggested by LiE).";
          outputDetails=errorLog.str();
          return false;
        }
        convertor=this->RootsOfBorel[j];//<-implicit type conversion here!
        bufferCoeff=this->AmbientWeyl.RootScalarCartanRoot(currentWeight, convertor);
        bufferCoeff*=outputMultsSimpleCoords[theIndex];
        currentAccum+=bufferCoeff;
//        std::cout << "<hr>current weight: " << currentWeight.ElementToString();
//        std::cout << "<br>current dominant representative " << currentDominantRepresentative.ElementToString();
      }
    currentAccum*=2;
//    std::cout << "<br>hwPlusRhoSquared: " << hwPlusRhoSquared.ElementToString();
//    std::cout << "<br>Coeff we divide by: " << (hwPlusRhoSquared-this->AmbientWeyl.RootScalarCartanRoot
//   (outputDominantWeightsSimpleCoords[k]+Rho, outputDominantWeightsSimpleCoords[k]+Rho))
//    .ElementToString()
    ;
    bufferCoeff=hwPlusRhoSquared;
    bufferCoeff-=this->AmbientWeyl.RootScalarCartanRoot
    (outputDominantWeightsSimpleCoords[k]+Rho, outputDominantWeightsSimpleCoords[k]+Rho);
    //bufferCoeff now holds the denominator participating in the Freudenthal formula.
    assert(!bufferCoeff.IsEqualToZero());
    currentAccum/=bufferCoeff;
    std::stringstream out;
    out << " Computed the multiplicities of " << k+1 << " out of "
    << outputDominantWeightsSimpleCoords.size << " dominant weights in the support.";
    theGlobalVariables.MakeStatusReport(out.str());
//    std::cout
//    << "<hr> Computed the multiplicities of " << k+1 << " out of " << outputDominantWeightsSimpleCoords.size << " dominant weights in the support.";
 //   theGlobalVariables.MakeStatusReport(out.str());
//    std::cout << "<br>time so far: " << theGlobalVariables.GetElapsedSeconds()-startTimer;
//    std::cout << " of which " << totalTimeSpentOnHashIndexing << " used for hash routines";
//    std::cout << " of which " << timeSpentRaisingWeights << " used to raise weights";
  }
//  std::cout << "<br>Total freudenthal running time: " << theGlobalVariables.GetElapsedSeconds()-startTimer;
  return true;
}

template <class CoefficientType>
bool charSSAlgMod<CoefficientType>::SplitCharOverLevi
(std::string* Report, charSSAlgMod& output, Vector<Rational>& splittingParSel, const Vector<Rational>& ParSelFDInducingPart,
 ReflectionSubgroupWeylGroup& outputWeylSub, GlobalVariables& theGlobalVariables)
{ std::stringstream out;
  std::string tempS;
  std::cout << "Splitting parabolic selection: " << splittingParSel.ElementToString();
  outputWeylSub.MakeParabolicFromSelectionSimpleRoots(this->GetOwner().theWeyl, splittingParSel, theGlobalVariables,1);
  outputWeylSub.ComputeRootSubsystem();
  ReflectionSubgroupWeylGroup complementGroup;
  Selection invertedSel;
  invertedSel=splittingParSel;
  invertedSel.InvertSelection();
  complementGroup.MakeParabolicFromSelectionSimpleRoots(this->listOwners->TheObjects[this->indexInOwners].theWeyl, invertedSel, theGlobalVariables,1);
  complementGroup.ComputeRootSubsystem();
  out << outputWeylSub.ElementToString(false);
  std::cout << "<hr> the Weyl subgroup: " << outputWeylSub.ElementToString(false);
  std::cout << this->ElementToString();
  std::cout << out.str();
  charSSAlgMod charAmbientFDWeyl, remainingCharDominantLevi;
  ReflectionSubgroupWeylGroup theFDWeyl;
  theFDWeyl.MakeParabolicFromSelectionSimpleRoots
  (this->GetOwner().theWeyl, ParSelFDInducingPart, theGlobalVariables, 1);
  MonomialChar<CoefficientType> tempMon, localHighest;
  List<CoefficientType> tempMults;
  HashedList<Vector<CoefficientType> > tempHashedRoots;
  WeylGroup& theWeyL=this->GetOwner().theWeyl;
  charAmbientFDWeyl.Reset();
  CoefficientType bufferCoeff, highestCoeff;
  for (int i=0; i<this->size; i++)
  { MonomialChar<CoefficientType>& currentMon=this->TheObjects[i];
    if (!theFDWeyl.FreudenthalEvalIrrepIsWRTLeviPart
        (currentMon.weightFundamentalCoords, tempHashedRoots, tempMults, tempS, theGlobalVariables, 10000))
    { if (Report!=0)
        *Report=tempS;
      return false;
    }
    std::cout << "<hr>FreudenthalEval on " << currentMon.ElementToString() << " generated the following report: "
    << tempS << "<hr>";
    for (int j=0; j<tempHashedRoots.size; j++)
    { bufferCoeff=this->theCoeffs[i];
      tempMon.weightFundamentalCoords=theWeyL.GetFundamentalCoordinatesFromSimple(tempHashedRoots[j]);
      bufferCoeff*=tempMults[j];
      charAmbientFDWeyl.AddMonomial(tempMon, bufferCoeff);
    }
  }
  std::cout << "<hr>" << tempS;
  std::cout << "<hr>Freudenthal eval ends up being: " << charAmbientFDWeyl.ElementToString();

  remainingCharDominantLevi.Reset();
  Vectors<CoefficientType> orbitDom;
  for (int i=0; i<charAmbientFDWeyl.size; i++)
  { orbitDom.SetSize(0);
    if (!theFDWeyl.GenerateOrbitReturnFalseIfTruncated
        (theWeyL.GetSimpleCoordinatesFromFundamental(charAmbientFDWeyl[i].weightFundamentalCoords),
         orbitDom, 10000))
    { out << "failed to generate the complement-sub-Weyl-orbit of weight "
      << this->GetOwner().theWeyl.GetSimpleCoordinatesFromFundamental
      (charAmbientFDWeyl[i].weightFundamentalCoords).ElementToString();
      if (Report!=0)
        *Report=out.str();
      return false;
    }
    std::cout << "<hr>the orbit with highest weight "
    << theWeyL.GetSimpleCoordinatesFromFundamental(charAmbientFDWeyl[i].weightFundamentalCoords).ElementToString()
    << " is: ";
    for (int l=0; l<orbitDom.size; l++)
      std::cout <<"<br>" << orbitDom[l].ElementToString();
    out << "<br>of them dominant are: <br>";
    for (int k=0; k<orbitDom.size; k++)
      if (outputWeylSub.IsDominantWeight(orbitDom[k]))
      { tempMon.weightFundamentalCoords=theWeyL.GetFundamentalCoordinatesFromSimple(orbitDom[k]);
        remainingCharDominantLevi.AddMonomial(tempMon, charAmbientFDWeyl.theCoeffs[i]);
        std::cout <<"<br>" << orbitDom[k].ElementToString() << " with coeff " << charAmbientFDWeyl.theCoeffs[i].ElementToString();
      }
  }
  output.Reset();
  out << "<br>Character w.r.t Levi part: " << CGI::GetHtmlMathDivFromLatexAddBeginARCL
  (remainingCharDominantLevi.ElementToString());
  std::cout << "Character w.r.t Levi part: " << CGI::GetHtmlMathDivFromLatexAddBeginARCL
  (remainingCharDominantLevi.ElementToString());

  Vector<CoefficientType> simpleGeneratorBaseField;
  while(!remainingCharDominantLevi.IsEqualToZero())
  { localHighest=*remainingCharDominantLevi.LastObject();
    for (bool Found=true; Found; )
    { Found=false;
      for (int i=0; i<outputWeylSub.simpleGenerators.size; i++)
      { tempMon=localHighest;
        simpleGeneratorBaseField=outputWeylSub.simpleGenerators[i]; // <- implicit type conversion here!
        tempMon.weightFundamentalCoords+=this->GetOwner().theWeyl.GetFundamentalCoordinatesFromSimple
        (simpleGeneratorBaseField);
        if (remainingCharDominantLevi.Contains(tempMon))
        { localHighest=tempMon;
          Found=true;
        }
      }
    }
    highestCoeff=remainingCharDominantLevi.theCoeffs[remainingCharDominantLevi.GetIndex(localHighest)];
    output.AddMonomial(localHighest, highestCoeff);
    if (!outputWeylSub.FreudenthalEvalIrrepIsWRTLeviPart
        (localHighest.weightFundamentalCoords, tempHashedRoots, tempMults, tempS, theGlobalVariables, 10000))
    { if (Report!=0)
        *Report=tempS;
      return false;
    }
    std::cout << "<hr>accounting " << localHighest.ElementToString() << " with coeff "
    << highestCoeff.ElementToString() << "<br>"
    << remainingCharDominantLevi.ElementToString();
    for (int i=0; i<tempHashedRoots.size; i++)
    { tempMon.weightFundamentalCoords=theWeyL.GetFundamentalCoordinatesFromSimple(tempHashedRoots[i]);
      bufferCoeff=tempMults[i];
      bufferCoeff*=highestCoeff;
      remainingCharDominantLevi.SubtractMonomial(tempMon, bufferCoeff);
      std::cout << "<br>-(" << bufferCoeff.ElementToString() << ")" << tempMon.ElementToString();
    }
    std::cout << "<br>remaining character after accounting:<br>" << remainingCharDominantLevi.ElementToString();
  }
  out << "<br>Character w.r.t Levi part: " << CGI::GetHtmlMathDivFromLatexAddBeginARCL
  (output.ElementToString())
  ;

  if (Report!=0)
  { //out << "<hr>"  << "The split character is: " << output.ElementToString("V", "\\omega", false);
    DrawingVariables theDV;
    std::string tempS;
    this->DrawMeNoMults(tempS, theGlobalVariables, theDV, 10000);
    Vector<Rational> tempRoot;
    out << "<hr>In the following weight visualization, a yellow line is drawn if the corresponding weights are "
    << " simple reflections of one another, with respect to a simple Vector<Rational> of the Levi part of the parabolic subalgebra. ";
    for (int i=0; i<output.size; i++)
    { tempRoot=theWeyL.GetSimpleCoordinatesFromFundamental(output[i].weightFundamentalCoords).GetVectorRational();
      outputWeylSub.DrawContour
      (tempRoot, theDV, theGlobalVariables, CGI::RedGreenBlue(200, 200, 0), 1000);
      std::stringstream tempStream;
      tempStream << output.theCoeffs[i].ElementToString();
      theDV.drawTextAtVectorBuffer(tempRoot, tempStream.str(), 0, DrawingVariables::PenStyleNormal, 0);
    }
    out << "<hr>" << theDV.GetHtmlFromDrawOperationsCreateDivWithUniqueName(theWeyL.GetDim());
    *Report=out.str();
  }
  return true;
}

template<class CoefficientType>
void ModuleSSalgebraNew<CoefficientType>::SplitOverLevi
  (std::string* Report, Vector<Rational>& splittingParSel, GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit,
   const CoefficientType& theRingZero)
{ if (this->theChaR.size!=1)
  { if (Report!=0)
      *Report="I have been instructed only to split modules that are irreducible over the ambient Lie algebra";
    return;
  }
  ReflectionSubgroupWeylGroup subWeyl;
  charSSAlgMod<CoefficientType> charWRTsubalgebra;
  this->theChaR.SplitCharOverLevi
  (Report, charWRTsubalgebra, splittingParSel, this->parabolicSelectionNonSelectedAreElementsLevi,
   subWeyl, theGlobalVariables);
/*  Vector<Rational> theHWsimpleCoords, theHWfundCoords;
  std::stringstream out;
  if(Report!=0)
    out << *Report;
  Selection splittingParSelectedInLevi;
  splittingParSelectedInLevi=splittingParSel;
  splittingParSelectedInLevi.InvertSelection();
  if (!splittingParSelectedInLevi.IsSubset(this->parabolicSelectionSelectedAreElementsLevi))
  { out << "The parabolic subalgebra you selected is not a subalgebra of the ambient parabolic subalgebra."
    << " The parabolic has Vectors<Rational> of Levi given by " << splittingParSel.ElementToString()
    <<" while the ambient parabolic subalgebra has Vectors<Rational> of Levi given by "
    << this->parabolicSelectionNonSelectedAreElementsLevi.ElementToString();
    if (Report!=0)
      *Report=out.str();
    return;
  }
  out << "<br>Parabolic selection: " << splittingParSel.ElementToString();
  List<List<List<CoefficientType> > > eigenSpacesPerSimpleGenerator;
 // if (false)
  eigenSpacesPerSimpleGenerator.SetSize(splittingParSelectedInLevi.CardinalitySelection);
  Vectors<CoefficientType> theFinalEigenSpace, tempSpace1, tempSpace2;
//  WeylGroup& theWeyL=this->theAlgebra.theWeyl;
  for (int i=0; i<splittingParSelectedInLevi.CardinalitySelection; i++)
  { int theGenIndex=splittingParSelectedInLevi.elements[i]+this->GetOwner().GetRank();
    Matrix<CoefficientType>& currentOp=this->actionsSimpleGensMatrixForM[theGenIndex];
    currentOp.FindZeroEigenSpacE(eigenSpacesPerSimpleGenerator[i], 1, -1, 0, theGlobalVariables);
    if (i==0)
      theFinalEigenSpace.AssignListListCoefficientType(eigenSpacesPerSimpleGenerator[i]);
    else
    { tempSpace1=theFinalEigenSpace;
      tempSpace2.AssignListListCoefficientType(eigenSpacesPerSimpleGenerator[i]);
      theFinalEigenSpace.IntersectTwoLinSpaces(tempSpace1, tempSpace2, theFinalEigenSpace, theGlobalVariables);
    }
  }
  out << "<br>Eigenvectors:<table> ";
//  Vector<Rational> zeroRoot;
//  zeroRoot.MakeZero(this->theAlgebra->GetRank());
  std::stringstream readyForLatexComsumption;
  readyForLatexComsumption << "\\begin{tabular}{|lll|}\n <br>";
  std::stringstream extraHWAddition;
  for (int i=0; i<this->parabolicSelectionNonSelectedAreElementsLevi.CardinalitySelection; i++)
    extraHWAddition << "x_{" << this->parabolicSelectionNonSelectedAreElementsLevi.elements[i]+1
    << "}\\omega_{" << this->parabolicSelectionNonSelectedAreElementsLevi.elements[i]+1 << "}+";
  readyForLatexComsumption << "\\hline\\multicolumn{3}{|c|}{Highest weight $" << extraHWAddition.str();
  readyForLatexComsumption
  << this->GetOwner().theWeyl.GetEpsilonCoords(this->theHWSimpleCoordS, theGlobalVariables)
  .ElementToStringEpsilonForm(true, false)
  << "$}\\\\\n<br>";
  readyForLatexComsumption << "weight fund. coord.& weight& singular vector \\\\\\hline\n<br>";
  for (int j=0; j<theFinalEigenSpace.size; j++)
  { out << "<tr><td>";
    ElementUniversalEnveloping<CoefficientType> currentElt, tempElt;
    currentElt.MakeZero(*this->theAlgebras, this->indexAlgebra);
    Vector<CoefficientType>& currentVect= theFinalEigenSpace[j];
    int lastNonZeroIndex=-1;
    for (int i=0; i<currentVect.size; i++)
      if (!(currentVect[i].IsEqualToZero()))
      { tempElt.MakeZero(*this->theAlgebras, this->indexAlgebra);
        tempElt.AddMonomial(this->theGeneratingWordsNonReduced[i], 1);
        tempElt*=currentVect[i];
        currentElt+=tempElt;
        lastNonZeroIndex=i;
      }
    Vector<Rational>& currentWeight=this->theGeneratingWordsNonReducedWeights[lastNonZeroIndex];
    readyForLatexComsumption << "$" << extraHWAddition.str()
    << this->GetOwner().theWeyl.GetFundamentalCoordinatesFromSimple(currentWeight)
    .ElementToStringLetterFormat("\\omega", false, false)
    << "$&$" << extraHWAddition.str()
    << this->GetOwner().theWeyl.GetEpsilonCoords(currentWeight, theGlobalVariables)
    .Vector<Rational>::ElementToStringEpsilonForm(true, false)
    << "$&$" << currentVect.ElementToStringLetterFormat("m", true, false) << "$";

    if (currentElt.size>1)
      out << "(";
    out << currentElt.ElementToString(&theGlobalVariables.theDefaultLieFormat);
    if (currentElt.size>1)
      out << ")";
    out << " v(" << this->theHWFundamentalCoordS.ElementToString() << "," << ((Vector<Rational>)this->parabolicSelectionNonSelectedAreElementsLevi).ElementToString() << ")" ;
    out << "</td><td>(weight: "
    << currentWeight.ElementToString() << ")</td></tr>";
    readyForLatexComsumption << "\\\\\n<br>";
  }
  out << "</table>";
  readyForLatexComsumption << "\\hline \n<br> \\end{tabular}";
  out << "<br>Your ready for LaTeX consumption text follows.<br>";
  out << readyForLatexComsumption.str();
  if (Report!=0)
    *Report=out.str();*/
}

template <class CoefficientType>
void charSSAlgMod<CoefficientType>::MakeTrivial(List<SemisimpleLieAlgebra>& inputOwners, int inputIndexInOwners)
{ this->MakeZero(inputOwners, inputIndexInOwners);
  MonomialChar<Rational> tempMon;
  tempMon.weightFundamentalCoords.MakeZero(inputOwners[inputIndexInOwners].GetRank());
  this->AddMonomial(tempMon, 1);
}

template <class CoefficientType>
bool charSSAlgMod<CoefficientType>::FreudenthalEvalMe
 (charSSAlgMod<CoefficientType>& outputCharOwnerSetToZero, std::string& outputDetails,
  GlobalVariables& theGlobalVariables, int upperBoundNumDominantWeights)
{ assert(&outputCharOwnerSetToZero!=this);
  outputCharOwnerSetToZero.Reset();
  Vector<CoefficientType> currentWeightFundCoords;
  List<CoefficientType> currentMults;
  HashedList<Vector<CoefficientType> > currentWeights;
  std::stringstream localErrors, localDetails;
  std::string localDetail;
  MonomialChar<CoefficientType> tempMon;
  CoefficientType bufferCoeff;
  for (int i=0; i<this->size; i++)
  { currentWeightFundCoords=this->TheObjects[i].weightFundamentalCoords;
    if (!this->GetOwner().theWeyl.FreudenthalEval
    (currentWeightFundCoords, currentWeights, currentMults, localDetail, theGlobalVariables, upperBoundNumDominantWeights))
    { localErrors << "Encountered error while evaluating freudenthal formula. Error details: " << localDetail
      << "<br> Further computation detail: " << localDetails.str();
      outputDetails=localErrors.str();
      return false;
    }
    if (localDetail!="")
      localDetails << "<br>MonomialP " << i+1 << " computation details: " << localDetail;
    for (int j=0; j<currentWeights.size; j++)
    { tempMon.weightFundamentalCoords=this->GetOwner().theWeyl.GetFundamentalCoordinatesFromSimple(currentWeights[j]);
      bufferCoeff=this->theCoeffs[i];
      bufferCoeff*=currentMults[j];
      outputCharOwnerSetToZero.AddMonomial(tempMon, bufferCoeff);
    }
  }
  outputDetails=localDetails.str();
  return true;
}

template <class CoefficientType>
bool WeylGroup::GenerateOrbit
(Vectors<CoefficientType>& theRoots, bool RhoAction, HashedList<Vector<CoefficientType> >& output,
 bool UseMinusRho, int expectedOrbitSize, WeylGroup* outputSubset, int UpperLimitNumElements)
{ output.Clear();
  for (int i=0; i<theRoots.size; i++)
    output.AddOnTop(theRoots.TheObjects[i]);
  Vector<CoefficientType> currentRoot;
  ElementWeylGroup tempEW;
  if (expectedOrbitSize<=0)
    expectedOrbitSize=(this->GetSizeWeylByFormula(this->WeylLetter, this->GetDim())).NumShort;
  if (UpperLimitNumElements>0 && expectedOrbitSize>UpperLimitNumElements)
    expectedOrbitSize=UpperLimitNumElements;
  output.SetExpectedSize(expectedOrbitSize);
  if (outputSubset!=0)
  { if (UpperLimitNumElements!=-1)
      expectedOrbitSize=MathRoutines::Minimum(UpperLimitNumElements, expectedOrbitSize);
    tempEW.size=0;
    outputSubset->SetExpectedSize(expectedOrbitSize);
    outputSubset->CartanSymmetric=this->CartanSymmetric;
    outputSubset->Clear();
    outputSubset->AddOnTop(tempEW);
  }
  for (int i=0; i<output.size; i++)
  { if (outputSubset!=0)
      tempEW=outputSubset->TheObjects[i];
    for (int j=0; j<this->CartanSymmetric.NumRows; j++)
    { currentRoot=output[i];
      //if (this->flagAnErrorHasOcurredTimeToPanic)
      //{ currentRoot.ComputeDebugString();
      //}
      this->SimpleReflection(j, currentRoot, RhoAction, UseMinusRho);
      //if (this->flagAnErrorHasOcurredTimeToPanic)
      //{ currentRoot.ComputeDebugString();
      //}
      if (output.AddNoRepetition(currentRoot))
        if (outputSubset!=0)
        { tempEW.AddOnTop(j);
          outputSubset->AddOnTop(tempEW);
          tempEW.PopIndexSwapWithLast(tempEW.size-1);
        }
      if (UpperLimitNumElements>0)
        if (output.size>=UpperLimitNumElements)
          return false;
    }
  }
  return true;
}

template <class CoefficientType>
bool WeylGroup::FreudenthalEval
(Vector<CoefficientType>& inputHWfundamentalCoords, HashedList<Vector<CoefficientType> >& outputDominantWeightsSimpleCoords,
 List<CoefficientType>& outputMultsSimpleCoords, std::string& outputDetails,
 GlobalVariables& theGlobalVariables, int UpperBoundFreudenthal)
{ //double startTimer=theGlobalVariables.GetElapsedSeconds();
  this->ComputeRho(true);
  Vectors<Rational> EiBasis;
  EiBasis.MakeEiBasis(this->GetDim());
  List<bool> Explored;
  Vector<CoefficientType> hwSimpleCoords=this->GetSimpleCoordinatesFromFundamental(inputHWfundamentalCoords);
  if (!this->GetAlLDominantWeightsHWFDIM
      (hwSimpleCoords, outputDominantWeightsSimpleCoords, UpperBoundFreudenthal, outputDetails, theGlobalVariables))
  { std::stringstream errorLog;
    errorLog << "Error: the number of dominant weights exceeded hard-coded limit of "
    << UpperBoundFreudenthal << ". Please check out whether LiE's implementation of the Freudenthal "
    << " formula can do your computation.";
    outputDetails=errorLog.str();
    return false;
  }
  Explored.initFillInObject(outputDominantWeightsSimpleCoords.size, false);
  outputMultsSimpleCoords.SetSize(outputDominantWeightsSimpleCoords.size);
  Vector<CoefficientType> currentWeight, currentDominantRepresentative, convertor;
  CoefficientType hwPlusRhoSquared;
  convertor=hwSimpleCoords;
  convertor+=this->rho;//<-implicit type conversion here!!!!
  hwPlusRhoSquared=this->RootScalarCartanRoot(convertor, convertor);
  outputMultsSimpleCoords[0]=1;
  Explored[0]=true;
//  std::cout << "<br>time for generating weights and initializations: " << theGlobalVariables.GetElapsedSeconds()-startTimer;
  //static double totalTimeSpentOnHashIndexing=0;
//  static double timeSpentRaisingWeights=0;
  CoefficientType BufferCoeff;
  for (int k=1; k<outputDominantWeightsSimpleCoords.size; k++)
  { Explored[k]=true;
    CoefficientType& currentAccum=outputMultsSimpleCoords[k];
    currentAccum=0;
    for (int j=0; j<this->RootsOfBorel.size; j++)
      for (int i=1; ; i++)
      { currentWeight=outputDominantWeightsSimpleCoords[k];
        currentWeight+=this->RootsOfBorel[j]*i;
        currentDominantRepresentative=currentWeight;
//        double startLocal=theGlobalVariables.GetElapsedSeconds();
        this->RaiseToDominantWeight(currentDominantRepresentative);
//        timeSpentRaisingWeights+=theGlobalVariables.GetElapsedSeconds()-startLocal;
        //double beforeHash=theGlobalVariables.GetElapsedSeconds();
        int theIndex=outputDominantWeightsSimpleCoords.GetIndex(currentDominantRepresentative);
        //totalTimeSpentOnHashIndexing+=theGlobalVariables.GetElapsedSeconds()-beforeHash;
        if (theIndex==-1)
          break;
//        std::cout << "<br> summing over weight: " << currentWeight.ElementToString();
        if (!Explored[theIndex])
        { std::stringstream errorLog;
          errorLog << "This is an internal error check. If you see it, that means "
          << " that the Freudenthal algorithm implementation is "
          << " wrong (because the author of the implementation was dumb enough to try to write less code than what is "
          << " suggested by LiE).";
          outputDetails=errorLog.str();
          return false;
        }
        BufferCoeff=this->RootScalarCartanRoot(currentWeight, this->RootsOfBorel[j]);
        BufferCoeff*=outputMultsSimpleCoords[theIndex];
        currentAccum+=BufferCoeff;
      }
    currentAccum*=2;
    convertor=outputDominantWeightsSimpleCoords[k];
    convertor+=this->rho;
    BufferCoeff=hwPlusRhoSquared;
    BufferCoeff-=this->RootScalarCartanRoot(convertor, convertor);
    currentAccum/=BufferCoeff;

//    std::cout << "<br>Coeff we divide by: " << (hwPlusRhoSquared-this->RootScalarCartanRoot
 //   (outputDominantWeightsSimpleCoords[k]+this->rho, outputDominantWeightsSimpleCoords[k]+this->rho))
  //  .ElementToString()
   // ;
    std::stringstream out;
    out << " Computed the multiplicities of " << k+1 << " out of "
    << outputDominantWeightsSimpleCoords.size << " dominant weights in the support.";
    theGlobalVariables.MakeStatusReport(out.str());
//    std::cout
//    << "<hr> Computed the multiplicities of " << k+1 << " out of " << outputDominantWeightsSimpleCoords.size << " dominant weights in the support.";
    theGlobalVariables.MakeStatusReport(out.str());
//    std::cout << "<br>time so far: " << theGlobalVariables.GetElapsedSeconds()-startTimer;
//    std::cout << " of which " << totalTimeSpentOnHashIndexing << " used for hash routines";
//    std::cout << " of which " << timeSpentRaisingWeights << " used to raise weights";
  }
//  std::cout << "<br>Total freudenthal running time: " << theGlobalVariables.GetElapsedSeconds()-startTimer;
  return true;
}

template<class CoefficientType>
bool WeylGroup::GetAlLDominantWeightsHWFDIM
(Vector<CoefficientType>& highestWeightSimpleCoords, HashedList<Vector<CoefficientType> >& outputWeightsSimpleCoords,
 int upperBoundDominantWeights, std::string& outputDetails, GlobalVariables& theGlobalVariables)
{ std::stringstream out;
//  double startTime=theGlobalVariables.GetElapsedSeconds();
//  std::cout << "<br>time elapsed: " << theGlobalVariables.GetElapsedSeconds()-startTime;
  Vector<CoefficientType> highestWeightTrue;
  highestWeightTrue=highestWeightSimpleCoords;
  this->RaiseToDominantWeight(highestWeightTrue);
  Vector<CoefficientType> highestWeightFundCoords=this->GetFundamentalCoordinatesFromSimple(highestWeightTrue);
  if (!highestWeightFundCoords.SumCoords().IsSmallInteger())
  { out << "<hr> The highest weight you gave in simple coordinates: " << highestWeightSimpleCoords.ElementToString()
    << " which equals " << highestWeightFundCoords.ElementToString() << "  in fundamental coordinates "
    << " is not integral dominant.<br>";
    outputDetails=out.str();
    return false;
  }
  int theTopHeightSimpleCoords=(int) highestWeightSimpleCoords.GetVectorRational().SumCoords().DoubleValue()+1;
//  int theTopHeightFundCoords=(int) highestWeightFundCoords.SumCoords().DoubleValue();
  if (theTopHeightSimpleCoords<0)
    theTopHeightSimpleCoords=0;
  List<HashedList<Vector<CoefficientType> > > outputWeightsByHeight;
  int topHeightRootSystem=this->RootsOfBorel.LastObject()->SumCoords().NumShort;
  int topHeightRootSystemPlusOne=topHeightRootSystem+1;
  outputWeightsByHeight.SetSize(topHeightRootSystemPlusOne);
  int finalHashSize=100;
  for (int i=0; i<topHeightRootSystemPlusOne; i++)
    outputWeightsByHeight[i].SetHashSizE(finalHashSize);
  outputWeightsSimpleCoords.Clear();
  outputWeightsSimpleCoords.SetHashSizE(10000);
  outputWeightsByHeight[0].AddOnTop(highestWeightTrue);
  int numTotalWeightsFound=0;
  int numPosRoots=this->RootsOfBorel.size;
  Vector<CoefficientType> currentWeight;
//  std::cout << "<br>time spend before working cycle: " << theGlobalVariables.GetElapsedSeconds()-startTime;
  for (int lowestUnexploredHeightDiff=0; lowestUnexploredHeightDiff<=theTopHeightSimpleCoords;
  lowestUnexploredHeightDiff++)
  { //double startCycleTime=theGlobalVariables.GetElapsedSeconds();
    if (upperBoundDominantWeights>0 && numTotalWeightsFound>upperBoundDominantWeights)
      break;
    int bufferIndexShift=lowestUnexploredHeightDiff%topHeightRootSystemPlusOne;
    HashedList<Vector<CoefficientType> >& currentHashes=outputWeightsByHeight[bufferIndexShift];
    for (int lowest=0; lowest<currentHashes.size; lowest++)
      for (int i=0; i<numPosRoots; i++)
      { currentWeight=currentHashes[lowest];
        currentWeight-=this->RootsOfBorel[i];
        if (this->IsDominantWeight(currentWeight))
        { int currentIndexShift=this->RootsOfBorel[i].SumCoords().NumShort;
          currentIndexShift=(currentIndexShift+bufferIndexShift)%topHeightRootSystemPlusOne;
          if (outputWeightsByHeight[currentIndexShift].AddNoRepetition(currentWeight))
          { numTotalWeightsFound++;
            outputWeightsByHeight[currentIndexShift].AdjustHashes();
          }
        }
      }
//    std::cout << "<br>time spent before accounting at height level " << lowestUnexploredHeightDiff
//    << ": " << theGlobalVariables.GetElapsedSeconds()-startCycleTime;
//    std::cout << " Size of current level: " << currentHashes.size;
    outputWeightsSimpleCoords.AddOnTop(currentHashes);
//    std::cout << ". Time spent after accounting at height level " << lowestUnexploredHeightDiff
//    << ": " << theGlobalVariables.GetElapsedSeconds()-startCycleTime;
//    startCycleTime=theGlobalVariables.GetElapsedSeconds();
    outputWeightsSimpleCoords.AdjustHashes();
    currentHashes.Clear();
//    std::cout << ". Time spent clearing up buffer at height level " << lowestUnexploredHeightDiff
//    << ": " << theGlobalVariables.GetElapsedSeconds()-startCycleTime;
  }
  out << " Total number of dominant weights: " << outputWeightsSimpleCoords.size;
  if (numTotalWeightsFound>=upperBoundDominantWeights)
    out << "<hr>This message is generated either because the number of weights has "
    << "exceeded the hard-coded RAM memory limits, or because "
    << " a priori bound for the number of weights is WRONG. If the latter "
    << " is the case, make sure to send an angry email to the author(s).";
  outputDetails=out.str();
  //std::cout << "<hr><hr>Total time spent generating weights: " << -startTime+theGlobalVariables.GetElapsedSeconds();
  return (numTotalWeightsFound<=upperBoundDominantWeights);
}

#endif
