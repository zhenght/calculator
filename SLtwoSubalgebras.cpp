#include "polyhedra.h"


template < > int HashedListBasicObjects<SltwoDecomposition>::PreferredHashSize=1000;
template < > int ListBasicObjects<SltwoDecomposition>::ListBasicObjectsActualSizeIncrement=1000;
template < > int HashedListBasicObjects<ElementSimpleLieAlgebra>::PreferredHashSize=100;
template < > int ListBasicObjects<ElementSimpleLieAlgebra>::ListBasicObjectsActualSizeIncrement=100;

extern ::IndicatorWindowVariables IndicatorWindowGlobalVariables;
void SltwoSubalgebras::Compute(GlobalVariables& theGlobalVariables)
{	this->theWeylGroup.MakeEn(8);
	int theDimension= this->theWeylGroup.KillingFormMatrix.NumRows;
	this->theWeylGroup.ComputeRho(true);
	this->ClearHashes();
  SelectionWithMaxMultiplicity theHselection;
	theHselection.initMe2(theDimension,2);
	int NumCycles= ::MathRoutines::KToTheNth(3,theDimension)-1;
	NumCycles=5;
	root hCandidate;
	roots rootsHavingScalarProd2WithH;
	roots hCommutingRoots;
	roots RootSpacesThatCommuteWithAllRootsInTheSlTwo;
	Rational tempRat;
  this->IndexZeroWeight=this->theWeylGroup.RootsOfBorel.size;
	::SltwoDecomposition BufferDecomposition;
	hCommutingRoots.MakeActualSizeAtLeastExpandOnTop(this->theWeylGroup.RootsOfBorel.size);
	for (int i=0; i<NumCycles;i++)
	{ this->MakeProgressReport(i,NumCycles,theGlobalVariables);
		theHselection.IncrementSubset();
		hCommutingRoots.size=0;
		rootsHavingScalarProd2WithH.size=0;
		RootSpacesThatCommuteWithAllRootsInTheSlTwo.size=0;
		//theHselection.ComputeDebugString();
		hCandidate.MakeZero(theDimension);
		this->MultiplicitiesFixedHweight.initFillInObject
			(this->theWeylGroup.RootSystem.size+1,0);
		this->MultiplicitiesFixedHweight.TheObjects[this->IndexZeroWeight]
			=theDimension;
		for (int j=0;j<theDimension;j++)
			hCandidate.TheObjects[j]=theHselection.Multiplicities.TheObjects[j];
		//hCandidate.ComputeDebugString();
		bool possible=true;
		for (int k=0; k<this->theWeylGroup.RootSystem.size;k++)
		{ //this->theWeylGroup.RootScalarKillingFormMatrixRoot
			root::RootScalarEuclideanRoot
				(hCandidate, this->theWeylGroup.RootSystem.TheObjects[k], tempRat);
			assert(tempRat.DenShort==1);
			if (tempRat.NumShort>theWeylGroup.RootSystem.size)
			{	possible=false;
				break;
			}
			this->MultiplicitiesFixedHweight.TheObjects[this->IndexZeroWeight+tempRat.NumShort]++;
			if (tempRat.NumShort==2)
        rootsHavingScalarProd2WithH.AddObjectOnTop(theWeylGroup.RootSystem.TheObjects[k]);
      if (tempRat.IsEqualToZero())
        hCommutingRoots.AddObjectOnTop(this->theWeylGroup.RootSystem.TheObjects[k]);
		}
		//this->ComputeDebugStringCurrent();
		for (int j=this->MultiplicitiesFixedHweight.size-1; j>=this->IndexZeroWeight;j--)
		{ int topMult=this->MultiplicitiesFixedHweight.TheObjects[j];
			if (topMult<0)
			{ possible=false;
				break;
			}
			if (topMult>0)
			{	if (j!=this->IndexZeroWeight)
					this->MultiplicitiesFixedHweight.TheObjects[this->IndexZeroWeight*2-j]-=topMult;
				for (int k=j-2;k>=this->IndexZeroWeight;k-=2)
				{	this->MultiplicitiesFixedHweight.TheObjects[k]-=topMult;
					if (k!=this->IndexZeroWeight)
						this->MultiplicitiesFixedHweight
							.TheObjects[this->IndexZeroWeight*2-k]-=topMult;
					assert(	this->MultiplicitiesFixedHweight.TheObjects[k]==
									this->MultiplicitiesFixedHweight
										.TheObjects[this->IndexZeroWeight*2-k]);
					if(this->MultiplicitiesFixedHweight.TheObjects[k]<0)
					{ possible=false;
						break;
					}
				}
			}
		}
		//this->ComputeDebugStringCurrent();
		if (possible)
		{ for (int k=0;k<this->theWeylGroup.RootSystem.size;k++)
      { root::RootScalarEuclideanRoot
          (hCandidate,this->theWeylGroup.RootSystem.TheObjects[k],tempRat);
        if (tempRat.IsEqualToZero())
          if (!rootsHavingScalarProd2WithH.ContainsARootConnectedTo
                (this->theWeylGroup.RootSystem.TheObjects[k],this->theWeylGroup))
            RootSpacesThatCommuteWithAllRootsInTheSlTwo.AddObjectOnTop
              (this->theWeylGroup.RootSystem.TheObjects[k]);
      }
      BufferDecomposition.DifferenceTwoHsimpleRootsIsARoot=false;
      for (int k=0;k<rootsHavingScalarProd2WithH.size;k++)
        for(int j=k+1;j<rootsHavingScalarProd2WithH.size;j++)
          if(this->theWeylGroup.IsARoot
              ( rootsHavingScalarProd2WithH.TheObjects[j]-
                rootsHavingScalarProd2WithH.TheObjects[k]))
            BufferDecomposition.DifferenceTwoHsimpleRootsIsARoot=true;
      BufferDecomposition.theModulesHighestWeights.size=0;
			BufferDecomposition.theModulesMultiplicities.size=0;
			for (int j=this->MultiplicitiesFixedHweight.size-1; j>=0;j--)
				if (this->MultiplicitiesFixedHweight.TheObjects[j]>0)
				{ BufferDecomposition.theModulesHighestWeights.AddObjectOnTop(j-this->IndexZeroWeight);
					BufferDecomposition.theModulesMultiplicities.AddObjectOnTop
						(this->MultiplicitiesFixedHweight.TheObjects[j]);
				}
      BufferDecomposition.hCharacteristic.Assign(hCandidate);
			BufferDecomposition.hCommutingRootSpaces.CopyFromBase(hCommutingRoots);
      BufferDecomposition.DiagramM.ComputeDiagramType(hCommutingRoots,this->theWeylGroup);
      BufferDecomposition.CentralizerDiagram.ComputeDiagramType
        (RootSpacesThatCommuteWithAllRootsInTheSlTwo,this->theWeylGroup);
      BufferDecomposition.RootsHavingScalarProduct2WithH.CopyFromBase
        (rootsHavingScalarProd2WithH);
      BufferDecomposition.ComputeDynkinsEpsilon(this->theWeylGroup);
			if (this->AddObjectOnTopNoRepetitionOfObjectHash(BufferDecomposition))
			{
			}
			//this->AddObjectOnTopHash(BufferDecomposition);
			//this->theHelements.AddObjectOnTop(hCandidate);
		}
	}
	this->ComputeDebugString(theGlobalVariables,this->theWeylGroup,false,false);
}

