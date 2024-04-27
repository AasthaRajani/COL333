#include<random>
#include<cmath>
#include <chrono>
#include "stdlib.h"
#include<cstdlib>
#include <algorithm> 
#include<iostream>
#include<ctime>
#include<fstream>
#include <bits/stdc++.h>
using namespace std;
class State{
    private:
        long long **time_mat;
        long long **freq;
        int *zones;
        float time_bound;
        int total_zones;
        int *locations;
        int total_locations;
        int max_swaps_to_try = 10;  // Initial value
        int successful_swaps = 0;   // Counter for successful swaps
        const int ADJUST_INTERVAL = 10;  // Adjust after every 10 iterations
        const double ADJUST_FACTOR = 0.10;  // Adjust by 10%
    public:
        // State(int total_locations,int total_zones,int time_bound);
        State(string filename);
        int get_location(int zone);
        int get_zone(int location);
        void init_state(chrono::steady_clock::time_point start);
        long long flash_score(int location1, int location2);
        long long neighbour(chrono::steady_clock::time_point start);
        long long cost_fun();
        void helper(chrono::steady_clock::time_point start);
        bool is_exceed(chrono::steady_clock::time_point start,double factor);
        void compute_allocation(chrono::steady_clock::time_point start);
        void readInInputFile(string inputfilename);
        bool check_output_format();
        void readOutputFile(string output_filename);
        void write_to_file(string outputfilename);

};
// State::State(int total_locations,int total_zones,int time_bound){
//     this->total_locations=total_locations;
//     this->total_zones=total_zones;
//     this->time_bound=time_bound;
//     this->zones= (int *) malloc (sizeof(int) * this->total_locations);
//     this->locations=(int *) malloc (sizeof(int) * this->total_locations);
//     for(int i=0;i<this->total_zones;i++){
//         this->zones[i]=i+1;
//         this->locations[i]=i+1;
//     }
//     for(int j=this->total_zones;j<this->total_locations;j++){
//         this->zones[j]=-1;
//         this->locations[j]=-1;
//     }
// }
void State::readInInputFile(string inputfilename)
{
    fstream ipfile;
    ipfile.open(inputfilename, ios::in);
    if (!ipfile) {
        std::cout<<"No such file\n";
        std::exit( 0 );
    }
    else {
        ipfile >> this->time_bound;
        ipfile >> this->total_zones;
        ipfile >> this->total_locations;
        if(total_zones>total_locations){
            cout<<"Number of zones more than locations, check format of input file\n";
            exit(0);
        }
        long long **tempT;
        long long **tempN;
        tempT = new long long*[total_locations];
        for (int i = 0; i < total_locations; ++i)
        tempT[i] = new long long[total_locations];
        tempN = new long long*[total_zones];
        for (int i = 0; i < total_zones; ++i)
            tempN[i] = new long long[total_zones];
        for(int i=0;i<total_zones;i++){
            for(int j=0;j<total_zones;j++)
            ipfile>>tempN[i][j];
        }
        for(int i=0;i<total_locations;i++){
            for(int j=0;j<total_locations;j++)
            ipfile>>tempT[i][j];
        }
    ipfile.close();
    this->time_mat= tempT;
    this->freq= tempN;
    }

}
State:: State(string filename){
    readInInputFile(filename);
    this->zones= (int *) malloc (sizeof(int) * this->total_locations);
    this->locations=(int *) malloc (sizeof(int) * this->total_locations);
// ============================================MODIFIED CODE===================================================================================


    std::vector<int> total_frequency(this->total_zones, 0);

    for(int i = 0; i < this->total_zones; i++) {
        for(int j = 0; j < this->total_zones; j++) {
            total_frequency[i] += this->freq[i][j];
        }
    }

    // Calculate the "centrality" score for each location
    std::vector<double> centrality_score(this->total_locations, 0.0);
    for(int i = 0; i < this->total_locations; i++) {
        for(int j = 0; j < this->total_locations; j++) {
            centrality_score[i] += time_mat[i][j]; // Lower is better
        }
    }

    // Sort zones and locations based on frequency and centrality respectively
    std::vector<int> zone_order(this->total_zones), location_order(this->total_locations);
    std::iota(zone_order.begin(), zone_order.end(), 0);
    std::iota(location_order.begin(), location_order.end(), 0);

    std::sort(zone_order.begin(), zone_order.end(), [&total_frequency](int a, int b) {
        return total_frequency[a] > total_frequency[b];
    });

    std::sort(location_order.begin(), location_order.end(), [&centrality_score](int a, int b) {
        return centrality_score[a] < centrality_score[b];
    });

    // Assign zones to locations
    // std::vector<int> zone_to_location(total_locations) , 
    for(int k = 0;k<this->total_locations;k++){
        this->zones[k]=-1;
        this->locations[k]=-1;
    }
    for(int i = 0; i < zone_order.size(); i++) {
        this->locations[zone_order[i]] = location_order[i]+1;
        this->zones[location_order[i]] = zone_order[i]+1;
    }
    




// ============================================================================================================================================
    // for(int i=0;i<this->total_zones;i++){
    //     this->zones[i]=i+1;
    //     this->locations[i]=i+1;
    // }
    // for(int j=this->total_zones;j<this->total_locations;j++){
    //     this->zones[j]=-1;
    //     this->locations[j]=-1;
    // }

}
bool State::is_exceed(chrono::steady_clock::time_point start,double factor){
    chrono::steady_clock::time_point end=chrono::steady_clock::now();
    std::chrono::steady_clock::duration elapsed = end - start;
    int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    return milliseconds>=this->time_bound*60*1000*factor;
}
int State::get_location(int zone){
    return this->locations[zone-1];
}
int State::get_zone(int location){
    return this->zones[location-1];
}

