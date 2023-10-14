/*
  ==============================================================================

    ECS.h
    Created: 31 Jul 2023 4:17:37pm
    Author:  Gavin

  ==============================================================================
*/

#pragma once

namespace jvk::ECS
{

class Component
{
public:
    virtual ~Component() { }
    virtual int getType() const = 0;
    virtual void prepare() { }
    virtual void apply() { }
    virtual void cease() { }
};

template <class T>
class ComponentBase : public Component
{
public:
    static int type;
    int getType() const override  { return T::type; }
};

static int nextType = 0;
template <typename T> int ComponentBase<T>::type(nextType++);

class Entity
{
public:
    Entity()
    { }
    
    void addComponent(std::unique_ptr<Component>&& component)
    {
        components.push_back(std::move(component));
    }
    
    template <class T>
    T* getComponent(int index = 0)
    {
        int i = 0;
        for (std::unique_ptr<Component>& component : components)
            if (component->getType() == T::type)
                if (i++ == index)
                    return static_cast<T*>(component.get());
        return nullptr;
    }
    
protected:
    std::vector<std::unique_ptr<Component>> components;
};

} // jvk::ECS
