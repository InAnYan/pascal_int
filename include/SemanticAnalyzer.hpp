#ifndef PASCAL_SEMANTIC_ANALYZER_HPP
#define PASCAL_SEMANTIC_ANALYZER_HPP

#include <Visitor.hpp>
#include <AST.hpp>
#include <Symbols.hpp>

namespace Pascal
{
	class SemanticAnalyzer : public AST::Visitor
	{
	public:
		SemanticAnalyzer();

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
		
	    std::shared_ptr<SymbolTable> getSymbolTable() const
		{ return m_Symtab; }
	private:
		std::shared_ptr<SymbolTable> m_Symtab;
		unsigned m_CurrentScopeLevel;
	};
}

#endif
