#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////


void limit(int & number, int mini, int maxi)
{
    if (number<mini) number = mini;
    if (number>maxi) number = maxi;
}

int ceilFixed(double number)
{
    int toReturn = ceil(number);
    if ((number - (int) number) < 1e-10) return (int) number;
    return toReturn;
}

//
//
// POSITION 
//
//

Position::Position(int in_r, int in_c)
{
    r = in_r;
    c = in_c;
}

Position::Position(const string & str_pos)
{
    int bracketLPos = str_pos.find("(");
    int bracketRPos = str_pos.find(")");
    int commaPos = str_pos.find(",");

    string r_str = str_pos.substr(bracketLPos+1, commaPos-(bracketLPos+1));
    string c_str = str_pos.substr(commaPos+1, bracketRPos-(commaPos+1));
    // cout << r_str << " " << c_str << endl;
    r = stoi(r_str);
    c = stoi(c_str);
}

int Position::getRow() const {return r;};
int Position::getCol() const {return c;};
void Position::setRow(int in_r) {r=in_r;};
void Position::setCol(int in_c) {c=in_c;};

string Position::str() const
{
    return ("("+to_string(r)+","+to_string(c)+")");
};

bool Position::isEqual(int in_r, int in_c) const
{
    return ((in_r==r) && (in_c==c));
};
bool Position::isEqual(Position pos) const
{
    return ((pos.r==r) && (pos.c==c));
};
int Position::ManhattanDistance(Position pos_1, Position pos_2)
{
    return abs(pos_1.getCol()-pos_2.getCol()) + abs(pos_1.getRow()-pos_2.getRow());
}

const Position Position::npos(-1,-1);


//
//
// ABOUT MAP ELEMENT
//
//
string strElementType(ElementType type)
{
    switch (type)
    {
    case 0:
        return "PATH";
        break;
    case 1:
        return "WALL";
        break;
    case 2:
        return "FAKE_WALL";
        break;
    default:
        return "NULL ELEMENT TYPE";
        break;
    }
    return "NULL ELEMENT TYPE";
}

MapElement::MapElement(ElementType in_type)
{
    type = in_type;
};
MapElement::MapElement()
{
    type = PATH;
};
MapElement::~MapElement() {};

ElementType MapElement::getType() const
{
    return type;
};


// Path
Path::Path(): MapElement(PATH) {};
Path::~Path() {};

ElementType Path::getType()
{
    return PATH;
}


// Wall
Wall::Wall(): MapElement(WALL) {};
Wall::~Wall() {};

ElementType Wall::getType()
{
    return WALL;
}

// Fake Wall
FakeWall::FakeWall(Position in_pos): MapElement(FAKE_WALL) 
{
    setReq_exp(calculateExp(in_pos));
};
FakeWall::FakeWall(): MapElement(FAKE_WALL) {
};
FakeWall::~FakeWall() {};

ElementType FakeWall::getType()
{
    return FAKE_WALL;
}

int FakeWall::getReqExp() const
{
    return req_exp;
}
void FakeWall::setReq_exp(int in_req_exp)
{
    req_exp = in_req_exp;
}
int FakeWall::calculateExp(Position in_pos)
{
    return (in_pos.getRow()*257+in_pos.getCol()*139+89)%900+1;
}
int FakeWall::calculateExp(int in_r, int in_c)
{
    return (in_r*257+in_c*139+89)%900+1;
}


//
//
// ABOUT MAP (MAZE)
//
//
Map::Map(int in_rows, int in_cols, 
        int num_walls, Position * array_walls,
        int num_fake_walls, Position * array_fake_walls)
{
    num_rows = in_rows;
    num_cols = in_cols;

    // all are paths
    map = new MapElement ** [in_rows];
    for (int i = 0 ; i<in_rows; i++)
    {
        map[i] = new MapElement * [in_cols];
    }
    for (int i = 0; i<in_rows; i++)
    {
        for (int j = 0 ;j<in_cols; j++)
        {
            map[i][j] = new Path();
        }
    }

    // generate walls
    for (int i = 0; i<num_walls; i++)
    {
        Position pos = array_walls[i];
        // cout << pos.getRow() << " " << pos.getCol() << endl;
        delete map[pos.getRow()][pos.getCol()];
        Wall * w = new Wall();
        map[pos.getRow()][pos.getCol()] = w;
    }

    // generate fake walls
    for (int i = 0; i<num_fake_walls; i++)
    {
        Position pos = array_fake_walls[i];
        delete map[pos.getRow()][pos.getCol()];
        FakeWall * fw = new FakeWall(pos);
        map[pos.getRow()][pos.getCol()] = fw;
    }
};
Map::~Map() 
{
    for (int i = 0; i<num_rows; i++)
    {
        for (int j = 0; j<num_cols; j++)
        {
            if (map[i][j]!=NULL) delete map[i][j];
        }
        if (map[i]!=NULL) delete[] map[i];
    }
};

int Map::getCol() {return num_cols;}
int Map::getRow() {return num_rows;}

bool Map::isValid(const Position & pos, MovingObject * mv_obj) const
{
    int r = pos.getRow();
    int c = pos.getCol();

    if (c<0 || c>=num_cols) return false;
    if (r<0 || r>=num_rows) return false;
    if (mv_obj==NULL) return false;
    
    if (map[r][c]->getType()==WALL) return false;

    if (mv_obj->isCharacter())
    { 
        if (mv_obj->getName()=="Watson" && map[r][c]->getType()==FAKE_WALL)
        {
            Watson * w = (Watson*) mv_obj;
            if (w->getExp()<=FakeWall::calculateExp(pos)) return false;
        }
    }
    return true;
};

string Map::str() const
{
    stringstream toReturn;
    for (int i = 0; i<num_rows; i++)
    {
        for (int j = 0; j<num_cols; j++)
        {
            toReturn << "[" << map[i][j]->getType() <<"] ";
        }
        toReturn << endl;
    }
    return toReturn.str();
}
// 
//
// Moving Object
//
//
MovingObject::MovingObject(int in_index, const Position in_pos, Map * in_map, const string & in_name)
{
    index =in_index;
    pos = in_pos;
    map = in_map;
    name = in_name;
};
MovingObject::~MovingObject() {};

string MovingObject::getName() 
{
    return name;
};
void MovingObject::setPosition(Position in_pos) {pos = in_pos;}

Position MovingObject::getCurrentPosition() const
{
    return pos;
};
Position MovingObject::getNextPosition() {return Position::npos;};
void MovingObject::move() {};
string MovingObject::str() const {return "";};
bool MovingObject::isCharacter() const {return false;};
bool MovingObject::meet(MovingObject * obj)
{
    return (pos.isEqual(obj->pos));
};

//Character
Character::Character(int in_index, const Position in_pos, Map * in_map, const string & in_name) :
    MovingObject(in_index, in_pos, in_map, in_name) {};
Character::~Character() {};

bool Character::isCharacter() const {return true;};


// Sherlock
Sherlock::Sherlock(int index, const string & in_moving_rule, const Position & init_pos,
             Map * map, int init_hp, int init_exp) :
    Character(index, init_pos, map, "Sherlock")
    {
        hp = init_hp;
        exp = init_exp;
        moving_rule = in_moving_rule;
        calledNext = 0;
        stage = 0;
        excemp = 0;
    }
Sherlock::~Sherlock() {};

int Sherlock::getHp() {return hp;};
void Sherlock::setHp(int in_hp) 
{
    hp = in_hp;
    limit(hp,0,maxHP);
};
int Sherlock::getExp() {return exp;};
void Sherlock::setExp(int in_exp)
{
    exp = in_exp;
    limit(exp,0,maxEXP);
};
int Sherlock::getStage() {return stage;};
void Sherlock::setStage(int in_st) {stage=in_st;};
bool Sherlock::isExcemp() {return excemp;};
void Sherlock::setExcemp(bool in_excemp) {excemp = in_excemp;};
string Sherlock::getMovingRule() {return moving_rule;};

Position Sherlock::getCurrentPosition() const {return pos;};
Position Sherlock::getNextPosition() {
    char direction = moving_rule[calledNext%moving_rule.size()];
    calledNext++;

    Position toReturn;
    switch (direction)
    {
    case 'U': // up
        toReturn = Position(pos.getRow()-1, pos.getCol());
        break;
    case 'L': // left
        toReturn = Position(pos.getRow(), pos.getCol()-1);
        break;
    case 'D': // down
        toReturn = Position(pos.getRow()+1, pos.getCol());
        break;
    case 'R': // right
        toReturn = Position(pos.getRow(), pos.getCol()+1);
        break;
    default:
        break;
    }
    if (!map->isValid(toReturn, this)) toReturn = Position::npos;

    return toReturn;
};
void Sherlock::move()
{
    Position nextPos = getNextPosition();
    if (!nextPos.isEqual(Position::npos)) pos = nextPos;
    // cout << pos.str() << endl;
};
string Sherlock::str() const {
    return "Sherlock[index="+to_string(index)+";pos="+pos.str()+";moving_rule="+moving_rule+"]";
};

// Watson
Watson::Watson(int index, const string & in_moving_rule, const Position & init_pos,
             Map * map, int init_hp, int init_exp) :
    Character(index, init_pos, map, "Watson")
    {
        hp = init_hp;
        exp = init_exp;
        moving_rule = in_moving_rule;
        calledNext = 0;
        stage = 0;
        pass = 0;
    }
Watson::~Watson() {};

int Watson::getHp() {return hp;};
void Watson::setHp(int in_hp) 
{
    hp = in_hp;
    limit(hp,0,maxHP);
};
int Watson::getExp() {return exp;};
void Watson::setExp(int in_exp)
{
    exp = in_exp;
    limit(exp,0,maxEXP);
};
int Watson::getStage() {return stage;};
void Watson::setStage(int in_st) {stage=in_st;};
bool Watson::canPass() {return pass;};
void Watson::setPass(bool in_pass) {pass = in_pass;};
string Watson::getMovingRule() {return moving_rule;};

Position Watson::getCurrentPosition() const {return pos;};
Position Watson::getNextPosition() {
    char direction = moving_rule[calledNext%moving_rule.size()];
    calledNext++;

    Position toReturn;
    switch (direction)
    {
    case 'U': // up
        toReturn = Position(pos.getRow()-1, pos.getCol());
        break;
    case 'L': // left
        toReturn = Position(pos.getRow(), pos.getCol()-1);
        break;
    case 'D': // down
        toReturn = Position(pos.getRow()+1, pos.getCol());
        break;
    case 'R': // right
        toReturn = Position(pos.getRow(), pos.getCol()+1);
        break;
    default:
        break;
    }
    if (!map->isValid(toReturn, this)) toReturn = Position::npos;

    return toReturn;
};
void Watson::move() {
    Position nextPos = getNextPosition();
    if (!nextPos.isEqual(Position::npos)) pos = nextPos;
    // cout << pos.str() << endl;
};
string Watson::str() const 
{
    return "Watson[index="+to_string(index)+";pos="+pos.str()+";moving_rule="+moving_rule+"]";
};