void State::init_state(chrono::steady_clock::time_point start){
    srand(time(nullptr));
    int i1=rand()%(this->total_locations);
    int i2=rand()%(this->total_locations);
    long long diff=0;
    while(true){
        // cout<<"Yes in"<<endl;
        if(is_exceed(start,0.099898)){
            // cout<<"break"<<endl;
            break;
        }
        if(this->zones[i1]==-1 && this->zones[i2]==-1){
            // cout<<"free location"<<endl;
            i1=rand()%(this->total_locations);
            i2=rand()%(this->total_locations);
            continue;
        }
        // cout<<"outside if"<<endl;
        diff= this->flash_score(i1+1,i2+1);
        // cout<<"diff "<<diff<<endl;
        if(diff<0){
            // cout<<"loc: "<<locations[zones[i1]-1]<<" "<<locations[zones[i2]-1]<<" "<<endl;
            if(this->zones[i1]!=-1 && this->zones[i2]!=-1){
                swap(this->locations[this->zones[i1]-1],this->locations[this->zones[i2]-1]);
                swap(this->zones[i1],this->zones[i2]);
            }
            else if(this->zones[i2]==-1){
                this->locations[this->zones[i1]-1]=i2+1;
                swap(this->zones[i1],this->zones[i2]);
            }
            else if(this->zones[i1]==-1){
                this->locations[this->zones[i2]-1]=i1+1;
                swap(this->zones[i1],this->zones[i2]);
            }
            // cout<<"please break"<<endl;
            break;
        }
        else{
            // cout<<"in random"<<endl;
            i1=rand()%(this->total_locations);
            i2=rand()%(this->total_locations);
        }
    }
    // cout<<"Yes I am broken"<<endl;
}
long long State::flash_score(int location1,int location2){
    //  if (location1 == location2) return 0;

    // long long original_cost = 0;
    // long long swapped_cost = 0;
    
    // int zone1 = this->zones[location1-1]-1;
    // int zone2 = this->zones[location2-1]-1;
    // // cout<<"zones are "<<zone1<<" "<<zone2<<endl;
    // // Calculate cost difference for paths involving loc1
    // for (int i = 0; i < this->total_zones; i++) {
    //     // Ignore if i is same as zone1 or zone2
    //     // cout<<"inside the loop"<<endl;
    //     if (i == zone1 || i == zone2) continue;

    //     if(zone1>=0){        // Cost in original position
    //     original_cost += (long long)this->freq[zone1][i] * (long long)this->time_mat[location1-1][this->locations[i]-1];
    //     original_cost += (long long)this->freq[i][zone1] * (long long)this->time_mat[this->locations[i]-1][location1-1];

    //     // Cost in swapped position
    //     swapped_cost += (long long)this->freq[zone1][i] * (long long)this->time_mat[location2-1][this->locations[i]-1];
    //     swapped_cost += (long long)this->freq[i][zone1] * (long long)this->time_mat[this->locations[i]-1][location2-1];}
    // }

    // // Calculate cost difference for paths involving loc2
    // for (int i = 0; i < this->total_zones; i++) {
    //     // Ignore if i is same as zone1 or zone2
    //     if (i == zone1 || i == zone2) continue;
    //     if(zone2>=0){
    //     original_cost += (long long)this->freq[zone2][i] * (long long)this->time_mat[location2-1][this->locations[i]-1];
    //     original_cost += (long long)this->freq[i][zone2] * (long long)this->time_mat[this->locations[i]-1][location2-1];

    //     // Cost in swapped position
    //     swapped_cost += (long long)this->freq[zone2][i] * (long long)this->time_mat[location1-1][this->locations[i]-1];
    //     swapped_cost += (long long)this->freq[i][zone2] * (long long)this->time_mat[this->locations[i]-1][location1-1];}
    // }

    // // Return the difference in cost
    // // cout<<"original cost is "<<original_cost<<endl;
    // // cout<<"swapped cost is "<<swapped_cost<<endl;
    // return swapped_cost - original_cost;
    int z1=this->zones[location1-1];
    int z2=this->zones[location2-1];
    // cout<<z1<<" "<<z2<<endl;
    long long ans=0;
    for(int i1=0;i1<this->total_zones;i1++){
        // cout<<i1<<endl;
        long long f1=0;
        long long f2=0;
        long long g1=0;
        long long g2=0;
        if(i1!=z1-1 && i1!=z2-1){
            if(z1>0){
                f1=this->freq[i1][z1-1] * this->time_mat[this->locations[i1]-1][location1-1] + this->freq[z1-1][i1] * this->time_mat[location1-1][this->locations[i1]-1];
                g1=this->freq[i1][z1-1] * this->time_mat[this->locations[i1]-1][location2-1] + this->freq[z1-1][i1] * this->time_mat[location2-1][this->locations[i1]-1];
            }
            if(z2>0){
                f2=this->freq[i1][z2-1] * this->time_mat[this->locations[i1]-1][location2-1] + this->freq[z2-1][i1] * this->time_mat[location2-1][this->locations[i1]-1];
                g2=this->freq[i1][z2-1] * this->time_mat[this->locations[i1]-1][location1-1] + this->freq[z2-1][i1] * this->time_mat[location1-1][this->locations[i1]-1];
            }
        } 
        // cout<<f1<<" "<<f2<<" "<<g1<<" "<<g2<<endl;
        ans = ans - f1 - f2 + g1 + g2;
    }
    return ans;
}
// ---------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------Old Neighbour----------------------------------------------------------------------------
long long State::neighbour(chrono::steady_clock::time_point start){
    long long diff=0;
    int attempts =0;
    while(attempts<this->max_swaps_to_try){
        int i = rand()% this->total_locations;
        int j = rand()% this->total_locations;
        // cout<<"First index "<<i<<" Second index "<<j<<endl;
        if(this->zones[i]==-1 && this->zones[j]==-1){
                continue;
            }
            diff=this->flash_score(i+1,j+1);
            // cout<<"Diff is---------->>  "<<diff<<endl;
            if(diff<0){
                this->successful_swaps++;
                if(this->zones[i]!=-1 && this->zones[j]!=-1){
                    swap(this->locations[this->zones[i]-1],this->locations[this->zones[j]-1]);
                    swap(this->zones[i],this->zones[j]);
                }
                else if(this->zones[j]==-1){
                    this->locations[this->zones[i]-1]=j+1;
                    swap(this->zones[i],this->zones[j]);
                }
                else if(this->zones[i]==-1){
                    this->locations[this->zones[j]-1]=i+1;
                    swap(this->zones[i],this->zones[j]);
                }
                // fp=true;
                break;
            }
            if(this->is_exceed(start,1)){
                // cout<<"from neighbour"<<endl;
                break;
            }
            attempts++;

    }
    if(diff>=0){
        long long original=this->cost_fun();
        // cout<<"Original is "<<original<<endl;
        long long diff2=0;
        while(true){
            if(is_exceed(start,0.8888999)){
                // cout<<"from random"<<endl;
                break;
            }
            int *c=(int *) malloc (sizeof(int) * this->total_locations); //memory allot nhi hua
            int *c2=(int *) malloc (sizeof(int) * this->total_locations);
            for(int x1=0;x1<this->total_locations;x1++){
                c[x1]=this->zones[x1];
                c2[x1]=this->locations[x1];
            }
            random_shuffle(this->zones, this->zones+this->total_locations);
            for(int k=0;k<this->total_locations;k++){
                if(this->zones[k]!=-1){
                    this->locations[this->zones[k]-1]=k+1;
                } 
            }
            for(int k2=this->total_zones;k2<this->total_locations;k2++){
                this->locations[k2]=-1;
            }
            diff2=this->cost_fun();
            if(diff2>=original){
                free(this->zones);
                free(this->locations);
                this->zones=c;
                this->locations=c2;
            }
            else{
                // cout<<"from here"<<endl;
                free(c);
                free(c2);
                break;
            }
        }

    }
    static int iteration_counter = 0;
    iteration_counter++;

    if (iteration_counter % this->ADJUST_INTERVAL == 0) {
        if (this->successful_swaps > this->ADJUST_INTERVAL / 2) {  // More than half were successful
            this->max_swaps_to_try += static_cast<int>(this->max_swaps_to_try * this->ADJUST_FACTOR);
        } else {
            this->max_swaps_to_try -= static_cast<int>(this->max_swaps_to_try * this->ADJUST_FACTOR);
        }
        this->successful_swaps = 0;  // Reset for next interval
    }
    return this->cost_fun();
}
// -------------------------------------------------------------------------------------------------------------------------------------------
// -------------------------------Old Neighbour code--------------------------------------------------------------------------------------------





