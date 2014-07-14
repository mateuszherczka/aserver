#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>


#include <ServerConfig.hpp>
#include <KukaResponse.hpp>
#include <KukaCommand.hpp>

using boost::asio::ip::tcp;
using std::exception;
using boost::thread;
using std::cout;
using std::endl;

typedef boost::shared_ptr<tcp::socket> socket_ptr;


class Server
{
    public:
        Server() {

            serverConfig.load();
            cout << "Port MaxBufferSize EndString:" << endl;
            serverConfig.printValues();

            startListening(serverConfig.getPort());
        }

        virtual ~Server() {
            closeConnection();
        }

        void session() {};

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

                connected = true;

                cout << "A client connected." << endl;

                // spawn
                boost::thread read_thread(&Server::readMessage,this, sock);
                boost::thread write_thread(&Server::writeMessage,this, sock);

                // block until threads return
                read_thread.join();
                write_thread.join();
            }
            catch (std::exception &e){
                cout << "Server connection exception: " << e.what() << endl;
                closeConnection();
                return;
            }
        };

        void closeConnection() {
            // TODO: clean thread interrupt
            connected = false;
            //socket.close();
        };
    protected:

    private:

        bool connected = false;
        bool doParse = false;

        // XML parsers
        ServerConfig serverConfig;
        KukaCommand command;
        KukaResponse response;

        void readMessage(socket_ptr sock) {
            // TODO: is lock read necessary (probably not)
            cout << "Server read thread started." << endl;
            try {
                int counter = 0;
                while (sock->is_open() && connected) {
                    boost::asio::streambuf message(serverConfig.getMaxBufferSize()); // TODO: streambuf should not be created here but reused, how?

                    try {   // if endtag isnt found before buffer max reached
                        boost::asio::read_until(*sock, message, serverConfig.getEndString());
                        cout << "Server read message #" << counter << endl;
                        cout << streambufToPtr(message) << endl;

                        if (doParse) {
                            response.parse(message);
                            response.printValues();
                        }

                        ++counter;
                    }
                    catch (std::exception &e){  // complain but don't quit
                        cout << "Reading (no matching xml end element): " << e.what() << endl;
                        //cout << "Buffer contents:" << endl;
                        //cout << streambufToPtr(message);
                    }
                }
            }
            catch (std::exception &e){
                cout << "Fatal reading exception: " << e.what() << endl;
                closeConnection();
                return;
            }
        };  // separate thread

        void writeMessage(socket_ptr sock) {
            // TODO: is lock write necessary (probably not)
            cout << "Server write thread started." << endl;
            int counter = 0;    // just to change message a little
            try {
                while (sock->is_open() && connected) {
                    // is there something in the write queue?
                    // if so, write

                    boost::this_thread::sleep( boost::posix_time::seconds(5) );

                    // write xml every second
                    // TODO: make this an external function/interface/handle/thingie
                    boost::asio::streambuf message;
                    std::vector<int> info {1, counter, 1};
                    std::vector<double> frame {counter, counter, counter, counter, counter, counter};   // WARNING DONT SEND THIS WHEN MOVING

                    // TODO: create info and frame for command
                    command.format(message, info, frame);
                    boost::asio::write(*sock, message);

                    cout << "Server wrote message #" << counter << endl;
                    ++counter;
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
