#include "polyhedra.h"
// This file is meant to be for people to modify if they do not want to modify the main files polyhedra.cpp or polyhedra.h
// The reason I would recommend that is because the file polyhedra.cpp compiles very slowly (around 30 seconds), so small modifications
// take a long time to check. Using a separate file and linking it to the GUI is the solution I chose (may not be the best, see below for discussion).
// If the code you write here reaches the mature phase where you have realized all functions you think it should have
// and you are generally satisfied with it, simply cut & paste it in the main files (the class declarations in polyhedra.h and the implementation in polyhedra.cpp).

//Discussion: other options for speeding up the compilation that I have considered.
//1. Cut up the main files polyhedra.h and polyhedra.cpp into small quick-to-compile pieces. This might be the best solution in the long run. However, I do not want to
//   do it just yet, because 1) I am not sure what should those parts be - it is not yet clear in my head how to partition the code in conceptually distinct pieces
//   2) this would certainly create additional maintainance work 3) this will increase the learning curve for a person wanting to just use the program and wanting to eventually
//   modify some tiny bit and 4) I got an advice on the c++ forum www.cplusplus.com that partitioning the .h file will eventually lead to slower compile times,
//   especially with the massive use of templates that I do. Therefore, such a partitioning should not be done before the code reaches
//   greater maturity (see also point (1) ).
//2. Use precompiled headers or some other voodoo. I am tocally against that. Those are compiler specific, will require me to learn extra unnecessary info which might
//    be out of date in a few years, and will make even higher entry learning curve for another to join the project. This is bad.
//    I should mention in favor of Microsoft that their IDE does recompile very quickly small modifications of the file polyhedra.cpp. I believe it does so by
//    keeping a database of your recently changed code, and recompiling only the recently changed pieces. Hats off to Microsoft for doing that completely programmatically,
//    and not bothering the programmer with stupid things such as how to set up precompiled headers.
//
// To whomever might be reading this (if anyone): happy hacking and I hope you find my code useful, that it didn't cause you many headaches, and that you
// did something useful with it! Cheers!

void ComputationSetup::ComputeCharaterFormulas(ComputationSetup& inputData, GlobalVariables& theGlobalVariables)
{ if (inputData.thePartialFraction.theChambers.flagSliceWithAWallInitDone)
    while (inputData.thePartialFraction.theChambers.oneStepChamberSlice(theGlobalVariables)){}
  else
  { WeylGroup A2test;
    A2test.MakeArbitrary('G', 2);
    A2test.ComputeRho(true);
    inputData.thePartialFraction.theChambers.initCharacterComputation(A2test, A2test.RootsOfBorel, theGlobalVariables);
  }
}

class GeneralizedVermaModuleCharacters
{
public:
  bool flagUsingNewSplit;
  List<MatrixLargeRational> theLinearOperators;
  roots GmodKnegativeWeights;
  ConeGlobal PreimageWeylChamberLargerAlgebra;
  ConeGlobal WeylChamberSmallerAlgebra;
  List<QuasiPolynomial> theQPsNonSubstituted;
  List<List<QuasiPolynomial> > theQPsSubstituted;
//  List<CombinatorialChamber> parametricChambers;
  List<QuasiPolynomial> theMultiplicities;
  List<QuasiPolynomial > theMultiplicitiesExtremaCandidates;
  int tempDebugCounter;
  List<Rational> theCoeffs;
  roots theTranslations;
  roots theTranslationsProjected;
  partFractions thePfs;
  List<List<roots> > paramSubChambers, nonParamVertices;
  List<List<QuasiPolynomial> > ExtremeQPsParamSubchambers;
  List<roots> allParamSubChambersRepetitionsAllowed;
  List<Cone> allParamSubChambersRepetitionsAllowedConeForm;
  CombinatorialChamberContainer projectivizedChamber;
  ConeComplex projectivizedParamComplex;
  ConeComplex projectivizedChamberTest;
  std::stringstream log;
  GeneralizedVermaModuleCharacters(){this->flagUsingNewSplit=true;
  this->tempDebugCounter=1;
  }
  void GetProjection(int indexOperator, const root& input, root& output);
  void FindMultiplicitiesExtrema(GlobalVariables& theGlobalVariables);
  void ProcessExtremaOneChamber
  (Cone& input, GlobalVariables& theGlobalVariables)
  ;
  void ProcessOneParametricChamber
  (int numNonParams, int numParams, roots& inputNormals, List<roots>& theParamChambers, List<roots>& theNonParamVertices, GlobalVariables& theGlobalVariables)
  ;
  void ComputeQPsFromChamberComplex
  (GlobalVariables& theGlobalVariables)
  ;
  void GetSubFromIndex(QPSub& output, int theIndex);
  void GetSubFromNonParamArray
  (QPSub& output, roots& NonParams, int numParams)
  ;
  void initQPs
  (GlobalVariables& theGlobalVariables)
  ;
  void initFromHomomorphism
  (HomomorphismSemisimpleLieAlgebra& input, GlobalVariables& theGlobalVariables)
  ;
  void TransformToWeylProjective
  (WallData& output, GlobalVariables& theGlobalVariables)
  ;
  void TransformToWeylProjective
  (GlobalVariables& theGlobalVariables)
  ;
  void TransformToWeylProjective
  (CombinatorialChamberContainer& owner, CombinatorialChamber& output, GlobalVariables& theGlobalVariables)
  ;
  void TransformToWeylProjective
  (int indexOperator, root& startingNormal, root& outputNormal)
  ;
};

void GeneralizedVermaModuleCharacters::TransformToWeylProjective
  (int indexOperator, root& startingNormal, root& outputNormal)
{ MatrixLargeRational& theOperator=this->theLinearOperators.TheObjects[indexOperator];
  root& theTranslation=this->theTranslationsProjected.TheObjects[indexOperator];
  outputNormal.MakeZero(startingNormal.size+theOperator.NumCols+1);
  for (int i=0; i<startingNormal.size; i++)
  { outputNormal.TheObjects[i]=startingNormal.TheObjects[i];
    *outputNormal.LastObject()-=theTranslation.TheObjects[i]*startingNormal.TheObjects[i];
  }
  for (int i=0; i<theOperator.NumCols; i++)
    for (int j=0; j<theOperator.NumRows; j++)
      outputNormal.TheObjects[startingNormal.size+i]-=theOperator.elements[j][i]*startingNormal.TheObjects[j];
}

void GeneralizedVermaModuleCharacters::TransformToWeylProjective
  (WallData& output, GlobalVariables& theGlobalVariables)
{ root tempRoot=output.normal;
  this->TransformToWeylProjective(0, tempRoot, output.normal);
}

void GeneralizedVermaModuleCharacters::TransformToWeylProjective
  (CombinatorialChamberContainer& owner, CombinatorialChamber& output, GlobalVariables& theGlobalVariables)
{ for (int i=0; i<output.Externalwalls.size; i++)
    this->TransformToWeylProjective(output.Externalwalls.TheObjects[i], theGlobalVariables);
  WallData newWall;
  output.Externalwalls.MakeActualSizeAtLeastExpandOnTop(this->PreimageWeylChamberLargerAlgebra.size+output.Externalwalls.size);
  for (int i=0; i<this->PreimageWeylChamberLargerAlgebra.size; i++)
  { newWall.normal=PreimageWeylChamberLargerAlgebra.TheObjects[i];
    output.Externalwalls.AddObjectOnTop(newWall);
  }
//  output.ComputeDebugString(owner);
  output.AllVertices.size=0;
  output.ComputeVerticesFromNormals(owner, theGlobalVariables);
}

void GeneralizedVermaModuleCharacters::TransformToWeylProjective
(GlobalVariables& theGlobalVariables)
{ this->projectivizedChamber.flagSliceWithAWallInitDone=true;
  this->projectivizedChamber.theDirections=this->GmodKnegativeWeights;
  this->projectivizedChamber.init();
  this->projectivizedChamber.SliceTheEuclideanSpace(theGlobalVariables, false);
  this->projectivizedChamber.drawOutput(theGlobalVariables.theDrawingVariables, this->projectivizedChamber.IndicatorRoot, 0);
  std::string tempS;
  this->projectivizedChamber.ElementToString(tempS);
  this->log << tempS;
  this->projectivizedChamber.NewHyperplanesToSliceWith.size=0;
  this->projectivizedChamber.theHyperplanes.size=0;
  this->projectivizedChamber.AmbientDimension=this->projectivizedChamber.AmbientDimension+this->theLinearOperators.TheObjects[0].NumCols+1;
  root wallToSliceWith;
//  roots oldDirections;
  for (int k=0; k<this->theLinearOperators.size; k++)
    for (int i=0; i<this->projectivizedChamber.size; i++)
      if (this->projectivizedChamber.TheObjects[i]!=0)
        for (int j=0; j<this->projectivizedChamber.TheObjects[i]->Externalwalls.size; j++)
        { this->TransformToWeylProjective(k, this->projectivizedChamber.TheObjects[i]->Externalwalls.TheObjects[j].normal, wallToSliceWith);
//          if (k==0)
//            oldDirections.AddOnBottomNoRepetition(wallToSliceWith);
          wallToSliceWith.ScaleToIntegralMinHeightFirstNonZeroCoordinatePositive();
          if (k>0)
            this->projectivizedChamber.NewHyperplanesToSliceWith.AddOnTopNoRepetition(wallToSliceWith);
          this->projectivizedChamber.theHyperplanes.AddObjectOnTopNoRepetitionOfObjectHash(wallToSliceWith);
        }
  this->log << "\n Projectivized walls to slice with(" << this->projectivizedChamber.NewHyperplanesToSliceWith.size << "):" ;
  for (int i=0; i<this->projectivizedChamber.NewHyperplanesToSliceWith.size; i++)
    this->log << "\n" << this->projectivizedChamber.NewHyperplanesToSliceWith.TheObjects[i].ElementToString();
  this->log << "\n";
  for (int i=0; i<this->projectivizedChamber.size; i++)
    if (this->projectivizedChamber.TheObjects[i]!=0)
      this->TransformToWeylProjective(this->projectivizedChamber, *this->projectivizedChamber.TheObjects[i], theGlobalVariables);
  root tempRoot;
  for (int i=0; i<this->projectivizedChamber.TheGlobalConeNormals.size; i++)
  { tempRoot.MakeZero(this->projectivizedChamber.AmbientDimension);
    int startingDim=this->projectivizedChamber.TheGlobalConeNormals.TheObjects[i].size;
    for (int j=0; j<startingDim; j++)
    { tempRoot.TheObjects[j]=this->projectivizedChamber.TheGlobalConeNormals.TheObjects[i].TheObjects[j];
      tempRoot.TheObjects[j+startingDim]=-tempRoot.TheObjects[j];
    }
    this->projectivizedChamber.TheGlobalConeNormals.TheObjects[i]=tempRoot;
  }
  roots tempRoots;
  this->projectivizedChamber.TheGlobalConeNormals.AddListOnTop(this->PreimageWeylChamberLargerAlgebra);
  this->log << "the global cone normals: " << this->projectivizedChamber.TheGlobalConeNormals.ElementToString();
  for (int i=0; i<this->WeylChamberSmallerAlgebra.size; i++)
  { root& currentWeylWall=this->WeylChamberSmallerAlgebra.TheObjects[i];
    this->projectivizedChamber.SliceWithAWall(currentWeylWall, theGlobalVariables);
    for (int j=0; j<this->projectivizedChamber.size; j++)
      if (this->projectivizedChamber.TheObjects[j]!=0)
      { CombinatorialChamber& current=*this->projectivizedChamber.TheObjects[j];
        if (root::RootScalarEuclideanRoot(current.InternalPoint, currentWeylWall).IsNegative())
          current.flagPermanentlyZero=true;
      }
  }
  if(this->flagUsingNewSplit)
  { List<roots> tempChambers;
    tempChambers.MakeActualSizeAtLeastExpandOnTop(this->projectivizedChamber.size);
    for (int j=0; j<this->projectivizedChamber.size; j++)
      if (this->projectivizedChamber.TheObjects[j]!=0)
        if (!this->projectivizedChamber.TheObjects[j]->flagPermanentlyZero)
        { CombinatorialChamber& current=*this->projectivizedChamber.TheObjects[j];
          tempChambers.SetSize(tempChambers.size+1);
          current.GetWallNormals(*tempChambers.LastObject());
        }
    this->projectivizedChamberTest.initFromCones(tempChambers, theGlobalVariables);
    this->projectivizedChamberTest.splittingNormals.ClearTheObjects();
    this->projectivizedChamberTest.splittingNormals.AddListOnTopNoRepetitionOfObjectHash(this->projectivizedChamber.theHyperplanes);
  }
  this->projectivizedChamber.ElementToString(tempS);
  this->log << tempS;
  theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
  theGlobalVariables.theIndicatorVariables.StatusString1=this->log.str();
  theGlobalVariables.MakeReport();
  this->projectivizedChamber.NumAffineHyperplanesProcessed=-1;
//  this->projectivizedChamber.NewHyperplanesToSliceWith.size=1;
}

