#ifndef NETWORKS_H
#define NETWORKS_H
	#include "networks.h"
#endif

Node *make_netwrk_ER(){
   Node *graph;
   int flag, maxnbrs=0, i, j, i1, i2, d1,d2;
   
   graph=(Node *) malloc(sizeof(Node)*N);
   for(i=0;i<N;i++){
     graph[i].id=i;
     graph[i].neigh=(Node **) malloc(sizeof(Node *)*BUFFER);
     graph[i].pointer=graph+i;
     graph[i].virtual_degree=0;
     graph[i].n=1;
  } 
 
   for(j=0;j<NL;j++) { //PAU: L is the number of links
     i1=IRAND(N); //PAU: Random number between 0 and NN-1
     do {
        do i2=IRAND(N); while(i2==i1); //PAU: if equal, select another node
        if((graph[i1].virtual_degree>0)&&(graph[i2].virtual_degree>0)) { //PAU: Make sure that the two nodes are not neighbours
           flag=0;
           for(d1=0;d1<graph[i1].virtual_degree;d1++) for(d2=0;d2<graph[i2].virtual_degree;d2++) 
             if((graph[i1].id==graph[i2].neigh[d2]->id)||(graph[i2].id==graph[i1].neigh[d1]->id)) flag=1; 
        }
     } while (flag==1);
     graph[i1].neigh[graph[i1].virtual_degree]=graph+i2; graph[i1].virtual_degree++; if(graph[i1].virtual_degree>maxnbrs) maxnbrs=graph[i1].virtual_degree; //PAU: Add the edge and refresh the maxnbrs variable, which is max_i(degree[i])
     graph[i2].neigh[graph[i2].virtual_degree]=graph+i1; graph[i2].virtual_degree++; if(graph[i2].virtual_degree>maxnbrs) maxnbrs=graph[i2].virtual_degree;
     if(graph[i1].virtual_degree>BUFFER || graph[i2].virtual_degree>BUFFER){
       fprintf(stderr,"ERROR: Buffer overlap\n"); return NULL;
    }
   }
   
    for(i=0;i<N;i++){
     graph[i].neigh=(Node **) realloc(graph[i].neigh,sizeof(Node *)*graph[i].virtual_degree);
     graph[i].dynamic_degree=graph[i].virtual_degree;
    }
    
    return graph;
}      


Node *make_netwrk_ER_percol(char *name){
   Node *graph;
   int flag=0, maxnbrs=0, i, j, i1, i2, d1,d2;
   FILE *foot;
   foot=fopen(name,"w");
   graph=(Node *) malloc(sizeof(Node)*N);
   for(i=0;i<N;i++){
     graph[i].id=i;
     graph[i].neigh=(Node **) malloc(sizeof(Node *)*BUFFER);
     graph[i].pointer=graph+i;
     graph[i].virtual_degree=0;
     graph[i].n=1;
  } 
 
   for(j=0;j<NL;j++) { //PAU: L is the number of links
     i1=IRAND(N); //PAU: Random number between 0 and NN-1
     do {
        do i2=IRAND(N); while(i2==i1); //PAU: if equal, select another node
        if((graph[i1].virtual_degree>0)&&(graph[i2].virtual_degree>0)) { //PAU: Make sure that the two nodes are not neighbours
           flag=0;
           for(d1=0;d1<graph[i1].virtual_degree;d1++) for(d2=0;d2<graph[i2].virtual_degree;d2++) 
             if((graph[i1].id==graph[i2].neigh[d2]->id)||(graph[i2].id==graph[i1].neigh[d1]->id)) flag=1; 
        }
     } while (flag==1);
     graph[i1].neigh[graph[i1].virtual_degree]=graph+i2; graph[i1].virtual_degree++; if(graph[i1].virtual_degree>maxnbrs) maxnbrs=graph[i1].virtual_degree; //PAU: Add the edge and refresh the maxnbrs variable, which is max_i(degree[i])
     graph[i2].neigh[graph[i2].virtual_degree]=graph+i1; graph[i2].virtual_degree++; if(graph[i2].virtual_degree>maxnbrs) maxnbrs=graph[i2].virtual_degree;
     if(graph[i1].virtual_degree>BUFFER || graph[i2].virtual_degree>BUFFER){
       fprintf(stderr,"ERROR: Buffer overlap\n"); return NULL;
    }
    fprintf(foot,"%lf %lf\n",j/(1.*N),newman_ziff_adapted(graph));
    fprintf(stderr,"%lf\r",j/(1.*N));
   }
   
    for(i=0;i<N;i++){
     graph[i].neigh=(Node **) realloc(graph[i].neigh,sizeof(Node *)*graph[i].virtual_degree);
     graph[i].dynamic_degree=graph[i].virtual_degree;
    }
    fclose(foot);
    return graph;
}      

