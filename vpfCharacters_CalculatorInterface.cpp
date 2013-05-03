#include "vpf.h"
#include "vpfCharacters.h"
#include "vpfGraph.h"
#include "vpfCharacters_CalculatorInterface.h"

static ProjectInformationInstance ProjectInfoVpfCharactersCalculatorInterfaceCPP
(__FILE__, "Weyl group calculator interface");

bool WeylGroupCalculatorFunctions::innerWeylOrbit
(CommandList& theCommands, const Expression& input, Expression& output,
 bool useFundCoords, bool useRho)
{ if (!input.IsListNElements(3))
    return output.SetError("innerWeylOrbit takes two arguments", theCommands);
  const Expression& theSSalgebraNode=input[1];
  const Expression& vectorNode=input[2];
  SemisimpleLieAlgebra* theSSalgebra;
  std::string errorString;
  if (!theCommands.CallConversionFunctionReturnsNonConstUseCarefully
      (Serialization::innerSSLieAlgebra, theSSalgebraNode, theSSalgebra, &errorString))
    return output.SetError(errorString, theCommands);
  Vector<Polynomial<Rational> > theHWfundCoords, theHWsimpleCoords, currentWeight;
  Expression theContext;
  if (!theCommands.GetVectoR(vectorNode, theHWfundCoords, &theContext, theSSalgebra->GetRank(), Serialization::innerPolynomial))
    return output.SetError("Failed to extract highest weight", theCommands);
  WeylGroup& theWeyl=theSSalgebra->theWeyl;
  if (!useFundCoords)
  { theHWsimpleCoords=theHWfundCoords;
    theHWfundCoords=theWeyl.GetFundamentalCoordinatesFromSimple(theHWsimpleCoords);
  } else
    theHWsimpleCoords=theWeyl.GetSimpleCoordinatesFromFundamental(theHWfundCoords);
  std::stringstream out, latexReport;
  Vectors<Polynomial<Rational> > theHWs;
  FormatExpressions theFormat;
  theContext.ContextGetFormatExpressions(theFormat);
//  theFormat.fundamentalWeightLetter="\\psi";
  theHWs.AddOnTop(theHWsimpleCoords);
  HashedList<Vector<Polynomial<Rational> > > outputOrbit;
  WeylGroup orbitGeneratingSet;
  Polynomial<Rational> theExp;
  if (!theSSalgebra->theWeyl.GenerateOrbit(theHWs, useRho, outputOrbit, false, 1921, &orbitGeneratingSet.theElements, 1921))
    out << "Failed to generate the entire orbit (maybe too large?), generated the first " << outputOrbit.size
    << " elements only.";
  else
    out << "The orbit has " << outputOrbit.size << " elements.";
  latexReport
  << "\\begin{longtable}{p{3cm}p{4cm}p{4cm}p{4cm}}Element & Eps. coord. & Image fund. coordinates& "
  << "Hw minus wt. \\\\\n<br>";
  out << "<table><tr> <td>Group element</td> <td>Image in simple coords</td> "
  << "<td>Epsilon coords</td><td>Fundamental coords</td>";
  if (useRho)
    out << "<td>Corresponding b-singular vector candidate</td>";
  out << "</tr>";
  MonomialUniversalEnveloping<Polynomial<Rational> > standardElt;
  LargeInt tempInt;
  bool useMathTag=outputOrbit.size<150;
  Matrix<Rational> epsCoordMat;
  theWeyl.theDynkinType.GetEpsilonMatrix(epsCoordMat);
  for (int i=0; i<outputOrbit.size; i++)
  { theFormat.simpleRootLetter="\\alpha";
    theFormat.fundamentalWeightLetter="\\psi";
    std::string orbitEltString=outputOrbit[i].ToString(&theFormat);
    Vector<Polynomial<Rational> > epsVect=outputOrbit[i];
    epsCoordMat.ActOnVectorColumn(epsVect);
    std::string orbitEltStringEpsilonCoords=epsVect.ToStringLetterFormat("\\varepsilon", &theFormat);
    std::string weightEltString=
    theWeyl.GetFundamentalCoordinatesFromSimple(outputOrbit[i]).ToStringLetterFormat
    (theFormat.fundamentalWeightLetter, &theFormat);
    out << "<tr>" << "<td>"
    << (useMathTag ? CGI::GetHtmlMathSpanPure(orbitGeneratingSet.theElements[i].ToString()) : orbitGeneratingSet.theElements[i].ToString())
    << "</td><td>"
    << (useMathTag ? CGI::GetHtmlMathSpanPure(orbitEltString) : orbitEltString) << "</td><td>"
    << (useMathTag ? CGI::GetHtmlMathSpanPure(orbitEltStringEpsilonCoords) : orbitEltStringEpsilonCoords)
    << "</td><td>"
    << (useMathTag ? CGI::GetHtmlMathSpanPure(weightEltString) : weightEltString)
    << "</td>";
    latexReport << "$" << orbitGeneratingSet.theElements[i].ToString(&theFormat) << "$ & $"
    << orbitEltStringEpsilonCoords
    << "$ & $"
    <<  weightEltString << "$ & $"
    << (outputOrbit[0]-outputOrbit[i]).ToStringLetterFormat(theFormat.simpleRootLetter, &theFormat)
    << "$\\\\\n<br>"
    ;
    if (useRho)
    { currentWeight=theHWsimpleCoords;
      standardElt.MakeConst(*theSSalgebra);
      bool isGood=true;
      for (int j=0; j<orbitGeneratingSet.theElements[i].size; j++)
      { int simpleIndex=orbitGeneratingSet.theElements[i][j];
        theExp=theWeyl.GetScalarProdSimpleRoot(currentWeight, simpleIndex);
        theWeyl.SimpleReflectionRhoModified(simpleIndex, currentWeight);
        theExp*=2;
        theExp/=theWeyl.CartanSymmetric.elements[simpleIndex][simpleIndex];
        theExp+=1;
        if (theExp.IsInteger(&tempInt))
          if (tempInt<0)
          { isGood=false;
            break;
          }
        standardElt.MultiplyByGeneratorPowerOnTheLeft
        (theSSalgebra->GetNumPosRoots() -simpleIndex-1, theExp);
      }
      out << "<td>";
      if (isGood)
        out << CGI::GetHtmlMathSpanPure(standardElt.ToString(&theFormat));
      else
        out << "-";
      out << "</td>";
    }
    out << "</tr>";
  }
  latexReport << "\\end{longtable}";
  out << "</table>" << "<br> " << latexReport.str();
  return output.AssignValue(out.str(), theCommands);
}

