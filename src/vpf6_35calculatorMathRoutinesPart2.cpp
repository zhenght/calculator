//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#include "vpf.h"
#include "vpfHeader3Calculator2_InnerFunctions.h"
#include "vpfHeader3Calculator1_InnerTypedFunctions.h"
#include "vpfHeader2Math9DrawingVariables.h"

ProjectInformationInstance ProjectInfoVpf6_35cpp(__FILE__, "More calculator built-in functions. ");

struct MeshTriangles{
  Expression theFun;
  Calculator* owner;
  int XstartingGridCount;
  int YstartingGridCount;
  int numBadEvaluations;
  int numGoodEvaluations;
  int maxNumTriangles;
  double Height;
  double Width;
  double minTriangleSideAsPercentOfWidthPlusHeight;
  MeshTriangles()
  { this->XstartingGridCount=0;
    this->YstartingGridCount=0;
    this->numBadEvaluations=0;
    this->numGoodEvaluations=0;
    this->flagFunctionEvaluationFailed=false;
    this->minTriangleSideAsPercentOfWidthPlusHeight=0.001;
    this->maxNumTriangles=2000;
    this->flagTriangleLimitReached=false;
    this->flagShowGrid=false;
  }
  List<bool> trianglesUsed;
  List<List<Vector<double> > > theTriangles;
  MapLisT<Vector<double>, double, MathRoutines::HashVectorDoubles> theEvaluatedPoints;
  HashedList<Expression> knownEs;
  List<double> knownValues;
  Vector<double> lowerLeftCorner, upperRightCorner;
  Plot theGrid;
  Plot theCurve;
  Plot thePlot;
  bool flagShowGrid;
  bool flagFunctionEvaluationFailed;
  bool flagTriangleLimitReached;
  int CleanUpTrianglesReturnUpdatedCurrentIndex(int currentIndex);
  void ComputeImplicitPlot();
  void ComputeImplicitPlotPart2();
  void PlotGrid(int theColor);
  bool EvaluatesToDouble(double& whichDouble);
  double GetValueAtPoint(const Vector<double>& thePoint);
  void EvaluateFunAtTriangleVertices(int triangleIndex);
  double GetTriangleMaxSideLength(int triangleIndex);
  void Subdivide(int triangleIndex);
  void AddPointFromVerticesValues
  (Vectors<double>& outputAppend, const Vector<double>& left,
   const Vector<double>& right, double leftVal, double rightVal)
   ;
  bool ComputePoints
  (Calculator& theCommands, const Expression& input, bool showGrid)
;
};

void MeshTriangles::PlotGrid(int theColor)
{ MacroRegisterFunctionWithName("MeshTriangles::PlotGrid");
  this->theGrid.thePlots.SetSize(0);
  this->theGrid.thePlots.SetExpectedSize(this->theGrid.thePlots.size+this->theTriangles.size*3);
//  List<Vector<double> > currentLine;
//  currentLine.SetSize(2);
  PlotObject currentLinePlot;
  List<Vector<double> >& pointsVector=currentLinePlot.thePoints;
  currentLinePlot.thePoints.SetSize(4);
  currentLinePlot.colorRGB=theColor;
  for (int i=0; i<this->theTriangles.size; i++)
  { pointsVector[0]=this->theTriangles[i][0];
    pointsVector[1]=this->theTriangles[i][1];
    pointsVector[2]=this->theTriangles[i][2];
    pointsVector[3]=this->theTriangles[i][0];
    this->theGrid.thePlots.AddOnTop(currentLinePlot);
  }
//  this->theGrid.the=theColor;
}

double MeshTriangles::GetValueAtPoint(const Vector<double>& thePoint)
{ int theIndex= this->theEvaluatedPoints.GetIndex(thePoint);
  if (theIndex!=-1)
    return this->theEvaluatedPoints[theIndex];
  this->knownValues[this->knownValues.size-2]=thePoint[0];
  this->knownValues[this->knownValues.size-1]=thePoint[1];
  double functionValue=0;
  if (this->theFun.EvaluatesToDoubleUnderSubstitutions(this->knownEs, this->knownValues, &functionValue))
    this->numGoodEvaluations++;
  else
  { this->numBadEvaluations++;
    functionValue=NAN;
  }
  this->theEvaluatedPoints.SetKeyValue(thePoint, functionValue);
  return functionValue;
}

void MeshTriangles::EvaluateFunAtTriangleVertices(int triangleIndex)
{ MacroRegisterFunctionWithName("MeshTriangles::EvaluateFunAtTriangleVertices");
  for (int j=0; j<this->theTriangles[triangleIndex].size; j++)
    this->GetValueAtPoint(this->theTriangles[triangleIndex][j]);
}

double MeshTriangles::GetTriangleMaxSideLength(int triangleIndex)
{ MacroRegisterFunctionWithName("MeshTriangles::GetTriangleMaxSideLength");
  if (this->theTriangles[triangleIndex].size!=3)
    crash << "Error: triangle needs three vertices, instead it has vertices: " << this->theTriangles[triangleIndex] << crash;
  Vector<double>& firstV = this->theTriangles[triangleIndex][0];
  Vector<double>& secondV= this->theTriangles[triangleIndex][1];
  Vector<double>& thirdV = this->theTriangles[triangleIndex][2];
  List<Vector<double> > theSides;
  theSides.AddOnTop(firstV-secondV);
  theSides.AddOnTop(secondV-thirdV);
  theSides.AddOnTop(thirdV-firstV);
  double result=0;
  for (int i=0; i<theSides.size; i++)
  { //stOutput << "<br>current side: " << theSides[i];
    double normSquared= theSides[i][0]*theSides[i][0]+theSides[i][1]*theSides[i][1];
    result=MathRoutines::Maximum(result, FloatingPoint::sqrt(normSquared));
  }
  return result;
}

void MeshTriangles::AddPointFromVerticesValues
  (Vectors<double>& outputAppend, const Vector<double>& left,
   const Vector<double>& right, double leftVal, double rightVal)
{ //MacroRegisterFunctionWithName("MeshTriangles::AddPointFromVerticesValues");
  double Delta=leftVal-rightVal;
  if (Delta==0)
  { outputAppend.AddOnTop(left);
    return;
  }
  double contributionLeft=rightVal/Delta;
  if (contributionLeft<0)
    contributionLeft*=-1;
  double contributionRight=leftVal/Delta;
  if (contributionRight<0)
    contributionRight*=-1;
  outputAppend.AddOnTop(left*contributionLeft+right*contributionRight);
}

