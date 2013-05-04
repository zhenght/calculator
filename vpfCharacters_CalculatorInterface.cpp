#include "vpf.h"
#include "vpfCharacters.h"
#include "vpfGraph.h"
#include "vpfCharacters_CalculatorInterface.h"

static ProjectInformationInstance ProjectInfoVpfCharactersCalculatorInterfaceCPP
(__FILE__, "Weyl group calculator interface");

template <typename coefficient>
void WeylGroupRepresentation<coefficient>::ComputeAllGeneratorImagesFromSimple
( GlobalVariables* theGlobalVariables)
{ this->CheckInitialization();
  this->OwnerGroup->CheckInitializationFDrepComputation();
  HashedList<ElementWeylGroup> ElementsExplored;
  ElementsExplored.SetExpectedSize(this->OwnerGroup->theElements.size);
  this->theElementImages[0].MakeIdMatrix(this->GetDim());
  ElementWeylGroup tempElt;
  int theRank=this->OwnerGroup->GetDim();
  tempElt.owner=this->OwnerGroup;
  ElementsExplored.AddOnTop(tempElt);
  for (int i=0; i<ElementsExplored.size; i++)
    for (int j=0; j<theRank; j++)
    { tempElt=ElementsExplored[i];
      tempElt.AddOnTop(j);
      tempElt.MakeCanonical();
      if (!ElementsExplored.Contains(tempElt))
      { int indexCurrentElt=this->OwnerGroup->theElements.GetIndex(tempElt);
        this->theElementIsComputed[indexCurrentElt]=true;
        this->theElementImages[i].MultiplyOnTheLeft
        (this->theElementImages[j+1], this->theElementImages[indexCurrentElt]);
        ElementsExplored.AddOnTop(tempElt);
      }
    }
}