// criminal
Criminal::Criminal(int index, const Position & init_pos, Map * map, Sherlock * in_sherlock, Watson * in_watson) :
    Character(index, init_pos, map, "Criminal") 
{
    sherlock = in_sherlock;
    watson = in_watson;
    step = 0;
    lastPos = Position::npos;
};
Criminal::~Criminal() {};

Position Criminal::getLastPosition() const {return lastPos;};
Position Criminal::getCurrentPosition() const {return pos;};
Position Criminal::getNextPosition() 
{
    int c = pos.getCol();
    int r = pos.getRow();

    //      U0
    //  L1       R3
    //      D2
    Position pos[] = {Position(r-1,c), Position(r, c-1), Position(r+1,c), Position(r,c+1)};
    bool valid[4] = {map->isValid(pos[0], this), map->isValid(pos[1], this),
                    map->isValid(pos[2], this), map->isValid(pos[3], this)};
    // for (int i = 0; i<4; i++)
    // {
    //     valid[i] = valid[i] && !pos[i].isEqual(sherlock->getCurrentPosition()) && !pos[i].isEqual(watson->getCurrentPosition());
    // }

    // int currentDis = Position::ManhattanDistance(getCurrentPosition(), sherlock->getCurrentPosition())
    //                 + Position::ManhattanDistance(getCurrentPosition(), watson->getCurrentPosition());
    int distance[4] = {0,0,0,0};
    // int max = currentDis;
    int max = 0;
    for (int i = 0; i<4; i++) 
    {
        if (valid[i]) distance[i] = Position::ManhattanDistance(pos[i],sherlock->getCurrentPosition())
                    + Position::ManhattanDistance(pos[i],watson->getCurrentPosition());
        if (distance[i]>max) max = distance[i];
    }

    if (max == 0) return Position::npos;

    for (int i = 0; i<4; i++)
    {
        if (distance[i]==max) return pos[i];
    }

    return Position::npos;
};
int Criminal::getStep() {return step;}
void Criminal::move() {
    Position nextPos = getNextPosition();
    if (!nextPos.isEqual(Position::npos)) 
    {
        lastPos = pos;
        pos = nextPos;
        step++;
    }
    // cout << pos.str() << endl;
}; 
string Criminal::str() const
{
    return "Criminal[index="+to_string(index)+";pos="+pos.str()+"]";
};
Sherlock* Criminal::getSherlock() {return sherlock;}
Watson* Criminal::getWatson() {return watson;}
//Robot
string strRobotType(RobotType type)
{
    switch (type)
    {
    case 0:
        return "C";
        break;
    case 1:
        return "S";
        break;
    case 2:
        return "W";
        break;
    case 3:
        return "SW";
        break;
    default:
        return "NULL ROBOT TYPE";
        break;
    }
    return "NULL ROBOT TYPE";
}

Robot::Robot(int in_index, const Position in_pos, Map * in_map, const string & in_name, RobotType in_type):
    MovingObject(in_index, in_pos, in_map, in_name)
{
    type = in_type;
    int p = in_pos.getCol()*in_pos.getRow();
    while (p/10!=0)
    {
        p=p/10+p%10;
    }

    if (p>=0 && p<=1)
    {
        item = new MagicBook();
    }
    else if (p>=2 && p<=3)
    {
        item = new EnergyDrink();
    }
    else if (p>=4 && p<=5)
    {
        item = new FirstAid();
    }
    else if (p>=6 && p<=7)
    {
        item = new ExcemptionCard();
    }
    else if (p>=8 && p<=9)
    {
        int t = (in_pos.getRow()*11+in_pos.getCol())%4;
        item = new PassingCard(t);
    }
};
Robot::~Robot() {if (item!=NULL) delete item;};

BaseItem * Robot::getItem() {return item;}
string Robot::getItemStr() 
{
    return strItemType(item->getType());
}
RobotType Robot::getType() {return type;};
bool Robot::isCharacter() const {return false;};

// robot C
RobotC::RobotC(int index, const Position & init_pos, Map * map, Criminal* in_criminal):
    Robot(index, init_pos, map, "RobotC", C)
{
    criminal = in_criminal;
};
RobotC::~RobotC() {};

Position RobotC::getCurrentPosition() const {return pos;};
Position RobotC::getNextPosition()
{
    if (pos.isEqual(criminal->getLastPosition())) return Position::npos;
    else return criminal->getLastPosition();
};
void RobotC::move() {
    if (!getNextPosition().isEqual(Position::npos)) pos = getNextPosition();
};
string RobotC::str() const {
    return "Robot[pos="+pos.str()+";type="+strRobotType(C)+";dist=]";
};
int RobotC::getDistance(Sherlock* sherlock) const
{
    return Position::ManhattanDistance(pos, sherlock->getCurrentPosition());
};
int RobotC::getDistance(Watson* watson) const
{
    return Position::ManhattanDistance(pos, watson->getCurrentPosition());
};

// robot S
RobotS::RobotS(int index, const Position & init_pos, Map * map, Criminal* in_criminal, Sherlock* in_sherlock):
    Robot(index, init_pos, map, "RobotS", S)
{
    criminal = in_criminal;
    sherlock = in_sherlock;
};
RobotS::~RobotS() {};

Position RobotS::getCurrentPosition() const {return pos;};
Position RobotS::getNextPosition()
{
    int c = pos.getCol();
    int r = pos.getRow();
    int maxDis = map->getCol()+map->getRow()-2;

    //      U0
    //  L3       R1
    //      D2
    Position pos[] = {Position(r-1,c), Position(r, c+1), Position(r+1,c), Position(r,c-1)};
    bool valid[4] = {map->isValid(pos[0], this), map->isValid(pos[1], this),
                    map->isValid(pos[2], this), map->isValid(pos[3], this)};

    int distance[4] = {maxDis, maxDis, maxDis, maxDis};
    int min = maxDis;
    for (int i = 0; i<4; i++) 
    {
        if (valid[i]) distance[i] = Position::ManhattanDistance(pos[i],sherlock->getCurrentPosition());
        if (distance[i]<min) min = distance[i];
    }
    // for (int i = 0; i<4; i++) cout << distance[i] << " ";
    // cout << endl;
    // cout << min << endl;

    if (min == maxDis) return Position::npos;

    for (int i = 0; i<4; i++)
    {
        if (distance[i]==min) return pos[i];
    }

    return Position::npos;
};
void RobotS::move() {
    Position nextPos = getNextPosition();
    if (!nextPos.isEqual(Position::npos)) pos=nextPos;
};
int RobotS::getDistance() const {
    return Position::ManhattanDistance(pos, sherlock->getCurrentPosition());
};
string RobotS::str() const {
    return "Robot[pos="+pos.str()+";type="+strRobotType(S)+";dist="+to_string(getDistance())+"]";
};

// robot W
RobotW::RobotW( int index, const Position & init_pos, Map * map, Criminal* in_criminal, Watson* in_watson):
    Robot(index, init_pos, map, "RobotW", W)
{
    criminal = in_criminal;
    watson = in_watson;
};
RobotW::~RobotW() {};

Position RobotW::getCurrentPosition() const {return pos;};
Position RobotW::getNextPosition() 
{
    int c = pos.getCol();
    int r = pos.getRow();
    int maxDis = map->getCol()+map->getRow()-2;

    //      U0
    //  L3       R1
    //      D2
    Position pos[] = {Position(r-1,c), Position(r, c+1), Position(r+1,c), Position(r,c-1)};
    bool valid[4] = {map->isValid(pos[0], this), map->isValid(pos[1], this),
                    map->isValid(pos[2], this), map->isValid(pos[3], this)};

    int distance[4] = {maxDis, maxDis, maxDis, maxDis};
    int min = maxDis;
    for (int i = 0; i<4; i++) 
    {
        if (valid[i]) distance[i] = Position::ManhattanDistance(pos[i],watson->getCurrentPosition());
        if (distance[i]<min) min = distance[i];
    }

    if (min == maxDis) return Position::npos;

    for (int i = 0; i<4; i++)
    {
        if (distance[i]==min) return pos[i];
    }

    return Position::npos;
};
void RobotW::move() {
    if (!getNextPosition().isEqual(Position::npos)) pos=getNextPosition();
};
int RobotW::getDistance() const {
    return Position::ManhattanDistance(pos, watson->getCurrentPosition());
};
string RobotW::str() const {
    return "Robot[pos="+pos.str()+";type="+strRobotType(W)+";dist="+to_string(getDistance())+"]";
};

// robot SW
RobotSW::RobotSW( int index, const Position & init_pos, Map * map, 
            Criminal* in_criminal,Sherlock* in_sherlock, Watson* in_watson):
    Robot(index, init_pos, map, "RobotSW", SW)
{
    criminal = in_criminal;
    sherlock = in_sherlock;
    watson = in_watson;
};
RobotSW::~RobotSW() {};

Position RobotSW::getCurrentPosition() const {return pos;};
Position RobotSW::getNextPosition()
{
    int c = pos.getCol();
    int r = pos.getRow();
    int maxDis = (map->getCol()+map->getRow()-2)*2;

    //      UU0
    //   UL7   UR1
    //LL6   ORI   RR2
    //   DL5   DR3
    //      DD4
    Position pos[] = {Position(r-2,c), Position(r-1, c+1), Position(r,c+2), Position(r+1,c+1),
                        Position(r+2,c), Position(r+1,c-1), Position(r,c-2), Position(r-1,c-1)};
    // for (int i = 0;i <8; i++) cout <<pos[i].str() << " ";
    // cout << endl;

    bool valid[8] = {map->isValid(pos[0], this), map->isValid(pos[1], this),
                    map->isValid(pos[2], this), map->isValid(pos[3], this),
                    map->isValid(pos[4], this), map->isValid(pos[5], this),
                    map->isValid(pos[6], this), map->isValid(pos[7], this)};
    // for (int i = 0;i <8; i++) cout <<valid[i] << " ";
    // cout << endl;

    int distance[8] = {maxDis, maxDis, maxDis, maxDis, maxDis, maxDis, maxDis, maxDis};
    int min = maxDis;
    for (int i = 0; i<8; i++) 
    {
        if (valid[i]) distance[i] = Position::ManhattanDistance(pos[i],sherlock->getCurrentPosition())
                                + Position::ManhattanDistance(pos[i],watson->getCurrentPosition());
        if (distance[i]<min) min = distance[i];
    }
    // for (int i = 0;i <8; i++) cout <<distance[i] << " ";
    // cout << endl;

    if (min == maxDis) return Position::npos;

    for (int i = 0; i<8; i++)
    {
        if (distance[i]==min) return pos[i];
    }

    return Position::npos;
};
void RobotSW::move() {
    if (!getNextPosition().isEqual(Position::npos)) pos=getNextPosition();
};
int RobotSW::getDistance() const {
    return Position::ManhattanDistance(pos, sherlock->getCurrentPosition())
            + Position::ManhattanDistance(pos,watson->getCurrentPosition());
};
string RobotSW::str() const {
    return "Robot[pos="+pos.str()+";type="+strRobotType(SW)+";dist="+to_string(getDistance())+"]";
};

