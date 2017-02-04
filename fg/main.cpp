//
//  main.cpp
//  FrameGrabber
//
//  Created by Basolateral on 1/31/17.
//  Copyright © 2017 Basolateral. All rights reserved.
//

#include "stdafx.h"
#include "Interval.h"
#include <sys/stat.h>
#define USE_FFMPEG

using namespace cv;
using namespace std;

// Constant variables
const string AIR_ACTIVE = "Air Active";
const string AIR_PASSIVE = "Air Passive";
const string INTERIM = "Interim";
const bool DEBUG_VAR = false;
const string TEST_MP4 = "input/CSD2122sub-111615.mp4";
const string TEST_CSV = "input/CSD2122sub-111615.csv";
const unsigned int MAX_FRAMES = 3;

// Variables
vector<Interval> intervals;		// Container for intervals
VideoCapture video;				// Loads the video
string video_filename;			// Video's name
bool frame_active = true;		// True if frame grabbing for active rat, false if recording for passive rat

// Forward declarations
static void load_video();
static void load_csv();
static void filter();
static void grab_frames();

int main(int argc, const char * argv[]) {
    cout << "FrameGrabber\nAuthor: Andy Thai" << endl;
    cout << "\nUsing OpenCV version " << CV_VERSION << endl;
    if (DEBUG_VAR) {
        cout << getBuildInformation();
    }
    
    srand((unsigned int)time(NULL)); // Set seed for randomness
    
    struct stat st;
    if (stat("input/",&st) == 0) {
        if ((st.st_mode) & (S_IFDIR != 0)) {
            std::cout << "Contains input directory!" << std::endl;
        }
    }
    else {
        std::cout << "No input directory detected!" << std::endl <<
            "Creating input directory..." << std::endl <<
            "The program will shut off. Please move your input files into the input folder." << std::endl;
        mkdir("input",0777);
        cin.ignore();
        return 1;
    }
    
    if (stat("output/",&st) == 0) {
        if ((st.st_mode) & (S_IFDIR != 0)) {
            std::cout << "Contains output directory!" << std::endl;
        }
    }
    else {
        std::cout << "No output directory detected!" << std::endl <<
            "Creating output directory..." << std::endl;
        mkdir("output",0777);
    }
    
    // Check if input and output directories exist
    /**
    if (!fs::exists("input")) { // Check if input folder exists
        cout << "\nNo input folder detected! Creating input folder." << endl <<
        "FrameGrabber will shut down. Please move your input files to the input folder." << endl;
        fs::create_directory("input"); // create input folder
        cin.ignore();
        return 1;
    }
    if (!fs::exists("output")) { // Check if output folder exists
        cout << "\nCreating output folder..." << endl;
        fs::create_directory("output"); // create output folder
    }**/
    
    // Load files
    load_video();
    load_csv();
    
    // Filter prompt
    filter();
    if (intervals.size() == 0) {
        cout << "\nERROR: No intervals to grab frames from!" << endl;
        cout << "Press ENTER to close program." << endl;
        cin.ignore();
        return 1;
    }
    
    // Prompt for choosing active or passive rat
    string frame_focus;
    bool focus_valid_input = false;
    cout << "\n(A)ctive or (P)assive: ";
    while (!focus_valid_input) {
        getline(cin, frame_focus);
        if (frame_focus == "a" || frame_focus == "A") {
            frame_active = true;
            focus_valid_input = true;
        }
        else if (frame_focus == "p" || frame_focus == "P") {
            frame_active = false;
            focus_valid_input = true;
        }
        else {
            focus_valid_input = false;
            cout << "Invalid input. Please enter in a or p." << endl;
        }
    }
    
    // Print controls
    cout << "\nThis program will grab a frame from the previously loaded video." << endl <<
    "CONTROLS:" << endl <<
    "\ta: Accept" << endl <<
    "\t\tThe program will accept the frame and save it in the output folder." <<
    "\n\tr: Reject" << endl <<
    "\t\tThe program will reject the frame and will select another one." << endl <<
    "\tS: Skip" << endl <<
    "\t\tThe program will skip the current interval and go to the next one." << endl <<
    "\t\tNOTE: To skip, you must press capital S (Shift + S)." << endl <<
    "WARNING: DO NOT close the image window manually!" << endl << endl;
    
    // Start frame grabbing process
    grab_frames();
    
    // Exit behavior
    cout << endl << video_filename << " frame grabbing concluded!" << endl;
    cout << "Press ENTER to exit program." << endl;
    cin.ignore();
    
    return 0;
}

