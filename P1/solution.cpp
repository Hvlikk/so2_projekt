#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

// Global mutex to synchronize console output
mutex coutMut;

// Philosopher behavior implementation
void philosopher(int id, vector<mutex>& forks) {
    // Total number of available forks
    int forksCount = forks.size();

    //Specify fork indexes (left side is same as philosopher id)
    int left = id;
    // Right fork uses modulo to ensure propper id (for philosopher n, right fork id > n)  ; id € <0, n>
    int right = (id + 1) % forksCount;

    // Deadlock prevention
    // Always lock lower-numbered fork first
    int first = min(left, right);
    int second = max(left, right);

    // Infinite behavior loop (think -> acquire forks -> eat -> release)
    while(true) {
        // Thinking phase
        { // Start synchronized output block
            lock_guard<mutex> lock(coutMut); // Output synchronization
            cout << "Filozof " << id << " mysli\n";
        } // End block - lock automatically released

        // Simulate thinking time (1 second)
        this_thread::sleep_for(chrono::milliseconds(1000));

        // Getting fork phase
        { // Start synchronized output block
            lock_guard<mutex> lock(coutMut); // Output synchronization
            cout << "Filozof " << id << " probuje podniesc widelce "
                 << first << " i " << second << "\n";
        } // End block - lock automatically released

        // Acquire forks in order
        forks[first].lock();   // Lock first fork (lower index)
        forks[second].lock();  // Lock second fork (higher index)

        // Eating phase
        { // Start synchronized output block
            lock_guard<mutex> lock(coutMut); // Output synchronization
            cout << "Filozof " << id << " je\n";
        } // End block - lock automatically released

        // Simulate eating time (2 seconds)
        this_thread::sleep_for(chrono::milliseconds(2000));

        // Release forks (order doesn't matter for unlocking)
        forks[second].unlock(); // Release second fork
        forks[first].unlock();  // Release first fork

        // Eating end notification
        { // Start synchronized output block
            lock_guard<mutex> lock(coutMut); // Output synchronization
            cout << "Filozof " << id << " odlozyl widelce\n";
        } // End block - lock automatically released
    }
}

// Program entry point, main function to set up simulation
int main(int argc, char* argv[]) {
    // Verify correct number of command-line arguments
    if(argc != 2) {
        cerr << "Uzycie: " << argv[0] << " <liczba_filozofów>\n"; // Error message
        return 1;                                                 // Exit with error code
    }


    int philosophersCount = atoi(argv[1]); // Convert string argument to integer

    // Validate philosopher count requirement
    if(philosophersCount <= 1) {
        cerr << "Liczba filozofow musi być większa niż 1\n";         // Error message
        return 1;                                                   // Exit with error code
    }

    // Create vector of fork mutexes (one per philosopher)
    vector<mutex> forks(philosophersCount);
    // Vector for philosopher threads
    vector<thread> philosophers;

    // Create philosopher threads
    for(int i = 0; i < philosophersCount; ++i) {
        // Emplace thread with arguments:
        // 1: philosopher function
        // 2: philosopher ID
        // 3: reference to forks, ref used for reference to forks vector, we can't copy mutex
        philosophers.emplace_back(philosopher, i, ref(forks));
    }

    // Wait for all threads to finish ; inf execution.
    for(auto& t : philosophers) {
        t.join(); // Block main until thread terminates, ensure proper clean up
                  // Never happens - infinite loop
    }

    return 0;
}