template <typename coefficient>
void WeylGroupRepresentation<coefficient>::operator*=
(const WeylGroupRepresentation<coefficient>& other)
{ //lazy programmers handling://////
  if (&other==this )
  { WeylGroupRepresentation<coefficient> otherCopy;
    otherCopy=other;
    *this*=otherCopy;
    return;
  }
  //////////////////////////////////
  if (this->OwnerGroup!=other.OwnerGroup)
  { std::cout << "This is a programming error: attempting to multiply representations with "
    << " different owner groups. " << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  WeylGroupRepresentation<coefficient> output;
  output.reset(this->OwnerGroup);
  output.theCharacter=this->theCharacter;
  for (int i=0; i<output.theCharacter.size; i++)
    output.theCharacter[i]*=this->theCharacter[i];
  for(int i=0; i<output.theElementImages.size; i++)
    if (this->theElementIsComputed[i] && other.theElementIsComputed[i])
    { output.theElementImages[i].AssignTensorProduct
      (this->theElementImages[i],other.theElementImages[i]);
      output.theElementIsComputed[i]=true;
    }
  *this=output;
}

template <typename coefficient>
void WeylGroupRepresentation<coefficient>::Restrict
(const Vectors<coefficient>& VectorSpaceBasisSubrep, const Vector<Rational>& remainingCharacter,
 WeylGroupRepresentation<coefficient>& output)
{ int d = this->GetDim();
  Matrix<coefficient> GramMatrixInverted;
  GramMatrixInverted.init(d, d);
  for(int i=0; i<d; i++)
    for(int j=0; j<d; j++)
      GramMatrixInverted.elements[i][j] =
      VectorSpaceBasisSubrep[i].ScalarEuclidean(VectorSpaceBasisSubrep[j]);
  GramMatrixInverted.Invert();
  output.reset(this->OwnerGroup);
  output.theCharacter=remainingCharacter;
  for(int i=0; i<this->theElementImages.size; i++)
    if (this->theElementIsComputed[i])
    { output.theElementIsComputed[i]=true;
      Matrix<coefficient>::MatrixInBasis
      (this->theElementImages[i], output.theElementImages[i], VectorSpaceBasisSubrep,
       GramMatrixInverted);
    }
  for (int i=0; i<this->classFunctionMatrices.size; i++)
    if (this->classFunctionMatricesComputed[i])
    { output.classFunctionMatricesComputed[i]=true;
      Matrix<coefficient>::MatrixInBasis
      (this->classFunctionMatrices[i], output.classFunctionMatrices[i], VectorSpaceBasisSubrep,
       GramMatrixInverted);
    }
}

template <class coefficient>
coefficient WeylGroupRepresentation<coefficient>::GetNumberOfComponents()
{ return this->OwnerGroup->GetCharacterNorm(this->GetCharacter());
}

template <typename coefficient>
void WeylGroupRepresentation<coefficient>::GetClassFunctionMatrix
(Vector<coefficient>& virtualCharacter, Matrix<coefficient>& outputMat, GlobalVariables* theGlobalVariables)
{ this->CheckInitialization();
  this->OwnerGroup->CheckInitializationFDrepComputation();
  outputMat.MakeZeroMatrix(this->GetDim());
  int numClasses=this->OwnerGroup->conjugacyClasses.size;
  for(int cci=0; cci<numClasses; cci++)
  { if(virtualCharacter[cci] == 0)
      continue;
    if (!this->classFunctionMatricesComputed[cci])
    { List<int>& currentConjugacyClass=this->OwnerGroup->conjugacyClasses[cci];
      this->classFunctionMatrices[cci].MakeZeroMatrix(this->GetDim());
      for (int i=0; i<currentConjugacyClass.size; i++)
      { if (!this->theElementIsComputed[currentConjugacyClass[i]])
          this->ComputeAllGeneratorImagesFromSimple(theGlobalVariables);
        this->classFunctionMatrices[cci]+=this->theElementImages[currentConjugacyClass[i]];
      }
      this->classFunctionMatricesComputed[cci]=true;
    }
    for(int j=0; j<outputMat.NumRows; j++)
      for(int k=0; k<outputMat.NumCols; k++)
        outputMat(j,k)+= this->classFunctionMatrices[cci](j,k) * virtualCharacter[cci];
  }
}

template <class coefficient>
bool WeylGroupRepresentation<coefficient>::DecomposeMeIntoIrreps
(Vector<Rational>& outputIrrepMults, GlobalVariables* theGlobalVariables)
{ this->CheckInitialization();
  this->OwnerGroup->CheckInitializationFDrepComputation();
  outputIrrepMults.MakeZero(this->OwnerGroup->conjugacyClasses.size);
  return this->DecomposeMeIntoIrrepsRecursive(outputIrrepMults, theGlobalVariables);
}

template <class CoefficientType>
void Polynomial<CoefficientType>::AssignMinPoly(const Matrix<CoefficientType>& input)
{ if (input.NumCols!=input.NumRows)
  { std::cout << "Programming error: requesting the minimimal polynomial of a non-square matrix. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  int theDim = input.NumCols;
  this->MakeOne(1);
  Vectors<CoefficientType> theBasis;
  Vector<CoefficientType> theVectorPowers;
  Vector<CoefficientType> firstDependentPower;
  Polynomial<CoefficientType> currentFactor;
  MonomialP tempM;
  for(int col = 0; col < theDim; col++)
  { theVectorPowers.MakeEi(theDim,col);
    theBasis.SetSize(0);
    theBasis.AddOnTop(theVectorPowers);
    for(int i=0; i<theDim; i++)
    { input.ActOnVectorColumn(*theBasis.LastObject(), theVectorPowers);
      if(theBasis.LinSpanContainsVector(theVectorPowers))
        break;
      theBasis.AddOnTop(theVectorPowers);
    }
    theVectorPowers.GetCoordsInBasiS(theBasis, firstDependentPower);
    currentFactor.SetExpectedSize(theBasis.size+1);
    currentFactor.MakeZero();
    for(int i=0; i<theBasis.size; i++)
    { tempM.MakeEi(0, i, 1);
      currentFactor.AddMonomial(tempM, -1);
    }
    tempM.MakeEi(0, theBasis.size,1);
    currentFactor.AddMonomial(tempM, 1);
    *this = MathRoutines::lcm(*this, currentFactor);
  }
}

template <class Element>
bool Matrix<Element>::GetEigenspacesProvidedAllAreIntegralWithEigenValueSmallerThanDim
(List<Vectors<Element> >& output)const
{ int theDim = this->NumCols;
  output.SetExpectedSize(theDim);
  output.SetSize(0);
  int found = 0;
  Polynomial<Element> theMinPoly;
  theMinPoly.AssignMinPoly(*this);
  Vector<Element> theEigenValueCandidate;
  theEigenValueCandidate.SetSize(1);
  Matrix<Rational> tempMat;
  for(int ii=0; ii<2*theDim+2; ii++) // lol, this did end up working though
  { int i = ((ii+1)/2) * (2*(ii%2)-1); // 0,1,-1,2,-2,3,-3,...
//    std::cout << "checking " << i << " found " << found << std::endl;
    theEigenValueCandidate[0] = i;
    if(theMinPoly.Evaluate(theEigenValueCandidate) == 0)
    { tempMat = *this;
      output.SetSize(output.size+1);
      tempMat.GetEigenspaceModifyMe(theEigenValueCandidate[0], *output.LastObject());
      found += output.LastObject()->size;
      if(found == theDim)
        return true;
    }
  }
  return false;
}

template <class coefficient>
bool WeylGroupRepresentation<coefficient>::DecomposeMeIntoIrrepsRecursive
(Vector<Rational>& outputIrrepMults, GlobalVariables* theGlobalVariables)
{ this->CheckInitialization();
  this->OwnerGroup->CheckInitializationFDrepComputation();
  if(this->GetNumberOfComponents() == 1)
  { int indexMe=this->OwnerGroup->irreps.GetIndex(*this);
    if(indexMe== -1)
    { this->OwnerGroup->irreps.AddOnTop(*this);
      indexMe=this->OwnerGroup->irreps.size-1;
    }
    outputIrrepMults[indexMe]+=1;
    return true;
  }
  Matrix<coefficient> splittingOperatorMatrix;
  Vectors<coefficient> splittingMatrixKernel, remainingVectorSpace, tempSpace;
  Vector<coefficient> remainingCharacter;
  remainingCharacter=this->theCharacter;
  remainingVectorSpace.MakeEiBasis(this->GetDim());
  //chop off already known pieces:
  for(int i=0; i<this->OwnerGroup->irreps.size; i++)
  { coefficient NumIrrepsOfType=this->OwnerGroup->GetHermitianProduct
       (this->theCharacter, this->OwnerGroup->irreps[i].theCharacter);
    if(NumIrrepsOfType!=0)
    { //std::cout << "contains irrep " << i << std::endl;
      this->GetClassFunctionMatrix(this->OwnerGroup->irreps[i].theCharacter, splittingOperatorMatrix);
      splittingOperatorMatrix.GetZeroEigenSpaceModifyMe(splittingMatrixKernel);
      remainingVectorSpace.IntersectTwoLinSpaces(splittingMatrixKernel, remainingVectorSpace, tempSpace);
      remainingVectorSpace=tempSpace;
      outputIrrepMults[i]+=NumIrrepsOfType;
      remainingCharacter-=this->OwnerGroup->irreps[i].theCharacter*NumIrrepsOfType;
    }
  }
  if((remainingVectorSpace.size < this->GetDim()) && (remainingVectorSpace.size > 0))
  { std::cout << "<br>restricting to subrep(s)... ";
    WeylGroupRepresentation<coefficient> reducedRep;
    this->Restrict(remainingVectorSpace, remainingCharacter, reducedRep);
    std::cout << "done" << std::endl;
    std::cout << "Decomposing remaining subrep(s) " << reducedRep.GetCharacter() << std::endl;
    return reducedRep.DecomposeMeIntoIrrepsRecursive(outputIrrepMults, theGlobalVariables);
  }
  if(remainingVectorSpace.size == 0)
    return true;
  int NumClasses=this->OwnerGroup->conjugacyClasses.size;
  Vector<coefficient> virtualChar;
  List<Vectors<coefficient> > theSubRepsBasis;
  for(int cfi=0; cfi<NumClasses; cfi++)
  { virtualChar.MakeZero(NumClasses);
    virtualChar[cfi] = 1;
    std::cout << "<br>getting matrix of virtual char " << virtualChar << std::endl;
    this->GetClassFunctionMatrix(virtualChar, splittingOperatorMatrix, theGlobalVariables);
    bool tempB=
    splittingOperatorMatrix.GetEigenspacesProvidedAllAreIntegralWithEigenValueSmallerThanDim
    (theSubRepsBasis);
    if (!tempB)
    { std::cout << "<br>This is a mathematical or programming mistake: "
      << "splittingOperatorMatrix should have small integral values, which it doesn't!"
      << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
      assert(false);
    }
    std::cout << "<br>the eigenspaces were ";
    for(int i=0; i<theSubRepsBasis.size; i++)
      std::cout << theSubRepsBasis[i].size << " ";
    std::cout << std::endl;
  }
  WeylGroupRepresentation<coefficient> newRep;
  for(int i=0; i<theSubRepsBasis.size; i++)
  { this->Restrict(theSubRepsBasis[i], remainingCharacter, newRep);
    newRep.theCharacter.SetSize(0);
    if (this->OwnerGroup->GetCharacterNorm(newRep.GetCharacter())>1)
    { std::cout << "<hr><b>Found reducible isotypic component! Should this happen?</b><hr>";
      return false;
    }
    int theIndex =this->OwnerGroup->irreps.AddNoRepetitionOrReturnIndexFirst(newRep);
    outputIrrepMults[theIndex]+=1;
  }
  return true;
}

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
  if (theGroup.CartanSymmetric.NumRows>6)
    return output.AssignValue<std::string>
    ("I have been instructed not to do this for Weyl groups of rank greater \
     than 6 because of the size of the computation.", theCommands);

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
  ElementWeylGroup tempTestElt;
  for (int i=0; i<theGroup.theElements.size; i++)
  { tempTestElt=theGroup.theElements[i];
    tempTestElt.MakeCanonical();
  }
  return true;
}

bool WeylGroupCalculatorFunctions::innerTensorWeylReps
(CommandList& theCommands, const Expression& input, Expression& output)
{ //std::cout << "Here i am!";
  if (input.children.size!=3)
    return false;
  WeylGroupRepresentation<Rational> leftRep;
  WeylGroupRepresentation<Rational> rightRep;
  if (!input[1].IsOfType<WeylGroupRepresentation<Rational> > (&leftRep))
    return false;
  if (!input[2].IsOfType<WeylGroupRepresentation<Rational> > (&rightRep))
    return false;
  FormatExpressions theFormat;
  theFormat.flagUseLatex=true;
  theFormat.flagUseHTML=false;
  //std::cout << "<br>left rep is: " << leftRep.ToString(&theFormat);
  //std::cout << "<br>right rep is: " << rightRep.ToString(&theFormat);
  if (leftRep.OwnerGroup!=rightRep.OwnerGroup)
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
{ if (!input.IsOfType<WeylGroupRepresentation<Rational> > ())
    return false;
//  theRep.Decomposition(theCFs, outputReps);
  WeylGroupRepresentation<Rational>& theRep =
  input.GetValuENonConstUseWithCaution<WeylGroupRepresentation<Rational> >();
  Vector<Rational> theChar;
  theRep.DecomposeMeIntoIrreps(theChar, theCommands.theGlobalVariableS);
  output.reset(theCommands, theChar.GetNumNonZeroCoords()+1);
  output.AddChildAtomOnTop(theCommands.opSequence());
  Expression tempE;
//  std::srtingstream out;
  for (int i=0; i<theChar.size; i++)
    if (theChar[i]!=0)
    { tempE.AssignValue(theRep.OwnerGroup->irreps[i], theCommands);
      output.AddChildOnTop(tempE);
    }
  theCommands.Comments << "<hr>Rep as a linear combination of irreps: " << theChar.ToString();
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
  WeylGroup& theGroup=output.GetValuENonConstUseWithCaution<WeylGroup>();
  std::cout << theGroup.ToString();
  WeylGroupRepresentation<Rational> tempRep;
  theGroup.StandardRepresentation(tempRep);
  return output.AssignValue(tempRep, theCommands);
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
  ProgressReport theReport(theCommands.theGlobalVariableS);
  for (int i=0; i<theSet.size; i++)
    for (int j=0; j<numGenerators; j++)
    { tempSub.reset();
      theProduct.MakeProducT(theCommands, theSet[j], theSet[i]);
      std::stringstream reportStream;
      reportStream << "found " << theSet.size << "elements so far, exploring element " << i+1;
      reportStream << "<br>Evaluating: " << theProduct.ToString();
      theReport.Report(reportStream.str());
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
