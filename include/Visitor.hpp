#ifndef PASCAL_VISITOR_HPP
#define PASCAL_VISITOR_HPP

namespace Pascal
{
	namespace AST
	{
		class ProgramNode;
		class VarDeclNode;
		class BlockNode;
		class TypeNode;
		class StatementNode;
		class CompoundNode;
		class AssignmentNode;
		class VariableNode;
		class NullStatementNode;
		class NumberNode;
		class BinOpNode;
		class UnaryOpNode;
		class ProcDeclNode;
		class ParamNode;
		class ProcCallNode;
		
		class Visitor
		{
		public:
			virtual ~Visitor() {};

			virtual void visitProgramNode       (AST::ProgramNode       const& node) = 0;
			virtual void visitVarDeclNode       (AST::VarDeclNode       const& node) = 0;
			virtual void visitBlockNode         (AST::BlockNode         const& node) = 0;
			virtual void visitTypeNode          (AST::TypeNode          const& node) = 0;
			virtual void visitStatementNode     (AST::StatementNode     const& node) = 0;
			virtual void visitCompoundNode      (AST::CompoundNode      const& node) = 0;
			virtual void visitAssignmentNode    (AST::AssignmentNode    const& node) = 0;
			virtual void visitVariableNode      (AST::VariableNode      const& node) = 0;
			virtual void visitNullStatementNode (AST::NullStatementNode const& node) = 0;
			virtual void visitNumberNode        (AST::NumberNode        const& node) = 0;
			virtual void visitBinOpNode         (AST::BinOpNode         const& node) = 0;
			virtual void visitUnaryOpNode       (AST::UnaryOpNode       const& node) = 0;
			virtual void visitProcDeclNode      (AST::ProcDeclNode      const& node) = 0;
			virtual void visitParamNode         (AST::ParamNode         const& node) = 0;
			virtual void visitProcCallNode      (AST::ProcCallNode      const& node) = 0;
		};
	}
}

#endif
