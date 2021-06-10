#include <iostream>
#include <math.h> 
#include <vector>
using namespace std;
int _HEIGHT=1080;
int _WIDTH=1920;
class CELL{
     public:
          bool life=false;
          int x;
          int y;
          CELL* neighbors[8];
          CELL(int _x,int _y){
               this->x=_x;
               this->y=_y;
          }
          bool getLife(){
               cout << "getting life:"<< this->life <<" is life!" <<endl;
               cout << "x and y are:"<< this->x << "  and  "<< this->y <<endl;
               return this->life==true?1:0;
          }
          void setLife(bool bLife){
               cout << "setting life:"<< this->life << "  to this new life:"<<bLife<<endl;
               this->life = bLife;
               cout <<":life is now"<<this->life<<" so there!!!!"<<endl;
               cout << "x and y are:"<< this->x << "  and  "<< this->y <<endl;
          }
          void setN(CELL&n1,CELL&n2,CELL&n3,CELL&n4,CELL&n5,CELL&n6,CELL&n7,CELL&n8){
               cout<<"setting n of"<< this->x<< " : "<<this->y<<endl;
               this->neighbors[0]=&n1;
               this->neighbors[1]=&n2;
               this->neighbors[2]=&n3;
               this->neighbors[3]=&n4;
               this->neighbors[4]=&n5;
               this->neighbors[5]=&n6;
               this->neighbors[6]=&n7;
               this->neighbors[7]=&n8;
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
};
 


class TABLE{
     public:
     int height=_HEIGHT;
     int width=_WIDTH;
     vector<vector<CELL>> matrix_A;
     vector<vector<CELL>> matrix_B;
     bool current_matrix_is_a = true;
     TABLE(){
          cout<< "table function:"<<endl;  
          this->matrix_A.resize (this->height);
          this->matrix_B.resize (this->height);
          for (int y = 0; y < this->height; y++)
               {
               for (int x = 0; x < this->width; x++)
                    {
                    matrix_A[y].push_back(CELL(x,y));
                    matrix_B[y].push_back(CELL(x,y));     
               }
          }
          cout<< "cells initetd :"<<endl;  
          cout<< "now setting neighbor cells initetd :"<<endl;
          this->_setNeighbors();
     }  
     void _setNeighbors(){
          try{
          for (int y = 0; y < this->height-1; y++)
               {
               for (int x = 0; x < this->width-1; x++)
                    {
                    cout<< "now setting neighbor cells x/y :"<< x <<":"<< y <<endl;       
                    int wrappedYUnder=  (y-1)<0              ? this->height-1 :y-1;
                    int wrappedYOver =  (y+1)>this->height-1 ? 0              :y+1;
                    int wrappedXUnder=  (x-1)<0              ? this->width-1  :x-1;
                    int wrappedXOver =  (x+1)>this->width-1  ? 0              :x+1;
                    cout<<wrappedYOver<< ":"<<wrappedYUnder<<":"<<wrappedXOver<<":"<<wrappedXUnder<<":"<<endl;


                    CELL n1=this->findCellByXY(wrappedYUnder,x);
                    cout<<"n1 found," <<endl;
                    CELL n2=this->findCellByXY(wrappedYUnder,wrappedXOver);
                    cout<<"n2 found," <<endl;

                    CELL n3=this->findCellByXY((y),wrappedXOver);
                    cout<<"n3 found," <<endl;

                    CELL n4=this->findCellByXY(wrappedYOver,wrappedXOver);

                    cout<<"n4 found," <<endl;
                    CELL n5=this->findCellByXY(wrappedYOver,x);

                    cout<<"n5 found," <<endl;
                    CELL n6=this->findCellByXY(wrappedYOver,wrappedXUnder);

                    cout<<"n6 found," <<endl;
                    CELL n7=this->findCellByXY((y),wrappedXUnder);

                    cout<<"n7 found," <<endl;
                    CELL n8=this->findCellByXY(wrappedYUnder,wrappedXUnder);
                    cout<<"n8 found," <<endl;
                    
                    cout<<"neighbors a found," <<endl;



                    this->current_matrix_is_a=!this->current_matrix_is_a;

                    CELL nb1=this->findCellByXY(wrappedYUnder,x);
                    CELL nb2=this->findCellByXY(wrappedYUnder,wrappedXOver);
                    CELL nb3=this->findCellByXY((y),wrappedXOver);
                    CELL nb4=this->findCellByXY(wrappedYOver,wrappedXOver);
                    CELL nb5=this->findCellByXY(wrappedYOver,x);
                    CELL nb6=this->findCellByXY(wrappedYOver,wrappedXUnder);
                    CELL nb7=this->findCellByXY((y),wrappedXUnder);
                    CELL nb8=this->findCellByXY(wrappedYUnder,wrappedXUnder);

                    cout<<"neighbors b found," <<endl;
                    this->current_matrix_is_a=!this->current_matrix_is_a;

                    cout<<"setting n," <<endl;
                    this->matrix_A[y][x].setN(n1,n2,n3,n4,n5,n6,n7,n8);
                    this->matrix_B[y][x].setN(nb1,nb2,nb3,nb4,nb5,nb6,nb7,nb8);
                    
               }
          }
          }catch(exception &e){
               cout <<e.what()<< endl;
          }         
          cout<< "neighbors set:"<<endl;  
     }
     CELL findCellByXY(int x, int y){
          try{
          CELL target = this->getcurrentMatrix()[y][x];
                    return target;

          }catch(exception& E){
               cout<< E.what() << "is error"<<endl;
          }
          CELL target(x,y);
          cout<< "bad target"<<endl;
          return target;
     }
     vector<std::vector<CELL>> getcurrentMatrix(){
          if(this->current_matrix_is_a==true){
               return this->matrix_A; 
          }
          return this->matrix_B;
     }
     int countNValOf(CELL &c){
          int nVal= c.nVal();
          return nVal;
     }
};



int main()
{
     cout << "Hello World" << endl;
     TABLE t;
     t.findCellByXY(10,10).setLife(true);
     int test = t.findCellByXY(10,10).getLife();
     CELL  testCell = t.findCellByXY(10,11);
     cout<<"test cell done?" <<endl;

     cout << testCell.x <<testCell.y <<" should be 10 and 11?" <<endl;
     t.countNValOf(testCell);     
     cout << test << "is test" << endl;
     for(int i=0; i<1000;i++){
          cout << test<<endl;
     }
}