#ifndef FUNCTIONS_H2
#define FUNCTIONS_H2

   
void PrintCycleC0(int& M_0, int startB, int endB, int choice,vector<pair<int, int>>& CyC0,int& V_L_1,int& V0) {
   int M_1;
    for (int B = startB; B <= endB; ++B) {
        
        vector<pair<int, int>> tempGraph;
        vector<pair<int, int>> CyC0 = LGraph(B, tempGraph, choice);

        if (FindCycle(CyC0)) {
            cout << "C_0" << " (for M_0 = " << B << "): {";
            for (size_t i = 0; i < CyC0.size(); ++i) {
                cout << "(" << CyC0[i].first << ", " << CyC0[i].second << ")";
                if (i < CyC0.size() - 1) {
                    cout << ", ";
                }
            }
            cout << "}" << endl;

            M_0 = B; 
            cout << "M_0 = "  << M_0 << endl;
            V0 = CyC0.front().first;
            V_L_1 = CyC0.back().first;
            cout << "V_0 = "  << V0 << endl;
            cout << "V_L-1 = "  << V_L_1 << endl;
            return;  
        }
    }
    
    cout << "No cycle found in the range B = " << startB << " to B = " << endB << endl;
}

void Find_K(int& M_0, int endB, int choice, int& K) {
    for (int i = M_0 +1; i <= endB; ++i){
        vector<pair<int, int>> tempGraph1;

        vector<pair<int, int>> currentGraph1 = LGraph(i, tempGraph1, choice);

        if (FindCycle(currentGraph1)){
            K = i - M_0;
            cout << "K = "  << K << endl;
            cout << "***********************" << endl;
            return;
        }
    }
    
}
// Step6
void generateNewArc(int& K, int& M_0, int& V0, int& V_L_1,int& M_1) {
    M_1 = M_0 + K; 
    
    pair<int, int> newArc = make_pair(V_L_1 + K, V0);
    
    stringstream actionGraph;
    actionGraph << "(xpi = " << V_L_1 + K << ") && (domSize() = " << M_1 << ") && (xi != " << V0 << ") -> xi := " << V0 << ";";

    cout << "Current Domain Size (M) = " << M_1 << endl;
    cout << "New arc: (" << newArc.first << ", " << newArc.second << ")" << endl;
    cout << "Action Graph for New arc: (xpi = V_L-1 + K) && (domSize() = M_0 + K) && (xi != V0) -> xi := V0" << endl;
    cout << "Action Graph for New arc: " << actionGraph.str() << endl;
}
// Step7
void generateLinearSetL0(vector<pair<int, int>> &CyC0, int& M_0, int& K, int& V_L_1, int& V0, int& choice, int& Vn) {
    
    int M = M_0 + K;  
    for (int i = V_L_1 + 1; i <= M; ++i) { 
        if (L(V_L_1, i, choice, M)) { 
            Vn = i;
            break;
        }
    }

    if (Vn == -1) {
        cout << "No valid vertex Vn found for the new cycle." << endl;
        return;
    }
    if (!CyC0.empty() && CyC0.back() == make_pair(V_L_1, V0)) {
        CyC0.pop_back();
    }

    vector<pair<int, int>> CyC1 = CyC0; 
    CyC1.push_back({V_L_1, Vn});       
    CyC1.push_back({Vn, V0});          

    
    cout << "New Vertex Vn = " << Vn << endl;
    cout << "Remove the arc: (" << V_L_1 << ", " << V0 << ") "
     << "And add the arcs: (" << V_L_1 << ", " << Vn << ") "
     << "and (" << Vn << ", " << V0 << ") in C_0" << endl;
    cout << "Linear Set L_0 (for M = M_0 + K = " << M << "): {";

    for (size_t i = 0; i < CyC1.size(); ++i) {
        cout << "(" << CyC1[i].first << ", " << CyC1[i].second << ")";
        if (i != CyC1.size() - 1) {
            cout << ", ";
        }
    }
    cout << "}" << endl;

    CyC0 = CyC1;
}

void Remainingarcs_LC_0(vector<pair<int, int>>& CyC0, int& M_0) {
    vector<pair<int, int>> tempGraph;

    CyC0 = LGraph(M_0, tempGraph, choice);
    cout << "Remaining arcs (LC_0): {";
    for (const auto& arc : CyC0) {
        cout << "(" << arc.first << ", " << arc.second << ")";
        if (&arc != &CyC0.back()) {
            cout << ", ";
        }
    }
    cout << "}" << endl;
}

