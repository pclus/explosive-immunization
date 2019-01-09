#include "exploimmun.h"

Node *graph;

int main(int argc, char **argv){
//----------------------------------------------
/*	 INITIALIZATION 	*/
//----------------------------------------------
	if(argc < 3){
		fprintf(stderr,"USAGE: ./exploimmun <Number of candidates> <Network file>\n");
		return -1;
	}
	kk=atoi(argv[1]); 		//number of candidates
	if(kk<1){
		fprintf(stderr,"ERROR: Insert a valid number of candidates\n");
		return -1;
	}
	if( read_network(argv[2]) == -1 ){
		return -1;
	}
	eff_thr=6; 					// effective degree cut-off
	effective_degree(graph); 			// effective degree computation
	root=(int *) malloc(sizeof(int)*BUFFER);	//needed for the computation of scores, auxiliar.
	initialize_rng(time(0));
	double threshold=1./sqrt(N); 	//approx. percolation threshold where to start sigma2
	struct timeval time_begin, time_end;
//----------------------------------------------
/*	 COMPUTING SIGMA 1	*/
//----------------------------------------------
	gettimeofday(&time_begin, NULL);  
	explosive_immunization(threshold,1,0);
	gettimeofday(&time_end, NULL);
	double elapsed_time_sigma1=(time_end.tv_sec-time_begin.tv_sec)+(time_end.tv_usec-time_begin.tv_usec)*1.0e-6;
//----------------------------------------------
/*	 COMPUTING SIGMA 2	*/
//----------------------------------------------
	int nn=reset_net(graph);
        gettimeofday(&time_begin, NULL);
        explosive_immunization(threshold,2,nn);
        gettimeofday(&time_end, NULL);
	double elapsed_time_sigma2=(time_end.tv_sec-time_begin.tv_sec)+(time_end.tv_usec-time_begin.tv_usec)*1.0e-6;



//----------------------------------------------
/*	 PRINT TIMES	*/
//----------------------------------------------
 	fprintf(stderr, "\nComputation time:\n\tscore 1: %.2g [seconds]\n\tscore 2: %.2g [seconds]\n", elapsed_time_sigma1, elapsed_time_sigma2);

//----------------------------------------------
/*	 FINISH	*/
//----------------------------------------------
	int i;
	for(i=0;i<N;i++)
	    free(graph[i].neigh);
	free(graph);
	free(root);
	return 1;
}

/* Reads the input network file */
int read_network(char *namefile){
	int i;
	fprintf(stderr,"Reading network file: %s\n\n",namefile);
	if((graph=read_net(namefile))==NULL){
		fprintf(stderr,"ERROR: File %s not found\n",namefile);
		fprintf(stderr,"ERROR: program breaking\n");
		return -1;
	}

	for(i=0;i<N;i++)
	    graph[i].n=0;

	return 1;
}

/* Body of the method */
int explosive_immunization(double threshold, int sigma, int init){
	int id, i, j,id_min, imax, nnodes, id_aux, flag=(sigma==1 ? 0 : 1), largest=0;
	int *selected=malloc(sizeof(int)*kk);
	double score, score_min, stilde, q;
	double g=newman_ziff(graph, &largest, &stilde);
	FILE *fout=(sigma==1 ? fopen("output_sigma1.dat","w") : fopen("output_sigma2.dat","w"));

	fprintf(stderr,"\nStarting sigma %d, threshold=%.5g\n",sigma, threshold);
	for(nnodes=init;nnodes<N;nnodes++){	
		q=1-nnodes*1./N;
		fprintf(fout,"%lf %lf %lf\n",q,g, sqrt(stilde));
		fprintf(stderr,"%07d G(%01.05f) = %.5f    \r",nnodes, q, g);
		score_min=N;
		id=0;
		imax=((N-nnodes)<kk? N-nnodes: kk); //number of candidates min(kk, N-nnodes) nodes
		for(i=0;i<imax;i++){ // select the candidates
			
			if(imax<kk){ do{ id++;} while(graph[id].n==1);}
			else{
				do{ id=IRAND(N);} 
		     		while(graph[id].selected==true || graph[id].n==1);
			}
			selected[i]=id;
			graph[id].selected=true;
			score=(sigma==1 ? count_sigma1(graph, id) : count_sigma2(graph,id)); // choose proper score
			if(score<score_min){
				score_min=score;
				id_min=id;
			}
		}
		graph[id_min].n=1;
		g=newman_ziff_back(graph, &largest, graph+id_min, &stilde);
		for(i=0;i<((N-nnodes)<kk? N-nnodes: kk);i++) graph[selected[i]].selected=false;
		if((g>threshold) && flag==0){
			flag=1;
			if(print_threshold_conditions(q)==-1){
				fclose(fout);
				free(selected);
				return -1;
			}
		}
	}
	fclose(fout);
	free(selected);
	fprintf(stderr,"\n sigma %d finished\n",sigma);
	return 1;
}

/* Stores the nodes vaccinated at the percolation threshold */
int print_threshold_conditions(double q){
	fprintf(stderr,"\nPrinting threshold conditions...\n");
	int i;
	FILE *fic=fopen("threshold_conditions.dat", "w");
	if(fic==NULL){fprintf(stderr,"ERROR in printing ic: bad file\n"); return -1;}
	for(i=0;i<N;i++){
		fprintf(fic, "%d %d\n", i, graph[i].n);
	}
	fclose(fic);


	fprintf(stderr,"Done!\n");
	return 1;
}
