#include "cppfactory.h"

#include <vector>

std::string CppClassUnit::compile(unsigned int level) const
{
    std::string result = generateShift(level) + "class " + m_name;
    if (m_flags & FINAL) {
        result += " final";
    }
    if (!m_parent.empty()) {
        result += " : public " + m_parent;
    }
    result += " {\n";

    static const std::vector<std::pair<int, std::string>> sections = {
        {PUBLIC, "public"},
        {INTERNAL, "public"},
        {PROTECTED, "protected"},
        {PROTECTED_INTERNAL, "protected"},
        {PRIVATE_PROTECTED, "protected"},
        {PRIVATE, "private"},
        {PACKAGE_PRIVATE, "private"}
    };

    std::string lastLabel;
    for (const auto& section : sections) {
        auto it = m_fields.find(section.first);
        if (it == m_fields.end() || it->second.empty()) {
            continue;
        }
        if (section.second != lastLabel) {
            result += section.second + ":\n";
            lastLabel = section.second;
        }
        for (const auto& field : it->second) {
            result += field->compile(level + 1);
        }
        result += "\n";
    }

    result += generateShift(level) + "};\n";
    return result;
}

std::string CppMethodUnit::compile(unsigned int level) const
{
    std::string result = generateShift(level);
    if (m_flags & STATIC) {
        result += "static ";
    } else if (m_flags & (VIRTUAL | ABSTRACT)) {
        result += "virtual ";
    }
    result += m_returnType + " " + m_name + "()";
    if (m_flags & CONST) {
        result += " const";
    }
    if (m_flags & OVERRIDE) {
        result += " override";
    }
    if (m_flags & SEALED) {
        result += " final";
    }
    if ((m_flags & FINAL) && (m_flags & VIRTUAL)) {
        result += " final";
    }
    if (m_flags & ABSTRACT) {
        result += " = 0;\n";
        return result;
    }
    result += " {\n";
    for (const auto& statement : m_body) {
        result += statement->compile(level + 1);
    }
    result += generateShift(level) + "}\n";
    return result;
}
