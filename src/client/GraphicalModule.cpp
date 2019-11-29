/*
** EPITECH PROJECT, 2019
** CPP_rtype_2019
** File description:
** Component TEMPLATE
*/

/// \file GraphicalModule.cpp
/// \author Someone
/// \brief GraphicalModule methods

#include "GraphicalModule.hpp"

Client::GraphicalModule::GraphicalModule()
    : _window(sf::RenderWindow(sf::VideoMode(1920, 1080), "SFML works!")), _closed(false),
    _trackEvent(0), _bitmaskList({1, 2, 4, 8, 16, 32, 64, 128})
{
    sf::Texture artefact;

    // Creating an artefact texture
    artefact.create(50, 50);
    _textures.emplace(0, std::make_pair("artefact", artefact));

    // Pushing the event list
    _evtList.push_back({sf::Keyboard::Key::Z, "Move Up"});
    _evtList.push_back({sf::Keyboard::Key::S, "Move Down"});
    _evtList.push_back({sf::Keyboard::Key::Q, "Move Left"});
    _evtList.push_back({sf::Keyboard::Key::D, "Move Right"});
    _evtList.push_back({sf::Keyboard::Key::Num1, "1"});
    _evtList.push_back({sf::Keyboard::Key::Num2, "2"});
    _evtList.push_back({sf::Keyboard::Key::Num3, "3"});
    _evtList.push_back({sf::Keyboard::Key::Num5, "5"});
}

sf::RenderWindow &Client::GraphicalModule::window()
{
    return _window;
}

void Client::GraphicalModule::createEntity(std::size_t id)
{
    if (_entities.find(id) != _entities.end())
        return;

    std::shared_ptr<Client::Entity> entity(new Client::Entity(_textures[0].second));

    _entities.emplace(id, entity);
}

void Client::GraphicalModule::createEntity(std::size_t id, std::size_t txtId)
{
    if (_entities.find(id) != _entities.end()) {
        if (_entities[id]->textureIdx() != txtId)
            _entities[id]->setTextureIdx(txtId, _textures[txtId].second);
        return;
    }

    std::shared_ptr<Client::Entity> entity(new Client::Entity(_textures[txtId].second, txtId));

    _entities.emplace(id, entity);
}

std::size_t Client::GraphicalModule::addTexture(const std::string &filepath)
{
    // Checking if the texture already exists
    for (auto &it : _textures)
        if (it.second.first == filepath)
            return it.first;

    // If not, adding it to our unordered map
    sf::Texture newTexture;

    // Checking if the texture can be loaded
    if (!newTexture.loadFromFile(filepath)) {
        Debug::Logger *log = Debug::Logger::getInstance();

        log->generateDebugMessage(Debug::INFO, "Couldn't load a texture", "Client::GraphicalModule::addTexture");
        return 0;
    }
    _textures.emplace(_textures.size() + 1, std::make_pair(filepath, newTexture));
    return _textures.size();
}

bool Client::GraphicalModule::run()
{
    if (_window.isOpen()) {

        // Process events and display them
        processEvents();
        display();
    }
    return _closed;
}

void Client::GraphicalModule::parsePackets(void *packet)
{
    Network::headerUdp *packetHeader = static_cast<Network::headerUdp *>(packet);
    Network::Entity *entity = nullptr;

    // The server sent data about an entity
    if (packetHeader->code == Network::SERVER_TICK)
        entity = getEntityParams(packetHeader);
    if (entity) {
        std::size_t id = addTexture((char *)entity->texture);
        createEntity(entity->id, id);
        _entities[entity->id]->setPosition(entity->x, entity->y, entity->z);
    }
}

Network::Entity *Client::GraphicalModule::getEntityParams(Network::headerUdp *packetHeader)
{
    // getting the length of the path of the texture
    unsigned int len = Network::UDP_BUF_SIZE - (sizeof(unsigned long long) + sizeof(float) * 3);
    Network::Entity *packetEntity = new Network::Entity;

    // Copying all packet data into the structure
    std::memcpy(&(packetEntity->id), (unsigned long long *)packetHeader->data, sizeof(unsigned long long));
    std::memcpy(&(packetEntity->x), (float *)(packetHeader->data + sizeof(unsigned long long)), sizeof(float));
    std::memcpy(&(packetEntity->y), (float *)(packetHeader->data + sizeof(unsigned long long) + sizeof(float)), sizeof(float));
    std::memcpy(&(packetEntity->z), (float *)(packetHeader->data + sizeof(unsigned long long) + sizeof(float) * 2), sizeof(float));
    std::memcpy(packetEntity->texture, packetHeader->data + sizeof(unsigned long long) + sizeof(float) * 3, len);
    return packetEntity;
}

std::size_t Client::GraphicalModule::trackEvent() const
{
    return _trackEvent;
}

void Client::GraphicalModule::processEvents()
{
    while (_window.pollEvent(_events)) {

        // The window has been closed
        if (_events.type == sf::Event::Closed) {
            _window.close();
            _closed = true;
        }
        for (uint i = 0; i < _evtList.size(); i++)
            if (sf::Keyboard::isKeyPressed(_evtList[i].first)) {
                _trackEvent |= _bitmaskList[i];
            } else if (_events.type == sf::Event::KeyReleased && _events.key.code == _evtList[i].first) {
                _trackEvent ^= _bitmaskList[i];
            }
    }
}

void Client::GraphicalModule::display()
{
    // Clear the screen
    _window.clear();

    // Draw all entities
    for (auto &it : _entities)
        _window.draw(it.second->sprite());
    
    // Display all drawings
    _window.display();
}