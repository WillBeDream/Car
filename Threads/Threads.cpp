// Threads.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<thread>

using std::cout;
using std::cin;
using std::endl;

using namespace std::chrono_literals;

bool finish = false;

void Hello()
{
    while (!finish)
    {
        cout << "+ ";
        std::this_thread::sleep_for(100ms);
    }
}

void World()
{
    while (!finish)
    {
        cout << "- ";
        std::this_thread::sleep_for(100ms);
    }
}

int main()
{
    setlocale(LC_ALL, "");
    /*std::thread hello_thread(Hello);
    std::thread world_thread(World);*/

    std::thread hello_thread;
    std::thread world_thread;

    hello_thread = std::thread(Hello);
    world_thread = std::thread(World);

    cin.get();
    finish = true;

    hello_thread.join();
    world_thread.join();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