static void load_video() {

    // Get video file location
    cout << "Input video file: ";
    getline(cin, video_filename);
    
    // Default test file if none specified
    if (video_filename == "") {
        //video_filename = TEST_MP4;
    }
    else {
        video_filename = "input/" + video_filename;
    }
    
    // Initialize and load video into VideoCapture class
    video = VideoCapture();
    //video.set(CV_CAP_PROP_FOURCC, CV_FOURCC('H', '2', '6', '4'));
    //video.open(video_filename, CAP_FFMPEG);
    video.open(video_filename);
    
    // Check if stream opens a valid file
    while (!video.isOpened())
    {
        //csv_load_first_try = false;
        cout << "Could not open file " << video_filename << " to read.\n"; // if the open file fails.
        cout << "Please re-enter the video file name: ";
        getline(cin, video_filename);
        if (video_filename == "") {
            //video_filename = TEST_MP4;
        }
        else {
            video_filename = "input/" + video_filename;
        }
        video.release();
        //video.open(video_filename, CAP_FFMPEG);
        video.open(video_filename);
    }
    cout << "Loading " << video_filename << "..." << endl << endl;
}

static void load_csv() {
    // Get .csv file location
    cout << "Input .csv file: ";
    std::string csv_filename;
    getline(cin, csv_filename);
    
    // Default test file if none specified
    if (csv_filename == "") {
        //csv_filename = TEST_CSV;
        csv_filename = video_filename.substr(0, video_filename.size() - 4) + ".csv";
    }
    else {
        csv_filename = "input/" + csv_filename;
    }
    
    // Loading csv
    ifstream in_stream(csv_filename, ios::binary);
    
    // Check if stream opens a valid file
    while (in_stream.fail())
    {
        cout << "Could not open file " << csv_filename << " to read.\n"; // if the open file fails.
        cout << "Please re-enter the csv file name: ";
        getline(cin, csv_filename);
        if (csv_filename == "") {
            csv_filename = video_filename.substr(0, video_filename.size() - 4) + ".csv";
        }
        else {
            csv_filename = "input/" + csv_filename;
        }
        in_stream.close();
        in_stream.open(csv_filename, ios::binary);
    }
    
    cout << "Loading " << csv_filename << "..." << endl;
    
    // Skip first line
    string line;
    getline(in_stream, line);
    
    // Read rest of .csv
    while (getline(in_stream, line)) {
        
        // Look for the three keywords
        if (line.find(AIR_ACTIVE) != string::npos ||
            line.find(AIR_PASSIVE) != string::npos ||
            line.find(INTERIM) != string::npos) {
            
            // Find start time
            string delimiter = ",";
            int start_find = (int)line.find(delimiter);
            string start_time_string = line.substr(0, start_find);
            // Pull data from start time string
            int start_time_minutes = stoi(start_time_string.substr(0, 2));
            int start_time_seconds = stoi(start_time_string.substr(3, 5));
            double start_time_milliseconds_to_seconds = stod("0." + start_time_string.substr(6, 7));
            double start_time = start_time_minutes * 60 + start_time_seconds + start_time_milliseconds_to_seconds;
            
            // Find end time
            string delimiter2 = ",,,";
            int end_find = (int)line.find(delimiter2);
            string end_time_string = line.substr(start_find + 1, end_find - 8);
            // Pull data from end time string
            int end_time_minutes = stoi(end_time_string.substr(0, 2));
            int end_time_seconds = stoi(end_time_string.substr(3, 5));
            double end_time_milliseconds_to_seconds = stod("0." + end_time_string.substr(6, 7));
            double end_time = end_time_minutes * 60 + end_time_seconds + end_time_milliseconds_to_seconds;
            
            // Find label / name
            string name = line.substr(end_find + 3, line.length() - 1);
            
            // Create interval
            Interval current_interval = Interval(name, start_time, end_time);
            intervals.push_back(current_interval);
        }
    }
    cout << "Total amount of csv intervals: " << intervals.size() << endl << endl;
    in_stream.close();
}

