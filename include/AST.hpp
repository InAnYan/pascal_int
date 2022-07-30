#ifndef PASCAL_AST_HPP
#define PASCAL_AST_HPP

#include <Visitor.hpp>
#include <Lexer.hpp>

#include <string>
#include <memory>

namespace Pascal
{
	class Parser;
	
	namespace AST
	{
		class Node
		{
		public:
			virtual ~Node() {}
			virtual void accept(Visitor* visitor) const = 0;
		};

		class ProgramNode : public Node
		{
		public:
			ProgramNode(Token name, std::unique_ptr<BlockNode> block)
				: m_Name(std::move(name)), m_Block(std::move(block)) {}
			
			Token getName() const { return m_Name; }
			BlockNode const& getBlock() const { return *m_Block; }

			void accept(Visitor* visitor) const
			{
				visitor->visitProgramNode(*this);
			}
			
		private:
			Token_t m_Name;
			std::unique_ptr<BlockNode> m_Block;
		};

		class BlockNode : public Node
		{
		public:
			BlockNode(std::vector<std::unique_ptr<VarDeclNode>> varDecls,
					  std::vector<std::unique_ptr<ProcDeclNode>> procDecls,
					  std::unique_ptr<CompoundNode> compound)
				: m_VarDecls(std::move(varDecls)), m_ProcDecls(std::move(procDecls)),
				  m_Compound(std::move(compound))
			{}
			
			std::vector<std::unique_ptr<VarDeclNode>> const& getVarDecls() const
			{ return m_VarDecls; }

			std::vector<std::unique_ptr<ProcDeclNode>> const& getProcDecls() const
			{ return m_ProcDecls; }
			
			CompoundNode const& getCompound() const
			{ return *m_Compound; }

			void accept(Visitor* visitor) const
			{
				visitor->visitBlockNode(*this);
			}
		private:
		    std::vector<std::unique_ptr<VarDeclNode>> m_VarDecls;
			std::vector<std::unique_ptr<ProcDeclNode>> m_ProcDecls;
		    std::unique_ptr<CompoundNode> m_Compound;
		};

		class VarDeclNode : public Node
		{
		public:
			VarDeclNode(std::unique_ptr<VariableNode> var, std::unique_ptr<TypeNode> type)
				: m_Var(std::move(var)), m_Type(std::move(type)) {}

			VariableNode const& getVar() const { return *m_Var; }
			TypeNode const& getType() const { return *m_Type; }

			void accept(Visitor* visitor) const
			{
				visitor->visitVarDeclNode(*this);
			}
		private:
			std::unique_ptr<VariableNode> m_Var;
			std::unique_ptr<TypeNode> m_Type;
		};

		class ParamNode : public Node
		{
		public:
		    ParamNode(std::unique_ptr<VariableNode> var, std::unique_ptr<TypeNode> type)
				: m_Var(std::move(var)), m_Type(std::move(type)) {}

			VariableNode const& getVar() const { return *m_Var; }
			TypeNode const& getType() const { return *m_Type; }

			void accept(Visitor* visitor) const
			{
				visitor->visitParamNode(*this);
			}
		private:
			std::unique_ptr<VariableNode> m_Var;
			std::unique_ptr<TypeNode> m_Type;
		};
		
		class ProcDeclNode : public Node
		{
		public:
			ProcDeclNode(Token name, std::vector<std::unique_ptr<ParamNode>> params,
						 std::unique_ptr<BlockNode> block)
				: m_Name(name), m_Params(std::move(params)), m_Block(std::move(block)) {}

		    Token getProcName() const
			{ return m_Name; }

			std::vector<std::unique_ptr<ParamNode>> const& getParams() const
			{ return m_Params; }
			
			BlockNode const& getBlock() const
			{ return *m_Block; }

			void accept(Visitor* visitor) const
			{
				visitor->visitProcDeclNode(*this);
			}

		private:
		    Token_t m_Name;
		    std::vector<std::unique_ptr<ParamNode>> m_Params;
			std::unique_ptr<BlockNode> m_Block;
		};
		