void CategorizeVertices(const vector<pair<int, int>>& L_graph, vector<int>& V_C0, vector<int>& V_tree0, int& Mx) {
    for (int num = 0; num < Mx; ++num) {
        bool found = false;
        for (const auto& edge : L_graph) {
            if (edge.first == num || edge.second == num) {
                found = true;
                break;
            }
        }
        if (found) {
            V_C0.push_back(num);
        } else {
            V_tree0.push_back(num);
        }
    }

    cout << "Vertices in V_C0: ";
    for (int v : V_C0) {
        cout << v << " ";
    }
    cout << endl;

    cout << "Vertices in V_tree0: ";
    for (int v : V_tree0) {
        cout << v << " ";
    }
    cout << endl;
}

// Step9

vector<pair<int, int>> GenerateSpanningTree(vector<pair<int, int>>& spanningTree,int V0, const vector<int>& V_tree0) {
    
    for (int vertex : V_tree0) {
        spanningTree.push_back({vertex, V0});
    }

    cout << "SpanningTree (Common Core): {";
    for (size_t i = 0; i < spanningTree.size(); ++i) {
        cout << "(" << spanningTree[i].first << ", " << spanningTree[i].second << ")";
        if (i != spanningTree.size() - 1) {
            cout << ", ";
        }
    }
    cout << "}" << endl;
    return spanningTree;
}

int findVM(int& V_L_1, int& k, int& M_0, int& Vn) {
    int start = V_L_1;
    int end = V_L_1 + k;

    for (int num = start +1; num < end; ++num) {
        if (num < Vn) {
            return num;
        }
    }
    return -1;
}
//Step10
void CheckAndBuildU(int& M_0, int& V_L_1, int& K, const vector<int>& V_tree0,int& V_M, unordered_set<int>& U,int& Vn) {
    
    V_M = findVM(V_L_1, K,  M_0, Vn);
    if (V_M != -1)
    {
        
    for (int u : V_tree0) {
        if (L(V_M, u, choice, M_0)) {
            U.insert(u);    
        } 
    }

    cout << "U: {";
    for (auto it = U.begin(); it != U.end(); ++it) {
        cout << *it;
        if (next(it) != U.end()) {
            cout << ", ";
        }
    }
    cout << "}" << endl;
    } else {
     cout << " " << endl;      
    }
    

    
   
}

pair<int, int> addPairs(const pair<int, int>& p1, const pair<int, int>& p2) {
    return {p1.first + p2.first, p1.second + p2.second};
}

//Step11

void LinearSet_Lc(unordered_set<int>& U, const vector<int>& V_C0, int& K, int& V_M) {
    pair<int, int> result;
    
    if (V_M != -1)
    {
    if (U.empty()) {
        int vc;
        cout << "U is empty. Please pick a vertex from V_C0: ";
        for (size_t i = 0; i < V_C0.size(); ++i) {
            cout << V_C0[i];
            if (i != V_C0.size() - 1) cout << ", ";
        }
        cout << endl;
        cin >> vc;
        pair<int, int> pair1 = {V_M - K, vc};
        pair<int, int> pair2 = {K, 0};
        pair<int, int> result = addPairs(pair1, pair2);

        cout << "L_c (Unbounded Core) : {(" << result.first << ", " << result.second << "), ... }" << endl;
    } 
    //Step12
   
    else {
        int w = *U.begin(); 

        cout << "L_i+1 sets:" << endl;
        for (int i = 0; i <= K - 2; ++i) {
            pair<int, int> pair1 = {V_M + i - K, w + i - K};
            pair<int, int> pair2 = {K, K};
            pair<int, int> result = addPairs(pair1, pair2);
            cout << "L_" << i + 1 << ": {(" << result.first << ", " << result.second << ")}" << endl;
        }
    }
    } else {
        cout << "L_c = {}" << endl; 
    }
    
} 

pair<int, int> calculateDifference1(pair<int, int> v1, pair<int, int> v2) {
    return make_pair(abs(v1.first - v2.first), abs(v1.second - v2.second));
} 