//
//
// Array Moving Object
//
//
ArrayMovingObject::ArrayMovingObject(int in_cap)
{
    count = 0;
    capacity = in_cap;
    arr_mv_objs = new MovingObject * [capacity];
}
ArrayMovingObject::~ArrayMovingObject() 
{
    // for (int i = 0; i<count; i++)
    // {
    //     if (arr_mv_objs[i]!=NULL) delete arr_mv_objs[i];
    // }
    if (arr_mv_objs!=NULL) delete [] arr_mv_objs;
};

// void ArrayMovingObject::initialize()/* undone */
// {

// }; 
int ArrayMovingObject::getCapacity() {return capacity;};
bool ArrayMovingObject::isFull() const {return (count==capacity);};
bool ArrayMovingObject::add(MovingObject * mv_obj) 
{
    if (isFull()) return false;
    arr_mv_objs[count] = mv_obj;
    count++;
    // cout << count << endl;; 
    return true;
};
MovingObject * ArrayMovingObject::get(int index) const {return arr_mv_objs[index];};
int ArrayMovingObject::size() const {return count;}; // return current number of elements in the array
string ArrayMovingObject::str() const 
{
    string toReturn = "";
    toReturn = "ArrayMovingObject[count="+to_string(count)+";capacity="+to_string(capacity)+";";
    for (int i = 0; i<count; i++)
    {
        toReturn += arr_mv_objs[i]->str();
        if (i!=count-1) toReturn+= ";";
    } 
    toReturn+= "]";
    return toReturn;
};

//
//
// Configuration
//
//
Configuration::Configuration(const string & filepath)
{
    map_num_rows = 0;
    map_num_cols = 0;
    max_num_moving_objects = 0;
    num_walls = 0;
    num_fake_walls = 0;
    sherlock_moving_rule = "";
    sherlock_init_hp = 0;
    sherlock_init_exp = 0;
    watson_moving_rule = "";
    watson_init_hp = 0;
    watson_init_exp = 0;
    num_steps = 0;

    string gotText;
    ifstream file(filepath);

    while (getline (file, gotText)) 
    {
        // cout << gotText << endl;
        string pre = getPreStr(gotText);
        string inf = getInfStr(gotText);
        // cout << pre << " " << inf << endl;
        // cout << "what?" << endl;

        if (pre == "MAP_NUM_ROWS")
        {
            map_num_rows = stoi(inf);
        } 
        else if (pre == "MAP_NUM_COLS")
        {
            map_num_cols = stoi(inf);
        } 
        else if (pre == "MAX_NUM_MOVING_OBJECTS")
        {
            max_num_moving_objects = stoi(inf);
        } 
        else if (pre == "NUM_WALLS") 
        {
            num_walls = stoi(inf);
        }
        else if (pre == "ARRAY_WALLS") 
        {
            if (inf!="[]") num_walls = countDotComma(inf)+1;
            arr_walls = getPositionArray(inf);
        } 
        else if (pre == "NUM_FAKE_WALLS") 
        {
            num_fake_walls = stoi(inf);
        }
        else if (pre == "ARRAY_FAKE_WALLS") 
        {
            if (inf!="[]") num_fake_walls = countDotComma(inf)+1;
            arr_fake_walls = getPositionArray(inf);
        }
        else if (pre == "SHERLOCK_MOVING_RULE")
        {
            sherlock_moving_rule = inf;
        }
        else if (pre == "SHERLOCK_INIT_POS")
        {
            sherlock_init_pos = Position(inf);
        }
        else if (pre == "SHERLOCK_INIT_HP")
        {
            sherlock_init_hp = stoi(inf);
        }
        else if (pre == "SHERLOCK_INIT_EXP")
        {
            sherlock_init_exp = stoi(inf);
        }
        else if (pre == "WATSON_MOVING_RULE")
        {
            watson_moving_rule = inf;
        }
        else if (pre == "WATSON_INIT_POS")
        {
            watson_init_pos = Position(inf);
        }
        else if (pre == "WATSON_INIT_HP")
        {
            watson_init_hp = stoi(inf);
        }
        else if (pre == "WATSON_INIT_EXP")
        {
            watson_init_exp = stoi(inf);
        }
        else if (pre == "NUM_STEPS")
        {
            num_steps = stoi(inf);
        }
        else if (pre == "CRIMINAL_INIT_POS")
        {
            criminal_init_pos = Position(inf);
        }
    }

    // Close the file  
    file.close();
};
Configuration::~Configuration() 
{
    if (arr_walls!=nullptr) delete[] arr_walls;
    if (arr_fake_walls!=nullptr) delete[] arr_fake_walls;
};

string Configuration::str() const 
{
    string toReturn = "";
    toReturn +="Configuration[\n";
    toReturn +="MAP_NUM_ROWS="+to_string(map_num_rows)+"\n";
    toReturn +="MAP_NUM_COLS=" + to_string(map_num_cols)+ "\n";
    toReturn +="MAX_NUM_MOVING_OBJECTS="+ to_string(max_num_moving_objects) +"\n";
    toReturn += "NUM_WALLS=" + to_string(num_walls) + "\n";
    toReturn += "ARRAY_WALLS=" + positionArrStr(arr_walls, num_walls) + "\n";
    toReturn += "NUM_FAKE_WALLS="+to_string(num_fake_walls) + "\n";
    toReturn += "ARRAY_FAKE_WALLS=" + positionArrStr(arr_fake_walls, num_fake_walls) + "\n";
    toReturn +="SHERLOCK_MOVING_RULE="+sherlock_moving_rule +"\n";
    toReturn +="SHERLOCK_INIT_POS=" + sherlock_init_pos.str() +"\n";
    toReturn +="SHERLOCK_INIT_HP=" +to_string(sherlock_init_hp) +"\n";
    toReturn +="SHERLOCK_INIT_EXP=" +to_string(sherlock_init_exp) +"\n";
    toReturn += "WATSON_MOVING_RULE=" + watson_moving_rule +"\n";
    toReturn += "WATSON_INIT_POS=" +watson_init_pos.str() +"\n";
    toReturn += "WATSON_INIT_HP=" +to_string(watson_init_hp) +"\n";
    toReturn += "WATSON_INIT_EXP=" +to_string(watson_init_exp) +"\n";
    toReturn += "CRIMINAL_INIT_POS=" +criminal_init_pos.str() +"\n";
    toReturn += "NUM_STEPS="+ to_string(num_steps) +"\n";
    toReturn += "]";

    return toReturn;
};
string Configuration::getPreStr(string str)
{
    int equalPos = str.find("=");

    string pre = str.substr(0, equalPos);
    return pre;
};
string Configuration::getInfStr(string str)
{
    int equalPos = str.find("=");

    string inf = str.substr(equalPos+1, str.size()-(equalPos+1));
    return inf;
};
int Configuration::countDotComma(string str)
{
    if (str=="[]") return 0;
    int toReturn = 0;
    int size = str.size();
    for (int i = 0; i<size; i++)
    {
        if (str[i]==';') toReturn++;
    }
    return toReturn;
};
Position * Configuration::getPositionArray(string str) 
{
    if (str=="[]") return nullptr;
    Position * toReturn = new Position[countDotComma(str)+1];
    str = str.substr(1, str.size()-2);

    if (countDotComma(str)==0) 
    {
        toReturn[0] = Position(str);
        return toReturn;
    }
    else 
    {
        int temp = 0;
    
        // cout << str << endl;
        int dotCommaPos = str.find(';');
        // cout << dotCommaPos << endl;

        while (dotCommaPos!=-1 || dotCommaPos<str.size())
        {
            // cout << str.find(';') << endl;

            string pos_str = str.substr(0, dotCommaPos);
            toReturn[temp] = Position(pos_str);
            temp++;

            // cout << str.substr(dotCommaPos+1, str.size()-(dotCommaPos+1));
            str = str.substr(dotCommaPos+1, str.size()-(dotCommaPos+1));
            dotCommaPos = str.find(';');
            // cout << dotCommaPos << endl;
            if (dotCommaPos==-1) 
            {
                toReturn[temp] = Position(str);
                break;
            }
        }
    }
    return toReturn;
};
string Configuration::positionArrStr(Position * arr_pos, int size)
{
    if (arr_pos==nullptr) return "[]";

    string toReturn = "[";

    for (int i = 0; i<size; i ++)
    {
        toReturn+=arr_pos[i].str();
        if (i!=size-1) toReturn +=";";
    }

    toReturn += "]";
    return toReturn;
}
int Configuration::getSteps() {return num_steps;}
//
//
// Item
//
//
string strItemType(ItemType type)
{
    switch (type)
    {
    case 0:
        return "MAGIC_BOOK";
        break;
    case 1:
        return "ENERGY_DRINK";
        break;
    case 2:
        return "FIRST_AID";
        break;
    case 3:
        return "EXCEMPTION_CARD";
        break;
    case 4:
        return "PASSING_CARD";
        break;
    default:
        return "NULL ITEM TYPE";
        break;
    }
    return "NULL ELEMENT TYPE";
}

BaseItem::BaseItem(ItemType in_type) {type = in_type;};
BaseItem::~BaseItem() {};

ItemType BaseItem::getType() {return type;};
bool BaseItem::canUse(Character* obj, Robot * robot) {return true;};
bool BaseItem::canUse_St(Character* obj, Robot * robot) {return true;};
void BaseItem::use(Character* obj, Robot * robot) {return;};

// Magic Book
MagicBook::MagicBook() : BaseItem(MAGIC_BOOK) {};
MagicBook::~MagicBook() {};

bool MagicBook::canUse_St(Character* obj, Robot * robot)
{
    if (obj->getName()=="Sherlock")
    {
        Sherlock * s = (Sherlock *) obj;
        return (s->getExp()<=350 && s->getStage()==2);
    }
    else if (obj->getName()=="Watson")
    {
        Watson * w = (Watson *) obj;
        return (w->getExp()<=350 && w->getStage()==2);
    }
    else return false;
};
bool MagicBook::canUse(Character* obj, Robot * robot)
{
    if (obj->getName()=="Sherlock")
    {
        Sherlock * s = (Sherlock *) obj;
        return (s->getExp()<=350);
    }
    else if (obj->getName()=="Watson")
    {
        Watson * w = (Watson *) obj;
        return (w->getExp()<=350);
    }
    else return false;
};

