/*
Kézi eszköz terminál,
1 kijelző
beolvasáskor kiírja a kijelzőre a nevet és a rajta lévő összeget,
2 gomb
12 gombos
lcd
*/

// #define DUAL_LCD_SCREEN

#include "include/input.h"
#include "include/display.h"
#include "include/CardService.h"
#include <Keypad.h>

CardService cardservice;

Profile temp_profile;

// DO NOT GIVE BIGGER NAME THAN THIS, IT WILL CAUSE THE SD CARD TO GO KERNEL PANIC
#define LOGFILE_NAME "HAND0.log"

#define button_pin_enter A0
#define button_pin_cancel A1

const byte ROWS = 4; // four rows
const byte COLS = 3; // three columns

const char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}};

#define MAXKEYPADINPUT 12
char key_input[MAXKEYPADINPUT] = {0};
short key_input_size = 0;
const byte rowPins[ROWS] = {3, 4, 5, 6}; // 2-7-6-4 LÁBAK a D3-D4-D5-D6 PORTRA
const byte colPins[COLS] = {A2, A3, 2};  // 3-1-5 LÁBAK a D7-D8-D9 PORTRA

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// TODO add keypad
// Redo pin layout
Button btn_enter(button_pin_enter);
Button btn_cancel(button_pin_cancel);

void Error_loop()
{
  lcd_write("Syst. init fail", "Restart device!");
  while (1)
  {
    delay(200);
  }
}

/**
 * @brief Get the Key Pad Input, returns true if changed
 *
 * @param buff input buffer
 * @param lenght lenght of the buffer
 * @param isPositive sign flag
 */
bool GetKeyPadInput(char *buff, bool &isPositive, short &lenght, short max)
{
  char key = keypad.getKey();
  if (key)
  {
    logn("blenght: " + String(lenght));
    logn("KEY: " + String(key));

    // beep(BEEP_CLICK); // TODO redo

    if (key == '*')
    {
      // change sign
      isPositive = !isPositive;
    }
    else if (key == '#') // backspace, remove one from last
    {
      if (lenght > 0)
        lenght--;
      buff[lenght] = '\0';
    }
    else
    {
      if (lenght < (max - 1))
      {
        logn("kissebb" + String(max));
        buff[lenght++] = key;
        buff[lenght] = '\0';
      }
    }
    logn("lenght: " + String(lenght));
    logn("keys: " + String(key_input));
    return true;
  }
  return false;
}

void setup()
{
  init_log; // Initialize serial communications with the PC, cn be turned off by define

  lcd_init(); // initialize the 2x16 lcd display i2c

  bool succes = cardservice.init(); // init SPI rfid and sd card system

  if (!succes)
  {
    Error_loop();
  }

  // led_zold.turn(on); // if all good green led turned on
  logn("Connected!");

  pinMode(BUZZER, OUTPUT); // Set buzzer - pin 8 as an output //MOVE TO DISPLAY
  // beep(3);

  beep(5);

  // delay(1000);

  // tone(BUZZER, 808, 1000);
}

uint32_t next_check_time = 0;

