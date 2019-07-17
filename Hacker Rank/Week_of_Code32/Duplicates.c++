#include <bits/stdc++.h>

using namespace std;

string s_bar(string& str, int length){

string result = "";

for(int i = 0; i < length; i++){

        if(str.at(i) == '1')

          result += '0';

        else

          result += '1';

}

return result;

}

string duplication(int x){

    // Complete this function



    string result;



    string s = "0";



    string t; // s bar is t



    while(s.length() <= 1000){



        t = s_bar(s, s.length());



        s += t;



    }



    result = s[x];



    return result;



}







int main() {



    int q;



    cin >> q;



    for(int a0 = 0; a0 < q; a0++){



        int x;



        cin >> x;



        string result = duplication(x);



        cout << result << endl;



    }



    return 0;



}
