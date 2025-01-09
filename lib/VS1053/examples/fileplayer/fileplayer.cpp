#include <display.h>
#include "font/arial_14.h"
#include "font/micro_5x6.h"
#include "font/standard_5x8.h"

#include <SPI.h>

//Add the SdFat Libraries
#include <SdFat.h>
#include <FreeStack.h>

//and the MP3 Shield Library
#include <vs1053_SdFat.h>

// Below is not needed if interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_Timer1
#include <TimerOne.h>
#elif defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer
#include <SimpleTimer.h>
#endif

Display lcd;
SdFat sd;
vs1053 MP3player;
int16_t last_ms_char; // milliseconds of last recieved character from Serial port.
int8_t buffer_pos; // next position to recieve character from Serial port.


char buffer[6]; // 0-35K+null

uint32_t  millis_prv;

//------------------------------------------------------------------------------
/**
 * \brief Print Help Menu.
 *
 * Prints a full menu of the commands available along with descriptions.
 */
void help()
{
  lcd.println(F("\f[1-9] to play a track"));
  lcd.println(F("[s] to stop playing"));
  lcd.println(F("[d] display directory of SdCard"));
  lcd.println(F("[+ or -] volume"));
  lcd.println(F("[> or <] play speed"));
  lcd.println(F("[p] to pause."));
  lcd.println(F("[B] Inc bass 10Hz"));
  lcd.println(F("[C] Inc bass 1dB"));
  lcd.println(F("[T] Inc treble 1000Hz"));
  lcd.println(F("[E] Inc treble 1dB"));
  lcd.println(F("[l] Display list of music files"));
  lcd.println(F("[h] this help"));
}

void SerialPrintPaddedNumber(int16_t value, int8_t digits)
{
  int currentMax = 10;
  for (byte i = 1; i < digits; i++) {
    if (value < currentMax) {
      lcd.print("0");
    }
    currentMax *= 10;
  }
  lcd.print(value);
}

//------------------------------------------------------------------------------
/**
 * \brief Decode the Menu.
 *
 * Parses through the characters of the users input, executing corresponding
 * MP3player library functions and features then displaying a brief menu and
 * prompting for next input command.
 */