bool WeylGroupCalculatorFunctions::innerWeylGroupIrrepsAndCharTable
(CommandList& theCommands, const Expression& input, Expression& output)
{ if (!WeylGroupCalculatorFunctions::innerWeylGroupConjugacyClasses(theCommands, input, output))
    return false;
  if (!output.IsOfType<WeylGroup>())
    return true;
  WeylGroup& theGroup=output.GetValuENonConstUseWithCaution<WeylGroup>();
  theGroup.ComputeIrreducibleRepresentations();
  FormatExpressions tempFormat;
  tempFormat.flagUseLatex=true;
  tempFormat.flagUseHTML=false;
  std::stringstream out;
  out << theGroup.ToString(&tempFormat);
  return output.AssignValue(out.str(), theCommands);
}

bool WeylGroupCalculatorFunctions::innerWeylGroupOrbitFundRho
(CommandList& theCommands, const Expression& input, Expression& output)
{ return WeylGroupCalculatorFunctions::innerWeylOrbit(theCommands, input, output, true, true);
}

bool WeylGroupCalculatorFunctions::innerWeylGroupOrbitFund
(CommandList& theCommands, const Expression& input, Expression& output)
{ return WeylGroupCalculatorFunctions::innerWeylOrbit(theCommands, input, output, true, false);
}

bool WeylGroupCalculatorFunctions::innerWeylGroupOrbitSimple
(CommandList& theCommands, const Expression& input, Expression& output)
{ return WeylGroupCalculatorFunctions::innerWeylOrbit(theCommands, input, output, false, false);
}

bool WeylGroupCalculatorFunctions::innerWeylGroupConjugacyClasses
(CommandList& theCommands, const Expression& input, Expression& output)
{ SemisimpleLieAlgebra* thePointer;
  std::string errorString;
  if (!theCommands.CallConversionFunctionReturnsNonConstUseCarefully
      (Serialization::innerSSLieAlgebra, input, thePointer, &errorString))
    return output.SetError(errorString, theCommands);
  output.AssignValue(thePointer->theWeyl, theCommands);
  WeylGroup& theGroup=output.GetValuENonConstUseWithCaution<WeylGroup>();
  if (theGroup.CartanSymmetric.NumRows>4)
    return output.AssignValue<std::string>
    ("I have been instructed not to do this for Weyl groups of rank greater \
     than 4 because of the size of the computation.", theCommands);

  CoxeterGroup otherGroup;
  otherGroup.MakeFrom(theGroup.CartanSymmetric);
  double timeStart1=theCommands.theGlobalVariableS->GetElapsedSeconds();
  theGroup.ComputeConjugacyClasses();
  std::cout << "Time of conjugacy class computation method1: "
  << theCommands.theGlobalVariableS->GetElapsedSeconds()-timeStart1;
  double timeStart2=theCommands.theGlobalVariableS->GetElapsedSeconds();
  otherGroup.ComputeConjugacyClasses();
  std::cout << "Time of conjugacy class computation method2: "
  << theCommands.theGlobalVariableS->GetElapsedSeconds()-timeStart2;

  return true;
}

