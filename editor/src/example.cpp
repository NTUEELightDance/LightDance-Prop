#include <iostream> 
#include <time.h> 
#include <string.h>
#include "../lib/LED_strip.h"
#include "../lib/OFrgba_to_bgriref.h"
#include "../lib/pca.h"
#include "../lib/pcaDefinition.h"
#include <fstream>

// #define fps 30
using namespace std;

// the number of leds of every strip
// maximum 16 numbers in the vector ( maximum 16 led strips )
// declaration


// LED Parts

#define Finger_LEDs 48
#define Lower_Energy_LEDs 48
#define Upper_Energy_LEDs 46
#define Arm_Inside_LEDs 45

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
int Finger_LED[Finger_LEDs];
int Lower_Energy_LED[Lower_Energy_LEDs];
int Upper_Energy_LED[Upper_Energy_LEDs];
int Arm_Inside_LED[Arm_Inside_LEDs];

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
        LED[channel][3*i] = char(strip[i] >> 16 & 0xff);
        LED[channel][3*i+1] = char(strip[i] >> 8 & 0xff);
        LED[channel][3*i+2] = char(strip[i] >> 16 & 0xff);
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
/*
void showOF() {
    pca.WriteAll(OF);
}

void rgbToOF(uint8_t r, uint8_t g, uint8_t b) {

}
*/

// void output() {
//     // Finger_LED = 0,
//     // Lower_Energy_LED = 1,
//     // Upper_Energy_LED = 2,
//     // Arm_Inside_LEDd= 3
//     ofstream outfile;
//     outfile.open("./json/LED.json");
//     outfile << "{" << endl;
//     outfile << "\t\"Finger_LED\": [" << endl;
//     for (int i = 0; i < json.size(); ++i) {
//         outfile << "\t\t{" << endl;
//         outfile << "\t\t\t\"start\": " << timeStamps[i] << "," << endl;
//         outfile << "\t\t\t\"fade\": " << "false," << endl;
//         outfile << "\t\t\t\"status\": " << "[" << endl;
//         for (int j = 0; j < Finger_LEDs; ++j) {
//             int colorCode = json[i][LEDparts::Finger_LEDd][3*j + 0];
//             colorCode = (colorCode << 8) | json[i][LEDparts::Finger_LEDd][3*j + 1];
//             colorCode = (colorCode << 8) | json[i][LEDparts::Finger_LEDd][3*j + 2];
//             outfile << "\t\t\t\t{ \"colorCode\": " << colorCode << "," << "\"alpha\": 15 }";
//             if (j == Finger_LEDs-1)
//                 outfile << endl;
//             else
//                 outfile << "," << endl;
//         }
//         outfile << "\t\t\t]" << endl;
//         outfile << "\t\t}";
//         if (i == json.size()-1)
//             outfile << endl;
//         else
//             outfile << "," << endl;
//     }
//     outfile << "\t]," << endl;
//     outfile << "\t\"Lower_Energy_LED\": [" << endl;
//     for (int i = 0; i < json.size(); ++i) {
//         outfile << "\t\t{" << endl;
//         outfile << "\t\t\t\"start\": " << timeStamps[i] << "," << endl;
//         outfile << "\t\t\t\"fade\": " << "false," << endl;
//         outfile << "\t\t\t\"status\": " << "[" << endl;
//         for (int j = 0; j < Lower_Energy_LEDs; ++j) {
//             int colorCode = json[i][LEDparts::Lower_Energy_LEDd][3*j + 0];
//             colorCode = (colorCode << 8) | json[i][LEDparts::Lower_Energy_LEDd][3*j + 1];
//             colorCode = (colorCode << 8) | json[i][LEDparts::Lower_Energy_LEDd][3*j + 2];
//             outfile << "\t\t\t\t{ \"colorCode\": " << colorCode << "," << "\"alpha\": 15 }";
//             if (j == Lower_Energy_LEDs-1)
//                 outfile << endl;
//             else
//                 outfile << "," << endl;
//         }
//         outfile << "\t\t\t]" << endl;
//         outfile << "\t\t}";
//         if (i == json.size()-1)
//             outfile << endl;
//         else
//             outfile << "," << endl;
//     }
//     outfile << "\t]," << endl;

//     outfile << "\t\"Upper_Energy_LED\": [" << endl;
//     for (int i = 0; i < json.size(); ++i) {
//         outfile << "\t\t{" << endl;
//         outfile << "\t\t\t\"start\": " << timeStamps[i] << "," << endl;
//         outfile << "\t\t\t\"fade\": " << "false," << endl;
//         outfile << "\t\t\t\"status\": " << "[" << endl;
//         for (int j = 0; j < Upper_Energy_LEDs; ++j) {
//             int colorCode = json[i][LEDparts::Upper_Energy_LEDd][3*j + 0];
//             colorCode = (colorCode << 8) | json[i][LEDparts::Upper_Energy_LEDd][3*j + 1];
//             colorCode = (colorCode << 8) | json[i][LEDparts::Upper_Energy_LEDd][3*j + 2];
//             outfile << "\t\t\t\t{ \"colorCode\": " << colorCode << "," << "\"alpha\": 15 }";
//                         cout << colorCode << endl;
//             if (j == Upper_Energy_LEDs-1)
//                 outfile << endl;
//             else
//                 outfile << "," << endl;
//         }
//         outfile << "\t\t\t]" << endl;
//         outfile << "\t\t}";
//         if (i == json.size()-1)
//             outfile << endl;
//         else
//             outfile << "," << endl;
//     }
//     outfile << "\t]," << endl;

