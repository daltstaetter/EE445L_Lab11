// filename ******** fixed.h ************** 
// possible header file for Lab 1 

//  Valvano
// September 4, 2012

/* 
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011

 Copyright 2011 by Jonathan W. Valvano, valvano@mail.utexas.edu
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Option 1 (have a board, will debug with OLED output). Lab1.c is not real useful for this approach
void Fixed_uDecOut2(unsigned long n);
void Fixed_sDecOut3(long n);
void Fixed_uBinOut8(unsigned long n); 

Option 2 (do not have board and will test in the simulator). See lab1.c for an example of how to test
void Fixed_uDecOut2s(unsigned long n,  char *string);
void Fixed_sDecOut3s(long n, char *string);
void Fixed_uBinOut8s(unsigned long n,  char *string); 

The difference in the options will be option 1 uses printf, and option 2 uses sprintf. Both have the OLED as the starter project.
 */


/****************Fixed_uDecOut2s***************
 converts fixed point number to ASCII string
 format unsigned 32-bit with resolution 0.01
 range 0 to 999.99
 Input: unsigned 32-bit integer part of fixed point number
         greater than 99999 means invalid fixed-point number
 Output: null-terminated string exactly 6 characters plus null
 Examples
 12345 to "123.45"  
 22100 to "221.00"
   102 to "  1.02" 
    31 to "  0.31" 
100000 to "***.**"    */ 
void Fixed_uDecOut2s(unsigned long n,  char *string);

 /****************Fixed_uDecOut2***************
 outputs the fixed-point value on the OLED
 format unsigned 32-bit with resolution 0.01
 range 0 to 999.99
 Input: unsigned 32-bit integer part of fixed point number
         greater than 99999 means invalid fixed-point number
 Output: none
 Examples
 12345 to "123.45"  
 22100 to "221.00"
   102 to "  1.02" 
    31 to "  0.31" 
100000 to "***.**"    */ 
void Fixed_uDecOut2(unsigned long n);



/****************Fixed_sDecOut3s***************
 converts fixed point number to ASCII string
 format signed 32-bit with resolution 0.001
 range -9.999 to +9.999
 Input: signed 32-bit integer part of fixed point number
 Output: null-terminated string exactly 6 characters plus null
 Examples
  2345 to " 2.345"  
 -8100 to "-8.100"
  -102 to "-0.102" 
    31 to " 0.031" 
   
 */ 
void Fixed_sDecOut3s(long n, char *string);

/****************Fixed_sDecOut3***************
 converts fixed point number to OLED
 format signed 32-bit with resolution 0.001
 range -9.999 to +9.999
 Input: signed 32-bit integer part of fixed point number
 Output: none
 OLED has exactly 6 characters
 Examples
  2345 to " 2.345"  
 -8100 to "-8.100"
  -102 to "-0.102" 
    31 to " 0.031" 
 */ 
void Fixed_sDecOut3(long n);



/**************Fixed_uBinOut8s***************
 unsigned 32-bit binary fixed-point with a resolution of 1/256. 
 The full-scale range is from 0 to 999.99. 
 If the integer part is larger than 256000, it signifies an error. 
 The Fixed_uBinOut8 function takes an unsigned 32-bit integer part 
 of the binary fixed-point number and outputs the fixed-point value on the OLED. 
 Input: unsigned 32-bit integer part of fixed point number
 Output: null-terminated string
Parameter output string
     0     "  0.00"
     2     "  0.01"
    64     "  0.25"
   100     "  0.39"
   50      "  1.95"
   512     "  2.00"
  5000     " 19.53"
 30000     "117.19"
255997     "999.99"
256000     "***.**"
*/
void Fixed_uBinOut8s(unsigned long n,  char *string); 

/**************Fixed_uBinOut8***************
 unsigned 32-bit binary fixed-point with a resolution of 1/256. 
 The full-scale range is from 0 to 999.99. 
 If the integer part is larger than 256000, it signifies an error. 
 The Fixed_uBinOut8 function takes an unsigned 32-bit integer part 
 of the binary fixed-point number and outputs the fixed-point value on the OLED. 
 Input: unsigned 32-bit integer part of fixed point number
 Output: none
Parameter OLED display
     0	  0.00
     2	  0.01
    64	  0.25
   100	  0.39
   500	  1.95
   512	  2.00
  5000	 19.53
 30000	117.19
255997	999.99
256000	***.**
*/
void Fixed_uBinOut8(unsigned long n); 