void SltwoSubalgebras::ElementToString
	(	std::string &output, GlobalVariables &theGlobalVariables, WeylGroup &theWeyl,
		bool useLatex, bool UseHtml)
{	std::string tempS; std::stringstream out;
  int Zcounter=0;
  int numGood=0;
	out <<"Number sl(2)s: "<< this->size<<"\n\n";
	//for (int k=0;k<2;k++)
	//{	//bool tempBool=(k==0);
	for (int i=0;i<this->size;i++)
  //  if (!this->TheObjects[i].DifferenceTwoHsimpleRootsIsARoot)//)==tempBool)
    { //if(this->TheObjects[i].DynkinsEpsilon==0)
      numGood++;
      this->TheObjects[i].hCharacteristic.ElementToString(tempS);
      out << "h: " << tempS;
      root& r=this->TheObjects[i].hCharacteristic;
      out <<"$\\begin{array}{ccccccc}"
          << r.TheObjects[7].ElementToString() <<" & "
          << r.TheObjects[6].ElementToString() <<" & "
          << r.TheObjects[5].ElementToString() <<" & "
          << r.TheObjects[4].ElementToString() <<" & "
          << r.TheObjects[3].ElementToString() <<" & "
          << r.TheObjects[2].ElementToString() <<" & "
          << r.TheObjects[0].ElementToString()
          <<"\\\\&&&&"
          << r.TheObjects[1].ElementToString()
          <<"\\end{array}$\n\n";
      out << "\n sl(2) - module decomposition: ";
      int DimensionCentralizer;
      for (int j=0;j<this->TheObjects[i].theModulesHighestWeights.size;j++)
      {	if ( this->TheObjects[i].theModulesMultiplicities.TheObjects[j]!=1)
          out << this->TheObjects[i].theModulesMultiplicities.TheObjects[j];
        out	<<"$V_{" << this->TheObjects[i].theModulesHighestWeights.TheObjects[j]
            <<"}$;   ";
        if (this->TheObjects[i].theModulesHighestWeights.TheObjects[j]==0)
          DimensionCentralizer=
            this->TheObjects[i].theModulesMultiplicities.TheObjects[j];
      }
      int easyCentralizerDim=this->TheObjects[i].CentralizerDiagram.NumRootsGeneratedByDiagram()+
        this->TheObjects[i].CentralizerDiagram.RankTotal();
      int dimM=this->TheObjects[i].DiagramM.NumRootsGeneratedByDiagram()+
        this->TheObjects[i].DiagramM.RankTotal();
      out <<"\n\nm is of type: " << this->TheObjects[i].DiagramM.DebugString<<" Dimension of m: "<< dimM<< "\n\n";
      if (dimM<=DimensionCentralizer+3)
      { out <<"\n\n~ {This subalgebra satisfies your inequality}\\\\~\\\\~\\\\~ ";
        Zcounter++;
      }
      else
        out <<"\n\nDimension of m is smaller than the total dim of  $V_0$";

      if (  DimensionCentralizer==easyCentralizerDim  )
        out <<"\n\n\n   Dynkin Diagram centralizer: "
            << this->TheObjects[i].CentralizerDiagram.DebugString;
      else
        out <<"\n\n\n   Dynkin Diagram generated by root spaces commuting with e: "
            << this->TheObjects[i].CentralizerDiagram.DebugString;
      if (DimensionCentralizer<easyCentralizerDim)
        out <<"aaaaaaaarrrrrrrrrrrrrggggggggggggggggghhhhhhhhh";
      out <<"\n\n\\rule{\\textwidth}{0.4pt}";
    }
	out <<  "\n\n Num subalgebras satisfying your inequality= "<< Zcounter;
	out <<  "\n\nNum good subalgebras: "<< numGood;
	output=out.str();
}

