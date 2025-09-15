#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    cin.ignore();

    // Read men preference lists
    vector<string> men(n), women(n);
    unordered_map<string, int> manIndex, womanIndex;

    vector<vector<string>> menPrefNames(n);
    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);
        stringstream ss(line);
        ss >> men[i];
        manIndex[men[i]] = i;
        string w;
        while (ss >> w) menPrefNames[i].push_back(w);
    }

    // Read women preference lists
    vector<vector<int>> womenRank(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);
        stringstream ss(line);
        ss >> women[i];
        womanIndex[women[i]] = i;
        string m;
        int rank = 0;
        while (ss >> m) {
            womenRank[i][manIndex[m]] = rank++;
        }
    }

    // Convert men preferences from names → indices
    vector<vector<int>> menPrefs(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            menPrefs[i][j] = womanIndex[menPrefNames[i][j]];
        }
    }

    // Gale–Shapley algorithm
    vector<int> manPartner(n, -1), womanPartner(n, -1);
    vector<int> nextProposal(n, 0); // which woman each man proposes next
    queue<int> freeMen;
    for (int i = 0; i < n; i++) freeMen.push(i);

    while (!freeMen.empty()) {
        int m = freeMen.front(); freeMen.pop();
        int w = menPrefs[m][nextProposal[m]++];
        if (womanPartner[w] == -1) {
            // woman w is free
            womanPartner[w] = m;
            manPartner[m] = w;
        } else {
            int m2 = womanPartner[w];
            if (womenRank[w][m] < womenRank[w][m2]) {
                // w prefers m over m2
                womanPartner[w] = m;
                manPartner[m] = w;
                manPartner[m2] = -1;
                freeMen.push(m2);
            } else {
                // w rejects m
                freeMen.push(m);
            }
        }
    }

    // Output matches (in input order of men)
    for (int i = 0; i < n; i++) {
        cout << men[i] << " " << women[manPartner[i]] << "\n";
    }
    return 0;
}
