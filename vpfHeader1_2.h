//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#ifndef vpfHeader1_2_h_already_included
#define vpfHeader1_2_h_already_included

#include "vpfHeader1.h"
#include "vpfHeader3ListReferences.h"

static ProjectInformationInstance ProjectInfoVpfHeader1_2(__FILE__, "Header, math routines. ");

class WeylGroup;

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
    (const Matrix<Rational>& theLinearMap, const Vectors<Rational>& input, Vectors<Rational>& output, GlobalVariables& theGlobalVariables)
;
  void IntersectWithPreimageOfLattice
  (const Matrix<Rational> & theLinearMap, const Lattice& other, GlobalVariables& theGlobalVariables)
;
  void IntersectWithLineGivenBy(Vector<Rational>& inputLine, Vector<Rational>& outputGenerator);
  static bool GetClosestPointInDirectionOfTheNormalToAffineWallMovingIntegralStepsInDirection
  (Vector<Rational>& startingPoint, Vector<Rational>& theAffineHyperplane, Vector<Rational>& theDirection, Vector<Rational>& outputPoint)
  ;
  void GetDefaultFundamentalDomainInternalPoint(Vector<Rational>& output);
  bool GetInternalPointInConeForSomeFundamentalDomain
(Vector<Rational>& output, Cone& coneContainingOutputPoint, GlobalVariables& theGlobalVariables)
  ;
  void GetRootOnLatticeSmallestPositiveProportionalTo
(Vector<Rational>& input, Vector<Rational>& output, GlobalVariables& theGlobalVariables)
  ;
  void GetRougherLatticeFromAffineHyperplaneDirectionAndLattice
  (const Vector<Rational>& theDirection, Vector<Rational>& outputDirectionMultipleOnLattice, Vector<Rational>& theShift, Vector<Rational>& theAffineHyperplane,
   Vectors<Rational>& outputRepresentatives,
   Vectors<Rational>& movementInDirectionPerRepresentative,
   Lattice& outputRougherLattice,
   GlobalVariables& theGlobalVariables)
     ;
  void ApplyLinearMap
  (Matrix<Rational>& theMap, Lattice& output)
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
    (const Matrix<Rational>& theSub, GlobalVariables& theGlobalVariables)
;
  bool SubstitutionHomogeneous
    (const PolynomialSubstitution<Rational>& theSub, GlobalVariables& theGlobalVariables)
;
//the following function follows the same convention as the preceding except that we allow n<m. However,
// in order to assure that the preimage of the lattice is a lattice,
//we provide as input an ambient lattice in the new vector space of dimension m
  bool SubstitutionHomogeneous
    (const Matrix<Rational>& theSub, Lattice& resultIsSubsetOf, GlobalVariables& theGlobalVariables)
;
  void Reduce
  ()
  ;
  void IntersectWithLinearSubspaceSpannedBy(const Vectors<Rational>& theSubspaceBasis);
  void IntersectWithLinearSubspaceGivenByNormals(const Vectors<Rational>& theSubspaceNormals);
  void IntersectWithLinearSubspaceGivenByNormal(const Vector<Rational>& theNormal);
