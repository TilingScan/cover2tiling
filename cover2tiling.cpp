/*************************************************************************/
/*           C O V E R 2 T I L I N G . C P P  V. 0.1                     */
/*************************************************************************/
/*                                                                       */
/*        Convert coverage file to TilingScan file format.               */
/*           http://github.com/TilingScan/cover2tiling                   */
/*                                                                       */
/*************************************************************************/
/* J. M. Juanes, A. Miguel, L. J. Morales,  J. E. Pérez-Ortín, V. Arnau  */
/*************************************************************************/
/*          By : V. Arnau & J.M. Juanes -  21 - April - 2015             */
/*************************************************************************/

#include <iostream>
using namespace std;

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  uchar  unsigned char
#define  LINEA      200
#define  SOSTENIDO   35

const double min_value = 0.005;

//Define the structures
struct Chromosome;
struct Hits;

//Typedefs
typedef Chromosome * Chrs;
typedef Hits * Hit ;

//Structure Chromosome
struct Chromosome
{
	string name;
	int num;
	Hit hit;
	Chrs sig;
};

//Structure Hits
struct Hits
{
	int nuc;
	double val;
	Hit sig;
};

//Start the array
void ArrayInit(int pos[], int val[], int start, int N)
{
	for(int i = 0; i < N; i++)
	{
		//Starts the position
		pos[i] = start + i;
		
		//Starts the nucleotide value
		val[i] = 0;
	}
}

//Search in the array
void ArraySearchInsert(int pos[], int val[], int n_pos, int n_val, int N)
{
	//Loop
	for(int i = 0; i < N; i++)
	{
		//Search
		if(pos[i] == n_pos)
		{
			//Insert
			val[i] = n_val;
			
			//Break
			break;
		}
	}
}

//Calculate the average
double Average(int val[], int N)
{
	//Vars
	int sum = 0;
	double av;
	
	//Loop
	for(int i = 0; i < N; i++)
	{
		//Increases the sum
		sum = sum + val[i];
	}
	
	//Calculates the average
	av = (double) sum/N;
	
	//Warning if sum = 0
	if(av < min_value)
	{
		av = min_value;
	}
	
	//Return
	return av;
}

//Reads the coverage file
void ReadCoverage(string file, int N, Chrs & l)
{
	//Vars
	ifstream entrada;
	string str_aux, str_read = "";
	Chrs chr, chr_fin;
	Hit hit, hit_fin;
	int start = -1, get_pos, get_cov;
	
	//Array
	int arr_pos[N], arr_val[N];
	
	//Start the list of chromosomes
	l = NULL;
	chr_fin = NULL;
	
	//Open the file
	entrada.open(file.c_str());
	
	//Read all the file
	while(!entrada.eof())
	{
		//Gets the name
		entrada >> str_aux;
		
		//Check white space
		if(str_aux == "")
		{			
			//Continue
			continue;
		}
		
		//Check if is a new chromosome
		if(str_aux != str_read)
		{
			//If is not the first time
			if(start != -1)
			{
				//Ends the hit
				hit = new Hits;
				hit->nuc = (int) start + N/2;
				hit->val = Average(arr_val, N);
				hit->sig = NULL;
				
				//Save
				if(hit_fin == NULL)
				{
					chr_fin->hit = hit;
				}
				else
				{
					hit_fin->sig = hit;
				}
				
				//Increase the counter
				//chr_fin->num = chr_fin->num + 1;
			}
			
			//Create a new chromosome
			chr = new Chromosome;
			chr->name = str_aux;
			chr->num = 1;
			chr->hit = NULL;
			
			//Check if is the first
			if(chr_fin == NULL)
			{
				//First
				l = chr;
			}
			else
			{
				//Not the first
				chr_fin->sig = chr;
			}
			
			//Get the next
			chr_fin = chr;
			
			//Restart the hits
			hit_fin = NULL;
			
			//Restart the counters
			start = -1;
			
			//Restart the str
			str_read = str_aux;
		}
		
		//Gets the position - nucleotide - coverage - base read - quality
		entrada >> get_pos >> str_aux >> get_cov >> str_aux >> str_aux;
		//entrada >> get_pos >> get_cov;
		
		//Check if start is defined
		if(start == -1)
		{
			//Save the start
			start = get_pos;
			//start = 0;
			
			//Initialize the array
			ArrayInit(arr_pos, arr_val, start, N);
		}
		//Check if we have exceed the start + N
		if(get_pos >= start + N)
		{
			//Finish
			while(get_pos >= start + N)
			{
				//Next hit
				hit = new Hits;
				hit->nuc = (int) start + N/2;
				hit->val = Average(arr_val, N);
				hit->sig = NULL;
				
				//Save
				if(hit_fin == NULL)
				{
					chr_fin->hit = hit;
				}
				else
				{
					hit_fin->sig = hit;
				}
				
				//Get it nog
				hit_fin = hit;	
				
				//Increase the counter
				chr_fin->num = chr_fin->num + 1;			
				
				//Next
				start = start + N;
				ArrayInit(arr_pos, arr_val, start, N);
			}
		}
		
		//Insert the position
		ArraySearchInsert(arr_pos, arr_val, get_pos, get_cov, N);
		
		//Clear
		str_aux = "";		
	}
	
	//Save the last
	hit = new Hits;
	hit->nuc = (int) start + N/2;
	hit->val = Average(arr_val, N);
	hit->sig = NULL;
	
	//Save
	if(hit_fin == NULL)
	{
		chr_fin->hit = hit;
	}
	else
	{
		hit_fin->sig = hit;
	}
	
	//Ends the chromosomes list
	chr_fin->sig = NULL;
	
	//Close the file
	entrada.close();
}

