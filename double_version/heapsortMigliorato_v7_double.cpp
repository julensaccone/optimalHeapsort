#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include <iomanip>
#include <ctime>

using namespace std;

int OptimalHeapsortComparisons, BasicHeapsortComparisons;

bool checkInputValid(int arraySize, int minValueOfArray, int maxValueOfArray, int numberOfArraysToGenerate);
int isheap_ric(vector<double> &a, int i, int j, int *index1, int *index2);
int isheap(vector<double> &A, int size, int *index1, int *index2);
void randomVectorGenerator(vector<double> &A, int size, double minValueOfArray, double maxValueOfArray);
void printFinalArray(vector<double> &V);
void printInputArray(vector<double> &a, int b);
void printAnalysis(double optimalHeapsortDuration, double basicHeapsortDuration, int optimalHeapsortComparisons, int basicHeapsortComparisons);
void printAverageAnalysis(int numberOfArraysToGenerate, int arraySize, int minValueOfArray, int maxValueOfArray, double sumOfDurationOptimalHeapsort, double sumOfDurationBasicHeapsort, int sumOfComparisonOptimalHeapsort, int sumOfComparisonBasicHeapsort);

// funzione per rendere più costosi i confronti
void timeWaster();

void rearrange(vector<double> &B, int firstTreeNodeIndex, int lastTreeNodeIndex);
void createHeap(vector<double> &B);
void basicHeapsort(vector<double> &B);

int findGreaterChild(vector<double> &A, int leftChild, int j);
void heapify(vector<double> &A, int firstTreeNodeIndex, int lastTreeNodeIndex);
void buildHeap(vector<double> &A);
void moveUpGreaterChildTillHalfTree(vector<double> &A, int &currentTreeNodeIndex, int treeHeight);
void moveUpLastLeaf(vector<double> &A, int vacantNodeIndex, int lastLeaf, int vacantNodeFatherIndex);
void rebuildHeap(vector<double> &A, int currentTreeNodeIndex, int lastTreeNodeIndex, int &treeHeight);
void optimalHeapsort(vector<double> &A);

