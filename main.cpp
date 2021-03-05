#include <iostream>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include<Windows.h>
#include <stack>
#include <map>
#include<queue>
#include<unordered_map>

using namespace std;

struct City {
    string name;
    map<string, int> directWay;
};

struct Rib {
public:
    City startCity;
    City endCity;
    int roadWay;
};

void initializeGraph(vector<City> &cityList, vector<Rib> &structRibs);

stack<string> greedyAlgorithm(int &finalDistance, vector<Rib> &structRibs, const string &start, const string &end);

int getAFunction(City currentCity, Rib myRib, int ancestorDistance, string end);

void printGreedyAlgorithm(stack<string> path, string start, string end, int distance);

vector<string> AStarAlgorithm(int &distance,vector<City> cityList, vector<Rib> &structRibs, const string &start, const string &end);
void printAAlgorithm(vector<string> path, string start, string end, int distance);

int main() {
    vector<City> cityList;
    vector<Rib> ribsList;
    initializeGraph(cityList, ribsList);
    int distance=0;
    cout<< "A* distance"<<endl;
             printAAlgorithm(AStarAlgorithm(distance,cityList, ribsList, cityList[1].name, cityList[5].name), cityList[1].name, cityList[5].name, distance);
cout<<endl;


    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (i == j)continue;
            int distance = 0;
            greedyAlgorithm(distance, ribsList, cityList[i].name, cityList[j].name);
            printGreedyAlgorithm(greedyAlgorithm(distance, ribsList, cityList[i].name, cityList[j].name),
                                 cityList[i].name, cityList[j].name, distance);

        }
    }

    return 0;
}

void initializeGraph(vector<City> &cityList, vector<Rib> &structRibs) {

    City myCity;
    Rib myRib{};
    string tmpString;
    int distance;

    ifstream inFile;
    inFile.open("GraphWithScales.txt");

    if (!inFile.is_open()) cout << "It is not open" << endl;
    for (int i = 0; i < 15; i++) {
        inFile >> tmpString;
        myCity.name = tmpString;
        cityList.push_back(myCity);
    }

    for (int i = 0; i < 15; i++) {
        inFile >> tmpString;
        for (int j = 0; j < 15; j++) {
            inFile >> distance;
            cityList[i].directWay.insert(pair<string, int>(cityList[j].name, distance));
        }
    }

    while (!inFile.eof()) {
        inFile >> tmpString;
        for (auto &i : cityList) {
            if (i.name == tmpString)myRib.startCity = i;
        }
        inFile >> tmpString;
        for (auto &i : cityList) {
            if (i.name == tmpString)myRib.endCity = i;
        }
        inFile >> distance;
        myRib.roadWay = distance;
        structRibs.push_back(myRib);
    }
    inFile.close();
}

stack<string> greedyAlgorithm(int &finalDistance, vector<Rib> &structRibs, const string &start, const string &end) {
    vector<bool> markedRibs(structRibs.size(), false);
    stack<string> cityStack;
    cityStack.push(start);
    while (cityStack.top() != end && !cityStack.empty()) {

        int minDistance = 10000000;
        string minPick;
        int addedDistance = 0;
        int markedRib = 0;

        for (int i = 0; i < structRibs.size(); i++) {

            if (cityStack.top() == structRibs[i].startCity.name && !markedRibs[i]) {
                if (structRibs[i].endCity.name == end) {
                    finalDistance += structRibs[i].roadWay;
                    cityStack.push(end);
                }
                if (structRibs[i].endCity.directWay.find(end)->second < minDistance) {
                    minDistance = structRibs[i].endCity.directWay.find(end)->second;
                    minPick = structRibs[i].endCity.name;
                    addedDistance = structRibs[i].roadWay;
                    markedRib = i;
                }
            }

            if (cityStack.top() == structRibs[i].endCity.name && !markedRibs[i]) {
                if (structRibs[i].startCity.name == end) {
                    finalDistance += structRibs[i].roadWay;
                    cityStack.push(end);
                }
                if (structRibs[i].startCity.directWay.find(end)->second < minDistance) {
                    minDistance = structRibs[i].startCity.directWay.find(end)->second;
                    minPick = structRibs[i].startCity.name;
                    addedDistance = structRibs[i].roadWay;
                    markedRib = i;


                }
            }
        }
        if (addedDistance == 0)cityStack.pop();
        else {
            cityStack.push(minPick);
            finalDistance += addedDistance;
        }
        markedRibs[markedRib] = true;
    }
    return cityStack;
}

