#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <chrono>     
using namespace std; 
constexpr int _height=1080;
constexpr int _width =1920;


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

          for (int x = 0; x < this->width; x++){
               this->matrix_C[x].resize(this->height);
               for (int y = 0; y < this->height; y++){
                    this->matrix_C[x][y].resize(3);     
                    this->matrix_A[x].push_back(Cell(x,y));
                    this->matrix_B[x].push_back(Cell(x,y));
                    for(int c = 0; c<3;c++){
                         this->matrix_C[x][y][c]=0;     
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
          return &(*get_current_matrix())[x][y];
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
          for(int y=0;y<this->height-1;y++){
               for(int x=0;x<this->width-1;x++){
                    int l = (*current_matrix)[x][y].game_of_life();
                    int c= (*next_matrix)[x][y].set_life_get_count(l);
                    this->set_pixel(x,y,l,c);
               }    
          }
          this->flip_matrix();
     }
     void render(int i){
               this->step();
               this->write_file(i);
          
     }
     void write_file(int i){
          char file_name[50];
          int file_name_length;
          file_name_length=sprintf(file_name, "images\\game_of_life_test_%d.ppm\0", i);              
          FILE *file = fopen(file_name, "wb");      
          fprintf(file, "P6\n%d %d\n255\n", this->width-1, this->height-1);
          for(int x =0;x<this->width-1;x++){
               for(int y =0;y<this->height-1;y++){
                    this->_color[0]=this->matrix_C[x][y][0];  
                    this->_color[1]=this->matrix_C[x][y][1];
                    this->_color[2]=this->matrix_C[x][y][2];
                    fwrite(this->_color,1,3, file);

               }
          }
          fclose(file);
     }
     
     void set_pixel(int x, int y,int l,int c){
          if(l){

               this->matrix_C[x][y][0]=255;
               this->matrix_C[x][y][1]=255;
               this->matrix_C[x][y][2]=255;
               
               }else{  
               this->matrix_C[x][y][0]=0;
               this->matrix_C[x][y][1]=0;
               this->matrix_C[x][y][2]=0;
               
               }
     }
     void set_random_pixel(){
          int x=rand()%(this->width-1);
          int y=rand()%(this->height-1);
          this->set_pixel(x,y,1,1);
          (*this->get_current_matrix())[x][y].set_life(1);
          (*this->get_next_matrix())[x][y].set_life(1);

     }
     void set_pixel_and_matrixies(int x, int y){
          this->set_pixel(x,y,1,1);
          (*this->get_current_matrix())[x][y].set_life(1);
          (*this->get_next_matrix())[x][y].set_life(1);

     }
};

int main(){
     cout << "making game of life:" << endl;
     Board t;
     cout<<"board made"<<endl;
     t.set_pixel_and_matrixies(0,0);
     t.set_pixel_and_matrixies(1,0);
     t.set_pixel_and_matrixies(2,0);
     t.set_pixel_and_matrixies(10,10);
     t.set_pixel_and_matrixies(11,10);
     t.set_pixel_and_matrixies(12,10);
     t.set_pixel_and_matrixies(13,10);
     t.set_pixel_and_matrixies(14,10);
     t.set_pixel_and_matrixies(15,10);
     t.set_pixel_and_matrixies(16,10);
     t.set_pixel_and_matrixies(17,10);
     t.set_pixel_and_matrixies(18,10);
     t.set_pixel_and_matrixies(10,12);
     t.set_pixel_and_matrixies(11,12);
     t.set_pixel_and_matrixies(12,12);
     t.set_pixel_and_matrixies(13,12);
     t.set_pixel_and_matrixies(14,12);
     t.set_pixel_and_matrixies(_width-1,_height-1);
     t.set_pixel_and_matrixies(40,40);
     t.set_pixel_and_matrixies(40,41);
     t.set_pixel_and_matrixies(40,42);
     t.set_pixel_and_matrixies(41,42);
     t.set_pixel_and_matrixies(42,41);
     Cell* test = t.get_cell_by_XY(25,25);
     for(Cell* n : test->neighbors){
          n->set_life(1);
     }
     t.write_file(100);

     long average=0;
     for(int i=0;i<99999;i++){
          t.set_random_pixel();
     }

     t.write_file(101);
     for(int i=0; i<100;i++){
          auto start = std::chrono::high_resolution_clock::now();
          cout<<"rendering  "<<i;
          t.render(i);
          auto stop = std::chrono::high_resolution_clock::now();
          auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
          average = duration.count()*1/(i+1)+average;
          int iterations_left=100-i;
          long estimation = iterations_left*average;
          cout << "  which took: "<<duration.count() << " milliseconds... total time estimation is: "<< estimation<<"milliseconds left \r";

     }

std::cin.get();
}


