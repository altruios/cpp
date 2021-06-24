#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <chrono>
#include <thread>
# define M_PI           3.14159265358979323846  /* pi */
#include "json.hpp"
#include <iomanip>
// for convenience
using json = nlohmann::json;
using namespace std; 
constexpr int _height=432;
constexpr int _width =768;
constexpr int _scale = 5;
constexpr int _renders= 280;
          struct Thread_data{
               int render_number;
               vector<string> used_shapes;
               Thread_data(int rn, vector<string> us){
                    used_shapes=us;
                    render_number=rn;
               }
          };
class Board;
class Shape{
     public:
     vector<vector<int>> pattern;
     int height;
     int width;
     string name;
     string author;
     string year;
     Board *board_ref;
     int id;
     Shape(string name, vector<vector<int>> pattern, int id,int height,int width,string author, string year){
          this->pattern = pattern;
          this->height=height;
          this->width=width;
          this->name = name;
          this->author=author;
          this->year = year;
          this->id=id;
     }
     bool equals(Shape other_shape){
          if(this->id==other_shape.id){
               return true;
          }
          return false;
     }
};
class Cell{
     public:
          int life=0;
          int x;
          int y;
          int c=1;
          Cell* neighbors[8];
          double init_frequency= 40.0;
          double frequency= 40.0;
          Cell(int _x,int _y){
               this->x=_x;
               this->y=_y;
          }
          int get_life(){
               return this->life;
          }
          int get_life_count(){
               return c;
          }
          void set_color(int _c){
               this->c=_c;
          }
          void set_life(int _Life){
               this->life = _Life;
          }
          void set_n(Cell* n1,Cell* n2,Cell* n3,Cell* n4,Cell* n5,Cell* n6,Cell* n7,Cell* n8){
               this->neighbors[0]=n1;
               this->neighbors[1]=n2;
               this->neighbors[2]=n3;
               this->neighbors[3]=n4;
               this->neighbors[4]=n5;
               this->neighbors[5]=n6;
               this->neighbors[6]=n7;
               this->neighbors[7]=n8;
          }
          int get_n(){
               int n=0;
               for (Cell* c : this->neighbors){
                    n+=c->get_life();
               }
               return n;
          }
          void color_counter_step(const int g){
               if(g==this->get_life()){
                    this->c++;
               }else{
                    this->c=1;
               }
          }
          void increase_frequency(double amount){
               this->frequency = double(max(300.0, (double(this->frequency))+amount));
          }
          void reduce_frequency(double amount){
               this->frequency = double(min(30.0, (double(this->frequency))-amount));
          }
          void reset_frequency(){
               this->frequency=init_frequency;
          }
          int game_of_life(){
               int n = this->get_n();
               int l = this->get_life();
               int g = (l==1)?(n>3||n<2)?0:1:(n==3)?1:0;
               return g;
          }
};

