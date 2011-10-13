#include "polyhedra.h"
// This file is meant to be for people to modify if they do not want to modify the main files polyhedra.cpp or polyhedra.h
// The reason I would recommend that is because the file polyhedra.cpp compiles very slowly (around 90 seconds), so small modifications
// take a long time to check. Using a separate file is the solution I chose (may not be the best, see below for discussion).
// If the code you write here reaches the mature phase where you have realized all functions you think it should have
// and you are generally satisfied with it, simply cut & paste it in the main files (the class declarations in polyhedra.h and the implementation in polyhedra.cpp).

//Discussion: other options for speeding up the compilation that I have considered.
//1. Cut up the main files polyhedra.h and polyhedra.cpp into small quick-to-compile pieces. This might be the best solution in the long run. However, I do not want to
//   do it just yet, because 1) I am not sure what should those parts be - it is not yet clear in my head how to partition the code in conceptually distinct pieces
//   2) this would certainly create additional maintainance work 3) this will increase the learning curve for a person wanting to just use the program and wanting to eventually
//   modify some tiny bit and 4) I got an advice on the c++ forum www.cplusplus.com that partitioning the .h file will eventually lead to slower compile times,
//   especially with the massive use of templates that I do. Therefore, such a partitioning should not be done before the code reaches
//   greater maturity (see also the first paragraph).
//2. Use precompiled headers or some other voodoo. I am tocally against that. Those are compiler specific, will require me to learn extra unnecessary info which might
//    be out of date in a few years, and will make even higher entry learning curve for another to join the project. This is bad.
//    I should mention in favor of Microsoft that their IDE does recompile very quickly small modifications of the file polyhedra.cpp. I believe it does so by
//    keeping a database of your recently changed code, and recompiling only the recently changed pieces. Hats off to Microsoft for doing that completely programmatically,
//    and not bothering the programmer with stupid things such as how to set up precompiled headers.
//
// To whomever might be reading this (if anyone): happy hacking and I hope you find my code useful, that it didn't cause you many headaches, and that you
// did something useful with it! Cheers!

void LargeIntUnsigned::AssignString(const std::string& input)
{ if (input.size()<10)
  { unsigned int x=std::atoi(input.c_str());
    this->AssignShiftedUInt(x, 0);
    return;
  }
  this->MakeZero();
  for (unsigned int i=0; i<input.size(); i++)
  { this->operator*=(10);
    this->operator+=((unsigned) MathRoutines::GetIntFromDigit(input[i]));
  }
}

void Rational::AssignString(const std::string& input)
{ this->MakeZero();
  if (input=="0")
    return;
  bool inputIsNegative=false;
  int positionInTempS=0;
  if (input[0]=='-')
  { positionInTempS++;
    inputIsNegative=true;
  }
  std::string buffer;
  buffer.reserve(input.size());
  LargeIntUnsigned reader;
  bool readingNumerator=true;
  for (unsigned i=positionInTempS; i<input.length(); i++)
  { char a= input[i];
    if (a=='/')
    { readingNumerator=false;
      reader.AssignString(buffer);
      this->operator=(reader);
      buffer="";
    } else
      buffer.push_back(input[i]);
  }
  reader.AssignString(buffer);
  if (readingNumerator)
    this->operator=(reader);
  else
    this->operator/=(reader);
  if (inputIsNegative)
    this->Minus();
}

void GeneralizedVermaModuleCharacters::TransformToWeylProjective
  (int indexOperator, root& startingNormal, root& outputNormal)
{ MatrixLargeRational theOperatorExtended=this->theLinearOperatorsExtended.TheObjects[indexOperator];
  root& theTranslation=this->theTranslationsProjecteD.TheObjects[indexOperator];
  //the goddamned sign in front of theTranslation is now checked: it should be + and not -
  Rational theConst=root::RootScalarEuclideanRoot(this->NonIntegralOriginModification+theTranslation, startingNormal);
  theOperatorExtended.Transpose();
  outputNormal=startingNormal;
  theOperatorExtended.ActOnAroot(outputNormal);
  outputNormal.SetSize(outputNormal.size+1);
  *outputNormal.LastObject()=-theConst;
}

void GeneralizedVermaModuleCharacters::TransformToWeylProjectiveStep1
(GlobalVariables& theGlobalVariables)
{ this->smallerAlgebraChamber.InitFromDirectionsAndRefine(this->GmodKNegWeightsBasisChanged, theGlobalVariables);
  theGlobalVariables.theIndicatorVariables.StatusString1=this->smallerAlgebraChamber.ElementToString(false, false);
  theGlobalVariables.MakeReport();
  this->log << "Directions for making the chamber basis changed: " << this->GmodKNegWeightsBasisChanged.ElementToString()
  << "\n Resulting chamber before projectivization:\n " << this->smallerAlgebraChamber.ElementToString(false, false);
  theGlobalVariables.theIndicatorVariables.StatusString1=this->log.str();
}

void GeneralizedVermaModuleCharacters::TransformToWeylProjectiveStep2
(GlobalVariables& theGlobalVariables)
{ std::stringstream out;
  ConeComplex projectivizedChamberFinal;
  Cone currentProjectiveCone;
  roots tempRoots, extraWeylChamberWalls;
  root tempRoot, wallAtInfinity, wallToSliceWith;
//  int dimSmallerAlgebra=this->theLinearOperators.TheObjects[0].NumRows;
//  int dimLargerAlgebra=this->theLinearOperators.TheObjects[0].NumCols;
//  int dimFinal=dimSmallerAlgebra+dimLargerAlgebra+1;
  projectivizedChamberFinal.init();
  for (int i=0; i<this->smallerAlgebraChamber.size; i++)
  { Cone& currentAffineCone=this->smallerAlgebraChamber.TheObjects[i];
    tempRoots.SetSize(currentAffineCone.Normals.size);
    for (int j=0; j<currentAffineCone.Normals.size; j++)
      this->TransformToWeylProjective(0, currentAffineCone.Normals[j], tempRoots[j]);
    tempRoots.AddListOnTop(this->PreimageWeylChamberLargerAlgebra.Normals);
    theGlobalVariables.theIndicatorVariables.StatusString1=tempRoots.ElementToString();
    theGlobalVariables.MakeReport();
    currentProjectiveCone.CreateFromNormals(tempRoots, theGlobalVariables);
    projectivizedChamberFinal.AddNonRefinedChamberOnTopNoRepetition(currentProjectiveCone);
  }
  for (int i=0; i<this->PreimageWeylChamberSmallerAlgebra.Normals.size; i++)
    projectivizedChamberFinal.splittingNormals.AddObjectOnTopHash(this->PreimageWeylChamberSmallerAlgebra.Normals[i]);
  out << "projectivized chamber before chopping non-dominant part:\n" << projectivizedChamberFinal.ElementToString(false, false);
  projectivizedChamberFinal.Refine(theGlobalVariables);
  out << "Refined projectivized chamber before chopping non-dominant part:\n" << projectivizedChamberFinal.ElementToString(false, false);
  for (int i=0; i<projectivizedChamberFinal.size; i++)
  { Cone& currentCone=projectivizedChamberFinal[i];
    bool isNonDominant=false;
    for (int j=0; j< this->PreimageWeylChamberSmallerAlgebra.Normals.size; j++)
      if (currentCone.GetInternalPoint().ScalarEuclidean(this->PreimageWeylChamberSmallerAlgebra.Normals[j]).IsNegative())
      { isNonDominant=true;
        break;
      }
    if (isNonDominant)
    { projectivizedChamberFinal.PopChamberSwapWithLast(i);
      i--;
    }
  }
  theGlobalVariables.theIndicatorVariables.StatusString1=out.str();
  theGlobalVariables.MakeReport();

  projectivizedChamberFinal.indexLowestNonRefinedChamber=0;
  this->projectivizedChambeR=projectivizedChamberFinal;
  for (int k=1; k<this->theLinearOperators.size; k++)
    for (int i=0; i<this->smallerAlgebraChamber.size; i++)
      for (int j=0; j<this->smallerAlgebraChamber[i].Normals.size; j++)
      { this->TransformToWeylProjective(k, this->smallerAlgebraChamber[i].Normals[j], wallToSliceWith);
        wallToSliceWith.ScaleToIntegralMinHeightFirstNonZeroCoordinatePositive();
        this->projectivizedChambeR.splittingNormals.AddObjectOnTopNoRepetitionOfObjectHash(wallToSliceWith);
      }
  out << "projectivized chamber chopped non-dominant part:\n"  << this->projectivizedChambeR.ElementToString(false, false);
  theGlobalVariables.theIndicatorVariables.StatusString1=out.str();
}

void HomomorphismSemisimpleLieAlgebra::ApplyHomomorphism
  (ElementSimpleLieAlgebra& input, ElementSimpleLieAlgebra& output)
{ assert(&output!=&input);
  output.Nullify(this->theRange);
  for (int i=0; i<input.NonZeroElements.CardinalitySelection; i++)
  { int currentIndex=input.NonZeroElements.elements[i];
    output+=this->imagesAllChevalleyGenerators[currentIndex]*input.coeffsRootSpaces[currentIndex];
  }
  output.Hcomponent.MakeZero(this->theRange.GetRank());
  for (int i=0; i<input.Hcomponent.size; i++)
    output.Hcomponent+=
    this->imagesAllChevalleyGenerators
    [this->theDomain.CartanIndexToChevalleyGeneratorIndex(i)].Hcomponent  *input.Hcomponent[i];
  output.ComputeNonZeroElements();
}

void HomomorphismSemisimpleLieAlgebra::
GetMapSmallCartanDualToLargeCartanDual
(MatrixLargeRational& output)
{ output.init(this->theRange.GetRank(), this->theDomain.GetRank());
  ElementSimpleLieAlgebra domainElt, imageElt;
  domainElt.Nullify(this->theDomain);
  for (int i=0; i<this->theDomain.GetRank(); i++)
  { domainElt.Hcomponent.MakeEi(this->theDomain.GetRank(), i);
//    std::string tempS=domainElt.ElementToString();
    this->ApplyHomomorphism(domainElt, imageElt);
//    std::string tempS2=imageElt.ElementToString();
    output.AssignVectorToColumnKeepOtherColsIntactNoInit(i, imageElt.Hcomponent);
  }
}

void GeneralizedVermaModuleCharacters::initFromHomomorphism
  (root& theParabolicSel, HomomorphismSemisimpleLieAlgebra& input, GlobalVariables& theGlobalVariables)
{ roots tempRoots;
  this->WeylLarger=input.theRange.theWeyl;
  this->WeylSmaller=input.theDomain.theWeyl;
  WeylGroup& theWeyL=input.theRange.theWeyl;
//  input.ProjectOntoSmallCartan(theWeyl.RootsOfBorel, tempRoots, theGlobalVariables);
  this->log << "projections: " << tempRoots.ElementToString();
  theWeyL.ComputeWeylGroup();
  this->NonIntegralOriginModification="(0,0)";
  MatrixLargeRational theProjection;
  root startingWeight, projectedWeight;
  PolynomialOutputFormat theFormat;
  SSalgebraModule tempM;
  std::stringstream tempStream;
  input.ComputeHomomorphismFromImagesSimpleChevalleyGenerators(theGlobalVariables);
  tempM.InduceFromEmbedding(tempStream, input, theGlobalVariables);
  input.GetWeightsGmodK(this->GmodKnegativeWeightS, theGlobalVariables);
//  this->log << "weights of g mod k: " << this->GmodKnegativeWeights.ElementToString();
  MatrixLargeRational tempMat;
  tempMat=input.theDomain.theWeyl.CartanSymmetric;
  tempMat.Invert(theGlobalVariables);
  tempMat.ActOnRoots(this->GmodKnegativeWeightS);
  this->log << this->GmodKnegativeWeightS.ElementToString();
  this->preferredBasiS.SetSize(2);
  this->preferredBasiS[0]=-this->GmodKnegativeWeightS[1];
  this->preferredBasiS[1]=-this->GmodKnegativeWeightS[2];
  this->preferredBasisChangE.AssignRootsToRowsOfMatrix(this->preferredBasiS);
  this->preferredBasisChangE.Transpose();
  this->preferredBasisChangeInversE=this->preferredBasisChangE;
  this->preferredBasisChangeInversE.Invert();
  this->preferredBasisChangeInversE.ActOnRoots(this->GmodKnegativeWeightS, this->GmodKNegWeightsBasisChanged);
  this->log << "\nWeights after basis change: " << this->GmodKNegWeightsBasisChanged.ElementToString();
  for (int i=0; i<this->GmodKnegativeWeightS.size; i++)
    if (this->GmodKnegativeWeightS[i].IsPositiveOrZero())
    { this->GmodKnegativeWeightS.PopIndexSwapWithLast(i);
      i--;
    }
  for (int i=0; i<this->GmodKNegWeightsBasisChanged.size; i++)
    if (this->GmodKNegWeightsBasisChanged[i].IsPositiveOrZero())
    { this->GmodKNegWeightsBasisChanged.PopIndexSwapWithLast(i);
      i--;
    }
  this->log << "\nNegative weights after basis change: " << this->GmodKNegWeightsBasisChanged.ElementToString();
  theProjection.init(input.theDomain.GetRank(), input.theRange.GetRank());
  for (int i=0; i<input.theRange.GetRank(); i++)
  { startingWeight.MakeEi(input.theRange.GetRank(), i);
    input.ProjectOntoSmallCartan(startingWeight, projectedWeight, theGlobalVariables);
    this->preferredBasisChangeInversE.ActOnAroot(projectedWeight);
    for (int j=0; j<projectedWeight.size; j++)
      theProjection.elements[j][i]=projectedWeight[j];
  }
  ReflectionSubgroupWeylGroup theSubgroup;
  this->ParabolicLeviPartRootSpacesZeroStandsForSelected=theParabolicSel;
  MatrixLargeRational DualCartanEmbedding;
  input.GetMapSmallCartanDualToLargeCartanDual(DualCartanEmbedding);
  root ParabolicEvaluationRootImage, tempRoot;
  ParabolicEvaluationRootImage=this->ParabolicLeviPartRootSpacesZeroStandsForSelected;
  this->ParabolicSelectionSmallerAlgebra.init(input.theDomain.GetRank());
  for (int i=0; i<input.theDomain.GetRank(); i++)
  { DualCartanEmbedding.ColToRoot(i, tempRoot);
    if (ParabolicEvaluationRootImage.ScalarEuclidean(tempRoot).IsPositive())
      this->ParabolicSelectionSmallerAlgebra.AddSelectionAppendNewIndex(i);
  }
  this->log << "\nDual cartan embedding smaller into larger:\n" << DualCartanEmbedding.ElementToString(false, false);
  this->log << "\nParabolic subalgebra large algebra: " << this->ParabolicLeviPartRootSpacesZeroStandsForSelected.ElementToString();
  tempRoot= this->ParabolicSelectionSmallerAlgebra;
  this->log << "\nParabolic subalgebra smaller algebra: " << tempRoot.ElementToString();
  theSubgroup.MakeParabolicFromSelectionSimpleRoots
  (theWeyL, this->ParabolicLeviPartRootSpacesZeroStandsForSelected, theGlobalVariables, -1);

  this->theLinearOperators.SetSize(theSubgroup.size);
  this->theLinearOperatorsExtended.SetSize(theSubgroup.size);
  this->theTranslationS.SetSize(theSubgroup.size);
  this->theTranslationsProjecteD.SetSize(theSubgroup.size);
  this->theCoeffs.SetSize(theSubgroup.size);

  this->log << "\nMatrix form of the elements of Weyl group of the Levi part of the parabolic (" << theSubgroup.size << " elements):\n";

  for (int i=0; i<theSubgroup.size; i++)
  { MatrixLargeRational& currentLinearOperator=this->theLinearOperators[i];
    theSubgroup.AmbientWeyl.GetMatrixOfElement(theSubgroup[i], currentLinearOperator);
//    currentLinearOperator.MultiplyOnTheLeft(preferredBasisChangeInverse);
    this->log << "\n" << currentLinearOperator.ElementToString(false, false);
    currentLinearOperator.ActOnAroot(theWeyL.rho, this->theTranslationS[i]);
    this->theTranslationS[i]-=theWeyL.rho;
    this->theTranslationS[i].MinusRoot();
    theProjection.ActOnAroot(this->theTranslationS[i], this->theTranslationsProjecteD[i]);
    if (theSubgroup[i].size%2==0)
      this->theCoeffs[i]=1;
    else
      this->theCoeffs[i]=-1;
  }
  this->log << "\n\n\nMatrix of the projection operator:\n" << theProjection.ElementToString(false, false);
  this->log << "\n\n\nMatrix form of the operators $u_w$, the translations $\tau_w$ and their projections (" << this->theLinearOperatorsExtended.size << "):";
  //List<MatrixLargeRational> tempList;
  for (int k=0; k<this->theLinearOperators.size; k++)
  { MatrixLargeRational& currentLO=this->theLinearOperators.TheObjects[k];
    MatrixLargeRational& currentLOExtended=this->theLinearOperatorsExtended.TheObjects[k];
    currentLO.MultiplyOnTheLeft(theProjection);
    currentLO*=-1;
    //tempList.AddOnTopNoRepetition(this->theLinearOperators.TheObjects[i]);
    currentLOExtended.MakeIdMatrix(currentLO.NumRows);
    currentLOExtended.Resize(currentLO.NumRows, currentLO.NumRows+currentLO.NumCols, true);
    for (int i=0; i<currentLO.NumRows; i++)
      for (int j=0; j<currentLO.NumCols; j++)
        currentLOExtended.elements[i][j+currentLO.NumRows]=currentLO.elements[i][j];
    this->log << "\n\n" << currentLOExtended.ElementToString(false, false);
    this->log << this->theTranslationS[k].ElementToString() << ";   " << this->theTranslationsProjecteD[k].ElementToString();
  }

  List<int> displayIndicesReflections;
  for (int i=0; i<this->ParabolicLeviPartRootSpacesZeroStandsForSelected.MaxSize; i++)
    if (!this->ParabolicLeviPartRootSpacesZeroStandsForSelected.selected[i])
      displayIndicesReflections.AddObjectOnTop(i+1);
  Matrix<PolynomialRationalCoeff> tempMatPoly;
  Vector<PolynomialRationalCoeff> tempVect, tempVect2;
  tempVect.SetSize(input.theDomain.theWeyl.GetDim()+input.theRange.theWeyl.GetDim());
  for (int i=0; i<tempVect.size; i++)
    tempVect[i].MakeMonomialOneLetter(tempVect.size, i, 1, (Rational) 1);
  tempMatPoly.init(input.theDomain.theWeyl.GetDim(), tempVect.size);
  PolynomialRationalCoeff polyZero;
  polyZero.Nullify(tempVect.size);
  theFormat.alphabet[0]="x_1";
  theFormat.alphabet[1]="x_2";
  theFormat.alphabet[2]="y_1";
  theFormat.alphabet[3]="y_2";
  theFormat.alphabet[4]="y_3";
  this->log << "\n\n\\begin{longtable}{r|l}$w$ & \\begin{tabular}{c}Argument of the vector partition function in (\\ref{eqMultG2inB3General}) =\\\\ $u_w\\circ" << tempVect.ElementToString(theFormat) << "-\\tau_w$ \\end{tabular}  \\\\ \\hline \\endhead";
  for (int i=0; i<this->theLinearOperatorsExtended.size; i++)
  { MatrixLargeRational& currentLoExt=this->theLinearOperatorsExtended[i];
    for (int j=0; j<currentLoExt.NumRows; j++)
      for (int k=0; k<currentLoExt.NumCols; k++)
        tempMatPoly.elements[j][k].MakeNVarConst(tempVect.size, currentLoExt.elements[j][k]);
    tempMatPoly.ActOnVectorColumn(tempVect, tempVect2, polyZero);
    for (int j=0; j<tempVect2.size; j++)
      tempVect2[j]+=this->theTranslationsProjecteD[i][j];
    this->log << "\n$" <<  theSubgroup[i].ElementToString(true, false, "\\eta", & displayIndicesReflections) << "$&$"
    << tempVect2.ElementToString(theFormat) << "$\\\\";
  }
  this->log <<"\\end{longtable}\n\n";
//  this->log << "\n\n\nThere are " << tempList.size << " different operators.";
  Lattice tempLattice;
  theWeyL.GetIntegralLatticeInSimpleCoordinates(tempLattice);
  this->theExtendedIntegralLatticeMatForM.basisRationalForm.MakeIdMatrix(input.theDomain.GetRank());
  this->theExtendedIntegralLatticeMatForM.basisRationalForm.DirectSumWith(tempLattice.basisRationalForm, (Rational) 0);
  this->theExtendedIntegralLatticeMatForM.MakeFromMat(this->theExtendedIntegralLatticeMatForM.basisRationalForm);
  tempMat=theWeyL.CartanSymmetric;
  tempMat.Invert(theGlobalVariables);
  roots WallsWeylChamberLargerAlgebra;
  for (int i=0; i<tempMat.NumRows; i++)
  { tempMat.RowToRoot(i, tempRoot);
    if(ParabolicEvaluationRootImage[i].IsEqualToZero())
    { WallsWeylChamberLargerAlgebra.SetSize(WallsWeylChamberLargerAlgebra.size+1);
      *WallsWeylChamberLargerAlgebra.LastObject()=tempRoot;
    }
  }
  this->log << "\n\n\n**************\nParabolic selection larger algebra:" << ParabolicEvaluationRootImage.ElementToString() << "\nWalls Weyl chamber larger algebra: " << WallsWeylChamberLargerAlgebra.ElementToString();
  this->log << "\n**************\n\n";
  roots rootsGeneratingExtendedLattice;
  int totalDim=input.theRange.GetRank()+input.theDomain.GetRank();
  rootsGeneratingExtendedLattice.SetSize(totalDim);
  this->log << "\n" << tempMat.ElementToString(false, false) << "\n";
  this->log << this->theExtendedIntegralLatticeMatForM.ElementToString(false, false);
  this->WeylChamberSmallerAlgebra.CreateFromNormals(WallsWeylChamberLargerAlgebra, theGlobalVariables);
  this->log << "\nWeyl chamber larger algebra before projectivizing: " << this->WeylChamberSmallerAlgebra.ElementToString(theFormat) << "\n";
  this->PreimageWeylChamberSmallerAlgebra.Normals=this->WeylChamberSmallerAlgebra.Normals;
  for (int i=0; i<this->PreimageWeylChamberLargerAlgebra.Normals.size; i++)
  { tempRoot.MakeZero(input.theRange.GetRank()+input.theDomain.GetRank()+1);
    for (int j=0; j<input.theRange.GetRank(); j++)
      tempRoot.TheObjects[j+input.theDomain.GetRank()]=this->PreimageWeylChamberLargerAlgebra.Normals[i][j];
    this->PreimageWeylChamberLargerAlgebra.Normals[i]=tempRoot;
  }
  tempMat=input.theDomain.theWeyl.CartanSymmetric;
  tempMat.Invert(theGlobalVariables);
  tempRoots.size=0;
  root ParabolicEvaluationRootSmallerAlgebra;
  ParabolicEvaluationRootSmallerAlgebra=this->ParabolicSelectionSmallerAlgebra;
  for (int i=0; i<tempMat.NumRows; i++)
  { input.theDomain.theWeyl.CartanSymmetric.RowToRoot(i, tempRoot);
    if (tempRoot.ScalarEuclidean(ParabolicEvaluationRootSmallerAlgebra).IsEqualToZero())
    { tempRoots.SetSize(tempRoots.size+1);
      tempMat.RowToRoot(i, *tempRoots.LastObject());
    }
  }
  this->preferredBasisChangeInversE.ActOnRoots(tempRoots);
  this->log << "**********************\n\n\n";
  this->log << "\nthe smaller parabolic selection: " << this->ParabolicSelectionSmallerAlgebra.ElementToString();
  this->log << "the roots generating the chamber walls: " << tempRoots.ElementToString();
  this->PreimageWeylChamberSmallerAlgebra.CreateFromVertices(tempRoots, theGlobalVariables);
  this->log << "\nWeyl chamber smaller algebra: " << this->PreimageWeylChamberSmallerAlgebra.ElementToString(theFormat) << "\n";
  this->log << "**********************\n\n\n";
  this->log << "\nThe first operator extended:\n" << this->theLinearOperatorsExtended.TheObjects[0].ElementToString(false, false) << "\n";
  this->log << "\nThe second operator extended:\n" << this->theLinearOperatorsExtended.TheObjects[1].ElementToString(false, false) << "\n";
  /*tempMat=this->theLinearOperatorsExtended.TheObjects[0];
  tempMat.Transpose();
  tempMat.ActOnRoots(this->PreimageWeylChamberSmallerAlgebra);
  for (int i=0; i<this->PreimageWeylChamberSmallerAlgebra.size; i++)
  { this->PreimageWeylChamberSmallerAlgebra.TheObjects[i].SetSize(input.theRange.GetRank()+input.theDomain.GetRank()+1);
    *this->PreimageWeylChamberSmallerAlgebra.TheObjects[i].LastObject()=0;
  }*/
  for (int i=0; i<this->PreimageWeylChamberSmallerAlgebra.Normals.size; i++)
  { tempRoot.MakeZero(input.theRange.GetRank()+input.theDomain.GetRank()+1);
    for (int j=0; j<input.theDomain.GetRank(); j++)
      tempRoot.TheObjects[j]=this->PreimageWeylChamberSmallerAlgebra.Normals[i][j];
  //  for (int j=0; j<input.theRange.GetRank(); j++)
   //   tempRoot.TheObjects[j+input.theDomain.GetRank()]=tempRoot2.TheObjects[j];
    this->PreimageWeylChamberSmallerAlgebra.Normals[i]=tempRoot;
  }

  tempRoot.MakeEi(input.theRange.GetRank()+input.theDomain.GetRank()+1, input.theRange.GetRank()+input.theDomain.GetRank());
  this->PreimageWeylChamberLargerAlgebra.Normals.AddObjectOnTop(tempRoot);
  this->log << "\nPreimage Weyl chamber smaller algebra: " << this->PreimageWeylChamberSmallerAlgebra.ElementToString(theFormat) << "\n";
  this->log << "\nPreimage Weyl chamber larger algebra: " << this->PreimageWeylChamberLargerAlgebra.ElementToString(theFormat) << "\n";

  theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
  theGlobalVariables.theIndicatorVariables.StatusString1=this->log.str();
  theGlobalVariables.MakeReport();
}

void WeylGroup::GetMatrixOfElement(int theIndex, MatrixLargeRational& outputMatrix)
{ assert(theIndex<this->size);
  this->GetMatrixOfElement(this->TheObjects[theIndex], outputMatrix);
}

void ReflectionSubgroupWeylGroup::MakeParabolicFromSelectionSimpleRoots
(WeylGroup& inputWeyl, Selection& ZeroesMeanSimpleRootSpaceIsInParabolic, GlobalVariables& theGlobalVariables, int UpperLimitNumElements)
{ roots selectedRoots;
  selectedRoots.MakeActualSizeAtLeastExpandOnTop(ZeroesMeanSimpleRootSpaceIsInParabolic.MaxSize- ZeroesMeanSimpleRootSpaceIsInParabolic.CardinalitySelection);
  this->AmbientWeyl=inputWeyl;
  for (int i=0; i<ZeroesMeanSimpleRootSpaceIsInParabolic.MaxSize; i++)
    if (!ZeroesMeanSimpleRootSpaceIsInParabolic.selected[i])
    { selectedRoots.SetSize(selectedRoots.size+1);
      selectedRoots.LastObject()->MakeEi(inputWeyl.GetDim(), i);
    }
  rootsCollection tempRootsCol;
  this->ComputeSubGroupFromGeneratingReflections(selectedRoots, tempRootsCol, theGlobalVariables, UpperLimitNumElements, true);
}

void WeylGroup::GetMatrixOfElement(ElementWeylGroup& input, MatrixLargeRational& outputMatrix)
{ root tempRoot;
  int theDim=this->CartanSymmetric.NumRows;
  outputMatrix.init(theDim, theDim);
  for (int i=0; i<theDim; i++)
  { tempRoot.MakeEi(theDim, i);
    this->ActOn(input, tempRoot, false, false, (Rational) 0);
    for (int j=0; j<theDim; j++)
      outputMatrix.elements[j][i]=tempRoot.TheObjects[j];
  }
}

GeneralizedVermaModuleCharacters tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis;

void GeneralizedVermaModuleCharacters::GetProjection(int indexOperator, const root& input, root& output)
{ MatrixLargeRational& currentExtendedOperator=this->theLinearOperatorsExtended[indexOperator];
  root& currentTranslation=this->theTranslationsProjecteD[indexOperator];
  assert (!input.LastObject()->IsEqualToZero());
  output=input;
  Rational tempRat=*output.LastObject();
  output/=tempRat;
  output.size--;
  currentExtendedOperator.ActOnAroot(output);
  output+=currentTranslation;
}

void MatrixLargeRational::GetMatrixIntWithDen(Matrix<LargeInt>& outputMat, LargeIntUnsigned& outputDen)
{ outputDen.MakeOne();
  MatrixLargeRational tempMat;
  tempMat=*this;
  outputMat.init(this->NumRows, this->NumCols);
  LargeIntUnsigned tempUI;
  Rational tempRat;
  for (int i=0; i<this->NumRows; i++)
    for (int j=0; j<this->NumCols; j++)
    { tempMat.elements[i][j].GetDen(tempUI);
      if (!tempUI.IsEqualToOne())
      { outputDen*=tempUI;
        tempRat=tempUI;
        tempMat*=tempRat;
      }
    }
  for (int i=0; i<this->NumRows; i++)
    for (int j=0; j<this->NumCols; j++)
      tempMat.elements[i][j].GetNum(outputMat.elements[i][j]);
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
  root& theTranslation=this->theTranslationS[theIndex];
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


void Selection::operator=(const root& other)
{ this->init(other.size);
  for (int i=0; i<other.size; i++)
    if (!other[i].IsEqualToZero())
      this->selected[i]=true;
  this->ComputeIndicesFromSelection();
}

void GeneralizedVermaModuleCharacters::ComputeQPsFromChamberComplex
(GlobalVariables& theGlobalVariables)
{ std::stringstream out;
  PolynomialOutputFormat theFormat;
  root tempRoot;
  CGIspecificRoutines::OpenDataFileOrCreateIfNotPresent(this->theMultiplicitiesMaxOutputReport2, "/home/todor/math/vectorpartition/trunk/ExtremaPolys.txt", false, true, false);
  this->thePfs.initFromRoots(this->GmodKNegWeightsBasisChanged, theGlobalVariables);
  this->thePfs.ComputeDebugString(theGlobalVariables);
  out << this->thePfs.DebugString;
  this->thePfs.split(theGlobalVariables, 0);
  this->thePfs.ComputeDebugString(theGlobalVariables);
  out << "=" << this->thePfs.DebugString;
//  int totalDim=this->theTranslationS[0].size+this->theTranslationsProjecteD[0].size;
  this->theQPsSubstituted.SetSize(this->projectivizedChambeR.size);
  this->thePfs.theChambers.init();
  this->thePfs.theChambers.theDirections=this->GmodKNegWeightsBasisChanged;
  this->thePfs.theChambers.SliceTheEuclideanSpace(theGlobalVariables, false);
  this->theQPsNonSubstituted.SetSize(this->thePfs.theChambers.size);
  this->theQPsSubstituted.SetSize(this->thePfs.theChambers.size);
  out << "\n\nThe vector partition functions in each chamber follow.";
  theGlobalVariables.theIndicatorVariables.ProgressReportStringsNeedRefresh=true;
  for (int i=0; i<this->thePfs.theChambers.size; i++)
    if (this->thePfs.theChambers.TheObjects[i]!=0)
    { QuasiPolynomial& currentQPNoSub= this->theQPsNonSubstituted.TheObjects[i];
      this->theQPsSubstituted.TheObjects[i].SetSize(this->theLinearOperators.size);
      this->thePfs.GetVectorPartitionFunction(currentQPNoSub, this->thePfs.theChambers.TheObjects[i]->InternalPoint, theGlobalVariables);
      out << "\nChamber " << i+1 << " with internal point " << this->thePfs.theChambers.TheObjects[i]->InternalPoint.ElementToString() << " the quasipoly is: " << currentQPNoSub.ElementToString(false, false);
      for (int k=0; k<this->theLinearOperators.size; k++)
      { QuasiPolynomial& currentQPSub=this->theQPsSubstituted.TheObjects[i].TheObjects[k];
        std::stringstream tempStream;
        tempStream << "Processing chamber " << i+1 << " linear operator " << k+1;
        theGlobalVariables.theIndicatorVariables.ProgressReportStrings[0]= tempStream.str();
        theGlobalVariables.MakeReport();
        currentQPNoSub.Substitution(this->theLinearOperatorsExtended.TheObjects[k], this->theTranslationsProjecteD[k], this->theExtendedIntegralLatticeMatForM, currentQPSub, theGlobalVariables);
        out << "; after substitution we get: " << currentQPSub.ElementToString(false, false);
      }
    }
//  this->theParser.theHmm.theRange.theWeyl.GetIntegralLatticeInSimpleCoordinates(integralLattice);
  //out << "\nThe integral lattice:\n" << integralLattice.ElementToString(false, false);
  //this->theMultiplicitiesMaxOutputReport2.flush();
  QuasiPolynomial tempQP;
  this->theMultiplicities.SetSize(this->projectivizedChambeR.size);
  this->numNonZeroMults=0;
  for (int i=0; i<this->projectivizedChambeR.size; i++)
  { QuasiPolynomial& currentSum=this->theMultiplicities.TheObjects[i];
    currentSum.MakeZeroOverLattice(this->theExtendedIntegralLatticeMatForM);
    for (int k=0; k<this->theLinearOperators.size; k++)
    { this->GetProjection(k, this->projectivizedChambeR.TheObjects[i].GetInternalPoint(), tempRoot);
      tempRoot-=this->NonIntegralOriginModification;
      int theIndex= this->thePfs.theChambers.GetFirstChamberIndexContainingPoint(tempRoot);
      if (theIndex!=-1)
      { tempQP=this->theQPsSubstituted.TheObjects[theIndex].TheObjects[k];
        tempQP*=this->theCoeffs.TheObjects[k];
        currentSum+=tempQP;
      }
      std::stringstream tempStream;
      tempStream << " Chamber " << i+1 << " translation " << k+1;
      theGlobalVariables.theIndicatorVariables.ProgressReportStrings[0]=tempStream.str();
      theGlobalVariables.MakeReport();
    }
    if (!currentSum.IsEqualToZero())
      this->numNonZeroMults++;
    std::stringstream tempStream;
    tempStream << " So far " << i+1 << " out of " << this->projectivizedChambeR.size << " processed " << this->numNonZeroMults << " non-zero total.";
    theGlobalVariables.theIndicatorVariables.ProgressReportStrings[1]=tempStream.str();
    theGlobalVariables.MakeReport();
    out << "\nChamber " << i+1 << ": the quasipolynomial is: " << currentSum.ElementToString(false, false);
    out << "\nThe chamber is: " << this->projectivizedChambeR.TheObjects[i].ElementToString(theFormat);
  }
//  this->projectivizedChamber.ComputeDebugString();
//  out << "\n\n" << this->projectivizedChamber.DebugString;
  theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
  theGlobalVariables.theIndicatorVariables.StatusString1=out.str();
  theGlobalVariables.MakeReport();
  this->theMultiplicitiesMaxOutputReport2 << out.str();
}

int ParserNode::EvaluateCreateFromDirectionsAndSalamiSlice
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ roots tempRoots;
  int theDim;
  if(!theNode.GetRootsEqualDimNoConversionNoEmptyArgument(theArgumentList, tempRoots, theDim))
    return theNode.SetError(theNode.errorDimensionProblem);
  ConeComplex tempComplex;
  tempComplex.InitFromDirectionsAndRefine(tempRoots, theGlobalVariables);
  PolynomialOutputFormat tempFormat;
  theNode.outputString=tempComplex.DrawMeToHtmlProjective(theGlobalVariables.theDrawingVariables, tempFormat);
  theNode.ExpressionType=theNode.typeString;
  return theNode.errorNoError;
}

int ParserNode::EvaluateG2InB3Computation
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ root parSel="(0,0,0)";
  tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis.IncrementComputation
    (parSel, theGlobalVariables)
    ;
  return theNode.errorNoError;
}

void GeneralizedVermaModuleCharacters::ReadFromDefaultFile(GlobalVariables* theGlobalVariables)
{ std::fstream input;
  if (!CGIspecificRoutines::FileExists( "/home/todor/math/vectorpartition/trunk/GenVermaComputation.txt"))
  { this->computationPhase=0;
    return;
  }
  CGIspecificRoutines::OpenDataFileOrCreateIfNotPresent(input, "/home/todor/math/vectorpartition/trunk/GenVermaComputation.txt", false, false, false);
  this->ReadFromFile(input, theGlobalVariables);
  input.close();
}

void GeneralizedVermaModuleCharacters::WriteToDefaultFile(GlobalVariables* theGlobalVariables)
{ std::fstream output;
  CGIspecificRoutines::OpenDataFileOrCreateIfNotPresent(output, "/home/todor/math/vectorpartition/trunk/GenVermaComputation.txt", false, true, false);
  this->WriteToFile(output, theGlobalVariables);
}

std::string GeneralizedVermaModuleCharacters::ElementToStringMultiplicitiesReport
(GlobalVariables& theGlobalVariables)
{ assert(this->theMultiplicities.size== this->projectivizedChambeR.size);
  std::stringstream out;
  PolynomialOutputFormat theFormat;
  theFormat.alphabet[0]="x_1";
  theFormat.alphabet[1]="x_2";
  theFormat.alphabet[2]="y_1";
  theFormat.alphabet[3]="y_2";
  theFormat.alphabet[4]="y_3";
  out << "Number chambers: " << projectivizedChambeR.size << " of them " << this->numNonZeroMults << " non-zero.";
  int numInequalities=0;
  for (int i=0; i<this->projectivizedChambeR.size; i++)
  { numInequalities+=this->projectivizedChambeR[i].Normals.size;
  }
  out << "\nNumber of inequalities: " << numInequalities;

  out << this->PrepareReport(theGlobalVariables);
  return out.str();
}

int ParserNode::EvaluateG2InB3MultsParabolic
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ root highestWeight, parSel;
  ParserNode& nodeHW=theNode.owner->TheObjects[theArgumentList[0]];
  ParserNode& nodeSel=theNode.owner->TheObjects[theArgumentList[1]];
  if(!nodeHW.GetRootRationalDontUseForFunctionArguments(highestWeight, theGlobalVariables))
    return theNode.SetError(theNode.errorProgramming);
  if (!nodeSel.GetRootRationalDontUseForFunctionArguments(parSel, theGlobalVariables))
    return theNode.SetError(theNode.errorProgramming);
  if (highestWeight.size!=3 || parSel.size!=3)
    return theNode.SetError(theNode.errorDimensionProblem);
  theNode.outputString=tempCharsEraseWillBeErasedShouldntHaveLocalObjectsLikeThis
  .ComputeMultsLargerAlgebraHighestWeight(highestWeight, parSel, *theNode.owner, theGlobalVariables);
  theNode.ExpressionType=theNode.typeString;
  return theNode.errorNoError;
}

std::string GeneralizedVermaModuleCharacters::ComputeMultsLargerAlgebraHighestWeight
  ( root& highestWeightLargerAlg, root& parabolicSel, Parser& theParser, GlobalVariables& theGlobalVariables
   )
{ std::stringstream out;
  WeylGroup& LargerWeyl=theParser.theHmm.theRange.theWeyl;
  WeylGroup& SmallerWeyl=theParser.theHmm.theDomain.theWeyl;
  if (LargerWeyl.GetDim()!=3 || LargerWeyl.WeylLetter!='B')
    return "Error: algebra is not so(7).";
  this->initFromHomomorphism(parabolicSel, theParser.theHmm, theGlobalVariables);
  MatrixLargeRational tempMat;
  tempMat=LargerWeyl.CartanSymmetric;
  tempMat.Invert();
  tempMat.ActOnAroot(highestWeightLargerAlg);
  root tempRoot, ZeroRoot;
  DrawingVariables& drawOps=theGlobalVariables.theDrawingVariables;
  int theSmallDim=SmallerWeyl.CartanSymmetric.NumRows;
  drawOps.theBuffer.initDimensions(theSmallDim, 1);
  drawOps.theBuffer.MakeMeAStandardBasis(theSmallDim);
  PolynomialOutputFormat tempFormat;
  drawOps.theBuffer.ProjectionsEiVectors[0][0]=0.5;  drawOps.theBuffer.ProjectionsEiVectors[0][1]=-0.87;
  drawOps.theBuffer.ProjectionsEiVectors[1][0]=-0.5; drawOps.theBuffer.ProjectionsEiVectors[1][1]=-0.87;
  drawOps.theBuffer.GraphicsUnit[0]=50;
  ConeComplex tempComplex;
  tempComplex.InitFromDirectionsAndRefine(this->GmodKNegWeightsBasisChanged, theGlobalVariables);
  ZeroRoot.MakeZero(theSmallDim);
  for (int i=0; i<theSmallDim; i++)
  { tempRoot.MakeEi(theSmallDim, i);
    drawOps.theBuffer.drawLineBetweenTwoVectorsBuffer
    (ZeroRoot, tempRoot, drawOps.PenStyleNormal, CGIspecificRoutines::RedGreenBlue(150,150,150));
  }

  for (int i=0; i<this->theLinearOperators.size; i++)
  { this->theLinearOperators[i].ActOnAroot(highestWeightLargerAlg, tempRoot);
    tempRoot.MinusRoot();
    tempRoot-=this->theTranslationsProjecteD[i];
    drawOps.
    drawCircleAtVectorBuffer(tempRoot, 2, DrawingVariables::PenStyleNormal, CGIspecificRoutines::RedGreenBlue(0,0,100));
    drawOps.drawLineBetweenTwoVectorsBuffer(tempRoot, ZeroRoot, DrawingVariables::PenStyleNormal, CGIspecificRoutines::RedGreenBlue(240, 240, 240));
    tempComplex.DrawMeProjective(&tempRoot, false, drawOps, tempFormat);
    out << tempRoot.ElementToString();
  }
  drawOps.drawCoordSystemBuffer(drawOps, 3, 0);
  out << drawOps.GetHtmlFromDrawOperationsCreateDivWithUniqueName(2);
  out << "Input so(7)-highest weight: " << highestWeightLargerAlg.ElementToString();
  out << "<br> Input parabolics selections: " << parabolicSel.ElementToString();
  return out.str();
}


void GeneralizedVermaModuleCharacters::SortMultiplicities(GlobalVariables& theGlobalVariables)
{ List<Cone> tempList;
  tempList=this->projectivizedChambeR;
  tempList.QuickSortAscending();
  List<QuasiPolynomial> tempQPlist;
  tempQPlist.SetSize(this->theMultiplicities.size);
  for (int i=0; i<this->theMultiplicities.size; i++)
    tempQPlist[i]=this->theMultiplicities[this->projectivizedChambeR.IndexOfObjectHash(tempList[i])];
  this->theMultiplicities=tempQPlist;
  this->projectivizedChambeR.ClearTheObjects();
  for (int i=0; i<tempList.size; i++)
    this->projectivizedChambeR.AddObjectOnTopHash(tempList[i]);
}

void DynkinDiagramRootSubalgebra::ComputeDynkinString
(int indexComponent, WeylGroup& theWeyl)
{ assert(indexComponent<this->SimpleBasesConnectedComponents.size);
  std::stringstream out;
  roots& currentComponent= this->SimpleBasesConnectedComponents.TheObjects[indexComponent];
  List<int>& currentEnds=this->indicesEnds.TheObjects[indexComponent];
  if (this->numberOfThreeValencyNodes(indexComponent, theWeyl)==1)
  {//type D or E
    //in type D first comes the triple node, then the long string, then the one-root strings
    // the long string is oriented with the end that is connected to the triple node having
    //smaller index
    //in type E similarly the longest string comes first oriented with the root that is linked to the triple node having smaller index
    // then comes the second longest string (oriented in the same fashion)
    // and last the one-root string
    root tripleNode;
    int tripleNodeindex=this->indicesThreeNodes.TheObjects[indexComponent];
    tripleNode.Assign( currentComponent.TheObjects[tripleNodeindex]);
    roots tempRoots;
    tempRoots.CopyFromBase(currentComponent);
    tempRoots.PopIndexSwapWithLast(tripleNodeindex);
    DynkinDiagramRootSubalgebra tempDiagram;
    tempDiagram.ComputeDiagramTypeKeepInput(tempRoots, theWeyl);
    assert(tempDiagram.SimpleBasesConnectedComponents.size==3);
    List<int> indicesLongComponents;
    indicesLongComponents.size=0;
    Rational tempRat;
    for (int i=0; i<3; i++)
    { if(tempDiagram.SimpleBasesConnectedComponents.TheObjects[i].size>1)
        indicesLongComponents.AddObjectOnTop(i);
      theWeyl.RootScalarCartanRoot(tempDiagram.SimpleBasesConnectedComponents.TheObjects[i].TheObjects[0], currentComponent.TheObjects[tripleNodeindex], tempRat);
      if (tempRat.IsEqualToZero())
        tempDiagram.SimpleBasesConnectedComponents.TheObjects[i].ReverseOrderElements();
    }
    for(int i=0; i<3; i++)
      for(int j=i+1; j<3; j++)
        if (tempDiagram.SimpleBasesConnectedComponents.TheObjects[i].size<tempDiagram.SimpleBasesConnectedComponents.TheObjects[j].size)
        { tempRoots.CopyFromBase(tempDiagram.SimpleBasesConnectedComponents.TheObjects[i]);
          tempDiagram.SimpleBasesConnectedComponents.TheObjects[i].CopyFromBase(tempDiagram.SimpleBasesConnectedComponents.TheObjects[j]);
          tempDiagram.SimpleBasesConnectedComponents.TheObjects[j].CopyFromBase(tempRoots);
        }
    currentComponent.size=0;
    currentComponent.AddObjectOnTop(tripleNode);
    for (int i=0; i<3; i++)
      currentComponent.AddListOnTop(tempDiagram.SimpleBasesConnectedComponents.TheObjects[i]);
    if ( indicesLongComponents.size==1 || indicesLongComponents.size==0)
      out << this->SetComponent("D", currentComponent.size, indexComponent);
    else
    {//type E
      assert(indicesLongComponents.size==2);
      out << this->SetComponent("E", currentComponent.size, indexComponent);
    }
  }else
  { Rational length1, length2, tempRat;
    theWeyl.RootScalarCartanRoot(currentComponent.TheObjects[0], currentComponent.TheObjects[0], length1);
    int numLength1=1; int numLength2=0;
    for(int i=1; i<currentComponent.size; i++)
    { theWeyl.RootScalarCartanRoot(currentComponent.TheObjects[i], currentComponent.TheObjects[i],  tempRat);
      if (tempRat.IsEqualTo(length1))
        numLength1++;
      else
      { numLength2++;
        length2.Assign(tempRat);
      }
    }
    if (numLength2==0 )
    { //type A
      if (length1.IsEqualTo(theWeyl.LongRootLength))
        out << this->SetComponent("A", numLength1, indexComponent);
      else
        out << this->SetComponent("A'", numLength1, indexComponent);
    }
    else
    {//the longer root should have smaller index
      Rational greaterlength, tempRat;
      int numGreaterLength=numLength2;
      int numSmallerLength=numLength1;
      greaterlength.Assign(length2);
      if (length1.IsGreaterThan(length2))
      { greaterlength.Assign(length1);
        numGreaterLength=numLength1;
        numSmallerLength=numLength2;
      }
      theWeyl.RootScalarCartanRoot(currentComponent.TheObjects[currentEnds.TheObjects[0]], currentComponent.TheObjects[currentEnds.TheObjects[0]], tempRat);
      if (greaterlength.IsGreaterThan(tempRat))
        currentEnds.SwapTwoIndices(0, 1);
      if (numLength1==numLength2)
      {//B2, C2, F4 or G2
        if (numLength1!=1)
        { out << this->SetComponent("F", 4, indexComponent);
          assert(numLength1==2);
        } else
        { if (length1.NumShort==6 || length2.NumShort==6)
            out << this->SetComponent("G", 2, indexComponent);
          else
            out << this->SetComponent("B", 2, indexComponent);
        }
      } else
      { if (numGreaterLength>numSmallerLength)
          out << this->SetComponent("B", currentComponent.size, indexComponent);
        else
          out << this->SetComponent("C", currentComponent.size, indexComponent);
      }
    }
    currentComponent.SwapTwoIndices(0, currentEnds.TheObjects[0]);
    for (int i=0; i<currentComponent.size; i++)
      for (int j=i+1; j<currentComponent.size; j++)
      { theWeyl.RootScalarCartanRoot(currentComponent.TheObjects[i], currentComponent.TheObjects[j], tempRat);
        if (!tempRat.IsEqualToZero())
        { currentComponent.SwapTwoIndices(i+1, j);
          break;
        }
      }
  }
  this->DynkinTypeStrings.TheObjects[indexComponent]=out.str();
}

std::string DynkinDiagramRootSubalgebra::SetComponent(const std::string& WeylLetterWithLength, int WeylRank, int componentIndex)
{ this->ComponentLetters[componentIndex]=WeylLetterWithLength;
  this->ComponentRanks[componentIndex]=WeylRank;
  std::string result=this->GetNameFrom(WeylLetterWithLength, WeylRank, false);
  this->DynkinTypeStrings[componentIndex]=result;
  return result;
}

std::string DynkinDiagramRootSubalgebra::GetNameFrom
  (const std::string& WeylLetterWithLength , int WeylRank, bool IncludeAlgebraNames)
{ std::stringstream out;
  out << WeylLetterWithLength << "_" << WeylRank;
  if (IncludeAlgebraNames && (WeylLetterWithLength[0]=='A' || WeylLetterWithLength[0]=='B' || WeylLetterWithLength[0]=='C' || WeylLetterWithLength[0]=='D'))
    out << "(" << SemisimpleLieAlgebra::GetLieAlgebraName(WeylLetterWithLength[0], WeylRank) << ")";
  return out.str();
}

void DynkinDiagramRootSubalgebra::ElementToStrinG
  (std::string& output, bool useDollarSigns, bool IncludeAlgebraNames)
{ std::stringstream out;
  for (int j=0; j<this->sameTypeComponents.size; j++)
  { int numSameTypeComponents= this->sameTypeComponents.TheObjects[j].size;
    if (useDollarSigns)
      out << "$";
    if (numSameTypeComponents!=1)
      out << numSameTypeComponents;
    out << this->GetNameFrom(this->ComponentLetters[this->sameTypeComponents[j][0]], this->ComponentRanks[this->sameTypeComponents[j][0]], IncludeAlgebraNames);
    if (useDollarSigns)
      out << "$";
    if (j!=this->sameTypeComponents.size-1)
      out << "+";
  }
  output=out.str();
}

void WeylGroup::GetWeylChamber
(Cone& output, GlobalVariables& theGlobalVariables)
{ MatrixLargeRational tempMat;
  tempMat=this->CartanSymmetric;
  tempMat.Invert();
  roots tempRoots;
  tempRoots.AssignMatrixRows(tempMat);
  output.CreateFromNormals(tempRoots, theGlobalVariables);
}

std::string GeneralizedVermaModuleCharacters::CheckMultiplicitiesVsOrbits
  (GlobalVariables& theGlobalVariables)
{ std::stringstream out;
  int totalDimAffine=this->WeylLarger.GetDim()+this->WeylSmaller.GetDim();
  int smallDim=this->WeylSmaller.GetDim();
  root normal;
  normal.MakeZero(totalDimAffine+1);
  roots newWalls;
  ConeComplex tempComplex;
  tempComplex=this->projectivizedChambeR;
  for (int i=0; i<this->WeylChamberSmallerAlgebra.Normals.size; i++)
  { for (int j=0; j<smallDim; j++)
      normal[j]=this->WeylChamberSmallerAlgebra.Normals[i][j];
    newWalls.AddObjectOnTop(normal);
    tempComplex.splittingNormals.AddObjectOnTopHash(normal);
  }
  tempComplex.indexLowestNonRefinedChamber=0;
  tempComplex.Refine(theGlobalVariables);
  out << "Number chambers with new walls: " << tempComplex.size;
  out << "\n" << tempComplex.ElementToString();
  return out.str();
}

void GeneralizedVermaModuleCharacters::IncrementComputation
  (root& parabolicSel, GlobalVariables& theGlobalVariables)
{ std::stringstream out;
//  this->UpperLimitChambersForDebugPurposes=5;
  PolynomialRationalCoeff::PreferredHashSize=10;
  this->thePauseControlleR.InitComputation();
//  if (false)
  this->ParabolicLeviPartRootSpacesZeroStandsForSelected=parabolicSel;
  if (this->UpperLimitChambersForDebugPurposes==0 || this->theLinearOperators.size==0)
    this->ReadFromDefaultFile(&theGlobalVariables);
  switch (this->computationPhase)
  { case 0:
      this->theParser.theHmm.MakeG2InB3(this->theParser, theGlobalVariables);
      this->initFromHomomorphism(parabolicSel, this->theParser.theHmm, theGlobalVariables);
      this->TransformToWeylProjectiveStep1(theGlobalVariables);
      out << theGlobalVariables.theIndicatorVariables.StatusString1;
      this->TransformToWeylProjectiveStep2(theGlobalVariables);
      out << theGlobalVariables.theIndicatorVariables.StatusString1;
      break;
    case 1:
      this->projectivizedChambeR.Refine(theGlobalVariables);
      this->SortMultiplicities(theGlobalVariables);
      out << this->projectivizedChambeR.ElementToString(false, false);
//      out << theGlobalVariables.theIndicatorVariables.StatusString1;
      break;
    case 2:
      this->ComputeQPsFromChamberComplex(theGlobalVariables);
      out << this->ElementToStringMultiplicitiesReport(theGlobalVariables);
      break;
    case 3:
//      out << this->CheckMultiplicitiesVsOrbits(theGlobalVariables);
      break;
    case 4:
      this->InitTheMaxComputation(theGlobalVariables);
      out << theGlobalVariables.theIndicatorVariables.StatusString1;
      break;
    case 5:
      this->theMaxComputation.FindExtremaParametricStep1(this->thePauseControlleR, true, theGlobalVariables);
      out << theGlobalVariables.theIndicatorVariables.StatusString1;
      break;
    case 6:
      this->theMaxComputation.FindExtremaParametricStep3(this->thePauseControlleR, theGlobalVariables);
      out << theGlobalVariables.theIndicatorVariables.StatusString1;
      break;
    case 7:
      this->theMaxComputation.FindExtremaParametricStep4(this->thePauseControlleR, theGlobalVariables);
      out << theGlobalVariables.theIndicatorVariables.StatusString1;
      break;
    case 8:
      this->theMaxComputation.FindExtremaParametricStep5(this->thePauseControlleR, theGlobalVariables);
      out << theGlobalVariables.theIndicatorVariables.StatusString1;
      break;
    default:
      break;
  }
  this->computationPhase++;
  theGlobalVariables.theIndicatorVariables.StatusString1=out.str();
  if (this->computationPhase>8)
  { theGlobalVariables.theIndicatorVariables.StatusString1=this->PrepareReport(theGlobalVariables);
  }
  theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
  theGlobalVariables.MakeReport();
  if (this->UpperLimitChambersForDebugPurposes<=0)
    if (this->computationPhase < 30)
      this->WriteToDefaultFile(&theGlobalVariables);
  this->thePauseControlleR.ExitComputation();
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

std::string roots::ElementsToInequalitiesString(bool useLatex, bool useHtml, bool LastVarIsConstant, PolynomialOutputFormat& theFormat)
{ std::stringstream out;
  std::string tempS;
  std::string theLetter="x";
  if (useLatex)
    out << "\\begin{array}{l}";
  for (int i=0; i<this->size; i++)
  { root& current=this->TheObjects[i];
    tempS= current.ElementToStringLetterFormat(theFormat, useLatex, LastVarIsConstant);
    if (tempS=="")
      out << "(0";
    out << tempS;
    if (!LastVarIsConstant)
    { if (useLatex)
        out << "\\geq 0\\\\";
      else
        out << "=>0\n";
    } else
    { if (useLatex)
        out << "\\geq " << (-(*current.LastObject())).ElementToString() << "\\\\";
      else
        out << "=>" <<  (-(*current.LastObject())).ElementToString();
    }
    if (tempS=="")
      out << ")";
    if (useHtml)
      out << "<br>";
    else
      out << "\n";
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

int ParserNode::EvaluateWeylAction
  (ParserNode& theNode,
   List<int>& theArgumentList, GlobalVariables& theGlobalVariables,
   bool DualAction, bool useRho, bool useMinusRho)
{ Vector<RationalFunction> theWeight;
  WeylGroup& theWeyl=theNode.owner->theHmm.theRange.theWeyl;
  if (theArgumentList.size!=theWeyl.GetDim())
    return theNode.SetError(theNode.errorDimensionProblem);
  int theDim=theArgumentList.size;
  theNode.impliedNumVars=theNode.GetMaxImpliedNumVarsChildren();
  theWeight.SetSize(theDim);
  for (int i=0; i<theArgumentList.size; i++)
  { theWeight.TheObjects[i]=theNode.owner->TheObjects[theArgumentList.TheObjects[i]].ratFunction.GetElement();
    theWeight.TheObjects[i].SetNumVariablesSubDeletedVarsByOne(theNode.impliedNumVars);
  }
  std::stringstream out;
  theWeyl.ComputeWeylGroup(51840);
//  std::cout << theWeight.ElementToString();
  out << "We denote the Weyl group elements by using their minimal-length expressions using simple reflections.<br>"
  << " In what follows, s_{\\eta_ i} denotes the reflection with respect to the i^th simple root. <br>The elements of the Weyl group are given in Bruhat order, "
  << "i.e. are sorted according to the length of their expression using simple reflections.<br>";
  if (theWeyl.size>=51840)
    out << "Only the first 51840 elements have been computed. <br> If you want a larger computation <br> please use the C++ code directly.";
  out << "Number of elements: " << theWeyl.size << "<br>";
  Vector<RationalFunction> theOrbitElement;
  RationalFunction RFZero;
  RFZero.Nullify(theNode.impliedNumVars, &theGlobalVariables);
  std::stringstream tempStream;
  tempStream << "\\begin{array}{rcl}";
  for (int i=0; i<theWeyl.size; i++)
  { theOrbitElement=theWeight;
    if (!DualAction)
      theWeyl.ActOn<RationalFunction>(i, theOrbitElement, useRho, useMinusRho, RFZero);
    else
    {
    }
    tempStream << theWeyl[i].ElementToString() << "\\circ " << theWeight.ElementToString() << " &=& " << theOrbitElement.ElementToString() << " \\\\ <br>";
  }
  tempStream << "\\end{array}";
  out << CGIspecificRoutines::GetHtmlMathDivFromLatexFormulA(tempStream.str());
  theNode.outputString=out.str();
  theNode.ExpressionType=theNode.typeString;
  return theNode.errorNoError;
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
  ParallelComputing::CheckPointerCounters();
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
    ParallelComputing::CheckPointerCounters();
  #endif
            this->TheObjects[this->PreferredNextChambers.TheObjects[0]]=0;
          }
      this->PreferredNextChambers.PopIndexShiftUp(0);
      if (this->flagAnErrorHasOcurredTimeToPanic)
        this->ComputeDebugString();
    }
}

void GeneralizedVermaModuleCharacters::GetSubFromNonParamArray
(MatrixLargeRational& output, root& outputTranslation, roots& NonParams, int numParams)
{ //reminder: the very last variable comes from the projectivization and contributes to the translation only!
  int numNonParams=NonParams.size;
  output.init(numParams+numNonParams-1, numParams-1);
  outputTranslation.MakeZero(numParams+numNonParams-1);
  output.NullifyAll();
  for (int l=0; l<numNonParams; l++)
  { for (int k=0; k<numParams-1; k++)
      output.elements[l][k]= NonParams.TheObjects[l].TheObjects[k];
    outputTranslation.TheObjects[l]=*NonParams.TheObjects[l].LastObject();
  }
  for (int l=0; l<numParams-1; l++)
    output.elements[l+numNonParams][l]= 1;
}

void ConeLatticeAndShiftMaxComputation::init
(root& theNEq, Cone& startingCone, Lattice& startingLattice, root& startingShift)
{ ConeLatticeAndShift theCLS;
  theCLS.theProjectivizedCone=startingCone;
  theCLS.theLattice=startingLattice;
  theCLS.theShift=startingShift;
  this->numNonParaM=0;
  this->numProcessedNonParam=0;
  this->LPtoMaximizeLargerDim.size=0;
  this->LPtoMaximizeSmallerDim.size=0;
  this->theStartingRepresentative=startingShift;
  this->theFinalRepresentatives.size=0;
  this->complexStartingPerRepresentative.size=0;
  this->complexRefinedPerRepresentative.size=0;
  this->theConesLargerDim.AddObjectOnTop(theCLS);
  this->LPtoMaximizeLargerDim.AddObjectOnTop(theNEq);
  this->IsInfinity.initFillInObject(1, false);
}

int ParserNode::EvaluateFindExtremaInDirectionOverLattice
    (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ PolynomialRationalCoeff& theLinPoly=theNode.owner->TheObjects[theArgumentList.TheObjects[0]].polyValue.GetElement();
 // int numNonParams =  theNode.owner->TheObjects[theArgumentList.TheObjects[1]].intValue;
  //int numParams =  theNode.owner->TheObjects[theArgumentList.TheObjects[2]].intValue;
  ParserNode& theShiftNode= theNode.owner->TheObjects[theArgumentList[1]];
  Lattice& currentLattice=theNode.owner->TheObjects[theArgumentList[2]].theLattice.GetElement();
  Cone& currentCone=theNode.owner->TheObjects[theArgumentList[3]].theCone.GetElement();
  int numNonParam=theNode.owner->TheObjects[theArgumentList[4]].intValue;
  root theShift;
  root theNEqBeforeCheck, theNEq;
  if(!theLinPoly.IsLinearGetRootConstantTermLastCoordinate(theNEqBeforeCheck, (Rational) 0))
    return theNode.SetError(theNode.errorImplicitRequirementNotSatisfied);
  if(theNEqBeforeCheck.size>currentCone.GetDim())
    return theNode.SetError(theNode.errorDimensionProblem);
  else
  { theNEq.MakeZero(currentCone.GetDim());
    for (int i=0; i<theNEqBeforeCheck.size; i++)
      theNEq[i]=theNEqBeforeCheck[i];
  }
  if (!theShiftNode.GetRootRationalDontUseForFunctionArguments(theShift, theGlobalVariables))
    return theNode.SetError(errorDimensionProblem);
  if (theShift.size!=theNEq.size-1)
    return theNode.SetError(errorDimensionProblem);
/*  if (theNEq.size!=numNonParams+numParams+1)
    return theNode.SetError(theNode.errorBadOrNoArgument);*/
  if (numNonParam>theShift.size-1)
    return theNode.SetError(errorImplicitRequirementNotSatisfied);
  GeneralizedVermaModuleCharacters tempChars;
  List<int> nodesCurrentRoot;
  List<roots> outputParamChambers, outputNonParamVertices;
  PolynomialOutputFormat tempFormat;
  std::stringstream out;
  PolynomialOutputFormat theFormat;
  out << "Input data: normal: " << theNEq.ElementToString()
//  << "; numNonParams: " << numNonParams << "; numParams: " << numParams
  << "; cone: " << currentCone.ElementToString(false, true, false, true, theFormat);
  std::cout << "Input data: normal: " << theNEq.ElementToString()
//  << "; numNonParams: " << numNonParams << "; numParams: " << numParams
  << "; cone: " << currentCone.ElementToString(false, true, false, true, theFormat);
  ConeLatticeAndShiftMaxComputation theComputation;
  theComputation.init(theNEq, currentCone, currentLattice, theShift);
  Controller pauseController;
  theComputation.numNonParaM=numNonParam;
  theComputation.FindExtremaParametricStep1(pauseController, false, theGlobalVariables);
  for (int i=0; i<MathRoutines::Minimum(theShift.size, theFormat.alphabet.size); i++)
//    if (i<numNonParam)
  { std::stringstream tempStream;
    tempStream << "x_{" << i+1+numNonParam << "}";
    theFormat.alphabet[i]=tempStream.str();
  } /*else
  { std::stringstream tempStream;
    tempStream << "y_{" << i-numNonParam+1 << "}";
    theFormat.alphabet[i]=tempStream.str();
  }*/
  out << "<hr><hr><hr>" << theComputation.ElementToString(theFormat);
  theNode.outputString=out.str();
  theNode.ExpressionType=theNode.typeString;
  return theNode.errorNoError;
}

int ParserNode::EvaluateSliceCone
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ Cone& theCone=theNode.owner->TheObjects[theArgumentList.TheObjects[0]].theCone.GetElement();
  root theDirection;
  if (! theNode.owner->TheObjects[theArgumentList.TheObjects[1]].GetRootRationalDontUseForFunctionArguments(theDirection, theGlobalVariables))
    return theNode.errorProgramming;
  if (theCone.GetDim()!=theDirection.size)
    return theNode.errorDimensionProblem;
  if (theCone.flagIsTheZeroCone || theCone.Normals.size==0)
  { theNode.outputString="Error: the cone to be sliced is either empty or is the entire space.";
    return theNode.errorImplicitRequirementNotSatisfied;
  }
  ConeComplex theComplex;
  PolynomialOutputFormat tempFormat;
  theCone.SliceInDirection(theDirection, theComplex, theGlobalVariables);
  theNode.outputString=theComplex.DrawMeToHtmlProjective(theGlobalVariables.theDrawingVariables, tempFormat);
//  theNode.outputString=theComplex.ElementToString(false, true);
  theNode.ExpressionType=theNode.typeString;
  return theNode.errorNoError;
}

bool Cone::ProduceNormalFromTwoNormalsAndSlicingDirection
  (root& SlicingDirection, root& normal1, root& normal2, root& output)
{ // we are looking for a normal n of the form n=t1*normal1+t2*normal2
  // such that  <t1*normal1+t2*normal2, slicingDirection>=0
  Rational normal1ScalarDirection=normal1.ScalarEuclidean(SlicingDirection);
  if (normal1ScalarDirection.IsEqualToZero())
  { output=normal1;
    return false;
  }
  //from now on we assume t2=1;
  Rational t1=-normal2.ScalarEuclidean(SlicingDirection)/normal1ScalarDirection;
  output=normal2;
  output+=normal1*t1;
  output.ScaleByPositiveRationalToIntegralMinHeight();
  return true;
}

void Cone::SliceInDirection
  (root& theDirection, ConeComplex& output, GlobalVariables& theGlobalVariables)
{ output.init();
  output.AddNonRefinedChamberOnTopNoRepetition(*this);
  output.slicingDirections.AddObjectOnTop(theDirection);
  //std::cout <<output.ElementToString(false, true);
  output.Refine(theGlobalVariables);
  //std::cout <<output.ElementToString(false, true);
}

void Lattice::ApplyLinearMap(MatrixLargeRational& theMap, Lattice& output)
{ roots tempRoots;
  tempRoots.AssignMatrixRows(this->basisRationalForm);
  theMap.ActOnRoots(tempRoots);
  output.MakeFromRoots(tempRoots);
}

std::string ConeLatticeAndShiftMaxComputation::ElementToString
  (PolynomialOutputFormat& theFormat)
{ std::stringstream out;
  out << "<hr>Resulting lattice: " << this->theFinalRougherLattice.ElementToString(true, false) << "<hr><hr>";
/*  if (this->complexStartingPerRepresentative.size>0)
  { out << "<hr> Non-refined complex per representative:<br>\n ";
    for (int i=0; i<this->complexStartingPerRepresentative.size; i++)
    { out << "Lattice+shift="  << this->theFinalRepresentatives[i].ElementToString() << " + " << this->theFinalRougherLattice.ElementToString();
      out << "<br>\n" << this->complexStartingPerRepresentative[i].ElementToString(false, true);
      out << "the function we need to max: " << this->LPtoMaximizeSmallerDim[i].ElementToString();
    }
  }*/
  out << "<hr><hr>Cones not processed(number of cones " << this->theConesLargerDim.size << "):\n<hr>\n";
  DrawingVariables theDrawingVariables;
  PolynomialRationalCoeff tempP;
  for (int i=0; i<this->theConesLargerDim.size; i++)
  { out << "";// << this->theConesLargerDim[i].ElementToString(theFormat);
    //out << "<br>" << this->LPtoMaximizeLargerDim[i].ElementToString();
    theDrawingVariables.theBuffer.init();
    out << "<br>" << this->theConesLargerDim[i].theProjectivizedCone.DrawMeToHtmlLastCoordAffine(theDrawingVariables, theFormat);
    out << "<br>over " << this->theConesLargerDim[i].theShift.ElementToString() << " + " << this->theConesLargerDim[i].theLattice.ElementToString();
    tempP.MakeLinPolyFromRootLastCoordConst(this->LPtoMaximizeLargerDim[i]);
    out << "<br>the function we have maxed, as a function of the remaining variables, is: " << tempP.ElementToString(theFormat) << "<hr><hr>";
  }
  if (this->theConesSmallerDim.size>0)
  { out << "<br>Cones processed: <br>";
    for (int i=0; i<this->theConesSmallerDim.size; i++)
    { out << this->theConesSmallerDim[i].ElementToString(theFormat);
      //out << "<br>" << this->LPtoMaximizeSmallerDim[i].ElementToString();
      theDrawingVariables.theBuffer.init();
      out << this->theConesSmallerDim[i].theProjectivizedCone.DrawMeToHtmlLastCoordAffine(theDrawingVariables, theFormat);
    }
  }
  return out.str();
}

void ConeLatticeAndShiftMaxComputation::FindExtremaParametricStep3
    (Controller& thePauseController, GlobalVariables& theGlobalVariables)
{ this->theFinalRougherLattice=this->theConesLargerDim[0].theLattice;
  theGlobalVariables.theIndicatorVariables.ProgressReportStringsNeedRefresh=true;
  for (int i=1; i<this->theConesLargerDim.size; i++)
  { this->theFinalRougherLattice.IntersectWith(this->theConesLargerDim[i].theLattice);
    std::stringstream tempStream;
    tempStream << "intersecing lattice " << i+1 << " out of " << this->theConesLargerDim.size;
    theGlobalVariables.theIndicatorVariables.ProgressReportStrings[0]=tempStream.str();
    theGlobalVariables.MakeReport();
  }
  this->theFinalRepresentatives.size=0;
  roots tempRoots, tempRoots2;
  tempRoots2.SetSize(1);
  for (int i=0; i<this->theConesLargerDim.size; i++)
  { tempRoots2[0]=this->theConesLargerDim[i].theShift;
    this->theConesLargerDim[i].theLattice.GetAllRepresentativesProjectingDownTo(this->theFinalRougherLattice, tempRoots2, tempRoots);
    this->theFinalRepresentatives.AddRootSnoRepetition(tempRoots);
    std::stringstream tempStream;
    tempStream << "Computing representative " << i+1 << " out of " << this->theConesLargerDim.size;
    tempStream << "\nSo far " << this->theFinalRepresentatives.size << " found.";
    theGlobalVariables.theIndicatorVariables.ProgressReportStrings[1]=tempStream.str();
    theGlobalVariables.MakeReport();
  }
  this->complexStartingPerRepresentative.SetSize(this->theFinalRepresentatives.size);
  this->startingLPtoMaximize.SetSize(this->theFinalRepresentatives.size);
  this->finalMaxima.SetSize(this->theFinalRepresentatives.size);
  root tempRoot;
  for (int i=0; i<this->theFinalRepresentatives.size; i++)
    for (int j=0; j<this->theConesLargerDim.size; j++)
    { tempRoot=this->theFinalRepresentatives[i];
      this->theConesLargerDim[j].theLattice.ReduceVector(tempRoot);
      if (tempRoot==this->theConesLargerDim[j].theShift)
      { this->complexStartingPerRepresentative[i].AddObjectOnTopHash(this->theConesLargerDim[j].theProjectivizedCone);
        this->startingLPtoMaximize[i].AddObjectOnTop(this->LPtoMaximizeLargerDim[j]);
      }
    }
}

/*void ConeLatticeAndShiftMaxComputation::FindExtremaParametricStep2TrimChamberForMultOne
    (Controller& thePauseController, GlobalVariables& theGlobalVariables)
{ Cone trimmedCone;
  roots tempRoots;
  root multFreeWall;
  int startingNumCones=this->theConesLargerDim.size;
  int numKilledCones=0;
  for (int i=0; i<this->theConesLargerDim.size; i++)
  { trimmedCone.Normals=this->theConesLargerDim[i].theProjectivizedCone.Normals;
    multFreeWall=this->LPtoMaximizeLargerDim[i];
    multFreeWall.MinusRoot();
    *multFreeWall.LastObject()+=1;
    trimmedCone.Normals.AddObjectOnTop(multFreeWall);
    trimmedCone.CreateFromNormals(trimmedCone.Normals, theGlobalVariables);
    if (!trimmedCone.flagIsTheZeroCone)
      this->theConesLargerDim[i].theProjectivizedCone=trimmedCone;
    else
    { this->theConesLargerDim.PopIndexSwapWithLast(i);
      this->LPtoMaximizeLargerDim.PopIndexSwapWithLast(i);
      i--;
      numKilledCones++;
    }
    std::stringstream tempStream;
    tempStream << "Processed " << i+numKilledCones << " out of " << startingNumCones;
    tempStream << "\nKilled " << numKilledCones << " cones so far";
    theGlobalVariables.theIndicatorVariables.ProgressReportStrings[2]=tempStream.str();
    theGlobalVariables.MakeReport();
  }
}
*/

void ConeLatticeAndShiftMaxComputation::FindExtremaParametricStep4
    (Controller& thePauseController, GlobalVariables& theGlobalVariables)
{ this->complexRefinedPerRepresentative.SetSize(this->theFinalRepresentatives.size);
  this->theMaximaCandidates.SetSize(this->theFinalRepresentatives.size);
  for (int i=0; i<this->theFinalRepresentatives.size; i++)
  { ConeComplex& currentComplex= this->complexRefinedPerRepresentative[i];
    currentComplex.initFromCones(this->complexStartingPerRepresentative[i], true, theGlobalVariables);
    std::stringstream tempStream;
    tempStream << "Processing representative " << i+1 << " out of " << this->theFinalRepresentatives.size;
    theGlobalVariables.theIndicatorVariables.ProgressReportStrings[0]=tempStream.str();
    theGlobalVariables.MakeReport();
    currentComplex.Refine(theGlobalVariables);
    this->theMaximaCandidates[i].SetSize(currentComplex.size);
    for (int j=0; j<currentComplex.size; j++)
      for (int k=0; k<this->complexStartingPerRepresentative[k].size; k++)
        if (this->complexStartingPerRepresentative[i][k].IsInCone(currentComplex[j].GetInternalPoint()))
          this->theMaximaCandidates[i][j].AddOnTopNoRepetition(this->startingLPtoMaximize[i][k]);
  }
}

void ConeLatticeAndShiftMaxComputation::FindExtremaParametricStep5
    (Controller& thePauseController, GlobalVariables& theGlobalVariables)
{ this->finalMaximaChambers.SetSize(this->theFinalRepresentatives.size);
  this->finalMaximaChambersIndicesMaxFunctions.SetSize(this->theFinalRepresentatives.size);
  for (int i=0; i<1; i++ )//this->theFinalRepresentatives.size; i++)
  { this->finalMaximaChambers[i].SetSize(this->complexRefinedPerRepresentative[i].size);
    this->theFinalRepresentatives[i].SetSize(this->complexRefinedPerRepresentative[i].size);
    for(int j=0; j<1; j++)//this->complexRefinedPerRepresentative[i].size; j++)
    { Cone& currentCone=this->complexRefinedPerRepresentative[i][j];
      this->finalMaximaChambers[i][j].init();
      this->finalMaximaChambers[i][j]
      .findMaxLFOverConeProjective
      (currentCone, this->theMaximaCandidates[i][j],
      this->finalMaximaChambersIndicesMaxFunctions[i][j], theGlobalVariables);
     // tempComplex.a
    }
  }
}

void ConeLatticeAndShiftMaxComputation::FindExtremaParametricStep1
    (Controller& thePauseController, bool assumeNewConesHaveSufficientlyManyProjectiveVertices, GlobalVariables& theGlobalVariables)
{ //std::cout << "<hr>starting complex: " << this->ElementToString();
  PolynomialOutputFormat tempFormat;
  for (; this->numProcessedNonParam<this->numNonParaM; this->numProcessedNonParam++)
  { while(this->theConesLargerDim.size>0)
    { ConeLatticeAndShift& currentCLS=*this->theConesLargerDim.LastObject();
      assert(this->LPtoMaximizeLargerDim.LastObject()->size==currentCLS.GetDimAffine()+1);
      if (!this->LPtoMaximizeLargerDim.LastObject()->IsEqualToZero())
        currentCLS.FindExtremaInDirectionOverLatticeOneNonParam
          (*this->LPtoMaximizeLargerDim.LastObject(), this->LPtoMaximizeSmallerDim, this->theConesSmallerDim,
           theGlobalVariables);
      this->theConesLargerDim.size--;
      this->LPtoMaximizeLargerDim.size--;
      thePauseController.SafePointDontCallMeFromDestructors();
      std::stringstream tempStream1, tempStream2, tempStream3;
      tempStream1 << "Processing " << this->numProcessedNonParam+1 << " out of " << this->numNonParaM;
      tempStream2 << "Remaining cones: " << this->theConesLargerDim.size;
      tempStream3 << "Cones smaller dim total: " << this->theConesSmallerDim.size;
      theGlobalVariables.theIndicatorVariables.ProgressReportStrings[0]=tempStream1.str();
      theGlobalVariables.theIndicatorVariables.ProgressReportStrings[1]=tempStream2.str();
      theGlobalVariables.theIndicatorVariables.ProgressReportStrings[2]=tempStream3.str();
      theGlobalVariables.MakeReport();
    }
    //std::cout << "<hr><hr>" << this->ElementToString();
    std::stringstream out;
    theGlobalVariables.theIndicatorVariables.StatusString1=out.str();
    theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
    theGlobalVariables.MakeReport();
    this->LPtoMaximizeLargerDim=this->LPtoMaximizeSmallerDim;
    this->theConesLargerDim=this->theConesSmallerDim;
    this->theConesSmallerDim.size=0;
    this->LPtoMaximizeSmallerDim.size=0;
  }
}

void ConeLatticeAndShift::FindExtremaInDirectionOverLatticeOneNonParamDegenerateCase
(root& theLPToMaximizeAffine, roots& outputAppendLPToMaximizeAffine,
 List<ConeLatticeAndShift>& outputAppend, MatrixLargeRational& theProjectionLatticeLevel,
 GlobalVariables& theGlobalVariables)
{ MatrixLargeRational matVertices;
  matVertices.AssignRootsToRowsOfMatrix(this->theProjectivizedCone.Vertices);
  roots theNormals;
  matVertices.FindZeroEigenSpace(theNormals, theGlobalVariables);
  root preferredNormal;
  for (int i=0; i<theNormals.size; i++)
    if (!theNormals[i][0].IsEqualToZero())
    { preferredNormal=theNormals[i];
      break;
    }
  Rational firstCoord=preferredNormal[0];
  preferredNormal.ShiftToTheLeftOnePos();
  preferredNormal/=-firstCoord;
  ConeLatticeAndShift tempCLS;
  roots newNormals=this->theProjectivizedCone.Normals;
  Rational firstCoordNewNormal, tempRat;
  for (int i=0; i<newNormals.size; i++)
  { firstCoordNewNormal=newNormals[i][0];
    newNormals[i].ShiftToTheLeftOnePos();
    newNormals[i]+=preferredNormal*firstCoordNewNormal;
  }
  //bool tempBool=
  tempCLS.theProjectivizedCone.CreateFromNormals(newNormals, theGlobalVariables);
  tempCLS.theShift=this->theShift;
  tempCLS.theShift.ShiftToTheLeftOnePos();
  this->theLattice.ApplyLinearMap(theProjectionLatticeLevel, tempCLS.theLattice);
  root tempRoot;
  tempRoot=theLPToMaximizeAffine.GetShiftToTheLeftOnePosition();
  //root tempRoot2;
  //tempRoot2=preferredNormal;
  tempRoot+=(preferredNormal*theLPToMaximizeAffine[0]);
  if (!tempCLS.theProjectivizedCone.flagIsTheZeroCone)
  { outputAppend.AddObjectOnTop(tempCLS);
    outputAppendLPToMaximizeAffine.AddObjectOnTop(tempRoot);
  }
}

void ConeLatticeAndShift::FindExtremaInDirectionOverLatticeOneNonParam
(root& theLPToMaximizeAffine, roots& outputAppendLPToMaximizeAffine,
 List<ConeLatticeAndShift>& outputAppend,
 GlobalVariables& theGlobalVariables)
{ root direction;
  PolynomialOutputFormat theFormat;
  int theDimProjectivized=this->GetDimProjectivized();
  MatrixLargeRational theProjectionLatticeLevel;
  MatrixLargeRational theProjectionAffine;
  theProjectionLatticeLevel.init(theDimProjectivized-2, theDimProjectivized-1);
  theProjectionLatticeLevel.NullifyAll();
  for (int i=0; i<theProjectionLatticeLevel.NumRows; i++)
    theProjectionLatticeLevel.elements[i][i+1]=1;
  direction.MakeEi(theDimProjectivized, 0);
  if (!this->theProjectivizedCone.Vertices.LinSpanContainsRoot(direction, theGlobalVariables))
  { this->FindExtremaInDirectionOverLatticeOneNonParamDegenerateCase
      (theLPToMaximizeAffine, outputAppendLPToMaximizeAffine, outputAppend, theProjectionLatticeLevel,
       theGlobalVariables);
    return;
  }
  if (outputAppend.size>=10)
  { std::stringstream tempStream;
    tempStream << "<hr><hr><hr><hr>The bad cone:" << this->theProjectivizedCone.ElementToString(theFormat);
    theGlobalVariables.theIndicatorVariables.StatusString1=tempStream.str();
    theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
    theGlobalVariables.MakeReport();
  }
  ConeComplex complexBeforeProjection;
  complexBeforeProjection.init();
  complexBeforeProjection.AddNonRefinedChamberOnTopNoRepetition(this->theProjectivizedCone);
  if (direction.ScalarEuclidean(theLPToMaximizeAffine).IsNegative())
    direction.MinusRoot();
  complexBeforeProjection.slicingDirections.AddObjectOnTop(direction);
  complexBeforeProjection.slicingDirections.AddObjectOnTop(-direction);
//  std::cout << "<hr>complex before refining: <br>\n" << complexBeforeProjection.ElementToString(false, true);
  complexBeforeProjection.Refine(theGlobalVariables);
//  std::cout << "<hr>complex before projection: <br>\n" << complexBeforeProjection.ElementToString(false, true);
  root tempRoot, extraEquation, exitNormalAffine, enteringNormalAffine, exitNormalLatticeLevel, enteringNormalLatticeLevel, exitNormalShiftedAffineProjected;
  root directionSmallerDim, directionSmallerDimOnLattice;
  root theShiftReduced=this->theShift;
  this->theLattice.ReduceVector(theShiftReduced);
  roots exitRepresentatives, exitWallsShifted;
  roots currentShifts;
  Lattice exitRougherLattice;
  ConeLatticeAndShift tempCLS;
  directionSmallerDim.MakeEi(theDimProjectivized-1, 0);
//  std::cout << "<hr>";
  roots theNewNormals;
  for (int i=0; i<complexBeforeProjection.size; i++)
  { Cone& currentCone=complexBeforeProjection.TheObjects[i];
    int numNonPerpWalls=0;
    theNewNormals.size=0;
    bool foundEnteringNormal=false;
    bool foundExitNormal=false;
    for (int j=0; j<currentCone.Normals.size; j++)
    { root& currentNormal=currentCone.Normals.TheObjects[j];
      if (currentNormal[0].IsEqualToZero())
      { tempRoot=currentNormal.GetShiftToTheLeftOnePosition();
        theNewNormals.AddObjectOnTop(tempRoot);
      } else
      { //std::cout << "<hr>";
        //std::cout << "<br>currentWall: " << currentNormal.ElementToString();
        numNonPerpWalls++;
        assert(numNonPerpWalls<3);
        if (!currentNormal.ScalarEuclidean(direction).IsPositive() && !foundExitNormal)
        { theLattice.GetRougherLatticeFromAffineHyperplaneDirectionAndLattice
            (directionSmallerDim, directionSmallerDimOnLattice, theShift, currentNormal, exitRepresentatives,
             exitWallsShifted, exitRougherLattice, theGlobalVariables);
          exitNormalAffine=currentNormal;
          exitNormalLatticeLevel=exitNormalAffine;
          exitNormalLatticeLevel.SetSize(theDimProjectivized-1);
          foundExitNormal=true;
        } else
        { enteringNormalAffine=currentNormal;
          foundEnteringNormal=true;
        }
      }
    }
    exitRougherLattice.ApplyLinearMap(theProjectionLatticeLevel, tempCLS.theLattice);
    for (int j=0; j<exitRepresentatives.size; j++)
    { exitRepresentatives[j]+=theShiftReduced;
      Lattice::GetClosestPointInDirectionOfTheNormalToAffineWallMovingIntegralStepsInDirection
        (exitRepresentatives[j], exitNormalAffine, directionSmallerDimOnLattice, exitRepresentatives[j]);
    }
    std::cout << "<hr><hr><hr>" << currentCone.ElementToString(false, true, theFormat);
    std::cout << "<br>Entering normal: " << ((foundEnteringNormal) ? enteringNormalAffine.ElementToString() : "not found");
    std::cout << "<br>Exit normal: " << ((foundExitNormal) ? exitNormalAffine.ElementToString() : "not found");
    std::cout << "<br>The shifted lattice representatives: " << exitRepresentatives.ElementToString() << "<br>exitNormalsShiftedAffineProjected";
    assert(theNewNormals.size>0);
    for (int j=0; j<exitRepresentatives.size; j++)
    { tempCLS.theProjectivizedCone.Normals=theNewNormals;
      exitNormalShiftedAffineProjected=exitNormalAffine.GetShiftToTheLeftOnePosition();
      *exitNormalShiftedAffineProjected.LastObject()=-exitNormalLatticeLevel.ScalarEuclidean(exitRepresentatives[j]);
      std::cout << exitNormalShiftedAffineProjected.ElementToString() << ", ";
      if (foundEnteringNormal)
      { extraEquation= enteringNormalAffine.GetShiftToTheLeftOnePosition();
        extraEquation-=(exitNormalShiftedAffineProjected*enteringNormalAffine[0])/exitNormalAffine[0];
        std::cout << "extra equation: " << extraEquation.ElementToString() << ", ";
        tempCLS.theProjectivizedCone.Normals.AddObjectOnTop(extraEquation);
      }
      Rational tempRat=theLPToMaximizeAffine[0];
      tempRoot=theLPToMaximizeAffine.GetShiftToTheLeftOnePosition();
      tempRoot-=exitNormalShiftedAffineProjected*theLPToMaximizeAffine[0]/exitNormalAffine[0];
      outputAppendLPToMaximizeAffine.AddObjectOnTop(tempRoot);
      assert(tempCLS.theProjectivizedCone.Normals.size>0);
      roots tempTempRoots=tempCLS.theProjectivizedCone.Normals;
      //bool tempBool=
      tempCLS.theProjectivizedCone.CreateFromNormals(tempTempRoots, theGlobalVariables);
      /*if (!tempBool)
      { std::stringstream tempStream;
        tempStream << "The bad starting cone (cone number " << i+1 << "):" << this->ElementToString(theFormat) << "<hr><hr><hr><hr>The bad cone:" << tempCLS.ElementToString(theFormat);
        tempStream << "<br>\n\n" << this->theProjectivizedCone.Normals.ElementToString(false, false, false);
        tempStream << "\n\n<br>\n\n" << complexBeforeProjection.ElementToString(false, true);
        if (!foundEnteringNormal)
          tempStream << "<hr>not found entering normal!!!!!!<hr>";
        if (!foundExitNormal)
          tempStream << "<hr>not found exit normal!!!!!!<hr>";
        Cone tempCone;
        tempCone.CreateFromNormals(tempTempRoots, theGlobalVariables);
        tempStream << "\n\n\n\n<br><br><hr>The bad normals: " << tempTempRoots.ElementToString();
        tempStream << "\n\n\n\n<br><br><hr>The bad normals after creation: " << tempCLS.theProjectivizedCone.Normals.ElementToString();
        theGlobalVariables.theIndicatorVariables.StatusString1=tempStream.str();
        theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
        theGlobalVariables.MakeReport();
        for (int i=0; i<10000000; i++)
          if (i%3==0)
            i=i+2;
        while(true){}
      }*/
      //assert(tempBool);
      //std::cout << tempCLS.theProjectivizedCone.ElementToString(false, true, true, true, theFormat);
      if (!tempCLS.theProjectivizedCone.flagIsTheZeroCone)
      { theProjectionLatticeLevel.ActOnAroot(exitRepresentatives[j], tempCLS.theShift);
        outputAppend.AddObjectOnTop(tempCLS);
        if (tempCLS.GetDimProjectivized()==0)
        { theGlobalVariables.theIndicatorVariables.StatusString1=tempTempRoots.ElementToString();
          theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
          theGlobalVariables.MakeReport();
          while(true) {}
        }
        assert(tempCLS.GetDimProjectivized()==theDimProjectivized-1);
      }
    }
  }
//  std::cout << "<hr><hr><hr>";
//  for (int i=0; i<outputAppend.size; i++)
//  { std::cout << outputAppend[i].theProjectivizedCone.ElementToString(false, true, true, true);
//  }
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
      tempRoot.ScaleByPositiveRationalToIntegralMinHeight();
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
(int indexChamberBeingRefined, bool weAreSlicingInDirection, bool weAreChopping, root& killerNormal, GlobalVariables& theGlobalVariables)
{ Cone& myDyingCone=this->TheObjects[indexChamberBeingRefined];
/*  if (!myDyingCone.flagHasSufficientlyManyVertices)
  { this->flagChambersHaveTooFewVertices=true;
    return false;
  }*/
  Cone& newPlusCone= theGlobalVariables.coneBuffer1NewSplit;
  Cone& newMinusCone=theGlobalVariables.coneBuffer2NewSplit;
  bool needToRecomputeVertices=(myDyingCone.Normals.GetRankOfSpanOfElements(theGlobalVariables)<this->GetDim());
//  newPlusCone.flagHasSufficientlyManyVertices=true;
//  newMinusCone.flagHasSufficientlyManyVertices=true;
  newPlusCone.LowestIndexNotCheckedForSlicingInDirection=myDyingCone.LowestIndexNotCheckedForSlicingInDirection;
  newMinusCone.LowestIndexNotCheckedForSlicingInDirection=myDyingCone.LowestIndexNotCheckedForSlicingInDirection;
  newPlusCone.LowestIndexNotCheckedForChopping=myDyingCone.LowestIndexNotCheckedForChopping;
  newMinusCone.LowestIndexNotCheckedForChopping=myDyingCone.LowestIndexNotCheckedForChopping;
  newPlusCone.flagIsTheZeroCone=false;
  newMinusCone.flagIsTheZeroCone=false;
  if (weAreChopping)
  { newPlusCone.LowestIndexNotCheckedForChopping++;
    newMinusCone.LowestIndexNotCheckedForChopping++;
  }
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
  if (needToRecomputeVertices)
  { newPlusCone.CreateFromNormals(newPlusCone.Normals, theGlobalVariables);
    newMinusCone.CreateFromNormals(newMinusCone.Normals, theGlobalVariables);
  }
  this->AddNonRefinedChamberOnTopNoRepetition(newPlusCone);
  this->AddNonRefinedChamberOnTopNoRepetition(newMinusCone);
  return true;
}

void ConeComplex::PopChamberSwapWithLast(int index)
{ this->PopIndexSwapWithLastHash(index);
}

bool ConeComplex::AddNonRefinedChamberOnTopNoRepetition(Cone& newCone)
{ newCone.Normals.QuickSortAscending();
  return this->AddObjectOnTopNoRepetitionOfObjectHash(newCone);
}

void ConeComplex::RefineOneStep(GlobalVariables& theGlobalVariables)
{ if (this->indexLowestNonRefinedChamber>=this->size)
    return;
  Cone& currentCone=this->TheObjects[this->indexLowestNonRefinedChamber];
  for (; currentCone.LowestIndexNotCheckedForChopping<this->splittingNormals.size; currentCone.LowestIndexNotCheckedForChopping++)
   if (this->SplitChamber(this->indexLowestNonRefinedChamber, false, true, this->splittingNormals.TheObjects[currentCone.LowestIndexNotCheckedForChopping], theGlobalVariables))
      return;
  root currentNewWall;
  for (; currentCone.LowestIndexNotCheckedForSlicingInDirection< this->slicingDirections.size; currentCone.LowestIndexNotCheckedForSlicingInDirection++)
    for (int i=0; i<currentCone.Normals.size; i++)
      if (this->slicingDirections.TheObjects[currentCone.LowestIndexNotCheckedForSlicingInDirection].ScalarEuclidean(currentCone.Normals.TheObjects[i]).IsPositive())
        for (int j=i+1; j<currentCone.Normals.size; j++)
          if (this->slicingDirections.TheObjects[currentCone.LowestIndexNotCheckedForSlicingInDirection].ScalarEuclidean(currentCone.Normals.TheObjects[j]).IsPositive())
            if (currentCone.ProduceNormalFromTwoNormalsAndSlicingDirection(this->slicingDirections.TheObjects[currentCone.LowestIndexNotCheckedForSlicingInDirection], currentCone.Normals.TheObjects[i], currentCone.Normals.TheObjects[j], currentNewWall))
              if (this->SplitChamber(this->indexLowestNonRefinedChamber, true, false, currentNewWall, theGlobalVariables))
                return;
  this->indexLowestNonRefinedChamber++;
}

void ConeComplex::InitFromDirectionsAndRefine(roots& inputVectors, GlobalVariables& theGlobalVariables)
{ this->init();
  Cone startingCone;
  startingCone.CreateFromVertices(inputVectors, theGlobalVariables);
  this->AddNonRefinedChamberOnTopNoRepetition(startingCone);
  this->slicingDirections.AddListOnTop(inputVectors);
  this->Refine(theGlobalVariables);
}

void ConeComplex::Sort(GlobalVariables& theGlobalVariables)
{ List<Cone> tempList;
  tempList=*this;
  tempList.QuickSortAscending();
  this->ClearTheObjects();
  for (int i=0; i<tempList.size; i++)
    this->AddObjectOnTopHash(tempList[i]);
}

void ConeComplex::RefineAndSort(GlobalVariables& theGlobalVariables)
{ this->Refine(theGlobalVariables);
  this->Sort(theGlobalVariables);
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

void Cone::ComputeVerticesFromNormalsNoFakeVertices
(GlobalVariables& theGlobalVariables)
{ this->Vertices.size=0;
  Selection theSel, nonPivotPoints;
  for (int i=0; i<this->Normals.size; i++)
    this->Normals.TheObjects[i].ScaleByPositiveRationalToIntegralMinHeight();
  int theDim=this->Normals.TheObjects[0].size;
  theSel.init(this->Normals.size);
  int numCycles=theSel.GetNumCombinationsFixedCardinality(theDim-1);
  if (theDim==1)
  { numCycles=0;
    bool foundNegative=false;
    bool foundPositive=false;
    for (int i=0; i<this->Normals.size; i++)
    { if(this->Normals[i].IsPositiveOrZero())
        foundPositive=true;
      if (this->Normals[i].IsNegativeOrZero())
        foundNegative=true;
    }
    if (foundNegative xor foundPositive)
    { this->Vertices.SetSizeMakeMatrix(1,1);
      if (foundNegative)
        this->Vertices[0][0]=-1;
      else
        this->Vertices[0][0]=1;
    }
    return;
  }
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
      { tempRoot.ScaleByPositiveRationalToIntegralMinHeight();
        this->Vertices.AddOnTopNoRepetition(tempRoot);
      }
    }
  }
}

bool Cone::EliminateFakeNormalsUsingVertices
(int theDiM, int numAddedFakeWalls, GlobalVariables& theGlobalVariables)
{ if(this->Vertices.size==0)
  { this->flagIsTheZeroCone=true;
    this->Normals.SetSize(0);
    return false;
  }
  roots& verticesOnWall=theGlobalVariables.rootsGeneralPurposeBuffer2;
  if (numAddedFakeWalls!=0)
  { //we modify the normals so that they lie in the subspace spanned by the vertices
    MatrixLargeRational tempMat, matNormals, gramMatrixInverted;
    tempMat.AssignRootsToRowsOfMatrix(this->Vertices);
    roots NormalsToSubspace;
    tempMat.FindZeroEigenSpace(NormalsToSubspace, theGlobalVariables);
    if (NormalsToSubspace.size>0)
    { matNormals.AssignRootsToRowsOfMatrix(NormalsToSubspace);
//      std::cout << "<br>Normals to the subspace spanned by the vertices: " << NormalsToSubspace.ElementToString();
      gramMatrixInverted=matNormals;
      gramMatrixInverted.Transpose();
      gramMatrixInverted.MultiplyOnTheLeft(matNormals);
      gramMatrixInverted.Invert(theGlobalVariables);
      root tempRoot;
      for (int i=0; i<this->Normals.size; i++)
      { matNormals.ActOnAroot(this->Normals[i], tempRoot);
        gramMatrixInverted.ActOnAroot(tempRoot);
        for (int j=0; j<tempRoot.size; j++)
          this->Normals[i]-=NormalsToSubspace[j]*tempRoot[j];
        this->Normals[i].ScaleByPositiveRationalToIntegralMinHeight();
        if (this->Normals[i].IsEqualToZero())
        { this->Normals.PopIndexSwapWithLast(i);
          i--;
        }
      }
      //all normals should now lie in the subspace spanned by the vertices
      //add the walls needed to go down to the subspace
      this->Normals.MakeActualSizeAtLeastExpandOnTop(this->Normals.size+2*NormalsToSubspace.size);
      for (int i=0; i<NormalsToSubspace.size; i++)
      { NormalsToSubspace[i].ScaleByPositiveRationalToIntegralMinHeight();
        this->Normals.AddObjectOnTop(NormalsToSubspace[i]);
        this->Normals.AddObjectOnTop(-NormalsToSubspace[i]);
      }
    }
  }
  int DesiredRank=this->Vertices.GetRankOfSpanOfElements(theGlobalVariables);
  if (DesiredRank>1)
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
            if (theRank==DesiredRank-1)
            { wallIsGood=true;
              break;
            }
        }
      if (!wallIsGood)
      { this->Normals.PopIndexSwapWithLast(i);
        i--;
      }
    }
  //eliminate identical normals
  this->Normals.QuickSortAscending();
  int currentUniqueElementIndex=0;
  for (int i=1; i<this->Normals.size; i++)
    if (this->Normals[currentUniqueElementIndex]!=this->Normals[i])
    { currentUniqueElementIndex++;
      this->Normals.SwapTwoIndices(currentUniqueElementIndex, i);
    }
  if (this->Normals.size>0)
    this->Normals.SetSize(currentUniqueElementIndex+1);
  for (int i=0; i<this->Vertices.size; i++)
    if (this->Normals.HasAnElementWithNegativeScalarProduct(this->Vertices[i]))
      assert(false);
  for (int i=0; i<this->Normals.size; i++)
    if (!this->Vertices.HasAnElementWithPositiveScalarProduct(this->Normals.TheObjects[i]))
      return false;
  return numAddedFakeWalls==0;
}

bool Cone::CreateFromVertices(roots& inputVertices, GlobalVariables& theGlobalVariables)
{ this->LowestIndexNotCheckedForChopping=0;
  this->LowestIndexNotCheckedForSlicingInDirection=0;
 // std::cout << inputVertices.ElementToString();
  this->flagIsTheZeroCone=false;
  if (inputVertices.size<=0)
  { this->Normals.size=0;
    this->Vertices.size=0;
    this->flagIsTheZeroCone=true;
    return false;
  }
  this->Normals.size=0;
  int rankVerticesSpan=inputVertices.GetRankOfSpanOfElements(theGlobalVariables);
  int theDim=inputVertices.GetDimensionOfElements();
  roots extraVertices;
  if (rankVerticesSpan<theDim)
  { MatrixLargeRational tempMat;
    tempMat.AssignRootsToRowsOfMatrix(inputVertices);
    tempMat.FindZeroEigenSpace(extraVertices);
    for (int i=0; i<extraVertices.size; i++)
    { this->Normals.AddObjectOnTop(extraVertices[i]);
      this->Normals.AddObjectOnTop(-extraVertices[i]);
    }
  }
  int NumCandidates = MathRoutines::NChooseK(inputVertices.size, rankVerticesSpan-1);
  Selection theSelection;
  theSelection.init(inputVertices.size);
  root normalCandidate;
  for (int i=0; i<NumCandidates; i++)
  { theSelection.incrementSelectionFixedCardinality(rankVerticesSpan-1);
    for (int j=0; j<theSelection.CardinalitySelection; j++)
      extraVertices.AddObjectOnTop(inputVertices[theSelection.elements[j]]);
    if (extraVertices.ComputeNormal(normalCandidate))
    { bool hasPositive; bool hasNegative;
      hasPositive=false; hasNegative=false;
      for (int j=0; j<inputVertices.size; j++)
      { Rational tempRat= normalCandidate.ScalarEuclidean(inputVertices[j]);
        if (tempRat.IsNegative())
          hasNegative=true;
        if (tempRat.IsPositive())
          hasPositive=true;
        if (hasNegative && hasPositive)
          break;
      }
      normalCandidate.ScaleByPositiveRationalToIntegralMinHeight();
      if ((hasNegative && !hasPositive))
        normalCandidate.MinusRoot();
      if (!(hasNegative && hasPositive))
        this->Normals.AddOnTopNoRepetition(normalCandidate);
    }
    extraVertices.size=theDim-rankVerticesSpan;
  }
//  std::cout << "<br>Candidate normals: " << this->Normals.ElementToString();
  return this->CreateFromNormals(this->Normals, theGlobalVariables);
}

bool Cone::CreateFromNormalS
  (roots& inputNormals, bool UseWithExtremeMathCautionAssumeConeHasSufficientlyManyProjectiveVertices, GlobalVariables& theGlobalVariables)
{ this->flagIsTheZeroCone=false;
  this->LowestIndexNotCheckedForChopping=0;
  this->LowestIndexNotCheckedForSlicingInDirection=0;
  int theDim=1;
  if (inputNormals.size>0)
    theDim=inputNormals[0].size;
  this->Normals=inputNormals;
  for (int i=0; i<this->Normals.size; i++)
    if (this->Normals[i].IsEqualToZero())
    { this->Normals.PopIndexSwapWithLast(i);
      i--;
    }
  int numAddedFakeWalls=0;
  if (!UseWithExtremeMathCautionAssumeConeHasSufficientlyManyProjectiveVertices)
    for (int i=0; i<theDim && this->Normals.GetRankOfSpanOfElements(theGlobalVariables)<theDim; i++)
    { root tempRoot;
      tempRoot.MakeEi(theDim, i);
      if(!this->Normals.LinSpanContainsRoot(tempRoot, theGlobalVariables))
      { numAddedFakeWalls++;
        this->Normals.AddObjectOnTop(tempRoot);
      }
    }
//  std::cout << "<br>Normals (" << inputNormals.size << " input " << numAddedFakeWalls << " fake): " << this->Normals.ElementToString();
  this->ComputeVerticesFromNormalsNoFakeVertices(theGlobalVariables);
//  std::cout << "<br>Vertices before adding minus vertices: " << this->Vertices.ElementToString();
  if (numAddedFakeWalls>0)
  { this->Normals.SetSize(this->Normals.size-numAddedFakeWalls);
    root tempRoot;
    int originalSize=this->Vertices.size;
    for (int i=0; i<originalSize; i++)
    { tempRoot=-this->Vertices[i];
      if (this->IsInCone(tempRoot))
        this->Vertices.AddOnTopNoRepetition(tempRoot);
    }
  }
//  std::cout << "<br>Vertices: " << this->Vertices.ElementToString();
  return this->EliminateFakeNormalsUsingVertices(theDim, numAddedFakeWalls, theGlobalVariables);
}

void ConeComplex::initFromCones
(List<Cone>& NormalsOfCones, bool AssumeConesHaveSufficientlyManyProjectiveVertices, GlobalVariables& theGlobalVariables)
{ List<roots> tempRoots;
  tempRoots.SetSize(NormalsOfCones.size);
  for (int i=0; i<NormalsOfCones.size; i++)
    tempRoots[i]=NormalsOfCones[i].Normals;
  this->initFromCones(tempRoots, AssumeConesHaveSufficientlyManyProjectiveVertices, theGlobalVariables);
}

void ConeComplex::initFromCones
(List<roots>& NormalsOfCones, bool UseWithExtremeMathCautionAssumeConeHasSufficientlyManyProjectiveVertices, GlobalVariables& theGlobalVariables)
{ Cone tempCone;
  this->ClearTheObjects();
  theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
  theGlobalVariables.theIndicatorVariables.StatusString1=NormalsOfCones.ElementToStringGeneric();
  theGlobalVariables.MakeReport();
  theGlobalVariables.MakeReport();
//  for (int i=0; i<10000000; i++){int j=i*i*i;}
  for (int i=0; i<NormalsOfCones.size; i++)
  { if (tempCone.CreateFromNormalS(NormalsOfCones.TheObjects[i], UseWithExtremeMathCautionAssumeConeHasSufficientlyManyProjectiveVertices, theGlobalVariables))
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

std::string Cone::ElementToString(bool useLatex, bool useHtml, bool PrepareMathReport, bool lastVarIsConstant, PolynomialOutputFormat& theFormat)
{ std::stringstream out;
  if (this->flagIsTheZeroCone)
    out << "The cone is the zero cone.";
  else if(this->Normals.size==0)
    out << "The cone is the entire space";
  if (!PrepareMathReport)
  { out << "Index next wall to refine by: " << this->LowestIndexNotCheckedForChopping << "\n";
    if (useHtml)
      out << "<br>";
    out << "\nIndex next direction to slice in: " << this->LowestIndexNotCheckedForSlicingInDirection << "\n";
  }
  if (useHtml)
    out << "<br>";
  out << "Normals:\n";
  if (useHtml)
    out << "<br>";
  if (useLatex)
    out << "\\[";
  out << this->Normals.ElementsToInequalitiesString(useLatex, useHtml, lastVarIsConstant, theFormat);
  if (useLatex)
    out << "\\]";
  out << "\nProjectivized Vertices: " << this->Vertices.ElementToString();
  if (useHtml)
    out << "<br>";
  if (this->Vertices.size>0)
    out << "\nInternal point: " << this->GetInternalPoint().ElementToString();
  return out.str();
}

std::string ConeComplex::ElementToString(bool useLatex, bool useHtml)
{ std::stringstream out;
  PolynomialOutputFormat theFormat;
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
  if (this->slicingDirections.size>0)
  { if (useHtml)
      out << "<br>\n";
    out << " Directions to slice along: " << this->slicingDirections.ElementToString();
  }
  for (int i=0; i<this->size; i++)
  { if (useHtml)
      out << "<hr>";
    out << "\n\n\nChamber " << i+1 << ":\n";
    if (useHtml)
      out << "<br>";
    out << this->TheObjects[i].ElementToString(useLatex, useHtml, theFormat) << "\n\n\n";
  }
  return out.str();
}

int ParserNode::EvaluateGroebner
(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ List<PolynomialRationalCoeff> inputBasis, outputGroebner;
  theNode.impliedNumVars= theNode.GetMaxImpliedNumVarsChildren();
  for (int i=0; i<theArgumentList.size; i++)
  { PolynomialRationalCoeff& currentPoly=theNode.owner->TheObjects[theArgumentList[i]].polyValue.GetElement();
    currentPoly.SetNumVariablesSubDeletedVarsByOne(theNode.impliedNumVars);
    outputGroebner.AddObjectOnTop(currentPoly);
  }
  inputBasis=outputGroebner;
  PolynomialRationalCoeff buffer1, buffer2, buffer3, buffer4;
  Monomial<Rational> bufferMon1, bufferMon2;
  RationalFunction::TransformToReducedGroebnerBasis(outputGroebner, buffer1, buffer2, buffer3, buffer4, bufferMon1, bufferMon2, & theGlobalVariables);
  std::stringstream out;
  out << "<br>Starting basis: ";
  std::stringstream out1, out2;
  PolynomialOutputFormat theFormat;
  for(int i=0; i<inputBasis.size; i++)
    out1 << inputBasis.TheObjects[i].ElementToString(theFormat) << ", ";
  out << CGIspecificRoutines::GetHtmlMathDivFromLatexFormulaAddBeginArrayRCL(out1.str());
  out << "<br>Reduced Groebner basis:";
  for(int i=0; i<outputGroebner.size; i++)
    out2 << outputGroebner.TheObjects[i].ElementToString(theFormat) << ", ";
  out << CGIspecificRoutines::GetHtmlMathDivFromLatexFormulaAddBeginArrayRCL(out2.str());
  theNode.ExpressionType=theNode.typeString;
  theNode.outputString= out.str();
  return theNode.errorNoError;
}

void RationalFunction::GetRelations
  ( List<PolynomialRationalCoeff>& theGenerators, GlobalVariables& theGlobalVariables
   )
{ if (theGenerators.size==0)
    return;
  List<PolynomialRationalCoeff> theGroebnerBasis;
  theGroebnerBasis=theGenerators;
  int numStartingGenerators=theGenerators.size;
  int numStartingVariables= theGenerators.TheObjects[0].NumVars;
  PolynomialRationalCoeff tempP;
  PolynomialOutputFormat tempFormat;
  for (int i=0; i<numStartingGenerators; i++)
  { PolynomialRationalCoeff& currentPoly=theGroebnerBasis.TheObjects[i];
    currentPoly.SetNumVariablesSubDeletedVarsByOne(numStartingVariables+numStartingGenerators);
    tempP.MakeNVarDegOnePoly(numStartingVariables+numStartingGenerators, i+numStartingVariables, (Rational) -1);
    currentPoly+=tempP;
//  std::cout << currentPoly.ElementToString(false, tempFormat) << "<br>";
  }
  PolynomialRationalCoeff buffer1, buffer2, buffer3, buffer4;
  Monomial<Rational> bufferMon1, bufferMon2;
  RationalFunction::TransformToReducedGroebnerBasis
  (
   theGroebnerBasis, buffer1, buffer2, buffer3, buffer4, bufferMon1, bufferMon2,
   & Monomial<Rational>::LeftIsGEQLexicographicLastVariableWeakest, & theGlobalVariables
  );
//  std::cout << "<br>the ending generators:<br> ";
//  for (int i=0; i<theGroebnerBasis.size; i++)
//  { std::cout << theGroebnerBasis.TheObjects[i].ElementToString(false, tempFormat) << "<br>";
//  }
  theGenerators.MakeActualSizeAtLeastExpandOnTop(theGroebnerBasis.size);
  theGenerators.size=0;
  for (int i=0; i<theGroebnerBasis.size; i++)
  { PolynomialRationalCoeff& currentPoly= theGroebnerBasis.TheObjects[i];
    bool bad=false;
    for (int j=0; j<numStartingVariables; j++)
      if(currentPoly.GetMaxPowerOfVariableIndex(j)>0)
      { bad=true;
        break;
      }
    if (!bad)
     theGenerators.AddObjectOnTop(currentPoly);
  }
}

int ParserNode::EvaluateRelations
(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ List<PolynomialRationalCoeff> inputBasis, outputRelations;
  theNode.impliedNumVars= theNode.GetMaxImpliedNumVarsChildren();
  for (int i=0; i<theArgumentList.size; i++)
  { PolynomialRationalCoeff& currentPoly=theNode.owner->TheObjects[theArgumentList[i]].polyValue.GetElement();
    currentPoly.SetNumVariablesSubDeletedVarsByOne(theNode.impliedNumVars);
    outputRelations.AddObjectOnTop(currentPoly);
  }
  inputBasis=outputRelations;
  RationalFunction::GetRelations(outputRelations, theGlobalVariables);
  std::stringstream out;
  out << "<br>Starting elements:";
  PolynomialOutputFormat theFormat;
  std::stringstream out3;
  for(int i=0; i<inputBasis.size; i++)
    out3 << " u_{" << i+1+theNode.impliedNumVars << "}:=" << inputBasis[i].ElementToString(theFormat) << ", ";
  out << CGIspecificRoutines::GetHtmlMathSpanFromLatexFormula(out3.str());
  out << "<br>Resulting relations:";
  std::stringstream out2;
  theFormat.MakeAlphabetArbitraryWithIndex("u", "v");
  for(int i=0; i<outputRelations.size; i++)
    out2 << outputRelations.TheObjects[i].ElementToString(theFormat) << ", ";
  out << CGIspecificRoutines::GetHtmlMathSpanFromLatexFormula(out2.str());
  theNode.ExpressionType=theNode.typeString;
  theNode.outputString= out.str();
  return theNode.errorNoError;
}

int ParserNode::EvaluateCone
(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ roots theNormals;
  int theDim;
  if (!theNode.GetRootsEqualDimNoConversionNoEmptyArgument(theArgumentList, theNormals, theDim))
    return theNode.SetError(theNode.errorDimensionProblem);
  Cone& currentCone=theNode.theCone.GetElement();
  currentCone.CreateFromNormals(theNormals, theGlobalVariables);
  PolynomialOutputFormat theFormat;
  theNode.outputString=currentCone.DrawMeToHtmlProjective(theGlobalVariables.theDrawingVariables, theFormat);
  theNode.ExpressionType=theNode.typeCone;
  return theNode.errorNoError;
}

int ParserNode::EvaluateConeFromVertices
(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ roots theVertices;
  int theDim;
  if (!theNode.GetRootsEqualDimNoConversionNoEmptyArgument(theArgumentList, theVertices, theDim))
    return theNode.SetError(theNode.errorDimensionProblem);
  Cone& currentCone=theNode.theCone.GetElement();
  currentCone.CreateFromVertices(theVertices, theGlobalVariables);
  PolynomialOutputFormat theFormat;
  theNode.outputString=currentCone.DrawMeToHtmlProjective(theGlobalVariables.theDrawingVariables, theFormat);
  theNode.ExpressionType=theNode.typeCone;
  return theNode.errorNoError;
}

bool ConeComplex::findMaxLFOverConeProjective
  (Cone& input, List<PolynomialRationalCoeff>& inputLinPolys, List<int>& outputMaximumOverEeachSubChamber, GlobalVariables& theGlobalVariables)
{ roots HyperPlanesCorrespondingToLF;
  if (input.Normals.size<1 || inputLinPolys.size<1)
    return false;
  int theDim=input.Normals.TheObjects[0].size;
  HyperPlanesCorrespondingToLF.SetSize(inputLinPolys.size);
  for (int i=0; i<inputLinPolys.size; i++)
  { PolynomialRationalCoeff& currentPoly=inputLinPolys.TheObjects[i];
    if (currentPoly.TotalDegree()!=1 || theDim!=currentPoly.NumVars)
    { std::cout << "u messed up the dim! The num vars is " << currentPoly.NumVars << " and the dim of the cone is " << theDim;
      return false;
    }
    root& newWall=HyperPlanesCorrespondingToLF.TheObjects[i];
    newWall.MakeZero(theDim);
    for (int j=0; j<currentPoly.size; j++)
      for (int k=0; k<theDim; k++)
        if (currentPoly.TheObjects[j].degrees[k]==1)
        { newWall.TheObjects[k]=currentPoly.TheObjects[j].Coefficient;
          break;
        }
  }
  return this->findMaxLFOverConeProjective
  (input, HyperPlanesCorrespondingToLF,
   outputMaximumOverEeachSubChamber, theGlobalVariables);
}

bool ConeComplex::findMaxLFOverConeProjective
  (Cone& input, roots& inputLFsLastCoordConst,
   List<int>& outputMaximumOverEeachSubChamber,
   GlobalVariables& theGlobalVariables)
{ this->init();
  this->AddNonRefinedChamberOnTopNoRepetition(input);
  root tempRoot;
  for (int i=0; i<inputLFsLastCoordConst.size; i++)
    for (int j=i+1; j<inputLFsLastCoordConst.size; j++)
    { tempRoot=inputLFsLastCoordConst[i]-inputLFsLastCoordConst[j];
      tempRoot.ScaleToIntegralMinHeightFirstNonZeroCoordinatePositive();
      if (!tempRoot.IsEqualToZero())
        this->splittingNormals.AddObjectOnTopNoRepetitionOfObjectHash(tempRoot);
    }
  std::cout << this->ElementToString(false, true);
  this->Refine(theGlobalVariables);
  outputMaximumOverEeachSubChamber.SetSize(this->size);
  Rational theMax=0;
  for (int i=0; i<this->size; i++)
  { this->TheObjects[i].GetInternalPoint(tempRoot);
    bool isInitialized=false;
    tempRoot.ComputeDebugString();
    for (int j=0; j<inputLFsLastCoordConst.size; j++)
      if (!isInitialized || tempRoot.ScalarEuclidean(inputLFsLastCoordConst.TheObjects[j], (Rational) 0)>theMax)
      { inputLFsLastCoordConst.TheObjects[j].ComputeDebugString();
        theMax=tempRoot.ScalarEuclidean(inputLFsLastCoordConst.TheObjects[j], (Rational) 0);
        outputMaximumOverEeachSubChamber.TheObjects[i]=j;
        isInitialized=true;
      }
  }
  for (int i=0; i<this->size; i++)
  { //std::cout << "<br>Chamber " << i+1 << " maximum linear function is the function of index " << outputMaximumOverEeachSubChamber.TheObjects[i] << ": " << inputLinPolys.TheObjects[outputMaximumOverEeachSubChamber.TheObjects[i]].ElementToString();
    //std::cout << "<br>The chamber is given by: " << this->TheObjects[i].ElementToString(false, true);
  }
  return true;
}

//std::string tempDebugString;
void Lattice::Reduce
()
{ //////////////////////////////////the below function is for debugging purposes only!
  //Don't wanna mess around with #ifdefs, but it should be surrounded by #ifdef DEBUG .... #endif
  //bool flagTesting=true;
  //static MatrixLargeRational testMatrix;
  //if (flagTesting)
  //  this->TestGaussianEliminationEuclideanDomainRationals(testMatrix);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  LargeInt tempInt, tempInt2;
  tempInt.MakeMOne(); tempInt2.MakeOne();
  this->basis.GaussianEliminationEuclideanDomain(tempInt, tempInt2);
  int numRowsToTrim=0;
  for (int i=this->basis.NumRows-1; i>=0; i--)
  { bool foundNonZeroRow=false;
    for (int j=0; j<this->basis.NumCols; j++)
      if (!this->basis.elements[i][j].IsEqualToZero())
      { foundNonZeroRow=true;
        break;
      }
    if (foundNonZeroRow)
      break;
    numRowsToTrim++;
  }
  this->basis.Resize(this->basis.NumRows-numRowsToTrim, this->basis.NumCols, true);
  this->basisRationalForm.AssignMatrixIntWithDen(this->basis, this->Den);
//  if (flagTesting)
 // { testMatrix.Resize(testMatrix.NumRows-numRowsToTrim, testMatrix.NumCols, true);
//    std::stringstream out;
//    std::cout << "<br>basis rational form: " << this->basisRationalForm.ElementToString(true, false);
//    out << "<br>basis rational form: " << this->basisRationalForm.ElementToString(true, false);
//    std::string tempS=out.str();
//    assert(testMatrix==this->basisRationalForm);
//  }
}

void Lattice::TestGaussianEliminationEuclideanDomainRationals(MatrixLargeRational& output)
{ output.AssignMatrixIntWithDen(this->basis, this->Den);
  std::stringstream out;
  std::cout << "Test output: " << output.ElementToString(true, false);
  out << "Test output: " << output.ElementToString(true, false);
  output.GaussianEliminationEuclideanDomain((Rational) -1, (Rational) 1);
  std::cout << "<br>After gaussian elimination:" << output.ElementToString(true, false);
  out << "<br>After gaussian elimination:" << output.ElementToString(true, false);
//  tempDebugString=out.str();
}

void Lattice::RefineByOtherLattice(const Lattice& other)
{ if (other.basis.NumCols==0)
    return;
  if (other.basis==this->basis && this->Den==other.Den)
    return;

  assert(other.GetDim()==this->GetDim());
  int theDim=this->GetDim();
  LargeIntUnsigned oldDen=this->Den;
  LargeIntUnsigned::lcm(other.Den, oldDen, this->Den);
  LargeIntUnsigned scaleThis, scaleOther;
  scaleThis=this->Den/oldDen;
  scaleOther=this->Den/other.Den;
  int oldNumRows=this->basis.NumRows;
  LargeInt tempI; tempI=scaleThis;
  this->basis*=tempI;
  this->basis.Resize(this->basis.NumRows+other.basis.NumRows, theDim, true);
  for (int i=oldNumRows; i<this->basis.NumRows; i++)
    for (int j=0; j<this->basis.NumCols; j++)
      this->basis.elements[i][j]=other.basis.elements[i-oldNumRows][j]*scaleOther;
  this->Reduce();
}

bool ParserNode::GetRootsEqualDimNoConversionNoEmptyArgument
(List<int>& theArgumentList, roots& output, int& outputDim)
{ ParserNode& firstNode= this->owner->TheObjects[theArgumentList.TheObjects[0]];
  output.SetSize(theArgumentList.size);
  if (firstNode.ExpressionType!=this->typeArray)
  { outputDim=1;
    for (int i=0; i<theArgumentList.size; i++)
    { ParserNode& currentNode=this->owner->TheObjects[theArgumentList.TheObjects[i]];
      if (currentNode.ExpressionType!=this->typeRational)
        return false;
      output.TheObjects[i].SetSize(1);
      output.TheObjects[i].TheObjects[0]=currentNode.rationalValue;
    }
    return true;
  }
  outputDim=firstNode.children.size;
  for (int i=0; i<theArgumentList.size; i++)
  { ParserNode& currentNode=this->owner->TheObjects[theArgumentList.TheObjects[i]];
    if (outputDim!=currentNode.children.size)
      return false;
    output.TheObjects[i].SetSize(outputDim);
    for (int j=0; j<currentNode.children.size; j++)
      output.TheObjects[i].TheObjects[j]=this->owner->TheObjects[currentNode.children.TheObjects[j]].rationalValue;
  }
  return true;
}

int ParserNode::EvaluateLattice
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ roots LatticeBase;
  int theDim=-1;
  if(!theNode.GetRootsEqualDimNoConversionNoEmptyArgument(theArgumentList, LatticeBase, theDim))
    return theNode.SetError(theNode.errorDimensionProblem);
  theNode.theLattice.GetElement().MakeFromRoots(LatticeBase);
  theNode.outputString=theNode.theLattice.GetElement().ElementToString();
  theNode.ExpressionType=theNode.typeLattice;
  return theNode.errorNoError;
}

void Lattice::MakeFromMat(const MatrixLargeRational& input)
{ this->basisRationalForm=input;
  this->basisRationalForm.GetMatrixIntWithDen(this->basis, this->Den);
  this->Reduce();
}

void Lattice::MakeFromRoots(const roots& input)
{ MatrixLargeRational tempMat;
  tempMat.AssignRootsToRowsOfMatrix(input);
  tempMat.GetMatrixIntWithDen(this->basis, this->Den);
  this->Reduce();
}

std::string Lattice::ElementToString(bool useHtml, bool useLatex)const
{ std::stringstream out;
  out << "L=<";
  roots tempRoots;
  tempRoots.AssignMatrixRows(this->basisRationalForm);
  for (int i=0; i<this->basisRationalForm.NumRows; i++)
  { out << tempRoots.TheObjects[i].ElementToString();
    if (i!=this->basisRationalForm.NumRows-1)
      out << ",";
  }
  out << ">";
  return out.str();
}

#ifdef WIN32
#pragma warning(disable:4244)//warning 4244: data loss from conversion
#endif
void LargeIntUnsigned::DivPositive(const LargeIntUnsigned& x, LargeIntUnsigned& quotientOutput, LargeIntUnsigned& remainderOutput) const
{ LargeIntUnsigned remainder, quotient, divisor;
  remainder.Assign(*this);
  divisor.Assign(x);
  assert(!divisor.IsEqualToZero());
  quotient.MakeZero();
  //std::string tempS1, tempS2, tempS3;
  while (remainder.IsGEQ(divisor))
  { unsigned int q;
    LargeIntUnsigned current, Total;
    if (*remainder.LastObject()>*divisor.LastObject())
    { q=*remainder.LastObject()/(*divisor.LastObject()+1);
      current.AssignShiftedUInt(q, remainder.size-divisor.size);
    }
    else
    { if (remainder.size==divisor.size)
        current.AssignShiftedUInt(1, 0);
      else
      { q=this->CarryOverBound/(divisor.TheObjects[divisor.size-1]+1);
        current.AssignShiftedUInt(q, remainder.size- divisor.size-1);
        current.MultiplyByUInt(remainder.TheObjects[remainder.size-1]);
      }
    }
    Total.Assign(divisor);
    Total.MultiplyBy(current);
    //if (!remainder.IsGEQ(Total))
    //{ tempS1= remainder.ElementToString();
    //  tempS2=Total.ElementToString();
    //  remainder.IsGEQ(Total);
    //}
    remainder.SubtractSmallerPositive(Total);
    quotient.Add(current);
  }
  remainderOutput.Assign(remainder);
  quotientOutput.Assign(quotient);
//  assert(remainderOut.CheckForConsistensy());
}
#ifdef WIN32
#pragma warning(default:4244)//warning 4244: data loss from conversion
#endif

//returning false means that the lattice given as rougher is not actually rougher than the current lattice
//or that there are too many representatives
bool Lattice::GetAllRepresentatives
  (const Lattice& rougherLattice, roots& output)const
{ output.size=0;
  if (this->basis.NumRows!=rougherLattice.basis.NumRows)
    return false;
  List<int> thePeriods;
  roots thePeriodVectors;
  thePeriods.SetSize(this->basis.NumRows);
  thePeriodVectors.SetSize(this->basis.NumRows);
  root tempRoot, tempRoot2;
  int col=0;
  int theDim=this->GetDim();
  Rational currentPeriod;
  LargeInt currentPeriodInt;
  for (int i=0; i<this->basis.NumRows; i++)
  { while (this->basisRationalForm.elements[i][col].IsEqualToZero())
      col++;
    currentPeriod=rougherLattice.basisRationalForm.elements[i][col]/this->basisRationalForm.elements[i][col];
    currentPeriod.GetNum(currentPeriodInt);
    if (currentPeriodInt.value.size>1)
//    { std::cout << "exited at point 1 counter i is " << i;
      return false;
//    }
    else
      thePeriods.TheObjects[i]=currentPeriodInt.value.TheObjects[0];
    this->basisRationalForm.RowToRoot(i, thePeriodVectors.TheObjects[i]);
    rougherLattice.basisRationalForm.RowToRoot(i, tempRoot2);
    tempRoot=thePeriodVectors.TheObjects[i];
    tempRoot*=thePeriods.TheObjects[i];

//    if (!(tempRoot-tempRoot2).IsEqualToZero())
//    { std::cout << "exited at point 2 counter i is " << i << " the period vector=" << thePeriodVectors.TheObjects[i].ElementToString() << " rougher lattice basis vector: " << tempRoot2.ElementToString();
//      return false;
//    }
  }
//  std::cout << thePeriodVectors.ElementToString() << "<br>The periods: ";
//  for (int i=0; i<thePeriods.size; i++)
//    std::cout << thePeriods.TheObjects[i] << ", ";
  for (int i=0; i<thePeriods.size; i++)
    thePeriods.TheObjects[i]--;
  SelectionWithDifferentMaxMultiplicities theCoeffSelection;
  theCoeffSelection.initFromInts(thePeriods);
  int NumCycles=theCoeffSelection.getTotalNumSubsets();
  output.SetSize(NumCycles);
  for (int i=0; i<NumCycles; i++, theCoeffSelection.IncrementSubset())
  { output.TheObjects[i].MakeZero(theDim);
    for (int j=0; j<theCoeffSelection.Multiplicities.size; j++)
      output.TheObjects[i]+=thePeriodVectors.TheObjects[j]*theCoeffSelection.Multiplicities.TheObjects[j];
  }
//  std::cout << "The representatives: " << output.ElementToString(false, true, false);
  return true;
}

bool Lattice::GetClosestPointInDirectionOfTheNormalToAffineWallMovingIntegralStepsInDirection
  (root& startingPoint, root& theAffineHyperplane, root& theDirection, root& outputPoint)
{ root theNormal=theAffineHyperplane;
  theNormal.SetSize(theAffineHyperplane.size-1);
  Rational theShift=-(*theAffineHyperplane.LastObject());
  if (theNormal.ScalarEuclidean(startingPoint)==theShift)
  { outputPoint=startingPoint;
    return true;
  }
  if (theDirection.ScalarEuclidean(theNormal).IsEqualToZero())
    return false;
  Rational theMovement= (theShift-startingPoint.ScalarEuclidean(theNormal))/theDirection.ScalarEuclidean(theNormal);
  std::cout << "<br>the movement: " << theMovement.ElementToString() << ", (" << theShift.ElementToString() << " - " << startingPoint.ScalarEuclidean(theNormal).ElementToString() << ")/ " << theDirection.ScalarEuclidean(theNormal).ElementToString() << ", ";
  if (!theMovement.IsInteger())
  { std::cout << "the movement aint integral; ";
    theMovement.AssignFloor();
    if (theDirection.ScalarEuclidean(theNormal).IsPositive())
      theMovement+=1;
  }
  std::cout << "the normal: " << theNormal.ElementToString() <<", the direction: " << theDirection.ElementToString() << ", the shift: " << theShift.ElementToString()
  << ", the movement: " << theMovement.ElementToString() << ", startingPoint: " << startingPoint.ElementToString();
  outputPoint=startingPoint;
  outputPoint+=(theDirection*theMovement);
  std::cout << ", finalPoint: " << outputPoint.ElementToString();
  return true;
}

bool Lattice::GetAllRepresentativesProjectingDownTo
  (const Lattice& rougherLattice, roots& startingShifts, roots& output)const
{ roots tempRepresentatives;
  if (!this->GetAllRepresentatives(rougherLattice, tempRepresentatives))
    return false;
  output.MakeActualSizeAtLeastExpandOnTop(startingShifts.size*tempRepresentatives.size);
  output.size=0;
  for (int i=0; i<startingShifts.size; i++)
    for (int j=0; j<tempRepresentatives.size; j++)
      output.AddObjectOnTop(startingShifts.TheObjects[i]+tempRepresentatives[j]);
  return true;
}

void QuasiPolynomial::MakeRougherLattice(const Lattice& latticeToRoughenBy)
{ if (this->AmbientLatticeReduced==latticeToRoughenBy)
    return;
  //std::cout << "roughening " << this->AmbientLatticeReduced.ElementToString() << "by" << latticeToRoughenBy.ElementToString();
  Lattice OldLattice;
  OldLattice=this->AmbientLatticeReduced;
  this->AmbientLatticeReduced.IntersectWith(latticeToRoughenBy);
  roots representativesQuotientLattice;
  //std::cout << "getting all representatives of " << OldLattice.ElementToString() << "inside" << this->AmbientLatticeReduced.ElementToString();
  //std::cout << "<br><br><br><br>*********<br><br><br><br>";
  OldLattice.GetAllRepresentatives(this->AmbientLatticeReduced, representativesQuotientLattice);
  roots OldLatticeShifts=this->LatticeShifts;
  List<PolynomialRationalCoeff> oldValues;
  oldValues=this->valueOnEachLatticeShift;
  this->LatticeShifts.SetSize(OldLatticeShifts.size*representativesQuotientLattice.size);
  this->valueOnEachLatticeShift.SetSize(this->LatticeShifts.size);
  for (int i=0; i<OldLatticeShifts.size; i++)
    for (int j=0; j<representativesQuotientLattice.size; j++)
    { this->LatticeShifts.TheObjects[i*representativesQuotientLattice.size+j]=OldLatticeShifts.TheObjects[i]+representativesQuotientLattice.TheObjects[j];
      this->valueOnEachLatticeShift.TheObjects[i*representativesQuotientLattice.size+j]=oldValues.TheObjects[i];
    }
}

void Lattice::GetDualLattice(Lattice& output)const
{ if (this->GetRank()!=this->GetDim())
    return;
  MatrixLargeRational tempMat;
  tempMat=this->basisRationalForm;
  tempMat.Invert();
  tempMat.Transpose();
  tempMat.GetMatrixIntWithDen(output.basis, output.Den);
  output.Reduce();
}

bool Lattice::FindOnePreimageInLatticeOf
    (const MatrixLargeRational& theLinearMap, const roots& input, roots& output, GlobalVariables& theGlobalVariables)
{ Vectors<Rational> thisBasis, tempInput, tempOutput;
  thisBasis.AssignMatrixRows(this->basisRationalForm);
  theLinearMap.ActOnVectorsColumn(thisBasis, (Rational) 0);
  input.GetVectorsRational(tempInput);
  bool result=tempInput.GetIntegralCoordsInBasisIfTheyExist(thisBasis, tempOutput, (Rational) 1, (Rational) -1, (Rational) 0);
  output.AssignVectorsRational(tempOutput);
  //std::cout << "<hr>acting by " << this->basisRationalForm.ElementToString(true, false) << " on " << output.ElementToString();
  MatrixLargeRational tempMat;
  tempMat=this->basisRationalForm;
  tempMat.Transpose();
  tempMat.ActOnRoots(output);
  //std::cout << "<br> to get " << output.ElementToString();
  return result;
}

void Lattice::IntersectWithPreimageOfLattice
  (const MatrixLargeRational& theLinearMap, const Lattice& other, GlobalVariables& theGlobalVariables)
{ roots startingBasis, imageStartingBasis, basisImageIntersection, basisImageIntersectionInCoordsWRTimageStartingBasis, ImageBasisInImageStartingBasisCoords;
  roots resultNonKernelPart, resultKernelPart, result, tempRoots;
  startingBasis.AssignMatrixRows(this->basisRationalForm);
  //std::cout << "<br>Starting basis: " << startingBasis.ElementToString();
  //std::cout << "<br>The linear map: " << theLinearMap.ElementToString(true, false);
  theLinearMap.ActOnRoots(startingBasis, imageStartingBasis);
  //std::cout << "<br>Image of starting basis: " << imageStartingBasis.ElementToString();
  Lattice ImageLattice;
  ImageLattice.MakeFromRoots(imageStartingBasis);
  ImageLattice.IntersectWith(other);
  //std::cout << "<br>Image lattice: " << ImageLattice.ElementToString(true, false);
  basisImageIntersection.AssignMatrixRows(ImageLattice.basisRationalForm);
  Vectors<Rational> tempBasisImageIntersection, tempImageStartingBasis, tempImageBasisInImageStartingBasisCoords;
  basisImageIntersection.GetVectorsRational(tempBasisImageIntersection);
  imageStartingBasis.GetVectorsRational(tempImageStartingBasis);
  bool tempBool= tempBasisImageIntersection.GetIntegralCoordsInBasisIfTheyExist(tempImageStartingBasis, tempImageBasisInImageStartingBasisCoords, (Rational) 1, (Rational) -1, (Rational) 0);
  ImageBasisInImageStartingBasisCoords.AssignVectorsRational(tempImageBasisInImageStartingBasisCoords);
  assert(tempBool);
  //std::cout << "<br> Basis of image lattice expressed in coordinates with respect to image of starting basis: " << ImageBasisInImageStartingBasisCoords.ElementToString();
  resultNonKernelPart.SetSize(ImageBasisInImageStartingBasisCoords.size);
  for (int i=0; i<resultNonKernelPart.size; i++)
  { root& currentRoot=resultNonKernelPart.TheObjects[i];
    currentRoot.MakeZero(this->GetDim());
    for (int j=0; j<startingBasis.size; j++)
      currentRoot+=startingBasis.TheObjects[j]*ImageBasisInImageStartingBasisCoords.TheObjects[i].TheObjects[j];
  }
  //std::cout << "<br> Result non-kernel part (representatives): " << resultNonKernelPart.ElementToString();
  Lattice KernelPart;
  KernelPart=*this;
  tempRoots.AssignMatrixRows(theLinearMap);
  KernelPart.IntersectWithLinearSubspaceGivenByNormals(tempRoots);
  //std::cout << "<br>Kernel part of the result: " << KernelPart.ElementToString(true, false);
  result.AssignMatrixRows(KernelPart.basisRationalForm);
  result.AddListOnTop(resultNonKernelPart);
  this->MakeFromRoots(result);
  //std::cout << "<br> And the result is.... " << this->ElementToString(true, false);
}

void Lattice::IntersectWith(const Lattice& other)
{ roots commonBasis, otherBasis, startBasis;
  startBasis.AssignMatrixRows(this->basisRationalForm);
  otherBasis.AssignMatrixRows(other.basisRationalForm);
  GlobalVariables theGlobalVariables;
//  std::cout << "<br>this basis: " << startBasis.ElementToString();
//  std::cout << "<br>other basis: " << otherBasis.ElementToString();
  startBasis.IntersectTwoLinSpaces(startBasis, otherBasis, commonBasis, theGlobalVariables);
//  std::cout << "<br> basis of linear space intersection: " << commonBasis.ElementToString() << "<br><br> ";
  Lattice thisLatticeIntersected, otherLatticeIntersected;
  thisLatticeIntersected=*this;
  otherLatticeIntersected=other;
  thisLatticeIntersected.IntersectWithLinearSubspaceSpannedBy(commonBasis);
//  std::cout << "<br> linear space intersected with this lattice: " << thisLatticeIntersected.ElementToString(true, false);
  otherLatticeIntersected.IntersectWithLinearSubspaceSpannedBy(commonBasis);
//  std::cout << "<br> linear space intersected with other lattice: " << otherLatticeIntersected.ElementToString(true, false);
  roots thisCommonBasis, otherCommonBasis, thisCommonCoords, otherCommonCoords;
  thisCommonBasis.AssignMatrixRows(thisLatticeIntersected.basisRationalForm);
  otherCommonBasis.AssignMatrixRows(otherLatticeIntersected.basisRationalForm);
  thisCommonBasis.GetCoordsInBasis(commonBasis, thisCommonCoords, theGlobalVariables);
  otherCommonBasis.GetCoordsInBasis(commonBasis, otherCommonCoords, theGlobalVariables);
//  std::cout << "<br>this lattice intersection new coords: " << thisCommonBasis.ElementToString();
//  std::cout << "<br>other lattice intersection new coords: " << otherBasis.ElementToString();
  Lattice thisCommonCoordsLattice, otherCommonCoordsLattice;
  thisCommonCoordsLattice.MakeFromRoots(thisCommonCoords);
  otherCommonCoordsLattice.MakeFromRoots(otherCommonCoords);
//  std::cout << "<br> linear space intersected with this lattice new coords: " << thisCommonCoordsLattice.ElementToString(true, false);
//  std::cout << "<br> linear space intersected with other lattice new coords: " << otherCommonCoordsLattice.ElementToString(true, false);
  thisCommonCoordsLattice.IntersectWithBothOfMaxRank(otherCommonCoordsLattice);
//  std::cout << "<br> intersection lattice new coords: " << thisCommonCoordsLattice.ElementToString(true, false);
  roots resultBasis;
  resultBasis.SetSize(thisCommonCoordsLattice.basisRationalForm.NumRows);
  for (int i=0; i<resultBasis.size; i++)
  { root& currentRoot=resultBasis.TheObjects[i];
    currentRoot.MakeZero(this->GetDim());
    for (int j=0; j<thisCommonCoordsLattice.basisRationalForm.NumCols; j++)
      currentRoot+=thisCommonCoordsLattice.basisRationalForm.elements[i][j]*commonBasis.TheObjects[j];
  }
  this->MakeFromRoots(resultBasis);
//  std::cout << "<br>final answer: intersection is: " << this->ElementToString(true, false);
}

void Lattice::IntersectWithBothOfMaxRank(const Lattice& other)
{ Lattice dualLatticeThis, dualLatticeOther;
  //std::cout << "intersecting " << this->ElementToString() << " and " << other.ElementToString();
  assert(this->basis.NumRows==this->GetDim() && this->GetDim()==other.GetDim() && other.basis.NumRows== this->GetDim());
  this->GetDualLattice(dualLatticeThis);
  //std::cout << "<br>dual lattice of left: " << dualLatticeThis.ElementToString();
  other.GetDualLattice(dualLatticeOther);
  //std::cout << "<br>dual lattice of right: " << dualLatticeOther.ElementToString();
  dualLatticeThis.RefineByOtherLattice(dualLatticeOther);
  //std::cout << "<br> common refinement of dual lattice: " << dualLatticeThis.ElementToString();
  dualLatticeThis.GetDualLattice(*this);
  //std::cout << "<br>final result: " << this->ElementToString();
}

void QuasiPolynomial::operator+=(const QuasiPolynomial& other)
{ //std::cout << "current ambient lattice: " << this->AmbientLatticeReduced.ElementToString();
  this->MakeRougherLattice(other.AmbientLatticeReduced);
  //std::cout << "roughened: " << this->AmbientLatticeReduced.ElementToString();
  QuasiPolynomial tempQP=other;
  //std::cout << "<br><br>other ambient lattice: " << this->AmbientLatticeReduced.ElementToString();
  tempQP.MakeRougherLattice(this->AmbientLatticeReduced);
  //std::cout << "roughened: " << this->AmbientLatticeReduced.ElementToString() << "<br><br><br><br>*******<br><br><br>";
  for(int i=0; i<tempQP.LatticeShifts.size; i++)
    this->AddLatticeShift(tempQP.valueOnEachLatticeShift.TheObjects[i], tempQP.LatticeShifts.TheObjects[i]);
}

std::string QuasiPolynomial::ElementToString(bool useHtml, bool useLatex, const PolynomialOutputFormat& thePolyFormat)
{ std::stringstream out;
  //if (useHtml)
//  out << "the lattice: " << this->AmbientLatticeReduced.ElementToString(useHtml, useLatex);
//  out << "<br>We have " << this->LatticeShifts.size << " lattice shifts. The polynomial on each lattice shift follows.";
  if (this->LatticeShifts.size==0)
    return "0";
  for (int i=0; i<this->LatticeShifts.size; i++)
  { //if(useHtml)
      //out << "<br>Shift: " << this->LatticeShifts.TheObjects[i].ElementToString() << "; polynomial: ";
    if (useLatex)
    { if(!useHtml)
        out << "$\\begin{array}{rcl}&&";
      else
        out << "<span class=\"math\">";
    }
    out << this->valueOnEachLatticeShift.TheObjects[i].ElementToString(thePolyFormat);
    if (useLatex)
    { if(!useHtml)
        out << "\\end{array}$";
      else
        out << "</span>";
    }
    out << " over ";
    if (!this->LatticeShifts.TheObjects[i].IsEqualToZero())
      out << this->LatticeShifts.TheObjects[i].ElementToString() << " + ";
    if (useLatex)
    { if (!useHtml)
        out << "$\\Lambda$, \\\\\n ";
      else
        out << "<span class=\"math\"> \\Lambda</span>";
    } else
      out << "L ";
    if (this->LatticeShifts.size>1)
    { if (useHtml)
        out << "<br>";
      if (useLatex)
        out << "\\\\";
      out << "\n\n";
    }
  }
  if (!this->AmbientLatticeReduced.basisRationalForm.IsIdMatrix())
  { if(!useLatex)
      out << ", where L=< ";
    if (useLatex && useHtml)
      out << ", where <span class=\"math\">\\Lambda</span>=<";
    if (useLatex && !useHtml)
      out << " where $\\Lambda=\\left\\langle\\begin{array}{c}";
    roots tempRoots;
    tempRoots.AssignMatrixRows(this->AmbientLatticeReduced.basisRationalForm);
    for (int i=0; i<tempRoots.size; i++)
    { out << tempRoots.TheObjects[i].ElementToString();
      if (i!=tempRoots.size-1)
      { out << ", ";
        if (useLatex)
          out << "\\\\";
      }
    }
    if(!useLatex)
      out << " >";
    if (useLatex && useHtml)
      out << " >";
    if (useLatex && !useHtml)
      out << "\\end{array}\\right\\rangle$";
  } else
    if (useLatex)
      out << ", where $\\Lambda=\\mathbb{Z}^{" <<  this->GetNumVars() << "}$";
    else
      out << "Z^" <<  this->GetNumVars();
  return out.str();
}

bool ParserNode::GetRootInt(Vector<int>& output, GlobalVariables& theGlobalVariables)
{ assert(false);
  return true;
}

bool ParserNode::GetRootRationalFromFunctionArguments
(//List<int>& argumentList,
 root& output, GlobalVariables& theGlobalVariables)
{ roots tempRoots;
  int theDim;
  this->GetRootsEqualDimNoConversionNoEmptyArgument(this->children, tempRoots, theDim);
  if (tempRoots.size!=1)
    return false;
  output = tempRoots[0];
  return true;
}

bool ParserNode::GetRootRationalDontUseForFunctionArguments(root& output, GlobalVariables& theGlobalVariables)
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

void QuasiPolynomial::MakeFromPolyShiftAndLattice
(const PolynomialRationalCoeff& inputPoly, const root& theShift, const Lattice& theLattice, GlobalVariables& theGlobalVariables)
{ this->AmbientLatticeReduced=theLattice;
  this->LatticeShifts.SetSize(1);
  this->LatticeShifts.TheObjects[0]=theShift;
  this->AmbientLatticeReduced.ReduceVector(this->LatticeShifts.TheObjects[0]);
  this->valueOnEachLatticeShift.SetSize(1);
  this->valueOnEachLatticeShift.TheObjects[0]=inputPoly;
}

bool Lattice::ReduceVector(Vector<Rational>& theVector)
{ root output;
  assert(theVector.size==this->GetDim());
  Vectors<Rational> basisRoots;
  basisRoots.AssignMatrixRows(this->basisRationalForm);
  //std::cout <<  "the basis: " << basisRoots.ElementToString();
  if (!theVector.GetCoordsInBasiS(basisRoots, output, (Rational) 1, (Rational) 0))
  { std::cout << "oops bad!";
    return false;
  }
  for (int i=0; i<output.size; i++)
    output.TheObjects[i].AssignFracValue();
  theVector.MakeZero(theVector.size, (Rational) 0);
  for (int i=0; i<basisRoots.size; i++)
    theVector+=basisRoots.TheObjects[i]*output.TheObjects[i];
  //std::cout << "the vector " << theVector.ElementToString() << " in the basis " << basisRoots.ElementToString() << " has coordinates: " << output.ElementToString();
  return true;
}

void Lattice::MakeZn(int theDim)
{ this->basisRationalForm.MakeIdMatrix(theDim);
  this->basis.MakeIdMatrix(theDim, (LargeInt) 1, (LargeInt) 0);
  this->Den.MakeOne();
}

void QuasiPolynomial::MakeZeroOverLattice(Lattice& theLattice)
{ this->AmbientLatticeReduced=theLattice;
  this->LatticeShifts.size=0;
  this->valueOnEachLatticeShift.size=0;
}

void QuasiPolynomial::MakeZeroLatTiceZn(int theDim)
{ this->AmbientLatticeReduced.MakeZn(theDim);
  this->LatticeShifts.size=0;
  this->valueOnEachLatticeShift.size=0;
}

void partFraction::GetRootsFromDenominator
(partFractions& owner, roots& output)
{ output.SetSize(this->IndicesNonZeroMults.size);
  for (int i=0; i<this->IndicesNonZeroMults.size; i++)
    output.TheObjects[i].AssignIntRoot(owner.RootsToIndices.TheObjects[this->IndicesNonZeroMults.TheObjects[i]]);
}

void partFraction::ComputePolyCorrespondingToOneMonomial
  (partFractions& owner, QuasiPolynomial& outputQP, int monomialIndex, roots& normals, Lattice& theLattice, GlobalVariables& theGlobalVariables)
{ int theDimension=owner.AmbientDimension;
  root shiftRational; shiftRational.SetSize(theDimension);
  Monomial<LargeInt>& currentMon=this->Coefficient.TheObjects[monomialIndex];
  PolynomialRationalCoeff tempP, outputPolyPart;
  for (int j=0; j<theDimension; j++)
    shiftRational.TheObjects[j].AssignInteger(currentMon.degrees[j]);
  Rational tempRat, tempRat2;
  tempRat=currentMon.Coefficient;
  outputPolyPart.MakeNVarConst((int)theDimension, tempRat);
  for (int i=0; i<theDimension; i++)
  { this->MakePolynomialFromOneNormal(normals.TheObjects[i], shiftRational, this->TheObjects[this->IndicesNonZeroMults.TheObjects[i]].Multiplicities.TheObjects[0], tempP);
    outputPolyPart.MultiplyBy(tempP);
  }
  outputQP.MakeFromPolyShiftAndLattice(outputPolyPart, shiftRational, theLattice, theGlobalVariables);
  this->AlreadyAccountedForInGUIDisplay=true;
}

void partFraction::GetVectorPartitionFunction
  (partFractions& owner, QuasiPolynomial& output, GlobalVariables& theGlobalVariables)
{ QuasiPolynomial shiftedPoly;
  roots theNormals, theLatticeGenerators;
  if (partFraction::MakingConsistencyCheck)
    partFraction::CheckSum.MakeZero();
  if (partFraction::flagAnErrorHasOccurredTimeToPanic)
    this->ComputeDebugString(owner, theGlobalVariables);
  this->GetRootsFromDenominator(owner, theLatticeGenerators);
  Lattice theLattice;
  theLattice.MakeFromRoots(theLatticeGenerators);
  MatrixLargeRational theNormalsMatForm;
  theNormalsMatForm.AssignRootsToRowsOfMatrix(theLatticeGenerators);
  theNormalsMatForm.Invert();
  theNormals.AssignMatrixColumns(theNormalsMatForm);
  output.MakeZeroLatTiceZn(owner.AmbientDimension);

  for (int i=0; i<this->Coefficient.size; i++)
  { this->ComputePolyCorrespondingToOneMonomial(owner, shiftedPoly, i, theNormals, theLattice, theGlobalVariables);
    output.ComputeDebugString();
    shiftedPoly.ComputeDebugString();
    output+=shiftedPoly;
//    if (RecordNumMonomials)
//    { std::stringstream out4, out3;
//      out4 << "Current fraction: " << i+1<< " out of " << this->Coefficient.size << " processed";
//      partFractions::NumProcessedForVPFMonomialsTotal++;
//      out3  << " Processed " << partFractions::NumProcessedForVPFMonomialsTotal << " out of " << partFractions::NumMonomialsInNumeratorsRelevantFractions << " relevant monomials";
//      theGlobalVariables.theIndicatorVariables.ProgressReportStrings[3]= out4.str();
//      theGlobalVariables.theIndicatorVariables.ProgressReportStrings[2]= out3.str();
//      theGlobalVariables.FeedIndicatorWindow(theGlobalVariables.theIndicatorVariables);
//    }
  }
//  if (partFraction::MakingConsistencyCheck)
//  { Rational tempLRat;
//    output.Evaluate(partFraction::theVectorToBePartitioned, tempLRat);
//    assert(tempLRat.DenShort==1);
//    assert(tempLRat.IsEqualTo(partFraction::CheckSum));
//  }
//  if (StoreToFile)
//  { this->FileStoragePosition= partFraction::TheBigDump.tellp();
//    output.WriteToFile(partFraction::TheBigDump);
//    partFractions::ComputedContributionsList.flush();
//  }

//  Accum.ComputeDebugString();
}

bool partFractions::GetVectorPartitionFunction
  (QuasiPolynomial& output, root& newIndicator, GlobalVariables& theGlobalVariables)
{ if(this->AssureIndicatorRegularity(theGlobalVariables, newIndicator))
  { theGlobalVariables.theIndicatorVariables.flagRootIsModified=true;
    theGlobalVariables.theIndicatorVariables.modifiedRoot.AssignRoot(newIndicator);
    theGlobalVariables.theIndicatorVariables.ProgressReportStrings[4]="Indicator modified to regular";
    theGlobalVariables.FeedIndicatorWindow(theGlobalVariables.theIndicatorVariables);
  } else
    theGlobalVariables.theIndicatorVariables.flagRootIsModified=false;
  this->ResetRelevanceIsComputed();
  if (!this->CheckForMinimalityDecompositionWithRespectToRoot(&newIndicator, theGlobalVariables))
    return false;
  this->NumProcessedForVPFfractions=0;
  Rational oldCheckSum;
  QuasiPolynomialOld oldOutput;
  if (partFraction::MakingConsistencyCheck)
    partFractions::CheckSum.MakeZero();
  theGlobalVariables.theIndicatorVariables.NumProcessedMonomialsCurrentFraction=0;
  output.MakeZeroLatTiceZn(this->AmbientDimension);
  ///////////////////////////////////////////////
  //this->flagAnErrorHasOccurredTimeToPanic=true;
  //partFraction::flagAnErrorHasOccurredTimeToPanic=true;
  //this->ComputeDebugString();
  ///////////////////////////////////////////////
  QuasiPolynomial tempQP;
  for (int i=0; i<this->size; i++)
  { //if (this->flagAnErrorHasOccurredTimeToPanic)
    //{ this->TheObjects[i].ComputeDebugString();
    //}
    if (this->TheObjects[i].rootIsInFractionCone(*this, &newIndicator, theGlobalVariables))
    { partFraction& currentPF=this->TheObjects[i];
      currentPF.GetVectorPartitionFunction(*this, tempQP, theGlobalVariables);
/*      if(partFraction::MakingConsistencyCheck)
      {  Rational tempLRat2, tempLRat3, tempLRat4;
        std::string tempS1, tempS2, tempS3, tempS4;
        tempQP.Evaluate(partFraction::theVectorToBePartitioned, tempLRat2);
        output.Evaluate(partFraction::theVectorToBePartitioned, tempLRat3);
        tempLRat2.ElementToString(tempS1);
        tempLRat3.ElementToString(tempS2);
        tempLRat4.Assign(tempLRat2);
        tempLRat4.Add(tempLRat3);
        partFraction::CheckSum.ElementToString(tempS3);
        partFractions::CheckSum.ElementToString(tempS4);
        assert(tempLRat2.den.IsEqualTo(LIOne));
        assert(tempLRat3.den.IsEqualTo(LIOne));
        assert(tempLRat2.IsEqualTo(partFraction::CheckSum));
        assert(tempLRat4.IsEqualTo(partFractions::CheckSum));
        if (i==4)
        { Stop();
          QuasiPolynomialOld::flagAnErrorHasOccurredTimeToPanic=true;
          ::RandomCodeIDontWantToDelete theEvilBug;
          theEvilBug.EvilPoly1.Assign(output);
          theEvilBug.EvilPoly2.Assign(tempQP);
          theEvilBug.RevealTheEvilConspiracy();
          RandomCodeIDontWantToDelete::UsingEvilList1=false;
        }
      }*/
      output+=tempQP;
  /*    if (partFraction::MakingConsistencyCheck)
      {  Rational tempLRat;
        output.Evaluate(partFraction::theVectorToBePartitioned, tempLRat);
        if ((! tempLRat.IsEqualTo(partFractions::CheckSum)) || (! tempLRat.den.IsEqualTo(LIOne)))
        { std::string tempS, tempS2, tempS3, tempS4;
          Rational tempLRat2;
          tempQP.Evaluate(partFraction::theVectorToBePartitioned, tempLRat2);
          partFractions::CheckSum.ElementToString(tempS);
          tempLRat.ElementToString(tempS2);
          oldCheckSum.ElementToString(tempS3);
          tempLRat2.ElementToString(tempS4);
          partFraction::flagAnErrorHasOccurredTimeToPanic=true;
  //        assert(false);
        }
        oldCheckSum.Assign(partFractions::CheckSum);
        oldOutput.Assign(output);
      }*/
      this->MakeProgressVPFcomputation(theGlobalVariables);
    }
  }
/*  if (partFraction::MakingConsistencyCheck)
  { std::string tempS;
    partFractions::CheckSum.ElementToString(tempS);
    Stop();
  }*/
  return true;
}

bool PolynomialRationalCoeff::GetIntegerPoly(Polynomial<LargeInt>& output)const
{ output.Nullify(this->NumVars);
  Monomial<LargeInt> tempM;
  tempM.init(this->NumVars);
  output.MakeActualSizeAtLeastExpandOnTop(this->size);
  for (int i=0; i<this->size; i++)
  { Monomial<Rational>& currentMon=this->TheObjects[i];
    if (!currentMon.Coefficient.IsInteger())
      return false;
    for (int j=0; j<this->NumVars; j++)
      tempM.degrees[i]=currentMon.degrees[i];
    currentMon.Coefficient.GetNum(tempM.Coefficient);
    output.AddMonomial(tempM);
  }
  return true;
}

void LargeInt::WriteToFile(std::fstream& output)
{ std::string tempS;
  this->ElementToString(tempS);
  output << tempS;
}

void LargeInt::ReadFromFile(std::fstream& input)
{ std::string tempS;
  input >> tempS;
  this->AssignString(tempS);
}

void LargeInt::AssignString(const std::string& input)
{ if (input.size()==0)
    return;
  this->MakeZero();
  unsigned int startingIndex=0;
  if (input[0]=='-')
    startingIndex=1;
  for (unsigned int i=startingIndex; i<input.size(); i++)
  { int x=MathRoutines::GetIntFromDigit(input[i]);
    if (x==-1)
      return;
    if (i>startingIndex)
      this->value*=10;
    this->value+=(unsigned) x;
  }
  if (!this->IsEqualToZero())
    if (input[0]=='-')
      this->sign=-1;
}

int ParserNode::EvaluateSplit
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ ParserNode& thePFNode=theNode.owner->TheObjects[theArgumentList.TheObjects[0]];
  theNode.thePFs.GetElement()=thePFNode.thePFs.GetElement();
  theNode.thePFs.GetElement().split(theGlobalVariables, 0);
//  std::string tempS;
//  std::stringstream out;
//  theNode.thePFs.GetElement().ElementToString(tempS, theGlobalVariables);
//  out << "<div class=\"math\">" << tempS << "</div>";
//  theNode.outputString=out.str();
  theNode.ExpressionType=theNode.typePartialFractions;
  return theNode.errorNoError;
}

int ParserNode::EvaluatePartialFractions
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ roots theVectors;
  int theDim=-1;
  if (!theNode.GetRootsEqualDimNoConversionNoEmptyArgument(theArgumentList, theVectors, theDim))
    return theNode.SetError(theNode.errorDimensionProblem);
  //std::cout << theVectors.ElementToString();
  if (!theNode.thePFs.GetElement().initFromRoots(theVectors, theGlobalVariables))
    return theNode.SetError(theNode.errorImplicitRequirementNotSatisfied);
  std::string tempS;
  std::stringstream out;
  PolynomialOutputFormat theFormat;
  theNode.thePFs.GetElement().ElementToString(tempS, theGlobalVariables, theFormat);
  out << "<div class=\"math\">" << tempS << "</div>";
  theNode.outputString=out.str();
  theNode.ExpressionType=theNode.typePartialFractions;
  return theNode.errorNoError;
}

void ParserNode::CreateDefaultLatexAndPDFfromString
(std::string& theLatexFileString)
{ std::fstream outputFile;
  std::string fileName;
  std::stringstream out;
  fileName.append(this->owner->outputFolderPath);
  fileName.append("output.tex");
  CGIspecificRoutines::OpenDataFileOrCreateIfNotPresent(outputFile, fileName, false, true, false);
  outputFile << theLatexFileString;
  out << "A latex/pdf file: <a href=\"" << this->owner->outputFolderDisplayPath << "output.tex\"> output.tex</a>";
  out << ", <a href=\"" << this->owner->outputFolderDisplayPath << "output.pdf\"> output.pdf</a>";
  this->outputString=out.str();
  std::stringstream theCommand;
  theCommand << "pdflatex -output-directory=" << this->owner->outputFolderPath << "   " << fileName ;
  //std::cout << theCommand.str();
  this->owner->SystemCommands.AddObjectOnTop(theCommand.str());
  this->ExpressionType=this->typeString;
}

std::string partFractions::DoTheFullComputationReturnLatexFileString(GlobalVariables& theGlobalVariables, roots& toBePartitioned, PolynomialOutputFormat& theFormat)
{ if (toBePartitioned.size<1)
    return "";
  this->AmbientDimension= toBePartitioned.TheObjects[0].size;
  this->theChambers.AmbientDimension= this->AmbientDimension;
  this->theChambers.theDirections.CopyFromBase(toBePartitioned);
  return this->DoTheFullComputationReturnLatexFileString(theGlobalVariables, theFormat);
}

std::string partFractions::DoTheFullComputationReturnLatexFileString(GlobalVariables& theGlobalVariables, PolynomialOutputFormat& theFormat)
{ this->theChambers.thePauseController.InitComputation();
  //this->theChambers.ReadFromDefaultFile(theGlobalVariables);
  std::stringstream out;
  this->theChambers.SliceTheEuclideanSpace(theGlobalVariables, false);
  this->theChambers.QuickSortAscending();
  this->theChambers.LabelChamberIndicesProperly();
  root tempRoot; tempRoot.MakeZero(this->AmbientDimension);
  tempRoot.MakeZero(this->AmbientDimension);
  this->theChambers.drawOutput(theGlobalVariables.theDrawingVariables, tempRoot, 0);
  this->theChambers.thePauseController.ExitComputation();
  this->initFromRoots(theChambers.theDirections, theGlobalVariables);
  out << "\\documentclass{article}\\usepackage{amsmath, amsfonts, amssymb} \n\\begin{document}\n";
  out << "The vector partition funciton is the number of ways you can represent a vector $(x_1,\\dots, x_n)$ as a non-negative integral linear combination of "
        << " a given set of vectors.  You requested the vector partition function with respect to the set of vectors: " << theChambers.theDirections.ElementToStringGeneric();
  out << "\n\n The corresponding generating function is: " << this->ElementToString(theGlobalVariables, theFormat) << "= (after splitting acording to algorithm)";
  this->split(theGlobalVariables, 0);
  out << this->ElementToString(theGlobalVariables, theFormat);
  out << "Therefore the vector partition function is given by " << this->theChambers.GetNumNonZeroPointers()
        << " quasipolynomials depending on which set of linear inequalities is satisfied (each such set we call ``Chamber'').";
  QuasiPolynomial tempQP;
  std::string tempS;
  for (int i=0; i<this->theChambers.size; i++)
    if (this->theChambers.TheObjects[i]!=0)
    { CombinatorialChamber& currentChamber=*this->theChambers.TheObjects[i];
      this->GetVectorPartitionFunction(tempQP, currentChamber.InternalPoint, theGlobalVariables);
      currentChamber.ElementToInequalitiesString(tempS, this->theChambers, true, false, theFormat);
      out << "\n\nChamber: " << tempS;
      out << "Quasipolynomial: " << tempQP.ElementToString(false, true, theFormat);
    }
  out << "\\end{document}";
  return out.str();
}

int ParserNode::EvaluateVectorPFIndicator
(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ partFractions& currentPF=theNode.thePFs.GetElement();
  roots toBePartitioned; int tempDim;
  theNode.GetRootsEqualDimNoConversionNoEmptyArgument(theArgumentList, toBePartitioned, tempDim);
  std::stringstream out;
  PolynomialOutputFormat theFormat;
//  theFormat.alphabet.TheObjects[0]="t_1";
//  theFormat.alphabet.TheObjects[1]="t_2";
  std::string tempS= currentPF.DoTheFullComputationReturnLatexFileString(theGlobalVariables, toBePartitioned, theFormat);
  theNode.CreateDefaultLatexAndPDFfromString(tempS);
  return theNode.errorNoError;
}

void QuasiPolynomial::AddLatticeShift(const PolynomialRationalCoeff& input, const root& inputShift)
{ root theShift=inputShift;
  this->AmbientLatticeReduced.ReduceVector(theShift);
  int index=this->LatticeShifts.IndexOfObject(theShift);
  if (index==-1)
  { index=this->LatticeShifts.size;
    this->LatticeShifts.AddObjectOnTop(theShift);
    this->valueOnEachLatticeShift.ExpandOnTop(1);
    this->valueOnEachLatticeShift.LastObject()->Nullify(input.NumVars);
  }
  this->valueOnEachLatticeShift.TheObjects[index]+=input;
  if (this->valueOnEachLatticeShift.TheObjects[index].IsEqualToZero())
  { this->LatticeShifts.PopIndexSwapWithLast(index);
    this->valueOnEachLatticeShift.PopIndexSwapWithLast(index);
  }
}


void QuasiPolynomial::Substitution
  (const MatrixLargeRational& mapFromNewSpaceToOldSpace,
   const Lattice& ambientLatticeNewSpace, QuasiPolynomial& output, GlobalVariables& theGlobalVariables)
{ //Format of the substitution.
  //If we want to carry out a substitution in P(y_1, ..., y_m),
  // of the form
  //y_1=a_11 x_1+...+a_1nx_n
  //...
  //y_m=a_m1 x_1+...+a_mnx_n
  // then the mapFromNewSpaceToOldSpace shouldbe the matrix
  //a_11 ... a_1n
  //...
  //a_m1 ... a_mn
  assert(this!=&output);
  assert(mapFromNewSpaceToOldSpace.NumRows==this->GetNumVars());
  assert(ambientLatticeNewSpace.GetDim()==mapFromNewSpaceToOldSpace.NumCols);
  output.AmbientLatticeReduced=ambientLatticeNewSpace;
  output.AmbientLatticeReduced.IntersectWithPreimageOfLattice(mapFromNewSpaceToOldSpace, this->AmbientLatticeReduced, theGlobalVariables);
  roots allRepresentatives, imagesAllRepresentatives;
  bool tempBool=ambientLatticeNewSpace.GetAllRepresentatives(output.AmbientLatticeReduced, allRepresentatives);
  assert(tempBool);
  mapFromNewSpaceToOldSpace.ActOnRoots(allRepresentatives, imagesAllRepresentatives);
  PolynomialsRationalCoeff theSub;
  theSub.SetSize(this->GetNumVars());
  root tempRoot;
  for (int i=0; i<theSub.size; i++)
  { Polynomial<Rational>& currentPoly=theSub.TheObjects[i];
    mapFromNewSpaceToOldSpace.RowToRoot(i, tempRoot);
    currentPoly.MakeLinPolyFromRootNoConstantTerm(tempRoot);
  }
  PolynomialRationalCoeff tempP;
  for (int i=0; i<this->valueOnEachLatticeShift.size; i++)
  { this->valueOnEachLatticeShift.TheObjects[i].Substitution(theSub, tempP, mapFromNewSpaceToOldSpace.NumCols, (Rational) 1);
    for (int j=0; j<allRepresentatives.size; j++)
      if (imagesAllRepresentatives.TheObjects[j]==this->LatticeShifts.TheObjects[i])
        output.AddLatticeShift(tempP, allRepresentatives.TheObjects[j]);
  }
}

void QuasiPolynomial::Substitution
(const root& inputTranslation, QuasiPolynomial& output, GlobalVariables& theGlobalVariables)
{ //format of the translation. If the starting quasipolynomial was P(y_1, ..., y_n),
  //and the translation has coordinates (t_1, ..., t_n),
  //then the resulting quasipolynomial will be P(x_1-t_1, ..., x_n-t_n)
  PolynomialsRationalCoeff theSub;
  theSub.MakeIdSubstitution(this->GetNumVars(), (Rational) 1);
  for (int i=0; i<theSub.size; i++)
    theSub.TheObjects[i].AddConstant(-inputTranslation.TheObjects[i]);
  PolynomialRationalCoeff tempP;
  output.MakeZeroLatTiceZn(this->GetNumVars());
  output.AmbientLatticeReduced=this->AmbientLatticeReduced;
  for (int i=0; i<this->valueOnEachLatticeShift.size; i++)
  { this->valueOnEachLatticeShift.TheObjects[i].Substitution(theSub, tempP, this->GetNumVars(), (Rational) 1);
    output.AddLatticeShift(tempP, this->LatticeShifts.TheObjects[i]+inputTranslation);
  }
}

void QuasiPolynomial::Substitution
  (const MatrixLargeRational& mapFromNewSpaceToOldSpace, const root& inputTranslation,
   const Lattice& ambientLatticeNewSpace, QuasiPolynomial& output, GlobalVariables& theGlobalVariables)
{ QuasiPolynomial tempQP;
  this->Substitution(inputTranslation, tempQP, theGlobalVariables);
  tempQP.Substitution(mapFromNewSpaceToOldSpace, ambientLatticeNewSpace, output, theGlobalVariables);
}

bool QuasiPolynomial::SubstitutionLessVariables
  (const PolynomialsRationalCoeff& theSub, QuasiPolynomial& output, GlobalVariables& theGlobalVariables)const
{ MatrixLargeRational theLatticeSub;
  if (!this->AmbientLatticeReduced.GetHomogeneousSubMatFromSubIgnoreConstantTerms(theSub, theLatticeSub, theGlobalVariables))
    return false;
  MatrixLargeRational theSubLatticeShift;
  output.AmbientLatticeReduced=this->AmbientLatticeReduced;
  if(!output.AmbientLatticeReduced.SubstitutionHomogeneous(theLatticeSub, theGlobalVariables))
    return false;
  theSubLatticeShift.init(theLatticeSub.NumRows,1);
  for (int i=0; i<theSubLatticeShift.NumRows; i++)
    theSub.TheObjects[i].GetConstantTerm(theSubLatticeShift.elements[i][0], (Rational) 0);
  MatrixLargeRational theShiftImage, shiftMatForm;
  output.LatticeShifts.size=0;
  output.valueOnEachLatticeShift.size=0;
  output.valueOnEachLatticeShift.MakeActualSizeAtLeastExpandOnTop(this->LatticeShifts.size);
  output.LatticeShifts.MakeActualSizeAtLeastExpandOnTop(this->LatticeShifts.size);
  root tempRoot;
  PolynomialRationalCoeff tempP;
  for (int i=0; i<this->LatticeShifts.size; i++)
  { shiftMatForm.AssignVectorColumn(this->LatticeShifts.TheObjects[i]);
    shiftMatForm-=theSubLatticeShift;
    if (theLatticeSub.Solve_Ax_Equals_b_ModifyInputReturnFirstSolutionIfExists(theLatticeSub, shiftMatForm, theShiftImage))
    { tempRoot.AssignMatDetectRowOrColumn(theShiftImage);
      this->valueOnEachLatticeShift.TheObjects[i].Substitution(theSub, tempP, tempRoot.size, (Rational) 1);
      output.AddLatticeShift(tempP, tempRoot);
    }
  }
  return true;
}

bool Lattice::SubstitutionHomogeneous
    (const PolynomialsRationalCoeff& theSub, GlobalVariables& theGlobalVariables)
{ MatrixLargeRational matSub;
  if (!this->GetHomogeneousSubMatFromSubIgnoreConstantTerms(theSub, matSub, theGlobalVariables))
    return false;
  return this->SubstitutionHomogeneous(matSub, theGlobalVariables);
}

bool Lattice::GetHomogeneousSubMatFromSubIgnoreConstantTerms
(const PolynomialsRationalCoeff& theSub, MatrixLargeRational& output, GlobalVariables& theGlobalVariables)
{ if (theSub.size<1)
    return false;
  int theTargetDim=theSub.TheObjects[0].NumVars;
  output.init(theSub.size, theTargetDim);
  for (int i=0; i<theSub.size; i++)
  { Polynomial<Rational>& currentPoly=theSub.TheObjects[i];
    if (!currentPoly.IsLinear())
      return false;
    for (int j=0; j<theTargetDim; j++)
      currentPoly.GetCoeffInFrontOfLinearTermVariableIndex(j, output.elements[i][j], (Rational) 0);
  }
  return true;
}

void Lattice::IntersectWithLinearSubspaceGivenByNormal(const root& theNormal)
{ //std::cout << "<br>Starting lattice: " << this->ElementToString(true, false) << "<br>";
  roots startingBasis, resultBasis;
  startingBasis.AssignMatrixRows(this->basisRationalForm);
  //std::cout << "Starting normal: " << theNormal.ElementToString();
  root theScalarProducts;
  theScalarProducts.SetSize(startingBasis.size);
  for (int i=0; i<this->basisRationalForm.NumRows; i++)
    theScalarProducts.TheObjects[i]=root::RootScalarEuclideanRoot(startingBasis.TheObjects[i], theNormal);
  if (theScalarProducts.IsEqualToZero())
    return;
  //std::cout << "<br>the scalar products: " << theScalarProducts.ElementToString();
  int pivotColumnIndex=theScalarProducts.getIndexLastNonZeroCoordinate();
  Rational pivotCoeff=theScalarProducts.TheObjects[pivotColumnIndex];
  theScalarProducts/=-pivotCoeff;
  //std::cout << "<br>the scalar products after scaling: " << theScalarProducts.ElementToString();
  roots eigenSpacePlusOrthogonalComponent;
  eigenSpacePlusOrthogonalComponent.SetSize(theScalarProducts.size);
  for (int i=0; i<theScalarProducts.size; i++)
    if (i!=pivotColumnIndex)
    { root& currentRoot=eigenSpacePlusOrthogonalComponent.TheObjects[i];
      currentRoot.MakeZero(theScalarProducts.size);
      currentRoot.TheObjects[i]=1;
      currentRoot.TheObjects[pivotColumnIndex]=theScalarProducts.TheObjects[i];
    }
  theScalarProducts.ScaleByPositiveRationalToIntegralMinHeight();
  //std::cout << "<br>the scalar products after scaling to integral: " << theScalarProducts.ElementToString();
  eigenSpacePlusOrthogonalComponent.TheObjects[pivotColumnIndex]=theScalarProducts;
  //std::cout << "<br>The eigenspace before intersection: " << eigenSpacePlusOrthogonalComponent.ElementToString();
  Lattice eigenLattice, theZnLattice;
  eigenLattice.MakeFromRoots(eigenSpacePlusOrthogonalComponent);
  //std::cout << "<br>The eigen-Lattice: " << eigenLattice.ElementToString(true, false);
  theZnLattice.MakeZn(theScalarProducts.size);
  //std::cout << "<br>The Zn-Lattice: " << theZnLattice.ElementToString(true, false);
  theZnLattice.IntersectWithBothOfMaxRank(eigenLattice);
  //std::cout << "<br>Zn intersected with eigen-Lattice: " << theZnLattice.ElementToString(true, false);
  resultBasis.MakeActualSizeAtLeastExpandOnTop(theScalarProducts.size-1);
  root tempRoot, resultRoot; Rational orthogonalComponent;
  for (int i=0; i<theZnLattice.basisRationalForm.NumRows; i++)
  { theZnLattice.basisRationalForm.RowToRoot(i, tempRoot);
    orthogonalComponent=tempRoot.ScalarEuclidean(theScalarProducts)/theScalarProducts.ScalarEuclidean(theScalarProducts);
    tempRoot-=theScalarProducts*orthogonalComponent;
    assert(orthogonalComponent.IsInteger());
    if (!tempRoot.IsEqualToZero())
    { resultRoot.MakeZero(this->GetDim());
      for (int j=0; j<startingBasis.size; j++)
        resultRoot+=startingBasis.TheObjects[j]*tempRoot.TheObjects[j];
      resultBasis.AddObjectOnTop(resultRoot);
    }
  }
  //std::cout << "<br>Resulting basis: " << resultBasis.ElementToString();
  this->MakeFromRoots(resultBasis);
  //std::cout << "<br>Final answer lattice form: " << this->ElementToString(true, false);
}

void Lattice::IntersectWithLinearSubspaceSpannedBy(const roots& theSubspaceBasis)
{ roots theNormals;
  MatrixLargeRational theMat;
  theSubspaceBasis.GetMatRootsToRows(theMat);
  theMat.FindZeroEigenSpace(theNormals);
  this->IntersectWithLinearSubspaceGivenByNormals(theNormals);
}

void Lattice::IntersectWithLinearSubspaceGivenByNormals(const roots& theSubspaceNormals)
{ //std::cout << "<br>********************Debug info for IntersectWithLinearSubspaceGivenByNormals*******************";
  for (int i=0; i<theSubspaceNormals.size; i++)
    this->IntersectWithLinearSubspaceGivenByNormal(theSubspaceNormals.TheObjects[i]);
  //std::cout << "<br>********************End of debug info for IntersectWithLinearSubspaceGivenByNormals*******************";
}

bool Lattice::SubstitutionHomogeneous
 (const MatrixLargeRational& theSub, Lattice& resultIsSubsetOf, GlobalVariables& theGlobalVariables)
{ /*roots preimageBasis;
  preimageBasis.AssignMatrixRows(this->basisRationalForm);
  MatrixLargeRational theSubModifiable, currentBasisVector, oneSolution;
  for (int i=0; i<preimageBasis.size; i++)
  { theSubModifiable=theSub;
    currentBasisVector.AssignVectorColumn(preimageBasis.TheObjects[i]);
    if (theSubModifiable.Solve_Ax_Equals_b_ModifyInputReturnFirstSolutionIfExists(theSubModifiable, currentBasisVector, oneSolution))
    { theSubModifiable=theSub;
      theSubModifiable.fin
    }
  }*/
return false;
}

bool Lattice::SubstitutionHomogeneous
  (const MatrixLargeRational& theSub, GlobalVariables& theGlobalVariables)
{ int targetDim=theSub.NumCols;
  if (theSub.NumRows!=this->GetDim())
    return false;
  //std::cout <<"<br> the sub: " << theSub.ElementToString(true, false) << "<br>";
  int startingDim=this->GetDim();
  MatrixLargeRational theMat, oldBasisTransformed, matRelationBetweenStartingVariables;
  theMat=theSub;
  oldBasisTransformed=this->basisRationalForm;
  oldBasisTransformed.Transpose();
  Selection nonPivotPoints;
  theMat.ComputeDebugString(false, false);
  //std::cout << "<br>the matrices to be transformed: " << theMat.ElementToString(true, false) << "<br>" << oldBasisTransformed.ElementToString(true, false);
  theMat.GaussianEliminationByRows(oldBasisTransformed, nonPivotPoints);
  //std::cout << "<br>afer transformation: " << theMat.ElementToString(true, false) << "<br>" << oldBasisTransformed.ElementToString(true, false);
  if (nonPivotPoints.CardinalitySelection!=0)
    return false;
  int numNonZeroRows=nonPivotPoints.MaxSize;
  int numZeroRows=theMat.NumRows-numNonZeroRows;
  matRelationBetweenStartingVariables.init(numZeroRows, startingDim);
  for (int i=0; i<numZeroRows; i++)
    for (int j=0; j<startingDim; j++)
      matRelationBetweenStartingVariables.elements[i][j]=oldBasisTransformed.elements[i+numNonZeroRows][j];
  roots theEigenSpace;
  matRelationBetweenStartingVariables.FindZeroEigenSpace(theEigenSpace, theGlobalVariables);
  //std::cout << "<br>matRelationBetweenStartingVariables" <<  matRelationBetweenStartingVariables.ElementToString(true, false);
  for (int i=0; i<theEigenSpace.size; i++)
    theEigenSpace.TheObjects[i].ScaleByPositiveRationalToIntegralMinHeight();
  //std::cout << "the basis: " << theEigenSpace.ElementToString();
  oldBasisTransformed.ActOnRoots(theEigenSpace);
  //std::cout << "<br>the basis transformed: " << theEigenSpace.ElementToString();
  this->basisRationalForm.init(targetDim, targetDim);
  for (int i=0; i<targetDim; i++)
    for (int j=0; j<targetDim; j++)
      this->basisRationalForm.elements[i][j]=theEigenSpace.TheObjects[i].TheObjects[j];
  this->basisRationalForm.GetMatrixIntWithDen(this->basis, this->Den);
  this->Reduce();
  //std::cout << "<br><br>and the sub result is: <br>" << this->ElementToString(true, false);
  return true;
}

void QuasiPolynomial::operator*=(const Rational& theConst)
{ if (theConst.IsEqualToZero())
  { this->valueOnEachLatticeShift.size=0;
    this->LatticeShifts.size=0;
    return;
  }
  for (int i=0; i<this->valueOnEachLatticeShift.size; i++)
    this->valueOnEachLatticeShift.TheObjects[i]*=theConst;
}

void Cone::WriteToFile
  (std::fstream& output, GlobalVariables* theGlobalVariables)
{ output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE(this->GetXMLClassName());
  output << "Cone( ";
  for (int i=0; i<this->Normals.size; i++)
  { output << "(";
    for (int j=0; j<this->Normals.TheObjects[i].size; j++)
    { output << this->Normals.TheObjects[i].TheObjects[j].ElementToString();
      if (j!=this->Normals.TheObjects[i].size-1)
        output << ",";
    }
    output << ")";
    if (i!=this->Normals.size-1)
      output << ", ";
  }
  output << " ) ";
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE(this->GetXMLClassName());
}

bool Cone::ReadFromFile(std::fstream& output, GlobalVariables* theGlobalVariables)
{ if (theGlobalVariables!=0)
    return this->ReadFromFile(output, theGlobalVariables->rootsConeWriteToFileBuffer, theGlobalVariables);
  else
  { roots tempRoots;
    return this->ReadFromFile(output, tempRoots, 0);
  }
}

bool Cone::ReadFromFile
  (std::fstream& input, roots& buffer, GlobalVariables* theGlobalVariables)
{ std::string tempS, rootString;
  int NumWordsRead;
  XMLRoutines::ReadThroughFirstOpenTag(input, NumWordsRead, this->GetXMLClassName());
  assert(NumWordsRead==0);
  input >> tempS;
  buffer.size=0;
  root tempRoot;
  if (tempS!="Cone(")
  { std::cout << "tempS was instead " << tempS;
    XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, NumWordsRead, this->GetXMLClassName());
    return false;
  }
  for (input >> tempS; tempS!=")" && tempS!=""; input >> tempS)
  { tempRoot.AssignString(tempS);
    buffer.AddObjectOnTop(tempRoot);
    //std::cout << "vector input " << tempS << " read as " << tempRoot.ElementToString();
  }
  if (buffer.size<1)
  { XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, NumWordsRead, this->GetXMLClassName());
    return false;
  }
  int theDim=buffer.TheObjects[0].size;
  for (int i=1; i<buffer.size; i++)
    if (buffer.TheObjects[i].size!=theDim)
    { XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, NumWordsRead, this->GetXMLClassName());
      return false;
    }
  bool result;
  if (theGlobalVariables!=0)
    result=this->CreateFromNormals(buffer, *theGlobalVariables);
  else
  { GlobalVariables tempGlobalVariables;
    result=this->CreateFromNormals(buffer, tempGlobalVariables);
  }
  result= XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, NumWordsRead, this->GetXMLClassName()) && result;
//  assert(tempBool);
  return result;
}

void ConeLatticeAndShift::WriteToFile
  (std::fstream& output, GlobalVariables* theGlobalVariables)
{ output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE(this->GetXMLClassName());
  this->theLattice.WriteToFile(output, theGlobalVariables);
  this->theProjectivizedCone.WriteToFile(output, theGlobalVariables);
  this->theShift.WriteToFile(output);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE(this->GetXMLClassName());
}

bool ConeLatticeAndShift::ReadFromFile
  (std::fstream& output, GlobalVariables* theGlobalVariables)
{ XMLRoutines::ReadThroughFirstOpenTag(output, this->GetXMLClassName());
  this->theLattice.ReadFromFile(output, theGlobalVariables);
  this->theProjectivizedCone.ReadFromFile(output, theGlobalVariables);
  this->theShift.ReadFromFile(output);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(output, this->GetXMLClassName());
  return true;
}

void ConeLatticeAndShiftMaxComputation::WriteToFile
  (std::fstream& output, GlobalVariables* theGlobalVariables)
{ output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE(this->GetXMLClassName());
  output << " NumParam: " << this->numNonParaM << " numProcessedNonParam: " << this->numProcessedNonParam << "\n";

  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("complexStartingPerRepresentative");
  this->complexStartingPerRepresentative.WriteToFile(output, theGlobalVariables);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("complexStartingPerRepresentative");

  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("complexRefinedPerRepresentative");
  this->complexRefinedPerRepresentative.WriteToFile(output, theGlobalVariables);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("complexRefinedPerRepresentative");

  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("theMaximaCandidates");
  this->theMaximaCandidates.WriteToFile(output);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("theMaximaCandidates");

  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("startingLPtoMaximize");
  this->startingLPtoMaximize.WriteToFile(output);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("startingLPtoMaximize");

  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("finalMaxima");
  this->finalMaxima.WriteToFile(output);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("finalMaxima");

  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("theStartingLattice");
  this->theStartingLattice.WriteToFile(output, theGlobalVariables);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("theStartingLattice");

  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("theFinalRougherLattice");
  this->theFinalRougherLattice.WriteToFile(output, theGlobalVariables);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("theFinalRougherLattice");

  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("theStartingRepresentative");
  this->theStartingRepresentative.WriteToFile(output);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("theStartingRepresentative");

  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("theFinalRepresentatives");
  this->theFinalRepresentatives.WriteToFile(output, theGlobalVariables);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("theFinalRepresentatives");

  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("theConesLargerDim");
  this->theConesLargerDim.WriteToFile(output, theGlobalVariables);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("theConesLargerDim");

  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("theConesSmallerDim");
  this->theConesSmallerDim.WriteToFile(output, theGlobalVariables);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("theConesSmallerDim");

  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("IsInfinity");
  output << this->IsInfinity;
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("IsInfinity");

  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("LPtoMaximizeLargerDim");
  this->LPtoMaximizeLargerDim.WriteToFile(output, theGlobalVariables);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("LPtoMaximizeLargerDim");

  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("LPtoMaximizeSmallerDim");
  this->LPtoMaximizeSmallerDim.WriteToFile(output, theGlobalVariables);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("LPtoMaximizeSmallerDim");

  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE(this->GetXMLClassName()) << "\n";
}

bool ConeLatticeAndShiftMaxComputation::ReadFromFile
(std::fstream& input, GlobalVariables* theGlobalVariables, int UpperLimitDebugPurposes)
{ int numReadWords;
  XMLRoutines::ReadThroughFirstOpenTag(input, numReadWords, this->GetXMLClassName());
  std::string tempS;
  input >> tempS >> this->numNonParaM >> tempS >> this->numProcessedNonParam;
  if (theGlobalVariables!=0)
  { theGlobalVariables->IncrementReadWriteDepth();
    theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading complex starting per representative...";
    theGlobalVariables->MakeReport();
  }
  XMLRoutines::ReadThroughFirstOpenTag(input, "complexStartingPerRepresentative");
  this->complexStartingPerRepresentative.ReadFromFile(input, theGlobalVariables);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, "complexStartingPerRepresentative");

  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading complex refined per representative...";
    theGlobalVariables->MakeReport();
  }
  XMLRoutines::ReadThroughFirstOpenTag(input, "complexRefinedPerRepresentative");
  this->complexRefinedPerRepresentative.ReadFromFile(input, theGlobalVariables);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, "complexRefinedPerRepresentative");

  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading the max candidates...";
    theGlobalVariables->MakeReport();
  }
  XMLRoutines::ReadThroughFirstOpenTag(input, "theMaximaCandidates");
  this->theMaximaCandidates.ReadFromFile(input, theGlobalVariables);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, "theMaximaCandidates");

  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading starting linear polys...";
    theGlobalVariables->MakeReport();
  }
  XMLRoutines::ReadThroughFirstOpenTag(input, "startingLPtoMaximize");
  this->startingLPtoMaximize.ReadFromFile(input, theGlobalVariables);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, "startingLPtoMaximize");

  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading final maxima...";
    theGlobalVariables->MakeReport();
  }
  XMLRoutines::ReadThroughFirstOpenTag(input, "finalMaxima");
  this->finalMaxima.ReadFromFile(input, theGlobalVariables);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, "finalMaxima");

  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading starting lattice...";
    theGlobalVariables->MakeReport();
  }
  XMLRoutines::ReadThroughFirstOpenTag(input, "theStartingLattice");
  this->theStartingLattice.ReadFromFile(input, theGlobalVariables);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, "theStartingLattice");

  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading final rougher lattice...";
    theGlobalVariables->MakeReport();
  }
  XMLRoutines::ReadThroughFirstOpenTag(input, "theFinalRougherLattice");
  this->theFinalRougherLattice.ReadFromFile(input, theGlobalVariables);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, "theFinalRougherLattice");

  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading starting representative...";
    theGlobalVariables->MakeReport();
  }
  XMLRoutines::ReadThroughFirstOpenTag(input, "theStartingRepresentative");
  this->theStartingRepresentative.ReadFromFile(input);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, "theStartingRepresentative");

  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading final representatives...";
    theGlobalVariables->MakeReport();
  }
  XMLRoutines::ReadThroughFirstOpenTag(input, "theFinalRepresentatives");
  this->theFinalRepresentatives.ReadFromFile(input, theGlobalVariables);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, "theFinalRepresentatives");

  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading cones larger dim...";
    theGlobalVariables->MakeReport();
  }
  XMLRoutines::ReadThroughFirstOpenTag(input, "theConesLargerDim");
  this->theConesLargerDim.ReadFromFile(input, theGlobalVariables, UpperLimitDebugPurposes);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, "theConesLargerDim");

  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading cones smaller dim...";
    theGlobalVariables->MakeReport();
  }
  XMLRoutines::ReadThroughFirstOpenTag(input, "theConesSmallerDim");
  this->theConesSmallerDim.ReadFromFile(input, theGlobalVariables);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, "theConesSmallerDim");

  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading IsInfinite array...";
    theGlobalVariables->MakeReport();
  }
  XMLRoutines::ReadThroughFirstOpenTag(input, "IsInfinity");
  input >> this->IsInfinity;
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, "IsInfinity");

  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading LPtoMaximizeLargerDim...";
    theGlobalVariables->MakeReport();
  }
  XMLRoutines::ReadThroughFirstOpenTag(input, "LPtoMaximizeLargerDim");
  this->LPtoMaximizeLargerDim.ReadFromFile(input, theGlobalVariables, UpperLimitDebugPurposes);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, "LPtoMaximizeLargerDim");

  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading LPtoMaximizeSmallerDim...";
    theGlobalVariables->MakeReport();
  }
  XMLRoutines::ReadThroughFirstOpenTag(input, "LPtoMaximizeSmallerDim");
  this->LPtoMaximizeSmallerDim.ReadFromFile(input, theGlobalVariables);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, "LPtoMaximizeSmallerDim");

  if (theGlobalVariables!=0)
    theGlobalVariables->ProgressReportDepth--;
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, numReadWords, this->GetXMLClassName());
  assert(numReadWords==0);
  return true;
}

void GeneralizedVermaModuleCharacters::WriteToFile
  (std::fstream& output, GlobalVariables* theGlobalVariables)
{ output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE(this->GetXMLClassName());
  output << "ComputationPhase: " << this->computationPhase << "\n";
  output << "NumProcessedConesParam: " << this->NumProcessedConesParam << "\n";
  output << "NumProcessedExtremaEqualOne: " << this->NumProcessedExtremaEqualOne << "\n";
  output << "NormalConstAdjustment: ";
  this->NonIntegralOriginModification.WriteToFile(output);
  output << "\n";
  output << "ChamberIndicatorHighestWeightLargerAlgebra: ";
  this->ParabolicLeviPartRootSpacesZeroStandsForSelected.WriteToFile(output);
  this->ParabolicSelectionSmallerAlgebra.WriteToFile(output);
  output << "\n";
  this->WeylLarger.WriteToFile(output);
  this->WeylSmaller.WriteToFile(output);
  this->preferredBasiS.WriteToFile(output);
  this->preferredBasisChangE.WriteToFile(output);
  this->preferredBasisChangeInversE.WriteToFile(output);
  this->theExtendedIntegralLatticeMatForM.WriteToFile(output, theGlobalVariables);
  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStringsNeedRefresh=true;
    theGlobalVariables->theIndicatorVariables.ProgressReportStrings[0]="Writing small data... ";
    theGlobalVariables->MakeReport();
  }
  this->theMaxComputation.WriteToFile(output, theGlobalVariables);
  this->GmodKnegativeWeightS.WriteToFile(output, theGlobalVariables);
  this->GmodKNegWeightsBasisChanged.WriteToFile(output, theGlobalVariables);
  this->theLinearOperators.WriteToFile(output);
  this->theLinearOperatorsExtended.WriteToFile(output);
  this->PreimageWeylChamberLargerAlgebra.WriteToFile(output, theGlobalVariables);
  this->PreimageWeylChamberSmallerAlgebra.WriteToFile(output, theGlobalVariables);
  this->WeylChamberSmallerAlgebra.WriteToFile(output, theGlobalVariables);
  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[0]="Writing QP's non-subbed... ";
    theGlobalVariables->MakeReport();
  }
  this->theQPsNonSubstituted.WriteToFile(output, theGlobalVariables);
  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[0]="Writing QP's subbed... ";
    theGlobalVariables->MakeReport();
  }

  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("QPsSubbed");
  this->theQPsSubstituted.WriteToFile(output, theGlobalVariables);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("QPsSubbed");
  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[0]="Writing small data... ";
    theGlobalVariables->MakeReport();
  }
  output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE("theMultiplicities");
  this->theMultiplicities.WriteToFile(output, theGlobalVariables, this->UpperLimitChambersForDebugPurposes);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE("theMultiplicities");
//  this->theMultiplicitiesExtremaCandidates.WriteToFile(output, theGlobalVariables);
  this->theCoeffs.WriteToFile(output);
  this->theTranslationS.WriteToFile(output, theGlobalVariables);
  this->theTranslationsProjecteD.WriteToFile(output, theGlobalVariables);
  this->thePfs.WriteToFile(output, theGlobalVariables);
//  this->paramSubChambers.WriteToFile(output, theGlobalVariables);
//  this->nonParamVertices.WriteToFile(output, theGlobalVariables);
  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[0]="Writing param chamber complex... ";
    theGlobalVariables->MakeReport();
  }
  this->projectivizedParamComplex.WriteToFile(output, theGlobalVariables);
  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[0]="Writing projectivized chamber complex... ";
    theGlobalVariables->MakeReport();
  }
  this->smallerAlgebraChamber.WriteToFile(output, theGlobalVariables, this->UpperLimitChambersForDebugPurposes);
  this->projectivizedChambeR.WriteToFile(output, theGlobalVariables, this->UpperLimitChambersForDebugPurposes);
  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[0]="Writing to file done...";
    theGlobalVariables->MakeReport();
  }
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE(this->GetXMLClassName());
}

void QuasiPolynomial::WriteToFile(std::fstream& output, GlobalVariables* theGlobalVariables)
{ this->valueOnEachLatticeShift.WriteToFile(output, theGlobalVariables);
  this->AmbientLatticeReduced.WriteToFile(output, theGlobalVariables);
  this->LatticeShifts.WriteToFile(output, theGlobalVariables);
}

bool QuasiPolynomial::ReadFromFile(std::fstream& input, GlobalVariables* theGlobalVariables)
{ this->valueOnEachLatticeShift.ReadFromFile(input, theGlobalVariables);
  this->AmbientLatticeReduced.ReadFromFile(input, theGlobalVariables);
  this->LatticeShifts.ReadFromFile(input, theGlobalVariables);
  this->buffers=theGlobalVariables;
  return true;
}

void Lattice::WriteToFile(std::fstream& output, GlobalVariables* theGlobalVariables)
{ output << XMLRoutines::GetOpenTagNoInputCheckAppendSpacE(this->GetXMLClassName());
  this->basisRationalForm.WriteToFile(output);
  output << XMLRoutines::GetCloseTagNoInputCheckAppendSpacE(this->GetXMLClassName());
}

bool Lattice::ReadFromFile(std::fstream& input, GlobalVariables* theGlobalVariables)
{ int numReadWords;
  XMLRoutines::ReadThroughFirstOpenTag(input, numReadWords, this->GetXMLClassName());
  bool result=this->basisRationalForm.ReadFromFile(input);
  this->basisRationalForm.GetMatrixIntWithDen(this->basis, this->Den);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, numReadWords, this->GetXMLClassName());
  assert(numReadWords==0);
  return result;
}

bool GeneralizedVermaModuleCharacters::ReadFromFileNoComputationPhase
  (std::fstream& input, GlobalVariables* theGlobalVariables)
{ std::string tempS;
  input >> tempS >> this->NumProcessedConesParam;
  input >> tempS >> this->NumProcessedExtremaEqualOne;
  input >> tempS;
  int numReadWords;
  if (tempS!="NormalConstAdjustment:")
  { assert(false);
    return false;
  }
  this->NonIntegralOriginModification.ReadFromFile(input);
  input >> tempS;
  this->ParabolicLeviPartRootSpacesZeroStandsForSelected.ReadFromFile(input);
  this->ParabolicSelectionSmallerAlgebra.ReadFromFile(input);
  this->WeylLarger.ReadFromFile(input);
  this->WeylSmaller.ReadFromFile(input);
  this->preferredBasiS.ReadFromFile(input, theGlobalVariables);
  this->preferredBasisChangE.ReadFromFile(input, theGlobalVariables);
  this->preferredBasisChangeInversE.ReadFromFile(input);
  this->theExtendedIntegralLatticeMatForM.ReadFromFile(input, theGlobalVariables);
  if (theGlobalVariables!=0)
    theGlobalVariables->IncrementReadWriteDepth();
  if (theGlobalVariables!=0)
    this->theParser.theHmm.MakeG2InB3(this->theParser, *theGlobalVariables);
  else
  { GlobalVariables tempGlobalVars;
    this->theParser.theHmm.MakeG2InB3(this->theParser, tempGlobalVars);
  }
  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.StatusString1="";
    theGlobalVariables->theIndicatorVariables.ProgressReportStringsNeedRefresh=true;
    theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading param subchambers cone form... ";
    theGlobalVariables->MakeReport();
  }
  this->theMaxComputation.ReadFromFile(input, theGlobalVariables, this->UpperLimitChambersForDebugPurposes);
  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading more pieces of data... ";
    theGlobalVariables->MakeReport();
  }
  this->GmodKnegativeWeightS.ReadFromFile(input, theGlobalVariables);
  this->GmodKNegWeightsBasisChanged.ReadFromFile(input, theGlobalVariables);
  this->theLinearOperators.ReadFromFile(input, theGlobalVariables);
  this->theLinearOperatorsExtended.ReadFromFile(input, theGlobalVariables);
  this->PreimageWeylChamberLargerAlgebra.ReadFromFile(input, theGlobalVariables);
  this->PreimageWeylChamberSmallerAlgebra.ReadFromFile(input, theGlobalVariables);
  this->WeylChamberSmallerAlgebra.ReadFromFile(input, theGlobalVariables);
  this->theQPsNonSubstituted.ReadFromFile(input, theGlobalVariables);
  XMLRoutines::ReadThroughFirstOpenTag(input, numReadWords, "QPsSubbed");
  this->theQPsSubstituted.ReadFromFile(input, theGlobalVariables);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, numReadWords, "QPsSubbed");
  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading multiplicities... ";
    theGlobalVariables->MakeReport();
  }
  XMLRoutines::ReadThroughFirstOpenTag(input, numReadWords, "theMultiplicities");
  this->theMultiplicities.ReadFromFile(input, theGlobalVariables, this->UpperLimitChambersForDebugPurposes);
  XMLRoutines::ReadEverythingPassedTagOpenUntilTagClose(input, numReadWords, "theMultiplicities");
  this->theCoeffs.ReadFromFile(input);
  this->theTranslationS.ReadFromFile(input, theGlobalVariables);
  this->theTranslationsProjecteD.ReadFromFile(input, theGlobalVariables);
  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading partial fractions... ";
    theGlobalVariables->MakeReport();
  }
  this->thePfs.ReadFromFile(input, theGlobalVariables);
  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading projectivized param complex... ";
    theGlobalVariables->MakeReport();
  }
  this->projectivizedParamComplex.ReadFromFile(input, theGlobalVariables, -1);
  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading the complex... ";
    theGlobalVariables->MakeReport();
  }
  this->smallerAlgebraChamber.ReadFromFile(input, theGlobalVariables, this->UpperLimitChambersForDebugPurposes);
  this->projectivizedChambeR.ReadFromFile(input, theGlobalVariables, this->UpperLimitChambersForDebugPurposes);
  if (theGlobalVariables!=0)
  { theGlobalVariables->theIndicatorVariables.ProgressReportStrings[theGlobalVariables->ProgressReportDepth]="Loading complete... ";
    theGlobalVariables->MakeReport();
    theGlobalVariables->ProgressReportDepth--;
  }
  return true;
}

void ConeComplex::WriteToFile
  (std::fstream& output, GlobalVariables* theGlobalVariables, int UpperLimit)
{ this->List<Cone>::WriteToFile(output, theGlobalVariables, UpperLimit);
  output << "IndexLowestNonRefined: " << this->indexLowestNonRefinedChamber << "\n";
  this->splittingNormals.WriteToFile(output);
}

bool ConeComplex::ReadFromFile
  (std::fstream& input, GlobalVariables* theGlobalVariables, int UpperLimitDebugPurposes)
{ if(!this->List<Cone>::ReadFromFile(input, theGlobalVariables, UpperLimitDebugPurposes))
  { assert(false);
    return false;
  }
  std::string tempS;
  input >> tempS >> this->indexLowestNonRefinedChamber;
  if (tempS!="IndexLowestNonRefined:")
  { assert(false);
    return false;
  }
  if (!this->splittingNormals.ReadFromFile(input))
  { assert(false);
    return false;
  }
  //!!!!!!!!The following needs to be fixed!
  this->slicingDirections.size=0;
  return true;
}

void WeylGroup::GetFundamentalWeightsInSimpleCoordinates(roots& output)
{ MatrixLargeRational tempMat;
  tempMat=this->CartanSymmetric;
  Rational tempRat;
  for (int i=0; i<this->GetDim(); i++)
  { tempRat=2/this->CartanSymmetric.elements[i][i];
    tempMat.RowTimesScalar(i, tempRat);
  }
  tempMat.Transpose();
  tempMat.Invert();
  output.AssignMatrixRows(tempMat);
}

void WeylGroup::GetIntegralLatticeInSimpleCoordinates(Lattice& output)
{ output.basisRationalForm=this->CartanSymmetric;
  root tempRoot;
  for (int i=0; i<this->GetDim(); i++)
  { tempRoot.MakeEi(this->GetDim(), i);
    output.basisRationalForm.RowTimesScalar(i, 2/this->RootScalarCartanRoot(tempRoot, tempRoot));
  }
  output.basisRationalForm.Transpose();
  output.basisRationalForm.Invert();
//  std::cout << output.basisRationalForm.ElementToString(true, false);
  output.MakeFromMat(output.basisRationalForm);
  output.Reduce();
}

int ParserNode::EvaluatePrintRootSystem
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ std::stringstream out;
  WeylGroup& theWeyl=theNode.owner->theHmm.theRange.theWeyl;
  out << "<br>Symmetric Cartan matrix in Bourbaki order:<br>" <<
  CGIspecificRoutines::GetHtmlMathDivFromLatexFormulaAddBeginArrayRCL(theNode.owner->theHmm.theRange.theWeyl.CartanSymmetric.ElementToString(false, true) );
  Rational tempRat;
  MatrixLargeRational tempMat;
  tempMat = theWeyl.CartanSymmetric;
  tempMat.ComputeDeterminantOverwriteMatrix(tempRat);
  out  << "<br>The determinant of the symmetric Cartan matrix is: " << tempRat.ElementToString();
  //Lattice tempLattice;
  //theWeyl.GetIntegralLatticeInSimpleCoordinates(tempLattice);
  //out << "<br>The integral lattice in simple coordinates is (generated by): " << tempLattice.ElementToString(true, false);
  //roots integralRoots, integralRootsEpsForm;
  roots fundamentalWeights, fundamentalWeightsEpsForm;
  //integralRoots.AssignMatrixRows(tempLattice.basisRationalForm);
  //theWeyl.GetEpsilonCoords(integralRoots, integralRootsEpsForm, theGlobalVariables);
  //out << "<br>The integral lattice generators in epsilon format: " << integralRootsEpsForm.ElementToStringEpsilonForm();
  theWeyl.GetFundamentalWeightsInSimpleCoordinates(fundamentalWeights);
  roots simpleBasis, simplebasisEpsCoords;
  out << "<hr>Simple basis (regular and epsilon coordinates (convention: Humphreys)): <table>";
  simpleBasis.MakeEiBasis(theWeyl.GetDim());
  theWeyl.GetEpsilonCoords(simpleBasis, simplebasisEpsCoords, theGlobalVariables);
  for (int i=0; i< simplebasisEpsCoords.size; i++)
  { out << "<tr><td>" << simpleBasis[i].ElementToString() << " </td><td>=</td> <td>"
    << CGIspecificRoutines::
    GetHtmlMathFromLatexFormulA
    (simplebasisEpsCoords[i].ElementToStringEpsilonForm(), "", "</td><td>", false, false)
    << "</td></tr>";
  }
  out << "</table>";
  out << "<hr>The fundamental weights (the j^th fundamental weight is the weight that has scalar product 1 with the j^th simple root times 2 divided by the root length squared, and 0 with the remaining simple roots): ";
  theWeyl.GetEpsilonCoords(fundamentalWeights, fundamentalWeightsEpsForm, theGlobalVariables);
  out << "<table>";
  for (int i=0; i< fundamentalWeights.size; i++)
  { out << "<tr><td>" << fundamentalWeights[i].ElementToString() << "</td><td> =</td><td> "
    << CGIspecificRoutines::
    GetHtmlMathFromLatexFormulA
    (fundamentalWeightsEpsForm[i].ElementToStringEpsilonForm(), "", "</td><td>", false, false)
    << "</td></tr>";
  }
  out << "</table>";
  out << "<hr>Root system:<table><tr><td>Simple basis coordinates</td><td></td><td>Epsilon coordinates (convention: Humphreys)</td></tr> ";
  roots rootSystemEpsCoords;
  theWeyl.GetEpsilonCoords(theWeyl.RootSystem, rootSystemEpsCoords, theGlobalVariables);
  for (int i=0; i<theWeyl.RootSystem.size; i++)
  { root& current=theNode.owner->theHmm.theRange.theWeyl.RootSystem.TheObjects[i];
    out << "<tr><td>" << current.ElementToString() << "</td><td>=</td><td>"
    << rootSystemEpsCoords[i].ElementToStringLetterFormat("e")
    << "</td></tr>";
  }
  out << "</table>";
  theNode.outputString=out.str();
  theNode.ExpressionType=theNode.typeString;
  return theNode.errorNoError;
}

template<class CoefficientType>
void Vectors<CoefficientType>::IntersectTwoLinSpaces
  (const Vectors<CoefficientType>& firstSpace, const Vectors<CoefficientType>& secondSpace,
   Vectors<CoefficientType>& output, const CoefficientType& theRingZero,
   GlobalVariables& theGlobalVariables)
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
  output.MakeActualSizeAtLeastExpandOnTop(tempSel.CardinalitySelection);
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
    output.AddObjectOnTop(nextIntersection);
  }
  //std::cout << "<br> final output: " << output.ElementToString();
  //std::cout << "<br>******************End of debugging linear space intersections";
}

int ParserNode::EvaluateIntersectLatticeWithPreimageLattice
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ roots theLinearMap;
  Lattice& firstLattice=theNode.owner->TheObjects[theArgumentList.TheObjects[0]].theLattice.GetElement();
  Lattice& secondLattice=theNode.owner->TheObjects[theArgumentList.TheObjects[1]].theLattice.GetElement();
  List<int> theMapTempList;
  theMapTempList.AddObjectOnTop(theArgumentList.TheObjects[2]);
  int theDim;
  if (!theNode.owner->TheObjects[theArgumentList.TheObjects[2]].GetRootsEqualDimNoConversionNoEmptyArgument(theMapTempList, theLinearMap, theDim))
    return theNode.SetError(theNode.errorDimensionProblem);
  MatrixLargeRational theLinearMapMat;
  theLinearMapMat.AssignRootsToRowsOfMatrix(theLinearMap);
  if(theLinearMapMat.NumCols!=firstLattice.GetDim() || theLinearMapMat.NumRows!=secondLattice.GetDim())
    return theNode.SetError(theNode.errorDimensionProblem);
  //theLinearMapMat.Transpose();

  theNode.theLattice.GetElement()=firstLattice;
  theNode.theLattice.GetElement().IntersectWithPreimageOfLattice(theLinearMapMat, secondLattice, theGlobalVariables);
  theNode.ExpressionType=theNode.typeLattice;
  return theNode.errorNoError;
}

void PolynomialsRationalCoeff::MakeLinearSubbedVarsCorrespondToMatRows(MatrixLargeRational& theMat, root& theConstants)
{ MatrixLargeRational tempMat;
  tempMat=theMat;
  tempMat.Transpose();
  tempMat.Resize(tempMat.NumRows+1, tempMat.NumCols, true);
  for (int i=0; i<theConstants.size; i++)
    tempMat.elements[tempMat.NumRows-1][i]=theConstants.TheObjects[i];
  this->MakeLinearSubConstTermsLastRow(tempMat);
}

void PolynomialsRationalCoeff::MakeLinearSubConstTermsLastRow(MatrixLargeRational& theMat)
{ this->SetSize(theMat.NumCols);
  Monomial<Rational> tempM;
  for (int i=0; i<this->size; i++)
  { this->TheObjects[i].Nullify((int)theMat.NumRows-1);
    for (int j=0; j<theMat.NumRows-1; j++)
    { tempM.init((int)theMat.NumRows-1);
      tempM.degrees[j]=1;
      tempM.Coefficient.Assign(theMat.elements[j][i]);
      this->TheObjects[i].AddMonomial(tempM);
    }
    this->TheObjects[i].AddConstant(theMat.elements[theMat.NumRows-1][i]);
  }
}

void Cone::IntersectAHyperplane
  (root& theNormal, Cone& outputConeLowerDim, GlobalVariables& theGlobalVariables)
{ assert(!theNormal.IsEqualToZero());
  int theDimension=theNormal.size;
  MatrixLargeRational tempMat, theEmbedding, theProjection;
  tempMat.AssignVectorRow(theNormal);
  roots theBasis;
  tempMat.FindZeroEigenSpace(theBasis);
  assert(theBasis.size==theNormal.size-1);
  theEmbedding.AssignRootsToRowsOfMatrix(theBasis);
  theEmbedding.Transpose();
  theBasis.AddObjectOnTop(theNormal);
  roots tempRoots, tempRoots2;
  tempRoots.MakeEiBasis(theDimension);
  tempRoots.GetCoordsInBasis(theBasis, tempRoots2, theGlobalVariables);
  theProjection.AssignRootsToRowsOfMatrix(tempRoots2);
  theProjection.Transpose();
  theProjection.Resize(theDimension-1, theDimension, false);
  roots newNormals=this->Normals;
  theProjection.ActOnRoots(newNormals);
  bool tempBool=outputConeLowerDim.CreateFromNormals(newNormals, theGlobalVariables);
  assert(!tempBool);

}

bool Cone::GetRootFromLPolyConstantTermGoesToLastVariable
(PolynomialRationalCoeff& inputLPoly, root& output)
{ if (!inputLPoly.IsLinear())
    return false;
  output.MakeZero(inputLPoly.NumVars+1);
  for (int i=0; i<inputLPoly.size; i++)
  { int theIndex;
    if (inputLPoly.TheObjects[i].::Monomial<Rational>::IsOneLetterFirstDegree(theIndex))
      output.TheObjects[theIndex]=inputLPoly.TheObjects[i].Coefficient;
    else
      *output.LastObject()=inputLPoly.TheObjects[i].Coefficient;
  }
  return true;
}

bool Cone::SolveLPolyEqualsZeroIAmProjective
  ( PolynomialRationalCoeff& inputLPoly,
   Cone& outputCone, GlobalVariables& theGlobalVariables
   )
{ root theNormal;
  if (!this->GetRootFromLPolyConstantTermGoesToLastVariable(inputLPoly, theNormal))
    return false;
  this->IntersectAHyperplane(theNormal, outputCone, theGlobalVariables);
  return true;
}

bool Cone::SolveLQuasiPolyEqualsZeroIAmProjective
  ( QuasiPolynomial& inputLQP,
   List<Cone>& outputConesOverEachLatticeShift, GlobalVariables& theGlobalVariables
   )
{ outputConesOverEachLatticeShift.SetSize(inputLQP.LatticeShifts.size);
  bool result=true;
  for (int i=0; i<inputLQP.LatticeShifts.size; i++)
    result=result && this->SolveLPolyEqualsZeroIAmProjective(inputLQP.valueOnEachLatticeShift.TheObjects[i], outputConesOverEachLatticeShift.TheObjects[i], theGlobalVariables);
  return result;
}

bool ParserFunctionArgumentTree::ConvertOneArgumentIndex
  (ParserNode& theNode, int theIndex, int& lowestNestedIndexNonExplored, GlobalVariables& theGlobalVariables)
{ int theType=this->functionArguments.TheObjects[theIndex];
  if (theType==theNode.typeDots)
  { int oldlowestNestedIndexNonExplored=lowestNestedIndexNonExplored-1;
    return this->ConvertOneArgumentIndex(theNode, theIndex-1, oldlowestNestedIndexNonExplored, theGlobalVariables);
  }
  if (theType==theNode.typeArray)
  { List<int> tempList;
    lowestNestedIndexNonExplored++;
    return this->nestedArgumentsOfArguments.TheObjects[lowestNestedIndexNonExplored-1].ConvertArguments(theNode, tempList, theGlobalVariables);
  }
  return theNode.ConvertToType(theType, theNode.impliedNumVars, theGlobalVariables);
}

bool ParserFunctionArgumentTree::ConvertArguments
  (ParserNode& theNode, List<int>& outputArgumentIndices, GlobalVariables& theGlobalVariables)
{ if (theNode.ExpressionType==theNode.typeArray)
    outputArgumentIndices=theNode.children;
  else
  { outputArgumentIndices.size=0;
    outputArgumentIndices.AddObjectOnTop(theNode.indexInOwner);
  }
  int argumentCounter=0;
  int lowestNestedIndexNonExplored=0;
  List<int> tempList;
  for (int i=0; i<this->functionArguments.size; i++)
    if (this->functionArguments.TheObjects[i]==theNode.typeDots)
      while(argumentCounter<outputArgumentIndices.size)
      { ParserNode& currentNode=theNode.owner->TheObjects[outputArgumentIndices.TheObjects[argumentCounter]];
        if (!this->ConvertOneArgumentIndex(currentNode, i, lowestNestedIndexNonExplored, theGlobalVariables))
          break;
        else
          argumentCounter++;
      }
    else
    { if (argumentCounter<outputArgumentIndices.size)
      { ParserNode& currentNode=theNode.owner->TheObjects[outputArgumentIndices.TheObjects[argumentCounter]];
        if (!this->ConvertOneArgumentIndex(currentNode, i, lowestNestedIndexNonExplored, theGlobalVariables))
          return false;
        argumentCounter++;
      }
      else
        return false;
    }
  if (argumentCounter<outputArgumentIndices.size)
    return false;
  return true;
}

int ParserFunction::CallMe
  (ParserNode& theNode, GlobalVariables& theGlobalVariables)
{ assert(this->theActualFunction!=0);
  List<int> argumentsIndices;
  if (theNode.Operation==Parser::tokenFunction)
  { assert(theNode.children.size>0);
    ParserNode& argumentNode=theNode.owner->TheObjects[theNode.children.TheObjects[0]];
    if(!this->theArguments.ConvertArguments(argumentNode, argumentsIndices, theGlobalVariables))
      return theNode.SetError(theNode.errorBadOrNoArgument);
  } else
  { assert(theNode.Operation==Parser::tokenFunctionNoArgument);
  }
  return this->theActualFunction(theNode, argumentsIndices, theGlobalVariables);
}

bool ParserFunction::MakeMe
  (const std::string& theFunctionName, const std::string& theFunctionArguments, const std::string& theFunctionDescription, const std::string& theExample, int (*inputFunctionAddress)(ParserNode& theNode, List<int>& theArguments, GlobalVariables& theGlobalVariables))
{ if(!this->theArguments.MakeFunctionArgumentFromString(theFunctionArguments))
    return false;
  this->functionDescription=theFunctionDescription;
  this->functionName=theFunctionName;
  this->theActualFunction=inputFunctionAddress;
  this->example=theExample;
  return true;
}

std::string ParserFunctionArgumentTree::GetArgumentStringFromToken(int theArgument)
{ switch(theArgument)
  { case ParserNode::typeRational: return "Rational";
    case ParserNode::typeQuasiPolynomial: return "QP";
    case ParserNode::typeIntegerOrIndex: return "Integer";
    case ParserNode::typePoly: return "Polynomial";
    case ParserNode::typeCone: return "Cone";
    case ParserNode::typeRationalFunction: return "RF";
    case ParserNode::typePartialFractions: return "PF";
    case ParserNode::typeLattice: return "Lattice";
    case ParserNode::typeDots: return "...";
    case ParserNode::typeArray: return "";
    default: return "Error";
  }
}

int ParserFunctionArgumentTree::GetTokenFromArgumentStringNoSpaces
(const std::string& theArgument)
{ if (theArgument=="Rational")
    return ParserNode::typeRational;
  if (theArgument=="QP")
    return ParserNode::typeQuasiPolynomial;
  if (theArgument=="Polynomial")
    return ParserNode::typePoly;
  if (theArgument=="Cone")
    return ParserNode::typeCone;
  if (theArgument=="Integer")
    return ParserNode::typeIntegerOrIndex;
  if (theArgument=="PF")
    return ParserNode::typePartialFractions;
  if (theArgument=="Lattice")
    return ParserNode::typeLattice;
  if (theArgument=="RF")
    return ParserNode::typeRationalFunction;
  if (theArgument=="...")
    return ParserNode::typeDots;
  return -1;
}

bool ParserFunctionArgumentTree::MakeFunctionArgumentFromString
(const std::string& theArgumentList)
{ unsigned int currentChar=0;
  return this->MakeFromString(currentChar, theArgumentList);
}

bool ParserFunctionArgumentTree::MakeFromString
(unsigned int& currentChar, const std::string& theArgumentList)
{ std::string currentArgument;
  this->functionArguments.size=0;
  this->nestedArgumentsOfArguments.size=0;
  for (; currentChar<theArgumentList.size(); currentChar++)
    if (theArgumentList[currentChar]=='(')
      break;
  currentChar++;
  for (; currentChar<theArgumentList.size(); currentChar++)
  { if (theArgumentList[currentChar]==',' || theArgumentList[currentChar]==')')
    { if (currentArgument!="")
      { this->functionArguments.AddObjectOnTop(this->GetTokenFromArgumentStringChangeInput(currentArgument));
        if (*this->functionArguments.LastObject()==-1)
          return false;
        currentArgument="";
      }
    }
    else
      currentArgument.push_back(theArgumentList[currentChar]);
    if (theArgumentList[currentChar]==')')
      return true;
    if (theArgumentList[currentChar]=='(')
    { this->functionArguments.AddObjectOnTop(ParserNode::typeArray);
      this->nestedArgumentsOfArguments.ExpandOnTop(1);
      if(!this->nestedArgumentsOfArguments.LastObject()->MakeFromString(currentChar, theArgumentList))
        return false;
      currentArgument="";
    }
  }
  return false;
}

int ParserNode::EvaluateSolveLPolyEqualsZeroOverCone
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ ParserNode& polyNode=theNode.owner->TheObjects[theArgumentList.TheObjects[0]];
  ParserNode& coneNode=theNode.owner->TheObjects[theArgumentList.TheObjects[1]];
  if (polyNode.polyValue.GetElement().NumVars!=coneNode.theCone.GetElement().GetDim()-1)
    return theNode.SetError(theNode.errorDimensionProblem);
  Cone& currentCone=coneNode.theCone.GetElement();
  currentCone.SolveLPolyEqualsZeroIAmProjective(polyNode.polyValue.GetElement(), theNode.theCone.GetElement(), theGlobalVariables);
  PolynomialOutputFormat theFormat;
  theNode.outputString=theNode.theCone.GetElement().ElementToString(false, true, theFormat);
  return theNode.errorNoError;
}

int ParserNode::EvaluateDrawConeAffine
(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ theGlobalVariables.theDrawingVariables.theBuffer.init();
  PolynomialOutputFormat theFormat;
  theNode.outputString= theNode.owner->TheObjects[theArgumentList[0]].theCone.GetElement().DrawMeToHtmlLastCoordAffine(theGlobalVariables.theDrawingVariables, theFormat);
  theNode.ExpressionType=theNode.typeString;
  return theNode.errorNoError;
}

int ParserNode::EvaluateDrawConeProjective
(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ theGlobalVariables.theDrawingVariables.theBuffer.init();
  PolynomialOutputFormat theFormat;
  theNode.outputString= theNode.owner->TheObjects[theArgumentList[0]].theCone.GetElement().DrawMeToHtmlProjective(theGlobalVariables.theDrawingVariables, theFormat);
  theNode.ExpressionType=theNode.typeString;
  return theNode.errorNoError;
}

void rootSubalgebras::MakeProgressReportGenerationSubalgebras(rootSubalgebras& bufferSAs, int RecursionDepth, GlobalVariables& theGlobalVariables, int currentIndex, int TotalIndex)
{ if (theGlobalVariables.GetFeedDataToIndicatorWindowDefault()==0)
    return;
  for (int i=0; i<=RecursionDepth; i++)
  { int lineIndex=i/3;
    theGlobalVariables.theIndicatorVariables.ProgressReportStrings.SetSize(MathRoutines::Maximum(lineIndex+1, 5));
    std::string& currentLine=theGlobalVariables.theIndicatorVariables.ProgressReportStrings[lineIndex];
    if (i%3==0)
      currentLine="";
    currentLine+= bufferSAs.TheObjects[i].theDynkinDiagram.DynkinStrinG;
    if (i!=RecursionDepth)
      currentLine+=": ";
  }
  theGlobalVariables.theIndicatorVariables.ProgressReportStrings.SetSize(MathRoutines::Maximum(theGlobalVariables.theIndicatorVariables.ProgressReportStrings.size+1, 5));
  std::stringstream out;
  out << "Included root " << currentIndex+1 << " out of " << TotalIndex << " Total found SAs: " << this->size;
  *theGlobalVariables.theIndicatorVariables.ProgressReportStrings.LastObject()=out.str();
  theGlobalVariables.FeedIndicatorWindow(theGlobalVariables.theIndicatorVariables);
}

void Parser::InitJavaScriptDisplayIndicator()
{ std::stringstream output;
  output << " <!>\n";
  output << " <script type=\"text/javascript\"> \n";
  output << " var timeOutCounter=0;\n";
  output << " window.setTimeout(\"progressReport()\",1000);\n";
  output << " var newReportString=\"\";\n";
  output << " function progressReport()\n";
  output << " { timeOutCounter++;\n";
  output << "   var oRequest = new XMLHttpRequest();\n";
  output << "   var sURL  = \"" << this->indicatorReportFileNameDisplay << "\";\n";
  output << "   oRequest.open(\"GET\",sURL,false);\n";
  output << "   oRequest.setRequestHeader(\"User-Agent\",navigator.userAgent);\n";
  output << "   oRequest.send(null)\n";
  output << "   if (oRequest.status==200)\n";
  output << "   { newReportString= oRequest.responseText;\n";
  output << "     el = document.getElementById(\"idProgressReport\").innerHTML= \"Refreshing indicator each second. Number of seconds: \"+ timeOutCounter+\"<br>Status file content:<br>\" +newReportString;\n";
  output << "   }\n";
  output << "   window.setTimeout(\"progressReport()\",1000);\n";
  output << " }\n";
  output << " </script>\n";
  output << " <div id=\"idProgressReport\">\n";
  output << " </div>\n";
  output << " \n";
  output << " \n";
  this->javaScriptDisplayingIndicator=output.str();
}

int ParserNode::EvaluatePrintRootSAsAndSlTwos
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables, bool redirectToSlTwos,
   bool forceRecompute)
{ std::stringstream outSltwoPath, outMainPath, out, outSltwoDisplayPath, outMainDisplayPath;
  CGIspecificRoutines::SetCGIServerIgnoreUserAbort();
  theGlobalVariables.MaxAllowedComputationTimeInSeconds=10000;
  char weylLetter=theNode.owner->DefaultWeylLetter;
  int theRank=theNode.owner->DefaultWeylRank;
  outMainPath << theNode.owner->outputFolderPath <<  weylLetter << theRank << "/";
  outMainDisplayPath << theNode.owner->outputFolderDisplayPath << weylLetter << theRank << "/";
  outSltwoPath << outMainPath.str() << "sl2s/";
  outSltwoDisplayPath << outMainDisplayPath.str() << "sl2s/";
  bool NeedToCreateFolders=(!CGIspecificRoutines::FileExists(outMainPath.str()) || !CGIspecificRoutines::FileExists(outSltwoPath.str()));
  if (NeedToCreateFolders)
  { std::stringstream outMkDirCommand1, outMkDirCommand2;
    outMkDirCommand1 << "mkdir " << outMainPath.str();
    outMkDirCommand2 << "mkdir " << outSltwoPath.str();
    theNode.owner->SystemCommands.AddObjectOnTop(outMkDirCommand1.str());
    theNode.owner->SystemCommands.AddObjectOnTop(outMkDirCommand2.str());
    out << "<br><br>... Created the missing folders for the database. <b> Running  a second time... Please wait for automatic redirection."
    <<  " Clicking back/refresh button in the browser will cause broken links in the calculator due to a technical "
    << "(Apache server configuration) problem.</b><br><br>"
    << "<META http-equiv=\"refresh\" content=\"3; url="
    << "/cgi-bin/calculator?"
    << " textType=" << weylLetter << "&textDim=" << theRank << "&textInput=";
    out << CGIspecificRoutines::UnCivilizeStringCGI(theNode.owner->StringBeingParsed);
    out << "\">  Redirecting in 3 seconds";
    theNode.outputString=out.str();
    theNode.ExpressionType=theNode.typeString;
    return theNode.errorNoError;
  }
  std::stringstream outRootHtmlFileName, outRootHtmlDisplayName, outSltwoMainFile, outSltwoFileDisplayName;
  outSltwoMainFile << outSltwoPath.str() << "sl2s.html";
  outSltwoFileDisplayName << outSltwoDisplayPath.str() << "sl2s.html";
  outRootHtmlFileName << outMainPath.str() << "rootHtml.html";
  outRootHtmlDisplayName << outMainDisplayPath.str() << "rootHtml.html";
  bool mustRecompute=forceRecompute;
  if (!CGIspecificRoutines::FileExists(outSltwoMainFile.str()) || !CGIspecificRoutines::FileExists(outRootHtmlFileName.str()))
    mustRecompute=true;
  bool showIndicator=theNode.owner->flagDisplayIndicator;
  if (mustRecompute)
  { if (showIndicator)
    { out << "<br>The computation is in progress. <b><br>Please do not click back/refresh button: it will cause broken links in the calculator. <br>Appologies for this technical (Apache server configuration) problem. <br>Hope to alleviate it soon.</b>"
      << "<br>Below is an indicator for the progress of the computation."
      << "<br> The computation is slow, up to around 10 minutes for E_8."
      << "<br>Once it is done, you should be redirected automatically to the result page."
      << "<br>To go back to the calculator main page use the back button on your browser.";
      std::stringstream afterSystemCommands;
      std::stringstream noindicatorLink;
      afterSystemCommands << "<META http-equiv=\"refresh\" content=\"0; url=";
      noindicatorLink << "/cgi-bin/calculator?"
      << " textType=" << weylLetter << "&textDim=" << theRank << "&textInput="
      << CGIspecificRoutines::UnCivilizeStringCGI(theNode.owner->StringBeingParsed+ " NoIndicator");
      afterSystemCommands << noindicatorLink.str() << "\">";
      theNode.owner->afterSystemCommands=afterSystemCommands.str();
      out << "\n<br>\nComputing ...<br>" << theNode.owner->javaScriptDisplayingIndicator;
      theGlobalVariables.ClearIndicatorVars();
      theGlobalVariables.MakeReport();
      theNode.outputString=out.str();
      theNode.ExpressionType=theNode.typeString;
      return theNode.errorNoError;
    } else
    { SltwoSubalgebras theSl2s;
      theSl2s.owner.FindSl2Subalgebras(theSl2s, weylLetter, theRank, theGlobalVariables);
      std::string PathSl2= outSltwoPath.str(); std::string DisplayPathSl2=outSltwoDisplayPath.str();
      theSl2s.ElementToHtml(theGlobalVariables, theSl2s.owner.theWeyl, true, PathSl2, DisplayPathSl2);
      theNode.owner->SystemCommands.AddListOnTop(theSl2s.listSystemCommandsLatex);
      theNode.owner->SystemCommands.AddListOnTop(theSl2s.listSystemCommandsDVIPNG);
    }
  }
//  std::stringstream afterSystemCommands;
  out << "<META http-equiv=\"refresh\" content=\"0; url=";
  if (!redirectToSlTwos)
    out << outRootHtmlDisplayName.str();
  else
    out << outSltwoFileDisplayName.str();
  out << "\">";
  //theNode.owner->afterSystemCommands=afterSystemCommands.str();
  theNode.outputString=out.str();
  theNode.ExpressionType=theNode.typeString;
//  theGlobalVariables.MaxAllowedComputationTimeInSeconds=oldMaxAllowedComputationTimeInSeconds;
  return theNode.errorNoError;
}

std::string Parser::GetFunctionDescription()
{ std::stringstream out;
  for (int i=0; i<this->theFunctionList.size; i++)
    if (this->theFunctionList.TheObjects[i].flagVisible)
      out << this->theFunctionList.TheObjects[i].ElementToString(true, false) << " <br>";
  return out.str();
}

std::string ParserFunctionArgumentTree::ElementToString(bool useHtml, bool useLatex)const
{ std::stringstream out;
  out << "(";
  int lowestIndexNonExplored=0;
  if (this->functionArguments.size==0)
    return "";
  for (int i=0; i<this->functionArguments.size; i++)
  { out << this->GetArgumentStringFromToken(this->functionArguments.TheObjects[i]);
    if (this->functionArguments.TheObjects[i]==ParserNode::typeArray)
    { out << this->nestedArgumentsOfArguments.TheObjects[lowestIndexNonExplored].ElementToString(useHtml, useLatex);
      lowestIndexNonExplored++;
    }
    if (i!=this->functionArguments.size-1)
      out << ", ";
  }
  out  << ")";
  return out.str();
}

bool CGIspecificRoutines::GetHtmlStringSafeishReturnFalseIfIdentical(const std::string& input, std::string& output)
{ std::stringstream out;
  bool result=false;
  for (unsigned int i=0; i<input.size(); i++)
  { if (input[i]=='<')
      out << "&lt;";
    if (input[i]=='>')
      out << "&gt;";
    if (input[i]!='<' && input[i]!='>')
      out << input[i];
    else
      result=true;
  }
  output=out.str();
  return result;
}

std::string CGIspecificRoutines::UnCivilizeStringCGI(const std::string& input)
{ std::stringstream out;
  for (unsigned int i=0; i<input.size(); i++)
    switch (input[i])
    { case ' ': out << "+"; break;
      case '+': out << "%2B"; break;
      case '(': out << "%28"; break;
      case ')': out << "%29"; break;
      case '[': out << "%5B"; break;
      case ']': out << "%5D"; break;
      case ',': out << "%2C"; break;
      case '{': out << "%7B"; break;
      case ';': out << "%3B"; break;
      case '/': out << "%2F"; break;
      case ':': out << "%3A"; break;
      case '^': out << "%5E"; break;
      case '\\': out << "%5C"; break;
      case '}': out << "%7D"; break;
      case '\n': out << "%0D%0A"; break;
//      case '': out << ""; break;
      default: out << input[i]; break;
    }
  return out.str();
}

std::string ParserFunction::ElementToString(bool useHtml, bool useLatex)const
{ std::stringstream out;
  out << "<div style=\"display: inline\" id=\"functionBox" << this->functionName << "\" >";
  out << this->functionName;
  out << this->theArguments.ElementToString(useHtml, useLatex);
  out <<  "<button" << CGIspecificRoutines::GetStyleButtonLikeHtml() << " onclick=\"switchMenu('fun" << this->functionName << "');\">More/less info</button><div id=\"fun" << this->functionName
        << "\" style=\"display: none\"><br>" << this->functionDescription << "<br>Example. <a href=\"/cgi-bin/calculator?"
        << " textType=" << this->exampleAmbientWeylLetter << "&textDim=" << this->exampleAmbientWeylRank << "&textInput="
        << CGIspecificRoutines::UnCivilizeStringCGI(this->example)
        << "\"> " << this->example << "</a></div></div>";
  return out.str();
}

std::string GeneralizedVermaModuleCharacters::PrepareReport(GlobalVariables& theGlobalVariables)
{ std::stringstream out;
  PolynomialOutputFormat theFormat;
  int tempI=0;
  root tempRoot;
  theFormat.alphabet.TheObjects[tempI]="x_1"; tempI++;
  theFormat.alphabet.TheObjects[tempI]="x_2"; tempI++;
  theFormat.alphabet.TheObjects[tempI]="y_1"; tempI++;
  theFormat.alphabet.TheObjects[tempI]="y_2"; tempI++;
  theFormat.alphabet.TheObjects[tempI]="y_3"; tempI++;
  out << "\\documentclass{article}\\usepackage{amsmath, longtable, amsfonts, amssymb, verbatim, hyperref}\n\\begin{document}\\tiny\n";
  out << "\n The chamber complex + multiplicities follow.\n\n\n \\begin{longtable}{cc}\\caption{Multiplicities of generalized Verma modules $m(x_1,x_2, y_1, y_2, y_3)$ for $\\gop$ with Dynkin diagram";
  std::stringstream tempStream;
  tempStream << "(";
  for (int i=0; i<this->ParabolicLeviPartRootSpacesZeroStandsForSelected.MaxSize; i++)
  { if (this->ParabolicLeviPartRootSpacesZeroStandsForSelected.selected[i])
      tempStream << "+";
    else
      tempStream << "0";
    if (i!=this->ParabolicLeviPartRootSpacesZeroStandsForSelected.MaxSize-1)
      tempStream << ",";
  }
  tempStream << ")";
  out << "$" << tempStream.str() <<  "$ \\label{table" << tempStream.str() << "}}\\\\\n";
  out << "Inequlities& $m(x_1,x_2, y_1, y_2, y_3)$\\endhead\n";
  int numFoundChambers=0;
  List<int> DisplayIndicesprojectivizedChambers;
  for (int i=0; i<this->projectivizedChambeR.size; i++)
  { QuasiPolynomial& theMult=this->theMultiplicities[i];
    if (!theMult.IsEqualToZero())
    { numFoundChambers++;
      out << "\\hline\\multicolumn{2}{c}{Chamber " << numFoundChambers << "}\\\\\n";
      DisplayIndicesprojectivizedChambers.AddObjectOnTop(numFoundChambers);
      out << this->PrepareReportOneCone(theFormat, this->projectivizedChambeR[i], theGlobalVariables) << "&";
      out << "\\begin{tabular}{c}";
      out << theMult.ElementToString(false, true, theFormat) << "\\end{tabular}\\\\\n";
    } else
      DisplayIndicesprojectivizedChambers.AddObjectOnTop(-1);
  }
  out << "\\end{longtable}\n\n\n Multiplicity free chambers \n";
  numFoundChambers=0;
  out << "\n\\begin{longtable}{cc} ";
  out << "Normals& Multiplicity of module with highest weight $(x_1,x_2)$\\endhead\n";
 /* for (int i=0; i<this->projectivezedChambersSplitByMultFreeWalls.size; i++)
  { tempRoot=this->projectivezedChambersSplitByMultFreeWalls.TheObjects[i].GetInternalPoint();
    bool found=false;
    for (int j=0; j<this->projectivizedChamber.size; j++)
      if (this->projectivizedChamber.TheObjects[j].IsInCone(tempRoot))
      { assert(!found);
        found=true;
      }
  }
  for (int i=0; i<this->projectivizedChamber.size; i++)
  { QuasiPolynomial& theMult=this->theMultiplicities.TheObjects[i];
    if (!theMult.IsEqualToZero())
    { int indexMultFreeChamber=-1;
      for (int j=0; j<this->projectivezedChambersSplitByMultFreeWalls.size; j++)
      { tempRoot=this->projectivezedChambersSplitByMultFreeWalls.TheObjects[j].GetInternalPoint();
        if (this->projectivizedChamber.TheObjects[i].IsInCone(tempRoot))
        { Rational tempRat;
          tempRat=*tempRoot.LastObject();
          if (tempRat!=0)
            tempRoot/=tempRat;
          theMult.valueOnEachLatticeShift.TheObjects[0].Evaluate(tempRoot, tempRat);
          if(tempRat<1)
          { indexMultFreeChamber=j;
            break;
          }
        }
      }
      if (indexMultFreeChamber!=-1)
      { numFoundChambers++;
        out << "\\hline\\multicolumn{2}{c}{Chamber " << DisplayIndicesprojectivizedChambers.TheObjects[i] << "}\\\\\n";
        out << this->PrepareReportOneCone(theFormat, this->projectivezedChambersSplitByMultFreeWalls.TheObjects[indexMultFreeChamber], theGlobalVariables) << "&";
        out << theMult.ElementToString(false, true, theFormat) << "\\\\\n";
      }
    }
  }*/
  out << "Total number of chambers with multiplicity 1 or less: " << numFoundChambers;
  out << "\\end{longtable}\n\n\n\n";
  out << "\\end{document}";
  return out.str();
}

std::string GeneralizedVermaModuleCharacters::PrepareReportOneCone
  (PolynomialOutputFormat& theFormat, Cone& theCone, GlobalVariables& theGlobalVariables)
{ std::stringstream out1;
  std::string tempS1, tempS2;
  root normalNoConstant;
  int dimSmallerAlgebra=this->theLinearOperators.TheObjects[0].NumRows;
  int dimLargerAlgebra=this->theLinearOperators.TheObjects[0].NumCols;
  Rational theConst;
  out1 << "\\begin{tabular}{rcl}";
  for (int i=0; i<theCone.Normals.size; i++)
  { root& currentNormal=theCone.Normals.TheObjects[i];
    normalNoConstant=currentNormal;
    normalNoConstant.SetSize(dimSmallerAlgebra+dimLargerAlgebra);
    theConst=-(*currentNormal.LastObject());
    if(!normalNoConstant.IsEqualToZero())
      out1 << "$" << normalNoConstant.ElementToStringLetterFormat(theFormat, true, false) << "$ & $\\geq$ & $"
               << theConst.ElementToString() << "$ \\\\";
  }
  out1 << "\\end{tabular}";
  return out1.str();
}

std::string root::ElementToStringLetterFormat(const std::string& inputLetter, bool useLatex, bool DontIncludeLastVar)
{ PolynomialOutputFormat tempFormat;
  tempFormat.MakeAlphabetArbitraryWithIndex(inputLetter, tempFormat.alphabetBases[1]);
  return this->ElementToStringLetterFormat(tempFormat, useLatex, DontIncludeLastVar);
}

std::string root::ElementToStringLetterFormat(const PolynomialOutputFormat& theFormat, bool useLatex, bool DontIncludeLastVar)
{ if (this->IsEqualToZero())
    return "0";
  std::stringstream out;
  std::string tempS;
  bool found=false;
  int NumVars= DontIncludeLastVar ? this->size-1 : this->size;
  for(int i=0; i<NumVars; i++)
    if (!this->TheObjects[i].IsEqualToZero())
    { this->TheObjects[i].ElementToString(tempS);
      if (tempS=="1")
        tempS="";
      if (tempS=="-1")
        tempS="-";
      if (found)
      { if (tempS.size()>0)
        { if (tempS[0]!='-')
            out << "+";
        } else
          out << "+";
      }
      found=true;
      out << tempS;
      out << theFormat.alphabet.TheObjects[i];
    }
  return out.str();
}

bool MatrixLargeRational::IsIdMatrix()const
{ if (this->NumRows!=this->NumCols || this->NumRows<=0)
     return false;
   for (int i=0; i<this->NumRows; i++)
     for (int j=0; j<this->NumCols; j++)
       if (i==j)
       { if (!this->elements[i][j].IsEqualToOne())
           return false;
       } else
         if (!this->elements[i][j].IsEqualToZero())
           return false;
   return true;
}

root oneFracWithMultiplicitiesAndElongations::GetCheckSumRoot(int NumVars)
{ root output;
  output.SetSize(NumVars);
  for (int i=0; i<NumVars; i++)
    output.TheObjects[i]=i+2;
  return output;
}

bool partFractions::RemoveRedundantShortRootsIndex(GlobalVariables& theGlobalVariables, root* Indicator, int theIndex)
{ if (!this->TheObjects[theIndex].rootIsInFractionCone(*this, Indicator, theGlobalVariables))
    return false;
  bool found=false;
  for (int k=0; k < this->TheObjects[theIndex].IndicesNonZeroMults.size; k++)
  { int currentIndex=this->TheObjects[theIndex].IndicesNonZeroMults.TheObjects[k];
    oneFracWithMultiplicitiesAndElongations& currentFrac = this->TheObjects[theIndex].TheObjects[currentIndex];
    if (currentFrac.Elongations.size>1)
    { found=true;
      break;
    }
  }
  if (!found)
    return false;
  partFraction& thePF= theGlobalVariables.fracRemoveRedundantRootsBuffer1;
  thePF.Assign(this->TheObjects[theIndex]);
  this->PopIndexSwapLastHashAndAccount(theIndex, theGlobalVariables, Indicator);
  Rational localStartCheckSum, localEndCheckSum;
  std::string tempS, tempS1, tempS2;
  Polynomial<LargeInt> ComputationalBufferCoefficient, tempIP;
  PolyPartFractionNumerator ComputationalBufferCoefficientNonExpanded, tempPP;
  ComputationalBufferCoefficient.AssignPolynomialLight(thePF.Coefficient);
  thePF.CoefficientNonExpanded.ComputePolyPartFractionNumerator(ComputationalBufferCoefficientNonExpanded, this->AmbientDimension);
  if (this->flagMakingProgressReport || this->flagAnErrorHasOccurredTimeToPanic)
  { //thePF.ComputeDebugString(*this, theGlobalVariables);
    //std::cout << thePF.DebugString;
    thePF.ComputeOneCheckSum(*this, localStartCheckSum, this->AmbientDimension, theGlobalVariables);
    localStartCheckSum.ElementToString(tempS2);
  }
  for (int k=0; k<thePF.IndicesNonZeroMults.size; k++)
  { int currentIndex=thePF.IndicesNonZeroMults.TheObjects[k];
    oneFracWithMultiplicitiesAndElongations& currentFrac = thePF.TheObjects[currentIndex];
    int LCMElongations = currentFrac.GetLCMElongations();
    this->RootsToIndices.TheObjects[currentIndex].ElementToString(tempS);
    while (currentFrac.Elongations.size>1)
    { for (int i=0; i<currentFrac.Elongations.size; i++)
      { int ElongationValue=currentFrac.Elongations.TheObjects[i];
        if (ElongationValue!=LCMElongations)
        { int numSummands=LCMElongations/ElongationValue;
          if (thePF.UncoveringBrackets)
          { thePF.GetNElongationPoly(*this, currentIndex, ElongationValue, numSummands, tempIP, this->AmbientDimension);
            tempIP.ComputeDebugString();
            tempIP.RaiseToPower(currentFrac.Multiplicities.TheObjects[i], (LargeInt) 1);
            tempIP.ComputeDebugString();
            ComputationalBufferCoefficient.MultiplyBy(tempIP);
          }
          else
          { PolyPartFractionNumerator tempP;
            thePF.GetNElongationPoly(*this, currentIndex, ElongationValue, numSummands, tempP, this->AmbientDimension);
            tempP.RaiseToPower(currentFrac.Multiplicities.TheObjects[i], (LargeInt) 1);
//            this->CoefficientNonExpanded.ComputeDebugString();
            ComputationalBufferCoefficientNonExpanded.MultiplyBy(tempP);
//            this->CoefficientNonExpanded.ComputeDebugString();
          }
          ComputationalBufferCoefficientNonExpanded.ComputeDebugString();
          currentFrac.AddMultiplicity(currentFrac.Multiplicities.TheObjects[i], LCMElongations);
          currentFrac.AddMultiplicity(-currentFrac.Multiplicities.TheObjects[i], ElongationValue);
          thePF.Coefficient.AssignPolynomial(ComputationalBufferCoefficient);
          thePF.ComputeOneCheckSum(*this, localEndCheckSum, this->AmbientDimension, theGlobalVariables);
          assert(localEndCheckSum.IsEqualTo(localStartCheckSum));
        }
      }
    }
    if (partFraction::MakingConsistencyCheck || this->flagAnErrorHasOccurredTimeToPanic)
    { thePF.Coefficient.AssignPolynomial(ComputationalBufferCoefficient);
      thePF.ComputeOneCheckSum(*this, localEndCheckSum, this->AmbientDimension, theGlobalVariables);
      localEndCheckSum.ElementToString(tempS1);
      assert(localStartCheckSum.IsEqualTo(localEndCheckSum));
    }
  }
  thePF.CoefficientNonExpanded.AssignPolyPartFractionNumerator(ComputationalBufferCoefficientNonExpanded);
  thePF.Coefficient.AssignPolynomial(ComputationalBufferCoefficient);
  int tempI = this->IndexOfObjectHash(thePF);
  if (tempI==-1)
    this->AddAlreadyReduced(thePF, theGlobalVariables, Indicator);
  else
  { this->AccountPartFractionInternals(-1, tempI, Indicator, theGlobalVariables);
    this->TheObjects[tempI].AddReturnShouldAttemptReduction(thePF, *this, theGlobalVariables);
    this->AccountPartFractionInternals(1, tempI, Indicator, theGlobalVariables);
  }
  return true;
}

int ParserNode::EvaluateInvariantsSl2DegreeM
(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ List<int> thePartition;
  if (theArgumentList.size!=2)
    return theNode.SetError(theNode.errorWrongNumberOfArguments);
  int theDegree=theNode.owner->TheObjects[theArgumentList.TheObjects[0]].intValue;
  if (theDegree<0)
    return theNode.SetError(theNode.errorImplicitRequirementNotSatisfied);
  ParserNode& partitionNode=theNode.owner->TheObjects[theArgumentList.TheObjects[1]];
  if (partitionNode.ExpressionType==typeArray)
  { thePartition.SetSize(partitionNode.children.size);
    for (int i=0; i<partitionNode.children.size; i++)
    { thePartition.TheObjects[i]=theNode.owner->TheObjects[partitionNode.children.TheObjects[i]].intValue;
      if (thePartition.TheObjects[i]<1)
        return theNode.SetError(theNode.errorImplicitRequirementNotSatisfied);
  //    std::cout<< thePartition.TheObjects[i] << ",";
    }
  } else
  { thePartition.SetSize(1);
    thePartition.TheObjects[0]=partitionNode.intValue;
  }
  slTwoInSlN theSl2;
  theNode.ExpressionType=theNode.typeString;
  List<PolynomialRationalCoeff> outputList;
  std::string tempS;
  if (! theSl2.ComputeInvariantsOfDegree(thePartition, theDegree, outputList, tempS, theGlobalVariables))
  { theNode.outputString=tempS;
    return theNode.SetError(theNode.errorImplicitRequirementNotSatisfied);
  }
  theNode.ExpressionType=theNode.typeString;
  std::stringstream out;
  out << "A basis for the invariants of degree " << theDegree << " is given by (number of elements: " << outputList.size << ")";
  for (int i=0; i<outputList.size; i++)
  { out << "<br>" << outputList.TheObjects[i].ElementToString() << ", ";
  }
  theNode.outputString=out.str();
  return theNode.errorNoError;
}

void MatrixLargeRational::ActOnMonomialAsDifferentialOperator(Monomial<Rational>& input, PolynomialRationalCoeff& output)
{ assert(this->NumRows==this->NumCols && this->NumRows==input.NumVariables);
  Monomial<Rational> tempMon;
  tempMon.init(tempMon.NumVariables);
  output.Nullify(tempMon.NumVariables);
  for (int i=0; i<this->NumRows; i++)
    for (int j=0; j<this->NumCols; j++)
    { tempMon=input;
      tempMon.Coefficient*=tempMon.degrees[j];
      tempMon.Coefficient*=this->elements[i][j];
      tempMon.degrees[j]--;
      tempMon.degrees[i]++;
      output.AddMonomial(tempMon);
    }
}

void LargeIntUnsigned::ElementToStringLargeElementDecimal(std::string& output)const
{ std::stringstream out;
  if (this->CarryOverBound==1000000000UL)
  { std::string tempS;
    out << (this->TheObjects[this->size-1]);
    for (int i=this->size-2; i>=0; i--)
    { std::stringstream tempStream;
      tempStream << this->TheObjects[i];
      tempS=tempStream.str();
      int numZeroesToPad=9-tempS.length();
      for (int j=0; j<numZeroesToPad; j++)
          out << "0";
      out << tempS;
    }
    output= out.str();
    return;
  }
  unsigned int base=10;
  int MaxNumIntegersPerCarryOverBound=11;
  List<LargeIntUnsigned> bufferPowersOfBase;
  int initialNumDigitsEstimate=MaxNumIntegersPerCarryOverBound*this->size;
  int sizeBufferPowersOfBase=MathRoutines::Minimum(initialNumDigitsEstimate, 10000);
  bufferPowersOfBase.SetSize(sizeBufferPowersOfBase);
  LargeIntUnsigned currentPower;
  LargeIntUnsigned Remainder=*this;
  int numRemainingDigits;
  while (!Remainder.IsEqualToZero())
  { currentPower.MakeOne();
    numRemainingDigits=0;
    int highestBufferIndex=-1;
    bufferPowersOfBase.TheObjects[0].MakeOne();
    bool bufferFilled=false;
    while (Remainder.IsGEQ(currentPower))
    { numRemainingDigits++;
      highestBufferIndex++;
      highestBufferIndex%=sizeBufferPowersOfBase;
      bufferPowersOfBase.TheObjects[highestBufferIndex]=currentPower;
      if (highestBufferIndex==sizeBufferPowersOfBase-1)
        bufferFilled=true;
      currentPower*=base;
    }
    int startIndex=highestBufferIndex;
    do
    { currentPower=bufferPowersOfBase.TheObjects[highestBufferIndex];
      unsigned int theDigit=0;
      while (Remainder.IsGEQ(currentPower))
      { theDigit++;
        currentPower+=bufferPowersOfBase.TheObjects[highestBufferIndex];
      }
      out << theDigit;
      numRemainingDigits--;
      if (theDigit!=1)
        bufferPowersOfBase.TheObjects[highestBufferIndex]*=theDigit;
      Remainder.SubtractSmallerPositive(bufferPowersOfBase.TheObjects[highestBufferIndex]);
      highestBufferIndex--;
      if (highestBufferIndex==-1)
      { highestBufferIndex=sizeBufferPowersOfBase-1;
        if (!bufferFilled)
          break;
      }
    } while (highestBufferIndex!=startIndex);
  }
  for (int i=0; i<numRemainingDigits; i++)
    out << "0";
  output=out.str();
}

int ParserNode::EvaluateFactorial
    (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ Rational tempRat;
  int theArgument=theNode.owner->TheObjects[theArgumentList.TheObjects[0]].intValue;
  if (theArgument<0)
    return theNode.SetError(theNode.errorImplicitRequirementNotSatisfied);
  if (theArgument>5000)
  { theNode.outputString="Computation too large.";
    return theNode.SetError(theNode.errorImplicitRequirementNotSatisfied);
  }
  theNode.rationalValue=tempRat.Factorial(theArgument, & theGlobalVariables);
//  theNode.ExpressionType=theNode.typeRational;
  theNode.ExpressionType=theNode.typeRational;
  //theNode.outputString=theNode.rationalValue.ElementToString();
  return theNode.errorNoError;
}

int ParserNode::EvaluatePrintAllPrimesEratosthenes
    (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ Rational tempRat;
  int theArgument=theNode.owner->TheObjects[theArgumentList.TheObjects[0]].intValue;
  if (theArgument<0)
    return theNode.SetError(theNode.errorImplicitRequirementNotSatisfied);
  if (theArgument>20000)
  { theNode.outputString="Computation too large.";
    return theNode.SetError(theNode.errorImplicitRequirementNotSatisfied);
  }
  List<unsigned int> thePrimes;
  LargeIntUnsigned::GetAllPrimesSmallerThanOrEqualToUseEratosthenesSieve(theArgument, thePrimes);
  std::stringstream out;
  for (int i=0; i<thePrimes.size; i++)
    out << thePrimes.TheObjects[i] << ", ";
  theNode.outputString=out.str();
  theNode.ExpressionType=theNode.typeString;
  return theNode.errorNoError;
}

void LargeIntUnsigned::AssignFactorial(unsigned int x, GlobalVariables* theGlobalVariables)
{ this->MakeOne();
  List<unsigned int> primesBelowX;
  LargeIntUnsigned::GetAllPrimesSmallerThanOrEqualToUseEratosthenesSieve(x, primesBelowX);
  LargeIntUnsigned tempInt, tempOne;
  tempOne.MakeOne();
  for (int i=0; i<primesBelowX.size; i++)
  { unsigned int thePrime=primesBelowX.TheObjects[i];
    unsigned int thePowerOfThePrime=0;
    unsigned int currentPower=thePrime;
    do
    { thePowerOfThePrime+=x/currentPower;
      currentPower*=thePrime;
    }
    while (currentPower<=x);
    tempInt.AssignShiftedUInt(thePrime, 0);
    MathRoutines::RaiseToPower(tempInt, thePowerOfThePrime, tempOne);
    *this*=tempInt;
/*    if (theGlobalVariables!=0)
    { std::stringstream out;
      out << "processing prime " << thePrime << " (" << i+1 << " out of " << primesBelowX.size << ").";
      theGlobalVariables->theIndicatorVariables.ProgressReportStrings[0]=out.str();
      theGlobalVariables->theIndicatorVariables.ProgressReportStringsNeedRefresh=true;
      theGlobalVariables->MakeReport();
    }*/
  }
}

void Lattice::GetRougherLatticeFromAffineHyperplaneDirectionAndLattice
  (const root& theDirection, root& outputDirectionMultipleOnLattice, root& theShift, root& theAffineHyperplane,
   roots& outputRepresentatives,
   roots& movementInDirectionPerRepresentative,
   Lattice& outputRougherLattice,
   GlobalVariables& theGlobalVariables)
{ root theNormal=theAffineHyperplane;
  theNormal.SetSize(theNormal.size-1);
  if (theDirection.ScalarEuclidean(theNormal).IsEqualToZero())
    return;
  Rational theConstOnTheOtherSide=-*theAffineHyperplane.LastObject();
  roots theBasis;
  theBasis.AssignMatrixRows(this->basisRationalForm);
  Lattice theHyperplaneLatticeNoShift, theDirectionLattice;//, normalProjectionLattice, theTrueProjectionLattice;
  roots tempRoots; //root tempRoot;
  tempRoots.AddObjectOnTop(theDirection);
  theDirectionLattice=*this;
  theDirectionLattice.IntersectWithLinearSubspaceSpannedBy(tempRoots);
  theDirectionLattice.basisRationalForm.RowToRoot(0, outputDirectionMultipleOnLattice);
  //std::cout << "<br>the normal: " << theNormal.ElementToString();
  //std::cout << "<br> the direction lattice: " << theDirectionLattice.ElementToString();
  theHyperplaneLatticeNoShift=*this;
  theHyperplaneLatticeNoShift.IntersectWithLinearSubspaceGivenByNormal(theNormal);
  //std::cout << "<br>the non-affine hyperplane intersected with the lattice: " << theHyperplaneLatticeNoShift.ElementToString();
  tempRoots.AssignMatrixRows(theHyperplaneLatticeNoShift.basisRationalForm);
  tempRoots.AddObjectOnTop(outputDirectionMultipleOnLattice);
  outputRougherLattice.MakeFromRoots(tempRoots);
  this->GetAllRepresentatives(outputRougherLattice, outputRepresentatives);
  //std::cout << "<br>the rougher lattice: " << outputRougherLattice.ElementToString();
  //std::cout << "<br>representatives of the quotient of the two lattices: " << outputRepresentatives.ElementToString();
  for (int i=0; i<outputRepresentatives.size; i++)
  { outputRepresentatives.TheObjects[i]+=theShift;
    outputRougherLattice.ReduceVector(outputRepresentatives.TheObjects[i]);
  }
  Rational theShiftedConst, unitMovement, tempRat;
  unitMovement=theNormal.ScalarEuclidean(outputDirectionMultipleOnLattice);
  movementInDirectionPerRepresentative.SetSize(outputRepresentatives.size);
  //std::cout << "<br>Affine hyperplane per representative: ";
  for (int i=0; i<outputRepresentatives.size; i++)
  { tempRat=(theNormal.ScalarEuclidean(outputRepresentatives.TheObjects[i]) - theConstOnTheOtherSide)/unitMovement;
    tempRat.AssignFracValue();
    theShiftedConst=theConstOnTheOtherSide + tempRat;
    root& currentMovement=movementInDirectionPerRepresentative.TheObjects[i];
    currentMovement=theAffineHyperplane;
    *currentMovement.LastObject()=theShiftedConst;
    //std::cout << "<br>Representative: " << outputRepresentatives.TheObjects[i].ElementToString() << " and the hyperplane: " << currentMovement.ElementToString();
  }
//  std::cout << "<hr>"
}

int ParserNode::EvaluateClosestPointToHyperplaneAlongTheNormal
(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ Lattice& currentLattice=theNode.owner->TheObjects[theArgumentList.TheObjects[3]].theLattice.GetElement();
  ParserNode& theHyperplaneNode=theNode.owner->TheObjects[theArgumentList.TheObjects[2]];
  ParserNode& theDirectionNode=theNode.owner->TheObjects[theArgumentList.TheObjects[0]];
  ParserNode& theShiftNode=theNode.owner->TheObjects[theArgumentList.TheObjects[1]];
  //ParserNode& theRayNode=theNode.owner->TheObjects[theArgumentList.TheObjects[2]];
  root theAffineNormal, theRay, theRayMultiple, theShift;//, theRay;
  if (!theHyperplaneNode.GetRootRationalDontUseForFunctionArguments(theAffineNormal, theGlobalVariables))
    return theNode.SetError(theNode.errorProgramming);
  if (!theDirectionNode.GetRootRationalDontUseForFunctionArguments(theRay, theGlobalVariables))
    return theNode.SetError(theNode.errorProgramming);
  if (!theShiftNode.GetRootRationalDontUseForFunctionArguments(theShift, theGlobalVariables))
    return theNode.SetError(theNode.errorProgramming);
//  if (!theRayNode.GetRootRational(theRay, theGlobalVariables))
//    return theNode.SetError(theNode.errorProgramming);
  //int theDim=t;
  if (theAffineNormal.size-1!=currentLattice.GetDim() || currentLattice.GetRank()!=currentLattice.GetDim() || theRay.size!=currentLattice.GetDim() || theRay.size!=theShift.size)
    return theNode.SetError(theNode.errorDimensionProblem);
  roots representatives, theMovements;
  Lattice theRougherLattice;
  currentLattice.GetRougherLatticeFromAffineHyperplaneDirectionAndLattice
  (theRay, theRayMultiple, theShift, theAffineNormal, representatives, theMovements, theRougherLattice, theGlobalVariables);
  return theNode.errorNoError;
}

bool slTwoInSlN::ComputeInvariantsOfDegree
  (List<int>& decompositionDimensions, int theDegree, List<PolynomialRationalCoeff>& output, std::string& outputError, GlobalVariables& theGlobalVariables)
{ this->initFromModuleDecomposition(decompositionDimensions, false, false);
  SelectionWithMaxMultiplicity theSel;
  theSel.initMaxMultiplicity(this->theDimension, theDegree);
  PolynomialOutputFormat PolyFormatLocal;
  outputError="";
  int numCycles=theSel.NumCombinationsOfCardinality(theDegree);
  if (numCycles<=0)
  { outputError= " Computation too large. ";
    return false;
  }
  PolynomialRationalCoeff basisMonsZeroWeight, basisMonsAll;
  basisMonsZeroWeight.Nullify(this->theDimension);
  basisMonsZeroWeight.MakeActualSizeAtLeastExpandOnTop(numCycles);
  basisMonsAll.Nullify(this->theDimension);
  basisMonsAll.MakeActualSizeAtLeastExpandOnTop(numCycles);
  Monomial<Rational> theMon;
  theMon.init(this->theDimension);
  root theWeight;
  root theCartanAction;
  theCartanAction.SetSize(this->theDimension);
  theWeight.SetSize(this->theDimension);
  for (int j=0; j<this->theDimension; j++)
    theCartanAction.TheObjects[j]=this->theH.elements[j][j];
  theSel.IncrementSubsetFixedCardinality(theDegree);
  theMon.Coefficient.MakeOne();
  for (int i=0; i<numCycles; i++, theSel.IncrementSubsetFixedCardinality(theDegree))
  { for (int j=0; j<this->theDimension; j++)
    { theMon.degrees[j]=theSel.Multiplicities.TheObjects[j];
      theWeight.TheObjects[j]=theMon.degrees[j];
    }
    basisMonsAll.AddMonomial(theMon);
    if (root::RootScalarEuclideanRoot(theWeight, theCartanAction).IsEqualToZero())
      basisMonsZeroWeight.AddMonomial(theMon);
  }
//  std::cout << "<br>Num cycles:" << numCycles << "<br>The basis mons (there are " << basisMonsZeroWeight.size << " of them): "  << basisMonsZeroWeight.ElementToString(PolyFormatLocal);
  MatrixLargeRational tempMat;
  tempMat.init(basisMonsAll.size*2, basisMonsZeroWeight.size);
//  tempMat.init(basisMonsAll.size*numGenerators, basisMonsZeroWeight.size);
  PolynomialRationalCoeff tempP;
  for (int l=0; l<2; l++)
    for (int k=0; k<basisMonsZeroWeight.size; k++)
    { if (l==0)
      { this->theE.ActOnMonomialAsDifferentialOperator(basisMonsZeroWeight.TheObjects[k], tempP);

      }
      else
        this->theF.ActOnMonomialAsDifferentialOperator(basisMonsZeroWeight.TheObjects[k], tempP);
      for (int j=0; j<basisMonsAll.size; j++)
      { int indexInResult=tempP.IndexOfObjectHash(basisMonsAll.TheObjects[j]);
        int currentRow=l*basisMonsAll.size+j;
        if (indexInResult==-1)
          tempMat.elements[currentRow][k]=0;
        else
          tempMat.elements[currentRow][k]=tempP.TheObjects[indexInResult].Coefficient;
      }
    }
//  if (tempMat.NumRows<120)
//    std::cout << "<div class=\"math\" scale=\"50\">" << tempMat.ElementToString(false, true) << "</div>";
  roots tempRoots;
  tempMat.FindZeroEigenSpace(tempRoots, theGlobalVariables);
  output.SetSize(tempRoots.size);
//  std::cout << "<br>invariants root form: " << tempRoots.ElementToString();
//  std::cout << "<br> .... and the invariants are: ";
  for (int i=0; i<output.size; i++)
  { Polynomial<Rational>& current=output.TheObjects[i];
    current.Nullify(this->theDimension);
    for (int j=0; j<basisMonsZeroWeight.size; j++)
      if (!tempRoots.TheObjects[i].TheObjects[j].IsEqualToZero())
      { theMon.Assign(basisMonsZeroWeight.TheObjects[j]);
        theMon.Coefficient=tempRoots.TheObjects[i].TheObjects[j];
        current.AddMonomial(theMon);
      }
 //   std::cout << "<br>Invariant " << i << ":<br>" << current.ElementToString(PolyFormatLocal);
  }
  return true;
}

void GeneralizedVermaModuleCharacters::InitTheMaxComputation
(GlobalVariables& theGlobalVariables)
{ this->theMaxComputation.numNonParaM=2;
  this->theMaxComputation.theConesLargerDim.MakeActualSizeAtLeastExpandOnTop(this->projectivizedChambeR.size);
  this->theMaxComputation.LPtoMaximizeLargerDim.MakeActualSizeAtLeastExpandOnTop(this->theMultiplicities.size);
  this->theMaxComputation.theConesLargerDim.SetSize(0);
  this->theMaxComputation.LPtoMaximizeLargerDim.SetSize(0);
  Lattice ZnLattice;
  int theAffineDim=5;
//  int theProjectivizedDim=theAffineDim+1;
  ZnLattice.MakeZn(theAffineDim);
  this->numNonZeroMults=0;
  theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
  ConeLatticeAndShift currentCLS;
  root theLPtoMax;
  for (int i=0; i<this->theMultiplicities.size; i++)
    if (! this->theMultiplicities[i].IsEqualToZero())
    { currentCLS.theProjectivizedCone=this->projectivizedChambeR.TheObjects[i];
      currentCLS.theShift.MakeZero(theAffineDim);
      currentCLS.theLattice=ZnLattice;
      bool tempBool= this->theMultiplicities[i].valueOnEachLatticeShift[0].GetRootFromLinPolyConstTermLastVariable(theLPtoMax, (Rational) 0);
      assert(tempBool);
      this->theMaxComputation.theConesLargerDim.AddObjectOnTop(currentCLS);
      this->theMaxComputation.LPtoMaximizeLargerDim.AddObjectOnTop(theLPtoMax);
      this->numNonZeroMults++;
      std::stringstream out;
      out << "Initialized " << i+1 << " out of " << this->theMaxComputation.theConesLargerDim.size << "; so far " << this->numNonZeroMults << " non-zero multiplicities";
      theGlobalVariables.theIndicatorVariables.StatusString1=out.str();
      theGlobalVariables.MakeReport();
    }
}

std::string DrawingVariables::GetColorHtmlFromColorIndex(int colorIndex)
{ std::stringstream out;
  int r=(colorIndex/65536)%256;
  int g=(colorIndex/256)%256;
  int b=colorIndex%256;
  out << "#";
  if (r<16)
    out << 0;
  out << std::hex  << r;
  if (g<16)
    out << 0;
  out << std::hex  << g;
  if (b<16)
    out << 0;
  out << std::hex  << b;
  return out.str();
}

std::string ConeLatticeAndShift::ElementToString(PolynomialOutputFormat& theFormat)
{ std::stringstream out;
  out << this->theProjectivizedCone.ElementToString(false, true, true, true, theFormat);
  out << "<br>Shift+lattice: " << this->theShift.ElementToString() << " + " << this->theLattice.ElementToString();
  return out.str();
}

std::string ElementSimpleLieAlgebra::ElementToStringNegativeRootSpacesFirst
  (bool useRootNotation, bool useEpsilonNotation, SemisimpleLieAlgebra& owner, PolynomialOutputFormat& thePolyFormat, GlobalVariables& theGlobalVariables)
{ std::stringstream out;
  std::string tempS;
  if (this->NonZeroElements.CardinalitySelection==0 && this->Hcomponent.IsEqualToZero())
    return "0";
  for (int i=0; i<this->NonZeroElements.CardinalitySelection; i++)
  { int theIndex=this->NonZeroElements.elements[i];
    int DisplayIndex=owner.RootIndexToDisplayIndexNegativeSpacesFirstThenCartan(theIndex);
    if (!this->coeffsRootSpaces.TheObjects[theIndex].IsNegative()&& i!=0)
      out << "+";
    tempS=this->coeffsRootSpaces.TheObjects[theIndex].ElementToString();
    if (tempS=="1")
      tempS="";
    if (tempS=="-1")
      tempS="-";
    if (!useRootNotation)
      out << tempS << thePolyFormat.alphabetBases[0] << "_{" << DisplayIndex << "}";
    else
    { out << tempS << thePolyFormat.alphabetBases[0] << "_{";
      if (useEpsilonNotation)
        out << owner.ElementToStringRootIndexToEpsForm(theIndex, theGlobalVariables);
      else
        out << owner.ElementToStringRootIndexToSimpleBasis(theIndex, theGlobalVariables);
      out << "}";
    }
  }
  if (this->Hcomponent.IsEqualToZero())
    return out.str();
  if (useRootNotation)
  { root tempRoot=this->Hcomponent;
    if (useEpsilonNotation)
      tempRoot=owner.theWeyl.GetEpsilonCoords(this->Hcomponent, theGlobalVariables);
    bool isEmpty=(this->NonZeroElements.CardinalitySelection==0);
    for (int i=0; i<tempRoot.size; i++)
      if (!tempRoot.TheObjects[i].IsEqualToZero())
      { tempRoot.TheObjects[i].ElementToString(tempS);
        if (tempS=="1")
          tempS="";
        if (tempS=="-1")
          tempS="-";
        if (!isEmpty)
        { if (tempS!="")
          { if (tempS[0]!='-')
               out << "+";
          } else
            out << "+";
        }
        out << tempS << "h_{";
        out << ((useEpsilonNotation) ? "\\varepsilon" : "\\alpha");
        out << "_{" << i+1 << "}}";
        isEmpty=false;
      }
  } else
  { tempS=this->Hcomponent.ElementToStringLetterFormat(thePolyFormat.alphabetBases[1] , true);
    if (tempS[0]!='-' && this->NonZeroElements.CardinalitySelection>0)
      out << "+";
    out << tempS;
  }
  return out.str();
}

void SemisimpleLieAlgebra::ElementToStringNegativeRootSpacesFirst
  (std::string& output, bool useRootNotation, bool useEpsilonNotation, bool useHtml, bool useLatex, bool usePNG,
   PolynomialOutputFormat& theFormat, GlobalVariables& theGlobalVariables)
{ std::stringstream out;
  std::string tempS;
  root tempRoot;
  std::string beginMath;
  std::string endMath;
  if (!usePNG)
  { beginMath="<DIV class=\"math\" scale=\"50\">\\begin{array}{";
    endMath="\\end{array}</DIV>";
  } else
  { beginMath="\\begin{tabular}{";
    endMath="\\end{tabular}";
  }
  int numRoots=this->theWeyl.RootSystem.size;
  int numPosRoots=this->theWeyl.RootsOfBorel.size;
  int theDimension = this->theWeyl.CartanSymmetric.NumRows;
  ElementSimpleLieAlgebra tempElt1, tempElt2, tempElt3;
//  out << beginMath << "\\begin{array}{ccc}a& a&a\\\\a&a&a\\end{array}";
  if (usePNG)
  { out << "\\begin{tabular}{cc}";
    out << "\\hline generator &corresponding root space\\\\\\hline";
    for (int i=0; i<numRoots+theDimension; i++)
    { if (i==numPosRoots)
      { if (useRootNotation)
          out << "\\hline\\begin{tabular}{c}$"
          << theFormat.alphabetBases[1] << "_\\alpha$:=$\\frac{\\langle\\alpha,\\alpha\\rangle}{2}["
          << theFormat.alphabetBases[0] << "_{\\alpha},"
          << theFormat.alphabetBases[0] << "_{-\\alpha}]$\\\\$"
          << theFormat.alphabetBases[1] << "_\\alpha$ is dual to the root $\\alpha$\\end{tabular} & 0 \\\\\\hline";
        else
          out << "\\hline\\begin{tabular}{c}$"
          << theFormat.alphabetBases[1] << "_i$:=$\\frac{\\langle\\alpha_i,\\alpha_i\\rangle}{2}["
          << theFormat.alphabetBases[0] << "_{i},"
          << theFormat.alphabetBases[0] << "_{-i}]$\\\\$"
          << theFormat.alphabetBases[1] << "_i$ is dual to the root $\\alpha_i$\\end{tabular} & 0 \\\\\\hline";
//        out << "  \\\\\\hline";

        //out << "\\hline generator & corresponding root space\\\\\\hline";
        i+=theDimension;
      }
      out << "$" << this->GetLetterFromGeneratorIndex(i, false, theFormat);
      int rootIndex=this->ChevalleyGeneratorIndexToRootIndex(i);
      if (useRootNotation)
      { out << ":=" << theFormat.alphabetBases[0] << "_{";
        if (useEpsilonNotation)
          out << this->ElementToStringRootIndexToEpsForm(rootIndex, theGlobalVariables);
        else
          out << this->ElementToStringRootIndexToSimpleBasis(rootIndex, theGlobalVariables);
        out<< "}";
      }
      out << "$&";
      if (!useEpsilonNotation)
        out << this->theWeyl.RootSystem.TheObjects[rootIndex].ElementToString() << "\\\\";
      else
        out << "$" << this->theWeyl.GetEpsilonCoords(this->theWeyl.RootSystem.TheObjects[rootIndex], theGlobalVariables).ElementToStringEpsilonForm() << "$\\\\";
    }
    out << "\\end{tabular}";
  }
  if (numRoots+theDimension<100)
  { for (int i=0; i<numRoots+theDimension+1; i++)
      beginMath+="c";
    beginMath+="}\n";
    out << beginMath;
    std::string tempHeader=out.str();
    if(usePNG)
      out << "$";
    out << "[\\bullet, \\bullet]\n";
    if(usePNG)
      out << "$";
    for (int i=0; i<numRoots+theDimension; i++)
    { tempElt1.AssignChevalleyGeneratorCoeffOneIndexNegativeRootspacesFirstThenCartanThenPositivE(i, *this);
      tempS=tempElt1.ElementToStringNegativeRootSpacesFirst
      (useRootNotation, useEpsilonNotation, *this, theFormat, theGlobalVariables);
      out << " & ";
      if(usePNG)
        out << "$";
      out << tempS;
      if(usePNG)
        out << "$";
    }
    out << "\\\\\n";
    Rational tempRat;
    //int lineCounter=0;
    for (int i=0; i<theDimension+numRoots; i++)
    { tempElt1.AssignChevalleyGeneratorCoeffOneIndexNegativeRootspacesFirstThenCartanThenPositivE(i,*this);
      tempS=tempElt1.ElementToStringNegativeRootSpacesFirst
      (useRootNotation, useEpsilonNotation, *this, theFormat, theGlobalVariables);
      if(usePNG)
        out << "$";
      out << tempS;
      if(usePNG)
        out << "$";
      for (int j=0; j<numRoots+theDimension; j++)
      { tempElt2.AssignChevalleyGeneratorCoeffOneIndexNegativeRootspacesFirstThenCartanThenPositivE(j, *this);
        this->LieBracket(tempElt1, tempElt2, tempElt3);
        tempS=tempElt3.ElementToStringNegativeRootSpacesFirst
          (useRootNotation, useEpsilonNotation, *this, theFormat, theGlobalVariables);
        out << "& ";
        if(usePNG)
          out << "$";
        out << tempS;
        if(usePNG)
          out << "$";
      }
      out << "\\\\\n";
    }
    out << endMath;
  } else
    out << "\\begin{tabular}{c} table trimmed to avoid LaTeX/browser memory issues.\\end{tabular}";

  output=out.str();
}

void ElementSimpleLieAlgebra::ElementToString(std::string& output, bool useHtml, bool useLatex, bool usePNG, std::string* physicalPath, std::string* htmlPathServer)const
{ std::stringstream out; std::string tempS;
  if (this->IsEqualToZero())
    out << "0";
  if (useLatex)
    out << "$";
  for (int i=0; i<this->NonZeroElements.CardinalitySelection; i++)
  { this->coeffsRootSpaces.TheObjects[this->NonZeroElements.elements[i]].ElementToString(tempS);
    if (tempS=="1")
      tempS="";
    if (tempS=="-1")
      tempS="-";
    if (i!=0)
    { if (tempS!="")
      { if (tempS[0]!='-')
          out << "+";
      } else
        out << "+";
    }
    out << tempS << "g_{" << this->NonZeroElements.elements[i]+1 << "}";
  }
  for (int i=0; i<this->Hcomponent.size; i++)
    if (!this->Hcomponent.TheObjects[i].IsEqualToZero())
    { this->Hcomponent.TheObjects[i].ElementToString(tempS);
      if (tempS=="1")
        tempS="";
      if (tempS=="-1")
        tempS="-";
      if (tempS!="")
      { if (tempS[0]!='-')
          out << "+";
      } else
        out << "+";
      out << tempS << "h_{\\alpha_{" << i+1 << "}}";
    }
  if(useLatex)
    out << "$";
  output= out.str();
}

void DrawOperations::MakeMeAStandardBasis(int theDim)
{ if (theDim<1)
    return;
  if (theDim>3)
  { this->ProjectionsEiVectors.SetSizeMakeMatrix(theDim, 2);
    for (int i=0; i<theDim; i++)
    { this->ProjectionsEiVectors[i][0]=sin((double)i/(double)theDim* MathRoutines::Pi());
      this->ProjectionsEiVectors[i][1]=cos((double)i/(double)theDim* MathRoutines::Pi());
    }
  }
  else
  { this->ProjectionsEiVectors.SetSizeMakeMatrix(3, 2);
    this->ProjectionsEiVectors[0][0]=1;
    this->ProjectionsEiVectors[0][1]=0;
    this->ProjectionsEiVectors[1][0]=0;
    this->ProjectionsEiVectors[1][1]=-1;
    this->ProjectionsEiVectors[2][0]=.7;
    this->ProjectionsEiVectors[2][1]=-0.4;
    this->ProjectionsEiVectors.SetSize(theDim);
  }
}

std::string DrawingVariables::GetHtmlFromDrawOperationsCreateDivWithUniqueName(int theDimension)
{ std::stringstream out, tempStream1, tempStream2, tempStream3, tempStream4, tempStream5, tempStream6;
  std::stringstream tempStream7, tempStream8, tempStream9, tempStream10, tempStream11;
  this->NumHtmlGraphics++;
  int timesCalled=this->NumHtmlGraphics;
  tempStream1 << "drawConeInit" << timesCalled;
  std::string theInitFunctionName= tempStream1.str();
  tempStream5 << "drawAll" << timesCalled;
  std::string theDrawFunctionName= tempStream5.str();
  tempStream2 << "idCanvasCone" << timesCalled;
  std::string theCanvasId= tempStream2.str();
  tempStream3 << "surf" << timesCalled;
  std::string theSurfaceName=tempStream3.str();
  tempStream4 << "pts1" << timesCalled;
  std::string Points1ArrayName=tempStream4.str();
  tempStream10 << "pts2" << timesCalled;
  std::string Points2ArrayName=tempStream10.str();
  tempStream11<< "circ" << timesCalled;
  std::string circArrayName=tempStream11.str();
  tempStream6 << "basisCone" << timesCalled;
  std::string basisName = tempStream6.str();
  tempStream7 << "shiftXCone" << timesCalled;
  std::string shiftX=tempStream7.str();
  tempStream8 << "shiftYCone" << timesCalled;
  std::string shiftY=tempStream8.str();
  tempStream9 << "convXY" << timesCalled;
  std::string functionConvertToXYName=tempStream9.str();

  out << "<div style=\"width:" << this->DefaultHtmlWidth << ";height:" << this->DefaultHtmlHeight << ";border:solid 1px\" id=\"" << theCanvasId
  << "\" onmousedown=\"clickCanvasCone" << timesCalled << "(event.clientX, event.clientY);\" onmouseup=\"selectedBasisIndexCone" << timesCalled
  << "=-1;\" onmousemove=\"mouseMoveRedrawCone" <<  timesCalled << "(event.clientX, event.clientY);\" "
  << "onmousewheel=\"mouseHandleWheelCone" << timesCalled << "(event);\""
  << "></div><br>" << CGIspecificRoutines::GetHtmlButton("button"+theCanvasId, theDrawFunctionName+"();", "redraw");
  out << "<script type=\"text/javascript\">\n"
  << "var " << Points1ArrayName << "=new Array(" << this->theBuffer.theDrawLineBetweenTwoRootsOperations.size << ");\n"
  << "var " << Points2ArrayName << "=new Array(" << this->theBuffer.theDrawLineBetweenTwoRootsOperations.size << ");\n"
  << "var " << circArrayName << "=new Array(" << this->theBuffer.theDrawCircleAtVectorOperations.size << ");\n";
  for (int i=0; i<this->theBuffer.theDrawLineBetweenTwoRootsOperations.size; i++)
  { Vector<double>& current1=theBuffer.theDrawLineBetweenTwoRootsOperations[i].v1;
    Vector<double>& current2=theBuffer.theDrawLineBetweenTwoRootsOperations[i].v2;
    out << Points1ArrayName << "[" << i << "]=[";
    for (int j=0; j<theDimension; j++)
    { out << current1[j];
      if (j!=theDimension-1)
        out << ",";
    }
    out << "];\n";
    out << Points2ArrayName << "[" << i << "]=[";
    for (int j=0; j<theDimension; j++)
    { out << current2[j];
      if (j!=theDimension-1)
        out << ",";
    }
    out << "];\n";
  }
  for (int i=0; i<this->theBuffer.theDrawCircleAtVectorOperations.size; i++)
  { Vector<double>& current1=theBuffer.theDrawCircleAtVectorOperations[i].theVector;
    out << circArrayName << "[" << i << "]=[";
    for (int j=0; j<theDimension; j++)
    { out << current1[j];
      if (j!=theDimension-1)
        out << ",";
    }
    out << "];\n";
  }
  if (this->theBuffer.ProjectionsEiVectors.size!= theDimension)
    this->theBuffer.MakeMeAStandardBasis(theDimension);
  out  << "var " << basisName << "=new Array(" << theDimension << ");\n";
  for (int i=0; i<theDimension; i++)
    out << basisName << "[" << i << "]=[" << this->theBuffer.ProjectionsEiVectors[i][0] << "," << this->theBuffer.ProjectionsEiVectors[i][1] << "];\n";
  out << "var " << shiftX << "=" <<
  this->theBuffer.centerX[0]
  //100
  << ";\n";
  out << "var " <<  shiftY << "=" <<
  this->theBuffer.centerY[0]
  //100
  << ";\n";
  out << "var GraphicsUnitCone" << timesCalled << "=" << this->theBuffer.GraphicsUnit[0] << ";\n";
  out << "function " << functionConvertToXYName << "(vector){\n";
  out << "resultX=" << shiftX << "; resultY=" << shiftY << ";\nfor (i=0; i<" << theDimension << "; i++){\n";
  out << "resultX+=vector[i]*" << basisName << "[i][0]*GraphicsUnitCone" << timesCalled << ";\n";
  out << "resultY+=vector[i]*" << basisName << "[i][1]*GraphicsUnitCone" << timesCalled << ";\n}\n";
  out << "result=[resultX, resultY];\n";
  out << "return result;\n";
  out << "}\n";
  out << "var " << theSurfaceName << ";\n"
  << "function " << theDrawFunctionName << "(){\n"
  << theSurfaceName << ".clear();\n";
  for (int i=0; i<this->theBuffer.IndexNthDrawOperation.size; i++)
  { int currentIndex=this->theBuffer.IndexNthDrawOperation[i];
    switch(theBuffer.TypeNthDrawOperation[i])
    { case DrawOperations::typeDrawLineBetweenTwoVectors:
        out << theSurfaceName << ".createLine({"
              << " x1 :" << functionConvertToXYName << "( " << Points1ArrayName << "["
              << currentIndex<< "])[0],"
              << " y1 :" << functionConvertToXYName << "( " << Points1ArrayName << "["
              << currentIndex << "])[1],"
              << " x2 :"  << functionConvertToXYName << "( " << Points2ArrayName << "["
              << currentIndex << "])[0],"
              << " y2 :" << functionConvertToXYName << "( " << Points2ArrayName << "["
              << currentIndex << "])[1] }).setStroke({color : \""
              << this->GetColorHtmlFromColorIndex(this->theBuffer.theDrawLineBetweenTwoRootsOperations[currentIndex].ColorIndex)
              << "\"});\n";
        break;
      case DrawOperations::typeDrawCircleAtVector:
        out << theSurfaceName << ".createEllipse({"
              << " cx :" << functionConvertToXYName << "( " << circArrayName << "["
              << currentIndex<< "])[0],"
              << " cy :" << functionConvertToXYName << "( " << circArrayName << "["
              << currentIndex << "])[1],"
              << " rx : "  <<  this->theBuffer.theDrawCircleAtVectorOperations[currentIndex].radius << ","
              << " ry :" << this->theBuffer.theDrawCircleAtVectorOperations[currentIndex].radius
              << " }).setStroke({color : \""
              << this->GetColorHtmlFromColorIndex(this->theBuffer.theDrawCircleAtVectorOperations[currentIndex].ColorIndex)
              << "\"});\n";
        break;
      default: break;
    }
  }
  out << "}\n"
  << "function " << theInitFunctionName << "(){\n"
  << "node = dojo.byId(\"" << theCanvasId << "\");\n"
  << theSurfaceName << "  = dojox.gfx.createSurface(node," << this->DefaultHtmlWidth << "," << this->DefaultHtmlHeight << ");\n"
  << theDrawFunctionName << "();\n"
  << " }\n";
  out << "var selectedBasisIndexCone" << timesCalled << "=-1;\n"
  << "var clickTolerance=5;\n"
  << "function ptsWithinClickToleranceCone" << timesCalled << "(x1, y1, x2, y2)\n"
  << "{ if (x1-x2>clickTolerance || x2-x1>clickTolerance || y1-y2>clickTolerance || y2-y1>clickTolerance )\n    return false;\n  return true;\n}";
  out << "\nfunction clickCanvasCone" << timesCalled << "(cx,cy)\n"
  << "{ divPosX=0;\n  divPosY=0;\n  thePointer= document.getElementById(\"idCanvasCone" << timesCalled << "\");\n"
  << "  while(thePointer)  {\n  divPosX += thePointer.offsetLeft;\n  divPosY += thePointer.offsetTop;\n  thePointer = thePointer.offsetParent;\n  }"
  << "\n  posx=(cx-divPosX+document.body.scrollLeft-" << shiftX << ");"
  << "\n  posy=(cy-divPosY+document.body.scrollTop-" << shiftY << ");\n  selectedBasisIndexCone" << timesCalled <<"=-1;\n"
  << "if (ptsWithinClickToleranceCone" << timesCalled << "(posx,posy,0,0))" << "\nselectedBasisIndexCone" << timesCalled << "=-2;\n"
  <<  "for (i=0;i<3;i++)  {\n if (ptsWithinClickToleranceCone" << timesCalled
  << "(posx, posy, basisCone" << timesCalled << "[i][0]*GraphicsUnitCone" << timesCalled << ", basisCone"
  << timesCalled << "[i][1]*GraphicsUnitCone" << timesCalled << "))\n"
  << "  selectedBasisIndexCone" << timesCalled << "=i;  \n}\n}\nfunction mouseMoveRedrawCone" << timesCalled << "(cx, cy)\n"
  << "{ if (selectedBasisIndexCone" << timesCalled << "==-1)\n    return;\n  divPosX=0;\n  divPosY=0;\n"
  << "  thePointer= document.getElementById(\"idCanvasCone"<< timesCalled << "\");\n  while(thePointer)\n  { divPosX += thePointer.offsetLeft;\n"
  << "    divPosY += thePointer.offsetTop;\n    thePointer = thePointer.offsetParent;\n  }\n"
  << "  posx=(cx-divPosX+document.body.scrollLeft-" << shiftX << ");\n"
  << "  posy=-(cy-divPosY+document.body.scrollTop-" << shiftY << ");\n"
  << "if (selectedBasisIndexCone" << timesCalled << "==-2)\n{ shiftXCone"<< timesCalled << "=(cx-divPosX+document.body.scrollLeft);\n"
  << shiftY << "=(cy-divPosY+document.body.scrollTop);\n  }  else\n"
  << "{ basisCone" << timesCalled << "[selectedBasisIndexCone" << timesCalled << "][0]=posx/GraphicsUnitCone" << timesCalled << ";\n"
  <<  "basisCone" << timesCalled << "[selectedBasisIndexCone" << timesCalled << "][1]=-posy/GraphicsUnitCone" << timesCalled << ";\n  }\n  "
  << theDrawFunctionName << " ();\n}\n";
  out << "function mouseHandleWheelCone" << timesCalled << "(theEvent){\n"
  << "theEvent = theEvent ? theEvent : window.event;\n theEvent.preventDefault();\ntheEvent.stopPropagation();\ntheWheelDelta = theEvent.detail ? theEvent.detail * -1 : theEvent.wheelDelta / 40;\n"
  << "GraphicsUnitCone" << timesCalled << "+=theWheelDelta;\n"
  << "if (GraphicsUnitCone" << timesCalled << "==0)\n GraphicsUnitCone" << timesCalled << "=3;\n"
  << "for (i=0; i<" << theDimension << "; i++){\n "
  << basisName << "[i][0]*=GraphicsUnitCone" << timesCalled << "/(GraphicsUnitCone" << timesCalled << "-theWheelDelta);\n"
  << basisName << "[i][1]*=GraphicsUnitCone" << timesCalled << "/(GraphicsUnitCone" << timesCalled << "-theWheelDelta);\n"
  << "}\n"
  << theDrawFunctionName << "();\n}\n";

  out  << ""//"dojo.require(\"dojo.gfx\");"
  << " dojo.addOnLoad(" << theInitFunctionName << "); "
  << "</script>";
  return out.str();
}

std::string ConeComplex::DrawMeToHtmlLastCoordAffine
(DrawingVariables& theDrawingVariables, PolynomialOutputFormat& theFormat)
{ bool isBad=false;
  isBad=this->DrawMeLastCoordAffine(theDrawingVariables, theFormat);
  std::stringstream out;
  out << theDrawingVariables.GetHtmlFromDrawOperationsCreateDivWithUniqueName(this->GetDim()-1);
  if (isBad)
    out << "<hr>" << "found cones which I can't draw<hr>";
  out << this->ElementToString(false, true);
/*  for (int i=0; i<this->size; i++)
  { theDrawingVariables.theBuffer.init();
    out <<"<hr>" << this->TheObjects[i].DrawMeToHtmlLastCoordAffine(theDrawingVariables, theFormat);
  }*/
  return out.str();
}

std::string ConeComplex::DrawMeToHtmlProjective
(DrawingVariables& theDrawingVariables, PolynomialOutputFormat& theFormat)
{ bool isGood=true;
  isGood=this->DrawMeProjective(0, true, theDrawingVariables, theFormat);
  std::stringstream out;
  out << theDrawingVariables.GetHtmlFromDrawOperationsCreateDivWithUniqueName(this->GetDim());
  if (!isGood)
    out << "<hr>" << "found cones which I can't draw<hr>";
  out << this->ElementToString(false, true);
/*  for (int i=0; i<this->size; i++)
  { theDrawingVariables.theBuffer.init();
    out <<"<hr>" << this->TheObjects[i].DrawMeToHtmlLastCoordAffine(theDrawingVariables, theFormat);
  }*/
  return out.str();
}

bool ConeComplex::DrawMeLastCoordAffine
(DrawingVariables& theDrawingVariables, PolynomialOutputFormat& theFormat)
{ bool result=true;
  for (int i=0; i<this->size; i++)
  { //theDrawingVariables.theBuffer.init();
    result=this->TheObjects[i].DrawMeLastCoordAffine(theDrawingVariables, theFormat) && result;
    //std::cout <<"<hr> drawing number " << i+1 << ": " << theDrawingVariables.GetHtmlFromDrawOperationsCreateDivWithUniqueName(this->GetDim()-1);
  }
  return result;
}

bool ConeComplex::DrawMeProjective
(root* coordCenterTranslation, bool InitDrawVars, DrawingVariables& theDrawingVariables, PolynomialOutputFormat& theFormat)
{ bool result=true;
  if (InitDrawVars)
  { root tempRoot, coordCenter, ZeroRoot;
    ZeroRoot.MakeZero(this->GetDim());
    if (coordCenterTranslation==0)
      coordCenter=ZeroRoot;
    else
      coordCenter=*coordCenterTranslation;
    theDrawingVariables.theBuffer.MakeMeAStandardBasis(this->GetDim());
    for (int i=0; i<this->GetDim(); i++)
    { tempRoot.MakeEi(this->GetDim(), i);
      theDrawingVariables.drawLineBetweenTwoVectorsBuffer
      (ZeroRoot+coordCenter, tempRoot+coordCenter, theDrawingVariables.PenStyleNormal, CGIspecificRoutines::RedGreenBlue(205,205,205));
    }
  }
  for (int i=0; i<this->size; i++)
  { //theDrawingVariables.theBuffer.init();
    result=(this->TheObjects[i].DrawMeProjective(coordCenterTranslation, false, theDrawingVariables, theFormat) && result);
    //std::cout <<"<hr> drawing number " << i+1 << ": " << theDrawingVariables.GetHtmlFromDrawOperationsCreateDivWithUniqueName(this->GetDim()-1);
  }
  return result;
}

bool Cone::DrawMeLastCoordAffine
(DrawingVariables& theDrawingVariables, PolynomialOutputFormat& theFormat)
{ root ZeroRoot;
  ZeroRoot.MakeZero(this->GetDim()-1);
  theDrawingVariables.theBuffer.MakeMeAStandardBasis(this->GetDim()-1);
  roots VerticesScaled=this->Vertices;
  Rational tempRat;
  List<bool> DrawVertex;
  DrawVertex.initFillInObject(this->Vertices.size, true);
  bool foundBadVertex=false;
  for (int i=0; i<VerticesScaled.size; i++)
  { tempRat=*VerticesScaled[i].LastObject();
    VerticesScaled[i].SetSize(this->GetDim()-1);
    if (tempRat.IsPositive())
      VerticesScaled[i]/=tempRat;
    if (tempRat.IsEqualToZero())
      VerticesScaled[i]*=10000;
    if (tempRat.IsNegative())
    { DrawVertex[i]=false;
      foundBadVertex=true;
    }
  }
  root tempRoot;
  for (int i=0; i<this->GetDim()-1; i++)
  { tempRoot.MakeEi(this->GetDim()-1, i);
    theDrawingVariables.drawLineBetweenTwoVectorsBuffer
    (ZeroRoot, tempRoot, theDrawingVariables.PenStyleNormal, CGIspecificRoutines::RedGreenBlue(205,205,205));
  }
  for (int k=0; k<this->Normals.size; k++)
    for (int i=0; i<this->Vertices.size; i++)
      if (DrawVertex[i] && this->Normals[k].ScalarEuclidean(this->Vertices[i]).IsEqualToZero())
        for (int j=i+1; j<this->Vertices.size; j++)
          if(DrawVertex[j] && this->Normals[k].ScalarEuclidean(this->Vertices[j]).IsEqualToZero())
            theDrawingVariables.drawLineBetweenTwoVectorsBuffer
            (VerticesScaled[i], VerticesScaled[j], theDrawingVariables.PenStyleNormal, CGIspecificRoutines::RedGreenBlue(0,0,0));
  return foundBadVertex;
}

std::string Cone::DrawMeToHtmlLastCoordAffine
(DrawingVariables& theDrawingVariables, PolynomialOutputFormat& theFormat)
{ if (this->flagIsTheZeroCone)
    return "The cone is empty.";
  if (this->Normals.size<1)
    return "The cone is the entire space";
  if (this->Vertices.size<1)
    return "The cone is empty";
  std::stringstream out;
  bool foundBadVertex=this->DrawMeLastCoordAffine(theDrawingVariables, theFormat);
  if (foundBadVertex)
    out << "<br>The cone does not lie in the upper half-space. ";
  else
    out << theDrawingVariables.GetHtmlFromDrawOperationsCreateDivWithUniqueName(this->GetDim()-1);
  out << "<br>" << this->ElementToString(false, true, true, true, theFormat);
  return out.str();
}

bool Cone::DrawMeProjective
(root* coordCenterTranslation, bool initTheDrawVars, DrawingVariables& theDrawingVariables, PolynomialOutputFormat& theFormat)
{ root ZeroRoot, coordCenter;
  ZeroRoot.MakeZero(this->GetDim());
  if (coordCenterTranslation==0)
    coordCenter=ZeroRoot;
  else
    coordCenter=*coordCenterTranslation;
//  theDrawingVariables.theBuffer.init();
  roots VerticesScaled=this->Vertices;
  for (int i=0; i<VerticesScaled.size; i++)
  { Rational sumAbsValuesCoords=0;
    for (int j=0; j<this->GetDim(); j++)
      sumAbsValuesCoords+=(VerticesScaled[i][j].IsPositive()) ? VerticesScaled[i][j] : -VerticesScaled[i][j];
    assert(!sumAbsValuesCoords.IsEqualToZero());
    VerticesScaled[i]/=sumAbsValuesCoords;
  }
  root tempRoot;
  if (initTheDrawVars)
  { theDrawingVariables.theBuffer.MakeMeAStandardBasis(this->GetDim());
    for (int i=0; i<this->GetDim(); i++)
    { tempRoot.MakeEi(this->GetDim(), i);
      theDrawingVariables.drawLineBetweenTwoVectorsBuffer
      (ZeroRoot+coordCenter, tempRoot+coordCenter, theDrawingVariables.PenStyleNormal, CGIspecificRoutines::RedGreenBlue(205,205,205));
    }
  }
  for (int i=0; i<this->Vertices.size; i++)
    theDrawingVariables.drawLineBetweenTwoVectorsBuffer
    (ZeroRoot+coordCenter, VerticesScaled[i]*10000+coordCenter, theDrawingVariables.PenStyleNormal, CGIspecificRoutines::RedGreenBlue(180,180,180));
  for (int k=0; k<this->Normals.size; k++)
    for (int i=0; i<this->Vertices.size; i++)
      if (this->Normals[k].ScalarEuclidean(this->Vertices[i]).IsEqualToZero())
        for (int j=i+1; j<this->Vertices.size; j++)
          if(this->Normals[k].ScalarEuclidean(this->Vertices[j]).IsEqualToZero())
            theDrawingVariables.drawLineBetweenTwoVectorsBuffer
            (VerticesScaled[i]+coordCenter, VerticesScaled[j]+coordCenter, theDrawingVariables.PenStyleNormal, CGIspecificRoutines::RedGreenBlue(0,0,0));
  return true;
}

std::string Cone::DrawMeToHtmlProjective(DrawingVariables& theDrawingVariables, PolynomialOutputFormat& theFormat)
{ if (this->flagIsTheZeroCone)
    return "The cone is the zero cone (i.e. contains only the origin).";
  if (this->Normals.size<=0)
    return "The cone is the entire space.";
  std::stringstream out;
  if (this->Vertices.size<1)
  { out << "There has been a programming error. The cone is empty.<br>" << this->ElementToString(false, true, true, false, theFormat);
    return out.str();
  }
  this->DrawMeProjective(0, true, theDrawingVariables, theFormat);
  out << theDrawingVariables.GetHtmlFromDrawOperationsCreateDivWithUniqueName(this->GetDim());
  out << "<br>" << this->ElementToString(false, true, true, false, theFormat);
  return out.str();
}

std::string CGIspecificRoutines::GetHtmlButton
(const std::string& buttonID, const std::string& theScript, const std::string& buttonText)
{ std::stringstream out;
  out << "\n<button id=\"" << buttonID << "\" " << CGIspecificRoutines::GetStyleButtonLikeHtml()
    << " onclick=\"" << theScript << "\">" << buttonText << "</button>";
  return out.str();
}

std::string CGIspecificRoutines::GetHtmlMathFromLatexFormulA
  (const std::string& input, const std::string& prependString, const std::string& appendStringBeforeButton, bool useDiv, bool useBeginArrayRCL)
{ std::stringstream out;
  CGIspecificRoutines::GlobalFormulaIdentifier++;
  out << prependString;
  if (useDiv)
    out << "<div";
  else
    out << "<span";
  out << " id=\"theResult" << CGIspecificRoutines::GlobalFormulaIdentifier << "\" class=\"math\" scale=\"50\">";
  if (useBeginArrayRCL)
    out << "\\begin{array}{rcl}&&\n";
  out << input;
  if (useBeginArrayRCL)
    out << "\n\\end{array}";
  if (useDiv)
    out << "</div><br>";
  else
    out << "</span>";
  out << "<textarea id=\"theResultLatex" << CGIspecificRoutines::GlobalFormulaIdentifier << "\" style=\"display: none\">";
  if (useBeginArrayRCL)
    out << "\\begin{array}{rcl}&&\n";
  out << input ;
  if (useBeginArrayRCL)
    out << "\n\\end{array}";
  out << "</textarea>";
  if (useDiv)
    out << "\n<br>";
  out <<appendStringBeforeButton;
  out << "\n<button id=\"ButtonToggleLatex"  << CGIspecificRoutines::GlobalFormulaIdentifier
        << " \" " << CGIspecificRoutines::GetStyleButtonLikeHtml() << " onclick=\"switchMenu('theResult" << GlobalFormulaIdentifier
        << "'); switchMenu('theResultLatex" << CGIspecificRoutines::GlobalFormulaIdentifier << "');\"\">LaTeX show/hide</button>";
  return out.str();
}

int DrawOperations::GetDimFirstDimensionDependentOperation()
{ if (this->theDrawLineBetweenTwoRootsOperations.size>0)
    return this->theDrawLineBetweenTwoRootsOperations[0].v1.size;
  if (this->theDrawTextAtVectorOperations.size>0)
    return this->theDrawTextAtVectorOperations[0].theVector.size;
  return 2;
}

int DrawOperations::GetDimFromBilinearForm()
{ return this->theBilinearForm.NumRows;
}

void DrawOperations::initDimensions(int theDim, int numAnimationFrames)
{ this->theBilinearForm.MakeIdMatrix(theDim, 1, 0);
  this->ProjectionsEiVectors.SetSizeMakeMatrix(theDim, 2);
  Vectors<double> tempBasis;
  tempBasis.SetSizeMakeMatrix(2, theDim);
  this->BasisProjectionPlane.initFillInObject(numAnimationFrames, tempBasis);
  this->BasisToDrawCirclesAt.MakeEiBasis(theDim, 1, 0);
  this->SelectedPlane=0;
  this->SelectedCircleMinus2noneMinus1Center=-2;
  this->centerX.initFillInObject(numAnimationFrames, 300);
  this->centerY.initFillInObject(numAnimationFrames, 300);
  this->GraphicsUnit.initFillInObject(numAnimationFrames, DrawOperations::GraphicsUnitDefault);
}

void DrawOperations::EnsureProperInitialization()
{ int theDim=this->GetDimFirstDimensionDependentOperation();
  bool isGood=(this->ProjectionsEiVectors.size==theDim && this->theBilinearForm.NumRows==theDim);
  int numFrames=this->BasisProjectionPlane.size;
  if (this->SelectedPlane>= this->BasisProjectionPlane.size)
  { numFrames=this->SelectedPlane+1;
    isGood=false;
  }
  if (isGood)
    isGood=(this->BasisProjectionPlane[this->SelectedPlane].size==2);
  if (isGood)
    isGood=(this->BasisProjectionPlane[this->SelectedPlane][0].size==theDim);
  if (!isGood)
    this->initDimensions(theDim, numFrames);
}

void WeylGroup::GetLongestWeylElt(ElementWeylGroup& outputWeylElt)
{ root lowest;
  this->ComputeRho(false);
//  std::cout << "rho: " << this->rho.ElementToString() << "<hr>";
  this->GetLowestElementInOrbit(this->rho, lowest, outputWeylElt, false, false);
  //std::stringstream out;
  //out << outputWeylElt;
//  out << "\n<br>";
  //MatrixLargeRational tempMat;
  //this->GetMatrixOfElement(outputWeylElt, tempMat);
  //out << tempMat.ElementToString(true, false);
  //std::cout << out.str();
  //std::cout << outputWeylElt;
 //std::cout << this->GetMatrixOfElement(
}

void WeylGroup::GetLowestElementInOrbit
  (root & input, root& output, ElementWeylGroup& outputWeylElt, bool RhoAction, bool UseMinusRho)
{ assert(& input!=&output);
  output=input;
  roots eiBasis;
  eiBasis.MakeEiBasis(this->GetDim());
  outputWeylElt.size=0;
  for (bool found = true; found; )
  { found=false;
    for (int i=0; i<this->GetDim(); i++)
      if (this->RootScalarCartanRoot(output, eiBasis[i]).IsPositive())
      { found=true;
        this->SimpleReflection<Rational>(i, output, RhoAction, UseMinusRho, (Rational) 0);
        outputWeylElt.AddObjectOnTop(i);
      }
  }
}

bool WeylGroup::IsEigenSpaceGeneratorCoxeterElement(root& input)
{ ElementWeylGroup tempElt;
  this->GetCoxeterElement(tempElt);
  MatrixLargeRational matCoxeterElt;
  this->GetMatrixOfElement(tempElt, matCoxeterElt);
  root tempRoot=input;
  for (int i=0; i<this->GetDim(); i++)
    matCoxeterElt.ActOnAroot(tempRoot);
  return tempRoot==input;
}

void MatrixLargeRational::ApproximateLargestEigenSpace
  (roots& outputBasis, const Rational& DesiredError, int SuggestedOrder, int numIterations)
{ assert(false);
  /*assert(this->NumRows==this->NumCols && this->NumRows>0);
  outputBasis.SetSize(2);
  outputBasis[0].MakeEi(this->NumCols,0);
  outputBasis[1]=outputBasis[0];
  root* r1=&outputBasis[0];
  root* r2=&outputBasis[1];
  if (numIterations<=0)
    numIterations=1000;
  //if (SuggestedOrder<1)
  for (int i=0; i< numIterations; i++)
  { this->ActOnAroot(*r1, *r2);
    if (!r2->IsEqualToZero())
      (*r2)/=r2->ScalarEuclidean(*r2);
    MathRoutines::swap(r1, r2);
  }*/
}



template<class Base>
std::iostream& operator<< (std::iostream& output, const CompleX<Base>& input)
{ if (input.IsEqualToZero())
  { output << "0";
    return output;
  }
  if (input.Re!=0)
  { output << input.Re;
    if (input.Im!=0)
      output << " + ";
  }
  if (input.Im!=0)
    output << "i";
  if (input.Im<0)
    output << "(";
  if (input.Im!=1)
    output << input.Im;
  if (input.Im<0)
    output << ")";
  return output;
}

int ParserNode::EvaluateDrawRootSystem
  (ParserNode& theNode, char theWeylLetter, int theDimension, GlobalVariables& theGlobalVariables, root* bluePoint)
{ WeylGroup theWeyl;
  theWeyl.MakeArbitrary(theWeylLetter, theDimension);
  theWeyl.ComputeRho(true);
  root ZeroRoot;
  theNode.impliedNumVars=theDimension;
  ZeroRoot.MakeZero(theDimension);
  ElementWeylGroup tempElt;
  theWeyl.GetCoxeterElement(tempElt);
  MatrixLargeRational matCoxeterElt, tempMat;
  theWeyl.GetMatrixOfElement(tempElt, matCoxeterElt);
  std::cout << matCoxeterElt.ElementToString(true, false);
  tempMat=matCoxeterElt;
  int coxeterNumber=theWeyl.RootSystem.LastObject()->SumCoordinates().NumShort+1;
  for (int i=0; i<coxeterNumber-1; i++)
    tempMat.MultiplyOnTheLeft(matCoxeterElt);
//  std::cout << "<br>coxeter transformation to the power of " << coxeterNumber << " equals: " << tempMat.ElementToString(true, false);
  CompleX<double> theEigenValue;
  theEigenValue.Re= cos(2*MathRoutines::Pi()/coxeterNumber);
  theEigenValue.Im= sin(2*MathRoutines::Pi()/coxeterNumber);
  Matrix<CompleX<double> > eigenMat, idMat;
  eigenMat.init(matCoxeterElt.NumRows, matCoxeterElt.NumCols);
  for (int i =0; i<eigenMat.NumRows; i++)
    for (int j=0; j<eigenMat.NumCols; j++)
    { eigenMat.elements[i][j]=matCoxeterElt.elements[i][j].DoubleValue();
      if (i==j)
        eigenMat.elements[i][i]-=theEigenValue;
    }
  List<List<CompleX<double> > > theEigenSpaceList;
  eigenMat.FindZeroEigenSpacE(theEigenSpaceList, (CompleX<double>) 1, (CompleX<double>) -1, (CompleX<double>) 0, theGlobalVariables);
  Vectors<CompleX<double> > theEigenSpace;
  DrawOperations theDrawOperators;
  theDrawOperators.init();
  theDrawOperators.initDimensions(theDimension, 1);
  theDrawOperators.GraphicsUnit[0]=DrawOperations::GraphicsUnitDefault;
  theEigenSpace.operator=(theEigenSpaceList);
  for (int i=0; i<theDimension; i++)
    for (int j=0; j<theDimension; j++)
      theDrawOperators.theBilinearForm.elements[i][j]=theWeyl.CartanSymmetric.elements[i][j].DoubleValue();
  Vector<double> tempRoot;
  theDrawOperators.SelectedPlane=0;
  Vectors<double>& theTwoPlane= theDrawOperators.BasisProjectionPlane[0];
  assert(theTwoPlane.size==2);
  if (theEigenSpace.size>0)
  { for (int j=0; j<theDimension; j++)
    { theTwoPlane[0][j]=theEigenSpace[0][j].Re;
      theTwoPlane[1][j]=theEigenSpace[0][j].Im;
    }
    theDrawOperators.ModifyToOrthonormalNoShiftSecond(theDrawOperators.BasisProjectionPlane[0][0], theDrawOperators.BasisProjectionPlane[0][1]);
  }
//  std::cout << "<hr><hr>the eigenspace: " << theEigenSpace.ElementToString(false, true, false);
//  std::stringstream tempStream;
//  tempStream << "<hr>the eigen mat:";
//  tempStream << eigenMat;
//  std::cout << tempStream.str();
  roots RootSystemSorted;
  RootSystemSorted.CopyFromBase(theWeyl.RootSystem);
  List<double> lengths;
  lengths.SetSize(RootSystemSorted.size);
  for (int i=0; i<theWeyl.RootSystem.size; i++)
  { tempRoot.SetSize(theDimension);
    for (int j=0; j<theDimension; j++)
      tempRoot[j]=theWeyl.RootSystem[i][j].DoubleValue();
    double Length1 = theWeyl.RootScalarCartanRoot(tempRoot, theDrawOperators.BasisProjectionPlane[0][0]);
    double Length2 = theWeyl.RootScalarCartanRoot(tempRoot, theDrawOperators.BasisProjectionPlane[0][1]);
    lengths[i]=sqrt(Length1*Length1+Length2*Length2);
  }
  for (int i=0; i<RootSystemSorted.size; i++)
    for (int j=i; j<RootSystemSorted.size; j++)
      if (lengths[i]<lengths[j])
      { MathRoutines::swap(lengths[i], lengths[j]);
        MathRoutines::swap(RootSystemSorted[i], RootSystemSorted[j]);
      }
  root differenceRoot;
  differenceRoot=RootSystemSorted[0]-RootSystemSorted[1];
  Rational minLength= theWeyl.RootScalarCartanRoot(differenceRoot, differenceRoot);
  for (int i=2; i<RootSystemSorted.size; i++)
  { differenceRoot=RootSystemSorted[0]-RootSystemSorted[i];
    if (minLength> theWeyl.RootScalarCartanRoot(differenceRoot, differenceRoot))
      minLength=theWeyl.RootScalarCartanRoot(differenceRoot, differenceRoot);
  }
  std::cout << "<hr>the min length is: " << minLength.ElementToString();
  Rational tempRat;
  if (bluePoint!=0)
  { theDrawOperators.drawCircleAtVectorBuffer(*bluePoint, 5, DrawingVariables::PenStyleNormal, CGIspecificRoutines::RedGreenBlue(0,0,255));
    theDrawOperators.drawCircleAtVectorBuffer(*bluePoint, 4, DrawingVariables::PenStyleNormal, CGIspecificRoutines::RedGreenBlue(0,0,255));
    theDrawOperators.drawCircleAtVectorBuffer(*bluePoint, 3, DrawingVariables::PenStyleNormal, CGIspecificRoutines::RedGreenBlue(0,0,255));
  }
  theGlobalVariables.theDrawingVariables.DefaultHtmlHeight=600;
  theGlobalVariables.theDrawingVariables.DefaultHtmlWidth=600;
  theDrawOperators.centerX[0]=300;
  theDrawOperators.centerY[0]=300;
  for (int i=0; i<RootSystemSorted.size; i++)
  { int color=CGIspecificRoutines::RedGreenBlue(0, 255, 0);
    theDrawOperators.drawLineBetweenTwoVectorsBuffer(ZeroRoot, RootSystemSorted[i], DrawingVariables::PenStyleNormal, color);
    theDrawOperators.drawCircleAtVectorBuffer(RootSystemSorted[i], 2, DrawingVariables::PenStyleNormal, CGIspecificRoutines::RedGreenBlue(255,0,0));
    for (int j=i+1; j<RootSystemSorted.size; j++)
    { differenceRoot=RootSystemSorted[i]-RootSystemSorted[j];
      tempRat=theWeyl.RootScalarCartanRoot(differenceRoot, differenceRoot);
      if (minLength== tempRat)
        theDrawOperators.drawLineBetweenTwoVectorsBuffer(RootSystemSorted[i], RootSystemSorted[j], DrawingVariables::PenStyleNormal, CGIspecificRoutines::RedGreenBlue(0, 0, 255));
    }
  }
  root tempRootRat;
  for (int i=0; i<theDimension; i++)
  { tempRootRat.MakeEi(theDimension, i);
    theDrawOperators.drawCircleAtVectorBuffer(tempRootRat, 1, DrawingVariables::PenStyleNormal, CGIspecificRoutines::RedGreenBlue(255,0,0));
  }
  std::stringstream tempStream;
  tempStream << theWeyl.WeylLetter << theWeyl.GetDim() << " (" << SemisimpleLieAlgebra::GetLieAlgebraName(theWeyl.WeylLetter, theWeyl.GetDim()) << ")";
  theDrawOperators.drawTextBuffer(0, 0, tempStream.str(), 10, CGIspecificRoutines::RedGreenBlue(0,0,0), DrawingVariables::TextStyleNormal);
  theGlobalVariables.theDrawingVariables.theBuffer=theDrawOperators;
  theGlobalVariables.theDrawingVariables.theBuffer.ComputeProjectionsEiVectors();
  theNode.outputString = theGlobalVariables.theDrawingVariables.GetHtmlFromDrawOperationsCreateDivWithUniqueName(theDimension);
  theNode.outputString+="\n<br>\nReference: John Stembridge, <a href=\"http://www.math.lsa.umich.edu/~jrs/coxplane.html\">http://www.math.lsa.umich.edu/~jrs/coxplane.html</a>.";
  theNode.theAnimation.GetElement().MakeZero();
  theNode.theAnimation.GetElement()+=theDrawOperators;
  theNode.ExpressionType=theNode.typeAnimation;
  theNode.theAnimation.GetElement().flagAnimating=true;
  theNode.theAnimation.GetElement().flagIsPausedWhileAnimating=true;
  return theNode.errorNoError;

}

int ParserNode::EvaluateAnimateRootSystem
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables, root* bluePoint)
{ char theWeylLetter= (char)theNode.owner->TheObjects[theArgumentList[0]].intValue;
  int theDim= theNode.owner->TheObjects[theArgumentList[1]].intValue;
  theWeylLetter+='A';
  CGIspecificRoutines::MakeSureWeylGroupIsSane(theWeylLetter, theDim);
  int NumFrames=theNode.owner->TheObjects[theArgumentList[2]].intValue;
  if(NumFrames<1)
    NumFrames=1;
  int result=ParserNode::EvaluateDrawRootSystem(theNode, theArgumentList, theGlobalVariables, bluePoint);
  if (result==theNode.typeError)
    return result;
  theNode.ExpressionType=theNode.typeAnimation;
  DrawOperations theOps;
  theOps=theNode.theAnimation.GetElement().thePhysicalDrawOps[0];
  theOps.flagAnimatingMovingCoordSystem=true;
  Vectors<double> theFirstBasis=theOps.BasisProjectionPlane[0];
  Matrix<double> theForm=theOps.theBilinearForm;
  Vectors<double> theDraggableBasis=theOps.BasisToDrawCirclesAt;
  theOps.initDimensions(theForm, theDraggableBasis, theFirstBasis, NumFrames);
  Vector<double> target1, target2, start1, start2;
  target1.MakeEi(theDim, 0, 1, 0);
  target2.MakeEi(theDim, ((theDim>2)? 2 : 1), 1, 0);
  start1=theOps.BasisProjectionPlane[0][0];
  start2=theOps.BasisProjectionPlane[0][1];
  theOps.ModifyToOrthonormalNoShiftSecond(target1, target2);
  int& indexBitMap=theOps.SelectedPlane;
  for (indexBitMap=0; indexBitMap<NumFrames; indexBitMap++)
  { double fraction=0;
    if (NumFrames>1)
      fraction=((double) indexBitMap)/((double)(NumFrames-1));
    theOps.BasisProjectionPlane[indexBitMap][0]=start1*(1-fraction);
    theOps.BasisProjectionPlane[indexBitMap][1]=start2*(1-fraction);
    theOps.BasisProjectionPlane[indexBitMap][0]+=target1*fraction;
    theOps.BasisProjectionPlane[indexBitMap][1]+=target2*fraction;
    theOps.ModifyToOrthonormalNoShiftSecond(theOps.BasisProjectionPlane[indexBitMap][0], theOps.BasisProjectionPlane[indexBitMap][1]);
  }
  indexBitMap=0;
  theOps.drawTextBuffer(0, 15, "center can be moved", 0, 10, DrawingVariables::TextStyleNormal);
  theOps.drawTextBuffer(0, 30, "basis(=darker red dots) can be rotated", 0, 10, DrawingVariables::TextStyleNormal);
  theNode.theAnimation.GetElement().MakeZero();
  theNode.theAnimation.GetElement()+=theOps;
  theNode.theAnimation.GetElement().flagAnimating=true;
  theNode.theAnimation.GetElement().indexVirtualOp=0;
  return result;
}

void AnimationBuffer::operator=(const AnimationBuffer& other)
{ this->stillFrame=other.stillFrame;
  this->thePhysicalDrawOps=other.thePhysicalDrawOps;
  this->theVirtualOpS=other.theVirtualOpS;
  this->flagAnimating=other.flagAnimating;
  this->flagIsPausedWhileAnimating=other.flagIsPausedWhileAnimating;
  this->indexVirtualOp=other.indexVirtualOp;
  assert(this->indexVirtualOp<this->theVirtualOpS.size);

}

AnimationBuffer::AnimationBuffer()
{ this->stillFrame.init();
  this->MakeZero();
}

void AnimationBuffer::operator+=(const DrawOperations& other)
{ VirtualDrawOp theOp;
  theOp.theVirtualOp=this->typeDrawOps;
  theOp.indexPhysicalDrawOp=this->thePhysicalDrawOps.size;
  this->thePhysicalDrawOps.AddObjectOnTop(other);
  theOp.indexPhysicalFrame=0;
  if (this->theVirtualOpS.size>0)
    theOp.indexPhysicalFrame=this->theVirtualOpS.LastObject()->indexPhysicalFrame+1;
  theOp.selectedPlaneInPhysicalDrawOp=0;
  this->theVirtualOpS.MakeActualSizeAtLeastExpandOnTop(this->theVirtualOpS.size+other.BasisProjectionPlane.size);
  for (int i=0; i<other.BasisProjectionPlane.size; i++)
  { this->theVirtualOpS.AddObjectOnTop(theOp);
    theOp.indexPhysicalFrame++;
    theOp.selectedPlaneInPhysicalDrawOp++;
  }
}

void AnimationBuffer::AddPause(int numFrames)
{ VirtualDrawOp theVOp;
  theVOp.indexPhysicalFrame=this->GetNumPhysicalFramesNoStillFrame()-1;
  theVOp.selectedPlaneInPhysicalDrawOp=-1;
  theVOp.theVirtualOp=this->typePause;
  theVOp.indexPhysicalDrawOp=this->thePhysicalDrawOps.size-1;
  this->theVirtualOpS.MakeActualSizeAtLeastExpandOnTop(this->theVirtualOpS.size+numFrames);
  for (int i=0; i<numFrames; i++)
    this->theVirtualOpS.AddObjectOnTop(theVOp);
}

void AnimationBuffer::MakeZero()
{ this->theVirtualOpS.size=0;
  this->thePhysicalDrawOps.size=0;
  this->indexVirtualOp=-1;
  this->flagAnimating=false;
  this->flagIsPausedWhileAnimating=false;
}

std::string VirtualDrawOp::ElementToString()
{ std::stringstream out;
  switch(this->theVirtualOp)
  { case AnimationBuffer::typeDrawOps:
      out << "draw operations; ";
      break;
    case AnimationBuffer::typeClearScreen:
      out << "clear screen;";
      break;
    case AnimationBuffer::typePause:
      out << "pause;";
      break;
    default:
      out << "type of draw function not documented";
      break;
  }
  out << " draw op: " << this->indexPhysicalDrawOp;
  out << " sel. plane: " << this->selectedPlaneInPhysicalDrawOp;
  out << " phys. frame index: " << this->indexPhysicalFrame;
  return out.str();
}

std::string AnimationBuffer::ElementToString()
{ std::stringstream out;
  for (int i=0; i<this->theVirtualOpS.size; i++)
    out << "Frame " << i << ": " << this->theVirtualOpS[i].ElementToString() << "\n";
  return out.str();
}

void AnimationBuffer::operator+=(const AnimationBuffer& other)
{ if (other.theVirtualOpS.size<=0)
    return;
  int physicalOpShift=this->thePhysicalDrawOps.size;
  int physicalFrameShift=0;
  if (this->theVirtualOpS.size>0)
    physicalFrameShift=this->theVirtualOpS.LastObject()->indexPhysicalFrame+1;
  this->thePhysicalDrawOps.MakeActualSizeAtLeastExpandOnTop(this->thePhysicalDrawOps.size+other.thePhysicalDrawOps.size);
  for (int i=0; i<other.thePhysicalDrawOps.size; i++)
    this->thePhysicalDrawOps.AddObjectOnTop(other.thePhysicalDrawOps[i]);
  this->theVirtualOpS.MakeActualSizeAtLeastExpandOnTop(this->theVirtualOpS.size+other.theVirtualOpS.size);
  VirtualDrawOp currentOp;
  for (int i=0; i<other.theVirtualOpS.size; i++)
  { currentOp=other.theVirtualOpS[i];
    currentOp.indexPhysicalDrawOp+=physicalOpShift;
    currentOp.indexPhysicalFrame+=physicalFrameShift;
    this->theVirtualOpS.AddObjectOnTop(currentOp);
  }
  if (this->thePhysicalDrawOps.size>0)
    this->indexVirtualOp=0;
  if (other.flagAnimating)
    this->flagAnimating=true;
}

int AnimationBuffer::GetIndexCurrentDrawOps()
{ if (!this->flagAnimating || this->thePhysicalDrawOps.size==0 || this->indexVirtualOp<0)
    return -1;
  assert(this->indexVirtualOp<this->theVirtualOpS.size);
  return this->theVirtualOpS[this->indexVirtualOp].indexPhysicalDrawOp;
}

DrawOperations& AnimationBuffer::GetCurrentDrawOps()
{ int theIndex=this->GetIndexCurrentDrawOps();
  if (theIndex==-1)
  { return this->stillFrame;
  }
  DrawOperations& result=this->thePhysicalDrawOps[theIndex];
  result.SelectedPlane=this->theVirtualOpS[this->indexVirtualOp].selectedPlaneInPhysicalDrawOp;
  if (result.SelectedPlane<0 || result.BasisProjectionPlane.size<=result.SelectedPlane)
    result.SelectedPlane=result.BasisProjectionPlane.size-1;
  return result;
}

int AnimationBuffer::GetNumPhysicalFramesNoStillFrame()
{ if (this->theVirtualOpS.size<=0)
    return 0;
  int result = this->theVirtualOpS.LastObject()->indexPhysicalFrame+1;
  if (result<=0)
    result=1;
  return result;
}

bool AnimationBuffer::IncrementOpReturnNeedsRedraw()
{ if (!this->flagAnimating)
    return false;
  if (this->theVirtualOpS.size<=0)
    return false;
  int oldPhysicalFrame=this->GetIndexCurrentPhysicalFrame();
  this->indexVirtualOp=(this->indexVirtualOp+1)% this->theVirtualOpS.size;
  return (oldPhysicalFrame!=this->GetIndexCurrentPhysicalFrame());
}

int AnimationBuffer::GetIndexCurrentPhysicalFrame()
{ if (this->theVirtualOpS.size<=0 || this->indexVirtualOp<0)
    return 0;
  assert(this->indexVirtualOp<this->theVirtualOpS.size);
  int result=this->theVirtualOpS[this->indexVirtualOp].indexPhysicalFrame;
  if (result<0)
    result=0;
  return result;
}

void AnimationBuffer::DrawNoInit(DrawingVariables& theDrawingVariables, GlobalVariables& theGlobalVariables)
{ theGlobalVariables.theIndicatorVariables.ProgressReportStringsNeedRefresh=true;
  int indexCurrentFrame=-2;
  int numTotalPhysicalFrames=this->GetNumPhysicalFramesNoStillFrame();
  for (this->indexVirtualOp=0;  this->indexVirtualOp<this->theVirtualOpS.size; this->indexVirtualOp++)
    if (this->GetIndexCurrentPhysicalFrame()!=indexCurrentFrame)
    { indexCurrentFrame=this->GetIndexCurrentPhysicalFrame();
      std::stringstream tempStream;
      tempStream << "Computing frame " << indexCurrentFrame+1 << " out of " << numTotalPhysicalFrames << " physical frames.";
      theGlobalVariables.theIndicatorVariables.ProgressReportStrings[0]=tempStream.str();
      theDrawingVariables.drawBufferNoIniT(this->GetCurrentDrawOps());
    }
  this->indexVirtualOp=0;
}

int ParserNode::EvaluateAnimateRootSystemBluePoint
(ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ root tempRoot;
  ParserNode& aNode=theNode.owner->TheObjects[theArgumentList[3]];
  aNode.GetRootRationalDontUseForFunctionArguments(tempRoot, theGlobalVariables);
  return theNode.EvaluateAnimateRootSystem(theNode, theArgumentList, theGlobalVariables, & tempRoot);
}

int ParserNode::EvaluateAnimationPause
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ int NumFrames=theNode.owner->TheObjects[theArgumentList[0]].intValue;
  if(NumFrames<0)
    NumFrames=0;
  theNode.ExpressionType=theNode.typeAnimation;
  theNode.theAnimation.GetElement().MakeZero();
  theNode.theAnimation.GetElement().AddPause(NumFrames);
  return theNode.errorNoError;
}

int ParserNode::EvaluateAnimationDots
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ char theWeylLetter= (char)theNode.owner->TheObjects[theArgumentList[0]].intValue;
  int theDim= theNode.owner->TheObjects[theArgumentList[1]].intValue;
  theWeylLetter+='A';
  CGIspecificRoutines::MakeSureWeylGroupIsSane(theWeylLetter, theDim);
  WeylGroup theWeyl;
  theWeyl.MakeArbitrary('A', 2);
  DrawOperations theDrawOps;
  Vectors<double> theFirstBasis;
  theDim=2;
  theFirstBasis.MakeEiBasis(theDim, 1, 0);
  Vectors<double> theDraggableBasis;
  theDraggableBasis.MakeEiBasis(theDim, 1, 0);
  theDrawOps.init();
  theDrawOps.initDimensions(theWeyl.CartanSymmetric, theDraggableBasis, theFirstBasis, 1);
  theDrawOps.flagAnimatingMovingCoordSystem=true;
  ParserNode& weightNode=theNode.owner->TheObjects[theArgumentList[3]];
  roots dots;
  int thenewDim;
  if (!weightNode.GetRootsEqualDimNoConversionNoEmptyArgument(weightNode.children, dots, thenewDim))
    return theNode.SetError(errorDimensionProblem);
  if (thenewDim!=theDim)
    return theNode.SetError(errorDimensionProblem);
  LargeIntUnsigned tempInt;
  Rational tempRat=theNode.owner->TheObjects[theArgumentList[2]].rationalValue;
  tempRat.GetNumUnsigned(tempInt);
  int theColor=tempInt.TheObjects[0];
//  for(int i=0; i<theArgumentList.size-3; i++)
//    extraDot[i]= theNode.owner->TheObjects[theArgumentList[i+3]].rationalValue;
  dots.MakeEiBasis(2);
  for (int i=0; i<dots.size;i++)
    theDrawOps.drawCircleAtVectorBuffer(dots[i], 2, DrawingVariables::PenStyleNormal, theColor);
  theDrawOps.SelectedPlane=0;
  theDrawOps.GraphicsUnit=10;
  theNode.ExpressionType=theNode.typeAnimation;
  theNode.theAnimation.GetElement().MakeZero();
  theNode.theAnimation.GetElement()+=(theDrawOps);
  theNode.theAnimation.GetElement().flagAnimating=true;
  theNode.ExpressionType=theNode.typeAnimation;

  return theNode.errorNoError;
}

int ParserNode::EvaluateAnimationClearScreen
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ char theWeylLetter= (char)theNode.owner->TheObjects[theArgumentList[0]].intValue;
  int theDim= theNode.owner->TheObjects[theArgumentList[1]].intValue;
  theWeylLetter+='A';
  CGIspecificRoutines::MakeSureWeylGroupIsSane(theWeylLetter, theDim);
  WeylGroup theWeyl;
  theWeyl.MakeArbitrary(theWeylLetter, theDim);
  theWeyl.ComputeRho(true);
  DrawOperations theOps;
  Vectors<double> theFirstBasis;
  theFirstBasis.MakeEiBasis(theDim, 1, 0);
  Vectors<double> theDraggableBasis;
  theDraggableBasis.MakeEiBasis(theDim, 1, 0);
  theOps.init();
  theOps.initDimensions(theWeyl.CartanSymmetric, theDraggableBasis, theFirstBasis, 1);
  theOps.flagAnimatingMovingCoordSystem=true;

  theNode.ExpressionType=theNode.typeAnimation;
  theNode.theAnimation.GetElement().MakeZero();
  theNode.theAnimation.GetElement()+=theOps;
  theNode.theAnimation.GetElement().flagAnimating=true;
  return theNode.errorNoError;
}

int ParserNode::EvaluateDrawRootSystem
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables, root* bluePoint)
{ char theWeylLetter= (char)theNode.owner->TheObjects[theArgumentList[0]].intValue;
  int theDimension= theNode.owner->TheObjects[theArgumentList[1]].intValue;
  theWeylLetter+='A';
  CGIspecificRoutines::MakeSureWeylGroupIsSane(theWeylLetter, theDimension);
  return theNode.EvaluateDrawRootSystem(theNode, theWeylLetter, theDimension, theGlobalVariables, bluePoint);
}

double DrawOperations::getAngleFromXandY(double x, double y, double neighborX, double neighborY)
{ double result;
  if (x!=0)
   result= atan(y/x);
  else
    if (y>0)
      result= MathRoutines::Pi()/2;
    else
      result= MathRoutines::Pi()/(-2);
  return result;
}

void DrawOperations::click(double x , double y)
{ this->EnsureProperInitialization();
  this->SelectedCircleMinus2noneMinus1Center=-2;
  if (this->AreWithinClickTolerance(x, y, this->centerX[this->SelectedPlane], this->centerY[this->SelectedPlane]))
    this->SelectedCircleMinus2noneMinus1Center=-1;
  int theDim=this->theBilinearForm.NumRows;
  for (int i=0; i<theDim; i++)
  { double Xbasis, Ybasis;
    this->GetCoordsDrawingComputeAll(this->BasisToDrawCirclesAt[i], Xbasis, Ybasis);
    if (this->AreWithinClickTolerance(x, y, Xbasis, Ybasis))
    { this->SelectedCircleMinus2noneMinus1Center=i;
      return;
    }
  }
}

void DrawOperations::RotateOutOfPlane
  (std::stringstream& logger, Vector<double>& input, Vector<double>& output,
   Vector<double>& orthoBasis1, Vector<double>& orthoBasis2, double oldTanSquared, double newTanSquared)
{ Vector<double> projection= orthoBasis1;
  Vector<double> vComponent= input;
  double scal1= this->theBilinearForm.ScalarProduct(orthoBasis1, input);
  double scal2= this->theBilinearForm.ScalarProduct(orthoBasis2, input);
  projection*=scal1;
  projection+= orthoBasis2*scal2;
  vComponent-=projection;
  logger << "\ngetScalarProd=" << this->theBilinearForm.ScalarProduct(projection, vComponent);
  if (oldTanSquared<0 || newTanSquared<0)
    return;
  double oldAngle=atan(sqrt(oldTanSquared));
  double newAngle=atan(sqrt(newTanSquared));
  double angleChange=-oldAngle+newAngle;
  projection=orthoBasis1;
  projection*=cos(angleChange)*scal1-sin(angleChange)*scal2;
  projection+=orthoBasis2*(sin(angleChange)*scal1+sin(angleChange)*scal2);
  output = vComponent;
  output+=projection;
}

void DrawOperations::ModifyToOrthonormalNoShiftSecond
(Vector<double>& root1, Vector<double>& root2)
{ //if  (this->getScalarProduct(root2, root2)==0)
  //  root2.MakeEi(this->theWeyl.CartanSymmetric.NumRows,1);
  double theScalar= this->theBilinearForm.ScalarProduct(root1, root2)/this->theBilinearForm.ScalarProduct(root2, root2);
  root1-=root2*theScalar;
  this->ScaleToUnitLength(root1);
  this->ScaleToUnitLength(root2);
}

void DrawOperations::ComputeProjectionsEiVectors()
{ int theDimension=this->theBilinearForm.NumRows;
  this->ProjectionsEiVectors.SetSizeMakeMatrix(theDimension, 2);
  Vector<double> tempRoot;
  for (int i=0; i<theDimension; i++)
  { tempRoot.MakeEi(theDimension, i, 1, 0);
    this->ProjectionsEiVectors[i][0]=this->theBilinearForm.ScalarProduct(tempRoot, this->BasisProjectionPlane[this->SelectedPlane][0]);
    this->ProjectionsEiVectors[i][1]=this->theBilinearForm.ScalarProduct(tempRoot, this->BasisProjectionPlane[this->SelectedPlane][1]);
  }
}

void DrawOperations::ComputeXYsFromProjectionsEisAndGraphicsUnit()
{ for (int i=0; i<this->theDrawLineBetweenTwoRootsOperations.size; i++)
  { DrawLineBetweenTwoRootsOperation& theOperation= this->theDrawLineBetweenTwoRootsOperations[i];
    this->GetCoordsForDrawingProjectionsComputed(theOperation.v1, theOperation.v2, theOperation.precomputedX1, theOperation.precomputedY1, theOperation.precomputedX2, theOperation.precomputedY2);
  }
  for (int i=0; i<this->theDrawTextAtVectorOperations.size; i++)
  { DrawTextAtVectorOperation& theTextOperation=this->theDrawTextAtVectorOperations[i];
    this->GetCoordsForDrawingProjectionsComputed(theTextOperation.theVector, theTextOperation.precomputedX, theTextOperation.precomputedY);
  }
  for (int i=0; i<this->theDrawCircleAtVectorOperations.size; i++)
  { DrawCircleAtVectorOperation& theCircleOperation=this->theDrawCircleAtVectorOperations[i];
    this->GetCoordsForDrawingProjectionsComputed(theCircleOperation.theVector, theCircleOperation.precomputedX, theCircleOperation.precomputedY);
  }
}

void DrawOperations::changeBasisPreserveAngles(double newX, double newY, GlobalVariables& theGlobalVariables)
{ double bufferCenterX=this->centerX[this->SelectedPlane];
  double bufferCenterY=this->centerY[this->SelectedPlane];
  double bufferGraphicsUnit=this->GraphicsUnit[this->SelectedPlane];
  newX=(newX-bufferCenterX)/bufferGraphicsUnit;
  newY=(newY-bufferCenterY)/bufferGraphicsUnit;
  if (newX==0 && newY==0)
    return;
  std::stringstream out;
  Vector<double>& selectedRoot=this->BasisToDrawCirclesAt[this->SelectedCircleMinus2noneMinus1Center];
  double selectedRootLength=this->theBilinearForm.ScalarProduct(selectedRoot, selectedRoot);
  double oldX, oldY;
  this->GetCoordsDrawingComputeAll(selectedRoot, oldX, oldY);
  oldX=(oldX- bufferCenterX)/bufferGraphicsUnit;
  oldY=(oldY- bufferCenterY)/bufferGraphicsUnit;

  double oldAngle= getAngleFromXandY(oldX, oldY, newX, newY);
  double newAngle= getAngleFromXandY(newX, newY, oldX, oldY);
  double AngleChange= -newAngle+oldAngle;
  double epsilon=0.000000000001;
  while (AngleChange>MathRoutines::Pi()/2+epsilon)
  { AngleChange-=MathRoutines::Pi();
  }
  while (AngleChange<=MathRoutines::Pi()/(-2)-epsilon)
  { AngleChange+=MathRoutines::Pi();
  }
  out << "\nold angle: " << oldAngle;
  out << "\nnew angle:  " << newAngle;
  Vector<double> NewVectorE1, NewVectorE2;
  Vectors<double>& currentBasisPlane=this->BasisProjectionPlane[this->SelectedPlane];
  NewVectorE1= currentBasisPlane[0]*cos(AngleChange);
  NewVectorE1+=currentBasisPlane[1]*sin(AngleChange);
  NewVectorE2= currentBasisPlane[1]*cos(AngleChange);
  NewVectorE2+=currentBasisPlane[0]*(-sin(AngleChange));
  currentBasisPlane[0]=NewVectorE1;
  currentBasisPlane[1]=NewVectorE2;
  double RootTimesE1=this->theBilinearForm.ScalarProduct(selectedRoot, currentBasisPlane[0]);
  double RootTimesE2=this->theBilinearForm.ScalarProduct(selectedRoot, currentBasisPlane[1]);
  Vector<double> vOrthogonal=selectedRoot;
  Vector<double> vProjection=currentBasisPlane[0]*RootTimesE1;
  vProjection+=currentBasisPlane[1]*RootTimesE2;
  vOrthogonal-= vProjection;
  double oldRatioProjectionOverHeightSquared = (oldX*oldX+oldY*oldY)/ (selectedRootLength-oldX*oldX-oldY*oldY);
  double newRatioProjectionOverHeightSquared = (newX*newX+newY*newY)/ (selectedRootLength-newX*newX-newY*newY);
  out << "\noldRatio: " << oldRatioProjectionOverHeightSquared;
  out << "\nnewRatio: " << newRatioProjectionOverHeightSquared;
  if (this->theBilinearForm.ScalarProduct(vOrthogonal, vOrthogonal)>epsilon || this->theBilinearForm.ScalarProduct(vOrthogonal, vOrthogonal)<-epsilon)
  { this->ScaleToUnitLength(vProjection);
    this->ScaleToUnitLength(vOrthogonal);
    out << "\nscaled vOrthogonal=" << vOrthogonal.ElementToString() << "->" << this->theBilinearForm.ScalarProduct(vOrthogonal, vOrthogonal);
    out << "\nscaled vProjection=" << vProjection.ElementToString() << "->" <<this->theBilinearForm.ScalarProduct(vProjection, vProjection);
    out << "\ntheScalarProd: " << this->theBilinearForm.ScalarProduct(vOrthogonal, vProjection);
    this->RotateOutOfPlane(out, currentBasisPlane[0], currentBasisPlane[0], vProjection, vOrthogonal, oldRatioProjectionOverHeightSquared, newRatioProjectionOverHeightSquared);
    this->RotateOutOfPlane(out, currentBasisPlane[1], currentBasisPlane[1], vProjection, vOrthogonal, oldRatioProjectionOverHeightSquared, newRatioProjectionOverHeightSquared);
  }
//  this->e1.ComputeDebugString();
//  this->e2.ComputeDebugString();
  this->ModifyToOrthonormalNoShiftSecond(currentBasisPlane[0], currentBasisPlane[1]);
//  this->e1.ComputeDebugString();
//  this->e2.ComputeDebugString();
  out << "\ne1=" << currentBasisPlane[0].ElementToString();
  out << "\ne2=" << currentBasisPlane[1].ElementToString();
  out << "\ne1*e2=" << this->theBilinearForm.ScalarProduct(currentBasisPlane[0], currentBasisPlane[1]);
  if (this->specialOperationsOnBasisChange!=0)
    this->specialOperationsOnBasisChange(*this, theGlobalVariables);
  this->ComputeProjectionsEiVectors();
  this->DebugString= out.str();
}

class ImpreciseDouble
{
  private:
  double precision;
  double theValue;
  public:
  void ElementToString(std::string& output)
  { output=this->ElementToString();
  }
  std::string ElementToString()
  { std::stringstream out;
    out << this->theValue;
    return out.str();
  }
  void operator=(const ImpreciseDouble& other)
  { this->theValue=other.theValue;
    this->precision=other.precision;
  }
  void operator=(double other)
  { this->theValue=other;
  }
  ImpreciseDouble(const ImpreciseDouble& other)
  { this->operator=(other);
  }
  ImpreciseDouble()
  { this->theValue=0;
    this->precision=0.1;
  }
  ImpreciseDouble(double other)
  { this->operator=(other);
  }
  void operator+=(const ImpreciseDouble& other)
  { if (!other.IsEqualToZero())
      this->theValue+=other.theValue;
  }
  void operator-=(const ImpreciseDouble& other)
  { if (!other.IsEqualToZero())
      this->theValue-=other.theValue;
  }
  void operator=(const Rational& other)
  { this->theValue=other.DoubleValue();
  }
  bool IsEqualToZero()const
  { if (this->theValue<0)
      return (-theValue)< this->precision;
    return this->theValue<this->precision;
  }
  inline bool operator<=(const ImpreciseDouble& other)
  { return ! (other<*this);
  }
  bool IsPositive()const
  { return this->theValue>this->precision;
  }
  inline bool IsNegative()const
  { return *this<this->GetZero();
  }
  bool operator<(const ImpreciseDouble& other)const
  { ImpreciseDouble temp=other;
    temp-=*this;
    return temp.IsPositive();
  }
  void AssignFloor()
  { this->theValue=floor(this->theValue);
  }
  inline ImpreciseDouble operator/(const ImpreciseDouble& other)const
  { ImpreciseDouble result;
    result=*this;
    if (other.IsEqualToZero())
    { //the below is written like this to avoid this->theValue/0;
      //If the user attempts to divide by zero, I want a regular division by zero exception to be generated
      result.theValue=this->theValue/(other.theValue-other.theValue);
      return result;
    }
    result.theValue/=other.theValue;
    return result;
  }
  void operator*=(const ImpreciseDouble& other)
  { if (!other.IsEqualToZero())
      this->theValue*=other.theValue;
    else
      this->theValue=0;
  }
  bool operator==(const ImpreciseDouble& other)const
  { int diff=this->theValue-other.theValue;
    if (diff<0)
      diff=-diff;
    return diff<this->precision;
  }
  static inline ImpreciseDouble GetMinusOne()
  { return -1;
  }
  static inline ImpreciseDouble GetOne()
  { return 1;
  }
  static inline ImpreciseDouble GetZero()
  { return 0;
  }
};

int ParserNode::EvaluateLatticeImprecise
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ roots tempRoots;
  int theDim=-1;
  if (!theNode.GetRootsEqualDimNoConversionNoEmptyArgument(theArgumentList, tempRoots, theDim))
    return theNode.SetError(theNode.errorBadOrNoArgument);
  Matrix<ImpreciseDouble> theMat;
  theMat.init(tempRoots.size, theDim);
  for (int i=0; i<theMat.NumRows; i++)
    for (int j=0; j<theMat.NumCols; j++)
      theMat.elements[i][j]=tempRoots[i][j];
  std::cout << "starting matrix: " << theMat.ElementToString(true, false);
  theMat.GaussianEliminationEuclideanDomain(ImpreciseDouble::GetMinusOne(), ImpreciseDouble::GetOne());
  std::cout << "<br>final matrix: " << theMat.ElementToString(true, false);
  return theNode.errorNoError;
}

void DrawOperations::projectionMultiplicityMergeOnBasisChange(DrawOperations& theOps, GlobalVariables& theGlobalVariables)
{ Matrix<ImpreciseDouble> theMat;
  int theDim=theOps.theBilinearForm.NumRows;
  theMat.init(theDim, 2);
//we assume that the ComputeProjectionsEiVectors has been called
//  theOps.ComputeProjectionsEiVectors();
  for(int i=0; i<theOps.ProjectionsEiVectors.size; i++)
    for (int j=0; j<2; j++)
      theMat.elements[i][j]=theOps.ProjectionsEiVectors[i][j];
  theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
  std::stringstream out;
  out << "before elimination:\n" << theMat.ElementToString(false, false);
  theMat.GaussianEliminationEuclideanDomain(ImpreciseDouble::GetMinusOne(), ImpreciseDouble::GetOne());
  out << "after elimination:\n" << theMat.ElementToString(false, false);
  theGlobalVariables.theIndicatorVariables.StatusString1=out.str();
  theGlobalVariables.MakeReport();

}

int ParserNode::EvaluateDrawG2InB3
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ //theNode.owner->theHmm.MakeG2InB3(theParser, theGlobalVariables);
  AnimationBuffer& Ani=theNode.theAnimation.GetElement();
  theNode.EvaluateDrawRootSystem(theNode, 'B', 3, theGlobalVariables, 0);
  SelectionWithMaxMultiplicity theSel;
  int theDim=3;
  int multSize=9;
  theSel.initMaxMultiplicity(theDim, multSize);
  int numPoints= theSel.NumSelectionsTotal();
  root tempRoot;
  DrawOperations& theOps=Ani.thePhysicalDrawOps[0];
  root highestWeight;
  highestWeight.MakeZero(theDim);
  hashedRoots theWeights;
  for (int i=0; i<numPoints; i++, theSel.IncrementSubset())
  { tempRoot=theSel;
    tempRoot.MinusRoot();
    theWeights.AddObjectOnTopHash(tempRoot);
    theOps.drawCircleAtVectorBuffer(tempRoot, 2, DrawingVariables::PenStyleNormal, CGIspecificRoutines::RedGreenBlue(150,150,150));
  }
  theOps.indexStartingModifiableTextCommands=theOps.theDrawTextAtVectorOperations.size;
  theOps.specialOperationsOnBasisChange=&theOps.projectionMultiplicityMergeOnBasisChange;
  return theNode.errorNoError;
}

void DrawOperations::operator+=(const DrawOperations& other)
{ if (this->theBilinearForm.NumRows!=other.theBilinearForm.NumRows)
    return;
  this->TypeNthDrawOperation.AddListOnTop(other.TypeNthDrawOperation);
  int shiftDrawText=this->theDrawTextOperations.size;
  int shiftDrawTextAtVector=this->theDrawTextAtVectorOperations.size;
  int shiftDrawLine=this->theDrawLineOperations.size;
  int shiftDrawLineBnVectors=this->theDrawLineBetweenTwoRootsOperations.size;
  int shiftDrawCircleAtVector=this->theDrawCircleAtVectorOperations.size;
  this->IndexNthDrawOperation.MakeActualSizeAtLeastExpandOnTop
  (this->IndexNthDrawOperation.size+other.IndexNthDrawOperation.size);
  for (int i=0; i<other.TypeNthDrawOperation.size; i++)
    switch(other.TypeNthDrawOperation[i])
    { case DrawOperations::typeDrawCircleAtVector:
        this->IndexNthDrawOperation.AddObjectOnTop(other.IndexNthDrawOperation[i]+shiftDrawCircleAtVector);
        break;
      case DrawOperations::typeDrawLine:
        this->IndexNthDrawOperation.AddObjectOnTop(other.IndexNthDrawOperation[i]+shiftDrawLine);
        break;
      case DrawOperations::typeDrawLineBetweenTwoVectors:
        this->IndexNthDrawOperation.AddObjectOnTop(other.IndexNthDrawOperation[i]+shiftDrawLineBnVectors);
        break;
      case DrawOperations::typeDrawText:
        this->IndexNthDrawOperation.AddObjectOnTop(other.IndexNthDrawOperation[i]+shiftDrawText);
        break;
      case DrawOperations::typeDrawTextAtVector:
        this->IndexNthDrawOperation.AddObjectOnTop(other.IndexNthDrawOperation[i]+shiftDrawTextAtVector);
        break;
      default:
        assert(false);
        break;
    }
  this->theDrawTextOperations.AddListOnTop(other.theDrawTextOperations);
  this->theDrawLineOperations.AddListOnTop(other.theDrawLineOperations);
  this->theDrawLineBetweenTwoRootsOperations.AddListOnTop(other.theDrawLineBetweenTwoRootsOperations);
  this->theDrawTextAtVectorOperations.AddListOnTop(other.theDrawTextAtVectorOperations);
  this->theDrawCircleAtVectorOperations.AddListOnTop(other.theDrawCircleAtVectorOperations);
  //this->BasisProjectionPlane.AddListOnTop(other.BasisProjectionPlane);
  //this->centerX.AddListOnTop(other.centerX);
  //this->centerY.AddListOnTop(other.centerY);
  //this->GraphicsUnit.AddListOnTop(other.GraphicsUnit);
  this->SelectedPlane=0;
}

DrawOperations& AnimationBuffer::GetLastDrawOps()
{ if (this->thePhysicalDrawOps.size<=0)
    return this->stillFrame;
  return *this->thePhysicalDrawOps.LastObject();
}

std::string ElementWeylGroup::ElementToString
  (bool useLatex, bool useHtml, const std::string& simpleRootLetter, List<int>* DisplayIndicesOfSimpleRoots)
{ if (this->size==0)
    return "id";
  std::stringstream out;
  for (int i=this->size-1; i>=0; i--)
  { out << "s_{" << simpleRootLetter << "_{";
    if (DisplayIndicesOfSimpleRoots==0)
      out << this->TheObjects[i]+1;
    else
      out << (*DisplayIndicesOfSimpleRoots)[this->TheObjects[i]];
    out << "} }";
  }
  return out.str();
}

void ReflectionSubgroupWeylGroup::ElementToString(std::string& output)
{ std::stringstream out; std::string tempS;
  int currentSize=0;
  out << "\\begin{array}{c}";
  for (int i=0; i<this->size; i++)
  { out << this->TheObjects[i].ElementToString();
    if (i!=this->size-1)
    { if (currentSize<this->TheObjects[i+1].size)
      { currentSize=this->TheObjects[i+1].size;
        out << "\\\\";
      } else out << ",";
    }
  }
  out << "\\end{array}";

  this->ExternalAutomorphisms.ElementToStringGeneric(tempS);
  out << tempS;
  output=out.str();
}

int ParserNode::EvaluateParabolicWeylGroups
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ WeylGroup& theAmbientWeyl=theNode.owner->theHmm.theRange.theWeyl;
  Selection parabolicSel;
  parabolicSel.init(theAmbientWeyl.GetDim());
  int numCycles=MathRoutines::TwoToTheNth(parabolicSel.MaxSize);
  ReflectionSubgroupWeylGroup theSubgroup;
  std::stringstream out;
  for (int i=0; i<numCycles; i++, parabolicSel.incrementSelection())
  { theSubgroup.MakeParabolicFromSelectionSimpleRoots(theAmbientWeyl, parabolicSel, theGlobalVariables, 2000);
    out << "<hr>" << CGIspecificRoutines::GetHtmlMathDivFromLatexFormulA(theSubgroup.ElementToString());
  }
  theNode.outputString=out.str();
  theNode.ExpressionType=theNode.typeString;
  return theNode.errorNoError;
}

std::string ReflectionSubgroupWeylGroup::ElementToStringBruhatGraph()
{ if (this->size<1)
    return "Error, non-initialized group";
  if (this->size==1)
    return "id";
  List<List<List<int> > > arrows;
  List<List<int> > Layers;
  root tempRoot;
  Layers.MakeActualSizeAtLeastExpandOnTop(this->size);
  int GraphWidth=1;
  int oldLayerElementLength=-1;
  for (int i=0; i< this->size; i++)
  { if (this->TheObjects[i].size!=oldLayerElementLength)
    { Layers.SetSize(Layers.size+1);
      oldLayerElementLength=this->TheObjects[i].size;
    }
    Layers.LastObject()->AddObjectOnTop(i);
    GraphWidth=MathRoutines::Maximum(GraphWidth, Layers.LastObject()->size);
  }
  hashedRoots orbit;
  orbit.MakeActualSizeAtLeastExpandOnTop(this->size);
  for (int i=0; i<this->size; i++)
  { this->ActByElement(i, this->AmbientWeyl.rho, tempRoot);
    orbit.AddObjectOnTopHash(tempRoot);
  }
  arrows.SetSize(Layers.size);
  for (int i=0; i< Layers.size; i++)
  { arrows[i].SetSize(Layers[i].size);
    for (int j=0; j<Layers[i].size; j++)
      for (int k=0; k<this->simpleGenerators.size; k++)
      { this->AmbientWeyl.ReflectBetaWRTAlpha(this->simpleGenerators[k], orbit[Layers[i][j]], false, tempRoot);
        int index=orbit.IndexOfObjectHash(tempRoot);
        assert(index!=-1);
        if (this->TheObjects[index].size>this->TheObjects[Layers[i][j]].size)
          arrows[i][j].AddObjectOnTop(index);
      }
  }
  std::stringstream out;
//  std::cout << this->simpleGenerators.ElementToString();
  List<int> DisplayIndicesSimpleGenerators;
  DisplayIndicesSimpleGenerators.SetSize(this->simpleGenerators.size);
  for (int i=0; i<this->simpleGenerators.size; i++)
    DisplayIndicesSimpleGenerators[i]=this->AmbientWeyl.RootsOfBorel.IndexOfObject(this->simpleGenerators[i])+1;
  out << "\\xymatrix{";
  bool GraphWidthIsOdd=((GraphWidth%2)!=0);
  if (!GraphWidthIsOdd)
    GraphWidth++;
  for (int i=0; i<Layers.size; i++)
  { int currentRowOffset=(GraphWidth-Layers[i].size)/2;
    int nextRowOffset=-1;
    if (i<Layers.size-1)
      nextRowOffset=(GraphWidth-Layers[i+1].size)/2;
    for (int j=0; j<currentRowOffset; j++)
      out << "&";
    for (int j=0; j<Layers[i].size; j++)
    { out << this->TheObjects[Layers[i][j]].ElementToString(true, false, "\\eta", &DisplayIndicesSimpleGenerators);
      int currentOffset=j+currentRowOffset;
      if (Layers[i].size%2==0)
        if (currentOffset>=GraphWidth/2)
          currentOffset++;
      for (int k=0; k<arrows[i][j].size; k++)
      { out << " \\ar[d";
        int indexInLayer=Layers[i+1].IndexOfObject(arrows[i][j][k]);
        assert(indexInLayer!=-1);
        int nextOffset=indexInLayer+nextRowOffset;
        if (Layers[i+1].size%2==0)
          if (nextOffset>=GraphWidth/2)
            nextOffset++;
        int actualOffset=-currentOffset+nextOffset;
        for (int l=0; l<actualOffset; l++)
          out << "r";
        for (int l=0; l>actualOffset; l--)
          out << "l";
        out << "]";
      }
      out << " & ";
      if (Layers[i].size%2==0 && j==Layers[i].size/2-1)
        out << " & ";
    }
    out << " \\\\\n";
  }
  out << "}";
  return out.str();
}

int ParserNode::EvaluateParabolicWeylGroupsBruhatGraph
  (ParserNode& theNode, List<int>& theArgumentList, GlobalVariables& theGlobalVariables)
{ WeylGroup& theAmbientWeyl=theNode.owner->theHmm.theRange.theWeyl;
  root tempRoot;
  if (!theNode.GetRootRationalFromFunctionArguments(tempRoot, theGlobalVariables))
    return theNode.SetError(theNode.errorBadOrNoArgument);
  if (tempRoot.size!=theAmbientWeyl.GetDim())
    return theNode.SetError(theNode.errorDimensionProblem);
  Selection parabolicSel;
  parabolicSel=tempRoot;
  ReflectionSubgroupWeylGroup theSubgroup;
  std::stringstream out;
  std::fstream outputFile;
  std::string fileName;
  fileName.append(theNode.owner->outputFolderPath);
  fileName.append("output");
  CGIspecificRoutines::OpenDataFileOrCreateIfNotPresent(outputFile, fileName+".tex", false, true, false);
  theSubgroup.MakeParabolicFromSelectionSimpleRoots(theAmbientWeyl,  parabolicSel, theGlobalVariables, 500);
  outputFile << "\\documentclass{article}\\usepackage[all,cmtip]{xy}\\begin{document}\n";
  if (theSubgroup.size>498)
  { if (theSubgroup.AmbientWeyl.GetSizeWeylByFormula('E', 6) <= theSubgroup.AmbientWeyl.GetSizeWeylByFormula(theAmbientWeyl.WeylLetter, theAmbientWeyl.GetDim()))
      out << "Even I can't handle the truth, when it is so large<br>";
    else
      out << "LaTeX can't handle handle the truth, when it is so large. <br>";
  }
  else
 { outputFile << "\\[" << theSubgroup.ElementToStringBruhatGraph() << "\\]";
   outputFile << "\n\\end{document}";
   out << "A png file: <a href=\"" << theNode.owner->outputFolderDisplayPath << "output.tex\"> output.tex</a>";
   out << ", <a href=\"" << theNode.owner->outputFolderDisplayPath << "output.png\"> output.png</a>";
 }
  theNode.outputString=out.str();
//  theCommand << "pdflatex -output-directory=" << theNode.owner->outputFolderPath << "   " << fileName ;

  theNode.owner->SystemCommands.AddObjectOnTop("latex  -output-directory=" + theNode.owner->outputFolderPath + " " + fileName + ".tex");

  theNode.owner->SystemCommands.AddObjectOnTop("dvipng " + fileName + ".dvi -o " + fileName + ".png -T tight");
  theNode.outputString=out.str();
  theNode.ExpressionType=theNode.typeString;
  return theNode.errorNoError;
}

void Parser::initFunctionList(char defaultExampleWeylLetter, int defaultExampleWeylRank)
{ if (this->flagFunctionListInitialized)
    return;
  this->flagFunctionListInitialized=true;
  ParserFunction theFunction;
  this->AddOneFunctionToDictionaryNoFail
  ("coneFromNormals",
   "((Rational,...),...)",
   "A polyhedron in n dimensions such that all of its walls pass through zero. The argument vectors describe the normals of the cone walls. \
   To each such polyhedron C we assign the affine polyhedron obtained by intersecting C with the hyperplane passing through (0,...,0,1) and parallel to the hyperplane spanned by the vectors with last coordinate zero.",
   "coneFromNormals((1,0),(0,1)) ",
    & ParserNode::EvaluateCone
   );
  this->AddOneFunctionToDictionaryNoFail
  ("coneFromVertices",
   "((Rational,...),...)",
   "Create the cone spanned over the non-negative rationals by the input vectors. You can use the alias name cone, for example cone((1,0,0),(0,1,0), (0,0,1),(0,1,1), (1,1,0),(1,1,1)).",
   "coneFromVertices((1,0,0),(0,1,0), (0,0,1),(0,1,1), (1,1,0),(1,1,1)) ",
    & ParserNode::EvaluateConeFromVertices
   );
  this->AddOneFunctionToDictionaryNoFail
  ("cone",
   "((Rational,...),...)",
   "Alias for coneFromVertices.",
   "cone((1,0,0),(0,1,0), (0,0,1),(0,1,1), (1,1,0),(1,1,1)) ",
    DefaultWeylLetter, DefaultWeylRank, false,
    & ParserNode::EvaluateConeFromVertices
   );
  this->AddOneFunctionToDictionaryNoFail
  ("slTwoInSlN",
   "(Integer,...)",
   "Prints out sl(2) embedded in sl(N) according to the partition described by the arguments.",
   "slTwoInSlN(2,3)",
    & ParserNode::EvaluateSlTwoInSlN
   );
  this->AddOneFunctionToDictionaryNoFail
  ("getWeylDim",
   "(Rational,...)",
   "Evaluates the Weyl dimension formula. Input: a vector in dual root coordinates.",
   "getWeylDim(0,0,1)",
    & ParserNode::EvaluateWeylDimFormula
   );
  this->AddOneFunctionToDictionaryNoFail
  ("printRootSystem",
   "()",
   "Print the root system of the ambient Lie algebra.",
   "printRootSystem",
   DefaultWeylLetter, DefaultWeylRank,
    & ParserNode::EvaluatePrintRootSystem
   );
  this->AddOneFunctionToDictionaryNoFail
  ("printBthreeDecomposedOverGtwo",
   "()",
   "Prints the decomposition of so(7) over G2. The G2-generators embedding is hardcoded according to an article of MacGovern.",
   "printBthreeDecomposedOverGtwo",
    & ParserNode::EvaluatePrintDecomposition
   );
  this->AddOneFunctionToDictionaryNoFail
  ("actByWeyl",
   "(RF,...)",
   "Act by the Weyl group on a weight vector whose coordinates are given in simple basis coordinates. For example, for the Weyl group of A_2 (sl(3)),  the vector (1, -x_1) corresponds to a-x_1b, where a and b are the first and second simple roots, and x_1 is a variable. The coordinates are allowed to be arbitrary rational functions. ",
   "actByWeyl(x_1, x_2, x_3)",
    & ParserNode::EvaluateWeylAction
   );
  this->AddOneFunctionToDictionaryNoFail
  ("actByWeylRho",
   "(RF,...)",
   "Act using the \\rho-modified action on a weight vector whose coordinates are given in simple basis coordinates. For an element of the Weyl group w, the \\rho-modified action of w on \\alpha is given by w(\\alpha+\\rho)-\\rho, where \\rho is the half-sum of the positive roots of the ambient root system. \
   This function is the affine \\rho-modification of the function ActByWeyl. The following example calls the action of the Weyl group of so(7); the half-sum of the positive roots of so(7) in simple coordinates equals (5,8,9).",
   "actByWeylRho(x_1-5/2, x_2-8/2,x_3-9/2)",
    & ParserNode::EvaluateWeylRhoAction
   );
  this->AddOneFunctionToDictionaryNoFail
  ("partialFraction",
   "((Rational,...),...)",
   "Gives the partial fraction corresponding to the vector partition function of the arguments.<br> The arguments must be non-zero vectors with non-negative integral coordinates.",
   "partialFraction((1,0), (0,1), (1,1))",
    & ParserNode::EvaluatePartialFractions
   );
  this->AddOneFunctionToDictionaryNoFail
  ("split",
   "(PF)",
   "Splits a partial fraction according to this <a href=\"http://arxiv.org/abs/0910.4675\"> text </a>, or a modification/improvement of it.",
   "split(partialFraction((1,0), (0,1), (1,1)))",
    & ParserNode::EvaluateSplit
   );
  this->AddOneFunctionToDictionaryNoFail
  ("vpf",
   "((Rational,...),...)",
   "Computes the vector partition function with respect to the input vectors, according to this <a href=\"http://arxiv.org/abs/0910.4675\"> text </a>.",
   "vpf((1,0), (0,1), (1,1))",
    & ParserNode::EvaluateVectorPFIndicator
   );
   this->AddOneFunctionToDictionaryNoFail
  ("transformToReducedGroebnerBasis",
   "(Polynomial,...)",
   "<b> This function is largely untested. If you use it make sure to double-check the output. </b> Transforms to reduced Groebner basis using Buchberger's algorithm with respect to the lexicographic monomial ordering x_1^l&lt; x_2^m&lt;x_3^n&lt;....",
   "transformToReducedGroebnerBasis(x_1^3+x_1x_2+1, x_1x_2, x_2^3)",
    & ParserNode::EvaluateGroebner
   );
   this->AddOneFunctionToDictionaryNoFail
  ("getRelations",
   "(Polynomial,...)",
   "<b> This function is largely untested. If you use it make sure to double-check the output. </b>Get the algebraic relations between the input polynomials.",
   "getRelations(x_1^2, x_1x_2, x_2^2)",
    & ParserNode::EvaluateRelations
   );
  this->AddOneFunctionToDictionaryNoFail
  ("printRootSubalgebras",
   "()",
   "Computes all root subalgebras. The computation is served from the hard disk if it is already computed. The function will redirect you to a new page, to return to the calculator use the back button.",
   "printRootSubalgebras",
   DefaultWeylLetter, DefaultWeylRank,
    & ParserNode::EvaluatePrintRootSAs
   );
  this->AddOneFunctionToDictionaryNoFail
  ("printSlTwos",
   "()",
   "Computes all sl(2) subalgebras (equivalently, all nilpotent orbits) over the complex numbers. The computation is served from the hard disk if it is already computed. The function will redirect you to a new page, to return to the calculator use the back button.",
   "printSlTwos",
   DefaultWeylLetter, DefaultWeylRank,
    & ParserNode::EvaluatePrintSlTwos
   );
  this->AddOneFunctionToDictionaryNoFail
  ("zprintSlTwosAndRootSAsFORCERecompute",
   "()",
   "<b>Use only if there are broken links in root subalgebra/sl(2)-database. \
   When executing this command, please be patient and do not click any links until automatically redirected. </b> \
   I have a problem with setting up the Apache server and the way it handles SIGABORT signals \
   (this is what happens when you don't wait for the computation to finish and click on a link prematurely). \
   This function is a temporary solution to this (minor) technical problem. \
   The function name starts with z because it is meant to be used rarely.",
   "zprintSlTwosAndRootSAsFORCERecompute",
    DefaultWeylLetter, DefaultWeylRank,
    & ParserNode::EvaluatePrintRootSAsForceRecompute
   );
  this->AddOneFunctionToDictionaryNoFail
  ("factorial",
   "(Integer)",
   "Returns the factorial of a non-negative integer.<=5000",
   "factorial(2011)",
    & ParserNode::EvaluateFactorial
   );
  this->AddOneFunctionToDictionaryNoFail
  ("printAllPrimesUpTo",
   "(Integer)",
   "Prints all primes smaller than argument and smaller than<=20000 using Eratosthenes sieve.",
   "printAllPrimesUpTo(2011)",
    & ParserNode::EvaluatePrintAllPrimesEratosthenes
   );
  this->AddOneFunctionToDictionaryNoFail
  ("invariantsSlTwoOfDegree",
   "(Integer, (Integer,...))",
   "Given an sl(2)-representation  V_1+...+V_n , computes the symmetric tensor algebra invariants of sl(2). Input: the first variable is the degree, followed by a vector describing the dimensions of  each V_i.",
   "invariantsSlTwoOfDegree(2,(2,2))",
    & ParserNode::EvaluateInvariantsSl2DegreeM
   );
/*  this->AddOneFunctionToDictionaryNoFail
  ("findExtremaInDirectionOverLatticeShiftedOneNonParam",
   "(Polynomial, (Rational,...), Lattice, Cone)",
   "<b> Experimental please don't use.</b>",
   "findExtremaInDirectionOverLatticeShiftedOneNonParam(-2x_1+x_2, (0,0), lattice((1,0),(0,1)), cone((-1,1,0),(1,0,0), (-2,-1,4)))",
    & ParserNode::EvaluateFindExtremaInDirectionOverLatticeOneNonParam
   );*/
  this->AddOneFunctionToDictionaryNoFail
  ("findMaximumInDirectionOverLatticeShifted",
   "(Polynomial, (Rational,...), Lattice, Cone, Integer)",
   "<b> Experimental please don't use.</b>",
   "findMaximumInDirectionOverLatticeShifted(-2x_1+x_2+x_3, (0,0,0), lattice((1,0,0),(0,1,0),(0,0,1)), cone((1,0,0,0),(0,1,0,0), (1,1,1,-4), (0,1,-1,0), (0,0,0,1)),2)",
    & ParserNode::EvaluateFindExtremaInDirectionOverLattice
   );
  this->AddOneFunctionToDictionaryNoFail
  ("lattice",
   "((Rational,...),...)",
   "Rational lattice generated by the argument vectors.",
   "lattice((1,3), (2,2))",
    & ParserNode::EvaluateLattice
   );
  this->AddOneFunctionToDictionaryNoFail
  ("getPointOnShiftedLatticeClosestToWallInDirection",
   "((Rational,...), (Rational,...), (Rational,...), Lattice)",
   "<b>Experimental, please don't use</b>.",
   "getPointOnShiftedLatticeClosestToWallInDirection( (2,3), (1/2,1/2), (1,1/5,1/3), lattice((1,1), (0,2)))",
    & ParserNode::EvaluateClosestPointToHyperplaneAlongTheNormal
   );
  this->AddOneFunctionToDictionaryNoFail
  ("intersectLatticeWithPreimageOfLattice",
   "(Lattice, Lattice, ((Rational,...),...))",
   "Intersects the first lattice with the preimage of the second lattice under the linear map described by the third argument. Suppose the dimension of vector space of the first lattice is m, and the dimension of the vector space of the second lattice is n. Then the linear map should be given by a list of m vectors, with n coordinates each. The i^th of these vectors should give the image of the i^th basis vector of the vector space of the first lattice. ",
   "intersectLatticeWithPreimageOfLattice(  lattice((1,1), (0,2)), lattice(26/5), ( (1), (1/5)) )",
    & ParserNode::EvaluateIntersectLatticeWithPreimageLattice
   );
  this->AddOneFunctionToDictionaryNoFail
  ("sliceConeUniqueExitWall",
   "(Cone, (Rational, ...))",
   "Slices the projective cone into smaller projective cones such that in each piece, for all point inside that piece, tracing a ray in the direction opposite to the one given by the second argument will \
   exit the cone from a unique wall (i.e. the exit wall does not depend on the starting point). This slice can be named the \"salami\"-slice, as a piece of salami can be cut at an arbitrary angle, \
   however the ends of the salami should always consist of a single plane.",
   "sliceConeUniqueExitWall( coneFromNormals((1,0,0),(0,1,0),(0,0,1)), (1,1,1) )",
    & ParserNode::EvaluateSliceCone
   );
  this->AddOneFunctionToDictionaryNoFail
  ("sliceDirections",
   "((Rational, ...),...)",
   "Exit wall slice. Slices the cone spanned by the input vectors so as to satisfy the salami slice condition inductively with respect to each new added vector. ",
   "sliceDirections( (1,0,0),(0,1,0),(0,0,1),(1,1,0), (0,1,1),(1,1,1) )",
    DefaultWeylLetter, DefaultWeylRank, true,
    & ParserNode::EvaluateCreateFromDirectionsAndSalamiSlice
   );
   this->AddOneFunctionToDictionaryNoFail
  ("drawConeProjective",
   "(Cone)",
   "On condition that the cone has a vertex at 0, draws a picture corresponding to the polyhedron with walls passing through 0 represented by the argument. ",
   "drawConeProjective( coneFromNormals((1,0,0),(0,1,0),(0,0,1)))",
    & ParserNode::EvaluateDrawConeProjective
   );
   this->AddOneFunctionToDictionaryNoFail
  ("drawConeAffine",
   "(Cone)",
   "On condition that the cone lies in the non-strict upper half-plane and has a vertex at 0, draws the intersection of the cone with the affine hyperplane passing through (0,...,0,1) and parallel to the hyperplane spanned by the vectors with last coordinate 0. ",
   "drawConeAffine( coneFromNormals((1,0,0,0),(0,1,0,0),(0,-1,1,0), (-1,-1,-1,5/2), (0,0,0,1)))",
    & ParserNode::EvaluateDrawConeAffine
   );
  this->AddOneFunctionToDictionaryNoFail
  ("RunGtwoInBthree",
   "()",
   "Run the G_2 in B_3 computation. Experimental, don't use.",
   "RunGtwoInBthree",
   DefaultWeylLetter, DefaultWeylRank, false,
    & ParserNode::EvaluateG2InB3Computation
   );
  this->AddOneFunctionToDictionaryNoFail
  ("drawRootSystem",
   "(Integer, Integer)",
   "<b>Experimental.</b> Draw the root system. First argument = weyl letter (A=0, B=1,..., G=7) . Second argument=weyl rank. The example draws E6.",
   "drawRootSystem(4,6)",
   DefaultWeylLetter, DefaultWeylRank,
    & ParserNode::EvaluateDrawRootSystem
   );
  this->AddOneFunctionToDictionaryNoFail
  ("animateRootSystemDefault",
   "(Integer, Integer, Integer)",
   "<b>Experimental.</b> Animate the root system. First argument = weyl letter 0=A. Second argument=weyl rank. Third argument= number of frames.",
   "animateRootSystemDefault(50)",
   DefaultWeylLetter, DefaultWeylRank, false,
    & ParserNode::EvaluateAnimateRootSystem
   );
  this->AddOneFunctionToDictionaryNoFail
  ("animatePause",
   "(Integer)",
   "<b>Experimental.</b> Animate the root system. First argument = weyl letter 0=A. Second argument=weyl rank. This argument= number of frames.",
   "animatePause(50)",
   DefaultWeylLetter, DefaultWeylRank, false,
    & ParserNode::EvaluateAnimationPause
   );
  this->AddOneFunctionToDictionaryNoFail
  ("animateClearScreen",
   "(Integer, Integer)",
   "<b>Experimental.</b> Animate the root system. First argument = weyl letter 0=A. Second argument=weyl rank. This argument= number of frames.",
   "animateClearScreen(0,2)",
   DefaultWeylLetter, DefaultWeylRank, false,
    & ParserNode::EvaluateAnimationClearScreen
   );
  this->AddOneFunctionToDictionaryNoFail
  ("animateRootSystemBlueDot",
   "(Integer, Integer, Integer, (Rational,...))",
   "<b>Experimental.</b> Animate the root system. First argument = weyl letter 0=A. Second argument=weyl rank. This argument= number of frames.",
   "animateRootSystemBlueDot(0,2, 16777216, ((1,0),(0,1)))",
   DefaultWeylLetter, DefaultWeylRank, false,
    & ParserNode::EvaluateAnimateRootSystemBluePoint
   );
  this->AddOneFunctionToDictionaryNoFail
  ("latticeImprecise",
   "((Rational,...),...)",
   "<b>Experimental.</b> Draws G2 in B3.",
   "latticeImprecise((1, 0.000000001), (1.00000000001, 1.00000000001), (1/2, 1/2))",
   DefaultWeylLetter, DefaultWeylRank, false,
    & ParserNode::EvaluateLatticeImprecise
   );
  this->AddOneFunctionToDictionaryNoFail
  ("drawGtwoInBthree",
   "()",
   "<b>Experimental.</b> Draws G2 in B3.",
   "drawGtwoInBthree",
   DefaultWeylLetter, DefaultWeylRank, false,
    & ParserNode::EvaluateDrawG2InB3
   );
  this->AddOneFunctionToDictionaryNoFail
  ("parabolicsInfo",
   "()",
   "Makes a table with information about the parabolic subalgebras of the ambient Lie algebra.",
   "parabolicsInfo",
   DefaultWeylLetter, DefaultWeylRank, false,
    & ParserNode::EvaluateParabolicWeylGroups
   );
  this->AddOneFunctionToDictionaryNoFail
  ("parabolicsInfoBruhatGraph",
   "(Integer,...)",
   "<b>Experimental, please don't use.</b>Makes a table with information about the parabolic subalgebras of the ambient Lie algebra. The input must have as many integers as there are simple roots in the ambient \
   Lie algebra. If the root is crossed out (i.e. not a root space of the Levi part), one should put a 1 in the corresponding coordinate. Otherwise, one should put 0. For example, for Lie algebra B3(so(7)), \
   calling parabolicsInfoBruhatGraph(0,0,0) gives you the Weyl group info for the entire algebra; calling parabolicsInfoBruhatGraph(1,0,0) gives you info for the Weyl subgroup generated by the last two simple roots \
   In the produced graph, the element s_{\\eta_i} corresponds to a reflection with respect to the i^th simple root. You will get your output as a .png file link, you must click onto the link to see the end result. ",
   "parabolicsInfoBruhatGraph(0,0,0)",
//   DefaultWeylLetter, DefaultWeylRank, true,
    & ParserNode::EvaluateParabolicWeylGroupsBruhatGraph
   );
  this->AddOneFunctionToDictionaryNoFail
  ("gTwoInBthreeMultsParabolic",
   "((Rational,...), (Rational,...))",
   "<b>Experimental, please don't use.</b> Computes the multiplicities of all G2 generalized Verma modules in the generalized Verma module of so(7) with so(7)-highest weight given by the first argument. \
   The second argument describes the parabolic subalgebra of so(7) (its intersection with G2 determines the parabolic subalgebra in G2). ",
   "gTwoInBthreeMultsParabolic((10,0,0), (1,0,0) )",
   DefaultWeylLetter, DefaultWeylRank, true,
    & ParserNode::EvaluateG2InB3MultsParabolic
   );
/*   this->AddOneFunctionToDictionaryNoFail
  ("solveLPolyEqualsZeroOverCone",
   "(Polynomial, Cone)",
   "<b> This function is largely untested. If you use it make sure to double-check the output. </b> Solves a linear equation over a set of linear inequalities. The first argument must be the linear polynomial (of, say, n variables). Then second argument must be the set of projectivized linear inequalities (in n+1 variables).",
   "solveLPolyEqualsZeroOverCone(x_1+x_2-1/2,cone((1,0,0), (0,1,0), (1, 1, 1) ))",
    & ParserNode::EvaluateSolveLPolyEqualsZeroOverCone
   );*/
   //printAllSlTwos
   this->theFunctionList.QuickSortAscending();
}