bool WeylGroupCalculatorFunctions::innerTensorWeylReps
(CommandList& theCommands, const Expression& input, Expression& output)
{ //std::cout << "Here i am!";
  if (input.children.size!=3)
    return false;
  CoxeterRepresentation<Rational> leftRep;
  CoxeterRepresentation<Rational> rightRep;
  if (!input[1].IsOfType<CoxeterRepresentation<Rational> > (&leftRep))
    return false;
  if (!input[2].IsOfType<CoxeterRepresentation<Rational> > (&rightRep))
    return false;
  FormatExpressions theFormat;
  theFormat.flagUseLatex=true;
  theFormat.flagUseHTML=false;
  //std::cout << "<br>left rep is: " << leftRep.ToString(&theFormat);
  //std::cout << "<br>right rep is: " << rightRep.ToString(&theFormat);
  if (leftRep.G!=rightRep.G)
    return output.SetError
    ("Error: attempting to tensor irreps with different owner groups. ", theCommands);
  leftRep*=rightRep;
  return output.AssignValue(leftRep, theCommands);
}

bool WeylGroupCalculatorFunctions::innerTensorAndDecomposeWeylReps
(CommandList& theCommands, const Expression& input, Expression& output)
{ Expression theTensor;
  if (!WeylGroupCalculatorFunctions::innerTensorAndDecomposeWeylReps(theCommands, input, theTensor))
    return false;
  return WeylGroupCalculatorFunctions::innerDecomposeWeylRep(theCommands, theTensor, output);
}

bool WeylGroupCalculatorFunctions::innerDecomposeWeylRep
(CommandList& theCommands, const Expression& input, Expression& output)
{ CoxeterRepresentation<Rational> theRep;
  if (!input.IsOfType<CoxeterRepresentation<Rational> > (&theRep))
    return false;
  List<ClassFunction<Rational> > theCFs;
  List<CoxeterRepresentation<Rational> > outputReps;
  theRep.Decomposition(theCFs, outputReps);
  output.reset(theCommands, outputReps.size+1);
  output.AddChildAtomOnTop(theCommands.opSequence());
  Expression tempE;
  for (int i=0; i<outputReps.size; i++)
  { tempE.AssignValue(outputReps[i], theCommands);
    output.AddChildOnTop(tempE);
  }
  return true;
}

bool WeylGroupCalculatorFunctions::innerWeylGroupNaturalRep
(CommandList& theCommands, const Expression& input, Expression& output)
{ if (!WeylGroupCalculatorFunctions::innerWeylGroupConjugacyClasses
      (theCommands, input, output))
    return false;
  if (!output.IsOfType<WeylGroup>())
    return false;
  std::cout << "not implemented!";
  assert(false);
  WeylGroup& theGroup=output.GetValuENonConstUseWithCaution<WeylGroup>();
  //std::cout << theGroup.ToString();
  //return output.AssignValue(theGroup.StandardRepresentation(), theCommands);
}

bool WeylGroupCalculatorFunctions::innerCoxeterElement
(CommandList& theCommands, const Expression& input, Expression& output)
{ //if (!input.IsSequenceNElementS(2))
  //return output.SetError("Function Coxeter element takes two arguments.", theCommands);
  if(input.children.size<2){
    return output.SetError
    ("Function CoxeterElement needs to know what group the element belongs to", theCommands);
  }
  //note that if input is list of 2 elements then input[0] is sequence atom, and your two elements are in fact
  //input[1] and input[2];
  SemisimpleLieAlgebra* thePointer;
  std::string errorString;
  if (!theCommands.CallConversionFunctionReturnsNonConstUseCarefully
      (Serialization::innerSSLieAlgebra, input[1], thePointer, &errorString))
    return output.SetError(errorString, theCommands);
  ElementWeylGroup theElt;
  theElt.ReservE(input.children.size-2);
  for(int i=2; i<input.children.size; i++){
    int tmp;
    if (!input[i].IsSmallInteger(& tmp))
      return false;
    theElt.AddOnTop(tmp-1);
  }
  WeylGroup theGroup;
  theGroup=thePointer->theWeyl;
  int indexOfOwnerGroupInObjectContainer=
  theCommands.theObjectContainer.theWeylGroups.AddNoRepetitionOrReturnIndexFirst(theGroup);
  //std::cout << "Group type: " << theGroup.ToString() << "<br>Index in container: "
  //<< indexOfOwnerGroupInObjectContainer;

  theElt.owner=&theCommands.theObjectContainer.theWeylGroups[indexOfOwnerGroupInObjectContainer];
  //std::cout << "<br>theElt.owner: " << theElt.owner;
//  std::cout << "<b>Not implemented!!!!!</b> You requested reflection indexed by " << theReflection;
  for(int i=0; i<theElt.size; i++){
    if (theElt[i] >= thePointer->GetRank() || theElt[i] < 0)
      return output.SetError("Bad reflection index", theCommands);
  }
//  std::cout << "\n" << theGroup.rho << " " << theElt.owner->rho << std::endl;
  theElt.MakeCanonical();
  return output.AssignValue(theElt, theCommands);
}

