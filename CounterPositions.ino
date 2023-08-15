void readline() {
  for (int i = 0 ; i < Unit_sensor ; i++) {
      infaData[i] = infa.read(i);
  }
  if (infaData[2] < MidPoint[2] && infaData[3] < MidPoint[3] && infaData[4] < MidPoint[4] && infaData[5] < MidPoint[5]) {
    cross_state = 1;
  }
  else {
    cross_state = 0;
  }
}
uint16_t position() {
  bool onLine = false;
  uint32_t Average = 0; 
  uint32_t sum = 0;
  uint16_t current_sum;
  for (uint8_t i = 0 ; i < Unit_sensor ; i++) {
    uint16_t value = map(uint16_t(infa.read(i)), black[i], white[i], 1000, 0);
    if (value < 0) {value = 0;}
    if (value > 1000) {value = 1000;}
    if (value > 200) { onLine = true;}
    if (value > 50) {
      Average += (uint32_t)value * (i * 1000);
      sum += value;
    }
  }
  if (!onLine)
  {
    // If it last read to the left of center, return 0.
    if (_lastPosition < (Unit_sensor - 1) * 1000 / 2) {
      return 0;
    }
    // If it last read to the right of center, return the max.
    else {
      return (Unit_sensor - 1) * 1000;
    }
  }
  _lastPosition = Average / sum;
  return _lastPosition;
}

void readCalibrate () {
  uint16_t Cmax, Cmin;
  for (int i = 0 ; i < 400 ; i++) {
    for (int j = 0 ; j < Unit_sensor ; j++) {
      uint16_t _data = infa.read(j);
      //uint16_t _data = map(infa.read(j), 0, 4096, 0, 1000);
      if (_data > white[j]) {
        white[j] = _data;
      }
      if (_data < black[j]) {
        black[j] = _data;
      }
    }
  }
}
