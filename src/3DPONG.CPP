//3D PONG HAS BEEN COMPLETED, AND IT IS EXECUTABLE

//ENJOY

#include<fstream.h>
#include<iostream.h>
#include<conio.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<dos.h>


#include<math.h>

#include<graphics.h>

const float MAG = 10, DT = 1;


const float XFLD= 640*MAG, YFLD= 440*MAG, ZFLD= 450*MAG;


const float EWALL=0.999; //COEFF OF RESTITUTION OF WALL

const int CRACKMAX = 7;

//CLASS VECTOR


class vector
{
 public:


 float i, j, k;


 vector(float a=0, float b=0, float c=0)
 {
  i=a, j=b, k=c;
 }


 float mod()
 {
  float x = pow(i*i+j*j+k*k,(float)1/2); //
 return x;
 }



 float dot(vector B)
 {
  return (i*B.i+j*B.j+k*B.k);
 }


 vector cross(vector B)
 {
  vector C(j*B.k-k*B.j,k*B.i-i*B.k,i*B.j-j*B.i);


  return C;
 }


 void equalto(vector B, float c=1)
 {
  i=B.i*c;
  j=B.j*c;
  k=B.k*c;
 }


 vector on(vector B)
 {
	 vector A(i,j,k);
	 float x = A.dot(B)/(B.mod()*B.mod());
	 A.equalto(B,x);
	 return A;
 }


 vector off(vector B)
 {
	 vector a = on(B);
	 vector C(i-a.i,j-a.j,k-a.k);
     return C;
 }


 vector unitvector()
 {
	 vector A(i/mod(),j/mod(),k/mod());


	 return A;
 }






};



class ball
{

 public:

	vector p, v, s, a; int b,r;

	void bounce(vector A, float e)
	{
		//if(oncheck(A,c,width,r))
		//{
		vector left=v.off(A);
		vector right=v.on(A);
		right.equalto(v.on(A),float (-e));


		vector net(left.i+right.i,left.j+right.j,left.k+right.k);




		vector l=A.unitvector();


		v.equalto(net);


		b++;

	}





	void move(float t)
	{
		p.i+=v.i*t;
		p.j+=v.j*t;
		p.k+=v.k*t;
	}


	void accelerate(float t)
	{
		v.i+=a.j*t;
		v.j+=a.j*t;
		v.k+=a.k*t;
	}


};



class pad
{
 public:

 vector p, A; float l, e, h, x1, x2, y1, y2,slope; int bcol;

 void move(char ch)
 {

  if(ch=='a')
  {
   if(p.i>0)
    p.i-=10*MAG;
  }
  else if(ch=='w')
  {
   if(p.j<YFLD-h)
    p.j+=10*MAG;
  }
  else if(ch=='d')
  {
   if(p.i<XFLD-l)
    p.i+=10*MAG;
  }
  else if(ch=='s')
  {
   if(p.j>0)
    p.j-=10*MAG;
  }

 }


 int bounce(ball &B)
 {
  vector ORIG = A;

  int i=(B.p.k>=p.k)? 1:0;

  int bound=0;
  B.accelerate(DT);
  B.move(DT);

  if(i>0)
  {
   if(B.p.k<p.k)
   {
    if((B.p.i-p.i<=l+B.r*MAG)&&(B.p.j-p.j<=h+B.r*MAG)&&(B.p.i>p.i-B.r*MAG)&&(B.p.j>p.j-B.r*MAG))
    {
     bound++;
     if(B.p.i-p.i<x1)
     {
      A.i=-slope;
     }
     if(B.p.i-p.i>x2)
     {
      A.i=slope;
     }
     if(B.p.j-p.j<y1)
     {
      A.j=-slope;
     }
     if(B.p.j-p.j>y2)
     {
      A.j=slope;
     }

     B.bounce(A,e);
     B.move(DT);
    }
   }
  }
  /*else
  {
   if(B.p.k>p.k)
   {
    if((B.p.i-p.i<=l)&&(B.p.j-p.j<=h)&&(B.p.i>p.i)&&(B.p.j>p.j))
    {
     B.bounce(A,e);

     B.move(-DT);

     bound++;

    }

   }
  }*/
  A.equalto(ORIG);

  return bound;
 }

};



int wallbounce(ball &B)  //1,2,3,4 in order of BLUR, 5 for Front, 0 for none.
{
	if(B.p.i+B.r*MAG>XFLD)
	{
		vector A(-1,0,0);
		B.p.i=XFLD-B.r*MAG;
		B.bounce(A,EWALL);
		return 4;
	}
	if(B.p.i<B.r*MAG)
	{
		vector A(1,0,0);
		B.p.i=B.r*MAG;
		B.bounce(A,EWALL);
		return 2;
	}
	if(B.p.j>YFLD-B.r*MAG)
	{
		vector A(0,-1,0);
		B.p.j=YFLD-B.r*MAG;
		B.bounce(A,EWALL);
		return 1;
	}
	if(B.p.j<B.r*MAG)
	{
		vector A(0,1,0);
		B.p.j=B.r*MAG;
		B.bounce(A,EWALL);
		return 3;
	}
	if(B.p.k>ZFLD-B.r*MAG)
	{
	 vector A(0,0,-1);
	 B.p.k=ZFLD-B.r*MAG;
	 B.bounce(A,EWALL);
	 return 5;
	}

	return 0;
}

