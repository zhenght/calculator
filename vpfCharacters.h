#ifndef vpfCharacterHeader
#define vpfCharacterHeader
#include "vpf.h"

static ProjectInformationInstance ProjectInfoVpfCharactersHEader
(__FILE__, "Header file, experimental code by Thomas: finite group characters sandbox. Not fully implemented yet.");

class Character;

class FiniteGroup{
    public:
    int N;
    int ccCount;
    List<int> ccSizes;
    List<int> squares;
    List<Character> characterTable;
};

class Character{
    public:
    FiniteGroup G;
    List<int> data;

    Character(){}

    int IP(const Character &other) const;
    int norm() const;
    Character operator*(const Character &other) const;
    Character Sym2() const;
    Character Alt2() const;
    Character operator+(const Character &other) const;
    Character operator-(const Character &other) const;
    Character ReducedWithChars(const List<Character> chars = 0);
    int& operator[](int i) const;
    void operator=(const Character& X);
    std::string ToString(FormatExpressions* theFormat) const;
    std::string ToString() const;
    static unsigned int HashFunction(const Character& input);
    bool operator==(const Character& other) const;
};
std::ostream& operator<<(std::ostream& out, const Character X);

class CoxeterGroup: public FiniteGroup{
    public:
    CoxeterGroup(){
      this->nGens=-1;
    }
    CoxeterGroup(const DynkinType& D){
      this->MakeFrom(D);
    }
    void MakeFrom(const DynkinType& D);
//    void MakeFromDynkinType(DynkinType D);
    Matrix<Rational> CartanSymmetric;
    HashedList<Vector<Rational> > rootSystem;
    Vector<Rational> rho;

    // this part needs to be revisited before we try E8
    HashedList<Vector<Rational> > rhoOrbit;
    List<List<int> > conjugacyClasses;

    int nGens;
    std::string ToString(FormatExpressions* theFormat=0)const
    { std::stringstream out;
      out << "this: " << this << this->CartanSymmetric.ToString(theFormat);
      return out.str();
    }
    bool operator==(const CoxeterGroup& other)const
    { return this->CartanSymmetric==other.CartanSymmetric;
    }
    void operator=(const CoxeterGroup& other)
    { this->CartanSymmetric=other.CartanSymmetric;
      this->rootSystem=other.rootSystem;
      this->rho = other.rho;
      this->rhoOrbit=other.rhoOrbit;
      this->conjugacyClasses=other.conjugacyClasses;
      this->nGens=other.nGens;
      std::cout << "operator=; rho=" << this->rho << "\n" << std::endl;
    }
    Vector<Rational> SimpleReflection(int i, const Vector<Rational> &right) const;
    HashedList<Vector<Rational> > GetOrbit(const Vector<Rational> &v) const;
    void ComputeRhoOrbit();
    Matrix<Rational> TodorsVectorToInvMatrix(const Vector<Rational> &v) const;
    Matrix<Rational> TodorsVectorToMatrix(const Vector<Rational> &v) const;
    // note: the list is backwards
    List<int> DecomposeTodorsVector(const Vector<Rational> &v) const;
    Vector<Rational> ComposeTodorsVector(const List<int> &l) const;
    Vector<Rational> ApplyList(const List<int> &l, const Vector<Rational> &v) const;
    Vector<Rational> SimpleConjugation(int i, const Vector<Rational> &v) const;
    void ComputeConjugacyClasses();
    void ComputeSquares();
    void ComputeInitialCharacters();
};

class CoxeterElement{
  public:
  List<int> reflections;
  CoxeterGroup* owner;
  CoxeterElement():owner(0){}
  CoxeterElement(CoxeterGroup* owner, List<int> reflections){
    this->owner = owner; this->reflections = reflections;
  }
  void canonicalize();
  static unsigned int HashFunction(const CoxeterElement& input);
  std::string ToString(FormatExpressions* theFormat=0);
  void operator*=(const CoxeterElement& other);
  void operator=(const CoxeterElement& other);
  bool operator==(const CoxeterElement& other) const;
};

#endif
