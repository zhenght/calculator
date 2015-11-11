#include "vpfHeader2Math3_SymmetricGroupsAndGeneralizations.h"
#include "vpfImplementationHeader2Math3_FiniteGroups.h"

ProjectInformationInstance ProjectInfoVpf2Math3_SymmetricGroupsAndGeneralizations(__FILE__, "Symmetric groups and generalizations, work in progress. ");

// conjugacy classes of type P are of size |Sn|/P.Fulton61z()
int Partition::Fulton61z() const
{ int acc = 1;
  int samecount = 1;
  for(int j=1; j<=this->p.size; j++)
  { // what kind of logic is this?  Make it a do...while loop.
    if((j!=this->p.size) && (this->p[j-1] == this->p[j]))
    { samecount++;
    } else
    { acc *= MathRoutines::KToTheNth(this->p[j-1], samecount);
      acc *= MathRoutines::Factorial(samecount);
      samecount = 1;
    }
  }
  return acc;
/*
int acc = 1;
  List<int> nums;
  List<int> counts;
  for(int i=0; i<this->p.size; i++)
  { int numdex = nums.GetIndex(this->p[i]);
    if(numdex == -1)
    { nums.AddOnTop(this->p[i]);
      counts.AddOnTop(1);
    } else
    { counts[numdex] += 1;
    }
  }
  for(int i=0; i<nums.size; i++)
  { int acci = 1;
    for(int j=0; j<counts[i]; j++)
      acci *= nums[i];
    for(int j=1; j<counts[i]; j++)
      acci *= j;
    acc *= acci;
  }
  return acc;*/
}

int& Partition::operator[](int i) const
{ return p[i];
}

void Partition::FromListInt(const List<int> &in, int lastElement)
{ int n = 0;
  int l = -1;
  int i=0;
  if(lastElement == -1)
    lastElement = in.size;
  for(; i<lastElement; i++)
  { if(in[i] < 0)
      crash << "Partitions should not have negative numbers in them";
    if(l == -1)
      l = in[i];
    else
      if(in[i] > l)
        crash << "Partitions should be sorted in descending order";
    if(in[i] == 0)
      break;
    n += in[i];
  }
  this->p.SetSize(i);
  this->n = n;
  for(int j=0; j<i; j++)
    p[j] = in[j];
}

void Partition::GetPartitions(List<Partition>& out, int n)
{ out.SetSize(0);
  if(n==0)
  { out.SetSize(1);
    out[0].n = 0;
    return;
  }
  if(n==1)
  { out.SetSize(1);
    out[0].n = 1;
    out[0].p.AddOnTop(1);
    return;
  }
  List<int> p;
  p.SetSize(n);
  for(int i=0; i<n; i++)
    p[i] = 0;
  p[1] = n;
  int k = 1;
  while(k != 0)
  { int x = p[k-1] + 1;
    int y = p[k] - 1;
    k -= 1;
    while(x <= y)
    { p[k] = x;
      y -= x;
      k += 1;
    }
    p[k] = x+y;
    //out.SetSize(out.size+1);
    //out[out.size-1].FromListInt(p,k+1);
    int s = out.size;
    out.SetSize(s+1);
    out[s].p.SetSize(k+1);
    for(int i=0; i<k+1; i++)
      out[s].p[i] = p[k-i];
    out[s].n = n;
  }
}

void Partition::FillTableau(Tableau& out, List<int>& stuffing) const
{ if(stuffing.size < this->n)
    crash << "need at least " << this->n << " things to stuff tableau with partition " << *this << " but you provided only " << stuffing.size << '\n' << crash;

  out.t.SetSize(this->p.size);
  int cur = 0;
  for(int i=0; i<this->p.size; i++)
  { out.t[i].SetSize(this->p[i]);
    for(int j=0; j<this->p[i]; j++)
    { out.t[i][j] = stuffing[cur];
      cur++;
    }
  }
//  stOutput << "Debugging Partition::FillTableau: " << *this << " fills out to " << out << "\n";
}

void Partition::FillTableauOrdered(Tableau& in) const
{ in.t.SetSize(this->p.size);
  int cur = 0;
  for(int i=0; i<this->p.size; i++)
  { in.t[i].SetSize(this->p[i]);
    for(int j=0; j<this->p[i]; j++)
    { in.t[i][j] = cur;
      cur++;
    }
  }
}

void Partition::GetAllStandardTableaux(List<Tableau>& out) const
{ //stOutput << "Debugging Partition::GetAllStandardTableaux with partition " << *this << "\n";
  PermutationGroup sn;
  sn.MakeSymmetricGroup(this->n);
  sn.ComputeAllElements();
  //stOutput << sn << '\n';
  GeneratorPermutationsOfList<int> perms;
  for(perms.Initialize(this->n); !perms.DoneIterating(); ++perms)
  { Tableau theTableau;
    this->FillTableau(theTableau,*perms);
    if(theTableau.IsStandard())
      out.AddOnTop(theTableau);
  }
}


void Partition::TestAllSpechtModules(int n)
{ MacroRegisterFunctionWithName("Partition::TestAllSpechtModules");
  List<Partition> partitions;
  Partition::GetPartitions(partitions, n);
  int fac = 1;
  for(int i=1; i<=n; i++)
    fac *= i;
  for(int i=0; i<partitions.size; i++)
  { //std::cout << partitions[i] << "\n";
    stOutput << partitions[i] << "\n";
    //std::cout << "got to here\n";

    List<Matrix<Rational> > repgens;
    //std::cout << "got to here2\n";
    partitions[i].SpechtModuleMatricesOfTranspositionsjjplusone(repgens);
    //std::cout << "got to here3\n";
    for(int ri=0; ri<repgens.size; ri++)
    { //std::cout << "got to here4\n";
      stOutput << repgens[ri].ToStringPlainText();
      //std::cout << "got to here5\n";
      Rational det = repgens[ri].GetDeterminant();
      stOutput << " determinant is " << det << "\n\n";
      if((det != 1) && (det != -1))
        crash << "invalid determinant" << crash;
    }
    std::cout << "got to here7\n";
    SimpleFiniteGroup<Matrix<Rational> > outg;
    std::cout << "got to here8\n";
    outg.generators = repgens;
    std::cout << "got to here9\n";
    outg.ComputeElementsAndCCs();
    std::cout << "got to here10\n";
    stOutput << outg << "\n\n\n";
    std::cout << "got to here11\n";
    if (outg.theElements.size!=0)
      if((fac % outg.theElements.size) != 0)
        crash << "invalid elements count" << crash;
  }
}

