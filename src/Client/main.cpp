#include "../../include/client.hpp"

int main()
{
    try
    {
        // Creation of client sockett
        Client client(DEFAULT_IP_ADDRESS_V4, DEFAULT_PORT_NUMBER);

        // Connecting to client socket
        client.Connect();

        std::string str("");
        while (str not_eq "quit\n")
        {
            std::osyncstream(std::cout) << "Sending request to the server...\n";
            str = client.processingClientMessage();
            std::osyncstream(std::cout) << "Response received: " << str << '\n';
        }

        client.Close();
    }
    catch (const boost::system::system_error &e)
    {
        std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
        return e.code().value();
    }

    return EXIT_SUCCESS;
}