int MeshTriangles::CleanUpTrianglesReturnUpdatedCurrentIndex(int currentIndex)
{ MacroRegisterFunctionWithName("MeshTriangles::CleanUpTrianglesReturnUpdatedCurrentIndex");
  if (this->flagTriangleLimitReached || this->theTriangles.size<this->maxNumTriangles*2)
    return currentIndex;
  //clean up carried out only when we have twice exceeded the triangle limit
  //this way clean up will be carried only after we have generated at least maxNumTriangles since the last cleanup
  //(which leaves no more than maxNumTriangles alive).
  if (this->theTriangles.size!=this->trianglesUsed.size)
    crash << "this->theTriangles.size is not equal to this->trianglesUsed.size: the values are: "
    << this->theTriangles.size << " and " << this->trianglesUsed.size << ". " << crash;
  int lowestFree=0;
  for (int i=0; i<this->theTriangles.size; i++)
    if (this->trianglesUsed[i])
    { this->theTriangles[lowestFree]=this->theTriangles[i];
      this->trianglesUsed[lowestFree]=true;
      if (i==currentIndex)
        currentIndex=lowestFree;
      lowestFree++;
    }
  this->theTriangles.SetSize(lowestFree);
  this->trianglesUsed.SetSize(lowestFree);
  if (lowestFree>this->maxNumTriangles)
    this->flagTriangleLimitReached=true;
  return currentIndex;
}

void MeshTriangles::Subdivide(int triangleIndex)
{ MacroRegisterFunctionWithName("MeshTriangles::Subdivide");
  List<Vector<double> > currentTriangle=this->theTriangles[triangleIndex];
  if (currentTriangle.size!=3)
    crash << "Triangle in mesh with less than 3 sides! " << crash;
  List<Vector<double> > insideTriange;
  insideTriange.SetSize(3);
  insideTriange[0]=(currentTriangle[1]+currentTriangle[2])*0.5;
  insideTriange[1]=(currentTriangle[2]+currentTriangle[0])*0.5;
  insideTriange[2]=(currentTriangle[0]+currentTriangle[1])*0.5;
  this->theTriangles.AddOnTop(insideTriange);
  this->trianglesUsed.AddOnTop(true);
  List<Vector<double> > newTriangle;
  newTriangle=insideTriange;
  newTriangle[0]=currentTriangle[0];
  this->theTriangles.AddOnTop(newTriangle);
  this->trianglesUsed.AddOnTop(true);
  newTriangle=insideTriange;
  newTriangle[1]=currentTriangle[1];
  this->theTriangles.AddOnTop(newTriangle);
  this->trianglesUsed.AddOnTop(true);
  newTriangle=insideTriange;
  newTriangle[2]=currentTriangle[2];
  this->theTriangles.AddOnTop(newTriangle);
  this->trianglesUsed.AddOnTop(true);
}

void MeshTriangles::ComputeImplicitPlotPart2()
{ MacroRegisterFunctionWithName("MeshTriangles::ComputeImplicitPlotPart2");
  this->theTriangles.SetExpectedSize(this->maxNumTriangles*2);
  this->trianglesUsed.SetExpectedSize(this->maxNumTriangles*2);
  this->trianglesUsed.initFillInObject(this->theTriangles.size, true);
  this->theEvaluatedPoints.SetExpectedSize(this->maxNumTriangles*4);
  this->flagTriangleLimitReached=false;
  for (int i=0; i<this->theTriangles.size; i++)
    this->EvaluateFunAtTriangleVertices(i);
  if (this->numGoodEvaluations<5)
  { this->flagFunctionEvaluationFailed=true;
    return;
  }
  double minSide=MathRoutines::Minimum(this->Height, this->Width)*this->minTriangleSideAsPercentOfWidthPlusHeight;
  PlotObject currentPlot;
  currentPlot.colorRGB=CGI::RedGreenBlue(255, 0, 0);
  Vectors<double>& theSegment=currentPlot.thePoints;
  List<Vector<double> > currentTriangle;
  for (int i=0; i<this->theTriangles.size; i++)
  { currentTriangle=this->theTriangles[i]; //making a copy in case this->theTriangles changes underneath.
    if (currentTriangle.size!=3)
      crash << "Error: triangle needs three vertices, instead it has vertices: " << currentTriangle << crash;
    bool isGood=true;
    for (int j=0; j<currentTriangle.size; j++)
      if (this->GetValueAtPoint(currentTriangle[j])==NAN )
      { isGood=false;
        break;
      }
    if (!isGood)
    { this->trianglesUsed=false;
      continue;
    }
    double val0=this->GetValueAtPoint(currentTriangle[0]);
    double val1=this->GetValueAtPoint(currentTriangle[1]);
    double val2=this->GetValueAtPoint(currentTriangle[2]);
    double prod01=val0*val1;
    double prod12=val1*val2;
    double prod20=val2*val0;
    if (prod01>0 && prod12>0 && prod20>0)
    { this->trianglesUsed[i]=false;
      continue;
    }
    i=this->CleanUpTrianglesReturnUpdatedCurrentIndex(i);
    if (this->GetTriangleMaxSideLength(i)>minSide && !this->flagTriangleLimitReached)
    { this->Subdivide(i);
      continue;
    }
    theSegment.SetSize(0);
    if (prod01<=0)
      this->AddPointFromVerticesValues(theSegment, currentTriangle[0], currentTriangle[1], val0, val1);
    if (prod12<=0)
      this->AddPointFromVerticesValues(theSegment, currentTriangle[1], currentTriangle[2], val1, val2);
    if (prod20<=0 && theSegment.size<2)
      this->AddPointFromVerticesValues(theSegment, currentTriangle[2], currentTriangle[0], val2, val0);
    if (theSegment.size!=2)
      continue;
    this->theCurve.thePlots.AddOnTop(currentPlot);
  }
}

