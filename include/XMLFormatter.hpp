#ifndef XMLFORMATTER_H
#define XMLFORMATTER_H


class XMLFormatter
{
    public:
        XMLFormatter() {}
        virtual ~XMLFormatter() {}

        const int MALFORMED_XML_ERROR = 666;

    protected:


        XMLDocument doc;        // tinyxml

        int errorState = 0;
        bool valid = true; // this is set to false if there is a format error

        void formatError(const char *e) {
            cout << "Failed to formt XML at: " << e << endl;
            errorState = MALFORMED_XML_ERROR;
            valid = false;
        }

    private:
};

#endif // XMLFORMATTER_H
