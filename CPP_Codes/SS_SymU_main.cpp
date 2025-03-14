#include <iostream>
#include <vector>
#include <utility>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <queue>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include <tuple>
#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include <cassert>

using namespace std;
using namespace std::chrono;


int choice, B, gammaa, initialB;
vector<pair<int, int>> L_graph,L_graph2, CommonC, UnboundedC;

typedef tuple<int, list<int>, int> ActionGraphItem;
typedef vector<ActionGraphItem> ActionGraph;
ActionGraph actionGraph;

struct Edge {
    int source;
    string label;
    int destinations;
};
int sum(int a, int b, int BB){
    return ((a + b) % BB);
}

int inc2(int a, int BB) {
    return sum(a, 2,BB);
}
int subtract(int a, int b, int BB){
    return ((a - b + B) % BB);
}

int inc(int a, int BB) {
    return sum(a, 1,BB);
}


int dec(int a, int BB){
    return ((a + BB - 1) % BB);
}

int MSum(int a,int b, int BB){
    if ((b == BB - 1)&& (a==0)) {
        return (a + BB);
    }else{
        return (a);
    }
    
}

//The desired protocol for L(xi_1, xi) or L(Pxi, xi)
bool L(int xi_1, int xi,int choice, int BB) {
    switch (choice) {
        case 1:
            return (((subtract(xi_1, xi,BB) % 2) == 0) && (xi % 2 != 0)); // OddParity1
        case 2:
            return (sum(xi_1, xi,BB) % 2 == 0); // SumNotEven
        case 3:
            return (xi_1 == xi); // Agreement
        case 4:
            return (subtract(xi_1, xi,BB) % 2 == 0); // Parity
        case 5:
            return (xi_1 <= xi); // Sorting
        case 6:
            return ((xi_1 == xi) || (xi_1 == inc(xi,BB))); // NearAgreement
        case 7:
            return ((xi == inc2(xi_1,BB)) && (xi_1 % 2 != 0)); // OddParity2
        case 8:
            return ((xi == inc2(xi_1,BB)) && (xi_1 % 2 == 0)); // evenParity        
        case 9:
            return (xi == inc(xi_1,BB)); // k-coloring
        case 10:
            return (xi_1 < MSum(xi,xi_1,BB)); // Sorting2    
        default:
            cout << "Error: Invalid choice." << endl;
            return false;
    }
} 

string NegL(int choice) {
    switch (choice) {
        case 1:
            return ("(|Xi_1 - Xi| % 2 != 0) || (Xi % 2 == 0)");
        case 2:
            return ("(Xi_1 + Xi) % 2 != 0");
        case 3:
            return ("Xi_1 != Xi");
        case 4:
            return ("|Xi_1 - Xi| % 2 != 0");
        case 5:
            return ("Xi_1 > Xi");
        case 6:
            return ("(Xi_1 != Xi) && (Xi_1 != Xi + 1)");
        case 7:
            return ("(Xi != X_pi + 2) || ((X_pi % 2) == 0)");    
        case 8:
            return ("(Xi != X_pi + 2) || ((X_pi % 2) != 0)");
        case 9:
            return ("Xi != X_pi + 1");
        case 10:
            return ("Xi <= X_pi");    
        default:
            assert(false && "Invalid choice"); 
            return "Invalid"; 
    }
}

bool CheckSelfLoop(int B, int choice) {
    bool hasSelfLoop = false;

    for (int M = 2; M <= B; ++M) {
        hasSelfLoop = false; 

        for (int i = 0; i < M; ++i) {
            if (L(i, i, choice,B)) { 
                hasSelfLoop = true;
                break; 
            }
        }

        if (!hasSelfLoop) {
            hasSelfLoop = false;
        }
    }

    return hasSelfLoop; 
}

vector<pair<int, int>> LGraph(int B, vector<pair<int, int>>& L_graph, int choice) {
     for (int i = 0; i < B; ++i) {
            for (int j = 0; j < B; ++j) {
                if (L(i, j, choice,B)) {
                    L_graph.push_back(make_pair(i, j));
                }
            }
        }
        
    return L_graph;
}

void PrintLGraph(vector<pair<int, int>> L_graph2){
    cout << "Locality graph (for M = 10): {";
        for (const auto& pair : L_graph2) {
            cout << "(" << pair.first << ", " << pair.second << ")";
            if (&pair != &L_graph2.back()) {
                cout << ", ";
            }
        }
        cout << "}" << endl;
}

bool FindCycle(const vector<pair<int, int>>& L_graph) {
    if (L_graph.size() < 2) {
        return false;
    }
    int firstElement = L_graph.front().first;
    int lastElement = L_graph.back().second;
    return (firstElement == lastElement);
    cout << "The graph has a cycle." << endl;
}

