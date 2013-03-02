#include "vpfHeader2.h"
static ProjectInformationInstance ProjectInfoVpf5_1cpp
(__FILE__, "C++ object to calculator expression serialization/deserialization.");

template <>
bool Serialization::SerializeMon<ChevalleyGenerator>
(CommandList& theCommands, const ChevalleyGenerator& input,
 Expression& output, const Expression& theContext, bool& isNonConst)
{ isNonConst=true;
  Expression theTypeE, genIndexE;
  SemisimpleLieAlgebra& owner=*input.owneR;
  if (!Serialization::SerializeMonCollection
      (theCommands, owner.theWeyl.theDynkinType, theTypeE, theContext))
    return false;
  if (input.theGeneratorIndex>=owner.GetNumPosRoots() &&
      input.theGeneratorIndex< owner.GetNumPosRoots()+owner.GetRank())
  { output.MakeSerialization("getCartanGenerator", theCommands, 1);
    genIndexE.AssignValue(owner.GetDisplayIndexFromGenerator(input.theGeneratorIndex), theCommands);
  } else
  { output.MakeSerialization("getChevalleyGenerator", theCommands, 1);
    genIndexE.AssignValue(owner.GetDisplayIndexFromGenerator(input.theGeneratorIndex), theCommands);
  }
  output.AddChildOnTop(theTypeE);
  return output.AddChildOnTop(genIndexE);
}

template <>
bool Serialization::DeSerializeMonGetContext<ChevalleyGenerator>
(CommandList& theCommands, const Expression& input, Expression& outputContext)
{ if (!input.IsListNElements(4))
  { theCommands.Comments
    << "<hr>Failed to get ChevalleyGenerator context: "
    << "input is not a sequence of 4 elements, instead it has "
    << input.children.size << " elements, i.e., is " << input.ToString() << "</hr>";
    return false;
  }
  DynkinType theType;
  if (!Serialization::DeSerializeMonCollection(theCommands, input[2], theType))
  { theCommands.Comments
    << "<hr>Failed to load dynkin type from "
    << input[2].ToString() << ".";
    return false;
  }
  SemisimpleLieAlgebra tempAlgebra;
  tempAlgebra.theWeyl.MakeFromDynkinType(theType);
  bool feelsLikeTheVeryFirstTime=
  theCommands.theObjectContainer.theLieAlgebras.Contains(tempAlgebra);
  int algebraIdentifier=
  theCommands.theObjectContainer.theLieAlgebras.AddNoRepetitionOrReturnIndexFirst(tempAlgebra);
  if (feelsLikeTheVeryFirstTime)
  { theCommands.theObjectContainer.theLieAlgebras[algebraIdentifier].ComputeChevalleyConstantS
    (theCommands.theGlobalVariableS);
  }
  outputContext.ContextMakeContextSSLieAlgebrA(algebraIdentifier, theCommands);
  return true;
}

template <>
bool Serialization::DeSerializeMonGetContext<DynkinSimpleType>
(CommandList& theCommands, const Expression& input, Expression& outputContext)
{ outputContext.MakeEmptyContext(theCommands);
  return true;
}

template <>
bool Serialization::DeSerializeMon
(CommandList& theCommands, const Expression& input, const Expression& inputContext,
 ChevalleyGenerator& outputMon)
{ int AlgIndex=inputContext.ContextGetIndexAmbientSSalg();
  if (AlgIndex==-1)
  { theCommands.Comments << "<hr>Can't load Chevalley generator: "
    << " failed extract ambient algebra index from context " << inputContext.ToString();
    return false;
  }
  if (!input.IsListNElements(4))
  { theCommands.Comments << "<hr>Can't load Chevalley generator: "
    << " input is not a list of 4 elements, instead it has " << input.children.size
    << " elements, i.e., is " << input.ToString();
    return false;
  }
  const Expression& generatorIndexE=input[3];
  int generatorIndex;
  if (!generatorIndexE.IsSmallInteger(&generatorIndex))
    return false;
  std::string theOperation;
  if (!input[1].IsOperation(&theOperation))
  { theCommands.Comments
    << "<hr>Can't load Chevalley generator: second argument is not an operation, instead it is "
    << input[1].ToString();
    return false;
  }
  outputMon.owneR=&theCommands.theObjectContainer.theLieAlgebras[AlgIndex];
//  std::cout << "<hr>owner rank, owner num gens: " << outputMon.owneR->GetRank() << ", "
//  << outputMon.owneR->GetNumGenerators();
  if (theOperation=="getCartanGenerator")
    generatorIndex+=outputMon.owneR->GetNumPosRoots();
  else if (theOperation=="getChevalleyGenerator")
    generatorIndex=outputMon.owneR->GetGeneratorFromDisplayIndex(generatorIndex);
  else
  { theCommands.Comments << "<hr>Failed to load Chevalley generator: the generator name was  "
    << theOperation << "; it must either be getCartanGenerator or getChevalleyGenerator.";
    return false;
  }
  if (generatorIndex<0 || generatorIndex>=outputMon.owneR->GetNumGenerators())
  { theCommands.Comments << "<hr>Failed to load Chevalley generator: final generator index is "
    << generatorIndex << ". ";
    return false;
  }
  outputMon.theGeneratorIndex=generatorIndex;
  return true;
}

