void isCrossing () {
  while (1)
  {
    readline();
    if (infaData[0] < MidPoint[0] && infaData[7] < MidPoint[7])
    {
      digitalWrite(Buzzer, HIGH);
      delay(50);
      digitalWrite(Buzzer, LOW);
      while (analogRead(A0) > MidpointBackR && analogRead(A1) > MidpointBackL){pid();}
      Stop();
      break;
    }
    else
    {
      pid ();
    }
  }
}
void TurnRight () {
  while (1) {
    if (infa.read(7) > MidPoint[7]) {
      motorA(-20);
      motorB(14);  
    }
    else {
      motorA(-100);
      motorB(-100);
      delay(10);
      motorA(0);
      motorB(0);
      break;  
    }
  }
}

void TurnLeft () {
  while (1) {
    if (infa.read(0) > MidPoint[0]) {
      motorA(17);
      motorB(-15);  
    }
    else {
      motorA(-100);
      motorB(-100);
      delay(10);
      motorA(0);
      motorB(0);
      break;  
    }
  }
}
