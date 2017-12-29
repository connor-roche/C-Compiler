/*
 * parser.h
 *
 *      Author: connor roche
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <istream>
#include <sstream>
#include <fstream>
#include <iostream>
using std::istream;

#include <string>
#include <map>
using std::string;
using std::stoi;
using std::map;
using namespace std;

#include "lexer.h"

extern void error(int linenum, const string& message);
extern void semanticError(int linenum, const string& message);
extern int lineNumber;
extern int s_error;


enum TypeForNode { INT_TYPE, STRING_TYPE, ERROR_TYPE };

class Value {
  TypeForNode	type;
  int			ival;
  string		sval;

 public:
 Value(TypeForNode type = ERROR_TYPE, int ival = 0, std::string sval = "") : type(type), ival(ival), sval(sval) {}

  TypeForNode getType() const { return type; }

  int GetIntValue() const {
    if( type == INT_TYPE ) return ival;
    throw "This variable is not an int";
  }
  void SetIntValue(int v) {
    if( type == INT_TYPE ) ival = v;
    throw "This variable is not an int";
  }

  string GetStringValue() const {
    if( type == STRING_TYPE ) return sval;
    throw "This variable is not a string";
  }
  void SetStringValue(string v) {
    if( type == STRING_TYPE ) sval = v;
    throw "This variable is not a string";
  }

  Value *Add(Value *right) {
    if (right->getType() == STRING_TYPE && this->getType() == STRING_TYPE)
      return new Value(STRING_TYPE, -1, this->sval + right->sval);
    else
      return new Value(INT_TYPE, right->ival + this->ival, "");
  };

  Value *Sub(Value *right) {
    return new Value(INT_TYPE, this->ival - right->ival, "");
  };

  Value *Div(Value *right) {
    if (right->getType() == STRING_TYPE && this->getType() == STRING_TYPE) {
      string r = this->sval;
      size_t f = this->sval.find(right->sval);
      if (f != std::string::npos) 
	r = r.substr(0, f) + r.substr(f + right->sval.size(), r.size() - 1);
      return new Value(STRING_TYPE, -1, r);
    }
    else
      return new Value(INT_TYPE, this->ival / right->ival, "");
  };

  Value *Mult(Value *right) {
    if (right->getType() == INT_TYPE && this->getType() == STRING_TYPE) {
      string r = "";
      int counter = right->ival;
      while (counter != 0) {
	r += this->sval;
	counter--;
      }
      return new Value(STRING_TYPE, -1, r);
    }
    else if (right->getType() == STRING_TYPE && this->getType() == INT_TYPE) {
      string r = "";
      int counter = this->ival;
      while (counter != 0) {
	r += right->sval;
	counter--;
      }
      return new Value(STRING_TYPE, -1, r);
    }
    else
      return new Value(INT_TYPE, right->ival * this->ival, "");
  };

  void printValue() {
    if(!this)
      return;

    if (this->getType() == STRING_TYPE)
      cout << this->sval;
    else
      cout << this->ival;
  }

	
};

extern map<string,Value> SymbolTable;

class ParseTree {
  int			linenumber;
  ParseTree	*left;
  ParseTree	*right;

 public:
 ParseTree(int n, ParseTree *l = 0, ParseTree *r = 0) : linenumber(n), left(l), right(r) {}
  virtual ~ParseTree() {}

  ParseTree* getLeft() const { return left; }
  ParseTree* getRight() const { return right; }
  int getLineNumber() const { return linenumber; }

  virtual TypeForNode getType() const { return ERROR_TYPE; }
  virtual int GetIntValue() const { throw "no integer value"; }
  virtual string GetStringValue() const { throw "no string value"; }

  void generateTrace(ostream& out) {
    if( left ) {
      out << "L";
      left->generateTrace(out);
      out << "u";
    }
    if( right ) {
      out << "R";
      right->generateTrace(out);
      out << "U";
    }
    out << "N";
  }

  int traceAndCount(int (ParseTree::*f)() const) {
    int cnt = 0;
    if( left ) {
      cnt += left->traceAndCount(f);
    }
    if( right ) {
      cnt += right->traceAndCount(f);
    }
    cnt += (this->*f)();
    return cnt;
  }

  virtual int countSet() const { return 0; }
  virtual int countPlus() const { return 0; }
  virtual int countStar() const { return 0; }

  virtual void evaluate(std::map<std::string, Value*> &state) {
    if (this->getLeft())
      this->getLeft()->evaluate(state);
    if (this->getRight())
      this->getRight()->evaluate(state);
  };

  virtual Value *getValue(std::map<std::string, Value*> &state) {
    return NULL;
  }

  virtual int findSemanticErrors() const { return 0; }
};

class StatementList : public ParseTree {
 public:
 StatementList(ParseTree *first, ParseTree *rest) : ParseTree(0, first, rest) {}

};

class DeclStatement : public ParseTree {
  TypeForNode	type;
  string 		id;

 public:
 DeclStatement(int line, TypeForNode ty, string id) : ParseTree(line), type(ty), id(id) {}

  int findSemanticErrors() const {
    if( SymbolTable.find(id) != SymbolTable.end() ) {
      semanticError(this->getLineNumber(), "variable " + id + " was already declared");
      return 1;
    }
    SymbolTable[id] = Value(type);
    return 0;
  }

};

class SetStatement : public ParseTree {
  string id;

 public:
 SetStatement(int line, string id, ParseTree *ex) : ParseTree(line, ex), id(id) {}
  int countSet() const { return 1; }

  int findSemanticErrors() const {
    if( SymbolTable.find(id) == SymbolTable.end() ) {
      semanticError(this->getLineNumber(), "variable " + id + " is used before being declared");
      return 1;
    }
    if ((this->getLeft()->getType()) != (SymbolTable[id].getType())) {
      semanticError(this->getLineNumber(), "type error");
      return 1;
    }
    return 0;
  }

  void evaluate(std::map<std::string, Value*> &state) {
    state[id] = this->getLeft()->getValue(state);
  };
};

class PrintStatement : public ParseTree {
  bool addNL;

 public:
 PrintStatement(int line, bool isPrintln, ParseTree *ex) : ParseTree(line, ex), addNL(isPrintln) {}

  void doprinting() const {
    if( addNL ) std::cout << std::endl;
  }

  void evaluate(std::map<std::string, Value*> &state) {
      this->getLeft()->getValue(state)->printValue();
      this->doprinting();
  };
};

class Addition : public ParseTree {
 public:
 Addition(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}

  // will need to fill in type and value;
  // remember type is a function of
  int countPlus() const { return 1; }

  TypeForNode getType() const {
    if ((this->getRight()->getType() == INT_TYPE) && (this->getLeft()->getType() == INT_TYPE))
      return INT_TYPE;
    if ((this->getRight()->getType() == STRING_TYPE) && (this->getLeft()->getType() == STRING_TYPE))
      return STRING_TYPE;
    return ERROR_TYPE;
  }
  
  int findSemanticErrors() const {
    if (this->getType() == ERROR_TYPE) {
      semanticError(this->getLineNumber(), "type error");
      return 1;
    }
    return 0;
  }

  Value *getValue(std::map<std::string, Value*> &state) {
    return this->getLeft()->getValue(state)->Add(this->getRight()->getValue(state));
  }
};

class Subtraction : public ParseTree {
 public:
 Subtraction(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}

  // will need to fill in type and value;
  // remember type is a function of
  TypeForNode getType() const {
    if ((this->getRight()->getType() == INT_TYPE) && (this->getLeft()->getType() == INT_TYPE))
      return INT_TYPE;
    return ERROR_TYPE;
  }
  
  int findSemanticErrors() const {
    if (this->getType() == ERROR_TYPE) {
      semanticError(this->getLineNumber(), "type error");
      return 1;
    }
    return 0;
  }

  Value *getValue(std::map<std::string, Value*> &state) {
    return this->getLeft()->getValue(state)->Sub(this->getRight()->getValue(state));
  }
};

class Multiplication : public ParseTree {
 public:
 Multiplication(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}

  // will need to fill in type and value;
  // remember type is a function of
  int countStar() const { return 1; }
  TypeForNode getType() const {
    if ((this->getRight()->getType() == INT_TYPE) && (this->getLeft()->getType() == INT_TYPE))
      return INT_TYPE;
    if ((this->getRight()->getType() == INT_TYPE) && (this->getLeft()->getType() == STRING_TYPE))
      return STRING_TYPE;
    if ((this->getRight()->getType() == STRING_TYPE) && (this->getLeft()->getType() == INT_TYPE))
      return STRING_TYPE;
    return ERROR_TYPE;
  }
  int findSemanticErrors() const {
    if (this->getType() == ERROR_TYPE) {
      semanticError(this->getLineNumber(), "type error");
      return 1;
    }
    return 0;
  }

  Value *getValue(std::map<std::string, Value*> &state) {
    return this->getLeft()->getValue(state)->Mult(this->getRight()->getValue(state));
  }
};

class Division : public ParseTree {
 public:
 Division(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}

  // will need to fill in type and value;
  // remember type is a function of
    TypeForNode getType() const {
    if ((this->getRight()->getType() == INT_TYPE) && (this->getLeft()->getType() == INT_TYPE))
      return INT_TYPE;
    if ((this->getRight()->getType() == STRING_TYPE) && (this->getLeft()->getType() == STRING_TYPE))
      return STRING_TYPE;
    return ERROR_TYPE;
  }
  
  int findSemanticErrors() const {
    if (this->getType() == ERROR_TYPE) {
      semanticError(this->getLineNumber(), "type error");
      return 1;
    }
    return 0;
  }

  Value *getValue(std::map<std::string, Value*> &state) {
    return this->getLeft()->getValue(state)->Div(this->getRight()->getValue(state));
  }
};

class IntegerConstant : public ParseTree {
  int	value;

 public:
 IntegerConstant(const Token& tok) : ParseTree(tok.GetLinenum()) {
    value = stoi( tok.GetLexeme() );
  }

  TypeForNode getType() const { return INT_TYPE; }
  int GetIntValue() const { return value; }

  Value *getValue(std::map<std::string, Value*> &state) {
    return new Value(INT_TYPE, value, "");
  }
};

class StringConstant : public ParseTree {
  string	value;

 public:
 StringConstant(const Token& tok) : ParseTree(tok.GetLinenum()) {
    value = tok.GetLexeme();
  }

  TypeForNode getType() const { return STRING_TYPE; }
  string GetStringValue() const { return value; }

  Value *getValue(std::map<std::string, Value*> &state) {
    return new Value(STRING_TYPE, -1, value.substr(1, value.size() - 2));
  }
};

class Identifier : public ParseTree {
  string	value;

 public:
 Identifier(const Token& tok) : ParseTree(tok.GetLinenum()) {
    value = tok.GetLexeme();
  }

  TypeForNode getType() const {
    if (SymbolTable.find(value) != SymbolTable.end())
      return SymbolTable[value].getType();
    else
      return STRING_TYPE;
  }
  int GetIntValue() const { return 0; }
  string GetStringValue() const { return "foo"; }

  int findSemanticErrors() const {
    if( SymbolTable.find(value) == SymbolTable.end() ) {
      semanticError(this->getLineNumber(), "variable " + value + " is used before being declared");
      return 1;
    }
    return 0;
  }

  Value *getValue(std::map<std::string, Value*> &state) {
      return state[value];
    }
};

extern ParseTree *	Prog(istream* in);
extern ParseTree *	StmtList(istream* in);
extern ParseTree *	Stmt(istream* in);
extern ParseTree *	Decl(istream* in);
extern ParseTree *	Set(istream* in);
extern ParseTree *	Print(istream* in);
extern ParseTree *	Expr(istream* in);
extern ParseTree *	Term(istream* in);
extern ParseTree *	Primary(istream* in);


#endif /* PARSER_H_ */
