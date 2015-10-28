//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#include "vpf.h"
#include "vpfHeader3Calculator3_WeylGroupCharacters.h"
#include "vpfHeader3Calculator1_InnerTypedFunctions.h"
#include "vpfHeader3Calculator2_InnerFunctions.h"
#include "vpfImplementationHeader2Math051_PolynomialComputations_Basic.h" //undefined reference to Polynomial<AlgebraicNumber>::MakeOne(int)
ProjectInformationInstance ProjectInfoVpf4cpp(__FILE__, "List of calculator functions. ");
//This file lists calculator functions and various hard-coded rules. Please do not use for any other purposes.

void Calculator::initPredefinedInnerFunctions()
{ this->AddOperationInnerHandler
  ("TestCalculatorIndicator", CalculatorFunctionsGeneral::innerTestIndicator, "",
   "(This is not a mathematical function). Tests the calculator indicator mechanism.",
   "TestCalculatorIndicator(1000)", true, false)
   ;
  this->AddOperationInnerHandler
  ("TestStandardOutput", CalculatorFunctionsGeneral::innerTestStandardOutput, "",
   "(This is not a mathematical function). Tests the calculator standard output.",
   "TestStandardOutput(\"The quick brown fox jumps over the lazy dog\")", true, false)
   ;
  this->AddOperationInnerHandler
  ("crash", CalculatorFunctionsGeneral::innerCrash, "",
   "Crashes the calculator: tests the crashing mechanism (are crash logs properly created, etc.).",
   "crash(0)");
  this->AddOperationInnerHandler
  ("crashListOutOfBounds", CalculatorFunctionsGeneral::innerCrashByListOutOfBounds, "",
   "Crashes the calculator: tests the crashing mechanism (are crash logs properly created, etc.).",
   "crash(0)");
  this->AddOperationInnerHandler
  ("DrawExpressionTree", CalculatorFunctionsGeneral::innerDrawExpressionGraph, "",
   "Draws the internal tree structure of an expression. Does not unfold built-in types.",
   "DrawExpressionTree( e^x)");
  this->AddOperationInnerHandler
  ("DrawExpressionTreeFull", CalculatorFunctionsGeneral::innerDrawExpressionGraphFull, "",
   "Draws the internal tree structure of an expression. Unfolds built-in types. ",
   "DrawExpressionTreeFull( 1); DrawExpressionTree(1+ 1);DrawExpressionTree( Freeze{}(1+1));");
  this->AddOperationInnerHandler
  ("Lispify", CalculatorFunctionsGeneral::innerLispify, "",
   "Shows the internal tree structure of an expression, without completely unfolding the tree structure of expressions that represent a single mathematical\
   entity.",
   "Lispify( e^x)");

  this->AddOperationInnerHandler
  ("LispifyFull", CalculatorFunctionsGeneral::innerLispifyFull, "",
   "Shows the complete internal tree structure of an expression (replacing the expression with a string).",
   "LispifyFull( e^x)");
  this->AddOperationInnerHandler
  ("ToString", CalculatorFunctionsGeneral::innerQuoteToString, "",
   "Transforms an arbitrary expression to its string representation.",
   "ToString( e^x); \n \"e^x\";\"The quick brown fox jumps over the lazy dog.\"");
  this->AddOperationInnerHandler
  ("\"", CalculatorFunctionsGeneral::innerQuoteToString, "",
   "Same as ToString function, but cannot be executed directly because of the special quotes syntax.",
   "ToString( e^x); \n \"e^x\";\"The quick brown fox jumps over the lazy dog.\"");
  this->AddOperationInnerHandler
  ("TestBase64", CalculatorFunctionsGeneral::innerBase64ToCharToBase64Test, "",
   "Test function: converts a base64 string to bitstream and back to base64. Output must be identical to input. ",
   "TestBase64(\"TheQuickBrownFoxJumpsOverTheLazyDog=\");\
   \nTestBase64(\"TheQuickBrownFoxJumpsOverTheLazyDog\")");
  this->AddOperationInnerHandler
  ("CharToBase64", CalculatorFunctionsGeneral::innerCharToBase64, "",
   "Converts characters to bit stream and the bitstream to base64. The character to bit stream conversion is not fixed at the moment and may be \
   system/compiler dependent. I believe that the character to bit stream conversion should be standard for the standard letters in the alphabet.\
   Fancy UTF8 will probably be not read correctly from the CGI input, and furthermore will not be converted in a standard fashion to bit stream.\
   The examples below are taken from Wikipedia. ",
   "CharToBase64(\"pleasure.\");\
    CharToBase64(\"leasure.\");\
    CharToBase64(\"easure.\");\
    CharToBase64(\"asure.\");\
    CharToBase64(\"sure.\");\
   ");
  this->AddOperationInnerHandler
  ("x509CertificateCrunch", CalculatorFunctionsGeneral::innerX509certificateCrunch, "",
   "Processes a x509 certificate database. This function is part of a security research project and \
   will be documented at a later time (if at all). Please do not use this function. ",
   "x509CertificateCrunch(\"certificates\");\
   ", false, true, "CalculatorFunctionsGeneral::innerX509certificateCrunch");

  this->AddOperationInnerHandler
  ("Sha1", CalculatorFunctionsGeneral::innerSha1OfString, "",
   "Converts characters to a sequence of bits and computes the sha1 hash value of those bits. \
   The examples below are taken from Wikipedia. ",
   "Sha1(\"The quick brown fox jumps over the lazy dog\");\
    Sha1(\"The quick brown fox jumps over the lazy cog\");\
   ");

  this->AddOperationInnerHandler
  ("MakeMakefile", CalculatorFunctionsGeneral::innerMakeMakeFile, "",
   "Makes a makefile. ",
   "MakeMakefile(0)", false);

  this->AddOperationInnerHandler
  ("AutomatedTest", this->innerAutomatedTest, "",
   "Runs a big bad automated test of all built in functions against a set of known good results. ",
   "AutomatedTest{}(0)", false);
  this->AddOperationInnerHandler
  ("AutomatedTestSetKnownGoodCopy", this->innerAutomatedTestSetKnownGoodCopy, "",
   "Runs a big bad automated test of all built in functions to create a file containing a set of known good results.",
   "AutomatedTestSetKnownGoodCopy 0", false);
  this->AddOperationInnerHandler
  ("!", CalculatorFunctionsGeneral::innerFactorial, "",
   "Factorial function. ",
   "5!");

  this->AddOperationInnerHandler
  ("printMacdonaldPolys", CalculatorFunctionsWeylGroup::innerMacdonaldPolys, "",
   "Prints macdonald polynomials from a semisimple type. ",
   "printMacdonaldPolys{}(B_3)", true);
  this->AddOperationInnerHandler
  ("MakeExpression", CalculatorConversions::innerExpressionFromBuiltInType, "",
   "Creates expression from built-in polynomial. ",
   "MakeExpression(Polynomial{}((x-2y+z-1)^2(x+y-z)));\
   \nMakeExpression( MakeRationalFunction(1/(1+x^2)));");
  this->AddOperationInnerHandler
  ("Polynomial", CalculatorConversions::innerPolynomial<Rational>, "",
   "Creates a polynomial expression with rational coefficients. ",
   "Polynomial{}((x-2y+z-1)^2(x+y-z));\nPolynomial{}(y^2)-(Polynomial{}y)^2");
  this->AddOperationInnerHandler
  ("PolynomialAlgebraicNumbers", CalculatorConversions::innerPolynomial<AlgebraicNumber>, "",
   "Creates a polynomial expression with algebraic number coefficients. ",
   "PolynomialAlgebraicNumbers{}((x+\\sqrt{2})^2 (\\sqrt{3}x-\\sqrt{5}));");
  this->AddOperationInnerHandler
  ("AlgebraicNumberFromPoly", CalculatorFunctionsGeneral::innerGetAlgebraicNumberFromMinPoly, "",
   "Creates an algebraic number that is a root of a polynomial with algebraic number coefficients. ",
   "AlgebraicNumberFromPoly{}(x^3+\\sqrt{2}x+1);");

  this->AddOperationInnerHandler
  ("PrincipalSlTwoIndex", CalculatorFunctionsGeneral::innerGetPrincipalSl2Index, "",
   "Gives the symmetric Cartan corresponding to a given dynkin type.",
   "PrincipalSlTwoIndex(G_2^3+D_4^2);", true, false, "CalculatorFunctionsGeneral::innerGetPrincipalSl2Index");//, false);
  this->AddOperationInnerHandler
  ("DynkinIndicesSlTwoSubalgebras", CalculatorFunctionsGeneral::innerGetDynkinIndicesSlTwoSubalgebras, "",
   "Fetches the absolute Dynkin indices of the sl(2) subalgebras of the input Dynkin type. The indices of each simple \
    component are computed by actually constructing the sl(2)-subalgebras; clearly this can be greatly optimized\
    but we postpone that for a future version. ",
   "  DynkinIndicesSlTwoSubalgebras(2A_2);\
    \n DynkinIndicesSlTwoSubalgebras(2A_2+A_2^2);\
    \n DynkinIndicesSlTwoSubalgebras(2A_2+A_2^7);\
    \n DynkinIndicesSlTwoSubalgebras(2G_2+G_2^7+F_4+F_4^19);", true, false,
    "CalculatorFunctionsGeneral::innerGetDynkinIndicesSlTwoSubalgebras");//, false);
  this->AddOperationInnerHandler
  ("CartanSymmetric", CalculatorFunctionsGeneral::innerGetSymmetricCartan, "",
   "Gives the symmetric Cartan corresponding to a given dynkin type.",
   "CartanSymmetric(e^7_6);");//, false);
  this->AddOperationInnerHandler
  ("GrowDynkinType", CalculatorFunctionsGeneral::innerGrowDynkinType, "",
   "This is a calculator testing function. Grows a dynkin type inside an ambient Dynkin type. ",
   "GrowDynkinType(A^30_1+d^30_4, e_6); GrowDynkinType(g^35_2+B^30_2, e_6);");//, false);
  this->AddOperationInnerHandler
  ("IsDifferentialOneFormOneVariable", CalculatorFunctionsGeneral::innerIsDifferentialOneFormOneVariable, "",
   "Tests whether the expression is a differential form in one variable.  ",
   "IsDifferentialOneFormOneVariable(\\diff x ); IsDifferentialOneFormOneVariable(x\\diff y ); \
   IsDifferentialOneFormOneVariable(\\frac{\\diff y}{y} );\
   IsDifferentialOneFormOneVariable(1/(\\diff y) );");

  this->AddOperationInnerHandler
  ("Polynomialize", CalculatorFunctionsGeneral::outerPolynomialize, "",
   "Polynomialize(a) is equivalent to MakeExpression(Polynomial(a)).",
   "C= (c a+ a b +b c +1 )^3; A=Polynomialize(C);B=MakeExpression(Polynomial(C)); A-B");

  this->AddOperationInnerHandler
  ("IsConstant", CalculatorFunctionsGeneral::innerIsConstant, "",
   "Tests whether the expression is a constant.  ",
   "IsConstant(\\pi^2); IsConstant(1);IsConstant(x);IsConstant(e^{\\sin(\\pi^2+e+\\sqrt{2}+3)}  ); ");
  this->AddOperationInnerHandler
  ("\\int", CalculatorFunctionsGeneral::innerIntegratePowerByUncoveringParenthesisFirst, "",
   "Attempts to rearrange into standard polynomial form and then integrate.  ",
   "\\int  \\left( \\frac{x(x+1) }{ 2} \\right)^2 dx ", true, false, "CalculatorFunctionsGeneral::innerIntegratePowerByUncoveringParenthesisFirst");
  this->AddOperationInnerHandler
  ("\\int", CalculatorFunctionsGeneral::innerIntegrateXnDiffX, "",
   "Integrates x^n dx.  ",
   "\\int x dx ", true, false, "CalculatorFunctionsGeneral::innerIntegrateXnDiffX");
  this->AddOperationInnerHandler
  ("\\int", CalculatorFunctionsGeneral::innerIntegrateSum, "",
   "If the integral is of the form \\int (A+B )dx, tries recursively to integrate A and B. \
   If successful, integrates the sum in the obvious way.",
   "\\int (x+1+\\sqrt{}2) dx ", true, false, "CalculatorFunctionsGeneral::innerIntegrateSum");

  this->AddOperationInnerHandler
  ("Differentiate", CalculatorFunctionsGeneral::innerDifferentiateSqrt, "",
   "Differentiation - square root function.  ",
   "d/dx(sqrt(x));", true, false, "CalculatorFunctionsGeneral::innerDifferentiateSqrt");


  //the function integrating the building blocks must come in the exact order below:
  //else we risk infinite substitution cycle. The reasons are implementation-specific.
  this->AddOperationInnerHandler
  ("\\int", CalculatorFunctionsGeneral::innerIntegrateRationalFunctionBuidingBlockIa, "",
   "Integrates building block Ia.  ",
   "\\int  (\\frac{3}{(x/2-1)} ) dx ", true, false, "CalculatorFunctionsGeneral::innerIntegrateRationalFunctionBuidingBlockIa");
  this->AddOperationInnerHandler
  ("\\int", CalculatorFunctionsGeneral::innerIntegrateRationalFunctionBuidingBlockIb, "",
   "Integrates building block Ib.  ",
   "\\int  (\\frac{2}{(3x-1)^2} ) dx ", true, false, "CalculatorFunctionsGeneral::innerIntegrateRationalFunctionBuidingBlockIb");
  this->AddOperationInnerHandler
  ("\\int", CalculatorFunctionsGeneral::innerIntegrateRationalFunctionBuidingBlockIIaandIIIa, "",
   "Integrates building blocks IIa and IIIa.  ",
   "\\int  (\\frac{3x+2}{x^2+x+1} ) dx ", true, false, "CalculatorFunctionsGeneral::innerIntegrateRationalFunctionBuidingBlockIIaandIIIa");
  this->AddOperationInnerHandler
  ("\\int", CalculatorFunctionsGeneral::innerIntegrateRationalFunctionBuidingBlockIIIb, "",
   "Integrates building blocks IIIb.  ",
   "\\int  (\\frac{3x+2}{(x^2+x+1)^2} ) dx ", true, false, "CalculatorFunctionsGeneral::innerIntegrateRationalFunctionBuidingBlockIIIb");
  this->AddOperationInnerHandler
  ("\\int", CalculatorFunctionsGeneral::innerIntegrateRationalFunctionBuidingBlockIIb, "",
   "Integrates building blocks IIb.  ",
   "\\int  (\\frac{3x+2}{(x^2+x+1)^2} ) dx ", true, false, "CalculatorFunctionsGeneral::innerIntegrateRationalFunctionBuidingBlockIIb");
  this->AddOperationInnerHandler
  ("\\int", CalculatorFunctionsGeneral::innerIntegrateRationalFunctionSplitToBuidingBlocks, "",
   "Attempts to split an integral of a rational function into building block integrals.  ",
   "\\int  \\frac{1}{x(x+1)} dx ", true, false, "CalculatorFunctionsGeneral::innerIntegrateRationalFunctionSplitToBuidingBlocks");


/*  this->AddOperationInnerHandler
  ("\\diff", CalculatorFunctionsGeneral::innerDifferential, "",
   "Differential.  ",
   "\\diff x;");
*/

  this->AddOperationInnerHandler
  ("Differentiate", CalculatorFunctionsGeneral::innerDifferentiateTrigAndInverseTrig, "",
   "Differentiation - product rule.  ",
   "Differentiate(x,  (\\sin x) \\cos x )", true, false, "CalculatorFunctionsGeneral::innerDifferentiateTrigAndInverseTrig");
  this->AddOperationInnerHandler
  ("Differentiate", CalculatorFunctionsGeneral::innerDifferentiateChainRule, "",
   "Differentiation - chain rule.  ",
   "Differentiate(x,  \\sin x^2 \\cos (\\sin x))", true, false, "CalculatorFunctionsGeneral::innerDifferentiateChainRule");
  this->AddOperationInnerHandler
  ("Differentiate", CalculatorFunctionsGeneral::innerDifferentiateAtimesB, "",
   "Differentiation - product rule.  ",
   "Differentiate(x, f*g )", true, false, "CalculatorFunctionsGeneral::innerDifferentiateAtimesB");
  this->AddOperationInnerHandler
  ("Differentiate", CalculatorFunctionsGeneral::innerDifferentiateAdivideBCommutative, "",
   "Differentiation - division rule, commutative. For the time being differentiation is assumed to be over commutative rings. \
   This might change in the future. The commutative division rule is d/dx (f/g^n)= ((d/dx f) g- n f (d/dx g))/g^{n+1}.  ",
   "Differentiate(x, f/g ); Differentiate(x, f/g^5 ); Differentiate(x, f/g^n )", true, false, "CalculatorFunctionsGeneral::innerDifferentiateAdivideBCommutative");
  this->AddOperationInnerHandler
  ("Differentiate", CalculatorFunctionsGeneral::innerDifferentiateAplusB, "",
   "Differentiation - sum rule.  ",
   "Differentiate(x, f+g )", true, false, "CalculatorFunctionsGeneral::innerDifferentiateAplusB");
  this->AddOperationInnerHandler
  ("Differentiate", CalculatorFunctionsGeneral::innerDifferentiateConstant, "",
   "Differeniation - constant rule. ",
   "Differentiate(x, 5 ); d/dx \\sqrt{5}", true, false, "CalculatorFunctionsGeneral::innerDifferentiateConstant");
  this->AddOperationInnerHandler
  ("Differentiate", CalculatorFunctionsGeneral::innerDifferentiateX, "",
   "Differeniation - d/dx x= 1.",
   "Differentiate(x, x ); d/dx x", true, false, "CalculatorFunctionsGeneral::innerDifferentiateX");
  this->AddOperationInnerHandler
  ("Differentiate", CalculatorFunctionsGeneral::innerDifferentiateConstPower, "",
   "Differeniation - d/dx x^n= n x^{n-1}.",
   "Differentiate(x, x^2 ); d/dx x^-1; d/dt t^{\\sqrt{2}}; d/dc c^{DoubleValue 3.14}", true, false,
   "CalculatorFunctionsGeneral::innerDifferentiateConstPower");
  this->AddOperationInnerHandler
  ("Differentiate", CalculatorFunctionsGeneral::innerDifferentiateAPowerB, "",
   "Differeniation - d/dx a^b= d/dx(e^{b\\log a}) = a^b d/dx(b\\log a) .",
   "Differentiate(x, x^2 ); d/dx x^-1; d/dt t^{\\sqrt{2}}; d/dc c^{DoubleValue 3.14}", true, false, "CalculatorFunctionsGeneral::innerDifferentiateAPowerB");

  this->AddOperationInnerHandler
  ("\\times", CalculatorFunctionsGeneral::innerCrossProduct, "",
   "Cross product in IIId. Returns error if given two non-3d vectors.",
   "(1,-2,3 ) \\times( 1,-1,-1)", true, false, "CalculatorFunctionsGeneral::innerCrossProduct");

  this->AddOperationInnerHandler
  ("ElementWeylAlgebraDO", Calculator::innerElementWeylAlgebra, "",
   "Creates element of a weyl algebra = polynomial coefficient differential operator. First argument denotes differential operator letter, \
   second argument - the dual polynomial expression. ",
   "\\partial_{{i}}=ElementWeylAlgebraDO{}(\\partial_i, x_i); \nx_{{i}}=Polynomial{}x_i; \n[\\partial_1, x_1]; \n\\partial_1 x_1; \nx_1\\partial_1",
   true, false, "Calculator::innerElementWeylAlgebra");
  this->AddOperationInnerHandler
  ("ElementWeylAlgebraPoly", Calculator::innerPolynomialWithEWA, "",
   "Creates a monomial from the second argument whose differential operator letter is the first argument. ",
   "x_{{i}}=ElementWeylAlgebraPoly{}(\\partial_i, x_i); \n\\partial_{{i}}=ElementWeylAlgebraDO{}(\\partial_i, x_i);\n\\partial_1 x_1", true, false,
   "Calculator::innerPolynomialWithEWA");
  this->AddOperationInnerHandler
  ("MakeRationalFunction", CalculatorConversions::innerRationalFunction, "",
   "Creates a built-in rational function.",
   "MakeRationalFunction{}(x_1+MakeRationalFunction{}x_1+x_2)", true, false, "CalculatorConversions::innerRationalFunction");
  this->AddOperationInnerHandler
  ("MakeMatrix", CalculatorConversions::innerMakeMatrix,"",
   "Creates an internal c++ matrix structure from double list of rationals. \
   ", "s_1=MatrixRationals{}((-1,-1,0,0), (0,1,0,0), (0,0,1,0), (0,0,0,1));\
   \ns_2=MakeMatrix{}((1,0,0,0), (-1,-1,-1,0), (0,0,1,0), (0,0,0,1)); \
   \ns_3=MakeMatrix{}((1,0,0,0), (0,1,0,0), (0,-2,-1,-1), (0,0,0,1)); \
   \ns_4=MakeMatrix{}((1,0,0,0), (0,1,0,0), (0,0,1,0), (0,0,-1,-1)); ", true, false, "CalculatorConversions::innerMatrixRational");
  this->AddOperationInnerHandler
  ("MakeMatrixTensorForm", CalculatorConversions::innerMatrixRationalTensorForm,"",
   "Same as MakeMatrix but uses different c++ implementation (class MatrixTensor instead of class Matrix). \
   ", "s_1=MakeMatrixTensorForm{}((-1,-1,0,0), (0,1,0,0), (0,0,1,0), (0,0,0,1)); ", true, false,
   "CalculatorConversions::innerMatrixRationalTensorForm");
  this->AddOperationInnerHandler
  ("MakeMatrixRFs", CalculatorConversions::innerMatrixRationalFunction,"",
   "Creates an internal c++ matrix structure from double list of polynomial functions. \
   ", "s_1=MatrixRFs{}((1-t, 2), (3, 2-t))", true, false, "CalculatorConversions::innerMatrixRationalFunction");
  this->AddOperationInnerHandler
  ("FourierTransformDO", CalculatorFunctionsGeneral::innerFourierTransformEWA, "",
   "Fourier-transforms an element of a weyl algebra. Multiplies each monomial term of odd total degree by -1 \
   (total degree= sum of degrees in the polynomial variables plus the degrees of the differential variables. ",
   "x=ElementWeylAlgebraPoly{}(\\partial, x); \n\\partial=ElementWeylAlgebraDO{}(\\partial, x);\
   \n  a=x^3+x\\partial; b=\\partial x+\\partial^3+\\partial;  \n[FourierTransformDO{}a,FourierTransformDO{}b]-FourierTransformDO{}[a,b]");

  this->AddOperationInnerHandler
  ("PerturbSplittingNormal", Calculator::innerPerturbSplittingNormal, "",
   "Takes 3 arguments: normal, cone and general vectors. Attempts to perturb the normal so that the cone \
   stays on one side of the normal and so that the general vectors \
   have as little zero scalar products with the normal as possible.",
   "PerturbSplittingNormal{}((0,0,0,0), ( \
   (-1, -2, -2, -2), (-1, -1, -2, -2), (0, -1, -2, -2), (-1, -1, -1, -1), (0, -1, -1, -1), \
   (0, 0, -1, -1), (-1, -1, 0, 0), (0, -1, 0, 0), (-1, 0, 0, 0), (1, 0, 0, 0), (0, 1, 0, 0), (1, 1, 0, 0), \
   (0, 0, 1, 1), (0, 1, 1, 1), (1, 1, 1, 1), (0, 1, 2, 2), (1, 1, 2, 2), (1, 2, 2, 2)\
   ),\
   ( \
   (-2, -3, -4, -2), (-1, -3, -4, -2), (-1, -2, -4, -2), (-1, -2, -3, -2), (-1, -2, -2, -2), (-1, -2, -3, -1), \
   (-1, -1, -2, -2), (-1, -2, -2, -1), (0, -1, -2, -2), (-1, -1, -2, -1), (-1, -2, -2, 0), \
   (0, -1, -2, -1), (-1, -1, -1, -1), (-1, -1, -2, 0), (0, -1, -1, -1), (0, -1, -2, 0), (-1, -1, -1, 0), \
   (0, 0, -1, -1), (0, -1, -1, 0), (-1, -1, 0, 0), (0, 0, 0, -1), (0, 0, -1, 0), (0, -1, 0, 0), (-1, 0, 0, 0), \
   (1, 0, 0, 0), (0, 1, 0, 0), (0, 0, 1, 0), (0, 0, 0, 1), (1, 1, 0, 0), (0, 1, 1, 0), (0, 0, 1, 1), (1, 1, 1, 0), \
   (0, 1, 2, 0), (0, 1, 1, 1), (1, 1, 2, 0), (1, 1, 1, 1), (0, 1, 2, 1), (1, 2, 2, 0), (1, 1, 2, 1), (0, 1, 2, 2), (1, 2, 2, 1), \
   (1, 1, 2, 2), (1, 2, 3, 1), (1, 2, 2, 2), (1, 2, 3, 2), (1, 2, 4, 2), (1, 3, 4, 2), (2, 3, 4, 2)\
   )\
    )", true, false)
   ;
  this->AddOperationInnerHandler
  ("TestSpechtModules", CalculatorFunctionsWeylGroup::innerTestSpechtModules, "",
   "Tests all Specht modules of S_n, where n is the only argument taken by the function.",
   "TestSpechtModules(4); ", true, false)
   ;
  this->AddOperationInnerHandler
  ("MakeElementWeylGroup", CalculatorFunctionsWeylGroup::innerWeylGroupElement, "",
   "Needs a group name and a list of generators",
   "s_{{i}}=MakeElementWeylGroup{}(A_2, i);\n(s_1+s_2)(2s_1+s_2)(3s_1+s_2)", true, false)
   ;
  this->AddOperationInnerHandler
  ("MakeElementWeylGroup", CalculatorFunctionsWeylGroup::innerWeylGroupElement, "",
   "Needs a group name and a list of generators",
   "s_{{i}}=MakeElementWeylGroup{}(A_2, i);\n(s_1+s_2)(2s_1+s_2)(3s_1+s_2)", true, false)
   ;

  this->AddOperationInnerHandler
  ("WeylGroupTauSignatures", CalculatorFunctionsWeylGroup::innerSignSignatureRootSubsystems, "",
   "Prints the tau signatures of a Weyl group. See a common article by T. Folz-Donahue, S. Jackson, T. Milev, A. Noel. ",
   "WeylGroupTauSignatures{}(b_3);", true, false)
   ;

  this->AddOperationInnerHandler
  ("WeylGroupConjugacyClassesFromAllElements", CalculatorFunctionsWeylGroup::innerWeylGroupConjugacyClassesFromAllElements, "",
   "For small ranks, computes the conjugacy classes of a Weyl group by enumerating all elements of the group. ",
   "WeylGroupConjugacyClassesFromAllElements{}(A_2);", true, true, "CalculatorFunctionsWeylGroup::innerWeylGroupConjugacyClassesFromAllElements");
   ;
  this->AddOperationInnerHandler
  ("WeylGroupConjugacyClassesRepresentatives", CalculatorFunctionsWeylGroup::innerWeylGroupConjugacyClassesRepresentatives, "",
   "Computes a representatvie in each conjugacy classes of a Weyl group by comparing conjugacy classes invariants and by enumerating conjugacy class orbits. ",
   "WeylGroupConjugacyClassesRepresentatives{}(A_2);", true, true)
   ;
  this->AddOperationInnerHandler
  ("WeylGroupConjugacyClasses", CalculatorFunctionsWeylGroup::innerWeylGroupConjugacyClasseS, "",
   "Loads the conjugacy classes of a Weyl group (hard-coded), or computes them if rank<=6. ",
   "WeylGroupConjugacyClasses{}(f_4);", true, true)
   ;
  this->AddOperationInnerHandler
  ("WeylGroupCharTable", CalculatorFunctionsWeylGroup::innerWeylGroupLoadOrComputeCharTable, "",
   "Loads the character table of a Weyl group. The character tables are hard-coded.",
   "WeylGroupCharTable{}(b_3);", true, false)
   ;
  this->AddOperationInnerHandler
  ("WeylGroupIrrepsAndCharTableComputeFromScratch", CalculatorFunctionsWeylGroup::innerWeylGroupIrrepsAndCharTableComputeFromScratch, "",
   "<b>Work in progress. Please do not use. </b> Computes from scratch the irreducible representations and the character table of a Weyl group.",
   "WeylGroupIrrepsAndCharTableComputeFromScratch{}(b_3);", true, true)
   ;
    this->AddOperationInnerHandler
  ("WeylOrbitSize",  CalculatorFunctionsWeylGroup::innerWeylGroupOrbitSize, "",
   "Computes the size of a Weyl group orbit of a weight",
   "WeylOrbitSize(E_6, (3,3,3,7,7,11)); WeylOrbitSize(E_8, (3,3,3,7,7,11,13,13));");
    this->AddOperationInnerHandler
  ("WeylOrbitSimpleCoords",  CalculatorFunctionsWeylGroup::innerWeylGroupOrbitSimple, "",
   "Generates a Weyl orbit printout from simple coords.\
    First argument = type. Second argument = weight in simple coords. \
    The orbit size is cut off at max 1920 elements (type D_5).",
   "WeylOrbitSimpleCoords{}(B_2, (y, y));");
     this->AddOperationInnerHandler
  ("WeylGroupOrbitOuterSimple", CalculatorFunctionsWeylGroup::innerWeylGroupOrbitOuterSimple, "",
   "Generates a Weyl outer orbit printout from simple coords. The outer orbit is the orbit under the Weyl\
    group extended with the outer automoprhisms of the Weyl group.\
    First argument = type. Second argument = weight in simple coords. \
    The orbit size is cut off at 1921*2 elements.",
   "WeylGroupOrbitOuterSimple{}(D_4, (1,0,0,0))", true, false)
   ;
  this->AddOperationInnerHandler
  ("WeylOrbitFundCoords", CalculatorFunctionsWeylGroup::innerWeylGroupOrbitFund, "",
   "Generates a Weyl orbit printout from fundamental coords.\
    First argument = type. Second argument = weight in fundamental coords. ",
   "WeylOrbitFundCoords{}(B_2, (y, 0));");
  this->AddOperationInnerHandler
  ("WeylOrbitFundRho", CalculatorFunctionsWeylGroup::innerWeylGroupOrbitFundRho, "",
   "Generates a Weyl orbit printout from fundamental coords.\
    First argument = type. Second argument = weight in fundamental coords. Doing the rho-modified action. ",
   "WeylOrbitFundRho{}(B_2, (y, 0) )");
  this->AddOperationInnerHandler
  ("WeylRaiseToMaximallyDominant", CalculatorFunctionsWeylGroup::innerWeylNoOuterRaiseToMaximallyDominant, "",
   "Raises a set of rational vectors simultaneously to maximally dominant: it raises the first vector\
   to dominant, then each consecutive vector is raised as much as possible \
   without changing the preceding, alrady raised,  vectors. First argument = type. \
   Second argument = weight in simple coords. ",
   "WeylRaiseToMaximallyDominant{}(F_4,  (3, 6, 8, 4), (-1, -3, -2, 0)  ); \
   \nWeylRaiseToMaximallyDominant{}(F_4,  (3, 6, 8, 4), (0, -3, -4, -2)  );\
   \nWeylRaiseToMaximallyDominant{}(F_4, (0, -3, -4, -2) , (3, 6, 8, 4)  );\
   ");
  this->AddOperationInnerHandler
  ("MakeWeight", CalculatorFunctionsWeylGroup::innerLieAlgebraWeight, "",
   "Makes a weight. First argument= type. Second argument= coordinate. Third argument =\
   one of the keywords epsilon, simple, fundamental, standing for the coordinate system. ",
   "\\varepsilon_{{a}}=MakeWeight{}(B_3, a, epsilon); \
   \n\\psi_{{a}}=MakeWeight{}(B_3, a, fundamental);\
   \n\\eta_{{a}}= MakeWeight{}(B_3, a, simple);\
   \n\\varepsilon_1;\
   \n\\psi_1;\
   \n\\eta_3\
   ");
  this->AddOperationInnerHandler
  ("MakeRho", CalculatorFunctionsWeylGroup::innerLieAlgebraRhoWeight, "",
   "Makes the half-sum of the positive roots.",
   "\n\\rho=MakeRho(B_3);\
   ");
   this->AddOperationInnerHandler
  ("IsOuterAuto", CalculatorFunctionsWeylGroup::innerIsOuterAutoWeylGroup, "",
   "Checks if element is outer automorphism of a root system. First argument = type. \
   Second argument = matrix linear operator corresponding written in simple basis. ",
   "A=MakeMatrix\
((1, 0, 0, 0, 0, -1),\
(0, 0, 0, 0, 1, -2),\
(0, 0, 1, 0, 0, -2),\
(0, 0, 0, 1, 0, -3),\
(0, 1, 0, 0, 0, -2),\
(0, 0, 0, 0, 0, -1));\
   IsOuterAuto{}(e_6,  A);\
   ");
  this->AddOperationInnerHandler
  ("WeylOuterRaiseToMaximallyDominant", CalculatorFunctionsWeylGroup::innerWeylOuterRaiseToMaximallyDominant, "",
   "Same as WeylRaiseToMaximallyDominant but uses outer Weyl group automorphisms as well.",
   "WeylOuterRaiseToMaximallyDominant{}(D_4,  (1, 0, 0, 0), (0, 0, 0, 1)  ); \
    WeylOuterRaiseToMaximallyDominant{}(D_4,  (0, 0, 0, 1), (1, 0, 0, 0)  );\
    WeylOuterRaiseToMaximallyDominant{}(D_4,  (1, 0, 0, 0), (0, 0, 1, 0)  );\
    WeylOuterRaiseToMaximallyDominant{}(D_4,  (0, 0, 1, 0), (1, 0, 0, 0)  );\
   ");
  this->AddOperationInnerHandler
  ("GenerateFiniteMultiplicativelyClosedSet", Calculator::innerGenerateMultiplicativelyClosedSet, "",
   "The first argument gives upper_bound to the number of elements of the set. \
   Generates a finite multiplicatively closed set of at most upper_bound elements, \
   or returns error indicating the multiplicatively closed \
   set is larger than the upper bound.",
   "s_1=MakeMatrix{}((-1 , 1 , 0),(0 , 1 , 0),(0 , 0 , 1));\
    \ns_2=MakeMatrix{}((1 , 0 , 0),(1 , -1 , 1),(0 , 0 , 1));\
    \ns_3=MakeMatrix{}((1 , 0 , 0),(0 , 1 , 0),(0 , 2 , -1));\
    \nGenerateFiniteMultiplicativelyClosedSet{}(48, s_1, s_2, s_3);", true, false)
   ;
  this->AddOperationInnerHandler
  ("CartanSA", CalculatorFunctionsGeneral::innerConstructCartanSA, "",
   "Generates a Cartan subalgebra, code is still experimental.",
    "g_{{i}}=getChevalleyGenerator{}(b_3, i);\
   \nh_{{i}}=getCartanGenerator{}(b_3, i);\
   CartanSA(g_1+g_3, g_-1+g_-3, g_2, g_-2);\
   "
    , true, true, "CalculatorFunctionsGeneral::innerConstructCartanSA")
   ;
  this->AddOperationInnerHandler
  ("GenerateVectorSpaceClosedWRTLieBracket", CalculatorFunctionsGeneral::innerGenerateVectorSpaceClosedWRTLieBracket, "",
   "Generates a vector space closed with respect to the Lie bracket from input of type elements of weyl algebra or Lie algebra. \
   The first argument of the input is an upper bound for the dimension of the vector space. The remaining \
   arguments must be differential operators. The output is a vector space \
   basis printout. Fails if the dimension of the vector space is larger than the upper bound.",
   "\\partial_{{i}}=ElementWeylAlgebraDO{}(\\partial_i, x_i); x_{{i}}=ElementWeylAlgebraPoly{}(\\partial_i, x_i); \
    \ng_1=(-x_{5} \\partial_{5}^{2}-x_{4} \\partial_{4} \\partial_{5}  -x_{3} \\partial_{3} \\partial_{5} \
    \n-x_{2} \\partial_{2} \\partial_{5}  +x_{1} \\partial_{2} \\partial_{4} +x_{1} \\partial_{3}^{2});\
    \ng_-1=x_5;\
    \ng_2=x_{5} \\partial_{4} -x_{2} \\partial_{1} ;\
    \ng_-2=(x_{4} \\partial_{5} -x_{1} \\partial_{2} );\
    \ng_3=(2x_{4} \\partial_{3} -x_{3} \\partial_{2} );\
    \ng_-3=(x_{3} \\partial_{4} -2x_{2} \\partial_{3} );\
    \nGenerateVectorSpaceClosedWRTLieBracket{}(50, g_1, g_-1, g_2, g_-2, g_3, g_-3);\
    \nGenerateVectorSpaceClosedWRTLieBracket{}(50,g_1,g_2, g_-2, g_3, g_-3);\
    \nGenerateVectorSpaceClosedWRTLieBracket{}(50,g_1, g_-1, g_2, g_-2, g_3); \
    q_{{i}}= getChevalleyGenerator{}(F_4, i);  \
    s_2=-q_{-5}-q_{-6};\ns_1=q_{20}+q_{19};\n s_-1=-q_{-19}-q_{-20}\n; s_-2=2q_{6}+2q_{5};\
    \nGenerateVectorSpaceClosedWRTLieBracket(52, s_1, s_2, s_-1, s_-2);\
    ", true, false)
   ;
  this->AddOperationInnerHandler
  ("WeylGroupNaturalRep", CalculatorFunctionsWeylGroup::innerWeylGroupNaturalRep, "",
   "Gets the natural representation of the Weyl group.",
   "WeylGroupNaturalRep{}(B_3)", true, false)
   ;
  this->AddOperationInnerHandler
  ("WeylGroupGetOuterAutoGenerators", CalculatorFunctionsWeylGroup::innerWeylGroupOuterAutoGeneratorsPrint, "",
   "Gets the generators of the outer automorphism group (the group generated by the graph automorphisms \
   of the Dynkin diagram.",
   "WeylGroupGetOuterAutoGenerators{}(2D_4)", true, false)
   ;

  this->AddOperationInnerHandler
  ("MakeVirtualWeylGroupRepresentation", CalculatorFunctionsWeylGroup::innerMakeVirtualWeylRep, "",
   "Convert a representation into virtual one.",
   "MakeVirtualWeylGroupRepresentation{}(WeylGroupNaturalRep{}(B_3))", true, true)
   ;
  this->AddOperationInnerHandler
  ("MinPoly", CalculatorFunctionsGeneral::innerMinPolyMatrix, "",
   "Computes the minimal polynomial of a matrix, provided that the matrix is not too large.",
   "A=MakeMatrix{}((0,1), (-1,0)); p=MinPoly{}A", true, false)
   ;
  this->AddOperationInnerHandler
  ("CharPoly", CalculatorFunctionsGeneral::innerCharPolyMatrix, "",
   "Computes the characteristic polynomial of a matrix (=det(A-q*Id)), provided that the matrix is not too large.",
   "A=\\begin{array}{cc}2 & 3& 5\\\\ 7& 11& 13\\\\ 17&19 &23 \\end{array}; p=MinPoly{}A", true, false)
   ;
  this->AddOperationInnerHandler
  ("MakeCharacterLieAlg", this->innerCharacterSSLieAlgFD, "",
   "Creates character of a semisimple Lie algebra finite dimensional irreducible module. \
   First argument gives type, second argument gives highest weight in fundamental coordinates.",
   "x=MakeCharacterLieAlg{}(G_2, (1,0));\ny=MakeCharacterLieAlg{}(G_2, (0,1));\nx*y", true, false)
   ;
  this->AddOperationInnerHandler
  ("GetLinks", this->innerGetLinksToSimpleLieAlgerbas, "",
   "Gets simple Lie algebra links to the calculator.",
   "GetLinks{}0", false, false)
   ;
  this->AddOperationInnerHandler
  ("ConesIntersection", this->innerConesIntersect, "",
   "Takes as input two sequences of vectors, generates two cones, \
   and intersects them using the simplex\
   algorithm. The output is a string report of the operation. \
   The first cone is generated over Z_{&gt;} (``strict cone'')\
   the second cone is generated over Z_{&gt;=0} (``non-strict cone'').",
   "v_1=(1, 2, 3 ); v_2=(1, 3, 2); v_3=(3,1,1); v_4=(-2,2, 2);\
   \n ConesIntersection{}((v_1,v_2 ), (v_3,v_4 ));\nConesIntersection{}((v_1,v_2 ), (v_3,-v_4 ));", true, false)
   ;

  this->AddOperationInnerHandler
  ("Crawl", CalculatorFunctionsGeneral::innerCrawlTexFile, "",
   "Crawls a latex file collecting all local style files and all \\input commands to produce a single latex file. \
   The function was originally designed for the purposes of the freecalc project as not been tested on any other projects,\
   please use only for freecalc.",
   "Crawl(\"/home/todor/math/freecalc/homework/UMB-All-Problems-By-Course/Calc-I-MasterProblemSheet.tex\")", true, false
   , "CalculatorFunctionsGeneral::innerCrawlTexFile")
   ;
  this->AddOperationInnerHandler
  ("BuildFreecalc", CalculatorFunctionsGeneral::innerBuildFreecalc, "",
   "Builds the freecalc lecture pdfs. \
   Takes as argument the lecuture file.",
   "BuildFreecalc(\"/home/todor/math/freecalc/lectures/UMB-M141-2015-Fall/Lecture_2015_Fall_Calculus_II_Math_141.tex\");\
   \nBuildFreecalc(\"/home/todor/math/freecalc/lectures/UMB-M140-2015-Fall/Lecture_2015_Fall_Calculus_I_Math_140.tex\");\
   \nBuildFreecalc(\"/home/todor/math/freecalc/homework/UMB-M141-2015-Fall/Homework_2015_Fall_Calculus_II_Math_141.tex\");\
   \nBuildFreecalc(\"/home/todor/math/freecalc/homework/UMB-M140-2015-Fall/Homework_2015_Fall_Calculus_I_Math_140.tex\");\
   ", false, false
   , "CalculatorFunctionsGeneral::innerBuildFreecalc")
   ;
  this->AddOperationInnerHandler
  ("SetOutputFile", CalculatorFunctionsGeneral::innerSetOutputFile, "",
   "Sets an output/progress report file different from the default one. No dots, forward slashes, back slashes or file extensions allowed.\
   A .html will be appended to the output file name.",
   "SetOutputFile(\"E7_subalgebras\"); SetOutputFile(\"/root/output.html\")", true, false
   , "CalculatorFunctionsGeneral::innerSetOutputFile")
   ;
  this->AddOperationInnerHandler
  ("CoefficientsPowersOf", CalculatorFunctionsGeneral::innerCoefficientsPowersOf, "",
   "Extracts the coefficients of the powers of the first argument in the second argument.",
   "CoefficientsPowersOf(x, a x^2+ b *3 x +c +\\pi +3)", true, false)
   ;

  this->AddOperationInnerHandler
  ("\\log", CalculatorFunctionsGeneral::innerLog, "",
   "Logarithm function. Gives a decimal approximation of the natural logarithm provided the input is a double number. ",
   "\\log{}(e); \\log 10", true, false)
   ;
  this->AddOperationInnerHandler
  ("\\arctan", CalculatorFunctionsGeneral::innerArctan, "",
   "Arctan function. Tries to evaluate the arctan function. ",
   "\\arctan(3/4)", true, false)
   ;
  this->AddOperationInnerHandler
  ("\\arccos", CalculatorFunctionsGeneral::innerArccos, "",
   "Arccos function. Tries to evaluate the arccos function. ",
   "\\arccos(3/4)", true, false)
   ;
  this->AddOperationInnerHandler
  ("\\arcsin", CalculatorFunctionsGeneral::innerArcsin, "",
   "Arcsin function. Tries to evaluate the arcsin function. ",
   "\\arcsin(3/4)", true, false)
   ;
  this->AddOperationInnerHandler
  ("\\sin", CalculatorFunctionsGeneral::innerSin, "",
   "Sine function. Evaluates to a decimal approximation if the input is a double number. ",
   "\\sin{}(3.1415)", true, false)
   ;
  this->AddOperationInnerHandler
  ("\\cos", CalculatorFunctionsGeneral::innerCos, "",
   "Cosine function. Evaluates to a decimal approximation if the input is a double number. ",
   "\\cos{}(3.1415)", true, false)
   ;
  this->AddOperationInnerHandler
  ("\\tan", CalculatorFunctionsGeneral::innerTan, "",
   "Tangent function. Substitutes \\tan{}{{x}}=\\sin{}x/\\cos x. ",
   "\\tan{}(3.1415); \\tan 1.570796327", true, false)
   ;
  this->AddOperationInnerHandler
  ("\\cot", CalculatorFunctionsGeneral::innerCot, "",
   "Tangent function. Substitutes \\tan{}{{x}}=\\cos{}x/\\sin x. ",
   "\\cot{}(3.1415); \\cot 1.570796327", true, false)
   ;
  this->AddOperationInnerHandler
  ("\\csc", CalculatorFunctionsGeneral::innerCsc, "",
   "Cosecant function. Substitutes \\csc{}{{x}}=1/\\sin x. ",
   "\\csc{}(3.1415); \\csc 1.570796327", true, false)
   ;
  this->AddOperationInnerHandler
  ("\\sec", CalculatorFunctionsGeneral::innerSec, "",
   "Secant function. Substitutes \\sec{}{{x}}=1/\\sec x. ",
   "\\sec{}(3.1415); \\sec 1.570796327", true, false)
   ;

  this->AddOperationInnerHandler
  ("DoubleValue", CalculatorFunctionsGeneral::innerEvaluateToDouble, "",
   "Double value of a rational number.",
   "DoubleValue{}(3/7)", true, false)
   ;
  this->AddOperationInnerHandler
  ("ModP", this->innerZmodP, "",
   "Number modulo P. First argument = number, second argument = modulo.",
   "ModP{}(7, 3)", true, false)
   ;
  this->AddOperationInnerHandler
  ("AdCommonEigenspace", &this->innerAdCommonEigenSpaces, "",
   "Computes common eigenspace of the adjoint action of semisimple Lie algebra elemets inside the \
   semisimple Lie algebra. ",
   "AdCommonEigenspace{}(F_4, -5 (g_{9})+3 (g_{13})+5 (g_{16})+4 (g_{10}), g_{14}+g_{22}+g_{20})")
   ;
  this->AddOperationInnerHandler
  ("AttemptExtendingEtoHEFwithHinCartan", &this->innerAttemptExtendingEtoHEFwithHinCartan, "",
   "Attempts to embed an element E into an sl(2)-triple over the rationals, such that the element H is in the \
   ambient Cartan algebra. ",
   "AttemptExtendingEtoHEFwithHinCartan{}(F_4, g_1+2g_2+3g_3+4g_4)")
   ;

  this->AddOperationInnerHandler
  ("gcdPoly", &this->innerGCD, "",
   "Greatest common divisor polynomial of two polynomials. \
   The divisor is scaled so that all coefficients are relatively prime integers, \
   and so that the leading monomial under the graded lexicographic order (x_2>x_1, etc.)\
   has positive coefficient.",
   "gcdPoly{}(-x_{13}^{2}x_{15}^{3}x_{21}^{2}x_{22}-2x_{13}^{2}x_{14}x_{15}x_{17}x_{21}^{3}+\
   2x_{13}^{2}x_{15}^{2}x_{16}x_{21}^{3}+2x_{13}^{2}x_{15}^{2}x_{17}x_{20}x_{21}^{2}+\
   x_{13}^{2}x_{14}x_{17}x_{18}x_{19}x_{21}^{2}-x_{13}^{2}x_{15}x_{16}x_{18}x_{19}x_{21}^{2}+\
   x_{13}^{2}x_{15}x_{17}^{2}x_{19}x_{21}^{2},\
   x_{13}^{3}x_{15}x_{18}x_{21}^{2}x_{22}-2x_{13}^{3}x_{17}^{2}x_{21}^{3}-\
   4x_{13}^{2}x_{14}x_{15}x_{17}x_{21}^{3}+2x_{13}^{2}x_{15}^{2}x_{16}x_{21}^{3}-\
   2x_{13}^{3}x_{17}x_{18}x_{20}x_{21}^{2}+2x_{13}^{2}x_{14}x_{17}x_{18}x_{19}x_{21}^{2}-\
   x_{13}^{2}x_{15}x_{16}x_{18}x_{19}x_{21}^{2}  )")
   ;
  this->AddOperationInnerHandler
  ("lcmPoly", &this->innerLCM, "",
   "Least common multiple of two polynomials.\
   The output is scaled so that all coefficients are relatively prime integers, \
   and so that the leading monomial under the graded lexicographic order (x_2>x_1, etc.)\
   has positive coefficient.",
   "lcmPoly{}(-x_{13}^{2}x_{15}^{3}x_{21}^{2}x_{22}-2x_{13}^{2}x_{14}x_{15}x_{17}x_{21}^{3}+\
   2x_{13}^{2}x_{15}^{2}x_{16}x_{21}^{3}+2x_{13}^{2}x_{15}^{2}x_{17}x_{20}x_{21}^{2}+\
   x_{13}^{2}x_{14}x_{17}x_{18}x_{19}x_{21}^{2}-x_{13}^{2}x_{15}x_{16}x_{18}x_{19}x_{21}^{2}+\
   x_{13}^{2}x_{15}x_{17}^{2}x_{19}x_{21}^{2},\
   x_{13}^{3}x_{15}x_{18}x_{21}^{2}x_{22}-2x_{13}^{3}x_{17}^{2}x_{21}^{3}-\
   4x_{13}^{2}x_{14}x_{15}x_{17}x_{21}^{3}+2x_{13}^{2}x_{15}^{2}x_{16}x_{21}^{3}-\
   2x_{13}^{3}x_{17}x_{18}x_{20}x_{21}^{2}+2x_{13}^{2}x_{14}x_{17}x_{18}x_{19}x_{21}^{2}-\
   x_{13}^{2}x_{15}x_{16}x_{18}x_{19}x_{21}^{2}  )")
   ;
  this->AddOperationInnerHandler
  ("Interpolate", &this->innerInterpolatePoly, "",
   "Constructs the one-variable polynomial of minimal degree that passes through the points. Points are given in the form \
   ((x_1, y_1),(x_2, y_2), ...,(x_n, y_n))",
   "Interpolate{}(1,0) ; Interpolate{}((1,0),(2,3)); Interpolate{}((1,1), (2,2), (3, 4), (4, 8), (5, 16))", true, false)
   ;
  this->AddOperationInnerHandler
  ("PolyDivRemainder", &this->innerPolynomialDivisionRemainder, "",
   "Returns the remainder after taking quotient of a \
    polynomial divided by a set of polynomials using the default monomial order (lexicographic).",
   "PolyDivRemainder{}(x^7+6x y+5x y^8+y^5, x+y^2-1, y^3-x y) ;", true, false)
   ;
  this->AddOperationInnerHandler
  ("PolyDivStringGrLex", &this->innerPolynomialDivisionVerboseGrLex, "",
   "Prints a string representing division of \
   a polynomial by a set of polynomials using gr lex order, for example, x^2 y^3 >x y^4, y^11>x^10. ",
   "PolyDivStringGrLex{}(x^7+6x y+5x y^8+y^5, x^2+2, y^3-1) ;", true, false)
   ;
  this->AddOperationInnerHandler
  ("PolyDivStringGrLexRev", &this->innerPolynomialDivisionVerboseGrLexRev, "",
   "Prints a string representing division of \
   a polynomial by a set of polynomials using total degree (graded) order with \
   equal total degrees compared by lex order with reversed order of the letters, \
   for example, x y^4> x^2 y^3 , x^11>y^10. ",
   "PolyDivStringGrLexRev{}(x^7+6x y+5x y^8+y^5, x^2+2, y^3-1) ;", true, false)
   ;
  this->AddOperationInnerHandler
  ("PolyDivStringLex", &this->innerPolynomialDivisionVerboseLex, "",
   "Prints a string representing division of \
   a polynomial by a set of polynomials using the lex order, for example, x^2 y^4 > x y^1000 > x y^2. ",
   "PolyDivStringLex{}(x^7+6x y+5x y^8+y^5, x^2+2, y^3-1) ;", true, false)
   ;
  this->AddOperationInnerHandler
  ("PolyDivStringLexRev", &this->innerPolynomialDivisionVerboseLexRev, "",
   "Prints a string representing division of \
   a polynomial by a set of polynomials using the lex order with reversed order of variables, \
   for example, y^2 > x^1000 y > x y. ",
   "PolyDivStringLexRev{}(x^7+6x y+5x y^8+y^5, x^2+2, y^3-1) ;", true, false)
   ;
  this->AddOperationInnerHandler
  ("suffixNotationForPostScript", this->innerSuffixNotationForPostScript, "",
   "Suffix notation. for postscript, used to quickly generate pstricks drawings in LaTeX.  \
   ",
   "suffixNotationForPostScript{}((1/3 +a+b)*c)")
   ;
  this->AddOperationInnerHandler ("DFQEuler", CalculatorFunctionsGeneral::innerDFQsEulersMethod, "",
   "<b>Calculus teaching function.</b> Iterates Euler's method to approximate solutions of first order ordinary DFQ's. \
   First argument = expression for y',\
   second and third argument = x and y initial values, fourth argument = number of approximating points,\
    fifth and sixth argument = left and right endpoints.\
   ",
   "DFQEuler(x^2 + y^2 - 1, 0, 0, 1000, -2.5, 2.5)")
   ;
  this->AddOperationInnerHandler ("drawPolar", CalculatorFunctionsGeneral::innerPlotPolarRfunctionTheta, "",
   "<b>Calculus teaching function.</b> Draws polar curve given in polar coordinates in the form \
   r=f(t), where t is the angle variable. The angle variable is measured in degrees. \
   The first argument gives the function, the second and third argument give the upper and \
   lower bounds of the angle. \
   ",
   "drawPolar(1+sin  t, 0, \\pi); \
   \ndrawPolar((1 + 9/10 cos(8 t) ) (1 + 1/10 cos (24 t) ) (9/10 + 5/100 cos (200 t)) (1 + sin t), 0, 2\\pi)")
   ;
  this->AddOperationInnerHandler ("drawPolarExtended", CalculatorFunctionsGeneral::innerPlotPolarRfunctionThetaExtended, "",
   "<b>Calculus teaching function.</b> Same as drawPolar but also produces a graph in the (rho,theta)-plane. \
   ",
   "drawPolarExtended(1+sin  t, 0, \\pi); \
   \ndrawPolarExtended((1 + 9/10 cos(8 t) ) (1 + 1/10 cos (24 t) ) (9/10 + 5/100 cos (200 t)) (1 + sin t), 0, 2\\pi)")
   ;
  this->AddOperationInnerHandler ("plotConeUsualProjection", CalculatorFunctionsGeneral::innerPlotConeUsualProjection, "",
   "<b>Calculus teaching function.</b>Plots a cone with center of base at (0,0,0) with base lying in the xy plane\
    of radius r and height h(first and second argument) viewed from a distance d (third argument) and viewing height vh (fourth argument).\
   ",
   "plotConeUsualProjection(2, 4, 3, 2)")
   ;
  this->AddOperationInnerHandler ("plotWedge", CalculatorFunctionsGeneral::innerPlotWedge, "",
   "<b>Calculus teaching function.</b>Plots a wedge centered at x,y (first, second argument) or radius r (third argument) \
   with starting/end angle given by fourth and fifth argument.\
   ",
   "plotWedge(2, 2, 1.1,1.570796327, 2*3.1415 )")
   ;

  this->AddOperationInnerHandler ("GaussianElimination", CalculatorFunctionsGeneral::innerGaussianEliminationMatrix, "",
   "Gaussian elimination of a matrix. Prints a detailed string that shows the Gaussian elimination of \
    a matrix.",
   "GaussianElimination(\\begin{array}{cccccc} sqrt 1 & sqrt 2 & sqrt 3 & 1 & 0& 0\\\\ sqrt 4 & sqrt 5 & sqrt 6 & 0 &1&0\\\\ sqrt 7 & sqrt 8 & sqrt 9 & 0 & 0 & 1     \\end{array})")
   ;
  this->AddOperationInnerHandler ("UnivariatePartialFractions", CalculatorFunctionsGeneral::innerSplitToPartialFractionsOverAlgebraicReals, "",
   "Attempts to splits a univariate rational function into partial fractions over the real algebraic numbers.\
   <b>Currently the function is not implemented properly.</b> \
   At the moment, the function factors the denominator over the rationals. If this factorization\
   yields quadratic and linear terms at the function succeeds, else fails.",
   "UnivariatePartialFractions(\\frac{x^11}{x^{8}-2x^{6}-2x^{5}+4x^{3}+x^{2}-2});")
   ;

  this->AddOperationInnerHandler ("plotCurve", CalculatorFunctionsGeneral::innerPlotParametricCurve, "",
   "<b>Calculus teaching function.</b> Plots a curve sitting in 2-dimensional space. \
    The first and second argument give the x and y coordinate functions; the curve parameter must be t.\
   ",
   "plotCurve{}(sin{}t,\\cos(2t), 0, 5)")
   ;

  this->AddOperationInnerHandler ("plot2D", CalculatorFunctionsGeneral::innerPlot2D, "",
   "<b>Calculus teaching function.</b> Makes a 2d plot of a function given in the form \
   y=f(x). The the second and third argument give the upper and \
   lower bounds of x. Plots may be added together- adding plots superimposes the plots. \
   Trigonometry functions must be entered in the usual calculator format, i.e., in the form \
   sin{}x, cos{}x, etc. or in the form \\sin{}x, \\cos{}x, etc. (both forms are fine).\
   We recall that in the calculator, {}  stands for function application (the symbols {} were\
   chosen as they produce invisible LaTeX output).\
   ",
   "plot2D{}(\\sin{}x+cos{}x, 0, 5)", true, false, "CalculatorFunctionsGeneral::innerPlot2D")
   ;
  this->AddOperationInnerHandler ("plot2DWithBars", CalculatorFunctionsGeneral::innerPlot2DWithBars, "",
   "<b>Calculus teaching function.</b> Same as plot2D but plots two functions with bars locked between the two functions; \
   , used to illustrate approximations to definite integrals.",
   "\nA=3/2- ((-3/4+1/4 (x))^{2});\nB= (1/4 (x))^{2}+2;\nplot2DWithBars{}(A, B, 0, 5, 1)")
   ;
  this->AddOperationInnerHandler ("plotIntegralOf", CalculatorFunctionsGeneral::innerPlotIntegralOf, "",
   "<b>Calculus teaching function.</b> Plots area locked under curve.",
   "\nA=3/2- ((-3/4+1/4 (x))^{2});\nplotIntegralOf{}(A, 0, 5)")
   ;
  this->AddOperationInnerHandler
  ("IsInteger", this->innerIsInteger, "",
   " If the argument has no bound variables, returns 1 if the argument is an integer, 0 otherwise. ",
   "IsInteger{}a;\nIsInteger{}1;\nf{}{{a}}=IsInteger{}a;\nf{}1;\nf{}b");
  this->AddOperationInnerHandler
  ("IsEven", CalculatorFunctionsGeneral::innerIsEven, "",
   " If the argument has no bound variables, returns 1 if the argument is an even integer, 0 otherwise. ",
   "i^{{n}}:if IsEven n=(-1)^n; i^100 ");
  this->AddOperationInnerHandler
  ("IsNonEmptySequence", CalculatorFunctionsGeneral::innerIsNonEmptySequence, "",
   " If the argument has no bound variables, returns 1 if the argument is an integer, 0 otherwise. ",
   "IsInteger{}a;\nIsInteger{}1;\nf{}{{a}}=IsInteger{}a;\nf{}1;\nf{}b", true, false,
   "CalculatorFunctionsGeneral::innerIsNonEmptySequence");
  this->AddOperationInnerHandler
  ("IsRational", this->innerIsRational, "",
   " If the argument has no bound variables, returns 1 if the argument is an rational, 0 otherwise. ",
   "IsRational{}a;IsRational{}-1;\nf{}{{a}}=IsRational{}a;\nIsRational{}1;\nIsRational{}b");
    this->AddOperationInnerHandler
  ("Not", this->innerNot, "",
   " If the argument is a small integer, returns 1 if the argument is 0 and 1 the argument is non-zero. \
   If the argument is not a small integer, does nothing. ",
   "Not{}1;Not{}a; Not{}0; Not{}(3==4)");
    this->AddOperationInnerHandler
  ("AllPartitions", CalculatorFunctionsGeneral::innerAllPartitions, "",
   "Prints all partitions of the vector (first argument) using a given list of vectors (second argument). \
   All partitioning vectors should have positive coordinates. ",
   "AllPartitions((10,11), ((1,2), (2,3), (4,5), (2,1), (3,2), (5,4))) ");

    this->AddOperationInnerHandler
  ("PrintNonNegativeVectorsLevel", this->innerPrintZnEnumeration, "",
   " Prints all vectors of grade level d with n coordinates lying in Z_{>=0}. Function meant for \
   debugging purposes. First argument =dimension, second argument=grading level. ",
   "PrintNonNegativeVectorsLevel{}(4, 5);PrintNonNegativeVectorsLevel{}(4, 0); ");
  this->AddOperationInnerHandler
  ("SemisimpleLieAlgebra", CalculatorConversions::innerSSLieAlgebra, "",
   "Creates a semisimple Lie algebra. The semisimple Lie algebra is given via its Dynkin type. A simple Dynkin type is given by\
    type letter with upper index equal to the inverse of the scale of the symmetric Cartan matrix and lower index equal to \
    the rank of the subalgebra. For example A^2_3 stands for type A_3 (sl (4)) with symmetric Cartan matrix scale equal to 1/2.\
    If the upper index is omitted, it is implied to be 1.<hr>\
    We define the symmetric Cartan matrix as the matrix whose (i,j)^{th} enry is the scalar product of the i^{th} and j^{th} roots. \
    We assume the roots to be ordered in the order implied by the (non-symmetric) Cartan matrices on page 59 in Huphreys, \
    Introduction to Lie algebras and representation theory. If the upper index of the Dynkin type is 1, the corresponding symmetric Cartan matrix is\
    assigned the default value. The default values are chosen so that the long root has squared length 2 in types \
    A_n, B_n, D_n, E_6, E_7, E_8, and F_4, squared length 4 in C_n, and squared length 6 in type G_2. \
    <br>If upper index is not equal to 1, the symmetric Cartan matrix is given by dividing the default symmetric Cartan matrix by the upper index. \
    ",
   "g_{{i}}=getChevalleyGenerator{}(SemisimpleLieAlgebra{}G_2, i);\
   \nh_{{i}}=getCartanGenerator{}(SemisimpleLieAlgebra{}G_2, i);\
    \n[g_1,g_{-1}]; \n[g_2, g_{-2}]; \n[h_{1}, g_6]; \n[h_2, g_{-6}]");
  this->AddOperationInnerHandler
  ("printSemisimpleLieAlgebra", &this->innerPrintSSLieAlgebraVerbose, "",
   "Creates a printout with information about the semisimple Lie algebra, including the Lie bracket pairing table. \
   In addition, this function creates a graphics of the root system. \
   ",
   "printSemisimpleLieAlgebra{}(F_4);\nprintSemisimpleLieAlgebra{}(2G^5_2+B_3);");
  this->AddOperationInnerHandler
  ("getChevalleyGenerator", this->innerGetChevGen, "",
   "First argument must be a semisimple Lie algebra, second argument must \
   be an integer from -N to N,\
   where N is the number of positive roots of the Lie algebra.\
   The function returns the Chevalley-Weyl generator labeled by the root\
   corresponding to the integer index. The roots are indexed in the ordered displayed by the \
   printSemisimpleLieAlgebra function. ",
   "[getChevalleyGenerator{}(G_2, 6), getChevalleyGenerator{}(G_2, -6)]");
  this->AddOperationInnerHandler
  ("getCartanGenerator", this->innerGetCartanGen, "",
   "First argument must be a semisimple Lie algebra, second argument must \
   be a number between 1 and K, where K is the rank of the Lie algebra.\
   In this case the function returns the element of the Cartan subalgebra that is dual \
   to the simple root with the same index. Note that this element of the Cartan subalgebra\
   is proportional to a Chevalley-Weyl generator with a coefficient of proportionality \
   equal to 2/(simple root length squared) ).",
   "getCartanGenerator{}(G_2, 1)");

  this->AddOperationInnerHandler
  ("FunctionToMatrix", this->innerFunctionToMatrix,"",
   "Creates a matrix from a function. The first argument gives the function, \
   the second argument the number of rows, \
   the third- the number of columns.\
   ", "X=FunctionToMatrix{}(A,5,5);\n A{}({{a}},{{b}})=a/b;\n X; \\det {} X");
  this->AddOperationInnerHandler
  ("Transpose", this->innerTranspose, "",
   "Transposes a matrix of expressions. \
   ", "Transpose{}(1,2); (1,2)^t");

  this->AddOperationInnerHandler
  ("\\det", CalculatorFunctionsGeneral::innerDeterminant, "",
   "Tries to convert to a matrix of rationals or matrix of rational \
   functions and computes the determinant if\
   not too large. \
   ", "X=FunctionToMatrix{}(A,5,5);\n A{}({{a}},{{b}})=1/(a+b);\n X; \\det {} X");
  this->AddOperationInnerHandler
  ("DeterminantPolynomial", this->innerDeterminantPolynomial, "",
   "Attempts to converts the entries of the matrix to polynomials and computes the determinant polynomial.\
   The matrix must not be larger than 8x8. Note that this function does not perform Gaussian elimination: it is meant \
   to be used with multivariate polynomials with large number of variables. ",
   "DeterminantPolynomial{}\\left(\\begin{array}{ccc}x_{11}& x_{12} \
   & x_{13}\\\\ x_{21} & x_{22} & x_{23} \\\\ x_{31} & x_{32} & x_{33} \\end{array} \\right) ");
  this->AddOperationInnerHandler
  ("Sort", CalculatorFunctionsGeneral::innerSort, "",
   "Sorts a sequence. The purpose of this function is to test the exrpession comparison function of the calculator. ",
   "Sort(x^2, x^3, x^1, x^{-1}) ");
  this->AddOperationInnerHandler
  ("BlocksOfCommutativity", CalculatorFunctionsGeneral::innerGetUserDefinedSubExpressions, "",
   "Returns subexpression blocks of commutativity. ",
   "BlocksOfCommutativity(\\sin x + x^2+ 3x y +18x ^{3/4 y}+\\sqrt{2}^{\\sqrt{2}c})");
  this->AddOperationInnerHandler
  ("hwv", CalculatorFunctionsGeneral::innerHWV, "",
   "Highest weight vector in a generalized Verma module. \
   The first argument gives the semisimple Lie algebra. The second argument \
   gives the highest weight in fundamental coordinates. \
   The third argument parametrizes the parabolic subalgebra, e.g. (1,0,0) stands for a \
   parabolic subalgebra with first simple root crossed-out. The second argument is allowed to have \
   entries that are not non-negative integers in the positions in which the third argument has 1's. ",
   "g_{{i}}=getChevalleyGenerator{}(B_3, i);h_{{i}}=getCartanGenerator{}(B_3, i);\
   \nv_\\mu=hwv{} (A_3, (1,0,1),(0,0,0));\nv_\\lambda=hwv{}(B_3, (x_1,0,1),(1,0,0));\nh_1g_{-1}v_\\lambda");
  this->AddOperationInnerHandler
  ("printModule", CalculatorFunctionsGeneral::innerPrintGenVermaModule, "",
   "Makes a report on a finite dimensional Lie algebra module, or more generally, on \
   a generalized Verma module (irreducible finite dimensional semisimple Lie algebra \
   modules are a partial case of generalized Verma modules). \
   The first argument gives the semisimple Lie algebra. The second argument \
   gives the highest weight in fundamental coordinates. \
   The third argument parametrizes the parabolic subalgebra, e.g. (1,0) stands for a \
   parabolic subalgebra (lying in algebra of rank 2) with first simple root crossed-out. \
   The second argument is allowed to have \
   entries that are not non-negative integers in the positions in which the third argument has 1's. ",
   "printModule{} (G_2, (2,0),(0,0))");
  this->AddOperationInnerHandler
  ("hwTAAbf", CalculatorFunctionsGeneral::innerHWTAABF, "",
   "Highest weight transpose anti-automorphism bilinear form, a.k.a. Shapovalov form. \
   Let M be a Verma module with highest weight vector v given in \
   fundamental coordinates. Let P:M->M\
   be a projection map onto Cv that maps every weight vector of M of weight different from the \
   highest to 0. Let u_1, u_2 be two words in the universal enveloping algebra. Then define hwTAAbf(u_1,u_2)=\
   Tr_M (P ( taa(u_1) u_2 ), where taa() is the transpose anti-automorphism of g. ",
   "g_{{a}}=getChevalleyGenerator{} (G_2, a);\nhwTAAbf{}(g_{-1} g_{-2}, g_{-1}g_{-2}, (2,2))");
  this->AddOperationInnerHandler
  ("WeylDimFormula", CalculatorFunctionsGeneral::innerWeylDimFormula, "",
   "Weyl dimension formula. First argument gives the type of the Weyl group of the simple\
    Lie algebra in the form Type_Rank (e.g. E_6).\
   The second argument gives the highest weight in fundamental coordinates. ",
   "WeylDimFormula{}(G_2, (x,0));\nWeylDimFormula{}(B_3, (x,0,0));");
  this->AddOperationInnerHandler
  ("animateLittelmannPaths", this->fAnimateLittelmannPaths, "",
   "Generates all Littelmann-Lakshmibai-Seshadri paths, draws them and animates them. \
   Presented first on the seminar in Charles University Prague. \
   The first argument gives the type of the semisimple  Lie algebra, the second gives the highest weight. \
   ",
   "animateLittelmannPaths{}(G_2, (2,0));");
  this->AddOperationInnerHandler
  ("DecomposeInducingRepGenVermaModule",
   CalculatorFunctionsGeneral::innerDecomposeFDPartGeneralizedVermaModuleOverLeviPart, "",
   "Decomposes the inducing module of a generalized Verma module over the Levi part of a parabolic smaller than the inducing one.\
   The first argument gives the type of the algebra. The second argument gives the highest weight of the module in \
   fundamental coordinates. The third argument gives the parabolic subalgebra with respect to which we induce. \
   The last argument gives the parabolic subalgebra with respect to whose Levi part we decompose.",
   "DecomposeInducingRepGenVermaModule{}(B_3,(0, 1,1),(1,0,0), (1,0,1))");
  this->AddOperationInnerHandler
  ("Casimir", this->innerCasimir, "",
   "Gives the Casimir element. ",
   "Casimir{}(G_2)");
   this->AddOperationInnerHandler
  ("CasimirWRTLevi", CalculatorFunctionsGeneral::innerCasimirWRTlevi, "",
   "Gives the Casimir element of a Levi part of a parabolic subalgebra. First argument = ambient Lie algebra. Second argument \
   = parabolic selection =1 stands for simple root outside of the Levi component, 0 stands for simple root of the Levi component.",
   "CasimirWRTLevi{}(B_3, (1,0,0))");
  this->AddOperationInnerHandler
  ("hmmG2inB3", this->innerEmbedG2inB3, "",
   "Embeds elements of the Universal enveloping of G_2 in B_3, following an embedding found in a paper by McGovern.",
   "g_{{a}}=getChevalleyGenerator{} (G_2, a); hmmG2inB3{}(g_1);\nhmmG2inB3{}(g_2) ");
  this->AddOperationInnerHandler
  ("drawRootSystem", CalculatorFunctionsGeneral::innerDrawRootSystem, "",
   "Draws the root system of a semisimple Lie algebra. Takes one or three arguments: \
    if one argument is provided, that must be a semisimple Lie algebra or a semisimple Lie algebra \
    type; if three arguments are provided, the first must be a semisimple Lie algebra or a semisimple \
    Lie algebra type, and the second and third argument must be two rational vectors of dimension\
    equal to the rank of the semisimple Lie algebra. \
    The root system is drawn in a Coxeter plane.\
    If the extra two vectors are given, they are used to initialize a preferred projection plane\
    in the change-to-basis entries below the graphics. Clicking the obvious button creates a basis \
    change animations ideal for presentations.",
   "drawRootSystem{}(A_7, (1, 0 , 2, 2, 2, 0, 1), (1, 3, 2, 2, 2, 3, 1));");
  this->AddOperationInnerHandler
  ("drawWeightSupportWithMults", CalculatorFunctionsGeneral::innerDrawWeightSupportWithMults, "",
   "Draws the weight support of an irreducible finite-dimensional highest weight module. \
   The first argument gives the type and the second gives the highest weight given in \
   fundamendal weight basis. \
   The maximum number of drawn weights allowed is (hard-code) limited to 10 000. If \
   the number of weights in the weight support exceeds this number, only the first 10 000 weights will be drawn. \
   <b> Warning. Drawing text (i.e. multiplicities) \
   is very javascrtipt-processor-intensive. Use only for *small* examples, else you might hang your browser. </b>",
   "drawWeightSupportWithMults{}(B_3,(0,1,1));\n drawWeightSupportWithMults{}(G_2,(1,0))");
  this->AddOperationInnerHandler
  ("drawWeightSupport", CalculatorFunctionsGeneral::innerDrawWeightSupport, "",
   "Same as drawWeightSupportWithMults but displays no multiplicities. Same warning for hanging up your browser \
    with javascript holds.",
   "drawWeightSupport{}(B_3,(1,1,1)); drawWeightSupport{}(G_2,(1,2))");
  this->AddOperationInnerHandler
  ("SplitFDpartB3overG2CharsOnly", this->fSplitFDpartB3overG2CharsOnly, "",
   "Splits the finite dimensional part of the inducing module of the generalized Verma module of\
    B_3(so(7)) into G_2-components. \
   The argument is gives the highest weight of the generalized Verma module in fundamental \
   coordinates with respect to so(7). \
   The arguments which are not small integers indicate the non-selected roots of the inducing parabolic\
    subalgebra of B_3. ",
   "SplitFDpartB3overG2CharsOnly{}(x_1,2,0)");
  this->AddOperationInnerHandler
  ("SplitFDpartB3overG2", CalculatorFunctionsGeneral::innerSplitFDpartB3overG2, "",
   "Splits the finite dimensional part of the inducing module of the generalized Verma module of \
   B_3(so(7)) into G_2-components. \
   The argument is gives the highest weight of the generalized Verma module in fundamental \
   coordinates with respect to so(7). \
   The arguments which are not small integers indicate the non-selected roots of the inducing parabolic \
   subalgebra of B_3. ",
   "SplitFDpartB3overG2{}(x_1,1,0)");
  this->AddOperationInnerHandler
  ("PrintB3G2branchingTableCharsOnly",this->fPrintB3G2branchingTableCharsOnly, "",
   "Creates a table of branching of finite dimensional B_3-modules over G_2. The argument of the \
   function gives the maximum height \
   of the B_3-weight. The second argument indicates the parabolic subalgebra of B_3- zero entry \
   stands for the corresponding root space lying \
   in the Levi part of the parabolic. Non-zero entry means the corresponding negative root space is \
   not in the parabolic. The expression given \
   in that coordinate is used as the corresponding highest weight. ",
   "PrintB3G2branchingTableCharsOnly{}(2, (0,0,0)); PrintB3G2branchingTableCharsOnly{}(2, (x_1,0,0))", true, true);
  this->AddOperationInnerHandler
  ("PrintB3G2branchingTable", this->fPrintB3G2branchingTable, "",
   "Creates a table of branching of finite dimensional B_3-modules over G_2. \
    The argument of the function gives the maximum height \
   of the B_3-weight. The function works with arguments 0 or 1; values of 2 or more must be run off-line.",
   "PrintB3G2branchingTable{}(1, (0,0,0)); PrintB3G2branchingTable{}(1, (x_1,0,0))", true, true);
  this->AddOperationInnerHandler
  ("SplitFDTensorGenericGeneralizedVerma", CalculatorFunctionsGeneral::innerSplitGenericGenVermaTensorFD, "",
   "Experimental, please don't use. Splits generic generalized Verma module tensor finite dimensional module. ",
   "SplitFDTensorGenericGeneralizedVerma{}(G_2, (1, 0), (x_1, x_2)); ");
  this->AddOperationInnerHandler
  ("WriteGenVermaAsDiffOperatorsUpToWeightLevel",
   CalculatorFunctionsGeneral::innerWriteGenVermaModAsDiffOperatorUpToLevel, "",
   "<b>Experimental, please don't use</b>. Embeds a Lie algebra \
   in the Weyl algebra with matrix coefficients.\
   The third argument gives the highest weight. \
   The non-zero entries of the highest weight give the\
   root spaces outside of the Levi part of the parabolic. \
   The second argument gives the weight level to which the computation should be carried out",
   "WriteGenVermaAsDiffOperatorsUpToWeightLevel{}(B_3, 1, (0, 0, y)); ");
  this->AddOperationInnerHandler
  ("MapSemisimpleLieAlgebraInWeylAlgebraGeneratorOrder",
   this->innerWriteGenVermaModAsDiffOperatorsGeneratorOrder, "",
   "Constructs a parabolically induced map from a semisimple Lie algebra \
   to a Weyl algebra with matrix coefficients. More precisely, this function constructs a \
   generalized Verma module and writes it using differential operators with matrix coefficients. \
   The first argument gives the type of the semisimple Lie algebra, the second argument \
   gives the highest weight in fundamental coordinates. The third argument gives the\
   parabolic subalgebra selection with 0 standing for non-crossed out roots and 1 for \
   crossed-out roots. In all non crossed-out root positions, \
   the coordinates of the highest weight \
   vector must be small integers (because the highest\
   weight must be dominant and integral with respect to the Levi part of the inducing parabolic\
   subalgebra). In the crossed-out root positions, the coordiantes of the highest weight vector\
   can be arbitrary polynomial expressions. \
   The algorithm depends on an order chosen on the Chevalley-Weyl generators of the nilradical.\
   Here, we sort the nilradical elements in descending order relative to the order of their roots.\
   In addition to the first 3 arguments, this function\
   accepts further optional arguments, customizing the notation of the final printout.",
   "MapSemisimpleLieAlgebraInWeylAlgebraGeneratorOrder{}(B_3, (0,0,0), (0, 0, 1), x, \\partial, a); ", true, false);
  this->AddOperationInnerHandler
  ("MapSemisimpleLieAlgebraInWeylAlgebra", this->innerWriteGenVermaModAsDiffOperatorsNilOrderDescending, "",
   "Same as MapSemisimpleLieAlgebraInWeylAlgebra, but with a different order on the elements of the nilradical. \
   We sort the elements of the nilradical as follows. Consider the partial order given \
   by the graded lex order on the coordinates that correspond to crossed-out roots. Then extend this partial order \
   by ``breaking the ties'' via the graded lexicographic order on the non-crossed out roots. This extension we call\
   the nil-order. Now this function sorts the elements of the nilradical in descending nil-order, that is, \
   elements with higher nil-order come first.",
   "MapSemisimpleLieAlgebraInWeylAlgebra{}(B_3, (0,0,0), (0, 0, 1), x, \\partial, a); ", true, false);
  this->AddOperationInnerHandler
  ("MapSemisimpleLieAlgebraInWeylAlgebraAllGens", this->innerWriteGenVermaModAsDiffOperatorsAllGensNilOrderDescending, "",
   "Constructs a parabolically induced map from a semisimple Lie algebra \
   to a Weyl algebra with matrix coefficients. More precisely, this function constructs a \
   generalized Verma module and writes it using differential operators with matrix coefficients. \
   The first argument gives the type of the semisimple Lie algebra, the second argument \
   gives the highest weight in fundamental coordinates. The third argument gives the\
   parabolic subalgebra selection with 0 standing for non-crossed out roots and 1 for \
   crossed-out roots. In all non crossed-out root positions, \
   the coordinates of the highest weight \
   vector must be small integers (because the highest\
   weight must be dominant and integral with respect to the Levi part of the inducing parabolic\
   subalgebra). In the crossed-out root positions, the coordiantes of the highest weight vector\
   can be arbitrary polynomial expressions. In addition to the first 3 arguments, this function\
   accepts further optional arguments, customizing the notation of the final printout.\
   The differential operator part of the algorithm uses the nil-order given by the function \
   MapSemisimpleLieAlgebraInWeylAlgebra.",
   "MapSemisimpleLieAlgebraInWeylAlgebraAllGens{}(B_3, (0,0,0), (0, 0, 1), x, \\partial, a); ", true, false);
  this->AddOperationInnerHandler
  ("KLcoeffs", this->fKLcoeffs, "",
   "Computes the n by n tables of 1) Kazhdan-Lusztig polynomials, 2) R polynomials and 3) Kazhdan-Lusztig \
   polynomials evaluated at one, where n<=192  is the size of the Weyl group (i.e. no larger than D_4(so(8)).\
   The notation follows the original paper by Kazhdan and Lusztig, \"\
   Representations of Coxeter Groups and Hecke Algebras\"\
   . The algorithm is directly derived from formulas (2.0a-c) there, as explained in \
   the Wikipedia page on Kazhdan-Lusztig polynomials. \
   Please note that the 192 by 192 element table takes almost 3 minutes to compute.\
   Faster implementations of the KL polynomials are available from programs by Fokko du Cloux and others\
   (our simple implementation stores the full table of R-polynomials and KL-polynomials in RAM memory at \
   all times, unlike\
   the other more efficient implementations).",
   "KLcoeffs{}(B_3)");
  this->AddOperationInnerHandler
  ("RootSubsystem", this->innerRootSubsystem, "",
   "Generates a root subsystem of a simple type. First argument indicates simple type, second, third,... arguments \
   give the generating roots. ",
   "RootSubsystem(F_4, (0,1,0,0), (0,0,1,0), (1,1,2,2))");

  this->AddOperationInnerHandler
  ("printRootSubalgebras", CalculatorFunctionsGeneral::innerPrintRootSAs, "",
   "Prints sl(2) subalgebras and root subalgebras. \
   The argument gives the type of the Lie algebra in the form Type_Rank (e.g. E_6).",
   "printRootSubalgebras(E_6)");
  this->AddOperationInnerHandler
  ("printRootSubalgebrasRecompute", CalculatorFunctionsGeneral::innerPrintRootSAsForceRecompute, "",
   "Same as printRootSubalgebras but forces recomputation. Use to  \
   recompute obsolete or interrupted output files.",
   "printRootSubalgebras(f_4)", false, false, "CalculatorFunctionsGeneral::innerPrintRootSAsForceRecompute");
  this->AddOperationInnerHandler
  ("printSlTwoSubalgebras", CalculatorFunctionsGeneral::innerPrintSltwos, "",
   "Prints sl(2) subalgebras and root subalgebras. \
   The argument gives the type of the Lie algebra in the form Type_Rank (e.g. E_6).",
   "printSlTwoSubalgebras(g_2)");

   this->AddOperationInnerHandler
  ("parabolicsInfoBruhatGraph", CalculatorFunctionsGeneral::innerParabolicWeylGroupsBruhatGraph, "",
   " Makes a table with information about the Weyl group of a parabolic subalgebra of the ambient Lie algebra, \
   as well as the cosets \
   (given by minimal coset representatives) of the Weyl subgroup in question. \
   The input must have as many integers as there are simple roots in the ambient \
   Lie algebra. If the root is crossed out (i.e. not a root space of the Levi part), \
   one should put a 1 in the corresponding \
   coordinate. \
   Otherwise, one should put 0. For example, for Lie algebra B3(so(7)), \
   calling parabolicsInfoBruhatGraph(0,0,0) gives you the Weyl group info for the entire algebra; \
   calling parabolicsInfoBruhatGraph(1,0,0) gives you info for the Weyl subgroup generated by \
   the last two simple root. \
   In the produced graph, the element s_{\\eta_i} corresponds to a reflection with respect to the i^th simple root. \
   You will get your output as a .png file link, you must click onto the link to see the end result. \
   <b>Please do not use for subalgebras larger than B_4 (so(9)). The vpf program has no problem handling this \
   function up to E_6 but LaTeX crashes trying to process the output. </b>",
   "parabolicsInfoBruhatGraph{}(B_3,(1,0,0),(1,0,0))");
  this->AddOperationInnerHandler
  ("fTestMonomialBasisConjecture", this->fTestMonomialBaseConjecture, "",
   "Tests the monomial basis conjecture from the Jackson-Milev paper. First argument gives rank bound. \
    Second argument gives dimension bound. ",
   "fTestMonomialBasisConjecture{}(2, 50)");
/*  this->AddOperationInnerHandler
  ("DecomposeCharGenVermaToIrreps", & this->fDecomposeCharGenVerma, "",
   "<b>This is an experiment; I have no mathematical proof that this function works. \
   If you are seeing this function then I forgot to comment it out before updating the calculator. \
   Please don't use this function.</b> Decomposes the \
   character of a generalized Verma module as a sum of characters of irreducible highest weight \
    modules.\
    First argument = type. Second argument = highest weight. Non-integer entries give parabolic selection. ",
   "DecomposeCharGenVermaToIrreps{}(G_2, (x_1, 0))");*/
  this->AddOperationInnerHandler
  ("LSpath", this->fLSPath, "",
   "Lakshmibai-Seshadri path starting at 0. The first argument gives the semisimple Lie algebra, \
    the next arguments give the waypoints of the path.",
   "LSpath{}(G_2, (0,0), (2,1) )");
  this->AddOperationInnerHandler
  ("LROdefine", this->fLittelmannOperator, "",
   "Littelmann root operator e_i, where e_i is the Littelmann root operator with \
   respect to root of index i. If i is negative then the e_i root operator is defined to be \
   the f_\alpha operator.",
   "e_{{i}}=LROdefine_i; e_{-1} e_{-1} LSpath{}(G_2, (0,0), (2,1))", true, true);
  this->AddOperationInnerHandler
  ("printProductDistancesModN", CalculatorFunctionsGeneral::innerFindProductDistanceModN, "",
   "First argument = number N. Second argument = list of positive numbers. This function finds, modulo N, \
   the product distance between 1 and each number mod N. We define the product distance between 1 and a number X as \
   the minimal sum of elements whose product equals X mod N, where the elements are taken from the predefined list of \
   positive integers given by the second argument.",
   "printProductDistancesModN( 65537, (97,98,99,100,101,102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122)) ",
   false, true, "CalculatorFunctionsGeneral::innerFindProductDistanceModN");
  this->AddOperationInnerHandler
  ("SolveProductSumEquationOverSetModN", CalculatorFunctionsGeneral::innerSolveProductSumEquationOverSetModN, "",
   "Tries to find one solution of the system a_1*a_2* ...= X mod N a_1+a_2+...=Y where the a_i's belong to a \
   predefined set of positive numbers. ",
   "SolveProductSumEquationOverSetModN(theMod=65537; theProduct=16628; theSum=1286; theSet= (97,98,99,100,101,102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122)) ",
   true, false, "CalculatorFunctionsGeneral::innerFindProductDistanceModN");
  this->AddOperationInnerHandler
  ("IsNilpotent", CalculatorFunctionsGeneral::innerIsNilpotent, "",
   "Computes whether a matrix is nilpotent. May/will be extended to work for an arbitrary object \
   for which the term \"nilpotent\" makes sense. ",
   "IsNilpotent{}((0,1),(0,0))");
  this->AddOperationInnerHandler
  ("InvertMatrixVerbose", this->innerInvertMatrixVerbose, "",
   "<b>Calculus teaching function.</b> Inverts a matrix of rationals if invertible, in any other case generates an error. Makes a detailed \
   printout of all Gaussian elimantion steps. ",
   "InvertMatrixVerbose{}((1,2),(2,3))");
  this->AddOperationInnerHandler
  ("InvertMatrix", CalculatorFunctionsGeneral::innerInvertMatrix, "",
   "Inverts a matrix of rationals if invertible, in any other case generates an error. ",
   "X=MakeMatrix((1,2,1), (1,0,1), (-1,1,0)); InvertMatrix X- X^{-1}");
  this->AddOperationInnerHandler
  ("Trace", CalculatorFunctionsGeneral::innerTrace, "",
   "Gets trace of a square matrix. ",
   "X=MakeMatrix((1,2,1), (1,0,1), (-1,1,0)); Trace X");
  this->AddOperationInnerHandler
  ("Reverse", this->innerReverseOrdeR, "",
   "Reverses order of elements. This operation will reverse products, orderes of lists, etc. \
    More precisely, the command leaves the fist child in the internal representation of the object in place\
    and flips the order of all other children.",
   "Reverse{}(s_1 s_2 s_3s_4s_2s_3s_1s_2s_3s_4s_1s_2s_3s_2s_1)", true, false, "Calculator::innerReverseOrdeR");
  this->AddOperationInnerHandler
  ("ReverseRecursively", this->innerReverseOrderRecursively, "",
   "Same as Reverse but will apply recursively to the children expressions as well.",
   "Reverse{}(s_1 s_2 s_3s_4s_2s_3s_1s_2s_3s_4s_1s_2s_3s_2s_1)", true, false, "Calculator::innerReverseOrderRecursively");

  this->AddOperationInnerHandler
  ("FindOneSolutionSerreLikePolynomialSystem", CalculatorFunctionsGeneral::innerSolveSerreLikeSystemNoUpperLimit, "",
   "Attempts to heuristically solve a system of polynomial of equations over the rationals. \
   The input system of equations can be arbitrary, \
   however it is assumed that the system is Serre-like, i.e., comes \
   from the Serre relations governing the embedding of a semisimple Lie algebra in a semisimple Lie algebra. \
   This suggests a certain heuristic solution strategy (will be documented as the code matures). \
   To stress it out explicitly, the algorithm is heuristic - i.e., is not guaranteed to work. \
   The result of the function is a printout with one of the possible outcomes.\
   <br>Outcome 1. While processing the polynomial system, the computation limit was \
   hit. The computation was aborted. \
   No information on the system can be given (except that it is large).\
   <br>Outcome 2. The reduced Groebner basis of the system is {1}. Therefore the system is contradictory - no solution exists. \
   <br>Outcome 3. The reduced Groebner basis was found and is not equal to {1}. Therefore a solution over the complex numbers \
   exists. However, no such solution was found.\
   <br>Outcome 4. A solution was found and is presented to the user.\
   ",
   "FindOneSolutionSerreLikePolynomialSystem{}(  \
    x_{12}x_{24}-x_{10}x_{22}-2x_{8}x_{20}-x_{7}x_{19}+1, \
    x_{11}x_{24}-x_{10}x_{23}-x_{8}x_{21},\
    x_{9}x_{24}-x_{8}x_{23}+x_{7}x_{21},\
    x_{6}x_{24}+2x_{5}x_{23}-x_{4}x_{22}+2x_{3}x_{21}-2x_{2}x_{20}-x_{1}x_{19},\
    x_{12}x_{23}-x_{11}x_{22}-x_{9}x_{20},\
    x_{11}x_{23}+x_{10}x_{22}+x_{8}x_{20}-1,\
    x_{9}x_{23}+x_{8}x_{22}-x_{7}x_{20},\
    x_{12}x_{21}-x_{11}x_{20}+x_{9}x_{19},\
    x_{11}x_{21}+x_{10}x_{20}-x_{8}x_{19},\
    x_{9}x_{21}+x_{8}x_{20}+x_{7}x_{19}-1,\
    x_{12}x_{18}+2x_{11}x_{17}-x_{10}x_{16}+2x_{9}x_{15}-2x_{8}x_{14}-x_{7}x_{13},\
    x_{6}x_{18}-x_{4}x_{16}-2x_{2}x_{14}-x_{1}x_{13}+1,\
    x_{5}x_{18}+x_{4}x_{17}+x_{2}x_{15},\
    x_{3}x_{18}+x_{2}x_{17}-x_{1}x_{15},\
    x_{6}x_{17}+x_{5}x_{16}+x_{3}x_{14},\
    x_{5}x_{17}+x_{4}x_{16}+x_{2}x_{14}-1,\
    x_{3}x_{17}+x_{2}x_{16}-x_{1}x_{14},\
    x_{6}x_{15}+x_{5}x_{14}-x_{3}x_{13},\
    x_{5}x_{15}+x_{4}x_{14}-x_{2}x_{13},\
    x_{3}x_{15}+x_{2}x_{14}+x_{1}x_{13}-1)", true, false, "CalculatorFunctionsGeneral::innerSolveSerreLikeSystemNoUpperLimit");
  this->AddOperationInnerHandler
  ("FindOneSolutionSerreLikePolynomialSystemUpperLimit", CalculatorFunctionsGeneral::innerSolveSerreLikeSystemUpperLimit, "",
   "Same as FindOneSolutionSerreLikePolynomialSystem but the first argument gives upper limits \
   to the number of polynomial computations that can be carried out.\
    ",
   "FindOneSolutionSerreLikePolynomialSystemUpperLimit{}( 2001, \
    x_{12}x_{24}-x_{10}x_{22}-2x_{8}x_{20}-x_{7}x_{19}+1, \
    x_{11}x_{24}-x_{10}x_{23}-x_{8}x_{21},\
    x_{9}x_{24}-x_{8}x_{23}+x_{7}x_{21},\
    x_{6}x_{24}+2x_{5}x_{23}-x_{4}x_{22}+2x_{3}x_{21}-2x_{2}x_{20}-x_{1}x_{19},\
    x_{12}x_{23}-x_{11}x_{22}-x_{9}x_{20},\
    x_{11}x_{23}+x_{10}x_{22}+x_{8}x_{20}-1,\
    x_{9}x_{23}+x_{8}x_{22}-x_{7}x_{20},\
    x_{12}x_{21}-x_{11}x_{20}+x_{9}x_{19},\
    x_{11}x_{21}+x_{10}x_{20}-x_{8}x_{19},\
    x_{9}x_{21}+x_{8}x_{20}+x_{7}x_{19}-1,\
    x_{12}x_{18}+2x_{11}x_{17}-x_{10}x_{16}+2x_{9}x_{15}-2x_{8}x_{14}-x_{7}x_{13},\
    x_{6}x_{18}-x_{4}x_{16}-2x_{2}x_{14}-x_{1}x_{13}+1,\
    x_{5}x_{18}+x_{4}x_{17}+x_{2}x_{15},\
    x_{3}x_{18}+x_{2}x_{17}-x_{1}x_{15},\
    x_{6}x_{17}+x_{5}x_{16}+x_{3}x_{14},\
    x_{5}x_{17}+x_{4}x_{16}+x_{2}x_{14}-1,\
    x_{3}x_{17}+x_{2}x_{16}-x_{1}x_{14},\
    x_{6}x_{15}+x_{5}x_{14}-x_{3}x_{13},\
    x_{5}x_{15}+x_{4}x_{14}-x_{2}x_{13},\
    x_{3}x_{15}+x_{2}x_{14}+x_{1}x_{13}-1)", true, false, "CalculatorFunctionsGeneral::innerSolveSerreLikeSystemUpperLimit");
  this->AddOperationInnerHandler
  ("FindOneSolutionSerreLikePolynomialSystemAlgebraic", CalculatorFunctionsGeneral::innerSolveSerreLikeSystemAlgebraic, "",
   "Same as FindOneSolutionSerreLikePolynomialSystem but starts directly over algebraic closure.\
    ",
   "FindOneSolutionSerreLikePolynomialSystemAlgebraic{}( \
    x^2+1, y x z -1, z^2 x+y-1, w u)", true, false, "CalculatorFunctionsGeneral::innerSolveSerreLikeSystemAlgebraic");
  this->AddOperationInnerHandler
  ("FindOneSolutionSerreLikePolynomialSystemAlgebraicUpperLimit",
   CalculatorFunctionsGeneral::innerSolveSerreLikeSystemAlgebraicUpperLimit, "",
   "Same as FindOneSolutionSerreLikePolynomialSystemAlgebraic but the first argument gives upper limits \
   to the number of polynomial computations that can be carried out.\
    ",
   "FindOneSolutionSerreLikePolynomialSystemAlgebraicUpperLimit{}( 10000, \
    x^2+1, y x z -1, z^2 x+y-1, w u)", true, false, "CalculatorFunctionsGeneral::innerSolveSerreLikeSystemAlgebraicUpperLimit");
  this->AddOperationInnerHandler
  ("GroebnerLexUpperLimit", this->innerGroebnerLex, "",
   "Transforms to a reduced Groebner basis using the  \
    lexicographic order. \
    The lexicographic order is inherited from the comparison of the underlying expressions.\
    <b>The first argument gives an upper bound to the number of polynomial operations allowed.</b> \
    A non-positive number signifies no upper limit, however please do not use (this is a public \
    web server and multiple instances of a large computation might hog it up). \
    The resulting printout will let your know whether the upper limit was hit or not. \
    <br>\
    <b>Description of the algorithm.</b>\
    Let f_1 and f_2 be two polynomials. Following Cox, Little, O'Shea, \
    \"Ideals, Varieties, Algorithms\", page 81,\
    denote by S(f_1, f_2) the symmetric difference of f_1 and f_2. More precisely, let\
    x^{\\gamma_1} be the leading monomial of f_1 and x^{\\gamma_2} \
    be the leading monomial of f_2, with leading coefficients c_1 and c_2. Then define \
    S(f_1, f_2)= c_2* f_1* lcm (x^\\gamma_1, \\gamma_2)/x^\\gamma_1 -\
    c_1* f_2 *lcm (x^\\gamma_1, x^\\gamma_2)/x^\\gamma_2. \
    Here lcm stands for least common multiple of monomials, defined in the obvious way. \
    <br>\n1. Allocate two buckets of polynomials - one \"main\" bucket and\
    additional bucket. At any given moment in the algorithm, \
    the union of main bucket and the additional bucket should give a basis of the ideal.  \
    <br>\n2. Move all input elements into the additional bucket.\
    <br>\n3. Call the <b>reduce additional bucket</b> subroutine. In the c++ implementation\
    the function is  called GroebnerBasisComputation::AddPolyAndReduceBasis.\
    <br>\n4. Set changed to be true.\
    <br>\n5. While (changed)\
    <br>\n5.1 For all elements f_1, f_2 in the main bucket form the symmetric difference \
    S(f_1, f_2) and add it to the additional bucket. \
    <br>\n5.2 Call the <b>reduce additional bucket</b> subroutine.  \
    and set changed to be equal to the result of the subroutine.\
    <br><b>Reduce additional bucket</b> subroutine.\
    <br>\n1. Set changedMainBucket to be false.\
    <br>\n2.While (additional bucket is not empty)\
    <br>\n2.1. While (additional bucket is not empty)\
    <br>\n2.1.1 Remove the top element from the additional bucket; call that element currentElement.\
    <br>\n2.1.2 Divide currentElement by the elements of the main bucket, and \
    record the resulting remainder element in currentRemainder.\
    Here we use the multivariate \
    polynomial division algorithm, page 62 of Cox, Little, O'Shea, \
    \"Ideals, Varieties, Algorithms\".\
    <br>\n2.1.3 If currentRemainder is non-zero, add it to the main bucket \
    and set changedMainBucket to be true. \
    <br>\n2.2 For each element in the main bucket \
    <br>\n2.2.1 Call the considered element currentElement.\
    <br>\n2.2.2 Divide currentElement by the remaining elements (excluding currentElement itself)\
    in the main bucket. Call the remainder of the division currentRemainder.\
    <br>\n2.2.3 If currentRemainder is not equal to currentElement, \
    remove currentElement from the main bucket and add currentRemainder to the additional bucket. \
    Note that this operation modifies the main bucket: each element of the main bucket must be\
    traversed exactly once by the current cycle, but the division is carried with \
    the modified state of the main bucket. \
    <br>\n3. Return changedMainBucket.  <br><b>End of algorithm description.</b>\
   ",
   "GroebnerLexUpperLimit{}(10000, s^2+c^2+1, a-s^4, b-c^4 );\
   \nGroebnerLexUpperLimit{}(5, s^2+c^2+1, a-s^4, b-c^4 );");
    this->AddOperationInnerHandler
  ("GroebnerRevLexUpperLimit", this->innerGroebnerRevLex, "",
   "Same as GroebnerLexUpperLimit but uses reverse order on the variables (z<x).",
   "GroebnerRevLexUpperLimit{}(10000, s^2+c^2+1, a-s^4, b-c^4 );\
   \nGroebnerRevLexUpperLimit{}(5, s^2+c^2+1, a-s^4, b-c^4 );");

  this->AddOperationInnerHandler
  ("GroebnerGrLexUpperLimit", this->innerGroebnerGrLex, "",
   "Transforms to a reduced Groebner basis relative to the graded \
   lexicographic order. In the graded lexicographic order, monomials are first compared by\
   total degree, then by lexicographic order. \
   The lexicographic order is inherited from the comparison of the underlying expressions. \
   <b>The first argument gives an upper bound to the number of polynomial operations allowed.</b> \
   A non-positive number signifies no upper limit, however please do not use (this is a public \
   web server and multiple instances of a large computation might hog it up). \
   The resulting printout will let your know whether the upper limit was hit or not. \
   For a description of the algorithm used see the description of function GroebnerLexUpperLimit.",
   "GroebnerGrLexUpperLimit{}(10000, a^2+b^2+1, x-a^4, y-b^4 );\n \
   GroebnerGrLexUpperLimit{}(5, a^2+b^2+1, x-a^4, y-b^4 )");

  this->AddOperationInnerHandler
  ("ComputeFKFT", CalculatorFunctionsGeneral::innerComputePairingTablesAndFKFTsubalgebras, "",
   "Attempts to compute all Fernando-Kac subalgebras according to the most experimental, latest and greatest algorithm. Argument must be of type \
   semisimple Lie subalgebras. ",
   "ComputeFKFT( ComputeSemisimpleSubalgebras(c_3))", false, false,
   "CalculatorFunctionsGeneral::innerComputePairingTablesAndFKFTsubalgebras");
  this->AddOperationInnerHandler
  ("ComputeSemisimpleSubalgebras", CalculatorFunctionsGeneral::innerComputeSemisimpleSubalgebras, "",
   "Computes the semisimple subalgebras of a semisimple Lie algebra and creates a data structure containing them. ",
   "ComputeSemisimpleSubalgebras(A_2)", false,
   false, "CalculatorFunctionsGeneral::innerComputeSemisimpleSubalgebras");
  this->AddOperationInnerHandler
  ("CentralizerChains", CalculatorFunctionsGeneral::innerGetCentralizerChainsSemisimpleSubalgebras, "",
   "Creates a printout with centralizer chains of semisimple Lie subalgebras. ",
   "CentralizerChains (ComputeSemisimpleSubalgebras{}(B_3))");

  this->AddOperationInnerHandler
  ("printSemisimpleSubalgebras", this->innerPrintSSsubalgebrasRegular, "",
   " <b>This function is being developed and is not imiplemented fully yet. </b> \
   Prints the semisimple subalgebras of a semisimple Lie algebra. ",
   "printSemisimpleSubalgebras(B_3)", true, false);
  this->AddOperationInnerHandler
  ("printSemisimpleSubalgebrasRecompute", Calculator::innerPrintSSsubalgebrasRecompute, "",
   " <b>This function is being developed and is not imiplemented fully yet. </b> \
   Prints the semisimple subalgebras of a semisimple Lie algebra. ",
   "printSemisimpleSubalgebrasRecompute(C_3)", false, false, "Calculator::innerPrintSSsubalgebrasRecompute");
  this->AddOperationInnerHandler
  ("printSemisimpleSubalgebrasNoCentralizers", this->innerPrintSSsubalgebrasNoCentralizers, "",
   " <b>This function is being developed and is not imiplemented fully yet. </b> \
   Prints the semisimple subalgebra candidates of a Lie algebra. ",
   "printSemisimpleSubalgebrasNoCentralizers(A_3)", false, false);
  this->AddOperationInnerHandler
  ("printSemisimpleSubalgebrasFull", this->innerPrintSSsubalgebrasNilradicals, "",
   " <b>This function is being developed and is not imiplemented fully yet. </b> \
   Prints the semisimple subalgebras of a semisimple Lie algebra. ",
   "printSemisimpleSubalgebrasFull{}(A_2)", false, false);
  this->AddOperationInnerHandler
  ("CanBeExtendedParabolicallyTo", CalculatorFunctionsGeneral::innerCanBeExtendedParabolicallyTo, "",
   "Finds whether a Dynkin type extends via the standard parabolic extension to another. ",
   "CanBeExtendedParabolicallyTo(A^3_1, A_5); CanBeExtendedParabolicallyTo(a_2, b_3); CanBeExtendedParabolicallyTo(g_2, b_3);\
   CanBeExtendedParabolicallyTo(d_5, e_6); CanBeExtendedParabolicallyTo(b_3, a_10); CanBeExtendedParabolicallyTo(c_2, c_10);\
   CanBeExtendedParabolicallyTo(b_3+e_6+a_3, d_4+e_8+f_4)\
   ", true, false);
  this->AddOperationInnerHandler
  ("EmbedSemisimpleInSemisimple", CalculatorFunctionsGeneral::innerEmbedSSalgInSSalg, "",
   " Trys to find all embeddings of the first semisimple type into the second. Records all intermediate subalgebras. ",
   "EmbedSemisimpleInSemisimple{}(G^3_2, B_3);EmbedSemisimpleInSemisimple{}(G_2, B_3)", true, false);
  this->AddOperationInnerHandler
  ("LoadSemisimpleSubalgebras", CalculatorConversions::innerLoadSemisimpleSubalgebras, "",
   " <b>This function is being developed and is not imiplemented fully yet. </b> \
   Loads a semisimpleSubalgebra from expression. ",
   "LoadSemisimpleSubalgebras {}(EmbedSemisimpleInSemisimple{}(G_2, B_3))", true, false)
   ;
  this->AddOperationInnerHandler
  ("SltwoSubalgebra", CalculatorConversions::innerSlTwoSubalgebraPrecomputed, "",
   " <b>This function is being developed and is not imiplemented fully yet. </b> \
   Loads an sl(2) subalgebra from expression. ",
   "SltwoSubalgebra(B_{3},g_  +(B)_{3}, -1)\\\\\
    +3 ((getChevalleyGenerator, (B)_{3}, -2)), \
    3 ((getChevalleyGenerator, (B)_{3}, 3))\\\\\
    +6 ((getChevalleyGenerator, (B)_{3}, 1))\\\\\
    +10/3 ((getChevalleyGenerator, (B)_{3}, 2)))", true, true)
   ;
  this->AddOperationInnerHandler
  ("LoadSlTwoSubalgebras", CalculatorConversions::innerLoadSltwoSubalgebras, "",
   " <b>This function is being developed and is not imiplemented fully yet. </b> \
   Loads the sl(2) subalgebras of a semisimple Lie algebra from expression. ",
   "LoadSemisimpleSubalgebras{}(experimentalEmbedSemisimpleInSemisimple{}(G_2, B_3))", true, true)
   ;

//     this->AddOperationInnerHandler
//  ("printAllPartitions", & this->fPrintAllPartitions, "",
//   "Prints (Kostant) partitions . ",
//   "printAllPartitions{}(A_2, (2,3))");
//  this->AddOperationInnerHandler
//  ("Differential", & this->fDifferential, "",
//   "Differential. ",
//   "d{}{{a}}=Differential{}a;\nx=Polynomial{}x;\nd{}(x^2/(x+1))");
  this->AddOperationInnerHandler
  ("\\sqrt", this->innerSqrt, "",
   "Square root of a rational, implemented as algebraic extension of the rationals. ",
   "\\sqrt 2+\\sqrt 3;(\\sqrt{}2+\\sqrt{}3+\\sqrt{}6)^2", true);

  this->AddOperationInnerHandler
  ("FactorOneVarPolyOverRationals", this->innerFactorPoly, "",
   "Factors a one variable polynomial over the rationals using Kroenecker's method. \
    After clearing denominators, assume the poly has integer coefficients.\
    If looking for an integer coefficient divisor of degree k, \
    plug in k different integer values of the poly\
    and finds the set of all possible divisors of the value of poly at the k points.\
    this gives a finite set of possibilities for the divisors, as interpolated by\
    Lagrange polynomials.",
   "FactorOneVarPolyOverRationals{}(x^{8}-44x^{6}+438x^{4}-1292x^{2}+529); \
   FactorOneVarPolyOverRationals{}(x^{8}+2x^{7}-3x^{6}-4x^{5}+6x^{4}+2x^{3}-13x^{2}+1)", true, false)
   ;
  this->AddOperationInnerHandler
  ("Freudenthal", this->innerFreudenthalEval, "",
   "Computes the dominant weights with multiplicities of a finite dimensional \
   module of a highest weight given in fundamental coordinates. The first argument gives \
   the semisimple Lie algebra type, the second argument gives the highest weight in \
   fundamental coordinates. ",
   "Freudenthal{}(B_3, (2,2,2))", true, false)
   ;
  this->AddOperationInnerHandler
  ("Killing", this->innerKillingForm, "",
   "Computes the Killing form product of two elements of semisimple Lie algebra. ",
   "h_{{i}}=getCartanGenerator{}(F_1, i);\
    KF{}({{i}},{{j}})=Killing{}(h_i, h_j);\
    FunctionToMatrix(KF, 4, 4)", true, false)
   ;
  this->AddOperationInnerHandler
  ("FreudenthalFull", this->innerFreudenthalFull, "",
   "Computes the full character and prints it to screen. \
    Argument format same as the Freudenthal function. ",
   "FreudenthalFull{}(G_2, (2,1))", true, false)
   ;
   this->AddOperationInnerHandler
  ("TestMouseHover", CalculatorFunctionsGeneral::innerTestMathMouseHover, "",
   "This is not a mathematical function; instead it is an html/javascript test function. Converts the input to a math mouse hover class.",
   "TestMouseHover(x^2)", true, false)
   ;
   this->AddOperationInnerHandler
  ("CoefficientOf", CalculatorFunctionsGeneral::innerCoefficientOf, "",
   "Gets the coefficient of the first argument in the second. ",
   "CoefficientOf(y, x*x*y+x*z*y*z+x*y*x)", true)
   ;


//  this->AddOperationInnerHandler
//  ("minPoly", & this->fMinPoly, "",
//   "If the argument of the function is an algebraic number returns its minimal polynomial, else does nothing. ",
//   "minPoly{}(\\sqrt{}2+\\sqrt{}3)");

}

