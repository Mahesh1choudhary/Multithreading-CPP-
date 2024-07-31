
/*
Question link - https://leetcode.com/discuss/interview-question/object-oriented-design/339625/rubrik-key-value-store-with-snapshot

Build a key-value data structure that allows the user to take a snapshot of the data. The user can read the key-value store from any snapshot.

Structure has the normal key/value like methods plus something like

snapshot = dataStructure.takeSnapshot();
value = dataStructure.get(key, snapshot);
void dataStructure.deleteSnapshot();


*/



#include <iostream>
#include <bits/stdc++.h>
using namespace std;

class dataStructure{
    map<int,int> mp;    // key,value
    map<int, map<int,int>> snapshots;
    int snapshot_no=0;
    
    public:
    
    void put(int key, int value){
        mp[key]=value;  // if key already exists, then value is updated
    }
    
    void takeSnapshot(){
        snapshot_no++;
        snapshots[snapshot_no]=mp;
    }
    
    int get(int key, int snap_id){
        if(snapshots.count(snap_id)==0 || snapshots[snap_id].count(key)==0){
            return -1;
        }
        return snapshots[snap_id][key];
    }
    
    void delete_snapshot(int snap_id){
        mp.erase(snap_id);
    }
    
    
};





int main()
{
    

    return 0;
}
