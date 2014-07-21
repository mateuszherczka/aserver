#include <iostream>

#include <types.hpp>
#include <HandlingServer.hpp>
#include <DataFile.hpp>

#include<boost/format.hpp>


//using std::cout;
//using std::endl;

//typedef std::vector<int> info_vec;
//typedef std::vector<double> frame_vec;
//typedef std::vector<frame_vec> trajectory_vec;

int main()
{

    DataFile dataFile;

    trajectory_vec trajectory;
    std::string filename = "trajectory.txt";

    dataFile.loadSpaceDelimited(filename, trajectory);

    cout << "File contents:" << endl;
    for (auto frame : trajectory) {
        for (const double &val : frame) {

            cout << boost::format("%d ") %dataFile.roundToPrecision(val);
        }
        cout << endl;
    }

    /*
    HandlingServer aserver;     // loads config file on creation

    aserver.startListening();   // blocks until connection

    while( aserver.isConnected() ) {

        boost::this_thread::sleep( boost::posix_time::milliseconds(100));   // idle
    }
    */

    cout << "Done, exiting." << endl;
    return 0;
}