static void filter() {
    cout << "You have the option to enable a filter. Doing so will remove\n" <<
    "all intervals less than the specified value. Enable filter? (y/n): ";
    
    // Variables
    string filter_input_string;			// User input
    bool filter_on;						// Determines if user picked filter on or off
    bool valid_input = false;			// Loop user input in case invalid answer
    
    // Get user input
    while (!valid_input) {
        getline(cin, filter_input_string);
        if (filter_input_string == "yes" || filter_input_string == "y") {
            filter_on = true;
            valid_input = true;
        }
        else if (filter_input_string == "no" || filter_input_string == "n") {
            filter_on = false;
            return;
        }
        else {
            valid_input = false;
            cout << "Invalid input. Please try again. (y/n): ";
        }
    }
    
    // Ask for filter threshold
    cout << "\nFilter activated. All intervals less than the filter in seconds will be\n"
    << "pruned from the list and ignored. Please input the filter threshold: ";
    string threshold_string;
    double threshold = 0;
    bool filter_valid_input = false;
    
    // Loop check for validity in input
    while (!filter_valid_input) {
        getline(cin, threshold_string);
        
        // Check if input is a number
        if (!threshold_string.empty() && threshold_string.find_first_not_of("0123456789.") == string::npos) {
            cout << "\nPruning all intervals less than " << threshold_string << " seconds." << endl;
            filter_valid_input = true;
        }
        else {
            cout << "Invalid input. Please input a number: ";
            filter_valid_input = false;
        }
    }
    threshold = stod(threshold_string);
    
    // Prune intervals
    int num_pruned = 0;
    vector<Interval>::iterator it;
    for (it = intervals.begin(); it != intervals.end();) {
        
        if (it->getLengthSeconds() < threshold) {
            num_pruned++;
            it = intervals.erase(it);
        }
        else {
            ++it;
        }
        
    }
    
    cout << "Pruned " << num_pruned << " intervals." << endl;
    cout << "Total amount of csv intervals left: " << intervals.size() << endl;
    return;
    
}