void parse_menu(byte key_command)
{

  uint8_t result; // result code from some function as to be tested at later time.

  // Note these buffer may be desired to exist globably.
  // but do take much space if only needed temporarily, hence they are here.
  char title[30]; // buffer to contain the extract the Title from the current filehandles
  char artist[30]; // buffer to contain the extract the artist name from the current filehandles
  char album[30]; // buffer to contain the extract the album name from the current filehandles

  lcd.print(F("Received command: "));
  lcd.write(key_command);
  lcd.println(F(" "));

  //if s, stop the current track
  if (key_command == 's') {
    lcd.println(F("Stopping"));
    MP3player.stopTrack();

    //if 1-9, play corresponding track
  }
  else if (key_command >= '1' && key_command <= '9') {
    //convert ascii numbers to real numbers
    key_command = key_command - 48;

  #if USE_MULTIPLE_CARDS
    sd.chvol(); // assign desired sdcard's volume.
  #endif
    //tell the MP3 Shield to play a track
    result = MP3player.playTrack(key_command);

    //check result, see readme for error codes.
    if (result != 0) {
      lcd.print(F("Error code: "));
      lcd.print(result);
      lcd.println(F(" when trying to play track"));
    }
    else {

      lcd.println(F("Playing:"));

      //we can get track info by using the following functions and arguments
      //the functions will extract the requested information, and put it in the array we pass in
      MP3player.trackTitle((char *)&title);
      MP3player.trackArtist((char *)&artist);
      MP3player.trackAlbum((char *)&album);

      //print out the arrays of track information
      lcd.write((byte *)&title, 30);
      lcd.printf(F("\n"));
      lcd.print(F("by:  "));
      lcd.write((byte *)&artist, 30);
      lcd.printf(F("\n"));
    }

    //if +/- to change volume
  }
  else if ((key_command == '-') || (key_command == '+')) {
    union twobyte mp3_vol; // create key_command existing variable that can be both word and double byte of left and right.
    mp3_vol.word = MP3player.getVolume(); // returns a double uint8_t of Left and Right packed into int16_t

    if (key_command == '-') { // note dB is negative
      // assume equal balance and use byte[1] for math
      if (mp3_vol.byte[1] >= 254) { // range check
        mp3_vol.byte[1] = 254;
      }
      else {
        mp3_vol.byte[1] += 2; // keep it simpler with whole dB's
      }
    }
    else {
      if (mp3_vol.byte[1] <= 2) { // range check
        mp3_vol.byte[1] = 2;
      }
      else {
        mp3_vol.byte[1] -= 2;
      }
    }
    // push byte[1] into both left and right assuming equal balance.
    MP3player.setVolume(mp3_vol.byte[1], mp3_vol.byte[1]); // commit new volume
    lcd.print(F("Volume changed to -"));
    lcd.print(mp3_vol.byte[1] >> 1);
    lcd.println(F("[dB]"));

    //if < or > to change Play Speed
  }
  else if ((key_command == '>') || (key_command == '<')) {
    uint16_t playspeed = MP3player.getPlaySpeed(); // create key_command existing variable
    // note playspeed of Zero is equal to ONE, normal speed.
    if (key_command == '>') { // note dB is negative
      // assume equal balance and use byte[1] for math
      if (playspeed >= 254) { // range check
        playspeed = 5;
      }
      else {
        playspeed += 1; // keep it simpler with whole dB's
      }
    }
    else {
      if (playspeed == 0) { // range check
        playspeed = 0;
      }
      else {
        playspeed -= 1;
      }
    }
    MP3player.setPlaySpeed(playspeed); // commit new playspeed
    lcd.print(F("playspeed to "));
    lcd.println(playspeed);

    /* Alterativly, you could call a track by it's file name by using playMP3(filename);
    But you must stick to 8.1 filenames, only 8 characters long, and 3 for the extension */
  }
  else if (key_command == 'f' || key_command == 'F') {
    uint32_t offset = 0;
    if (key_command == 'F') {
      offset = 2000;
    }

    //create a string with the filename
    char trackName[] = "track001.mp3";

  #if USE_MULTIPLE_CARDS
    sd.chvol(); // assign desired sdcard's volume.
  #endif
    //tell the MP3 Shield to play that file
    result = MP3player.playMP3(trackName, offset);
    //check result, see readme for error codes.
    if (result != 0) {
      lcd.print(F("Error code: "));
      lcd.print(result);
      lcd.println(F(" when trying to play track"));
    }

    /* Display the file on the SdCard */
  }
  else if (key_command == 'd') {
    if (!MP3player.isPlaying()) {
      // prevent root.ls when playing, something locks the dump. but keeps playing.
      // yes, I have tried another unique instance with same results.
      // something about SdFat and its 500byte cache.
      lcd.println(F("Files found (name date time size):"));
      sd.ls(LS_R | LS_DATE | LS_SIZE);
    }
    else {
      lcd.println(F("Busy Playing Files, try again later."));
    }

    /* Get and Display the Audio Information */
  }
  else if (key_command == 'i') {
    MP3player.getAudioInfo();

  }
  else if (key_command == 'p') {
    if (MP3player.getState() == playback) {
      MP3player.pauseMusic();
      lcd.println(F("Pausing"));
    }
    else if (MP3player.getState() == paused_playback) {
      MP3player.resumeMusic();
      lcd.println(F("Resuming"));
    }
    else {
      lcd.println(F("Not Playing!"));
    }

  }
  else if (key_command == 't') {
    int8_t teststate = MP3player.enableTestSineWave(126);
    if (teststate == -1) {
      lcd.println(F("Un-Available while playing music or chip in reset."));
    }
    else if (teststate == 1) {
      lcd.println(F("Enabling Test Sine Wave"));
    }
    else if (teststate == 2) {
      MP3player.disableTestSineWave();
      lcd.println(F("Disabling Test Sine Wave"));
    }

  }
  else if (key_command == 'S') {
    lcd.println(F("Current State of VS10xx is."));
    lcd.print(F("isPlaying() = "));
    lcd.println(MP3player.isPlaying());

    lcd.print(F("getState() = "));
    switch (MP3player.getState()) {
      case uninitialized:
        lcd.print(F("uninitialized"));
        break;
      case initialized:
        lcd.print(F("initialized"));
        break;
      case deactivated:
        lcd.print(F("deactivated"));
        break;
      case loading:
        lcd.print(F("loading"));
        break;
      case ready:
        lcd.print(F("ready"));
        break;
      case playback:
        lcd.print(F("playback"));
        break;
      case paused_playback:
        lcd.print(F("paused_playback"));
        break;
      case testing_memory:
        lcd.print(F("testing_memory"));
        break;
      case testing_sinewave:
        lcd.print(F("testing_sinewave"));
        break;
      default:;
    }
    lcd.printf(F("\n"));

  }
  // else if (key_command == 'b') {
  //   lcd.println(F("Playing Static MIDI file."));
  //   MP3player.SendSingleMIDInote();
  //   lcd.println(F("Ended Static MIDI file."));

#if !defined(__AVR_ATmega32U4__)
// }
// else if (key_command == 'm') {
//   uint16_t teststate = MP3player.memoryTest();
//   if (teststate == (uint16_t)-1) {
//     lcd.println(F("Un-Available while playing music or chip in reset."));
//   }
  // else if (teststate == 2) {
  //   teststate = MP3player.disableTestSineWave();
  //   lcd.println(F("Un-Available while Sine Wave Test"));
  // }
  // else {
  //   lcd.print(F("Memory Test Results = "));
  //   lcd.println(teststate, HEX);
  //   lcd.println(F("Result should be 0x83FF."));
  //   lcd.println(F("Reset is needed to recover to normal operation"));
  // }

// }
  else if (key_command == 'e') {
    uint8_t earspeaker = MP3player.getEarSpeaker();
    if (earspeaker >= 3) {
      earspeaker = 0;
    }
    else {
      earspeaker++;
    }
    MP3player.setEarSpeaker(earspeaker); // commit new earspeaker
    lcd.print(F("earspeaker to "));
    lcd.println(earspeaker);

  }
  else if (key_command == 'r') {
    MP3player.resumeMusic(2000);

  }
  else if (key_command == 'R') {
    MP3player.stopTrack();
    MP3player.vs_init();
    lcd.println(F("Reseting VS10xx chip"));

  }
  else if (key_command == 'g') {
    int32_t offset_ms = 20000; // Note this is just an example, try your own number.
    lcd.print(F("jumping to "));
    lcd.print(offset_ms);
    lcd.println(F("[milliseconds]"));
    result = MP3player.skipTo(offset_ms);
    if (result != 0) {
      lcd.print(F("Error code: "));
      lcd.print(result);
      lcd.println(F(" when trying to skip track"));
    }

  }
  else if (key_command == 'k') {
    int32_t offset_ms = -1000; // Note this is just an example, try your own number.
    lcd.print(F("moving = "));
    lcd.print(offset_ms);
    lcd.println(F("[milliseconds]"));
    result = MP3player.skip(offset_ms);
    if (result != 0) {
      lcd.print(F("Error code: "));
      lcd.print(result);
      lcd.println(F(" when trying to skip track"));
    }

  }
  // else if (key_command == 'O') {
  //   MP3player.end();
  //   lcd.println(F("VS10xx placed into low power reset mode."));

  // }
  // else if (key_command == 'o') {
  //   MP3player.begin();
  //   lcd.println(F("VS10xx restored from low power reset mode."));

  // }
  // else if (key_command == 'D') {
  //   uint16_t diff_state = MP3player.getDifferentialOutput();
  //   lcd.print(F("Differential Mode "));
  //   if (diff_state == 0) {
  //     MP3player.setDifferentialOutput(1);
  //     lcd.println(F("Enabled."));
  //   }
  //   else {
  //     MP3player.setDifferentialOutput(0);
  //     lcd.println(F("Disabled."));
  //   }

  // }
  else if (key_command == 'V') {
    MP3player.setVUmeter(1);
    lcd.println(F("Use \"No line ending\""));
    lcd.print(F("VU meter = "));
    lcd.println(MP3player.getVUmeter());
    lcd.println(F("Hit Any key to stop."));

    while (!Serial.available()) {
      union twobyte vu;
      vu.word = MP3player.getVUlevel();
      lcd.print(F("VU: L = "));
      lcd.print(vu.byte[1]);
      lcd.print(F(" / R = "));
      lcd.print(vu.byte[0]);
      lcd.println(" dB");
      delay(1000);
    }
    Serial.read();

    MP3player.setVUmeter(0);
    lcd.print(F("VU meter = "));
    lcd.println(MP3player.getVUmeter());

  }
  else if (key_command == 'T') {
    uint16_t TrebleFrequency = MP3player.getTrebleFrequency();
    lcd.print(F("Former TrebleFrequency = "));
    lcd.println(TrebleFrequency);
    if (TrebleFrequency >= 15000) { // Range is from 0 - 1500Hz
      TrebleFrequency = 0;
    }
    else {
      TrebleFrequency += 1000;
    }
    MP3player.setTrebleFrequency(TrebleFrequency);
    lcd.print(F("New TrebleFrequency = "));
    lcd.println(MP3player.getTrebleFrequency());

  }
  else if (key_command == 'E') {
    int8_t TrebleAmplitude = MP3player.getTrebleAmplitude();
    lcd.print(F("Former TrebleAmplitude = "));
    lcd.println(TrebleAmplitude);
    if (TrebleAmplitude >= 7) { // Range is from -8 - 7dB
      TrebleAmplitude = -8;
    }
    else {
      TrebleAmplitude++;
    }
    MP3player.setTrebleAmplitude(TrebleAmplitude);
    lcd.print(F("New TrebleAmplitude = "));
    lcd.println(MP3player.getTrebleAmplitude());

  }
  else if (key_command == 'B') {
    uint16_t BassFrequency = MP3player.getBassFrequency();
    lcd.print(F("Former BassFrequency = "));
    lcd.println(BassFrequency);
    if (BassFrequency >= 150) { // Range is from 20hz - 150hz
      BassFrequency = 0;
    }
    else {
      BassFrequency += 10;
    }
    MP3player.setBassFrequency(BassFrequency);
    lcd.print(F("New BassFrequency = "));
    lcd.println(MP3player.getBassFrequency());

  }
  else if (key_command == 'C') {
    uint16_t BassAmplitude = MP3player.getBassAmplitude();
    lcd.print(F("Former BassAmplitude = "));
    lcd.println(BassAmplitude);
    if (BassAmplitude >= 15) { // Range is from 0 - 15dB
      BassAmplitude = 0;
    }
    else {
      BassAmplitude++;
    }
    MP3player.setBassAmplitude(BassAmplitude);
    lcd.print(F("New BassAmplitude = "));
    lcd.println(MP3player.getBassAmplitude());

  }
  else if (key_command == 'M') {
    uint16_t monostate = MP3player.getMonoMode();
    lcd.print(F("Mono Mode "));
    if (monostate == 0) {
      MP3player.setMonoMode(1);
      lcd.println(F("Enabled."));
    }
    else {
      MP3player.setMonoMode(0);
      lcd.println(F("Disabled."));
    }
  #endif

    /* List out music files on the SdCard */
  }
  else if (key_command == 'l') {
    if (!MP3player.isPlaying()) {
      lcd.println(F("\fMusic Files found :"));
      SdFile file;
      char filename[13];
      sd.chdir("/");
      uint16_t count = 1;
      while (file.openNext(sd.vwd(), O_READ)) {
        file.getName(filename, sizeof(filename));
        if (isFnMusic(filename)) {
          SerialPrintPaddedNumber(count, 5);
          lcd.print(F(": "));
          lcd.println(filename);
          count++;
        }
        file.close();
      }
      lcd.println(F("Enter Index of File to play"));

    }
    else {
      lcd.println(F("Busy Playing Files, try again later."));
    }

  }
  else if (key_command == 'h') {
    help();
  }
}

