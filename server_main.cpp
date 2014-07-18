#include <iostream>

#include <HandlingServer.hpp>


using std::cout;
using std::endl;

typedef std::vector<int> info_vec;
typedef std::vector<double> frame_vec;

int main()
{
    HandlingServer aserver;     // loads config file on creation

    aserver.startListening();   // blocks until connection

    while( aserver.isConnected() ) {

        boost::this_thread::sleep( boost::posix_time::milliseconds(100));   // idle
    }

    cout << "Done, exiting." << endl;
    return 0;
}

