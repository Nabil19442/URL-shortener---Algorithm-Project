#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

const string DATA_FILE = "urls.txt";
const string ALPHABET =
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

unordered_map<string,string> urlMap;
long long keyCounter = 0;

void gotoxy(int x,int y){
    COORD c{(SHORT)x,(SHORT)y};
    SetConsoleCursorPosition(hConsole,c);
}

void setcolor(int c){
    SetConsoleTextAttribute(hConsole,c);
}

void hideCursor(){
    CONSOLE_CURSOR_INFO ci{1,FALSE};
    SetConsoleCursorInfo(hConsole,&ci);
}

void getConsoleSize(int &w,int &h){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole,&csbi);
    w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void flushInputBuffer(){
    while(_kbhit()) _getch();
}

void drawBoxCentered(int width,int height){
    int sw,sh;
    getConsoleSize(sw,sh);

    int left = (sw-width)/2;
    int top = (sh-height)/2;
    int right = left + width - 1;
    int bottom = top  + height - 1;

    setcolor(14);

    gotoxy(left,top);        
    cout<<char(201);
    gotoxy(right,top);       
    cout<<char(187);
    gotoxy(left,bottom);     
    cout<<char(200);
    gotoxy(right,bottom);    
    cout<<char(188);

    for(int x=left+1;x<right;x++){
        gotoxy(x,top);       
        cout<<char(205);
        gotoxy(x,bottom);    
        cout<<char(205);
    }
    for(int y=top+1;y<bottom;y++){
        gotoxy(left,y);      
        cout<<char(186);
        gotoxy(right,y);     
        cout<<char(186);
    }

    setcolor(7);
}

void printClipped(int x,int y,const string &text,int maxWidth){
    gotoxy(x,y);
    if((int)text.size()<=maxWidth){
        cout<<text;
    } else {
        cout<<text.substr(0,maxWidth-3)<<"...";
    }
}

void loadingScreen(){
    system("cls");
    hideCursor();

    int sw,sh;
    getConsoleSize(sw,sh);

    setcolor(11);
    gotoxy(sw/2-14,sh/2-2);
    cout<<"LOADING URL SHORTENER";

    setcolor(6);
    gotoxy(sw/2-25,sh/2);
    for(int i=0;i<50;i++){
        cout<<char(219);
        Sleep(25);
    }
    setcolor(7);
    Sleep(300);
}

void endingScreen(){
    system("cls");
    drawBoxCentered(50,7);

    int sw,sh;
    getConsoleSize(sw,sh);

    setcolor(10);
    gotoxy(sw/2-15,sh/2-1);
    cout<<"THANK YOU FOR USING";

    gotoxy(sw/2-13,sh/2);
    cout<<"URL SHORTENER";

    setcolor(7);
    gotoxy(sw/2-18,sh/2+2);
    cout<<"Press any key to exit...";
    _getch();
}

string inputInBox(int x,int y,int maxWidth){
    string s="";
    gotoxy(x,y);

    while(true){
        int ch=_getch();

        if(ch==13) break;            // ENTER
        if(ch==8) return "__BACK__"; // BACKSPACE

        if(ch>=32 && ch<=126){
            if((int)s.size()<maxWidth){
                s.push_back((char)ch);
                cout<<(char)ch;
            }
        }
    }
    return s;
}


string generateKey(){
    long long x=keyCounter++;
    string key="";
    while(true){
        key=ALPHABET[x%62]+key;
        x/=62;
        if(x==0) break;
    }
    return key;
}

string shortenURL_logic(const string &url){
    string key=generateKey();
    urlMap[key]=url;
    return key;
}

string lookupURL_logic(const string &key){
    if(urlMap.count(key)) return urlMap[key];
    return "URL NOT FOUND";
}

void saveToFile(){
    ofstream out(DATA_FILE);
    out<<keyCounter<<"\n";
    for(auto &p:urlMap)
        out<<p.first<<" "<<p.second<<"\n";
}

void loadFromFile(){
    ifstream in(DATA_FILE);
    if(!in) return;

    urlMap.clear();
    in>>keyCounter;
    string k,u;
    while(in>>k){
        in>>ws;
        getline(in,u);
        urlMap[k]=u;
    }
}