void MagicBook::use(Character* obj, Robot * robot)
{
    if (obj->getName()=="Sherlock")
    {
        Sherlock * s = (Sherlock *) obj;
        s->setExp(s->getExp()+ ceilFixed(s->getExp()*0.25));
        s->setStage(0);
        // cout << "Sherlock used Magic Book" << endl;
        return;
    }
    else if (obj->getName()=="Watson")
    {
        Watson * w = (Watson *) obj;
        w->setExp(w->getExp()+ ceilFixed(w->getExp()*0.25));
        // cout << "Watson used Magic Book" << endl;
        w->setStage(0);
        return;
    }
    else return;
};

//Energy Drink
EnergyDrink::EnergyDrink() :BaseItem(ENERGY_DRINK) {};
EnergyDrink::~EnergyDrink() {};

bool EnergyDrink::canUse_St(Character* obj, Robot * robot)
{
    if (obj->getName()=="Sherlock")
    {
        Sherlock * s = (Sherlock *) obj;
        return (s->getHp()<=100 && s->getStage()==2);
    }
    else if (obj->getName()=="Watson")
    {
        Watson * w = (Watson *) obj;
        return (w->getHp()<=100 && w->getStage()==2);
    }
    else return false;
};
bool EnergyDrink::canUse(Character* obj, Robot * robot)
{
    if (obj->getName()=="Sherlock")
    {
        Sherlock * s = (Sherlock *) obj;
        return (s->getHp()<=100);
    }
    else if (obj->getName()=="Watson")
    {
        Watson * w = (Watson *) obj;
        return (w->getHp()<=100);
    }
    else return false;
};
void EnergyDrink::use(Character* obj, Robot * robot)
{
    if (obj->getName()=="Sherlock")
    {
        Sherlock * s = (Sherlock *) obj;
        s->setHp(s->getHp()+ ceilFixed(s->getHp()*0.2));
        s->setStage(0);
        // cout << "Sherlock used Energy Drink" << endl;
        return;
    }
    else if (obj->getName()=="Watson")
    {
        Watson * w = (Watson *) obj;
        w->setHp(w->getHp()+ ceilFixed(w->getHp()*0.2));
        w->setStage(0);
        // cout << "Watson used Energy Drink" << endl;
        return;
    }
    else return;
};

//First Aid
FirstAid::FirstAid() :BaseItem(FIRST_AID) {};
FirstAid::~FirstAid() {};

bool FirstAid::canUse_St(Character* obj, Robot * robot)
{
    if (obj->getName()=="Sherlock")
    {
        Sherlock * s = (Sherlock *) obj;
        return ((s->getExp()<=350) || (s->getHp()<=100)) && s->getStage()==2;
    }
    else if (obj->getName()=="Watson")
    {
        Watson * w = (Watson *) obj;
        return ((w->getExp()<=350) || (w->getHp()<=100)) && w->getStage()==2;
    }
    else return false;
}
bool FirstAid::canUse(Character* obj, Robot * robot)
{
    if (obj->getName()=="Sherlock")
    {
        Sherlock * s = (Sherlock *) obj;
        return ((s->getExp()<=350) || (s->getHp()<=100));
    }
    else if (obj->getName()=="Watson")
    {
        Watson * w = (Watson *) obj;
        return ((w->getExp()<=350) || (w->getHp()<=100));
    }
    else return false;
}
void FirstAid::use(Character* obj, Robot * robot)
{
    if (obj->getName()=="Sherlock")
    {
        Sherlock * s = (Sherlock *) obj;
        s->setHp(s->getHp()+ ceilFixed(s->getHp()*0.5));
        // cout << "Sherlock used First Aid" << endl;
        s->setStage(0);
        return;
    }
    else if (obj->getName()=="Watson")
    {
        Watson * w = (Watson *) obj;
        w->setHp(w->getHp()+ ceilFixed(w->getHp()*0.5));
        // cout << "Watson used First Aid" << endl;
        w->setStage(0);
        return;
    }
    else return;
}

// Excemption Card
ExcemptionCard::ExcemptionCard() : BaseItem(EXCEMPTION_CARD) {};
ExcemptionCard::~ExcemptionCard() {};

bool ExcemptionCard::canUse_St(Character *obj, Robot * robot)
{
    if (obj->getName()=="Sherlock")
    {
        Sherlock * s = (Sherlock *) obj;
        return (s->getHp()%2!=0) && s->getStage()==1;
    }
    else return false;
}
bool ExcemptionCard::canUse(Character *obj, Robot * robot)
{
    if (obj->getName()=="Sherlock")
    {
        Sherlock * s = (Sherlock *) obj;
        return (s->getHp()%2!=0);
    }
    else return false;
}
void ExcemptionCard::use(Character *obj, Robot * robot) 
{
    Sherlock * s = (Sherlock *) obj;
    s->setExcemp(true);
}

// Passing 
PassingCard::PassingCard(int r, int c) : BaseItem(PASSING_CARD) 
{
    int t = (r*11+c)%4;
    switch (t)
    {
    case 0:
        challenge = "RobotS";
        break;
    case 1:
        challenge = "RobotC";
        break;
    case 2:
        challenge = "RobotSW";
        break;
    case 3:
        challenge = "all";
        break;        
    default:
        break;
    }
};
PassingCard::PassingCard(int t) : BaseItem(PASSING_CARD) 
{
    switch (t)
    {
    case 0:
        challenge = "RobotS";
        break;
    case 1:
        challenge = "RobotC";
        break;
    case 2:
        challenge = "RobotSW";
        break;
    case 3:
        challenge = "all";
        break;        
    default:
        break;
    }
};
PassingCard::PassingCard(string in_str) : BaseItem(PASSING_CARD) 
{
    challenge = in_str;
};
PassingCard::~PassingCard() {};

string PassingCard::getChallenge() {return challenge;}
bool PassingCard::canUse_St(Character *obj, Robot * robot)
{
    if (obj->getName()=="Watson")
    {
        Watson * w = (Watson *) obj;
        return (w->getHp()%2==0) && w->getStage()==1;
    }
    else return false;
}
bool PassingCard::canUse(Character *obj, Robot * robot)
{
    if (obj->getName()=="Watson")
    {
        Watson * w = (Watson *) obj;
        return (w->getHp()%2==0);
    }
    else return false;
}
void PassingCard::use(Character *obj, Robot * robot)
{
    Watson * w = (Watson *) obj;

    if (challenge!="all" && challenge!=robot->getName())
    {
        w->setExp(w->getExp()-50);
    }

    w->setPass(true);
}

//
//
// Inventory (Bag)
//
//
const int sherlockBagSize = 13;
const int watsonBagSize = 15;
ItemNode::ItemNode(BaseItem * in_item, ItemNode* next){
    item = in_item;
    link = next;
};
ItemNode::~ItemNode() {};

//BaseBag
BaseBag::BaseBag(int in_size, Character * in_obj)
{
    size = in_size;
    current = 0;
    obj=in_obj;
    head = NULL;
};
BaseBag::~BaseBag() 
{
    if (head!=NULL) 
    {
        while (head!=NULL)
        {
            // cout << "YO" << endl;
            ItemNode* temp = head;
            head = head->link;
            delete temp;
        }
        delete head;
    }
};

bool BaseBag::insert(BaseItem* item) {return false;}; // virtual
BaseItem * BaseBag::get() {return NULL;}; // virtual
BaseItem * BaseBag::get_St() {return NULL;} // virtual
BaseItem * BaseBag::get(ItemType) {return NULL;}; // virtual
BaseItem * BaseBag::get_NC(ItemType) {return NULL;}; // virtual
string BaseBag::str() const {return "";}; // virtual

bool BaseBag::isFull() const {
    return current>=size;
}
bool BaseBag::add(BaseItem * item)
{
    if (current>=size) return false;
    else
    {
        if (current == 0) head->item=item;
        else {
            ItemNode * created = new ItemNode(item, NULL);
            ItemNode * last;
            for (int i = 0; i<current; i++)
            {
                if (i==0) last = head;
                else last = last->link;
            }
            last->link =created;    
        }
        current++;
        return true;
    }
};
bool BaseBag::insert(int index, BaseItem * item) {
    if (index>current-1 || index<0) return false;
    else
    {
        if (index==0) 
        {
            if (current == 0) head->item=item;
            else 
            {
                ItemNode * created = new ItemNode(item, head);
                head = created;
            }
        }
        else
        {
            ItemNode * last = head;
            for (int i = 0; i<index-1; i++)
            {
            last = last->link;
            }

            ItemNode * created = new ItemNode(item, last->link);
            last->link = created;
        }
        
        current++;
        return true;
    }
};
void BaseBag::printChain() {
    ItemNode * last = head;
    for (int i = 0; i<current; i++)
    {
        if (i!=current-1) cout << strItemType(last->item->getType()) << ",";
        else cout << strItemType(last->item->getType());
        last = last->link;
    }
};
bool BaseBag::pop_front()
{
    if (current == 0) return false;
    else
    {
        ItemNode* nextHead = head->link;
        head->link = NULL;
        ItemNode* lastHead = head;
        head = nextHead;
        delete lastHead;
        
        current--;
    }

    return true;
}
bool BaseBag::swap(int a, int b)
{
    if (a==b) return false;

    if (a>=current || a<0 || b>=current || b<0) return false;

    int big = max(a,b);
    int small = min(a,b);
    ItemNode * last;
    ItemNode * lastSmall;
    ItemNode * lastBig;
    ItemNode * smallNode;
    ItemNode * bigNode;
    ItemNode * temp;

    // initialize variable
    for (int i = 0; i<current; i++)
    {
        if (i==0) last = head;
        else {last = last->link;}

        if (i==big-1) lastBig = last;
        if (i==small-1) lastSmall = last;
        if (i==big) bigNode = last;
        if (i==small) smallNode = last;
    }

    //start swapping
    if (small == 0) head = bigNode;
    else lastSmall->link = bigNode;
    lastBig->link = smallNode;

    temp = smallNode->link;
    smallNode->link = bigNode->link;
    bigNode->link = temp;
    return true;
}
// bool BaseBag::remove(int index)
// {
//     if (index==0) return pop_front();
//     if (index<0 || index>=current) return false;

//     ItemNode * last;
//     for (int i = 0 ; i<index; i++)
//     {
//         // cout << i << endl;
//         if (i==0) last = head;
//         else
//         {
//             last = last->link;
//         }
//     }
//     ItemNode * toRemove = last->link;
//     last->link = toRemove->link;
//     toRemove->link=NULL;
    
