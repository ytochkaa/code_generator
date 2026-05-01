#include "csharpfactory.h"

#include <vector>

std::string CSharpClassUnit::accessModifierToString(int accessModifier)
{
    switch (accessModifier) {
    case PUBLIC:
        return "public";
    case PROTECTED:
        return "protected";
    case PRIVATE:
        return "private";
    case INTERNAL:
        return "internal";
    case PROTECTED_INTERNAL:
        return "protected internal";
    case PRIVATE_PROTECTED:
        return "private protected";
    case PACKAGE_PRIVATE:
        return "internal";
    default:
        return "private";
    }
}

std::string CSharpClassUnit::compile(unsigned int level) const
{
    std::string result = generateShift(level);
    if (m_flags & ABSTRACT) {
        result += "abstract ";
    }
    if (m_flags & STATIC) {
        result += "static ";
    }
    if (m_flags & FINAL) {
        result += "sealed ";
    }
    if (m_flags & PARTIAL) {
        result += "partial ";
    }
    result += "class " + m_name;
    if (!m_parent.empty()) {
        result += " : " + m_parent;
    }
    result += " {\n";

    static const std::vector<int> order = {
        PUBLIC, INTERNAL, PROTECTED, PROTECTED_INTERNAL, PRIVATE_PROTECTED, PRIVATE, PACKAGE_PRIVATE
    };
    for (int accessModifier : order) {
        auto it = m_fields.find(accessModifier);
        if (it == m_fields.end() || it->second.empty()) {
            continue;
        }
        for (const auto& field : it->second) {
            result += field->compile(level + 1);
        }
    }

    result += generateShift(level) + "}\n";
    return result;
}

std::string CSharpMethodUnit::compile(unsigned int level) const
{
    std::string result = generateShift(level);
    result += CSharpClassUnit::accessModifierToString(m_accessModifier) + " ";
    if (m_flags & STATIC) {
        result += "static ";
    }
    if (m_flags & ABSTRACT) {
        result += "abstract ";
    }
    if (m_flags & VIRTUAL) {
        result += "virtual ";
    }
    if (m_flags & OVERRIDE) {
        result += "override ";
    }
    if (m_flags & SEALED) {
        result += "sealed ";
    }
    if (m_flags & ASYNC) {
        result += "async ";
    }
    result += m_returnType + " " + m_name + "()";
    if (m_flags & ABSTRACT) {
        result += ";\n";
        return result;
    }
    result += " {\n";
    for (const auto& statement : m_body) {
        result += statement->compile(level + 1);
    }
    result += generateShift(level) + "}\n";
    return result;
}

std::string CSharpPrintOperatorUnit::compile(unsigned int level) const
{
    return generateShift(level) + "Console.WriteLine(\"" + m_text + "\");\n";
}

std::shared_ptr<ClassUnit> CSharpFactory::createClass(const std::string& name, Unit::Flags flags) const
{
    return std::make_shared<CSharpClassUnit>(name, flags);
}

std::shared_ptr<MethodUnit> CSharpFactory::createMethod(const std::string& name, const std::string& returnType, Unit::Flags flags) const
{
    return std::make_shared<CSharpMethodUnit>(name, returnType, flags);
}

std::shared_ptr<PrintOperatorUnit> CSharpFactory::createPrintOperator(const std::string& text) const
{
    return std::make_shared<CSharpPrintOperatorUnit>(text);
}

std::string CSharpFactory::wrapProgram(const std::string& classCode) const
{
    return "using System;\n\n" + classCode + "\nclass Program {\n    static void Main() {\n    }\n}\n";
}

std::string CSharpFactory::fileName() const
{
    return "MyClass.cs";
}
