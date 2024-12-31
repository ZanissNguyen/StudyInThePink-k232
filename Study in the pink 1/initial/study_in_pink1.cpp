/*
* Fundamental Programming HK 232
* Submitted code for Assignment 1
* Student: Nguyen Thanh Phat
* ID: 2312593
*/
#include "study_in_pink1.h"

bool readFile(
        const string & filename,
        int & HP1,
        int & HP2,
        int & EXP1,
        int & EXP2,
        int & M1,
        int & M2,
        int & E1,
        int & E2,
        int & E3
) {
    // This function is used to read the input file,
    // DO NOT MODIFY THIS FUNTION
    ifstream ifs(filename);
    if (ifs.is_open()) {
        ifs >> HP1 >> HP2
            >> EXP1 >> EXP2
            >> M1 >> M2
            >> E1 >> E2 >> E3;
        
        //limit input
        limitHP(HP1);
        limitHP(HP2);
        limitEXP(EXP1);
        limitEXP(EXP2);
        limitM(M1);
        limitM(M2);
        

        return true;
    }
    else {
        cerr << "The file is not found" << endl;
        return false;
    }
}

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Task 1

int ceilFixed(double number)
{
    int toReturn = ceil(number);
    if ((number - (int) number) < 1e-10) return (int) number;
    return toReturn;
}

const int maxStHP = 666;
const int maxStEXP = 600;
const int maxStM = 3000; 

void limitHP(int &HP)
{
    //range Health Point   
    HP = (HP<0)?0:HP;
    HP = (HP>maxStHP)?maxStHP:HP;
}

void limitEXP(int &EXP)
{
    //range Experience
    EXP = (EXP<0)?0:EXP;
    EXP = (EXP>maxStEXP)?maxStEXP:EXP;
}

void limitM(int &M)
{
    // range Money
    M = (M<0)?0:M;
    M = (M>maxStM)?maxStM:M;
}

// function for each given infomation cases
void firstMeet_info(int num, bool & info, int & EXP2, int & E1)
{
    info = true;
    switch (num)
    {
        case 1:
            EXP2+= (ceil(double(E1)/4)+19);
            limitEXP(EXP2);
            break;
        case 2:
            EXP2+= (ceil(double(E1)/9)+21);
            limitEXP(EXP2);
            break;
        case 3:
            EXP2+= (ceil(double(E1)/16)+17);
            limitEXP(EXP2);
            break;
        default:
            break;
    }
}

// main function for task 1
int firstMeet(int & EXP1, int & EXP2, int E1) {
    // TODO: Complete this function  
    if (E1<0 || E1>99) return -99;
    limitEXP(EXP1);
    limitEXP(EXP2);
    // case E1 is 0 --> 3
    if (E1<=3)
    {
        int D = E1*3+EXP1*7;
        switch (E1)
        {
        case 0: 
            EXP2+=29;
            limitEXP(EXP2);
            break;
        case 1:
            EXP2+=45;
            limitEXP(EXP2);
            break;
        case 2:
            EXP2+=75;
            limitEXP(EXP2);
            break;
        case 3:
            EXP2 += (29 + 45 + 75);
            limitEXP(EXP2);
            break;
        default:
            break;
        }
        
        if (D%2==0) // even, Sherlock is right.
            EXP1 += ceil(double(D)/200);
        else EXP1 = ceil(EXP1 - double(D)/100); // odd, Sherlock is wrong.
        limitEXP(EXP1);
    } else // case E1 is 4 --> 99
    {
        bool info[3] = {false, false, false};
        bool steak = true;

        if (E1>=4 && E1<=19)
        {
            firstMeet_info(1, info[0], EXP2, E1);
        }
        else if (E1>=20 && E1<=49)
        {
            firstMeet_info(2, info[1], EXP2, E1);
        }
        else if (E1>=50 && E1<=65)
        {
            firstMeet_info(3, info[2], EXP2, E1);
        }
        else if (E1>=66 && E1<=79)
        {
            firstMeet_info(1, info[0], EXP2, E1);
            if (EXP2>200) firstMeet_info(2, info[1], EXP2, E1);
        }
        else if (E1>=80 && E1<=99)
        {
            firstMeet_info(1, info[0], EXP2, E1);
            firstMeet_info(2, info[1], EXP2, E1);
            if (EXP2>400) firstMeet_info(3, info[2], EXP2, E1);
        }

        for (int i = 0; i<3; i++)
        {
            if (info[i]==false) 
            {
                steak = false;
                break;
            }
        } 

        if (steak) EXP2 += ceil(0.15*double(EXP2));limitEXP(EXP2);
        EXP1 -= E1;limitEXP(EXP1);
    }

    return EXP1 + EXP2;
}

