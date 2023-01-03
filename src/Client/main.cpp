#include "../../include/client.hpp"

int main()
{
    try
    {
        // Creation of client sockett
        Client client(RAW_IP_ADDR, PORT_NUM);

        // Connecting to client socket
        client.Connect();

        while (true)
        {
            std::osyncstream(std::cout) << "Sending request to the server... " << std::endl;
            std::string str{client.processingClientMessage()};

            if (str == "quit\n")
                break;

            std::osyncstream(std::cout) << "Response received: " << str << std::endl;
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
