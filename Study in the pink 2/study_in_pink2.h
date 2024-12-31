/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 2
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Student's answer for Assignment 2
* Programming Fundamentals Spring 2023
* Author: Nguyen Thanh Phat
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

class Configuration;
class TestStudyInPink;
class MovingObject; 
class BaseItem; 

void limit(int & number, int mini, int maxi);

int ceilFixed(double number);

//
//
// POSITION
//
//

class Position 
{
friend class TestStudyInPink;
private:
    int r, c;
public:
    static const Position npos;

    Position(int in_r=0, int in_c=0);
    Position(const string & str_pos);

    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);

    string str() const;
    bool isEqual(int in_r, int in_c) const;
    bool isEqual(Position pos) const;

    static int ManhattanDistance(Position pos_1, Position pos_2);
};

//
//
//  MAP ELEMENT
//
//
enum ElementType { PATH=0, WALL, FAKE_WALL };
string strElementType(ElementType type);

class MapElement 
{
friend class TestStudyInPink;
protected:
    ElementType type;
public:
    MapElement(ElementType in_type);
    MapElement();
    virtual ~MapElement();

    virtual ElementType getType() const;
};

class Path : public MapElement 
{
friend class TestStudyInPink;
public:
    Path();
    ~Path();

    ElementType getType();
};

class Wall : public MapElement 
{
friend class TestStudyInPink;
public:
    Wall();
    ~Wall();

    ElementType getType();
};

class FakeWall : public MapElement 
{
friend class TestStudyInPink;
private:
    int req_exp;
public:
    FakeWall(Position in_pos);
    FakeWall();
    ~FakeWall();

    ElementType getType();
    int getReqExp() const;
    void setReq_exp(int in_req_exp);

    static int calculateExp(Position in_pos);
    static int calculateExp(int in_r, int in_c);
};

//
//
// MAP ( MAZE )
//
//
class Map
{
friend class TestStudyInPink;
private:
    int num_rows;
    int num_cols;
    MapElement *** map;
public:
    Map(int in_rows, int in_cols, 
        int num_walls, Position * array_walls,
        int num_fake_walls, Position * array_fake_walls);
    ~Map();

    int getRow();
    int getCol();
    
    bool isValid(const Position & pos, MovingObject * mv_obj) const;
    string str() const;
};

//
//
// Moving Object
//
//
class MovingObject
{
friend class TestStudyInPink;
protected:
    int index;
    Position pos;
    Map * map;
    string name;
public:
    MovingObject(int in_index, const Position in_pos, Map * in_map, const string & in_name);
    virtual ~MovingObject();

    string getName();
    void setPosition(Position pos);

    virtual Position getCurrentPosition() const = 0;
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual bool isCharacter() const = 0;

    bool meet(MovingObject *);
};

// character
// stage:
// 0: mean normal
// 1: meet robot (find card to use)
// 2: complete robot challenge (find healing item)
class Character : public MovingObject
{
friend class TestStudyInPink;
public:
    Character(int in_index, const Position in_pos, Map * in_map, const string & in_name);
    ~Character();

    bool isCharacter() const;
};

const int maxHP = 500;
const int maxEXP = 900;

class Sherlock : public Character
{
friend class TestStudyInPink;
private:
    int hp;
    int exp;
    string moving_rule;
    int calledNext;
    bool excemp;
    int stage;
public:
    Sherlock(int index, const string & in_moving_rule, const Position & init_pos,
             Map * map, int init_hp, int init_exp);
    ~Sherlock();

    int getHp();
    void setHp(int in_hp);
    int getExp();
    void setExp(int in_exp);
    int getStage();
    void setStage(int in_st);
    bool isExcemp();
    void setExcemp(bool in_excemp);
    string getMovingRule();

    Position getCurrentPosition() const;
    Position getNextPosition();
    void move();
    string str() const;
};

class Watson : public Character
{
friend class TestStudyInPink;
private:
    int hp;
    int exp;
    string moving_rule;
    int calledNext;
    bool pass;
    int stage;
public:
    Watson(int index, const string & in_moving_rule, const Position & init_pos,
             Map * map, int init_hp, int init_exp);
    ~Watson();

    int getHp();
    void setHp(int in_hp);
    int getExp();
    void setExp(int in_exp);
    int getStage();
    void setStage(int in_st);
    string getMovingRule();
    bool canPass();
    void setPass(bool in_pass);

    Position getCurrentPosition() const;
    Position getNextPosition();
    void move();
    string str() const;
};

class Criminal : public Character
{
friend class TestStudyInPink;
private:
    Sherlock * sherlock;
    Watson * watson;
    int step;
    Position lastPos;
public:
    Criminal(int index, const Position & init_pos, Map * map, Sherlock * in_sherlock, Watson * in_watson);
    ~Criminal();

    Sherlock* getSherlock();
    Watson* getWatson();
    int getStep();

