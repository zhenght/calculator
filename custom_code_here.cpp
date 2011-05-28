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
  (CombinatorialChamberContainer& owner, GlobalVariables& theGlobalVariables)
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
    this->Externalwalls.TheObjects[i].TransformToWeylProjective(owner, theGlobalVariables);
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
//  this->ComputeDebugString();
  out << "Next chamber to slice: " << this->indexNextChamberToSlice;
  out << "Preferred next chambers:  ";
  for (int i=0; i<this->PreferredNextChambers.size; i++)
    out << this->PreferredNextChambers.TheObjects[i] << ",";
  out << "\nProcessed hyperplanes: " << this->NumAffineHyperplanesProcessed  << " out of " << this->NewHyperplanesToSliceWith.size << "\n";
  out << "Number of chamber pointers (including zero pointers): " << this->size;
//  out << this->DebugString;
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
