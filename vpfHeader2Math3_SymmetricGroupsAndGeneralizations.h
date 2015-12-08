//The current file is licensed under the license terms found in the main header file "vpf.h".
//For additional information refer to the file "vpf.h".
#ifndef vpfSymmetricGroupsAndGeneralizationsHeader_alreadyIncluded
#define vpfSymmetricGroupsAndGeneralizationsHeader_alreadyIncluded
#include "vpfHeader1General0_General.h"
#include "vpfHeader2Math0_General.h"
#include "vpfHeader2Math8_VectorSpace.h"
#include "vpfHeader2Math4_Graph.h"
#include "vpfHeader2Math3_FiniteGroups.h"


static ProjectInformationInstance ProjectInfoVpfSymmetricGroups(__FILE__, "Header file, symmetric and related groups. Work in progress.");


// dense bases for sparse spaces
// should this carry around the
// List<MonomialCollection<templateMonomial, coefficient> >
// it came from?
template <class templateVector, class templateMonomial, class coefficient>
class SparseSubspaceBasis
{ public:
  bool flagDeallocated;
  int rank;
  List<templateMonomial> involvedMonomials;
  Matrix<coefficient> projectionOperator;

  void SetBasis(const List<templateVector>& basis);
  void DenseVectorInBasis(Vector<coefficient>& out, const templateVector& in);
  bool CheckConsistency() const
  { if (this->flagDeallocated)
      crash << "This is a programming error: use of SparseSubspaceBasis after free. " << crash;
    for (int i=0; i<this->involvedMonomials.size; i++)
    { this->involvedMonomials[i].CheckConsistency();
    }
    this->projectionOperator.CheckConsistency();
    return true;
  }
  SparseSubspaceBasis(){this->flagDeallocated=false;}
  ~SparseSubspaceBasis(){this->flagDeallocated=true;}
  template <typename somestream>
  somestream& IntoStream(somestream& out) const;
  std::string ToString() const;
};

template <class templateVector, class templateMonomial, class coefficient>
void SparseSubspaceBasis<templateVector, templateMonomial, coefficient>::SetBasis(const List<templateVector>& basis)
{ this->CheckConsistency();
  if(basis.size == 0)
    return;
  for(int i=0; i<basis.size; i++)
    for(int j=0; j<basis[i].theMonomials.size; j++)
      this->involvedMonomials.BSInsertDontDup(basis[i].theMonomials[j]);
  Matrix<coefficient> basisMatrix;
  basisMatrix.init(this->involvedMonomials.size, basis.size);
  for(int j=0; j<basis.size; j++)
    for(int i=0; i<involvedMonomials.size; i++)
      basisMatrix.elements[i][j] = basis[j].GetMonomialCoefficient(involvedMonomials[i]);
  Matrix<coefficient> basisMatrixT = basisMatrix;
  basisMatrixT.Transpose();
  Matrix<coefficient> tmp;
  basisMatrix.MultiplyOnTheLeft(basisMatrixT,this->projectionOperator);
  this->projectionOperator.Invert();
  this->projectionOperator *= basisMatrixT;
  this->rank = basis.size;
}

template <class templateVector, class templateMonomial, class coefficient>
void SparseSubspaceBasis<templateVector,templateMonomial,coefficient>::DenseVectorInBasis(Vector<coefficient>& out, const templateVector& in)
{ Vector<coefficient> inDense;
  inDense.SetSize(this->involvedMonomials.size);
  for(int i=0; i<involvedMonomials.size; i++)
    inDense[i] = in.GetMonomialCoefficient(involvedMonomials[i]);
  out = this->projectionOperator * inDense;
}

template <class templateVector, class templateMonomial, class coefficient>
template <typename somestream>
somestream& SparseSubspaceBasis<templateVector,templateMonomial,coefficient>::IntoStream(somestream& out) const
{ out << "Sparse subspace basis object for basis of rank " << this->rank;
  out << " involving monomials " << this->involvedMonomials.ToStringCommaDelimited() << '\n';
  out << " with projection operator\n" << this->projectionOperator.ToStringPlainText() << '\n';
  return out;
}

template <class templateVector, class templateMonomial, class coefficient>
std::string SparseSubspaceBasis<templateVector,templateMonomial,coefficient>::ToString() const
{ std::stringstream out;
  this->IntoStream(out);
  return out.str();
}

template <class templateVector, class templateMonomial, class coefficient>
std::ostream& operator<<(std::ostream& out, const SparseSubspaceBasis<templateVector,templateMonomial,coefficient>& data)
{ return data.IntoStream(out);
}


class Partition;
class Tableau;
class PermutationR2;
class PermutationGroup;

// Partitions are ordered from largest to smallest
class Partition
{ public:
  int n;
  List<int> p;

  int& operator[](int i) const;
  void FromListInt(const List<int> &in, int lastElement = -1);
  static void GetPartitions(List<Partition> &out, int n);
  void FillTableau(Tableau& out, List<int>& stuffing) const;
  void FillTableauOrdered(Tableau& out) const;
  void GetAllStandardTableaux(List<Tableau>& out) const;
  template <typename scalar>
  void SpechtModuleMatricesOfTranspositions1j(List<Matrix<scalar> >& out) const;
  template <typename scalar>
  void SpechtModuleMatricesOfTranspositionsjjplusone(List<Matrix<scalar> >& out) const;
  template <typename scalar>
  void SpechtModuleMatricesOfPermutations(List<Matrix<scalar> >& out, const List<PermutationR2>& perms) const;
  template <typename scalar>
  void SpechtModuleMatrixOfPermutation(List<Matrix<scalar> >& out, const PermutationR2& p) const;
  static void TestAllSpechtModules(int n);
  // int might not be wide enough
  int Fulton61z() const;
  bool operator==(const Partition& right) const;
  bool operator!=(const Partition& right) const;
  bool operator<(const Partition& right) const;
  bool operator>(const Partition& right) const;

  template <typename somestream>
  somestream& IntoStream(somestream& out) const;
  std::string ToString() const;
  friend std::ostream& operator<<(std::ostream& out, const Partition& data)
  { return data.IntoStream(out);
  }
};

// Tableau methods assume the partition ordering from largest to smallest
class Tableau
{ public:
  List<List<int> > t;

  bool IsStandard() const;
  List<List<int> > GetColumns() const;
  void RowStabilizer(PermutationGroup& in) const;
  void ColumnStabilizer(PermutationGroup& in) const;
  template <typename coefficient>
  void YoungSymmetrizerAction(ElementMonomialAlgebra<MonomialTensor<int,MathRoutines::IntUnsignIdentity>,coefficient>&out, const ElementMonomialAlgebra<MonomialTensor<int,MathRoutines::IntUnsignIdentity>,coefficient>& in);
  List<int> TurnIntoList() const;

  template <typename somestream>
  somestream& IntoStream(somestream& out) const;
  std::string ToString() const;
  friend std::ostream& operator<<(std::ostream& out, const Tableau& data)
  { return data.IntoStream(out);
  }
};


// Permutations are expected to be represented as a list of cycles
// with the smallest numbers coming first.  each cycle is expected
// to be at least a transposition
// Permutations are also elements of the direct limit of the symmetric groups
class PermutationR2
{
public:
  List<List<int> > cycles;

