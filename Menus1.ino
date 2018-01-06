/* This sketch is based on Buttons3.  

It is intended to provide an outline Menu / Sub-menu system based around the control buttons on the LCD shield,
with the manoeuvring in any menu done via Up / Down, Right taking you into a sub-menu, Left taking you back up 
a level, snd Select initiating an action (where appropriate).

Going back up a level could also be done using an Exit selection within the menu. */


      // Call libraries:
      #include <hd44780.h>
      #include <hd44780ioClass/hd44780_pinIO.h> // Arduino pin i/o class header

//  declare Arduino pins used for LCD functions and the lcd object:

      // Define LCD pinout:
      const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7;
      hd44780_pinIO lcd(rs, en, db4, db5, db6, db7);

      // Define LCD geometry
      const int LCD_COLS = 16;
      const int LCD_ROWS = 2;

      // Define variables for display on LCD:
      String LCDline1;
      String LCDline2;


void LCDprintfunction () {  //Defines the LCDprintfunction to provide a simple means of outputting.
      lcd.clear();
      lcd.print(LCDline1);
      lcd.setCursor(0,1);
      lcd.print(LCDline2);
      return;
}


void setup()
{
  Serial.begin(9600);
  
  // initialize LCD with number of columns and rows: 
  
  lcd.begin(LCD_COLS, LCD_ROWS);

  
  // Print a preliminary message to the LCD
  
      LCDline1 = "Menu: Select";
      LCDline2 = "to start ..";
      LCDprintfunction();
      delay(100);

  // end of setup loop
}

//  Works OK to here! *******************************************************************

/*  Notes regarding the LCD:
  lcd.setCursor - first column is 0, not 1 - similarly first row is 0, not 1!

The buttons return the following analogue read values:
  Right     - 0
  Up        - 99
  Down      - 254
  Left      - 407
  Select    - 639
  No press  - 1023
*/


// Define initial variables:

String laststatename;


int getstart = 0;

void getstartfn() {
//  Read the analog value produced by pressing the button.
      int returnval = analogRead(0);     
      // Note: with no button pressed, the analogue read returns a value of 1023, 
      // so a statement is needed to catch values over 1000.
      
      //Detect initial decision to access menu using Select button.
      if (returnval>=550 && returnval<=650) {getstart = 1;}
      else getstart = 0;
      return;
}

void menumovefn() {




return;  
}

void loop() {
  // put your main code here, to run repeatedly

int menulevel;
int menuitem;
int menulevelmin = 0;
int menulevelmax = 1;
int menuitemmin = 0;
int menuitemmax = 4;   // deon't think this is going to work - different values dependant on the menu.

   getstartfn();
   if (getstart==1){  //Have we decided to access the menu system?

      /*  Consider a two-level menu system:  Menu A is the top level, and is developed with 4 options.
       *  Assume options Menu A1  and Menu A3 each have sub-menus, with the 0 option for each being Exit (up a level)
       *  and options Menu A2 and Menu A4 have individuaal actions called from functions.
       *  Sub-menus for A1 and A3 each have 3 options.
       */
   
      // Define the string variables to represent the menu choices in main menu A.
      const char* Menu_top_string[]={"Exit", "Motor", "Blink LED fast", "Light", "Blink LED slow"};  //This could continue if needed!

       // Define the string variables to represent the menu choices in sub-menu A1.
      const char* Menu_A1_string[]={"Exit", "Motor ON", "Motor OFF"};
       
      // Define the string variables to represent the menu choices in sub-menu A3.
      const char* Menu_A3_string[]={"Exit", "Light ON", "Light OFF"};
     


//void loop(){
//for (int i = 0; i < 6; i++){
  // Serial.println(myStrings[i]);
   //delay(500);


      

      menulevel = 0;
      menuitem = 0;
      
      //String statename = statename0;  //Sets state to 0 at start of every loop cycle.


      int x = analogRead(0);   


      // Determine movement within menus, based on analogue read:
      if (x<50) {  menulevel = menulevel + 1;}         // Right - display action or sub-menu header
      else if (x<200) {  menuitem = menuitem - 1;}   // Up - if exists, display menu option above, else stay
      else if (x<400) {  menuitem = menuitem + 1;}   // Down - if exists, display menu option below, else stay
      else if (x<600) {  menulevel = menulevel - 1;}   // Left - if exists, display next higher menu level, else stay
      else if (x<800) {  ???;}   // Select - if action, do, else ????
      else if (x>1000){ ???;}    // No Press - stay.


      // Output:  Only produced if the state has changed and a button has been pressed.
  if (statename != laststatename && statename != statename6) {  

      //Serial Monitor output:            
      Serial.print ("x = "); Serial.print (x); Serial.print ("  ");
      Serial.println (statename);
    
      // LCD output:
      lcd.clear();
      LCDline1 = ("x = "); 
      LCDline2 = (statename);
      LCDprintfunction();
      lcd.setCursor(7,0); lcd.print(x);
      delay (50);
      }
      //Reemember the current state for the next loop
      laststatename = statename;

  }
}

  
