//#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define FILEN 64800 //1368
#define POLYN 25

struct Polygon{
	//char age[10];
	int nvert;
	float vertx[3000];
	float verty[3000];
}poly[12932];


float calculateSD(float data[],int number) {
    float sum = 0.0, mean, SD = 0.0;
    int i;
    for (i = 0; i < number; ++i) {
        sum += data[i];
    }
    mean = sum / number;
    for (i = 0; i < number; ++i)
        SD += pow(data[i] - mean, 2);
    return sqrt(SD / number);
}

int pnpoly(int nvert, float vertx[], float verty[], float testx, float testy){
	int i,j,c=0;
	for(i=0,j=nvert-1; i<nvert; j=i++){
		if (((verty[i]>testy)!=(verty[j]>testy)) && (testx<(vertx[j]-vertx[i])*(testy-verty[i])/(verty[j]-verty[i])+vertx[i]))
			c=!c;
	}
	return c;
}

int main(int agrc, char *argv[]){
	FILE *p1,*p2,*p3,*p4,*p5;
	char input_s1[255],input_s2[255],input_s3[255],input_s4[255],input_s5[255];
	char rdn[255], polynn[255], outputn[255], labn[255];
	int n,i,j,polyno=-1,f,count;
	float testx,testy,x,y,original,modified,testx1,testy1,topo,lithod,resi;
	float average[4][255]={0.0},averagevalue,averagelithod;
	float resitopo[POLYN][10000], lithodarray[POLYN][10000], area[POLYN]; 
	int countave[255]={0};

	if (agrc < 4){
        fprintf(stderr,"Insufficient parameters!");
        exit(0);
    }

	sprintf(rdn,"%s",argv[1]);
	sprintf(polynn,"%s",argv[2]);
	sprintf(outputn,"%s",argv[3]);
	sprintf(labn,"%s",argv[4]);
	if((p1=fopen(rdn,"r"))==NULL){
		printf("Can not open the residual topography file.\n");
		exit(0);
	}
	if((p2=fopen(polynn,"r"))==NULL){
		printf("Can not open craton polygon file.\n");
		exit(0);
	}

/*
	if((p4=fopen("xyz-bd1_2","r"))==NULL){
		printf("Can not open the observed topography file\n");
		exit(0);
		}
*/
	if((p3=fopen(outputn,"w"))==NULL){
        printf("Can not open the craton-value for the scatter plot.\n");
        exit(0);

	}

    if((p5=fopen(labn,"r"))==NULL){
        printf("Can not open the Lithosphere depth file\n");
        exit(0);
        }


	printf("Step 1 finished!\n");

	//Calculate the number of polygons
	fgets(input_s2,255,p2);
	while(!feof(p2)){
		fgets(input_s3,255,p2);
		if(input_s2[0]=='>'&&input_s3[0]!='>'){
			polyno++;
			n=0;
	//		strcpy(poly[polyno].age,input_s2);
			printf("This is PolyNo.%d!\n",polyno);
		}
		else if(input_s3[0]!='>'){
			sscanf(input_s2,"%f %f",&x,&y);
			poly[polyno].vertx[n]=x;
			poly[polyno].verty[n]=y;
			poly[polyno].nvert=++n;
//			printf("PolyNo.%d has %d vertexs!\n",polyno,n);
		}
		strcpy(input_s2,input_s3);
	}

	printf("Step 2 finished!\n");

	//judge whether the point is within the polygon or not
	for(j=0;j<FILEN;j++){
		fgets(input_s1,255,p1);
		sscanf(input_s1,"%f %f %f",&testx,&testy,&original);
//		printf("original: %f %f %f\n",testx,testy,original);
		
//		fgets(input_s4,255,p4);
//		printf("after fgets\n");
//		sscanf(input_s4,"%f %f %f",&testx1,&testy1,&topo);
//		printf("modified: %f %f %f\n",testx1,testy1,modified);	
		fgets(input_s5,255,p5);
		sscanf(input_s5,"%f %f %f",&testx1,&testy1,&lithod);
		for(i=0;i<POLYN;i++){
			f=pnpoly(poly[i].nvert,poly[i].vertx,poly[i].verty,testx,testy);
			if(f!=0) break;

		}
		
//		resi=topo-original;
		resi=original;
//		printf("after pnpoly function\n");
//		if(f!=0 )
//			fprintf(p3,"%f %f %f %f\n",testx,testy,resi,lithod);
		if(f!=0){
			average[0][i]=average[0][i]+testx;
			average[1][i]=average[1][i]+testy;
			average[2][i]=average[2][i]+resi;
			average[3][i]=average[3][i]+lithod;
			lithodarray[i][countave[i]]=lithod;
			resitopo[i][countave[i]]=resi;
			countave[i]=countave[i]+1;
			area[i]=area[i]+sin((90-testy)/180*3.1415926);
//			if(i==6)
//				printf("latitude=%f, area[%d]=%f\n",testy,i,area[i]);

		}


	}
    averagevalue=0.0;
    count=0;
	averagelithod=0.0;
	for(i=0;i<POLYN;i++){
		fprintf(p3,"%f %f %f %f %f %f %f %f\n",average[0][i]/countave[i],average[1][i]/countave[i],average[2][i]/countave[i],average[3][i]/countave[i],calculateSD(resitopo[i],countave[i]),calculateSD(lithodarray[i],countave[i]),sqrt(area[i]),sqrt(countave[i]));
		count=count+countave[i];
		averagevalue=averagevalue+average[2][i];
		averagelithod=averagelithod+average[3][i];
	}
	averagevalue=averagevalue/count;
	averagelithod=averagelithod/count;
	printf("The averaged value for all cratons is: resitopo = %f km, LAB depth = %f km\n",averagevalue, averagelithod);
	printf("FINISHED!!!\n");

	fclose(p1);
	fclose(p2);
	fclose(p3);
//	fclose(p4);
	fclose(p5);
	return 0;
}