Node *make_netwrk_ER_expercol(char *name){
   Node *graph;
   int sa,sb,flag=0,flagb=0, maxnbrs=0, i, j, i1,i1b,i2b, i2, d1,d2;
   FILE *foot;
   foot=fopen(name,"w");
   graph=(Node *) malloc(sizeof(Node)*N);
   for(i=0;i<N;i++){
     graph[i].id=i;
     graph[i].neigh=(Node **) malloc(sizeof(Node *)*BUFFER);
     graph[i].pointer=graph+i;
     graph[i].virtual_degree=0;
     graph[i].n=1;
     graph[i].cluster_size=1;
  } 
 
   for(j=0;j<NL;j++) { //PAU: L is the number of links
     i1=IRAND(N); //PAU: Random number between 0 and NN-1
     do {
//        fprintf(stderr,"\n\nfda\n\n");
        do i2=IRAND(N); while(i2==i1); //PAU: if equal, select another node
        if((graph[i1].virtual_degree>0)&&(graph[i2].virtual_degree>0)) { //PAU: Make sure that the two nodes are not neighbours
           flag=0;
           for(d1=0;d1<graph[i1].virtual_degree;d1++) for(d2=0;d2<graph[i2].virtual_degree;d2++) 
             if((graph[i1].id==graph[i2].neigh[d2]->id)||(graph[i2].id==graph[i1].neigh[d1]->id)) flag=1; 
        }
     } while (flag==1);

     if(j>1){
       	  i1b=IRAND(N);
	do {
	  do i2b=IRAND(N); while(i2b==i1b); //PAU: if equal, select another node
	  if((graph[i1b].virtual_degree>0)&&(graph[i2b].virtual_degree>0)) { //PAU: Make sure that the two nodes are not neighbours
	    flagb=0;
	    for(d1=0;d1<graph[i1b].virtual_degree;d1++) for(d2=0;d2<graph[i2b].virtual_degree;d2++) 
	      if((graph[i1b].id==graph[i2b].neigh[d2]->id)||(graph[i2b].id==graph[i1b].neigh[d1]->id)) flagb=1; 
	  }
      } while (flagb==1);
      sa=findroot(graph+i1)->cluster_size*findroot(graph+i2)->cluster_size;
      sb=findroot(graph+i1b)->cluster_size*findroot(graph+i2b)->cluster_size;
      if(sb<sa){
	i1=i1b; i2=i2b;
      }
     }
     graph[i1].neigh[graph[i1].virtual_degree]=graph+i2; graph[i1].virtual_degree++; if(graph[i1].virtual_degree>maxnbrs) maxnbrs=graph[i1].virtual_degree; //PAU: Add the edge and refresh the maxnbrs variable, which is max_i(degree[i])
     graph[i2].neigh[graph[i2].virtual_degree]=graph+i1; graph[i2].virtual_degree++; if(graph[i2].virtual_degree>maxnbrs) maxnbrs=graph[i2].virtual_degree;
     if(graph[i1].virtual_degree>BUFFER || graph[i2].virtual_degree>BUFFER){
       fprintf(stderr,"ERROR: Buffer overlap\n"); return NULL;
    }
    fprintf(foot,"%lf %lf\n",j/(1.*N),newman_ziff_adapted(graph));
    fprintf(stderr,"%lf\r",j/(1.*N));
   }
   
    for(i=0;i<N;i++){
     graph[i].neigh=(Node **) realloc(graph[i].neigh,sizeof(Node *)*graph[i].virtual_degree);
     graph[i].dynamic_degree=graph[i].virtual_degree;
    }
    fclose(foot);
    return graph;
}     