void Calculator::initPredefinedStandardOperations()
{ //IMPORTANT.
  //The order of registration of operation handlers for the same operation name
  // defines the order in which operation
  //handlers are called. Operations handlers registered first are executed first.
  //The order only matters for different handlers of the same operation.
  //This order is very important, as some of the
  //handlers will act properly only if the preceding ones have been carried through
  //(example: outerExtractBaseMultiplication requires outerAssociate).
  //Note that one can easily transform the code so that the order does not matter.
  //One can do that by ``packing'' the the correct order of operations into a super-function,
  //or by making the corresponding handlers call each other as needed.
  //A combination of these two was indeed the original design approach, however what is used now
  //is more modular, conceptual, and easier to test: in short, the better engineering solution.
  this->AddOperationOuterHandler
  (";", this->outerMeltBrackets, "",
   "Melts down a layer of parenthesis around a list of commands \
   prepended with the Melt operation.\
   \n More precisely,\
   cycles through all immediate children of the expression. \
   \nIf the k^th child X is a list of two elements starting with Melt, \
   then the operation does the following.\
   If the second child Y of X is a list starting with EndStatement(;), \
   then X is replaced with the second, third, ... children of Y. \
   If Y is not a list starting with EndStatement, X is replaced with Y.\
   ", "c=(a=b);\na;\nc;\na;\nd=(e=f; g=h);\nd;\ne;\nMelt{}d;\ne;\ng;  ", true);
  this->AddOperationOuterHandler
  ("=", this->outerCheckRule, "",
   "Checks whether the rule is of the form A=A, and substitutes the expression with an error if that \
   is the case. This usually happens when a general rule is entered twice.\
   In the following example, we try to redefine the associative rule\
   of the calculator. This fails because the associative rule is already implemented:\
   the left hand side of the below expression is substituted with a*(b*c), and thus the rule becomes\
   a*(b*c)=a*(b*c), which clearly is infinite substitution.",
   "%LogEvaluation\n({{a}}*{{b}})*{{c}}=a*(b*c);  ", true);

  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddRatToRat, this->opRational(), this->opRational(),
   "Adds two rational numbers. ",
   "2+3", true, false, "CalculatorFunctionsBinaryOps::innerAddRatToRat");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddEltZmodPorRatToEltZmodPorRat, this->opRational(), this->opEltZmodP(),
   "Adds elements of Z_p. ",
   " (2 mod  7)+3", true, false, "CalculatorFunctionsBinaryOps::innerAddEltZmodPorRatToEltZmodPorRat");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddEltZmodPorRatToEltZmodPorRat, this->opEltZmodP(), this->opRational(),
   "Adds elements of Z_p. ",
   " (2 mod  7)+3", true, false, "CalculatorFunctionsBinaryOps::innerAddEltZmodPorRatToEltZmodPorRat");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddEltZmodPorRatToEltZmodPorRat, this->opEltZmodP(), this->opEltZmodP(),
   "Adds elements of Z_p. ",
   " (2 mod  7)+3", true, false, "CalculatorFunctionsBinaryOps::innerAddEltZmodPorRatToEltZmodPorRat");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddAlgebraicNumberToAlgebraicNumber, this->opAlgNumber(), this->opAlgNumber(),
   "Adds two algebraic numbers. ",
   "\\sqrt {2}+ \\sqrt {3} + \\sqrt{6}", true, false, "CalculatorFunctionsBinaryOps::innerAddAlgebraicNumberToAlgebraicNumber");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddAlgebraicNumberToAlgebraicNumber, this->opAlgNumber(), this->opRational(),
   "Adds algebraic number to rational. ",
   "1/(\\sqrt {2}+ 1+\\sqrt{3}+\\sqrt{6})", true, false, "CalculatorFunctionsBinaryOps::innerAddAlgebraicNumberToAlgebraicNumber");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddAlgebraicNumberToAlgebraicNumber, this->opRational(), this->opAlgNumber(),
   "Adds rational to algebraic number. ",
   "1/(1+\\sqrt {2}+\\sqrt{}6)", true, false, "CalculatorFunctionsBinaryOps::innerAddAlgebraicNumberToAlgebraicNumber");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddDoubleOrRatToDoubleOrRat, this->opDouble(), this->opRational(),
   "Adds double or rational to a double or rational approximately using the built-in cpp \
   addition, returning double. ",
   "DoubleValue{}(3.14159265358979323846)+1"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddDoubleOrRatToDoubleOrRat");
  this->AddOperationOuterHandler
  ("+", Calculator::outerPlus, "",
   "Collects all terms (over the rationals), adding up terms proportional up to a rational number. \
    Zero summands are removed, unless zero is the only term left. ", "1+a-2a_1+1/2+a_1", true, false,
    "Calculator::outerPlus");
  this->AddOperationOuterHandler
  ("+", CalculatorFunctionsGeneral::outerCombineFractionsCommutative, "",
   "Combines fractions on condition that all participants commute. \
   Equivalent to {{a}}/{{b}}+{{c}}/{{d}}=(a *d+c*b)/(d*b); \
   Please note that this transformation is not correct if b and d do not commute. ",
   "a/b+c/d", true, false, "CalculatorFunctionsGeneral::outerCombineFractionsCommutative");

  this->AddOperationOuterHandler
  ("+", this->outerCombineFractions, "",
   "Combines fractions. Equivalent to {{a}}/{{b}}+{{c}}=(a+c*b)/b; ",
   "f{}{{x}}=(2x+3)/(2x+1);\ng{}{{y}}=(y-2)/(y+3);\ng{}f{}z;\nf{}g{}z", true);
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddEltTensorToEltTensor, this->opElementTensorGVM(), this->opElementTensorGVM(),
   "Adds two elements of tensor products of generalized Verma modules. ",
   "v=hwv{}(G_2, (1,0),(0,0));\
   \n(3/4 v)\\otimes v-3/4 (v\\otimes v)", true, false, "CalculatorFunctionsBinaryOps::innerAddEltTensorToEltTensor");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddPlotToPlot, this->opCalculusPlot(), this->opCalculusPlot(),
   "Superimposes two plots. ",
   "plot2D{}(sin{}(x), -5, 5)+ plot2D{}(1/sin{}(x ), 0.1, 3.041592654)",
   true, false, "CalculatorFunctionsBinaryOps::innerAddPlotToPlot");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly, this->opRational(), this->opPoly(),
   "Adds a rational to a polynomial. ",
   "1+Polynomial{}\\lambda; Polynomial{}\\lambda+1"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly, this->opPoly(), this->opRational(),
   "Adds a polynomial to a rational. ",
   "1+Polynomial{}\\lambda; Polynomial{}\\lambda+1"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly, this->opPoly(), this->opPoly(),
   "Adds a polynomial to a polynomial. ",
   "x=1+Polynomial{}\\lambda; x+x"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly");

  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly, this->opPolyOverANs(), this->opPolyOverANs(),
   "Adds a polynomial over the algebraic numbers to a polynomial over the algebraic numbers. ",
   "PolynomialAlgebraicNumbers(\\sqrt{12}x)+PolynomialAlgebraicNumbers(-\\sqrt{3}x)"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly, this->opAlgNumber(), this->opPolyOverANs(),
   "Adds a polynomial over the algebraic numbers to a polynomial over the algebraic numbers. ",
   "PolynomialAlgebraicNumbers(\\sqrt{12})+PolynomialAlgebraicNumbers(\\sqrt{3}x)"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly, this->opPolyOverANs(), this->opAlgNumber(),
   "Adds a polynomial over the algebraic numbers to a polynomial over the algebraic numbers. ",
   "PolynomialAlgebraicNumbers(\\sqrt{12}x)+PolynomialAlgebraicNumbers(-\\sqrt{3})"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly, this->opRational(), this->opPolyOverANs(),
   "Adds a polynomial over the algebraic numbers to a polynomial over the algebraic numbers. ",
   "1+PolynomialAlgebraicNumbers(\\sqrt{12}x)"
   , true, "CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly, this->opPolyOverANs(), this->opRational(),
   "Adds a polynomial over the algebraic numbers to a polynomial over the algebraic numbers. ",
   "PolynomialAlgebraicNumbers(\\sqrt{12}x)+1"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly, this->opPoly(), this->opAlgNumber(),
   "Adds a polynomial over the algebraic numbers to a polynomial over the algebraic numbers. ",
   "PolynomialAlgebraicNumbers(x)+\\sqrt{2}"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly, this->opPoly(), this->opPolyOverANs(),
   "Adds a polynomial over the algebraic numbers to a polynomial over the algebraic numbers. ",
   "Polynomial{}x+ PolynomialAlgebraicNumbers(\\sqrt{2}x)"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly, this->opPolyOverANs(), this->opPoly(),
   "Adds a polynomial over the algebraic numbers to a polynomial over the algebraic numbers. ",
   "PolynomialAlgebraicNumbers(\\sqrt{2}x)+Polynomial{}x"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly, this->opAlgNumber(), this->opPoly(),
   "Adds a polynomial over the algebraic numbers to a polynomial over the algebraic numbers. ",
   "\\sqrt{2}+PolynomialAlgebraicNumbers(x)"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddNumberOrPolyToNumberOrPoly");

  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddRatOrPolyOrEWAToRatOrPolyOrEWA, this->opRational(), this->opElementWeylAlgebra(),
   "Adds a rational or polynomial to element weyl algebra. ",
   " \\partial_{{i}}=ElementWeylAlgebraDO{}(\\partial_i, x_i); \nx_{{i}}=Polynomial{}x_i;\nx_i\\partial_i-\\partial_i x_i-[x_i, \\partial_i]"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddRatOrPolyOrEWAToRatOrPolyOrEWA");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddRatOrPolyOrEWAToRatOrPolyOrEWA, this->opPoly(), this->opElementWeylAlgebra(),
   "Adds a rational or polynomial to element weyl algebra. ",
   " \\partial_{{i}}=ElementWeylAlgebraDO{}(\\partial_i, x_i); \nx_{{i}}=Polynomial{}x_i;\nx_i+\\partial_i+x_i\\partial_i-\\partial_i x_i-[x_i, \\partial_i]"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddRatOrPolyOrEWAToRatOrPolyOrEWA");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddRatOrPolyOrEWAToRatOrPolyOrEWA, this->opElementWeylAlgebra(), this->opPoly(),
   "Adds a rational or polynomial to element weyl algebra. ",
   " \\partial_{{i}}=ElementWeylAlgebraDO{}(\\partial_i, x_i); \nx_{{i}}=Polynomial{}x_i;\nx_i+x_i\\partial_i-\\partial_i x_i-[x_i, \\partial_i]"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddRatOrPolyOrEWAToRatOrPolyOrEWA");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddRatOrPolyOrEWAToRatOrPolyOrEWA, this->opElementWeylAlgebra(), this->opElementWeylAlgebra(),
   "Adds a rational or polynomial to element weyl algebra. ",
   " \\partial_{{i}}=ElementWeylAlgebraDO{}(\\partial_i, x_i); \nx_{{i}}=Polynomial{}x_i;\nx_i\\partial_i-\\partial_i x_i-[x_i, \\partial_i]"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddRatOrPolyOrEWAToRatOrPolyOrEWA");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddRatOrPolyOrRFToRatOrPolyOrRF, this->opRationalFunction(), this->opRationalFunction(),
   "Adds a rational function to a rational function. ",
   "WeylDimFormula{}(a_2, (0,3)) + WeylDimFormula{}(a_2, (3,0)) + 4 WeylDimFormula{}(a_2, (1,1)) "
   , true, false, "CalculatorFunctionsBinaryOps::innerAddRatOrPolyOrRFToRatOrPolyOrRF");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddDoubleOrRatToDoubleOrRat, this->opRational(), this->opDouble(),
   "Adds double or rational to a double or rational approximately using the built-in cpp \
   addition, returning double. ",
   "DoubleValue{}(3.14159265358979323846)+1"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddDoubleOrRatToDoubleOrRat");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddDoubleOrRatToDoubleOrRat, this->opDouble(), this->opDouble(),
   "Adds double or rational to a double or rational approximately using the built-in cpp \
   addition, returning double. ",
   "DoubleValue{}(3.14159265358979323846)+1"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddDoubleOrRatToDoubleOrRat");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddWeightToWeight, this->opWeightLieAlgPoly(), this->opWeightLieAlgPoly(),
   "Adds two weights. ",
   "\\lambda=Polynomial{}\\lambda; \\varepsilon_{{a}}=MakeWeight{}(B_3, a, epsilon); (1/2+\\lambda)\\varepsilon_1+1/2\\varepsilon_2+1/2\\varepsilon_3  "
   , true, false, "CalculatorFunctionsBinaryOps::innerAddWeightToWeight");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddUEToAny, this->opElementUEoverRF(), this->opElementUEoverRF(),
   "Adds an element of UE (Universal Enveloping algebra) to an element of UE.",
   " g_{{{i}}}=getChevalleyGenerator{}(F_{1}, {{i}});\nh_{{{i}}}=getCartanGenerator{}(F_{1}, {{i}});\n\
   [g_{22}+g_{20}+g_{14},g_{-14}+g_{-20}+g_{-22}]"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddUEToAny");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddSequenceToSequence, this->opSequence(), this->opSequence(),
   "Adds two sequences (termwise), provided the sequences have the same number of entries. \
   In case the entries of the sequences are elements of a base field, corresponds to vector addition.",
   "v_{1}=(1, 2, 3);\nv_{2}=(1, 3, 2);\nv_{3}=(3, 1, 1);\nv_{4}=(-2, 2, 2);\n1/2v_{1}+1/2v_{2}+7/8v_{3}+13/16v_{4}"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddSequenceToSequence");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddMatrixRationalOrAlgebraicToMatrixRationalOrAlgebraic,
   this->opMatRat(), this->opMatRat(),
   "Adds two matrices.",
   " A=MakeMatrix{}((5, 8), (3, 5)); A*A-A; B=MakeMatrix{}((5, 8), (3, 5)); A+B"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddMatrixRationalOrAlgebraicToMatrixRationalOrAlgebraic");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddMatrixRationalOrAlgebraicToMatrixRationalOrAlgebraic,
   this->opMatAlgebraic(), this->opMatAlgebraic(),
   "Adds two matrices.",
   " A=MakeMatrix{}((\\sqrt{}5, 8), (\\sqrt{}3, 5)); A*A-A; A+A*A;"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddMatrixRationalOrAlgebraicToMatrixRationalOrAlgebraic");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddMatrixRationalOrAlgebraicToMatrixRationalOrAlgebraic,
   this->opMatRat(), this->opMatAlgebraic(),
   "Adds two matrices.",
   " A=MakeMatrix{}((5, 8), (3, 5)); B=MakeMatrix{}((\\sqrt{}5, 8), (\\sqrt{}3, 5)); A+B; B+A"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddMatrixRationalOrAlgebraicToMatrixRationalOrAlgebraic");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddMatrixRationalOrAlgebraicToMatrixRationalOrAlgebraic,
   this->opMatAlgebraic(), this->opMatRat(),
   "Adds two matrices.",
   " A=MakeMatrix{}((5, 8), (3, 5)); B=MakeMatrix{}((\\sqrt{}5, 8), (\\sqrt{}3, 5)); A+B; B+A"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddMatrixRationalOrAlgebraicToMatrixRationalOrAlgebraic");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddMatrixRFsToMatrixRFs,
   this->opMatRF(), this->opMatRF(),
   "Adds two matrices.",
   " A=MatrixRFs{}((5, 8z), (3, 5)); A*A-A;"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddMatrixRationalToMatrixRational");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("+", CalculatorFunctionsBinaryOps::innerAddMatrixTensorToMatrixTensor,
   this->opMatTensorRat(), this->opMatTensorRat(),
   "Adds two matrices.",
   " A=MakeMatrixTensorForm{}((5, 8), (3, 5)); 3A*A-A;"
   , true, false, "CalculatorFunctionsBinaryOps::innerAddMatrixTensorToMatrixTensor");

  this->AddOperationOuterHandler
  ("-", this->outerMinus, "",
   "Transforms a-b to a+(-1)*b and -b to (-1)*b. Equivalent to a rule \
   -{{b}}=MinnusOne*b; {{a}}-{{b}}=a+MinnusOne*b", "-1+(-5)", true);

  this->AddOperationOuterHandler
  ("*", Calculator::outerTimesToFunctionApplication, "",
   "On condition that F is a built-int function name or built-in operation, replaces F*x with F{}x.",
   "plot2D(\\sin{}x+cos{}x, 0, 5) ", true, false, "Calculator::outerTimesToFunctionApplication");
  this->AddOperationInnerHandler
  ("*", CalculatorFunctionsGeneral::innerCompositeMultiplyIntegralFbyDx, "",
   "Transformation: (\\int{} f) dx= \\int{}(f dx)",
   "(\\int x)dx", true, false, "CalculatorFunctionsGeneral::innerCompositeMultiplyIntegralFbyDx");

  this->AddOperationOuterHandler
  ("*", CalculatorFunctionsGeneral::outerDifferentiateWRTxTimesAny, "",
   "Replaces Differentiate{}(x)*a by  Differentiate{}(x,a).",
   "d/dx (1/x) ", true, false, "CalculatorFunctionsGeneral::outerDifferentiateWRTxTimesAny");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyEltZmodPorRatByEltZmodPorRat, this->opRational(), this->opEltZmodP(),
   "Multiplies elements of Z_p. ",
   " (2 mod  7)*3", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyEltZmodPorRatByEltZmodPorRat");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyEltZmodPorRatByEltZmodPorRat, this->opEltZmodP(), this->opEltZmodP(),
   "Multiplies elements of Z_p. ",
   " (2 mod  7)*3", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyEltZmodPorRatByEltZmodPorRat");
    this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyAlgebraicNumberByAlgebraicNumber, this->opAlgNumber(), this->opAlgNumber(),
   "Multiplies two algebraic numbers. ",
   "\\sqrt{}2(\\sqrt {2}* \\sqrt {3} +\\sqrt{}2)", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyAlgebraicNumberByAlgebraicNumber");
    this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyAlgebraicNumberByAlgebraicNumber, this->opRational(), this->opAlgNumber(),
   "Multiplies two algebraic number by rational. ",
   "2(\\sqrt {2}+\\sqrt{}3)", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyAlgebraicNumberByAlgebraicNumber");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps:: innerMultiplyRatByRat, this->opRational(), this->opRational(),
   "Multiplies two rationals. ",
   "2*3", true, false, "CalculatorFunctionsBinaryOps:: innerMultiplyRatByRat");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyDoubleOrRatByDoubleOrRat, this->opRational(), this->opDouble(),
   "Multiplies rational by a double approximately using the built-in cpp multiplication \
   returning double. The cpp multiplication is supposed to call the system's \
   hardware double multiplication routine. ",
   "DoubleValue{}(1/3)*3; \
   \nz=101^20;\
   \nDoubleValue{}(z);\
   \nDoubleValue{}(z)+DoubleValue{}(1)-DoubleValue{}(z); \
   \n(z+1)-z;\
   \n y=101^200;\
   \nDoubleValue(y)"
   , true, false, "CalculatorFunctionsBinaryOps::innerMultiplyDoubleOrRatByDoubleOrRat");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyDoubleOrRatByDoubleOrRat, this->opDouble(), this->opRational(),
   "Multiplies rational by a double approximately using the built-in cpp multiplication \
   returning double. The cpp multiplication is supposed to call the system's \
   hardware double multiplication routine. ",
   "DoubleValue{}(1/3)*3; \
   \nDoubleValue{}((101)^{20});\
   \nDoubleValue{}(DoubleValue{}((101)^{20})+DoubleValue{}(1))-DoubleValue{}(101^{20})"
   , true, false, "CalculatorFunctionsBinaryOps::innerMultiplyDoubleOrRatByDoubleOrRat");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyDoubleOrRatByDoubleOrRat, this->opDouble(), this->opDouble(),
   "Multiplies rational by a double approximately using the built-in cpp multiplication \
   returning double. The cpp multiplication is supposed to call the system's \
   hardware double multiplication routine. ",
   "DoubleValue{}(1/3)*3; \
   \nDoubleValue{}((101)^{20});\
   \nDoubleValue{}(DoubleValue{}((101)^{20})+DoubleValue{}(1))-DoubleValue{}(101^{20})"
   , true, false, "CalculatorFunctionsBinaryOps::innerMultiplyDoubleOrRatByDoubleOrRat");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyCoxeterEltByCoxeterElt, this->opWeylGroupElement(), this->opWeylGroupElement(),
   "Multiplies two coxeter elements if possible. ",
   "x=MakeElementWeylGroup{}(A_2, 1); x*x", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyCoxeterEltByCoxeterElt");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyCharSSLieAlgByCharSSLieAlg, this->opCharSSAlgMod(), this->opCharSSAlgMod(),
   "Multiplies two semisimple Lie algebra finite dimensinal characters and decomposes using the \
   Brauer-Klimyk formula, Humphreys J. Introduction to Lie algebras and representation theory, \
   page 142, exercise 9. ",
   "x=MakeCharacterLieAlg{}(G_2, (1,0));\ny=MakeCharacterLieAlg{}(G_2, (0,1));\nx*y", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyCharSSLieAlgByCharSSLieAl");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyRatOrPolyOrEWAByRatOrPolyOrEWA, this->opRational(), this->opElementWeylAlgebra(),
   "Multiplies rational or polynomial or element weyl algebra by rational or polynomial or element weyl algebra. ",
   " \\partial_{{i}}=ElementWeylAlgebraDO{}(\\partial_i, x_i); \nx_{{i}}=Polynomial{}x_i; 3\\partial_i", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyRatOrPolyOrEWAByRatOrPolyOrEWA");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyRatOrPolyOrEWAByRatOrPolyOrEWA, this->opPoly(), this->opElementWeylAlgebra(),
   "Multiplies rational or polynomial or element weyl algebra by rational or polynomial or element weyl algebra. ",
   " \\partial_{{i}}=ElementWeylAlgebraDO{}(\\partial_i, x_i); \nx_{{i}}=Polynomial{}x_i;\nx_i\\partial_i-\\partial_i x_i-[x_i, \\partial_i]", true,
   false, "CalculatorFunctionsBinaryOps::innerMultiplyRatOrPolyOrEWAByRatOrPolyOrEWA");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyRatOrPolyOrEWAByRatOrPolyOrEWA, this->opElementWeylAlgebra(), this->opElementWeylAlgebra(),
   "Multiplies rational or polynomial or element weyl algebra by rational or polynomial or element weyl algebra. ",
   " \\partial_{{i}}=ElementWeylAlgebraDO{}(\\partial_i, x_i); \nx_{{i}}=Polynomial{}x_i;\na=x_1x_2;\nb=\\partial_1\\partial_2; a b - b a -[a,b] ",
   true, false, "CalculatorFunctionsBinaryOps::innerMultiplyRatOrPolyOrEWAByRatOrPolyOrEWA");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyRatOrPolyOrEWAByRatOrPolyOrEWA, this->opElementWeylAlgebra(), this->opPoly(),
   "Multiplies rational or polynomial or element weyl algebra by rational or polynomial or element weyl algebra. ",
   "\\partial_{{i}}=ElementWeylAlgebraDO{}(\\partial_i, x_i); \nx_{{i}}=Polynomial{}x_i;\nx_i\\partial_i-\\partial_i x_i-[x_i, \\partial_i]",
   true, false, "CalculatorFunctionsBinaryOps::innerMultiplyRatOrPolyOrEWAByRatOrPolyOrEWA");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly, this->opPoly(), this->opRational(),
   "Multiplies polynomial by a rational (polynomial comes first). ",
   "2*Polynomial{}(a+b);\nPolynomial{}(a+b)/2;\nPolynomial{}((a+b)^3)*Polynomial{}((a+c)^3);", true,
   false, "CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly, this->opRational(), this->opPoly(),
   "Multiplies rational by a polynomial (rational comes first). ",
   "2*Polynomial{}(a+b);\nPolynomial{}(a+b)/2;\nPolynomial{}((a+b)^3)*Polynomial{}((a+c)^3);", true,
   false, "CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly, this->opPoly(), this->opPoly(),
   "Multiplies two polynomials. ",
   "2*Polynomial{}(a+b);\nPolynomial{}(a+b)/2;\nPolynomial{}((a+b)^3)*Polynomial{}((a+c)^3);", true,
   false, "CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly");

  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly, this->opPolyOverANs(), this->opPolyOverANs(),
   "Multiplies two polynomials over the algebraic numbers. ",
   "PolynomialAlgebraicNumbers{}(\\sqrt{3}x)*PolynomialAlgebraicNumbers{}(\\sqrt{6}y);", true,
   false, "CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly, this->opPolyOverANs(), this->opAlgNumber(),
   "Multiplies two polynomials over the algebraic numbers. ",
   "PolynomialAlgebraicNumbers{}(\\sqrt{3}x)*\\sqrt{6};", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly, this->opAlgNumber(), this->opPolyOverANs(),
   "Multiplies two polynomials over the algebraic numbers. ",
   "\\sqrt{6}*PolynomialAlgebraicNumbers{}(\\sqrt{3}x);", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly, this->opRational(), this->opPolyOverANs(),
   "Multiplies two polynomials over the algebraic numbers. ",
   "PolynomialAlgebraicNumbers{}(\\sqrt{3}x)*2;", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly, this->opPoly(), this->opAlgNumber(),
   "Multiplies two polynomials over the algebraic numbers. ",
   "Polynomial{}(x)*\\sqrt{2};", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly, this->opAlgNumber(), this->opPoly(),
   "Multiplies two polynomials over the algebraic numbers. ",
   "\\sqrt{3}*PolynomialAlgebraicNumbers{}(x);", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyNumberOrPolyByNumberOrPoly");

  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyMatrixRationalOrRationalByMatrixRational,
   this->opMatRat(), this->opMatRat(),
   "Multiplies matrix rational by matrix rational. ",
   "M=MakeMatrix{}((1,1), (0,1)); M; M*M; M*M*M; M*M*M*M; 2*M ", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyMatrixRationalOrRationalByMatrixRational");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyMatrixRFOrRFByMatrixRF,
   this->opRational(), this->opMatRF(),
   "Multiplies a rational number by matrix of polynomial functions. ",
   "M=MatrixRFs{}((x,1/x), (0,1)); M; M*M; M*M*M; M*M*M*M; 2*M ", true, false,
   "CalculatorFunctionsBinaryOps::innerMultiplyMatrixRFOrRFByMatrixRF");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyMatrixRFOrRFByMatrixRF,
   this->opPoly(), this->opMatRF(),
   "Multiplies matrix of polynomial functions by polynomial. ",
   "M=MatrixRFs{}((x,1/x), (0,1)); M; M*M; M*M*M; M*M*M*M; 2*M ", true, false,
   "CalculatorFunctionsBinaryOps::innerMultiplyMatrixRFOrRFByMatrixRF");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyMatrixRFOrRFByMatrixRF,
   this->opMatRF(), this->opMatRF(),
   "Multiplies matrix of polynomial functions by matrix of polynomial functions. ",
   "M=MatrixRFs{}((x,1/x), (0,1)); M; M*M; M*M*M; M*M*M*M; 2*M ", true, false,
   "CalculatorFunctionsBinaryOps::innerMultiplyMatrixRFOrRFByMatrixRF");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyMatrixRFOrRFByMatrixRF,
   this->opRationalFunction(), this->opMatRF(),
   "Multiplies a polynomial by a matrix of polynomials. ",
   "M=MatrixRFs{}((x^2,1), (0,1)); (MakeRationalFunction{}(1/x)) M; ", true, false,
   "CalculatorFunctionsBinaryOps::innerMultiplyMatrixRFOrRFByMatrixRF");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyMatrixTensorOrRationalByMatrixTensor,
   this->opMatTensorRat(), this->opMatTensorRat(),
   "Multiplies matrix rational by matrix tensor. ",
   "M=MakeMatrixTensorForm{}((1,1), (0,1)); M; M*M; M*M*M; M*M*M*M; 2*M ", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyMatrixTensorOrRationalByMatrixTensor");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyMatrixRationalOrRationalByMatrixRational,
   this->opRational(), this->opMatRat(),
   "Multiplies rational by matrix rational. ",
   "M=MakeMatrix{}((1,1), (0,1)); M; M*M; M*M*M; M*M*M*M; 2*M ", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyMatrixRationalOrRationalByMatrixRational");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyMatrixTensorOrRationalByMatrixTensor,
   this->opRational(), this->opMatTensorRat(),
   "Multiplies rational by matrix tensor form. ",
   "M=MakeMatrixTensorForm{}((1,1), (0,1)); M; M*M; M*M*M; M*M*M*M; 2*M ", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyMatrixTensorOrRationalByMatrixTensor");
   this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsWeylGroup::innerTensorAndDecomposeWeylReps,
   this->opWeylGroupVirtualRep(), this->opWeylGroupVirtualRep(),
   "Tensor and decompose two virtual Weyl group representations. ",
   "W= WeylGroupNaturalRep{}(B_3); V=MakeVirtualWeylGroupRepresentation{}W; W\\otimes W; V*V", true,
   true, "CalculatorFunctionsWeylGroup::innerTensorAndDecomposeWeylReps");
  this->AddOperationInnerHandler
  ("*", Calculator::innerMultiplyAtoXtimesAtoYequalsAtoXplusY, "",
   "Collects multiplicand exponents. ",
   "x*(x*y)*x*(x*x^3*x); ", true, false, "Calculator::innerMultiplyAtoXtimesAtoYequalsAtoXplusY");
  this->AddOperationInnerHandler
  ("*", Calculator::innerMultiplyByOne, "",
   "Rule 1*{{anything}}=anything.",
   "x*1;x*1-x ", true, false, "Calculator::innerMultiplyByOne");
  this->AddOperationHandler
  ("*", CalculatorFunctionsGeneral::outerCommuteConstants, "",
   "Rule that commutes constants to the left-most positions.  \
    Provided that a is a constant number (built in) and b is not, replaces b*a by a*b. ",
   "x 6^{1/3}; (x 10^{1/2})^{1/3}", true, true, false,  "CalculatorFunctionsGeneral::outerCommuteConstants");
  this->AddOperationHandler
  ("*", CalculatorFunctionsGeneral::outerMergeConstantRadicals, "",
   "If a and b are constants, replaces a^{c}b^c by (a b)^c.",
   "\\sqrt{}2 \\sqrt{}3", true, true, false, "CalculatorFunctionsGeneral::outerMergeConstantRadicals");


  this->AddOperationOuterHandler
  ("*", Calculator::outerAssociate, "",
   "Associative law: reorders the multiplicative tree in standard form. ",
   "(a*b)*(c*(d*e)*f) - a*b*c*d*e*f;(a*b)*(c*(e*d)*f) - a*b*c*d*e*f", true, false, "Calculator::outerAssociate");
  this->AddOperationOuterHandler
  ("*", CalculatorFunctionsGeneral::outerCommuteAtimesBtimesCifUnivariate, "",
   "Commutative law: replaces a*b by b*a provided that 1) a and and b depend on exactly one user-defined variable, 2) a is not a constant and 3) a>b as an expression. ",
   "x(x+1)^{-1}x; x(y+1)^{-1}x; (\\sin x ) x (\\cos x)", true, false, "CalculatorFunctionsGeneral::outerCommuteAtimesBtimesCifUnivariate");
  this->AddOperationOuterHandler
  ("*", CalculatorFunctionsGeneral::outerCommuteAtimesBifUnivariate, "",
   "Commutative law: replaces a*b by b*a provided that 1) a and and b depend on exactly one user-defined variable, 2) a is not a constant and 3) a>b as an expression. ",
   "x(x+1)^{-1}x; x(y+1)^{-1}x", true, false, "CalculatorFunctionsGeneral::outerCommuteAtimesBifUnivariate");
  this->AddOperationOuterHandler
  ("*", Calculator::outerExtractBaseMultiplication, "",
   "Pulls rationals in the front of multiplicative terms.",
   "2*((3*c)*(4*d)); 3*((a*(d-d))b*c)", true, false, "Calculator::outerExtractBaseMultiplication");
  this->AddOperationOuterHandler
  ("*", Calculator::outerAssociateTimesDivision, "",
   "Associative law w.r.t. division. ",
   "a*(b/c); (a*b)/c-a*(b/c)", true, false, "Calculator::outerAssociateTimesDivision");
  this->AddOperationOuterHandler
  ("*", Calculator::innerCancelMultiplicativeInverse, "",
   "Cancels multiplicative inverse. ",
   "(a*b)/b; (a/b)*b", true, false, "Calculator::innerCancelMultiplicativeInverse");
  this->AddOperationOuterHandler
  ("*", Calculator::outerDistributeTimes, "",
   "Distributive law (left and right).",
   "(a+b)*c; \n a*(b+c)", true, false, "Calculator::outerDistributeTimes");
  this->AddOperationOuterHandler
  ("*", CalculatorFunctionsGeneral::outerDivideReplaceAdivBpowerItimesBpowerJ, "",
   "Rule: (a/x^t)x^s=a x^{s-t}.",
   " (a/x) x^{-1} ;(a/x^2) x^{3};(a/x) x^{3}; (a/x^2) x", true, false, "CalculatorFunctionsGeneral::outerDivideReplaceAdivBpowerItimesBpowerJ");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyAnyByUE, this->opRational(), this->opElementUEoverRF(),
   "Multiplies rational number by an element universal enveloping algebra.",
   "g_{{i}}= getChevalleyGenerator{}(F_1, i); h_{{i}}=getCartanGenerator{}(F_1, i) ; \
   \n[g_{22}+g_{20}+g_{14},g_{17}-6/5g_{14}]", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyAnyByUE");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyAnyByUE, this->opElementUEoverRF(), this->opElementUEoverRF(),
   "Multiplies elment Universal enveloping by element universal enveloping algebra.",
   "g_{{i}}= getChevalleyGenerator{}(F_1, i); h_{{i}}=getCartanGenerator{}(F_1, i) ; \
   \n[g_{22}+g_{20}+g_{14},g_{17}-6/5g_{14}]", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyAnyByUE");

  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyRatOrPolyOrRFByRatOrPolyOrRF, this->opRational(), this->opRationalFunction(),
   "Multiplies rational number by a rational function.",
   "WeylDimFormula{}(a_2, (0,3)) + WeylDimFormula{}(a_2, (3,0)) + 4 WeylDimFormula{}(a_2, (1,1)) ", true,
   false, "CalculatorFunctionsBinaryOps::innerMultiplyRatOrPolyOrRFByRatOrPolyOrRF");

  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyAnyByEltTensor, this->opRational(), this->opElementTensorGVM(),
   "Handles multiplying rational number by an element of tensor product of generalized Verma modules. \
   Not fully tested and documented at the moment.  \
   Will get more documented in the future. ",
   "X=G_2;\ng_{{i}}=getChevalleyGenerator{}(X,i);\nh_{{i}}=getCartanGenerator{}(X, i);  \
   \nv=hwv{}(G_2, (1,0),(0,0));\
   \n2/5 v;\n(3/4 v)\\otimes v;\n3/4 (v\\otimes v);\n(3/4 v)\\otimes v-3/4 (v\\otimes v)", true,
   false, "CalculatorFunctionsBinaryOps::innerMultiplyAnyByEltTensor");

  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyAnyByEltTensor, this->opPoly(), this->opElementTensorGVM(),
   "Handles multiplying polynomial by an element of tensor product of generalized Verma modules. \
   Not fully tested and documented at the moment.  \
   Will get more documented in the future. ",
   "X=G_2;\ng_{{i}}=getChevalleyGenerator{}(X,i);\nh_{{i}}=getCartanGenerator{}(X, i);  \
   \nz=Polynomial{}y;\
   \nv=hwv{}(G_2, (z,1),(1,0));\
   \n(2*z) v;\n", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyAnyByEltTensor");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyAnyByEltTensor, this->opRationalFunction(), this->opElementTensorGVM(),
   "Handles multiplying rational function number by an element of tensor product of generalized Verma modules. \
   Not fully tested and documented at the moment.  \
   Will get more documented in the future. ",
   "X=G_2;\ng_{{i}}=getChevalleyGenerator{}(X,i);\nh_{{i}}=getCartanGenerator{}(X, i);  \
   \nz=Polynomial{}y;\nv=hwv{}(G_2, (z,1),(1,0));\
   \n1/z v", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyAnyByEltTensor");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyAnyByEltTensor, this->opElementUEoverRF(), this->opElementTensorGVM(),
   "Handles acting by element Universal enveloping on an element of tensor product of generalized Verma modules. \
   Not fully tested and documented at the moment.  \
   Will get more documented in the future. ",
   "X=G_2;\ng_{{i}}=getChevalleyGenerator{}(X,i);\nh_{{i}}=getCartanGenerator{}(X, i);  \
   \nz=Polynomial{}y;\nv=hwv{}(G_2, (z,1),(1,0));\
   \n h_1 v; \nh_2 v;\n g_1 g_{-1} v ", true, false, "CalculatorFunctionsBinaryOps::innerMultiplyAnyByEltTensor");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyAnyScalarBySequence, this->opRational(), this->opSequence(),
   "Carries out multiplication between a rational number on left \
   and sequence on the right. Corresponds to multiplying a vector by a scalar \
   (however please note a sequence does not necessarily consist of elements of a ring, so the latter \
    interpretation might not be applicable).",
   "v_{1}=(1, 2, 3);\nv_{2}=(1, 3, 2);\nv_{3}=(3, 1, 1);\nv_{4}=(-2, 2, 2);\n1/2v_{1}+1/2v_{2}+7/8v_{3}+13/16v_{4}"
   , true, false, "CalculatorFunctionsBinaryOps::innerMultiplyAnyScalarBySequence");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyAnyScalarBySequence, this->opDouble(), this->opSequence(),
   "Carries out multiplication between a double number on left \
   and sequence on the right. Corresponds to multiplying a vector by a scalar.",
   "(1 ,2)- DoubleValue{} 1 (2,3)"
   , true, false, "CalculatorFunctionsBinaryOps::innerMultiplyAnyScalarBySequence");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyAnyScalarBySequence, this->opAlgNumber(), this->opSequence(),
   "Multiplies a double number on left and a sequence on the right.scalar.",
   "(1 ,2)- DoubleValue{} 1 (2,3)"
   , true, false, "CalculatorFunctionsBinaryOps::innerMultiplyAnyScalarBySequence");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyAnyScalarBySequence, this->opPoly(), this->opSequence(),
   "Carries out multiplication between a rational number on left \
   and sequence on the right.",
   "x=Polynomial{}x; v=x*(1, 2, 3);"
   , true, false, "CalculatorFunctionsBinaryOps::innerMultiplyAnyScalarBySequence");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyWeylGroupEltByWeightPoly, this->opWeylGroupElement(), this->opWeightLieAlgPoly(),
   "Element of weyl group action on a weight. ",
   "s_{{a}}=MakeElementWeylGroup(B_3, a); \\varepsilon_{{a}}=MakeWeight{}(B_3, a, epsilon);   \
   \nx=Polynomial{}x; \\mu= x\\varepsilon_1; s_1s_2s_3s_2s_1 \\mu"
   , true, false, "CalculatorFunctionsBinaryOps::innerMultiplyWeylGroupEltByWeightPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyRatOrPolyByWeightPoly, this->opPoly(), this->opWeightLieAlgPoly(),
   "Carries out multiplication between a rational or polynomial on left and a weight on the right.",
   "\\varepsilon_{{a}}=MakeWeight{}(B_3, a, epsilon);   x=Polynomial{}x; x\\varepsilon_1"
   , true, false, "CalculatorFunctionsBinaryOps::innerMultiplyRatOrPolyByWeightPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplyRatOrPolyByWeightPoly, this->opRational(), this->opWeightLieAlgPoly(),
   "Carries out multiplication between a rational or polynomial on left and a weight on the right.",
   "\\varepsilon_{{a}}=MakeWeight{}(B_3, a, epsilon);   x=Polynomial{}x; x\\varepsilon_1"
   , true, false, "CalculatorFunctionsBinaryOps::innerMultiplyRatOrPolyByWeightPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("*", CalculatorFunctionsBinaryOps::innerMultiplySequenceMatrixBySequenceMatrix,
   this->opSequence(), this->opSequence(),
   "Multiplies two sequences of sequences in a similar way as if those were matrices.",
   "((1,-1),(0,-1))((1, 0), (-1,-1))", true, false, "CalculatorFunctionsBinaryOps::innerMultiplySequenceMatrixBySequenceMatrix");

  this->AddOperationInnerHandler
  ("\\choose", CalculatorFunctionsBinaryOps::innerNChooseK, "",
   "Evaluates the binomial coefficient if possible.",
   "8 \\choose 3 ", true, false, "CalculatorFunctionsBinaryOps::innerNChooseK");

  this->AddOperationOuterHandler
  ("mod", Calculator::innerZmodP, "",
    "Same as ModP but uses the mod notation.",
    " 7 mod 3", true, false, "Calculator::innerZmodP");

  this->AddOperationBinaryInnerHandlerWithTypes
  ("/", CalculatorFunctionsBinaryOps::innerDivideEltZmodPorRatByEltZmodPorRat, this->opEltZmodP(), this->opEltZmodP(),
   "Divides elements of Z_p. ",
   " (2 mod  7) / (3 mod 7)", true, false, "CalculatorFunctionsBinaryOps::innerDivideEltZmodPorRatByEltZmodPorRat");
  this->AddOperationOuterHandler
  ("/", CalculatorFunctionsGeneral::innerDiffdivDiffxToDifferentiation, "",
   "Replaces \\diff /\\diff {}x by Differentiate{}(x). Note that the variable of differentiation is expected to be the string following the d letter. ",
   "\\diff /\\diff {}x x", true, false, "CalculatorFunctionsGeneral::innerDiffdivDiffxToDifferentiation");
  this->AddOperationOuterHandler
  ("/", CalculatorFunctionsGeneral::innerDdivDxToDiffDivDiffx, "",
   "Replaces d/dx by \\diff /\\diff{}x. Note that the variable of differentiation is expected to be the string following the d letter. ",
   "d/dx x", true, false, "CalculatorFunctionsGeneral::innerDdivDxToDiffDivDiffx");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("/", CalculatorFunctionsBinaryOps::innerDivideAlgebraicNumberOrRatByAlgebraicNumberOrRat, this->opAlgNumber(), this->opAlgNumber(),
   "Divides algebraic numbers. ",
   "1/(1+\\sqrt{}2+\\sqrt{}3+\\sqrt{}5+\\sqrt{}7)", true, false, "CalculatorFunctionsBinaryOps::innerDivideAlgebraicNumberOrRatByAlgebraicNumberOrRat");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("/", CalculatorFunctionsBinaryOps::innerDivideAlgebraicNumberOrRatByAlgebraicNumberOrRat, this->opRational(), this->opAlgNumber(),
   "Divides rational by algebraic number. ",
   "1/(\\sqrt{}2+\\sqrt{}3+\\sqrt{}5)", true, false, "");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("/", CalculatorFunctionsBinaryOps::innerDivideAlgebraicNumberOrRatByAlgebraicNumberOrRat, this->opAlgNumber(), this->opRational(),
   "Divides algebraic number by rational. ",
   "(\\sqrt{}2+\\sqrt{}3+\\sqrt{}5)/5", true, false, "CalculatorFunctionsBinaryOps::innerDivideAlgebraicNumberOrRatByAlgebraicNumberOrRat");
  this->AddOperationOuterHandler
  ("/", CalculatorFunctionsGeneral::outerDivideByNumber, "",
    "If b is rational, algebraic, or double, substitutes (anything)/b with anything* (1/b).",
    "6/15+(a+b)/5; a/\\sqrt{}2; x/DoubleValue{}10^10;x/DoubleValue{}5 ; 6/4+3/0", true, false, "CalculatorFunctionsGeneral::outerDivideByNumber");
  this->AddOperationOuterHandler
  ("/", Calculator::innerSubZeroDivAnythingWithZero, "",
   "Provided that x is not equal to zero, substitutes 0/x with 0. ",
   "0/b; ", true, false, "Calculator::innerSubZeroDivAnythingWithZero");
  this->AddOperationOuterHandler
  ("/", CalculatorFunctionsGeneral::outerDivCancellations, "",
   "Division cancellations. Substitutes (a/b)/(a/d) with d/a and (a/b)/(c/b) with a/c. ",
   " (a/b)/(a/d); (a/b)/(c/b)  ", true, false, "CalculatorFunctionsGeneral::outerDivCancellations");
  this->AddOperationOuterHandler
  ("/", CalculatorFunctionsGeneral::outerAssociateDivisionDivision, "",
   "Substitutes (a/b)/c =a/(c*b); a/(b/c)=a*c/b; .\
   Note the order of multiplication in the rules: this operation is safe and correct for \
   non-commutative rings as well.",
   "(a/b)/c; a/(b/c);", true, false, "CalculatorFunctionsGeneral::outerAssociateDivisionDivision");
  this->AddOperationOuterHandler
  ("/", CalculatorFunctionsGeneral::outerAssociateAdivBdivCpowerD, "",
   "Substitutes a/(b/c)^d=a c^d/b^d;.\
   Note the order of multiplication in the rules: this operation is safe and correct for \
   non-commutative rings as well.",
   "a/(b/c)^d;", true, false, "CalculatorFunctionsGeneral::outerAssociateAdivBdivCpowerD");
  this->AddOperationOuterHandler
  ("/", CalculatorFunctionsGeneral::outerAtimesBpowerJplusEtcDivBpowerI, "",
   "Rule: (a_0 + a_1 x^{c_1}+ ... + a_n x^{c_n}) /x^t=a_0 x^{-t}+ a_1 x^{c_1-t}+...+a_n x^{c_n-t} ",
   " (a x^{1/2} + b x )/x; (a x^{1/2} + b x )/x^2;", true, false, "CalculatorFunctionsGeneral::outerAtimesBpowerJplusEtcDivBpowerI");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("/", CalculatorFunctionsBinaryOps::innerDivideRatByRat, this->opRational(), this->opRational(),
   "Divides two rational numbers. ",
   "4/6; 2/0;", true, false, "CalculatorFunctionsBinaryOps::innerDivideRatByRat");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("/", CalculatorFunctionsBinaryOps::innerDivideDoubleByDouble, this->opRational(), this->opDouble(),
   "Divides doubles. ",
   "a=0.5; b=0.5; c=DoubleValue{}3.3; a/c; c/a; c/c", true, false, "CalculatorFunctionsBinaryOps::innerDivideDoubleByDouble");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("/", CalculatorFunctionsBinaryOps::innerDivideDoubleByDouble, this->opDouble(), this->opDouble(),
   "Divides doubles. ",
   "a=0.5; b=0.5; c=DoubleValue{}3.3; a/c; c/a; c/c", true, false, "CalculatorFunctionsBinaryOps::innerDivideDoubleByDouble");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("/", CalculatorFunctionsBinaryOps::innerDivideRFOrPolyOrRatByRFOrPoly, this->opRational(), this->opPoly(),
   "Divides rational by polynomial (to get a rational function).",
   "z=Polynomial{}(x^2+y^2);\n1/z", true, false, "CalculatorFunctionsBinaryOps::innerDivideRFOrPolyOrRatByRFOrPoly");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("/", CalculatorFunctionsBinaryOps::innerDivideRFOrPolyOrRatByRFOrPoly, this->opPoly(), this->opPoly(),
   "Divides polynomial by polynomial (to get a rational function). ",
   "Polynomial{}(-x_{1}^{2}x_{2}x_{3}-x_{1}^{2}x_{3}+x_{2}+1)/\
   \nPolynomial{}(x_{1}^{2}x_{2}x_{3}-x_{1}^{2}x_{3}-x_{2}+1) ", true, false, "CalculatorFunctionsBinaryOps::innerDivideRFOrPolyOrRatByRFOrPoly");

  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerRatByRatGetAlgebraicNumber, this->opRational(), this->opRational(),
   "Convert rational exponent to the sqrt function. ",
   "10^{1/2}- 10* \\sqrt{1/10}", true, false, "CalculatorFunctionsBinaryOps::innerPowerRatByRatGetAlgebraicNumber");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerRatByRat, this->opRational(), this->opRational(),
   "Raises rational to power, provided the power is a small integer. ",
   "{3^3}^3; 3^{3^3}; 3^3^3; 0^3; 0^{-3}; ", true, false, "CalculatorFunctionsBinaryOps::innerPowerRatByRat");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerRatByRatReducePrimeFactors, this->opRational(), this->opRational(),
   "If a rational number is small enough to factor, reduces the rational exponents of the rational number. ",
   "\n(4/9)^{17/3}; (12/7)^{7/2} ", true, false, "CalculatorFunctionsBinaryOps::innerPowerRatByRatReducePrimeFactors");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerDoubleOrRatToDoubleOrRat, this->opRational(), this->opDouble(),
   "Calls the built-in cpp functions to approximately raise a double to a power,\
   provided either the base or the exponent is a double, and provided that \
   the base is non-negative. ",
   "f{}{{x}}=x^3+p x+q; \
   \nXcardano=( -q/2+ (q^2/4+p^3/27)^(1/2))^(1/3) +( -q/2- (q^2/4+p^3/27)^(1/2))^(1/3);\
   \nq=DoubleValue{}1; \np=DoubleValue{}1; \nXcardano; \nf{}x; \nf{}Xcardano   ",
   true, false, "CalculatorFunctionsBinaryOps::innerPowerDoubleOrRatToDoubleOrRat");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerSequenceByT, this->opSequence(), -1, //-1= any type
   "Provided the exponent is t or T, calls the Transpose function on the base.",
   "X=(1,2)^t; X-Transpose{}(1,2)  ",
   true, false, "CalculatorFunctionsBinaryOps::innerPowerSequenceByT");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerSequenceMatrixByRat, this->opSequence(), this->opRational(), //-1= any type
   "If the base is a matrix and the exponent is a rational number: 1. If the base is not square, returns error. 2. If the base is square and consists of \
   rational numbers and the exponent is a small integer, raises the base to the corresponding power. If the power is 0 or negative and the determinant of\
   the matrix is zero, returns error. ",
   "X=\\begin{array}{cc}0 & 1 \\\\ 1 &1\\end{array}; X^5; X^{-5}  ",
   true, false, "CalculatorFunctionsBinaryOps::innerPowerSequenceMatrixByRat");
  this->AddOperationHandler
  ("^", CalculatorFunctionsGeneral::innerPowerAnyToZero, "",
   "Replaces p^0 by 1 if p is non-zero, and by an error message if p is zero.",
   "A=x^0; x=0; A; B=x^0; 0^0; ",
   true, true, false, "CalculatorFunctionsGeneral::innerPowerAnyToZero");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerPolyBySmallInteger, this->opPoly(), this->opRational(),
   "Raises poly to small integer power. ",
   "x=Polynomial{}x; y=Polynomial{}y;(x+2y+x y+x^2+3y^2)^3",
   true, false, "CalculatorFunctionsBinaryOps::innerPowerPolyBySmallInteger");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerMatBySmallInteger, this->opMatRat(), this->opRational(),
   "Raises matrix rationals to small integer power. ",
   "X=MakeMatrix((0,1),(1,1)); q=100; \nX^q; \nFibonacci{}0=1; \nFibonacci{}1=1; \nFibonacci{}{{n}}:if((n>0)* (IsInteger{}n))=Fibonacci{}(n-1)+Fibonacci{}(n-2); \
   \nMakeMatrix((Fibonacci{}(q-2), Fibonacci{}(q-1)), (Fibonacci{}q-1, Fibonacci{}q))",
   true, false, "CalculatorFunctionsBinaryOps::innerPowerMatBySmallInteger");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerMatBySmallInteger, this->opMatRF(), this->opRational(),
   "Raises a polynomial matrix to small integer power. ",
   "X=MakeMatrixRFs((x,y),(1,2)); X^5",
   true, false, "CalculatorFunctionsBinaryOps::innerPowerMatBySmallInteger");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerMatBySmallInteger, this->opMatAlgebraic(), this->opRational(),
   "Raises algebraic number matrix to small integer power. ",
   "X=MakeMatrix((\\sqrt{}2,1),(\\sqrt{}3,3)); X^100",
   true, false, "CalculatorFunctionsBinaryOps::innerPowerMatBySmallInteger");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerAlgNumPolyBySmallInteger, this->opPolyOverANs(), this->opRational(),
   "Raises poly over algebraic numbers to small integer power. ",
   " x=Polynomial{}x; y=Polynomial{}y;(x+\\sqrt{2}y+x y+x^2+\\sqrt{3}y^2)^3",
   true, false, "CalculatorFunctionsBinaryOps::innerPowerAlgNumPolyBySmallInteger");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerAlgebraicNumberBySmallInteger, this->opAlgNumber(), this->opRational(),
   "Raises algebraic number to small integer power. ",
   "a=3/2; b=-15/2; c=33/4;\
    \nx=(-b+\\sqrt{}(b^2-4a c))/(2a);\
    \nB=c+a x^{2}+b x;", true, false, "CalculatorFunctionsBinaryOps::innerPowerAlgebraicNumberBySmallInteger");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerEWABySmallInteger, this->opElementWeylAlgebra(), this->opRational(),
   "Raises element of weyl algebra to integer power. ",
   "\\partial=ElementWeylAlgebraDO{}(\\partial, x); \nx=ElementWeylAlgebraPoly{}(\\partial, x); \na=x\\partial; \na^10; \\partial x^{3/2}; \\partial^{3/2} x",
   true, false, "CalculatorFunctionsBinaryOps::innerPowerEWABySmallInteger");

  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerElementUEbyRatOrPolyOrRF, this->opElementUEoverRF(), this->opRational(),
   "Raises element of universal enveloping to integer power. \
   If the exponent is non-positive integer but the element of the UE is \
   a single generator with coefficient 1, the exponent will be carried out formally. ",
   "g_{{i}}= getChevalleyGenerator{}(G_2, i); h_{{i}}=getCartanGenerator{}(G_2, i) ;\
    \n (g_1+g_2)^2+ g_1^{1/2}",
   true, false, "CalculatorFunctionsBinaryOps::innerPowerElementUEbyRatOrPolyOrRF");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerElementUEbyRatOrPolyOrRF, this->opElementUEoverRF(), this->opPoly(),
   "Provided that an element of Universal Enveloping algebra is \
   a single generator (raised to arbitrary formal polynomial power) with coefficient 1,\
   raises (formally) the element of the UE to arbitrary polynomial power. ",
   "g_{{i}}= getChevalleyGenerator{}(G_2, i); h_{{i}}=getCartanGenerator{}(G_2, i) ;\
    \n ((((g_1)^{Polynomial{}x})^{Polynomial{}y})+g_2)^2",
   true, false, "CalculatorFunctionsBinaryOps::innerPowerElementUEbyRatOrPolyOrRF");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerElementUEbyRatOrPolyOrRF, this->opElementUEoverRF(), this->opRationalFunction(),
   "Provided that an element of Universal Enveloping algebra is \
   a single generator (raised to arbitrary formal RF power) with coefficient 1,\
   raises (formally) the element of the UE to arbitrary RF power. ",
   "g_{{i}}= getChevalleyGenerator{}(G_2, i); h_{{i}}=getCartanGenerator{}(G_2, i) ;\
    \n ((((g_1)^{Polynomial{}x})^{Polynomial{}y})+g_2)^2",
   true, false, "CalculatorFunctionsBinaryOps::innerPowerElementUEbyRatOrPolyOrRF");
  this->AddOperationBinaryInnerHandlerWithTypes
  ("^", CalculatorFunctionsBinaryOps::innerPowerDoubleOrRatToDoubleOrRat, this->opDouble(), this->opRational(),
   "Calls the built-in cpp functions to approximately raise a double to a power,\
   provided either the base or the exponent is a double. If the base is negative and \
   the exponent is rational with odd denominator, the exponent is evaluated to the corresponding\
   real negative root. ",
   "f{}{{x}}=x^3+p x+q; \
   \nXcardano=( -q/2+ (q^2/4+p^3/27)^(1/2))^(1/3) +( -q/2- (q^2/4+p^3/27)^(1/2))^(1/3);\
   \nq=DoubleValue{}1; \np=DoubleValue{}1; \nXcardano; \nf{}x; \nf{}Xcardano   ",
   true, false, "CalculatorFunctionsBinaryOps::innerPowerDoubleOrRatToDoubleOrRat");
  this->AddOperationOuterHandler
  ("^", Calculator::innerAssociateExponentExponent, "",
   "Substitutes (a^b)^c with a^{b*c}.",
   "(a^m)^n", true, false, "Calculator::innerAssociateExponentExponent");
  this->AddOperationOuterHandler
  ("^", Calculator::innerDistributeExponent, "",
   "If a is a constant, substitutes (a*b)^c with a^c b^c.",
   "(a*b)^n; (\\sqrt(2)*b)^2", true, false, "Calculator::innerDistributeExponent");
  this->AddOperationOuterHandler
  ("^", Calculator::outerPowerRaiseToFirst, "",
   "Realizes the tranformation {{anything}}^1=a. ",
  "x^1+x^2; A^1", true, false, "Calculator::outerPowerRaiseToFirst");

  this->AddOperationBinaryInnerHandlerWithTypes
  ("\\otimes", CalculatorFunctionsBinaryOps::innerTensorEltTensorByEltTensor, this->opElementTensorGVM(), this->opElementTensorGVM(),
   "Tensor product of two generalized Verma modules. \
   Not fully tested and documented at the moment.  \
   Will get more documented in the future. ",
   "X=G_2;\ng_{{i}}=getChevalleyGenerator{}(X,i);\nh_{{i}}=getCartanGenerator{}(X, i);  \
   \nz=Polynomial{}y;\nv=hwv{}(G_2, (z,1),(1,0));\
   \ng_{-1}(v\\otimes v);\
   \ng_{-1}g_{-1}(v\\otimes v)", true);
  this->AddOperationInnerHandler
  ("[]", CalculatorFunctionsBinaryOps::innerLieBracketRatPolyOrEWAWithRatPolyOrEWA, "",
   "Lie bracket of elements of weyl algebras=differential operators with polynomial coefficients. ",
   "\\partial_{{i}}=ElementWeylAlgebraDO{}(\\partial_i, x_i); \nx_{{i}}=Polynomial{}x_i; \n[\\partial_1, x_1]; ", true);
  this->AddOperationInnerHandler
  ("[]", CalculatorFunctionsBinaryOps::innerLieBracketRatOrUEWithRatOrUE, "",
   "Lie bracket of elements of semisimple Lie algebra. ",
   "X=A_1;\ng_{{i}}=getChevalleyGenerator{}(X,i);\nh_{{i}}=getCartanGenerator{}(X,i);\n[g_1,g_{-1}] ", true);

  this->AddOperationBinaryInnerHandlerWithTypes
  ("\\otimes", CalculatorFunctionsWeylGroup::innerTensorWeylReps,
   this->opWeylGroupRep(), this->opWeylGroupRep(),
   "Tensor product of two Weyl group reps. Does not decompose the tensor product. \
   If you want decomposition, use V*V instead. ",
   "V=WeylGroupNaturalRep{}(B_3); V\\otimes V; V*V", true);
  this->AddOperationBinaryInnerHandlerWithTypes
  ("\\otimes", CalculatorFunctionsBinaryOps::innerTensorMatRatByMatRat,
   this->opMatRat(), this->opMatRat(),
   "Tensor product of two matrices.",
    "P=((0 , 2 ),(1 , 0)); Q=((0 , 3 ),(1 , 0)); \
    \nX=MakeMatrix{}P; Y=MatrixRationals{}Q; \
    \nZ=MatrixRationalsTensorForm{}P; W=MatrixRationalsTensorForm{}Q; \
    X\\otimes Y; Z\\otimes W", true);
  this->AddOperationBinaryInnerHandlerWithTypes
  ("\\otimes", CalculatorFunctionsBinaryOps::innerTensorMatByMatTensor,
   this->opMatTensorRat(), this->opMatTensorRat(),
   "Same as tensor product of matrices but uses class MatrixTensor instead of class Matrix.",
    "P=((0 , 2 ),(1 , 0)); Q=((0 , 3 ),(1 , 0)); \
    \nX=MatrixRationals{}P; Y=MatrixRationals{}Q; \
    \nZ=MatrixRationalsTensorForm{}P; W=MatrixRationalsTensorForm{}Q; \
    \nX\\otimes Y; Z\\otimes W", true);
  this->AddOperationOuterHandler
  ("\\otimes", this->outerTensor, "",
   "Please do note use (or use at your own risk): this is work-in-progress. \
   Will be documented when implemented and tested. Tensor product of \
   generalized Verma modules. ",
   "X=G_2;\ng_{{i}}=getChevalleyGenerator{}(X,i);\nh_{{i}}=getCartanGenerator{}(X,i);  \
   \nv=hwv{}(X, (1,0),(0,0));\
   \ng_{-1}(v\\otimes v);\
   \ng_{-1}g_{-1}(v\\otimes v); ", true);
  this->AddOperationOuterHandler
  ("=:", this->StandardIsDenotedBy, "", "The operation =: is the \"is denoted by\" operation. \
   The expression a=:b always reduces to \
   a=b. In addition to the transformation, the pair of expressions a,b is registered in a \
   special global \"registry\". This has the following effect. Every time \
   the expression b is met, it is displayed on the screen as a. We note that subsequent \
   occurrences of the expression a will first be replaced by b (as mandated\
   by the a=b command), but then displayed on the screen as a.", "x=:y;\ny;\nz;\nz=y;\nz ", true);
  std::stringstream StandardPowerStreamInfo, moreInfoOnIntegers;
  moreInfoOnIntegers
  << " LargeIntUnsigned::SquareRootOfCarryOverBound is "
  << " restricted to be smaller than the square root of a positive signed int on the system: in this way, multiplying two small integers and "
  << " storing the result in a signed int is guaranteed to produce the correct result. "
  << " Rationals are stored in the computer as 8 bytes+ one pointer (which is another 4 bytes on a 32 bit system). "
  << " The pointer either equals zero, or points to a dynamically resizable structure able to hold "
  << " arbitrary integer sizes (within the system's address space limits). If the pointer is zero, we call  "
  << " such a rational number small. In this case its denominator and numerator are stored in "
  << " the other 8 bytes, and should both be smaller than LargeIntUnsigned::CarryOverBound="
  << LargeIntUnsigned::CarryOverBound
  << " . When requesting an arithmetic operation, if both rationals are small, (i.e. their pointer zero)"
  << " a check is performed whether the denominator and numerator are smaller in absolute value than "
  << " LargeIntUnsigned::SquareRootOfCarryOverBound="
  << LargeIntUnsigned::SquareRootOfCarryOverBound
  << ". If that check passes, the two rationals are multiplied using the "
  << " built-in processor instructions for operations with integers. If any of the check fails, both rationals are converted to the larger dynamically "
  << " resizeable structure, sufficient memory is allocated, and multiplication/addition is carried using algorithms located in class "
  << " LargeIntUnsigned. The algorithms are O(log(n)^2) (the number of digits squared), "
  << " slower than the ones in the standard numerical computation libraries (such as GMP). However, the algorithms "
  << " provide the essential convenience of having a self-contained mathematical library. An implementation of "
  << " discrete Fourier transform multiplication algorithm (O(log(n)(log(log n))) is considered, and will be made if need arises. ";
  StandardPowerStreamInfo
  << "If the left argument evaluates to atom, and if the right argument is a small integer atom, "
  << " tries to carry out the raise-to-power operation. If successful, substitutes the expression with the obtained atom. "
  << " A small integer is defined at compile time in the variable LargeIntUnsigned::SquareRootOfCarryOverBound (currently equal to "
  << LargeIntUnsigned::SquareRootOfCarryOverBound << "). "
  << CGI::GetHtmlSpanHidableStartsHiddeN(moreInfoOnIntegers.str());

  this->AddOperationOuterHandler
  ("<", this->outerLessThan, "",
   "If both the left hand side and the right hand side are rational, replaces the expression by \
   1 if the left number is less than the right, else replaces the expression by 0.",
   "3<4; 5<4", true);
  this->AddOperationOuterHandler
  (">", this->outerGreaterThan, "",
   "Greater than: has similar action to the less than sign. The following example shows an implementation of commutativity. ",
   "x_{{i}}*x_{{j}}*{{a}}:if i>j=x_j*x_i*a;\n x_{{i}}*x_{{j}}:if i>j=x_j*x_i; (x_2*x_1- x_1*x_3)(x_1x_5+x_5x_4x_2); x_5x_4x_3x_2x_1", true);

  this->AddOperationOuterHandler
  ("==", this->outerEqualEqual, "",
   "If either the left or the right argument contains a bound variable does nothing. \
    Else evaluates to 1 if the left argument equals the right argument.",
   "x==y;\nx==1;\nIsEqualToX{} {{a}}=a==x;\nIsEqualToX{}y;\nIsEqualToX{}x;\
   \nIsEqualToX{}1;\nx=1;\nIsEqualToX{}1", true);
  this->AddOperationOuterHandler
  ("last", CalculatorFunctionsGeneral::innerLastElement, "",
   "Returns the last element of the expression, provided the argument has no bound variables. If the expression has bound variables does nothing.",
   "p{}((), 0 )=1;\
    \np{}({{x}}, {{n}}):if n<0=0;\
    \np{}((), {{n}})=0;\
    \np{}({{x}},{{n}})=p{}(x, n-last x)+p{}(removeLast x, n);\
    \np{}((1,2, 5, 10, 25,100), 100);", true);
  this->AddOperationOuterHandler
  ("removeLast", CalculatorFunctionsGeneral::innerRemoveLastElement, "",
   "Returns a list with the last element removed, provided the argument has no bound variables. If the expression has bound variables does nothing.",
   "X=(a,b,c); Y= (removeLast X)\\cup Sequence{}(last X)-X; ", true);
  this->AddOperationOuterHandler
  ("\\cap", CalculatorFunctionsGeneral::innerIntersection, "",
   "Intersects lists. For the time being, the output order is not specified (will be fixed in the future).",
   "(a,b,c)\\cap (c, d, e);x=(a,b,c)\\cap (c, d, e);\na= 1;\nd=1;(a,b,c)\\cap (c, d, e); x", true);
  this->AddOperationOuterHandler
  ("\\cup", CalculatorFunctionsGeneral::innerUnion, "",
   "If all arguments of \\cup are of type list, substitutes the expression with \
   a list containing \
   the union of all members (with repetition).",
   "x\\cup (MakeSequence{} x \\cup MakeSequence{}x \\cup (a,b,x))", true);
  this->AddOperationOuterHandler
  ("\\sqcup", CalculatorFunctionsGeneral::innerUnionNoRepetition, "",
   "If all arguments of \\sqcup are of type list, substitutes the expression with a list \
   containing \
   the union of all members; all repeating members are discarded.",
   "(x,y,x)\\sqcup(1,x,y,2)", true);
}