// --------------------------------------------------------New Neighbour Code----------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------



// long long State::neighbour(chrono::steady_clock::time_point start){
//     // cout<<"in"<<endl;
//     long long diff=0;
//     bool fp=false;
//     while(!this->is_exceed(start,0.8899)){
//         int i = rand() % this->total_locations;
//         int j = rand() % this->total_locations;
//         if(this->zones[i]==-1 && this->zones[j]==-1){
//             continue;
//         }
//         diff=this->flash_score(i+1,j+1);
//         // cout<<"Diff is "<<diff<<endl;
//         if(diff<0){
//             if(this->zones[i]!=-1 && this->zones[j]!=-1){
//                 swap(this->locations[this->zones[i]-1],this->locations[this->zones[j]-1]);
//                 swap(this->zones[i],this->zones[j]);
//             }
//             else if(this->zones[j]==-1){
//                 this->locations[this->zones[i]-1]=j+1;
//                 swap(this->zones[i],this->zones[j]);
//             }
//             else if(this->zones[i]==-1){
//                 this->locations[this->zones[j]-1]=i+1;
//                 swap(this->zones[i],this->zones[j]);
//             }
//             // cout<<"in if"<<endl;
//             break;
//         }
//         if(is_exceed(start,1)){
//             // cout<<"there"<<endl;
//             break;
//         }
//     }
//     if(diff>=0){
//         long long original=this->cost_fun();
//         long long diff2=0;
//         while(true){
//             if(is_exceed(start,0.8888999)){
//                 break;
//             }
//             // auto seed = chrono::system_clock::now().time_since_epoch().count();
//             // default_random_engine engine(seed);
//             int *c=(int *) malloc (sizeof(int) * this->total_locations); //memory allot nhi hua
//             int *c2=(int *) malloc (sizeof(int) * this->total_locations);
//             for(int x1=0;x1<this->total_locations;x1++){
//                 c[x1]=this->zones[x1];
//                 c2[x1]=this->locations[x1];
//             }
//             random_shuffle(this->zones, this->zones+this->total_locations);
//             for(int k=0;k<this->total_locations;k++){
//                 if(this->zones[k]!=-1){
//                     this->locations[this->zones[k]-1]=k+1;
//                 } 
//             }
//             for(int k2=this->total_zones;k2<this->total_locations;k2++){
//                 this->locations[k2]=-1;
//             }
//             diff2=this->cost_fun();
//             // cout<<"Diff is "<<diff2<<endl;
//             // cout<<" aastha"<<endl;
//             if(diff2>=original){
//                 this->zones=c;
//                 this->locations=c2;
//             }
//             else{
//                 free(c);
//                 free(c2);
//                 break;
//             }
            

