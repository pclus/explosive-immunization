#ifndef NETWORKS_H
#define NETWORKS_H
        #include "networks.h"
#endif



Node *read_net(char *name){
	int i,j,k, n=0, edges=0;
	FILE *fin;
	Node *graph;
	
	if((fin=fopen(name,"r"))==NULL){
		fprintf(stderr,"ERROR (networks.c/read_net): %s not found!\n",name);
	 	return NULL;
	}
	else{
		fscanf(fin,"%d\n",&N);
		fprintf(stderr,"\nNetwork size = %d\n",N);
		fprintf(stdout,"NETWORK DATA:\n");
		fprintf(stdout,"Network name: %s\n",name);
		fprintf(stdout,"Network size = %d\n",N);
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
		fprintf(stderr,"reading %06d %06d\r",i,j);
		for(k=0;k<graph[i].virtual_degree;k++)		//check (and skip) repeated edges
			if(graph[i].neigh[k]->id==graph[j].id)
		    		break;
		if(k==graph[i].virtual_degree){
			graph[i].neigh[graph[i].virtual_degree++]=graph+j;
			graph[j].neigh[graph[j].virtual_degree++]=graph+i;
			edges++;
		}
		if(graph[i].virtual_degree>BUFFER || graph[j].virtual_degree > BUFFER){
			fprintf(stderr,"ERROR (networks.c/read_net): overflow in buffer...\n");
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
	fprintf(stdout,"Edges: %d\n",edges);
	fprintf(stdout,"Single nodes: %d\n",csi);
	fprintf(stderr,"\n");
	fclose(fin);
	return graph;
}
