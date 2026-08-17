#include <Arduino.h>
#include "HT1621.h"
#include <cstring>

HT1621::HT1621(){
	_buffer[0] = 0x00;
	_buffer[2] = 0x00;
	_buffer[4] = 0x00;
	_buffer[6] = 0x00;
}

void HT1621::begin(int cs_p, int wr_p, int data_p, int backlight_p)
{
	pinMode(cs_p, OUTPUT);
	pinMode(wr_p, OUTPUT);
	pinMode(data_p, OUTPUT);
	pinMode(backlight_p, OUTPUT);

	_cs_p = cs_p;
	_wr_p = wr_p;
	_data_p = data_p;
	_backlight_p = backlight_p;
	_backlight_en = true;

	config();
}

void HT1621::begin(int cs_p, int wr_p, int data_p)
{
	pinMode(cs_p, OUTPUT);
	pinMode(wr_p, OUTPUT);
	pinMode(data_p, OUTPUT);

	_cs_p = cs_p;
	_wr_p = wr_p;
	_data_p = data_p;
	_backlight_en = false;

	config();
}

void HT1621::wrDATA(unsigned char data, unsigned char cnt)
{
	unsigned char i;

	for(i = 0; i < cnt; i++)
	{
		digitalWrite(_wr_p, LOW);
		delayMicroseconds(4);

		if(data & 0x80)
			digitalWrite(_data_p, HIGH);
		else
			digitalWrite(_data_p, LOW);

		digitalWrite(_wr_p, HIGH);
		delayMicroseconds(4);

		data <<= 1;
	}
}

void HT1621::wrclrdata(unsigned char addr, unsigned char sdata)
{
	addr <<= 2;

	digitalWrite(_cs_p, LOW);

	wrDATA(0xa0, 3);
	wrDATA(addr, 6);
	wrDATA(sdata, 8);

	digitalWrite(_cs_p, HIGH);
}

void HT1621::display()
{
	wrCMD(LCDON);
}

void HT1621::noDisplay()
{
	wrCMD(LCDOFF);
}

void HT1621::wrone(unsigned char addr, unsigned char sdata)
{
	addr <<= 2;

	digitalWrite(_cs_p, LOW);

	wrDATA(0xa0, 3);
	wrDATA(addr, 6);
	wrDATA(sdata, 8);

	digitalWrite(_cs_p, HIGH);
}

void HT1621::backlight()
{
	if(_backlight_en)
		digitalWrite(_backlight_p, HIGH);

	delay(1);
}

void HT1621::noBacklight()
{
	if(_backlight_en)
		digitalWrite(_backlight_p, LOW);

	delay(1);
}

void HT1621::wrCMD(unsigned char CMD)
{
	digitalWrite(_cs_p, LOW);

	wrDATA(0x80, 4);
	wrDATA(CMD, 8);

	digitalWrite(_cs_p, HIGH);
}

void HT1621::config()
{
	wrCMD(BIAS);
	wrCMD(RC256);
	wrCMD(SYSDIS);
	wrCMD(WDTDIS1);
	wrCMD(SYSEN);
	wrCMD(LCDON);
}

void HT1621::wrCLR(unsigned char len)
{
	unsigned char addr = 0;

	for(unsigned char i = 0; i < len; i++)
	{
		wrclrdata(addr, 0x00);
		addr += 2;
	}
}

void HT1621::clear()
{
	wrCLR(6);
}


