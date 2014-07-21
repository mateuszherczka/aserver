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

    // load a space-delimited file
    DataFile dataFile;
    std::string filename = "trajectory.txt";

    //std::vector< frame_vec > trajectory;
    trajectory_vec trajectory;  // trajectory vector

    dataFile.loadSpaceDelimited(filename, trajectory);  // store file contents in trajectory vector

    // set parameters for trajectory
    info_vec info(6,1);  // info vector, init all values to 1

    int vel = 20;   // [mm/sec], <Vel></Vel>, info[4]
    int frames = boost::lexical_cast<int>(trajectory.size());   // <Frames></Frames>, info[5]
    info[4] = vel;
    info[5] = frames;

    cout << "Have " << trajectory.size() << " frames to send, starting server." << endl;

    // start server
    HandlingServer aserver;     // loads config file on creation

    aserver.startListening();   // blocks until connection


    cout << "Now sending " << frames << " frames." << endl;
    boost::this_thread::sleep( boost::posix_time::milliseconds(100));   // idle

    // send trajectory
    aserver.sendTrajectory(info, trajectory);

    // idle
    while( aserver.isConnected() ) {

        boost::this_thread::sleep( boost::posix_time::milliseconds(100));   // idle
    }


    cout << "Done, exiting." << endl;
    return 0;
}

    /*
    cout << "Loaded file contents:" << endl;
    for (auto frame : trajectory) {
        for (const double &val : frame) {

            cout << boost::format("%d ") %dataFile.roundToPrecision(val);
        }
        cout << endl;
    }
    */