//     delete toRemove;
//     current--;

//     return true;
// }
// int BaseBag::getFirst(ItemType type)
// {
//     ItemNode * last;
//     for (int i = 0 ; i<current; i++)
//     {
//         // cout << i << endl;
//         if (i==0) last = head;
//         else
//         {
//             last = last->link;
//         }
//         if (last!=NULL && last->item->getType()==type) return i;
//     }
//     return -1;
// }
// BaseItem * BaseBag::get_NS(ItemType type) 
// {
//     int pos = getFirst(type); 
//     if (pos==-1) return NULL;

//     ItemNode * last;
//     for (int i = 0 ; i<=pos; i++)
//     {
//         if (i==0) last = head;
//         else
//         {
//             last = last->link;
//         }
//     }

//     BaseItem * toReturn;
//     if (last->item->getType()==MAGIC_BOOK) toReturn = new MagicBook();
//     else if (last->item->getType()==ENERGY_DRINK) toReturn = new EnergyDrink();
//     else if (last->item->getType()==FIRST_AID) toReturn = new FirstAid();
//     else if (last->item->getType()==EXCEMPTION_CARD) toReturn = new ExcemptionCard();
//     else if (last->item->getType()==PASSING_CARD) 
//     {
//         PassingCard * passingCard = (PassingCard*) (last->item);
//         toReturn = new PassingCard(passingCard->getChallenge());
//     }
//     remove(pos);

//     return toReturn;
// };

//Sherlock Bag
SherlockBag::SherlockBag(Sherlock * in_sherlock) : BaseBag(sherlockBagSize, in_sherlock) {};
SherlockBag::~SherlockBag() {};

bool SherlockBag::insert(BaseItem* item) {
    if (current>=size) return false;
    else
    {
        if (current == 0) 
        {
            if (head==NULL) head = new ItemNode(item,NULL);
            else head->item=item;
        }
        else 
        {
            ItemNode * created = new ItemNode(item, head);
            head = created;
        }
        current++;
        return true;
    }
    return false;
};
BaseItem * SherlockBag::get() 
{
    ItemNode * last;
    for (int i = 0 ; i<current; i++)
    {
        // cout << i << endl;
        if (i==0) last = head;
        else
        {
            last = last->link;
        }
        if (last->item->canUse(obj, NULL)) 
        {   
            BaseItem * toReturn;
            if (last->item->getType()==MAGIC_BOOK) toReturn = new MagicBook();
            else if (last->item->getType()==ENERGY_DRINK) toReturn = new EnergyDrink();
            else if (last->item->getType()==FIRST_AID) toReturn = new FirstAid();
            else if (last->item->getType()==EXCEMPTION_CARD) toReturn = new ExcemptionCard();
            else if (last->item->getType()==PASSING_CARD) 
            {
                PassingCard * passingCard = (PassingCard*) (last->item);
                toReturn = new PassingCard(passingCard->getChallenge());
            }
            swap(0,i);
            pop_front();
            return toReturn;
        }
    }
    return NULL;
};

BaseItem * SherlockBag::get_St() 
{
    ItemNode * last;
    for (int i = 0 ; i<current; i++)
    {
        // cout << i << endl;
        if (i==0) last = head;
        else
        {
            last = last->link;
        }
        if (last->item->canUse_St(obj, NULL)) 
        {   
            BaseItem * toReturn;
            if (last->item->getType()==MAGIC_BOOK) toReturn = new MagicBook();
            else if (last->item->getType()==ENERGY_DRINK) toReturn = new EnergyDrink();
            else if (last->item->getType()==FIRST_AID) toReturn = new FirstAid();
            else if (last->item->getType()==EXCEMPTION_CARD) toReturn = new ExcemptionCard();
            else if (last->item->getType()==PASSING_CARD) 
            {
                PassingCard * passingCard = (PassingCard*) (last->item);
                toReturn = new PassingCard(passingCard->getChallenge());
            }
            swap(0,i);
            pop_front();
            return toReturn;
        }
    }
    return NULL;
};

BaseItem * SherlockBag::get(ItemType type) 
{
    ItemNode * last;
    for (int i = 0 ; i<current; i++)
    {
        if (i==0) last = head;
        else
        {
            last = last->link;
        }
        // cout << type << endl;
        // cout << (last->item->getType()==type) << endl;
        if (last->item->getType()==type && last->item->canUse(obj, NULL)) 
        {   
            BaseItem * toReturn;
            if (last->item->getType()==MAGIC_BOOK) toReturn = new MagicBook();
            else if (last->item->getType()==ENERGY_DRINK) toReturn = new EnergyDrink();
            else if (last->item->getType()==FIRST_AID) toReturn = new FirstAid();
            else if (last->item->getType()==EXCEMPTION_CARD) toReturn = new ExcemptionCard();
            else if (last->item->getType()==PASSING_CARD) 
            {
                PassingCard * passingCard = (PassingCard*) (last->item);
                toReturn = new PassingCard(passingCard->getChallenge());
            }
            swap(0,i);
            pop_front();
            return toReturn;
        }
    }
    return NULL;
};
BaseItem * SherlockBag::get_NC(ItemType type) 
{
    ItemNode * last;
    for (int i = 0 ; i<current; i++)
    {
        if (i==0) last = head;
        else
        {
            last = last->link;
        }
        // cout << type << endl;
        // cout << (last->item->getType()==type) << endl;
        if (last->item->getType()==type) 
        {   
            BaseItem * toReturn;
            if (last->item->getType()==MAGIC_BOOK) toReturn = new MagicBook();
            else if (last->item->getType()==ENERGY_DRINK) toReturn = new EnergyDrink();
            else if (last->item->getType()==FIRST_AID) toReturn = new FirstAid();
            else if (last->item->getType()==EXCEMPTION_CARD) toReturn = new ExcemptionCard();
            else if (last->item->getType()==PASSING_CARD) 
            {
                PassingCard * passingCard = (PassingCard*) (last->item);
                toReturn = new PassingCard(passingCard->getChallenge());
            }
            swap(0,i);
            pop_front();
            return toReturn;
        }
    }
    return NULL;
};
string SherlockBag::str() const {
    string toReturn = "Bag[count="+to_string(current)+";";

    if (head == NULL || head->item == NULL) 
    {
        toReturn += "]";
        return toReturn;
    }

    ItemNode * last = head;
    for (int i = 0; i<current; i++)
    {
        if (i!=current-1) toReturn+= strItemType(last->item->getType()) + ",";
        else toReturn+= strItemType(last->item->getType())+ "]";
        last = last->link;
    }

    return toReturn;
};

//Watson Bag
WatsonBag::WatsonBag(Watson * in_watson) : BaseBag(watsonBagSize, in_watson) {};
WatsonBag::~WatsonBag() {};

bool WatsonBag::insert(BaseItem* item) {
    if (current>=size) return false;
    else
    {
        if (current == 0) 
        {
            if (head==NULL) head = new ItemNode(item,NULL);
            else head->item=item;
        }
        else {
            ItemNode * created = new ItemNode(item, head);
            head = created;
        }
        current++;
        return true;
    }
    return false;
};
BaseItem * WatsonBag::get()
{
    ItemNode * last;
    for (int i = 0 ; i<current; i++)
    {
        // cout << i << endl;
        if (i==0) last = head;
        else
        {
            last = last->link;
        }
        if (last->item->canUse(obj, NULL)) 
        {   
            
            BaseItem * toReturn;
            if (last->item->getType()==MAGIC_BOOK) toReturn = new MagicBook();
            else if (last->item->getType()==ENERGY_DRINK) toReturn = new EnergyDrink();
            else if (last->item->getType()==FIRST_AID) toReturn = new FirstAid();
            else if (last->item->getType()==EXCEMPTION_CARD) toReturn = new ExcemptionCard();
            else if (last->item->getType()==PASSING_CARD) 
            {
                PassingCard * passingCard = (PassingCard*) (last->item);
                toReturn = new PassingCard(passingCard->getChallenge());
            }
            swap(0,i);
            pop_front();
            return toReturn;
        }
    }
    return NULL;
};
BaseItem * WatsonBag::get_St()
{
    ItemNode * last;
    for (int i = 0 ; i<current; i++)
    {
        // cout << i << endl;
        if (i==0) last = head;
        else
        {
            last = last->link;
        }
        if (last->item->canUse_St(obj, NULL)) 
        {   
            
            BaseItem * toReturn;
            if (last->item->getType()==MAGIC_BOOK) toReturn = new MagicBook();
            else if (last->item->getType()==ENERGY_DRINK) toReturn = new EnergyDrink();
            else if (last->item->getType()==FIRST_AID) toReturn = new FirstAid();
            else if (last->item->getType()==EXCEMPTION_CARD) toReturn = new ExcemptionCard();
            else if (last->item->getType()==PASSING_CARD) 
            {
                PassingCard * passingCard = (PassingCard*) (last->item);
                toReturn = new PassingCard(passingCard->getChallenge());
            }
            swap(0,i);
            pop_front();
            return toReturn;
        }
    }
    return NULL;
};

BaseItem * WatsonBag::get(ItemType type) {
    ItemNode * last;
    for (int i = 0 ; i<current; i++)
    {
        if (i==0) last = head;
        else
        {
            last = last->link;
        }
        // cout << type << endl;
        // cout << (last->item->getType()==type) << endl;
        if (last->item->getType()==type && last->item->canUse(obj, NULL)) 
        {   
            BaseItem * toReturn;
            if (last->item->getType()==MAGIC_BOOK) toReturn = new MagicBook();
            else if (last->item->getType()==ENERGY_DRINK) toReturn = new EnergyDrink();
            else if (last->item->getType()==FIRST_AID) toReturn = new FirstAid();
            else if (last->item->getType()==EXCEMPTION_CARD) toReturn = new ExcemptionCard();
            else if (last->item->getType()==PASSING_CARD) 
            {
                PassingCard * passingCard = (PassingCard*) (last->item);
                toReturn = new PassingCard(passingCard->getChallenge());
            }
            swap(0,i);
            pop_front();
            return toReturn;
        }
    }
    return NULL;
};
BaseItem * WatsonBag::get_NC(ItemType type) 
{
    ItemNode * last;
    for (int i = 0 ; i<current; i++)
    {
        if (i==0) last = head;
        else
        {
            last = last->link;
        }
        // cout << type << endl;
        // cout << (last->item->getType()==type) << endl;
        if (last->item->getType()==type) 
        {   
            BaseItem * toReturn;
            if (last->item->getType()==MAGIC_BOOK) toReturn = new MagicBook();
            else if (last->item->getType()==ENERGY_DRINK) toReturn = new EnergyDrink();
            else if (last->item->getType()==FIRST_AID) toReturn = new FirstAid();
            else if (last->item->getType()==EXCEMPTION_CARD) toReturn = new ExcemptionCard();
            else if (last->item->getType()==PASSING_CARD) 
            {
                PassingCard * passingCard = (PassingCard*) (last->item);
                toReturn = new PassingCard(passingCard->getChallenge());
            }
            swap(0,i);
            pop_front();
            return toReturn;
        }
    }
    return NULL;
};
string WatsonBag::str() const 
{
    string toReturn = "Bag[count="+to_string(current)+";";
    if (head == NULL || head->item == NULL) 
    {
        toReturn += "]";
        return toReturn;
    }

    ItemNode * last = head;
    for (int i = 0; i<current; i++)
    {
        if (i!=current-1) toReturn+= strItemType(last->item->getType()) + ",";
        else toReturn+= strItemType(last->item->getType())+ "]";
        last = last->link;
    }

    return toReturn;
};