int main()
{
    int arraySize, number, numberOfArraysToGenerate = 10, sumOfComparisonOptimalHeapsort = 0, sumOfComparisonBasicHeapsort = 0, dots;
    vector<double> A, B;
    double minValueOfArray, maxValueOfArray, OptimalHeapsortDuration, BasicHeapsortDuration, sumOfDurationOptimalHeapsort = 0, sumOfDurationBasicHeapsort = 0;
    clock_t startAlgorithmTimestamp, endAlgorithmTimestamp;
    bool print = true;

    system("clear");

    cout << "========== SORTING PROGRAM ==========" << endl;
    cout << "Array's dimension: ";
    cin >> arraySize;
    cout << "Minimum possible value assumable by elements of the array: ";
    cin >> minValueOfArray;
    cout << "Maximum possible value assumable by elements of the array: ";
    cin >> maxValueOfArray;
    cout << "Number of array to generate (for execution time analysis): ";
    cin >> numberOfArraysToGenerate;

    if (!checkInputValid(arraySize, minValueOfArray, maxValueOfArray, numberOfArraysToGenerate))
        return 0;

    if (arraySize >= 50000 || numberOfArraysToGenerate >= 100)
    {
        print = false;
        dots = numberOfArraysToGenerate / 3;
        cout << "\n\nCalculating...";
    }

    for (int i = 0; i < numberOfArraysToGenerate; i++)
    {
        A.clear();
        randomVectorGenerator(A, arraySize, minValueOfArray, maxValueOfArray);
        B = A;
        OptimalHeapsortComparisons = 0;
        BasicHeapsortComparisons = 0;

        if (print)
            printInputArray(A, 0);

        /*
        CON LA LIBRERIA CHRONO E LA FUNZIONE high_resolution_clock::now() MISURO IL WALL TIME, CIOÈ IL TEMPO EFFETTIVO:
            auto startAlgorithmTimestamp = std::chrono::high_resolution_clock::now();
            optimalHeapsort(A);
            auto endAlgorithmTimestamp = std::chrono::high_resolution_clock::now();
            OptimalHeapsortDuration = double(endAlgorithmTimestamp - startAlgorithmTimestamp);

            startAlgorithmTimestamp = std::chrono::high_resolution_clock::now();
            basicHeapsort(B);
            endAlgorithmTimestamp = std::chrono::high_resolution_clock::now();
            BasicHeapsortDuration = double(endAlgorithmTimestamp - startAlgorithmTimestamp);

        INVECE CON LA LIBRERIA TIME.H E LA FUNZIONE clock() MISURO IL CPU TIME, CIOÈ IL TEMPO IN CUI L'ALGORITMO UTILIZZA LA CPU
        QUINDI SENZA I TEMPI DI I/O, IL CHE MI SEMBRA PIÙ RAPPRESENTATIVO.
        Per misurare i tempi in entrambi i casi si prende il timestamp prima e dopo dell'operazione che di cui si vuole misurare la durata,
        in questo caso la chiamata della funzione in cui svolge l'algoritmo, e in fine si sottraggono i tempi (end - start).
        Per il CPU TIME con time.h bisogna dividere il risultato della sottrazione per il numero di clock per secondo,
        contenuti nella costante CLOCKS_PER_SEC.
        */
        startAlgorithmTimestamp = clock();
        optimalHeapsort(A);
        endAlgorithmTimestamp = clock();
        OptimalHeapsortDuration = double(endAlgorithmTimestamp - startAlgorithmTimestamp) / CLOCKS_PER_SEC;
        sumOfDurationOptimalHeapsort += OptimalHeapsortDuration;

        if (print)
        {
            printFinalArray(A);
            printInputArray(B, 1);
        }

        startAlgorithmTimestamp = clock();
        basicHeapsort(B);
        endAlgorithmTimestamp = clock();
        BasicHeapsortDuration = double(endAlgorithmTimestamp - startAlgorithmTimestamp) / CLOCKS_PER_SEC;
        sumOfDurationBasicHeapsort += BasicHeapsortDuration;

        if (print)
        {
            printFinalArray(B);
            printAnalysis(OptimalHeapsortDuration, BasicHeapsortDuration, OptimalHeapsortComparisons, BasicHeapsortComparisons);
        }
        sumOfComparisonOptimalHeapsort += OptimalHeapsortComparisons;
        sumOfComparisonBasicHeapsort += BasicHeapsortComparisons;

        if (!print && (i == 0 || i == dots || i == dots * 2))
            cout << "\n...";
    }

    printAverageAnalysis(numberOfArraysToGenerate, arraySize, minValueOfArray, maxValueOfArray, sumOfDurationOptimalHeapsort, sumOfDurationBasicHeapsort, sumOfComparisonOptimalHeapsort, sumOfComparisonBasicHeapsort);

    return 0;
}

bool checkInputValid(int arraySize, int minValueOfArray, int maxValueOfArray, int numberOfArraysToGenerate)
{

    if (arraySize < 0)
    {
        cout << "\n\nWrong Input!\nArray's dimension can't be negative!" << endl;
        return false;
    }
    else if (arraySize == 0)
    {
        cout << "\n\nWrong Input!\nIt does not make sense to sort an array of 0 elements!" << endl;
        return false;
    }
    else if (arraySize == 1)
    {
        cout << "\n\nWrong Input!\nAn array of dimension 1 is already sorted!" << endl;
        return false;
    }

    if (minValueOfArray > maxValueOfArray)
    {
        cout << "\n\nWrong Input!\nMinimum value assumable must be greater than maximum value assumable!" << endl;
        return false;
    }

    if (numberOfArraysToGenerate <= 0)
    {
        cout << "\n\nWrong Input!\nThe number of arrays to generate must be positive!" << endl;
        return false;
    }

    return true;
}
//****************************************************************************************************************************

int isheap_ric(vector<double> &a, int i, int j, int *index1, int *index2)
{
    int dx = 2 * i + 1; // indice del figlio sx di i
    // se i è una foglia
    if (2 * i + 1 >= j)
        return 1;
    // se ha un figlio con indice maggiorre
    if (a[dx] > a[i] || (dx + 1 < j && a[dx + 1] > a[i]))
    {
        *index1 = i;
        *index2 = dx;
        return 0;
    }

    return isheap_ric(a, dx, j, index1, index2) && isheap_ric(a, dx + 1, j, index1, index2);
}

int isheap(vector<double> &A, int size, int *index1, int *index2)
{
    return isheap_ric(A, 0, size, index1, index2);
}