string Even(int choice) {
    switch (choice) {
        case 7:
            return (" && ((Xpi % 2) != 0) ");
        case 8:
            return (" && ((Xpi % 2) == 0) ");
        default:  
            return ("");  
    }
}                
string Odd(int choice) {
    switch (choice) {
        case 7:
            return (" && ((Xpi % 2) == 0) ");
        case 8:
            return (" && ((Xpi % 2) != 0) ");
        default:  
            return ("");  
    }
}

bool IsCycleFixedOrIncreasing(const vector<pair<int, int>>& L_graph, int B) {
    vector<int> cycleSizes;

    // Iterate through the range to find cycles for different values of M
    for (int M = 2; M <= B; ++M) {
        int cycleSize = 0;
        // Check if a cycle exists for the given M
        if (FindCycle(L_graph)) {
            cycleSizes.push_back(cycleSize);
        } else {
            // If no cycle exists for a given M, return false
            return false;
        }
    }

    // If cycles are found, analyze their properties
    if (!cycleSizes.empty()) {
        // Check if all cycles have the same size (fixed cycle)
        bool isFixed = true;
        for (size_t i = 1; i < cycleSizes.size(); ++i) {
            if (cycleSizes[i] != cycleSizes[0]) {
                isFixed = false;
                break;
            }
        }

        if (isFixed) {
            return false; // Fixed cycles should return false
        }

        // Check if the cycle sizes are strictly increasing
        for (size_t i = 1; i < cycleSizes.size(); ++i) {
            if (cycleSizes[i] <= cycleSizes[i - 1]) {
                return false; // Not strictly increasing
            }
        }

        return true; // Strictly increasing cycles
    }

    // Default return false if no cycles are found
    return false;
}

string listToString1(const list<int>& lst) {
    string result;
    for (int val : lst) {
        result += to_string(val) + "|";
    }
   
    if (!result.empty()) {
        result.pop_back();
    }
    return result;
}

vector<Edge> convertToEdges1(const ActionGraph& actionGraph) {
    vector<Edge> edges;

    for (const auto& item : actionGraph) {
        Edge edge;
        edge.source = get<0>(item);
        edge.label = listToString1(get<1>(item));
        edge.destinations = get<2>(item);
        edges.push_back(edge);
    }

    return edges;
}

void drawGraph(const vector<Edge>& edges, int choice) {
    ofstream dotFile("graph.dot");
    dotFile << "digraph G {" << endl;

    for (const auto& edge : edges) {
        dotFile << "    " << edge.source << " -> " << edge.destinations << " [label=\"" << edge.label << "\"];" << endl;
    }

    dotFile << "}" << endl;
    dotFile.close();

    if (choice == 1 )
            system("dot -Tpng graph.dot -o OddParity.png"); // OddParity
    if (choice == 2 )
            system("dot -Tpng graph.dot -o SumNotEven.png"); // SumNotEven
    if (choice == 3 )
            system("dot -Tpng graph.dot -o Agreement.png"); // Agreement
    if (choice == 4 )
            system("dot -Tpng graph.dot -o Parity.png"); // Parity
    if (choice == 5 )
            system("dot -Tpng graph.dot -o Sorting.png"); // Sorting
    if (choice == 6 )
            system("dot -Tpng graph.dot -o NearAgreement.png"); // NearAgreement
    if (choice == 7 )
            system("dot -Tpng graph.dot -o OddParity2.png");
    if (choice == 8 )
            system("dot -Tpng graph.dot -o EvenParity.png");                
    if (choice == 9 )
            system("dot -Tpng graph.dot -o K_Coloring.png");
    if (choice == 10 )
            system("dot -Tpng graph.dot -o Sorting2.png");
}


