#include "../../include/client.hpp"

// Test string: 1253861794201345684590134913469308656340523450982453247592391237
// After transform -> 9999999KBKBKBKB777KBKBKBKBKB55555555KBKBKBKBKBKBKBKBKB3333333333KBKBKBKBKBKBKB111111KBKBKBKBKB
// Sum of only digits -> 160 (is more than 2 digits and multiple to 32)

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
            jthread jth1([&client]()
                              { client.processingClientMessage(); });
            jthread jth2([&client]()
                              { client.processingSharedBuffer(); });
        }

        client.Close();
    }
    catch (const boost::system::system_error &e)
    {
        std::cerr << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
        return e.code().value();
    }

    return EXIT_SUCCESS;
}