bool Partition::operator==(const Partition& right) const
{ return this->p == right.p;
}

bool Partition::operator!=(const Partition& right) const
{ return this->p != right.p;
}

// largest numbers first
bool Partition::operator<(const Partition& right) const
{ if(this->n < right.n)
    return true;
  if(this->n > right.n)
    return false;
  // largest first
  if(this->p > right.p)
    return true;
  if(this->p < right.p)
    return false;
  return false;
}

// is there a better way to do this then copypasting codes?
bool Partition::operator>(const Partition& right) const
{ if(this->n < right.n)
    return false;
  if(this->n > right.n)
    return true;
  // largest first
  if(this->p > right.p)
    return false;
  if(this->p < right.p)
    return true;
  return false;
}


std::string Partition::ToString() const
{ std::stringstream out;
  this->IntoStream(out);
  return out.str();
}


// Tableau implementation

List<int> Tableau::TurnIntoList() const
{ List<int> out;
  for(int i=0; i<this->t.size; i++)
    for(int j=0; j<this->t[i].size; j++)
      out.AddOnTop(this->t[i][j]);
  return out;
}

List<List<int> > Tableau::GetColumns() const
{ List<List<int> > out;
  if(this->t.size == 0)
    return out;
  out.SetSize(this->t[0].size);
  for(int i=0; i<this->t[0].size; i++)
  { for(int j=0; j<t.size && t[j].size > i; j++)
    { out[i].AddOnTop(t[j][i]);
    }
  }
  return out;
}

bool Tableau::IsStandard() const
{// stOutput << "Debugging Tableau::IsStandard: ";
 // stOutput << *this << " is ";
  if(this->t.size == 0)
  {// stOutput << "standard.\n";
    return true;
  }
  for(int i=0; i<this->t.size; i++)
  { int cur=-1;
    for(int j=0; j<this->t[i].size; j++)
    { if(!(cur < this->t[i][j]))
      {// stOutput << "not standard (row " << i << ").\n";
        return false;
      }
      cur = this->t[i][j];
    }
  }
  for(int i=0; i<this->t[0].size; i++)
  { int cur = -1;
    int j=0;
    while(true)
    { if(j == this->t.size)
        break;
      if(this->t[j].size <= i)
        break;
      if(!(cur <= this->t[j][i]))
      {// stOutput << "not standard (column " << i << ").\n";
        return false;
      }
      cur = this->t[j][i];
      j++;
    }
  }
  //stOutput << "standard.\n";
  return true;
}

void Tableau::RowStabilizer(PermutationGroup& in) const
{ for(int i=0; i<this->t.size; i++)
  { if(this->t[i].size == 1)
      continue;
    for(int j=1; j<this->t[i].size; j++)
    { in.generators.SetSize(in.generators.size+1);
      in.generators[in.generators.size-1].BuildTransposition(this->t[i][0],this->t[i][j]);
    }
  }
}

void Tableau::ColumnStabilizer(PermutationGroup& in) const
{ if(this->t.size == 0)
    return;
  for(int i=0; i<this->t[0].size; i++)
  { int j=1;
    while(true)
    { if((j==this->t.size) || (this->t[j].size <= i))
        break;
      in.generators.SetSize(in.generators.size+1);
      in.generators[in.generators.size-1].BuildTransposition(this->t[0][i],this->t[j][i]);
      j++;
    }
  }
}


std::string Tableau::ToString() const
{ std::stringstream out;
  this->IntoStream(out);
  return out.str();
}



// int operator*(int) expects the permutation to be canonical
// having each number exactly once
// this method can take any list of cyles
void PermutationR2::MakeCanonical()
{ int bon = this->BiggestOccurringNumber();
  List<bool> used;
  used.initFillInObject(bon+1,false);
  for(int i=0; i<used.size; i++)
    used[i] = false;
  List<List<int> > tmp;
  for(int i=0; i<=bon; i++)
  { if(used[i])
      continue;
    int head = i;
    int cur = head;
    bool incycle = false;
    while(true)
    { // same as int operator*(int) but without the short circuit
      for(int j=this->cycles.size-1; j>=0; j--)
      { for(int k=0; k<this->cycles[j].size; k++)
        { if(this->cycles[j][k] != cur)
            continue;
          if(k != this->cycles[j].size-1)
            cur = this->cycles[j][k+1];
          else
            cur = this->cycles[j][0];
        break;
        }
      }
      if(cur == head)
        break;
      if(!incycle)
      { tmp.SetSize(tmp.size + 1);
        tmp[tmp.size-1].AddOnTop(head);
        used[head] = true;
        incycle = true;
      }
      tmp[tmp.size-1].AddOnTop(cur);
      used[cur] = true;
    }
  }
//  stOutput << "MakeCanonical: turning " << *this;
  this->cycles = tmp;
//  stOutput << " into " << *this << ".";
}

int PermutationR2::BiggestOccurringNumber() const
{ int bon=0;
  for(int i=0; i<this->cycles.size; i++)
    for(int j=0; j<this->cycles[i].size; j++)
      if(bon < this->cycles[i][j])
        bon = this->cycles[i][j];
  return bon;
}

