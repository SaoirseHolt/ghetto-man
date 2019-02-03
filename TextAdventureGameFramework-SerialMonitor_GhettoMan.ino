#include <openGLCD.h>
#include <openGLCD_Buildinfo.h>
#include <openGLCD_Config.h>
#include <stdarg.h>

#include "fonts/allFonts.h"
#include "bitmaps/allBitmaps.h"

const arr_len(x) = (sizeof(x) / sizeof(*x));
const int A_Button = 1;
const int B_Button = 2;
const int C_Button = 3;
const int R_Bumper = 4;
const int L_Bumper = 5;
int Button = 0; //Stores what button has been pushed to correspond with an action.
String selOpt; //Stores lineA, lineB, or lineC depending on which button is pressed when prompted.
String incString; //Used by buttonPress() to store data recieved through Serial input.
int Health = 100; //Player's health.
int WeaponUsed = 0; //Stores the current weapon that the player is holding.
int contPages = 1; //Modified by contextOptions() to store number of pages of active context options. Each page will reflect 3 or fewer options.
int currentPage = 1; //Modified by contextOptions() to store active page.
String lineA = ""; //Modified by contextOptions to store the up to 3 active context options.
String lineB = "";
String lineC = "";
boolean SpecialClrScreen = 0;
boolean Item[10]; //Array that stores the items you've collected.
const int NUM_ITEMS = arr_len(Item);
boolean Weapon[10]; //Array that stores the weapons you've collected.
const int NUM_WEAPS = arr_len(Weapon);
int curStage; //Variable that stores the current scene. Used by setStage() and changeStage() to load the appropriate scene.
boolean gameLoop; //Variable that causes the changeStage() function to loop inside adventureGame(). Set value to 0 to return to mainMenu().
const boolean debugMode = 0; //Setting this value to 1 enables "debug mode" (Serial monitor input/output).


gText fullTextarea = gText(0, 0, GLCD.Right, GLCD.Bottom, SystemFont5x7)
gText eventTextarea = gText(0, 0, GLCD.Right, GLCD.CenterY-1, SystemFont5x7);
gText actionTextarea = gText(0, GLCD.CenterY, GLCD.Right, GLCD.Bottom, SystemFont5x7);

void setup()
{
  Serial.begin(9600);
//  GLCD.Init();
//  GLCD.ClearScreen();
//  GLCD.SelectFont(SystemFont5x7, BLACK);
//  titleScreen();
}

void loop()
{
  gameLoop = 1;
  mainMenu();
}

void ClearScreen(boolean L)
{
  GLCD.ClearScreen();
  if (L==0){ //Draws a center line to divide the textareas, for action scenes. INCOMPLETE

  }
  else if(L==1){ //Draws a box around the outside of the screen, for information-only scenes. INCOMPLETE

    SpecialClrScreen = 1;
  }
}

void Prt(String printText, int printRegion = 1, boolean newLine = 1) //Print function. Should be called whenever text is to be printed. First parameter is text to print, second paremeter represents where on the GLCD to print (0 = fullscreen, 1 = top half, 2 = bottom half). Third parameter should be set to 0 to prevent a newline.
{
  if (newLine == 0) {
    switch (printRegion) {
      case 0:
      fullTextarea.println(printText);
      if (debugMode) {Serial.print(printText);}
      break;

      case 1:
      eventTextarea.print(printText);
      if (debugMode) {Serial.print(printText);}
      break;

      case 2:
      actionTextarea.print(printText);
      if (debugMode) {Serial.print(printText);}
      break;

      default:
      fullTextArea.println(); fullTextarea.println("ERROR: Prt function requires a print region from 0-2 to be specified");
      if (debugMode) {Serial.println(); Serial.println("ERROR: Prt function requires a print region from 0-2 to be specified");}
      break;
    }
  }
  else {
    switch (printRegion) {
      case 0:
      fullTextarea.println(printText);
      if (debugMode) {Serial.println(printText);}
      break;

      case 1:
      eventTextarea.println(printText);
      if (debugMode) {Serial.println(printText);}
      break;

      case 2:
      actionTextarea.println(printText);
      if (debugMode) {Serial.println(printText);}
      break;

      default:
      fullTextArea.println(); fullTextarea.println("ERROR: Prt function requires a print region from 0-2 to be specified");
      if (debugMode) {Serial.println(); Serial.println("ERROR: Prt function requires a print region from 0-2 to be specified");}
      break;
    }
  }
}