//
//
// Program
//
//
StudyPinkProgram::StudyPinkProgram(const string & config_file_path) 
{
    config = new Configuration(config_file_path);
    // cout << config->str() << endl;
    map = new Map(config->map_num_rows, config->map_num_cols
                , config->num_walls, config->arr_walls
                , config->num_fake_walls, config->arr_fake_walls);
    sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos
                    , map, config->sherlock_init_hp, config->sherlock_init_exp);
    watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos
                    , map, config->watson_init_hp, config->watson_init_exp);
    criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);

    arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    for (int i = 0; i<3; i++)
    {
        if (config->max_num_moving_objects>=1 && i==0) arr_mv_objs->add(criminal);
        if (config->max_num_moving_objects>=2 && i==1) arr_mv_objs->add(sherlock);
        if (config->max_num_moving_objects>=3 && i==2) arr_mv_objs->add(watson);
    }
    sherlockBag = new SherlockBag(sherlock);
    watsonBag = new WatsonBag(watson);
};
StudyPinkProgram::~StudyPinkProgram() 
{
    if (arr_mv_objs!=NULL) 
    {
        // cout << "arrmvobj" << endl;
        delete arr_mv_objs;
    }
    if (map!=NULL) 
    {
        // cout << "map" << endl;
        delete map;
    }

    if (sherlock!=NULL) 
    {
        // cout << "sher" << endl;
        delete sherlock;
    }
    
    if (watson!=NULL) 
    {
        // cout << "wat" << endl;
        delete watson;
    }
    // else cout << "not found wat" << endl;

    if (criminal!=NULL) 
    {
        // cout << "crim" << endl;
        delete criminal; 
    } 
    // else cout << "not found wat" << endl;

    if (watsonBag!=NULL) 
    {
        // cout << "wBag" << endl;
        delete watsonBag;
    } 
    // else cout << "not found wat" << endl;

    if (sherlockBag!=NULL)
    {
        // cout << "sBag" << endl;
        delete sherlockBag;
        // cout << "Errre?" << endl;
    }

    if (config!=NULL)
    {
        // cout << "conf" << endl;
        delete config;
    } 
};
bool StudyPinkProgram::isStop() const 
{
    if (sherlock->getHp()==1) return true;
    if (sherlock->getExp()==1) return true;
    if (watson->getHp()==1) return true;
    if (watson->getExp()==1) return true;
    if (sherlock->meet(criminal)) return true;
    if (watson->meet(criminal)) return true;
    return false;
};
void StudyPinkProgram::printResult() const
{
    if (sherlock->meet(criminal)) 
    {
        cout << "Sherlock caught the criminal" << endl;
    }
    else if (watson->meet(criminal)) 
    {
        cout << "Watson caught the criminal" << endl;
    }
    else 
    {
        cout << "The criminal escaped" << endl;
    }
}
void StudyPinkProgram::printStep(int si) const 
{
    bool isTest = false;
    if (isTest)
    {
        int width_step = 1;
        int tempStep = config->getSteps();
        while(tempStep/10!=0)
        {
            tempStep=tempStep/10;
            width_step++;
        }
        int width_index = 1;
        int tempIndex = config->max_num_moving_objects;
        while (tempIndex/10!=0)
        {
            tempIndex=tempIndex/10;
            width_index++;
        }
        cout << "* Step " << setw(width_step) << setfill('0') << si << endl;
        for (int i = 0; i<arr_mv_objs->count; i++)
        {   
            cout <<"> > Index " << setw(width_index) << setfill('0') <<i << " --|-- " << arr_mv_objs->get(i)->str() << " --|-- "  << endl;
        }      
        cout << endl;    
    }
    else
    {
        cout << "Step: " << setw(4) << setfill('0') << si
        << "--"
        << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }
    

    // school test case
    
}
void StudyPinkProgram::run(bool verbose) /* undone */ 
{
    bool isTest = false;
    int lastSpawn = 0;

    for (int istep = 0; istep < config->getSteps(); istep++) 
    {
        if (isStop()) 
        {
            if (isTest) cout << "Program Stopped" << endl;
            printStep(istep);
            break;
        }

        // Move
        for (int i = 0; i < arr_mv_objs->size(); ++i) 
        {
            arr_mv_objs->get(i)->move();
            
            if (arr_mv_objs->get(i)->getName()=="Criminal")
            {
                CriminalEvents(isTest);
                if (isStop()) 
                {
                    break;
                }
                spawnRobot(criminal->getStep(), lastSpawn, isTest);
            }
            else if (arr_mv_objs->get(i)->getName()=="Sherlock")
            {
                SherlockEvents(isTest);
                if (isStop()) 
                {
                    break;
                }
            }
            else if (arr_mv_objs->get(i)->getName()=="Watson")
            {
                WatsonEvents(isTest);
                if (isStop()) 
                {
                    break;
                }
            }
            else 
            {
                RobotEvents(arr_mv_objs->get(i), isTest);
                if (isStop()) 
                {
                    break;
                }
            }
        }

        if (isStop()) 
        {
            if (isTest) cout << "Program Stopped" << endl;
            printStep(istep);
            break;
        }

        if (verbose) 
        {
            printStep(istep);
        }
    }
    printResult();
}
void StudyPinkProgram::showAll()
{
    cout << config->str() << endl;
    cout << sherlock->str() << endl;
    cout << watson->str() << endl;
    cout << criminal->str() << endl;
    cout << map->str() << endl;
    cout << arr_mv_objs->str() << endl;
    cout << sherlockBag->str() << endl;
    cout << watsonBag->str() << endl;
}
void StudyPinkProgram::spawnRobot(int cStep, int & lastSpawn, bool isTest)
{
    if (cStep%3==0)
    {   
        if (cStep!=lastSpawn)
        {
            if (cStep==3) // first Robot Spawn
            {  
                RobotC * robotC = new RobotC(arr_mv_objs->count, criminal->getLastPosition(), map, criminal);
                if (arr_mv_objs->add(robotC) && isTest) cout << "Robot C spawned (" << robotC->getItemStr() << ")" << endl;
            }
            else
            {
                int sDis = Position::ManhattanDistance(sherlock->getCurrentPosition(), criminal->getCurrentPosition());
                int wDis = Position::ManhattanDistance(watson->getCurrentPosition(), criminal->getCurrentPosition());
                if (sDis<wDis) 
                {
                    RobotS * robotS = new RobotS(arr_mv_objs->count, criminal->getLastPosition(), map, criminal, sherlock);
                    if (arr_mv_objs->add(robotS) && isTest) cout << "Robot S spawned (" << robotS->getItemStr() << ")" << endl;
                }
                else if (sDis>wDis)
                {
                    RobotW * robotW = new RobotW(arr_mv_objs->count, criminal->getLastPosition(), map, criminal, watson);
                    if (arr_mv_objs->add(robotW) && isTest) cout << "Robot W spawned (" << robotW->getItemStr() << ")" << endl;
                }
                else if (sDis==wDis)
                {
                    RobotSW * robotSW = new RobotSW(arr_mv_objs->count, criminal->getLastPosition(), map, criminal, sherlock, watson);
                    if (arr_mv_objs->add(robotSW) && isTest) cout << "Robot SW spawned (" << robotSW->getItemStr() << ")" << endl;
                }
            }
            lastSpawn = cStep;
        }
    }
}

void StudyPinkProgram::SherlockEvents(bool verbose)
{
    bool isTest = verbose;
    for (int i = 0; i < arr_mv_objs->size(); ++i) 
    {
        MovingObject * obj = arr_mv_objs->get(i);
        
        if (sherlock->meet(obj))
        {
            if (obj->getName()=="Watson")
            {
                tradeCard(sherlock, sherlockBag, watson, watsonBag, isTest);
            }
            else if (obj->getName()=="Criminal")
            {
                if (isTest) cout << "Sherlock: I got you! | Criminal Caught" << endl;
                return;
            } else if (!obj->isCharacter())
            {       
                // Find Excemption Card
                if (isTest) 
                {
                    cout << "--------------------------------------" << endl;
                    cout << "Sherlock meet Robot | Finding Excemption Card..." << endl;
                    cout << sherlockBag->str() << endl;
                }

                // change stage
                sherlock->setStage(1);

                // find excemption card
                BaseItem * card = sherlockBag->get_St();
                if (card!=NULL) 
                {
                    card->use(sherlock, NULL);
                    if (isTest) cout << "Sherlock: Excemption Card used!" << endl;
                } 

                // Challenge
                if (obj->getName()=="RobotC")
                {
                    RobotC * robotC = (RobotC *) obj;
                    C_challenge_S(sherlock, sherlockBag, criminal, robotC, isTest);
                }
                else if (obj->getName()=="RobotS")
                {
                    RobotS * robotS = (RobotS *) obj;
                    S_challenge_S(sherlock, sherlockBag, robotS, isTest);

                } else if (obj->getName()=="RobotW")
                {
                    RobotW * robotW = (RobotW *) obj;
                    W_challenge_S(sherlockBag, robotW, isTest);
                } else if (obj->getName()=="RobotSW")
                {
                    RobotSW * robotSW = (RobotSW *) obj;
                    SW_challenge_S(sherlock, sherlockBag, robotSW, isTest);
                }

                // change stage
                sherlock->setExcemp(false);
                sherlock->setStage(2);
            
                // Healing
                S_heal(sherlock, sherlockBag, isTest);

                // change stage 
                sherlock->setStage(0);

                if (isStop())
                {
                    return;
                }
            } 
        }
    }
    return;
}