void Calculator::initPredefinedOperationsComposite()
{ MacroRegisterFunctionWithName("Calculator::initPredefinedOperationsComposite");
  this->AddOperationComposite
  ("Rational", CalculatorFunctionsGeneral::innerConstantFunction, "",
   "If x is a constant, replaces x{}({{anything}})=x; ",
   "0{}3;2{}y;(\\sqrt{}2){}x;", true, true, false, "CalculatorFunctionsGeneral::innerConstantFunction");
  this->AddOperationComposite
  ("RationalFunction", CalculatorFunctionsGeneral::innerRationalFunctionSubstitution, "",
   "If x is a constant, replaces x{}({{anything}})=x; ",
   "0{}3;2{}y;(\\sqrt{}2){}x;", true, true, false, "CalculatorFunctionsGeneral::innerRationalFunctionSubstitution");
  this->AddOperationComposite
  ("+", CalculatorFunctionsGeneral::innerCompositeArithmeticOperationEvaluatedOnArgument, "",
   "Equivalent to (a+b){}x=(a{}x)+(b{}x) ",
   "(a+b){}x;", true, true, false, "CalculatorFunctionsGeneral::innerCompositeArithmeticOperationEvaluatedOnArgument");
  this->AddOperationComposite
  ("*", CalculatorFunctionsGeneral::innerCompositeArithmeticOperationEvaluatedOnArgument, "",
   "Equivalent to (a*b){}x=(a{}x)*(b{}x) ",
   "(a*b){}x;", true, true, false, "CalculatorFunctionsGeneral::innerCompositeArithmeticOperationEvaluatedOnArgument");
  this->AddOperationComposite
  ("/", CalculatorFunctionsGeneral::innerCompositeArithmeticOperationEvaluatedOnArgument, "",
   "Equivalent to (a/b){}x=(a{}x)/(b{}x) ",
   "(a/b){}x;", true, true, false, "CalculatorFunctionsGeneral::innerCompositeArithmeticOperationEvaluatedOnArgument");
  this->AddOperationComposite
  ("AlgebraicNumber", CalculatorFunctionsGeneral::innerConstantFunction, "",
   "If x is a constant, replaces x{}({{anything}})=x; ",
   "0{}3;2{}y;(\\sqrt{}2){}x;", true, true, false, "CalculatorFunctionsGeneral::innerConstantFunction");
  this->AddOperationComposite
  ("Sequence", CalculatorFunctionsGeneral::innerDereferenceOperator, "",
   "Dereferences a sequence. The syntax is as illustrated by the example. ",
   "X=(a,b,c); X_1; X_2; X_3; X_4; X_j; j=3; X_j; \
    \nDenominations=(1, 5, 10, 25,50, 100,200, 500, 1000, 2000, 5000);\
    \np(0, 0 )=1;\
    \np({{a}},{{x}}):if x<0=0;\
    \np(0,{{x}})=0;\
    \np({{a}},{{x}} )=p(a-1,x)+ p(a, x-Denominations_a);\
    \np(11,100)\
  ", true, true, false, "CalculatorFunctionsGeneral::innerDereferenceOperator");
  this->AddOperationComposite
  (";", CalculatorFunctionsGeneral::innerDereferenceOperator, "",
   "Dereferences a sequence of rules. The syntax is as illustrated by the example. ",
   "A=d/dx( \\sqrt(x+y)-4x^2y^2); (d/dx(y)=0; A)_2;  ", true, true, false, "CalculatorFunctionsGeneral::innerDereferenceOperator");
  this->AddOperationComposite
  ("ElementWeylAlgebra", CalculatorFunctionsGeneral::innerCompositeEWAactOnPoly, "",
   "Differential operation acting on a polynomial. ",
   "x=ElementWeylAlgebraPoly{}(\\partial, x);\\partial=ElementWeylAlgebraDO{}(\\partial, x);\n \\partial{}(x); \\partial^{2}{}(x^3+x^2); x{}(x^2)",
   true, true, false, "CalculatorFunctionsGeneral::innerCompositeEWAactOnPoly");
  this->AddOperationComposite
  ("*", CalculatorFunctionsGeneral::innerCompositeConstTimesAnyActOn, "",
   "Rule (a*f){}x= a*(f{}x), provided a is a constant. ",
   "(2\\sin){}x-2(\\sin x) ", true, true, false, "CalculatorFunctionsGeneral::innerCompositeConstTimesAnyActOn");
  this->AddOperationComposite
  ("^", CalculatorFunctionsGeneral::innerCompositeApowerBevaluatedAtC, "",
   "Provided that n is not equal to -1, use the rule ({{f}}^{{n}}){}{{x}}=(f{}x)^n.",
   "\\tan^2 x; (f^-2) {}x ; (f^-1){}x ",
   true, true, false, "CalculatorFunctionsGeneral::innerCompositeApowerBevaluatedAtC");
  this->AddOperationComposite
  ("Differentiate", CalculatorFunctionsGeneral::innerCompositeDifferentiateLog, "",
   "Differentiates log.",
   "d/dx (\\log x)",
   true, true, false, "CalculatorFunctionsGeneral::innerCompositeDifferentiateLog");

  //this->AddOperationComposite
  //("Differentiate", CalculatorFunctionsGeneral::innerDdivDxToDifferentiation, "",
  // " ",
//  .. "", true);
}