bool isSorted(vector<double> &A, int *index)
{
    for (int i = 0; i < A.size() - 1; i++)
        if (A[i] > A[i + 1])
        {
            *index = i;
            return false;
        }
    return true;
}

/**
 * @brief Genera randomicamente tanti elementi quanto indicato dal parametro 'size' e li mette nel vettore.
 *
 * @param size dimensione dell'array (quindi quanti elementi generare).
 * @param minValueOfArray minimo valore assumibile dagli elementi nell'array.
 * @param maxValueOfArray massimo valore assumibile dagli elementi dell'array.
 */
void randomVectorGenerator(vector<double> &A, int size, double minValueOfArray, double maxValueOfArray)
{
    // First create an instance of an engine.
    random_device rnd_device;
    // Specify the engine and distribution.
    mt19937 mersenne_engine{rnd_device()}; // Generates random integers
    // uniform_int_distribution<int> dist{minValueOfArray, maxValueOfArray};
    uniform_real_distribution<double> dist(minValueOfArray, maxValueOfArray);

    auto gen = [&dist, &mersenne_engine]()
    {
        return dist(mersenne_engine);
    };
    A.resize(size);
    generate(begin(A), end(A), gen);
}

void printFinalArray(vector<double> &V)
{
    int index;

    cout << endl
         << "Final Array: " << endl;
    if (isSorted(V, &index))
    {
        for (int i = 0; i < V.size(); i++)
        {
            cout << "   " << V[i];
        }
    }
    else
    {
        for (int i = 0; i <= index + 5; i++)
        {
            cout << "   " << V[i];
        }
    }

    if (isSorted(V, &index))
        cout << endl
             << "Array is sorted!" << endl;
    else
    {
        cout << endl
             << "Array is not sorted!" << endl;
        cout << "A[" << index - 1 << "] = " << V[index - 1] << endl;
        cout << "A[" << index << "] = " << V[index] << endl;
        cout << "A[" << index + 1 << "] = " << V[index + 1] << endl;
    }
}

void printInputArray(vector<double> &a, int b)
{
    if (!b)
    {
        cout << endl
             << endl
             << "/===========================================================/\n####WITH OPTIMAL HEAPSORT####";
    }
    else
    {
        cout << endl
             << "_________________________________________________\n####WITH BASIC HEAPSORT####";
    }

    cout << endl
         << "Array in input: " << endl;
    for (int i = 0; i < a.size(); i++)
    {
        cout << "   " << a[i];
    }
    cout << endl;
}

void printAnalysis(double optimalHeapsortDuration, double basicHeapsortDuration, int optimalHeapsortComparisons, int basicHeapsortComparisons)
{
    cout << endl
         << endl
         << "Time of execution for OptimalHeapsort: " << endl
         << fixed << setprecision(8) << optimalHeapsortDuration << " seconds" << endl;
    cout << optimalHeapsortDuration * 1000.0f << " milliseconds" << endl;
    cout << optimalHeapsortDuration * 1000000.0f << " microseconds" << endl
         << endl;

    // STAMPO UN'ANALISI DEL TEMPO DI ESECUZIONE DI BASIC HEAPSORT IN VARIE UNITÀ DI MISURA
    cout << "Time of execution with basic Heapsort: " << endl
         << fixed << setprecision(8) << basicHeapsortDuration << " seconds" << endl;
    cout << basicHeapsortDuration * 1000.0f << " milliseconds" << endl;
    cout << basicHeapsortDuration * 1000000.0f << " microseconds" << endl
         << "/===========================================================/" << endl
         << endl;

    cout << "Optimal Heapsort - total number of comparison: " << optimalHeapsortComparisons << endl;
    cout << endl
         << "Basic Heapsort - total number of comparison: " << basicHeapsortComparisons << endl;
}

