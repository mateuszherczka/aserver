#ifndef KUKACOMMAND_H
#define KUKACOMMAND_H


class KukaCommand
{
    public:

        KukaCommand() {}

        virtual ~KukaCommand() {}

        /*
            mesage: a streambuf to fill
            info:   Mode Tick Id Run
            frame:  X Y Z A B C
        */
        void format( boost::asio::streambuf &message, const std::vector<int> &info , const std::vector<double> &frame) {

            std::ostream to_message_stream(&message);

            to_message_stream   << "<ExternalData>\r\n";

            to_message_stream   << "<Mode>" << info[0] << "</Mode>\r\n";
            to_message_stream   << "<Tick>" << info[1] << "</Tick>\r\n";
            to_message_stream   << "<Id>" << info[2] << "</Id>\r\n";
            to_message_stream   << "<Run>" << info[3] << "</Run>\r\n";

            to_message_stream   << "<Frame X=\"" << frame[0] << "\" "
                                << "Y=\"" << frame[1] << "\" "
                                << "Z=\"" << frame[2] << "\" "
                                << "A=\"" << frame[3] << "\" "
                                << "B=\"" << frame[4] << "\" "
                                << "C=\"" << frame[5] << "\"/>\r\n";

            to_message_stream   << "</ExternalData>\r\n";
            //to_message_stream   << "\r\n";
        }

    protected:

    private:

};

#endif // KUKACOMMAND_H

//        /*
//            Mode Tick Run
//        */
//        void setInfo(const std::vector<int> &info) {
//            try {
//
//                robotInfo = info;
//            }
//            catch (std::exception &e) {
//                    cout << "Setting info exception: " << e.what() << endl;
//            }
//        }
//
//        /*
//            X Y Z A B C
//        */
//        void setFrame(const std::vector<double> &frame) {
//            try {
//                robotFrame = frame;
//            }
//            catch (std::exception &e) {
//                cout << "Setting frame exception: " << e.what() << endl;
//            }
//        }

//        std::vector<int> robotInfo;
//        std::vector<double> robotFrame;


//                for (int i = 0; i < info.size();++i) {
//                    robotInfo[i] = info[i];
//                }
//                catch (std::exception &e) {
//                    cout << "Setting info exception: " << e.what() << endl;
//                }

//                for (int i = 0; i < info.size();++i) {
//                    robotFrame[i] = frame[i];
//                }
//                catch (std::exception &e) {
//                    cout << "Setting frame exception: " << e.what() << endl;
//                }
