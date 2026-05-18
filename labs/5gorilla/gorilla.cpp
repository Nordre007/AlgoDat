#include <bits/stdc++.h>
// Include the string library

using namespace std;


int main() {

    unordered_map<char, int> keys;
    string line;
    getline(cin, line);

    int inputSize = (line.size() / 2) + 1;

    for(int i = 0; i < inputSize; i++) {
        keys.insert({line[i * 2], i});  
    }

    int lookupMatrix[inputSize][inputSize];

    for(int i = 0; i < inputSize; i++) {
        for(int j = 0; j < inputSize; j++) {
            cin >> lookupMatrix[i][j];
        }
    }

    int nbrOfQueries;
    cin >> nbrOfQueries;

    for (int i = 0; i < nbrOfQueries; i++) {
        string str1;
        string str2;
        
        cin >> str1 >> str2;

        int n = str1.size();
        int m = str2.size();
        vector<vector<int>> scoreMatrix(n+1, vector<int>(m+1));

        for (int k = 0; k < n + 1; k++) {
            scoreMatrix[k][0] = k * -4;
        }

        for (int k = 0; k < m + 1; k++) {
            scoreMatrix[0][k] = k * -4;
        }

        for(int j = 1; j < n + 1; j++) {
            for(int k = 1; k < m + 1; k++) {
                int lookupScore = lookupMatrix[keys[str1[j-1]]][keys[str2[k-1]]];

                int localLargest = max(scoreMatrix[j - 1][k] -4, max(scoreMatrix[j][k - 1] - 4, scoreMatrix[j-1][k-1] + lookupScore));


                scoreMatrix[j][k] = localLargest;
            }
        }

        
        string firstStr = "";
        string secondStr = "";

        
        while(n != 0 && m != 0) {
           

            if  (scoreMatrix[n][m] == scoreMatrix[n-1][m-1] + lookupMatrix[keys[str1[n - 1]]][keys[str2[m-1]]]) {
                firstStr.insert(0, string(1, str1[n -1]));
                secondStr.insert(0, string(1, str2[m -1]));

                n = n - 1;
                m = m - 1;
            } else if (scoreMatrix[n][m] == scoreMatrix[n][m-1] - 4) {  // came from LEFT
                firstStr.insert(0, "*");
                secondStr.insert(0, string(1, str2[m-1]));
                m = m - 1;
            } else {                                                      // came from ABOVE
                firstStr.insert(0, string(1, str1[n-1]));
                secondStr.insert(0, "*");
                n = n - 1; 
            }

        }

        while (n > 0) {
        firstStr.insert(0, string(1, str1[n-1]));
        secondStr.insert(0, "*");
        n--;
        }
        while (m > 0) {
            firstStr.insert(0, "*");
            secondStr.insert(0, string(1, str2[m-1]));
            m--;
        }


        cout << firstStr << " " << secondStr << "\n";
        
        
    }



    return 0;
}