// Task 2
int nearestPrime(int number)
{
    int previous = floor(sqrt(number))*floor(sqrt(number));
    int after = ceil(sqrt(number))*ceil(sqrt(number));
    int diffPre = abs(previous-number);
    int diffAft = abs(after-number);

    return (diffPre<=diffAft)?previous:after;
}

double probabilityAsLane01(int EXP)
{
    double p = 0.0;
    int nPrime = nearestPrime(EXP);

    // cout << EXP << endl;
    // cout << nPrime << endl;/
    if (EXP >= nPrime) p =1;
    else p = double((double(EXP)/double(nPrime)+80.0)/123.0);

    return p;
}

bool takeMeal(int & paid, int & HP1, int & M1, int meal, int bigMeal)
{
    if (HP1<200) // too hungry
    {   
        HP1+=ceilFixed(0.3*HP1);
        M1-=bigMeal;
        paid+=bigMeal;
    } else      // little tired =))
    {
        HP1+=ceilFixed(0.1*HP1);
        M1-=meal;
        paid+=meal;
    }
    limitHP(HP1);
    limitM(M1);
    // cout << "Take Meal: (HP, M) = ("<<HP1<<", "<<M1<<")"<<endl;
    return false;
}

bool bookVehicle(int & paid, int & EXP1, int & M1, int taxi, int horse)
{
    if (EXP1<400) //choose taxi
    {   
        M1-=taxi;
        paid+=taxi;
    } else          // choose horse
    {
        M1-=horse;
        paid+=horse;
    }
    EXP1+=ceilFixed(0.13*EXP1);
    limitEXP(EXP1);
    limitM(M1);
    // cout << "Book Vehicle: (EXP, M) = ("<<EXP1<<", "<<M1<<")"<<endl;
    return false;
}

bool meetHomeless(int & paid, int & EXP1, int & M1, int normal, int VIP)
{
    if (EXP1<300)
    {   
        M1-=normal;
        paid+=normal;
    } else
    {
        M1-=VIP;
        paid+=VIP;
    }
    EXP1=ceil(0.9*EXP1);
    limitEXP(EXP1);
    limitM(M1);
    // cout << "Meet Homeless: (EXP, M) = ("<<EXP1<<", "<<M1<<")"<<endl;
    return false;
}

int getPosition(int E2)
{
    if (E2/10==0) return E2;
    else 
    {
        E2=E2%10+E2/10;
        return E2%10;
    }
}