void SltwoSubalgebras::ComputeDebugStringCurrent()
{	std::string tempS; std::stringstream out;
	for (int i=0;i<this->MultiplicitiesFixedHweight.size;i++)
		out << i-this->IndexZeroWeight<<": "<<this->MultiplicitiesFixedHweight.TheObjects[i] <<"\n";
	this->DebugString=out.str();
}

void SltwoSubalgebras::MakeProgressReport
	(int index, int outOf, GlobalVariables &theGlobalVariables)
{ std::stringstream out;
	out <<index <<" out of "<< outOf <<" =3^8-1 computed";
	::IndicatorWindowGlobalVariables.ProgressReportString1=out.str();
	::IndicatorWindowGlobalVariables.String1NeedsRefresh=true;
	if (theGlobalVariables.FeedDataToIndicatorWindowDefault!=0)
		theGlobalVariables.FeedDataToIndicatorWindowDefault(::IndicatorWindowGlobalVariables);
}

void SltwoDecomposition::ComputeDynkinsEpsilon(WeylGroup& theWeyl)
{ this->DynkinsEpsilon =
    this->DiagramM.NumRootsGeneratedByDiagram()+this->DiagramM.RankTotal();
  int r=0;
  for (int i=0;i<this->hCharacteristic.size;i++)
    if (!this->hCharacteristic.TheObjects[i].IsEqualToZero())
      r++;
  this->DynkinsEpsilon+= r;
  for (int i=0;i<theWeyl.RootSystem.size;i++)
  { int NumNonZeroFound=0;
    for (int j=0;j<this->hCharacteristic.size;j++)
    { if (theWeyl.RootSystem.TheObjects[i].TheObjects[j]==1)
        NumNonZeroFound++;
      if(NumNonZeroFound>1)
        break;
    }
    if (NumNonZeroFound==1)
      this->DynkinsEpsilon--;
  }
  this->DynkinsEpsilon=0;
  for (int i=0;i<this->hCharacteristic.size;i++)
    if (this->hCharacteristic.TheObjects[i]==1)
      this->DynkinsEpsilon=-1;
  this->DynkinsEpsilon=0;

}