template <>
bool Serialization::SerializeMon<MonomialUniversalEnveloping<RationalFunctionOld> >
(CommandList& theCommands, const MonomialUniversalEnveloping<RationalFunctionOld>& input,
 Expression& output, const Expression& theContext, bool& isNonConst)
{ output.reset(theCommands);
  if (input.IsEqualToOne())
  { isNonConst=false;
    return true;
  }
  ChevalleyGenerator currentGen;
  currentGen.owneR=input.owneR;
  Expression baseE, exponentE, nextTermE;
  currentGen.theGeneratorIndex=input.generatorsIndices[0];
  bool tempNonConst;
  if (!Serialization::SerializeMon
      (theCommands, currentGen, baseE, theContext, tempNonConst))
  { theCommands.Comments << "<hr>Failed to store " << currentGen.ToString() << ". ";
    return false;
  }
  if (!input.Powers[0].IsEqualToOne())
  { if (!Serialization::innerStoreObject
        (theCommands, input.Powers[0], exponentE, theContext))
    { theCommands.Comments << "<hr>Failed to store the exponent " << input.Powers[0].ToString()
      << " with context " << theContext.ToString();
      return false;
    }
    output.MakeXOX(theCommands, theCommands.opThePower(), baseE, exponentE);
  } else
    output=baseE;
  for (int i=1; i<input.generatorsIndices.size; i++)
  { currentGen.theGeneratorIndex=input.generatorsIndices[i];
    if (!Serialization::SerializeMon
        (theCommands, currentGen, baseE, theContext, tempNonConst))
    { theCommands.Comments << "<hr>Failed to store " << currentGen.ToString() << ". ";
      return false;
    }
    if (!input.Powers[0].IsEqualToOne())
    { if (!Serialization::innerStoreObject
          (theCommands, input.Powers[0], exponentE, theContext))
      { theCommands.Comments << "<hr>Failed to store the exponent " << input.Powers[0].ToString()
        << " with context " << theContext.ToString();
        return false;
      }
      nextTermE.MakeXOX(theCommands, theCommands.opThePower(), baseE, exponentE);
    } else
      nextTermE=baseE;
    output.MakeXOX(theCommands, theCommands.opTimes(), output, nextTermE);
  }
  return true;
}

template <>
bool Serialization::SerializeMon<MonomialP>
(CommandList& theCommands, const MonomialP& input,
  Expression& output, const Expression& theContext, bool& isNonConst)
{ MacroRegisterFunctionWithName("Serialization::SerializeMon_MonomialP");
  Expression exponentE, monE, tempE, letterE;
  isNonConst=false;
  output.reset(theCommands);
  for (int j=input.GetMinNumVars()-1; j>=0; j--)
    if (input(j)!=0)
    { letterE=theContext.ContextGetContextVariable(j);
      if (input(j)==1)
        monE=letterE;
      else
      { exponentE.AssignValue(input(j), theCommands);
        monE.MakeXOX(theCommands, theCommands.opThePower(), letterE, exponentE);
      }
      if (!isNonConst)
        output=monE;
      else
      { tempE=output;
        output.MakeXOX(theCommands, theCommands.opTimes(), monE, tempE);
      }
      isNonConst=true;
    }
  return true;
}

