/**
 * @file main.cpp
 * @brief Point d'entrée du compilateur ifcc.
 *
 * Ce programme lit un fichier C simplifié, le parse avec ANTLR, vérifie la
 * table des symboles, puis génère du code intermédiaire et de l'assemblage.
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "antlr4-runtime.h"
#include "generated/ifccLexer.h"
#include "generated/ifccParser.h"
#include "generated/ifccBaseVisitor.h"

#include "CodeGenVisitor.h"
#include "IR.h"
#include "SymbolTableVisitor.h"

using namespace antlr4;
using namespace std;

int main(int argn, const char **argv)
{
  stringstream in;
  string filename;
  IRInstr::Target target = IRInstr::X86;

  if (argn == 3 && string(argv[1]) == "--msp430") {
      target = IRInstr::MSP430;
      filename = argv[2];
  } else if (argn == 2) {
      filename = argv[1];
  } else {
      cerr << "usage: ifcc [--msp430] path/to/file.c" << endl;
      exit(1);
  }

  ifstream lecture(filename);
  if (!lecture.good()) {
      cerr << "error: cannot read file: " << filename << endl;
      exit(1);
  }
  in << lecture.rdbuf();
  
  ANTLRInputStream input(in.str());

  ifccLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  tokens.fill();

  ifccParser parser(&tokens);
  tree::ParseTree* tree = parser.axiom();

  if(parser.getNumberOfSyntaxErrors() != 0)
  {
      cerr << "error: syntax error during parsing" << endl;
      exit(1);
  }

  SymbolTableVisitor stv;
  stv.visit(tree);

  if (stv.hasErrors()) {
    return 1; // on arrête la compilation s'il y a des erreurs de déclaration ou d'utilisation
  }

  DefFonction* def = new DefFonction("main", vector<pair<string,Type>>{}, INT32);

  CodeGenVisitor cgv(def, target);
  cgv.visit(tree);

  return 0;
}
