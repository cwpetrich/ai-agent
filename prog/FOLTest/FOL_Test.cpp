#include <iostream>
#include <iomanip>
#include "ai_fol.h"

#define DO_CNF 1
#define DO_SYMBOL 0
#define DO_SENTENCE 0
#define DO_PARSE 0

#if DO_SYMBOL
void symbol_test()
{
  ai::FOL::Symbol s("B11");

  std::cout << s << std::endl;

  s.SetType(ai::FOL::Symbol::TYPE_CONSTANT);
  
  std::cout << s << std::endl;
  
  s.SetType(ai::FOL::Symbol::TYPE_VARIABLE);
  
  std::cout << s << std::endl;
  
  s.SetType(ai::FOL::Symbol::TYPE_PREDICATE);
  s.SetArity(2);
  
  std::cout << s << std::endl;

  s.SetType(ai::FOL::Symbol::TYPE_FUNCTION);
  s.SetArity(3);
  
  std::cout << s << std::endl;
} 
#endif

#if DO_SENTENCE
void sentence_test()
{
  ai::FOL::Symbol *P = new ai::FOL::Symbol("P");
  ai::FOL::Symbol *Q = new ai::FOL::Symbol("Q");
  ai::FOL::Symbol *R = new ai::FOL::Symbol("R");
  ai::FOL::Symbol *S = new ai::FOL::Symbol("S");

  ai::FOL::Sentence *Ps = new ai::FOL::Sentence(P);
  ai::FOL::Sentence *Qs = new ai::FOL::Sentence(Q);
  ai::FOL::Sentence *Rs = new ai::FOL::Sentence(R);
  ai::FOL::Sentence *Ss = new ai::FOL::Sentence(S);

  ai::FOL::Sentence *QRs = new ai::FOL::Sentence(Qs, ai::FOL::Sentence::O_AND, Rs);
  ai::FOL::Sentence *NPs = new ai::FOL::Sentence(ai::FOL::Sentence::O_NOT, Ps);
  ai::FOL::Sentence *ORs = new ai::FOL::Sentence(NPs, ai::FOL::Sentence::O_OR, QRs);
  
  ai::FOL::Sentence *Finals = new ai::FOL::Sentence(ORs, ai::FOL::Sentence::O_IMPLIES, Ss);

  ai::FOL::Sentence *Cs     = new ai::FOL::Sentence(ai::FOL::Sentence::T_COMMA);
  ai::FOL::Sentence *FAs    = new ai::FOL::Sentence(ai::FOL::Sentence::T_FORALL);
  ai::FOL::Sentence *Es     = new ai::FOL::Sentence(ai::FOL::Sentence::T_EXISTS);

  std::cout << *Finals << std::endl;
  std::cout << *Cs << std::endl;
  std::cout << *FAs << std::endl;
  std::cout << *Es << std::endl;

  delete Cs;
  delete FAs;
  delete Es;
  delete Finals;
  delete P;
  delete Q;
  delete R;
  delete S;
  
}
#endif

#if DO_PARSE
void parse_test()
{
  if(1)
    {
      ai::FOL::KnowledgeBase kb;
      //  kb.AddSentence("((!P) | (Q & R)) => S");
      kb.AddSentence("Happy(A)");
      kb.AddSentence("Happy(A,B,C)");
      kb.AddSentence("Happy(A,Sad(B),C)");
      kb.AddSentence("Other(A,Sad(Sad(B)),C)");
      kb.AddSentence("FailBecauseNoParen(x) => Other(y)");
      kb.AddSentence("(Some(x) => Other(y))");
      kb.AddSentence("(( X(A) & Y(A) ) => ( W(A) | Z(B) ))");

      std::cout << std::endl << kb << std::endl;
      //kb.AddSentence("((!P_11s2) | (Q & R)) => S");
    }

  if(1)
    {
      ai::FOL::KnowledgeBase kb;
      kb.AddSentence("man(Marcus)");
      kb.AddSentence("Pompeian(Marcus)");
      kb.AddSentence("forall x (Pompeian(x) => Roman(x))");
      kb.AddSentence("ruler(Caesar)");
      kb.AddSentence("forall x Roman(x) => loyalto(x, Caesar) | hate(x,Caesar)");
      kb.AddSentence("forall x ( Roman(x) => ( loyalto(x, Caesar) | hate(x,Caesar) ) )");
      kb.AddSentence("forall x ( exists y loyalto(x,y) )");
      kb.AddSentence("forall x ( forall y ( ( (person(x) & ruler(y)) & tryassassinate(x,y)) => (! loyalto(x,y)) ) )");
      kb.AddSentence("tryassassinate(Marcus, Caesar)");
      kb.AddSentence("forall x man(x) => person(x)");
      kb.AddSentence("forall x ( man(x) => person(x) )");
      
      std::cout << std::endl << kb << std::endl;
    }
  
  if(1)
    {
      ai::FOL::KnowledgeBase kb;
      kb.AddSentence("(((foo(x) & bar(x)) | fred(x)) => (!barney(x)))");
      kb.AddSentence("wilma = betty");
      kb.AddSentence("(wilma(x, y, z) = betty)");
      
      std::cout << std::endl << kb << std::endl;
    }
  
}
#endif