#include "UniRing.h"
#include "TopDownTree.h"
// تابع برای اجرای SS_SymU
void executeSS_SymU(int B, int choice) {
     // The gamma detected
    queue<int> SelectGamma;
    int Gama;
    int gamma = ChooseGamma(B, SelectGamma, Gama, choice);
    auto start = high_resolution_clock::now();
    vector<pair<int, int>> Lgraph;
    // Gen Locality graph
    L_graph = LocalityGraph(B, gamma, Lgraph, choice);
    
    // construct the induced subgraph based on cycles involving gamma
    vector<pair<int, int>> Gprime;
    vector<pair<int, int>> Sgraph;
    vector<pair<int, int>> Sgraph1;
    Sgraph = SubGraph(L_graph, gamma, Gprime);
    Sgraph1 = SelfLoop(Sgraph, gamma);
    vector<pair<int, int>> inducedSubgraph;
    vector<pair<int, int>> result;
    inducedSubgraph = MissArcs(Sgraph1, gamma, B, result);

    // Gen spanning tree
   vector<pair<int, int>> Ti2;
   vector<pair<int, int>> S_T;
    printTree(inducedSubgraph, B, gamma, Ti2, S_T);

    

    //Gen Common Core CC
    vector<pair<int, int>> CommonCore;
    findCommonCore(Ti2, CommonCore);

        pair<int, int> BaseV;
        pair<int, int> PeriodV;
        
     
    //Gen Unbounded core UC
    vector<pair<int, int>> UC;
    
    UnboundedCore(gamma,choice, CommonCore, BaseV, PeriodV, UC);
    
    //Extract the formula (UC)
    pair<int, int>  arcU;
    arcU = UC.front() ;
    pair<bool, int> Phi2 = Phi_UCLset(arcU, BaseV, PeriodV);
    bool PhiUbool = Phi2.first;
    int PhiUint = Phi2.second;

    

    tuple<bool, string, int> psi2 = Psi_UCLset(arcU, BaseV, PeriodV);
    bool XiPrimeUbool = get<0>(psi2);
    string XiPrimeUst = get<1>(psi2);
    int Xiprimeint= get<2>(psi2);

    //Gen Actions
    printActionsCC(CommonCore, choice);
    GenActionsUC(choice, PhiUint, XiPrimeUst);
    
    
    //Gen Actions Graph
    int maxM = 10;
    typedef tuple<int, list<int>, int> ActionGraphItem;
    typedef vector<ActionGraphItem> ActionGraph;
    ActionGraph actionGraph;
    GenActionGraph(maxM, B, choice, CommonCore, UC, actionGraph, BaseV, PeriodV, Xiprimeint);

    //Draw Graph
    vector<Edge> edges = convertToEdges(actionGraph);
    drawGraph(edges, choice);

    cout << "Graph is drawn in graph.png file." << endl;
    
    //time
    auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        
    cout << "The execution time of this program: " << duration.count() << " ms" << endl;
   
    //Memory
    size_t memoryUC = sizeof(actionGraph[0]) * actionGraph.size(); // calculate memory used for UC
    cout << "The memory used for the Action Graph: " << memoryUC << " bytes" << endl; // print memory used for UC
}

void executeTopDownTree() {
     // Gen Locality graph
    vector<pair<int, int>> Lgraph2;
    vector<pair<int, int>> L_graph2;
    auto start = high_resolution_clock::now();
    L_graph2 = LGraph(10, Lgraph2, choice);
    PrintLGraph(L_graph2);

    int M_0; 
    int V0;
    int K;
    int V_L_1;
    vector<pair<int, int>> CyC0;
    PrintCycleC0(M_0,2, B, choice,CyC0, V_L_1, V0);
    Find_K(M_0, 8, choice,K);
   
    int Vn = -1;
    int M_1;
    generateNewArc(K, M_0, V0, V_L_1,M_1);
    cout << "***********************" << endl;
    generateLinearSetL0(CyC0, M_0, K, V_L_1, V0, choice, Vn);
    Remainingarcs_LC_0(CyC0, M_0);

    vector<int> V_C0; 
    vector<int> V_tree0; 
    vector<pair<int, int>> T1;
    vector<pair<int, int>> T2;
    CategorizeVertices(L_graph2, V_C0, V_tree0, M_0);
    vector<pair<int, int>> spanningTree;
    vector<pair<int, int>> tree = GenerateSpanningTree(spanningTree,V0, V_tree0);
    unordered_set<int> U;
    int V_M;
    CheckAndBuildU(M_0,V_L_1, K, V_tree0, V_M,U,Vn);
    LinearSet_Lc(U, V_C0, K, V_M);
    
    cout << "***********************************" << endl;
    
    int MaxM = 10;
    vector<pair<int, int>> newArc;
    vector<pair<int, int>> L_c;
    vector<pair<int, int>> L_0;
    pair<int, int> BaseV;
    pair<int, int> PeriodV;
    vector<pair<int, int>> R_CC;
    pair<int, int> BaseV0;
    pair<int, int> PeriodV0;
    All_LinearSets(V_L_1,M_0,choice,K, MaxM,V0,newArc,spanningTree, L_c,L_0,BaseV, PeriodV,R_CC,CyC0,BaseV0, PeriodV0);
    
     pair<int, int> arcU, arcU0;

    if (!L_c.empty()) {
        arcU = L_c.front();
    }

    if (!L_0.empty()) {
        arcU0 = L_0[0];
    }

    pair<bool, int> Phi2 = PPhi_UCLset(arcU, BaseV, PeriodV);
    bool PhiUbool = Phi2.first;
    int PhiUint = Phi2.second;

    pair<bool, int> Phi20 = PPhi_UCLset(arcU0, BaseV0, PeriodV0);
    bool PhiUbool0 = Phi20.first;
    int PhiUint0 = Phi20.second;

    tuple<bool, string, int> psi2 = PPsi_UCLset(arcU, BaseV, PeriodV);
    bool XiPrimeUbool = get<0>(psi2);
    string XiPrimeUst = get<1>(psi2);
    int Xiprimeint = get<2>(psi2);

    tuple<bool, string, int> psi20 = PPsi_UCLset(arcU0, BaseV0, PeriodV0);
    bool XiPrimeUbool0 = get<0>(psi20);
    string XiPrimeUst0 = get<1>(psi20);
    int Xiprimeint0 = get<2>(psi20);

    vector<pair<int, int>> BaseVVector = {BaseV0};
    
    if (spanningTree.empty()) {
        printActionsCC1(BaseVVector, choice, BaseV, PeriodV);
        GenActionsL_0(choice, PhiUint0, XiPrimeUst0);
    } else {
        // Gen Actions
        printActionsCC1(spanningTree, choice, BaseV, PeriodV);
        printActionsCC1(BaseVVector, choice, BaseV, PeriodV);    
        GenActionsL_0(choice, PhiUint0, XiPrimeUst0);
        GenActionsL_c(choice, PhiUint, XiPrimeUst);
    }
    
    // Gen Actions Graph
    typedef tuple<int, list<int>, int> ActionGraphItem;
    typedef vector<ActionGraphItem> ActionGraph;
    ActionGraph actionGraph;
    GenActionGraph1(MaxM, B, choice, newArc, spanningTree, L_c, L_0, actionGraph, BaseV, PeriodV, BaseV0, PeriodV0, Xiprimeint, Xiprimeint0);
   
    // Draw Graph
    vector<Edge> edges = convertToEdges1(actionGraph);
    drawGraph(edges, choice);

    cout << "Graph is drawn in graph.png file." << endl;
      //time
    auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        
    cout << "The execution time of this program: " << duration.count() << " ms" << endl;
   
    //Memory
    size_t memoryUC = sizeof(actionGraph[0]) * actionGraph.size(); // calculate memory used for UC
    cout << "The memory used for the Action Graph: " << memoryUC << " bytes" << endl; // print memory used for UC

}

