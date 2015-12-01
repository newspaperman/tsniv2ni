/*
    TS NI V.11 to NI G.703 
    Copyright (C) 2015 Newspaperman

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include<iostream>
#include<fstream>
#include<string.h>
#include<stdlib.h>

using namespace std;

istream* inbuf;
ostream* outbuf;
int size;
bool active=false;
const char fsync0[]={0xff, 0x07,0x3a,0xb6};
const char fsync1[]={0xff, 0xf8,0xc5,0x49};
char * frpd;
int count;

void processEti(char* buf, int len, bool isStarting) {
	if(isStarting) {
		if(active) {
			outbuf->write(frpd,6140-count);	
		}
		active=true;
		count=0;
	}
	count+=len;
	if(active) {
		if(isStarting) {
			if(buf[0]%2==0) {
				outbuf->write(fsync0,4);		
			} else {
				outbuf->write(fsync1,4);		
			}
		}
		outbuf->write(buf, len);

	}
}



int main(int argc, char** argv) {
	if(argc!=2) {
		fprintf(stderr,"Usage: %s <pid>\n",argv[0]);
	}
	inbuf = &cin;
	outbuf = &cout;
	size=200;
	char* packets= new char[size*188];
	frpd= new char[6140];
	memset(frpd,0x55,6140);
	count=0;
	while(1) {
		inbuf->read(packets,size * 188);
		if(inbuf->fail()) break;
		unsigned char *p;
		//outbuf->write(packets,size*188);
		for(int i=0; i<size*188;i+=188) {
				int start=0;
				/*if((((packets[i+3])&0xC0))!=0x00) {
					std::cerr << "encrypted" << std::endl;	
					continue;
				}*/
				int pid=(unsigned char)(packets[i+1]);
				pid&=0x01F;
				pid<<=8;
				pid|=(unsigned char)(packets[i+2]);
				
				if(pid!=atoi(argv[1])) {
					continue;

				}
				if((((packets[i+3])&0x30)>>4)==0x03) {
					start++;
					//printf("%i<>",static_cast<unsigned char>(packet[4]));
					start+=static_cast<unsigned char>(packets[i+4]); //adaption + payload, offset addieren
					if(start> 183) {
						std::cerr << "wrong AF" << std::endl;	
						continue;
					}
				}
				else if(((((packets[i + 3])&0x30)>>4)==0x02)) {
					std::cerr<< "cont " << i << std::endl;
					continue; // nur adaption field
				}
				if((((packets[i+1])&0x40)>>4)==0x04) { //START INDICATOR
					start=1;
					processEti(&packets[i+4+start],184-start, true);
				} else {
					processEti(&packets[i+4+start],184-start, false);
				}
		}
	}
	return 0;
}