#if 0
void kb_test()
{
  ai::PL::KnowledgeBase kb;
  kb.AddSentence("! P_1_1");
  kb.AddSentence("B_1_1 <=> (P_1_2 | P_2_1)");
  kb.AddSentence("B_2_1 <=> (P_1_1 | P_2_2 | P_3_1)");
  kb.AddSentence("! B_1_1");
  kb.AddSentence("B_2_1");
  
  std::cout << kb << std::endl;

  std::cout << "Truth: " << kb.Evaluate() << std::endl;
}
#endif

#if 0
void model_test()
{
  ai::PL::KnowledgeBase kb;
  kb.AddSentence("! P_1_1");
  kb.AddSentence("B_1_1 <=> (P_1_2 | P_2_1)");
  kb.AddSentence("B_2_1 <=> (P_1_1 | P_2_2 | P_3_1)");
  kb.AddSentence("! B_1_1");
  kb.AddSentence("B_2_1");

  std::vector<ai::PL::Symbol *> symbols;
  symbols.push_back(new ai::PL::Symbol("P_1_1"));
  symbols[symbols.size()-1]->SetValue(ai::PL::Symbol::KNOWN_FALSE);
  symbols.push_back(new ai::PL::Symbol("B_1_1"));
  symbols[symbols.size()-1]->SetValue(ai::PL::Symbol::KNOWN_FALSE);
  symbols.push_back(new ai::PL::Symbol("B_2_1"));
  symbols[symbols.size()-1]->SetValue(ai::PL::Symbol::KNOWN_TRUE);
  symbols.push_back(new ai::PL::Symbol("P_1_2"));
  symbols[symbols.size()-1]->SetValue(ai::PL::Symbol::KNOWN_FALSE);
  symbols.push_back(new ai::PL::Symbol("P_2_1"));
  symbols[symbols.size()-1]->SetValue(ai::PL::Symbol::KNOWN_FALSE);
  symbols.push_back(new ai::PL::Symbol("P_2_2"));
  symbols[symbols.size()-1]->SetValue(ai::PL::Symbol::KNOWN_FALSE);
  symbols.push_back(new ai::PL::Symbol("P_3_1"));
  symbols[symbols.size()-1]->SetValue(ai::PL::Symbol::KNOWN_FALSE);

  kb.SetModel(symbols);
  
  std::cout << kb << std::endl;

  std::cout << "Truth: " << kb.Evaluate() << std::endl;
}
#endif

#if 0
static int count = 0;