void GeneralizedVermaModuleCharacters::initFromHomomorphism
  (HomomorphismSemisimpleLieAlgebra& input, GlobalVariables& theGlobalVariables)
{ roots tempRoots;
  WeylGroup& theWeyl=input.theRange.theWeyl;
//  input.ProjectOntoSmallCartan(theWeyl.RootsOfBorel, tempRoots, theGlobalVariables);
  this->log << "projections: " << tempRoots.ElementToString();
  theWeyl.ComputeWeylGroup();
  this->theLinearOperators.SetSize(theWeyl.size);
  this->theTranslations.SetSize(theWeyl.size);
  this->theTranslationsProjected.SetSize(theWeyl.size);
  this->theCoeffs.SetSize(theWeyl.size);
  MatrixLargeRational theProjection;
  theProjection.init(input.theDomain.GetRank(), input.theRange.GetRank());
  root startingWeight, projectedWeight;
  SSalgebraModule tempM;
  std::stringstream tempStream;
  input.ComputeHomomorphismFromImagesSimpleChevalleyGenerators(theGlobalVariables);
  tempM.InduceFromEmbedding(tempStream, input, theGlobalVariables);
  input.GetWeightsGmodK(this->GmodKnegativeWeights, theGlobalVariables);
  MatrixLargeRational tempMat;
  tempMat=input.theDomain.theWeyl.CartanSymmetric;
  tempMat.Invert(theGlobalVariables);
  tempMat.ActOnRoots(this->GmodKnegativeWeights);
  this->log << this->GmodKnegativeWeights.ElementToString();
  for (int i=0; i<this->GmodKnegativeWeights.size; i++)
    if (this->GmodKnegativeWeights.TheObjects[i].IsPositiveOrZero())
    { this->GmodKnegativeWeights.PopIndexSwapWithLast(i);
      i--;
    }
  for (int i=0; i< input.theRange.GetRank(); i++)
  { startingWeight.MakeEi(input.theRange.GetRank(), i);
    input.ProjectOntoSmallCartan(startingWeight, projectedWeight, theGlobalVariables);
    for (int j=0; j<projectedWeight.size; j++)
      theProjection.elements[j][i]=projectedWeight.TheObjects[j];
  }
  this->log << "\nMatrix form of the elements of W(B_3) (" << theWeyl.size << " elements):\n";
  for (int i=0; i<theWeyl.size; i++)
  { theWeyl.GetMatrixOfElement(i, this->theLinearOperators.TheObjects[i]);
    this->log << "\n" << this->theLinearOperators.TheObjects[i].ElementToString(false, false);
    this->theTranslations.TheObjects[i]=theWeyl.rho;
    theWeyl.ActOn(i, this->theTranslations.TheObjects[i], true, false, (Rational) 0);
    this->theTranslations.TheObjects[i]-=theWeyl.rho;
    theProjection.ActOnAroot(this->theTranslations.TheObjects[i], this->theTranslationsProjected.TheObjects[i]);
    if (theWeyl.TheObjects[i].size%2==0)
      this->theCoeffs.TheObjects[i]=1;
    else
      this->theCoeffs.TheObjects[i]=-1;
  }
  this->log << "\n\n\nMatrix of the projection operator:\n" << theProjection.ElementToString(false, false);
  this->log << "\n\n\nMatrix form of the operators $u_w$, the translations and their projections (" << this->theLinearOperators.size << "):";
  List<MatrixLargeRational> tempList;
  for (int i=0; i<this->theLinearOperators.size; i++)
  { this->theLinearOperators.TheObjects[i].MultiplyOnTheLeft(theProjection);
    this->log << "\n\n" << this->theLinearOperators.TheObjects[i].ElementToString(false, false);
    this->log << this->theTranslations.TheObjects[i].ElementToString() << ";   " << this->theTranslationsProjected.TheObjects[i].ElementToString();
    tempList.AddOnTopNoRepetition(this->theLinearOperators.TheObjects[i]);
  }
  this->log << "\n\n\nThere are " << tempList.size << " different operators.";
  tempMat=theWeyl.CartanSymmetric;
  tempMat.Invert(theGlobalVariables);
  tempRoots.AssignMatrixRows(tempMat);
  this->PreimageWeylChamberLargerAlgebra.ComputeFromDirections(tempRoots, theGlobalVariables, theWeyl.GetDim());
  this->log << "\nWeyl chamber larger algebra before projectivizing: " << this->PreimageWeylChamberLargerAlgebra.ElementToString() << "\n";
  root tempRoot;
  for (int i=0; i<this->PreimageWeylChamberLargerAlgebra.size; i++)
  { tempRoot.MakeZero(input.theRange.GetRank()+input.theDomain.GetRank()+1);
    for (int j=0; j<input.theRange.GetRank(); j++)
      tempRoot.TheObjects[j+input.theDomain.GetRank()]=this->PreimageWeylChamberLargerAlgebra.TheObjects[i].TheObjects[j];
    this->PreimageWeylChamberLargerAlgebra.TheObjects[i]=tempRoot;
  }

  tempMat=input.theDomain.theWeyl.CartanSymmetric;
  tempMat.Invert(theGlobalVariables);
  tempRoots.AssignMatrixRows(tempMat);
  this->WeylChamberSmallerAlgebra.ComputeFromDirections(tempRoots, theGlobalVariables, input.theDomain.GetRank());
  this->log << "Weyl chamber smaller algebra before projectivizing: " << this->WeylChamberSmallerAlgebra.ElementToString() << "\n";
  for (int i=0; i<this->WeylChamberSmallerAlgebra.size; i++)
  { tempRoot.MakeZero(input.theRange.GetRank()+input.theDomain.GetRank()+1);
    for (int j=0; j<input.theDomain.GetRank(); j++)
      tempRoot.TheObjects[j]=this->WeylChamberSmallerAlgebra.TheObjects[i].TheObjects[j];
    this->WeylChamberSmallerAlgebra.TheObjects[i]=tempRoot;
  }

  tempRoot.MakeEi(input.theRange.GetRank()+input.theDomain.GetRank()+1, input.theRange.GetRank()+input.theDomain.GetRank());
  this->PreimageWeylChamberLargerAlgebra.AddObjectOnTop(tempRoot);
  this->log << "\nWeyl chamber larger algebra after projectivizing: " << this->PreimageWeylChamberLargerAlgebra.ElementToString() << "\n";
  this->log << "\nWeyl chamber smaller algebra after projectivizing: " << this->WeylChamberSmallerAlgebra.ElementToString() << "\n";

  theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
  theGlobalVariables.theIndicatorVariables.StatusString1=this->log.str();
  theGlobalVariables.MakeReport();
}

void WeylGroup::GetMatrixOfElement(int theIndex, MatrixLargeRational& outputMatrix)
{ root tempRoot;
  int theDim=this->CartanSymmetric.NumRows;
  outputMatrix.init(theDim, theDim);
  for (int i=0; i<theDim; i++)
  { tempRoot.MakeEi(theDim, i);
    this->ActOn(theIndex, tempRoot, false, false, (Rational) 0);
    for (int j=0; j<theDim; j++)
      outputMatrix.elements[j][i]=tempRoot.TheObjects[j];
  }
}

GeneralizedVermaModuleCharacters tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis;

void GeneralizedVermaModuleCharacters::GetProjection(int indexOperator, const root& input, root& output)
{ MatrixLargeRational& currentOperator=this->theLinearOperators.TheObjects[indexOperator];
  root& currentTranslation=this->theTranslations.TheObjects[indexOperator];
  root tempRoot2;
  tempRoot2.SetSize(currentOperator.NumCols);
  output.MakeZero(currentOperator.NumRows);
  for (int i=0; i<currentOperator.NumCols; i++)
    tempRoot2.TheObjects[i]=input.TheObjects[currentOperator.NumRows+i];
  for (int j=0; j<currentOperator.NumRows; j++)
    output.TheObjects[j]=input.TheObjects[j];
  currentOperator.ActOnAroot(tempRoot2);
  output-=tempRoot2;
  currentOperator.ActOnAroot(currentTranslation, tempRoot2);
  output-=tempRoot2;
}

void MatrixLargeRational::GetMatrixIntWithDen(MatrixIntTightMemoryFit& outputMat, int& outputDen)
{ outputDen=this->FindPositiveLCMCoefficientDenominatorsTruncated();
  outputMat.init(this->NumRows, this->NumCols);
  for (int i=0; i<this->NumRows; i++)
    for (int j=0; j<this->NumCols; j++)
      outputMat.elements[i][j]=(this->elements[i][j]*outputDen).NumShort;
}

