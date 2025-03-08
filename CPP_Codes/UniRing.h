#ifndef FUNCTIONS_H
#define FUNCTIONS_H



int ChooseGamma(int B, queue<int> SelectGamma, int gamma, int choice) {
    for (int M = 2; M <= B; ++M) {
        for (int i = 0; i < M; ++i) {
            if (L(i,i,choice,B)) {
                SelectGamma.push(i);
            }
        }
    break;}
    if (SelectGamma.empty()) {
        cout << "A gamma as a root could not be found" << endl;
    }    
        if (!SelectGamma.empty()) {
            cout << "The proposed list of gammas as the root is: ";
            while (!SelectGamma.empty()) {
                cout << SelectGamma.front() << " ";
                SelectGamma.pop();
            }
            cout << endl;
            int G;
            cout << "Please enter a gamma from the above list: ";
            cin >> G;
            gamma = G;

        } return gamma;
}

vector<pair<int, int>> LocalityGraph(int B, int gamma, vector<pair<int, int>>& L_graph, int choice) {
    if (gamma != -1) {
        for (int i = 0; i < B; ++i) {
            for (int j = 0; j < B; ++j) {
                if (L(i, j, choice,B)) {
                    L_graph.push_back(make_pair(i, j));
                }
            }
        }
        // Print Locality Graph
        cout << "Locality graph: {";
        for (const auto& pair : L_graph) {
            cout << "(" << pair.first << ", " << pair.second << ")";
            if (&pair != &L_graph.back()) {
                cout << ", ";
            }
        }
        cout << "}" << endl;
    }
    return L_graph;
}

void BackwardEdges(const vector<pair<int, int>>& graph, int current, unordered_set<int>& visited, vector<pair<int, int>>& subgraph) {
    visited.insert(current);
    for (const auto& edge : graph) {
        if (edge.second == current && visited.find(edge.first) == visited.end()) {
            subgraph.push_back(edge);
            BackwardEdges(graph, edge.first, visited, subgraph);
            return;
        }
    }
}

void ForwardEdges(const vector<pair<int, int>>& graph, int current, unordered_set<int>& visited1, vector<pair<int, int>>& subgraph1) {
    visited1.insert(current);
    for (const auto& edge : graph) {
        if (edge.first == current && visited1.find(edge.second) == visited1.end()) {
            subgraph1.push_back(edge);
            ForwardEdges(graph, edge.second, visited1, subgraph1);
            return;
        }
    }
}

vector<pair<int, int>> SubGraph(vector<pair<int, int>>& graph, int gamma, vector<pair<int, int>>& Gprime){
    unordered_set<int> visited;
    unordered_set<int> visited1;
    bool backward = false;
    for (const auto& edge : graph) {
        if (edge.first != edge.second) {
            if (edge.first == gamma && edge.second == gamma + 1){
                backward = true;
                ForwardEdges(graph, gamma, visited, Gprime);

            } 
            if (edge.second == gamma && edge.first == gamma + 1) {
                    backward = true;
                    BackwardEdges(graph, gamma, visited1, Gprime);
                }
                unordered_set<int> visited2;
                unordered_set<int> visited3;
                if (!backward){
                    if (gamma ==0){
                        BackwardEdges(graph, gamma, visited2, Gprime);
                        BackwardEdges(graph, gamma +1, visited3, Gprime);

                    }
                    if (gamma > 0) {
                        BackwardEdges(graph, gamma - 1, visited, Gprime);
                        BackwardEdges(graph, gamma, visited, Gprime);
                    }
                }
        }    
    }  
    return Gprime;
}  


vector<pair<int, int>> SelfLoop(vector<pair<int, int>>& graph, int gamma) {
    bool zero = false;
    bool One = false;
    bool selfloop = false;
    for (const auto& pair : graph) {
        if (pair.first == gamma && pair.second == gamma) {
            selfloop = true;
            break;
        }
    }
    if (!selfloop) {
        graph.push_back({gamma, gamma});
    }
    for (const auto& pair : graph) {
        if ((gamma == 0) && (pair.first == 1 && pair.second == gamma) || (pair.first == gamma && pair.second == 1) ) {
            zero = true; break;
        }
        else if ((gamma > 0) && (pair.first == 0 && pair.second == gamma) ||(pair.first == gamma && pair.second == 0)) {
            One = true; break;
        }     
    }
    if (!zero) {
        graph.push_back({1, gamma});
    }
    if (!One) {
        graph.push_back({0, gamma});
    }
    return graph;
}