void model_test_2_aux(ai::PL::KnowledgeBase &kb,
		      std::vector<ai::PL::Symbol *> &symbols,
		      unsigned int pos)
{
  if(pos >= symbols.size())
    {
      kb.SetModel(symbols);
      
      ai::PL::Symbol::State rval = kb.Evaluate();

      if(rval == ai::PL::Symbol::KNOWN_TRUE)
	{
	  std::cout << kb << std::endl;
	  std::cout << "Truth: " << rval << std::endl;
	}
      return;
    }
  count ++;
  
  ai::PL::Symbol::State p;
  for(p = ai::PL::Symbol::KNOWN_TRUE; p <= ai::PL::Symbol::KNOWN_FALSE; p++)
    {
      symbols[pos]->SetValue(p);
      model_test_2_aux(kb, symbols, pos + 1);
    }
}
#endif
#if 0
void model_test_2()
{
  ai::PL::KnowledgeBase kb;
  kb.AddSentence("! P_1_1");
  kb.AddSentence("B_1_1 <=> (P_1_2 | P_2_1)");
  kb.AddSentence("B_2_1 <=> (P_1_1 | P_2_2 | P_3_1)");
  kb.AddSentence("! B_1_1");
  kb.AddSentence("B_2_1");

  std::vector<ai::PL::Symbol *> symbols;
  symbols.push_back(new ai::PL::Symbol("P_1_1"));
  symbols.push_back(new ai::PL::Symbol("B_1_1"));
  symbols.push_back(new ai::PL::Symbol("B_2_1"));
  symbols.push_back(new ai::PL::Symbol("P_1_2"));
  symbols.push_back(new ai::PL::Symbol("P_2_1"));
  symbols.push_back(new ai::PL::Symbol("P_2_2"));
  symbols.push_back(new ai::PL::Symbol("P_3_1"));

  std::cout << std::endl
	    << std::endl
	    << "Starting model enumeration"
	    << std::endl
	    << std::endl;
  count = 0;
  model_test_2_aux(kb, symbols, 0);
  std::cout << std::endl
	    << count << std::endl
	    << std::endl
	    << "Ending model enumeration"
	    << std::endl
	    << std::endl;
  
}
#endif

#if 0
void model_test_3_aux(ai::PL::KnowledgeBase &kb,
		      std::vector<ai::PL::Symbol *> &symbols,
		      unsigned int pos)
{
  kb.SetModel(symbols);
  
  ai::PL::Symbol::State rval = kb.Evaluate();

  if(rval == ai::PL::Symbol::KNOWN_TRUE)
    {
      std::cout << kb << std::endl;
      std::cout << "Truth: " << rval << std::endl;
      return;
    }
  else if(rval == ai::PL::Symbol::KNOWN_FALSE)
    {
      return;
    }
  // still unknown
  count ++;
  
  ai::PL::Symbol::State p;
  for(p = ai::PL::Symbol::KNOWN_TRUE; p <= ai::PL::Symbol::KNOWN_FALSE; p++)
    {
      symbols[pos]->SetValue(p);
      model_test_3_aux(kb, symbols, pos + 1);
    }
}

void model_test_3()
{
  ai::PL::KnowledgeBase kb;
  kb.AddSentence("! P_1_1");
  kb.AddSentence("B_1_1 <=> (P_1_2 | P_2_1)");
  kb.AddSentence("B_2_1 <=> (P_1_1 | P_2_2 | P_3_1)");
  kb.AddSentence("! B_1_1");
  kb.AddSentence("B_2_1");

  std::vector<ai::PL::Symbol *> symbols;
  symbols.push_back(new ai::PL::Symbol("P_1_1"));
  symbols.push_back(new ai::PL::Symbol("B_1_1"));
  symbols.push_back(new ai::PL::Symbol("B_2_1"));
  symbols.push_back(new ai::PL::Symbol("P_1_2"));
  symbols.push_back(new ai::PL::Symbol("P_2_1"));
  symbols.push_back(new ai::PL::Symbol("P_2_2"));
  symbols.push_back(new ai::PL::Symbol("P_3_1"));

  std::cout << std::endl
	    << std::endl
	    << "Starting model enumeration"
	    << std::endl
	    << std::endl;
  count = 0;
  model_test_3_aux(kb, symbols, 0);
  std::cout << std::endl
	    << count << std::endl
	    << std::endl
	    << "Ending model enumeration"
	    << std::endl
	    << std::endl;
  
}
#endif

#if 0
void tt_entails_test_aux(ai::PL::KnowledgeBase &kb,
			 ai::PL::KnowledgeBase &alpha)
{
  ai::PL::Symbol::State rval;

  rval = kb.TT_Entails(alpha);
  
  std::cout << "KB" << std::endl << kb 
	    << "alpha" << std::endl << alpha
	    << "result: " << rval
	    << std::endl
	    << std::endl;
}
  