void All_LinearSets(int& V_L_1,int& M_0,int& choice,int& K, int& B, int& V0, vector<pair<int, int>>& newArc,vector<pair<int, int>>& spanningTree, vector<pair<int, int>>& L_c,vector<pair<int, int>>& L_0,pair<int, int>& BaseV, pair<int, int>& PeriodV,vector<pair<int, int>>& R_CC, vector<pair<int, int>>& CyC0,pair<int, int>& BaseV0, pair<int, int>& PeriodV0) {
      
    cout << "Suppose Current Domain Size (M) = " << B << endl;

    vector<pair<int, int>> tempGraph;
    vector<pair<int, int>> CyCcl = LGraph(B, tempGraph, choice); 

    int V_L_11 = CyCcl.back().first;
    newArc.push_back(make_pair(V_L_11, V0));
    cout << "New arc: {";
    for (const auto& pair : newArc) {
        cout << "(" << pair.first << ", " << pair.second << ")";
        if (&pair != &newArc.back()) {
            cout << ", ";
        }
    }
    cout << "}" << endl;

    stringstream actionGraph;
    actionGraph << "(xpi = " << V_L_11 << ") && (domSize() = " << B << ") && (" << NegL(choice) << ") && (xi != " << V0 << ") -> xi := " << V0 << ";";

    vector<int> V_C0;   
    vector<int> V_tree0;  

    for (int num = 0; num < B; ++num) {
        bool found = false;
        for (const auto& edge : CyCcl) {
            if (edge.first == num || edge.second == num) {
                found = true;
                break;
            }
        }

        if (found) {
            V_C0.push_back(num);
        } else {
            V_tree0.push_back(num);
        }
    }

    if (spanningTree.empty())
    {
        tempGraph.clear();
        CyCcl=LGraph(B, tempGraph, choice);
        BaseV0 = CyCcl[0];
        cout << "BaseVector of L_0(bv) = (" << BaseV0.first << ", " << BaseV0.second << ")" << endl;
        L_0 = vector<pair<int, int>>(CyCcl.begin() + 1, CyCcl.end());
        pair<int, int> LastCa = L_0.front();
        pair<int, int> pv1 = calculateDifference1(CyCcl[0], CyCcl[1]);
        PeriodV = make_pair(pv1.first, pv1.second);
        cout << "PeriodVector of L_0 (pv) = (" << PeriodV.first << ", " << PeriodV.second << ")" << endl; 
       
        cout << "L_0: {";
            for (size_t i = 0; i < L_0.size(); ++i) { 
            cout << "(" << L_0[i].first << ", " << L_0[i].second << ")";
            if (i != L_0.size() - 1) { 
            cout << ", ";
        }
    }
    cout << "}" << endl;
    }else{
    for (int vertex : V_tree0) {
        bool isUnique = true;

         for (const auto& pair : spanningTree) {
            if (pair.first == vertex) {
                isUnique = false;
                break;
            }
        }

        if (isUnique) {
            L_c.push_back({vertex, V0});
        }
    }

    
    BaseV = L_c.front();
    pair<int, int> LastCC = spanningTree.back();
    pair<int, int> pv = calculateDifference1(BaseV, LastCC);
    PeriodV = make_pair(pv.first, pv.second);

    R_CC.push_back(make_pair(V0,V_L_1));

    BaseV0 = R_CC.front();
    pair<int, int> pv0;  
    pv0.first += K;  
    pv0.second += K; 

    PeriodV0 = make_pair(pv0.first, pv0.second);
    cout << "BaseVector of L_0(bv) = (" << BaseV0.first << ", " << BaseV0.second << ")" << endl;
    cout << "PeriodVector of L_0 (pv) = (" << PeriodV0.first << ", " << PeriodV0.second << ")" << endl; 
    
     if (!CyCcl.empty()) {
        L_0 = vector<std::pair<int, int>>(CyCcl.begin() + 1, CyCcl.end());
    }
    
    cout << "L_0: {";
    for (size_t i = 0; i < L_0.size(); ++i) { 
        cout << "(" << L_0[i].first << ", " << L_0[i].second << ")";
        if (i != L_0.size() - 1) { 
            cout << ", ";
        }
    }
    cout << "}" << endl;

    cout << "SpanningTree (Common Core): {";
    for (size_t i = 0; i < spanningTree.size(); ++i) {
        cout << "(" << spanningTree[i].first << ", " << spanningTree[i].second << ")";
        if (i != spanningTree.size() - 1) {
            cout << ", ";
        }
    }
    cout << "}" << endl;

    cout << "BaseVector of L_c(bv) = (" << BaseV.first << ", " << BaseV.second << ")" << endl;
    cout << "PeriodVector of L_c (pv) = (" << PeriodV.first << ", " << PeriodV.second << ")" << endl; 
   
    if (!L_c.empty()) {
        
    cout << "L_c: {";
    for (size_t i = 0; i < L_c.size(); ++i) {
        cout << "(" << L_c[i].first << ", " << L_c[i].second << ")";
        if (i != L_c.size() - 1) {
            cout << ", ";
        }
    }
    cout << "}" << endl;
    }
    }
    
    cout <<"================================"<< endl;
    cout << "Action of New arc: (xpi = V_L-1 + K) && (domSize() = M_0 + ky) && (" << NegL(choice) << ") && (xi != V0) -> xi := V0 , where (y is Lambda in N)" << endl;
    cout << "Action of New arc: " << actionGraph.str() << endl;

}