void RemoveDuplic(const int gamma, int B , vector<pair<int, int>>& mergedSet, vector<pair<int, int>>& result ){
    set<pair<int, int>> uniqueSet(mergedSet.begin(), mergedSet.end());
    result.clear();
    for (const auto& element : uniqueSet) {
        pair<int, int> modifiedElement = element;
        if (gamma >= 1 && element.first == gamma && element.second == 0) {
            modifiedElement.first = 0;
            modifiedElement.second = gamma;
        }
        result.push_back(modifiedElement);
    }
}

// Function to add missing arcs from nodes absent in the induced subgraph to the root && SelfLoop
vector<pair<int, int>> MissArcs(vector<pair<int, int>>& graph, int gamma, int B, vector<pair<int, int>>& result){
    for (int i = 0; i < B; ++i) {
        bool found = false;
            for (const auto& pair : graph) {
               if (pair.first == i || pair.second == i) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                graph.push_back({i, gamma});
            }
    } 
    
    //RemoveDuplicate
    RemoveDuplic(gamma, B, graph, result);
    //Sorting Graph
    sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });
    return result;
}

// Function to print the resulting spanning tree
void printTree(const vector<pair<int, int>>& subgraph, int B, int gamma, vector<pair<int, int>>& T1, vector<pair<int, int>>& T2) { 
   vector<pair<int, int>> Ti;
   vector<pair<int, int>> Tif;
   int T = 0;
    for (int i = 2; i <= B ; ++i)
    {
        for (const auto& arc : subgraph)
        {
            for (int j = 0; j < i; ++j) {
                if ((arc.first == j && arc.second == j) || (arc.first == j && arc.second <= j) || (arc.first <= j && arc.second == j)){
                    Ti.push_back(arc);
                    RemoveDuplic(gamma,B, Ti, Tif);
                }
            }
       }       
       if (i == 3)
       {
        T1 = Tif;
       }
       if (i == 4)
       {
        T2 = Tif;
       }
        cout << "A spanning tree (M = " << i << "), T" << ++T  << ": {"; 
        for (const auto& arc : Tif)
            {
               cout << "(" << arc.first << ", " << arc.second << ")";
               if (&arc != &Tif.back())
                {
                   cout << ", ";
                }
            }
            cout << "}" << endl;
    }
}

void findCommonCore(const vector<pair<int, int>>& S_T, vector<pair<int, int>>& CommonCore) {
    cout << "Common Core (CC): {";
    for (const auto& arc : S_T) {
        CommonCore.push_back(arc);
        cout << "(" << arc.first << ", " << arc.second << ")";
        if (&arc != &S_T.back()) {
            cout << ", ";
        }
    }
    cout << "}" << endl;
}

pair<int, int> calculateDifference(pair<int, int> v1, pair<int, int> v2) {
    return make_pair(abs(v1.first - v2.first), abs(v1.second - v2.second));
} 

void UnboundedCore(const int gamma, int choice, vector<pair<int, int>>& ST, pair<int, int>& BaseV, pair<int, int>& PeriodV, vector<pair<int, int>>& UC){
    vector<int> U;
    pair<int, int> LastCC = ST.back();
    int M = 3;
    int v_M = M;
    for (int u = 2; u >= 0 ; --u){
        if (L(v_M, u, choice,B) || L(u, v_M, choice,B)){
            U.push_back(u);
        break;
        }
        
    }

    if (U.empty()) {
        UC.push_back(make_pair(v_M, gamma));
        BaseV = make_pair(v_M, gamma);
        pair<int, int> pv = calculateDifference(BaseV, LastCC);
        PeriodV = make_pair(pv.first, pv.second); 
    }
    else{
        for (int w : U){
            if (L(v_M, w, choice,B)){
        UC.push_back(make_pair(v_M, w));
        BaseV = make_pair(v_M, w);
        pair<int, int> pv = calculateDifference(BaseV, LastCC);
        PeriodV = make_pair(pv.first, pv.second);
        }
        else {
            if (L(w, v_M, choice,B)){
        UC.push_back(make_pair(w, v_M));
        BaseV = make_pair(w, v_M);
        pair<int, int> pv = calculateDifference(BaseV, LastCC);
        PeriodV = make_pair(pv.first, pv.second);
        }

        }
        }
    }
    cout << "BaseVector (bv) = (" << BaseV.first << ", " << BaseV.second << ")" << endl;
    cout << "PeriodVector (pv) = (" << PeriodV.first << ", " << PeriodV.second << ")" << endl; 
    cout << "Unbounded Core (UC) : {";
    for (const auto& pair : UC) {
        cout << "(" << pair.first << ", " << pair.second << ")";
        if (&pair != &UC.back()) {
            cout << ", ";
        }
    }
    cout << "}" << endl; 
    
    
}