void tt_entails_test()
{
  ai::PL::KnowledgeBase kb;
  kb.AddSentence("! P_1_1");
  kb.AddSentence("B_1_1 <=> (P_1_2 | P_2_1)");
  kb.AddSentence("B_2_1 <=> (P_1_1 | P_2_2 | P_3_1)");
  kb.AddSentence("! B_1_1");
  kb.AddSentence("B_2_1");

  ai::PL::KnowledgeBase alpha;
  alpha.AddSentence("! P_1_1");
  tt_entails_test_aux(kb, alpha);
  
  ai::PL::KnowledgeBase alpha_1;
  alpha_1.AddSentence("!P_1_2");
  tt_entails_test_aux(kb, alpha_1);

  ai::PL::KnowledgeBase alpha_2;
  alpha_2.AddSentence("P_1_2");
  tt_entails_test_aux(kb, alpha_2);
  
  ai::PL::KnowledgeBase alpha_3;
  alpha_3.AddSentence("!P_2_2");
  tt_entails_test_aux(kb, alpha_3);
  
  ai::PL::KnowledgeBase alpha_4;
  alpha_4.AddSentence("P_2_2");
  tt_entails_test_aux(kb, alpha_4);
  /*
  */
  
}
#endif

#if 0
void tt_entails_test_2_aux(ai::PL::KnowledgeBase &kb,
			   ai::PL::KnowledgeBase &alpha)
{
  ai::PL::Symbol::State rval;

  rval = kb.TT_Entails_Pruned(alpha);
  
  std::cout << "KB" << std::endl << kb 
	    << "alpha" << std::endl << alpha
	    << "result: " << rval
	    << std::endl
	    << std::endl;
}
  
void tt_entails_test_2()
{
  ai::PL::KnowledgeBase kb;
  kb.AddSentence("! P_1_1");
  kb.AddSentence("B_1_1 <=> (P_1_2 | P_2_1)");
  kb.AddSentence("B_2_1 <=> (P_1_1 | P_2_2 | P_3_1)");
  kb.AddSentence("! B_1_1");
  kb.AddSentence("B_2_1");

  ai::PL::KnowledgeBase alpha;
  alpha.AddSentence("! P_1_1");
  tt_entails_test_2_aux(kb, alpha);
  
  ai::PL::KnowledgeBase alpha_1;
  alpha_1.AddSentence("!P_1_2");
  tt_entails_test_2_aux(kb, alpha_1);

  ai::PL::KnowledgeBase alpha_2;
  alpha_2.AddSentence("P_1_2");
  tt_entails_test_2_aux(kb, alpha_2);
  
  ai::PL::KnowledgeBase alpha_3;
  alpha_3.AddSentence("!P_2_2");
  tt_entails_test_2_aux(kb, alpha_3);
  
  ai::PL::KnowledgeBase alpha_4;
  alpha_4.AddSentence("P_2_2");
  tt_entails_test_2_aux(kb, alpha_4);
  /*
  */
  
}
#endif

#if DO_CNF
void cnf_test()
{
  std::string sentences[] =
    {
      "X(a) <=> Y(b)",
      "X(a) <=> (Y(b) & (A(x) <=> B(y)))",
      "(!(!X(a)))",
      "(!(!(!(!(!(!X(a)))))))",
      "(!(!(!(!(!(!(!X(a))))))))",
      "(!((((((X(a))))))))",
      "forall x ( forall y Animal(y) => Loves(x,y) ) => ( exists y Loves(y,x) )",
      "! ( X(a) & Y(b) )",
      "! ( X(a) | Y(b) )",
      "! ( ( ( X(a) | Y(b) ) ) )",
      "X(A)"
    };
  for(unsigned int i = 0; i < sizeof(sentences)/sizeof(sentences[0]); i ++)
    {
      ai::FOL::KnowledgeBase kb;
      kb.AddSentence(sentences[i]);
      const ai::FOL::Sentence * s = kb.GetSentences()[0];
      
      ai::FOL::Sentence *my_s = new ai::FOL::Sentence(*s);
      std::cout << (*my_s) << std::endl;
      
      std::vector<ai::FOL::Sentence *> clauses;
      my_s->GetCNF(clauses);
      
      for(unsigned int i = 0; i < clauses.size(); i++)
	{
	  std::cout << (*(clauses[i])) << std::endl;
	  delete clauses[i];
	  clauses[i] = 0;
	}
      std::cout << std::endl;
    }

}
#endif

int main()
{
#if DO_SYMBOL
  symbol_test();
#endif
#if DO_SENTENCE
  sentence_test();
#endif
#if DO_PARSE
  parse_test();
#endif
#if DO_CNF
  cnf_test();
#endif
  /*
  kb_test();
  
  model_test();
  model_test_2();
  model_test_3();

  tt_entails_test();
  
  tt_entails_test_2();
  cnf_test();
  */
  
  return 0;
}