//Save a file with the TilingScan format
void SaveFile(string file, Chrs l)
{
	//Vars
	ofstream salida;
	Chrs c;
	Hit h;
	int cont = 1;
	
	//Open the file
	salida.open(file.c_str());
	
	//Save the headers
	salida << "# File generated with Cover2Tiling" << endl;
	salida << "# http://github.com/TilingScan/cover2tiling" << endl;
	
	//Starts the list
	c = l;
	
	//Read all chromosomes
	while(c != NULL)
	{
		//New line
		salida << endl;
		
		//Save the chromosome header
		salida << "# Sequence " << cont << endl;
		salida << "# Name	" << c->name << endl;
		salida << "# Number Of Hits	" << c->num << endl;
		salida << endl;
		
		//Hits
		h = c->hit;
		
		//Read all hits
		while(h != NULL)
		{
			//Save
			salida << h->nuc << "	" << h->val << endl;
			
			//Next hit
			h = h->sig;
		}
		
		//Next chromosome
		c = c->sig;

		//Update cont
		cont = cont + 1;
	}
	
	//Close the file
	salida.close();
	
	//Success!
	cout << "File " << file << " is successfully generated!" << endl << endl;
}

//Check constant N
int CheckN(string c)
{
	//Convert string to int
	int n = atoi(c.c_str());
	
	//First, check if n is > 0
	if(n <= 0)
	{
		//Show error
		cout << "Error: please, set N > 0" << endl << endl;
		system("pause");
		exit(0);
	}
	else
	{
		//Chech if n is odd
		int m = (int) n/2;
		if(2*m == n)
		{
			//Update n
			n = n + 1;
			
			//Show warning
			cout << "Warning: We have changed N to " << n << " because it must be odd." << endl << endl;
		}
	}
	
	//Return n
	return n;
}


//Check if file exists
void FileExists(string file)
{
	//File
	ifstream entrada;
	
	//Open the file
	entrada.open(file.c_str());
	
	//Check if file exists
	if(!entrada)
	{
		//Show warning
		cout << "Fatal error: " << file << " doesn't exists." << endl << endl;
		system("pause");
		exit(0);
	}
	
	//Close file
	entrada.close();
}

//Check number of argumnets
void CheckArguments(int n)
{
	//Check if the number of arguments are > 4
	if(n < 4)
	{
		//Shows error
		cout << "Fatal error: invalid arguments." << endl << endl;
		cout << "How to use:" << endl << endl;
		cout << "cover2tiling  cover_file  tiling_file N" << endl << endl;
		cout << "Where:" << endl;
		cout << "cover_file  : Output file of samtools mpileup comand." << endl;
		cout << "tiling_file : Output file to used in http://tilingscan.uv.es/." << endl;
		cout << "N           : Average nucleotides for each hit. It must be odd." << endl << endl;
		
		//Wait
		system("pause");
		
		//Exit
		exit(0);
	}
}

// Prints the header
void PrintHeader(void)
{
	cout << endl;
	cout << "========================================================" << endl;
	cout << "=                     COVER2TILING                     =" << endl;
	cout << "=       http://github.com/TilingScan/cover2tiling      =" << endl;
	cout << "========================================================" << endl;
	cout << "=       V. Arnau & JM Juanes   ====   21-IV-2015       =" << endl;
	cout << "=           UNIVERSIDAD DE VALENCIA (SPAIN)            =" << endl;
	cout << "========================================================" << endl << endl;
}

// Main function
int main(int argc,char **argv)
{
	//Number of nucleotrides
	int N;
	
	//List of chromosomes
	Chrs l;
		
	//Show the header
	PrintHeader();
	
	//Check the arguments
	CheckArguments(argc);
	
	//Check if coverage file exists :	 
	FileExists(argv[1]);
	
	//Check constant N:
	N = CheckN(argv[3]);
	
	//Read the input file
	ReadCoverage(argv[1], N, l);
	
	//Save to a file
	SaveFile(argv[2], l);
	
	//Exit
	return 0;
}