void titleScreen() //Function to display the title screen of the game.
{
//  GLCD.DrawBitmap(introScreen, 0,0);
  delay(6000);
}

void mainMenu() //Main menu function.
{
//  ClearScreen(0);
//  eventTextarea.println("Welcome to GHETTO MAN!!");
  Prt(1, "Welcome to GHETTO MAN!");
  contextOptions("Start Game", "Display Instructions", "Credits", "");

  if (selOpt == "Start Game") {adventureGame();}
  else if (selOpt == "Display Instructions") {instructions();}
  else if (selOpt == "Credits") {credits();}
}

void buttonPress(int N) //Function to assign button value to $Button. The argument specifies how many inputs we expect, as detailed below. Use buttonPress(0) if you don't care what button is pressed, but it cannot be triggered by L or R (4 or 5).
{
//  while(digitalRead(A_Button == LOW) && digitalRead(B_Button == LOW) && digitalRead(C_Button == LOW) && digitalRead(R_Bumper == LOW) && digitalRead(L_Bumper == LOW)) {};
//  if (digitalRead(A_Button == HIGH)) {Button = 1;}
//  else if (digitalRead(B_Button == HIGH)) {Button = 2;}
//  else if (digitalRead(C_Button == HIGH)) {Button = 3;}
//  else if (digitalRead(R_Bumper == HIGH)) {Button = 4;}
//  else if (digitalRead(L_Bumper == HIGH)) {Button = 5;}
//  else {buttonPressNested();}
  // send data only when you receive data:
  boolean buttonNull = 0;
  if (N == 0 && (Button == 4 || Button == 5)) {buttonNull = 1;} //This allows us to skip the "waiting for input" state for the purpose of changing pages using 4 or 5 (Left/Right buttons).
  while(!buttonNull) {
    while(Serial.available()) {
      // read the incoming byte:
      incString = Serial.readString();
      Button = incString.toInt();

      // say what you got:
      Serial.print("I received: ");
      Serial.println(incString);

      switch (N) {
        case 1: //Input only allows 1 button
        if (Button == 1) {buttonNull = 1;}
        else {Serial.println("Invalid input. Try again.");}
        break;

        case 2: //Input only allows 2 buttons
        if (Button == 1 || Button == 2) {buttonNull = 1;}
        else {Serial.println("Invalid input. Try again.");}
        break;

        case 3: //Input allows 3 buttons (no paging)
        if (Button  == 1 || Button == 2 || Button == 3) {buttonNull = 1;}
        else {Serial.println("Invalid input. Try again.");}
        break;

        case 4: //Input allows 4 buttons (paging left)
        if (Button == 1 || Button == 2 || Button == 3 || Button == 4) {buttonNull = 1;}
        else {Serial.println("Invalid input. Try again.");}
        break;

        case 5: //Input allows 4 buttons (paging right)
        if (Button == 1 || Button == 2 || Button == 3 || Button == 5) {buttonNull = 1;}
        else {Serial.println("Invalid input. Try again.");}
        break;

        case 6: //Input allows 5 buttons (paging left and right)
        if (Button == 1 || Button == 2 || Button == 3 || Button == 4 || Button == 5) {buttonNull = 1;}
        else {Serial.println("Invalid input. Try again.");}
        break;

        case 11: //Input allows only 2 buttons (1 option and paging left)
        if (Button == 1 || Button == 4) {buttonNull = 1;}
        else {Serial.println("Invalid input. Try again.");}
        break;

        case 12: //Input allows only 3 buttons (2 options and paging left)
        if (Button == 1 || Button == 2 || Button == 4) {buttonNull = 1;}
        else {Serial.println("Invalid input. Try again.");}
        break;

        default: //No # of inputs specified
        if (Button == 1 || Button == 2 || Button == 3) {buttonNull = 1;}
        else {Serial.println("(You must press A, B, or C.)");}
        break;
      }
      Serial.println("");
    }
  }
  delay(250);
}