  // the name MakeCanonical clashes with MakeFromMul and MakeFromTableau
  // but it seems to be typical of this project
  void MakeCanonical();
  unsigned int HashFunction() const;
  static  unsigned int HashFunction(const PermutationR2& in)
                                      { return in.HashFunction();};
  bool operator==(const PermutationR2& right) const;
  bool IsID() const;
  int BiggestOccurringNumber() const;
  int operator*(int i) const;
  int Sign() const;
  void BuildTransposition(int i, int j);
  void BuildCycle(const List<int>& cycle);
  // Build operations leave the Permutation in a possibly not canonical state
  // you must MakeCanonical() after using them to get virtually anything to work
  // properly.
  void AddTransposition(int i, int j);
  void AddCycle(const List<int>& cycle);
  void MakeFromListOfCycles(const List<List<int> >& cycle);
  // A jumbled up list of integers 0-n
  void MakeFromActionDescription(const List<int>& actionDescription);
  // Cycle structure as a histogram of cycle lengths
  void GetCycleStructure(List<int>& out) const;
  // Cycle structure as the related partition
  void GetCycleStructure(Partition& out, int n_in_Sn = -1) const;
  // Row by row
  void MakeFromTableauRows(const Tableau& in);
  // this type is hard to *=
  // we are returing the smallest n such that this is an element of Sn
  int MakeFromMul(const PermutationR2& left, const PermutationR2& right);
  void MakeID(const PermutationR2& unused);
  // the purpose of this is compat with other element classes that need
  // to see some things before they can be a proper identity element
  void Invert();
  // some day i'm going to move the out to the first parameter everywhere
  // after i find out why it was put last this time
  PermutationR2 operator^(const PermutationR2& right) const;
  static void ConjugationAction(const PermutationR2& conjugateWith, const PermutationR2& conjugateOn, PermutationR2& out);

  // Should this be operator*(), operator()(), or just ActOn()?
  // I vote operator()() for an action, but I chose operator*() for some reason
  // and thanks to the nature of operator overloading, it's too late to change
  PermutationR2 operator*(const PermutationR2& right) const;

  static bool AreConjugate(const PermutationR2& x, const PermutationR2& y);

  template <typename Object>
  void ActOnList(List<Object>& in) const;
//  MonomialTensor<T1, T2> operator*(MonomialTensor<T1,T2>& right) const;
  void ActOnMonomialTensor(MonomialTensor<int,MathRoutines::IntUnsignIdentity>& out, const MonomialTensor<int,MathRoutines::IntUnsignIdentity>& in) const;

  template <typename coefficient>
  void ActOnTensor(ElementMonomialAlgebra<MonomialTensor<int,MathRoutines::IntUnsignIdentity>,coefficient>& out, const ElementMonomialAlgebra<MonomialTensor<int,MathRoutines::IntUnsignIdentity>,coefficient>& in) const;

  void GetWordjjPlus1(List<int>& word) const;

  template <typename coefficient>
  void GetCharacteristicPolyStandardRepresentation(Polynomial<coefficient>& out)
  { Matrix<coefficient> M;
    int n = this->BiggestOccurringNumber()+1;
    M.MakeZeroMatrix(n);
    for(int i=0; i<n; i++)
      M(i,(*this)*i) = 1;
    out.AssignCharPoly(M);
  }

  bool HasDifferentConjugacyInvariantsFrom(PermutationR2& other)
  { List<int> a,b;
    this->GetCycleStructure(a);
    other.GetCycleStructure(b);
    return !(a==b);
  }

  // this is strictly for sorting purposes, of course
  bool operator>(const PermutationR2& other) const { return this->cycles > other.cycles;}

  template <typename somestream>
  somestream& IntoStream(somestream& out) const;
  std::string ToString(FormatExpressions* unused=0) const;
  friend std::ostream& operator<<(std::ostream& out, const PermutationR2& data)
  { return data.IntoStream(out);
  }
};

// a hyperoctahedral group is a semidirect product of a symmetric group and
// a group of bits.  is there a better name for the group of bits than s?
// false and true are 0 and 1 by convention, which is kinda backwards lol
// false is 1, true is -1.
// an unallocated ElementHyperoctahedralGroup does not have a bits field
// having no bits field is always acceptable and it simply means the bits
// are all unset.
// Multiplication is as folows: (p1,s1)*(p2,s2) = (p1*p2,(p2*s1)*s2)
// where the action of p on s comes from conjugation p⁻¹sp in the standard
// representation, fortuitously (0,1,2) conjugated with xyz gives yzx
// an ElementHyperoctahedralGroup is not an element of any particular
// hyperoctahedral group, but rather of the direct limit of the hyperoctahedral
// groups.  It is the responsibility of the caller, of course, to ensure that
// all requested operations are defined.
class ElementHyperoctahedralGroup
{ public:
  PermutationR2 p;
  List<bool> s;

  void MakeFromPermutation(const PermutationR2& in);
  void MakeFromBits(const List<bool>& in);
  void AddTransposition(int i, int j);
  void ToggleBit(int i);
  void MakeFromMul(const ElementHyperoctahedralGroup& left, const ElementHyperoctahedralGroup& right);
  // for compatibility with element classes that need a prototype to be able to
  // turn themselves into the identity element
  void MakeID(const ElementHyperoctahedralGroup& unused);
  bool IsID() const;
  void Invert();

  ElementHyperoctahedralGroup operator*(const ElementHyperoctahedralGroup& right) const;

  int SmallestN() const;
  int CountSetBits() const;
  ElementHyperoctahedralGroup operator^(const ElementHyperoctahedralGroup& right) const;
  static void ConjugationAction(const ElementHyperoctahedralGroup& conjugateWith, const ElementHyperoctahedralGroup& conjugateOn, ElementHyperoctahedralGroup& out);
  bool HasDifferentConjugacyInvariantsFrom(const ElementHyperoctahedralGroup& other) const;
  static bool AreConjugate(const ElementHyperoctahedralGroup& x, const ElementHyperoctahedralGroup& y);
  void GetCharacteristicPolyStandardRepresentation(Polynomial<Rational>& out) const;
  bool operator==(const ElementHyperoctahedralGroup& right) const;
  bool operator>(const ElementHyperoctahedralGroup& right) const;
  unsigned int HashFunction() const;
  static  unsigned int HashFunction(const ElementHyperoctahedralGroup& in)
                                      { return in.HashFunction();}

  template <typename somestream>
  somestream& IntoStream(somestream& out) const;

  std::string ToString(FormatExpressions* unused = 0) const;

  friend std::ostream& operator<<(std::ostream& out, const ElementHyperoctahedralGroup& data)
  { return data.IntoStream(out);
  }
};

template <typename elementSomeGroup>
class ConjugacyClassR2
{ public:
  elementSomeGroup representative;
  int size;
  bool haveRepresentativeWord;
  List<int> representativeWord;
  // the other members are not necessarily meaningful
  int representativeIndex;
  List<int> theElements;


  template <typename somestream>
  somestream& IntoStream(somestream& out) const;
  std::string ToString() const;
  ConjugacyClassR2(){this->haveRepresentativeWord=false;}
};

template <typename object>
class GeneratorPermutationsOfList
{ public:
  List<object> l;
  bool done_iterating;
  bool go_once;

  struct stack_frame
  { int c;
    int loop_i;
    int program_counter;
  };
  List<struct stack_frame> stack;
  int frame_pointer;

  enum pcpositions
  { beginning, loop, firstout, afterloop, end};