// rewrite this function for your LCD mapping
void HT1621::update()
{
	wrone(0, _buffer[0]);
	wrone(2, _buffer[2]);
	wrone(4, _buffer[4]);
	wrone(6, _buffer[6]);
}
void HT1621::setSegmentbinary(int addr, int bit, bool state)
{
    if(state)
        _buffer[addr] |= (1 << bit);
    else
        _buffer[addr] &= ~(1 << bit);
}
void HT1621::setSegmentInDigit(int digit, char segment, bool state)
{
    switch(digit)
    {
        case 1:
            switch(segment)
            {
                case 'a':
                    setSegmentbinary(6, 7, state);
                    break;
                case 'b':
                    setSegmentbinary(6, 6, state);
                    break;
                case 'c':
                    setSegmentbinary(6, 5, state);
                    break;
                case 'd':
                    setSegmentbinary(6, 4, state);
                    break;
                case 'e':
                    setSegmentbinary(4, 0, state);
                    break;
                case 'f':
                    setSegmentbinary(4, 2, state);
                    break;
                case 'g':
                    setSegmentbinary(4, 1, state);
                    break;
            }
            break;


        case 2:
            switch(segment)
            {
                case 'a':
                    setSegmentbinary(4, 7, state);
                    break;
                case 'b':
                    setSegmentbinary(4, 6, state);
                    break;
                case 'c':
                    setSegmentbinary(4, 5, state);
                    break;
                case 'd':
                    setSegmentbinary(4, 4, state);
                    break;
                case 'e':
                    setSegmentbinary(2, 0, state);
                    break;
                case 'f':
                    setSegmentbinary(2, 2, state);
                    break;
                case 'g':
                    setSegmentbinary(2, 1, state);
                    break;
            }
            break;


        case 3:
            switch(segment)
            {
                case 'a':
                    setSegmentbinary(2, 7, state);
                    break;
                case 'b':
                    setSegmentbinary(2,6, state);
                    break;
                case 'c':
                    setSegmentbinary(2, 5, state);
                    break;
                case 'd':
                    setSegmentbinary(2, 4, state);
                    break;
                case 'e':
                    setSegmentbinary(0,0, state);
                    break;
                case 'f':
                    setSegmentbinary(0, 2, state);
                    break;
                case 'g':
                    setSegmentbinary(0, 1, state);
                    break;
            }
            break;
    }
}
void HT1621::setDigitnum(int digitPosition, int number)
{
    setSegmentInDigit(digitPosition, 'a', false);
    setSegmentInDigit(digitPosition, 'b', false);
    setSegmentInDigit(digitPosition, 'c', false);
    setSegmentInDigit(digitPosition, 'd', false);
    setSegmentInDigit(digitPosition, 'e', false);
    setSegmentInDigit(digitPosition, 'f', false);
    setSegmentInDigit(digitPosition, 'g', false);

    switch(number)
    {
        case 0 :
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'c', true);
            setSegmentInDigit(digitPosition, 'd', true);
            setSegmentInDigit(digitPosition, 'e', true);
            setSegmentInDigit(digitPosition, 'f', true);
            break;

        case 1 :
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'c', true);
            break;

        case 2 :
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'g', true);
            setSegmentInDigit(digitPosition, 'e', true);
            setSegmentInDigit(digitPosition, 'd', true);
            break;

        case 3 :
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'c', true);
            setSegmentInDigit(digitPosition, 'd', true);
            setSegmentInDigit(digitPosition, 'g', true);
            break;

        case 4 :
            setSegmentInDigit(digitPosition, 'f', true);
            setSegmentInDigit(digitPosition, 'g', true);
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'c', true);
            break;

        case 5 :
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'f', true);
            setSegmentInDigit(digitPosition, 'g', true);
            setSegmentInDigit(digitPosition, 'c', true);
            setSegmentInDigit(digitPosition, 'd', true);
            break;

        case 6 :
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'f', true);
            setSegmentInDigit(digitPosition, 'g', true);
            setSegmentInDigit(digitPosition, 'e', true);
            setSegmentInDigit(digitPosition, 'c', true);
            setSegmentInDigit(digitPosition, 'd', true);
            break;

        case 7 :
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'c', true);
            break;

        case 8 :
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'c', true);
            setSegmentInDigit(digitPosition, 'd', true);
            setSegmentInDigit(digitPosition, 'e', true);
            setSegmentInDigit(digitPosition, 'f', true);
            setSegmentInDigit(digitPosition, 'g', true);
            break;

        case 9:
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'c', true);
            setSegmentInDigit(digitPosition, 'd', true);
            setSegmentInDigit(digitPosition, 'f', true);
            setSegmentInDigit(digitPosition, 'g', true);
            break;
    }
}
void HT1621::setDigit(int digitPosition, char number)
{
    setSegmentInDigit(digitPosition, 'a', false);
    setSegmentInDigit(digitPosition, 'b', false);
    setSegmentInDigit(digitPosition, 'c', false);
    setSegmentInDigit(digitPosition, 'd', false);
    setSegmentInDigit(digitPosition, 'e', false);
    setSegmentInDigit(digitPosition, 'f', false);
    setSegmentInDigit(digitPosition, 'g', false);

    switch(number)
    {
        case '0':
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'c', true);
            setSegmentInDigit(digitPosition, 'd', true);
            setSegmentInDigit(digitPosition, 'e', true);
            setSegmentInDigit(digitPosition, 'f', true);
            break;

        case '1':
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'c', true);
            break;

        case '2':
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'g', true);
            setSegmentInDigit(digitPosition, 'e', true);
            setSegmentInDigit(digitPosition, 'd', true);
            break;

        case '3':
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'c', true);
            setSegmentInDigit(digitPosition, 'd', true);
            setSegmentInDigit(digitPosition, 'g', true);
            break;

        case '4':
            setSegmentInDigit(digitPosition, 'f', true);
            setSegmentInDigit(digitPosition, 'g', true);
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'c', true);
            break;

        case '5':
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'f', true);
            setSegmentInDigit(digitPosition, 'g', true);
            setSegmentInDigit(digitPosition, 'c', true);
            setSegmentInDigit(digitPosition, 'd', true);
            break;

        case '6':
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'f', true);
            setSegmentInDigit(digitPosition, 'g', true);
            setSegmentInDigit(digitPosition, 'e', true);
            setSegmentInDigit(digitPosition, 'c', true);
            setSegmentInDigit(digitPosition, 'd', true);
            break;

        case '7':
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'c', true);
            break;

        case '8':
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'c', true);
            setSegmentInDigit(digitPosition, 'd', true);
            setSegmentInDigit(digitPosition, 'e', true);
            setSegmentInDigit(digitPosition, 'f', true);
            setSegmentInDigit(digitPosition, 'g', true);
            break;

        case '9':
            setSegmentInDigit(digitPosition, 'a', true);
            setSegmentInDigit(digitPosition, 'b', true);
            setSegmentInDigit(digitPosition, 'c', true);
            setSegmentInDigit(digitPosition, 'd', true);
            setSegmentInDigit(digitPosition, 'f', true);
            setSegmentInDigit(digitPosition, 'g', true);
            break;
  case 'a':
    setSegmentInDigit(digitPosition, 'a', true);
    setSegmentInDigit(digitPosition, 'b', true);
    setSegmentInDigit(digitPosition, 'c', true);
    setSegmentInDigit(digitPosition, 'e', true);
    setSegmentInDigit(digitPosition, 'f', true);
    setSegmentInDigit(digitPosition, 'g', true);
    break;

  case 'b':
    setSegmentInDigit(digitPosition, 'c', true);
    setSegmentInDigit(digitPosition, 'd', true);
    setSegmentInDigit(digitPosition, 'e', true);
    setSegmentInDigit(digitPosition, 'f', true);
    setSegmentInDigit(digitPosition, 'g', true);
    break;

  case 'c':
    setSegmentInDigit(digitPosition, 'd', true);
    setSegmentInDigit(digitPosition, 'e', true);
    setSegmentInDigit(digitPosition, 'g', true);
    break;

 case 'd':
    setSegmentInDigit(digitPosition, 'b', true);
    setSegmentInDigit(digitPosition, 'c', true);
    setSegmentInDigit(digitPosition, 'd', true);
    setSegmentInDigit(digitPosition, 'e', true);
    setSegmentInDigit(digitPosition, 'g', true);
    break;

 case 'e':
    setSegmentInDigit(digitPosition, 'a', true);
    setSegmentInDigit(digitPosition, 'd', true);
    setSegmentInDigit(digitPosition, 'e', true);
    setSegmentInDigit(digitPosition, 'f', true);
    setSegmentInDigit(digitPosition, 'g', true);
    break;

 case 'f':
    setSegmentInDigit(digitPosition, 'a', true);
    setSegmentInDigit(digitPosition, 'e', true);
    setSegmentInDigit(digitPosition, 'f', true);
    setSegmentInDigit(digitPosition, 'g', true);
    break;

 case 'h':
    setSegmentInDigit(digitPosition, 'c', true);
    setSegmentInDigit(digitPosition, 'e', true);
    setSegmentInDigit(digitPosition, 'f', true);
    setSegmentInDigit(digitPosition, 'g', true);
    break;

 case 'i':
    setSegmentInDigit(digitPosition, 'b', true);
    setSegmentInDigit(digitPosition, 'c', true);
    break;

 case 'j':
    setSegmentInDigit(digitPosition, 'b', true);
    setSegmentInDigit(digitPosition, 'c', true);
    setSegmentInDigit(digitPosition, 'd', true);
    setSegmentInDigit(digitPosition, 'e', true);
    break;

 case 'l':
    setSegmentInDigit(digitPosition, 'd', true);
    setSegmentInDigit(digitPosition, 'e', true);
    setSegmentInDigit(digitPosition, 'f', true);
    break;

 case 'n':
    setSegmentInDigit(digitPosition, 'c', true);
    setSegmentInDigit(digitPosition, 'e', true);
    setSegmentInDigit(digitPosition, 'g', true);
    break;

 case 'o':
    setSegmentInDigit(digitPosition, 'c', true);
    setSegmentInDigit(digitPosition, 'd', true);
    setSegmentInDigit(digitPosition, 'e', true);
    setSegmentInDigit(digitPosition, 'g', true);
    break;

 case 'p':
    setSegmentInDigit(digitPosition, 'a', true);
    setSegmentInDigit(digitPosition, 'b', true);
    setSegmentInDigit(digitPosition, 'e', true);
    setSegmentInDigit(digitPosition, 'f', true);
    setSegmentInDigit(digitPosition, 'g', true);
    break;

 case 'r':
    setSegmentInDigit(digitPosition, 'e', true);
    setSegmentInDigit(digitPosition, 'g', true);
    break;

 case 't':
    setSegmentInDigit(digitPosition, 'd', true);
    setSegmentInDigit(digitPosition, 'e', true);
    setSegmentInDigit(digitPosition, 'f', true);
    setSegmentInDigit(digitPosition, 'g', true);
    break;

 case 'u':
    setSegmentInDigit(digitPosition, 'c', true);
    setSegmentInDigit(digitPosition, 'd', true);
    setSegmentInDigit(digitPosition, 'e', true);
    break;

 case 'y':
    setSegmentInDigit(digitPosition, 'b', true);
    setSegmentInDigit(digitPosition, 'c', true);
    setSegmentInDigit(digitPosition, 'd', true);
    setSegmentInDigit(digitPosition, 'f', true);
    setSegmentInDigit(digitPosition, 'g', true);
    break;

 case '-':
    setSegmentInDigit(digitPosition, 'g', true);
    break;

 case '_':
    setSegmentInDigit(digitPosition, 'd', true);
    break;

 case ' ':
    break;
    case 's':
    setSegmentInDigit(digitPosition, 'a', true);
    setSegmentInDigit(digitPosition, 'c', true);
    setSegmentInDigit(digitPosition, 'd', true);
    setSegmentInDigit(digitPosition, 'f', true);
    setSegmentInDigit(digitPosition, 'g', true);
    break;
 case 'x':
    setSegmentInDigit(digitPosition, 'c', true);
    setSegmentInDigit(digitPosition, 'b', true);
    setSegmentInDigit(digitPosition, 'e', true);
    setSegmentInDigit(digitPosition, 'f', true);
    setSegmentInDigit(digitPosition, 'g', true);
    break;


    }
}
void HT1621::printicon(char* text){
    setSegmentbinary(0,3,!strcmp(text,"all"));
    setSegmentbinary(0,4,!strcmp(text,"program"));
    setSegmentbinary(0,5,!strcmp(text,"rep"));
    setSegmentbinary(0,6,!strcmp(text,"random"));
    setSegmentbinary(0,7,!strcmp(text,"usb"));
    setSegmentbinary(2,3,!strcmp(text,"play"));
    setSegmentbinary(6,0,!strcmp(text,"card"));
    setSegmentbinary(6,1,!strcmp(text,"wma"));
    setSegmentbinary(6,2,!strcmp(text,"album"));
    setSegmentbinary(6,3,!strcmp(text,"mp3"));
    update();
    }

void HT1621::print(char* text){
    setDigit(3, ' ');
    setDigit(2, ' ');
    setDigit(1, ' ');

    if (strlen(text)>=3){
        text += (strlen(text)-3);

        for(int i=3;i>=1;i--){
            char digit=*text;
            setDigit(i, digit);
            text++;
        }
    }
    else if(strlen(text)==2){

        for(int i=2;i>=1;i--){
            char digit=*text;
            setDigit(i, digit);
            text++;
        }
    }
    else if(strlen(text)==1){

        char digit=*text;
        setDigit(1, digit);
    }

    update();
}
void HT1621::printnum(int num)
{
    setDigit(3, ' ');
    setDigit(2, ' ');
    setDigit(1, ' ');

    if(num > 999)
        num = 999;

    if(num >= 100)
    {
        setDigitnum(3, num / 100);
        setDigitnum(2, (num / 10) % 10);
        setDigitnum(1, num % 10);
    }
    else if(num >= 10)
    {
        setDigitnum(2, num / 10);
        setDigitnum(1, num % 10);
    }
    else
    {
        setDigitnum(1, num);
    }

    update();
}