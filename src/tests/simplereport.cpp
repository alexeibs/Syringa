#include "simplereport.h"

namespace Syringa {
namespace Tests {

void SimpleReport::write(const std::list<std::string> &callStack, const std::string &messages)
{
    m_content.append("\n---\n");
    if (!callStack.empty()) {
        auto it = callStack.begin();
        m_content.append(*it);
        for (auto itEnd = callStack.end(); ++it != itEnd;) {
            m_content.append(" - ");
            m_content.append(*it);
        }
        m_content.append("\n\n");
    }
    m_content.append(messages);
    m_content.append("\n");
}

void SimpleReport::clear()
{
    m_content.clear();
}

}} // namespace Syringa::Tests
