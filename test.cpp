#include "vpf.h"
// ^ that is first for precompilation

#include "vpfCharacters.h"
#include "vpfGraph.h"
#include "testlib.h"
#include <iostream>

/*
template <typename vector>
class LinearSpace
{ public:
  List<vector> basis;

  void AddToSpace(const vector &v);
  int dim() const;
  bool Contains(const vector &v) const;
};

void AddToSpace(const vector &v)
{
}
*/

ElementMonomialAlgebra<CoxeterElement, Rational> FromCharacter(const Character<Rational>& X)
{ ElementMonomialAlgebra<CoxeterElement, Rational> out;
  for(int i=0; i<X.G->ccCount; i++)
  { for(int j=0; j<X.G->ccSizes[i]; j++)
    { CoxeterElement tmp = X.G->GetCoxeterElement(X.G->conjugacyClasses[i][j]);
      out.AddMonomial(tmp,X.data[i]);
    }
  }
  return out;
}



// G_ij^k
// (G_i)j = k
//

template <typename coefficient>
Matrix<coefficient> GetMatrix(const Character<coefficient>& X)
{ Matrix<coefficient> M;
  M.MakeZeroMatrix(X.G->N);
  for(int i1=0; i1<X.G->ccCount; i1++)
  { for(int i2=0; i2<X.G->ccSizes[i1]; i2++)
    { int i=X.G->conjugacyClasses[i1][i2];
      for(int j=0; j<X.G->N; j++)
      { M.elements[j][X.G->MultiplyElements(i,j)] = X.data[i1];
      }
    }
  }
  return M;
}

int skipcount(int n, const List<int>& l)
{ int o = 0;
  for(int i=0;i<l.size;i++)
  { if(l[i] == n)
      return -1;
    if(l[i]<n)
    { o++;
    }
  }
  return n-o;
}

int popcnt(int i) // from the internet
{ i = i - ((i >> 1) & 0x55555555);
  i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
  return ((((i + (i >> 4)) & 0xF0F0F0F) * 0x1010101) & 0xffffffff) >> 24;
}

List<List<int> > powersetgrade(int G, const List<int>& l)
{ List<List<int> > out;
  for(int i=0; i<1<<l.size; i++)
  { if(popcnt(i)==G){
      List<int> outi;
      for(int j=0;j<l.size;j++){
        if(i&(1<<j))
          outi.AddOnTop(j);
      }
      out.AddOnTop(outi);
    }
  }
  return out;
}

template<typename Element>
Element minor_det(const Matrix<Element>& M, const List<int>& l)
{
  Matrix<Element> MM;
  MM.init(M.NumRows-l.size,M.NumCols-l.size);
  for(int i=0; i<M.NumRows; i++)
  { int ii = skipcount(i,l);
    if(ii == -1)
      continue;
    for(int j=0; j<M.NumCols; j++)
    { int jj = skipcount(j,l);
      if(jj == -1)
        continue;
      MM.elements[ii][jj] = M.elements[i][j];
    }
  }
//  Element x;
//  MM.ComputeDeterminantOverwriteMatrix(&x);
//  return x;
  return MM.GetDeterminant();
}

template<typename Element>
List<Element> charpoly(const Matrix<Element>& M)
{ List<Element> p;
  p.SetSize(M.NumRows+1);
  List<int> rowids;
  for(int i=0; i<M.NumRows; i++)
    rowids.AddOnTop(i);
  for(int i=0; i<M.NumRows; i++)
  { Element acc = 0;
    List<List<int> > ps = powersetgrade(i,rowids);
    for(int si=0; si < ps.size; si++)
      acc += minor_det(M,ps[si]);
    if(i%2==0)
      acc = -acc;
    p[M.NumRows-i] = acc;
  }
  p[0] = ((M.NumRows%2)==0)? -1 : 1;

  return p;
}

/*Rational Matrix<Rational>::GetTrace(){
    Rational acc = 0;
    for(int i=0; i<this.NumCols; i++){
        acc += this.elements[i][j];
    }
    return acc;
}*/