void StudyPinkProgram::WatsonEvents(bool verbose)
{
    bool isTest = verbose;
    for (int i = 0; i < arr_mv_objs->size(); ++i) 
    {
        MovingObject * obj = arr_mv_objs->get(i);
        
        if (watson->meet(obj))
        {
            if (obj->getName()=="Sherlock")
            {
                tradeCard(sherlock, sherlockBag, watson, watsonBag, isTest);
            }
            else if (obj->getName()=="Criminal")
            {
                if (isTest) cout << "Watson: I got you! | Criminal Caught" << endl;
                return;
            } else if (!obj->isCharacter())
            {       
                if (isTest) 
                {
                    cout << "--------------------------------------" << endl;
                    cout << "Watson meet Robot | Finding Passing Card..." << endl;
                    cout << watsonBag->str() << endl;
                    cout << "HP:" << watson->getHp() << ", EXP:" << watson->getExp() << endl;  
                }

                // change stage
                watson->setStage(1);

                Robot * robot = (Robot *) obj;
                // Find Passing Card
                BaseItem * card = watsonBag->get_St();
                if (card!=NULL) card->use(watson, robot);

                if (watson->canPass())
                {
                    watsonBag->insert(robot->getItem());
                    watson->setPass(false);
                    watson->setStage(0);
                    if (isTest)
                    {
                        cout << "Passing card used! Gains "<< robot->getItemStr()  << endl;
                        cout << watsonBag->str() << endl;
                        cout << "EXP:" << watson->getExp() << endl;
                        cout << "--------------------------------------" << endl;
                    }
                }
                else
                {
                    // Challenge
                    if (obj->getName()=="RobotC")
                    {
                        RobotC * robotC = (RobotC *) obj;
                        C_challenge_W(watson, watsonBag, robotC, isTest);
                    }
                    else if (obj->getName()=="RobotS")
                    {
                        RobotS * robotS = (RobotS *) obj;
                        S_challenge_W(isTest);
                    } else if (obj->getName()=="RobotW")
                    {
                        RobotW * robotW = (RobotW *) obj;
                        W_challenge_W(watson, watsonBag, robotW, isTest);
                    } else if (obj->getName()=="RobotSW")
                    {
                        RobotSW * robotSW = (RobotSW *) obj;
                        SW_challenge_W(watson, watsonBag, robotSW, isTest);
                    }
                }

                // change stage
                watson->setStage(2);

                // Healing
                W_heal(watson, watsonBag, isTest);

                // change stage(
                watson->setStage(0);

                if (isStop())
                {
                    return;
                }
            } 
        }
    }
    return;
}

void StudyPinkProgram::CriminalEvents(bool verbose)
{
    bool isTest = verbose;
    for (int i = 0; i < arr_mv_objs->size(); ++i) 
    {
        MovingObject * obj = arr_mv_objs->get(i);
        
        if (criminal->meet(obj))
        {
            if (obj->getName()=="Sherlock")
            {
                if (isTest) cout << "Criminal caught!" << endl;
                return;
            }
            else if (obj->getName()=="Watson")
            {
                if (isTest) cout << "Criminal caught!" << endl;
                return;
            } 
        }
    }
    return;
}

void StudyPinkProgram::RobotEvents(MovingObject* robot, bool verbose)
{
    bool isTest = verbose;
    for (int i = 0; i < arr_mv_objs->size(); ++i) 
    {
        MovingObject * obj = arr_mv_objs->get(i);
        
        if (robot->meet(obj))
        {
            if (obj->getName()=="Watson")
            {
                if (isTest) 
                {
                    cout << "--------------------------------------" << endl;
                    cout << "Watson meet Robot | Finding Passing Card..." << endl;
                    cout << watsonBag->str() << endl;
                    cout << "HP:" << watson->getHp() << ", EXP:" << watson->getExp() << endl;  
                }

                // change stage
                watson->setStage(1);

                Robot * r = (Robot *) robot;
                // Find Passing Card
                BaseItem * card = watsonBag->get_St();
                if (card!=NULL) card->use(watson, r);

                if (watson->canPass())
                {
                    watsonBag->insert(r->getItem());
                    watson->setPass(false);
                    watson->setStage(0);
                    if (isTest)
                    {
                        cout << "Passing card used! Gains "<< r->getItemStr()  << endl;
                        cout << watsonBag->str() << endl;
                        cout << "EXP:" << watson->getExp() << endl;
                        cout << "--------------------------------------" << endl;
                    }
                }
                else
                {
                    // Challenge
                    if (robot->getName()=="RobotC")
                    {
                        RobotC * robotC = (RobotC *) robot;
                        C_challenge_W(watson, watsonBag, robotC, isTest);
                    }
                    else if (robot->getName()=="RobotS")
                    {
                        RobotS * robotS = (RobotS *) robot;
                        S_challenge_W(isTest);
                    } else if (robot->getName()=="RobotW")
                    {
                        RobotW * robotW = (RobotW *) robot;
                        W_challenge_W(watson, watsonBag, robotW, isTest);
                    } else if (robot->getName()=="RobotSW")
                    {
                        RobotSW * robotSW = (RobotSW *) robot;
                        SW_challenge_W(watson, watsonBag, robotSW, isTest);
                    }
                }

                // change stage
                watson->setStage(2);

                // Healing
                W_heal(watson, watsonBag, isTest);

                // change stage(
                watson->setStage(0);
                if (isStop())
                {
                    break;
                }
            } else if (obj->getName()=="Sherlock")
            {
                // Find Excemption Card
                if (isTest) 
                {
                    cout << "--------------------------------------" << endl;
                    cout << "Sherlock meet Robot | Finding Excemption Card..." << endl;
                    cout << sherlockBag->str() << endl;
                }

                // change stage
                sherlock->setStage(1);

                // find excemption card
                BaseItem * card = sherlockBag->get_St();
                if (card!=NULL) 
                {
                    card->use(sherlock, NULL);
                    if (isTest) cout << "Sherlock: Excemption Card used!" << endl;
                } 

                // Challenge
                if (robot->getName()=="RobotC")
                {
                    RobotC * robotC = (RobotC *) robot;
                    C_challenge_S(sherlock, sherlockBag, criminal, robotC, isTest);
                }
                else if (robot->getName()=="RobotS")
                {
                    RobotS * robotS = (RobotS *) robot;
                    S_challenge_S(sherlock, sherlockBag, robotS, isTest);

                } else if (robot->getName()=="RobotW")
                {
                    RobotW * robotW = (RobotW *) robot;
                    W_challenge_S(sherlockBag, robotW, isTest);
                } else if (robot->getName()=="RobotSW")
                {
                    RobotSW * robotSW = (RobotSW *) robot;
                    SW_challenge_S(sherlock, sherlockBag, robotSW, isTest);
                }

                // change stage
                sherlock->setExcemp(false);
                sherlock->setStage(2);
            
                // Healing
                S_heal(sherlock, sherlockBag, isTest);

                // change stage 
                sherlock->setStage(0);

                if (isStop())
                {
                    break;
                }
            }
        }
    }
    return;
}

// Robot C challenge Sherlock
void C_challenge_S(Sherlock * sherlock, SherlockBag * sherlockBag, Criminal * criminal, RobotC * robotC, bool isTest)
{
    if (isTest) 
    {
        cout << "--------------------------------------" << endl;
        cout << "Sherlock meet Robot C | Try to catch Criminal" << endl;
        cout << sherlockBag->str() << endl;
        cout << "HP:" << sherlock->getHp() << ", EXP:" << sherlock->getExp() << endl;  
    }
    if (sherlock->getExp()>500)
    {
        sherlock->setPosition(criminal->getCurrentPosition());
        if (isTest) 
        {
            cout << "robot defeated! Sherlock caught Criminal" << endl;
            cout << "Sherlock position: " << sherlock->getCurrentPosition().str() << endl;
            cout << "--------------------------------------" << endl;
        }
    } else
    {
        sherlockBag->insert(robotC->getItem());
        if (isTest)
        {
            cout << "robot defeated, but criminal escaped. Sherlock gains "<< robotC->getItemStr() << endl;
            cout << sherlockBag->str() <<endl;
            cout << "--------------------------------------" << endl;
        }
    }
};

// Robot S challenge Sherlock
void S_challenge_S(Sherlock * sherlock, SherlockBag * sherlockBag, RobotS * robotS, bool isTest)
{
    if (isTest) 
    {
        cout << "--------------------------------------" << endl;
        cout << "Sherlock meet Robot S | Doing calculus" << endl;
        cout << sherlockBag->str() << endl;
        cout << "HP:" << sherlock->getHp() << ", EXP:" << sherlock->getExp() << endl;  
    }
    if (sherlock->getExp()>400)
    {
        sherlockBag->insert(robotS->getItem());
        if (isTest) 
        {
            cout << "robot defeated! Sherlock gains " << robotS->getItemStr() <<endl;
            cout << sherlockBag->str() << endl;
            cout << "--------------------------------------" << endl;
        }
    } else
    {  
        if (!sherlock->isExcemp()) 
        {
            sherlock->setExp(ceilFixed(sherlock->getExp()*0.9));
            if (isTest)
            {
                cout << "robot defeat Sherlock (-10% EXP)" << endl;
                cout << "HP:"<<sherlock->getHp() << ", EXP:"<< sherlock->getExp() <<endl;
                cout << "--------------------------------------" << endl;
            }
        } else 
        {
            if (isTest)
            {
                cout << "robot defeat Sherlock (Excemption used)" << endl;
                cout << "HP:"<<sherlock->getHp() << ", EXP:"<< sherlock->getExp() <<endl;
                cout << "--------------------------------------" << endl;
            }
        }  
    }
}

// Robot W challenge Sherlock
void W_challenge_S(SherlockBag *sherlockBag, RobotW * robotW, bool isTest)
{
    sherlockBag->insert(robotW->getItem());
    if (isTest)
    {
        cout << "--------------------------------------" << endl;
        cout << "Sherlock meet Robot W | Sherlock gains " << robotW->getItemStr() << endl;
        cout << sherlockBag->str() << endl;
        cout << "--------------------------------------" << endl;
    }
}

// Robot SW challenge Sherlock
void SW_challenge_S(Sherlock * sherlock, SherlockBag* sherlockBag, RobotSW * robotSW, bool isTest)
{
    if (isTest) 
    {
        cout << "--------------------------------------" << endl;
        cout << "Sherlock meet robot SW | Have a battle" << endl;
        cout << sherlockBag->str() << endl;
        cout << "HP:" << sherlock->getHp() << ", EXP:" << sherlock->getExp() << endl;  
    }
    if (sherlock->getExp()>300 && sherlock->getHp()>335)
    {
        sherlockBag->insert(robotSW->getItem());
        if (isTest) 
        {
            cout << "robot defeated! Sherlock gains " << robotSW->getItemStr() <<endl;
            cout << sherlockBag->str() << endl;
            cout << "--------------------------------------" << endl;
        }
    } else
    {  
        if (!sherlock->isExcemp()) 
        {
            sherlock->setExp(ceilFixed(sherlock->getExp()*0.85));
            sherlock->setHp(ceilFixed(sherlock->getHp()*0.85));
            if (isTest)
            {
                cout << "robot defeat Sherlock (-15% HP, -15% EXP)" << endl;
                cout << "HP:"<<sherlock->getHp() << ", EXP:"<< sherlock->getExp() <<endl;
                cout << "--------------------------------------" << endl;
            }
        } else 
        {
            if (isTest)
            {
                cout << "robot defeat Sherlock (Excemption used)" << endl;
                cout << "HP:"<<sherlock->getHp() << ", EXP:"<< sherlock->getExp() <<endl;
                cout << "--------------------------------------" << endl;
            }
        }  
    }
}