  void Initialize(List<object> theL)
  { if(theL.size == 0)
     { done_iterating = true;
       return;
     }
    this->l = theL;
    this->done_iterating = false;
    this->go_once = false;
    this->frame_pointer = 0;
    this->stack.SetSize(this->l.size);
    this->stack[0].c = this->l.size-1;
    this->stack[0].program_counter = 0;
    ++(*this);
  }
  // this function is the only one that is mathematically interesting
  // by the way what's with the signature lol
  GeneratorPermutationsOfList& operator++()
  { //stOutput << "++ called, ";
    while(true)
    { //varsout();
      if(frame_pointer == -1)
      { if(this->l.size == 1)
        { if(!go_once)
            go_once = true;
          else
            done_iterating = true;
        }
        else
          done_iterating = true;
        return *this;
      }
      switch(stack[frame_pointer].program_counter)
      { case pcpositions::beginning:
        if(stack[frame_pointer].c == 0)
        { frame_pointer--;
          return *this;
        }
        stack[frame_pointer].loop_i = 0;
        case pcpositions::loop:
        if(stack[frame_pointer].loop_i == stack[frame_pointer].c)
        { stack[frame_pointer].program_counter = pcpositions::afterloop;
          break;
        }
        stack[frame_pointer].program_counter = pcpositions::firstout;
        frame_pointer++;
        stack[frame_pointer].c = stack[frame_pointer-1].c-1;
        stack[frame_pointer].program_counter = pcpositions::beginning;
        break;
        case firstout:
        if(stack[frame_pointer].c%2!=0)
          l.SwapTwoIndices(l.size-1-stack[frame_pointer].loop_i,l.size-1-stack[frame_pointer].c);
        else
          l.SwapTwoIndices(l.size-1,l.size-1-stack[frame_pointer].c);
        stack[frame_pointer].loop_i++;
        stack[frame_pointer].program_counter = pcpositions::loop;
        break;
        case afterloop:
        stack[frame_pointer].program_counter = pcpositions::end;
        frame_pointer++;
        stack[frame_pointer].c = stack[frame_pointer-1].c-1;
        stack[frame_pointer].program_counter = pcpositions::beginning;
        break;
        case end:
        frame_pointer--;
      }
    }
  }

  void Initialize(int theN)
  { List<int> ll;
    ll.SetSize(theN);
    for(int i=0; i<theN; i++)
      ll[i] = i;
    this->Initialize(ll);
  }

  // get current element, using bizarre cxx syntax because why not
  // apparently you can't return a reference from a const method?
  List<int>& operator*()
  { return l;
  }

  bool DoneIterating() const
  { return done_iterating;
  }


  void varsout() const
  { stOutput << "stack: [";
    for(int i=0; i<stack.size; i++)
    { stOutput << "(";
      if(i==frame_pointer)
        stOutput << "(";

      stOutput << "pc=" << stack[i].program_counter;
      stOutput << ",li=" << stack[i].loop_i;
      stOutput << ",c=" << stack[i].c;

      if(i==frame_pointer)
        stOutput << ")";
      stOutput << ")";
      if(i != stack.size-1)
        stOutput << ", ";
    }
    stOutput << "] l=" << l.ToStringCommaDelimited() << "\n";
  }
};

// PermutationR2::bon was never intended to be configured properly everywhere
// and PermutationR2s returned from here don't have it set
// is it even a good idea for a PermutationR2 to carry it around?
class GeneratorPermutationR2sOnIndices
{ public:
  List<int> replacements;
  GeneratorPermutationsOfList<int> pads;

  void Initialize(const List<int>& l)
  { replacements = l;
    replacements.QuickSortAscending();
    pads.Initialize(l.size);
  }

  GeneratorPermutationR2sOnIndices& operator++()
  { ++pads;
    return *this; // what the f*** does this even mean how the hell
  }

  // this method name is such bullshit lol
  PermutationR2 operator*()
  { List<int> l = *pads; // for this line, it can't be const, since the other method can't return
    PermutationR2 out;    // a const list& or even verify that the list isn't being modified
    out.MakeFromActionDescription(l);
    for(int i=0; i<out.cycles.size; i++)
      for(int j=0; j<out.cycles[i].size; j++)
        out.cycles[i][j] = replacements[out.cycles[i][j]];
    return out;
  }

  void Initialize(int n)
  { List<int> l;
    l.SetSize(n);
    for(int i=0; i<n; i++)
      l[i] = i;
    Initialize(l);
  }

  bool DoneIterating() const
  { return pads.DoneIterating();
  }

  void ResetIteration()
  { this->Initialize(replacements);
  }
};


template <typename TElementGenerator, typename TElement>
class GeneratorProductOfGenerators
{ public:
  List<TElementGenerator > generators;

  enum pcpositions {beginning, loop, midloop, end};
  struct frame
  { int program_counter;
    TElement subprod;
  };
  List<struct frame> stack;
  int frame_pointer;


  void Initialize(List<TElementGenerator> theGenerators)
  { generators = theGenerators;
    stack.SetSize(generators.size);
    frame_pointer = 0;
    stack[frame_pointer].program_counter = pcpositions::beginning;
    ++(*this);
  }

  /* This program is too confusing to write without writing it in python first
  def operator++(frame_pointer):
    permgens[frame_pointer].Initialize(indiceses[frame_pointer])
    for permi in permgens[frame_pointer]:
      if(frame_pointer > 0):
        subprods[frame_pointer] = subprods[frame_pointer-1]*permi
      else:
        subprods[frame_pointer] = permi
      if(frame_pointer == len(permgens)-1):
        yield subprods[frame_pointer]
      else
        self.operator++(frame_pointer+1)
      ++permgens[frame_pointer]
  */

  GeneratorProductOfGenerators& operator++()
  { while(true)
    { if(frame_pointer == -1)
        return  *this; // seriously tho what the f*** does this even mean
      switch(stack[frame_pointer].program_counter)
      { case pcpositions::beginning:
        generators[frame_pointer].ResetIteration();
        case pcpositions::loop:
        if(generators[frame_pointer].DoneIterating())
        { stack[frame_pointer].program_counter = pcpositions::end;
          break;
        }
        { // permi is a block local variable, so no "jump to case label crosses initialization" error lol
        TElement permi = *(generators[frame_pointer]);
        if(frame_pointer == 0)
          stack[frame_pointer].subprod = permi;
        else
          stack[frame_pointer].subprod.MakeFromMul(stack[frame_pointer-1].subprod,permi);
        if(frame_pointer == generators.size-1)
        { stack[frame_pointer].program_counter = pcpositions::midloop;
          return *this;
        } else
        { stack[frame_pointer].program_counter = pcpositions::midloop;
          frame_pointer++;
          stack[frame_pointer].program_counter = pcpositions::beginning;
          break;
        }
        }
        case pcpositions::midloop:
        ++generators[frame_pointer];
        stack[frame_pointer].program_counter = pcpositions::loop;
        break;
        case pcpositions::end:
        frame_pointer--;
      }
    }
  }

  TElement operator*()
  { return stack.LastObject()->subprod;
  }

  bool DoneIterating()
  { if(frame_pointer == -1)
      return true;
    return false;
  }
};

/* Apparently, parametric polymorphism doesn't work well with inheritance and templates.
class GeneratorElementsSnxSnOnIndicesAndIndices: public GeneratorProductOfGenerators<GeneratorPermutationR2sOnIndices, PermutationR2>
{ public:
  void Initialize(List<List<int> > indiceses)
  { List<GeneratorPermutationR2sOnIndices> gens;
    gens.SetSize(indiceses.size);
    for(int i=0; i<indiceses.size; i++)
    { gens[i].Initialize(indiceses[i]);
    }
    this->Initialize(gens);
  }
};
*/

class GeneratorElementsSnxSnOnIndicesAndIndices
{ public:
  List<GeneratorPermutationR2sOnIndices> generators;

  enum pcpositions {beginning, loop, midloop, end};
  struct frame
  { int program_counter;
    PermutationR2 subprod;
  };
  List<struct frame> stack;
  int frame_pointer;


  void Initialize(List<GeneratorPermutationR2sOnIndices> theGenerators)
  { if(theGenerators.size == 0)
    {  frame_pointer = -1;
       return;
    }
    generators = theGenerators;
    stack.SetSize(generators.size);
    frame_pointer = 0;
    stack[frame_pointer].program_counter = pcpositions::beginning;
    ++(*this);
  }

  /* This program is too confusing to write without writing it in python first
  def operator++(frame_pointer):
    permgens[frame_pointer].Initialize(indiceses[frame_pointer])
    for permi in permgens[frame_pointer]:
      if(frame_pointer > 0):
        subprods[frame_pointer] = subprods[frame_pointer-1]*permi
      else:
        subprods[frame_pointer] = permi
      if(frame_pointer == len(permgens)-1):
        yield subprods[frame_pointer]
      else
        self.operator++(frame_pointer+1)
      ++permgens[frame_pointer]
  */

