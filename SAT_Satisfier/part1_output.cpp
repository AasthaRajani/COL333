#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#define ll long long
using namespace std;

int main(int argc, char* argv[]){
    if (argc < 2) {
        std::cerr << "give the file name"<< std::endl;
        return 1; 
    }
    ll N=0;
    ifstream N_in("input_N.txt");
        if (!N_in.is_open()) {
            cerr << "Error: Unable to open the input_N." <<endl;
            return 1;
        }
        string firstLine;
        getline(N_in, firstLine);
        stringstream ss1(firstLine);
        string token;
        getline(ss1,token,' ');
        N = stoi(token);
    ifstream sat_output(string(argv[1])+".satoutput");
        if (!sat_output.is_open()) {
            cerr << "Error: Unable to open the sat_output." <<endl;
            return 1;
        }
        // string firstLine;
        getline(sat_output, firstLine);
        string secondLine;
        vector<ll> G1;
        vector<ll> G2;
        getline(sat_output, secondLine);
        stringstream ss(secondLine);
        // string token;
        while (getline(ss, token, ' ')) {
            if (stoi(token)>2*N + 1){
            break;}
            if (stoi(token)>0 && stoi(token)<=N){
                G1.push_back(stoi(token));
            }
            if (stoi(token)>0 && stoi(token)>N){
                G2.push_back(stoi(token)-N-1);
            }
        }
        sat_output.close();
        ofstream outputFile1(string(argv[1])+".mapping",ofstream::trunc);
        outputFile1<<"#1"<<endl;
        for(int i=0;i<G1.size();i++){
            if (i==G1.size()-1){
            outputFile1<<G1[i];
            break;}
            outputFile1<<G1[i]<<" ";
        }
        outputFile1<<endl;
        outputFile1<<"#2"<<endl;
        for(int i=0;i<G2.size();i++){
            if (i==G2.size()-1){ 
            outputFile1<<G2[i];
            break;}
            outputFile1<<G2[i]<<" ";
        }    
        outputFile1<<endl;
        outputFile1.close();
}