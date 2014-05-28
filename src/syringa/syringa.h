#ifndef SYRINGA_H
#define SYRINGA_H

#include <utility>
#include <functional>
#include <list>
#include <sstream>
#include "export.h"

namespace Syringa {

class SYRINGA_API Buffer {
public:
    struct SYRINGA_API LineWriter {
        LineWriter(std::ostream& stream);
        LineWriter(const LineWriter&) = delete;
        void operator=(const LineWriter&) = delete;
        LineWriter(LineWriter&&);
        ~LineWriter();
        std::ostream& stream();
    private:
        std::ostream* m_stream = nullptr;
    };

    Buffer();
    Buffer(const Buffer&) = delete;
    void operator=(const Buffer&) = delete;
    ~Buffer();
    void clear();
    LineWriter write();
    std::string content();

private:
    std::ostringstream m_stream;
};

template<typename T>
class Expectation {
public:
    explicit Expectation(T&& value, Buffer& logger)
        : m_value(std::forward<T>(value))
        , m_logger(logger)
    {
    }
    void toEqual(const T& t)
    {
        if (m_value != t)
            m_logger.write().stream() << "Expected " << m_value << " to equal " << t;
    }

private:
    T m_value;
    Buffer& m_logger;
};

class SYRINGA_API TestRunner {
public:
    typedef std::function<void()> Function;
    typedef std::list<std::pair<std::string, Function>> GroupList;
    typedef std::list<std::string> CallStack;
    typedef std::function<void (const CallStack&, const std::string& messages)> Output;

    static TestRunner& instance();

    void setOutput(Output);
    void run(const Function& tests);

    void describe(std::string name, Function group);
    void it(std::string name, Function testCase);

    template<typename T>
    Expectation<T> expect(T&& t)
    {
        return Expectation<T>(std::forward<T>(t), m_logger);
    }

private:
    TestRunner();
    TestRunner(const TestRunner&) = delete;
    void operator=(const TestRunner&) = delete;

private:
    void runGroups();

private:
    Buffer m_logger;
    Output m_output;

    std::list<GroupList> m_groupStack;
    CallStack m_callStack;

    bool m_isInsideTestCase = false;
};

SYRINGA_API void setOutput(TestRunner::Output);
SYRINGA_API void run(const TestRunner::Function& tests);
SYRINGA_API void describe(std::string name, TestRunner::Function group);
SYRINGA_API void it(std::string name, TestRunner::Function testCase);

template<typename T>
Expectation<T> expect(T&& t)
{
    return TestRunner::instance().expect(std::forward<T>(t));
}

} // namespace Syringa

#endif // SYRINGA_H
