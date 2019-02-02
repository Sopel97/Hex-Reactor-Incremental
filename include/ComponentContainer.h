#ifndef COMPONENTCONTAINER_H
#define COMPONENTCONTAINER_H

#include <string>
#include <memory>

class Component;

class ComponentContainer //contains identification of a component as well as the component itself
{
public:
    std::shared_ptr<Component> component;
    std::string name;
    std::string description;
};

#endif // COMPONENTCONTAINER_H
