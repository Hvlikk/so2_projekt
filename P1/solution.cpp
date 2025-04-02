#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

mutex cout_mutex;

void philosopher(int id, vector<mutex>& forks) {
    int forksCount = forks.size();
    int left = id;
    int right = (id + 1) % forksCount;

    // Ustalenie kolejności blokowania widelców
    int first = min(left, right);
    int second = max(left, right);

    while(true) {
        // Myślenie
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Filozof " << id << " mysli\n";
        }
        this_thread::sleep_for(chrono::milliseconds(1000));

        // Podnoszenie widelców
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Filozof " << id << " probuje podniesc widelce "
                 << first << " i " << second << "\n";
        }

        forks[first].lock();
        forks[second].lock();

        // Jedzenie
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Filozof " << id << " je\n";
        }
        this_thread::sleep_for(chrono::milliseconds(2000));

        // Odkładanie widelców
        forks[second].unlock();
        forks[first].unlock();

        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Filozof " << id << " odlozyl widelce\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cerr << "Uzycie: " << argv[0] << " <liczba_filozofów>\n";
        return 1;
    }

    int philosophersCount = atoi(argv[1]);

    if(philosophersCount <= 1) {
        cerr << "Liczba filozofow musi być większa niż 1\n";
        return 1;
    }

    vector<mutex> forks(philosophersCount);
    vector<thread> philosophers;

    for(int i = 0; i < philosophersCount; ++i) {
        philosophers.emplace_back(philosopher, i, ref(forks));
    }

    for(auto& t : philosophers) {
        t.join();
    }

    return 0;
}