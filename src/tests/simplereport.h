#ifndef SIMPLEREPORT_H
#define SIMPLEREPORT_H

#include <list>
#include <string>

namespace Syringa {
namespace Tests {

class SimpleReport {
public:
    void write(const std::list<std::string>& callStack, const std::string& messages);
    void clear();
    const std::string& content() { return m_content; }

private:
    std::string m_content;
};

}} // namespace Syringa::Tests


#endif // SIMPLEREPORT_H
