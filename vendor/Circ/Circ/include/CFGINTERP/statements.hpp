#pragma once
#include <any>
#include <vector>
#include <iostream>
#include "expressions.hpp"
/**
* The purpose of statements in the Circ parser is to separate execution of ast nodes from evaluation of other nodes.
* When we execute a statement we can define which expression nodes needs to be evaluated within that statement.
* Since we only allow for declarations in the Circ cfg language, statements are reduced to a Declaration.
* Statements implementation is structured via a Visitor which allows for an easier REPL implementation.
*/
namespace Serialization {



	struct Block;
	struct Decl;
	


	struct StatementVisitor {
		virtual std::any visitDecl(Decl* d) = 0;
	};

	struct BaseStatement {
		BaseStatement() {};
		virtual std::any accept(StatementVisitor* v) = 0;
		~BaseStatement() {};
	};


	struct Decl : BaseStatement {
		std::string key;
		BaseExpression* value;
		Decl(const std::string& k, BaseExpression* v) : key(k), value(v) {};
		std::any accept(StatementVisitor* v) override {
			return v->visitDecl(this);
		}
		~Decl() {};
	};

	
}
