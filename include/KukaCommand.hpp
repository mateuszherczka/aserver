#ifndef KUKACOMMAND_H
#define KUKACOMMAND_H

#include <types.hpp>

class KukaCommand
{
    public:

        // ---------------------------------------------------------------------------
        // Constructor / Destructor
        // ---------------------------------------------------------------------------

        KukaCommand() {}

        virtual ~KukaCommand() {}

        // ---------------------------------------------------------------------------
        // Public methods
        // ---------------------------------------------------------------------------

        /*
            message: a streambuf to fill
            info:   <Mode Tick Id Run Vel Frames>
            frame:  <X Y Z A B C>
        */
        void formatPose( boost::asio::streambuf &message, const info_vec &info , const frame_vec &frame) {
            // TODO: rewrite this for new data structure - vector< framevector<double> >
            std::ostream to_message_stream(&message);


            to_message_stream   << "<ExternalData>\r\n";

            to_message_stream   << "<Mode>" << info[0] << "</Mode>\r\n";
            to_message_stream   << "<Tick>" << info[1] << "</Tick>\r\n";
            to_message_stream   << "<Id>" << info[2] << "</Id>\r\n";
            to_message_stream   << "<Run>" << info[3] << "</Run>\r\n";
            to_message_stream   << "<Vel>" << info[4] << "</Vel>\r\n";
            to_message_stream   << "<Tol>" << info[5] << "</Tol>\r\n";
            to_message_stream   << "<Frames>" << info[6] << "</Frames>\r\n";

            to_message_stream   << "<XFrame XPos=\"" << frame[0] << "\" "   // the attributes MUST be XPos YPos ZPos ARot BRot CRot
                                << "YPos=\"" << frame[1] << "\" "
                                << "ZPos=\"" << frame[2] << "\" "
                                << "ARot=\"" << frame[3] << "\" "
                                << "BRot=\"" << frame[4] << "\" "
                                << "CRot=\"" << frame[5] << "\" />\r\n";

            to_message_stream   << "</ExternalData>\r\n";

            //to_message_stream   << "\r\n";
        }

        /*
            message: a streambuf to fill
            info:   <Mode Tick Id Run Vel Frames>
            trajectory:  vector of several frames <X Y Z A B C>
        */
        void formatTrajectory(boost::asio::streambuf &message, const info_vec &info , const trajectory_vec &trajectory) {

            // TODO: if this is a fact, something is crap further up the line
            //if (info[5] != trajectory.size()) {cerr << "Warning! Size of trajectory vector doesn't match framecount in info vector!" << endl; }

            std::ostream to_message_stream(&message);

            // doc tag
            to_message_stream   << "<ExternalData>\r\n";

            // info part
            to_message_stream   << "<Mode>" << info[0] << "</Mode>\r\n";
            to_message_stream   << "<Tick>" << info[1] << "</Tick>\r\n";
            to_message_stream   << "<Id>" << info[2] << "</Id>\r\n";
            to_message_stream   << "<Run>" << info[3] << "</Run>\r\n";
            to_message_stream   << "<Vel>" << info[4] << "</Vel>\r\n";
            to_message_stream   << "<Tol>" << info[5] << "</Tol>\r\n";
            to_message_stream   << "<Frames>" << info[6] << "</Frames>\r\n";

            // trajectory
            for (const frame_vec &frame : trajectory) {

                // a frame
                to_message_stream   << "<XFrame XPos=\"" << frame[0] << "\" "   // the attributes MUST be XPos YPos ZPos ARot BRot CRot
                                << "YPos=\"" << frame[1] << "\" "
                                << "ZPos=\"" << frame[2] << "\" "
                                << "ARot=\"" << frame[3] << "\" "
                                << "BRot=\"" << frame[4] << "\" "
                                << "CRot=\"" << frame[5] << "\" />\r\n";
            }

            to_message_stream   << "</ExternalData>\r\n";

        }

    protected:

    private:

        // ---------------------------------------------------------------------------
        // Private data
        // ---------------------------------------------------------------------------

        // default
        //std::string docTag = "</Robot>";

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
