#include "TestRunner.h"

using namespace std;

int TestRunner::RunAll(int argc, char *argv [])
{
	int errorCode = 0;
	for (auto test : m_tests)
	{
		errorCode |= QTest::qExec(test, argc, argv);
		std::cout << std::endl;
	};
	if (m_tests.empty())
		cout << "Sad test project with no tests... :(" << endl;

	return errorCode;
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
