#include <iostream>

#include <HandlingServer.hpp>


using std::cout;
using std::endl;

int main()
{
    HandlingServer aserver;     // loads config file on creation

    aserver.startListening();   // blocks until connection

    while( aserver.isConnected() ) {

        // this thread isn't doing anything
        boost::this_thread::sleep( boost::posix_time::milliseconds(100));
    }

    cout << "Done, exiting." << endl;
    return 0;
}

