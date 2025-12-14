#include <EasySockets/EasySockets.hpp>
#include <iostream>


int main()
{
    try
    {
        const std::string data = "testing";
        std::cout << "Creating context\n";
        es::Context context{};

        std::cout << "Creating server socket\n";
        es::Socket server{es::IpVersion::DUAL_STACK, es::Protocol::TCP};
        std::cout << "Binding server socket\n";
        server.bind_to({"::", 8080});
        std::cout << "Start listening on server\n";
        server.listen_for_connections(2);

        std::cout << "Creating client ipv4 socket\n";
        es::Socket client_ipv4{es::IpVersion::IPV4, es::Protocol::TCP};
        std::cout << "Connecting ipv4 client to server\n";
        client_ipv4.connect_to({"127.0.0.1", 8080});

        std::cout << "Accepting ipv4 client from server\n";
        es::Socket accept_ipv4 = server.accept_connection();

        std::cout << "Sending data from ipv4 client\n";
        client_ipv4.send_data(data.data(), data.length());

        char buffer[1024];
        int64_t received{};

        std::cout << "Receiving data from ipv4 client\n";
        received = accept_ipv4.receive_data(buffer, sizeof(buffer));
        if (std::string{buffer, static_cast<size_t>(received)} != data)
        {
            throw std::runtime_error("Server did not receive correct data.\nData:" + std::string{buffer});
        }

        std::cout << "Creating client ipv6 socket\n";
        es::Socket client_ipv6{es::IpVersion::IPV6, es::Protocol::TCP};
        std::cout << "Connecting ipv6 client to server\n";
        client_ipv6.connect_to({"::1", 8080});

        std::cout << "Accepting ipv6 client from server\n";
        es::Socket accept_ipv6 = server.accept_connection();

        std::cout << "Sending data from ipv6 client\n";
        client_ipv6.send_data(data.data(), data.length());

        std::cout << "Receiving data from ipv6 client\n";
        received = accept_ipv6.receive_data(buffer, sizeof(buffer));
        if (std::string{buffer, static_cast<size_t>(received)} != data)
        {
            throw std::runtime_error("Server did not receive correct data.\nData: " + std::string{buffer});
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    return 0;
}