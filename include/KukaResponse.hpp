#ifndef KUKARESPONSE_H
#define KUKARESPONSE_H

#include <vector>

#include <XMLParser.hpp>

using boost::lexical_cast;
using boost::bad_lexical_cast;

class KukaResponse : public XMLParser
{
    public:

        // ---------------------------------------------------------------------------
        // Constructor / Destructor
        // ---------------------------------------------------------------------------

        KukaResponse() : info(5,0), frame(6,0) {}

        virtual ~KukaResponse() {}

        // ---------------------------------------------------------------------------
        // Public Methods
        // ---------------------------------------------------------------------------

        // parses and stores internally
        void parse(boost::asio::streambuf &message) {

            // try to parse
            errorState = doc.Parse(streambufToPtr(message));

            if (errorState != 0) {  // something wrong with xml
                malformedXMLError("KukaResponse XMLDocument" );
                return;
            }

            // doc could parse message, try to get data
            valid = true;
            XMLHandle docHandle( &doc );
            XMLText *xmltext;

            // Status
            xmltext = docHandle.FirstChildElement( "Robot" )
                        .FirstChildElement( "Status" )
                        .FirstChild()   // this is a text, not an element
                        .ToText();

            if( xmltext ) {
                try {
                    info[0] = boost::lexical_cast<int>(xmltext->Value());
                }
                catch (const boost::bad_lexical_cast &e) {
                    badCast("Status");
                }
            }
            else { nodeNotFound("Status"); }

            // Error
            xmltext = docHandle.FirstChildElement( "Robot" )
                        .FirstChildElement( "Error" )
                        .FirstChild()   // this is a text, not an element
                        .ToText();

            if( xmltext ) {
                try {
                    info[1] = boost::lexical_cast<int>(xmltext->Value());
                }
                catch (const boost::bad_lexical_cast &e) {
                    badCast("Error");
                }
            }
            else { nodeNotFound("Error"); }

            // Mode
            xmltext = docHandle.FirstChildElement( "Robot" )
                        .FirstChildElement( "Mode" )
                        .FirstChild()   // this is a text, not an element
                        .ToText();

            if( xmltext ) {
                try {
                    info[2] = boost::lexical_cast<int>(xmltext->Value());
                }
                catch (const boost::bad_lexical_cast &e) {
                    badCast("Mode");
                }
            }
            else { nodeNotFound("Mode"); }

            // Tick
            xmltext = docHandle.FirstChildElement( "Robot" )
                        .FirstChildElement( "Tick" )
                        .FirstChild()   // this is a text, not an element
                        .ToText();

            if( xmltext ) {
                try {
                    info[3] = boost::lexical_cast<int>(xmltext->Value());
                }
                catch (const boost::bad_lexical_cast &e) {
                    badCast("Tick");
                }
            }
            else { nodeNotFound("Tick"); }

            // Id
            xmltext = docHandle.FirstChildElement( "Robot" )
                        .FirstChildElement( "Id" )
                        .FirstChild()   // this is a text, not an element
                        .ToText();

            if( xmltext ) {
                try {
                    info[4] = boost::lexical_cast<int>(xmltext->Value());
                }
                catch (const boost::bad_lexical_cast &e) {
                    badCast("Id");
                }
            }
            else { nodeNotFound("Id"); }

            // ActPos
            XMLElement *xframe = docHandle.FirstChildElement( "Robot" ).
                        FirstChildElement( "ActPos" ).
                        ToElement();

            if ( xframe ) {    // check for nulpointer

                int xmlerr = 0;

                xmlerr += xframe->QueryDoubleAttribute( "X",&frame[0] );    // errorcode 0 if ok

                xmlerr += xframe->QueryDoubleAttribute( "Y",&frame[1] );

                xmlerr += xframe->QueryDoubleAttribute( "Z",&frame[2] );

                xmlerr += xframe->QueryDoubleAttribute( "A",&frame[3] );    // errorcode 0 if ok

                xmlerr += xframe->QueryDoubleAttribute( "B",&frame[4] );

                xmlerr += xframe->QueryDoubleAttribute( "C",&frame[5] );

                if (xmlerr != 0) {
                    malformedXMLError("ActPos Attributes");
                }
            }
            else {  nodeNotFound("ActPos"); }
        }

        /*
            Status Error Mode Tick Id
        */
        std::vector<int> getInfo() {
            return info;
        }

        /*
            X Y Z A B C
        */
        std::vector<double> getFrame() {
            return frame;
        }

        void printValues() {

            cout << "Info: ";
            for (const auto &val : info) {
                cout << val << " ";
            }
            cout << endl;

            cout << "Frame: ";
            for (const auto &val : frame) {
                cout << val << " ";
            }
            cout << endl;
        }

    protected:

    private:

        // ---------------------------------------------------------------------------
        // Private Data
        // ---------------------------------------------------------------------------

        std::vector<int> info;
        std::vector<double> frame;

};

#endif // KUKARESPONSE_H


//        int robotStatus = 0;
//        int robotError = 0;
//        int robotMode = 0;
//        int robotTick = 0;
//        int robotId = 0;

//        double robotX = 0;
//        double robotY = 0;
//        double robotZ = 0;
//        double robotA = 0;
//        double robotB = 0;
//        double robotC = 0;
