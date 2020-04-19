#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <string>

using namespace std;

#define TEXT "I know everything about you ;)"

void sendMessageInBuffer(int bufferSize, int sendTo, const char* message, int rank);
//bool sendNumberSynchronous(int rank, int size,bool chekProcess);


int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv); 

	MPI_Status status;
	int rank, size;
	int numberForMux = 3;
	int numberFromConsole,chekProcessNumber = 0;

	const char* message = TEXT;
	int bufferSize = strlen(message) + 1;
	char readMessage[31];
	


	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (size <= 3) {

		cout << "Few processes" << endl;
	}
	else {

		switch (rank) {

		case 0:
			//1
			MPI_Send(&numberForMux, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

			while (numberForMux < 924)
			{
				MPI_Recv(&numberForMux, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
				if (numberForMux < 924)
				{
					cout << "NumberForMux = " << numberForMux << " Rank = " << rank << endl;
					numberForMux *= 3;
					MPI_Send(&numberForMux, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
				}
			}
			//2
			sendMessageInBuffer(bufferSize, 1, TEXT,rank);
			//3
			
			cout << "Enter number = ";
			cin >> numberFromConsole;
			cout << endl;

			if (numberFromConsole > 5) {

				chekProcessNumber = 0;
				MPI_Ssend(&chekProcessNumber, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
				MPI_Ssend(&chekProcessNumber, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
				MPI_Ssend(&chekProcessNumber, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);

				MPI_Ssend(&numberFromConsole, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
				MPI_Ssend(&numberFromConsole, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
				cout << "Number > 5 " << "message send rank 1 and 3" << endl;



			}
			else {
				chekProcessNumber = 1;
				MPI_Ssend(&numberFromConsole, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
				MPI_Ssend(&chekProcessNumber, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
				MPI_Ssend(&numberFromConsole, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
				MPI_Ssend(&chekProcessNumber, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
				cout << "Number < 5 " << "message send rank 2" << endl;
				
				
			}
			break;

		case 1:

			//1
			while (numberForMux < 924) {
				MPI_Recv(&numberForMux, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

				if (numberForMux < 924) {
					cout << "NumberForMux = " << numberForMux << " Rank = " << rank << endl;
					numberForMux *= 3;
					MPI_Send(&numberForMux, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
				}
			}

			//2
			MPI_Recv(&readMessage, bufferSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
			sendMessageInBuffer(bufferSize, 2, readMessage,rank);
			//3
			MPI_Recv(&chekProcessNumber, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

			if (chekProcessNumber == 0) {
				MPI_Recv(&numberFromConsole, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
				cout << "Rank= " << rank << "; Message= " << numberFromConsole << endl;
			}

			break;
		case 2:
			//2
			MPI_Recv(&readMessage, bufferSize, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
			sendMessageInBuffer(bufferSize, 3, readMessage, rank);
			//3
			MPI_Recv(&chekProcessNumber, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

			if (chekProcessNumber == 1) {
				MPI_Recv(&numberFromConsole, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
				cout << "Rank= " << rank << "; Message= " << numberFromConsole << endl;
			}

			break;
		case 3:
			//2
			MPI_Recv(&readMessage, bufferSize, MPI_CHAR, 2, 0, MPI_COMM_WORLD, &status);
			cout << "Message from buffer = " << readMessage << endl;
			//3
			MPI_Recv(&chekProcessNumber, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

			if (chekProcessNumber == 0) {
				MPI_Recv(&numberFromConsole, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
				cout << "Rank= " << rank << "; Message= " << numberFromConsole << endl;
			}

			break;

		default:
			cout << "Rank = " << rank << " empty" << endl;
			break;
		}
	}
	

	MPI_Finalize();
	return 0;
}

void sendMessageInBuffer(int bufferSize,int sendTo,const char* message,int rank) {

	vector<char> buff(bufferSize + MPI_BSEND_OVERHEAD);
	char* pBuff = buff.data();
	MPI_Buffer_attach(pBuff, bufferSize + MPI_BSEND_OVERHEAD);
	char* strbuff = (char*)message;
	MPI_Bsend(strbuff, bufferSize, MPI_CHAR, sendTo, 0, MPI_COMM_WORLD);
	MPI_Buffer_detach(&pBuff, &bufferSize);
	//cout << "I sent message in buffer ,my rank = "<<rank<<endl;
	return;
}

