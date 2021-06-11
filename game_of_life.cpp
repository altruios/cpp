#include <iostream>
#include <fstream>
#include <math.h>

#include <vector>
using namespace std;
constexpr int _height=50;
constexpr int _width=50;
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
          
          int get_g(){
               return this->g;
          }
          
          int get_r(){
               return this->r;
          }
          int get_b(){
               return this->b;
          }
          void color_step(){
               this->r= min(255,this->c*29);
               this->b= min(255,this->c*59);
               this->g= min(255,this->c*47);
          }
          void set_life(int _Life){
               this->life = _Life;
          }
          void setN(CELL* n1,CELL* n2,CELL* n3,CELL* n4,CELL* n5,CELL* n6,CELL* n7,CELL* n8){
               this->neighbors[0]=n1;
               this->neighbors[1]=n2;
               this->neighbors[2]=n3;
               this->neighbors[3]=n4;
               this->neighbors[4]=n5;
               this->neighbors[5]=n6;
               this->neighbors[6]=n7;
               this->neighbors[7]=n8;
          }
          int nVal(){
               int n=0;
               for (CELL* c : this->neighbors){
                    n+=c->get_life();
               }
               return n;
          }
          int game_of_life(){
               int n = this->nVal();
               int l = this->get_life();
               int g = (l==1)?(n>3||n<2)?0:1:(n==3)?1:0;
               if(g==l){
                    this->c++;
               }else{
                    this->c=0;
               }
               this->color_step();

               return g;

          }
};

class TABLE{
     public:
     int pixelsSize = _height*_width*3;
     unsigned char* pixels=(unsigned char*)malloc(this->width*this->height*3);
     int height=_height;
     int width=_width;
     vector<vector<CELL>> matrix_A;
     vector<vector<CELL>> matrix_B;
     vector<vector<RGB>>  rMatrix;
     bool current_matrix_is_a = true;
     TABLE(){
          this->matrix_A.resize (this->width);
          this->matrix_B.resize (this->width);
          this->rMatrix.resize (this->width);

          for (int x = 0; x < this->width; x++){
               for (int y = 0; y < this->height; y++){
                    this->matrix_A[x].push_back(CELL(x,y));
                    this->matrix_B[x].push_back(CELL(x,y));     
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

                    this->matrix_A[x][y].setN(n1,n2,n3,n4,n5,n6,n7,n8);
                    this->matrix_B[x][y].setN(nb1,nb2,nb3,nb4,nb5,nb6,nb7,nb8);    
               }
          }
          cout<< "neighbors set"<<endl;  
     }
     CELL* findCellByXY(int x, int y){
          return &(*getcurrentMatrix())[x][y];
     }
     vector<std::vector<CELL>>* getcurrentMatrix(){
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
     int countNValOf(CELL &c){
          int nVal= c.nVal();
          return nVal;
     }
     void flip_matrix(){
          this->current_matrix_is_a = !this->current_matrix_is_a;
     }
     void step(){
          vector<vector<CELL>>* current_matrix = this->getcurrentMatrix();
          vector<vector<CELL>>* next_matrix = this->get_next_matrix();

          for(int y=0;y<this->height-1;y++){
               for(int x=0;x<this->width-1;x++){
                    int step = (y*this->width+x);
                    (*next_matrix)[x][y].set_life((*current_matrix)[x][y].game_of_life());
               }    
          }
          this->flip_matrix();

     }
     void render(int i){
          for(int index=0;index<i;index++){
               this->step();
               this->writeFile(index);
          }
     }
     void render_image(){
          vector<vector<CELL>>* current_matrix = this->getcurrentMatrix();
          for(int x=0;x<this->width;x++){
               for(int y=0;y<this->height;y++){
                    this->setPixel(x,y,current_matrix);
               }
          }
     }
     void writeFile(int i){
          std::ofstream file("game of life_test_"+std::to_string(i)+".txt");      
          this->render_image();
          file << "render:"+to_string(i)+"\n\n";    
          for(int x =0;x<this->width;x++){
               for(int y =0;y<this->height;y++){
                    file<< this->rMatrix[x][y].r;
                    file<< this->rMatrix[x][y].g;
                    file<< this->rMatrix[x][y].b;
               }
          }



          file.close();
     }
     void setPixel(int x, int y, vector<vector<CELL>>* current_matrix)
          {
          RGB color;
          color.r = ((*current_matrix)[x][y].get_r());
          color.g = ((*current_matrix)[x][y].get_g());
          color.b = ((*current_matrix)[x][y].get_b());
          this->rMatrix[x][y]=color;
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
     
     CELL* test = t.findCellByXY(25,25);
     for(CELL* n : test->neighbors){

          n->set_life(1);
     }


     cout<<"writing to test file"<<endl;
     t.writeFile(100);

     for(int i=0; i<10;i++){
          cout<<"rendering  "<<i<<endl;
          t.render(i);
     }

std::cin.get();
}


