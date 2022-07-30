#ifndef PASCAL_GRAPHVIZ_VISITOR_HPP
#define PASCAL_GRAPHVIZ_VISITOR_HPP

#include <Visitor.hpp>

#include <string>
#include <vector>
#include <fstream>

namespace Pascal
{
	class GraphvizVisitor : public AST::Visitor
	{
	public:
		GraphvizVisitor(std::string const& fileName);
		~GraphvizVisitor();

		void flush();
		
		void visitProgramNode(AST::ProgramNode const& node);
		void visitVarDeclNode(AST::VarDeclNode const& node);
		void visitBlockNode(AST::BlockNode const& node);
		void visitTypeNode(AST::TypeNode const& node);
		void visitStatementNode(AST::StatementNode const& node);
		void visitCompoundNode(AST::CompoundNode const& node);
		void visitAssignmentNode(AST::AssignmentNode const& node);
		void visitVariableNode(AST::VariableNode const& node);
		void visitNullStatementNode(AST::NullStatementNode const& node);
		void visitNumberNode(AST::NumberNode const& node);
		void visitBinOpNode(AST::BinOpNode const& node);
		void visitUnaryOpNode(AST::UnaryOpNode const& node);
		void visitProcDeclNode(const AST::ProcDeclNode &node);
		void visitParamNode(const AST::ParamNode &node);
		void visitProcCallNode(const AST::ProcCallNode& node);

	private:
		long internalCounter;
		std::vector<std::string> derivateStack;
		std::string fileOutName;
		std::ofstream fout;
		std::string createNode(std::string const& name);
	};
}

#endif