template <>
bool Serialization::DeSerializeMon<DynkinSimpleType>
(CommandList& theCommands, const Expression& input, const Expression& inputContext,
 DynkinSimpleType& outputMon)
{ MacroRegisterFunctionWithName("Serialization::DeSerializeMon_DynkinSimpleType");
  if (input.children.size!=2)
  { theCommands.Comments
    << "<hr>Error while extracting Dynkin simple type: the monomial "
    << input.ToString() << " appears not to be a Dynkin simple type<hr>";
    return false;
  }
  Expression rankE=input[1];
  Expression typeLetter=input[0];
  Rational firstCoRootSquaredLength=2;
  bool foundLengthFromExpression=false;
  if (typeLetter.IsListStartingWithAtom(theCommands.opThePower()))
  { if (!typeLetter[2].IsOfType<Rational>(&firstCoRootSquaredLength))
    { theCommands.Comments << "<hr>Couldn't extract first co-root length from "
      << input.ToString() << "<hr>";
      return false;
    }
    if (firstCoRootSquaredLength<=0)
    { theCommands.Comments
      << "Couldn't extract positive rational first co-root length from "
      << input.ToString();
      return false;
    }
    typeLetter.AssignMeMyChild(1);
    foundLengthFromExpression=true;
  }
  std::string theTypeName;
  if (!typeLetter.IsOperation(&theTypeName))
  { theCommands.Comments << "I couldn't extract a type letter from "
    << input.ToString();
    return false;
  }
  if (theTypeName.size()!=1)
  { theCommands.Comments << "<hr>Error while extracting Dynkin simple type:"
    << "The type of a simple Lie algebra must be the letter A, B, C, D, E, F or G."
    << "Instead, it is " << theTypeName + ". Error encountered while processing "
    << input.ToString();
    return false;
  }
  char theWeylLetter=theTypeName[0];
  if (theWeylLetter=='a') theWeylLetter='A';
  if (theWeylLetter=='b') theWeylLetter='B';
  if (theWeylLetter=='c') theWeylLetter='C';
  if (theWeylLetter=='d') theWeylLetter='D';
  if (theWeylLetter=='e') theWeylLetter='E';
  if (theWeylLetter=='f') theWeylLetter='F';
  if (theWeylLetter=='g') theWeylLetter='G';
  if (!(theWeylLetter=='A' || theWeylLetter=='B' || theWeylLetter=='C' ||
        theWeylLetter=='D' || theWeylLetter=='E' || theWeylLetter=='F' || theWeylLetter=='G'))
  { theCommands.Comments << "The type of a simple Lie algebra must be the letter A, B, C, D, E, F or G; "
    << "error while processing " << input.ToString();
    return false;
  }
  int theRank;
  if (!rankE.IsSmallInteger(&theRank))
  { theCommands.Comments << "I wasn't able to extract rank from "
    << input.ToString();
    return false;
  }
  if (theRank<1 || theRank>20)
  { theCommands.Comments << "<hr>The rank of a simple Lie algebra must be between 1 and 20; error while processing "
    << input.ToString();
    return false;
  }
  if (theWeylLetter=='E' &&(theRank>8 || theRank<3))
  { theCommands.Comments << "<hr>Type E must have rank 6,7 or 8 ";
    return false;
  }
  outputMon.theLetter=theWeylLetter;
  outputMon.theRank= theRank;
  if (!foundLengthFromExpression)
    if (theWeylLetter=='F')
      firstCoRootSquaredLength=1;
  outputMon.lengthFirstCoRootSquared= firstCoRootSquaredLength;
  return true;
}

template <>
bool Serialization::SerializeMon<DynkinSimpleType>
(CommandList& theCommands, const DynkinSimpleType& input, Expression& output,
 const Expression& theContext, bool& isNonConst)
{ MacroRegisterFunctionWithName("Serialization::DynkinSimpleType");
  Expression letterE, rankE, letterAndIndexE, indexE;
  std::string letterS;
  letterS=input.theLetter;
  letterE.MakeAtom(theCommands.AddOperationNoRepetitionOrReturnIndexFirst(letterS), theCommands);
  indexE.AssignValue(input.lengthFirstCoRootSquared, theCommands);
  rankE.AssignValue(input.theRank, theCommands);
  letterAndIndexE.MakeXOX(theCommands, theCommands.opThePower(), letterE, indexE);
  output.reset(theCommands);
  output.format=output.formatFunctionUseUnderscore;
  output.AddChildOnTop(letterAndIndexE);
  output.AddChildOnTop(rankE);
//  std::cout << "output: " << output.ToString();
  return true;
}

template <class dataType>
bool CommandList::innerExtractPMTDtreeContext
(CommandList& theCommands, const Expression& input, Expression& output)
{ RecursionDepthCounter theRecursionCounter(&theCommands.RecursionDeptH);
  MacroRegisterFunctionWithName("CommandList::innerExtractPMTDtreeContext");
  if (theCommands.RecursionDeptH>theCommands.MaxRecursionDeptH)
  { std::stringstream out;
    out << "Max recursion depth of " << theCommands.MaxRecursionDeptH
    << " exceeded while trying to evaluate to polynomial an expression "
    << "(i.e., your polynomial expression is too large).";
    return output.SetError(out.str(), theCommands);
  }
//  std::cout << "<br>Extracting context from: " << input.ToString();
  if (input.IsListStartingWithAtom(theCommands.opTimes()) ||
      input.IsListStartingWithAtom(theCommands. opPlus()) ||
      input.IsListStartingWithAtom(theCommands.opMinus()) )
  { output.reset(theCommands, 1);
    output.AssignChildAtomValue(0, theCommands.opContext(), theCommands);
    Expression newContext, intermediateContext;
    for (int i=1; i<input.children.size; i++)
      if (theCommands.innerExtractPMTDtreeContext<dataType>(theCommands, input[i], newContext))
      { if (!Expression::ContextMergeContexts(newContext, output, intermediateContext))
          return false;
        output=intermediateContext;
      } else
        return false;
    return true;
  }
  int thePower;
  if (input.IsListNElementsStartingWithAtom(theCommands.opThePower(), 3))
    if (input[2].IsSmallInteger(&thePower))
      return theCommands.innerExtractPMTDtreeContext<dataType>(theCommands, input[1], output);
  if(input.IsAtoM(theCommands.opContext()))
  { theCommands.Comments << "Error in context extraction: encountered context keyword. ";
    return false;
  }
  Expression theContext;
  input.GetContextForConversionIgnoreMyContext<dataType>(theContext);
  return Expression::ContextMergeContexts(theContext, input.GetContext(), output);
}

