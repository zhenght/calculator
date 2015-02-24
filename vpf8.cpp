//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#include "vpfHeader1General0_General.h"
#include "vpfHeader2Math0_General.h"
#include "vpfImplementationHeader2Math051_PolynomialComputations_Basic.h"
#include "vpfImplementationHeader2Math052_PolynomialComputations_Advanced.h"

ProjectInformationInstance ProjectInfoVpf8cpp(__FILE__, "Math routines implementation. ");


void LargeIntUnsigned::AssignString(const std::string& input)
{ if (input.size()<10)
  { unsigned int x=std::atoi(input.c_str());
    this->AssignShiftedUInt(x, 0);
    return;
  }
  this->MakeZero();
  for (unsigned int i=0; i<input.size(); i++)
  { this->operator*=(10);
    int whichDigit=input[i]-'0';
    if (whichDigit>9 || whichDigit<0)
      crash << "This is a programming error: LargeIntUnsigned::AssignString" << " called on the string " << input
      << " which does not consist entirely of digits. Please note that LargeIntUnsigned::AssignString is a no-fail function, intended for "
      << " internal use only. If you want to parse arbitrary unsafe expressions coming from the user, please use the big gun (a.k.a. Calculator). "
      << crash;
    this->operator+=((unsigned) whichDigit);
  }
}

bool LargeIntUnsigned::AssignStringFailureAllowed(const std::string& input, bool ignoreNonDigits)
{ MacroRegisterFunctionWithName("LargeIntUnsigned::AssignStringFailureAllowed");
  if (input.size()>10000000) //<- sorry folks, no more than 10 million digits.
    return false;
  if (input.size()<10)
  { unsigned int x=std::atoi(input.c_str());
    this->AssignShiftedUInt(x, 0);
    return true;
  }
  this->MakeZero();
  for (unsigned int i=0; i<input.size(); i++)
  { this->operator*=(10);
    int whichDigit=input[i]-'0';
    if (whichDigit>9 || whichDigit<0)
    { if (!ignoreNonDigits)
        return false;
    } else
      this->operator+=((unsigned) whichDigit);
  }
  return true;
}

void Rational::AssignString(const std::string& input)
{ this->MakeZero();
  if (input=="0")
    return;
  int sign=1;
  int ReaderDigit=-1;
  Rational readerDen=1;
  int positionInString=0;
  for (; positionInString<(signed)input.size(); positionInString++)
  { if (input[positionInString]=='-')
    { sign=-1;
      positionInString++;
      break;
    }
    if (MathRoutines::isADigit(input[positionInString]))
      break;
  }
  for (; positionInString<(signed)input.size(); positionInString++)
  { if (MathRoutines::isADigit(input[positionInString], &ReaderDigit))
    { *this*=10;
      *this+=ReaderDigit;
    }
    if (input[positionInString]=='/')
    { positionInString++;
      readerDen=0;
      break;
    }
  }
  for (; positionInString<(signed)input.size(); positionInString++)
    if (MathRoutines::isADigit(input[positionInString], &ReaderDigit))
    { readerDen*=10;
      readerDen+=ReaderDigit;
    }
  if (readerDen==0)
    crash << "Programming error: Rational::AssignString encountered a zero denominator. " << crash;
  *this/=readerDen;
  *this*=sign;
}

void Selection::operator=(const Vector<Rational>& other)
{ this->init(other.size);
  for (int i=0; i<other.size; i++)
    if (!other[i].IsEqualToZero())
      this->selected[i]=true;
  this->ComputeIndicesFromSelection();
}

void ConeComplex::InitFromAffineDirectionsAndRefine(Vectors<Rational>& inputDirections, Vectors<Rational>& inputAffinePoints, GlobalVariables& theGlobalVariables)
{ if(inputDirections.size!=inputAffinePoints.size || inputDirections.size<=0)
    crash << crash;
  Vectors<Rational> projectivizedDirections;
  projectivizedDirections.SetSize(inputDirections.size*2);
  int theAffineDim= inputDirections[0].size;
  for (int i=0; i<inputDirections.size; i++)
  { projectivizedDirections[i]=inputDirections[i];
    projectivizedDirections[i].SetSize(theAffineDim+1);
    *projectivizedDirections[i].LastObject()=0;
  }
  for (int i=0; i<inputAffinePoints.size; i++)
  { projectivizedDirections[i+inputAffinePoints.size]=inputAffinePoints[i];
    projectivizedDirections[i+inputAffinePoints.size].SetSize(theAffineDim+1);
    *projectivizedDirections[i+inputAffinePoints.size].LastObject()=1;
  }
  this->InitFromDirectionsAndRefine(projectivizedDirections, theGlobalVariables);
}

void ConeComplex::MakeAffineAndTransformToProjectiveDimPlusOne(Vector<Rational>& affinePoint, ConeComplex& output, GlobalVariables& theGlobalVariables)
{ if(&output==this)
    crash << crash;
  output.init();
  Cone tempCone;
  Vectors<Rational> newNormals;
  Vector<Rational> tempRoot;
  int theAffineDim=affinePoint.size;
  for (int i=0; i<this->size; i++)
  { newNormals.SetSize(this->TheObjects[i].Normals.size+1);
    for (int j=0; j<this->TheObjects[i].Normals.size; j++)
      newNormals[j]= this->TheObjects[i].Normals[j].GetProjectivizedNormal(affinePoint);
    newNormals.LastObject()->MakeEi(theAffineDim+1, theAffineDim);
    tempCone.CreateFromNormals(newNormals, &theGlobalVariables);
    output.AddNonRefinedChamberOnTopNoRepetition(tempCone, theGlobalVariables);
  }
}

template<class coefficient>
Vector<coefficient> Vector<coefficient>::GetProjectivizedNormal(Vector<coefficient>& affinePoint)
{ Vector<coefficient> result=*this;
  result.SetSize(this->size+1);
  *result.LastObject()=-affinePoint.ScalarEuclidean(*this);
  return result;
}

void Lattice::GetRootOnLatticeSmallestPositiveProportionalTo(Vector<Rational>& input, Vector<Rational>& output, GlobalVariables& theGlobalVariables)
{ if(&input==&output)
    crash << crash;
  Vectors<Rational> theBasis;
  Vector<Rational> tempRoot;
  theBasis.AssignMatrixRows(this->basisRationalForm);
  input.GetCoordsInBasiS(theBasis, tempRoot);
  tempRoot.ScaleByPositiveRationalToIntegralMinHeight();
  Matrix<Rational>  tempMat;
  tempMat=this->basisRationalForm;
  tempMat.Transpose();
  tempMat.ActOnVectorColumn(tempRoot, output);
/*
  Vectors<Rational> tempRoots;
  tempRoots.AddOnTop(input);
  Lattice tempLattice=*this;
  tempLattice.IntersectWithLinearSubspaceSpannedBy(tempRoots);
  if(!tempLattice.basisRationalForm.NumRows==1)crash << crash;
  tempLattice.basisRationalForm.RowToRoot(0, output);
  Rational tempRat;
  bool tempBool=  output.IsProportionalTo(input, tempRat);
  if(!tempBool)crash << crash;
  if (tempRat.IsNegative())
    output.Minus();*/
}

bool Cone::GetLatticePointsInCone
(Lattice& theLattice, Vector<Rational>& theShift, int upperBoundPointsInEachDim, bool lastCoordinateIsOne, Vectors<Rational>& outputPoints, Vector<Rational>* shiftAllPointsBy)const
{ if (upperBoundPointsInEachDim<=0)
    upperBoundPointsInEachDim=5;
  Vector<Rational> theActualShift=theShift;
  theLattice.ReduceVector(theActualShift);
  int theDimAffine=this->GetDim();
  if (lastCoordinateIsOne)
    theDimAffine--;
  SelectionWithMaxMultiplicity boundingBox;
  boundingBox.initMaxMultiplicity(theDimAffine, upperBoundPointsInEachDim*2);
  //format of the boundingBox:
  // if bounding box shows a vector (x_1, ...) then
  // it corresponds to a vector with coodinates (x_1-upperBoundPointsInEachDim, x_2-upperBoundPointsInEachDim, ...)
  int numCycles= boundingBox.NumSelectionsTotal();
  if (numCycles<=0 || numCycles>1000000)//we test up to 1 million lattice points.
  //This is very restrictive: in 8 dimensions, selecting upperBoundPointsInEachDim=2,
  //we get a total of (2*2+1)^8=390625 points to test, which is a pretty darn small box
    return false;
  outputPoints.ReservE(numCycles);
  outputPoints.size=0;
  Vector<Rational> candidatePoint;
  Vectors<Rational> LatticeBasis;
  LatticeBasis.AssignMatrixRows(theLattice.basisRationalForm);
  for (int i=0; i<numCycles; i++, boundingBox.IncrementSubset())
  { candidatePoint=theActualShift;
    if (shiftAllPointsBy!=0)
      candidatePoint+=*shiftAllPointsBy;
    for (int j=0; j<boundingBox.Multiplicities.size; j++)
      candidatePoint+=LatticeBasis[j]*
      (boundingBox.Multiplicities[j]-upperBoundPointsInEachDim);
    if (lastCoordinateIsOne)
    { candidatePoint.SetSize(candidatePoint.size+1);
      *candidatePoint.LastObject()=1;
    }
    if (this->IsInCone(candidatePoint))
    {
      outputPoints.AddOnTop(candidatePoint);
    }
  }
  return true;
}

void PiecewiseQuasipolynomial::operator*=(const Rational& other)
{ if (other.IsEqualToZero())
  { this->MakeZero(this->NumVariables, *this->theBuffers);
    return;
  }
  for (int i=0; i<this->theQPs.size; i++)
    this->theQPs[i]*=other;
}

void PiecewiseQuasipolynomial::operator+=(const PiecewiseQuasipolynomial& other)
{ this->MakeCommonRefinement(other);
  for (int i=0; i<this->theProjectivizedComplex.size; i++)
  { int theIndex=other.theProjectivizedComplex.GetLowestIndexchamberContaining
    (this->theProjectivizedComplex[i].GetInternalPoint());
    if (theIndex!=-1)
      this->theQPs[i]+=other.theQPs[theIndex];
  }
}

bool PiecewiseQuasipolynomial::MakeVPF(Vectors<Rational>& theRoots, std::string& outputstring, GlobalVariables& theGlobalVariables)
{ if (theRoots.size<=0)
  { outputstring = "Error.";
    return false;
  }
  this->theBuffers=& theGlobalVariables;
  this->NumVariables=theRoots.GetDim();
  PartFractions theFracs;
  FormatExpressions theFormat;
  std::stringstream out;
  std::string whatWentWrong;

  theFracs.initFromRoots(theRoots, theGlobalVariables);
  out << CGI::GetMathMouseHover(theFracs.ToString(theGlobalVariables, theFormat));
  theFracs.split(theGlobalVariables, 0);
  out << CGI::GetMathMouseHover(theFracs.ToString(theGlobalVariables, theFormat));
  //theFracs.theChambers.InitFromDirectionsAndRefine(theRoots, theGlobalVariables);
  crash << crash ;
//  theFracs.theChambersOld.AmbientDimension=theRoots[0].size;
//  theFracs.theChambersOld.theDirections=theRoots;
//  theFracs.theChambersOld.SliceTheEuclideanSpace(theGlobalVariables, false);
//  theFracs.theChambers.AssignCombinatorialChamberComplex(theFracs.theChambersOld, theGlobalVariables);
  this->theQPs.SetSize(theFracs.theChambers.size);
  Vector<Rational> indicator;
  for (int i=0; i< theFracs.theChambers.size; i++)
  { indicator= theFracs.theChambers[i].GetInternalPoint();
    theFracs.GetVectorPartitionFunction(this->theQPs[i], indicator, theGlobalVariables);
    //QuasiPolynomial& currentQP=this->theQPs[i];
  }
  Lattice baseLattice;
  baseLattice.MakeFromRoots(theRoots);
  Cone baseCone;
  baseCone.CreateFromVertices(theRoots, &theGlobalVariables);
  Vector<Rational> shiftRoot;
  baseLattice.GetInternalPointInConeForSomeFundamentalDomain(shiftRoot, baseCone, theGlobalVariables);
  shiftRoot.Minus();
//  stOutput << "shiftRoot: " << shiftRoot.ToString();
  theFracs.theChambers.MakeAffineAndTransformToProjectiveDimPlusOne
  (shiftRoot, this->theProjectivizedComplex, theGlobalVariables);
  outputstring=out.str();
  return true;
}

bool Lattice::GetInternalPointInConeForSomeFundamentalDomain(Vector<Rational>& output, Cone& coneContainingOutputPoint, GlobalVariables& theGlobalVariables)
{ Vector<Rational> coordsInBasis;
  coneContainingOutputPoint.GetInternalPoint(output);
  Vectors<Rational> basisRoots;
  basisRoots.AssignMatrixRows(this->basisRationalForm);
  if (!output.GetCoordsInBasiS(basisRoots, coordsInBasis))
    return false;
  Rational maxCoord=coordsInBasis[0];
  if (maxCoord<0)
    maxCoord=-maxCoord;
  for (int i=0; i<coordsInBasis.size; i++)
  { Rational tempRat=(coordsInBasis[i]<0)? -coordsInBasis[i] : coordsInBasis[i];
    if (tempRat>maxCoord)
      maxCoord=tempRat;
  }
  maxCoord+=1;
  output/=maxCoord;
  return true;
}

void Cone::TranslateMeMyLastCoordinateAffinization(Vector<Rational>& theTranslationVector)
{ if(theTranslationVector.size!=this->GetDim()-1)
    crash << crash;
  Vector<Rational> tempRoot;
  for (int i=0; i<this->Normals.size; i++)
  { tempRoot=this->Normals[i];
    tempRoot.size--;
    (*this->Normals[i].LastObject())-= tempRoot.ScalarEuclidean(theTranslationVector);
  }
  tempRoot=theTranslationVector;
  tempRoot.SetSize(theTranslationVector.size+1);
  *tempRoot.LastObject()=0;
  for (int i=0; i<this->Vertices.size; i++)
    if (!this->Vertices[i].LastObject()->IsEqualToZero())
    { Rational tempRat=*this->Vertices[i].LastObject();
      this->Vertices[i]/=tempRat;
      this->Vertices[i]+=tempRoot;
      this->Vertices[i].ScaleByPositiveRationalToIntegralMinHeight();
    }
}

void ConeComplex::GetAllWallsConesNoOrientationNoRepetitionNoSplittingNormals(Vectors<Rational>& output)const
{ HashedList<Vector<Rational> > outputHashed;
  Vector<Rational> tempRoot;
  for (int i=0; i< this->size; i++)
    for (int j=0; j<this->TheObjects[i].Normals.size; j++)
    { tempRoot=this->TheObjects[i].Normals[j];
      tempRoot.ScaleToIntegralMinHeightFirstNonZeroCoordinatePositive();
      outputHashed.AddOnTopNoRepetition(tempRoot);
    }
  output=(outputHashed);
}

