#include <iostream> 
#include <time.h> 
#include "../lib/LED_strip.h"
#include "../lib/OFrgba_to_bgriref.h"
#include "../lib/pca.h"
#include "../lib/pcaDefinition.h"
#include <fstream>

#define fps 30
using namespace std;

// the number of leds of every strip
// maximum 16 numbers in the vector ( maximum 16 led strips )
// declaration


// LED Parts

#define Finger_LEDs 28
#define Lower_Energy_LEDs 33
#define Upper_Energy_LEDs 69
#define Arm_Inside_LEDs 20
enum LEDparts {
    Finger_LEDd = 0,
    Lower_Energy_LEDd = 1,
    Upper_Energy_LEDd = 2,
    Arm_Inside_LEDd = 3
};
int Finger_LED[Finger_LEDs];
int Lower_Energy_LED[Lower_Energy_LEDs];
int Upper_Energy_LED[Upper_Energy_LEDs];
int Arm_Inside_LED[Arm_Inside_LEDs];

vector<uint16_t> nLeds;
vector< vector<char> > LED; 
LED_Strip strips;
vector< vector<vector<char>> > json;
vector<unsigned long long> timeStamps;
bool export_json;
// OF Parts

#define OF_Nums 20
enum OFparts {
    Thumb = 0,
    Fore_Finger_1 = 1,
    Fore_Finger_2 = 2,
    Middle_Finger_1 = 3,
    Middle_Finger_2 = 4,
    Ring_Finger_1 = 5,
    Ring_Finger_2 = 6,
    Little_Finger_1 = 7,
    Little_Finger_2 = 8,
    F_Diamond = 9,
    B_Diamond = 10,
    Palm_F_Side = 11,
    Palm_B_Side = 12,
    Palm_Shield = 13,
    Palm = 14,
    Arm_F_Side = 15,
    Arm_F_Shield = 16,
    Arm_B_Shield = 17,
    Arm_B_Side = 18,
    Arm_Inside = 19
};
/*
vector<vector<char>> OF;
PCA pca;
*/
unsigned long long timeStamp;


/*** need to modify by yourself ***/


/**********************************/

void init() {
    nLeds = {28, 33, 69, 20};
    strips.initialize(nLeds);
    LED.resize(nLeds.size());
    for(int i = 0; i < nLeds.size(); ++i){
		LED[i].resize(nLeds[i] * 3, (char)0);
	}
    /*
    OF.resize(OF_Nums);
    for(int i = 0; i < OF_Nums; ++i) {
        OF[i].resize(6);
    }
    */
    timeStamp = 0;
}

void delay(int t) {
    clock_t begin = clock();
    timeStamp += t;
    if (!export_json)
        while ((1000 * (clock() - begin)) / CLOCKS_PER_SEC < t); // CLOCK_PER_SEC is defined in time.h
}

int RGB(int r, int g, int b) {
    return (r << 16) | (g << 8) | b;
}

void lightchannel(int* channel, int size, int r, int g, int b) {
    for (int i = 0; i < size; ++i) {
        channel[i]  = RGB(r, g, b);
    }
}
/*
void loadOneStrip(uint8_t *strip, int channel) {
    for (int i = 0; i < nLeds[channel]; ++i) {
        buf[channel][3*i] = strip[3*i];
        buf[channel][3*i+1] = strip[3*i+1];
        buf[channel][3*i+2] = strip[3*i+2];
    }
}
*/

void showLED() {
    /*
    loadOneStrip(Finger_LED, 0);
    loadOneStrip(Lower_Energy_LED, 1);
    loadOneStrip(Upper_Energy_LED, 2);
    loadOneStrip(Arm_Inside_LED, 3);
    */
    static clock_t lastTime = clock();
    if (!export_json) {
        for (int i = 0; i < Finger_LEDs; ++i) {
            LED[LEDparts::Finger_LEDd][3*i + 0] = char(Finger_LED[i] >> 16 & 0xff);
            LED[LEDparts::Finger_LEDd][3*i + 1] = char(Finger_LED[i] >> 8 & 0xff);
            LED[LEDparts::Finger_LEDd][3*i + 2] = char(Finger_LED[i] & 0xff);
        }
        for (int i = 0; i < Lower_Energy_LEDs; ++i) {
            LED[LEDparts::Lower_Energy_LEDd][3*i + 0] = char(Lower_Energy_LED[i] >> 16 & 0xff);
            LED[LEDparts::Lower_Energy_LEDd][3*i + 1] = char(Lower_Energy_LED[i] >> 8 & 0xff);
            LED[LEDparts::Lower_Energy_LEDd][3*i + 2] = char(Lower_Energy_LED[i] & 0xff);
        }
        for (int i = 0; i < Upper_Energy_LEDs; ++i) {
            LED[LEDparts::Upper_Energy_LEDd][3*i + 0] = char(Upper_Energy_LED[i] >> 16 & 0xff);
            LED[LEDparts::Upper_Energy_LEDd][3*i + 1] = char(Upper_Energy_LED[i] >> 8 & 0xff);
            LED[LEDparts::Upper_Energy_LEDd][3*i + 2] = char(Upper_Energy_LED[i] & 0xff);
        }
        for (int i = 0; i < Arm_Inside_LEDs; ++i) {
            LED[LEDparts::Arm_Inside_LEDd][3*i + 0] = char(Arm_Inside_LED[i] >> 16 & 0xff);
            LED[LEDparts::Arm_Inside_LEDd][3*i + 1] = char(Arm_Inside_LED[i] >> 8 & 0xff);
            LED[LEDparts::Arm_Inside_LEDd][3*i + 2] = char(Arm_Inside_LED[i] & 0xff);
        }
        strips.sendToStrip(LED);
    }
    json.push_back(LED);
    timeStamps.push_back(timeStamp);
    // Write json
}
/*
void showOF() {
    pca.WriteAll(OF);
}

void rgbToOF(uint8_t r, uint8_t g, uint8_t b) {

}
*/