// main function task 2
int traceLuggage(int & HP1, int & EXP1, int & M1, int E2)
{
    // TODO: Complete this function
    if (E2<0 || E2>99) return -99;
    limitHP(HP1);
    limitEXP(EXP1);
    limitM(M1);
    double p1, p2, p3;
    
    //lane 01
    p1 = probabilityAsLane01(EXP1);
    // cout << fixed << setprecision(2);
    // cout << "p1 = "<< p1 <<endl;

    //lane 02
    int paid = 0;
    const int bigMeal = 150;
    const int meal = 70;
    const int taxi = 200;
    const int horse = 120;
    const int homeless = 100;
    const int homelessVIP = 120;

    double halfWallet = double(M1)/2.0;

    if (E2%2==0) // E2 is even
    {
        if (M1>0)
        {
            takeMeal(paid, HP1, M1,meal,bigMeal);
            if (M1!=0)
            {
                bookVehicle(paid, EXP1, M1, taxi, horse);
                if (M1!=0)
                {
                    meetHomeless(paid, EXP1, M1, homeless, homelessVIP);
                }
            }
        }
    }
    else //E2 is odd
    {
        while (paid<=halfWallet && M1!=0)
        {
            takeMeal(paid, HP1, M1, meal, bigMeal);
            if (paid>halfWallet || M1==0) break;
            bookVehicle(paid, EXP1, M1, taxi, horse);
            if (paid>halfWallet || M1==0) break;
            meetHomeless(paid, EXP1, M1, homeless, homelessVIP);
            if (paid>halfWallet || M1==0) break;
        }
    }
    
    HP1=ceil(0.83*HP1);
    EXP1+=ceilFixed(0.17*EXP1);
    limitHP(HP1);
    limitEXP(EXP1);
    
    p2 = probabilityAsLane01(EXP1);
    // cout << "p2 = "<<p2 << endl;

    //lane 03
    int probability[10] = {32,47,28,79,100,50,22,83,64,11};
    // cout << getPosition(E2) << endl;
    p3 = probability[getPosition(E2)]/100.0;
    // cout << "p3 = "<<p3 << endl;

    // calculate result
    if ((p1==1.00000000) && (p2==1.0000000) && (p3==1.0000000))
    {
        //there is some misunderstood =))
        // cout << " Here? " << endl;
        EXP1=ceil(0.75*EXP1);
    } else
    {
        double avgP = (p1+p2+p3)/3.0;
        // cout << "P = "<< avgP << endl;
        if (avgP<0.5) //it is hard to find luggage
        {
            HP1=ceil(0.85*HP1);
            EXP1+=ceilFixed(0.15*EXP1);
        }
        else          // finding luggage is easy now
        {
            // cout << "YEET?" <<endl;
            HP1=ceil(0.9*HP1);
            EXP1+=ceilFixed(0.2*EXP1);
        }
    }
    limitHP(HP1);
    limitEXP(EXP1);

    return HP1 + EXP1 + M1;
}

// Task 3

// if number have 2 numbers ex: 44, 37
// return 3+7 = 10 --> 1+0 = 1
//        4+4 = 8
void twoToOne(int & number)
{
    while(number/10!=0)
    {
        number = number/10+number%10;
    }
}

