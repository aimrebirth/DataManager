#include "context.h"

Context::Strings &operator+=(Context::Strings &s1, const Context::Strings &s2)
{
    s1.insert(s1.end(), s2.begin(), s2.end());
    return s1;
}

Context::Strings operator+(const Context::Strings &s1, const Context::Strings &s2)
{
    Context::Strings s(s1.begin(), s1.end());
    return s += s2;
}

Context::Context(const Text &indent, const Text &newline)
    : indent(indent), newline(newline)
{
}

void Context::addText(const Text &s)
{
    text.back() += s;
}

void Context::addNoNewLine(const Text &s)
{
    text.push_back(space + s);
}

void Context::addLineNoSpace(const Text & s)
{
    text.push_back(s);
}

void Context::addLine(const Text &s)
{
    if (s.empty())
        text.push_back(s);
    else
        text.push_back(space + s);
}

void Context::decreaseIndent()
{
    if (space.size() >= indent.size())
        space.resize(space.size() - indent.size());
}

void Context::increaseIndent()
{
    space += indent;
}

void Context::beginBlock(const Text &s, bool indent)
{
    if (!s.empty())
        addLine(s);
    addLine("{");
    if (indent)
        increaseIndent();
}

void Context::endBlock(bool semicolon)
{
    decreaseIndent();
    addLine(semicolon ? "};" : "}");
}

void Context::beginFunction(const Text &s)
{
    beginBlock(s);
}

void Context::endFunction()
{
    endBlock();
    addLine();
}

void Context::beginNamespace(const Text &s)
{
    addLineNoSpace("namespace " + s);
    addLineNoSpace("{");
    addLine();
    namespaces.push(s);
}

void Context::endNamespace(const Text &ns)
{
    Text s = ns;
    if (!namespaces.empty() && ns.empty())
    {
        s = namespaces.top();
        namespaces.pop();
    }
    addLineNoSpace("} // namespace " + s);
}

void Context::ifdef(const Text &s)
{
    addLineNoSpace("#ifdef " + s);
}

void Context::endif()
{
    addLineNoSpace("#endif");
}

void Context::trimEnd(size_t n)
{
    if (text.empty())
        return;
    auto &t = text.back();
    auto sz = t.size();
    if (n > sz)
        n = sz;
    t.resize(sz - n);
}

Context::Text Context::getText() const
{
    Text s;
    if (before_)
        s += before_->getText();
    for (auto &str : text)
        s += str + newline;
    s.resize(s.size() - 1);
    if (after_)
        s += after_->getText();
    return s;
}

void Context::emptyLines(int n)
{
    int e = 0;
    for (auto i = text.rbegin(); i != text.rend(); ++i)
    {
        if (i->empty())
            e++;
        else
            break;
    }
    if (e < n)
    {
        while (e++ != n)
            addLine();
    }
    else if (e > n)
    {
        text.resize(text.size() - (e - n));
    }
}

Context &Context::operator+=(const Context &rhs)
{
    if (before_ && rhs.before_)
        before_->text += rhs.before_->text;
    else if (rhs.before_)
    {
        before().emptyLines(1);
        before().text += rhs.before_->text;
    }
    text += rhs.text;
    if (after_ && rhs.after_)
        after_->text += rhs.after_->text;
    else if (rhs.after_)
    {
        after().emptyLines(1);
        after().text += rhs.after_->text;
    }
    return *this;
}
