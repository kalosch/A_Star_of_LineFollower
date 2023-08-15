float previous_error = 0;
float integral = 0;
float dt = 10;
int Error = 0;
float output = 0;
float Setpoint = 50;
unsigned long last_time = 0;
int TimeStemp = 0;
int ReadyMove = 0;
int Speed;

void serial (bool select_monitor) {
  if (select_monitor == true) {
    Serial.print (new_position);
    Serial.print (" ");
    Serial.print (56);
    Serial.println ();
  }
  else {

  }
}

void set_center () {
  float Kp = 0.2;
  float Ki = 0.01;
  float Kd = 1;
  if ( millis() - last_time > 10) {
    last_time = millis();
    new_position = map (position(), 0, 7000, 0, 100);
    Error = 56 - new_position;
    int proportional = Error;
    integral = integral + Error * dt;
    float derivative = (Error - previous_error) / dt;
    output = Kp * proportional + Ki * integral + Kd * derivative;

    if (output > 30) {
      output = 30;
    }
    if (output < -30) {
      output = -30;
    }
    if (integral > 100) {
      integral = 100;
    }
    if (integral < -100) {
      integral = -100;
    }

    if (new_position < 55) {
      //      motorA (output + 10);
      //      motorB (10 + ((-1)*output));
      motorA (output);
      motorB ((-1)*output);
    }
    else if (new_position > 57) {
      //      motorA (output + 10);
      //      motorB (((-1)*output) + 10);
      motorA (output);
      motorB ((-1)*output);
    }
    else {
      motorA (0);
      motorB (0);
    }
    serial (false);
    previous_error = Error;
  }
}

void pid () {
  float Kp = 0.2; //0.2
  float Ki = 0.0015; //0.004
  float Kd = 30; //30
  new_position = map (position(), 0, 7000, 0, 100);
  new_position = simpleKalmanFilter.updateEstimate(new_position);
  if ( millis() - last_time > 10) {
    last_time = millis();
    if (new_position > 100)  {
      new_position = 100;
    }
    if (new_position < 0)    {
      new_position = 0;
    }
    Error = Setpoint - new_position;
    int proportional = Error;
    integral = integral + Error * dt;
    float derivative = (Error - previous_error) / dt;
    output = Kp * proportional + Ki * integral + Kd * derivative;
    if (output > 30) {
      output = 30;
    }
    if (output < -30) {
      output = -30;
    }
    if (integral > 100) {
      integral = 100;
    }
    if (integral < -100) {
      integral = -100;
    }
    readline();
    if (cross_state == 1) {
      motorA (17); //15
      motorB (12); //13
    }
    else {
      if (new_position < 50) {
        motorA (output + 17);
        motorB (12 + ((-1)*output));
      }
      else if (new_position > 50) {
        motorA (output + 17);
        motorB (((-1)*output) + 12);
      }
      else { /*(new_position >= 55 && new_position <= 57)*/
        motorA (17); //15
        motorB (12); //13
      }
    }

    Serial.print("NewPos:" + String(new_position));
    Serial.print("output:" + String(output));
    Serial.println();
    serial (false);
    previous_error = Error;
  }
}
