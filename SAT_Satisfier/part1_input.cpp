#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
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
    int count = 0;
    for (ll i=0;i<=K;i++){
        for (ll j=0;j<=N;j++){
            if (i==K && j==N){
                count ++;
                outputFile<< string_literal(dp[i][j]) <<" 0" <<endl;}
            if (i==0 && j==0){
                count ++;
                outputFile<< string_literal(dp[i][j]) <<" 0" <<endl;
            }
            else if (i==0){
                count ++;
                outputFile<< string_literal(dp[i][j]) << " 0" <<endl;
            }
            else if (j==0){
                count ++;
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
                count += 4;
            }
        }
    }
    cout<<"Clauses by make_clause_dp "<<count<<endl;
}

// function to make clauses for edges in the graph
// make clauses for all the non-edges in the graph
void make_clause_edges(vector<vector<ll> > adj,vector<ll> X,vector<ll> Y,ofstream& outputFile){
    // int count =0;
    ll n = adj.size();
    for (ll i=1;i<n;i++){
        for (ll j=i+1;j<n;j++){
            if (adj[i][j]==0){
                // count +=2 ;
                outputFile<< string_literal(-X[i]) << " " << string_literal(-X[j]) <<" 0" <<endl;
                outputFile<< string_literal(-Y[i]) << " " << string_literal(-Y[j]) <<" 0" <<endl;
            }
        }
    }
    // cout<<"Clauses by make_clause_edges "<<count<<endl;

}

// function to make clauses for non-overlapping nodes
// count = 0;
void make_node_clause(vector<ll> X,vector<ll> Y, ofstream& outputFile){
    int count = 0;
    for(int i=1;i<X.size();i++){
        count ++;
        outputFile<< string_literal(-X[i]) << " " << string_literal(-Y[i]) << " 0" << endl;
    }
    cout<<"Clauses by make_node_edges "<<count<<endl;
}


int main(int argc, char* argv[]){
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
    ll N=0,E=0,K1=0,K2=0;
    string line;
    vector<vector<ll> > adj;
    bool get=false;
    while (getline(inputFile, line)) {
        ll u, v, k1, k2;
        // reading the input lines
        if (std::istringstream(line) >> u >> v >> k1 >> k2) {
            // cout<< u <<" "<< v << " " << k1 << " " << k2 <<endl;
            if (get==false){
                ofstream outputFile("input_N.txt",ofstream::trunc);
                N=u;
                outputFile<<N;
                E=v;
                K1=k1;
                K2=k2;
                get=true;
                for (int i=0;i<=N;i++){
                    adj.push_back(vector<ll>(N+1,0));
                }
            }
            else{
                cerr << "Error: Declared twice"<<endl;
            }
            }
        else if(std::istringstream(line) >> u >> v) {
                // cout<< u <<" "<< v <<endl;
                adj[u][v] = 1;
                adj[v][u] = 1;
            }
        else {
            cerr << "Error: Failed to extract integers from the line." << endl;
        }
    }
    // vector for encoding the nodes with numbers
    vector<ll> X(N+1,0);
    vector<ll> Y(N+1,0);
    for(ll i=0;i<=N;i++){
        X[i] = i;
    }
    for(ll i=N+1;i<=2*N+1;i++){
        Y[i-N-1] = i;
    }
    // dp relation where dp table is encoded with numbers
    vector<vector<ll> > dp_x(N+1,vector<ll>(N+1,0));
    vector<vector<ll> > dp_y(N+1,vector<ll>(N+1,0));
    ll curr = N+1+(N+1);
    for (int i=0;i<=N;i++){
        for (int j=0;j<=N;j++){
            dp_x[i][j] = curr++;
        }
    }
    for (int i=0;i<=N;i++){
        for (int j=0;j<=N;j++){
            dp_y[i][j] = curr++;
        }
    }
    inputFile.close();
    string outputname = string(argv[1])+".satinput";
    ofstream outputFile(outputname,ofstream::trunc);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
        return 1;
    }
    // here CNF form is output to sat_encoding.txt for the answer got
    outputFile<<"p cnf "<<(N+1)*(N+1)+2*N + 1 +(N+1)*(K2+1)<<" "<<4*(N)*(K1+K2) + 3*N + K1 + K2 + 4 + N*(N-1) - 2*E<<endl;
    make_node_clause(X,Y,outputFile);
    make_clause_dp(dp_x,X,N,K1,outputFile);
    // make_clause_dp(dp_y,X,N,K2,outputFile);
    // make_clause_dp(dp_x,Y,N,K1,outputFile);
    make_clause_dp(dp_y,Y,N,K2,outputFile);
    make_clause_edges(adj,X,Y,outputFile);
    // system(command);
    outputFile.close();
    // ifstream sat_output("sat_output.txt");
    // if (!sat_output.is_open()) {
    //     cerr << "Error: Unable to open the sat_output." <<endl;
    //     return 1;
    // }
    // string firstLine;
    // getline(sat_output, firstLine);
    // string secondLine;
    // vector<ll> answer;
    // getline(sat_output, secondLine);
    // stringstream ss(secondLine);
    // string token;
    // while (getline(ss, token, ' ')) {
    //     if (stoi(token)>N){
    //     break;}
    //     if (stoi(token)>0){
    //         answer.push_back(stoi(token));
    //     }
    // }
    // sat_output.close();
    // ofstream outputFile1("output.txt",ofstream::trunc);
    // outputFile1<<"#1"<<endl;
    // for(auto x : answer){
    //     outputFile1<<x<<" ";
    // }
    // outputFile1<<endl;
    // outputFile1.close();
    // k is statisfiable printing the answer
}
// 7 10 4 2
// 1 2
// 1 3
// 1 4
// 1 6
// 4 5
// 3 2
// 4 2
// 5 3
// 3 4
// 6 7

// 5 5 3 2
// 1 2
// 1 4
// 3 4
// 3 5
// 4 5