void Calculator::initPredefinedStandardOperationsWithoutHandler()
{ MacroRegisterFunctionWithName("Calculator::initPredefinedStandardOperationsWithoutHandler");
  //additional operations treated like function names but otherwise not parsed as syntactic elements.

  this->AddOperationNoRepetitionAllowed("Freeze");
  this->AddOperationNoRepetitionAllowed("MonomialCollection");
  this->AddOperationNoRepetitionAllowed("MonomialPoly");
  this->AddOperationNoRepetitionAllowed("Melt");
  this->AddOperationNoRepetitionAllowed("Bind");
  this->AddOperationNoRepetitionAllowed("\\log");
  //additional operations with the same status as user-input expressions.
  this->AddOperationNoRepetitionAllowed("\\pi");
  this->AddOperationNoRepetitionAllowed("e");
  this->AddOperationNoRepetitionAllowed("\\arctan");
  this->AddOperationNoRepetitionAllowed("\\diff");
}

void Calculator::initAtomsNotGoodForChainRule()
{ MacroRegisterFunctionWithName("Calculator::initAtomsNotGoodForChainRule");
  this->atomsNotAllowingChainRule.AddOnTopNoRepetitionMustBeNewCrashIfNot("Bind");
}

void Calculator::initAtomsThatAllowCommutingOfArguments()
{ MacroRegisterFunctionWithName("Calculator::initAtomsThatAllowCommutingOfArguments");
  this->atomsThatAllowCommutingOfCompositesStartingWithThem.SetExpectedSize(30);
  this->atomsThatAllowCommutingOfCompositesStartingWithThem.AddOnTopNoRepetitionMustBeNewCrashIfNot("+");
  this->atomsThatAllowCommutingOfCompositesStartingWithThem.AddOnTopNoRepetitionMustBeNewCrashIfNot("*");
  this->atomsThatAllowCommutingOfCompositesStartingWithThem.AddOnTopNoRepetitionMustBeNewCrashIfNot("/");
  this->atomsThatAllowCommutingOfCompositesStartingWithThem.AddOnTopNoRepetitionMustBeNewCrashIfNot("^");
  this->atomsThatAllowCommutingOfCompositesStartingWithThem.AddOnTopNoRepetitionMustBeNewCrashIfNot("\\sin");
  this->atomsThatAllowCommutingOfCompositesStartingWithThem.AddOnTopNoRepetitionMustBeNewCrashIfNot("\\cos");
  this->atomsThatAllowCommutingOfCompositesStartingWithThem.AddOnTopNoRepetitionMustBeNewCrashIfNot("\\log");
}

