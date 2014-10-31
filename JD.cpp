// JD@.cpp 
#include "stdafx.h"
#include "MT.h"	//ZkEcCX^

//csvt@CÇÝÝ
void Filehandle(const char *filename, double csvData[][VEC]){
	ifstream csvFile(filename);
	string str;
	stringstream ss;
	double *Data = new double[DATA*VEC];

	for (int row = 0; row < DATA; row++){
		for (int col = 0; col < VEC - 1; col++){
			getline(csvFile.seekg(0, ios_base::cur), str, ',');
			ss.str(str);
			ss >> Data[VEC * row + col];

			ss.str("");
			ss.clear(stringstream::goodbit);
		}
		//ÅãÌ1ñÍæØê¶ªá¤½ßAÊ
		getline(csvFile.seekg(0, ios_base::cur), str, '\n');

		ss.str(str);
		ss >> Data[VEC * row + VEC - 1];
		ss.str("");
		ss.clear(stringstream::goodbit);
	}
	csvFile.close();

	//½³zñÖÌÚs[u
	for (int row = 0; row < DATA; row++){
		for (int col = 0; col < VEC; col++){
			csvData[row][col] = Data[VEC * row + col];
			cout << csvData[row][col] << "\t";
		}
		cout << endl;
	}
	delete[]Data;
}
//f[^«Ý
void datawrite(){}

//xNgÔÌ£ªè
double Distance(double F[][VEC], int i, double G[][VEC],int j){//½³zñÆ£ðvZµ½¢Y¦ðãüBÔÁ¿á¯KvÈñÂÌzñ¾¯nµ½¢
	double D = 0;
	for (int k = 0; k < VEC; k++){
		D += pow((F[i][k] - G[j][k]), 2);
	}
	return  sqrt(D);
}

//¶¬	(0~(A-1))Ì_
inline unsigned long  makerandom(int A){
	return (unsigned)(genrand_int32() % A);
}

//NX^úlÌÝè(k-means++@)
void getcluster(double F[][VEC],  int *CLNO, double centro[][VEC]){
	//_ÉÅÌNX^Sð`CX
	int First = makerandom(DATA);
	CLNO[First] = 10000;
	cout << "_úNX^" << First << "ªIÎêÜµ½" << endl << endl;

	for (int i = 0; i < VEC; i++)
		centro[0][i] = F[First][i];//údSðãü

	//cèÌúNX^SðIÔ(NX^-1Â)
	for (int i = 1; i < nclusters; i++){
		double L = 0;//NX^»èl()
		double *R = new double[DATA];//£Ìi[É

		for (int j = 0; j < DATA; j++){
			R[j] = Distance(F, j, centro,i-1);
			L += pow(R[j], 2);
		}
		while (1){
			double luck = genrand_res53()*L;//[0,1)ðìÁÄA0~LÌÉ
			double Judge = 0;
			int j = 0;
			do{
				Judge += pow(R[j], 2);
				j++;
			} while (Judge < luck);
			//m¦ÅÌàÌðIoB·ÅÉIÎêÄ¢½çAâè¼µ
			if ((CLNO[j] != 10000)&&(j<DATA)){
				CLNO[j] = 10000;
				for (int col = 0; col < VEC; col++)
				centro[i][col] = F[j][col];

				break;
			}
		}
		delete[] R;
	}
	//\¦
	cout << "-----FIRST CETRAL-----" << endl;
	for (int i = 0; i < nclusters; i++){
		cout << "CL : " << i << endl;		
				for (int col = 0; col < VEC; col++){
					cout << centro[i][col] << "\t";
				}
				cout << endl;
	}
}
//NX^ª¯
void Divide(double csvdata[][VEC],  int *CLNO, double centro[][VEC]){
	for (int j = 0; j < DATA; j++){
		double R ,Rmin = 0;
		for (int i = 0; i < nclusters; i++){
			R = Distance(csvdata, j, centro, i);
			
			if ((i==0)||(Rmin>=R)){
				Rmin = R;
				CLNO[j] = i;
			}
		}
	}
	cout << "\n------NEW DIVSION------" << endl;
	cout << "DATA NO\t\tDIVISION" << endl;
	for (int j = 0; j < DATA; j++)
		cout << j + 1 << "\t\t" << CLNO[j] << endl;
}