int chartable[10][10] =
{
{1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
{1, -1, -1,  1,  1, -1,  1,  1, -1, -1},
{3,  1,  1, -1,  1,  0,  0, -1, -1, -3},
{3, -1, -1, -1,  1,  0,  0, -1,  1,  3},
{2,  0, -2,  0,  0,  1, -1,  2,  0, -2},
{2,  0,  2,  0,  0, -1, -1,  2,  0,  2},
{1,  1, -1, -1, -1, -1,  1,  1,  1, -1},
{1, -1,  1, -1, -1,  1,  1,  1, -1,  1},
{3, -1,  1,  1, -1,  0,  0, -1,  1, -3},
{3,  1, -1,  1, -1,  0,  0, -1, -1,  3}
};

bool VerifyChartable(const CoxeterGroup &G, bool printresults = false){
  bool okay = true;
  if(printresults)
    std::cout << "one" << '\n';
  for(int i=0; i<G.ccCount; i++)
  { if(printresults)
      std::cout << G.characterTable[i].norm() << std::endl;
    if(G.characterTable[i].norm() != 1)
      okay = false;
  }
  if(printresults)
    std::cout << "zero" << '\n';
  for(int i=0; i<G.ccCount; i++)
    for(int j=0; j<G.ccCount; j++)
    { if(j==i)
        continue;
      if(G.characterTable[i].IP(G.characterTable[j]) != 0)
        okay = false;
      if(printresults)
        std::cout << G.characterTable[i].IP(G.characterTable[j]) << std::endl;
    }
  return okay;
}


int main(void){
/*
    Rational zero = Rational(0,1);
    Rational one = Rational(1,1);
    Rational two = Rational(2,1);
    std::cout << zero*one << "\n" << zero+one << one*two << one+two << two*two << std::endl;

//    Vector<Rational> v;
//    v.SetSize(3);
//    v="(1/2,3,-1/5)";
//    v[1].AssignString("1/3");
  //  v[2]=5;
    Vector<Rational> v;
    v = "(1/3,1,2)";
    std::cout << v << std::endl;

    Vector<Rational> w;
    w.SetSize(3);
    w[0] = 0;
    w[1] = 1;
    w[2] = 2;
    std::cout << w << std::endl;

    Matrix<Rational> M;
    M.Resize(3,3,false,&zero);
    M.ActOnVectorColumn(v,zero);
    std::cout << M << std::endl;
    std::cout << v << std::endl;
*/
/*
    Rational one = 1;
    DynkinType D;
    D.MakeSimpleType('B',3,&one);
    WeylGroup G;
    G.MakeFromDynkinType(D);
    std::cout << G << std::endl;
    std::cout << G.CartanSymmetric << std::endl;
    G.ComputeWeylGroup();
    std::cout << G << std::endl;
*/
/*
    Vector<Rational> v1; v1 = "(1,0)";
    Vector<Rational> v2; v2 = "(0,1)";

    std::cout << D << std::endl;
    D.GetCartanSymmetric(M);
    std::cout << M << std::endl;
    Vectors<Rational> V;
    G.ComputeWeylGroupAndRootsOfBorel(V);
    std::cout << G.GetReport() << std::endl;
    std::cout << "printing G " << G << std::endl;
    G.GetMatrixOfElement(0,M);


    Graph *g = new Graph(6,3);
    g->AddEdge(0,1);
    g->AddEdge(1,2);
    g->AddEdge(2,3);
    g->AddEdge(4,5);
    g->AddEdge(0,3);
    std::cout << "g" << g->edges << std::endl;
    std::cout << g->data << std::endl;
    List<List<int> > ccs;
    ccs = g->DestructivelyGetConnectedComponents();
    std::cout << ccs << std::endl;

    std::cout << G.RootsOfBorel << std::endl;

    std::cout << G[0] << '\n' << G[1] << '\n' << G[2] << '\n' << std::endl;
    M = GetMatrixOfElement(G,G[6]);
    std::cout << GetMatrixOfElement(G,G[6]) << std::endl;
    std::cout << G[6] << ' ' << M.GetDeterminant() << ' ' << M(0,0)+M(1,1)+M(2,2) << std::endl;
*/


  DynkinType D;
  D.MakeSimpleType('B',3);
  Matrix<Rational> M;
  D.GetCartanSymmetric(M);
  std::cout << M << std::endl;

  CoxeterGroup G;
  G.MakeFrom(D);
  G.ComputeInitialCharacters();
  for(int i=0;i<G.characterTable.size;i++)
  std::cout << G.characterTable[i] << std::endl;

  for(int i=0;i<G.ccCount;i++){
    int n = G.conjugacyClasses[i][0];
    Matrix<Rational> M = G.TodorsVectorToMatrix(G.rhoOrbit[n]);
    std::cout << i << charpoly(M) << "\n\n";
  //  std::cout << M << '\n';
  //  std::cout << "\n\n";
  }

  G.characterTable.size = 0;
  for(int i=0; i<10; i++){
    Character<Rational> X;
    X.G = &G;
    for(int j=0; j<10; j++){
      X.data.AddOnTop(chartable[i][j]);
    }
    G.characterTable.AddOnTop(X);
  }

  std::cout << G.characterTable << std::endl;

  GroupRingElement<Rational> XeP;
  XeP.MakeFromCharacter(G.characterTable[0]);
  std::cout << XeP.data << std::endl;

  GroupRingElement<Rational> E;
  E.MakeEi(&G,0);
  std::cout << E << std::endl;
  std::cout << XeP*E << std::endl;

  GroupRingElement<Rational> XstdP;
  XstdP.MakeFromCharacter(G.characterTable[2]);
  std::cout << XstdP << std::endl;

  std::cout << "\nhi there\n";
  for(int i=0; i<G.N; i++)
  { E.MakeEi(&G,i);
    std::cout << XstdP*E << std::endl;
  }

  std::cout << GetMatrix(G.characterTable[0]) << std::endl;
  std::cout << GetMatrix(G.characterTable[2]) << std::endl;

  E.MakeEi(&G,33);
  std::cout << XstdP*E << std::endl;
  std::cout << GetMatrix(G.characterTable[2])*E.data << std::endl;

  ElementMonomialAlgebra<CoxeterElement, Rational> Xstdp = FromCharacter(G.characterTable[2]);
  std::cout << Xstdp << std::endl;
  std::cout << "here they are" << std::endl;
  List<ElementMonomialAlgebra<CoxeterElement, Rational> > l;
  for(int i=0; i<G.N; i++)
  { ElementMonomialAlgebra<CoxeterElement, Rational> tmp;
    tmp.AddMonomial(G.GetCoxeterElement(i),1);
    tmp *= Xstdp;
    l.AddOnTop(tmp);
    std::cout << tmp << '\n' << std::endl;
  }
  Xstdp.GaussianEliminationByRows(l);
  std::cout << l << std::endl;
/*  Matrix<Rational> ct;
  ct.init(G.ccCount,G.ccCount);
  for(int i=0; i<G.ccCount; i++)
    for(int j=0; j<G.ccCount; j++)
    { ct.elements[i][j] = chartable[i][j];
      ct.elements[i][j] /= G.N;
      ct.elements[i][j] *= G.ccSizes[j];
    }

  List<Vector<Rational> > powers;
  Vector<Rational> v;
  v = G.characterTable[2].data;
  std::cout << v << '\n' << ct*v << '\n';
  powers.AddOnTop(v);
  for(int i=1; i<50; i++)
  { Vector<Rational> v;
    v = G.characterTable[2].data;
    for(int j=0; j<G.ccCount; j++)
      v[j] *= powers[i-1][j];
    std::cout << v << '\n' << ct*v << '\n';
    powers.AddOnTop(v);
  }*/


/*
    std::cout << G.rho << std::endl;
    G.ComputeRhoOrbit();
    std::cout << "this orbit has " << G.rhoOrbit.size << std::endl;
    std::cout << G.rhoOrbit << std::endl;
    Vector<Rational> v = G.rhoOrbit[34];
    M = G.TodorsVectorToMatrix(v);
    std::cout << M*G.rho << " derp " << v << std::endl;
*/
//    for(int i=0;i<G.rhoOrbit.size;i++){
//    std::cout << G.TodorsVectorToMatrix(G.rhoOrbit[i]) << "\n\n";
//    }
//    std::cout << std::endl;
/*
    std::cout << v << G.DecomposeTodorsVector(v) << G.ComposeTodorsVector(G.DecomposeTodorsVector(v)) << std::endl;

    G.ComputeConjugacyClasses();
    std::cout << G.conjugacyClasses << std::endl;
    std::cout << G.ccSizes << std::endl;
    G.ComputeSquares();
    std::cout << G.squares << std::endl;
    G.ComputeInitialCharacters();
    std::cout << G.characterTable << std::endl;
    std::cout << G.characterTable[0].norm() << G.characterTable[1].norm() << G.characterTable[2].norm() << std::endl;

//    ElementMonomialAlgebra<FiniteGroupElement, Rational> a;
    Character X = G.characterTable[2] - G.characterTable[0];
    std::cout << X << X.norm() << std::endl;

    List<Vector<Rational> > l;
    for(int i=0;i<G.rootSystem.size;i++){
        bool usethis = true;
        for(int j=0; j<4; j++)
            if(G.rootSystem[i][j] < 0){
                usethis = false;
                break;
            }
        if(usethis == true)
            l.AddOnTop(G.rootSystem[i]);
    }
    std::cout << l << std::endl;
*/
/*
    std::string* S = [
    ""

    ]
*/
    std::string s;
    std::cin >> s;
    return 0;
}