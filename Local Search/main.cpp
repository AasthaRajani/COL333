#include <cstdlib>
#include "new_start.cpp"
#include <time.h>
#include<iostream>
#include<fstream>
using namespace std;
int main(int argc, char** argv)
{

    //  // Parse the input.
    // if ( argc < 3 )
    // {   
    //     cout<<"Missing arguments\n";
    //     cout<<"Correct format : \n";
    //     cout << "./main <input_filename> <output_filename>";
    //     exit ( 0 );
    // }
    // string inputfilename ( argv[1] );
    // string outputfilename ( argv[2] );
    
    // SportsLayout *s  = new SportsLayout( inputfilename );
    // s->compute_allocation();
    // s->write_to_file(outputfilename);

    // long long cost = s->cost_fn ();
    // cout<< "cost:"<<cost<<endl;
    chrono::steady_clock::time_point start=chrono::steady_clock::now();
    // int total_locations=3;
    // int total_zones = 2;
    // int time_bound = 1;
    // long long t[3][3]={{0,3,1},{3,0,1},{1,1,0}};
    // long long f[2][2]={{1,2},{2,3}};
    // ==============================================================================================
    // int total_locations=5;
    // int total_zones = 3;
    // int time_bound = 2;
    // long long t[5][5]={{0,3,2,1,4},{3,0,2,2,5},{2,2,0,1,6},{1,2,1,0,2},{4,5,6,2,0}};
    // long long f[3][3]={{1,1,2},{3,1,4},{3,2,2}};
    // ===================================================================================================
    // int total_locations = 6;
    // int total_zones = 3;
    // int time_bound = 2;
    // long long f[3][3] = {{1,4,2},{5,1,4},{0,2,2}};
    // long long t[6][6] = {{0,4,2,1,4,1},{4,0,3,2,5,2},{2,3,0,1,3,3},{1,2,1,0,2,5},{4,5,3,2,0,1},{1,2,3,5,1,0}};
// ============================================================================================================================
    // int total_locations = 4;
    // int total_zones = 3;
    // int time_bound = 2;
    // long long f[3][3] = {{2,5,3},{2,4,1},{3,4,5}};
    // long long t[6][6] = {{0,3,4,1},{3,0,4,1},{4,4,0,8},{1,1,8,0}};

    // long long **freq = new long long *[total_zones];
    // long long **time = new long long *[total_locations];

    // for (int i = 0; i < total_zones; i++) {
    //     freq[i] = new long long[total_zones];
    //     for (int j = 0; j < total_zones; j++) {
    //         freq[i][j] = f[i][j];
    //     }
    // }

    // for (int i = 0; i < total_locations; i++) {
    //     time[i] = new long long[total_locations];
    //     for (int j = 0; j < total_locations; j++) {
    //         time[i][j] = t[i][j];
    //     }
    // }

    // State *state=new State("input7.txt");
    // // state->readInInputFile("input.txt");
    // cout<<state->cost_fun()<<endl;
    // state->compute_allocation(start);
    // state->write_to_file("output.txt");
    // bool check = state->check_output_format();
    // state->readOutputFile("output.txt");
    // cout<<"done"<<endl;
    // cout<<state->cost_fun()<<endl;
    // // cout<<state->get_location(2)<<endl;
    // // for(int i=0;i<total_zones;i++){
    // //     cout<<state->get_location(i+1)<<" ";
    // // }
    // // state->set_zone();
    // // cout<<state->flash_score(1,4)<<endl;
    // return 0;
         // Parse the input.
    if ( argc < 3 )
    {   
        cout<<"Missing arguments\n";
        cout<<"Correct format : \n";
        cout << "./main <input_filename> <output_filename>";
        exit ( 0 );
    }
    string inputfilename ( argv[1] );
    string outputfilename ( argv[2] );
    
    State *s  = new State( inputfilename );
    s->compute_allocation(start);
    s->write_to_file(outputfilename);
    
    long long cost = s->cost_fun ();
    cout<< "cost:"<<cost<<endl;


    return 0;


}