//NX^SÌXV
int Center(double csvdata[][VEC], int *CLNO, double centro[][VEC]){
	//û©»èp
	int flag = 0;
	double check;

	for (int i = 0; i < nclusters; i++){
		double sum = 0;
		for (int j = 0; j < VEC; j++){
			check = centro[i][j];
			double sum = 0;
			int count = 0;
			for (int k = 0; k < DATA; k++){
				if (CLNO[k] == i) {
					sum += csvdata[k][j];
					count++;
				}
			}
			double average = sum / (double)count;
			centro[i][j] = average;

			//û©»è
			if ((fabs(check - centro[i][j]) <= CONVERGENCE)&&flag==0) flag = 0;
			else flag = 1; 
		}
	}
	//û©»è
	if (flag == 0)	return 0;

	cout << "\n-----NEW CENTRAL----- " << endl;
	for (int i = 0; i < nclusters; i++){
		for (int j = 0; j < VEC; j++){
			cout << centro[i][j]<<"\t\t";
		}
		cout << endl;
	}
	return 1;
}

//kmeans@
void Kmeans(double csvdata[][VEC],  int *CLNO, double centro[][VEC], int *Nj){
	while (1){
		Divide(csvdata,  CLNO, centro);
		if (Center(csvdata,  CLNO, centro) == 0)	break;
	}
	//eNX^ÉÜÜêévfÌÂÌZo
	for (int i = 0; i < nclusters; i++){
		Nj[i] = 0;
		for (int j = 0; j < DATA; j++){
			if (CLNO[j] == i)	Nj[i]++;
		}
	}

	cout << "\n------FINAL DIVSION------" << endl;
	cout << "DATA NO\t\tDIVISION" << endl;
	for (int j = 0; j < DATA; j++)	cout << j + 1 << "\t\t" << CLNO[j] << endl;

	cout << "\n-----NUMBER OF CLUSTER-----" << endl;
	cout << "NLNO\t\tNUMBER" << endl;
	for (int i = 0; i < nclusters; i++)	cout << i << "\t\t" << Nj[i] << endl;
}

//JD@(JDlÌZoANX^OÌ]¿)
void JD(double csvdata[][VEC], int *CLNO, double centro[][VEC], int *Nj){
	double p = 0;//JDvalue
	//ÄÌZo(NX^dS¯mÌ£ÌZo)
	double Ä[nclusters][nclusters];
	for (int i = 0; i < nclusters; i++){
		for (int j = 0; j < nclusters; j++){
			Ä[i][j] = Distance(centro, i, centro, j);
		}
	}

	//ÅÌZo(NX^dSÆevfÆÌ£Ì½Ï)
	double Å[nclusters];
	for (int j = 0; j < nclusters; j++){
		Å[j] = 0;//ú»
		for (int k = 0; k < DATA; k++){
			if (CLNO[k] == j){
				Å[j] += Distance(csvdata, k, centro, j);
			}
		}
		Å[j] /= Nj[j];
	}

	//JDvalueÌZo
	for (int i = 0; i < nclusters; i++){
		double max = 0,r;
		for (int j = 0; j < nclusters; j++){
			if (i != j){
				r = (Å[i] + Å[j]) / Ä[i][j];
				if (max <= r){
					max = r;
				}
			}
		}
		p += max;
	}
	p /= nclusters;
	cout << p << endl;
}


int main(){
	init_genrand((unsigned)time(NULL));//ú»

	int CLNO[DATA];//®µÄ¢éNX^ÔÛB
	for (int i = 0; i < DATA; i++){
		CLNO[DATA] = 0;
	}
	double centro[nclusters][VEC];//NX^dS

	//csvf[^ÇÝÝ
	double csvdata[DATA][VEC];
	Filehandle("C:\\Users\\1004196\\Documents\\Visual Studio 2013\\Projects\\JD@\\input\\input.csv", csvdata);

	//úNX^ÌÝè(kmeans++@)
	getcluster(csvdata, CLNO,centro);

	int Nj[nclusters];//NX^\¬vfÌÛÇ
	//kmeans@
	Kmeans(csvdata, CLNO, centro, Nj);

	//JD@
	JD(csvdata, CLNO, centro, Nj);

	//f[^«Ý
	//datawrite();

	return 0;
}
