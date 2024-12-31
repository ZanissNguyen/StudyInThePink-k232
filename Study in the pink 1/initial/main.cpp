/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.

#include "study_in_pink1.h"
// #include <iostream>
// #include <windows.h>

using namespace std;

void generateTestCase(int &HP1, int &HP2, int &EXP1, int &EXP2, int &M1, int &M2, int &E1, int &E2, int &E3)
{
    srand((unsigned) time(NULL));

    HP1 = rand() % 666;
    HP2 = rand() % 666;
    EXP1 = rand() % 600;
    EXP2 = rand() % 600;
    M1 = rand() % 3000;
    M2 = rand() % 3000;
    E1 = rand() %99;
    E2 = rand() %99;
    E3 = rand() %99; 
}

void sa_tc_01() 
{
    string input_file("sa_tc_01_input");
    int HP1=0, HP2=0, EXP1=0, EXP2=0, M1=0, M2=0, E1=0, E2=0, E3=0;
    if (!readFile(input_file, HP1, HP2, EXP1, EXP2, M1, M2, E1, E2, E3))
    {
        return;
    }

    // generateTestCase(HP1, HP2, EXP1, EXP2, M1, M2, E1, E2, E3);

    //checked for 3 test cases in introduce file
    // cout << "-----------------------------------------------------------" << endl;
    // cout << "Task 1: First Meet." << endl;
    // cout << "EXP1: " << EXP1 << ", EXP2: " << EXP2 << ", E1: " << E1 << endl;
    // int result_1 = firstMeet(EXP1, EXP2, E1);
    // cout << "EXP1: " << EXP1 << ", EXP2: " << EXP2 << ", result: " << result_1 << endl;
    // cout << "-----------------------------------------------------------" << endl;

    // generateTestCase(HP1, HP2, EXP1, EXP2, M1, M2, E1, E2, E3);

    // checked for 1 test cases in introduce file
    // i wonder the case start money equal zero? what will happen?
    // answer: It will do an event then money less than 0 and limited equal 0, then Sherlock jog to the end.
    cout << "-----------------------------------------------------------" << endl;
    cout << "Task 2: Trace Luggage." << endl;
    cout << "HP1: " << HP1 << ", EXP1: " << EXP1 << ", M1: " << M1 << ", E2: " << E2 << endl;
    int result_2 = traceLuggage(HP1, EXP1, M1, E2);
    cout << "HP1: " << HP1 << ", EXP1: " << EXP1 << ", M1: " << M1 << ", result: " << result_2 << endl;
    cout << "-----------------------------------------------------------" << endl;

    // generateTestCase(HP1, HP2, EXP1, EXP2, M1, M2, E1, E2, E3);
    // checked for 2 test cases in introduce file
    // cout << "-----------------------------------------------------------" << endl;
    // cout << "Task 3: Chase Taxi." << endl;
    // cout << "HP1: " << HP1 << ", EXP1: " << EXP1 << ", HP2: " << HP2 << ", EXP2: " << EXP2 << ", E3: "<< E3 << endl;
    // int result_3 = chaseTaxi(HP1, EXP1, HP2, EXP2, E3);
    // cout << "HP1: " << HP1 << ", EXP1: " << EXP1 << ", HP2: " << HP2 << ", EXP2: " << EXP2 << ", result: "<< result_3 << endl;
    // cout << "-----------------------------------------------------------" << endl;

    // checked for 2 test cases in introduce file
    // cout << "-----------------------------------------------------------" << endl;
    // cout << "Task 4: check Password." << endl;
    // const char * email = "xyz@gmail.com";
    // const char * password = "1324xyz$k";
    // cout << "email: " << email << ", password: " << password <<  endl;
    // int result_4 = checkPassword(password, email);
    // cout << "result: "<< result_4 << endl;
    // cout << "-----------------------------------------------------------" << endl;

    // checked for 2 test case
    // cout << "-----------------------------------------------------------" << endl;
    // cout << "Task 5: Find Correct Password." << endl;
    // // string arr_pwds[] = {"123xyz", "pink#pink", "pink123", "xxxx@xxxx", "pink#pink", "pink123", "pink123"};
    // const char * arr_pwds[] = {"pink$1253", "123#xyz1", "123!xyz5", "pink#pink", "pink#pink", "pink$1253"};
    // int num_pwds = sizeof(arr_pwds)/sizeof(arr_pwds[0]);
    // for (int i = 0; i<num_pwds; i++)
    // {
    //     cout << arr_pwds[i] <<" ";
    // }
    // cout << endl;
    // int result_5 = findCorrectPassword(arr_pwds, num_pwds);
    // cout << "result: "<<result_5 << endl;
    // cout << "-----------------------------------------------------------" << endl;
}

int main(int argc, const char * argv[]) {
    sa_tc_01();

    return 0;
}