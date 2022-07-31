#include <pscpch.hpp>
#include "CallStack.hpp"
#include "Lexer.hpp"
#include "ReportsManager.hpp"
#include <Interpreter.hpp>
#include <AST.hpp>
#include <pthread.h>
#include <stdexcept>

namespace Pascal
{
	void Interpreter::visitProgramNode(AST::ProgramNode const& node)
	{
		m_CallStack.push(ActivationRecord(node.getName().str, ARType::PROGRAM, 1));
		node.getBlock().accept(this);
		std::cout << m_CallStack.toString() << std::endl;
		m_CallStack.pop();
	}
	
	void Interpreter::visitVarDeclNode(AST::VarDeclNode const& node)
	{ }

	void Interpreter::visitBlockNode(AST::BlockNode const& node)
	{
		node.getCompound().accept(this);
	}

	void Interpreter::visitTypeNode(AST::TypeNode const& node)
	{ }

	void Interpreter::visitStatementNode(AST::StatementNode const& node)
	{ }

	void Interpreter::visitCompoundNode(AST::CompoundNode const& node)
	{
		for (auto const& e : node.getStatements())
			e->accept(this);
	}

	void Interpreter::visitAssignmentNode(AST::AssignmentNode const& node)
	{
		node.getExpr().accept(this);
		m_CallStack.peek()[node.getVar().getToken().str] = m_Accum;
	}

	void Interpreter::visitVariableNode(AST::VariableNode const& node)
	{
		m_Accum = m_CallStack.peek()[node.getToken().str];
	}

	void Interpreter::visitNullStatementNode(AST::NullStatementNode const& node)
	{ }

	void Interpreter::visitNumberNode(AST::NumberNode const& node)
	{
		m_Accum.value = std::stol(node.getToken().str);
	}

	void Interpreter::visitBinOpNode(AST::BinOpNode const& node)
	{
		node.getLeft().accept(this);
		ARObject left = m_Accum;
		node.getRight().accept(this);
		ARObject right = m_Accum;
		switch (node.getOperation().type)
		{
		case TokenType::PLUS:
			m_Accum.value = left.value + right.value;
			break;
		case TokenType::MINUS:
			m_Accum.value = left.value - right.value;
			break;
		case TokenType::PRODUCT:
			m_Accum.value = left.value * right.value;
			break;
		case TokenType::DIVISION:
			if (right.value == 0)
			{
				ReportsManager::ReportError(node.getOperation().pos, ErrorType::DIVISION_BY_ZERO, false);
			}
			else
			{
				m_Accum.value = left.value / right.value;
			}
			break;
		case TokenType::MOD:
			m_Accum.value = left.value % right.value;
			break;
		default:
			throw std::runtime_error("unbelivable");
		}
	}

	void Interpreter::visitUnaryOpNode(AST::UnaryOpNode const& node)
	{
		node.getExpr().accept(this);
		switch (node.getOperation().type)
		{
		case TokenType::PLUS:
			m_Accum.value = +m_Accum.value;
			break;
		case TokenType::MINUS:
			m_Accum.value = -m_Accum.value;
			break;
		default:
			throw std::runtime_error("unbelivable");			
		}
	}

	void Interpreter::visitProcDeclNode(const AST::ProcDeclNode &node)
	{ }

	void Interpreter::visitParamNode(const AST::ParamNode &node)
	{ }

	void Interpreter::visitProcCallNode(const AST::ProcCallNode& node)
	{ }
}    
