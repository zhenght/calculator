//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#include "vpf.h"
#include "vpf1_5SubsetsSelections.h"
ProjectInformationInstance ProjectInfoVpf5cpp(__FILE__, "Implementation file for the calculator parser part 2: meant for built-in functions. ");

//If you get a the C++ multiple definition error:
//- Try moving template *EXPLICIT* specializations (i.e. template <>)
//  to the *beginning* (!) of .cpp files.
//- Try moving template generics into .h files.

template <>
bool ReflectionSubgroupWeylGroup::IsDominantWRTgenerator<RationalFunctionOld>(const Vector<RationalFunctionOld>& theWeight, int generatorIndex)
{ Vector<RationalFunctionOld> tempVect;
  RationalFunctionOld tempRF;
  tempVect=this->simpleGenerators[generatorIndex].GetVectorRational();
  tempRF=this->AmbientWeyl.RootScalarCartanRoot(theWeight, tempVect);
  if (tempRF.expressionType!=tempRF.typeRational)
  { std::cout << "This might or might not be a programming mistake: I am being asked whether a weight"
    << " with rational function coefficients is dominant. I took the scalar products with the positive simple roots "
    << " whose reflections generate the ambient group, however one of the scalar products in question was non-constant. "
    << " More precisely, the scalar product of " << theWeight.ToString() << " and " << tempVect.ToString()
    << " equals " << tempRF.ToString() << ". I cannot decide (more precisely, do not want to *silently* decide for you) "
    << " whether a non-constant function is positive or not. "
    << " If this is not a programming mistake, you might want to consider introducing a substitution "
    << " evaluating the rational function, some sort of a monomial order, or some other method of deciding the \"sign\" of a rational function."
    << " Whether or not this is a mistake, I am crashing. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
    return false;
  }
  return !tempRF.ratValue.IsNegative();
}

template <>
bool ReflectionSubgroupWeylGroup::IsDominantWRTgenerator<Rational>(const Vector<Rational>& theWeight, int generatorIndex)
{ return !this->AmbientWeyl.RootScalarCartanRoot
  (theWeight, this->simpleGenerators[generatorIndex]).IsNegative();
}

template <>
bool WeylGroup::IsDominantWRTgenerator<RationalFunctionOld>(const Vector<RationalFunctionOld>& theWeight, int generatorIndex)
{ Vector<Rational> tempVect;
  RationalFunctionOld tempRF;
  tempVect.MakeEi(this->GetDim(), generatorIndex);
  tempRF=this->RootScalarCartanRoot(theWeight, tempVect);
  if (tempRF.expressionType!=tempRF.typeRational)
  { std::cout << "This might or might not be a programming mistake: I am being asked whether a weight"
    << " with rational function coefficients is dominant. I took the scalar products with the positive simple roots "
    << " whose reflections generate the ambient group, however one of the scalar products in question was non-constant. "
    << " More precisely, the scalar product of " << theWeight.ToString() << " and " << tempVect.ToString()
    << " equals " << tempRF.ToString() << ". I cannot decide (more precisely, do not want to *silently* decide for you) "
    << " whether a non-constant function is positive or not. "
    << " If this is not a programming mistake, you might want to consider introducing a substitution "
    << " evaluating the rational function, some sort of a monomial order, or some other method of deciding the \"sign\" of a rational function."
    << " Whether or not this is a mistake, I am crashing.  "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
    return false;
  }
  return !tempRF.ratValue.IsNegative();
}

template <>
bool WeylGroup::IsDominantWRTgenerator<Rational>(const Vector<Rational>& theWeight, int generatorIndex)
{ return !this->GetScalarProdSimpleRoot(theWeight, generatorIndex).IsNegative();
}

void ReflectionSubgroupWeylGroup::MakeParabolicFromSelectionSimpleRoots
(WeylGroup& inputWeyl, const Vector<Rational> & ZeroesMeanSimpleRootSpaceIsInParabolic, GlobalVariables& theGlobalVariables, int UpperLimitNumElements)
{ Selection tempSel;
  tempSel=ZeroesMeanSimpleRootSpaceIsInParabolic;
  this->MakeParabolicFromSelectionSimpleRoots(inputWeyl, tempSel, theGlobalVariables, UpperLimitNumElements);
}

