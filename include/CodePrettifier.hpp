#ifndef PASCAL_CODE_PRETTIFIER_HPP
#define PASCAL_CODE_PRETTIFIER_HPP

#include <Visitor.hpp>

#include <string>
#include <sstream>
#include <memory>

namespace Pascal
{
	class CodePrettifier : public AST::Visitor
	{
	public:
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
		void visitProcDeclNode(AST::ProcDeclNode const& node);
		void visitParamNode(const AST::ParamNode &node);

		std::string toString() const;
		
	private:
		unsigned currentScopeLevel = 1;
		
		std::stringstream ss;
	};
}

#endif