//------------------------------------------------------------------------------
/**
 * \brief Main Loop the Arduino Chip
 *
 * This is called at the end of Arduino kernel's main loop before recycling.
 * And is where the user's serial input of bytes are read and analyzed by
 * parsed_menu.
 *
 * Additionally, if the means of refilling is not interrupt based then the
 * MP3player object is serviced with the availaible function.
 *
 * \note Actual examples of the libraries public functions are implemented in
 * the parse_menu() function.
 */
void loop()
{

  // Below is only needed if not interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) \
    && ( (USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer) \
    ||   (USE_MP3_REFILL_MEANS == USE_MP3_Polled)      )

  MP3player.available();
#endif

  char inByte;
  if (Serial.available() > 0) {
    inByte = Serial.read();
    if ((0x20 <= inByte) && (inByte <= 0x126)) { // strip off non-ASCII, such as CR or LF
      if (isDigit(inByte)) { // macro for ((inByte >= '0') && (inByte <= '9'))
        // else if it is a number, add it to the string
        buffer[buffer_pos++] = inByte;
      }
      else {
        // input char is a letter command
        buffer_pos = 0;
        parse_menu(inByte);
      }
      buffer[buffer_pos] = 0; // update end of line
      last_ms_char = millis(); // stroke the inter character timeout.
    }
  }
  else if ((millis() - last_ms_char) > 500 && (buffer_pos > 0)) {
    // ICT expired and have something
    if (buffer_pos == 1) {
      // look for single byte (non-number) menu commands
      parse_menu(buffer[buffer_pos - 1]);

    }
    else if (buffer_pos > 5) {
      // dump if entered command is greater then uint16_t
      lcd.println(F("Ignored, Number is Too Big!"));

    }
    else {
      // otherwise its a number, scan through files looking for matching index.
      int16_t fn_index = atoi(buffer);
      SdFile file;
      char filename[13];
      sd.chdir("/");
      uint16_t count = 1;
      while (file.openNext(sd.vwd(), O_READ)) {
        file.getName(filename, sizeof(filename));
        if (isFnMusic(filename)) {

          if (count == (uint16_t)fn_index) {
            lcd.print(F("Index "));
            SerialPrintPaddedNumber(count, 5);
            lcd.print(F(": "));
            lcd.println(filename);
            lcd.print(F("Playing filename: "));
            lcd.println(filename);
            int8_t result = MP3player.playMP3(filename);
            //check result, see readme for error codes.
            if (result != 0) {
              lcd.print(F("Error code: "));
              lcd.print(result);
              lcd.println(F(" when trying to play track"));
            }
            char title[30]; // buffer to contain the extract the Title from the current filehandles
            char artist[30]; // buffer to contain the extract the artist name from the current filehandles
            char album[30]; // buffer to contain the extract the album name from the current filehandles
            MP3player.trackTitle((char *)&title);
            MP3player.trackArtist((char *)&artist);
            MP3player.trackAlbum((char *)&album);

            //print out the arrays of track information
            lcd.write((byte *)&title, 30);
            lcd.printf(F("\n"));
            lcd.print(F("by:  "));
            lcd.write((byte *)&artist, 30);
            lcd.printf(F("\n"));
            break;
          }
          count++;
        }
        file.close();
      }

    }

    //reset buffer to start over
    buffer_pos = 0;
    buffer[buffer_pos] = 0; // delimit
  }

  delay(100);
}


