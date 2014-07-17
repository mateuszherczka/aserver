#include <iostream>

#include <Server.hpp>


using std::cout;
using std::endl;

int main()
{
    Server aserver;     // loads config

    int maxcount = 10;

    aserver.startListening();   // blocks until connection

    if ( aserver.isConnected() ) {

        // send some messages
        for (int i = 0; i < maxcount; ++i) {

            double j = i*((1.0)/maxcount);

            std::vector<int> info = {i,i,i,i,i};
            std::vector<double> frame = {j,j,j,j,j,j};
            aserver.sendPose(info,frame);
        }
    }

    while (!aserver.sendQueueEmpty()) {
        boost::this_thread::sleep( boost::posix_time::milliseconds(100) );
    }

    aserver.closeConnection();

    cout << "Done." << endl;
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
