#include <iostream>
#include <fstream>
#include <iomanip>
#include <unistd.h>
#include <cstring>
#include "Agent/FileDescriptor.h"
#include "ai_pl.h"

#define BUF_SIZE 1024
int process_file(ai::PL::KnowledgeBase &kb, char *buf, std::istream &in, std::ostream &out);

int do_ask(ai::PL::KnowledgeBase &kb, int interactive, char *buf, std::istream &in, std::ostream &out, const char *prompt)
{
  ai::PL::Symbol::State rval;
  ai::PL::KnowledgeBase question;
  char *q = 0;
  unsigned int len = strlen(prompt);
  
  if(strlen(buf) > len)
    {
      q = &buf[len];
    }
  else
    {
      if(interactive)
	{
	  out << prompt << "> " << std::flush;
	}
      in.getline(buf, BUF_SIZE);
      q = buf;
    }
  question.AddSentence(q);
  if(!interactive)
    {
      out << prompt << " " << *(question.GetSentences()[0]) << std::endl << std::flush;
    }
  if(strcmp(prompt, "dpll") == 0)
    {
      rval = kb.DPLL_Entails(question);
    }
  else if(strcmp(prompt, "ask2") == 0)
    {
      rval = kb.TT_Entails_Pruned(question);
    }
  else if(strcmp(prompt, "ask") == 0)
    {
      rval = kb.TT_Entails(question);
    }
  else
    {
      out << "Unknown ask mode: " << prompt << std::endl;
      rval = ai::PL::Symbol::UNKNOWN;
    }
  if(rval == ai::PL::Symbol::KNOWN_TRUE)
    {
      out << "YES, " << q << " is entailed." << std::endl;
    }
  else
    {
      out << "NO, " << q << " is not entailed." << std::endl;
    }
  out << kb.GetCount() << " evaluations."  << std::endl;
  return 0;
}

int do_tell(ai::PL::KnowledgeBase &kb, int interactive, char *buf, std::istream &in, std::ostream &out)
{
  char * sentence = 0;
  if(strlen(buf) > 4)
    {
      sentence = &buf[4];
    }
  else
    {
      if(interactive)
	{
	  out << "tell> " << std::flush;
	}
      in.getline(buf, BUF_SIZE);
      sentence = buf;
    }
  kb.AddSentence(sentence);
  if(!interactive)
    {
      out << "tell " << sentence << std::endl << std::flush;
    }
  return 0;
}

int do_list(ai::PL::KnowledgeBase &kb, int interactive, char *buf, std::istream &in, std::ostream &out)
{
  const std::vector<ai::PL::Sentence *>       &sentences = kb.GetSentences();
  std::vector<ai::PL::Sentence *>::const_iterator sen_it = sentences.begin();
  while(sen_it != sentences.end())
    {
      out << *(*sen_it) << std::endl;
      sen_it ++;
    }
  return 0;
}

int do_cnf(ai::PL::KnowledgeBase &kb, int interactive, char *buf, std::istream &in, std::ostream &out)
{
  const std::vector<ai::PL::Sentence *>          &sentences = kb.GetSentences();
  std::vector<ai::PL::Sentence *>::const_iterator sen_it = sentences.begin();
  while(sen_it != sentences.end())
    {
      ai::PL::Sentence *s_tmp = new ai::PL::Sentence(*(*sen_it));
      s_tmp->CNF();
      out << *(s_tmp) << std::endl;
      sen_it ++;
      delete s_tmp;
    }
  return 0;
}

int do_load(ai::PL::KnowledgeBase &kb, int interactive, char *buf, std::istream &in, std::ostream &out)
{
  unsigned int i;
  for(i = 4; i < strlen(buf) && buf[i] == ' '; i++) { /* empty block */ }
  std::ifstream fin(&buf[i]);
  if(!fin)
    {
      out << "Unknown file: '" << &buf[i] << "'." << std::endl;
    }
  else
    {
      process_file(kb, buf, fin, out);
      fin.close();
    }
  return 0;
}

int do_echo(ai::PL::KnowledgeBase &kb, int interactive, char *buf, std::istream &in, std::ostream &out)
{
  unsigned int i;
  for(i = 4; i < strlen(buf) && buf[i] == ' '; i++) { /* empty block */ }
  out << &buf[i] << std::endl;
  return 0;
}

int process_line(ai::PL::KnowledgeBase &kb, int interactive, char *buf, std::istream &in, std::ostream &out)
{
  if(strncmp(buf, "tell", 4) == 0)
    {
      do_tell(kb, interactive, buf, in, out);
    }
  else if(strncmp(buf, "ask2", 4) == 0)
    {
      do_ask(kb, interactive, buf, in, out, "ask2");
    }
  else if(strncmp(buf, "dpll", 4) == 0)
    {
      do_ask(kb, interactive, buf, in, out, "dpll");
    }
  else if(strncmp(buf, "ask", 3) == 0)
    {
      do_ask(kb, interactive, buf, in, out, "ask");
    }
  else if(strncmp(buf, "list", 4) == 0)
    {
      do_list(kb, interactive, buf, in, out);
    }
  else if(strncmp(buf, "cnf", 3) == 0)
    {
      do_cnf(kb, interactive, buf, in, out);
    }
  else if(strncmp(buf, "quit", 4) == 0)
    {
      return 1;
    }
  else if(strncmp(buf, "load", 4) == 0)
    {
      do_load(kb, interactive, buf, in, out);
    }
  else if(strncmp(buf, "echo", 4) == 0)
    {
      do_echo(kb, interactive, buf, in, out);
    }
  else if(strncmp(buf, "help", 4) == 0)
    {
      out << "known commands" << std::endl
	  << "help  - this message" << std::endl
	  << "tell  - prompt for new statement" << std::endl
	  << "ask   - prompt for statement to check for entailment" << std::endl
	  << "ask2  - prompt for statement to check for entailment (pruned)" << std::endl
	  << "dpll  - prompt for statement to check for entailment (dpll)" << std::endl
	  << "list  - display known statements" << std::endl
	  << "cnf   - display known statements in CNF" << std::endl
	  << "load  - load statements from file" << std::endl
	  << "echo  - display text to output stream" << std::endl
	  << "quit  - end session" << std::endl << std::flush;
    }
  else if(strncmp(buf, "#", 1) == 0)
    {
      // comment, skip it
    }
  else if(strlen(buf) == 0)
    {
      // empty line, skip it
    }
  else
    {
      out << "unknown command.  use help to list known commands." << std::endl;
      out << buf << std::endl;
    }
  return 0;
}

int process_file(ai::PL::KnowledgeBase &kb, char *buf, std::istream &in, std::ostream &out)
{
  int done = 0;
  int interactive = 0;
  
  if(isatty(fileno(in)))
    {
      interactive = 1;
    }

  while(in.good() && !done)
    {
      if(interactive)
	{
	  out << "> " << std::flush;
	}
      in.getline(buf, BUF_SIZE);
      if((strlen(buf) == 0) && !in.good())
	{ // end of file without any data
	  out << std::endl;
	  continue;
	}
      done = process_line(kb, interactive, buf, in, out);
    }
  return 0;
}

int main()
{
  ai::PL::KnowledgeBase kb;
  char buf[BUF_SIZE];
  process_file(kb, buf, std::cin, std::cout);
  return 0;
}