void setup()
{

  RGB background(0x04, 0x00, 0x40);
  lcd.clear(background);
  lcd.background(background);
  lcd.color(RGB(0xff, 0xff, 0x04));

  lcd.font(&micro_5x6);
  // lcd.font(&standard_5x8);
  // lcd.font(&number_8x16);
  // lcd.font(&number_15x31);
  // lcd.font(&arial_14);
  lcd.set_interline(2);
  uint8_t result; //result code from some function as to be tested at later time.

  Serial.begin(9600);

  lcd.print(F("F_CPU = "));
  lcd.println(F_CPU);
  lcd.print(F("Free RAM = ")); // available in Version 1.0 F() bases the string to into Flash, to use less SRAM.
  lcd.println(FreeStack());  // FreeRam() is provided by SdFatUtil.h


  //Initialize the SdCard.
  if (!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  // depending upon your SdCard environment, SPI_HAVE_SPEED may work better.
  if (!sd.chdir("/")) sd.errorHalt("sd.chdir");

  //Initialize the MP3 Player Shield
  result = MP3player.begin();
  //check result, see readme for error codes.
  if (result != 0) {
    lcd.print(F("Error code: "));
    lcd.print(result);
    lcd.println(F(" when trying to start MP3 player"));
    if (result == 6) {
      lcd.println(F("Warning: patch file not found, skipping.")); // can be removed for space, if needed.
      lcd.println(F("Use the \"d\" command to verify SdCard can be read")); // can be removed for space, if needed.
    }
  }

#if (0)
  // Typically not used by most shields, hence commented out.
  lcd.println(F("Applying ADMixer patch."));
  if (MP3player.ADMixerLoad("admxster.053") == 0) {
    lcd.println(F("Setting ADMixer Volume."));
    MP3player.ADMixerVol(-3);
  }
#endif

  help();
  last_ms_char = millis(); // stroke the inter character timeout.
  buffer_pos = 0; // start the command string at zero length.
  parse_menu('l'); // display the list of files to play

}
