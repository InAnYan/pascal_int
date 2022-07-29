#include <pscpch.hpp>
#include <Symbols.hpp>
#include <ReportsManager.hpp>

#include <iomanip>
#include <cassert>

namespace Pascal
{
	SymbolTable::SymbolTable(std::string const& scopeName, unsigned scopeLevel,
							 std::shared_ptr<SymbolTable> enclosingScope)
		: m_ScopeName(scopeName), m_ScopeLevel(scopeLevel), m_EnclosingScope(enclosingScope)
	{
	}
	
	void SymbolTable::initBuiltins()
	{
		m_Symbols["integer"] = std::make_shared<BuiltInTypeSymbol>("integer");
		m_Symbols["real"] = std::make_shared<BuiltInTypeSymbol>("real");
	}
	
	void SymbolTable::define(std::shared_ptr<Symbol> sym)
	{
		if (m_Symbols.count(sym->getName()))
			ReportsManager::ReportError(sym->getPos(), ErrorType::NAME_REDEFINITION);
		else
			m_Symbols[sym->getName()] = sym;
	}
	
    std::shared_ptr<const Symbol> SymbolTable::lookup(const std::string &name) const
	{
		if (!m_Symbols.count(name))
		{
			if (m_EnclosingScope != nullptr)
			    return m_EnclosingScope->lookup(name);
			else
				return nullptr;
		}
		else
		{
			return m_Symbols.at(name);
		}
	}

    std::shared_ptr<Symbol> SymbolTable::change(const std::string &name)
	{
		assert(lookup(name) != nullptr); // Is it needed?

		if (!m_Symbols.count(name))
		{
			if (m_EnclosingScope != nullptr)
			    return m_EnclosingScope->change(name);
			else
				return nullptr;
		}
		else
		{
			return m_Symbols.at(name);
		}
	}

	bool SymbolTable::isBelongThisScope(std::shared_ptr<Symbol> sym) const
	{
	    for (auto const& sym_p : m_Symbols)
		{
			if (sym_p.second == sym)
				return true;
		}
		
		return false;
	}

	std::string SymbolTable::toString() const
	{
		std::stringstream ss;

		ss << "SYMBOL TABLE" << std::endl;
		ss << "================================" << std::endl;
		ss << "SymbolTable name: \"" << m_ScopeName << "\"" << std::endl;
		ss << "Enclosing Scope: \"" <<
			((m_EnclosingScope == nullptr) ? ("__none__") : (m_EnclosingScope->getName())) <<
			"\"" << std::endl;
		ss << "Scope level: " << m_ScopeLevel << std::endl;
		ss << "Contents: " << std::endl;
		ss << "--------------------------------" << std::endl;
		
		for (auto const& sym_p : m_Symbols)
		{
			ss << std::setw(11) << sym_p.second->getName() << ": " << sym_p.second->toString() << std::endl;
		}

		return ss.str();
	}
}