void contextOptions(char* charArray, ...) //Accepts any number of arguments, it is terminated by passing "" as the final argument. Writes arguments to a character array for the purpose of storing context options.
{
  va_list argptr;
  char* cA = charArray;
  int C = 0; //Reflects the number of arguments passed. (A value of 3 means 3 arguments, and so on.)
  String strBuf = "";

  va_start(argptr, charArray);
  while (cA != "") { //As long as the current argument isn't "", we will append it to strBuf, meaning all the arguments will end up in one string.
    strBuf = String(strBuf + cA + ",");
    cA = va_arg(argptr, char*);
    C++;
  }
  va_end(argptr);

  String contextOptions[C+1];
  for (int i=0; i<C; i++) { //Reads strBuf until it finds a comma, indicating a new element. Stores that element as a key in contextOptions, then removes it from strBuf.
    int commaLoc = strBuf.indexOf(',');
    contextOptions[i] = strBuf.substring(0, commaLoc);
    strBuf.remove(0, commaLoc+1);
  }

  contPages = C / 3; //Uses the variable C to determine number of context option pages. Each page consist of 3 or fewer options.
  int remainder = C % 3;
  if (remainder > 0) {contPages++;}
  int pageContRange = currentPage * 3; //Writes context options within the range of the current page to lineA, lineB, and lineC.
  if (pageContRange - 2 <= C) {
    lineA = contextOptions[pageContRange - 3];
    if (pageContRange - 1 <= C) {
      lineB = contextOptions[pageContRange - 2];
      if (pageContRange <= C) {
        lineC = contextOptions[pageContRange - 1];
      }
      else {lineC = "";}
    }
    else {lineB = ""; lineC = "";}
  }
  else {lineA = ""; lineB = ""; lineC = "";}

  if (lineA != "") {Prt("A - ", 2, 0); Prt(lineA, 2, 1);} //Writes lineA, lineB, and lineC to the screen.
  else {Prt("", 2, 1);}
  if (lineB != "") {Prt("B - ", 2, 0); Prt(lineB, 2, 1);}
  else {Prt("", 2, 1);}
  if (lineC != "") {Prt("C - ", 2, 0); Prt(lineC, 2, 1);}
  else {Prt("", 2, 1);}

  if (currentPage < contPages) {Prt("--> (Page ", 2, 0); Prt(currentPage, 2, 0); Prt(" of ", 2, 0); Prt(contPages, 2, 0); Prt(")", 2, 1);} //Prints directional arrows based on the number of pages available and where they are in relation to the active page.
  else if (currentPage > 1) {Prt("<-- (Page ", 2, 0); Prt(currentPage, 2, 0); Prt(" of ", 2, 0); Prt(contPages, 2, 0); Prt(")", 2, 1);}
  else if (currentPage == contPages) {Prt("", 2, 1);}
  else {Prt("<---> (Page ", 2, 0); Prt(currentPage, 2, 0); Prt(" of ", 2, 0); Prt(contPages, 2, 0); Prt(")", 2, 1);}

  if (lineA == "") {buttonPress(0);} //Calls buttonPress(), using the page layout to determine how many context options to check for.
  else if (lineB == "" && currentPage > 1) {buttonPress(11);}
  else if (lineB == "") {buttonPress(1);}
  else if (lineC == "" && currentPage > 1) {buttonPress(12);}
  else if (lineC == "") {buttonPress(2);}
  else if ((currentPage < contPages || currentPage > 1) && currentPage != 1 && currentPage != contPages) {buttonPress(6);}
  else if (currentPage < contPages) {buttonPress(5);}
  else if (currentPage > 1) {buttonPress(4);}
  else {buttonPress(3);}

  selOpt = "";
  switch (Button) { //Based on which button is pressed, it will either modify selOpt with the corresponding context option or change pages.
    case 1:
    selOpt = lineA;
    break;

    case 2:
    selOpt = lineB;
    break;

    case 3:
    selOpt = lineC;
    break;

    case 4:
    currentPage--;
    break;

    case 5:
    currentPage++;
    break;
  }
}

//void buttonPressNested() //Function for use as a nested buttonPress() call, in case their first press doesn't register.
//{
//  while(digitalRead(A_Button == LOW) && digitalRead(B_Button == LOW) && digitalRead(C_Button == LOW) && digitalRead(R_Bumper == LOW) && digitalRead(L_Bumper == LOW)) {};
//  if (digitalRead(A_Button == HIGH)) {Button = 1;}
//  else if (digitalRead(B_Button == HIGH)) {Button = 2;}
//  else if (digitalRead(C_Button == HIGH)) {Button = 3;}
//  else if (digitalRead(R_Bumper == HIGH)) {Button = 4;}
//  else if (digitalRead(L_Bumper == HIGH)) {Button = 5;}
//  else {buttonPressNested();}
//}

