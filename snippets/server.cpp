
int maxcount = 10000;
int maxiter = 100;

for (int iter = 0; iter < maxiter; ++iter) {

        cout << "Sending iter " << iter << "." << endl;
        if ( aserver.isConnected() ) {

            // send some messages
            for (int i = 0; i < maxcount; ++i) {

                double j = i*((1.0)/maxcount);

                std::vector<int> info = {i,i,i,i,i};
                std::vector<double> frame = {j,j,j,j,j,j};
                aserver.sendPose(info,frame);

                if ( (rand() % 100) > 90 ) {
                    boost::this_thread::sleep( boost::posix_time::milliseconds(1) );
                }
            }
        }

        while (!aserver.sendQueueEmpty()) {
            boost::this_thread::sleep( boost::posix_time::milliseconds(100) );
        }

        cout << "Sent, waiting." << endl;
        boost::this_thread::sleep( boost::posix_time::milliseconds(1));

    }


   //                        cout << "Server read message #" << counter << endl;
//                        cout << streambufToPtr(message) << endl;

   /*
                    // for testing we write something every nn seconds
                    //boost::this_thread::sleep( boost::posix_time::seconds(1) );

                    // we write a series of messages, then do nothing more
                    if (counter == 1) {    // only send once to test

                        boost::asio::streambuf message;

                        std::vector<int> info = {1, counter, 1, 1};  // mode tick id run
                        std::vector<double> frame = {0, 700, 700, -90, 0, 180};
                        command.format(message, info, frame);
                        boost::asio::write(*sock, message);

                        cout << "Server wrote message #" << counter << endl;
                        cout << streambufToPtr(message) << endl;
                        cout << endl;

                        ++counter;
                    }
                    else if (counter == 2) {

                        boost::asio::streambuf message;

                        std::vector<int> info = {1, counter, 1, 1};  // mode tick id run
                        std::vector<double> frame = {100, 600, 600, -90, 0, 180};
                        command.format(message, info, frame);
                        boost::asio::write(*sock, message);

                        cout << "Server wrote message #" << counter << endl;
                        cout << streambufToPtr(message) << endl;
                        cout << endl;

                        ++counter;
                    }
                    else if (counter == 3) {

                        boost::asio::streambuf message;

                        std::vector<int> info = {1, counter, 1, 1};  // mode tick id run
                        std::vector<double> frame = {-100, 500, 500, -90, 0, 180};
                        command.format(message, info, frame);
                        boost::asio::write(*sock, message);

                        cout << "Server wrote message #" << counter << endl;
                        cout << streambufToPtr(message) << endl;
                        cout << endl;

                        ++ counter;
                    }
                    else if (counter == 4) {

                        boost::asio::streambuf message;

                        std::vector<int> info = {1, counter, 1, 0};  // mode tick id run
                        std::vector<double> frame = {0, 530, 730, -90, 45, 180};   // home and then quit KRL
                        command.format(message, info, frame);
                        boost::asio::write(*sock, message);

                        cout << "Server wrote message #" << counter << endl;
                        cout << streambufToPtr(message) << endl;
                        cout << endl;

                        ++ counter;
                    }
                    else {

                    ++counter;  // do nothing

                    }
                    */


                    //                    cout << "Wrote message " << writtenMessages << ". Waiting 1 sec." << endl;
//                    cout << "--------------------------" << endl;

                    //boost::this_thread::sleep( boost::posix_time::seconds(1) );

                    //std::pair<std::vector<int>,std::vector< std::vector<double>> > inPair;
                    //messageQueue.wait_and_pop(inPair);  // should wake up when something is in the queue

                    //boost::asio::streambuf message;

                    //command.format(message, inPair.first, inPair.second);   // first: infovector<int>, second: vector< framevectors<double> >

                    //boost::asio::write(*sock, message);

//                    cout << "--------------------------" << endl;
//                    cout << "Writing message:" << endl << endl;
//                    cout << streambufToPtr(*message);
//                    cout << endl;

// queue for incoming messages
        //ThreadSafeQueue< std::pair <std::vector<int>,std::vector< std::vector<double> > >> messageQueue;

        // or should we do a queue of streambufs?
        //ThreadSafeQueue< boost::asio::streambuf > streambufQueue;

//#include <KukaBuildXMLFrame.hpp>
//#include <KukaParseXMLFrame.hpp>
//#include <KukaBuildXMLExample.hpp>
//#include <KukaParseXMLExample.hpp>

        //KukaBuildXMLFrame kukaBuildMessage;
        //KukaParseXMLFrame kukaParseMessage;
        //KukaBuildXMLExample kukaBuildMessage;
        //KukaParseXMLExample kukaParseMessage;

//kukaBuildMessage.build(message,1+counter,2+counter,3+counter,4+counter,5+counter,6+counter);
                    //command.setPosXYZ(counter,counter,counter);

//std::string defaultHost = "localhost"; //std::string defaultHost = "127.0.0.1";
//std::string defaultPort = "6008";
//std::string endString = "</Robot>";
//std::size_t maxBufferSize = 1024;


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