int main() {
    while (true) {
        // Selecting the Topology
        char topologyChoice;
        cout << "Please choose the topology:" << endl;
        cout << "a. Uniring" << endl;
        cout << "b. Top-down Tree" << endl;
        cin >> topologyChoice;

        // Selecting the protocol 
         if (topologyChoice == 'a' || topologyChoice == 'A') {
        
        cout << "Please choose a Uniring protocol:" << endl;
        cout << "1. Odd Parity1 : (|Xi_1 - Xi| % 2 == 0) || (Xi % 2 != 0)" << endl;
        cout << "2. Sum Not Even : (Xi_1 + Xi) % 2 == 0" << endl;
        cout << "3. Agreement : (Xi_1 == Xi)" << endl;
        cout << "4. Parity : |Xi_1 - Xi| % 2 == 0" << endl;
        cout << "5. Sorting : (Xi_1 <= Xi)" << endl;
        cout << "6. Near Agreement : (Xi_1 == Xi) && (Xi_1 == Xi + 1)" << endl;
    } else if (topologyChoice == 'b' || topologyChoice == 'B') {
       
        cout << "Please choose a Top-down Tree protocol:" << endl;
        cout << "7. Odd Parity2 : (Xi = Xpi + 2) && ((Xpi % 2) != 0)" << endl;
        cout << "8. Even Parity : (Xi = Xpi + 2) && ((Xpi % 2) == 0)" << endl;
        cout << "9. K-coloring : Xi = Xpi + 1" << endl;
        cout << "10. Sorting2 : Xi > Xpi" << endl;
    } else {
       
        cout << "Error: Invalid choice." << endl;
    }

    
    cout <<  "Please enter your choice: ";
    cin >> choice;

        // Domain of xi
        B = 4; 
        vector<int> cycleSizes;
        vector<pair<int, int>> Lgraph;
        vector<pair<int, int>> L_graph;
        L_graph = LGraph(B, Lgraph, choice);
       
        
        if (CheckSelfLoop(B, choice)) {
            executeSS_SymU(B, choice);
        } else {
            if (!IsCycleFixedOrIncreasing(L_graph, B)) {
                executeTopDownTree();
            } else {
                executeTopDownTree();
                //executeSS_SymU(B, choice);
            }
        } 
        
               // Exit or Restart?
        char choice1;
        cout << "Do you want to exit the program? (Y/N): ";
        cin >> choice1;
        if (choice1 == 'N' || choice1 == 'n') {
            continue;
        } else if (choice1 == 'Y' || choice1 == 'y') {
            break;
        }
    }
    return 0;
}
