#pragma once

#include <QTest>
#include <algorithm>
#include <list>
#include <QSharedPointer>
#include <iostream>
#include <functional>

class QStringList;

//////////////////////////////////////////////////////////////////////////
// Test Runner allows automatic execution of tests
class TestRunner
{
public:
	static TestRunner& Instance();
	void RegisterTest(QObject* test);
	int RunAll(int argc, char** argv);
	int RunAllColorized(int argc, char** argv);
private:
	int RunAll(const QStringList& cmd, std::function<void(QObject* test)> fn = [](QObject*){ std::cout << std::endl; });
	std::list<QObject*> m_tests;
};

//////////////////////////////////////////////////////////////////////////
//Support structure
template <class T>
class Test
{
public:
	QSharedPointer<T> child;

	Test(const QString& name) : child(new T)
	{
		child->setObjectName(name);
		TestRunner::Instance().RegisterTest(child.data());
	}
};


// Use this macro after your test declaration (in the header file)
#define DECLARE_TEST(className) static Test<className> t(#className);

// Use this macro to execute all tests
#define RUN_ALL_TESTS(argc, argv) TestRunner::Instance().RunAll(argc, argv)

// Use this macro to execute all tests (colored out)
#define RUN_ALL_TESTS_COLOR(argc, argv) TestRunner::Instance().RunAllColorized(argc, argv)