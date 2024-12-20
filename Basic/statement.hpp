/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include <sstream>
#include <string>
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"
#include "Utils/tokenScanner.hpp"
#include "evalstate.hpp"
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"

class Program;

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement
{
public:
  /*
   * Constructor: Statement
   * ----------------------
   * The base class constructor is empty.  Each subclass must provide
   * its own constructor.
   */

  Statement();
  /*
   * Destructor: ~Statement
   * Usage: delete stmt;
   * -------------------
   * The destructor deallocates the storage for this expression.
   * It must be declared virtual to ensure that the correct subclass
   * destructor is called when deleting a statement.
   */

  virtual ~Statement();

  /*
   * Method: execute
   * Usage: stmt->execute(state);
   * ----------------------------
   * This method executes a BASIC statement.  Each of the subclasses
   * defines its own execute method that implements the necessary
   * operations.  As was true for the expression evaluator, this
   * method takes an EvalState object for looking up variables or
   * controlling the operation of the interpreter.
   */
  virtual void execute(EvalState &state, Program &program) = 0;

  virtual void dir_execute(EvalState &state, Program &program) = 0;

private:
};


/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

class REMStatement : public Statement
{
  std::string line;
  friend Program;
  friend void direct_execute(std::string &line, Program &program, EvalState &state);

  REMStatement(const std::string &line);

  ~REMStatement() override = default;

  void execute(EvalState &state, Program &program) override;

  void dir_execute(EvalState &state, Program &program) override;
};

class LETStatement : public Statement
{
  std::string line;
  friend Program;
  friend void direct_execute(std::string &line, Program &program, EvalState &state);

  LETStatement(const std::string &line);

  ~LETStatement() override = default;

  void execute(EvalState &state, Program &program) override;

  void dir_execute(EvalState &state, Program &program) override;
};

class PRINTStatement : public Statement
{
  std::string line;
  friend Program;
  friend void direct_execute(std::string &line, Program &program, EvalState &state);

  PRINTStatement(const std::string &line);

  ~PRINTStatement() override = default;

  void execute(EvalState &state, Program &program) override;

  void dir_execute(EvalState &state, Program &program) override;
};

class INPUTStatement : public Statement
{
  std::string line;
  friend Program;
  friend void direct_execute(std::string &line, Program &program, EvalState &state);

  INPUTStatement(const std::string &line);

  ~INPUTStatement() override = default;

  void execute(EvalState &state, Program &program) override;

  void dir_execute(EvalState &state, Program &program) override;
};

class ENDStatement : public Statement
{
  friend Program;
  friend void direct_execute(std::string &line, Program &program, EvalState &state);

  ENDStatement();

  ~ENDStatement() override = default;

  void execute(EvalState &state, Program &program) override;

  void dir_execute(EvalState &state, Program &program) override;
};

class GOTOStatement : public Statement
{
  std::string line;
  friend Program;
  friend void direct_execute(std::string &line, Program &program, EvalState &state);

  GOTOStatement(const std::string &line);

  ~GOTOStatement() override = default;

  void execute(EvalState &state, Program &program) override;

  void dir_execute(EvalState &state, Program &program) override;
};

class IFStatement : public Statement
{
  std::string line;
  friend Program;
  friend void direct_execute(std::string &line, Program &program, EvalState &state);

  IFStatement(const std::string &line);

  ~IFStatement() override = default;

  void execute(EvalState &state, Program &program) override;

  void dir_execute(EvalState &state, Program &program) override;
};
#endif