pair<bool, int> Phi_FirstLset(pair<int, int> arc) {
    int Xi_1 = arc.first;
    pair<int, int> BaseV = make_pair(arc.first, arc.second);
    pair<int, int> PeriodV = make_pair(0, 0);
    bool PhiCbool = (Xi_1 <= BaseV.first - PeriodV.first);
    int PhiCint = BaseV.first - PeriodV.first;
    return make_pair(PhiCbool, PhiCint);
}

pair<bool, int> Phi_UCLset(pair<int, int> arc, pair<int, int> BaseV, pair<int, int> PeriodV) {
    int Xi_1 = arc.first + PeriodV.first;
    bool PhiUbool;
    int PhiUint;

    PhiUbool = (Xi_1 >= BaseV.first);
    PhiUint = BaseV.first;

    return make_pair(PhiUbool, PhiUint);
}



pair<bool, int> Psi_FirstLset(pair<int, int> arc) {
    int XiPrime = arc.second;
    int Xi_1 = arc.first;
    bool XiPrimeCbool = (XiPrime == arc.second);
    int XiPrimeCst = arc.second;

    return make_pair(XiPrimeCbool, XiPrimeCst);
}


tuple<bool, string, int> Psi_UCLset(pair<int, int> arc, pair<int, int> BaseV, pair<int, int> PeriodV) {
    int XiPrime = arc.second + PeriodV.second;
    int Xi_1 = arc.first + PeriodV.first;
    int R;
    bool XiPrimeUbool = (XiPrime == (Xi_1 + (BaseV.second - BaseV.first) + (PeriodV.second - PeriodV.first)));
    R = (BaseV.second - BaseV.first) + (PeriodV.second - PeriodV.first);
    string XiPrimeUst;
    int XiPrimeint;
    if (R > 0) {
       XiPrimeUst = "Xi_1+" + to_string(R);
       XiPrimeint = R;
    }
    else {
        if (R <= 0 && (abs(R) == Xi_1)|| ( (Xi_1 + (BaseV.second - BaseV.first) + (PeriodV.second - PeriodV.first)) == BaseV.second))
        {
            XiPrimeUst = to_string(Xi_1 + (BaseV.second - BaseV.first) + (PeriodV.second - PeriodV.first));
            XiPrimeint = (Xi_1 + (BaseV.second - BaseV.first) + (PeriodV.second - PeriodV.first));
        } else{
            XiPrimeUst = "Xi_1" + to_string(R);
            XiPrimeint = R;
        }
        
    }

    return make_tuple(XiPrimeUbool, XiPrimeUst, XiPrimeint);
}

void Allowable_XiLabels(pair<int, int> arc, int B, int choice, vector<int>& List) {
    bool del = false;
    for (int j = 0; j < B; ++j) {
        if ((choice == 6) && (j == B - 1) && (arc.first == 0) ){
            del = true;
        }
        if ((!del) && (!L(arc.first, j, choice,B)) && (j != arc.second) ) {
            List.push_back(j);
        }
    }
}
void GenActionsCC(pair<int, int> arcC, int choice,  int i){    
    pair<bool, int> Phi1 = Phi_FirstLset(arcC);
    bool PhiCbool = Phi1.first;
    int PhiCint = Phi1.second;
    pair<bool, int> psi1 = Psi_FirstLset(arcC);
    bool XiPrimeCbool = psi1.first;
    int XiPrimeCst = psi1.second;
    cout << "Action of CC " << i << ": (Xi_1 <= " << PhiCint << ") && (" << NegL(choice) << ") && (Xi != " << XiPrimeCst << ") -> Xi := " << XiPrimeCst << endl;   
}

void printActionsCC(vector<pair<int, int>> CommonCore, int choice){
    int i = 0;
    pair<int, int> arcC;
    for (const auto& pair : CommonCore) {
        arcC = make_pair(pair.first, pair.second);
        ++i;
        GenActionsCC(arcC, choice, i);
    }    
}
void GenActionsUC(int choice, int PhiUint, string XiPrimeUst){
    cout << "Action of UC:   (Xi_1 >= " << PhiUint << ") && (" << NegL(choice) << ") && (Xi != " << XiPrimeUst << ") -> Xi := " << XiPrimeUst << endl;
}