int PermutationR2::operator*(int cur) const
{ for(int i=0; i<this->cycles.size; i++)
    for(int j=0; j<this->cycles[i].size; j++)
      if(this->cycles[i][j] == cur)
      { if(j+1 != this->cycles[i].size)
          cur = this->cycles[i][j+1];
        else
          cur = this->cycles[i][0];
        break;
      }
  return cur;
}

int PermutationR2::MakeFromMul(const PermutationR2& left, const PermutationR2& right)
{ int lbon = left.BiggestOccurringNumber();
  int rbon = right.BiggestOccurringNumber();
  int bon = (lbon > rbon)?lbon:rbon;
  List<bool> unused;
  unused.SetSize(bon+1);
  for(int i=0; i<bon; i++)
    unused[i] = true;
  bool incycle = false;
  this->cycles.SetSize(0);
  for(int head=0; head<bon; head++)
  { if(!unused[head])
      continue;
    unused[head] = false;
    int cur = head;
    while(true)
    { cur = right*cur;
      cur = left*cur;
      if(cur == head)
      { incycle = false;
        break;
      }
      if(!incycle)
      { this->cycles.SetSize(this->cycles.size+1);
        this->cycles[this->cycles.size-1].SetSize(0);
        this->cycles[this->cycles.size-1].AddOnTop(head);
        incycle = true;
      }
      this->cycles[this->cycles.size-1].AddOnTop(cur);
      unused[cur] = false;
    }
  }
  return bon;
}

PermutationR2 PermutationR2::operator*(const PermutationR2& right) const
{ PermutationR2 out;
  out.MakeFromMul(*this,right);
  return out;
}

void PermutationR2::MakeID(const PermutationR2& unused)
{ this->cycles.SetSize(0);
}

void PermutationR2::Invert()
{ //stOutput << "Inverting permutation: " << *this;
  for(int i=0; i<this->cycles.size; i++)
  { if(this->cycles[i].size == 2)
      continue;
    this->cycles[i].ReverseRange(1,this->cycles[i].size);
  }
  //stOutput<< " inverts to " << *this << '\n';
}

void PermutationR2::Conjugate(const PermutationR2& conjugateMe, const PermutationR2& conjugateBy, PermutationR2& out)
{ PermutationR2 conjugateInverse = conjugateBy;
  conjugateInverse.Invert();
  PermutationR2 tmp;
  tmp.MakeFromMul(conjugateMe,conjugateBy);
  out.MakeFromMul(conjugateInverse,tmp);
}

bool PermutationR2::AreConjugate(const PermutationR2& x, const PermutationR2& y)
{ List<int> xcsh, ycsh;
  x.GetCycleStructure(xcsh);
  y.GetCycleStructure(ycsh);
  if(xcsh != ycsh)
    return false;
  return true;
}

int PermutationR2::Sign() const
{ int sign = 1;
  for(int i=0; i<this->cycles.size; i++)
    if(!(this->cycles[i].size % 2))
      sign *= -1;
  return sign;
}

void PermutationR2::BuildCycle(const List<int>& cycle)
{ this->cycles.AddOnTop(cycle);
}

void PermutationR2::BuildTransposition(int i, int j)
{ this->cycles.SetSize(this->cycles.size+1);
  this->cycles[0].SetSize(2);
  if(i<j)
  { this->cycles[0][0] = i;
    this->cycles[0][1] = j;
  }
  else
  { this->cycles[0][0] = j;
    this->cycles[0][1] = i;
  }
}

void PermutationR2::AddCycle(const List<int>& cycle)
{ this->BuildCycle(cycle);
  this->MakeCanonical();
}

void PermutationR2::AddTransposition(int i, int j)
{ this->BuildTransposition(i,j);
  this->MakeCanonical();
}

void PermutationR2::MakeFromListOfCycles(const List<List<int> >& inCycles)
{ this->cycles = inCycles;
  this->MakeCanonical();
}

// same code as operator* lolol
void PermutationR2::MakeFromActionDescription(const List<int>& actionDescription)
{ int bon = actionDescription.size-1;
  List<bool> unused;
  unused.SetSize(bon+1);
  for(int i=0; i<bon; i++)
    unused[i] = true;
  bool incycle = false;
  this->cycles.SetSize(0);
  for(int head=0; head<bon; head++)
  { if(!unused[head])
      continue;
    unused[head] = false;
    int cur = head;
    while(true)
    { cur = actionDescription[cur];
      if(cur == head)
      { incycle = false;
        break;
      }
      if(!incycle)
      { this->cycles.SetSize(this->cycles.size+1);
        this->cycles[this->cycles.size-1].SetSize(0);
        this->cycles[this->cycles.size-1].AddOnTop(head);
        incycle = true;
      }
      this->cycles[this->cycles.size-1].AddOnTop(cur);
      unused[cur] = false;
    }
  }
}

void PermutationR2::GetCycleStructure(List<int>& out) const
{ int N = 0;
  for(int i=0; i<this->cycles.size; i++)
    if(N<this->cycles[i].size)
      N = this->cycles[i].size;
  out.initFillInObject(N+1,0);

  for(int i=0; i<this->cycles.size; i++)
    out[this->cycles[i].size] += 1;
}

void PermutationR2::GetCycleStructure(Partition& out, int n_in_Sn) const
{ // unless otherwise specified, use the smallest group we are
  // canonically an element of
  if(n_in_Sn == -1)
    n_in_Sn = this->BiggestOccurringNumber();
  out.n = n_in_Sn;
  out.p.SetSize(this->cycles.size);
  for(int i=0; i<this->cycles.size; i++)
    out.p[i] = this->cycles[i].size;
  out.p.QuickSortDescending();
}