void ConeComplex::RefineMakeCommonRefinement(const ConeComplex& other, GlobalVariables& theGlobalVariables)
{ Vectors<Rational> newWalls;
  Cone tempCone=this->ConvexHull;
  if (tempCone.MakeConvexHullOfMeAnd(other.ConvexHull, theGlobalVariables))
  { this->GetAllWallsConesNoOrientationNoRepetitionNoSplittingNormals(newWalls);
    this->init();
    this->ConvexHull=tempCone;
    this->AddNonRefinedChamberOnTopNoRepetition(tempCone, theGlobalVariables);
    this->splittingNormals.AddOnTopNoRepetition(newWalls);
  }
  other.GetAllWallsConesNoOrientationNoRepetitionNoSplittingNormals(newWalls);
  this->splittingNormals.AddOnTopNoRepetition(newWalls);
  this->indexLowestNonRefinedChamber=0;
  this->Refine(theGlobalVariables);
}

void ConeComplex::TranslateMeMyLastCoordinateAffinization(Vector<Rational>& theTranslationVector, GlobalVariables& theGlobalVariables)
{ ConeComplex myCopy;
  myCopy=*this;
  this->init();
  Cone tempCone;
  for (int i=0; i<myCopy.size; i++)
  { tempCone=myCopy[i];
    tempCone.TranslateMeMyLastCoordinateAffinization(theTranslationVector);
    this->AddNonRefinedChamberOnTopNoRepetition(tempCone, theGlobalVariables);
  }
  Vector<Rational> normalNoAffinePart, newNormal;
  for (int j=0; j<myCopy.splittingNormals.size; j++)
  { normalNoAffinePart= myCopy.splittingNormals[j];
    newNormal=normalNoAffinePart;
    normalNoAffinePart.size--;
    (*newNormal.LastObject())-=normalNoAffinePart.ScalarEuclidean(theTranslationVector);
    this->splittingNormals.AddOnTop(newNormal);
  }
}

void PiecewiseQuasipolynomial::TranslateArgument(Vector<Rational>& translateToBeAddedToArgument, GlobalVariables& theGlobalVariables)
{ if(!this->theBuffers!=0)
    crash << crash;
  Vector<Rational> chamberShift=-translateToBeAddedToArgument;
//  stOutput << "the translation: " << translateToBeAddedToArgument.ToString();
  this->theProjectivizedComplex.TranslateMeMyLastCoordinateAffinization(chamberShift, theGlobalVariables);
  QuasiPolynomial tempQP;
  for (int i=0; i<this->theQPs.size; i++)
  { this->theQPs[i].Substitution(-translateToBeAddedToArgument, tempQP, theGlobalVariables);
    this->theQPs[i]=tempQP;
  }
}

std::string PiecewiseQuasipolynomial::ToString(bool useLatex, bool useHtml)
{ std::stringstream out;
  FormatExpressions theFormat;
  for (int i=0; i<this->theProjectivizedComplex.size; i++)
  { const Cone& currentCone= this->theProjectivizedComplex[i];
    QuasiPolynomial& currentQP=this->theQPs[i];
    out << "Chamber number " << i+1;
    if (useHtml)
      out << "<br>";
    out << currentCone.ToString(&theFormat);
    if (useHtml)
      out << "<br>";
    out << "quasipolynomial: ";
    if (useLatex& useHtml)
      out << CGI::GetMathSpanBeginArrayL(currentQP.ToString(useHtml, useLatex));
    else
      out << currentQP.ToString(useHtml, useLatex);
    if (useHtml)
      out << "<hr>";
  }
  return out.str();
}

void PiecewiseQuasipolynomial::DrawMe(DrawingVariables& theDrawingVars, int numLatticePointsPerDim, Cone* RestrictingChamber, Vector<Rational>* distinguishedPoint)
{ FormatExpressions theFormat;
  Vectors<Rational> latticePoints;
  HashedList<Vector<Rational> > theLatticePointsFinal;
  List<int> theLatticePointColors;
  List<int> tempList;
  if (numLatticePointsPerDim<0)
    numLatticePointsPerDim=0;
  int ZeroColor=CGI::RedGreenBlue(200, 200, 200);
  for (int i=0; i<this->theProjectivizedComplex.size; i++)
  { int chamberWallColor=0;
    bool isZeroChamber=this->theQPs[i].IsEqualToZero();
    if (isZeroChamber)
      chamberWallColor= ZeroColor;
    this->theProjectivizedComplex[i].DrawMeLastCoordAffine
    (false, theDrawingVars, theFormat, chamberWallColor);
    std::stringstream tempStream;
    tempStream << i+1;
    Vector<Rational> tempRoot=this->theProjectivizedComplex[i].GetInternalPoint();
    tempRoot.MakeAffineUsingLastCoordinate();
//    theDrawingVars.drawTextAtVectorBuffer
//     (tempRoot, tempStream.str(), chamberWallColor, theDrawingVars.PenStyleNormal, 0);
    for (int j=0; j<this->theQPs[i].LatticeShifts.size; j++)
    { this->theProjectivizedComplex[i].GetLatticePointsInCone
      (this->theQPs[i].AmbientLatticeReduced, this->theQPs[i].LatticeShifts[j], numLatticePointsPerDim, true, latticePoints,
      distinguishedPoint);
      tempList.initFillInObject(latticePoints.size, chamberWallColor);
      if (RestrictingChamber!=0)
        for (int k=0; k<latticePoints.size; k++)
        { tempRoot=latticePoints[k];
          tempRoot.MakeAffineUsingLastCoordinate();
          if (!RestrictingChamber->IsInCone(tempRoot))
            tempList[k]=ZeroColor;
        }
      theLatticePointsFinal.AddOnTop(latticePoints);
      theLatticePointColors.AddListOnTop(tempList);
    }
  }
  for (int i=0; i<theLatticePointsFinal.size; i++)
  { theDrawingVars.drawCircleAtVectorBuffer
    (theLatticePointsFinal[i], 2, theDrawingVars.PenStyleNormal, theLatticePointColors[i]);
    theDrawingVars.drawTextAtVectorBuffer
    (theLatticePointsFinal[i], this->EvaluateInputProjectivized(theLatticePointsFinal[i]).ToString(),
     theLatticePointColors[i],
    DrawingVariables::PenStyleNormal, 0);
  }
}

Rational QuasiPolynomial::Evaluate(const Vector<Rational>& input)
{ Vector<Rational> testLatticeBelonging;
  for (int i=0; i<this->LatticeShifts.size; i++)
  { testLatticeBelonging=this->LatticeShifts[i]-input;
    if (this->AmbientLatticeReduced.IsInLattice(testLatticeBelonging))
      return this->valueOnEachLatticeShift[i].Evaluate(input);
  }
  return 0;
}

Rational PiecewiseQuasipolynomial::Evaluate(const Vector<Rational>& input)
{ if(input.size!=this->theProjectivizedComplex.GetDim()-1)
    crash << crash;
  Vector<Rational> ProjectivizedInput=input;
  ProjectivizedInput.SetSize(input.size+1);
  *ProjectivizedInput.LastObject()=1;
  return this->EvaluateInputProjectivized(ProjectivizedInput);
}

Rational PiecewiseQuasipolynomial::EvaluateInputProjectivized(const Vector<Rational>& input)
{ Rational result;
  if(input.size!=this->theProjectivizedComplex.GetDim())
    crash << crash;
  Vector<Rational> AffineInput=input;
  AffineInput.SetSize(input.size-1);
  int theIndex=this->theProjectivizedComplex.GetLowestIndexchamberContaining(input);
  if (theIndex==-1)
    return 0;
  result=this->theQPs[theIndex].Evaluate(AffineInput);
  //the following for cycle is for self-check purposes only. Comment it out as soon as
  // the code has been tested sufficiently
  for (int i=0; i<this->theProjectivizedComplex.size; i++)
    if (this->theProjectivizedComplex[i].IsInCone(input))
    { Rational altResult=this->theQPs[i].Evaluate(AffineInput);
      if (result!=altResult)
      if (false)
      { static bool firstFail=true;
        if (!firstFail)
          break;
        FormatExpressions tempFormat;
        stOutput << "<hr>Error!!! Failed on chamber " << theIndex+1 << " and " << i+1;
        stOutput << "<br>Evaluating at point " << AffineInput.ToString() << "<br>";
        stOutput << "<br>Chamber " << theIndex+1 << ": " << this->theProjectivizedComplex[theIndex].ToString(&tempFormat);
        stOutput << "<br>QP: " << this->theQPs[theIndex].ToString(true, false);
        stOutput << "<br>value: " << result.ToString();
        stOutput << "<br><br>Chamber " << i+1 << ": " << this->theProjectivizedComplex[i].ToString(&tempFormat);
        stOutput << "<br>QP: " << this->theQPs[i].ToString(true, false);
        stOutput << "<br>value: " << altResult.ToString();
        if (firstFail)
        { DrawingVariables tempDV;
          stOutput << "<br><b>Point of failure: " << AffineInput.ToString() << "</b>";
          //this->DrawMe(tempDV);
          this->theProjectivizedComplex.DrawMeLastCoordAffine(true, tempDV, tempFormat);
          tempDV.NumHtmlGraphics=5;
          tempDV.theBuffer.drawCircleAtVectorBuffer(AffineInput, 5, DrawingVariables::PenStyleNormal, CGI::RedGreenBlue(0,0,0));
          tempDV.theBuffer.drawCircleAtVectorBuffer(AffineInput, 10, DrawingVariables::PenStyleNormal, CGI::RedGreenBlue(0,0,0));
          tempDV.theBuffer.drawCircleAtVectorBuffer(AffineInput, 4, DrawingVariables::PenStyleNormal, CGI::RedGreenBlue(255,0,0));
          stOutput << "<br> <script src=\"http://ajax.googleapis.com/ajax/libs/dojo/1.6.1/dojo/dojo.xd.js\" type=\"text/javascript\"></script>\n";
          stOutput << tempDV.GetHtmlFromDrawOperationsCreateDivWithUniqueName(this->theProjectivizedComplex.GetDim()-1);

        }
        firstFail=false;
      }
//      if(result!=altResult) crash << crash;
    }
  return result;
}

void PiecewiseQuasipolynomial::MakeCommonRefinement(const ConeComplex& other)
{ if(this->theBuffers==0)
    crash << crash;
  GlobalVariables& theGlobalVariables=*this->theBuffers;
  List<QuasiPolynomial> oldQPList=this->theQPs;
  ConeComplex oldComplex=this->theProjectivizedComplex;
  this->theProjectivizedComplex.RefineMakeCommonRefinement(other, theGlobalVariables);
  this->theQPs.SetSize(this->theProjectivizedComplex.size);
  for (int i=0; i<this->theProjectivizedComplex.size; i++)
  { int theOldIndex=oldComplex.GetLowestIndexchamberContaining(this->theProjectivizedComplex[i].GetInternalPoint());
    if (theOldIndex!=-1)
      this->theQPs[i]=oldQPList[theOldIndex];
    else
    //the below needs to be fixed!!!!!
      this->theQPs[i].MakeZeroLatTiceZn(this->GetNumVars());
  }
}

void Lattice::GetDefaultFundamentalDomainInternalPoint(Vector<Rational>& output)
{ output.MakeZero(this->GetDim());
  Vector<Rational> tempRoot;
  for (int i=0; i<this->basisRationalForm.NumRows; i++)
  { this->basisRationalForm.GetVectorFromRow(i, tempRoot);
    output+=tempRoot;
  }
  output/=2;
}

bool PartFractions::split(GlobalVariables& theGlobalVariables, Vector<Rational>* Indicator)
{ //PartFraction::flagAnErrorHasOccurredTimeToPanic=true;
  //this->flagAnErrorHasOccurredTimeToPanic=true;
  if (!this->flagInitialized)
  { this->IndexLowestNonProcessed=0;
    this->PrepareIndicatorVariables();
    this->PrepareCheckSums(theGlobalVariables);
    this->flagInitialized=true;
  }
//  stOutput << "<br>checksum start: " << this->StartCheckSum.ToString();
  if (this->splitPartial(theGlobalVariables, Indicator))
  { //this->ComputeDebugString();
//    this->CompareCheckSums(theGlobalVariables);
    this->RemoveRedundantShortRoots(theGlobalVariables, Indicator);
    //this->ComputeDebugString();
    //this->ComputeDebugString();
    this->CompareCheckSums(theGlobalVariables);
    this->IndexLowestNonProcessed= this->size();
    this->MakeProgressReportSplittingMainPart(theGlobalVariables);
  }
//  stOutput << "<br>checksum finish: " << this->EndCheckSum.ToString();

  return false;
}

void Cone::ChangeBasis(Matrix<Rational>& theLinearMap, GlobalVariables& theGlobalVariables)
{ //Vectors<Rational> newNormals;
//  Matrix<Rational> tempMat=theLinearMap;
  theLinearMap.ActOnVectorsColumn(this->Normals);
  this->CreateFromNormals(this->Normals, &theGlobalVariables);
}

void Cone::TransformToWeylProjective(ConeComplex& owner, GlobalVariables& theGlobalVariables)
{ crash << crash;
/*
  for (int i=0; i<this->Externalwalls.size; i++)
    this->Externalwalls.TheObjects[i].TransformToWeylProjective(theGlobalVariables);
  WallData newWall;
  this->Externalwalls.Reserve(owner.WeylChamber.size+this->Externalwalls.size);
  Vectors<Rational> newExternalWalls;
  owner.GetWeylChamberWallsForCharacterComputation(newExternalWalls);
  for (int i=0; i<newExternalWalls.size; i++)
  { newWall.normal=newExternalWalls.TheObjects[i];
    this->Externalwalls.AddOnTop(newWall);
  }
  this->AllVertices.size=0;
  this->ComputeVerticesFromNormals(owner, theGlobalVariables);*/
}

void ConeComplex::TransformToWeylProjective(GlobalVariables& theGlobalVariables)
{/* this->AmbientWeyl.GetElement().ComputeAllElements();
  this->log << this->AmbientWeyl.GetElement().ToString();
  std::string tempS;
  this->ToString(tempS);
  this->log << "\nWeyl chamber: " << this->WeylChamber.ToString() << "\n";
  this->log << tempS;
  this->NewHyperplanesToSliceWith.size=0;
  this->theHyperplanes.size=0;
  Vector<Rational> wallToSliceWith;
//  Vectors<Rational> oldDirections;
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
          this->theHyperplanes.AddOnTopNoRepetition(wallToSliceWith);
        }
  this->log << "\n Affine walls to slice with:";
  for (int i=0; i<this->NewHyperplanesToSliceWith.size; i++)
    this->log << "\n" << this->NewHyperplanesToSliceWith.TheObjects[i].ToString();
  this->log << "\n";
  this->AmbientDimension=this->AmbientDimension*2+1;
  for (int i=0; i<this->size; i++)
    if (this->TheObjects[i]!=0)
    { this->TheObjects[i]->TransformToWeylProjective(*this, theGlobalVariables);
//      theVertices.AddRootSnoRepetition(this->TheObjects[i]->AllVertices);
    }
  //this->startingCones.initFromDirections()
  Vector<Rational> tempRoot;
  for (int i=0; i<this->TheGlobalConeNormals.size; i++)
  { tempRoot.MakeZero(this->AmbientDimension);
    int startingDim=this->TheGlobalConeNormals.TheObjects[i].size;
    for (int j=0; j<startingDim; j++)
    { tempRoot.TheObjects[j]=this->TheGlobalConeNormals.TheObjects[i].TheObjects[j];
      tempRoot.TheObjects[j+startingDim]=-tempRoot.TheObjects[j];
    }
    this->TheGlobalConeNormals.TheObjects[i]=tempRoot;
  }
  Vectors<Rational> tempRoots;
  this->GetWeylChamberWallsForCharacterComputation(tempRoots);
  this->TheGlobalConeNormals.AddListOnTop(tempRoots);
  this->log << "the global cone normals: " << this->TheGlobalConeNormals.ToString();
  this->ToString(tempS);
  this->log << tempS;
  theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
  theGlobalVariables.theIndicatorVariables.StatusString1=this->log.str();
  theGlobalVariables.MakeReport();*/
}

