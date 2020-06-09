#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Rotary.h>

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/* 
rotary button values:
1 - resting: 1023
2, 3, 4 - rotate CW: 550, then 622, then 715
4, 3, 2 - rotate CCW: 715, then 622, then 550
5 - center button: 25
6 - back button: 350
*/
Rotary rotary(A0, 1023, 550, 622, 715, 25, 350, 10);
RotaryReturn rotaryOutput;

Menu menuState = Top;
byte menuOption;

int axis0;
int axis1;
int axis2;
int axis3;

void setup()
{

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("state:");
  lcd.setCursor(0, 1);
  lcd.print("count:");
}

void loop()
{
  rotaryOutput = rotary.check();

  // update the menu state using the collected rotary input
  updateMenu(rotaryOutput);


  //delay(1);        // delay in between reads for stability
}

#pragma region MENU

enum Menu {Top, Data, Pair, Bind, Calibrate, Calibrate_All, Calibrate_Single};
byte submenuSelected = 0;
byte dataPage = 0;
unsigned long timeOfButtonPress;

void updateMenu(RotaryReturn input)
{
  switch (menuState) // OOP didn't work out so I'm making this big ass switch.
  {
  case Top:
    if (input == none)
    {
    }
    else if (input == Up || input == Down)
    {
      if (input == Down)
      {
        submenuSelected++;
        if (submenuSelected > 3)
        {
          submenuSelected = 0;
        }
      }      
      else
      {
        submenuSelected--;
        if (submenuSelected > 3)
        {
          submenuSelected = 3;
        }
      }
      renderTop();
    }
    else if (input == Button1) // if button1 pressed, go to currently selected submenu
    {
      switch (submenuSelected)
      {
      case 0: // Data
        menuState = Data;
        return;
      case 1: // Pair
        menuState = Pair;
        return;
      case 2: // Bind
        menuState = Bind;
        return;
      case 3: // Calibrate
        menuState = Calibrate;
        return;
      default:
        break;
      }
    }
    else if (input == Button2)
    {
      //nomally exit, but do nothing here since this is the top level menu
      //return %parentMenu
    }
    break;

  case Data:
    if (input == none)
    {
    }
    else if (input == Up || input == Down)
    {
      if (input == Down)
      {
        dataPage++;
        if (dataPage > 2)
        {
          dataPage = 0;
        }
      }      
      else
      {
        dataPage--;
        if (dataPage > 2)
        {
          dataPage = 3;
        }
      }
    }
    else if (input == Button2)
    {
      menuState = Top;
      renderTop();
      return;
    }
    else if (input == Button1) // while button1 is pressed, show name/legend to current data menu.
    {
      timeOfButtonPress = millis();
    }
    renderData();

    break;    

  default:
    break;
  }
}

void renderTop()
{
  lcd.setCursor(0, 0);
  switch (submenuSelected)
  {
  case 0: // Data
    lcd.print("Data            ");
    break;
  case 1: // Pair
    lcd.print("Pair            ");
    break;
  case 2: // Bind
    lcd.print("Bind            ");
    break;
  case 3: // Calibrate
    lcd.print("Calibrate       ");
    break;
  default:
    break;
  }
  lcd.println("            --->");
}

void renderData()
{
  if (timeOfButtonPress + 1000 > millis())
  {
    // Data pages:
    // 0 - Axes
    // 1 - Buttons
    // 2 - Paired
    lcd.setCursor(0, 0);
    switch (dataPage)
    {
    case 0: // Axes
      lcd.print("Axes 1-4        ");
      break;
    case 1: // Buttons
      lcd.println("Buttons 1-8     ");
      lcd.println("X when pressed  ");
      break;
    case 2: // Paired
      lcd.println("battery level of");
      lcd.println("paired device");
      break;
    default:
      break;
    }
  }
  else
  {
    lcd.setCursor(0, 0);
    switch (dataPage)
    {
    case 0: // Data
      lcd.print("Data            ");
      break;
    case 1: // Pair
      lcd.print("Pair            ");
      break;
    case 2: // Bind
      lcd.print("Bind            ");
      break;
    default:
      break;
    }
  }
}

/*
class Menu
{
private:
    
public:
    Menu();
    ~Menu();

    virtual Menu* UpdateMenu(RotaryReturn input)
    {
      return mainMenu();
    }
};

class mainMenu : public Menu
{
private:
  byte submenuSelected;
public:
  mainMenu()
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("Data            ");
    lcd.println("            --->");
  };

  ~mainMenu();

  Menu* UpdateMenu(RotaryReturn input)
  {
    if (input == none)
    {
    }
    else if (input == CW || input == CCW)
    {
      if (input == CW)
      {
        submenuSelected++;
        if (submenuSelected > 3)
        {
          submenuSelected = 0;
        }
      }      
      else
      {
        submenuSelected--;
        if (submenuSelected > 3)
        {
          submenuSelected = 3;
        }
      }
      switch (submenuSelected)
      {
      case 0: // Data
        lcd.setCursor(0, 0);
        lcd.print("Data            ");
        break;
      case 1: // Pair
        lcd.setCursor(0, 0);
        lcd.print("Pair            ");
        break;      
      case 2: // Bind
        lcd.setCursor(0, 0);
        lcd.print("Bind            ");
        break;      
      case 3: // Calibrate
        lcd.setCursor(0, 0);
        lcd.print("Calibrate       ");
        break;
      default:
        break;
      }
    }
    else if (input == Button1) // if button1 pressed, go to currently selected submenu
    {
      switch (submenuSelected)
      {
      case 0: // Data
        return dataMenu();
      case 1: // Pair
        return pairMenu();
      case 2: // Bind
        return bindMenu();
      case 3: // Calibrate
        return calibrateMenu();
      default:
        break;
      }
    }
    else if (input == Button2)
    {
      //nomally exit, but do nothing here since this is the top level menu
      //return %parentMenu
    }
    return this;
  }
};

class dataMenu : public Menu
{
private:
  byte pageOpen;
  // Data pages:
  // 0 - Axes  
  // 1 - Buttons
  // 2 - Paired
public:
  dataMenu()
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("Data            ");
    lcd.println("            --->");
  };

  ~dataMenu();

  Menu* UpdateMenu(RotaryReturn input)
  {
    if (input == none)
    {
    }
    else if (input == CW || input == CCW)
    {
      if (input == CW)
      {
        pageOpen++;
        if (pageOpen > 3)
        {
          pageOpen = 0;
        }
      }      
      else
      {
        pageOpen--;
        if (pageOpen > 3)
        {
          pageOpen = 3;
        }
      }
      switch (pageOpen)
      {
      case 0: // Data
        lcd.setCursor(0, 0);
        lcd.print("Data            ");
        break;
      case 1: // Pair
        lcd.setCursor(0, 0);
        lcd.print("Pair            ");
        break;      
      case 2: // Bind
        lcd.setCursor(0, 0);
        lcd.print("Bind            ");
        break;      
      case 3: // Calibrate
        lcd.setCursor(0, 0);
        lcd.print("Calibrate       ");
        break;
      default:
        break;
      }
    }
    else if (input == Button2)
    {
      //nomally exit, but do nothing here since this is the top level menu
      //return %parentMenu
    }
    else if (input == Button1) // while button1 is pressed, show name/legend to current data menu.
    {
      switch (pageOpen)
      {
      case 0: // Axes
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.println("Control axes    ");
        return this;
      case 1: // Pair
        return pairMenu();
      case 2: // Bind
        return bindMenu();
      default:
        break;
      }
    }
    return this;
  }
};
*/


#pragma endregion MENU