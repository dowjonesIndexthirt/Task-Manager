#include "procRead.hpp"

#include <filesystem>
#include <algorithm>
#include <iterator>
#include <fstream>

namespace {
    /* Reads a proc status file, true if it could read, false if not */
    bool readStatusFile(const std::string& pid, processInfo& info){
        std::ifstream procFile("/proc/" + pid + "/status");

        if (!procFile.is_open()){
            #ifdef PROC_DEBUG
            std::cerr << "Could not open /proc/ " << pid << "/status\n";
            #endif

            return false;
        }

        std::string ln;

        while (procFile >> ln){
            if (ln == "Pid:")
                procFile >> info.pid;
            else if (ln == "PPid:")
                procFile >> info.parentPiD;
            else if (ln == "Uid:")
                procFile >> info.uid; // There are 4 values but they go as:
                                      // Real UID, Effective UID, Saved-set UID, Filesystem UID
                                      // Because we only need the real UID, we will ignore the rest.
            else if (ln =="Name:")
                procFile >> info.name;
            else if (ln == "State:")
                procFile >> info.state;
            else if (ln == "VmRSS:")
                procFile >> info.residentMemKB;
            else if (ln == "VmSize:")
                procFile >> info.virtMemKB;
            else if (ln == "Threads:")
                procFile >> info.threadCount;
        }

        // Safely closing so Linux likes us :)
        procFile.close();

        return true;
    };
   
    /* Read the stat file for a process, only gets user CPU time and system CPU time  */
    bool readStatFile(const std::string& pid, processInfo& info){
        std::ifstream procFile("/proc/" + pid + "/stat"); // Open the stat file

        if (!procFile.is_open()){
            #ifdef PROC_DEBUG
            std::cerr << "Could not open /proc/" << pid << "/stat file.\n";
            #endif

            return false;
        }

        std::string ln;

        std::getline(procFile, ln);

        std::size_t commEnd = ln.rfind(')');

        // Validate we found the closing parenthesis
        if (commEnd == std::string::npos){
            #ifdef PROC_DEBUG
            std::cerr << "Could not find closing parenthesis\n";
            #endif
            
            return false;
        }

        // We ignore fields 3-13 of stat
        std::istringstream statStream(ln.substr(commEnd + 1));

        for (std::size_t i = 0; i < 11; ++i)
            if(!(statStream >> ln)){
                #ifdef PROC_DEBUG
                std::cerr << "Statstream ended abruptly.\n";
                #endif

                return false;
            }

        // Extracting what we need
        if (!(statStream >> info.userCpuTicks >> info.systemCpuTicks)){
            #ifdef PROC_DEBUG
            std::cerr << "Could not get either user cpu ticks or system cpu ticks from statstream!\n";
            #endif
            return false;
        }

        // Discarding the opther stat parts since all we want are fields 14 and 15

        // Close it.
        procFile.close();

        return true;
    }

    /* Read the cmdline file of a process, getting the command line args */
    bool readCmdLineFile(const std::string& pid, processInfo& info){
        std::ifstream file("/proc/" + pid + "/cmdline", std::ios::binary);

        if (!file.is_open()){
            #ifdef PROC_DEBUG
            std::cerr << "Could not open /proc/" << pid << "/cmdline\n";
            #endif

            info.command = "Unknown"; // Return it as unknown since we cannot open it :)
            return false;
        }

        std::string command(std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{});
        
        std::replace(command.begin(), command.end(), '\0', ' ');

        // Clean up a trailing space
        if (!command.empty() && (command.back() == ' '))
            command.pop_back();

        info.command = command;
        
        file.close();
        return true;
    }

    /* Tries to get /proc/PID/exe for a symbolic link, if applicable. Generally, no :) */
    bool readExe(const std::string& pid, processInfo& info){
        std::error_code ec;
        auto path = std::filesystem::read_symlink("/proc/" + pid + "/exe", ec);

        if (!ec){
            info.exec = path.string();
            return true;
        } else {
            #ifdef PROC_DEBUG
            std::cerr << "Could not read /proc/" << pid << "/exe: " << ec.message() << '\n';
            #endif

            info.exec = "Unknown"; // Could not retrieve the symlink that has the file. Give unknown for easier display
            return false;
        }
    }

    /* Validates entire string is numeric */
    bool isNumber(const std::string& str){
        return !(str.empty()) && std::all_of(str.begin(), str.end(), [](unsigned char c){
                                                                        return std::isdigit(c);
                                                                     }); // Better with lambda

        // Could be done with a for loop but this looks cooler
        // https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
        // Linked site: https://ideone.com/OjVJWh
    }

}

/* Reads and prints all of PID1 */
void readAndPrintPID1(){
    processInfo proc;

    readStatusFile("1", proc);
    readStatFile("1", proc);
    readCmdLineFile("1", proc);
    readExe("1", proc);

    std::cout << proc << std::endl;
}

/* Reads a PID from the string number and returns it */
/* Call this with auto proc = readPID("NUM");*/
std::optional<processInfo> readPID(const std::string &pid){
    processInfo proc;

    // Since status is what builds a process, we will return nothing since it is not any "real" data
    if(!readStatusFile(pid, proc))
        return std::nullopt;
    readStatFile(pid, proc);
    readCmdLineFile(pid, proc);
    readExe(pid, proc);

    // The other info is less important we can build from here.
    return proc;
}

/* Gets all possible pids from /proc and returns a vector of completed/mostly completed PIDs */
std::vector<processInfo> readAllPIDs(){
    std::vector<processInfo> vec;

    for (const auto& entry : std::filesystem::directory_iterator("/proc")){
        // Don't validate if directory beause of a weird potential edge case.

        std::string name = entry.path().filename().string();

        if (isNumber(name)){
                auto proc = readPID(name);

                if (proc)
                    vec.push_back(std::move(*proc)); // Moves the entire struct here instead of copying.
        }

    }

    return vec;
}
