#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <KukaBuildXMLFrame.hpp>
#include <KukaParseXMLFrame.hpp>

using boost::asio::ip::tcp;
using std::exception;
using boost::thread;
using std::cout;
using std::endl;

typedef boost::shared_ptr<tcp::socket> socket_ptr;


class Server
{
    public:
        Server() {}

        virtual ~Server() {
            closeConnection();
        }

        void session() {};

        /*
        Connect to default.
        */
//        void connectToServer() {
//            connectToServer(defaultHost,defaultPort);
//        }

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

        std::string defaultHost = "localhost"; //std::string defaultHost = "127.0.0.1";
        std::string defaultPort = "10001";

        KukaBuildXMLFrame kukaBuildMessage;
        KukaParseXMLFrame kukaParseMessage;

        void readMessage(socket_ptr sock) {
            // TODO: is lock read necessary (probably not)
            cout << "Server read thread started." << endl;
            try {
                int counter = 0;
                while (sock->is_open() && connected) {
                    boost::asio::streambuf message; // TODO: streambuf sould not be created here but reused, how?
                    boost::asio::read_until(*sock, message, "</Rob>\r\n");

                    cout << "Server read message #" << counter << endl;
                    kukaParseMessage.parse(message);
                    kukaParseMessage.printValues();

                    ++counter;
                }
            }
            catch (std::exception &e){
                cout << "Reading exception: " << e.what() << endl;
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

                    boost::this_thread::sleep( boost::posix_time::seconds(3) );

                    // write xml every second
                    boost::asio::streambuf message;
                    kukaBuildMessage.build(message,1+counter,2+counter,3+counter,4+counter,5+counter,6+counter);
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
};

#endif // SERVER_H