static bool GetHomogeneousSubMatFromSubIgnoreConstantTerms
(const PolynomialSubstitution<Rational>& theSub, Matrix<Rational>& output, GlobalVariables& theGlobalVariables)
;
  //returning false means that the lattice given as rougher is not actually rougher than the current lattice
  //or that there are too many representatives
  bool GetAllRepresentatives
  (const Lattice& rougherLattice, Vectors<Rational>& output)const
  ;
  bool GetAllRepresentativesProjectingDownTo
  (const Lattice& rougherLattice, Vectors<Rational>& startingShifts, Vectors<Rational>& output)const
  ;
  inline std::string ToString()const{return this->ToString(true, false);}
  std::string ToString(bool useHtml, bool useLatex)const;
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
  (const Matrix<Rational>& input)
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
  std::string ToString(bool useHtml, bool useLatex){return this->ToString(useHtml, useLatex, 0);}
  std::string ToString(bool useHtml, bool useLatex, FormatExpressions* thePolyFormat);
  void ComputeDebugString(){this->DebugString=this->ToString(false, false);}
  Rational Evaluate(const Vector<Rational>& input);
  void AddLatticeShift(const Polynomial<Rational>& input, const Vector<Rational>& inputShift);
  void AddAssumingLatticeIsSame(const QuasiPolynomial& other);
  void MakeRougherLattice(const Lattice& latticeToRoughenBy);
  void MakeFromPolyShiftAndLattice
  (const Polynomial<Rational>& inputPoly, const MonomialP& theShift, const Lattice& theLattice, GlobalVariables& theGlobalVariables);
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
  (const Matrix<Rational>& mapFromNewSpaceToOldSpace,
   const Lattice& ambientLatticeNewSpace, QuasiPolynomial& output, GlobalVariables& theGlobalVariables)
  ;
  void Substitution
  (const Vector<Rational>& inputTranslationSubtractedFromArgument,
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
  bool rootIsInFractionCone (partFractions& owner, Vector<Rational>* theRoot, GlobalVariables& theGlobalVariables);
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
  friend std::ostream& operator << (std::ostream& output, const partFraction& input)
  { output << " Not implemented, please fix. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    return output;
  }
  static const bool IsEqualToZero(){return false;}
  bool RemoveRedundantShortRootsClassicalRootSystem
(partFractions& owner, Vector<Rational>* Indicator, Polynomial<LargeInt>& buffer1, int theDimension, GlobalVariables& theGlobalVariables)
  ;
  bool RemoveRedundantShortRoots(partFractions& owner, Vector<Rational>* Indicator, GlobalVariables& theGlobalVariables, int theDimension);
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
  (QuasiPolynomial& outputQP, const MonomialP& theMon, Vectors<Rational>& normals,
   Lattice& theLattice, GlobalVariables& theGlobalVariables)
  ;
  static void EvaluateIntPoly
  (const Polynomial<LargeInt>& input, const Vector<Rational>& values, Rational& output)
  ;
  static void MakePolynomialFromOneNormal
  (Vector<Rational>& normal, const MonomialP& shiftRational, int theMult,
   Polynomial<Rational>& output);
  void ComputeNormals
(partFractions& owner, Vectors<Rational>& output, int theDimension, Matrix<Rational>& buffer)
  ;
  int ComputeGainingMultiplicityIndexInLinearRelation
(bool flagUsingOrlikSolomon, Matrix<Rational>& theLinearRelation)
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
  unsigned int HashFunction() const;
  static inline unsigned int HashFunction(const partFraction& input)
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
  bool operator==(const partFraction& right)const;
  void operator=(const partFraction& right);
  bool initFromRoots(partFractions& owner, Vectors<Rational>& input);
  std::string ToString
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
  std::string ToString(FormatExpressions* theFormat=0)const;
  void TransformToWeylProjective
  (ConeComplex& owner, GlobalVariables& theGlobalVariables)
  ;
  std::string DrawMeToHtmlProjective(DrawingVariables& theDrawingVariables, FormatExpressions& theFormat);
  std::string DrawMeToHtmlLastCoordAffine(DrawingVariables& theDrawingVariables, FormatExpressions& theFormat);
  void TranslateMeMyLastCoordinateAffinization(Vector<Rational>& theTranslationVector);
  bool IsAnHonest1DEdgeAffine(const Vector<Rational>& vertex1, const Vector<Rational>& vertex2)const
  { int numCommonWalls=0;
    for (int i=0; i<this->Normals.size; i++)
      if(vertex1.ScalarEuclidean(this->Normals[i]).IsEqualToZero() &&
         vertex2.ScalarEuclidean(this->Normals[i]).IsEqualToZero())
      { numCommonWalls++;
        if (numCommonWalls==this->GetDim()-2)
          return true;
      }
    return false;
  }
  bool IsTheEntireSpace()
  { return this->Normals.size==0 && this->flagIsTheZeroCone;
  }
  bool IsAnHonest1DEdgeAffine(int vertexIndex1, int vertexIndex2)const
  { Vector<Rational>& vertex1=this->Vertices[vertexIndex1];
    Vector<Rational>& vertex2=this->Vertices[vertexIndex2];
    return this->IsAnHonest1DEdgeAffine(vertex1, vertex2);
  }
  bool DrawMeLastCoordAffine
  (bool InitDrawVars, DrawingVariables& theDrawingVariables, FormatExpressions& theFormat,
   int ChamberWallColor=0)const;
  bool DrawMeProjective
(Vector<Rational>* coordCenterTranslation, bool initTheDrawVars, DrawingVariables& theDrawingVariables, FormatExpressions& theFormat)
  const;
  bool IsInCone(const Vector<Rational>& point) const;
  bool IsInCone(const Vectors<Rational>& vertices)const
  { for (int i=0; i<vertices.size; i++)
      if (!this->IsInCone(vertices[i]))
        return false;
    return true;
  }
  bool GetLatticePointsInCone
  (Lattice& theLattice, Vector<Rational>& theShift, int upperBoundPointsInEachDim, bool lastCoordinateIsOne,
   Vectors<Rational>& outputPoints, Vector<Rational>* shiftAllPointsBy)const
  ;
  bool MakeConvexHullOfMeAnd(const Cone& other, GlobalVariables& theGlobalVariables);
  void ChangeBasis
  (Matrix<Rational>& theLinearMap, GlobalVariables& theGlobalVariables)
    ;
  std::string DebugString;
  int GetDim()const
  { if (this->Normals.size==0)
      return 0;
    return this->Normals[0].size;
  }
  void SliceInDirection
  (Vector<Rational>& theDirection, ConeComplex& output, GlobalVariables& theGlobalVariables)
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
  void GetInternalPoint(Vector<Rational>& output)const
  { if (this->Vertices.size<=0)
      return;
    this->Vertices.sum(output, this->Vertices[0].size);
  }
  Vector<Rational> GetInternalPoint()const
  { Vector<Rational> result;
    this->GetInternalPoint(result);
    return result;
  }
  unsigned int HashFunction()const
  { return this->Vertices.HashFunction();
  }
  static inline unsigned int HashFunction(const Cone& input)
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
  std::string ToString(FormatExpressions& theFormat);
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
  int GetDim()
  { if (this->size<=0)
      return -1;
    return this->TheObjects[0].GetDim();
  }
  bool AddNonRefinedChamberOnTopNoRepetition(const Cone& newCone, GlobalVariables& theGlobalVariables);
  void PopChamberSwapWithLast(int index);
  void GetAllWallsConesNoOrientationNoRepetitionNoSplittingNormals
    (Vectors<Rational>& output)const
  ;
  bool DrawMeLastCoordAffine
  (bool InitDrawVars, DrawingVariables& theDrawingVariables, FormatExpressions& theFormat)
  ;
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
(Vector<Rational>* coordCenterTranslation, bool InitDrawVars, DrawingVariables& theDrawingVariables, FormatExpressions& theFormat)
  ;
  std::string DrawMeToHtmlProjective
(DrawingVariables& theDrawingVariables, FormatExpressions& theFormat)
;
  std::string ToString(){return this->ToString(false, false);}
  std::string ToString(bool useLatex, bool useHtml);
  void ComputeDebugString(){this->DebugString=this->ToString();}
  int GetLowestIndexchamberContaining(const Vector<Rational>& theRoot)const
  { for (int i=0; i<this->size; i++)
      if (this->TheObjects[i].IsInCone(theRoot))
        return i;
    return -1;
  }
  bool findMaxLFOverConeProjective
  (const Cone& input, List<Polynomial<Rational> >& inputLinPolys, List<int>& outputMaximumOverEeachSubChamber, GlobalVariables& theGlobalVariables)
  ;
  bool findMaxLFOverConeProjective
  (const Cone& input, Vectors<Rational>& inputLFsLastCoordConst,
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
(int indexChamberBeingRefined, bool weAreSlicingInDirection, bool weAreChopping, Vector<Rational>& killerNormal, GlobalVariables& theGlobalVariables)
  ;
  void GetNewVerticesAppend
  (Cone& myDyingCone, Vector<Rational>& killerNormal, HashedList<Vector<Rational> >& outputVertices, GlobalVariables& theGlobalVariables)
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
  std::string ToString(GlobalVariables& theGlobalVariables, FormatExpressions& theFormat){std::string tempS; this->ToString(tempS, theGlobalVariables, theFormat);  return tempS;}
  void ToString(std::string& output, GlobalVariables& theGlobalVariables, FormatExpressions& theFormat);
  int ToString
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

class DynkinSimpleType
{ friend std::ostream& operator << (std::ostream& output, const DynkinSimpleType& theMon)
  { output << theMon.ToString();
    return output;
  }
  public:
  char theLetter;
  int theRank;
  Rational lengthFirstCoRootSquared;
  DynkinSimpleType(): theLetter('X'), theRank(-1), lengthFirstCoRootSquared(0){}
  void MakeAone()
  { this->theLetter='A';
    this->theRank=1;
    this->lengthFirstCoRootSquared=2;
  }
  int GetRootSystemSize()const;
  int GetRootSystemPlusRank()const
  { return this->GetRootSystemSize()+this->theRank;
  }
  int GetSSAlgDim()const
  { return this->GetRootSystemSize()+this->theRank;
  }
  void MakeArbitrary(char inputLetter, int inputRank, Rational inputLengthFirstCorRootSquared=0)
  { if ((inputLetter!= 'A' && inputLetter!='B' &&
         inputLetter!= 'C' && inputLetter!='D' &&
         inputLetter!= 'E' && inputLetter!='F' &&
         inputLetter!= 'G') || inputRank<=0 )
    { std::cout << "This is a programming error. Requested to create a simple Dynkin type "
      << "of type " << inputLetter << " and rank " << inputRank << ". This is not allowed: "
      << " I only accept types A, B, C, D, E, F and G and non-negative ranks. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    if (inputLetter=='G')
      inputRank=2;
    if (inputLetter=='F')
      inputRank=4;
    if (inputLetter=='E' && inputRank>8)
      inputRank=8;
    this->theRank=inputRank;
    this->theLetter=inputLetter;
    this->lengthFirstCoRootSquared=inputLengthFirstCorRootSquared;
  }
  void GetCartanSymmetric(Matrix<Rational>& output)const;
  void GetAn(int n, Matrix<Rational>& output)const;
  void GetBn(int n, Matrix<Rational>& output)const;
  void GetCn(int n, Matrix<Rational>& output)const;
  void GetDn(int n, Matrix<Rational>& output)const;
  void GetEn(int n, Matrix<Rational>& output)const;
  void GetF4(Matrix<Rational>& output)const;
  void GetG2(Matrix<Rational>& output)const;
  bool IsPossibleCoRootLength(const Rational& input)const;
  void operator=(const DynkinSimpleType& other)
  { this->theLetter=other.theLetter;
    this->theRank=other.theRank;
    this->lengthFirstCoRootSquared=other.lengthFirstCoRootSquared;
  }
  bool operator==(const DynkinSimpleType& other)const
  { return
    this->theLetter==other.theLetter && this->theRank==other.theRank &&
    this->lengthFirstCoRootSquared==other.lengthFirstCoRootSquared;
  }
  static unsigned int HashFunction(const DynkinSimpleType& input)
  { return ((unsigned int)
    input.theLetter)*2+input.theRank
    +SomeRandomPrimes[0]*input.lengthFirstCoRootSquared.HashFunction();
  }
  unsigned int HashFunction()const
  { return this->HashFunction(*this);
  }
  inline bool IsEqualToZero()const
  { return false;
  }
  void GetAutomorphismActingOnVectorROWSwhichStandOnTheRight
  (Matrix<Rational>& output, int AutoIndex)const;
  Rational GetDefaultCoRootLengthSquared(int rootIndex)const;
  Rational GetDefaultRootLengthSquared(int rootIndex)const;
  Rational GetRatioRootSquaredToFirstSquared(int rootIndex)const;
  static Rational GetRatioLongRootToFirst(char inputWeylLetter, int inputRank);
  bool HasEasySubalgebras()const;
  Rational GetRatioLongRootToFirst()const
  { return this->GetRatioLongRootToFirst(this->theLetter, this->theRank);
  }
  std::string ToStringLinksToCalculator(FormatExpressions* theFormat=0)const;
  std::string ToString(FormatExpressions* theFormat=0)const;
  void operator++(int);
  bool operator>(const DynkinSimpleType& other)const
  { if (this->theRank>other.theRank)
      return true;
    if (this->theRank<other.theRank)
      return false;
    if ((this->theLetter=='B' ||this->theLetter=='C') && other.theLetter=='D')
      return true;
    if (this->theLetter=='D' && (other.theLetter=='B' ||other.theLetter=='C'))
      return false;
    if (this->theLetter>other.theLetter)
      return true;
    if (this->theLetter<other.theLetter)
      return false;
    return this->lengthFirstCoRootSquared>other.lengthFirstCoRootSquared;
  }
  static void GetEpsilonMatrix(char WeylLetter, int WeylRank, Matrix<Rational>& output);

  inline bool operator<(const DynkinSimpleType& other)const
  { return other>*this;
  }
  bool operator<(int otherRank)const;
};

class DynkinType: public MonomialCollection<DynkinSimpleType, Rational>
{
public:
  void GetLettersTypesMults
  (List<char>* outputLetters=0, List<int>* outputRanks=0, List<int>* outputMults=0,
   List<Rational>* outputFirstCoRootLengthsSquared=0)const;
  void GetTypesWithMults
  (List<DynkinSimpleType>& output)const;
  bool IsOfSimpleType(char inputType, int inputRank)const
  { char currentType;
    int currentRank;
    if (!this->IsSimple(&currentType, &currentRank))
      return false;
    return currentType==inputType && currentRank==inputRank;
  }
  bool IsSimple(char* outputtype=0, int* outputRank=0, Rational* outputLength=0)const
  ;
  void GetDynkinTypeWithDefaultLengths(DynkinType& output);
  DynkinSimpleType GetGreatestSimpleType()const;
  DynkinSimpleType GetSmallestSimpleType()const;
  Rational GetSizeWeylGroupByFormula()const;
  std::string ToString(FormatExpressions* theFormat=0)const
  { std::stringstream out;
    out << *this;
    return out.str();
  }
  void ScaleFirstCoRootSquaredLength(const Rational& multiplyCoRootSquaredLengthBy);
  int GetMult(int SimpleTypeIdentifier)const
  { int result;
    if(!this->theCoeffs[SimpleTypeIdentifier].IsSmallInteger(&result))
    { std::cout << "This is a programming error: Dynkin type has multiplicity that is not a small integer "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    return result;
  }
  bool IsPossibleCoRootLength(const Rational& input)const
  { for (int i=0; i<this->size; i++)
      if ((*this)[i].IsPossibleCoRootLength(input))
        return true;
    return false;
  }
  int GetNumSimpleComponents()const;
  Rational GetRankRational()const;
  int GetRank()const;
  int GetRootSystemSize()const
  { Rational result=0;
    for (int i=0; i<this->size; i++)
      result+=this->theCoeffs[i]*(*this)[i].GetRootSystemSize();
    int intResult;
    if (!result.IsSmallInteger(&intResult))
    { std::cout << "This is a programming error: multiplicity of simple type is not a small integer. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    return intResult;
  }
  int GetRootSystemPlusRank()const
  { Rational result=0;
    for (int i=0; i<this->size; i++)
      result+=this->theCoeffs[i]*(*this)[i].GetRootSystemPlusRank();
    int intResult;
    if (!result.IsSmallInteger(&intResult))
    { std::cout << "This is a programming error: multiplicity of simple type is not a small integer. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    return intResult;
  }
  void MakeSimpleType(char type, int rank, const Rational* inputFirstCoRootSqLength=0);
  void GetEpsilonMatrix(Matrix<Rational>& output)const;
  void GetCartanSymmetric(Matrix<Rational>& output)const;
  std::string GetLieAlgebraName(FormatExpressions* theFormat=0)const;
  std::string GetWeylGroupName(FormatExpressions* theFormat=0)const;
  bool HasExceptionalComponent()const;
  bool operator>(const DynkinType& other)const;
};

template <class coefficient>
class WeylGroupRepresentation
{
  private:
  List<Matrix<coefficient> > theElementImages;
  List<bool> theElementIsComputed;
  Vector<coefficient> theCharacter;
  List<Matrix<coefficient> > classFunctionMatrices;
  List<bool> classFunctionMatricesComputed;
  friend class WeylGroup;
  WeylGroupRepresentation* parent;
  Vectors<coefficient> vectorSpaceBasis;
  Matrix<coefficient> gramMatrixInverted;
  public:
  WeylGroup* OwnerGroup;
  WeylGroupRepresentation():parent(0), OwnerGroup(0){}
  unsigned int HashFunction() const;
  bool CheckInitialization()const;
  bool CheckAllSimpleGensAreOK()const;
  static unsigned int HashFunction(const WeylGroupRepresentation<coefficient>& input)
  { return input.HashFunction();
  }
  void ComputeAllGeneratorImagesFromSimple(GlobalVariables* theGlobalVariables=0);
  Vector<coefficient>& GetCharacter();
  void GetLargestDenominatorSimpleGens
  (LargeIntUnsigned& outputLCM, LargeIntUnsigned& outputDen)const;
  void reset(WeylGroup* inputOwner);
  void CheckRepIsMultiplicativelyClosed();
  void GetClassFunctionMatrix
(Vector<coefficient>& virtualCharacter, Matrix<coefficient>& outputMat,
 GlobalVariables* theGlobalVariables=0)
;
  int GetDim()const
  { if (this->theElementImages.size==1)
      return this->theElementImages[0].NumRows;
    return this->theElementImages[1].NumRows;
  }
  void Restrict
(const Vectors<coefficient>& VectorSpaceBasisSubrep, const Vector<Rational>& remainingCharacter,
 WeylGroupRepresentation<coefficient>& output, GlobalVariables* theGlobalVariables=0)
   ;
  bool DecomposeMeIntoIrrepsRecursive
  (Vector<Rational>& outputIrrepMults, GlobalVariables* theGlobalVariables=0);
  bool DecomposeMeIntoIrreps(Vector<Rational>& outputIrrepMults, GlobalVariables* theGlobalVariables=0);
  coefficient GetNumberOfComponents();
  void operator*=(const WeylGroupRepresentation<coefficient>& other);
  bool operator==(const WeylGroupRepresentation<coefficient>& other)const
  { return this->OwnerGroup==other.OwnerGroup && this->theCharacter==other.theCharacter;
  }
  void SpreadVector(const Vector<coefficient>& input, Vectors<coefficient>& outputBasisGeneratedSpace);
  std::string ToString(FormatExpressions* theFormat=0)const;
  Matrix<coefficient>& GetElementImage(int elementIndex);
  void SetElementImage(int elementIndex, const Matrix<coefficient>& input)
  { this->theElementImages[elementIndex] = input;
    this->theElementIsComputed[elementIndex] = true;
  }
  bool operator>(const WeylGroupRepresentation<coefficient>& other)const
  { return this->theCharacter.isGreaterThanLexicographic(other.theCharacter);
  }
};

class ElementWeylGroup: public List<int>
{
public:
  WeylGroup* owner;
  ElementWeylGroup():owner(0)
  {}
  bool CheckInitialization()
  { if (this->owner==0)
    { std::cout << "This is a programming error: non-initialized element Weyl group. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
      return false;
    }
    return true;
  }
  void MakeCanonical();
  void MakeCanonicalProvidedAllElementsOwnerAreComputed();
  //the two canonical forms do not coincide! This needs to be fixed, not critical however.
  void MakeCanonicalWithoutElementsOwnerComputed();
  void ToString(std::string& output)
  { output=this->ToString();
  }
  std::string ToString
  (FormatExpressions* theFormat=0, List<int>* DisplayIndicesOfSimpleRoots=0)const
  { return this->ToString(-1, theFormat, DisplayIndicesOfSimpleRoots);
  }
  std::string ToString
  (int NumSimpleGens, FormatExpressions* theFormat=0, List<int>* DisplayIndicesOfSimpleRoots=0)const
  ;
  unsigned int HashFunction() const;
  static inline unsigned int HashFunction(const ElementWeylGroup& input)
  { return input.HashFunction();
  }
  void operator*=(const ElementWeylGroup& other);
  ElementWeylGroup operator*(const ElementWeylGroup& other)
  { ElementWeylGroup result=*this;
    result*=other;
    return result;
  }
  ElementWeylGroup Inverse() const;

  bool operator==(const ElementWeylGroup& other)const
  { if (this->owner!=other.owner)
      return false;
    return this->::List<int>::operator==(other);
  }
};

class WeylGroup
{
//  Matrix<int> CartanSymmetricIntBuffer;
//  void UpdateIntBuffer()
//  { this->CartanSymmetricIntBuffer.init(this->CartanSymmetric.NumRows, this->CartanSymmetric.NumCols);
//    for (int i=0; i<this->CartanSymmetric.NumRows; i++)
//      for (int j=0; j<this->CartanSymmetric.NumCols; j++)
//        this->CartanSymmetricIntBuffer.elements[i][j]=this->CartanSymmetric.elements[i][j].NumShort;
//  }
  Matrix<Rational> FundamentalToSimpleCoords;
  Matrix<Rational> SimpleToFundamentalCoords;
  MemorySaving<Matrix<Rational> > MatrixSendsSimpleVectorsToEpsilonVectors;
  bool flagFundamentalToSimpleMatricesAreComputed;
  inline void ComputeFundamentalToSimpleMatrices()
  { if (flagFundamentalToSimpleMatricesAreComputed)
      return;
    Vectors<Rational> fundamentalBasis;
    this->GetFundamentalWeightsInSimpleCoordinates(fundamentalBasis);
    this->FundamentalToSimpleCoords.AssignVectorsToRows(fundamentalBasis);
    this->FundamentalToSimpleCoords.Transpose();
    this->SimpleToFundamentalCoords=this->FundamentalToSimpleCoords;
    this->SimpleToFundamentalCoords.Invert();
    this->flagFundamentalToSimpleMatricesAreComputed=true;
  }
public:
  DynkinType theDynkinType;
  Matrix<Rational> CartanSymmetric;
  HashedList<ElementWeylGroup> theElements;
  Vector<Rational> rho;
  HashedList<Vector<Rational> > rhoOrbit;
  HashedList<Vector<Rational> > RootSystem;
  Vectors<Rational> RootsOfBorel;

  List<List<int> > conjugacyClasses;
  List<WeylGroupRepresentation<Rational> > irreps;

  static bool flagAnErrorHasOcurredTimeToPanic;
//  void MakeFromParSel(Vector<Rational> & parSel, WeylGroup& input);
  void init()
  { this->flagFundamentalToSimpleMatricesAreComputed=false;
  }
  void ComputeConjugacyClasses(GlobalVariables* theGlobalVariables=0);
  void ComputeIrreducibleRepresentations(GlobalVariables* theGlobalVariables=0);
  void ComputeRho(bool Recompute);
  std::string ToString(FormatExpressions* theFormat=0);
  void MakeFromDynkinType(const DynkinType& inputType)
  ;
  bool CheckInitializationFDrepComputation()const;
  template <typename coefficient>
  coefficient GetHermitianProduct
  (const Vector<coefficient>& leftCharacter, const Vector<coefficient>& rightCharacter)const
  ;
  template <typename coefficient>
  inline coefficient GetCharacterNorm
  (const Vector<coefficient>& theCharacter)const
  { return this->GetHermitianProduct(theCharacter, theCharacter);
  }
  void GetSignCharacter(Vector<Rational>& out);
  void StandardRepresentation(WeylGroupRepresentation<Rational>& output);
  void MakeArbitrarySimple(char WeylGroupLetter, int n, const Rational* firstCoRootLengthSquared=0);
  void GenerateAdditivelyClosedSubset(Vectors<Rational>& input, Vectors<Rational>& output);
  Rational GetKillingDivTraceRatio();
  Rational EstimateNumDominantWeightsBelow(Vector<Rational> & inputHWsimpleCoords, GlobalVariables& theGlobalVariables);
  bool ContainsARootNonStronglyPerpendicularTo(Vectors<Rational>& theVectors, Vector<Rational>& input)
 ;
  int NumRootsConnectedTo(Vectors<Rational>& theVectors, Vector<Rational>& input)
  ;
    void GetHighestWeightsAllRepsDimLessThanOrEqualTo
  (List<Vector<Rational> >& outputHighestWeightsFundCoords, int inputDimBound
   )
   ;
  Rational GetLongestRootLengthSquared();
  static unsigned int HashFunction(const WeylGroup& input)
  { return input.CartanSymmetric.HashFunction();
  }
  unsigned int HashFunction()const
  { return this->HashFunction(*this);
  }
  WeylGroup();
  bool IsOfSimpleType(char desiredType, int desiredRank)const
  { return this->theDynkinType.IsOfSimpleType(desiredType, desiredRank);
  }
  Matrix<Rational>* GetMatrixFundamentalToSimpleCoords()
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
  Vectors<CoefficientType> GetSimpleCoordinatesFromFundamental
  (const Vectors<CoefficientType>& inputInFundamentalCoords)
  ;
  template<class CoefficientType>
  Vector<CoefficientType> GetFundamentalCoordinatesFromSimple
  (const Vector<CoefficientType>& inputInSimpleCoords)
  ;
  template<class CoefficientType>
  Vector<CoefficientType> GetDualCoordinatesFromSimple
  (const Vector<CoefficientType>& inputInSimpleCoords)
  { return this->GetDualCoordinatesFromFundamental(this->GetFundamentalCoordinatesFromSimple(inputInSimpleCoords));
  }
  template <class CoefficientType>
  Vector<CoefficientType> GetDualCoordinatesFromFundamental
  (const Vector<CoefficientType>& inputInFundamentalCoords)
  { Vector<CoefficientType> result=inputInFundamentalCoords;
    if (result.size!=this->GetDim())
    { std::cout << "This is a programming error. The input fundamental weight has " << result.size
      << " coordinates, while the rank of the Weyl group is " << this->GetDim() << ". "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    for (int i=0; i<result.size; i++)
      result[i]*=this->CartanSymmetric.elements[i][i]/2;
    return result;
  }
  template <class CoefficientType>
  CoefficientType GetScalarProdSimpleRoot(const Vector<CoefficientType>& input, int indexSimpleRoot)
  { if (indexSimpleRoot<0 || indexSimpleRoot>=this->GetDim())
    { std::cout << "This is a programming error. Attempting to take scalar product with simple root of index "
      << indexSimpleRoot
      << " which is impossible, as the rank of the Weyl group is " << this->GetDim() << ". "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    CoefficientType result, buffer;
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
  (Vector<CoefficientType>& theWeight, int* sign=0, bool* stabilizerFound=0,
   ElementWeylGroup* raisingElt=0)
  ;
  template <class CoefficientType>
  void RaiseToMaximallyDominant
  (List<Vector<CoefficientType> >& theWeight)const
  ;
  void GetCoxeterPlane
  (Vector<double>& outputBasis1, Vector<double>& outputBasis2, GlobalVariables& theGlobalVariables)
;
  void GetSimpleReflectionMatrix(int indexSimpleRoot, Matrix<Rational>& output)const;
  void DrawRootSystem
(DrawingVariables& outputDV, bool wipeCanvas, GlobalVariables& theGlobalVariables,
 bool drawWeylChamber, Vector<Rational> * bluePoint=0, bool LabelDynkinDiagramVertices=false,
 Vectors<Rational>* predefinedProjectionPlane=0)
  ;
  bool HasStronglyPerpendicularDecompositionWRT
(Vector<Rational>& input, int UpperBoundNumBetas, Vectors<Rational>& theSet, Vectors<Rational>& output,
 List<Rational>& outputCoeffs, bool IntegralCoefficientsOnly)
 ;
//  void MakeFromDynkinType(List<char>& theLetters, List<int>& theRanks, List<int>* theMultiplicities);
//  void MakeFromDynkinType(List<char>& theLetters, List<int>& theRanks){ this->MakeFromDynkinType(theLetters, theRanks, 0); }
  //void GetLongRootLength(Rational& output);
  static bool IsAddmisibleDynkinType(char candidateLetter, int n);
  //the below will not do anything if the inputLetter is not a valid Dynkin letter
  static void TransformToAdmissibleDynkinType(char inputLetter, int& outputRank);
  void GetEpsilonCoords(const Vector<Rational>& input, Vector<Rational>& output);
  void GetEpsilonCoords(const List<Vector<Rational> >& input, Vectors<Rational>& output);
  bool IsStronglyPerpendicularTo(const Vector<Rational>& input, const Vector<Rational>& other);
  bool IsStronglyPerpendicularTo(const Vector<Rational>& input, const Vectors<Rational>& others);
  void GetEpsilonCoordsWRTsubalgebra(Vectors<Rational>& generators, List<Vector<Rational> >& input, Vectors<Rational>& output);
  bool LeftIsHigherInBruhatOrderThanRight(ElementWeylGroup& left, ElementWeylGroup& right);
  void GetMatrixReflection(Vector<Rational>& reflectionRoot, Matrix<Rational>& output);
  template <class CoefficientType>
  bool GetAlLDominantWeightsHWFDIM
(Vector<CoefficientType> & highestWeightSimpleCoords, HashedList<Vector<CoefficientType> >& outputWeightsSimpleCoords,
 int upperBoundDominantWeights, std::string* outputDetails, GlobalVariables* theGlobalVariables)
  ;
template <class CoefficientType>
  bool FreudenthalEval
  (Vector<CoefficientType>& inputHWfundamentalCoords, HashedList<Vector<CoefficientType> >& outputDominantWeightsSimpleCoords,
   List<CoefficientType>& outputMultsSimpleCoords, std::string* outputDetails,
   GlobalVariables* theGlobalVariables, int UpperBoundFreudenthal)
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
  void ComputeAllElements(int UpperLimitNumElements=0);
  void ComputeWeylGroupAndRootsOfBorel(Vectors<Rational>& output);
  void ComputeRootsOfBorel(Vectors<Rational>& output);
  static Rational GetSizeWeylGroupByFormula(char weylLetter, int theDim);
  bool IsARoot(const Vector<Rational>& input)const
  { return this->RootSystem.Contains(input);
  }
  void GenerateRootSubsystem(Vectors<Rational>& theRoots);
  template <class CoefficientType>
  bool GenerateOrbit
  (Vectors<CoefficientType>& theRoots, bool RhoAction, HashedList<Vector<CoefficientType> >& output,
   bool UseMinusRho, int expectedOrbitSize=-1, HashedList<ElementWeylGroup>* outputSubset=0,
   int UpperLimitNumElements=-1)
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
  (Vector<Rational> & inputOutput, ElementWeylGroup* outputWeylElt,
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
  void ActOn(const ElementWeylGroup& theGroupElement, Vector<Element>& theVector)const
  { for (int i=0; i<theGroupElement.size; i++)
      this->SimpleReflection(theGroupElement[i], theVector);
  }
  template <class Element>
  void ActOnRhoModified(const ElementWeylGroup& theGroupElement, Vector<Element>& theVector)const
  { for (int i=0; i<theGroupElement.size; i++)
      this->SimpleReflectionRhoModified(theGroupElement[i], theVector);
  }
  template <class Element>
  void ActOnRhoModified(int indexOfWeylElement, Vector<Element>& theVector)const
  { this->ActOnRhoModified(this->theElements[indexOfWeylElement], theVector);
  }
  template <class Element>
  void ActOn(int indexOfWeylElement, Vector<Element>& theVector)const
  { this->ActOn(this->theElements[indexOfWeylElement], theVector);
  }
  template <class Element>
  void ActOnDual(int index,Vector<Element>& theVector, bool RhoAction, const Element& theRingZero);
  //theRoot is a list of the simple coordinates of the Vector<Rational>
  //theRoot serves as both input and output
  void ActOnRootAlgByGroupElement(int index, PolynomialSubstitution<Rational>& theRoot, bool RhoAction);
  void ActOnRootByGroupElement(int index, Vector<Rational>& theRoot, bool RhoAction, bool UseMinusRho);
  void ActOnDualSpaceElementByGroupElement(int index, Vector<Rational>& theDualSpaceElement, bool RhoAction);
  void SimpleReflectionRoot(int index, Vector<Rational>& theRoot, bool RhoAction, bool UseMinusRho);
  template <class Element>
  void SimpleReflection
  (int index, Vector<Element>& theVector)const;
  template <class Element>
  void SimpleReflectionRhoModified
  (int index, Vector<Element>& theVector)const;
  template <class Element>
  void SimpleReflectionMinusRhoModified
  (int index, Vector<Element>& theVector)const;
  int GetRootReflection(int rootIndex);
  void GetMatrixOfElement(int theIndex, Matrix<Rational>& outputMatrix)const ;
  void GetMatrixOfElement(const ElementWeylGroup& input, Matrix<Rational>& outputMatrix)const;
  void GetElementOfMatrix(Matrix<Rational>& input, ElementWeylGroup &output);
  void SimpleReflectionDualSpace(int index, Vector<Rational>& DualSpaceElement);
  void SimpleReflectionRootAlg(int index, PolynomialSubstitution<Rational>& theRoot, bool RhoAction);
  bool IsPositiveOrPerpWRTh(const Vector<Rational>& input, const Vector<Rational>& theH)
  { return this->RootScalarCartanRoot(input, theH).IsPositiveOrZero();
  }
  template<class CoefficientType>
  void ReflectBetaWRTAlpha
  (const Vector<Rational>& alpha, const Vector<CoefficientType>& Beta, bool RhoAction,
   Vector<CoefficientType>& Output)const;
  bool IsRegular(Vector<Rational>& input, int* indexFirstPerpendicularRoot);
  template <class leftType, class rightType>
  void RootScalarCartanRoot
  (const Vector<leftType>& r1, const Vector<rightType>& r2, leftType& output)const;
  double RootScalarCartanRoot(const Vector<double>& r1, const Vector<double>& r2)const
  { if (r1.size!=r2.size || r1.size!=this->GetDim())
    { std::cout << "This is a programming error: attempting to take the root system scalar product of "
      << "vectors of different dimension or of dimension different from that of the ambient Lie algebra. "
      << "The two input vectors were " << r1.ToString() << " and " << r2.ToString()
      << " and the rank of the Weyl group is " << this->GetDim() << ". "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    double result=0;
    for (int i=0; i<this->GetDim(); i++)
      for (int j=0; j<this->GetDim(); j++)
        result+=this->CartanSymmetric.elements[i][j].DoubleValue()*r1.TheObjects[i]*r2.TheObjects[j];
    return result;
  }
  template <class leftType, class rightType>
  leftType RootScalarCartanRoot(const Vector<leftType>& r1, const Vector<rightType>& r2)const
  { leftType tempRat;
    this->RootScalarCartanRoot(r1, r2, tempRat);
    return tempRat;
  }
  //the below functions perturbs input so that inputH has non-zero scalar product with Vectors<Rational> of the Vector<Rational>  system,
  //without changing the inputH-sign of any Vector<Rational>  that had a non-zero scalar product to begin with
  void PerturbWeightToRegularWRTrootSystem(const Vector<Rational>& inputH, Vector<Rational>& output);
  template <class CoefficientType>
  bool IsDominantWRTgenerator(const Vector<CoefficientType>& theWeight, int generatorIndex)
;
  template <class CoefficientType>
  bool IsDominantWeight(const Vector<CoefficientType>& theWeight)
;
  void TransformToSimpleBasisGenerators(Vectors<Rational>& theGens);
  void TransformToSimpleBasisGeneratorsWRTh(Vectors<Rational>& theGens, const Vector<Rational>& theH);
  bool operator==(const WeylGroup& other)const;
  void operator+=(const WeylGroup& other);
};

template <class Element>
void WeylGroup::SimpleReflectionRhoModified
(int index, Vector<Element>& theVector)const
{ Element alphaShift, tempRat;
  alphaShift=0;
  for (int i=0; i<this->CartanSymmetric.NumCols; i++)
  { tempRat=(theVector[i]);
    tempRat*=this->CartanSymmetric.elements[index][i]*(-2);
    alphaShift+=(tempRat);
  }
  alphaShift/=(this->CartanSymmetric.elements[index][index]);
  alphaShift-=1;
  theVector[index]+=(alphaShift);
}

template <class Element>
void WeylGroup::SimpleReflectionMinusRhoModified
  (int index, Vector<Element>& theVector)const
{ Element alphaShift, tempRat;
  alphaShift=0;
  for (int i=0; i<this->CartanSymmetric.NumCols; i++)
  { tempRat=(theVector[i]);
    tempRat*=(this->CartanSymmetric.elements[index][i]*(-2));
    alphaShift+=(tempRat);
  }
  alphaShift/=(this->CartanSymmetric.elements[index][index]);
  alphaShift+=(1);
  theVector[index]+=(alphaShift);
}

template <class Element>
void WeylGroup::SimpleReflection
(int index, Vector<Element>& theVector)const
{ Element alphaShift, tempRat;
  alphaShift=0;
  for (int i=0; i<this->CartanSymmetric.NumCols; i++)
  { tempRat=(theVector[i]);
    tempRat*=(this->CartanSymmetric.elements[index][i]*(-2));
    alphaShift+=(tempRat);
  }
  alphaShift/=(this->CartanSymmetric.elements[index][index]);
  theVector[index]+=(alphaShift);
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
  // For a fixed external automorphism (of type Vectors<Rational>) the i^th entry gives the image
  //of the simple root  with 1 on the i^th coordinate
  List<Vectors<Rational> > ExternalAutomorphisms;
  HashedList<Vector<Rational> > RootSubsystem;
  Vectors<Rational> RootsOfBorel;
  void ToString(std::string& output, bool displayElements);
  void GetGroupElementsIndexedAsAmbientGroup
  (List<ElementWeylGroup>& output);
  std::string ElementToStringBruhatGraph();
  std::string ElementToStringCosetGraph();
  std::string ElementToStringFromLayersAndArrows
  (List<List<List<int> > >& arrows, List<List<int> >& Layers, int GraphWidth, bool useAmbientIndices)
  ;
  std::string ToString(bool displayElements=true){std::string tempS; this->ToString(tempS, displayElements); return tempS;}
  Vector<Rational> GetRho();
  template <class CoefficientType>
  void RaiseToDominantWeight
  (Vector<CoefficientType>& theWeight, int* sign=0, bool* stabilizerFound=0)
  ;
  template <class CoefficientType>
  bool FreudenthalEvalIrrepIsWRTLeviPart
(Vector<CoefficientType>& inputHWfundamentalCoords, HashedList<Vector<CoefficientType> >& outputDominantWeightsSimpleCoords,
 List<CoefficientType>& outputMultsSimpleCoordS, std::string& outputDetails,
 GlobalVariables& theGlobalVariables, int UpperBoundFreudenthal)
  ;
  bool MakeParabolicFromSelectionSimpleRoots
(WeylGroup& inputWeyl, const Selection& ZeroesMeanSimpleRootSpaceIsInParabolic, GlobalVariables& theGlobalVariables, int UpperLimitNumElements)
  ;
  void MakeParabolicFromSelectionSimpleRoots
(WeylGroup& inputWeyl, const Vector<Rational>& ZeroesMeanSimpleRootSpaceIsInParabolic, GlobalVariables& theGlobalVariables, int UpperLimitNumElements)
;
  bool GetAlLDominantWeightsHWFDIMwithRespectToAmbientAlgebra
  (Vector<Rational>& highestWeightSimpleCoords, HashedList<Vector<Rational> >& outputWeightsSimpleCoords,
 int upperBoundDominantWeights, std::string& outputDetails, GlobalVariables& theGlobalVariables)
 ;
  template <class CoefficientType>
  bool GetAlLDominantWeightsHWFDIM
  (Vector<CoefficientType>& highestWeightSimpleCoords, HashedList<Vector<CoefficientType> >& outputWeightsSimpleCoords,
 int upperBoundDominantWeights, std::string& outputDetails, GlobalVariables& theGlobalVariables)
 ;
  bool DrawContour
(const Vector<Rational>& highestWeightSimpleCoord, DrawingVariables& theDV, GlobalVariables& theGlobalVariables, int theColor,
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
  template <class CoefficientType>
  CoefficientType WeylDimFormulaSimpleCoords
  (const Vector<CoefficientType>& theWeightInSimpleCoords, const CoefficientType& theRingUnit=1)
  ;
  void FindQuotientRepresentatives(int UpperLimit);
  void GetMatrixOfElement(const ElementWeylGroup& input, Matrix<Rational>& outputMatrix)const;
  template <class CoefficientType>
  bool GenerateOrbitReturnFalseIfTruncated
  (const Vector<CoefficientType>& input, Vectors<CoefficientType>& outputOrbit,
   int UpperLimitNumElements);
  bool ComputeSubGroupFromGeneratingReflections
  (Vectors<Rational>* inputGenerators, List<Vectors<Rational> >* inputExternalAutos,
   GlobalVariables* theGlobalVariables, int UpperLimitNumElements,
   bool recomputeAmbientRho);
  void ComputeRootSubsystem();
  template <class CoefficientType>
  void ActByElement(int index, Vector<CoefficientType>& theRoot)const;
  template <class CoefficientType>
  void ActByElement(int index, Vector<CoefficientType>& input, Vector<CoefficientType>& output)const
  { this->ActByElement(this->TheObjects[index], input, output);
  }
  template <class CoefficientType>
  void ActByElement
  (const ElementWeylGroup& theElement, const Vector<CoefficientType>& input, Vector<CoefficientType>& output)const;
  template <class CoefficientType>
  void ActByElement(int index, const Vectors<CoefficientType>& input, Vectors<CoefficientType>& output)const
  { this->ActByElement(this->TheObjects[index], input, output);
  }
  template <class CoefficientType>
  void ActByElement
  (const ElementWeylGroup& theElement, const Vectors<CoefficientType>& input, Vectors<CoefficientType>& output)const;
  void WriteToFile(std::fstream& output, GlobalVariables* theGlobalVariables);
  void ReadFromFile(std::fstream& input, GlobalVariables* theGlobalVariables);
  void operator=(const ReflectionSubgroupWeylGroup& other);
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
  void ToString(std::string& output, rootSubalgebra& owner);
  void ToString(std::string& output, bool useLaTeX, bool useHtml, rootSubalgebra& owner);
  void ComputeDebugString(rootSubalgebra& owner){ this->ToString(this->DebugString, owner); }
};

class DynkinDiagramRootSubalgebra
{
public:
//  std::string DynkinStrinG;
  List<Vectors<Rational> > SimpleBasesConnectedComponents;
  //to each connected component of the simple bases corresponds
  //its dynkin string with the same index
  List<DynkinSimpleType> SimpleComponentTypes;
  List<int> indicesThreeNodes;
  List<List<int> > indicesEnds;
  List<List<int> > sameTypeComponents;
  List<int> indexUniComponent;
  List<int> indexInUniComponent;
  std::string ToString
  (bool IncludeAlgebraNames=false)const;
  int RankTotal();
  int NumRootsGeneratedByDiagram();
  void Sort();
  void SwapDynkinStrings(int i, int j);
  bool LetterIsDynkinGreaterThanLetter(char letter1, char letter2);
  //the below function takes as an input a set of roots and computes the corredponding Dynkin diagram of the
  //root  subsystem. Note: the simleBasisInput is required to be a set of simple roots.
  //The procedure calls a
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
  int numberOfThreeValencyNodes(int indexComponent, WeylGroup& theWeyl);
  void Assign(const DynkinDiagramRootSubalgebra& right);
  inline void operator=(const DynkinDiagramRootSubalgebra& right){this->Assign(right); }
  bool operator==(const DynkinDiagramRootSubalgebra& right) const;
  bool IsGreaterThan(DynkinDiagramRootSubalgebra& right);
  Rational GetSizeCorrespondingWeylGroupByFormula();
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
  int ToString(std::string& output, rootSubalgebras& owners, bool useLatex, bool includeScalarsProductsEachSide, bool includeMixedScalarProducts);
  int RootsToScalarProductString(Vectors<Rational>& inputLeft, Vectors<Rational>& inputRight, const std::string& letterTypeLeft, const std::string& letterTypeRight, std::string& output, bool useLatex, rootSubalgebra& owner);
  void ComputeConnectedComponents(Vectors<Rational>& input, rootSubalgebra& owner, List<List<int> >& output);
  void ComputeDebugString(rootSubalgebras& owner, bool includeScalarsProducts, bool includeMixedScalarProducts){ this->ToString(this->DebugString, owner, true, includeScalarsProducts, includeMixedScalarProducts);  }
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
  { this->Alphas=(right.Alphas);
    this->Betas=(right.Betas);
    this->AlphaCoeffs=(right.AlphaCoeffs);
    this->BetaCoeffs=(right.BetaCoeffs);
    this->AlphaKComponents=(right.AlphaKComponents);
    this->BetaKComponents=(right.BetaKComponents);
    this->theDiagram=right.theDiagram;
    this->theDiagramRelAndK=right.theDiagramRelAndK;
    this->IndexOwnerRootSubalgebra=right.IndexOwnerRootSubalgebra;
    this->DebugString= right.DebugString;
  }
  bool operator==(const coneRelation& right){ return this->DebugString==right.DebugString; }
  unsigned int HashFunction() const
  { int tempI= ::MathRoutines::Minimum((int) this->DebugString.length(), ::SomeRandomPrimesSize);
    unsigned int result=0;
    for (int i=0; i<tempI; i++)
      result+= this->DebugString[i]*::SomeRandomPrimes[i];
    return result;
  }
  static inline unsigned int HashFunction(const coneRelation& input)
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
  void ToString
(std::string& output, rootSubalgebras& owners, bool useLatex, bool useHtml, std::string* htmlPathPhysical,
 std::string* htmlPathServer, GlobalVariables& theGlobalVariables, const std::string& DisplayNameCalculator)
  ;
  void ComputeDebugString(rootSubalgebras& owners, std::string* htmlPathPhysical, std::string* htmlPathServer, GlobalVariables& theGlobalVariables)
  { this->ToString (this->DebugString, owners, true, false, htmlPathPhysical, htmlPathServer, theGlobalVariables, "");
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
  List<Vectors<Rational> > kModulesKepsCoords;
  List<Vectors<Rational> > kModulesgEpsCoords;
  List<Vectors<Rational> > kModules;
  List<Vectors<Rational> > PosRootsKConnectedComponents;
  List<Selection> theKEnumerations;
  List<int> theKComponentRanks;
  SemisimpleLieAlgebra* owneR;
//  int indexInOwners;
  rootSubalgebra();
  WeylGroup& GetAmbientWeyl();
  SemisimpleLieAlgebra& GetOwnerSSalg();
  //returns -1 if the weight/root  is not in g/k
  int GetIndexKmoduleContainingRoot(Vector<Rational>& input);
  void GetCoxeterPlane
  (Vector<double>& outputBasis1, Vector<double>& outputBasis2, GlobalVariables& theGlobalVariables)
  ;
  void GetCoxeterElement(Matrix<Rational>& output);
  bool IsGeneratingSingularVectors(int indexKmod, Vectors<Rational>& NilradicalRoots);
  bool rootIsInCentralizer(Vector<Rational>& input);
  bool IsBKhighest(Vector<Rational>& input);
  bool rootIsInNilradicalParabolicCentralizer
  (Selection& positiveSimpleRootsSel, Vector<Rational>& input);
  void ComputeEpsCoordsWRTk(GlobalVariables& theGlobalVariables);
  bool AttemptTheTripleTrick(coneRelation& theRel, Vectors<Rational>& NilradicalRoots, GlobalVariables& theGlobalVariables);
  bool AttemptTheTripleTrickWRTSubalgebra(coneRelation& theRel, Vectors<Rational>& highestWeightsAllowed, Vectors<Rational>& NilradicalRoots, GlobalVariables& theGlobalVariables);
  void ExtractRelations(Matrix<Rational>& matA, Matrix<Rational>& matX, Vectors<Rational>& NilradicalRoots, rootSubalgebras& owner, int indexInOwner, GlobalVariables& theGlobalVariables, Vectors<Rational>& Ksingular);
  bool GenerateIsomorphismsPreservingBorel(rootSubalgebra& right, GlobalVariables& theGlobalVariables, ReflectionSubgroupWeylGroup* outputAutomorphisms, bool actOnCentralizerOnly);
  void GenerateAutomorphismsPreservingBorel(GlobalVariables& theGlobalVariables, ReflectionSubgroupWeylGroup& outputAutomorphisms);
  void MakeGeneratingSingularVectors(coneRelation& theRelation, Vectors<Rational>& nilradicalRoots);
  bool attemptExtensionToIsomorphismNoCentralizer(Vectors<Rational>& Domain, Vectors<Rational>& Range, GlobalVariables& theGlobalVariables, int RecursionDepth, ReflectionSubgroupWeylGroup* outputAutomorphisms, bool GenerateAllpossibleExtensions, bool* abortKmodule, Vectors<Rational>* additionalDomain, Vectors<Rational>* additionalRange);
  static bool attemptExtensionToIsomorphism
  (Vectors<Rational>& Domain, Vectors<Rational>& Range,
   GlobalVariables& theGlobalVariables, ReflectionSubgroupWeylGroup* outputAutomorphisms,
   bool actOnCentralizerOnly, SemisimpleLieAlgebra& inputOwner,
   bool* DomainAndRangeGenerateNonIsoSAs)
  ;
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
  void MakeProgressReportPossibleNilradicalComputation(GlobalVariables* theGlobalVariables, rootSubalgebras& owner, int indexInOwner);
  void MakeProgressReportGenAutos(int progress, int outOf, int found, GlobalVariables& theGlobalVariables);
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
  std::string ToString(FormatExpressions* theFormat=0, GlobalVariables* theGlobalVariables=0);
  void ToString(std::string& output, GlobalVariables* theGlobalVariables)
  { this->ToString(output, false, false, false, theGlobalVariables);
  }
  void ElementToHtml(int index, std::string& path, SltwoSubalgebras* sl2s, GlobalVariables* theGlobalVariables);
  void ToString
  (std::string& output, bool useLatex, bool useHtml, bool includeKEpsCoords,
   GlobalVariables* theGlobalVariables)
  { this->ToString(output, 0, 0, useLatex, useHtml, includeKEpsCoords, theGlobalVariables);
  }
  void ToString
  (std::string& output, SltwoSubalgebras* sl2s, int indexInOwner, bool useLatex, bool useHtml,
   bool includeKEpsCoords, GlobalVariables* theGlobalVariables);
  bool RootsDefineASubalgebra(Vectors<Rational>& theRoots);
  void GenerateKmodMultTable
  (List<List<List<int> > >& output, List<int>& oppositeKmods,
   GlobalVariables* theGlobalVariables)
  ;
  void KmodTimesKmod(int index1, int index2, List<int>& oppositeKmods, List<int>& output);
  void init(SemisimpleLieAlgebra& inputOwner);
  void GetSsl2SubalgebrasAppendListNoRepetition
(SltwoSubalgebras& output, int indexInContainer, GlobalVariables& theGlobalVariables)
  ;
  void ComputeAllButAmbientWeyl();
  void ComputeDynkinDiagramKandCentralizer();
  void ComputeAll();
  void ComputeRootsOfK();
  void ComputeKModules();
  void ComputeHighestWeightInTheSameKMod(const Vector<Rational>& input, Vector<Rational>& outputHW);
  void ComputeExtremeWeightInTheSameKMod(const Vector<Rational>& input, Vector<Rational>& outputW, bool lookingForHighest);
  inline void operator=(const rootSubalgebra& right)
  { this->Assign(right);
  }
  void Assign(const rootSubalgebra& right);
  void ComputeLowestWeightInTheSameKMod(const Vector<Rational>& input, Vector<Rational>& outputLW);
  void GetLinearCombinationFromMaxRankRootsAndExtraRoot(bool DoEnumeration, GlobalVariables& theGlobalVariables);
//  void commonCodeForGetLinearCombinationFromMaxRankRootsAndExtraRoot();
  void initForNilradicalGeneration();
  void GetLinearCombinationFromMaxRankRootsAndExtraRootMethod2(GlobalVariables& theGlobalVariables);
  bool LinCombToString(Vector<Rational>& alphaRoot, int coeff, Vector<Rational>& linComb, std::string& output);
  bool LinCombToStringDistinguishedIndex(int distinguished, Vector<Rational>& alphaRoot, int coeff, Vector<Rational> & linComb, std::string& output);
  void WriteMultTableAndOppositeKmodsToFile(std::fstream& output, List<List<List<int> > >& inMultTable, List<int>& inOpposites);
  void ReadMultTableAndOppositeKmodsFromFile(std::fstream& input, List<List<List<int> > >& outMultTable, List<int>& outOpposites);
};

class rootSubalgebras: public List<rootSubalgebra>
{
public:
  coneRelations theBadRelations;
  coneRelations theGoodRelations;
  coneRelations theMinRels;
  List<List<int> > ActionsNormalizerCentralizerNilradical;
  List<ReflectionSubgroupWeylGroup> CentralizerOuterIsomorphisms;
  List<ReflectionSubgroupWeylGroup> CentralizerIsomorphisms;
  //Code used in nilradical generation:
  List<Selection> ImpiedSelectionsNilradical;
  List<List<List<int> > > storedNilradicals;
  SemisimpleLieAlgebra* owneR;
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
  SemisimpleLieAlgebra& GetOwnerSSalgebra();
  WeylGroup& GetOwnerWeyl();
  std::string ToString();
  void ComputeKmodMultTables(GlobalVariables* theGlobalVariables);
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
  void ElementToHtml
(std::string& header, std::string& pathPhysical, std::string& htmlPathServer,
 SltwoSubalgebras* Sl2s, const std::string& calculatorDisplayName, GlobalVariables* theGlobalVariables)
;
  void ElementToStringCentralizerIsomorphisms(std::string& output, bool useLatex, bool useHtml, int fromIndex, int NumToProcess, GlobalVariables& theGlobalVariables);
  void ToString
(std::string& output, SltwoSubalgebras* sl2s, bool useLatex, bool useHtml, bool includeKEpsCoords,
 std::string* htmlPathPhysical, std::string* htmlPathServer,
 GlobalVariables* theGlobalVariables, const std::string& DisplayNameCalculator)
  ;
  void ComputeLProhibitingRelations(GlobalVariables& theGlobalVariables);
  void ComputeAllRootSubalgebrasUpToIso(GlobalVariables& theGlobalVariables, int StartingIndex, int NumToBeProcessed);
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
    this->owneR=0;
    this->initForNilradicalGeneration();
  }
};

class VectorPartition
{
public:
  Vectors<Rational> PartitioningRoots;
  Vector<Rational> theRoot;
  //format: each element of thePartitions gives an array whose entries give
  // the multiplicity of the weights. I.e. if PartitioningRoots has 2 elements, then thePartitions[0]
  // would have 2 elements: the first giving the multiplicity of PartitioningRoots[0]
  //and the second - the multiplicity of
  // PartitioningRoots[1]
  List<List<int> > thePartitions;
  std::string DebugString;
  std::string ToString(bool useHtml);
  void ComputeDebugString(bool useHtml)
  { this->DebugString=this->ToString(useHtml);
  }
  int ComputeVectorPartitionFunctionSmall(Vector<Rational> & theRoot, Vectors<Rational>& theRoots);
  void ComputeAllPartitions();
  void ComputeAllPartitionsRecursive(int currentIndex, List<int>& CurrentPartition, int UpperBoundEachIndex, Vector<Rational> & toBePartitioned);
};

class SemisimpleLieAlgebra
{
public:
  bool flagAnErrorHasOccurredTimeToPanic;
  WeylGroup theWeyl;
  //format:
  //the Chevalley constants are listed in the same order as the root system of the Weyl group
  // i.e. if \alpha is the root at the i^th position in this->theWyl.RootSystem and \beta -
  //the root  at the j^th position, then
  //the chevalley constant N_{\alpha\beta} given by [g^\alpha, g^\beta]=N_{\alpha\beta}g^{\alpha+\beta}
  //will be located at the ij^{th} entry in the below matrix.
  //Let $\alpha$ be a root . Then our choice of the elements of the Cartan subalgebra is such that
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
  Matrix<ElementSemisimpleLieAlgebra<Rational> > theLiebrackets;
//  List<int> OppositeRootSpaces;
  List<int> UEGeneratorOrderIncludingCartanElts;
  unsigned int HashFunction()const
  { return this->HashFunction(*this);
  }
  static unsigned int HashFunction(const SemisimpleLieAlgebra& input)
  { return input.theWeyl.HashFunction();
  }
  void Assign(const SemisimpleLieAlgebra& other)
  { //this->owner=other.owner;
    //this->indexInOwner=other.indexInOwner;
    this->theWeyl=(other.theWeyl);
    this->ChevalleyConstants=other.ChevalleyConstants;
    this->theLiebrackets=(other.theLiebrackets);
//    this->theLiebracketPairingIndices=(other.theLiebracketPairingIndices);
    this->Computed=(other.Computed);
    this->UEGeneratorOrderIncludingCartanElts=other.UEGeneratorOrderIncludingCartanElts;
  }
  int GetOppositeGeneratorIndex(int theIndex)
  { return this->GetNumGenerators()-theIndex-1;
  }
  bool IsASimpleGenerator(int generatorIndex)
  { return
    (generatorIndex< this->GetNumPosRoots() &&
     generatorIndex>=this->GetNumPosRoots()-this->GetRank())
    ||
    (generatorIndex>=this->GetNumPosRoots()+this->GetRank() &&
     generatorIndex<this->GetNumPosRoots()+this->GetRank()*2
     );
  }
  SemisimpleLieAlgebra(){ }
  template <class CoefficientType>
  void GenerateLieSubalgebra(List<ElementSemisimpleLieAlgebra<CoefficientType> >& inputOutputGenerators);
  void ComputeMultTable(GlobalVariables& theGlobalVariables);
  bool IsOfSimpleType(char desiredType, int desiredRank)const
  { return this->theWeyl.IsOfSimpleType(desiredType, desiredRank);
  }
  template <class CoefficientType>
  void GetCommonCentralizer
  (const List<ElementSemisimpleLieAlgebra<CoefficientType> >& inputElementsToCentralize,
   List<ElementSemisimpleLieAlgebra<CoefficientType> >& outputCentralizingElements)
   ;
  void GetChevalleyGeneratorAsLieBracketsSimpleGens
  (int generatorIndex, List<int>& outputIndicesFormatAd0Ad1Ad2etc,
   Rational& outputMultiplyLieBracketsToGetGenerator)
  ;
  std::string ToString(FormatExpressions* inputFormat=0);
  std::string GetStringFromChevalleyGenerator
  (int theIndex, FormatExpressions* thePolynomialFormat)const
  ;
  bool CommutatorIsNonZero(int leftIndex, int rightIndex)
  { return !this->theLiebrackets.elements[leftIndex][rightIndex].IsEqualToZero();
  }
  std::string GetLieAlgebraName()const
  { return this->theWeyl.theDynkinType.GetLieAlgebraName();
  }
  void GetMinusTransposeAuto(const ElementSemisimpleLieAlgebra<Rational>& input, ElementSemisimpleLieAlgebra<Rational>& output);
  void GenerateWeightSupportMethod2(Vector<Rational> & theHighestWeight, Vectors<Rational>& output, GlobalVariables& theGlobalVariables);
  inline int GetNumGenerators()const{ return this->theWeyl.CartanSymmetric.NumRows+this->theWeyl.RootSystem.size;}
  inline int GetNumPosRoots()const{ return this->theWeyl.RootsOfBorel.size;}
  inline int GetRank()const{ return this->theWeyl.CartanSymmetric.NumRows;}
  void OrderSetNilradicalNegativeMost(Selection& parSelZeroMeansLeviPart)
  { Vector<Rational> tempVect;
    tempVect=parSelZeroMeansLeviPart;
    for (int i=0; i<this->GetNumGenerators(); i++)
    { Rational translationCoeff=this->GetWeightOfGenerator(i).ScalarEuclidean(tempVect)* this->GetNumPosRoots();
      this->UEGeneratorOrderIncludingCartanElts[i]=i+translationCoeff.NumShort;
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
  int GetCartanIndexFromSimpleGeneratorIndex(int theIndex)
  { return this->theWeyl.RootsOfBorel.size+theIndex;
  }
  int GetGeneratorFromRoot(const Vector<Rational>& input)
  { return this->GetGeneratorFromRootIndex(this->theWeyl.RootSystem.GetIndex(input));
  }
  int GetRootIndexFromDisplayIndex(int theIndex);
  int GetGeneratorFromDisplayIndex(int theIndex)
  { if (theIndex<0)
      return theIndex+this->GetNumPosRoots();
    return theIndex+this->GetNumPosRoots()+this->GetRank()-1;
  }
  int GetGeneratorFromRootIndex(int theIndex)const;
  int GetDisplayIndexFromRootIndex(int theIndex)const;
  //the below function returns an negative number if the chevalley generator is an element of the Cartan subalgebra
  int GetRootIndexFromGenerator(int theIndex)const;
  int GetCartanIndexFromGenerator(int theIndex)
  { return theIndex+this->theWeyl.RootsOfBorel.size;
  }
  int GetDisplayIndexFromGenerator(int theIndex)const
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
    int left  = this->GetRootIndexFromGenerator(leftIndex);
    int right=this->GetRootIndexFromGenerator(rightIndex);
    return (this->theWeyl.RootSystem[left]+this->theWeyl.RootSystem[right]).IsEqualToZero();
  }
  void GenerateVermaMonomials(Vector<Rational>& highestWeight, GlobalVariables& theGlobalVariables)
  ;
  void ComputeChevalleyConstantS
(GlobalVariables* theGlobalVariables)
  ;
  //Setup: \gamma+\delta=\epsilon+\zeta=\eta is a Vector<Rational> .
  //then the below function computes n_{-\epsilon, -\zeta}
  template<class CoefficientType>
  void LieBracket
  (const ElementSemisimpleLieAlgebra<CoefficientType>& g1,
   const ElementSemisimpleLieAlgebra<CoefficientType>& g2,
   ElementSemisimpleLieAlgebra<CoefficientType>& output);
  void ComputeOneChevalleyConstant(int indexGamma, int indexDelta, int indexMinusEpsilon, int indexMinusZeta, int indexEta);
  void ExploitSymmetryAndCyclicityChevalleyConstants(int indexI, int indexJ);
  void ExploitSymmetryChevalleyConstants(int indexI, int indexJ);
  void ExploitTheCyclicTrick(int i, int j, int k);
  bool GetMaxQForWhichBetaMinusQAlphaIsARoot
(const Vector<Rational>& alpha, const Vector<Rational>& beta, int& output)const
  ;
  Rational GetConstant(const Vector<Rational> & root1, const Vector<Rational> & root2);
  bool CheckClosedness(std::string& output, GlobalVariables& theGlobalVariables);
  void ElementToStringVermaMonomials(std::string& output);
  void ElementToStringEmbedding(std::string& output);
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
  bool GetConstantOrHElement(const Vector<Rational> & root1, const Vector<Rational>& root2, Rational& outputRat, Vector<Rational>& outputH);
  bool TestForConsistency(GlobalVariables& theGlobalVariables);
  bool FindComplementaryNilpotent
  (ElementSemisimpleLieAlgebra<Rational>& e,
   ElementSemisimpleLieAlgebra<Rational>& output, GlobalVariables& theGlobalVariables);
  bool AttemptExtendingHEtoHEFWRTSubalgebra
  (Vectors<Rational>& RootsWithCharacteristic2,
   Selection& theZeroCharacteristics, Vectors<Rational>& simpleBasisSA, Vector<Rational>& h,
   ElementSemisimpleLieAlgebra<Rational>& outputE, ElementSemisimpleLieAlgebra<Rational>& outputF,
   Matrix<Rational>& outputMatrixSystemToBeSolved,
   PolynomialSubstitution<Rational>& outputSystemToBeSolved,
   Matrix<Rational>& outputSystemColumnVector, GlobalVariables& theGlobalVariables)
  ;
  static void FindSl2Subalgebras
  (SemisimpleLieAlgebra& inputOwner, SltwoSubalgebras& output, GlobalVariables& theGlobalVariables)
   ;
  void GetSl2SubalgebraFromRootSA(GlobalVariables& theGlobalVariables);
  template<class CoefficientType>
  void GetAd(Matrix<CoefficientType>& output, ElementSemisimpleLieAlgebra<CoefficientType>& e);
  void initHEFSystemFromECoeffs
  (int theRelativeDimension, Vectors<Rational>& rootsInPlay, Vectors<Rational>& simpleBasisSA,
   Vectors<Rational>& SelectedExtraPositiveRoots, int numberVariables, int numRootsChar2,
   int halfNumberVariables, Vector<Rational>& targetH, Matrix<Rational>& inputFCoeffs,
   Matrix<Rational>& outputMatrixSystemToBeSolved, Matrix<Rational>& outputSystemColumnVector,
   PolynomialSubstitution<Rational>& outputSystemToBeSolved);
  void MakeChevalleyTestReport(int i, int j, int k, int Total, GlobalVariables& theGlobalVariables);
  bool IsInTheWeightSupport(Vector<Rational>& theWeight, Vector<Rational>& highestWeight, GlobalVariables& theGlobalVariables);
  void GenerateOneMonomialPerWeightInTheWeightSupport(Vector<Rational>& theHighestWeight, GlobalVariables& theGlobalVariables);
  void CreateEmbeddingFromFDModuleHaving1dimWeightSpaces(Vector<Rational>& theHighestWeight, GlobalVariables& theGlobalVariables);
  int GetLengthStringAlongAlphaThroughBeta(Vector<Rational>& alpha, Vector<Rational>& beta, int& distanceToHighestWeight, Vectors<Rational>& weightSupport);
  void ComputeOneAutomorphism(GlobalVariables& theGlobalVariables, Matrix<Rational>& outputAuto,  bool useNegativeRootsFirst);
  void operator=(const SemisimpleLieAlgebra& other){ this->Assign(other);}
  bool operator==(const SemisimpleLieAlgebra& other)const
  { return this->theWeyl==other.theWeyl;
  }
};

class HomomorphismSemisimpleLieAlgebra
{
public:
  SemisimpleLieAlgebra* domainAlg;
  SemisimpleLieAlgebra* rangeAlg;
  //Let rk:=Rank(Domain)
  //format of ImagesSimpleChevalleyGenerators: the first rk elements give
  //the images of the Chevalley generators corresponding to simple positive roots
  //the second rk elements give the images of the Chevalley generators corresponding to simple
  //negative roots
  List<ElementSemisimpleLieAlgebra<Rational> > imagesSimpleChevalleyGenerators;
  //format of ImagesAllChevalleyGenerators: the Generators are given in the same order as
  //the one used in MonomialUniversalEnveloping
  List<ElementSemisimpleLieAlgebra<Rational> > imagesAllChevalleyGenerators;
  List<ElementSemisimpleLieAlgebra<Rational> > domainAllChevalleyGenerators;
  List<ElementSemisimpleLieAlgebra<Rational> > GmodK;
  Vectors<Rational> RestrictedRootSystem;
  Vectors<Rational> ImagesCartanDomain;
  SemisimpleLieAlgebra& theDomain()
  { if (domainAlg==0)
    { std::cout << "This is a programming error: non-initialized HomomorphismSemisimpleLieAlgebra. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    return *domainAlg;
  }
  SemisimpleLieAlgebra& theRange()
  { if (rangeAlg==0)
    { std::cout << "This is a programming error: non-initialized HomomorphismSemisimpleLieAlgebra. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    return *rangeAlg;
  }
  HomomorphismSemisimpleLieAlgebra(): domainAlg(0), rangeAlg(0){}
  std::string DebugString;
  void GetWeightsGmodKInSimpleCoordsK
  (Vectors<Rational>& outputWeights, GlobalVariables& theGlobalVariables)
  { this->GetWeightsWrtKInSimpleCoordsK(outputWeights, this->GmodK, theGlobalVariables);}
  void GetWeightsKInSimpleCoordsK
  (Vectors<Rational>& outputWeights, GlobalVariables& theGlobalVariables)
  { this->GetWeightsWrtKInSimpleCoordsK(outputWeights, this->imagesAllChevalleyGenerators, theGlobalVariables); }
  void GetWeightsWrtKInSimpleCoordsK
  (Vectors<Rational>& outputWeights,
   List<ElementSemisimpleLieAlgebra<Rational> >& inputElts, GlobalVariables& theGlobalVariables)
  ;
  void ToString(std::string& output, GlobalVariables& theGlobalVariables) {this->ToString(output, false, theGlobalVariables);};
  void ToString(std::string& output, bool useHtml, GlobalVariables& theGlobalVariables);
  void MakeGinGWithId
  (char theWeylLetter, int theWeylDim, ListReferences<SemisimpleLieAlgebra>& ownerOfAlgebras,
   GlobalVariables& theGlobalVariables)
  ;
  void ProjectOntoSmallCartan(Vector<Rational>& input, Vector<Rational> & output, GlobalVariables& theGlobalVariables);
  void ProjectOntoSmallCartan(Vectors<Rational>& input, Vectors<Rational>& output, GlobalVariables& theGlobalVariables);
  void GetMapSmallCartanDualToLargeCartanDual(Matrix<Rational> & output);
  void ComputeDebugString(GlobalVariables& theGlobalVariables)
  { this->ToString(this->DebugString, theGlobalVariables);
  }
  void ComputeDebugString(bool useHtml, GlobalVariables& theGlobalVariables){this->ToString(this->DebugString, useHtml, theGlobalVariables);}
  std::string ToString(GlobalVariables& theGlobalVariables){ std::string tempS; this->ToString(tempS, theGlobalVariables); return tempS; }
  void GetRestrictionAmbientRootSystemToTheSmallerCartanSA(Vectors<Rational>& output, GlobalVariables& theGlobalVariables);
  bool ComputeHomomorphismFromImagesSimpleChevalleyGenerators(GlobalVariables& theGlobalVariables);
  bool CheckClosednessLieBracket(GlobalVariables& theGlobalVariables);
  void ApplyHomomorphism
  (const ElementSemisimpleLieAlgebra<Rational>& input, ElementSemisimpleLieAlgebra<Rational>& output)
  ;
  bool ApplyHomomorphism
  (const ElementUniversalEnveloping<RationalFunctionOld>& input,
   ElementUniversalEnveloping<RationalFunctionOld>& output, GlobalVariables& theGlobalVariables)
   ;
  bool ApplyHomomorphism
(const MonomialUniversalEnveloping<RationalFunctionOld>& input, const RationalFunctionOld& theCoeff,
 ElementUniversalEnveloping<RationalFunctionOld>& output, GlobalVariables& theGlobalVariables)
   ;
};

class SemisimpleLieAlgebraOrdered
{
public:
  SemisimpleLieAlgebra theOwner;
  //the format of the order is arbitrary except for the following requirements:
  //-All elements of the order must be either 1) nilpotent or 2) elements of the Cartan
  //-Let the number of positive roots be N and the rank be K. Then the indices N,..., N+K-1 must
  //   correspond to the elements of the Cartan.
  List<ElementSemisimpleLieAlgebra<Rational> > theOrder;
  //The order of chevalley generators is as follows. First come negative roots,
  //then elements of cartan, then positive Vectors<Rational>
  //The weights are in increasing order
  //The i^th column of the matrix gives the coordinates of the i^th Chevalley generator
  //in the current coordinates
  Matrix<Rational> ChevalleyGeneratorsInCurrentCoords;
  void AssignGeneratorCoeffOne
  (int theIndex, ElementSemisimpleLieAlgebra<Rational>& output)
  { output.operator=(this->theOrder[theIndex]);
  }
  int GetDisplayIndexFromGeneratorIndex(int GeneratorIndex);
  void GetLinearCombinationFrom
  (ElementSemisimpleLieAlgebra<Rational>& input, Vector<Rational>& theCoeffs)
  ;
  void init
  (List<ElementSemisimpleLieAlgebra<Rational> >& inputOrder, SemisimpleLieAlgebra& owner, GlobalVariables& theGlobalVariables)
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
  std::string ToString
(bool useLatex, bool useCalculatorFormat, FormatExpressions* PolyFormatLocal, GlobalVariables& theGlobalVariables)const
;
  void ComputeDebugString()
  { GlobalVariables theGlobalVariables;
    FormatExpressions PolyFormatLocal;
    this->DebugString=this->ToString(false, true, &PolyFormatLocal, theGlobalVariables);
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
  unsigned int HashFunction() const;
  static inline unsigned int HashFunction(const MonomialUniversalEnvelopingOrdered<CoefficientType>& input)
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
  { this->generatorsIndices=(other.generatorsIndices);
    this->Powers=(other.Powers);
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
  void ToString
(std::string& output, bool useLatex,
 bool useCalculatorFormat,
 FormatExpressions& PolyFormatLocal, GlobalVariables& theGlobalVariables)const
  ;
  std::string ToString
  (bool useLatex, bool useCalculatorFormat, FormatExpressions& PolyFormatLocal,
   GlobalVariables& theGlobalVariables)const
  { std::string tempS;
    this->ToString(tempS, useLatex, useCalculatorFormat, PolyFormatLocal, theGlobalVariables);
    return tempS;
  }
  std::string ToString
  (FormatExpressions& PolyFormatLocal, GlobalVariables& theGlobalVariables)const
  { std::string tempS;
    this->ToString(tempS, true, true, PolyFormatLocal, theGlobalVariables);
    return tempS;
  }
  std::string ToString(FormatExpressions& PolyFormatLocal)const
  { GlobalVariables theGlobalVariables;
    return this->ToString(PolyFormatLocal, theGlobalVariables);
  }
  bool NeedsBrackets()const{return this->size>1;}
  void ComputeDebugString()
  { FormatExpressions tempFormat;
    GlobalVariables theGlobalVariables;
    this->DebugString=this->ToString(tempFormat, theGlobalVariables);
  }
  SemisimpleLieAlgebraOrdered* owner;
  void AddMonomial(const MonomialUniversalEnvelopingOrdered<CoefficientType>& input);
  void MakeHgenerator(const Vector<Rational> & input, int numVars, SemisimpleLieAlgebraOrdered& theOwner);
  void AssignElementLieAlgebra
(const ElementSemisimpleLieAlgebra<Rational>& input, const CoefficientType& theRingUnit,
 const CoefficientType& theRingZero, SemisimpleLieAlgebraOrdered& theOwner)
;
  void MakeOneGenerator
  (int theIndex, const CoefficientType& theCoeff, SemisimpleLieAlgebraOrdered& owner,
   GlobalVariables* theContext);
//  void MakeOneGeneratorCoeffOne(Vector<Rational> & rootSpace, int numVars, SemisimpleLieAlgebraOrdered& theOwner){this->MakeOneGeneratorCoeffOne(theOwner.GetGeneratorFromRoot(rootSpace), numVars, theOwner);};
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
  (ElementUniversalEnveloping<CoefficientType>& output, SemisimpleLieAlgebra& inputOwner);
  bool ConvertToLieAlgebraElementIfPossible
  (ElementSemisimpleLieAlgebra<Rational>& output)const;
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
  void ActOnMe(const ElementSemisimpleLieAlgebra<Rational>& theElt, ElementUniversalEnvelopingOrdered& output);
  void LieBracketOnTheRight(const ElementUniversalEnvelopingOrdered& right, ElementUniversalEnvelopingOrdered& output);
  void LieBracketOnTheRight
  (const ElementSemisimpleLieAlgebra<Rational>& right, const CoefficientType& ringUnit, const CoefficientType& ringZero)
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
class MonomialUniversalEnveloping : public MonomialTensor<CoefficientType>
{
private:
public:

  std::string ToString
  (FormatExpressions* theFormat=0)const
  ;
  SemisimpleLieAlgebra* owneR;
  // SelectedIndices gives the non-zero powers of the chevalley generators participating in the monomial
  // Powers gives the powers of the Chevalley generators in the order they appear in generatorsIndices
  friend std::ostream& operator << (std::ostream& output, const MonomialUniversalEnveloping<CoefficientType>& theMon)
  { output << theMon.ToString();
    return output;
  }
  bool IsAConstant()const
  { return this->IsEqualToOne();
  }
  static const bool IsEqualToZero(){return false;}
  bool AdjointRepresentationAction
  (const ElementUniversalEnveloping<CoefficientType>& input, ElementUniversalEnveloping<CoefficientType>& output, GlobalVariables& theGlobalVariables)
  ;
  template<class otherType>
  void operator=(const MonomialUniversalEnveloping<otherType>& other)
  { this->MonomialTensor<CoefficientType>::operator=(other);
    this->owners=other.owners;
    this->indexInOwners=other.indexInOwners;
  }
  SemisimpleLieAlgebra& GetOwner()const
  { return *this->owneR;
  }
  void MakeZero
(int numVars, SemisimpleLieAlgebra& inputOwner)
  ;
  void MakeZero
  (const CoefficientType& theRingZero, SemisimpleLieAlgebra& inputOwner)
  { this->init(inputOwner);
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
  void SetNumVariables(int newNumVars);
  void Substitution(const PolynomialSubstitution<Rational>& theSub);
  unsigned int HashFunction() const
  { return this->::MonomialTensor<CoefficientType>::HashFunction();
  }
  static inline unsigned int HashFunction(const MonomialUniversalEnveloping<CoefficientType>& input)
  { return input.HashFunction();
  }
  void GetDegree(CoefficientType& output)
  { if (this->Powers.size==0)
    { output.MakeZero();
      return;
    }
    output=this->Powers[0];
    for (int i=1; i<this->generatorsIndices.size; i++)
      output+=(this->Powers[i]);
  }
  void init(SemisimpleLieAlgebra& inputOwner)
  { this->owneR=&inputOwner;
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
  void MakeConst(SemisimpleLieAlgebra& inputOwner)
  { this->generatorsIndices.size=0;
    this->Powers.size=0;
    this->owneR=&inputOwner;
  }
  //we assume the standard order for being simplified to be Ascending.
  //this way the positive roots will end up being in the end, which is very
  //convenient for computing with Verma modules.
  //Format of the order of the generators:
  // first come the negative roots, in increasing height, then the elements of
  //the Cartan subalgebra, then the positive roots, in increasing height
  //The order of the positive roots is the same as the order in which roots are kept
  //in WeylGroup::RootSystem.
  // The "zero level roots" - i.e. the elements of the Cartan subalgebra lie in between
  // the negative and positive rootss.
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
  MonomialUniversalEnveloping():owneR(0){}
  bool operator>(const MonomialUniversalEnveloping& other)
  { return this->::MonomialTensor<CoefficientType>::operator>(other);
  }
  bool operator==(const MonomialUniversalEnveloping& other)const
  { return this->owneR==other.owneR
    && this->Powers==other.Powers && this->generatorsIndices==other.generatorsIndices;
  }
  inline void operator=(const MonomialUniversalEnveloping& other)
  { this->::MonomialTensor<CoefficientType>::operator=(other);
    this->owneR=other.owneR;
  }
  inline void operator*=(const MonomialUniversalEnveloping& other)
  { this->::MonomialTensor<CoefficientType>::operator*=(other);
  }
};

template <class CoefficientType>
class ElementUniversalEnveloping: public ElementMonomialAlgebra<MonomialUniversalEnveloping<CoefficientType>, CoefficientType>
{
private:
  void CleanUpZeroCoeff();
  friend class MonomialUniversalEnveloping<CoefficientType>;
public:
  SemisimpleLieAlgebra* owneR;
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
    tempElt.MakeZero(*this->owner);
    tempElt.AddMonomial(right, theRingUnit);
    return this->HWTAAbilinearForm
    (tempElt, output, subHiGoesToIthElement, theGlobalVariables, theRingUnit, theRingZero, logStream);
  }
  bool NeedsBracketForMultiplication()
  { return this->size>1;
  }
  bool ApplyMinusTransposeAutoOnMe();
  bool ApplyTransposeAntiAutoOnMe();
  void MakeHgenerator
  (const Vector<Rational>& input, SemisimpleLieAlgebra& inputOwner,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
   ;
  void AssignElementLieAlgebra
(const ElementSemisimpleLieAlgebra<Rational>& input, SemisimpleLieAlgebra& inputOwner,
 const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
  ;
  bool GetWithSimpleGeneratorsOnly(MonomialCollection<MonomialTensor<CoefficientType>, CoefficientType>& output);
  void MakeOneGenerator
(int theIndex, SemisimpleLieAlgebra& inputOwner, const CoefficientType& theRingUnit)
  ;
  void MakeOneGeneratorCoeffOne
(int theIndex, SemisimpleLieAlgebra& inputOwners,
 const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
 ;
  void MakeOneGeneratorCoeffOne
  (int theIndex, int numVars, SemisimpleLieAlgebra& inputOwner)
;
  void MakeOneGeneratorCoeffOne
  (const Vector<Rational>& rootSpace, SemisimpleLieAlgebra& inputOwner,
  const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
  { this->MakeOneGeneratorCoeffOne
    (inputOwner.GetGeneratorFromRoot(rootSpace), inputOwner, theRingUnit, theRingZero);
  }
  void MakeZero(SemisimpleLieAlgebra& inputOwner);
  bool ConvertToLieAlgebraElementIfPossible(ElementSemisimpleLieAlgebra<Rational>& output)const;
  void MakeConst(const Rational& coeff, int numVars, SemisimpleLieAlgebra& inputOwner);
  void MakeConst(const CoefficientType& coeff, SemisimpleLieAlgebra& inputOwner)
  { this->MakeZero(inputOwner);
    MonomialUniversalEnveloping<CoefficientType> tempMon;
    tempMon.MakeConst(inputOwner);
    this->AddMonomial(tempMon, coeff);
  }
  void Simplify
  (GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
   ;
  int GetMinNumVars()const
  { int result=0;
    for (int i=0; i<this->size; i++)
    { result=MathRoutines::Maximum(result, this->theCoeffs[i].GetMinNumVars());
      result=MathRoutines::Maximum(result, (*this)[i].GetMinNumVars());
    }
    return result;
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
   const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
   ;
  static void GetCoordinateFormOfSpanOfElements
  (List<ElementUniversalEnveloping<CoefficientType> >& theElements,
   Vectors<CoefficientType>& outputCoordinates,
   ElementUniversalEnveloping<CoefficientType>& outputCorrespondingMonomials,
   GlobalVariables& theGlobalVariables)
;
  bool GetCoordsInBasis
  (List<ElementUniversalEnveloping<CoefficientType> >& theBasis, Vector<CoefficientType>& output,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero,
   GlobalVariables& theGlobalVariables)const
;
  static inline unsigned int HashFunction (const ElementUniversalEnveloping<CoefficientType>& input)
  { return input.HashFunction();
  }
  unsigned int HashFunction()const
  { return this->::MonomialCollection<MonomialUniversalEnveloping<CoefficientType>, CoefficientType>::HashFunction();
  }
template<class CoefficientTypeQuotientField>
static bool GetBasisFromSpanOfElements
  (List<ElementUniversalEnveloping<CoefficientType> >& theElements,
   Vectors<CoefficientTypeQuotientField>& outputCoords,
   List<ElementUniversalEnveloping<CoefficientType> >& outputTheBasis,
   const CoefficientTypeQuotientField& theFieldUnit,
   const CoefficientTypeQuotientField& theFieldZero,
   GlobalVariables& theGlobalVariables)
   ;

  void AssignFromCoordinateFormWRTBasis
  (List<ElementUniversalEnveloping<CoefficientType> >& theBasis,
   Vector<CoefficientType>& input, SemisimpleLieAlgebra& owner)
  ;
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
 const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
   ;
  void LieBracketOnTheRight
  (const ElementUniversalEnveloping<CoefficientType>& right,
   ElementUniversalEnveloping<CoefficientType>& output)const;
  void LieBracketOnTheLeft(const ElementSemisimpleLieAlgebra<Rational>& left);
  void AssignInt(int coeff, int numVars, SemisimpleLieAlgebra& theOwner)
  { Rational tempRat=coeff;
    this->MakeConst(tempRat, numVars, &theOwner);
  }
  SemisimpleLieAlgebra& GetOwner()const
  { return *this->owneR;
  }
  template <class otherType>
  void Assign(const ElementUniversalEnveloping<otherType>& other)
  { this->owners=other.owners;
    this->indexInOwners=other.indexInOwners;
    MonomialUniversalEnveloping<CoefficientType> tempMon;
    CoefficientType theCoeff;
    for (int i=0; i<other.size; i++)
    { tempMon=other[i];
      theCoeff=other.theCoeffs[i];
      this->AddMonomial(tempMon, theCoeff);
    }
  }
  void operator=(const Rational& other)
  { this->MakeConst(other, 0, *this->owner);
  }

  void operator=(const ElementUniversalEnveloping<CoefficientType>& other)
  { this->::MonomialCollection<MonomialUniversalEnveloping<CoefficientType>, CoefficientType>::operator=(other);
    this->owneR=other.owneR;
  }
  void operator*=(const ElementUniversalEnveloping<CoefficientType>& standsOnTheRight);
  void operator*=(const CoefficientType& other)
  { this->::MonomialCollection<MonomialUniversalEnveloping<CoefficientType>, CoefficientType>
    ::operator*=(other);
  }
  template<class otherType>
  void operator/=(const otherType& other)
  { this->::MonomialCollection<MonomialUniversalEnveloping<CoefficientType>, CoefficientType>
    ::operator/=(other);
  }
  ElementUniversalEnveloping<CoefficientType>():owneR(0){}
  ElementUniversalEnveloping<CoefficientType>(const ElementUniversalEnveloping<CoefficientType>& other){this->operator=(other);}
};

class ElementWeylAlgebra : public MonomialCollection<MonomialWeylAlgebra, Rational>
{
public:
  void MakeGEpsPlusEpsInTypeD(int i, int j, int NumVars);
  void MakeGEpsMinusEpsInTypeD(int i, int j, int NumVars);
  void MakeGMinusEpsMinusEpsInTypeD(int i, int j, int NumVars);
  void Makexixj(int i, int j, int NumVars);
  void Makexi(int i, int NumVars);
  void Makedi(int i, int NumVars);
  void Makedidj(int i, int j, int NumVars);
  void Makexidj(int i, int j, int NumVars);
  static void GetStandardOrderDiffOperatorCorrespondingToNraisedTo
(const Rational& inputRationalPower, int indexVar, ElementWeylAlgebra& outputDO,
 Polynomial<Rational>& outputDenominator, GlobalVariables& theGlobalVariables)
   ;
  bool ActOnPolynomial(Polynomial<Rational>& thePoly);
  void SetNumVariables(int newNumVars);
  void MultiplyOnTheLeft(ElementWeylAlgebra& standsOnTheLeft, GlobalVariables& theGlobalVariables);
  void LieBracketOnTheLeft(ElementWeylAlgebra& standsOnTheLeft, GlobalVariables& theGlobalVariables);
  void LieBracketOnTheLeftMakeReport(ElementWeylAlgebra& standsOnTheLeft, GlobalVariables& theGlobalVariables, std::string& report);
  void LieBracketOnTheRightMakeReport(ElementWeylAlgebra& standsOnTheRight, GlobalVariables& theGlobalVariables, std::string& report);
  void LieBracketOnTheRight(ElementWeylAlgebra& standsOnTheRight, GlobalVariables& theGlobalVariables);
  void MakeConst(const Rational& theConst);
  void SubstitutionTreatPartialsAndVarsAsIndependent
  (PolynomialSubstitution<Rational>& theSub)
  ;
  void MakeOne(int ExpectedNumVars=0)
  { MonomialWeylAlgebra tempMon;
    tempMon.polynomialPart.MakeOne(ExpectedNumVars);
    tempMon.differentialPart.MakeOne(ExpectedNumVars);
    this->MakeZero();
    this->AddMonomial(tempMon, 1);
  }
  void RaiseToPower(int thePower);
  void MultiplyTwoMonomials
(const MonomialWeylAlgebra& left, const MonomialWeylAlgebra& right,
 ElementWeylAlgebra& output)const
;
  void AssignPolynomial(const Polynomial<Rational>& input)
  { this->MakeZero();
    MonomialWeylAlgebra tempM;
    for (int i=0; i<input.size; i++)
    { tempM.polynomialPart=input[i];
      this->AddMonomial(tempM, input.theCoeffs[i]);
    }
  }
  void operator=(const std::string& input);
  inline void operator*=(const Rational& other)
  { this->MonomialCollection<MonomialWeylAlgebra, Rational>::operator*=(other);
  }
  void operator*=(const ElementWeylAlgebra& other);
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
  (const Vector<Rational>& weightInSimpleCoords, WeylGroup& theOwner)
  ;
  void MakeFromWaypoints
  (Vectors<Rational>& weightsInSimpleCoords, WeylGroup& theOwner)
  { this->owner=&theOwner;
    this->Waypoints=weightsInSimpleCoords;
    this->Simplify();
  }
  void ActByFalpha(int indexAlpha);
  void ActByEalpha(int indexAlpha);
  void ActByEFDisplayIndex(int displayIndex);
//   List<Rational> Speeds;
  void operator+=(const LittelmannPath& other)
  { this->Waypoints.ReservE(this->Waypoints.size+other.Waypoints.size);
    Vector<Rational> endPoint=*this->Waypoints.LastObject();
    for (int i=0; i<other.Waypoints.size; i++)
      this->Waypoints.AddOnTop(other.Waypoints[i]+endPoint);
  }
  bool IsAdaptedString
  (MonomialTensor<int, MathRoutines::IntUnsignIdentity>& theString)
  ;
  std::string ElementToStringIndicesToCalculatorOutput
(LittelmannPath& inputStartingPath, List<int>& input)
  ;
  std::string ElementToStringOperatorSequenceStartingOnMe
(List<int>& input)
  ;
  bool GenerateOrbit
(List<LittelmannPath>& output, List<List<int> >& outputOperators, GlobalVariables& theGlobalVariables, int UpperBoundNumElts,
 Selection* parabolicNonSelectedAreInLeviPart=0)
;
std::string GenerateOrbitAndAnimate
(GlobalVariables& theGlobalVariables)
;
  bool MinimaAreIntegral();
  std::string ToString(bool useSimpleCoords=true, bool useArrows=true, bool includeDominance=false)const;
  void Simplify();
  unsigned int HashFunction()const
  { return this->Waypoints.HashFunction();
  }
  static inline unsigned int HashFunction(const LittelmannPath& input)
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

  std::string ToString(FormatExpressions* theFormat=0);
  void init
  (Vector<Rational>& theNEq, Cone& startingCone, Lattice& startingLattice, Vector<Rational> & startingShift)
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
  std::string ToString(bool useLatex, bool useHtml);
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
  static const bool IsEqualToZero()
  { return false;
  }
  friend std::ostream& operator << (std::ostream& output, const MonomialChar<CoefficientType>& input)
  { output << input.ToString();
    return output;
  }
  void AccountSingleWeight
(const Vector<Rational>& currentWeightSimpleCoords, const Vector<Rational>& otherHighestWeightSimpleCoords,
 WeylGroup& theWeyl, Rational& theMult, charSSAlgMod<CoefficientType>& outputAccum)const
  ;
  std::string TensorAndDecompose
(const MonomialChar<CoefficientType>& other, SemisimpleLieAlgebra& owner, charSSAlgMod<CoefficientType>& output,
 GlobalVariables& theGlobalVariables)const
   ;
  std::string ToString
  (FormatExpressions* theFormat=0)const
  ;
  inline unsigned int HashFunction()const
  { return weightFundamentalCoords.HashFunction();
  }
  static inline unsigned int HashFunction(const MonomialChar<CoefficientType>& input)
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
  SemisimpleLieAlgebra* owner;
  void Reset()
  { this->::MonomialCollection<MonomialChar<CoefficientType>, CoefficientType>::MakeZero();
    this->owner=0;
  }
  void MakeZero(SemisimpleLieAlgebra* inputOwner=0)
  { this->Reset();
    this->owner=inputOwner;
  }
  bool IsEqualToZero()
  { return this->size==0;
  }
  charSSAlgMod(): owner(0){}
  std::string ElementToStringCharacter(List<Vector<Rational> >& theWeights, List<Rational>& theMults);
  unsigned int HashFunction()const
  { return this->HashFunction(*this);
  }
  static unsigned int HashFunction(const charSSAlgMod<CoefficientType>& input)
  { return input.::MonomialCollection<MonomialChar<CoefficientType>, CoefficientType>
    ::HashFunction(input);
  }

  void MakeFromWeight
(const Vector<CoefficientType>& inputWeightSimpleCoords, SemisimpleLieAlgebra* inputOwner)
   ;
  bool SplitCharOverRedSubalg
(std::string* Report, charSSAlgMod& output, branchingData& inputData,
 GlobalVariables& theGlobalVariables)
   ;
  bool GetDominantCharacterWRTsubalgebra
 (charSSAlgMod& outputCharOwnerSetToZero, std::string& outputDetails,
  GlobalVariables& theGlobalVariables, int upperBoundNumDominantWeights)
;
  bool FreudenthalEvalMeDominantWeightsOnly
 (charSSAlgMod<CoefficientType>& outputCharOwnerSetToZero, int upperBoundNumDominantWeights,
  std::string* outputDetails, GlobalVariables* theGlobalVariables)
  ;
  bool FreudenthalEvalMeFullCharacter
 (charSSAlgMod<CoefficientType>& outputCharOwnerSetToZero, int upperBoundNumDominantWeights,
  std::string* outputDetails, GlobalVariables* theGlobalVariables)
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
(WeylGroup& actualAmbientWeyl, GlobalVariables& theGlobalVariables, DrawingVariables& theDrawingVars)const
  ;
  SemisimpleLieAlgebra& GetOwner()
  { if (this->owner==0)
    { std::cout << "This is a programming error: requesting owner semisimple Lie algebra "
      << " of non-initialized "
      << " character. " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    return *this->owner;
  }

  bool DrawMe
(std::string& outputDetails, GlobalVariables& theGlobalVariables,
 DrawingVariables& theDrawingVars, int upperBoundWeights, bool useMults)
  ;
  bool SplitOverLeviMonsEncodeHIGHESTWeight
(std::string* Report, charSSAlgMod& output, const Selection& splittingParSel, const Selection& ParSelFDInducingPart,
 ReflectionSubgroupWeylGroup& outputWeylSub, GlobalVariables& theGlobalVariables)
     ;
  int GetIndexExtremeWeightRelativeToWeyl
  (WeylGroup& theWeyl)const
  ;
  void MakeTrivial(SemisimpleLieAlgebra& inputOwner);
  std::string MultiplyBy(const charSSAlgMod& other, GlobalVariables& theGlobalVariables);
  std::string operator*=(const charSSAlgMod& other);
  std::string operator*=(const MonomialChar<Rational>& other);
  void operator*=(const CoefficientType& other)
  { this->::MonomialCollection<MonomialChar<CoefficientType>, CoefficientType>::operator*=(other);
  }
};

//the following data is isolated in a struct because it is
//way too large a lump to pass separately
struct branchingData
{ HomomorphismSemisimpleLieAlgebra theHmm;
  FormatExpressions theFormat;
  Vector<RationalFunctionOld> theWeightFundCoords;
  charSSAlgMod<RationalFunctionOld> theAmbientChar;
  charSSAlgMod<RationalFunctionOld> theSmallCharFDpart;
  Selection selInducing;
  Selection selSmallParSel;
  Selection SelSplittingParSel;
  Vectors<Rational> weightsNilradicalLarge;
  Vectors<Rational> weightsNilradicalSmall;
  Vectors<Rational> weightsNilModPreNil;
  List<int> indicesNilradicalSmall;
  List<int> indicesNilradicalLarge;
  List<ElementSemisimpleLieAlgebra<Rational> > nilradicalLarge;
  List<ElementSemisimpleLieAlgebra<Rational> > nilradicalSmall;
  List<ElementSemisimpleLieAlgebra<Rational> > NilModPreNil;
  Vectors<RationalFunctionOld> outputWeightsFundCoordS;
  Vectors<RationalFunctionOld> outputWeightsSimpleCoords;
  Vectors<RationalFunctionOld> g2Weights;
  Vectors<RationalFunctionOld> g2DualWeights;
  Vectors<RationalFunctionOld> leviEigenSpace;
  Vectors<Rational> generatorsSmallSub;
  HashedList<RationalFunctionOld> theCharacterDifferences;
  List<ElementUniversalEnveloping<RationalFunctionOld> > outputEigenWords;
  List<RationalFunctionOld> theChars;
  List<ElementSumGeneralizedVermas<RationalFunctionOld> > theEigenVectorS;
  List<ElementUniversalEnveloping<RationalFunctionOld> > theUEelts;
  List<Rational> additionalMultipliers;
  List<RationalFunctionOld> theShapovalovProducts;
  List<ElementSumGeneralizedVermas<RationalFunctionOld> > theEigenVectorsLevi;
  ReflectionSubgroupWeylGroup WeylFD;
  ReflectionSubgroupWeylGroup WeylFDSmallAsSubInLarge;
  ReflectionSubgroupWeylGroup WeylFDSmall;
  std::string GetStringCasimirProjector(int theIndex, const Rational& additionalMultiple);
  template <class CoefficientType>
  Vector<CoefficientType> ProjectWeight(Vector<CoefficientType>& input);
  void resetOutputData();
  void initAssumingParSelAndHmmInittedPart1NoSubgroups(GlobalVariables& theGlobalVariables)
  ;
  void initAssumingParSelAndHmmInittedPart2Subgroups(GlobalVariables& theGlobalVariables)
  ;
  void initAssumingParSelAndHmmInitted(GlobalVariables& theGlobalVariables)
  { MacroRegisterFunctionWithName("branchingData::initAssumingParSelAndHmmInitted");
    this->initAssumingParSelAndHmmInittedPart1NoSubgroups(theGlobalVariables);
    this->initAssumingParSelAndHmmInittedPart2Subgroups(theGlobalVariables);
  }
};

template <class CoefficientType>
Vector<CoefficientType> branchingData::ProjectWeight(Vector<CoefficientType>& input)
{ Vector<CoefficientType> result;
  Vector<CoefficientType> fundCoordsSmaller;
  fundCoordsSmaller.MakeZero(this->theHmm.theDomain().GetRank());
  for (int j=0; j<this->theHmm.theDomain().GetRank(); j++)
  { fundCoordsSmaller[j]=this->theHmm.theRange().theWeyl.RootScalarCartanRoot(input, theHmm.ImagesCartanDomain[j]);
    fundCoordsSmaller[j]/=this->theHmm.theDomain().theWeyl.CartanSymmetric.elements[j][j]/2;
  }
  result=this->theHmm.theDomain().theWeyl.GetSimpleCoordinatesFromFundamental(fundCoordsSmaller);
  return result;
}

class MonomialMatrix
{
  friend std::ostream& operator << (std::ostream& output, const MonomialMatrix& theMon)
  { output << theMon.ToString();
    return output;
  }
  public:
  int vIndex;
  int dualIndex;
  bool IsId;
  MonomialMatrix(const MonomialMatrix& other)
  { this->operator=(other);
  }
  MonomialMatrix():vIndex(-1), dualIndex(-1), IsId(false){}
  MonomialMatrix(int i, int j): vIndex(i), dualIndex(j), IsId(false){}
  void operator=(const MonomialMatrix& other)
  { this->vIndex=other.vIndex;
    this->dualIndex=other.dualIndex;
    this->IsId=other.IsId;
  }
  void MakeOne()
  { this->MakeIdSpecial();
  }
  void Transpose()
  { MathRoutines::swap(this->vIndex, this->dualIndex);
  }
  void MakeZero()
  { this->IsId=false;
    this->vIndex=-1;
    this->dualIndex=-1;
  }
  bool IsEqualToZero()const
  { return !this->IsId && this->vIndex==-1 && this->dualIndex==-1;
  }
  bool operator==(const MonomialMatrix& other)const
  { return this->vIndex==other.vIndex && this->dualIndex==other.dualIndex && this->IsId==other.IsId;
  }
  static unsigned int HashFunction(const MonomialMatrix& input)
  { return input.vIndex*SomeRandomPrimes[0]+input.dualIndex*SomeRandomPrimes[1]+input.IsId;
  }
  inline unsigned int HashFunction()const
  { return HashFunction(*this);
  }
  bool operator>(const MonomialMatrix& other)const
  { if (this->IsId!=other.IsId)
      return this->IsId>other.IsId;
    if (this->vIndex==other.vIndex)
      return this->dualIndex>other.dualIndex;
    return this->vIndex>other.vIndex;
  }
  void MakeIdSpecial()
  { this->vIndex=-1;
    this->dualIndex=-1;
    this->IsId=true;
  }
  std::string ToString(FormatExpressions* theFormat=0)const
  { std::stringstream out;
    if (!this->IsId)
      out << "m_{" << this->vIndex+1 << "}\\otimes " << "m^*_{" << this->dualIndex+1 << "}";
    else
      out << "id";
    return out.str();
  }
  void operator*=(const MonomialMatrix& other)
  { MacroRegisterFunctionWithName("MonomialMatrix<CoefficientType>::operator*=");
    if (this==&other)
    { MonomialMatrix otherCopy;
      otherCopy=other;
      *this*=otherCopy;
      return;
    }
    if (other.IsId)
      return;
    if (this->IsId)
    { *this=other;
      return;
    }
    if (this->dualIndex==other.vIndex)
      this->dualIndex=other.dualIndex;
    else
      this->MakeZero();
  }
};

template <class CoefficientType>
class MatrixTensor: public ElementMonomialAlgebra<MonomialMatrix, CoefficientType >
{
public:
  void MakeIdSpecial()
  { this->MakeZero();
    MonomialMatrix theMon;
    theMon.MakeIdSpecial();
    this->AddMonomial(theMon, 1);
  }
  void MakeId(int numVars)
  { this->MakeZero();
    MonomialMatrix theMon;
    for (int i=0; i<numVars; i++)
    { theMon.dualIndex=i;
      theMon.vIndex=i;
      this->AddMonomial(theMon, 1);
    }
  }
  int GetMaxNumColsNumRows()
  { int result=0;
    for (int i=0; i<this->size; i++)
    { result=MathRoutines::Maximum(result, (*this)[i].dualIndex);
      result=MathRoutines::Maximum(result, (*this)[i].vIndex);
    }
    return result;
  }
  bool IsPositiveDefinite()
  { Matrix<CoefficientType> other;
    this->GetMatrix(other, this->GetMaxNumColsNumRows());
    return other.IsPositiveDefinite();
  }
  void operator=(const Matrix<CoefficientType>& other)
  { this->MakeZero();
    MonomialMatrix theMon;
    for (int i=0; i<other.NumRows; i++)
      for (int j=0; j<other.NumCols; j++)
        if (!other.elements[i][j].IsEqualToZero())
        { theMon.dualIndex=j;
          theMon.vIndex=i;
          this->AddMonomial(theMon, other.elements[i][j]);
        }
  }
  void Substitution(const PolynomialSubstitution<Rational>& theSub)
  { MatrixTensor<CoefficientType> thisCopy=*this;
    this->MakeZero();
    CoefficientType tempCF;
    for (int i=0; i<thisCopy.size; i++)
    { tempCF=thisCopy.theCoeffs[i];
      tempCF.Substitution(theSub);
      this->AddMonomial(thisCopy[i] , tempCF);
    }
  }
  void Transpose()
  { MatrixTensor<CoefficientType> output;
    MonomialMatrix theMon;
    output.MakeZero();
    for (int i=0; i<this->size; i++)
    { theMon=(*this)[i];
      theMon.Transpose();
      output.AddMonomial(theMon, this->theCoeffs[i]);
    }
    *this=output;
  }
  void SetNumVariables(int newNumVars)
  { MatrixTensor<CoefficientType> thisCopy=*this;
    this->MakeZero();
    CoefficientType tempCF;
    for (int i=0; i<thisCopy.size; i++)
    { tempCF=thisCopy.theCoeffs[i];
      tempCF.SetNumVariables(newNumVars);
      this->AddMonomial(thisCopy[i] , tempCF);
    }
  }
  void LieBracketOnTheLeft(const MatrixTensor<CoefficientType>& standsOnTheLeft)
  { MacroRegisterFunctionWithName("MatrixTensor<CoefficientType>::LieBracketOnTheLeft");
    MatrixTensor<CoefficientType> output;
    MonomialMatrix theMon;
    output.MakeZero();
    output.SetExpectedSize(this->size*standsOnTheLeft.size*2);
    CoefficientType tempCF;
    for (int i=0; i<this->size; i++)
      for (int j=0; j<standsOnTheLeft.size; j++)
      { tempCF=this->theCoeffs[i];
        tempCF*=standsOnTheLeft.theCoeffs[j];
        theMon=standsOnTheLeft[j];
        theMon*=(*this)[i];
        output.AddMonomial(theMon, tempCF);
        tempCF*=-1;
        theMon=(*this)[i];
        theMon*=standsOnTheLeft[j];
        output.AddMonomial(theMon, tempCF);
      }
    *this=output;
  }
  void GetMatrix(Matrix<CoefficientType>& output, int theDim)
  { output.init(theDim, theDim);
    output.NullifyAll();
    for (int i=0; i<this->size; i++)
      if ((*this)[i].IsId)
        for (int j=0; j<theDim; j++)
          output.elements[j][j]+= this->theCoeffs[i];
      else
        output.elements[(*this)[i].vIndex][(*this)[i].dualIndex]+=this->theCoeffs[i];
  }
};

template <class CoefficientType>
class ModuleSSalgebra
{ List<MatrixTensor<CoefficientType> > actionsGeneratorsMaT;
  List<List<List<ElementUniversalEnveloping<CoefficientType> > > > actionsGeneratorS;
  Selection ComputedGeneratorActions;
  Rational hwtaabfSimpleGensOnly
  (const MonomialTensor<int, MathRoutines::IntUnsignIdentity>& leftMon,
   const MonomialTensor<int, MathRoutines::IntUnsignIdentity>& rightMon,
   ProgressReport* theProgressReport=0)
  ;
  Rational hwTrace
  (const Pair<MonomialTensor<int, MathRoutines::IntUnsignIdentity>, MonomialTensor<int, MathRoutines::IntUnsignIdentity> >& thePair,
   ProgressReport* theProgressReport=0)
  ;
  void TestConsistency(GlobalVariables& theGlobalVariables);
public:
  SemisimpleLieAlgebra* owneR;
  HashedList<MonomialUniversalEnveloping<CoefficientType> > theGeneratingWordsNonReduced;
  //Note: for some reason, the linker fails to resolve without the explicit template
  //specialization below.
  //[Update:] mae a bug report on this in the gcc bug tracker.
  //This issue has officially been recognized as a bug. Hope to get a fix soon.
  HashedListSpecialized
  <MonomialTensor<int, MathRoutines::IntUnsignIdentity> >
  theGeneratingWordsNonReducedInt;
  Vectors<Rational> theGeneratingWordsWeightsPlusWeightFDpart;

  List<LittelmannPath> thePaths;

  List<List<MonomialUniversalEnveloping<CoefficientType> > > theGeneratingWordsGrouppedByWeight;
  List<List<MonomialTensor<int, MathRoutines::IntUnsignIdentity> > > theGeneratingWordsIntGrouppedByWeight;
//  List<ElementUniversalEnveloping<CoefficientType> > theSimpleGens;
//  List<List<List<ElementUniversalEnveloping<CoefficientType> > > > actionsSimpleGens;
//  List<Matrix<CoefficientType> > actionsSimpleGensMatrixForM;
  List<Matrix<CoefficientType> > theBilinearFormsAtEachWeightLevel;
  List<Matrix<CoefficientType> > theBilinearFormsInverted;
//  Vectors<Rational> weightsSimpleGens;

  Vector<CoefficientType> theHWDualCoordsBaseFielD;
  Vector<CoefficientType> theHWSimpleCoordSBaseField;
  Vector<CoefficientType> theHWFundamentalCoordsBaseField;

  Vector<Rational> theHWFDpartDualCoords;
  Vector<Rational> theHWFDpartSimpleCoordS;
  Vector<Rational> theHWFDpartFundamentalCoordS;

//  List<List<Matrix<CoefficientType> > >
  HashedList<Vector<Rational> > theModuleWeightsSimpleCoords;
  charSSAlgMod<CoefficientType> theCharOverH;
  charSSAlgMod<CoefficientType> theChaR;
  Selection parabolicSelectionNonSelectedAreElementsLevi;
  Selection parabolicSelectionSelectedAreElementsLevi;
  std::string highestWeightVectorNotation;
  //Note: for some reason, the linker fails to resolve without the
  //explicit template specialization below.
  //[Update:] This is now a recognized gcc bug.
  HashedListSpecialized
  <
  Pair<MonomialTensor<int, MathRoutines::IntUnsignIdentity>, MonomialTensor<int, MathRoutines::IntUnsignIdentity> >
  > cachedPairs;
  List<Rational> cachedTraces;

  bool flagIsInitialized;
  bool flagConjectureBholds;
  bool flagConjectureCholds;
  int NumCachedPairsBeforeSimpleGen;
  int NumRationalMultiplicationsAndAdditionsBeforeSimpleGen;
  int MaxNumCachedPairs;

  bool operator==(const ModuleSSalgebra<CoefficientType>& other)
  { return
    this->owneR==other.owneR
    && this->theHWFundamentalCoordsBaseField==other.theHWFundamentalCoordsBaseField
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
  void operator=(const ModuleSSalgebra<CoefficientType>& other)
  { if (this==&other)
      return;
    this->NumCachedPairsBeforeSimpleGen=other.NumCachedPairsBeforeSimpleGen;
    this->NumRationalMultiplicationsAndAdditionsBeforeSimpleGen
    =other.NumRationalMultiplicationsAndAdditionsBeforeSimpleGen;
    this->cachedPairs=other.cachedPairs;
    this->cachedTraces=other.cachedTraces;
    this->actionsGeneratorsMaT=other.actionsGeneratorsMaT;
    this->actionsGeneratorS=other.actionsGeneratorS;
    this->ComputedGeneratorActions=other.ComputedGeneratorActions;
    this->owneR=other.owneR;
    this->theGeneratingWordsNonReduced= other.theGeneratingWordsNonReduced;
    this->theGeneratingWordsNonReducedInt=other.theGeneratingWordsNonReducedInt;
    this->theGeneratingWordsWeightsPlusWeightFDpart=other.theGeneratingWordsWeightsPlusWeightFDpart;
    this->theGeneratingWordsGrouppedByWeight= other.theGeneratingWordsGrouppedByWeight;
    this->theGeneratingWordsIntGrouppedByWeight=other.theGeneratingWordsIntGrouppedByWeight;
//    this->theSimpleGens=other.theSimpleGens;
    this->theBilinearFormsAtEachWeightLevel=other.theBilinearFormsAtEachWeightLevel;
    this->theBilinearFormsInverted=other.theBilinearFormsInverted;
//    this->weightsSimpleGens=other.weightsSimpleGens;
    this->theHWFDpartDualCoords = other.theHWFDpartDualCoords;
    this->theHWFDpartSimpleCoordS=other.theHWFDpartSimpleCoordS;
    this->theHWFDpartFundamentalCoordS=other.theHWFDpartFundamentalCoordS;
    this->theHWDualCoordsBaseFielD=other.theHWDualCoordsBaseFielD;
    this->theHWSimpleCoordSBaseField=other.theHWSimpleCoordSBaseField;
    this->theHWFundamentalCoordsBaseField= other.theHWFundamentalCoordsBaseField;
    this->theModuleWeightsSimpleCoords=other.theModuleWeightsSimpleCoords;
    this->theCharOverH=other.theCharOverH;
    this->theChaR=other.theChaR;
    this->parabolicSelectionNonSelectedAreElementsLevi=other.parabolicSelectionNonSelectedAreElementsLevi;
    this->parabolicSelectionSelectedAreElementsLevi=other.parabolicSelectionSelectedAreElementsLevi;
    this->flagIsInitialized=other.flagIsInitialized;
    this->flagConjectureBholds=other.flagConjectureBholds;
    this->flagConjectureCholds=other.flagConjectureCholds;
    this->highestWeightVectorNotation=other.highestWeightVectorNotation;
    this->thePaths=other.thePaths;
//    std::cout << "<hr><hr><b>Copying from:</b> " << other.ToString()
//    << "<b>Copy result:</b>" << this->ToString() << "<br><b>End of copy</b><hr><hr>";

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
  void GetFDchar(charSSAlgMod<CoefficientType>& output);
  void Substitution(const PolynomialSubstitution<Rational>& theSub);
//  List<ElementUniversalEnveloping<CoefficientType> > theGeneratingWordsLittelmannForm;
//  HashedList<MonomialUniversalEnveloping<CoefficientType> > theGeneratingMonsPBWform;
//  List
//  List<Matrix<CoefficientType> > ActionsChevalleyGenerators;
  MatrixTensor<CoefficientType>& GetActionGeneratorIndeX
  (int generatorIndex, GlobalVariables& theGlobalVariables,
 const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
 ;
  MatrixTensor<CoefficientType>& GetActionSimpleGeneratorIndex
  (int generatorIndex, GlobalVariables& theGlobalVariables,
 const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0)
 ;
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
  void IntermediateStepForMakeFromHW
( Vector<CoefficientType>& HWDualCoordS, GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit,
  const CoefficientType& theRingZero)
  ;
  bool MakeFromHW
(SemisimpleLieAlgebra& inputAlgebra, Vector<CoefficientType>& HWFundCoords,
 const Selection& selNonSelectedAreElementsLevi, GlobalVariables& theGlobalVariables,
const CoefficientType& theRingUnit, const CoefficientType& theRingZero,
 std::string* outputReport, bool computeSimpleGens=true)
  ;
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
  (ElementUniversalEnveloping<CoefficientType>& inputHomogeneous, Vector<Rational> & weightUEEltSimpleCoords,
   List<List<ElementUniversalEnveloping<CoefficientType> > >& outputSortedByArgumentWeight,
   GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
   ;
  void GetMatrixHomogenousElt
  (ElementUniversalEnveloping<CoefficientType>& inputHomogeneous,
   List<List<ElementUniversalEnveloping<CoefficientType> > >& outputSortedByArgumentWeight,
    Vector<Rational> & weightUEEltSimpleCoords, MatrixTensor<CoefficientType>& output,
   GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
   ;
  void ExpressAsLinearCombinationHomogenousElement
  (ElementUniversalEnveloping<CoefficientType>& inputHomogeneous,
   ElementUniversalEnveloping<CoefficientType>& outputHomogeneous,
   int indexInputBasis, const Vector<CoefficientType>& subHiGoesToIthElement,
   GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero
   )
  ;
  std::string ToString(FormatExpressions* theFormat=0)const;
  std::string ElementToStringHWV(FormatExpressions* theFormat=0)const
  { if (this->highestWeightVectorNotation!="")
      return this->highestWeightVectorNotation;
    std::stringstream out;
    out << "v_{" << this->theHWFundamentalCoordsBaseField.ToString(theFormat)
    << ", " << this->parabolicSelectionNonSelectedAreElementsLevi.ToString() << "}";
    return out.str();
//    return "hwv{}("+ this->GetOwner().GetLieAlgebraName(false)+ "," + this->theHWFundamentalCoordsBaseField.ToString(theFormat) + ","
//    + Vector<Rational> (this->parabolicSelectionNonSelectedAreElementsLevi).ToString(theFormat) + ")";
  }
  void SplitOverLevi
  (std::string* Report, Selection& splittingParSel, GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit,
   const CoefficientType& theRingZero, List<ElementUniversalEnveloping<CoefficientType> >* outputEigenVectors=0,
   Vectors<CoefficientType>* outputWeightsFundCoords=0, Vectors<CoefficientType>* outputEigenSpace=0, charSSAlgMod<CoefficientType>* outputChar=0)
   ;
  void SplitFDpartOverFKLeviRedSubalg
  (HomomorphismSemisimpleLieAlgebra& theHmm, Selection& LeviInSmall,
   GlobalVariables& theGlobalVariables, List<ElementUniversalEnveloping<CoefficientType> >* outputEigenVectors=0,
   Vectors<CoefficientType>* outputWeightsFundCoords=0, Vectors<CoefficientType>* outputEigenSpace=0,
   std::stringstream* comments=0, const CoefficientType& theRingUnit=1,
   const CoefficientType& theRingZero=0)
   ;
   template<class ResultType>
   void GetElementsNilradical
(List<ElementUniversalEnveloping<ResultType> >& output, bool useNegativeNilradical,
 List<int>* listOfGenerators=0)
   ;
   bool IsNotInLevi
(int theGeneratorIndex)
   ;
   bool IsNotInParabolic
(int theGeneratorIndex)
   ;
   void GetGenericUnMinusElt
   (bool shiftPowersByNumVarsBaseField, ElementUniversalEnveloping<Polynomial<Rational> >& output, GlobalVariables& theGlobalVariables)
   ;
   void GetGenericUnMinusElt
   (bool shiftPowersByNumVarsBaseField, ElementUniversalEnveloping<RationalFunctionOld>& output, GlobalVariables& theGlobalVariables)
   ;
   //The input of the following function is supposed to be an honest element of the
   //Universal enveloping,
   //i.e. inputElt is not allowed to have non-small integer exponents.
   bool GetActionGenVermaModuleAsDiffOperator
(ElementSemisimpleLieAlgebra<Rational>& inputElt, quasiDiffOp<Rational>& output,
  GlobalVariables& theGlobalVariables)
   ;
bool GetActionEulerOperatorPart
(const MonomialP& theCoeff, ElementWeylAlgebra& outputDO, GlobalVariables& theGlobalVariables)
   ;
   ModuleSSalgebra() : owneR(0), flagIsInitialized(false), MaxNumCachedPairs(1000000)
   {}
};

template<class CoefficientType>
class MonomialGeneralizedVerma
{
  public:
  ModuleSSalgebra<CoefficientType>* owneR;
  MonomialUniversalEnveloping<CoefficientType> theMonCoeffOne;
  int indexFDVector;
  MonomialGeneralizedVerma(): owneR(0), indexFDVector(-1) { }
  friend std::ostream& operator << (std::ostream& output, const MonomialGeneralizedVerma<CoefficientType>& theGen)
  { output << theGen.ToString();
    return output;
  }
  void MultiplyMeByUEEltOnTheLefT
  (const ElementUniversalEnveloping<CoefficientType>& theUE, ElementSumGeneralizedVermas<CoefficientType>& output,
   GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
   ;
  void operator=(const MonomialGeneralizedVerma<CoefficientType>& other)
  { this->owneR=other.owneR;
    this->indexFDVector=other.indexFDVector;
    this->theMonCoeffOne=other.theMonCoeffOne;
  }

  std::string ToString
  (FormatExpressions* theFormat=0, bool includeV=true)const
  ;
  static const bool IsEqualToZero(){return false;}
  bool operator==(const MonomialGeneralizedVerma<CoefficientType>& other)const
  { if (this->indexFDVector==other.indexFDVector && this->owneR==other.owneR)
      return this->theMonCoeffOne==other.theMonCoeffOne;
    return false;
  }
  void SetNumVariables(int GoalNumVars)
  { assert(this->owneR->size>this->indexInOwner);
    this->theMonCoeffOne.SetNumVariables(GoalNumVars);
    this->owneR->TheObjects[this->indexInOwner].SetNumVariables(GoalNumVars);
  }
  void Substitution
  (const PolynomialSubstitution<Rational>& theSub, ListReferences<ModuleSSalgebra<CoefficientType> >& theMods);
  unsigned int HashFunction()const
  { return this->indexFDVector*SomeRandomPrimes[0]+ (*((unsigned *)&(this->owneR)))*SomeRandomPrimes[1];
  }
  static inline unsigned int HashFunction(const MonomialGeneralizedVerma<CoefficientType>& input)
  { return input.HashFunction();
  }
  bool operator>(const MonomialGeneralizedVerma<CoefficientType>& other)
  { if (this->owneR!=other.owneR)
  // use of ulong is correct on i386, amd64, and a number of other popular platforms
  // uintptr_t is only available in c++0x
      return (unsigned long)this->owneR>(unsigned long)other.owneR;
    if (this->indexFDVector!=other.indexFDVector)
      return this->indexFDVector>other.indexFDVector;
    return this->theMonCoeffOne>other.theMonCoeffOne;
  }
  void ReduceMe
(ElementSumGeneralizedVermas<CoefficientType>& output, GlobalVariables& theGlobalVariables,
  const CoefficientType& theRingUnit, const CoefficientType& theRingZero)const
   ;
  bool IsHWV()const
  { if (!this->theMonCoeffOne.IsEqualToOne())
      return false;
//    std::cout << "<br>hi, my name is: " << this->ToString() << " and my index is: " << this->indexFDVector
//    << " and this->GetOwner().GetDim()-1 is " << this->GetOwner().GetDim()-1;
    return this->GetOwner().GetDim()-1==this->indexFDVector;
  }
  void MakeConst(ModuleSSalgebra<CoefficientType>& inputOwner)
  { this->owneR=&inputOwner;
    this->theMonCoeffOne.MakeConst(*inputOwner.owneR);
  }
  ModuleSSalgebra<CoefficientType>& GetOwner()const
  { return *this->owneR;
  }
};

template<class CoefficientType>
class ElementSumGeneralizedVermas : public MonomialCollection<MonomialGeneralizedVerma<CoefficientType>, CoefficientType >
{
  public:
//  ModuleSSalgebra<CoefficientType>* owneR;
  void MultiplyMeByUEEltOnTheLeft
  (const ElementUniversalEnveloping<CoefficientType>& theUE, GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
  ;
  unsigned int HashFunction()const
  { return this->MonomialCollection<MonomialGeneralizedVerma<CoefficientType>, CoefficientType >::HashFunction();
  }
  static unsigned int HashFunction(const ElementSumGeneralizedVermas<CoefficientType>& input)
  { return input.HashFunction();
  }
  ElementSumGeneralizedVermas(){}
  void MakeHWV
  (ModuleSSalgebra<CoefficientType>& theOwner, const CoefficientType& theRingUnit)
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
  bool ExtractElementUE(ElementUniversalEnveloping<CoefficientType>& output, SemisimpleLieAlgebra& theOwner);
  inline void operator=(const ElementSumGeneralizedVermas<CoefficientType>& other)
  { this->::MonomialCollection<MonomialGeneralizedVerma<CoefficientType>, CoefficientType>:: operator=(other);
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
  friend std::ostream& operator << (std::ostream& output, const MonomialTensorGeneralizedVermas<CoefficientType>& input)
  { output << input.ToString();
    return output;
  }
  static const bool IsEqualToZero(){return false;}
  int GetNumVars()
  { return this->Coefficient.GetNumVars();
  }
  void operator*=(const MonomialTensorGeneralizedVermas<CoefficientType>& other)
  { if (this==&other)
    { MonomialTensorGeneralizedVermas<CoefficientType> tempMon1;
      tempMon1=other;
      *this*=tempMon1;
      return;
    }
    this->theMons.AddListOnTop(other.theMons);
  }
  void operator*=(const MonomialGeneralizedVerma<CoefficientType>& other)
  { this->theMons.AddOnTop(other);
  }
  unsigned int HashFunction()const
  { int numCycles=MathRoutines::Minimum(SomeRandomPrimesSize, this->theMons.size);
    unsigned int result=0;
    for (int i=0; i<numCycles; i++)
      result+=SomeRandomPrimes[i]*this->theMons[i].HashFunction();
    return result;
  }
  static unsigned int HashFunction(const MonomialTensorGeneralizedVermas<CoefficientType>& input)
  { return input.HashFunction();
  }
  void SetNumVariables(int GoalNumVars)
  { for (int i=0; i<this->theMons.size; i++)
      this->theMons[i].SetNumVariables(GoalNumVars);
  }
  void Substitution
  (const PolynomialSubstitution<Rational>& theSub, ListReferences<ModuleSSalgebra<CoefficientType> >& theMods)
  { for (int i=0; i<this->theMons.size; i++)
      this->theMons[i].Substitution(theSub, theMods);
  }
  std::string ToString(FormatExpressions* theFormat=0, bool includeV=true)const
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
  bool IsHWV()const
  { if (this->theMons.size!=1)
      return false;
    return this->theMons[0].IsHWV();
  }
  bool operator>(const MonomialTensorGeneralizedVermas<CoefficientType>& other)const
  { if (this->theMons.size>other.theMons.size)
      return true;
    if (other.theMons.size>this->theMons.size)
      return false;
    ///This might need a rewrite. As it is, it will cause monomials to be sorted according to the
    ///alphabetical order of their human-readable strings. If I have time, I will make a better scheme for
    ///comparison.
    return this->ToString()>other.ToString();
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
  bool MultiplyOnTheLeft
  (const ElementUniversalEnveloping<CoefficientType>& theUE,
   ElementTensorsGeneralizedVermas<CoefficientType>& output,
   SemisimpleLieAlgebra& ownerAlgebra, GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero)const
  ;
  bool MultiplyOnTheLeft
  (const MonomialUniversalEnveloping<CoefficientType>& theUE,
   ElementTensorsGeneralizedVermas<CoefficientType>& output,
   SemisimpleLieAlgebra& ownerAlgebra,
   GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)const
  ;
  void TensorOnTheRight
  (const ElementTensorsGeneralizedVermas<CoefficientType>& right, GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit=1, const CoefficientType& theRingZero=0
   )
   ;
  void MultiplyByElementLieAlg
  (ElementTensorsGeneralizedVermas<CoefficientType>& output,
   SemisimpleLieAlgebra& ownerAlgebra,  int indexGenerator, GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero)const
  ;
  void MultiplyBy
  (const ElementTensorsGeneralizedVermas<CoefficientType>& standsOnTheRight,
   GlobalVariables& theGlobalVariables)
   ;
  bool IsHWV
  ()const
  { if (this->theCoeffs.size!=1)
      return false;
    if (!this->theCoeffs[0].IsEqualToOne())
      return false;
    return this->TheObjects[0].IsHWV();
  }

  void MakeHWV
  (ModuleSSalgebra<CoefficientType>& theOwner, const CoefficientType& theRingUnit)
  ;
  void Substitution
  (const PolynomialSubstitution<Rational>& theSub, ListReferences<ModuleSSalgebra<CoefficientType> >& theMods)
  ;
  void SetNumVariables
  (int GoalNumVars)
  { for (int i=0; i<this->size; i++)
      this->TheObjects[i].SetNumVariables(GoalNumVars);
  }
  int GetIndexLieAlgebra()const
  { for (int i=0; i<this->size; i++)
    { MonomialTensorGeneralizedVermas<CoefficientType>& theMon=this->TheObjects[i];
      if (theMon.theMons.size>0)
        return theMon.theMons[0].owneR->TheObjects[theMon.theMons[0].indexInOwner].indexAlgebra;
    }
    return -1;
  }
  ModuleSSalgebra<CoefficientType>& GetOwnerModule()const
  { if (this->size <=0)
    { std::cout << "This is a programming error: calling GetOwnerModule() on a tensor element which has no monomials."
      << " This is not allowed as the index of the owner modules are stored in the monomials. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    MonomialTensorGeneralizedVermas<CoefficientType>& theMon=this->TheObjects[0];
    if (theMon.theMons.size<=0)
    { std::cout << "This is a programming error: calling GetOwnerModule() on a tensor element which has a constant monomial."
      << " This is not allowed: constant monomials do not have owners. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    MonomialGeneralizedVerma<CoefficientType>& theGmon=theMon.theMons[0];
    return *theGmon.owneR;
  }
  int GetIndexOwnerModule()const
  { for (int i=0; i<this->size; i++)
    { MonomialTensorGeneralizedVermas<CoefficientType>& theMon=this->TheObjects[i];
      if (theMon.theMons.size>0)
        return theMon.theMons[0].indexInOwner;
    }
    return -1;
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
  { return this->:: MonomialCollection<MonomialTensorGeneralizedVermas<CoefficientType>, CoefficientType >::HashFunction();
  }
  void operator=(const ElementTensorsGeneralizedVermas<CoefficientType>& other)
  { this->::MonomialCollection<MonomialTensorGeneralizedVermas<CoefficientType>, CoefficientType>::operator=(other);
  }
  void operator=(const ElementSumGeneralizedVermas<CoefficientType>& other);
  static unsigned int HashFunction(const ElementTensorsGeneralizedVermas<CoefficientType>& input)
  { return input.HashFunction();
  }
};

class PolynomialOverModule;

template <class CoefficientType>
class ElementVermaModuleOrdered
{
public:
  ElementUniversalEnvelopingOrdered<CoefficientType> theElT;
  std::string DebugString;
  PolynomialSubstitution<CoefficientType> theSubNthElementIsImageNthCoordSimpleBasis;
  void ComputeDebugString(){FormatExpressions tempFormat; this->DebugString=this->ToString(tempFormat);}
  std::string ToString(const FormatExpressions& theFormat)const;
  void ToString(std::string& output)const{output=this->ToString();}
  bool IsEqualToZero()const {return this->theElT.IsEqualToZero();}
  bool NeedsBrackets()const;
  void AssignElementUniversalEnvelopingOrderedTimesHighestWeightVector
  (ElementUniversalEnvelopingOrdered<CoefficientType>& input, const ElementVermaModuleOrdered<CoefficientType>& theRingZero,
   GlobalVariables* theContext, const CoefficientType& theRingUnit)
  ;
  void ActOnMe
  (const ElementSemisimpleLieAlgebra<Rational>& actingElt, ElementVermaModuleOrdered<CoefficientType>& output, SemisimpleLieAlgebra& owner, const CoefficientType& theRingUnit, const CoefficientType& theRingZero, GlobalVariables* theContext)const
  ;
  static void GetBasisFromSpanOfElements
  (List<ElementVermaModuleOrdered<CoefficientType> >& theElements, Vectors<RationalFunctionOld>& outputCoordinates, List<ElementVermaModuleOrdered>& outputTheBasis,
    const RationalFunctionOld& RFOne, const RationalFunctionOld& RFZero, GlobalVariables& theGlobalVariables)
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
(const ElementSemisimpleLieAlgebra<Rational>& other,
 ElementVermaModuleOrdered<CoefficientType>& output, const CoefficientType& theRingUnit,
 const CoefficientType& theRingZero, GlobalVariables* theContext)
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
  HomomorphismSemisimpleLieAlgebra theHmm;
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
  WeylGroup WeylSmaller;
  WeylGroup WeylLarger;
  int computationPhase;
  int NumProcessedConesParam;
  int NumProcessedExtremaEqualOne;
  std::string ComputeMultsLargerAlgebraHighestWeight
  (Vector<Rational>& highestWeightLargerAlgebraFundamentalCoords,
   Vector<Rational>& parabolicSel, GlobalVariables& theGlobalVariables)
  ;
  std::string CheckMultiplicitiesVsOrbits
  (GlobalVariables& theGlobalVariables)
  ;
  std::string ElementToStringMultiplicitiesReport
  (GlobalVariables& theGlobalVariables)
  ;
  void IncrementComputation
  (Vector<Rational>& parabolicSel, GlobalVariables& theGlobalVariables)
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
    { //this->theParser.theHmm.MakeG2InB3(this->theParser, *theGlobalVariables);
//      this->initFromHomomorphism(parSel, this->theParser.theHmm, *theGlobalVariables);
    } else
    { GlobalVariables tempGlobalVars;
//      this->theParser.theHmm.MakeG2InB3(this->theParser, tempGlobalVars);
  //    this->initFromHomomorphism(parSel, this->theParser.theHmm, tempGlobalVars);
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
  (FormatExpressions& theFormat, const Cone& theCone, GlobalVariables& theGlobalVariables)
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
(Matrix<Rational>& output, Vector<Rational>& outputTranslation, Vectors<Rational>& NonParams, int numParams)
  ;
  void initQPs
  (GlobalVariables& theGlobalVariables)
  ;
  void initFromHomomorphism
  (Vector<Rational>& theParabolicSel, HomomorphismSemisimpleLieAlgebra& input, GlobalVariables& theGlobalVariables)
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
void Matrix<Element>::GetVectorFromRow(int rowIndex, Vector<Element>& output)const
{ output.SetSize(this->NumCols);
  for (int i=0; i<this->NumCols; i++)
    output.TheObjects[i]=this->elements[rowIndex][i];
}

template<typename Element>
void Matrix<Element>::GetVectorFromColumn(int colIndex, Vector<Element>& output)const
{ output.SetSize(this->NumRows);
  for (int i=0; i<this->NumRows; i++)
    output[i]=this->elements[i][colIndex];
}

class affineHyperplane
{
public:
  std::string DebugString;
  Vector<Rational> affinePoint;
  Vector<Rational> normal;
  void ToString(std::string& output);
  void ComputeDebugString(){this->ToString(this->DebugString); }
  //void InduceFromFacet(Facet& input);
  //the below returns false if the projection is not of full dimension
  unsigned int HashFunction()const;
  static inline unsigned int HashFunction(const affineHyperplane& input)
  { return input.HashFunction();
  }
//  bool ProjectFromFacet(Facet& input);
  bool ProjectFromFacetNormal(Vector<Rational>& input);
  bool ContainsPoint(Vector<Rational> & thePoint);
  void MakeFromNormalAndPoint(Vector<Rational>& inputPoint, Vector<Rational>& inputNormal);
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
  void ToString(std::string& output);
  void ComputeDebugString(){this->ToString(this->DebugString); }
};

class affineCone
{
public:
  affineHyperplanes theWalls;
  unsigned int HashFunction() const;
  inline static unsigned int HashFunction(const affineCone& input){return input.HashFunction();}
  inline int GetDimension();
  void SuperimposeAffineCones(affineCones& theOtherComplex);
  //void induceFromCombinatorialChamber(CombinatorialChamber& input);
  bool WallIsInternalInCone(affineHyperplane& theKillerCandidate);
  //The below function returns true if the system of homogeneous linear inequalities Ax<=b
  //has a solution, false otherwise, where A is a matrix and x and b are column vectors.
//  bool SystemLinearInequalitiesHasSolution
//    (Matrix<Rational> & matA, Matrix<Rational> & matb, Matrix<Rational> & outputPoint);
  bool SplitByAffineHyperplane(affineHyperplane& theKillerPlane, affineCones& output);
  void Assign(const affineCone& right){this->theWalls=(right.theWalls); }
  inline void operator=(const affineCone& right){this->Assign(right); }
};

class affineCones: public HashedList<affineCone>
{
public:
  void SuperimposeAffineCones(affineCones& theOtherComplex);
};

template <class Object>
void List<Object>::QuickSortAscendingNoOrder(int BottomIndex, int TopIndex)
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
  this->QuickSortAscendingNoOrder(BottomIndex, HighIndex-1);
  this->QuickSortAscendingNoOrder(HighIndex+1, TopIndex);
}

template <class Object>
void List<Object>::QuickSortAscendingOrder
(int BottomIndex, int TopIndex, List<Object>::OrderLeftGreaterThanRight theOrder)
{ if (TopIndex<=BottomIndex)
    return;
  int HighIndex = TopIndex;
  for (int LowIndex = BottomIndex+1; LowIndex<=HighIndex; LowIndex++)
    if (theOrder(this->TheObjects[LowIndex],(this->TheObjects[BottomIndex])))
    { this->SwapTwoIndices(LowIndex, HighIndex);
      LowIndex--;
      HighIndex--;
    }
  if (theOrder(this->TheObjects[HighIndex], this->TheObjects[BottomIndex]))
  { if (HighIndex==BottomIndex)
    { std::cout << "This is a programming error. The programmer has given me a bad strict order: "
      << " the order claims that object of index " << HighIndex << " is strictly greater than itself "
      << " which is not allowed for strict orders. Maybe the programmer has given a non-strict order instead of "
      << " strict one by mistake? " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    HighIndex--;
  }
  this->SwapTwoIndices(BottomIndex, HighIndex);
  this->QuickSortAscendingOrder(BottomIndex, HighIndex-1, theOrder);
  this->QuickSortAscendingOrder(HighIndex+1, TopIndex, theOrder);
}

template <class Object>
template <class compareClass>
bool List<Object>::QuickSortAscendingCustom
(int BottomIndex, int TopIndex, compareClass& theCompareror)
{ if (TopIndex<=BottomIndex)
    return true;
  int HighIndex = TopIndex;
  for (int LowIndex = BottomIndex+1; LowIndex<=HighIndex; LowIndex++)
    if (theCompareror.CompareLeftGreaterThanRight
        (this->TheObjects[LowIndex],(this->TheObjects[BottomIndex])))
    { this->SwapTwoIndices(LowIndex, HighIndex);
      LowIndex--;
      HighIndex--;
    }
  if (theCompareror.CompareLeftGreaterThanRight(this->TheObjects[HighIndex],this->TheObjects[BottomIndex]))
    HighIndex--;
  this->SwapTwoIndices(BottomIndex, HighIndex);
  if (!this->QuickSortAscendingCustom(BottomIndex, HighIndex-1, theCompareror))
    return false;
  if (!this->QuickSortAscendingCustom(HighIndex+1, TopIndex, theCompareror))
    return false;
  return true;
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
{ if (index1<0 || index1>=this->size || index2<0 || index2>=this->size)
  { std::cout << "This is a programming error: requested to the elements with indices "
    << index1 << " and " << index2 << " in a list that has " << this->size << " elements. This "
    << " is impossible. " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  if (index1==index2)
    return;
  Object tempO;
  tempO= this->TheObjects[index1];
  this->TheObjects[index1]=this->TheObjects[index2];
  this->TheObjects[index2]=tempO;
}

template<class Object>
int List<Object>::GetIndex(const Object& o) const
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
{ return  sizeof(this->ActualSize)+ sizeof(this->size)+ sizeof(this->TheObjects);
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
{ if (this->GetIndex(o)!=-1)
    return false;
  this->AddOnTop(o);
  return true;
}

template <class Object>
inline Object* List<Object>::LastObject()const
{ // <-Registering stack trace forbidden! Multithreading deadlock alert.
  if (this->size<=0)
  { std::cout << "This is a programming error: trying to fetch the last object of an array with "
    << this->size << " elements. " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  return &this->TheObjects[this->size-1];
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
void List<Object>::ReservE(int theSize)
{ // <-Registering stack trace forbidden! Multithreading deadlock alert.
  if (this->ActualSize<theSize)
  { ParallelComputing::SafePointDontCallMeFromDestructors();
    this->ExpandArrayOnTop(theSize- this->ActualSize);
  }
}

template <class Object>
void List<Object>::RemoveFirstOccurenceSwapWithLast(const Object& o)
{ for (int i=0; i<this->size; i++)
    if (o==this->TheObjects[i])
    { this->RemoveIndexSwapWithLast(i);
      return;
    }
}

template <class Object>
void List<Object>::SetSize(int theSize)
{// <-Registering stack trace forbidden! Multithreading deadlock alert.
  if (theSize<0)
    theSize=0;
  this->ReservE(theSize);
  this->size=theSize;
}

template <class Object>
inline std::string List<Object>::ToString()const
{ std::stringstream out;
  for (int i=0; i<this->size; i++)
    out << this->TheObjects[i].ToString() << "\n";
  return out.str();
}

template <class Object>
inline std::string List<Object>::ToString(FormatExpressions* theFormat)const
{ std::stringstream out;
  for (int i=0; i<this->size; i++)
    out << this->TheObjects[i].ToString(theFormat) << "\n";
  return out.str();
}

template <class Object>
inline void List<Object>::AddObjectOnTopCreateNew()
{ this->SetSize(this->size+1);
}

template <class Object>
void List<Object>::RemoveIndexSwapWithLast(int index)
{ if (this->size==0)
    return;
  this->size--;
  this->TheObjects[index]=this->TheObjects[this->size];
}

template <class Object>
void List<Object>::RemoveLastObject()
{ if (this->size==0)
  { std::cout << "Programming error: attempting to pop empty list"
    << MathRoutines::GetStackTraceEtcErrorMessage(__FILE__,__LINE__);
    assert(false);
  }
  this->size--;
}

template <class Object>
Object List<Object>::PopLastObject()
{ if (this->size==0){
    std::cout << "Programming error: attempting to pop empty list" << MathRoutines::GetStackTraceEtcErrorMessage(__FILE__,__LINE__);
    assert(false);
  }
  this->size--;
  return this->TheObjects[size];
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
{ delete [] this->TheObjects;
  this->size=0;
  this->TheObjects=0;
#ifdef CGIversionLimitRAMuse
ParallelComputing::GlobalPointerCounter-=this->ActualSize;
  ParallelComputing::CheckPointerCounters();
#endif
  this->ActualSize=0;
}

template <class Object>
List<Object>::~List()
{ delete [] this->TheObjects;
#ifdef CGIversionLimitRAMuse
ParallelComputing::GlobalPointerCounter-=this->ActualSize;
  ParallelComputing::CheckPointerCounters();
#endif
  this->size=0;
  this->ActualSize=0;
}

template <class Object>
void List<Object>::ExpandArrayOnTop(int increase)
{// <-Registering stack trace forbidden! Multithreading deadlock alert.
  if (increase<=0)
    return;
  MacroIncrementCounter(NumListResizesTotal);
  Object* newArray = new Object[this->ActualSize+increase];
#ifdef CGIversionLimitRAMuse
ParallelComputing::GlobalPointerCounter+=this->ActualSize+increase;
  ParallelComputing::CheckPointerCounters();
#endif
  for (int i=0; i<this->size; i++)
    newArray[i]=this->TheObjects[i];
  delete [] this->TheObjects;
#ifdef CGIversionLimitRAMuse
ParallelComputing::GlobalPointerCounter-=this->ActualSize;
  ParallelComputing::CheckPointerCounters();
#endif
  this->TheObjects= newArray;
  this->ActualSize+=increase;
}

template <class Object>
void List<Object>::ReverseOrderElements()
{ int tempI= this->size/2;
  for (int i=0; i<tempI; i++)
    this->SwapTwoIndices(i, this->size-i-1);
}

template <class Object>
void List<Object>::AddOnTop(const Object& o)
{// <-Registering stack trace forbidden! Multithreading deadlock alert.
  if (this->size>this->ActualSize)
  { std::cout << "This is a programming error: the actual size of the list is " << this->ActualSize
    << " but this->size equals " << this->size
    << ". " << MathRoutines::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  if (this->size==this->ActualSize)
    this->ExpandArrayOnTop
    (this->GetNewSizeRelativeToExpectedSize(this->ActualSize+1)- this->size);
  this->TheObjects[size]=o;
  this->size++;
}

class KLpolys: public HashedList<Vector<Rational> >
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
  List<List<Polynomial<Rational> > > theKLPolys;
  List<List<Polynomial<Rational> > > theRPolys;
  List<List<Rational> > theKLcoeffs;
  void KLcoeffsToString(List<int>& theKLCoeffs, std::string& output);
  void FindNextToExplore();
  int FindMinimalBruhatNonExplored(List<bool>& theExplored);
  int FindMaximalBruhatNonExplored(List<bool>& theExplored);
  void initTheMults();
  void Compute(int x);
  void Check();
  //returns true if computation succeeded, false otherwise
  bool ComputeRxy(int x, int y, int SimpleReflectionIndex);
  void ComputeKLxy(int x, int y);
  bool IsMaxNonEplored(int index);
  bool IndexGEQIndex(int a, int b);
  bool IndexGreaterThanIndex(int a, int b);
  void ComputeDebugString();
  std::string ToString(FormatExpressions* theFormat=0);
  void MergeBruhatLists(int fromList, int toList);
  std::string KLPolysToString(FormatExpressions* theFormat=0);
  void ComputeKLcoefficients();
  int ChamberIndicatorToIndex(Vector<Rational>& ChamberIndicator);
  std::string RPolysToString(FormatExpressions* theFormat=0);
  bool ComputeKLPolys(WeylGroup* theWeylGroup);
  void ComputeRPolys();
  int ComputeProductfromSimpleReflectionsActionList(int x, int y);
  void WriteKLCoeffsToFile(std::fstream& output, List<int>& KLcoeff, int TopIndex);
  //returns the TopIndex of the KL coefficients
  int ReadKLCoeffsFromFile(std::fstream& input, List<int>& output);
  KLpolys(){this->TheWeylGroup=0; }
  void GeneratePartialBruhatOrder();
  void ExtendOrder();
  void ComputeFullBruhatOrder();
  void initFromWeyl(WeylGroup* theWeylGroup);
};

template<class CoefficientType>
void ElementVermaModuleOrdered<CoefficientType>::GetBasisFromSpanOfElements
  (List<ElementVermaModuleOrdered>& theElements, Vectors<RationalFunctionOld>& outputCoordinates, List<ElementVermaModuleOrdered>& outputTheBasis,
    const RationalFunctionOld& RFOne, const RationalFunctionOld& RFZero, GlobalVariables& theGlobalVariables)
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
(const ElementSemisimpleLieAlgebra<Rational>& other,
 ElementVermaModuleOrdered<CoefficientType>& output, const CoefficientType& theRingUnit,
 const CoefficientType& theRingZero, GlobalVariables* theContext)
{ ElementUniversalEnvelopingOrdered<CoefficientType> tempElt;
  tempElt.AssignElementLieAlgebra(other, theRingUnit, theRingZero, *this->theElT.owner);
  //std::cout << "<br>multiplying " << tempElt.ToString() << " times " << this->ToString();
  tempElt*=this->theElT;
  output.theElT=tempElt;
  //std::cout << "<br> ... and the result before simplifying is: " << output.theElT.ToString();
  output.theElT.Simplify(theContext, theRingUnit, theRingZero);
  //std::cout << "<br> before modding out we get: " << output.theElT.ToString();
  output.theElT.ModOutVermaRelationSOld(false, this->theSubNthElementIsImageNthCoordSimpleBasis, theContext, theRingUnit);
  //std::cout << "<br> finally we get: " << output.theElT.ToString();
}

template <class CoefficientType>
std::string ElementVermaModuleOrdered<CoefficientType>::ToString(const FormatExpressions& theFormat)const
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
  (const ElementSemisimpleLieAlgebra<Rational>& actingElt, ElementVermaModuleOrdered<CoefficientType>& output, SemisimpleLieAlgebra& owner,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero, GlobalVariables* theContext)const
{ ElementUniversalEnvelopingOrdered<CoefficientType> tempElt;
  tempElt.AssignElementLieAlgebra(actingElt, theRingUnit, theRingZero, *this->theElT.owner);
//  std::cout << "<br>" << actingElt.ToString() << " acts on " << this->ToString();
  tempElt.LieBracketOnTheRight(this->theElT, output.theElT);
  output.theElT.Simplify(theContext, theRingUnit, theRingZero);
  output.theSubNthElementIsImageNthCoordSimpleBasis=this->theSubNthElementIsImageNthCoordSimpleBasis;
//  std::cout << "<br>and the result before modding out is: " << output.ToString();
//  int numVars=output.theElt.GetNumVars();
  output.theElT.ModOutVermaRelationSOld(false, this->theSubNthElementIsImageNthCoordSimpleBasis, theContext, theRingUnit);
//  std::cout << "<br>and after modding out we get: " << output.ToString();
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
      outputCorrespondingMonomials.AddOnTopNoRepetition(theElements.TheObjects[i].TheObjects[j]);
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
    { this->RemoveIndexSwapWithLast(i);
      i--;
    }
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::operator+=(const ElementUniversalEnvelopingOrdered<CoefficientType>& other)
{ this->ReservE(this->size+other.size);
  for (int i=0; i<other.size; i++)
    this->AddMonomialNoCleanUpZeroCoeff(other.TheObjects[i]);
  this->CleanUpZeroCoeff();
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::operator-=(const ElementUniversalEnvelopingOrdered<CoefficientType>& other)
{ this->ReservE(this->size+other.size);
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
//  tempS=other.ToString();
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
  output.ReservE(this->size*other.size);
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
    if (output[IndexlowestNonSimplified].Coefficient.IsEqualToZero())
      reductionOccurred=true;
    else
      for (int i=0; i<output[IndexlowestNonSimplified].generatorsIndices.size-1; i++)
        if (output[IndexlowestNonSimplified].generatorsIndices[i]>output[IndexlowestNonSimplified].generatorsIndices[i+1])
        { if (output[IndexlowestNonSimplified].SwitchConsecutiveIndicesIfTheyCommute
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
      output.RemoveIndexSwapWithLast(IndexlowestNonSimplified);
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
  ElementSemisimpleLieAlgebra<Rational> tempElt;
  this->owner->theOwner.LieBracket(this->owner->theOrder.TheObjects[theLeftGeneratorIndex], this->owner->theOrder.TheObjects[theRightGeneratorIndex], tempElt);
  if (tempElt.IsEqualToZero())
  { output.generatorsIndices.ReservE(this->generatorsIndices.size);
    output.Powers.ReservE(this->generatorsIndices.size);
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
  tempMon.Powers.ReservE(this->generatorsIndices.size+2);
  tempMon.generatorsIndices.ReservE(this->generatorsIndices.size+2);
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
//  if (this->ToString()=="2f_{5}f_{-5}f_{-4}" || this->ToString()=="2f_{11}f_{-4}")
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
  ElementSemisimpleLieAlgebra<Rational> adResulT, tempElT, theLeftElt;
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
  tempMon.Powers.SetExpectedSize(this->generatorsIndices.size+2);
  tempMon.generatorsIndices.SetExpectedSize(this->generatorsIndices.size+2);
  tempMon.Powers.size=0;
  tempMon.generatorsIndices.size=0;
  int rightGeneratorIndex= this->generatorsIndices.TheObjects[theIndeX+1];
  int leftGeneratorIndex=this->generatorsIndices.TheObjects[theIndeX];
  CoefficientType theRightPower, theLeftPower;
  theRightPower= this->Powers.TheObjects[theIndeX+1];
  theLeftPower= this->Powers.TheObjects[theIndeX];
  theLeftPower-=1;
  int powerDrop=0;
 /* if (this->ToString()=="2f_{5}f_{-5}f_{-4}" || this->ToString()=="2f_{11}f_{-4}")
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
  ElementSemisimpleLieAlgebra<Rational> adResult, tempElt, tempRightElt;
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
  if (theLeftPower.IsSmallInteger(&tempInt))
  { if(theRightPower.IsSmallInteger(&tempInt))
      return true;
    int numPosRoots=this->owner->theOwner.theWeyl.RootsOfBorel.size;
    int theDimension= this->owner->theOwner.theWeyl.CartanSymmetric.NumRows;
    if(rightGeneratorIndex>= numPosRoots && rightGeneratorIndex<numPosRoots+theDimension)
    { ElementSemisimpleLieAlgebra<Rational> tempElt;
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
void Matrix<Element>::ComputeDeterminantOverwriteMatrix(Element& output, const Element& theRingOne, const Element& theRingZero)
{ int tempI;
  output=theRingOne;
  Element tempRat;
  assert(this->NumCols==this->NumRows);
  int dim =this->NumCols;
  for (int i=0; i<dim; i++)
  {  //this->ComputeDebugString();
    tempI = this->FindPivot(i, i);
    if (tempI==-1)
    { output=theRingZero;
      return;
    }
    this->SwitchTwoRows(i, tempI);
    if(tempI!=i)
      output*=-1;
    tempRat=(this->elements[i][i]);
    output*=(tempRat);
    tempRat.Invert();
    this->RowTimesScalar(i, tempRat);
    for (int j=i+1; j<dim; j++)
      if (!this->elements[j][i].IsEqualToZero())
      { tempRat=(this->elements[j][i]);
        tempRat.Minus();
        this->AddTwoRows (i, j, i, tempRat);
      }
  }
}

template<class CoefficientType>
Rational ModuleSSalgebra<CoefficientType>::hwTrace
(const Pair
 <MonomialTensor<int, MathRoutines::IntUnsignIdentity>,
 MonomialTensor<int, MathRoutines::IntUnsignIdentity> >
 & thePair, ProgressReport* theProgressReport
   )
{ MacroRegisterFunctionWithName("ModuleSSalgebra<CoefficientType>::hwTrace");
  int indexInCache=this->cachedPairs.GetIndex(thePair);
  if (indexInCache!=-1)
    return this->cachedTraces[indexInCache];
  if (thePair.Object1.generatorsIndices.size==0)
    return 1;
  Pair<MonomialTensor<int, MathRoutines::IntUnsignIdentity>,
  MonomialTensor<int, MathRoutines::IntUnsignIdentity> > newPair;
  MonomialTensor<int, MathRoutines::IntUnsignIdentity>& newLeft=newPair.Object1;
  MonomialTensor<int, MathRoutines::IntUnsignIdentity>& newRight=newPair.Object2;
  const MonomialTensor<int, MathRoutines::IntUnsignIdentity>& oldRight=thePair.Object2;
  newLeft=thePair.Object1;
  (*newLeft.Powers.LastObject())-=1;
  int theIndex= *newLeft.generatorsIndices.LastObject();
  if (*newLeft.Powers.LastObject()==0)
  { newLeft.generatorsIndices.size--;
    newLeft.Powers.size--;
  }
  int theIndexMinus=
  2*this->GetOwner().GetNumPosRoots() + this->GetOwner().GetRank()-theIndex-1;
  int theSimpleIndex= theIndex-this->GetOwner().GetNumPosRoots()-this->GetOwner().GetRank();
  MonomialTensor<int, MathRoutines::IntUnsignIdentity> Accum;
  Accum.Powers.ReservE(oldRight.Powers.size);
  Accum.generatorsIndices.ReservE(oldRight.generatorsIndices.size);
  Vector<Rational> RemainingWeight;
  Rational result=0;
  Rational summand;
  WeylGroup& theWeyl=this->GetOwner().theWeyl;
//  std::stringstream tempStr;
//  tempStr << thePair.Object2;
//  if (tempStr.str()=="g_{1}^{1}g_{2}^{1}")
//    std::cout << "<hr><hr>";
//  std::cout << "<br>Computing (" << thePair.Object1 << ", " << thePair.Object2 << ")";
//  if (this->cachedPairs.size<this->MaxNumCachedPairs)
//  { indexInCache=this->cachedPairs.size;
//    this->cachedPairs.AddOnTop(thePair);
//    this->cachedTraces.AddOnTop(0);
//  }
  for (int i=0; i<oldRight.generatorsIndices.size; i++)
  { if (oldRight.generatorsIndices[i]==theIndexMinus)
    { summand=0;
      newRight=Accum;
      newRight.MultiplyByGeneratorPowerOnTheRight
      (oldRight.generatorsIndices[i], oldRight.Powers[i]-1);
      RemainingWeight.MakeZero(theWeyl.GetDim());
      for (int j=i+1; j<oldRight.generatorsIndices.size; j++)
      { newRight.MultiplyByGeneratorPowerOnTheRight
        (oldRight.generatorsIndices[j], oldRight.Powers[j]);
        RemainingWeight+=theWeyl.RootSystem[oldRight.generatorsIndices[j]]* oldRight.Powers[j];
      }
      RemainingWeight+=this->theHWFDpartSimpleCoordS;
//      std::cout << "<br>Remaining weight: " << RemainingWeight.ToString();
      summand+=theWeyl.GetScalarProdSimpleRoot(RemainingWeight, theSimpleIndex);
      summand*=2;
      summand/=theWeyl.CartanSymmetric.elements[theSimpleIndex][theSimpleIndex];
//      std::cout << "<br>The scalar product: " << summand.ToString();
      summand+=1;
      summand-=oldRight.Powers[i];
 //     std::cout << "<br>summand: " << summand.ToString();
      if (!summand.IsEqualToZero())
        summand*=this->hwTrace(newPair, theProgressReport);
//      std::cout << "<br>summand after recursion: " << summand.ToString();
      summand*=oldRight.Powers[i];
      result+=summand;
    }
    Accum.generatorsIndices.AddOnTop(oldRight.generatorsIndices[i]);
    Accum.Powers.AddOnTop(oldRight.Powers[i]);
  }
//  if (indexInCache!=-1)
//    this->cachedTraces[indexInCache]=result;
//  if (ProjectInformation::GetMainProjectInfo().CustomStackTrace.size<35)
  if (this->cachedPairs.size<this->MaxNumCachedPairs)
  { this->cachedPairs.AddOnTop(thePair);
    this->cachedTraces.AddOnTop(result);
  }
  if (theProgressReport!=0 && this->cachedPairs.size<500000)
  { std::stringstream tempStream;
    tempStream << "Number of cached pairs: " << this->cachedPairs.size
    << " at recursion depth " << ProjectInformation::GetMainProjectInfo().CustomStackTrace.size;
    theProgressReport->Report(tempStream.str());
  }

//  std::cout << "<br>Computed (" << thePair.Object1 << ", "
//  << thePair.Object2 << ")=" << result.ToString();
//  if (tempStr.str()=="g_{1}^{1}g_{2}^{1}")
//    std::cout << "<hr><hr>";

  return result;
}

template<class CoefficientType>
void ModuleSSalgebra<CoefficientType>::ApplyTAA
(MonomialTensor<int, MathRoutines::IntUnsignIdentity>& theMon)
{ for (int i=0; i<theMon.generatorsIndices.size; i++)
    theMon.generatorsIndices[i]=
    this->GetOwner().GetNumPosRoots()*2+
    this->GetOwner().GetRank()-theMon.generatorsIndices[i]-1;
  theMon.Powers.ReverseOrderElements();
  theMon.generatorsIndices.ReverseOrderElements();
}

template<class CoefficientType>
Rational ModuleSSalgebra<CoefficientType>::hwtaabfSimpleGensOnly
  (const MonomialTensor<int, MathRoutines::IntUnsignIdentity>& leftMon,
   const MonomialTensor<int, MathRoutines::IntUnsignIdentity>& rightMon,
   ProgressReport* theProgressReport)
{ MacroRegisterFunctionWithName("ModuleSSalgebra<CoefficientType>::hwtaabfSimpleGensOnly");
  const MonomialTensor<int, MathRoutines::IntUnsignIdentity>* left=&leftMon;
  const MonomialTensor<int, MathRoutines::IntUnsignIdentity>* right=&rightMon;
  if (leftMon>rightMon)
    MathRoutines::swap(left, right);
  Pair<MonomialTensor<int, MathRoutines::IntUnsignIdentity>,
  MonomialTensor<int, MathRoutines::IntUnsignIdentity> > thePair;
  thePair.Object1=*left;
  thePair.Object2=*right;
//  std::cout << "<br>Computing " << thePair.Object1 << " times " << thePair.Object2 << "<br>";
  this->ApplyTAA(thePair.Object1);
  Rational result= this->hwTrace(thePair, theProgressReport);
  if (theProgressReport!=0)
  { std::stringstream tempStream;
    tempStream << this->cachedPairs.size << " total cached pairs";
    theProgressReport->Report(tempStream.str());
  }
  return result;
}

template<class Element>
void Matrix<Element>::Substitution(const PolynomialSubstitution<Rational>& theSub)
{ for (int i=0; i<this->NumRows; i++)
    for (int j=0; j<this->NumCols; j++)
      this->elements[i][j].Substitution(theSub);
}

template<class CoefficientType>
void ModuleSSalgebra<CoefficientType>::Substitution
(const PolynomialSubstitution<Rational>& theSub)
{ for (int i=0; i<this->actionsGeneratorsMaT.size; i++)
    this->actionsGeneratorsMaT[i].Substitution(theSub);
  for (int i=0; i<this->actionsGeneratorS.size; i++)
    for (int j=0; j<this->actionsGeneratorS[i].size; j++)
      for (int k=0; k<this->actionsGeneratorS[i][j].size; k++)
        this->actionsGeneratorS[i][j][k].Substitution(theSub);
  List<MonomialUniversalEnveloping<CoefficientType> > oldGeneratingWordsNonReduced;
  oldGeneratingWordsNonReduced=(this->theGeneratingWordsNonReduced);
  this->theGeneratingWordsNonReduced.Clear();
  for (int i=0; i<oldGeneratingWordsNonReduced.size; i++)
  { oldGeneratingWordsNonReduced[i].Substitution(theSub);
    this->theGeneratingWordsNonReduced.AddOnTop(oldGeneratingWordsNonReduced[i]);
  }
  for (int i=0; i<this->theGeneratingWordsGrouppedByWeight.size; i++)
    for (int j=0; j<this->theGeneratingWordsGrouppedByWeight[i].size; j++)
      this->theGeneratingWordsGrouppedByWeight[i][j].Substitution(theSub);
  for (int i=0; i<this->theBilinearFormsAtEachWeightLevel.size; i++)
  { this->theBilinearFormsAtEachWeightLevel[i].Substitution(theSub);
    this->theBilinearFormsInverted[i].Substitution(theSub);
  }
  for (int i=0; i<this->theHWDualCoordsBaseFielD.size; i++)
  { this->theHWDualCoordsBaseFielD[i].Substitution(theSub);
    this->theHWFundamentalCoordsBaseField[i].Substitution(theSub);
    this->theHWSimpleCoordSBaseField[i].Substitution(theSub);
  }
}

template <class CoefficientType>
std::string MonomialTensorGeneralizedVermas<CoefficientType>::ToString
  (FormatExpressions* theFormat, bool includeV)const
{ std::stringstream out;
  std::string tempS;
  if (this->theMons.size>1)
    for (int i=0; i<this->theMons.size; i++)
    { bool ishwv=this->theMons[i].IsHWV();
      if (!ishwv)
        out << "(";
      out << this->theMons[i].ToString(theFormat, includeV);
      if (!ishwv)
        out  << ")";
      if (i!=this->theMons.size-1)
        out << "\\otimes ";
    }
  else
    out << this->theMons[0].ToString(theFormat, includeV);
//  std::cout << "<br>" << out.str() << " has " << this->theMons.size << " multiplicands with hash functions: ";
//  for (int i=0; i<this->theMons.size; i++)
//    std::cout << this->theMons[i].HashFunction() << ", ";
  return out.str();
}

template <class CoefficientType>
std::string MonomialGeneralizedVerma<CoefficientType>::ToString(FormatExpressions* theFormat, bool includeV)const
{ if (this->owneR==0)
  { std::cout << "This is a programming error: non-initialized generalized Verma monomial (owner is 0)."
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  ModuleSSalgebra<CoefficientType>& theMod=*this->owneR;
  std::string tempS;
  if (tempS=="1")
    tempS="";
  if (tempS=="-1")
    tempS="-";
  tempS+=this->theMonCoeffOne.ToString(theFormat);
  if (tempS=="1")
    tempS="";
  if (tempS=="-1")
    tempS="-";
  bool needsCdot=(tempS!="1" && tempS!="-" && tempS!="");
  std::stringstream out;
  out << tempS;
  tempS= theMod.theGeneratingWordsNonReduced[this->indexFDVector].ToString(theFormat);
  if (tempS!="1")
    out << tempS;
  needsCdot=needsCdot || (tempS!="1" && tempS!="-");
  if (needsCdot)
    out << "\\cdot ";
  if (includeV)
    out << theMod.ElementToStringHWV(theFormat);
//  std::cout << "<br>MonomialP " << out.str() << " has indexInOwner " << this->indexInOwner;
  return out.str();
}

template <class TemplateMonomial, class CoefficientType>
std::ostream& operator<<
  (std::ostream& output,
   const MonomialCollection<TemplateMonomial, CoefficientType>& theCollection)
{ if (theCollection.size==0)
  { output << "0";
    return output;
  }
  std::string tempS1, tempS2;
  List<TemplateMonomial> sortedMons;
  sortedMons=theCollection;
  sortedMons.QuickSortDescending();
//  out << "(hash: " << this->HashFunction() << ")";
  int cutOffCounter=0;
  std::string oldCustomTimes="";
  for (int i=0; i<sortedMons.size; i++)
  { TemplateMonomial& currentMon=sortedMons[i];
    std::stringstream tempStream;
    CoefficientType& currentCoeff=theCollection.theCoeffs[theCollection.GetIndex(currentMon)];
    tempStream << currentCoeff;
    tempS1=tempStream.str();
    tempS2=currentMon.ToString();
    if (tempS1=="1" && tempS2!="1")
      tempS1="";
    if (tempS1=="-1"&& tempS2!="1")
      tempS1="-";
    if(tempS2!="1")
      tempS1+=tempS2;
    if (i>0)
    { if (tempS1.size()>0)
      { if (tempS1[0]!='-')
        { output << "+";
          cutOffCounter+=1;
        }
      } else
      { output << "+";
        cutOffCounter+=1;
      }
    }
    output << tempS1;
    cutOffCounter+=tempS1.size();
  }
  return output;
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
std::string MonomialUniversalEnveloping<CoefficientType>::ToString(FormatExpressions* theFormat)const
{ std::stringstream out;
  std::string tempS;
  if (this->owneR==0)
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
      out << thePower.ToString(theFormat);
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
(int numVars, SemisimpleLieAlgebra& inputOwner)
{ this->Coefficient.MakeZero(numVars);
  this->owneR=&inputOwner;
  this->generatorsIndices.size=0;
  this->Powers.size=0;
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::MakeCasimir
(SemisimpleLieAlgebra& theOwner, GlobalVariables& theGlobalVariables,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ //std::stringstream out;
  this->MakeZero(theOwner);
  WeylGroup& theWeyl= this->GetOwner().theWeyl;
  int theDimension=theWeyl.CartanSymmetric.NumRows;
  Vector<Rational> tempRoot1, tempRoot2;
  Matrix<Rational> killingForm;
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
//  std::cout << killingForm.ToString(true, false);
//  killingForm.Invert(theGlobalVariables);
//  killingForm.ComputeDebugString();
//  out << killingForm.ToString(true, false);
//  std::cout << killingForm.ToString(true, false);


  ElementUniversalEnveloping<CoefficientType> tempElt1, tempElt2;
//this code is to check a math formula:
//  ElementUniversalEnveloping checkElement;
//  checkElement.MakeZero(theOwner);
  Matrix<Rational> invertedSymCartan;
  invertedSymCartan=theWeyl.CartanSymmetric;
  invertedSymCartan.Invert();
////////////////////////////////////////////////////////////////////////
  for (int i=0; i<theDimension; i++)
  { tempRoot1.MakeEi(theDimension, i);
  //implementation without the ninja formula:
//    killingForm.ActOnVectorColumn(tempRoot1, tempRoot2);
//    tempElt1.MakeHgenerator(tempRoot1, numVars, theOwner);
//    tempElt2.MakeHgenerator(tempRoot2, numVars, theOwner);
//    tempElt1*=tempElt2;
//    *this+=tempElt1;
// Alternative implementation using a ninja formula I cooked up after looking at the printouts:
    invertedSymCartan.ActOnVectorColumn(tempRoot1, tempRoot2);
    tempElt1.MakeHgenerator(tempRoot1, theOwner, theRingUnit, theRingZero);
    tempElt2.MakeHgenerator(tempRoot2, theOwner, theRingUnit, theRingZero);
    tempElt1*=tempElt2;
    *this+=tempElt1;
  }
  Rational tempRat;
  Vector<Rational> theSum;
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
    Vector<Rational>& theRoot=theWeyl.RootSystem.TheObjects[i];
    tempElt2.MakeOneGeneratorCoeffOne(-theRoot, theOwner, theRingUnit, theRingZero);
    tempElt1.MakeOneGeneratorCoeffOne(theRoot, theOwner, theRingUnit, theRingZero);
    tempElt2*=tempElt1;
    tempElt2*=theWeyl.RootScalarCartanRoot(theWeyl.RootSystem[i], theWeyl.RootSystem[i])/2;
    *this+=tempElt2;
  }
  *this/=theWeyl.GetKillingDivTraceRatio();
// Check that the ninja formula is correct:
//  FormatExpressions tempPolyFormat;
//  tempPolyFormat.MakeAlphabetArbitraryWithIndex("g", "h");
//  std::cout << "Killing divided by trace ratio:" << theWeyl.GetKillingDivTraceRatio().ToString();
//  std::cout << "<br>casimir: " << this->ToString(false, false, theGlobalVariables, tempPolyFormat);
//  std::cout << "<br>check element: " << checkElement.ToString(false, false, theGlobalVariables, tempPolyFormat);

//  std::cout << "<br> check element minus casimir=" << checkElement.ToString(false, false, theGlobalVariables, tempPolyFormat);
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
  this->Simplify(theGlobalVariables, theRingUnit, theRingZero);
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::SubstitutionCoefficients
(PolynomialSubstitution<Rational>& theSub, GlobalVariables* theContext, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ ElementUniversalEnveloping<CoefficientType> endResult;
  MonomialUniversalEnveloping<CoefficientType> currentMon;
  endResult.MakeZero(*this->owner);
  CoefficientType tempCF;
  for (int i=0; i<this->size; i++)
  { currentMon=this->TheObjects[i];
    this->theCoeffs[i].Substitution(theSub);
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
//  std::cout  << "this: " << this->ToString()
//  << " right: " << right.ToString() << " this*right: " << tempElt.ToString();
  tempElt2=right;
  tempElt2*=*this;
  output=tempElt;
  output-=tempElt2;
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::CleanUpZeroCoeff()
{ for (int i=0; i<this->size; i++)
    if (this->TheObjects[i].Coefficient.IsEqualToZero())
    { this->RemoveIndexSwapWithLast(i);
      i--;
    }
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::MakeOneGenerator
(int theIndex, SemisimpleLieAlgebra& inputOwner, const CoefficientType& theRingUnit)
{ this->MakeZero(inputOwner);
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  tempMon.init(inputOwner);
  tempMon.MultiplyByGeneratorPowerOnTheRight(theIndex, theRingUnit);
  this->AddMonomial(tempMon, theRingUnit);
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::MakeOneGeneratorCoeffOne
(int theIndex, SemisimpleLieAlgebra& inputOwner,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ this->MakeZero(inputOwner);
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  tempMon.MakeZero(theRingZero, inputOwner);
  tempMon.MultiplyByGeneratorPowerOnTheRight(theIndex, theRingUnit);
  this->AddMonomial(tempMon, theRingUnit);
}

template <class CoefficientType>
bool ElementUniversalEnveloping<CoefficientType>::ConvertToLieAlgebraElementIfPossible
(ElementSemisimpleLieAlgebra<Rational>& output)const
{ output.MakeZero(*this->owneR);
//  SemisimpleLieAlgebra& theOwner=this->owners->TheObjects[this->indexInOwners];
//  int numPosRoots=theOwner.theWeyl.RootsOfBorel.size;
  Rational theConst=0;
  CoefficientType theExponent;
  ChevalleyGenerator tempChevalley;
  for (int i=0; i<this->size; i++)
  { MonomialUniversalEnveloping<CoefficientType>& tempMon= this->TheObjects[i];
    tempMon.GetDegree(theExponent);
    if (!theExponent.IsEqualToOne())
      return false;
    Rational theConst;
    if (!this->theCoeffs[i].IsConstant(&theConst))
      return false;
    tempChevalley.MakeGenerator(*this->owneR, tempMon.generatorsIndices[0]);
    output.AddMonomial(tempChevalley, theConst);
  }
  return true;
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::AssignElementLieAlgebra
(const ElementSemisimpleLieAlgebra<Rational>& input, SemisimpleLieAlgebra& inputOwner,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ this->MakeZero(inputOwner);
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  tempMon.MakeZero(theRingZero, inputOwner);
  tempMon.generatorsIndices.SetSize(1);
  tempMon.Powers.SetSize(1);
  tempMon.Powers[0]=theRingUnit;
  CoefficientType tempCF;
  for (int i=0; i<input.size; i++)
  { tempCF=theRingUnit; //<- to facilitate implicit type conversion: theRingUnit does not have to be of type Rational
    tempCF*=input.theCoeffs[i];//<- to facilitate implicit type conversion: theRingUnit does not have to be of type Rational
    tempMon.generatorsIndices[0]=input[i].theGeneratorIndex;
    this->AddMonomial(tempMon, tempCF);
  }
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::GetCoordinateFormOfSpanOfElements
(List<ElementUniversalEnveloping<CoefficientType> >& theElements,
 Vectors<CoefficientType>& outputCoordinates,
 ElementUniversalEnveloping<CoefficientType>& outputCorrespondingMonomials,
 GlobalVariables& theGlobalVariables)
{ if (theElements.size==0)
    return;
  outputCorrespondingMonomials.MakeZero(*theElements[0].owners, theElements[0].indexInOwners);
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  for (int i=0; i<theElements.size; i++)
    for (int j=0; j<theElements[i].size; j++)
      outputCorrespondingMonomials.AddOnTopNoRepetition(theElements[i][j]);
  outputCoordinates.SetSize(theElements.size);
  Polynomial<Rational>  ZeroPoly;
  ZeroPoly.MakeZero();
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
void ElementUniversalEnveloping<CoefficientType>::MakeHgenerator
(const Vector<Rational>& input, SemisimpleLieAlgebra& inputOwner,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ MonomialUniversalEnveloping<CoefficientType> tempMon;
  this->MakeZero(inputOwner);
  tempMon.MakeZero(theRingZero, inputOwner);
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
{ output.MakeZero(*this->owner);
  output.AddOnTop(*this);
  this->SimplifyAccumulateInOutputNoOutputInit(output, theGlobalVariables, theRingUnit, theRingZero);
}

template <class CoefficientType>
bool ElementUniversalEnvelopingOrdered<CoefficientType>::GetElementUniversalEnveloping
  (ElementUniversalEnveloping<CoefficientType>& output, SemisimpleLieAlgebra& inputOwner)
{ ElementUniversalEnveloping<CoefficientType> Accum, tempElt;
  Accum.MakeZero(inputOwner);
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
  (ElementUniversalEnveloping<CoefficientType>& output, SemisimpleLieAlgebra& inputOwner)
{ ElementUniversalEnveloping<CoefficientType> Accum;
  ElementUniversalEnveloping<CoefficientType> tempMon;
  int theIndex;
  int theDegree;
  Accum.MakeConst(this->Coefficient, inputOwner);
  for (int i=0; i<this->generatorsIndices.size; i++)
    if (this->Powers[i].IsSmallInteger(&theDegree))
    { tempMon.AssignElementLieAlgebra
      (this->owner->theOrder[this->generatorsIndices[i]], inputOwner,
        this->Coefficient.GetOne(), this->Coefficient.GetZero());
      tempMon.RaiseToPower(theDegree);
      Accum*=(tempMon);
    } else
      if (this->owner->theOrder[this->generatorsIndices[i]].IsACoeffOneChevalleyGenerator(theIndex, owner))
      { tempMon.MakeOneGeneratorCoeffOne
        (theIndex, inputOwner, this->Coefficient.GetOne(), this->Coefficient.GetZero());
        tempMon[0].Powers[0]=this->Powers[i];
        Accum*=(tempMon);
      } else
        return false;
  output.operator=(Accum);
  return true;
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
      outputCorrespondingMonomials.AddOnTopNoRepetition(theElements.TheObjects[i].TheObjects[j]);
  outputCoordinates.SetSize(theElements.size);
  Polynomial<Rational>  ZeroPoly;
  ZeroPoly.MakeZero();
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
  this->Coefficient.Substitution(theSub);
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
(const Rational& coeff, int numVars, SemisimpleLieAlgebra& inputOwner)
{ MonomialUniversalEnveloping<CoefficientType> tempMon;
  this->MakeZero(inputOwner);
  if (coeff.IsEqualToZero())
    return;
  CoefficientType tempP;
  tempP= coeff;
  tempMon.MakeConst(inputOwner);
  this->AddMonomial(tempMon, tempP);
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::MakeZero(SemisimpleLieAlgebra& inputOwner)
{ this->::MonomialCollection<MonomialUniversalEnveloping<CoefficientType>, CoefficientType >::MakeZero();
  this->owneR=&inputOwner;
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::MultiplyBy
  (const MonomialUniversalEnveloping<CoefficientType>& standsOnTheRight, const CoefficientType& theCoeff)
{ if (standsOnTheRight.generatorsIndices.size==0)
    return;
  ElementUniversalEnveloping<CoefficientType> output;
  output.SetExpectedSize(this->size);
  output.MakeZero(*this->owner);
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
void ElementUniversalEnveloping<CoefficientType>::operator*=
(const ElementUniversalEnveloping& standsOnTheRight)
{ this->::ElementMonomialAlgebra<MonomialUniversalEnveloping<CoefficientType>, CoefficientType>
  ::operator*=
  ( (ElementMonomialAlgebra<MonomialUniversalEnveloping<CoefficientType>, CoefficientType>)
   standsOnTheRight);
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::RaiseToPower(int thePower)
{ ElementUniversalEnveloping<CoefficientType> buffer;
  buffer=*this;
  if (this->size==0)
    return;
  this->MakeConst(this->theCoeffs[0].GetOne(), *this->owneR);
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
unsigned int MonomialUniversalEnvelopingOrdered<CoefficientType>::HashFunction() const
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
  Polynomial<Rational> tempP;
  tempP.MakeConst(thePower);
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
std::string MonomialUniversalEnvelopingOrdered<CoefficientType>::ToString
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
    tempS= this->Coefficient.ToString(PolyFormatLocal);
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
    tempS=thePower.ToString(PolyFormatLocal);
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
void ElementUniversalEnvelopingOrdered<CoefficientType>::ToString
(std::string& output, bool useLatex,
 bool useCalculatorFormat, FormatExpressions& PolyFormatLocal, GlobalVariables& theGlobalVariables)const
{ std::stringstream out;
  std::string tempS;
  if (this->size==0)
    out << "0";
  int IndexCharAtLastLineBreak=0;
  for (int i=0; i<this->size; i++)
  { MonomialUniversalEnvelopingOrdered<CoefficientType>& current=this->TheObjects[i];
    tempS=current.ToString(false, useCalculatorFormat, &PolyFormatLocal, theGlobalVariables);
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
  ElementSemisimpleLieAlgebra<Rational> tempElt;
  CoefficientType theCoeff;
  theCoeff=inputCoeff;
  this->MakeConst(theCoeff, owner);
  //std::cout << "<br>after initialization with constant I am " << this->ToString();
  for (int i=0; i<input.generatorsIndices.size; i++)
  { int thePower;
    bool isASmallInt=input.Powers.TheObjects[i].IsSmallInteger(&thePower);
    if (isASmallInt)
    { tempElt.MakeGenerator
      (input.generatorsIndices.TheObjects[i], *input.owners, input.indexInOwners);
      theMon.AssignElementLieAlgebra(tempElt, theRingUnit, theRingZero, owner);
      //std::cout << "<br>raising " << theMon.ToString() << " to power " << thePower;
      theMon.RaiseToPower(thePower, theRingUnit);
      //std::cout << " to obtain " << theMon.ToString();
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
(const ElementSemisimpleLieAlgebra<Rational>& input, const CoefficientType& theRingUnit,
 const CoefficientType& theRingZero, SemisimpleLieAlgebraOrdered& theOwner)
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
  //std::cout << "<br>raising " <<buffer.ToString() << " to power " << thePower;
  for (int i=0; i<thePower; i++)
    this->operator*=(buffer);
  //std::cout << "<br> and the result is " << this->ToString();
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::LieBracketOnTheRight
(const ElementSemisimpleLieAlgebra<Rational>& right, const CoefficientType& ringUnit,
 const CoefficientType& ringZero)
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
      this->RemoveIndexSwapWithLast(theIndex);
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
  { this->TheObjects[i].MakeZero();
    for (int j=0; j<theMat.NumRows-1; j++)
    { tempM.MakeOne(theMat.NumRows-1);
      tempM[j]=1;
      this->TheObjects[i].AddMonomial(tempM, theMat.elements[j][i]);
    }
    this->TheObjects[i]+=(theMat.elements[theMat.NumRows-1][i]);
  }
}

template <class CoefficientType>
void ElementTensorsGeneralizedVermas<CoefficientType>::Substitution
  (const PolynomialSubstitution<Rational>& theSub, ListReferences<ModuleSSalgebra<CoefficientType> >& theMods)
{ ElementTensorsGeneralizedVermas<CoefficientType> output;
  MonomialTensorGeneralizedVermas<CoefficientType> currentMon;
  output.MakeZero();
  CoefficientType tempCF;
  for (int i=0; i<this->size; i++)
  { currentMon=this->TheObjects[i];
//    std::cout << "<br>currentMon before sub: " << currentMon.ToString();
    currentMon.Substitution(theSub, theMods);
//    std::cout << "<br>currentMon after sub: " << currentMon.ToString();
    tempCF=this->theCoeffs[i];
//    std::cout << "<br>cf before sub: " << tempCF.ToString();
    tempCF.Substitution(theSub);
//    std::cout << "<br>cf after sub: " << tempCF.ToString();
    output.AddMonomial(currentMon, tempCF);
  }
  *this=output;
//  std::cout << "<hr>result: " << this->ToString(theGlobalVariables);
}

template <class CoefficientType>
void MonomialGeneralizedVerma<CoefficientType>::Substitution
(const PolynomialSubstitution<Rational>& theSub, ListReferences<ModuleSSalgebra<CoefficientType> >& theMods)
{ //std::cout << "<br>ze ue mon before sub: " << this->theMonCoeffOne.ToString();
  this->theMonCoeffOne.Substitution(theSub);
  //std::cout << "<br>ze ue mon after sub: " << this->theMonCoeffOne.ToString();
  ModuleSSalgebra<CoefficientType> newOwner;
  newOwner=*this->owneR;
  newOwner.Substitution(theSub);
  //std::cout << "<br>old index in owner: " << this->indexInOwner;
  int newModIndex=theMods.AddNoRepetitionOrReturnIndexFirst(newOwner);
  this->owneR=&theMods[newModIndex];
  //std::cout << "<br>new index in owner: " << this->indexInOwner;
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::Substitution
(const PolynomialSubstitution<Rational>& theSub)
{ ElementUniversalEnveloping<CoefficientType> output;
  output.MakeZero(*this->owneR);
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
bool ElementTensorsGeneralizedVermas<CoefficientType>::MultiplyOnTheLeft
(const ElementUniversalEnveloping<CoefficientType>& theUE,
 ElementTensorsGeneralizedVermas<CoefficientType>& output,
 SemisimpleLieAlgebra& ownerAlgebra,
 GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)const
{ ElementTensorsGeneralizedVermas<CoefficientType> buffer;
  output.MakeZero();
  for (int i=0; i<theUE.size; i++)
  { if (!this->MultiplyOnTheLeft(theUE[i], buffer, ownerAlgebra, theGlobalVariables, theRingUnit, theRingZero))
    { ElementSumGeneralizedVermas<CoefficientType> tempOutput;
      std::cout << "<hr>emergency mode!";
      for (int j=0; j<this->size; j++)
      { MonomialTensorGeneralizedVermas<CoefficientType> currentMon=this->TheObjects[j];
        if (currentMon.theMons.size!=1)
          return false;
        MonomialGeneralizedVerma<CoefficientType>& currentSingleMon=currentMon.theMons[0];
        if (j==0)
          tempOutput.MakeZero();
        tempOutput.AddMonomial(currentSingleMon, this->theCoeffs[j]);
      }
      tempOutput.MultiplyMeByUEEltOnTheLeft(theUE, theGlobalVariables, theRingUnit, theRingZero);
      output=tempOutput;
      return true;
    }
//    int commentmewhendone;
//    theUE.theCoeffs[i].checkConsistency();
//    buffer.GrandMasterConsistencyCheck();
//    buffer.checkConsistency();
    for (int k=0; k<buffer.theCoeffs.size; k++)
    { std::string debugString=buffer.theCoeffs[k].ToString();
//      buffer.theCoeffs[k].checkConsistency();
    }
    buffer*=theUE.theCoeffs[i];
    std::cout << "<br>and your beloved buffer, after being multiplied by " << theUE.theCoeffs[i].ToString() << " equals " << buffer.ToString();
    output+=buffer;
  }
  return true;
  //for (int i=0; i<theOwner.size; i++)
  //{ //std::cout << "<hr><hr>Module" << i+1 << "<br>" << theOwner[i].ToString();
  //}
}

template <class CoefficientType>
void ElementTensorsGeneralizedVermas<CoefficientType>::MakeHWV
(ModuleSSalgebra<CoefficientType>& theOwner, const CoefficientType& theRingUnit)
{ MonomialTensorGeneralizedVermas<CoefficientType> tensorMon;
  CoefficientType currentCoeff;
  currentCoeff=theRingUnit;
  tensorMon.theMons.SetSize(1);
  MonomialGeneralizedVerma<CoefficientType>& theMon=tensorMon.theMons[0];
  theMon.indexFDVector=theOwner.theGeneratingWordsNonReduced.size-1;
  theMon.MakeConst(theOwner);
  this->MakeZero();
  this->AddMonomial(tensorMon, theRingUnit);
}

template <class CoefficientType>
bool ElementTensorsGeneralizedVermas<CoefficientType>::MultiplyOnTheLeft
(const MonomialUniversalEnveloping<CoefficientType>& theUE,
 ElementTensorsGeneralizedVermas<CoefficientType>& output,
 SemisimpleLieAlgebra& ownerAlgebra,
 GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)const
{ assert(&output!=this);
//  int commentmewhendone;
//  static int problemCounter=0;
//  problemCounter++;
//  output.checkConsistency();
//  std::string debugString;
//  if (problemCounter==44)
//  { debugString=this->ToString();
//  }
//  this->checkConsistency();
////
  output=*this;
////
//  output.checkConsistency();
  ElementTensorsGeneralizedVermas<CoefficientType> buffer;
  for(int i=theUE.Powers.size-1; i>=0; i--)
  { int thePower;
    if (!theUE.Powers[i].IsSmallInteger(&thePower))
      return false;
    int theIndex=theUE.generatorsIndices[i];
    for (int j=0; j<thePower; j++)
    { //std::cout <<"<hr>Acting by generator index " << theIndex << " on " << output.ToString();
      output.MultiplyByElementLieAlg(buffer, ownerAlgebra, theIndex, theGlobalVariables, theRingUnit, theRingZero);
//      buffer.checkConsistency();
      output=buffer;
//  int commentmewhendone4;
//  output.checkConsistency();
      //std::cout << "<br>to get: " << output.ToString();
    }
  }
//  int commentmewhendone2;
//  output.checkConsistency();
  return true;
}

template <class CoefficientType>
void ElementTensorsGeneralizedVermas<CoefficientType>::MultiplyByElementLieAlg
(ElementTensorsGeneralizedVermas<CoefficientType>& output,
 SemisimpleLieAlgebra& ownerAlgebra,
 int indexGenerator, GlobalVariables& theGlobalVariables,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)const
{ output.MakeZero();
  MonomialTensorGeneralizedVermas<CoefficientType> accumMon, monActedOn;
  ElementSumGeneralizedVermas<CoefficientType> tempElt;
  ElementUniversalEnveloping<CoefficientType> theGenerator;

  theGenerator.MakeOneGenerator
  (indexGenerator, ownerAlgebra, theRingUnit);
  ;
//  FormatExpressions tempFormat;
//  tempFormat.MakeAlphabetArbitraryWithIndex("g", "h");
  CoefficientType currentCoeff;
  for (int i=0; i<this->size; i++)
  { MonomialTensorGeneralizedVermas<CoefficientType>& currentMon=this->TheObjects[i];
    accumMon.theMons.SetSize(0);
    for (int j=0; j<currentMon.theMons.size; j++)
    { tempElt.MakeZero();
      tempElt.AddMonomial(currentMon.theMons[j], theRingUnit);
//      std::cout << "<hr> Acting by " << theGenerator.ToString() << " on " << tempElt.ToString() << "<br>";
      tempElt.MultiplyMeByUEEltOnTheLeft(theGenerator, theGlobalVariables, theRingUnit, theRingZero);
//      if (tempElt.size>0)
//        std::cout << "<br> result: " << tempElt.ToString() << ", the first coeff of tempElt is : " << tempElt.theCoeffs[0].ToString();
      for (int k=0; k<tempElt.size; k++)
      { currentCoeff=this->theCoeffs[i];
//        std::cout << "<br>coeff: " << currentCoeff.ToString() << " times " << tempElt.theCoeffs[k].ToString() << " equals ";
        currentCoeff*=tempElt.theCoeffs[k];
//        std::cout << currentCoeff.ToString();
        monActedOn=accumMon;
        monActedOn*=(tempElt[k]);
        for (int l=j+1; l<currentMon.theMons.size; l++)
          monActedOn*=currentMon.theMons[l];
//        std::cout << "<br>Adding: " << currentCoeff.ToString() << " times " << monActedOn.ToString() << " to " << output.ToString();
        output.AddMonomial(monActedOn, currentCoeff);
//        std::cout << "<br>to get " << output.ToString();
      }
      accumMon*=currentMon.theMons[j];
    }
  }
}

template <class CoefficientType>
void MonomialGeneralizedVerma<CoefficientType>::MultiplyMeByUEEltOnTheLefT
(const ElementUniversalEnveloping<CoefficientType>& theUE, ElementSumGeneralizedVermas<CoefficientType>& output,
GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ MacroRegisterFunctionWithName("MonomialGeneralizedVerma<CoefficientType>::MultiplyMeByUEEltOnTheLefT");
  MonomialGeneralizedVerma<CoefficientType> currentMon;
  output.MakeZero();
  ElementSumGeneralizedVermas<CoefficientType> buffer;
  ProgressReport theReport(&theGlobalVariables);
  for (int j=0; j<theUE.size; j++)
  { currentMon.theMonCoeffOne=theUE[j];
    std::cout << "<br>currentMon: " << currentMon.theMonCoeffOne.ToString();
    currentMon.theMonCoeffOne*=this->theMonCoeffOne;
    std::cout << "<br>currentMon after multi: " << currentMon.theMonCoeffOne.ToString();
    currentMon.owneR=this->owneR;
    currentMon.indexFDVector=this->indexFDVector;
    currentMon.owneR=this->owneR;
    std::cout << "<hr>Applying " <<theUE.theCoeffs[j].ToString()
    << " times " << theUE[j].ToString() << " on " << this->ToString();
    std::stringstream reportStream;
    reportStream << "reducing mon: " << currentMon.ToString() << ", index" << j+1 << " out of " << theUE.size << "...";
    std::cout << "reducing mon: " << currentMon.ToString() << ", index" << j+1 << " out of " << theUE.size << "...";
    theReport.Report(reportStream.str());
    currentMon.ReduceMe(buffer, theGlobalVariables, theRingUnit, theRingZero);
    reportStream << " done.";
    std::cout << " done.";
    theReport.Report(reportStream.str());
    std::cout << "<br>buffer: " << buffer.ToString() << " multiplied by " << theUE.theCoeffs[j].ToString();
    buffer*=theUE.theCoeffs[j];
    output+=buffer;
    std::cout << " equals: " << buffer.ToString();
  }
  std::cout << "<br>result: " << this->ToString();
}

template <class CoefficientType>
void ElementSumGeneralizedVermas<CoefficientType>::MultiplyMeByUEEltOnTheLeft
  (const ElementUniversalEnveloping<CoefficientType>& theUE, GlobalVariables& theGlobalVariables,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ MacroRegisterFunctionWithName("ElementSumGeneralizedVermas<CoefficientType>::MultiplyMeByUEEltOnTheLeft");
  ElementSumGeneralizedVermas<CoefficientType> buffer, Accum;
//std::cout << "<br>Multiplying " << this->ToString() << " by " << theUE.ToString();
  Accum.MakeZero();
  for (int i=0; i<this->size; i++)
  {// std::cout << "<br>Multiplying " << this->TheObjects[i].ToString() << " by " << theUE.ToString() << " by " << this->theCoeffs[i].ToString();
    this->TheObjects[i].MultiplyMeByUEEltOnTheLefT(theUE, buffer, theGlobalVariables, theRingUnit, theRingZero);
    //std::cout << "<br>buffer " << buffer.ToString() << " multiplied by coeff " << this->theCoeffs[i].ToString();
    buffer*=this->theCoeffs[i];
//    std::cout << "<br>to obtain " << buffer.ToString();
    Accum+=buffer;
//    std::cout << " <br> to accummulate to " << Accum.ToString();
  }
  *this=Accum;
//  std::cout << "<br>To get in the damned end: " << this->ToString();
}

template <class CoefficientType>
void MonomialGeneralizedVerma<CoefficientType>::ReduceMe
(ElementSumGeneralizedVermas<CoefficientType>& output, GlobalVariables& theGlobalVariables,
  const CoefficientType& theRingUnit, const CoefficientType& theRingZero)const
{ MacroRegisterFunctionWithName("MonomialGeneralizedVerma::ReduceMe");
  //std::cout << "<hr><hr>Reducing  " << this->ToString();
  ModuleSSalgebra<CoefficientType>& theMod=*this->owneR;
  output.MakeZero();
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  tempMon=this->theMonCoeffOne;
  tempMon*=theMod.theGeneratingWordsNonReduced[this->indexFDVector];
  int indexCheck=theMod.theGeneratingWordsNonReduced.GetIndex(tempMon);
  if (indexCheck!=-1)
  { MonomialGeneralizedVerma<CoefficientType> basisMon;
    basisMon.MakeConst(*this->owneR);
    basisMon.indexFDVector=indexCheck;
    output.AddMonomial(basisMon, theRingUnit);
//    std::cout << "<br>Reduced " << this->ToString() << " to " << output.ToString() << " = " << basisMon.ToString();
//    std::cout << "<br> index check is " << indexCheck << " corresponding to " << theMod.theGeneratingWordsNonReduced[indexCheck].ToString();
//    theGlobalVariables.MakeProgressReport("Monomial basis of fd part. ", 2);
    return;
  }
//  std::cout << "<br>Not a monomial basis of fd part";
//  theGlobalVariables.MakeProgressReport("Monomial not basis of fd part. ", 2);
  theMod.GetOwner().OrderSetNilradicalNegativeMost(theMod.parabolicSelectionNonSelectedAreElementsLevi);
//  std::cout << "<br>";
//  for (int i=0; i<theMod.GetOwner().UEGeneratorOrderIncludingCartanElts.size; i++)
//  { std::cout << "<br>generator index " << i << " has order " << theMod.GetOwner().UEGeneratorOrderIncludingCartanElts[i];
//  }
  ElementUniversalEnveloping<CoefficientType> theUEelt;
  theUEelt.MakeZero(*this->GetOwner().owneR);
  theUEelt.AddMonomial(this->theMonCoeffOne, theRingUnit);
//  std::cout << " <br>the monomial:" << this->ToString();
  theUEelt.Simplify(theGlobalVariables, theRingUnit, theRingZero);
//  std::cout << " <br>the corresponding ue with F.D. part cut off: " << theUEelt.ToString();

  MonomialUniversalEnveloping<CoefficientType> currentMon;
  MonomialGeneralizedVerma<CoefficientType> newMon;
  MatrixTensor<CoefficientType> tempMat1, tempMat2;
  //std::cout << theMod.ToString();
  //std::cout << "<br>theMod.theModuleWeightsSimpleCoords.size: "
  //<< theMod.theModuleWeightsSimpleCoords.size;
  ProgressReport theReport(&theGlobalVariables);
  CoefficientType theCF;
  for (int l=0; l<theUEelt.size; l++)
  { currentMon=theUEelt[l];
    //std::cout << "<br> Processing monomial " << currentMon.ToString();
    tempMat1.MakeIdSpecial();
    for (int k=currentMon.Powers.size-1; k>=0; k--)
    { std::stringstream reportStream;
      reportStream << "accounting monomial " << currentMon.ToString() << " of index " << l+1 << " out of "
      << theUEelt.size << " and letter index " << currentMon.Powers.size-k << " out of " << currentMon.Powers.size << "...";
      theReport.Report(reportStream.str());
      int thePower;
      if (!currentMon.Powers[k].IsSmallInteger(&thePower))
        break;
      int theIndex=currentMon.generatorsIndices[k];
      if (theMod.HasFreeAction(theIndex))
        break;
      tempMat2=tempMat1;
      tempMat1=theMod.GetActionGeneratorIndeX
      (theIndex, theGlobalVariables, theRingUnit, theRingZero);
      tempMat1.RaiseToPower(thePower);
      tempMat1*=tempMat2;
      currentMon.Powers.size--;
      currentMon.generatorsIndices.size--;
      reportStream << "done!";
      theReport.Report(reportStream.str());
    }
//    std::cout << "<br> Action is the " << currentMon.ToString() << " free action plus <br>" << tempMat1.ToString();
    newMon.owneR=this->owneR;
    for (int i=0; i<tempMat1.size; i++)
    { int otherIndex=-1;
      if (tempMat1[i].dualIndex==this->indexFDVector)
        otherIndex=tempMat1[i].vIndex;
      if (tempMat1[i].IsId)
        otherIndex=this->indexFDVector;
      if (otherIndex!=-1)
      { newMon.theMonCoeffOne=currentMon;
        newMon.indexFDVector=otherIndex;
        theCF=theUEelt.theCoeffs[l];
        theCF*=tempMat1.theCoeffs[i];
        //std::cout << "<br>adding to " << output.ToString()
        //<< " the monomial " << newMon.ToString() << " with coefficient "
        //<< theCF.ToString() << " to obtain ";
        output.AddMonomial(newMon, theCF);
        //std::cout << output.ToString();
      }
    }
  }
//  std::cout << "<br>Matrix of the action: " << tempMat1.ToString();
//  std::cout << "<br> Final output: " << output.ToString();
  theMod.GetOwner().OrderSSLieAlgebraStandard();
}

template <class CoefficientType>
MatrixTensor<CoefficientType>& ModuleSSalgebra<CoefficientType>::GetActionGeneratorIndeX
(int generatorIndex, GlobalVariables& theGlobalVariables,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ MacroRegisterFunctionWithName("ModuleSSalgebra<CoefficientType>::GetActionGeneratorIndeX");
  int numGenerators=this->GetOwner().GetNumGenerators();
  assert(generatorIndex>=0 && generatorIndex<numGenerators);
  if (this->ComputedGeneratorActions.selected[generatorIndex])
  { //std::cout << "<br>generator index " << generatorIndex << " is precomputed: "
    //<< this->actionsGeneratorsMaT[generatorIndex].ToString(true, false);
    return this->actionsGeneratorsMaT[generatorIndex];
  }
  this->ComputedGeneratorActions.AddSelectionAppendNewIndex(generatorIndex);
  if (this->HasFreeAction(generatorIndex))
  { std::cout << "This is a programming error, due to a change in implementation of "
    << " the generalized Verma module class. " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
    this->actionsGeneratorsMaT[generatorIndex].MakeZero();
    //std::cout << "<br>generator index " << generatorIndex << " has free action. ";
    return this->actionsGeneratorsMaT[generatorIndex];
  }
  if (this->HasZeroActionFDpart(generatorIndex))
  { this->actionsGeneratorsMaT[generatorIndex].MakeZero();
    //std::cout << "<br>generator index " << generatorIndex << " has ZERO action. ";
    return this->actionsGeneratorsMaT[generatorIndex];
  }
  if (this->GetOwner().IsASimpleGenerator(generatorIndex))
    return this->GetActionSimpleGeneratorIndex(generatorIndex, theGlobalVariables, theRingUnit, theRingZero);
  MatrixTensor<CoefficientType>& output=this->actionsGeneratorsMaT[generatorIndex];
  if (this->GetOwner().IsGeneratorFromCartan(generatorIndex))
  { output.MakeZero();
    MonomialMatrix theMon;
    Vector<CoefficientType> weightH;
    CoefficientType tempCF, hwCFshift;
    weightH.MakeEi(this->GetOwner().GetRank(), generatorIndex-this->GetOwner().GetNumPosRoots());
    hwCFshift=this->GetOwner().theWeyl.RootScalarCartanRoot(weightH, this->theHWSimpleCoordSBaseField);
    hwCFshift-=this->GetOwner().theWeyl.RootScalarCartanRoot(weightH, this->theHWFDpartSimpleCoordS);
//    std::cout << "<br>the h: " << weightH.ToString() << "<br> the hw: " << this->theHWSimpleCoordSBaseField.ToString()
//    << "<br>hwCfshift: " << hwCFshift.ToString() << "  <br>the generator equals: ";
    for (int i=0; i<this->theGeneratingWordsNonReduced.size; i++)
    { Vector<Rational>& theWeight=this->theGeneratingWordsWeightsPlusWeightFDpart[i];
      tempCF=this->GetOwner().theWeyl.RootScalarCartanRoot(weightH, theWeight);
      tempCF+=hwCFshift;
      theMon.IsId=false;
      theMon.vIndex=i;
      theMon.dualIndex=i;
      output.AddMonomial(theMon, tempCF);
//      std::cout << " + " << tempCF.ToString() << "*" << theMon.ToString();
    }
    return output;
  }
  List<int> adActions;
  Rational theCoeff;
  this->GetOwner().GetChevalleyGeneratorAsLieBracketsSimpleGens(generatorIndex, adActions, theCoeff);
  MatrixTensor<CoefficientType> tempO;
  output=this->GetActionGeneratorIndeX(*adActions.LastObject(), theGlobalVariables);
  for (int i=adActions.size-2; i>=0; i--)
  { tempO=this->GetActionGeneratorIndeX(adActions[i], theGlobalVariables);
    output.LieBracketOnTheLeft(tempO);
  }
  CoefficientType tempCF;
  tempCF=theCoeff;
  output*=tempCF;
  return output;
}

template  <class CoefficientType>
void ModuleSSalgebra<CoefficientType>::
GetMatrixHomogenousElt
(ElementUniversalEnveloping<CoefficientType>& inputHomogeneous,
  List<List<ElementUniversalEnveloping<CoefficientType> > >& outputSortedByArgumentWeight,
  Vector<Rational>& weightUEEltSimpleCoords, MatrixTensor<CoefficientType>& output,
  GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{// std::cout << "<hr>status of the module @ start GetMatrixHomogenousElt" << this->ToString();
  this->GetAdActionHomogenousElT
  (inputHomogeneous, weightUEEltSimpleCoords, outputSortedByArgumentWeight, theGlobalVariables, theRingUnit, theRingZero)
  ;
  output.MakeZero();
  for (int j=0; j<outputSortedByArgumentWeight.size; j++)
    for (int k=0; k<outputSortedByArgumentWeight[j].size; k++)
    { MonomialUniversalEnveloping<CoefficientType>& currentMon=this->theGeneratingWordsGrouppedByWeight[j][k];
      ElementUniversalEnveloping<CoefficientType>& imageCurrentMon=outputSortedByArgumentWeight[j][k];
      int indexColumn=this->theGeneratingWordsNonReduced.GetIndex(currentMon);
      assert(indexColumn!=-1);
      for (int l=0; l< imageCurrentMon.size; l++)
      { int indexRow=this->theGeneratingWordsNonReduced.GetIndex(imageCurrentMon[l]);
        assert(indexRow!=-1);
        output.AddMonomial(MonomialMatrix(indexRow, indexColumn), imageCurrentMon.theCoeffs[l]);
//        std::cout <<"<br> Index row: " << indexRow << "; index column: " << indexColumn;
      }
    }
  if (!this->flagIsInitialized)
    return;
//  std::cout << "<hr><hr><hr><hr><hr>input GetMatrixHomogenousElt: " << inputHomogeneous.ToString();
//  std::cout << "<hr>status of the module @ GetMatrixHomogenousElt" << this->ToString();
//  std::cout << "<hr>output GetMatrixHomogenousElt: " << output.ToString(true, false);
}

template<class CoefficientType>
CoefficientType WeylGroup::WeylDimFormulaSimpleCoords(Vector<CoefficientType>& theWeightInSimpleCoords, const CoefficientType& theRingUnit)
{ CoefficientType Result, buffer;
  Vector<CoefficientType> rhoOverNewRing, rootOfBorelNewRing, sumWithRho;//<-to facilitate type conversion!
  rhoOverNewRing=this->rho;//<-type conversion here!
  Result=theRingUnit;
//  std::cout << "<br>doing the weyl dim formula with: " << theWeightInSimpleCoords.ToString();
//  std::cout << "<br>rho is:" << rhoOverNewRing.ToString();
//  std::cout << "<br>rho before conversion: " << this->rho.ToString();
//  std::cout << "<br>we get: ";
  for (int i=0; i<this->RootsOfBorel.size; i++)
  { rootOfBorelNewRing=this->RootsOfBorel[i]; //<-type conversion here!
    sumWithRho=rhoOverNewRing+theWeightInSimpleCoords;
    buffer=(this->RootScalarCartanRoot(sumWithRho, rootOfBorelNewRing));
    buffer/=this->RootScalarCartanRoot(rhoOverNewRing, rootOfBorelNewRing);
//    std::cout << "(" << buffer.ToString() << ")";
    Result*=buffer;
  }
  return Result;
}

template<class CoefficientType>
CoefficientType ReflectionSubgroupWeylGroup::WeylDimFormulaSimpleCoords(const Vector<CoefficientType>& theWeightInSimpleCoords, const CoefficientType& theRingUnit)
{ CoefficientType Result, buffer;
  Vector<CoefficientType> rhoOverNewRing, rootOfBorelNewRing, sumWithRho;//<-to facilitate type conversion!
  Vector<Rational> rho;
  this->RootsOfBorel.sum(rho, this->AmbientWeyl.GetDim());
  rho/=2;
  rhoOverNewRing=rho;//<-type conversion here!
  Result=theRingUnit;
//  std::cout << "<br>doing the weyl dim formula with: " << theWeightInSimpleCoords.ToString();
//  std::cout << "<br>rho is:" << rhoOverNewRing.ToString();
//  std::cout << "<br>we get: ";
  for (int i=0; i<this->RootsOfBorel.size; i++)
  { rootOfBorelNewRing=this->RootsOfBorel[i]; //<-type conversion here!
    sumWithRho=rhoOverNewRing+theWeightInSimpleCoords;
    buffer=(this->AmbientWeyl.RootScalarCartanRoot(sumWithRho, rootOfBorelNewRing));
    buffer/=this->AmbientWeyl.RootScalarCartanRoot(rhoOverNewRing, rootOfBorelNewRing);
//    std::cout << "(" << buffer.ToString() << ")";
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
//  std::cout << "<br>transition matrix from fundamental to simple: " << tempMat.ToString();
  tempMat.ActOnVectorColumn(result);
  return result;
}

template<class CoefficientType>
Vectors<CoefficientType> WeylGroup::GetSimpleCoordinatesFromFundamental
(const Vectors<CoefficientType>& inputInFundamentalCoords)
{ Matrix<Rational>& tempMat=*this->GetMatrixFundamentalToSimpleCoords();
  Vectors<CoefficientType> result;
  result=inputInFundamentalCoords;
//  std::cout << "<br>transition matrix from fundamental to simple: " << tempMat.ToString();
  tempMat.ActOnVectorsColumn(result);
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
{ if (r1.size!=r2.size || r1.size!=this->GetDim())
  { std::cout << "This is a programming error: attempting to get the scalar products of two weights "
    << " that are not of the same dimension as the rank of the Weyl group. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  output=r1[0].GetZero();
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
(const List<Vector<CoefficientType> >& firstSpace, const List<Vector<CoefficientType> >& secondSpace,
 List<Vector<CoefficientType> >& output, GlobalVariables* theGlobalVariables)
{ //std::cout << "<br>*****Debugging Intersection linear spaces: ";
  //std::cout << "<br>input first space: " << firstSpace.ToString();
  //std::cout << "<br>input second space: " << secondSpace.ToString();
  Vectors<CoefficientType> firstReduced, secondReduced;
  Selection tempSel;
  Vectors<CoefficientType>::SelectABasisInSubspace
  (firstSpace, firstReduced, tempSel, theGlobalVariables);
  Vectors<CoefficientType>::SelectABasisInSubspace
  (secondSpace, secondReduced, tempSel, theGlobalVariables);
//  std::cout << "<br>first selected basis: " << firstReduced.ToString();
//  std::cout << "<br>second selected basis: " << secondReduced.ToString();
  if (firstReduced.size==0 || secondReduced.size==0)
  { output.size=0;
    return;
  }
  int theDim=firstReduced[0].size;
  Matrix<CoefficientType> theMat;
  theMat.init(theDim, firstReduced.size+secondReduced.size);
  for (int i=0; i<theDim; i++)
  { for (int j=0; j<firstReduced.size; j++)
      theMat(i,j)=firstReduced[j][i];
    for (int j=0; j<secondReduced.size; j++)
    { theMat(i,firstReduced.size+j)=0;
      theMat(i,firstReduced.size+j)-=secondReduced[j][i];
    }
  }
//  std::cout << "<br>The matrix before the gaussian elimination:" << theMat.ToString();
  theMat.GaussianEliminationByRows(0, &tempSel);
//  std::cout << "<br>The matrix after the gaussian elimination:" << theMat.ToString();
  output.ReservE(tempSel.CardinalitySelection);
  output.size=0;
  Vector<CoefficientType> nextIntersection;
  for(int i=0; i<tempSel.CardinalitySelection; i++)
  { int currentIndex=tempSel.elements[i];
//    std::cout << "<br>current pivot index : " << currentIndex;
    assert(currentIndex>=firstReduced.size);
    nextIntersection.MakeZero(theDim);
    for (int j=0; j<firstReduced.size; j++)
      if (!tempSel.selected[j])
        nextIntersection+=firstReduced[j]*theMat.elements[j][currentIndex];
    output.AddOnTop(nextIntersection);
  }
//  std::cout << "<br> final output: " << output.ToString();
//  std::cout << "<br>******************End of debugging linear space intersections";
}

template <class CoefficientType>
bool ReflectionSubgroupWeylGroup::GetAlLDominantWeightsHWFDIM
(Vector<CoefficientType>& highestWeightSimpleCoords, HashedList<Vector<CoefficientType> >& outputWeightsSimpleCoords,
 int upperBoundDominantWeights, std::string& outputDetails, GlobalVariables& theGlobalVariables)
{ MacroRegisterFunctionWithName("ReflectionSubgroupWeylGroup::GetAlLDominantWeightsHWFDIM");
  std::stringstream out;
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
          if (outputWeightsByHeight[currentIndexShift].AddOnTopNoRepetition(currentWeight))
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
{ MacroRegisterFunctionWithName("ReflectionSubgroupWeylGroup::RaiseToDominantWeight");
  if (sign!=0)
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
void WeylGroup::ReflectBetaWRTAlpha(const Vector<Rational>& alpha, const Vector<CoefficientType>& Beta, bool RhoAction, Vector<CoefficientType>& Output)const
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
std::string MonomialChar<CoefficientType>::ToString
  (FormatExpressions* theFormat)const
{ std::stringstream out;
  bool useOmega=true;
  std::string oldCustomPlus;
  std::string VectorSpaceLetter="V";
  if (theFormat!=0)
  { useOmega=(theFormat->fundamentalWeightLetter=="");
    oldCustomPlus=theFormat->CustomPlusSign;
    theFormat->CustomPlusSign="";
    VectorSpaceLetter=theFormat->FDrepLetter;
  }
  if (useOmega)
    out << VectorSpaceLetter << "_{"
    << weightFundamentalCoords.ToStringLetterFormat("\\omega", theFormat) << "}";
  else
    out << VectorSpaceLetter << "_{"
    << weightFundamentalCoords.ToStringLetterFormat
    (theFormat->fundamentalWeightLetter, theFormat) << "}";
  if (theFormat!=0)
    theFormat->CustomPlusSign=oldCustomPlus;
  return out.str();
}

template<class CoefficientType>
bool charSSAlgMod<CoefficientType>::DrawMe
(std::string& outputDetails, GlobalVariables& theGlobalVariables,
 DrawingVariables& theDrawingVars, int upperBoundWeights, bool useMults)
{ charSSAlgMod<CoefficientType> CharCartan;
  bool result= this->FreudenthalEvalMeDominantWeightsOnly
  (CharCartan, upperBoundWeights, &outputDetails, &theGlobalVariables);
  std::stringstream out;
  Vectors<Rational> currentOrbit;
  WeylGroup& theWeyl=this->owner->theWeyl;
  theWeyl.DrawRootSystem(theDrawingVars, false, theGlobalVariables, true);
  int totalNumWeights=0;
  Vectors<CoefficientType> dominantWeightsNonHashed;
  HashedList<Vector<CoefficientType> > finalWeights;
  Vector<Rational> convertor;
  for (int i=0; i< CharCartan.size; i++)
  { const MonomialChar<CoefficientType>& currentMon=CharCartan[i];
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
      theDrawingVars.drawCircleAtVectorBuffer(convertor, 3, DrawingVariables::PenStyleNormal, CGI::RedGreenBlue(0,0,0));
      if (useMults)
        theDrawingVars.drawTextAtVectorBuffer
          (convertor, CharCartan.theCoeffs[i].ToString(), CGI::RedGreenBlue(0,0,0), theDrawingVars.PenStyleNormal, 0);
    }
  }
  out << "<br>Number of computed weights: " << totalNumWeights << ". ";
  if (result && totalNumWeights<upperBoundWeights)
    out << "<br>All weights were computed and are drawn. <br>";
  else
    out << "<br><b> Not all weights were computed. </b>";
  outputDetails=out.str();
  return result;
}

template <class CoefficientType>
void charSSAlgMod<CoefficientType>::DrawMeAssumeCharIsOverCartan
(WeylGroup& actualAmbientWeyl, GlobalVariables& theGlobalVariables, DrawingVariables& theDrawingVars)const
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
    (actualWeightRationalPart, this->theCoeffs[j].ToString(), CGI::RedGreenBlue(0,0,0), theDrawingVars.PenStyleNormal, 0);
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
//      int oldsize=theOrbit.size;
//      std::string debugString=tempRoot.ToString() ;
      theOrbit.AddOnTopNoRepetition(tempRoot);
//      if (oldsize<theOrbit.size)
//        std::cout << "<br>" << debugString << " with hash " << tempRoot.HashFunction() << " added, ";
//      else
//        std::cout << "<br>" << debugString << " with hash " << tempRoot.HashFunction() << " NOT added, ";
    }
    for (int j=1; j<this->ExternalAutomorphisms.size; j++)
    { ExternalAutosOverAmbientField.GetElement()=this->ExternalAutomorphisms[j];
      theOrbit[i].GetCoordsInBasiS(ExternalAutosOverAmbientField.GetElement(), tempRoot);
      theOrbit.AddOnTopNoRepetition(tempRoot);
    }
    if (UpperLimitNumElements>0)
      if (theOrbit.size>=UpperLimitNumElements)
      { result=false;
        break;
      }
  }
  outputOrbit=(theOrbit);
  return result;
}

template <class CoefficientType>
bool ReflectionSubgroupWeylGroup::FreudenthalEvalIrrepIsWRTLeviPart
(Vector<CoefficientType>& inputHWfundamentalCoords,
 HashedList<Vector<CoefficientType> >& outputDominantWeightsSimpleCoordS,
 List<CoefficientType>& outputMultsSimpleCoords, std::string& outputDetails,
 GlobalVariables& theGlobalVariables, int UpperBoundFreudenthal)
{ MacroRegisterFunctionWithName("ReflectionSubgroupWeylGroup::FreudenthalEvalIrrepIsWRTLeviPart");
  //double startTimer=theGlobalVariables.GetElapsedSeconds();
  this->ComputeRootSubsystem();
  Vector<Rational> EiVect;
  List<bool> Explored;
  CoefficientType theRingZero;
  theRingZero=inputHWfundamentalCoords[0].GetZero();
  /////////////////////////
  Vector<CoefficientType> hwSimpleCoordsLeviPart, hwSimpleCoordsNilPart;
  hwSimpleCoordsLeviPart=inputHWfundamentalCoords;
  hwSimpleCoordsNilPart=inputHWfundamentalCoords;
  for (int i=0; i<hwSimpleCoordsLeviPart.size; i++)
  { EiVect.MakeEi(hwSimpleCoordsLeviPart.size, i);
    if (!this->RootsOfBorel.Contains(EiVect))
      hwSimpleCoordsLeviPart[i]=theRingZero;
    else
      hwSimpleCoordsNilPart[i]=theRingZero;
  }
  hwSimpleCoordsLeviPart=this->AmbientWeyl.GetSimpleCoordinatesFromFundamental(hwSimpleCoordsLeviPart);
  hwSimpleCoordsNilPart=this->AmbientWeyl.GetSimpleCoordinatesFromFundamental(hwSimpleCoordsNilPart);
//  std::cout << "highest weight levi part simple coords: " << hwSimpleCoordsLeviPart.ToString();
//  std::cout << "highest weight nil part siple coords: " << hwSimpleCoordsNilPart.ToString();
  ///////////////////////////
  HashedList<Vector<CoefficientType> > outputDomWeightsSimpleCoordsLeviPart;

  if (!this->GetAlLDominantWeightsHWFDIM
      (hwSimpleCoordsLeviPart, outputDomWeightsSimpleCoordsLeviPart, UpperBoundFreudenthal, outputDetails, theGlobalVariables))
  { std::stringstream errorLog;
    errorLog << "Error: the number of dominant weights exceeded hard-coded limit of "
    << UpperBoundFreudenthal
    << ". Please check out whether LiE's implementation of the Freudenthal "
    << " formula can do your computation.";
    outputDetails=errorLog.str();
    return false;
  }
//  std::cout << "Highest weight: " << hwSimpleCoords.ToString() << "<br>Dominant weights wrt. levi part("
//  << outputDomWeightsSimpleCoordsLeviPart.size << "):<br> ";
//  for (int i=0; i<outputDomWeightsSimpleCoordsLeviPart.size; i++)
//    std::cout << "<br>" << outputDomWeightsSimpleCoordsLeviPart[i].ToString();
  Explored.initFillInObject(outputDomWeightsSimpleCoordsLeviPart.size, false);
  outputMultsSimpleCoords.SetSize(outputDomWeightsSimpleCoordsLeviPart.size);
  Vector<CoefficientType> currentWeight, currentDominantRepresentative;
  Vector<CoefficientType> Rho;
  Rho=this->GetRho();//<-implicit type conversion here!
//  std::cout << "<br> Rho equals: " << Rho.ToString();
  //out << "<br> Rho equals: " << Rho.ToString();
  CoefficientType hwPlusRhoSquared;
  hwPlusRhoSquared=this->AmbientWeyl.RootScalarCartanRoot(hwSimpleCoordsLeviPart+Rho, hwSimpleCoordsLeviPart+Rho);
  Explored[0]=true;
  outputMultsSimpleCoords[0]=1;
//  std::cout << "<br>time for generating weights and initializations: " << theGlobalVariables.GetElapsedSeconds()-startTimer;
  //static double totalTimeSpentOnHashIndexing=0;
//  static double timeSpentRaisingWeights=0;
//std::cout << "Freudenthal eval w.r.t subalgebra: positive root subsystem: " <<  this->RootsOfBorel.ToString();
  Vector<CoefficientType> convertor;
  CoefficientType bufferCoeff;
  ProgressReport theReport(&theGlobalVariables);
  for (int k=1; k< outputDomWeightsSimpleCoordsLeviPart.size; k++)
  { Explored[k]=true;
    CoefficientType& currentAccum=outputMultsSimpleCoords[k];
    currentAccum=0;
    for (int j=0; j<this->RootsOfBorel.size; j++)
      for (int i=1; ; i++)
      { convertor=this->RootsOfBorel[j];
        convertor*=i;
        currentWeight= outputDomWeightsSimpleCoordsLeviPart[k]+convertor;
        currentDominantRepresentative=currentWeight;
//        double startLocal=theGlobalVariables.GetElapsedSeconds();
        this->RaiseToDominantWeight(currentDominantRepresentative);
//        std::cout << "<br>currentDominant representative: " << currentDominantRepresentative.ToString();
        int theIndex=outputDomWeightsSimpleCoordsLeviPart.GetIndex(currentDominantRepresentative);
//        std::cout << "<br>index of currentDomain rep: " << theIndex;
        //totalTimeSpentOnHashIndexing+=theGlobalVariables.GetElapsedSeconds()-beforeHash;
        if (theIndex==-1)
          break;
//        std::cout << "<br> summing over weight: " << currentWeight.ToString();
        if (!Explored[theIndex])
        { std::stringstream errorLog;
          errorLog << "This is an internal error check. If you see it, that means "
          << " that the Freudenthal algorithm implementation is "
          << " wrong (because the author of the implementation was dumb enough to"
          << " try to write less code than what is "
          << " suggested by LiE).";
          outputDetails=errorLog.str();
          return false;
        }
        convertor=this->RootsOfBorel[j];//<-implicit type conversion here!
        bufferCoeff=this->AmbientWeyl.RootScalarCartanRoot(currentWeight, convertor);
        bufferCoeff*=outputMultsSimpleCoords[theIndex];
        currentAccum+=bufferCoeff;
//        std::cout << "<hr>current weight: " << currentWeight.ToString();
//        std::cout << "<br>current dominant representative " << currentDominantRepresentative.ToString();
      }
    currentAccum*=2;
   // std::cout << "<br>hwPlusRhoSquared: " << hwPlusRhoSquared.ToString();
    bufferCoeff=hwPlusRhoSquared;
    bufferCoeff-=this->AmbientWeyl.RootScalarCartanRoot
    (outputDomWeightsSimpleCoordsLeviPart[k]+Rho, outputDomWeightsSimpleCoordsLeviPart[k]+Rho);
    //bufferCoeff now holds the denominator participating in the Freudenthal formula.
    assert(!bufferCoeff.IsEqualToZero());
    currentAccum/=bufferCoeff;
//    std::cout << "<br>Coeff we divide by: " << bufferCoeff.ToString()
 //   ;
    std::stringstream out;
    out << " Computed the multiplicities of " << k+1 << " out of "
    << outputDomWeightsSimpleCoordsLeviPart.size << " dominant weights in the support.";
    theReport.Report(out.str());
//    std::cout
  //  << "<hr> Computed the multiplicities of " << k+1 << " out of " << outputDomWeightsSimpleCoordsLeviPart.size << " dominant weights in the support.";
 //   theGlobalVariables.MakeStatusReport(out.str());
//    std::cout << "<br>time so far: " << theGlobalVariables.GetElapsedSeconds()-startTimer;
//    std::cout << " of which " << totalTimeSpentOnHashIndexing << " used for hash routines";
//    std::cout << " of which " << timeSpentRaisingWeights << " used to raise weights";
  }
  outputDominantWeightsSimpleCoordS.Clear();
  outputDominantWeightsSimpleCoordS.SetExpectedSize(outputDomWeightsSimpleCoordsLeviPart.size);
  for (int i=0; i<outputDomWeightsSimpleCoordsLeviPart.size; i++)
    outputDominantWeightsSimpleCoordS.AddOnTop(outputDomWeightsSimpleCoordsLeviPart[i]+hwSimpleCoordsNilPart);
//  std::cout << "<br>Total freudenthal running time: " << theGlobalVariables.GetElapsedSeconds()-startTimer;
  return true;
}

template <class CoefficientType>
bool charSSAlgMod<CoefficientType>::SplitOverLeviMonsEncodeHIGHESTWeight
(std::string* Report, charSSAlgMod& output, const Selection& splittingParSel,
 const Selection& ParSelFDInducingPart,
 ReflectionSubgroupWeylGroup& outputWeylSub, GlobalVariables& theGlobalVariables)
{ MacroRegisterFunctionWithName("charSSAlgMod<CoefficientType>::SplitOverLeviMonsEncodeHIGHESTWeight");
  std::stringstream out;
  std::string tempS;
//  std::cout << "Splitting parabolic selection: " << splittingParSel.ToString();
  outputWeylSub.MakeParabolicFromSelectionSimpleRoots(this->GetOwner().theWeyl, splittingParSel, theGlobalVariables,1);
  outputWeylSub.ComputeRootSubsystem();
  ReflectionSubgroupWeylGroup complementGroup;
  Selection invertedSel;
  invertedSel=splittingParSel;
  invertedSel.InvertSelection();
  complementGroup.MakeParabolicFromSelectionSimpleRoots
  (this->owner->theWeyl, invertedSel, theGlobalVariables,1);
  complementGroup.ComputeRootSubsystem();
  out << outputWeylSub.ToString(false);
//  std::cout << "<hr> the Weyl subgroup: " << outputWeylSub.ToString(false);
//  std::cout << this->ToString();
//  std::cout << out.str();
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
  out << "Starting character: " << this->ToString();
  for (int i=0; i<this->size; i++)
  { MonomialChar<CoefficientType>& currentMon=this->TheObjects[i];
    if (!theFDWeyl.FreudenthalEvalIrrepIsWRTLeviPart
        (currentMon.weightFundamentalCoords, tempHashedRoots, tempMults, tempS, theGlobalVariables, 10000))
    { if (Report!=0)
        *Report=tempS;
      return false;
    }
//    std::cout << "<hr>FreudenthalEval on " << currentMon.ToString() << " generated the following report: "
//    << tempS << "<hr>";
    for (int j=0; j<tempHashedRoots.size; j++)
    { bufferCoeff=this->theCoeffs[i];
      tempMon.weightFundamentalCoords=theWeyL.GetFundamentalCoordinatesFromSimple(tempHashedRoots[j]);
      bufferCoeff*=tempMults[j];
      charAmbientFDWeyl.AddMonomial(tempMon, bufferCoeff);
    }
  }
//  std::cout << "<hr>" << tempS;
  std::cout << "<hr>Freudenthal eval ends up being: " << charAmbientFDWeyl.ToString();

  remainingCharDominantLevi.Reset();
  Vectors<CoefficientType> orbitDom;
  for (int i=0; i<charAmbientFDWeyl.size; i++)
  { orbitDom.SetSize(0);
    if (!theFDWeyl.GenerateOrbitReturnFalseIfTruncated
        (theWeyL.GetSimpleCoordinatesFromFundamental(charAmbientFDWeyl[i].weightFundamentalCoords),
         orbitDom, 10000))
    { out << "failed to generate the complement-sub-Weyl-orbit of weight "
      << this->GetOwner().theWeyl.GetSimpleCoordinatesFromFundamental
      (charAmbientFDWeyl[i].weightFundamentalCoords).ToString();
      if (Report!=0)
        *Report=out.str();
      return false;
    }
//    std::cout << "<hr>the orbit with highest weight "
//    << theWeyL.GetSimpleCoordinatesFromFundamental(charAmbientFDWeyl[i].weightFundamentalCoords).ToString()
//    << " is: ";
//    for (int l=0; l<orbitDom.size; l++)
//      std::cout <<"<br>" << orbitDom[l].ToString();
//    std::cout << "<hr>of them dominant are: <br>";
    for (int k=0; k<orbitDom.size; k++)
      if (outputWeylSub.IsDominantWeight(orbitDom[k]))
      { tempMon.weightFundamentalCoords=theWeyL.GetFundamentalCoordinatesFromSimple(orbitDom[k]);
        remainingCharDominantLevi.AddMonomial(tempMon, charAmbientFDWeyl.theCoeffs[i]);
//        std::cout << "<br>" << orbitDom[k].ToString() << " with coeff " << charAmbientFDWeyl.theCoeffs[i].ToString();
      }
  }
  output.Reset();
  out << "<br>Character w.r.t Levi part: " << CGI::GetHtmlMathDivFromLatexAddBeginArrayL
  (remainingCharDominantLevi.ToString());
//  std::cout << "<br>Character w.r.t Levi part: " << CGI::GetHtmlMathDivFromLatexAddBeginArrayL
// (remainingCharDominantLevi.ToString());

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
//    std::cout << "<hr>accounting " << localHighest.ToString() << " with coeff "
//    << highestCoeff.ToString() << "<br>"
//    << remainingCharDominantLevi.ToString();
    for (int i=0; i<tempHashedRoots.size; i++)
    { tempMon.weightFundamentalCoords=theWeyL.GetFundamentalCoordinatesFromSimple(tempHashedRoots[i]);
      bufferCoeff=tempMults[i];
      bufferCoeff*=highestCoeff;
      remainingCharDominantLevi.SubtractMonomial(tempMon, bufferCoeff);
//      std::cout << "<br>-(" << bufferCoeff.ToString() << ")" << tempMon.ToString();
    }
//    std::cout << "<br>remaining character after accounting:<br>" << remainingCharDominantLevi.ToString();
  }
  out << "<br>Character w.r.t Levi part: " << CGI::GetHtmlMathDivFromLatexAddBeginArrayL
  (output.ToString())
  ;

  if (Report!=0)
  { //out << "<hr>"  << "The split character is: " << output.ToString("V", "\\omega", false);
    DrawingVariables theDV;
    std::string tempS;
    this->DrawMeNoMults(tempS, theGlobalVariables, theDV, 10000);
    Vector<Rational> tempRoot;
    out << "<hr>In the following weight visualization, a yellow line is drawn if the corresponding weights are "
    << " simple reflections of one another, with respect to a simple root of the Levi part of the parabolic subalgebra. ";
    for (int i=0; i<output.size; i++)
    { tempRoot=theWeyL.GetSimpleCoordinatesFromFundamental(output[i].weightFundamentalCoords).GetVectorRational();
      outputWeylSub.DrawContour
      (tempRoot, theDV, theGlobalVariables, CGI::RedGreenBlue(200, 200, 0), 1000);
      std::stringstream tempStream;
      tempStream << output.theCoeffs[i].ToString();
      theDV.drawTextAtVectorBuffer(tempRoot, tempStream.str(), 0, DrawingVariables::PenStyleNormal, 0);
    }
    out << "<hr>" << theDV.GetHtmlFromDrawOperationsCreateDivWithUniqueName(theWeyL.GetDim());
    *Report=out.str();
  }
  return true;
}

template<class CoefficientType>
void ModuleSSalgebra<CoefficientType>::SplitOverLevi
  (std::string* Report, Selection& splittingParSel, GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit,
   const CoefficientType& theRingZero, List<ElementUniversalEnveloping<CoefficientType> >* outputEigenVectors,
   Vectors<CoefficientType>* outputWeightsFundCoords, Vectors<CoefficientType>* outputEigenSpace, charSSAlgMod<CoefficientType>* outputChar)
{ MacroRegisterFunctionWithName("ModuleSSalgebra<CoefficientType>::SplitOverLevi");
  if (this->theChaR.size!=1)
  { if (Report!=0)
    { std::stringstream out;
      out << "I have been instructed only to split modules that are irreducible over the ambient Lie algebra";
      out << " Instead I got the character " << this->theChaR.ToString() << " (" << this->theChaR.size << " monomials)";
      *Report=out.str();
    }
    return;
  }
  ReflectionSubgroupWeylGroup subWeyl;
  MemorySaving<charSSAlgMod<CoefficientType> > buffer;
  charSSAlgMod<CoefficientType>& charWRTsubalgebra= (outputChar==0) ? buffer.GetElement() : *outputChar;
  this->theChaR.SplitOverLeviMonsEncodeHIGHESTWeight
  (Report, charWRTsubalgebra, splittingParSel, this->parabolicSelectionNonSelectedAreElementsLevi,
   subWeyl, theGlobalVariables);
  Vector<Rational> theHWsimpleCoords, theHWfundCoords;
  std::stringstream out;
  if(Report!=0)
    out << *Report;
  Selection splittingParSelectedInLevi;
  splittingParSelectedInLevi=splittingParSel;
  splittingParSelectedInLevi.InvertSelection();
  if (!splittingParSelectedInLevi.IsSubset(this->parabolicSelectionSelectedAreElementsLevi))
  { out << "The parabolic subalgebra you selected is not a subalgebra of the ambient parabolic subalgebra."
    << " The parabolic has root of Levi given by " << splittingParSel.ToString()
    <<" while the ambient parabolic subalgebra has root of Levi given by "
    << this->parabolicSelectionNonSelectedAreElementsLevi.ToString();
    if (Report!=0)
      *Report=out.str();
    return;
  }
  out << "<br>Parabolic selection: " << splittingParSel.ToString();
  List<List<Vector<CoefficientType> > > eigenSpacesPerSimpleGenerator;
 // if (false)
  eigenSpacesPerSimpleGenerator.SetSize(splittingParSelectedInLevi.CardinalitySelection);
  Vectors<CoefficientType> tempSpace1, tempSpace2;
  MemorySaving<Vectors<CoefficientType> > tempEigenVects;
  Vectors<CoefficientType>& theFinalEigenSpace= (outputEigenSpace==0) ? tempEigenVects.GetElement() : *outputEigenSpace;
//  WeylGroup& theWeyL=this->theAlgebra.theWeyl;
  theFinalEigenSpace.MakeEiBasis(this->GetDim(), theRingUnit, theRingZero);
  for (int i=0; i<splittingParSelectedInLevi.CardinalitySelection; i++)
  { int theGenIndex=splittingParSelectedInLevi.elements[i]+this->GetOwner().GetRank()+this->GetOwner().GetNumPosRoots();
    MatrixTensor<CoefficientType>& currentOp=
    this->GetActionGeneratorIndeX(theGenIndex, theGlobalVariables, theRingUnit, theRingZero);
    Matrix<CoefficientType> currentOpMat;
    currentOp.GetMatrix(currentOpMat, this->GetDim() );
    currentOpMat.GetZeroEigenSpaceModifyMe(eigenSpacesPerSimpleGenerator[i]);
    tempSpace1=theFinalEigenSpace;
    tempSpace2=eigenSpacesPerSimpleGenerator[i];
    theFinalEigenSpace.IntersectTwoLinSpaces
    (tempSpace1, tempSpace2, theFinalEigenSpace, &theGlobalVariables)
    ;
  }
  out << "<br>Eigenvectors:<table> ";
//  Vector<Rational> zeroRoot;
//  zeroRoot.MakeZero(this->theAlgebra->GetRank());
  std::stringstream readyForLatexComsumption;
  readyForLatexComsumption << "\\begin{tabular}{|lll|}\n <br>";

  readyForLatexComsumption << "\\hline\\multicolumn{3}{|c|}{Highest weight $"
  << this->theHWFundamentalCoordsBaseField.ToStringLetterFormat("\\omega") << "$}\\\\\n<br>";
  readyForLatexComsumption << "weight fund. coord.& singular vector \\\\\\hline\n<br>";
  Vector<CoefficientType> currentWeight;
  Vector<CoefficientType> hwFundCoordsNilPart;
  hwFundCoordsNilPart=this->theHWFundamentalCoordsBaseField;
  hwFundCoordsNilPart-=this->theHWFDpartFundamentalCoordS;
  ElementUniversalEnveloping<CoefficientType> currentElt, tempElt;
  for (int j=0; j<theFinalEigenSpace.size; j++)
  { out << "<tr><td>";
    currentElt.MakeZero(this->GetOwner());
    Vector<CoefficientType>& currentVect= theFinalEigenSpace[j];
    int lastNonZeroIndex=-1;
    for (int i=0; i<currentVect.size; i++)
      if (!(currentVect[i].IsEqualToZero()))
      { tempElt.MakeZero(this->GetOwner());
        tempElt.AddMonomial(this->theGeneratingWordsNonReduced[i], 1);
        tempElt*=currentVect[i];
        currentElt+=tempElt;
        lastNonZeroIndex=i;
      }
    currentWeight=subWeyl.AmbientWeyl.GetFundamentalCoordinatesFromSimple
    (this->theGeneratingWordsWeightsPlusWeightFDpart[lastNonZeroIndex]);//<-implicitTypeConversionHere
    currentWeight+=hwFundCoordsNilPart;
    readyForLatexComsumption <<  "$" << currentWeight.ToStringLetterFormat("\\omega")
    << "$&$" << currentVect.ToStringLetterFormat("m") << "$";
    if (currentElt.size>1)
      out << "(";
    if (outputEigenVectors!=0)
      outputEigenVectors->AddOnTop(currentElt);
    if (outputWeightsFundCoords!=0)
      outputWeightsFundCoords->AddOnTop(currentWeight);
    out << currentElt.ToString(&theGlobalVariables.theDefaultFormat);
    if (currentElt.size>1)
      out << ")";
    out << " v_\\lambda";
    out << "</td><td>(weight: "
    << currentWeight.ToStringLetterFormat("\\omega") << ")</td></tr>";
    readyForLatexComsumption << "\\\\\n<br>";
  }
  out << "</table>";
  readyForLatexComsumption << "\\hline \n<br> \\end{tabular}";
  out << "<br>Your ready for LaTeX consumption text follows.<br>";
  out << readyForLatexComsumption.str();
  if (Report!=0)
    *Report=out.str();
}

template <class CoefficientType>
void charSSAlgMod<CoefficientType>::MakeTrivial(SemisimpleLieAlgebra& inputOwner)
{ this->MakeZero(inputOwner);
  MonomialChar<Rational> tempMon;
  tempMon.weightFundamentalCoords.MakeZero(inputOwner.GetRank());
  this->AddMonomial(tempMon, 1);
}

template <class CoefficientType>
std::string charSSAlgMod<CoefficientType>::operator*=(const charSSAlgMod& other)
{ GlobalVariables theGlobalVariables;
  return this->MultiplyBy(other, theGlobalVariables);
}

template <class CoefficientType>
std::string charSSAlgMod<CoefficientType>::MultiplyBy(const charSSAlgMod& other, GlobalVariables& theGlobalVariables)
{ if (this->owner!=other.owner || this->owner==0)
  { std::cout
    << "This is a programming error: attempting to multiply characters of different or non-initialized "
    << "semisimple Lie algebras."
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  this->SetExpectedSize(other.size+this->size);
  charSSAlgMod result, summand;
  result.MakeZero(this->owner);
  std::string potentialError;
  CoefficientType theCF;
  for (int i=0; i<this->size; i++)
    for (int j=0; j<other.size; j++)
    { const MonomialChar<Rational>& left = (*this)[i];
      const MonomialChar<Rational>& right=other[j];
      potentialError=left.TensorAndDecompose(right, *this->owner, summand, theGlobalVariables);
      if (potentialError!="")
        return potentialError;
      theCF=this->theCoeffs[i];
      theCF*=other.theCoeffs[j];
      summand*=theCF;
      result+=summand;
    }
  this->operator=(result);
  return "";
}

template <class CoefficientType>
void MonomialChar<CoefficientType>::AccountSingleWeight
(const Vector<Rational>& currentWeightSimpleCoords, const Vector<Rational>& otherHighestWeightSimpleCoords,
 WeylGroup& theWeyl,
 Rational& theMult, charSSAlgMod<CoefficientType>& outputAccum)const
{ //This is the Brauer-Klimyk formula. Reference:
  //Humphreys J., Introduction to Lie algebras and representation theory
  //page 142, exercise 9.
  //std::cout << "<hr>Accounting " << currentWeightSimpleCoords.ToString()
  //<< " with coefficient " << finalCoeff.ToString();
  MacroRegisterFunctionWithName("MonomialChar_CoefficientType::AccountSingleWeight");
  Vector<Rational> dominant=currentWeightSimpleCoords;
  dominant+=otherHighestWeightSimpleCoords;
  dominant+=theWeyl.rho;
  int sign;
  // a weight has no stabilizer if and only if it is not stabilized by all root reflections.
  for (int i=0; i<theWeyl.RootsOfBorel.size; i++)
    if (theWeyl.RootScalarCartanRoot(dominant, theWeyl.RootsOfBorel[i]).IsEqualToZero())
      return;
//  std::cout << "<br> Before raising to dominant, in simple coords: " << dominant.ToString() << "<br>";
  theWeyl.RaiseToDominantWeight(dominant, &sign);
//  std::cout << "After raising to dominant: " << dominant.ToString() << "<br>";
  dominant-=theWeyl.rho;
  if (!theWeyl.IsDominantWeight(dominant))
    return;
  MonomialChar<Rational> tempMon;
  tempMon.weightFundamentalCoords=theWeyl.GetFundamentalCoordinatesFromSimple(dominant);
  CoefficientType coeffChange;
  coeffChange= theMult;
  coeffChange*=sign;
//  std::cout << "; final contribution: " << tempMon.Coefficient.ToString()
//  << "*" << tempMon.weightFundamentalCoords.ToString() << "<br>";
  outputAccum.AddMonomial(tempMon, coeffChange);
}

template <class CoefficientType>
std::string MonomialChar<CoefficientType>::TensorAndDecompose
(const MonomialChar<CoefficientType>& other, SemisimpleLieAlgebra& owner,
 charSSAlgMod<CoefficientType>& output, GlobalVariables& theGlobalVariables)const
{ //This is the Brauer-Klimyk formula. Reference:
  //Humphreys J. Introduction to Lie algebras and representation theory
  //page 142, exercise 9.
  MacroRegisterFunctionWithName("MonomialChar_CoefficientType::TensorAndDecompose");
  std::stringstream errorLog;
  std::string tempS;
  output.MakeZero(&owner);
  WeylGroup& theWeyl=owner.theWeyl;
  Vector<Rational> leftHWFundCoords;
  leftHWFundCoords=this->weightFundamentalCoords;
  Vector<Rational> rightHWFundCoords;
  rightHWFundCoords =other.weightFundamentalCoords;

  Rational leftTotalDim= theWeyl.WeylDimFormulaFundamentalCoords(leftHWFundCoords);
  Rational rightTotalDim= theWeyl.WeylDimFormulaFundamentalCoords(rightHWFundCoords);
  if (leftTotalDim>rightTotalDim)
  { MathRoutines::swap(leftTotalDim, rightTotalDim);
    MathRoutines::swap(leftHWFundCoords, rightHWFundCoords);
  }
  HashedList<Vector<Rational> > weightsLeftSimpleCoords;
  List<Rational> multsLeft;
  if (!theWeyl.FreudenthalEval
      (leftHWFundCoords, weightsLeftSimpleCoords, multsLeft, &tempS, &theGlobalVariables, 1000000))
  { errorLog << "Freudenthal formula generated error: " << tempS;
    return errorLog.str();
  }
  HashedList<Vector<Rational> > currentOrbit;
  const int OrbitSizeHardLimit=10000000;
//  int theRank=theWeyl.GetDim();
  Vector<Rational> rightHWSimpleCoords=theWeyl.GetSimpleCoordinatesFromFundamental(rightHWFundCoords);
  Vectors<Rational> tempRoots;
  tempRoots.SetSize(1);
//  std::cout << "weights of smaller module: " << weightsLeftSimpleCoords.ToString();
  for (int i=0; i<weightsLeftSimpleCoords.size; i++)
  { tempRoots[0]=weightsLeftSimpleCoords[i];
    theWeyl.GenerateOrbit(tempRoots, false, currentOrbit, false, 0, 0, OrbitSizeHardLimit);
    if (currentOrbit.size>=OrbitSizeHardLimit)
    { errorLog << "Error: orbit layer size exceeded hard-coded limit of " << OrbitSizeHardLimit << ".";
      return errorLog.str();
    }
    for (int j=0; j<currentOrbit.size; j++)
      this->AccountSingleWeight
        (currentOrbit[j], rightHWSimpleCoords, theWeyl, multsLeft[i], output);
  }
//  std::cout << "<hr><hr><hr><hr>";
  return errorLog.str();
}

template <class CoefficientType>
std::string charSSAlgMod<CoefficientType>::ElementToStringCharacter
(List<Vector<Rational> >& theWeights, List<Rational>& theMults)
{ std::stringstream out;
  MonomialChar<Rational> currentSummand;
  charSSAlgMod theMod;
  theMod.MakeZero(*this->listOwners, this->indexInOwners);
  theMod.Reserve(theWeights.size);
  theMod.SetHashSizE(theWeights.size);
  assert(theMults.size==theWeights.size);
  for (int i=0; i<theWeights.size; i++)
  { currentSummand.weightFundamentalCoords=
    this->listOwners->TheObjects[this->indexInOwners].theWeyl.GetFundamentalCoordinatesFromSimple(theWeights[i]);
    theMod.AddMonomial(currentSummand, theMults[i]);
  }
  out << CGI::GetHtmlMathSpanFromLatexFormulaAddBeginArrayL(theMod.ToString());
  return out.str();
}

template <class CoefficientType>
bool charSSAlgMod<CoefficientType>::FreudenthalEvalMeFullCharacter
 (charSSAlgMod<CoefficientType>& outputCharOwnerSetToZero, int upperBoundNumDominantWeights,
  std::string* outputDetails, GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("charSSAlgMod_CoefficientType::FreudenthalEvalMeFullCharacter");
  charSSAlgMod<CoefficientType> domChar;
  if (!this->FreudenthalEvalMeDominantWeightsOnly
      (domChar, upperBoundNumDominantWeights, outputDetails, theGlobalVariables))
    return false;
  outputCharOwnerSetToZero.Reset();
  Vectors<Rational> theVect;
  HashedList<Vector<Rational> > theOrbit;
  theVect.SetSize(1);
  MonomialChar<CoefficientType> tempMon;
  for (int i=0; i<domChar.size; i++)
  { theVect[0]=this->GetOwner().theWeyl.GetSimpleCoordinatesFromFundamental
    (domChar[i].weightFundamentalCoords);
    if (!this->GetOwner().theWeyl.GenerateOrbit(theVect, false, theOrbit, false, -1, 0, upperBoundNumDominantWeights))
    { if (outputDetails!=0)
        *outputDetails= "failed to generate orbit (possibly too large?)";
      return false;
    }
    for (int j=0; j<theOrbit.size; j++)
    { tempMon.weightFundamentalCoords
      =this->GetOwner().theWeyl.GetFundamentalCoordinatesFromSimple(theOrbit[j]);
      outputCharOwnerSetToZero.AddMonomial(tempMon, domChar.theCoeffs[i]);
    }
  }
  return true;
}

template <class CoefficientType>
bool charSSAlgMod<CoefficientType>::FreudenthalEvalMeDominantWeightsOnly
 (charSSAlgMod<CoefficientType>& outputCharOwnerSetToZero, int upperBoundNumDominantWeights,
  std::string* outputDetails, GlobalVariables* theGlobalVariables)
{ MacroRegisterFunctionWithName("charSSAlgMod_CoefficientType::FreudenthalEvalMeDominantWeightsOnly");
  assert(&outputCharOwnerSetToZero!=this);
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
    (currentWeightFundCoords, currentWeights, currentMults, &localDetail, theGlobalVariables, upperBoundNumDominantWeights))
    { if (outputDetails!=0)
      { localErrors << "Encountered error while evaluating freudenthal formula. Error details: " << localDetail
        << "<br> Further computation detail: " << localDetails.str();
        *outputDetails=localErrors.str();
      }
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
  if (outputDetails!=0)
    *outputDetails=localDetails.str();
  return true;
}

template <class CoefficientType>
bool WeylGroup::GenerateOrbit
(Vectors<CoefficientType>& theRoots, bool RhoAction, HashedList<Vector<CoefficientType> >& output,
 bool UseMinusRho, int expectedOrbitSize, HashedList<ElementWeylGroup>* outputSubset, int UpperLimitNumElements)
{ output.Clear();
  for (int i=0; i<theRoots.size; i++)
    output.AddOnTop(theRoots[i]);
  Vector<CoefficientType> currentRoot;
  ElementWeylGroup tempEW;
  tempEW.owner=this;
  if (expectedOrbitSize<=0)
    expectedOrbitSize=(this->theDynkinType.GetSizeWeylGroupByFormula()).NumShort;
  if (UpperLimitNumElements>0 && expectedOrbitSize>UpperLimitNumElements)
    expectedOrbitSize=UpperLimitNumElements;
  output.SetExpectedSize(expectedOrbitSize);
  if (outputSubset!=0)
  { if (UpperLimitNumElements!=-1)
      expectedOrbitSize=MathRoutines::Minimum(UpperLimitNumElements, expectedOrbitSize);
    tempEW.size=0;
    outputSubset->SetExpectedSize(expectedOrbitSize);
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
      if (!RhoAction)
        this->SimpleReflection(j, currentRoot);
      else if (!UseMinusRho)
        this->SimpleReflectionRhoModified(j, currentRoot);
      else
        this->SimpleReflectionMinusRhoModified(j, currentRoot);
      //if (this->flagAnErrorHasOcurredTimeToPanic)
      //{ currentRoot.ComputeDebugString();
      //}
      if (output.AddOnTopNoRepetition(currentRoot))
        if (outputSubset!=0)
        { tempEW.AddOnTop(j);
          outputSubset->AddOnTop(tempEW);
          tempEW.RemoveIndexSwapWithLast(tempEW.size-1);
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
(Vector<CoefficientType>& inputHWfundamentalCoords,
 HashedList<Vector<CoefficientType> >& outputDominantWeightsSimpleCoords,
 List<CoefficientType>& outputMultsSimpleCoords, std::string* outputDetails,
 GlobalVariables* theGlobalVariables, int UpperBoundFreudenthal)
{ //double startTimer=theGlobalVariables.GetElapsedSeconds();
  MacroRegisterFunctionWithName("WeylGroup::FreudenthalEval");
  for (int i=0; i<inputHWfundamentalCoords.size; i++)
    if (inputHWfundamentalCoords[i]<0)
    { if (outputDetails!=0)
        *outputDetails= "The highest weight is not dominant and I cannot apply the Freudenthal formula.";
      return false;
    }
  this->ComputeRho(true);
  Vectors<Rational> EiBasis;
  EiBasis.MakeEiBasis(this->GetDim());
  List<bool> Explored;
  Vector<CoefficientType> hwSimpleCoords=this->GetSimpleCoordinatesFromFundamental(inputHWfundamentalCoords);
  if (!this->GetAlLDominantWeightsHWFDIM
      (hwSimpleCoords, outputDominantWeightsSimpleCoords, UpperBoundFreudenthal, outputDetails, theGlobalVariables))
    return false;
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
  ProgressReport theReport(theGlobalVariables);
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
//        std::cout << "<br> summing over weight: " << currentWeight.ToString();
        if (!Explored[theIndex])
        { if (outputDetails!=0)
          { std::stringstream errorLog;
            errorLog << "This is an internal error check. If you see it, that means "
            << " that the Freudenthal algorithm implementation is "
            << " wrong (because the author of the implementation was dumb enough to try to write less code than what is "
            << " suggested by LiE).";
            *outputDetails=errorLog.str();
          }
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
    if (BufferCoeff==0)
    { std::cout << "This is a programming or a mathematical error. I get "
      << " that the denominator in the Freundenthal formula is zero. "
      << " The highest weight is " << inputHWfundamentalCoords.ToString()
      << ". The Weyl group details follow. "
      << this->ToString();
      assert(false);
    }
    currentAccum/=BufferCoeff;

//    std::cout << "<br>Coeff we divide by: " << (hwPlusRhoSquared-this->RootScalarCartanRoot
 //   (outputDominantWeightsSimpleCoords[k]+this->rho, outputDominantWeightsSimpleCoords[k]+this->rho))
  //  .ToString()
   // ;
    std::stringstream out;
    out << " Computed the multiplicities of " << k+1 << " out of "
    << outputDominantWeightsSimpleCoords.size << " dominant weights in the support.";
    theReport.Report(out.str());
//    std::cout
//    << "<hr> Computed the multiplicities of " << k+1 << " out of " << outputDominantWeightsSimpleCoords.size << " dominant weights in the support.";
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
 int upperBoundDominantWeights, std::string* outputDetails, GlobalVariables* theGlobalVariables)
{ std::stringstream out;
//  double startTime=theGlobalVariables.GetElapsedSeconds();
//  std::cout << "<br>time elapsed: " << theGlobalVariables.GetElapsedSeconds()-startTime;
  Vector<CoefficientType> highestWeightTrue;
  highestWeightTrue=highestWeightSimpleCoords;
  this->RaiseToDominantWeight(highestWeightTrue);
  Vector<CoefficientType> highestWeightFundCoords=this->GetFundamentalCoordinatesFromSimple(highestWeightTrue);
  if (!highestWeightFundCoords.SumCoords().IsSmallInteger())
  { if (outputDetails!=0)
    { out << "<hr> The highest weight you gave in simple coordinates: "
      << highestWeightSimpleCoords.ToString()
      << " which equals " << highestWeightFundCoords.ToString()
      << "  in fundamental coordinates "
      << " is not integral dominant.<br>";
      *outputDetails=out.str();
    }
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
  for(int lowestUnexploredHeightDiff=0;
      lowestUnexploredHeightDiff<=theTopHeightSimpleCoords;
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
          if (outputWeightsByHeight[currentIndexShift].AddOnTopNoRepetition(currentWeight))
          { numTotalWeightsFound++;
            outputWeightsByHeight[currentIndexShift].AdjustHashes();
          }
        }
      }
//    std::cout << "<br>time spent before accounting at height level " << lowestUnexploredHeightDiff
//    << ": " << theGlobalVariables.GetElapsedSeconds()-startCycleTime;
//    std::cout << " Size of current level: " << currentHashes.size;
    outputWeightsSimpleCoords.AddOnTop(currentHashes);
//    std::cout << ". Time spent after accounting at height level "
//    << lowestUnexploredHeightDiff
//    << ": " << theGlobalVariables.GetElapsedSeconds()-startCycleTime;
//    startCycleTime=theGlobalVariables.GetElapsedSeconds();
    outputWeightsSimpleCoords.AdjustHashes();
    currentHashes.Clear();
//    std::cout << ". Time spent clearing up buffer at height level " << lowestUnexploredHeightDiff
//    << ": " << theGlobalVariables.GetElapsedSeconds()-startCycleTime;
    if (numTotalWeightsFound>upperBoundDominantWeights && upperBoundDominantWeights>0)
    { out << "<hr>The number of weights has "
      << "exceeded the RAM memory limits, aborting the weight generation. ";
      return false;
    }
  }
  if (outputDetails!=0)
  { out << " Total number of dominant weights: " << outputWeightsSimpleCoords.size;
    *outputDetails=out.str();
  }
  return true;
}

template <class CoefficientType>
void ElementSumGeneralizedVermas<CoefficientType>::MakeHWV
(ModuleSSalgebra<CoefficientType>& theOwner, const CoefficientType& theRingUnit)
{ this->MakeZero();
  MonomialGeneralizedVerma<CoefficientType> theMon;
  theMon.indexFDVector=theOwner.theGeneratingWordsNonReduced.size-1;
  theMon.theMonCoeffOne.MakeConst(theOwner.GetOwner());
  theMon.owneR=&theOwner;
  this->AddMonomial(theMon, theRingUnit);
}

template <class CoefficientType>
void ReflectionSubgroupWeylGroup::ActByElement(int index, Vector<CoefficientType>& theRoot)const
{ Vector<CoefficientType> tempRoot;
  this->ActByElement(index, theRoot, tempRoot);
  theRoot=(tempRoot);
}

template <class CoefficientType>
void ReflectionSubgroupWeylGroup::ActByElement
(const ElementWeylGroup& theElement, const Vector<CoefficientType>& input, Vector<CoefficientType>& output)const
{ assert(&input!=&output);
  int NumElts=theElement.size;
  Vector<CoefficientType> tempRoot, tempRoot2;
  output=(input);
  for (int i=0; i<NumElts; i++)
  { int tempI=theElement[i];
    if(tempI<this->simpleGenerators.size)
      this->AmbientWeyl.ReflectBetaWRTAlpha(this->simpleGenerators.TheObjects[tempI], output, false, output);
    else
    { tempI-=this->simpleGenerators.size;
      tempRoot.MakeZero(input.size);
      for (int j=0; j<output.size; j++)
      { tempRoot2=this->ExternalAutomorphisms[tempI][j];
        tempRoot2*=output[j];
        tempRoot+=tempRoot2;
      }
    }
  }
}

template<class CoefficientType>
void ReflectionSubgroupWeylGroup::ActByElement
(const ElementWeylGroup& theElement, const Vectors<CoefficientType>& input, Vectors<CoefficientType>& output)const
{ assert(&input!=&output);
  output.SetSize(input.size);
  for (int i=0; i<input.size; i++)
    this->ActByElement(theElement, input[i], output[i]);
}

template <class CoefficientType>
MatrixTensor<CoefficientType>& ModuleSSalgebra<CoefficientType>::GetActionSimpleGeneratorIndex
(int generatorIndex, GlobalVariables& theGlobalVariables,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ Vector<Rational> genWeight=this->GetOwner().GetWeightOfGenerator(generatorIndex);
  Vector<Rational> targetWeight;
  Pair
  <MonomialTensor<int, MathRoutines::IntUnsignIdentity>,
    MonomialTensor<int, MathRoutines::IntUnsignIdentity> >
  currentPair;
  MatrixTensor<CoefficientType>& outputMat= this->actionsGeneratorsMaT[generatorIndex];
  Vector<CoefficientType> theScalarProds;
  outputMat.MakeZero();
  for (int i=0; i<this->theGeneratingWordsGrouppedByWeight.size; i++)
  { List<MonomialTensor<int, MathRoutines::IntUnsignIdentity> >& currentWordList=
    this->theGeneratingWordsIntGrouppedByWeight[i];
    const Vector<Rational>& currentWeight=this->theModuleWeightsSimpleCoords[i];
    targetWeight=currentWeight+genWeight;
//    std::cout << "<br>target weight: " << targetWeight.ToString() << "="
//    << currentWeight.ToString() << "+" << inputWeight.ToString();
    int weightLevelIndex=this->theModuleWeightsSimpleCoords.GetIndex(targetWeight);
    if (weightLevelIndex!=-1)
    { int columnOffset=this->GetOffsetFromWeightIndex(i);
      int rowOffset=this->GetOffsetFromWeightIndex(weightLevelIndex);
      List<MonomialTensor<int, MathRoutines::IntUnsignIdentity> >&
      otherWordList=this->theGeneratingWordsIntGrouppedByWeight[weightLevelIndex];
      for (int j=0; j<currentWordList.size; j++)
      { theScalarProds.SetSize(otherWordList.size);
        for (int k=0; k<otherWordList.size; k++)
        { if (generatorIndex>this->GetOwner().GetNumPosRoots())
          { currentPair.Object1=currentWordList[j];
            this->ApplyTAA(currentPair.Object1);
            currentPair.Object2=otherWordList[k];
            currentPair.Object2.MultiplyByGeneratorPowerOnTheLeft
            (this->GetOwner().GetOppositeGeneratorIndex(generatorIndex), 1);
          } else
          { currentPair.Object1=currentWordList[j];
            currentPair.Object1.MultiplyByGeneratorPowerOnTheLeft(generatorIndex, 1);
            this->ApplyTAA(currentPair.Object1);
            currentPair.Object2=otherWordList[k];
          }
          ProgressReport theReport(&theGlobalVariables);
          theScalarProds[k]=this->hwTrace(currentPair, &theReport);
        }
        this->theBilinearFormsInverted[weightLevelIndex].ActOnVectorColumn(theScalarProds);
        for (int k=0; k<theScalarProds.size; k++)
          outputMat.AddMonomial(MonomialMatrix(rowOffset+k, columnOffset+j), theScalarProds[k]);
      }
    }
  }
  return outputMat;
}

template <class CoefficientType>
void ElementUniversalEnvelopingOrdered<CoefficientType>::ModOutVermaRelations
  (GlobalVariables* theContext, const List<CoefficientType>* subHiGoesToIthElement,
   const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ MonomialUniversalEnvelopingOrdered<CoefficientType> tempMon;
  ElementUniversalEnvelopingOrdered<CoefficientType> output;
  output.MakeZero(*this->owner);
  for (int i=0; i<this->size; i++)
  { tempMon= this->TheObjects[i];
//    tempMon.ComputeDebugString();
    tempMon.ModOutVermaRelations(theContext, subHiGoesToIthElement, theRingUnit, theRingZero);
//    tempMon.ComputeDebugString();
    output.AddMonomial(tempMon);
  }
  this->operator=(output);
}

template <class CoefficientType>
void MonomialUniversalEnvelopingOrdered<CoefficientType>::ModOutVermaRelations
(GlobalVariables* theContext, const List<CoefficientType>* subHiGoesToIthElement,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ int numPosRoots=this->owner->theOwner.GetNumPosRoots();
  int theDimension=this->owner->theOwner.GetRank();
  for (int i=this->generatorsIndices.size-1; i>=0; i--)
  { int IndexCurrentGenerator=this->generatorsIndices.TheObjects[i];
    if (IndexCurrentGenerator>=numPosRoots+theDimension)
    { this->MakeZero(theRingZero, *this->owner);
      return;
    }
    if (IndexCurrentGenerator<numPosRoots)
      return;
    if (IndexCurrentGenerator>=numPosRoots &&  IndexCurrentGenerator<numPosRoots+theDimension)
    { int theDegree;
      if (!this->Powers.TheObjects[i].IsSmallInteger(theDegree))
        return;
      if (subHiGoesToIthElement==0)
      { this->MakeZero(theRingZero, *this->owner);

        return;
      }
      CoefficientType theSubbedH=theRingZero;
      Vector<Rational>& currentH=this->owner->theOrder.TheObjects[IndexCurrentGenerator].Hcomponent;
      for (int j=0; j<currentH.size; j++)
        theSubbedH+=(*subHiGoesToIthElement)[j]*currentH[j];
      MathRoutines::RaiseToPower(theSubbedH, theDegree, theRingUnit);
//      this->owner->theOrder.TheObjects[IndexCurrentGenerator].Hcomponent.ComputeDebugString();
      //assert(this->Coefficient.checkConsistency());
      this->Coefficient.operator*=(theSubbedH);
      //assert(this->Coefficient.checkConsistency());
      this->generatorsIndices.size--;
      this->Powers.size--;
    }
  }
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::ModOutVermaRelations
(GlobalVariables* theContext, const Vector<CoefficientType>* subHiGoesToIthElement,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ MonomialUniversalEnveloping<CoefficientType> tempMon;
  ElementUniversalEnveloping<CoefficientType> output;
  output.MakeZero(*this->owneR);
  CoefficientType acquiredCoeff;
  for (int i=0; i<this->size; i++)
  { tempMon= this->TheObjects[i];
    tempMon.ModOutVermaRelations(acquiredCoeff, theContext, subHiGoesToIthElement, theRingUnit, theRingZero);
    acquiredCoeff*=this->theCoeffs[i];
//    std::cout << "<hr><hr>Adding " << tempMon.ToString() << " times " << acquiredCoeff.ToString() << " to " << output.ToString();
    output.AddMonomial(tempMon, acquiredCoeff);
//    std::cout <<"<br> to obtain " << output.ToString();
  }
  this->operator=(output);
}

template <class CoefficientType>
void ElementUniversalEnveloping<CoefficientType>::Simplify
(GlobalVariables& theGlobalVariables, const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ ElementUniversalEnveloping<CoefficientType> buffer;
  ElementUniversalEnveloping<CoefficientType> outpuT;
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  CoefficientType currentCoeff;
  outpuT.MakeZero(*this->owneR);
  for (; this->size>0; )
  {// FormatExpressions tempFormat;
    //tempFormat.MakeAlphabetArbitraryWithIndex("g", "h");
    //std::cout << "<hr>(At the start of reduction cycle) *this+output - (At the end of reduction cycle)(*this+output)=<br>" << (*this+outpuT).ToString(&tempFormat);
    this->PopMonomial(this->size-1, tempMon, currentCoeff);
    bool reductionOccurred=false;
    for (int i=0; i<tempMon.generatorsIndices.size-1; i++)
      if (!this->GetOwner().AreOrderedProperly(tempMon.generatorsIndices[i], tempMon.generatorsIndices[i+1]))
      { if (tempMon.SwitchConsecutiveIndicesIfTheyCommute(i))
        { this->AddMonomial(tempMon, currentCoeff);
          reductionOccurred=true;
          break;
        }
        if (tempMon.CommutingAnBtoBAnPlusLowerOrderAllowed
            (tempMon.Powers[i], tempMon.generatorsIndices[i], tempMon.Powers[i+1], tempMon.generatorsIndices[i+1]))
        { tempMon.CommuteAnBtoBAnPlusLowerOrder(i, buffer, theRingUnit, theRingZero);
          buffer*=currentCoeff;
          *this+=buffer;
          reductionOccurred=true;
          break;
        }
        if (tempMon.CommutingABntoBnAPlusLowerOrderAllowed
            (tempMon.Powers[i], tempMon.generatorsIndices[i], tempMon.Powers[i+1], tempMon.generatorsIndices[i+1]))
        { tempMon.CommuteABntoBnAPlusLowerOrder(i, buffer, theRingUnit, theRingZero);
          buffer*=currentCoeff;
          *this+=buffer;
          reductionOccurred=true;
          break;
        }
      }
    if(!reductionOccurred)
      outpuT.AddMonomial(tempMon, currentCoeff);
//    std::cout << "-<br>(" << (*this+outpuT).ToString() << ")<br>(this should simplify to zero).";
  }
  *this=outpuT;
}

template <class CoefficientType>
bool MonomialUniversalEnveloping<CoefficientType>::CommutingAnBtoBAnPlusLowerOrderAllowed
(CoefficientType& theLeftPower, int leftGeneratorIndex,
 CoefficientType& theRightPower, int rightGeneratorIndex)
{ return this->CommutingABntoBnAPlusLowerOrderAllowed
(theRightPower, rightGeneratorIndex, theLeftPower, leftGeneratorIndex);
}

template <class CoefficientType>
bool MonomialUniversalEnveloping<CoefficientType>::CommutingABntoBnAPlusLowerOrderAllowed
(CoefficientType& theLeftPower, int leftGeneratorIndex, CoefficientType& theRightPower, int rightGeneratorIndex)
{ int leftPower, rightPower;
  if (!theLeftPower.IsSmallInteger(&leftPower))
    return false;
  if (leftPower<0)
    return false;
  if(theRightPower.IsSmallInteger(&rightPower))
    if (rightPower>=0)
      return true;
  int numPosRoots=this->GetOwner().theWeyl.RootsOfBorel.size;
  int theDimension= this->GetOwner().theWeyl.CartanSymmetric.NumRows;
  if(rightGeneratorIndex>= numPosRoots && rightGeneratorIndex<numPosRoots+theDimension)
    return this->GetOwner().theLiebrackets.elements[leftGeneratorIndex][rightGeneratorIndex].IsEqualToZero();
  return true;
}

template <class CoefficientType>
bool MonomialUniversalEnveloping<CoefficientType>::SwitchConsecutiveIndicesIfTheyCommute
(int theLeftIndex)
{ if (theLeftIndex>= this->generatorsIndices.size-1)
    return false;
  int leftGenerator=this->generatorsIndices[theLeftIndex];
  int rightGenerator=this->generatorsIndices[theLeftIndex+1];
  if (!this->GetOwner().theLiebrackets.elements[leftGenerator][rightGenerator].IsEqualToZero())
    return false;
  this->generatorsIndices.SwapTwoIndices(theLeftIndex, theLeftIndex+1);
  this->Powers.SwapTwoIndices(theLeftIndex, theLeftIndex+1);
  this->SimplifyEqualConsecutiveGenerators(theLeftIndex-1);
  return true;
}

template <class CoefficientType>
void MonomialUniversalEnveloping<CoefficientType>::CommuteAnBtoBAnPlusLowerOrder
(int indexA, ElementUniversalEnveloping<CoefficientType>& output, const CoefficientType& theRingUnit,
 const CoefficientType& theRingZero)
{ if (indexA==this->generatorsIndices.size-1)
    return;
  output.MakeZero(*this->owneR);
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  tempMon.init(*this->owneR);
  tempMon.Powers.SetExpectedSize(this->generatorsIndices.size+2);
  tempMon.generatorsIndices.SetExpectedSize(this->generatorsIndices.size+2);
  tempMon.Powers.size=0;
  tempMon.generatorsIndices.size=0;
  int rightGeneratorIndeX= this->generatorsIndices[indexA+1];
  int leftGeneratorIndeX=this->generatorsIndices[indexA];
  CoefficientType theRightPoweR, theLeftPoweR;
  theRightPoweR= this->Powers[indexA+1];
  theLeftPoweR= this->Powers[indexA];
  theRightPoweR-=1;
  int powerDroP=0;
  CoefficientType acquiredCoefficienT, incomingAcquiredCoefficienT;
  acquiredCoefficienT=theRingUnit;
  for (int i=0; i<indexA; i++)
    tempMon.MultiplyByGeneratorPowerOnTheRight(this->generatorsIndices[i], this->Powers[i]);
  MonomialUniversalEnveloping<CoefficientType> startMon;
  startMon=tempMon;
  ElementSemisimpleLieAlgebra<Rational> adAToTheIthOfB, aElt;
  adAToTheIthOfB.MakeGenerator(rightGeneratorIndeX, *this->owneR);
  aElt.MakeGenerator(leftGeneratorIndeX, *this->owneR);
  //Formula realized:
  //a^n b =\sum_{i=0}^\infty \binom{n}{i} (\ad a)^i (b)a^{n-i}
  //Proof (Dixmier): let L_x stand for left multiplication by x and R_x stand for right multiplication.
  //Then L_x and R_x commute and L_x-R_x=ad_x, i.e.
  //(L_a)^n=(R_a+ad_a)^n.
  do
  { for (int i=0; i<adAToTheIthOfB.size; i++)
    { int theNewGeneratorIndex=adAToTheIthOfB[i].theGeneratorIndex;
      tempMon=startMon;
      incomingAcquiredCoefficienT=acquiredCoefficienT;
      incomingAcquiredCoefficienT*=adAToTheIthOfB.theCoeffs[i];
      tempMon.MultiplyByGeneratorPowerOnTheRight(theNewGeneratorIndex, theRingUnit);
      tempMon.MultiplyByGeneratorPowerOnTheRight(leftGeneratorIndeX, theLeftPoweR);
      tempMon.MultiplyByGeneratorPowerOnTheRight(rightGeneratorIndeX, theRightPoweR);
      for (int i=indexA+2; i<this->generatorsIndices.size; i++)
        tempMon.MultiplyByGeneratorPowerOnTheRight(this->generatorsIndices[i], this->Powers[i]);
      //tempMon.ComputeDebugString();
      output.AddMonomial(tempMon, incomingAcquiredCoefficienT);
    }
    powerDroP++;
    acquiredCoefficienT*=(theLeftPoweR);
    acquiredCoefficienT/=(powerDroP);
    theLeftPoweR-=1;
//    std::cout <<"<hr>(ad_a)(" << adAToTheIthOfB.ToString(0) << ") =";
//    int commentwhendone;
//    aElt.checkConsistency();
//    adAToTheIthOfB.checkConsistency();
    this->GetOwner().LieBracket(aElt, adAToTheIthOfB, adAToTheIthOfB);
//    aElt.checkConsistency();
//    adAToTheIthOfB.checkConsistency();
//    std::cout << adAToTheIthOfB.ToString(0);
  }while(!adAToTheIthOfB.IsEqualToZero() && !acquiredCoefficienT.IsEqualToZero());
//  std::cout << "<hr>final output: " << this->ToString() << " = " << output.ToString();
}

template <class CoefficientType>
void MonomialUniversalEnveloping<CoefficientType>::CommuteABntoBnAPlusLowerOrder
(int theIndeX, ElementUniversalEnveloping<CoefficientType>& output,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ if (theIndeX==this->generatorsIndices.size-1)
    return;
  output.MakeZero(*this->owneR);
  MonomialUniversalEnveloping<CoefficientType> tempMon;
  tempMon.init(*this->owneR);
  tempMon.Powers.SetExpectedSize(this->generatorsIndices.size+2);
  tempMon.generatorsIndices.SetExpectedSize(this->generatorsIndices.size+2);
  tempMon.Powers.size=0;
  tempMon.generatorsIndices.size=0;
  int rightGeneratorIndex= this->generatorsIndices[theIndeX+1];
  int leftGeneratorIndex=this->generatorsIndices[theIndeX];
  CoefficientType theRightPower, theLeftPower;
  theRightPower= this->Powers[theIndeX+1];
  theLeftPower= this->Powers[theIndeX];
  theLeftPower-=1;
  int powerDrop=0;
  CoefficientType acquiredCoefficient, incomingAcquiredCoefficient;
  acquiredCoefficient=theRingUnit;
  for (int i=0; i<theIndeX; i++)
    tempMon.MultiplyByGeneratorPowerOnTheRight(this->generatorsIndices[i], this->Powers[i]);
  tempMon.MultiplyByGeneratorPowerOnTheRight(this->generatorsIndices[theIndeX], theLeftPower);
  MonomialUniversalEnveloping<CoefficientType> startMon, tempMon2;
  startMon=tempMon;
  ElementSemisimpleLieAlgebra<Rational> adResult, tempElt, rightGeneratorElt;
  adResult.MakeGenerator(leftGeneratorIndex, *this->owneR);
  rightGeneratorElt.MakeGenerator(rightGeneratorIndex, *this->owneR);
//  std::cout << this->ToString() << "=";
  //Formula realized:
  //a b^n  &=&\sum_{i=0}^\infty b^{n-i}(-\ad b)^i (a) \binom{n}{i} .
  //Proof (Dixmier): let L_x stand for left multiplication by x and R_x stand for right multiplication.
  //Then L_x and R_x commute and L_x-R_x=ad_x, i.e.
  //(L_b-ad_b)^n=R_b^n.
  do
  { for (int i=0; i<adResult.size; i++)
    { int theNewGeneratorIndex= adResult[i].theGeneratorIndex;
      tempMon=startMon;
      tempMon.MultiplyByGeneratorPowerOnTheRight(rightGeneratorIndex, theRightPower);
      incomingAcquiredCoefficient=acquiredCoefficient;
      incomingAcquiredCoefficient*=(adResult.theCoeffs[i]);
      tempMon.MultiplyByGeneratorPowerOnTheRight(theNewGeneratorIndex, theRingUnit);
      for (int i=theIndeX+2; i<this->generatorsIndices.size; i++)
        tempMon.MultiplyByGeneratorPowerOnTheRight(this->generatorsIndices[i], this->Powers[i]);
      output.AddMonomial(tempMon, incomingAcquiredCoefficient);
//      std::cout << "+(" << incomingAcquiredCoefficient.ToString() << ")" << tempMon.ToString();
    }
    acquiredCoefficient*=(theRightPower);
    theRightPower-=1;
    this->GetOwner().LieBracket(adResult, rightGeneratorElt, tempElt);
    adResult=tempElt;
    powerDrop++;
    acquiredCoefficient/=(powerDrop);
  }while(!adResult.IsEqualToZero() && !acquiredCoefficient.IsEqualToZero());
}

template <class CoefficientType>
bool MonomialUniversalEnveloping<CoefficientType>::HWTAAbilinearForm
(const MonomialUniversalEnveloping<CoefficientType>&right, CoefficientType& output,
 const Vector<CoefficientType>* subHiGoesToIthElement, GlobalVariables& theGlobalVariables,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero, std::stringstream* logStream)
{ ElementUniversalEnveloping<CoefficientType> tempElt1, tempElt2;
  tempElt1.MakeZero(*this->owner);
  tempElt1.AddMonomial(*this, theRingUnit);
  tempElt2.MakeZero(*this->owner);
  tempElt2.AddMonomial(right, theRingUnit);
  return tempElt1.HWTAAbilinearForm
  (tempElt2, output, subHiGoesToIthElement, theGlobalVariables, theRingUnit, theRingZero, logStream);
}

template<class CoefficientType>
bool ElementUniversalEnveloping<CoefficientType>::ApplyTransposeAntiAutoOnMe()
{ MonomialUniversalEnveloping<CoefficientType> tempMon;
  ElementUniversalEnveloping<CoefficientType> result;
  result.MakeZero(*this->owneR);
  int numPosRoots=this->GetOwner().GetNumPosRoots();
  int theRank=this->GetOwner().GetRank();
  CoefficientType theCoeff;
  this->CheckNumCoeffsConsistency(__FILE__, __LINE__);
  for (int i=0; i<this->size; i++)
  { MonomialUniversalEnveloping<CoefficientType>& currentMon=this->TheObjects[i];
    theCoeff=this->theCoeffs[i];
    tempMon.owneR=currentMon.owneR;
    tempMon.Powers.size=0;
    tempMon.generatorsIndices.size=0;
//    for (int j=0; j<currentMon.Powers.size; j++)
    for (int j=currentMon.Powers.size-1; j>=0; j--)
    { int thePower;
      if (!currentMon.Powers[j].IsSmallInteger(&thePower))
        return false;
      int theGenerator=currentMon.generatorsIndices[j];
      if (theGenerator<numPosRoots)
        theGenerator=2*numPosRoots+theRank-1-theGenerator;
      else if (theGenerator>=numPosRoots +theRank)
        theGenerator=-theGenerator+2*numPosRoots+theRank-1;
      tempMon.MultiplyByGeneratorPowerOnTheRight(theGenerator, currentMon.Powers[j]);
//      if (thePower%2==1)
//        tempMon.Coefficient*=-1;
//      ;
    }
    result.AddMonomial(tempMon, theCoeff);
  }
  *this=result;
  return true;
}

template <class CoefficientType>
bool ElementUniversalEnveloping<CoefficientType>::HWTAAbilinearForm
(const ElementUniversalEnveloping<CoefficientType>& right, CoefficientType& output,
 const Vector<CoefficientType>* subHiGoesToIthElement, GlobalVariables& theGlobalVariables,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero, std::stringstream* logStream)const
{ output=theRingZero;
  CoefficientType tempCF;
  ElementUniversalEnveloping<CoefficientType> TAleft;
//  this->CheckNumCoeffsConsistency(__FILE__, __LINE__);
  TAleft=*this;
//  TAleft.CheckNumCoeffsConsistency(__FILE__, __LINE__);
//  std::cout << "<hr>TAleft: " << TAleft.ToString(&theGlobalVariables.theDefaultLieFormat);
//  std::cout << "<br>right:" << right.ToString(&theGlobalVariables.theDefaultLieFormat);
  if (!TAleft.ApplyTransposeAntiAutoOnMe())
    return false;
  ElementUniversalEnveloping<CoefficientType> Accum, intermediateAccum, tempElt, startingElt;
  List<int> oldOrder=this->GetOwner().UEGeneratorOrderIncludingCartanElts;
  int numPosRoots=this->GetOwner().GetNumPosRoots();
  for (int i=0; i<numPosRoots; i++)
    this->GetOwner().UEGeneratorOrderIncludingCartanElts[i]=-1;
  Accum.MakeZero(this->GetOwner());
  MonomialUniversalEnveloping<CoefficientType> constMon;
  constMon.MakeConst(this->GetOwner());
//    std::cout << "<hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr>"
//  << "hw: " << subHiGoesToIthElement->ToString()
//  << "<hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr>"
 // << "(" << this->ElementToStringCalculatorFormat(theGlobalVariables, tempFormat)
 // << "," << right.ElementToStringCalculatorFormat(theGlobalVariables, tempFormat) << ")";

  if (logStream!=0)
  { *logStream << "left eltement transposed: " << TAleft.ToString(&theGlobalVariables.theDefaultFormat) << "<br>";
    *logStream << "right element: " << right.ToString(&theGlobalVariables.theDefaultFormat) << "<br>";
  }
  startingElt=right;
  startingElt.Simplify(theGlobalVariables, theRingUnit, theRingZero);
  if (logStream!=0)
    *logStream << "right element after simplification: "
    << startingElt.ToString(&theGlobalVariables.theDefaultFormat) << "<br>";
  startingElt.ModOutVermaRelations(&theGlobalVariables, subHiGoesToIthElement, theRingUnit, theRingZero);
  if (logStream!=0)
    *logStream << "right element after Verma rels: "
    << startingElt.ToString(&theGlobalVariables.theDefaultFormat) << "<br>";
  CoefficientType leftMonCoeff;
  for (int j=0; j<TAleft.size; j++)
  { intermediateAccum=startingElt;
    const MonomialUniversalEnveloping<CoefficientType>& leftMon=TAleft[j];
    leftMonCoeff=TAleft.theCoeffs[j];
    int thePower;
    for (int i=leftMon.Powers.size-1; i>=0; i--)
      if (leftMon.Powers[i].IsSmallInteger(&thePower))
        for (int k=0; k<thePower; k++)
        { tempElt.MakeOneGenerator(leftMon.generatorsIndices[i], this->GetOwner(), theRingUnit);
          MathRoutines::swap(tempElt, intermediateAccum);
          if (logStream!=0)
          { //*logStream << "tempElt before mult: " << tempElt.ToString(theGlobalVariables, tempFormat) << "<br>";
            *logStream << "intermediate before mult: " << intermediateAccum.ToString(&theGlobalVariables.theDefaultFormat) << "<br>";
          }
          intermediateAccum*=(tempElt);
          if (logStream!=0)
            *logStream << "intermediate before simplification: " << intermediateAccum.ToString(&theGlobalVariables.theDefaultFormat) << "<br>";
          intermediateAccum.Simplify(theGlobalVariables, theRingUnit, theRingZero);
          if (logStream!=0)
            *logStream << "intermediate after simplification: " << intermediateAccum.ToString(&theGlobalVariables.theDefaultFormat) << "<br>";
          intermediateAccum.ModOutVermaRelations(&theGlobalVariables, subHiGoesToIthElement, theRingUnit, theRingZero);
          if (logStream!=0)
            *logStream << "intermediate after Verma rels: " << intermediateAccum.ToString(&theGlobalVariables.theDefaultFormat) << "<br>";
        }
      else
      { this->GetOwner().UEGeneratorOrderIncludingCartanElts=oldOrder;
        return false;
      }
    intermediateAccum*=leftMonCoeff;
    Accum+=intermediateAccum;
    int theIndex= intermediateAccum.GetIndex(constMon);
    if (theIndex!=-1)
      output+=intermediateAccum.theCoeffs[theIndex];
  }
  if (logStream!=0)
    *logStream << "final UE element: " << Accum.ToString(&theGlobalVariables.theDefaultFormat);
  this->GetOwner().UEGeneratorOrderIncludingCartanElts=oldOrder;
//  if (logStream!=0)
//    std::cout << "<hr>" << logStream->str();
  return true;
}

template <class CoefficientType>
void MonomialUniversalEnveloping<CoefficientType>::ModOutVermaRelations
(CoefficientType& outputCoeff, GlobalVariables* theContext, const Vector<CoefficientType>* subHiGoesToIthElement,
 const CoefficientType& theRingUnit, const CoefficientType& theRingZero)
{ int numPosRoots=this->GetOwner().GetNumPosRoots();
  int theDimension=this->GetOwner().GetRank();
  outputCoeff=theRingUnit;
//  if (subHiGoesToIthElement!=0)
//    std::cout << "<br>The subHiGoesToIthElement: " << subHiGoesToIthElement->ToString();
  for (int i=this->generatorsIndices.size-1; i>=0; i--)
  { int IndexCurrentGenerator=this->generatorsIndices[i];
    if (IndexCurrentGenerator>=numPosRoots+theDimension)
    { this->MakeZero(theRingZero, *this->owneR);
      outputCoeff=theRingZero;
      return;
    }
    if (IndexCurrentGenerator<numPosRoots)
      return;
    if (IndexCurrentGenerator>=numPosRoots && IndexCurrentGenerator<numPosRoots+theDimension)
    { if (subHiGoesToIthElement==0)
      { this->MakeZero(theRingZero, *this->owneR);
        outputCoeff=theRingZero;
        return;
      }
      int theDegree;
      if (!this->Powers[i].IsSmallInteger(&theDegree))
        return;
      int hIndex=IndexCurrentGenerator-numPosRoots;
      CoefficientType theSubbedH;
      theSubbedH=(*subHiGoesToIthElement)[hIndex];
      MathRoutines::RaiseToPower(theSubbedH, theDegree, theRingUnit);
      outputCoeff*=(theSubbedH);
//      std::cout << "&nbsp outputCoeff=" << outputCoeff.ToString();
      //assert(this->Coefficient.checkConsistency());
      this->generatorsIndices.size--;
      this->Powers.size--;
    }
  }
}

template <class CoefficientType>
void WeylGroup::RaiseToDominantWeight
(Vector<CoefficientType>& theWeight, int* sign, bool* stabilizerFound, ElementWeylGroup* raisingElt)
{ if (sign!=0)
    *sign=1;
  if (stabilizerFound!=0)
    *stabilizerFound=false;
  CoefficientType theScalarProd;
  int theDim=this->GetDim();
  if (raisingElt!=0)
    raisingElt->SetSize(0);
  for (bool found = true; found; )
  { found=false;
    for (int i=0; i<theDim; i++)
    { theScalarProd=this->GetScalarProdSimpleRoot(theWeight, i);
      if (theScalarProd.IsNegative())
      { found=true;
        theScalarProd*=2;
        theScalarProd/=this->CartanSymmetric.elements[i][i];
        theWeight[i]-=theScalarProd;
        if (sign!=0)
          *sign*=-1;
        if (raisingElt!=0)
          raisingElt->AddOnTop(i);
      }
      if (stabilizerFound!=0)
        if (theScalarProd.IsEqualToZero())
          *stabilizerFound=true;
    }
  }
//  std::cout << "<hr># simple reflections applied total: " << numTimesReflectionWasApplied;
}


template<class Element>
bool Matrix<Element>::IsPositiveDefinite()
{ if (this->NumRows!=this->NumCols)
  { std::cout << "This is a programming error: attempting to evaluate whether a matrix is positive definite, but"
    << " the matrix is not square. " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  Element det;
  Matrix<Element> tempMat;
  for (int i=0; i< this->NumRows; i++)
  { tempMat.init(i+1, i+1);
    for (int j=0; j<tempMat.NumCols; j++)
      for (int k=0; k<tempMat.NumCols; k++)
        tempMat.elements[j][k]=this->elements[j][k];
    det=tempMat.GetDeterminant();
    if (det<=0)
      return false;
  }
  return true;
}

template<class CoefficientType>
bool ModuleSSalgebra<CoefficientType>::MakeFromHW
(SemisimpleLieAlgebra& inputAlgebra, Vector<CoefficientType>& HWFundCoords,
 const Selection& selNonSelectedAreElementsLevi, GlobalVariables& theGlobalVariables,
const CoefficientType& theRingUnit, const CoefficientType& theRingZero,
 std::string* outputReport, bool computeSimpleGens)
{ MacroRegisterFunctionWithName("ModuleSSalgebra<CoefficientType>::MakeFromHW");
  ProgressReport theReport(&theGlobalVariables);
  this->owneR=&inputAlgebra;
  SemisimpleLieAlgebra& theAlgebrA=inputAlgebra;

  int theRank=theAlgebrA.GetRank();
  if (HWFundCoords.size!=theRank || selNonSelectedAreElementsLevi.MaxSize!=theRank)
  { std::cout << "This is a programming error. I am asked to create a generalized Verma module "
    << " with a semisimple Lie algebra of rank " << theRank << " but the input highest weight, "
    << HWFundCoords.ToString() << ", has " << HWFundCoords.size << " coordinates and "
    << " the parabolic section indicates rank of " << selNonSelectedAreElementsLevi.MaxSize
    << ". " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  WeylGroup& theWeyl=theAlgebrA.theWeyl;
  this->cachedPairs.Clear();
  this->cachedTraces.SetSize(0);

  this->parabolicSelectionNonSelectedAreElementsLevi=selNonSelectedAreElementsLevi;
  this->parabolicSelectionSelectedAreElementsLevi=this->parabolicSelectionNonSelectedAreElementsLevi;
  this->parabolicSelectionSelectedAreElementsLevi.InvertSelection();

  this->theHWFundamentalCoordsBaseField=HWFundCoords;
  this->theHWDualCoordsBaseFielD.SetSize(theRank);
  this->theHWFDpartFundamentalCoordS.SetSize(theRank);

  for (int i=0; i<theRank; i++)
  { this->theHWFDpartFundamentalCoordS[i]=0;
    if (this->parabolicSelectionSelectedAreElementsLevi.selected[i])
    { int theCoord;
      if (!this->theHWFundamentalCoordsBaseField[i].IsSmallInteger(&theCoord))
      { if (outputReport!=0)
          *outputReport="The given module over the Levi part is not finite dimensional";
        return false;
      }
      this->theHWFDpartFundamentalCoordS[i]=theCoord;
    }
    this->theHWDualCoordsBaseFielD[i]=this->theHWFundamentalCoordsBaseField[i];
    this->theHWDualCoordsBaseFielD[i]*=theWeyl.CartanSymmetric.elements[i][i]/2;
  }

  this->theHWFDpartSimpleCoordS=theWeyl.GetSimpleCoordinatesFromFundamental
  (this->theHWFDpartFundamentalCoordS);
  this->theHWFDpartDualCoords= theWeyl.GetDualCoordinatesFromFundamental
  (this->theHWFDpartFundamentalCoordS);
  this->theHWSimpleCoordSBaseField=theWeyl.GetSimpleCoordinatesFromFundamental
  (this->theHWFundamentalCoordsBaseField);
  this->theChaR.MakeFromWeight(this->theHWSimpleCoordSBaseField, this->owneR);

 // std::cout << "<br>input fund coords base field: " << HWFundCoords.ToString();
 // std::cout << "<br>dual coords no base field: " << this->theHWDualCoordS.ToString();
 // std::cout << "<br>dual coords: " << this->theHWDualCoordsBaseField.ToString();
 // std::cout << "<br>fund coords no base field: " << this->theHWFDpartFundamentalCoordS.ToString();
 // std::cout << "<br>fund coords: " << this->theHWFundamentalCoordsBaseField.ToString();
 // std::cout << "<br>simple coords no base field: " << this->theHWFDpartSimpleCoordS.ToString();
 // std::cout << "<br>parabolicSelectionNonSelectedAreElementsLevi: " << this->parabolicSelectionNonSelectedAreElementsLevi.ToString();
 // std::cout << "<br>parabolicSelectionSelectedAreElementsLevi: " << this->parabolicSelectionSelectedAreElementsLevi.ToString();
  int startingNumRationalOperations
  =Rational::TotalLargeAdditions+Rational::TotalSmallAdditions
  +Rational::TotalLargeMultiplications+Rational::TotalSmallMultiplications;
  LittelmannPath startingPath;
  startingPath.MakeFromWeightInSimpleCoords(this->theHWFDpartSimpleCoordS, theWeyl);
//  std::cout << "<br>starting littelmann path: " << startingPath.ToString() << this->parabolicSelectionNonSelectedAreElementsLevi.ToString();
  List<List<int> > generatorsIndices;
  if (!startingPath.GenerateOrbit
      (this->thePaths, generatorsIndices, theGlobalVariables, 1000, & this->parabolicSelectionNonSelectedAreElementsLevi))
  { if (outputReport!=0)
      *outputReport = "Error: number of Littelmann paths exceeded allowed limit of 1000.";
    return false;
  }
  this->theModuleWeightsSimpleCoords.Clear();
  MonomialChar<CoefficientType> tempCharMon;
  this->theCharOverH.Reset();
  for (int i=0; i<this->thePaths.size; i++)
  { this->theModuleWeightsSimpleCoords.AddOnTopNoRepetition(*this->thePaths[i].Waypoints.LastObject());
    tempCharMon.weightFundamentalCoords= theWeyl.GetFundamentalCoordinatesFromSimple(*this->thePaths[i].Waypoints.LastObject());
    this->theCharOverH.AddMonomial(tempCharMon,1);
  }
//  std::cout << "<br>character over h (i.e. the set of weights with mults): " << this->theCharOverH.ToString();
//  std::cout << "<br>character: " << this->theChaR.ToString();
  this->theModuleWeightsSimpleCoords.QuickSortAscending();
  std::stringstream out2;
  ElementUniversalEnveloping<CoefficientType> tempElt;
  this->theGeneratingWordsGrouppedByWeight.SetSize(this->theModuleWeightsSimpleCoords.size);
  this->theGeneratingWordsIntGrouppedByWeight.SetSize(this->theModuleWeightsSimpleCoords.size);
  for (int i=0; i<this->theGeneratingWordsGrouppedByWeight.size; i++)
  { this->theGeneratingWordsGrouppedByWeight[i].size=0;
    this->theGeneratingWordsIntGrouppedByWeight[i].size=0;
  }
  MonomialUniversalEnveloping<CoefficientType> currentNonReducedElement;
  MonomialTensor<int, MathRoutines::IntUnsignIdentity> tempMonInt;
  for (int i=0; i<this->thePaths.size; i++)
  { List<int>& currentPath= generatorsIndices[i];
    currentNonReducedElement.MakeConst(this->GetOwner());
    tempMonInt.MakeConst();
    for (int j=currentPath.size-1; j>=0; j--)
    { int theIndex=currentPath[j];
      if (theIndex>0)
        theIndex++;
      currentNonReducedElement.MultiplyByGeneratorPowerOnTheRight
      (this->GetOwner().GetGeneratorFromDisplayIndex(theIndex), theRingUnit);
      tempMonInt.MultiplyByGeneratorPowerOnTheRight
      (this->GetOwner().GetGeneratorFromDisplayIndex(theIndex), 1);
    }
    Vector<Rational>& hwCurrent=*this->thePaths[i].Waypoints.LastObject();
    int theIndex=this->theModuleWeightsSimpleCoords.GetIndex(hwCurrent);
    if (theIndex==-1)
    { //std::cout << "couldn't find weight : " << hwCurrent.ToString() << " in " << this->theModuleWeightsSimpleCoords.ToString();
      out2 << "Error: could not generate all weights in the weight support. Maybe they are too many? Allowed "
      << " # of weights is 10000";
      if (outputReport!=0)
        *outputReport=out2.str();
      return false;
    }
    this->theGeneratingWordsGrouppedByWeight[theIndex].AddOnTop(currentNonReducedElement);
    this->theGeneratingWordsIntGrouppedByWeight[theIndex].AddOnTop(tempMonInt);
  }
  this->theGeneratingWordsNonReduced.Clear();
  this->theGeneratingWordsNonReduced.SetExpectedSize(this->thePaths.size);
  this->theGeneratingWordsNonReducedInt.Clear();
  this->theGeneratingWordsNonReducedInt.SetExpectedSize(this->thePaths.size);
  this->theGeneratingWordsNonReduced.size=0;
  this->theGeneratingWordsWeightsPlusWeightFDpart.SetSize(0);
  this->theGeneratingWordsWeightsPlusWeightFDpart.ReservE(this->thePaths.size);
  HashedList<ElementWeylGroup> theWeylElts;
//  int wordCounter=-1;
  for (int i=0; i<this->theGeneratingWordsGrouppedByWeight.size; i++)
  { List<MonomialUniversalEnveloping<CoefficientType> >& currentList=
    this->theGeneratingWordsGrouppedByWeight[i];
    List<MonomialTensor<int, MathRoutines::IntUnsignIdentity> >& currentListInt=
    this->theGeneratingWordsIntGrouppedByWeight[i];
    currentList.QuickSortDescending();
    currentListInt.QuickSortDescending();
    for (int j=0; j<currentList.size; j++)
    { //wordCounter++;
      this->theGeneratingWordsNonReduced.AddOnTop(currentList[j]);
      this->theGeneratingWordsNonReducedInt.AddOnTop(currentListInt[j]);
      this->theGeneratingWordsWeightsPlusWeightFDpart.AddOnTop(this->theModuleWeightsSimpleCoords[i]);
    }
  }
  this->IntermediateStepForMakeFromHW
  (this->theHWDualCoordsBaseFielD, theGlobalVariables, theRingUnit, theRingZero);
  this->NumCachedPairsBeforeSimpleGen=this->cachedPairs.size;
  this->NumRationalMultiplicationsAndAdditionsBeforeSimpleGen
  =Rational::TotalLargeAdditions+Rational::TotalSmallAdditions
  +Rational::TotalLargeMultiplications+Rational::TotalSmallMultiplications
  -startingNumRationalOperations;
  bool isBad=false;
  for (int k=0; k<this->theBilinearFormsAtEachWeightLevel.size; k++)
  { Matrix<CoefficientType>& theBF=this->theBilinearFormsAtEachWeightLevel[k];
    Matrix<CoefficientType>& theBFinverted= this->theBilinearFormsInverted[k];
    if (!theBF.IsNonNegativeAllEntries())
      this->flagConjectureBholds=false;
    if (theBFinverted.NumRows<=0)
      isBad=true;
  }
  if (isBad)
  { if(outputReport!=0)
    { out2 << "<br>Error: the Littelmann-path induced monomials do not give a monomial basis. ";
      *outputReport=out2.str();
    }
    return false;
  }
  ElementSemisimpleLieAlgebra<Rational> tempSSElt;
  if (computeSimpleGens)
    for (int k=0; k<2; k++)
      for (int j=0; j<this->GetOwner().GetRank(); j++)
        if (this->parabolicSelectionSelectedAreElementsLevi.selected[j])
        { int theIndex=this->GetOwner().GetNumPosRoots()-j-1;
          if (k==1)
            theIndex=this->GetOwner().GetNumPosRoots()+this->GetOwner().GetRank()+j;
          tempSSElt.MakeGenerator(theIndex, this->GetOwner());
          if (outputReport!=0)
            out2 << "<hr>Simple generator: " << tempSSElt.ToString(&theGlobalVariables.theDefaultFormat);
          MatrixTensor<CoefficientType>& theMatrix
          =this->GetActionGeneratorIndeX(theIndex, theGlobalVariables, theRingUnit, theRingZero);
          std::stringstream tempStream;
          tempStream << "computing action simple generator index " << (2*k-1)*(j+1) << " ... ";
          theReport.Report(tempStream.str());
          tempStream << " done!";
          theReport.Report(tempStream.str());
          if (outputReport!=0)
          { if (this->GetDim() <50)
              out2 << "<br>Matrix of elemenent in the m_i basis:<br>"
              << CGI::GetHtmlMathSpanFromLatexFormula(theMatrix.ToString());
            else
              out2 << "<br>Matrix of elemenent in the m_i basis:<br>"
              << theMatrix.ToString();
          }
      /*    for (int j=0; j<this->actionsSimpleGens[i].size; j++)
            for (int k=0; k<this->actionsSimpleGens[i][j].size; k++)
            { out << "<br>" << theSimpleGenerator.ToString(theGlobalVariables, tempFormat) << "\\cdot "
              << this->theGeneratingWordsGrouppedByWeight[j][k].ToString(false, false, theGlobalVariables, tempFormat)
              << "\\cdot v=" << this->actionsSimpleGens[i][j][k].ToString(theGlobalVariables, tempFormat)
              << "\\cdot v"
              ;
            }*/
          if (k==1)
            this->GetActionGeneratorIndeX
            (this->GetOwner().GetNumPosRoots()+j, theGlobalVariables, theRingUnit, theRingZero);
        }
  if (outputReport!=0)
    *outputReport= out2.str();
  this->flagIsInitialized=true;
//  std::cout << "<hr>MakeHW result: <br>" << this->ToString();
//  this->TestConsistency(theGlobalVariables);
  theReport.Report("Done with module generation");
  return true;
}

template<class CoefficientType>
void ModuleSSalgebra<CoefficientType>::IntermediateStepForMakeFromHW
  (Vector<CoefficientType>& HWDualCoordS, GlobalVariables& theGlobalVariables,
    const CoefficientType& theRingUnit, const CoefficientType& theRingZero
   )
{ MacroRegisterFunctionWithName("ModuleSSalgebra<CoefficientType>::IntermediateStepForMakeFromHW");
  ProgressReport theReport(&theGlobalVariables);
  ProgressReport theReport2(&theGlobalVariables);
  Vector<Rational> targetWeight;
  this->theBilinearFormsAtEachWeightLevel.SetSize(this->theGeneratingWordsGrouppedByWeight.size);
  this->theBilinearFormsInverted.SetSize(this->theGeneratingWordsGrouppedByWeight.size);
  this->ComputedGeneratorActions.init(this->GetOwner().GetNumGenerators());
  this->actionsGeneratorS.SetSize(this->GetOwner().GetNumGenerators());
  this->actionsGeneratorsMaT.SetSize(this->GetOwner().GetNumGenerators());
  int numScalarProducts=0;
  this->flagConjectureBholds=true;
  this->flagConjectureCholds=true;
  for (int l=0; l<this->theGeneratingWordsGrouppedByWeight.size; l++)
  { Matrix<CoefficientType>& currentBF=this->theBilinearFormsAtEachWeightLevel[l];
    List<MonomialUniversalEnveloping<CoefficientType> >& currentWordList=
    this->theGeneratingWordsGrouppedByWeight[l];
    List<MonomialTensor<int, MathRoutines::IntUnsignIdentity> >& currentWordListInt=
    this->theGeneratingWordsIntGrouppedByWeight[l];
    currentBF.init(currentWordList.size, currentWordList.size);
    for (int i=0; i<currentWordList.size; i++)
      for (int j=i; j<currentWordList.size; j++)
      { //std::cout << "<br>word " << i+1 << ": " << currentWordList[i].ToString(&theGlobalVariables.theDefaultLieFormat);
        //std::cout << "<br>word " << j+1 << ": " << currentWordList[j].ToString(&theGlobalVariables.theDefaultLieFormat);
        std::stringstream tempStream;
        tempStream << " Computing Shapovalov form layer " << l << " out of "
        << this->theGeneratingWordsGrouppedByWeight.size
        << " between indices " << i +1 << " and " << j+1 << " out of " << currentWordList.size;
        //currentWordList[i].HWTAAbilinearForm
        //(currentWordList[j], currentBF.elements[i][j], &HWDualCoordS, theGlobalVariables, theRingUnit, theRingZero, 0)
        //;
        numScalarProducts++;
        currentBF.elements[i][j]=
        this->hwtaabfSimpleGensOnly(currentWordListInt[i], currentWordListInt[j], &theReport2);
//        std::cout << "<br> (" << currentWordList[i].ToString()
//        << ", " << currentWordList[j].ToString() << ")=  Old: " << currentBF.elements[i][j].ToString()
//        << "&nbsp    New: " << tempI.ToString();
        theReport.Report(tempStream.str());
//        std::cout << "[" << currentWordList[i].ToString() << ", " << currentWordList[j].ToString() << "]=" <<currentBF.elements[i][j].ToString();
        if (i!=j)
          currentBF.elements[j][i]=currentBF.elements[i][j];
      }
    Matrix<CoefficientType> tempMat;
    tempMat=(currentBF);
    CoefficientType tempRat;
    tempMat.ComputeDeterminantOverwriteMatrix(tempRat, theRingUnit, theRingZero);
    if (!tempRat.IsEqualToZero())
    { this->theBilinearFormsInverted[l]=currentBF;
      this->theBilinearFormsInverted[l].Invert(theRingUnit, theRingZero);
//      if (!currentBF.IsPositiveDefinite())
//        this->flagConjectureCholds=false;
      if (!currentBF.IsNonNegativeAllEntries())
        this->flagConjectureBholds=false;
    } else
      this->theBilinearFormsInverted[l].init(0,0);
  }
//  for (int i=0; i<this->cachedPairs.size; i++)
//  { std::cout << "<br>(" << this->cachedPairs[i].Object1 << ", " << this->cachedPairs[i].Object2
//    << ") = " << this->cachedTraces[i];
//  }
}

template <class CoefficientType>
template <class ResultType>
void ModuleSSalgebra<CoefficientType>::GetElementsNilradical
(List<ElementUniversalEnveloping<ResultType> >& output, bool useNegativeNilradical,
 List<int>* listOfGenerators)
{ SemisimpleLieAlgebra& ownerSS=this->GetOwner();
  ownerSS.OrderSetNilradicalNegativeMost(this->parabolicSelectionNonSelectedAreElementsLevi);
  ElementUniversalEnveloping<ResultType> theElt;
  output.SetSize(0);
  output.ReservE(ownerSS.GetNumPosRoots());

  int theBeginning=useNegativeNilradical ? 0: ownerSS.GetNumPosRoots()+ownerSS.GetRank();
  MemorySaving<List<int> > tempList;
  if (listOfGenerators==0)
    listOfGenerators=&tempList.GetElement();
  listOfGenerators->SetSize(0);
  listOfGenerators->ReservE(ownerSS.GetNumPosRoots());
  for (int i=theBeginning; i<theBeginning+ownerSS.GetNumPosRoots(); i++)
    if (this->IsNotInLevi(i))
      listOfGenerators->AddOnTop(i);
  //bubble sort:
  for (int i=0; i<listOfGenerators->size; i++)
    for (int j=i+1; j<listOfGenerators->size; j++)
      if (ownerSS.UEGeneratorOrderIncludingCartanElts[listOfGenerators->TheObjects[i]]>
          ownerSS.UEGeneratorOrderIncludingCartanElts[listOfGenerators->TheObjects[j]])
        listOfGenerators->SwapTwoIndices(i, j);
  for (int i=0; i<listOfGenerators->size; i++)
  { theElt.MakeOneGeneratorCoeffOne(listOfGenerators->TheObjects[i], *this->owneR);
    output.AddOnTop(theElt);
  }
}

template <class CoefficientType>
void ElementSemisimpleLieAlgebra<CoefficientType>::MakeGenerator
  (int generatorIndex, SemisimpleLieAlgebra& inputOwner)
{ //Changing RootSystem order invalidates this function!
  if (&inputOwner==0)
  { std::cout << " This is a programming error: 0 pointer to Semisimple Lie algebra. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  this->MakeZero(inputOwner);
  ChevalleyGenerator tempGenerator;
  tempGenerator.MakeGenerator(inputOwner, generatorIndex);
  this->AddMonomial(tempGenerator, 1);
}

template <class CoefficientType>
void ElementSemisimpleLieAlgebra<CoefficientType>::MakeZero(const SemisimpleLieAlgebra& inputOwner)
{ this->::MonomialCollection<ChevalleyGenerator, CoefficientType>::MakeZero();
}

template <class CoefficientType>
void SemisimpleLieAlgebra::LieBracket
(const ElementSemisimpleLieAlgebra<CoefficientType>& g1,
 const ElementSemisimpleLieAlgebra<CoefficientType>& g2,
 ElementSemisimpleLieAlgebra<CoefficientType>& output)
{ if (&output==&g1 || &output==&g2)
  { ElementSemisimpleLieAlgebra<CoefficientType> outputNew;
    this->LieBracket(g1, g2, outputNew);
    output=outputNew;
    return;
  }
  output.MakeZero(*this);
  if (g1.IsEqualToZero() || g2.IsEqualToZero())
    return;
  int maxNumMonsFinal=g1.size*g2.size;
  output.SetExpectedSize(maxNumMonsFinal);
  CoefficientType theCoeff;
  ElementSemisimpleLieAlgebra<CoefficientType> buffer;
  for (int i=0; i<g1.size; i++)
    for (int j=0; j<g2.size; j++)
    { buffer=this->theLiebrackets.elements[g1[i].theGeneratorIndex][g2[j].theGeneratorIndex];
      theCoeff=g1.theCoeffs[i];
      theCoeff*=g2.theCoeffs[j];
      buffer*=theCoeff;
      output+=buffer;
    }
}

template <class CoefficientType>
void ElementSemisimpleLieAlgebra<CoefficientType>::ActOnMe
(const ElementSemisimpleLieAlgebra<CoefficientType>& theElt,
 ElementSemisimpleLieAlgebra<CoefficientType>& output, SemisimpleLieAlgebra& owner)
{ owner.LieBracket(theElt, *this, output);
}

template <class CoefficientType>
void ElementSemisimpleLieAlgebra<CoefficientType>::ActOnMe
(const ElementSemisimpleLieAlgebra<CoefficientType>& theElt,
 ElementSemisimpleLieAlgebra<CoefficientType>& output, SemisimpleLieAlgebra& owner,
 const RationalFunctionOld& theRingUnit, const RationalFunctionOld& theRingZero,
 GlobalVariables* theGlobalVariables)
{ owner.LieBracket(theElt, *this, output);
}

template <class CoefficientType>
Vector<CoefficientType> ElementSemisimpleLieAlgebra<CoefficientType>::GetCartanPart()const
{ Vector<CoefficientType> result;
  if (this->IsEqualToZero())
  { result.MakeZero(0);
    return result;
  }
  ChevalleyGenerator tempGen;
  SemisimpleLieAlgebra* owner=this->GetOwner();
  int theRank=owner->GetRank();
  int numPosRoots=owner->GetNumPosRoots();
  result.MakeZero(theRank);
  for (int i=0; i<theRank; i++)
  { tempGen.MakeGenerator(*owner, i+numPosRoots);
    int currentIndex=this->GetIndex(tempGen);
    if (currentIndex!=-1)
      result[i]+=this->theCoeffs[currentIndex];
  }
  return result;
}

template <class CoefficientType>
void ElementSemisimpleLieAlgebra<CoefficientType>::MakeHgenerator
(const Vector<CoefficientType>& theH, SemisimpleLieAlgebra& inputOwner)
{ ChevalleyGenerator tempGen;
  this->MakeZero(inputOwner);
  for (int i=0; i<theH.size; i++)
  { tempGen.MakeGenerator(inputOwner, inputOwner.GetCartanIndexFromGenerator(i));
    this->AddMonomial(tempGen, theH[i]);
  }
}

template <class CoefficientType>
void charSSAlgMod<CoefficientType>::MakeFromWeight
(const Vector<CoefficientType>& inputWeightSimpleCoords, SemisimpleLieAlgebra* inputOwner)
{ this->MakeZero(inputOwner);
  if (inputWeightSimpleCoords.size!=this->GetOwner().GetRank())
  { std::cout << "This is a programming error: attempting to create a character "
    << " from highest weight in simple coords "
    << inputWeightSimpleCoords.ToString() << "(" << inputWeightSimpleCoords.size
    << " coordinates) while the owner semisimple "
    << " Lie algebra is of rank " << this->GetOwner().GetRank()
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  MonomialChar<CoefficientType> theMon;
  theMon.weightFundamentalCoords=
  this->owner->theWeyl.GetFundamentalCoordinatesFromSimple(inputWeightSimpleCoords);
  this->AddMonomial(theMon, 1);
}

template<class CoefficientType>
bool ElementSemisimpleLieAlgebra<CoefficientType>::IsElementCartan()const
{ if (this->size==0)
    return true;
  SemisimpleLieAlgebra* owner= (*this)[0].owneR;
  for (int i=0; i<this->size; i++)
    if (!owner->IsGeneratorFromCartan((*this)[i].theGeneratorIndex))
      return false;
  return true;
}

template <typename coefficient>
unsigned int WeylGroupRepresentation<coefficient>::HashFunction()const
{ unsigned int result= this->OwnerGroup==0? 0 : this->OwnerGroup->HashFunction();
  result+=this->theCharacter.HashFunction();
  return result;
}

template <typename coefficient>
std::string WeylGroupRepresentation<coefficient>::ToString(FormatExpressions* theFormat)const
{ if (this->OwnerGroup==0)
    return "non-initialized representation";
  std::stringstream out;
  if (this->theCharacter.size>0)
    out << "Character: " << this->theCharacter.ToString(theFormat) << " of length "
    << this->OwnerGroup->GetCharacterNorm(this->theCharacter);
  else
    out << "Character needs to be computed.";
  int theRank=this->OwnerGroup->GetDim();
  LargeIntUnsigned theLCM, theDen;
  this->GetLargestDenominatorSimpleGens(theLCM, theDen);
  out << "\n<br>\n LCM denominators simple generators: "
  << theLCM.ToString() << ", largest denominator: " << theDen.ToString();
  out << "\n<br>\nThe simple generators (" << theRank << " total):<br> ";
  std::stringstream forYourCopyConvenience;
  for (int i=1; i<theRank+1; i++)
    if (this->theElementIsComputed[i])
    { std::stringstream tempStream;
      tempStream << "s_" << i << ":=MatrixRationals{}"
      << this->theElementImages[i].ToString(theFormat) << "; \\\\\n";
      forYourCopyConvenience << tempStream.str();
      out << CGI::GetHtmlMathSpanPure("\\begin{array}{l}"+ tempStream.str()+"\\end{array}", 3000);
    } else
      out << "Element s_{" << i << "} not computed ";
  out << "<br>For your copy convenience: <br>" << forYourCopyConvenience.str();
  return out.str();
}

template <typename coefficient>
coefficient WeylGroup::GetHermitianProduct
  (const Vector<coefficient>& leftCharacter, const Vector<coefficient>& rightCharacter)const
{ this->CheckInitializationFDrepComputation();
  coefficient result = 0;
  for(int i=0; i<this->conjugacyClasses.size; i++)
    result +=
    leftCharacter[i].GetComplexConjugate() * rightCharacter[i].GetComplexConjugate() *
    this->conjugacyClasses[i].size;
  result/=this->theElements.size;
  return result;
}
#endif