template <class dataType>
bool CommandList::outerExtractAndEvaluatePMTDtree
(CommandList& theCommands, const Expression& input, Expression& output)
{ Expression contextE;
  if (!theCommands.innerExtractPMTDtreeContext<dataType>(theCommands, input, contextE))
    return false;
//  std::cout << "<br>Extracted context from " << input.ToString() << ": "
//  << contextE.ToString();
  return theCommands.EvaluatePMTDtree<dataType>(contextE, input, output);
}

template <class dataType>
bool CommandList::EvaluatePMTDtree
(const Expression& inputContext, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::EvaluatePMTDtree");
  RecursionDepthCounter theRecursionCounter(&this->RecursionDeptH);
  if (this->RecursionDeptH>this->MaxRecursionDeptH)
  { this->Comments << "Max recursion depth of " << this->MaxRecursionDeptH
    << " exceeded while trying to evaluate polynomial expression "
    << "(i.e. your polynomial expression is too large).";
    return false;
  }
  dataType outputData;
//  std::cout << "<br>Context:" <<  inputContext.ToString()
//  << " Evaluating PMTD tree of " << input.ToString();
  if (input.IsListStartingWithAtom(this->opTimes()) ||
      input.IsListStartingWithAtom(this-> opPlus()) )
  { for (int i=1; i<input.children.size; i++)
    { if (!this->EvaluatePMTDtree<dataType>(inputContext, input[i], output))
        return false;
//      std::cout << "<br>Evaluated " << input[i].ToString() << " to " << output.ToString();
      if (input[0].IsAtoM(this->opTimes()))
      { if (i==1)
          outputData=output.GetValuE<dataType>();
        else
        { //std::cout << "<hr>Multiplying: " << outputBuffer.ToString(&this->theGlobalVariableS->theDefaultLieFormat)
          //<< " and " << bufferData.ToString(&this->theGlobalVariableS->theDefaultLieFormat);
          outputData*=output.GetValuE<dataType>();
          //std::cout << "<br>Result: " << outputBuffer.ToString(&this->theGlobalVariableS->theDefaultLieFormat) << "<br>";
        }
      } else if (input[0].IsAtoM(this->opPlus()))
      { //std::cout << "<hr>Status outputBuffer data before addition: " << outputBuffer.ToString(this->theGlobalVariableS->theDefaultLieFormat);
        if (i==1)
        { outputData=output.GetValuE<dataType>();
//          std::cout << "<hr> outputBuffer has been set to: " << outputBuffer.ToString(&this->theGlobalVariableS->theDefaultLieFormat)
//          << ", which should equal the bufferData: " << bufferData.ToString(&this->theGlobalVariableS->theDefaultLieFormat);
        } else
        { //std::cout << "<hr>Adding: " << outputBuffer.ToString(&this->theGlobalVariableS->theDefaultLieFormat)
          //<< " and " << bufferData.ToString(&this->theGlobalVariableS->theDefaultLieFormat);
          outputData+=output.GetValuE<dataType>();
          //std::cout << "<hr>Result: " << outputBuffer.ToString(&this->theGlobalVariableS->theDefaultLieFormat) << "<br>";
        }
      }
    }
    return output.AssignValueWithContext(outputData, inputContext, *this);
  }
  int thePower;
  if (input.IsListNElementsStartingWithAtom(this->opThePower(), 3))
    if (input[2].IsSmallInteger(&thePower))
    { if(!this->EvaluatePMTDtree<dataType>(inputContext, input[1], output))
        return false;
      outputData=output.GetValuE<dataType>();
      outputData.RaiseToPower(thePower);
      return output.AssignValueWithContext(outputData, inputContext, *this);
    }
  Expression intermediate=input;
  Expression tempContext=inputContext;
  if (!intermediate.SetContextAtLeastEqualTo(tempContext))
  { this->Comments << "Failed to set context " << tempContext.ToString()
    << " onto expression " << intermediate.ToString() << "."
    << " <b>This could be a programmSetContextAtLeastEqualToing error.</b> ";
    return false;
  }
  if (!intermediate.ConvertToType<dataType>(output))
  { this->Comments << "Failed to convert " << intermediate.ToString()
    << " to the desired type. ";
    return false;
  }
  return true;
}

