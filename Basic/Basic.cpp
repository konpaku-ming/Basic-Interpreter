/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"
#include "Utils/tokenScanner.hpp"
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"


/* Function prototypes */

void processLine(std::string &line, Program &program, EvalState &state);
void direct_execute(std::string &line, Program &program, EvalState &state);

/* Main program */

int main()
{
  EvalState state;
  Program program;
  // cout << "Stub implementation of BASIC" << endl;
  while (true)
  {
    try
    {
      std::string input;
      getline(std::cin, input);
      if (input.empty())
        continue;
      processLine(input, program, state);
    }
    catch (ErrorException &ex)
    {
      std::cout << ex.getMessage() << std::endl;
    }
  }
  return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string &line, Program &program, EvalState &state)
{
  TokenScanner scanner;
  scanner.ignoreWhitespace();
  scanner.scanNumbers();
  scanner.setInput(line);
  if (line[0] >= '0' && line[0] <= '9')
  {
    int split = 0;
    int lineNumber = 0;
    while (line[split] != ' ' && split < line.length())
    {
      lineNumber = lineNumber * 10 + line[split] - '0';
      split++;
    }
    if (split == line.length())
    {
      program.removeSourceLine(lineNumber);
    }
    else
    {
      std::string lineNumberStr = line.substr(0, split);
      split++;
      line = line.substr(split);
      program.addSourceLine(lineNumber, line);
      program.addLineNumberStr(lineNumber, lineNumberStr);
    }
  }
  else
  {
    direct_execute(line, program, state);
  }
}

void direct_execute(std::string &line, Program &program, EvalState &state)
{
  TokenScanner scanner;
  scanner.ignoreWhitespace();
  scanner.scanNumbers();
  scanner.setInput(line);
  std::string cmd = scanner.nextToken();
  if (cmd == "LET")
  {
    auto temp = LETStatement(line);
    try
    {
      temp.dir_execute(state, program);
      // program.gotoNextLine();
    }
    catch (ErrorException &ex)
    {
      std::cout << ex.getMessage() << std::endl;
    }
  }
  else if (cmd == "PRINT")
  {
    auto temp = PRINTStatement(line);
    try
    {
      temp.dir_execute(state, program);
      // program.gotoNextLine();
    }
    catch (ErrorException &ex)
    {
      std::cout << ex.getMessage() << std::endl;
    }
  }
  else if (cmd == "INPUT")
  {
    auto temp = INPUTStatement(line);
    try
    {
      temp.dir_execute(state, program);
      // program.gotoNextLine();
    }
    catch (ErrorException &ex)
    {
      std::cout << ex.getMessage() << std::endl;
    }
  }
  else if (cmd == "RUN")
  {
    program.run(state);
    return;
  }
  else if (cmd == "LIST")
  {
    program.list();
    return;
  }
  else if (cmd == "CLEAR")
  {
    program.clear();
    state.Clear();
    return;
  }
  else if (cmd == "QUIT")
  {
    program.quit();
    exit(0);
  }
  else if (cmd == "HELP")
  {
    std::cout << "WHAT CAN I SAY,MAN!\n";
    return;
  }
  else
  {
    error("SYNTAX ERROR");
  }
}