void MeshTriangles::ComputeImplicitPlot()
{ MacroRegisterFunctionWithName("MeshTriangles::ComputeImplicitPlot");
  if (this->XstartingGridCount==0 || this->YstartingGridCount==0)
    return;
  this->Width=this->upperRightCorner[0]-this->lowerLeftCorner[0];
  this->Height=this->upperRightCorner[1]-this->lowerLeftCorner[1];
  double DeltaX =(this->Width)/this->XstartingGridCount;
  double DeltaY =(this->Height)/this->YstartingGridCount;
  Vector<double> currentPoint;
  currentPoint.initFillInObject(2, 0);
  List<Vector<double> > currentTriangle;
  currentTriangle.initFillInObject(3, currentPoint);
  for (int i=0; i<this->XstartingGridCount; i++)
    for (int j=0; j<this->YstartingGridCount; j++)
    { currentTriangle[0][0]=this->lowerLeftCorner[0]+DeltaX*i;
      currentTriangle[0][1]=this->lowerLeftCorner[1]+DeltaY*j;
      currentTriangle[1]=currentTriangle[0];
      currentTriangle[1][0]+=DeltaX;
      currentTriangle[2]=currentTriangle[1];
      currentTriangle[2][1]+=DeltaY;
      this->theTriangles.AddOnTop(currentTriangle);
      currentTriangle[1]=currentTriangle[0];
      currentTriangle[1][1]+=DeltaY;
      this->theTriangles.AddOnTop(currentTriangle);
    }
  if (this->flagShowGrid)
  { this->PlotGrid(CGI::RedGreenBlue(240, 240, 0));
    this->thePlot.thePlots.AddListOnTop(this->theGrid.thePlots);
  }
  this->ComputeImplicitPlotPart2();
  if (this->flagShowGrid)
  { this->PlotGrid(CGI::RedGreenBlue(100, 100, 100));
    this->thePlot.thePlots.AddListOnTop(this->theGrid.thePlots);
  }
//  this->theCurve.colorRGB=CGI::RedGreenBlue(255,0,0);
  this->thePlot.thePlots.AddListOnTop(this->theCurve.thePlots);
//  stOutput << "ze lines: " << this->theCurve.theLines;
}

bool Calculator::GetVectorDoubles(const Expression& input, Vector<double>& output, int DesiredDimensionNonMandatory)
{ return this->GetVectoR(input, output, 0, DesiredDimensionNonMandatory, CalculatorFunctionsGeneral::innerEvaluateToDouble);
}

bool CalculatorFunctionsGeneral::innerGetPointsImplicitly
(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerGetPointsImplicitly");
  MeshTriangles theMesh;
  if (!theMesh.ComputePoints(theCommands, input, false))
    return false;
  HashedList<Vector<double>, MathRoutines::HashVectorDoubles> thePoints;
  for (int i=0; i<theMesh.theCurve.thePlots.size; i++)
  { thePoints.AddOnTopNoRepetition(theMesh.theCurve.thePlots[i].thePoints[0]);
    thePoints.AddOnTopNoRepetition(theMesh.theCurve.thePlots[i].thePoints[1]);
  }
  Matrix<double> theMatrix;
  theMatrix.AssignVectorsToRows(thePoints);
  return output.AssignValue(theMatrix, theCommands);
}

bool CalculatorFunctionsGeneral::innerPlotImplicitFunction
(Calculator& theCommands, const Expression& input, Expression& output)
{ return CalculatorFunctionsGeneral::innerPlotImplicitFunctionFull(theCommands, input, output, false);
}

bool CalculatorFunctionsGeneral::innerPlotImplicitShowGridFunction(Calculator& theCommands, const Expression& input, Expression& output)
{ return CalculatorFunctionsGeneral::innerPlotImplicitFunctionFull(theCommands, input, output, true);
}

bool MeshTriangles::ComputePoints
(Calculator& theCommands, const Expression& input, bool showGrid)
{ if (input.children.size<6)
    return false;
  this->flagShowGrid=showGrid;
  this->owner=&theCommands;
  this->theFun=input[1];
  this->knownEs=theCommands.knownDoubleConstants;
  this->knownValues=theCommands.knownDoubleConstantValues;
  this->knownValues.AddOnTop(0);
  this->knownValues.AddOnTop(0);
  HashedList<Expression> theFreeVars;
  if (!this->theFun.GetFreeVariables(theFreeVars, true))
    return theCommands << "Failed to extract free variables from: " << this->theFun.ToString();
  theFreeVars.QuickSortAscending();
  if (theFreeVars.size>2)
    return theCommands << "I got that your curve depends on " << theFreeVars.size << " expressions, namely: "
    << theFreeVars.ToStringCommaDelimited()
    << " and that is too many (2 max). ";
  Expression tempE;
  if (theFreeVars.size==0)
  { tempE.MakeAtom("x", theCommands);
    theFreeVars.AddOnTop(tempE);
  }
  if (theFreeVars.size==1)
  { tempE.MakeAtom("y",theCommands);
    if (theFreeVars[0]==tempE)
      tempE.MakeAtom("x", theCommands);
    theFreeVars.AddOnTop(tempE);
  }
  this->knownEs.AddOnTopNoRepetitionMustBeNewCrashIfNot(theFreeVars[0]);
  this->knownEs.AddOnTopNoRepetitionMustBeNewCrashIfNot(theFreeVars[1]);
  if (!theCommands.GetVectorDoubles(input[2], this->lowerLeftCorner))
    return theCommands << "Failed to extract lower left corner from: " << input[2].ToString();
  if (!theCommands.GetVectorDoubles(input[3], this->upperRightCorner))
    return theCommands << "Failed to extract upper right corner from: " << input[3].ToString();
  List<int> theGridCount, widthHeightInPixels;
  if (!theCommands.GetVectoRInt(input[4], theGridCount))
    return theCommands << "Failed to extract pair of small integers from: " << input[4].ToString();
  if (theGridCount.size!=2)
    return theCommands << "Failed to extract pair of small integers from: " << input[4].ToString();
  if (!theCommands.GetVectoRInt(input[5], widthHeightInPixels))
    return theCommands << "Failed to extract pair of small integers from: " << input[5].ToString();
  if (widthHeightInPixels.size!=2)
    return theCommands << "Failed to extract pair of small integers from: " << input[5].ToString();
  this->thePlot.DesiredHtmlWidthInPixels=widthHeightInPixels[0];
  this->thePlot.DesiredHtmlWidthInPixels=widthHeightInPixels[1];
  this->XstartingGridCount=theGridCount[0];
  this->YstartingGridCount=theGridCount[1];
  if (input.children.size>=7)
  { if (!input[6].IsSmallInteger(&this->maxNumTriangles))
      return theCommands << "Failed to extract small integer from: " << input[6].ToString();
    if (this->maxNumTriangles>20000)
    { this->maxNumTriangles=20000;
      theCommands << "Max number of triangles decreased from your input: " << input[6].ToString()
      << " to 20000. If you'd like to lift the restriction, modify code around: file: " << __FILE__
      << " line: " << __LINE__ << ". ";
    }
  }
  this->ComputeImplicitPlot();
  theCommands << "Evaluated function in: " << this->numGoodEvaluations << " points and failed to evaluate it at: "
  << this->numBadEvaluations << " points. ";
  return true;
}

bool CalculatorFunctionsGeneral::innerPlotImplicitFunctionFull
(Calculator& theCommands, const Expression& input, Expression& output, bool showGrid)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerPlotImplicitFunctionFull");
  MeshTriangles theMesh;
  if (!theMesh.ComputePoints(theCommands, input, showGrid))
    return false;
  return output.AssignValue(theMesh.thePlot, theCommands);
}