void standard(pad &P)
{

 vector A(0,0,1);

 vector p(10*MAG,10*MAG,1);

 P.A.equalto(A);
 P.p.equalto(p);

 P.l=150*MAG;
 P.h=100*MAG;

 P.x1=20*MAG;
 P.x2=130*MAG;
 P.y1=15*MAG;
 P.y2=85*MAG;
 P.slope=0.25;

 P.e=1.1;

 P.bcol=3;

}

void standard(ball &B)
{
//vector p, v, s, a; int b;

  vector P(150,150,150); B.r=12;

  //FIXED VEL: vector V(60,20,80);

  vector V(random(40)+40,15+random(15),40+random(45));

  vector A(0,0,0);

  B.p.equalto(P);

  B.v.equalto(V);

  B.a.equalto(A);

}

void fill_rect(int a, int b, int x, int y)
{
 int A[8]={a,b,x,b,x,y,a,y};

 fillpoly(4,A);

}
void framemove(char ch1, char ch2, int &x, int &y, float &MMAX)
{
 if(ch2==77) x++;
 if(ch2==75) x--;
 if(ch2==72) y++;
 if(ch2==80) y--;

 if(ch1=='t') MMAX+=0.1;
 if(ch1=='g') MMAX-=0.1;

 if(MMAX<1) MMAX = 1;
 if(MMAX>200) MMAX = 200;

 if(x>XFLD/MAG-XFLD/(MAG*MMAX)) x= XFLD/MAG-XFLD/(MAG*MMAX);
 if(x<0) x=0;
 if(y>YFLD/MAG-YFLD/(MAG*MMAX)) y= YFLD/MAG-YFLD/(MAG*MMAX);
 if(y<0) y=0;
}

void framedisplay(int xfar, int yfar, float MMAX, int face)
{
 int x = xfar, y = yfar;

 int L[8]={0,0,0,YFLD/MAG,x,y+YFLD/(MAG*MMAX),x,y};

 int R[8]={XFLD/MAG,0,XFLD/MAG,YFLD/MAG,x+XFLD/(MAG*MMAX),y+YFLD/(MAG*MMAX),x+XFLD/(MAG*MMAX),y};

 int U[8]={0,0,x,y,x+XFLD/(MAG*MMAX),y,XFLD/(MAG),0};

 int B[8]={XFLD/MAG,YFLD/MAG,0,YFLD/MAG,x,y+YFLD/(MAG*MMAX),x+XFLD/(MAG*MMAX),y+YFLD/(MAG*MMAX)};

 int F[8]={x,y,x+XFLD/(MAG*MMAX),y,x+XFLD/(MAG*MMAX),y+YFLD/(MAG*MMAX),x,y+YFLD/(MAG*MMAX)};

 for(int i=1;i<8;i+=2)
 {
  L[i]=479-L[i];
  U[i]=479-U[i];
  R[i]=479-R[i];
  B[i]=479-B[i];
  F[i]=479-F[i];
 }
 /*
 gotoxy(1,1);
 for(int I=0;I<8;I++)
 {
  cout<<L[I]<<" , "<<R[I]<<" , "<<U[I]<<" , "<<B[I]<<endl;
 }
 */
 setcolor(15);

 setfillstyle(1,0);

 if(face==1)
  setfillstyle(11,15);

 fillpoly(4,B);

 setfillstyle(1,0);

 if(face==2)
  setfillstyle(11,15);

 fillpoly(4,L);

 setfillstyle(1,0);

 if(face==3)
  setfillstyle(11,15);

 fillpoly(4,U);

 setfillstyle(1,0);

 if(face==4)
  setfillstyle(11,15);

 fillpoly(4,R);

 if(face==5)
  setfillstyle(11,15);

 fillpoly(4,F);

 setfillstyle(1,0);

}

void paddisplay(pad P, int bounce)
{

 float a = P.p.i/MAG;
 float b = 479-P.p.j/MAG;
 float c = P.p.i/MAG+P.l/MAG;
 float d = 479-P.p.j/MAG-P.h/MAG;

 int p = a, q = b, r = c, s = d;

 //gotoxy(1,2);cout<<"|"<<p<<"|"<<q<<"|"<<r<<"|"<<s;

 setcolor(15);

 if(bounce)
 {
  setfillstyle(1,15);
 }
 else
  setfillstyle(11,P.bcol);

 fill_rect(p,q,r,s);
 /*
 a = (P.p.i+P.x1)/MAG;
 b = 479-(P.p.j+P.y1)/MAG;
 c = (P.p.i+P.l - P.x2)/MAG/MAG;
 d = 479-(P.p.j+P.h - P.y2)/MAG;

 p = a, q = b, r = c, s = d;

 if(bounce)
 {
  setfillstyle(1,15);
 }
 else
  setfillstyle(10,7);

 setcolor(0);

 fill_rect(p,q,r,s);
 */
}

