#include <iostream>
#include <fstream>
#include <math.h> 
#include <vector>
using namespace std;
constexpr int _height=50;
constexpr int _width=50;
class CELL{
     public:
          int life=0;
          int x;
          int y;
          CELL* neighbors[8];
          CELL(int _x,int _y){
               this->x=_x;
               this->y=_y;
          }
          bool getLife(){
               return this->life;
          }
          void set_life(int bLife){
               this->life = bLife;
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
               return (this->neighbors[0]->getLife()+
               this->neighbors[1]->getLife()+
               this->neighbors[2]->getLife()+
               this->neighbors[3]->getLife()+
               this->neighbors[4]->getLife()+
               this->neighbors[5]->getLife()+
               this->neighbors[6]->getLife()+
               this->neighbors[7]->getLife());
          }
          int game_of_life(){
               int n = this->nVal();
               int l = this->getLife();
               return (l==1)?(n>3||n<2)?0:1:(n==3)?1:0;
          }
};

class TABLE{
     public:
     int height=_height;
     int width=_width;
     vector<vector<CELL>> matrix_A;
     vector<vector<CELL>> matrix_B;
     bool current_matrix_is_a = true;
     TABLE(){
          cout<< "table function:"<<endl;  
          this->matrix_A.resize (this->width);
          this->matrix_B.resize (this->width);
          for (int x = 0; x < this->width; x++){
               for (int y = 0; y < this->height; y++){
                    matrix_A[x].push_back(CELL(x,y));
                    matrix_B[x].push_back(CELL(x,y));     
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
     }
     void render(int i){
          for(int index=0;index<i;index++){
               this->step();
               this->writeFile(index);
               this->flip_matrix();
          }
     }
     void writeFile(int i){
          vector<vector<CELL>>* current_matrix = this->getcurrentMatrix();
          std::ofstream file("game of life_test_"+std::to_string(i)+".txt");      
          file << "render:"+to_string(i)+"\n\n";    
          for(int y=0;y<this->height;y++){
               for(int x=0;x<this->width;x++){
                    int life = ((*current_matrix)[x][y].getLife());
                    file << to_string(life);
               }
               file << "\n";    
          }
          file.close();
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
     t.findCellByXY(15,12)->set_life(1);
     t.findCellByXY(16,12)->set_life(1);
     t.findCellByXY(17,12)->set_life(1);
     t.findCellByXY(18,12)->set_life(1);

     for(int i=0; i<10;i++){
          t.render(i);
     }
}