#include <iostream> 
#include <time.h> 
#include <string.h>
#include "../lib/LED_strip.h"
#include "../lib/OFrgba_to_bgriref.h"
#include "../lib/pca.h"
#include "../lib/pcaDefinition.h"
#include <fstream>
#include "../lib/json.hpp"

// #define fps 30
using namespace std;

// the number of leds of every strip
// maximum 16 numbers in the vector ( maximum 16 led strips )
// declaration

nlohmann::json LEDjson;

// LED Parts
#define A1 48
#define A2 48
#define A3 46
#define A4 45
#define A5 43
#define A6 42
#define A7 40
#define A8 105
#define B1 48
#define B2 49
#define B3 46
#define B4 45
#define B5 43
#define B6 42
#define B7 35
#define B8 102


enum LEDparts {
    A1_LEDd = 0,
    A2_LEDd = 1,
    A3_LEDd = 2,
    A4_LEDd = 3,
    A5_LEDd = 4,
    A6_LEDd = 5,
    A7_LEDd = 6,
    A8_LEDd = 7,
    B1_LEDd = 8,
    B2_LEDd = 9,
    B3_LEDd = 10,
    B4_LEDd = 11,
    B5_LEDd = 12,
    B6_LEDd = 13,
    B7_LEDd = 14,
    B8_LEDd = 15
};

int A1_LED[A1];
int A2_LED[A2];
int A3_LED[A3];
int A4_LED[A4];
int A5_LED[A5];
int A6_LED[A6];
int A7_LED[A7];
int A8_LED[A8];
int B1_LED[B1];
int B2_LED[B2];
int B3_LED[B3];
int B4_LED[B4];
int B5_LED[B5];
int B6_LED[B6];
int B7_LED[B7];
int B8_LED[B8];

vector<string> ledName = {"A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8"};
vector<uint16_t> nLeds;
vector< vector<char> > LED; 
LED_Strip strips;
vector< vector<vector<char>> > json;
vector<unsigned long long> timeStamps;
bool export_json=false;
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

vector<vector<char>> OF;
PCA pca;

unsigned long long timeStamp;


/*** need to modify by yourself ***/


/**********************************/

void init() {
    nLeds = {48, 48, 46, 45, 43, 42, 40, 105, 48, 49, 46, 45, 43, 42, 35, 102};
    strips.initialize(nLeds);
    LED.resize(nLeds.size());
    for(int i = 0; i < nLeds.size(); ++i){
		LED[i].resize(nLeds[i] * 3, (char)0);
	}
    // OF.resize(OF_Nums);
    // for(int i = 0; i < OF_Nums; ++i) {
    //     OF[i].resize(6);
    // }
    ifstream i("json/LED.json");
    i >> LEDjson;
    timeStamp = 0;
}

void delay(int t) {
    clock_t begin = clock();
    timeStamp += t;
    if (!export_json)
        while ((1000 * (clock() - begin)) / CLOCKS_PER_SEC < t);
        // {
        //     if (((1000 * (clock() - begin)) / CLOCKS_PER_SEC) % 10 == 0)
        //         cout << (1000 * (clock() - begin)) / CLOCKS_PER_SEC << endl;
        // } // CLOCK_PER_SEC is defined in time.h
}

int RGB(int r, int g, int b) {
    return (r << 16) | (g << 8) | b;
}

void lightchannel(int* channel, int size, int r, int g, int b) {
    for (int i = 0; i < size; ++i) {
        channel[i]  = RGB(r, g, b);
    }
}

void loadOneStrip(int *strip, int channel) {
    for (int i = 0; i < nLeds[channel]; ++i) {
        LED[channel][3*i] = char(strip[i] >> 8 & 0xff);
        LED[channel][3*i+1] = char(strip[i] >> 16 & 0xff);
        LED[channel][3*i+2] = char(strip[i] & 0xff);
    }
}

