#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <chrono>
#include <thread>
# define M_PI           3.14159265358979323846  /* pi */
using namespace std; 
constexpr int _height=540;
constexpr int _width =960;
constexpr int _scale = 4;


class Cell{
     public:
          int life=0;
          int x;
          int y;
          int c=1;
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
     vector<vector<Cell>> matrix_A;
     vector<vector<Cell>> matrix_B;
     vector<vector<std::array<unsigned char, 3>>>  matrix_C;
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
                  //  this->matrix_C[y][x].resize(3);     
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
               this->write_file(file_count);
               auto stop = std::chrono::high_resolution_clock::now();
               auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
               auto count_left= render_count-file_count;
               long estimation  = (duration.count()*count_left)/1000;

               this->step();

               cout<<function_name << file_count<<"of:"<<render_count<<"  which took: "<<duration.count() <<"  eta:"<<estimation<< "\r";
          }
     }
     void write_file(int i){
          char file_name[50];
          int file_name_length;
          file_name_length=sprintf(file_name, "images\\game_of_life_test_%05d.ppm\0",  i);
          FILE *file = fopen(file_name, "wb");      
          fprintf(file, "P6\n%d %d\n255\n", (this->width-1)*(this->scale), (this->height-1)*(this->scale));
               for(int y =0;y<this->height-1;y++){
                    for(int i=0;i<this->scale;i++){
                         for(int x =0;x<this->width-1;x++){
                              for(int j=0;j<this->scale;j++){
                              //this->load_color(x,y);
                              fwrite(&this->matrix_C[y][x],1,3, file);
                         }
                    }
               }
          }
          fclose(file);
     }
     void load_color(int x,int y){
          this->_color[0]=this->matrix_C[y][x][0];  
          this->_color[1]=this->matrix_C[y][x][1];
          this->_color[2]=this->matrix_C[y][x][2];
     }
     void render_video(){
          //todo - python scipt call/replace.
          std::string command = "python imgToVid.py";

          FILE *python_render_script= popen(command.c_str(),"r");
          fscanf(python_render_script, command.c_str());
     }
     void set_pixel(int x, int y,int l,int c){
          if(l){
               this->matrix_C[y][x][0]=floor(100*sin((c)/M_PI*100) +105);
               this->matrix_C[y][x][1]=floor(100*tan((c*2)/M_PI*100)+155);
               this->matrix_C[y][x][2]=floor(100*cos((c*3)/M_PI*100)+155);
               
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
     void set_neighbors_of_XY(int x,int y){
           for(Cell* c:this->matrix_A[y][x].neighbors){
               c->set_life(1);
               this->set_pixel(c->x,c->y,1,1);

           }
          for(Cell* c:this->matrix_B[y][x].neighbors){
               c->set_life(1);
               this->set_pixel(c->x,c->y,1,1);

           }
     }
     void add_glider(){
          int x=rand()%(this->width-40)+20;
          int y=rand()%(this->height-40) +20;
          vector<vector<int>> glider {{x,y},{x,y+1},{x,y+2},{x-1,y+2},{x-2,y+1}};
          for(vector<int> item: glider){
               this->set_pixel_and_matrixies(item[0],item[1]);    
          }
     }
};




int main(){
     cout << "making game of life:" << endl;
     Board t;
     cout<<"board made"<<endl;
     for(int i=0;i<4200;i++){
          t.add_glider();
     }
     t.render(10000);

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