bool CommandList::innerSSLieAlgebra
(CommandList& theCommands, const Expression& input, Expression& output)
{ RecursionDepthCounter recursionCounter(&theCommands.RecursionDeptH);
  MacroRegisterFunctionWithName("CommandList::innerSSLieAlgebra");
  //std::cout << "<br>Now I'm here!";
  if (!Serialization::innerPolynomial(theCommands, input, output))
    return output.SetError
    ("Failed to extract the semismiple Lie algebra type from " + input.ToString(), theCommands);
  if (output.IsError())
    return true;
  Polynomial<Rational> theType=output.GetValuE<Polynomial<Rational> >();
  Expression theContext=output.GetContext();
  FormatExpressions theFormat;
  theContext.ContextGetFormatExpressions(theFormat);
  SemisimpleLieAlgebra tempSSalgebra;
  DynkinType theDynkinType;
  DynkinSimpleType simpleComponent;
  theDynkinType.MakeZero();
  for (int i=0; i<theType.size; i++)
  { MonomialP& currentMon=theType[i];
    int variableIndex;
    if (!currentMon.IsOneLetterFirstDegree(&variableIndex))
      return output.SetError
      ("Failed to extract type from monomial "+ currentMon.ToString(&theFormat), theCommands);
    Expression typEE= theContext.ContextGetContextVariable(variableIndex);
    if (!Serialization::DeSerializeMon(theCommands, typEE, theContext, simpleComponent))
      return false;
    int theMultiplicity=-1;
    if (!theType.theCoeffs[i].IsSmallInteger(&theMultiplicity))
      theMultiplicity=-1;
    if (theMultiplicity<0)
    { std::stringstream out;
      out << "I failed to convert the coefficient " << theType.theCoeffs[i]
      << " of " << currentMon.ToString(&theFormat) << " to a small integer";
      return output.SetError(out.str(), theCommands);
    }
    theDynkinType.AddMonomial(simpleComponent, theMultiplicity);
    if (theDynkinType.GetRank()>20)
    { std::stringstream out;
      out << "I have been instructed to allow semisimple Lie algebras of rank 20 maximum. "
      << " If you would like to relax this limitation edit file " << __FILE__ << " line "
      << __LINE__ << ". Note that the Chevalley constant computation reserves a dim(g)*dim(g)"
      << " table of RAM memory, which means the RAM memory rises with the 4^th power of dim(g). "
      << " You have been warned. "
      << " Alternatively, you may want to implement a sparse structure constant table "
      << "(write me an email if you want to do that, I will help you). ";
      return output.SetError(out.str(), theCommands);
    }
  }
  tempSSalgebra.theWeyl.MakeFromDynkinType(theDynkinType);
  int indexInOwner=theCommands.theObjectContainer.theLieAlgebras.GetIndex(tempSSalgebra);
  bool feelsLikeTheVeryFirstTime=(indexInOwner==-1);
  if (feelsLikeTheVeryFirstTime)
  { indexInOwner=theCommands.theObjectContainer.theLieAlgebras.size;
    theCommands.theObjectContainer.theLieAlgebras.AddOnTop(tempSSalgebra);
  }
  SemisimpleLieAlgebra& theSSalgebra=theCommands.theObjectContainer.theLieAlgebras[indexInOwner];
  output.AssignValue(theSSalgebra, theCommands);
  if (feelsLikeTheVeryFirstTime)
  { theSSalgebra.ComputeChevalleyConstantS(theCommands.theGlobalVariableS);
    Expression tempE;
    theCommands.innerPrintSSLieAlgebra(theCommands, output, tempE, false);
    theCommands.Comments << tempE.GetValuE<std::string>();
  }
  //theSSalgebra.TestForConsistency(*theCommands.theGlobalVariableS);
  return true;
}

bool Serialization::innerStoreSemisimpleLieAlgebra
  (CommandList& theCommands, const Expression& input, Expression& output)
{ if (!input.IsOfType<SemisimpleLieAlgebra>())
    return output.SetError
    ("Asking serialization of non-semisimple Lie algebra to semisimple Lie algebra not allowed. ",
     theCommands);
  SemisimpleLieAlgebra& owner=input.GetValuENonConstUseWithCaution<SemisimpleLieAlgebra>();
  return Serialization::innerStoreObject(theCommands, owner, output);
}

void Expression::MakeSerialization
(const std::string& secondEntry, CommandList& theCommands, int numElementsToReserve)
{ this->reset(theCommands);
  this->children.ReservE(numElementsToReserve+2);
  Expression tempE;
  tempE.MakeAtom(theCommands.opSerialization(), theCommands);
  this->AddChildOnTop(tempE);
  tempE.MakeAtom(theCommands.AddOperationNoRepetitionOrReturnIndexFirst(secondEntry), theCommands);
  this->AddChildOnTop(tempE);
}