void printAverageAnalysis(int numberOfArraysToGenerate, int arraySize, int minValueOfArray, int maxValueOfArray, double sumOfDurationOptimalHeapsort, double sumOfDurationBasicHeapsort, int sumOfComparisonOptimalHeapsort, int sumOfComparisonBasicHeapsort)
{
    double averageExecutionTimeOptimalHeapsort = sumOfDurationOptimalHeapsort / (double)numberOfArraysToGenerate;
    double averageExecutionTimeBasicHeapsort = sumOfDurationBasicHeapsort / (double)numberOfArraysToGenerate;

    int averageComparisonsOptimalHeapsort = sumOfComparisonOptimalHeapsort / numberOfArraysToGenerate;
    int averageComparisonsBasicHeapsort = sumOfComparisonBasicHeapsort / numberOfArraysToGenerate;

    cout << endl
         << endl
         << "^^^^^^^^^^^^^^^^^^^ AVERAGE EXECUTION TIME ANALYSIS ^^^^^^^^^^^^^^^^^^^";
    cout << endl
         << "Generating " << numberOfArraysToGenerate << " arrays of dimension " << arraySize << ", with minimum value assumable by elements " << minValueOfArray << " and maximum " << maxValueOfArray << endl
         << "####WITH OPTIMAL HEAPSORT:####\nthe average execution time is: " << endl
         << averageExecutionTimeOptimalHeapsort << " seconds" << endl;
    cout << averageExecutionTimeOptimalHeapsort * 1000.0f << " milliseconds" << endl;
    cout << averageExecutionTimeOptimalHeapsort * 1000000.0f << " microseconds" << endl
         << endl;

    cout << "\n####WITH BASIC HEAPSORT:####\nthe average execution time is: \n"
         << averageExecutionTimeBasicHeapsort << " seconds" << endl;
    cout << averageExecutionTimeBasicHeapsort * 1000.0f << " milliseconds" << endl;
    cout << averageExecutionTimeBasicHeapsort * 1000000.0f << " microseconds" << endl
         << endl;

    cout << "([basicHeapsort exTime] / [OptimalHeapsort exTime]) ratio = " << averageExecutionTimeBasicHeapsort / averageExecutionTimeOptimalHeapsort << endl
         << endl;

    cout << "Optimal Heapsort - average number of comparisons: " << averageComparisonsOptimalHeapsort << endl;
    cout << "Basic Heapsort - average number of comparisons: " << averageComparisonsBasicHeapsort << endl
         << endl
         << endl;

    if (averageComparisonsOptimalHeapsort < averageComparisonsBasicHeapsort)
        cout << "In conclusion, the Optimal Heapsort does less comparison than the Basic Heapsort" << endl;
    if ((averageExecutionTimeBasicHeapsort / averageExecutionTimeOptimalHeapsort) > 1)
        cout << "and Optimal Heapsort is " << fixed << setprecision(1) << (averageExecutionTimeBasicHeapsort / averageExecutionTimeOptimalHeapsort) << " times faster than the Basic Heapsort!!!" << endl;
}

//#########################################################################################

void timeWaster()
{
    int var = 0;
    for (int i = 0; i <= 1000; i++)
        var++;
}

void rearrange(vector<double> &B, int firstTreeNodeIndex, int lastTreeNodeIndex)
{
    int currentIndexNode = firstTreeNodeIndex, greaterChild;
    double NodeToPlace = B[currentIndexNode];

    bool stillToPlace = true;
    do
    {
        int leftChild = 2 * currentIndexNode + 1;
        if (leftChild >= lastTreeNodeIndex)
            stillToPlace = false;
        else
        {
            int rightChild = leftChild + 1;
            greaterChild = leftChild;
            if (rightChild < lastTreeNodeIndex)
            {
                ////timeWaster();
                BasicHeapsortComparisons++;
                if (B[rightChild] > B[leftChild])
                    greaterChild = rightChild;
            }
            // timeWaster();
            BasicHeapsortComparisons++;
            if (B[greaterChild] > NodeToPlace)
            {
                B[currentIndexNode] = B[greaterChild];
                currentIndexNode = greaterChild;
            }
            else
            {
                stillToPlace = false;
            }
        }

    } while (stillToPlace);
    B[currentIndexNode] = NodeToPlace;
}
void createHeap(vector<double> &B)
{
    int arraySize = B.size();
    for (int i = floor(arraySize / 2); i >= 0; i--)
        rearrange(B, i, arraySize);
}
void basicHeapsort(vector<double> &B)
{
    double temp;

    createHeap(B);

    for (int i = B.size() - 1; i >= 1; i--)
    {
        temp = B[0];
        B[0] = B[i];
        B[i] = temp;

        rearrange(B, 0, i);
    }
}

//============================================================================================================================

