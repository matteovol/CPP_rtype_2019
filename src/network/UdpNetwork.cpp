/*
** EPITECH PROJECT, 2019
** CPP_rtype_2019
** File description:
** UdpNetwork
*/

#include "UdpNetwork.hpp"

Server::UdpNetwork::UdpNetwork(boost::asio::io_context &io)
    : _socket(io, udp::endpoint(udp::v4(), 1111))
{
    startAccept();
}

Server::UdpNetwork::~UdpNetwork() {}

void Server::UdpNetwork::startAccept()
{
    std::cout << "startAccept" << std::endl;
    _socket.async_receive_from(boost::asio::buffer(_buf), _endpoint,
        boost::bind(&UdpNetwork::handleRead, this,
            boost::asio::placeholders::bytes_transferred));
}

void Server::UdpNetwork::handleRead([[maybe_unused]] const std::size_t size)
{
    std::cout << "New client" << std::endl;
    std::cout.write(_buf.data(), size);

    for (size_t i = 0; i < BUFFER_SIZE; i++)
        _buf[i] = 0;
    startAccept();
}

void Server::UdpNetwork::handleWrite([[maybe_unused]] const std::size_t size)
{
    std::cout << "Packet sent to IP" << std::endl;
}

void Server::UdpNetwork::writeback([[maybe_unused]] const uint code, const char *data, const std::size_t size)
{
    _socket.async_send_to(boost::asio::buffer(static_cast<const void *>(data), size), _endpoint,
        boost::bind(&UdpNetwork::handleWrite, this,
            boost::asio::placeholders::bytes_transferred));
}