void Calculator::initArithmeticOperations()
{ MacroRegisterFunctionWithName("Calculator::initArithmeticOperations");
  this->arithmeticOperations.AddOnTop("+");
  this->arithmeticOperations.AddOnTop("-");
  this->arithmeticOperations.AddOnTop("*");
  this->arithmeticOperations.AddOnTop("/");
  this->arithmeticOperations.AddOnTop("^");
}

void Calculator::initOperationsWhoseDomainsAreTheConstants()
{ MacroRegisterFunctionWithName("Calculator::initOperationsWhoseDomainIsTheConstants");
  this->knownFunctionsWithComplexRange.AddOnTopNoRepetitionMustBeNewCrashIfNot("+");
  this->knownFunctionsWithComplexRange.AddOnTopNoRepetitionMustBeNewCrashIfNot("-");
  this->knownFunctionsWithComplexRange.AddOnTopNoRepetitionMustBeNewCrashIfNot("*");
  this->knownFunctionsWithComplexRange.AddOnTopNoRepetitionMustBeNewCrashIfNot("/");
  this->knownFunctionsWithComplexRange.AddOnTopNoRepetitionMustBeNewCrashIfNot("^");
  this->knownFunctionsWithComplexRange.AddOnTopNoRepetitionMustBeNewCrashIfNot("\\sin");
  this->knownFunctionsWithComplexRange.AddOnTopNoRepetitionMustBeNewCrashIfNot("\\cos");
  this->knownFunctionsWithComplexRange.AddOnTopNoRepetitionMustBeNewCrashIfNot("\\tan");
  this->knownFunctionsWithComplexRange.AddOnTopNoRepetitionMustBeNewCrashIfNot("\\cot");
  this->knownFunctionsWithComplexRange.AddOnTopNoRepetitionMustBeNewCrashIfNot("\\sec");
  this->knownFunctionsWithComplexRange.AddOnTopNoRepetitionMustBeNewCrashIfNot("\\csc");
}

