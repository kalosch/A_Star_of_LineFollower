#include <Wire.h>
#include <ADS7828.h>
#include <SimpleKalmanFilter.h>
SimpleKalmanFilter simpleKalmanFilter(2, 2, 1);
ADS7828 infa;

int infaData[8];
uint32_t _lastPosition;
int black[] = {4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096}; //นับจากซ้าย
int white[] = {0, 0, 0, 0, 0, 0, 0, 0}; //นับจากซ้าย
int blackWheel[] = {252, 97};
int whiteWheel[] = {630, 500};

int MidPoint[8];
int MidpointBackL = 0;
int MidpointBackR = 0;

float kp, ki, kd, p, i, d, fl, m1, m2;
int error, preverror, last, sumerror, power;
uint8_t Unit_sensor = 8;
bool cross_state = 0;


int Motor_Right_Forward = 9;
int Motor_Right_Reverse = 8;
int Motor_Right_Speed = 13;
int Motor_Left_Forward = 10;
int Motor_Left_Reverse = 11;
int Motor_Left_Speed = 12;

int Buzzer = 53;
int SensorR = A0;
int SensorL = A1;
int new_position = 0;

class Node {
  public:
    Node() {}
    Node(Node* parent, int positionX, int positionY) {
      this->parent = parent;
      this->positionX = positionX;
      this->positionY = positionY;
      this->g = 0;
      this->h = 0;
      this->f = 0;
    }

    bool operator==(const Node& other) const {
      return positionX == other.positionX && positionY == other.positionY;
    }

    Node* parent;
    int positionX;
    int positionY;
    int g;
    int h;
    int f;
};

Node* openList[100];
Node* closedList[100];

int openCount = 0;
int closedCount = 0;
//int start_x = 5;
//int start_y = 3;
//int end_x = 2;
//int end_y = 6;
int start_x = 2;
int start_y = 6;
int end_x = 3;
int end_y = 0;
int previous_first = start_x;
int previous_second = start_y;

int start[2] = {start_x, start_y};
int end[2] = {end_x, end_y};
Node* path[100];
int pathCount = 0;
int current_compass  = 0;
int previous_compass = 1;

int maze[6][9] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0}
};

void setup() {

  Wire.begin();
  infa.begin(0x48);
  Serial.begin(115200);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, HIGH);
  delay (250);
  digitalWrite(Buzzer, LOW);
  pinMode(SensorL, INPUT);                  pinMode(SensorR, INPUT);
  pinMode(Motor_Right_Forward, OUTPUT);    pinMode(Motor_Left_Forward, OUTPUT);
  pinMode(Motor_Right_Reverse, OUTPUT);    pinMode(Motor_Left_Reverse, OUTPUT);
  pinMode(Motor_Right_Speed, OUTPUT);      pinMode(Motor_Left_Speed, OUTPUT);
  for (int i = 0 ; i < Unit_sensor ; i++) {
    MidPoint[i] = (black[i] + white[i]) / 2;
  }
  MidpointBackL = (blackWheel[1] + whiteWheel[1]) / 2;
  MidpointBackR = (blackWheel[2] + whiteWheel[2]) / 2;
  readCalibrate ();
  digitalWrite(Buzzer, HIGH);
  delay (250);
  digitalWrite(Buzzer, LOW);
  A_star (start[0], start[1], end[0], end[1]);
}

void loop() {
  if (digitalRead(4) == 0) {
    while (1) {
      new_position = map (position(), 0, 7000, 0, 100);
      new_position = simpleKalmanFilter.updateEstimate(new_position);
      for (int i = pathCount - 1; i >= 0; i--) {
        
        if (previous_first > path[i]->positionX) {
          current_compass = 1;
          if (current_compass == previous_compass) {
            isCrossing ();
            Serial.print (" forward");
          }
          else {
            if (current_compass - previous_compass == 1) {
              TurnRight ();
              Serial.print  (" rotate_right");
            }
            else if (current_compass - previous_compass == -1) {
              TurnLeft ();
              Serial.print (" rotate_left");
            }
            else if (current_compass - previous_compass == 2 || current_compass - previous_compass == -2) {
              Serial.print (" turn_back");
            }
            isCrossing();
            Serial.print (" Up");
          }
          //Serial.print ("Up");
        }
        if (previous_first < path[i]->positionX) {
          current_compass = 3;
          if (current_compass == previous_compass) {
            isCrossing ();
            Serial.print (" forward");
          }
          else {
            if (current_compass - previous_compass == 1) {
              TurnRight ();
              Serial.print (" rotate_right");
            }
            else if (current_compass - previous_compass == -1) {
              TurnLeft ();
              Serial.print (" rotate_left");
            }
            else if (current_compass - previous_compass == 2 || current_compass - previous_compass == -2) {
              Serial.print (" turn_back");
            }
            isCrossing();
            Serial.print (" Down");
          }
          //Serial.print ("Down");
        }
        if (previous_first == path[i]->positionX) {
          if (previous_second > path[i]->positionY) {
            current_compass = 4;
            if (current_compass == previous_compass) {
              isCrossing();
              Serial.print (" forward");
            }
            else {
              if (current_compass - previous_compass == -3) {
                TurnRight ();
                Serial.print (" rotate_right");
              }
              else if (current_compass - previous_compass == 3) {
                TurnLeft ();
                Serial.print (" rotate_left");
              }
              else if (current_compass - previous_compass == 1) {
                TurnRight ();
                Serial.print (" rotate_right");
              }
              else if (current_compass - previous_compass == -1) {
                TurnLeft ();
                Serial.print (" rotate_left");
              }
              else if (current_compass - previous_compass == 2 || current_compass - previous_compass == -2) {
                Serial.print (" turn_back");
              }
              isCrossing();
              Serial.print (" Left");
            }
            //Serial.print ("Left");
          }
          if (previous_second < path[i]->positionY) {
            current_compass = 2;
            if (current_compass == previous_compass) {
              isCrossing();
              Serial.print (" forward");
            }
            else {
              if (current_compass - previous_compass == 1) {
                TurnRight ();
                Serial.print (" rotate_right");
              }
              else if (current_compass - previous_compass == -1) {
                TurnLeft ();
                Serial.print (" rotate_left");
              }
              else if (current_compass - previous_compass == 2 || current_compass - previous_compass == -2) {
                Serial.print (" turn_back");
              }
              isCrossing();
              Serial.print (" Right");
            }
            //Serial.print ("Right");
          }
          if (previous_second == path[i]->positionY) {
            current_compass = 1;
            previous_compass = 1;
            Serial.println (" Start");
          }
        }

        previous_first = path[i]->positionX;
        previous_second = path[i]->positionY;
        previous_compass = current_compass;
        Serial.print("(");
        Serial.print(path[i]->positionX);
        Serial.print(", ");
        Serial.print(path[i]->positionY);
        Serial.print(")");
        
        Serial.println();
      }
      delay (1000);
      break;
    }
  }

}
