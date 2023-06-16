#include <iostream>
#include <filesystem>
#include <map>
#include <string>
#include <csignal>
#include <thread>
#include <atomic>
#include <iomanip>

std::vector<std::string> errors;
std::atomic<bool> running = true;


void signalHandler(int signum)
{
    //If user inputs an interrupt signal, we terminate the program
    std::cout << "\nInterrupt signal (" << signum << ") received.\n";
    running = false;
    // cleanup and close up stuff here  
    // terminate program  
    exit(signum);
}

void animation()
{
    //Creates the spinning animation
    int count = 0;
    const char* animationChars = "|/-\\";
    while (running)
    {
        std::cout << "\rCalculating sizes... " << animationChars[count++ % 4] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    //Clean up when done so doesn't show in the output
    std::cout << "\r" << std::string(80, ' ') << "\r" << std::flush;
}

std::string sizeToString(std::uintmax_t size)
{
    //Converting unit strings, typically anything above TB will not be used
    const char* units[] = { "B", "KB", "MB", "GB", "TB", "PB", "EB" };
    int i = 0;
    //Continue division untill correct unit is found
    while (size > 1024 && i < sizeof(units) / sizeof(*units) - 1) {
        size /= 1024;
        i++;
    }
    //Format the return string
    return std::to_string(size) + " " + units[i];
}

std::uintmax_t directorySize(const std::filesystem::path& path)
{
    //This will use recursion to find all the size of the files including an directory but not
    //listing into the directory possibly list tree of depth 1 to two is under consideration
    std::uintmax_t totalSize = 0;
    try {
        for (auto& p : std::filesystem::recursive_directory_iterator(path))
        {
            if (p.is_regular_file())
            {
                totalSize += p.file_size();
            }
            //Break if interrupted
            if (!running)
                break;
        }
    }
    catch (std::filesystem::filesystem_error& e) {
        //Catch the error so the program may contine, errors will be printed at the end
        //TODO: might be redundant, however this is safer
        errors.push_back(path.string() + " ... " + "FAILED: " + e.code().message());
    }
    return totalSize;
}

int main(int argc, char* argv[])
{
    //Start detecting for user signals and assign threads for animation
    signal(SIGINT, signalHandler);
    std::thread animationThread(animation);

    //Find default path
    std::filesystem::path pathToScan;
    if (argc > 2) {
        //Means relative path
        if (std::string(argv[1]) == "-r") {
            pathToScan = std::filesystem::current_path() / argv[2];
        }
        //Means global path path
        else if (std::string(argv[1]) == "-g") {
            pathToScan = argv[2];
        }
    }
    else {
        //Instruction unclear, so default path
        pathToScan = std::filesystem::current_path();
    }
    //Indicate the current path so we know we are on the right track
    std::cout << "Working with path: " << pathToScan << std::endl;

    //For sorting in descending order of sizes
    std::map<std::uintmax_t, std::filesystem::path, std::greater<>> sortedPaths;

    //The actual scanning starts here
    for (const auto& entry : std::filesystem::directory_iterator(pathToScan))
    {
        if (!running)
            break;

        std::uintmax_t size = 0;
        try {
            if (entry.is_directory())
            {
                //If is directory, need recursion
                size = directorySize(entry.path());
            }
            else if (entry.is_regular_file())
            {
                //If is not directory, simply record the size
                size = entry.file_size();
            }
        }
        catch (std::filesystem::filesystem_error& e) {
            //Errors will be pushed to the back of the "errors" vector
            errors.push_back(entry.path().string() + " ...... " + "FAILED: " + e.code().message());
            continue;
        }
        //Record the information processed
        sortedPaths.insert(std::make_pair(size, entry.path()));
    }

    //Handles printing here, these are used for alignment, go through all recorded files
    int maxPathLength = 0;
    int maxSizeLength = 0;
    for (const auto& entry : sortedPaths) {
        maxPathLength = std::max(maxPathLength, static_cast<int>(entry.second.string().size()));
        maxSizeLength = std::max(maxSizeLength, static_cast<int>(sizeToString(entry.first).size()));
    }

    //Tunning is complete we can stop threads and terminate the program,, start wrapping up
    running = false;
    animationThread.join();

    //Print the files with proper alignments
    for (const auto& entry : sortedPaths)
    {
        std::cout << std::left << std::setw(maxPathLength) << entry.second.string()
            << " ... " << std::right << std::setw(maxSizeLength)
            << sizeToString(entry.first) << std::endl;
    }

    //Indicates that the following results are the errors
    std::cout << "\n" << std::string(50, '-') << "\n";

    //Print collected errors
    for (const auto& error : errors) {
        std::cout << error << std::endl;
    }
    //Indicate completion
    std::cout << "\nDone." << std::endl;

    return 0;

}