// Robot C challenge Watson
void C_challenge_W(Watson * watson, WatsonBag* watsonBag, RobotC * robotC, bool isTest)
{
    if (isTest) 
    {
        cout << "Watson meet Robot C | Take robot's item" << endl;  
    }

    watsonBag->insert(robotC->getItem());
    if (isTest) 
    {
        cout << "Watson gains " << robotC->getItemStr() <<endl;
        cout << watsonBag->str() << endl;
        cout << "--------------------------------------" << endl;
    }
    
}

// Robot S challenge Watson
void S_challenge_W(bool isTest)
{
    if (isTest)
    {
        cout << "--------------------------------------" << endl;
        cout << "Watson meet Robot S | Next please!" << endl;
        cout << "<Nothing Happen>" << endl;
        cout << "--------------------------------------" << endl;
    }
} 

// Robot W challenge Watson
void W_challenge_W(Watson *watson,WatsonBag* watsonBag, RobotW * robotW, bool isTest)
{
    if (isTest)
    {
        cout << "Watson meet Robot W | Have a battle" << endl; 
    }
    
    if (watson->getHp()>350)
    {
        watsonBag->insert(robotW->getItem());
        if (isTest) 
        {
            cout << "Robot defeated! Watson gains " << robotW->getItemStr() <<endl;
            cout << watsonBag->str() << endl;
            cout << "--------------------------------------" << endl;
        }
    } else
    {
        watson->setHp(ceilFixed(watson->getHp()*0.95));
        if (isTest) 
        {
            cout << "Robot defeat Watson (-5%HP)" << endl;
            cout << "HP:" << watson->getHp() << ", EXP:" << watson->getExp() << endl; 
            cout << "--------------------------------------" << endl;
        }
    }
}

// Robot SW challenge Watson
void SW_challenge_W(Watson * watson,WatsonBag* watsonBag, RobotSW * robotSW, bool isTest)
{
    if (isTest)
    {
        cout << "Watson meet Robot SW | Have a battle" << endl;
    }

    if (watson->getHp()>165 && watson->getExp()>600)
    {
        watsonBag->insert(robotSW->getItem());
        if (isTest) 
        {
            cout << "Robot defeated! Watson gains " << robotSW->getItemStr() <<endl;
            cout << watsonBag->str() << endl;
            cout << "--------------------------------------" << endl;
        }
    } else
    {
        watson->setHp(ceilFixed(watson->getHp()*0.85));
        watson->setExp(ceilFixed(watson->getExp()*0.85));
        if (isTest) 
        {
            cout << "Robot defeat Watson (-15%HP, -15%EXP)" << endl;
            cout << "HP:" << watson->getHp() << ", EXP:" << watson->getExp() << endl; 
            cout << "--------------------------------------" << endl;
        }
    }
}

void tradeCard(Sherlock *sherlock, SherlockBag* sherlockBag, Watson* watson,WatsonBag* watsonBag, bool isTest)
{
    if (isTest)
    {
        cout << "Sherlock: "<<sherlockBag-> str() << " "<< sherlockBag->isFull()<< endl;
        cout << "Watson: " << watsonBag->str() << " "<< watsonBag->isFull()<< endl;
    }

    while (!watsonBag->isFull())
    {
        BaseItem * item = sherlockBag->get_NC(PASSING_CARD);
        if (item!=NULL) 
        {
            watsonBag->insert(item);
            if (isTest) 
            {
                cout << "--------------------------------" <<endl;
                cout << "Sherlock give PASSING_CARD to Watson" << endl;
                cout << sherlockBag->str() << endl;
                cout << watsonBag->str() << endl;
                cout << "--------------------------------" <<endl;
            }
        }
        else 
        {
            if (isTest) cout << "Sherlock can't found Passing Card." << endl;
            break;
        }
    }
    if (watsonBag->isFull() && isTest) cout << "Watson Bag is full" << endl;
    
    while (!sherlockBag->isFull())
    {
        BaseItem * item = watsonBag->get_NC(EXCEMPTION_CARD);
        if (item!=NULL)
        {
            sherlockBag->insert(item);
            if (isTest) 
            {
                cout << "--------------------------------" <<endl;
                cout << "Watson give EXCEMPTION_CARD to Sherlock" << endl;
                cout << sherlockBag->str() << endl;
                cout << watsonBag->str() << endl;
                cout << "--------------------------------" <<endl;
            }
        } 
        else 
        {
            if (isTest) cout << "Watson can't found Excemption Card." << endl;
            break;
        }
    }
    if (sherlockBag->isFull() && isTest) cout << "Sherlock Bag is full" << endl;
    
    if (isTest)
    {
        cout << "Sherlock: "<<sherlockBag-> str() << endl;
        cout << "Watson: " << watsonBag->str() << endl;
    }
}

void S_heal (Sherlock * s, SherlockBag* sBag, bool isTest)
{
    BaseItem * healItem = sBag->get_St();
    if (healItem!=NULL) 
    {
        healItem->use(s, NULL);
        if (isTest)
        {
            cout << "Healing Stage - " << strItemType(healItem->getType()) << " used" << endl;
            cout << "HP:"<<s->getHp() << ", EXP:"<< s->getExp() <<endl;
            cout << "--------------------------------------" << endl;
        }
    }
}
void W_heal(Watson* w, WatsonBag* wBag, bool isTest)
{
    BaseItem * healItem = wBag->get_St();
    if (healItem!=NULL) 
    {
        healItem->use(w, NULL);
        if (isTest)
        {
            cout << "Healing Stage - " << strItemType(healItem->getType()) << " used" << endl;
            cout << "HP:"<<w->getHp() << ", EXP:"<< w->getExp() <<endl;
            cout << "--------------------------------------" << endl;
        }
    }
}

// for testing...
void SherlockEvents(Sherlock* sherlock, SherlockBag * sherlockBag,Watson* watson, WatsonBag* watsonBag, Criminal * criminal, MovingObject* obj, bool verbose)
{
    bool isTest = verbose;
    
    if (sherlock->meet(obj))
    {
        if (obj->getName()=="Watson")
        {
            tradeCard(sherlock, sherlockBag, watson, watsonBag, verbose);
        }
        else if (obj->getName()=="Criminal")
        {
            if (isTest) cout << "Sherlock: I got you! | Criminal Caught" << endl;
            return;
        } else if (!obj->isCharacter())
        {       
            if (isTest) 
            {
                cout << "--------------------------------------" << endl;
                cout << "Sherlock meet Robot | Finding Excemption Card..." << endl;
                cout << sherlockBag->str() << endl;
            }

            // change stage
            sherlock->setStage(1);

            // find excemption card
            BaseItem * card = sherlockBag->get();
            if (card!=NULL) 
            {
                card->use(sherlock, NULL);
                if (isTest) cout << "Sherlock: Excemption Card used!" << endl;
            } 

            // Challenge
            if (obj->getName()=="RobotC")
            {
                RobotC * robotC = (RobotC *) obj;
                C_challenge_S(sherlock, sherlockBag, criminal, robotC, isTest);
            }
            else if (obj->getName()=="RobotS")
            {
                RobotS * robotS = (RobotS *) obj;
                S_challenge_S(sherlock, sherlockBag, robotS, isTest);
            } else if (obj->getName()=="RobotW")
            {
                RobotW * robotW = (RobotW *) obj;
                W_challenge_S(sherlockBag, robotW, isTest);
            } else if (obj->getName()=="RobotSW")
            {
                RobotSW * robotSW = (RobotSW *) obj;
                SW_challenge_S(sherlock, sherlockBag, robotSW, isTest);
            }

            // change stage
            sherlock->setExcemp(false);
            sherlock->setStage(2);
            
            // Healing
            S_heal(sherlock, sherlockBag, isTest);

            // change stage 
            sherlock->setStage(0);
        } 
    }
    
    return;
}

void WatsonEvents(Sherlock * sherlock, SherlockBag* sherlockBag, Watson* watson, WatsonBag* watsonBag, MovingObject* obj, bool verbose)
{
    bool isTest = verbose;

    if (watson->meet(obj))
    {
        if (obj->getName()=="Watson")
        {
            tradeCard(sherlock, sherlockBag, watson, watsonBag, verbose);
        }
        else if (obj->getName()=="Criminal")
        {
            if (isTest) cout << "Watson: I got you! | Criminal Caught" << endl;
            return;
        } else if (!obj->isCharacter())
        {       
            if (isTest) 
            {
                cout << "--------------------------------------" << endl;
                cout << "Watson meet Robot | Finding Passing Card..." << endl;
                cout << watsonBag->str() << endl;
                cout << "HP:" << watson->getHp() << ", EXP:" << watson->getExp() << endl;  
            }

            // change stage
            watson->setStage(1);

            Robot * robot = (Robot *) obj;
            // Find Passing Card
            BaseItem * card = watsonBag->get();
            if (card!=NULL) card->use(watson, robot);

            if (watson->canPass())
            {
                watsonBag->insert(robot->getItem());
                watson->setPass(false);
                watson->setStage(0);
                if (isTest)
                {
                    cout << "Passing card used! Gains "<< robot->getItemStr() << endl;
                    cout << watsonBag->str() << endl;
                    cout << "EXP:" << watson->getExp() << endl;
                    cout << "--------------------------------------" << endl;
                }
            }
            else
            {
                // Challenge
                if (obj->getName()=="RobotC")
                {
                    RobotC * robotC = (RobotC *) obj;
                    C_challenge_W(watson, watsonBag, robotC, isTest);
                }
                else if (obj->getName()=="RobotS")
                {
                    RobotS * robotS = (RobotS *) obj;
                    S_challenge_W(isTest);
                } else if (obj->getName()=="RobotW")
                {
                    RobotW * robotW = (RobotW *) obj;
                    W_challenge_W(watson, watsonBag, robotW, isTest);
                } else if (obj->getName()=="RobotSW")
                {
                    RobotSW * robotSW = (RobotSW *) obj;
                    SW_challenge_W(watson, watsonBag, robotSW, isTest);
                }

                // change stage
                watson->setStage(2);

                 // Healing
                W_heal(watson, watsonBag, isTest);

                // change stage
                watson->setStage(0);
            }
        } 
    }
    return;
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////