void shortenURL_screen(){
    system("cls");
    drawBoxCentered(80,11);

    int sw,sh;
    getConsoleSize(sw,sh);
    int left=(sw-80)/2;
    int top =(sh-11)/2;

    setcolor(11);
    gotoxy(sw/2-10,top+1);
    cout<<"SHORTEN URL";

    setcolor(7);
    gotoxy(left+3,top+3);
    cout<<"Enter Long URL:";

    setcolor(8);
    gotoxy(left+3,top+8);
    cout<<"Use BACKSPACE to return to main menu";

    string url=inputInBox(left+3,top+5,72);
    if(url=="__BACK__") return;

    string key=shortenURL_logic(url);
    saveToFile();

    setcolor(10);
    printClipped(left+3,top+6,"Short URL: https://short.ly/"+key,72);

    _getch();
}

void lookupURL_screen(){
    system("cls");
    drawBoxCentered(70,10);

    int sw,sh;
    getConsoleSize(sw,sh);
    int left=(sw-70)/2;
    int top =(sh-10)/2;

    setcolor(11);
    gotoxy(sw/2-8,top+1);
    cout<<"LOOKUP URL";

    setcolor(7);
    gotoxy(left+3,top+3);
    cout<<"Enter Short Key:";

    setcolor(8);
    gotoxy(left+3,top+7);
    cout<<"Use BACKSPACE to return to main menu";

    string key=inputInBox(left+3,top+5,20);
    if(key=="__BACK__") return;

    string res=lookupURL_logic(key);

    setcolor(10);
    printClipped(left+3,top+6,"Result: "+res,60);

    _getch();
}

void showAll_screen(){
    system("cls");
    drawBoxCentered(90,20);

    int sw,sh;
    getConsoleSize(sw,sh);
    int left=(sw-90)/2;
    int top =(sh-20)/2;

    setcolor(11);
    gotoxy(sw/2-10,top+1);
    cout<<"ALL SHORTENED URLS";

    int row=top+3;
    setcolor(7);

    if(urlMap.empty()){
        gotoxy(sw/2-8,sh/2);
        cout<<"No URLs found.";
    } else {
        for(auto &p:urlMap){
            if(row>=top+18) break;
            string line=p.first+" -> "+p.second;
            printClipped(left+3,row++,line,82);
        }
    }
    _getch();
}

int menu(){
    vector<string> items={
        "Shorten URL",
        "Lookup URL",
        "Show All URLs",
        "Exit"
    };

    int sel=0;

    while(true){
        system("cls");
        hideCursor();
        drawBoxCentered(60,14);

        int sw,sh;
        getConsoleSize(sw,sh);
        int left=(sw-60)/2;
        int top =(sh-14)/2;

        setcolor(11);
        gotoxy(sw/2-10,top+1);
        cout<<"URL SHORTENER MENU";

        for(int i=0;i<items.size();i++){
            gotoxy(left+4,top+3+i);
            if(i==sel){
                setcolor(10);
                cout<<"> "<<items[i];
            } else {
                setcolor(7);
                cout<<"  "<<items[i];
            }
        }

        setcolor(8);
        gotoxy(left+4,top+10); 
        cout<<"UP   : Move Up";
        gotoxy(left+4,top+11); 
        cout<<"DOWN : Move Down";
        gotoxy(left+4,top+12); 
        cout<<"ENTER: Select";

        int ch=_getch();
        if(ch==224){
            ch=_getch();
            if(ch==72) sel=(sel-1+items.size())%items.size();
            else if(ch==80) sel=(sel+1)%items.size();
        } else if(ch==13){
            return sel;
        }
    }
}


int main(){
    loadFromFile();
    loadingScreen();

    while(true){
        int choice=menu();
        flushInputBuffer();

        if(choice==0) shortenURL_screen();
        else if(choice==1) lookupURL_screen();
        else if(choice==2) showAll_screen();
        else if(choice==3) break;
    }

    endingScreen();
    return 0;
}