bool Serialization::innerStoreObject
(CommandList& theCommands, const ElementSemisimpleLieAlgebra<Rational>& input, Expression& output)
{ MacroRegisterFunctionWithName("Serialization::innerStoreObject");
  Expression tempContext;
  tempContext.MakeEmptyContext(theCommands);
  if (!input.IsEqualToZero())
  { tempContext.ContextMakeContextSSLieAlgebrA
    (theCommands.theObjectContainer.theLieAlgebras.AddNoRepetitionOrReturnIndexFirst
    (*input.GetOwner()), theCommands);
  }
  Serialization::SerializeMonCollection(theCommands, input, output, tempContext);
  return true;
}

bool Serialization::innerStoreObject
(CommandList& theCommands, const slTwoSubalgebra& input, Expression& output)
{ MacroRegisterFunctionWithName("Serialization::innerStoreObject");
  output.MakeSerialization("LoadSltwoSubalgebra", theCommands);
  Expression tempE;
  Serialization::innerStoreObject(theCommands, input.theF, tempE);
  output.AddChildOnTop(tempE);
  Serialization::innerStoreObject(theCommands, input.theE, tempE);
  output.AddChildOnTop(tempE);

  /*List<int> highestWeights;
  List<int> multiplicitiesHighestWeights;
  List<int> weightSpaceDimensions;
  ElementSemisimpleLieAlgebra<Rational> theH, theE, theF;
  ElementSemisimpleLieAlgebra<Rational> bufferHbracketE, bufferHbracketF, bufferEbracketF;
  SemisimpleLieAlgebra* owneR;
  SltwoSubalgebras* container;
  Rational LengthHsquared;
  int indexInContainer;
  List<int> IndicesContainingRootSAs;
  List<int> IndicesMinimalContainingRootSA;
  List<List<int> > HighestWeightsDecompositionMinimalContainingRootSA;
  List<List<int> > MultiplicitiesDecompositionMinimalContainingRootSA;
  Vectors<Rational> preferredAmbientSimpleBasis;
  Vector<Rational> hCharacteristic;
  Vector<Rational> hElementCorrespondingToCharacteristic;
  Vectors<Rational> hCommutingRootSpaces;
  Vectors<Rational> RootsWithScalar2WithH;
  DynkinDiagramRootSubalgebra DiagramM;
  DynkinDiagramRootSubalgebra CentralizerDiagram;
  PolynomialSubstitution<Rational> theSystemToBeSolved;
  Matrix<Rational> theSystemMatrixForm;
  Matrix<Rational> theSystemColumnVector;
  bool DifferenceTwoHsimpleRootsIsARoot;*/
  return true;
}

bool Serialization::innerLoadFromObject
(CommandList& theCommands, const Expression& input, slTwoSubalgebra& output)
{ MacroRegisterFunctionWithName("Serialization::innerLoadFromObject");
  if (!input.IsListNElements(3))
  { theCommands.Comments << "<hr>input of innerLoadFromObject has "
    << input.children.size << " children, 3 expected. ";
    return false;
  }
  const Expression& theF=input[1];
  const Expression& theE=input[2];
  ElementSemisimpleLieAlgebra<Rational> eltF, eltE;
  if (!Serialization::DeSerializeMonCollection(theCommands, theF, eltF))
  { theCommands.Comments
    << "<hr>Failed to extract f element while loading sl(2) subalgebra<hr>";
    return false;
  }
  if (!Serialization::DeSerializeMonCollection(theCommands, theE, eltE))
  { theCommands.Comments
    << "<hr>Failed to extract e element while loading sl(2) subalgebra<hr>";
    return false;
  }
  if (eltE.IsEqualToZero() || eltF.IsEqualToZero())
  { theCommands.Comments << "<hr>Failed to load sl(2) subalgebra: either e or f is equal to zero. "
    << "e and f are: " << eltE.ToString() << ", " << eltF.ToString() << ". ";
    return false;
  }
  if (eltE.GetOwner()!=eltF.GetOwner())
  { theCommands.Comments
    << "<hr>Failed to load sl(2): E and F element of sl(2) have different owners."
    << " More precisely, the owner of e is " << eltE.GetOwner()->ToString() << " and the owner of f is "
    << eltF.GetOwner()->ToString();
    return false;
  }
  output.theE=eltE;
  output.theF=eltF;
  output.owneR=eltE.GetOwner();
  return true;
}

bool Serialization::innerLoadSltwoSubalgebra
(CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("Serialization::innerLoadSltwoSubalgebra");
  slTwoSubalgebra tempSL2;
  if (!Serialization::innerLoadFromObject(theCommands, input, tempSL2))
  { theCommands.Comments << "<hr>Failed to load sl(2) subalgebra. ";
    return false;
  }
  return output.AssignValue(tempSL2.ToString(), theCommands);
}