void balldisplay(ball B, int xfar, int yfar, float MMAX)
{
 int RMAX = B.r;

 //int x = B.p.i/MAG, y=479-B.p.j/MAG;

 float mfactor = ZFLD/(ZFLD+B.p.k/MMAX-B.p.k);

 int RF = RMAX/mfactor;

 int xnet = xfar*B.p.k/ZFLD;

 int ynet = yfar*B.p.k/ZFLD;

 int x = xnet+B.p.i/(MAG*mfactor), y= 479-(ynet+(B.p.j/(MAG*mfactor)));

 setfillstyle(0,15);

 setcolor(15);

 //ball's frame:

 int A[8]={ xnet,479-ynet,xnet + XFLD/(MAG*mfactor),479-ynet,xnet + XFLD/(MAG*mfactor),479-ynet - YFLD/(MAG*mfactor),xnet,479-ynet - YFLD/(MAG*mfactor)};

 fillpoly(4,A);

 setfillstyle(1,15);

 setcolor(15);

 pieslice(x,y,0,360,RF);

 setfillstyle(0,15);

}

void scrclr()
{
 int poly[8]={1,1,640,1,640,480,1,480};

 setcolor(0);
 setfillstyle(1,0);
 fillpoly(4,poly);
 drawpoly(4,poly);

}
/*
void loser()
{
 int wtf=rand()%20;

 for(int i=0;i<wtf;i++)
 {
  cout<<"_";
 }

 cout<<"LO";
 for(int j=0;j<wtf;j++)
 {
  cout<<"O";
 }

 cout<<"OSER ........";

 loser();
}
*/

void endsequence(float xw,  float yw)
{

 for(int CRACK=3;CRACK<=CRACKMAX;CRACK+=4)
 {
  for(int z=0;z<8;z++)
  {
   int p=1, r=1, s=1, q=1;
   int xn = xw, yn = yw;

   if(z==0)
    r=0;
   else if(z==1)
    r=0, p=-1, q=-1, s=-1;
   else if(z==2)
    p=0, q=-1;
   else if(z==3)
    p=0, s=-1, r=-1;
   else if(z==5)
    p=-1, q=-1;
   else if(z==6)
    p=-1, q=-1, s=-1, r=-1;
   else if(z==7)
    s=-1, r=-1;

   for(int n1=0;n1<CRACK;n1++)
   {
    int xt = xn, yt = yn;

    xn+=8*p + q*random(7);

    yn+=8*r + s*random(7);

    line(xt,yt,xn,yn);
   }
  }

 }
}
void main()
{
 randomize();

 cout<<1;getch();//debug

 int gdriver = DETECT, gmode;

 int xfar = 100, yfar = 100, bouncenet=0;

 float MMAX=2;

 initgraph(&gdriver, &gmode, "C:\\TC\\BGI");
 cout<<1;getch();//debug

 pad P;

 standard(P);

 ball B;

 standard(B);
  char ch='d', ch2;

 int stat=1;

 while(stat)
 {
  if(B.v.k>140) B.v.k=140;
  else if(B.v.k<-140) B.v.k=-140;

  delay(30);

  scrclr();

  if(kbhit())
  {
   ch=getch();
   if(ch==0) ch2=getch();
   //P.move(ch);

   framemove(ch,ch2,xfar,yfar,MMAX);
  }

  P.move(ch);
  //DYNAMICS CALCULATION

  int bounceface = wallbounce(B);

  int bounce=P.bounce(B);

  if(bounce==1)
  {
   bouncenet++;
  }
  //DISPLAY
  framedisplay(xfar,yfar,MMAX,bounceface);

  if(B.p.k>=P.p.k) balldisplay(B,xfar,yfar,MMAX);
  else paddisplay(P,bounce);

  if(B.p.k<P.p.k) balldisplay(B,xfar,yfar,MMAX);
  else paddisplay(P,bounce);


  gotoxy(1,1); cout<<"PAD: "<<P.p.i<<" "<<P.p.j<<" "<<P.p.k<<"| BALL: "<<B.v.i<<" "<<B.v.j<<" "<<B.v.k<<" | BOUNCES: "<<bouncenet;

  if(B.p.k<-10*MAG) stat=0;


 }

 if(!stat) endsequence(B.p.i/MAG,479-B.p.j/MAG);

 gotoxy(1,1);cout<<"NEW GAME BEGINS , press any key to continue:";

 getch();

 main();

}




