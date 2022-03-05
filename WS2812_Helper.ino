// ========================================================================================
// 
uint8_t WS2812_Helper_Reduce(uint8_t &ledValue, int reduceValue) {
  int lv = (int)ledValue;
  int result = lv - reduceValue ;
  if(result < 0) {
    ledValue = 0;
    return ledValue;
  }

  ledValue = (uint8_t)result;

  return ledValue;
}

// ========================================================================================
// 
uint8_t GetMiddle(uint16_t countRgbLeds) {
  return countRgbLeds / 2;
}
int8_t GetMoveOut(uint8_t moveMidle, uint8_t goOnline_index){
  int8_t moveOut = 0;
  if(goOnline_index < moveMidle) {
    moveOut = (int8_t)goOnline_index;
  }
  return moveOut;
}

int8_t GetMoveLeft(uint8_t goOnline_index, uint16_t countRgbLeds){

  // TODO: Wird mehrmals ausgeführt und kommt 
  //      immer mit dem gleichen Ausgangsergebnis.
  uint8_t moveMidle = GetMiddle(countRgbLeds);
  
  int8_t moveOut = GetMoveOut(moveMidle, goOnline_index);

  if(goOnline_index >= moveMidle) {
    return -1;
  }
  
  int8_t moveLeft = 0;
  if(moveMidle - 1 - moveOut >= 0){
    moveLeft = moveMidle - 1 - moveOut;
  }
  return moveLeft;
}

int8_t GetMoveRight(uint8_t goOnline_index, uint8_t countRgbLeds){
  // TODO: Wird mehrmals ausgeführt und kommt 
  //      immer mit dem gleichen Ausgangsergebnis.
  uint8_t moveMidle = GetMiddle(countRgbLeds);
  
  int8_t moveOut = GetMoveOut(moveMidle, goOnline_index);

  if(goOnline_index >= moveMidle) {
    return -1;
  }

  int8_t moveRight = 0;
  if(moveMidle + moveOut < countRgbLeds){
    moveRight = moveMidle + moveOut;
  }
  return moveRight;
}

// ========================================================================================
// 0 = Orange
// 1 = gelb
// 2 = türkis
// 3 = hellblau
// 4 = defuse neutral
// 5 = dark tyrkis
// 6 = dark wine red
// 7 = 
// 8 = lila
// 9 = rosa
void Helper_SetRgbColor(int colorIndex, uint8_t &red, uint8_t &green, uint8_t &blue) {
 
  switch(colorIndex) {
    case(0): {
      // Orange
      red = 255;
      green = 90;
      blue = 0;
      break;
    }
    case(1): {
      // gelb
      red = 255;
      green = 234;
      blue = 0;
      break;
    }
    case(2): {
      // türkis
      red = 0;
      green = 255;
      blue = 18;
      break;
    }
    case(3): {
      // hellblau
      red = 0;
      green = 204;
      blue = 255;
      break;
    }
    case(4): {
      // defuse neutral
      red = 154;
      green = 128;
      blue = 97;
      break;
    }
    case(5): {
      // dark tyrkis
      red = 28;
      green = 72;
      blue = 82;
      break;
    }
    case(6): {
      // dark wine red
      red = 107;
      green = 68;
      blue = 70;
      break;
    }
    case(7): {
      // 
      red = 90;
      green = 79;
      blue = 50;
      break;
    }
    case(8): {
      // lila
      red = 156;
      green = 0;
      blue = 255;
      break;
    }
    case(9): {
      // rosa
      red = 240;
      green = 0;
      blue = 255;
      break;
    }
     case(10): {
      // hell orange
      red = 255;
      green = 222;
      blue = 122;
      break;
    }
    default: {
      red = 10; 
      green = 10;
      blue = 10; 
      break;
    }
  }
}
