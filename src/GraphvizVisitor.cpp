#include <GraphvizVisitor.hpp>
#include <AST.hpp>

#include <sstream>
#include <algorithm>

namespace Pascal
{
	std::string quotesTransform(std::string const& work)
	{
		std::stringstream ss;
		for (char ch : work)
		{
			if (ch == '\"')
				ss << '\\' << ch;
			else
				ss << ch;
		}
		return ss.str();
	}
	
	GraphvizVisitor::GraphvizVisitor(std::string const& fileName)
		: fileOutName(fileName), fout(fileName, std::ios::out)
	{
		fout << "graph {\n\tlabel = \"" + fileName + "\"\n\tfontname = \"Courier New\"\n\t";
		internalCounter = 0;
	}

	GraphvizVisitor::~GraphvizVisitor()
	{
		fout.close();
	}

	void GraphvizVisitor::flush()
	{
		fout << "}\n";
		fout.flush();
	}

	std::string GraphvizVisitor::createNode(std::string const& name)
	{
		std::string id = "n" + std::to_string(internalCounter);
		fout << id << " [label=\"" + quotesTransform(name) + "\" shape=box fontname = \"Courier New\"]\n\t";
		
		if (!derivateStack.empty())
			fout << derivateStack.back() << " -- " << id << "\n\t";
		internalCounter++;
		return id;
	}

	void GraphvizVisitor::visitProgramNode(AST::ProgramNode const& node)
	{
	    derivateStack.push_back(createNode("Program: \"" + node.getName().str + "\""));
		node.getBlock().accept(this);
		derivateStack.pop_back();
	}
	
	void GraphvizVisitor::visitVarDeclNode(AST::VarDeclNode const& node)
	{
		derivateStack.push_back(createNode("VarDecl"));
		node.getVar().accept(this);
		node.getType().accept(this);
		derivateStack.pop_back();
	}
	void GraphvizVisitor::visitBlockNode(AST::BlockNode const& node)
	{
		derivateStack.push_back(createNode("Block"));
		
		for (auto const& e : node.getVarDecls())
		{
			e->accept(this);
		}
		
		for (auto const& e : node.getProcDecls())
		{
			e->accept(this);
		}
				
	    node.getCompound().accept(this);
		
		derivateStack.pop_back();
	}

	void GraphvizVisitor::visitTypeNode(AST::TypeNode const& node)
	{
	    createNode(node.getToken().str);
	}

	void GraphvizVisitor::visitStatementNode(AST::StatementNode const& node)
	{
		derivateStack.push_back(createNode("Statement"));
		node.accept(this);
		derivateStack.pop_back();
	}

	void GraphvizVisitor::visitCompoundNode(AST::CompoundNode const& node)
	{
		derivateStack.push_back(createNode("Compound"));
		for (auto const& e : node.getStatements())
		{
			e->accept(this);
		}
		derivateStack.pop_back();
	}

	void GraphvizVisitor::visitAssignmentNode(AST::AssignmentNode const& node)
	{
		derivateStack.push_back(createNode(":="));
		node.getVar().accept(this);
		node.getExpr().accept(this);
		derivateStack.pop_back();
	}

	void GraphvizVisitor::visitVariableNode(AST::VariableNode const& node)
	{
	    createNode(node.getToken().str);
	}

	void GraphvizVisitor::visitNullStatementNode(AST::NullStatementNode const& node)
	{
	    createNode("NoOp");
	}

	void GraphvizVisitor::visitNumberNode(AST::NumberNode const& node)
	{
	    std::string name = node.getToken().str;
	    createNode(name);
	}

	void GraphvizVisitor::visitBinOpNode(AST::BinOpNode const& node)
	{
	    derivateStack.push_back(createNode(node.getOperation().str));
		node.getLeft().accept(this);
		node.getRight().accept(this);
		derivateStack.pop_back();
	}

	void GraphvizVisitor::visitUnaryOpNode(AST::UnaryOpNode const& node)
	{
	    derivateStack.push_back(createNode(node.getOperation().str));
		node.getExpr().accept(this);
		derivateStack.pop_back();
	}

	void GraphvizVisitor::visitProcDeclNode(const AST::ProcDeclNode &node)
	{
		std::string name = "ProcDecl: \"" + node.getToken().str + "\"";
	    derivateStack.push_back(createNode(name));
	    for (auto const& e : node.getParams())
		{
			e->accept(this);
		}
		node.getBlock().accept(this);
		derivateStack.pop_back();
	}

	void GraphvizVisitor::visitParamNode(const AST::ParamNode &node)
	{
		derivateStack.push_back(createNode("Param"));
		node.getVar().accept(this);
		node.getType().accept(this);
		derivateStack.pop_back();
	}
}
