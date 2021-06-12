#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <chrono>
#include <thread>
using namespace std; 
constexpr int _height=50;
constexpr int _width =50;


class Cell{
     public:
          int life=0;
          int x;
          int y;
          int c=0;
          Cell* neighbors[8];
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
          int set_life_get_count(int _Life){
               this->color_counter_step(_Life);
               this->life = _Life;
               return this->c;
          }
          void set_life(int _Life){
               this->life = _Life;
               this->color_counter_step(_Life);
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
          int game_of_life(){
               int n = this->get_n();
               int l = this->get_life();
               int g = (l==1)?(n>3||n<2)?0:1:(n==3)?1:0;
               this->color_counter_step(g);
               return g;
          }
};

class Board{
     public:
     int height=_height;
     int width=_width;
     vector<vector<Cell>> matrix_A;
     vector<vector<Cell>> matrix_B;
     vector<vector<vector<unsigned char>>>  matrix_C;
     bool current_matrix_is_a = true;
     unsigned char _color[3];
     Board(){
          cout<<"board started"<<endl;
          this->matrix_A.resize (this->width);
          this->matrix_B.resize (this->width);
          this->matrix_C.resize (this->width);
          for (int y = 0; y < this->height; y++){
               this->matrix_C[y].resize(this->width);
               for (int x = 0; x < this->width; x++){
                    this->matrix_C[y][x].resize(3);     
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
          for (int y = 0; y < this->height-1; y++){
               for (int x = 0; x < this->width-1; x++){
                    int wrappedYUnder=  (y-1)<0              ? this->height-1 :y-1;
                    int wrappedYOver =  (y+1)>this->height-1 ? 0              :y+1;
                    int wrappedXUnder=  (x-1)<0              ? this->width-1  :x-1;
                    int wrappedXOver =  (x+1)>this->width-1  ? 0              :x+1;

                    Cell* n1=this->get_cell_by_XY(x,wrappedYUnder);
                    Cell* n2=this->get_cell_by_XY(wrappedXOver,wrappedYUnder);
                    Cell* n3=this->get_cell_by_XY(wrappedXOver,y);
                    Cell* n4=this->get_cell_by_XY(wrappedXOver,wrappedYOver);
                    Cell* n5=this->get_cell_by_XY(x,wrappedYOver);
                    Cell* n6=this->get_cell_by_XY(wrappedXUnder,wrappedYOver);
                    Cell* n7=this->get_cell_by_XY(wrappedXUnder,(y));
                    Cell* n8=this->get_cell_by_XY(wrappedXUnder,wrappedYUnder);

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

                    this->matrix_A[x][y].set_n(n1,n2,n3,n4,n5,n6,n7,n8);
                    this->matrix_B[x][y].set_n(nb1,nb2,nb3,nb4,nb5,nb6,nb7,nb8);    
               }
          }
          cout<< "neighbors set"<<endl;  
     }
     Cell* get_cell_by_XY(int x, int y){
          //the refference of the cell at [y][x] of the the matrix of pointer
          return &(*get_current_matrix())[y][x];
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
          cout<<"we take a step:"<<endl;
          vector<vector<Cell>>* current_matrix = this->get_current_matrix();
          vector<vector<Cell>>* next_matrix = this->get_next_matrix();
          for(int y=0;y<this->height-1;y++){
               for(int x=0;x<this->width-1;x++){
                    int l = (*current_matrix)[y][x].game_of_life();
                    int c= (*next_matrix)[y][x].set_life_get_count(l);
                    this->set_pixel(x,y,l,c);
               }    
          }
          this->flip_matrix();
     }
     void render(int i){
          this->render_images(i);
          this->render_video();
     }
     void render_images(int i){
          int render_count = i;
          string function_name = "render_images";
          for(int file_count =0;file_count<render_count;file_count++){
               auto start = std::chrono::high_resolution_clock::now();
               this->step();
               this->write_file(file_count);
               auto stop = std::chrono::high_resolution_clock::now();
               auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
               cout<<function_name << "  which took: "<<duration.count() << "\n";
          }
     }
     void write_file(int i){
          char file_name[50];
          int file_name_length;
          file_name_length=sprintf(file_name, "images\\game_of_life_test_%05d.ppm\0",  i);
          char file_test_name[50];
          int file_test_name_length;
          file_test_name_length=sprintf(file_test_name,"tests\\game_of_life_test_%d.txt\0",i);              
          FILE *file = fopen(file_name, "wb");      
          ofstream test_file(file_test_name);
          test_file<< "render test: ", i, "\n\n";
          fprintf(file, "P6\n%d %d\n255\n", this->width-1, this->height-1);
          test_file<<"[\n";
          for(int y =0;y<this->height-1;y++){
               test_file<< " [";
               for(int x =0;x<this->width-1;x++){
                    this->load_color(x,y);
                    fwrite(this->_color,1,3, file);
                    int l = (*this->get_current_matrix())[y][x].get_life();
                    int other_l = (*this->get_next_matrix())[y][x].get_life();
                    test_file<< "(x:"<< x<<" y:"<< y<<" c_m:"<<l<<", n_m:: "<<other_l<<"),";
               }
               test_file<<"],\n";
          }
          test_file<<"]";
          fclose(file);
          test_file.close();
     }
     void load_color(int x,int y){
          this->_color[0]=this->matrix_C[y][x][0];  
          this->_color[1]=this->matrix_C[y][x][1];
          this->_color[2]=this->matrix_C[y][x][2];
     }
     void render_video(){
          //todo - python scipt call/replace.
          std::string command = "python3 imgToVid.py";

          FILE *python_render_script= popen(command.c_str(),"r");
          fscanf(python_render_script, command.c_str());
     }
     void set_pixel(int x, int y,int l,int c){
          if(l){

               this->matrix_C[y][x][0]=255;
               this->matrix_C[y][x][1]=255;
               this->matrix_C[y][x][2]=255;
               
               }else{  
               this->matrix_C[y][x][0]=0;
               this->matrix_C[y][x][1]=0;
               this->matrix_C[y][x][2]=0;
               
               }
     }
     void set_random_pixel(){
          int x=rand()%(this->width-1);
          int y=rand()%(this->height-1);
          this->set_pixel(x,y,1,1);
          (*this->get_current_matrix())[y][x].set_life(1);
          (*this->get_next_matrix())[y][x].set_life(1);

     }
     void set_pixel_and_matrixies(int x, int y){
          
          this->set_pixel(x,y,1,1);
          (*this->get_current_matrix())[y][x].set_life(1);
          (*this->get_next_matrix())[y][x].set_life(1);

     }

};

int main(){
     cout << "making game of life:" << endl;
     Board t;
     cout<<"board made"<<endl;
     vector<vector<int>> test_points {
          {0,0},{1,0},{2,0},{16,10},{17,10},{18,10},{10,12},{11,12},{12,12},{13,12},
          {14,12}, {_width-1,_height-1},{40,40},{40,41},{40,42},{41,42},{42,41},{31,31},{31,32},{31,33}
     };
     for(vector<int> item: test_points){
     t.set_pixel_and_matrixies(item[0],item[1]);    
     }
     Cell* test = t.get_cell_by_XY(25,25);
     for(Cell* n : test->neighbors){
          n->set_life(1);
     }
     t.write_file(100); 
          
          t.render(100);

     cout<< "completed" <<endl;
     cout<<"closing in: ";
     cout<<"\b \b3";
     std::chrono::seconds one_second(30);
     std::this_thread::sleep_for(one_second);
     cout<<"\b \b2";
     std::this_thread::sleep_for(one_second);
     cout<<"\b \b1... just kidding... 30 seconds";
     std::this_thread::sleep_for(one_second);
     return 0;
}


