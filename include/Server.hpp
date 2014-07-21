#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <ThreadSafeQueue.hpp>

#include <types.hpp>

#include <ServerConfig.hpp>
#include <KukaResponse.hpp>
#include <KukaCommand.hpp>

using boost::asio::ip::tcp;
using boost::thread;
//using std::exception;
//using std::cout;
//using std::endl;

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------

typedef boost::shared_ptr<tcp::socket> socket_ptr;
typedef boost::shared_ptr<boost::asio::streambuf> streambuf_ptr;

//typedef std::vector<int> info_vec;
//typedef std::vector<double> frame_vec;
//typedef std::vector<frame_vec> trajectory_vec;

// ---------------------------------------------------------------------------
// Class
// ---------------------------------------------------------------------------

class Server
{
    public:

        // ---------------------------------------------------------------------------
        // Constructor / Destructor
        // ---------------------------------------------------------------------------

        Server() {

            loadConfig();
        }

        virtual ~Server() {
            closeConnection();
        }

        // ---------------------------------------------------------------------------
        // Pure Virtual
        // ---------------------------------------------------------------------------

        virtual void handleResponse() = 0; // This is mandatory to implement in derived class

        // ---------------------------------------------------------------------------
        // Methods
        // ---------------------------------------------------------------------------

        void sendPose(const info_vec &info, const frame_vec &frame) {

            streambuf_ptr message(new boost::asio::streambuf);
            command.formatPose(*message, info, frame);   // first: infovector<int>, second: framevector<int>

            messageQueue.push(message);
        }

        void sendTrajectory(const info_vec &info, const trajectory_vec &trajectory) {

            streambuf_ptr message(new boost::asio::streambuf);
            command.formatTrajectory(*message, info, trajectory);   // first: infovector<int>, second: framevector<int>

            messageQueue.push(message);
        } // TODO: implement sendTrajectory()

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

        bool sendQueueEmpty() { return messageQueue.empty(); }

    protected:

        KukaResponse response;  // accesible to derived class which implements handleResponse

    private:

        // ---------------------------------------------------------------------------
        // Data
        // ---------------------------------------------------------------------------

        bool connected = false;         // breaks read,write and onresponse loops when set to false

        int invalidParseCount = 0;      // handy counters
        int readMessageCount = 0;
        int writtenMessageCount = 0;

        // XML parsers
        ServerConfig serverConfig;
        KukaCommand command;

        ThreadSafeQueue<streambuf_ptr> messageQueue;
        ThreadSafeQueue<streambuf_ptr> responseQueue;

        /*
        Blocks while connected.
        */
        void onResponse(socket_ptr sock) {
            while (sock->is_open() && connected) {
                try {
                    streambuf_ptr message;
                    responseQueue.wait_and_pop(message);    // blocks

                    // we have a message, parse xml and do something
                    response.parse(*message);
                    if (response.isValid()) {   // call user defined handle response only if parser has some success
                        handleResponse();
                    }
                    else {
                        ++invalidParseCount;
                    }

                    message.reset();
                }
                catch (std::exception &e){
                    cout << "OnResponse exception: " << e.what() << endl;
                    //closeConnection();
                    //return;
                }
            }


        }

        // ---------------------------------------------------------------------------
        // Methods
        // ---------------------------------------------------------------------------
        /*
        Specify port. Blocks until incoming connection.
        When connected, spawns read/write threads, then exits.
        */
        void startListening(unsigned short port) {
            // TODO: make this so it listens continually, allowing connection if not already connected
            try {
                boost::asio::io_service io_service;
                tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));

                cout << "Server listening on port " << port << "." << endl;

                socket_ptr sock(new tcp::socket(io_service));
                acceptor.accept(*sock);

                cout << "A client connected." << endl;

                invalidParseCount = 0;
                readMessageCount = 0;
                writtenMessageCount = 0;

                // spawn
                boost::thread read_thread(&Server::readMessage,this, sock);
                boost::thread write_thread(&Server::writeMessage,this, sock);
                boost::thread response_thread(&Server::onResponse,this, sock);

                connected = true;

                cout << "Waiting 1 seconds." << endl;
                boost::this_thread::sleep( boost::posix_time::seconds(1) );
            }
            catch (std::exception &e){
                cout << "Server connection exception: " << e.what() << endl;
                closeConnection();
                return;
            }
        };

        /*
        Specify socket pointer.
        Blocks until connection ends.
        */
        void readMessage(socket_ptr sock) {
            // TODO: is lock read necessary (probably not)
            cout << "Server read thread started." << endl;
            try {

                while (sock->is_open() && connected) {

                    //boost::asio::streambuf message(serverConfig.getMaxBufferSize());
                    try {

                        streambuf_ptr message(new boost::asio::streambuf);

                        boost::system::error_code error;
                        boost::asio::read_until(*sock, *message, serverConfig.getEndString(), error);

                        if (error == boost::asio::error::eof) {
                            cout << "Client disconnected." << endl;
                            closeConnection();
                            return;
                        }

                        responseQueue.push(message);

                        ++readMessageCount;
                    }
                    catch (std::exception &e){  // complain but don't quit
                        cout << "ReadMessage exception (no matching xml end element?): " << e.what() << endl;
                        //cout << "Buffer contents:" << endl;
                        //cout << streambufToPtr(*message);
                    }
                }
            }
            catch (std::exception &e){  // TODO: this is useless right now, fix
                cout << "Fatal reading exception: " << e.what() << endl;
                closeConnection();
                return;
            }
        };  // separate thread

        /*
        Specify socket pointer.
        Blocks until connection ends.
        */
        void writeMessage(socket_ptr sock) {

            cout << "Server write thread started." << endl;

            try {
                while (sock->is_open() && connected) {

                    streambuf_ptr message;
                    messageQueue.wait_and_pop(message); // lets try this instead of:

                    //streambuf_ptr message = *messageQueue.wait_and_pop();

                    boost::asio::write(*sock, *message);

                    message.reset();    // TODO: is this how to cleanup? is the streambuf deleted now?

                    ++writtenMessageCount;
                }
            }
            catch (std::exception &e){
                cout << "Writing exception: " << e.what() << endl;
                closeConnection();
                return;
            }
        };     // separate thread

        /*
        Returns a pointer to buffer inside streambuf.
        */
        const char * streambufToPtr(boost::asio::streambuf &message) {
            const char* bufPtr=boost::asio::buffer_cast<const char*>(message.data());
            return bufPtr;
        }


};

#endif // SERVER_H