bool Serialization::innerStoreObject
(CommandList& theCommands, const SltwoSubalgebras& input, Expression& output)
{ MacroRegisterFunctionWithName("Serialization::innerStoreObject");
  output.MakeSerialization("LoadSlTwoSubalgebras", theCommands, 1);
  Expression theSequence, tempE;
  theSequence.reset(theCommands);
  tempE.MakeAtom(theCommands.opSequence(), theCommands);
  theSequence.children.ReservE(input.size+1);
  theSequence.AddChildOnTop(tempE);
  for (int i=0; i<input.size; i++)
  { if (!Serialization::innerStoreObject(theCommands, input[i], tempE))
      return false;
    theSequence.AddChildOnTop(tempE);
  }
  output.AddChildOnTop(theSequence);
  return true;
}

bool Serialization::innerStoreObject
  (CommandList& theCommands, const SemisimpleLieAlgebra& input, Expression& output)
{ MacroRegisterFunctionWithName("Serialization::innerStoreObject");
  output.MakeSerialization("SemisimpleLieAlgebra", theCommands, 1);
  std::cout << "<hr>" << output.ToString() << "<br>";
  Expression emptyC, tempE;
  emptyC.MakeEmptyContext(theCommands);
  if (!Serialization::SerializeMonCollection(theCommands, input.theWeyl.theDynkinType, tempE, emptyC))
    return false;
  std::cout << "<br>The mon collection: " << tempE.ToString();
  output.AddChildOnTop(tempE);
  output.format=output.formatDefault;
  return true;
}

bool Serialization::innerLoadSltwoSubalgebras
(CommandList& theCommands, const Expression& input, Expression& output)
{
  return false;
}

bool Serialization::innerLoadSemisimpleSubalgebras
  (CommandList& theCommands, const Expression& input, Expression& output)
{
  return false;
}

bool Serialization::innerStoreSemisimpleSubalgebras
  (CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("innerStoreSemisimpleSubalgebrass");
  if (!input.IsOfType<SemisimpleSubalgebras>())
    return false;
  SemisimpleSubalgebras& theSubalgebras=input.GetValuENonConstUseWithCaution<SemisimpleSubalgebras>();
  return Serialization::innerStoreObject(theCommands, theSubalgebras, output);
}

bool Serialization::innerStoreObject
  (CommandList& theCommands, const SemisimpleSubalgebras& input, Expression& output)
{ output.MakeSerialization("LoadSemisimpleSubalgebras", theCommands);
  Expression tempE;
  if (!Serialization::innerStoreObject(theCommands, *input.owneR, tempE))
    return false;
  output.AddChildOnTop(tempE);
  if (!Serialization::innerStoreObject(theCommands, input.theSl2s, tempE))
    return false;
  output.AddChildOnTop(tempE);
/*  List<SemisimpleLieAlgebra> SimpleComponentsSubalgebras;
  HashedListReferences<SemisimpleLieAlgebra> theSubalgebrasNonEmbedded;
  List<SltwoSubalgebras> theSl2sOfSubalgebras;

  List<CandidateSSSubalgebra> Hcandidates;
  int theRecursionCounter;
*/
  return true;
}

bool Serialization::innerStore
  (CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("innerStore");
  int theType;
  if (!input.IsBuiltInType(&theType))
    return false;
  if (theType==theCommands.opSSLieAlg())
    return Serialization::innerStoreSemisimpleLieAlgebra(theCommands, input, output);
  if (theType==theCommands.opPoly())
    return Serialization::innerStorePoly(theCommands, input, output);
  if (theType==theCommands.opSemisimpleSubalgebras())
    return Serialization::innerStoreSemisimpleSubalgebras(theCommands, input, output);
  if (theType==theCommands.opElementUEoverRF())
    return Serialization::innerStoreUE(theCommands, input, output);
  return output.SetError("Serialization not implemented for this data type.", theCommands);
}

bool Serialization::innerStoreUE
  (CommandList& theCommands, const Expression& input, Expression& output)
{ ElementUniversalEnveloping<RationalFunctionOld> theUE;
  if (!input.IsOfType(&theUE))
    return output.SetError("To ask to store a non-elementUE as an elementUE is not allowed", theCommands);
  output.MakeSerialization("UE", theCommands, 1);
  Expression tempE;
  Expression theContext=input.GetContext();
  if(!Serialization::SerializeMonCollection(theCommands, theUE, theContext, tempE))
  { theCommands.Comments << "<hr>Failed to store " << theUE.ToString();
    return false;
  }
  return output.AddChildOnTop(tempE);
}

bool Serialization::innerLoad
  (CommandList& theCommands, const Expression& input, Expression& output)
{ if (!input.IsListStartingWithAtom(theCommands.opSerialization()))
    return false;
  if (input.children.size<2)
    return false;
  output=input;
  output.children.PopIndexShiftDown(0);
  return true;
}