bool CalculatorConversions::innerMatrixDouble(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorConversions::innerMatrixDouble");
//  stOutput << "ere i am, jh";
  Matrix<double> theMat;
  if (!theCommands.GetMatrix(input, theMat, 0, 0, CalculatorFunctionsGeneral::innerEvaluateToDouble))
    return theCommands << "<br>Failed to get matrix of algebraic numbers. ";
  return output.AssignValue(theMat, theCommands);
}

bool CalculatorFunctionsGeneral::innerNumerator(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerNumerator");
  if (input.StartsWith(theCommands.opDivide()))
    if (input.children.size>1)
    { output=input[1];
      return true;
    }
  output=input;
  return true;
}

bool CalculatorFunctionsGeneral::innerDenominator(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerDenominator");
  Rational theRat, theDen;
  if (input.IsRational(&theRat))
  { theDen=theRat.GetDenominator();
    return output.AssignValue(theDen, theCommands);
  }
  if (input.StartsWith(theCommands.opDivide()))
    if (input.children.size>2)
    { output=input[2];
      return true;
    }
  return output.AssignValue(1, theCommands);
}

bool CalculatorFunctionsGeneral::innerMultiplySequence
(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerMultiplySequence");
  if (input.children.size<1)
    return false;
  if (input.children.size==1)
    return output.AssignValue(1, theCommands);
  List<Expression> theTerms;
  for (int i=1; i<input.children.size; i++)
    theTerms.AddOnTop(input[i]);
  return output.MakeProducT(theCommands, theTerms);
}

bool CalculatorFunctionsGeneral::innerEnsureExpressionDependsOnlyOnStandard
(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerEnsureExpressionDependsOnlyOnStandard");
  if (input.size()<3)
    return false;
  const Expression& theExpression=input[1];
  HashedList<Expression> allowedFreeVars, presentFreeVars;
  allowedFreeVars.SetExpectedSize(input.size()-2);
  presentFreeVars.SetExpectedSize(input.size()-2);
  for (int i=2; i<input.size(); i++)
    allowedFreeVars.AddOnTopNoRepetition(input[i]);
  std::stringstream out;
  theExpression.GetFreeVariables(presentFreeVars, true);
  if (!allowedFreeVars.Contains(presentFreeVars))
  { out << "<hr>";
    out << "Your expression:<br>\\(" << input[1].ToString() << "\\)"
    << "<br><span style='color:red'><b>contains the unexpected variable(s):</b></span><br><b>";
    bool found=false;
    for (int i=0; i< presentFreeVars.size; i++)
      if (!allowedFreeVars.Contains(presentFreeVars[i]))
      { if (found)
          out << ", ";
        found=true;
        out << presentFreeVars[i].ToString();
      }
    out << "</b>.";
    out << "<br>The expected variables are: " << allowedFreeVars.ToStringCommaDelimited() << ". ";
    out << "<br>Beware of typos such as:<br>[wrong:] <span style='color:red'>lnx, sqrt2</span>  "
    << "<br>[correct:] <span style='color:green'>ln(x)</span> or <span style='color:green'>ln x</span>, "
    << "<span style='color:green'>sqrt(2)</span> or <span style='color:green'>sqrt 2</span>.<hr>";
    return output.AssignValue(out.str(), theCommands);
  }
  return output.AssignValue(out.str(), theCommands);
}

bool CalculatorFunctionsGeneral::innerSort(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerSort");
  if (!input.IsListStartingWithAtom(theCommands.theAtoms.GetIndexIMustContainTheObject("Sort"))
      &&
      !input.IsSequenceNElementS())
    return false;
  List<Expression> sortedExpressions;
  sortedExpressions.Reserve(input.children.size-1);
  for (int i=1; i<input.children.size; i++)
    sortedExpressions.AddOnTop(input[i]);
/*  stOutput << "<hr>DEBUG";
  if (sortedExpressions.size==3)
  { if (sortedExpressions[0]>sortedExpressions[1])
      stOutput << "<br>" << sortedExpressions[0].ToString() << "&gt;" << sortedExpressions[1].ToString();
    else
      stOutput << "<br>" << sortedExpressions[1].ToString() << "&gt;" << sortedExpressions[0].ToString();
    if (sortedExpressions[0]>sortedExpressions[2])
      stOutput << "<br>" << sortedExpressions[0].ToString() << "&gt;" << sortedExpressions[2].ToString();
    else
      stOutput << "<br>" << sortedExpressions[2].ToString() << "&gt;" << sortedExpressions[0].ToString();
    if (sortedExpressions[1]>sortedExpressions[2])
      stOutput << "<br>" << sortedExpressions[1].ToString() << "&gt;" << sortedExpressions[2].ToString();
    else
      stOutput << "<br>" << sortedExpressions[2].ToString() << "&gt;" << sortedExpressions[1].ToString();
  }
    stOutput << "<hr>";*/
  sortedExpressions.QuickSortAscending();
  return output.MakeSequence(theCommands, &sortedExpressions);
}

bool CalculatorFunctionsGeneral::innerSortDescending
(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerSortDescending");
  if (!input.IsListStartingWithAtom(theCommands.theAtoms.GetIndexIMustContainTheObject("SortDescending"))
      &&
      !input.IsSequenceNElementS())
    return false;
  List<Expression> sortedExpressions;
  sortedExpressions.Reserve(input.children.size-1);
  for (int i=1; i<input.children.size; i++)
    sortedExpressions.AddOnTop(input[i]);
  sortedExpressions.QuickSortDescending();
  return output.MakeSequence(theCommands, &sortedExpressions);
}

bool CalculatorFunctionsGeneral::innerLength(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerLength");
  if (input.IsListStartingWithAtom(theCommands.theAtoms.GetIndexIMustContainTheObject("Length")) ||
      input.IsSequenceNElementS())
    return output.AssignValue(input.children.size-1, theCommands);
  return false;
}

