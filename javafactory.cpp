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