class Board{
     public:
     int height=_height;
     int width=_width;
     int scale=_scale;
     bool has_started_playing = false;
     vector<vector<Cell>> matrix_A;
     vector<vector<Cell>> matrix_B;
     vector<vector<std::array<unsigned char, 3>>>  matrix_C;
     bool current_matrix_is_a = true;
     Board(int h, int w){
          this->height = h;
          this->width=w;

          cout<<"board started"<<endl;
          this->matrix_A.resize (this->width);
          this->matrix_B.resize (this->width);
          this->matrix_C.resize (this->width);
          for (int y = 0; y < this->height; y++){
               this->matrix_C[y].resize(this->width);
               for (int x = 0; x < this->width; x++){
                    this->matrix_A[y].push_back(Cell(x,y));
                    this->matrix_B[y].push_back(Cell(x,y));
                    for(int c = 0; c<3;c++){
                         this->matrix_C[y][x][c]=0;     
                    }
               }
          }
          this->set_neighbors();
     }  
     void set_neighbors(){
          for (int y = 0; y < this->height; y++){
               for (int x = 0; x < this->width; x++){
                    int wrappedYUnder = (y + this->height - 1) % this->height;
                    int wrappedYOver  = (y + this->height + 1) % this->height;
                    int wrappedXUnder = (x + this->width - 1)   % this->width;
                    int wrappedXOver  = (x + this->width + 1)  % this->width;
                    Cell* n1=this->get_cell_by_XY(x,wrappedYUnder);//top
                    Cell* n2=this->get_cell_by_XY(wrappedXOver,wrappedYUnder);//top right
                    Cell* n3=this->get_cell_by_XY(wrappedXOver,y);//right
                    Cell* n4=this->get_cell_by_XY(wrappedXOver,wrappedYOver);//bottom right
                    Cell* n5=this->get_cell_by_XY(x,wrappedYOver);//bottom
                    Cell* n6=this->get_cell_by_XY(wrappedXUnder,wrappedYOver);//bottom left
                    Cell* n7=this->get_cell_by_XY(wrappedXUnder,(y));//left
                    Cell* n8=this->get_cell_by_XY(wrappedXUnder,wrappedYUnder);//top left
                    this->flip_matrix();
                    Cell* nb1=this->get_cell_by_XY(x,wrappedYUnder);
                    Cell* nb2=this->get_cell_by_XY(wrappedXOver,wrappedYUnder);
                    Cell* nb3=this->get_cell_by_XY(wrappedXOver,y);
                    Cell* nb4=this->get_cell_by_XY(wrappedXOver,wrappedYOver);
                    Cell* nb5=this->get_cell_by_XY(x,wrappedYOver);
                    Cell* nb6=this->get_cell_by_XY(wrappedXUnder,wrappedYOver);
                    Cell* nb7=this->get_cell_by_XY(wrappedXUnder,(y));
                    Cell* nb8=this->get_cell_by_XY(wrappedXUnder,wrappedYUnder);

                    this->flip_matrix();

                    this->matrix_A[y][x].set_n(n1,n2,n3,n4,n5,n6,n7,n8);
                    this->matrix_B[y][x].set_n(nb1,nb2,nb3,nb4,nb5,nb6,nb7,nb8);    
               }
          }
          cout<< "neighbors set"<<endl;  
     }
     Cell* get_cell_by_XY(int x, int y){
          //the address of the cell at [y][x] of the the matrix of pointer
          return &((*get_current_matrix())[y][x]);
     }
     vector<std::vector<Cell>>* get_current_matrix(){
          if(this->current_matrix_is_a==true){
               return &this->matrix_A; 
          }
          return &this->matrix_B;
     }
     vector<std::vector<Cell>>* get_next_matrix(){
          if(this->current_matrix_is_a==false){
               return &this->matrix_A; 
          }
          return &this->matrix_B;
     }
     void flip_matrix(){
          this->current_matrix_is_a = !this->current_matrix_is_a;
     }
     void step(){
          vector<vector<Cell>>* current_matrix = this->get_current_matrix();
          vector<vector<Cell>>* next_matrix = this->get_next_matrix();
          for(int y=0;y<this->height;y++){
               for(int x=0;x<this->width;x++){
                    int l = (*current_matrix)[y][x].game_of_life();
                    (*current_matrix)[y][x].color_counter_step(l);
                    int c = (*current_matrix)[y][x].get_life_count();
                    (*next_matrix)[y][x].set_life(l);
                    (*next_matrix)[y][x].set_color(c);
                 
                    if(l){
                         (*next_matrix)[y][x].increase_frequency(c);
                    }else{
                         (*next_matrix)[y][x].reduce_frequency(1);

                    }
                                        

                    double freq = (*current_matrix)[y][x].frequency;

                    this->set_pixel(x,y,l,c,freq);
               }    
          }
          this->flip_matrix();
     }
     void render(int i, int render_number, vector<string> used_shapes){
          this->render_images(i,render_number);
          cout<<"\nfinished image render"<<endl;

          Thread_data td(render_number,used_shapes);
          pthread_t thread1;
          pthread_t thread2;
         // int rc = pthread_create(&thread1, NULL,  &this->render_video,(void *)&td);          
         // int rc2 = pthread_create(&thread2, NULL, &this->play_start, NULL);
          
          this->render_video(used_shapes,render_number);
         // this->play_start();
          //std::chrono::seconds rest_time(270);
         // std::this_thread::sleep_for(rest_time);

     }
     void play_start(){
          string command="ffplay -fs -i ./images/game_of_life_test_%05d.ppm -vf scale=3840:-1:flags=neighbor ";// 

          FILE *open_ffmpeg_play_script= popen(command.c_str(),"r");
          fscanf(open_ffmpeg_play_script, command.c_str());
          //pthread_exit(NULL);

     }
     void render_images(int i, int render_number){
          int render_count = i;
          string function_name = "render_images";
          for(int file_count =0;file_count<render_count;file_count++){
               auto start = std::chrono::high_resolution_clock::now();
               this->write_file(file_count); //there can be only one;
               this->step();
               auto stop = std::chrono::high_resolution_clock::now();
               auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
               auto count_left= render_count-file_count;
               long estimation  = (duration.count()*count_left)/1000;
               cout<<function_name <<setfill('0') <<setw(5) << file_count<<"of:"<<setfill('0') <<setw(5) <<render_count<<"  which took: "<<setfill('0') <<setw(5) << duration.count() <<"  eta:"<<setfill('0') <<setw(5) << estimation<< "to finish video "<< setfill('0') <<setw(5) << render_number<<"\r";
          }
     }

