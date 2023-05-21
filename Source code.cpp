#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX 5
typedef enum action_tag{ARRIVE,DEPART}action_type;
typedef enum boolean{FALSE,TRUE}Boolean;
typedef struct plane_detail
{
int id;//id number of plane
int tm;//time of arrival in queue
}Plane;
typedef struct plane_queue
{
int fp;
int count;
int rp;
Plane p[MAX];
}Queue;

void initialize(Queue *p)
{
p->fp=0;
p->count=0;
p->rp=-1;
}
void start(int *endtime,double *expectarrive,double *expectdepart)
{
Boolean ok;

int opt;
printf("Welcome to Airport Simulation Program\n");
printf("Only a Single Runway is Available\n");
printf("The waiting list of landing or departure has only %d slots\n",MAX);
printf("How many units of time will the simulation run?\n");
scanf("%d",endtime);
//***********Randomize();
do{
printf("How many expected arrivals per unit time?\n");
scanf("%lf",expectarrive);
printf("How many expected departures per unit time?\n");
scanf("%lf",expectdepart);
if(*expectarrive<0.0||*expectdepart<0.0){
printf("These numbers should be non negative\n");
ok=FALSE;
}
else if(*expectarrive+*expectdepart>1.0){
printf("Airport will become saturated\nRead new numbers?(enter 1 or
0)\n");
scanf("%d",&opt);
if(opt)
ok=FALSE;
else
ok=TRUE;
}
else ok=TRUE;

}
while(ok==FALSE);
}

int RandomNumber ( double expectedvalue )
{
int n = 0 ;
double em ;
double x ;

em = exp ( -expectedvalue ) ;
x = rand( ) / ( double ) INT_MAX ;

while ( x > em )
{
n++ ;
x *= rand( ) / ( double ) INT_MAX ;
}

return n ;
}
void NewPlane(Plane *p,int *nplanes,int curtime,action_type kind)
{
(*nplanes)++;
p->id=*nplanes;

p->tm=curtime;
switch(kind)
{
case ARRIVE: printf("\tPlane %3d ready to land.\n",*nplanes);
break;
case DEPART: printf("\tPlane %3d ready to takeoff.\n",*nplanes);
break;
}
}
int Full(Queue *p)
{
if (p->count==MAX)
return 1;
else return 0;
}
void Refuse(Plane p,int *nrefuse,action_type kind)
{
switch(kind)
{
case ARRIVE: printf("plane %3d directed to another airport.\n",p.id);
break;
case DEPART: printf("plane %3d told to try later.\n",p.id);
break;
}
(*nrefuse)++;

}
void AddQueue(Plane plane,Queue *q)
{
(q->count)++;
q->rp=(q->rp+1)%MAX;
q->p[q->rp]=plane;
}
int Empty(Queue *p)
{
if(p->count==0)
return 1;
else return 0;
}
void DeleteQueue(Plane *plane,Queue *q)
{
*plane=q->p[q->fp];
q->count--;
q->fp=(q->fp+1)%MAX;
}
void Land(Plane p,int curtime,int *nland,int *landwait)
{
int wait;
wait=curtime-p.tm;
printf("%3d : Plane %3d landed; in queue for %d units.\n",curtime,p.id,wait);
(*nland)++;

*landwait+=wait;
}
void Fly(Plane p,int curtime,int *ntakeoff,int *takeoffwait)
{
int wait;
wait=curtime-p.tm;
printf("%3d : Plane %3d took off; in queue for %d units\n",curtime,p.id,wait);
(*ntakeoff)++;
*takeoffwait+=wait;
}
void Idle(int curtime,int *idletime)
{
printf("%3d : Runway is idle.\n",curtime);
(*idletime)++;
}

int Size(Queue *q)
{
return q->count;
}
void Conclude(int nplanes,int nland,int ntakeoff,int nrefuse,int landwait,int
takeoffwait,int idletime,int endtime,Queue *pt,Queue *pl)
{
printf("Simulation has concluded after %d units.\n",endtime);
printf("Total number of planes processed: %3d\n",nplanes);
printf(" Number of planes landed: %3d\n",nland);

printf(" Number of planes taken off: %3d\n",ntakeoff);
printf(" Number of planes refused use: %3d\n",nrefuse);
printf(" Number left ready to land: %3d\n",Size(pl));
printf(" Number of planes take off %3d\n",Size(pt));
if(endtime>0)
printf(" Percentage of time runway idle:
%6.2f\n",((double)idletime/endtime)*100.0);
if(nland>0)
printf(" Average wait time to land: %6.2f\n",((double)landwait/nland));
if(ntakeoff>0)
printf(" Average wait time to take off:
%6.2f\n",((double)takeoffwait/ntakeoff));
}
int main()
{
Queue landing,takeoff;
Queue *pl=&landing,*pt=&takeoff;

Plane plane;
int curtime,endtime; //one unit=time taken for landing or takeoff,total number of
units
double expectarrive,expectdepart;//number of planes in one unit of time
int i;
int idletime,landwait/*total waiting time for landed
planes*/,takeoffwait,nland,nplanes,nrefuse,ntakeoff;
int pri/*random integer*/;
initialize(pl);initialize(pt);

nplanes=nland=ntakeoff=nrefuse=0;
landwait=takeoffwait=idletime=0;
start(&endtime,&expectarrive,&expectdepart);
for(curtime=1;curtime<=endtime;curtime++){
pri=RandomNumber(expectarrive);
for(i=1;i<=pri;i++){ //for landing queue
NewPlane(&plane,&nplanes,curtime,ARRIVE);
if(Full(pl))
Refuse(plane,&nrefuse,ARRIVE);
else AddQueue(plane,pl);
}
pri=RandomNumber(expectdepart);
for(i=1;i<=pri;i++){ //for landing queue
NewPlane(&plane,&nplanes,curtime,DEPART);
if(Full(pt))
Refuse(plane,&nrefuse,DEPART);
else AddQueue(plane,pt);
}
if(!Empty(pl)){//land the plane
DeleteQueue(&plane,pl);
Land(plane,curtime,&nland,&landwait);
}
else if(!Empty(pt)){//takeoff the plane
DeleteQueue(&plane,pt);
Fly(plane,curtime,&ntakeoff,&takeoffwait);

}
else
Idle(curtime,&idletime);
}
Conclude(nplanes,nland,ntakeoff,nrefuse,landwait,takeoffwait,idletime,endtime,pt,
pl);
}