void QPSub::MakeSubFromMatrixRational(MatrixLargeRational& input)
{ input.GetMatrixIntWithDen(this->TheQNSub, this->QNSubDen);
  this->MakeSubFromMatrixIntAndDen(this->TheQNSub, this->QNSubDen);
}

void GeneralizedVermaModuleCharacters::GetSubFromIndex(QPSub& output, int theIndex)
{ MatrixLargeRational& theOperator=this->theLinearOperators.TheObjects[theIndex];
  int dimLargerAlgebra=theOperator.NumCols;
  int dimSmallerAlgebra=theOperator.NumRows;
  root& theTranslation=this->theTranslations.TheObjects[theIndex];
  MatrixLargeRational tempMat;
  tempMat.init(dimLargerAlgebra+dimSmallerAlgebra+1, dimSmallerAlgebra);
  tempMat.NullifyAll();
  for (int j=0; j<dimSmallerAlgebra; j++)
  { tempMat.elements[j][j]=1;
    for (int i=0; i<dimLargerAlgebra; i++)
      tempMat.elements[i][j]-=theOperator.elements[j][i];
    tempMat.elements[dimLargerAlgebra+dimSmallerAlgebra][j]=-theTranslation.TheObjects[j];
  }
  output.MakeSubFromMatrixRational(tempMat);
}

std::string QPSub::ElementToString()
{ std::stringstream out;
  out << "Matrix den:" << this->QNSubDen << " and the matrix:\n";
  for (int i=0; i<this->TheQNSub.NumRows; i++)
  { for (int j=0; j<this->TheQNSub.NumCols; j++)
      out << this->TheQNSub.elements[i][j] << "\t";
    out << "\n";
  }
  for (int i=0; i<this->RationalPolyForm.size; i++)
    out << "x_" << i+1 << "->" << this->RationalPolyForm.TheObjects[i].ElementToString() << ", ";
  return out.str();
}

void GeneralizedVermaModuleCharacters::ComputeQPsFromChamberComplex
(GlobalVariables& theGlobalVariables)
{ if (!this->flagUsingNewSplit)
    return;
  std::stringstream out;
  PolynomialOutputFormat theFormat;
  root tempRoot;
  QPSub theSub;
  this->thePfs.initFromRoots(this->GmodKnegativeWeights, theGlobalVariables);
  this->thePfs.ComputeDebugString(theGlobalVariables);
  out << this->thePfs.DebugString;
  this->thePfs.split(theGlobalVariables, 0);
  this->thePfs.ComputeDebugString(theGlobalVariables);
  out << "=" << this->thePfs.DebugString;
  int totalDim=this->theTranslations.TheObjects[0].size-1;
  this->theQPsSubstituted.SetSize(this->projectivizedChamberTest.size);
  this->theMultiplicities.SetSize(this->projectivizedChamberTest.size);
  this->thePfs.theChambers.init();
  this->thePfs.theChambers.theDirections=this->GmodKnegativeWeights;
  this->thePfs.theChambers.SliceTheEuclideanSpace(theGlobalVariables, false);
  this->theQPsNonSubstituted.SetSize(this->thePfs.theChambers.size);
  for (int i=0; i<this->thePfs.theChambers.size; i++)
    if (this->thePfs.theChambers.TheObjects[i]!=0)
    { QuasiPolynomial& currentQPNoSub= this->theQPsNonSubstituted.TheObjects[i];
      this->thePfs.partFractionsToPartitionFunctionAdaptedToRoot(currentQPNoSub, this->thePfs.theChambers.TheObjects[i]->InternalPoint, false, true, theGlobalVariables, true);
    }
  QuasiPolynomial theQPNoSub;
  theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=false;
  for (int i=0; i<this->projectivizedChamberTest.size; i++)
//  if(i<this->tempDebugCounter)
  { this->theQPsSubstituted.TheObjects[i].SetSize(this->theLinearOperators.size);
    for (int k=0; k<this->theLinearOperators.size; k++)
//    if (k<this->tempDebugCounter)
    { QuasiPolynomial& currentQPSub=this->theQPsSubstituted.TheObjects[i].TheObjects[k];
      this->GetProjection(k, this->projectivizedChamberTest.TheObjects[i].GetInternalPoint(), tempRoot);
      int theIndex= this->thePfs.theChambers.GetFirstChamberIndexContainingPoint(tempRoot);
      if (theIndex==-1)
        theQPNoSub.Nullify(tempRoot.size);
      else
        theQPNoSub=this->theQPsNonSubstituted.TheObjects[theIndex];
      std::stringstream tempStream;
      tempStream << "chamber " << i+1 << " linear opeator " << k+1;
      theGlobalVariables.theIndicatorVariables.ProgressReportString1= tempStream.str();
      theGlobalVariables.MakeReport();
      out << "\nChamber " << i << " translation " << k << ": " << theQPNoSub.ElementToString(false, theFormat);
      this->GetSubFromIndex(theSub, k);
      theQPNoSub.RationalLinearSubstitution(theSub, currentQPSub);
      out << "; after substitution we get: " << currentQPSub.ElementToString(false, theFormat);
      out << "\nthe sub is: " << theSub.ElementToString();
    }
  }
  theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
  QuasiPolynomial tempQP;
  for (int i=0; i<this->projectivizedChamberTest.size; i++)
//  if (i<this->tempDebugCounter)
  { QuasiPolynomial& currentSum=this->theMultiplicities.TheObjects[i];
    currentSum.Nullify(totalDim);
    for (int k=0; k<this->theLinearOperators.size; k++)
//    if (k<this->tempDebugCounter)
    { tempQP=this->theQPsSubstituted.TheObjects[i].TheObjects[k];
      tempQP*=this->theCoeffs.TheObjects[k];
      currentSum+=tempQP;
    }
    out << "\nChamber " << i+1 << ": the quasipolynomial is: " << currentSum.ElementToString();\
    out << "\nThe chamber is: " << this->projectivizedChamberTest.TheObjects[i].ElementToString();
  }
  this->projectivizedChamberTest.ComputeDebugString();
  out << "\n\n" << this->projectivizedChamberTest.DebugString;
  theGlobalVariables.theIndicatorVariables.StatusString1=out.str();
  theGlobalVariables.MakeReport();
}

void ComputationSetup::ComputeGenVermaCharaterG2inB3(ComputationSetup& inputData, GlobalVariables& theGlobalVariables)
{ if (tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis.projectivizedChamber.flagSliceWithAWallInitDone)
  { if (!tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis.flagUsingNewSplit)
    { while (tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis.projectivizedChamber.oneStepChamberSlice(theGlobalVariables)){}
    } else
    { tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis.projectivizedChamberTest.Refine(theGlobalVariables);
      std::stringstream out;
      out << tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis.projectivizedChamberTest.ElementToString();
      theGlobalVariables.theIndicatorVariables.StatusString1=out.str();
      theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
      theGlobalVariables.MakeReport();
    }
    tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis.ComputeQPsFromChamberComplex(theGlobalVariables);
    tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis.FindMultiplicitiesExtrema(theGlobalVariables);
  }
  else
  { inputData.theParser.theHmm.MakeG2InB3(inputData.theParser, theGlobalVariables);
    tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis.initFromHomomorphism(inputData.theParser.theHmm, theGlobalVariables);
    tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis.TransformToWeylProjective(theGlobalVariables);
//    theGlobalVariables.theIndicatorVariables.StatusString1=tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis.log.str();
    theGlobalVariables.theIndicatorVariables.StatusString1=tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis.projectivizedChamberTest.ElementToString();
    theGlobalVariables.MakeReport();
  }
}

void CombinatorialChamberContainer::initCharacterComputation
  (WeylGroup& inputWeyl, roots& inputWeights, GlobalVariables& theGlobalVariables)
{ this->flagSliceWithAWallInitDone=true;
  this->AmbientWeyl.GetElement()=inputWeyl;
  this->theDirections=inputWeights;
  this->init();
  this->SliceTheEuclideanSpace(theGlobalVariables, false);
  this->drawOutput(theGlobalVariables.theDrawingVariables, this->IndicatorRoot, 0);
  WeylGroup& theWeyl= this->AmbientWeyl.GetElement();
  MatrixLargeRational tempMat;
  tempMat=theWeyl.CartanSymmetric;
  tempMat.Invert(theGlobalVariables);
  roots tempRoots;
  tempRoots.AssignMatrixRows(tempMat);
  this->WeylChamber.ComputeFromDirections(tempRoots, theGlobalVariables, theWeyl.GetDim());
  this->TransformToWeylProjective(theGlobalVariables);
  this->NumAffineHyperplanesProcessed=-1;
}

void WallData::TransformToWeylProjective
  (GlobalVariables& theGlobalVariables)
{ root tempRoot;
  int startingDimension=this->normal.size;
  tempRoot.MakeZero(startingDimension*2+1);
  for (int i=0; i<startingDimension; i++)
  { tempRoot.TheObjects[i]=this->normal.TheObjects[i];
    tempRoot.TheObjects[i+startingDimension]= -this->normal.TheObjects[i];
  }
  this->normal=tempRoot;
}

void CombinatorialChamberContainer::GetWeylChamberWallsForCharacterComputation(roots& output)
{ output.SetSize(this->WeylChamber.size+1);
  int startingDimension=(this->AmbientDimension-1)/2;
  for (int i=0; i<this->WeylChamber.size; i++)
  { output.TheObjects[i].MakeZero(this->AmbientDimension);
    for(int j=0; j<startingDimension; j++)
      output.TheObjects[i].TheObjects[startingDimension+j]=this->WeylChamber.TheObjects[i].TheObjects[j];
    output.TheObjects[i].ComputeDebugString();
  }
  output.LastObject()->MakeZero(this->AmbientDimension);
  *output.LastObject()->LastObject()=1;
}

void CombinatorialChamber::TransformToWeylProjective
  (CombinatorialChamberContainer& owner, GlobalVariables& theGlobalVariables)
{ for (int i=0; i<this->Externalwalls.size; i++)
    this->Externalwalls.TheObjects[i].TransformToWeylProjective(theGlobalVariables);
  WallData newWall;
  this->Externalwalls.MakeActualSizeAtLeastExpandOnTop(owner.WeylChamber.size+this->Externalwalls.size);
  roots newExternalWalls;
  owner.GetWeylChamberWallsForCharacterComputation(newExternalWalls);
  for (int i=0; i<newExternalWalls.size; i++)
  { newWall.normal=newExternalWalls.TheObjects[i];
    this->Externalwalls.AddObjectOnTop(newWall);
  }
  this->AllVertices.size=0;
  this->ComputeVerticesFromNormals(owner, theGlobalVariables);
}

