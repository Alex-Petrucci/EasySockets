#include <EasySockets/EasySockets.hpp>
#include <iostream>

int main()
{
    try
    {
        const std::string data = "testing";

        std::cout << "Creating context\n";
        es::Context context;

        std::cout << "Creating server socket\n";
        es::Socket server{es::IpVersion::dual_stack, es::Protocol::udp};
        std::cout << "Binding server\n";
        server.bind_to({"::", 8080});

        std::cout << "Creating ipv4 client socket\n";
        es::Socket client_ipv4{es::IpVersion::ipv4, es::Protocol::udp};
        std::cout << "Sending data from ipv4 client\n";
        client_ipv4.send_data_to(data.c_str(), data.length(), {"127.0.0.1", 8080});

        es::EndPoint client_endpoint{};
        char buffer[1024]{};
        int64_t received{};

        std::cout << "Receiving data from ipv4 client\n";
        received = server.receive_data_from(buffer, sizeof(buffer), client_endpoint);
        if (std::string{buffer, static_cast<size_t>(received)} != data)
        {
            throw std::runtime_error("Server did not receive correct data.\nData:" + std::string{buffer} +
                "\nSender: {" + client_endpoint.address + ":" + std::to_string(client_endpoint.port) + "}");
        }

        std::cout << "Creating ipv6 client socket\n";
        es::Socket client_ipv6{es::IpVersion::ipv6, es::Protocol::udp};
        std::cout << "Sending data from ipv6 client\n";
        client_ipv6.send_data_to(data.c_str(), data.length(), {"::1", 8080});

        std::cout << "Receiving data from ipv6 client\n";
        received = server.receive_data_from(buffer, sizeof(buffer), client_endpoint);
        if (std::string{buffer, static_cast<size_t>(received)} != data)
        {
            throw std::runtime_error("Server did not receive correct data.\nData:" + std::string{buffer} +
                "\nSender: {" + client_endpoint.address + ":" + std::to_string(client_endpoint.port) + "}");
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    return 0;
}