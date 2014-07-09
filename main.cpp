#include <iostream>

#include <Server.hpp>

using std::cout;
using std::endl;

int main()
{
    Server aserver;
    aserver.startListening(10001);

    cout << "Server exiting." << endl;
    return 0;
}