void ConeLatticeAndShiftMaxComputation::init
(Vector<Rational>& theNEq, Cone& startingCone, Lattice& startingLattice, Vector<Rational>& startingShift)
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
  this->theConesLargerDim.AddOnTop(theCLS);
  this->LPtoMaximizeLargerDim.AddOnTop(theNEq);
  this->IsInfinity.initFillInObject(1, false);
}

void Cone::SliceInDirection
  (Vector<Rational>& theDirection, ConeComplex& output, GlobalVariables& theGlobalVariables)
{ output.init();
  output.AddNonRefinedChamberOnTopNoRepetition(*this, theGlobalVariables);
  output.slicingDirections.AddOnTop(theDirection);
  //stOutput <<output.ToString(false, true);
  output.Refine(theGlobalVariables);
  //stOutput <<output.ToString(false, true);
}

void Lattice::ApplyLinearMap(Matrix<Rational> & theMap, Lattice& output)
{ Vectors<Rational> tempRoots;
  tempRoots.AssignMatrixRows(this->basisRationalForm);
  theMap.ActOnVectorsColumn(tempRoots);
  output.MakeFromRoots(tempRoots);
}

std::string ConeLatticeAndShiftMaxComputation::ToString
  (FormatExpressions* theFormat)
{ std::stringstream out;
  out << "<hr>Resulting lattice: " << this->theFinalRougherLattice.ToString(true, false) << "<hr><hr>";
/*  if (this->complexStartingPerRepresentative.size>0)
  { out << "<hr> Non-refined complex per representative:<br>\n ";
    for (int i=0; i<this->complexStartingPerRepresentative.size; i++)
    { out << "Lattice+shift="  << this->theFinalRepresentatives[i].ToString() << " + " << this->theFinalRougherLattice.ToString();
      out << "<br>\n" << this->complexStartingPerRepresentative[i].ToString(false, true);
      out << "the function we need to max: " << this->LPtoMaximizeSmallerDim[i].ToString();
    }
  }*/
  out << "<hr><hr>Cones not processed(number of cones " << this->theConesLargerDim.size << "):\n<hr>\n";
  DrawingVariables theDrawingVariables;
  Polynomial<Rational>  tempP;
  for (int i=0; i<this->theConesLargerDim.size; i++)
  { out << "";// << this->theConesLargerDim[i].ToString(theFormat);
    //out << "<br>" << this->LPtoMaximizeLargerDim[i].ToString();
    theDrawingVariables.theBuffer.init();
    out << "<br>" << this->theConesLargerDim[i].theProjectivizedCone.DrawMeToHtmlLastCoordAffine(theDrawingVariables, *theFormat);
    out << "<br>over " << this->theConesLargerDim[i].theShift.ToString() << " + " << this->theConesLargerDim[i].theLattice.ToString();
    tempP.MakeLinPolyFromRootLastCoordConst(this->LPtoMaximizeLargerDim[i]);
    out << "<br>the function we have maxed, as a function of the remaining variables, is: " << tempP.ToString(theFormat) << "<hr><hr>";
  }
  if (this->theConesSmallerDim.size>0)
  { out << "<br>Cones processed: <br>";
    for (int i=0; i<this->theConesSmallerDim.size; i++)
    { out << this->theConesSmallerDim[i].ToString(*theFormat);
      //out << "<br>" << this->LPtoMaximizeSmallerDim[i].ToString();
      theDrawingVariables.theBuffer.init();
      out << this->theConesSmallerDim[i].theProjectivizedCone.DrawMeToHtmlLastCoordAffine(theDrawingVariables, *theFormat);
    }
  }
  return out.str();
}

void ConeLatticeAndShiftMaxComputation::FindExtremaParametricStep3
    (Controller& thePauseController, GlobalVariables& theGlobalVariables)
{ this->theFinalRougherLattice=this->theConesLargerDim[0].theLattice;
  ProgressReport theReport(&theGlobalVariables);
  ProgressReport theReport2(&theGlobalVariables);
  for (int i=1; i<this->theConesLargerDim.size; i++)
  { this->theFinalRougherLattice.IntersectWith(this->theConesLargerDim[i].theLattice);
    std::stringstream tempStream;
    tempStream << "intersecing lattice " << i+1 << " out of " << this->theConesLargerDim.size;
    theReport.Report(tempStream.str());
  }
  this->theFinalRepresentatives.size=0;
  Vectors<Rational> tempRoots, tempRoots2;
  tempRoots2.SetSize(1);
  for (int i=0; i<this->theConesLargerDim.size; i++)
  { tempRoots2[0]=this->theConesLargerDim[i].theShift;
    this->theConesLargerDim[i].theLattice.GetAllRepresentativesProjectingDownTo(this->theFinalRougherLattice, tempRoots2, tempRoots);
    this->theFinalRepresentatives.AddOnTopNoRepetition(tempRoots);
    std::stringstream tempStream;
    tempStream << "Computing representative " << i+1 << " out of " << this->theConesLargerDim.size;
    tempStream << "\nSo far " << this->theFinalRepresentatives.size << " found.";
    theReport2.Report(tempStream.str());
  }
  this->complexStartingPerRepresentative.SetSize(this->theFinalRepresentatives.size);
  this->startingLPtoMaximize.SetSize(this->theFinalRepresentatives.size);
  this->finalMaxima.SetSize(this->theFinalRepresentatives.size);
  Vector<Rational> tempRoot;
  for (int i=0; i<this->theFinalRepresentatives.size; i++)
    for (int j=0; j<this->theConesLargerDim.size; j++)
    { tempRoot=this->theFinalRepresentatives[i];
      this->theConesLargerDim[j].theLattice.ReduceVector(tempRoot);
      if (tempRoot==this->theConesLargerDim[j].theShift)
      { this->complexStartingPerRepresentative[i].AddOnTop(this->theConesLargerDim[j].theProjectivizedCone);
        this->startingLPtoMaximize[i].AddOnTop(this->LPtoMaximizeLargerDim[j]);
      }
    }
}

/*void ConeLatticeAndShiftMaxComputation::FindExtremaParametricStep2TrimChamberForMultOne
    (Controller& thePauseController, GlobalVariables& theGlobalVariables)
{ Cone trimmedCone;
  Vectors<Rational> tempRoots;
  Vector<Rational> multFreeWall;
  int startingNumCones=this->theConesLargerDim.size;
  int numKilledCones=0;
  for (int i=0; i<this->theConesLargerDim.size; i++)
  { trimmedCone.Normals=this->theConesLargerDim[i].theProjectivizedCone.Normals;
    multFreeWall=this->LPtoMaximizeLargerDim[i];
    multFreeWall.Minus();
    *multFreeWall.LastObject()+=1;
    trimmedCone.Normals.AddOnTop(multFreeWall);
    trimmedCone.CreateFromNormals(trimmedCone.Normals, theGlobalVariables);
    if (!trimmedCone.flagIsTheZeroCone)
      this->theConesLargerDim[i].theProjectivizedCone=trimmedCone;
    else
    { this->theConesLargerDim.RemoveIndexSwapWithLast(i);
      this->LPtoMaximizeLargerDim.RemoveIndexSwapWithLast(i);
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
  ProgressReport theReport(&theGlobalVariables);
  for (int i=0; i<this->theFinalRepresentatives.size; i++)
  { ConeComplex& currentComplex= this->complexRefinedPerRepresentative[i];
    currentComplex.initFromCones(this->complexStartingPerRepresentative[i], true, theGlobalVariables);
    std::stringstream tempStream;
    tempStream << "Processing representative " << i+1 << " out of " << this->theFinalRepresentatives.size;
    theReport.Report(tempStream.str());
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
    { const Cone& currentCone=this->complexRefinedPerRepresentative[i][j];
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
{ //stOutput << "<hr>starting complex: " << this->ToString();
  FormatExpressions tempFormat;
  ProgressReport theReport1(&theGlobalVariables);
  ProgressReport theReport2(&theGlobalVariables);
  ProgressReport theReport3(&theGlobalVariables);
  for (; this->numProcessedNonParam<this->numNonParaM; this->numProcessedNonParam++)
  { while(this->theConesLargerDim.size>0)
    { ConeLatticeAndShift& currentCLS=*this->theConesLargerDim.LastObject();
      if(this->LPtoMaximizeLargerDim.LastObject()->size!=currentCLS.GetDimAffine()+1)
        crash << crash;
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
      theReport1.Report(tempStream1.str());
      theReport2.Report(tempStream2.str());
      theReport3.Report(tempStream3.str());
    }
    //stOutput << "<hr><hr>" << this->ToString();
    this->LPtoMaximizeLargerDim=this->LPtoMaximizeSmallerDim;
    this->theConesLargerDim=this->theConesSmallerDim;
    this->theConesSmallerDim.size=0;
    this->LPtoMaximizeSmallerDim.size=0;
  }
}

void ConeLatticeAndShift::FindExtremaInDirectionOverLatticeOneNonParamDegenerateCase
(Vector<Rational>& theLPToMaximizeAffine, Vectors<Rational>& outputAppendLPToMaximizeAffine,
 List<ConeLatticeAndShift>& outputAppend, Matrix<Rational> & theProjectionLatticeLevel,
 GlobalVariables& theGlobalVariables)
{ Matrix<Rational> matVertices;
  matVertices.AssignVectorsToRows(this->theProjectivizedCone.Vertices);
  Vectors<Rational> theNormals;
  matVertices.GetZeroEigenSpaceModifyMe(theNormals);
  Vector<Rational> preferredNormal;
  for (int i=0; i<theNormals.size; i++)
    if (!theNormals[i][0].IsEqualToZero())
    { preferredNormal=theNormals[i];
      break;
    }
  Rational firstCoord=preferredNormal[0];
  preferredNormal.ShiftToTheLeftOnePos();
  preferredNormal/=-firstCoord;
  ConeLatticeAndShift tempCLS;
  Vectors<Rational> newNormals=this->theProjectivizedCone.Normals;
  Rational firstCoordNewNormal, tempRat;
  for (int i=0; i<newNormals.size; i++)
  { firstCoordNewNormal=newNormals[i][0];
    newNormals[i].ShiftToTheLeftOnePos();
    newNormals[i]+=preferredNormal*firstCoordNewNormal;
  }
  //bool tempBool=
  tempCLS.theProjectivizedCone.CreateFromNormals(newNormals, &theGlobalVariables);
  tempCLS.theShift=this->theShift;
  tempCLS.theShift.ShiftToTheLeftOnePos();
  this->theLattice.ApplyLinearMap(theProjectionLatticeLevel, tempCLS.theLattice);
  Vector<Rational> tempRoot;
  tempRoot=theLPToMaximizeAffine.GetShiftToTheLeftOnePosition();
  //Vector<Rational> tempRoot2;
  //tempRoot2=preferredNormal;
  tempRoot+=(preferredNormal*theLPToMaximizeAffine[0]);
  if (!tempCLS.theProjectivizedCone.flagIsTheZeroCone)
  { outputAppend.AddOnTop(tempCLS);
    outputAppendLPToMaximizeAffine.AddOnTop(tempRoot);
  }
}

void ConeLatticeAndShift::FindExtremaInDirectionOverLatticeOneNonParam
(Vector<Rational>& theLPToMaximizeAffine, Vectors<Rational>& outputAppendLPToMaximizeAffine,
 List<ConeLatticeAndShift>& outputAppend,
 GlobalVariables& theGlobalVariables)
{ Vector<Rational> direction;
  FormatExpressions theFormat;
  int theDimProjectivized=this->GetDimProjectivized();
  Matrix<Rational>  theProjectionLatticeLevel;
  Matrix<Rational>  theProjectionAffine;
  theProjectionLatticeLevel.init(theDimProjectivized-2, theDimProjectivized-1);
  theProjectionLatticeLevel.MakeZero();
  for (int i=0; i<theProjectionLatticeLevel.NumRows; i++)
    theProjectionLatticeLevel.elements[i][i+1]=1;
  direction.MakeEi(theDimProjectivized, 0);
  if (!this->theProjectivizedCone.Vertices.LinSpanContainsVector(direction))
  { this->FindExtremaInDirectionOverLatticeOneNonParamDegenerateCase
      (theLPToMaximizeAffine, outputAppendLPToMaximizeAffine, outputAppend, theProjectionLatticeLevel,
       theGlobalVariables);
    return;
  }
  ProgressReport theReport(&theGlobalVariables);
  if (outputAppend.size>=10)
  { std::stringstream tempStream;
    tempStream << "<hr><hr><hr><hr>The bad cone:" << this->theProjectivizedCone.ToString(&theFormat);
    theReport.Report(tempStream.str());
  }
  ConeComplex complexBeforeProjection;
  complexBeforeProjection.init();
  complexBeforeProjection.AddNonRefinedChamberOnTopNoRepetition(this->theProjectivizedCone, theGlobalVariables);
  if (direction.ScalarEuclidean(theLPToMaximizeAffine).IsNegative())
    direction.Minus();
  complexBeforeProjection.slicingDirections.AddOnTop(direction);
  complexBeforeProjection.slicingDirections.AddOnTop(-direction);
//  stOutput << "<hr>complex before refining: <br>\n" << complexBeforeProjection.ToString(false, true);
  complexBeforeProjection.Refine(theGlobalVariables);
//  stOutput << "<hr>complex before projection: <br>\n" << complexBeforeProjection.ToString(false, true);
  Vector<Rational> tempRoot, extraEquation, exitNormalAffine, enteringNormalAffine, exitNormalLatticeLevel, enteringNormalLatticeLevel, exitNormalShiftedAffineProjected;
  Vector<Rational> directionSmallerDim, directionSmallerDimOnLattice;
  Vector<Rational> theShiftReduced=this->theShift;
  this->theLattice.ReduceVector(theShiftReduced);
  Vectors<Rational> exitRepresentatives, exitWallsShifted;
  Vectors<Rational> currentShifts;
  Lattice exitRougherLattice;
  ConeLatticeAndShift tempCLS;
  directionSmallerDim.MakeEi(theDimProjectivized-1, 0);
//  stOutput << "<hr>";
  Vectors<Rational> theNewNormals;
  for (int i=0; i<complexBeforeProjection.size; i++)
  { Cone& currentCone=complexBeforeProjection.TheObjects[i];
    int numNonPerpWalls=0;
    theNewNormals.size=0;
    bool foundEnteringNormal=false;
    bool foundExitNormal=false;
    for (int j=0; j<currentCone.Normals.size; j++)
    { Vector<Rational>& currentNormal=currentCone.Normals.TheObjects[j];
      if (currentNormal[0].IsEqualToZero())
      { tempRoot=currentNormal.GetShiftToTheLeftOnePosition();
        theNewNormals.AddOnTop(tempRoot);
      } else
      { //stOutput << "<hr>";
        //stOutput << "<br>currentWall: " << currentNormal.ToString();
        numNonPerpWalls++;
        if(numNonPerpWalls>=3)
          crash << crash;
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
    stOutput << "<hr><hr><hr>" << currentCone.ToString(&theFormat);
    stOutput << "<br>Entering normal: " << ((foundEnteringNormal) ? enteringNormalAffine.ToString() : "not found");
    stOutput << "<br>Exit normal: " << ((foundExitNormal) ? exitNormalAffine.ToString() : "not found");
    stOutput << "<br>The shifted lattice representatives: " << exitRepresentatives.ToString() << "<br>exitNormalsShiftedAffineProjected";
    if(theNewNormals.size<=0)
      crash << crash;
    for (int j=0; j<exitRepresentatives.size; j++)
    { tempCLS.theProjectivizedCone.Normals=theNewNormals;
      exitNormalShiftedAffineProjected=exitNormalAffine.GetShiftToTheLeftOnePosition();
      *exitNormalShiftedAffineProjected.LastObject()=-exitNormalLatticeLevel.ScalarEuclidean(exitRepresentatives[j]);
      stOutput << exitNormalShiftedAffineProjected.ToString() << ", ";
      if (foundEnteringNormal)
      { extraEquation= enteringNormalAffine.GetShiftToTheLeftOnePosition();
        extraEquation-=(exitNormalShiftedAffineProjected*enteringNormalAffine[0])/exitNormalAffine[0];
        stOutput << "extra equation: " << extraEquation.ToString() << ", ";
        tempCLS.theProjectivizedCone.Normals.AddOnTop(extraEquation);
      }
      Rational tempRat=theLPToMaximizeAffine[0];
      tempRoot=theLPToMaximizeAffine.GetShiftToTheLeftOnePosition();
      tempRoot-=exitNormalShiftedAffineProjected*theLPToMaximizeAffine[0]/exitNormalAffine[0];
      outputAppendLPToMaximizeAffine.AddOnTop(tempRoot);
      if(tempCLS.theProjectivizedCone.Normals.size<=0)
        crash << crash;
      Vectors<Rational> tempTempRoots=tempCLS.theProjectivizedCone.Normals;
      //bool tempBool=
      tempCLS.theProjectivizedCone.CreateFromNormals(tempTempRoots, &theGlobalVariables);
      /*if (!tempBool)
      { std::stringstream tempStream;
        tempStream << "The bad starting cone (cone number " << i+1 << "):" << this->ToString(theFormat) << "<hr><hr><hr><hr>The bad cone:" << tempCLS.ToString(theFormat);
        tempStream << "<br>\n\n" << this->theProjectivizedCone.Normals.ToString(false, false, false);
        tempStream << "\n\n<br>\n\n" << complexBeforeProjection.ToString(false, true);
        if (!foundEnteringNormal)
          tempStream << "<hr>not found entering normal!!!!!!<hr>";
        if (!foundExitNormal)
          tempStream << "<hr>not found exit normal!!!!!!<hr>";
        Cone tempCone;
        tempCone.CreateFromNormals(tempTempRoots, theGlobalVariables);
        tempStream << "\n\n\n\n<br><br><hr>The bad normals: " << tempTempRoots.ToString();
        tempStream << "\n\n\n\n<br><br><hr>The bad normals after creation: " << tempCLS.theProjectivizedCone.Normals.ToString();
        theGlobalVariables.theIndicatorVariables.StatusString1=tempStream.str();
        theGlobalVariables.theIndicatorVariables.StatusString1NeedsRefresh=true;
        theGlobalVariables.MakeReport();
        for (int i=0; i<10000000; i++)
          if (i%3==0)
            i=i+2;
        while(true){}
      }*/
      //if(!tempBool)crash << crash;
      //stOutput << tempCLS.theProjectivizedCone.ToString(false, true, true, true, theFormat);
      if (!tempCLS.theProjectivizedCone.flagIsTheZeroCone)
      { theProjectionLatticeLevel.ActOnVectorColumn(exitRepresentatives[j], tempCLS.theShift);
        outputAppend.AddOnTop(tempCLS);
        if (tempCLS.GetDimProjectivized()==0)
        { theReport.Report(tempTempRoots.ToString());
          while(true) {}
        }
        if(tempCLS.GetDimProjectivized()!=theDimProjectivized-1)
          crash << crash;
      }
    }
  }