void CombinatorialChamberContainer::TransformToWeylProjective
  (GlobalVariables& theGlobalVariables)
{ this->AmbientWeyl.GetElement().ComputeWeylGroup();
  this->log << this->AmbientWeyl.GetElement().ElementToString();
  std::string tempS;
  this->ElementToString(tempS);
  this->log << "\nWeyl chamber: " << this->WeylChamber.ElementToString() << "\n";
  this->log << tempS;
  this->NewHyperplanesToSliceWith.size=0;
  this->theHyperplanes.size=0;
  root wallToSliceWith;
//  roots oldDirections;
  for (int k=0; k<this->AmbientWeyl.GetElement().size; k++)
    for (int i=0; i<this->size; i++)
      if (this->TheObjects[i]!=0)
        for (int j=0; j<this->TheObjects[i]->Externalwalls.size; j++)
        { this->GetAffineWallImage(k, this->TheObjects[i]->Externalwalls.TheObjects[j], wallToSliceWith, theGlobalVariables);
//          if (k==0)
//            oldDirections.AddOnBottomNoRepetition(wallToSliceWith);
          wallToSliceWith.ScaleToIntegralMinHeightFirstNonZeroCoordinatePositive();
          if (k>0)
            this->NewHyperplanesToSliceWith.AddOnTopNoRepetition(wallToSliceWith);
          this->theHyperplanes.AddObjectOnTopNoRepetitionOfObjectHash(wallToSliceWith);
        }
  this->log << "\n Affine walls to slice with:";
  for (int i=0; i<this->NewHyperplanesToSliceWith.size; i++)
    this->log << "\n" << this->NewHyperplanesToSliceWith.TheObjects[i].ElementToString();
  this->log << "\n";
  this->AmbientDimension=this->AmbientDimension*2+1;
  for (int i=0; i<this->size; i++)
    if (this->TheObjects[i]!=0)
    { this->TheObjects[i]->TransformToWeylProjective(*this, theGlobalVariables);
//      theVertices.AddRootSnoRepetition(this->TheObjects[i]->AllVertices);
    }
  //this->startingCones.initFromDirections()
  root tempRoot;
  for (int i=0; i<this->TheGlobalConeNormals.size; i++)
  { tempRoot.MakeZero(this->AmbientDimension);
    int startingDim=this->TheGlobalConeNormals.TheObjects[i].size;
    for (int j=0; j<startingDim; j++)
    { tempRoot.TheObjects[j]=this->TheGlobalConeNormals.TheObjects[i].TheObjects[j];
      tempRoot.TheObjects[j+startingDim]=-tempRoot.TheObjects[j];
    }
    this->TheGlobalConeNormals.TheObjects[i]=tempRoot;
  }
  roots tempRoots;
  this->GetWeylChamberWallsForCharacterComputation(tempRoots);
  this->TheGlobalConeNormals.AddListOnTop(tempRoots);
  this->log << "the global cone normals: " << this->TheGlobalConeNormals.ElementToString();
  this->ElementToString(tempS);
  this->log << tempS;
  theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
  theGlobalVariables.theIndicatorVariables.StatusString1=this->log.str();
  theGlobalVariables.MakeReport();
}

std::string roots::ElementsToInequalitiesString(bool useLatex, bool useHtml)
{ std::stringstream out;
  std::string theLetter="x";
  if (useLatex)
    out << "\\begin{array}{l}";
  for (int i=0; i<this->size; i++)
  { root& current=this->TheObjects[i];
    out << current.ElementToStringLetterFormat(theLetter, useLatex);
    if (useLatex)
      out << "\\geq 0\\\\";
    else
      out << "=>0\n";
    if (useHtml)
      out << "<br>";
  }
  if (useLatex)
    out << "\\end{array}";
  return out.str();
}

void CombinatorialChamber::ElementToInequalitiesString(std::string& output, CombinatorialChamberContainer& owner, bool useLatex, bool useHtml, PolynomialOutputFormat& PolyFormatLocal)
{ int theDimension=owner.AmbientDimension;
  this->SortNormals();
  this->AllVertices.QuickSortAscending();
  std::string tempS; std::stringstream out;
  if (useLatex)
    out << "\n\\begin{eqnarray*}\n";
  if (useHtml)
    out << "\n<br>\n";
  for (int i=0; i<this->Externalwalls.size; i++)
  { root& currentNormal= this->ExternalwallsNormalsSorted.TheObjects[i];
    for (int j=0; j<theDimension; j++)
    { currentNormal.TheObjects[j].ElementToString(tempS);
      if (tempS!="0")
      { if (tempS=="1")
        { tempS="";
          out << '+';
        }
        if (tempS=="-1")
          tempS="-";
        if (j!=0)
          if (tempS.size()!=0)
            if (tempS[0]!='-')
              out << '+';
        out << tempS << PolyFormatLocal.GetLetterIndex(j);
      }
    }
    if (this->Externalwalls.TheObjects[i].flagIsClosed)
    { if (useLatex)
        out << "& \\geq & 0\\\\";
      if (useHtml)
        out << " >= 0\n<br>\n";
      if (!(useHtml || useLatex))
        out << ">=0\n";
    } else
    { if (useLatex)
        out << "& > & 0\\\\";
      if (useHtml)
        out << " > 0\n<br>\n";
      if (!(useHtml || useLatex))
        out << ">0\n";
    }
  }
  if (useLatex)
    out << "\\end{eqnarray*}";
  output=out.str();
}

bool CombinatorialChamberContainer::oneStepChamberSlice(GlobalVariables& theGlobalVariables)
{ if (this->PreferredNextChambers.size==0 && this->NumAffineHyperplanesProcessed<this->NewHyperplanesToSliceWith.size)
  this->NumAffineHyperplanesProcessed++;
  if (!(this->NumAffineHyperplanesProcessed < this->NewHyperplanesToSliceWith.size))
  { this->flagDrawingProjective=false;
//    this->ProjectToDefaultAffineSpace(theGlobalVariables);
    std::stringstream out;
    this->ComputeDebugString();
    out << this->DebugString;
    theGlobalVariables.theIndicatorVariables.StatusString1=out.str();
    return false;
  }
  if (this->PreferredNextChambers.size==0)
    this->SliceWithAWallInitSimple(this->NewHyperplanesToSliceWith.TheObjects[this->NumAffineHyperplanesProcessed], theGlobalVariables);
  else
    this->SliceWithAWallOneIncrement(this->NewHyperplanesToSliceWith.TheObjects[this->NumAffineHyperplanesProcessed], theGlobalVariables);
  std::stringstream out;
  //this->ComputeDebugString();
  out << "Next chamber to slice: " << this->indexNextChamberToSlice;
  out << "Preferred next chambers:  ";
  for (int i=0; i<this->PreferredNextChambers.size; i++)
    out << this->PreferredNextChambers.TheObjects[i] << ",";
  out << "\nProcessed hyperplanes: " << this->NumAffineHyperplanesProcessed  << " out of " << this->NewHyperplanesToSliceWith.size << "\n";
  out << "Number of chamber pointers (including zero pointers): " << this->size;
  //out << this->DebugString;
  theGlobalVariables.theIndicatorVariables.StatusString1=out.str();
  theGlobalVariables.MakeReport();
  return true;
}

void CombinatorialChamberContainer::GetAffineWallImage
  (int indexWeylElement, WallData& input, root& output, GlobalVariables& theGlobalVariables)
{ WeylGroup& currentWeyl=this->AmbientWeyl.GetElement();
  Vector<PolynomialRationalCoeff> tempRoot;
  PolynomialRationalCoeff tempP, PZero;
  PZero.Nullify(this->AmbientDimension);
  tempRoot.SetSize(this->AmbientDimension);
  for (int i=0; i<this->AmbientDimension; i++)
  { tempP.MakeMonomialOneLetter(this->AmbientDimension, i, 1, (Rational) 1);
    tempRoot.TheObjects[i]=tempP;
  }
  this->log << "\nConverting: " << input.normal.ElementToString() << ". Action of element " << indexWeylElement << " on " << tempRoot.ElementToString();
  currentWeyl.ActOn(indexWeylElement, tempRoot, true, true, PZero);
  this->log << ": " << tempRoot.ElementToString();
  output.MakeZero(this->AmbientDimension*2+1);
  for (int i=0; i<this->AmbientDimension; i++)
  { output.TheObjects[i]=input.normal.TheObjects[i];
    PolynomialRationalCoeff& tempP=tempRoot.TheObjects[i];
    for (int j=0; j< tempP.size; j++)
    { Monomial<Rational>& tempMon=tempP.TheObjects[j];
      int theVarIndex;
      if (tempMon.IsAConstant())
        (*output.LastObject())+=-tempMon.Coefficient*output.TheObjects[i];
      else
      { if (!tempMon.IsOneLetterFirstDegree(theVarIndex))
          assert(false);
        output.TheObjects[theVarIndex+this->AmbientDimension]+=-tempMon.Coefficient*output.TheObjects[i];
      }
    }
  }
}

void ParserNode::EvaluateWeylAction
  (GlobalVariables& theGlobalVariables, bool DualAction, bool useRho, bool useMinusRho)
{ if (this->children.size!=1)
  { this->SetError(this->errorProgramming);
    return;
  }
  ParserNode& theArgument=this->owner->TheObjects[this->children.TheObjects[0]];
  HomomorphismSemisimpleLieAlgebra& theHmm= this->owner->theHmm;
  Vector<RationalFunction> theWeight;
  if (theHmm.theRange.GetRank()>1)
  { if (!theArgument.ConvertChildrenToType(this->typeRationalFunction, theGlobalVariables)|| theHmm.theRange.GetRank()!=theArgument.children.size)
    { this->SetError(this->errorBadOrNoArgument);
      return;
    }
    int theDimension=theArgument.children.size;
    theWeight.SetSize(theDimension);
    for (int i=0; i<theDimension; i++)
    { ParserNode& current= this->owner->TheObjects[theArgument.children.TheObjects[i]];
      theWeight.TheObjects[i]=current.ratFunction.GetElement();
    }
  } else
    if (!theArgument.ConvertToType(this->typeRationalFunction, theGlobalVariables))
    { this->SetError(this->errorBadOrNoArgument);
      return;
    } else
    { theWeight.SetSize(1);
      theWeight.TheObjects[0]=theArgument.ratFunction.GetElement();
    }
  std::stringstream out;
  theHmm.theRange.theWeyl.ComputeWeylGroup(51840);
  if (theHmm.theRange.theWeyl.size>=51840)
    out << "Only the first 51840 elements have been computed. <br> If you want a larger computation <br> please use the C++ code directly.";
  out << "Number of elements: " << theHmm.theRange.theWeyl.size << "<br>";
  Vector<RationalFunction> theOrbitElement;
  RationalFunction RFZero;
  RFZero.Nullify(this->owner->NumVariables, &theGlobalVariables);
  for (int i=0; i<theHmm.theRange.theWeyl.size; i++)
  { theOrbitElement=theWeight;
    if (!DualAction)
      theHmm.theRange.theWeyl.ActOn<RationalFunction>(i, theOrbitElement, useRho, useMinusRho, RFZero);
    else
    {
    }
    out << theOrbitElement.ElementToString() << "<br>";
  }
  this->outputString=out.str();
  this->ExpressionType=this->typeString;
}

