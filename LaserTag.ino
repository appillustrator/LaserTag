/*
Last Updated By Elvis Wolcott 2/29/16

Note: In order to meet the deadline this does not implement IR codes because I did not have much time to type this code up.
It still works for FFA, but has to assume everyone deals the same amount of damage.
Currently this is unnoticable, and this is more of a note to self for when the LCD is added than anything else.
Currently functions or short or empty because their functionality is not yet needed.


Note: As I continue to improve this project I will add more game modes.
Also, an LCD display is coming soon so that I can display scores etc.
This will allow me to add the loadouts feature and better display health.
Once the LCD is added I will add the functionality to track kills,
for now I have ommited it to save space and time.
*/
#include "dotstar/dotstar.h" //download from adafruit's product page
#include <stdlib.h>
#include "Particle.h"

#define NUMPIXELS   60 // Number of LEDs in strip
#define PID 000000 //you need to make this unique for every player

//out of the box colors for you to use if you want:
#define GREEN 0x0000FF  //If these don't show up correctly it means your LED strip is configured to recieve colors in a different format
#define RED 0xFF0000
#define BLUE 0x00FF00

#define IRLED_PIN D2
#define MOTOR_PIN A4
#define RELOAD_PIN D0
#define FIRE_PIN D1
#define SENSOR_PIN D3

#define RELOAD_CLR 0x000000 //the color you want it to be while reloading (everyone should agree on this to avoid confusion)
#define FIRE_CLR 0x000000 //the color you want it to be while fireing (everyone should agree on this to avoid confusion)
#define DEAD_CLR 0x000000 //the color you want it to be while dead (everyone should agree on this to avoid confusion)
#define TEAM_CLR 0x000000 //the color you want it to normally be (don't share with someone until team gamemode is added)

#define FULL_AMMO 100
#define FULL_HEALTH 50
#define FULL_CLIP 10

#define DMG  10 //later this will be replaced by a funtion to fetch damage value from the cloud

int reserve, clip, health;
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, A5, A3);
void reload() {
  displayColor(RELOAD_CLR);
  reserve -= (FULL_CLIP - clip);
  if(reserve > 0) {
    clip = FULL_CLIP;
  } else {
    clip = (0 - reserve);
  }
  delay(200000) //2 sec reload
  displayColor(TEAM_CLR);
}

void takeDMG() {
  health -= DMG;
}

void shoot() {
  if(clip > 0) {
    displayColor(FIRE_CLR);
    digitalWrite(IRLED_PIN, HIGH);
    delay(100); //.sec fire time
    digitalWrite(IRLED_PIN, LOW);
    displayColor(TEAM_CLR);
  }
}

void died() {
  displayColor(DEAD_CLR);
  Particle.publish("I died!") //later this will send meaningful data
  delay(1000000); //10 sec respawn
  respawn();
  displayColor(TEAM_CLR);
}

void displayColor(int color){ //turn all the LEDs one color
  for(int i = 0;i < NUMPIXELS; i++ ){
    strip.setPixelColor(i, color);
  }
  strip.show();
}
void respawn(){
  clip = FULL_CLIP;
  reserve = FULL_AMMO;
  health = FULL_HEALTH;
}
void setup() {
  strip.begin();
  strip.show();
  pinMode(IRLED_PIN, OUTPUT);
  pinMode(RELOAD_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(FIRE_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);
}

void loop() {
  if(SENSOR_PIN){
    takeDMG();
    delay(100); //one shot won't register twice
  }
  if(RELOAD_PIN){
    reaload();
  }
}