int findGreaterChild(vector<double> &A, int leftChild, int j)
{
    int rightChild = leftChild + 1;
    if (rightChild <= j)
    {
        // timeWaster();
        OptimalHeapsortComparisons++;
        if (A[rightChild] > A[leftChild])
            return rightChild;
    }
    return leftChild;
}

void heapify(vector<double> &A, int firstTreeNodeIndex, int lastTreeNodeIndex)
{
    int greaterChild, leftChild, rightChild;
    double NodeToPlace = A[firstTreeNodeIndex];
    bool flag = true;

    do
    {
        leftChild = (2 * firstTreeNodeIndex) + 1;
        if (leftChild > lastTreeNodeIndex)
            flag = false;
        else
        {
            greaterChild = findGreaterChild(A, leftChild, lastTreeNodeIndex);
            // timeWaster();
            OptimalHeapsortComparisons++;
            if (A[greaterChild] > NodeToPlace)
            {
                A[firstTreeNodeIndex] = A[greaterChild];
                firstTreeNodeIndex = greaterChild;
            }
            else
                flag = false;
        }
    } while (flag);
    A[firstTreeNodeIndex] = NodeToPlace;
}

void buildHeap(vector<double> &A)
{
    int n = A.size() - 1;
    for (int i = floor(n / 2); i >= 0; i--)
        heapify(A, i, n);
}

void moveUpGreaterChildTillHalfTree(vector<double> &A, int &currentTreeNodeIndex, int treeHeight)
{
    int greaterChild, leftChild, rightChild;
    int count = 1;
    while (count <= floor(treeHeight / 2))
    {
        leftChild = (2 * currentTreeNodeIndex) + 1;
        rightChild = leftChild + 1;
        // timeWaster();
        OptimalHeapsortComparisons++;
        greaterChild = (A[leftChild] > A[rightChild]) ? leftChild : rightChild;
        A[currentTreeNodeIndex] = A[greaterChild];
        currentTreeNodeIndex = greaterChild;
        count++;
    }
}

void moveUpLastLeaf(vector<double> &A, int vacantNodeIndex, int lastLeaf, int vacantNodeFatherIndex)
{
    // timeWaster();
    OptimalHeapsortComparisons++;
    while (A[lastLeaf] > A[vacantNodeFatherIndex])
    {
        A[vacantNodeIndex] = A[vacantNodeFatherIndex];
        vacantNodeIndex = vacantNodeFatherIndex;
        vacantNodeFatherIndex = floor((vacantNodeIndex - 1) / 2);
        // timeWaster();
        OptimalHeapsortComparisons++;
    }
    A[vacantNodeIndex] = A[lastLeaf];
}

void rebuildHeap(vector<double> &A, int currentTreeNodeIndex, int lastTreeNodeIndex, int &treeHeight)
{
    bool repeat = true;
    int greaterChild, vacantNodeFatherIndex, leftChild, lastLeaf = lastTreeNodeIndex + 1;
    do
    {
        if (treeHeight == 1)
        {
            A[currentTreeNodeIndex] = A[lastLeaf];
            heapify(A, currentTreeNodeIndex, lastTreeNodeIndex);
            return;
        }
        else
        {
            moveUpGreaterChildTillHalfTree(A, currentTreeNodeIndex, treeHeight);
        }

        // timeWaster();
        OptimalHeapsortComparisons++;
        vacantNodeFatherIndex = floor((currentTreeNodeIndex - 1) / 2);
        if (A[lastLeaf] >= A[vacantNodeFatherIndex])
        {
            moveUpLastLeaf(A, currentTreeNodeIndex, lastLeaf, vacantNodeFatherIndex);
            repeat = false;
        }
        else
        {
            treeHeight -= floor(treeHeight / 2);
        }
    } while (repeat);
}

/**
 * @brief Ordina un array utilizzando un Heapsort migliorato.
 */
void optimalHeapsort(vector<double> &A)
{
    int firstTreeNodeIndex, treeHeight, lastTreeNodeIndex;
    double temp;

    buildHeap(A);

    for (int j = A.size() - 1; j >= 2; j--)
    {
        temp = A[0];
        treeHeight = floor(log2(j));
        firstTreeNodeIndex = 0;
        lastTreeNodeIndex = j - 1;
        rebuildHeap(A, firstTreeNodeIndex, lastTreeNodeIndex, treeHeight);
        A[j] = temp;
    }
    temp = A[0];
    A[0] = A[1];
    A[1] = temp;
}
