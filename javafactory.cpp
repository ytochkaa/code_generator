#include "javafactory.h"

#include <vector>

std::string JavaClassUnit::accessModifierToString(int accessModifier)
{
    switch (accessModifier) {
    case PUBLIC:
        return "public";
    case PROTECTED:
        return "protected";
    case PROTECTED_INTERNAL:
        return "protected";
    case PRIVATE:
        return "private";
    case PRIVATE_PROTECTED:
        return "private";
    case INTERNAL:
        return "";
    case PACKAGE_PRIVATE:
        return "";
    default:
        return "";
    }
}

std::string JavaClassUnit::compile(unsigned int level) const
{
    std::string result = generateShift(level);
    if (m_flags & ABSTRACT) {
        result += "abstract ";
    }
    if (m_flags & STATIC) {
        result += "static ";
    }
    if (m_flags & FINAL) {
        result += "final ";
    }
    result += "class " + m_name;
    if (!m_parent.empty()) {
        result += " extends " + m_parent;
    }
    result += " {\n";

    static const std::vector<int> order = {
        PUBLIC, PROTECTED, PROTECTED_INTERNAL, PRIVATE, PRIVATE_PROTECTED, INTERNAL, PACKAGE_PRIVATE
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

std::string JavaMethodUnit::compile(unsigned int level) const
{
    std::string result = generateShift(level);
    std::string access = JavaClassUnit::accessModifierToString(m_accessModifier);
    if (!access.empty()) {
        result += access + " ";
    }
    if (m_flags & STATIC) {
        result += "static ";
    }
    if (m_flags & ABSTRACT) {
        result += "abstract ";
    }
    if (m_flags & VIRTUAL) {
        result += "virtual ";
    }
    if (m_flags & FINAL) {
        result += "final ";
    }
    result += m_returnType + " " + m_name + "()";
    if (m_flags & CONST) {
        result += " const";
    }
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

std::string JavaPrintOperatorUnit::compile(unsigned int level) const
{
    return generateShift(level) + "System.out.println(\"" + m_text + "\");\n";
}

std::shared_ptr<ClassUnit> JavaFactory::createClass(const std::string& name, Unit::Flags flags) const
{
    return std::make_shared<JavaClassUnit>(name, flags);
}

std::shared_ptr<MethodUnit> JavaFactory::createMethod(const std::string& name, const std::string& returnType, Unit::Flags flags) const
{
    return std::make_shared<JavaMethodUnit>(name, returnType, flags);
}

std::shared_ptr<PrintOperatorUnit> JavaFactory::createPrintOperator(const std::string& text) const
{
    return std::make_shared<JavaPrintOperatorUnit>(text);
}

std::string JavaFactory::wrapProgram(const std::string& classCode) const
{
    return classCode;
}

std::string JavaFactory::fileName() const
{
    return "MyClass.java";
}
