#include <iostream>
#include <chrono>
#include <vector>
#include <map>

using namespace std;

struct RunPoint {
    RunPoint(vector<int>& nums, int target,
                vector<int>(*solution)(vector<int> &, int target)):
        nums(nums), res(0), ms(0.0)
    {
        auto start = chrono::steady_clock::now();
        res=solution(nums, target);
        auto end   = chrono::steady_clock::now();
        chrono::duration<double, milli> dur = end-start;
        ms = dur.count();
    }
    vector<int>& nums;
    vector<int> res;
    double ms;
};

vector<int> twoSumSlow(vector<int>& nums, int target){
    vector<int> indexes;
    for(int i =0; i < nums.size(); i++){
        for(int j =i+1; j < nums.size(); j++){
            if(nums[i]+nums[j] == target) {
                indexes.insert(indexes.end(), {i,j});
                return indexes;
            }
        }
    } return indexes;
}

vector<int> twoSumFast(vector<int>& nums, int target){
    vector<int> indexes;
    map<int, int> diffs;
    for(int i=0; i < nums.size(); i++){
        int diff = target-nums[i];
        if(diffs.find(diff) != diffs.end()){
            indexes.insert(indexes.end(), {diffs.find(diff)->second, i});
            return indexes;
        }
        diffs.insert(pair<int, int>(nums[i], i));
    }
    return indexes;
}

vector<int> v[5];
int answers[]={18,22,36,3,66};
static int SIZE = 5;
void init_vectors(){
    v[0].insert(v[0].end(), {2,7,11,15});
    v[1].insert(v[1].end(), {2,7,11,15,11});
    v[2].insert(v[2].end(), {2,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,20,15,21});
    v[3].insert(v[3].end(), {2,7,11,15,7,7,7,7,77,7,7,7,7,77,7,7,7,77,7,1});
    v[4].insert(v[4].end(), {2,7,11,15,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,12,21,42,62,4});
}


vector<RunPoint> runSums(vector<int>(*solution)(vector<int> &, int target)) {
    vector<RunPoint> points;
    int i = SIZE;
    while (i--) {
        points.push_back(RunPoint(v[i],
                    answers[i],
                    solution));
    }
    return points;
}

int getPointsInInterval(const vector<RunPoint> points, double tMin, double tMax)
{
    int count = 0;
    for (vector<RunPoint>::const_iterator cit = points.begin(); cit != points.end(); ++cit) {
        if (cit->ms >= tMin && cit->ms <= tMax)
            ++count;
    }
    return count;
}

int main() {
    init_vectors();
    vector<RunPoint> rFast= runSums(twoSumFast);
    vector<RunPoint> rSlow= runSums(twoSumSlow);
    for(int i =0; i < 10; i++){
        printf("Within %f ms: Fast -> %d\t Slow -> %d\n", 
                        0.01-i*0.001,
                        getPointsInInterval(rFast,0.0,(double)0.01-i*0.001),
                        getPointsInInterval(rSlow,0.0,(double)0.01-i*0.001));
    }
    return 0;
}
