/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"


/* Implementation of the Statement class */

bool isVariable(const std::string &var);

bool isNumber(const std::string &temp);

int GetValue(TokenScanner &token_scanner, EvalState &state);

bool check(char op, int lvalue, int rvalue);

Statement::Statement() = default;

Statement::~Statement() = default;

REMStatement::REMStatement(const std::string &line) { this->line = line; }

void REMStatement::execute(EvalState &state, Program &program)
{
  program.adjustGOTO(false);
  program.gotoNextLine();
}

void REMStatement::dir_execute(EvalState &state, Program &program) { program.adjustGOTO(false); }

LETStatement::LETStatement(const std::string &line) { this->line = line; }

void LETStatement::execute(EvalState &state, Program &program)
{
  program.adjustGOTO(false);
  TokenScanner token_scanner;
  token_scanner.ignoreWhitespace();
  // token_scanner.scanNumbers();
  token_scanner.setInput(line);
  token_scanner.nextToken();
  std::string var = token_scanner.nextToken();
  if (!isVariable(var))
  {
    error("SYNTAX ERROR");
  }
  if (token_scanner.nextToken() == "=")
  {
    int value = GetValue(token_scanner, state);
    state.setValue(var, value);
    program.gotoNextLine();
  }
  else
  {
    error("SYNTAX ERROR");
  }
}

void LETStatement::dir_execute(EvalState &state, Program &program)
{
  program.adjustGOTO(false);
  TokenScanner token_scanner;
  token_scanner.ignoreWhitespace();
  token_scanner.scanNumbers();
  token_scanner.setInput(line);
  token_scanner.nextToken();
  std::string var = token_scanner.nextToken();
  if (!isVariable(var))
  {
    error("SYNTAX ERROR");
  }
  if (token_scanner.nextToken() == "=")
  {
    int value = GetValue(token_scanner, state);
    state.setValue(var, value);
  }
  else
  {
    error("SYNTAX ERROR");
  }
}

PRINTStatement::PRINTStatement(const std::string &line) { this->line = line; }

void PRINTStatement::execute(EvalState &state, Program &program)
{
  program.adjustGOTO(false);
  TokenScanner token_scanner;
  token_scanner.ignoreWhitespace();
  token_scanner.scanNumbers();
  token_scanner.setInput(line);
  token_scanner.nextToken();
  int value = GetValue(token_scanner, state);
  if (value == -1919810)
  {
    program.end();
    return;
  }
  if (value != -114514)
  {
    std::cout << value << "\n";
  }
  program.gotoNextLine();
}

void PRINTStatement::dir_execute(EvalState &state, Program &program)
{
  program.adjustGOTO(false);
  TokenScanner token_scanner;
  token_scanner.ignoreWhitespace();
  token_scanner.scanNumbers();
  token_scanner.setInput(line);
  token_scanner.nextToken();
  int value = GetValue(token_scanner, state);
  if (value == -1919810)
  {
    program.end();
    return;
  }
  if (value != -114514)
  {
    std::cout << value << "\n";
  }
}

INPUTStatement::INPUTStatement(const std::string &line) { this->line = line; }

void INPUTStatement::execute(EvalState &state, Program &program)
{
  program.adjustGOTO(false);
  std::cout << " ? ";
  TokenScanner token_scanner;
  token_scanner.ignoreWhitespace();
  // token_scanner.scanNumbers();
  token_scanner.setInput(line);
  token_scanner.nextToken();
  std::string var = token_scanner.nextToken();
  std::string temp;
  int value;
  while (true)
  {
    getline(std::cin, temp);
    if (!isNumber(temp))
    {
      std::cout << "INVALID NUMBER\n"
                << " ? ";
    }
    else
    {
      value = stringToInteger(temp);
      break;
    }
  }
  state.setValue(var, value);
  program.gotoNextLine();
}

void INPUTStatement::dir_execute(EvalState &state, Program &program)
{
  program.adjustGOTO(false);
  std::cout << " ? ";
  TokenScanner token_scanner;
  token_scanner.ignoreWhitespace();
  token_scanner.scanNumbers();
  token_scanner.setInput(line);
  token_scanner.nextToken();
  std::string var = token_scanner.nextToken();
  std::string temp;
  int value;
  while (true)
  {
    getline(std::cin, temp);
    if (!isNumber(temp))
    {
      std::cout << "INVALID NUMBER\n"
                << " ? ";
    }
    else
    {
      value = stringToInteger(temp);
      break;
    }
  }
  state.setValue(var, value);
}

ENDStatement::ENDStatement() = default;

void ENDStatement::execute(EvalState &state, Program &program) { program.end(); }

void ENDStatement::dir_execute(EvalState &state, Program &program) { program.end(); }

GOTOStatement::GOTOStatement(const std::string &line) { this->line = line; }

void GOTOStatement::execute(EvalState &state, Program &program)
{
  program.adjustGOTO(true);
  TokenScanner token_scanner;
  token_scanner.ignoreWhitespace();
  token_scanner.scanNumbers();
  token_scanner.setInput(line);
  token_scanner.nextToken();
  int lineNumber = stringToInteger(token_scanner.nextToken());
  program.gotoLineNumber(lineNumber);
}

