#include <pscpch.hpp>
#include <AST.hpp>
#include <SimpleEvalVisitor.hpp>

#include <stdexcept>

namespace Pascal
{
	using namespace AST;

	void SimpleEvalVisitor::visitProgramNode(AST::ProgramNode const& node)
	{
	    node.getBlock().accept(this);
	}
	
	void SimpleEvalVisitor::visitVarDeclNode(AST::VarDeclNode const& node)
	{
		vars[node.getVar().getToken().str] = 0xff;
	}

	void SimpleEvalVisitor::visitBlockNode(AST::BlockNode const& node)
	{
		for (auto const& e : node.getVarDecls())
		{
			e->accept(this);
		}

		for (auto const& e : node.getProcDecls())
		{
			e->accept(this);
		}
		
	    node.getCompound().accept(this);
	}

	void SimpleEvalVisitor::visitTypeNode(AST::TypeNode const& node)
	{
		
	}

	void SimpleEvalVisitor::visitStatementNode(AST::StatementNode const& node)
	{
		node.accept(this);
	}

	void SimpleEvalVisitor::visitCompoundNode(AST::CompoundNode const& node)
	{
		for (auto const& e : node.getStatements())
		{
			e->accept(this);
		}
	}

	void SimpleEvalVisitor::visitAssignmentNode(AST::AssignmentNode const& node)
	{
		node.getExpr().accept(this);
		vars[node.getVar().getToken().str] = acc;
	}

	void SimpleEvalVisitor::visitVariableNode(AST::VariableNode const& node)
	{
		acc = vars[node.getToken().str];
	}

	void SimpleEvalVisitor::visitNullStatementNode(AST::NullStatementNode const& node)
	{
		acc = 0;
	}

	void SimpleEvalVisitor::visitNumberNode(AST::NumberNode const& node)
	{
		acc = std::stof(node.getToken().str);
	}

	void SimpleEvalVisitor::visitBinOpNode(AST::BinOpNode const& node)
	{
		node.getLeft().accept(this);
		float left = acc;
		node.getRight().accept(this);
		float right = acc;

		switch (node.getOperation().type)
		{
		case TokenType::PLUS:
			acc = left + right;
			break;
		case TokenType::MINUS:
			acc = left - right;
			break;
		case TokenType::PRODUCT:
			acc = left * right;
			break;
		case TokenType::DIVISION:
			acc = left / right;
			break;
		case TokenType::MOD:
			acc = left / right;
			break;
		default:
			throw std::runtime_error("binary unbelivable");
		}
	}

	void SimpleEvalVisitor::visitUnaryOpNode(AST::UnaryOpNode const& node)
	{
		node.getExpr().accept(this);

		switch (node.getOperation().type)
		{
		case TokenType::PLUS:
			acc = +acc;
			break;
		case TokenType::MINUS:
			acc = -acc;
			break;
		default:
			throw std::runtime_error("unary unbelivable");
		}
	}

	void SimpleEvalVisitor::visitProcDeclNode(const AST::ProcDeclNode &node)
	{
		
	}

	void SimpleEvalVisitor::visitParamNode(const AST::ParamNode &node)
	{
		
	}

	void SimpleEvalVisitor::visitProcCallNode(const AST::ProcCallNode& node)
	{
		
	}
}
