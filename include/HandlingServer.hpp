#ifndef HANDLINGSERVER_H
#define HANDLINGSERVER_H

#include <Server.hpp>


class HandlingServer : public Server
{
    public:
        HandlingServer() {}
        virtual ~HandlingServer() {}

        void handleResponse() override {

            if (handledCount % 100 == 0) {
                cout << "Handling response #" << handledCount << endl;
            }


            int i = handledCount;
            double j = i*0.001;

            info_vec info = {i,i,i,i};
            frame_vec frame = {j,j,j,j,j,j};

            sendPose(info,frame);

            ++handledCount;

            //response.printValues();
        }

    protected:
    private:

        int handledCount = 0;
};

#endif // HANDLINGSERVER_H