//  stOutput << "<hr><hr><hr>";
//  for (int i=0; i<outputAppend.size; i++)
//  { stOutput << outputAppend[i].theProjectivizedCone.ToString(false, true, true, true);
//  }
}

void ConeComplex::GetNewVerticesAppend
  (Cone& myDyingCone, Vector<Rational>& killerNormal, HashedList<Vector<Rational> >& outputVertices, GlobalVariables& theGlobalVariables)
{ int theDimMinusTwo=killerNormal.size-2;
  int theDim=killerNormal.size;
  int numCycles=MathRoutines::NChooseK(myDyingCone.Normals.size, theDimMinusTwo);
  Selection theSel=theGlobalVariables.selGetNewVerticesAppend.GetElement();
  Selection nonPivotPoints=theGlobalVariables.selGetNewVerticesAppend2.GetElement();
  theSel.init(myDyingCone.Normals.size);
//  int IndexLastZeroWithOneBefore, NumOnesAfterLastZeroWithOneBefore;
  Matrix<Rational>& theLinearAlgebra=theGlobalVariables.matGetNewVerticesAppend.GetElement();
  theLinearAlgebra.init(theDimMinusTwo+1, theDim);
  Vector<Rational> tempRoot;
  for (int i=0; i<numCycles; i++)
  { theSel.incrementSelectionFixedCardinality(theDimMinusTwo);//, IndexLastZeroWithOneBefore, NumOnesAfterLastZeroWithOneBefore);
    //int LowestRowUnchanged=theDimMinusTwo-2-NumOnesAfterLastZeroWithOneBefore;
    //for (int j=theDimMinusTwo-1; j>LowestRowUnchanged; j--)
    for (int j=0; j<theDimMinusTwo; j++)
    { Vector<Rational>& currentNormal=myDyingCone.Normals.TheObjects[theSel.elements[j]];
      for (int k=0; k<theDim; k++)
        theLinearAlgebra.elements[j][k]=currentNormal.TheObjects[k];
    }
    for (int k=0; k<theDim; k++)
      theLinearAlgebra.elements[theDimMinusTwo][k]=killerNormal.TheObjects[k];
    theLinearAlgebra.GaussianEliminationByRows(0, &nonPivotPoints);
    if (nonPivotPoints.CardinalitySelection==1)
    { theLinearAlgebra.NonPivotPointsToEigenVector(nonPivotPoints, tempRoot, (Rational) 1, (Rational) 0);
      tempRoot.ScaleByPositiveRationalToIntegralMinHeight();
      if (myDyingCone.IsInCone(tempRoot))
        outputVertices.AddOnTopNoRepetition(tempRoot);
      else
      { tempRoot.Minus();
        if (myDyingCone.IsInCone(tempRoot))
          outputVertices.AddOnTopNoRepetition(tempRoot);
      }
    }
  }
}

bool ConeComplex::SplitChamber
(int indexChamberBeingRefined, bool weAreSlicingInDirection, bool weAreChopping, Vector<Rational>& killerNormal, GlobalVariables& theGlobalVariables)
{ Cone& myDyingCone=this->TheObjects[indexChamberBeingRefined];
/*  if (!myDyingCone.flagHasSufficientlyManyVertices)
  { this->flagChambersHaveTooFewVertices=true;
    return false;
  }*/
  Cone newPlusCone, newMinusCone;
  Matrix<Rational>& bufferMat=theGlobalVariables.matSplitChamberBuff.GetElement();
  Selection& bufferSel=theGlobalVariables.selSplitChamber.GetElement();
  bool needToRecomputeVertices=
  (myDyingCone.Normals.GetRankOfSpanOfElements(&bufferMat, &bufferSel)<this->GetDim());
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
  HashedList<Vector<Rational> >& ZeroVertices=theGlobalVariables.hashedRootsNewChamberSplit.GetElement();
  ZeroVertices.Clear();
  Rational tempRat;
  for (int i=0; i<myDyingCone.Vertices.size; i++)
  { killerNormal.ScalarEuclidean(myDyingCone.Vertices[i], tempRat);
    if (tempRat.IsPositive())
      newPlusCone.Vertices.AddOnTop(myDyingCone.Vertices[i]);
    if (tempRat.IsEqualToZero())
      ZeroVertices.AddOnTopNoRepetition(myDyingCone.Vertices.TheObjects[i]);
    if (tempRat.IsNegative())
      newMinusCone.Vertices.AddOnTop(myDyingCone.Vertices.TheObjects[i]);
  }
  if (newPlusCone.Vertices.size==0 || newMinusCone.Vertices.size==0)
    return false;
  this->GetNewVerticesAppend(myDyingCone, killerNormal, ZeroVertices, theGlobalVariables);
  for (int i=0; i<myDyingCone.Normals.size; i++)
  { if (newPlusCone.Vertices.HasAnElementPerpendicularTo(myDyingCone.Normals[i]))
      newPlusCone.Normals.AddOnTop(myDyingCone.Normals[i]);
    if (newMinusCone.Vertices.HasAnElementPerpendicularTo(myDyingCone.Normals[i]))
      newMinusCone.Normals.AddOnTop(myDyingCone.Normals[i]);
  }
  newPlusCone.Normals.AddOnTop(killerNormal);
  newMinusCone.Normals.AddOnTop(-killerNormal);
  newPlusCone.Vertices.AddListOnTop(ZeroVertices);
  newMinusCone.Vertices.AddListOnTop(ZeroVertices);
/*  Cone tempCone;
  tempCone.CreateFromNormals(newPlusCone.Normals, theGlobalVariables);
  if(tempCone.Vertices.size!=newPlusCone.Vertices.size)crash << crash;
  tempCone.CreateFromNormals(newMinusCone.Normals, theGlobalVariables);
  if(tempCone.Vertices.size!=newMinusCone.Vertices.size)crash << crash;
*/
  this->PopChamberSwapWithLast(indexChamberBeingRefined);
  if (needToRecomputeVertices)
  { newPlusCone.CreateFromNormals(newPlusCone.Normals, &theGlobalVariables);
    newMinusCone.CreateFromNormals(newMinusCone.Normals, &theGlobalVariables);
  }
  this->AddNonRefinedChamberOnTopNoRepetition(newPlusCone, theGlobalVariables);
  this->AddNonRefinedChamberOnTopNoRepetition(newMinusCone, theGlobalVariables);
  return true;
}

void ConeComplex::PopChamberSwapWithLast(int index)
{ this->RemoveIndexSwapWithLast(index);
}

bool Cone::MakeConvexHullOfMeAnd(const Cone& other, GlobalVariables& theGlobalVariables)
{ if (this->IsInCone(other.Vertices))
    return false;
  if (other.flagIsTheZeroCone)
    return false;
  if (this->flagIsTheZeroCone)
  { this->operator=(other);
    return true;
  }
  Vectors<Rational> newVertices;
  newVertices.AddListOnTop(other.Vertices);
  newVertices.AddListOnTop(this->Vertices);
  this->CreateFromVertices(newVertices, &theGlobalVariables);
  return true;
}

bool ConeComplex::AddNonRefinedChamberOnTopNoRepetition(const Cone& newCone, GlobalVariables& theGlobalVariables)
{ Cone theConeSorted;
  theConeSorted=newCone;
  theConeSorted.Normals.QuickSortAscending();
  this->ConvexHull.MakeConvexHullOfMeAnd(theConeSorted, theGlobalVariables);
  return this->AddOnTopNoRepetition(theConeSorted);
}

void ConeComplex::RefineOneStep(GlobalVariables& theGlobalVariables)
{ if (this->indexLowestNonRefinedChamber>=this->size)
    return;
  Cone& currentCone=this->TheObjects[this->indexLowestNonRefinedChamber];
  for (; currentCone.LowestIndexNotCheckedForChopping<this->splittingNormals.size; currentCone.LowestIndexNotCheckedForChopping++)
   if (this->SplitChamber(this->indexLowestNonRefinedChamber, false, true, this->splittingNormals.TheObjects[currentCone.LowestIndexNotCheckedForChopping], theGlobalVariables))
      return;
  Vector<Rational> currentNewWall;
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

void ConeComplex::InitFromDirectionsAndRefine(Vectors<Rational>& inputVectors, GlobalVariables& theGlobalVariables)
{ this->init();
  Cone startingCone;
  startingCone.CreateFromVertices(inputVectors, &theGlobalVariables);
  this->AddNonRefinedChamberOnTopNoRepetition(startingCone, theGlobalVariables);
  this->slicingDirections.AddListOnTop(inputVectors);
  this->Refine(theGlobalVariables);
}

void ConeComplex::Sort(GlobalVariables& theGlobalVariables)
{ List<Cone> tempList;
  tempList=*this;
  tempList.QuickSortAscending();
  this->Clear();
  for (int i=0; i<tempList.size; i++)
    this->AddOnTop(tempList[i]);
}

void ConeComplex::RefineAndSort(GlobalVariables& theGlobalVariables)
{ this->Refine(theGlobalVariables);
  this->Sort(theGlobalVariables);
}

void ConeComplex::Refine(GlobalVariables& theGlobalVariables)
{ ProgressReport theReport(&theGlobalVariables);
  while (this->indexLowestNonRefinedChamber<this->size)
  { this->RefineOneStep(theGlobalVariables);
    std::stringstream out;
    out << "Refined " << this->indexLowestNonRefinedChamber << " out of " << this->size;
    theReport.Report(out.str());
  }
}

void Cone::ComputeVerticesFromNormalsNoFakeVertices(GlobalVariables* theGlobalVariables)
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
  Matrix<Rational> theMat;
  Vector<Rational> tempRoot;
  theMat.init(theDim-1, theDim);
  for (int i=0; i<numCycles; i++)
  { theSel.incrementSelectionFixedCardinality(theDim-1);
    for (int j=0; j<theSel.CardinalitySelection; j++)
      for (int k=0; k<theDim; k++)
        theMat.elements[j][k]=this->Normals.TheObjects[theSel.elements[j]].TheObjects[k];
    theMat.GaussianEliminationByRows(0, &nonPivotPoints);
    if (nonPivotPoints.CardinalitySelection==1)
    { theMat.NonPivotPointsToEigenVector(nonPivotPoints, tempRoot);
      bool tempBool=this->IsInCone(tempRoot);
      if (!tempBool)
      { tempRoot.Minus();
        tempBool=this->IsInCone(tempRoot);
      }
      if (tempBool)
      { tempRoot.ScaleByPositiveRationalToIntegralMinHeight();
        this->Vertices.AddOnTopNoRepetition(tempRoot);
      }
    }
  }
}