void showLED() {
    /*
    loadOneStrip(Finger_LED, 0);
    loadOneStrip(Lower_Energy_LED, 1);
    loadOneStrip(Upper_Energy_LED, 2);
    loadOneStrip(Arm_Inside_LED, 3);
    */
    static clock_t lastTime = clock();
    if (!export_json) {
        // for (int i = 0; i < Finger_LEDs; ++i) {
        //     LED[LEDparts::Finger_LEDd][3*i + 0] = char(Finger_LED[i] >> 16 & 0xff);
        //     LED[LEDparts::Finger_LEDd][3*i + 1] = char(Finger_LED[i] >> 8 & 0xff);
        //     LED[LEDparts::Finger_LEDd][3*i + 2] = char(Finger_LED[i] & 0xff);
        // }
        // for (int i = 0; i < Lower_Energy_LEDs; ++i) {
        //     LED[LEDparts::Lower_Energy_LEDd][3*i + 0] = char(Lower_Energy_LED[i] >> 16 & 0xff);
        //     LED[LEDparts::Lower_Energy_LEDd][3*i + 1] = char(Lower_Energy_LED[i] >> 8 & 0xff);
        //     LED[LEDparts::Lower_Energy_LEDd][3*i + 2] = char(Lower_Energy_LED[i] & 0xff);
        // }
        // for (int i = 0; i < Upper_Energy_LEDs; ++i) {
        //     LED[LEDparts::Upper_Energy_LEDd][3*i + 0] = char(Upper_Energy_LED[i] >> 16 & 0xff);
        //     LED[LEDparts::Upper_Energy_LEDd][3*i + 1] = char(Upper_Energy_LED[i] >> 8 & 0xff);
        //     LED[LEDparts::Upper_Energy_LEDd][3*i + 2] = char(Upper_Energy_LED[i] & 0xff);
        // }
        // for (int i = 0; i < Arm_Inside_LEDs; ++i) {
        //     LED[LEDparts::Arm_Inside_LEDd][3*i + 0] = char(Arm_Inside_LED[i] >> 16 & 0xff);
        //     LED[LEDparts::Arm_Inside_LEDd][3*i + 1] = char(Arm_Inside_LED[i] >> 8 & 0xff);
        //     LED[LEDparts::Arm_Inside_LEDd][3*i + 2] = char(Arm_Inside_LED[i] & 0xff);
        // }

        loadOneStrip(A1_LED, LEDparts::A1_LEDd);
        loadOneStrip(A2_LED, LEDparts::A2_LEDd);
        loadOneStrip(A3_LED, LEDparts::A3_LEDd);
        loadOneStrip(A4_LED, LEDparts::A4_LEDd);
        loadOneStrip(A5_LED, LEDparts::A5_LEDd);
        loadOneStrip(A6_LED, LEDparts::A6_LEDd);
        loadOneStrip(A7_LED, LEDparts::A7_LEDd);
        loadOneStrip(A8_LED, LEDparts::A8_LEDd);
        loadOneStrip(B1_LED, LEDparts::B1_LEDd);
        loadOneStrip(B2_LED, LEDparts::B2_LEDd);
        loadOneStrip(B3_LED, LEDparts::B3_LEDd);
        loadOneStrip(B4_LED, LEDparts::B4_LEDd);
        loadOneStrip(B5_LED, LEDparts::B5_LEDd);
        loadOneStrip(B6_LED, LEDparts::B6_LEDd);
        loadOneStrip(B7_LED, LEDparts::B7_LEDd);
        loadOneStrip(B8_LED, LEDparts::B8_LEDd);

        strips.sendToStrip(LED);
    }
    json.push_back(LED);
    timeStamps.push_back(timeStamp);
    // Write json
}

void updateStrip(int frame, int* strip, int channel) {
    for (int i = 0; i < nLeds[channel]; ++i) {
        strip[i] = LEDjson[ledName[channel]][frame]["status"][i]["colorCode"];
    }
}


void updateFrame(int frame) {
    updateStrip(frame, A1_LED, LEDparts::A1_LEDd);
    updateStrip(frame, A2_LED, LEDparts::A2_LEDd);
    updateStrip(frame, A3_LED, LEDparts::A3_LEDd);
    updateStrip(frame, A4_LED, LEDparts::A4_LEDd);
    updateStrip(frame, A5_LED, LEDparts::A5_LEDd);
    updateStrip(frame, A6_LED, LEDparts::A6_LEDd);
    updateStrip(frame, A7_LED, LEDparts::A7_LEDd);
    updateStrip(frame, A8_LED, LEDparts::A8_LEDd);
    updateStrip(frame, B1_LED, LEDparts::B1_LEDd);
    updateStrip(frame, B2_LED, LEDparts::B2_LEDd);
    updateStrip(frame, B3_LED, LEDparts::B3_LEDd);
    updateStrip(frame, B4_LED, LEDparts::B4_LEDd);
    updateStrip(frame, B5_LED, LEDparts::B5_LEDd);
    updateStrip(frame, B6_LED, LEDparts::B6_LEDd);
    updateStrip(frame, B7_LED, LEDparts::B7_LEDd);
    updateStrip(frame, B8_LED, LEDparts::B8_LEDd);
}
/*
void showOF() {
    pca.WriteAll(OF);
}

void rgbToOF(uint8_t r, uint8_t g, uint8_t b) {

}
*/

int main(int argc, char* argv[]) {
    // PCA pca;
    init();
    if (argc > 1)
        export_json = (strcmp(argv[1], "output") == 0);
    
    int fps;
    cin >> fps;
    int t = 1000/fps;

    int frame = 1;
    clock_t initTime = clock();
    clock_t lastFrame = clock();
    updateFrame(0);

    while(frame < LEDjson["A1"].size()){
        if ((1000 * (clock() - lastFrame)) / CLOCKS_PER_SEC > t) {
            showLED();
            lastFrame = clock();
        }
        if (((1000 * (clock() - initTime)) / CLOCKS_PER_SEC) > LEDjson["A1"][frame]["start"]) {
            updateFrame(frame);
            cout << frame << endl;
            frame++;
        }

    }
    showLED();
    pinMode(0, INPUT);
}
