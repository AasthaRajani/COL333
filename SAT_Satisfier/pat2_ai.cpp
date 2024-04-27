#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <chrono>
#include <stdio.h>
#define ll long long
using namespace std;

// funciton to convert a literal into a string

string string_literal(ll x){
    if (x>0){
        return to_string(x);
    }
    else{
        return "-"+to_string(-x);
    }
}

// function to make clauses for dp relation in the graph 
// for bases cases only one clause either true or false
// for other cases we have 4 clauses
void make_clause_dp(vector<vector<ll> > dp, vector<ll> X,ll N,ll K,ofstream& outputFile){
    for (ll i=0;i<=K;i++){
        for (ll j=0;j<=N;j++){
            if (i==K && j==N){
                outputFile<< string_literal(dp[i][j]) <<" 0" <<endl;}
             if (i==0 && j==0){
                outputFile<< string_literal(dp[i][j]) <<" 0" <<endl;
            }
            else if (i==0){
                outputFile<< string_literal(dp[i][j]) << " 0" <<endl;
            }
            else if (j==0){
                outputFile<< string_literal(-dp[i][j]) <<" 0" <<endl;
            }else{
                ll a,b,c,d;
                a=dp[i][j];
                b=dp[i][j-1];
                c = dp[i-1][j-1];
                d = X[j];
                outputFile<< string_literal(a)<<" "<<string_literal(-b)<<" 0"<<endl;
                outputFile<< string_literal(-a)<<" "<<string_literal(b)<<" "<<string_literal(c)<<" 0"<<endl;
                outputFile<< string_literal(-a)<<" "<<string_literal(b)<<" "<<string_literal(-c)<<" "<<string_literal(d)<<" 0"<<endl;
                outputFile<< string_literal(a)<<" "<<string_literal(b)<<" "<<string_literal(-c)<<" "<<string_literal(-d)<<" 0"<<endl;
            }
        }
    }
}

// function to make clauses for edges in the graph
// make clauses for all the non-edges in the graph
void make_clause_edges(vector<vector<ll> > adj,vector<ll> X,ofstream& outputFile){
    ll n = adj.size();
    for (ll i=1;i<n;i++){
        for (ll j=i+1;j<n;j++){
            if (adj[i][j]==0){
                outputFile<< string_literal(-X[i]) << " " << string_literal(-X[j]) <<" 0" <<endl;
            }
        }
    }

}

int main(int argc, char* argv[]){
    auto start_time = std::chrono::high_resolution_clock::now();

    if (argc < 2) {
        std::cerr << "give the file name"<< std::endl;
        return 1; 
    }
    // taking the input file
    ifstream inputFile(string(argv[1])+".graph");

    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open the file." <<endl;
        return 1;
    }
    ll N=0,E=0;
    string line;
    vector<vector<ll> > adj;
    bool get=false;
    while (getline(inputFile, line)) {
        ll u, v;
        // reading the input lines
        if (std::istringstream(line) >> u >> v) {
            if (get==false){
                N=u;
                E=v;
                get=true;
                for (int i=0;i<=N;i++){
                    adj.push_back(vector<ll>(N+1,0));
                }

            }else{
                adj[u][v] = 1;
                adj[v][u] = 1;
            }
        } else {
            cerr << "Error: Failed to extract integers from the line." << endl;
        }
    }
    // vector for encoding the nodes with numbers
    vector<ll> X(N+1,0);
    for(ll i=0;i<=N;i++){
        X[i] = i;
    }
    // dp relation where dp table is encoded with numbers
    vector<vector<ll> > dp(N+1,vector<ll>(N+1,0));
    for (int i=0;i<=N;i++){
        for (int j=0;j<=N;j++){
            dp[i][j] = (N+1)*(i+1)+j;
        }
    }
    inputFile.close();
    // using binary search to find the maximum value of k
    ll low = 1;
    ll high = N;
    // command for calling minisat
    const char* command = "minisat sat_encoding.txt sat_output.txt"; 
    while (low<=high){
        ofstream outputFile("sat_encoding.txt",ofstream::trunc);
        if (!outputFile.is_open()) {
            cerr << "Error: Unable to open the file." << endl;
            return 1;
        }
        // here CNF form is output to sat_encoding.txt
        ll mid = low + (high-low)/2;
        outputFile<<"p cnf "<<N + (N+1)*(mid+1)<<" "<<((N)*(N-1))/2-E + 4*N*mid+N+mid+2<<endl;
        make_clause_dp(dp,X,N,mid,outputFile);
        make_clause_edges(adj,X,outputFile);
        int status = system(command);
        outputFile.close();
        ifstream sat_output("sat_output.txt");
        if (!sat_output.is_open()) {
            cerr << "Error: Unable to open the sat_output." <<endl;
            return 1;
        }
        string firstLine;
        getline(sat_output, firstLine);
        // check if k is statisfiable or not
        if (firstLine=="SAT"){
            low = mid+1;
        }else{
            high = mid-1;
        }
        sat_output.close();
    }
    ofstream outputFile("sat_encoding.txt",ofstream::trunc);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
        return 1;
    }
    // here CNF form is output to sat_encoding.txt for the answer got
    outputFile<<"p cnf "<<N + (N+1)*(high+1)<<" "<<((N)*(N-1))/2-E + 4*N*high+N+high+2<<endl;
    make_clause_dp(dp,X,N,high,outputFile);
    make_clause_edges(adj,X,outputFile);
    system(command);
    outputFile.close();
    ifstream sat_output("sat_output.txt");
    if (!sat_output.is_open()) {
        cerr << "Error: Unable to open the sat_output." <<endl;
        return 1;
    }
    string firstLine;
    getline(sat_output, firstLine);
    string secondLine;
    vector<ll> answer;
    getline(sat_output, secondLine);
    stringstream ss(secondLine);
    string token;
    while (getline(ss, token, ' ')) {
        if (stoi(token)>N){
            break;}
        if (stoi(token)>0){
            answer.push_back(stoi(token));
            }
        }
    sat_output.close();
    ofstream outputFile1(string(argv[1])+".mapping",ofstream::trunc);
    outputFile1<<"#1"<<endl;
    for(int i=0;i<answer.size();i++){
        if (i==answer.size()-1){
            outputFile1<<answer[i];
            break;
        }
        outputFile1<<answer[i]<<" ";
    }
    outputFile1<<endl;
    outputFile1.close();
    // k is statisfiable printing the answer
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    // cout << "Running time: " << duration.count() << " milliseconds" << endl;
}