void recieveItem(int v)
{
  Item[v] = 1;
}

void recieveWeapon(int w)
{
  Weapon[w] = 1;
}

void clearItems()
{
  for (int i = 0; i < NUM_ITEMS; i++) {
    Item[i] = 0;
  }
}

void clearWeapons()
{
  for (int i = 0; i < NUM_WEAPS; i++) {
    Weapon[i] = 0;
  }
}

void setStage(int n) //Function to set which is the current scene. Sets all other stages to 0, then sets the current one to 1. Sets currentPage to 1 when called so we always start on the first page of a new scene.
{
  curStage = n;
  currentPage = 1;
}

void changeStage() //Function to load the proper scene. Goes through all the possible stages until it finds the one that is set to 1 (the scene to load), and then loads it.
{
//  if (SpecialClrScreen == 1) {SpecialClrScreen = 0;}
//  else {ClearScreen(0);}

//  Serial.println(curStage); //For debugging, prints the current stage to the screen.
  switch (curStage) {
    case 0:
    Prison();
    break;

    case 1:
    PrisonDuel();
    break;

    case 2:
    Alley1();
    break;

    case 3:
    Alley2();
    break;

    case 4:
    Dumpster();
    break;

    case 5:
    AlleyLadder();
    break;

    default:
    gameLoop = 0;
//    ClearScreen(1);
//    eventTextarea.print("Something seems to have broken. Sorry about that! Press any button to return to the Main Menu. (ERROR:Undefined Stage)");
    Prt("Something seems to have broken. Sorry about that! Press any button to return to the Main Menu. (ERROR:Undefined Stage)", 0, 1);
    buttonPress(0);
  }
}

void adventureGame() //Function to kick off gameplay.
{
//  ClearScreen(1);
//  eventTextarea.print("You are in a jail cell.");
  Prt("You are in a jail cell.");
//  actionTextarea.print("(Press any button to examine your surroundings)");
  Prt("(Press any button to examine your surroundings)", 2);
  setStage(0);
  clearItems();
  clearWeapons();
  buttonPress(0);
  while(gameLoop) {
    changeStage();
    buttonPress(0);
  }
}

void instructions() //Instructions function for main menu.
{
//  GLCD.ClearScreen();
  Prt("You are Ghetto Man! Once upon a time, there was a fellow named Jeff Harris. A fairly young man fresh out of \"da hood,\" he was working as a freshman focus teacher at an undisclosed highschool in unpredictable-weather-ville. He was imprisoned for calling his students' projects \"ghetto.\" Now, he wants to escape, but he needs YOUR help to find his freedom! (Any persons or places resembling real-life things are actually purely fictitious.)", 0);
//  GLCD.print("You are Ghetto Man! Once upon a time, there was a fellow named Jeff Harris. A fairly young man fresh out of \"da hood,\" he was working as a freshman focus teacher at an undisclosed highschool in unpredictable-weather-ville. He was imprisoned for calling his students' projects \"ghetto.\" Now, he wants to escape, but he needs YOUR help to find his freedom! (Any persons or places resembling real-life things are actually purely fictitious.)");
  buttonPress(0);
}

void credits() //Credits function for main menu.
{
//  GLCD.DrawBitmap(introScreen, 0,0);
  Prt("Programming - Saoirse Holt", 2);
  Prt("Design - Saoirse Holt", 2);
  Prt("Writing - Saoirse Holt", 2);
  buttonPress(0);
}

//From here on, all functions represent a form of a scene.