bool Serialization::innerPolynomial
(CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::innerPolynomial");
  RecursionDepthCounter theRecursionIncrementer(&theCommands.RecursionDeptH);
  //std::cout << "<br>Evaluating innerPolynomial on: " << input.ToString();
  //std::cout << "<br>First elt input is:" << input[0].ToString();
  return theCommands.outerExtractAndEvaluatePMTDtree<Polynomial<Rational> >
  (theCommands, input, output);
}

bool Serialization::innerStorePoly
  (CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("Serialization::innerStorePoly");
  Polynomial<Rational> thePoly;
  if (!input.IsOfType(&thePoly))
    return output.SetError
    ("To ask to store a non-polynomial to a polynomial is not allowed. ", theCommands);
  Expression theContext=input.GetContext();
  Expression tempE, resultE;
  if (!Serialization::SerializeMonCollection<MonomialP, Rational>(theCommands, thePoly, theContext, resultE))
    return false;
  resultE.CheckInitialization();
  output.MakeSerialization("Polynomial", theCommands, 1);
  output.AddChildOnTop(tempE);
  output.AddChildOnTop(resultE);
  output.format=output.formatDefault;
  return true;
}

bool CommandList::innerRationalFunction
(CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::innerPolynomial");
  RecursionDepthCounter theRecursionIncrementer(&theCommands.RecursionDeptH);
  //std::cout << "<br>Evaluating innerPolynomial on: " << input.ToString();
  //std::cout << "<br>First elt input is:" << input[0].ToString();
  bool result= theCommands.outerExtractAndEvaluatePMTDtree<RationalFunctionOld>
  (theCommands, input, output);
  if (!output.IsOfType<RationalFunctionOld>())
  { std::cout << "<br>This is a programming error: outerExtractAndEvaluatePMTDtree returned true "
    << "from input" << input.ToString()
    << " but the result is not of type RationalFunctionOld, instead it is "
    << output.ToString() << ". "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  return result;
}

bool CommandList::innerElementUniversalEnvelopingAlgebra
(CommandList& theCommands, const Expression& input, Expression& output)
{ MacroRegisterFunctionWithName("CommandList::innerElementUniversalEnvelopingAlgebra");
  RecursionDepthCounter theRecursionIncrementer(&theCommands.RecursionDeptH);
  if (input.IsOfType<ElementUniversalEnveloping<RationalFunctionOld > >())
  { output=input;
    return true;
  }
  if (!theCommands.outerExtractAndEvaluatePMTDtree<ElementUniversalEnveloping<RationalFunctionOld > >
      (theCommands, input, output))
    return output.SetError
    ("Failed to convert " +input.ToString() + " to element universal enveloping.", theCommands);
  ElementUniversalEnveloping<RationalFunctionOld> outputUE;
  if (!output.IsOfType(&outputUE))
    return output.SetError("Failed to convert to element universal enveloping.", theCommands);
//  std::cout << "<br>innerElementUniversalEnvelopingAlgebra: output.Context(): "
//  << output.GetContext().ToString();
  outputUE.Simplify(*theCommands.theGlobalVariableS, 1, 0);
  return output.AssignValueWithContext(outputUE, output.GetContext(), theCommands);
}

bool Serialization::innerLoadRationalFunction
(CommandList& theCommands, const Expression& input, Expression& output)
{ RationalFunctionOld theRF;
  if (!input.IsOfType(&theRF))
    return output.SetError
    ("To ask to store a non-rational function as a rational function is not allowed.", theCommands);
  Expression contextE=input.GetContext();
  return Serialization::innerStoreObject(theCommands, theRF, output, contextE);
}

bool Serialization::innerStoreObject
(CommandList& theCommands, const RationalFunctionOld& input, Expression& output, const Expression& theContext)
{ if (input.expressionType==input.typeRational)
    return output.AssignValue(input.ratValue, theCommands);
  Polynomial<Rational> theNumerator;
  input.GetNumerator(theNumerator);
  if (input.expressionType==input.typePoly)
    return Serialization::SerializeMonCollection(theCommands, theNumerator, output, theContext);
  if (input.expressionType!=input.typeRationalFunction)
  { std::cout << "This is a programming error: I am processing a rational function which is not "
    << " of type rational polynomial or honest rataional function. Something has gone very wrong. "
    << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    assert(false);
  }
  Polynomial<Rational> theDenominator;
  input.GetDenominator(theDenominator);
  Expression denE, numE;
  if (!Serialization::SerializeMonCollection(theCommands, theNumerator, numE, theContext ))
  { theCommands.Comments << "<hr>Failed to serialize numerator of rational function. ";
    return false;
  }
  if (!Serialization::SerializeMonCollection(theCommands, theDenominator, denE, theContext ))
  { theCommands.Comments << "<hr>Failed to serialize denominator of rational function. ";
    return false;
  }
  return output.MakeXOX(theCommands, theCommands.opDivide(), numE, denE);
}