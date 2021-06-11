#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <chrono>     
using namespace std;
constexpr int _height=1080;
constexpr int _width=1920;
struct RGB
{
    uint8_t r,g,b;
};

class CELL{
     public:
          int life=0;
          int x;
          int y;
          int r=0;
          int g=0;
          int b=0;
          int c=0;
          CELL* neighbors[8];
          CELL(int _x,int _y){
               this->x=_x;
               this->y=_y;
          }
          int get_life(){
               return this->life;
          }
          RGB get_color(){
               RGB C;
               C.r=this->r;
               C.g=this->g;
               C.b=this->b;
               return C;
          }
          void set_color(){
               if(this->get_life()){
               this->r= max(min(255,int(cos(this->c+1)*sin(29))),150);
               this->b= max(min(255,int(cos(this->c+1)*sin(59))),150);
               this->g= max(min(255,int(cos(this->c+1)*sin(47))),150);
               }else{
                    
               this->r= min(150,int(sin(this->c+1)*cos(29)));
               this->b= min(150,int(sin(this->c+1)*cos(59)));
               this->g= min(150,int(sin(this->c+1)*cos(47)));
               }
          }
          void set_life(int _Life){
               this->life = _Life;
               this->set_color();
          }
          void set_n(CELL* n1,CELL* n2,CELL* n3,CELL* n4,CELL* n5,CELL* n6,CELL* n7,CELL* n8){
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
               for (CELL* c : this->neighbors){
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

class TABLE{
     public:
     int height=_height;
     int width=_width;
     vector<vector<CELL>> matrix_A;
     vector<vector<CELL>> matrix_B;
     vector<vector<RGB>>  matrix_C;
     bool current_matrix_is_a = true;
     TABLE(){
          this->matrix_A.resize (this->width);
          this->matrix_B.resize (this->width);
          this->matrix_C.resize (this->width);

          for (int x = 0; x < this->width; x++){
               for (int y = 0; y < this->height; y++){
                    RGB r;     
                    this->matrix_A[x].push_back(CELL(x,y));
                    this->matrix_B[x].push_back(CELL(x,y));
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

                    CELL* n1=this->findCellByXY(x,wrappedYUnder);
                    CELL* n2=this->findCellByXY(wrappedXOver,wrappedYUnder);
                    CELL* n3=this->findCellByXY(wrappedXOver,y);
                    CELL* n4=this->findCellByXY(wrappedXOver,wrappedYOver);
                    CELL* n5=this->findCellByXY(x,wrappedYOver);
                    CELL* n6=this->findCellByXY(wrappedXUnder,wrappedYOver);
                    CELL* n7=this->findCellByXY(wrappedXUnder,(y));
                    CELL* n8=this->findCellByXY(wrappedXUnder,wrappedYUnder);

                    this->flip_matrix();

                    CELL* nb1=this->findCellByXY(x,wrappedYUnder);
                    CELL* nb2=this->findCellByXY(wrappedXOver,wrappedYUnder);
                    CELL* nb3=this->findCellByXY(wrappedXOver,y);
                    CELL* nb4=this->findCellByXY(wrappedXOver,wrappedYOver);
                    CELL* nb5=this->findCellByXY(x,wrappedYOver);
                    CELL* nb6=this->findCellByXY(wrappedXUnder,wrappedYOver);
                    CELL* nb7=this->findCellByXY(wrappedXUnder,(y));
                    CELL* nb8=this->findCellByXY(wrappedXUnder,wrappedYUnder);

                    this->flip_matrix();

                    this->matrix_A[x][y].set_n(n1,n2,n3,n4,n5,n6,n7,n8);
                    this->matrix_B[x][y].set_n(nb1,nb2,nb3,nb4,nb5,nb6,nb7,nb8);    
               }
          }
          cout<< "neighbors set"<<endl;  
     }
     CELL* findCellByXY(int x, int y){
          return &(*get_current_matrix())[x][y];
     }
     vector<std::vector<CELL>>* get_current_matrix(){
          if(this->current_matrix_is_a==true){
               return &this->matrix_A; 
          }
          return &this->matrix_B;
     }
     vector<std::vector<CELL>>* get_next_matrix(){
          if(this->current_matrix_is_a==false){
               return &this->matrix_A; 
          }
          return &this->matrix_B;
     }
     void flip_matrix(){
          this->current_matrix_is_a = !this->current_matrix_is_a;
     }
     void step(){
          vector<vector<CELL>>* current_matrix = this->get_current_matrix();
          vector<vector<CELL>>* next_matrix = this->get_next_matrix();

          for(int y=0;y<this->height-1;y++){
               for(int x=0;x<this->width-1;x++){
                    int step = (y*this->width+x);
                    (*next_matrix)[x][y].set_life((*current_matrix)[x][y].game_of_life());
                    this->set_pixel(x,y,next_matrix);
               }    
          }
          this->flip_matrix();

     }
     void render(int i){
          for(int index=0;index<i;index++){
               this->step();
               this->write_file(index);
          }
     }
     void write_file(int i){
          char file_name[50];
          int file_name_length;
          file_name_length=sprintf (file_name, "images\\game_of_life_test_%d.ppm\0", i);              
          FILE *file = fopen(file_name, "wb");      
          fprintf(file, "P6\n%d %d\n255\n", this->width, this->height);
          for(int x =0;x<this->width;x++){
               for(int y =0;y<this->height;y++){
                    unsigned char color[3];
                    color[0]=this->matrix_C[x][y].r;
                    color[1]=this->matrix_C[x][y].g;
                    color[2]=this->matrix_C[x][y].b;
                    fwrite(color, 1, 3, file);
               }
          }
          fclose(file);
     }
     void set_pixel(int x, int y, vector<vector<CELL>>* current_matrix){
          RGB color= ((*current_matrix)[x][y].get_color());
          this->matrix_C[x][y].r=color.r;
          this->matrix_C[x][y].g=color.g;
          this->matrix_C[x][y].b=color.b;
     }
};

int main(){
     cout << "Hello World" << endl;
     TABLE t;
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
     
     CELL* test = t.findCellByXY(25,25);
     for(CELL* n : test->neighbors){
          n->set_life(1);
     }
     CELL* test2 = t.findCellByXY(0,0);
     test2->set_life(1);
     test2->c=10;
     for(int i=0; i<100;i++){
          auto start = std::chrono::high_resolution_clock::now();
          cout<<"rendering  "<<i;
          t.render(i);
          auto stop = std::chrono::high_resolution_clock::now();
          auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
          cout << "which took: "<<duration.count() << " milliseconds"<<"\r";

     }

std::cin.get();
}