bool CalculatorFunctionsGeneral::innerEnsureExpressionDependsOnlyOnMandatoryVariables
(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerEnsureExpressionDependsOnlyOnMandatoryVariables");
  if (input.size()<3)
    return false;
  const Expression& theExpression=input[1];
  HashedList<Expression> mandatoryFreeVars, allowedFreeVars, presentFreeVars;
  if (input[2].IsSequenceNElementS())
  { mandatoryFreeVars.SetExpectedSize(input[2].size()-1);
    for (int i=1; i<input[2].size(); i++)
      mandatoryFreeVars.AddOnTop(input[2][i]);
  } else
    mandatoryFreeVars.AddOnTop(input[2]);
  allowedFreeVars.AddOnTop(mandatoryFreeVars);
  if (input.size()>3)
  { if (input[3].IsSequenceNElementS())
      for (int i=1; i<input[3].size(); i++)
        allowedFreeVars.AddOnTop(input[3][i]);
    else
      allowedFreeVars.AddOnTop(input[3]);
  }
  presentFreeVars.SetExpectedSize(input.size()-2);
  theExpression.GetFreeVariables(presentFreeVars, true);
  std::stringstream out;
  if (!presentFreeVars.Contains(mandatoryFreeVars))
  { out << "<hr>";
    out << "Your expression:<br>\\(" << input[1].ToString() << "\\)"
    << "<br><span style='color:red'><b>is required to contain the variables:</b></span><br><b>";
    bool found=false;
    for (int i=0; i< mandatoryFreeVars.size; i++)
      if (!presentFreeVars.Contains(mandatoryFreeVars[i]))
      { if (found)
          out << ", ";
        found=true;
        out << mandatoryFreeVars[i].ToString();
      }
    out << "</b>.";
    out << "<br>The mandatory variable(s) are: " << mandatoryFreeVars.ToStringCommaDelimited() << ". ";
  }
  if (!allowedFreeVars.Contains(presentFreeVars))
  { out << "<hr>";
    out << "Your expression:<br>\\(" << input[1].ToString() << "\\)"
    << "<br><span style='color:red'><b>contains the unexpected variable(s):</b></span><br><b>";
    bool found=false;
    for (int i=0; i< presentFreeVars.size; i++)
      if (!allowedFreeVars.Contains(presentFreeVars[i]))
      { if (found)
          out << ", ";
        found=true;
        out << presentFreeVars[i].ToString();
      }
    out << "</b>.";
    out << "<br>The expected variables are: " << allowedFreeVars.ToStringCommaDelimited() << ". ";
  }
  if (out.str()!="")
    out << "<br>Beware of typos such as:<br>[wrong:] <span style='color:red'>lnx, sqrt2</span>  "
    << "<br>[correct:] <span style='color:green'>ln(x)</span> or <span style='color:green'>ln x</span>, "
    << "<span style='color:green'>sqrt(2)</span> or <span style='color:green'>sqrt 2</span>.<hr>";

  return output.AssignValue(out.str(), theCommands);
}


bool CalculatorFunctionsGeneral::innerPlotLabel
(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerPlotRectangle");
  if (input.size()<4)
    return false;
  Vector<double> labelPosition;
  labelPosition.SetSize(2);
  if (!input[1].EvaluatesToDouble(& labelPosition[0]) || !input[2].EvaluatesToDouble(& labelPosition[1]))
    return false;
  std::string theLabel;
  if (!input[3].IsOfType<std::string>(&theLabel))
    theLabel=input[3].ToString();
  PlotObject thePlot;
  thePlot.thePlotString=theLabel;
  thePlot.thePoints.AddOnTop(labelPosition);
  thePlot.thePlotType="label";
  return output.AssignValue(thePlot, theCommands);

}

bool CalculatorFunctionsGeneral::innerPlotRectangle
(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerPlotRectangle");
  if (input.size()!=3)
    return false;
  Vectors<double> theRectangle;
  theRectangle.SetSize(2);
  if (!theCommands.GetVectorDoubles(input[1], theRectangle[0],2) || !theCommands.GetVectorDoubles(input[2], theRectangle[1], 2))
    return false;
  PlotObject thePlot;
  thePlot.theRectangles.AddOnTop(theRectangle);
  thePlot.colorRGB=CGI::RedGreenBlue(0,0,255);
  thePlot.fillColorRGB=CGI::RedGreenBlue(0,255,255);
  return output.AssignValue(thePlot, theCommands);
}

bool CalculatorFunctionsGeneral::innerSolveUnivariatePolynomialWithRadicalsWRT
(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerSolveUnivariatePolynomialWithRadicalsWRT");
  if (input.children.size!=3)
    return theCommands << "solveFor takes as input three arguments. ";
  Expression thePowers;
  Expression modifiedInput=input;
  if (!modifiedInput[2].StartsWith(theCommands.opDefine()))
  { if (!CalculatorFunctionsGeneral::innerCoefficientsPowersOf(theCommands, modifiedInput, thePowers))
      return theCommands << "Failed to extract the coefficients of " << modifiedInput[1].ToString()
      << " in " << modifiedInput[2].ToString();
  } else
  { Expression convertedEqualityE, convertedSimplifiedEqualityE;
    if (!CalculatorFunctionsGeneral::innerEqualityToArithmeticExpression(theCommands, modifiedInput[2], convertedEqualityE))
      return theCommands << "Failed to interpret the equality " << modifiedInput[2].ToString();
    if (!Calculator::EvaluateExpression(theCommands, convertedEqualityE, convertedSimplifiedEqualityE))
      return theCommands << "Failed to simplify: " << convertedEqualityE.ToString();
    //stOutput << "DEBUG: here be i; convertedSimplifiedEqualityE: " << convertedSimplifiedEqualityE.ToString() << ", lispified: "
    //<< convertedSimplifiedEqualityE.ToStringSemiFull();
    modifiedInput.SetChilD(2, convertedSimplifiedEqualityE);
    //stOutput << "DEBUG: here be i; modifiedInput: " << modifiedInput.ToString() << ", lispified: "
    //<< modifiedInput.ToStringSemiFull();
    if (!CalculatorFunctionsGeneral::innerCoefficientsPowersOf(theCommands, modifiedInput, thePowers))
      return theCommands << "Failed to extract the coefficients of " << modifiedInput[1].ToString()
      << " in " << modifiedInput[2].ToString() << " which was obtained from the equality "
      << input[2].ToString();
    //stOutput << "DEBUG: here be i; thePowers: " << thePowers.ToString() << ", lispified: "
    //<< thePowers.ToStringSemiFull();
  }
  if (!thePowers.IsSequenceNElementS())
    return theCommands << "This is not supposed to happen: expression "
    << thePowers.ToString() << " should be a list. This may be a programming bug. ";
  if (thePowers.size()==2)
    return theCommands << "Cannot solve: " << modifiedInput[2].ToString()
    << ". The expression does not depend on " << modifiedInput[1].ToString() << ". The coefficients of "
    << modifiedInput[1].ToString() << " are: " << thePowers.ToString();
  if (thePowers.size()==3)
  { output=thePowers[1];
    output*=-1;
    output/=thePowers[2];
    return true;
  }
  if (thePowers.children.size==4)
  { const Expression& a=thePowers[3];
    const Expression& b=thePowers[2];
    const Expression& c=thePowers[1];
    output.MakeSequence(theCommands);
//    stOutput << "DEBIG: a,b,c" << thePowers[3] << thePowers[2] << thePowers[1] << "<br>";
    Expression currentRoot;
    Expression theDiscriminant;
    theDiscriminant=b*b-a*c*4;
    Expression sqrtDiscriminant;
    sqrtDiscriminant.MakeSqrt(theCommands, theDiscriminant, 2);
    currentRoot=(b*(-1)-sqrtDiscriminant)/(a*2);
    output.AddChildOnTop(currentRoot);
    currentRoot=(b*(-1)+sqrtDiscriminant)/(a*2);
    output.AddChildOnTop(currentRoot);
    return true;
  }
  return false;
}

