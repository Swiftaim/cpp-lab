#pragma once

#include <algorithm>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

using namespace std;

struct Journal
{
    string title;
    vector<string> entries;

    Journal(string_view title);

    void add_entry(const string& entry);
};

struct PersistanceManager
{
    static void save(const Journal& journal, const string& filename);
};

enum class Color { red, green, blue };
enum class Size { small, medium, large, xl, xxl };

struct Product
{
    string name;
    Color color;
    Size size;

    Product(const char* name, Color color, Size size) : name(name), color(color), size(size) {}

    friend ostream& operator<<(ostream& o, const Product& p)
    {
        o << p.name;
        switch (p.color)
        {
            case Color::red:
                o << ", red";
                break;
            case Color::green:
                o << ", green";
                break;
            case Color::blue:
                o << ", blue";
                break;
        }

        switch (p.size)
        {
            case Size::small:
                o << ", S";
                break;
            case Size::medium:
                o << ", M";
                break;
            case Size::large:
                o << ", L";
                break;
            case Size::xl:
                o << ", XL";
                break;
            case Size::xxl:
                o << ", XXL";
                break;
        }
        return o << endl;
    }
};

template<typename T> struct Specification
{
    virtual bool is_satisfied(T& item) = 0;
};

template<typename T> struct Filter
{
    virtual vector<shared_ptr<T> > filter(const vector<shared_ptr<T> >& items, 
                                          Specification<T>& spec) = 0;
};

struct BetterProductFilter : public Filter<Product>
{
    vector<shared_ptr<Product> > filter(const vector<shared_ptr<Product> >& items, 
                                        Specification<Product>& spec) override
    {
        vector<shared_ptr<Product> > result;
        for (auto& item : items)
            if (spec.is_satisfied(*item))
                result.push_back(item);
        return result;
    }
};

struct ColorAndSizeSpec : Specification<Product>
{
    Color color;
    Size size;
    ColorAndSizeSpec(Color color, Size size) : color(color), size(size) {}

    virtual bool is_satisfied(Product& p) override
    {
        return (p.color == color && p.size == size);
    }
};

struct ProductFilter
{
    static vector<shared_ptr<Product> > by_color(const vector<shared_ptr<Product> >& items, Color color)
    {
        vector<shared_ptr<Product> > result;
        for (auto& p : items)
        {
            if (p->color == color)
                result.push_back(p);
        }
        return result;
    }
};