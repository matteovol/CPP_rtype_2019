/*
** EPITECH PROJECT, 2019
** Rtype
** File description:
** Mediator definition
*/

#include "Mediator.hpp"

Server::Mediator::Mediator() : _tcp (_ioContext, &Mediator::processTcpMessage), _isRunning(true)
{
    Debug::Logger *l = Debug::Logger::getInstance();
    l->generateDebugMessage(Debug::type::INFO , "Create Mediator", "Mediator ctor");
    _boostThread = std::thread(&Server::Mediator::launchBoost, this);
}

Server::Mediator::~Mediator()
{
    _ioContext.stop();
    _boostThread.join();
}

void Server::Mediator::launchBoost()
{
    _ioContext.run();
}

void Server::Mediator::start()
{
    std::string input;
    while(_isRunning) {
        std::cout << "$>";
        std::getline(std::cin, input);
        if (input == "shutdown")
            _isRunning = false;
    }
}

void Server::Mediator::createHub(const std::string &ip)
{
    //need to initialize a thread
    _mut.lock();
    _hubs.emplace_back(std::make_unique<Server::Hub>(_hubs.size() + 1, ip, _ioContext));
    _mut.unlock();
}

int Server::Mediator::hubNumber()
{
    return _hubs.size();
}

void Server::Mediator::processTcpMessage(Server::TcpConnection *socket)
{
    Debug::Logger *l = Debug::Logger::getInstance();
    l->generateDebugMessage(Debug::type::INFO , "Enter the callback function !", "Mediator");
    Server::header *h = static_cast<Server::header *>((void *)socket->buffer().data());
    std::cout << "Message from : " << socket->ip() << std::endl;
}

// Debug::Logger *l = Debug::Logger::getInstance();
// l->generateDebugMessage(Debug::type::INFO , "Enter the game", "main");
// auto scene = std::shared_ptr<Scenes::IScene>(new Scenes::SplashScene("Splash scene", engine.ECS()));

// engine.SceneMachine()->push(scene);
// engine.SceneMachine()->run();