  GeneratorElementsSnxSnOnIndicesAndIndices& operator++()
  { while(true)
    { if(frame_pointer == -1)
        return  *this; // seriously tho what the f*** does this even mean
      switch(stack[frame_pointer].program_counter)
      { case pcpositions::beginning:
        generators[frame_pointer].ResetIteration();
        case pcpositions::loop:
        if(generators[frame_pointer].DoneIterating())
        { stack[frame_pointer].program_counter = pcpositions::end;
          break;
        }
        { // permi is a block local variable, so no "jump to case label crosses initialization" error lol
        PermutationR2 permi = *(generators[frame_pointer]);
        if(frame_pointer == 0)
          stack[frame_pointer].subprod = permi;
        else
          stack[frame_pointer].subprod.MakeFromMul(stack[frame_pointer-1].subprod,permi);
        if(frame_pointer == generators.size-1)
        { stack[frame_pointer].program_counter = pcpositions::midloop;
          return *this;
        } else
        { stack[frame_pointer].program_counter = pcpositions::midloop;
          frame_pointer++;
          stack[frame_pointer].program_counter = pcpositions::beginning;
          break;
        }
        }
        case pcpositions::midloop:
        ++generators[frame_pointer];
        stack[frame_pointer].program_counter = pcpositions::loop;
        break;
        case pcpositions::end:
        frame_pointer--;
      }
    }
  }

  PermutationR2 operator*()
  { return stack.LastObject()->subprod;
  }

  bool DoneIterating()
  { if(frame_pointer == -1)
      return true;
    return false;
  }

  void Initialize(List<List<int> > indiceses)
  { List<GeneratorPermutationR2sOnIndices> gens;
    gens.SetSize(indiceses.size);
    for(int i=0; i<indiceses.size; i++)
    { gens[i].Initialize(indiceses[i]);
    }
    this->Initialize(gens);
  }
};

// sample element class.  Functional, verifiable documentation
// give it a multiplication function, feed it to SimpleFiniteGroup, and...
class ElementFiniteGroup
{ void* data;

  void* (*mul)(void*, void*);
  // the rest is optional
  void* (*inv)(void*) ;

  ElementFiniteGroup()
  { this->data=0;
    this->mul=0;
    this->inv=0;
  }

  ElementFiniteGroup operator*(ElementFiniteGroup right)
  { ElementFiniteGroup out;
    out.data = this->mul(this->data, right.data);
    out.mul = this->mul;
    out.inv = this->inv;
    return out;
  }
  ElementFiniteGroup Invert()
  { ElementFiniteGroup out;
    if(inv != NULL)
    { out.data = this->inv(this->data);
      return out;
    }
  }
  ElementFiniteGroup MakeID(const ElementFiniteGroup& prototype)
  { ElementFiniteGroup out;
    out.mul = prototype.mul;
    out.inv = prototype.inv;
    return out;
  }
};

// this is here until I figure out how to use FiniteGroup
// that, and I guess it might be useful for some Tableau stuff
// ok scratch that, it is a SimpleFiniteGroup class with easier criteria
// for being an element than FiniteGroup
// the reason so few methods are const is that there should ideally be very few SimpleFiniteGroup's and they
// should constantly be updating their state with calculations
/*
template <typename elementSomeGroup>
class FiniteGroup
{ public:
  List<elementSomeGroup> generators;
  HashedList<elementSomeGroup> theElements;
  bool haveElements;
  bool (*AreConjugateByFormula)(const elementSomeGroup& x, const elementSomeGroup& y);
  void (*ComputeCCSizesAndRepresentativesByFormula)(void* G);
  void (*GetWordByFormula)(void* G, const elementSomeGroup& g, List<int>& word);
  int (*GetSizeByFormula)(void* G);
  List<ConjugacyClassR2<elementSomeGroup> > conjugacyClasseS;
  bool flagCCsComputed;
  Matrix<int> generatorCommutationRelations;
  List<List<int> > theWords;
  bool haveWords;

  List<GroupRepresentation<FiniteGroup<elementSomeGroup>, Rational> > irreps;

  FiniteGroup()
  { this->AreConjugateByFormula=0;
    this->ComputeCCSizesAndRepresentativesByFormula=0;
    this->GetWordByFormula=0;
    this->GetSizeByFormula=0;
    this->flagCCsComputed=false;
    this->haveElements=false;
    this->haveWords=false;
  }

  void AddGenDontCompute(const elementSomeGroup& gen) { this->generators.AddOnTop(gen); }
  void ComputeAllElements(bool andWords = false);
  void ComputeCCSizesAndRepresentatives(void* unused = NULL);
  void ComputeElementsAndCCs(void* unused = NULL);
  void GetWord(const elementSomeGroup& g, List<int>& word);
  int GetSize();
  bool AreConjugate(const elementSomeGroup& x, const elementSomeGroup& y);
  bool PossiblyConjugate(const elementSomeGroup& x, const elementSomeGroup& y);
  void MakeID(elementSomeGroup& x);

  // the intention for this being here is to override it in maybe some kind
  // of coset like quotient group class or whatever lol
  bool IsID(const elementSomeGroup& g);
  void ComputeGeneratorCommutationRelations();
  void VerifyCCSizesAndRepresentativesFormula();
  void VerifyWords();
  std::string PrettyPrintGeneratorsCommutationRelations();
  std::string PrettyPrintCharacterTable();
  JSData RepresentationDataIntoJS();

  template <typename coefficient>
  coefficient GetHermitianProduct(const Vector<coefficient>& x1, const Vector<coefficient>& X2);

  template <typename somestream>
  somestream& IntoStream(somestream& out) const;
  std::string ToString() const;
};
*/

class PermutationGroup: public FiniteGroup<PermutationR2>
{ public:
  bool flagIsSymmetricGroup ;
  bool flagHasGenerators1j ;
  bool flagHasGeneratorsjjPlus1 ;

  void MakeSymmetricGroup(int n);
  void MakeSymmetricGroupGeneratorsjjPlus1(int n);

  //bool AreConjugate(const PermutationR2& x, const PermutationR2& y);

  static void ComputeCCSizesAndRepresentativesByFormulaImplementation(void* G);
  static LargeInt GetSizeByFormulaImplementation(void* G);
  static void GetWordjjPlus1Implementation(void* G, const PermutationR2& g, List<int>& word);

  PermutationGroup()
  { this->flagIsSymmetricGroup=false;
    this->flagHasGenerators1j=false;
    this->flagHasGeneratorsjjPlus1=false;
  }
  template <typename coefficient>
  void SpechtModuleOfPartition(const Partition& p, GroupRepresentation<FiniteGroup<PermutationR2>, coefficient>& rep);
  template <typename somestream>
  somestream& IntoStream(somestream& out);
  std::string ToString();
  friend std::ostream& operator<<(std::ostream& out, PermutationGroup& data)
  { return data.IntoStream(out);
  }
};

// the int N field may or may not be meaningful
class HyperoctahedralGroup: public FiniteGroup<ElementHyperoctahedralGroup>
{ public:
  bool isEntireHyperoctahedralGroup;
  bool isEntireDn ;
  int N;

  HyperoctahedralGroup()
  { this->isEntireHyperoctahedralGroup=false;
    this->isEntireDn=false;
    this->N=-1;
  }
  void MakeHyperoctahedralGroup(int n);
  void MakeBn(int n);

  static void ComputeCCSizesAndRepresentativesByFormulaImplementation(void* G);
  static void GetWordByFormulaImplementation(void* G, const ElementHyperoctahedralGroup& element, List<int>& word);
  static LargeInt GetSizeByFormulaImplementation(void* G);


  int GetN();


