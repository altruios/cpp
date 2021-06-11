#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <chrono>     
using namespace std;
constexpr int _height=1080;
constexpr int _width=1920;

class RGB{
     public:
     uint8_t r,g,b;
     RGB(){
          this->r=0;
          this->g=0;
          this->b=0;
     }
     RGB(int r, int g, int b){
          this->r=r;
          this->g=g;
          this->b=b;
     }
     
};
class Cell{
     public:
          int life=0;
          int x;
          int y;
          int c=0;
          RGB color{0,0,0};
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
          RGB* get_color(){
               return &this->color;
          }
          void set_color(){
               if(this->get_life()){
               this->color.r= max(min(255,int(cos(this->c+1)*sin(29))),150);
               this->color.b= max(min(255,int(cos(this->c+1)*sin(59))),150);
               this->color.g= max(min(255,int(cos(this->c+1)*sin(47))),150);
               }else{  
               this->color.r= min(150,int(sin(this->c+1)*cos(29)));
               this->color.b= min(150,int(sin(this->c+1)*cos(59)));
               this->color.g= min(150,int(sin(this->c+1)*cos(47)));
               }
          }
          int set_life_get_count(int _Life){
               this->color_counter_step(_Life);
               this->life = _Life;
               return this->c;
          }
          void set_life(int _Life){
               this->color_counter_step(_Life);
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
          void color_counter_step(int g){
               if(g==this->get_life()){
                    this->c++;
               }else{
                    this->c=0;
               }
          }
          int game_of_life(){
               int n = this->get_n();
               int l = this->get_life();
               int g = (l==1)?(n>3||n<2)?0:1:(n==3)?1:0;
               this->color_counter_step(g);
               this->set_color();
               return g;
          }
};

class Board{
     public:
     int height=_height;
     int width=_width;
     vector<vector<Cell>> matrix_A;
     vector<vector<Cell>> matrix_B;
     vector<vector<RGB>>  matrix_C;
     bool current_matrix_is_a = true;
     Board(){
          this->matrix_A.resize (this->width);
          this->matrix_B.resize (this->width);
          this->matrix_C.resize (this->width);

          for (int x = 0; x < this->width; x++){
               for (int y = 0; y < this->height; y++){
                    RGB r;     
                    this->matrix_A[x].push_back(Cell(x,y));
                    this->matrix_B[x].push_back(Cell(x,y));
                    this->matrix_C[x].push_back(r);     
               }
          }
          this->_setNeighbors();
     }  
     void _setNeighbors(){
          for (int y = 0; y < this->height-1; y++){
               for (int x = 0; x < this->width-1; x++){
                    int wrappedYUnder=  (y-1)<0              ? this->height-1 :y-1;
                    int wrappedYOver =  (y+1)>this->height-1 ? 0              :y+1;
                    int wrappedXUnder=  (x-1)<0              ? this->width-1  :x-1;
                    int wrappedXOver =  (x+1)>this->width-1  ? 0              :x+1;

                    Cell* n1=this->findCellByXY(x,wrappedYUnder);
                    Cell* n2=this->findCellByXY(wrappedXOver,wrappedYUnder);
                    Cell* n3=this->findCellByXY(wrappedXOver,y);
                    Cell* n4=this->findCellByXY(wrappedXOver,wrappedYOver);
                    Cell* n5=this->findCellByXY(x,wrappedYOver);
                    Cell* n6=this->findCellByXY(wrappedXUnder,wrappedYOver);
                    Cell* n7=this->findCellByXY(wrappedXUnder,(y));
                    Cell* n8=this->findCellByXY(wrappedXUnder,wrappedYUnder);

                    this->flip_matrix();

                    Cell* nb1=this->findCellByXY(x,wrappedYUnder);
                    Cell* nb2=this->findCellByXY(wrappedXOver,wrappedYUnder);
                    Cell* nb3=this->findCellByXY(wrappedXOver,y);
                    Cell* nb4=this->findCellByXY(wrappedXOver,wrappedYOver);
                    Cell* nb5=this->findCellByXY(x,wrappedYOver);
                    Cell* nb6=this->findCellByXY(wrappedXUnder,wrappedYOver);
                    Cell* nb7=this->findCellByXY(wrappedXUnder,(y));
                    Cell* nb8=this->findCellByXY(wrappedXUnder,wrappedYUnder);

                    this->flip_matrix();

                    this->matrix_A[x][y].set_n(n1,n2,n3,n4,n5,n6,n7,n8);
                    this->matrix_B[x][y].set_n(nb1,nb2,nb3,nb4,nb5,nb6,nb7,nb8);    
               }
          }
          cout<< "neighbors set"<<endl;  
     }
     Cell* findCellByXY(int x, int y){
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
                    int c= (*next_matrix)[x][y].set_life_get_count((*current_matrix)[x][y].game_of_life());
                    this->set_pixel(x,y,c,(*next_matrix)[x][y].get_life(),next_matrix);
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
          fprintf(file, "P6\n%d %d\n255\n", this->width, this->height);
          for(int x =0;x<this->width;x++){
               for(int y =0;y<this->height;y++){
                    unsigned char _color[3];
                    _color[0]=this->matrix_C[x][y].r;
                    _color[1]=this->matrix_C[x][y].g;
                    _color[2]=this->matrix_C[x][y].b;
                    fwrite(_color, 1, 3, file);
               }
          }
          fclose(file);
     }
     void set_pixel(int x, int y,int c, int l, vector<vector<Cell>>* current_matrix){
          if(l){
               this->matrix_C[x][y].r= 255;
               this->matrix_C[x][y].b= 255;
               this->matrix_C[x][y].g= 255;
               }else{  
               this->matrix_C[x][y].r=0;
               this->matrix_C[x][y].b= 0;
               this->matrix_C[x][y].g= 0;
               }
              
     }
     void set_random_pixel(){
          int x=rand()%(this->width-1);
          int y=rand()%(this->height-1);
          cout<<"x: "<<x<<" y: "<<y<<endl;
          (*this->get_current_matrix())[x][y].set_life(1);
     }
};

int main(){
     cout << "Hello World" << endl;
     Board t;
     cout<<"table made"<<endl;
     t.findCellByXY(10,10)->set_life(1);
     t.findCellByXY(11,10)->set_life(1);
     t.findCellByXY(12,10)->set_life(1);
     t.findCellByXY(13,10)->set_life(1);
     t.findCellByXY(14,10)->set_life(1);
     t.findCellByXY(15,10)->set_life(1);
     t.findCellByXY(16,10)->set_life(1);
     t.findCellByXY(17,10)->set_life(1);
     t.findCellByXY(18,10)->set_life(1);

     t.findCellByXY(10,12)->set_life(1);
     t.findCellByXY(11,12)->set_life(1);
     t.findCellByXY(12,12)->set_life(1);
     t.findCellByXY(13,12)->set_life(1);
     t.findCellByXY(14,12)->set_life(1);


     t.findCellByXY(_width,_height)->set_life(1);
     t.findCellByXY(40,40)->set_life(1);
     t.findCellByXY(40,41)->set_life(1);
     t.findCellByXY(40,42)->set_life(1);
     t.findCellByXY(41,42)->set_life(1);
     t.findCellByXY(42,41)->set_life(1);
     
     Cell* test = t.findCellByXY(25,25);
     for(Cell* n : test->neighbors){
          n->set_life(1);
     }
     Cell* test2 = t.findCellByXY(0,0);
     test2->set_life(1);
     test2->c=10;
     long average=0;
     for(int i=0;i<1500;i++){
          t.set_random_pixel();
          cout <<"setting random pixel"<< i <<endl;
     }
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


