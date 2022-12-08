// Student Name: Lauren Smillie
// Student Number: 21025828

// Project: SYDE 121, Assignment #9
// Filename: 21025828.cpp

// I hereby declare that this code, submitted for credit for the course
// SYDE 121, is a product of my own efforts. This coded solution has
// not been plagiarized from other sources and has not been knowingly
// plagiarized by others.

// Purpose: This program takes an inputted image, stores its information,
//          calculates statistics of the pixel values and creates an inverse image

// Due Date: 12/02/2022

# include <iostream>
# include <cmath>
# include <fstream>
# include <string>
# include <vector>
# include <iomanip>
using namespace std;

// Class to store instances of Image objects
class Image
{
    public:

        Image(ifstream& in_stream);
        // Function creates new instances of image objects from an input pgm file
        //      INPUTS: Input file (samp_img.pgm)
        //      OUTPUT: None

        int get_header(ifstream& in_image);
        // Function reads and stores the header of an input pgm file
        //      INPUTS: Input file (samp_img.pgm) and the number of header lines to copy 
        //      OUTPUT: None

        void invert(ofstream& out_stream);
        // Function inverts the pixel values of an image and outputs a corresponding .ppm file
        //      INPUTS: Output file (21025828_inverted.ppm)
        //      OUTPUT: None

        void avrg_row(ofstream& out_stream);
        // Function calculates the average of the pixels values of each row of an image and copies data to an output file
        //      INPUTS: Output file (21025828_stats.txt)
        //      OUTPUT: None

        void avrg_column(ofstream& out_stream);
        // Function calculates the average of the pixels values of each column of an image and copies data to an output file
        //      INPUTS: Output file (21025828_stats.txt)
        //      OUTPUT: None

        void avrg_image(ofstream& out_stream);
        // Function calculates the total average of all the pixel values in the image and outputs it to an output file
        //      INPUTS: Output file (21025828_stats.txt)
        //      OUTPUT: None

        double avrg_image();
        // Overloaded avrg_image function that just calculates the total average of all the pixel values in the image
        //      INPUTS: None
        //      OUTPUT: None

        void std_image(ofstream& out_stream);
        // Function calculates the standard deviation of the entire image and outputs in to an output file
        //      INPUTS: Output file (21025828_stats.txt)
        //      OUTPUT: None

        double std_image();
        // Overloaded std_image function that just calculates the standard deviation of the entire image
        //      INPUTS: None
        //      OUTPUT: None

        void distribution(ofstream& out_stream);
        // Function calculates and outputs the distribution of pixel values to an output file
        //      INPUTS: Output file (21025828_stats.txt)
        //      OUTPUT: None

        void destructor();
        // Function destroys the 2D dynamic array of pixels
        //      INPUTS: None
        //      OUTPUT: None
    
    private:
        int num_rows;               // Number of pixel rows in image
        int num_cols;               // Number of pixel columns in image
        int **pixels;               // Matrix / dynamic 2D array of all pixel values of image
        vector <string> heading;    // List of the lines in the heading of the file
                                    // i.e. image type, comments, dimensions, and max pixel value
};

int main()
{   
    // Declare file streams
    ifstream in_stream;
    ofstream out_stream;
    ofstream out_inv_img;

    // Open input image
    in_stream.open("samp_img.pgm");

    // Print error if it cannot be found
    if(in_stream.fail())
    {
       cout << "Input file \"samp_img.pgm\" could not be found. \n";
       exit(1);
    }
    
    // Open output file for image statistics (or create a new one)
    out_stream.open("21025828_stats.txt");

    // Open output file to store inverted image (or create a new one)
    out_inv_img.open("21025828_inverted.ppm");
    
    // Create new image using input file
    Image* img = new Image(in_stream);

    // Calculate image statistics on ORIGINAL IMAGE
    img -> avrg_row(out_stream);
    img -> avrg_column(out_stream);
    img -> avrg_image(out_stream);
    img -> std_image(out_stream);
    img -> distribution(out_stream);

    // Invert image
    img -> invert(out_inv_img);

    // Destroy 2D dynamic array of pixels
    img -> destructor();

    // Delete image object
    delete img;

    // Close file streams
    in_stream.close();
    out_stream.close();
    out_inv_img.close();

    return 0;
}