  void AllSpechtModules();
  void SpechtModuleOfPartititons(const Partition& positive, const Partition& negative,
                               GroupRepresentation<FiniteGroup<ElementHyperoctahedralGroup>, Rational> &out);
  void SomeModuleOfPartititons(const Partition& positive, const Partition& negative,
                               GroupRepresentation<FiniteGroup<ElementHyperoctahedralGroup>, Rational> &out,
                               Subgroup<HyperoctahedralGroup, ElementHyperoctahedralGroup>* subsn);


  template <typename somestream>
  somestream& IntoStream(somestream& out) const;

  std::string ToString() const;
  friend std::ostream& operator<<(std::ostream& out, const HyperoctahedralGroup& data)
  { return data.IntoStream(out);
  }
};

template <typename scalar>
void Partition::SpechtModuleMatricesOfTranspositions1j(List<Matrix<scalar> >& out) const
{ List<PermutationR2> perms;
  perms.SetSize(this->n-1);
  for(int i=0; i<this->n-1; i++)
    perms[i].BuildTransposition(0,i+1);
  this->SpechtModuleMatricesOfPermutations(out, perms);
}

template <typename scalar>
void Partition::SpechtModuleMatricesOfTranspositionsjjplusone(List<Matrix<scalar> >& out) const
{ List<PermutationR2> perms;
  perms.SetSize(this->n-1);
  for(int i=0; i<this->n-1; i++)
    perms[i].BuildTransposition(i,i+1);
  this->SpechtModuleMatricesOfPermutations(out, perms);
}

template <typename scalar>
void Partition::SpechtModuleMatrixOfPermutation(List<Matrix<scalar> >& out, const PermutationR2& p) const
{ List<PermutationR2> perms;
  perms.AddOnTop(p);
  this->SpechtModuleMatricesOfPermutations(out, perms);
}


template <typename scalar>
void Partition::SpechtModuleMatricesOfPermutations(List<Matrix<scalar> >& out, const List<PermutationR2>& perms) const
{ //stOutput << "Debugging Partition::SpechtModuleMatricesOfPermutations: " << *this;
  //stOutput << " with permutations " << perms.ToStringCommaDelimited() << '\n';
  Tableau initialTableau;
  List<int> stuffing;
  stuffing.SetSize(this->n);
  for(int i=0; i<this->n; i++)
    stuffing[i] = i;
  this->FillTableau(initialTableau, stuffing);
  MonomialTensor<int,MathRoutines::IntUnsignIdentity> tm1;
  tm1.generatorsIndices.SetSize(n);
  tm1.Powers.SetSize(n);
  for(int i=0; i<n; i++)
  { tm1.generatorsIndices[i] = i;
    tm1.Powers[i] = 1;
  }
  ElementMonomialAlgebra<MonomialTensor<int,MathRoutines::IntUnsignIdentity>,scalar> t1, t2, t3;
  t1.AddMonomial(tm1,1);
  initialTableau.YoungSymmetrizerAction(t2,t1);
  //stOutput << "Young symmetrizer: " << t1 << " projects to " << t2 << '\n';
  List<ElementMonomialAlgebra<MonomialTensor<int,MathRoutines::IntUnsignIdentity>,scalar> > basisvs;
  SparseSubspaceBasis<ElementMonomialAlgebra<MonomialTensor<int,MathRoutines::IntUnsignIdentity>,scalar>,MonomialTensor<int,MathRoutines::IntUnsignIdentity>,scalar> basis;
  List<Tableau> standardTableaux;
  this->GetAllStandardTableaux(standardTableaux);
  //stOutput << "Standard tableaux are " << standardTableaux.ToStringCommaDelimited() << '\n';
  basisvs.SetSize(standardTableaux.size);
  for(int i=0; i<standardTableaux.size; i++)
  { PermutationR2 p;
    p.MakeFromActionDescription(standardTableaux[i].TurnIntoList());
    p.ActOnTensor(basisvs[i],t2);
    //stOutput << "Tableau " << standardTableaux[i] << " is assigned permutation " << p;
    //stOutput << " permuting Young symmetrized tensor to " << basisvs[i] << '\n';
  }
  basis.SetBasis(basisvs);
  //stOutput << "Basis generated: " << basis << '\n';
  out.SetSize(perms.size);
  for(int permi=0; permi<perms.size; permi++)
  { out[permi].init(basis.rank,basis.rank);
    for(int bi=0; bi<basis.rank; bi++)
    { ElementMonomialAlgebra<MonomialTensor<int,MathRoutines::IntUnsignIdentity>,scalar> sparse;
      perms[permi].ActOnTensor(sparse,basisvs[bi]);
      Vector<scalar> dense;
      basis.DenseVectorInBasis(dense, sparse);
      // AssignColumnFromVector?  oh well.
      for(int j=0; j<basis.rank; j++)
        out[permi].elements[j][bi] = dense[j];
    }
  }
}

template <typename somestream>
somestream& Partition::IntoStream(somestream& out) const
{ out << this->n << ": ";
  for(int i=0; i<this->p.size; i++)
  { out << this->p[i];
    if(i != (this->p.size-1))
      out << " ";
  }
  return out;
}

template <typename coefficient>
void Tableau::YoungSymmetrizerAction(ElementMonomialAlgebra<MonomialTensor<int,MathRoutines::IntUnsignIdentity>,coefficient>&out, const ElementMonomialAlgebra<MonomialTensor<int,MathRoutines::IntUnsignIdentity>,coefficient>& in)
{ //stOutput << "Debugging Tableau::YoungSymmetrizerAction: " << *this << " acts on " << in << '\n';
  GeneratorElementsSnxSnOnIndicesAndIndices rs,cs;
  ElementMonomialAlgebra<MonomialTensor<int,MathRoutines::IntUnsignIdentity>,coefficient> rst;
  for(rs.Initialize(this->t); !rs.DoneIterating(); ++rs)
  { ElementMonomialAlgebra<MonomialTensor<int,MathRoutines::IntUnsignIdentity>,coefficient> tmp;
    (*rs).ActOnTensor(tmp, in);
    rst += tmp;
  }
//  stOutput << "Row stabilized: " << rst << '\n';
  out.MakeZero();
  for(cs.Initialize(this->GetColumns()); !cs.DoneIterating(); ++cs)
  { ElementMonomialAlgebra<MonomialTensor<int,MathRoutines::IntUnsignIdentity>,coefficient> tmp;
    PermutationR2 csi = *cs;
    csi.ActOnTensor(tmp,rst);
    out += tmp * csi.Sign();
  }
//  stOutput << "Young symmetrized: " << out << '\n';
}

template <typename somestream>
somestream& Tableau::IntoStream(somestream& out) const
{ out << "[";
  for(int i=0; i<this->t.size; i++)
  { out << "[";
    for(int j=0; j<this->t[i].size-1; j++)
      out << this->t[i][j] << " ";
    out << this->t[i][this->t[i].size-1];
    out << "]";
    if(i!=this->t.size-1)
      out << ", ";
  }
  out << "]";
  return out;
}

template <typename somestream>
somestream& PermutationR2::IntoStream(somestream& out) const
{ out << "[";
  for(int i=0; i<this->cycles.size; i++)
  { out << "(";
    for(int j=0; j<this->cycles[i].size; j++)
    { out << this->cycles[i][j];
      if(j!=this->cycles[i].size-1)
        out << ", ";
    }
    out << ")";
  }
  out << "]";
  return out;
}

template <typename Object>
void PermutationR2::ActOnList(List<Object>& in) const
{ in.PermuteIndices(this->cycles);
}

