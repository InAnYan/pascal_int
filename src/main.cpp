#include "Interpreter.hpp"
#include <pscpch.hpp>
#include <fstream>

#include <Lexer.hpp>
#include <Parser.hpp>
#include <ReportsManager.hpp>

#include <SimpleEvalVisitor.hpp>
#include <SemanticAnalyzer.hpp>
#include <GraphvizVisitor.hpp>
#include <CodePrettifier.hpp>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

using namespace std;

int main(int argc, char* argv[])
{
	vector<string> args;
	for (unsigned i = 1; i < argc; i++)
	{
		args.push_back(string(argv[i]));
	}
	
	if (args.empty())
	{
		cout << TermColor::BrightRed << "error" << TermColor::BrightWhite <<
			": no files specified" << TermColor::Reset << endl;
		return 1;
	}

	if (find(args.begin(), args.end(), "-h") != args.end() ||
		find(args.begin(), args.end(), "--help") != args.end())
	{
		cout << "TODO: Usage";
		return 0;
	}
	
	Pascal::ReportsManager::Init(args);

	// TODO: Support multiple files
	string inFileName;
	for (auto const& arg : args)
	{
		if (arg.rfind("-", 0) != 0)
		{
			inFileName = arg;
			break;
		}
	}
	
	ifstream fin(inFileName);
	if (!fin.is_open())
	{
	    cout << TermColor::BrightRed << "error" << TermColor::BrightWhite <<
			": can't open file \"" << args[0] << "\"" << TermColor::Reset << endl;
		return 2;
	}

	shared_ptr<string> prg = make_shared<string>();
	
	{
		stringstream ss;
		ss << fin.rdbuf();
		*prg = ss.str();
	}
	
	unique_ptr<Pascal::AST::ProgramNode> tree;

	Pascal::ReportsManager::SetCurrentFile({ inFileName, prg });
	
	try
	{
		{
			Pascal::TokenList tokens;
			tokens = Pascal::Tokenize(prg);
			{
				Pascal::Parser parser(tokens);
				tree = parser.parseProgram();
			}
		}

		Pascal::SemanticAnalyzer symTab;
		tree->accept(&symTab);

		if (Pascal::ReportsManager::GetErrorsCount() == 0)
		{
			Pascal::GraphvizVisitor graph(args[0] + ".dot");
			tree->accept(&graph);
			graph.flush();
			
			string cmd = "dot -Tsvg " + args[0] + ".dot -o " + args[0] + ".svg";
			exec(cmd.c_str());

			cout << symTab.getSymbolTable()->toString();

			Pascal::CodePrettifier pretty;
			tree->accept(&pretty);
			cout << endl << pretty.toString() << endl;

			Pascal::Interpreter interpreter;
		    tree->accept(&interpreter);
		}
	}
	catch (Pascal::StopExecution const& e)
	{
		if (Pascal::ReportsManager::GetWarningsCount() == 0)
		{
			cout << "Generated " << Pascal::ReportsManager::GetErrorsCount() << " errors." << endl;
		}
		else
		{
			cout << "Generated " << Pascal::ReportsManager::GetWarningsCount() <<
				" warnings and " << Pascal::ReportsManager::GetErrorsCount() << " errors." << endl;
		}
		return 1;
	}

	if (Pascal::ReportsManager::GetWarningsCount() > 0)
	{
		if (Pascal::ReportsManager::GetErrorsCount() > 0)
			cout << "Generated " << Pascal::ReportsManager::GetWarningsCount() <<
				" warnings and " << Pascal::ReportsManager::GetErrorsCount() << " errors." << endl;
		else
			cout << "Generated " << Pascal::ReportsManager::GetWarningsCount() << " warnings." << endl;
	}
	else if (Pascal::ReportsManager::GetErrorsCount() > 0)
		cout << "Generated " << Pascal::ReportsManager::GetErrorsCount() << " errors." << endl;
		
	return 0;
}