void output() {
    // Finger_LED = 0,
    // Lower_Energy_LED = 1,
    // Upper_Energy_LED = 2,
    // Arm_Inside_LEDd= 3
    ofstream outfile;
    outfile.open("./json/LED.json");
    outfile << "{" << endl;
    outfile << "\t\"Finger_LED\": [" << endl;
    for (int i = 0; i < json.size(); ++i) {
        outfile << "\t\t{" << endl;
        outfile << "\t\t\t\"start\": " << timeStamps[i] << "," << endl;
        outfile << "\t\t\t\"fade\": " << "false," << endl;
        outfile << "\t\t\t\"status\": " << "[" << endl;
        for (int j = 0; j < Finger_LEDs; ++j) {
            int colorCode = json[i][LEDparts::Finger_LEDd][3*j + 0];
            colorCode = (colorCode << 8) | json[i][LEDparts::Finger_LEDd][3*j + 1];
            colorCode = (colorCode << 8) | json[i][LEDparts::Finger_LEDd][3*j + 2];
            outfile << "\t\t\t\t{ \"colorCode\": " << colorCode << "," << "\"alpha\": 15 }";
            if (j == Finger_LEDs-1)
                outfile << endl;
            else
                outfile << "," << endl;
        }
        outfile << "\t\t\t]" << endl;
        outfile << "\t\t}";
        if (i == json.size()-1)
            outfile << endl;
        else
            outfile << "," << endl;
    }
    outfile << "\t]," << endl;
    outfile << "\t\"Lower_Energy_LED\": [" << endl;
    for (int i = 0; i < json.size(); ++i) {
        outfile << "\t\t{" << endl;
        outfile << "\t\t\t\"start\": " << timeStamps[i] << "," << endl;
        outfile << "\t\t\t\"fade\": " << "false," << endl;
        outfile << "\t\t\t\"status\": " << "[" << endl;
        for (int j = 0; j < Lower_Energy_LEDs; ++j) {
            int colorCode = json[i][LEDparts::Lower_Energy_LEDd][3*j + 0];
            colorCode = (colorCode << 8) | json[i][LEDparts::Lower_Energy_LEDd][3*j + 1];
            colorCode = (colorCode << 8) | json[i][LEDparts::Lower_Energy_LEDd][3*j + 2];
            outfile << "\t\t\t\t{ \"colorCode\": " << colorCode << "," << "\"alpha\": 15 }";
            if (j == Lower_Energy_LEDs-1)
                outfile << endl;
            else
                outfile << "," << endl;
        }
        outfile << "\t\t\t]" << endl;
        outfile << "\t\t}";
        if (i == json.size()-1)
            outfile << endl;
        else
            outfile << "," << endl;
    }
    outfile << "\t]," << endl;

    outfile << "\t\"Upper_Energy_LED\": [" << endl;
    for (int i = 0; i < json.size(); ++i) {
        outfile << "\t\t{" << endl;
        outfile << "\t\t\t\"start\": " << timeStamps[i] << "," << endl;
        outfile << "\t\t\t\"fade\": " << "false," << endl;
        outfile << "\t\t\t\"status\": " << "[" << endl;
        for (int j = 0; j < Upper_Energy_LEDs; ++j) {
            int colorCode = json[i][LEDparts::Upper_Energy_LEDd][3*j + 0];
            colorCode = (colorCode << 8) | json[i][LEDparts::Upper_Energy_LEDd][3*j + 1];
            colorCode = (colorCode << 8) | json[i][LEDparts::Upper_Energy_LEDd][3*j + 2];
            outfile << "\t\t\t\t{ \"colorCode\": " << colorCode << "," << "\"alpha\": 15 }";
                        cout << colorCode << endl;
            if (j == Upper_Energy_LEDs-1)
                outfile << endl;
            else
                outfile << "," << endl;
        }
        outfile << "\t\t\t]" << endl;
        outfile << "\t\t}";
        if (i == json.size()-1)
            outfile << endl;
        else
            outfile << "," << endl;
    }
    outfile << "\t]," << endl;

    outfile << "\t\"Arm_Inside_LED\": [" << endl;
    for (int i = 0; i < json.size(); ++i) {
        outfile << "\t\t{" << endl;
        outfile << "\t\t\t\"start\": " << timeStamps[i] << "," << endl;
        outfile << "\t\t\t\"fade\": " << "false," << endl;
        outfile << "\t\t\t\"status\": " << "[" << endl;
        for (int j = 0; j < Arm_Inside_LEDs; ++j) {
            int colorCode = json[i][LEDparts::Arm_Inside_LEDd][3*j + 0];
            colorCode = (colorCode << 8) | json[i][LEDparts::Arm_Inside_LEDd][3*j + 1];
            colorCode = (colorCode << 8) | json[i][LEDparts::Arm_Inside_LEDd][3*j + 2];
            outfile << "\t\t\t\t{ \"colorCode\": " << colorCode << "," << "\"alpha\": 15 }";
            if (j == Arm_Inside_LEDs-1)
                outfile << endl;
            else
                outfile << "," << endl;
        }
        outfile << "\t\t\t]" << endl;
        outfile << "\t\t}";
        if (i == json.size()-1)
            outfile << endl;
        else
            outfile << "," << endl;
    }
    outfile << "\t]" << endl;
    outfile << "}" << endl;

/*
    {
  "Glove_L_LED": [
    {
      "start": 0,
      "fade": true,
      "status": [
        { "colorCode": 16777005, "alpha": 10 },
        { "colorCode": 16777005, "alpha": 10 }
      ]
    },
    {
      "start": 100,
      "fade": false,
      "status": [
        { "colorCode": 16777005, "alpha": 10 },
        { "colorCode": 16777005, "alpha": 10 }
      ]
    },
    {
      "start": 200,
      "fade": false,
      "effect": [
        { "colorCode": 16777005, "alpha": 10 },
        { "colorCode": 16777005, "alpha": 10 }
      ]
    }
  ],
  "Glove_R_LED": [
    {
      "start": 0,
      "fade": true,
      "status": [
        { "colorCode": 16777005, "alpha": 10 },
        { "colorCode": 16777005
    vector< vector<vector<char>> > json;
*/
}
int main(int argc, char* argv[]) {
    init();
    export_json = (argc > 1);
    // resize buf to a (number of strips)*(number of leds of the strip)*3 vector
    // nLeds={60, 60, 60, 60, 60,60};//change the numbers of lights here
    // strips.initialize(nLeds);
    
	// buf.resize(nLeds.size());
	// for(int i = 0; i < nLeds.size(); ++i){
	// 	buf[i].resize(nLeds[i] * 3, (char)0);
	// }
    //clock_t init = clock();
    // set the colors of every leds
    // this example set every leds to green
    // the while loop is for 
    /*
    while(true) {
        for (int i = 0; i < Finger_LEDs; ++i) {
            Finger_LED[3*i] = 0; // r
            Finger_LED[3*i+1] = 0; // g
            Finger_LED[3*i+2] = 0; // b
        }
        // send data to stm32s
        showLED();
        // wait for stm to deal with the data
    }
    */
    lightchannel(Finger_LED, Finger_LEDs, 128, 128, 128);
    lightchannel(Lower_Energy_LED, Lower_Energy_LEDs, 128, 128, 128);
    lightchannel(Upper_Energy_LED, Upper_Energy_LEDs, 128, 128, 128);
    lightchannel(Arm_Inside_LED, Arm_Inside_LEDs, 128, 128, 128);

    // Upper_Energy_LED[10] = RGB(0, 0,128);
    // Lower_Energy_LED[2] = RGB(0, 0,128);
    showLED();
    
    delay(1000);
    lightchannel(Finger_LED, Finger_LEDs, 0, 0, 0);
    lightchannel(Lower_Energy_LED, Lower_Energy_LEDs, 0, 0, 0);
    lightchannel(Upper_Energy_LED, Upper_Energy_LEDs, 0, 0, 0);
    lightchannel(Arm_Inside_LED, Arm_Inside_LEDs, 0, 0, 0);;
    // Upper_Energy_LED[2] = RGB(128,128,128);
    // Lower_Energy_LED[1] = RGB(0,128,128);
    showLED();
    if (export_json) {
        output();
    }
}