bool CalculatorFunctionsGeneral::innerIntegralUpperBound(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("Calculator::innerIntegralUpperBound");
  if (input.size()!=3)
    return false;
  const Expression& base=input[1];
  if (!base.StartsWith(theCommands.opIntegral(),2))
    return false;
  Expression newSetE;
  newSetE.MakeXOX(theCommands, theCommands.opSequence(), base[1], input[2]);
  return output.MakeOX(theCommands, theCommands.opIntegral(), newSetE);
}

bool CalculatorFunctionsGeneral::innerPowerExponentToLog(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerPowerExponentToLog");
  if (!input.StartsWith(theCommands.opThePower(), 3))
    return false;
  const Expression& baseE=input[1];
  const Expression& powerE=input[2];
  if (baseE.IsAtomGivenData(theCommands.opE()))
    if (powerE.StartsWith(theCommands.opLog(), 2))
    { output=powerE[1];
      return true;
    }
  return false;
}

bool CalculatorFunctionsGeneral::innerSqrt(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("Calculator::innerSqrt");
  if (input.size()!=3)
    return false;
//  stOutput << "<br>Calling sqrt with input: " << input.ToString();
  Rational ratPower;
  if (input[1].IsRational(&ratPower))
    if (ratPower!=0)
    { Expression powerE, powerEreduced, theExponentE;
      ratPower.Invert();
      theExponentE.AssignValue(ratPower, theCommands);
      powerE.MakeXOX(theCommands,theCommands.opThePower(), input[2], theExponentE);
      if (CalculatorFunctionsBinaryOps::innerPowerRatByRatReducePrimeFactors
          (theCommands, powerE, powerEreduced))
        if (powerEreduced!=powerE && powerEreduced!=input)
        { output=powerEreduced;
          return true;
        }
    }
  if (input[2].IsEqualToOne())
    return output.AssignValue(1, theCommands);
  int thePower=0;
  if (!input[1].IsSmallInteger(&thePower))
    return false;
//  stOutput << "<br>the power that be: " << thePower ;
  if (!input[2].IsConstantNumber())
  {// stOutput << "<br>input is: " << input[2].ToString();
    theCommands.CheckInputNotSameAsOutput(input, output);
    Expression theExponent;
    Rational thePowerRat(1, thePower);
    theExponent.AssignValue(thePowerRat, theCommands);
    return output.MakeXOX(theCommands, theCommands.opThePower(), input[2], theExponent);
  }
  if (thePower>0 && input[2].IsEqualToZero())
    return output.AssignValue(0, theCommands);
  if (thePower==0 && input[2].IsEqualToZero())
    return output.AssignValue(1, theCommands);
  Rational rationalValue;
  if (!input[2].IsRational(&rationalValue))
    return false;
  if (thePower<0)
  { if (rationalValue.IsEqualToZero())
      return output.MakeError("Division by zero in expression: " + input.ToString(), theCommands);
    thePower*=-1;
    rationalValue.Invert();
  }
  if (thePower!=2)
    return false;
//  stOutput << "<br>Got here: rat value is: " << rationalValue.ToString();
  AlgebraicNumber theNumber;
  if (!theNumber.AssignRationalQuadraticRadical(rationalValue, theCommands.theObjectContainer.theAlgebraicClosure))
    return false;
  return output.AssignValue(theNumber, theCommands);
}

class SelectionFixedRankDifferentMaxMultiplicities
{
public:
  List<int> Multiplicities;
  List<int> MaxMultiplicities;
  int rank;
  bool flagFirstComputed;
  bool init();
  bool firstIncrement();
  bool IncrementReturnFalseIfPastLast();
  SelectionFixedRankDifferentMaxMultiplicities();
  std::string ToString();
  std::string ToStringFull();
};

class KostkaNumber
{
public:
  List<int> partition;
  List<int> tuple;
  int sumTuple;
  int sumPartition;
  int MaxNumCachedKostkaNumbers;
  LargeInt value;
//  bool IncrementReturnFalseIfPastLast();
  bool initTableaux(std::stringstream* comments=0);
  bool operator==(const KostkaNumber& other)const;
  static unsigned int HashFunction(const KostkaNumber& input);
  std::string ToString();
  KostkaNumber();
  bool Compute(HashedList<KostkaNumber>* KNcache, std::stringstream* comments=0);
};

unsigned int KostkaNumber::HashFunction(const KostkaNumber& input)
{ return  MathRoutines::HashListInts(input.partition)+
          MathRoutines::HashListInts(input.tuple);
}

bool KostkaNumber::operator ==(const KostkaNumber& other)const
{ return this->partition==other.partition && this->tuple==other.tuple;
}

KostkaNumber::KostkaNumber()
{ this->MaxNumCachedKostkaNumbers=10000;
}

std::string KostkaNumber::ToString()
{ std::stringstream out;
  out << "KostkaNumber" << "((";
  for (int i=0; i<this->partition.size; i++)
  { out << this->partition[i];
    if (i!=this->partition.size-1)
      out << ", ";
  }
  out << "), (";
  for (int i=0; i<this->tuple.size; i++)
  { out << this->tuple[i];
    if (i!=this->tuple.size-1)
      out << ", ";
  }
  out << "))";
  return out.str();
}

bool KostkaNumber::initTableaux(std::stringstream* comments)
{ for (int i=0; i<this->partition.size-1; i++)
    if (this->partition[i]<this->partition[i+1])
    { if (comments!=0)
        *comments << "Partition is supposed to be a non-decreasing sequence of integers, instead it is: "
        << this->partition;
      return false;
    }
  this->sumTuple=0;
  this->sumPartition=0;
  for (int i=0; i<this->tuple.size; i++)
  { this->sumTuple+=this->tuple[i];
    if (this->sumTuple>10000000 || this->tuple[i]>10000000 || this->tuple[i]<0)
    { if (comments!=0)
        *comments << "Failed to compute Kostka number: the tuple "
        << this->tuple << " is too large or negative. ";
      return false;
    }
  }
  for (int i=0; i<this->partition.size; i++)
  { this->sumPartition+=this->partition[i];
    if (this->sumPartition>10000000 || this->partition[i]>10000000 || this->partition[i]<0)
    { if (comments!=0)
        *comments << "Failed to compute Kostka number: the partition: "
        << this->partition << " is too large or negative. ";
      return false;
    }
  }
  return true;
}