bool Cone::EliminateFakeNormalsUsingVertices(int theDiM, int numAddedFakeWalls, GlobalVariables* theGlobalVariables)
{ if(this->Vertices.size==0)
  { this->flagIsTheZeroCone=true;
    this->Normals.SetSize(0);
    return false;
  }
  Vectors<Rational> verticesOnWall;
  if (numAddedFakeWalls!=0)
  { //we modify the normals so that they lie in the subspace spanned by the vertices
    Matrix<Rational> tempMat, matNormals, gramMatrixInverted;
    tempMat.AssignVectorsToRows(this->Vertices);
    Vectors<Rational> NormalsToSubspace;
    tempMat.GetZeroEigenSpaceModifyMe(NormalsToSubspace);
    if (NormalsToSubspace.size>0)
    { matNormals.AssignVectorsToRows(NormalsToSubspace);
//      stOutput << "<br>Normals to the subspace spanned by the vertices: " << NormalsToSubspace.ToString();
      gramMatrixInverted=matNormals;
      gramMatrixInverted.Transpose();
      gramMatrixInverted.MultiplyOnTheLeft(matNormals);
      gramMatrixInverted.Invert(theGlobalVariables);
      Vector<Rational> tempRoot;
      for (int i=0; i<this->Normals.size; i++)
      { matNormals.ActOnVectorColumn(this->Normals[i], tempRoot);
        gramMatrixInverted.ActOnVectorColumn(tempRoot);
        for (int j=0; j<tempRoot.size; j++)
          this->Normals[i]-=NormalsToSubspace[j]*tempRoot[j];
        this->Normals[i].ScaleByPositiveRationalToIntegralMinHeight();
        if (this->Normals[i].IsEqualToZero())
        { this->Normals.RemoveIndexSwapWithLast(i);
          i--;
        }
      }
      //all normals should now lie in the subspace spanned by the vertices
      //add the walls needed to go down to the subspace
      this->Normals.ReservE(this->Normals.size+2*NormalsToSubspace.size);
      for (int i=0; i<NormalsToSubspace.size; i++)
      { NormalsToSubspace[i].ScaleByPositiveRationalToIntegralMinHeight();
        this->Normals.AddOnTop(NormalsToSubspace[i]);
        this->Normals.AddOnTop(-NormalsToSubspace[i]);
      }
    }
  }
  Matrix<Rational> tempMatX;
  Selection tempSelX;
  int DesiredRank=this->Vertices.GetRankOfSpanOfElements(&tempMatX, &tempSelX);
  if (DesiredRank>1)
    for (int i=0; i<this->Normals.size; i++)
    { Vector<Rational>& currentNormal=this->Normals[i];
      verticesOnWall.size=0;
      bool wallIsGood=false;
      for (int j=0; j<this->Vertices.size; j++)
        if (currentNormal.ScalarEuclidean(this->Vertices[j]).IsEqualToZero())
        { verticesOnWall.AddOnTop(this->Vertices[j]);
          int theRank=verticesOnWall.GetRankOfSpanOfElements(&tempMatX, &tempSelX);
          if (theRank< verticesOnWall.size)
            verticesOnWall.RemoveLastObject();
          else
            if (theRank==DesiredRank-1)
            { wallIsGood=true;
              break;
            }
        }
      if (!wallIsGood)
      { this->Normals.RemoveIndexSwapWithLast(i);
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
      crash << crash;
  for (int i=0; i<this->Normals.size; i++)
    if (!this->Vertices.HasAnElementWithPositiveScalarProduct(this->Normals.TheObjects[i]))
      return false;
  return numAddedFakeWalls==0;
}

bool Cone::ProduceNormalFromTwoNormalsAndSlicingDirection(Vector<Rational>& SlicingDirection, Vector<Rational>& normal1, Vector<Rational>& normal2, Vector<Rational>& output)
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

bool Cone::CreateFromVertices(const Vectors<Rational>& inputVertices, GlobalVariables* theGlobalVariables)
{ this->LowestIndexNotCheckedForChopping=0;
  this->LowestIndexNotCheckedForSlicingInDirection=0;
 // stOutput << inputVertices.ToString();
  this->flagIsTheZeroCone=false;
  if (inputVertices.size<=0)
  { this->Normals.size=0;
    this->Vertices.size=0;
    this->flagIsTheZeroCone=true;
    return false;
  }
  this->Normals.size=0;
  Matrix<Rational> tempMat;
  Selection tempSel;
  int rankVerticesSpan=inputVertices.GetRankOfSpanOfElements(&tempMat, &tempSel);
  int theDim=inputVertices.GetDim();
  Vectors<Rational> extraVertices;
  extraVertices.SetSize(0);
  if (rankVerticesSpan<theDim)
  { Matrix<Rational>  tempMat;
    tempMat.AssignVectorsToRows(inputVertices);
    tempMat.GetZeroEigenSpace(extraVertices);
    for (int i=0; i<extraVertices.size; i++)
    { this->Normals.AddOnTop(extraVertices[i]);
      this->Normals.AddOnTop(-extraVertices[i]);
    }
  }
  int NumCandidates = MathRoutines::NChooseK(inputVertices.size, rankVerticesSpan-1);
  Selection theSelection;
  theSelection.init(inputVertices.size);
  Vector<Rational> normalCandidate;
  for (int i=0; i<NumCandidates; i++)
  { theSelection.incrementSelectionFixedCardinality(rankVerticesSpan-1);
    for (int j=0; j<theSelection.CardinalitySelection; j++)
      extraVertices.AddOnTop(inputVertices[theSelection.elements[j]]);
    if (extraVertices.ComputeNormal(normalCandidate, theDim))
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
        normalCandidate.Minus();
      if (!(hasNegative && hasPositive))
        this->Normals.AddOnTopNoRepetition(normalCandidate);
    }
    extraVertices.size=theDim-rankVerticesSpan;
  }
//  stOutput << "<br>Candidate normals: " << this->Normals.ToString();
  return this->CreateFromNormals(this->Normals, theGlobalVariables);
}

bool Cone::CreateFromNormalS(Vectors<Rational>& inputNormals, bool UseWithExtremeMathCautionAssumeConeHasSufficientlyManyProjectiveVertices, GlobalVariables* theGlobalVariables)
{ this->flagIsTheZeroCone=false;
  this->LowestIndexNotCheckedForChopping=0;
  this->LowestIndexNotCheckedForSlicingInDirection=0;
  int theDim=1;
  if (inputNormals.size>0)
    theDim=inputNormals[0].size;
  this->Normals=inputNormals;
  for (int i=0; i<this->Normals.size; i++)
    if (this->Normals[i].IsEqualToZero())
    { this->Normals.RemoveIndexSwapWithLast(i);
      i--;
    }
  int numAddedFakeWalls=0;
  Matrix<Rational> tempMat;
  Selection tempSel;
  if (!UseWithExtremeMathCautionAssumeConeHasSufficientlyManyProjectiveVertices)
    for (int i=0; i<theDim && this->Normals.GetRankOfSpanOfElements(&tempMat, &tempSel)<theDim; i++)
    { Vector<Rational> tempRoot;
      tempRoot.MakeEi(theDim, i);
      if(!this->Normals.LinSpanContainsVector(tempRoot, tempMat, tempSel))
      { numAddedFakeWalls++;
        this->Normals.AddOnTop(tempRoot);
      }
    }
//  stOutput << "<br>Normals (" << inputNormals.size << " input " << numAddedFakeWalls << " fake): " << this->Normals.ToString();
  this->ComputeVerticesFromNormalsNoFakeVertices(theGlobalVariables);
//  stOutput << "<br>Vertices before adding minus vertices: " << this->Vertices.ToString();
  if (numAddedFakeWalls>0)
  { this->Normals.SetSize(this->Normals.size-numAddedFakeWalls);
    Vector<Rational> tempRoot;
    int originalSize=this->Vertices.size;
    for (int i=0; i<originalSize; i++)
    { tempRoot=-this->Vertices[i];
      if (this->IsInCone(tempRoot))
        this->Vertices.AddOnTopNoRepetition(tempRoot);
    }
  }
//  stOutput << "<br>Vertices: " << this->Vertices.ToString();
  return this->EliminateFakeNormalsUsingVertices(theDim, numAddedFakeWalls, theGlobalVariables);
}

void ConeComplex::initFromCones(List<Cone>& NormalsOfCones, bool AssumeConesHaveSufficientlyManyProjectiveVertices, GlobalVariables& theGlobalVariables)
{ List<Vectors<Rational> > tempRoots;
  tempRoots.SetSize(NormalsOfCones.size);
  for (int i=0; i<NormalsOfCones.size; i++)
    tempRoots[i]=NormalsOfCones[i].Normals;
  this->initFromCones(tempRoots, AssumeConesHaveSufficientlyManyProjectiveVertices, theGlobalVariables);
}

void ConeComplex::initFromCones
(List<Vectors<Rational> >& NormalsOfCones, bool UseWithExtremeMathCautionAssumeConeHasSufficientlyManyProjectiveVertices, GlobalVariables& theGlobalVariables)
{ Cone tempCone;
  this->Clear();
  ProgressReport theReport(&theGlobalVariables);
  theReport.Report(NormalsOfCones.ToString());
//  for (int i=0; i<10000000; i++){int j=i*i*i;}
  for (int i=0; i<NormalsOfCones.size; i++)
  { if (tempCone.CreateFromNormalS(NormalsOfCones[i], UseWithExtremeMathCautionAssumeConeHasSufficientlyManyProjectiveVertices, &theGlobalVariables))
      this->AddNonRefinedChamberOnTopNoRepetition(tempCone, theGlobalVariables);
    std::stringstream out;
    out << "Initializing cone " << i+1 << " out of " << NormalsOfCones.size;
    theReport.Report(out.str());
  }
  Vector<Rational> tempRoot;
  this->splittingNormals.Clear();
  for (int i=0; i<this->size; i++)
    for (int j=0; j<this->TheObjects[i].Normals.size; j++)
    { tempRoot=this->TheObjects[i].Normals[j];
      tempRoot.ScaleToIntegralMinHeightFirstNonZeroCoordinatePositive();
      this->splittingNormals.AddOnTopNoRepetition(tempRoot);
      std::stringstream out;
      out << "Extracting walls from cone " << i+1 << " out of " << this->size << " total distinct chambers.";
      out << "\nProcessed " << j+1 << " out of " << this->TheObjects[i].Normals.size << " walls of the current chamber.";
      out << "\nTotal # of distinct walls found: " << this->splittingNormals.size;
      theReport.Report(out.str());
    }
}

std::string Cone::ToString(FormatExpressions* theFormat)const
{ std::stringstream out;
  bool PrepareMathReport= theFormat==0 ? false: theFormat->flagUseLatex;
  bool useHtml= theFormat==0 ? false: theFormat->flagUseHTML;
  bool useLatex= theFormat==0 ? false: theFormat->flagUseLatex;
  bool lastVarIsConstant=false;
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
  FormatExpressions tempF;
  if (theFormat==0)
    theFormat=&tempF;
  out << this->Normals.ElementsToInequalitiesString(useLatex, useHtml, lastVarIsConstant, *theFormat);
  if (useLatex)
    out << "\\]";
  out << "\nProjectivized Vertices: " << this->Vertices.ToString();
  if (useHtml)
    out << "<br>";
  if (this->Vertices.size>0)
    out << "\nInternal point: " << this->GetInternalPoint().ToString();
  return out.str();
}

std::string ConeComplex::ToString(bool useLatex, bool useHtml)
{ std::stringstream out;
  FormatExpressions theFormat;
  out << "Number of chambers: " << this->size;
  if (useHtml)
    out << "<br>";
  out << " Next non-refined chamber: " << this->indexLowestNonRefinedChamber+1;
  if (useHtml)
    out << "<br>";
  out << "Normals of walls to refine by: ";
  Vectors<Rational> tempRoots;
  tempRoots=(this->splittingNormals);
  out << tempRoots.ToString(&theFormat);
  if (this->slicingDirections.size>0)
  { if (useHtml)
      out << "<br>\n";
    out << " Directions to slice along: " << this->slicingDirections.ToString();
  }
  for (int i=0; i<this->size; i++)
  { if (useHtml)
      out << "<hr>";
    out << "\n\n\nChamber " << i+1 << ":\n";
    if (useHtml)
      out << "<br>";
    out << this->TheObjects[i].ToString(&theFormat) << "\n\n\n";
  }
  return out.str();
}

int RationalFunctionOld::GetMinNumVars()const
{ switch(this->expressionType)
  { case RationalFunctionOld::typeRational:
      return 0;
    case RationalFunctionOld::typePoly:
      return this->Numerator.GetElementConst().GetMinNumVars();
    case RationalFunctionOld::typeRationalFunction:
      return MathRoutines::Maximum(this->Numerator.GetElementConst().GetMinNumVars(), this->Denominator.GetElementConst().GetMinNumVars());
    default: //this should never happen! maybe crash << crash here...
      return -1;
  }
}

void RationalFunctionOld::GetRelations(List<Polynomial<Rational> >& theGenerators, GlobalVariables& theGlobalVariables)
{ if (theGenerators.size==0)
    return;
  List<Polynomial<Rational> > theGroebnerBasis;
  theGroebnerBasis=theGenerators;
  int numStartingGenerators=theGenerators.size;

  int numStartingVariables=0;
  for (int i=0; i<theGenerators.size; i++)
    numStartingVariables=MathRoutines::Maximum(numStartingVariables, theGenerators[0].GetMinNumVars());
  Polynomial<Rational> tempP;
  FormatExpressions tempFormat;
  for (int i=0; i<numStartingGenerators; i++)
  { Polynomial<Rational>& currentPoly=theGroebnerBasis[i];
    currentPoly.SetNumVariablesSubDeletedVarsByOne(numStartingVariables+numStartingGenerators);
    tempP.MakeDegreeOne(numStartingVariables+numStartingGenerators, i+numStartingVariables, (Rational) -1);
    currentPoly+=tempP;
//  stOutput << currentPoly.ToString(false, tempFormat) << "<br>";
  }
  GroebnerBasisComputation<Rational> theComputation;
  theComputation.thePolynomialOrder.theMonOrder=MonomialP::LeftIsGEQLexicographicLastVariableWeakest;
  theComputation.TransformToReducedGroebnerBasis(theGroebnerBasis, &theGlobalVariables);
//  stOutput << "<br>the ending generators:<br> ";
//  for (int i=0; i<theGroebnerBasis.size; i++)
//  { stOutput << theGroebnerBasis.TheObjects[i].ToString(false, tempFormat) << "<br>";
//  }
  theGenerators.ReservE(theGroebnerBasis.size);
  theGenerators.size=0;
  for (int i=0; i<theGroebnerBasis.size; i++)
  { Polynomial<Rational> & currentPoly= theGroebnerBasis.TheObjects[i];
    bool bad=false;
    for (int j=0; j<numStartingVariables; j++)
      if(currentPoly.GetMaxPowerOfVariableIndex(j)>0)
      { bad=true;
        break;
      }
    if (!bad)
     theGenerators.AddOnTop(currentPoly);
  }
}

bool ConeComplex::findMaxLFOverConeProjective
  (const Cone& input, List<Polynomial<Rational> >& inputLinPolys, List<int>& outputMaximumOverEeachSubChamber, GlobalVariables& theGlobalVariables)
{ Vectors<Rational> HyperPlanesCorrespondingToLF;
  if (input.Normals.size<1 || inputLinPolys.size<1)
    return false;
  int theDim=input.Normals[0].size;
  HyperPlanesCorrespondingToLF.SetSize(inputLinPolys.size);
  for (int i=0; i<inputLinPolys.size; i++)
  { Polynomial<Rational>& currentPoly=inputLinPolys[i];
    if (currentPoly.TotalDegree()!=1 )
    { stOutput << "You messed up the total degree which must be one, instead it is "
      << currentPoly.TotalDegree() << ". The dimension of the cone is " << theDim;
      return false;
    }
    Vector<Rational>& newWall=HyperPlanesCorrespondingToLF.TheObjects[i];
    newWall.MakeZero(theDim);
    for (int j=0; j<currentPoly.size(); j++)
      for (int k=0; k<theDim; k++)
        if (currentPoly[j](k)==1)
        { newWall[k]=currentPoly.theCoeffs[j];
          break;
        }
  }
  return this->findMaxLFOverConeProjective
  (input, HyperPlanesCorrespondingToLF, outputMaximumOverEeachSubChamber, theGlobalVariables);
}

bool ConeComplex::findMaxLFOverConeProjective
  (const Cone& input, Vectors<Rational>& inputLFsLastCoordConst,
   List<int>& outputMaximumOverEeachSubChamber,
   GlobalVariables& theGlobalVariables)
{ this->init();
  this->AddNonRefinedChamberOnTopNoRepetition(input, theGlobalVariables);
  Vector<Rational> tempRoot;
  for (int i=0; i<inputLFsLastCoordConst.size; i++)
    for (int j=i+1; j<inputLFsLastCoordConst.size; j++)
    { tempRoot=inputLFsLastCoordConst[i]-inputLFsLastCoordConst[j];
      tempRoot.ScaleToIntegralMinHeightFirstNonZeroCoordinatePositive();
      if (!tempRoot.IsEqualToZero())
        this->splittingNormals.AddOnTopNoRepetition(tempRoot);
    }
  stOutput << this->ToString(false, true);
  this->Refine(theGlobalVariables);
  outputMaximumOverEeachSubChamber.SetSize(this->size);
  Rational theMax=0;
  for (int i=0; i<this->size; i++)
  { this->TheObjects[i].GetInternalPoint(tempRoot);
    bool isInitialized=false;
    for (int j=0; j<inputLFsLastCoordConst.size; j++)
      if (!isInitialized || tempRoot.ScalarEuclidean(inputLFsLastCoordConst[j])>theMax)
      { theMax=tempRoot.ScalarEuclidean(inputLFsLastCoordConst[j]);
        outputMaximumOverEeachSubChamber[i]=j;
        isInitialized=true;
      }
  }
  for (int i=0; i<this->size; i++)
  { //stOutput << "<br>Chamber " << i+1 << " maximum linear function is the function of index " << outputMaximumOverEeachSubChamber.TheObjects[i] << ": " << inputLinPolys.TheObjects[outputMaximumOverEeachSubChamber.TheObjects[i]].ToString();
    //stOutput << "<br>The chamber is given by: " << this->TheObjects[i].ToString(false, true);
  }
  return true;
}

//std::string tempDebugString;
void Lattice::Reduce
()
{ //////////////////////////////////the below function is for debugging purposes only!
  //Don't wanna mess around with #ifdefs, but it should be surrounded by #ifdef DEBUG .... #endif
  //bool flagTesting=true;
  //static Matrix<Rational>  testMatrix;
  //if (flagTesting)
  //  this->TestGaussianEliminationEuclideanDomainRationals(testMatrix);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  this->basis.GaussianEliminationEuclideanDomain();
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
//    stOutput << "<br>basis rational form: " << this->basisRationalForm.ToString(true, false);
//    out << "<br>basis rational form: " << this->basisRationalForm.ToString(true, false);
//    std::string tempS=out.str();
//    if(testMatrix!=this->basisRationalForm)crash << crash;
//  }
}

void Lattice::TestGaussianEliminationEuclideanDomainRationals(Matrix<Rational> & output)
{ output.AssignMatrixIntWithDen(this->basis, this->Den);
  std::stringstream out;
  stOutput << "Test output: " << output.ToString();
  out << "Test output: " << output.ToString();
  output.GaussianEliminationEuclideanDomain();
  stOutput << "<br>After gaussian elimination:" << output.ToString();
  out << "<br>After gaussian elimination:" << output.ToString();
//  tempDebugString=out.str();
}

void Lattice::RefineByOtherLattice(const Lattice& other)
{ if (other.basis.NumCols==0)
    return;
  if (other.basis==this->basis && this->Den==other.Den)
    return;
  if(other.GetDim()!=this->GetDim())
    crash << crash;
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

void Lattice::MakeFromMat(const Matrix<Rational> & input)
{ this->basisRationalForm=input;
  this->basisRationalForm.GetMatrixIntWithDen(this->basis, this->Den);
  this->Reduce();
}

void Lattice::MakeFromRoots(const Vectors<Rational>& input)
{ Matrix<Rational> tempMat;
  tempMat.AssignVectorsToRows(input);
  tempMat.GetMatrixIntWithDen(this->basis, this->Den);
  this->Reduce();
}

std::string Lattice::ToString(bool useHtml, bool useLatex)const
{ std::stringstream out;
  out << "L=<";
  Vectors<Rational> tempRoots;
  tempRoots.AssignMatrixRows(this->basisRationalForm);
  for (int i=0; i<this->basisRationalForm.NumRows; i++)
  { out << tempRoots.TheObjects[i].ToString();
    if (i!=this->basisRationalForm.NumRows-1)
      out << ",";
  }
  out << ">";
  return out.str();
}

LargeIntUnsigned LargeIntUnsigned::operator/(unsigned int x)const
{ LargeIntUnsigned result;
  LargeIntUnsigned remainder;
  LargeIntUnsigned tempX;
  tempX.AssignShiftedUInt(x, 0);
  this->DivPositive(tempX, result, remainder);
//  if(!result.CheckForConsistensy()) crash << crash;
  return result;
}

LargeIntUnsigned LargeIntUnsigned::operator/(const LargeIntUnsigned& x)const
{ LargeIntUnsigned result;
  LargeIntUnsigned remainder;
  this->DivPositive(x, result, remainder);
  return result;
}

int LargeIntUnsigned::operator%(unsigned int x)
{ LargeIntUnsigned result;
  LargeIntUnsigned remainder;
  LargeIntUnsigned tempX;
  tempX.AssignShiftedUInt(x, 0);
  this->DivPositive(tempX, result, remainder);
  return remainder.theDigits[0];
}

void LargeIntUnsigned::MakeOne()
{ this->theDigits.SetSize(1);
  this->theDigits[0]=1;
}

void LargeIntUnsigned::MakeZero()
{ this->theDigits.SetSize(1);
  this->theDigits[0]=0;
}

void LargeIntUnsigned::MultiplyByUInt(unsigned int x)
{ LargeIntUnsigned tempLI;
  tempLI.AssignShiftedUInt(x, 0);
  this->MultiplyBy(tempLI);
}

void LargeIntUnsigned::MultiplyBy(const LargeIntUnsigned& x)
{ LargeIntUnsigned tempInt;
  this->MultiplyBy(x, tempInt);
  *this=tempInt;
//  if(!this->CheckForConsistensy())crash << crash;
}

bool LargeIntUnsigned::IsIntegerFittingInInt(int* whichInt)
{ LargeIntUnsigned twoToThe31=2;
  MathRoutines::RaiseToPower(twoToThe31, 31, (LargeIntUnsigned) 1);
  if (*this>=twoToThe31)
    return false;
  if (whichInt==0)
    return true;
  *whichInt=0;
  for (int i=this->theDigits.size-1; i>=0; i--)
  { *whichInt*=LargeIntUnsigned::CarryOverBound;
    *whichInt+=this->theDigits[i];
  }
  return true;
}

bool LargeIntUnsigned::IsGEQ(const LargeIntUnsigned& x)const
{ if (this->theDigits.size>x.theDigits.size)
    return true;
  if (this->theDigits.size<x.theDigits.size)
    return false;
  for (int i=this->theDigits.size-1; i>=0; i--)
  { if (x.theDigits[i]>this->theDigits[i])
      return false;
    if (x.theDigits[i]<this->theDigits[i])
      return true;
  }
  return true;
}

bool LargeIntUnsigned::IsEven()const
{ //stOutput << "<br>remainder by 2 is " << ((*this)%2).ToString()
  //<< " and ((*this)%2)==0 is " << (((*this)%2)==0);
  return ((*this)%2)==0;
}

bool LargeIntUnsigned::operator==(const LargeIntUnsigned& other)const
{ return this->theDigits==other.theDigits;
}

bool LargeIntUnsigned::operator!=(const LargeIntUnsigned& other)const
{ return ! ((*this)==other);
}

void LargeIntUnsigned::ToString(std::string& output)const
{ if (this->IsEqualToZero())
  { output="0";
    return;
  }
  if (this->theDigits.size>1)
  { this->ElementToStringLargeElementDecimal(output);
    return;
  }
  std::stringstream out;
  out << this->theDigits[0];
  output=out.str();
}

#ifdef WIN32
#pragma warning(disable:4244)//warning 4244: data loss from conversion
#endif
void LargeIntUnsigned::DivPositive(const LargeIntUnsigned& x, LargeIntUnsigned& quotientOutput, LargeIntUnsigned& remainderOutput) const
{ if (x.theDigits.size==1 && this->theDigits.size==1)
  { quotientOutput.AssignShiftedUInt(this->theDigits[0]/x.theDigits[0], 0);
    remainderOutput.AssignShiftedUInt(this->theDigits[0]%x.theDigits[0], 0);
//    stOutput << "Dividing " << this->ToString() << " by " << x.ToString() << " yields quotient " << quotientOutput.ToString()
//    << " and remainder " << remainderOutput.ToString();
    return;
  }
  LargeIntUnsigned remainder, quotient, divisor;
  remainder=(*this);
  divisor=(x);
  if(divisor.IsEqualToZero())
    crash << crash;
  quotient.MakeZero();
  //std::string tempS1, tempS2, tempS3;
  while (remainder.IsGEQ(divisor))
  { unsigned int q;
    LargeIntUnsigned current, Total;
    if (*remainder.theDigits.LastObject()>*divisor.theDigits.LastObject())
    { q=*remainder.theDigits.LastObject()/(*divisor.theDigits.LastObject()+1);
      current.AssignShiftedUInt(q, remainder.theDigits.size-divisor.theDigits.size);
    }
    else
    { if (remainder.theDigits.size==divisor.theDigits.size)
        current.AssignShiftedUInt(1, 0);
      else
      { q=this->CarryOverBound/(divisor.theDigits[divisor.theDigits.size-1]+1);
        current.AssignShiftedUInt(q, remainder.theDigits.size- divisor.theDigits.size-1);
        current.MultiplyByUInt(remainder.theDigits[remainder.theDigits.size-1]);
      }
    }
    Total=(divisor);
    Total.MultiplyBy(current);
    //if (!remainder.IsGEQ(Total))
    //{ tempS1= remainder.ToString();
    //  tempS2=Total.ToString();
    //  remainder.IsGEQ(Total);
    //}
    remainder.SubtractSmallerPositive(Total);
    quotient+=(current);
  }
  remainderOutput=(remainder);
  quotientOutput=(quotient);
//  stOutput << "Dividing " << this->ToString() << " by " << x.ToString() << " yields quotient " << quotientOutput.ToString()
//  << " and remainder " << remainderOutput.ToString();
//  if(!remainderOut.CheckForConsistensy())crash << crash;
}
#ifdef WIN32
#pragma warning(default:4244)//warning 4244: data loss from conversion
#endif

void LargeIntUnsigned::ElementToStringLargeElementDecimal(std::string& output)const
{ std::stringstream out;
  if (this->CarryOverBound==1000000000UL)
  { std::string tempS;
    out << (*this->theDigits.LastObject());
    for (int i=this->theDigits.size-2; i>=0; i--)
    { std::stringstream tempStream;
      tempStream << this->theDigits[i];
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
  int initialNumDigitsEstimate=MaxNumIntegersPerCarryOverBound*this->theDigits.size;
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
        currentPower+=bufferPowersOfBase[highestBufferIndex];
      }
      out << theDigit;
      numRemainingDigits--;
      if (theDigit!=1)
        bufferPowersOfBase[highestBufferIndex]*=theDigit;
      Remainder.SubtractSmallerPositive(bufferPowersOfBase[highestBufferIndex]);
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

LargeIntUnsigned::LargeIntUnsigned()
{ this->theDigits.SetSize(1);
  this->theDigits[0]=0;
}

LargeIntUnsigned::LargeIntUnsigned(unsigned int x)
{ this->AssignShiftedUInt(x,0);
}

LargeIntUnsigned::  LargeIntUnsigned(const LargeIntUnsigned& x)
{ (*this)=x;
}

inline void LargeIntUnsigned::AddShiftedUIntSmallerThanCarryOverBound(unsigned int x, int shift)
{ if(!(x<LargeIntUnsigned::CarryOverBound))
    crash << crash;
  while (x>0)
  { if (shift>=this->theDigits.size)
    { int oldsize=this->theDigits.size;
      this->theDigits.SetSize(shift+1);
      for (int i=oldsize; i<this->theDigits.size; i++)
        this->theDigits[i]=0;
    }
    this->theDigits[shift]+=x;
    if (this->theDigits[shift]>=LargeIntUnsigned::CarryOverBound)
    { this->theDigits[shift]-=LargeIntUnsigned::CarryOverBound;
      x=1;
      shift++;
    } else
      x=0;
  }
//  this->FitSize();
}

inline void LargeIntUnsigned::AssignShiftedUInt(unsigned int x, int shift)
{ if (x==0)
  { this->MakeZero();
    return;
  }
  this->theDigits.SetSize(shift+1);
  for (int i=0; i<shift; i++)
    this->theDigits[i]=0;
  unsigned int tempX= x%LargeIntUnsigned::CarryOverBound;
  this->theDigits[shift]=tempX;
  x= x/LargeIntUnsigned::CarryOverBound;
  while (x!=0)
  { tempX= x%LargeIntUnsigned::CarryOverBound;
    this->theDigits.AddOnTop(tempX);
    x= x/LargeIntUnsigned::CarryOverBound;
  }
}

inline void LargeIntUnsigned::AddNoFitSize(const LargeIntUnsigned& x)
{ MacroIncrementCounter(Rational::TotalLargeAdditions);
  int oldsize= this->theDigits.size;
  this->theDigits.SetSize(MathRoutines::Maximum(this->theDigits.size, x.theDigits.size)+1);
  for (int i=oldsize; i<this->theDigits.size; i++)
    this->theDigits[i]=0;
  unsigned int CarryOver=0;
  for(int i=0; i<x.theDigits.size; i++)
  { this->theDigits[i]+=x.theDigits[i]+CarryOver;
    if (this->theDigits[i]>=LargeIntUnsigned::CarryOverBound)
    { this->theDigits[i]-=LargeIntUnsigned::CarryOverBound;
      CarryOver=1;
    }
    else
      CarryOver=0;
  }
  if (CarryOver!=0)
    for(int i=x.theDigits.size; i<this->theDigits.size; i++)
    { this->theDigits[i]+=1;
      if (this->theDigits[i]>=LargeIntUnsigned::CarryOverBound)
        this->theDigits[i]-=LargeIntUnsigned::CarryOverBound;
      else
        break;
    }
}

void LargeIntUnsigned::operator+=(const LargeIntUnsigned& x)
{ this->AddNoFitSize(x);
  this->FitSize();
}

LargeIntUnsigned LargeIntUnsigned::operator-(const LargeIntUnsigned& other) const
{ LargeIntUnsigned result;
  result=*this;
  result.SubtractSmallerPositive(other);
  return result;
}

void LargeIntUnsigned::SubtractSmallerPositive(const LargeIntUnsigned& x)
{ unsigned int CarryOver=0;
  if (!this->IsGEQ(x))
    crash << "This is a programming error: attempting to subtract a larger LargeIntUnsigned from a smaller one. " << crash;
  for (int i=0; i<x.theDigits.size; i++)
    if (this->theDigits[i]<x.theDigits[i]+CarryOver)
    { this->theDigits[i]+=LargeIntUnsigned::CarryOverBound;
      this->theDigits[i]-=(x.theDigits[i]+CarryOver);
      CarryOver=1;
    }
    else
    { this->theDigits[i]-=(x.theDigits[i]+CarryOver);
      CarryOver=0;
    }
  if (CarryOver!=0)
  { for (int i=x.theDigits.size; i<this->theDigits.size; i++)
      if (this->theDigits[i]>0)
      { this->theDigits[i]--;
        break;
      }
      else
        this->theDigits[i]=LargeIntUnsigned::CarryOverBound-1;
  }
  this->FitSize();
//  if(!this->CheckForConsistensy())crash << crash;
}

extern GlobalVariables onePredefinedCopyOfGlobalVariables;
void LargeIntUnsigned::MultiplyBy(const LargeIntUnsigned& x, LargeIntUnsigned& output)const
{ if (this==&output || &x==&output)
  { LargeIntUnsigned thisCopy=*this;
    LargeIntUnsigned xCopy=x;
    return thisCopy.MultiplyBy(xCopy, output);
  }
  MacroIncrementCounter(Rational::TotalLargeMultiplications);
  output.theDigits.SetSize(x.theDigits.size+this->theDigits.size);
  for(int i=0; i<output.theDigits.size; i++)
    output.theDigits[i]=0;
  unsigned long long numCycles=0;
  bool doProgressReporT=false;
  ProgressReport theReport(&onePredefinedCopyOfGlobalVariables);
  unsigned long long totalCycles;
  if (onePredefinedCopyOfGlobalVariables.flagReportEverything || onePredefinedCopyOfGlobalVariables.flagReportLargeIntArithmetic)
  { totalCycles=((unsigned long long) this->theDigits.size)* ((unsigned long long) x.theDigits.size);
    if (totalCycles>2000)
      doProgressReporT=true;
  }
  for (int i=0; i<this->theDigits.size; i++)
    for(int j=0; j<x.theDigits.size; j++)
    { unsigned long long tempLong= this->theDigits[i];
      unsigned long long tempLong2= x.theDigits[j];
      tempLong= tempLong*tempLong2;
      unsigned long long lowPart= tempLong%LargeIntUnsigned::CarryOverBound;
      unsigned long long highPart= tempLong/LargeIntUnsigned::CarryOverBound;
      output.AddShiftedUIntSmallerThanCarryOverBound((unsigned int) lowPart, i+j);
      output.AddShiftedUIntSmallerThanCarryOverBound((unsigned int) highPart, i+j+1);
      if (doProgressReporT)
      { numCycles++;
        if ((numCycles% 1024)==0)
        { std::stringstream out;
          if (LargeIntUnsigned::CarryOverBound==1000000000UL)
            out << "Large integer multiplication, crunching " << numCycles*9 << " out of " << totalCycles*9
            << " pairs of digits = " << this->theDigits.size << " x " << x.theDigits.size << " digits.";
          else
            out << "Large integer multiplication, crunching " << numCycles << " out of " << totalCycles
            << " pairs of digits = " << this->theDigits.size << " x " << x.theDigits.size
            << " digits (base " << LargeIntUnsigned::CarryOverBound << ").";
          theReport.Report(out.str());
        }
      }
    }
  output.FitSize();
//  if(!this->CheckForConsistensy())crash << crash;
}

int LargeIntUnsigned::GetUnsignedIntValueTruncated()
{ return (int) this->theDigits[0];
}

double LargeIntUnsigned::GetDoubleValue()const
{ double result=0;
  for (int i=this->theDigits.size-1; i>=0; i--)
    result=result*LargeIntUnsigned::CarryOverBound+this->theDigits[i];
  return result;
}

void LargeIntUnsigned::gcd(const LargeIntUnsigned& a, const LargeIntUnsigned& b, LargeIntUnsigned& output)
{ MacroIncrementCounter(Rational::TotalLargeGCDcalls);
  LargeIntUnsigned p, q, r, temp;
  p=a;
  q=b;
  while(!q.IsEqualToZero())
  { p.DivPositive(q, temp, r);
    p=q;
    q=r;
  }
  output=p;
//  if(!output.CheckForConsistensy())crash << crash;
}

void LargeIntUnsigned::FitSize()
{ int newSize=this->theDigits.size;
  for (int i=this->theDigits.size-1; i>=1; i--)
    if (this->theDigits[i]==0)
      newSize--;
    else
      break;
  this->theDigits.SetSize(newSize);
//  if(!this->CheckForConsistensy())crash << crash;
}

void LargeIntUnsigned::AccountPrimeFactor(const LargeInt& theP, List<LargeInt>& outputPrimeFactors, List<LargeIntUnsigned>& outputMults)
{ if (outputPrimeFactors.size==0)
  { outputPrimeFactors.AddOnTop(theP);
    outputMults.AddOnTop(1);
    return;
  }
  if ((*outputPrimeFactors.LastObject()).operator==(theP))
    (*outputMults.LastObject())++;
  else
  { outputPrimeFactors.AddOnTop(theP);
    outputMults.AddOnTop(1);
  }
}

bool LargeIntUnsigned::Factor(List<LargeInt>& outputPrimeFactors, List<int>& outputMultiplicites)
{ MacroRegisterFunctionWithName("LargeIntUnsigned::Factor");
  List<LargeIntUnsigned> buffer;
  if (!this->Factor(outputPrimeFactors, buffer))
    return false;
  outputMultiplicites.SetSize(buffer.size);
  for (int i=0; i<buffer.size; i++)
    if (!buffer[i].IsIntegerFittingInInt(&outputMultiplicites[i]))
      return false;
  return true;
}

bool LargeIntUnsigned::Factor(List<LargeInt>& outputPrimeFactors, List<LargeIntUnsigned>& outputMultiplicites)
{ MacroRegisterFunctionWithName("LargeIntUnsigned::Factor");
  if (this->theDigits.size>1)
    return false;
  if (this->IsEqualToZero())
    crash << "This is a programming error: it was requested that I factor 0, which is forbidden." << crash;
  //stOutput << "Factoring: " << this->ToString();
  unsigned int n=this->theDigits[0];
  outputPrimeFactors.SetSize(0);
  outputMultiplicites.SetSize(0);
  while (n%2==0)
  { this->AccountPrimeFactor(2, outputPrimeFactors, outputMultiplicites);
    n/=2;
  }
  unsigned int upperboundPrimeDivisors= (unsigned int) FloatingPoint::sqrt((double)n);
  List<bool> theSieve;
  theSieve.initFillInObject(upperboundPrimeDivisors+1,true);
  for (unsigned int i=3; i<=upperboundPrimeDivisors; i+=2)
    if (theSieve[i])
    { while (n%i==0)
      { this->AccountPrimeFactor(i, outputPrimeFactors, outputMultiplicites);
        n/=i;
        upperboundPrimeDivisors= (unsigned int) FloatingPoint::sqrt((double)n);
      }
      for (unsigned int j=i; j<=upperboundPrimeDivisors; j+=i)
        theSieve[j]=false;
    }
  if (n>1)
    this->AccountPrimeFactor(n, outputPrimeFactors, outputMultiplicites);
  //stOutput << " ... and the factors are: " << outputPrimeFactors
  //<< " with mults: " << outputMultiplicites;
  return true;
}

void LargeIntUnsigned::lcm(const LargeIntUnsigned& a, const LargeIntUnsigned& b, LargeIntUnsigned& output)
{ LargeIntUnsigned tempUI, tempUI2;
  if (a.IsEqualToZero() || b.IsEqualToZero())
    crash << "This is a programming error: calling lcm on zero elements is not allowed. " << crash;
  LargeIntUnsigned::gcd(a, b, tempUI);
  a.MultiplyBy(b, tempUI2);
  output=(tempUI2);
  output.DivPositive(tempUI, output, tempUI2);
  if(output.IsEqualToZero())
    crash << crash;
}

void LargeIntUnsigned::operator=(const LargeIntUnsigned& x)
{ this->theDigits=x.theDigits;
}

void LargeIntUnsigned::operator=(unsigned int x)
{ this->AssignShiftedUInt(x,0);
}

unsigned int LargeIntUnsigned::HashFunction()const
{ int numCycles=MathRoutines::Minimum(this->theDigits.size, SomeRandomPrimesSize);
  unsigned int result=0;
  for (int i=0; i<numCycles; i++)
    result+=this->theDigits[i]*SomeRandomPrimes[i];
  return result;
}

bool LargeIntUnsigned::IsPositive()const
{ return this->theDigits.size>1 || this->theDigits[0]>0;
}

bool LargeIntUnsigned::IsEqualToOne()const
{ return this->theDigits.size==1 && this->theDigits[0]==1;
}

bool LargeIntUnsigned::IsEqualToZero()const
{ return this->theDigits.size==1 && this->theDigits[0]==0;
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

bool LargeInt::IsEven()const
{ return this->value.IsEven();
}

void LargeInt::WriteToFile(std::fstream& output)
{ std::string tempS;
  this->ToString(tempS);
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
  { int x=input[i]-'0';
    if (x<0 || x>9)
      crash << "LargeInt::AssignString is not allowed to fail." << crash;
    if (i>startingIndex)
      this->value*=10;
    this->value+=(unsigned) x;
  }
  if (!this->IsEqualToZero())
    if (input[0]=='-')
      this->sign=-1;
}

bool LargeInt::IsIntegerFittingInInt(int* whichInt)
{ if (!this->value.IsIntegerFittingInInt(whichInt))
    return false;
  if (whichInt!=0)
    *whichInt*=this->sign;
  return true;
}

void LargeInt::MultiplyByInt(int x)
{//  if (this->value.size==0) return;
  LargeInt tempI;
  tempI.AssignInt(x);
  *this*=(tempI);
}

bool LargeInt::operator==(const LargeInt& x)const
{ if (x.sign!=this->sign)
  { if (x.IsEqualToZero() && this->IsEqualToZero())
      return true;
    else
      return false;
  }
  return this->value==x.value;
}

bool LargeInt::CheckForConsistensy()
{ if (this->sign!=-1 && this->sign!=1)
    return false;
  for (int i=0; i<this->value.theDigits.size; i++)
    if (this->value.theDigits[i]>=LargeIntUnsigned::CarryOverBound)
      return false;
  return true;
}

double LargeInt::GetDoubleValue()const
{ return this->sign* this->value.GetDoubleValue();
}

void LargeInt::ToString(std::string& output)const
{ std::stringstream out;
  if (this->IsEqualToZero())
  { output.assign("0");
    return;
  }
  if (this->sign==-1)
    out << "-";
  std::string tempS;
  this->value.ToString(tempS);
  out << tempS;
  output=out.str();
}

void LargeInt::AssignInt(int x)
{ if (x==0)
  { this->MakeZero();
    return;
  }
  this->sign=1;
  if (x<0)
  { this->sign=-1;
    x=-x;
  }
  this->value.AssignShiftedUInt( (unsigned int)x, 0);
//  if(!this->CheckForConsistensy())crash << crash;
}

bool LargeInt::GetDivisors(List<int>& output, bool includeNegative)
{ if (this->value.theDigits.size>1)
    return false;
  int val= this->value.theDigits[0];
  if (val>50000)
    return false;
  output.SetSize(0);
  for (int i=1; i<= val; i++)
    if (val% i==0)
    { output.AddOnTop(i);
      if (includeNegative)
        output.AddOnTop(-i);
    }
  return true;
}

void LargeInt::AddLargeIntUnsigned(const LargeIntUnsigned& x)
{ if (this->sign==1)
  { this->value+=x;
    return;
  }
  if (this->value.IsGEQ(x))
    this->value.SubtractSmallerPositive(x);
  else
  { LargeIntUnsigned tempI=x;
    tempI.SubtractSmallerPositive(this->value);
    this->value=tempI;
    this->sign=1;
  }
}

void LargeInt::operator+=(const LargeInt& x)
{ if (this->sign==x.sign)
    this->value+=x.value;
  else
  { if (this->value.IsGEQ(x.value))
      this->value.SubtractSmallerPositive(x.value);
    else
    { LargeIntUnsigned tempI=this->value;
      this->value=x.value;
      this->value.SubtractSmallerPositive(tempI);
      this->sign= x.sign;
    }
  }
//  if(!this->CheckForConsistensy())crash << crash;
}

void LargeInt::MakeZero()
{ this->value.MakeZero();
  this->sign=1;
}

void LargeInt::operator=(const Rational& other)
{ if (!other.IsInteger(this))
    crash << "This is a programming error: converting implicitly rational number " << other.ToString()
    << " to integer is not possible as the Rational number is not integral. " << crash;
//  if(!this->CheckForConsistensy())crash << crash;
}

void LargeInt::operator=(const LargeInt& x)
{ this->sign=x.sign;
  this->value=(x.value);
//  if(!this->CheckForConsistensy())crash << crash;
}

LargeInt LargeInt::operator/(int x)const
{ LargeInt result;
  LargeIntUnsigned remainder;
  LargeIntUnsigned tempX;
  int absX=x; signed char signX=1;
  if (x<0)
  { signX=-1;
    absX=-absX;
  }
  tempX.AssignShiftedUInt(absX, 0);
  this->value.DivPositive(tempX, result.value, remainder);
//  if(!result.CheckForConsistensy()) crash << crash;
  result.sign=this->sign* signX;
  return result;
}

LargeInt LargeInt::operator/(LargeInt& x)const
{ LargeInt result;
  LargeInt remainder;
  this->value.DivPositive(x.value, result.value, remainder.value);
  result.sign= this->sign* x.sign;
  if(!result.CheckForConsistensy())
    crash << crash;
  return result;
}

int LargeInt::operator%(int x)
{ if(x<=0)
    crash << crash;
  LargeIntUnsigned result;
  LargeIntUnsigned remainder;
  LargeIntUnsigned tempX;
  if (x<0)
    x=-x;
  tempX.AssignShiftedUInt(x, 0);
  this->value.DivPositive(tempX, result, remainder);
  if (remainder.theDigits.size ==0)
    return 0;
  else
  { if(this->sign == -1)
      return x-remainder.theDigits[0];
    else
      return remainder.theDigits[0];
  }
}

void Rational::WriteToFile(std::fstream& output)
{ output << this->ToString();
}

inline void Rational::RaiseToPower(int x)
{ Rational tempRat;
  tempRat.MakeOne();
  if (x<0)
  { x=-x;
    this->Invert();
  }
  LargeInt tempNum=this->GetNumerator();
  LargeIntUnsigned oneLI;
  oneLI.MakeOne();
  MathRoutines::RaiseToPower(tempNum.value, x, oneLI);
  LargeIntUnsigned tempDen= this->GetDenominator();
  MathRoutines::RaiseToPower(tempDen, x, oneLI);
  int theSign= (this->IsPositive() || x%2==0) ? 1 :-1;
  this->AllocateExtended();
  this->Extended->num.sign=(signed char)theSign;
  this->Extended->den=tempDen;
  this->Extended->num.value=tempNum.value;
  this->ShrinkExtendedPartIfPossible();
}

inline void Rational::Invert()
{ if (this->Extended==0)
  { int tempI= this->DenShort;
    if(tempI<=0)
      crash << crash;
    if (this->NumShort<0)
    { this->DenShort=-this->NumShort;
      this->NumShort=-tempI;
    }
    else
    { this->DenShort= this->NumShort;
      this->NumShort= tempI;
    }
    return;
  }
  MathRoutines::swap(this->Extended->den, this->Extended->num.value);
}

void Rational::ReadFromFile(std::istream& input)
{ std::string tempS;
  input >> tempS;
  this->AssignString(tempS);
}

inline void Rational::MultiplyByInt(int x)
{ Rational tempRat;
  tempRat.AssignInteger(x);
  this->MultiplyBy(tempRat);
}

inline void Rational::MultiplyBy(const Rational& r)
{ if (r.Extended==0 && this->Extended==0)
    if (this->TryToMultiplyQuickly(r.NumShort, r.DenShort))
      return;
  this->InitExtendedFromShortIfNeeded();
  if (r.Extended!=0)
  { this->Extended->num*=(r.Extended->num);
    this->Extended->den*=(r.Extended->den);
  }
  else
  { this->Extended->num.MultiplyByInt(r.NumShort);
    this->Extended->den.MultiplyByUInt((unsigned int)r.DenShort);
  }
  this->Simplify();
}

Rational operator/(int left, const Rational& right)
{ Rational tempRat=left;
  tempRat/=right;
  return tempRat;
}

Rational operator-(const Rational& argument)
{ Rational tempRat;
  tempRat.Assign(argument);
  tempRat.Minus();
  return tempRat;
}

inline void Rational::MultiplyByLargeInt(LargeInt& x)
{ this->InitExtendedFromShortIfNeeded();
  this->Extended->num*=(x);
  this->Simplify();
}

void Rational::MultiplyByLargeIntUnsigned(LargeIntUnsigned& x)
{ this->InitExtendedFromShortIfNeeded();
  this->Extended->num.value.MultiplyBy(x);
  this->Simplify();
}

inline void Rational::DivideBy(const Rational& r)
{ if (this==&r)
  { this->MakeOne();
    return;
  }
  Rational tempRat=r;
  tempRat.Invert();
  this->operator*=(tempRat);
}

Rational Rational::operator/(const Rational& right) const
{ Rational tempRat;
  tempRat.Assign(*this);
  tempRat.DivideBy(right);
  return tempRat;
}

Rational Rational::operator*(const Rational& right) const
{ Rational tempRat;
  tempRat.Assign(*this);
  tempRat.MultiplyBy(right);
  return tempRat;
}

Rational Rational::operator+(const Rational& right) const
{ Rational tempRat;
  tempRat.Assign(*this);
  tempRat+=(right);
  return tempRat;
}

Rational Rational::operator-(const Rational& right) const
{ Rational tempRat;
  tempRat.Assign(*this);
  tempRat-=(right);
  return tempRat;
}

void Rational::Assign(const Rational& r)
{ this->NumShort = r.NumShort;
  this->DenShort = r.DenShort;
  if (r.Extended==0)
  { if (this->Extended==0)
      return;
    this->FreeExtended();
    return;
  }
  this->InitExtendedFromShortIfNeeded();
  this->Extended->num=(r.Extended->num);
  this->Extended->den=(r.Extended->den);
}

void Rational::AssignFracValue()
{ if (this->Extended==0)
  { if (this->NumShort==0)
      return;
    if (this->DenShort==1)
    { this->NumShort=0;
      this->DenShort=1;
      return;
    }
    this->NumShort=this->NumShort% this->DenShort;
    if (this->NumShort<0)
      this->NumShort+=this->DenShort;
    return;
  }
  if (this->IsEqualToZero())
    return;
  if (this->Extended->den.IsEqualToOne())
  { this->MakeZero();
    return;
  }
  LargeIntUnsigned newNum, tempI;
  this->Extended->num.value.DivPositive(this->Extended->den, tempI, newNum);
  this->Extended->num.value=newNum;
  if (this->Extended->num.IsNegative())
    this->Extended->num.AddLargeIntUnsigned(this->Extended->den);
  if(!this->Extended->num.IsPositiveOrZero())
    crash << crash;
  this->Simplify();
}

void Rational::AddInteger(int x)
{ Rational tempRat;
  tempRat.AssignNumeratorAndDenominator(x, 1);
  this->operator+=(tempRat);
}

bool Rational::IsGreaterThan(const Rational& r) const
{ Rational tempRat;
  tempRat.Assign(*this);
  tempRat.Subtract(r);
  return tempRat.IsPositive();
}

void Rational::Subtract(const Rational& r)
{ Rational temp;
  temp.Assign(r);
  temp.Minus();
  this->operator+=(temp);
}

bool Rational::GetSquareRootIfRational(Rational& output)const
{ if (*this<0)
    return false;
  LargeInt theNum=this->GetNumerator();
  LargeIntUnsigned theDen= this->GetDenominator();
  List<LargeInt> primeFactorsNum, primeFactorsDen;
  List<LargeIntUnsigned> multsNum, multsDen;
  if (!theNum.value.Factor(primeFactorsNum, multsNum))
    return false;
  if (!theDen.Factor(primeFactorsDen, multsDen))
    return false;
  output=1;
  Rational tempRat;
  for (int i=0; i<primeFactorsNum.size; i++)
  { if (multsNum[i]%2!=0)
      return false;
    tempRat=primeFactorsNum[i];
    int currentMult=-1;
    if (!multsNum[i].IsIntegerFittingInInt(&currentMult))
      return false;
    tempRat.RaiseToPower(currentMult/2);
    output*=tempRat;
  }
  for (int i=0; i<primeFactorsDen.size; i++)
  { if (multsDen[i]%2!=0)
      return false;
    tempRat=primeFactorsDen[i];
    int currentMult=-1;
    if (!multsDen[i].IsIntegerFittingInInt(&currentMult))
      return false;
    tempRat.RaiseToPower(currentMult/2);
    output/=tempRat;
  }
  return true;
}

bool Rational::GetPrimeFactorsAbsoluteValue
  (List<LargeInt>& numeratorPrimeFactors, List<LargeIntUnsigned>& numeratorMultiplicities,
   List<LargeInt>& denominatorPrimeFactors, List<LargeIntUnsigned>& denominatorMultiplicities)
{ MacroRegisterFunctionWithName("Rational::GetPrimeFactorsAbsoluteValue");
  if (!this->GetNumerator().value.Factor(numeratorPrimeFactors, numeratorMultiplicities))
    return false;
  return this->GetDenominator().Factor(denominatorPrimeFactors, denominatorMultiplicities);
}

void Rational::AssignInteger(int x)
{ this->FreeExtended();
  this->DenShort=1;
  this->NumShort=x;
}

bool Rational::ShrinkExtendedPartIfPossible()
{ if (this->Extended==0)
    return true;
  if (this->Extended->num.value.theDigits.size>1 || this->Extended->den.theDigits.size>1 || this->Extended->num.value.theDigits[0]>=(unsigned int) LargeIntUnsigned::SquareRootOfCarryOverBound ||
      this->Extended->den.theDigits[0]>= (unsigned int) LargeIntUnsigned::SquareRootOfCarryOverBound)
    return false;
  this->NumShort= this->Extended->num.GetIntValueTruncated();
  this->DenShort= this->Extended->den.GetUnsignedIntValueTruncated();
  this->FreeExtended();
  return true;
}

Rational Rational::Factorial(int n, GlobalVariables* theGlobalVariables)
{ if (n<0)
  { crash << "This is a programming error: taking factorial of the negative number " << n << ". " << crash;
    return 0;
  }
  LargeIntUnsigned result;
  result.AssignFactorial((unsigned int) n, theGlobalVariables);
  Rational answer;
  answer=result;
  return answer;
}

Rational Rational::TwoToTheNth(int n)
{ Rational result=1;
  if (n>=0)
    for (int i=0; i<n; i++)
      result.MultiplyByInt(2);
  else
    for (int i=0; i>n; i--)
      result.DivideByInteger(2);
  return result;
}

Rational Rational::NChooseK(const Rational &n, int k)
{ Rational result;
  result.MakeOne();
  for (int i=0; i<k; i++)
  { //stOutput << "<br>" << result.ToString() << " * " << (n-i).ToString() << "/" << i+1;
    result*=n-i;
    result.DivideByInteger(i+1);
    //stOutput << "=" << result.ToString();
  }
  return result;
}

Rational Rational::NtoTheKth(int n, int k)
{ Rational result=n;
  result.RaiseToPower(k);
  return result;
}

bool Rational::IsInteger(LargeInt* whichInteger)const
{ bool result=false;
  if (this->Extended==0)
  { result=(this->DenShort==1);
    if (whichInteger!=0)
      *whichInteger=this->NumShort;
  }
  else
    if (this->Extended->den.IsEqualToOne())
    { result=true;
      if (whichInteger!=0)
        *whichInteger=this->Extended->num;
    }
  return result;
}

double Rational::GetDoubleValue()const
{ if (this->Extended==0)
    return (double)this->NumShort/(double)this->DenShort;
  else
    return this->Extended->num.GetDoubleValue()/this->Extended->den.GetDoubleValue();
}

void Rational::Simplify()
{ if (this->Extended==0)
  { if (this->NumShort==0)
      this->DenShort=1;
    else
    { if (this->DenShort==1)
        return;
      int tempGCD;
      if (this->NumShort>0)
        tempGCD= this->gcd(this->NumShort, this->DenShort);
      else
        tempGCD= this->gcd(-this->NumShort, this->DenShort);
      this->NumShort/=tempGCD;
      this->DenShort/=tempGCD;
    }
    return;
  }
  if (this->Extended->num.IsEqualToZero())
  { this->MakeZero();
    return;
  }
  if (!this->Extended->den.IsEqualToOne())
  { LargeIntUnsigned tempI;
    LargeIntUnsigned::gcd(this->Extended->den, this->Extended->num.value, tempI);
    /*if (Rational::flagAnErrorHasOccurredTimeToPanic)
    { std::string tempS1, tempS2, tempS3;
      tempI.ToString(tempS1);
      this->ToString(tempS2);
    }*/
    LargeIntUnsigned tempI2;
    this->Extended->den.DivPositive(tempI, this->Extended->den, tempI2);
    this->Extended->num.value.DivPositive(tempI, this->Extended->num.value, tempI2);
  }
  this->ShrinkExtendedPartIfPossible();
}

void Rational::DrawElement(GlobalVariables& theGlobalVariables, DrawElementInputOutput& theDrawData)
{ std::string tempS;
  tempS=this->ToString();
  theGlobalVariables.theDrawingVariables.theBuffer.drawTextBuffer(theDrawData.TopLeftCornerX, theDrawData.TopLeftCornerY, tempS, 0, theGlobalVariables.theDrawingVariables.fontSizeNormal, theGlobalVariables.theDrawingVariables.TextStyleNormal);
  theDrawData.outputHeight=10;
  theDrawData.outputWidth=10*tempS.size();
}

void Rational::operator=(const Polynomial<Rational>& other)
{ if (!other.IsConstant(this))
    crash << "This is a programming error: attempting to assign non-constant polynomial to a Rational number is not allowed. "
    << crash;
}

bool Rational::IsEqualTo(const Rational& b) const
{ if (this->Extended==0 && b.Extended==0)
    return (this->NumShort*b.DenShort==b.NumShort*this->DenShort);
  Rational tempRat;
  tempRat.Assign(*this);
  tempRat.Subtract(b);
  return tempRat.IsEqualToZero();
}

bool Rational::IsGreaterThanOrEqualTo(const Rational& right)const
{ if (this->Extended==0 && right.Extended==0)
    return (this->NumShort*right.DenShort>=right.NumShort*this->DenShort);
  Rational tempRat;
  tempRat.Assign(*this);
  tempRat.Subtract(right);
  return tempRat.IsPositiveOrZero();
}

std::string Rational::ToString(FormatExpressions* theFormat)const
{ if (theFormat!=0)
    if (theFormat->flagUseFrac)
      return this->ToStringFrac();
  std::stringstream out;
  if (this->Extended==0)
  { out << this->NumShort;
    if (this->DenShort!=1)
      out << "/" << this->DenShort;
  } else
  { std::string tempS;
    this->Extended->num.ToString(tempS);
    out << tempS;
    this->Extended->den.ToString(tempS);
    if (tempS!="1")
      out << "/" << tempS;
  }
  return out.str();
}

std::string Rational::ToStringForFileOperations(FormatExpressions* notUsed)const
{ std::stringstream out;
  if (this->Extended==0)
  { if (this->NumShort<0)
      out << "-";
    int numShortAbsoluteValue=this->NumShort<0 ?- this->NumShort: this->NumShort;
    if (this->DenShort==1)
      out << numShortAbsoluteValue;
    else
      out << numShortAbsoluteValue << "_div_" << this->DenShort;
    return out.str();
  }
  std::string tempS;
  LargeInt numAbsVal=this->Extended->num;
  if (numAbsVal<0)
  { out << "-";
    numAbsVal.sign=1;
  }
  if (this->Extended->den.IsEqualToOne())
    out << numAbsVal.ToString();
  else
    out << numAbsVal.ToString() << "_div_" << this->Extended->den.ToString();
  return out.str();
}

std::string Rational::ToStringFrac()const
{ std::stringstream out;
  if (this->Extended==0)
  { if (this->NumShort<0)
      out << "-";
    int numShortAbsoluteValue=this->NumShort<0 ?- this->NumShort: this->NumShort;
    if (this->DenShort==1)
      out << numShortAbsoluteValue;
    else
      out << "\\frac{" << numShortAbsoluteValue << "}{" << this->DenShort << "}";
    return out.str();
  }
  std::string tempS;
  LargeInt numAbsVal=this->Extended->num;
  if (numAbsVal<0)
  { out << "-";
    numAbsVal.sign=1;
  }
  if (this->Extended->den.IsEqualToOne())
    out << numAbsVal.ToString();
  else
    out << "\\frac{" << numAbsVal.ToString() << "}{" << this->Extended->den.ToString() << "}";
  return out.str();
}

void Rational::operator=(const AlgebraicNumber& other)
{ bool isGood=other.IsRational(this);
  if (!isGood)
    crash << "This is a programming error: attempting to assign the non-rational algebraic number " << other.ToString() << "to a rational number. " << crash;
}


