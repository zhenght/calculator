//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#ifndef vpfHeader1_4_h_already_included
#define vpfHeader1_4_h_already_included

#include "vpfHeader1_2.h"
static ProjectInformationInstance ProjectInfoVpfHeader1_4
(__FILE__, "Header, math routines concerning semisimple Lie algebras. ");

class SltwoSubalgebras : public HashedList<slTwo>
{
  friend class SemisimpleSubalgebras;
public:
//  List< int > hSingularWeights;
  List<int> MultiplicitiesFixedHweight;
  List<List<int> > IndicesSl2sContainedInRootSA;
  List<int> IndicesSl2decompositionFlas;
  Vectors<Rational> BadHCharacteristics;
  int IndexZeroWeight;
  List<SemisimpleLieAlgebra>* owners;
  int IndexInOwners;
  rootSubalgebras theRootSAs;
  List<std::string> texFileNamesForPNG;
  List<std::string> texStringsEachFile;
  List<std::string> listSystemCommandsLatex;
  List<std::string> listSystemCommandsDVIPNG;
  void CheckForCorrectInitializationCrashIfNot()
  { if (this->owners==0 || this->IndexInOwners<0)
    { std::cout << "This is a programming error. "
      << " Object SltwoSubalgebras is not initialized, although it is supposed to be. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
  }
  WeylGroup& GetOwnerWeyl()
  { return this->GetOwner().theWeyl;
  }
  SemisimpleLieAlgebra& GetOwner()
  { this->CheckForCorrectInitializationCrashIfNot();
    return (*this->owners)[this->IndexInOwners];
  }
  void ComputeModuleDecompositionsOfAmbientLieAlgebra(GlobalVariables& theGlobalVariables)
  { for(int i=0; i<this->size; i++)
      this->TheObjects[i].ComputeModuleDecompositionAmbientLieAlgebra(theGlobalVariables);
  }
  void init(List<SemisimpleLieAlgebra>* inputOwners, int inputIndexInOwner);
  SltwoSubalgebras(): owners(0), IndexInOwners(-1)
  {}
  SltwoSubalgebras(List<SemisimpleLieAlgebra>* inputOwners, int inputIndexInOwner)
  : owners(inputOwners), IndexInOwners(inputIndexInOwner)
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
  void getZuckermansArrayE8(Vectors<Rational>& output);
  void MakeProgressReport(int index, int outOf, GlobalVariables& theGlobalVariables);
  void ComputeDebugStringCurrent();
  bool ContainsSl2WithGivenH(Vector<Rational>& theH, int* outputIndex);
  bool ContainsSl2WithGivenHCharacteristic(Vector<Rational> & theHCharacteristic, int* outputIndex);
  void ElementToHtml
  (FormatExpressions* theFormat=0, GlobalVariables* theGlobalVariables=0)
   ;
  std::string ElementToStringNoGenerators(FormatExpressions* theFormat=0);
  void ElementToStringModuleDecompositionMinimalContainingRegularSAs(std::string& output, bool useLatex, bool useHtml);
  std::string ToString(FormatExpressions* theFormat=0);
};

class DynkinSimpleType
{ friend std::ostream& operator << (std::ostream& output, const DynkinSimpleType& theMon)
  { output << theMon.ToString();
    return output;
  }
  public:
  char theLetter;
  int theRank;
  DynkinSimpleType():theLetter('X'), theRank(-1){}
  bool operator>(const DynkinSimpleType& other)const
  { if (this->theRank>other.theRank)
      return true;
    if (this->theRank<other.theRank)
      return false;
    if (this->theLetter=='B' && other.theLetter=='D')
      return true;
    if (this->theLetter=='D' && other.theLetter=='B')
      return false;
    return this->theLetter>other.theLetter;
  }
  void operator=(const DynkinSimpleType& other)
  { this->theLetter=other.theLetter;
    this->theRank=other.theRank;
  }
  bool operator==(const DynkinSimpleType& other)const
  { return this->theLetter==other.theLetter && this->theRank==other.theRank;
  }
  static unsigned int HashFunction(const DynkinSimpleType& input)
  { return ((unsigned int) input.theLetter)*2+input.theRank;
  }
  unsigned int HashFunction()const
  { return this->HashFunction(*this);
  }
  inline bool IsEqualToZero()const
  { return false;
  }
  std::string ToString()const
  { std::stringstream out;
    if (theRank>=10)
      out << theLetter << "_{" << theRank << "}";
    else
      out << theLetter << "_" << theRank;
    return out.str();
  }
};

class DynkinType : public MonomialCollection<DynkinSimpleType, int>
{
public:
  void GetLettersTypesMults (List<char>& outputLetters, List<int>& outputRanks, List<int>& outputMults)
  { outputLetters.SetSize(0);
    outputRanks.SetSize(0);
    outputMults.SetSize(0);
    for (int i=0; i<this->size; i++)
    { outputLetters.AddOnTop((*this)[i].theLetter);
      outputRanks.AddOnTop((*this)[i].theRank);
      outputMults.AddOnTop(this->theCoeffs[i]);
    }
  }
};

class SemisimpleSubalgebras
{
public:
  SltwoSubalgebras theSl2s;
  List<SemisimpleLieAlgebra>* owners;
  int indexInOwners;
  List<Vectors<Rational> > theHcandidates;
  int indexLowestUnexplored;
  SltwoSubalgebras CandidatesPrincipalSl2ofSubalgebra;
  Selection RemainingCandidates;
//  List<DynkinDiagramRootSubalgebra> thePossibleDynkinDiagrams;
  List<DynkinType> theTypes;
  List<List<int> > thePartitionValues;
  List<List<int> > thePartitionMultiplicities;
  List<SltwoSubalgebras> theCandidateSubAlgebras;
  List<List<int> > IndicesMatchingSl2s;
  List<List<int> > IndicesMatchingPartitionSl2s;
  List<List<int> > IndicesMatchingActualSl2s;
  void FindHCandidates
  (GlobalVariables* theGlobalVariables)
  ;
  void FindHCandidatesWithOneExtraHContaining
  (Vectors<Rational>& inpuT, GlobalVariables* theGlobalVariables)
  ;
  SemisimpleLieAlgebra& GetSSowner()
  { if (this->owners==0 || this->indexInOwners<0)
    { std::cout << "This is a programming error: attempted to access non-initialized "
      << " semisimple Lie subalgerbas. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    return (*this->owners)[this->indexInOwners];
  }
  SemisimpleSubalgebras(List<SemisimpleLieAlgebra>* inputOwners, int inputIndexInOwner):
  owners(inputOwners), indexInOwners(inputIndexInOwner)
  { this->theSl2s.owners=inputOwners;
    this->theSl2s.IndexInOwners=inputIndexInOwner;
  }
  std::string ToString(FormatExpressions* theFormat=0);
  void MatchRealSl2sToPartitionSl2s();
  void MakeSelectionBasedOnPrincipalSl2s(GlobalVariables& theGlobalVariables);
  void MatchActualSl2sFixedRootSAToPartitionSl2s(GlobalVariables& theGlobalVariables);
  void GenerateModuleDecompositionsPrincipalSl2s(int theRank, GlobalVariables& theGlobalVariables);
  std::string ElementToStringCandidatePrincipalSl2s(FormatExpressions* theFormat=0);
  void FindTheSSSubalgebras
  (List<SemisimpleLieAlgebra>* newOwner, int newIndexInOwner, GlobalVariables* theGlobalVariables)
  ;
  void FindTheSSSubalgebrasPart2
  (GlobalVariables* theGlobalVariables)
  ;
  void EnumerateAllPossibleDynkinDiagramsOfRankUpTo(int theRank);
  void GenerateAllPartitionsUpTo(int theRank);
  void GenerateAllPartitionsDontInit(int theRank);
  void GenerateAllDiagramsForPartitionRecursive(int indexPartition, int indexInPartition, List<int>& ranksBuffer, List<int>& multiplicitiesBuffer, List<char>& lettersBuffer);
  void GenerateAllPartitionsRecursive(int remainingToBePartitioned, int CurrentValue, List<int>& Multiplicities, List<int>& Values);
};


#endif

