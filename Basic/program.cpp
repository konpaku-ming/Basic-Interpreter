/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"


Program::Program() = default;

Program::~Program() = default;

void Program::clear()
{
  // Replace this stub with your own code
  for (auto it : line_num)
  {
    delete statement_list[it];
  }
  line_num.clear();
  line_list.clear();
  statement_list.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line)
{
  // Replace this stub with your own code
  if (isLineExist(lineNumber))
  {
    delete statement_list[lineNumber];
  }
  line_num.insert(lineNumber);
  Statement *st = Program::setStatement(line);
  line_list[lineNumber] = line;
  statement_list[lineNumber] = st;
}

void Program::removeSourceLine(int lineNumber)
{
  // Replace this stub with your own code
  if (isLineExist(lineNumber))
  {
    line_num.erase(lineNumber);
    line_list.erase(lineNumber);
    line_num_str_list.erase(lineNumber);
    delete statement_list[lineNumber];
    statement_list.erase(lineNumber);
  }
}

std::string Program::getSourceLine(int lineNumber)
{
  // Replace this stub with your own code
  if (isLineExist(lineNumber))
  {
    return line_list[lineNumber];
  }
  return "";
}

void Program::setParsedStatement(int lineNumber, Statement *stmt)
{
  // Replace this stub with your own code
  if (isLineExist(lineNumber))
  {
    statement_list[lineNumber] = stmt;
  }
  else
    error("SYNTAX ERROR\n");
}

// void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber)
{
  // Replace this stub with your own code
  if (isLineExist(lineNumber))
  {
    return statement_list[lineNumber];
  }
  return nullptr;
}

int Program::getFirstLineNumber()
{
  // Replace this stub with your own code
  if (line_num.empty())
  {
    return -1;
  }
  return *line_num.begin();
}

int Program::getNextLineNumber(int lineNumber)
{
  // Replace this stub with your own code
  auto cur_it = line_num.find(lineNumber);
  if (cur_it != line_num.end())
  {
    ++cur_it;
    if (cur_it != line_num.end())
    {
      return *cur_it;
    }
  }
  return -1;
}

// more func to add
bool Program::isLineExist(int lineNumber)
{
  if (line_num.find(lineNumber) != line_num.end())
    return true;
  return false;
}

int Program::getCurLineNumber() const { return cur_line_num; }

void Program::initCurLineNumber() { cur_line_num = getFirstLineNumber(); }

void Program::gotoNextLine() { cur_line_num = getNextLineNumber(cur_line_num); }

void Program::adjustGOTO(bool opt) { is_goto = opt; }

void Program::end() { cur_line_num = -1; }

void Program::gotoLineNumber(const int lineNumber)
{
  if (isLineExist(lineNumber))
  {
    if (is_goto)
    {
      cur_line_num = lineNumber;
    }
  }
  else
  {
    error("LINE NUMBER ERROR");
  }
}

Statement *Program::setStatement(const std::string &line)
{
  TokenScanner token_scanner;
  token_scanner.ignoreWhitespace();
  token_scanner.scanNumbers();
  token_scanner.setInput(line);
  std::string cmd = token_scanner.nextToken();
  if (cmd == "REM")
  {
    return new REMStatement(line);
  }
  if (cmd == "LET")
  {
    return new LETStatement(line);
  }
  if (cmd == "PRINT")
  {
    return new PRINTStatement(line);
  }
  if (cmd == "INPUT")
  {
    return new INPUTStatement(line);
  }
  if (cmd == "END")
  {
    return new ENDStatement();
  }
  if (cmd == "GOTO")
  {
    return new GOTOStatement(line);
  }
  if (cmd == "IF")
  {
    return new IFStatement(line);
  }
  error("SYNTAX ERROR");
  return nullptr;
}

void Program::addLineNumberStr(int lineNumber, std::string &lineNumberStr)
{
  line_num_str_list[lineNumber] = lineNumberStr;
}

void Program::list()
{
  for (auto it : line_num)
  {
    std::cout << line_num_str_list[it] << " " << line_list[it] << "\n";
  }
}

void Program::run(EvalState &state)
{
  initCurLineNumber();
  while (cur_line_num != -1)
  {
    Statement *cur_statement = statement_list[cur_line_num];
    cur_statement->execute(state, *this);
  }
}

void Program::quit()
{
  for (auto it : line_num)
  {
    delete statement_list[it];
  }
}
