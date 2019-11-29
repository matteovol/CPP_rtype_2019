/*
** EPITECH PROJECT, 2019
** CPP_rtype_2019
** File description:
** ClientMediator
*/

/// \file ClientMediator.cpp
/// \author Matteo V.
/// \brief Implementation of Mediator

#include "ClientMediator.hpp"

void Client::ClientMediator::run()
{
    // Connect to the server
    connectionProcedure();
    const sf::RenderWindow &w = _graph.window();

    // If the client has been connected, we can start to loop
    // cf. RFC to understand the gameloop
    while (w.isOpen()) {
        readNetwork();
        _graph.processEvents();
        sendEvents();
        _graph.display();
    }
}

void Client::ClientMediator::connectionProcedure()
{
    askForHub();
    playerIsReady();
}

void Client::ClientMediator::askForHub()
{
    // sending a packet
    Network::headerTcp *header = sendEmptyTcpHeader(Network::ASK_FOR_HUB);

    // Casting the port into an int
    std::memcpy(&_port, header->data, sizeof(int));
    std::cout << _port << std::endl;
    _udp.bind(_port);

    // Displaying the code
    {
        Debug::Logger *log = Debug::Logger::getInstance();
        std::string format = "The client is currently connected to the hub n°";

        log->generateDebugMessage(Debug::INFO, format + std::to_string(header->hubNbr), "Client::ClientMediator::askForHub");
    }

    // Assigning the hub to the current client
    _hub = header->hubNbr;

     // Freeing ressources
    delete[] header;
}

void Client::ClientMediator::playerIsReady()
{
     sendEmptyTcpHeader(Network::CLIENT_IS_READY);

    // Client is ready
    Debug::Logger *log = Debug::Logger::getInstance();
    log->generateDebugMessage(Debug::INFO, "The client is ready.", "Client::ClientMediator::playerIsReady");
}

Network::headerTcp *Client::ClientMediator::sendEmptyTcpHeader(std::size_t code)
{
    // Creating a packet to request a hub
    struct Network::headerTcp *data = new Network::headerTcp();

    // Filling the tcp header
    data->code = code;
    data->hubNbr = _hub;
    std::memset(data->data, 0, 1024);

    // Sending the packet
    _tcp.send((void *)data, sizeof(Network::headerTcp));
    delete data;

    // Getting the response from the server
    char *response = _tcp.receive();
    while (!response && code != Network::CLIENT_IS_READY)
        response = _tcp.receive();

    Network::headerTcp *header = static_cast<Network::headerTcp *>((void *)response);
    return header;
}

void Client::ClientMediator::sendEvents()
{
    struct Network::headerUdp *data = new Network::headerUdp();
    std::size_t evtTracker = _graph.trackEvent();

    data->code = Network::CLIENT_TICK;
    std::memcpy(data->data, &evtTracker, sizeof(std::size_t));

    _udp.send(data, _port, sizeof(Network::headerUdp));
}

void Client::ClientMediator::readNetwork()
{
    void *packet = _udp.receive();
    if (packet)
        _graph.parsePackets(packet);
}