void CombinatorialChamberContainer::SliceWithAWallInitSimple(root& TheKillerFacetNormal, GlobalVariables& theGlobalVariables)
{ this->flagMakingASingleHyperplaneSlice=true;
  this->flagSliceWithAWallInitDone=false;
  this->PurgeZeroPointers();
  this->PreferredNextChambers.size=0;
  this->LabelAllUnexplored();
  this->PreferredNextChambers.size=0;
  for (int i=0; i<this->size; i++)
    if (this->TheObjects[i]!=0)
      this->PreferredNextChambers.AddObjectOnTop(i);
  this->indexNextChamberToSlice=*this->PreferredNextChambers.LastObject();
  this->PreferredNextChambers.PopLastObject();
  this->flagSliceWithAWallInitDone=true;
}

void CombinatorialChamberContainer::SliceWithAWallInit(root& TheKillerFacetNormal, GlobalVariables& theGlobalVariables)
{ this->flagMakingASingleHyperplaneSlice=true;
  this->flagSliceWithAWallInitDone=false;
  this->PurgeZeroPointers();
  this->PreferredNextChambers.size=0;
  this->LabelAllUnexplored();
  root tempRoot; tempRoot.MakeZero(this->AmbientDimension);
  if (this->flagAnErrorHasOcurredTimeToPanic)
    TheKillerFacetNormal.ComputeDebugString();
  for (int i=0; i<this->size; i++)
  { if (this->flagAnErrorHasOcurredTimeToPanic)
      this->ComputeDebugString();
    if (this->TheObjects[i]!=0)
    { this->indexNextChamberToSlice=i;
      if (this->TheObjects[i]->SplitChamber(TheKillerFacetNormal, *this, tempRoot, theGlobalVariables))
      { delete this->TheObjects[i];
#ifdef CGIversionLimitRAMuse
  ParallelComputing::GlobalPointerCounter--;
  if (ParallelComputing::GlobalPointerCounter>::ParallelComputing::cgiLimitRAMuseNumPointersInList){ std::cout <<"<b>Error:</b> Number of pointers allocated exceeded allowed limit of " <<::ParallelComputing::cgiLimitRAMuseNumPointersInList; std::exit(0); }
#endif
        this->TheObjects[i]=0;
        break;
      }
    }
  }
  this->flagSliceWithAWallInitDone=true;
}

void CombinatorialChamberContainer::SliceWithAWall(root& TheKillerFacetNormal, GlobalVariables& theGlobalVariables)
{ this->SliceWithAWallInitSimple(TheKillerFacetNormal, theGlobalVariables);
  while (this->PreferredNextChambers.size>0)
    this->SliceWithAWallOneIncrement(TheKillerFacetNormal, theGlobalVariables);
  this->PurgeZeroPointers();
  this->PurgeInternalWalls();
}

void CombinatorialChamberContainer::SliceWithAWallOneIncrement(root& TheKillerFacetNormal, GlobalVariables& theGlobalVariables)
{ root tempRoot; tempRoot.MakeZero(this->AmbientDimension);
  if (!this->flagSliceWithAWallInitDone)
    this->SliceWithAWallInitSimple(TheKillerFacetNormal, theGlobalVariables);
  else
    if(this->PreferredNextChambers.size>0)
    { this->indexNextChamberToSlice=this->PreferredNextChambers.TheObjects[0];
      if (this->TheObjects[this->PreferredNextChambers.TheObjects[0]]!=0)
        if (!this->TheObjects[this->PreferredNextChambers.TheObjects[0]]->flagExplored)
          if (this->TheObjects[this->PreferredNextChambers.TheObjects[0]]->SplitChamber(TheKillerFacetNormal, *this, tempRoot, theGlobalVariables))
          { delete this->TheObjects[this->PreferredNextChambers.TheObjects[0]];
  #ifdef CGIversionLimitRAMuse
    ParallelComputing::GlobalPointerCounter--;
    if (ParallelComputing::GlobalPointerCounter>::ParallelComputing::cgiLimitRAMuseNumPointersInList){ std::cout <<"<b>Error:</b> Number of pointers allocated exceeded allowed limit of " <<::ParallelComputing::cgiLimitRAMuseNumPointersInList; std::exit(0); }
  #endif
            this->TheObjects[this->PreferredNextChambers.TheObjects[0]]=0;
          }
      this->PreferredNextChambers.PopIndexShiftUp(0);
      if (this->flagAnErrorHasOcurredTimeToPanic)
        this->ComputeDebugString();
    }
}

void GeneralizedVermaModuleCharacters::GetSubFromNonParamArray
(QPSub& output, roots& NonParams, int numParams)
{ int numNonParams=NonParams.size;
  MatrixLargeRational subRationalForm;
  subRationalForm.init(numParams, numParams+numNonParams-1);
  subRationalForm.NullifyAll();
  for (int k=0; k<numParams; k++)
    for (int l=0; l<numNonParams; l++)
      subRationalForm.elements[k][l]= NonParams.TheObjects[l].TheObjects[k];
  for (int l=0; l<numParams-1; l++)
    subRationalForm.elements[l][l+numNonParams]= 1;
  output.MakeSubFromMatrixRational(subRationalForm);
}

void GeneralizedVermaModuleCharacters::FindMultiplicitiesExtrema(GlobalVariables& theGlobalVariables)
{ if (!this->flagUsingNewSplit)
    return;
  this->paramSubChambers.SetSize(this->projectivizedChamberTest.size);
  this->nonParamVertices.SetSize(this->projectivizedChamberTest.size);
  int theDimension=6;
  if (this->projectivizedChamberTest.size>0)
    theDimension=this->projectivizedChamberTest.TheObjects[0].Normals.TheObjects[0].size;
  this->ExtremeQPsParamSubchambers.SetSize(this->projectivizedChamberTest.size);
  this->allParamSubChambersRepetitionsAllowed.MakeActualSizeAtLeastExpandOnTop(this->projectivizedChamberTest.size*theDimension);
  this->allParamSubChambersRepetitionsAllowed.size=0;
  std::stringstream out;
  QPSub subForFindingExtrema;
  QuasiPolynomial currentExtremaCandidate;
  int numParams=0, numNonParams=0;
  if (this->theLinearOperators.size>0)
  { numParams=this->theLinearOperators.TheObjects[0].NumCols+1;
    numNonParams=this->theLinearOperators.TheObjects[0].NumRows;
  }
//  List<QPSub> tempQPSubList;
  this->theMultiplicitiesExtremaCandidates.MakeActualSizeAtLeastExpandOnTop(this->projectivizedChamberTest.size*theDimension);
  for (int i=0; i<this->projectivizedChamberTest.size; i++)
  { List<roots>& currentParamChamberList=this->paramSubChambers.TheObjects[i];
    List<roots>& currentNonParamVerticesList=this->nonParamVertices.TheObjects[i];
    theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=false;
    std::stringstream progressReport1;
    progressReport1 << "processing chamber " << i+1;
    theGlobalVariables.theIndicatorVariables.ProgressReportString1=progressReport1.str();
    theGlobalVariables.theIndicatorVariables.String1NeedsRefresh=true;
    theGlobalVariables.MakeReport();
    this->ProcessOneParametricChamber
    (this->theLinearOperators.TheObjects[0].NumRows, this->theLinearOperators.TheObjects[0].NumCols+1, this->projectivizedChamberTest.TheObjects[i].Normals,
    currentParamChamberList, currentNonParamVerticesList, theGlobalVariables);
    this->allParamSubChambersRepetitionsAllowed.AddListOnTop(currentParamChamberList);
//    if (i<this->tempDebugCounter)
    for (int j=0; j<currentNonParamVerticesList.size; j++)
    { this->GetSubFromNonParamArray(subForFindingExtrema, currentNonParamVerticesList.TheObjects[j], numParams);
     // tempQPSubList.AddObjectOnTop(subForFindingExtrema);
      this->theMultiplicities.TheObjects[i].RationalLinearSubstitution(subForFindingExtrema, currentExtremaCandidate);
      this->theMultiplicitiesExtremaCandidates.AddObjectOnTop(currentExtremaCandidate);
    }

    this->ExtremeQPsParamSubchambers.TheObjects[i].SetSize(currentParamChamberList.size);
    for (int j=0; j< currentParamChamberList.size; j++)
    { std::stringstream progressReport2;
      progressReport2 << "processing chamber " << i+1 << " subchamber " << j+1 << " out of " << currentParamChamberList.size << " Total: " << this->allParamSubChambersRepetitionsAllowed.size;
      theGlobalVariables.theIndicatorVariables.ProgressReportString2=progressReport2.str();
      theGlobalVariables.theIndicatorVariables.String2NeedsRefresh=true;
      theGlobalVariables.MakeReport();
      roots& currentParamChamber=currentParamChamberList.TheObjects[j];
      roots& currentNonParamVertices=currentNonParamVerticesList.TheObjects[j];
      out << "\nChamber: " << currentParamChamber.ElementsToInequalitiesString(false, false);
      out << "\nVertices: " << currentNonParamVertices.ElementToStringLetterFormat("x", false);
    }
  }
  std::stringstream out2;
//  for (int i=0; i<tempQPSubList.size; i++)
//  { out2 << " Chamber candidate " << i+1 << " the Sub:\n" << tempQPSubList.TheObjects[i].ElementToString() << "\n";
 // }
  this->allParamSubChambersRepetitionsAllowedConeForm.SetSize(this->allParamSubChambersRepetitionsAllowed.size);
  for (int i=0; i<this->allParamSubChambersRepetitionsAllowed.size; i++)
  { Cone& currentCone= this->allParamSubChambersRepetitionsAllowedConeForm.TheObjects[i];
    currentCone.CreateFromNormals(this->allParamSubChambersRepetitionsAllowed.TheObjects[i], theGlobalVariables);
    std::stringstream out4;
    out4 << "preparing starting cones: " << i+1 << " out of " << this->allParamSubChambersRepetitionsAllowed.size;
    theGlobalVariables.theIndicatorVariables.StatusString1=out4.str();
    theGlobalVariables.MakeReport();
  }
  this->projectivizedParamComplex.initFromCones(this->allParamSubChambersRepetitionsAllowed, theGlobalVariables);

  //this->projectivizedParamComplex.Refine(theGlobalVariables);
  this->theMultiplicitiesExtremaCandidates.SetSize(this->projectivizedParamComplex.size);
  std::stringstream out3;
  for (int i=0; i<this->projectivizedParamComplex.size; i++)
  if (i<this->tempDebugCounter)
  { Cone& currentCone= this->projectivizedParamComplex.TheObjects[i];
    out3 << "\n\n\nChamber " << i+1 << " the extreme multiplicities are among \n ";
    this->ProcessExtremaOneChamber(currentCone, theGlobalVariables);
  }
  theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
//  theGlobalVariables.theIndicatorVariables.StatusString1=this->projectivizedParamComplex.ElementToString();
//  theGlobalVariables.theIndicatorVariables.StatusString1=out2.str();
  //theGlobalVariables.theIndicatorVariables.StatusString1=out3.str();
  theGlobalVariables.MakeReport();
}
void GeneralizedVermaModuleCharacters::ProcessExtremaOneChamber
  (Cone& input, GlobalVariables& theGlobalVariables)
{ std::stringstream out3;
  out3 << "extreme multiplicities are among: ";
  for (int j=0; j<this->allParamSubChambersRepetitionsAllowedConeForm.size; j++)
  // if (j<this->tempDebugCounter)
    if (input.IsInCone(this->allParamSubChambersRepetitionsAllowedConeForm.TheObjects[j].GetInternalPoint()))
    { out3 << this->theMultiplicitiesExtremaCandidates.TheObjects[j].ElementToString() << "\n";
      std::stringstream out4;
      out4 << "starting chamber " << j+1 << " out of " << this->allParamSubChambersRepetitionsAllowedConeForm.size;
      theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
      theGlobalVariables.theIndicatorVariables.StatusString1=out4.str();
      theGlobalVariables.MakeReport();
    }
  theGlobalVariables.theIndicatorVariables.StatusString1=out3.str();
  theGlobalVariables.MakeReport();
}

