#include "syringa.h"

using namespace std;

namespace Syringa {

TestRunner& TestRunner::instance()
{
    static TestRunner single;
    return single;
}

void TestRunner::setOutput(TestRunner::Output output)
{
    m_output = std::move(output);
}

void TestRunner::run(const TestRunner::Function &tests) {
    m_groupStack.emplace_back(GroupList());
    tests();
    runGroups();
}

void TestRunner::describe(string name, TestRunner::Function group)
{
    if (m_isInsideTestCase) {
        m_logger.write().stream() << "Syntax error - unexpected \"describe\"";
        return;
    }
    m_groupStack.back().emplace_back(std::move(name), std::move(group));
}

void TestRunner::it(string name, TestRunner::Function testCase)
{
    if (m_isInsideTestCase) {
        m_logger.write().stream() << "Syntax error - unexpected \"it\"";
        return;
    }
    m_isInsideTestCase = true;
    m_logger.clear();
    m_callStack.emplace_back(std::move(name));
    testCase();
    auto messages = m_logger.content();
    if (!messages.empty())
        m_output(m_callStack, messages);
    m_callStack.pop_back();
    m_isInsideTestCase = false;
}

TestRunner::TestRunner()
{
    setOutput([](const CallStack&, const string&){});
}

void TestRunner::runGroups()
{
    if (m_groupStack.empty())
        return;

    auto& groups = m_groupStack.back();
    if (!groups.empty()) {
        for (auto& group : groups) {
            m_callStack.emplace_back(std::move(group.first));
            m_groupStack.emplace_back(GroupList());
            group.second();
            runGroups();
            m_callStack.pop_back();
        }
    }
    m_groupStack.pop_back();
}

Buffer::LineWriter::LineWriter(ostream& stream)
    : m_stream(&stream)
{
}

Buffer::LineWriter::LineWriter(Buffer::LineWriter&& other)
    : m_stream(other.m_stream)
{
    other.m_stream = nullptr;
}

Buffer::LineWriter::~LineWriter()
{
    if (m_stream)
        *m_stream << std::endl;
}

ostream& Buffer::LineWriter::stream()
{
    return *m_stream;
}

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
}

void Buffer::clear()
{
    m_stream.str("");
    m_stream.clear();
    m_stream.seekp(0);
}

Buffer::LineWriter Buffer::write()
{
    return LineWriter(m_stream);
}

string Buffer::content()
{
    return m_stream.str();
}

void run(const TestRunner::Function& tests)
{
    TestRunner::instance().run(tests);
}

void setOutput(TestRunner::Output output)
{
    TestRunner::instance().setOutput(std::move(output));
}

void describe(string name, TestRunner::Function group)
{
    TestRunner::instance().describe(std::move(name), std::move(group));
}

void it(string name, TestRunner::Function testCase)
{
    TestRunner::instance().it(std::move(name), std::move(testCase));
}

} // namespace Syringa
