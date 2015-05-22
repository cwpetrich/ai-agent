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
  /* tell ((G_A & R_A) | ((!G_A) & (!R_A))) <=> T_A
   * tell ((G_A & (!R_A)) | ((!G_A) & R_A)) <=> (!T_A) */

  // Truth teller rule.
  std::sprintf(buf, "((G_%s & R_%s) | ((!G_%s) & (!R_%s))) <=> T_%s",
               name.c_str(), name.c_str(), name.c_str(), name.c_str(),
	       name.c_str());
  AddSentence(buf);
  // Not a truth teller rule.
  std::sprintf(buf, "((G_%s & (!R_%s)) | ((!G_%s) & R_%s)) <=> (!T_%s)",
               name.c_str(), name.c_str(), name.c_str(), name.c_str(),
	       name.c_str());
  AddSentence(buf);
}
void DragonsKnowledgeBase::TellDragonSays(const std::string &name,
                                          const std::string &sentence)
{
  /* tell T_A <=> (!R_B) */
  std::sprintf(buf, "(T_%s) <=> (%s)",
               name.c_str(), sentence.c_str());
  AddSentence(buf);
}

bool DragonsKnowledgeBase::AskDragonRed(const std::string &name)
{
  bool rval;
  ai::PL::KnowledgeBase question;
  std::sprintf(buf, "!G_%s", name.c_str());
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
  std::sprintf(buf, "!R_%s", name.c_str());
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
     ((G_A & R_A) | ((!G_A) & (!R_A))) <=> T_A
     ((G_A & (!R_A)) | ((!G_A) & R_A)) <=> (!T_A)
     ((G_B & R_B) | ((!G_B) & (!R_B))) <=> T_B
     ((G_B & (!R_B)) | ((!G_B) & R_B)) <=> (!T_B)

     T_A <=> G_A
     T_A <=> (!R_B)
     
     T_B <=> (!R_A)
     T_B <=> R_B

     # Semantics of symbols.
     T_x: x is a Truth teller
     G_x: x is Gray
     !G_x: x is red
     R_x: x is Rational
     !R_x: x is predator
  */

  // new knowledge base
  DragonsKnowledgeBase kb;
  // tell physics
  kb.TellDragonExists("A");
  kb.TellDragonExists("B");
  // tell instance information
  kb.TellDragonSays("A", "G_A");    // I am gray, (A is gray), G_A
  kb.TellDragonSays("A", "(!R_B)"); // B is a predator, !R_B
  kb.TellDragonSays("B", "(!R_A)"); // A is a predator, !R_A
  kb.TellDragonSays("B", "R_B");    // I am a rational, (B is a rational), R_B

  // ask questions
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