int Image :: get_header(ifstream& in_image)
{
    string line;                // Current line being read by iterator
    int dim_counter = 0;        // Counter to keep track of the dimension line's number
    bool at_matrix = false;     // Marker to see if the current line is made of pixels
    
    getline(in_image, line);     // Get image type
    heading.push_back(line);    // Copy to heading
    dim_counter ++;

    // Copy all comments (works for a variable number of comments in file)
    while(getline(in_image, line) && !at_matrix)
    {
        // Add comment to heading
        heading.push_back(line);
        dim_counter ++;

        // Check if the line is a comment
        while(line.at(0) == '#')
        {
            // Continue reading comments until the dimension line of the image is reached 
            getline(in_image, line);
            heading.push_back(line);
            dim_counter ++;

            if(line.at(0) != '#')
            {
                at_matrix = true;
            }
        }
    }
    heading.push_back(line);

    // Return the file line number that contains the pixel dimensions
    return dim_counter;
}

Image :: Image(ifstream& in_image)
{
    string line;                            // Current and previous line of file
    int dim_line = get_header(in_image);    // The line number which holds the matrix's dimensions
    int pix_val;                            // The value of the current pixel in the image

    // Set iterator to beginning of file again
    in_image.clear();
    in_image.seekg(0);

    // Get to line above the dimension line
    for(int i = 0; i < dim_line - 1; i++)
    {
        getline(in_image, line);
    }

    // Get rows and columns of pixel matrix from dimemsion line of file
    in_image >> num_rows;
    in_image >> num_cols;

    // Skip past dimension line and max value line to get to pixel values
    getline(in_image, line);
    getline(in_image, line);

    pixels = new int*[num_rows];    // Initialize the rows of dynamic 2D array

    for(int i = 0; i < num_rows; i++)
    {
        // Initialize each column of dynamic 2D array
        pixels[i] = new int[num_cols];

        for(int j = 0; j < num_cols; j++)
        {
            // Get current pixel value from input file and set it to corresponding spot in 2D array
            in_image >> pix_val;
            pixels[i][j] = pix_val;
        }
    }   

    return;
}

void Image :: destructor()
{
    for(int i = 0; i < num_rows; i++)
    {
        // Delete each column pixel array
        delete[] pixels[i];
    }   
    // Delete the rows of the pixel array
    delete[] pixels;
    return;
}


void Image :: invert(ofstream& out_stream)
{    
    // Copy heading to inverted image file
    for(int k = 0; k < heading.size(); k++)
    {
        out_stream << heading[k] << "\n";
    }

    for(int i = 0; i < num_rows; i++)
    {
        // Separate each row by a creating a new line
        if(i != 0)
        {
            out_stream << "\n";
        }
        
        // Ouput the inverted pixel value to inverted image file
        for(int j = 0; j < num_cols; j++)
        {
            // Separate each pixel value by a space
            if(j != 0)
            {
                out_stream << " ";
            }

            // Calculate and ouput the inverse of each of the original image's pixels
            pixels[i][j] = abs(255 - pixels[i][j]);
            out_stream << pixels[i][j];
        }
    }
    // Output an extra line to increase file readability across image editing programs
    out_stream << endl;

    // Marker for console
    cout << "Inverted image created and outputted to  21025828_inverted.ppm.";
    return;
}

void Image :: avrg_row(ofstream& out_stream)
{
    double row_sum, row_avg;
    out_stream << "Average Pixel Values of Rows: " << endl;

    for(int i = 0; i < num_rows; i++)
    {
        // Calculate the sum of the pixel values for each row
        row_sum = 0;
        for(int j = 0; j < num_cols; j++)
        {
            row_sum += pixels[i][j];
        }
        
        // Calcualte the average of the row's pixel values
        row_avg = row_sum / (double) num_cols;

        // Output result to 21025828_stats.txt file
        out_stream << "Avg Row (" << i + 1 << ") : " << row_avg << endl;
    }

    // Marker for console
    cout << "Average of rows outputted to 21025828_stats.txt." << endl;
    out_stream << endl;
    return;
}

