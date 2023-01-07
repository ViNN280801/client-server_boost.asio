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
        while (true)
        {
            std::osyncstream(std::cout) << "Sending request to the server...\n";
            std::jthread jth1([&client]()
                              { client.processingClientMessage(); });
            std::jthread jth2([&client]()
                              { client.processingSharedBuffer(); });
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
