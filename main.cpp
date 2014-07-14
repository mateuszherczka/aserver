#include <iostream>

#include <Server.hpp>


using std::cout;
using std::endl;

int main()
{
    Server aserver;

    cout << "Server exiting." << endl;
    return 0;
}

// #include <ServerConfig.hpp>
//aserver.startListening(6008);
//ServerConfig serverConfig;
//ServerConfig.printValues();

//    KukaCommand command;
//    boost::asio::streambuf message;
//    std::vector<int> info {1, 2, 3};
//    std::vector<double> frame {4, 5, 6, 7, 8, 9};   // WARNING DONT SEND THIS WHEN MOVING
//    command.format(message, info, frame);
//
//    KukaResponse response;
//    response.parse(message);
//    response.printValues();