void ElementSimpleLieAlgebra::init (SimpleLieAlgebra& owner)
{ this->Hcomponent.SetSizeExpandOnTopLight(owner.theWeyl.KillingFormMatrix.NumRows);
  this->coeffsRootSpaces.SetSizeExpandOnTopNoObjectInit(owner.theWeyl.RootSystem.size);
}

void ElementSimpleLieAlgebra::Nullify(SimpleLieAlgebra& owner)
{ this->init(owner);
  this->Hcomponent.MakeZero(owner.theWeyl.KillingFormMatrix.NumRows);
  for(int j=0; j<this->coeffsRootSpaces.size;j++)
    this->coeffsRootSpaces.TheObjects[j].MakeZero();
}

void SimpleLieAlgebra::LieBracket
  ( const ElementSimpleLieAlgebra& g1, const ElementSimpleLieAlgebra& g2,
    ElementSimpleLieAlgebra& output)
{ assert(&output!=&g1 && &output!=&g2);
  output.Nullify(*this);
  root tempRoot, rootIplusRootJ;
  Rational tempRat;
  for (int i=0;i<g1.coeffsRootSpaces.size;i++)
    for (int j=0; j<g2.coeffsRootSpaces.size;j++)
      if (!g1.coeffsRootSpaces.TheObjects[i].IsEqualToZero() &&
          !g2.coeffsRootSpaces.TheObjects[j].IsEqualToZero())
      { root& rootI= this->theWeyl.RootSystem.TheObjects[i];
        root& rootJ= this->theWeyl.RootSystem.TheObjects[j];
        rootIplusRootJ=rootI+rootJ;
        if (rootIplusRootJ.IsEqualToZero())
        { tempRat.Assign(g1.coeffsRootSpaces.TheObjects[i]);
          tempRat.MultiplyBy(g2.coeffsRootSpaces.TheObjects[j]);
          tempRoot.Assign(rootI);
          tempRoot.DivByLargeRational
            ((this->theWeyl.RootScalarKillingFormMatrixRoot(rootI, rootI)/tempRat)/2);
          output.Hcomponent.Add(tempRoot);
        }
        else
        { if (!this->ChevalleyConstants.elements[i][j].IsEqualToZero())
          { int theIndex=this->theWeyl.RootSystem.IndexOfObjectHash(rootIplusRootJ);
            tempRat.Assign(g1.coeffsRootSpaces.TheObjects[i]);
            tempRat.MultiplyBy(g2.coeffsRootSpaces.TheObjects[j]);
            output.coeffsRootSpaces.TheObjects[theIndex].Add
              (tempRat*this->ChevalleyConstants.elements[i][j]);
          }
        }
      }
  ElementSimpleLieAlgebra const* element1;
  ElementSimpleLieAlgebra const* element2;
  Rational order; order.MakeOne();
  element1=&g1; element2=&g2;		
  for (int l=0;l<2;l++)
	{	for (int j=0;j<element2->NonZeroElements.CardinalitySelection;j++)
		{ this->theWeyl.RootScalarKillingFormMatrixRoot
				( this->theWeyl.RootSystem.TheObjects[element2->NonZeroElements.elements[j]],
					element1->Hcomponent,tempRat);
			tempRat.MultiplyBy
				(element2->coeffsRootSpaces.TheObjects[element2->NonZeroElements.elements[j]]);
			tempRat.MultiplyBy(order);
			output.coeffsRootSpaces.TheObjects[element2->NonZeroElements.elements[j]].Add(tempRat);
		}
		order.MakeMOne();
		element1=&g2;
		element2=&g1;
	}
	output.ComputeNonZeroElements();  
}