bool Check_NegLXi(pair<int, int> arc, int choice, int B) {
    vector<int> List;
    Allowable_XiLabels(arc, B, choice, List);
    if (List.empty()) return true;
    else {
    for (int j : List) {
        if (!L(arc.first, j, choice,B) && j != arc.second) {
            return true;
        }
    }
    }
    return false;
}
 
bool CheckGaurdCC(pair<int, int> arc, int choice, int B) {
    pair<bool, int> Phi1 = Phi_FirstLset(arc);
    bool PhiCbool = Phi1.first;

    pair<bool, int> psi1 = Psi_FirstLset(arc);
    bool XiPrimeCbool = psi1.first;
    bool NegL = Check_NegLXi(arc, choice, B);

    if (PhiCbool && NegL && XiPrimeCbool)
        return true;
    else
        return false;
}

bool CheckGaurdUC(pair<int, int> arc, pair<int, int> BaseV, pair<int, int> PeriodV, int choice, int B) {
    pair<bool, int> Phi2 = Phi_UCLset(arc, BaseV, PeriodV);
    bool PhiUbool = Phi2.first;

    tuple<bool, string, int> psi2 = Psi_UCLset(arc, BaseV, PeriodV);
    bool XiPrimeUbool = get<0>(psi2);
    bool NegL = Check_NegLXi(arc, choice, B);

    if (PhiUbool && NegL && XiPrimeUbool)
        return true;
    else
        return false;
}



void printActionGraph(const ActionGraph& actionGraph) {
    cout << "Action Graph (Labeled Arcs) for Example M = 10:  (a, [b] -> c)" << endl;
    cout << "{";
    for (const auto& item : actionGraph) {
        cout << "(" << get<0>(item) << ", [";
        for (auto it = get<1>(item).begin(); it != get<1>(item).end(); ++it) {
            if (it != get<1>(item).begin()) {
                cout << "|";
            }
            cout << *it;
        }
        cout << "], " << get<2>(item) << ")";
        if (&item != &actionGraph.back()) {
                cout << ", " << endl;
            }
    }
    cout << "}" << endl;
}

void GenActionGraph(int maxM, int B, int choice, vector<pair<int, int>> CommonCore, vector<pair<int, int>> UC, ActionGraph& actionGraph, pair<int, int> BaseV, pair<int, int> PeriodV, int XiPrimeint) {
    for (size_t i = 0; i < CommonCore.size(); ++i) {
        bool GaurdC = CheckGaurdCC(CommonCore[i], choice, maxM);
        if (GaurdC) {
            vector<int> List;
            Allowable_XiLabels(CommonCore[i], maxM, choice, List);
            list<int> Llist(List.begin(), List.end());
            actionGraph.push_back(make_tuple(CommonCore[i].first, Llist, CommonCore[i].second));
        }
    } 
    for (size_t i = 0; i < UC.size(); ++i) {
        bool GaurdU = CheckGaurdUC(UC[i], BaseV, PeriodV, choice, maxM);
        if (GaurdU) {
            vector<int> List;
            Allowable_XiLabels(UC[i], maxM, choice, List);
            list<int> Llist(List.begin(), List.end());
            actionGraph.push_back(make_tuple(UC[i].first, Llist, UC[i].second));
        }
    } 
    for (int i = 1; i<= maxM - 4; ++i){
        UC.push_back(make_pair(UC[i-1].first + PeriodV.first, UC[i-1].second + PeriodV.second));    
    } 
    for (size_t i = 1; i < UC.size(); ++i){
        vector<int> List;
        Allowable_XiLabels(UC[i], maxM, choice, List);
        list<int> Llist(List.begin(), List.end());
        actionGraph.push_back(make_tuple(UC[i].first, Llist, UC[i].second));
    }
    printActionGraph(actionGraph);  
}



string listToString(const list<int>& lst) {
    string result;
    for (int val : lst) {
        result += to_string(val) + "|";
    }
    if (!result.empty()) {
        result.pop_back();
    }
    return result;
}

vector<Edge> convertToEdges(const ActionGraph& actionGraph) {
    vector<Edge> edges;

    for (const auto& item : actionGraph) {
        Edge edge;
        edge.source = get<0>(item);
        edge.label = listToString(get<1>(item));
        edge.destinations = get<2>(item);
        edges.push_back(edge);
    }

    return edges;
}


#endif 