//     outfile << "\t\"Arm_Inside_LED\": [" << endl;
//     for (int i = 0; i < json.size(); ++i) {
//         outfile << "\t\t{" << endl;
//         outfile << "\t\t\t\"start\": " << timeStamps[i] << "," << endl;
//         outfile << "\t\t\t\"fade\": " << "false," << endl;
//         outfile << "\t\t\t\"status\": " << "[" << endl;
//         for (int j = 0; j < Arm_Inside_LEDs; ++j) {
//             int colorCode = json[i][LEDparts::Arm_Inside_LEDd][3*j + 0];
//             colorCode = (colorCode << 8) | json[i][LEDparts::Arm_Inside_LEDd][3*j + 1];
//             colorCode = (colorCode << 8) | json[i][LEDparts::Arm_Inside_LEDd][3*j + 2];
//             outfile << "\t\t\t\t{ \"colorCode\": " << colorCode << "," << "\"alpha\": 15 }";
//             if (j == Arm_Inside_LEDs-1)
//                 outfile << endl;
//             else
//                 outfile << "," << endl;
//         }
//         outfile << "\t\t\t]" << endl;
//         outfile << "\t\t}";
//         if (i == json.size()-1)
//             outfile << endl;
//         else
//             outfile << "," << endl;
//     }
//     outfile << "\t]" << endl;
//     outfile << "}" << endl;

// /*
//     {
//   "Glove_L_LED": [
//     {
//       "start": 0,
//       "fade": true,
//       "status": [
//         { "colorCode": 16777005, "alpha": 10 },
//         { "colorCode": 16777005, "alpha": 10 }
//       ]
//     },
//     {
//       "start": 100,
//       "fade": false,
//       "status": [
//         { "colorCode": 16777005, "alpha": 10 },
//         { "colorCode": 16777005, "alpha": 10 }
//       ]
//     },
//     {
//       "start": 200,
//       "fade": false,
//       "effect": [
//         { "colorCode": 16777005, "alpha": 10 },
//         { "colorCode": 16777005, "alpha": 10 }
//       ]
//     }
//   ],
//   "Glove_R_LED": [
//     {
//       "start": 0,
//       "fade": true,
//       "status": [
//         { "colorCode": 16777005, "alpha": 10 },
//         { "colorCode": 16777005
//     vector< vector<vector<char>> > json;
// */
// }
int main(int argc, char* argv[]) {
    // PCA pca;
    init();
    if (argc > 1)
        export_json = (strcmp(argv[1], "output") == 0);
    
    int fps;
    cin >> fps;
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
    while(true){
        for (int i = 130; i >= 0; i-=10){
            lightchannel(A1_LED, A1, i, i, i);
            lightchannel(A2_LED, A2, i, i, i);
            lightchannel(A3_LED, A3, i, i, i);
            lightchannel(A4_LED, A4, i, i, i);
            lightchannel(A5_LED, A5, i, i, i);
            lightchannel(A6_LED, A6, i, i, i);
            lightchannel(A7_LED, A7, i, i, i);
            lightchannel(A8_LED, A8, i, i, i);
            lightchannel(B1_LED, B1, i, i, i);
            lightchannel(B2_LED, B2, i, i, i);
            lightchannel(B3_LED, B3, i, i, i);
            lightchannel(B4_LED, B4, i, i, i);
            lightchannel(B5_LED, B5, i, i, i);
            lightchannel(B6_LED, B6, i, i, i);
            lightchannel(B7_LED, B7, i, i, i);
            lightchannel(B8_LED, B8, i, i, i);
            showLED();
            delay(1000/fps);
        }

        for (int i = 0; i <= 130; i+=10){
            lightchannel(A1_LED, A1, i, i, i);
            lightchannel(A2_LED, A2, i, i, i);
            lightchannel(A3_LED, A3, i, i, i);
            lightchannel(A4_LED, A4, i, i, i);
            lightchannel(A5_LED, A5, i, i, i);
            lightchannel(A6_LED, A6, i, i, i);
            lightchannel(A7_LED, A7, i, i, i);
            lightchannel(A8_LED, A8, i, i, i);
            lightchannel(B1_LED, B1, i, i, i);
            lightchannel(B2_LED, B2, i, i, i);
            lightchannel(B3_LED, B3, i, i, i);
            lightchannel(B4_LED, B4, i, i, i);
            lightchannel(B5_LED, B5, i, i, i);
            lightchannel(B6_LED, B6, i, i, i);
            lightchannel(B7_LED, B7, i, i, i);
            lightchannel(B8_LED, B8, i, i, i);
            showLED();
            delay(1000/fps);
        }

        // Upper_Energy_LED[10] = RGB(0, 0,128);
        // Lower_Energy_LED[2] = RGB(0, 0,128);
        // showLED();
        
        // delay(100);
        // lightchannel(Finger_LED, Finger_LEDs, 0, 0, 0);
        // lightchannel(Lower_Energy_LED, Lower_Energy_LEDs, 0, 0, 0);
        // lightchannel(Upper_Energy_LED, Upper_Energy_LEDs, 0, 0, 0);
        // lightchannel(Arm_Inside_LED, Arm_Inside_LEDs, 0, 0, 0);;
        // // Upper_Energy_LED[2] = RGB(128,128,128);
        // // Lower_Energy_LED[1] = RGB(0,128,128);
        // showLED();
        // delay(100);
    }
    // if (export_json) {
    //     output();
    // }
    pinMode(0, INPUT);
}