template <typename coefficient>
void PermutationR2::ActOnTensor(ElementMonomialAlgebra<MonomialTensor<int,MathRoutines::IntUnsignIdentity>,coefficient>& out,
  const ElementMonomialAlgebra<MonomialTensor<int,MathRoutines::IntUnsignIdentity>,coefficient>& in) const
{ for(int i=0; i<in.theMonomials.size; i++)
  { MonomialTensor<int,MathRoutines::IntUnsignIdentity> tmpout,tmpin;
    tmpin = in.theMonomials[i];
    this->ActOnMonomialTensor(tmpout,tmpin);
    out.AddMonomial(tmpout,in.theCoeffs[i]);
  }
}

template <typename elementSomeGroup>
bool FiniteGroup<elementSomeGroup>::PossiblyConjugate(const elementSomeGroup& x, const elementSomeGroup& y)
{ return true;
}

template <typename elementSomeGroup>
bool FiniteGroup<elementSomeGroup>::AreConjugate(const elementSomeGroup& x, const elementSomeGroup& y)
{ if(this->AreConjugateByFormula)
    return this->AreConjugateByFormula(x,y);
  if(!this->flagCCsComputed)
    this->ComputeCCSizesAndRepresentatives(NULL);
  int xi = this->theElements.GetIndex(x);
  int yi = this->theElements.GetIndex(y);
  for(int i=0; i<this->conjugacyClasseS.size; i++)
    if(this->conjugacyClasseS[i].indicesEltsInOwner.BSContains(xi))
      if(this->conjugacyClasseS[i].indicesEltsInOwner.BSContains(yi))
        return true;
  return false;
}

/*
template <typename elementSomeGroup>
template <typename coefficient>
coefficient FiniteGroup<elementSomeGroup>::GetHermitianProduct(const Vector<coefficient>& X1, const Vector<coefficient>& X2)
{ if(!this->flagCCsComputed)
    this->ComputeCCSizesAndRepresentatives(NULL);
  coefficient acc = 0;
  for(int i=0; i<this->conjugacyClasseS.size; i++)
    acc += MathRoutines::ComplexConjugate(X1[i]) * X2[i] * this->conjugacyClasseS[i].size;
  return  acc / this->GetSize();
}
*/

template <typename elementSomeGroup>
void FiniteGroup<elementSomeGroup>::MakeID(elementSomeGroup& e)
{ if(this->generators.size != 0)
    e.MakeID(this->generators[0]);
}

template <typename elementSomeGroup>
void FiniteGroup<elementSomeGroup>::ComputeAllElementsWordsConjugacyIfObvious(bool andWords)
{ if(this->flagAllElementsAreComputed)
  { if(!(andWords && !flagWordsComputed))
      if(!(!this->flagCCsComputed && AreConjugateByFormula))
        return; // no reason to recompute.  skipping
    stOutput << "Recomputing elements of some group, for God only knows why.  Check frames above " << __FILE__ << ':' << __LINE__ << '\n';
    this->theElements.SetSize(0);
  }
  elementSomeGroup e;
  this->MakeID(e);
  this->theElements.AddOnTop(e);
  if(andWords && !GetWordByFormula)
    this->theWords.SetSize(1);
  List<elementSomeGroup> recentadds;
  recentadds.AddOnTop(e);
  List<List<int> > recentwords;
  if(andWords && !GetWordByFormula)
    recentwords.SetSize(1);
  if(AreConjugateByFormula && (ComputeCCSizesAndRepresentativesByFormula == NULL))
  { this->conjugacyClasseS.SetSize(1);
    this->conjugacyClasseS[0].size = 1;
    this->conjugacyClasseS[0].representative = e;
    if(andWords)
    { if(!GetWordByFormula)
        this->conjugacyClasseS[0].representativeIndex = 0;
      this->conjugacyClasseS[0].flagRepresentativeWordComputed = true;
    }
  }
  while(recentadds.size > 0)
  { // this is a necessary copy.  I wish I could tell cxx it is a move.
    elementSomeGroup r = recentadds.PopLastObject();
    List<int> rword;
    if(andWords && !GetWordByFormula)
      rword = recentwords.PopLastObject();
    for(int i=0; i<this->generators.size; i++)
    for(int i=0; i<this->generators.size; i++)
    { elementSomeGroup p = r*generators[i];
      if(this->theElements.AddOnTopNoRepetition(p))
      { recentadds.AddOnTop(p);
        if(andWords && !GetWordByFormula)
        { recentwords.AddOnTop(rword);
          recentwords[recentwords.size-1].AddOnTop(i);
        }
        if(andWords && !GetWordByFormula)
          this->theWords.AddOnTop(recentwords[recentwords.size-1]);
        if(AreConjugateByFormula && (ComputeCCSizesAndRepresentativesByFormula == NULL))
        { int pindex = this->theElements.GetIndex(p);
          bool alreadyHaveClass = false;
          for(int i=0; i<this->conjugacyClasseS.size; i++)
          { if(this->AreConjugate(this->conjugacyClasseS[i].representative,p))
            { this->conjugacyClasseS[i].size++;
              alreadyHaveClass = true;
              break;
            }
          } // if only cxx had for...else
          if(!alreadyHaveClass)
          { int ncc = this->conjugacyClasseS.size;
            this->conjugacyClasseS.SetSize(ncc+1);
            this->conjugacyClasseS[ncc].representative = p;
            this->conjugacyClasseS[ncc].size = 1;
            if(andWords && !GetWordByFormula)
            { this->conjugacyClasseS[ncc].representativeIndex = pindex;
              this->conjugacyClasseS[ncc].representativeWord = recentwords[recentwords.size-1];
              this->conjugacyClasseS[ncc].flagRepresentativeWordComputed = true;
            }
            if(GetWordByFormula)
            { this->GetWord(this->conjugacyClasseS[ncc].representative, this->conjugacyClasseS[ncc].representativeWord);
              this->conjugacyClasseS[ncc].flagRepresentativeWordComputed = true;
            }
          }
        }
      }
    }
  }
  this->flagAllElementsAreComputed = true;
  if(AreConjugateByFormula && (ComputeCCSizesAndRepresentativesByFormula == NULL))
  { this->flagCCsComputed = true;
    this->flagCCRepresentativesComputed = true;
  }
  if(andWords)
    this->flagWordsComputed = true;
  this->sizePrivate = this->theElements.size;
}

// this needs some better design
// the intention here is to do representation theory.  so we need the sizes,
// representatives, and words.
template <typename elementSomeGroup>
void FiniteGroup<elementSomeGroup>::ComputeCCSizesRepresentativesWords(GlobalVariables* unused)
{ if(this->GetWordByFormula)
    this->flagWordsComputed = true;
  if(this->flagCCsComputed && this->flagWordsComputed)
    return;
  if(this->ComputeCCSizesAndRepresentativesByFormula)
  { this->ComputeCCSizesAndRepresentativesByFormula(this);
    return;
  }
  if(this->AreConjugateByFormula)
  { this->ComputeAllElementsWordsConjugacyIfObvious(true);
    return;
  }
  if(!this->flagWordsComputed)
    this->ComputeAllElementsWordsConjugacyIfObvious(true);
  if(!this->flagAllElementsAreComputed)
    this->ComputeAllElementsWordsConjugacyIfObvious(true);
  GraphOLD conjugacygraph = GraphOLD(this->theElements.size, this->generators.size);
  for(int i=0; i<this->theElements.size; i++)
    for(int j=0; j<this->generators.size; j++)
    { elementSomeGroup x = this->theElements[i] ^ this->generators[j];
      int xi = this->theElements.GetIndex(x);
      conjugacygraph.AddEdge(i,xi);
    }
  List<List<int> > components = conjugacygraph.DestructivelyGetConnectedComponents();
  // boxing and unboxing...
  this->conjugacyClasseS.SetSize(components.size);
  for(int i=0; i<components.size; i++)
  { this->conjugacyClasseS[i].representative = this->theElements[components[i][0]];
    this->conjugacyClasseS[i].size = components[i].size;
    this->conjugacyClasseS[i].indicesEltsInOwner = components[i];
    this->conjugacyClasseS[i].representativeIndex = components[i][0];
    if(this->flagWordsComputed)
      GetWord(conjugacyClasseS[i].representative, conjugacyClasseS[i].representativeWord);
  }
  this->flagCCsComputed = true;
  this->flagCCRepresentativesComputed = true;
}

