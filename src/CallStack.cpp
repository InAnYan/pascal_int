#include <pscpch.hpp>
#include <CallStack.hpp>

namespace Pascal
{
	std::string typeToString(ARType type)
	{
		switch (type)
		{
		case ARType::PROGRAM:
			return "PROGRAM";
		case ARType::FUNCTION:
			return "FUNCTION";
		case ARType::PROCEDURE:
			return "PROCEDURE";
		}
	}
	
	std::string ActivationRecord::toString() const
	{
		std::stringstream ss;

		ss << m_NestingLevel << ": " << typeToString(m_Type)
		   << " " << m_Name << std::endl;

		for (auto const& p : m_Members)
		{
			ss << "-- " << std::setw(8) << p.first << " : " <<
				p.second.value << std::endl;
		}
		
		return ss.str();
	}

	std::string CallStack::toString() const
	{
		std::stringstream ss;

		ss << "CALL STACK: " << std::endl;

		for (auto const& e : m_Records)
		{
			ss << e.toString();

		}
		return ss.str();
	}
}    
