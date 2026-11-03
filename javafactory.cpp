#include "javafactory.h"

std::string JavaClassUnit::accessModifierToString(int accessModifier)
{
    switch (accessModifier) {
    case PUBLIC:
        return "public ";
    case PROTECTED:
    case PROTECTED_INTERNAL:
        return "protected ";
    case PRIVATE:
    case PRIVATE_PROTECTED:
        return "private ";
    default:
        return "";
    }
}

std::string JavaClassUnit::compile(unsigned int level) const
{
    std::string result = generateShift(level) + accessModifierToString(m_accessModifier);
    if (m_flags & ABSTRACT) {
        result += "abstract ";
    }
    if (m_flags & FINAL) {
        result += "final ";
    }
    result += "class " + m_name + " {\n";

    for (const auto& section : m_fields) {
        for (const auto& field : section.second) {
            result += field->compile(level + 1);
        }
    }

    result += generateShift(level) + "}\n";
    return result;
}

std::string JavaMethodUnit::compile(unsigned int level) const
{
    std::string result = generateShift(level) + JavaClassUnit::accessModifierToString(m_accessModifier);
    if (m_flags & STATIC) {
        result += "static ";
    }
    if (m_flags & ABSTRACT) {
        result += "abstract ";
    }
    if (m_flags & FINAL) {
        result += "final ";
    }
    result += m_returnType + " " + m_name + "()";
    if (m_flags & ABSTRACT) {
        return result + ";\n";
    }
    return result + compileBody(level);
}

std::string JavaPrintOperatorUnit::compile(unsigned int level) const
{
    return generateShift(level) + "System.out.print(\"" + m_text + "\");\n";
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