void ElementSimpleLieAlgebra::ComputeNonZeroElements()
{ this->NonZeroElements.init(this->coeffsRootSpaces.size);
  for (int i=0;i<this->coeffsRootSpaces.size;i++)
    if (!this->coeffsRootSpaces.TheObjects[i].IsEqualToZero())
      this->NonZeroElements.AddSelectionAppendNewIndex(i);
}

bool SimpleLieAlgebra::FindComplementaryNilpotent
  ( root* h, ElementSimpleLieAlgebra& e, ElementSimpleLieAlgebra& output,
    GlobalVariables& theGlobalVariables)
{ assert(e.Hcomponent.IsEqualToZero());
  e.ComputeNonZeroElements();
  root Difference;
  //format of the system
  //Let the roots of the Weyl be r_1, ..., r_k where k=this->theWeyl.RootSystem.size
  //Let the coefficient in front of r_i in f be x_i, let x be the column vector of the x_i's.
  //Let b be the column vector parametrizing [e,f] (<- Lie bracket). We write b as a column vector
  //in the following format: first come the coefficients in front of p_i, where p_i is the i^th
  //vector of this->theWeyl.RootSystem, and then comes the element of the Cartan.
  //Note that the column vector b is ordered differently from the column vector x!!!
  //Then define theSystem to be the matrix A such that  A x=b.
  int theDimension = this->theWeyl.KillingFormMatrix.NumRows;
  MatrixLargeRational theSystem, targetH;
  int NumRows=this->theWeyl.RootSystem.size;
  int FakeConditionIndex=-1;
  if (h!=0)
		NumRows+=theDimension;
	else
	{	NumRows++;
		root& oneRoot=this->theWeyl.RootSystem.TheObjects[e.NonZeroElements.elements[0]];
		root tempRoot;
		for (int i=0;i<theDimension;i++)
		{ tempRoot.MakeZero(theDimension);
			tempRoot.TheObjects[i].MakeOne();
			if (this->theWeyl.RootScalarKillingFormMatrixRoot(tempRoot,oneRoot).IsPositive())
			{	FakeConditionIndex=1;
				break;
			}
		}
  }
  theSystem.init( NumRows,this->theWeyl.RootSystem.size);
  targetH.init(NumRows,1);
  targetH.NullifyAll();
  theSystem.NullifyAll();
  for (int indexF=0; indexF<this->theWeyl.RootSystem.size; indexF++)
    for (int j=0; j<e.NonZeroElements.CardinalitySelection; j++)
    { int indexE = e.NonZeroElements.elements[j];
      root& rootE = this->theWeyl.RootSystem.TheObjects[indexE];
      root PotentialF = this->theWeyl.RootSystem.TheObjects[indexF];
      root relation = rootE+PotentialF;
      if (!relation.IsEqualToZero())
      { int indexRel = this->theWeyl.RootSystem.IndexOfObjectHash(relation);
        if (indexRel!=-1)
          theSystem.elements[indexRel][indexF].Assign
            (	e.coeffsRootSpaces.TheObjects[indexE]*
							this->ChevalleyConstants.elements[indexE][indexF]);
      }else
      {	if (h!=0) 
					for (int k=0; k<theDimension; k++)
						theSystem.elements[this->theWeyl.RootSystem.size+k][indexF].Assign
							(	e.coeffsRootSpaces.TheObjects[indexE]*rootE.TheObjects[k]*2/
								this->theWeyl.RootScalarKillingFormMatrixRoot(rootE,rootE));
				else
					theSystem.elements[this->theWeyl.RootSystem.size+0][indexF].Assign
						(	e.coeffsRootSpaces.TheObjects[indexE]*rootE.TheObjects[FakeConditionIndex]*2/
							this->theWeyl.RootScalarKillingFormMatrixRoot(rootE,rootE));
      }
    }
  if (h!=0)
		for (int i=0;i<this->theWeyl.KillingFormMatrix.NumRows;i++)
			targetH.elements[i+this->theWeyl.RootSystem.size][0].Assign(h->TheObjects[i]);
	else
		targetH.elements[this->theWeyl.RootSystem.size][0].MakeOne();
  targetH.ComputeDebugString();
	theSystem.ComputeDebugString();
  MatrixLargeRational result;
  bool hasSolution=
    theSystem.Solve_Ax_Equals_b_ModifyInputReturnFirstSolutionIfExists(theSystem,targetH,result);
	//result.ComputeDebugString();
  if (hasSolution)
  { output.Nullify(*this);
		output.Hcomponent.MakeZero(theDimension);
    for (int i=0;i<this->theWeyl.RootSystem.size;i++)
      output.coeffsRootSpaces.TheObjects[i].Assign(result.elements[i][0]);
    output.ComputeNonZeroElements();
  }
  return hasSolution;
}