bool CommandList::innerMinPolyMatrix
(CommandList& theCommands, const Expression& input, Expression& output)
{ if (!theCommands.innerMatrixRational(theCommands, input, output))
    return false;
  Matrix<Rational> theMat;
  if (!output.IsOfType<Matrix<Rational> >(&theMat))
  { theCommands.Comments << "<hr> Successfully called innerMatrixRational onto input " << input.ToString()
    << " to get " << output.ToString()
    << " but the return type was not a matrix of rationals. ";
    return true;
  }
  if (theMat.NumRows!=theMat.NumCols || theMat.NumRows<=0)
    return output.SetError("Error: matrix is not square!", theCommands);
  FormatExpressions tempF;
  tempF.polyAlphabeT.SetSize(1);
  tempF.polyAlphabeT[0]="q";
  UDPolynomial<Rational> theMinPoly;
  theMinPoly.AssignMinPoly(theMat);
  return output.AssignValue(theMinPoly.ToString(&tempF), theCommands);
}

bool CommandList::innerGenerateMultiplicativelyClosedSet
(CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::innerGenerateMultiplicativelyClosedSet");
  if (input.children.size<=2)
    return output.SetError("I need at least two arguments - upper bound and at least one element to multiply.", theCommands);
  int upperLimit;
  if (!input[1].IsSmallInteger(&upperLimit))
    return output.SetError
    ("First argument must be a small integer, serving as upper bound for the set.", theCommands);
  if (upperLimit <=0)
  { upperLimit=10000;
    theCommands.Comments << "The upper computation limit I got was 0 or less; I replaced it with the default value "
    << upperLimit << ".";
  }
  HashedList<Expression> theSet;
  theSet.SetExpectedSize(input.children.size-2);
  for (int i=2; i<input.children.size; i++)
    theSet.AddOnTop(input[i]);
  int numGenerators=theSet.size;
  Expression theProduct, evaluatedProduct;
  BoundVariablesSubstitution tempSub;
  bool tempBool;
  //std::cout << "<br>" << theSet[0].ToString() << "->" << theSet[0].ToStringFull() << " is with hash " << theSet[0].HashFunction();
  for (int i=0; i<theSet.size; i++)
    for (int j=0; j<numGenerators; j++)
    { tempSub.reset();
      theProduct.MakeProducT(theCommands, theSet[j], theSet[i]);
      //std::cout << "<br>Evaluating: " << theProduct.ToString() << "->" << theProduct.ToStringFull();
      theCommands.EvaluateExpression(theProduct, evaluatedProduct, tempSub, tempBool);
      //std::cout << " to get " << evaluatedProduct.ToString() << "->" << evaluatedProduct.ToStringFull();
      //std::cout << " with hash " << evaluatedProduct.HashFunction();
      //if (evaluatedProduct==theSet[0])
      //{ //std::cout << " and equals the first element. ";
      //}
      theSet.AddOnTopNoRepetition(evaluatedProduct);
      if (theSet.size>upperLimit)
      { std::stringstream out;
        out << "<hr>While generating multiplicatively closed set, I went above the upper limit of "
        << upperLimit << " elements.";
        evaluatedProduct.SetError(out.str(), theCommands);
        theSet.AddOnTop(evaluatedProduct);
        i=theSet.size; break;
      }
    }
  theCommands.Comments << "<hr>Generated a list of " << theSet.size << " elements";
  output.reset(theCommands, theSet.size+1);
  output.AddChildAtomOnTop(theCommands.opSequence());
  for (int i=0; i<theSet.size; i++)
    output.AddChildOnTop(theSet[i]);
  return true;

}
