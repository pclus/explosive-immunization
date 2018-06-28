#include "exploimmun.h"

Node *graph;
// This is a stupid comment
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

	double threshold=1./sqrt(N); 	//threshold two start sigma2
	struct timeval time_begin, time_end;
//----------------------------------------------
/*	 COMPUTING 	*/
//----------------------------------------------
	gettimeofday(&time_begin, NULL);  
	explosive_sigma1(threshold);
	gettimeofday(&time_end, NULL);

//----------------------------------------------
/*	 PRINT TIMES	*/
//----------------------------------------------
	double elapsed_time=(time_end.tv_sec-time_begin.tv_sec)+(time_end.tv_usec-time_begin.tv_usec)*1.0e-6;
	FILE *ftime=fopen("times.dat","a");
 	fprintf(ftime, "%d %.16g\n", kk, elapsed_time);
	fclose(ftime);

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


int read_network(char *namefile){
	int i;
	fprintf(stderr,"Looking for %s network...\n",namefile);
	if((graph=read_net(namefile))==NULL){
		fprintf(stderr,"ERROR: File %s not found\n",namefile);
		fprintf(stderr,"ERROR: program breaking\n");
		return -1;
	}
	fprintf(stderr,"done!\n");

	for(i=0;i<N;i++)
	    graph[i].n=0;

	return 1;
}

int explosive_sigma1(double threshold){
	int id, i, j,id_min, imax, nnodes, id_aux, flag=0, largest=0;
	int *selected=malloc(sizeof(int)*kk);
	double score, score_min, stilde, q;
	double g=newman_ziff(graph, &largest, &stilde);
	FILE *fout=fopen("output.dat","w");

	fprintf(stderr,"\nStarting sigma1 %.16g\n",threshold);
	for(nnodes=0;nnodes<N;nnodes++){	
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
			score=graph[id].effective_degree+count_sigma1(graph, id); //sigma1
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
	fprintf(stderr,"\nDone!\n");
	return 1;
}


int print_threshold_conditions(double q){
	fprintf(stderr,"\nPrinting threshold conditions...\n");
	int i;
	FILE *fic=fopen("threshold_conditions.dat", "w");
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