bool ReflectionSubgroupWeylGroup::GetAlLDominantWeightsHWFDIMwithRespectToAmbientAlgebra
(Vector<Rational>& highestWeightSimpleCoords, HashedList<Vector<Rational> >& outputWeightsSimpleCoords,
 int upperBoundDominantWeights, std::string& outputDetails, GlobalVariables& theGlobalVariables)
{ std::stringstream out;
//  double startTime=theGlobalVariables.GetElapsedSeconds();
//  std::cout << "<br>time elapsed: " << theGlobalVariables.GetElapsedSeconds()-startTime;
  Vector<Rational> highestWeightTrue=highestWeightSimpleCoords;
  Vectors<Rational> basisEi;
  int theDim=this->AmbientWeyl.GetDim();
  basisEi.MakeEiBasis(theDim);
  this->RaiseToDominantWeight(highestWeightTrue);
  Vector<Rational> highestWeightFundCoords=this->AmbientWeyl.GetFundamentalCoordinatesFromSimple(highestWeightTrue);
  if (!highestWeightFundCoords.SumCoords().IsSmallInteger())
    return false;
  int theTopHeightSimpleCoords=(int) highestWeightSimpleCoords.SumCoords().DoubleValue()+1;
//  int theTopHeightFundCoords=(int) highestWeightFundCoords.SumCoords().DoubleValue();
  if (theTopHeightSimpleCoords<0)
    theTopHeightSimpleCoords=0;
  List<HashedList<Vector<Rational> > > outputWeightsByHeight;
  int topHeightRootSystem=this->AmbientWeyl.RootsOfBorel.LastObject()->SumCoords().NumShort;
  int topHeightRootSystemPlusOne=topHeightRootSystem+1;
  outputWeightsByHeight.SetSize(topHeightRootSystemPlusOne);
  int finalHashSize=100;
  for (int i=0; i<topHeightRootSystemPlusOne; i++)
    outputWeightsByHeight[i].SetHashSizE(finalHashSize);
  outputWeightsSimpleCoords.Clear();
  outputWeightsByHeight[0].AddOnTop(highestWeightTrue);
  int numTotalWeightsFound=0;
  int numPosRoots=this->AmbientWeyl.RootsOfBorel.size;
  Vector<Rational> currentWeight, currentWeightRaisedToDominantWRTAmbientAlgebra;
//  std::cout << "<br>time spend before working cycle: " << theGlobalVariables.GetElapsedSeconds()-startTime;
  for (int lowestUnexploredHeightDiff=0; lowestUnexploredHeightDiff<=theTopHeightSimpleCoords;
  lowestUnexploredHeightDiff++)
  { //double startCycleTime=theGlobalVariables.GetElapsedSeconds();
    if (upperBoundDominantWeights>0 && numTotalWeightsFound>upperBoundDominantWeights)
      break;
    int bufferIndexShift=lowestUnexploredHeightDiff%topHeightRootSystemPlusOne;
    HashedList<Vector<Rational> >& currentHashes=outputWeightsByHeight[bufferIndexShift];
    for (int lowest=0; lowest<currentHashes.size; lowest++)
      for (int i=0; i<numPosRoots; i++)
      { currentWeight=currentHashes[lowest];
        currentWeight-=this->AmbientWeyl.RootsOfBorel[i];
        if (this->IsDominantWeight(currentWeight))
        { currentWeightRaisedToDominantWRTAmbientAlgebra=currentWeight;
          this->AmbientWeyl.RaiseToDominantWeight(currentWeightRaisedToDominantWRTAmbientAlgebra);
          currentWeightRaisedToDominantWRTAmbientAlgebra-=highestWeightTrue;
          if (currentWeightRaisedToDominantWRTAmbientAlgebra.IsNegativeOrZero())
          { int currentIndexShift=this->AmbientWeyl.RootsOfBorel[i].SumCoords().NumShort;
            currentIndexShift=(currentIndexShift+bufferIndexShift)%topHeightRootSystemPlusOne;
            if (outputWeightsByHeight[currentIndexShift].AddOnTopNoRepetition(currentWeight))
            { numTotalWeightsFound++;
              outputWeightsByHeight[currentIndexShift].AdjustHashes();
            }
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

bool CommandList::fWeylDimFormula
(CommandList& theCommands, const Expression& input, Expression& output)
{ RecursionDepthCounter recursionCounter(&theCommands.RecursionDeptH);
  if (!input.IsListNElements(3))
    return output.SetError("This function takes 2 arguments", theCommands);

  std::string errorString;
  SemisimpleLieAlgebra* theSSowner;
  if (!theCommands.CallConversionFunctionReturnsNonConstUseCarefully
      (Serialization::innerSSLieAlgebra, input[1], theSSowner, &errorString))
    return output.SetError(errorString, theCommands);

  Vector<RationalFunctionOld> theWeight;
  Expression newContext(theCommands);
//  std::cout << "<br>input[2] is: " << input[2].ToString();
  if (!theCommands.GetVectoR<RationalFunctionOld>
      (input[2], theWeight, &newContext, theSSowner->GetRank(),
       theCommands.innerRationalFunction))
    return output.SetError
    ("Failed to convert the argument of the function to a highest weight vector", theCommands);
  RationalFunctionOld rfOne;
  rfOne.MakeOne(theCommands.theGlobalVariableS);
  Vector<RationalFunctionOld> theWeightInSimpleCoords;
  FormatExpressions theFormat;
  newContext.ContextGetFormatExpressions(theFormat);
  theWeightInSimpleCoords =
  theSSowner->theWeyl.GetSimpleCoordinatesFromFundamental(theWeight);
  //std::cout << "The fundamental coords: " << theWeight.ToString();
  theCommands.Comments << "<br>Weyl dim formula input: simple coords: "
  << theWeightInSimpleCoords.ToString(&theFormat) << ", fundamental coords: "
  << theWeight.ToString(&theFormat);
  RationalFunctionOld tempRF=
  theSSowner->theWeyl.WeylDimFormulaSimpleCoords(theWeightInSimpleCoords);
  //std::cout << "<br>The result: " << tempRF.ToString();
  return output.AssignValueWithContext(tempRF, newContext, theCommands);
}

bool CommandList::fAnimateLittelmannPaths
(CommandList& theCommands, const Expression& input, Expression& output)
{ RecursionDepthCounter recursionCounter(&theCommands.RecursionDeptH);
  if (!input.IsListNElements(3))
    return output.SetError("This function takes 2 arguments", theCommands);
  SemisimpleLieAlgebra* theSSowner=0;
  std::string errorMessage;
  if (!theCommands.CallConversionFunctionReturnsNonConstUseCarefully
      (Serialization::innerSSLieAlgebra, input[1], theSSowner, &errorMessage))
    return output.SetError(errorMessage, theCommands);
  Vector<Rational> theWeight;
  Expression tempContext(theCommands);
  if (!theCommands.GetVectoR<Rational>
      (input[2], theWeight, &tempContext, theSSowner->GetRank(), 0))
    return output.SetError
    ("Failed to convert the argument of the function to a highest weight vector", theCommands);
  Vector<Rational> theWeightInSimpleCoords;
  theWeightInSimpleCoords = theSSowner->theWeyl.GetSimpleCoordinatesFromFundamental(theWeight);
  //std::cout << "The fundamental coords: " << theWeight.ToString();
  theCommands.Comments
  << "<br>Function fAnimateLittelmannPaths: your input in simple coords: "
  << theWeightInSimpleCoords.ToString();
  LittelmannPath thePath;
  thePath.MakeFromWeightInSimpleCoords(theWeightInSimpleCoords, theSSowner->theWeyl);
  return output.AssignValue
  (thePath.GenerateOrbitAndAnimate(*theCommands.theGlobalVariableS), theCommands);
}

void CommandList::GetOutputFolders
(const DynkinType& theType, std::string& outputFolderPhysical, std::string& outputFolderDisplay,
 FormatExpressions& outputFormat)
{ std::stringstream outMainPath, outMainDisplayPath;
  outMainPath << this->PhysicalPathOutputFolder << theType.ToString() << "/";
  outputFolderPhysical=outMainPath.str();
  outMainDisplayPath << this->DisplayPathOutputFolder << theType.ToString() << "/";
  outputFolderDisplay=outMainDisplayPath.str();
  outputFormat.flagUseHTML=true;
  outputFormat.flagUseLatex=false;
  outputFormat.flagUsePNG=true;
  outputFormat.PathDisplayNameCalculator=this->DisplayNameCalculator;
  outputFormat.PathPhysicalOutputFolder=outputFolderPhysical;
  outputFormat.PathDisplayOutputFolder=outputFolderDisplay;
  outputFormat.PathDisplayServerBaseFolder=this->DisplayPathServerBase;
}

bool CommandList::innerRootSAsAndSltwos
(CommandList& theCommands, const Expression& input, Expression& output, bool showSLtwos)
{ MacroRegisterFunctionWithName("CommandList::innerRootSAsAndSltwos");
  //bool showIndicator=true;
  std::string errorString;
  SemisimpleLieAlgebra* ownerSS;
  if (!theCommands.CallConversionFunctionReturnsNonConstUseCarefully
      (Serialization::innerSSLieAlgebra, input, ownerSS, &errorString))
    return output.SetError(errorString, theCommands);
  CGI::SetCGIServerIgnoreUserAbort();
  std::string outMainDisplayPatH, outMainPatH;
  std::stringstream outSltwoPath, outSltwoDisplayPath;
  theCommands.theGlobalVariableS->MaxComputationTimeSecondsNonPositiveMeansNoLimit=10000;
  FormatExpressions theFormat;
  theCommands.GetOutputFolders(ownerSS->theWeyl.theDynkinType, outMainPatH, outMainDisplayPatH, theFormat);
  outSltwoPath << outMainPatH << "sl2s/";
  outSltwoDisplayPath << outMainDisplayPatH << "sl2s/";
  bool NeedToCreateFolders=(!CGI::FileExists(outMainPatH) || !CGI::FileExists(outSltwoPath.str()));
  if (NeedToCreateFolders)
  { std::stringstream outMkDirCommand1, outMkDirCommand2;
    outMkDirCommand1 << "mkdir " << outMainPatH;
    outMkDirCommand2 << "mkdir " << outSltwoPath.str();
    theCommands.theGlobalVariableS->System(outMkDirCommand1.str());
    theCommands.theGlobalVariableS->System(outMkDirCommand2.str());
  }
  std::stringstream outRootHtmlFileName, outRootHtmlDisplayName, outSltwoMainFile, outSltwoFileDisplayName;
  outSltwoMainFile << outSltwoPath.str() << "sl2s.html";
  outSltwoFileDisplayName << outSltwoDisplayPath.str() << "sl2s.html";
  outRootHtmlFileName << outMainPatH << "rootHtml.html";
  outRootHtmlDisplayName << outMainDisplayPatH << "rootHtml.html";
  bool mustRecompute=false;
  theCommands.theGlobalVariableS->MaxComputationTimeSecondsNonPositiveMeansNoLimit =1000;

  if (!CGI::FileExists(outSltwoMainFile.str()) || !CGI::FileExists(outRootHtmlFileName.str()))
    mustRecompute=true;
  std::stringstream out;
  if (mustRecompute)
  { //std::cout << theCommands.javaScriptDisplayingIndicator;
    std::cout.flush();
    std::cout
    << "<br>The computation is in progress. <b>"
    << "<br>Please do not click back/refresh button: "
    << "it will cause broken links in the calculator. "
    << "<br>Appologies for this technical (Apache server configuration) problem. "
    << "<br>Alleviating it is around the bottom of a very long to-do list.</b>"
    << "<br> The computation is slow, up to around 10 minutes for E_8.<br>";
    SltwoSubalgebras theSl2s(*ownerSS);
    ownerSS->FindSl2Subalgebras(*ownerSS, theSl2s, *theCommands.theGlobalVariableS);
    std::string PathSl2= outSltwoPath.str();
    std::string DisplayPathSl2=outSltwoDisplayPath.str();
    theSl2s.ElementToHtml(&theFormat, theCommands.theGlobalVariableS);
    theCommands.SystemCommands.AddListOnTop(theSl2s.listSystemCommandsLatex);
    theCommands.SystemCommands.AddListOnTop(theSl2s.listSystemCommandsDVIPNG);
  } else
    out << "The table is precomputed and served from the hard disk. ";
  out << "<a href=\""
  << (showSLtwos ? outSltwoFileDisplayName.str() : outRootHtmlDisplayName.str())
  << "\">See the result in a separate file/page. </a>";
  out << "<meta http-equiv=\"refresh\" content=\"1; url="
  << (showSLtwos ? outSltwoFileDisplayName.str() : outRootHtmlDisplayName.str())
  << "\">";
  return output.AssignValue(out.str(), theCommands);
}

bool CommandList::fDecomposeFDPartGeneralizedVermaModuleOverLeviPart
(CommandList& theCommands, const Expression& input, Expression& output)
{ RecursionDepthCounter recursionCounter(&theCommands.RecursionDeptH);
  if (!input.IsListNElements(5))
    return output.SetError
    ("Function fDecomposeFDPartGeneralizedVermaModuleOverLeviPart expects 4 arguments.", theCommands);
  const Expression& typeNode=input[1];
  const Expression& weightNode=input[2];
  const Expression& inducingParNode=input[3];
  const Expression& splittingParNode=input[4];
  SemisimpleLieAlgebra* ownerSSPointer=0;
  std::string errorString;
  if (! theCommands.CallConversionFunctionReturnsNonConstUseCarefully
      (Serialization::innerSSLieAlgebra, typeNode, ownerSSPointer, &errorString))
    return output.SetError(errorString, theCommands);
  Vector<RationalFunctionOld> theWeightFundCoords;
  Vector<Rational> inducingParSel, splittingParSel;
  SemisimpleLieAlgebra& ownerSS=*ownerSSPointer;
  WeylGroup& theWeyl=ownerSS.theWeyl;
  int theDim=ownerSS.GetRank();
  Expression finalContext;
  if (!theCommands.GetVectoR<RationalFunctionOld>
      (weightNode, theWeightFundCoords, &finalContext, theDim, theCommands.innerRationalFunction))
    return output.SetError
    ("Failed to extract highest weight from the second argument.", theCommands)
    ;
  if (!theCommands.GetVectoR<Rational>
      (inducingParNode, inducingParSel, &finalContext, theDim, 0))
    return output.SetError("Failed to extract parabolic selection from the third argument", theCommands);
  if (!theCommands.GetVectoR<Rational>
      (splittingParNode, splittingParSel, &finalContext, theDim, 0))
    return output.SetError("Failed to extract parabolic selection from the fourth argument", theCommands);
  theCommands.Comments
  << "Your input weight in fundamental coordinates: "
  << theWeightFundCoords.ToString();
  theCommands.Comments << "<br>Your input weight in simple coordinates: "
  << theWeyl.GetSimpleCoordinatesFromFundamental(theWeightFundCoords).ToString()
  << "<br>Your inducing parabolic subalgebra: " << inducingParSel.ToString() << "."
  <<"<br>The parabolic subalgebra I should split over: " << splittingParSel.ToString()
  << ".";
  ModuleSSalgebra<RationalFunctionOld> theMod;
  Selection selInducing= inducingParSel;
  Selection selSplittingParSel=splittingParSel;
  theMod.MakeFromHW
  (ownerSS, theWeightFundCoords, selInducing, *theCommands.theGlobalVariableS, 1, 0, 0, false);
  std::string report;
  theMod.SplitOverLevi(&report, selSplittingParSel, *theCommands.theGlobalVariableS, 1, 0);
  return output.AssignValue(report, theCommands);
}

bool CommandList::fCasimir
(CommandList& theCommands, const Expression& input, Expression& output)
{ RecursionDepthCounter recursionCounter(&theCommands.RecursionDeptH);
  std::string errorString;
  SemisimpleLieAlgebra* theSSalg;
  if (!theCommands.CallConversionFunctionReturnsNonConstUseCarefully
      (Serialization::innerSSLieAlgebra, input, theSSalg, &errorString))
    return output.SetError(errorString, theCommands);
  SemisimpleLieAlgebra& theSSowner=*theSSalg;
  if (theCommands.theGlobalVariableS->MaxComputationTimeSecondsNonPositiveMeansNoLimit<50)
    theCommands.theGlobalVariableS->MaxComputationTimeSecondsNonPositiveMeansNoLimit=50;
  RationalFunctionOld rfOne, rfZero;
  rfOne.MakeOne(theCommands.theGlobalVariableS);
  rfZero.MakeZero(theCommands.theGlobalVariableS);
  ElementUniversalEnveloping<RationalFunctionOld> theCasimir;
  theCasimir.MakeCasimir(theSSowner, *theCommands.theGlobalVariableS, rfOne, rfZero);
//  theCasimir.Simplify(*theCommands.theGlobalVariableS);
  theCommands.Comments << "Context Lie algebra: "
  << ". The coefficient: " << theSSowner.theWeyl.GetKillingDivTraceRatio().ToString()
  <<  ". The Casimir element of the ambient Lie algebra. ";
  output.AssignValue(theCasimir, theCommands);
  return true;
}

bool CommandList::innerDrawWeightSupportWithMults
(CommandList& theCommands, const Expression& input, Expression& output)
{ //theNode.owner->theHmm.MakeG2InB3(theParser, theGlobalVariables);
  if (!input.IsListNElements(3))
    return output.SetError
    ("Error: the function for drawing weight support takes two  arguments (type and highest weight)",
     theCommands);
  const Expression& typeNode=input[1];
  const Expression& hwNode=input[2];
  SemisimpleLieAlgebra* theSSalgpointer;
  std::string errorString;
  if (!theCommands.CallConversionFunctionReturnsNonConstUseCarefully
      (Serialization::innerSSLieAlgebra, typeNode, theSSalgpointer, &errorString))
    return output.SetError(errorString, theCommands);
  Vector<Rational> highestWeightFundCoords;
  Expression theContext;
  if (!theCommands.GetVectoR<Rational>
      (hwNode, highestWeightFundCoords, &theContext, theSSalgpointer->GetRank(), 0))
    return output.SetError("Failed to extract highest weight vector", theCommands);
  Vector<Rational> highestWeightSimpleCoords;
  WeylGroup& theWeyl=theSSalgpointer->theWeyl;
  highestWeightSimpleCoords= theWeyl.GetSimpleCoordinatesFromFundamental(highestWeightFundCoords);
  //Vectors<Rational> theWeightsToBeDrawn;
  std::stringstream out;
  charSSAlgMod<Rational> theChar;
  theChar.MakeFromWeight(highestWeightSimpleCoords, theSSalgpointer);
  DrawingVariables theDV;
  std::string report;
  theChar.DrawMeWithMults(report, *theCommands.theGlobalVariableS, theDV, 10000);
  out << report << theDV.GetHtmlFromDrawOperationsCreateDivWithUniqueName(theWeyl.GetDim());
  return output.AssignValue(out.str(), theCommands);
}

bool CommandList::innerDrawRootSystem
(CommandList& theCommands, const Expression& input, Expression& output)
{ //theNode.owner->theHmm.MakeG2InB3(theParser, theGlobalVariables);
  std::string errorString;
  bool hasPreferredProjectionPlane= input.IsListNElements(4);
  const Expression& typeNode= hasPreferredProjectionPlane ? input[1] : input;
  SemisimpleLieAlgebra* theAlgPointer;
  if (!theCommands.CallConversionFunctionReturnsNonConstUseCarefully
      (Serialization::innerSSLieAlgebra, typeNode, theAlgPointer, &errorString))
    return output.SetError(errorString, theCommands);
  SemisimpleLieAlgebra& theAlg=*theAlgPointer;
  WeylGroup& theWeyl=theAlg.theWeyl;
  Vectors<Rational> preferredProjectionPlane;
  if (hasPreferredProjectionPlane)
  { preferredProjectionPlane.SetSize(2);
    bool isGood=
    theCommands.GetVectoR(input[2], preferredProjectionPlane[0], 0, theWeyl.GetDim(), 0) &&
    theCommands.GetVectoR(input[3], preferredProjectionPlane[1], 0, theWeyl.GetDim(), 0);
    if (!isGood)
      return output.SetError
      ("Failed to convert second or third argument to vector of desired dimension", theCommands);
  }
  std::stringstream out;
  DrawingVariables theDV;
  theWeyl.DrawRootSystem(theDV, true, *theCommands.theGlobalVariableS, false, 0, true, 0);
  if (hasPreferredProjectionPlane)
  { theDV.flagFillUserDefinedProjection=true;
    theDV.FillUserDefinedProjection=preferredProjectionPlane;
  }
  out << theDV.GetHtmlFromDrawOperationsCreateDivWithUniqueName(theWeyl.GetDim());
  return output.AssignValue(out.str(), theCommands);
}

bool CommandList::innerDrawWeightSupport
(CommandList& theCommands, const Expression& input, Expression& output)
{ //theNode.owner->theHmm.MakeG2InB3(theParser, theGlobalVariables);
  if (!input.IsListNElements(3))
    return output.SetError("Wrong number of arguments, must be 2. ", theCommands);
  const Expression& typeNode=input[1];
  const Expression& hwNode=input[2];
  std::string errorString;
  SemisimpleLieAlgebra* theAlgPointer;
  if (!theCommands.CallConversionFunctionReturnsNonConstUseCarefully
      (Serialization::innerSSLieAlgebra, typeNode, theAlgPointer, &errorString))
    return output.SetError(errorString, theCommands);
  SemisimpleLieAlgebra& theAlg=*theAlgPointer;
  Vector<Rational> highestWeightFundCoords;
  Expression tempContext;
  if (!theCommands.GetVectoR<Rational>
      (hwNode, highestWeightFundCoords, &tempContext, theAlg.GetRank(), 0))
    return false;
  Vector<Rational> highestWeightSimpleCoords;
  WeylGroup& theWeyl=theAlg.theWeyl;
  highestWeightSimpleCoords= theWeyl.GetSimpleCoordinatesFromFundamental(highestWeightFundCoords);
  //Vectors<Rational> theWeightsToBeDrawn;
  std::stringstream out;
  charSSAlgMod<Rational> theChar;
  theChar.MakeFromWeight(highestWeightSimpleCoords, theAlgPointer);
  DrawingVariables theDV;
  std::string report;
  theChar.DrawMeNoMults(report, *theCommands.theGlobalVariableS, theDV, 10000);
  out << report << theDV.GetHtmlFromDrawOperationsCreateDivWithUniqueName(theWeyl.GetDim());
  return output.AssignValue(out.str(), theCommands);
}

bool CommandList::fEmbedG2inB3
(CommandList& theCommands, const Expression& input, Expression& output)
{ theCommands.innerElementUniversalEnvelopingAlgebra(theCommands, input, output);
  if (!output.IsOfType<ElementUniversalEnveloping<RationalFunctionOld> >())
    return output.SetError
    ("Failed to convert argument to element of the Universal enveloping algebra. ", theCommands);
  SemisimpleLieAlgebra& ownerSS=*output.GetAmbientSSAlgebraNonConstUseWithCaution();
  if (!ownerSS.IsOfSimpleType('G', 2))
    return output.SetError
    ("Error: embedding of G_2 in B_3 takes elements of U(G_2) as arguments.", theCommands);
  HomomorphismSemisimpleLieAlgebra theHmm;
  theCommands.MakeHmmG2InB3(theHmm);

  ElementUniversalEnveloping<RationalFunctionOld> argument=
  output.GetValuE<ElementUniversalEnveloping<RationalFunctionOld> >();
  ElementUniversalEnveloping<RationalFunctionOld> outputUE;
  if(!theHmm.ApplyHomomorphism(argument, outputUE, *theCommands.theGlobalVariableS))
    return output.SetError("Failed to apply homomorphism for unspecified reason", theCommands);
//  std::cout << theHmm.ToString(*theCommands.theGlobalVariableS);
  outputUE.Simplify(*theCommands.theGlobalVariableS, 1, 0);
  return output.AssignValue(outputUE, theCommands);
}

std::string CGI::GetAnimateShowHideJavascriptMustBEPutInHTMLHead()
{ std::stringstream out;
  out << "<script src=\"//ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js\" type=\"text/javascript\"></script>";
  return out.str();
}

std::string CGI::GetSliderSpanStartsHidden(const std::string& content, const std::string& label, const std::string& desiredID)
{ std::stringstream out;
  CGI::GlobalFormulaIdentifier++;
  std::stringstream idStringStream;
  idStringStream << desiredID;
  if (desiredID=="")
    idStringStream << "UnnamedSpan" << CGI::GlobalFormulaIdentifier;
  out << "<a href=\"javascript:;\" onmusedown=\"document.getElementById('"  << idStringStream.str() << "').slideToggle('slow');\">Expand/collapse</a>";
  out << "<span id=\"" << idStringStream.str() << "\" style=\"display:none\">" << content << "</span>";
  return out.str();
}

std::string LittelmannPath::GenerateOrbitAndAnimate(GlobalVariables& theGlobalVariables)
{ std::stringstream out;
  List<LittelmannPath> theOrbit;
  List<List<int> > theGens;
  if (!
  this->GenerateOrbit(theOrbit, theGens, theGlobalVariables, 1000, 0))
  { out  << "<b>Not all paths were genenerated, only the first " << theOrbit.size << "</b>";
  }
  AnimationBuffer theBuffer;
//  int theDim=this->owner->GetDim();
  Vectors<double> coxPlane;
  coxPlane.SetSize(2);
  Vectors<double> draggableBAsis;
  this->owner->GetCoxeterPlane(coxPlane[0], coxPlane[1], theGlobalVariables);
//  theBuffer.theBuffer.initDimensions(this->owner.CartanSymmetric, draggableBAsis, coxPlane, 1);
  DrawingVariables tempDV, tempDV2;
  this->owner->DrawRootSystem(tempDV, true, theGlobalVariables, true);
  this->owner->DrawRootSystem(tempDV2, true, theGlobalVariables, true);
  theBuffer.theBuffer=tempDV.theBuffer;
  theBuffer.theFrames.SetSize(theOrbit.size);
  for (int i=0; i<theOrbit.size; i++)
  { DrawingVariables& currentOps=theBuffer.theFrames[i];
    LittelmannPath& currentPath=theOrbit[i];
    currentOps.theBuffer=tempDV.theBuffer;
    for (int j=0; j<currentPath.Waypoints.size; j++)
    { if (j!=currentPath.Waypoints.size-1)
      { currentOps.drawLineBetweenTwoVectorsBuffer(currentPath.Waypoints[j], currentPath.Waypoints[j+1], DrawingVariables::PenStyleNormal, 0);
        tempDV2.theBuffer.drawLineBetweenTwoVectorsBuffer(currentPath.Waypoints[j], currentPath.Waypoints[j+1], DrawingVariables::PenStyleNormal, 0);
      }
      currentOps.drawCircleAtVectorBuffer(currentPath.Waypoints[j], 2, DrawingVariables::PenStyleNormal, 0);
      tempDV2.theBuffer.drawCircleAtVectorBuffer(currentPath.Waypoints[j], 4, DrawingVariables::PenStyleNormal, 0);
    }
  }
  out << "<br>Animation of the Littelmann paths follows.";
  out << theBuffer.GetHtmlFromDrawOperationsCreateDivWithUniqueName(this->owner->GetDim());
  out << "<br>Here are all Littelmann paths drawn simultaneously. ";
  out << tempDV2.GetHtmlFromDrawOperationsCreateDivWithUniqueName(this->owner->GetDim());
  out << "Littelmann paths in simple coordinates given in the order in which they are generated (" << theOrbit.size << " total):<br>";
  out << "<table>";
  for (int i=0; i<theOrbit.size; i++)
  { LittelmannPath& currentPath=theOrbit[i];
    out << "<tr><td>" << currentPath.ToString() << "</td>"
    << "<td>" << this->ElementToStringOperatorSequenceStartingOnMe(theGens[i]) << "</td></tr>";
  }
  out << "</table>";
  LittelmannPath lastPath=theOrbit[0];
  LittelmannPath tempPath;
  MonomialTensor<int, MathRoutines::IntUnsignIdentity> tempMon;
  tempMon=*theGens.LastObject();
  tempMon.generatorsIndices.ReverseOrderElements();
  tempMon.Powers.ReverseOrderElements();
  out << "<table>";
  for (int i=tempMon.generatorsIndices.size-1; i>=1; i--)
  { int curInd=-tempMon.generatorsIndices[i]-1;
    int nextInd=-tempMon.generatorsIndices[i-1] -1;
    for (int k=0; k<tempMon.Powers[i]; k++)
      lastPath.ActByFalpha(curInd);
    tempPath=lastPath;
    tempPath.ActByEalpha(nextInd);
    out << "<tr><td> e_" << nextInd+1 << "("
    << lastPath.ToString() << ") =</td>" <<"<td>" << tempPath.ToString() << "</td>";
/*    for (int j=0; j<this->owner->GetDim(); j++)
    { tempPath=lastPath;
      tempPath.ActByEalpha(j);
      out << "<td> e_" << j+1 << "("
      << lastPath.ToString() << ")=</td>" <<"<td>" << tempPath.ToString() << "</td>";
    }*/
    out << "</tr>";
  }
  out << "</table>";
  out << "<table><td>corresponding element of U(g)</td><td>is adapted</td><td>path</td><td>e operators with non-zero action.</td>";
  for (int i=0; i<theGens.size; i++)
  { //tempPath.MakeFromWeightInSimpleCoords(*theOrbit[i].Waypoints.LastObject(), *this->owner);
    //tempPath=*theOrbit.LastObject();
    tempPath=theOrbit[i];
    tempMon=theGens[i];
    tempMon.generatorsIndices.ReverseOrderElements();
    tempMon.Powers.ReverseOrderElements();
    bool isadapted=tempPath.IsAdaptedString(tempMon);
    out << "<tr><td>" << tempMon.ToString() << "</td><td>"
    << (isadapted ? "is adapted to" : "is not adapted to" )
    <<  "</td><td>" << tempPath.ToString() << "</td><td>";
    for (int j=0; j<this->owner->GetDim(); j++)
    { tempPath=theOrbit[i];
      tempPath.ActByEFDisplayIndex(j+1);
      if (!tempPath.IsEqualToZero())
        out << "e_{" << j+1 << "}, ";
      tempPath=theOrbit[i];
      tempPath.ActByEFDisplayIndex(-j-1);
      if (!tempPath.IsEqualToZero())
        out << "e_{" << -j-1 << "}, ";
    }
    out << "</td></tr>";
  }
  out << "</table>";
  return out.str();
}

std::string AnimationBuffer::GetHtmlFromDrawOperationsCreateDivWithUniqueName(int theDimension)
{ if (theDimension<2)
    return "<br><b>Pictures of dimension less than two are not drawn.</b><br>";
  if (theFrames.size<1)
    return "<br><b>Empty animation</b>";
  std::stringstream out, tempStream1, tempStream2, tempStream3, tempStream4, tempStream5, tempStream6;
  std::stringstream tempStream7, tempStream8, tempStream9, tempStream10, tempStream11,
  tempStream12, tempStream13, tempStream14, tempStream15, tempStream4_1, tempStream11_1;
  DrawingVariables* boss=&this->theFrames[0];
  this->theBuffer=boss->theBuffer;
  boss->NumHtmlGraphics++;
  int timesCalled=boss->NumHtmlGraphics;
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
  tempStream4_1 << "colorsLines" << timesCalled;
  std::string Points1ArrayNameColors=tempStream4_1.str();

  tempStream10 << "pts2" << timesCalled;
  std::string Points2ArrayName=tempStream10.str();

  tempStream11 << "circ" << timesCalled;
  std::string circArrayName=tempStream11.str();
  tempStream11_1 << "circRadii" << timesCalled;
  std::string circRadiiArrayName=tempStream11_1.str();

  tempStream12 << "txt" << timesCalled;
  std::string txtArrayName=tempStream12.str();

  tempStream6 << "basisCircles" << timesCalled;
  std::string basisCircles = tempStream6.str();
  tempStream13 << "proj" << timesCalled;
  std::string projName = tempStream13.str();
  tempStream14 << "projCirc" << timesCalled;
  std::string projBasisCircles= tempStream14.str();
  tempStream15 << "eiBasis" << timesCalled;
  std::string eiBasis= tempStream15.str();

  tempStream7 << "shiftXCone" << timesCalled;
  std::string shiftX=tempStream7.str();
  tempStream8 << "shiftYCone" << timesCalled;
  std::string shiftY=tempStream8.str();
  tempStream9 << "convXY" << timesCalled;
  std::string functionConvertToXYName=tempStream9.str();
  out << "<script type=\"text/javascript\" src=\"http://ajax.googleapis.com/ajax/libs/dojo/1.6.1/dojo/dojo.xd.js\""
  << " djConfig = \"parseOnLoad: true\"></script>";
  out << "<button" << CGI::GetStyleButtonLikeHtml()
  << "onclick=\"dojoOnLoadDoesntWork" << timesCalled << "();\" >"
  << "click if graphics doesn't show</button>";
  out << "<div style=\"width:" << boss->DefaultHtmlWidth << ";height:" << boss->DefaultHtmlHeight << ";border:solid 1px\" id=\"" << theCanvasId
  << "\" onmousedown=\"clickCanvasCone" << timesCalled << "(event.clientX, event.clientY);\" onmouseup=\"selectedBasisIndexCone" << timesCalled
  << "=-1;\" onmousemove=\"mouseMoveRedrawCone" <<  timesCalled << "(event.clientX, event.clientY);\" "
  << "onmousewheel=\"mouseHandleWheelCone" << timesCalled << "(event);\""
  << "></div>";
  out
  << "<br>The projection plane (drawn on the screen) is spanned by the following two vectors<br> \n";
  List<List<std::string> > textEbasisNamesUserInput;
  List<List<std::string> > textEbasisNamesReadOnly;
  textEbasisNamesUserInput.SetSize(2);
  textEbasisNamesReadOnly.SetSize(2);
  for (int i=0; i<2; i++)
  { textEbasisNamesReadOnly[i].SetSize(theDimension);
    for (int j=0; j<theDimension; j++)
    { std::stringstream tmpStream;
      tmpStream << "textEbasisReadOnly" << timesCalled << "_" << i << "_" << j;
      textEbasisNamesReadOnly[i][j]=tmpStream.str();
      out << "<span id=\"" << textEbasisNamesReadOnly[i][j]
      << "\"> </span>\n";
    }
    out << "<br>";
  }
  for (int i=0; i<2; i++)
  { textEbasisNamesUserInput[i].SetSize(theDimension);
    for (int j=0; j<theDimension; j++)
    { std::stringstream tmpStream, tmpStream2;
      tmpStream << "textEbasisUserInput" << timesCalled << "_" << i << "_" << j;
      textEbasisNamesUserInput[i][j]=tmpStream.str();
      out << "<textarea rows=\"1\" cols=\"2\" id=\"" << textEbasisNamesUserInput[i][j]
      << "\">" << "</textarea>\n";
    }
    out << "<br>";
  }
  out << "<button onclick=\"startProjectionPlaneUser" << timesCalled << "();\">Change to basis</button><br>";
  out
  << "The bilinear form of the vector space follows. The ij^th element "
  << " gives the scalar product of e_i and e_j. If you enter a degenerate or non-positive definite "
  << " symmetric bilinear form"
  << " the javascript might crash. You are expected to enter a symmetric strictly positive definite matrix. <br> \n";
  for (int i=0; i<this->theBuffer.theBilinearForm.NumRows; i++)
  { for (int j=0; j<this->theBuffer.theBilinearForm.NumCols; j++)
    { std::stringstream tmpStream;
      tmpStream << "textBilinearForm" << timesCalled << "_" << i << "_" << j;
      out << "<textarea rows=\"1\" cols=\"2\" id=\"" << tmpStream.str()
      << "\" \n onChange=\"BilinearForm"
      << timesCalled << "[" << i << "]["
      << j << "]=document.getElementById('" << tmpStream.str() << "').value;\">"
      << this->theBuffer.theBilinearForm.elements[i][j] << "</textarea>\n";
    }
    out << "<br>";
  }
  out << CGI::GetHtmlButton("button"+theCanvasId, theDrawFunctionName+"();", "redraw");
  out << "<br>The picture is drawn using javascript."
  << "<br> Left click + hold+ move the mouse on a special vector = rotates the special vector. "
  << "Special vectors, if any, should be labeled according to the author's ``artistic'' inspiration (usually dark red dots).  "
  << "<br>Moving a vector rotates ``infinitesimally'' the projection plane of your screen "
  << "<br>1) inside the projection plane "
  << "<br>2) in the plane spanned by the selected vector and its orthogonal complement relative to the projection plane. "
  << "<br>The angle change matches the motion of your mouse pointer.  "
  << " Special care must be taken if the selected vector lies "
  << "inside the projection plane or the selected vector is orthogonal to the projection plane. "
  << "If one of these cases happens, the picture might jump around a bit."
  << "<br>The mouse wheel zooms in/out. "
  << " Zooming is tested to work on Firefox and google Chrome browsers on Ubuntu."
  ;
  out << "<br>Currently, " << this->theBuffer.IndexNthDrawOperation.size << " elements are drawn. ";
  if (this->theBuffer.IndexNthDrawOperation.size>500)
    out  << " The visualization is javascript/pc processor <b>intensive</b> so it will <b>not work well</b> "
    <<  " for graphics with lots of elements. This message is displayed only when the number of drawn elements is more than 500.";
  out << "<hr>";

  out << "<script type=\"text/javascript\">\n";

  if (this->theBuffer.ProjectionsEiVectors.size!= theDimension || this->theBuffer.theBilinearForm.NumRows!=theDimension)
  { this->theBuffer.MakeMeAStandardBasis(theDimension);
    //std::cout << "made a standard basis!";
  }
  out << "var BilinearForm" << timesCalled << "= new Array(" << theDimension << ");\n";
  for (int i=0; i<theDimension; i++)
  { out << "BilinearForm" << timesCalled << "[" << i << "]=new Array(" << theDimension << ");\n";
    for (int j=0; j<theDimension; j++)
      out << "BilinearForm" << timesCalled << "[" << i << "][" << j << "]=" << this->theBuffer.theBilinearForm.elements[i][j] << ";\t";
    out << "\n";
  }
  this->theBuffer.ModifyToOrthonormalNoShiftSecond
  (this->theBuffer.BasisProjectionPlane[0][1], this->theBuffer.BasisProjectionPlane[0][0]);
//  (this->theBuffer.BasisProjectionPlane[0][0], this->theBuffer.BasisProjectionPlane[0][1]);
  out
  << "var VectorE1Cone" << timesCalled << "= new Array(" << theDimension << ");\n"
  << "var VectorE2Cone" << timesCalled << "= new Array(" << theDimension << ");\n"
  << "var VectorE1ConeGoal" << timesCalled << "= new Array(" << theDimension << ");\n"
  << "var VectorE2ConeGoal" << timesCalled << "= new Array(" << theDimension << ");\n"
  << "var VectorE1ConeStart" << timesCalled << "= new Array(" << theDimension << ");\n"
  << "var VectorE2ConeStart" << timesCalled << "= new Array(" << theDimension << ");\n";

  for (int i=0; i<theDimension; i++)
  { out << "VectorE2Cone" << timesCalled << "[" << i << "]=" << this->theBuffer.BasisProjectionPlane[0][1][i] << ";\t";
    out << "VectorE1Cone" << timesCalled << "[" << i << "]=" << this->theBuffer.BasisProjectionPlane[0][0][i] << ";\n";
  }
  out << "var frameCount" << timesCalled << "=0;\n";
  out << "var frameCountGoesUp" << timesCalled << "=true;\n";
  int numFramesUserPlane=50;
  out << "function startProjectionPlaneUser" << timesCalled << "(){\n"
  << " frameCount" << timesCalled << "=0;\n";
  for (int i=0; i<theDimension; i++)
    out
    << "  VectorE1ConeStart" << timesCalled << "[" << i << "]=VectorE1Cone" << timesCalled << "[" << i << "];\n"
    << "  VectorE2ConeStart" << timesCalled << "[" << i << "]=VectorE2Cone" << timesCalled << "[" << i << "];\n"
    << "  VectorE1ConeGoal" << timesCalled << "[" << i << "]=document.getElementById(\""
    << textEbasisNamesUserInput[0][i] << "\").value;\n"
    << "  VectorE2ConeGoal" << timesCalled << "[" << i << "]=document.getElementById(\""
    << textEbasisNamesUserInput[1][i] << "\").value;\n";
  out
  << "  changeProjectionPlaneUser" << timesCalled << "();\n"
  << "\n}\n";
  out << "function changeProjectionPlaneUser" << timesCalled << "(){\n"
  << "  frameCount" << timesCalled << "++;\n"
  << "  if (frameCount" << timesCalled << "> " << numFramesUserPlane << ")\n"
  << "    return;\n"
  << "  for (i=0; i<" << theDimension << "; i++)\n"
  << "  { "
  << "VectorE1Cone" << timesCalled << "[i]=VectorE1ConeGoal" << timesCalled << "[i]*"
  << "(frameCount" << timesCalled << "/" << numFramesUserPlane << ")"
  << "+ VectorE1ConeStart" << timesCalled << "[i]*"
  << "(1-frameCount" << timesCalled << "/" << numFramesUserPlane << ");\n"
  << "    VectorE2Cone" << timesCalled << "[i]=VectorE2ConeGoal" << timesCalled << "[i]*"
  << "(frameCount" << timesCalled << "/" << numFramesUserPlane << ")"
  << "+ VectorE2ConeStart" << timesCalled << "[i]*"
  << "(1-frameCount" << timesCalled << "/" << numFramesUserPlane << ");\n"
  << "  }\n"
  << "  MakeVectorE1AndE2orthonormal" << timesCalled << "();\n"
  << "  ComputeProjections" << timesCalled << "();\n"
  << "  " << theDrawFunctionName << "();\n"
  << "  window.setTimeout(\"changeProjectionPlaneUser" << timesCalled << "()\",100);\n"
  << "}";
  if (this->theBuffer.BasisProjectionPlane.size>2)
  { out << "BasisProjectionPlane" << timesCalled << "=new Array(" << this->theBuffer.BasisProjectionPlane.size << ");\n";
    for (int j=0; j<this->theBuffer.BasisProjectionPlane.size; j++)
    { out << "BasisProjectionPlane" << timesCalled << "[" << j << "]=new Array(2);\n";
      for (int k=0; k<2; k++)
      { out << "BasisProjectionPlane" << timesCalled << "[" << j << "][" << k << "]=new Array(" << theDimension << ");\n";
        for (int l=0; l<theDimension; l++)
          out << "BasisProjectionPlane" << timesCalled << "[" << j << "][" << k << "][" << l << "]="
          << this->theBuffer.BasisProjectionPlane[j][k][l] << ";\t";
        out << "\n";
      }
      out << "\n";
    }
    out << "function changeProjectionPlaneOnTimer" << timesCalled << "(){\n"
    << "if(frameCountGoesUp" << timesCalled << ")\n"
    << "  frameCount" << timesCalled << "++;\n"
    << "else\n"
    << "  frameCount" << timesCalled << "--;\n"
    << "if (frameCount" << timesCalled << "==" << this->theBuffer.BasisProjectionPlane.size-1 << " || "
    << "frameCount" << timesCalled << "==0)\n"
    << "{ \n"
    << "  frameCountGoesUp" << timesCalled << "=! frameCountGoesUp" << timesCalled << ";\n"
    << "}\n"
    << "if (frameCount" << timesCalled << ">= " << this->theBuffer.BasisProjectionPlane.size << ")\n"
    << "  return;"
    << "for (i=0; i<" << theDimension << "; i++)\n"
    << "{ VectorE1Cone" << timesCalled << "[i]=BasisProjectionPlane" << timesCalled
    << "[frameCount" << timesCalled << "][0][i];\n"
    << "  VectorE2Cone" << timesCalled << "[i]=BasisProjectionPlane" << timesCalled
    << "[frameCount" << timesCalled << "][1][i];\n"
    << "\n}\n";
    out << theDrawFunctionName << "();\n";
    out << "window.setTimeout(\"changeProjectionPlaneOnTimer" << timesCalled << "()\",100);\n"
    << "}\n";
  }

  out << "var " << projName << "= new Array(" << theDimension << ");\n";
  out << "var " << eiBasis << "= new Array(" << theDimension << ");\n";
  for (int i=0; i<theDimension; i++)
    out << projName << "[" << i << "]= new Array(2);\n";
  out  << "var " << basisCircles << "=new Array(" << this->theBuffer.BasisToDrawCirclesAt.size << ");\n";
  out  << "var " << projBasisCircles << "=new Array(" << this->theBuffer.BasisToDrawCirclesAt.size << ");\n";
  for (int i=0; i<this->theBuffer.BasisToDrawCirclesAt.size; i++)
  { out << basisCircles << "[" << i << "]=[";
    for (int j=0; j<theDimension; j++)
    { out << this->theBuffer.BasisToDrawCirclesAt[i][j];
      if(j!=theDimension-1)
        out << ",";
     }
    out <<  "];\n";
  }
  for (int i=0; i<theDimension; i++)
  { ////////////////////
    out << eiBasis << "[" << i << "]=[";
    for (int j=0; j<theDimension; j++)
    { out << ((i==j)? 1 :0);
      if(j!=theDimension-1)
        out << ",";
     }
    out <<  "];\n";
    //////////////////
    out << projBasisCircles << "[" << i << "]= new Array(2);\n";
  }
  out << "var " << Points1ArrayName << "=new Array(" << this->theFrames.size << ");\n"
  << "var " << Points2ArrayName << "=new Array(" << this->theFrames.size << ");\n"
  << "var " << circArrayName << "=new Array(" << this->theFrames.size << ");\n"
  << "var " << circRadiiArrayName << "=new Array(" << this->theFrames.size << ");\n"
  << "var " << txtArrayName << "=new Array(" << this->theFrames.size << ");\n"
  << "var " << Points1ArrayNameColors << "=new Array(" << this->theFrames.size << ");\n"
  ;
  for (int k=0; k<this->theFrames.size; k++)
  { DrawOperations& currentOps= this->theFrames[k].theBuffer;
    out << Points1ArrayName << "[" << k << "]" << "=new Array(" << currentOps.theDrawLineBetweenTwoRootsOperations.size << ");\n"
    << Points2ArrayName << "[" << k << "]=new Array(" << currentOps.theDrawLineBetweenTwoRootsOperations.size << ");\n"
    << circArrayName         << "[" << k << "]=new Array(" << currentOps.theDrawCircleAtVectorOperations.size << ");\n"
    << circRadiiArrayName << "[" << k << "]=new Array(" << currentOps.theDrawCircleAtVectorOperations.size << ");\n"
    << txtArrayName << "[" << k << "]=new Array(" << currentOps.theDrawTextAtVectorOperations.size << ");\n"
    << Points1ArrayNameColors << "[" << k << "]=new Array(" << currentOps.theDrawLineBetweenTwoRootsOperations.size << ");\n"
    ;
    for (int i=0; i<currentOps.theDrawLineBetweenTwoRootsOperations.size; i++)
    { Vector<double>& current1=currentOps.theDrawLineBetweenTwoRootsOperations[i].v1;
      Vector<double>& current2=currentOps.theDrawLineBetweenTwoRootsOperations[i].v2;
      out << Points1ArrayName << "[" << k << "]" << "[" << i << "]=[";
      for (int j=0; j<theDimension; j++)
      { out << current1[j];
        if (j!=theDimension-1)
          out << ",";
      }
      out << "];\n";
      out << Points1ArrayNameColors << "[" << k << "]" << "[" << i << "]=\""
      << boss->GetColorHtmlFromColorIndex(currentOps.theDrawLineBetweenTwoRootsOperations[i].ColorIndex)
      << "\";";
      out << Points2ArrayName << "[" << k << "]" << "[" << i << "]=[";
      for (int j=0; j<theDimension; j++)
      { out << current2[j];
        if (j!=theDimension-1)
          out << ",";
      }
      out << "];\n";
    }
    for (int i=0; i<currentOps.theDrawCircleAtVectorOperations.size; i++)
    { Vector<double>& current1=currentOps.theDrawCircleAtVectorOperations[i].theVector;
      out << circRadiiArrayName<< "[" << k << "]" << "[" << i << "]=" << currentOps.theDrawCircleAtVectorOperations[i].radius << ";\n";
      out << circArrayName << "[" << k << "]" << "[" << i << "]=[";
      for (int j=0; j<theDimension; j++)
      { out << current1[j];
        if (j!=theDimension-1)
          out << ",";
      }
      out << "];\n";
    }
    for (int i=0; i<currentOps.theDrawTextAtVectorOperations.size; i++)
    { Vector<double>& current1=currentOps.theDrawTextAtVectorOperations [i].theVector;
      out << txtArrayName << "[" << k << "]" << "[" << i << "]=[";
      for (int j=0; j<theDimension; j++)
      { out << current1[j];
        if (j!=theDimension-1)
          out << ",";
      }
      out << "];\n";
    }
  }
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
  out << "resultX+=vector[i]*" << projName << "[i][0];\n";
  out << "resultY+=vector[i]*" << projName << "[i][1];\n}\n";
  out << "result=[resultX, resultY];\n";
  out << "return result;\n";
  out << "}\n";
  out << "var globalFrameCounter=0;\n";
  int frameDelay=3000;
  out
  << "function incrementGlobalFrameCounter" << timesCalled << "(){\n"
  << "globalFrameCounter++;\n"
  << "if (globalFrameCounter>=" << this->theFrames.size << ")\n"
  << "globalFrameCounter=0;\n"
  << "  ComputeProjections" << timesCalled << "();\n"
  << "  " << theDrawFunctionName << "();\n"
  << "window.setTimeout(\"incrementGlobalFrameCounter" << timesCalled << "()\"," << frameDelay << ");\n"
  << "}\n"
  ;
  out << "var " << theSurfaceName << "=0;\n"
  << "function " << theDrawFunctionName << "(){\n"
  << "  if (" << theSurfaceName << "==0)\n"
  << "    " << theInitFunctionName << "();\n"
  << "ComputeProjections" << timesCalled << "();\n"
  << theSurfaceName << ".clear();\n";
  out
  << "for (s=0; s<" << Points1ArrayName << "[globalFrameCounter].length" << "; s++ )\n  "
  << theSurfaceName << ".createLine({"
  << " x1 : " << functionConvertToXYName << "( " << Points1ArrayName << "[globalFrameCounter][s])[0],"
  << " y1 :" << functionConvertToXYName << "( " << Points1ArrayName << "[globalFrameCounter][s])[1],"
  << " x2 :"  << functionConvertToXYName << "( " << Points2ArrayName << "[globalFrameCounter][s])[0],"
  << " y2 :" << functionConvertToXYName << "( " << Points2ArrayName << "[globalFrameCounter][s])[1] }).setStroke({color : "
  << Points1ArrayNameColors <<  "[globalFrameCounter][s]"
  << "});\n";
  out
  << "for (s=0; s<" << circArrayName << "[globalFrameCounter].length" << "; s++ )\n  "
  << theSurfaceName << ".createEllipse({"
  << " cx :" << functionConvertToXYName << "( " << circArrayName <<"[globalFrameCounter]" << "["
  << "s" << "])[0],"
  << " cy :" << functionConvertToXYName << "( " << circArrayName <<"[globalFrameCounter]" << "["
  << "s" << "])[1],"
  << " rx : "  <<  circRadiiArrayName << "[globalFrameCounter][s]" << ","
  << " ry :" << circRadiiArrayName << "[globalFrameCounter][s]"  << " }).setStroke({color : \""
  << "#FF0000"
  << "\"});\n";
  /*  for (int i=0; i<this->theBuffer.IndexNthDrawOperation.size; i++)
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
              << boss->GetColorHtmlFromColorIndex(this->theBuffer.theDrawLineBetweenTwoRootsOperations[currentIndex].ColorIndex)
              << "\"});\n";
        break;
      case DrawOperations::typeDrawCircleAtVector:
        out << theSurfaceName << ".createEllipse({"
              << " cx :" << functionConvertToXYName << "( " << circArrayName << "["
              << currentIndex << "])[0],"
              << " cy :" << functionConvertToXYName << "( " << circArrayName << "["
              << currentIndex << "])[1],"
              << " rx : "  <<  this->theBuffer.theDrawCircleAtVectorOperations[currentIndex].radius << ","
              << " ry :" << this->theBuffer.theDrawCircleAtVectorOperations[currentIndex].radius
              << " }).setStroke({color : \""
              << boss->GetColorHtmlFromColorIndex(this->theBuffer.theDrawCircleAtVectorOperations[currentIndex].ColorIndex)
              << "\"});\n";
        break;
      case DrawOperations::typeDrawTextAtVector:
        out << theSurfaceName << ".createText({"
              << " x :" << functionConvertToXYName << "( " << txtArrayName << "["
              << currentIndex<< "])[0],"
              << " y :" << functionConvertToXYName << "( " << txtArrayName << "["
              << currentIndex << "])[1],"
              << " text : \""  <<  this->theBuffer.theDrawTextAtVectorOperations[currentIndex].theText
              << "\" }).setStroke({color : \""
              << boss->GetColorHtmlFromColorIndex(this->theBuffer.theDrawTextAtVectorOperations[currentIndex].ColorIndex)
              << "\"});\n";
      default: break;
    }
  }*/
  out << "}\n"
  << "function " << theInitFunctionName << "(){\n"
  << "node = dojo.byId(\"" << theCanvasId << "\");\n"
//  << " dojo.require(\"dojox.gfx\", function(){"
  << "if (dojox.gfx!=undefined)\n"
  << "{ dojo.require(\"dojox.gfx\"); "
  << theSurfaceName << "  = dojox.gfx.createSurface(node,"
  << boss->DefaultHtmlWidth << "," << boss->DefaultHtmlHeight << ");}\n"
  << " else return;\n "
//  << "});\n"
  << "  ComputeProjections" << timesCalled << "();\n"
  << theDrawFunctionName << "();\n";
  if (this->theBuffer.BasisProjectionPlane.size>2)
    out << "window.setTimeout(\"changeProjectionPlaneOnTimer" << timesCalled << "()\",100);\n";
  if (this->theFrames.size>1)
    out <<"window.setTimeout(\"incrementGlobalFrameCounter" << timesCalled << "()\"," << frameDelay << ");\n";
  out << " }\n";
  out << "var selectedBasisIndexCone" << timesCalled << "=-1;\n"
  << "var clickTolerance=5;\n"
  << "function ptsWithinClickToleranceCone" << timesCalled << "(x1, y1, x2, y2)\n"
  << "{ if (x1-x2>clickTolerance || x2-x1>clickTolerance || y1-y2>clickTolerance || y2-y1>clickTolerance )\n    return false;\n  return true;\n}";

  out << "function MultiplyVector" << timesCalled << "(output, coeff)"
  << "{ for (var i=0; i<output.length; i++)\n"
  << "  output[i]*=coeff;\n"
  << "}\n"
  << "function AddVectorTimes" << timesCalled << "(output, inputVector, coeff)\n"
  << "{ for (var i=0; i<output.length; i++)\n"
  << "  output[i]+=inputVector[i]*coeff;\n"
  << "}\n"
  << "function ScaleToUnitLength" << timesCalled << "(vector)\n"
  << "{ MultiplyVector" << timesCalled << "(vector, 1/Math.sqrt(getScalarProduct" << timesCalled << "(vector,vector)));\n"
  << "}\n";
  out
  << "function RotateOutOfPlane" << timesCalled << "(input, orthoBasis1, orthoBasis2, oldTanSquared, newTanSquared, newX, newY, oldX, oldY)"
  << "{ var projection= dojo.clone(orthoBasis1);\n"
  << "  var vComponent= dojo.clone(input);\n"
  << "  var scal1= getScalarProduct" << timesCalled << "(orthoBasis1, input);\n"
  << "  var scal2= getScalarProduct" << timesCalled << "(orthoBasis2, input);\n"
  << "  MultiplyVector" << timesCalled << "(projection, scal1);\n"
  << "  AddVectorTimes" << timesCalled << "(projection, orthoBasis2, scal2);\n"
  << "  AddVectorTimes" << timesCalled << "(vComponent, projection, -1);\n"
  << "  var oldAngle=Math.atan(Math.sqrt(oldTanSquared));\n"
  << "  var newAngle=Math.atan(Math.sqrt(newTanSquared));\n"
  << "  if (isNaN(oldAngle) || isNaN(newAngle))\n"
  << "    return input;\n"
  << "  var angleChange=-oldAngle+newAngle;\n"
  << "  if (newX*oldX<0 && newY*oldY<0)\n"
//  <<  " {
//  << "    angleChange=oldAngle+newAngle;\n"
  << "    angleChange*=-1;\n"
//  << "  };"
//  << "  topBound=Math.PI/2;\n"
//  << "  bottomBound=-Math.PI/2;\n"
//  << "  while (angleChange>=topBound || angleChange<bottomBound)\n"
//  << "    if(angleChange>=topBound)\n"
//  << "      angleChange-=Math.PI;\n"
//  << "    else"
//  << "      angleChange+=Math.PI;\n"
  << "  projection=dojo.clone(orthoBasis1);\n"
  << "  MultiplyVector" << timesCalled << "(projection, Math.cos(angleChange)*scal1-Math.sin(angleChange)*scal2);\n"
  << "  AddVectorTimes" << timesCalled << "(projection, orthoBasis2, Math.sin(angleChange)*scal1+Math.sin(angleChange)*scal2);\n"
  << "  var result= vComponent;\n"
  << "  AddVectorTimes" << timesCalled << "(result, projection, 1);\n"
  << "  return result;\n"
  << "}\n";
  out << "function MakeVectorE1AndE2orthonormal" << timesCalled << "(){\n"
  << "  AddVectorTimes" << timesCalled << "(VectorE2Cone" << timesCalled
  << ", VectorE1Cone" << timesCalled << ", -getScalarProduct"
  << timesCalled << "(VectorE1Cone" << timesCalled << ", VectorE2Cone" << timesCalled
  << ")/getScalarProduct" << timesCalled
  << "(VectorE1Cone" << timesCalled << ", VectorE1Cone" << timesCalled << "));\n"
  << "  ScaleToUnitLength" << timesCalled << "(VectorE1Cone" << timesCalled << ");\n"
  << "  ScaleToUnitLength" << timesCalled << "(VectorE2Cone" << timesCalled << ");\n"
  << "}\n";
  out << "function ComputeProjections" << timesCalled << "()\n"
  << "{ for (var i=0; i<" << theDimension << "; i++)\n"
  << "  { " << projName << "[i][0]=GraphicsUnitCone" << timesCalled << "*getScalarProduct" << timesCalled
  << "(VectorE1Cone" << timesCalled << ","  << eiBasis << "[i]);\n"
  << "    " << projName << "[i][1]=-GraphicsUnitCone" << timesCalled << "*getScalarProduct" << timesCalled
  << "(VectorE2Cone" << timesCalled << ", " << eiBasis << "[i]);\n"
  << "  }\n";
  for (int j=0; j<theDimension; j++)
    out << "  document.getElementById(\"" << textEbasisNamesReadOnly[0][j] << "\").innerHTML="
    << "VectorE1Cone" << timesCalled << "[" << j << "];\n"
    << "  document.getElementById(\"" << textEbasisNamesReadOnly[1][j] << "\").innerHTML="
    << "VectorE2Cone" << timesCalled << "[" << j << "];\n";
  out
  << "  for (var i=0; i<" << this->theBuffer.BasisToDrawCirclesAt.size << "; i++)\n"
  << "  { " << projBasisCircles << "[i][0]=GraphicsUnitCone" << timesCalled << "*getScalarProduct" << timesCalled
  << "(VectorE1Cone" << timesCalled << ", " << basisCircles << "[i]);\n"
  << "    " << projBasisCircles << "[i][1]=-GraphicsUnitCone" << timesCalled << "*getScalarProduct" << timesCalled
  << "(VectorE2Cone" << timesCalled << ", " << basisCircles << "[i]);\n"
  << "  }\n"
  << "}\n";
  out << "\nfunction getScalarProduct" << timesCalled << "(root1, root2)\n"
  << "{ var result=0;\n"
  << "  for (var i=0; i<" << theDimension << "; i++)\n"
  << "    for (var j=0; j<" << theDimension << "; j++)\n"
  << "      result+= root1[i]*root2[j]*BilinearForm" << timesCalled << "[i][j];\n"
  << "  return result;\n"
  << "}\n";
  out
  << "function getAngleFromXandY" << timesCalled << "(x, y, neighborX, neighborY)\n"
  << "{ var result;\n"
  << "  if (x!=0)\n"
  << "   result= Math.atan(y/x);\n"
  << "  else\n"
  << "    if (y>0)\n"
  << "      result= Math.PI/2;\n"
  << "    else\n"
  << "      result= -Math.PI/2;\n"
  << "  return result;\n"
  << "}\n";
  out
  << "function getAngleChange" << timesCalled << "(newX, newY, oldX, oldY)\n"
  << "{ var result=getAngleFromXandY" << timesCalled
  << "(newX, newY, oldX, oldY)-getAngleFromXandY" << timesCalled << "(oldX, oldY, newX, newY);\n"
  << "  topBound=Math.PI/2;\n"
  << "  bottomBound=-Math.PI/2;\n"
  << "  while (result>topBound || result< bottomBound)\n"
  << "    if (result>topBound)\n"
  << "      result-=Math.PI;\n"
  << "    else\n"
  << "      result+=Math.PI;\n"
  << "  return result;\n"
  << "}\n";
  out << "\nfunction changeBasis" << timesCalled <<  "(selectedIndex, newX, newY)\n"
  << "{ if (newX==0 && newY==0)\n"
  << "    return;\n"
  << "  var selectedRoot=" << basisCircles << "[selectedIndex];\n"
  << "  var selectedRootLength=getScalarProduct" << timesCalled << "(selectedRoot,selectedRoot);\n"
  << "  var projectionSelected=" << projBasisCircles << "[selectedIndex];\n"
  << "  var oldX=projectionSelected[0]/GraphicsUnitCone" << timesCalled << ";\n"
  << "  var oldY=-projectionSelected[1]/GraphicsUnitCone" << timesCalled << ";\n"
  << "  newX/=GraphicsUnitCone" << timesCalled << ";\n"
  << "  newY/=GraphicsUnitCone" << timesCalled << ";\n"
  << "  var epsilon=0.000000015;\n"
  << "  if (newX*newX+newY*newY>0.003)\n"
  << "  { var AngleChange= -getAngleChange" << timesCalled << "(newX, newY, oldX, oldY);\n"
  << "    var NewVectorE1=dojo.clone(VectorE1Cone" << timesCalled << ");\n"
  << "    var NewVectorE2= dojo.clone(VectorE2Cone" << timesCalled << ");\n"
  << "    MultiplyVector" << timesCalled << "(NewVectorE1, Math.cos(AngleChange));\n"
  << "    AddVectorTimes" << timesCalled << "(NewVectorE1, VectorE2Cone" << timesCalled << ", Math.sin(AngleChange));\n"
  << "    MultiplyVector" << timesCalled << "(NewVectorE2, Math.cos(AngleChange));\n"
  << "    AddVectorTimes" << timesCalled << "(NewVectorE2, VectorE1Cone" << timesCalled << ", -Math.sin(AngleChange));\n"
  << "    VectorE1Cone" << timesCalled << "=NewVectorE1;\n"
  << "    VectorE2Cone" << timesCalled << "=NewVectorE2;\n"
  << "  }\n"
  << "  if (newX*newX+newY*newY>0.0001)\n"
  << "  { var RootTimesE1=getScalarProduct" << timesCalled << "(selectedRoot, VectorE1Cone" << timesCalled << ");\n"
  << "    var RootTimesE2=getScalarProduct" << timesCalled << "(selectedRoot, VectorE2Cone" << timesCalled << ");\n"
  << "    var vOrthogonal=dojo.clone(selectedRoot);\n"
  << "    var vProjection=dojo.clone(VectorE1Cone" << timesCalled << ");\n"
  << "    MultiplyVector" << timesCalled << "(vProjection, RootTimesE1);\n"
  << "    AddVectorTimes" << timesCalled << "(vProjection, VectorE2Cone" << timesCalled << ", RootTimesE2" << ");\n"
  << "    AddVectorTimes" << timesCalled << "(vOrthogonal, vProjection, -1);\n"
  << "    var oldRatioProjectionOverHeightSquared = (oldX*oldX+oldY*oldY)/ (selectedRootLength-oldX*oldX-oldY*oldY);\n"
  << "    var newRatioProjectionOverHeightSquared = (newX*newX+newY*newY)/ (selectedRootLength-newX*newX-newY*newY);\n";
  if (theDimension>2)
  { out
    << "    if (getScalarProduct" << timesCalled << "(vOrthogonal, vOrthogonal)<epsilon && getScalarProduct" << timesCalled
    << "(vOrthogonal, vOrthogonal)>-epsilon)\n"
    << "    { vOrthogonal=dojo.clone(" << eiBasis <<  "[2]);\n"
    << "    }\n";
  }
  out
  << "    if (getScalarProduct" << timesCalled << "(vOrthogonal, vOrthogonal)>epsilon || getScalarProduct" << timesCalled
  << "(vOrthogonal, vOrthogonal)<-epsilon)\n"
  << "    { if (oldRatioProjectionOverHeightSquared==0)\n"
  << "      { vProjection=dojo.clone(VectorE1Cone" << timesCalled << ");\n"
  << "        MultiplyVector" << timesCalled << "(vProjection, -newX);\n"
  << "        AddVectorTimes" << timesCalled << "(vProjection, VectorE2Cone" << timesCalled << ", newY" << ");\n"
  << "      }\n"
  << "      ScaleToUnitLength" << timesCalled << "(vProjection);\n"
  << "      ScaleToUnitLength" << timesCalled << "(vOrthogonal);\n"
  << "      VectorE1Cone" << timesCalled << "=RotateOutOfPlane" << timesCalled << "(VectorE1Cone" << timesCalled
  << ", vProjection, vOrthogonal, oldRatioProjectionOverHeightSquared, newRatioProjectionOverHeightSquared, newX, newY, oldX, oldY);\n"
  << "      VectorE2Cone" << timesCalled << "=RotateOutOfPlane" << timesCalled << "(VectorE2Cone" << timesCalled
  << ", vProjection, vOrthogonal, oldRatioProjectionOverHeightSquared, newRatioProjectionOverHeightSquared, newX, newY, oldX, oldY);\n"
  << "    }\n"
  << "    MakeVectorE1AndE2orthonormal" << timesCalled << "();\n"
  << "    ComputeProjections" << timesCalled << "();\n"
  << "  }\n"
  << "}\n";

  out << "\nfunction clickCanvasCone" << timesCalled << "(cx,cy)\n"
  << "{ divPosX=0;\n  divPosY=0;\n  thePointer= document.getElementById(\"idCanvasCone" << timesCalled << "\");\n"
  << "  while(thePointer)  {\n  divPosX += thePointer.offsetLeft;\n  divPosY += thePointer.offsetTop;\n  thePointer = thePointer.offsetParent;\n  }"
  << "\n  posx=(cx-divPosX+document.body.scrollLeft-" << shiftX << ");"
  << "\n  posy=(cy-divPosY+document.body.scrollTop-" << shiftY << ");\n  selectedBasisIndexCone" << timesCalled <<"=-1;\n"
  << "if (ptsWithinClickToleranceCone" << timesCalled << "(posx,posy,0,0))" << "\nselectedBasisIndexCone" << timesCalled << "=-2;\n"
  <<  "for (i=0; i<" << theDimension << ";i++)  {\n if (ptsWithinClickToleranceCone" << timesCalled
  << "(posx, posy, " << projBasisCircles << "[i][0]" << ", " << projBasisCircles
  << "[i][1]" << "))\n"
  << "  selectedBasisIndexCone" << timesCalled << "=i;  \n}\n}\nfunction mouseMoveRedrawCone" << timesCalled << "(cx, cy)\n"
  << "{ if (selectedBasisIndexCone" << timesCalled << "==-1)\n    return;\n  divPosX=0;\n  divPosY=0;\n"
  << "  thePointer= document.getElementById(\"idCanvasCone"<< timesCalled << "\");\n  while(thePointer)\n  { divPosX += thePointer.offsetLeft;\n"
  << "    divPosY += thePointer.offsetTop;\n    thePointer = thePointer.offsetParent;\n  }\n"
  << "  posx=(cx-divPosX+document.body.scrollLeft-" << shiftX << ");\n"
  << "  posy=-(cy-divPosY+document.body.scrollTop-" << shiftY << ");\n"
  << "if (selectedBasisIndexCone" << timesCalled << "==-2)\n{ shiftXCone" << timesCalled << "=(cx-divPosX+document.body.scrollLeft);\n"
  << shiftY << "=(cy-divPosY+document.body.scrollTop);\n  }  else\n"
  << "{ changeBasis" << timesCalled << "(selectedBasisIndexCone" << timesCalled << ", posx, posy);\n  }\n  "
  << theDrawFunctionName << " ();\n}\n";
  out << "\n tempDiv=document.getElementById(\"" << theCanvasId << "\");";
  out << "\ntempDiv.addEventListener (\"DOMMouseScroll\", mouseHandleWheelCone"
  << timesCalled << ", true);\n";
  out << "function mouseHandleWheelCone" << timesCalled << "(theEvent){\n"
  << "theEvent = theEvent ? theEvent : window.event;\n theEvent.preventDefault();\ntheEvent.stopPropagation();\ntheWheelDelta = theEvent.detail ? theEvent.detail * -1 : theEvent.wheelDelta / 40;\n"
  << "GraphicsUnitCone" << timesCalled << "+=theWheelDelta;\n"
  << "if (GraphicsUnitCone" << timesCalled << "==0)\n GraphicsUnitCone" << timesCalled << "=3;\n"
//  << "for (i=0; i<" << theDimension << "; i++){\n "
//  << basisName << "[i][0]*=GraphicsUnitCone" << timesCalled << "/(GraphicsUnitCone" << timesCalled << "-theWheelDelta);\n"
//  << basisName << "[i][1]*=GraphicsUnitCone" << timesCalled << "/(GraphicsUnitCone" << timesCalled << "-theWheelDelta);\n"
//  << "}\n"
  << theDrawFunctionName << "();\n}\n";
out
  << "dojo.require(\"dojox.gfx\");\n"
  << "dojo.addOnLoad(" << theInitFunctionName << ");\n"
  << "dojo.addOnLoad(" << theDrawFunctionName << ");\n"
  << "function dojoOnLoadDoesntWork" << timesCalled << " (){\n"
  << "  if (" << theSurfaceName << "==0){\n  "
  << theInitFunctionName << "();\n"
  << "  window.setTimeout(function(){dojoOnLoadDoesntWork" << timesCalled << "();}, 1000);\n"
  << "  }\n"
  << "}"
  << "dojoOnLoadDoesntWork" << timesCalled << "();\n"
  << "</script>\n"
  ;
  return out.str();
}

template<class coefficient>
void ModuleSSalgebra<coefficient>::SplitFDpartOverFKLeviRedSubalg
  (HomomorphismSemisimpleLieAlgebra& theHmm, Selection& LeviInSmall, GlobalVariables& theGlobalVariables,
   List<ElementUniversalEnveloping<coefficient> >* outputEigenVectors,
   Vectors<coefficient>* outputWeightsFundCoords, Vectors<coefficient>* outputEigenSpace,
   std::stringstream* comments, const coefficient& theRingUnit, const coefficient& theRingZero)
{ MacroRegisterFunctionWithName("ModuleSSalgebra<coefficient>::SplitFDpartOverFKLeviRedSubalg");
  if (this->theChaR.size()!=1)
  { if (comments!=0)
    { std::stringstream out;
      out << "I have been instructed only to split modules that are irreducible over the ambient Lie algebra";
      out << " Instead I got the character " << this->theChaR.ToString() << " (" << this->theChaR.size() << " monomials)";
      *comments << out.str();
    }
    return;
  }
  std::string tempS;
//  ReflectionSubgroupWeylGroup subWeylInLarge, subWeylStandalone;
//  charSSAlgMod<coefficient> charWRTsubalgebra;
//  Selection LeviInSmall;
//  this->theChaR.SplitCharOverRedSubalg
//  (&tempS, charWRTsubalgebra, this->parabolicSelectionNonSelectedAreElementsLevi, theHmm.ImagesCartanDomain,
//   *theHmm.owners, theHmm.indexDomain, LeviInSmall, subWeylInLarge, subWeylStandalone, theGlobalVariables);
  Vector<Rational> theHWsimpleCoords, theHWfundCoords;
  std::stringstream out;
  if(comments!=0)
    out << tempS;
  out << "<br>Parabolic selection: " << LeviInSmall.ToString();
  std::stringstream tempStream1;
  tempStream1 << "Started splitting the f.d. part of the " << theHmm.theRange().GetLieAlgebraName() << "-module with highest weight in fund coords "
  << this->theChaR[0].weightFundamentalCoordS.ToString();
  ProgressReport theReport(&theGlobalVariables);
  theReport.Report(tempStream1.str());
//  std::cout << "<br>Parabolic selection: " << LeviInSmall.ToString();
  List<List<Vector<coefficient> > > eigenSpacesPerSimpleGenerator;
  Selection InvertedLeviInSmall;
  InvertedLeviInSmall=LeviInSmall;
  InvertedLeviInSmall.InvertSelection();
  eigenSpacesPerSimpleGenerator.SetSize(InvertedLeviInSmall.CardinalitySelection);
  Vectors<coefficient> tempSpace1, tempSpace2;
  MemorySaving<Vectors<coefficient> > tempEigenVects;
  Vectors<coefficient>& theFinalEigenSpace= (outputEigenSpace==0) ? tempEigenVects.GetElement() : *outputEigenSpace;
  theFinalEigenSpace.SetSize(0);
//  WeylGroup& theWeyL=this->theAlgebra.theWeyl;
  if (InvertedLeviInSmall.CardinalitySelection==0)
    theFinalEigenSpace.MakeEiBasis(this->GetDim());
  for (int i=0; i<InvertedLeviInSmall.CardinalitySelection; i++)
  { ElementSemisimpleLieAlgebra<Rational>& currentElt=
    theHmm.imagesSimpleChevalleyGenerators[InvertedLeviInSmall.elements[i]];
    //std::cout << "<br>current element is: " << currentElt.ToString();
    MatrixTensor<coefficient> currentOp, tempMat;
    currentOp.MakeZero();
    for (int j=0; j<currentElt.size(); j++)
    { //std::cout << "<br>fetching action of generator of index " << currentElt[j].theGeneratorIndex;
      tempMat=this->GetActionGeneratorIndeX
      (currentElt[j].theGeneratorIndex, theGlobalVariables, theRingUnit, theRingZero);
      tempMat*=currentElt.theCoeffs[j];
      currentOp+=tempMat;
    }
      std::stringstream tempStream3;
      double timeAtStart1=theGlobalVariables.GetElapsedSeconds();
      tempStream3 << "Computing eigenspace corresponding to " << currentElt.ToString() << "...";
      theReport.Report(tempStream3.str());
    Matrix<coefficient> currentOpMat;
    currentOp.GetMatrix(currentOpMat, this->GetDim());
    currentOpMat.GetZeroEigenSpace(eigenSpacesPerSimpleGenerator[i]);
      tempStream3 << " done in " << theGlobalVariables.GetElapsedSeconds()-timeAtStart1
      << " seconds.";
      theReport.Report(tempStream3.str());
    if (i==0)
      theFinalEigenSpace=(eigenSpacesPerSimpleGenerator[i]);
    else
    {   std::stringstream tempStream4;
        double timeAtStart2=theGlobalVariables.GetElapsedSeconds();
        tempStream4 << "Intersecting with eigenspace corresponding to " << currentElt.ToString() << "...";
      tempSpace1=theFinalEigenSpace;
        theReport.Report(tempStream4.str());
      tempSpace2=eigenSpacesPerSimpleGenerator[i];
      theFinalEigenSpace.IntersectTwoLinSpaces
      (tempSpace1, tempSpace2, theFinalEigenSpace, &theGlobalVariables);
        tempStream4 << " done in " << theGlobalVariables.GetElapsedSeconds()-timeAtStart2 << " seconds.";
        theReport.Report(tempStream4.str());
    }
  }
  out << "<br>Eigenvectors:<table> ";
//  Vector<Rational> zeroRoot;
//  zeroRoot.MakeZero(this->theAlgebra->GetRank());
  std::stringstream readyForLatexComsumption;
  readyForLatexComsumption << "\\begin{tabular}{|lll|}\n <br>";

  readyForLatexComsumption << "\\hline\\multicolumn{3}{|c|}{Highest weight $"
  << this->theHWFundamentalCoordsBaseField.ToStringLetterFormat("\\omega") << "$}\\\\\n<br>";
  readyForLatexComsumption << "weight fund. coord.& singular vector \\\\\\hline\n<br>";
  Vector<coefficient> currentWeight;
  Vector<coefficient> hwFundCoordsNilPart;
  hwFundCoordsNilPart=this->theHWFundamentalCoordsBaseField;
  hwFundCoordsNilPart-=this->theHWFDpartFundamentalCoordS;
  ElementUniversalEnveloping<coefficient> currentElt, tempElt;
  if (outputEigenVectors!=0)
    outputEigenVectors->SetSize(0);
  for (int j=0; j<theFinalEigenSpace.size; j++)
  { out << "<tr><td>";
    currentElt.MakeZero(this->GetOwner());
    Vector<coefficient>& currentVect= theFinalEigenSpace[j];
    int lastNonZeroIndex=-1;
    for (int i=0; i<currentVect.size; i++)
      if (!(currentVect[i].IsEqualToZero()))
      { tempElt.MakeZero(this->GetOwner());
        tempElt.AddMonomial(this->theGeneratingWordsNonReduced[i], 1);
        tempElt*=currentVect[i];
        currentElt+=tempElt;
        lastNonZeroIndex=i;
      }
    currentWeight=theHmm.theRange().theWeyl.GetFundamentalCoordinatesFromSimple
    (this->theGeneratingWordsWeightsPlusWeightFDpart[lastNonZeroIndex]);//<-implicit type conversion here
    currentWeight+=hwFundCoordsNilPart;
    readyForLatexComsumption <<  "$" << currentWeight.ToStringLetterFormat("\\omega")
    << "$&$" << currentVect.ToStringLetterFormat("m") << "$";
    if (currentElt.size()>1)
      out << "(";
    if (outputEigenVectors!=0)
      outputEigenVectors->AddOnTop(currentElt);
    if (outputWeightsFundCoords!=0)
      outputWeightsFundCoords->AddOnTop(currentWeight);
    out << currentElt.ToString(&theGlobalVariables.theDefaultFormat);
    if (currentElt.size()>1)
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
//  if (outputEigenSpace!=0)
//  { std::cout << "<br> outputEigenSpace->size=" << outputEigenSpace->size << "; outputEigenVectors->size=" << outputEigenVectors->size;
//  }
  theReport.Report("SplitFDpartOverFKLeviRedSubalg done!");
  if (comments!=0)
    *comments << out.str();
}

bool CommandList::fSplitFDpartB3overG2CharsOnly
(CommandList& theCommands, const Expression& input, Expression& output
 )
{ branchingData theG2B3Data;
  return theCommands.fSplitFDpartB3overG2CharsOutput
  (theCommands, input, output, theG2B3Data);
}

bool CommandList::innerSplitFDpartB3overG2Init
(CommandList& theCommands, const Expression& input, Expression& output,
 branchingData& theG2B3Data, Expression& outputContext)
{ MacroRegisterFunctionWithName("CommandList::innerSplitFDpartB3overG2Init");
  if (!input.IsListNElements(4))
    return output.SetError
    ("Splitting the f.d. part of a B_3-representation over G_2 requires 3 arguments",
     theCommands);
  std::cout << input.ToString();
  if (!theCommands.GetVectorFromFunctionArguments<RationalFunctionOld>
      (input, theG2B3Data.theWeightFundCoords, &outputContext, 3,
       theCommands.innerRationalFunction))
    output.SetError
    ("Failed to extract highest weight in fundamental coordinates. ", theCommands);
  theCommands.MakeHmmG2InB3(theG2B3Data.theHmm);
  theG2B3Data.selInducing.init(3);
  for (int i=0; i<theG2B3Data.theWeightFundCoords.size; i++)
    if (!theG2B3Data.theWeightFundCoords[i].IsSmallInteger())
      theG2B3Data.selInducing.AddSelectionAppendNewIndex(i);
  theG2B3Data.initAssumingParSelAndHmmInittedPart1NoSubgroups(*theCommands.theGlobalVariableS);
  return true;
}

bool CommandList::fSplitFDpartB3overG2CharsOutput
(CommandList& theCommands, const Expression& input, Expression& output,
 branchingData& theG2B3Data
 )
{ MacroRegisterFunctionWithName("fSplitFDpartB3overG2CharsOutput");
  Expression theContext(theCommands);
  theCommands.innerSplitFDpartB3overG2Init
  (theCommands, input, output, theG2B3Data, theContext);
  if (output.IsError())
    return true;
  std::stringstream out;
  out << "<br>Highest weight: " << theG2B3Data.theWeightFundCoords.ToString()
  << "<br>Parabolic selection: " << theG2B3Data.selInducing.ToString();
  std::string report;
  Vectors<RationalFunctionOld> outputWeightsFundCoordS;
  Vectors<RationalFunctionOld> outputWeightsSimpleCoords;
  Vectors<RationalFunctionOld> leviEigenSpace;
  Vector<RationalFunctionOld> ih1, ih2;
  ReflectionSubgroupWeylGroup subGroupLarge, subGroupSmall;
  charSSAlgMod<RationalFunctionOld> tempChar;
  charSSAlgMod<RationalFunctionOld> startingChar;
  startingChar.MakeFromWeight
  (theG2B3Data.theHmm.theRange().theWeyl.GetSimpleCoordinatesFromFundamental
   (theG2B3Data.theWeightFundCoords), &theG2B3Data.theHmm.theRange());
  startingChar.SplitCharOverRedSubalg
  (&report, tempChar, theG2B3Data, *theCommands.theGlobalVariableS);
  out << report;
  return output.AssignValue(out.str(), theCommands);
}

void CommandList::MakeHmmG2InB3(HomomorphismSemisimpleLieAlgebra& output)
{ SemisimpleLieAlgebra tempb3alg, tempg2alg;
  tempb3alg.theWeyl.MakeArbitrarySimple('B',3);
  tempg2alg.theWeyl.MakeArbitrarySimple('G',2);
  output.domainAlg=&this->theObjectContainer.theLieAlgebras
  [this->theObjectContainer.theLieAlgebras.AddNoRepetitionOrReturnIndexFirst(tempg2alg)];
  output.rangeAlg =
  &this->theObjectContainer.theLieAlgebras
  [this->theObjectContainer.theLieAlgebras.AddNoRepetitionOrReturnIndexFirst(tempb3alg)];

  output.theRange().ComputeChevalleyConstantS(this->theGlobalVariableS);
  output.theDomain().ComputeChevalleyConstantS(this->theGlobalVariableS);
  ElementSemisimpleLieAlgebra<Rational> g_2, g_1plusg_3, g_m2, g_m1plusg_m3, tempElt;
  g_2.MakeGenerator         (13, output.theRange());
  g_m2.MakeGenerator        (7,  output.theRange());
  g_1plusg_3.MakeGenerator  (12, output.theRange());
  tempElt.MakeGenerator     (14, output.theRange());
  g_1plusg_3+=tempElt;
  g_m1plusg_m3.MakeGenerator(6, output.theRange());
  tempElt.MakeGenerator     (8, output.theRange());
  g_m1plusg_m3+=tempElt;
//  std::cout << "<hr>g_2: " << g_2.ToString();
//  std::cout << "<hr>g_{1}+g_{3}: " << g_1plusg_3.ToString();
//  std::cout << "<hr>g_{-2}: " << g_m2.ToString();
//  std::cout << "<hr>g_{-1}+g_{-3}: " << g_m1plusg_m3.ToString();
  output.imagesSimpleChevalleyGenerators.SetSize(4);
  output.imagesSimpleChevalleyGenerators[0]=g_1plusg_3;
  output.imagesSimpleChevalleyGenerators[1]=g_2;
  output.imagesSimpleChevalleyGenerators[3]=g_m2;
  output.imagesSimpleChevalleyGenerators[2]=g_m1plusg_m3;
  output.ComputeHomomorphismFromImagesSimpleChevalleyGenerators(*this->theGlobalVariableS);
  output.GetRestrictionAmbientRootSystemToTheSmallerCartanSA(output.RestrictedRootSystem, *this->theGlobalVariableS);
}

template<class coefficient>
bool Polynomial<coefficient>::FindOneVarRatRoots(List<Rational>& output)
{ MacroRegisterFunctionWithName("Polynomial_CoefficientType::FindOneVarRatRoots");
  if (this->GetMinNumVars()>1)
    return false;
  output.SetSize(0);
  if (this->GetMinNumVars()==0 ||this->IsEqualToZero())
    return true;
  Polynomial<coefficient> myCopy;
  myCopy=*this;
  myCopy.ScaleToIntegralMinHeightOverTheRationalsReturnsWhatIWasMultipliedBy();
  Rational lowestTerm, highestTerm;
  this->GetConstantTerm(lowestTerm);
  if (lowestTerm==0)
  { Polynomial<Rational> x1, tempP;
    x1.MakeMonomiaL(0, 1, 1);
    myCopy.DivideBy(x1, myCopy, tempP);
    List<Rational> tempList;
    bool result=myCopy.FindOneVarRatRoots(tempList);
    output.AddOnTop(0);
    output.AddListOnTop(tempList);
    return result;
  }
  if (this->IsAConstant())
    return true;
  int indexHighest= this->GetIndexMaxMonomialLexicographicLastVariableStrongest();
  highestTerm=this->theCoeffs[indexHighest];
  if (!highestTerm.IsSmallInteger() || !lowestTerm.IsSmallInteger())
    return false;
  Vector<Rational> tempV;
  Rational val;
  tempV.SetSize(1);
  List<int> divisorsH, divisorsS;
  LargeInt hT, lT;
  highestTerm.GetNumerator(hT);
  lowestTerm.GetNumerator(lT);
  if (! hT.GetDivisors(divisorsH, false) || !  lT.GetDivisors(divisorsS, true))
    return false;
  for (int i=0; i<divisorsH.size; i++)
    for (int j=0; j<divisorsS.size; j++)
    { tempV[0].AssignNumeratorAndDenominator(divisorsS[j],divisorsH[i]);
      val=myCopy.Evaluate(tempV);
//      std::cout << "<br>" << myCopy.ToString() << " eval at "
//      << tempV.ToString() << " equals " << val.ToString();
      if (val==0)
      { Polynomial<Rational> divisor, tempP;
        divisor.MakeDegreeOne(1, 0, 1, -tempV[0]);
        myCopy.DivideBy(divisor, myCopy, tempP);
        output.AddOnTop(tempV[0]);
        List<Rational> tempList;
        bool result=myCopy.FindOneVarRatRoots(tempList);
        output.AddListOnTop(tempList);
        return result;
      }
    }
  return true;
}

bool CommandList::fPrintB3G2branchingIntermediate
(CommandList& theCommands, const Expression& input, Expression& output,
 Vectors<RationalFunctionOld>& theHWs, branchingData& theG2B3Data, Expression& theContext)
{ MacroRegisterFunctionWithName("CommandList::fPrintB3G2branchingIntermediate");
  std::stringstream out, timeReport;
  std::stringstream latexTable, latexTable2;
  bool isFD=(theG2B3Data.selInducing.CardinalitySelection==0);
  if (isFD)
  { out << "<table border=\"1\"><tr><td>$so(7)$-highest weight</td><td>Decomposition over $G_2$</td>"
    <<  "<td>$G_2\\cap b$-eigenvectors </td></tr>";
    latexTable << "\\begin{longtable}{|ccccl|} \\caption{\\label{tableB3fdsOverG2charsAndHWV} "
    << " Decompositions of finite dimensional $so(7)$-modules over $G_2$}\\\\"
    << "\\hline so(7)& dim. &$G_2$&dim.& $\\mathfrak b \\cap G_2$-singular vectors\\\\ \\hline"
    << "\\endhead \n<br>";
  } else
  { out << "Let " << CGI::GetHtmlMathSpanPure("p\\subset so(7)") << " be the "
    << theG2B3Data.selInducing.ToString() << "-parabolic subalgebra"
    << " and let " << CGI::GetHtmlMathSpanPure("{p}':= p\\cap G_2")
    << ". Then  " << CGI::GetHtmlMathSpanPure("{p}'") << " is the "
    << theG2B3Data.selSmallParSel.ToString() << "- parabolic subalgebra of G_2"
    << "<br> <table border=\"1\"><tr><td>$so(7)$-highest weight</td>"
    << "<td>character difference from top</td>"
    << "<td>Decomposition of inducing module over "
    << CGI::GetHtmlMathSpanPure("p'")
    << "</td><td>" << CGI::GetHtmlMathSpanPure("p'\\cap b")
    << "-eigenvectors</td><td>Casimir projector</td><td>Extra multiplier</td><td>corresponding "
    << CGI::GetHtmlMathSpanPure("G_2\\cap b")
    << "-eigenvectors</td><td>Shapovalov square</td></tr>";
    latexTable << "\\begin{longtable}{|cccclll|} \\caption{\\label{tableB3fdsOverG2charsAndHWV"
    << theG2B3Data.selInducing.ToString() << "} "
    << "Decomposition of inducing $" << theG2B3Data.selInducing.ToString() << "$-$\\mathfrak p$-module over $"
    << theG2B3Data.selSmallParSel.ToString() << "$-$\\mathfrak p':=\\mathfrak p\\cap G_2$}\\\\"
    << "\\hline $\\mathfrak p-inducing$& dim. &$\\mathfrak p' decomp. $&dim.&"
    << " $\\mathfrak b \\cap \\mathfrak p'$-singular vectors & Casimir projector "
    << "& Corresp. $\\mathfrak b \\cap G_2$-singular vectors  \\\\ \\hline"
    << "\\endhead \n<br>";
  }
//  std::cout << theContext.ToString();
  theContext.ContextGetFormatExpressions(theG2B3Data.theFormat);
  theG2B3Data.theFormat.flagUseLatex=true;
  theG2B3Data.theFormat.NumAmpersandsPerNewLineForLaTeX=0;
  Expression tempExpression;
  RationalFunctionOld rfZero, rfOne;
  rfZero.MakeZero(theCommands.theGlobalVariableS);
  rfOne.MakeOne(theCommands.theGlobalVariableS);
  latexTable2 << "\\begin{longtable}{|rll|}\\caption"
  << "{Values of $x_1$ for each $v_{\\lambda,i}$}\\label{tableCriticalValuesvlambda}"
  << "\\endhead";
  for (int i=0; i<theHWs.size; i++)
  { theG2B3Data.theWeightFundCoords=theHWs[i];
    theCommands.fSplitFDpartB3overG2inner
    (theCommands, theG2B3Data, tempExpression);
    timeReport << tempExpression.GetValuE<std::string>();
    RationalFunctionOld numEigenVectors;
    numEigenVectors=rfZero;
    for (int j=0; j<theG2B3Data.theSmallCharFDpart.size(); j++)
      numEigenVectors+=theG2B3Data.theSmallCharFDpart.theCoeffs[j];
    theG2B3Data.theFormat.CustomPlusSign="";
    int eigenIndexcounter=0;
    if (i!=0)
      latexTable2 << "\\hline\\multicolumn{3}{|c|}{$\\lambda="
      << theG2B3Data.theWeightFundCoords.ToStringLetterFormat("\\omega", &theG2B3Data.theFormat)
      << "$}\\\\vector& coefficient of $v_\\lambda$ in $Sh_{\\lambda,i}$ &$x_1\\notin$ \\\\\\hline";
    for (int k=0; k<theG2B3Data.theSmallCharFDpart.size(); k++ )
    { charSSAlgMod<RationalFunctionOld> tempChar;
      tempChar.AddMonomial
      (theG2B3Data.theSmallCharFDpart[k], theG2B3Data.theSmallCharFDpart.theCoeffs[k]);
      int multiplicity=0;
      theG2B3Data.theSmallCharFDpart.theCoeffs[k].IsSmallInteger(&multiplicity);
      for (int counter=0; counter<multiplicity; counter++, eigenIndexcounter++)
      { out << "<tr>";
        if (k==0 && counter==0)
        { theG2B3Data.theFormat.CustomPlusSign="\\oplus ";
          theG2B3Data.theFormat.fundamentalWeightLetter= "\\omega";
          out << "<td rowspan=\"" << numEigenVectors.ToString() << "\">"
          << theG2B3Data.theAmbientChar.ToString(&theG2B3Data.theFormat) << "</td> ";
          if (!isFD)
          { out << "<td rowspan=\"" << numEigenVectors.ToString() << "\">";
            for (int charcounter1=0; charcounter1<theG2B3Data.theCharacterDifferences.size; charcounter1++)
            { std::string tempS=theG2B3Data.theFormat.CustomPlusSign;
              theG2B3Data.theFormat.CustomPlusSign="";
              out << "A_{" << charcounter1 << "}:="
              << theG2B3Data.theCharacterDifferences[charcounter1].ToString(&theG2B3Data.theFormat)
              << ";";
              if (charcounter1!=theG2B3Data.theCharacterDifferences.size-1)
                out << "<br>";
              theG2B3Data.theFormat.CustomPlusSign=tempS;
            }
            out << "</td> ";
          }
          latexTable << "\\multirow{" << theG2B3Data.theEigenVectorS.size  << "}{*}{$"
          << theG2B3Data.theAmbientChar.ToString(&theG2B3Data.theFormat) << "$}";
          latexTable << "& \\multirow{" << theG2B3Data.theEigenVectorS.size  << "}{*}{$"
          << theG2B3Data.WeylFD.WeylDimFormulaSimpleCoords
          (theG2B3Data.WeylFD.AmbientWeyl.GetSimpleCoordinatesFromFundamental
           (theG2B3Data.theAmbientChar[0].weightFundamentalCoordS)).ToString(&theG2B3Data.theFormat)
          << "$}";
        } else
          latexTable << "&";
        latexTable << " & ";
        if (counter==0)
        { theG2B3Data.theFormat.fundamentalWeightLetter= "\\psi";
          theG2B3Data.theFormat.CustomPlusSign="\\oplus ";
          out << "<td rowspan=\"" << multiplicity << " \">"
          << tempChar.ToString(&theG2B3Data.theFormat) << "</td>";
          latexTable << "\\multirow{" << multiplicity  << "}{*}{$";
//          if (k!=0)
//            latexTable << "\\oplus ";
          latexTable << tempChar.ToString(&theG2B3Data.theFormat) << "$}";
          latexTable << "&\\multirow{" << multiplicity  << "}{*}{$";
          if (multiplicity>1)
            latexTable << multiplicity << "\\times";
          latexTable << theG2B3Data.WeylFDSmall.WeylDimFormulaSimpleCoords
          (theG2B3Data.WeylFDSmall.AmbientWeyl.GetSimpleCoordinatesFromFundamental
           (tempChar[0].weightFundamentalCoordS), rfOne).ToString(&theG2B3Data.theFormat)
          << "$}";
        } else
          latexTable << "&";
        latexTable << "&";
        theG2B3Data.theFormat.CustomPlusSign="";
        out << "<td>"
        << CGI::GetHtmlMathSpanNoButtonAddBeginArrayL
        (theG2B3Data.theEigenVectorsLevi[eigenIndexcounter].ToString(&theG2B3Data.theFormat))
        << "</td>";
        theG2B3Data.theFormat.MaxLineLength=20;
        latexTable << "$\\begin{array}{l}"
        << theG2B3Data.theEigenVectorsLevi[eigenIndexcounter].ToString(&theG2B3Data.theFormat)
        << "\\end{array}$ \n";
        if (!isFD)
        { std::string tempS1=theG2B3Data.GetStringCasimirProjector(eigenIndexcounter, 12);
          std::string tempS2=
          "("+ theG2B3Data.theUEelts[eigenIndexcounter].ToString(&theG2B3Data.theFormat)
          + ")\\cdot v_\\lambda";
          out << "<td>" << CGI::GetHtmlMathSpanNoButtonAddBeginArrayL(tempS1) << "</td>";
          out << "<td>" << theG2B3Data.additionalMultipliers[eigenIndexcounter].ToString() << "</td>";
          out << "<td>" << CGI::GetHtmlMathSpanNoButtonAddBeginArrayL(tempS2) << "</td>";
          out << "<td>"
          << theG2B3Data.theShapovalovProducts[eigenIndexcounter].ToString(&theG2B3Data.theFormat);
          out << "</td>";
          int theIndex;
          numEigenVectors.IsSmallInteger(&theIndex);
          if (theIndex- eigenIndexcounter -1>0)
          { List<Rational> tempList, tempList2;
            latexTable2 << " $v_{\\lambda," <<  theIndex- eigenIndexcounter -1 << "} $&";
            Polynomial<Rational> tempP;
            theG2B3Data.theShapovalovProducts[eigenIndexcounter].GetNumerator(tempP);
//            std::cout << "<br>before scaling: " << tempP.ToString();
            tempP.ScaleToIntegralMinHeightOverTheRationalsReturnsWhatIWasMultipliedBy();
//            std::cout << "<br>after scaling: " << tempP.ToString();
            latexTable2 << "$\\begin{array}{l}"
            << tempP.ToString(&theG2B3Data.theFormat)
            << "\\end{array}$ & ";
            if (tempP.FindOneVarRatRoots(tempList))
            { tempList2.AddOnTopNoRepetition(tempList);
              out << "<td>Rational roots: " << tempList.ToString() << "</td>";
              latexTable2 << tempList2.ToString();
            }
            latexTable2 << "\\\\";
          }
          latexTable << "& $\\begin{array}{l}" << tempS1 << "\\end{array}$" << "&"
          << "$\\begin{array}{l}" << tempS2 << "\\end{array}$";
        }
        latexTable << "\\\\ \n <br>\n";
        if (counter!=multiplicity-1)
        { if (isFD)
            latexTable << "\\cline{5-5}";
          else
            latexTable << "\\cline{5-7}";
        }
        out << "</tr>";
      }
      if (k!=theG2B3Data.theSmallCharFDpart.size()-1)
      { if (isFD)
          latexTable << "\\cline{3-5}";
        else
          latexTable << "\\cline{3-7}";
      }
    }
//    if (i!=theHWs.size-1)
//      latexTable << "\\arrayrulecolor{gray} ";
    latexTable << "\\hline";
//    if (i!=theHWs.size-1)
//      latexTable << "\\arrayrulecolor{black} ";
  }
  latexTable << "\\hline";
  out << "</table>";
  latexTable << "\\end{longtable}";
  latexTable2 << "\\end{longtable}";
  out << "<br>" << timeReport.str() << "<br><br><br><b>Ready for LaTeX consumption:</b><br  >";
  out << latexTable2.str();
  out <<"<br><br><br>";
  out << latexTable.str();
  out <<"<br>";
  return output.AssignValue(out.str(), theCommands);
}

bool CommandList::fPrintB3G2branchingTableInit
(CommandList& theCommands, const Expression& input, Expression& output,
 branchingData& theG2B3data, int& desiredHeight, Expression& outputContext)
{ MacroRegisterFunctionWithName("CommandList::fPrintB3G2branchingTableInit");
  if (input.children.size!=3)
    return output.SetError
    ("I need two arguments: first is height, second is parabolic selection. ", theCommands);
  desiredHeight=0;
  if (!input[1].IsSmallInteger(&desiredHeight))
    return output.SetError("the first argument must be a small integer", theCommands);
  if (desiredHeight<0)
    desiredHeight=0;
  const Expression& weightNode= input[2];
  theCommands.innerSplitFDpartB3overG2Init
  (theCommands, weightNode, output, theG2B3data, outputContext);
  if (output.IsError())
    return true;
  return false;
}

bool CommandList::fPrintB3G2branchingTable
(CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::fPrintB3G2branchingTable");
  Vectors<RationalFunctionOld> theHWs;
  branchingData theG2B3Data;
  Expression theContext(theCommands);
  if (!theCommands.fPrintB3G2branchingTableCommon
      (theCommands, input, output, theHWs, theG2B3Data, theContext))
    return false;
  if (output.IsError())
    return true;
//  std::cout << " <br>the highest weights: " << theHWs.ToString();
  return theCommands.fPrintB3G2branchingIntermediate
  (theCommands, input, output, theHWs, theG2B3Data, theContext)
  ;
}

bool CommandList::fPrintB3G2branchingTableCommon
(CommandList& theCommands, const Expression& input, Expression& output,
  Vectors<RationalFunctionOld>& outputHWs, branchingData& theG2B3Data, Expression& theContext
  )
{ MacroRegisterFunctionWithName("CommandList::fPrintB3G2branchingTableCommon");
  std::stringstream out, timeReport;
  Vector<Rational> theHW;
  Vector<RationalFunctionOld> theHWrf;
  SelectionWithMaxMultiplicity theHWenumerator;
  int desiredHeight=0;
  if (! theCommands.fPrintB3G2branchingTableInit
    (theCommands, input, output, theG2B3Data, desiredHeight, theContext))
    return false;
  if (output.IsError())
    return true;
  Selection invertedSelInducing=theG2B3Data.selInducing;
  theContext.ContextGetFormatExpressions(theG2B3Data.theFormat);
  invertedSelInducing.InvertSelection();
  outputHWs.SetSize(0);
  for (int j=0; j<=desiredHeight; j++)
  { theHWenumerator.initMaxMultiplicity(3-theG2B3Data.selInducing.CardinalitySelection, j);
    theHWenumerator.IncrementSubsetFixedCardinality(j);
    int numCycles=theHWenumerator.NumCombinationsOfCardinality(j);
    for (int i=0; i<numCycles; i++, theHWenumerator.IncrementSubsetFixedCardinality(j))
    { theHWrf=theG2B3Data.theWeightFundCoords;
      for (int k=0; k<invertedSelInducing.CardinalitySelection; k++)
        theHWrf[invertedSelInducing.elements[k]]+=theHWenumerator.Multiplicities[k];
      outputHWs.AddOnTop(theHWrf);
    }
  }
//  std::cout << " <br>the highest weights: " << outputHWs.ToString();
  return true;
}

bool CommandList::fPrintB3G2branchingTableCharsOnly
(CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::fPrintB3G2branchingTableCharsOnly");
  branchingData theg2b3data;
  Expression theContext(theCommands);
  Vectors<RationalFunctionOld> theHWs;
  theCommands.fPrintB3G2branchingTableCommon
  (theCommands, input, output, theHWs, theg2b3data, theContext);
  if (output.IsError())
    return true;
  charSSAlgMod<RationalFunctionOld> theCharacter, outputChar;
  Vector<Rational> theHW;
  std::stringstream out;
  std::stringstream latexTable;
  bool isFD=(theg2b3data.selInducing.CardinalitySelection==0);
  if (isFD)
  { out << "<table><tr><td>$so(7)$-highest weight</td><td>Dimension</td><td>Decomposition over $G_2$</td><td>Dimensions</td>"
    << "</tr>";
    latexTable << "\\begin{longtable}{|rl|} \\caption{\\label{tableB3fdsOverG2charsonly} "
    << "Decompositions of finite dimensional $so(7)$-modules over $G_2$}\\\\"
    << "\\hline$so(7)$-module & ~~~~~~ decomposition over $G_2$\\endhead \\hline\n<br>";
  } else
  { out << "Let " << CGI::GetHtmlMathSpanPure("p\\subset so(7)") << " be the "
    << theg2b3data.selInducing.ToString() << "-parabolic subalgebra"
    << " and let " << CGI::GetHtmlMathSpanPure("{p}':= p\\cap G_2")
    << ". Then  " << CGI::GetHtmlMathSpanPure("{p}'") << " is the "
    << theg2b3data.selSmallParSel.ToString() << "- parabolic subalgebra of G_2"
    << "<br> <table><tr><td>$so(7)$-highest weight</td>"
    << "<td>Dimension of inducing fin. dim. "
    << CGI::GetHtmlMathSpanPure(" p")
    << "-module</td><td>Decomposition of inducing module over "
    << CGI::GetHtmlMathSpanPure("p'")
    << "</td><td>Dimensions</td>"
    << " <td>Highest weight <br> is sufficiently generic <br> if none of <br>the following vanish</td>"
    << "</tr>";
    latexTable << "\\begin{longtable}{|p{2cm}l|} \\caption{\\label{tableB3fdsOverG2charsonly"
    << theg2b3data.selInducing.ToString() << "} "
    << "Decompositions of inducing $\\mathfrak{p}" << "_{"
    << theg2b3data.selInducing.ToString()
    << "}" << "$-modules over $\\bar{ \\mathfrak {l}}$"
    << ", where $\\mathfrak{l}$ is the reductive Levi part of "
    << "$\\mathfrak{p}" << "_{" << theg2b3data.selInducing.ToString()
    <<  "}$ and $\\bar{\\mathfrak {l}}$ is the reductive Levi part of $\\bar{\\mathfrak {p}} _{"
    << theg2b3data.selSmallParSel.ToString()  << "}$" << " } \\\\"
    << "\\hline $V_\\lambda(\\mathfrak l)$ " << "& Decomposition over $\\bar { \\mathfrak l}$ "
    << "\\endhead \\hline\n<br>";
  }
  theg2b3data.theFormat.flagUseLatex=true;
  ElementUniversalEnveloping<RationalFunctionOld> theCasimir, theCentralCharacter, resultChar;
  HashedList<ElementUniversalEnveloping<RationalFunctionOld> > theCentralChars;

  theCasimir.MakeCasimir(theg2b3data.theHmm.theDomain(), *theCommands.theGlobalVariableS);
  WeylGroup& smallWeyl=theg2b3data.theHmm.theDomain().theWeyl;
//  WeylGroup& largeWeyl=theg2b3data.theHmm.theRange().theWeyl;
  for (int k=0; k<theHWs.size; k++)
  { theCharacter.MakeFromWeight
    (theg2b3data.theHmm.theRange().theWeyl.GetSimpleCoordinatesFromFundamental(theHWs[k]),
     &theg2b3data.theHmm.theRange());
    theCharacter.SplitCharOverRedSubalg(0, outputChar, theg2b3data, *theCommands.theGlobalVariableS);
    theg2b3data.theFormat.fundamentalWeightLetter= "\\omega";
    out << "<tr><td> " << theCharacter.ToString(&theg2b3data.theFormat) << "</td> ";
    out << "<td>"
    << theg2b3data.WeylFD.WeylDimFormulaSimpleCoords
    (theg2b3data.WeylFD.AmbientWeyl.GetSimpleCoordinatesFromFundamental
    (theCharacter[0].weightFundamentalCoordS)).ToString() << "</td>";
    latexTable << " $ " << theCharacter.ToString(&theg2b3data.theFormat) << " $ ";
    theg2b3data.theFormat.fundamentalWeightLetter= "\\psi";
    out << "<td>" << outputChar.ToString(&theg2b3data.theFormat) << "</td>";
    out << "<td>";
    theg2b3data.theFormat.CustomPlusSign="\\oplus ";
    Vector<RationalFunctionOld> leftWeightSimple, leftWeightDual, rightWeightSimple, rightWeightDual;
    theCentralChars.Clear();
    for (int i=0; i<outputChar.size(); i++)
    { if (!outputChar.theCoeffs[i].IsEqualToOne())
        out << outputChar.theCoeffs[i].ToString() << " x ";
      out << theg2b3data.WeylFDSmall.WeylDimFormulaSimpleCoords
      (theg2b3data.WeylFDSmall.AmbientWeyl.GetSimpleCoordinatesFromFundamental
      (outputChar[i].weightFundamentalCoordS));
      if (i!=outputChar.size()-1)
        out << "+";
      leftWeightSimple=smallWeyl.GetSimpleCoordinatesFromFundamental(outputChar[i].weightFundamentalCoordS);
      leftWeightDual=smallWeyl.GetDualCoordinatesFromFundamental(outputChar[i].weightFundamentalCoordS);
      for (int j=0; j<outputChar.size(); j++)
      { rightWeightSimple=smallWeyl.GetSimpleCoordinatesFromFundamental(outputChar[j].weightFundamentalCoordS);
        rightWeightDual=smallWeyl.GetDualCoordinatesFromFundamental(outputChar[j].weightFundamentalCoordS);
//        if (i!=j)
        if ((rightWeightSimple-leftWeightSimple).IsPositive())
        { resultChar=theCasimir;
          theCentralCharacter=theCasimir;
          resultChar.ModOutVermaRelations(theCommands.theGlobalVariableS, &rightWeightDual);
          theCentralCharacter.ModOutVermaRelations(theCommands.theGlobalVariableS, &leftWeightDual);
          resultChar-=theCentralCharacter;
          resultChar.ScaleToIntegralMinHeightOverTheRationalsReturnsWhatIWasMultipliedBy();
          resultChar*=-1;
          theCentralChars.AddOnTopNoRepetition(resultChar);
        }
      }
    }
    out << "</td>";
    theg2b3data.theFormat.MaxLineLength=80;
    latexTable << "& $\\begin{array}{l} " << outputChar.ToString(&theg2b3data.theFormat) << "\\end{array} $ ";
    theg2b3data.theFormat.CustomPlusSign="";
    if (!isFD && theCentralChars.size>0)
    { out << "<td>";
      latexTable << " \\\\\n<br> ";
      latexTable << "\\multicolumn{2}{|p{11.6cm}|}{ \\tiny Strong Condition B: ";
      for (int l=0; l<theCentralChars.size; l++)
      { out << theCentralChars[l].ToString(&theg2b3data.theFormat) << "<br> ";
        latexTable << "$" << theCentralChars[l].ToString(&theg2b3data.theFormat) << "\\neq 0$";
        if (l!=theCentralChars.size-1)
          latexTable << ", ";
      }
      latexTable << "\\normalsize}";
      out << " </td>";
    }
    out << "</tr>";
    latexTable << "\\\\\n <br>\n";
    latexTable << "\\hline";
  }
  out << "</table>";
  latexTable << "\\end{longtable}";
  out << "<br><b>Ready for LaTeX consumption:</b><br>%preamble: "
  << "<br>\\documentclass{article}<br>\\usepackage{longtable, amssymb}"
  << "<br>\\begin{document}"
  << "<br>%text body<br>"
  << latexTable.str()
  << "<br>%end of text body <br>\\end{document}";
  return output.AssignValue(out.str(), theCommands);
}

bool CommandList::fSplitFDpartB3overG2
(CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::fSplitFDpartB3overG2");
  branchingData theG2B3Data;
  Expression theContext(theCommands);
  theCommands.innerSplitFDpartB3overG2Init
  (theCommands, input, output, theG2B3Data, theContext);
  if (output.IsError())
    return true;
  std::stringstream out;
  Vectors<RationalFunctionOld> theHWs;
  theHWs.AddOnTop(theG2B3Data.theWeightFundCoords);
  return theCommands.fPrintB3G2branchingIntermediate
  (theCommands, input, output, theHWs, theG2B3Data, theContext);
}

bool CommandList::fSplitFDpartB3overG2old
(CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::fSplitFDpartB3overG2old");
  branchingData theG2B3Data;
  theCommands.fSplitFDpartB3overG2CharsOutput
  (theCommands, input, output, theG2B3Data);
  if (output.IsError())
    return true;
  std::stringstream out;
  theCommands.fSplitFDpartB3overG2inner(theCommands, theG2B3Data, output);
  out << "<br>Highest weight: " << theG2B3Data.theWeightFundCoords.ToString() << "<br>Parabolic selection: "
  << theG2B3Data.selInducing.ToString() << "<br>common Levi part of G_2 and B_3: "
  << theG2B3Data.selSmallParSel.ToString();
  out
  << "<table border=\"1\"><tr><td>word</td><td>B_3-weight simple coords</td><td>B_3-weight fund. coords </td>"
  << "<td>G_2 simple coordinates</td><td>G2-fund. coords</td><td>G2-dual coordinates</td><td>character</td></tr>";
    std::stringstream readyForLatexConsumptionTable1, readyForLatexConsumptionTable2;

  readyForLatexConsumptionTable1 << "\\hline\\multicolumn{3}{|c|}{Highest weight $ "
  <<  theG2B3Data.theWeightFundCoords.ToStringLetterFormat("\\omega")
  << "$}\\\\ weight fund. coord.& singular vector& weight proj. $\\bar h^*$ \\\\\\hline\n<br> ";
  for (int i=0; i< theG2B3Data.outputWeightsSimpleCoords.size; i++)
  { Vector<RationalFunctionOld>& currentWeightSimpleB3coords=theG2B3Data.outputWeightsSimpleCoords[i];
    Vector<RationalFunctionOld>& currentWeightFundB3coords=theG2B3Data.outputWeightsFundCoordS[i];
    Vector<RationalFunctionOld>& currentG2Weight=theG2B3Data.g2Weights[i];
    Vector<RationalFunctionOld>& currentG2DualWeight=theG2B3Data.g2DualWeights[i];
    readyForLatexConsumptionTable1 << "$" << currentWeightFundB3coords.ToStringLetterFormat("\\omega")
    << " $ & $" << theG2B3Data.leviEigenSpace[i].ToStringLetterFormat("m")
    << " $ & $ " << currentG2Weight.ToStringLetterFormat("\\alpha") << " $ \\\\\n<br>";
    out << "<tr><td>" << theG2B3Data.outputEigenWords[i].ToString() << "</td><td> "
    << currentWeightSimpleB3coords.ToString() << "</td><td> " << currentWeightFundB3coords.ToString()
    << "</td><td>" << currentG2Weight.ToStringLetterFormat("\\alpha") << "</td><td> "
    << theG2B3Data.theHmm.theDomain().theWeyl.GetFundamentalCoordinatesFromSimple(currentG2Weight).ToString()
    << "</td><td> " << currentG2DualWeight.ToString() << "</td>";
    out << "<td>" << CGI::GetHtmlMathSpanNoButtonAddBeginArrayL(theG2B3Data.theChars[i].ToString()) << "</td>";
    out << "</tr>";
  }
  readyForLatexConsumptionTable1 <<"\\hline \n";
  out << "</table>";
  out << "<br>Ready for LaTeX consumption: ";
  out << "<br><br>" << readyForLatexConsumptionTable1.str() << "<br><br>";
  out << "<table border=\"1\"><tr><td>weight</td><td>the elt closed form</td><td>the elt</td></tr>";
  Vector<RationalFunctionOld> weightDifference, weightDifferenceDualCoords;
  std::stringstream formulaStream1;
  for (int k=0; k<theG2B3Data.g2Weights.size; k++)
  { out << "<tr><td>" << theG2B3Data.g2Weights[k].ToString() << "</td><td>";
    for (int j=0; j<theG2B3Data.g2Weights.size; j++)
    { weightDifference=theG2B3Data.g2Weights[j] - theG2B3Data.g2Weights[k];
      if (weightDifference.IsPositive())
        formulaStream1 << "(12(i(\\bar c) - " << theG2B3Data.theChars[j].ToString() <<  "))";
    }
    formulaStream1 << "v_\\lambda";
    out << CGI::GetHtmlMathSpanNoButtonAddBeginArrayL(formulaStream1.str()) << "</td><td>"
    << CGI::GetHtmlMathSpanNoButtonAddBeginArrayL(theG2B3Data.theEigenVectorS[k].ToString())
    << "</td></tr>";
  }
  out << "</table>";
  out << "<br>Time final: " << theCommands.theGlobalVariableS->GetElapsedSeconds();
  return output.AssignValue(out.str(), theCommands);
}

void branchingData::resetOutputData()
{ this->theEigenVectorS.SetSize(0);
  this->theEigenVectorsLevi.SetSize(0);
  this->outputEigenWords.SetSize(0);
  this->g2Weights.SetSize(0);
  this->outputWeightsFundCoordS.SetSize(0);
  this->theCharacterDifferences.Clear();
}

template<class coefficient>
bool ElementSumGeneralizedVermas<coefficient>::
ExtractElementUE(ElementUniversalEnveloping<coefficient>& output, SemisimpleLieAlgebra& theOwner)
{ output.MakeZero(theOwner);
  ModuleSSalgebra<coefficient>* theModPtr=0;
  MonomialUniversalEnveloping<coefficient> tempMon;
  for (int i=0; i<this->size(); i++)
  { const MonomialGeneralizedVerma<coefficient>& currentMon=(*this)[i];
    if (i==0)
      theModPtr=currentMon.owneR;
    if (currentMon.owneR!=theModPtr)
      return false;
    tempMon=currentMon.theMonCoeffOne;
    tempMon*=currentMon.GetOwner().theGeneratingWordsNonReduced[currentMon.indexFDVector];
    output.AddMonomial(tempMon, this->theCoeffs[i]);
  }
  return true;
}

bool CommandList::fSplitFDpartB3overG2inner
(CommandList& theCommands, branchingData& theG2B3Data, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::fSplitFDpartB3overG2inner");
//  std::stringstream out;
//  std::cout << "Highest weight: " << theWeightFundCoords.ToString() << "; Parabolic selection: " << selInducing.ToString();
  ModuleSSalgebra<RationalFunctionOld> theModCopy;
  theModCopy.MakeFromHW
  (theG2B3Data.theHmm.theRange(), theG2B3Data.theWeightFundCoords, theG2B3Data.selInducing,
   *theCommands.theGlobalVariableS, 1, 0, 0, false);
  std::string report;
  theG2B3Data.resetOutputData();
  theG2B3Data.initAssumingParSelAndHmmInitted(*theCommands.theGlobalVariableS);
  theG2B3Data.SelSplittingParSel=theG2B3Data.selInducing;
  if (theG2B3Data.SelSplittingParSel.selected[0]!=theG2B3Data.SelSplittingParSel.selected[2])
  { theG2B3Data.SelSplittingParSel.AddSelectionAppendNewIndex(0);
    theG2B3Data.SelSplittingParSel.AddSelectionAppendNewIndex(2);
  }
  Vector<Rational> splittingParSel;
  splittingParSel=theG2B3Data.SelSplittingParSel;

  theCommands.theObjectContainer.theCategoryOmodules.AddNoRepetitionOrReturnIndexFirst(theModCopy);
  int theModIndex=theCommands.theObjectContainer.theCategoryOmodules.GetIndex(theModCopy);
  ModuleSSalgebra<RationalFunctionOld>& theMod=
  theCommands.theObjectContainer.theCategoryOmodules[theModIndex];
  std::stringstream out;
  out << "<br>Time elapsed before making B3 irrep: "
  << theCommands.theGlobalVariableS->GetElapsedSeconds();
  double timeAtStart=theCommands.theGlobalVariableS->GetElapsedSeconds();
  theMod.SplitFDpartOverFKLeviRedSubalg
  (theG2B3Data.theHmm, theG2B3Data.selSmallParSel, *theCommands.theGlobalVariableS, &theG2B3Data.outputEigenWords,
   &theG2B3Data.outputWeightsFundCoordS, &theG2B3Data.leviEigenSpace, 0);
   out << "<br>Time needed to make B3 irrep: " << theCommands.theGlobalVariableS->GetElapsedSeconds()-timeAtStart;
  //out << report;
//  int numVars=theWeightFundCoords[0].NumVars;
  theG2B3Data.g2Weights.SetSize(theG2B3Data.outputWeightsFundCoordS.size);
  //std::cout << "theG2B3Data.outputWeightsFundCoordS: " << theG2B3Data.outputWeightsFundCoordS.ToString() << "<br>";
  theG2B3Data.g2DualWeights.SetSize(theG2B3Data.outputWeightsFundCoordS.size);
  Matrix<Rational> invertedG2cartanMat;
  invertedG2cartanMat=theG2B3Data.theHmm.theDomain().theWeyl.CartanSymmetric;
  invertedG2cartanMat.Invert();
  theG2B3Data.outputWeightsSimpleCoords=
  theG2B3Data.theHmm.theRange().theWeyl.GetSimpleCoordinatesFromFundamental
  (theG2B3Data.outputWeightsFundCoordS);
  theG2B3Data.theAmbientChar.MakeFromWeight
  (theG2B3Data.theHmm.theRange().theWeyl.GetSimpleCoordinatesFromFundamental
   (theG2B3Data.theWeightFundCoords), &theG2B3Data.theHmm.theRange());
  theG2B3Data.theSmallCharFDpart.MakeZero();
  charSSAlgMod<RationalFunctionOld> tempMon;
  for (int i=0; i< theG2B3Data.outputWeightsSimpleCoords.size; i++)
  { Vector<RationalFunctionOld>& currentWeight=theG2B3Data.outputWeightsSimpleCoords[i];
    Vector<RationalFunctionOld>& currentG2Weight=theG2B3Data.g2Weights[i];
    Vector<RationalFunctionOld>& currentG2DualWeight=theG2B3Data.g2DualWeights[i];
    currentG2DualWeight.SetSize(2);
    currentG2DualWeight[0]=theG2B3Data.theHmm.theRange().theWeyl.RootScalarCartanRoot
    (currentWeight, theG2B3Data.theHmm.ImagesCartanDomain[0]);//<-note: implicit type conversion: the return type is the left coefficient type.
    currentG2DualWeight[1]=theG2B3Data.theHmm.theRange().theWeyl.RootScalarCartanRoot
    (currentWeight, theG2B3Data.theHmm.ImagesCartanDomain[1]);//<-note: implicit type conversion: the return type is the left coefficient type.
    invertedG2cartanMat.ActOnVectorColumn(currentG2DualWeight, currentG2Weight);//<-g2weight is now computed;
    tempMon.MakeFromWeight(currentG2Weight, &theG2B3Data.theHmm.theDomain());
    theG2B3Data.theSmallCharFDpart+=tempMon;
  }
  ElementUniversalEnveloping<RationalFunctionOld> theG2Casimir, theG2CasimirCopy, imageCasimirInB3, bufferCasimirImage, tempElt;
  theG2Casimir.MakeCasimir(theG2B3Data.theHmm.theDomain(), *theCommands.theGlobalVariableS, 1, 0);
  Vector<RationalFunctionOld> highestWeightG2dualCoords=*theG2B3Data.g2DualWeights.LastObject();

  theG2B3Data.theChars.SetSize(theG2B3Data.outputWeightsFundCoordS.size);
//  RationalFunctionOld& baseChar=*theChars.LastObject();
//  baseChar.MakeZero(numVars, theCommands.theGlobalVariableS);
//  theG2Casimir.ModOutVermaRelations(theCommands.theGlobalVariableS, &highestWeightG2dualCoords, 1, 0);
//  if (!theG2Casimir.IsEqualToZero())
//    baseChar=theG2Casimir.theCoeffs[0];
//  out << "<br>Base G_2-character: " << baseChar.ToString() << " corresponding to g2-dual weight " << highestWeightG2dualCoords.ToString();
  theCommands.theGlobalVariableS->MaxComputationTimeSecondsNonPositiveMeansNoLimit=400;
  for (int i=0; i< theG2B3Data.outputWeightsSimpleCoords.size; i++)
  { Vector<RationalFunctionOld>& currentG2DualWeight=theG2B3Data.g2DualWeights[i];
    theG2CasimirCopy=theG2Casimir;
    theG2CasimirCopy.ModOutVermaRelations(theCommands.theGlobalVariableS, &currentG2DualWeight, 1,0);
    if (theG2CasimirCopy.IsEqualToZero())
      theG2B3Data.theChars[i]=0;
    else
      theG2B3Data.theChars[i]=theG2CasimirCopy.theCoeffs[0];
  }
//  theCommands.theGlobalVariableS->MaxComputationTimeSecondsNonPositiveMeansNoLimit=1000;
  theG2B3Data.theEigenVectorsLevi.SetSize(theG2B3Data.g2Weights.size);
  theG2B3Data.theEigenVectorS.SetSize(theG2B3Data.g2Weights.size);
  theG2B3Data.additionalMultipliers.SetSize(theG2B3Data.g2Weights.size);
  theG2B3Data.theShapovalovProducts.SetSize(theG2B3Data.g2Weights.size);
  theG2B3Data.theUEelts.SetSize(theG2B3Data.g2Weights.size);
  ElementSumGeneralizedVermas<RationalFunctionOld>& theHWV=
  *theG2B3Data.theEigenVectorsLevi.LastObject();
  theHWV.MakeHWV(theMod, 1);
  theHWV*=-1;
  *theG2B3Data.theEigenVectorS.LastObject()=theHWV;
  Vector<RationalFunctionOld> weightDifference, weightDifferenceDualCoords;
  theG2B3Data.theHmm.ApplyHomomorphism(theG2Casimir, imageCasimirInB3, *theCommands.theGlobalVariableS);
  theG2Casimir.checkConsistency();
  imageCasimirInB3.checkConsistency();
  RationalFunctionOld charDiff;
  for (int k=0; k<theG2B3Data.g2Weights.size; k++)
  { ElementSumGeneralizedVermas<RationalFunctionOld>& currentTensorEltLevi=theG2B3Data.theEigenVectorsLevi[k];
    ElementSumGeneralizedVermas<RationalFunctionOld>& currentTensorEltEigen=theG2B3Data.theEigenVectorS[k];
    ElementUniversalEnveloping<RationalFunctionOld>& currentUEelt=theG2B3Data.theUEelts[k];
    currentTensorEltLevi=theHWV;
    //std::cout << "<br>multiplying " << currentTensorElt.ToString() << " by " << theG2B3Data.outputEigenWords[k].ToString();
    currentTensorEltLevi.MultiplyMeByUEEltOnTheLeft(theG2B3Data.outputEigenWords[k], *theCommands.theGlobalVariableS, 1, 0);
    currentTensorEltEigen=currentTensorEltLevi;
    //std::cout << "<br> to obtain " << currentTensorElt.ToString();
    if (theG2B3Data.selInducing.CardinalitySelection>0)
      for (int j=0; j<theG2B3Data.g2Weights.size; j++)
      { weightDifference=theG2B3Data.g2Weights[j] - theG2B3Data.g2Weights[k];
        if (weightDifference.IsPositive())
        { theG2CasimirCopy=imageCasimirInB3;
          tempElt.MakeConst(theG2B3Data.theChars[j], theG2B3Data.theHmm.theRange());
          theG2CasimirCopy-=tempElt;
          theG2CasimirCopy*=12;
          //std::cout << "<hr>Multiplying " << theG2CasimirCopy.ToString() << " and " << currentTensorElt.ToString();
          currentTensorEltEigen.MultiplyMeByUEEltOnTheLeft
          (theG2CasimirCopy, *theCommands.theGlobalVariableS, 1, 0);
          charDiff=theG2B3Data.theChars[j];
          charDiff-=*theG2B3Data.theChars.LastObject();
          theG2B3Data.theCharacterDifferences.AddOnTopNoRepetition(charDiff);
          //std::cout << "<br>To obtain " << currentTensorElt.ToString() << "<hr>";
        }
      }
    theG2B3Data.additionalMultipliers[k]= currentTensorEltEigen.ScaleToIntegralMinHeightOverTheRationalsReturnsWhatIWasMultipliedBy();
    currentTensorEltEigen.ExtractElementUE(currentUEelt, *theMod.owneR);
    currentUEelt.HWTAAbilinearForm
    (currentUEelt, theG2B3Data.theShapovalovProducts[k], &theMod.theHWDualCoordsBaseFielD,
     *theCommands.theGlobalVariableS,1,0,0);
  }
  return output.AssignValue(out.str(), theCommands);
}

template <class coefficient>
bool charSSAlgMod<coefficient>::SplitCharOverRedSubalg
(std::string* Report, charSSAlgMod& output, branchingData& inputData, GlobalVariables& theGlobalVariables)
{ if (this->IsEqualToZero())
    return false;
  this->CheckNonZeroOwner();
  WeylGroup& theWeyL=this->GetOwner()->theWeyl;
  std::stringstream out;
  std::string tempS;
  inputData.initAssumingParSelAndHmmInitted(theGlobalVariables);
  ReflectionSubgroupWeylGroup& WeylFDSmallAsSubInLarge=inputData.WeylFDSmallAsSubInLarge;
  ReflectionSubgroupWeylGroup& WeylFDSmall=inputData.WeylFDSmall;
  SemisimpleLieAlgebra& theSmallAlgebra= inputData.theHmm.theDomain();
  Vectors<Rational>& embeddingsSimpleEiGoesTo=inputData.theHmm.ImagesCartanDomain;

//  std::cout << "the ambient Weyl of levi of parabolic: <br>" << inputData.WeylFD.ToString();
//  std::cout << "the small subgroup embedded: <br>" << WeylFDSmallAsSubInLarge.ToString();
//  std::cout << "the small subgroup: <br>" << WeylFDSmall.ToString();
//  std::cout << "<br>starting char: " << this->ToString();
  charSSAlgMod charAmbientFDWeyl, remainingCharProjected, remainingCharDominantLevI;

  MonomialChar<coefficient> tempMon, localHighest;
  List<coefficient> tempMults;
  HashedList<Vector<coefficient> > tempHashedRoots;
  coefficient bufferCoeff, highestCoeff;
  for (int i=0; i<this->size(); i++)
  { const MonomialChar<coefficient>& currentMon=(*this)[i];
    if (!inputData.WeylFD.FreudenthalEvalIrrepIsWRTLeviPart
        (currentMon.weightFundamentalCoordS, tempHashedRoots, tempMults, tempS, theGlobalVariables, 10000))
    { if (Report!=0)
        *Report=tempS;
      return false;
    }
//    std::cout << "<hr>FreudenthalEval on " << currentMon.ToString() << " generated the following report: "
//    << tempS << "<hr>";
    for (int j=0; j<tempHashedRoots.size; j++)
    { bufferCoeff=this->theCoeffs[i];
      tempMon.weightFundamentalCoordS=theWeyL.GetFundamentalCoordinatesFromSimple(tempHashedRoots[j]);
      tempMon.owner=this->GetOwner();
      bufferCoeff*=tempMults[j];
      charAmbientFDWeyl.AddMonomial(tempMon, bufferCoeff);
    }
  }
//  std::cout << "<hr>" << tempS;
//  std::cout << "<hr>Freudenthal eval ends up being: " << charAmbientFDWeyl.ToString();
  Vectors<coefficient> orbitDom;
  for (int i=0; i<charAmbientFDWeyl.size(); i++)
  { orbitDom.SetSize(0);
    if (!inputData.WeylFD.GenerateOrbitReturnFalseIfTruncated
        (theWeyL.GetSimpleCoordinatesFromFundamental(charAmbientFDWeyl[i].weightFundamentalCoordS), orbitDom, 10000))
    { out << "failed to generate the complement-sub-Weyl-orbit of weight "
      << theWeyL.GetSimpleCoordinatesFromFundamental(charAmbientFDWeyl[i].weightFundamentalCoordS).ToString();
//      std::cout << "failed to generate the complement-sub-Weyl-orbit of weight "
//      << theWeyL.GetSimpleCoordinatesFromFundamental
//      (charAmbientFDWeyl[i].weightFundamentalCoords).ToString();
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
    tempMon.owner=this->GetOwner();
    for (int k=0; k<orbitDom.size; k++)
      if (WeylFDSmallAsSubInLarge.IsDominantWeight(orbitDom[k]))
      { tempMon.weightFundamentalCoordS=theWeyL.GetFundamentalCoordinatesFromSimple(orbitDom[k]);
        remainingCharDominantLevI.AddMonomial(tempMon, charAmbientFDWeyl.theCoeffs[i]);
        //std::cout << "<br>" << orbitDom[k].ToString() << " with coeff " << charAmbientFDWeyl.theCoeffs[i].ToString();
      }// else
       //std::cout << "<br>" << orbitDom[k].ToString() << " is not dominant ";
  }
  FormatExpressions theFormat;
  theFormat.flagUseLatex=true;
  theFormat.CustomPlusSign="\\oplus ";
  theFormat.fundamentalWeightLetter="\\omega";
  out << "<br>Character w.r.t Levi part of the parabolic of the larger algebra: "
  << CGI::GetHtmlMathDivFromLatexAddBeginArrayL(remainingCharDominantLevI.ToString(&theFormat));
  //std::cout << "<br>Character w.r.t Levi part: " << CGI::GetHtmlMathDivFromLatexAddBeginArrayL
  //(remainingCharDominantLevI.ToString());
  remainingCharProjected.MakeZero();
  Vector<coefficient> fundCoordsSmaller, theProjection, inSimpleCoords;
  fundCoordsSmaller.SetSize(WeylFDSmall.AmbientWeyl.GetDim());
  for (int i=0; i<remainingCharDominantLevI.size(); i++)
  { inSimpleCoords=theWeyL.GetSimpleCoordinatesFromFundamental(remainingCharDominantLevI[i].weightFundamentalCoordS);
    for (int j=0; j<WeylFDSmall.AmbientWeyl.GetDim(); j++)
    { fundCoordsSmaller[j]=theWeyL.RootScalarCartanRoot(inSimpleCoords, embeddingsSimpleEiGoesTo[j]);
      fundCoordsSmaller[j]/=WeylFDSmall.AmbientWeyl.CartanSymmetric.elements[j][j]/2;
    }
    //std::cout << "<br>insimple coords: " << inSimpleCoords.ToString() << "; fundcoordssmaller: " << fundCoordsSmaller.ToString();
    tempMon.owner=&theSmallAlgebra;
    tempMon.weightFundamentalCoordS=fundCoordsSmaller;
    remainingCharProjected.AddMonomial(tempMon, remainingCharDominantLevI.theCoeffs[i]);
  }
//  std::cout << "<br>Character w.r.t subalgebra: " << CGI::GetHtmlMathDivFromLatexAddBeginArrayL
// (remainingCharProjected.ToString());
  Vector<coefficient> simpleGeneratorBaseField;
  output.MakeZero();
  while(!remainingCharProjected.IsEqualToZero())
  { localHighest=*remainingCharProjected.theMonomials.LastObject();
    for (bool Found=true; Found; )
    { Found=false;
      for (int i=0; i<WeylFDSmall.RootsOfBorel.size; i++)
      { tempMon=localHighest;
        simpleGeneratorBaseField=WeylFDSmall.RootsOfBorel[i]; // <- implicit type conversion here!
        tempMon.weightFundamentalCoordS+=WeylFDSmall.AmbientWeyl.GetFundamentalCoordinatesFromSimple(simpleGeneratorBaseField);
//        std::cout << "<br>candidate highest mon found simple & usual coords: "
//        << WeylFDSmall.AmbientWeyl.GetSimpleCoordinatesFromFundamental(tempMon.weightFundamentalCoords).ToString()
//        << " = " << tempMon.ToString();
        if (remainingCharProjected.theMonomials.Contains(tempMon))
        { localHighest=tempMon;
          Found=true;
        }
      }
    }
//    std::cout << "<br>The highest mon found simple & usual coords: "
//    << WeylFDSmall.AmbientWeyl.GetSimpleCoordinatesFromFundamental(localHighest.weightFundamentalCoords).ToString()
//    << " = " << localHighest.ToString();
    highestCoeff=remainingCharProjected.theCoeffs[remainingCharProjected.theMonomials.GetIndex(localHighest)];
    output.AddMonomial(localHighest, highestCoeff);
    if (!WeylFDSmall.FreudenthalEvalIrrepIsWRTLeviPart
        (localHighest.weightFundamentalCoordS, tempHashedRoots, tempMults, tempS, theGlobalVariables, 10000))
    { if (Report!=0)
        *Report=tempS;
      return false;
    }
//    std::cout << "<br>Freudenthal eval w.r.t. smaller subalgebra report: " << tempS;
//    std::cout << "<hr>accounting " << localHighest.ToString() << " with coeff "
//    << highestCoeff.ToString() << "<br>"
//    << remainingCharProjected.ToString();
    for (int i=0; i<tempHashedRoots.size; i++)
    { tempMon.owner=&theSmallAlgebra;
      tempMon.weightFundamentalCoordS=WeylFDSmall.AmbientWeyl.GetFundamentalCoordinatesFromSimple(tempHashedRoots[i]);
      bufferCoeff=tempMults[i];
      bufferCoeff*=highestCoeff;
      remainingCharProjected.SubtractMonomial(tempMon, bufferCoeff);
//      std::cout << "<br>-(" << bufferCoeff.ToString() << ")" << tempMon.ToString();
    }
//    std::cout << "<br>remaining character after accounting:<br>" << remainingCharProjected.ToString();
  }
  theFormat.fundamentalWeightLetter="\\psi";
  out << "<br>Character w.r.t the Levi part of the parabolic of the small algebra: " << CGI::GetHtmlMathDivFromLatexAddBeginArrayL
  (output.ToString(&theFormat))
  ;
//  std::cout << "<br>Character w.r.t Levi part: " << CGI::GetHtmlMathDivFromLatexAddBeginArrayL
//  (output.ToString())
//  ;

  if (Report!=0)
  { //out << "<hr>"  << "The split character is: " << output.ToString("V", "\\omega", false);
    DrawingVariables theDV1;
    std::string tempS;
    output.DrawMeNoMults(tempS, theGlobalVariables, theDV1, 10000);
    Vector<Rational> tempRoot, tempRoot2;
    WeylFDSmall.AmbientWeyl.ComputeAllElements(20);
    out << "<hr>";//In the following weight visualization, a yellow line is drawn if the corresponding weights are "
    //<< " simple reflections of one another, with respect to a simple Vector<Rational> of the Levi part of the parabolic subalgebra. ";
    for (int i=0; i<output.size(); i++)
    { tempRoot=WeylFDSmall.AmbientWeyl.GetSimpleCoordinatesFromFundamental(output[i].weightFundamentalCoordS).GetVectorRational();
//      smallWeyl.DrawContour
 //     (tempRoot, theDV1, theGlobalVariables, CGI::RedGreenBlue(200, 200, 0), 1000);
      std::stringstream tempStream;
      tempStream << output.theCoeffs[i].ToString();
      theDV1.drawTextAtVectorBuffer(tempRoot, tempStream.str(), 0, DrawingVariables::PenStyleNormal, 0);
      for (int j=1; j<WeylFDSmall.AmbientWeyl.theElements.size; j++)
      { tempRoot2=tempRoot;
        WeylFDSmall.AmbientWeyl.ActOnRhoModified(j, tempRoot2);
        theDV1.drawCircleAtVectorBuffer(tempRoot2, 5, DrawingVariables::PenStyleNormal, CGI::RedGreenBlue(200,0,0));
      }
    }
    out << "<hr>" << theDV1.GetHtmlFromDrawOperationsCreateDivWithUniqueName(WeylFDSmall.AmbientWeyl.GetDim());
/*    DrawingVariables theDV2;
    std::string tempS;
    this->DrawMeNoMults(tempS, theGlobalVariables, theDV2, 10000);
    Vector<Rational> tempRoot;
    out << "<hr>In the following weight visualization, a yellow line is drawn if the corresponding weights are "
    << " simple reflections of one another, with respect to a simple Vector<Rational> of the Levi part of the parabolic subalgebra. ";
    for (int i=0; i<output.size; i++)
    { tempRoot=theWeyl.GetSimpleCoordinatesFromFundamental(output[i].weightFundamentalCoords).GetVectorRational();
      outputSubGroup.DrawContour
      (tempRoot, theDV2, theGlobalVariables, CGI::RedGreenBlue(200, 200, 0), 1000);
      std::stringstream tempStream;
      tempStream << output.theCoeffs[i].ToString();
      theDV2.drawTextAtVectorBuffer(tempRoot, tempStream.str(), 0, DrawingVariables::PenStyleNormal, 0);
    }
    out << "<hr>" << theDV2.GetHtmlFromDrawOperationsCreateDivWithUniqueName(theWeyl.GetDim());*/
    *Report=out.str();
  }
//  std::cout << "<br>time at finish: " << theGlobalVariables.GetElapsedSeconds();
  return true;
}

bool CommandList::fJacobiSymbol
  (CommandList& theCommands, const Expression& input, Expression& output)
{ //this function is not implemented yet.
  return false;
  if (input.children.size!=3)
    return false;
  const Expression& leftE=input[1];
  const Expression& rightE=input[2];
  int leftInt, rightInt;
  if (!leftE.IsSmallInteger(&leftInt) || !rightE.IsSmallInteger(&rightInt))
    return false;
//  int result=Jacobi(leftInt, rightInt);
  return true;
}

bool CommandList::fParabolicWeylGroups
  (CommandList& theCommands, const Expression& input, Expression& output)
{ Selection selectionParSel;
  SemisimpleLieAlgebra* theSSPointer;
  std::string errorString;
  if (!theCommands.CallConversionFunctionReturnsNonConstUseCarefully
      (Serialization::innerSSLieAlgebra, input, theSSPointer, &errorString))
    return output.SetError(errorString, theCommands);
  SemisimpleLieAlgebra& theSSalgebra=*theSSPointer;
  int numCycles=MathRoutines::TwoToTheNth(selectionParSel.MaxSize);
  ReflectionSubgroupWeylGroup theSubgroup;
  std::stringstream out;
  for (int i=0; i<numCycles; i++, selectionParSel.incrementSelection())
  { theSubgroup.MakeParabolicFromSelectionSimpleRoots
    (theSSalgebra.theWeyl, selectionParSel, *theCommands.theGlobalVariableS, 2000)
    ;
    out << "<hr>" << CGI::GetHtmlMathDivFromLatexFormulA(theSubgroup.ToString());
  }
  return output.AssignValue(out.str(), theCommands);
}

bool CommandList::fParabolicWeylGroupsBruhatGraph
  (CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::fParabolicWeylGroupsBruhatGraph");
  RecursionDepthCounter theRecursion(&theCommands.RecursionDeptH);
  Selection parabolicSel;
  Vector<RationalFunctionOld> theHWfundcoords, tempRoot, theHWsimplecoords;
  Expression hwContext(theCommands);
  SemisimpleLieAlgebra* theSSalgPointer;
  if(!theCommands.innerGetTypeHighestWeightParabolic
    (theCommands, input, output, theHWfundcoords, parabolicSel, hwContext, theSSalgPointer,
     theCommands.innerRationalFunction))
    return output.SetError("Failed to extract highest weight vector data", theCommands);
  else
    if (output.IsError())
      return true;
  SemisimpleLieAlgebra& theSSalgebra=*theSSalgPointer;

  WeylGroup& theAmbientWeyl=theSSalgebra.theWeyl;
  ReflectionSubgroupWeylGroup theSubgroup;
  std::stringstream out;
  std::fstream outputFile, outputFile2;
  std::string fileName, filename2;
  fileName=theCommands.PhysicalNameDefaultOutput+"1";
  filename2=theCommands.PhysicalNameDefaultOutput+"2";
  CGI::OpenFileCreateIfNotPresent(outputFile, fileName+".tex", false, true, false);
  CGI::OpenFileCreateIfNotPresent(outputFile2, filename2+".tex", false, true, false);
  if (!theSubgroup.MakeParabolicFromSelectionSimpleRoots
      (theAmbientWeyl, parabolicSel, *theCommands.theGlobalVariableS, 500))
    return output.SetError
    ("<br><br>Failed to generate Weyl subgroup, 500 elements is the limit", theCommands);
  theSubgroup.FindQuotientRepresentatives(2000);
  out << "<br>Number elements of the coset: " << theSubgroup.RepresentativesQuotientAmbientOrder.size;
  out << "<br>Number of elements of the Weyl group of the Levi part: " << theSubgroup.size;
  out << "<br>Number of elements of the ambient Weyl: " << theSubgroup.AmbientWeyl.theElements.size;
  outputFile << "\\documentclass{article}\\usepackage[all,cmtip]{xy}\\begin{document}\n";
  outputFile2 << "\\documentclass{article}\\usepackage[all,cmtip]{xy}\\begin{document}\n";
  FormatExpressions theFormat;
  hwContext.ContextGetFormatExpressions(theFormat);
  if (theSubgroup.size>498)
  { if (theSubgroup.AmbientWeyl.GetSizeWeylGroupByFormula('E', 6) <=
        theSubgroup.AmbientWeyl.theDynkinType.GetSizeWeylGroupByFormula())
      out << "Even I can't handle the truth, when it is so large<br>";
    else
      out << "LaTeX can't handle handle the truth, when it is so large. <br>";
  }
  else
  { bool useJavascript=theSubgroup.size<100;
    outputFile << "\\[" << theSubgroup.ElementToStringBruhatGraph() << "\\]";
    outputFile << "\n\\end{document}";
    outputFile2 << "\\[" << theSubgroup.ElementToStringCosetGraph() << "\\]";
    outputFile2 << "\n\\end{document}";
    out << "<hr>"
    << " The Hasse graph of the Weyl group of the Levi part follows. <a href=\""
    << theCommands.DisplayNameDefaultOutput << "1.tex\"> "
    << theCommands.DisplayNameDefaultOutput << "1.tex</a>";
    out << ", <iframe src=\"" << theCommands.DisplayNameDefaultOutput
    << "1.png\" width=\"800\" height=\"600\">"
    << theCommands.DisplayNameDefaultOutput << "1.png</iframe>";
    out << "<hr>"
    << " The coset graph of the Weyl group of the Levi part follows. The cosets are right, i.e. a coset "
    << " of the subgroup X is written in the form Xw, where w is one of the elements below. "
    << "<a href=\""
    << theCommands.DisplayNameDefaultOutput
    << "2.tex\"> " <<  theCommands.DisplayNameDefaultOutput << "2.tex</a>";
    out << ", <iframe src=\"" << theCommands.DisplayNameDefaultOutput
    << "2.png\" width=\"800\" height=\"600\"> "
    << theCommands.DisplayNameDefaultOutput << "2.png</iframe>";
    out <<"<b>The .png file might be bad if LaTeX crashed while trying to process it; "
    << "please check whether the .tex corresponds to the .png.</b>";
    out << "<hr>Additional printout follows.<br> ";
    out << "<br>Representatives of the coset follow. Below them you can find the elements of the subgroup. <br>";
    out << "<table><tr><td>Element</td><td>weight simple coords</td><td>weight fund. coords</td></tr>";
    theFormat.fundamentalWeightLetter="\\omega";
    for (int i=0; i<theSubgroup.RepresentativesQuotientAmbientOrder.size; i++)
    { ElementWeylGroup& current=theSubgroup.RepresentativesQuotientAmbientOrder[i];
      out << "<tr><td>"
      << (useJavascript ? CGI::GetHtmlMathSpanPure(current.ToString()) : current.ToString())
      << "</td>";
      theHWsimplecoords=
      theSSalgebra.theWeyl.GetSimpleCoordinatesFromFundamental(theHWfundcoords);
      theSSalgebra.theWeyl.ActOnRhoModified
      (theSubgroup.RepresentativesQuotientAmbientOrder[i], theHWsimplecoords);
      out << "<td>"
      << (useJavascript ? CGI::GetHtmlMathSpanPure(theHWsimplecoords.ToString(&theFormat))
      : theHWsimplecoords.ToString(&theFormat))
      << "</td>";
      tempRoot = theSSalgebra.theWeyl.GetFundamentalCoordinatesFromSimple(theHWsimplecoords);
      std::string theFundString=
      tempRoot.ToStringLetterFormat(theFormat.fundamentalWeightLetter, &theFormat);
      out << "<td>" << (useJavascript ? CGI::GetHtmlMathSpanPure(theFundString): theFundString)
      << "</td>";
      out << "</tr>";
    }
    out << "</table><hr>";
    out << theSubgroup.ToString();
  }
  outputFile.close();
  outputFile2.close();
  std::cout << "<!--";
  std::cout.flush();
  std::string command1="latex  -output-directory=" + theCommands.PhysicalPathOutputFolder + " " + fileName + ".tex";
  std::string command2="dvipng " + fileName + ".dvi -o " + fileName + ".png -T tight";
  std::string command3="latex  -output-directory=" + theCommands.PhysicalPathOutputFolder + " " + filename2 + ".tex";
  std::string command4="dvipng " + filename2 + ".dvi -o " + filename2 + ".png -T tight";
//  std::cout << "<br>" << command1;
//  std::cout << "<br>" << command2;
//  std::cout << "<br>" << command3;
//  std::cout << "<br>" << command4;
  theCommands.theGlobalVariableS->System(command1);
  theCommands.theGlobalVariableS->System(command2);
  theCommands.theGlobalVariableS->System(command3);
  theCommands.theGlobalVariableS->System(command4);
  std::cout << "-->";
  return output.AssignValue(out.str(), theCommands);
}

bool CommandList::fPrintAllPartitions
  (CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::fPrintAllPartitions");
  RecursionDepthCounter theRecursion(&theCommands.RecursionDeptH);
  if (!input.IsListNElements(3))
    return output.SetError("Function fPrintAllPartitions expects 2 arguments.", theCommands);

  std::string errorString;
  SemisimpleLieAlgebra* theSSowner;
  if (!theCommands.CallConversionFunctionReturnsNonConstUseCarefully
      (Serialization::innerSSLieAlgebra, input[1], theSSowner, &errorString))
    return output.SetError(errorString, theCommands);

  SemisimpleLieAlgebra& theSSalgebra=*theSSowner;
  Expression theContext;
  Vector<Rational> theHW;
//  std::cout << theExpression.ToString() << " rank "
//  << theSSalgebra.GetRank() << " child one : " << input[2].ToString();
  if (!theCommands.GetVectoR<Rational>
      (input[2], theHW, &theContext, theSSalgebra.GetRank()))
    return output.SetError
    ("Failed to extract weight you want partitioned from "+ input[2].ToString(), theCommands);
  Vector<int> theHWint;
  theHWint.SetSize(theHW.size);
  for (int i=0; i<theHW.size; i++)
    if (!theHW[i].IsSmallInteger(&theHWint[i]) || theHW[i]<0)
      return output.SetError
      ("The input weight you gave is bad: it must consist of non-negative small integers", theCommands);
  std::stringstream out;
  out << "<br>the weight you want partitioned: " << theHWint;
  Vector<int> thePartition;
  thePartition.SetSize(theSSalgebra.GetNumPosRoots());
  for (int i=0; i<thePartition.size; i++)
    thePartition[i]=0;
  Vector<Rational> theWeight, tmpWt;
  Vectors<Rational>& rootsBorel=theSSalgebra.theWeyl.RootsOfBorel;
  int counter =0;
  int totalCycles=0;
  theWeight.MakeZero(theSSalgebra.GetRank());
  int i=rootsBorel.size;
  while (i>0 && counter<10000)// && totalCycles<100000)
  { totalCycles++;
    if (theWeight==theHW)
    { tmpWt=thePartition;
      out << "<br>" << tmpWt.ToStringLetterFormat("\\alpha");
      counter++;
    }
//    out << "<br>counter: " << counter << " partition: " << thePartition << " i: " << i;
    if (!(theHW-theWeight).IsPositive() || i>rootsBorel.size)
    { if (i<=rootsBorel.size)
      { theWeight-=rootsBorel[i-1]*thePartition[i-1];
        thePartition[i-1]=0;
      }
      i--;
      if (i>0)
      { theWeight+=rootsBorel[i-1];
        thePartition[i-1]++;
      }
    } else
      i++;
  }
  out << "<br>Done in " << totalCycles << " cycles.";
  out << "<br>" << counter << " total partitions ";
  return output.AssignValue(out.str(), theCommands);
}

void WeylGroup::GetHighestWeightsAllRepsDimLessThanOrEqualTo
  (List<Vector<Rational> >& outputHighestWeightsFundCoords, int inputDimBound
   )
{ if (inputDimBound<1)
  { outputHighestWeightsFundCoords.SetSize(0);
    return;
  }
  HashedList<Vector<Rational> > output;
  Vector<Rational> current;
  current.MakeZero(this->GetDim());
  output.AddOnTop(current);
  Rational theDim;
  Rational dimBound=inputDimBound+1;
  for (int i=0; i<output.size; i++)
  { current=output[i];
    for (int k=0; k<this->GetDim(); k++)
    { current[k]+=1;
      theDim=this->WeylDimFormulaFundamentalCoords(current);
      std::string DebugString=current.ToString();
      std::string DebugString2=theDim.ToString();
      if (theDim< dimBound)
        output.AddOnTopNoRepetition(current);
      current[k]-=1;
    }
  }
  outputHighestWeightsFundCoords=output;
}

bool CommandList::fTestMonomialBaseConjecture
  (CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::fTestMonomialBaseConjecture");
  RecursionDepthCounter theRecursion(&theCommands.RecursionDeptH);
  if (!input.IsListNElements(3))
    return output.SetError("fTestMonomialBaseConjecture takes two arguments as input", theCommands);
  const Expression& rankE=input[1];
  const Expression& dimE=input[2];
  int rankBound=0;
  int dimBound=0;
  if (!rankE.IsSmallInteger(&rankBound) || !dimE.IsSmallInteger(&dimBound))
    return output.SetError("The rank and  dim bounds must be small integers", theCommands);
  if (rankBound<2 || rankBound > 100 || dimBound <1 || dimBound > 10000)
    return output.SetError
    ("The rank bound must be an integer between 2 and 100, and the dim bound must be an integer between 1 and 10000", theCommands);
  std::stringstream out;
  List<int> theRanks;
  List<char> theWeylLetters;
  for (int i=2; i<=rankBound; i++)
  { theRanks.AddOnTop(i);
    theWeylLetters.AddOnTop('A');
    theRanks.AddOnTop(i);
    theWeylLetters.AddOnTop('B');
    theRanks.AddOnTop(i);
    theWeylLetters.AddOnTop('C');
    if (i>=4)
    { theRanks.AddOnTop(i);
      theWeylLetters.AddOnTop('D');
    }
    if (i>=6 && i<=8)
    { theRanks.AddOnTop(i);
      theWeylLetters.AddOnTop('E');
    }
    if (i==4 )
    { theRanks.AddOnTop(i);
      theWeylLetters.AddOnTop('F');
    }
    if (i==2 )
    { theRanks.AddOnTop(i);
      theWeylLetters.AddOnTop('G');
    }
  }
  List<List<Vector<Rational> > > theHighestWeights;
  theHighestWeights.SetSize(theRanks.size);
  ModuleSSalgebra<Rational> theMod;
  bool foundBad=false;
  theCommands.theObjectContainer.theLieAlgebras.SetSize(theRanks.size);
  Selection tempSel;
  std::stringstream latexReport;
  latexReport << "\\documentclass{article} <br>\\usepackage{longtable}\\begin{document}<br>\n\n\n\n\n";
  latexReport << " \\begin{longtable}{|lllll|} ";
  ProgressReport theReport(theCommands.theGlobalVariableS);
  bool ConjectureBholds=true;
  bool ConjectureCholds=true;
  LittelmannPath hwPath;
  List<LittelmannPath> tempList;
  List<List<int> > theStrings;
  MonomialTensor<int, MathRoutines::IntUnsignIdentity> tempMon;
  for (int i=0; i<theRanks.size; i++)
  { SemisimpleLieAlgebra& currentAlg=theCommands.theObjectContainer.theLieAlgebras[i];
    currentAlg.theWeyl.MakeArbitrarySimple(theWeylLetters[i], theRanks[i]);
    currentAlg.ComputeChevalleyConstantS(theCommands.theGlobalVariableS);
    currentAlg.theWeyl.GetHighestWeightsAllRepsDimLessThanOrEqualTo(theHighestWeights[i], dimBound);
    latexReport << "\\hline\\multicolumn{5}{c}{"
    << "$" << currentAlg.GetLieAlgebraName() << "$}\\\\\\hline\n\n"
    << "$\\lambda$ & dim &\\# pairs 1& \\# pairs total  & \\# Arithmetic op.  \\\\\\hline";
    out << "<br>"
    << " <table><tr><td  border=\"1\" colspan=\"3\">"
    << theWeylLetters[i] << "_{" << theRanks[i] << "}"
    << "</td></tr> <tr><td>highest weight</td><td>dim</td></tr>";
    List<Vector<Rational> >& theHws=theHighestWeights[i];
    tempSel.init(theRanks[i]);
    for (int j=0; j<theHws.size; j++)
    { std::stringstream reportStream;

      Vector<Rational>& currentHW=theHws[j];

      out << "<tr><td> " << currentHW.ToString() << "</td><td>"
      << currentAlg.theWeyl.WeylDimFormulaFundamentalCoords(currentHW) << "</td>";
      reportStream << "Processing " << currentAlg.GetLieAlgebraName()
      << ", index  "<< i+1 << " out of " << theRanks.size << ",  highest weight "
      << currentHW.ToString() << ", dim: "
      << currentAlg.theWeyl.WeylDimFormulaFundamentalCoords(currentHW)
      << ", index " << j+1 << " out of " << theHws.size;
      theReport.Report(reportStream.str());
      latexReport << "$" << currentHW.ToStringLetterFormat("\\omega") << "$ &"
      << currentAlg.theWeyl.WeylDimFormulaFundamentalCoords(currentHW) << "&";
//      int startRatOps=Rational::TotalLargeAdditions+Rational::TotalSmallAdditions
//      +Rational::TotalLargeMultiplications+Rational::TotalSmallMultiplications;
      hwPath.MakeFromWeightInSimpleCoords
      (currentAlg.theWeyl.GetSimpleCoordinatesFromFundamental(currentHW), currentAlg.theWeyl);
//      double timeBeforeOrbit=theCommands.theGlobalVariableS->GetElapsedSeconds();
      hwPath.GenerateOrbit
      (tempList, theStrings, *theCommands.theGlobalVariableS,
      MathRoutines::Minimum(1000, currentAlg.theWeyl.WeylDimFormulaFundamentalCoords(currentHW).NumShort),
       0);
      reportStream << "\nPath orbit size = " << theStrings.size << " generated in "
      << theCommands.theGlobalVariableS->GetElapsedSeconds() << " seconds.";
      theReport.Report(reportStream.str());
      for (int k=0; k<theStrings.size; k++)
      { LittelmannPath& currentPath=tempList[k];
        tempMon=theStrings[k];
        tempMon.generatorsIndices.ReverseOrderElements();
        tempMon.Powers.ReverseOrderElements();
        if (!currentPath.IsAdaptedString(tempMon))
        { foundBad=true;
          break;
        }
      }
      if (!foundBad)
        out << "<td>all strings adapted</td>";
      else
        out << "<td>has non-adapted string</td>";
//      goto tmp;
/*      if (theMod.MakeFromHW
          (theCommands.theObjectContainer.theLieAlgebras, i,
           currentHW, tempSel, *theCommands.theGlobalVariableS, 1, 0, 0, true))
      { out << "<td>is good</td>";
        if (!theMod.flagConjectureBholds)
        { out << "<td><b>conjecture B fails!</b></td>";
          ConjectureBholds=false;
        }
        if (!theMod.flagConjectureCholds)
        { out << "<td><b>conjecture C holds</b></td>";
          ConjectureCholds=false;
        }
        if (theMod.NumCachedPairsBeforeSimpleGen>=theMod.MaxNumCachedPairs)
          latexReport << "$ \\geq$ " << theMod.MaxNumCachedPairs;
        else
          latexReport << theMod.NumCachedPairsBeforeSimpleGen;
        latexReport
        << "&" << theMod.cachedPairs.size << " & "
        << Rational::TotalLargeAdditions+Rational::TotalSmallAdditions
        +Rational::TotalLargeMultiplications+Rational::TotalSmallMultiplications -
        startRatOps;
      }
      else
      { latexReport << " & \\textbf{BAD}";
        out << "<td><b>Is bad!!!!</b></td>";
        theReport.Report("BAD BAD BAD!!!");
        foundBad=true;
        break;
      }*/
//      tmp:
      out << "</tr>";
      if (foundBad)
        break;
      latexReport << "\\\\";
    }
    out << "</table>";
    if (foundBad)
      break;
  }
  latexReport << "\\end{longtable} \n\n\n\n";
  if (ConjectureBholds)
    latexReport << " Conjecture B holds for all computed entries.";
  else
    latexReport << "Conjecture B fails.";
  if (ConjectureCholds)
    latexReport << "Conjecture C holds for all computed entries.";
  else
    latexReport << "Conjecture C fails.";
  latexReport << "\\end{document}";
  out << "<br><br>\n\n\n\n\n" << latexReport.str();
  return output.AssignValue(out.str(), theCommands);
}

bool CommandList::fLittelmannOperator
  (CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::fLittelmannOperator");
  RecursionDepthCounter theRecursionIncrementer(&theCommands.RecursionDeptH);
  if (input.HasBoundVariables())
    return false;
  int theIndex=0;
  if (!input.IsSmallInteger(&theIndex))
    return output.SetError
    ("The argument of the Littelmann root operator is expected to be a small integer, instead you gave me"
     +input.ToString(), theCommands);
  if (theIndex==0)
    return output.SetError
    ("The index of the Littelmann root operator is expected to be non-zero", theCommands);

  return output.AssignValue(theIndex, theCommands);
}

bool CommandList::fLSPath
  (CommandList& theCommands, const Expression& input, Expression& output)
{ RecursionDepthCounter theRecutionIncrementer(&theCommands.RecursionDeptH);
  MacroRegisterFunctionWithName("CommandList::fLSPath");
  if (input.children.size<3)
    return output.SetError("LSPath needs at least two arguments.", theCommands);

  std::string errorString;
  SemisimpleLieAlgebra* theSSowner;
  if (!theCommands.CallConversionFunctionReturnsNonConstUseCarefully
      (Serialization::innerSSLieAlgebra, input[1], theSSowner, &errorString))
    return output.SetError(errorString, theCommands);

  SemisimpleLieAlgebra& ownerSSalgebra=*theSSowner;
  Vectors<Rational> waypoints;
  waypoints.SetSize(input.children.size-2);
  for (int i=2; i<input.children.size; i++)
    if (!theCommands.GetVectoR<Rational>(input[i], waypoints[i-2], 0, ownerSSalgebra.GetRank(), 0))
      return output.SetError("Failed to extract waypoints", theCommands);
  waypoints=ownerSSalgebra.theWeyl.GetSimpleCoordinatesFromFundamental(waypoints);
  LittelmannPath theLSpath;
  theLSpath.MakeFromWaypoints(waypoints, ownerSSalgebra.theWeyl);
  return output.AssignValue(theLSpath, theCommands);
}

bool CommandList::innerInvertMatrix
(CommandList& theCommands, const Expression& input, Expression& output)
{ Matrix<Rational> mat, outputMat, tempMat;
  if (!theCommands.GetMatriXFromArguments<Rational>(input, mat, 0, -1, 0))
    return output.SetError
    ("Failed to extract matrix with rational coefficients", theCommands);
  if (mat.NumRows!=mat.NumCols || mat.NumCols<1)
    return output.SetError("The matrix is not square", theCommands);
  outputMat.MakeIdMatrix(mat.NumRows);
  int tempI;
  int NumFoundPivots = 0;
  std::stringstream out;
  Rational tempElement;
  FormatExpressions theFormat;
  theFormat.flagUseLatex=true;
  theFormat.flagUseHTML=false;
  theFormat.MatrixColumnVerticalLineIndex=mat.NumCols-1;
  out << "Computing " << CGI::GetHtmlMathSpanPure(mat.ToString(&theFormat) + "^{-1}");
  tempMat=mat;
  tempMat.AppendMatrixOnTheRight(outputMat);
  out << "<br>" << CGI::GetHtmlMathSpanPure(tempMat.ToString(&theFormat)) ;
  for (int i=0; i<mat.NumCols; i++)
  { tempI = mat.FindPivot(i, NumFoundPivots);
    if (tempI!=-1)
    { if (tempI!=NumFoundPivots)
      { mat.SwitchTwoRows(NumFoundPivots, tempI);
        outputMat.SwitchTwoRows (NumFoundPivots, tempI);
        out << "<br>switch row " << NumFoundPivots+1 << " and row " << tempI+1 << ": ";
        tempMat=mat;
        tempMat.AppendMatrixOnTheRight(outputMat);
        out << "<br>" << CGI::GetHtmlMathSpanPure(outputMat.ToString(&theFormat));
      }
      tempElement=mat.elements[NumFoundPivots][i];
      tempElement.Invert();
      if (tempElement!=1)
        out << "<br> multiply row " << NumFoundPivots+1 << " by " << tempElement << ": ";
      mat.RowTimesScalar(NumFoundPivots, tempElement);
      outputMat.RowTimesScalar(NumFoundPivots, tempElement);
      if (tempElement!=1)
      { tempMat=mat;
        tempMat.AppendMatrixOnTheRight(outputMat);
        out << CGI::GetHtmlMathSpanPure(tempMat.ToString(&theFormat));
      }
      bool found = false;
      for (int j = 0; j<mat.NumRows; j++)
        if (j!=NumFoundPivots)
          if (!mat.elements[j][i].IsEqualToZero())
          { tempElement=(mat.elements[j][i]);
            tempElement.Minus();
            mat.AddTwoRows(NumFoundPivots, j, i, tempElement);
            outputMat.AddTwoRows(NumFoundPivots, j, 0, tempElement);
            if(!found)
              out << "<br>";
            else
              out << ", ";
            found =true;
            out << " Row index " << NumFoundPivots+1 << " times "
            << tempElement << " added to row index " << j+1;
          }
      if (found)
      { out << ": <br> ";
        tempMat=mat;
        tempMat.AppendMatrixOnTheRight(outputMat);
        out << CGI::GetHtmlMathSpanPure(tempMat.ToString(&theFormat));
      }
      NumFoundPivots++;
    }
  }
  if (NumFoundPivots<mat.NumRows)
    out << "<br>Matrix to the right of the vertical line not transformed to the identity matrix => starting matrix is not invertible. ";
  else
    out << "<br>The inverse of the starting matrix can be read off on the matrix to the left of the id matrix: "
    << CGI::GetHtmlMathSpanPure(output.ToString(&theFormat));
  return output.AssignValue(out.str(), theCommands);
}

bool CommandList::fDifferential
(CommandList& theCommands, const Expression& input, Expression& output)
{ return false;
  /* if (!theExpression.EvaluatesToAtom())
    if (!theCommands.CallCalculatorFunction
        (theCommands., inputIndexBoundVars, theExpression, comments))
      return output.SetError("Failed to convert argument of differential to differential form. ");
  Data theData;
  theData=theExpression.GetAtomicValue();
  if (!theData.ConvertToTypE<DifferentialForm<Rational> >())
    return output.SetError("Failed to convert argument of differential to differential form.");

  theCommands.innerPolynomial(the)*/
  return true;
}

bool LargeIntUnsigned::Factor(List<unsigned int>& outputPrimeFactors, List<int>& outputMultiplicites)
{ if (this->size>1)
    return false;
  if (this->IsEqualToZero())
  { std::cout << "This is a programming error: it was requested that I factor 0, which is forbidden."
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  unsigned int n=(*this)[0];
  outputPrimeFactors.size=0;
  outputMultiplicites.size=0;
  while (n%2==0)
  { this->AccountPrimeFactor(2, outputPrimeFactors, outputMultiplicites);
    n/=2;
  }
  unsigned int upperboundPrimeDivisors= (unsigned int) sqrt((double)n);
  List<bool> theSieve;
  theSieve.initFillInObject(upperboundPrimeDivisors+1,true);
  for (unsigned int i=3; i<=upperboundPrimeDivisors; i+=2)
    if (theSieve[i])
    { while (n%i==0)
      { this->AccountPrimeFactor(i, outputPrimeFactors, outputMultiplicites);
        n/=i;
        upperboundPrimeDivisors= (unsigned int) sqrt((double)n);
      }
      for (unsigned int j=i; j<=upperboundPrimeDivisors; j+=i)
        theSieve[j]=false;
    }
  if (n>1)
    this->AccountPrimeFactor(n, outputPrimeFactors, outputMultiplicites);
  return true;
}

template <class coefficient>
void Polynomial<coefficient>::Interpolate(const Vector<coefficient>& thePoints, const Vector<coefficient>& ValuesAtThePoints)
{ Polynomial<coefficient> theLagrangeInterpolator, tempP;
  this->MakeZero();
  for (int i=0; i<thePoints.size; i++)
  { theLagrangeInterpolator.MakeConst(1, 1);
    for (int j=0; j<thePoints.size; j++)
      if (i!=j)
      { tempP.MakeDegreeOne(1, 0, 1, -thePoints[j]);
        tempP/=thePoints[i]-thePoints[j];
        theLagrangeInterpolator*=tempP;
      }
    theLagrangeInterpolator*=ValuesAtThePoints[i];
    *this+=theLagrangeInterpolator;
  }
}

template <class coefficient>
void Polynomial<coefficient>::GetValuesLagrangeInterpolandsAtConsecutivePoints
(Vector<Rational>& inputConsecutivePointsOfInterpolation, Vector<Rational>& inputPointsOfEvaluation, Vectors<Rational>& outputValuesInterpolands)
{ outputValuesInterpolands.SetSize(inputConsecutivePointsOfInterpolation.size);
  for (int i=0; i<inputConsecutivePointsOfInterpolation.size; i++)
  { Vector<Rational>& currentInterpoland=outputValuesInterpolands[i];
    currentInterpoland.SetSize(inputPointsOfEvaluation.size);
    for (int j=0; j<inputConsecutivePointsOfInterpolation.size; j++)
      currentInterpoland[j]= i==j ? 1 : 0;
    for (int j=inputConsecutivePointsOfInterpolation.size; j<inputPointsOfEvaluation.size; j++)
    { currentInterpoland[j]=1;
      for (int k=0; k<inputConsecutivePointsOfInterpolation.size; k++)
        if(i!=k)
        { currentInterpoland[j]*=inputPointsOfEvaluation[j]-inputConsecutivePointsOfInterpolation[k];
          currentInterpoland[j]/=inputConsecutivePointsOfInterpolation[i]-inputConsecutivePointsOfInterpolation[k];
        }
    }
  }
}

template <class coefficient>
bool Polynomial<coefficient>::
FactorMeOutputIsSmallestDivisor(Polynomial<Rational>& output, std::stringstream* comments)
{ MacroRegisterFunctionWithName("Polynomial_CoefficientType::FactorMeOutputIsSmallestDivisor");
  if (this->GetMinNumVars()>1)
    return false;
  if (this->GetMinNumVars()==0)
    return true;
  Polynomial<Rational> thePoly=*this;
  Rational theMultiple=thePoly.ScaleToIntegralMinHeightOverTheRationalsReturnsWhatIWasMultipliedBy();
  int degree=0;
  if (!thePoly.TotalDegree().IsSmallInteger(&degree))
    return false;
  int degreeLeft=degree/2;
  int degreeRight=degree-degreeLeft;
  Vector<Rational> AllPointsOfEvaluation;
  List<List<unsigned int> > thePrimeFactorsAtPoints;
  List<List<int> > thePrimeFactorsMults;
  Vector<Rational> theValuesAtPoints, theValuesAtPointsLeft, theValuesAtPointsRight;
  AllPointsOfEvaluation.SetSize(degree+1);
  thePrimeFactorsAtPoints.SetSize(degreeLeft+1);
  thePrimeFactorsMults.SetSize(degreeLeft+1);
  std::cout << "<br><b>Factoring: " << this->ToString() << "</b>";
  std::cout << "<br>Degree left: " << degreeLeft;
  std::cout << "<br>Interpolating at: 0,";
  AllPointsOfEvaluation[0]=0;
  for (int i=1; i<AllPointsOfEvaluation.size; i++)
  { AllPointsOfEvaluation[i]= i%2==1 ? i/2+1 : -(i/2);
    std::cout << AllPointsOfEvaluation[i].ToString() << ", ";
  }
  Vector<Rational> theArgument;
  theArgument.SetSize(1);
  theValuesAtPoints.SetSize(AllPointsOfEvaluation.size);
  LargeInt tempLI;
  for (int i=0; i<AllPointsOfEvaluation.size; i++)
  { theArgument[0]=AllPointsOfEvaluation[i];
    theValuesAtPoints[i]= thePoly.Evaluate(theArgument);
    std::cout << "<br>" << thePoly.ToString() << " evaluated at " << theArgument[0].ToString()
    << " equals: " << theValuesAtPoints[i].ToString();
    if (theValuesAtPoints[i].IsEqualToZero())
    { output.MakeDegreeOne(1, 0, 1, -theArgument[0]);
      std::cout << "<hr>Found a divisor, and it is: " << output.ToString();
      *this/=output;
      std::cout << "<br>divident: " << this->ToString();
      return true;
    }
    std::cout << "<br>value at " << AllPointsOfEvaluation[i].ToString() << " = " << theValuesAtPoints[i].ToString();
    if (i<degreeLeft+1)
    { theValuesAtPoints[i].IsInteger(&tempLI);
      if(!tempLI.value.Factor(thePrimeFactorsAtPoints[i], thePrimeFactorsMults[i]))
      { if (comments!=0)
          *comments << "<br>Aborting polynomial factorization: failed to factor the integer "
          << theValuesAtPoints[i].ToString() << " (most probably the integer is too large).";
        return false;
      }
      std::cout << "=+/- ";
      for (int j=0; j<thePrimeFactorsAtPoints[i].size; j++)
        for (int k=0; k<thePrimeFactorsMults[i][j]; k++)
          std::cout << thePrimeFactorsAtPoints[i][j] << "*";
    }
  }
  Incrementable<Incrementable<SelectionOneItem> > theDivisorSel;
  Incrementable<SelectionOneItem> signSel;
  Vector<Rational> eiVector;
  Polynomial<Rational> interPoly, checkRemainder;
  Vectors<Rational> valuesLeftInterpolands, valuesRightInterpolands;
  Vector<Rational> PointsOfInterpolationLeft, PointsOfInterpolationRight;
  PointsOfInterpolationLeft.ReservE(degreeLeft+1);
  PointsOfInterpolationRight.ReservE(degreeRight+1);
  Vector<Rational> theArgumentRat;
  Rational currentPrimePowerContribution, currentPointContribution;
  theArgumentRat.SetSize(1);
  for (int i=0; i<=degreeLeft; i++)
    PointsOfInterpolationLeft.AddOnTop(AllPointsOfEvaluation[i]);
  for (int i=0; i<=degreeRight; i++)
    PointsOfInterpolationRight.AddOnTop(AllPointsOfEvaluation[i]);
  theDivisorSel.initFromMults(thePrimeFactorsMults);
  signSel.initFromMults(1, degreeLeft+1);
//  signSel.theElements[0].initFromMults(0);
  valuesLeftInterpolands.SetSize(degreeLeft+1);
  valuesRightInterpolands.SetSize(degreeRight+1);
  this->GetValuesLagrangeInterpolandsAtConsecutivePoints(PointsOfInterpolationLeft, AllPointsOfEvaluation, valuesLeftInterpolands);
  if (degreeLeft!=degreeRight)
    this->GetValuesLagrangeInterpolandsAtConsecutivePoints(PointsOfInterpolationRight, AllPointsOfEvaluation, valuesRightInterpolands);
  else
    valuesRightInterpolands=valuesLeftInterpolands;
  do do
  { //std::cout << "<hr>Selection: " << theDivisorSel.ToString() << "<br>Sign selection: " << signSel.ToString();
      //continue;
    theValuesAtPointsLeft.MakeZero(theValuesAtPoints.size);
    Rational firstValue;
    bool isGood=false;//<-we shall first check if the divisor is constant.
    std::cout << "<hr>Values left candidate: ";
    for (int j=0; j<theDivisorSel.theElements.size; j++)
    { currentPointContribution=1;
      for (int k=0; k<theDivisorSel[j].theElements.size; k++)
      { currentPrimePowerContribution=thePrimeFactorsAtPoints[j][k];
        currentPrimePowerContribution.RaiseToPower(theDivisorSel[j][k].SelectedMult);
        currentPointContribution*=currentPrimePowerContribution;
      }
      if (signSel[j].SelectedMult == 1)
        currentPointContribution*=-1;
      if (j==0)
        firstValue=currentPointContribution;
      else
        if (firstValue!=currentPointContribution)
          isGood=true; //<- the divisor has takes two different values, hence is non-constant.
      std::cout << " at " << AllPointsOfEvaluation[j].ToString() << " -> " << currentPointContribution.ToString();
      //std::cout << theValuesAtPointsLeft.ToString();
//      std::cout << "<br>adding " << (valuesLeftInterpolands[j]*currentPointContribution).ToString()
//      << " to " << theValuesAtPointsLeft.ToString();
      std::cout.flush();
      theValuesAtPointsLeft+= valuesLeftInterpolands[j]*currentPointContribution;
//      std::cout << " to get " << theValuesAtPointsLeft.ToString();
    }
    std::cout << (isGood ? " is good, " : " NO GOOD, ") << " final values at points left: " << theValuesAtPointsLeft.ToString();
    if (!isGood)
      continue;
    theValuesAtPointsRight.MakeZero(theValuesAtPoints.size);
    for (int j=0; j<valuesRightInterpolands.size; j++)
    { if (theValuesAtPointsLeft[j].IsEqualToZero() )
      { isGood=false;
        break;
      }
      currentPointContribution=(theValuesAtPoints[j])/theValuesAtPointsLeft[j];
      if (!currentPointContribution.IsInteger())
      { isGood=false;
        break;
      }
      theValuesAtPointsRight+=valuesRightInterpolands[j]*currentPointContribution;
    }
    std::cout << (isGood ? "<br>Right is good, " : "Right is NO GOOD, ") << "values at points right: " << theValuesAtPointsRight.ToString();
    if (!isGood)
      continue;
    for (int k=valuesRightInterpolands.size; k<theValuesAtPoints.size; k++)
      if ((theValuesAtPointsLeft[k]*theValuesAtPointsRight[k])!=theValuesAtPoints[k])
      { isGood=false;
        break;
      }
    if (!isGood)
      continue;
    output.Interpolate((Vector<Rational>) PointsOfInterpolationLeft, (Vector<Rational>) theValuesAtPointsLeft);
    this->DivideBy(output, interPoly, checkRemainder);
    if (!checkRemainder.IsEqualToZero())
    { std::cout << "This is a programming error: polynomial " << output.ToString()
      << " was computed to be a divisor of " << this->ToString() << " but it is not. "
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    *this=interPoly;
    return true;
  } while (theDivisorSel.IncrementReturnFalseIfBackToBeginning());
  while (signSel.IncrementReturnFalseIfBackToBeginning());
  output=*this;
  this->MakeOne(1);
  return true;
}

bool CommandList::innerFactorPoly
(CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::innerFactorPoly");
  RecursionDepthCounter theRecursionIncrementer(&theCommands.RecursionDeptH);
  std::cout << "here I am .";
  if (!theCommands.CallCalculatorFunction(Serialization::innerPolynomial, input, output))
    return false;
  std::cout << "here I am .";
  Expression theContext=output.GetContext();
  Polynomial<Rational> thePoly=output.GetValuE<Polynomial<Rational> >();
  if (thePoly.GetMinNumVars()>1)
    return output.SetError
    ("I have been taught to factor one variable polys only. ", theCommands);
  Polynomial<Rational> smallestDiv;
  List<Polynomial<Rational> > theFactors;
  while (!thePoly.IsAConstant())
  { if(!thePoly.FactorMeOutputIsSmallestDivisor(smallestDiv, &theCommands.Comments))
      return false;
    std::cout << "<hr><b>Smallest divisor: " << smallestDiv.ToString() << ", thepoly: " << thePoly.ToString()
    << "</b>";
    Rational tempRat=smallestDiv.ScaleToIntegralMinHeightFirstCoeffPosReturnsWhatIWasMultipliedBy();
    thePoly/=tempRat;
    theFactors.AddOnTop(smallestDiv);
  }
  output.reset(theCommands, theFactors.size+1);
  Expression tempE;
  output.AddChildAtomOnTop(theCommands.opSequence());
  for (int i=0; i<theFactors.size; i++)
  { tempE.AssignValueWithContext(theFactors[i], theContext, theCommands);
    output.AddChildOnTop(tempE);
  }
  output.format=output.formatMatrix;
  std::cout << "<hr>At this point of time, theExpression is: " << output.ToString();
  return true;
}

bool CommandList::innerZmodP
(CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::innerZmodP");
  if (!input.IsListNElements(3))
    return false;
  Rational left, right;
  if (!input[1].IsOfType<Rational>(&left) || ! input[2].IsOfType<Rational>(&right))
    return false;
  LargeInt base;
  if (!right.IsInteger(&base))
    return false;
  if (base.IsEqualToZero())
    return false;
  LargeIntUnsigned theDen, theNum, theGCD;
  left.GetDenominator(theDen);
  LargeIntUnsigned::gcd(theDen, base.value, theGCD);
  if (theGCD>1)
    return false;
  left.GetNumerator(theNum);
  ElementZmodP outputElt;
  outputElt.theModulo=base.value;
  outputElt=theNum;
  return output.AssignValue(outputElt, theCommands);

}

bool CommandList::innerDouble
(CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::innerDouble");
  Rational ratValue;
  if (input.IsOfType<double>())
  { output=input;
    return true;
  }
  if (!input.IsOfType(&ratValue))
  { std::cout << "input not of rat value: " << input.ToString();
    return false;
  }
  return output.AssignValue(ratValue.DoubleValue(), theCommands);
}

bool CommandList::innerSqrt
(CommandList& theCommands, const Expression& input, Expression& output)
{ if (!input.IsOfType<Rational>())
    return false;
  AlgebraicNumber theNumber;
  theNumber.AssignRationalRadical(input.GetValuE<Rational>(), theCommands.theObjectContainer.theAlgebraicClosure);
  return output.AssignValue(theNumber, theCommands);
}

bool CommandList::innerPrintZnEnumeration
(CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::innerPrintZnEnumeration");
  if (!input.IsListNElements(3))
    return false;
  int grade, dimension;
  if (!input[2].IsSmallInteger(& grade) || !input[1].IsSmallInteger(&dimension))
    return false;
  if (grade>10 || dimension >5 || grade<0 || dimension<0)
    return false;
  SelectionPositiveIntegers theSel;
  theSel.init(dimension);
  std::stringstream out2, out;
  LargeIntUnsigned gradeLarge=(unsigned) grade;
  int counter=0;
  for (theSel.SetFirstInGradeLevel(gradeLarge); theSel.GetGrading()==gradeLarge; theSel.IncrementReturnFalseIfBackToBeginning())
  { out2 << theSel.ToString() << "<br>";
    counter++;
  }
  out << "Total " << counter << " vectors:<br>" << out2.str();
  return output.AssignValue(out.str(), theCommands);
}

bool Expression::AssignMatrixExpressions
(const Matrix<Expression>& input, CommandList& owner)
{ this->reset(owner, input.NumRows+1);
  this->AddChildAtomOnTop(owner.opSequence());
  Expression currentRow;
  for (int i=0; i<input.NumRows; i++)
  { currentRow.reset(owner);
    currentRow.children.ReservE(input.NumCols+1);
    currentRow.AddChildAtomOnTop(owner.opSequence());
    for (int j=0; j<input.NumCols; j++)
      currentRow.AddChildOnTop(input(i,j));
    currentRow.format=this->formatMatrixRow;
    this->AddChildOnTop(currentRow);
  }
  this->format=this->formatMatrix;
  return true;
}

bool CommandList::GetMatrixExpressions
(const Expression& input, Matrix<Expression>& output, int desiredNumRows, int desiredNumCols)
{ if (!input.IsSequenceNElementS())
  { output.init(1,1);
    output(0,0)=input;
    return true;
  }
  if (!input[1].IsSequenceNElementS())
  { if (desiredNumRows>0)
      if (desiredNumRows!=1)
        return false;
    if (desiredNumCols>0)
      if (desiredNumCols!=input.children.size-1)
        return false;
    output.init(1, input.children.size-1);
    for (int i=1; i<input.children.size; i++)
      output(0, i-1)=input[i];
    return true;
  }
  if (desiredNumRows>0)
    if (desiredNumRows!=input.children.size-1)
      return false;
  if (desiredNumCols>0)
    if (desiredNumCols!=input[1].children.size-1)
      return false;
  output.init(input.children.size-1, input[1].children.size-1);
  for (int i=1; i<input.children.size; i++)
    if (input[i].IsSequenceNElementS(output.NumCols))
      for (int j=1; j<input[i].children.size; j++)
        output(i-1, j-1)=input[i][j];
    else
      return false;
  return true;
}

class DoxygenInstance
{
  public:
  ElementTensorsGeneralizedVermas<RationalFunctionOld> doXyCanYouParseME;
};