void Prison() //Stage 0
{
  Prt("You observe a multitude of objects adorning your cell.");
  if (Item[0] || Weapon[0]) {contextOptions("Cot", "Cellmate", "Toilet", "Striped Pajamas", "Toilet Paper", "");}
  else {contextOptions("Cot", "Cellmate", "Toilet", "Striped Pajamas", "Toilet Paper", "Food Tray", "");}

  if (selOpt == "Cot") {Prt("It's a cot.");}
  else if (selOpt == "Cellmate") {
    if (Weapon[0]) {setStage(1); Prt("The hulking beast of a man sharing your cell snarls and lunges at you. Guards come running to break up the duel...");}
    else {Prt("Your fellow inmate doesn't appreciate the look you give him and beats you up.");}
  }
  else if (selOpt == "Toilet") {
    if (Weapon[0]) {setStage(1); Prt("The guards hear you flush, and they are none too happy about you being up after curfew...");}
    else {Prt("The prison guards come to enforce the \"no toilet after 9:00 PM\" rule, and proceed to beat you up.");}
  }
  else if (selOpt == "Striped Pajamas") {Prt("You won't win any awards for style wearing these.");}
  else if (selOpt == "Toilet Paper") {
    if (Item[0]) {recieveWeapon(0); Item[0] = 0; Prt("You shave the food tray down to a fine point. Not the most impressive weapon ever, but it'll have to do. (Acquired Plastic Shiv!)");}
    else {Prt("It's pretty rough. Might actually be sandpaper.");}
  }
  else if (selOpt == "Food Tray") {Item[0] = 1; Prt("You pick up the food tray. You note that it appears pretty sturdy, and that you could perhaps fashion it into a weapon. (Acquired Plastic Food Tray!)");}
}

void PrisonDuel() //Stage 1
{
  Prt("The guards are approaching you, about to draw their batons. This is probably going to hurt.");
  contextOptions("*Cross swords*", "*Cower in fear*", "");

  if (selOpt == "*Cross swords*") {setStage(2); Prt("It isn't pretty but you come out on top. Leaving the guards groaning on your cell floor, you make a break for the employee entrance.");}
  else if (selOpt == "*Cower in fear*") {gameLoop = 0; Prt("The guards find your shiv, and they lay into you. After a thorough, meticulous beating, they haul you off to solitary confinement. Good luck getting out now. GAME OVER");}
}

void Alley1() //Stage 2
{
  Prt("You burst out into the cold night air and collapse against a rough brick wall, wheezing from exertion. The city is still bustling even this late; a loud, blaring horn roars for a good 10 seconds from a nearby intersection, encouraging you to duck into a dark alley. The harsh fluorescents from the jail have robbed you entirely of your night vision.")
  Prt("(Press any button to examine your surroundings)", 2);
  setStage(3);
}

void Alley2() //Stage 3
{
  if (Weapon[1]) {Prt("The vagabond is nowhere to be seen.");
  contextOptions("Dumpster", "Ladder", "");
  }
  else if (Weapon[2]) {Prt("A puddle of goo is seeping out of the grey, shriveled husk on the ground, slowly dissolving the asphalt around it.");
  contextOptions("Dumpster", "Ladder", "");
  }
  else {Prt("A dumpster has been tipped forward and shoved up against the concrete wall of an apartment complex. Lying on his side, head propped up by an asphalt clod, a particularly grizzled vagabond eyes you warily.");
  contextOptions("Dumpster", "Hobo", "Ladder", "");
  }

  if (selOpt == "Dumpster") {setStage(4); Prt("Big. Metal. Smelly. You kneel down to take a look inside.");}
  else if (selOpt == "Hobo") {Prt("You take a step toward the vagabond, who sits up to look at you questioningly.");}
  else if (selOpt == "Ladder") {setStage(5); Prt("You might be able to reach it if you jump.");}
}

void Dumpster() //Stage 4
{
  Prt("The stench is overwhelming, but there may be something of use in here.");
  if (Item[1]) {contextOptions("Rotten Fruit", "*Leave Dumpster*", "");}
  else {contextOptions("Snorkel", "Rotten Fruit", "*Leave Dumpster*", "");}

  if (selOpt == "Snorkel") {Item[1] = 1; Prt("There aren't many uses for one of these, but you may as well grab it. (Acquired Snorkel!)");}
  else if (selOpt == "Rotten Fruit") {Prt("You're not a fruit fly, best leave these where they are.");}
  else if (selOpt == "*Leave Dumpster*") {setStage(3); Prt("Dumpster diving isn't really your thing. You crawl backward and let the lid return to its preferred angle.");}
}

void AlleyLadder() //Stage 5
{
  Prt("It looks to be about 80% rust.");
  contextOptions("Climb", "Leave", "");

  if (selOpt == "Climb") {Prt("There are 3 rungs missing in a row."); contextOptions("Jump", "I'll pass", "");
    if (selOpt == "Jump") {Prt("You tense your legs and launch upward, barely catching yourself as the bottom half of the ladder crashes to the ground and disintegrates to ruddy brown dust. Seconds later you hear police sirens wailing concerningly nearby. The only way out is up.", 0);}
  }
  else {Prt("I guess you can check this out *ladder*.");}
  setStage(3);
}