bool KostkaNumber::Compute(HashedList<KostkaNumber>* KNcache, std::stringstream* comments)
{ MacroRegisterFunctionWithName("KostkaNumber::Compute");
  this->value=-1;
  if (!this->initTableaux(comments))
    return false;
  if (this->sumTuple!=this->sumPartition)
  { this->value=0;
    return true;
  }
  if (this->sumTuple==0)
  { if (this->sumPartition==0)
      this->value=1;
    else
      this->value=0;
    return true;
  } //we are now guaranteed each of the tuple and the partition has at least one entry.
//  stOutput << "<br>Computing: " << this->ToString();

  SelectionFixedRankDifferentMaxMultiplicities theSel;
  theSel.init();
  theSel.MaxMultiplicities.SetSize(this->partition.size);
  for (int i=0; i<this->partition.size; i++)
    if (i!=this->partition.size-1)
      theSel.MaxMultiplicities[i]=this->partition[i]-this->partition[i+1];
    else
      theSel.MaxMultiplicities[i]=this->partition[i];
  theSel.rank=*this->tuple.LastObject();
  this->value=0;
//  stOutput << "<br>Selection before start: " << theSel.ToStringFull();
  while (theSel.IncrementReturnFalseIfPastLast())
  { //stOutput << "<br>current selection: " << theSel.ToStringFull();
    KostkaNumber ancestor;
    ancestor.partition=this->partition;
    ancestor.tuple=this->tuple;
    ancestor.tuple.SetSize(ancestor.tuple.size-1);
    for (int i=0; i<theSel.Multiplicities.size; i++)
      ancestor.partition[i]-=theSel.Multiplicities[i];
    if (KNcache!=0)
    { int ancestorIndex=KNcache->GetIndex(ancestor);
      if (ancestorIndex!=-1)
        ancestor=KNcache->GetElement(ancestorIndex);
      else
        if (!ancestor.Compute(KNcache, comments))
          return false;
        else
          if (KNcache->size<this->MaxNumCachedKostkaNumbers)
            KNcache->AddOnTop(ancestor);
    } else
      if (!ancestor.Compute(KNcache, comments))
        return false;
    this->value+=ancestor.value;
  }
  stOutput << "<hr>" << this->ToString() << " = "
            << this->value.ToString() << "<hr>";
  return true;
}

bool CalculatorFunctionsGeneral::innerKostkaNumber(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerKostkaNumber");
  if (input.size()!=3)
    return false;
  KostkaNumber theKN;
  if (!theCommands.GetVectoRInt(input[1], theKN.partition) ||
      !theCommands.GetVectoRInt(input[2], theKN.tuple))
    return theCommands << "Failed to extract partition and tuple from input: " << input.ToString();
  std::stringstream out;
  HashedList<KostkaNumber> theKNs;
  if (theKN.Compute(&theKNs, &out))
    out << "<br>Final result: " << theKN.value.ToString();
  return output.AssignValue(out.str(), theCommands);
}

std::string SelectionFixedRankDifferentMaxMultiplicities::ToString()
{ std::stringstream out;
  Vector<int> theMults;
  theMults=this->Multiplicities;
  out << theMults;
  return out.str();
}

std::string SelectionFixedRankDifferentMaxMultiplicities::ToStringFull()
{ std::stringstream out;
  Vector<int> theMults, theMaxMults;
  theMaxMults=this->MaxMultiplicities;
  theMults=this->Multiplicities;
  out << "Multiplicities: " << theMults << "; max: " << theMaxMults
  << "; rank: " << this->rank;
  return out.str();
}

SelectionFixedRankDifferentMaxMultiplicities::SelectionFixedRankDifferentMaxMultiplicities()
{ this->flagFirstComputed=false;
}

bool SelectionFixedRankDifferentMaxMultiplicities::init()
{ this->flagFirstComputed=false;
  return true;
}

bool SelectionFixedRankDifferentMaxMultiplicities::firstIncrement()
{ MacroRegisterFunctionWithName("SelectionFixedRankDifferentMaxMultiplicities::firstIncrement");
  this->flagFirstComputed=true;
  this->Multiplicities.SetSize(this->MaxMultiplicities.size);
  int remainingRank=this->rank;
  for (int i=this->MaxMultiplicities.size-1; i>=0; i--)
  { if (this->MaxMultiplicities[i]<remainingRank)
      this->Multiplicities[i]=this->MaxMultiplicities[i];
    else
      this->Multiplicities[i]=remainingRank;
    remainingRank-=this->Multiplicities[i];
  }
  if (remainingRank>0)
    return false;
  return true;
}

bool SelectionFixedRankDifferentMaxMultiplicities::IncrementReturnFalseIfPastLast()
{ MacroRegisterFunctionWithName("SelectionFixedRankDifferentMaxMultiplicities::IncrementReturnFalseIfPastLast");
  if (this->rank<0)
    return false;
  if (!this->flagFirstComputed)
    return this->firstIncrement();
  int rankToRedistribute=0;
  for (int i=this->Multiplicities.size-2; i>=0; i--)
  { rankToRedistribute+=this->Multiplicities[i+1];
    this->Multiplicities[i+1]=0;
    if (this->Multiplicities[i]<this->MaxMultiplicities[i] && rankToRedistribute>0)
    { this->Multiplicities[i]++;
      rankToRedistribute--;
      for (int j=this->Multiplicities.size-1; j>i; j--)
      { if (this->MaxMultiplicities[j]<=rankToRedistribute)
          this->Multiplicities[j]=this->MaxMultiplicities[j];
        else
          this->Multiplicities[j]=rankToRedistribute;
        rankToRedistribute-=this->Multiplicities[j];
        if (rankToRedistribute==0)
          return true;
      }
      return true;
    }
  }
  return false;
}

bool CalculatorFunctionsGeneral::innerAllSelectionsFixedRank(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerKostkaNumber");
  if (input.size()!=3)
    return false;
  SelectionFixedRankDifferentMaxMultiplicities theSel;
  if (!input[1].IsSmallInteger(&theSel.rank))
    return false;
  if (!theCommands.GetVectoRInt(input[2], theSel.MaxMultiplicities))
    return theCommands << "Failed to extract list of multiplicities from "
    << input[2].ToString();
  if (theSel.rank<0)
    return output.AssignValue(0, theCommands);
  theSel.init();
  std::stringstream out;
  out << "Max multiplicities: " << theSel.MaxMultiplicities << " rank: "
  << theSel.rank;
  while (theSel.IncrementReturnFalseIfPastLast())
  { out << "<br>" << theSel.ToString();
  }
  return output.AssignValue(out.str(), theCommands);
}