Node *make_netwrk_SF(double degree){
   Node *graph;	
   int m,i,ii, j,jj, *sequence;
   
   graph=(Node *) malloc(sizeof(Node)*N);
   for(i=0;i<N;i++){
     graph[i].id=i;
     graph[i].neigh=(Node **) malloc(sizeof(Node *)*BUFFER);
     graph[i].pointer=graph+i;
     graph[i].virtual_degree=0;
     graph[i].dynamic_degree=0;
     graph[i].n=1;
  } 
  fprintf(stderr,"\nComputing sequence\n");
  sequence=degree_SF(degree, graph,&m);
  
  for(i=0;i<m;i+=2){
    fprintf(stderr,"Attaching %05d\r",i);
    ii=sequence[i];
    jj=sequence[i+1];
    graph[ii].neigh[graph[ii].dynamic_degree++]=graph+jj;
    graph[jj].neigh[graph[jj].dynamic_degree++]=graph+ii;
  }
  
  free(sequence);
  for(i=0;i<N;i++){
    graph[i].neigh=(Node **) realloc(graph[i].neigh,sizeof(Node *)*graph[i].virtual_degree);
    if(graph[i].dynamic_degree!=graph[i].virtual_degree)
      fprintf(stderr,"\nError in make_netwrk_SF:\nImportant error: degrees do not coincide...\n\n");
  }
  
  return graph;
}

int *degree_SF(double degree, Node *graph, int *mm){
  int flag=0,j=0,aux=0,m=0, id, i, k, *sequence;
  double sum, *p, prob, c=0;
  j=1;
  do{
    c+=pow(j++,-degree);
  }while(j<N);
  c=1./c;
  j=0;
  
  p=(double *) malloc(sizeof(double)*N);

  k=0; sum=0; prob=0;
  for(i=0;i<N;i++){
    p[i]=RAND;
  }
  do{
    fprintf(stderr,"k=%d\r",k);
    k++;
    prob+=c*pow(k,-degree);
    for(i=0;i<N;i++)
      if(p[i]<prob && p[i]>=0){
	graph[i].virtual_degree=k;
	p[i]=-1;
	flag++;
	m+=k;
      }
  }while(flag<N);
    
  free(p);
  
  if(m%2!=0)
    graph[IRAND(N)].virtual_degree++;
  m++;
  sequence=(int *) malloc(sizeof(int)*m);
  for(i=0;i<m;i++)
    sequence[i]=-1;
  fprintf(stderr,"\nRandomizing m=%d...\n",m);
  
  for(i=0;i<m;i++){
    fprintf(stderr,"i=%d\r",i);
    do id=IRAND(m); while(sequence[id]!=-1);
    sequence[id]=j;
    aux++;
    if(aux>=graph[j].virtual_degree){
      j++;
      aux=0;
    }
  }
  *mm=m;
  return sequence;
}