void PermutationR2::MakeFromTableauRows(const Tableau& in)
{ this->cycles.SetSize(in.t.size);
  for(int i=0; i<in.t.size; i++)
    this->cycles[i] = in.t[i];
  this->MakeCanonical();
}

bool PermutationR2::operator==(const PermutationR2& right) const
{ return this->cycles == right.cycles;
}

bool PermutationR2::IsID() const
{ return(this->cycles.size == 0);
}

unsigned int PermutationR2::HashFunction() const
{ unsigned int acc = 0;
  unsigned int n = 0;
  for(int i=0; i<this->cycles.size; i++)
    for(int j=0; j<this->cycles[i].size; j++)
    { acc += SomeRandomPrimes[n] * this->cycles[i][j];
      n++;
      if(n >= (unsigned) SomeRandomPrimesSize)
        n = 0;
    }
  return acc;
}


std::string PermutationR2::ToString() const
{ std::stringstream out;
  this->IntoStream(out);
  return out.str();
}




//  MonomialTensor<T1, T2> operator*(MonomialTensor<T1,T2>& right) const;
// should this get stuffed in MonomialTensor?
void PermutationR2::ActOnMonomialTensor(MonomialTensor<int,MathRoutines::IntUnsignIdentity>& out, const MonomialTensor<int,MathRoutines::IntUnsignIdentity>& in) const
{// stOutput << "Debugging PermutationR2::ActOnMonomialTensor: " << *this << " acting on " << in << "\n";
  int rank=0;
  for(int i=0; i<in.Powers.size; i++)
    rank += in.Powers[i];
  List<int> expanded;
  expanded.SetSize(rank);
  for(int i=0,cur=0; i<in.generatorsIndices.size; i++)
     for(int j=0; j<in.Powers[i]; j++)
     { expanded[cur] = in.generatorsIndices[i];
         cur++;
     }
  //stOutput << "Exponents expanded: ";
  //for(int i=0; i<expanded.size; i++)
  //  stOutput << expanded[i] << " ";
  this->ActOnList(expanded);
  //stOutput << "\nIndices permuted:   ";
  //for(int i=0; i<expanded.size; i++)
  //  stOutput << expanded[i] << " ";
  //stOutput << "\n";
  int i=0, xi=0;
  while(xi < expanded.size)
  { out.generatorsIndices.AddOnTop(expanded[xi]);
    xi++;
    for(int j=1; ; j++,xi++)
      if((xi==expanded.size) || (expanded[xi]!=out.generatorsIndices[i]))
      { out.Powers.AddOnTop(j);
        break;
      }
    i++;
  }
  //stOutput << "Reassembled: " << out << "\n";
}

void PermutationR2::GetWordjjPlus1(List<int>& word) const
{ List<int> transpositions;
  for(int i=0; i<this->cycles.size; i++)
  { for(int j=0; j<this->cycles[i].size-1; j++)
    { transpositions.AddOnTop(this->cycles[i][j]);
      transpositions.AddOnTop(this->cycles[i][j+1]);
    }
  }
  for(int i=0; i<transpositions.size; i+=2)
  { int ti = transpositions[i];
    int tj = transpositions[i+1];
    if(ti > tj)
    { int tk = ti;
      ti = tj;
      tj = tk;
    }
    int tl = tj - ti -1;
    for(int tt=0; tt<tl; tt++)
      word.AddOnTop(ti+tt);
    word.AddOnTop(tj-1);
    int end = word.size-2;
    for(int tt=0; tt<tl; tt++)
    { word.SetSize(word.size+1);
      word[word.size-1] = word[end-tt];
    }
  }
  //stOutput << *this << " has transpositions " << transpositions.ToStringCommaDelimited() << " and word " << word.ToStringCommaDelimited() << '\n';
}


void PermutationGroup::MakeSymmetricGroup(int n)
{ this->generators.SetSize(n-1);
  for(int i=0; i<n-1; i++)
  { this->generators[i].AddTransposition(0,i+1);
  }
  this->isSymmetricGroup = true;
  this->hasGenerators1j = true;
  this->ComputeCCSizesAndRepresentativesByFormula = PermutationGroup::ComputeCCSizesAndRepresentativesByFormulaImplementation;
  this->AreConjugateByFormula = PermutationR2::AreConjugate;
  this->GetSizeByFormula = PermutationGroup::GetSizeByFormulaImplementation;
}

void PermutationGroup::MakeSymmetricGroupGeneratorsjjPlus1(int n)
{ this->generators.SetSize(n-1);
  for(int i=0; i<n-1; i++)
  { this->generators[i].AddTransposition(i,i+1);
  }
  this->isSymmetricGroup = true;
  this->hasGeneratorsjjPlus1 = true;
  this->ComputeCCSizesAndRepresentativesByFormula = PermutationGroup::ComputeCCSizesAndRepresentativesByFormulaImplementation;
  this->AreConjugateByFormula = PermutationR2::AreConjugate;
  this->GetSizeByFormula = PermutationGroup::GetSizeByFormulaImplementation;
  this->GetWordByFormula = PermutationGroup::GetWordjjPlus1Implementation;
}

/*bool PermutationGroup::AreConjugate(const PermutationR2& x, const PermutationR2& y)
{ if(this->isSymmetricGroup)
    return PermutationR2::AreConjugate(x,y);
  return this->SimpleFiniteGroup<PermutationR2>::AreConjugate(x,y);
}*/

int PermutationGroup::GetSizeByFormulaImplementation(void* GG)
{ PermutationGroup* G = (PermutationGroup*) GG;
  if(G->isSymmetricGroup)
    return MathRoutines::Factorial(G->generators.size + 1);
  crash << "This method should not have been called " << __FILE__ << ":" << __LINE__ << crash;
  // control reaches end of non-void function
  return -1;
}