void ElementSimpleLieAlgebra::MultiplyByRational
	(SimpleLieAlgebra& owner, const Rational& theNumber)
{ if (theNumber.IsEqualToZero())
	{	this->Nullify(owner);
		return;
	}
	this->Hcomponent.MultiplyByLargeRational(theNumber);
	for(int i=0;i<this->NonZeroElements.CardinalitySelection;i++)
		this->coeffsRootSpaces.TheObjects[this->NonZeroElements.elements[i]].MultiplyBy(theNumber);			
}

void ElementSimpleLieAlgebra::ElementToString
	(std::string& output, SimpleLieAlgebra& owner, bool useHtml, bool useLatex)
{ std::stringstream out; std::string tempS;
	if (useLatex)
		out <<"$";
  for (int i=0;i<this->NonZeroElements.CardinalitySelection;i++)
  { this->coeffsRootSpaces.TheObjects[this->NonZeroElements.elements[i]]
			.ElementToString(tempS);
    if (tempS=="1")
			tempS="";
		if (tempS=="-1")
			tempS="-";
		if (i!=0)
		{	if (tempS!="")
			{	if (tempS[0]!='-')
					out <<"+";
			} else
				out<<"+";		
		}					
		out <<tempS<<"g^{\\alpha_{" << this->NonZeroElements.elements[i]+1<<"}}";
  }
	for (int i=0;i<this->Hcomponent.size;i++)
		if (!this->Hcomponent.TheObjects[i].IsEqualToZero())
		{ this->Hcomponent.TheObjects[i].ElementToString(tempS);
			if (tempS=="1")
				tempS="";
			if (tempS=="-1")
				tempS="-";
			if (tempS!="")
			{	if (tempS[0]!='-')
					out <<"+";
			} else
				out<<"+";	
			out <<tempS<<"h_{\\alpha_{"<<i+1<<"}}";
		}
	if(useLatex)
		out <<"$";
  output= out.str();
}

void ElementSimpleLieAlgebra::SetCoefficient
  ( root& indexingRoot, Rational& theCoeff, SimpleLieAlgebra& owner )
{ int index= owner.theWeyl.RootSystem.IndexOfObjectHash(indexingRoot);
  if (index!=-1)
    this->coeffsRootSpaces.TheObjects[index].Assign(theCoeff);
  this->ComputeNonZeroElements();
}
void ElementSimpleLieAlgebra::SetCoefficient
  ( root& indexingRoot, int theCoeff, SimpleLieAlgebra& owner )
{ Rational tempRat=theCoeff; this->SetCoefficient(indexingRoot,tempRat, owner);
}