		class VariableNode : public Node
		{
		public:
			VariableNode(Token name)
				: m_Name(name) {}

		    Token getToken() const { return m_Name; }

			void accept(Visitor* visitor) const
			{
				visitor->visitVariableNode(*this);
			}
		private:
			Token_t m_Name;
		};

		class TypeNode : public Node
		{
		public:
			TypeNode(Token name)
				: m_Name(name) {}

		    Token getToken() const { return m_Name; }

			void accept(Visitor* visitor) const
			{
				visitor->visitTypeNode(*this);
			}
		private:
		    Token_t m_Name;
		};

		class StatementNode : public Node
		{
			
		};

		class CompoundNode : public StatementNode
		{
		public:
			CompoundNode(std::vector<std::unique_ptr<StatementNode>> statements)
				: m_Statements(std::move(statements))
			{}
			
			std::vector<std::unique_ptr<StatementNode>> const& getStatements() const
			{ return m_Statements; }

			void accept(Visitor* visitor) const
			{
				visitor->visitCompoundNode(*this);
			}
		private:
		    std::vector<std::unique_ptr<StatementNode>> m_Statements;
		};
		
		class AssignmentNode : public StatementNode
		{
		public:
			AssignmentNode(std::unique_ptr<VariableNode> var, std::unique_ptr<Node> expr)
				: m_Var(std::move(var)), m_Expr(std::move(expr)) {}

			VariableNode const& getVar() const { return *m_Var; }
			Node const& getExpr() const { return *m_Expr; }

			void accept(Visitor* visitor) const
			{
				visitor->visitAssignmentNode(*this);
			}
		private:
			std::unique_ptr<VariableNode> m_Var;
			std::unique_ptr<Node> m_Expr;
		};

		class NullStatementNode : public StatementNode
		{
		public:
			void accept(Visitor* visitor) const
			{
				visitor->visitNullStatementNode(*this);
			}
		};

		class BinOpNode : public Node
		{
		public:
			BinOpNode(std::unique_ptr<Node> left, std::unique_ptr<Node> right, Token operation)
				: m_Left(std::move(left)), m_Right(std::move(right)),
				  m_Operation(operation) {}

			Node const& getLeft() const { return *m_Left; }
			Node const& getRight() const { return *m_Right; }
			Token getOperation() const { return m_Operation; }

			void accept(Visitor* visitor) const
			{
				visitor->visitBinOpNode(*this);
			}
		private:
			std::unique_ptr<Node> m_Left;
			std::unique_ptr<Node> m_Right;
			Token_t m_Operation;
		};

		class UnaryOpNode : public Node
		{
		public:
			UnaryOpNode(std::unique_ptr<Node> node, Token operation)
				: m_Node(std::move(node)), m_Operation(operation) {}
			
			Node const& getExpr() const { return *m_Node; }
			Token getOperation() const { return m_Operation; }

			void accept(Visitor* visitor) const
			{
				visitor->visitUnaryOpNode(*this);
			}
		private:
			std::unique_ptr<Node> m_Node;
			Token_t m_Operation;
		};

		class NumberNode : public Node
		{
		public:
			NumberNode(Token number)
				: m_Number(number) {}

			Token getToken() const { return m_Number; }

			void accept(Visitor* visitor) const
			{
				visitor->visitNumberNode(*this);
			}
		private:
			Token_t m_Number;
		};

		class ProcCallNode : public StatementNode
		{
		public:
			ProcCallNode(Token procName,
						 std::vector<std::unique_ptr<Node>> procArgs)
				: m_Name(procName), m_Args(std::move(procArgs)) {}

			Token getProcName() const { return m_Name; }
			std::vector<std::unique_ptr<Node>> const&
			getArguments() const { return m_Args; }

			void accept(Visitor* visitor) const
			{
				visitor->visitProcCallNode(*this);
			}
		private:
			Token_t m_Name;
		    std::vector<std::unique_ptr<Node>> m_Args;
		};
	}
}

#endif