void PermutationGroup::ComputeCCSizesAndRepresentativesByFormulaImplementation(void* GG)
{ PermutationGroup* G = (PermutationGroup*) GG;
  if(!G->isSymmetricGroup)
    return G->ComputeCCSizesAndRepresentatives();
  G->flagCCsComputed = true;
  int N = G->generators.size + 1;
  List<Partition> parts;
  Partition::GetPartitions(parts, N);
  G->conjugacyClasseS.SetSize(parts.size);
  int facn = MathRoutines::Factorial(N);
  for(int i=0; i<parts.size; i++)
  { Tableau t;
    parts[i].FillTableauOrdered(t);
    G->conjugacyClasseS[i].representative.MakeFromTableauRows(t);
    G->GetWord(G->conjugacyClasseS[i].representative, G->conjugacyClasseS[i].representativeWord);
    G->conjugacyClasseS[i].haveRepresentativeWord = true;
    G->conjugacyClasseS[i].size = facn / parts[i].Fulton61z();
  }
}

/*void PermutationGroup::GetWord(const PermutationR2& g, List<int>& word)
{ if(this->isSymmetricGroup && this->hasGeneratorsjjPlus1)
    return g.GetWordjjPlus1(word);
  crash << "Maybe try implementing PermutationR2::GetWord1j or for whatever generator set you have " << __FILE__ << ":" << __LINE__ << crash;
}*/

void PermutationGroup::GetWordjjPlus1Implementation(void* G, const PermutationR2& g, List<int>& word)
{ g.GetWordjjPlus1(word);
}

std::string PermutationGroup::ToString()
{ std::stringstream out;
  this->IntoStream(out);
  return out.str();
}



void ElementHyperoctahedralGroup::MakeFromPermutation(const PermutationR2& in)
{ this->p = in;
}

void ElementHyperoctahedralGroup::MakeFromBits(const List<bool>& in)
{ this->s = in;
}

bool NeedRankAdjustment(const ElementHyperoctahedralGroup& left, const ElementHyperoctahedralGroup& right)
{ int theRank=left.s.size;
  if (theRank==0)
    theRank = right.s.size;
  if (theRank==0)
    return false;
  if (left.s.size==0)
    return true;
  else
    if (left.s.size!=theRank)
      crash <<"Change here and maybe elsewhere if you want to do stuff with multiple ranks of ElementHyperoctahedralGroup" << crash;
  if (right.s.size==0)
    return true;
  else
    if (right.s.size!=theRank)
      crash <<"Change here and maybe elsewhere if you want to do stuff with multiple ranks of ElementHyperoctahedralGroup" << crash;
  return false;
}


void EnsureSameRank(ElementHyperoctahedralGroup& left, ElementHyperoctahedralGroup& right)
{ int theRank=left.s.size;
  if (theRank==0)
    theRank = right.s.size;
  if (theRank==0)
    return;
  if (left.s.size==0)
    left.s.initFillInObject(theRank, false);
  if (right.s.size==0)
    right.s.initFillInObject(theRank, false);
}

void ElementHyperoctahedralGroup::MakeFromMul(const ElementHyperoctahedralGroup& left, const ElementHyperoctahedralGroup& right)
{ this->p.MakeFromMul(left.p,right.p);
  int rightprank = right.p.BiggestOccurringNumber()+1;
  int thisssize = std::max(left.s.size,rightprank);
  thisssize = std::max(thisssize, right.s.size);
  this->s.SetSize(thisssize);
  for(int i=0; i<left.s.size; i++)
    this->s[i] = left.s[i];
  for(int i=left.s.size; i<thisssize; i++)
    this->s[i] = false;
  // see the documentation for ElementHyperoctahedralGroup if you
  // don't understand why the right wants to act on the left in this way
  // the following three lines are the mathematical part of this function,
  // the rest is to ensure the element looks good or whatever
  right.p.ActOnList(this->s);
  for(int i=0; i<right.s.size; i++)
    this->s[i] ^= right.s[i];

  int lastset = 0;
  for(int i=0; i<this->s.size; i++)
    if(this->s[i])
      lastset = i;
  this->s.SetSize(lastset+1);

/*  if(NeedRankAdjustment(left, right))
  { ElementHyperoctahedralGroup newLeft=left;
    ElementHyperoctahedralGroup newRight=right;
    EnsureSameRank(newLeft, newRight);
    this->MakeFromMul(newLeft, newRight);
    return;
  }
  if(right.s.size != 0)
  { this->s = left.s;
    right.p.ActOnList(this->s);
    for(int i=0; i<this->s.size; i++)
      this->s[i] ^= right.s[i];
  }
  if(left.s.size != 0)
  { this->s = left.s;
    right.p.ActOnList(this->s);
  }*/

  //stOutput << left << "*" << right << "=" << *this << "   ";
}

int ElementHyperoctahedralGroup::SmallestN()  const
{ int pn = this->p.BiggestOccurringNumber()+1;
  if(this->p.cycles.size == 0)
    pn = 0;
  int sn = 0;
  for(int i=0; i<this->s.size; i++)
    if(this->s[i])
      sn = i;
  return pn > sn ? pn : sn;
}

ElementHyperoctahedralGroup ElementHyperoctahedralGroup::operator*(const ElementHyperoctahedralGroup& right) const
{ ElementHyperoctahedralGroup out;
  out.MakeFromMul(*this, right);
  return out;
}

void ElementHyperoctahedralGroup::MakeID(const ElementHyperoctahedralGroup& unused)
{ this->p.MakeID(this->p);
  this->s.SetSize(0);
}

bool ElementHyperoctahedralGroup::IsID() const
{ if(!this->p.IsID())
    return false;
  for(int i=0; i<this->s.size; i++)
    if(this->s[i])
      return false;
  return true;
}