pair<bool, int> PPhi_FirstLset(pair<int, int> arc,pair<int, int>& BaseV,pair<int, int>& PeriodV) {
    int Xpi = arc.first;
    bool PhiCbool = (Xpi <= (BaseV.first - PeriodV.first));
    int PhiCint = (BaseV.first - PeriodV.first);
    return make_pair(PhiCbool, PhiCint);
}

pair<bool, int> PPhi_UCLset(pair<int, int> arc, pair<int, int> BaseV, pair<int, int> PeriodV) {
    int Xpi = arc.first + PeriodV.first;
    bool PhiUbool;
    int PhiUint;

    PhiUbool = (Xpi >= arc.first);
    PhiUint = arc.first;

    return make_pair(PhiUbool, PhiUint);
}



pair<bool, int> PPsi_FirstLset(pair<int, int> arc) {
    int XiPrime = arc.second;
    int Xpi = arc.first;
    bool XiPrimeCbool = (XiPrime == arc.second);
    int XiPrimeCst = arc.second;

    return make_pair(XiPrimeCbool, XiPrimeCst);
}


tuple<bool, string, int> PPsi_UCLset(pair<int, int> arc, pair<int, int> BaseV, pair<int, int> PeriodV) {
    int XiPrime = arc.second + PeriodV.second;
    int Xpi = arc.first + PeriodV.first;
    int R;
    bool XiPrimeUbool = (XiPrime == (Xpi + (BaseV.second - BaseV.first) + (PeriodV.second - PeriodV.first)));
    R = (BaseV.second - BaseV.first) + (PeriodV.second - PeriodV.first);
    string XiPrimeUst;
    int XiPrimeint;
    if (R > 0) {
       XiPrimeUst = "X_pi+" + to_string(R);
       XiPrimeint = R;
    }
    else {
        if (R <= 0 && (abs(R) == Xpi)|| ( (Xpi + (BaseV.second - BaseV.first) + (PeriodV.second - PeriodV.first)) == BaseV.second))
        {
            XiPrimeUst = to_string(Xpi + (BaseV.second - BaseV.first) + (PeriodV.second - PeriodV.first));
            XiPrimeint = (Xpi + (BaseV.second - BaseV.first) + (PeriodV.second - PeriodV.first));
        } else{
            XiPrimeUst = "X_pi" + to_string(R);
            XiPrimeint = R;
        }
        
    }

    return make_tuple(XiPrimeUbool, XiPrimeUst, XiPrimeint);
}

void Allowable_XiLabels1(pair<int, int> arc, int B, int choice, vector<int>& List) {
    for (int j = 0; j < B; ++j) {
        if (!L(arc.first, j, choice,B) && (j != arc.second) ){
            List.push_back(j);
        }   
    }
}
void GenActionsCC1(pair<int, int> arcC, int choice,  int i,pair<int, int>& BaseV,pair<int, int>& PeriodV){    
    pair<bool, int> Phi1 = PPhi_FirstLset(arcC, BaseV,PeriodV);
    bool PhiCbool = Phi1.first;
    int PhiCint = Phi1.second;
    pair<bool, int> psi1 = PPsi_FirstLset(arcC);
    bool XiPrimeCbool = psi1.first;
    int XiPrimeCst = psi1.second;
    cout << "Action of (" << arcC.first << "," << arcC.second <<"): (X_pi = " << arcC.first << ") && (" << NegL(choice) << ") && (Xi != " << XiPrimeCst << ") -> Xi := " << XiPrimeCst << endl;   
}

void printActionsCC1(vector<pair<int, int>> CommonCore, int choice,pair<int, int>& BaseV,pair<int, int>& PeriodV){
    int i = 0;
    pair<int, int> arcC;
    for (const auto& pair : CommonCore) {
        arcC = make_pair(pair.first, pair.second);
        ++i;
        GenActionsCC1(arcC, choice, i, BaseV, PeriodV);
    }    
}
void GenActionsL_0(int choice, int PhiUint, string XiPrimeUst){
    cout << "Action of L_0:   (X_pi >= " << PhiUint << ")"<< Even(choice) << "&& (" << NegL(choice) << ") && (Xi != " << XiPrimeUst << ") -> Xi := " << XiPrimeUst << endl;
}

void GenActionsL_c(int choice, int PhiUint, string XiPrimeUst){
    cout << "Action of L_c:   (X_pi >= " << PhiUint << ")" << Odd(choice) << "&& (" << NegL(choice) << ") && (Xi != " << XiPrimeUst << ") -> Xi := " << XiPrimeUst << endl;
}