//         }
    
//     }
//     return this->cost_fun();
// }
// --------------------------------------------------------New neighbour Code---------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------

long long State::cost_fun(){
    long long cost=0;
    for(int i=0;i<this->total_zones;i++)
    {
        for(int j=0;j<this->total_zones;j++) 
        {
            cost+=(long long)this->freq[i][j]*(long long)this->time_mat[this->locations[i]-1][this->locations[j]-1];
        }
    }
    return cost;
}
void State::helper(chrono::steady_clock::time_point start){
    long long global_max=this->cost_fun();
    // cout<<"ORIGINAL COT IS ------------------------------------------------------------------------------"<<global_max<<endl;
    long long d=0;
    while(true){
        d = this->neighbour(start);
        // for(int i=0;i<this->total_locations;i++){
        //     cout<<"LOCATION OF ZONE "<<i+1<<"is: "<<this->locations[i]<<"ZONE AT LOCATION "<<i+1<<"is: "<<this->zones[i]<<endl;
        // }
        if(is_exceed(start,1)){
            break;
        }
        if(d<global_max){
            // cout<<"in update"<<endl;
            // cout<<d<<endl;
            global_max = d;
        }
    }
    // cout<<"FINAL--------------------------------------------"<<global_max<<endl;
}
void State::compute_allocation(chrono::steady_clock::time_point start){
    while(true){
        // cout<<"start loop"<<endl;
        this->init_state(start);
        // cout<<this->cost_fun()<<endl;
        if(is_exceed(start,0.0099898)){
            // cout<<"finish"<<endl;
            break;
        }
        // cout<<"Inside while"<<endl;
    }
    // this->init_state(start);
    // cout<<"sfter init "<<this->cost_fun()<<endl;
    this->helper(start);
}
bool State::check_output_format()
{

    vector<bool> visited(this->total_locations,false);
    for(int i=0;i<this->total_zones;i++)
    {
        if((this->locations[i]>=1 && this->locations[i]<=this->total_locations))
        {
            if(!visited[this->locations[i]-1])
            visited[this->locations[i]-1]=true;
            else
            {
                cout<<"Repeated locations, check format\n";
                return false;
            }
        }
        else
        {
            cout<<"Invalid location, check format\n";
            return false;
        }
    }

    return true;

}

void State::readOutputFile(string output_filename)
{
        fstream ipfile;
        ipfile.open(output_filename, ios::in);
        if (!ipfile) {
            cout << "No such file\n";
            exit( 0 );
        }
        else {
            
            vector<int> ip;
            while (1) {
                int t;
                ipfile >> t;
                ip.push_back(t);
                if (ipfile.eof())
                    break;
                
            }
        
        if(ip.size()!=this->total_zones+1)
        {
            cout<<ip.size()<<endl;
            cout<<"number of values not equal to number of zones, check output format\n";
            exit(0);
        }
        for(int i=0;i<this->total_zones;i++)
        this->locations[i]=ip[i];
    ipfile.close();

    if(!check_output_format())
        exit(0);
    cout<<"Read output file, format OK"<<endl;

        }
    
}
void State::write_to_file(string outputfilename){

        // Open the file for writing
ofstream outputFile(outputfilename);

// Check if the file is opened successfully
if (!outputFile.is_open()) {
    cerr << "Failed to open the file for writing." << std::endl;
    exit(0);
}

for(int i=0;i<this->total_zones;i++)
outputFile<<this->locations[i]<<" ";

// Close the file
outputFile.close();

cout << "Allocation written to the file successfully." << endl;

}