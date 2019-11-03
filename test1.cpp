#include<iostream>
#include <string.h>
#include <fstream>
#include<vector>
#include <stdlib.h>
#include<cmath>
#include<iomanip>
#define PI 3.1415926
using namespace std;

struct Dian{
	char name;
	double x;
	double y;
};
struct Angle{
	char name;
	double angle;
};
struct Jiaohui{
	char behind;
	char self;
	char ahead;
	double angle;
}; 

vector<string> split(const string& str, const string& delim) {  
	vector<string> res;  
	if("" == str) return res;  
	//先将要切割的字符串从string类型转换为char*类型  
	char * strs = new char[str.length() + 1] ; 
	strcpy(strs, str.c_str());   
	char * d = new char[delim.length() + 1];  
	strcpy(d, delim.c_str());  
	char *p = strtok(strs, d);  
	while(p) {  
		string s = p; //分割得到的字符串转换为string类型  
		res.push_back(s); //存入结果数组  
		p = strtok(NULL, d);  
	}  
	delete strs;
	return res; 
} 

void rad2angle(double rad,int &dd,int &mm,double &ss){
	dd = int(rad/PI*180);
	mm = int((rad/PI*180 - int(rad/PI*180)) * 60);
	ss = (rad/PI*180*60 - int(rad/PI*180*60))*60;
    cout << setw(3) <<setfill('0')<< dd <<"°";
	cout << setw(2) <<setfill('0')<< mm <<"'";
	cout << ss <<"\""<< endl;
}

void read_data(string file,string str[]){
	ifstream infile(file.c_str());
	if(!infile.is_open()){
		cout << "cannot open this file" <<endl;
	}
	for(int i = 0; i < 16; i++){
		infile >> str[i];
	}
	infile.close();
}

void write_data(string file,double alpha,string name){
	int dd,mm;double ss;
	rad2angle(alpha, dd, mm, ss);
	ofstream outfile(file.c_str(),ios::app);
	if(!outfile.is_open()){
		cout << "cannot open this file" <<endl;
	}
	outfile << name<<":"<< setw(3) <<setfill('0')<< dd <<"°" << setw(2) <<setfill('0')<< mm <<"'" << ss <<"\""<<"\n";
	outfile.close();
}

void zuobiao(string data[],Dian coordinate[]){
	vector<string> Str;
	for(int j = 0; j<4;j++){
		Str = split(data[j],",");
		strcpy(&coordinate[j].name,Str[0].c_str());
		coordinate[j].x = strtod(Str[1].c_str(),NULL);
		coordinate[j].y = strtod(Str[2].c_str(),NULL);
	}
}

double angle (Dian a,Dian b){
	double deltaX = b.x - a.x;
	double deltaY = b.y - a.y;
	double alpha;
	try{
		alpha = atan(deltaY/deltaX);
		if (deltaX < 0 && deltaY > 0){
			alpha = PI - alpha;
		}
		if (deltaX < 0 && deltaY < 0){
			alpha = PI + alpha;
		}
		if (deltaX > 0 && deltaY < 0){
			alpha = 2 * PI - alpha;
		}
	}
	catch(...){
		if (deltaY > 0){
			alpha = PI / 2;
		}else if (deltaY < 0){
			alpha = PI / 2 * 3;
		}else{
			alpha = 0;
		}
	}
	return (fmod(alpha,2*PI));
}
double dingjiao(double a,double b){
	return abs(a - b);
}
void bianchang(string data[],double x[]){
	for(int j = 4; j<10;j++){
	    x[j-4] = strtod(data[j].c_str(),NULL);
	}
}
void jiaohui(string data[],Jiaohui x[]){
	vector<string> Str;
	for(int j = 11; j<14;j++){
		Str = split(data[j],",");
		strcpy(&x[j-11].behind,Str[0].c_str());
		strcpy(&x[j-11].self,Str[1].c_str());
		strcpy(&x[j-11].ahead,Str[2].c_str());
		x[j-11].angle = strtod(Str[3].c_str(),NULL)/180*PI;
	}
}
void solveP(double a,double b,double &x){
	for (int i = 0; i < 3; i++){
		x = 1/(1/tan(a)-1/tan(b));
	}
}
void coordinateP(double a[],Dian b[],double &x,double &y){
	x = (a[0]*b[0].x + a[1]*b[1].x+a[2]*b[2].x)/(a[0]+a[1]+a[2]);
	y = (a[0]*b[0].y + a[1]*b[1].y+a[2]*b[2].y)/(a[0]+a[1]+a[2]);
	cout << setprecision(8)  << x <<endl;
	cout << setprecision(8)  << y <<endl;
}
void circle(Dian D1,Dian D2,Dian D3,double Px,double Py){
	double x,y,r,x1,y1,x2,y2,x3,y3,D;
	x1 = D1.x,y1 = D1.y; x2 = D2.x,y2 = D2.y;x3 = D3.x,y3 = D3.y;
	x = ((y2 - y1)*(y3*y3-y1*y1+x3*x3-x1*x1) - (y3-y1)*(y2*y2-y1*y1+x2*x2-x1*x1)) / (2*(x3-x1)*(y2-y1) - 2*(x2-x1)*(y3-y1));
	y = ((x2 - x1)*(x3*x3-x1*x1+y3*y3-y1*y1) - (x3-x1)*(x2*x2-x1*x1+y2*y2-y1*y1)) / (2*(y3-y1)*(x2-x1) - 2*(y2-y1)*(x3-x1));
	r = sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
	D = sqrt((Px-x)*(Px-x)+(Py-y)*(Py-y));
	if (abs(D - r)<=r/5){
		cout << "不合格"<<endl;
	} 
	cout << "abs(D - r):"<<abs(D - r)<<endl;
	cout << "r:"<<r<<endl;
}
int main(){
	Dian coordinate[4];
	Angle a[3];
	string read_file = "data.txt", out_file = "out.txt";
	string data[16]; 
	read_data(read_file,data);
	zuobiao(data,coordinate);
	int shuru[6][2] = {{0,1},{0,2},{1,2},{1,0},{2,0},{2,1}};
	string name[6] = {"AB","AC","BC","BA","CA","CB"};
	string angle_name[3] = {"A","B","C"};
	int shuru1[3][2] = {{0,1},{2,3},{4,5}};
	double alpha[6];
	for(int i = 0;i<6;i++){
		alpha[i] = angle(coordinate[shuru[i][0]],coordinate[shuru[i][1]]);
		write_data(out_file,alpha[i],name[i]);
	}
	for(int i = 0; i < 3; i++){
		strcpy(&a[i].name,angle_name[i].c_str());
		a[i].angle = dingjiao(alpha[shuru1[i][0]],alpha[shuru1[i][1]]);
		write_data(out_file,a[i].angle,angle_name[i]);
	}
	double length[6];
	Jiaohui J[3];
	bianchang(data,length);
	jiaohui(data,J);
	double P[3];
	for (int i = 0; i < 3; i++){
		solveP(a[i].angle,J[i].angle,P[i]);
		cout << P[i]<<endl;
	}
	double Px,Py;
	coordinateP(P,coordinate,Px,Py);
	circle(coordinate[0],coordinate[1],coordinate[2],Px,Py);
	return 0;
}