vector<string> AStarAlgorithm(int &distance,vector<City> cityList, vector<Rib> &structRibs, const string &start, const string &end) {

    unordered_map<int, string> distanceFunction;
    unordered_map<string, int> reverseDistanceFunction;
    priority_queue<int, vector<int>, greater<>> myPriority;
    unordered_map<string, pair<int, string>> ancestor;
    unordered_map<string, bool> opened;
    for (int i = 0; i < 15; i++) {
        opened.insert(pair<string, bool>(cityList[i].name, false));
    }
    bool isEnd = false;

    for (int i = 0; i < cityList.size(); i++) {
        if (cityList[i].name == start) {
            myPriority.push(cityList[i].directWay.find(end)->second);
            distanceFunction.insert(pair<int, string>(cityList[i].directWay.find(end)->second, start));
            break;
        }
    }

    ancestor.insert(pair<string, pair<int, string>>(start, pair<int, string>(0, "")));
    opened[start] = true;
    while (!isEnd) {
        string currentCity = distanceFunction[myPriority.top()];
        int priority = myPriority.top();
        for (int i = 0; i < structRibs.size(); i++) {

            if (structRibs[i].startCity.name == currentCity && !opened[structRibs[i].endCity.name]) {
                opened[structRibs[i].endCity.name] = true;
                ancestor.insert(pair<string, pair<int, string>>(structRibs[i].endCity.name, pair<int, string>(
                        ancestor[structRibs[i].startCity.name].first + structRibs[i].roadWay,
                        structRibs[i].startCity.name)));
                int function = getAFunction(structRibs[i].endCity, structRibs[i],
                                            ancestor[structRibs[i].endCity.name].first, end);
                myPriority.push(function);
                distanceFunction.insert(pair<int, string>(function, structRibs[i].endCity.name));
                reverseDistanceFunction.insert(pair<string, int>(structRibs[i].endCity.name, function));
                if (structRibs[i].endCity.name == end)isEnd = true;
            }

            if (structRibs[i].endCity.name == currentCity && !opened[structRibs[i].startCity.name]) {
                opened[structRibs[i].startCity.name] = true;
                ancestor.insert(pair<string, pair<int, string>>(structRibs[i].startCity.name, pair<int, string>(
                        ancestor[structRibs[i].endCity.name].first + structRibs[i].roadWay,
                        structRibs[i].endCity.name)));
                int function = getAFunction(structRibs[i].startCity, structRibs[i],
                                            ancestor[structRibs[i].startCity.name].first, end);
                myPriority.push(function);
                distanceFunction.insert(pair<int, string>(function, structRibs[i].startCity.name));
                reverseDistanceFunction.insert(pair<string, int>(structRibs[i].startCity.name, function));
                if (structRibs[i].startCity.name == end)isEnd = true;
            }

        }
        if (priority == myPriority.top())break;
    }
    distance = ancestor[end].first;
    vector<string> path;
    string tmpAncestor = end;
    do {
        path.push_back(tmpAncestor);
        tmpAncestor = ancestor[tmpAncestor].second;

    } while (tmpAncestor != "");
return path;
}

int getAFunction(City cityToFind, Rib myRib, int ancestorDistance, string end) {
    int function = ancestorDistance + cityToFind.directWay.find(end)->second;
    return function;
}


void printGreedyAlgorithm(stack<string> path, string start, string end, int distance) {
    path.pop();
    cout << "distance between cities " << start << " and " << end << ": " << distance << " km" << endl;
    cout << "path: ";
    do {
        cout << path.top() << " ";
        path.pop();
    } while (!path.empty());
    cout << endl;
}

void printAAlgorithm(vector<string> path, string start, string end, int distance) {

    cout << "distance between cities " << start << " and " << end << ": " << distance << " km" << endl;
    cout << "path: ";
    for(int i=0; i<path.size(); i++) {
        cout << path[i] << " ";

    }
    cout << endl;
}