bool ParserNode::ExtractArgumentList
(List<int>& outputArgumentIndices)
{ if (this->children.size!=1)
    return false;
  ParserNode& theArgument=this->owner->TheObjects[this->children.TheObjects[0]];
  if (theArgument.ExpressionType==this->typeArray)
    outputArgumentIndices=theArgument.array.GetElement();
  else
    outputArgumentIndices.initFillInObject(1, this->children.TheObjects[0]);
  return true;
}

int ParserNode::EvaluateChamberParam(GlobalVariables& theGlobalVariables)
{ List<int> theArguments;
  this->ExtractArgumentList(theArguments);
  GeneralizedVermaModuleCharacters tempChars;
  if (theArguments.size<=2)
    return this->SetError(this->errorBadOrNoArgument);
  if (!this->owner->TheObjects[theArguments.TheObjects[0]].ConvertToType(this->typeIntegerOrIndex, theGlobalVariables) || !this->owner->TheObjects[theArguments.TheObjects[1]].ConvertToType(this->typeIntegerOrIndex, theGlobalVariables))
    return this->SetError(this->errorBadOrNoArgument);
  int dimNonParam=this->owner->TheObjects[theArguments.TheObjects[0]].intValue;
  int dimParam=this->owner->TheObjects[theArguments.TheObjects[1]].intValue;
  roots theWalls;
  root tempRoot;
  List<int> nodesCurrentRoot;
  for (int i=2; i<theArguments.size; i++)
  { ParserNode& currentNode=this->owner->TheObjects[theArguments.TheObjects[i]];
    if (!currentNode.ConvertToType(this->typeArray, theGlobalVariables))
      return this->SetError(this->errorBadOrNoArgument);
    nodesCurrentRoot= currentNode.array.GetElement();
    if (nodesCurrentRoot.size!=dimNonParam+dimParam)
      return this->SetError(this->errorBadOrNoArgument);
    tempRoot.SetSize(nodesCurrentRoot.size);
    for (int j=0; j<nodesCurrentRoot.size; j++)
    { ParserNode& currentCoordinateNode=this->owner->TheObjects[nodesCurrentRoot.TheObjects[j]];
      if (!currentCoordinateNode.ConvertToType(this->typeRational, theGlobalVariables))
        return this->SetError(this->errorBadOrNoArgument);
      tempRoot.TheObjects[j]=currentCoordinateNode.rationalValue;
    }
    theWalls.AddObjectOnTop(tempRoot);
  }
  List<roots> outputParamChambers, outputNonParamVertices;
  tempChars.ProcessOneParametricChamber(dimNonParam, dimParam, theWalls, outputParamChambers, outputNonParamVertices, theGlobalVariables);
  std::stringstream out;
  out << "<div class=\"math\">" << theWalls.ElementsToInequalitiesString(true, false) << "</div>";
  this->outputString=out.str();
  this->ExpressionType=(this->typeString);
  return this->errorNoError;
}

void GeneralizedVermaModuleCharacters::ProcessOneParametricChamber
  (int numNonParams, int numParams, roots& inputNormals, List<roots>& theParamChambers, List<roots>& theNonParamVertices, GlobalVariables& theGlobalVariables)
{ /*roots nonParametricPart, parametricPart;
  nonParametricPart.SetSize(input.Externalwalls.size);
  parametricPart.SetSize(input.Externalwalls.size);
  for (int i=0; i<input.Externalwalls.size; i++)
  { root& currentNP=nonParametricPart.TheObjects[i];
    root& currentP= parametricPart.TheObjects[i];
    currentNP.SetSize(numNonParams);
    currentP.SetSize(numParams);
    for (int j=0; j<numNonParams; j++)
      currentNP.TheObjects[j]=input.Externalwalls.TheObjects[i].normal.TheObjects[j];
    for (int j=0; j<numParams; j++)
      currentNP.TheObjects[j]=input.Externalwalls.TheObjects[i].normal.TheObjects[numNonParams+j];
  }*/
  Selection selectedNormals;
  selectedNormals.init(inputNormals.size);
  int numCycles=MathRoutines::NChooseK(inputNormals.size, numNonParams);
  MatrixLargeRational matrixNonParams, matrixParams;
  matrixNonParams.init(inputNormals.size, numNonParams);
  matrixParams.init(inputNormals.size, numParams);
  Selection NonPivotPoints;
  roots basisRoots;
  basisRoots.SetSize(numNonParams);
  roots inducedParamChamber;
  roots nonParamVertices;
  inducedParamChamber.SetSize(inputNormals.size-numNonParams);
  nonParamVertices.SetSize(numNonParams);
  theParamChambers.MakeActualSizeAtLeastExpandOnTop(numCycles);
  theNonParamVertices.MakeActualSizeAtLeastExpandOnTop(numCycles);
  theParamChambers.size=0;
  theNonParamVertices.size=0;
  for (int i=0; i<numCycles; i++)
  { selectedNormals.incrementSelectionFixedCardinality(numNonParams);
    for (int j=0; j<numNonParams; j++)
    { root& currentNormal=inputNormals.TheObjects[selectedNormals.elements[j]];
      root& currentBasisElt=basisRoots.TheObjects[j];
      currentBasisElt.SetSize(numNonParams);
      for (int k=0; k<numNonParams; k++)
      { matrixNonParams.elements[j][k]=currentNormal.TheObjects[k];
        currentBasisElt.TheObjects[k]=currentNormal.TheObjects[k];
      }
      for (int k=0; k<numParams; k++)
        matrixParams.elements[j][k]=currentNormal.TheObjects[k+numNonParams];
    }
    if (basisRoots.GetRankOfSpanOfElements(theGlobalVariables)==numNonParams)
    { for (int j=0, counter=numNonParams; j<inputNormals.size; j++)
        if (!selectedNormals.selected[j])
        { root& currentNormal=inputNormals.TheObjects[j];
          for (int k=0; k<numNonParams; k++)
            matrixNonParams.elements[counter][k]=currentNormal.TheObjects[k];
          for (int k=0; k<numParams; k++)
            matrixParams.elements[counter][k]=currentNormal.TheObjects[k+numNonParams];
          counter++;
        }
      MatrixLargeRational::GaussianEliminationByRows(matrixNonParams, matrixParams, NonPivotPoints);
      for (int j=0; j<numNonParams; j++)
        matrixParams.RowToRoot(j, nonParamVertices.TheObjects[j]);
      for (int j=0; j<inputNormals.size-numNonParams; j++)
        matrixParams.RowToRoot(j+numNonParams, inducedParamChamber.TheObjects[j]);
      theNonParamVertices.AddObjectOnTop(nonParamVertices);
      theParamChambers.AddObjectOnTop(inducedParamChamber);
    }
  }
  for (int i=0; i< theNonParamVertices.size; i++)
  { std::cout << "<br>Number " << i+1 << ", vertices: " << theNonParamVertices.TheObjects[i].ElementToString();
    std::cout << "<br>the parametric chamber: ";
    std::cout << "<div class=\"math\">" << theParamChambers.TheObjects[i].ElementsToInequalitiesString(true, false) << "</div>";
  }
}

void ConeComplex::GetNewVerticesAppend
  (Cone& myDyingCone, root& killerNormal, hashedRoots& outputVertices, GlobalVariables& theGlobalVariables)
{ int theDimMinusTwo=killerNormal.size-2;
  int theDim=killerNormal.size;
  int numCycles=MathRoutines::NChooseK(myDyingCone.Normals.size, theDimMinusTwo);
  Selection& theSel=theGlobalVariables.selComputeNormalExcludingIndex;
  Selection& nonPivotPoints=theGlobalVariables.selNonPivotPointsNewSplit;
  theSel.init(myDyingCone.Normals.size);
//  int IndexLastZeroWithOneBefore, NumOnesAfterLastZeroWithOneBefore;
  MatrixLargeRational& theLinearAlgebra=theGlobalVariables.matComputeNormalExcludingIndex;
  MatrixLargeRational matEmpty;
  theLinearAlgebra.init(theDimMinusTwo+1, theDim);
  root tempRoot;
  for (int i=0; i<numCycles; i++)
  { theSel.incrementSelectionFixedCardinality(theDimMinusTwo);//, IndexLastZeroWithOneBefore, NumOnesAfterLastZeroWithOneBefore);
    //int LowestRowUnchanged=theDimMinusTwo-2-NumOnesAfterLastZeroWithOneBefore;
    //for (int j=theDimMinusTwo-1; j>LowestRowUnchanged; j--)
    for (int j=0; j<theDimMinusTwo; j++)
    { root& currentNormal=myDyingCone.Normals.TheObjects[theSel.elements[j]];
      for (int k=0; k<theDim; k++)
        theLinearAlgebra.elements[j][k]=currentNormal.TheObjects[k];
    }
    for (int k=0; k<theDim; k++)
      theLinearAlgebra.elements[theDimMinusTwo][k]=killerNormal.TheObjects[k];
    theLinearAlgebra.GaussianEliminationByRows(theLinearAlgebra, matEmpty, nonPivotPoints);
    if (nonPivotPoints.CardinalitySelection==1)
    { theLinearAlgebra.NonPivotPointsToEigenVector(nonPivotPoints, tempRoot, (Rational) 1, (Rational) 0);
      tempRoot.ScaleToIntegralMinHeight();
      if (myDyingCone.IsInCone(tempRoot))
        outputVertices.AddObjectOnTopNoRepetitionOfObjectHash(tempRoot);
      else
      { tempRoot.MinusRoot();
        if (myDyingCone.IsInCone(tempRoot))
          outputVertices.AddObjectOnTopNoRepetitionOfObjectHash(tempRoot);
      }
    }
  }
}

