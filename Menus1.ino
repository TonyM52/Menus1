/* This sketch is based on Buttons3.
  It is intended to provide an outline Menu / Sub-menu system based around the control buttons on the LCD shield,
  with the manoeuvring in any menu done via Up / Down, Right taking you into a sub-menu, Left taking you back up
  a level, snd Select initiating an action (where appropriate).
  a level, snd Select initiating an action (where appropriate).
*/

// Call libraries:

#include <hd44780.h>
#include <hd44780ioClass/hd44780_pinIO.h> // Arduino pin i/o class header

//  LCD stuff:

/*  Notes regarding the LCD:
  lcd.setCursor - first column is 0, not 1 - similarly first row is 0, not 1!

  The buttons return the following analogue read values:
  Right     - 0     Up        - 99      Down      - 254
  Left      - 407   Select    - 639     No press  - 1023
*/

// Define LCD pinout:

const int rs = 8, en = 9, db4 = 4, db5 = 5, db6 = 6, db7 = 7;
hd44780_pinIO lcd(rs, en, db4, db5, db6, db7);

// Define LCD geometry

const int LCD_COLS = 16;
const int LCD_ROWS = 2;

// Define variables for display on LCD:
String LCDline1;
String LCDline2;


// Define global variables:

int menurow = 0;        // Row position
int menucol = 0;        // Column position
int menurowmin = 0;     // Minimum allowable value of menurow
int menurowmax = 6;     // Maximum allowable value of menurow
int menucolmin = 0;     // Minimum allowable value of menucol
int menucolmax = 1;     // Maximum allowable value of menucol
int menurowlast = 20;
int menucollast = 20;
String menutext;
char* Menu_Column_1();
char* Menu_Column_2();
int getstart = 0;
int oldgetstart = 0;
int returnval;
String button;
int i;



void LCDprintfunction () {  //Defines the LCDprintfunction to provide a simple means of outputting.
      lcd.clear();
      lcd.print(LCDline1);
      lcd.setCursor(0,1);
      lcd.print(LCDline2);
      Serial.print ("Done LCD print function: LCD line 1 is "); Serial.print (LCDline1); Serial.print (", LCD line 2 is "); Serial.println (LCDline2);
      return;
}


void setup() {
  Serial.begin(9600);

  // initialize LCD with number of columns and rows:
  lcd.begin(LCD_COLS, LCD_ROWS);

// initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
Serial.println ("Completed set-up loop");
} // end of setup loop


void fastblinkfunction() {
  i=1;
  for (i=1; i<15; i++){
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for 1/2 a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for 1/2 a second
}
return;
}



void slowblinkfunction() {
  i=1;
  for (i=1; i<15; i++) {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for 1 second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for 1 second
}
return;
}