     void write_file_to_scale(int i){
          char file_name[50];
          int file_name_length;
          file_name_length=sprintf(file_name, "images/game_of_life_test_%05d.ppm",  i);
          FILE *file = fopen(file_name, "wb");      
          fprintf(file, "P6\n%d %d\n255\n", (this->width-1)*(this->scale), (this->height-1)*(this->scale));
               for(int y =0;y<this->height-1;y++){
                    for(int i=0;i<this->scale;i++){
                    for(int x =0;x<this->width-1;x++){
                         for(int j=0;j<this->scale;j++){
                         fwrite(&this->matrix_C[y][x],1,3, file);
                    }}}
               }
          fclose(file);
     }
     void write_file(int i){
          char file_name[50];
          int file_name_length;
          file_name_length = sprintf(file_name, "images/game_of_life_test_%05d.ppm",  i);
          FILE *file = fopen(file_name, "wb");      
          fprintf(file, "P6\n%d %d\n255\n", (this->width-1), (this->height-1));
               for(int y =0;y<this->height-1;y++){
                    for(int x =0;x<this->width-1;x++){
                         fwrite(&this->matrix_C[y][x],1,3, file);
                    }
               }
          fclose(file);
     }
     void render_video(vector<string> used_shapes,int render_number){
          cout<<"starting render"<<endl;
          
          string command = "ffmpeg -r 15 -f image2 -i images/game_of_life_test_%05d.ppm -vf scale=3840:-1:flags=neighbor renders/game_of_life"+string( 5, '0').append(to_string(render_number))+".mov";
          char documentation_path[50];
          int doc = sprintf(documentation_path, "renders/game_of_life%05d.txt", render_number);

          FILE *python_render_script= popen(command.c_str(),"r");
          fscanf(python_render_script, command.c_str());

          ofstream documentation;
          documentation.open (documentation_path);

          ifstream description;
          description.open("description.txt");
          if(!documentation.is_open()){
               cout<<endl<<endl<<endl<<"document not open?"<<endl<<endl;
          }
          if(!description.is_open()){
               cout<<endl<<endl<<endl<< "description did not open"<<endl<<endl;
          }else{
          documentation << description.rdbuf();
          documentation << ".\n";
          
          for(string s:used_shapes){
               documentation<<s;
               documentation<<endl;
          }
          documentation.close();
          description.close(); 
          }
         // pthread_exit(NULL);


     }
     void set_pixel(int x, int y,int l,int c, double freq){
          if(l){
        
               int amplitude=127;
               int center =128;
               double color_steps = 50;
               double frequency = (2*M_PI/color_steps);
               this->matrix_C[y][x][0]=char(sin(frequency*c+3)*amplitude+center);
               this->matrix_C[y][x][1]=char(sin(frequency*c+1)*amplitude+center);
               this->matrix_C[y][x][2]=char(sin(frequency*c+5)*amplitude+center);
          }else{               
               int death_count = 10;
               int death_state = int(max(0, death_count - c));
               int death_state_blue =int(max(0,death_state/2));  
               this->matrix_C[y][x][0]=death_state;
               this->matrix_C[y][x][1]=0;
               this->matrix_C[y][x][2]=death_state_blue;
               }
     } 
     void set_random_pixel(){
          int x=rand()%(this->width-1);
          int y=rand()%(this->height-1);
          this->set_pixel(x,y,1,1,3);
          (*this->get_current_matrix())[y][x].set_life(1);
          (*this->get_next_matrix())[y][x].set_life(1);
     }
     void set_pixel_and_matrixies(int x, int y, int life){
          
          this->set_pixel(x,y,life,1,3);
          (*this->get_current_matrix())[y][x].set_life(life);
          (*this->get_next_matrix())[y][x].set_life(life);

     }
     void set_neighbors_of_XY(int x,int y){
           for(Cell* c:this->matrix_A[y][x].neighbors){
               c->set_life(1);
               this->set_pixel(c->x,c->y,1,1,3);

           }
          for(Cell* c:this->matrix_B[y][x].neighbors){
               c->set_life(1);
               this->set_pixel(c->x,c->y,1,1,3);

           }
     }
     
