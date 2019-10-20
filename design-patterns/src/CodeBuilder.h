#pragma once

#include <ostream>
#include <string>
#include <vector>

using namespace std;

struct Field
{
    string name;
    string type;

    Field(string name, string type) : name(name), type(type) {}
};

struct Class
{
    string name;
    vector<Field> fields;

    Class() =   default;
};

struct CodeBuilder
{
    Class class_instance;

    CodeBuilder(const string& class_name)
    {
        class_instance.name = class_name;
    }

    CodeBuilder& add_field(const string& name, const string& type)
    {
        class_instance.fields.push_back({name, type});
        return *this;
    }

    friend ostream& operator<<(ostream& os, const CodeBuilder& obj)
    {
        string indent(2, ' ');
        os << "class " << obj.class_instance.name << endl;
        os << "{" << endl;
        for (auto& field : obj.class_instance.fields)
            os << indent << field.type << " " << field.name << ";" << endl;
        os << "};" << endl;
        return os;
    }
};