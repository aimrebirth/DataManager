#pragma once

#include <memory>
#include <string>
#include <stack>
#include <vector>

class Context
{
public:
    using Text = std::string;
    using Strings = std::vector<std::string>;

public:
    Context(const Text &indent = "    ", const Text &newline = "\n");

    void addLine(const Text &s = Text());
    void addNoNewLine(const Text &s);
    void addLineNoSpace(const Text &s);
    void addText(const Text &s);

    void decreaseIndent();
    void increaseIndent();

    void beginBlock(const Text &s = "", bool indent = true);
    void endBlock(bool semicolon = false);
    void beginFunction(const Text &s = "");
    void endFunction();
    void beginNamespace(const Text &s);
    void endNamespace(const Text &s = Text());

    void ifdef(const Text &s);
    void endif();

    void trimEnd(size_t n);

    Text getText() const;

    Context &before()
    {
        if (!before_)
            before_ = std::make_unique<Context>();
        return *before_;
    }
    Context &after()
    {
        if (!after_)
            after_ = std::make_unique<Context>();
        return *after_;
    }
    
    void emptyLines(int n);

    Context &operator+=(const Context &rhs);

private:
    Strings text;
    std::unique_ptr<Context> before_;
    std::unique_ptr<Context> after_;

    Text space;
    Text indent;
    Text newline;
    std::stack<Text> namespaces;
};

struct ModuleContext
{
    Context cpp;
    Context hpp;

    ModuleContext() {}
    ModuleContext &operator+=(const ModuleContext &rhs)
    {
        cpp += rhs.cpp;
        hpp += rhs.hpp;
        return *this;
    }
};
