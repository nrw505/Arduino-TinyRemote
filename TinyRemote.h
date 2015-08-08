/*
 * IRremote
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.htm http://arcfn.com
 * Edited by Mitra to add new controller SANYO
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 *
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
* LG added by Darryl Smith (based on the JVC protocol)
 */

#ifndef IRremote_h
#define IRremote_h

#define TINY_IR_RECV 1
#define TINY_PROTO_NEC 1

// The following are compile-time library options.
// If you change them, recompile the library.
// If DEBUG is defined, a lot of debugging output will be printed during decoding.
// TEST must be defined for the IRtest unittests to work.  It will make some
// methods virtual, which will be slightly slower, which is why it is optional.
// #define DEBUG
// #define TEST

// Results returned from the decoder
class decode_results {
public:
  int decode_type; // NEC, SONY, RC5, UNKNOWN
  union { // This is used for decoding Panasonic and Sharp data
    unsigned int panasonicAddress;
    unsigned int sharpAddress;
  };
  unsigned long value; // Decoded value
  int bits; // Number of bits in decoded value
  volatile unsigned int *rawbuf; // Raw intervals in .5 us ticks
  int rawlen; // Number of records in rawbuf.
};

// Values for decode_type
#define NEC 1
#define SONY 2
#define RC5 3
#define RC6 4
#define DISH 5
#define SHARP 6
#define PANASONIC 7
#define JVC 8
#define SANYO 9
#define MITSUBISHI 10
#define SAMSUNG 11
#define LG 12
#define UNKNOWN -1

// Decoded value for NEC when a repeat code is received
#define REPEAT 0xffffffff

#if TINY_IR_RECV
// main class for receiving IR
class IRrecv
{
public:
  IRrecv(int recvpin);
  void blink13(int blinkflag);
  int decode(decode_results *results);
  void enableIRIn();
  void resume();
private:
  // These are called by decode
#if TINY_PROTO_RC5 || TINY_PROTO_RC6
  int getRClevel(decode_results *results, int *offset, int *used, int t1);
#endif
#if TINY_PROTO_NEC
  long decodeNEC(decode_results *results);
#endif
#if TINY_PROTO_SONY
  long decodeSony(decode_results *results);
#endif
#if TINY_PROTO_SANYO
  long decodeSanyo(decode_results *results);
#endif
#if TINY_PROTO_MITSUBISHI
  long decodeMitsubishi(decode_results *results);
#endif
#if TINY_PROTO_RC5
  long decodeRC5(decode_results *results);
#endif
#if TINY_PROTO_RC6
  long decodeRC6(decode_results *results);
#endif
#if TINY_PROTO_PANASONIC
  long decodePanasonic(decode_results *results);
#endif
#if TINY_PROTO_LG
  long decodeLG(decode_results *results);
#endif
#if TINY_PROTO_JVC
  long decodeJVC(decode_results *results);
#endif
#if TINY_PROTO_SAMSUNG
  long decodeSAMSUNG(decode_results *results);
#endif
#if TINY_PROTO_SONY
  long decodeHash(decode_results *results);
  int compare(unsigned int oldval, unsigned int newval);
#endif

} 
;
#endif

// Only used for testing; can remove virtual for shorter code
#ifdef TEST
#define VIRTUAL virtual
#else
#define VIRTUAL
#endif

#if TINY_IR_SEND
class IRsend
{
public:
  IRsend() {}
#if TINY_PROTO_NEC
  void sendNEC(unsigned long data, int nbits);
#endif
#if TINY_PROTO_SONY
  void sendSony(unsigned long data, int nbits);
  // Neither Sanyo nor Mitsubishi send is implemented yet
  //  void sendSanyo(unsigned long data, int nbits);
  //  void sendMitsubishi(unsigned long data, int nbits);
#endif
#if TINY_PROTO_RAW
  void sendRaw(unsigned int buf[], int len, int hz);
#endif
#if TINY_PROTO_RC5
  void sendRC5(unsigned long data, int nbits);
#endif
#if TINY_PROTO_RC6
  void sendRC6(unsigned long data, int nbits);
#endif
#if TINY_PROTO_DISH
  void sendDISH(unsigned long data, int nbits);
#endif
#if TINY_PROTO_SHARP
  void sendSharp(unsigned int address, unsigned int command);
  void sendSharpRaw(unsigned long data, int nbits);
#endif
#if TINY_PROTO_PANASONIC
  void sendPanasonic(unsigned int address, unsigned long data);
#endif
#if TINY_PROTO_JVC
  void sendJVC(unsigned long data, int nbits, int repeat); // *Note instead of sending the REPEAT constant if you want the JVC repeat signal sent, send the original code value and change the repeat argument from 0 to 1. JVC protocol repeats by skipping the header NOT by sending a separate code value like NEC does.
  // private:
#endif
#if TINY_PROTO_SAMSUNG
  void sendSAMSUNG(unsigned long data, int nbits);
#endif
  void enableIROut(int khz);
  VIRTUAL void mark(int usec);
  VIRTUAL void space(int usec);
}
;

#endif


// Some useful constants

#define USECPERTICK 50  // microseconds per clock interrupt tick
#define RAWBUF 100 // Length of raw duration buffer

// Marks tend to be 100us too long, and spaces 100us too short
// when received due to sensor lag.
#define MARK_EXCESS 100

#endif