void ElementHyperoctahedralGroup::Invert()
{ this->p.Invert();
}

void ElementHyperoctahedralGroup::Conjugate(const ElementHyperoctahedralGroup& element, const ElementHyperoctahedralGroup& conjugateBy, ElementHyperoctahedralGroup& out)
{ ElementHyperoctahedralGroup conjugateInverse;
  conjugateInverse.p = conjugateBy.p;
  conjugateInverse.p.Invert();
  conjugateInverse.s = conjugateBy.s;
  ElementHyperoctahedralGroup tmp;
  tmp.MakeFromMul(element,conjugateBy);
  out.MakeFromMul(conjugateInverse,tmp);
  //stOutput << element << "^" << conjugateBy << "=" << out << "  ";
}

int ElementHyperoctahedralGroup::CountSetBits() const
{ int cnt = 0;
  for(int i=0; i<this->s.size; i++)
    if(this->s[i])
      cnt++;
  return cnt;
}

bool ElementHyperoctahedralGroup::HasDifferentConjugacyInvariantsFrom(const ElementHyperoctahedralGroup& other) const
{ return !ElementHyperoctahedralGroup::AreConjugate(*this, other);
}

//static gets applied exactly once for some reason
bool ElementHyperoctahedralGroup::AreConjugate(const ElementHyperoctahedralGroup& x, const ElementHyperoctahedralGroup& y)
{ int xsp = x.CountSetBits();
  int ysp = y.CountSetBits();
  if(xsp != ysp)
    return false;
  return PermutationR2::AreConjugate(x.p,y.p);
}

void ElementHyperoctahedralGroup::GetCharacteristicPolyStandardRepresentation(Polynomial<Rational>& out) const
{ Matrix<Rational> M;
  int n = this->SmallestN();
  if(n==0) // this is the identity element.  it thinks it lives in the zero group, because it lives in all the groups
  { out.MakeConst(1);
    return;
  }
  M.init(n,n);
  M.MakeZero();
  for(int i=0; i<n; i++)
  { int a = this->p*i;
    M.elements[i][a] = (this->s[i])?-1:1; // ahaha 0 is false
  }
  out.AssignCharPoly(M);
}


bool ElementHyperoctahedralGroup::operator==(const ElementHyperoctahedralGroup& right) const
{ /*if(this->s.size > 0)
    if(right.s.size > 0)
      for(int i=0; i<this->s.size; i++)
        if(this->s[i] != right.s[i])
          return false;
  if(this->s.size > 0)
    if(right.s.size == 0)
      for(int i=0; i<this->s.size; i++)
        if(this->s[i] != false)
          return false;
  if(this->s.size == 0)
    if(right.s.size > 0)
      for(int i=0; i<right.s.size; i++)
        if(right.s[i] != false)
          return false;*/
  int sharedsize = std::min(this->s.size, right.s.size);
  for(int i=0; i<sharedsize; i++)
    if(this->s[i] != right.s[i])
      return false;
  for(int i=sharedsize; i<this->s.size; i++)
    if(this->s[i])
      return false;
  for(int i=sharedsize; i<right.s.size; i++)
    if(right.s[i])
      return false;
  return this->p == right.p;
}

unsigned int ElementHyperoctahedralGroup::HashFunction() const
{ int acc = this->p.HashFunction();
  // idk how to make bools their own like hash function or whatever lol
  // I mean why not just have a parametric collection of global HashFunction
  // that does normal stuff to normal types and calls .HashFunction() on
  // class types or whatever
  // its ok though because I need to be sure that a string of false at the end
  // hashes to the exact same thing as not having a string of false at the end
  for(int i=0; (i<this->s.size)&&(i<SomeRandomPrimesSize); i++)
    acc += this->s[i] ? SomeRandomPrimes[i] : 0;
  return acc;
}

bool ElementHyperoctahedralGroup::operator>(const ElementHyperoctahedralGroup& right) const
{ int n = this->s.size;
  // the following register load and jump instructions will likely never be used
  // but they are fast
  int nr = right.s.size;
  if(n > nr)
    return true;
  if(n<nr)
    return false;
  // ok back to stuff that gets used
  Partition l,r;
  this->p.GetCycleStructure(l,n);
  right.p.GetCycleStructure(r,n);
  if(l>r)
    return true;
  if(r>l)
    return false;
  if(this->p.cycles>right.p.cycles)
    return true;
  if(this->p.cycles<right.p.cycles)
    return false;
  if(this->s>right.s)
    return true;
  if(this->s<right.s)
    return false;
  return false;
}


std::string ElementHyperoctahedralGroup::ToString(FormatExpressions* unused) const
{ std::stringstream out;
  this->IntoStream(out);
  return out.str();
}

void HyperoctahedralGroup::MakeHyperoctahedralGroup(int n)
{ this->isEntireHyperoctahedralGroup = true;
// these methods are currently broken because I don't actually know what conjugacy
// in hyperoctahedral groups means
//this->ComputeCCSizesAndRepresentativesByFormula = HyperoctahedralGroup::ComputeCCSizesAndRepresentativesByFormulaImplementation;
//  this->AreConjugateByFormula = ElementHyperoctahedralGroup::AreConjugate;
  this->GetWordByFormula = HyperoctahedralGroup::GetWordByFormulaImplementation;
  this->GetSizeByFormula = HyperoctahedralGroup::GetSizeByFormulaImplementation;
  this->N = n;
  this->generators.SetSize(n-1+n);
  for(int i=0; i<n-1; i++)
  { this->generators[i].p.AddTransposition(i,i+1);
    this->generators[i].s.SetSize(n);
    for(int j=0; j<n; j++)
      this->generators[i].s[j] = false;
  }
  for(int i=0; i<n; i++)
  { this->generators[i+n-1].s.SetSize(n);
    for(int j=0; j<n; j++)
    { this->generators[i+n-1].s[j] = false;
    }
    this->generators[i+n-1].s[i] = true;
  }
}


