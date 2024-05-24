This is the data from the jump test.

In the data we see a single int which indicates the jump nr.
Inbetween the jumps the data is printed multiple times.


Code added:
  In void loop:
    if (abs(measured_values[0])<0.3){
     Serial.print("Fa");
     Serial.println(measured_values[0]);
     Serial.print("Fv");
     Serial.println(measured_values[1]);
     Serial.println();
    }


  In jump.ino - in if statement where wheelspeed for brake is set:


    Serial.println(JumpNr);
    JumpNr++;

    Where:
	Jump is set to 0 in the top of "jump.ino"