void loop()
{

clear_card:

  temp_profile = Profile();

  logn("Waiting for Card.");
  lcd_write("Kartya", "  Kereses");

  // short offset = 0;

  while (true)
  {

    if (next_check_time <= millis())
    {
      if (cardservice.Get_new_card())
        break;
      if (cardservice.Check_card())
        break;
      next_check_time = millis() + 1000;
    }
    delay(500);

    // lcd1.clear();
    // lcd1.setCursor(0, 0);
    // lcd1.print("Hello Stalker!");
    // lcd1.setCursor(0, 1);
    // lcd1.print(&"Kerlek helyezd be a kartyat"[offset %= 28]);
    // offset++;
  }
  cardservice.Read_profile_from_card(&temp_profile);
  temp_profile.print();

  // int32_t cost = 0;
  bool update = true;
  bool isPositive = false;

  key_input[0] = '\0';
  key_input_size = 0;

button_pressing:
  /////////////////////
  /// Kártya behelyezve/
  /////////////////////
  lcd_write(temp_profile.name, String(temp_profile.balance).c_str());

  // led_sarga.turn(on);

  while (true)
  {
    if (next_check_time <= millis())
    {
      // logn("check");
      if (!cardservice.Check_card())
        return;
      next_check_time = millis() + 200;
    }

    update = GetKeyPadInput(key_input, isPositive, key_input_size, MAXKEYPADINPUT);
    // logn("afk"+String(key_input_size));
    if (btn_enter.GetState() == ButtonState::Pressed)
    {
      update = true;
      logn("Enter");
      goto transaction;
    }
    if (btn_cancel.GetState() == ButtonState::Pressed)
    {
      key_input[0] = '\0';
      key_input_size = 0;
      isPositive = false;
      update = true;
      // beep(1);
      logn("cancel");
    }

    if (update) // update lcd screen with new data
    {
      update = false;

      lcd0.clear();
      // lcd1.clear();
      lcd0.setCursor(0, 0);
      // lcd1.setCursor(0, 0);

      static char buff[32];
      sprintf(buff, "Egyn: %ld", temp_profile.balance); // do we need this refresh?
      lcd0.print(buff);
      // lcd1.print(buff);

      sprintf(buff, "Terh:%s%s", isPositive ? " " : "-", key_input);
      lcd0.setCursor(0, 1);
      // lcd1.setCursor(0, 1);
      lcd0.print(buff);
      // lcd1.print(buff);
    }
    delay(30);
  }

transaction:

  long long_value = String(key_input).toInt() * (isPositive ? 1 : -1);

  // if 0 ther may be an error TODO check this

  long_value = constrain(long_value, -MAX_VALUE, MAX_VALUE);
  // it is save to convert into int32_t
  int32_t cost_value = int32_t(long_value);

  if (cost_value == 0)
  {
    // Maybe error, check TODO
    goto button_pressing;
  }

  // All good
  // pénz kell a kártyára írni

  logn("Transaction.");

  int32_t previous_balance = temp_profile.balance;

  auto result = temp_profile.add_to_balance(cost_value);

  // ellenörzés, hogy biztos megcsinálható e
  if (result < 0)
  { // nem mehetünk minuszba
    beep(4);
    // TDO write to lcd
    lcd_write("Hiba! Nincs eleg", " fedezet!", lcd0);
    // lcd_write("Hiba! Nincs eleg", " fedezet!", lcd1);
    delay(5000);
    // led_piros.turn(off);
    goto button_pressing;
  }

  // try to write it to the card
  if (!cardservice.Write_profile_to_card(&temp_profile))
  {
    // temp_profile.balance = previous_balance;
    // temp_profile.transaction_count--;
    //  profile save failure, something went wrong
    lcd_write("Transaction fail", "Contact IT");
    delay(5000);
    lcd_write("Device will", "Reset");
    delay(5000);
    return;
  }
  // everything is fine, card write is done and succesfull

  beep(6);
  delay(100);

  if (result == TransactionResult::MAXEDOUT)
  {
    beep(6); // TODO new beep
    lcd_write("Kartya", "   megtelt");
    delay(1000);
  }

  if (!cardservice.Check_card())
  {
    return; // start from the begining
  }

  temp_profile.print();

  {
#ifdef USESERIAL
    // 16 + 1 +
    static char str[80];
    //  log to card
    //  name,uid,transaction_cunt,previous,added,new_balance
    // 4294967296
    //"aaaaaaaaaaaaaaaa,4294967296,4294967296,-2147483648,-2147483648,-2147483648R"
    logn(temp_profile.balance);
    sprintf(str, "%s,%lu,%lu,%ld,%ld,%ld\n", temp_profile.name, temp_profile.uid, temp_profile.transaction_count, previous_balance, cost_value, temp_profile.balance); // csv format
    logn(String(str));
#endif
  }
}