    Position getLastPosition() const;
    Position getCurrentPosition() const;
    Position getNextPosition();
    void move();
    string str() const;
};

// Robot
enum RobotType { C=0, S, W, SW };
string strRobotType(RobotType type);

class Robot: public MovingObject
{
friend class TestStudyInPink;
private:
    BaseItem * item;
    RobotType type;
public:
    Robot(int in_index, const Position in_pos, Map * in_map, const string & in_name, RobotType in_type);
    virtual ~Robot();

    BaseItem * getItem();
    string getItemStr();
    RobotType getType();
    bool isCharacter() const;
};

class RobotC: public Robot
{
friend class TestStudyInPink;
private:
    Criminal * criminal;
public:
    RobotC(int index, const Position & init_pos, Map * map, Criminal* in_criminal);
    ~RobotC();

    Position getCurrentPosition() const;
    Position getNextPosition();
    void move();
    string str() const;
    int getDistance(Sherlock* sherlock) const;
    int getDistance(Watson* watson) const;
};

class RobotS: public Robot
{
friend class TestStudyInPink;
private:
    Criminal * criminal;
    Sherlock * sherlock;
public:
    RobotS( int index, const Position & init_pos, Map * map,
            Criminal* in_criminal, Sherlock* in_Sherlock);
    ~RobotS();

    Position getCurrentPosition() const;
    Position getNextPosition();
    void move();
    string str() const;
    int getDistance() const;
};

class RobotW: public Robot
{
friend class TestStudyInPink;
private:
    Criminal * criminal;
    Watson * watson;
public:
    RobotW( int index, const Position & init_pos, Map * map, 
            Criminal* in_criminal, Watson* in_watson);
    ~RobotW();

    Position getCurrentPosition() const;
    Position getNextPosition();
    void move();
    string str() const;
    int getDistance() const;
};

class RobotSW: public Robot
{
friend class TestStudyInPink;
private:
    Criminal * criminal;
    Sherlock * sherlock;
    Watson * watson;
public:
    RobotSW( int index, const Position & init_pos, Map * map, 
            Criminal* in_criminal, Sherlock* in_Sherlock, Watson* in_watson);
    ~RobotSW();

    Position getCurrentPosition() const;
    Position getNextPosition();
    void move();
    string str() const;
    int getDistance() const;
};

//
//
// Array moving object
//
//
class ArrayMovingObject
{
friend class TestStudyInPink;
friend class StudyPinkProgram;
private:
    MovingObject* *arr_mv_objs; // 1-D array of Pointer
    int count;
    int capacity;
public:
    ArrayMovingObject(int in_cap);
    ~ArrayMovingObject();

    // static void initialize(); 
    int getCapacity();
    bool isFull() const;
    bool add(MovingObject * mv_obj);
    MovingObject * get(int index) const;
    int size() const;
    string str() const;
};

//
//
// Configuration
//
//
class Configuration 
{
friend class TestStudyInPink;
friend class StudyPinkProgram;

protected:
    int map_num_rows;
    int map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position * arr_walls;
    int num_fake_walls;
    Position * arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps;
public:
    Configuration(const string & filepath);
    ~Configuration();
    string str() const;
    int getSteps();
    static string getPreStr(string str);
    static string getInfStr(string str);
    static int countDotComma(string str);
    static Position * getPositionArray(string str);
    static string positionArrStr(Position * pos_arr, int size);
};

//
//
// BASE ITEM
//
//

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
string strItemType(ItemType type);

class BaseItem
{
friend class TestStudyInPink;
private:
    ItemType type;
public:
    BaseItem(ItemType type);
    ~BaseItem();

    ItemType getType();

    virtual bool canUse(Character* obj, Robot * robot=NULL) = 0;
    virtual bool canUse_St(Character* obj, Robot * robot=NULL) = 0;
    virtual void use(Character* obj, Robot * robot=NULL) = 0;
};

//Description: contain lots of knowledge, help character recover up to 25% of EXP
//Require: exp<=350
class MagicBook : public BaseItem
{
friend class TestStudyInPink;
public:
    MagicBook();
    ~MagicBook();

    bool canUse(Character* obj, Robot * robot=NULL);
    bool canUse_St(Character* obj, Robot * robot=NULL);
    void use(Character* obj, Robot * robot=NULL);
};
//Description: Recover up to 20% of HP
//Require: hp<=100
class EnergyDrink: public BaseItem
{
friend class TestStudyInPink;
public:
    EnergyDrink();
    ~EnergyDrink();

    bool canUse(Character* obj, Robot * robot=NULL);
    bool canUse_St(Character* obj, Robot * robot=NULL);
    void use(Character* obj, Robot * robot=NULL);
};
//Description: Recover up to 50% of HP
//Require: exp<=350 or hp<=100
class FirstAid: public BaseItem
{
friend class TestStudyInPink;
public:
    FirstAid();
    ~FirstAid();