// Main function task 3
int chaseTaxi(int & HP1, int & EXP1, int & HP2, int & EXP2, int E3) 
{
    // TODO: Complete this function
    if (E3<0 || E3>99) return -99;
    limitHP(HP1);
    limitEXP(EXP1);
    limitHP(HP2);
    limitEXP(EXP2);
    
    int size = 10;
    int index = size-1;
    int taxiPoint[size][size];

    // generate Matrix of taxi's point
    // cout << "Created Matrix: "<< endl;
    for (int i = 0; i<size; i++)
    {
        for (int j = 0; j<size; j++)
        {
            taxiPoint[i][j] = ((E3*j)+(i*2))*(i-j);
            // cout << setw(6) << taxiPoint[i][j] << " ";
        }
        // cout << endl;
    }

    // determine meet position
    int rowMeetValue = E3*2;
    int columnMeetValue = -E3;

    int rowMeetPos = 0;
    int columnMeetPos = 0;

    // count amount of numbers that greater or smaller than some value...
    for (int i = 0; i<size; i++)
    {
        for (int j = 0; j<size; j++)
        {
            if (taxiPoint[i][j]>rowMeetValue) rowMeetPos++;
            if (taxiPoint[i][j]<columnMeetValue) columnMeetPos++;
        }
    }

    twoToOne(rowMeetPos);
    twoToOne(columnMeetPos);

    // cout << "Meet Position: (" << rowMeetPos<<","<<columnMeetPos << ")"<<endl;

    // taxi point
    int taxiPointResult = taxiPoint[rowMeetPos][columnMeetPos];
    // cout << "Taxi Point: "<<taxiPointResult << endl;

    // sherlock and watson point (max of left diagonal and right diagonal)
    int sherlockPoint = taxiPointResult;
    // check for the left diagonal
    // $ * *
    // * $ *
    // * * $
    {
        int temp = min(rowMeetPos, columnMeetPos);
        int L_row = rowMeetPos-temp;
        int L_column = columnMeetPos-temp;
        // cout << "(" << L_row<<","<<L_column << ")"<<endl;
        while ((L_row>=0 && L_row<=index) && (L_column>=0 && L_column<=index))
        {
            sherlockPoint = max(sherlockPoint, taxiPoint[L_row][L_column]);
            L_row++;
            L_column++;
            // cout << "(" << L_row<<","<<L_column << ")"<<endl;
        }
    }

    // check for the right diagonal
    // * * $
    // * $ *
    // $ * *
    {
        int temp = abs(index-columnMeetPos);
        int R_row = rowMeetPos-temp;
        int R_column = columnMeetPos+temp;
        // cout << "(" << R_row <<","<< R_column << ")"<<endl;
        while ((R_row>=0 && R_row<=index) && (R_column>=0 && R_column<=index))
        {
            sherlockPoint = max(sherlockPoint, taxiPoint[R_row][R_column]);
            R_row++;
            R_column--;
            // cout << "(" << R_row<<","<<R_column << ")"<<endl;
        }
    }

    // cout << "Sherlock Point: "<<sherlockPoint << endl;

    // calculate result
    int result = 0; // (abs(taxiPointResult)>=abs(sherlockPoint))?taxiPointResult:sherlockPoint;
    if (abs(taxiPointResult)>abs(sherlockPoint))
    {
        // sherlock doesn't catch the taxi, decrease EXP and HP for Sherlock and Watson
        result = taxiPointResult;
        HP1=ceil(0.9*double(HP1));
        EXP1=ceil(0.88*double(EXP1));
        HP2=ceil(0.9*double(HP2));
        EXP2=ceil(0.88*double(EXP2));
    } else // sherlock catch the taxi, increase EXP and HP for Sherlock and Watson
    {
        result = sherlockPoint;
        HP1+=ceilFixed(0.1*HP1);
        EXP1+=ceilFixed(0.12*double(EXP1));
        HP2+=ceilFixed(0.1*double(HP2));
        EXP2+=ceilFixed(0.12*double(EXP2));
    }
    limitHP(HP1);limitHP(HP2);
    limitEXP(EXP1);limitEXP(EXP2);

    return result;
}

// Task 4
bool isUpCaseChar(char c)
{
    int start = 65;
    return (c>=start && c<=(start+25));
}

bool isLowCaseChar(char c)
{
    int start = 97;
    return (c>=start && c<=(start+25));
}

bool isNumberChar(char c)
{
    int start = 48;
    return (c>=start && c<=(start+9)); 
}

// main function for task 4
int checkPassword(const char * s, const char * e) 
{
    // TODO: Complete this function
    string str = s;
    string email = e;
    const string special = "@#%$!";
    int length = str.size();

    // about SE
    int apos = email.find("@");
    // cout << apos << endl;
    string se = email.substr(0,apos);
    // int sePos = (str[0]=='@')?(-1):str.find(se);
    int sePos = str.find(se);
    // cout << se<<" " << sePos << endl;

    // about continuous char
    bool continuous = false;
    int i;
    for (i=0; i<length-2; i++)
    {
        if (str[i]==str[i+1] && str[i+1]==str[i+2]) 
        {
            continuous = true;
            break;
        }
    }

    bool haveSpecial = false;
    for (int j = 0; j<length; j++)
    {
        if (special.find(str[j])!=-1)
        {
            haveSpecial = true;
            break;
        }
    }

    if (length<8) return -1; // too short
    if (length>20) return -2; // too long
    if (sePos!=-1) return -(300+sePos); // contain gmail
    if (continuous) return -(400+i); // contain a chain similar character
    if (!haveSpecial) return -5; // don't have special character
    
    // check for valid character
    for (i=0;i<length; i++)
    {
        if (!(isUpCaseChar(str[i]) 
        || (isLowCaseChar(str[i])) 
        || (isNumberChar(str[i])) 
        || (special.find(str[i])!=-1)
        )) return i;
    }

    return -10;
}