static void grab_frames() {
    Mat image = Mat();
    // Make resized image copy for view window size
    Mat small_image = Mat();
    Size small_image_size = Size(720, 540); // Original ratio: 1440 x 1080
    string output_directory = "output/";
    
    // Setting location to save frames
    string video_cut = video_filename.substr(video_filename.find("/") + 1, video_filename.length());	// Cut down on filename
    video_cut = video_cut.substr(0, video_cut.find("-") + 1);	// Cut down on filename again
    string video_date = video_filename.substr(video_filename.find("-") + 1, video_filename.length());
    video_date = video_date.substr(0, video_date.find("."));			// Date
    string jpg_extension = ".jpg";
    
    string prefix;
    if (frame_active) {
        prefix = "-A";
    }
    else {
        prefix = "-P";
    }
    string video_name = output_directory + video_cut + video_date + "/" + video_cut + video_date;
    string video_name_directory = output_directory + video_cut + video_date + "/";
    string frame_directory = video_name + prefix;
    
    // Make directory
    std::cout << "BELOW: " << std::endl;
    struct stat st;
    if (stat(video_name_directory.c_str(),&st) == 0) {
        if ((st.st_mode) & (S_IFDIR != 0)) {
            std::cout << "NOTE: This video already has an existing directory!" << std::endl;
        }
    }
    else {
        std::cout << "Creating directory " << video_name_directory << std::endl;
        mkdir(video_name_directory.c_str(),0777);
    }
    
    // Iterate through each interval
    for (int i = 0; i < (int)intervals.size(); i++) {
        cout << "\nInterval " << i + 1 << "/" << intervals.size() << endl;
        cout << "Interval start time: " << intervals[i].getStartTimeSeconds() << " seconds" << endl;
        cout << "Interval end time: " << intervals[i].getEndTimeSeconds() << " seconds" << endl;
        cout << "Interval length: " << intervals[i].getLengthSeconds() << " seconds" << endl << endl;
        int num_frames = 0;
        double curr_start = intervals[i].getStartTimeMs();
        double curr_end = intervals[i].getEndTimeMs();
        
        // Repeat until frames grabbed reaches MAX_FRAMES
        while (num_frames < MAX_FRAMES) {
            double random_frame = (curr_end - curr_start) * ((double)rand() / (double)RAND_MAX) + curr_start;
            string window_title = "Interval " + to_string(i + 1) + "/" + to_string(intervals.size()) +
            ", Frame " + to_string(num_frames + 1) + "/" + to_string(MAX_FRAMES);
            cout << "-------------------------------------------------------------------------" << endl;
            cout << "Grabbing frame at " << random_frame / 1000 << " seconds." << endl;
            
            // Set time
            video.set(CV_CAP_PROP_POS_MSEC, random_frame);
            video.read(image);
            resize(image, small_image, small_image_size);
            namedWindow(window_title, WINDOW_AUTOSIZE);
            imshow(window_title, small_image);
            
            // Calculate generated frame time
            int random_frame_min = (int)(random_frame / 1000.0 / 60.0); // in seconds
            int random_frame_sec = (int)(random_frame / 1000.0 - random_frame_min * 60.0);
            int random_frame_ms = (int)round((random_frame / 1000.0 - random_frame_min * 60.0 - random_frame_sec) * 10.0);
            if (random_frame_ms == 10) {
                random_frame_ms = 0;
            }
            
            // Modify to fit format
            string directory_min = to_string(random_frame_min);
            if (directory_min.length() == 1) {
                directory_min = "0" + directory_min;
            }
            string directory_sec = to_string(random_frame_sec);
            if (directory_sec.length() == 1) {
                directory_sec = "0" + directory_sec;
            }
            string directory_ms1000 = to_string(random_frame_ms);
            string image_directory = frame_directory + directory_min + "_" + directory_sec + "_" + directory_ms1000 + jpg_extension;
            
            // Check for valid key input
            while (1) {
                char key_pressed = waitKey(0);
                
                if (key_pressed == 'a') {
                    cout << "Frame at " << random_frame / 1000 << " seconds ACCEPTED!" << endl;
                    num_frames++;
                    cout << "Saving frame at " << image_directory << endl;
                    cout << "-------------------------------------------------------------------------" << endl << endl;
                    
                    // Save image
                    imwrite(image_directory, image);
                    
                    destroyAllWindows();
                    break;
                }
                else if (key_pressed == 'r') {
                    cout << "Frame at " << random_frame / 1000 << " seconds REJECTED!" << endl;
                    cout << "-------------------------------------------------------------------------" << endl << endl;
                    destroyAllWindows();
                    break;
                }
                else if (key_pressed == 'S') {
                    cout << "Interval " << to_string(i + 1) << " SKIPPED!" << endl;
                    cout << "-------------------------------------------------------------------------" << endl << endl;
                    num_frames = MAX_FRAMES;
                    destroyAllWindows();
                    break;
                }
            }
        }
    }
}