    bool canUse(Character* obj, Robot * robot=NULL);
    bool canUse_St(Character* obj, Robot * robot=NULL);
    void use(Character* obj, Robot * robot=NULL);
};
//Description: HP and EXP protection if character fail Robot's challenge
//Require: character is Sherlock and Hp is odd
class ExcemptionCard: public BaseItem
{
friend class TestStudyInPink;
public:
    ExcemptionCard();
    ~ExcemptionCard();

    bool canUse(Character* obj, Robot * robot=NULL);
    bool canUse_St(Character* obj, Robot * robot=NULL);
    void use(Character* obj, Robot * robot=NULL);
};
//Description: ignore challenge if card has same challenge property; if these are not same -50EXP
//Require: character is Watson and HP is even
class PassingCard: public BaseItem
{
friend class TestStudyInPink;
private:
    string challenge;
public:
    PassingCard(int r, int c);
    PassingCard(int t);
    PassingCard(string challenge);
    ~PassingCard();

    string getChallenge();
    bool canUse(Character* obj, Robot * robot=NULL);
    bool canUse_St(Character* obj, Robot * robot=NULL);
    void use(Character* obj, Robot * robot=NULL);
};

//
//
// Inventory (Bag)
//
//
class ItemNode
{
friend class TestStudyInPink;
public:
    BaseItem * item;
    ItemNode * link;
    ItemNode(BaseItem*, ItemNode*);
    ~ItemNode();
};

class BaseBag
{
friend class TestStudyInPink;
protected:
    int size;
    int current;
    Character * obj;
    ItemNode * head;
public:
    BaseBag(int, Character *);
    virtual ~BaseBag();

    virtual bool insert(BaseItem* item);
    virtual BaseItem * get();
    virtual BaseItem * get_St();
    virtual BaseItem * get(ItemType);
    virtual BaseItem * get_NC(ItemType);
    virtual string str() const;

    bool isFull() const;
    bool add(BaseItem*);
    bool insert(int, BaseItem *);
    void printChain();
    bool pop_front();
    bool swap(int, int);
    // bool remove(int);
    // int getFirst(ItemType);
    // BaseItem * get_NS(ItemType);
};

class SherlockBag: public BaseBag
{
friend class TestStudyInPink;
public:
    SherlockBag(Sherlock *);
    ~SherlockBag();
    bool insert(BaseItem* item);
    BaseItem * get();
    BaseItem * get_St();
    BaseItem * get(ItemType);
    BaseItem * get_NC(ItemType);
    string str() const;
};

class WatsonBag: public BaseBag
{
friend class TestStudyInPink;
public:
    WatsonBag(Watson *);
    ~WatsonBag();
    bool insert(BaseItem* item);
    BaseItem * get();
    BaseItem * get_St();
    BaseItem * get(ItemType);
    BaseItem * get_NC(ItemType);
    string str() const;
};
//
//
// Program
//
//
class StudyPinkProgram 
{
friend class TestStudyInPink;

private:
    Configuration * config;
    
    Sherlock * sherlock;
    SherlockBag * sherlockBag;
    Watson * watson;
    WatsonBag * watsonBag;
    Criminal * criminal;
    
    Map * map;
    ArrayMovingObject * arr_mv_objs;
public:
    StudyPinkProgram(const string & config_file_path);

    bool isStop() const;
    void printResult() const; 
    void printStep(int) const;
    void run(bool); 

    void showAll();
    void spawnRobot(int, int &, bool);
    void CriminalEvents(bool);
    void SherlockEvents(bool);
    void WatsonEvents(bool);
    void RobotEvents(MovingObject*, bool);

    ~StudyPinkProgram();
};

void C_challenge_S(Sherlock *, SherlockBag*, Criminal *, RobotC *, bool);
void S_challenge_S(Sherlock *, SherlockBag*, RobotS *, bool);
void W_challenge_S(SherlockBag*, RobotW*, bool);
void SW_challenge_S(Sherlock *, SherlockBag*, RobotSW *, bool);
void C_challenge_W(Watson *, WatsonBag*, RobotC *, bool);
void S_challenge_W(bool);
void W_challenge_W(Watson *,WatsonBag*, RobotW *, bool);
void SW_challenge_W(Watson *,WatsonBag*, RobotSW *, bool);
void tradeCard(Sherlock *, SherlockBag*, Watson* ,WatsonBag*, bool);
void S_heal(Sherlock *, SherlockBag*, bool);
void W_heal(Watson *, WatsonBag*, bool);

// for testing.
void WatsonEvents(Sherlock*, SherlockBag* ,Watson*, WatsonBag*, MovingObject*, bool);
void SherlockEvents(Sherlock*, SherlockBag*, Watson*, WatsonBag* ,Criminal*, MovingObject*, bool);
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////

#endif /* _H_STUDY_IN_PINK_2_H_ */