void Calculator::initBuiltInAtomsWhosePowersAreInterprettedAsFunctions()
{ MacroRegisterFunctionWithName("Calculator::initBuiltInAtomsWhosePowersAreInterprettedAsFunctions");
  this->atomsWhoseExponentsAreInterprettedAsFunctions.AddOnTopNoRepetitionMustBeNewCrashIfNot("\\sin");
  this->atomsWhoseExponentsAreInterprettedAsFunctions.AddOnTopNoRepetitionMustBeNewCrashIfNot("\\cos");
  this->atomsWhoseExponentsAreInterprettedAsFunctions.AddOnTopNoRepetitionMustBeNewCrashIfNot("\\tan");
  this->atomsWhoseExponentsAreInterprettedAsFunctions.AddOnTopNoRepetitionMustBeNewCrashIfNot("\\cot");
  this->atomsWhoseExponentsAreInterprettedAsFunctions.AddOnTopNoRepetitionMustBeNewCrashIfNot("\\sec");
  this->atomsWhoseExponentsAreInterprettedAsFunctions.AddOnTopNoRepetitionMustBeNewCrashIfNot("\\csc");
}

void Calculator::AddKnownDoubleConstant(const std::string& theConstantName, double theConstantValue)
{ this->atomsNotInterprettedAsFunctions.AddOnTopNoRepetitionMustBeNewCrashIfNot(theConstantName);
  Expression theConstantE;
  theConstantE.MakeAtom(theConstantName, *this);
  this->knownDoubleConstants.AddOnTopNoRepetitionMustBeNewCrashIfNot(theConstantE);
  this->knownDoubleConstantValues.AddOnTop(theConstantValue);
}

void Calculator::initBuiltInAtomsNotInterprettedAsFunctions()
{ MacroRegisterFunctionWithName("Calculator::initBuiltInAtomsNotInterprettedAsFunctions");
  this->atomsNotInterprettedAsFunctions.SetExpectedSize(30);

  this->AddKnownDoubleConstant("\\pi", MathRoutines::Pi());
  this->AddKnownDoubleConstant("e", MathRoutines::E());
}

void Calculator::initAtomsThatFreezeArguments()
{ MacroRegisterFunctionWithName("Calculator::initAtomsThatFreezeArguments");
  this->atomsThatFreezeArguments.SetExpectedSize(this->builtInTypes.size+100);
  this->atomsThatFreezeArguments.AddOnTop(this->builtInTypes);
  this->atomsThatFreezeArguments.AddOnTopNoRepetitionMustBeNewCrashIfNot("ElementWeylAlgebraDO"); //<-needed to facilitate civilized context handling
  this->atomsThatFreezeArguments.AddOnTopNoRepetitionMustBeNewCrashIfNot("ElementWeylAlgebraPoly"); //<-needed to facilitate civilized context handling
  this->atomsThatFreezeArguments.AddOnTopNoRepetitionMustBeNewCrashIfNot("Freeze");
}
