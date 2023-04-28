#include<vector>
#include<iostream>
#include<algorithm>
#include<climits>

using namespace std;

vector<long double> dp;
vector<long double> segments;

vector<long double> computeCost(vector<pair<long double, long double>> &points, int i, int j){

    long double productSum = 0, xSum = 0, ySum = 0, xSquareSum = 0;
    int n = j - i + 1;

    for(int k = i; k <= j; k++){
        productSum += (points[k].first * points[k].second);
        xSum += points[k].first;
        ySum += points[k].second;
        xSquareSum += (points[k].first * points[k].first);
    }

    long double a = ((n * productSum) - (xSum * ySum)) / ((n * xSquareSum) - (xSum * xSum));
    long double b = (ySum - (a * xSum)) / n;

    long double cost = 0;
    for(int k = i; k <= j; k++){
        long double difference = points[k].second - (a * points[k].first) - b;
        cost += (difference * difference);
    }

    return {a, b, cost};
}

long double optimalLines(vector<vector<long double>>& lineCost, int C, int j){

    if (j < 0) return 0;
    if (dp[j] != LONG_MAX) return dp[j];

    long double answer = LONG_MAX;
    int idx;

    for(int i = 0; i <= j; i++){
        long double temp = min(answer, lineCost[i][j] + C + optimalLines(lineCost, C, i - 1));
        if (temp < answer){
            idx = i;
            answer = temp;
        }
    }
    segments[j] = idx;

    return dp[j] = answer;
}

long double optimalLinesIterative(vector<vector<long double>>& lineCost, int C, int j){

    for(int i = 0; i <= j; i++) {
        for(int k = 0; k <= i; k++) {
            long double temp = (k == 0 ? 0 : dp[k - 1]) + lineCost[k][i] + C;
            if(temp < dp[i]) {
                dp[i] = temp;
                segments[i] = k;
            }
        }
    }

    int idx = j;
    while(idx >= 0) {
        idx = segments[idx] - 1;
        segments[idx + 1] = idx;
    }

    return dp[j];
}

int main(){

    int n, C;
    cin >> n >> C;

    vector<pair<long double, long double>> points(n);
    for(int i = 0; i < n; i++){
        cin >> points[i].first >> points[i].second;
    }

    sort(points.begin(), points.end());
    
    vector<vector<long double>> a(n, vector<long double>(n));
    vector<vector<long double>> b(n, vector<long double>(n));
    vector<vector<long double>> lineCost(n, vector<long double>(n));

    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            vector<long double> result = computeCost(points, i, j);
            a[i][j] = result[0];
            b[i][j] = result[1];
            lineCost[i][j] = result[2];
        }
    }

    dp.assign(n, LONG_MAX);
    segments.assign(n, -1);
    optimalLines(lineCost, C, n - 1);

    cout << dp[n - 1] << endl;
    int idx = n - 1;
    vector<long double> pts;

    vector<vector<long double>> slopeIntercept;

    while(segments[idx] > 0){
        pts.push_back(segments[idx]);
        slopeIntercept.push_back({a[segments[idx]][idx], b[segments[idx]][idx]});
        idx = segments[idx] - 1;
    }

    slopeIntercept.push_back({a[0][idx], b[0][idx]});

    reverse(slopeIntercept.begin(), slopeIntercept.end());

    vector<vector<pair<long double, long double>>> lines(pts.size() + 1, vector<pair<long double, long double>>());
    int x = n - 1;
    for(int i = 0; i < pts.size(); i++){
        for(int j = pts[i]; j <= x; j++){
            lines[i].push_back(points[j]);
        }
        x = pts[i] - 1;
    }
    
    for(int i = 0; i <= x; i++){
        lines[pts.size()].push_back(points[i]);
    }
    reverse(lines.begin(), lines.end());

    for(int i = 0; i < lines.size(); i++){
        for(int j = 0; j < lines[i].size(); j++){
            cout << '(' << lines[i][j].first << ',' << lines[i][j].second << ')';
        }
        cout << " " << slopeIntercept[i][0] << " " << slopeIntercept[i][1];
        cout << endl;
    }
}
