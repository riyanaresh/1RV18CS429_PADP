#include<math.h>
#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void p0_set_input(int *input1,int * input2);
void p0_send_input(int input1,int input2);
void p0_receive_output(int *output1,int* output2);
int p1_receive_input();
int p1_compute_output(int input1);
void p1_send_output(int output1);
int p2_receive_input();
void p2_send_output(int output2);

int p2_compute_output(int input2);
void timestamp();

int main(int argc,char ** argv){
	int id,ierr,input1,input2,output1,output2,p;
	double wtime;

	ierr=MPI_Init(&argc,&argv);

	if(ierr!=0){
		printf("\nMPI Multitask fatal error\nMPI_Init returned non zero error");
		exit(1);
	}
	ierr=MPI_Comm_rank(MPI_COMM_WORLD,&id);
	ierr=MPI_Comm_size(MPI_COMM_WORLD,&p);

	if(p<3){
		printf("\nMPI_MULTITASK FATAL ERROR \n number of processes must be atleast three\n");
		ierr=MPI_Finalize();
		exit(1);
	}

	if(id==0){
		timestamp();
		printf("MPI Multitask:\n C / MPI Version\n");

		wtime=MPI_Wtime();
		p0_set_input(&input1,&input2);
		p0_send_input(input1,input2);
		p0_receive_output(&output1,&output2);

		wtime=MPI_Wtime()-wtime;

		printf("Process 0 time=%g\n",wtime);

		ierr=MPI_Finalize();

		printf("MPI Multitask:\n Normal end of execution\n");

		timestamp();


	}

	else if (id==1){
		wtime=MPI_Wtime();
		input1=p1_receive_input();
		output1=p1_compute_output(input1);
		p1_send_output(output1);
		wtime=MPI_Wtime()-wtime;
		printf("Process 1 time = %g\n",wtime);
		ierr=MPI_Finalize();
	}

	else if (id==2){
		wtime=MPI_Wtime();
		input2=p2_receive_input();
		output2=p2_compute_output(input2);
		p2_send_output(output2);

		wtime=MPI_Wtime()-wtime;
		printf("Process 2 time = %g\n",wtime);

		ierr=MPI_Finalize();
	}

	return 0;


}


void p0_set_input(int *input1,int * input2)
{
	*input1=10;
	*input2=1000;

	printf("\n P0 set parameters\ninput1=%d input2=%d",*input1,*input2);

	return;

}


void p0_send_input(int input1,int input2){
	int id,tag;
	id=1;
	tag=1;
	MPI_Send(&input1,1,MPI_INT,id,tag,MPI_COMM_WORLD);

	id=2;
	tag=2;
	MPI_Send(&input2,1,MPI_INT,id,tag,MPI_COMM_WORLD);

	return;
}


void p0_receive_output(int *output1,int* output2){
	int output,output_received,source;
	MPI_Status status;

	output_received=0;

	while(output_received<2){
		MPI_Recv(&output,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);

		source=status.MPI_SOURCE;

		if(source==1){
			*output1=output;
		}
		else{
			*output2=output;
		}

		output_received=output_received+1;

	}
	printf("\nprocess 1 returned output=%d\nprocess2 returned output=%d\n",*output1,*output2);
	return;
}

int p1_receive_input(){
	int id,input1,tag;
	MPI_Status status;
	MPI_Recv(&input1,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);

	return input1;
}



int p1_compute_output(int input1){
	int i=0,ans=1;
	for(i=1;i<=input1;i++){
		ans*=i;
	}
	return ans;
}

void p1_send_output(int output1){
	int id,tag;
	id=0;tag=3;

	MPI_Send(&output1,1,MPI_INT,id,tag,MPI_COMM_WORLD);
	return;
}


int p2_receive_input(){
	int id,tag,input2;
	MPI_Status status;

	id=0;tag=2;

	MPI_Recv(&input2,1,MPI_INT,id,tag,MPI_COMM_WORLD,&status);

	return input2;
}



int p2_compute_output(int input2){
	int i=0,ans=0;
	for(i=1;i<=input2;i++){
		ans+=i;
	}
	return ans;
}

void p2_send_output(int output2){

	int id,tag;
	id=0;tag=4;
	MPI_Send(&output2,1,MPI_INT,id,tag,MPI_COMM_WORLD);
	return ;
}


void timestamp(){

	static char time_buffer[40];
	const struct tm *tm;
	time_t now;

	now=time(NULL);


	tm=localtime(&now);

	strftime(time_buffer,40,"%d %B %Y %I:%M:%S %p",tm);

	printf("%s\n",time_buffer);

	return;
}