// Task 5
int findCorrectPassword(const char * arr[], int num_pwds)
{
    // TODO: Complete this function
    // each time an new string appear i add it to storage, then next time i see it i ignore
    // count amount of different string
    string arr_pwds[num_pwds];
    for (int i = 0; i<num_pwds; i++)
    {
        arr_pwds[i]=arr[i];
    }

    string storage = "";
    int count = 0;

    for (int i=0; i<num_pwds; i++)
    {
        if (storage.find(arr_pwds[i])==-1) 
        {
            storage+=arr_pwds[i]+" ";
            count++;
        }
    }

    // cpyArr and amount are used parallel
    // EX: pink123      pink#pink       abcxyz
    //           2              1            3
    // mean pink123 appear 2 times in original array

    int size_dis = count; // size of these couple
    string cpyArr[size_dis]; // clone an array with repeat string.
    int amount[size_dis];    // make an array contain many time an string appear

    count = 0;
    storage = "";

    for (int i = 0; i<num_pwds; i++)
    {
        if (storage.find(arr_pwds[i])==-1) 
        {
            storage+=arr_pwds[i]+" ";
            cpyArr[count] = arr_pwds[i];
            amount[count] = 0;
            count++;
        }
    }

    // for (int i = 0; i<size_dis; i++)
    // {
    //     cout <<setw(10)<< cpyArr[i] <<" ";
    // }
    // cout << endl;

    //counting many times a string appear and found most times an string appear
    int maxA = 0;
    for (int i = 0; i<size_dis; i++)
    {
        for (int j = 0; j <num_pwds; j++)
        {
            if (cpyArr[i]==arr_pwds[j]) amount[i]++;
            maxA = max(maxA, amount[i]);
        }
    }

    // for (int i = 0; i<size_dis; i++)
    // {
    //     cout <<setw(10)<< amount[i] <<" ";
    // }
    // cout << endl;

    // cout << "Most appear times: "<<maxA << endl;

    // Create 2 other array that
    // mostAppearPos contain position in cpyArr that appear most times
    // mostAppearString contain string in cpyArr that appear most times
    // they are used parallel
    count = 0;

    // determine numbers of elements
    for (int i = 0; i<size_dis; i++)
    {
        if (amount[i]==maxA) count++;
    }

    int size_most = count;                  // size of these couple
    int mostAppearPos[size_most];           // contain position of string that appear most
    string mostAppearString[size_most];     // contain string that appear most

    count = 0;

    for (int i = 0; i< size_dis; i++)
    {
        if (amount[i]==maxA)
        {
            mostAppearPos[count]=i;
            count++;
        }
    }
    
    for (int i = 0; i<size_most; i++)
    {
        mostAppearString[i] = cpyArr[mostAppearPos[i]];
    }

    // cout << "Can be correct passwords: " << endl;
    // for (int i = 0; i<size_most; i++)
    // {
    //     cout << mostAppearString[i] << " ";
    // }
    // cout << endl;

    // find correct password from most appear string, the longer password is the more correct it is.
    string correctPwd = mostAppearString[0];
    for (int i = 1; i<size_most; i++)
    {
        if (mostAppearString[i].length()>correctPwd.length()) correctPwd = mostAppearString[i];
    }
    // cout << "Correct Password: "<<correctPwd << endl;

    // find password position in original array
    // cout << "Position of correct password: "; 
    for (int i =0; i<num_pwds;i++)
    {
        if (correctPwd==arr_pwds[i]) return i;
    }

    return -99;
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////