template <typename someGroup, typename elementSomeGroup>
void MissingGeneratorsSubgroupElementGetWord(void* Hp, elementSomeGroup& g, List<int>& out)
{ Subgroup<someGroup, elementSomeGroup> *H = (Subgroup<someGroup, elementSomeGroup>*) Hp;
  List<int> superword;
  H->parent->GetWord(g, superword);
  out.SetSize(0);
  for(int i=0; i<superword.size; i++)
  { if(!H->superGeneratorWordExists[superword[i]])
    { if(!H->HasElement(g))
        stOutput << "element " << g << " isn't even a member of " << H << '\n';
      crash << "element " << g << " is assigned parent word " << superword.ToStringCommaDelimited()
            << " containing generator not found in subgroup " << superword[i]
            << " so if this does belong to the subgroup, we need a better algorithm at "  << __FILE__ << ":" << __LINE__ << crash;
    }
    out.AddListOnTop(H->superGeneratorWords[superword[i]]);
  }
  stOutput << "MissingGeneratorsSubgroupElementGetWord: " << g << " is assigned word " << out.ToStringCommaDelimited()
           << " translated from parent group's word " << superword.ToStringCommaDelimited() << '\n';
}

// Bp = <[12:],[23:],[:001],[:010],[:100]>
// Bm = <[45:],[56:],[:001],[:010],[:100]>
// Bn ⊃ Bp × Bm, with [34:] missing, we represent [34:] with an identity matrix
// It is conceivable that this function can be replaced with a geneneric induce function
// in a subgroup datatype that maps generators to generators.
void HyperoctahedralGroup::SpechtModuleOfPartititons(const Partition& positive, const Partition& negative,
                                                     GroupRepresentation<SimpleFiniteGroup<ElementHyperoctahedralGroup>,Rational> &out,
                                                     PermutationGroup* subsn)
{ List<Matrix<Rational> > pozm, negm;
  stOutput << "HyperoctahedralGroup::SpectModuleOfPartitions(" << positive << ", " << negative << ")\n";
  // the next two things should be done in parallel.  How can I make that happen?
  positive.SpechtModuleMatricesOfTranspositionsjjplusone(pozm);
  negative.SpechtModuleMatricesOfTranspositionsjjplusone(negm);

  if(subsn == NULL)
  { PermutationGroup sn;
    subsn = &sn;
  }
  if(!subsn->isSymmetricGroup)
    subsn->MakeSymmetricGroupGeneratorsjjPlus1(this->N);
  Subgroup<PermutationGroup, PermutationR2> PxM;
  List<PermutationR2> subgens;
  subgens = subsn->generators;
  if(pozm.size + negm.size != subsn->generators.size)
    subgens.RemoveIndexShiftDown(pozm.size);
  PxM.initFromGroupAndGenerators(*subsn, subgens);
  PxM.superGeneratorWords.SetSize(subsn->generators.size);
  PxM.superGeneratorWordExists.SetSize(subsn->generators.size);
  for(int i=0; i<subsn->generators.size; i++)
  { PxM.superGeneratorWords[i].AddOnTop(i);
    PxM.superGeneratorWordExists[i] = true;
  }
  if(pozm.size + negm.size != subsn->generators.size)
  { PxM.superGeneratorWordExists[pozm.size] = false;
    for(int i=pozm.size+1; i<subsn->generators.size; i++)
      PxM.superGeneratorWords[i][0] -= 1;
  }
  stOutput << "Generating subgroup:\n";
  for(int i=0; i<subsn->generators.size; i++)
    stOutput << i << " " << subsn->generators[i] << " " << PxM.superGeneratorWordExists[i] << " "
             << PxM.superGeneratorWords[i].ToStringCommaDelimited() << '\n';
  for(int i=0; i<PxM.generators.size; i++)
    stOutput << i << " " << PxM.generators[i] << '\n';
  //PxM.CosetRepresentativeEnumerator = HyperoctahedralGroup::ParabolicSubgroupCosetRepresentativeEnumerator;
  PxM.GetWordByFormula = MissingGeneratorsSubgroupElementGetWord<PermutationGroup, PermutationR2>;
  GroupRepresentation<Subgroup<PermutationGroup, PermutationR2>, Rational> pxmr;
  pxmr.ownerGroup = &PxM;
  pxmr.generatorS.SetSize(PxM.generators.size);
  Matrix<Rational> hid, kid;
  if(pozm.size == 0)
    hid.MakeIdMatrix(1);
  else
    hid.MakeID(pozm[0]);
  if(negm.size == 0)
    kid.MakeIdMatrix(1);
  else
    kid.MakeID(negm[0]);
  int i=0;
  for(int pi=0; pi<pozm.size; i++, pi++)
    pxmr.generatorS[i].AssignTensorProduct(pozm[pi], kid);
  for(int ni=0; ni<negm.size; i++, ni++)
    pxmr.generatorS[i].AssignTensorProduct(hid, negm[ni]);
  GroupRepresentation<PermutationGroup, Rational> subsnr;
  PxM.InduceRepresentation(pxmr,subsnr);
  out.generatorS = subsnr.generatorS;
  out.generatorS.SetSize(generators.size);
  i=subsnr.generatorS.size;
  for(int psi=0; psi<positive.n; i++, psi++)
    out.generatorS[i].MakeID(out.generatorS[0]);
  for(int nsi=0; nsi<negative.n; i++, nsi++)
  { out.generatorS[i].MakeID(out.generatorS[0]);
    out.generatorS[i] *= -1;
  }
  std::stringstream ids;
  ids << negative << ", " << positive;
  out.identifyingString = ids.str();
  out.ownerGroup = this;
  if(!out.VerifyRepresentation())
    crash << crash;
}