Node *make_netwrk_BA(){
   Node *graph;
   int mink=BUFFER,maxk=0,aux[MBA], flag, i, j, k, t, tk=0, m=MBA, m0=M0, kmax=BUFFER;
   double p,l;
   
   graph=(Node *) malloc(sizeof(Node)*N);
   for(i=0;i<N;i++){
     graph[i].id=i;
     graph[i].neigh=(Node **) malloc(sizeof(Node *)*BUFFER);
     graph[i].pointer=graph+i;
     graph[i].virtual_degree=0;
     graph[i].n=1;
     graph[i].selected=false;
  } 
  
  //inititalization
  for(i=1;i<m0;i++){
    graph[i].neigh[graph[i].virtual_degree++]=graph+(i-1);
    graph[i-1].neigh[graph[i-1].virtual_degree++]=graph+i;
    tk+=2;
  }
  
  for(i=m0;i<N;i++)
    for(k=0;k<m;k++){
      do{
	p=RAND;
	flag=0;
	l=0;
	for(j=0;j<i;j++)
	  if(p<(l+=((double)graph[j].virtual_degree)/((double)tk))) break;
	for(t=0;t<k;t++) if(aux[t]==j) {flag=1; break;}
// 	if(graph[j].virtual_degree==KMAX) flag=1;
	if(flag==0){
	  graph[j].neigh[graph[j].virtual_degree++]=graph+i;
	  graph[i].neigh[graph[i].virtual_degree++]=graph+j;
	  tk+=2;
	  aux[k]=j;
	  fprintf(stderr,"Node %08d goes with %08d\r",i,j);
	  }
	}
	while (flag==1);
    }
  fprintf(stderr,"\n");
   
  for(i=0;i<N;i++){
    graph[i].neigh=(Node **) realloc(graph[i].neigh,sizeof(Node *)*graph[i].virtual_degree);
    graph[i].dynamic_degree=graph[i].virtual_degree;
    if(graph[i].virtual_degree>maxk)
      maxk=graph[i].virtual_degree;
    if(graph[i].virtual_degree<mink)
      mink=graph[i].virtual_degree;
  }
  fprintf(stderr,"Maximum degree = %d, minimum = %d\n", maxk, mink);
  return graph;
}

// int validate(Node *graph){
//   FILE *fout;
//   int *q, i,k;
//   double *p;
//   
//   q=(int *)malloc(sizeof(int)*KMAX);
//   p=(double *)malloc(sizeof(double)*KMAX);
//   
//   for(k=0;k<KMAX;k++)
//     q[k]=0;
//   
//   for(k=0;k<KMAX;k++)
//     for(i=0;i<N;i++)
//       if(graph[i].virtual_degree==k)
// 	q[k]++;
//       
//   fout=fopen("degree_distr","w");
//   for(k=0;k<KMAX;k++){
//     p[k]=((double) q[k])/((double) N);
//     fprintf(fout,"%d %.16G\n",k,p[k]);
//   }
// 
//   fclose(fout);free(q); free(p);
//   return 1;
// }

int distribution(Node *graph){
  int i,j;
  int *distr;
  int bins=500;
  
  FILE *foutd;
  foutd=fopen("distr.dat","w");
  distr=(int *) malloc(sizeof(int)*bins);
  for(i=0;i<bins;i++)
    distr[i]=0;
  for(i=0;i<N;i++){
    j=graph[i].virtual_degree;
    j=(j>=bins ? bins-1 : j);
    distr[j]++;
  }
  for(i=0;i<bins;i++)
    fprintf(foutd,"%d %d\n",i,distr[i]);
  free(distr);
  fclose(foutd);
  return 1;
}

int print_network(Node *graph){
  int i,j;
  FILE *fout;
  
  fout=fopen("network.txt","w");
  for(i=0;i<N;i++)
    for(j=0;j<graph[i].virtual_degree;j++)
      if(i<graph[i].neigh[j]->id)
	fprintf(fout,"%d %d\n", i, graph[i].neigh[j]->id);
  
  fclose(fout);
  return 1;
}