/*
template <typename elementSomeGroup>
void FiniteGroup<elementSomeGroup>::ComputeElementsAndCCs(void* unused)
{ this->ComputeCCSizesAndRepresentatives();
}
*/

template <typename elementSomeGroup>
void FiniteGroup<elementSomeGroup>::GetWord(const elementSomeGroup& g, List<int>& word)
{ if(GetWordByFormula)
    return this->GetWordByFormula(this,g,word);
  if(!this->flagWordsComputed)
    this->ComputeAllElementsWordsConjugacyIfObvious(true);
  word = this->theWords[this->theElements.GetIndex(g)];
}

template <typename elementSomeGroup>
bool FiniteGroup<elementSomeGroup>::IsID(elementSomeGroup& g)
{ return g.IsID();
}

/*// If it's ever needed, this might be the way to build a SimpleFiniteGroup<Matrix<coefficient> >
template <typename coefficient>
bool SimpleFiniteGroup<Matrix<coefficient> >::IsID(const Matrix<coefficient>& g)
{ return g.IsIdMatrix();
}
*/

template <typename elementSomeGroup>
void FiniteGroup<elementSomeGroup>::ComputeGeneratorCommutationRelations()
{ if(this->generatorCommutationRelations.NumRows == this->generators.size)
    return;
  this->generatorCommutationRelations.init(this->generators.size, this-> generators.size);
  for(int i=0; i<this->generators.size; i++)
    for(int j=i; j<this->generators.size; j++)
    { elementSomeGroup g;
      if(i==j)
        g = this->generators[i];
      else
        g = this->generators[i] * this->generators[j];
      elementSomeGroup gi = g;
      int cr = 1;
      while(!this->IsID(gi))
      { gi = gi * g;
        cr++;
      }
      this->generatorCommutationRelations(i,j) = cr;
      this->generatorCommutationRelations(j,i) = cr;
    }
}

template <typename elementSomeGroup>
std::string FiniteGroup<elementSomeGroup>::PrettyPrintGeneratorCommutationRelations()
{ this->ComputeGeneratorCommutationRelations();
  std::string crs = this->generatorCommutationRelations.ToStringPlainText();
  List<char*> rows;
  int i;
  rows.AddOnTop(&crs[0]);
  while((i = crs.find('\n')) != -1)
  { crs[i] = 0;
    rows.AddOnTop(&crs[i+1]);
  }
  bool generatorStringsTooLarge = false;
  bool generatorStringsHaveNewline = false;
  List<std::string> genstrings;
  genstrings.SetSize(this->generators.size);
  for(int i=0; i<this->generators.size; i++)
  { std::stringstream geni;
    geni << this->generators[i];
    genstrings[i] = geni.str();
    if(genstrings[i].length() > 50)
      generatorStringsTooLarge = true;
    // does cxx not have a 'get index of, or, if not found, return -1' method
    // in std::string?
    //if(genstrings[i].find('\n') != -1)
    //{ generatorStringsHaveNewline = true;
    //  generatorStringsTooLarge = true;
    //}
  }
  if(!generatorStringsTooLarge)
  { std::stringstream out;
    for(int i=0; i<this->generators.size; i++)
      out << i << " " << genstrings[i] << '\n';
  }
  std::stringstream out;
  for(int i=0; i<this->generators.size; i++)
    // nota bene: the left shift operator binds tighter than ?: operator, even
    // when it is used as some weird string processing doohickey.  This is also
    // why it is dangerous to use the bitwise xor operator for
    // arithmetic exponentiation
    // furthermore, did you know that operands to ?: are not permitted to have
    // different types, even when they are about to be fed to an operator that
    // can take both types?
    out << i << (generatorStringsHaveNewline?"\n":" ") << genstrings[i] << '\n';
  for(int i=0; i<this->generators.size; i++)
    out << i << " " << rows[i] << '\n';
  return out.str().c_str();
}

template <typename elementSomeGroup>
std::string FiniteGroup<elementSomeGroup>::PrettyPrintCharacterTable()
{ std::stringstream out;
  out << *this;
  out << this->GetSize() << "elements.  Representatives and sizes are ";
  for(int i=0; i<this->conjugacyClasseS.size; i++)
  { out << this->conjugacyClasseS[i].representative << " " << this->conjugacyClasseS[i].size;
    if(i != this->conjugacyClasseS.size-1)
      out << ", ";
    else
      out << "\n";
  }

  // pad the numbers out front
  List<std::string> numbers;
  int numpad = 0;
  for(int i=0; i<this->irreps.size; i++)
  { std::stringstream ns;
    ns << i;
    numbers.AddOnTop(ns.str());
    int nil = numbers.LastObject()->length();
    if(numpad < nil);
      numpad = nil;
  }

  // pad the character values
  List<List<std::string> > values;
  values.SetSize(this->irreps.size);
  int cols_per_elt = 0;
  for(int i=0; i<this->irreps.size; i++)
  { values[i].SetSize(this->irreps[i].theCharacteR.data.size);
    for(int j=0; j<this->irreps[i].theCharacteR.data.size; j++)
    { values[i][j] = irreps[i].theCharacteR.data[j].ToString();
      int vijcols = values[i][j].length();
      if(vijcols > cols_per_elt)
        cols_per_elt = vijcols;
    }
  }
  cols_per_elt++;

  // ok print it all up
  for(int i=0; i<values.size; i++)
  { int padn = numpad - numbers[i].length();
    for(int pp = 0; pp < padn; pp++)
      out << ' ';
    out << numbers[i];
    out << ' ';
    out << '[';
    for(int j=0; j<values[i].size; j++)
    { int padl = cols_per_elt - values[i][j].length();
      for(int pp=0; pp<padl; pp++)
        out << ' ';
      out << values[i][j];
    }
    Rational x = this->irreps[i].theCharacteR.Norm();
    if(x != 1)
      out << "][" << x;
    out << "] " << this->irreps[i].identifyingString;
    out << '\n';
  }

  Rational x = 0;
  for(int i=0; i<this->irreps.size; i++)
    x += irreps[i].theCharacteR.data[0] * irreps[i].theCharacteR.data[0];
  out << "Sum of squares of first column: " << x << '\n';
  // print information about if anything's wrong
  for(int i=0; i<this->irreps.size; i++)
    for(int j=i+1; j<this->irreps.size; j++)
    { Rational x = this->irreps[i].theCharacteR.InnerProduct(this->irreps[j].theCharacteR);
      if(x != 0)
        out << "characters " << i << ", " << j << " have inner product " << x << '\n';
    }
  return out.str();
}

template <typename elementSomeGroup>
JSData FiniteGroup<elementSomeGroup>::RepresentationDataIntoJS()
{ JSData out;
  for(int i=0; i<irreps.size; i++)
    out[i] = irreps[i].JSOut();
  return out;
}