void HyperoctahedralGroup::AllSpechtModules()
{ int N = this->GetN();
  PermutationGroup subsn;
  subsn.MakeSymmetricGroupGeneratorsjjPlus1(N);
  for(int p=0; p<=N; p++)
  { List<Partition> nps;
    Partition::GetPartitions(nps,p);
    for(int npi=0; npi<nps.size; npi++)
    { List<Partition> pps;
      Partition::GetPartitions(pps,N-p);
      for(int ppi=0; ppi<pps.size; ppi++)
      { GroupRepresentation<SimpleFiniteGroup<ElementHyperoctahedralGroup>, Rational> sm;
        stOutput << "Computing representation {" << nps[npi] << "}, {" << pps[ppi] << "}\n";
        this->SpechtModuleOfPartititons(pps[ppi],nps[npi],sm,&subsn);
        sm.VerifyRepresentation();
        stOutput << sm << '\n';
        this->irreps.AddOnTop(sm);
      }
    }
  }
  this->irreps.QuickSortAscending();
  stOutput << this->PrettyPrintCharacterTable() << '\n';
  RepresentationDataIntoJS().writefile("representations_hyperoctahedral_group");
}

int HyperoctahedralGroup::GetN()
{ if(isEntireHyperoctahedralGroup)
    return this->N;
  this->N = 0;
  for(int i=0; i<this->generators.size; i++)
  { int gn = generators[i].SmallestN();
    if(gn > this->N)
      this->N = gn;
  }
  return this->N;
}

int HyperoctahedralGroup::GetSizeByFormulaImplementation(void* GG)
{ HyperoctahedralGroup* G = (HyperoctahedralGroup*) GG;
  stOutput << "HyperoctahedralGroup::GetSize() called.  N=" << G->N << '\n';
  if(G->isEntireHyperoctahedralGroup)
    return MathRoutines::Factorial(G->N) * (1<<G->N);
  if(G->isEntireDn)
    return MathRoutines::Factorial(G->N) * (1<<G->N) / 2;
  crash << "This method should not have been called " << __FILE__ << ":" << __LINE__ << crash;
  // control reaches end of non-void function
  return -1;
}

void HyperoctahedralGroup::GetWordByFormulaImplementation(void* GG, const ElementHyperoctahedralGroup& g, List<int>& word)
{ HyperoctahedralGroup* G = (HyperoctahedralGroup*) GG;
  if(G->isEntireHyperoctahedralGroup)
  { g.p.GetWordjjPlus1(word);
    for(int i=0; i<g.s.size; i++)
      if(g.s[i])
        word.AddOnTop(G->N-1+i);
    return;
  }
  crash << "This method should not have been called " << __FILE__ << ":" << __LINE__ << crash;
}

// number of conjugacy classes: partitions * #[0,N]
// representative of conjugacy classes: (a standard tableau, first k bits)
// number of elements of conjugacy classes: n!/(elements in row)/(same rows)! * (n choose k)
void HyperoctahedralGroup::ComputeCCSizesAndRepresentativesByFormulaImplementation(void* GG)
{ HyperoctahedralGroup* G = (HyperoctahedralGroup*) GG;
  if(!G->isEntireHyperoctahedralGroup)
    return G->ComputeCCSizesAndRepresentatives();
  List<Partition> ccp;
  Partition::GetPartitions(ccp, G->N);
  int psize = ccp.size;
  int ssize = G->N+1;
  int nfac = 1;
  for(int i=2; i<=G->N; i++)
    nfac *= i;
  G->conjugacyClasseS.SetSize(psize * ssize);
  for(int pi=0; pi<psize; pi++)
  { int p61z = ccp[pi].Fulton61z();
    int snccsz = nfac / p61z;
    for(int si=0; si<ssize; si++)
    { Tableau t;
      ccp[pi].FillTableauOrdered(t);
      G->conjugacyClasseS[pi*ssize + si].representative.p.MakeFromTableauRows(t);
      G->conjugacyClasseS[pi*ssize + si].representative.s.SetSize(G->N);
      for(int i=0; i<G->N; i++)
        if(i<si)
          G->conjugacyClasseS[pi*ssize + si].representative.s[i] = true;
        else
          G->conjugacyClasseS[pi*ssize + si].representative.s[i] = false;
      G->conjugacyClasseS[pi*ssize + si].size = snccsz;
      G->conjugacyClasseS[pi*ssize+si].size *= MathRoutines::NChooseK(G->N, si);
      G->conjugacyClasseS[pi*ssize+si].haveRepresentativeWord = true;
      G->GetWord(G->conjugacyClasseS[pi*ssize+si].representative, G->conjugacyClasseS[pi*ssize+si].representativeWord);
    }
  }
  G->flagCCsComputed = true;
  /* VerifyFormulaCCRepresentatives
  if(this->easyConjugacyDetermination)
    for(int i=0; i<this->conjugacyClasseS.size; i++)
      for(int j=i+1; j<this->conjugacyClasseS.size; j++)
        if(this->AreConjugate(this->conjugacyClasseS[i].representative, this->conjugacyClasseS[j].representative))
          crash << "Claimed conjugacy class representatives are actually conjugate " << __FILE__ << ":" << __LINE__ << crash;
  */
}


std::string HyperoctahedralGroup::ToString() const
{ std::stringstream out;
  this->IntoStream(out);
  return out.str();
}


template <>
bool FiniteGroup<ElementHyperoctahedralGroup>::AreConjugate
(const ElementHyperoctahedralGroup& x, const ElementHyperoctahedralGroup& y)
{return ElementHyperoctahedralGroup::AreConjugate(x,y);}
