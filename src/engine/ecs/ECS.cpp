/*
** EPITECH PROJECT, 2019
** CPP_rtype_2019
** File description:
** Game Engine methods
*/

#include "ECS.hpp"

Module::EntityComponentSystem::EntityComponentSystem() : _newId(0), _initialised(false)
{
    init();
}

void Module::EntityComponentSystem::addSystem(ECS::flagType type, std::unique_ptr<ECS::ISystem> &system)
{
    if (_systems.find(type) == _systems.end())
        _systems.emplace(type, std::move(system));
}

unsigned long long Module::EntityComponentSystem::createEntity(const std::string &tag)
{
    // Creating an new id for a new entity
    _newId++;
    _entities.emplace(_newId - 1, new ECS::Entity(tag));

    return _newId - 1;
}

unsigned long long Module::EntityComponentSystem::createEntity(const char *tag)
{
    // Creating an new id for a new entity
    _newId++;
    _entities.emplace(_newId - 1, new ECS::Entity(tag));

    return _newId - 1;
}

void Module::EntityComponentSystem::addComponentToEntity(unsigned long long id, ECS::flagType flag, std::shared_ptr<ECS::IComponent> &component)
{
    _entities[id]->addComponent(flag, component);
}

void Module::EntityComponentSystem::setEntityVisibleState(unsigned long long id, bool state)
{
    _entities[id]->setVisible(state);
}

bool Module::EntityComponentSystem::isEntityVisible(unsigned long long id)
{
    return _entities[id]->isVisible();
}

bool Module::EntityComponentSystem::hasEntity(unsigned long long id) const
{
    return !(_entities.find(id) == _entities.end());
}

bool Module::EntityComponentSystem::hasComponent(unsigned long long id, ECS::flagType type)
{
    return _entities[id]->hasComponent(type);
}

std::string Module::EntityComponentSystem::tag(unsigned long long id)
{
    return _entities[id]->tag();
}

std::shared_ptr<ECS::ISystem> &Module::EntityComponentSystem::system(ECS::flagType type)
{
    return _systems[type];
}

std::shared_ptr<ECS::Entity> &Module::EntityComponentSystem::entity(unsigned long long id)
{
    return _entities[id];
}

std::size_t Module::EntityComponentSystem::systems() const
{
    return _systems.size();
}

bool Module::EntityComponentSystem::isInitialised()
{
    return _initialised;
}

void Module::EntityComponentSystem::update()
{
    // std::async ?
    for (auto &system : _systems)
        system.second->update(_entities);
}

void Module::EntityComponentSystem::init()
{
    _initialised = true;
}