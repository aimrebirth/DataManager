#include "context.h"

Context::Context(const std::string &indent, const std::string &newline)
    : indent(indent), newline(newline)
{
}

void Context::addText(const std::string &s)
{
    text += s;
}

void Context::addNoNewLine(const std::string &s)
{
    text += space + s;
}

void Context::addLineNoSpace(const std::string & s)
{
    text += s + newline;
}

void Context::addLine()
{
    text += newline;
}

void Context::addLine(const std::string &s)
{
    text += space + s + newline;
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

void Context::beginBlock(const std::string &s, bool indent)
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

void Context::beginFunction(const std::string &s)
{
    beginBlock(s);
}

void Context::endFunction()
{
    endBlock();
    addLine();
}

void Context::beginNamespace(const std::string &s)
{
    addLineNoSpace("namespace " + s);
    addLineNoSpace("{");
    addLine();
    namespaces.push(s);
}

void Context::endNamespace(const std::string &ns)
{
    std::string s = ns;
    if (!namespaces.empty() && ns.empty())
    {
        s = namespaces.top();
        namespaces.pop();
    }
    addLineNoSpace("} // namespace " + s);
}

void Context::ifdef(const std::string &s)
{
    addLineNoSpace("#ifdef " + s);
}

void Context::endif()
{
    addLineNoSpace("#endif");
}

void Context::trimEnd(size_t n)
{
    auto sz = text.size();
    if (n > sz)
        n = sz;
    text.resize(sz - n);
}

std::string Context::getText() const
{
    std::string s;
    if (before_)
        s += before_->getText();
    s += text;
    if (after_)
        s += after_->getText();
    return s;
}

Context &Context::operator+=(const Context &rhs)
{
    if (before_ && rhs.before_)
        before_->text += rhs.before_->text;
    else if (rhs.before_)
    {
        before().addLine();
        before().addText(rhs.before_->text);
    }
    text += rhs.text;
    if (after_ && rhs.after_)
        after_->text += rhs.after_->text;
    else if (rhs.after_)
    {
        //after().addLine();
        after().addText(rhs.after_->text);
    }
    return *this;
}
