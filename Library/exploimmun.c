#include "exploimmun.h"

Node *graph;

int main(int argc, char **argv){
//----------------------------------------------
	/* INITIALIZATION */
	root=(int *) malloc(sizeof(int)*BUFFER); //needed for the computation of scores, auxiliar.
	//N=30000;
	//make_netwrk_ER_expercol("explosive_percolation.dat");
	//make_netwrk_ER_percol("random_percolation.dat");
	//return -1;
	//make_network();
	read_network(argv[2]);
	//distribution(graph); 
	double threshold=1./sqrt(N); //threshold two start sigma2
	kk=atoi(argv[1]); //number of candidates
	eff_thr=6; // effective degree cut-off
	effective_degree(graph); // effective degree computation
//----------------------------------------------

	struct timeval time_begin, time_end;
	gettimeofday(&time_begin, NULL);  
	explosive_sigma1(threshold);
	gettimeofday(&time_end, NULL);
	double elapsed_time=(time_end.tv_sec-time_begin.tv_sec)+(time_end.tv_usec-time_begin.tv_usec)*1.0e-6;
	FILE *ftime=fopen("times.dat","a");
 	fprintf(ftime, "%d %.16g\n", kk, elapsed_time);
	fclose(ftime);

	/* FINISH */
	int i;
	for(i=0;i<N;i++)
	    free(graph[i].neigh);
	free(graph);
	free(root);
	return 1;
}


int make_network(){
	make_netwrk_ER();
	return 1;
}

int read_network(char *namefile){
	char *route=(char *)malloc(sizeof(char)*90);
	int i;
	fprintf(stderr,"Read existing network (must be stored in 'NetworkData' folder)\n");
	//fprintf(stderr,"Network name: ");
	//fscanf(stdin,"%s",namefile);
	sprintf(route,"NetworkData/%s",namefile);
	if((graph=read_net(route))==NULL){
		fprintf(stderr,"ERROR: Probably %s doesn't exist...\n",route);
		fprintf(stderr,"ERROR: program breaking\n");
		free(namefile); free(route);
		return -1;
	}
	free(route);
	for(i=0;i<N;i++)
	    graph[i].n=0;
	return 1;
}

int explosive_sigma1(double threshold){
	double score, score_min, stilde, q;
	int id, i, j,id_min, imax, nnodes, id_aux, flag=0, largest=0;
	int *selected=malloc(sizeof(int)*kk);
	FILE *fout=fopen("output.dat","w");
	double g=newman_ziff(graph, &largest, &stilde);
	fprintf(stderr,"\nStarting sigma1 %.16g\n",threshold);
for(nnodes=0;nnodes<N;nnodes++)
{	
	q=1-nnodes*1./N;
	fprintf(fout,"%lf %lf %lf\n",q,g, sqrt(stilde));
	fprintf(stderr,"%07d G(%01.05f) = %.5f    \r",nnodes, q, g);
	score_min=N;
	id=0;
	imax=((N-nnodes)<kk? N-nnodes: kk); //number of candidates min(kk, N-nnodes) nodes
	for(i=0;i<imax;i++) // select the candidates
	{	
		if(imax<kk){ do{ id++;} while(graph[id].n==1);}
		else{
			do{ id=IRAND(N);} 
	     		while(graph[id].selected==true || graph[id].n==1);
		}
		selected[i]=id;
		graph[id].selected=true;
		score=graph[id].effective_degree+count_sigma1(graph, id); //sigma1
		if(score<score_min){
			score_min=score;
			id_min=id;
		}
	}
	graph[id_min].n=1;
	g=newman_ziff_back(graph, &largest, graph+id_min, &stilde);
	for(i=0;i<((N-nnodes)<kk? N-nnodes: kk);i++) graph[selected[i]].selected=false;
	if((g>threshold) && flag==0)
		{flag=1; if(print_threshold_conditions(q)==-1) return -1;}
}
fclose(fout);
free(selected);
fprintf(stderr,"\nDone!\n");
return 1;
}


int print_threshold_conditions(double q){
	fprintf(stderr,"\nPrinting threshold conditions...\n");
	int i;
	FILE *fic=fopen("initial_conditions/ic.dat", "w");
	if(fic==NULL){fprintf(stderr,"ERROR in printing ic: bad file\n"); return -1;}
	for(i=0;i<N;i++){
	fprintf(fic, "%d %d\n", i, graph[i].n);
	}
	fclose(fic);

	FILE *fth=fopen("thresholds.dat", "a");
	fprintf(fth,"%d %.16g\n",kk, q);
	fclose(fth);

	fprintf(stderr,"Done!\n");
	return 1;
}
