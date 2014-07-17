#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include<ThreadSafeQueue.hpp>

#include <ServerConfig.hpp>
#include <KukaResponse.hpp>
#include <KukaCommand.hpp>

using boost::asio::ip::tcp;
using std::exception;
using boost::thread;
using std::cout;
using std::endl;

typedef boost::shared_ptr<tcp::socket> socket_ptr;
typedef boost::shared_ptr<boost::asio::streambuf> streambuf_ptr;


class Server
{
    public:
        Server() {

            loadConfig();

            //startListening(serverConfig.getPort());
        }

        virtual ~Server() {
            closeConnection();
        }

        void sendPose(const std::vector<int> &info, std::vector<double> &frame) {


            streambuf_ptr message(new boost::asio::streambuf);
            command.format(*message, info, frame);   // first: infovector<int>, second: framevector<int>

            shPtrQueue.push(message);

            //cout << "Pushed message." << endl;

        }    // TODO: implement sendPose()

        void sendTrajectory() {} // TODO: implement sendTrajectory()

        void startListening() {
            startListening(serverConfig.getPort());
        }

        void loadConfig() {

            serverConfig.load();
            cout << "Port MaxBufferSize EndString:" << endl;
            serverConfig.printValues();

        }


        void closeConnection() {
            // TODO: clean thread interrupt
            connected = false;

        }

        bool isConnected() { return connected; }

        bool sendQueueEmpty() { return shPtrQueue.empty(); }

    protected:

    private:

        bool connected = false;
        bool doParse = true;

        // XML parsers
        ServerConfig serverConfig;
        KukaCommand command;
        KukaResponse response;

        // queue for incoming messages
        //ThreadSafeQueue< std::pair <std::vector<int>,std::vector< std::vector<double> > >> messageQueue;

        // or should we do a queue of streambufs?
        //ThreadSafeQueue< boost::asio::streambuf > streambufQueue;

        ThreadSafeQueue< streambuf_ptr > shPtrQueue;

        /*
        Specify port.
        Blocks until connection ends.
        */
        void startListening(unsigned short port) {

            try {
                // we need one of these
                boost::asio::io_service io_service;
                tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));

                cout << "Server listening on port " << port << "." << endl;

                socket_ptr sock(new tcp::socket(io_service));
                acceptor.accept(*sock);

                cout << "A client connected." << endl;

                // spawn
                boost::thread read_thread(&Server::readMessage,this, sock);
                boost::thread write_thread(&Server::writeMessage,this, sock);

                connected = true;

                // block until threads return
                //read_thread.join();
                //write_thread.join();
            }
            catch (std::exception &e){
                cout << "Server connection exception: " << e.what() << endl;
                closeConnection();
                return;
            }
        };

        void readMessage(socket_ptr sock) {
            // TODO: is lock read necessary (probably not)
            cout << "Server read thread started." << endl;
            try {
                int counter = 0;
                while (sock->is_open() && connected) {

                    boost::asio::streambuf message(serverConfig.getMaxBufferSize()); // TODO: streambuf should not be created here but reused, how?
                    try {
                        boost::system::error_code error;
                        boost::asio::read_until(*sock, message, serverConfig.getEndString(), error);

                        if (error == boost::asio::error::eof) {
                            cout << "Client disconnected." << endl;
                            connected = false;
                            return;
                        }

                        if (doParse) {
                            response.parse(message);
                            response.printValues();
                        }

                        cout << "Server read message #" << counter << endl;
                        cout << streambufToPtr(message) << endl;

                        ++counter;
                    }
                    catch (std::exception &e){  // complain but don't quit
                        cout << "Reading (no matching xml end element?): " << e.what() << endl;
                        cout << "Buffer contents:" << endl;
                        cout << streambufToPtr(message);
                    }
                }
            }
            catch (std::exception &e){  // TODO: this is useless right now, fix
                cout << "Fatal reading exception: " << e.what() << endl;
                closeConnection();
                return;
            }
        };  // separate thread

        void writeMessage(socket_ptr sock) {

            cout << "Server write thread started, waiting 1 seconds." << endl;
            boost::this_thread::sleep( boost::posix_time::seconds(1) );

            int writtenMessages = 0;    // just to keep track of how many messages written

            try {
                while (sock->is_open() && connected) {

                    //std::pair<std::vector<int>,std::vector< std::vector<double>> > inPair;
                    //messageQueue.wait_and_pop(inPair);  // should wake up when something is in the queue

                    //boost::asio::streambuf message;

                    //command.format(message, inPair.first, inPair.second);   // first: infovector<int>, second: vector< framevectors<double> >

                    //boost::asio::write(*sock, message);

                    streambuf_ptr message = *shPtrQueue.wait_and_pop();

//                    cout << "--------------------------" << endl;
//                    cout << "Writing message:" << endl << endl;
//                    cout << streambufToPtr(*message);
//                    cout << endl;

                    boost::asio::write(*sock, *message);

                    message.reset();    // TODO: is this how to cleanup? is the streambuf deleted now?

                    ++writtenMessages;

//                    cout << "Wrote message " << writtenMessages << ". Waiting 1 sec." << endl;
//                    cout << "--------------------------" << endl;

                    //boost::this_thread::sleep( boost::posix_time::seconds(1) );
                }
            }
            catch (std::exception &e){
                cout << "Writing exception: " << e.what() << endl;
                closeConnection();
                return;
            }
        };     // separate thread

        /*
        Gets a pointer to buffer inside streambuf.
        */
        const char * streambufToPtr(boost::asio::streambuf &message) {
            const char* bufPtr=boost::asio::buffer_cast<const char*>(message.data());
            return bufPtr;
        }


};

#endif // SERVER_H

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
