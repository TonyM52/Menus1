/* This sketch is based on Buttons3.
  It is intended to provide an outline Menu / Sub-menu system based around the control buttons on the LCD shield,
  with the manoeuvring in any menu done via Up / Down, Right taking you into a sub-menu, Left taking you back up
  a level, snd Select initiating an action (where appropriate).

a level, snd Select initiating an action (where appropriate).
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

//Defines the LCDprintfunction to provide a simple means of outputting.

void LCDprintfunction () {  
  
  return;
}

// Define global variables:

int menurow = 0;        // Row position
int menucol = 0;        // Column position
int menurowmin = 0;     // Minimum allowable value of menurow
int menurowmax = 6;     // Maximum allowable value of menurow
int menucolmin = 0;     // Minimum allowable value of menucol
int menucolmax = 1;     // Maximum allowable value of menucol
int menurowlast = 20;
int menucollast = 20;
char* menutext;
int getstart = 0;
int oldgetstart = 0;
int returnval;


void setup()
{
  Serial.begin(9600);

  // initialize LCD with number of columns and rows:

  lcd.begin(LCD_COLS, LCD_ROWS);

 re_initiate:
  // Print a preliminary message to the LCD

  LCDline1 = "Menu: Select";
  LCDline2 = "to start ..";
  LCDprintfunction();
  
  Serial.println("Printed preliminary message on LCD.");  delay(1000);

  // Confirm start of menu operations:
  //  Read the analog value produced by pressing the button.
  
  do{
  int returnval = analogRead(0);
  
  // Note: with no button pressed, the analogue read returns a value of 1023,
  // so a statement is needed to catch values over 1000.

  //Detect initial decision to access menu using Select button.
  
  if (returnval >= 550 && returnval <= 650) {
    getstart = 1;

    if (getstart != oldgetstart) {
      Serial.print ("In Getstart function: Getstart = "), Serial.println (getstart);
      Serial.print ("In Getstart function: returnval = "), Serial.println (returnval);
    }
    oldgetstart = getstart;
  }
  delay(2000);
  } while (getstart != 1);
  
} // end of setup loop


void do_menu_action() { //Function to handle the actions:

  Serial.println("In ""Do Action"" function");

  if (menurow == 0 && menucol == 0 && returnval <650 && returnval > 550) {
    getstart = 0;
    goto re_initiate;
  }
  
  delay (100);
  return;
}



void loop() {
  // put your main code here, to run repeatedly
int returnval = 1023;

  Serial.println();
  Serial.println ("Start of main loop:");

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

    Serial.println();
    Serial.println ("In main loop, ready to look for button presses:");
    Serial.print ("menurow = "); Serial.print(menurow); Serial.print(", menucol = "); Serial.println (menucol);
  
// Identifies the chosen movements within the menu system, leading to the desired action:

    //  Read the analog value produced by pressing the button.
    
    do{
    returnval = analogRead(0);
    } while (returnval > 1000);
  
   Serial.println();
   Serial.print ("Got returnval = "); Serial.print(returnval);

    do{
    if (returnval == 0) {  //i.e. Right
      menucol = menucol + 1;
      if (menucol > menucolmax) {
        menucol = menucolmax;
      }}
    else if (returnval > 0 && returnval < 150) { // i.e. Up
      menurow = menurow - 1;
      if (menurow < menurowmin) {
        menurow = menurowmin;
      }}
    else if (returnval > 150 && returnval < 300) { // i.e. Down
      menurow = menurow + 1;
      if (menurow > menurowmax) {
        menurow = menurowmax;
      }}
    else if (returnval > 350 && returnval < 450) {  //i.e. Left
      menucol = menucol - 1;
      if (menucol < menucolmin) {
        menucol = menucolmin;
      }}
  
  // Extract the relevant menu text using menurow, meucol:
    if (menucol == 0) {
      menutext = Menu_Column_1[menurow];
    }
    else if (menucol == 1) {
      menutext = Menu_Column_2[menurow];
    }
  Serial.println();
  Serial.print("End of menuposition loop, menurow =  "); Serial.print(menurow); Serial.print(", menucol =  "); Serial.println(menucol);
  
  if (returnval > 450 && returnval < 650) {  //i.e. Select
    do_menu_action();
  }
  delay(50);
 
     // Output:  Only produced if the menu selection has changed.
    Serial.println();
    Serial.println ("In Output section of main loop:");

    if (menurow != menurowlast && menucol != menucollast) {

      //Serial Monitor output:
      Serial.println();
      Serial.println ("In Output section of main loop:");
      Serial.print ("Menu column is: "); Serial.println (menucol);
      Serial.print ("Menu row is: "); Serial.println (menurow);
      Serial.print ("Menu reads: "); Serial.println (menutext);

      // LCD output:
      lcd.clear();
      LCDline1 = (menutext);
      LCDline2 = (" ");
      lcd.clear();
  lcd.print(LCDline1);
  lcd.setCursor(0, 1);
  lcd.print(LCDline2);

      delay (1000);
    }
    } while (returnval <1000);
    
    //Reemember the current state for the next loop

    menurowlast = menurow;
    menucollast = menucol;

  }  // end of void loop



