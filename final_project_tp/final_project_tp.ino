#include <U8glib.h>
#include <Wire.h>

const int array_size = 10;             // max amount of balls that can be in queue
int next_ball[array_size] = {}; // the "queue" of balls waiting to be sorted
int amount_in_array = 0;               // The amount of vaulues in next_ball[]
int just_sorted = 0; //The last idenetified golf ball

int sorted_prov1 = 0; // amount sorted to gate 1 = ProV1
int sorted_nxt = 0;   // amount sorted to gate 2 = Nxt Tour
int sorted_nike = 0;  // amount sorted to gate 3 = Nike

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NO_ACK);

void setup()
{
  pinMode(2, OUTPUT); // Gate 1 relay
  pinMode(3, OUTPUT); // Gate 2 relay
  pinMode(4, OUTPUT); // Gate 3 relay
  pinMode(5, INPUT);  // Shock module for gate 1
  pinMode(6, INPUT);  // Shock module for gate 2
  pinMode(7, INPUT);  // Shock module for gate 3
  Wire.begin();
  oled.setFont(u8g_font_helvB10);

  Serial.begin(115200); //BYT BAUDRATE?
}

void loop()
{
  updateOled("ProV1: " + String(sorted_prov1), "Nxt: " + String(sorted_nxt), "Nike: " + String(sorted_nike));

  update_just_sorted();


  if (just_sorted > 0)
  {
    addball(just_sorted);
  }


  if (amount_in_array > 0)
  {
    open_gate(next_ball[0]);
  }
}

/*
Updates the oled screen to show the amount of balls sorted
Parameters: none
Returns: none
*/
void updateOled(String v1, String nxt, String nike)
{
  oled.firstPage();
  do
  {
    oled.drawStr(10, 15, v1.c_str());
    oled.drawStr(10, 35, nxt.c_str());
    oled.drawStr(10, 55, nike.c_str());

  } while (oled.nextPage());
}

/*
  updates the just_sorted var to the last detacted golf ball
  Parameters: none
  Returns: none

*/
void update_just_sorted()
{
  if (Serial.available() > 0){
    String msg = Serial.readString();
    if (msg == "1"){
      just_sorted = 1; 
    }
    else if (msg == "2"){
      just_sorted = 2; 
    }
    else if (msg == "3"){
      just_sorted = 3; 
    }
    else {
      just_sorted = 0;
    }
  }
  // Serial.print(just_sorted);
}

/*
    This function writes the current values in the nextball array to the serial monitor
    Parameters: none
    Returns: none
*/
/* OBS används inte längre
void printa()
{
  Serial.print("next_ball[] = [");
  for (int i = 0; i < amount_in_array; i++)
  {
    Serial.print(next_ball[i]);
    if (i < amount_in_array - 1)
    {
      Serial.print(", ");
    }
  }
  Serial.println("]");
}
*/

/*
    Adds a value to the end of the next_ball array
      updates the amount_in_array variable

    Parameters: gate_num (the gate the inspected ball is to go to)
    Returns: none
*/
void addball(int gate_num)
{
  if (amount_in_array < array_size)
  {
    next_ball[(amount_in_array)] = gate_num;
    amount_in_array++;
  }
  else
  {
    Serial.println("Array is full, can't add more elements.");
  }
  just_sorted = 0;
}

/*
    Removes the first value of the next_ball array
        Also updates the var: amount_in_array
    Parameters: none
    Returns: none
*/
void remove_first()
{
  if (amount_in_array > 0)
  {
    int temp_array[array_size]; // Temporary array with the same maximum size

    for (int i = 1; i < amount_in_array; i++)
    { // assign the same vaulues to the temporary array, skipping the first element
      temp_array[i - 1] = next_ball[i];
    }

    // Copy the values from the temporary array back to next_ball
    for (int i = 0; i < amount_in_array - 1; i++)
    {
      next_ball[i] = temp_array[i];
    }

    amount_in_array--;
  }
  else
  {
    Serial.println("Array is empty, can't remove the element");
  }
}

/*
This function opens the corresponding gate and waits for the golfball to pass though before closing the gate again.
  Then removes the first value in first_ball
Paramerters: x = the gate you want to open
returns: none
*/
void open_gate(int x)
{
  if (x == 1) // gate 1 "ProV1"
  {
    digitalWrite(2, HIGH);
    while (digitalRead(5) == HIGH)
    {
      update_just_sorted();

      if (just_sorted > 0)
      {
        addball(just_sorted);
      }
    }
    digitalWrite(2, LOW);
    sorted_prov1++;
  }
  else if (x == 2) // Gate 2 "nxt"
  {
    digitalWrite(3, HIGH);
    while (digitalRead(6) == HIGH)
    {
      update_just_sorted();
      if (just_sorted > 0)
      {
        addball(just_sorted);
      }
    }
    digitalWrite(3, LOW);
    sorted_nxt++;
  }
  else if (x == 3) // Gate 3 "nike"
  {
    digitalWrite(4, HIGH);
    while (digitalRead(7) == HIGH)
    {
      update_just_sorted();
      if (just_sorted > 0)
      {
        addball(just_sorted);
      }
    }
    digitalWrite(4, LOW);
    sorted_nike++;
  }
  remove_first();
}