bool CalculatorFunctionsGeneral::innerFloor(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerFloor");
  Rational theRat;
  if(input.IsOfType<Rational>(&theRat))
  { theRat.AssignFloor();
    return output.AssignValue(theRat, theCommands);
  }
  double theDouble=0;
  if (input.EvaluatesToDouble(&theDouble))
    return output.AssignValue((int) std::floor(theDouble), theCommands);
  return false;
}

bool CalculatorFunctionsGeneral::innerPlotSegment(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerPlotSegment");
  if (input.children.size<3)
    return false;
//  stOutput << "DEBUG: Here i am";
  const Expression& leftE=input[1];
  const Expression& rightE=input[2];
  Vector<Rational> leftV, rightV;
//  Vector<double> leftVd, rightVd;
  if (!theCommands.GetVectoR(leftE,leftV ) || !theCommands.GetVectoR(rightE, rightV))
    return false;
  if (leftV.size!=rightV.size)
    return false;
  if (leftV.size!=2 && leftV.size!=3)
    return false;
  int colorIndex=0;
  if (input.children.size>=4)
  { std::string theColor="black";
    int colorCandidate=0;
    const Expression& colorE=input[3];
    bool usingColorString= colorE.IsOfType<std::string>(&theColor);
    if (!usingColorString)
      usingColorString= colorE.IsAtom(&theColor);
    if (usingColorString)
    { usingColorString=DrawingVariables::GetColorIntFromColorString(theColor, colorIndex);
      if (!usingColorString)
        theCommands << "Unrecognized color: " << theColor;
    }
    if (!usingColorString)
      if(colorE.IsIntegerFittingInInt(&colorCandidate))
        colorIndex=colorCandidate;
//    stOutput << "Final color index: " << colorIndex;
  }
  PlotObject theSegment;
  theSegment.thePlotType="plotFunction";
  theSegment.colorRGB=colorIndex;
  if (leftV.size==3)
    theSegment.flagIs3d=true;
  theSegment.thePoints.AddOnTop(leftV.GetVectorDouble());
  theSegment.thePoints.AddOnTop(rightV.GetVectorDouble());
  Plot thePlot;
  thePlot+=theSegment;
  thePlot.DesiredHtmlWidthInPixels=200;
  thePlot.DesiredHtmlHeightInPixels=200;
  return output.AssignValue(thePlot, theCommands);
}

bool CalculatorFunctionsGeneral::innerThaw(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerThaw");
  if (!input.StartsWith(theCommands.opFreeze()))
  { output=input;
    return true;
  }
  if (input.size()>2)
  { output=input;
    return output.SetChildAtomValue(0, theCommands.opSequence());
  }
  if (input.size()==2)
  { output=input[1];
    return true;
  }
  return false;
}

bool CalculatorFunctionsGeneral::innerLCM(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerLCM");
  if (input.size()<3)
    return false;
  Vector<LargeInt> theInts;
  if (!theCommands.GetVectorLargeIntFromFunctionArguments(input, theInts))
    return false;
  if (theInts.size<1)//<-this shouldn't happen if GetVectorLargeIntFromFunctionArguments works as intended.
    return false;
  LargeIntUnsigned theResult=theInts[0].value;
  if (theResult==0)
    return false;
  for (int i=1; i<theInts.size; i++)
  { if (theInts[i].value==0)
      return false;
    theResult=LargeIntUnsigned::lcm(theResult, theInts[i].value);
  }
  return output.AssignValue(theResult, theCommands);
}

bool CalculatorFunctionsGeneral::innerGCD(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerGCD");
  if (input.size()<3)
    return false;
  Vector<LargeInt> theInts;
  if (!theCommands.GetVectorLargeIntFromFunctionArguments(input, theInts))
    return false;
  if (theInts.size<1)//<-this shouldn't happen if GetVectorLargeIntFromFunctionArguments works as intended.
    return false;
  LargeIntUnsigned theResult=theInts[0].value;
  if (theResult==0)
    return false;
  for (int i=1; i<theInts.size; i++)
  { if (theInts[i].value==0)
      return false;
    theResult=LargeIntUnsigned::gcd(theResult, theInts[i].value);
  }
  return output.AssignValue(theResult, theCommands);
}

bool CalculatorFunctionsGeneral::innerLogBaseSimpleCases(Calculator& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CalculatorFunctionsGeneral::innerLogBaseSimpleCases");
  //stOutput << "reducing ...." << input.ToStringSemiFull();
  if (!input.StartsWith(theCommands.opLogBase(), 3))
    return false;
  Rational theBase, theArg;
  if (!input[1].IsOfType<Rational>(&theBase) || !input[2].IsOfType<Rational>(&theArg))
    return false;
  if (theBase<0 || theArg<0)
    return false;
  if (theBase==1)
    return false;
  bool changed=false;
  int theSign=1;
  if (theBase<1)
  { theBase.Invert();
    theSign*=-1;
    changed=true;
  }
  if (theArg==1)
    return output.AssignValue(0, theCommands);
  if (theArg<1)
  { theArg.Invert();
    theSign*=-1;
    changed=true;
  }
  LargeInt baseInt, argNum;
  if (!theBase.IsInteger(&baseInt))
    return false;
  argNum=theArg.GetNumerator();
  LargeIntUnsigned argDen=theArg.GetDenominator();
  double doubleBase= baseInt.GetDoubleValue();
  double doubleArgNum=argNum.GetDoubleValue();
  if (FloatingPoint::log(doubleArgNum)/FloatingPoint::log(doubleBase)>1000)
    return false;
  int intPart=0;
  while (argNum% baseInt ==0)
  { intPart++;
    argNum/=baseInt;
    changed=true;
  }
  if (!changed)
    return false;
  intPart*=theSign;
  theArg=argNum;
  theArg/=argDen;
  Expression logPartE, newBase, newArg;
  newBase.AssignValue(theBase, theCommands);
  newArg.AssignValue(theArg, theCommands);
  logPartE.MakeXOX(theCommands, theCommands.opLogBase(), newBase, newArg);
  if (intPart==0)
  { output=logPartE;
    return true;
  }
  Expression intPartE, mOne;
  intPartE.AssignValue(intPart, theCommands);
  if (theSign<0)
  { mOne.AssignValue(-1, theCommands);
    logPartE*=mOne;
  }
  output=intPartE+logPartE;
  return true;
}
