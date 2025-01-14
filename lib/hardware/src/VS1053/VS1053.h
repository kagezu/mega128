// #include <VS1053.h>
#pragma once
#include "config.h"
#include <spi/spi.h>




class VS1053 {
protected:
  SPI_Master *_spi;

public:
  VS1053(SPI_Master *spi) { _spi = spi; }
  void begin();
  void end();
  void init();


  void set_volume(uint8_t, uint8_t);
  void set_volume(uint16_t);
  void set_volume(uint8_t);
  uint16_t getTrebleFrequency();
  int8_t  getTrebleAmplitude();
  uint16_t getBassFrequency();
  int8_t getBassAmplitude();
  void setTrebleFrequency(uint16_t);
  void setTrebleAmplitude(int8_t);
  void setBassFrequency(uint16_t);
  void setBassAmplitude(uint8_t);
  void setPlaySpeed(uint16_t);
  uint16_t getPlaySpeed();
  uint16_t getVolume();
  uint8_t getEarSpeaker();
  void setEarSpeaker(uint16_t);
  uint16_t getMonoMode();
  void setMonoMode(uint16_t);
  void setDifferentialOutput(uint16_t);
  uint8_t getDifferentialOutput();
  uint8_t playTrack(uint8_t);
  uint8_t playMP3(char *, uint32_t timecode = 0);
  void trackTitle(char *);
  void trackArtist(char *);
  void trackAlbum(char *);
  void stopTrack();
  uint8_t isPlaying();
  uint8_t skip(int32_t);
  uint8_t skipTo(uint32_t);
  uint32_t currentPosition();
  void setBitRate(uint16_t);
  void pauseDataStream();
  void resumeDataStream();
  void pauseMusic();
  bool resumeMusic();
  uint8_t resumeMusic(uint32_t);
  static void available();
  void getAudioInfo();
  uint8_t enableTestSineWave(uint8_t);
  uint8_t disableTestSineWave();
  uint16_t memoryTest();
  uint8_t ADMixerLoad(char *);
  void ADMixerVol(int8_t);
  int8_t getVUmeter();
  int8_t setVUmeter(int8_t);
  int16_t getVUlevel();
  void SendSingleMIDInote();

private:
  static uint16_t read_register(uint8_t);

  static void refill();
  static void flush_cancel();
  static void spiInit();
  static void write_register(uint8_t, uint8_t, uint8_t);
  static void write_register(uint8_t, uint16_t);
  static uint16_t Mp3ReadWRAM(uint16_t);
  static void Mp3WriteWRAM(uint16_t, uint16_t);
  void getTrackInfo(uint8_t, char *);
  static void enableRefill();
  static void disableRefill();
  void getBitRateFromMP3File(char *);
  void  VSLoadUserCode(char *);



  /** \brief Rate of the SPI to be used with communicating to the VSdsp.*/
  static uint16_t spi_Read_Rate;
  static uint16_t spi_Write_Rate;

  /** \brief Buffer for moving data between Filehandle and VSdsp.*/
  static uint8_t mp3DataBuffer[32];

  /** \brief contains a local value of the beleived current bit-rate.*/
  uint8_t bitrate;

  /** \brief contains a filehandles offset to the begining of the current file.*/
  uint32_t start_of_music;

  /** \brief contains a local value of the VSdsp's master volume left channels*/
  uint8_t VolL;

  /** \brief contains a local value of the VSdsp's master volume Right channels*/
  uint8_t VolR;

  /**
   * \brief A handler for accessing nibbles of the SCI_BASS word.
   *
   * a union of the SCI_BASS value and of its nibbles for
   * Treble/Bass and Freq/Amp.
   */
  union sci_bass_m {

    /**
     * \brief whole word value
     *
     * allows access and handeling of whole uint16_t (aka word) value
     */
    uint16_t word;

    /**
     * \brief individual Nibbles
     *
     * allows access and handeling of individual nibble values
     */
    struct {
      uint8_t  Bass_Freqlimt : 4; // 0..3
      uint8_t  Bass_Amplitude : 4; // 4..7
      uint8_t  Treble_Freqlimt : 4; // 8..11
      int8_t  Treble_Amplitude : 4; // 12..15
    }nibble;
  };
};


union tbyte {
  uint16_t word;
  uint8_t  byte[2];
};