void loop() {
  // put your main code here, to run repeatedly

re_initiate:  //Following "Select" on the "Exit" menu option, need to go back to the beginning.

  // Print a preliminary message to the LCD
  LCDline1 = "Menu: Select";
  LCDline2 = "to start ..";
  LCDprintfunction();

  Serial.println("Preliminary message now on LCD.");  
  
  // Confirm start of menu operations:
    //  Read the analog value produced by pressing the button.
    re_read_1:
    int returnval = analogRead(0);  // Note: with no button pressed, the analogue read returns a value of 1023.
   
    while (returnval == 1023) {  //This loop is to "wait" till a button is pressed.
    // Do Nothing!
    goto re_read_1;
    }

    //Detect initial decision to access menu using Select button.
    
    if (returnval >= 550 && returnval <= 650) {  //i.e. "Select" button has been pressed
      button = "Select";
      getstart = 1;

    if (getstart != oldgetstart) {  //Checking to see if we have JUST got permission to go into the menus
        Serial.print ("Confirmed menu entry: Getstart = "), Serial.println (getstart);
        Serial.print ("In Getstart function: returnval = "), Serial.println (returnval);
      }
      oldgetstart = getstart;
    }

  
  
//Start of menu display proper:


  Serial.println(); Serial.println ("Starting menu display:");

  /*  Consider a two-level menu system:  Menu A is the top level, and is developed with 4 options.
      Assume options Menu A1  and Menu A3 each have sub-menus, with the 0 option for each being Exit (up a level)
      and options Menu A2 and Menu A4 have individuaal actions called from functions.
      Sub-menus for A1 and A3 each have 3 options.
      Overall dimensions of the menu array are thus 5 (rows) x 2 (columns) - i.e.:

      Top menu:         Item:
      Exit              Null        Select on Exit:  go back to start
      Motor             Motor On    Select on Motor On:  Start motor running
      Motor             Motor Off   Select on Motor Off:  Turn motor off
      Blink LED fast    Null        Select on Blink LED fast:  Run the Blink Fast function (self-exits).
      Light             Light On    Select on Light on:  Turn light on.
      Light             Light Off   Select on Light off:  Turn light off.
      Blink LED slow    Null        Select on Blink LED slow:  Run the Blink Slow function (self-exits).
  */

  // Define the string variables to represent the menu choices in Columns 1 and 2:

char* Menu_Column_1[] = {"Exit", "Motor", "Motor", "Blink LED fast", "Light", "Light", "Blink LED slow"}; //This could continue if needed!
char* Menu_Column_2[] = {"Null", "Motor ON", "Motor OFF", "Null", "Light ON", "Light OFF", "Null" };

still_in_menu:

  Serial.println();
  Serial.println ("In main loop, ready to look for button presses:");
  Serial.print ("menurow = "); Serial.print(menurow); Serial.print(", menucol = "); Serial.print (menucol); Serial.print ("returnval = "); Serial.print (returnval);

   // Get the button press and identify action:
   //  Read the analog value produced by pressing the button.

    re_read_2:
    returnval = analogRead(0);  // Note: with no button pressed, the analogue read returns a value of 1023.
   
    while (returnval > 1000) {  //This loop is to "wait" till a button is pressed.
    // Do Nothing!
    delay(50);
    button = "Nul";
    goto re_read_2;
    }

    if (returnval == 0) {  //i.e. Right
      button = "Right";
      menucol = menucol + 1;
      if (menucol > menucolmax) {
        menucol = menucolmax;
      }
    }
    else if (returnval > 0 && returnval < 150) { // i.e. Up
      button = "Up";
      menurow = menurow - 1;
      if (menurow < menurowmin) {
        menurow = menurowmin;
      }
    }
    else if (returnval > 150 && returnval < 300) { // i.e. Down
      button = "Down";
      menurow = menurow + 1;
      if (menurow > menurowmax) {
        menurow = menurowmax;
      }
    }
    else if (returnval > 350 && returnval < 450) {  //i.e. Left
      button = "Left";
      menucol = menucol - 1;
      if (menucol < menucolmin) {
        menucol = menucolmin;
      }
    }
    else if (returnval >= 550 && returnval <= 650) {  //i.e. "Select" 
      button = "Select";
    }

// Extract the relevant menu text using menurow, menucol:
    if (menucol == 0) {
      menutext = Menu_Column_1[menurow];
    }
    else if (menucol == 1) {
      menutext = Menu_Column_2[menurow];
    }


//Displaying menu text and/or taking action if Select button HAS been pressed: 
if (button == "Select") {
  if (menurow == 0 && menucol ==0) { //Main menu "Exit" option.
    getstart = 0;
    goto re_initiate;
    Serial.println ("Exiting menu - restarting from beginning.");
  }
  else if (menurow == 1  && menucol == 1) { // Motor sub menu item Turn Motor ON
    LCDline1 = menutext;
    LCDline2 = "Motor is ON";
      LCDprintfunction();
    Serial.println ("Motor has been selected ON - LCD line 2 updated.");
  }
  else if (menurow == 2  && menucol == 1) { // Motor sub menu item Turn Motor OFF
    LCDline1 = menutext;
    LCDline2 = "Motor is OFF";
      LCDprintfunction();
    Serial.println ("Motor has been selected OFF - LCD line 2 updated.");
  }
    else if (menurow == 3  && menucol == 0) { // Blink LED FAST option
    LCDline1 = menutext;
    LCDline2 = "Fast Blink";
      LCDprintfunction();
    fastblinkfunction();
    Serial.println ("Fast Blink initiated - LCD line 2 updated.");
    }
    else if (menurow == 4  && menucol == 1) { // Light ON option
    LCDline1 = menutext;
    LCDline2 = "Light is ON";
      LCDprintfunction();
    Serial.println ("Light turned ON - LCD line 2 updated.");
    }
    else if (menurow == 5  && menucol == 1) { // Light OFF option
    LCDline1 = menutext;
    LCDline2 = "Light is OFF";
      LCDprintfunction();
    Serial.println ("Light turned OFF - LCD line 2 updated.");
    }
    else if (menurow == 6  && menucol == 0) { // Blink LED SLOW option
    LCDline1 = menutext;
    LCDline2 = "SLOW Blink";
      LCDprintfunction();
    slowblinkfunction();
    Serial.println ("Slow Blink initiated - LCD line 2 updated.");
    }
}

//Check for change:
if (menurow == menurowlast && menucol == menucollast){
  goto still_in_menu;
  //If the menu selection has not changed, and Select HAS NOT been pressed, then restart the read loop:
}


// Displaying the menu text if the Select button HAS NOT been pressed:
if (button != "Select") {
    //Send text to LCD:
    LCDline1 = menutext;
    LCDline2 = "";
    LCDprintfunction();
    Serial.print ("Updated LCD:  Line 1 reads "); Serial.println (LCDline1);
}  //End of menu display loop when Select HAS NOT been pressed.
   
  menurowlast = menurow;
  menucollast = menucol;

  goto still_in_menu;

}  // end of void loop



