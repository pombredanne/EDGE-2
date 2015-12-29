#ifndef EE_ENTITY_HPP
#define EE_ENTITY_HPP

#include <string>

using std::string;

class Entity
{
public:
    Entity(const string &name);
    ~Entity();

    string getName();

    void X(double x);
    void Y(double y);
    void W(double w);
    void H(double h);

    double X();
    double Y();
    double W();
    double H();

private:
    string name;
    double x, y;
    double w, h;
};

#endif