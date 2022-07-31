#ifndef PASCAL_CALL_STACK_HPP
#define PASCAL_CALL_STACK_HPP

#include <string>
#include <map>
#include <deque>

namespace Pascal
{
	// AR - Activation Record
	enum class ARType
	{
	    PROGRAM,
		PROCEDURE,
		FUNCTION
	};

	std::string typeToString(ARType type);
	
	typedef struct
	{
		long value;
	} ARObject; // Temporary
	
	class ActivationRecord
	{
	public:
		ActivationRecord(std::string const& name, ARType type, unsigned nestingLevel)
			: m_Name(name), m_Type(type), m_NestingLevel(nestingLevel) {}

		ARObject& operator[](std::string const& name)
		{ return m_Members[name]; }

		std::string const& getName()     const { return m_Name; }
		const ARType getType()           const { return m_Type; }
		const unsigned getNestingLevel() const { return m_NestingLevel; }
		std::string toString()           const;

	private:
		std::string m_Name;
		ARType m_Type;
		unsigned m_NestingLevel;

		std::map<std::string, ARObject> m_Members;
	};

	class CallStack
	{
	public:
		void push(ActivationRecord const& record)
		{
			m_Records.push_back(record);
		}

		void pop()
		{
			m_Records.pop_back();
		}

	    ActivationRecord& peek()
		{
			return m_Records.back();
		}

		std::string toString() const;
		
	private:
		std::deque<ActivationRecord> m_Records;
	};
}    

#endif
