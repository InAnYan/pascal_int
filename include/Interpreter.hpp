#ifndef PASCAL_INTERPRETER_HPP
#define PASCAL_INTERPRETER_HPP

#include <Visitor.hpp>
#include <CallStack.hpp>

namespace Pascal
{
	class Interpreter : public AST::Visitor
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
		void visitProcDeclNode(const AST::ProcDeclNode &node);
		void visitParamNode(const AST::ParamNode &node);
		void visitProcCallNode(const AST::ProcCallNode& node);

	private:
		CallStack m_CallStack;
		ARObject m_Accum;
	};
}    

#endif
