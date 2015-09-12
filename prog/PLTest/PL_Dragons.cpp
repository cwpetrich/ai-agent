#include <ai_pl.h>
#include <string>
#include <cstdio>

/* This buffer is used only in the local file. */
static const int BUF_SIZE = 1024;
static char buf[BUF_SIZE];

class DragonsKnowledgeBase : public ai::PL::KnowledgeBase
{
public:
  DragonsKnowledgeBase();
  virtual ~DragonsKnowledgeBase();
  void TellDragonExists(const std::string &name);
  void TellDragonSays(const std::string &name,
		      const std::string &sentence);
  bool AskDragonRed(const std::string &name);
  bool AskDragonGray(const std::string &name);
  bool AskDragonRational(const std::string &name);
  bool AskDragonPredator(const std::string &name);
protected:
private:
};

DragonsKnowledgeBase::DragonsKnowledgeBase()
  : ai::PL::KnowledgeBase()
{
  // nothing here
}
DragonsKnowledgeBase::~DragonsKnowledgeBase()
{
  // nothing here
}
void DragonsKnowledgeBase::TellDragonExists(const std::string &name)
{
  // Truth teller rule.
  std::sprintf(buf, "TT_%s <=> ( (G_%s & R_%s) | (D_%s & P_%s) )",
	       name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str());
  AddSentence(buf);
  // Not a truth teller rule.
  std::sprintf(buf, "(!TT_%s) <=> ( (D_%s & R_%s) | (G_%s & P_%s) )",
	       name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str());
  AddSentence(buf);
  // Color rule.
  std::sprintf(buf, "G_%s <=> (!D_%s)",
	       name.c_str(), name.c_str());
  AddSentence(buf);
  // Type rule.
  std::sprintf(buf, "R_%s <=> (!P_%s)",
	       name.c_str(), name.c_str());
  AddSentence(buf);
}
void DragonsKnowledgeBase::TellDragonSays(const std::string &name,
					  const std::string &sentence)
{
  std::sprintf(buf, "(TT_%s) <=> (%s)",
	       name.c_str(), sentence.c_str());
  AddSentence(buf);
}

bool DragonsKnowledgeBase::AskDragonRed(const std::string &name)
{
  bool rval;
  ai::PL::KnowledgeBase question;
  std::sprintf(buf, "D_%s", name.c_str());
  question.AddSentence(buf);
  rval = (DPLL_Entails(question) == ai::PL::Symbol::KNOWN_TRUE);
  return rval;
}
bool DragonsKnowledgeBase::AskDragonGray(const std::string &name)
{
  bool rval;
  ai::PL::KnowledgeBase question;
  std::sprintf(buf, "G_%s", name.c_str());
  question.AddSentence(buf);
  rval = (DPLL_Entails(question) == ai::PL::Symbol::KNOWN_TRUE);
  return rval;
}
bool DragonsKnowledgeBase::AskDragonRational(const std::string &name)
{
  bool rval;
  ai::PL::KnowledgeBase question;
  std::sprintf(buf, "R_%s", name.c_str());
  question.AddSentence(buf);
  rval = (DPLL_Entails(question) == ai::PL::Symbol::KNOWN_TRUE);
  return rval;
}
bool DragonsKnowledgeBase::AskDragonPredator(const std::string &name)
{
  bool rval;
  ai::PL::KnowledgeBase question;
  std::sprintf(buf, "P_%s", name.c_str());
  question.AddSentence(buf);
  rval = (DPLL_Entails(question) == ai::PL::Symbol::KNOWN_TRUE);
  return rval;
}

void test_problem1()
{
  /* This problem comes from "Logic Puzzles" by Fernandez, Summers, and Willis
   *
   * A knight approaches two dragons and inquires as to the color and
   * type of each.  Gray rationals and red predators always tell the
   * truth;  red rationals and gray predators always lie.  The dragons
   * respond below:
   *
   * A. 1. I am gray.
   *    2. B is a predator.
   * B  1. A is a predator.
   *    2. I am a rational.
   */
  /* We translate this problem into the following propositional logic:
     TT_A <=> ( (G_A & R_A) | (D_A & P_A) )
     (!TT_A) <=> ( (D_A & R_A) | (G_A & P_A) )
     G_A <=> (!D_A)
     R_A <=> (!P_A)
     TT_B <=> ( (G_B & R_B) | (D_B & P_B) )
     (!TT_B) <=> ( (D_B & R_B) | (G_B & P_B) )
     G_B <=> (!D_B)
     R_B <=> (!P_B)
     TT_A <=> G_A
     TT_A <=> P_B
     TT_B <=> P_A
     TT_B <=> R_B

     # Semantics of symbols.
     TT_x: x is a Truth Teller
     G_x: x is Gray
     D_x: x is reD
     R_x: x is Rational
     P_x: x is Predator
  */
  DragonsKnowledgeBase kb;
  kb.TellDragonExists("A");
  kb.TellDragonExists("B");
  kb.TellDragonSays("A", "G_A"); // I am gray, (A is gray), G_A
  kb.TellDragonSays("A", "P_B"); // B is a predator, P_B
  kb.TellDragonSays("B", "P_A"); // A is a predator, P_A
  kb.TellDragonSays("B", "R_B"); // I am a rational, (B is a rational), R_B

  std::string names[2] = { "A", "B" };
  for(int i = 0; i < 2; i ++)
    {
      if( kb.AskDragonGray(names[i]) && kb.AskDragonRational(names[i]) )
	{
	  std::cout << names[i] << " is a Gray Rational." << std::endl;
	}
      if( kb.AskDragonRed(names[i]) && kb.AskDragonPredator(names[i]) )
	{
	  std::cout << names[i] << " is a Red Predator." << std::endl;
	}
      if( kb.AskDragonRed(names[i]) && kb.AskDragonRational(names[i]) )
	{
	  std::cout << names[i] << " is a Red Rational." << std::endl;
	}
      if( kb.AskDragonGray(names[i]) && kb.AskDragonPredator(names[i]) )
	{
	  std::cout << names[i] << " is a Gray Predator." << std::endl;
	}
    }
}

int main(int argc, char **argv)
{
  test_problem1();
  return 0;
}
  
