#ifndef KUKARESPONSE_H
#define KUKARESPONSE_H

#include <vector>

#include <XMLParser.hpp>

using boost::lexical_cast;
using boost::bad_lexical_cast;

class KukaResponse : public XMLParser
{
    public:

        KukaResponse() : robotInfo(5,0), robotFrame(6,0) {

            //std::vector<int> robotInfo (5,0);
            //std::vector<double> robotFrame (6,0);
        }
        virtual ~KukaResponse() {}

        // TODO: implement parse()
        void parse(boost::asio::streambuf &message) {

            // try to parse
            errorState = doc.Parse(streambufToPtr(message));

            if (errorState != 0) {  // something wrong with xml
                malformedXMLError("KukaResponse XMLDocument" );
                valid = false;
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
                    robotInfo[0] = boost::lexical_cast<int>(xmltext->Value());
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
                    robotInfo[1] = boost::lexical_cast<int>(xmltext->Value());
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
                    robotInfo[2] = boost::lexical_cast<int>(xmltext->Value());
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
                    robotInfo[3] = boost::lexical_cast<int>(xmltext->Value());
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
                    robotInfo[4] = boost::lexical_cast<int>(xmltext->Value());
                }
                catch (const boost::bad_lexical_cast &e) {
                    badCast("Id");
                }
            }
            else { nodeNotFound("Id"); }

            // ActPos
            XMLElement *frame = docHandle.FirstChildElement( "Robot" ).
                        FirstChildElement( "ActPos" ).
                        ToElement();

            if ( frame ) {    // check for nulpointer

                int xmlerr = 0;

                xmlerr += frame->QueryDoubleAttribute( "X",&robotFrame[0] );    // errorcode 0 if ok

                xmlerr += frame->QueryDoubleAttribute( "Y",&robotFrame[1] );

                xmlerr += frame->QueryDoubleAttribute( "Z",&robotFrame[2] );

                xmlerr += frame->QueryDoubleAttribute( "A",&robotFrame[3] );    // errorcode 0 if ok

                xmlerr += frame->QueryDoubleAttribute( "B",&robotFrame[4] );

                xmlerr += frame->QueryDoubleAttribute( "C",&robotFrame[5] );

                if (xmlerr != 0) {
                    malformedXMLError("ActPos Attributes");
                    valid = false;
                }
            }
            else {  nodeNotFound("ActPos"); }
        }

        std::vector<int> getInfo() {
            return robotInfo;
        }

        std::vector<double> getFrame() {
            return robotFrame;
        }

    protected:

    private:

        std::vector<int> robotInfo;
        std::vector<double> robotFrame;

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
