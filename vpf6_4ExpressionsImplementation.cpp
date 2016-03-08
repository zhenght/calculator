//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#include "vpf.h"
#include "vpfImplementationHeader2Math3_WeylAlgebra.h"
#include "vpfImplementationHeader2Math15_UniversalEnveloping.h"
#include "vpfImplementationHeader2Math6_ModulesSSLieAlgebras.h"
#include "vpfImplementationHeader2Math1_SemisimpleLieAlgebras.h"
#include "vpfImplementationHeader2Math3_FiniteGroups.h"
#include "vpfHeader2Math3_SymmetricGroupsAndGeneralizations.h"
ProjectInformationInstance ProjectInfoVpf6_4ExpressionsImplementationcpp(__FILE__, "Calculator expression implementation. ");

Expression operator*(const Expression& left, const Expression& right)
{ left.CheckInitialization();
  Expression result;

  result.MakeXOX(*left.owner, left.owner->opTimes(), left, right);
//  stOutput << "<br>result: " << result.ToString();
  return result;
}

Expression operator/(const Expression& left, const Expression& right)
{ left.CheckInitialization();
  Expression result;
  result.MakeXOX(*left.owner, left.owner->opDivide(), left, right);
  return result;
}

Expression operator+(const Expression& left, const Expression& right)
{ left.CheckInitialization();
  Expression result;
  result.MakeXOX(*left.owner, left.owner->opPlus(), left, right);
  return result;
}

Expression operator-(const Expression& left, const Expression& right)
{ left.CheckInitialization();
  Expression result;
  result.MakeXOX(*left.owner, left.owner->opMinus(), left, right);
  return result;
}

//If you get a specialization after instantiation error:
//the following template specialization funcitons must appear
//here and nowhere else (discovered through extremely painful experimentation).

template < >
List<Expression>::OrderLeftGreaterThanRight
FormatExpressions::GetMonOrder<Expression>()
{ return 0;
}

//Expression::GetTypeOperation specializations follow
template < >
int Expression::GetTypeOperation<RationalFunctionOld>()const
{ this->CheckInitialization();
  return this->owner->opRationalFunction();
}

template < >
int Expression::GetTypeOperation<Rational>()const
{ this->CheckInitialization();
  return this->owner->opRational();
}

template < >
int Expression::GetTypeOperation<ElementHyperoctahedralGroupR2>()const
{ this->CheckInitialization();
  return this->owner->opElementHyperOctahedral();
}

template < >
int Expression::GetTypeOperation<GroupRepresentation<FiniteGroup<ElementHyperoctahedralGroupR2>, Rational> >()const
{ this->CheckInitialization();
  return this->owner->opHyperoctahedralGroupRep();
}

template < >
int Expression::GetTypeOperation<std::string>()const
{ this->CheckInitialization();
  return this->owner->opString();
}

template < >
int Expression::GetTypeOperation<AlgebraicNumber>()const
{ this->CheckInitialization();
  return this->owner->opAlgNumber();
}

template < >
int Expression::GetTypeOperation<ElementZmodP>()const
{ this->CheckInitialization();
  return this->owner->opEltZmodP();
}

template < >
int Expression::GetTypeOperation<ElementUniversalEnveloping<RationalFunctionOld> >()const
{ this->CheckInitialization();
  return this->owner->opElementUEoverRF();
}

template < >
int Expression::GetTypeOperation<Matrix<Rational> >()const
{ this->CheckInitialization();
  return this->owner->opMatRat();
}

template < >
int Expression::GetTypeOperation<Matrix<AlgebraicNumber> >()const
{ this->CheckInitialization();
  return this->owner->opMatAlgebraic();
}

template < >
int Expression::GetTypeOperation<Weight<Rational> >()const
{ this->CheckInitialization();
  return this->owner->opWeightLieAlg();
}

template < >
int Expression::GetTypeOperation<Weight<Polynomial<Rational> > >()const
{ this->CheckInitialization();
  return this->owner->opWeightLieAlgPoly();
}

template < >
int Expression::GetTypeOperation<MatrixTensor<Rational> >()const
{ this->CheckInitialization();
  return this->owner->opMatTensorRat();
}

template < >
int Expression::GetTypeOperation<Matrix<RationalFunctionOld> >()const
{ this->CheckInitialization();
  return this->owner->opMatRF();
}

template < >
int Expression::GetTypeOperation<int>()const
{ this->CheckInitialization();
  return this->owner->opRational();
}

template < >
int Expression::GetTypeOperation<double>()const
{ this->CheckInitialization();
  return this->owner->opDouble();
}

template < >
int Expression::GetTypeOperation<Polynomial<Rational> >()const
{ this->CheckInitialization();
  return this->owner->opPoly();
}

template < >
int Expression::GetTypeOperation<Polynomial<AlgebraicNumber> >()const
{ this->CheckInitialization();
  return this->owner->opPolyOverANs();
}

template < >
int Expression::GetTypeOperation<ElementWeylAlgebra<Rational> >()const
{ this->CheckInitialization();
  return this->owner->opElementWeylAlgebra();
}

template < >
int Expression::GetTypeOperation<SemisimpleLieAlgebra>()const
{ this->CheckInitialization();
  return this->owner->opSSLieAlg();
}

template < >
int Expression::GetTypeOperation<LittelmannPath>()const
{ this->CheckInitialization();
  return this->owner->opLittelmannPath();
}

template < >
int Expression::GetTypeOperation<MonomialTensor<int, MathRoutines::IntUnsignIdentity> >()const
{ this->CheckInitialization();
  return this->owner->opLRO();
}

template < >
int Expression::GetTypeOperation<ElementTensorsGeneralizedVermas<RationalFunctionOld> >()const
{ this->CheckInitialization();
  return this->owner->opElementTensorGVM();
}

template < >
int Expression::GetTypeOperation<charSSAlgMod<Rational> >()const
{ this->CheckInitialization();
  return this->owner->opCharSSAlgMod();
}

template < >
int Expression::GetTypeOperation<Plot>()const
{ this->CheckInitialization();
  return this->owner->opCalculusPlot();
}

template < >
int Expression::GetTypeOperation<PlotObject>()const
{ this->CheckInitialization();
  return this->owner->opCalculusPlot();
}

template < >
int Expression::GetTypeOperation<SemisimpleSubalgebras>()const
{ this->CheckInitialization();
  return this->owner->opSemisimpleSubalgebras();
}

template < >
int Expression::GetTypeOperation<WeylGroupData>()const
{ this->CheckInitialization();
  return this->owner->opWeylGroup();
}

template < >
int Expression::GetTypeOperation<GroupRepresentation<FiniteGroup<ElementWeylGroup<WeylGroupData> >, Rational> >()const
{ this->CheckInitialization();
  return this->owner->opWeylGroupRep();
}

template < >
int Expression::GetTypeOperation<ElementWeylGroup<WeylGroupData> >()const
{ this->CheckInitialization();
  return this->owner->opWeylGroupElement();
}

template < >
int Expression::GetTypeOperation<VirtualRepresentation<FiniteGroup<ElementWeylGroup<WeylGroupData> >, Rational> >()const
{ this->CheckInitialization();
  return this->owner->opWeylGroupVirtualRep();
}

//Expression::GetTypeOperation specializations end.
//Expression::ConvertsToType specializations follow.
template < >
bool Expression::ConvertsToType(
ElementSemisimpleLieAlgebra<AlgebraicNumber>* whichElement
)const
{ MacroRegisterFunctionWithName("Expression::ConvertsToType");
  if (this->owner==0)
    return false;
  ElementUniversalEnveloping<RationalFunctionOld> theUEElt;
  if (!this->IsOfType<ElementUniversalEnveloping<RationalFunctionOld> >(&theUEElt))
    return false;
  ElementSemisimpleLieAlgebra<Rational> theRatElt;
  if (!theUEElt.GetLieAlgebraElementIfPossible(theRatElt))
    return false;
  if (whichElement!=0)
    *whichElement=theRatElt;
  return true;
}