bool ConeComplex::SplitChamber
(int indexChamberBeingRefined, root& killerNormal, GlobalVariables& theGlobalVariables)
{ Cone& myDyingCone=this->TheObjects[indexChamberBeingRefined];
  if (!myDyingCone.flagHasSufficientlyManyVertices)
  { this->flagChambersHaveTooFewVertices=true;
    return false;
  }
  Cone& newPlusCone= theGlobalVariables.coneBuffer1NewSplit;
  Cone& newMinusCone=theGlobalVariables.coneBuffer2NewSplit;
  newPlusCone.flagHasSufficientlyManyVertices=true;
  newMinusCone.flagHasSufficientlyManyVertices=true;
  newPlusCone.LowestIndexIHaventBeenCheckedForSplitting=myDyingCone.LowestIndexIHaventBeenCheckedForSplitting+1;
  newMinusCone.LowestIndexIHaventBeenCheckedForSplitting=myDyingCone.LowestIndexIHaventBeenCheckedForSplitting+1;
  newPlusCone.Vertices.size=0; newPlusCone.Normals.size=0;
  newMinusCone.Vertices.size=0; newMinusCone.Normals.size=0;
  hashedRoots& ZeroVertices=theGlobalVariables.hashedRootsNewChamberSplit;
  ZeroVertices.ClearTheObjects();
  Rational tempRat;
  for (int i=0; i<myDyingCone.Vertices.size; i++)
  { root::RootScalarEuclideanRoot(killerNormal, myDyingCone.Vertices.TheObjects[i], tempRat);
    if (tempRat.IsPositive())
      newPlusCone.Vertices.AddObjectOnTop(myDyingCone.Vertices.TheObjects[i]);
    if (tempRat.IsEqualToZero())
      ZeroVertices.AddObjectOnTopNoRepetitionOfObjectHash(myDyingCone.Vertices.TheObjects[i]);
    if (tempRat.IsNegative())
      newMinusCone.Vertices.AddObjectOnTop(myDyingCone.Vertices.TheObjects[i]);
  }
  if (newPlusCone.Vertices.size==0 || newMinusCone.Vertices.size==0)
    return false;
  this->GetNewVerticesAppend(myDyingCone, killerNormal, ZeroVertices, theGlobalVariables);
  for (int i=0; i<myDyingCone.Normals.size; i++)
  { if (newPlusCone.Vertices.HasAnElementPerpendicularTo(myDyingCone.Normals.TheObjects[i]))
      newPlusCone.Normals.AddObjectOnTop(myDyingCone.Normals.TheObjects[i]);
    if (newMinusCone.Vertices.HasAnElementPerpendicularTo(myDyingCone.Normals.TheObjects[i]))
      newMinusCone.Normals.AddObjectOnTop(myDyingCone.Normals.TheObjects[i]);
  }
  newPlusCone.Normals.AddObjectOnTop(killerNormal);
  newMinusCone.Normals.AddObjectOnTop(-killerNormal);
  newPlusCone.Vertices.AddListOnTop(ZeroVertices);
  newMinusCone.Vertices.AddListOnTop(ZeroVertices);
/*  Cone tempCone;
  tempCone.CreateFromNormals(newPlusCone.Normals, theGlobalVariables);
  assert(tempCone.Vertices.size==newPlusCone.Vertices.size);
  tempCone.CreateFromNormals(newMinusCone.Normals, theGlobalVariables);
  assert(tempCone.Vertices.size==newMinusCone.Vertices.size);
*/
  this->PopChamberSwapWithLast(indexChamberBeingRefined);
  this->AddNonRefinedChamberOnTopNoRepetition(newPlusCone);
  this->AddNonRefinedChamberOnTopNoRepetition(newMinusCone);
  return true;
}

void ConeComplex::PopChamberSwapWithLast(int index)
{ this->PopIndexSwapWithLastHash(index);
}

void ConeComplex::AddNonRefinedChamberOnTopNoRepetition(Cone& newCone)
{ newCone.Normals.QuickSortAscending();
  this->AddObjectOnTopNoRepetitionOfObjectHash(newCone);
}

void ConeComplex::RefineOneStep(GlobalVariables& theGlobalVariables)
{ if (this->indexLowestNonRefinedChamber>=this->size)
    return;
  Cone& currentCone=this->TheObjects[this->indexLowestNonRefinedChamber];
  for (; currentCone.LowestIndexIHaventBeenCheckedForSplitting<this->splittingNormals.size; currentCone.LowestIndexIHaventBeenCheckedForSplitting++)
    if (this->SplitChamber(this->indexLowestNonRefinedChamber, this->splittingNormals.TheObjects[currentCone.LowestIndexIHaventBeenCheckedForSplitting], theGlobalVariables))
      return;
    this->indexLowestNonRefinedChamber++;
}

void ConeComplex::Refine(GlobalVariables& theGlobalVariables)
{ while (this->indexLowestNonRefinedChamber<this->size)
  { this->RefineOneStep(theGlobalVariables);
    std::stringstream out;
    out << "Refined " << this->indexLowestNonRefinedChamber << " out of " << this->size;
    theGlobalVariables.theIndicatorVariables.StatusString1=out.str();
    theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
    theGlobalVariables.MakeReport();
  }
}

bool Cone::CreateFromNormals
  (roots& inputNormals, GlobalVariables& theGlobalVariables)
{ this->Normals.CopyFromBase(inputNormals);
//o  this->Data=inputData;
//  roots& candidateVertices=theGlobalVariables.rootsGeneralPurposeBuffer1;
  Selection theSel, nonPivotPoints;
  this->Vertices.size=0;
  if (this->Normals.size==0)
    return false;
  for (int i=0; i<this->Normals.size; i++)
    this->Normals.TheObjects[i].ScaleToIntegralMinHeight();
  int theDim=this->Normals.TheObjects[0].size;
  theSel.init(this->Normals.size);
  int numCycles=theSel.GetNumCombinationsFixedCardinality(theDim-1);
  MatrixLargeRational& theMat=theGlobalVariables.matComputeNormalFromSelection;
  MatrixLargeRational emptyMat;
  root tempRoot;
  theMat.init(theDim-1, theDim);
  for (int i=0; i<numCycles; i++)
  { theSel.incrementSelectionFixedCardinality(theDim-1);
    for (int j=0; j<theSel.CardinalitySelection; j++)
      for (int k=0; k<theDim; k++)
        theMat.elements[j][k]=this->Normals.TheObjects[theSel.elements[j]].TheObjects[k];
    theMat.GaussianEliminationByRows(theMat, emptyMat, nonPivotPoints);
    if (nonPivotPoints.CardinalitySelection==1)
    { theMat.NonPivotPointsToRoot(nonPivotPoints, tempRoot);
      bool tempBool=this->IsInCone(tempRoot);
      if (!tempBool)
      { tempRoot.MinusRoot();
        tempBool=this->IsInCone(tempRoot);
      }
      if (tempBool)
      { tempRoot.ScaleToIntegralMinHeight();
        this->Vertices.AddOnTopNoRepetition(tempRoot);
      }
    }
  }
  //time to eliminate possible fake walls
  roots& verticesOnWall=theGlobalVariables.rootsGeneralPurposeBuffer2;
  for (int i=0; i<this->Normals.size; i++)
  { root& currentNormal=this->Normals.TheObjects[i];
    verticesOnWall.size=0;
    bool wallIsGood=false;
    for (int j=0; j<this->Vertices.size; j++)
      if (root::RootScalarEuclideanRoot(this->Vertices.TheObjects[j], currentNormal).IsEqualToZero())
      { verticesOnWall.AddObjectOnTop(this->Vertices.TheObjects[j]);
        int theRank=verticesOnWall.GetRankOfSpanOfElements(theGlobalVariables);
        if (theRank< verticesOnWall.size)
          verticesOnWall.PopLastObject();
        else
          if (theRank==theDim-1)
          { wallIsGood=true;
            break;
          }
      }
    if (!wallIsGood)
    { this->Normals.PopIndexSwapWithLast(i);
      i--;
    }
  }
  if (this->Normals.size==0 || this->Vertices.size==0)
    return false;
  this->Normals.QuickSortAscending();
  //this->ComputeDebugString();
  for (int i=0; i<this->Normals.size; i++)
    if (!(this->Vertices.HasAnElementWithNegativeScalarProduct(this->Normals.TheObjects[i])||this->Vertices.HasAnElementWithPositiveScalarProduct(this->Normals.TheObjects[i])))
      return false;
  return true;
}

void ConeComplex::initFromCones
(List<roots>& NormalsOfCones, GlobalVariables& theGlobalVariables)
{ Cone tempCone;
  this->ClearTheObjects();
  theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
  theGlobalVariables.theIndicatorVariables.StatusString1=NormalsOfCones.ElementToStringGeneric();
  theGlobalVariables.MakeReport();
  theGlobalVariables.MakeReport();
//  for (int i=0; i<10000000; i++){int j=i*i*i;}
  for (int i=0; i<NormalsOfCones.size; i++)
  { if (tempCone.CreateFromNormals(NormalsOfCones.TheObjects[i], theGlobalVariables))
      this->AddNonRefinedChamberOnTopNoRepetition(tempCone);
    std::stringstream out;
    out << "Initializing cone " << i+1 << " out of " << NormalsOfCones.size;
    theGlobalVariables.theIndicatorVariables.StatusString1=out.str();
    theGlobalVariables.MakeReport();
  }
  root tempRoot;
  this->splittingNormals.ClearTheObjects();
  for (int i=0; i<this->size; i++)
    for (int j=0; j<this->TheObjects[i].Normals.size; j++)
    { tempRoot=this->TheObjects[i].Normals.TheObjects[j];
      tempRoot.ScaleToIntegralMinHeightFirstNonZeroCoordinatePositive();
      this->splittingNormals.AddObjectOnTopNoRepetitionOfObjectHash(tempRoot);
      std::stringstream out;
      out << "Extracting walls from cone " << i+1 << " out of " << this->size << " total distinct chambers.";
      out << "\nProcessed " << j+1 << " out of " << this->TheObjects[i].Normals.size << " walls of the current chamber.";
      out << "\nTotal # of distinct walls found: " << this->splittingNormals.size;
      theGlobalVariables.theIndicatorVariables.StatusString1=out.str();
      theGlobalVariables.MakeReport();
    }
}

std::string Cone::ElementToString(bool useLatex, bool useHtml)
{ std::stringstream out;
  out << "Index next wall to refine by: " << this->LowestIndexIHaventBeenCheckedForSplitting << "\n";
  if (useHtml)
    out << "<br>";
  out << "Normals:\n";
  if (useHtml)
    out << "<br>";
  out << this->Normals.ElementsToInequalitiesString(useLatex, useHtml);
  out << "\nVertices:" << this->Vertices.ElementToString();
  return out.str();
}

