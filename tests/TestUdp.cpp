#include <EasySockets/EasySockets.hpp>
#include <iostream>

int main()
{
    try
    {
#pragma region SETUP
        const std::string data = "testing";

        std::cout << "Creating context\n";
        es::Context context;

        std::cout << "Creating server socket\n";
        es::Socket server = es::Socket::make_bound_udp(8080);

        es::EndPoint client_endpoint{};
        char buffer[1024]{};
        int64_t received{};
#pragma endregion

#pragma region IPV4 CLIENT
        std::cout << "Creating ipv4 client socket\n";
        es::Socket client_ipv4{es::IpVersion::IPV4, es::Protocol::UDP};

        std::cout << "Sending data from ipv4 client\n";
        client_ipv4.send_data_to(data.c_str(), data.length(), {"127.0.0.1", 8080});

        std::cout << "Receiving data from ipv4 client\n";
        received = server.receive_data_from(buffer, sizeof(buffer), client_endpoint);

        if (std::string{buffer, static_cast<size_t>(received)} != data)
        {
            throw std::runtime_error("Server did not receive correct data.\nData:" + std::string{buffer} +
                "\nSender: {" + client_endpoint.address + ":" + std::to_string(client_endpoint.port) + "}");
        }

#pragma endregion

#pragma region IPV6 CLIENT
        std::cout << "Creating ipv6 client socket\n";
        es::Socket client_ipv6{es::IpVersion::IPV6, es::Protocol::UDP};

        std::cout << "Sending data from ipv6 client\n";
        client_ipv6.send_data_to(data.c_str(), data.length(), {"::1", 8080});

        std::cout << "Receiving data from ipv6 client\n";
        received = server.receive_data_from(buffer, sizeof(buffer), client_endpoint);

        if (std::string{buffer, static_cast<size_t>(received)} != data)
        {
            throw std::runtime_error("Server did not receive correct data.\nData:" + std::string{buffer} +
                "\nSender: {" + client_endpoint.address + ":" + std::to_string(client_endpoint.port) + "}");
        }
#pragma endregion

#pragma region IPV4 RESOLVED CLIENT
        std::cout << "Creating ipv4 resolved client\n";
        es::Socket resolved_ipv4 = es::Socket::make_connected_udp({"127.0.0.1", 8080});

        std::cout << "Sending data from resolved ipv4\n";
        resolved_ipv4.send_data(data.data(), data.size());

        std::cout << "Recieving resolved ipv4 data on server\n";
        received = server.receive_data_from(buffer, sizeof(buffer), client_endpoint);

        if (std::string{buffer, static_cast<size_t>(received)} != data)
        {
            throw std::runtime_error("Server did not receive correct data.\nData:" + std::string{buffer} +
                "\nSender: {" + client_endpoint.address + ":" + std::to_string(client_endpoint.port) + "}");
        }
#pragma endregion

#pragma region IPV6 RESOLVED CLIENT
        std::cout << "Creating ipv6 resolved client\n";
        es::Socket resolved_ipv6 = es::Socket::make_connected_udp({"::1", 8080});

        std::cout << "Sending data from resolved ipv4\n";
        resolved_ipv6.send_data(data.data(), data.size());

        std::cout << "Recieving resolved ipv6 data on server\n";
        received = server.receive_data_from(buffer, sizeof(buffer), client_endpoint);

        if (std::string{buffer, static_cast<size_t>(received)} != data)
        {
            throw std::runtime_error("Server did not receive correct data.\nData:" + std::string{buffer} +
                "\nSender: {" + client_endpoint.address + ":" + std::to_string(client_endpoint.port) + "}");
        }
#pragma endregion

#pragma region CLEANUP
        std::cout << "Manually closing socket\n";
        server.close();
#pragma endregion
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    return 0;
}