#include <iostream>
#include <iomanip>
#include "ai_pl.h"


void symbol_test()
{
  ai::PL::Symbol s("B11");

  std::cout << s << std::endl;
  
  s.SetValue(ai::PL::Symbol::KNOWN_FALSE);
  
  std::cout << s << std::endl;
  
  s.SetValue(ai::PL::Symbol::KNOWN_TRUE);
  
  std::cout << s << std::endl;
  
  s.SetValue(ai::PL::Symbol::UNKNOWN);
  
  std::cout << s << std::endl;
} 

void sentence_test()
{
  ai::PL::Symbol *P = new ai::PL::Symbol("P");
  ai::PL::Symbol *Q = new ai::PL::Symbol("Q");
  ai::PL::Symbol *R = new ai::PL::Symbol("R");
  ai::PL::Symbol *S = new ai::PL::Symbol("S");

  ai::PL::Sentence *Ps = new ai::PL::Sentence(P);
  ai::PL::Sentence *Qs = new ai::PL::Sentence(Q);
  ai::PL::Sentence *Rs = new ai::PL::Sentence(R);
  ai::PL::Sentence *Ss = new ai::PL::Sentence(S);

  ai::PL::Sentence *QRs = new ai::PL::Sentence(Qs, ai::PL::Sentence::O_AND, Rs);
  ai::PL::Sentence *NPs = new ai::PL::Sentence(ai::PL::Sentence::O_NOT, Ps);
  ai::PL::Sentence *ORs = new ai::PL::Sentence(NPs, ai::PL::Sentence::O_OR, QRs);
  
  ai::PL::Sentence *Finals = new ai::PL::Sentence(ORs, ai::PL::Sentence::O_IMPLIES, Ss);

  std::cout << *Finals << std::endl;

  ai::PL::Symbol::State p,q,r,s;
  ai::PL::Symbol::State result;
  int error = 0;
  std::string msg = "";
  
  for(p = ai::PL::Symbol::KNOWN_TRUE; p < ai::PL::Symbol::STATE_MAX; p++)
    {
      P->SetValue(p);
      result = Ps->Evaluate();
      if(p != result)
	{
	  error ++;
	  std::cout << "***"
		    << (*P)
		    << " p=" << p
		    << " Ps=" << (*Ps)
		    << " result=" << result
		    << std::endl;
	}
    }
  if(error)
    {
      return;
    }
  
  for(q = ai::PL::Symbol::KNOWN_TRUE; q < ai::PL::Symbol::STATE_MAX; q++)
    for(r = ai::PL::Symbol::KNOWN_TRUE; r < ai::PL::Symbol::STATE_MAX; r++)
      {
	Q->SetValue(q);
	R->SetValue(r);
	result = QRs->Evaluate();
	if((q == r) && (q == ai::PL::Symbol::KNOWN_TRUE))
	  { // both true
	    if(result != ai::PL::Symbol::KNOWN_TRUE)
	      {
		msg += "should be true";
		error ++;
	      }
	  }
	else if((q == ai::PL::Symbol::KNOWN_FALSE) || (r == ai::PL::Symbol::KNOWN_FALSE))
	  { // either or both false
	    if(result != ai::PL::Symbol::KNOWN_FALSE)
	      {
		msg += "should be false";
		error ++;
	      }
	  }
	else if((q == ai::PL::Symbol::UNKNOWN && r == ai::PL::Symbol::KNOWN_TRUE) ||
		(r == ai::PL::Symbol::UNKNOWN && q == ai::PL::Symbol::KNOWN_TRUE))
	  { // one unknown, other true
	    if(result != ai::PL::Symbol::UNKNOWN)
	      {
		msg += "should be unknown";
		error ++;
	      }
	  }
	else if(q == ai::PL::Symbol::UNKNOWN && r == ai::PL::Symbol::UNKNOWN)
	  { // both unknown
	    if(result != ai::PL::Symbol::UNKNOWN)
	      {
		msg += "should be unknown";
		error ++;
	      }
	  }
	else
	  {
	    msg += "uncaught case";
	    error ++;
	  }
	if(error)
	  {
	    std::cout << (*Q)
		      << " q=" << q
		      << " Qs=" << (*Qs)
		      << std::endl
		      << (*R)
		      << " r=" << r
		      << " Rs=" << (*Rs)
		      << std::endl
		      << " result=" << result
		      << " QRs=" << (*QRs)
		      << std::endl
		      << msg 
		      << std::endl;
	    return;
	  }
      }
  
  for(p = ai::PL::Symbol::KNOWN_TRUE; p < ai::PL::Symbol::STATE_MAX; p++)
    for(q = ai::PL::Symbol::KNOWN_TRUE; q < ai::PL::Symbol::STATE_MAX; q++)
      for(r = ai::PL::Symbol::KNOWN_TRUE; r < ai::PL::Symbol::STATE_MAX; r++)
	{
	  P->SetValue(p);
	  Q->SetValue(q);
	  R->SetValue(r);
	  result = ORs->Evaluate();

	  if(NPs->Evaluate() == ai::PL::Symbol::KNOWN_TRUE)
	    { 
	      if(result != ai::PL::Symbol::KNOWN_TRUE)
		{
		  msg += "should be true !p == true";
		  error ++;
		}
	    }
	  else if(QRs->Evaluate() == ai::PL::Symbol::KNOWN_TRUE)
	    {
	      if(result != ai::PL::Symbol::KNOWN_TRUE)
		{
		  msg += "should be true q&r == true";
		  error ++;
		}
	    }
	  else if((NPs->Evaluate() == ai::PL::Symbol::KNOWN_FALSE) &&
		  (QRs->Evaluate() == ai::PL::Symbol::KNOWN_FALSE))
	    {
	      if(result != ai::PL::Symbol::KNOWN_FALSE)
		{
		  msg += "should be false (!p == false) & (q&r == false)";
		  error ++;
		}
	    }
	  else if(((NPs->Evaluate() == ai::PL::Symbol::KNOWN_FALSE) &&
		   (QRs->Evaluate() == ai::PL::Symbol::UNKNOWN)) ||
		  ((NPs->Evaluate() == ai::PL::Symbol::UNKNOWN) &&
		   (QRs->Evaluate() == ai::PL::Symbol::KNOWN_FALSE)))

	    { 
	      if(result != ai::PL::Symbol::UNKNOWN)
		{
		  msg += "should be unknown (one false)";
		  error ++;
		}
	    }
	  else if((NPs->Evaluate() == ai::PL::Symbol::UNKNOWN) &&
		  (QRs->Evaluate() == ai::PL::Symbol::UNKNOWN))
	    { 
	      if(result != ai::PL::Symbol::UNKNOWN)
		{
		  msg += "should be unknown (both unknown)";
		  error ++;
		}
	    }
	  else
	    {
	      msg += "uncaught case";
	      error ++;
	    }

	  if(error)
	    {
	      std::cout << (*P)
			<< " p=" << p
			<< " Ps=" << (*Ps)
			<< std::endl
			<< " NPs=" << (*NPs)
			<< std::endl
			<< (*Q)
			<< " q=" << q
			<< " Qs=" << (*Qs)
			<< std::endl
			<< (*R)
			<< " r=" << r
			<< " Rs=" << (*Rs)
			<< std::endl
			<< " QRs=" << (*QRs)
			<< std::endl
			<< " result=" << result
			<< " ORs=" << (*ORs)
			<< std::endl
			<< msg 
			<< std::endl;
	      return;
	    }
	}
  
  
  for(p = ai::PL::Symbol::KNOWN_TRUE; p < ai::PL::Symbol::STATE_MAX; p++)
    for(q = ai::PL::Symbol::KNOWN_TRUE; q < ai::PL::Symbol::STATE_MAX; q++)
      for(r = ai::PL::Symbol::KNOWN_TRUE; r < ai::PL::Symbol::STATE_MAX; r++)
	for(s = ai::PL::Symbol::KNOWN_TRUE; s < ai::PL::Symbol::STATE_MAX; s++)
	  {
	    P->SetValue(p);
	    Q->SetValue(q);
	    R->SetValue(r);
	    S->SetValue(s);
	    result = Finals->Evaluate();

	    if(ORs->Evaluate() == ai::PL::Symbol::KNOWN_FALSE)
	      {
		if(result != ai::PL::Symbol::KNOWN_TRUE)
		  {
		    msg += "should be true (LHS is false)";
		    error ++;
		  }
	      }
	    else if((ORs->Evaluate() == ai::PL::Symbol::KNOWN_TRUE) &&
		    (Ss->Evaluate() == ai::PL::Symbol::KNOWN_FALSE))
	      {
		if(result != ai::PL::Symbol::KNOWN_FALSE)
		  {
		    msg += "should be false (RHS is false)";
		    error ++;
		  }
	      }
	    else if((ORs->Evaluate() == ai::PL::Symbol::KNOWN_TRUE) &&
		    (Ss->Evaluate() == ai::PL::Symbol::KNOWN_TRUE))
	      {
		if(result != ai::PL::Symbol::KNOWN_TRUE)
		  {
		    msg += "should be true (RHS is true)";
		    error ++;
		  }
	      }
	    else if(ORs->Evaluate() == ai::PL::Symbol::UNKNOWN)
	      {
		if(result != ai::PL::Symbol::UNKNOWN)
		  {
		    msg += "should be unknown (LHS is unknown)";
		    error ++;
		  }
	      }
	    else if((ORs->Evaluate() == ai::PL::Symbol::KNOWN_TRUE) &&
		    (Ss->Evaluate() == ai::PL::Symbol::UNKNOWN))
	      {
		if(result != ai::PL::Symbol::UNKNOWN)
		  {
		    msg += "should be unknown (RHS is unknown)";
		    error ++;
		  }
	      }
	    else
	      {
		msg += "uncaught case";
		error ++;
	      }
	    if(error)
	      {
		std::cout << (*P)
			  << " p=" << p
			  << " Ps=" << (*Ps)
			  << std::endl
			  << " NPs=" << (*NPs)
			  << std::endl
			  << (*Q)
			  << " q=" << q
			  << " Qs=" << (*Qs)
			  << std::endl
			  << (*R)
			  << " r=" << r
			  << " Rs=" << (*Rs)
			  << std::endl
			  << " QRs=" << (*QRs)
			  << std::endl
			  << " ORs=" << (*ORs)
			  << std::endl
			  << (*S)
			  << " s=" << s
			  << " Ss=" << (*Ss)
			  << std::endl
			  << " result=" << result
			  << " Finals=" << (*Finals)
			  << std::endl
			  << msg 
			  << std::endl;
	      return;
	      }
	  }
  
  delete Finals;
  delete P;
  delete Q;
  delete R;
  delete S;
  
}

void parse_test()
{
  ai::PL::KnowledgeBase kb;
  kb.AddSentence("((!P) | (Q & R)) => S");
  //kb.AddSentence("((!P_11s2) | (Q & R)) => S");
}

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

void cnf_test()
{
  ai::PL::KnowledgeBase kb;
  kb.AddSentence("B_2_2 <=> (P_1_2 | P_2_1 | P_2_3 | P_3_2 )");
  //kb.AddSentence("B11 <=> (P12 | P21)");
  //kb.AddSentence("B11 <=> ((!P12) | P21)");
  const ai::PL::Sentence * s = kb.GetSentences()[0];

  ai::PL::Sentence *my_s = new ai::PL::Sentence(*s);

  std::vector<ai::PL::Sentence *> clauses;
  my_s->CNF();
  ai::PL::Sentence::SplitClauses(my_s, clauses);

  for(unsigned int i = 0; i < clauses.size(); i++)
    {
      std::cout << (*(clauses[i])) << std::endl;
    }

}


int main()
{
  /*
  symbol_test();
  sentence_test();
  parse_test();
  kb_test();
  
  model_test();
  model_test_2();
  model_test_3();

  tt_entails_test();
  
  tt_entails_test_2();
  */
  cnf_test();
  
  return 0;
}
