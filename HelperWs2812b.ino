//
// 0 = Orange
// 1 = gelb
// 2 = türkis

// 8 = lila
// 9 = rosa
void HelperSetRgbColor(int colorIndex, int &red, int &green, int &blue) {
 
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

      // random
      //red = 9; //random(0, 150);
      //green = 0; //random(0, 150);
      //blue = 0; //random(0, 150);
      break;
    }
  }
}