     void add_shape(int x,int y,Shape &shape){
          for(vector<int> item: shape.pattern){
               this->set_pixel_and_matrixies(x+item[0],y+item[1],1);    
          }
     }
     
     void add_glider(){
          int x=rand()%(this->width-40)+20;
          int y=rand()%(this->height-40) +20;
          vector<vector<int>> glider {{x,y},{x,y+1},{x,y+2},{x-1,y+2},{x-2,y+1}};
          for(vector<int> item: glider){
               this->set_pixel_and_matrixies(item[0],item[1],1);    
          }
     }
     void clear(){
          for(int y = 0; y < this->height;y++){
               for(int x = 0;x < this->width;x++){
                    this->set_pixel_and_matrixies(x,y,0);
               }
          }
     }
};



vector<Shape> get_lexicon_shapes(){
     cout<< "opening file" <<endl;
     
     std::ifstream json_file("game_of_life_lexicon.json");
     cout<< "making json" <<endl;
     
     json lexicon;
     
     cout<< "pushing data" <<endl;
     json_file >> lexicon;
     
     cout<< "accessing lexicon shapes" <<endl;
     vector<Shape> lexicon_shapes;
     int id=1;
     cout<< "id ready" <<endl;
     for(const auto &item: lexicon.items()){
          auto s = item.value();
          cout<< "created s"<<endl;
          string name = s["name"];
          string author = s["author"]!=NULL?s["author"]:"no known author?";
          string year = s["year"]!=NULL?s["year"]:"no known year?";
                    cout<<" : "<<year<<endl;

          cout<<author<<endl;
          vector<vector<int>> pattern;
          auto width=s["width"];
          auto height=s["height"];
          for(const auto &coordinate: item.value()["pattern"].items()){
               int x = coordinate.value()["x"];
               int y = coordinate.value()["y"];
               if (x>width)
               {
                    width=x;
               };

               if(y>height){
                    height=y;
               }
               vector<int> c{x,y};
               pattern.push_back(c);
          }
          lexicon_shapes.push_back(Shape(name, pattern,id,height,width,author, year));
          id++;
     }
     return lexicon_shapes;
}
 
Shape get_random_shape(vector<Shape>& s){
          int index=rand()*2147%((s.size()));
          return (s[index]);
}
   
int main(){
     cout<< "random seed set"<<endl;
     srand (time(NULL));


     cout << "making game of life:" << endl;
     Board t{_height,_width};
     cout<<"ending"<<endl;
     vector<Shape> lexicon = get_lexicon_shapes();
     vector<string> used_shapes;
     for(int render_count=0;render_count<_renders;render_count++)
     {
          for(int i=0;i<125;i++){
               Shape s = get_random_shape(lexicon);
               int maxH= t.height-s.height-10;
               int maxW= t.width-s.width-10;
               int x= rand()%(maxW-1);
               int y= rand()%(maxH-1);
               t.add_shape(x,y,s);
               used_shapes.push_back(s.name+"  ||  "+s.author+"  ||  "+s.year);
               cout<< "getting "<<s.name<<endl;
          }
          for( string s :used_shapes){
               cout<< "used shape: "<<s<<endl;
          }
          t.render(4500, render_count, used_shapes);
          t.clear();
          cout<<"\n rendered run"<<endl;
     }
     cout<< "completed" <<endl;
     cout<<"closing in: ";
     cout<<"\b \b3";
     std::chrono::seconds one_second(1);
     std::this_thread::sleep_for(one_second);
     cout<<"\b \b2";
     std::this_thread::sleep_for(one_second);
     cout<<"\b \b1...";
     std::this_thread::sleep_for(one_second);
     return 0;
}


