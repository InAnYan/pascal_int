#ifndef PASCAL_SYMBOLS_HPP
#define PASCAL_SYMBOLS_HPP

#include <string>
#include <sstream>
#include <map>
#include <memory>
#include <vector>

namespace Pascal
{
	enum class SymbolType
	{
		NONE = 0,
		BUILTIN_TYPE,
		VARIABLE,
		PROCEDURE,
		FUNCTION
	};
    
	class Symbol
	{
	public:
		Symbol(std::string const& name, size_t whereDefined)
			: m_Name(name), m_Pos(whereDefined) {}
		virtual ~Symbol() {}

		std::string const& getName() const { return m_Name; }
		
		size_t getPos() const { return m_Pos; }
		
		virtual const SymbolType getType() const = 0;
		virtual std::string toString() const = 0;
		
	private:
		std::string m_Name;
		size_t m_Pos;
	};

	class BuiltInTypeSymbol : public Symbol
	{
	public:
	    BuiltInTypeSymbol(std::string const& name)
			: Symbol(name, 0) {}

		const SymbolType getType() const { return SymbolType::BUILTIN_TYPE; }

		std::string toString() const
		{
			std::stringstream ss;
			ss << "<BuiltInTypeSymbol(name=\"" << getName() << "\")>";
			return ss.str();
		}
	};

	class VariableSymbol : public Symbol
	{
	public:
		VariableSymbol(std::string const& name,
					   std::shared_ptr<const Symbol> parent,
					   size_t whereDefined)
			: Symbol(name, whereDefined), m_Parent(parent) {}
		
		const SymbolType getType() const
		{ return SymbolType::VARIABLE; }

		std::string toString() const
		{
			std::stringstream ss;
			ss << "<VariableSymbol(name=\"" << getName() <<	"\", type=";
			if (m_Parent == nullptr)
			{
				ss << "nullptr";
			}
			else
			{
				ss << "\"" << m_Parent->getName() << "\"";
			}
			ss << ")>";
			return ss.str();
		}
		
		const bool getDirty() const
		{ return m_Dirty; }

		const bool getUsed() const
		{ return m_Used; }

		std::shared_ptr<const Symbol> getParent() const
		{ return m_Parent; }
		
		void undirty() { m_Dirty = false; }
		void beUsed()  { m_Used = true; }
		
	private:
		bool m_Dirty = true;
		bool m_Used = false;
		std::shared_ptr<const Symbol> m_Parent;
	};

	class ProcedureSymbol : public Symbol
	{
	public:
		ProcedureSymbol(std::string const& name, size_t whereDefined,
					    std::vector<VariableSymbol> const& params)
			: Symbol(name, whereDefined), m_Params(params) {}
		
		const SymbolType getType() const { return SymbolType::PROCEDURE; }
		std::vector<VariableSymbol> const& getArgs() const
		{ return m_Params; }
		
		std::string toString() const
		{
			std::stringstream ss;
			ss << "<ProcedureSymbol(name=\"" << getName() << "\", params=[";
			if (!(m_Params.empty()))
			{
				for (unsigned i = 0; i < m_Params.size(); i++)
				{
					ss << m_Params[i].toString();
					if (i != m_Params.size() - 1)
						ss << ", ";
				}
			}
			ss << "])>";
			return ss.str();
		}

	private:
		std::vector<VariableSymbol> m_Params;
	};
	
	class SymbolTable
	{
	public:
		SymbolTable(std::string const& scopeName, unsigned scopeLevel,
					std::shared_ptr<SymbolTable> enclosingScope);

		void define(std::shared_ptr<Symbol> sym);
		std::shared_ptr<const Symbol> lookup(std::string const& name) const;
	    std::shared_ptr<Symbol> change(std::string const& name);

		bool isBelongThisScope(std::shared_ptr<Symbol> sym) const;
		
		std::string const& getName() const { return m_ScopeName; }
		std::shared_ptr<SymbolTable> getEnclosingScope() { return m_EnclosingScope; }
		std::map<std::string, std::shared_ptr<Symbol>> const&
		getSymbols() const { return m_Symbols; }
		std::string toString() const;
		
		void initBuiltins();
	private:
		std::string m_ScopeName;
		unsigned m_ScopeLevel;
		
		std::map<std::string, std::shared_ptr<Symbol>> m_Symbols;
		std::shared_ptr<SymbolTable> m_EnclosingScope;
	};
}

#endif