std::string ConeComplex::ElementToString(bool useLatex, bool useHtml)
{ std::stringstream out;
  out << "Number of chambers: " << this->size;
  if (useHtml)
    out << "<br>";
  out  << " Next non-refined chamber: " << this->indexLowestNonRefinedChamber+1;
  if (useHtml)
    out << "<br>";
  out << "Normals of walls to refine by: ";
  roots tempRoots;
  tempRoots.CopyFromBase(this->splittingNormals);
  out << tempRoots.ElementToString(useLatex, useHtml, false);
  for (int i=0; i<this->size; i++)
  { if (useHtml)
      out << "<br>";
    out << "\nChamber " << i+1 << ":\n";
    if (useHtml)
      out << "<br>";
    out << this->TheObjects[i].ElementToString(useLatex, useHtml) << "\n\n\n";
  }
  return out.str();
}

int ParserNode::EvaluateCone(GlobalVariables& theGlobalVariables)
{ List<int> argumentsList;
  this->ExtractArgumentList(argumentsList);
  int theDim=-1;
  roots coneWalls; root currentWall;
  for (int i=0; i<argumentsList.size; i++)
  { ParserNode& currentNode=this->owner->TheObjects[argumentsList.TheObjects[i]];
    if (!currentNode.ConvertToType(this->typeArray, theGlobalVariables))
      return this->SetError(this->errorBadOrNoArgument);
    if (theDim==-1)
      theDim=currentNode.array.GetElement().size;
    if (currentNode.array.GetElement().size!=theDim)
      return this->SetError(this->errorDimensionProblem);
    currentWall.SetSize(theDim);
    for (int j=0; j<currentNode.array.GetElement().size; j++)
    { ParserNode& currentCoord=this->owner->TheObjects[currentNode.array.GetElement().TheObjects[j]];
      if (!currentCoord.ConvertToType(this->typeRational, theGlobalVariables))
        return this->SetError(this->errorBadOrNoArgument);
      currentWall.TheObjects[j]=currentCoord.rationalValue;
    }
    coneWalls.AddObjectOnTop(currentWall);
  }
  std::stringstream out;
  if (!this->theCone.GetElement().CreateFromNormals(coneWalls, theGlobalVariables))
    out << "the cone has too few or no vertices.";
  out << "<br>Cone walls input: " << coneWalls.ElementToString(false, false, false);
  out << "<br><br>Cone:" << this->theCone.GetElement().ElementToString(false, true);
  this->outputString=out.str();

  this->ExpressionType=this->typeCone;
  return this->errorNoError;
}

int ParserNode::EvaluateMaxLFOverCone(GlobalVariables& theGlobalVariables)
{ List<int> argumentList;
  this->ExtractArgumentList(argumentList);
  if (argumentList.size<2)
    return this->SetError(this->errorBadOrNoArgument);
  List<PolynomialRationalCoeff> thePolys;
  thePolys.SetSize(argumentList.size-1);
  for (int i=0; i<argumentList.size-1; i++)
  { ParserNode& currentNode=this->owner->TheObjects[argumentList.TheObjects[i]];
    if (!currentNode.ConvertToType(this->typePoly, theGlobalVariables))
      return this->SetError(this->errorBadOrNoArgument);
    thePolys.TheObjects[i]=currentNode.polyValue.GetElement();
  }
  ParserNode& coneNode=this->owner->TheObjects[*argumentList.LastObject()];
  if (!coneNode.ConvertToType(this->typeCone, theGlobalVariables))
    return this->SetError(this->errorBadOrNoArgument);
  Cone currentCone;
  currentCone=coneNode.theCone.GetElement();
  std::stringstream out;
  out << "input polys: ";
  for (int i=0; i<thePolys.size; i++)
    out <<  thePolys.TheObjects[i].ElementToString() << "<br>";
  out << "<br>The cone: " << currentCone.ElementToString();
  ConeComplex theComplex;
  List<int> output;
  theComplex.findMaxLFOverConeProjective(currentCone, thePolys, output, theGlobalVariables);
  out << "<br>" << theComplex.ElementToString(false, true);
  this->outputString=out.str();
  this->ExpressionType=this->typeString;
  return this->errorNoError;
}

bool ConeComplex::findMaxLFOverConeProjective
  (Cone& input, List<PolynomialRationalCoeff>& inputLinPolys, List<int>& outputMaximumOverEeachSubChamber, GlobalVariables& theGlobalVariables)
{ this->init();
  this->AddNonRefinedChamberOnTopNoRepetition(input);
  roots HyperPlanesCorrespondingToLF;
  if (input.Normals.size<1)
    return false;
  int theDim=input.Normals.TheObjects[0].size;
  HyperPlanesCorrespondingToLF.SetSize(inputLinPolys.size);
  for (int i=0; i<inputLinPolys.size; i++)
  { PolynomialRationalCoeff& currentPoly=inputLinPolys.TheObjects[i];
    if (currentPoly.TotalDegree()!=1 || theDim!=currentPoly.NumVars)
      return false;
    root& newWall=HyperPlanesCorrespondingToLF.TheObjects[i];
    newWall.MakeZero(theDim);
    for (int j=0; j<currentPoly.size; j++)
      for (int k=0; k<theDim; k++)
        if (currentPoly.TheObjects[j].degrees[k]==1)
        { newWall.TheObjects[k]=currentPoly.TheObjects[j].Coefficient;
          break;
        }
  }
  root tempRoot;
  for (int i=0; i<HyperPlanesCorrespondingToLF.size; i++)
    for (int j=i+1; j<HyperPlanesCorrespondingToLF.size; j++)
    { tempRoot=HyperPlanesCorrespondingToLF.TheObjects[i]-HyperPlanesCorrespondingToLF.TheObjects[j];
      tempRoot.ScaleToIntegralMinHeightFirstNonZeroCoordinatePositive();
      if (!tempRoot.IsEqualToZero())
        this->splittingNormals.AddObjectOnTopNoRepetitionOfObjectHash(tempRoot);
    }
  std::cout << this->ElementToString(false, true);
  this->Refine(theGlobalVariables);
  return true;
}

void RationalFunction::AddHonestRF
(const RationalFunction& other)
{ Rational tempRat;
  if (!this->Denominator.GetElement().IsProportionalTo(other.Denominator.GetElementConst(), tempRat, (Rational) 1))
  { PolynomialRationalCoeff buffer;
    RationalFunction debugger;
    debugger=other;
    debugger.ComputeDebugString();
    this->ComputeDebugString();
    buffer=this->Denominator.GetElement();
    this->Numerator.GetElement().MultiplyBy(other.Denominator.GetElementConst());
    buffer.MultiplyBy(other.Numerator.GetElementConst());
    this->Numerator.GetElement().AddPolynomial(buffer);
    this->Denominator.GetElement().MultiplyBy(other.Denominator.GetElementConst());
    this->Simplify();
    this->ComputeDebugString();
  } else
  { this->Numerator.GetElement().TimesConstant(tempRat);
    this->Denominator.GetElement().TimesConstant(tempRat);
    this->Numerator.GetElement().AddPolynomial(other.Numerator.GetElementConst());
    this->ReduceMemory();
    this->SimplifyLeadingCoefficientOnly();
  }
  assert(this->checkConsistency());
}

void ParserNode::EvaluateTimes(GlobalVariables& theGlobalVariables)
{ if (!this->AllChildrenAreOfDefinedNonErrorType())
  { this->SetError(this->errorOperationByUndefinedOrErrorType);
    return;
  }
  this->ConvertChildrenAndMyselfToStrongestExpressionChildren(theGlobalVariables);
  this->InitForMultiplication(&theGlobalVariables);
  LargeInt theInt;
  for (int i=0; i<this->children.size; i++)
  { ParserNode& currentChild=this->owner->TheObjects[this->children.TheObjects[i]];
    switch (this->ExpressionType)
    { case ParserNode::typeIntegerOrIndex:
        theInt=this->intValue;
        theInt*=currentChild.intValue;
        if (theInt.value.size>1)
        { this->ExpressionType= this->typeRational;
          this->rationalValue=theInt;
        } else
          this->intValue=theInt.value.TheObjects[0]*theInt.sign;
      break;
      case ParserNode::typeRational: this->rationalValue*=currentChild.rationalValue; break;
      case ParserNode::typeRationalFunction: this->ratFunction.GetElement()*=currentChild.ratFunction.GetElement(); break;
      case ParserNode::typePoly: this->polyValue.GetElement().MultiplyBy(currentChild.polyValue.GetElement()); break;
      case ParserNode::typeUEelement: this->UEElement.GetElement()*=currentChild.UEElement.GetElement(); break;
      case ParserNode::typeUEElementOrdered: this->UEElementOrdered.GetElement()*=currentChild.UEElementOrdered.GetElement(); break;
      case ParserNode::typeWeylAlgebraElement: this->WeylAlgebraElement.GetElement().MultiplyOnTheRight(currentChild.WeylAlgebraElement.GetElement()); break;
      default: this->SetError(this->errorMultiplicationByNonAllowedTypes); return;
    }
  }
}

void ParserNode::EvaluatePlus(GlobalVariables& theGlobalVariables)
{ if (!this->AllChildrenAreOfDefinedNonErrorType())
  { this->ExpressionType=this->typeError;
    return;
  }
  this->ConvertChildrenAndMyselfToStrongestExpressionChildren(theGlobalVariables);
  this->InitForAddition(&theGlobalVariables);
  LargeInt theInt;
  if (this->children.TheObjects[0]==6 && this->children.TheObjects[1]==14)
  { int x=MathRoutines::NChooseK(15,5);

  }
  for (int i=0; i<this->children.size; i++)
  { ParserNode& currentChild=this->owner->TheObjects[this->children.TheObjects[i]];
    switch (this->ExpressionType)
    { case ParserNode::typeIntegerOrIndex:
        theInt=this->intValue;
        theInt+=currentChild.intValue;
        if (theInt.value.size>1)
        { this->ExpressionType= this->typeRational;
          this->rationalValue=theInt;
        } else
          this->intValue=theInt.value.TheObjects[0]*theInt.sign;
      break;
      case ParserNode::typeRational: this->rationalValue+=currentChild.rationalValue; break;
      case ParserNode::typeRationalFunction: this->ratFunction.GetElement()+=currentChild.ratFunction.GetElement(); break;
      case ParserNode::typePoly: this->polyValue.GetElement().AddPolynomial(currentChild.polyValue.GetElement()); break;
      case ParserNode::typeUEElementOrdered: this->UEElementOrdered.GetElement().operator+=(currentChild.UEElementOrdered.GetElement()); break;
      case ParserNode::typeUEelement: this->UEElement.GetElement()+=currentChild.UEElement.GetElement(); break;
      case ParserNode::typeWeylAlgebraElement: this->WeylAlgebraElement.GetElement().Add(currentChild.WeylAlgebraElement.GetElement()); break;
      default: this->ExpressionType=this->typeError; return;
    }
  }
}