void ::SimpleLieAlgebra::ElementToString
	(std::string &output, bool useHtml, bool useLatex, GlobalVariables& theGlobalVariables)
{ std::stringstream out;
	std::string tempS;
	int numRoots=this->theWeyl.RootSystem.size;
	int theDimension= this->theWeyl.KillingFormMatrix.NumRows;
	roots theBasis;
	theBasis.SetSizeExpandOnTopNoObjectInit(theDimension);
	for (int i=0;i<theDimension;i++)
	{	out <<"$\\alpha_{"<< i+1<<"} := $ ";
		theBasis.TheObjects[i].MakeZero(theDimension);
		theBasis.TheObjects[i].TheObjects[i].MakeOne();
		out <<theBasis.TheObjects[i].ElementToString()<<"\n\n";
	}
	root tempRoot;
	for (int i=theDimension;i<numRoots;i++)
	{	this->theWeyl.RootSystem.TheObjects[i].GetCoordsInBasis
			(theBasis,tempRoot,theGlobalVariables);
		//this->theWeyl.RootSystem.TheObjects[i].ComputeDebugString();
		//tempRoot.ComputeDebugString();
		out <<"$\\alpha_"<<i+1<<" := ";
		for (int j=0;j<theDimension;j++)
		{ tempRoot.TheObjects[j].ElementToString(tempS);
			if (tempS!="0")
			{	if (tempS=="1")
					tempS="";
				if (tempS=="-1")
					tempS="-";
				if (j!=0)
				{	if (tempS!="")
					{	if (tempS[0]!='-')
							out <<"+";
					} else
						out <<"+";
				}
				out<<tempS<< "\\alpha_{" <<j+1<<"}"; 
			}
		}
		out <<"$";
		this->theWeyl.RootSystem.TheObjects[i].ElementToString(tempS);
		out <<"="<<tempS<< "\n\n";
	}
	for (int i=0;i<theDimension;i++)
	{	out <<"$h_{\\alpha_"<< i+1<<"} (g^{\\gamma}) := \\langle\\gamma,\\alpha_{"
				<<i+1<<" } \\rangle g^{\\gamma}$, for any $\\gamma$.\n\n";
	}
	out <<"\n\n";
	if (useLatex)
	{	out <<"\\begin{tabular}{c";
		for(int i=0;i<numRoots;i++)
			out <<"c";
		out <<"}";
		out <<"$[\\bullet,\\bullet]$&";
	}
	for (int i=0;i<numRoots;i++)
	{ out <<"$g^{\\alpha_{"<<i+1<<"}}$";
		if (i!=numRoots-1)
			out<<"&";
	}
	out <<"\\\\\n";
	Rational tempRat;
	for (int i=0;i<this->ChevalleyConstants.NumRows;i++)
	{	out <<"$g^{\\alpha_{"<<i+1<<"}}$&";
		for (int j=0; j<this->ChevalleyConstants.NumCols;j++)
		{	if (this->Computed.elements[i][j])
			{	this->ChevalleyConstants.elements[i][j].ElementToString(tempS);
				if (tempS=="1")
					tempS="";
				if (tempS=="-1")
					tempS="-";
				if (tempS=="0")
					out <<"0,";
				else
				{ tempRoot=
						this->theWeyl.RootSystem.TheObjects[i]+
						this->theWeyl.RootSystem.TheObjects[j];
					int index=this->theWeyl.RootSystem.IndexOfObjectHash(tempRoot);
					out <<"$"<<tempS<<"g^{\\alpha_{" << index+1<<"}}$, ";
				}
			}
			else
			{ root& theRoot= this->theWeyl.RootSystem.TheObjects[i];
				tempRat=2;
				tempRat.DivideBy(this->theWeyl.RootScalarKillingFormMatrixRoot(theRoot,theRoot));
				if (theRoot.IsNegativeOrZero())
					tempRat.Minus();
				tempRat.ElementToString(tempS);
				if (tempS=="1")
					tempS="";
				if (tempS=="-1")
					tempS="-";
				out << "$"<<tempS<< "h_{\\alpha_{" <<i+1<<"}}$";				
			}
			if (useLatex && j!=this->ChevalleyConstants.NumCols-1)
				out<<" & ";
			else
				if(!useLatex)
					out<<"\t";
		}
		if (useLatex)
			out<<"\\\\";
		out <<"\n";
	}
	if (useLatex)
		out <<"\\end{tabular}";
	//this->ChevalleyConstants.ElementToSting(tempS);
	//out <<"\n"<< tempS<<"\n";
	output=out.str();
}