bool Check_NegLXi1(pair<int, int> arc, int choice, int B) {
    vector<int> List;
    Allowable_XiLabels1(arc, B, choice, List);
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
 
bool CheckGaurdCC1(pair<int, int> arc, int choice, int B,pair<int, int>& BaseV,pair<int, int>& PeriodV) {
    pair<bool, int> Phi1 = PPhi_FirstLset(arc, BaseV,PeriodV);
    bool PhiCbool = Phi1.first;

    pair<bool, int> psi1 = PPsi_FirstLset(arc);
    bool XiPrimeCbool = psi1.first;
    bool NegL = Check_NegLXi1(arc, choice, B);

    if (PhiCbool && NegL && XiPrimeCbool)
        return true;
    else
        return false;
}

bool CheckGaurdUC1(pair<int, int> arc, pair<int, int> BaseV, pair<int, int> PeriodV, int choice, int B) {
    pair<bool, int> Phi2 = PPhi_UCLset(arc, BaseV, PeriodV);
    bool PhiUbool = Phi2.first;

    tuple<bool, string, int> psi2 = PPsi_UCLset(arc, BaseV, PeriodV);
    bool XiPrimeUbool = get<0>(psi2);
    bool NegL = Check_NegLXi1(arc, choice, B);

    if (PhiUbool && NegL && XiPrimeUbool)
        return true;
    else
        return false;
}



void printActionGraph1(const ActionGraph& actionGraph) {
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

void GenActionGraph1(int maxM, int B, int choice,vector<pair<int, int>>& newArcc, vector<pair<int, int>>& spanningTree, vector<pair<int, int>>& L_c, vector<pair<int, int>>& L_0, ActionGraph& actionGraph, pair<int, int> BaseV, pair<int, int> PeriodV, pair<int, int> BaseV0, pair<int, int> PeriodV0, int XiPrimeint, int XiPrimeint0) {
    pair<int, int> newArc = newArcc.front();
if (spanningTree.empty()) {
    vector<int> List;
    Allowable_XiLabels1(BaseV0, maxM, choice, List);
    list<int> Llist(List.begin(), List.end());
    actionGraph.push_back(make_tuple(BaseV0.first, Llist, BaseV0.second));
    
    for (size_t i = 0; i < L_0.size(); ++i) {
        bool GaurdU = CheckGaurdUC1(L_0[i], BaseV0, PeriodV0, choice, maxM);
        if (GaurdU) {
            List.clear();
            Allowable_XiLabels1(L_0[i], maxM, choice, List);
            Llist.assign(List.begin(), List.end()); 
            actionGraph.push_back(make_tuple(L_0[i].first, Llist, L_0[i].second));
        }
    } 
    List.clear();
    Allowable_XiLabels1(newArc, maxM, choice, List);
    Llist.assign(List.begin(), List.end());
    actionGraph.push_back(make_tuple(newArc.first, Llist, newArc.second));
   
} else {
    vector<int> List; 
    Allowable_XiLabels1(BaseV0, maxM, choice, List);
    list<int> Llist(List.begin(), List.end()); 
    actionGraph.push_back(make_tuple(BaseV0.first, Llist, BaseV0.second));
    
    for (size_t i = 0; i < L_0.size(); ++i) {
        bool GaurdU = CheckGaurdUC1(L_0[i], BaseV0, PeriodV0, choice, maxM);
        if (GaurdU) {
            List.clear(); 
            Allowable_XiLabels1(L_0[i], maxM, choice, List);
            Llist.assign(List.begin(), List.end());
            actionGraph.push_back(make_tuple(L_0[i].first, Llist, L_0[i].second));
        }
    }

    for (size_t i = 0; i < spanningTree.size(); ++i) {
        List.clear(); 
        Allowable_XiLabels1(spanningTree[i], maxM, choice, List);
        Llist.assign(List.begin(), List.end());
        actionGraph.push_back(make_tuple(spanningTree[i].first, Llist, spanningTree[i].second));
    } 
    for (size_t i = 0; i < L_c.size(); ++i) {
        List.clear(); 
        Allowable_XiLabels1(L_c[i], maxM, choice, List);
        Llist.assign(List.begin(), List.end());
        actionGraph.push_back(make_tuple(L_c[i].first, Llist, L_c[i].second));
    }
    List.clear();
    Allowable_XiLabels1(newArc, maxM, choice, List);
    Llist.assign(List.begin(), List.end());
    actionGraph.push_back(make_tuple(newArc.first, Llist, newArc.second));
}

printActionGraph1(actionGraph); 
}

#endif 