#include <pscpch.hpp>
#include <AST.hpp>
#include <CodePrettifier.hpp>

namespace Pascal
{
	void CodePrettifier::visitProgramNode(AST::ProgramNode const& node)
	{
		ss << "program " << node.getName().str << ";" << std::endl << std::endl;
		node.getBlock().accept(this);
		ss.seekp(-1, ss.cur);
		ss << "." << std::endl;
	}
	
	void CodePrettifier::visitVarDeclNode(AST::VarDeclNode const& node)
	{
		ss << node.getVar().getToken().str << " : " <<
			node.getType().getToken().str << ";";
	}
	
	void CodePrettifier::visitBlockNode(AST::BlockNode const& node)
	{
		for (auto const& e : node.getVarDecls())
		{
			ss << std::string(currentScopeLevel*4, ' ') << "var ";
			e->accept(this);
			ss << std::endl;
		}

		for (auto const& e : node.getProcDecls())
		{
			ss << std::endl << std::string(currentScopeLevel*4, ' ');
			e->accept(this);
		}

		node.getCompound().accept(this);
	}
	
	void CodePrettifier::visitTypeNode(AST::TypeNode const& node)
	{
		
	}
	
	void CodePrettifier::visitStatementNode(AST::StatementNode const& node)
	{
		node.accept(this);
	}
	
	void CodePrettifier::visitCompoundNode(AST::CompoundNode const& node)
	{
		ss << std::endl << std::string((currentScopeLevel-1)*4, ' ') << "begin" << std::endl;
		for (auto const& e : node.getStatements())
		{
			ss << std::string(currentScopeLevel*4, ' ');
			e->accept(this);
		}
		ss << std::endl << std::string((currentScopeLevel-1)*4, ' ') << "end;";
	}
	
	void CodePrettifier::visitAssignmentNode(AST::AssignmentNode const& node)
	{
		node.getVar().accept(this);
		ss << " := ";
		node.getExpr().accept(this);
		ss << ";" << std::endl;
	}
	
	void CodePrettifier::visitVariableNode(AST::VariableNode const& node)
	{
		ss << node.getToken().str;
	}
	
	void CodePrettifier::visitNullStatementNode(AST::NullStatementNode const& node)
	{
		ss.seekp(-1, ss.cur);
	}
	
	void CodePrettifier::visitNumberNode(AST::NumberNode const& node)
	{
		ss << node.getToken().str;
	}
	
	void CodePrettifier::visitBinOpNode(AST::BinOpNode const& node)
	{
		node.getLeft().accept(this);
	    ss << " " << node.getOperation().str << " ";
		node.getRight().accept(this);
	}
	
	void CodePrettifier::visitUnaryOpNode(AST::UnaryOpNode const& node)
	{
		ss << node.getOperation().str;
		node.getExpr().accept(this);
	}
	
	void CodePrettifier::visitProcDeclNode(AST::ProcDeclNode const& node)
	{
		ss << "procedure " << node.getProcName().str;
		if (!node.getParams().empty())
		{
			ss << "(";
			for (unsigned i = 0; i < node.getParams().size(); i++)
			{
				node.getParams()[i]->accept(this);
				if (i != node.getParams().size() - 1)
					ss << "; ";
			}
			ss << ")";
		}
		ss << ";" << std::endl;

		currentScopeLevel++;
		node.getBlock().accept(this);
		ss << " { END OF \"" << node.getProcName().str << "\" }" << std::endl;
		currentScopeLevel--;
	}
	
	void CodePrettifier::visitParamNode(const AST::ParamNode &node)
	{
		ss << node.getVar().getToken().str << currentScopeLevel << " : " <<
			node.getType().getToken().str;
	}

	void CodePrettifier::visitProcCallNode(const AST::ProcCallNode& node)
	{
		ss << node.getProcName().str << "(";
		if (!node.getArguments().empty())
		{
			for (unsigned i = 0; i < node.getArguments().size(); i++)
			{
				node.getArguments()[i]->accept(this);
				if (i != node.getArguments().size() - 1)
					ss << ", ";
			}
		}
		ss << ");";
	}

	std::string CodePrettifier::toString() const
	{
		return ss.str();
	}
}