void GOTOStatement::dir_execute(EvalState &state, Program &program)
{
  program.adjustGOTO(true);
  TokenScanner token_scanner;
  token_scanner.ignoreWhitespace();
  token_scanner.scanNumbers();
  token_scanner.setInput(line);
  token_scanner.nextToken();
  int lineNumber = stringToInteger(token_scanner.nextToken());
  program.gotoLineNumber(lineNumber);
}

IFStatement::IFStatement(const std::string &line) { this->line = line; }

void IFStatement::execute(EvalState &state, Program &program)
{
  int cmp = 0;
  while (line[cmp] != '=' && line[cmp] != '>' && line[cmp] != '<')
  {
    cmp++;
  }
  char op = line[cmp];
  std::string left_exp = line.substr(3, cmp - 3);
  TokenScanner left_scanner;
  left_scanner.ignoreWhitespace();
  // left_scanner.scanNumbers();
  left_scanner.setInput(left_exp);
  int left_value = GetValue(left_scanner, state);
  if (left_value == -1919810)
  {
    program.end();
    return;
  }
  int r_begin = cmp + 1;
  while (line[r_begin] == ' ')
  {
    r_begin++;
  }
  int r_end = r_begin;
  while (line[r_end + 1] != 'T' || line[r_end + 2] != 'H' || line[r_end + 3] != 'E' || line[r_end + 4] != 'N')
  {
    r_end++;
  }
  std::string right_exp = line.substr(r_begin, r_end - r_begin + 1);
  TokenScanner right_scanner;
  right_scanner.ignoreWhitespace();
  // right_scanner.scanNumbers();
  right_scanner.setInput(right_exp);
  int right_value = GetValue(right_scanner, state);
  if (right_value == -1919810)
  {
    program.end();
    return;
  }
  if (check(op, left_value, right_value))
  {
    std::string then_exp = line.substr(r_end + 1);
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(then_exp);
    scanner.nextToken();
    int lineNumber = stringToInteger(scanner.nextToken());
    program.adjustGOTO(true);
    program.gotoLineNumber(lineNumber);
  }
  else
    program.gotoNextLine();
}

void IFStatement::dir_execute(EvalState &state, Program &program)
{
  int cmp = 0;
  while (line[cmp] != '=' && line[cmp] != '>' && line[cmp] != '<')
  {
    cmp++;
  }
  char op = line[cmp];
  std::string left_exp = line.substr(3, cmp - 3);
  TokenScanner left_scanner;
  left_scanner.ignoreWhitespace();
  left_scanner.scanNumbers();
  left_scanner.setInput(left_exp);
  int left_value = GetValue(left_scanner, state);
  int r_begin = cmp + 1;
  while (line[r_begin] == ' ')
  {
    r_begin++;
  }
  int r_end = r_begin;
  while (line[r_end + 1] != 'T' || line[r_end + 2] != 'H' || line[r_end + 3] != 'E' || line[r_end + 4] != 'N')
  {
    r_end++;
  }
  std::string right_exp = line.substr(r_begin, r_end - r_begin + 1);
  TokenScanner right_scanner;
  right_scanner.ignoreWhitespace();
  right_scanner.scanNumbers();
  right_scanner.setInput(right_exp);
  int right_value = GetValue(right_scanner, state);
  if (check(op, left_value, right_value))
  {
    std::string then_exp = line.substr(r_end + 1);
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(then_exp);
    scanner.nextToken();
    int lineNumber = stringToInteger(scanner.nextToken());
    program.adjustGOTO(true);
    program.gotoLineNumber(lineNumber);
  }
  else
    program.gotoNextLine();
}

int GetValue(TokenScanner &token_scanner, EvalState &state)
{
  Expression *exp = parseExp(token_scanner);
  try
  {
    const int value = exp->eval(state);
    delete exp;
    return value;
  }
  catch (ErrorException &ex)
  {
    std::cout << ex.getMessage() << std::endl;
    delete exp;
    if (ex.getMessage() == "DIVIDE BY ZERO")
    {
      return -1919810;
    }
    return -114514;
  }
}

bool check(const char op, const int lvalue, const int rvalue)
{
  if (op == '=')
    return lvalue == rvalue;
  if (op == '<')
    return lvalue < rvalue;
  if (op == '>')
    return lvalue > rvalue;
  error("SYNTAX ERROR");
  return false;
}

bool isVariable(const std::string &var)
{
  for (int i = 0; i < var.length(); i++)
  {
    if ((var[i] >= '0' && var[i] <= '9') || (var[i] >= 'a' && var[i] <= 'z') || (var[i] >= 'A' && var[i] <= 'Z'))
      continue;
    return false;
  }
  if (var == "REM" || var == "LET" || var == "PRINT" || var == "END" || var == "RUN" || var == "INPUT" ||
      var == "GOTO" || var == "IF" || var == "THEN" || var == "QUIT" || var == "LIST" || var == "CLEAR" ||
      var == "HELP")
  {
    return false;
  }
  return true;
}

bool isNumber(const std::string &temp)
{
  if (temp[0] != '-' && (temp[0] < '0' || temp[0] > '9'))
    return false;
  for (int i = 1; i < temp.length(); i++)
  {
    if (temp[i] < '0' || temp[i] > '9')
    {
      return false;
    }
  }
  return true;
}