template <typename elementSomeGroup>
void FiniteGroup<elementSomeGroup>::VerifyCCSizesAndRepresentativesFormula()
{ this->ComputeCCSizesAndRepresentatives(0);
  FiniteGroup<elementSomeGroup> GG;
  GG.generators = this->generators;
  //GG.AreConjugateByFormula = this->AreConjugateByFormula;
  GG.ComputeCCSizesAndRepresentatives(0);
  stOutput << "Conjugacy class sizes by formula: ";
  for(int i=0; i<this->conjugacyClasseS.size; i++)
    stOutput << this->conjugacyClasseS[i].size << ", ";
  stOutput << '\n';
  for(int i=0; i<this->conjugacyClasseS.size; i++)
  { stOutput << this->conjugacyClasseS[i].representative;
    stOutput << '\n';
  }
  stOutput << "Conjugacy class sizes by brute force: ";
  for(int i=0; i<GG.conjugacyClasseS.size; i++)
    stOutput << GG.conjugacyClasseS[i].size << ", ";
  stOutput << '\n';
  for(int i=0; i<GG.conjugacyClasseS.size; i++)
  { stOutput << GG.conjugacyClasseS[i].representative << "\n";
  }
  stOutput << '\n';
  if(GG.conjugacyClasseS.size != this->conjugacyClasseS.size)
    stOutput << "Error: wrong number of conjugacy classes, " << this->conjugacyClasseS.size << " should be " << GG.conjugacyClasseS.size << '\n';
  List<int> classes_found;
  for(int i=0; i<this->conjugacyClasseS.size; i++)
  { int gcc;
    if(GG.AreConjugateByFormula)
    { for(int gci=0; gci<GG.conjugacyClasseS.size; gci++)
        if(GG.AreConjugateByFormula(GG.conjugacyClasseS[gci].representative, this->conjugacyClasseS[i].representative))
        { gcc = gci;
          break;
        }
    } else
    { int cri = GG.theElements.GetIndex(this->conjugacyClasseS[i].representative);
      for(int gci=0; gci<GG.conjugacyClasseS.size; gci++)
        if(GG.conjugacyClasseS[gci].indicesEltsInOwner.BSContains(cri))
        { gcc = gci;
          break;
        }
    }
    stOutput << "class " << i << " representative " << this->conjugacyClasseS[i].representative << " belongs to cc " << gcc << " with representative " << GG.conjugacyClasseS[gcc].representative << '\n';
    if(classes_found.BSInsertDontDup(gcc) == -1)
      stOutput << "error\n";
  }
  stOutput << "classes found are " << classes_found.ToStringCommaDelimited() << '\n';
}

template <typename elementSomeGroup>
void FiniteGroup<elementSomeGroup>::VerifyWords()
{ if(!this->flagAllElementsAreComputed)
    this->ComputeAllElementsWordsConjugacyIfObvious(true);
  for(int i=0; i<this->theElements.size; i++)
  { List<int> word;
    GetWord(this->theElements[i], word);
    elementSomeGroup g;
    this->MakeID(g);
    for(int j=0; j<word.size; j++)
      g = g*this->generators[word[j]];
    if(!(g == this->theElements[i]))
      stOutput << this->theElements[i] << " has word " << word.ToStringCommaDelimited() << " which corresponds to " << g << "\n";
  }
}

/*
template <typename elementSomeGroup>
template <typename somestream>
somestream& FiniteGroup<elementSomeGroup>::IntoStream(somestream& out) const
{ out << "Finite Group with " << this->generators.size << " generators";
  if(this->haveElements)
    out << ", " << this->theElements.size << " elements";
  if(this->flagCCsComputed)
    out << ", " << this->conjugacyClasseS.size << " conjugacy classes";
  out <<", generated by: ";
  for(int i=0; i<this->generators.size; i++)
  { out << this->generators[i];
    if(i != this->generators.size-1)
      out << ", ";
  }
  return out;
}
*/

template <typename somestream>
somestream& PermutationGroup::IntoStream(somestream& out)
{ if(!this->flagIsSymmetricGroup)
  { out << "Permutation Group with " << this->theElements.size << " elements, generated by: ";
    for(int i=0; i<this->generators.size; i++)
    { out << this->generators[i];
      if(i != this->generators.size-1)
        out << ", ";
    }
  }
  out << "Symmetric Group on " << this->generators.size + 1 << " letters (";
  if(this->flagHasGenerators1j)
    out << "generators are (1 j))";
  if(this->flagHasGeneratorsjjPlus1)
    out << "generators are (j j+1))";
  out << " thus having " << this->GetSize() << " elements.";
  return out;
}

/*
template <typename elementSomeGroup>
std::string FiniteGroup<elementSomeGroup>::ToString() const
{ std::stringstream out;
  this->IntoStream(out);
  return out.str();
}
*/

template <typename coefficient>
void PermutationGroup::SpechtModuleOfPartition
(const Partition& p, GroupRepresentation<FiniteGroup<PermutationR2>, coefficient>& rep)
{ p.SpechtModuleMatricesOfPermutations(rep.generatorS, this->generators);
  rep.ownerGroup = this;
}

/*
template <typename elementSomeGroup>
std::ostream& operator<<(std::ostream& out, const FiniteGroup<elementSomeGroup>& data)
{ return data.IntoStream(out);
}
*/

template <typename somestream>
somestream& ElementHyperoctahedralGroup::IntoStream(somestream& out) const
{ out << '[' << this->p << ',';
  for(int i=0; i<this->s.size; i++)
    if(this->s[i])
      out << '1';
    else
      out << '0';
  out << ']';
  return out;
}

template <typename somestream>
somestream& HyperoctahedralGroup::IntoStream(somestream& out) const
{ if(this->isEntireHyperoctahedralGroup)
  { out << "Hyperoctahedral Group of rank " << this->generators[0].s.size;
  } else if(this->isEntireDn)
  { out << "Dn group of rank" << this->generators[0].s.size;
  } else
  { out << "Finite group of hyperoctahedral elements, generated by ";
    for(int i=0; i<this->generators.size; i++)
      out << this->generators[i];
  }
  return out;
}

template <typename elementSomeGroup>
template <typename somestream>
somestream& ConjugacyClassR2<elementSomeGroup>::IntoStream(somestream& out) const
{ out << "[" << this->representative << ", " << this->theElements.size << "]";
  return out;
}

template <typename elementSomeGroup>
std::string ConjugacyClassR2<elementSomeGroup>::ToString() const
{ std::stringstream out;
  this->IntoStream(out);
  return out.str();
}

template <typename elementSomeGroup>
std::ostream& operator<<(std::ostream& out, const ConjugacyClassR2<elementSomeGroup>& data)
{ return data.IntoStream(out);
}

template <typename someGroup, typename coefficient>
bool GroupRepresentation<someGroup, coefficient>::VerifyRepresentation()
{ bool badrep = false;
  if(this->generatorS.size != this->ownerGroup->generatorCommutationRelations.NumRows)
    this->ownerGroup->ComputeGeneratorCommutationRelations();
    for(int i=0; i<this->generatorS.size; i++)
      for(int j=i; j<this->generatorS.size; j++)
      { Matrix<Rational> M1 = this->generatorS[i] * this->generatorS[j];
        Matrix<Rational> Mi = M1;
        for(int n=1; n<this->ownerGroup->generatorCommutationRelations(i,j); n++)
          Mi *= M1;
        if(!Mi.IsIdMatrix())
        { stOutput << "generators " << i << ", " << j << " i.e. elements ";
          stOutput << this->ownerGroup->generators[i] << ", " << this->ownerGroup->generators[j];
          stOutput << " are assigned matrices which fail to have commutation relations ";
          stOutput << this->ownerGroup->generatorCommutationRelations(i,j) << "\n";
          stOutput << this->generatorS[i].ToStringPlainText() << ",\n";
          stOutput << this->generatorS[j].ToStringPlainText() << "\n\n";
          badrep = true;
        }
      }
  if(badrep)
  { FiniteGroup<Matrix<Rational> > RG;
    RG.generators = this->generatorS;
    LargeInt GS = this->ownerGroup->GetSize();
    LargeInt RGS = RG.GetSize();
    if((GS % RGS) != 0)
      crash << "Violation of Lagrange's theorem (" << RGS << "∤" << GS << " " << __FILE__ << ":" << __LINE__ << crash;
    //crash << "Not a representation, crashing as soon as possible" << crash;
  }
  if(!badrep)
    stOutput << "VerifyRepresentation: this has the proper commutation relations\n";
  return !badrep;
}


#endif
