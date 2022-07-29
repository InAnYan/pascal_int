#include <pscpch.hpp>
#include <SemanticAnalyzer.hpp>
#include <ReportsManager.hpp>

#include <iostream>

namespace Pascal
{
	SemanticAnalyzer::SemanticAnalyzer()
	{
		m_CurrentScopeLevel = 1;
		m_Symtab = std::make_shared<SymbolTable>("global", m_CurrentScopeLevel, nullptr);
		m_Symtab->initBuiltins();
	}
	
	void SemanticAnalyzer::visitProgramNode(AST::ProgramNode const& node)
	{
		node.getBlock().accept(this);

		for (auto const& sym_p : m_Symtab->getSymbols())
		{
			if (sym_p.second->getType() == SymbolType::VARIABLE)
			{
				if (!(reinterpret_cast<const VariableSymbol*>(sym_p.second.get())->getUsed()))
					ReportsManager::ReportWarning(sym_p.second->getPos(), WarningType::UNUSED_VAR);
			}
		}
	}
	
	void SemanticAnalyzer::visitVarDeclNode(AST::VarDeclNode const& node)
	{
		node.getType().accept(this);
		m_Symtab->define(std::make_shared<VariableSymbol>(
							node.getVar().getToken().str,
							m_Symtab->lookup(node.getType().getToken().str),
							node.getVar().getToken().pos
							));
	}
	
	void SemanticAnalyzer::visitBlockNode(AST::BlockNode const& node)
	{
		for (auto const& e : node.getVarDecls())
			e->accept(this);

		for (auto const& e : node.getProcDecls())
			e->accept(this);
		
		node.getCompound().accept(this);
	}
	
	void SemanticAnalyzer::visitTypeNode(AST::TypeNode const& node)
	{
		if (m_Symtab->lookup(node.getToken().str) == nullptr)
		    ReportsManager::ReportError(node.getToken().pos, ErrorType::NAME_UNDEFINED, true);
	}
	
	void SemanticAnalyzer::visitStatementNode(AST::StatementNode const& node)
	{
		node.accept(this);
	}
	
	void SemanticAnalyzer::visitCompoundNode(AST::CompoundNode const& node)
	{
		for (auto const& e : node.getStatements())
			e->accept(this);
	}
	void SemanticAnalyzer::visitAssignmentNode(AST::AssignmentNode const& node)
	{
		node.getExpr().accept(this);

		if (m_Symtab->lookup(node.getVar().getToken().str) == nullptr)
		{
			ReportsManager::ReportError(node.getVar().getToken().pos, ErrorType::NAME_UNDEFINED);
		}
		else
		{
			std::shared_ptr<Symbol> sym = m_Symtab->change(node.getVar().getToken().str);
			
			if (sym->getType() != SymbolType::VARIABLE)
			{
				ReportsManager::ReportError(node.getVar().getToken().pos, ErrorType::ILLEGAL_ASSIGNMENT);
				ReportsManager::ReportNote(sym->getPos(), "the declaration is here");
			}
			else
			{
				reinterpret_cast<VariableSymbol*>(sym.get())->undirty();
			}
		
			node.getVar().accept(this);
		}
	}
	
	void SemanticAnalyzer::visitVariableNode(AST::VariableNode const& node)
	{
		if (m_Symtab->lookup(node.getToken().str) == nullptr)
		{
			ReportsManager::ReportError(node.getToken().pos, ErrorType::NAME_UNDEFINED, true);
		}
		else
		{
			// Is it guaranteed that 'node.getToken().str' will always be a variable?
			#define VAR_SYM reinterpret_cast<VariableSymbol*>(sym.get())
			
			std::shared_ptr<Symbol> sym = m_Symtab->change(node.getToken().str);
			
			if (m_Symtab->isBelongThisScope(sym) && VAR_SYM->getDirty())
				ReportsManager::ReportWarning(node.getToken().pos, WarningType::UNINTIALIZED_VAR);	
			
			VAR_SYM->beUsed();

			#undef VAR_SYM
		}
	}
	
	void SemanticAnalyzer::visitNullStatementNode(AST::NullStatementNode const& node)
	{
		
	}
	
	void SemanticAnalyzer::visitNumberNode(AST::NumberNode const& node)
	{
		
	}
	
	void SemanticAnalyzer::visitBinOpNode(AST::BinOpNode const& node)
	{
		node.getLeft().accept(this);
		node.getRight().accept(this);
	}
	
	void SemanticAnalyzer::visitUnaryOpNode(AST::UnaryOpNode const& node)
	{
		node.getExpr().accept(this);
	}

	void SemanticAnalyzer::visitProcDeclNode(const AST::ProcDeclNode &node)
	{
		std::unique_ptr<std::vector<std::unique_ptr<VariableSymbol>>> procSymParams =
			std::make_unique<std::vector<std::unique_ptr<VariableSymbol>>>();
		
		for (auto const& e : node.getParams())
		{
			procSymParams->push_back(make_unique<VariableSymbol>(
										 e->getVar().getToken().str,
										 m_Symtab->lookup(e->getType().getToken().str), // ???
										 e->getVar().getToken().pos
										 ));
		}
		
		m_Symtab->define(std::make_shared<ProcedureSymbol>(
							 node.getToken().str,
							 node.getToken().pos,
							 std::move(procSymParams)
							 ));

		std::shared_ptr<SymbolTable> oldScope = m_Symtab;
		m_CurrentScopeLevel++;
		m_Symtab = std::make_shared<SymbolTable>(node.getToken().str, m_CurrentScopeLevel, oldScope);

		for (auto const& e : node.getParams())
		{
			e->accept(this);
		}
							 
		node.getBlock().accept(this);

		std::cout << m_Symtab->toString() << std::endl;

		for (auto const& sym_p : m_Symtab->getSymbols())
		{
			if (sym_p.second->getType() == SymbolType::VARIABLE)
			{
				if (!(reinterpret_cast<const VariableSymbol*>(sym_p.second.get())->getUsed()))
					ReportsManager::ReportWarning(sym_p.second->getPos(), WarningType::UNUSED_VAR);
			}
		}
		
		// This makes no sense
		m_Symtab = m_Symtab->getEnclosingScope();
		m_CurrentScopeLevel--;
	}

	void SemanticAnalyzer::visitParamNode(const AST::ParamNode &node)
	{
		m_Symtab->define(make_unique<VariableSymbol>(
							 node.getVar().getToken().str,
							 m_Symtab->lookup(node.getType().getToken().str),
							 node.getVar().getToken().pos
							 ));	
	}
}
