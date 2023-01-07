#include "../../include/server.hpp"

int main()
{
    Server server;

    try
    {
        server.start(DEFAULT_PORT_NUMBER);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        server.stop();
    }
    catch (const boost::system::system_error &e)
    {
        std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
        return e.code().value();
    }

    return EXIT_SUCCESS;
}
