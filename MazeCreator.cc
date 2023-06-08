#include <iostream>
#include <string>
#include <vector>

#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

typedef vector<vector<int>> Table,Maze;
typedef vector<vector<bool>> Btable;
typedef vector<int> Coord;

Maze maze;
Btable visited;
Table all_directions = {{0,-2},{-2,0},{0,2},{2,0}};
vector<Table> all_paths;

Coord init_pos({1,1});
Coord target;

int nb_check(0);
int SIZE_X;
int SIZE_Y;
int NB_COLORS(3);

string P("P2");

bool bool_found = false;
bool bool_display = false;
bool allow = true;

int count;
int LIMIT(20);
unsigned int DELAY(20);

string WRONG_DIM("==Wrong dimensions==");
string WRONG_ENTRY("==Wrong entry==");

void check(){
	cout<<"Check "<<nb_check<<endl;
	nb_check++;
}

void getSize(){
	string text_x, text_y;
	cin>>text_x>>text_y;
	while(true){
		try{
			SIZE_X = stoi(text_x);
			SIZE_Y = stoi(text_y);
			break;
		}
		catch(...){
			cout<<WRONG_DIM<<endl;
			allow = false;
		}
	}
}

void readSize(const char * argv[]){
	while(true){
		try{
			SIZE_X = stoi(argv[1]);	
			SIZE_Y = stoi(argv[2]);
			break;
		}
		catch(...){
			cout<<WRONG_DIM<<endl;
			allow = false;
		}
	}
}

bool checkEntry(){
	if(SIZE_X<100 && SIZE_Y<100) return true;
	return false;
}

void display(Maze maze){
	for(auto row:maze){
		for(auto number:row){
			cout<<number;
			if(number>10) cout<<"  ";
			else cout<<" ";
		}
		cout<<endl;
	}
}

void display(Btable visited){
	for(auto row:visited){
		for(auto n:row){
			if(n) cout<<3<<" ";
			else  cout<<0<<" ";
		}
		cout<<endl;
	}
}

bool filled(){
	for(auto line:visited){
		for(auto col:line){
			if(col==false) return false;
		}
	}
	return true;
}

void showCoord(string name, Coord coord){
	cout<<name<<": "<<coord[0]<<","<<coord[1]<<endl;
}

bool outside(Coord position){
	if(position[0]>(SIZE_Y-2) || position[1]> (SIZE_X-2) || position[0]<1 || position[1]<1) return true;
	else return false;
}

Table scanForFree(Coord position){
	Table directions;
	for(auto dir:all_directions){
		Coord look(2);
		look[0] = position[0] + dir[0];
		look[1] = position[1] + dir[1];
		if(!outside(look)){
			if(visited[look[0]][look[1]]==false) directions.push_back(dir);
		}
		
	}
	return directions;
}

void go(Coord position){
	visited[position[0]][position[1]] = true;
	
	Table free_directions = scanForFree(position);
	
	while(free_directions.size()>0){
		Coord look(2);
		Coord path(2);
		int random_dir(rand()%free_directions.size());
		look[0] = position[0] + free_directions[random_dir][0];
		look[1] = position[1] + free_directions[random_dir][1];
		
		path[0] = position[0] + (free_directions[random_dir][0]/2);
		path[1] = position[1] + (free_directions[random_dir][1]/2);
		
		if(!outside(look)){
			if(visited[look[0]][look[1]]==false){
				visited[path[0]][path[1]] = true;
				if(bool_display){
					display(visited);
					this_thread::sleep_for(chrono::milliseconds(DELAY) );
					system("clear");
				}
				go(look);
			}
		}
		free_directions = scanForFree(position);
	}
}

int main(int argc, const char * argv[]){
	srand(time(NULL));
	if(argc>3){
		int b = stoi(argv[3]);
		switch (b)
		{
			case 0:
				bool_display = false;
				break;
			case 1:
				bool_display = true;
				break;
			default:
				cout<<WRONG_ENTRY<<endl;
				allow = false;
		}
	}
	
	if(argc>4){
		DELAY = stoi(argv[4]);
	}
	
	if(bool_display)system("clear");
	
	if(argc>1) readSize(argv);
	else getSize();
	
	if(allow)
	{
		vector<vector<int>> m(SIZE_Y, vector<int>(SIZE_X));
		vector<vector<bool>> v(SIZE_Y, vector<bool>(SIZE_X));
		visited = v;
		maze = m;
	
		//Find all paths
		go({SIZE_Y/2,SIZE_X/2});
		if(bool_display){
			display(visited);
		}
		else{
			cout<<P<<endl;
			cout<<SIZE_X<<" "<<SIZE_Y<<endl;
			cout<<NB_COLORS<<endl;
			display(visited);
			
			//~ //Write the result into a pbm file
			//~ fstream result_file;
			//~ result_file.open(FILE_LOCATION + FILE_NAME, app);
		}
	}
}
