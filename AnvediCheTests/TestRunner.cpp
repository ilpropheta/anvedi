#include "TestRunner.h"
#include <windows.h>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

QStringList FromCmdArgs(int argc, char** argv)
{
	QStringList cmd;
	for (auto i = 0; i < argc; ++i)
	{
		cmd.push_back(argv[i]);
	}
	return cmd;
}

int TestRunner::RunAll(int argc, char** argv)
{
	return RunAll(FromCmdArgs(argc, argv));
}

int TestRunner::RunAll(const QStringList& cmd)
{
	int errorCode = 0;
	for (auto test : m_tests)
	{
		errorCode |= QTest::qExec(test, cmd);
		std::cout << std::endl;
	};
	if (m_tests.empty())
		cout << "Sad test project with no tests... :(" << endl;

	return errorCode;
}

enum Color { blue = 1, green, cyan, red, purple, yellow, grey, dgrey, hblue, hgreen, hcyan, hred, hpurple, hyellow, hwhite };

void SetConsoleColor(Color color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD) color);
}

WORD GetConsoleColor()
{
	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO con_info;
	GetConsoleScreenBufferInfo(consoleHandle, &con_info);
	return con_info.wAttributes;
}

class ColorGuard
{
public:
	ColorGuard() : m_color(GetConsoleColor()) {}

	~ColorGuard()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_color);
	}

private:
	WORD m_color;
};


int TestRunner::RunAllColorized(int argc, char** argv)
{
	auto cmd = FromCmdArgs(argc, argv);
	cmd.push_back("-o");
	cmd.push_back("tmpLog,txt");
	auto exitCode = RunAll(cmd);
	
	ifstream file("tmpLog");
	string line;
	const regex fail("(FAIL.*)");
	const regex pass("(PASS.*)");

	while (getline(file, line))
	{
		ColorGuard guard;
		if (regex_match(line, fail))
		{
			SetConsoleColor(hred);
		}
		if (regex_match(line, pass))
		{
			SetConsoleColor(hgreen);
		}
		cout << line << endl;
	}

	if (exitCode)
	{
		ColorGuard guard;
		SetConsoleColor(hred);
		cout << ">>>>> TESTS FAILED!!!" << endl;
	}

	return exitCode;
}

void TestRunner::RegisterTest(QObject* test)
{
	auto testName = test->objectName();

	if (!std::count_if(begin(m_tests), end(m_tests), [&testName](QObject* elem) {
		return elem->objectName() == testName;
	}))
	{
		m_tests.push_back(test);
	}
}

TestRunner& TestRunner::Instance()
{
	static TestRunner instance;
	return instance;
}