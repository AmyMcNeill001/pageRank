#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
using namespace std;

const string path = "//Users//amymcneill//Desktop//homework4//large_data.txt";

struct Node
{
    int position;
    double weight;
    double incoming;
    string siteName;
    vector<Node*> links;
    Node* next;
    
    Node();
    Node(string &inName, int &inPosition);
    void link(Node &linkedTo);
};//END Node declaration

Node::Node()
{
    position = -1;
    weight = 0;
    incoming = 0;
    siteName = "NOT A SITE";
    next = NULL;
}//END node default construction definition

Node::Node(string &inName, int &inPosition)
{
    position = inPosition;
    weight = 100;
    incoming = 0;
    siteName = inName;
    next = NULL;
}//END node constructor with parameters definition

void Node::link(Node &linkedTo)
{
    links.push_back(&linkedTo);
}//END link function definition

class Graph: public Node
{
public:
    Node* nodeTable;
    int nodeNumber;
    unordered_map<string, int>location;
    Graph(vector<string> &inNodes);
    void connectNodes(vector<pair<string, string>> &inLinks);
    bool update();
    void results();
};//END Graph declaration



Graph::Graph(vector<string> &inNodes)
{
    nodeNumber = inNodes.size();
    nodeTable = new Node[nodeNumber];
    
    for( int ii = 0; ii < nodeNumber; ii++)
    {
        string thisWord = inNodes.at(ii);
        location[thisWord] = ii;
        nodeTable[ii] = *new Node(thisWord, ii);
    }//END while loop to make first table
    
}//END Graph constructor definition

void Graph::connectNodes(vector<pair<string, string>> &inLinks)
{
    string firstString, secondString;
    
    for(int ii = 0; ii < inLinks.size(); ii++)
    {
        firstString = (inLinks.at(ii)).first;
        secondString = (inLinks.at(ii)).second;
        nodeTable[location[firstString]].link(nodeTable[location[secondString]]);
    }//END for loop to iterate through links
    
}//END connectNodes function definition

bool Graph::update()
{
    bool needToRepeat = true;
    
    for (int ii = 0; ii < nodeNumber; ii++)
    {
        if(nodeTable[ii].siteName != "NOT A SITE")
        {
            double temp = nodeTable[ii].weight / nodeTable[ii].links.size();

            for(int xx = 0; xx < nodeTable[ii].links.size(); xx++)
            {
                Node* transfer = nodeTable[ii].links.at(xx);
                transfer->incoming = transfer->incoming + temp;
            }//END FOR LOOP TO GIVE WEIGHTS AWAY
            
            nodeTable[ii].weight = 0.0;
        }//END if statement to ensure active node
    }//END for loop to tierate through nodes
    for (int ii = 0; ii < nodeNumber; ii++)
    {
        if(nodeTable[ii].siteName != "NOT A SITE")
        {
            nodeTable[ii].weight = nodeTable[ii].incoming;
            nodeTable[ii].incoming = 0.0;
        }//END if statement to make sure node exists
    }//END for loop to transfer values
    
    
    return needToRepeat;
}//END function update definition

void Graph::results()
{
    double winner = nodeTable[0].weight;
    int winnerNumber = 0;
    string winnerName = nodeTable[0].siteName;
    
    for (int ii = 1; ii < nodeNumber - 1; ii++)
    {
        if(nodeTable[ii].weight > winner)
        {
            winner = nodeTable[ii].weight;
            winnerNumber = ii;
        }//END if statement for larger weight
            
    }//END for loop to interate through all members MUST SCALE
    
    cout << "The most popular website is " << nodeTable[winnerNumber].siteName << "!" << endl;
    
}//END function results definition

int main()
{
    int nodeNumber, linkNumber;
    int firstLayer;
    vector<string> inNodes;
    vector<pair<string, string>> inLinks;
    ifstream inFile(path.c_str());
    
    inFile >> nodeNumber;
    if(nodeNumber < 26)
        firstLayer = nodeNumber;
    else
        firstLayer = 26;

    string temp;
    for (int ii = 0; ii < nodeNumber; ii++)
    {
        inFile >> temp;
        inNodes.push_back(temp);
    }//END for loop to read in the nodes
    
    
    Graph inGraph(inNodes);
    
    inFile >> linkNumber;
    pair<string, string> tempPair;
    string first, second;
    for (int ii = 0; ii < linkNumber; ii++)
    {
        inFile >> first >> second;
        tempPair = make_pair(first, second);
        inLinks.push_back(tempPair);
    }//END for loop to read in links
    
    inGraph.connectNodes(inLinks);
    
    double iterations = 775/nodeNumber + 1;
    for (int ii = 0; ii < iterations; ii++)
    {
        inGraph.update();
    }
    
    inGraph.results();
    
}//END function main