void Image :: avrg_column(ofstream& out_stream)
{
    double col_sum, col_avg;
    out_stream << "Average Pixel Values of Columns: " << endl;

    for(int i = 0; i < num_cols; i++)
    {
        // Calculate the sum of the pixel values for each column
        col_sum = 0;
        for(int j = 0; j < num_rows; j++)
        {
            col_sum += pixels[j][i];
        }
        
        // Calcualte the average of the col's pixel values
        col_avg = col_sum / (double) num_rows;
        
        // Output results to 21025828_stats.txt file
        out_stream << "Avg Col (" << i + 1 << ") : " << col_avg << endl;
    }

    // Marker for console
    cout << "Average of cols outputted to 21025828_stats.txt." << endl;
    out_stream << endl;
    return;
}

void Image :: avrg_image(ofstream& out_stream)
{
    double total_sum, total_avg;
    total_sum = 0;
    
    // Sum all the pixels in the image
    for(int i = 0; i < num_rows; i++)
    {
        for(int j = 0; j < num_cols; j++)
        {
            total_sum += pixels[i][j];
        }
    }

    // Calculate total average and output to 21025828_stats.txt file
    total_avg = total_sum / (double) (num_rows * num_cols);
    out_stream << "Avg Pixel : " << total_avg << endl;

    // Marker for console
    cout << "Average of image outputted to 21025828_stats.txt." << endl;
    out_stream << endl;
    return;
}

double Image :: avrg_image()
{
    double total_sum, total_avg;
    total_sum = 0;

    // Sum all the pixels in the image
    for(int i = 0; i < num_rows; i++)
    {
        for(int j = 0; j < num_cols; j++)
        {
            total_sum += pixels[i][j];
        }
    }
    
    // Calculate and return the total average
    total_avg = total_sum / (double) (num_rows * num_cols);
    return total_avg;
}

void Image :: std_image(ofstream& out_stream)
{
    double std_sum = 0;     // Variable storing the sum of (xi - xavg)^2 of each pixel

    // Get average pixel value of image
    double avg_pixel = avrg_image();

    // Sum the squared difference of each individual pixel and the average pixel value
    for(int i = 0; i < num_rows; i++)
    {
        for(int j = 0; j < num_cols; j++)
        {
            std_sum += pow(pixels[i][j] - avg_pixel, 2);
        }
    }

    // Calculate the standard deviation using formula and ioutput it to 21025828_stats.txt file
    double std_dev = sqrt(std_sum / (num_rows * num_cols - 1));
    out_stream << "Standard Deviation : " << std_dev << endl;

    // Marker for console
    cout << "Standard Deviation of image outputted to 21025828_stats.txt." << endl;
    out_stream << endl;
    return;
}

double Image :: std_image()
{
    double std_sum = 0;     // Variable storing the sum of (xi - xavg)^2 of each pixel

    // Get the average pixel value of the image
    double avg_pixel = avrg_image();

    // Sum the squared difference of each individual pixel and the average pixel value
    for(int i = 0; i < num_rows; i++)
    {
        for(int j = 0; j < num_cols; j++)
        {
            std_sum += pow(pixels[i][j] - avg_pixel, 2);
        }
    }

    // Calculate and return the standard deviation using formula
    double std_dev = sqrt(std_sum / (num_rows * num_cols - 1));
    return std_dev;
}

void Image :: distribution(ofstream& out_stream)
{
    // Get the standard devitation and average of the image
    double std_dev = std_image();
    double avg = avrg_image();

    // Output specified distribution results to the 21025828_stats.txt file
    out_stream << "Average + 1 STD: " << left << setw(10) << avg + std_dev << "\t";
    out_stream << "Average - 1 STD: " << left << setw(10) << avg - std_dev << "\n";

    out_stream << "Average + 2 STD: " << left << setw(10) << avg + 2 * std_dev << "\t";
    out_stream << "Average - 2 STD: " << left << setw(10) << avg - 2 * std_dev << "\n";

    out_stream << "Average + 3 STD: " << left << setw(10) << avg + 3 * std_dev << "\t";
    out_stream << "Average - 3 STD: " << left << setw(10) << avg - 3 * std_dev << "\n";

    // Marker for console
    cout << "Distribution of image outputted to 21025828_stats.txt." << endl;
    out_stream << endl;
    return;
}