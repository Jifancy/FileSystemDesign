#include <bits/stdc++.h>

using namespace std;

int main()
{   

    string s = "";

    // command to be run.
    //char *cmd = ; 

    // open pipe stream.
    FILE *fp = popen("date","r");
    int ch; 

    // error checking.
    // if(!fp) {
    //         fprintf(stderr,"Error popen with %s\n",cmd);
    //         exit(1);
    // }   

    // read from the process and print.
    while((ch = fgetc(fp)) != EOF) {
            s += ch;
    }

    if (s[s.length()-1] == '\n') s.pop_back();

    cout << s << endl;

// close the stream.
    pclose(fp);



    return 0;
}