//Expression::ConvertsToType specializations end.
//Expression::AddObjectReturnIndex specializations follow
template < >
int Expression::AddObjectReturnIndex(const
SemisimpleSubalgebras
& inputValue)const
{ this->CheckInitialization();
  if (!this->owner->theObjectContainer.theSSsubalgebras.Contains(inputValue))
    crash << "Semisimple subalgebras must be allocated directly in the object container. " << crash;
  return this->owner->theObjectContainer.theSSsubalgebras.GetIndex(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
ElementHyperoctahedralGroupR2
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theElementsHyperOctGroup
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
std::string
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theStrings
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
GroupRepresentation<FiniteGroup<ElementHyperoctahedralGroupR2>, Rational>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theHyperoctahedralReps
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
Rational
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theRationals
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
double
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theDoubles
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
RationalFunctionOld
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theRFs
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
ElementUniversalEnveloping<RationalFunctionOld>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theUEs
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
Weight<Rational>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theWeights
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
Weight<Polynomial<Rational> >
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theWeightsPoly
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
int
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theRationals
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
Polynomial<Rational>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.thePolys
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
Polynomial<AlgebraicNumber>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.thePolysOverANs
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
ElementWeylAlgebra<Rational>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theWeylAlgebraElements
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
SemisimpleLieAlgebra
& inputValue)const
{ this->CheckInitialization();
  if (!this->owner->theObjectContainer.theLieAlgebras.Contains(inputValue))
    crash << "Semisimple Lie algebra must be allocated directly in the object container. " << crash;
  return this->owner->theObjectContainer.theLieAlgebras.GetIndex(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
AlgebraicNumber
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theAlgebraicNumbers
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
ElementZmodP
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theEltsModP
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
MonomialTensor<int, MathRoutines::IntUnsignIdentity>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theLittelmannOperators
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
ElementTensorsGeneralizedVermas<RationalFunctionOld>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theTensorElts
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
charSSAlgMod<Rational>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theCharsSSLieAlgFD
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
LittelmannPath
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theLSpaths
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
Matrix<Rational>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theMatRats
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
Matrix<AlgebraicNumber>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theMatsAlgebraic
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
MatrixTensor<Rational>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theMatTensorRats
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
Matrix<RationalFunctionOld>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theMatRFs
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
Plot
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.thePlots
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
PlotObject
& inputValue)const
{ this->CheckInitialization();
  Plot plotContainer;
  plotContainer+=inputValue;
  return this->owner->theObjectContainer.thePlots
  .AddNoRepetitionOrReturnIndexFirst(plotContainer);
}

template < >
int Expression::AddObjectReturnIndex(const
WeylGroupData
& inputValue)const
{ this->CheckInitialization();
  for (int i=0; i<this->owner->theObjectContainer.theLieAlgebras.size; i++)
    if (this->owner->theObjectContainer.theLieAlgebras[i].theWeyl==inputValue)
      return i;
  SemisimpleLieAlgebra theLA;
  theLA.theWeyl=inputValue;
  this->owner->theObjectContainer.theLieAlgebras.AddOnTop(theLA);
  return this->owner->theObjectContainer.theLieAlgebras.size-1;
}

template < >
int Expression::AddObjectReturnIndex(const
GroupRepresentation<FiniteGroup<ElementWeylGroup<WeylGroupData> >, Rational>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theWeylGroupReps
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
VirtualRepresentation<FiniteGroup<ElementWeylGroup<WeylGroupData> >, Rational>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theWeylGroupVirtualReps
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

template < >
int Expression::AddObjectReturnIndex(const
ElementWeylGroup<WeylGroupData>
& inputValue)const
{ this->CheckInitialization();
  return this->owner->theObjectContainer.theWeylGroupElements
  .AddNoRepetitionOrReturnIndexFirst(inputValue);
}

//Expression::AddObjectReturnIndex specializations end

//start Expression::GetValueNonConst specializations.
template < >
Rational& Expression::GetValueNonConst()const
{ if (!this->IsOfType<Rational>())
    crash << "This is a programming error: expression not of required type Rational. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theRationals.GetElement(this->GetLastChild().theData);
}

template < >
ElementHyperoctahedralGroupR2& Expression::GetValueNonConst()const
{ if (!this->IsOfType<ElementHyperoctahedralGroupR2>())
    crash << "This is a programming error: expression not of required type Rational. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theElementsHyperOctGroup.GetElement(this->GetLastChild().theData);
}

template < >
GroupRepresentation<FiniteGroup<ElementHyperoctahedralGroupR2>, Rational> & Expression::GetValueNonConst()const
{ if (!this->IsOfType<GroupRepresentation<FiniteGroup<ElementHyperoctahedralGroupR2>, Rational> >())
    crash << "This is a programming error: expression not of required type Rational. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theHyperoctahedralReps[this->GetLastChild().theData];
}

template < >
ElementZmodP& Expression::GetValueNonConst()const
{ if (!this->IsOfType<ElementZmodP>())
    crash << "This is a programming error: expression not of required type ElementZmodP. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theEltsModP.GetElement(this->GetLastChild().theData);
}

template < >
AlgebraicNumber& Expression::GetValueNonConst()const
{ if (!this->IsOfType<AlgebraicNumber>())
    crash << "This is a programming error: expression not of required type AlgebraicNumber. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theAlgebraicNumbers.GetElement(this->GetLastChild().theData);
}

template < >
double& Expression::GetValueNonConst()const
{ if (!this->IsOfType<double>())
    crash << "This is a programming error: expression not of required type double. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theDoubles.GetElement(this->GetLastChild().theData);
}

template < >
std::string& Expression::GetValueNonConst()const
{ if (!this->IsOfType<std::string>())
    crash << "This is a programming error: expression not of required type std::string. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theStrings.GetElement(this->GetLastChild().theData);
}

template < >
RationalFunctionOld& Expression::GetValueNonConst()const
{ if (!this->IsOfType<RationalFunctionOld>())
    crash << "This is a programming error: expression not of required type RationalFunctionOld. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theRFs.GetElement(this->GetLastChild().theData);
}

template < >
ElementUniversalEnveloping<RationalFunctionOld>& Expression::GetValueNonConst()const
{ if (!this->IsOfType<ElementUniversalEnveloping<RationalFunctionOld> >())
    crash << "This is a programming error: expression not of required type ElementUniversalEnveloping_RationalFunctionOld. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theUEs.GetElement(this->GetLastChild().theData);
}

template < >
Polynomial<Rational>& Expression::GetValueNonConst()const
{ if (!this->IsOfType<Polynomial<Rational> >())
    crash << "This is a programming error: expression not of required type Polynomial_Rational. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.thePolys.GetElement(this->GetLastChild().theData);
}

template < >
Polynomial<AlgebraicNumber>& Expression::GetValueNonConst()const
{ if (!this->IsOfType<Polynomial<AlgebraicNumber> >())
    crash << "This is a programming error: expression not of required type Polynomial_AlgebraicNumber. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.thePolysOverANs.GetElement(this->GetLastChild().theData);
}

template < >
ElementWeylAlgebra<Rational>& Expression::GetValueNonConst()const
{ if (!this->IsOfType<ElementWeylAlgebra<Rational> >())
    crash << "This is a programming error: expression not of required type ElementWeylAlgebra_Rational. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theWeylAlgebraElements.GetElement(this->GetLastChild().theData);
}

template < >
LittelmannPath& Expression::GetValueNonConst()const
{ if (!this->IsOfType<LittelmannPath>())
    crash << "This is a programming error: expression not of required type LittelmannPath. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theLSpaths.GetElement(this->GetLastChild().theData);
}

template < >
Weight<Polynomial<Rational> >& Expression::GetValueNonConst()const
{ if (!this->IsOfType<Weight<Polynomial<Rational> > >())
    crash << "This is a programming error: expression not of required type LittelmannPath. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theWeightsPoly.GetElement(this->GetLastChild().theData);
}

template < >
MonomialTensor<int, MathRoutines::IntUnsignIdentity>& Expression::GetValueNonConst()const
{ if (!this->IsOfType<MonomialTensor<int, MathRoutines::IntUnsignIdentity> >())
    crash << "This is a programming error: expression not of required type MonomialTensor. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theLittelmannOperators.GetElement(this->GetLastChild().theData);
}

template < >
ElementTensorsGeneralizedVermas<RationalFunctionOld>& Expression::GetValueNonConst()const
{ if (!this->IsOfType<ElementTensorsGeneralizedVermas<RationalFunctionOld> >())
    crash << "This is a programming error: expression not of required type ElementTGVM_RationalFunctionOld. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theTensorElts.GetElement(this->GetLastChild().theData);
}

template < >
charSSAlgMod<Rational>& Expression::GetValueNonConst()const
{ if (!this->IsOfType<charSSAlgMod<Rational> >())
    crash << "This is a programming error: expression not of required type charSSAlgMod_Rational. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theCharsSSLieAlgFD.GetElement(this->GetLastChild().theData);
}

template < >
SemisimpleLieAlgebra& Expression::GetValueNonConst()const
{ if (!this->IsOfType<SemisimpleLieAlgebra>())
    crash << "This is a programming error: expression not of required type SemisimpleLieAlgebra. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theLieAlgebras.GetElement(this->GetLastChild().theData);
}

template < >
Matrix<Rational>& Expression::GetValueNonConst()const
{ if (!this->IsOfType<Matrix<Rational> >())
    crash << "This is a programming error: expression not of required type Matrix_Rational. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theMatRats.GetElement(this->GetLastChild().theData);
}

template < >
Matrix<AlgebraicNumber>& Expression::GetValueNonConst()const
{ if (!this->IsOfType<Matrix<AlgebraicNumber> >())
    crash << "This is a programming error: expression not of required type MatrixAlgebraic. The expression equals "
    << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theMatsAlgebraic.GetElement(this->GetLastChild().theData);
}

template < >
MatrixTensor<Rational>& Expression::GetValueNonConst()const
{ if (!this->IsOfType<MatrixTensor<Rational> >())
    crash << "This is a programming error: expression not of required type MatrixTensorRational. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theMatTensorRats.GetElement(this->GetLastChild().theData);
}

template < >
Matrix<RationalFunctionOld>& Expression::GetValueNonConst()const
{ if (!this->IsOfType<Matrix<RationalFunctionOld> >())
    crash << "This is a programming error: expression not of required type MatrixRF. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theMatRFs.GetElement(this->GetLastChild().theData);
}

template < >
SemisimpleSubalgebras& Expression::GetValueNonConst()const
{ if (!this->IsOfType<SemisimpleSubalgebras>())
    crash << "This is a programming error: expression not of required type SemisimpleSubalgebras. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theSSsubalgebras[this->GetLastChild().theData];
}

template < >
Plot& Expression::GetValueNonConst()const
{ if (!this->IsOfType<Plot>())
    crash << "This is a programming error: expression not of required type Plot. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.thePlots[this->GetLastChild().theData];
}

template < >
WeylGroupData& Expression::GetValueNonConst()const
{ if (!this->IsOfType<WeylGroupData>())
    crash << "This is a programming error: expression not of required type WeylGroupData. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theLieAlgebras.GetElement(this->GetLastChild().theData).theWeyl;
}

template < >
GroupRepresentation<FiniteGroup<ElementWeylGroup<WeylGroupData> >, Rational>& Expression::GetValueNonConst()const
{ if (!this->IsOfType<GroupRepresentation<FiniteGroup<ElementWeylGroup<WeylGroupData> >, Rational> >())
    crash << "This is a programming error: expression not of required type WeylGroupRepresentation_Rational. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theWeylGroupReps.GetElement(this->GetLastChild().theData);
}

template < >
ElementWeylGroup<WeylGroupData>& Expression::GetValueNonConst()const
{ if (!this->IsOfType<ElementWeylGroup<WeylGroupData> >())
    crash << "This is a programming error: expression not of required type ElementWeylGroup. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theWeylGroupElements.GetElement(this->GetLastChild().theData);
}

template < >
VirtualRepresentation<FiniteGroup<ElementWeylGroup<WeylGroupData> >, Rational>& Expression::GetValueNonConst()const
{ if (!this->IsOfType<VirtualRepresentation<FiniteGroup<ElementWeylGroup<WeylGroupData> >, Rational> >())
    crash << "This is a programming error: expression not of required type WeylGroupVirtualRepresentation. The expression equals " << this->ToString() << "." << crash;
  return this->owner->theObjectContainer.theWeylGroupVirtualReps.GetElement(this->GetLastChild().theData);
}

//end Expression::GetValueNonConst specializations.
//start Expression::ConvertToType specializations.
template< >
bool Expression::ConvertToType<Polynomial<AlgebraicNumber> >(Expression& output)const
{ MacroRegisterFunctionWithName("ConvertToType_Polynomial_AlgebraicNumber");
//  stOutput << "<br>got ere, converting " << this->ToString() << " to polynomial with algebraic number cfs.";
  this->CheckInitialization();
  if (this->IsOfType<Rational>())
  { Polynomial<AlgebraicNumber> resultP;
    resultP.MakeConst(this->GetValue<Rational>());
    return output.AssignValueWithContext(resultP, this->GetContext(), *this->owner);
  }
  if (this->IsOfType<AlgebraicNumber>())
  { //stOutput << "<br>" << this->ToString() << " IS an algebraic number!";
    Polynomial<AlgebraicNumber> resultP;
    resultP.MakeConst(this->GetValue<AlgebraicNumber>());
    //stOutput << "<br>output gotta be assigned: " << resultP.ToString();
    return output.AssignValueWithContext(resultP, this->GetContext(), *this->owner);
  }
//  stOutput << "<br>" << this->ToString() << " aint an algebraic number!";
  if (this->IsOfType<Polynomial<Rational> >())
  { Polynomial<AlgebraicNumber> resultP;
    resultP=this->GetValue<Polynomial<Rational> >();
    return output.AssignValueWithContext(resultP, this->GetContext(), *this->owner);
  }
  if (this->IsOfType<Polynomial<AlgebraicNumber> >())
  { output=*this;
    return true;
  }
  return false;
}

template< >
bool Expression::ConvertToType<Polynomial<Rational> >(Expression& output)const
{ MacroRegisterFunctionWithName("ConvertToType_Polynomial_Rational");
  this->CheckInitialization();
  if (this->IsOfType<Rational>())
  { Polynomial<Rational> resultP;
    resultP.MakeConst(this->GetValue<Rational>());
    return output.AssignValueWithContext(resultP, this->GetContext(), *this->owner);
  }
  if (this->IsOfType<Polynomial<Rational> >())
  { output=*this;
    return true;
  }
  return false;
}

template< >
bool Expression::ConvertToType<RationalFunctionOld>(Expression& output)const
{ MacroRegisterFunctionWithName("ConvertToType_RationalFunctionOld");
  this->CheckInitialization();
  if (this->IsOfType<Rational>())
  { RationalFunctionOld resultRF;
    resultRF.MakeConst(this->GetValue<Rational>(), &theGlobalVariables);
//    stOutput << "<hr>Converting to type: " << this->ToString();
//    stOutput << "<br>My context is: " << this->GetContext().ToString();
    return output.AssignValueWithContext(resultRF, this->GetContext(), *this->owner);
  }
  if (this->IsOfType<Polynomial<Rational> >())
  { RationalFunctionOld resultRF;
    resultRF.context=&theGlobalVariables;
    resultRF=this->GetValue<Polynomial<Rational> >();
    return output.AssignValueWithContext(resultRF, this->GetContext(), *this->owner);
  }
  if (this->IsOfType<RationalFunctionOld>())
  { output=*this;
    return true;
  }
  (*this->owner) << "<hr>ConvertToType_RationalFunctionOld: Failed to convert " << this->ToString() << " to rational function. ";
  return false;
}

template< >
bool Expression::ConvertToType<ElementWeylAlgebra<Rational> >(Expression& output)const
{ MacroRegisterFunctionWithName("ConvertToType_ElementWeylAlgebra");
  this->CheckInitialization();
  if (this->IsOfType<Rational>())
  { ElementWeylAlgebra<Rational> resultEWA;
    resultEWA.MakeConst(this->GetValue<Rational>());
    return output.AssignValueWithContext(resultEWA, this->GetContext(), *this->owner);
  }
  if (this->IsOfType<Polynomial<Rational> >())
  { ElementWeylAlgebra<Rational> resultEWA;
    resultEWA.AssignPolynomial(this->GetValue<Polynomial<Rational> >());
    return output.AssignValueWithContext(resultEWA, this->GetContext(), *this->owner);
  }
  if (this->IsOfType<ElementWeylAlgebra<Rational> >())
  { output=*this;
    return true;
  }
  return false;
}

template< >
bool Expression::ConvertToType<Weight<Polynomial<Rational> > >(Expression& output)const
{ MacroRegisterFunctionWithName("ConvertToType_Weight_Polynomial");
  this->CheckInitialization();
  if (this->IsOfType<Weight<Polynomial<Rational> > >())
  { output=*this;
    return true;
  }
  return false;
}

template< >
bool Expression::ConvertToType<ElementUniversalEnveloping<RationalFunctionOld> >(Expression& output)const
{ MacroRegisterFunctionWithName("ConvertToType_RationalFunctionOld");
  //stOutput << "<hr>convert to ue called on: " << this->ToString();
  this->CheckInitialization();
  if (!this->IsBuiltInType())
    return false;
  if (this->IsOfType<ElementUniversalEnveloping<RationalFunctionOld> >())
  { output=*this;
    return true;
  }
//  stOutput << "<hr>Getting ue from: " << this->ToString();
  SemisimpleLieAlgebra* theOwner=this->GetAmbientSSAlgebraNonConstUseWithCaution();
  if (theOwner==0)
  { this->owner->Comments << "<hr>Failed to convert " << this->ToString() << " (Lispified: " << this->ToStringFull()
    << ") to element of universal enveloping -  failed to extract ambient Lie algebra. ";
    return false;
  }
  ElementUniversalEnveloping<RationalFunctionOld> outputUE;
  if (this->IsOfType<Rational>())
  { outputUE.MakeConst(this->GetValue<Rational>(), *theOwner);
    return output.AssignValueWithContext(outputUE, this->GetContext(), *this->owner);
  } else if (this->IsOfType<Polynomial<Rational> >())
  { RationalFunctionOld tempRF;
    tempRF=this->GetValue<Polynomial<Rational> >();
    outputUE.MakeConst(tempRF, *theOwner);
    return output.AssignValueWithContext(outputUE, this->GetContext(), *this->owner);
  } else if (this->IsOfType<RationalFunctionOld>())
  { outputUE.MakeConst(this->GetValue<RationalFunctionOld>(), *theOwner);
    return output.AssignValueWithContext(outputUE, this->GetContext(), *this->owner);
  }
  return *this->owner << "<hr>Failed to convert " << this->ToString() << " to element of universal enveloping -  I don't know how. ";
}

template< >
bool Expression::ConvertToType<ElementTensorsGeneralizedVermas<RationalFunctionOld> >(Expression& output)const
{ MacroRegisterFunctionWithName("ConvertToType_RationalFunctionOld");
  this->CheckInitialization();
  if (this->IsOfType<ElementTensorsGeneralizedVermas<RationalFunctionOld> >())
  { output=*this;
    return true;
  }
  return false;
}
//end Expression::ConvertToType specializations.

bool Expression::CheckConsistencyRecursively()const
{ MacroRegisterFunctionWithName("Expression::CheckConsistencyRecursively");
  this->CheckConsistency();
  for (int i=0; i<this->children.size; i++)
    (*this)[i].CheckConsistency();
  return true;
}

bool Expression::CheckConsistency()const
{ MacroRegisterFunctionWithName("Expression::CheckConsistency");
  //warning: do not use ToString method from here: ToString itself calls CheckConosistency, so that causes an "infinite" recursion call cycle,
  //i.e., stack overflow.
  if (this->flagDeallocated)
    crash << "Use after free of Expression. " << crash;
  this->CheckInitialization();
  if (this->children.size==0)
    return false;
  if (this->IsBuiltInType())
  { if (this->children.size!=3)
      crash << "This is a programming error. At the moment of writing, an expression of built-in type must have 3 "
      << "children: type, context, and index in Calculator. The expression is " << this->ToStringFull()
      << crash;
    const Expression& mustBeTheContext=(*this)[1];
    if (!mustBeTheContext.StartsWith(this->owner->opContexT()))
      crash << "This is a programming error. At the moment of writing, the second child of a built-in type must be a context. It is instead "
      << mustBeTheContext.ToStringFull() << crash;
    for (int i=1; i<mustBeTheContext.children.size; i++)
    { bool isGood=false;
      const Expression& currentE=mustBeTheContext[i];
      if (currentE.StartsWith(this->owner->opPolynomialVariables()))
        isGood=true;
      if (currentE.StartsWith(this->owner->opSSLieAlg()))
        isGood=true;
      if (currentE.StartsWith(this->owner->opWeylAlgebraVariables()))
        isGood=true;
      if (!isGood)
        crash << "This is a programming error. The context " << mustBeTheContext.ToStringFull() << " has an entry which I do not recognize, namely, "
        << currentE.ToStringFull() << ". " << crash;
    }
  }
  return true;
}

const Expression& Expression::operator[](int n)const
{ this->CheckInitialization();
  int childIndex=this->children[n];
  if (childIndex<0)
    crash << "<hr>This is a programming error: the child of position " << n << " out of " << this->children.size-1
    << " is not contained in the expression container. " << crash;
  return this->owner->theExpressionContainer[childIndex];
}

bool Expression::AddChildOnTop(const Expression& inputChild)
{ this->CheckInitialization();
  this->children.AddOnTop(this->owner->theExpressionContainer.AddNoRepetitionOrReturnIndexFirst(inputChild));
  return true;
}

bool Expression::SetChildAtomValue(int childIndex, int TheAtomValue)
{ this->CheckInitialization();
  Expression tempE;
  tempE.MakeAtom(TheAtomValue, *this->owner);
  this->children.SetObjectAtIndex
  (childIndex, this->owner->theExpressionContainer.AddNoRepetitionOrReturnIndexFirst(tempE));
  return true;
}

bool Expression::SetChildAtomValue(int childIndex, const std::string& theAtom)
{ this->CheckInitialization();
  Expression tempE;
  tempE.MakeAtom(theAtom, *this->owner);
  this->children.SetObjectAtIndex
  (childIndex, this->owner->theExpressionContainer.AddNoRepetitionOrReturnIndexFirst(tempE));
  return true;
}

bool Expression::SetChilD(int childIndexInMe, const Expression& inputChild)
{ this->CheckInitialization();
  int theIndexOfTheExpression=this->owner->theExpressionContainer.AddNoRepetitionOrReturnIndexFirst(inputChild);
  this->children.SetObjectAtIndex(childIndexInMe, theIndexOfTheExpression);
  return true;
}

bool Expression::AddChildAtomOnTop(const std::string& theOperationString)
{ this->CheckInitialization();
  return this->AddChildAtomOnTop
  (this->owner->AddOperationNoRepetitionOrReturnIndexFirst(theOperationString));
}

bool Expression::StartsWithBuiltInAtom()const
{ if (this->owner==0)
    return false;
  if (this->IsAtom() || this->children.size<1)
    return false;
  return (*this)[0].IsBuiltInAtom();
}

bool Expression::StartsWithArithmeticOperation()const
{ if (this->owner==0)
    return false;
  if (this->IsAtom() || this->children.size<1)
    return false;
  return (*this)[0].IsArithmeticOperation();
}

bool Expression::StartsWithFunctionWithComplexRange()const
{ if (this->owner==0)
    return false;
  if (this->IsAtom() || this->children.size<1)
    return false;
  return (*this)[0].IsKnownFunctionWithComplexRange();
}

bool Expression::StartsWith(int theOp, int N)const
{ if (N!=-1)
  { if (this->children.size!=N)
      return false;
  } else
    if (this->children.size==0)
      return false;
  if (!(*this)[0].IsAtom())
    return false;
  if (theOp==-1)
    return true;
  return (*this)[0].theData==theOp;
}

int Expression::ContextGetNumContextVariables()const
{ if (this->owner==0)
    return 0;
  if (!this->StartsWith(this->owner->opContexT()))
    return 0;
  for (int i=1; i<this->children.size; i++)
    if ((*this)[i].StartsWith(this->owner->opPolynomialVariables()))
      return (*this)[i].children.size-1;
  return 0;
}

bool Expression::SetContextAtLeastEqualTo(Expression& inputOutputMinContext)
{ MacroRegisterFunctionWithName("Expression::SetContextAtLeastEqualTo");
  this->CheckInitialization();
  if (!this->IsBuiltInType())
    crash << "This is a programming error: calling Expression::SetContextAtLeastEqualTo on an expression that is not of built-in type. "
    << "Contexts are Reserved for built-in data types. " << crash;
  if (!inputOutputMinContext.IsContext())
  { this->owner->Comments << "<br>Warning: non-initialized input context in Expression::SetContextAtLeastEqualTo. Stack trace: "
    << Crasher::GetStackTraceEtcErrorMessage();
    inputOutputMinContext.MakeEmptyContext(*this->owner);
  }
//  stOutput << "<br>Setting context at least equal to " << inputOutputMinContext.ToString()
//  << " in expression " << this->ToString();
  Expression newContext;
  Expression myOldContext=this->GetContext();
//  stOutput << "<br>In " << this->ToString() << ", merging my old context, " << myOldContext.Lispify() << " and the input context, " << inputOutputMinContext.Lispify();
  if (!this->ContextMergeContexts(myOldContext, inputOutputMinContext, newContext))
    return false;
  if (myOldContext==newContext)
    return true;
  inputOutputMinContext=newContext;
  if (this->IsOfType<Rational>())
  { this->SetChilD(1, inputOutputMinContext);
//    stOutput << "<hr>Context of rational set; rational is: " << this->ToString();
    return true;
  }
  if (this->IsOfType<ElementWeylGroup<WeylGroupData> >())
    return this->AssignValueWithContext(this->GetValue<ElementWeylGroup<WeylGroupData> >(), inputOutputMinContext, *this->owner);
  if (this->IsOfType<AlgebraicNumber>())
    return this->SetChilD(1, inputOutputMinContext);
  if (this->IsOfType<ElementUniversalEnveloping<RationalFunctionOld> > ())
  { ElementUniversalEnveloping<RationalFunctionOld> newUE=this->GetValue<ElementUniversalEnveloping<RationalFunctionOld> >();
    PolynomialSubstitution<Rational> subPolyPart;
    myOldContext.ContextGetPolySubFromSuperContextNoFailure<Rational>(newContext, subPolyPart);
    newUE.Substitution(subPolyPart);
    return this->AssignValueWithContext(newUE, inputOutputMinContext, *this->owner);
  }
  if (this->IsOfType<Polynomial<Rational> >())
  { Polynomial<Rational> newPoly=this->GetValue<Polynomial<Rational> >();
    //stOutput << "<br>Subbing " << polySub.ToString() << " in "
    PolynomialSubstitution<Rational> subPolyPart;
    //<< newPoly.ToString();
    myOldContext.ContextGetPolySubFromSuperContextNoFailure<Rational>(newContext, subPolyPart);
    if (!newPoly.Substitution(subPolyPart))
      return false;
    return this->AssignValueWithContext(newPoly, inputOutputMinContext, *this->owner);
  }
  if (this->IsOfType<Polynomial<AlgebraicNumber> >())
  { Polynomial<AlgebraicNumber> newPoly=this->GetValue<Polynomial<AlgebraicNumber> >();
    //stOutput << "<br>Subbing " << polySub.ToString() << " in "
    PolynomialSubstitution<AlgebraicNumber> subPolyPart;
    //<< newPoly.ToString();
    myOldContext.ContextGetPolySubFromSuperContextNoFailure<AlgebraicNumber>(newContext, subPolyPart);
    if (!newPoly.Substitution(subPolyPart))
      return false;
    return this->AssignValueWithContext(newPoly, inputOutputMinContext, *this->owner);
  }
  if (this->IsOfType<ElementWeylAlgebra<Rational> >())
  { PolynomialSubstitution<Rational> subEWApart;
    PolynomialSubstitution<Rational> subPolyPart;
    myOldContext.ContextGetPolyAndEWASubFromSuperContextNoFailure(newContext, subPolyPart, subEWApart);
    ElementWeylAlgebra<Rational> outputEWA =this->GetValue<ElementWeylAlgebra<Rational> >();
//    stOutput << "<hr>Subbing in: " << outputEWA.ToString();
    if (!outputEWA.Substitution(subPolyPart, subEWApart))
    { this->owner->Comments << "<hr>Failed to convert " << outputEWA.ToString() << ": failed to carry out substitution "
      << subEWApart.ToString() << ", " << subPolyPart.ToString();
      return false;
    }
//    stOutput << "... to get: " << outputEWA.ToString();
    return this->AssignValueWithContext(outputEWA, inputOutputMinContext, *this->owner);
  }
  if (this->IsOfType<RationalFunctionOld>())
  { RationalFunctionOld newRF=this->GetValue<RationalFunctionOld>();
    PolynomialSubstitution<Rational> subPolyPart;
    myOldContext.ContextGetPolySubFromSuperContextNoFailure(newContext, subPolyPart);
    newRF.Substitution(subPolyPart);
    return this->AssignValueWithContext(newRF, inputOutputMinContext, *this->owner);
  }
  if (this->IsOfType<ElementTensorsGeneralizedVermas<RationalFunctionOld> >())
  { ElementTensorsGeneralizedVermas<RationalFunctionOld> newETGV=this->GetValue<ElementTensorsGeneralizedVermas<RationalFunctionOld> >();
    PolynomialSubstitution<Rational> subPolyPart;
    myOldContext.ContextGetPolySubFromSuperContextNoFailure(newContext, subPolyPart);
    newETGV.Substitution(subPolyPart, this->owner->theObjectContainer.theCategoryOmodules);
    return this->AssignValueWithContext(newETGV, inputOutputMinContext, *this->owner);
  }
  if (this->IsOfType<Weight<Polynomial<Rational> > >())
  { PolynomialSubstitution<Rational> subPolyPart;
    myOldContext.ContextGetPolySubFromSuperContextNoFailure(newContext, subPolyPart);
    Weight<Polynomial<Rational> > theWeight=this->GetValue<Weight<Polynomial<Rational> > >();
    for (int i=0; i<theWeight.weightFundamentalCoordS.size; i++)
      theWeight.weightFundamentalCoordS[i].Substitution(subPolyPart);
    return this->AssignValueWithContext(theWeight, inputOutputMinContext, *this->owner);
  }
  if (this->IsOfType<Matrix<RationalFunctionOld> >())
  { Matrix<RationalFunctionOld> newMat=this->GetValue<Matrix< RationalFunctionOld> >();
    //stOutput << "<br>Subbing " << polySub.ToString() << " in "
    PolynomialSubstitution<Rational> subPolyPart;
    //<< newPoly.ToString();
    myOldContext.ContextGetPolySubFromSuperContextNoFailure<Rational>(newContext, subPolyPart);
    for (int i=0; i< newMat.NumRows; i++)
      for (int j=0; j<newMat.NumCols; j++)
        if (!newMat(i,j).Substitution(subPolyPart))
          return *this->owner << "Failed to carry out the substitution "
          << subPolyPart.ToString() << " in the matrix " << this->ToString() << ". ";
    return this->AssignValueWithContext(newMat, inputOutputMinContext, *this->owner);
  }
  this->owner->Comments << "Expression " << this->ToString()
  << " is of built-in type but is not handled by Expression::SetContextAtLeastEqualTo. ";
  return false;
}

bool Expression::MergeContexts(Expression& leftE, Expression& rightE)
{ MacroRegisterFunctionWithName("Expression::MergeContexts");
  if (!leftE.HasContext() || !rightE.HasContext())
    return false;
  Expression leftC=leftE.GetContext();
  Expression rightC=rightE.GetContext();
  Expression outputC;
  if (!leftC.ContextMergeContexts(leftC, rightC, outputC))
    return false;
  if (!leftE.SetContextAtLeastEqualTo(outputC))
    return false;
  return rightE.SetContextAtLeastEqualTo(outputC);
}

Expression Expression::ContextGetContextVariable(int variableIndex)const
{ Expression thePolVars=this->ContextGetPolynomialVariables();
  if (thePolVars.children.size<=variableIndex+1)
  { Expression errorE;
    std::stringstream out;
    out << "Context ~does ~not ~have ~variable ~index ~" << variableIndex+1 << ". ";
    errorE.MakeError(out.str(), *this->owner);
    return errorE;
  }
  return thePolVars[variableIndex+1];
}

bool Expression::ContextSetSSLieAlgebrA(int indexInOwners, Calculator& owner)
{ if (!this->IsContext())
    crash << "This is a programming error: calling Expression::ContextSetSSLieAlgebrA on a non-context expression. " << crash;
  int index=this->ContextGetIndexAmbientSSalg();
  if (index!=-1 && index!=indexInOwners)
    return false;
  if (index==indexInOwners)
    return true;
  Expression LieAlgContextE;
  Expression emptyContext;
  emptyContext.MakeEmptyContext(owner);
  LieAlgContextE.reset(owner, 3);
  LieAlgContextE.AddChildAtomOnTop(owner.opSSLieAlg());
  LieAlgContextE.AddChildOnTop(emptyContext);
  LieAlgContextE.AddChildAtomOnTop(indexInOwners);
  return this->AddChildOnTop(LieAlgContextE);
}

bool Expression::ContextSetDiffOperatorVar(const Expression& thePolyVar, const Expression& theDiffOpVar)
{ if (!this->IsContext())
    crash << "This is a programming error: calling Expression::ContextSetDiffOperatorVar on a non-context expression. " << crash;
  Expression diffVarsE, polyVarsE;
  diffVarsE.reset(*this->owner, 2);
  diffVarsE.AddChildAtomOnTop(this->owner->opWeylAlgebraVariables());
  diffVarsE.AddChildOnTop(theDiffOpVar);
  polyVarsE.reset(*this->owner, 2);
  polyVarsE.AddChildAtomOnTop(this->owner->opPolynomialVariables());
  polyVarsE.AddChildOnTop(thePolyVar);
  bool foundDiffVarsE=false;
  bool foundPolyVarsE=false;
  for (int i=0; i<this->children.size; i++)
    if ((*this)[i].StartsWith(this->owner->opWeylAlgebraVariables()))
    { this->SetChilD(i, diffVarsE);
      foundDiffVarsE=true;
    } else if ((*this)[i].StartsWith(this->owner->opPolynomialVariables()))
    { this->SetChilD(i, polyVarsE);
      foundPolyVarsE=true;
    }
  if (!foundPolyVarsE)
    this->AddChildOnTop(polyVarsE);
  if (!foundDiffVarsE)
    this->AddChildOnTop(diffVarsE);
  return true;
}

Expression Expression::ContextGetDifferentialOperatorVariables()const
{ this->CheckInitialization();
  return this->ContextGetContextType(this->owner->opWeylAlgebraVariables());
}

Expression Expression::ContextGetPolynomialVariables()const
{ this->CheckInitialization();
  return this->ContextGetContextType(this->owner->opPolynomialVariables());
}

Expression Expression::ContextGetContextType(int theType)const
{ MacroRegisterFunctionWithName("Expression::ContextGetPolynomialVariables");
  this->CheckInitialization();
  if (!this->IsContext())
  { //crash << "This is a programming error: calling Expression::ContextGetPolynomialVariables"
    //<< " on a non-context expression. "
    //<< crash;
  }
  for (int i=1; i<this->children.size; i++)
    if ((*this)[i].StartsWith(theType))
      return (*this)[i];
  Expression output;
  output.reset(*this->owner, 1);
  output.AddChildAtomOnTop(theType);
  return output;
}

bool Expression::ContextMergeContexts(const Expression& leftContext, const Expression& rightContext, Expression& outputContext)
{ MacroRegisterFunctionWithName("Expression::ContextMergeContexts");
  if (&leftContext==&outputContext || &rightContext==&outputContext)
  { Expression leftCopy=leftContext;
    Expression rightCopy=rightContext;
    return Expression::ContextMergeContexts(leftCopy, rightCopy, outputContext);
  }
  int leftSSindex=leftContext.ContextGetIndexAmbientSSalg();
  int rightSSindex=rightContext.ContextGetIndexAmbientSSalg();
//  stOutput << "<br>left, right semisimple Lie algebra indices extracted from contexts " << leftContext.ToString() << " and "
//  << rightContext.ToString() << " are " << leftSSindex << ", " << rightSSindex << ". ";
  if (leftSSindex==-1)
    leftSSindex=rightSSindex;
  if (rightSSindex==-1)
    rightSSindex=leftSSindex;
  if (leftSSindex!=rightSSindex)
    return false;
//  stOutput << "<hr>Merging contexts: " << leftContext.ToString() << " and " << rightContext.ToString();
  Expression leftPolyV=leftContext.ContextGetPolynomialVariables();
  Expression rightPolyV=rightContext.ContextGetPolynomialVariables();
//  stOutput << "<br>Merging: leftPolyV: " << leftPolyV.ToString() << " and rightPolyV: " << rightPolyV.ToString();
  HashedList<Expression> polyVarUnion;
  MemorySaving<HashedList<Expression> > EWAVarUnion;
  polyVarUnion.SetExpectedSize(leftPolyV.children.size+rightPolyV.children.size-2);
  for (int i=1; i<leftPolyV.children.size; i++)
    polyVarUnion.AddOnTopNoRepetition(leftPolyV[i]);
  for (int i =1; i<rightPolyV.children.size; i++)
    polyVarUnion.AddOnTopNoRepetition(rightPolyV[i]);
  polyVarUnion.QuickSortAscending();
//  stOutput << "<br>Polyvarunion is: " << polyVarUnion.ToString();
  Calculator& owner=*leftContext.owner;
  outputContext.reset(owner, 5+polyVarUnion.size);
  outputContext.AddChildAtomOnTop(owner.opContexT());
  if (polyVarUnion.size>0)
  { Expression polyVarsE;
    polyVarsE.reset(owner, polyVarUnion.size+1);
    polyVarsE.AddChildAtomOnTop(owner.opPolynomialVariables());
    for (int i=0; i<polyVarUnion.size; i++)
      polyVarsE.AddChildOnTop(polyVarUnion[i]);
    outputContext.AddChildOnTop(polyVarsE);
  }
  //Converting differential operators if needed.
  Expression leftEWAV=leftContext.ContextGetDifferentialOperatorVariables();
  Expression rightEWAV=rightContext.ContextGetDifferentialOperatorVariables();
  if (leftEWAV.children.size>1 || rightEWAV.children.size>1)
  { Selection foundEWAVar;
    List<Expression> EWAVars;
    foundEWAVar.init(polyVarUnion.size);
    EWAVars.SetSize(polyVarUnion.size);
    Expression* currentPolyV=&leftPolyV;
    Expression* currentEWAV=&leftEWAV;
    for (int k=0; k<2; k++, currentPolyV=&rightPolyV, currentEWAV=&rightEWAV)
      for (int i=1; i<currentEWAV->children.size; i++)
      { int theIndex=polyVarUnion.GetIndex((*currentPolyV)[i]);
        if (foundEWAVar.selected[theIndex])
          if ((*currentEWAV)[i]!=EWAVars[theIndex])
            return owner << "<hr>Failed to merge contexts " << leftContext.ToString() << " and " << rightContext.ToString()
            << " because " << (*currentPolyV)[i].ToString() << " has two different corresponding differential operator variables: "
            << EWAVars[theIndex].ToString() << " and " << (*currentEWAV)[i].ToString();
        foundEWAVar.AddSelectionAppendNewIndex(theIndex);
        EWAVars[theIndex]=(*currentEWAV)[i];
      }
    for (int i=0; i<foundEWAVar.MaxSize; i++)
      if (!foundEWAVar.selected[i])
      { Expression currentEWAVar;
        currentEWAVar.reset(owner, 2);
        currentEWAVar.AddChildAtomOnTop(owner.AddOperationNoRepetitionOrReturnIndexFirst("\\partial"));
        Expression indexE;
        indexE.AssignValue(i, owner);
        currentEWAVar.AddChildOnTop(indexE);
        if (EWAVars.Contains(currentEWAVar))
          return owner << "<hr>Failed to merge contexts " << leftContext.ToString() << " and " << rightContext.ToString()
          << ": " << polyVarUnion[i].ToString() << " had no differential letter assigned to it. I tried to assign automatically  "
          << currentEWAVar.ToString() << " as differential operator letter, but it was already taken. ";
      }
    Expression diffVarsE;
    diffVarsE.reset(owner, polyVarUnion.size+1);
    diffVarsE.AddChildAtomOnTop(owner.opWeylAlgebraVariables());
    for (int i=0; i<EWAVars.size; i++)
      diffVarsE.AddChildOnTop(EWAVars[i]);
    outputContext.AddChildOnTop(diffVarsE);
  }
  if (leftSSindex!=-1)
  { Expression ssAlgE;
    Expression emptyContext;
    emptyContext.MakeEmptyContext(owner);
    ssAlgE.reset(owner, 2);
    ssAlgE.AddChildAtomOnTop(owner.opSSLieAlg());
    ssAlgE.AddChildOnTop(emptyContext);
    ssAlgE.AddChildAtomOnTop(leftSSindex);
    outputContext.AddChildOnTop(ssAlgE);
  }
//  stOutput << "<br>The result of the merge is: " << outputContext.ToString();
  return true;
}

void Expression::ContextGetFormatExpressions(FormatExpressions& output)const
{ output.polyAlphabeT.SetSize(0);
  output.polyDefaultLetter="x";
  if (this->owner==0)
    return;
  if (!this->StartsWith(this->owner->opContexT()))
    return;
  Expression thePolyE=this->ContextGetPolynomialVariables();
  output.polyAlphabeT.SetSize(thePolyE.children.size-1);
  for (int i=1; i<thePolyE.children.size; i++)
    output.polyAlphabeT[i-1]=thePolyE[i].ToString();
//    stOutput << "Drum roll: the poly alphabet: " << output.polyAlphabeT;
  Expression theEWAE=this->ContextGetDifferentialOperatorVariables();
  output.weylAlgebraLetters.SetSize(theEWAE.children.size-1);
  for (int i=1; i<theEWAE.children.size; i++)
    output.weylAlgebraLetters[i-1]=theEWAE[i].ToString();
}

bool Expression::GetFreeVariables(HashedList<Expression>& outputAccumulateFreeVariables, bool excludeNamedConstants)const
{ MacroRegisterFunctionWithName("Expression::GetFreeVariables");
  if (this->owner==0)
    return true;
  RecursionDepthCounter(&this->owner->RecursionDeptH);
  if (this->owner->RecursionDepthExceededHandleRoughly("In Expression::IsDifferentialOneFormOneVariable:"))
    return false;
  if (this->IsBuiltInType()) //<- this may need to be rewritten as some built in types will store free variables in their context.
    return true;
  if (this->IsAtom())
  { bool doAddExpression=!this->IsKnownFunctionWithComplexRange();
    if (doAddExpression && excludeNamedConstants)
      if (this->owner->knownDoubleConstants.Contains(*this))
        doAddExpression=false;
    if (doAddExpression)
      outputAccumulateFreeVariables.AddOnTopNoRepetition(*this);
    return true;
  }
  for (int i=0; i<this->children.size; i++)
    if (!((*this)[i].GetFreeVariables(outputAccumulateFreeVariables, excludeNamedConstants)))
      return false;
  return true;
}

bool Expression::IsDifferentialOneFormOneVariable(Expression* outputDifferentialOfWhat, Expression* outputCoeffInFrontOfDifferential)const
{ MacroRegisterFunctionWithName("Expression::IsDifferentialOneFormOneVariable");
  if (this->owner==0)
    return false;
  RecursionDepthCounter(&this->owner->RecursionDeptH);
  if (this->owner->RecursionDepthExceededHandleRoughly("In Expression::IsDifferentialOneFormOneVariable:"))
    return false;
  if (this->StartsWith(this->owner->opPlus(), 3))
  { Expression leftDiff, rightDiff, leftCoeff, rightCoeff;
    if (!(*this)[1].IsDifferentialOneFormOneVariable(&leftDiff, &leftCoeff))
      return false;
    if (!(*this)[2].IsDifferentialOneFormOneVariable(&rightDiff, &rightCoeff))
      return false;
    if (leftDiff!=rightDiff)
      return false;
    if (outputDifferentialOfWhat!=0)
      *outputDifferentialOfWhat=leftDiff;
    if (outputCoeffInFrontOfDifferential!=0)
      *outputCoeffInFrontOfDifferential=leftCoeff+rightCoeff;
    return true;
  }
  if (this->StartsWith(this->owner->opTimes(), 3))
  { if (!(*this)[1].IsDifferentialOneFormOneVariable(outputDifferentialOfWhat))
    { if (outputCoeffInFrontOfDifferential!=0)
        *outputCoeffInFrontOfDifferential=(*this)[1];
      Expression secondCoefficientOfDifferential;
      if((*this)[2].IsDifferentialOneFormOneVariable(outputDifferentialOfWhat, &secondCoefficientOfDifferential))
      { if (outputCoeffInFrontOfDifferential!=0)
          *outputCoeffInFrontOfDifferential*=secondCoefficientOfDifferential;
        return true;
      }
      return false;
    }
    if (outputCoeffInFrontOfDifferential!=0)
      *outputCoeffInFrontOfDifferential=(*this)[2];
    return !(*this)[2].IsDifferentialOneFormOneVariable();
  }
  if (this->StartsWith(this->owner->opDivide(), 3))
  { if (!(*this)[1].IsDifferentialOneFormOneVariable(outputDifferentialOfWhat))
      return false;
    if (outputCoeffInFrontOfDifferential!=0)
    { outputCoeffInFrontOfDifferential->reset(*this->owner);
      *outputCoeffInFrontOfDifferential=1;
      *outputCoeffInFrontOfDifferential/=(*this)[2];
    }
    return !(*this)[2].IsDifferentialOneFormOneVariable();
  }
  std::string theDiff;
  if (this->IsAtom(&theDiff))
    if (theDiff.size()>1)
      if (theDiff[0]=='d')
      { if (outputDifferentialOfWhat!=0)
          outputDifferentialOfWhat->MakeAtom(theDiff.substr(1, std::string::npos), *this->owner);
        if (outputCoeffInFrontOfDifferential!=0)
          outputCoeffInFrontOfDifferential->AssignValue(1, *this->owner);
//        stOutput << "theDiff string: " << theDiff;
        return true;
      }
//  stOutput << "theDiff string: " << theDiff;
  if (!this->IsListNElements(2))
    return false;
  if ((*this)[0]!="\\diff")
    return false;
  if (outputDifferentialOfWhat!=0)
    *outputDifferentialOfWhat=(*this)[1];
  if (outputCoeffInFrontOfDifferential!=0)
    outputCoeffInFrontOfDifferential->AssignValue(1, *this->owner);
  return true;
}

bool Expression::ContainsAsSubExpression(const Expression& input)const
{ if (this->owner==0)
    return false;
  RecursionDepthCounter theCounter(&this->owner->RecursionDeptH);
  if (this->owner->RecursionDepthExceededHandleRoughly("In Expression::ContainsAsSubExpression: "))
    return false;
  if (*this==input)
    return true;
  for (int i=0; i<this->children.size; i++)
    if ((*this)[i].ContainsAsSubExpression(input))
      return true;
  return false;
}

bool Expression::ContainsAsSubExpression(int inputAtom)const
{ if (this->owner==0)
    return false;
  Expression theE;
  theE.MakeAtom(inputAtom, *this->owner);
  return this->ContainsAsSubExpression(theE);
}

bool Expression::IsContext()const
{ if (this->owner==0)
    return false;
  return this->StartsWith(this->owner->opContexT());
}

bool Expression::IsError(std::string* outputErrorMessage)const
{ if (this->owner==0)
    return false;
  if (!this->StartsWith(this->owner->opError(), 2))
    return false;
  return (*this)[1].IsOfType<std::string>(outputErrorMessage);
}

bool Expression::IsSequenceNElementS(int N)const
{ if (this->owner==0)
    return false;
  return this->StartsWith(this->owner->opSequence(), N+1);
}

bool Expression::IsEqualToOne()const
{ int theInt;
  if (this->IsSmallInteger(&theInt))
    return theInt==1;
  return false;
}

bool Expression::IsEqualToMOne()const
{ MacroRegisterFunctionWithName("Expression::IsEqualToMOne");
  if (!this->IsConstantNumber())
    return false;
  if (this->IsOfType<Rational>())
    return this->GetValue<Rational>()==-1;
  return false;
}

bool Expression::IsMonEqualToZero()const
{ return this->IsEqualToZero();
}

bool Expression::IsEqualToZero()const
{ if (this->IsOfType<Rational>())
    return this->GetValue<Rational>().IsEqualToZero();
  if (this->IsOfType<double>())
    return this->GetValue<double>()==0;
  if (this->IsOfType<AlgebraicNumber>())
    return this->GetValue<AlgebraicNumber>().IsEqualToZero();
  if (this->IsOfType<Polynomial<Rational> >())
    return this->GetValue<Polynomial<Rational> >().IsEqualToZero();
  if (this->IsOfType<RationalFunctionOld>())
    return this->GetValue<RationalFunctionOld>().IsEqualToZero();
  return false;
}

bool Expression::MakeError(const std::string& theError, Calculator& owner)
{ this->reset(owner, 2);
  this->AddChildAtomOnTop(owner.opError());
  return this->AddChildValueOnTop(theError);
}

bool Expression::IsSmallInteger(int* whichInteger)const
{ Rational theRat;
  if (this->IsOfType<Rational>(&theRat))
    return theRat.IsSmallInteger(whichInteger);
  AlgebraicNumber theAlgNum;
  if (this->IsOfType<AlgebraicNumber>(&theAlgNum))
    return theAlgNum.IsSmallInteger(whichInteger);
  return false;

}

bool Expression::IsRational(Rational* whichRational)const
{ if (this->IsOfType<Rational>(whichRational))
    return true;
  AlgebraicNumber theAlgNum;
  if (this->IsOfType<AlgebraicNumber>(&theAlgNum))
    return theAlgNum.IsRational(whichRational);
  return false;
}

bool Expression::DivisionByMeShouldBeWrittenInExponentForm()const
{ if (this->owner==0)
    return false;
  RecursionDepthCounter theCounter(&this->owner->RecursionDeptH);
  if (this->owner->RecursionDepthExceededHandleRoughly())
    return false;
  if (this->StartsWith(this->owner->opTimes()))
    for (int i=1; i<this->children.size; i++)
      if (!(*this)[i].DivisionByMeShouldBeWrittenInExponentForm())
        return false;
  if (this->StartsWith(this->owner->opThePower(),3))
    return (*this)[1].DivisionByMeShouldBeWrittenInExponentForm();
  if (this->StartsWith(this->owner->opPlus()))
    return false;
  return true;
}

bool Expression::IsConstantNumber()const
{ MacroRegisterFunctionWithName("Expression::IsConstantNumber");
//  stOutput << "<br>Testing whether " << this->ToString() << " is constant.";
  if (this->owner==0)
    return false;
  RecursionDepthCounter thecounter(&this->owner->RecursionDeptH);
  if (this->owner->RecursionDepthExceededHandleRoughly("In Expression::IsConstantNumber: "))
    return false;
  if (this->IsOfType<Rational>() || this->IsOfType<AlgebraicNumber>() || this->IsOfType<double>())
    return true;
  if (this->IsAtomGivenData(this->owner->opPi()))
    return true;
  if (this->IsAtomGivenData(this->owner->opE()))
    return true;
  if (this->StartsWithFunctionWithComplexRange())
  { for (int i=1; i<this->children.size; i++)
      if (!(*this)[i].IsConstantNumber())
        return false;
    return true;
  }
//  stOutput << this->Lispify() << " is ... not a constant apparently!";
  return false;
}

bool Expression::IsInteger(LargeInt* whichInteger)const
{ Rational theRat;
  if (!this->IsOfType<Rational>(&theRat))
    return false;
  return theRat.IsInteger(whichInteger);
}

bool Expression::IsIntegerFittingInInt(int* whichInteger)const
{ Rational theRat;
//  stOutput << "Is integer fitting in int step 1: ";
  if (!this->IsOfType<Rational>(&theRat))
    return false;
//  stOutput << "Is integer fitting in int step 2: " << theRat.ToString();
  return theRat.IsIntegerFittingInInt(whichInteger);
}

template <class coefficient>
bool Expression::ContextGetPolySubFromSuperContextNoFailure(const Expression& largerContext, PolynomialSubstitution<coefficient>& output)const
{ bool mustBeTrue= this->ContextGetPolySubFromSuperContext(largerContext, output);
  if (!mustBeTrue)
    crash << "This is a programming error: I was not able to extract a polynomial substitution from smaller context "
    << this->ToString() << " relative to larger context " << largerContext.ToString() << ". " << crash;
  return mustBeTrue;
}

template <class coefficient>
bool Expression::ContextGetPolySubFromSuperContext(const Expression& largerContext, PolynomialSubstitution<coefficient>& output)const
{ Expression polyVarsElargerContext=largerContext.ContextGetPolynomialVariables();
  Expression polyVarsEsmallContext=this->ContextGetPolynomialVariables();
  output.SetSize(polyVarsElargerContext.children.size-1);
  int numVars=polyVarsElargerContext.children.size-1;
  for (int i=1; i<polyVarsEsmallContext.children.size; i++)
  { int theNewIndex=polyVarsElargerContext.GetIndexChild(polyVarsEsmallContext[i]);
    if (theNewIndex==-1)
      return false;
    output[i-1].MakeMonomiaL(theNewIndex-1, 1, 1, numVars);
  }
  return true;
}

bool Expression::ContextGetPolyAndEWASubFromSuperContextNoFailure
(const Expression& largerContext, PolynomialSubstitution<Rational>& outputPolyPart, PolynomialSubstitution<Rational>& outputEWApart)const
{ bool mustBeTrue= this->ContextGetPolyAndEWASubFromSuperContext(largerContext, outputPolyPart, outputEWApart);
  if (!mustBeTrue)
    crash << "This is a programming error: I was not able to extract a polynomial/differential operator substitution from smaller context "
    << this->ToString() << " relative to larger context " << largerContext.ToString() << crash;
  return mustBeTrue;
}

bool Expression::ContextGetPolyAndEWASubFromSuperContext
(const Expression& largerContext, PolynomialSubstitution<Rational>& outputPolyPart, PolynomialSubstitution<Rational>& outputEWApart)const
{ if(!this->ContextGetPolySubFromSuperContext(largerContext, outputPolyPart))
    return false;
  Expression EWAVarsElargerContext=largerContext.ContextGetDifferentialOperatorVariables();
  Expression EWAVarsEsmallContext=this->ContextGetDifferentialOperatorVariables();
  outputEWApart.SetSize(EWAVarsElargerContext.children.size-1);
  int numVars=EWAVarsElargerContext.children.size-1;
  for (int i=1; i<EWAVarsEsmallContext.children.size; i++)
  { int theNewIndex=EWAVarsElargerContext.GetIndexChild(EWAVarsEsmallContext[i]);
    if (theNewIndex==-1)
      return false;
    outputEWApart[i-1].MakeMonomiaL(theNewIndex-1, 1, 1, numVars);
  }
  return true;
}

bool Expression::MakeAtom(const std::string& atomName, Calculator& newBoss)
{ this->reset(newBoss);
  this->theData=newBoss.AddOperationNoRepetitionOrReturnIndexFirst(atomName);
  return true;
}

int Expression::ContextGetIndexAmbientSSalg()const
{ //stOutput << "<hr>trying to get ambient algebra from " << this->ToString();
  if (!this->IsContext() )
    return -1;
//  stOutput << ". I have " << this->children.size << " children. ";
  for (int i=1; i<this->children.size; i++)
    if ((*this)[i].StartsWith(this->owner->opSSLieAlg(), 3))
      return (*this)[i][2].theData;
  return -1;
}

void Expression::GetBaseExponentForm(Expression& outputBase, Expression& outputExponent)const
{ this->CheckInitialization();
  if (this->StartsWith(this->owner->opThePower(), 3))
  { outputBase=(*this)[1];
    outputExponent=(*this)[2];
    return;
  }
  outputBase=*this;
  outputExponent.AssignValue(1, *this->owner);
}

Expression Expression::GetContext()const
{ this->CheckInitialization();
  if (this->IsBuiltInType())
    return (*this)[1];
  crash << "This is a programming error: GetContext called on an Expression"
  << " that is not a built-in data type. "
  << " I can't display the expression as this may cause ``infinite'' "
  << "recursion if the error is caused by the ToString method. Here is however the lisp form "
  << this->ToStringFull() << " of the expression. " << "Here's  a stack trace. " << crash;
  Expression output;
  output.MakeEmptyContext(*this->owner);
  return output;
}

int Expression::GetNumContextVariables()const
{ return this->GetContext().ContextGetPolynomialVariables().children.size-1;
}

int Expression::GetNumCols()const
{ if (!this->IsSequenceNElementS() || this->format!=this->formatMatrix)
    return -1;
  int theMax=1;
  for (int i=1; i<this->children.size; i++)
    if ((*this)[i].format==this->formatMatrixRow &&
        (*this)[i].IsSequenceNElementS())
      theMax=MathRoutines::Maximum((*this)[i].children.size-1, theMax);
  return theMax;
}

void Expression::GetCoefficientMultiplicandForm(Expression& outputCoeff, Expression& outputNoCoeff)const
{ MacroRegisterFunctionWithName("Expression::GetCoefficientMultiplicandForm");
  this->CheckInitialization();
  if (this->StartsWith(this->owner->opTimes(), 3))
    if ((*this)[1].IsConstantNumber())
    { outputNoCoeff=(*this)[2];
      outputCoeff=(*this)[1];
      return;
    }
  outputCoeff.AssignValue(1, *this->owner);
  outputNoCoeff=*this;
}

void Expression::GetCoefficientMultiplicandForm(Rational& outputCoeff, Expression& outputNoCoeff)const
{ this->CheckInitialization();
  if (this->StartsWith(this->owner->opTimes(), 3))
    if ((*this)[1].IsOfType(&outputCoeff))
    { outputNoCoeff=(*this)[2];
      return;
    }
  outputCoeff=1;
  outputNoCoeff=*this;
  return;
}

int Expression::GetExpressionTreeSize()const
{ if (this->owner==0)
    return 1;
  this->CheckInitialization();
  RecursionDepthCounter theCounter(&this->owner->RecursionDeptH);
  if (this->owner->RecursionDepthExceededHandleRoughly("While computing Expression::GetExpressionTreeSize: "))
    return 1;
  if (this->IsAtom())
    return 1;
  int result=0;
  for (int i=0; i<this->children.size; i++)
    result+=(*this)[i].GetExpressionTreeSize();
  return result;
}

bool Expression::operator>(const Expression& other)const
{ Rational leftCoeff, rightCoeff;
  Expression leftMon, rightMon;
  this->GetCoefficientMultiplicandForm(leftCoeff, leftMon);
  other.GetCoefficientMultiplicandForm(rightCoeff, rightMon);
  if (leftMon==rightMon)
    return leftCoeff>rightCoeff;
  return leftMon.GreaterThanNoCoeff(rightMon);
}

bool Expression::GreaterThanNoCoeff(const Expression& other)const
{ MacroRegisterFunctionWithName("Expression::GreaterThanNoCoeff");
  if (this->IsOfType<Rational>() && other.IsOfType<Rational>())
    return this->GetValue<Rational>()>other.GetValue<Rational>();
  if (this->IsBuiltInType() && !other.IsBuiltInType())
    return false;
  if (!this->IsBuiltInType() && other.IsBuiltInType())
    return true;
//  stOutput << "<br>Comparing: " << this->ToString() << " with " << other.ToString();
  int thisExpressionTreeSize=this->GetExpressionTreeSize();
  int otherExpressionTreeSize=other.GetExpressionTreeSize();
  if (thisExpressionTreeSize>otherExpressionTreeSize)
  { //stOutput << "we have that thisExpressionTreeSize>otherExpressionTreeSize";
    return true;
  }
  if (otherExpressionTreeSize>thisExpressionTreeSize)
  { //stOutput << "we have that otherExpressionTreeSize>thisExpressionTreeSize";
    return false;
  }
  if (this->children.size==0 && other.children.size==0)
  { std::string leftS, rightS;
    if (this->IsAtom(&leftS))
      if (other.IsAtom(&rightS))
        return leftS>rightS;
    return this->theData>other.theData;
  }
//  stOutput << "<br>Got to here";
  if (this->children.size>other.children.size)
    return true;
  if (this->children.size<other.children.size)
    return false;
  for (int i=0; i<this->children.size; i++)
    if ((*this)[i]>other[i])
      return true;
    else if (other[i]>(*this)[i])
      return false;
  return false;
}

bool Expression::ToStringData(std::string& output, FormatExpressions* theFormat)const
{ MacroRegisterFunctionWithName("Expression::ToStringData");
  if (this->owner==0)
    return "(non-initialized)";
  std::stringstream out;
  bool result=false;
  bool isFinal=theFormat==0 ? false : theFormat->flagExpressionIsFinal;
  MemorySaving<FormatExpressions> contextFormat;
  bool showContext= this->owner==0 ? false : owner->flagDisplayContext;
  if (this->IsAtom())
  { if (this->owner->flagUseLnInsteadOfLog && this->IsAtomGivenData(this->owner->opLog()))
      out << "\\ln";
    else if (this->theData< this->owner->GetOperations().size && this->theData>=0)
      out << this->owner->GetOperations()[this->theData];
    else
      out << "(unknown~ atom~ of~ value~ " << this->theData << ")";
    result=true;
  } else if (this->IsOfType<std::string>())
  { if (isFinal)
      out << this->GetValue<std::string>();
    else
    { out << "(string~ not~ shown~ to~ avoid~ javascript~ problems~ (in~ case~ the~ string~ has~ javascript))";
      //out << CGI::GetStackTraceEtcErrorMessage(__FILE__, __LINE__);
    }
    result=true;
  } else if (this->IsOfType<Rational>())
  { if (this->HasNonEmptyContext())
      out << "Rational{}(" << (*this)[1].ToString() << ", ";
    if (!this->owner->flagUseFracInRationalLaTeX)
      out << this->GetValue<Rational>().ToString();
    else
      out << this->GetValue<Rational>().ToStringFrac();
    if (this->HasNonEmptyContext())
      out << ")";
    result=true;
  } else if (this->IsOfType<ElementZmodP>())
  { out << this->GetValue<ElementZmodP>().ToString();
    result=true;
  } else if (this->IsOfType<GroupRepresentation<FiniteGroup<ElementHyperoctahedralGroupR2>, Rational> >())
  { out << this->GetValue<GroupRepresentation<FiniteGroup<ElementHyperoctahedralGroupR2>, Rational> >().ToString();
    result=true;
  } else if (this->IsOfType<ElementHyperoctahedralGroupR2>())
  { //stOutput << "got here!!!!";
    out << this->GetValue<ElementHyperoctahedralGroupR2>().ToString(&contextFormat.GetElement());
    result=true;
  } else if (this->IsOfType<Polynomial<Rational> >())
  { this->GetContext().ContextGetFormatExpressions(contextFormat.GetElement());
    contextFormat.GetElement().flagUseFrac=true;
    out << "Polynomial{}(";
    out << this->GetValue<Polynomial<Rational> >().ToString(&contextFormat.GetElement()) << ")";
    if (showContext)
      out << "[" << this->GetContext().ToString() << "]";
    result=true;
  } else if (this->IsOfType<Polynomial<AlgebraicNumber> >())
  { this->GetContext().ContextGetFormatExpressions(contextFormat.GetElement());
    contextFormat.GetElement().flagUseFrac=true;
    out << "PolynomialAlgebraicNumbers{}(";
    std::string currentString=this->GetValue<Polynomial<AlgebraicNumber> >().ToString(&contextFormat.GetElement());
    if (currentString.size()>0)
      if (currentString[0]=='-')
      { currentString= currentString.substr(1);
        out << "-";
      }
    if (this->owner->flagUseNumberColors)
      out << "\\color{red}{";
    out << currentString;
    if (this->owner->flagUseNumberColors)
      out << "}";
    out << ")";
    if (showContext)
      out << "[" << this->GetContext().ToString() << "]";
    result=true;
  } else if (this->IsOfType<RationalFunctionOld>())
  { this->GetContext().ContextGetFormatExpressions(contextFormat.GetElement());
    contextFormat.GetElement().flagUseFrac=true;
    out << "RationalFunction{}("
    << this->GetValue<RationalFunctionOld>().ToString(&contextFormat.GetElement()) << ")";
    if (showContext)
      out << "[" << this->GetContext().ToString() << "]";
    result=true;
  } else if (this->IsOfType<Weight<Polynomial<Rational> > >())
  { this->GetContext().ContextGetFormatExpressions(contextFormat.GetElement());
    contextFormat.GetElement().flagFormatWeightAsVectorSpaceIndex=false;
    out << this->GetValue<Weight<Polynomial<Rational> > >().ToString(&contextFormat.GetElement());
    result=true;
  } else if (this->IsOfType<SemisimpleLieAlgebra>())
  { out << "SSLieAlg{}("
    << this->GetValue<SemisimpleLieAlgebra>().GetLieAlgebraName()
    << ")";
    result=true;
  } else if (this->IsOfType<ElementUniversalEnveloping<RationalFunctionOld> >())
  { this->GetContext().ContextGetFormatExpressions(contextFormat.GetElement());
    out << "UEE{}(" //<< this->GetContext().ToString() << ", "
    << this->GetValue<ElementUniversalEnveloping<RationalFunctionOld> >().ToString(&contextFormat.GetElement())
    << ")";
    result=true;
  } else if (this->IsOfType<MatrixTensor<Rational> >())
  { this->GetContext().ContextGetFormatExpressions(contextFormat.GetElement());
    contextFormat.GetElement().flagUseLatex=true;
    contextFormat.GetElement().flagUseHTML=false;
    if (this->GetValue<MatrixTensor<Rational> >().GetMinNumColsNumRows()<20)
      out << "MatrixRationalsTensorForm{}("
      << this->GetValue<MatrixTensor<Rational> > ().ToStringMatForm(&contextFormat.GetElement())
      << ")";
    else
      out << this->GetValue<MatrixTensor<Rational> >().ToString();
    result=true;
  } else if (this->IsOfType<Matrix<Rational> >())
  { this->GetContext().ContextGetFormatExpressions(contextFormat.GetElement());
    contextFormat.GetElement().flagUseLatex=true;
    contextFormat.GetElement().flagUseHTML=false;
    out << "MakeMatrix{}(" << this->GetValue<Matrix<Rational> >().ToString(&contextFormat.GetElement()) << ")";
    result=true;
  } else if (this->IsOfType<Matrix<AlgebraicNumber> >())
  { this->GetContext().ContextGetFormatExpressions(contextFormat.GetElement());
    contextFormat.GetElement().flagUseLatex=true;
    contextFormat.GetElement().flagUseHTML=false;
    out << "MakeMatrix{}(" << this->GetValue<Matrix<AlgebraicNumber> >().ToString(&contextFormat.GetElement()) << ")";
    result=true;
  } else if (this->IsOfType<ElementTensorsGeneralizedVermas<RationalFunctionOld> >())
  { this->GetContext().ContextGetFormatExpressions(contextFormat.GetElement());
    out << "ETGVM{}(";
    out << this->GetValue<ElementTensorsGeneralizedVermas<RationalFunctionOld> >().ToString(&contextFormat.GetElement());
    out << ")";
    result=true;
  } else if (this->IsOfType<Plot>())
  { if (isFinal)
    { Plot& thePlot=this->GetValueNonConst<Plot>();
      thePlot.flagIncludeExtraHtmlDescriptions=
      (theFormat==0) ? true : theFormat->flagIncludeExtraHtmlDescriptionsInPlots;
      out << thePlot.GetPlotHtml();
      if (this->owner->flagWriteLatexPlots)
      { out << this->owner->WriteDefaultLatexFileReturnHtmlLink
        (thePlot.GetPlotStringAddLatexCommands(false), true);
        out << "<br><b>LaTeX code used to generate the output. </b><br>"
        << thePlot.GetPlotStringAddLatexCommands(true);
      }
    } else
      out << "(plot not shown)";
    result=true;
  } else if (this->IsOfType<WeylGroupData>())
  { WeylGroupData& theGroup=this->GetValueNonConst<WeylGroupData>();
    contextFormat.GetElement().flagUseLatex=true;
    contextFormat.GetElement().flagUseHTML=false;
    contextFormat.GetElement().flagUseReflectionNotation=true;
    out << theGroup.ToString(&contextFormat.GetElement());
    result=true;
  } else if (this->IsOfType<ElementWeylGroup<WeylGroupData> >())
  { const ElementWeylGroup<WeylGroupData>& theElt=this->GetValue<ElementWeylGroup<WeylGroupData> >();
    contextFormat.GetElement().flagUseLatex=true;
    contextFormat.GetElement().flagUseHTML=false;
    contextFormat.GetElement().flagUseReflectionNotation=true;
    out << theElt.ToString(&contextFormat.GetElement());
    result=true;
  } else if (this->IsOfType<GroupRepresentation<FiniteGroup<ElementWeylGroup<WeylGroupData> >, Rational> >())
  { const GroupRepresentation<FiniteGroup<ElementWeylGroup<WeylGroupData> >, Rational>& theElt=
        this->GetValue<GroupRepresentation<FiniteGroup<ElementWeylGroup<WeylGroupData> >, Rational> >();
    contextFormat.GetElement().flagUseLatex=true;
    contextFormat.GetElement().flagUseHTML=false;
    contextFormat.GetElement().flagUseReflectionNotation=true;
    out << theElt.ToString(&contextFormat.GetElement());
    result=true;
  } else if (this->IsOfType<VirtualRepresentation<FiniteGroup<ElementWeylGroup<WeylGroupData> >, Rational> >())
  { const VirtualRepresentation<FiniteGroup<ElementWeylGroup<WeylGroupData> >, Rational>& theElt=this->GetValue<VirtualRepresentation<FiniteGroup<ElementWeylGroup<WeylGroupData> >, Rational> >();
    contextFormat.GetElement().flagUseLatex=true;
    contextFormat.GetElement().flagUseHTML=false;
    contextFormat.GetElement().flagUseReflectionNotation=true;
    out << theElt.ToString(&contextFormat.GetElement());
    result=true;
  } else if (this->IsOfType<charSSAlgMod<Rational> >())
  { charSSAlgMod<Rational> theElt=this->GetValue<charSSAlgMod<Rational> >();
    out << theElt.ToString();
    result=true;
  } else if (this->IsOfType<SemisimpleSubalgebras>())
  { SemisimpleSubalgebras& theSubalgebras=this->GetValueNonConst<SemisimpleSubalgebras>();
    contextFormat.GetElement().flagUseLatex=true;
    contextFormat.GetElement().flagUseHTML=true;
    contextFormat.GetElement().flagCandidateSubalgebraShortReportOnly=false;
    contextFormat.GetElement().flagIncludeMutableInformation=false;
    contextFormat.GetElement().flagUseMathSpanPureVsMouseHover=false;
    out << theSubalgebras.ToString(&contextFormat.GetElement());
    result=true;
  } else if (this->IsOfType<double>())
  { std::string currentString=FloatingPoint::DoubleToString(this->GetValue<double>());
    if (currentString.size()>0)
      if (currentString[0]=='-')
      { currentString= currentString.substr(1);
        out << "-";
      }
    if (this->owner->flagUseNumberColors)
      out << "\\color{blue}{";
    out << currentString;
    if (this->owner->flagUseNumberColors)
      out << "}";
    //stOutput << " converting to string : " << this->GetValue<double>();
    result=true;
  } else if (this->IsOfType<AlgebraicNumber>())
  { if (this->owner->flagUseFracInRationalLaTeX)
      contextFormat.GetElement().flagUseFrac=true;
    std::string currentString= this->GetValue<AlgebraicNumber>().ToString(&contextFormat.GetElement());
    if (currentString.size()>0)
      if (currentString[0]=='-')
      { currentString= currentString.substr(1);
        out << "-";
      }
    if (this->owner->flagUseNumberColors)
      out << "\\color{red}{";
    out << currentString;
    if (this->owner->flagUseNumberColors)
      out << "}";
    result=true;
  } else if (this->IsOfType<LittelmannPath>())
  { out << this->GetValue<LittelmannPath>().ToString();
    result=true;
  } else if (this->IsOfType<Matrix<RationalFunctionOld> >())
  { this->GetContext().ContextGetFormatExpressions(contextFormat.GetElement());
    contextFormat.GetElement().flagUseHTML=false;
    contextFormat.GetElement().flagUseLatex=true;
    out << this->GetValue<Matrix<RationalFunctionOld> >().ToString(&contextFormat.GetElement());
    result=true;
  } else if (this->IsOfType<ElementWeylAlgebra<Rational> >())
  { this->GetContext().ContextGetFormatExpressions(contextFormat.GetElement());
    contextFormat.GetElement().flagUseHTML=false;
    contextFormat.GetElement().flagUseLatex=true;
    out << "ElementWeylAlgebra{}(";
    out << this->GetValue<ElementWeylAlgebra<Rational> >().ToString(&contextFormat.GetElement());
    out << ")";
    if (showContext)
      out << "[" << this->GetContext().ToString() << "]";
    result=true;
  }
  output=out.str();
  return result;
}

std::string Expression::ToStringSemiFull()const
{ std::stringstream out;
  std::string tempS;
  if (this->ToStringData(tempS))
    out << tempS << " ";
  else
    if (this->children.size>0)
    { out << "(";
      for (int i=0; i<this->children.size; i++)
      { out << (*this)[i].ToStringSemiFull();
        if (i!=this->children.size-1)
          out << ", ";
      }
      out << ")";
    }
  return out.str();
}

std::string Expression::ToStringFull()const
{ std::stringstream out;
  if (this->IsAtom())
    out << this->theData << " ";
  if (this->children.size>0)
  { out << "(";
    for (int i=0; i<this->children.size; i++)
    { out << (*this)[i].ToStringFull();
      if (i!=this->children.size-1)
        out << ", ";
    }
    out << ")";
  }
  return out.str();
}

bool Expression::NeedsParenthesisForBaseOfExponent()const
{ //stOutput << "calling Expression::NeedsParenthesisForBaseOfExponent onto: " << this->ToStringSemiFull();
  if (this->owner==0)
    return false;
  if (this->StartsWith(this->owner->opBind()))
  { //stOutput << " to get:  " << (*this)[1].IsLisT() << "<hr>";
    return (*this)[1].IsLisT();
  }
  if (this->StartsWith(this->owner->opPlus()) || this->StartsWith(this->owner->opMinus()))
    return true;
  if (this->IsOfType<Rational>())
    return this->GetValue<Rational>()<0 || !this->GetValue<Rational>().IsInteger();
  if (this->IsOfType<AlgebraicNumber>())
  { LargeInt tempI;
    if (!this->GetValue<AlgebraicNumber>().IsInteger(&tempI))
      return true;
    return tempI>0;
  }
  if (this->IsAtom() || this->children.size==0)
    return false;
  return true;
}

bool Expression::NeedsParenthesisForMultiplicationWhenSittingOnTheRightMost()const
{ if (this->owner==0)
    return false;
//  stOutput << "DEBUG: NeedsparenRightMost: " << this->ToString()  << ", Lispified: " << this->ToStringSemiFull() << "<hr>";
  if (this->StartsWith(this->owner->opPlus()) || this->StartsWith(this->owner->opMinus()))
    return true;
  if (this->IsOfType<Rational>())
    return this->GetValue<Rational>().NeedsParenthesisForMultiplication();
  if (this->IsOfType<AlgebraicNumber>())
    return this->GetValue<AlgebraicNumber>().NeedsParenthesisForMultiplication();
  if (this->IsAtom() || this->children.size==0)
    return false;
  if (this->children.size>1)
  { const Expression& firstE=(*this)[0];
    //const Expression& secondE=(*this)[1];
    if (firstE.IsAtomGivenData(this->owner->opTimes()))
      return false;
    if (firstE.IsAtomGivenData(this->owner->opThePower()))
      return false;
    if (this->StartsWith(this->owner->opBind()))
      return (*this)[1].NeedsParenthesisForMultiplication();
    return false;
  }
  return false;
}

bool Expression::NeedsParenthesisForMultiplication()const
{ if (this->owner==0)
    return false;
//  stOutput << "DEBUG: Needsparen: " << this->ToString()  << ", Lispified: " << this->ToStringSemiFull() << "<hr>";
  if (this->StartsWith(this->owner->opPlus()) || this->StartsWith(this->owner->opMinus()))
    return true;
  if (this->IsOfType<Rational>())
    return this->GetValue<Rational>().NeedsParenthesisForMultiplication();
  if (this->IsOfType<AlgebraicNumber>())
    return this->GetValue<AlgebraicNumber>().NeedsParenthesisForMultiplication();
  if (this->IsAtom() || this->children.size==0)
    return false;
  if (this->children.size>1)
  { const Expression& firstE=(*this)[0];
    //const Expression& secondE=(*this)[1];
    if (firstE.IsAtomGivenData(this->owner->opTimes()))
      return false;
    if (firstE.IsAtomGivenData(this->owner->opThePower()))
      return false;
    if (this->StartsWith(this->owner->opBind()))
      return (*this)[1].NeedsParenthesisForMultiplication();
    return true;
  }
  return false;
}

std::string Expression::ToString(FormatExpressions* theFormat, Expression* startingExpression, Expression* Context)const
{ MacroRegisterFunctionWithName("Expression::ToString");
  if (this->owner!=0)
  { if (this->owner->RecursionDeptH+1>this->owner->MaxRecursionDeptH)
      return "(...)";
  } else
    return "(Error:NoOwner)";
  RecursionDepthCounter theRecursionCounter(&this->owner->RecursionDeptH);
  this->CheckConsistency();
  int notationIndex=owner->theObjectContainer.ExpressionWithNotation.GetIndex(*this);
  if (notationIndex!=-1)
    return owner->theObjectContainer.ExpressionNotation[notationIndex];
  std::stringstream out;
//  AddBrackets=true;
//  if (this->owner->flagLogSyntaxRules && recursionDepth<=1)
//  { out << "(ContextIndex=" << this->IndexBoundVars << ")";
//  }
  bool isFinal=theFormat==0 ? false : theFormat->flagExpressionIsFinal;
  bool allowNewLine= (theFormat==0) ? false : theFormat->flagExpressionNewLineAllowed;
  bool oldAllowNewLine= (theFormat==0) ? false : theFormat->flagExpressionNewLineAllowed;
  bool useFrac =this->owner->flagUseFracInRationalLaTeX; //(theFormat==0) ? true : theFormat->flagUseFrac;
  int lineBreak=50;
  int charCounter=0;
  std::string tempS;
  if (this->ToStringData(tempS, theFormat))
    out << tempS;
  else if (this->StartsWith(this->owner->opDefine(), 3))
  { std::string firstE=(*this)[1].ToString(theFormat);
    std::string secondE=(*this)[2].ToString(theFormat);
    if ((*this)[1].IsListStartingWithAtom(this->owner->opDefine()))
      out << "(" << firstE << ")";
    else
      out << firstE;
    out << "=";
    if ((*this)[2].IsListStartingWithAtom(this->owner->opDefine()))
      out << "(" << secondE << ")";
    else
      out << secondE;
  } else if (this->IsListStartingWithAtom(this->owner->opIsDenotedBy()))
    out << (*this)[1].ToString(theFormat) << "=:" << (*this)[2].ToString(theFormat);
  else if (this->StartsWith(this->owner->opQuote(),2))
    out << "\"" << (*this)[1].ToString(theFormat) << "\"";
  else if (this->IsListStartingWithAtom(this->owner->opDefineConditional()))
    out << (*this)[1].ToString(theFormat) << " :if " << (*this)[2].ToString(theFormat) << "=" << (*this)[3].ToString(theFormat);
  else if (this->StartsWith(this->owner->opDivide(), 3))
  { bool doUseFrac= this->formatUseFrac || this->owner->flagUseFracInRationalLaTeX;
    if(!doUseFrac)
    { std::string firstE= (*this)[1].ToString(theFormat);
      std::string secondE=(*this)[2].ToString(theFormat);
      bool firstNeedsBrackets= !((*this)[1].IsListStartingWithAtom(this->owner->opTimes())|| (*this)[1].IsListStartingWithAtom(this->owner->opDivide()));
      bool secondNeedsBrackets=true;
      if ((*this)[2].IsOfType<Rational>())
        if ((*this)[2].GetValue<Rational>().IsInteger())
          secondNeedsBrackets=false;
      if (firstNeedsBrackets)
        out << "(" << firstE << ")";
      else
        out << firstE;
      out << "/";
      if (secondNeedsBrackets)
        out << "(" << secondE << ")";
      else
        out << secondE;
    } else
    { if (theFormat!=0)
        theFormat->flagExpressionNewLineAllowed=false;
      std::string firstE= (*this)[1].ToString(theFormat);
      std::string secondE=(*this)[2].ToString(theFormat);
      out << "\\frac{" << firstE << "}{" << secondE << "}";
      if (theFormat!=0)
        theFormat->flagExpressionNewLineAllowed=oldAllowNewLine;
    }
  } else if (this->StartsWith(this->owner->opTensor(), 3) )
    out << (*this)[1].ToString(theFormat) << "\\otimes " << (*this)[2].ToString(theFormat);
  else if (this->StartsWith(this->owner->opChoose(),3) )
    out << (*this)[1].ToString(theFormat) << "\\choose " << (*this)[2].ToString(theFormat);
  else if (this->StartsWith(this->owner->opTimes(), 3))
  { std::string secondE=(*this)[2].ToString(theFormat);
    if ((*this)[1].IsAtomGivenData(this->owner->opSqrt()))
      out << "\\sqrt{" << secondE << "}";
    else
    { std::string firstE= (*this)[1].ToString(theFormat);
      bool firstNeedsBrackets=(*this)[1].NeedsParenthesisForMultiplication();
      bool secondNeedsBrackets=(*this)[2].NeedsParenthesisForMultiplicationWhenSittingOnTheRightMost();
      if (firstE=="-1" )
      { firstE="-";
        firstNeedsBrackets=false;
      }
      if (firstE=="1")
        firstE="";
      if (firstNeedsBrackets)
        out << "\\left(" << firstE << "\\right)";
      else
        out << firstE;
      bool mustHaveTimes=this->format==this->formatTimesDenotedByStar && firstE!="-" && firstE!="";
      if (firstE!="")
        if (MathRoutines::isADigit(firstE[firstE.size()-1]) && MathRoutines::isADigit(secondE[0]) )
          mustHaveTimes=true;
      if (mustHaveTimes)
        out << "*";
      else
        out << " ";
      if (secondNeedsBrackets)
        out << "\\left(" << secondE << "\\right)";
      else
        out << secondE;
    }
  } else if (this->StartsWith(this->owner->opCrossProduct()))
  { std::string secondE=(*this)[2].ToString(theFormat);
    std::string firstE= (*this)[1].ToString(theFormat);
    bool firstNeedsBrackets=(*this)[1].NeedsParenthesisForMultiplication();
    bool secondNeedsBrackets=(*this)[2].NeedsParenthesisForMultiplication();
    if (firstE=="-1" )
    { firstE="-";
      firstNeedsBrackets=false;
    }
    if (firstE=="1")
      firstE="";
    if (firstNeedsBrackets)
      out << "\\left(" << firstE << "\\right)";
    else
      out << firstE;
    out << "\\times ";
    if (secondNeedsBrackets)
      out << "\\left(" << secondE << "\\right)";
    else
      out << secondE;
  } else if (this->StartsWith(this->owner->opSqrt(), 3))
  { int thePower=0;
    bool hasPowerTwo=(*this)[1].IsSmallInteger(&thePower);
    if (hasPowerTwo)
      hasPowerTwo=(thePower==2);
    if (hasPowerTwo)
      out << "\\sqrt{" << (*this)[2].ToString() << "}";
    else
      out << "\\sqrt[" << (*this)[1].ToString() << "]{" << (*this)[2].ToString() << "}";
  } else if (this->IsListStartingWithAtom(this->owner->opFactorial()))
    out << (*this)[1].ToString(theFormat) << "!";
  else if (this->StartsWith(this->owner->opThePower(), 3))
  { bool involvesExponentsInterpretedAsFunctions=false;
    const Expression& firstE=(*this)[1];
    const Expression& secondE=(*this)[2];
    if (firstE.StartsWith(-1, 2))
      if (firstE[0].IsAtomWhoseExponentsAreInterpretedAsFunction() && !secondE.IsEqualToMOne())
      { involvesExponentsInterpretedAsFunctions=true;
        Expression newE, newFunE;
        newFunE.MakeXOX(*this->owner, this->owner->opThePower(), firstE[0], (*this)[2]);
        newE.reset(*this->owner, 2);
        newE.AddChildOnTop(newFunE);
        newE.AddChildOnTop(firstE[1]);
        this->CheckConsistency();
        newE.CheckConsistency();
        newFunE.CheckConsistency();
        //stOutput << "<br> tostringing a very special case: " << newE.ToString() << " lispified: " << this->ToStringFull();
        out << newE.ToString(theFormat);
      }
    if (!involvesExponentsInterpretedAsFunctions)
    { bool isSqrt=false;
      if ((*this)[2].IsOfType<Rational>())
        if ((*this)[2].GetValue<Rational>().IsEqualTo(Rational(1,2)))
          isSqrt=true;
      if (isSqrt)
        out << "\\sqrt{" << (*this)[1].ToString(theFormat) << "}";
      else
      { std::string secondEstr=(*this)[2].ToString(theFormat);
        std::string firstEstr=(*this)[1].ToString(theFormat);
        if ((*this)[1].NeedsParenthesisForBaseOfExponent())
        { bool useBigParenthesis=false;
          if ((*this)[1].StartsWith(this->owner->opDivide()))
            useBigParenthesis=true;
          if (useBigParenthesis)
            out << "\\left(";
          else
            out << "(";
          out << firstEstr;
          if (useBigParenthesis)
            out << "\\right)";
          else
            out << ")";
        } else
          out << firstEstr;
        out << "^{" << secondEstr << "}";
      }
    }
//    stOutput << "<br>tostringing: " << out.str() << "   lispified: " << this->ToStringFull();
  } else if (this->IsListStartingWithAtom(this->owner->opPlus() ))
  { if (this->children.size<2)
      crash << "Plus operation takes at least 2 arguments. " << crash;
    std::string tempS2= (*this)[1].ToString(theFormat);
    tempS=(*this)[2].ToString(theFormat);
    out << tempS2;
    if (allowNewLine && !useFrac && tempS2.size()>(unsigned)lineBreak)
      out << "\\\\\n";
    if (tempS.size()>0)
      if (tempS[0]!='-')
        out << "+";
    out << tempS;
  } else if (this->StartsWith(this->owner->opMinus(), 2))
  { if ((*this)[1].StartsWith(this->owner->opPlus()) ||(*this)[1].StartsWith(this->owner->opMinus()) )
      out << "-\\left(" << (*this)[1].ToString(theFormat) << "\\right)";
    else
      out << "-" << (*this)[1].ToString(theFormat);
  } else if (this->StartsWith(this->owner->opSqrt(), 2))
    out << "\\sqrt{" << (*this)[1].ToString(theFormat) << "}";
  else if (this->StartsWith(this->owner->opMinus(), 3))
  { if (!(this->children.size==3))
      crash << "This is a programming error: the minus function expects 1 or 2 arguments, instead there are " << this->children.size-1 << ". " << crash;
    out << (*this)[1].ToString(theFormat) << "-" << (*this)[2].ToString(theFormat);
  } else if (this->StartsWith(this->owner->opBind(), 2))
    out << "{{" << (*this)[1].ToString(theFormat) << "}}";
  else if (this->IsListStartingWithAtom(this->owner->opApplyFunction()))
  { if (this->children.size<2)
      crash << crash;
    switch(this->format)
    { case Expression::formatFunctionUseUnderscore:
        if (allowNewLine)
          theFormat->flagExpressionNewLineAllowed=false;
        out <<  (*this)[1].ToString(theFormat) << "_{" << (*this)[2].ToString(theFormat) << "}";
        if (allowNewLine)
          theFormat->flagExpressionNewLineAllowed=true;
        break;
      case Expression::formatFunctionUseCdot:
        out << (*this)[1].ToString(theFormat) << "\\cdot\\left(" << (*this)[1].ToString(theFormat) << "\\right)";
        break;
      default:
        out << (*this)[1].ToString(theFormat) << "{}\\left(" << (*this)[2].ToString(theFormat) << "\\right)";
        break;
    }
  } else if (this->IsListStartingWithAtom(this->owner->opEqualEqual()))
    out << (*this)[1].ToString(theFormat) << "==" << (*this)[2].ToString(theFormat);
  else if (this->IsListStartingWithAtom(this->owner->opGreaterThan()))
    out << (*this)[1].ToString(theFormat) << ">" << (*this)[2].ToString(theFormat);
  else if (this->IsListStartingWithAtom(this->owner->opLimit()))
    out << "\\lim_{" << (*this)[1].ToString(theFormat) << "}" << (*this)[2].ToString(theFormat);
  else if (this->IsListStartingWithAtom(this->owner->opLimitProcess()))
    out << (*this)[1].ToString(theFormat) << " \\to " << (*this)[2].ToString(theFormat);
  else if (this->IsListStartingWithAtom(this->owner->opLessThan()))
    out << (*this)[1].ToString(theFormat) << "<" << (*this)[2].ToString(theFormat);
  else if (this->IsListStartingWithAtom(this->owner->opSequence()))
  { switch (this->format)
    { case Expression::formatMatrixRow:
        for (int i=1; i<this->children.size; i++)
        { out << (*this)[i].ToString(theFormat);
          if (i!=this->children.size-1)
            out << "& ";
        }
        break;
      case Expression::formatMatrix:
        if (theFormat!=0)
          if (theFormat->flagUseLatex)
            out << "\\left(";
        out << "\\begin{array}{";
        for (int i=0; i<this->GetNumCols(); i++)
          out << "c";
        out << "} ";
        for (int i=1; i<this->children.size; i++)
        { out << (*this)[i].ToString(theFormat);
          if (i!=this->children.size-1)
            out << "\\\\ \n";
        }
        out << " \\end{array}";
        if (theFormat!=0)
          if (theFormat->flagUseLatex)
            out << "\\right)";
        break;
      default:
        if (this->children.size==2)
          out << "{Sequence{}";
        out << "(";
        for (int i=1; i<this->children.size; i++)
        { std::string currentChildString=(*this)[i].ToString(theFormat);
          out << currentChildString;
          charCounter+=currentChildString.size();
          if (i!=this->children.size-1)
          { out << ", ";
            if (theFormat!=0)
              if (allowNewLine && charCounter >50)
              { if (theFormat->flagUseLatex)
                  out << "\\\\\n";
                if (theFormat->flagUseHTML)
                  out << "\n<br>\n";
              }
          }
          charCounter%=50;
        }
        out << ")";
        if (this->children.size==2)
          out << "}";
        break;
    }
  } else if (this->IsListStartingWithAtom(this->owner->opLieBracket()))
    out << "[" << (*this)[1].ToString(theFormat) << "," << (*this)[2].ToString(theFormat) << "]";
  else if (this->IsListStartingWithAtom(this->owner->opMod()))
    out << (*this)[1].ToString(theFormat) << " mod " << (*this)[2].ToString(theFormat);
  else if (this->IsListStartingWithAtom(this->owner->opUnion()) && this->children.size==3)
    out << (*this)[1].ToString(theFormat) << "\\cup " << (*this)[2].ToString(theFormat);
  else if (this->IsListStartingWithAtom(this->owner->opIntersection()) && this->children.size==3)
    out << (*this)[1].ToString(theFormat) << "\\cap " << (*this)[2].ToString(theFormat);
  else if (this->IsListStartingWithAtom(this->owner->opUnionNoRepetition()))
    out << (*this)[1].ToString(theFormat) << "\\sqcup " << (*this)[2].ToString(theFormat);
  else if (this->IsListStartingWithAtom(this->owner->opEndStatement()))
  { bool createTable=(startingExpression!=0);
    if (!createTable)
      out << "(";
    for (int i=1; i<this->children.size; i++)
      if (createTable)
      { out << "<tr><td valign=\"top\">";
        out << "<hr> ";
        if (!this->owner->flagHideLHS)
        { if (i<(*startingExpression).children.size)
            out << CGI::GetMathMouseHoverBeginArrayL((*startingExpression)[i].ToString(theFormat));
          else
            out << "No matching starting expression- possible use of the Melt keyword.";
        } else
          out << "...";
        if (i!=this->children.size-1)
          out << ";";
        out << "</td><td valign=\"top\"><hr>";
        if ((*this)[i].IsOfType<std::string>() && isFinal)
          out << (*this)[i].GetValue<std::string>();
        else if (((*this)[i].IsOfType<Plot> () || (*this)[i].IsOfType<SemisimpleSubalgebras>() || (*this)[i].IsOfType<WeylGroupData>()
                  || (*this)[i].IsOfType<GroupRepresentation<FiniteGroup<ElementWeylGroup<WeylGroupData> >, Rational> >()) && isFinal)
          out << (*this)[i].ToString(theFormat);
        else
          out << CGI::GetMathSpanBeginArrayL((*this)[i].ToString(theFormat), 1700);
        if (i!=this->children.size-1)
          out << ";";
        out << "</td></tr>";
      } else
      { out << (*this)[i].ToString(theFormat);
        if (i!=this->children.size-1)
          out << ";";
      }
    if (!createTable)
      out << ")";
  } else if (this->StartsWith(this->owner->opError(), 2))
  { this->owner->NumErrors++;
    out << "(Error~ " << this->owner->NumErrors << ":~ see~ comments)";
    this->owner->Comments << "<br>Error " << this->owner->NumErrors << ". " << (*this)[1].ToString(theFormat);
  } else if (this->children.size==1)
    out << (*this)[0].ToString(theFormat);
  else if (this->children.size>=2)
  { out << (*this)[0].ToString(theFormat);
    bool needParenthesis=true;
    if (this->children.size==2)
    { if ((*this)[0].IsAtomWhoseExponentsAreInterpretedAsFunction())
        needParenthesis=!(*this)[1].IsAtom();
      if ((*this)[0].IsPowerOfAtomWhoseExponentsAreInterpretedAsFunction())
        needParenthesis=!(*this)[1].IsAtom();
    }
    if (this->format==this->formatFunctionUseUnderscore)
      out << "_";
    else if (this->format==this->formatFunctionUseCdot)
      out << "\\cdot";
    else
      out << "{}";
    if(this->format== this->formatFunctionUseUnderscore)
      out << "{";
    else if (needParenthesis)
      out << "\\left(";
    for (int i=1; i<this->children.size; i++)
    { out << (*this)[i].ToString(theFormat);
      if (i!=this->children.size-1)
        out << ", ";
    }
    if(this->format== this->formatFunctionUseUnderscore)
      out << "}";
    else if (needParenthesis)
      out << "\\right)";
//    stOutput << "<br>tostringing: " << out.str() << "   lispified: " << this->ToStringFull();

  } else //<-not sure if this case is possible
    out << "(ProgrammingError:NotDocumented)" ;
  if (startingExpression!=0)
  { std::stringstream outTrue;
    outTrue << "<table>";

//    outTrue << "<tr><td colspan=\"2\"> Double click expressions to get the LaTeX code. "
//    << "Many thanks to the <a href=\"http://www.math.union.edu/~dpvc/jsmath/\">jsmath</a> project!</td></tr>";
    outTrue << "<tr><th>Input</th><th>Result</th></tr>";
    // stOutput << this->Lispify();

    if (this->IsListStartingWithAtom(this->owner->opEndStatement()))
      outTrue << out.str();
    else
    { outTrue << "<tr><td>" << CGI::GetMathSpanBeginArrayL(startingExpression->ToString(theFormat), 1700);
      if ((this->IsOfType<std::string>() || this->IsOfType<Plot>() ||
           this->IsOfType<SemisimpleSubalgebras>() || this->IsOfType<WeylGroupData>()) && isFinal)
        outTrue << "</td><td>" << out.str() << "</td></tr>";
      else
        outTrue << "</td><td>" << CGI::GetMathSpanBeginArrayL(out.str(), 1700) << "</td></tr>";
    }
    outTrue << "</table>";
    return outTrue.str();
  }
//  if (useLatex && recursionDepth==0 && this->theOperation!=owner->opEndStatement())
//    return CGI::GetHtmlMathSpanFromLatexFormula(out.str());
  return out.str();
}

std::string Expression::Lispify()const
{ if (this->owner==0)
    return "Error: not initialized";
  RecursionDepthCounter theCounter(&this->owner->RecursionDeptH);
  if (this->owner->RecursionDeptH>this->owner->MaxRecursionDeptH)
    return "(error: max recursion depth ...)";
  std::string tempS;
  if (this->ToStringData(tempS))
    return tempS;
  if (this->children.size==0)
    return this->ToString();
  std::stringstream out;
  out << "(";
  for (int i=0; i<this->children.size; i++)
    out << " " << (*this)[i].Lispify();
  out << ")";
  return out.str();
}

bool Expression::IsLisT()const
{ if (this->owner==0)
    return false;
  if (this->children.size<=0)
    return false;
  if (this->theData!=this->owner->opLisT())
    crash << "This is a programming error. List expressions must have data valule equal to Calculator::opList(). " << crash;
  return true;
}

bool Expression::IsFrozen()const
{ std::string atomName;
  if (!this->StartsWith())
    return false;
  return (*this)[0].IsAtomThatFreezesArguments();
}

bool Expression::IsAtomThatFreezesArguments(std::string* outputWhichAtom)const
{ if (this->owner==0)
    return false;
  if (this->IsLisT())
    return false;
  if (this->theData<0 || this->theData>=this->owner->GetOperations().size)
    return false;
  if (outputWhichAtom!=0)
    *outputWhichAtom=this->owner->GetOperations()[this->theData];
  return this->owner->atomsThatFreezeArguments.Contains(this->owner->GetOperations()[this->theData]);
}

bool Expression::IsPowerOfAtomWhoseExponentsAreInterpretedAsFunction()const
{ if (this->owner==0)
    return false;
  if (!this->StartsWith(this->owner->opThePower(), 3))
    return false;
  return (*this)[1].IsAtomWhoseExponentsAreInterpretedAsFunction();
}

bool Expression::IsAtomWhoseExponentsAreInterpretedAsFunction(std::string* outputWhichAtom)const
{ if (this->owner==0)
    return false;
  if (this->IsLisT())
    return false;
  if (this->theData<0 || this->theData>=this->owner->GetOperations().size)
    return false;
  if (outputWhichAtom!=0)
    *outputWhichAtom=this->owner->GetOperations()[this->theData];
  return this->owner->atomsWhoseExponentsAreInterpretedAsFunctions.Contains(this->owner->GetOperations()[this->theData]);
}

bool Expression::IsAtomNotInterpretedAsFunction(std::string* outputWhichAtom)const
{ if (this->owner==0)
    return false;
  if (this->IsLisT())
    return false;
  if (this->theData<0 || this->theData>=this->owner->GetOperations().size)
    return false;
  if (outputWhichAtom!=0)
    *outputWhichAtom=this->owner->GetOperations()[this->theData];
  return this->owner->atomsNotInterpretedAsFunctions.Contains(this->owner->GetOperations()[this->theData]);
}

bool Expression::IsAtom(std::string* outputWhichOperation)const
{ if (this->owner==0)
    return false;
  if (this->IsLisT())
    return false;
  if (outputWhichOperation!=0)
  { if (this->theData<0 || this->theData>=this->owner->GetOperations().size)
    { std::stringstream out;
      out << "(data-atom:~" << this->theData << ")";
      *outputWhichOperation= out.str();
    }
    else
      *outputWhichOperation=this->owner->GetOperations()[this->theData];
  }
  return true;
}

bool Expression::IsAtomGivenData(int desiredDataUseMinusOneForAny)const
{ if (this->IsLisT())
    return false;
  if (desiredDataUseMinusOneForAny==-1)
    return true;
  return this->theData==desiredDataUseMinusOneForAny;
}

bool Expression::IsKnownFunctionWithComplexRange(std::string* outputWhichOperation)const
{ if (this->owner==0)
    return false;
  std::string operationName;
  if (outputWhichOperation==0)
    outputWhichOperation=&operationName;
  if (!this->IsBuiltInAtom(outputWhichOperation))
    return false;
  return this->owner->knownFunctionsWithComplexRange.Contains(operationName);
}

bool Expression::IsCalculatorStatusChanger()const
{ if (this->owner==0)
    return false;
//  stOutput << "DEBUG: Calling IsCalculatorStatusChanger for: " << this->ToString();
  return
  this->StartsWith(this->owner->opDefine()) ||
  this->StartsWith(this->owner->opDefineConditional()) ||
  this->StartsWith(this->owner->opRulesChanged());
}

bool Expression::IsArithmeticOperation(std::string* outputWhichOperation)const
{ if (this->owner==0)
    return false;
  std::string operationName;
  if (outputWhichOperation==0)
    outputWhichOperation=&operationName;
  if (!this->IsBuiltInAtom(outputWhichOperation))
    return false;
  return this->owner->arithmeticOperations.Contains(operationName);
}

bool Expression::IsBuiltInAtom(std::string* outputWhichOperation)const
{ if (this->owner==0)
    return false;
  if (this->IsLisT())
    return false;
  if (this->theData<0 || this->theData>=this->owner->GetOperations().size)
    return false;
  if (this->theData>= this->owner->NumPredefinedAtoms)
    return false;
  if (outputWhichOperation!=0)
    *outputWhichOperation=this->owner->GetOperations()[this->theData];
  return true;
}

bool Expression::IsGoodForChainRuleFunction(std::string* outputWhichOperation)const
{ if (this->owner==0)
    return false;
  if (this->IsLisT())
    return false;
  if (this->theData<0 || this->theData>=this->owner->GetOperations().size)
    return false;
//  stOutput << "ere be i - checking whether operation " << this->owner->GetOperations()[this->theData] << " is good for chain rule. ";
  if (outputWhichOperation!=0)
    *outputWhichOperation=this->owner->GetOperations()[this->theData];
  return !this->owner->atomsNotAllowingChainRule.Contains(this->owner->GetOperations()[this->theData]);
}

bool Expression::RemoveContext()
{ this->CheckInitialization();
  if (!this->HasContext())
    return true;
  this->children.RemoveIndexShiftDown(1);
  if (this->children.size==1)
    this->AssignMeMyChild(0);
  return true;
}

bool Expression::HasContext()const
{ this->CheckInitialization();
  if (!this->IsBuiltInType() || !(this->children.size==3))
    return false;
  //std::string debugString=(*this)[1].ToString();
  //stOutput << "<br>Trying to fetch context from: " << debugString ;
  return (*this)[1].IsListStartingWithAtom(this->owner->opContexT());
}

bool Expression::HasNonEmptyContext()const
{ if (!this->HasContext())
    return false;
  return !this->GetContext().StartsWith(this->owner->opContexT(), 1);
}

bool Expression::IsCacheableExpression()const
{ MacroRegisterFunctionWithName("Expression::IsCacheableExpression");
  if (this->owner==0)
    return true;
  if (this->IsBuiltInType())
    return true;
  for (int i=0; i<this->children.size; i++)
    if (!(*this)[i].IsCacheableExpression())
      return false;
  if (this->children.size>0)
    return true;
  Calculator& theBoss=*this->owner;
  if (this->theData<0 || this->theData>= theBoss.theAtoms.size)
    crash << "This is a programming error: corrupted atom in Expression::IsCacheableExpression. " << crash;
  return theBoss.atomsThatMustNotBeCached.Contains(theBoss.theAtoms[this->theData]);
}

bool Expression::IsBuiltInScalar()const
{ return this->IsOfType<Rational>() || this->IsOfType<Polynomial<Rational> >()
  || this->IsOfType<RationalFunctionOld>() || this->IsOfType<double>() || this->IsOfType<AlgebraicNumber>();
}

bool Expression::IsBuiltInType(std::string* outputWhichOperation)const
{ std::string tempS;
  if (!this->StartsWith())
    return false;
//  if (this->children.size<2 || !(*this)[this->children.size-1].IsAtoM())
//    return false;
  if (!(*this)[0].IsAtom(&tempS))
    return false;
  if (this->owner->GetBuiltInTypes().Contains(tempS))
  { if (outputWhichOperation!=0)
      *outputWhichOperation=tempS;
    return true;
  }
  return false;
}

bool Expression::IsBuiltInType(int* outputWhichType)const
{ std::string theType;
  if (!this->IsBuiltInType(&theType))
    return false;
  if (outputWhichType!=0)
    *outputWhichType=this->owner->GetOperations().GetIndex(theType);
  return true;
}

bool Expression::MakeProducT(Calculator& owner, const Expression& left, const Expression& right)
{ return this->MakeXOX(owner, owner.opTimes(), left, right);
}

bool Expression::MakeProducT(Calculator& owner, const List<Expression>& theMultiplicands)
{ if (theMultiplicands.size==0)
    return this->AssignValue(1, owner);
  return this->MakeXOXOdotsOX(owner, owner.opTimes(), theMultiplicands);
}

bool Expression::MakeSum(Calculator& owner, const List<Expression>& theSummands)
{ MacroRegisterFunctionWithName("Expression::MakeSum");
  if (theSummands.size==0)
    return this->AssignValue(0, owner);
  return this->MakeXOXOdotsOX(owner, owner.opPlus(), theSummands);
}

bool Expression::MakeOXdotsX(Calculator& owner, int theOp, const List<Expression>& theOpands)
{ MacroRegisterFunctionWithName("Expression::MakeOXdotsX");
  if (theOpands.size==0)
    crash << "Zero opands not allowed at this point. " << crash;
  if (theOpands.size==1)
  { *this=theOpands[0];
    return true;
  }
  *this=*theOpands.LastObject();
  for (int i=theOpands.size-2; i>=0; i--)
    this->MakeXOX(owner, theOp, theOpands[i], *this);
  this->CheckConsistencyRecursively();
  return true;
}

void Expression::MakeFunction(Calculator& owner, const Expression& theFunction, const Expression& theArgument)
{ this->reset(owner, 2);
  this->AddChildOnTop(theFunction);
  this->AddChildOnTop(theArgument);
  this->format=this->formatFunctionUseUnderscore;
}

bool Expression::MakeEmptyContext(Calculator& owner)
{ this->reset(owner, 1);
  return this->AddChildAtomOnTop(owner.opContexT());
}

bool Expression::ContextMakeContextWithOnePolyVar(Calculator& owner, const Expression& inputPolyVarE)
{ this->MakeEmptyContext(owner);
  Expression thePolyVars;
  thePolyVars.reset(owner, 2);
  thePolyVars.AddChildAtomOnTop(owner.opPolynomialVariables());
  thePolyVars.AddChildOnTop(inputPolyVarE);
  return this->AddChildOnTop(thePolyVars);
}

bool Expression::MakeContextWithOnePolyVarOneDiffVar(Calculator& owner, const Expression& inputPolyVarE, const Expression& inputDiffVarE)
{ this->MakeEmptyContext(owner);
  return this->ContextSetDiffOperatorVar(inputPolyVarE, inputDiffVarE);
}

bool Expression::MakeContextSSLieAlg(Calculator& owner, const SemisimpleLieAlgebra& theSSLiealg)
{ this->MakeEmptyContext(owner);
  return this->ContextSetSSLieAlgebrA(owner.theObjectContainer.theLieAlgebras.AddNoRepetitionOrReturnIndexFirst(theSSLiealg), owner);
}

bool Expression::ContextMakeContextWithOnePolyVar(Calculator& owner, const std::string& inputPolyVarName)
{ Expression varNameE;
  varNameE.MakeAtom(owner.AddOperationNoRepetitionOrReturnIndexFirst(inputPolyVarName), owner);
  return this->ContextMakeContextWithOnePolyVar(owner, varNameE);
}

bool Expression::MakeSqrt(Calculator& owner, const Rational& argument, const Rational& radicalSuperIndex)
{ Expression argumentE;
  argumentE.AssignValue(argument, owner);
  return this->MakeSqrt(owner, argumentE, radicalSuperIndex);
}

bool Expression::MakeSqrt(Calculator& owner, const Expression& argument, const Rational& radicalSuperIndex)
{ this->reset(owner,3);
  Expression radicalIndexE;
  radicalIndexE.AssignValue(radicalSuperIndex, owner);
  this->AddChildAtomOnTop(owner.opSqrt());
  this->AddChildOnTop(radicalIndexE);
  return this->AddChildOnTop(argument);
}

bool Expression::MakeXOX(Calculator& owner, int theOp, const Expression& left, const Expression& right)
{ MacroRegisterFunctionWithName("Expression::MakeXOX");
  if (&left==this || &right==this)
  { Expression leftCopy=left;
    Expression rightCopy=right;
    return this->MakeXOX(owner, theOp, leftCopy, rightCopy);
  }
  left.CheckInitialization();
  right.CheckInitialization();
  this->reset(owner, 3);
  this->theData=owner.opLisT();
  this->AddChildAtomOnTop(theOp);
  this->AddChildOnTop(left);
  return this->AddChildOnTop(right);
}

bool Expression::MakeOX(Calculator& owner, int theOp, const Expression& opArgument)
{ if (&opArgument==this)
  { Expression copyE=opArgument;
    return this->MakeOX(owner, theOp, copyE);
  }
  this->reset(owner);
  this->theData=owner.opLisT();
  this->AddChildAtomOnTop(theOp);
  return this->AddChildOnTop(opArgument);
}

bool Expression::Sequencefy()
{ this->CheckInitialization();
  if (this->IsSequenceNElementS())
    return true;
  return this->MakeOX(*this->owner, this->owner->opSequence(), *this);
}

bool Expression::operator==(int other)const
{ if (this->owner==0)
    return false;
  int theInt;
  if (!this->IsSmallInteger(&theInt))
    return false;
  return theInt==other;
}

bool Expression::operator==(const std::string& other)const
{ std::string tempS;
  if (!this->IsAtom(&tempS))
    return false;
  return tempS==other;
}
