#ifndef DATAFILE_H
#define DATAFILE_H

#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include <types.hpp>

//typedef std::vector<double> frame_vec;
//typedef std::vector<frame_vec> trajectory_vec;

//using std::cout;
//using std::cerr;
//using std::endl;

using std::string;
using std::ifstream;

/*
Helper class for loading space-delimited files generated in matlab etc.
*/
class DataFile
{
    public:
        DataFile() {}
        virtual ~DataFile() {}

        void loadSpaceDelimited(const string& filename, trajectory_vec &trajectory) {

            string line;
            ifstream file(filename);

            boost::char_separator<char> spaceSeparator(" ");

            if (file.is_open()) {

                while ( getline(file,line) ) {

                    // TODO: filter out empty lines and incorrect lines

                    boost::tokenizer<boost::char_separator<char>> tokens(line, spaceSeparator);

                    frame_vec frame;

                    try {

                        for (const auto &token : tokens) {  // token content should be a double

                            frame.push_back(boost::lexical_cast<double>(token));
                        }
                    }
                    catch (std::exception& e) {
                        cerr << "Cast to double in datafile failed, file not read: " << e.what();
                        return;
                    }

                    if (frame.size() == correctTokenCount) {
                        trajectory.push_back(frame);    // TODO: is this efficient copywise?
                    }
                }
                file.close();
            }

            else cerr << "Unable to open space delimited data file " << filename << endl;

        }

        double roundToPrecision(double val) {
            return floorf(val * roundingConstant + 0.5) / roundingConstant;
        }

        void setPrecision(int p) {
            precision = p;
            roundingConstant = pow(10, precision);
        }

    protected:
    private:

        int correctTokenCount = 6;
        int precision = 8;
        double roundingConstant = pow(10, precision);


};

#endif // DATAFILE_H
