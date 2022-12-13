#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>

using namespace std;

int main(int argc, char* argv[])
{

    string type = argv[1];
    string in_fname = argv[2];
    string out_fname = argv[3];

    // opens file
    ifstream inFile(in_fname);
    string line;
    vector<vector<double>> pixelVector2D;
    vector<double> pixelVector1D;
    vector<vector<double>> newLocalImage;
    
    if (type == "global")
    {
        // call functon for global binarization (not made yet)

        // taking matrix from .txt file and making a new matrix inside c++
        while (getline(inFile, line))
        {
            vector<double> temp;
            istringstream stream(line);
            double val;
            
            while (stream >> val)
            {
                temp.push_back(val);
                pixelVector1D.push_back(val);
            }
           
            pixelVector2D.push_back(temp);

        }

        // sorting 1d vector in order
        sort(pixelVector1D.begin(), pixelVector1D.end());
        
        int size1D = pixelVector1D.size();
        double thresholdIndexG = 0;
        double thresholdG = 0;

        if ((size1D % 2) == 0) // even
        {
            thresholdIndexG = (size1D / 2);
            thresholdG = pixelVector1D[thresholdIndexG];
        }

        else // odd
            thresholdG = floor((pixelVector1D[(size1D / 2)] + pixelVector1D[(size1D / 2) + 1]) / 2);

        int matrixSizeX = pixelVector2D[0].size();
        int matrixSizeY = pixelVector2D.size();

        // overwriting matrix vector
        for (int i = 0; i < matrixSizeY; i++)
        {
            for (int j = 0; j < matrixSizeX; j++)
            {
                if (pixelVector2D[i][j] < thresholdG)
                    pixelVector2D[i][j] = 0;

                else
                    pixelVector2D[i][j] = 255;
            }
        }

        // overwrite matrix in .txt file with new c++ matrix
        ofstream outFile(out_fname);

        for (std::vector<vector<double>>::const_iterator i = (pixelVector2D).begin(); i != (pixelVector2D).end(); ++i)
        {
            for (vector<double>::const_iterator j = i->begin(); j != i->end(); ++j)
            {
                outFile << *j << " ";
            }

            outFile << "\n";
        }
    }

    else // local
    {
        int size = stoi(argv[4]);
        

        // call function for local binarization (not made yet)

        // taking matrix from .txt file and making a new matrix inside c++
        while (getline(inFile, line))
        {
            vector <double> temp;
            istringstream stream(line);
            double val;

            while (stream >> val)
            {
                temp.push_back(val);
                pixelVector1D.push_back(val);
            }

            pixelVector2D.push_back(temp);
            newLocalImage.push_back(temp);
        }

        // check bounds of neighbors
        // scan all neighbors
        // calculate threshold with neighbors
        // repeat with all pixels

        //vector<vector<double>> newLocalImage = pixelVector2D;

        double pMax = *max_element(pixelVector1D.begin(), pixelVector1D.end());
        double pMin = *min_element(pixelVector1D.begin(), pixelVector1D.end());

        for (int i = 0; i < pixelVector2D.size(); i++)
        {
            for (int j = 0; j < pixelVector2D[i].size(); j++)
            {
                // a = distance from origin
                int a = size / 2;
                vector<int> temp1D;
                
                for (int k = i - a; k <= (i + a); k++)
                {
                    for (int l = j - a; l <= (j + a); l++)
                    {
                        /*if (l < 0 || l > pixelVector2D.size())
                            continue;
                        if (k < 0 || k > pixelVector2D[0].size())
                            continue;
                        else
                            temp1D.push_back(pixelVector2D[k][l]);*/

                        if(k >= 0 && k < pixelVector2D.size() && l >= 0 && l < pixelVector2D[k].size())
                            temp1D.push_back(pixelVector2D[k][l]);
                    }
                }
\
                int sum = 0;
                for (int m = 0; m < temp1D.size(); m++)
                    sum = sum + temp1D[m];

                double meanL = sum / (double) temp1D.size();

                // standard deviation
                double C = 0;
                for (int n = 0; n < temp1D.size(); n++)
                    C += pow((temp1D[n] - meanL), 2);
                double devL = sqrt(C / temp1D.size());

                double R = 0.5 * (pMax - pMin);
                double thresholdL = (meanL) * (1 + 0.2 * ((devL / R) - 1));

                if (pixelVector2D[i][j] < thresholdL)
                    newLocalImage[i][j] = 0;
                else
                    newLocalImage[i][j] = 255;                
            }
        }

        // saving and outputting .txt file
        ofstream outFile(out_fname);

        for (std::vector<vector<double>>::const_iterator i = (newLocalImage).begin(); i != (newLocalImage).end(); ++i)
        {
            for (vector<double>::const_iterator j = i->begin(); j != i->end(); ++j)
            {
                outFile << *j << " ";
            }

            outFile << "\n";
        }
    }
    return 0;
}