Node *read_net(char *name){
  int i,j,k, n=0, edges=0;
  FILE *fin;
  Node *graph;
  
  if((fin=fopen(name,"r"))==NULL){
    fprintf(stderr,"%s not found!\n",name);
    return NULL;
  }
  else{
    fscanf(fin,"%d\n",&N);
    fprintf(stderr,"\nNetwork size = %d\n",N);
    printf("NETWORK DATA:\n");
    printf("Network name: %s\n",name);
    printf("Network size = %d\n",N);
  }
  
  graph=(Node *) malloc(sizeof(Node)*N);
  for(i=0;i<N;i++){
     graph[i].id=i;
     graph[i].neigh=(Node **) malloc(sizeof(Node *)*BUFFER);
     graph[i].pointer=graph+i;
     graph[i].virtual_degree=0;
     graph[i].n=1;
     graph[i].selected=false;
  } 
  
  while(fscanf(fin,"%d %d\n",&i, &j)!=EOF){
//     i=i-1;j=j-1;
    fprintf(stderr,"reading %06d %06d\r",i,j);
    for(k=0;k<graph[i].virtual_degree;k++)//check (and skip) repeated edges
      if(graph[i].neigh[k]->id==graph[j].id)
	break;
    if(k==graph[i].virtual_degree){
      graph[i].neigh[graph[i].virtual_degree++]=graph+j;
      graph[j].neigh[graph[j].virtual_degree++]=graph+i;
      edges++;
    }
    if(graph[i].virtual_degree>BUFFER || graph[j].virtual_degree > BUFFER){
      fprintf(stderr,"Error overflow in buffer...\n");
      return NULL;
    }
    if(i>n)
      n=i;
    if(j>n)
      n=j;
  }
  int csi=0;
  for(i=0;i<N;i++){
    graph[i].neigh=(Node **) realloc(graph[i].neigh,sizeof(Node *)*graph[i].virtual_degree);
    if(graph[i].virtual_degree==0){
      fprintf(stderr,"NOTE: There are %d singleton nodes\r",csi++);
    }
    graph[i].dynamic_degree=graph[i].virtual_degree;
  }
  printf("Edges: %d\n",edges);
  printf("Single nodes: %d\n",csi);
  fprintf(stderr,"\n");
  fclose(fin);
  return graph;
}

#define OUT_NAME "network_.txt"
#define IN_NAME "Movements-Date-Size.dat"

int read_data(){
  FILE *fin, *fout;
  int x0, y0, x1, y1, size=0, numb,i,j, *coord_x, *coord_y;
  char *line, *s;
  size_t len;
  
  coord_x=(int *) malloc(sizeof(int)*BUFFER1);
  coord_y=(int *) malloc(sizeof(int)*BUFFER1);
  s=(char *) malloc(sizeof(char)*20);
  
  if((fin=fopen(IN_NAME,"r"))==NULL){
    fprintf(stderr,"ERROR: Error reading %s\n", IN_NAME);
    return -1;
  }
  if((fout=fopen(OUT_NAME,"w"))==NULL){
    fprintf(stderr,"ERROR: Error creating %s\n", OUT_NAME);
    return -1;
  }
  
  fprintf(stderr,"Parsing file %s to %s\n",IN_NAME, OUT_NAME);
//   (numb=fscanf(fin,"%d %d %d %d %s %*d\n", &x0,&y0,&x1,&y1,s))!=EOF
  
  while(getline(&line, &len, fin)!=-1){
    numb=sscanf(line,"%d %d %d %d %s %*d\n", &x0,&y0,&x1,&y1,s);
//     fprintf(stderr,"numb = %d\n",numb);
    if(numb==5 && filter(s)){
      for(i=0;i<size;i++){
	if(x0==coord_x[i] && y0==coord_y[i])
	  break;
      }
      if(i==size){
	coord_x[size]=x0;
	coord_y[size++]=y0;
      }
	
      for(j=0;j<size;j++){
	if(x1==coord_x[j] && y1==coord_y[j])
	  break;
      }
      if(j==size){
	coord_x[size]=x1;
	coord_y[size++]=y1;
      }
      if(size>BUFFER1){
	fprintf(stderr,"ERROR in read_data: Buffer overflow...\n");
	return -1;
      }
      fprintf(fout,"%d %d\n",i,j);
    }
  }
  fprintf(stderr,"File parsed. Size = %d\n",size);
  
  free(coord_x);free(coord_y); free(s);
  fclose(fin);fclose(fout);
  return 1;
  
}

bool filter(char *s){
  int i, dd,yy;
  char *mm, *token;
  
  mm=(char *) malloc(sizeof(char)*20);
  
  FILE *fin;
  s=strcat(s,";");
  
  token = strtok(s, " /\0\n");
  dd=atoi(token);
  
  token = strtok(NULL, "/");
  strcpy(mm,token);
  
  token = strtok(NULL, ";");
  yy=atoi(token);
  
  if(yy==6 /*&& !strcmp(mm,"Apr")*/){
    free(mm);
    return true;
  }
  free(mm);
  return false;
}
